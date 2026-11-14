"""Read a live Windows process identity without starting a command interpreter.

The launcher needs creation time and executable path before it can own a server.
PowerShell startup can exhaust an observation deadline even when that server is
healthy. These bindings query one process handle directly. They request only
query and synchronization rights; this module cannot terminate the process.
"""

from __future__ import annotations

import ctypes
from collections.abc import Callable, Iterator
from contextlib import contextmanager
from dataclasses import dataclass
from functools import lru_cache


@dataclass(frozen=True)
class WindowsProcessIdentity:
    """Cookie-compatible observations from one live process handle."""

    start: str
    command: str


class _FileTime(ctypes.Structure):
    """The Windows ABI uses two 32-bit words, including on 64-bit hosts."""

    _fields_ = [("low", ctypes.c_uint32), ("high", ctypes.c_uint32)]


@lru_cache(maxsize=1)
def _kernel32():
    """Bind exact native signatures once; all foreign values stay in this module.

    HANDLE must remain pointer-sized. ctypes otherwise assumes an integer return
    value and can truncate a handle on 64-bit Windows. Loading is lazy so other
    hosts can import the launcher without a Windows library.
    """

    kernel = ctypes.WinDLL("kernel32", use_last_error=True)
    kernel.OpenProcess.argtypes = [ctypes.c_uint32, ctypes.c_int32, ctypes.c_uint32]
    kernel.OpenProcess.restype = ctypes.c_void_p
    kernel.CloseHandle.argtypes = [ctypes.c_void_p]
    kernel.CloseHandle.restype = ctypes.c_int32
    kernel.WaitForSingleObject.argtypes = [ctypes.c_void_p, ctypes.c_uint32]
    kernel.WaitForSingleObject.restype = ctypes.c_uint32
    kernel.GetProcessTimes.argtypes = [ctypes.c_void_p] + [ctypes.POINTER(_FileTime)] * 4
    kernel.GetProcessTimes.restype = ctypes.c_int32
    kernel.QueryFullProcessImageNameW.argtypes = [
        ctypes.c_void_p, ctypes.c_uint32, ctypes.POINTER(ctypes.c_wchar),
        ctypes.POINTER(ctypes.c_uint32),
    ]
    kernel.QueryFullProcessImageNameW.restype = ctypes.c_int32
    return kernel


def process_identity(pid: int) -> WindowsProcessIdentity | None:
    """Return live creation/path observations, or None when either is unavailable.

    None is not proof of exit. The caller must retain its fail-closed ownership
    checks. An opened handle keeps both observations tied to the same process,
    even if Windows later reuses the numeric PID. Every opened handle is closed.

    GetProcessTimes uses 100-nanosecond units since 1601. Adding the epoch offset
    preserves the .NET ticks stored by the previous PowerShell observer. The
    image query requests the Win32 path spelling used by existing cookies.
    API contracts: https://learn.microsoft.com/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesstimes
    and https://learn.microsoft.com/windows/win32/api/winbase/nf-winbase-queryfullprocessimagenamew
    """

    if pid < 1 or pid > 0xFFFFFFFF:
        return None
    try:
        kernel = _kernel32()
    except (OSError, AttributeError):
        return None
    # PROCESS_QUERY_LIMITED_INFORMATION | SYNCHRONIZE. No termination authority.
    handle = kernel.OpenProcess(0x101000, False, pid)
    if not handle:
        return None
    try:
        # A process handle signals on exit. A zero timeout never blocks.
        if kernel.WaitForSingleObject(handle, 0) != 258:  # WAIT_TIMEOUT
            return None
        creation, exit_time, kernel_time, user_time = (_FileTime() for _ in range(4))
        if not kernel.GetProcessTimes(handle, ctypes.byref(creation),
                                      ctypes.byref(exit_time), ctypes.byref(kernel_time),
                                      ctypes.byref(user_time)):
            return None
        image = ctypes.create_unicode_buffer(32768)
        size = ctypes.c_uint32(len(image))
        if not kernel.QueryFullProcessImageNameW(handle, 0, image, ctypes.byref(size)):
            return None
        if size.value < 1 or size.value >= len(image) or not image.value:
            return None
        ticks = ((creation.high << 32) | creation.low) + 504911232000000000
        return WindowsProcessIdentity(f"windows-start-ticks:{ticks}",
                                      f"windows-path:{image.value}")
    except OSError:
        return None
    finally:
        kernel.CloseHandle(handle)


@contextmanager
def process_exit_observer(
    pid: int, expected_start: str,
) -> Iterator[Callable[[int], bool] | None]:
    """Hold one creation-verified process handle across a stop request.

    A successful taskkill request is not exit evidence. Only a signaled handle
    proves that this exact process exited, even if its PID is later reused.
    Access or identity failures yield no observer; every opened handle closes.
    """
    if pid < 1 or pid > 0xFFFFFFFF:
        yield None
        return
    try:
        kernel = _kernel32()
    except (OSError, AttributeError):
        yield None
        return
    try:
        handle = kernel.OpenProcess(0x101000, False, pid)
    except OSError:
        yield None
        return
    if not handle:
        yield None
        return
    try:
        creation, exit_time, kernel_time, user_time = (_FileTime() for _ in range(4))
        try:
            read = kernel.GetProcessTimes(
                handle, ctypes.byref(creation), ctypes.byref(exit_time),
                ctypes.byref(kernel_time), ctypes.byref(user_time),
            )
            ticks = ((creation.high << 32) | creation.low) + 504911232000000000
            valid = read and expected_start == f"windows-start-ticks:{ticks}"
        except OSError:
            valid = False
        if not valid:
            yield None
            return
        def exited(timeout_ms: int) -> bool:
            if timeout_ms < 0 or timeout_ms > 5000:
                raise ValueError("process exit observation must stay within five seconds")
            try:
                return kernel.WaitForSingleObject(handle, timeout_ms) == 0
            except OSError:
                return False
        yield exited
    finally:
        kernel.CloseHandle(handle)


def process_absent(pid: int) -> bool:
    """Prove absence only from a complete successful native PID snapshot.

    A full buffer may be truncated, so neither it nor an API/access failure
    authorizes cleanup. This bounded observation never matches names or signals
    a process. K32EnumProcesses documents completeness through returned byte size:
    https://learn.microsoft.com/windows/win32/api/psapi/nf-psapi-enumprocesses
    """
    if pid < 1 or pid > 0xFFFFFFFF:
        return False
    try:
        kernel = _kernel32()
        enumerate_processes = kernel.K32EnumProcesses
        enumerate_processes.argtypes = [ctypes.POINTER(ctypes.c_uint32), ctypes.c_uint32,
                                       ctypes.POINTER(ctypes.c_uint32)]
        enumerate_processes.restype = ctypes.c_int32
        processes = (ctypes.c_uint32 * 65536)()
        capacity = ctypes.sizeof(processes)
        used = ctypes.c_uint32()
        if not enumerate_processes(processes, capacity, ctypes.byref(used)):
            return False
        if used.value == 0 or used.value >= capacity or used.value % ctypes.sizeof(ctypes.c_uint32):
            return False
        return pid not in processes[:used.value // ctypes.sizeof(ctypes.c_uint32)]
    except (OSError, AttributeError):
        return False

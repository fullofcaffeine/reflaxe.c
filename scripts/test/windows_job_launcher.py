"""Bind a test server and its descendants to this launcher's lifetime on Windows.

A launcher that exits before readiness can leave grandchildren behind. At
that point taskkill cannot walk the missing parent. Before starting the
requested command, join a fresh Windows job with kill-on-close enabled.
Only this process owns the job handle. Windows closes it on every exit,
including termination by the test runner.
"""

import ctypes
from ctypes import wintypes
import subprocess
import sys


# Native layouts for SetInformationJobObject, from the Windows SDK's winnt.h.
# https://learn.microsoft.com/windows/win32/api/winnt/ns-winnt-jobobject_extended_limit_information
JOB_OBJECT_EXTENDED_LIMIT_INFORMATION = 9
JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE = 0x2000


class BasicLimits(ctypes.Structure):
    _fields_ = [
        ("PerProcessUserTimeLimit", ctypes.c_longlong),
        ("PerJobUserTimeLimit", ctypes.c_longlong),
        ("LimitFlags", wintypes.DWORD),
        ("MinimumWorkingSetSize", ctypes.c_size_t),
        ("MaximumWorkingSetSize", ctypes.c_size_t),
        ("ActiveProcessLimit", wintypes.DWORD),
        ("Affinity", ctypes.c_size_t),
        ("PriorityClass", wintypes.DWORD),
        ("SchedulingClass", wintypes.DWORD),
    ]


class ExtendedLimits(ctypes.Structure):
    # JOBOBJECT_EXTENDED_LIMIT_INFORMATION contains six 64-bit IO counters
    # followed by four SIZE_T values. All limits except kill-on-close are zero.
    _fields_ = [
        ("BasicLimitInformation", BasicLimits),
        ("IoInfo", ctypes.c_ulonglong * 6),
        ("MemoryLimits", ctypes.c_size_t * 4),
    ]


def main(command: list[str]) -> int:
    kernel = ctypes.WinDLL("kernel32", use_last_error=True)
    kernel.CreateJobObjectW.argtypes = [wintypes.LPVOID, wintypes.LPCWSTR]
    kernel.CreateJobObjectW.restype = wintypes.HANDLE
    kernel.SetInformationJobObject.argtypes = [
        wintypes.HANDLE, ctypes.c_int, wintypes.LPVOID, wintypes.DWORD,
    ]
    kernel.AssignProcessToJobObject.argtypes = [wintypes.HANDLE, wintypes.HANDLE]
    kernel.GetCurrentProcess.argtypes = []
    kernel.GetCurrentProcess.restype = wintypes.HANDLE
    # NULL security attributes give the job a non-inheritable handle. Leave
    # it open until process exit, otherwise this launcher also gets stopped.
    job = kernel.CreateJobObjectW(None, None)
    if not job:
        raise ctypes.WinError(ctypes.get_last_error())
    limits = ExtendedLimits()
    limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
    if not kernel.SetInformationJobObject(
        job, JOB_OBJECT_EXTENDED_LIMIT_INFORMATION, ctypes.byref(limits), ctypes.sizeof(limits)
    ):
        raise ctypes.WinError(ctypes.get_last_error())
    if not kernel.AssignProcessToJobObject(job, kernel.GetCurrentProcess()):
        raise ctypes.WinError(ctypes.get_last_error())
    return subprocess.call(command, stdout=sys.stdout, stderr=sys.stderr)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))

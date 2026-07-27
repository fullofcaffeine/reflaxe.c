#!/usr/bin/env python3
"""Own one exact, loopback-only Haxe compilation server for Caxecraft.

The server caches Haxe's parsing and typing work between requests. It does not
cache haxe.c's ``CompilationContext``, HxcIR, CAST, or generated project: those
remain fresh for every request. This module owns only process lifecycle and a
small compatibility cookie so a stale or foreign process can never be mistaken
for the compiler selected by this checkout.

Automatic mode may start, reuse, restart, and stop a process whose exact PID
and process-start identity still match its cookie. Explicit-attach mode is
different: it borrows an operator-provided endpoint and never kills it.
"""

from __future__ import annotations

import json
import os
import re
import signal
import socket
import subprocess
import sys
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Mapping, Sequence

from dev_build_state import atomic_write_state, canonical_digest
from dev_generation import VariantLock


COOKIE_KIND = "caxecraft-owned-haxe-server"
COOKIE_SCHEMA_VERSION = 1
LOOPBACK = "127.0.0.1"
ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent


class HaxeServerFailure(RuntimeError):
    """The requested server mode cannot be used without guessing ownership."""


@dataclass(frozen=True)
class HaxeInstallation:
    """One exact compiler installation selected by this checkout's `.haxerc`."""

    version: str
    compiler: Path
    standard_library: Path
    haxelib_repository: Path
    neko_library: Path | None


@dataclass(frozen=True)
class HaxeServerConnection:
    """One endpoint paired with the exact compiler installation it serves."""

    endpoint: str
    installation: HaxeInstallation


@dataclass(frozen=True)
class HaxeServerLease:
    """One server connection and whether Caxecraft owns its process lifecycle."""

    connection: HaxeServerConnection
    owned: bool
    compatibility_sha256: str | None


def _development_tool(name: str) -> str:
    local_name = f"{name}.cmd" if os.name == "nt" else name
    local = ROOT / "node_modules/.bin" / local_name
    return str(local) if local.is_file() else name


def pinned_haxe_installation(
    *,
    scope_root: Path = ROOT,
    environment: Mapping[str, str] | None = None,
) -> HaxeInstallation:
    """Resolve the exact Lix/HaxeShim installation without using global Haxe."""

    source_environment = os.environ if environment is None else environment
    pin_path = scope_root / ".haxerc"
    try:
        pin: object = json.loads(pin_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise HaxeServerFailure(f"cannot read the Haxe pin {pin_path}: {error}") from error
    if not isinstance(pin, dict):
        raise HaxeServerFailure(".haxerc must contain a JSON object")
    version = pin.get("version")
    if (
        not isinstance(version, str)
        or re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9.+_-]*", version) is None
    ):
        raise HaxeServerFailure(".haxerc contains an unsafe or missing Haxe version")
    if pin.get("resolveLibs") != "scoped":
        raise HaxeServerFailure("Caxecraft requires .haxerc resolveLibs=scoped")

    haxe_root_text = source_environment.get("HAXE_ROOT") or source_environment.get(
        "HAXESHIM_ROOT"
    )
    if haxe_root_text is None:
        home_key = "APPDATA" if os.name == "nt" else "HOME"
        home = source_environment.get(home_key)
        if not home:
            raise HaxeServerFailure(
                f"cannot resolve the Lix Haxe installation: {home_key} is unset"
            )
        haxe_root = Path(home) / "haxe"
    else:
        haxe_root = Path(haxe_root_text)

    versions_root = (haxe_root / "versions").resolve()
    installation_root = (versions_root / version).resolve()
    if installation_root.parent != versions_root:
        raise HaxeServerFailure("the pinned Haxe version escaped the versions directory")
    executable_name = "haxe.exe" if os.name == "nt" else "haxe"
    compiler = installation_root / executable_name
    standard_library = installation_root / "std"
    if not compiler.is_file() or (os.name != "nt" and not os.access(compiler, os.X_OK)):
        raise HaxeServerFailure(
            f"pinned Haxe {version} is not installed at {compiler}; run npm ci"
        )
    if not standard_library.is_dir():
        raise HaxeServerFailure(
            f"pinned Haxe {version} has no standard library at {standard_library}"
        )

    neko_library: Path | None = None
    platform_path = installation_root / "platform.txt"
    if platform_path.is_file():
        try:
            platform = platform_path.read_text(encoding="utf-8").strip()
        except (OSError, UnicodeError) as error:
            raise HaxeServerFailure(
                f"cannot read pinned Haxe platform metadata: {error}"
            ) from error
        if re.fullmatch(r"[A-Za-z0-9_-]+", platform) is None:
            raise HaxeServerFailure("pinned Haxe platform metadata is malformed")
        candidate = haxe_root / "neko" / "versions" / f"2.4.0-{platform}"
        if candidate.is_dir():
            neko_library = candidate.resolve()

    return HaxeInstallation(
        version,
        compiler,
        standard_library,
        (haxe_root / "haxelib").resolve(),
        neko_library,
    )


def pinned_haxe_environment(
    locale: str, installation: HaxeInstallation
) -> dict[str, str]:
    """Reproduce HaxeShim's compiler environment for a direct native client."""

    environment = os.environ.copy()
    environment["LC_ALL"] = locale
    environment.pop("HAXE_NO_SERVER", None)
    environment.update(
        {
            "HAXE_STD_PATH": str(installation.standard_library),
            "HAXEPATH": str(installation.compiler.parent),
            "HAXELIB_PATH": str(installation.haxelib_repository),
            "HAXE_VERSION": installation.version,
        }
    )
    if installation.neko_library is not None:
        if os.name == "nt":
            library_key = "PATH"
        elif sys.platform == "darwin":
            library_key = "DYLD_LIBRARY_PATH"
        else:
            library_key = "LD_LIBRARY_PATH"
        library_path = str(installation.neko_library)
        entries = environment.get(library_key, "").split(os.pathsep)
        if library_path not in entries:
            current = environment.get(library_key)
            environment[library_key] = (
                f"{current}{os.pathsep}{library_path}" if current else library_path
            )
    return environment


def verify_pinned_haxe(installation: HaxeInstallation) -> None:
    """Fail before server use if the resolved binary is not the repository pin."""

    result = subprocess.run(
        [str(installation.compiler), "--version"],
        cwd=ROOT,
        env=pinned_haxe_environment("C", installation),
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    observed = f"{result.stdout}{result.stderr}".strip()
    if result.returncode != 0 or observed != installation.version:
        raise HaxeServerFailure(
            "resolved Haxe compiler does not match .haxerc: "
            f"expected {installation.version!r}, observed {observed!r}, "
            f"exit={result.returncode}"
        )


def resolve_haxe_arguments(
    arguments: Sequence[str], *, locale: str
) -> tuple[str, ...]:
    """Ask HaxeShim to expand scoped HXML/library arguments for native Haxe."""

    environment = os.environ.copy()
    environment["LC_ALL"] = locale
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [
            _development_tool("haxe"),
            "--cwd",
            str(CASE),
            "--run",
            "resolve-args",
            *arguments,
        ],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stderr:
        raise HaxeServerFailure(
            "HaxeShim could not resolve the pinned Caxecraft build arguments\n"
            f"exit={result.returncode}\nstdout:\n{result.stdout}"
            f"\nstderr:\n{result.stderr}"
        )
    resolved = tuple(result.stdout.splitlines())
    if not resolved or any(
        not argument or "\x00" in argument or "\r" in argument or "\n" in argument
        for argument in resolved
    ):
        raise HaxeServerFailure("HaxeShim emitted malformed resolved arguments")
    if "--haxe-version" in resolved or any(
        argument.endswith(".hxml") for argument in resolved
    ):
        raise HaxeServerFailure(
            "HaxeShim left a proxy-only or unresolved build argument"
        )
    return resolved


def _endpoint(value: str) -> tuple[str, int]:
    match = re.fullmatch(r"([^:]+):([0-9]{1,5})", value)
    if match is None:
        raise HaxeServerFailure("Haxe server endpoint must use host:port syntax")
    host = match.group(1)
    port = int(match.group(2))
    if port < 1 or port > 65535:
        raise HaxeServerFailure("Haxe server endpoint port is outside 1..65535")
    return host, port


def _accepting(endpoint: str, *, timeout: float = 0.25) -> bool:
    try:
        host, port = _endpoint(endpoint)
        with socket.create_connection((host, port), timeout=timeout):
            return True
    except (OSError, HaxeServerFailure):
        return False


def _available_endpoint() -> str:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind((LOOPBACK, 0))
        return f"{LOOPBACK}:{int(candidate.getsockname()[1])}"


def process_start_identity(pid: int) -> str | None:
    """Return an OS fact that distinguishes a live PID from a later reused PID."""

    if pid < 1:
        return None
    if os.name == "nt":
        command = [
            "powershell",
            "-NoProfile",
            "-NonInteractive",
            "-Command",
            (
                f"$p=Get-Process -Id {pid} -ErrorAction SilentlyContinue;"
                "if($p){$p.StartTime.ToUniversalTime().Ticks}"
            ),
        ]
        try:
            result = subprocess.run(
                command, check=False, capture_output=True, text=True, timeout=3
            )
        except (OSError, subprocess.TimeoutExpired):
            return None
        value = result.stdout.strip()
        return f"windows-start-ticks:{value}" if result.returncode == 0 and value else None

    linux_stat = Path(f"/proc/{pid}/stat")
    if linux_stat.is_file():
        try:
            raw = linux_stat.read_text(encoding="utf-8")
            # The command name in field 2 may contain spaces and parentheses.
            # Everything after its final ')' starts at field 3; start time is
            # original field 22, therefore index 19 in this suffix.
            suffix = raw[raw.rfind(")") + 2 :].split()
            start_ticks = suffix[19]
            boot = Path("/proc/sys/kernel/random/boot_id").read_text(
                encoding="utf-8"
            ).strip()
            return f"linux:{boot}:{start_ticks}"
        except (OSError, UnicodeError, IndexError):
            return None

    try:
        result = subprocess.run(
            ["ps", "-p", str(pid), "-o", "lstart="],
            check=False,
            capture_output=True,
            text=True,
            timeout=3,
        )
    except (OSError, subprocess.TimeoutExpired):
        return None
    value = " ".join(result.stdout.split())
    return f"posix-lstart:{value}" if result.returncode == 0 and value else None


def process_command_identity(pid: int) -> str | None:
    """Identify the executable currently running under an already-owned PID."""

    if pid < 1:
        return None
    if os.name == "nt":
        try:
            result = subprocess.run(
                [
                    "powershell",
                    "-NoProfile",
                    "-NonInteractive",
                    "-Command",
                    (
                        f"$p=Get-Process -Id {pid} -ErrorAction SilentlyContinue;"
                        "if($p){$p.Path}"
                    ),
                ],
                check=False,
                capture_output=True,
                text=True,
                timeout=3,
            )
        except (OSError, subprocess.TimeoutExpired):
            return None
        value = result.stdout.strip()
        return f"windows-path:{value}" if result.returncode == 0 and value else None
    executable = Path(f"/proc/{pid}/exe")
    if executable.exists():
        try:
            return f"linux-exe:{executable.resolve()}"
        except OSError:
            return None
    try:
        result = subprocess.run(
            ["ps", "-p", str(pid), "-o", "comm="],
            check=False,
            capture_output=True,
            text=True,
            timeout=3,
        )
    except (OSError, subprocess.TimeoutExpired):
        return None
    value = result.stdout.strip()
    return f"posix-command:{value}" if result.returncode == 0 and value else None


def _cookie_digest(cookie: Mapping[str, object]) -> bool:
    digest = cookie.get("sha256")
    body = {key: value for key, value in cookie.items() if key != "sha256"}
    return isinstance(digest, str) and digest == canonical_digest(body)


def _read_cookie(path: Path) -> dict[str, object] | None:
    if not path.exists():
        return None
    if path.is_symlink() or not path.is_file():
        raise HaxeServerFailure(f"Haxe server cookie is not a regular file: {path}")
    try:
        value: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise HaxeServerFailure(f"Haxe server cookie is unreadable: {error}") from error
    if not isinstance(value, dict):
        raise HaxeServerFailure("Haxe server cookie must be a JSON object")
    return value


def _owned_cookie(cookie: Mapping[str, object]) -> bool:
    return (
        cookie.get("schemaVersion") == COOKIE_SCHEMA_VERSION
        and cookie.get("kind") == COOKIE_KIND
        and _cookie_digest(cookie)
        and isinstance(cookie.get("pid"), int)
        and not isinstance(cookie.get("pid"), bool)
        and isinstance(cookie.get("processStartIdentity"), str)
        and process_start_identity(int(cookie["pid"]))
        == cookie.get("processStartIdentity")
        and isinstance(cookie.get("processCommandIdentity"), str)
        and process_command_identity(int(cookie["pid"]))
        == cookie.get("processCommandIdentity")
    )


def _terminate_owned(cookie: Mapping[str, object]) -> bool:
    """Stop only the exact process named by a still-valid ownership cookie."""

    if not _owned_cookie(cookie):
        return False
    pid = int(cookie["pid"])
    try:
        if os.name == "nt":
            result = subprocess.run(
                ["taskkill", "/PID", str(pid), "/T"],
                check=False,
                capture_output=True,
                text=True,
                timeout=5,
            )
            if result.returncode != 0 and process_start_identity(pid) is not None:
                return False
        else:
            os.kill(pid, signal.SIGTERM)
    except (OSError, subprocess.TimeoutExpired):
        return process_start_identity(pid) is None

    def reaped_or_gone() -> bool:
        if os.name != "nt":
            try:
                reaped, _status = os.waitpid(pid, os.WNOHANG)
                if reaped == pid:
                    return True
            except ChildProcessError:
                pass
        return process_start_identity(pid) is None

    deadline = time.monotonic() + 5.0
    while time.monotonic() < deadline:
        if reaped_or_gone():
            return True
        time.sleep(0.05)
    if not _owned_cookie(cookie):
        return process_start_identity(pid) is None
    try:
        if os.name == "nt":
            subprocess.run(
                ["taskkill", "/F", "/PID", str(pid), "/T"],
                check=False,
                capture_output=True,
                text=True,
                timeout=5,
            )
        else:
            os.kill(pid, signal.SIGKILL)
    except (OSError, subprocess.TimeoutExpired):
        pass
    deadline = time.monotonic() + 5.0
    while time.monotonic() < deadline:
        if reaped_or_gone():
            return True
        time.sleep(0.05)
    return False


class OwnedHaxeServer:
    """Start or reuse one compatible server below a worktree-owned state root."""

    def __init__(
        self,
        state_root: Path,
        *,
        installation: HaxeInstallation,
        compatibility: Mapping[str, object],
    ):
        self.state_root = state_root
        self.installation = installation
        self.compatibility = {
            **dict(compatibility),
            "canonicalServerStateRoot": str(state_root.resolve()),
        }
        self.compatibility_sha256 = canonical_digest(self.compatibility)
        self.cookie_path = state_root / "hxc-haxe-server.json"
        self.lock_path = state_root / "hxc-haxe-server.lock"

    def _connection_from_cookie(
        self, cookie: Mapping[str, object]
    ) -> HaxeServerLease | None:
        endpoint = cookie.get("endpoint")
        if (
            not _owned_cookie(cookie)
            or cookie.get("compatibilitySha256") != self.compatibility_sha256
            or not isinstance(endpoint, str)
            or not _accepting(endpoint)
        ):
            return None
        return HaxeServerLease(
            connection=HaxeServerConnection(endpoint, self.installation),
            owned=True,
            compatibility_sha256=self.compatibility_sha256,
        )

    def _start(self) -> HaxeServerLease:
        self.state_root.mkdir(parents=True, exist_ok=True)
        if self.state_root.is_symlink() or not self.state_root.is_dir():
            raise HaxeServerFailure(
                f"Haxe server state root must be a real directory: {self.state_root}"
            )
        endpoint = _available_endpoint()
        stdout_path = self.state_root / "server.stdout.log"
        stderr_path = self.state_root / "server.stderr.log"
        command = [
            str(self.installation.compiler),
            "--server-listen",
            endpoint,
        ]
        environment = pinned_haxe_environment("C", self.installation)
        if os.name == "nt":
            with stdout_path.open("ab") as stdout, stderr_path.open("ab") as stderr:
                creation_flags = (
                    subprocess.CREATE_NEW_PROCESS_GROUP  # type: ignore[attr-defined]
                    | subprocess.DETACHED_PROCESS  # type: ignore[attr-defined]
                )
                process = subprocess.Popen(
                    command,
                    cwd=self.state_root,
                    env=environment,
                    stdin=subprocess.DEVNULL,
                    stdout=stdout,
                    stderr=stderr,
                    creationflags=creation_flags,
                )
            pid = process.pid
            # DETACHED_PROCESS intentionally survives this Python command.
            # Closing our process handle and recording an observed OS identity
            # leaves lifecycle ownership with the cookie rather than Popen.
            process._handle.Close()  # type: ignore[attr-defined]
            process.returncode = 0
        else:
            read_fd, write_fd = os.pipe()
            first_pid = os.fork()
            if first_pid == 0:
                try:
                    os.close(read_fd)
                    os.setsid()
                    second_pid = os.fork()
                    if second_pid != 0:
                        os._exit(0)
                    with stdout_path.open("ab", buffering=0) as stdout, stderr_path.open(
                        "ab", buffering=0
                    ) as stderr, open(os.devnull, "rb", buffering=0) as stdin:
                        os.chdir(self.state_root)
                        os.dup2(stdin.fileno(), 0)
                        os.dup2(stdout.fileno(), 1)
                        os.dup2(stderr.fileno(), 2)
                        os.write(write_fd, f"{os.getpid()}\n".encode("ascii"))
                        os.close(write_fd)
                        os.execve(command[0], command, environment)
                except BaseException:
                    os._exit(127)
            os.close(write_fd)
            with os.fdopen(read_fd, "rb", closefd=True) as pipe:
                raw_pid = pipe.readline(32)
            os.waitpid(first_pid, 0)
            try:
                pid = int(raw_pid.strip())
            except ValueError as error:
                raise HaxeServerFailure(
                    f"could not start detached Haxe server; see {stderr_path}"
                ) from error
        start_identity: str | None = None
        command_identity: str | None = None
        try:
            deadline = time.monotonic() + 10.0
            while time.monotonic() < deadline:
                start_identity = process_start_identity(pid)
                command_identity = process_command_identity(pid)
                if start_identity is None or command_identity is None:
                    raise HaxeServerFailure(
                        "owned Haxe server exited before accepting connections; "
                        f"see {stderr_path}"
                    )
                if _accepting(endpoint):
                    break
                time.sleep(0.05)
            else:
                raise HaxeServerFailure(
                    f"owned Haxe server did not accept connections; see {stderr_path}"
                )
            body: dict[str, object] = {
                "schemaVersion": COOKIE_SCHEMA_VERSION,
                "kind": COOKIE_KIND,
                "endpoint": endpoint,
                "pid": pid,
                "processStartIdentity": start_identity,
                "processCommandIdentity": command_identity,
                "compatibilitySha256": self.compatibility_sha256,
                "compiler": str(self.installation.compiler),
            }
            atomic_write_state(
                self.cookie_path, {**body, "sha256": canonical_digest(body)}
            )
        except BaseException:
            # Startup has not published an ownership cookie yet. Kill only the
            # exact process we just observed; a reused PID or changed command
            # fails this comparison and is left untouched.
            if (
                start_identity is not None
                and command_identity is not None
                and process_start_identity(pid) == start_identity
                and process_command_identity(pid) == command_identity
            ):
                try:
                    os.kill(pid, signal.SIGTERM)
                except OSError:
                    pass
            raise
        return HaxeServerLease(
            connection=HaxeServerConnection(endpoint, self.installation),
            owned=True,
            compatibility_sha256=self.compatibility_sha256,
        )

    def connect(self) -> HaxeServerLease:
        """Reuse a compatible process or replace only an exactly owned stale one."""

        self.state_root.mkdir(parents=True, exist_ok=True)
        with VariantLock(self.lock_path):
            cookie: dict[str, object] | None
            try:
                cookie = _read_cookie(self.cookie_path)
            except HaxeServerFailure:
                cookie = None
            if cookie is not None:
                connection = self._connection_from_cookie(cookie)
                if connection is not None:
                    return connection
                if _owned_cookie(cookie) and not _terminate_owned(cookie):
                    raise HaxeServerFailure(
                        "stale Haxe server is still alive but could not be stopped safely"
                    )
                self.cookie_path.unlink(missing_ok=True)
            return self._start()

    def stop(self) -> bool:
        """Stop the exact owned server; return false when none is safely owned."""

        self.state_root.mkdir(parents=True, exist_ok=True)
        with VariantLock(self.lock_path):
            try:
                cookie = _read_cookie(self.cookie_path)
            except HaxeServerFailure:
                cookie = None
            if cookie is None:
                return False
            stopped = _terminate_owned(cookie)
            if stopped:
                self.cookie_path.unlink(missing_ok=True)
            return stopped

    def restart_after_transport_failure(
        self, lease: HaxeServerLease
    ) -> HaxeServerLease:
        """Replace a failed automatic lease once, without touching attachments."""

        if not lease.owned:
            raise HaxeServerFailure("an explicitly attached Haxe server is not owned")
        self.stop()
        return self.connect()


def attached_server(
    endpoint: str, installation: HaxeInstallation
) -> HaxeServerLease:
    """Borrow a caller-owned endpoint after checking syntax and reachability."""

    _endpoint(endpoint)
    if not _accepting(endpoint):
        raise HaxeServerFailure(f"attached Haxe server is not reachable: {endpoint}")
    return HaxeServerLease(
        connection=HaxeServerConnection(endpoint, installation),
        owned=False,
        compatibility_sha256=None,
    )


def transport_failed(
    result: subprocess.CompletedProcess[str], lease: HaxeServerLease
) -> bool:
    """Distinguish a lost server from a normal source/compiler diagnostic."""

    if result.returncode == 0:
        return False
    combined = f"{result.stdout}\n{result.stderr}".lower()
    markers = (
        "couldn't connect",
        "could not connect",
        "connection refused",
        "connection reset",
        "broken pipe",
        "server disconnected",
        "socket error",
    )
    if any(marker in combined for marker in markers):
        return True
    if lease.owned:
        try:
            with socket.create_connection(
                _endpoint(lease.connection.endpoint), timeout=0.2
            ):
                return False
        except OSError:
            return True
    return False

#!/usr/bin/env python3
"""Run digest vectors and exact warm-cache lifecycle checks with pinned Haxe."""

from __future__ import annotations

import hashlib
import json
import os
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SUCCESS_SENTINEL = "content-digest: OK"


def development_tool(name: str) -> str:
    """Prefer the repository-installed tool so CI and local runs use one pin."""

    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def run_digest_vectors(haxe: str) -> None:
    """Keep the digest implementation anchored to public and differential vectors."""
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [
            haxe,
            "--class-path",
            "src",
            "--class-path",
            "test/content_digest",
            "--main",
            "ContentDigestProbe",
            "--interp",
        ],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        raise RuntimeError(
            "Haxe digest probe failed\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}",
        )
    if result.stdout.strip().splitlines() != [SUCCESS_SENTINEL]:
        raise RuntimeError(
            "unexpected digest-probe output\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}",
        )


def available_port() -> int:
    """Reserve an ephemeral loopback port for this isolated test process."""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    """Wait until the owned Haxe server accepts requests or fails visibly."""
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise RuntimeError(
                "Haxe compiler server exited early\n"
                f"stdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise RuntimeError("Haxe compiler server did not accept requests within 10 seconds")


def run_cache_request(
    haxe: str, endpoint: str, mode: str, report: Path
) -> dict[str, object]:
    """Execute one macro-server request and decode its stored cache evidence."""
    macro = (
        "GeneratedFileDigestCacheProbe.run("
        + json.dumps(mode)
        + ","
        + json.dumps(str(report))
        + ")"
    )
    environment = os.environ.copy()
    environment.pop("HAXE_NO_SERVER", None)
    result = subprocess.run(
        [
            haxe,
            "--connect",
            endpoint,
            "--class-path",
            "src",
            "--class-path",
            "test/content_digest",
            "--no-output",
            "--macro",
            macro,
        ],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise RuntimeError(
            f"cache probe `{mode}` failed\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    value = json.loads(report.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise RuntimeError(f"cache probe `{mode}` produced a non-object report")
    return value


def validate_file_evidence(mode: str, report: dict[str, object]) -> None:
    """Recompute every digest and UTF-8 size outside the production cache."""
    files = report.get("files")
    if not isinstance(files, list) or len(files) != 3:
        raise RuntimeError(f"cache probe `{mode}` reported an invalid file inventory")
    for raw_file in files:
        if not isinstance(raw_file, dict):
            raise RuntimeError(f"cache probe `{mode}` reported a non-object file")
        contents = raw_file.get("contents")
        digest = raw_file.get("sha256")
        byte_length = raw_file.get("byteLength")
        if not isinstance(contents, str) or not isinstance(digest, str):
            raise RuntimeError(f"cache probe `{mode}` omitted string content evidence")
        encoded = contents.encode("utf-8")
        if digest != hashlib.sha256(encoded).hexdigest():
            raise RuntimeError(f"cache probe `{mode}` reused an invalid SHA-256 digest")
        if byte_length != len(encoded):
            raise RuntimeError(f"cache probe `{mode}` reused an invalid byte length")


def validate_stats(
    mode: str,
    report: dict[str, object],
    *,
    enabled: bool,
    hits: int,
    misses: int,
) -> None:
    """Check the exact key mutation and one-generation retention result."""
    if report.get("aborted") is not False:
        raise RuntimeError(f"cache probe `{mode}` unexpectedly aborted")
    stats = report.get("stats")
    if not isinstance(stats, dict):
        raise RuntimeError(f"cache probe `{mode}` omitted cache statistics")
    expected = {
        "enabled": enabled,
        "hits": hits,
        "misses": misses,
        "retainedEntries": 3,
    }
    for key, value in expected.items():
        if stats.get(key) != value:
            raise RuntimeError(
                f"cache probe `{mode}` reported {key}={stats.get(key)!r}; "
                f"expected {value!r}"
            )
    files = report.get("files")
    assert isinstance(files, list)
    retained_bytes = sum(
        int(raw_file["byteLength"])
        for raw_file in files
        if isinstance(raw_file, dict)
    )
    if stats.get("retainedBytes") != retained_bytes:
        raise RuntimeError(
            f"cache probe `{mode}` retained-byte count drifted: {stats!r}"
        )


def run_cache_lifecycle(haxe: str) -> None:
    """Prove hits, every exact-key miss, abort isolation, and cache-off parity."""
    port = available_port()
    endpoint = str(port)
    environment = os.environ.copy()
    environment.pop("HAXE_NO_SERVER", None)
    server = subprocess.Popen(
        [haxe, "--wait", endpoint],
        cwd=ROOT,
        env=environment,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    sequence = (
        ("baseline", True, 0, 3),
        ("baseline", True, 3, 0),
        ("changed-content", True, 2, 1),
        ("abort-baseline", True, 2, 1),
        ("changed-content", True, 3, 0),
        ("changed-kind", True, 2, 1),
        ("changed-path", True, 2, 1),
        ("disabled", False, 0, 3),
        ("changed-path", True, 3, 0),
    )
    try:
        wait_for_server(server, port)
        with tempfile.TemporaryDirectory(prefix="reflaxe-c-digest-cache-") as temporary:
            root = Path(temporary)
            for index, (mode, enabled, hits, misses) in enumerate(sequence):
                report = run_cache_request(
                    haxe, endpoint, mode, root / f"{index:02d}-{mode}.json"
                )
                validate_file_evidence(mode, report)
                if mode == "abort-baseline":
                    if report.get("aborted") is not True or "stats" in report:
                        raise RuntimeError(
                            "aborted cache request published completion evidence"
                        )
                    continue
                validate_stats(
                    mode,
                    report,
                    enabled=enabled,
                    hits=hits,
                    misses=misses,
                )
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def main() -> int:
    haxe = development_tool("haxe")
    if shutil.which(haxe) is None:
        print(
            "content-digest: ERROR: pinned Haxe executable is unavailable",
            file=sys.stderr,
        )
        return 1
    try:
        run_digest_vectors(haxe)
        run_cache_lifecycle(haxe)
    except (
        OSError,
        RuntimeError,
        subprocess.TimeoutExpired,
        json.JSONDecodeError,
    ) as error:
        print(f"content-digest: ERROR: {error}", file=sys.stderr)
        return 1
    print(SUCCESS_SENTINEL)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

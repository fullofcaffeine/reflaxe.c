#!/usr/bin/env python3
"""Compile-backed typed C surface, metadata, determinism, and diagnostic probes."""

from __future__ import annotations

import json
import os
import re
import socket
import subprocess
import time
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURES = Path(__file__).resolve().parent / "fixtures"
EXPECTED = Path(__file__).resolve().parent / "expected/typed-c-contract.json"
REPORT_PREFIX = "HXC_TYPED_C_CONTRACT="


class TypedCProbeFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def compile_fixture(
    directory: str, main: str = "Main", report: bool = False, connect: str | None = None
) -> subprocess.CompletedProcess[str]:
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
        "-cp",
        str(FIXTURES / directory),
        "-lib",
        "reflaxe.c",
        "-D",
        "reflaxe_c_lifecycle_probe",
        "-D",
        "c_output=typed-c-contract-probe",
        ]
    )
    if report:
        command.extend(["-D", "reflaxe_c_contract_report"])
    command.extend(["-main", main, "--interp"])
    environment = os.environ.copy()
    if connect is None:
        environment["HAXE_NO_SERVER"] = "1"
    else:
        environment.pop("HAXE_NO_SERVER", None)
    return subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def require_success(result: subprocess.CompletedProcess[str], label: str) -> None:
    if result.returncode != 0:
        raise TypedCProbeFailure(
            f"{label} failed with {result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def extract_report(result: subprocess.CompletedProcess[str], label: str) -> tuple[str, dict[str, object]]:
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if len(lines) != 1:
        raise TypedCProbeFailure(
            f"{label} emitted {len(lines)} typed C reports, expected exactly one\nstdout:\n{result.stdout}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    return payload, json.loads(payload)


def check_positive_and_deterministic() -> None:
    forward = compile_fixture("positive", "Main", report=True)
    reverse = compile_fixture("positive", "MainReverse", report=True)
    require_success(forward, "forward-order typed C fixture")
    require_success(reverse, "reverse-order typed C fixture")

    forward_payload, forward_report = extract_report(forward, "forward-order typed C fixture")
    reverse_payload, reverse_report = extract_report(reverse, "reverse-order typed C fixture")
    if forward_payload != reverse_payload:
        raise TypedCProbeFailure("typed C contract report changed with root-module reference order")

    expected = json.loads(EXPECTED.read_text(encoding="utf-8"))
    if forward_report != expected:
        actual = json.dumps(forward_report, indent=2, sort_keys=True)
        wanted = json.dumps(expected, indent=2, sort_keys=True)
        raise TypedCProbeFailure(f"typed C contract snapshot drifted\nexpected:\n{wanted}\nactual:\n{actual}")

    effects = forward_report["effects"]
    if effects["runtimeFeatures"] != [] or effects["allocation"] != "none" or effects["unsafe"] != "none":
        raise TypedCProbeFailure(f"M0 typed C declarations gained a hidden runtime/unsafe effect: {effects!r}")
    if "hxrt" in forward_payload.lower():
        raise TypedCProbeFailure("M0 typed C declaration report unexpectedly mentions hxrt")


def check_negative_fixtures() -> None:
    cases = {
        "invalid_header": "generated header path contains a forbidden",
        "opaque_by_value": "cannot be embedded by value",
        "declaration_cycle": "impossible by-value C declaration cycle",
        "duplicate_symbol": "duplicate explicit C symbol",
        "static_assert": "static assertion failed: fixture invariant",
        "unknown_metadata": "unknown typed C metadata `@:c.heder`",
        "untyped_layout": "must use a typed `c.Layout` value",
        "invalid_build_fact": "library name contains a character outside",
        "missing_metadata_argument": "`@:c.header` expects 2 parameter(s), received 0",
        "invalid_pack": "`c.pack` must be a power of two",
        "reserved_symbol": "is reserved by C or reflaxe.c",
    }
    source_position = re.compile(r"(?:^|/)(?:Main|Widget[^/]*)\.hx:\d+: characters \d+-\d+")
    for directory, expected in cases.items():
        result = compile_fixture(directory)
        combined = result.stdout + result.stderr
        if result.returncode == 0:
            raise TypedCProbeFailure(f"negative fixture {directory} unexpectedly compiled")
        if "HXC5002:" not in combined or expected not in combined:
            raise TypedCProbeFailure(
                f"negative fixture {directory} missed its stable diagnostic\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        if source_position.search(combined) is None:
            raise TypedCProbeFailure(f"negative fixture {directory} did not report a Haxe source position\n{combined}")


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise TypedCProbeFailure(
                f"Haxe compiler server exited early\nstdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise TypedCProbeFailure("Haxe compiler server did not accept connections within 10 seconds")


def check_compiler_server_isolation() -> None:
    port = available_port()
    endpoint = str(port)
    environment = os.environ.copy()
    environment.pop("HAXE_NO_SERVER", None)
    server = subprocess.Popen(
        [development_tool("haxe"), "--wait", endpoint],
        cwd=ROOT,
        env=environment,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_server(server, port)
        first = compile_fixture("positive", "Main", report=True, connect=endpoint)
        invalid = compile_fixture("duplicate_symbol", connect=endpoint)
        repeated = compile_fixture("positive", "MainReverse", report=True, connect=endpoint)
        require_success(first, "compiler-server first typed C fixture")
        require_success(repeated, "compiler-server repeated typed C fixture")
        if invalid.returncode == 0 or "HXC5002: duplicate explicit C symbol" not in invalid.stdout + invalid.stderr:
            raise TypedCProbeFailure("compiler-server negative request missed its isolated HXC5002 diagnostic")
        first_payload, _ = extract_report(first, "compiler-server first typed C fixture")
        repeated_payload, _ = extract_report(repeated, "compiler-server repeated typed C fixture")
        if first_payload != repeated_payload:
            raise TypedCProbeFailure("typed C collector leaked or reordered state across compiler-server requests")
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def main() -> int:
    check_positive_and_deterministic()
    check_negative_fixtures()
    check_compiler_server_isolation()
    print(
        "typed-c-contract: OK: surface, metadata, order/server determinism, diagnostics, and zero-runtime effects"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

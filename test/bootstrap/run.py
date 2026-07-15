#!/usr/bin/env python3
"""Compile-backed lifecycle, order, CWD, and package-layout probes."""

from __future__ import annotations

import json
import os
import re
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURE = Path(__file__).resolve().parent
EXPECTED_TARGET_CONTRACT = FIXTURE / "expected/target-contract.json"
PROBE_DEFINE = "reflaxe_c_lifecycle_probe"
TARGET_REPORT_PREFIX = "HXC_TARGET_CONTRACT="
C_EXPECTED = "bootstrap=1 init=1 c=1 reflaxe_c=1 unicode=1 utf16=0"
# The Eval host already exposes target.unicode. Isolation is proven by the
# bootstrap/init/public/implementation markers, not by clearing host facts.
NON_C_EXPECTED = "bootstrap=0 init=0 c=0 reflaxe_c=0 unicode=1 utf16=0"
CALLER_C_EXPECTED = "bootstrap=0 init=0 c=1 reflaxe_c=0 unicode=1 utf16=0"


class ProbeFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def run(
    command: list[str],
    *,
    cwd: Path,
    expected_code: int = 0,
    label: str,
    no_server: bool = True,
) -> subprocess.CompletedProcess[str]:
    environment = os.environ.copy()
    if no_server:
        environment["HAXE_NO_SERVER"] = "1"
    else:
        environment.pop("HAXE_NO_SERVER", None)
    result = subprocess.run(
        command,
        cwd=cwd,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != expected_code:
        rendered = " ".join(command)
        raise ProbeFailure(
            f"{label} returned {result.returncode}, expected {expected_code}\n"
            f"command: {rendered}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return result


def assert_probe(result: subprocess.CompletedProcess[str], expected: str, label: str) -> None:
    lines = [line.strip() for line in result.stdout.splitlines() if line.strip()]
    if not lines or lines[-1] != expected:
        raise ProbeFailure(
            f"{label} produced an unexpected lifecycle report\n"
            f"expected: {expected}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def extract_target_report(
    result: subprocess.CompletedProcess[str], label: str
) -> dict[str, object]:
    reports = [
        line[len(TARGET_REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(TARGET_REPORT_PREFIX)
    ]
    if len(reports) != 1:
        raise ProbeFailure(
            f"{label} emitted {len(reports)} target reports, expected exactly one\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return json.loads(reports[0])


def source_command(
    *,
    c_build: bool,
    duplicate_macros: bool = False,
    prelude_defines: tuple[tuple[str, str | None], ...] = (),
    direct_defines: tuple[str, ...] = (),
    target_report: bool = False,
    carrier: str = "eval",
) -> list[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(FIXTURE),
    ]
    for name, value in prelude_defines:
        value_argument = "null" if value is None else json.dumps(value)
        command.extend(
            ["--macro", f"TargetPrelude.define({json.dumps(name)}, {value_argument})"]
        )
    command.extend(
        [
            "-lib",
            "reflaxe.c",
            "-D",
            PROBE_DEFINE,
        ]
    )
    if c_build:
        command.extend(["-D", "c_output=bootstrap-probe-output"])
    for define in direct_defines:
        command.extend(["-D", define])
    if duplicate_macros:
        command.extend(
            [
                "--macro",
                "reflaxe.c.CompilerBootstrap.Start()",
                "--macro",
                "reflaxe.c.CompilerInit.Start()",
            ]
        )
    if target_report:
        command.extend(["--macro", "TargetContractProbe.install()"])
    command.extend(["-main", "BootstrapProbe"])
    if carrier == "eval":
        command.append("--interp")
    elif carrier == "cross":
        command.append("--no-output")
    elif carrier == "js":
        command.extend(["--js", "bootstrap-probe.js", "--no-output"])
    else:
        raise ProbeFailure(f"unknown test carrier: {carrier}")
    return command


def check_source_checkout() -> None:
    nested_cwd = FIXTURE
    non_c = run(
        source_command(c_build=False),
        cwd=nested_cwd,
        label="non-C isolation probe",
    )
    assert_probe(non_c, NON_C_EXPECTED, "non-C isolation probe")

    c_build = run(
        source_command(c_build=True),
        cwd=nested_cwd,
        label="C activation probe",
    )
    assert_probe(c_build, C_EXPECTED, "C activation probe")

    duplicate = run(
        source_command(c_build=True, duplicate_macros=True),
        cwd=nested_cwd,
        label="exactly-once probe",
    )
    assert_probe(duplicate, C_EXPECTED, "exactly-once probe")


def check_target_identity_and_stdlib_branches() -> None:
    expected = json.loads(EXPECTED_TARGET_CONTRACT.read_text(encoding="utf-8"))

    c_output = run(
        source_command(c_build=True, target_report=True),
        cwd=FIXTURE,
        label="c_output target-contract probe",
    )
    assert_probe(c_output, C_EXPECTED, "c_output target-contract probe")
    if extract_target_report(c_output, "c_output target-contract probe") != expected:
        raise ProbeFailure("c_output target-contract snapshot drifted")

    duplicate = run(
        source_command(c_build=True, duplicate_macros=True, target_report=True),
        cwd=FIXTURE,
        label="duplicate target-contract probe",
    )
    assert_probe(duplicate, C_EXPECTED, "duplicate target-contract probe")
    if extract_target_report(duplicate, "duplicate target-contract probe") != expected:
        raise ProbeFailure("duplicate initialization changed the target-contract snapshot")

    name_only = run(
        source_command(
            c_build=False,
            prelude_defines=(("target.name", "c"),),
            target_report=True,
        ),
        cwd=FIXTURE,
        label="target.name activation probe",
    )
    assert_probe(name_only, C_EXPECTED, "target.name activation probe")
    expected_name_only = json.loads(json.dumps(expected))
    expected_name_only["defines"]["cOutput"] = False
    if extract_target_report(name_only, "target.name activation probe") != expected_name_only:
        raise ProbeFailure("target.name activation did not normalize to the C contract")

    non_c = run(
        source_command(c_build=False, target_report=True),
        cwd=FIXTURE,
        label="non-C target-contract isolation probe",
    )
    assert_probe(non_c, NON_C_EXPECTED, "non-C target-contract isolation probe")
    non_c_report = extract_target_report(non_c, "non-C target-contract isolation probe")
    non_c_defines = non_c_report["defines"]
    if (
        non_c_report["carrier"] is not None
        or non_c_defines["bootstrapCount"] is not None
        or non_c_defines["initCount"] is not None
        or non_c_defines["c"]
        or non_c_defines["reflaxeC"]
        or non_c_defines["targetName"] != "eval"
    ):
        raise ProbeFailure(f"non-C build was mutated by target initialization: {non_c_report!r}")

    caller_c = run(
        source_command(c_build=False, direct_defines=("c",)),
        cwd=FIXTURE,
        label="caller-supplied c non-activation probe",
    )
    assert_probe(caller_c, CALLER_C_EXPECTED, "caller-supplied c non-activation probe")


def assert_configuration_conflict(
    command: list[str], expected_fragment: str, label: str
) -> None:
    result = run(command, cwd=FIXTURE, expected_code=1, label=label)
    combined = result.stdout + result.stderr
    source_position = re.compile(r"BootstrapProbe\.hx:\d+: character(?:s)? \d+(?:-\d+)?")
    if "HXC0003:" not in combined or expected_fragment not in combined:
        raise ProbeFailure(
            f"{label} missed its stable configuration diagnostic\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if source_position.search(combined) is None:
        raise ProbeFailure(f"{label} did not identify the compilation root\n{combined}")


def check_target_conflicts() -> None:
    assert_configuration_conflict(
        source_command(
            c_build=True,
            prelude_defines=(("target.name", "js"),),
        ),
        "advertises `target.name=js`",
        "conflicting target.name probe",
    )
    assert_configuration_conflict(
        source_command(
            c_build=True,
            prelude_defines=(("target.utf16", None),),
        ),
        "exposes `target.utf16`",
        "conflicting target.utf16 probe",
    )
    assert_configuration_conflict(
        source_command(
            c_build=True,
            prelude_defines=(("target.atomics", None),),
        ),
        "before a reflaxe.c platform adapter has proven atomic support",
        "unproven target.atomics probe",
    )
    assert_configuration_conflict(
        source_command(
            c_build=True,
            direct_defines=("hxc_environment=freestanding",),
        ),
        "freestanding environment conflicts",
        "freestanding carrier-capability probe",
    )
    assert_configuration_conflict(
        source_command(c_build=True, carrier="js"),
        "Haxe carrier `js` advertises `target.name=js`",
        "unrelated platform carrier probe",
    )
    assert_configuration_conflict(
        source_command(c_build=True, carrier="cross"),
        "Haxe carrier `cross` exposes `target.utf16`",
        "Haxe 4 Cross UTF-16 carrier probe",
    )

    reserved = run(
        [
            development_tool("haxe"),
            "-cp",
            str(FIXTURE),
            "-D",
            "target.name=c",
            "-main",
            "BootstrapProbe",
            "--interp",
        ],
        cwd=FIXTURE,
        expected_code=1,
        label="reserved target namespace probe",
    )
    if "reserved compiler flag namespace `target.*`" not in reserved.stdout + reserved.stderr:
        raise ProbeFailure("Haxe 4 unexpectedly accepted target.name from the command line")


def check_reversed_order_fails() -> None:
    command = [
        development_tool("haxe"),
        "-cp",
        str(ROOT / "src"),
        "-cp",
        str(ROOT / "vendor/reflaxe/src"),
        "-cp",
        str(FIXTURE),
        "-D",
        f"reflaxe={json.loads((ROOT / 'vendor/reflaxe/haxelib.json').read_text())['version']}",
        "-D",
        PROBE_DEFINE,
        "-D",
        "c_output=bootstrap-probe-output",
        "--macro",
        "reflaxe.c.CompilerInit.Start()",
        "-main",
        "BootstrapProbe",
        "--interp",
    ]
    result = subprocess.run(
        command,
        cwd=FIXTURE,
        env={**os.environ, "HAXE_NO_SERVER": "1"},
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    combined = result.stdout + result.stderr
    if result.returncode == 0 or "CompilerBootstrap.Start() must run before" not in combined:
        raise ProbeFailure(
            "reversed lifecycle order did not fail with the expected diagnostic\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], address: tuple[str, int]) -> None:
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise ProbeFailure(
                "Haxe compiler server exited before accepting connections\n"
                f"stdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(address, timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise ProbeFailure("Haxe compiler server did not accept connections within 10 seconds")


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
        wait_for_server(server, ("127.0.0.1", port))
        expected_contract = json.loads(
            EXPECTED_TARGET_CONTRACT.read_text(encoding="utf-8")
        )
        cases = [
            (True, (), C_EXPECTED, "compiler-server first C build"),
            (
                True,
                (("target.atomics", None),),
                None,
                "compiler-server invalid capability build",
            ),
            (False, (), NON_C_EXPECTED, "compiler-server non-C isolation"),
            (True, (), C_EXPECTED, "compiler-server repeated C build"),
        ]
        for c_build, prelude_defines, expected, label in cases:
            command = source_command(
                c_build=c_build,
                prelude_defines=prelude_defines,
                target_report=c_build and expected is not None,
            )
            command[1:1] = ["--connect", endpoint]
            result = run(
                command,
                cwd=FIXTURE,
                expected_code=0 if expected is not None else 1,
                label=label,
                no_server=False,
            )
            if expected is None:
                if "HXC0003:" not in result.stdout + result.stderr:
                    raise ProbeFailure("compiler-server conflict lost its diagnostic")
                continue
            assert_probe(result, expected, label)
            if c_build and extract_target_report(result, label) != expected_contract:
                raise ProbeFailure(f"{label} leaked or changed target state")
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def copy_package_file(source: Path, stage: Path) -> None:
    destination = stage / source.relative_to(ROOT)
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, destination)


def stage_package(stage: Path) -> None:
    shutil.copytree(ROOT / "src", stage / "src")
    shutil.copytree(ROOT / "std", stage / "std")
    shutil.copytree(ROOT / "vendor/reflaxe/src", stage / "src", dirs_exist_ok=True)
    for relative in ("haxelib.json", "extraParams.hxml", "README.md", "LICENSE"):
        copy_package_file(ROOT / relative, stage)

    policy = json.loads((ROOT / "docs/specs/third-party-provenance.json").read_text())
    for entry in policy["release"]["requiredFiles"]:
        copy_package_file(ROOT / entry["path"], stage)


def check_package_layout() -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-package-") as temporary:
        temporary_root = Path(temporary)
        stage = temporary_root / "package"
        consumer = temporary_root / "consumer"
        nested = consumer / "nested"
        stage.mkdir()
        consumer.mkdir()
        nested.mkdir()
        stage_package(stage)

        run([development_tool("lix"), "scope", "create"], cwd=consumer, label="package scope creation")
        run([development_tool("lix"), "use", "haxe", "4.3.7"], cwd=consumer, label="package Haxe pin")
        run(
            [development_tool("lix"), "dev", "reflaxe.c", str(stage)],
            cwd=consumer,
            label="package library mount",
        )

        package_probe = run(
            [
                development_tool("haxe"),
                "-cp",
                str(FIXTURE),
                "-lib",
                "reflaxe.c",
                "-D",
                PROBE_DEFINE,
                "-D",
                "c_output=bootstrap-probe-output",
                "--macro",
                "TargetContractProbe.install()",
                "-main",
                "BootstrapProbe",
                "--interp",
            ],
            cwd=nested,
            label="staged package CWD probe",
        )
        assert_probe(package_probe, C_EXPECTED, "staged package CWD probe")
        expected_contract = json.loads(
            EXPECTED_TARGET_CONTRACT.read_text(encoding="utf-8")
        )
        if extract_target_report(package_probe, "staged package CWD probe") != expected_contract:
            raise ProbeFailure("staged package changed the target-contract snapshot")

        run(
            [
                sys.executable,
                str(ROOT / "scripts/ci/check_license_policy.py"),
                "--root",
                str(ROOT),
                "--package-root",
                str(stage),
                "--quiet",
            ],
            cwd=consumer,
            label="staged package notice probe",
        )


def main() -> int:
    try:
        check_source_checkout()
        check_target_identity_and_stdlib_branches()
        check_target_conflicts()
        check_reversed_order_fails()
        check_compiler_server_isolation()
        check_package_layout()
    except ProbeFailure as error:
        print(f"bootstrap-policy: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "bootstrap-policy: OK: source, target identity, stdlib branches, conflicts, order, isolation, server, and package probes passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

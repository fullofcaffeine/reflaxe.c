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
NON_C_EXPECTED = "bootstrap=0 init=0 c=0 reflaxe_c=0 unicode=1 utf16=0"
CALLER_C_EXPECTED = "bootstrap=0 init=0 c=1 reflaxe_c=0 unicode=1 utf16=0"
CONFIGURATION_DIAGNOSTIC_ID = "HXC0003"
LOWERING_DIAGNOSTIC_ID = "HXC1000"
LOWERING_DETAIL = "unimplemented whole-program lowering boundary"
DIAGNOSTIC_PROFILE = re.compile(r"\[profile=(?:portable|metal|unresolved)\]")


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


def assert_target_contract(
    result: subprocess.CompletedProcess[str], expected: dict[str, object], label: str
) -> None:
    actual = extract_target_report(result, label)
    if actual != expected:
        raise ProbeFailure(
            f"{label} changed the target-contract snapshot\n"
            f"expected:\n{json.dumps(expected, indent=2, sort_keys=True)}\n"
            f"actual:\n{json.dumps(actual, indent=2, sort_keys=True)}"
        )


def source_command(
    *,
    c_build: bool,
    duplicate_macros: bool = False,
    direct_defines: tuple[str, ...] = (),
    target_report: bool = False,
    carrier: str | None = None,
) -> list[str]:
    # The production C path is Haxe's custom target. Eval remains useful only
    # for proving that loading the library does not mutate unrelated builds.
    if carrier is None:
        carrier = "custom" if c_build else "eval"
    command = [
        development_tool("haxe"),
        "-cp",
        str(FIXTURE),
    ]
    command.extend(
        [
            "-lib",
            "reflaxe.c",
            "-D",
            PROBE_DEFINE,
        ]
    )
    if c_build and carrier != "custom" and carrier != "custom-no-output":
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
    elif carrier == "custom":
        command.extend(["--custom-target", "c=bootstrap-probe-output"])
    elif carrier == "custom-no-output":
        command.extend(["--custom-target", "c"])
    elif carrier == "cross":
        command.append("--no-output")
    elif carrier == "js":
        command.extend(["--js", "bootstrap-probe.js", "--no-output"])
    else:
        raise ProbeFailure(f"unknown test carrier: {carrier}")
    return command


def production_command(output: Path) -> list[str]:
    return [
        development_tool("haxe"),
        "-cp",
        str(FIXTURE),
        "-lib",
        "reflaxe.c",
        "--macro",
        "TargetContractProbe.install()",
        "-main",
        "BootstrapProbe",
        "--custom-target",
        f"c={output}",
    ]


def check_source_checkout() -> None:
    nested_cwd = FIXTURE
    expected = json.loads(EXPECTED_TARGET_CONTRACT.read_text(encoding="utf-8"))
    non_c = run(
        source_command(c_build=False),
        cwd=nested_cwd,
        label="non-C isolation probe",
    )
    assert_probe(non_c, NON_C_EXPECTED, "non-C isolation probe")

    c_build = run(
        source_command(c_build=True, target_report=True),
        cwd=nested_cwd,
        label="custom-target C activation probe",
    )
    assert_target_contract(c_build, expected, "custom-target C activation probe")

    duplicate = run(
        source_command(c_build=True, duplicate_macros=True, target_report=True),
        cwd=nested_cwd,
        label="exactly-once probe",
    )
    assert_target_contract(duplicate, expected, "exactly-once probe")


def check_target_identity_and_stdlib_branches() -> None:
    expected = json.loads(EXPECTED_TARGET_CONTRACT.read_text(encoding="utf-8"))

    custom_target = run(
        source_command(c_build=True, target_report=True),
        cwd=FIXTURE,
        label="custom-target contract probe",
    )
    assert_target_contract(custom_target, expected, "custom-target contract probe")

    duplicate = run(
        source_command(c_build=True, duplicate_macros=True, target_report=True),
        cwd=FIXTURE,
        label="duplicate target-contract probe",
    )
    assert_target_contract(duplicate, expected, "duplicate target-contract probe")

    explicit_transport = run(
        source_command(
            c_build=True,
            direct_defines=("c_output=bootstrap-probe-output",),
            target_report=True,
        ),
        cwd=FIXTURE,
        label="matching Reflaxe output transport probe",
    )
    assert_target_contract(
        explicit_transport, expected, "matching Reflaxe output transport probe"
    )

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
        or non_c_report["environment"] is not None
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
    source_position = re.compile(
        r"(?:^|[/ ])(?:test/bootstrap/)?BootstrapProbe\.hx(?::\d+: character(?:s)? \d+(?:-\d+)?)?",
        re.MULTILINE,
    )
    if CONFIGURATION_DIAGNOSTIC_ID not in combined or expected_fragment not in combined:
        raise ProbeFailure(
            f"{label} missed its stable configuration diagnostic\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if DIAGNOSTIC_PROFILE.search(combined) is None or "Remediation:" not in combined:
        raise ProbeFailure(f"{label} lost its profile or remediation field\n{combined}")
    if source_position.search(combined) is None:
        raise ProbeFailure(f"{label} did not identify the compilation root\n{combined}")


def check_target_conflicts() -> None:
    assert_configuration_conflict(
        source_command(
            c_build=True,
            direct_defines=("c_output=other-output",),
        ),
        "conflicts with the custom-target output",
        "conflicting output transport probe",
    )
    assert_configuration_conflict(
        source_command(
            c_build=True,
            direct_defines=("hxc_environment=invalid",),
        ),
        "unsupported C environment `invalid`",
        "invalid environment probe",
    )
    assert_configuration_conflict(
        source_command(c_build=True, carrier="custom-no-output"),
        "requires an output directory",
        "missing custom-target output probe",
    )
    assert_configuration_conflict(
        source_command(c_build=True, carrier="js"),
        "Haxe carrier `js` advertises `target.name=js`",
        "unrelated platform carrier probe",
    )
    assert_configuration_conflict(
        source_command(c_build=True, carrier="cross"),
        "Haxe carrier `cross` exposes `target.utf16`",
        "legacy Cross UTF-16 carrier probe",
    )

    expected_freestanding = json.loads(
        EXPECTED_TARGET_CONTRACT.read_text(encoding="utf-8")
    )
    expected_freestanding["environment"] = "freestanding"
    expected_freestanding["platform"]["sys"] = False
    expected_freestanding["defines"]["targetSys"] = False
    freestanding = run(
        source_command(
            c_build=True,
            direct_defines=("hxc_environment=freestanding",),
            target_report=True,
        ),
        cwd=FIXTURE,
        label="freestanding capability-clean probe",
    )
    assert_target_contract(
        freestanding, expected_freestanding, "freestanding capability-clean probe"
    )

    for define in ("target.name=c", "target.utf16", "target.atomics"):
        reserved = run(
            [
                development_tool("haxe"),
                "-cp",
                str(FIXTURE),
                "-D",
                define,
                "-main",
                "BootstrapProbe",
                "--interp",
            ],
            cwd=FIXTURE,
            expected_code=1,
            label=f"reserved target namespace probe ({define})",
        )
        if "reserved compiler flag namespace `target.*`" not in reserved.stdout + reserved.stderr:
            raise ProbeFailure(f"active Haxe unexpectedly accepted {define} from the command line")


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
        "--macro",
        "reflaxe.c.CompilerInit.Start()",
        "-main",
        "BootstrapProbe",
        "--custom-target",
        "c=bootstrap-probe-output",
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


def check_production_carrier_fails_closed() -> None:
    expected = json.loads(EXPECTED_TARGET_CONTRACT.read_text(encoding="utf-8"))
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-production-carrier-") as temporary:
        output = Path(temporary) / "generated"
        result = run(
            production_command(output),
            cwd=FIXTURE,
            expected_code=1,
            label="production custom-target lowering boundary probe",
        )
        combined = result.stdout + result.stderr
        if LOWERING_DIAGNOSTIC_ID not in combined or LOWERING_DETAIL not in combined:
            raise ProbeFailure("production custom target did not stop at HXC1000")
        if "[profile=portable]" not in combined or "Remediation:" not in combined:
            raise ProbeFailure("production HXC1000 lost its profile or remediation field")
        assert_target_contract(
            result, expected, "production custom-target lowering boundary probe"
        )
        if output.exists() and any(output.rglob("*")):
            raise ProbeFailure("HXC1000 production probe left a plausible generated artifact")


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
            (True, (), True, "compiler-server first C build"),
            (
                True,
                ("hxc_environment=invalid",),
                False,
                "compiler-server invalid environment build",
            ),
            (False, (), True, "compiler-server non-C isolation"),
            (True, (), True, "compiler-server repeated C build"),
        ]
        for c_build, direct_defines, succeeds, label in cases:
            command = source_command(
                c_build=c_build,
                direct_defines=direct_defines,
                target_report=c_build and succeeds,
            )
            command[1:1] = ["--connect", endpoint]
            result = run(
                command,
                cwd=FIXTURE,
                expected_code=0 if succeeds else 1,
                label=label,
                no_server=False,
            )
            if not succeeds:
                if CONFIGURATION_DIAGNOSTIC_ID not in result.stdout + result.stderr:
                    raise ProbeFailure("compiler-server conflict lost its diagnostic")
                continue
            if c_build:
                assert_target_contract(result, expected_contract, label)
            else:
                assert_probe(result, NON_C_EXPECTED, label)

        with tempfile.TemporaryDirectory(prefix="reflaxe-c-server-production-") as temporary:
            output = Path(temporary) / "generated"
            command = production_command(output)
            command[1:1] = ["--connect", endpoint]
            production = run(
                command,
                cwd=FIXTURE,
                expected_code=1,
                label="compiler-server production lowering boundary",
                no_server=False,
            )
            if LOWERING_DIAGNOSTIC_ID not in production.stdout + production.stderr:
                raise ProbeFailure("compiler-server production request missed HXC1000")
            assert_target_contract(
                production,
                expected_contract,
                "compiler-server production lowering boundary",
            )
            if output.exists() and any(output.rglob("*")):
                raise ProbeFailure("compiler-server HXC1000 request emitted an artifact")

        recovery_command = source_command(c_build=True, target_report=True)
        recovery_command[1:1] = ["--connect", endpoint]
        recovery = run(
            recovery_command,
            cwd=FIXTURE,
            label="compiler-server recovery after HXC1000",
            no_server=False,
        )
        assert_target_contract(
            recovery, expected_contract, "compiler-server recovery after HXC1000"
        )
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
        run(
            [development_tool("lix"), "use", "haxe", "5.0.0-preview.1"],
            cwd=consumer,
            label="package Haxe pin",
        )
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
                "--macro",
                "TargetContractProbe.install()",
                "-main",
                "BootstrapProbe",
                "--custom-target",
                "c=bootstrap-probe-output",
            ],
            cwd=nested,
            label="staged package CWD probe",
        )
        expected_contract = json.loads(
            EXPECTED_TARGET_CONTRACT.read_text(encoding="utf-8")
        )
        assert_target_contract(
            package_probe, expected_contract, "staged package CWD probe"
        )

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
        check_production_carrier_fails_closed()
        check_compiler_server_isolation()
        check_package_layout()
    except ProbeFailure as error:
        print(f"bootstrap-policy: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "bootstrap-policy: OK: custom target, platform config, stdlib branches, fail-closed lowering, isolation, server, and package probes passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

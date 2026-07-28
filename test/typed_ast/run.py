#!/usr/bin/env python3
"""Compile-backed typed-AST normalization, inventory, order, and server probes."""

from __future__ import annotations

import json
import os
import shutil
import socket
import subprocess
import tempfile
import time
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURES = Path(__file__).resolve().parent / "fixtures"
EXPECTED = Path(__file__).resolve().parent / "expected/typed-ast-inventory.json"
REPORT_PREFIX = "HXC_TYPED_AST_INVENTORY="
INCREMENTAL_REPORT_PREFIX = "HXC_INCREMENTAL_INPUT="
LOWERING_DIAGNOSTIC_ID = "HXC1001"
LOWERING_EXPECTATIONS = {
    "rich": (
        "Unsupported typed Haxe node `TVar(box:type):generic-class-reference-requires-bounded-class-specialization:FixtureBox`",
        "Main.hx:8: characters 3-38",
    ),
    "isolation": (
        "Unsupported typed Haxe node `TTry`",
        "Main.hx:5: lines 5-9",
    ),
}


class TypedAstProbeFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class CompileResult:
    process: subprocess.CompletedProcess[str]
    payload: str
    report: dict[str, object]
    incremental_payload: str
    incremental_report: dict[str, object]


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def compile_fixture(
    fixture: str,
    *,
    reverse_modules: bool = False,
    connect: str | None = None,
    fixture_root: Path | None = None,
) -> CompileResult:
    selected_fixture_root = fixture_root or FIXTURES / fixture
    with tempfile.TemporaryDirectory(prefix=f"hxc-typed-ast-{fixture}-") as temporary:
        output = Path(temporary) / "generated"
        command = [development_tool("haxe")]
        if connect is not None:
            command.extend(["--connect", connect])
        command.extend(
            [
                "-cp",
                str(selected_fixture_root),
                "-lib",
                "reflaxe.c",
                "-D",
                "reflaxe_c_typed_ast_report",
                "-D",
                "reflaxe_c_incremental_input_report",
            ]
        )
        if reverse_modules:
            command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
        command.extend(["-main", "Main", "--custom-target", f"c={output}"])

        environment = os.environ.copy()
        if connect is None:
            environment["HAXE_NO_SERVER"] = "1"
        else:
            environment.pop("HAXE_NO_SERVER", None)
        process = subprocess.run(
            command,
            cwd=ROOT,
            env=environment,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        emitted = [path for path in Path(temporary).rglob("*") if path.is_file()]
        if emitted:
            raise TypedAstProbeFailure(
                f"{fixture} HXC1001 boundary emitted files: "
                + ", ".join(path.relative_to(temporary).as_posix() for path in emitted)
            )

    combined = process.stdout + process.stderr
    lowering_detail, lowering_source = LOWERING_EXPECTATIONS[fixture]
    if (
        process.returncode == 0
        or LOWERING_DIAGNOSTIC_ID not in combined
        or lowering_detail not in combined
        or lowering_source not in combined
    ):
        raise TypedAstProbeFailure(
            f"{fixture} missed its exact source-anchored HXC1001 boundary\n"
            f"stdout:\n{process.stdout}\nstderr:\n{process.stderr}"
        )
    typed_payload, typed_report = parse_report(
        process.stdout, REPORT_PREFIX, f"{fixture} typed-AST"
    )
    incremental_payload, incremental_report = parse_report(
        process.stdout, INCREMENTAL_REPORT_PREFIX, f"{fixture} incremental input"
    )
    return CompileResult(
        process,
        typed_payload,
        typed_report,
        incremental_payload,
        incremental_report,
    )


def parse_report(
    stdout: str, prefix: str, label: str
) -> tuple[str, dict[str, object]]:
    """Read one prefixed JSON report without accepting missing or duplicate data."""

    lines = [
        line[len(prefix) :]
        for line in stdout.splitlines()
        if line.startswith(prefix)
    ]
    if len(lines) != 1:
        raise TypedAstProbeFailure(
            f"{label} emitted {len(lines)} reports, expected exactly one\n"
            f"stdout:\n{stdout}"
        )
    try:
        report = json.loads(lines[0])
    except json.JSONDecodeError as error:
        raise TypedAstProbeFailure(
            f"{label} emitted invalid JSON: {error}"
        ) from error
    if not isinstance(report, dict):
        raise TypedAstProbeFailure(f"{label} report is not a JSON object")
    return lines[0], report


def count_map(report: dict[str, object], category: str) -> dict[str, int]:
    inventory = report.get("inventory")
    if not isinstance(inventory, dict) or not isinstance(inventory.get(category), list):
        raise TypedAstProbeFailure(f"inventory omitted {category}")
    result: dict[str, int] = {}
    for entry in inventory[category]:
        if (
            not isinstance(entry, dict)
            or not isinstance(entry.get("kind"), str)
            or not isinstance(entry.get("count"), int)
        ):
            raise TypedAstProbeFailure(f"invalid {category} entry: {entry!r}")
        result[entry["kind"]] = entry["count"]
    return result


def assert_fixture_classification(report: dict[str, object]) -> None:
    if report.get("schemaVersion") != 1 or report.get("status") != "normalized-typed-input-before-body-lowering":
        raise TypedAstProbeFailure("typed-AST report schema/status drifted")
    entry = report.get("entryPoint")
    if not isinstance(entry, dict) or entry.get("modulePath") != "Main" or entry.get("declarationPath") != "Main":
        raise TypedAstProbeFailure(f"entry point was not classified: {entry!r}")

    modules = report.get("modules")
    if not isinstance(modules, list):
        raise TypedAstProbeFailure("typed-AST report omitted modules")
    fixture_module = next(
        (module for module in modules if isinstance(module, dict) and module.get("path") == "FixtureTypes"),
        None,
    )
    if not isinstance(fixture_module, dict) or fixture_module.get("sourcePath") != "FixtureTypes.hx":
        raise TypedAstProbeFailure("logical fixture module/source ownership was not retained")
    declarations = fixture_module.get("declarations")
    if not isinstance(declarations, list):
        raise TypedAstProbeFailure("fixture module omitted declarations")

    by_name = {
        declaration.get("name"): declaration
        for declaration in declarations
        if isinstance(declaration, dict) and isinstance(declaration.get("name"), str)
    }
    required = {
        "FixtureTypes": ("class", "primary", False),
        "FixtureMarker": ("class", "secondary", False),
        "FixtureBox": ("class", "secondary", False),
        "FixtureNativeClock": ("class", "secondary", True),
        "FixtureState": ("enum", "secondary", False),
        "FixturePoint": ("typedef", "secondary", False),
        "FixtureCounter": ("abstract", "secondary", False),
    }
    for name, (kind, ownership, is_extern) in required.items():
        declaration = by_name.get(name)
        if not isinstance(declaration, dict):
            raise TypedAstProbeFailure(f"typed-AST adapter omitted {name}")
        if (
            declaration.get("kind") != kind
            or declaration.get("ownership") != ownership
            or declaration.get("ownerModulePath") != "FixtureTypes"
            or declaration.get("isExtern") is not is_extern
        ):
            raise TypedAstProbeFailure(f"typed-AST classification drifted for {name}: {declaration!r}")
    if by_name["FixtureMarker"].get("isInterface") is not True:
        raise TypedAstProbeFailure("interface status was not retained")

    declaration_kinds = count_map(report, "declarationKinds")
    expression_kinds = count_map(report, "expressionKinds")
    metadata_names = count_map(report, "metadataNames")
    for kind in ("class", "enum", "typedef", "abstract"):
        if declaration_kinds.get(kind, 0) == 0:
            raise TypedAstProbeFailure(f"declaration inventory omitted {kind}")
    # Haxe normalizes source-level `for` and anonymous literals before this
    # boundary. Assert the typed node kinds actually presented to the target.
    for kind in ("call", "switch", "while", "throw", "enum-parameter"):
        if expression_kinds.get(kind, 0) == 0:
            raise TypedAstProbeFailure(f"expression inventory omitted {kind}")
    if metadata_names.get(":typedAstFixture", 0) < len(required):
        raise TypedAstProbeFailure("metadata inventory omitted fixture declarations")

    expression_roots = report.get("expressionRoots")
    if not isinstance(expression_roots, list) or not any(
        isinstance(root, dict)
        and root.get("ownerDeclarationPath") == "FixtureBox"
        and root.get("fieldName") == "marker"
        and root.get("role") == "member"
        for root in expression_roots
    ):
        raise TypedAstProbeFailure("expression-root ownership inventory omitted FixtureBox.marker")

    payload = json.dumps(report, sort_keys=True)
    if str(ROOT) in payload:
        raise TypedAstProbeFailure("inventory leaked a host path")


def text_array(
    report: dict[str, object], field: str, *, label: str
) -> list[str]:
    """Validate one deterministic string-array field from a diagnostic report."""

    value = report.get(field)
    if not isinstance(value, list) or any(not isinstance(item, str) for item in value):
        raise TypedAstProbeFailure(f"{label} has an invalid {field} array")
    if value != sorted(set(value)):
        raise TypedAstProbeFailure(f"{label} {field} is not sorted and unique")
    return value


def assert_incremental_report(
    report: dict[str, object],
    *,
    mode: str,
    required_rebuilt: set[str],
    required_reused: set[str],
    forbidden_rebuilt: set[str],
    forbidden_reused: set[str],
    require_empty_reused: bool = False,
    required_non_classes: set[str] | None = None,
) -> None:
    """Check the bounded class evidence without treating it as backend reuse."""

    label = f"{mode} incremental input report"
    if report.get("schemaVersion") != 1 or report.get("mode") != mode:
        raise TypedAstProbeFailure(f"{label} schema/mode drifted: {report!r}")
    actual_rebuilt = set(
        text_array(report, "rebuiltClassDeclarations", label=label)
    )
    actual_reused = set(
        text_array(report, "frontendReusedClassDeclarations", label=label)
    )
    if (
        not required_rebuilt.issubset(actual_rebuilt)
        or not required_reused.issubset(actual_reused)
        or forbidden_rebuilt.intersection(actual_rebuilt)
        or forbidden_reused.intersection(actual_reused)
        or actual_rebuilt.intersection(actual_reused)
        or require_empty_reused
        and actual_reused
    ):
        raise TypedAstProbeFailure(
            f"{label} class partition drifted: "
            f"rebuilt={sorted(actual_rebuilt)!r}, reused={sorted(actual_reused)!r}"
        )
    non_classes = set(
        text_array(
            report,
            "conservativelyReconsideredNonClassDeclarations",
            label=label,
        )
    )
    if required_non_classes is None:
        required_non_classes = {
            "FixtureCounter",
            "FixturePoint",
            "FixtureState",
        }
    if not required_non_classes.issubset(non_classes):
        raise TypedAstProbeFailure(
            f"{label} stopped conservatively carrying non-class declarations: "
            f"{sorted(non_classes)!r}"
        )
    unmatched = text_array(
        report, "unmatchedHaxeRebuiltClassPaths", label=label
    )
    if unmatched:
        raise TypedAstProbeFailure(
            f"{label} contained target-external rebuilt classes: {unmatched!r}"
        )
    payload = json.dumps(report, sort_keys=True)
    if str(ROOT) in payload:
        raise TypedAstProbeFailure(
            "incremental input inventory leaked a host path"
        )


def render_snapshot() -> CompileResult:
    forward = compile_fixture("rich")
    reverse = compile_fixture("rich", reverse_modules=True)
    repeated = compile_fixture("rich")
    if forward.payload != reverse.payload:
        raise TypedAstProbeFailure("normalized report changed when captured module order was reversed")
    if forward.payload != repeated.payload:
        raise TypedAstProbeFailure("two cold typed-AST inventory renders differed")
    assert_fixture_classification(forward.report)
    all_classes = {
        "FixtureBox",
        "FixtureMarker",
        "FixtureNativeClock",
        "FixtureTypes",
        "Main",
    }
    assert_incremental_report(
        forward.incremental_report,
        mode="cold-complete",
        required_rebuilt=all_classes,
        required_reused=set(),
        forbidden_rebuilt=set(),
        forbidden_reused=all_classes,
        require_empty_reused=True,
    )
    return forward


def check_expected_snapshot() -> None:
    actual = render_snapshot().report
    expected = json.loads(EXPECTED.read_text(encoding="utf-8"))
    if actual != expected:
        raise TypedAstProbeFailure(
            "typed-AST inventory snapshot drifted\n"
            f"expected:\n{json.dumps(expected, indent=2, sort_keys=True)}\n"
            f"actual:\n{json.dumps(actual, indent=2, sort_keys=True)}"
        )


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise TypedAstProbeFailure(
                f"Haxe compiler server exited early\nstdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise TypedAstProbeFailure("Haxe compiler server did not accept connections within 10 seconds")


def check_compiler_server_isolation() -> None:
    cold_isolation = compile_fixture("isolation")
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
        rich = compile_fixture("rich", connect=endpoint)
        first_isolation = compile_fixture("isolation", connect=endpoint)
        repeated_isolation = compile_fixture("isolation", connect=endpoint)
        if rich.payload == first_isolation.payload:
            raise TypedAstProbeFailure("server isolation fixtures unexpectedly produced the same inventory")
        if cold_isolation.payload != first_isolation.payload or first_isolation.payload != repeated_isolation.payload:
            raise TypedAstProbeFailure("typed modules or mutable context leaked across compiler-server requests")
        isolation_modules = {
            module.get("path")
            for module in first_isolation.report.get("modules", [])
            if isinstance(module, dict)
        }
        if "FixtureTypes" in isolation_modules:
            raise TypedAstProbeFailure(f"compiler-server module ownership leaked: {sorted(isolation_modules)!r}")
        main_module = next(
            (
                module
                for module in first_isolation.report.get("modules", [])
                if isinstance(module, dict) and module.get("path") == "Main"
            ),
            None,
        )
        if not isinstance(main_module, dict):
            raise TypedAstProbeFailure("compiler-server isolation report omitted Main module")
        isolation_declarations = {
            declaration.get("name")
            for declaration in main_module.get("declarations", [])
            if isinstance(declaration, dict)
        }
        if isolation_declarations != {"Main", "IsolationOnly"}:
            raise TypedAstProbeFailure(
                f"compiler-server secondary declaration ownership drifted: {sorted(isolation_declarations)!r}"
            )
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def compile_successful_incremental_fixture(
    fixture_root: Path, *, connect: str, output: Path
) -> CompileResult:
    """Compile one successful fixture so Haxe may commit its frontend cache."""

    command = [
        development_tool("haxe"),
        "--connect",
        connect,
        "-cp",
        str(fixture_root),
        "-lib",
        "reflaxe.c",
        "-D",
        "hxc_runtime_diagnostics=off",
        "-D",
        "reflaxe_c_incremental_input_report",
        "-main",
        "Main",
        "--custom-target",
        f"c={output}",
    ]
    environment = os.environ.copy()
    environment.pop("HAXE_NO_SERVER", None)
    process = subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if process.returncode != 0:
        raise TypedAstProbeFailure(
            "successful incremental fixture failed\n"
            f"stdout:\n{process.stdout}\nstderr:\n{process.stderr}"
        )
    emitted = [
        path
        for path in output.rglob("*")
        if path.is_file() and path.name != "_GeneratedFiles.json"
    ]
    if not emitted:
        raise TypedAstProbeFailure(
            "successful incremental fixture emitted no C project"
        )
    incremental_payload, incremental_report = parse_report(
        process.stdout,
        INCREMENTAL_REPORT_PREFIX,
        "successful incremental fixture",
    )
    return CompileResult(
        process,
        "",
        {},
        incremental_payload,
        incremental_report,
    )


def check_compiler_server_rebuild_inventory() -> None:
    """Change one Haxe module and prove the server report's exact safe boundary."""

    all_classes = {
        "ChangedWorker",
        "Main",
        "StableWorker",
        "_WorkerResult.WorkerResult_Fields_",
    }
    with tempfile.TemporaryDirectory(prefix="hxc-typed-ast-edit-") as temporary:
        editable = Path(temporary) / "incremental"
        shutil.copytree(FIXTURES / "incremental", editable)
        output = Path(temporary) / "generated"
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
            prime = compile_successful_incremental_fixture(
                editable, connect=endpoint, output=output
            )
            unchanged = compile_successful_incremental_fixture(
                editable, connect=endpoint, output=output
            )
            source = editable / "WorkerResult.hx"
            original = source.read_text(encoding="utf-8")
            changed = original.replace("changed: 4,", "changed: 5,", 1)
            if changed == original:
                raise TypedAstProbeFailure(
                    "incremental fixture edit anchor was not found"
                )
            source.write_text(changed, encoding="utf-8")
            edited = compile_successful_incremental_fixture(
                editable, connect=endpoint, output=output
            )

            assert_incremental_report(
                prime.incremental_report,
                mode="cold-complete",
                required_rebuilt=all_classes,
                required_reused=set(),
                forbidden_rebuilt=set(),
                forbidden_reused=all_classes,
                require_empty_reused=True,
                required_non_classes=set(),
            )
            assert_incremental_report(
                unchanged.incremental_report,
                mode="server-partial",
                required_rebuilt=set(),
                required_reused=all_classes,
                forbidden_rebuilt=all_classes,
                forbidden_reused=set(),
                required_non_classes=set(),
            )
            assert_incremental_report(
                edited.incremental_report,
                mode="server-partial",
                required_rebuilt={
                    "ChangedWorker",
                    "Main",
                    "_WorkerResult.WorkerResult_Fields_",
                },
                required_reused={"StableWorker"},
                forbidden_rebuilt={"StableWorker"},
                forbidden_reused={
                    "ChangedWorker",
                    "Main",
                    "_WorkerResult.WorkerResult_Fields_",
                },
                required_non_classes=set(),
            )
            if unchanged.incremental_payload == edited.incremental_payload:
                raise TypedAstProbeFailure(
                    "the edited request unexpectedly kept the same rebuild evidence"
                )
        finally:
            server.terminate()
            try:
                server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                server.kill()
                server.wait(timeout=5)


def main() -> int:
    try:
        check_expected_snapshot()
        check_compiler_server_isolation()
        check_compiler_server_rebuild_inventory()
    except (OSError, json.JSONDecodeError, subprocess.TimeoutExpired, TypedAstProbeFailure) as error:
        print(f"typed-ast: ERROR: {error}", file=os.sys.stderr)
        return 1
    print(
        "typed-ast: OK: declarations/metadata/entry ownership, order determinism, "
        "inventory coverage, exact HXC1001 no-output, compiler-server isolation, "
        "and one-module rebuild evidence"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

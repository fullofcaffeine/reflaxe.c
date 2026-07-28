#!/usr/bin/env python3
"""Compile-backed typed-AST normalization, inventory, order, and server probes."""

from __future__ import annotations

import json
import os
import re
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
CONSTRUCTOR_REPORT_PREFIX = "HXC_CONSTRUCTOR_LOWERING="
CONTROL_FLOW_CACHE_REPORT_PREFIX = "HXC_CONTROL_FLOW_PLAN_CACHE="
BODY_FUNCTION_REPLAY_REPORT_PREFIX = "HXC_BODY_FUNCTION_REPLAY_CACHE="
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
    hxcir: str | None = None
    generated_tree: dict[str, bytes] | None = None
    control_flow_cache_report: dict[str, object] | None = None
    body_function_replay_report: dict[str, object] | None = None


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
    fixture_root: Path,
    *,
    connect: str | None,
    output: Path,
    disable_control_flow_cache: bool = False,
) -> CompileResult:
    """Compile one successful fixture so Haxe may commit its frontend cache."""

    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend([
        "-cp",
        str(fixture_root),
        "-lib",
        "reflaxe.c",
        "-D",
        "hxc_runtime_diagnostics=off",
        "-D",
        "reflaxe_c_incremental_input_report",
        "-D",
        "reflaxe_c_constructor_lowering_report",
        "-D",
        "reflaxe_c_control_flow_plan_cache_report",
        "-D",
        "reflaxe_c_body_function_replay_cache_report",
        "-main",
        "Main",
        "--custom-target",
        f"c={output}",
    ])
    if disable_control_flow_cache:
        command.extend(
            [
                "-D",
                "reflaxe_c_test_disable_control_flow_plan_cache",
                "-D",
                "reflaxe_c_test_disable_body_function_replay_cache",
            ]
        )
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
    _, constructor_report = parse_report(
        process.stdout,
        CONSTRUCTOR_REPORT_PREFIX,
        "successful incremental HxcIR",
    )
    hxcir = constructor_report.get("hxcir")
    if not isinstance(hxcir, str) or not hxcir:
        raise TypedAstProbeFailure("successful incremental fixture omitted HxcIR")
    _, control_flow_cache_report = parse_report(
        process.stdout,
        CONTROL_FLOW_CACHE_REPORT_PREFIX,
        "successful incremental control-flow cache",
    )
    _, body_function_replay_report = parse_report(
        process.stdout,
        BODY_FUNCTION_REPLAY_REPORT_PREFIX,
        "successful incremental body-function replay cache",
    )
    return CompileResult(
        process,
        "",
        {},
        incremental_payload,
        incremental_report,
        hxcir,
        {
            path.relative_to(output).as_posix(): path.read_bytes()
            for path in sorted(output.rglob("*"))
            if path.is_file() and path.name != "_GeneratedFiles.json"
        },
        control_flow_cache_report,
        body_function_replay_report,
    )


def compile_failed_incremental_fixture(
    fixture_root: Path, *, connect: str | None, output: Path
) -> subprocess.CompletedProcess[str]:
    """Compile one unsupported mutation without accepting stale generated output."""

    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
            "-cp",
            str(fixture_root),
            "-lib",
            "reflaxe.c",
            "-D",
            "hxc_runtime_diagnostics=off",
            "-main",
            "Main",
            "--custom-target",
            f"c={output}",
        ]
    )
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
        timeout=60,
    )
    if process.returncode == 0 or "HXC1001" not in process.stderr:
        raise TypedAstProbeFailure(
            "unsupported named-record mutation did not fail with HXC1001\n"
            f"stdout:\n{process.stdout}\nstderr:\n{process.stderr}"
        )
    emitted = [
        path
        for path in output.rglob("*")
        if path.is_file() and path.name != "_GeneratedFiles.json"
    ]
    if emitted:
        raise TypedAstProbeFailure(
            "unsupported named-record mutation emitted production files: "
            + ", ".join(path.relative_to(output).as_posix() for path in emitted)
        )
    return process


def require_hxcir(result: CompileResult, label: str) -> str:
    """Return one successful request's semantic tree with an actionable failure."""

    if not isinstance(result.hxcir, str) or not result.hxcir:
        raise TypedAstProbeFailure(f"{label} omitted HxcIR")
    return result.hxcir


def require_generated_tree(result: CompileResult, label: str) -> dict[str, bytes]:
    """Return one successful request's immutable generated-file snapshot."""

    if not isinstance(result.generated_tree, dict) or not result.generated_tree:
        raise TypedAstProbeFailure(f"{label} omitted its generated project")
    return result.generated_tree


def assert_equivalent_incremental_result(
    expected: CompileResult, actual: CompileResult, label: str
) -> None:
    """Require semantic provenance and generated bytes to match exactly."""

    if require_hxcir(expected, "expected result") != require_hxcir(actual, label):
        raise TypedAstProbeFailure(f"{label} HxcIR differed from its cold oracle")
    if require_generated_tree(
        expected, "expected result"
    ) != require_generated_tree(actual, label):
        raise TypedAstProbeFailure(
            f"{label} generated project differed from its cold oracle"
        )


def require_control_flow_cache_report(
    result: CompileResult, label: str
) -> dict[str, object]:
    """Return one successful request's fail-closed cache evidence."""

    report = result.control_flow_cache_report
    if not isinstance(report, dict):
        raise TypedAstProbeFailure(
            f"{label} omitted its control-flow plan cache report"
        )
    return report


def assert_control_flow_cache(
    result: CompileResult,
    *,
    label: str,
    enabled: bool,
    hits: int,
    misses: int,
    retained_functions: int,
) -> None:
    """Check exact cache accounting instead of inferring reuse from elapsed time."""

    report = require_control_flow_cache_report(result, label)
    expected = {
        "enabled": enabled,
        "hits": hits,
        "misses": misses,
        "retainedFunctions": retained_functions,
    }
    for field, value in expected.items():
        if report.get(field) != value:
            raise TypedAstProbeFailure(
                f"{label} cache field {field} was {report.get(field)!r}, "
                f"expected {value!r}: {report!r}"
            )
    retained_key_units = report.get("retainedKeyCodeUnits")
    expected_positive_key_size = retained_functions > 0
    if not isinstance(retained_key_units, (int, float)) or (
        expected_positive_key_size and retained_key_units <= 0
    ) or (not expected_positive_key_size and retained_key_units != 0):
        raise TypedAstProbeFailure(
            f"{label} did not report a bounded retained key size: {report!r}"
        )


def control_flow_function_count(result: CompileResult, label: str) -> int:
    """Read the complete successful generation size from its first-request misses."""

    report = require_control_flow_cache_report(result, label)
    misses = report.get("misses")
    retained = report.get("retainedFunctions")
    if (
        report.get("enabled") is not True
        or not isinstance(misses, int)
        or misses <= 0
        or retained != misses
    ):
        raise TypedAstProbeFailure(
            f"{label} was not a complete cold cache generation: {report!r}"
        )
    return misses


def require_body_function_replay_report(
    result: CompileResult, label: str
) -> dict[str, object]:
    """Return one request's exact semantic-function replay accounting."""

    report = result.body_function_replay_report
    if not isinstance(report, dict):
        raise TypedAstProbeFailure(
            f"{label} omitted its body-function replay cache report"
        )
    return report


def assert_body_function_replay(
    result: CompileResult,
    *,
    label: str,
    enabled: bool,
    program_decision: str,
    program_revision_matched: bool,
    hits: int,
    misses: int,
    missing_function_misses: int,
    changed_function_input_misses: int,
    retained_functions: int,
) -> None:
    """Prove which complete HxcIR functions were replayed, rebuilt, and retained."""

    report = require_body_function_replay_report(result, label)
    expected = {
        "enabled": enabled,
        "programDecision": program_decision,
        "programRevisionMatched": program_revision_matched,
        "hits": hits,
        "misses": misses,
        "missingFunctionMisses": missing_function_misses,
        "changedFunctionInputMisses": changed_function_input_misses,
        "retainedFunctions": retained_functions,
    }
    for field, value in expected.items():
        if report.get(field) != value:
            raise TypedAstProbeFailure(
                f"{label} body-function replay field {field} was "
                f"{report.get(field)!r}, expected {value!r}: {report!r}"
            )
    retained_input_units = report.get("retainedInputCodeUnits")
    retained_revision_units = report.get("retainedProgramRevisionCodeUnits")
    expect_retained_text = retained_functions > 0
    for field, value in (
        ("retainedInputCodeUnits", retained_input_units),
        ("retainedProgramRevisionCodeUnits", retained_revision_units),
    ):
        if not isinstance(value, (int, float)) or (
            expect_retained_text and value <= 0
        ) or (not expect_retained_text and value != 0):
            raise TypedAstProbeFailure(
                f"{label} reported an invalid bounded replay size for {field}: "
                f"{report!r}"
            )


def body_function_count(result: CompileResult, label: str) -> int:
    """Read the complete semantic-function generation size from a cold request."""

    report = require_body_function_replay_report(result, label)
    misses = report.get("misses")
    retained = report.get("retainedFunctions")
    if (
        report.get("enabled") is not True
        or report.get("programDecision") != "no-prior-generation"
        or report.get("programRevisionMatched") is not False
        or not isinstance(misses, int)
        or misses <= 0
        or retained != misses
        or report.get("missingFunctionMisses") != 0
        or report.get("changedFunctionInputMisses") != 0
    ):
        raise TypedAstProbeFailure(
            f"{label} was not a complete cold body-function generation: {report!r}"
        )
    return misses


def assert_worker_record_sources(
    result: CompileResult, expected_fields: dict[str, tuple[int, int, int, int]], label: str
) -> None:
    """Prove record fields point to the typedef, never its object-literal use."""

    hxcir = require_hxcir(result, label)
    module_start = hxcir.find('module "WorkerResult"')
    module_end = hxcir.find('end module "WorkerResult"', module_start)
    if module_start < 0 or module_end < 0:
        raise TypedAstProbeFailure(f"{label} omitted the WorkerResult HxcIR module")
    section = hxcir[module_start:module_end]
    fields: dict[str, tuple[int, int, int, int]] = {}
    pattern = re.compile(
        r'^\s+field "([^"]+)".* @"WorkerResult\.hx":'
        r"(\d+):(\d+)-(\d+):(\d+)$",
        re.MULTILINE,
    )
    for match in pattern.finditer(section):
        fields[match.group(1)] = (
            int(match.group(2)),
            int(match.group(3)),
            int(match.group(4)),
            int(match.group(5)),
        )
    if fields != expected_fields:
        raise TypedAstProbeFailure(
            f"{label} named-record declaration sources drifted: {fields!r}"
        )


def stop_server(server: subprocess.Popen[str]) -> None:
    """Stop exactly the Haxe server process created by this test."""

    server.terminate()
    try:
        server.wait(timeout=5)
    except subprocess.TimeoutExpired:
        server.kill()
        server.wait(timeout=5)


def check_compiler_server_rebuild_inventory() -> None:
    """Prove frontend invalidation and exact named-record provenance together."""

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
            function_count = control_flow_function_count(prime, "server prime")
            replay_function_count = body_function_count(prime, "server prime")
            assert_body_function_replay(
                prime,
                label="server prime",
                enabled=True,
                program_decision="no-prior-generation",
                program_revision_matched=False,
                hits=0,
                misses=replay_function_count,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=replay_function_count,
            )
            assert_control_flow_cache(
                unchanged,
                label="unchanged warm request",
                enabled=True,
                hits=function_count,
                misses=0,
                retained_functions=function_count,
            )
            assert_body_function_replay(
                unchanged,
                label="unchanged warm request",
                enabled=True,
                program_decision="matched",
                program_revision_matched=True,
                hits=replay_function_count,
                misses=0,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=replay_function_count,
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
            edited_cold = compile_successful_incremental_fixture(
                editable,
                connect=None,
                output=Path(temporary) / "edited-cold",
            )
            assert_control_flow_cache(
                edited,
                label="implementation edit",
                enabled=True,
                hits=function_count - 1,
                misses=1,
                retained_functions=function_count,
            )
            assert_control_flow_cache(
                edited_cold,
                label="cold implementation edit",
                enabled=True,
                hits=0,
                misses=function_count,
                retained_functions=function_count,
            )
            assert_body_function_replay(
                edited,
                label="implementation edit",
                enabled=True,
                program_decision="matched",
                program_revision_matched=True,
                hits=replay_function_count - 1,
                misses=1,
                missing_function_misses=0,
                changed_function_input_misses=1,
                retained_functions=replay_function_count,
            )
            assert_body_function_replay(
                edited_cold,
                label="cold implementation edit",
                enabled=True,
                program_decision="no-prior-generation",
                program_revision_matched=False,
                hits=0,
                misses=replay_function_count,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=replay_function_count,
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
            original_fields = {
                "changed": (12, 2, 12, 20),
                "stable": (15, 2, 15, 19),
            }
            assert_worker_record_sources(prime, original_fields, "server prime")
            assert_worker_record_sources(
                unchanged, original_fields, "unchanged warm request"
            )
            assert_equivalent_incremental_result(
                prime, unchanged, "unchanged warm request"
            )
            assert_worker_record_sources(
                edited, original_fields, "implementation edit"
            )
            assert_equivalent_incremental_result(
                edited_cold, edited, "implementation edit"
            )

            main_source = editable / "Main.hx"
            original_main = main_source.read_text(encoding="utf-8")
            public_record = (
                original.replace("final stable:Int;", "final steady:Int;", 1)
                .replace("stable: StableWorker.value()", "steady: StableWorker.value()", 1)
            )
            public_main = original_main.replace(
                "result.changed + result.stable",
                "result.changed + result.steady",
                1,
            )
            if public_record == original or public_main == original_main:
                raise TypedAstProbeFailure(
                    "public typedef edit anchors were not found"
                )
            source.write_text(public_record, encoding="utf-8")
            main_source.write_text(public_main, encoding="utf-8")
            public_edit = compile_successful_incremental_fixture(
                editable, connect=endpoint, output=output
            )
            public_cold = compile_successful_incremental_fixture(
                editable,
                connect=None,
                output=Path(temporary) / "public-cold",
            )
            assert_worker_record_sources(
                public_edit,
                {
                    "changed": (12, 2, 12, 20),
                    "steady": (15, 2, 15, 19),
                },
                "public typedef edit",
            )
            assert_equivalent_incremental_result(
                public_cold, public_edit, "public typedef edit"
            )
            public_report = require_control_flow_cache_report(
                public_edit, "public typedef edit"
            )
            public_hits = public_report.get("hits")
            public_misses = public_report.get("misses")
            if (
                not isinstance(public_hits, int)
                or not isinstance(public_misses, int)
                or public_misses <= 0
                or public_hits + public_misses != function_count
            ):
                raise TypedAstProbeFailure(
                    "public typedef edit did not conservatively invalidate its "
                    f"changed semantic functions: {public_report!r}"
                )
            assert_body_function_replay(
                public_edit,
                label="public typedef edit",
                enabled=True,
                program_decision="program-changed",
                program_revision_matched=False,
                hits=0,
                misses=replay_function_count,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=replay_function_count,
            )
            assert_body_function_replay(
                public_cold,
                label="cold public typedef edit",
                enabled=True,
                program_decision="no-prior-generation",
                program_revision_matched=False,
                hits=0,
                misses=replay_function_count,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=replay_function_count,
            )

            unsupported = original.replace(
                "final stable:Int;", "final stable:Dynamic;", 1
            )
            if unsupported == original:
                raise TypedAstProbeFailure(
                    "unsupported typedef edit anchor was not found"
                )
            source.write_text(unsupported, encoding="utf-8")
            main_source.write_text(original_main, encoding="utf-8")
            failure_output = Path(temporary) / "failed"
            failed_first = compile_failed_incremental_fixture(
                editable, connect=endpoint, output=failure_output
            )
            failed_warm = compile_failed_incremental_fixture(
                editable, connect=endpoint, output=failure_output
            )
            failed_cold = compile_failed_incremental_fixture(
                editable,
                connect=None,
                output=Path(temporary) / "failed-cold",
            )
            for label, failure in (
                ("first server diagnostic", failed_first),
                ("warm server diagnostic", failed_warm),
                ("cold diagnostic", failed_cold),
            ):
                if "WorkerResult.hx:15: characters 2-23" not in failure.stderr:
                    raise TypedAstProbeFailure(
                        f"{label} did not point at the typedef field\n{failure.stderr}"
                    )
            if (
                failed_first.stderr != failed_warm.stderr
                or failed_first.stderr != failed_cold.stderr
            ):
                raise TypedAstProbeFailure(
                    "cold and warm named-record diagnostics differed"
                )

            source.write_text(original, encoding="utf-8")
            main_source.write_text(original_main, encoding="utf-8")
            reverted = compile_successful_incremental_fixture(
                editable, connect=endpoint, output=output
            )
            assert_equivalent_incremental_result(
                prime, reverted, "request-order restoration"
            )
            reverted_report = require_control_flow_cache_report(
                reverted, "request-order restoration"
            )
            reverted_hits = reverted_report.get("hits")
            reverted_misses = reverted_report.get("misses")
            if (
                not isinstance(reverted_hits, int)
                or not isinstance(reverted_misses, int)
                or reverted_misses <= 0
                or reverted_hits + reverted_misses != function_count
            ):
                raise TypedAstProbeFailure(
                    "success-failure-success restoration did not compare against "
                    f"the last successful generation: {reverted_report!r}"
                )
            assert_body_function_replay(
                reverted,
                label="success-failure-success restoration",
                enabled=True,
                # Haxe discards this macro context after the failed type shape.
                # A fresh generation is the fail-closed outcome; the rejected
                # request never becomes replay authority.
                program_decision="no-prior-generation",
                program_revision_matched=False,
                hits=0,
                misses=replay_function_count,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=replay_function_count,
            )

            second_worktree = Path(temporary) / "second-worktree"
            shutil.copytree(editable, second_worktree)
            second_result = compile_successful_incremental_fixture(
                second_worktree, connect=endpoint, output=output
            )
            assert_equivalent_incremental_result(
                prime, second_result, "second-worktree request"
            )
            assert_control_flow_cache(
                second_result,
                label="second-worktree request",
                enabled=True,
                hits=function_count,
                misses=0,
                retained_functions=function_count,
            )
            assert_body_function_replay(
                second_result,
                label="second-worktree request",
                enabled=True,
                program_decision="matched",
                program_revision_matched=True,
                hits=replay_function_count,
                misses=0,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=replay_function_count,
            )
            cache_off = compile_successful_incremental_fixture(
                editable,
                connect=endpoint,
                output=output,
                disable_control_flow_cache=True,
            )
            assert_equivalent_incremental_result(
                prime, cache_off, "cache-disabled warm request"
            )
            assert_control_flow_cache(
                cache_off,
                label="cache-disabled warm request",
                enabled=False,
                hits=0,
                misses=0,
                # Haxe gives a changed define signature its own persistent macro
                # context. The disabled request must therefore expose no retained
                # enabled-cache state rather than borrowing another context's data.
                retained_functions=0,
            )
            assert_body_function_replay(
                cache_off,
                label="cache-disabled warm request",
                enabled=False,
                program_decision="disabled",
                program_revision_matched=False,
                hits=0,
                misses=0,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=0,
            )
        finally:
            stop_server(server)

        cold_without_server = compile_successful_incremental_fixture(
            editable,
            connect=None,
            output=Path(temporary) / "cold-without-server",
        )
        assert_equivalent_incremental_result(
            prime, cold_without_server, "cache-off cold request"
        )
        assert_control_flow_cache(
            cold_without_server,
            label="fresh-process cold request",
            enabled=True,
            hits=0,
            misses=function_count,
            retained_functions=function_count,
        )
        assert_body_function_replay(
            cold_without_server,
            label="fresh-process cold request",
            enabled=True,
            program_decision="no-prior-generation",
            program_revision_matched=False,
            hits=0,
            misses=replay_function_count,
            missing_function_misses=0,
            changed_function_input_misses=0,
            retained_functions=replay_function_count,
        )

        restarted_port = available_port()
        restarted_endpoint = str(restarted_port)
        restarted = subprocess.Popen(
            [development_tool("haxe"), "--wait", restarted_endpoint],
            cwd=ROOT,
            env=environment,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        try:
            wait_for_server(restarted, restarted_port)
            restarted_first = compile_successful_incremental_fixture(
                editable, connect=restarted_endpoint, output=output
            )
            restarted_warm = compile_successful_incremental_fixture(
                editable, connect=restarted_endpoint, output=output
            )
            assert_equivalent_incremental_result(
                prime, restarted_first, "restarted-server first request"
            )
            assert_equivalent_incremental_result(
                prime, restarted_warm, "restarted-server warm request"
            )
            assert_control_flow_cache(
                restarted_first,
                label="restarted-server first request",
                enabled=True,
                hits=0,
                misses=function_count,
                retained_functions=function_count,
            )
            assert_control_flow_cache(
                restarted_warm,
                label="restarted-server warm request",
                enabled=True,
                hits=function_count,
                misses=0,
                retained_functions=function_count,
            )
            assert_body_function_replay(
                restarted_first,
                label="restarted-server first request",
                enabled=True,
                program_decision="no-prior-generation",
                program_revision_matched=False,
                hits=0,
                misses=replay_function_count,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=replay_function_count,
            )
            assert_body_function_replay(
                restarted_warm,
                label="restarted-server warm request",
                enabled=True,
                program_decision="matched",
                program_revision_matched=True,
                hits=replay_function_count,
                misses=0,
                missing_function_misses=0,
                changed_function_input_misses=0,
                retained_functions=replay_function_count,
            )
        finally:
            stop_server(restarted)


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
        "one-module rebuild evidence, exact named-record provenance, and exact "
        "validated control-flow and semantic-function replay"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

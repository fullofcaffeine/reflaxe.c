#!/usr/bin/env python3
"""Compile-backed typed-AST normalization, inventory, order, and server probes."""

from __future__ import annotations

import json
import os
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
LOWERING_DIAGNOSTIC_ID = "HXC1001"
LOWERING_EXPECTATIONS = {
    "rich": (
        "Unsupported typed Haxe node `TVar(box:type):reference-FixtureBox-non-null`",
        "Main.hx:8: characters 3-38",
    ),
    "isolation": (
        "Unsupported typed Haxe node `TIf`",
        "Main.hx:4: lines 4-6",
    ),
}


class TypedAstProbeFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class CompileResult:
    process: subprocess.CompletedProcess[str]
    payload: str
    report: dict[str, object]


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def compile_fixture(
    fixture: str,
    *,
    reverse_modules: bool = False,
    connect: str | None = None,
) -> CompileResult:
    with tempfile.TemporaryDirectory(prefix=f"hxc-typed-ast-{fixture}-") as temporary:
        output = Path(temporary) / "generated"
        command = [development_tool("haxe")]
        if connect is not None:
            command.extend(["--connect", connect])
        command.extend(
            [
                "-cp",
                str(FIXTURES / fixture),
                "-lib",
                "reflaxe.c",
                "-D",
                "reflaxe_c_typed_ast_report",
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
    lines = [
        line[len(REPORT_PREFIX) :]
        for line in process.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(lines) != 1:
        raise TypedAstProbeFailure(
            f"{fixture} emitted {len(lines)} typed-AST reports, expected exactly one\n"
            f"stdout:\n{process.stdout}"
        )
    try:
        report = json.loads(lines[0])
    except json.JSONDecodeError as error:
        raise TypedAstProbeFailure(f"{fixture} emitted invalid inventory JSON: {error}") from error
    return CompileResult(process, lines[0], report)


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


def render_snapshot() -> CompileResult:
    forward = compile_fixture("rich")
    reverse = compile_fixture("rich", reverse_modules=True)
    repeated = compile_fixture("rich")
    if forward.payload != reverse.payload:
        raise TypedAstProbeFailure("normalized report changed when captured module order was reversed")
    if forward.payload != repeated.payload:
        raise TypedAstProbeFailure("two cold typed-AST inventory renders differed")
    assert_fixture_classification(forward.report)
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


def main() -> int:
    try:
        check_expected_snapshot()
        check_compiler_server_isolation()
    except (OSError, json.JSONDecodeError, subprocess.TimeoutExpired, TypedAstProbeFailure) as error:
        print(f"typed-ast: ERROR: {error}", file=os.sys.stderr)
        return 1
    print(
        "typed-ast: OK: declarations/metadata/entry ownership, order determinism, "
        "inventory coverage, exact HXC1001 no-output, and compiler-server isolation"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

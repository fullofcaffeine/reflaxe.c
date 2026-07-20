#!/usr/bin/env python3
"""Prove deterministic Haxe static initialization through HxcIR and strict C11."""

from __future__ import annotations

import argparse
import difflib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Iterable
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
POSITIVE = Path(__file__).with_name("fixtures") / "positive"
CYCLE = Path(__file__).with_name("fixtures") / "cycle"
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
EXPECTED_ORACLE = "123,7,123,12,12,17,152,446"
EXPECTED_TYPE_ORDER = [
    "MStaticInitTrace",
    "ZStaticInitPrerequisite",
    "AStaticInitDependent",
]
EXPECTED_EXECUTION_ORDER = [
    "initializer.MStaticInitTrace.class",
    "initializer.ZStaticInitPrerequisite.class",
    "initializer.AStaticInitDependent.class",
    "initializer.MStaticInitTrace.field.classOrder",
    "initializer.MStaticInitTrace.field.phaseValue",
    "initializer.MStaticInitTrace.field.trace",
    "initializer.ZStaticInitPrerequisite.field.value",
    "initializer.AStaticInitDependent.field.first",
    "initializer.AStaticInitDependent.field.second",
    "initializer.AStaticInitDependent.field.value",
]
NATIVE_OUTCOME_SOURCE_SYMBOL = "StaticInitializationFixture.outcome"
NATIVE_SOURCE_SYMBOLS = (NATIVE_OUTCOME_SOURCE_SYMBOL,)
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-pedantic-errors",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)


class StaticInitializationFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class NativeToolchain:
    family: str
    compiler: str
    version: str


@dataclass(frozen=True)
class ProductionRender:
    output: Path
    report_payload: str
    report: dict[str, object]


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def base_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return environment


def run_oracle(root: Path) -> str:
    javascript = root / "oracle.js"
    compiled = subprocess.run(
        [
            development_tool("haxe"),
            "-cp",
            str(POSITIVE),
            "-main",
            "StaticInitializationFixture",
            "-D",
            "static_initialization_oracle",
            "-js",
            str(javascript),
        ],
        cwd=ROOT,
        env=base_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise StaticInitializationFailure(
            "pinned Haxe JavaScript oracle failed\n"
            f"stdout:\n{compiled.stdout}\nstderr:\n{compiled.stderr}"
        )
    node = shutil.which("node")
    if node is None:
        raise StaticInitializationFailure("Node.js is required for the pinned Haxe JavaScript oracle")
    ran = subprocess.run(
        [node, str(javascript)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=15,
    )
    if ran.returncode != 0 or ran.stderr or not ran.stdout.rstrip().endswith(EXPECTED_ORACLE):
        raise StaticInitializationFailure(
            "Haxe JavaScript static-initialization oracle drifted\n"
            f"exit={ran.returncode} stdout={ran.stdout!r} stderr={ran.stderr!r}"
        )
    return EXPECTED_ORACLE


def custom_target(
    output: Path,
    *,
    profile: str = "portable",
    runtime: str | None = None,
    reverse: bool = False,
    report: bool = True,
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(POSITIVE),
        "-lib",
        "reflaxe.c",
        "-main",
        "StaticInitializationFixture",
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    elif profile != "portable":
        raise StaticInitializationFailure(f"unknown profile {profile!r}")
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_static_initialization_report"])
    command.extend(["-D", "hxc_project_layout=unity", "--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=base_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=45,
    )


def render_production(
    output: Path,
    *,
    profile: str = "portable",
    runtime: str | None = None,
    reverse: bool = False,
) -> ProductionRender:
    result = custom_target(
        output, profile=profile, runtime=runtime, reverse=reverse, report=True
    )
    if result.returncode != 0 or result.stderr:
        raise StaticInitializationFailure(
            f"{profile} production compile failed\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if len(lines) != 1:
        raise StaticInitializationFailure(
            f"production compile emitted an invalid inspection envelope: {result.stdout!r}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report_value = json.loads(payload)
    if not isinstance(report_value, dict):
        raise StaticInitializationFailure("static-initialization inspection is not an object")
    return ProductionRender(output, payload, report_value)


def required_object(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise StaticInitializationFailure(f"{label} must be an object")
    return value


def required_text(value: object, label: str) -> str:
    if not isinstance(value, str):
        raise StaticInitializationFailure(f"{label} must be text")
    return value


def validate_plan(plan: dict[str, object]) -> None:
    if (
        plan.get("schemaVersion") != 1
        or plan.get("strategy") != "eager-haxe-type-order"
        or plan.get("phaseOrder")
        != ["class-initializers", "static-field-initializers", "entry"]
        or plan.get("typeOrder") != EXPECTED_TYPE_ORDER
        or plan.get("executionOrder") != EXPECTED_EXECUTION_ORDER
        or plan.get("runtimeFeatures") != []
        or plan.get("cyclePolicy")
        != "reject-cross-type-cycles-with-canonical-path"
    ):
        raise StaticInitializationFailure("static-initialization plan contract drifted")
    initializers = plan.get("initializers")
    if not isinstance(initializers, list) or len(initializers) != 10:
        raise StaticInitializationFailure("plan must contain three class and seven field initializers")
    ids: list[str] = []
    for initializer in initializers:
        record = required_object(initializer, "initializer record")
        identifier = required_text(record.get("id"), "initializer ID")
        if record.get("functionId") != identifier:
            raise StaticInitializationFailure(f"initializer {identifier} lost its HxcIR function")
        source = required_object(record.get("source"), f"initializer {identifier} source")
        file = required_text(source.get("file"), f"initializer {identifier} source file")
        if file.startswith("/") or "\\" in file or str(ROOT) in file:
            raise StaticInitializationFailure(f"initializer {identifier} leaked a host path")
        ids.append(identifier)
    if ids != EXPECTED_EXECUTION_ORDER or len(ids) != len(set(ids)):
        raise StaticInitializationFailure("initializer records are not unique execution order")

    dependencies = plan.get("dependencies")
    if not isinstance(dependencies, list):
        raise StaticInitializationFailure("plan dependencies must be an array")
    pairs = {
        (
            required_text(required_object(item, "dependency").get("dependentTypeId"), "dependent type"),
            required_text(required_object(item, "dependency").get("prerequisiteTypeId"), "prerequisite type"),
            required_text(required_object(item, "dependency").get("reason"), "dependency reason"),
        )
        for item in dependencies
    }
    expected_pairs = {
        ("AStaticInitDependent", "MStaticInitTrace", "static-field-reference"),
        ("AStaticInitDependent", "ZStaticInitPrerequisite", "static-field-reference"),
        ("ZStaticInitPrerequisite", "MStaticInitTrace", "static-field-reference"),
    }
    if pairs != expected_pairs:
        raise StaticInitializationFailure(f"dependency reasons drifted: {sorted(pairs)!r}")


def initializer_c_names(
    plan: dict[str, object], symbols: dict[str, object]
) -> list[str]:
    entries = symbols.get("symbols")
    initializers = plan.get("initializers")
    if not isinstance(entries, list) or not isinstance(initializers, list):
        raise StaticInitializationFailure("symbol or initializer records are missing")
    result: list[str] = []
    for item in initializers:
        record = required_object(item, "initializer")
        type_id = required_text(record.get("typeId"), "initializer type")
        kind = required_text(record.get("kind"), "initializer kind")
        if kind == "class-initializer":
            source_symbol = f"compiler.static-initialization.{type_id}.class-init#0"
        elif kind == "static-field-initializer":
            field = required_text(record.get("fieldName"), "initializer field")
            ordinal = record.get("sourceOrder")
            if not isinstance(ordinal, int):
                raise StaticInitializationFailure("field initializer source order is not an integer")
            source_symbol = (
                f"compiler.static-initialization.{type_id}.static-field-{field}#{ordinal + 1}"
            )
        else:
            raise StaticInitializationFailure(f"unknown initializer kind {kind!r}")
        matches = [
            entry.get("cName")
            for entry in entries
            if isinstance(entry, dict) and entry.get("sourceSymbol") == source_symbol
        ]
        if len(matches) != 1 or not isinstance(matches[0], str):
            raise StaticInitializationFailure(f"cannot resolve C symbol for {source_symbol}")
        result.append(matches[0])
    return result


def initialization_wrapper_c_name(symbols: dict[str, object]) -> str:
    entries = symbols.get("symbols")
    if not isinstance(entries, list):
        raise StaticInitializationFailure("symbol table entries are missing")
    matches = [
        entry.get("cName")
        for entry in entries
        if isinstance(entry, dict)
        and isinstance(entry.get("sourceSymbol"), str)
        and entry["sourceSymbol"].startswith(
            "compiler.static-initialization.hosted-executable."
        )
    ]
    if len(matches) != 1 or not isinstance(matches[0], str):
        raise StaticInitializationFailure(
            "cannot resolve the compiler-owned initialization wrapper"
        )
    return matches[0]


def source_symbol_c_name(symbols: dict[str, object], source_symbol: str) -> str:
    entries = symbols.get("symbols")
    if not isinstance(entries, list):
        raise StaticInitializationFailure("symbol table entries are missing")
    matches = [
        entry.get("cName")
        for entry in entries
        if isinstance(entry, dict) and entry.get("sourceSymbol") == source_symbol
    ]
    if len(matches) != 1 or not isinstance(matches[0], str):
        raise StaticInitializationFailure(
            f"cannot resolve one C symbol for {source_symbol}"
        )
    return matches[0]


def native_symbol_projection(symbols: dict[str, object]) -> dict[str, object]:
    return {
        "schemaVersion": 1,
        "algorithm": "static-initialization-native-symbol-projection-v1",
        "symbols": [
            {
                "sourceSymbol": source_symbol,
                "cName": source_symbol_c_name(symbols, source_symbol),
            }
            for source_symbol in NATIVE_SOURCE_SYMBOLS
        ],
    }


def validate_native_symbol_projection(projection: dict[str, object]) -> None:
    entries = projection.get("symbols")
    if (
        projection.get("schemaVersion") != 1
        or projection.get("algorithm")
        != "static-initialization-native-symbol-projection-v1"
        or not isinstance(entries, list)
    ):
        raise StaticInitializationFailure("native symbol projection drifted")
    sources: list[str] = []
    c_names: list[str] = []
    for entry in entries:
        if (
            not isinstance(entry, dict)
            or set(entry) != {"sourceSymbol", "cName"}
            or not isinstance(entry.get("sourceSymbol"), str)
            or not isinstance(entry.get("cName"), str)
        ):
            raise StaticInitializationFailure("native symbol projection is malformed")
        source_symbol = entry["sourceSymbol"]
        c_name = entry["cName"]
        if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", c_name) is None:
            raise StaticInitializationFailure(
                f"native symbol projection contains an invalid C name: {c_name!r}"
            )
        sources.append(source_symbol)
        c_names.append(c_name)
    if sources != list(NATIVE_SOURCE_SYMBOLS) or len(c_names) != len(set(c_names)):
        raise StaticInitializationFailure(
            "native symbol projection does not match the required source symbols"
        )


def validate_render(render: ProductionRender) -> None:
    report = render.report
    if report.get("schemaVersion") != 1:
        raise StaticInitializationFailure("inspection schema drifted")
    plan = required_object(report.get("plan"), "inspection plan")
    validate_plan(plan)
    hxcir = required_text(report.get("hxcir"), "inspection HxcIR")
    if str(ROOT) in hxcir or "\\" in hxcir or "hxrt" in hxcir.lower():
        raise StaticInitializationFailure("HxcIR leaked a host path or runtime selection")
    if hxcir.count("initialization=deferred(") != 7:
        raise StaticInitializationFailure("explicit static fields did not become seven deferred globals")
    if hxcir.count("initialize-global") != 7:
        raise StaticInitializationFailure("deferred field initializer functions do not initialize exactly once")
    for identifier in EXPECTED_EXECUTION_ORDER:
        if f'function "{identifier}"' not in hxcir:
            raise StaticInitializationFailure(f"HxcIR lost initializer function {identifier}")

    sidecar = json.loads(
        (render.output / "hxc.initialization-plan.json").read_text(encoding="utf-8")
    )
    if sidecar != plan:
        raise StaticInitializationFailure("project sidecar differs from production HxcIR inspection")
    runtime_plan = json.loads(
        (render.output / "hxc.runtime-plan.json").read_text(encoding="utf-8")
    )
    if (
        runtime_plan.get("status") != "analyzed-runtime-free"
        or runtime_plan.get("features") != []
        or "compiler-planned-eager-static-initialization"
        not in runtime_plan.get("directDecisions", [])
        or not runtime_plan.get("noRuntimeProof")
    ):
        raise StaticInitializationFailure("static initialization selected a runtime feature")
    symbols = json.loads((render.output / "hxc.symbols.json").read_text(encoding="utf-8"))
    symbol_table = required_object(symbols, "symbol table")
    header = (render.output / "include/hxc/program.h").read_text(encoding="utf-8")
    source = (render.output / "src/program.c").read_text(encoding="utf-8")
    expected_calls = initializer_c_names(plan, symbol_table)
    for initializer_name in expected_calls:
        if not re.search(
            rf"^void {re.escape(initializer_name)}\(void\)$",
            source,
            re.MULTILINE,
        ):
            raise StaticInitializationFailure(
                f"initializer {initializer_name} is not a project-private external C function"
            )
        if not re.search(rf"^void {re.escape(initializer_name)}\(void\);$", header, re.MULTILINE):
            raise StaticInitializationFailure(
                f"project-private initializer {initializer_name} lacks its private prototype"
            )
    wrapper_name = initialization_wrapper_c_name(symbol_table)
    wrapper = re.search(
        rf"static void {re.escape(wrapper_name)}\(void\)\n\{{\n"
        rf"(?P<body>(?:  [A-Za-z_][A-Za-z0-9_]*\(\);\n)*)\}}",
        source,
    )
    if wrapper is None:
        raise StaticInitializationFailure("generated C must contain one compiler-owned initialization wrapper")
    actual_calls = re.findall(
        r"  ([A-Za-z_][A-Za-z0-9_]*)\(\);", wrapper.group("body")
    )
    if actual_calls != expected_calls or len(actual_calls) != len(set(actual_calls)):
        raise StaticInitializationFailure("generated C initializer calls differ from the typed plan")
    main = re.search(r"int main\(void\)\n\{\n(?P<body>.*?)\n\}", source, re.DOTALL)
    if main is None or main.group("body").count(f"{wrapper_name}();") != 1:
        raise StaticInitializationFailure("hosted main does not invoke initialization exactly once")
    entry_name = source_symbol_c_name(
        symbol_table, "StaticInitializationFixture.main"
    )
    if main.group("body").find(f"{wrapper_name}();") > main.group("body").find(
        f"{entry_name}();"
    ):
        raise StaticInitializationFailure("hosted main invokes the Haxe entry before initialization")
    entries = symbol_table.get("symbols")
    if not isinstance(entries, list):
        raise StaticInitializationFailure("symbol table entries are missing")
    field_names = [
        entry.get("cName")
        for entry in entries
        if isinstance(entry, dict)
        and entry.get("kind") == "field"
        and isinstance(entry.get("cName"), str)
    ]
    if any(
        re.search(rf"^int32_t {re.escape(name)} = ", source, re.MULTILINE)
        for name in field_names
    ):
        raise StaticInitializationFailure("generated C relied on file-scope Haxe field initialization")
    all_bytes = b"\n".join(path.read_bytes() for path in sorted(render.output.rglob("*")) if path.is_file())
    if b"hxrt" in all_bytes.lower():
        raise StaticInitializationFailure("static-initialization project contains hxrt")


def generated_tree(root: Path) -> dict[str, bytes]:
    return {
        path.relative_to(root).as_posix(): path.read_bytes()
        for path in sorted(root.rglob("*"))
        if path.is_file()
    }


def cycle_compile(output: Path, *, reverse: bool = False) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(CYCLE),
        "-lib",
        "reflaxe.c",
        "-main",
        "StaticInitializationCycleFixture",
    ]
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    command.extend(["--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=base_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=45,
    )


def cycle_diagnostic(result: subprocess.CompletedProcess[str]) -> str:
    combined = result.stdout + result.stderr
    matches = [line.strip() for line in combined.splitlines() if "HXC1002:" in line]
    if result.returncode == 0 or len(matches) != 1:
        raise StaticInitializationFailure(
            f"cycle compile missed exact HXC1002\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    diagnostic = matches[0]
    for marker in (
        "StaticInitializationCycleA --[static-field-reference",
        "StaticInitializationCycleB --[static-field-reference",
        "--> StaticInitializationCycleA",
        "[profile=portable]",
        "Remediation:",
    ):
        if marker not in diagnostic:
            raise StaticInitializationFailure(f"cycle diagnostic lost {marker!r}")
    return diagnostic


def compiler_identity(executable: str) -> tuple[str, str]:
    result = subprocess.run(
        [executable, "--version"], capture_output=True, text=True, timeout=10
    )
    combined = (result.stdout + result.stderr).strip()
    first = combined.splitlines()[0] if combined else "unknown version"
    lowered = combined.lower()
    if "clang" in lowered:
        return "clang", first
    if "free software foundation" in lowered or "gcc" in lowered:
        return "gcc", first
    return "unknown", first


def available_compilers(selected: str | None = None) -> list[NativeToolchain]:
    families = [selected] if selected else ["gcc", "clang"]
    result: list[NativeToolchain] = []
    for family in families:
        executable = shutil.which(family)
        if executable is None:
            if selected:
                raise StaticInitializationFailure(f"required native toolchain missing: {family}")
            print(f"static-initialization: SKIP optional {family}: missing {family}")
            continue
        actual, version = compiler_identity(executable)
        if actual != family:
            if selected:
                raise StaticInitializationFailure(
                    f"required {family} command has {actual} identity"
                )
            print(f"static-initialization: SKIP optional {family}: identity is {actual}")
            continue
        print(
            f"static-initialization: {'REQUIRED' if selected else 'AVAILABLE'} "
            f"{family}: cc={version}"
        )
        result.append(NativeToolchain(family, executable, version))
    if not result:
        raise StaticInitializationFailure("no strict C11 compiler is available")
    return result


def compile_and_run(
    root: Path,
    symbols: dict[str, object],
    toolchain: NativeToolchain,
    optimization: str,
) -> None:
    sources = sorted((root / "src").glob("*.c"))
    header_text = (root / "include/hxc/program.h").read_text(encoding="utf-8")
    validate_native_symbol_projection(symbols)
    outcome_name = source_symbol_c_name(symbols, NATIVE_OUTCOME_SOURCE_SYMBOL)
    if re.search(
        rf"^int32_t {re.escape(outcome_name)}\(void\);$",
        header_text,
        re.MULTILINE,
    ) is None:
        raise StaticInitializationFailure("cannot resolve generated outcome function")
    renamed_header = root / "renamed-entry.h"
    renamed_header.write_text(
        "int hxc_generated_main(void);\n", encoding="utf-8", newline="\n"
    )
    harness = root / "static-initialization-harness.c"
    harness.write_text(
        '#include "hxc/program.h"\n\n'
        "int hxc_generated_main(void);\n\n"
        "int main(void)\n"
        "{\n"
        "  if (hxc_generated_main() != 0)\n"
        "  {\n"
        "    return 2;\n"
        "  }\n"
        f"  return {outcome_name}() == INT32_C(446) ? 0 : 1;\n"
        "}\n",
        encoding="utf-8",
        newline="\n",
    )
    objects: list[Path] = []
    for index, source in enumerate(sources):
        target = root / f"source-{index}-{toolchain.family}-{optimization[1:]}.o"
        compiled = subprocess.run(
            [
                toolchain.compiler,
                *STRICT_FLAGS,
                optimization,
                "-Dmain=hxc_generated_main",
                "-include",
                str(renamed_header),
                "-I",
                str(root / "include"),
                "-c",
                str(source),
                "-o",
                str(target),
            ],
            capture_output=True,
            text=True,
            check=False,
            timeout=30,
        )
        if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
            raise StaticInitializationFailure(
                f"{toolchain.family} {optimization} rejected generated C\n"
                f"stdout:\n{compiled.stdout}\nstderr:\n{compiled.stderr}"
            )
        objects.append(target)
    harness_object = root / f"harness-{toolchain.family}-{optimization[1:]}.o"
    compiled_harness = subprocess.run(
        [
            toolchain.compiler,
            *STRICT_FLAGS,
            optimization,
            "-I",
            str(root / "include"),
            "-c",
            str(harness),
            "-o",
            str(harness_object),
        ],
        capture_output=True,
        text=True,
        check=False,
        timeout=30,
    )
    if compiled_harness.returncode != 0 or compiled_harness.stdout or compiled_harness.stderr:
        raise StaticInitializationFailure(
            f"{toolchain.family} rejected the independent initialization harness\n"
            f"{compiled_harness.stdout}{compiled_harness.stderr}"
        )
    executable = root / f"static-init-{toolchain.family}-{optimization[1:]}"
    command = [
        toolchain.compiler,
        *[str(item) for item in objects],
        str(harness_object),
        "-o",
        str(executable),
    ]
    runtime_plan = json.loads((root / "hxc.runtime-plan.json").read_text())
    if "m" in runtime_plan.get("libraries", []):
        command.append("-lm")
    compiled = subprocess.run(
        command, capture_output=True, text=True, check=False, timeout=30
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise StaticInitializationFailure(
            f"{toolchain.family} {optimization} rejected generated C\n"
            f"stdout:\n{compiled.stdout}\nstderr:\n{compiled.stderr}"
        )
    ran = subprocess.run(
        [str(executable)], capture_output=True, text=True, check=False, timeout=10
    )
    if ran.returncode != 0 or ran.stdout or ran.stderr:
        raise StaticInitializationFailure(
            f"{toolchain.family} {optimization} differed from the Haxe oracle: "
            f"exit={ran.returncode} stdout={ran.stdout!r} stderr={ran.stderr!r}"
        )


def snapshot_values() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-static-initialization-snapshot-") as temporary:
        render = render_production(Path(temporary) / "output")
        validate_render(render)
        symbols = required_object(
            json.loads(
                (render.output / "hxc.symbols.json").read_text(encoding="utf-8")
            ),
            "snapshot symbol table",
        )
        return {
            "initialization-plan.json": required_object(
                render.report.get("plan"), "snapshot plan"
            ),
            "initialization.hxcir": required_text(
                render.report.get("hxcir"), "snapshot HxcIR"
            ),
            "program.h": (render.output / "include/hxc/program.h").read_text(
                encoding="utf-8"
            ),
            "program.c": (render.output / "src/program.c").read_text(
                encoding="utf-8"
            ),
            "native-symbols.json": native_symbol_projection(symbols),
        }


def difference(expected: str, actual: str, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def check_snapshots(values: dict[str, object]) -> None:
    for name, actual in values.items():
        path = EXPECTED / name
        if name.endswith(".json"):
            if actual != json.loads(path.read_text(encoding="utf-8")):
                raise StaticInitializationFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise StaticInitializationFailure(f"{name} snapshot must be text")
            expected = path.read_text(encoding="utf-8")
            if actual != expected:
                raise StaticInitializationFailure(
                    f"{name} drifted:\n" + difference(expected, actual, name)
                )


def write_snapshot_project(root: Path) -> dict[str, object]:
    header = root / "include/hxc/program.h"
    header.parent.mkdir(parents=True, exist_ok=True)
    header.write_text((EXPECTED / "program.h").read_text(), encoding="utf-8", newline="\n")
    source = root / "src/program.c"
    source.parent.mkdir(parents=True, exist_ok=True)
    source.write_text((EXPECTED / "program.c").read_text(), encoding="utf-8", newline="\n")
    (root / "hxc.runtime-plan.json").write_text(
        json.dumps({"libraries": []}), encoding="utf-8", newline="\n"
    )
    projection = required_object(
        json.loads((EXPECTED / "native-symbols.json").read_text(encoding="utf-8")),
        "checked-in native symbol projection",
    )
    validate_native_symbol_projection(projection)
    return projection


def check_full(selected: str | None = None) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-static-initialization-") as temporary:
        root = Path(temporary)
        run_oracle(root)
        portable = render_production(root / "portable")
        repeated = render_production(root / "repeated")
        reversed_render = render_production(root / "reversed", reverse=True)
        metal = render_production(root / "metal", profile="metal")
        runtime_none = render_production(root / "runtime-none", runtime="none")
        for render in (portable, repeated, reversed_render, metal, runtime_none):
            validate_render(render)
        if portable.report_payload != repeated.report_payload:
            raise StaticInitializationFailure("repeated initialization reports differ")
        if portable.report_payload != reversed_render.report_payload:
            raise StaticInitializationFailure("typed-module discovery order changed the initialization report")
        if generated_tree(portable.output) != generated_tree(repeated.output):
            raise StaticInitializationFailure("repeated production roots are not byte-identical")
        if generated_tree(portable.output) != generated_tree(reversed_render.output):
            raise StaticInitializationFailure("typed-module discovery order changed production artifacts")
        for relative in (
            "src/program.c",
            "include/hxc/program.h",
            "hxc.initialization-plan.json",
        ):
            if (portable.output / relative).read_bytes() != (metal.output / relative).read_bytes():
                raise StaticInitializationFailure(f"portable and metal diverged at {relative}")
        none_plan = json.loads(
            (runtime_none.output / "hxc.runtime-plan.json").read_text(encoding="utf-8")
        )
        if none_plan.get("resolvedPolicy") != "none" or none_plan.get("features") != []:
            raise StaticInitializationFailure("explicit runtime-none proof drifted")
        first_cycle = cycle_compile(root / "cycle-first")
        reverse_cycle = cycle_compile(root / "cycle-reverse", reverse=True)
        if cycle_diagnostic(first_cycle) != cycle_diagnostic(reverse_cycle):
            raise StaticInitializationFailure("cycle diagnostic path changed with typed-module order")
        for output in (root / "cycle-first", root / "cycle-reverse"):
            if output.exists() and any(output.rglob("*")):
                raise StaticInitializationFailure("HXC1002 cycle failure emitted an artifact")
        values = {
            "initialization-plan.json": portable.report["plan"],
            "initialization.hxcir": portable.report["hxcir"],
            "program.h": (portable.output / "include/hxc/program.h").read_text(
                encoding="utf-8"
            ),
            "program.c": (portable.output / "src/program.c").read_text(
                encoding="utf-8"
            ),
            "native-symbols.json": native_symbol_projection(
                required_object(
                    json.loads(
                        (portable.output / "hxc.symbols.json").read_text(
                            encoding="utf-8"
                        )
                    ),
                    "production symbol table",
                )
            ),
        }
        check_snapshots(values)
        native_symbols = required_object(
            values["native-symbols.json"], "native symbol projection"
        )
        for toolchain in available_compilers(selected):
            for optimization in ("-O0", "-O2"):
                compile_and_run(
                    portable.output, native_symbols, toolchain, optimization
                )


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("gcc", "clang"))
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    try:
        if args.native_only:
            with tempfile.TemporaryDirectory(
                prefix="hxc-static-initialization-native-"
            ) as temporary:
                root = Path(temporary)
                native_symbols = write_snapshot_project(root)
                for toolchain in available_compilers(args.toolchain):
                    for optimization in ("-O0", "-O2"):
                        compile_and_run(root, native_symbols, toolchain, optimization)
            print(
                "static-initialization: OK: required strict-C initialization native matrix passed"
            )
            return 0
        else:
            check_full(args.toolchain)
    except (
        StaticInitializationFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"static-initialization: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "static-initialization: OK: typed dependency reasons, class/field phases, "
        "exact-once HxcIR/C, HXC1002 cycles, JS differential, and zero-runtime strict C passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

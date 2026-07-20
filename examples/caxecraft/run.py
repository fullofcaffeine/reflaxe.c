#!/usr/bin/env python3
"""Prove the deterministic, runtime-free Caxecraft domain under Eval and C."""

from __future__ import annotations

import argparse
import difflib
import json
import os
import re
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from collections import Counter
from collections.abc import Iterable
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
sys.path.insert(0, str(CASE))
from check_assets import (  # noqa: E402
    AssetValidationError,
    negative_contracts,
    validate_asset_pack,
)

BUILD_HXML = CASE / "build.hxml"
ORACLE_HXML = CASE / "oracle.hxml"
EXPECTED = CASE / "expected"
NATIVE = CASE / "test/native"
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
COMMON_PRODUCTION_FILES = {
    "_GeneratedFiles.json",
    "cmake/CMakeLists.txt",
    "hxc.abi.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
    "meson.build",
}
SPLIT_HEADERS = (
    "include/hxc/detail/program_types.h",
    "include/hxc/modules/caxecraft/domain/AxisMove.h",
    "include/hxc/modules/caxecraft/domain/BlockCoord.h",
    "include/hxc/modules/caxecraft/domain/BlockKind.h",
    "include/hxc/modules/caxecraft/domain/CaxecraftTrace.h",
    "include/hxc/modules/caxecraft/domain/PlayerPhysics.h",
    "include/hxc/modules/caxecraft/domain/PlayerState.h",
    "include/hxc/modules/caxecraft/domain/RaycastHit.h",
    "include/hxc/modules/caxecraft/domain/StepInput.h",
    "include/hxc/modules/caxecraft/domain/VoxelRaycast.h",
    "include/hxc/modules/caxecraft/domain/World.h",
    "include/hxc/modules/caxecraft/domain/WorldStorage.h",
    "include/hxc/modules/caxecraft/qa/DomainProbe.h",
    "include/hxc/program.h",
)
SPLIT_SOURCES = (
    "src/hxc/main.c",
    "src/hxc/support.c",
    "src/modules/caxecraft/domain/CaxecraftTrace.c",
    "src/modules/caxecraft/domain/PlayerPhysics.c",
    "src/modules/caxecraft/domain/VoxelRaycast.c",
    "src/modules/caxecraft/domain/World.c",
    "src/modules/caxecraft/domain/WorldStorage.c",
    "src/modules/caxecraft/qa/DomainProbe.c",
)
PRODUCTION_FILES = {
    "split": COMMON_PRODUCTION_FILES | set(SPLIT_HEADERS) | set(SPLIT_SOURCES),
    "unity": COMMON_PRODUCTION_FILES
    | {"include/hxc/program.h", "src/program.c"},
}
SNAPSHOT_FORMATS = {
    **{path: "header" for path in SPLIT_HEADERS},
    **{path: "c" for path in SPLIT_SOURCES},
    "hxc.runtime-plan.json": "json",
    "method-symbols.json": "json",
    "readability-metrics.json": "json",
    "oracle.txt": "text",
}
LEGACY_TEMPORARY_IDENTIFIER_BASELINE = 971
MAX_CURRENT_DOMAIN_TEMPORARY_IDENTIFIERS = 400
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
SANITIZER_FLAGS = (
    "-fsanitize=address,undefined",
    "-fno-sanitize-recover=all",
    "-fno-omit-frame-pointer",
)
COVERAGE = frozenset(
    {
        "aabb-collision",
        "eval-differential",
        "seeded-property-corpus",
        "voxel-dda",
        "zero-runtime",
    }
)

sys.path.insert(0, str(ROOT / "scripts/test"))
from c_fixture_harness import (  # noqa: E402
    CFixtureFailure,
    CFixtureProject,
    resolve_toolchains,
    run_c_fixture_corpus,
    validate_report,
)


class CaxecraftFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class RenderedProject:
    output: Path
    artifacts: dict[str, bytes]
    hxcir: str
    runtime_plan: dict[str, object]
    method_symbols: dict[str, object]
    readability_metrics: dict[str, object]


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(locale: str, *, server: bool) -> dict[str, str]:
    environment = os.environ.copy()
    environment["LC_ALL"] = locale
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


def compile_target(
    output: Path,
    *,
    layout: str = "split",
    reverse: bool = False,
    locale: str = "C",
    connect: str | None = None,
    report: bool = False,
) -> subprocess.CompletedProcess[str]:
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
            "--cwd",
            str(CASE),
            BUILD_HXML.name,
            "-D",
            "hxc_runtime_diagnostics=off",
        ]
    )
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_static_initialization_report"])
    if layout == "unity":
        command.extend(["-D", "hxc_project_layout=unity"])
    elif layout != "split":
        raise CaxecraftFailure(f"unknown Caxecraft project layout {layout!r}")
    command.extend(["--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(locale, server=connect is not None),
        check=False,
        capture_output=True,
        text=True,
        timeout=90,
    )


def load_json(path: Path, label: str) -> dict[str, object]:
    try:
        value: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise CaxecraftFailure(f"cannot read {label}: {error}") from error
    if not isinstance(value, dict):
        raise CaxecraftFailure(f"{label} must contain a JSON object")
    return value


def extract_hxcir(result: subprocess.CompletedProcess[str], label: str) -> str:
    lines = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(lines) != 1:
        raise CaxecraftFailure(f"{label} emitted an invalid HxcIR report envelope")
    report = json.loads(lines[0])
    if not isinstance(report, dict) or report.get("schemaVersion") != 1:
        raise CaxecraftFailure(f"{label} emitted an invalid HxcIR report")
    hxcir = report.get("hxcir")
    if not isinstance(hxcir, str):
        raise CaxecraftFailure(f"{label} omitted its HxcIR dump")
    return hxcir


def normal_artifacts(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"), key=lambda item: item.as_posix().encode("utf-8"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def generated_files(output: Path) -> set[str]:
    return {
        path.relative_to(output).as_posix()
        for path in output.rglob("*")
        if path.is_file()
    }


def method_symbol_projection(symbols: dict[str, object]) -> dict[str, object]:
    entries = symbols.get("symbols")
    if not isinstance(entries, list):
        raise CaxecraftFailure("generated symbol table omitted its symbols array")
    methods: list[dict[str, str]] = []
    for entry in entries:
        if not isinstance(entry, dict):
            raise CaxecraftFailure("generated symbol table contains a malformed entry")
        source = entry.get("sourceSymbol")
        c_name = entry.get("cName")
        if (
            entry.get("kind") == "method"
            and isinstance(source, str)
            and source.startswith("caxecraft.")
        ):
            if not isinstance(c_name, str):
                raise CaxecraftFailure(f"method symbol {source!r} has no C name")
            methods.append({"sourceSymbol": source, "cName": c_name})
    methods.sort(key=lambda entry: entry["sourceSymbol"].encode("utf-8"))
    return {
        "schemaVersion": 1,
        "algorithm": "caxecraft-method-symbol-projection-v1",
        "methods": methods,
    }


def text_list(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise CaxecraftFailure(f"{label} must be a string array")
    return list(value)


def validate_runtime_plan(plan: dict[str, object]) -> None:
    proof = plan.get("noRuntimeProof")
    if (
        plan.get("schemaVersion") != 2
        or plan.get("algorithm") != "hxc-runtime-plan-v2"
        or plan.get("status") != "analyzed-runtime-free"
        or plan.get("profile") != "portable"
        or plan.get("environment") != "hosted"
        or plan.get("requestedPolicy") != "none"
        or plan.get("resolvedPolicy") != "none"
        or plan.get("policyProvenance") != "direct-define:hxc_runtime"
        or not isinstance(proof, dict)
        or proof.get("status") != "eligible"
        or proof.get("scope") != "reachable-whole-program"
    ):
        raise CaxecraftFailure("Caxecraft runtime-free policy/proof drifted")
    for key in (
        "selectedFeatures",
        "features",
        "artifacts",
        "artifactDetails",
        "libraries",
        "defines",
        "rootReasons",
        "dependencyEdges",
        "manualOverrides",
        "symbols",
    ):
        if plan.get(key) != []:
            raise CaxecraftFailure(f"Caxecraft unexpectedly populated runtime-plan {key}")
    absence = proof.get("runtimeAbsence")
    if not isinstance(absence, dict) or any(
        absence.get(key) != []
        for key in ("sources", "features", "symbols", "libraries", "includes", "defines")
    ):
        raise CaxecraftFailure("Caxecraft no-runtime proof retained a runtime effect")
    decisions = text_list(plan.get("directDecisions"), "runtime direct decisions")
    for expected in (
        "bounded-haxe-enum-values",
        "closed-anonymous-value-records",
        "primitive-static-storage",
        "selected-program-local-helpers",
        "ub-safe-primitive-operations",
    ):
        if expected not in decisions:
            raise CaxecraftFailure(f"Caxecraft runtime proof omitted {expected!r}")


def validate_method_symbols(projection: dict[str, object]) -> None:
    methods = projection.get("methods")
    if (
        projection.get("schemaVersion") != 1
        or projection.get("algorithm") != "caxecraft-method-symbol-projection-v1"
        or not isinstance(methods, list)
    ):
        raise CaxecraftFailure("Caxecraft method-symbol projection drifted")
    pairs: list[tuple[str, str]] = []
    for entry in methods:
        if (
            not isinstance(entry, dict)
            or not isinstance(entry.get("sourceSymbol"), str)
            or not isinstance(entry.get("cName"), str)
        ):
            raise CaxecraftFailure("Caxecraft method-symbol projection is malformed")
        pairs.append((entry["sourceSymbol"], entry["cName"]))
    if pairs != sorted(pairs, key=lambda pair: pair[0].encode("utf-8")):
        raise CaxecraftFailure("Caxecraft method-symbol projection is not UTF-8 sorted")
    sources = {source for source, _ in pairs}
    required = {
        "caxecraft.domain.CaxecraftTrace.propertyTrace(i32)",
        "caxecraft.domain.CaxecraftTrace.runTrace",
        "caxecraft.domain.VoxelRaycast.trace(span:mutable<u8>, f64, f64, f64, f64, f64, f64, f64)",
        "caxecraft.domain.World.generate(span:mutable<u8>, i32)",
        "caxecraft.qa.DomainProbe.selfCheck",
    }
    missing = sorted(required - sources)
    if missing:
        raise CaxecraftFailure(f"Caxecraft method symbols omitted {missing!r}")
    if not any(source.startswith("caxecraft.domain.PlayerPhysics.step(") for source in sources):
        raise CaxecraftFailure("Caxecraft method symbols omitted PlayerPhysics.step")
    if any("hxrt" in value.lower() for pair in pairs for value in pair):
        raise CaxecraftFailure("Caxecraft method symbols unexpectedly mention hxrt")


def projected_method_name(
    projection: dict[str, object], source_symbol: str, *, prefix: bool = False
) -> str:
    methods = projection.get("methods")
    if not isinstance(methods, list):
        raise CaxecraftFailure("Caxecraft method-symbol projection is malformed")
    matches = [
        item.get("cName")
        for item in methods
        if isinstance(item, dict)
        and isinstance(item.get("sourceSymbol"), str)
        and (
            item["sourceSymbol"].startswith(source_symbol)
            if prefix
            else item["sourceSymbol"] == source_symbol
        )
        and isinstance(item.get("cName"), str)
    ]
    if len(matches) != 1:
        raise CaxecraftFailure(
            f"cannot resolve one generated C method for {source_symbol!r}: {matches!r}"
        )
    return matches[0]


def native_method_symbol_header(projection: dict[str, object]) -> str:
    aliases = (
        ("CAXECRAFT_SELF_CHECK", "caxecraft.qa.DomainProbe.selfCheck"),
        (
            "CAXECRAFT_TERRAIN_TRACE",
            "caxecraft.domain.CaxecraftTrace.terrainTrace",
        ),
        ("CAXECRAFT_EDIT_TRACE", "caxecraft.domain.CaxecraftTrace.editTrace"),
        ("CAXECRAFT_RAY_TRACE", "caxecraft.domain.CaxecraftTrace.rayTrace"),
        (
            "CAXECRAFT_COLLISION_TRACE",
            "caxecraft.domain.CaxecraftTrace.collisionTrace",
        ),
        ("CAXECRAFT_RUN_TRACE", "caxecraft.domain.CaxecraftTrace.runTrace"),
        (
            "CAXECRAFT_PROPERTY_TRACE",
            "caxecraft.domain.CaxecraftTrace.propertyTrace(i32)",
        ),
    )
    definitions = []
    for alias, source_symbol in aliases:
        c_name = projected_method_name(projection, source_symbol)
        if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", c_name) is None:
            raise CaxecraftFailure(
                f"native method symbol is not a C identifier: {c_name!r}"
            )
        definitions.append(f"#define {alias} {c_name}")
    return "\n".join(
        (
            "#ifndef CAXECRAFT_TEST_METHOD_SYMBOLS_H_INCLUDED",
            "#define CAXECRAFT_TEST_METHOD_SYMBOLS_H_INCLUDED",
            "",
            "/* Test-only aliases projected from the compiler symbol report. */",
            *definitions,
            "",
            "#endif /* CAXECRAFT_TEST_METHOD_SYMBOLS_H_INCLUDED */",
            "",
        )
    )


def validate_hxcir(hxcir: str) -> None:
    for marker in (
        "hxcir schema=10",
        'function "function.caxecraft.domain.World.generate"',
        'function "function.caxecraft.domain.VoxelRaycast.trace"',
        'function "function.caxecraft.domain.PlayerPhysics.step"',
        "initialize-fixed-array",
        "initialize-span",
        "span-parameter-borrow",
    ):
        if marker not in hxcir:
            raise CaxecraftFailure(f"Caxecraft HxcIR omitted {marker!r}")
    for forbidden in ("runtime(feature=", "allocate", "cleanup action"):
        if forbidden in hxcir:
            raise CaxecraftFailure(f"Caxecraft HxcIR unexpectedly contains {forbidden!r}")
    if str(ROOT) in hxcir or "\\" in hxcir:
        raise CaxecraftFailure("Caxecraft HxcIR leaked a host path")


def validate_generated_text(
    header: bytes, source: bytes, method_symbols: dict[str, object]
) -> None:
    combined = header + b"\n" + source
    if str(ROOT).encode() in combined or b"\\" in combined or b"hxrt" in combined.lower():
        raise CaxecraftFailure("generated Caxecraft C leaked a host path or hxrt")
    text = combined.decode("utf-8")
    forbidden = re.compile(r"(?:\bhxrt_[A-Za-z0-9_]*\b|\b(?:malloc|calloc|realloc|free)\s*\()")
    match = forbidden.search(text)
    if match is not None:
        raise CaxecraftFailure(f"generated Caxecraft C selected {match.group(0)!r}")
    # Match compiler statements, not story/localization text that happens to say "goto".
    goto_match = re.search(r"(?m)^\s*goto\s+[A-Za-z_][A-Za-z0-9_]*\s*;$", text)
    compiler_label = re.search(r"(?m)^\s*hxc_[A-Za-z0-9_]+:\s*$", text)
    if goto_match is not None or compiler_label is not None:
        marker = goto_match.group(0) if goto_match is not None else compiler_label.group(0).strip()
        raise CaxecraftFailure(
            f"generated Caxecraft C retained compiler control-flow marker {marker!r}"
        )
    for marker in (
        "uint8_t hxc_storage[16384]",
        projected_method_name(
            method_symbols,
            "caxecraft.domain.VoxelRaycast.trace(span:mutable<u8>, f64, f64, f64, f64, f64, f64, f64)",
        ),
        projected_method_name(
            method_symbols, "caxecraft.domain.PlayerPhysics.step(", prefix=True
        ),
        projected_method_name(
            method_symbols, "caxecraft.qa.DomainProbe.selfCheck"
        ),
    ):
        if marker not in text:
            raise CaxecraftFailure(f"generated Caxecraft C omitted {marker!r}")


def validate_block_coord_header(content: str) -> None:
    guard = "HXC_CAXECRAFT_DOMAIN_BLOCK_COORD_H_INCLUDED"
    expected_record = (
        "struct hxc_caxecraft_domain_BlockCoord {\n"
        "  int32_t hxc_x;\n"
        "  int32_t hxc_y;\n"
        "  int32_t hxc_z;\n"
        "};"
    )
    if (
        not content.startswith(f"#ifndef {guard}\n#define {guard}\n")
        or expected_record not in content
        or not content.endswith(f"#endif /* {guard} */\n")
    ):
        raise CaxecraftFailure(
            "BlockCoord.h lost its readable guard, source-shaped tag, or hxc_x/y/z members"
        )
    for forbidden in ("HXC_GENERATED_PATH_", "closedzx", "_h4aec2e39", "zx2D"):
        if forbidden in content:
            raise CaxecraftFailure(
                f"BlockCoord.h leaked machine identity {forbidden!r} into ordinary C"
            )


def generated_readability_metrics(header: bytes, source: bytes) -> dict[str, object]:
    text = (header + b"\n" + source).decode("utf-8")
    identifier_references = re.findall(r"\b[A-Za-z_][A-Za-z0-9_]*\b", text)
    identifier_counts = Counter(identifier_references)
    identifiers = set(identifier_counts)
    temporaries = {name for name in identifiers if name.startswith("hxc_tmp_")}
    old_role_names = {
        name
        for name in identifiers
        if re.match(r"hxc_(?:type|field|method|local|temp|spec)_", name)
    }
    byte_escaped_names = {
        name for name in identifiers if re.search(r"zx[0-9A-Fa-f]{2}", name)
    }
    digest_names = {
        name for name in identifiers if re.search(r"[0-9a-f]{32,}", name)
    }
    hashed_names = {
        name for name in identifiers if re.search(r"_h[0-9a-f]{12,64}$", name)
    }
    guards = set(re.findall(r"(?m)^#ifndef ([A-Za-z_][A-Za-z0-9_]*)$", text))
    digest_guards = {
        guard
        for guard in guards
        if re.search(r"_H[0-9A-F]{16,64}_INCLUDED$", guard)
    }
    record_address_temporaries = {
        name
        for name in temporaries
        if name.startswith("hxc_tmp_record_field_address_")
    }
    compiler_labels = set(
        re.findall(r"(?m)^\s*(hxc_[A-Za-z0-9_]+):\s*$", text)
    )
    goto_statements = re.findall(
        r"(?m)^\s*goto\s+[A-Za-z_][A-Za-z0-9_]*\s*;$", text
    )
    maximum_identifier = max(identifiers, key=lambda value: (len(value), value))
    temporary_count = len(temporaries)
    return {
        "schemaVersion": 1,
        "status": "bounded-readable-generated-c",
        "generatedLineCount": len(text.splitlines()),
        "uniqueIdentifierCount": len(identifiers),
        "maximumIdentifierLength": len(maximum_identifier),
        "maximumIdentifier": maximum_identifier,
        "temporaryIdentifierCount": temporary_count,
        "temporaryReferences": sum(identifier_counts[name] for name in temporaries),
        "temporaryIdentifiersRemovedFromLegacyBaseline": (
            LEGACY_TEMPORARY_IDENTIFIER_BASELINE - temporary_count
        ),
        "recordFieldAddressTemporaryCount": len(record_address_temporaries),
        "oldRoleEncodedIdentifierCount": len(old_role_names),
        "oldByteEscapeIdentifierCount": len(byte_escaped_names),
        "semanticDigestIdentifierCount": len(digest_names),
        "hashSuffixedIdentifierCount": len(hashed_names),
        "headerGuardCount": len(guards),
        "hashedHeaderGuardCount": len(digest_guards),
        "gotoStatementCount": len(goto_statements),
        "compilerLabelCount": len(compiler_labels),
    }


def validate_readability_metrics(metrics: dict[str, object]) -> None:
    if (
        metrics.get("schemaVersion") != 1
        or metrics.get("status") != "bounded-readable-generated-c"
        or not isinstance(metrics.get("generatedLineCount"), int)
        or metrics.get("generatedLineCount", 0) <= 0
        or not isinstance(metrics.get("maximumIdentifierLength"), int)
        or metrics.get("maximumIdentifierLength", 121) > 120
        or not isinstance(metrics.get("temporaryIdentifierCount"), int)
        or metrics.get("temporaryIdentifierCount", 526)
        > MAX_CURRENT_DOMAIN_TEMPORARY_IDENTIFIERS
        or metrics.get("temporaryIdentifiersRemovedFromLegacyBaseline", 0) <= 0
        or metrics.get("recordFieldAddressTemporaryCount") != 0
        or metrics.get("oldRoleEncodedIdentifierCount") != 0
        or metrics.get("oldByteEscapeIdentifierCount") != 0
        or metrics.get("semanticDigestIdentifierCount") != 0
        or metrics.get("hashedHeaderGuardCount") != 0
        or metrics.get("gotoStatementCount") != 0
        or metrics.get("compilerLabelCount") != 0
    ):
        raise CaxecraftFailure(
            f"generated-C readability budget drifted: {metrics!r}"
        )


def validate_symbol_readability(symbols: dict[str, object]) -> None:
    entries = symbols.get("symbols")
    collisions = symbols.get("collisions")
    if (
        symbols.get("schemaVersion") != 2
        or symbols.get("algorithm") != "hxc-c-symbol-v2"
        or not isinstance(entries, list)
        or not isinstance(collisions, list)
    ):
        raise CaxecraftFailure("Caxecraft readable symbol-table contract drifted")
    collision_names: set[str] = set()
    for collision in collisions:
        if not isinstance(collision, dict) or not isinstance(
            collision.get("symbols"), list
        ):
            raise CaxecraftFailure(
                "Caxecraft symbol table contains a malformed collision ledger"
            )
        for item in collision["symbols"]:
            if not isinstance(item, dict) or not isinstance(item.get("cName"), str):
                raise CaxecraftFailure(
                    "Caxecraft symbol table contains a malformed collision entry"
                )
            collision_names.add(item["cName"])
    block_coord = []
    for entry in entries:
        if not isinstance(entry, dict) or not isinstance(entry.get("cName"), str):
            raise CaxecraftFailure("Caxecraft symbol table contains a malformed entry")
        c_name = entry["cName"]
        readable = entry.get("readableName")
        reasons = entry.get("escapeReasons")
        if len(c_name) > 120 or re.search(r"zx[0-9A-Fa-f]{2}", c_name):
            raise CaxecraftFailure(
                f"Caxecraft symbol retained an encoded/overlong C name: {c_name!r}"
            )
        if entry.get("collisionResolved") is True and c_name not in collision_names:
            raise CaxecraftFailure(
                f"Caxecraft collision suffix is absent from its ledger: {c_name!r}"
            )
        if re.search(r"_h[0-9a-f]{12,64}$", c_name) and not (
            entry.get("collisionResolved") is True
            or isinstance(reasons, list)
            and "length-limit" in reasons
        ):
            raise CaxecraftFailure(
                f"Caxecraft symbol has an unexplained hash suffix: {c_name!r}"
            )
        if readable == ["caxecraft", "domain", "BlockCoord"]:
            block_coord.append(c_name)
    if block_coord != ["hxc_caxecraft_domain_BlockCoord"]:
        raise CaxecraftFailure(
            f"BlockCoord semantic/display identity drifted: {block_coord!r}"
        )


def generated_c_bytes(output: Path, layout: str) -> tuple[bytes, bytes]:
    if layout == "split":
        headers = b"\n".join((output / path).read_bytes() for path in SPLIT_HEADERS)
        sources = b"\n".join((output / path).read_bytes() for path in SPLIT_SOURCES)
        return headers, sources
    if layout == "unity":
        return (
            (output / "include/hxc/program.h").read_bytes(),
            (output / "src/program.c").read_bytes(),
        )
    raise CaxecraftFailure(f"unknown generated-C layout {layout!r}")


def render_project(
    output: Path,
    *,
    label: str,
    layout: str = "split",
    reverse: bool = False,
    locale: str = "C",
    connect: str | None = None,
    report: bool = False,
) -> RenderedProject:
    result = compile_target(
        output,
        layout=layout,
        reverse=reverse,
        locale=locale,
        connect=connect,
        report=report,
    )
    allowed_stdout = result.stdout if report else ""
    if result.returncode != 0 or result.stderr or (not report and result.stdout):
        raise CaxecraftFailure(
            f"{label} failed or emitted diagnostics\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if report and not any(
        line.startswith(REPORT_PREFIX) for line in allowed_stdout.splitlines()
    ):
        raise CaxecraftFailure(f"{label} omitted its requested HxcIR report")
    actual_files = generated_files(output)
    expected_files = PRODUCTION_FILES.get(layout)
    if expected_files is None or actual_files != expected_files:
        raise CaxecraftFailure(
            f"{label} generated file set drifted: {sorted(actual_files)!r}"
        )
    manifest = load_json(output / "hxc.manifest.json", f"{label} compiler manifest")
    configuration = manifest.get("configuration")
    build = manifest.get("build")
    if (
        not isinstance(configuration, dict)
        or configuration.get("projectLayout") != layout
        or not isinstance(build, dict)
        or build.get("sources")
        != list(SPLIT_SOURCES if layout == "split" else ("src/program.c",))
        or build.get("privateHeaders")
        != list(SPLIT_HEADERS if layout == "split" else ("include/hxc/program.h",))
    ):
        raise CaxecraftFailure(f"{label} layout/build manifest drifted")
    runtime_plan = load_json(output / "hxc.runtime-plan.json", f"{label} runtime plan")
    validate_runtime_plan(runtime_plan)
    stdlib = load_json(output / "hxc.stdlib-report.json", f"{label} stdlib report")
    if (
        stdlib.get("schemaVersion") != 1
        or stdlib.get("status") != "analyzed-no-stdlib-use"
        or stdlib.get("modules") != []
        or stdlib.get("capabilities") != []
    ):
        raise CaxecraftFailure(f"{label} unexpectedly selected a Haxe stdlib surface")
    symbols = load_json(output / "hxc.symbols.json", f"{label} symbol table")
    validate_symbol_readability(symbols)
    projection = method_symbol_projection(symbols)
    validate_method_symbols(projection)
    header, source = generated_c_bytes(output, layout)
    validate_generated_text(header, source, projection)
    readability_metrics = generated_readability_metrics(header, source)
    validate_readability_metrics(readability_metrics)
    if layout == "split":
        validate_block_coord_header(
            (output / "include/hxc/modules/caxecraft/domain/BlockCoord.h").read_text(
                encoding="utf-8"
            )
        )
    hxcir = extract_hxcir(result, label) if report else ""
    if report:
        validate_hxcir(hxcir)
    return RenderedProject(
        output,
        normal_artifacts(output),
        hxcir,
        runtime_plan,
        projection,
        readability_metrics,
    )


def run_oracle() -> bytes:
    result = subprocess.run(
        [
            development_tool("haxe"),
            "--cwd",
            str(CASE),
            ORACLE_HXML.name,
        ],
        cwd=ROOT,
        env=haxe_environment("C", server=False),
        check=False,
        capture_output=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stderr:
        raise CaxecraftFailure(
            "Eval oracle failed or emitted stderr\n"
            f"exit={result.returncode}\nstdout={result.stdout!r}\nstderr={result.stderr!r}"
        )
    lines = result.stdout.splitlines()
    if len(lines) != 38 or lines[0] != b"0" or not result.stdout.endswith(b"\n"):
        raise CaxecraftFailure(
            "Eval oracle must emit a zero self-check, five canonical hashes, and "
            "32 property hashes"
        )
    for line in lines:
        try:
            value = int(line.decode("ascii"))
        except (UnicodeError, ValueError) as error:
            raise CaxecraftFailure(f"Eval oracle emitted a non-integer line: {line!r}") from error
        if value < -(1 << 31) or value > (1 << 31) - 1:
            raise CaxecraftFailure(f"Eval oracle value is outside int32: {value}")
    return result.stdout


def first_difference(left: bytes, right: bytes) -> int:
    for index, (left_byte, right_byte) in enumerate(zip(left, right)):
        if left_byte != right_byte:
            return index
    return min(len(left), len(right))


def assert_artifacts_equal(
    left: dict[str, bytes], right: dict[str, bytes], label: str
) -> None:
    if left.keys() != right.keys():
        raise CaxecraftFailure(
            f"{label} artifact sets differ: {sorted(left)!r} != {sorted(right)!r}"
        )
    for path in left:
        if left[path] != right[path]:
            raise CaxecraftFailure(
                f"{label} first differs in {path!r} at byte "
                f"{first_difference(left[path], right[path])}"
            )


def alternate_locale() -> str:
    result = subprocess.run(
        ["locale", "-a"], check=False, capture_output=True, text=True, timeout=10
    )
    available = {line.strip() for line in result.stdout.splitlines()}
    for candidate in ("C.UTF-8", "C.utf8", "en_US.UTF-8", "en_US.utf8"):
        if candidate in available:
            return candidate
    return "C"


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10.0
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise CaxecraftFailure(
                f"Haxe server exited early\nstdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise CaxecraftFailure("Haxe server did not accept connections")


def check_determinism(first: RenderedProject, root: Path) -> None:
    repeated = render_project(root / "repeated", label="repeated cold Caxecraft render")
    reversed_project = render_project(
        root / "reversed",
        label="reverse-order/locale Caxecraft render",
        reverse=True,
        locale=alternate_locale(),
    )
    assert_artifacts_equal(first.artifacts, repeated.artifacts, "repeated cold render")
    assert_artifacts_equal(
        first.artifacts, reversed_project.artifacts, "reverse-order/locale render"
    )

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
        warm_first = render_project(
            root / "warm-first",
            label="first warm-server Caxecraft render",
            connect=endpoint,
        )
        warm_repeated = render_project(
            root / "warm-repeated",
            label="repeated warm-server Caxecraft render",
            connect=endpoint,
        )
        assert_artifacts_equal(first.artifacts, warm_first.artifacts, "cold/warm render")
        assert_artifacts_equal(
            warm_first.artifacts, warm_repeated.artifacts, "warm-server repeated render"
        )
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def snapshot_values() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-snapshot-") as temporary:
        project = render_project(
            Path(temporary) / "generated",
            label="Caxecraft snapshot render",
            report=True,
        )
        oracle = run_oracle().decode("ascii")
        return {
            **{
                path: (project.output / path).read_text(encoding="utf-8")
                for path in (*SPLIT_HEADERS, *SPLIT_SOURCES)
            },
            "hxc.runtime-plan.json": project.runtime_plan,
            "method-symbols.json": project.method_symbols,
            "readability-metrics.json": project.readability_metrics,
            "oracle.txt": oracle,
        }


def expected_values() -> dict[str, object]:
    missing = [name for name in SNAPSHOT_FORMATS if not (EXPECTED / name).is_file()]
    if missing:
        raise CaxecraftFailure(
            "Caxecraft baseline is missing; run "
            "`npm run snapshots:update -- --suite caxecraft-domain`: "
            f"{missing!r}"
        )
    return {
        name: load_json(EXPECTED / name, f"expected {name}")
        if format_name == "json"
        else (EXPECTED / name).read_text(encoding="utf-8")
        for name, format_name in SNAPSHOT_FORMATS.items()
    }


def validate_expected(values: dict[str, object]) -> tuple[dict[str, bytes], bytes]:
    oracle = values.get("oracle.txt")
    runtime_plan = values.get("hxc.runtime-plan.json")
    method_symbols = values.get("method-symbols.json")
    readability_metrics = values.get("readability-metrics.json")
    generated = {
        path: values.get(path) for path in (*SPLIT_HEADERS, *SPLIT_SOURCES)
    }
    if not isinstance(oracle, str) or not all(
        isinstance(value, str) for value in generated.values()
    ):
        raise CaxecraftFailure("Caxecraft text baseline is malformed")
    if (
        not isinstance(runtime_plan, dict)
        or not isinstance(method_symbols, dict)
        or not isinstance(readability_metrics, dict)
    ):
        raise CaxecraftFailure("Caxecraft JSON baseline is malformed")
    validate_runtime_plan(runtime_plan)
    validate_method_symbols(method_symbols)
    generated_bytes = {
        path: value.encode("utf-8")
        for path, value in generated.items()
        if isinstance(value, str)
    }
    oracle_bytes = oracle.encode("ascii")
    validate_generated_text(
        b"\n".join(generated_bytes[path] for path in SPLIT_HEADERS),
        b"\n".join(generated_bytes[path] for path in SPLIT_SOURCES),
        method_symbols,
    )
    computed_metrics = generated_readability_metrics(
        b"\n".join(generated_bytes[path] for path in SPLIT_HEADERS),
        b"\n".join(generated_bytes[path] for path in SPLIT_SOURCES),
    )
    if readability_metrics != computed_metrics:
        raise CaxecraftFailure(
            "checked-in readability metrics do not describe the checked-in C"
        )
    validate_readability_metrics(readability_metrics)
    block_coord = generated.get(
        "include/hxc/modules/caxecraft/domain/BlockCoord.h"
    )
    if not isinstance(block_coord, str):
        raise CaxecraftFailure("checked-in Caxecraft baseline omitted BlockCoord.h")
    validate_block_coord_header(block_coord)
    lines = oracle_bytes.splitlines()
    if len(lines) != 38 or lines[0] != b"0" or not oracle_bytes.endswith(b"\n"):
        raise CaxecraftFailure("checked-in Caxecraft oracle baseline drifted")
    return generated_bytes, oracle_bytes


def validate_snapshots(project: RenderedProject, oracle: bytes) -> None:
    expected = expected_values()
    actual: dict[str, object] = {
        **{
            path: (project.output / path).read_text(encoding="utf-8")
            for path in (*SPLIT_HEADERS, *SPLIT_SOURCES)
        },
        "hxc.runtime-plan.json": project.runtime_plan,
        "method-symbols.json": project.method_symbols,
        "readability-metrics.json": project.readability_metrics,
        "oracle.txt": oracle.decode("ascii"),
    }
    if actual == expected:
        return
    for name in SNAPSHOT_FORMATS:
        if actual[name] != expected[name]:
            if isinstance(actual[name], str) and isinstance(expected[name], str):
                difference = "".join(
                    difflib.unified_diff(
                        expected[name].splitlines(keepends=True),
                        actual[name].splitlines(keepends=True),
                        fromfile=f"expected/{name}",
                        tofile=f"actual/{name}",
                        n=3,
                    )
                )
                detail = difference[:4000]
            else:
                detail = "semantic JSON differs"
            raise CaxecraftFailure(
                f"Caxecraft snapshot {name!r} drifted; run "
                "`npm run snapshots:update -- --suite caxecraft-domain`\n"
                f"{detail}"
            )


def prepare_native_fixture(
    fixture: Path, project: RenderedProject, layout: str
) -> None:
    shutil.copytree(project.output / "include", fixture / "generated/include")
    (fixture / "generated/src").mkdir(parents=True)
    (fixture / "native").mkdir(parents=True)
    shutil.copy2(NATIVE / "domain_harness.c", fixture / "native/domain_harness.c")
    (fixture / "native/method_symbols.h").write_text(
        native_method_symbol_header(project.method_symbols), encoding="utf-8"
    )
    if layout == "unity":
        shutil.copy2(
            project.output / "src/program.c", fixture / "generated/src/program.c"
        )
        shutil.copy2(
            NATIVE / "generated_program.c", fixture / "native/generated_program.c"
        )
    elif layout == "split":
        for relative in SPLIT_SOURCES:
            if relative == "src/hxc/main.c":
                continue
            destination = fixture / "generated" / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(project.output / relative, destination)
    else:
        raise CaxecraftFailure(f"unknown native Caxecraft layout {layout!r}")


def native_project(layout: str, oracle: bytes, *, sanitizer: bool) -> CFixtureProject:
    if layout == "split":
        sources = (
            "native/domain_harness.c",
            *(
                f"generated/{path}"
                for path in SPLIT_SOURCES
                if path != "src/hxc/main.c"
            ),
        )
        headers = (
            "native/method_symbols.h",
            *(f"generated/{path}" for path in SPLIT_HEADERS),
        )
    elif layout == "unity":
        sources = ("native/domain_harness.c", "native/generated_program.c")
        headers = (
            "native/method_symbols.h",
            "generated/include/hxc/program.h",
            "generated/src/program.c",
        )
    else:
        raise CaxecraftFailure(f"unknown native Caxecraft layout {layout!r}")
    return CFixtureProject(
        identifier=f"caxecraft-domain-{layout}",
        sources=sources,
        headers=headers,
        include_directories=("generated/include",),
        expected_stdout=oracle.decode("ascii"),
        coverage=tuple(sorted(COVERAGE)),
        link_arguments=SANITIZER_FLAGS if sanitizer else (),
    )


def inspect_generated_object_symbols(
    build_root: Path, report: dict[str, object], layout: str
) -> None:
    toolchains = report.get("toolchains")
    if not isinstance(toolchains, list):
        raise CaxecraftFailure("native Caxecraft report omitted toolchains")
    for entry in toolchains:
        if not isinstance(entry, dict) or not isinstance(entry.get("family"), str):
            raise CaxecraftFailure("native Caxecraft report has a malformed toolchain")
        family = entry["family"]
        project_root = build_root / family / f"caxecraft-domain-{layout}"
        objects = sorted(project_root.glob("*.o"))
        if not objects:
            raise CaxecraftFailure(
                f"cannot inspect {family} generated Caxecraft object symbols"
            )
        imported: set[str] = set()
        for generated_object in objects:
            result = subprocess.run(
                ["nm", "-u", str(generated_object)],
                cwd=ROOT,
                check=False,
                capture_output=True,
                text=True,
                timeout=30,
            )
            if result.returncode != 0:
                raise CaxecraftFailure(
                    f"cannot inspect {family} generated Caxecraft object symbols\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
            imported.update(
                line.split()[-1].lstrip("_").lower()
                for line in result.stdout.splitlines()
                if line.split()
            )
        forbidden = sorted(
            symbol
            for symbol in imported
            if symbol in {"malloc", "calloc", "realloc", "free"}
            or symbol.startswith("hxrt_")
        )
        if forbidden:
            raise CaxecraftFailure(
                f"{family} generated Caxecraft object imports forbidden "
                f"symbols: {', '.join(forbidden)}"
            )


def check_standalone_headers(
    project: RenderedProject, layout: str, requested_toolchain: str
) -> None:
    headers = (
        SPLIT_HEADERS if layout == "split" else ("include/hxc/program.h",)
    )
    include_root = project.output / "include"
    for toolchain in resolve_toolchains(
        requested_toolchain, repository_root=ROOT
    ):
        for header in headers:
            result = subprocess.run(
                [
                    toolchain.compiler,
                    *STRICT_FLAGS,
                    "-I",
                    str(include_root),
                    "-x",
                    "c",
                    "-fsyntax-only",
                    "-",
                ],
                input=f'#include "{header.removeprefix("include/")}"\n',
                check=False,
                capture_output=True,
                text=True,
                timeout=30,
            )
            if result.returncode != 0 or result.stdout or result.stderr:
                raise CaxecraftFailure(
                    f"{toolchain.family} rejected standalone {layout} header "
                    f"{header}\n{result.stdout}{result.stderr}"
                )


def run_native(
    project: RenderedProject,
    layout: str,
    oracle: bytes,
    *,
    requested_toolchain: str,
    root: Path,
    full: bool,
) -> None:
    fixture = root / f"fixture-{layout}"
    prepare_native_fixture(fixture, project, layout)
    if full:
        progress(f"standalone {layout} headers")
        check_standalone_headers(project, layout, requested_toolchain)
    modes = (
        (
            ("o0", ("-O0",), False),
            ("o2", ("-O2",), False),
            ("sanitizer", ("-O1", *SANITIZER_FLAGS), True),
        )
        if full
        else (("o2", ("-O2",), False),)
    )
    for mode, extra_flags, sanitizer in modes:
        progress(f"native {layout}/{mode}")
        build_root = root / mode
        report = run_c_fixture_corpus(
            suite=f"caxecraft-domain-{layout}-{mode}",
            projects=(native_project(layout, oracle, sanitizer=sanitizer),),
            fixture_root=fixture,
            build_root=build_root,
            repository_root=ROOT,
            requested_toolchain=requested_toolchain,
            strict_flags=(*STRICT_FLAGS, *extra_flags),
            timeout_seconds=120,
        )
        validate_report(report, required_coverage=COVERAGE)
        inspect_generated_object_symbols(build_root, report, layout)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument(
        "--native-only",
        action="store_true",
        help="compile and run the checked-in generated baseline without Haxe",
    )
    parser.add_argument(
        "--full",
        action="store_true",
        help="run the exhaustive determinism and O0/O2/sanitizer CI matrix",
    )
    return parser.parse_args(list(argv))


def progress(stage: str) -> None:
    print(f"caxecraft-domain: [{stage}]", flush=True)


def checked_in_split_project(root: Path, values: dict[str, object]) -> tuple[RenderedProject, bytes]:
    generated, oracle = validate_expected(values)
    for relative, content in generated.items():
        destination = root / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(content)
    runtime_plan = values.get("hxc.runtime-plan.json")
    method_symbols = values.get("method-symbols.json")
    readability_metrics = values.get("readability-metrics.json")
    if (
        not isinstance(runtime_plan, dict)
        or not isinstance(method_symbols, dict)
        or not isinstance(readability_metrics, dict)
    ):
        raise CaxecraftFailure("checked-in Caxecraft JSON baseline is malformed")
    return (
        RenderedProject(
            root,
            {},
            "",
            runtime_plan,
            method_symbols,
            readability_metrics,
        ),
        oracle,
    )


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        progress("asset manifest + negative contracts")
        validate_asset_pack(CASE / "assets")
        negative_contracts()
        with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-domain-") as temporary:
            root = Path(temporary)
            if args.native_only:
                progress("load checked-in split baseline")
                split, oracle = checked_in_split_project(
                    root / "checked-in-split", expected_values()
                )
                unity = None
            else:
                progress("Eval oracle")
                oracle = run_oracle()
                progress("split render + HxcIR")
                first = render_project(
                    root / "first",
                    label="first cold Caxecraft render",
                    report=True,
                )
                progress("unity render + semantic parity")
                unity = render_project(
                    root / "unity",
                    label="unity Caxecraft render",
                    layout="unity",
                    report=True,
                )
                if (
                    first.hxcir != unity.hxcir
                    or first.runtime_plan != unity.runtime_plan
                    or first.method_symbols != unity.method_symbols
                ):
                    raise CaxecraftFailure(
                        "split and unity layouts changed HxcIR, runtime, or method symbols"
                    )
                if args.full:
                    progress("cold/reversed/locale/warm determinism")
                    check_determinism(first, root / "determinism")
                progress("checked-in split snapshots")
                validate_snapshots(first, oracle)
                split = first
            progress("split native differential")
            run_native(
                split,
                "split",
                oracle,
                requested_toolchain=args.toolchain,
                root=root / "native-split",
                full=args.full or args.native_only,
            )
            if unity is not None:
                progress("unity native differential")
                run_native(
                    unity,
                    "unity",
                    oracle,
                    requested_toolchain=args.toolchain,
                    root=root / "native-unity",
                    full=args.full or args.native_only,
                )
    except (
        AssetValidationError,
        CFixtureFailure,
        CaxecraftFailure,
        OSError,
        UnicodeError,
        ValueError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"caxecraft-domain: ERROR: {error}", file=sys.stderr)
        return 1

    mode = "checked-in split C baseline" if args.native_only else "Eval/split+unity generated-C differential"
    matrix = "full O0/O2/ASan+UBSan" if args.full or args.native_only else "quick O2"
    parity = (
        "checked-in split layout validation"
        if args.native_only
        else "split/unity layout semantic parity"
    )
    print(
        "caxecraft-domain: OK: "
        f"{mode}, 32 seeded properties, exact traces, {matrix}, "
        f"zero hxrt/allocation symbols, bounded readable C metrics, and {parity} passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

#!/usr/bin/env python3
"""Prove private concrete-class layout, safe upcasts, and fail-closed edges."""

from __future__ import annotations

import argparse
import copy
import difflib
import json
import os
import signal
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Iterable
from dataclasses import replace
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("class_layout.hxml")
FIXTURES = Path(__file__).with_name("fixtures")
POSITIVE = FIXTURES / "positive"
NATIVE = Path(__file__).with_name("native")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_CLASS_LAYOUT="
PRODUCTION_FILES = {
    "_GeneratedFiles.json",
    "cmake/CMakeLists.txt",
    "hxc.abi.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
    "include/hxc/program.h",
    "meson.build",
    "src/program.c",
}

if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.c_fixture_harness import (  # noqa: E402
    C11_STRICT_FLAGS,
    CFixtureFailure,
    CFixtureProject,
    report_json,
    resolve_toolchains,
    run_c_fixture_corpus,
    validate_report,
)


CXX_STRICT_FLAGS = (
    "-std=c++17",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)
CXX_COMMANDS = {"gcc": "g++", "clang": "clang++"}
SANITIZER_FLAGS = (
    "-fsanitize=address,undefined",
    "-fno-sanitize-recover=all",
    "-fno-omit-frame-pointer",
)
REQUIRED_COVERAGE = frozenset(
    {
        "base-prefix-layout",
        "c-consumer",
        "checked-field-access",
        "class-reference-equality",
        "cpp-consumer",
        "dominating-receiver-guards",
        "empty-class-layout",
        "generated-executable",
        "null-preserving-upcast",
        "null-receiver-abort",
        "private-layout",
        "runtime-free",
    }
)


class ClassLayoutFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return environment


def render(
    label: str, *, reverse: bool = False, profile: str = "portable"
) -> tuple[str, dict[str, object]]:
    if profile not in ("portable", "metal"):
        raise ClassLayoutFailure(f"unknown class-layout profile {profile!r}")
    command = [development_tool("haxe"), str(HXML)]
    if reverse:
        command.extend(["-D", "class_layout_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "class_layout_profile=metal"])
    result = subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if result.returncode != 0 or result.stderr or len(lines) != 1:
        raise ClassLayoutFailure(
            f"{label} emitted an invalid report\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise ClassLayoutFailure(f"{label} report is not an object")
    return payload, report


def required_text(report: dict[str, object], key: str) -> str:
    value = report.get(key)
    if not isinstance(value, str):
        raise ClassLayoutFailure(f"class-layout report field {key!r} must be text")
    return value


def object_records(report: dict[str, object], key: str) -> list[dict[str, object]]:
    value = report.get(key)
    if not isinstance(value, list) or not all(isinstance(item, dict) for item in value):
        raise ClassLayoutFailure(f"class-layout report omitted {key!r} records")
    return value


def source_records(report: dict[str, object]) -> dict[str, str]:
    records = object_records(report, "sources")
    result: dict[str, str] = {}
    ordered: list[str] = []
    for record in records:
        path = record.get("path")
        content = record.get("content")
        if not isinstance(path, str) or not isinstance(content, str):
            raise ClassLayoutFailure(f"malformed class-layout source record: {record!r}")
        if (
            path in result
            or path.startswith("/")
            or "\\" in path
            or any(part in ("", ".", "..") for part in path.split("/"))
        ):
            raise ClassLayoutFailure(f"unsafe or duplicate source path {path!r}")
        ordered.append(path)
        result[path] = content
    if ordered != sorted(ordered, key=lambda value: value.encode("utf-8")):
        raise ClassLayoutFailure("class-layout source records are not UTF-8 ordered")
    return result


def class_map(report: dict[str, object]) -> dict[str, dict[str, object]]:
    records = object_records(report, "classes")
    result: dict[str, dict[str, object]] = {}
    for record in records:
        path = record.get("haxePath")
        if not isinstance(path, str) or path in result:
            raise ClassLayoutFailure("class-layout report has missing or duplicate Haxe paths")
        result[path] = record
    expected = {"EmptyBase", "EmptyLeaf", "RootRecord", "MiddleRecord", "LeafRecord"}
    if set(result) != expected:
        raise ClassLayoutFailure(f"class-layout nominal set drifted: {sorted(result)!r}")
    return result


def field_map(record: dict[str, object]) -> dict[str, dict[str, object]]:
    fields = record.get("fields")
    if not isinstance(fields, list) or not all(isinstance(field, dict) for field in fields):
        raise ClassLayoutFailure("class-layout field records are malformed")
    result: dict[str, dict[str, object]] = {}
    for field in fields:
        name = field.get("semanticName")
        if not isinstance(name, str) or name in result:
            raise ClassLayoutFailure("class-layout field semantic names are invalid")
        result[name] = field
    return result


def function_map(report: dict[str, object]) -> dict[str, str]:
    result: dict[str, str] = {}
    for record in object_records(report, "functions"):
        semantic = record.get("semanticName")
        c_name = record.get("cName")
        if (
            not isinstance(semantic, str)
            or not isinstance(c_name, str)
            or not c_name
            or semantic in result
        ):
            raise ClassLayoutFailure("class-layout function records are malformed")
        result[semantic] = c_name
    expected = {
        f"ClassLayoutFixture.{name}"
        for name in (
            "asEmptyBase",
            "asMiddle",
            "asRoot",
            "branchProofDoesNotEscape",
            "different",
            "isNull",
            "main",
            "readInherited",
            "readPeer",
            "readRoot",
            "readScore",
            "same",
            "sumAcrossBranch",
            "writeInherited",
            "writePeer",
        )
    }
    if set(result) != expected:
        raise ClassLayoutFailure("class-layout function inventory drifted")
    return result


def required_identifier(record: dict[str, object], key: str, owner: str) -> str:
    value = record.get(key)
    if not isinstance(value, str) or not value:
        raise ClassLayoutFailure(f"{owner} omitted finalized identifier {key!r}")
    return value


def contract_names(report: dict[str, object]) -> dict[str, str]:
    classes = class_map(report)
    root = classes["RootRecord"]
    middle = classes["MiddleRecord"]
    leaf = classes["LeafRecord"]
    empty_base = classes["EmptyBase"]
    empty_leaf = classes["EmptyLeaf"]
    root_fields = field_map(root)
    middle_fields = field_map(middle)
    leaf_fields = field_map(leaf)
    if (
        list(root_fields) != ["rootValue"]
        or list(middle_fields) != ["enabled"]
        or list(leaf_fields) != ["score", "peer"]
        or root_fields["rootValue"].get("type") != "i32"
        or middle_fields["enabled"].get("type") != "bool"
        or leaf_fields["score"].get("type") != "f64"
        or leaf_fields["peer"].get("type")
        != f"nullable-instance:{root.get('instanceId')}"
        or any(field.get("mutable") is not True for field in (*root_fields.values(), *middle_fields.values(), *leaf_fields.values()))
    ):
        raise ClassLayoutFailure("source-order class storage contract drifted")
    if (
        root.get("baseInstanceId") is not None
        or middle.get("baseInstanceId") != root.get("instanceId")
        or leaf.get("baseInstanceId") != middle.get("instanceId")
        or empty_base.get("baseInstanceId") is not None
        or empty_leaf.get("baseInstanceId") != empty_base.get("instanceId")
        or root.get("emptyAnchor") is not None
        or middle.get("emptyAnchor") is not None
        or leaf.get("emptyAnchor") is not None
        or empty_leaf.get("emptyAnchor") is not None
    ):
        raise ClassLayoutFailure("base-first or empty-class representation drifted")
    functions = function_map(report)
    names = {
        "root_tag": required_identifier(root, "cTag", "RootRecord"),
        "root_value": required_identifier(root_fields["rootValue"], "cName", "RootRecord.rootValue"),
        "middle_tag": required_identifier(middle, "cTag", "MiddleRecord"),
        "middle_base": required_identifier(middle, "baseMember", "MiddleRecord"),
        "middle_enabled": required_identifier(middle_fields["enabled"], "cName", "MiddleRecord.enabled"),
        "leaf_tag": required_identifier(leaf, "cTag", "LeafRecord"),
        "leaf_base": required_identifier(leaf, "baseMember", "LeafRecord"),
        "leaf_score": required_identifier(leaf_fields["score"], "cName", "LeafRecord.score"),
        "leaf_peer": required_identifier(leaf_fields["peer"], "cName", "LeafRecord.peer"),
        "empty_base_tag": required_identifier(empty_base, "cTag", "EmptyBase"),
        "empty_base_anchor": required_identifier(empty_base, "emptyAnchor", "EmptyBase"),
        "empty_leaf_tag": required_identifier(empty_leaf, "cTag", "EmptyLeaf"),
        "empty_leaf_base": required_identifier(empty_leaf, "baseMember", "EmptyLeaf"),
    }
    for semantic, c_name in functions.items():
        names["fn_" + semantic.rsplit(".", 1)[1]] = c_name
    return names


def function_section(hxcir: str, field: str) -> str:
    start_marker = f'  function "function.ClassLayoutFixture.{field}"'
    end_marker = f'  end function "function.ClassLayoutFixture.{field}"'
    start = hxcir.find(start_marker)
    end = hxcir.find(end_marker, start)
    if start == -1 or end == -1:
        raise ClassLayoutFailure(f"HxcIR omitted function {field}")
    return hxcir[start : end + len(end_marker)]


def null_check_count(hxcir_function: str) -> int:
    """Count semantic null-check instructions, excluding their descriptive IDs."""
    return hxcir_function.count(" result=- null-check value=")


def c_function_section(source: str, c_name: str) -> str:
    """Return one generated C definition so structural checks stay function-local."""
    cursor = 0
    while True:
        name = source.find(c_name + "(", cursor)
        if name == -1:
            raise ClassLayoutFailure(f"generated C omitted function {c_name}")
        opening = source.find("{", name)
        semicolon = source.find(";", name)
        if opening != -1 and (semicolon == -1 or opening < semicolon):
            depth = 0
            for index in range(opening, len(source)):
                if source[index] == "{":
                    depth += 1
                elif source[index] == "}":
                    depth -= 1
                    if depth == 0:
                        start = source.rfind("\n", 0, name) + 1
                        return source[start : index + 1]
            raise ClassLayoutFailure(f"generated C function {c_name} has no closing brace")
        cursor = name + len(c_name) + 1


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "concrete-private-class-layouts-direct-runtime-free"
        or report.get("profile") != profile
        or report.get("runtimeFeatures") != []
        or report.get("runtimeArtifacts") != []
    ):
        raise ClassLayoutFailure(f"class-layout report contract drifted for {profile}")
    names = contract_names(report)
    hxcir = required_text(report, "hxcir")
    header = required_text(report, "header")
    sources = source_records(report)
    if set(sources) != {"src/program.c"}:
        raise ClassLayoutFailure(f"class-layout source partition drifted: {sorted(sources)!r}")
    source = sources["src/program.c"]
    for label, value in (("HxcIR", hxcir), ("header", header), ("source", source)):
        if str(ROOT) in value or "\\" in value or "hxrt" in value.lower():
            raise ClassLayoutFailure(f"{label} leaked a host path or runtime dependency")
    if hxcir.count(" kind=class ") != 5 or hxcir.count(" header=none") != 5 or hxcir.count(" representation=direct ") != 5:
        raise ClassLayoutFailure("HxcIR class declarations lost bounded direct representation")
    inherited = function_section(hxcir, "readInherited")
    if (
        inherited.find("null-check") == -1
        or inherited.find('load place=field(dereference("parameter.0"),"rootValue")') == -1
        or inherited.find("null-check") > inherited.find("load place=field")
    ):
        raise ClassLayoutFailure("inherited field access lost its explicit null proof")
    if null_check_count(function_section(hxcir, "writeInherited")) != 1:
        raise ClassLayoutFailure("same-block receiver checks were not coalesced")
    if null_check_count(function_section(hxcir, "writePeer")) != 1:
        raise ClassLayoutFailure("same-block nullable-field checks were not coalesced")
    if null_check_count(function_section(hxcir, "sumAcrossBranch")) != 1:
        raise ClassLayoutFailure("entry receiver proof did not dominate branch and join reads")
    branch_join = function_section(hxcir, "branchProofDoesNotEscape")
    if null_check_count(branch_join) != 4:
        raise ClassLayoutFailure("branch-local receiver proofs escaped through an uncertain join")
    upcast = function_section(hxcir, "asRoot")
    if "convert value=" not in upcast or "kind=representation" not in upcast or "implementation=static" not in upcast:
        raise ClassLayoutFailure("derived-to-base conversion lost its inspectable representation step")
    if 'operation="haxe.class-reference.equal"' not in function_section(hxcir, "same") or 'operation="haxe.class-reference.not-equal"' not in function_section(hxcir, "different"):
        raise ClassLayoutFailure("class reference equality lost identity semantics")
    for name in (
        names["root_tag"],
        names["middle_tag"],
        names["leaf_tag"],
        names["empty_base_tag"],
        names["empty_leaf_tag"],
    ):
        if f"struct {name};" not in header or f"struct {name} {{" not in header:
            raise ClassLayoutFailure(f"private header omitted forward/complete class {name}")
    middle_definition = header[header.find(f"struct {names['middle_tag']} {{") :]
    leaf_definition = header[header.find(f"struct {names['leaf_tag']} {{") :]
    if (
        middle_definition.find(names["middle_base"]) > middle_definition.find(names["middle_enabled"])
        or leaf_definition.find(names["leaf_base"]) > leaf_definition.find(names["leaf_score"])
        or leaf_definition.find(names["leaf_score"]) > leaf_definition.find(names["leaf_peer"])
        or f"unsigned char {names['empty_base_anchor']};" not in header
        or "#include <stddef.h>" not in header
        or "#include <stdlib.h>" not in header
    ):
        raise ClassLayoutFailure("base-first private struct/header contract drifted")
    root_upcast = names["fn_asRoot"]
    if (
        source.count("_Static_assert(") < 20
        or source.count("offsetof(") < 10
        or source.count("abort();") < 6
        or f"{root_upcast}(" not in source
        or "== NULL ? NULL : &" not in source
        or f".{names['leaf_base']}.{names['middle_base']}" not in source
        or "int main(void)" not in source
    ):
        raise ClassLayoutFailure("structural class CAST emission/layout assertions drifted")
    for key in ("fn_writeInherited", "fn_writePeer", "fn_sumAcrossBranch"):
        if c_function_section(source, names[key]).count("abort();") != 1:
            raise ClassLayoutFailure(f"generated C did not retain exactly one dominating guard in {key}")
    if c_function_section(source, names["fn_branchProofDoesNotEscape"]).count("abort();") != 4:
        raise ClassLayoutFailure("generated C incorrectly reused a branch-local receiver guard")
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v2":
        raise ClassLayoutFailure("class-layout report omitted its finalized symbol table")


def normalized_profile(report: dict[str, object]) -> dict[str, object]:
    value = copy.deepcopy(report)
    value["profile"] = "<profile>"
    hxcir = value.get("hxcir")
    if isinstance(hxcir, str):
        value["hxcir"] = hxcir.replace('profile="portable"', 'profile="<profile>"').replace(
            'profile="metal"', 'profile="<profile>"'
        )
    return value


def snapshot_values(report: dict[str, object]) -> dict[str, object]:
    return {
        "classes.hxcir": required_text(report, "hxcir"),
        "program.h": required_text(report, "header"),
        "program.c": source_records(report)["src/program.c"],
        "classes.json": object_records(report, "classes"),
        "functions.json": object_records(report, "functions"),
        "symbols.json": report.get("symbols"),
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


def check_snapshots(report: dict[str, object]) -> None:
    for name, actual in snapshot_values(report).items():
        path = EXPECTED / name
        if not path.is_file():
            raise ClassLayoutFailure(
                "class-layout snapshots are missing; run "
                "npm run snapshots:update -- --suite class-layout"
            )
        if name.endswith(".json"):
            expected = json.loads(path.read_text(encoding="utf-8"))
            if actual != expected:
                raise ClassLayoutFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise ClassLayoutFailure(f"{name} snapshot is not text")
            expected = path.read_text(encoding="utf-8")
            if actual != expected:
                raise ClassLayoutFailure(f"{name} drifted:\n" + difference(expected, actual, name))


def macro_definitions(report: dict[str, object]) -> tuple[str, ...]:
    names = contract_names(report)
    definitions: list[str] = []
    for macro, key in (
        ("HXC_ROOT_TAG", "root_tag"),
        ("HXC_ROOT_VALUE", "root_value"),
        ("HXC_MIDDLE_TAG", "middle_tag"),
        ("HXC_MIDDLE_BASE", "middle_base"),
        ("HXC_MIDDLE_ENABLED", "middle_enabled"),
        ("HXC_LEAF_TAG", "leaf_tag"),
        ("HXC_LEAF_BASE", "leaf_base"),
        ("HXC_LEAF_SCORE", "leaf_score"),
        ("HXC_LEAF_PEER", "leaf_peer"),
        ("HXC_EMPTY_BASE_TAG", "empty_base_tag"),
        ("HXC_EMPTY_BASE_ANCHOR", "empty_base_anchor"),
        ("HXC_EMPTY_LEAF_TAG", "empty_leaf_tag"),
        ("HXC_EMPTY_LEAF_BASE", "empty_leaf_base"),
        ("HXC_FN_AS_EMPTY_BASE", "fn_asEmptyBase"),
        ("HXC_FN_AS_MIDDLE", "fn_asMiddle"),
        ("HXC_FN_AS_ROOT", "fn_asRoot"),
        ("HXC_FN_BRANCH_PROOF_DOES_NOT_ESCAPE", "fn_branchProofDoesNotEscape"),
        ("HXC_FN_DIFFERENT", "fn_different"),
        ("HXC_FN_IS_NULL", "fn_isNull"),
        ("HXC_FN_READ_INHERITED", "fn_readInherited"),
        ("HXC_FN_READ_PEER", "fn_readPeer"),
        ("HXC_FN_READ_ROOT", "fn_readRoot"),
        ("HXC_FN_READ_SCORE", "fn_readScore"),
        ("HXC_FN_SAME", "fn_same"),
        ("HXC_FN_SUM_ACROSS_BRANCH", "fn_sumAcrossBranch"),
        ("HXC_FN_WRITE_INHERITED", "fn_writeInherited"),
        ("HXC_FN_WRITE_PEER", "fn_writePeer"),
    ):
        definitions.append(f"-D{macro}={names[key]}")
    return tuple(definitions)


def write_native_fixture(report: dict[str, object], root: Path) -> None:
    header = root / "include/hxc/program.h"
    source = root / "src/program.c"
    native = root / "native/behavior_consumer.c"
    null_native = root / "native/null_consumer.c"
    header.parent.mkdir(parents=True)
    source.parent.mkdir(parents=True)
    native.parent.mkdir(parents=True)
    header.write_text(required_text(report, "header"), encoding="utf-8", newline="\n")
    source.write_text(source_records(report)["src/program.c"], encoding="utf-8", newline="\n")
    shutil.copyfile(NATIVE / "behavior_consumer.c", native)
    shutil.copyfile(NATIVE / "null_consumer.c", null_native)


def run_harness_matrix(
    report: dict[str, object], root: Path, *, requested_toolchain: str
) -> list[dict[str, object]]:
    fixture_root = root / "fixture"
    write_native_fixture(report, fixture_root)
    projects = (
        CFixtureProject(
            "class-behavior",
            ("native/behavior_consumer.c",),
            ("include/hxc/program.h", "src/program.c"),
            ("include",),
            "",
            (
                "base-prefix-layout",
                "c-consumer",
                "checked-field-access",
                "class-reference-equality",
                "dominating-receiver-guards",
                "empty-class-layout",
                "null-preserving-upcast",
                "private-layout",
            ),
        ),
        CFixtureProject(
            "generated-program",
            ("src/program.c",),
            ("include/hxc/program.h",),
            ("include",),
            "",
            ("generated-executable", "runtime-free"),
        ),
        CFixtureProject(
            "null-receiver",
            ("native/null_consumer.c",),
            ("include/hxc/program.h", "src/program.c"),
            ("include",),
            "",
            ("null-receiver-abort",),
            expected_exit=-signal.SIGABRT,
        ),
    )
    reports: list[dict[str, object]] = []
    for optimization in ("-O0", "-O2"):
        report_value = run_c_fixture_corpus(
            suite=f"class-layout-{optimization[1:].lower()}",
            projects=projects,
            fixture_root=fixture_root,
            build_root=root / f"c-build-{optimization[1:].lower()}",
            repository_root=ROOT,
            requested_toolchain=requested_toolchain,
            strict_flags=(*C11_STRICT_FLAGS, optimization, *macro_definitions(report)),
        )
        validate_report(
            report_value,
            required_coverage=REQUIRED_COVERAGE - {"cpp-consumer"},
        )
        encoded = report_json(report_value, compact=True)
        for forbidden in (str(ROOT), str(fixture_root), str(root)):
            if forbidden in encoded:
                raise ClassLayoutFailure(f"native harness report leaked absolute path {forbidden}")
        reports.append(report_value)
    for toolchain in resolve_toolchains(requested_toolchain, repository_root=ROOT):
        if not sanitizer_supported(toolchain.compiler, toolchain.family, root):
            print(
                f"class-layout: SKIP optional {toolchain.family} address/undefined "
                "sanitizers: toolchain does not provide them"
            )
            continue
        sanitized_projects = tuple(
            replace(
                project,
                identifier=project.identifier + "-sanitized",
                coverage=project.coverage + ("sanitizers",),
                link_arguments=SANITIZER_FLAGS,
            )
            for project in projects
            if project.expected_exit == 0
        )
        report_value = run_c_fixture_corpus(
            suite=f"class-layout-sanitized-{toolchain.family}",
            projects=sanitized_projects,
            fixture_root=fixture_root,
            build_root=root / f"c-build-sanitized-{toolchain.family}",
            repository_root=ROOT,
            requested_toolchain=toolchain.family,
            strict_flags=(
                *C11_STRICT_FLAGS,
                "-O1",
                *SANITIZER_FLAGS,
                *macro_definitions(report),
            ),
        )
        validate_report(report_value, required_coverage=frozenset({"sanitizers"}))
        reports.append(report_value)
    return reports


def sanitizer_supported(compiler: str, family: str, root: Path) -> bool:
    """Probe sanitizer availability without weakening the strict native lanes."""
    source = root / f"sanitizer-probe-{family}.c"
    executable = root / f"sanitizer-probe-{family}"
    source.write_text("int main(void) { return 0; }\n", encoding="utf-8", newline="\n")
    result = subprocess.run(
        [compiler, *SANITIZER_FLAGS, str(source), "-o", str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    return result.returncode == 0


def compiler_identity(executable: str) -> str:
    result = subprocess.run(
        [executable, "--version"],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        raise ClassLayoutFailure(f"cannot identify C++ compiler {executable}")
    text = (result.stdout + result.stderr).lower()
    if "clang" in text:
        return "clang"
    if "free software foundation" in text or "gcc" in text or "g++" in text:
        return "gcc"
    return "unknown"


def require_silent_success(command: list[str], *, label: str, cwd: Path = ROOT) -> None:
    result = subprocess.run(
        command,
        cwd=cwd,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ClassLayoutFailure(
            f"{label} failed\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def check_cpp_layout(
    report: dict[str, object], root: Path, *, requested_toolchain: str
) -> None:
    toolchains = resolve_toolchains(requested_toolchain, repository_root=ROOT)
    definitions = macro_definitions(report)
    for toolchain in toolchains:
        cxx_name = CXX_COMMANDS[toolchain.family]
        cxx = shutil.which(cxx_name)
        if cxx is None or compiler_identity(cxx) != toolchain.family:
            if requested_toolchain != "auto":
                raise ClassLayoutFailure(
                    f"required {toolchain.family} C++17 companion {cxx_name!r} is unavailable"
                )
            print(
                f"class-layout: SKIP optional {toolchain.family} C++17 consumer: "
                f"missing or mismatched {cxx_name}"
            )
            continue
        for optimization in ("-O0", "-O2"):
            build = root / "cxx-build" / toolchain.family / optimization[1:].lower()
            build.mkdir(parents=True, exist_ok=True)
            provider_object = build / "layout_provider.o"
            consumer_object = build / "layout_consumer.o"
            executable = build / "layout_consumer"
            require_silent_success(
                [
                    toolchain.compiler,
                    *C11_STRICT_FLAGS,
                    optimization,
                    *definitions,
                    f"-I{root / 'fixture/include'}",
                    "-c",
                    str(NATIVE / "layout_provider.c"),
                    "-o",
                    str(provider_object),
                ],
                label=f"{toolchain.family} {optimization} C class-layout provider",
            )
            require_silent_success(
                [
                    cxx,
                    *CXX_STRICT_FLAGS,
                    optimization,
                    *definitions,
                    f"-I{root / 'fixture/include'}",
                    "-c",
                    str(NATIVE / "layout_consumer.cpp"),
                    "-o",
                    str(consumer_object),
                ],
                label=f"{toolchain.family} {optimization} C++17 class-layout consumer",
            )
            require_silent_success(
                [cxx, str(provider_object), str(consumer_object), "-o", str(executable)],
                label=f"{toolchain.family} {optimization} C/C++17 class-layout link",
            )
            require_silent_success(
                [str(executable)],
                label=f"{toolchain.family} {optimization} C/C++17 class-layout run",
                cwd=build,
            )


def check_native(
    report: dict[str, object], *, requested_toolchain: str = "auto"
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-class-layout-native-") as temporary:
        root = Path(temporary)
        run_harness_matrix(report, root, requested_toolchain=requested_toolchain)
        check_cpp_layout(report, root, requested_toolchain=requested_toolchain)


def custom_target(
    fixture: Path,
    output: Path,
    *,
    main: str,
    profile: str = "portable",
    runtime: str | None = None,
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        main,
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    command.extend(["-D", "hxc_project_layout=unity", "--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def generated_tree(root: Path) -> dict[str, bytes]:
    return {
        path.relative_to(root).as_posix(): path.read_bytes()
        for path in sorted(root.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def generated_files(root: Path) -> set[str]:
    return {
        path.relative_to(root).as_posix()
        for path in root.rglob("*")
        if path.is_file()
    }


def require_compile_success(result: subprocess.CompletedProcess[str], label: str) -> None:
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ClassLayoutFailure(
            f"{label} failed\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def validate_production(root: Path, *, profile: str, policy: str) -> None:
    if generated_files(root) != PRODUCTION_FILES:
        raise ClassLayoutFailure(
            f"{profile}/{policy} production artifact set drifted: {sorted(generated_files(root))!r}"
        )
    manifest = json.loads((root / "hxc.manifest.json").read_text(encoding="utf-8"))
    runtime_plan = json.loads((root / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    abi = json.loads((root / "hxc.abi.json").read_text(encoding="utf-8"))
    proof = runtime_plan.get("noRuntimeProof")
    reachability = proof.get("reachability") if isinstance(proof, dict) else None
    build = manifest.get("build")
    if (
        manifest.get("compilationStatus") != "lowered-direct-value-executable"
        or manifest.get("configuration", {}).get("profile") != profile
        or not isinstance(build, dict)
        or build.get("publicHeaders") != []
        or build.get("privateHeaders") != ["include/hxc/program.h"]
        or abi.get("status") != "analyzed-no-public-exports"
        or abi.get("types") != []
        or abi.get("exports") != []
        or runtime_plan.get("schemaVersion") != 2
        or runtime_plan.get("status") != "analyzed-runtime-free"
        or runtime_plan.get("profile") != profile
        or runtime_plan.get("resolvedPolicy") != policy
        or runtime_plan.get("features") != []
        or runtime_plan.get("artifacts") != []
        or "concrete-class-reference-layouts" not in runtime_plan.get("directDecisions", [])
        or not isinstance(proof, dict)
        or proof.get("status") != "eligible"
        or proof.get("directDecisions") != runtime_plan.get("directDecisions")
        or not isinstance(reachability, dict)
        or reachability.get("typeInstances") != 5
        or reachability.get("runtimeIntents") != 0
    ):
        raise ClassLayoutFailure(f"{profile}/{policy} lost its private runtime-free class proof")
    combined = b"\n".join(
        path.read_bytes()
        for path in root.rglob("*")
        if path.is_file() and path.suffix in {".c", ".h"}
    ).lower()
    if b"hxrt" in combined or b"hxc_runtime" in combined:
        raise ClassLayoutFailure("class-layout production project selected runtime code")


def check_production() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-class-layout-production-") as temporary:
        root = Path(temporary)
        matrix = (
            ("first", "portable", None, "auto"),
            ("repeat", "portable", None, "auto"),
            ("none", "portable", "none", "none"),
            ("metal", "metal", None, "minimal"),
        )
        for name, profile, runtime, policy in matrix:
            output = root / name
            result = custom_target(
                POSITIVE,
                output,
                main="ClassLayoutFixture",
                profile=profile,
                runtime=runtime,
            )
            require_compile_success(result, f"{name} class-layout production compile")
            validate_production(output, profile=profile, policy=policy)
        if generated_tree(root / "first") != generated_tree(root / "repeat"):
            raise ClassLayoutFailure("two class-layout production roots were not byte-identical")
        for relative in ("include/hxc/program.h", "src/program.c"):
            if (root / "first" / relative).read_bytes() != (root / "metal" / relative).read_bytes():
                raise ClassLayoutFailure(f"portable and metal changed private class artifact {relative}")


def check_negative_cases() -> None:
    cases = {
        "interface": "TConst(TNull:requires-nullable-reference-or-direct-optional-context)",
        "generic": "generic-class-reference-requires-bounded-class-specialization:Box",
        "downcast": "function.Std.downcast",
    }
    with tempfile.TemporaryDirectory(prefix="hxc-class-layout-negative-") as temporary:
        root = Path(temporary)
        for directory, marker in cases.items():
            output = root / directory
            result = custom_target(FIXTURES / directory, output, main="Main")
            combined = (result.stdout + result.stderr).replace("\\", "/")
            if (
                result.returncode != 1
                or "HXC1001" not in combined
                or marker not in combined
                or f"fixtures/{directory}/Main.hx:" not in combined
                or generated_files(output)
            ):
                raise ClassLayoutFailure(
                    f"{directory} did not fail closed with exact HXC1001 and no output\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )


def snapshot_report() -> dict[str, object]:
    return {
        "schemaVersion": 1,
        "status": "concrete-private-class-layouts-direct-runtime-free",
        "profile": "portable",
        "hxcir": (EXPECTED / "classes.hxcir").read_text(encoding="utf-8"),
        "header": (EXPECTED / "program.h").read_text(encoding="utf-8"),
        "sources": [
            {
                "path": "src/program.c",
                "content": (EXPECTED / "program.c").read_text(encoding="utf-8"),
            }
        ],
        "classes": json.loads((EXPECTED / "classes.json").read_text(encoding="utf-8")),
        "functions": json.loads((EXPECTED / "functions.json").read_text(encoding="utf-8")),
        "symbols": json.loads((EXPECTED / "symbols.json").read_text(encoding="utf-8")),
        "runtimeFeatures": [],
        "runtimeArtifacts": [],
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("class-layout: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            report = snapshot_report()
            validate(report)
            check_native(report, requested_toolchain=args.toolchain)
            print("class-layout: OK: required class-layout native matrix passed")
            return 0

        first_payload, first = render("first class-layout render")
        second_payload, second = render("second class-layout render")
        reverse_payload, reverse = render("reverse-input class-layout render", reverse=True)
        _, metal = render("metal class-layout render", profile="metal")
        if first_payload != second_payload or first != second:
            raise ClassLayoutFailure("two class-layout renders differed")
        if first_payload != reverse_payload or first != reverse:
            raise ClassLayoutFailure("class layout changed with static-function discovery order")
        validate(first)
        validate(metal, profile="metal")
        if normalized_profile(first) != normalized_profile(metal):
            raise ClassLayoutFailure("portable and metal class layout diverged")
        check_snapshots(first)
        check_native(first, requested_toolchain=args.toolchain)
        check_production()
        check_negative_cases()
    except (
        ClassLayoutFailure,
        CFixtureFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"class-layout: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "class-layout: OK: private base-prefix structs, checked inherited fields, "
        "dominating receiver guards, null-preserving upcasts, identity equality, "
        "strict C11 and C++17 layout agreement, runtime-free production artifacts, "
        "and fail-closed future edges passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

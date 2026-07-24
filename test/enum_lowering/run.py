#!/usr/bin/env python3
"""Prove native/tagged Haxe-enum lowering, recursive layout, and fail-closed edges."""

from __future__ import annotations

import argparse
import copy
import difflib
import json
import os
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from collections.abc import Iterable
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("enum_lowering.hxml")
FIXTURES = Path(__file__).with_name("fixtures")
POSITIVE = FIXTURES / "positive"
NATIVE = Path(__file__).with_name("native")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_ENUM_LOWERING="
STATIC_REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
COMMON_PRODUCTION_FILES = {
    "_GeneratedFiles.json",
    "cmake/CMakeLists.txt",
    "hxc.abi.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.specializations.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
    "meson.build",
    "runtime/include/hxrt/allocator.h",
    "runtime/include/hxrt/array.h",
    "runtime/include/hxrt/base.h",
    "runtime/include/hxrt/status.h",
    "runtime/src/allocator.c",
    "runtime/src/array.c",
}
PRODUCTION_FILES_BY_LAYOUT = {
    "split": COMMON_PRODUCTION_FILES
    | {
        "include/hxc/detail/program_types.h",
        "include/hxc/modules/EnumFixture.h",
        "include/hxc/program.h",
        "src/hxc/main.c",
        "src/hxc/support.c",
        "src/modules/EnumFixture.c",
    },
    "package": COMMON_PRODUCTION_FILES
    | {
        "include/hxc/detail/program_types.h",
        "include/hxc/packages/package.h",
        "include/hxc/program.h",
        "src/hxc/main.c",
        "src/hxc/support.c",
        "src/packages/package.c",
    },
    "unity": COMMON_PRODUCTION_FILES
    | {
        "include/hxc/program.h",
        "src/program.c",
    },
}

RUNTIME_FEATURES = ["runtime-base", "status", "alloc", "array"]
RUNTIME_ARTIFACTS = [
    "runtime/include/hxrt/allocator.h",
    "runtime/include/hxrt/array.h",
    "runtime/include/hxrt/base.h",
    "runtime/include/hxrt/status.h",
    "runtime/src/allocator.c",
    "runtime/src/array.c",
]
BYTES_RUNTIME_FEATURES = ["runtime-base", "status", "alloc", "string-literal", "bytes"]
BYTES_RUNTIME_ARTIFACTS = [
    "runtime/include/hxrt/allocator.h",
    "runtime/include/hxrt/base.h",
    "runtime/include/hxrt/bytes.h",
    "runtime/include/hxrt/status.h",
    "runtime/include/hxrt/string_literal.h",
    "runtime/src/allocator.c",
    "runtime/src/bytes.c",
]

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
REQUIRED_COVERAGE = frozenset(
    {
        "checked-payload-projection",
        "c-consumer",
        "exhaustive-tag-switch",
        "fieldless-native-enum",
        "fieldless-enum-equality",
        "generated-executable",
        "recursive-finite-layout",
        "managed-record-lifecycle",
        "recursive-owned-enum",
        "tagged-union",
    }
)


class EnumLoweringFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(*, server: bool = False) -> dict[str, str]:
    environment = os.environ.copy()
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


def available_port() -> int:
    """Reserve an unused loopback port for this test's isolated Haxe server."""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    """Wait for the isolated server instead of racing the first compile request."""
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise EnumLoweringFailure(
                f"Haxe server exited early\nstdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise EnumLoweringFailure("Haxe server did not accept connections")


def render(
    label: str, *, reverse: bool = False, profile: str = "portable"
) -> tuple[str, dict[str, object]]:
    if profile not in ("portable", "metal"):
        raise EnumLoweringFailure(f"unknown enum profile {profile!r}")
    command = [development_tool("haxe"), str(HXML)]
    if reverse:
        command.extend(["-D", "enum_lowering_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "enum_lowering_profile=metal"])
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
        raise EnumLoweringFailure(
            f"{label} emitted an invalid report\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise EnumLoweringFailure(f"{label} report is not an object")
    return payload, report


def required_text(report: dict[str, object], key: str) -> str:
    value = report.get(key)
    if not isinstance(value, str):
        raise EnumLoweringFailure(f"enum report field {key!r} must be text")
    return value


def source_records(report: dict[str, object]) -> dict[str, str]:
    records = report.get("sources")
    if not isinstance(records, list):
        raise EnumLoweringFailure("enum report omitted source records")
    result: dict[str, str] = {}
    ordered: list[str] = []
    for record in records:
        if (
            not isinstance(record, dict)
            or not isinstance(record.get("path"), str)
            or not isinstance(record.get("content"), str)
        ):
            raise EnumLoweringFailure(f"malformed enum source record: {record!r}")
        path = record["path"]
        if (
            path in result
            or path.startswith("/")
            or "\\" in path
            or any(part in ("", ".", "..") for part in path.split("/"))
        ):
            raise EnumLoweringFailure(f"unsafe or duplicate source path {path!r}")
        ordered.append(path)
        result[path] = record["content"]
    if ordered != sorted(ordered, key=lambda value: value.encode("utf-8")):
        raise EnumLoweringFailure("enum source records are not UTF-8 ordered")
    return result


def enum_records(report: dict[str, object]) -> list[dict[str, object]]:
    value = report.get("enums")
    if not isinstance(value, list) or not all(isinstance(item, dict) for item in value):
        raise EnumLoweringFailure("enum report omitted representation records")
    return value


def enum_by_name(report: dict[str, object], display_name: str) -> dict[str, object]:
    matches = [item for item in enum_records(report) if item.get("displayName") == display_name]
    if len(matches) != 1:
        raise EnumLoweringFailure(
            f"expected one enum record for {display_name!r}, found {len(matches)}"
        )
    return matches[0]


def case_by_name(record: dict[str, object], name: str) -> dict[str, object]:
    cases = record.get("cases")
    if not isinstance(cases, list) or not all(isinstance(item, dict) for item in cases):
        raise EnumLoweringFailure("enum case records are malformed")
    matches = [item for item in cases if item.get("semanticName") == name]
    if len(matches) != 1:
        raise EnumLoweringFailure(f"expected one constructor {name!r}")
    return matches[0]


def payload_by_name(record: dict[str, object], name: str) -> dict[str, object]:
    payload = record.get("payload")
    if not isinstance(payload, list) or not all(isinstance(item, dict) for item in payload):
        raise EnumLoweringFailure("enum payload records are malformed")
    matches = [item for item in payload if item.get("semanticName") == name]
    if len(matches) != 1:
        raise EnumLoweringFailure(f"expected one payload field {name!r}")
    return matches[0]


def required_identifier(record: dict[str, object], key: str) -> str:
    value = record.get(key)
    if not isinstance(value, str) or not value:
        raise EnumLoweringFailure(f"enum record omitted finalized C identifier {key!r}")
    return value


def enum_names(report: dict[str, object]) -> dict[str, str]:
    mode = enum_by_name(report, "Mode")
    option_int = enum_by_name(report, "Option<i32>")
    option_bool = enum_by_name(report, "Option<bool>")
    chain = enum_by_name(report, "Chain<i32>")
    mode_on = case_by_name(mode, "On")
    option_int_some = case_by_name(option_int, "Some")
    option_bool_some = case_by_name(option_bool, "Some")
    chain_end = case_by_name(chain, "End")
    chain_link = case_by_name(chain, "Link")
    values = {
        "mode_tag": mode.get("valueTag"),
        "mode_on": mode_on.get("discriminant"),
        "option_int_tag": option_int.get("valueTag"),
        "option_int_tag_member": option_int.get("tagMember"),
        "option_int_payload_member": option_int.get("payloadMember"),
        "option_int_some": option_int_some.get("discriminant"),
        "option_int_some_union": option_int_some.get("unionMember"),
        "option_int_some_value": payload_by_name(option_int_some, "value").get("cName"),
        "option_bool_tag": option_bool.get("valueTag"),
        "option_bool_tag_member": option_bool.get("tagMember"),
        "option_bool_payload_member": option_bool.get("payloadMember"),
        "option_bool_some": option_bool_some.get("discriminant"),
        "option_bool_some_union": option_bool_some.get("unionMember"),
        "option_bool_some_value": payload_by_name(option_bool_some, "value").get("cName"),
        "chain_tag": chain.get("valueTag"),
        "chain_tag_member": chain.get("tagMember"),
        "chain_payload_member": chain.get("payloadMember"),
        "chain_end": chain_end.get("discriminant"),
        "chain_end_union": chain_end.get("unionMember"),
        "chain_end_value": payload_by_name(chain_end, "value").get("cName"),
        "chain_link": chain_link.get("discriminant"),
        "chain_link_union": chain_link.get("unionMember"),
        "chain_link_value": payload_by_name(chain_link, "value").get("cName"),
        "chain_link_next": payload_by_name(chain_link, "next").get("cName"),
    }
    if not all(isinstance(value, str) and value for value in values.values()):
        raise EnumLoweringFailure("enum report omitted native-consumer identifiers")
    return {key: str(value) for key, value in values.items()}


def function_section(hxcir: str, field: str) -> str:
    start_marker = f'  function "function.EnumFixture.{field}"'
    end_marker = f'  end function "function.EnumFixture.{field}"'
    start = hxcir.find(start_marker)
    end = hxcir.find(end_marker, start)
    if start == -1 or end == -1:
        raise EnumLoweringFailure(f"HxcIR omitted function {field}")
    return hxcir[start : end + len(end_marker)]


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "haxe-enums-owned-managed-values"
        or report.get("profile") != profile
        or report.get("runtimeFeatures") != RUNTIME_FEATURES
        or report.get("runtimeArtifacts") != RUNTIME_ARTIFACTS
    ):
        raise EnumLoweringFailure(f"enum report contract drifted for {profile}")
    records = enum_records(report)
    if len(records) != 7:
        raise EnumLoweringFailure(f"expected seven concrete enum instances, found {len(records)}")
    names = enum_names(report)
    mode = enum_by_name(report, "Mode")
    option_int = enum_by_name(report, "Option<i32>")
    option_bool = enum_by_name(report, "Option<bool>")
    option_rule_candidates = [
        record
        for record in records
        if str(record.get("displayName", "")).startswith("Option<closed-record(")
    ]
    if len(option_rule_candidates) != 1:
        raise EnumLoweringFailure("expected one generic Option specialization for Rule")
    option_rule = option_rule_candidates[0]
    chain = enum_by_name(report, "Chain<i32>")
    rule_envelope = enum_by_name(report, "RuleEnvelope")
    if (
        mode.get("representation") != "native-enum"
        or mode.get("recursive") is not False
        or mode.get("scopedLifetime") is not False
        or option_int.get("representation") != "tagged-union"
        or option_bool.get("representation") != "tagged-union"
        or option_rule.get("representation") != "tagged-union"
        or chain.get("representation") != "tagged-union"
        or chain.get("recursive") is not True
        or chain.get("scopedLifetime") is not True
        or rule_envelope.get("representation") != "tagged-union"
        or rule_envelope.get("recursive") is not False
        or rule_envelope.get("scopedLifetime") is not False
    ):
        raise EnumLoweringFailure("enum representation or recursion policy drifted")
    link_next = payload_by_name(case_by_name(chain, "Link"), "next")
    if link_next.get("indirect") is not True or not str(link_next.get("type", "")).startswith(
        "pointer:nonnull<instance:"
    ):
        raise EnumLoweringFailure("recursive Chain edge is not an explicit non-null pointer")
    wrapped_rule = payload_by_name(case_by_name(rule_envelope, "WrappedRule"), "rule")
    if wrapped_rule.get("indirect") is not False or not str(
        wrapped_rule.get("type", "")
    ).startswith("instance:instance.closed-record."):
        raise EnumLoweringFailure("managed closed record is not a direct typed enum payload")
    for record in records:
        cases = record.get("cases")
        if not isinstance(cases, list) or [case.get("tagValue") for case in cases] != list(
            range(len(cases))
        ):
            raise EnumLoweringFailure("enum discriminants lost Haxe source order")

    hxcir = required_text(report, "hxcir")
    header = required_text(report, "header")
    sources = source_records(report)
    if set(sources) != {"src/program.c"}:
        raise EnumLoweringFailure(f"enum source partition drifted: {sorted(sources)!r}")
    source = sources["src/program.c"]
    for label, value in (("HxcIR", hxcir), ("header", header), ("source", source)):
        if str(ROOT) in value or "\\" in value:
            raise EnumLoweringFailure(f"{label} leaked a host path")
    if not hxcir.startswith("hxcir schema=19\n") or hxcir.count(" representation=tagged ") != 6:
        raise EnumLoweringFailure("schema-19 tagged-union HxcIR inventory drifted")
    option_section = function_section(hxcir, "optionValue")
    recursive_section = function_section(hxcir, "recursiveLocal")
    main_section = function_section(hxcir, "main")
    mode_equality_section = function_section(hxcir, "modeEquality")
    wrap_rule_section = function_section(hxcir, "wrapRule")
    envelope_value_section = function_section(hxcir, "envelopeValue")
    apply_option_section = function_section(hxcir, "applyOption")
    constructor_value_section = function_section(hxcir, "constructorValue")
    option_int_instance = required_identifier(option_int, "instanceId")
    identity_call = main_section.find(
        'call dispatch=direct("function.EnumFixture.identity")'
    )
    identity_result_constructor = main_section.find(
        f'construct-tag instance="{option_int_instance}"', identity_call
    )
    if (
        "terminator tag-switch" not in option_section
        or "default=none" not in option_section
        or 'project-tag value=' not in option_section
        or f'check=checked-abort(profile="{profile}",build="debug")'
        not in option_section
        or "enum-recursive-payload-allocate" not in recursive_section
        or "enum-recursive-payload-load" not in recursive_section
        or 'binary operation="haxe.enum-tag.equal"' not in mode_equality_section
        or 'binary operation="haxe.enum-tag.not-equal"' not in mode_equality_section
        or identity_call == -1
        or identity_result_constructor == -1
        or identity_call > identity_result_constructor
        or 'implementation=program-local("aggregate-lifecycle:' not in wrap_rule_section
        or "terminator tag-switch" not in envelope_value_section
        or 'project-tag value=' not in envelope_value_section
        or "dispatch=closure(" not in apply_option_section
        or "enum-constructor-function-reference" not in constructor_value_section
        or "function.enum-constructor-adapter." not in constructor_value_section
        or 'instruction "instruction.0.construct-enum-adapter"' not in hxcir
    ):
        raise EnumLoweringFailure("enum construction, checking, switch, or recursive HxcIR drifted")
    if (
        f"enum {names['mode_tag']} {{" not in header
        or f"struct {names['option_int_tag']} {{" not in header
        or f"union {required_identifier(option_int, 'payloadUnionTag')} {{" not in header
        or f"struct {names['chain_tag']} *" not in header
        or "#include <stddef.h>" not in header
        or "#include <stdlib.h>" not in header
    ):
        raise EnumLoweringFailure("private native/tagged enum header drifted")
    if (
        source.count("_Static_assert(") < 20
        or "offsetof(" not in source
        or "switch (" not in source
        or "abort();" not in source
        or f"(struct {names['option_int_tag']}){{" not in source
        or " == " not in source
        or " != " not in source
        or "int main(void)" not in source
        or "hxc_record_" not in source
        or "_retain(void *" not in source
        or "_destroy(void *" not in source
    ):
        raise EnumLoweringFailure("structural enum CAST emission or checks drifted")
    record_retain_start = source.find("hxc_status hxc_record_")
    record_destroy_start = source.find("\nvoid hxc_record_", record_retain_start)
    recursive_clone_start = source.find("_retain_recursive_clone(void *")
    recursive_destroy_start = source.find("\nvoid ", recursive_clone_start)
    if (
        record_retain_start == -1
        or record_destroy_start == -1
        or recursive_clone_start == -1
        or recursive_destroy_start == -1
    ):
        raise EnumLoweringFailure("managed lifecycle helper boundaries disappeared")
    record_retain = source[record_retain_start:record_destroy_start]
    recursive_clone = source[recursive_clone_start:recursive_destroy_start]
    if (
        record_retain.count("_retain(") < 3
        or record_retain.count("hxc_array_ref_release(") < 2
        or "_destroy(&" not in record_retain
        or record_retain.count("return hxc_operation_status;") < 3
        or "hxc_free(" not in recursive_clone
    ):
        raise EnumLoweringFailure(
            "managed record or recursive clone lost failure rollback before ownership transfer"
        )
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v2":
        raise EnumLoweringFailure("enum report omitted its finalized symbol table")


def normalized_profile(report: dict[str, object]) -> dict[str, object]:
    value = copy.deepcopy(report)
    value["profile"] = "<profile>"
    value["hxcir"] = str(value["hxcir"]).replace(
        'checked-abort(profile="metal",', 'checked-abort(profile="portable",'
    )
    return value


def snapshot_values(report: dict[str, object]) -> dict[str, object]:
    return {
        "enums.hxcir": required_text(report, "hxcir"),
        "program.h": required_text(report, "header"),
        "program.c": source_records(report)["src/program.c"],
        "enums.json": enum_records(report),
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
            raise EnumLoweringFailure(
                "enum snapshots are missing; run "
                "npm run snapshots:update -- --suite enum-lowering"
            )
        if name.endswith(".json"):
            expected = json.loads(path.read_text(encoding="utf-8"))
            if actual != expected:
                raise EnumLoweringFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise EnumLoweringFailure(f"{name} snapshot is not text")
            expected = path.read_text(encoding="utf-8")
            if actual != expected:
                raise EnumLoweringFailure(
                    f"{name} drifted:\n" + difference(expected, actual, name)
                )


def macro_definitions(report: dict[str, object]) -> tuple[str, ...]:
    names = enum_names(report)
    pairs = (
        ("HXC_MODE_TAG", "mode_tag"),
        ("HXC_MODE_ON", "mode_on"),
        ("HXC_OPTION_INT_TAG", "option_int_tag"),
        ("HXC_OPTION_INT_TAG_MEMBER", "option_int_tag_member"),
        ("HXC_OPTION_INT_PAYLOAD_MEMBER", "option_int_payload_member"),
        ("HXC_OPTION_INT_SOME", "option_int_some"),
        ("HXC_OPTION_INT_SOME_UNION", "option_int_some_union"),
        ("HXC_OPTION_INT_SOME_VALUE", "option_int_some_value"),
        ("HXC_OPTION_BOOL_TAG", "option_bool_tag"),
        ("HXC_OPTION_BOOL_TAG_MEMBER", "option_bool_tag_member"),
        ("HXC_OPTION_BOOL_PAYLOAD_MEMBER", "option_bool_payload_member"),
        ("HXC_OPTION_BOOL_SOME", "option_bool_some"),
        ("HXC_OPTION_BOOL_SOME_UNION", "option_bool_some_union"),
        ("HXC_OPTION_BOOL_SOME_VALUE", "option_bool_some_value"),
        ("HXC_CHAIN_TAG", "chain_tag"),
        ("HXC_CHAIN_TAG_MEMBER", "chain_tag_member"),
        ("HXC_CHAIN_PAYLOAD_MEMBER", "chain_payload_member"),
        ("HXC_CHAIN_END", "chain_end"),
        ("HXC_CHAIN_END_UNION", "chain_end_union"),
        ("HXC_CHAIN_END_VALUE", "chain_end_value"),
        ("HXC_CHAIN_LINK", "chain_link"),
        ("HXC_CHAIN_LINK_UNION", "chain_link_union"),
        ("HXC_CHAIN_LINK_VALUE", "chain_link_value"),
        ("HXC_CHAIN_LINK_NEXT", "chain_link_next"),
    )
    return tuple(f"-D{macro}={names[key]}" for macro, key in pairs)


def write_native_fixture(report: dict[str, object], root: Path) -> None:
    header = root / "include/hxc/program.h"
    source = root / "src/program.c"
    native = root / "native/layout_consumer.c"
    header.parent.mkdir(parents=True)
    source.parent.mkdir(parents=True)
    native.parent.mkdir(parents=True)
    header.write_text(required_text(report, "header"), encoding="utf-8", newline="\n")
    source.write_text(source_records(report)["src/program.c"], encoding="utf-8", newline="\n")
    shutil.copyfile(NATIVE / "layout_consumer.c", native)
    for relative in RUNTIME_ARTIFACTS:
        destination = root / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        source_path = ROOT / relative.replace(
            "runtime/include", "runtime/hxrt/include"
        ).replace("runtime/src", "runtime/hxrt/src")
        shutil.copyfile(source_path, destination)


def run_harness_matrix(
    report: dict[str, object], root: Path, *, requested_toolchain: str
) -> list[dict[str, object]]:
    fixture_root = root / "fixture"
    write_native_fixture(report, fixture_root)
    projects = (
        CFixtureProject(
            "enum-layout",
            ("native/layout_consumer.c",),
            ("include/hxc/program.h",),
            ("include", "runtime/include"),
            "",
            (
                "c-consumer",
                "fieldless-native-enum",
                "tagged-union",
                "recursive-finite-layout",
            ),
        ),
        CFixtureProject(
            "generated-program",
            ("src/program.c", "runtime/src/allocator.c", "runtime/src/array.c"),
            ("include/hxc/program.h",),
            ("include", "runtime/include"),
            "",
            (
                "checked-payload-projection",
                "exhaustive-tag-switch",
                "fieldless-enum-equality",
                "generated-executable",
                "managed-record-lifecycle",
                "recursive-owned-enum",
            ),
        ),
    )
    reports: list[dict[str, object]] = []
    for optimization in ("-O0", "-O2"):
        report_value = run_c_fixture_corpus(
            suite=f"enum-lowering-{optimization[1:].lower()}",
            projects=projects,
            fixture_root=fixture_root,
            build_root=root / f"c-build-{optimization[1:].lower()}",
            repository_root=ROOT,
            requested_toolchain=requested_toolchain,
            strict_flags=(*C11_STRICT_FLAGS, optimization, *macro_definitions(report)),
        )
        validate_report(report_value, required_coverage=REQUIRED_COVERAGE)
        encoded = report_json(report_value, compact=True)
        for forbidden in (str(ROOT), str(fixture_root), str(root)):
            if forbidden in encoded:
                raise EnumLoweringFailure(
                    f"native harness report leaked absolute path {forbidden}"
                )
        reports.append(report_value)
    return reports


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
        raise EnumLoweringFailure(f"cannot identify C++ compiler {executable}")
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
        raise EnumLoweringFailure(
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
                raise EnumLoweringFailure(
                    f"required {toolchain.family} C++17 companion {cxx_name!r} is unavailable"
                )
            print(
                f"enum-lowering: SKIP optional {toolchain.family} C++17 consumer: "
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
                    f"-I{root / 'fixture/runtime/include'}",
                    "-c",
                    str(NATIVE / "layout_provider.c"),
                    "-o",
                    str(provider_object),
                ],
                label=f"{toolchain.family} {optimization} C enum layout provider",
            )
            require_silent_success(
                [
                    cxx,
                    *CXX_STRICT_FLAGS,
                    optimization,
                    *definitions,
                    f"-I{root / 'fixture/include'}",
                    f"-I{root / 'fixture/runtime/include'}",
                    "-c",
                    str(NATIVE / "layout_consumer.cpp"),
                    "-o",
                    str(consumer_object),
                ],
                label=f"{toolchain.family} {optimization} C++17 enum layout consumer",
            )
            require_silent_success(
                [cxx, str(provider_object), str(consumer_object), "-o", str(executable)],
                label=f"{toolchain.family} {optimization} C/C++17 enum layout link",
            )
            require_silent_success(
                [str(executable)],
                label=f"{toolchain.family} {optimization} C/C++17 enum layout run",
                cwd=build,
            )


def check_native(
    report: dict[str, object], *, requested_toolchain: str = "auto"
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-enum-native-") as temporary:
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
    layout: str = "unity",
    reverse: bool = False,
    connect: str | None = None,
    report: bool = False,
) -> subprocess.CompletedProcess[str]:
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend([
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        main,
        "-D",
        "hxc_runtime_diagnostics=off",
    ])
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_static_initialization_report"])
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    command.extend(["-D", f"hxc_project_layout={layout}", "--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(server=connect is not None),
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
        raise EnumLoweringFailure(
            f"{label} failed\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def extract_static_hxcir(
    result: subprocess.CompletedProcess[str], label: str
) -> str:
    """Read the one explicit compiler report while rejecting other output."""
    reports = [
        line[len(STATIC_REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(STATIC_REPORT_PREFIX)
    ]
    other_stdout = [
        line
        for line in result.stdout.splitlines()
        if not line.startswith(STATIC_REPORT_PREFIX)
    ]
    if result.returncode != 0 or result.stderr or other_stdout or len(reports) != 1:
        raise EnumLoweringFailure(
            f"{label} emitted an invalid HxcIR report\n"
            f"exit={result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    report = json.loads(reports[0])
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise EnumLoweringFailure(f"{label} omitted validated HxcIR text")
    return hxcir


def validate_production(root: Path, *, layout: str, profile: str, policy: str) -> None:
    if generated_files(root) != PRODUCTION_FILES_BY_LAYOUT[layout]:
        raise EnumLoweringFailure(
            f"{layout}/{profile}/{policy} production artifact set drifted: "
            f"{sorted(generated_files(root))!r}"
        )
    manifest = json.loads((root / "hxc.manifest.json").read_text(encoding="utf-8"))
    runtime_plan = json.loads((root / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    specializations = json.loads(
        (root / "hxc.specializations.json").read_text(encoding="utf-8")
    )
    specialization_summary = specializations.get("summary")
    if (
        manifest.get("compilationStatus") != "lowered-direct-value-executable"
        or manifest.get("configuration", {}).get("profile") != profile
        or runtime_plan.get("schemaVersion") != 2
        or runtime_plan.get("status") != "analyzed-runtime-features"
        or runtime_plan.get("profile") != profile
        or runtime_plan.get("resolvedPolicy") != policy
        or runtime_plan.get("features") != RUNTIME_FEATURES
        or runtime_plan.get("artifacts") != RUNTIME_ARTIFACTS
        or "bounded-haxe-enum-values" not in runtime_plan.get("directDecisions", [])
        or "closed-generic-specializations"
        not in runtime_plan.get("directDecisions", [])
        or "closed-anonymous-value-records" not in runtime_plan.get("directDecisions", [])
        or "managed-haxe-arrays" not in runtime_plan.get("directDecisions", [])
        or specializations.get("schemaVersion") != 1
        or specializations.get("algorithm") != "hxc-generic-specialization-v1"
        or specializations.get("status") != "analyzed-closed-specializations"
        or specializations.get("functionSpecializations") != []
        or not isinstance(specializations.get("typeSpecializations"), list)
        or len(specializations["typeSpecializations"]) != 4
        or not isinstance(specialization_summary, dict)
        or specialization_summary.get("functionSpecializations") != 0
        or specialization_summary.get("typeSpecializations") != 4
        or runtime_plan.get("noRuntimeProof") is not None
        or not isinstance(runtime_plan.get("rootReasons"), list)
        or not any(
            isinstance(reason, dict)
            and reason.get("featureId") == "alloc"
            and reason.get("operationId") == "allocation"
            for reason in runtime_plan["rootReasons"]
        )
    ):
        raise EnumLoweringFailure(
            f"{layout}/{profile}/{policy} lost its owned enum/record runtime proof"
        )
    combined = b"\n".join(
        path.read_bytes()
        for path in root.rglob("*")
        if path.is_file() and path.suffix in {".c", ".h"}
    ).lower()
    for marker in (b"hxc_alloc", b"hxc_array_ref", b"hxc_record_"):
        if marker not in combined:
            raise EnumLoweringFailure(f"enum production project omitted {marker!r}")


def check_production_server_determinism(root: Path) -> None:
    """Prove cold, first-server, and warm-server outputs are byte-identical."""
    port = available_port()
    endpoint = str(port)
    server = subprocess.Popen(
        [development_tool("haxe"), "--wait", endpoint],
        cwd=ROOT,
        env=haxe_environment(server=True),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_server(server, port)
        for name in ("server-first", "server-warm"):
            output = root / name
            result = custom_target(
                POSITIVE,
                output,
                main="EnumFixture",
                connect=endpoint,
            )
            require_compile_success(result, f"{name} enum production compile")
            validate_production(
                output, layout="unity", profile="portable", policy="auto"
            )
            if generated_tree(output) != generated_tree(root / "first"):
                raise EnumLoweringFailure(
                    f"{name} enum artifacts differed from the cold custom-target build"
                )
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def production_project(root: Path, *, layout: str) -> CFixtureProject:
    """Describe one emitted layout without assuming which files contain its code."""
    sources = tuple(path.relative_to(root).as_posix() for path in sorted(root.rglob("*.c")))
    headers = tuple(path.relative_to(root).as_posix() for path in sorted(root.rglob("*.h")))
    return CFixtureProject(
        f"production-enum-program-{layout}",
        sources,
        headers,
        ("include", "runtime/include"),
        "",
        (
            "generated-executable",
            f"{layout}-layout",
            "managed-record-lifecycle",
            "managed-record-enum-payload",
            "recursive-owned-enum",
        ),
    )


def check_production_native(
    root: Path, *, layout: str, requested_toolchain: str
) -> None:
    project = production_project(root, layout=layout)
    required = frozenset(project.coverage)
    report = run_c_fixture_corpus(
        suite=f"enum-production-{layout}-o0",
        projects=(project,),
        fixture_root=root,
        build_root=root.parent / "production-layout-build" / layout,
        repository_root=ROOT,
        requested_toolchain=requested_toolchain,
        strict_flags=(*C11_STRICT_FLAGS, "-O0"),
    )
    validate_report(report, required_coverage=required)


def check_production_sanitizers(root: Path, *, requested_toolchain: str) -> None:
    sanitizer_flags = (
        *C11_STRICT_FLAGS,
        "-O1",
        "-g",
        "-fno-omit-frame-pointer",
        "-fno-sanitize-recover=all",
        "-fsanitize=address,undefined",
    )
    base = production_project(root, layout="unity")
    project = CFixtureProject(
        "production-enum-program-sanitized",
        base.sources,
        base.headers,
        base.include_directories,
        base.expected_stdout,
        (*base.coverage, "asan-ubsan"),
        link_arguments=("-fsanitize=address,undefined",),
    )
    report = run_c_fixture_corpus(
        suite="enum-production-sanitized",
        projects=(project,),
        fixture_root=root,
        build_root=root.parent / "production-sanitized-build",
        repository_root=ROOT,
        requested_toolchain=requested_toolchain,
        strict_flags=sanitizer_flags,
    )
    validate_report(report, required_coverage=frozenset(project.coverage))


def check_production(*, requested_toolchain: str) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-enum-production-") as temporary:
        root = Path(temporary)
        matrix = (
            ("first", "unity", "portable", None, "auto", False),
            ("repeat", "unity", "portable", None, "auto", False),
            ("reverse", "unity", "portable", None, "auto", True),
            ("metal", "unity", "metal", None, "minimal", False),
            ("split", "split", "portable", None, "auto", False),
            ("package", "package", "portable", None, "auto", False),
        )
        for name, layout, profile, runtime, policy, reverse in matrix:
            output = root / name
            result = custom_target(
                POSITIVE,
                output,
                main="EnumFixture",
                profile=profile,
                runtime=runtime,
                layout=layout,
                reverse=reverse,
            )
            require_compile_success(result, f"{name} enum production compile")
            validate_production(output, layout=layout, profile=profile, policy=policy)
        if generated_tree(root / "first") != generated_tree(root / "repeat"):
            raise EnumLoweringFailure("two enum production roots were not byte-identical")
        if generated_tree(root / "first") != generated_tree(root / "reverse"):
            raise EnumLoweringFailure(
                "reversed typed-module discovery changed enum production artifacts"
            )
        check_production_server_determinism(root)
        for relative in ("include/hxc/program.h", "src/program.c"):
            if (root / "first" / relative).read_bytes() != (
                root / "metal" / relative
            ).read_bytes():
                raise EnumLoweringFailure(
                    f"portable and metal changed direct enum artifact {relative}"
                )
        project = production_project(root / "first", layout="unity")
        for optimization in ("-O0", "-O2"):
            native_report = run_c_fixture_corpus(
                suite=f"enum-production-{optimization[1:].lower()}",
                projects=(project,),
                fixture_root=root / "first",
                build_root=root / f"production-build-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            validate_report(
                native_report,
                required_coverage=frozenset(
                    project.coverage
                ),
            )
        for layout in ("split", "package"):
            check_production_native(
                root / layout,
                layout=layout,
                requested_toolchain=requested_toolchain,
            )
        check_production_sanitizers(
            root / "first", requested_toolchain=requested_toolchain
        )


def check_negative_cases() -> None:
    cases = {
        "payload_equality": ("HXC1001", "payload-enum-equality-requires-structural-semantics:PayloadValue"),
        "nonexhaustive": ("Unmatched patterns: On", "fixtures/nonexhaustive/Main.hx:"),
        "recursive_managed_class": (
            "HXC1001",
            "recursive-enum-with-collector-payload:ManagedChain",
        ),
        "recursive_conditional": (
            "HXC1001",
            "TIf(result-type:haxe-enum:",
        ),
        "unsupported_reference_payload": (
            "HXC1001",
            "UnsupportedValue.values:reference-policy-not-admitted:haxe-string-map-reference:",
        ),
    }
    with tempfile.TemporaryDirectory(prefix="hxc-enum-negative-") as temporary:
        root = Path(temporary)
        for directory, markers in cases.items():
            output = root / directory
            result = custom_target(FIXTURES / directory, output, main="Main")
            combined = (result.stdout + result.stderr).replace("\\", "/")
            if (
                result.returncode != 1
                or any(marker not in combined for marker in markers)
                or f"fixtures/{directory}/Main.hx:" not in combined
                or generated_files(output)
            ):
                raise EnumLoweringFailure(
                    f"{directory} did not fail closed at its exact source with no output\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )


def check_string_payload(*, requested_toolchain: str) -> None:
    """Prove that nominal literal-backed String values survive enum storage."""
    fixture = FIXTURES / "string_payload"
    with tempfile.TemporaryDirectory(prefix="hxc-enum-string-payload-") as temporary:
        root = Path(temporary)
        for layout in ("unity", "split", "package"):
            output = root / layout
            result = custom_target(fixture, output, main="Main", layout=layout)
            require_compile_success(result, f"{layout} String-payload enum compile")
            runtime_plan = json.loads(
                (output / "hxc.runtime-plan.json").read_text(encoding="utf-8")
            )
            if runtime_plan.get("features") != ["runtime-base", "string-literal"]:
                raise EnumLoweringFailure(
                    f"{layout} String payload selected more than the direct literal carrier"
                )
            reasons = runtime_plan.get("rootReasons")
            if (
                not isinstance(reasons, list)
                or not reasons
                or any(
                    not isinstance(reason, dict)
                    or reason.get("featureId") != "string-literal"
                    or reason.get("operationId") != "static-value"
                    or reason.get("kind") != "direct-string-value"
                    for reason in reasons
                )
            ):
                raise EnumLoweringFailure(
                    f"{layout} String payload lost its typed literal-storage reasons"
                )
            generated_c = b"\n".join(
                path.read_bytes() for path in sorted(output.rglob("*.c"))
            )
            for marker in (b"hxc_string", b"memcmp", b"caf"):
                if marker not in generated_c.lower():
                    raise EnumLoweringFailure(
                        f"{layout} String-payload C omitted {marker!r}"
                    )
            if b"malloc" in generated_c or b"hxc_string_copy" in generated_c:
                raise EnumLoweringFailure(
                    f"{layout} static String payload unexpectedly allocated or copied bytes"
                )
            sources = tuple(
                path.relative_to(output).as_posix()
                for path in sorted(output.rglob("*.c"))
            )
            headers = tuple(
                path.relative_to(output).as_posix()
                for path in sorted(output.rglob("*.h"))
            )
            project = CFixtureProject(
                f"enum-string-payload-{layout}",
                sources,
                headers,
                ("include", "runtime/include"),
                "",
                (
                    "generated-executable",
                    "literal-backed-string-value",
                    "tagged-union",
                ),
            )
            report = run_c_fixture_corpus(
                suite=f"enum-string-payload-{layout}",
                projects=(project,),
                fixture_root=output,
                build_root=root / f"native-{layout}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=C11_STRICT_FLAGS,
            )
            validate_report(
                report,
                required_coverage=frozenset(
                    {
                        "generated-executable",
                        "literal-backed-string-value",
                        "tagged-union",
                    }
                ),
            )


def check_managed_class_payload(*, requested_toolchain: str) -> None:
    """Prove exact GC roots and traces for class references inside value enums."""
    fixture = FIXTURES / "managed_class_payload"
    eval_result = subprocess.run(
        [development_tool("haxe"), "-cp", str(fixture), "-main", "Main", "--interp"],
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    require_compile_success(eval_result, "managed-class enum Eval oracle")
    with tempfile.TemporaryDirectory(prefix="hxc-enum-managed-class-") as temporary:
        root = Path(temporary)
        outputs: dict[str, Path] = {}
        for name, layout, reverse in (
            ("unity", "unity", False),
            ("repeat", "unity", False),
            ("reverse", "unity", True),
            ("split", "split", False),
            ("package", "package", False),
        ):
            output = root / name
            result = custom_target(
                fixture, output, main="Main", layout=layout, reverse=reverse
            )
            require_compile_success(result, f"{name} managed-class enum compile")
            outputs[name] = output
        if generated_tree(outputs["unity"]) != generated_tree(outputs["repeat"]):
            raise EnumLoweringFailure(
                "managed-class enum changed across repeated cold compiles"
            )
        if generated_tree(outputs["unity"]) != generated_tree(outputs["reverse"]):
            raise EnumLoweringFailure(
                "managed-class enum changed with typed-module discovery order"
            )

        port = available_port()
        endpoint = str(port)
        server = subprocess.Popen(
            [development_tool("haxe"), "--wait", endpoint],
            cwd=ROOT,
            env=haxe_environment(server=True),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        try:
            wait_for_server(server, port)
            for name in ("server-first", "server-warm"):
                output = root / name
                result = custom_target(
                    fixture, output, main="Main", connect=endpoint
                )
                require_compile_success(
                    result, f"{name} managed-class enum compile"
                )
                if generated_tree(output) != generated_tree(outputs["unity"]):
                    raise EnumLoweringFailure(
                        f"{name} managed-class enum output differed from cold output"
                    )
        finally:
            server.terminate()
            try:
                server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                server.kill()
                server.wait(timeout=5)

        plan = json.loads(
            (outputs["unity"] / "hxc.runtime-plan.json").read_text(encoding="utf-8")
        )
        if plan.get("features") != [
            "runtime-base",
            "status",
            "alloc",
            "array",
            "object",
            "gc",
        ] or "exact-traced-haxe-object-graph" not in plan.get("directDecisions", []):
            raise EnumLoweringFailure(
                "managed-class enum lost its exact collector/runtime plan"
            )
        generated_c = b"\n".join(
            path.read_bytes() for path in sorted(outputs["unity"].rglob("*.c"))
        )
        for marker in (
            b"hxc_SessionEvent",
            b"hxc_gc_root_frame_push",
            b"hxc_SessionEvent_Opened",
            b"hxc_array_",
            b"_trace_visit",
        ):
            if marker not in generated_c:
                raise EnumLoweringFailure(
                    f"managed-class enum generated C omitted {marker!r}"
                )

        def project(output: Path, label: str) -> CFixtureProject:
            return CFixtureProject(
                label,
                tuple(
                    path.relative_to(output).as_posix()
                    for path in sorted(output.rglob("*.c"))
                ),
                tuple(
                    path.relative_to(output).as_posix()
                    for path in sorted(output.rglob("*.h"))
                ),
                ("include", "runtime/include"),
                "",
                (
                    "enum-managed-class-payload",
                    "exact-root-projection",
                    "recursive-container-trace",
                    "generated-executable",
                ),
            )

        unity_project = project(outputs["unity"], "enum-managed-class-unity")
        for optimization in ("-O0", "-O2"):
            report = run_c_fixture_corpus(
                suite=f"enum-managed-class-{optimization[1:].lower()}",
                projects=(unity_project,),
                fixture_root=outputs["unity"],
                build_root=root / f"native-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            validate_report(report, required_coverage=frozenset(unity_project.coverage))
        sanitizer_project = CFixtureProject(
            "enum-managed-class-sanitized",
            unity_project.sources,
            unity_project.headers,
            unity_project.include_directories,
            "",
            (*unity_project.coverage, "asan-ubsan"),
            link_arguments=("-fsanitize=address,undefined",),
        )
        sanitizer_report = run_c_fixture_corpus(
            suite="enum-managed-class-sanitized",
            projects=(sanitizer_project,),
            fixture_root=outputs["unity"],
            build_root=root / "native-sanitized",
            repository_root=ROOT,
            requested_toolchain=requested_toolchain,
            strict_flags=(
                *C11_STRICT_FLAGS,
                "-O1",
                "-g",
                "-fno-omit-frame-pointer",
                "-fno-sanitize-recover=all",
                "-fsanitize=address,undefined",
            ),
        )
        validate_report(
            sanitizer_report, required_coverage=frozenset(sanitizer_project.coverage)
        )

        for layout in ("split", "package"):
            output = outputs[layout]
            layout_project = project(output, f"enum-managed-class-{layout}")
            report = run_c_fixture_corpus(
                suite=f"enum-managed-class-{layout}",
                projects=(layout_project,),
                fixture_root=output,
                build_root=root / f"native-{layout}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=C11_STRICT_FLAGS,
            )
            validate_report(report, required_coverage=frozenset(layout_project.coverage))

        toolchain = resolve_toolchains(
            requested_toolchain, repository_root=ROOT
        )[0]
        cxx = shutil.which(CXX_COMMANDS[toolchain.family])
        if cxx is not None:
            consumer = root / "private-layout-consumer.cpp"
            consumer.write_text(
                '#include "hxc/program.h"\nint enum_layout_consumer() { return 0; }\n',
                encoding="utf-8",
            )
            require_silent_success(
                [
                    cxx,
                    *CXX_STRICT_FLAGS,
                    "-I",
                    str(outputs["split"] / "include"),
                    "-I",
                    str(outputs["split"] / "runtime/include"),
                    "-c",
                    str(consumer),
                    "-o",
                    str(root / "private-layout-consumer.o"),
                ],
                label="managed-class enum C++17 private-header consumer",
            )


def check_bytes_payload(*, requested_toolchain: str) -> None:
    """Prove shared Bytes identity and active-case cleanup inside a value enum."""
    fixture = FIXTURES / "bytes_payload"
    eval_result = subprocess.run(
        [development_tool("haxe"), "-cp", str(fixture), "-main", "Main", "--interp"],
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    require_compile_success(eval_result, "Bytes-payload enum Eval oracle")

    with tempfile.TemporaryDirectory(prefix="hxc-enum-bytes-payload-") as temporary:
        root = Path(temporary)
        outputs: dict[str, Path] = {}
        hxcir = ""
        for name, layout, reverse, report in (
            ("unity", "unity", False, True),
            ("repeat", "unity", False, False),
            ("reverse", "unity", True, False),
            ("split", "split", False, False),
            ("package", "package", False, False),
        ):
            output = root / name
            result = custom_target(
                fixture,
                output,
                main="Main",
                layout=layout,
                reverse=reverse,
                report=report,
            )
            if report:
                hxcir = extract_static_hxcir(
                    result, f"{name} Bytes-payload enum compile"
                )
            else:
                require_compile_success(
                    result, f"{name} Bytes-payload enum compile"
                )
            outputs[name] = output

        if generated_tree(outputs["unity"]) != generated_tree(outputs["repeat"]):
            raise EnumLoweringFailure(
                "Bytes-payload enum changed across repeated cold compiles"
            )
        if generated_tree(outputs["unity"]) != generated_tree(outputs["reverse"]):
            raise EnumLoweringFailure(
                "Bytes-payload enum changed with typed-module discovery order"
            )

        port = available_port()
        endpoint = str(port)
        server = subprocess.Popen(
            [development_tool("haxe"), "--wait", endpoint],
            cwd=ROOT,
            env=haxe_environment(server=True),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        try:
            wait_for_server(server, port)
            for name in ("server-first", "server-warm"):
                output = root / name
                result = custom_target(
                    fixture, output, main="Main", connect=endpoint
                )
                require_compile_success(
                    result, f"{name} Bytes-payload enum compile"
                )
                if generated_tree(output) != generated_tree(outputs["unity"]):
                    raise EnumLoweringFailure(
                        f"{name} Bytes-payload enum output differed from cold output"
                    )
        finally:
            server.terminate()
            try:
                server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                server.kill()
                server.wait(timeout=5)

        for marker in (
            'name="ValidationResult" kind=tagged-union',
            'representation=managed("bytes")',
            "retain-enum-return",
            "enum-payload-project",
            'implementation=runtime("bytes")',
            'runtime(feature="bytes",operation="get")',
            'runtime(feature="bytes",operation="set")',
            "declare-managed-carrier",
            "ownership=move-fresh",
            "ownership=retain-borrowed(program-local(",
            "move-managed-carrier",
        ):
            if marker not in hxcir:
                raise EnumLoweringFailure(
                    f"Bytes-payload enum HxcIR omitted {marker!r}"
                )
        if " raw" in hxcir or str(ROOT) in hxcir:
            raise EnumLoweringFailure(
                "Bytes-payload enum HxcIR used raw syntax or leaked the checkout path"
            )

        for name, output in outputs.items():
            plan = json.loads(
                (output / "hxc.runtime-plan.json").read_text(encoding="utf-8")
            )
            operations = {
                reason.get("operationId")
                for reason in plan.get("rootReasons", [])
                if isinstance(reason, dict) and reason.get("featureId") == "bytes"
            }
            if (
                plan.get("features") != BYTES_RUNTIME_FEATURES
                or plan.get("artifacts") != BYTES_RUNTIME_ARTIFACTS
                or "bounded-haxe-enum-values"
                not in plan.get("directDecisions", [])
                or "managed-haxe-bytes" not in plan.get("directDecisions", [])
                or operations
                != {
                    "alloc",
                    "cleanup-release",
                    "get",
                    "managed-type-representation",
                    "retain",
                    "set",
                }
            ):
                raise EnumLoweringFailure(
                    f"{name} Bytes-payload enum selected the wrong runtime/lifetime plan"
                )

        generated_c = "\n".join(
            path.read_text(encoding="utf-8")
            for path in sorted(outputs["unity"].rglob("*.c"))
            if "runtime/" not in path.relative_to(outputs["unity"]).as_posix()
        )
        retain_start = generated_c.find("hxc_status hxc_enum_")
        destroy_start = generated_c.find("\nvoid hxc_enum_", retain_start)
        destroy_end = generated_c.find("\nstruct hxc_ValidationResult ", destroy_start)
        if retain_start == -1 or destroy_start == -1 or destroy_end == -1:
            raise EnumLoweringFailure(
                "Bytes-payload enum lifecycle helper boundaries disappeared"
            )
        retain_helper = generated_c[retain_start:destroy_start]
        destroy_helper = generated_c[destroy_start:destroy_end]
        failed_case = "case hxc_ValidationResult_ValidationFailed:"
        passed_case = "case hxc_ValidationResult_ValidationPassed:"
        for label, helper, operation in (
            ("retain", retain_helper, "hxc_bytes_ref_retain"),
            ("destroy", destroy_helper, "hxc_bytes_ref_release"),
        ):
            failed_start = helper.find(failed_case)
            passed_start = helper.find(passed_case)
            if (
                failed_start == -1
                or passed_start == -1
                or failed_start > passed_start
                or "hxc_bytes_ref_" in helper[failed_start:passed_start]
                or operation not in helper[passed_start:]
            ):
                raise EnumLoweringFailure(
                    f"Bytes-payload enum {label} helper touched an inactive union case"
                )
        for marker in (
            "struct hxc_ValidationResult",
            "hxc_bytes_ref_create_zeroed",
            "hxc_bytes_ref_get",
            "hxc_bytes_ref_set",
            "hxc_enum_",
        ):
            if marker not in generated_c:
                raise EnumLoweringFailure(
                    f"Bytes-payload generated C omitted {marker!r}"
                )
        if "goto " in generated_c:
            raise EnumLoweringFailure(
                "Bytes-payload enum unexpectedly required goto-based C lowering"
            )

        def project(output: Path, label: str) -> CFixtureProject:
            return CFixtureProject(
                label,
                tuple(
                    path.relative_to(output).as_posix()
                    for path in sorted(output.rglob("*.c"))
                ),
                tuple(
                    path.relative_to(output).as_posix()
                    for path in sorted(output.rglob("*.h"))
                ),
                ("include", "runtime/include"),
                "",
                (
                    "enum-managed-bytes-payload",
                    "active-tag-lifecycle",
                    "shared-mutable-identity",
                    "generated-executable",
                ),
            )

        unity_project = project(outputs["unity"], "enum-bytes-payload-unity")
        for optimization in ("-O0", "-O2"):
            report = run_c_fixture_corpus(
                suite=f"enum-bytes-payload-{optimization[1:].lower()}",
                projects=(unity_project,),
                fixture_root=outputs["unity"],
                build_root=root / f"native-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            validate_report(
                report, required_coverage=frozenset(unity_project.coverage)
            )

        sanitizer_project = CFixtureProject(
            "enum-bytes-payload-sanitized",
            unity_project.sources,
            unity_project.headers,
            unity_project.include_directories,
            "",
            (*unity_project.coverage, "asan-ubsan"),
            link_arguments=("-fsanitize=address,undefined",),
        )
        sanitizer_report = run_c_fixture_corpus(
            suite="enum-bytes-payload-sanitized",
            projects=(sanitizer_project,),
            fixture_root=outputs["unity"],
            build_root=root / "native-sanitized",
            repository_root=ROOT,
            requested_toolchain=requested_toolchain,
            strict_flags=(
                *C11_STRICT_FLAGS,
                "-O1",
                "-g",
                "-fno-omit-frame-pointer",
                "-fno-sanitize-recover=all",
                "-fsanitize=address,undefined",
            ),
        )
        validate_report(
            sanitizer_report,
            required_coverage=frozenset(sanitizer_project.coverage),
        )

        for layout in ("split", "package"):
            output = outputs[layout]
            layout_project = project(output, f"enum-bytes-payload-{layout}")
            report = run_c_fixture_corpus(
                suite=f"enum-bytes-payload-{layout}",
                projects=(layout_project,),
                fixture_root=output,
                build_root=root / f"native-{layout}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=C11_STRICT_FLAGS,
            )
            validate_report(
                report, required_coverage=frozenset(layout_project.coverage)
            )


def snapshot_report() -> dict[str, object]:
    return {
        "schemaVersion": 1,
        "status": "haxe-enums-owned-managed-values",
        "profile": "portable",
        "hxcir": (EXPECTED / "enums.hxcir").read_text(encoding="utf-8"),
        "header": (EXPECTED / "program.h").read_text(encoding="utf-8"),
        "sources": [
            {
                "path": "src/program.c",
                "content": (EXPECTED / "program.c").read_text(encoding="utf-8"),
            }
        ],
        "enums": json.loads((EXPECTED / "enums.json").read_text(encoding="utf-8")),
        "symbols": json.loads((EXPECTED / "symbols.json").read_text(encoding="utf-8")),
        "runtimeFeatures": RUNTIME_FEATURES,
        "runtimeArtifacts": RUNTIME_ARTIFACTS,
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument("--native-only", action="store_true")
    parser.add_argument("--bytes-payload-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("enum-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            report = snapshot_report()
            validate(report)
            check_native(report, requested_toolchain=args.toolchain)
            print("enum-lowering: OK: required enum native matrix passed")
            return 0
        if args.bytes_payload_only:
            check_bytes_payload(requested_toolchain=args.toolchain)
            check_negative_cases()
            print(
                "enum-lowering: OK: Bytes payload ownership, active-tag cleanup, "
                "determinism, native safety, and unsupported-reference rejection passed"
            )
            return 0

        first_payload, first = render("first enum render")
        second_payload, second = render("second enum render")
        reverse_payload, reverse = render("reverse-input enum render", reverse=True)
        metal_payload, metal = render("metal enum render", profile="metal")
        if first_payload != second_payload or first != second:
            raise EnumLoweringFailure("two enum renders differed")
        if first_payload != reverse_payload or first != reverse:
            raise EnumLoweringFailure("enum render changed with discovery order")
        validate(first)
        validate(metal, profile="metal")
        if normalized_profile(first) != normalized_profile(metal):
            raise EnumLoweringFailure("portable and metal enum lowering diverged")
        check_snapshots(first)
        check_native(first, requested_toolchain=args.toolchain)
        check_production(requested_toolchain=args.toolchain)
        check_string_payload(requested_toolchain=args.toolchain)
        check_managed_class_payload(requested_toolchain=args.toolchain)
        check_bytes_payload(requested_toolchain=args.toolchain)
        check_negative_cases()
    except (
        EnumLoweringFailure,
        CFixtureFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"enum-lowering: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "enum-lowering: OK: native and tagged representations, concrete generic "
        "specialization, checked exhaustive matches, finite recursive layout, strict "
        "C11 and C++17 agreement, owned recursive records, cold/warm-server and "
        "split/package/unity determinism, ASan/UBSan, literal-backed String payloads, "
        "exact managed-class enum roots/traces, managed Bytes payload ownership "
        "across layouts, and fail-closed edges passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

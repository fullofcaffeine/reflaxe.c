#!/usr/bin/env python3
"""Prove closed anonymous-record lowering, layout, copies, and fail-closed edges."""

from __future__ import annotations

import argparse
import copy
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
from collections.abc import Iterable
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("aggregate_lowering.hxml")
FIXTURES = Path(__file__).with_name("fixtures")
POSITIVE = FIXTURES / "positive"
NATIVE = Path(__file__).with_name("native")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_AGGREGATE_LOWERING="
CONSTRUCTOR_REPORT_PREFIX = "HXC_CONSTRUCTOR_LOWERING="
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
PRODUCTION_FILES_BY_LAYOUT = {
    "split": COMMON_PRODUCTION_FILES
    | {
        "include/hxc/detail/program_types.h",
        "include/hxc/modules/AggregateFixture.h",
        "include/hxc/program.h",
        "src/hxc/main.c",
        "src/hxc/support.c",
        "src/modules/AggregateFixture.c",
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
TYPE_HEADER_BY_LAYOUT = {
    "split": "include/hxc/modules/AggregateFixture.h",
    "package": "include/hxc/packages/package.h",
    "unity": "include/hxc/program.h",
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
REQUIRED_COVERAGE = frozenset(
    {
        "address-taking",
        "aggregate-construction",
        "by-value-copy",
        "c-consumer",
        "generated-executable",
        "nested-layout",
        "runtime-free",
        "structural-deduplication",
    }
)


class AggregateLoweringFailure(RuntimeError):
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


def render(
    label: str,
    *,
    reverse: bool = False,
    profile: str = "portable",
    connect: str | None = None,
) -> tuple[str, dict[str, object]]:
    if profile not in ("portable", "metal"):
        raise AggregateLoweringFailure(f"unknown aggregate profile {profile!r}")
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.append(str(HXML))
    if reverse:
        command.extend(["-D", "aggregate_lowering_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "aggregate_lowering_profile=metal"])
    result = subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(server=connect is not None),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if result.returncode != 0 or result.stderr or len(lines) != 1:
        raise AggregateLoweringFailure(
            f"{label} emitted an invalid report\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise AggregateLoweringFailure(f"{label} report is not an object")
    return payload, report


def available_port() -> int:
    """Reserve an unused loopback port for this test's isolated Haxe server."""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise AggregateLoweringFailure(
                f"Haxe server exited early\nstdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise AggregateLoweringFailure("Haxe server did not accept connections")


def required_text(report: dict[str, object], key: str) -> str:
    value = report.get(key)
    if not isinstance(value, str):
        raise AggregateLoweringFailure(f"aggregate report field {key!r} must be text")
    return value


def source_records(report: dict[str, object]) -> dict[str, str]:
    records = report.get("sources")
    if not isinstance(records, list):
        raise AggregateLoweringFailure("aggregate report omitted source records")
    result: dict[str, str] = {}
    ordered: list[str] = []
    for record in records:
        if (
            not isinstance(record, dict)
            or not isinstance(record.get("path"), str)
            or not isinstance(record.get("content"), str)
        ):
            raise AggregateLoweringFailure(f"malformed aggregate source record: {record!r}")
        path = record["path"]
        if (
            path in result
            or path.startswith("/")
            or "\\" in path
            or any(part in ("", ".", "..") for part in path.split("/"))
        ):
            raise AggregateLoweringFailure(f"unsafe or duplicate source path {path!r}")
        ordered.append(path)
        result[path] = record["content"]
    if ordered != sorted(ordered, key=lambda value: value.encode("utf-8")):
        raise AggregateLoweringFailure("aggregate source records are not UTF-8 ordered")
    return result


def aggregate_records(report: dict[str, object]) -> list[dict[str, object]]:
    value = report.get("aggregates")
    if not isinstance(value, list) or not all(isinstance(item, dict) for item in value):
        raise AggregateLoweringFailure("aggregate report omitted structural records")
    return value


def aggregate_names(report: dict[str, object]) -> dict[str, str]:
    records = aggregate_records(report)
    if len(records) != 7:
        raise AggregateLoweringFailure(
            "OrderA/OrderB did not deduplicate beside the envelope, optional envelope, flow, conditional, enum-abstract, and ordinary-enum records"
        )
    by_fields = {
        tuple(field.get("semanticName") for field in record.get("fields", [])): record
        for record in records
    }
    pair = by_fields.get(("a", "z"))
    envelope = by_fields.get(("enabled", "point"))
    optional_envelope = by_fields.get(("point",))
    flow_record = by_fields.get(("first", "order", "second", "third"))
    conditional_record = by_fields.get(("order", "value"))
    switch_record = by_fields.get(("state",))
    actor_record = by_fields.get(("phase",))
    if not all(
        isinstance(record, dict)
        for record in (
            pair,
            envelope,
            optional_envelope,
            flow_record,
            conditional_record,
            switch_record,
            actor_record,
        )
    ):
        raise AggregateLoweringFailure("aggregate records lost their five closed structural shapes")
    assert isinstance(pair, dict)
    assert isinstance(envelope, dict)
    assert isinstance(optional_envelope, dict)
    assert isinstance(flow_record, dict)
    assert isinstance(conditional_record, dict)
    assert isinstance(switch_record, dict)
    assert isinstance(actor_record, dict)
    pair_fields = pair.get("fields")
    envelope_fields = envelope.get("fields")
    optional_envelope_fields = optional_envelope.get("fields")
    flow_fields = flow_record.get("fields")
    conditional_fields = conditional_record.get("fields")
    switch_fields = switch_record.get("fields")
    actor_fields = actor_record.get("fields")
    if (
        not isinstance(pair_fields, list)
        or not all(isinstance(field, dict) for field in pair_fields)
        or not isinstance(envelope_fields, list)
        or not all(isinstance(field, dict) for field in envelope_fields)
        or not isinstance(flow_fields, list)
        or not all(isinstance(field, dict) for field in flow_fields)
        or not isinstance(conditional_fields, list)
        or not all(isinstance(field, dict) for field in conditional_fields)
        or not isinstance(switch_fields, list)
        or not all(isinstance(field, dict) for field in switch_fields)
        or not isinstance(actor_fields, list)
        or not all(isinstance(field, dict) for field in actor_fields)
        or not isinstance(optional_envelope_fields, list)
        or not all(isinstance(field, dict) for field in optional_envelope_fields)
    ):
        raise AggregateLoweringFailure("aggregate field records are malformed")
    if [field.get("semanticName") for field in pair_fields] != ["a", "z"]:
        raise AggregateLoweringFailure("pair fields are not in canonical UTF-8 order")
    if [field.get("type") for field in pair_fields] != ["i32", "i32"]:
        raise AggregateLoweringFailure("pair field types drifted")
    if [field.get("semanticName") for field in envelope_fields] != [
        "enabled",
        "point",
    ]:
        raise AggregateLoweringFailure("nested fields are not in canonical UTF-8 order")
    if (
        [field.get("semanticName") for field in flow_fields]
        != ["first", "order", "second", "third"]
        or [field.get("type") for field in flow_fields] != ["i32"] * 4
    ):
        raise AggregateLoweringFailure(
            "flow record fields lost their canonical direct-Int shape"
        )
    if (
        [field.get("semanticName") for field in conditional_fields]
        != ["order", "value"]
        or conditional_fields[0].get("type") != "i32"
        or conditional_fields[1].get("type") != f"instance:{pair.get('instanceId')}"
    ):
        raise AggregateLoweringFailure(
            "conditional record lost its order marker or selected direct value"
        )
    if (
        [field.get("semanticName") for field in optional_envelope_fields] != ["point"]
        or len(optional_envelope_fields) != 1
        or not str(optional_envelope_fields[0].get("type", "")).startswith("optional<instance:instance.closed-record.")
    ):
        raise AggregateLoweringFailure("optional envelope lost its tagged nullable-record field")
    if (
        [field.get("semanticName") for field in switch_fields] != ["state"]
        or [field.get("type") for field in switch_fields] != ["i32"]
    ):
        raise AggregateLoweringFailure("enum-abstract field did not retain its direct Int representation")
    if (
        [field.get("semanticName") for field in actor_fields] != ["phase"]
        or len(actor_fields) != 1
        or not isinstance(actor_fields[0].get("type"), str)
        or not str(actor_fields[0]["type"]).startswith("instance:instance.enum.")
    ):
        raise AggregateLoweringFailure("ordinary enum field did not retain its nominal HxcIR instance")
    actor_cases = actor_fields[0].get("enumCases")
    if not isinstance(actor_cases, list) or not all(isinstance(case, dict) for case in actor_cases):
        raise AggregateLoweringFailure("ordinary enum field omitted its finalized cases")
    cases_by_name = {case.get("semanticName"): case for case in actor_cases}
    waiting = cases_by_name.get("Waiting")
    moving = cases_by_name.get("Moving")
    if (
        not isinstance(waiting, dict)
        or not isinstance(moving, dict)
        or waiting.get("tagValue") != 0
        or moving.get("tagValue") != 1
        or len(cases_by_name) != 2
    ):
        raise AggregateLoweringFailure("ordinary enum cases lost their Haxe discriminants")
    moving_payload = moving.get("payload")
    if (
        waiting.get("payload") != []
        or not isinstance(moving_payload, list)
        or len(moving_payload) != 1
        or not isinstance(moving_payload[0], dict)
        or moving_payload[0].get("semanticName") != "speed"
        or moving_payload[0].get("type") != "i32"
    ):
        raise AggregateLoweringFailure("payload enum record field lost its typed Moving speed payload")
    pair_instance = pair.get("instanceId")
    if (
        not isinstance(pair_instance, str)
        or envelope_fields[0].get("type") != "bool"
        or envelope_fields[1].get("type") != f"instance:{pair_instance}"
    ):
        raise AggregateLoweringFailure("nested field did not retain the deduplicated pair instance")
    values = {
        "pair_tag": pair.get("cTag"),
        "pair_a": pair_fields[0].get("cName"),
        "pair_z": pair_fields[1].get("cName"),
        "envelope_tag": envelope.get("cTag"),
        "envelope_enabled": envelope_fields[0].get("cName"),
        "envelope_point": envelope_fields[1].get("cName"),
        "flow_tag": flow_record.get("cTag"),
        "flow_first": flow_fields[0].get("cName"),
        "flow_order": flow_fields[1].get("cName"),
        "flow_second": flow_fields[2].get("cName"),
        "flow_third": flow_fields[3].get("cName"),
        "switch_tag": switch_record.get("cTag"),
        "switch_state": switch_fields[0].get("cName"),
        "actor_tag": actor_record.get("cTag"),
        "actor_phase": actor_fields[0].get("cName"),
        "actor_phase_tag": actor_fields[0].get("enumTag"),
        "actor_phase_discriminant_tag": actor_fields[0].get("enumDiscriminantTag"),
        "actor_phase_payload_union_tag": actor_fields[0].get("enumPayloadUnionTag"),
        "actor_phase_tag_member": actor_fields[0].get("enumTagMember"),
        "actor_phase_payload_member": actor_fields[0].get("enumPayloadMember"),
        "actor_phase_waiting": waiting.get("cName"),
        "actor_phase_moving": moving.get("cName"),
        "actor_phase_moving_payload_tag": moving.get("payloadStructTag"),
        "actor_phase_moving_union_member": moving.get("unionMember"),
        "actor_phase_moving_speed": moving_payload[0].get("cName"),
    }
    if not all(isinstance(value, str) and value for value in values.values()):
        raise AggregateLoweringFailure("aggregate report omitted finalized C identifiers")
    return {key: str(value) for key, value in values.items()}


def function_section(hxcir: str, field: str) -> str:
    return named_function_section(hxcir, "AggregateFixture", field)


def named_function_section(hxcir: str, module: str, field: str) -> str:
    """Return one complete function so checks cannot pass on a neighboring body."""
    start_marker = f'  function "function.{module}.{field}"'
    end_marker = f'  end function "function.{module}.{field}"'
    start = hxcir.find(start_marker)
    end = hxcir.find(end_marker, start)
    if start == -1 or end == -1:
        raise AggregateLoweringFailure(f"HxcIR omitted function {module}.{field}")
    return hxcir[start : end + len(end_marker)]


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "closed-anonymous-records-direct-runtime-free"
        or report.get("profile") != profile
        or report.get("runtimeFeatures") != []
        or report.get("runtimeArtifacts") != []
    ):
        raise AggregateLoweringFailure(f"aggregate report contract drifted for {profile}")

    names = aggregate_names(report)
    hxcir = required_text(report, "hxcir")
    header = required_text(report, "header")
    sources = source_records(report)
    if set(sources) != {"src/program.c"}:
        raise AggregateLoweringFailure(f"aggregate source partition drifted: {sorted(sources)!r}")
    source = sources["src/program.c"]
    for label, value in (("HxcIR", hxcir), ("header", header), ("source", source)):
        if str(ROOT) in value or "\\" in value or "hxrt" in value.lower():
            raise AggregateLoweringFailure(f"{label} leaked a host path or runtime dependency")

    records = aggregate_records(report)
    pair_record = next(
        record
        for record in records
        if [field.get("semanticName") for field in record.get("fields", [])]
        == ["a", "z"]
    )
    envelope_record = next(
        record
        for record in records
        if [field.get("semanticName") for field in record.get("fields", [])]
        == ["enabled", "point"]
    )
    pair_type = f'type "type.closed-record.{pair_record["digest"]}"'
    envelope_type = f'type "type.closed-record.{envelope_record["digest"]}"'
    if hxcir.find(pair_type) == -1 or hxcir.find(pair_type) > hxcir.find(envelope_type):
        raise AggregateLoweringFailure("nested HxcIR declarations are not dependency-first")
    if (
        hxcir.count(" representation=direct ") != 7
        or hxcir.count(" representation=tagged ") != 1
    ):
        raise AggregateLoweringFailure(
            "closed records or their bounded tagged enum lost direct by-value representation"
        )

    make = function_section(hxcir, "make")
    calls = [make.find('call dispatch=direct("function.AggregateFixture.identity")')]
    calls.append(make.find('call dispatch=direct("function.AggregateFixture.identity")', calls[0] + 1))
    construction = make.find('fields=["a"="value.1","z"="value.0"]')
    if any(index == -1 for index in calls) or calls != sorted(calls) or calls[1] > construction:
        raise AggregateLoweringFailure(
            "object-literal evaluation order or canonical named construction drifted"
        )
    local_sum = function_section(hxcir, "localSum")
    load_a = local_sum.find('load place=field(local("local.2"),"a")')
    load_z = local_sum.find('load place=field(local("local.2"),"z")')
    if (
        min(load_a, load_z) < 0
        or load_a > load_z
        or "record-field-address" in local_sum
        or "load place=dereference" in local_sum
    ):
        raise AggregateLoweringFailure(
            "compiler-owned local field reads lost their direct ordered places"
        )
    copy_section = function_section(hxcir, "copy")
    if (
        "initialize place=local" not in copy_section
        or "load place=local" not in copy_section
        or copy_section.find("initialize place=local") > copy_section.find("load place=local")
    ):
        raise AggregateLoweringFailure("by-value record copy is not explicit in HxcIR")
    sum_section = function_section(hxcir, "sum")
    if sum_section.count('.record-field-project"') != 2:
        raise AggregateLoweringFailure("parameter field reads did not use value projection")
    switch_section = function_section(hxcir, "switchIsOn")
    if (
        switch_section.count('.record-field-project"') != 1
        or ':i32 project value="parameter.0" field="state"' not in switch_section
        or "equal" not in switch_section
    ):
        raise AggregateLoweringFailure(
            "enum-abstract record projection lost its direct scalar comparison"
        )
    actor_section = function_section(hxcir, "actorSpeed")
    if (
        actor_section.count('.record-field-project"') != 1
        or ':instance("instance.enum.' not in actor_section
        or 'terminator tag-switch value=' not in actor_section
        or 'tag="Moving" payload-index=0' not in actor_section
    ):
        raise AggregateLoweringFailure(
            "payload enum record projection lost its exhaustive tag switch or checked payload read"
        )
    flow_section = function_section(hxcir, "makeFlowRecord")
    flow_markers = [
        flow_section.find(".record-field-first-initialize"),
        flow_section.find(".record-field-second-initialize"),
        flow_section.find(".record-field-first-load"),
        flow_section.find(".record-field-second-load"),
        flow_section.find(".construct-record"),
    ]
    if (
        min(flow_markers) < 0
        or flow_markers != sorted(flow_markers)
        or 'fields=["first"=' not in flow_section
        or ',"order"=' not in flow_section
        or ',"second"=' not in flow_section
        or ',"third"=' not in flow_section
    ):
        raise AggregateLoweringFailure(
            "record fields were not preserved and reloaded around later control flow"
        )
    select_record = function_section(hxcir, "selectRecord")
    select_phase = function_section(hxcir, "selectPhase")
    for label, section in (
        ("record", select_record),
        ("unmanaged payload enum", select_phase),
    ):
        if (
            section.count("declare-uninitialized place=local") != 1
            or section.count("conditional-true-store") != 1
            or section.count("conditional-false-store") != 1
            or section.count("conditional-load") != 1
            or "conditional-default" in section
        ):
            raise AggregateLoweringFailure(
                f"{label} conditional lost its branch-assigned direct-value carrier"
            )

    pair_definition = f"struct {names['pair_tag']} {{"
    envelope_definition = f"struct {names['envelope_tag']} {{"
    flow_definition = f"struct {names['flow_tag']} {{"
    switch_definition = f"struct {names['switch_tag']} {{"
    actor_enum_definition = f"enum {names['actor_phase_discriminant_tag']} {{"
    actor_payload_definition = f"struct {names['actor_phase_moving_payload_tag']} {{"
    actor_union_definition = f"union {names['actor_phase_payload_union_tag']} {{"
    actor_value_definition = f"struct {names['actor_phase_tag']} {{"
    actor_definition = f"struct {names['actor_tag']} {{"
    if (
        header.find(pair_definition) == -1
        or header.find(envelope_definition) == -1
        or header.find(pair_definition) > header.find(envelope_definition)
        or f"int32_t {names['pair_a']};" not in header
        or f"int32_t {names['pair_z']};" not in header
        or f"bool {names['envelope_enabled']};" not in header
        or f"struct {names['pair_tag']} {names['envelope_point']};" not in header
        or header.find(flow_definition) == -1
        or f"int32_t {names['flow_first']};" not in header
        or f"int32_t {names['flow_order']};" not in header
        or f"int32_t {names['flow_second']};" not in header
        or f"int32_t {names['flow_third']};" not in header
        or header.find(switch_definition) == -1
        or f"int32_t {names['switch_state']};" not in header
        or header.find(actor_enum_definition) == -1
        or header.find(actor_payload_definition) == -1
        or header.find(actor_union_definition) == -1
        or header.find(actor_value_definition) == -1
        or header.find(actor_definition) == -1
        or not (
            header.find(actor_enum_definition)
            < header.find(actor_payload_definition)
            < header.find(actor_union_definition)
            < header.find(actor_value_definition)
            < header.find(actor_definition)
        )
        or f"{names['actor_phase_waiting']} = 0" not in header
        or f"{names['actor_phase_moving']} = 1" not in header
        or f"int32_t {names['actor_phase_moving_speed']};" not in header
        or f"struct {names['actor_phase_moving_payload_tag']} {names['actor_phase_moving_union_member']};" not in header
        or f"enum {names['actor_phase_discriminant_tag']} {names['actor_phase_tag_member']};" not in header
        or f"union {names['actor_phase_payload_union_tag']} {names['actor_phase_payload_member']};" not in header
        or f"struct {names['actor_phase_tag']} {names['actor_phase']};" not in header
        or "#include <stddef.h>" not in header
    ):
        raise AggregateLoweringFailure("private dependency-first struct header drifted")
    if (
        source.count("_Static_assert(") != 41
        or source.count("offsetof(") < 20
        or f"(struct {names['pair_tag']}){{" not in source
        or f"(struct {names['envelope_tag']}){{" not in source
        or f"(struct {names['flow_tag']}){{" not in source
        or f"(struct {names['switch_tag']}){{" not in source
        or f"(struct {names['actor_tag']}){{" not in source
        or "struct hxc_OrderA hxc_tmp_conditional_result_n5;" not in source
        or "struct hxc_ActorPhase hxc_tmp_conditional_result_n2;" not in source
        or "hxc_tmp_conditional_result_n5 = hxc_first;" not in source
        or "hxc_tmp_conditional_result_n5 = hxc_second;" not in source
        or ".hxc_tag = hxc_ActorPhase_Moving" not in source
        or ".hxc_tag = hxc_ActorPhase_Waiting" not in source
        or "(struct hxc_optional_OrderA){ .hxc_has_value = false }" not in source
        or "(struct hxc_optional_OrderA){ .hxc_has_value = true, .hxc_value = " not in source
        or ".hxc_has_value" not in source
        or ".hxc_value" not in source
        or "int main(void)" not in source
    ):
        raise AggregateLoweringFailure("structural CAST construction/layout assertions drifted")
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v2":
        raise AggregateLoweringFailure("aggregate report omitted its finalized symbol table")


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
        "aggregates.hxcir": required_text(report, "hxcir"),
        "program.h": required_text(report, "header"),
        "program.c": source_records(report)["src/program.c"],
        "aggregates.json": aggregate_records(report),
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
            raise AggregateLoweringFailure(
                "aggregate snapshots are missing; run "
                "npm run snapshots:update -- --suite aggregate-lowering"
            )
        if name.endswith(".json"):
            expected = json.loads(path.read_text(encoding="utf-8"))
            if actual != expected:
                raise AggregateLoweringFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise AggregateLoweringFailure(f"{name} snapshot is not text")
            expected = path.read_text(encoding="utf-8")
            if actual != expected:
                raise AggregateLoweringFailure(
                    f"{name} drifted:\n" + difference(expected, actual, name)
                )


def macro_definitions(report: dict[str, object]) -> tuple[str, ...]:
    names = aggregate_names(report)
    return tuple(
        f"-D{macro}={names[key]}"
        for macro, key in (
            ("HXC_PAIR_TAG", "pair_tag"),
            ("HXC_PAIR_A", "pair_a"),
            ("HXC_PAIR_Z", "pair_z"),
            ("HXC_ENVELOPE_TAG", "envelope_tag"),
            ("HXC_ENVELOPE_ENABLED", "envelope_enabled"),
            ("HXC_ENVELOPE_POINT", "envelope_point"),
            ("HXC_ACTOR_TAG", "actor_tag"),
            ("HXC_ACTOR_PHASE", "actor_phase"),
            ("HXC_ACTOR_PHASE_VALUE_TAG", "actor_phase_tag"),
            ("HXC_ACTOR_PHASE_DISCRIMINANT_TAG", "actor_phase_discriminant_tag"),
            ("HXC_ACTOR_PHASE_PAYLOAD_UNION_TAG", "actor_phase_payload_union_tag"),
            ("HXC_ACTOR_PHASE_TAG_MEMBER", "actor_phase_tag_member"),
            ("HXC_ACTOR_PHASE_PAYLOAD_MEMBER", "actor_phase_payload_member"),
            ("HXC_ACTOR_PHASE_WAITING", "actor_phase_waiting"),
            ("HXC_ACTOR_PHASE_MOVING", "actor_phase_moving"),
            ("HXC_ACTOR_PHASE_MOVING_PAYLOAD_TAG", "actor_phase_moving_payload_tag"),
            ("HXC_ACTOR_PHASE_MOVING_UNION_MEMBER", "actor_phase_moving_union_member"),
            ("HXC_ACTOR_PHASE_MOVING_SPEED", "actor_phase_moving_speed"),
        )
    )


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


def run_harness_matrix(
    report: dict[str, object], root: Path, *, requested_toolchain: str
) -> list[dict[str, object]]:
    fixture_root = root / "fixture"
    write_native_fixture(report, fixture_root)
    projects = (
        CFixtureProject(
            "aggregate-layout",
            ("native/layout_consumer.c",),
            ("include/hxc/program.h",),
            ("include",),
            "",
            (
                "address-taking",
                "aggregate-construction",
                "by-value-copy",
                "c-consumer",
                "nested-layout",
                "structural-deduplication",
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
    )
    reports: list[dict[str, object]] = []
    for optimization in ("-O0", "-O2"):
        report_value = run_c_fixture_corpus(
            suite=f"aggregate-lowering-{optimization[1:].lower()}",
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
                raise AggregateLoweringFailure(
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
        raise AggregateLoweringFailure(f"cannot identify C++ compiler {executable}")
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
        raise AggregateLoweringFailure(
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
                raise AggregateLoweringFailure(
                    f"required {toolchain.family} C++17 companion {cxx_name!r} is unavailable"
                )
            print(
                f"aggregate-lowering: SKIP optional {toolchain.family} C++17 consumer: "
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
                label=f"{toolchain.family} {optimization} C layout provider",
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
                label=f"{toolchain.family} {optimization} C++17 layout consumer",
            )
            require_silent_success(
                [cxx, str(provider_object), str(consumer_object), "-o", str(executable)],
                label=f"{toolchain.family} {optimization} C/C++17 layout link",
            )
            require_silent_success(
                [str(executable)],
                label=f"{toolchain.family} {optimization} C/C++17 layout run",
                cwd=build,
            )


def check_managed_optional_cpp(
    output: Path, build_root: Path, *, requested_toolchain: str
) -> None:
    """Compile the real generated optional header as strict C++17."""
    for toolchain in resolve_toolchains(requested_toolchain, repository_root=ROOT):
        cxx_name = CXX_COMMANDS[toolchain.family]
        cxx = shutil.which(cxx_name)
        if cxx is None or compiler_identity(cxx) != toolchain.family:
            if requested_toolchain != "auto":
                raise AggregateLoweringFailure(
                    f"required {toolchain.family} C++17 companion {cxx_name!r} is unavailable"
                )
            print(
                "aggregate-lowering: SKIP optional "
                f"{toolchain.family} managed-optional C++17 consumer"
            )
            continue
        for optimization in ("-O0", "-O2"):
            destination = (
                build_root
                / toolchain.family
                / optimization[1:].lower()
                / "managed_optional_consumer.o"
            )
            destination.parent.mkdir(parents=True, exist_ok=True)
            require_silent_success(
                [
                    cxx,
                    *CXX_STRICT_FLAGS,
                    optimization,
                    f"-I{output / 'include'}",
                    f"-I{output / 'runtime/include'}",
                    "-c",
                    str(NATIVE / "managed_optional_consumer.cpp"),
                    "-o",
                    str(destination),
                ],
                label=(
                    f"{toolchain.family} {optimization} managed-optional "
                    "generated-header consumer"
                ),
            )


def check_native(
    report: dict[str, object], *, requested_toolchain: str = "auto"
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-aggregate-native-") as temporary:
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
    lifecycle_probe: bool = False,
    layout: str = "unity",
    connect: str | None = None,
    hxcir_report: bool = False,
    reverse: bool = False,
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
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    if lifecycle_probe:
        command.extend(["-D", "reflaxe_c_lifecycle_probe"])
    if hxcir_report:
        command.extend(["-D", "reflaxe_c_constructor_lowering_report"])
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
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
        raise AggregateLoweringFailure(
            f"{label} failed\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def reported_hxcir(result: subprocess.CompletedProcess[str], label: str) -> str:
    """Read the semantic tree emitted by a real custom-target compilation."""
    lines = result.stdout.splitlines()
    reports = [
        line[len(CONSTRUCTOR_REPORT_PREFIX) :]
        for line in lines
        if line.startswith(CONSTRUCTOR_REPORT_PREFIX)
    ]
    other_output = [line for line in lines if not line.startswith(CONSTRUCTOR_REPORT_PREFIX)]
    if result.returncode != 0 or result.stderr or other_output or len(reports) != 1:
        raise AggregateLoweringFailure(
            f"{label} did not emit exactly one clean HxcIR report\n"
            f"exit={result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    report = json.loads(reports[0])
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise AggregateLoweringFailure(f"{label} omitted its HxcIR semantic tree")
    return hxcir


def validate_production(root: Path, *, layout: str, profile: str, policy: str) -> None:
    expected_files = PRODUCTION_FILES_BY_LAYOUT[layout]
    if generated_files(root) != expected_files:
        raise AggregateLoweringFailure(
            f"{layout}/{profile}/{policy} production artifact set drifted: "
            f"{sorted(generated_files(root))!r}"
        )
    manifest = json.loads((root / "hxc.manifest.json").read_text(encoding="utf-8"))
    runtime_plan = json.loads((root / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    proof = runtime_plan.get("noRuntimeProof")
    reachability = proof.get("reachability") if isinstance(proof, dict) else None
    if (
        manifest.get("compilationStatus") != "lowered-direct-value-executable"
        or manifest.get("configuration", {}).get("profile") != profile
        or runtime_plan.get("schemaVersion") != 2
        or runtime_plan.get("status") != "analyzed-runtime-free"
        or runtime_plan.get("profile") != profile
        or runtime_plan.get("resolvedPolicy") != policy
        or runtime_plan.get("features") != []
        or runtime_plan.get("artifacts") != []
        or "closed-anonymous-value-records" not in runtime_plan.get("directDecisions", [])
        or "bounded-haxe-enum-values" not in runtime_plan.get("directDecisions", [])
        or not isinstance(proof, dict)
        or proof.get("status") != "eligible"
        or proof.get("directDecisions") != runtime_plan.get("directDecisions")
        or reachability is None
        or reachability.get("typeInstances") != 8
        or reachability.get("runtimeIntents") != 0
    ):
        raise AggregateLoweringFailure(
            f"{layout}/{profile}/{policy} lost its runtime-free aggregate proof"
        )
    combined = b"\n".join(
        path.read_bytes()
        for path in root.rglob("*")
        if path.is_file() and path.suffix in {".c", ".h"}
    ).lower()
    for forbidden in (
        rb"\bhxrt_",
        rb"\bhxc_runtime\b",
        rb"\bgoto\b",
        rb"\bmalloc\s*\(",
        rb"\bcalloc\s*\(",
        rb"\brealloc\s*\(",
        rb"\bfree\s*\(",
    ):
        if re.search(forbidden, combined):
            raise AggregateLoweringFailure(
                f"aggregate production project contains forbidden C pattern {forbidden!r}"
            )

    type_header = (root / TYPE_HEADER_BY_LAYOUT[layout]).read_text(encoding="utf-8")
    enum_definition = type_header.find("enum hxc_ActorPhase_tag {")
    enum_value_definition = type_header.find("struct hxc_ActorPhase {")
    record_definition = type_header.find("struct hxc_ActorRecord {")
    if (
        enum_definition == -1
        or enum_value_definition == -1
        or record_definition == -1
        or not enum_definition < enum_value_definition < record_definition
        or "struct hxc_ActorPhase hxc_phase;" not in type_header
    ):
        raise AggregateLoweringFailure(
            f"{layout} did not define the tagged enum before its by-value record field"
        )


def check_production_native(
    root: Path, *, layout: str, requested_toolchain: str
) -> None:
    """Compile and run each generated layout under the repository's strict C11 flags."""
    sources = tuple(
        path.relative_to(root).as_posix()
        for path in sorted(root.rglob("*.c"))
    )
    headers = tuple(
        path.relative_to(root).as_posix()
        for path in sorted(root.rglob("*.h"))
    )
    project = CFixtureProject(
        f"aggregate-enum-record-{layout}",
        sources,
        headers,
        ("include",),
        "",
        (f"{layout}-layout", "enum-record", "generated-executable", "runtime-free"),
    )
    report = run_c_fixture_corpus(
        suite=f"aggregate-enum-record-{layout}",
        projects=(project,),
        fixture_root=root,
        build_root=root.parent / "layout-native" / layout,
        repository_root=ROOT,
        requested_toolchain=requested_toolchain,
        strict_flags=(*C11_STRICT_FLAGS, "-O0"),
    )
    validate_report(
        report,
        required_coverage=frozenset(
            {f"{layout}-layout", "enum-record", "generated-executable", "runtime-free"}
        ),
    )


def check_production_server_determinism(root: Path, expected_hxcir: str) -> None:
    """Compare cold output with two real custom-target requests on one Haxe server."""
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
        server_hxcir: list[str] = []
        for name in ("server-first", "server-warm"):
            output = root / name
            result = custom_target(
                POSITIVE,
                output,
                main="AggregateFixture",
                connect=endpoint,
                hxcir_report=True,
            )
            server_hxcir.append(
                reported_hxcir(result, f"{name} aggregate production compile")
            )
            validate_production(output, layout="unity", profile="portable", policy="auto")
            if generated_tree(output) != generated_tree(root / "first"):
                raise AggregateLoweringFailure(
                    f"{name} aggregate artifacts differed from the cold custom-target build"
                )
        if server_hxcir != [expected_hxcir, expected_hxcir]:
            comparisons = (
                f"cold==server-first: {expected_hxcir == server_hxcir[0]}; "
                f"cold==server-warm: {expected_hxcir == server_hxcir[1]}; "
                f"server-first==server-warm: {server_hxcir[0] == server_hxcir[1]}; "
                f"lengths: cold={len(expected_hxcir)}, first={len(server_hxcir[0])}, "
                f"warm={len(server_hxcir[1])}"
            )
            raise AggregateLoweringFailure(
                "cold and repeated warm-server custom-target HxcIR trees differed; "
                + comparisons
            )
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def check_production(*, requested_toolchain: str) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-aggregate-production-") as temporary:
        root = Path(temporary)
        cold_hxcir: str | None = None
        matrix = (
            ("first", "unity", "portable", None, "auto"),
            ("repeat", "unity", "portable", None, "auto"),
            ("none", "unity", "portable", "none", "none"),
            ("metal", "unity", "metal", None, "minimal"),
            ("split", "split", "portable", None, "auto"),
            ("package", "package", "portable", None, "auto"),
        )
        for name, layout, profile, runtime, policy in matrix:
            output = root / name
            result = custom_target(
                POSITIVE,
                output,
                main="AggregateFixture",
                profile=profile,
                runtime=runtime,
                layout=layout,
                hxcir_report=name == "first",
            )
            if name == "first":
                cold_hxcir = reported_hxcir(result, "first aggregate production compile")
            else:
                require_compile_success(result, f"{name} aggregate production compile")
            validate_production(output, layout=layout, profile=profile, policy=policy)
        if generated_tree(root / "first") != generated_tree(root / "repeat"):
            raise AggregateLoweringFailure("two aggregate production roots were not byte-identical")
        if cold_hxcir is None:
            raise AggregateLoweringFailure("cold aggregate production compile omitted HxcIR")
        check_production_server_determinism(root, cold_hxcir)
        for relative in ("include/hxc/program.h", "src/program.c"):
            if (root / "first" / relative).read_bytes() != (root / "metal" / relative).read_bytes():
                raise AggregateLoweringFailure(
                    f"portable and metal changed direct aggregate artifact {relative}"
                )
        for layout in ("unity", "split", "package"):
            output = root / ("first" if layout == "unity" else layout)
            check_production_native(
                output, layout=layout, requested_toolchain=requested_toolchain
            )


def check_negative_cases() -> None:
    cases = {
        "mutation": "TField(value:anonymous-field-mutation-requires-identity-preserving-alias-analysis)",
        "identity_equality": "TBinop(OpEq:left-type):closed-record-not-admitted-in-primitive-operation",
        "dynamic": "TFunction(argument:record):the dynamic source semantic type cannot stand in for a primitive",
        "void_field": "TFunction(return-type).field:value:Void-not-an-object-type",
    }
    with tempfile.TemporaryDirectory(prefix="hxc-aggregate-negative-") as temporary:
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
                raise AggregateLoweringFailure(
                    f"{directory} did not fail closed with exact HXC1001 and no output\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )

        # A contextual object literal with the wrong shape is rejected by the
        # Haxe typer before haxe.c receives TypedExpr. This separate assertion
        # keeps the source-positioned boundary honest without manufacturing an
        # impossible malformed TypedExpr through casts or untyped source.
        contextual_output = root / "contextual-mismatch"
        contextual = custom_target(
            FIXTURES / "contextual_mismatch", contextual_output, main="Main"
        )
        contextual_combined = (contextual.stdout + contextual.stderr).replace(
            "\\", "/"
        )
        if (
            contextual.returncode == 0
            or "fixtures/contextual_mismatch/Main.hx:" not in contextual_combined
            or "has no field y" not in contextual_combined
            or generated_files(contextual_output)
        ):
            raise AggregateLoweringFailure(
                "contextually typed record mismatch did not fail in the Haxe "
                "front end with a source position and no output\n"
                f"stdout:\n{contextual.stdout}\nstderr:\n{contextual.stderr}"
            )

        metal_output = root / "metal-packed-struct"
        metal = custom_target(
            ROOT / "test/typed_c/fixtures/metal_packed_struct",
            metal_output,
            main="Main",
            profile="metal",
            lifecycle_probe=True,
        )
        combined = (metal.stdout + metal.stderr).replace("\\", "/")
        if (
            metal.returncode != 1
            or "HXC5002" not in combined
            or "metal packed layouts are unavailable" not in combined
            or "[profile=metal]" not in combined
            or "metal_packed_struct/Main.hx:" not in combined
            or generated_files(metal_output)
        ):
            raise AggregateLoweringFailure(
                "metal packed typed-C struct did not fail closed with HXC5002 and no output\n"
                f"stdout:\n{metal.stdout}\nstderr:\n{metal.stderr}"
            )


def check_managed_optional(*, requested_toolchain: str) -> None:
    """Prove managed `Null<Record>` and `Null<Enum>` ownership in every mode."""
    fixture = FIXTURES / "managed_optional"
    oracle = subprocess.run(
        [development_tool("haxe"), "-cp", str(fixture), "-main", "Main", "--interp"],
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if oracle.returncode != 0 or oracle.stdout or oracle.stderr:
        raise AggregateLoweringFailure(
            "managed optional Eval oracle failed\n"
            f"stdout:\n{oracle.stdout}\nstderr:\n{oracle.stderr}"
        )

    with tempfile.TemporaryDirectory(prefix="hxc-managed-optional-") as temporary:
        root = Path(temporary)
        expected_features = [
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
        ]

        def validate_output(output: Path, label: str) -> None:
            runtime_plan = json.loads(
                (output / "hxc.runtime-plan.json").read_text(encoding="utf-8")
            )
            if runtime_plan.get("features") != expected_features:
                raise AggregateLoweringFailure(
                    f"{label} managed optional selected an unexpected runtime feature set"
                )
            generated_c = b"\n".join(
                path.read_bytes() for path in sorted(output.rglob("*.c"))
            ).lower()
            for marker in (
                b"hxc_optional_",
                b"hxc_record_",
                b"hxc_enum_",
                b"hxc_bytes_ref_retain",
            ):
                if marker not in generated_c:
                    raise AggregateLoweringFailure(
                        f"{label} managed optional C omitted lifecycle marker {marker!r}"
                    )
            generated_headers = b"\n".join(
                path.read_bytes() for path in sorted(output.rglob("*.h"))
            )
            parsed_choice_declaration = (
                b"struct hxc_Main_ParsedChoice {\n"
                b"  int32_t hxc_next;\n"
                b"  struct hxc_Main_RecursiveChoice hxc_value;\n"
                b"};"
            )
            if generated_headers.count(parsed_choice_declaration) != 1:
                raise AggregateLoweringFailure(
                    f"{label} did not emit exactly one readable ParsedChoice layout"
                )

        matrix = (
            ("first", "unity", False),
            ("repeat", "unity", False),
            ("reverse", "unity", True),
            ("split", "split", False),
            ("package", "package", False),
        )
        for name, layout, reverse in matrix:
            output = root / name
            result = custom_target(
                fixture,
                output,
                main="Main",
                layout=layout,
                reverse=reverse,
                hxcir_report=name == "first",
            )
            if name == "first":
                hxcir = reported_hxcir(result, "first managed optional compile")
                for marker in (
                    "optional-enum-switch-null-check",
                    "optional-enum-switch-unwrap",
                    "enum-parameter-HasValues-unwrap",
                    "tag-switch value=",
                    "project-tag value=",
                    "retain-optional-alias",
                    "release-optional-assignment-target",
                    'implementation=program-local("optional-lifecycle:',
                ):
                    if marker not in hxcir:
                        raise AggregateLoweringFailure(
                            f"managed optional HxcIR omitted {marker!r}"
                        )
                parsed_choice = named_function_section(
                    hxcir, "Main", "parsedChoice"
                )
                record_ids = set(
                    re.findall(r'instance\.closed-record\.[0-9a-f]+', parsed_choice)
                )
                if len(record_ids) != 1:
                    raise AggregateLoweringFailure(
                        "contextually typed optional record literal did not keep one "
                        f"record identity: {sorted(record_ids)!r}"
                    )
                record_id = next(iter(record_ids))
                for marker in (
                    "direct-optional-null-check",
                    "direct-optional-unwrap",
                    "retain-record-enum-field",
                    f'construct-aggregate instance="{record_id}"',
                    f"target=nullable(tagged,instance(\"{record_id}\"))",
                ):
                    if marker not in parsed_choice:
                        raise AggregateLoweringFailure(
                            "contextually typed optional record HxcIR omitted "
                            f"{marker!r}"
                        )
            else:
                require_compile_success(result, f"{name} managed optional compile")
            validate_output(output, name)
        baseline = generated_tree(root / "first")
        for name in ("repeat", "reverse"):
            if generated_tree(root / name) != baseline:
                raise AggregateLoweringFailure(
                    f"{name} managed optional output differed from the first compile"
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
                require_compile_success(result, f"{name} managed optional compile")
                validate_output(output, name)
                if generated_tree(output) != baseline:
                    raise AggregateLoweringFailure(
                        f"{name} managed optional output differed from the cold compile"
                    )
        finally:
            server.terminate()
            try:
                server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                server.kill()
                server.wait(timeout=5)

        def native_project(output: Path, label: str) -> CFixtureProject:
            sources = tuple(
                path.relative_to(output).as_posix()
                for path in sorted(output.rglob("*.c"))
            )
            headers = tuple(
                path.relative_to(output).as_posix()
                for path in sorted(output.rglob("*.h"))
            )
            return CFixtureProject(
                f"managed-optional-value-{label}",
                sources,
                headers,
                ("include", "runtime/include"),
                "",
                (
                    "generated-executable",
                    f"{label}-layout",
                    "managed-value-optional",
                    "present-and-absent",
                ),
            )

        for name in ("first", "split", "package"):
            output = root / name
            project = native_project(output, name)
            normal = run_c_fixture_corpus(
                suite=f"managed-optional-record-{name}",
                projects=(project,),
                fixture_root=output,
                build_root=root / "native" / name,
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, "-O0"),
            )
            validate_report(normal, required_coverage=frozenset(project.coverage))

        check_managed_optional_cpp(
            root / "first",
            root / "managed-optional-cxx",
            requested_toolchain=requested_toolchain,
        )

        base = native_project(root / "first", "unity")
        sanitized = CFixtureProject(
            "managed-optional-value-sanitized",
            base.sources,
            base.headers,
            base.include_directories,
            base.expected_stdout,
            (*base.coverage, "asan-ubsan"),
            link_arguments=("-fsanitize=address,undefined",),
        )
        sanitizer_report = run_c_fixture_corpus(
            suite="managed-optional-record-sanitized",
            projects=(sanitized,),
            fixture_root=root / "first",
            build_root=root / "sanitized",
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
            sanitizer_report, required_coverage=frozenset(sanitized.coverage)
        )

        rejected = custom_target(
            fixture, root / "runtime-none", main="Main", runtime="none"
        )
        combined = (rejected.stdout + rejected.stderr).replace("\\", "/")
        if (
            rejected.returncode != 1
            or "HXC2000" not in combined
            or "runtime policy `none`" not in combined
            or generated_files(root / "runtime-none")
        ):
            raise AggregateLoweringFailure(
                "managed optional did not fail closed under runtime-none\n"
                f"stdout:\n{rejected.stdout}\nstderr:\n{rejected.stderr}"
            )


def snapshot_report() -> dict[str, object]:
    return {
        "schemaVersion": 1,
        "status": "closed-anonymous-records-direct-runtime-free",
        "profile": "portable",
        "hxcir": (EXPECTED / "aggregates.hxcir").read_text(encoding="utf-8"),
        "header": (EXPECTED / "program.h").read_text(encoding="utf-8"),
        "sources": [
            {
                "path": "src/program.c",
                "content": (EXPECTED / "program.c").read_text(encoding="utf-8"),
            }
        ],
        "aggregates": json.loads((EXPECTED / "aggregates.json").read_text(encoding="utf-8")),
        "symbols": json.loads((EXPECTED / "symbols.json").read_text(encoding="utf-8")),
        "runtimeFeatures": [],
        "runtimeArtifacts": [],
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument("--native-only", action="store_true")
    parser.add_argument("--managed-optional-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("aggregate-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.managed_optional_only:
            check_managed_optional(requested_toolchain=args.toolchain)
            print("aggregate-lowering: OK: managed optional focused matrix passed")
            return 0
        if args.native_only:
            report = snapshot_report()
            validate(report)
            check_native(report, requested_toolchain=args.toolchain)
            print("aggregate-lowering: OK: required aggregate native matrix passed")
            return 0

        first_payload, first = render("first aggregate render")
        second_payload, second = render("second aggregate render")
        reverse_payload, reverse = render("reverse-input aggregate render", reverse=True)
        metal_payload, metal = render("metal aggregate render", profile="metal")
        if first_payload != second_payload or first != second:
            raise AggregateLoweringFailure("two aggregate renders differed")
        if first_payload != reverse_payload or first != reverse:
            raise AggregateLoweringFailure("aggregate render changed with discovery order")
        validate(first)
        validate(metal, profile="metal")
        if normalized_profile(first) != normalized_profile(metal):
            raise AggregateLoweringFailure("portable and metal aggregate lowering diverged")
        check_snapshots(first)
        check_native(first, requested_toolchain=args.toolchain)
        check_production(requested_toolchain=args.toolchain)
        check_managed_optional(requested_toolchain=args.toolchain)
        check_negative_cases()
    except (
        AggregateLoweringFailure,
        CFixtureFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"aggregate-lowering: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "aggregate-lowering: OK: structural deduplication, source-order evaluation, "
        "bounded payload-enum records in cold/server split/package/unity builds, explicit "
        "address/copy IR, strict C11 and C++17 layout agreement, runtime-free production "
        "artifacts, managed optional Eval/native/sanitizer parity, and fail-closed "
        "identity/layout edges passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

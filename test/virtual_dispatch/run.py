#!/usr/bin/env python3
"""Prove minimal deterministic virtual dispatch and override validation."""

from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import os
import re
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from collections.abc import Iterable, Iterator
from contextlib import contextmanager
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
FIXTURES = CASE / "fixtures"
POSITIVE = FIXTURES / "positive"
INTERFACE_POSITIVE = ROOT / "test/interface_dispatch/fixtures/positive"
NATIVE = CASE / "native"
EXPECTED = CASE / "expected"
SCHEMA = ROOT / "docs/specs/dispatch-report.schema.json"
REPORT_PREFIX = "HXC_VIRTUAL_DISPATCH="
PRODUCTION_FILES = {
    "_GeneratedFiles.json",
    "cmake/CMakeLists.txt",
    "hxc.abi.json",
    "hxc.dispatch.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
    "include/hxc/program.h",
    "meson.build",
    "src/program.c",
}
EXPECTED_SUMMARY = {
    "instanceCalls": 9,
    "directCalls": 6,
    "indirectCalls": 3,
    "layouts": 1,
    "slots": 1,
    "tables": 2,
    "adapters": 2,
}
EXPECTED_DIRECT_REASONS = [
    "final-method",
    "final-receiver-class",
    "non-virtual-metadata",
    "private-method",
    "super-call",
    "super-call",
]
EXPECTED_TABLES = {
    "vtable.LeafWorker": "method.LeafWorker.value",
    "vtable.MiddleWorker": "method.MiddleWorker.value",
}
NEGATIVE_CASES = {
    "contravariant_argument": (
        "Main.hx:17: lines 17-19",
        "virtual-override-representation-mismatch:slot=slot.ConsumerBase.accept:",
        ":argument=0:expected=pointer:nullable:instance:instance.class.",
    ),
    "covariant_return": (
        "Main.hx:14: lines 14-16",
        "virtual-override-representation-mismatch:slot=slot.ReturnBase.duplicate:",
        ":return:expected=pointer:nullable:instance:instance.class.",
    ),
}
REQUIRED_NATIVE_COVERAGE = frozenset(
    {
        "closed-world-virtual-dispatch",
        "direct-call-preservation",
        "minimal-vtable",
        "override-adapters",
        "runtime-free",
        "strict-c11",
    }
)
REQUIRED_INTERFACE_NATIVE_COVERAGE = frozenset(
    {
        "inherited-interface-method",
        "multiple-interface-views",
        "program-local-interface-tables",
        "runtime-free",
        "strict-c11",
    }
)

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
C_IDENTIFIER = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


class VirtualDispatchFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class RenderedProject:
    artifacts: dict[str, bytes]
    dispatch: dict[str, object]
    hxcir: str
    header: str
    source: str
    symbols: dict[str, object]
    runtime_plan: dict[str, object]
    manifest: dict[str, object]
    abi: dict[str, object]
    inspection_payload: str
    profile: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(
    locale: str = "C", *, connect: str | None = None
) -> dict[str, str]:
    environment = os.environ.copy()
    environment["LC_ALL"] = locale
    if connect is None:
        environment["HAXE_NO_SERVER"] = "1"
    else:
        environment.pop("HAXE_NO_SERVER", None)
    return environment


def compile_fixture(
    fixture: Path,
    output: Path,
    *,
    profile: str = "portable",
    runtime: str | None = None,
    reverse: bool = False,
    report: bool = False,
    locale: str = "C",
    connect: str | None = None,
    layout: str = "unity",
) -> subprocess.CompletedProcess[str]:
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
            "--cwd",
            str(fixture),
            "build.hxml",
            "-D",
            "hxc_runtime_diagnostics=off",
        ]
    )
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    elif profile != "portable":
        raise VirtualDispatchFailure(f"unknown dispatch profile {profile!r}")
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_virtual_dispatch_report"])
    if layout == "unity":
        command.extend(["-D", "hxc_project_layout=unity"])
    elif layout != "split":
        raise VirtualDispatchFailure(f"unknown dispatch project layout {layout!r}")
    command.extend(["--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(locale, connect=connect),
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )


def generated_files(root: Path) -> set[str]:
    if not root.exists():
        return set()
    return {
        artifact.relative_to(root).as_posix()
        for artifact in root.rglob("*")
        if artifact.is_file()
    }


def read_artifacts(root: Path) -> dict[str, bytes]:
    return {
        artifact.relative_to(root).as_posix(): artifact.read_bytes()
        for artifact in sorted(root.rglob("*"))
        if artifact.is_file() and artifact.name != "_GeneratedFiles.json"
    }


def load_json(path: Path, label: str) -> dict[str, object]:
    value = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise VirtualDispatchFailure(f"{label} is not a JSON object")
    return value


def require_dict(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise VirtualDispatchFailure(f"{label} is not an object")
    return value


def require_list(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise VirtualDispatchFailure(f"{label} is not an array")
    return value


def require_text(value: object, label: str) -> str:
    if not isinstance(value, str) or not value:
        raise VirtualDispatchFailure(f"{label} is not non-empty text")
    return value


def extract_report(
    result: subprocess.CompletedProcess[str], output: Path, label: str
) -> RenderedProject:
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    other = [line for line in result.stdout.splitlines() if not line.startswith(REPORT_PREFIX)]
    if result.returncode != 0 or result.stderr or len(lines) != 1 or other:
        raise VirtualDispatchFailure(
            f"{label} emitted an invalid virtual-dispatch inspection\n"
            f"exit={result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    files = generated_files(output)
    if files != PRODUCTION_FILES:
        raise VirtualDispatchFailure(
            f"{label} artifact set drifted: {sorted(files)!r}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    inspection = json.loads(payload)
    if not isinstance(inspection, dict):
        raise VirtualDispatchFailure(f"{label} inspection is not an object")
    sidecar = load_json(output / "hxc.dispatch.json", "dispatch sidecar")
    dispatch = require_dict(inspection.get("dispatch"), "inspection dispatch")
    if dispatch != sidecar:
        raise VirtualDispatchFailure("inspection and production dispatch sidecar differ")
    hxcir = require_text(inspection.get("hxcir"), "dispatch HxcIR")
    profile = require_text(inspection.get("profile"), "inspection profile")
    if inspection.get("schemaVersion") != 1:
        raise VirtualDispatchFailure("dispatch inspection lost wrapper schema version 1")
    return RenderedProject(
        artifacts=read_artifacts(output),
        dispatch=dispatch,
        hxcir=hxcir,
        header=(output / "include/hxc/program.h").read_text(encoding="utf-8"),
        source=(output / "src/program.c").read_text(encoding="utf-8"),
        symbols=load_json(output / "hxc.symbols.json", "symbol table"),
        runtime_plan=load_json(output / "hxc.runtime-plan.json", "runtime plan"),
        manifest=load_json(output / "hxc.manifest.json", "compiler manifest"),
        abi=load_json(output / "hxc.abi.json", "ABI manifest"),
        inspection_payload=payload,
        profile=profile,
    )


def render(
    label: str,
    *,
    profile: str = "portable",
    runtime: str | None = None,
    reverse: bool = False,
    locale: str = "C",
    connect: str | None = None,
    fixture: Path = POSITIVE,
    interface: bool = False,
) -> RenderedProject:
    with tempfile.TemporaryDirectory(prefix="hxc-virtual-dispatch-render-") as temporary:
        output = Path(temporary) / "generated"
        result = compile_fixture(
            fixture,
            output,
            profile=profile,
            runtime=runtime,
            reverse=reverse,
            report=True,
            locale=locale,
            connect=connect,
        )
        project = extract_report(result, output, label)
        if interface:
            validate_interface_project(project, expected_profile=profile)
        else:
            validate_project(project, expected_profile=profile)
        return project


def validate_schema_document() -> None:
    schema = load_json(SCHEMA, "dispatch report schema")
    properties = require_dict(schema.get("properties"), "schema properties")
    if (
        schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema"
        or schema.get("$id")
        != "https://reflaxe-c.dev/schemas/dispatch-report.schema.json"
        or schema.get("additionalProperties") is not False
        or require_dict(properties.get("schemaVersion"), "schema version").get("const")
        != 2
        or require_dict(properties.get("algorithm"), "schema algorithm").get("const")
        != "hxc-closed-world-instance-dispatch-v2"
    ):
        raise VirtualDispatchFailure("dispatch report schema identity drifted")


def validate_source(value: object, label: str) -> tuple[str, int, int, int, int]:
    source = require_dict(value, f"{label} source")
    file = require_text(source.get("file"), f"{label} source file")
    coordinates = [
        source.get(key)
        for key in ("startLine", "startColumn", "endLine", "endColumn")
    ]
    if (
        file.startswith("/")
        or "\\" in file
        or any(part in ("", ".", "..") for part in file.split("/"))
        or not all(isinstance(value, int) and value > 0 for value in coordinates)
    ):
        raise VirtualDispatchFailure(f"{label} has malformed source {source!r}")
    return (file, coordinates[0], coordinates[1], coordinates[2], coordinates[3])


def validate_dispatch(report: dict[str, object]) -> None:
    if (
        report.get("schemaVersion") != 2
        or report.get("algorithm") != "hxc-closed-world-instance-dispatch-v2"
        or report.get("status") != "analyzed-reachable-instance-dispatch"
        or report.get("tablePolicy")
        != "reachable-class-vtables-and-per-interface-itables-only"
        or report.get("adapterPolicy")
        != "representation-identical-overrides-with-typed-receiver-thunks"
        or report.get("runtimeFeatures") != []
        or report.get("summary") != EXPECTED_SUMMARY
    ):
        raise VirtualDispatchFailure("dispatch report identity, policy, or summary drifted")

    calls = require_list(report.get("calls"), "dispatch calls")
    direct_reasons: list[str] = []
    virtual_calls = 0
    derived_virtual_call = False
    source_keys: list[tuple[str, int, int, int, int]] = []
    for index, value in enumerate(calls):
        call = require_dict(value, f"dispatch call {index}")
        if call.get("id") != f"dispatch.call.{index}":
            raise VirtualDispatchFailure("dispatch calls lost stable ordinal IDs")
        for key in ("callerFunctionId", "methodFunctionId", "receiverStaticClass"):
            require_text(call.get(key), f"dispatch call {index} {key}")
        source_keys.append(validate_source(call.get("source"), f"dispatch call {index}"))
        choice = call.get("dispatch")
        reason = require_text(call.get("reason"), f"dispatch call {index} reason")
        if choice == "direct":
            direct_reasons.append(reason)
            require_text(call.get("targetFunctionId"), "direct target")
            if call.get("slotId") is not None:
                raise VirtualDispatchFailure("direct call unexpectedly retained a slot")
        elif choice == "virtual":
            virtual_calls += 1
            if (
                call.get("receiverStaticClass") == "LeafWorker"
                and call.get("methodFunctionId") == "method.LeafWorker.value"
            ):
                derived_virtual_call = True
            if (
                reason != "ordinary-overridable-instance-method"
                or call.get("targetFunctionId") is not None
                or call.get("slotId") != "slot.BaseWorker.value"
            ):
                raise VirtualDispatchFailure("indirect call explanation drifted")
        else:
            raise VirtualDispatchFailure(f"unknown dispatch choice {choice!r}")
    if (
        sorted(direct_reasons) != EXPECTED_DIRECT_REASONS
        or virtual_calls != 3
        or not derived_virtual_call
    ):
        raise VirtualDispatchFailure("direct-call reasons or indirect-call count drifted")
    if source_keys != sorted(
        source_keys, key=lambda value: (value[0].encode("utf-8"), *value[1:])
    ):
        raise VirtualDispatchFailure("dispatch calls are not in stable source order")

    layouts = require_list(report.get("layouts"), "dispatch layouts")
    slots = require_list(report.get("slots"), "dispatch slots")
    tables = require_list(report.get("tables"), "dispatch tables")
    if len(layouts) != 1 or len(slots) != 1 or len(tables) != 2:
        raise VirtualDispatchFailure("minimal dispatch plan cardinality drifted")
    layout = require_dict(layouts[0], "dispatch layout")
    slot = require_dict(slots[0], "dispatch slot")
    if (
        layout.get("id") != "vtable.layout.BaseWorker"
        or layout.get("slotIds") != ["slot.BaseWorker.value"]
        or slot.get("id") != "slot.BaseWorker.value"
        or slot.get("parameterRepresentations") != ["i32"]
        or slot.get("returnRepresentation") != "i32"
        or layout.get("rootInstanceId") != slot.get("ownerInstanceId")
    ):
        raise VirtualDispatchFailure("layout root or slot representation drifted")
    for key in ("cTag",):
        if C_IDENTIFIER.fullmatch(require_text(layout.get(key), f"layout {key}")) is None:
            raise VirtualDispatchFailure(f"layout {key} is not a C identifier")
    if C_IDENTIFIER.fullmatch(require_text(slot.get("cMember"), "slot C member")) is None:
        raise VirtualDispatchFailure("slot member is not a C identifier")

    table_ids: list[str] = []
    for value in tables:
        table = require_dict(value, "dispatch table")
        identifier = require_text(table.get("id"), "dispatch table ID")
        table_ids.append(identifier)
        entries = require_list(table.get("entries"), f"{identifier} entries")
        expected_implementation = EXPECTED_TABLES.get(identifier)
        if (
            expected_implementation is None
            or table.get("layoutId") != "vtable.layout.BaseWorker"
            or len(entries) != 1
            or C_IDENTIFIER.fullmatch(require_text(table.get("cName"), "table C name"))
            is None
        ):
            raise VirtualDispatchFailure(f"dispatch table {identifier!r} drifted")
        entry = require_dict(entries[0], f"{identifier} entry")
        if (
            entry.get("slotId") != "slot.BaseWorker.value"
            or entry.get("implementationFunctionId") != expected_implementation
            or C_IDENTIFIER.fullmatch(
                require_text(entry.get("adapterCName"), f"{identifier} adapter")
            )
            is None
        ):
            raise VirtualDispatchFailure(f"dispatch table {identifier!r} entry drifted")
    if table_ids != sorted(EXPECTED_TABLES):
        raise VirtualDispatchFailure("dispatch tables are not in canonical ID order")


def validate_hxcir(project: RenderedProject) -> None:
    hxcir = project.hxcir
    required = (
        "hxcir schema=18\n",
        'layout "vtable.layout.BaseWorker"',
        'slot "slot.BaseWorker.value"',
        'table "vtable.LeafWorker"',
        'table "vtable.MiddleWorker"',
        "bind-virtual-table place=",
        'dispatch=virtual(slot="slot.BaseWorker.value"',
        'dispatch=direct("method.BaseWorker.finalValue")',
        'dispatch=direct("method.BaseWorker.pinnedValue")',
        'dispatch=direct("method.FinalWorker.value")',
        'class-layout base=none header=virtual("vtable.layout.BaseWorker")',
        "class-layout base=none header=none",
    )
    if not hxcir.startswith(required[0]) or any(marker not in hxcir for marker in required[1:]):
        raise VirtualDispatchFailure("schema-18 dispatch HxcIR lost a structural marker")
    if hxcir.count('dispatch=virtual(slot="slot.BaseWorker.value"') != 3:
        raise VirtualDispatchFailure("HxcIR did not retain exactly three indirect calls")
    if str(ROOT) in hxcir or "\\" in hxcir or "hxrt" in hxcir.lower():
        raise VirtualDispatchFailure("dispatch HxcIR leaked a host path or runtime")


def validate_generated_c(project: RenderedProject) -> None:
    report = project.dispatch
    layout = require_dict(require_list(report.get("layouts"), "layouts")[0], "layout")
    slot = require_dict(require_list(report.get("slots"), "slots")[0], "slot")
    c_tag = require_text(layout.get("cTag"), "layout C tag")
    c_member = require_text(slot.get("cMember"), "slot C member")
    tables = [require_dict(value, "table") for value in require_list(report.get("tables"), "tables")]
    adapters = [
        require_text(
            require_dict(require_list(table.get("entries"), "entries")[0], "entry").get(
                "adapterCName"
            ),
            "adapter C name",
        )
        for table in tables
    ]
    if (
        project.header.count(f"const struct {c_tag} *") != 1
        or f"struct {c_tag} {{" not in project.header
        or f"(*{c_member})" not in project.header
        or "FinalWorker_virtual" in project.header
        or "FinalWorker_table" in project.header
    ):
        raise VirtualDispatchFailure("root-only C object header or vtable shape drifted")
    for table in tables:
        c_name = require_text(table.get("cName"), "table C name")
        if (
            f"extern const struct {c_tag} {c_name};" not in project.header
            or f"const struct {c_tag} {c_name}" not in project.source
            or f"static const struct {c_tag} {c_name}" in project.source
        ):
            raise VirtualDispatchFailure(f"generated C omitted table {c_name}")
    for adapter in adapters:
        if project.source.count(f"static int32_t {adapter}(") != 2:
            raise VirtualDispatchFailure(f"generated C omitted adapter declaration/definition {adapter}")
    if (
        project.source.count(f"->{c_member}(") != 3
        or "hxc_BaseWorker_finalValue(" not in project.source
        or "hxc_BaseWorker_pinnedValue(" not in project.source
        or "hxc_FinalWorker_value(" not in project.source
        or "int main(void)" not in project.source
    ):
        raise VirtualDispatchFailure("generated C lost indirect or preserved direct calls")


def validate_product_sidecars(project: RenderedProject, expected_profile: str) -> None:
    plan = project.runtime_plan
    proof = require_dict(plan.get("noRuntimeProof"), "no-runtime proof")
    if (
        project.profile != expected_profile
        or plan.get("profile") != expected_profile
        or plan.get("status") != "analyzed-runtime-free"
        or plan.get("features") != []
        or plan.get("artifacts") != []
        or "reachable-program-local-virtual-dispatch"
        not in require_list(plan.get("directDecisions"), "direct decisions")
        or proof.get("status") != "eligible"
        or "root-only program-local vtable pointers"
        not in str(proof.get("semanticProof"))
        or project.manifest.get("compilationStatus")
        != "lowered-direct-value-executable"
        or project.abi.get("status") != "analyzed-no-public-exports"
        or project.abi.get("exports") != []
        or project.abi.get("types") != []
    ):
        raise VirtualDispatchFailure("dispatch runtime, manifest, or ABI contract drifted")
    addressed = False
    for value in require_list(project.manifest.get("artifacts"), "manifest artifacts"):
        artifact = require_dict(value, "manifest artifact")
        if artifact.get("path") == "hxc.dispatch.json":
            content = project.artifacts.get("hxc.dispatch.json")
            if (
                artifact.get("kind") != "dispatch-report"
                or content is None
                or artifact.get("sha256") != hashlib.sha256(content).hexdigest()
            ):
                raise VirtualDispatchFailure("manifest dispatch address drifted")
            addressed = True
    if not addressed:
        raise VirtualDispatchFailure("manifest omitted the dispatch report")


def validate_project(project: RenderedProject, *, expected_profile: str) -> None:
    validate_dispatch(project.dispatch)
    validate_hxcir(project)
    validate_generated_c(project)
    validate_product_sidecars(project, expected_profile)
    if project.symbols.get("algorithm") != "hxc-c-symbol-v2":
        raise VirtualDispatchFailure("dispatch project omitted finalized symbols")
    for path, content in project.artifacts.items():
        if str(ROOT).encode() in content or b"\\" in content or b"hxrt" in content.lower():
            raise VirtualDispatchFailure(
                f"dispatch artifact {path!r} leaked a host path or runtime dependency"
            )


def validate_interface_project(
    project: RenderedProject, *, expected_profile: str
) -> None:
    """Validate inherited and multiple-interface dispatch as structural C.

    The fixture deliberately calls a base-interface method through a child
    interface and exposes the same object through a second interface. This
    catches accidental single-table designs and prevents an unused interface
    declaration from becoming emitted C merely because Haxe typed it.
    """
    report = project.dispatch
    if (
        report.get("schemaVersion") != 2
        or report.get("algorithm") != "hxc-closed-world-instance-dispatch-v2"
        or report.get("status") != "analyzed-reachable-instance-dispatch"
        or report.get("tablePolicy")
        != "reachable-class-vtables-and-per-interface-itables-only"
        or report.get("adapterPolicy")
        != "representation-identical-overrides-with-typed-receiver-thunks"
        or report.get("runtimeFeatures") != []
        or report.get("summary")
        != {
            "instanceCalls": 3,
            "directCalls": 0,
            "indirectCalls": 3,
            "layouts": 2,
            "slots": 3,
            "tables": 2,
            "adapters": 3,
        }
    ):
        raise VirtualDispatchFailure("interface dispatch report identity or summary drifted")

    calls = [
        require_dict(value, "interface dispatch call")
        for value in require_list(report.get("calls"), "interface dispatch calls")
    ]
    expected_calls = (
        ("CounterView", "method.ReadableView.read", "interface-slot.CounterView.read"),
        ("CounterView", "method.CounterView.doubled", "interface-slot.CounterView.doubled"),
        ("ResetView", "method.ResetView.resetValue", "interface-slot.ResetView.resetValue"),
    )
    for index, (call, expected) in enumerate(zip(calls, expected_calls, strict=True)):
        if (
            call.get("id") != f"dispatch.call.{index}"
            or call.get("dispatch") != "interface"
            or call.get("reason") != "ordinary-interface-method"
            or call.get("receiverStaticClass") != expected[0]
            or call.get("methodFunctionId") != expected[1]
            or call.get("slotId") != expected[2]
            or call.get("targetFunctionId") is not None
        ):
            raise VirtualDispatchFailure(f"interface dispatch call {index} drifted")
        validate_source(call.get("source"), f"interface dispatch call {index}")

    layouts = {
        require_text(layout.get("id"), "interface layout ID"): layout
        for layout in (
            require_dict(value, "interface layout")
            for value in require_list(report.get("layouts"), "interface layouts")
        )
    }
    slots = {
        require_text(slot.get("id"), "interface slot ID"): slot
        for slot in (
            require_dict(value, "interface slot")
            for value in require_list(report.get("slots"), "interface slots")
        )
    }
    tables = {
        require_text(table.get("id"), "interface table ID"): table
        for table in (
            require_dict(value, "interface table")
            for value in require_list(report.get("tables"), "interface tables")
        )
    }
    if (
        set(layouts) != {"itable.layout.CounterView", "itable.layout.ResetView"}
        or set(slots)
        != {
            "interface-slot.CounterView.doubled",
            "interface-slot.CounterView.read",
            "interface-slot.ResetView.resetValue",
        }
        or set(tables) != {"itable.Counter.CounterView", "itable.Counter.ResetView"}
        or layouts["itable.layout.CounterView"].get("slotIds")
        != ["interface-slot.CounterView.doubled", "interface-slot.CounterView.read"]
        or layouts["itable.layout.ResetView"].get("slotIds")
        != ["interface-slot.ResetView.resetValue"]
    ):
        raise VirtualDispatchFailure("interface layout, slot, or table ownership drifted")
    for table_id, table in tables.items():
        entries = require_list(table.get("entries"), f"{table_id} entries")
        if table.get("classInstanceId") is None or not entries:
            raise VirtualDispatchFailure(f"{table_id} lost its concrete implementation")
        for value in entries:
            entry = require_dict(value, f"{table_id} entry")
            if (
                require_text(entry.get("slotId"), f"{table_id} slot") not in slots
                or C_IDENTIFIER.fullmatch(
                    require_text(entry.get("adapterCName"), f"{table_id} adapter")
                )
                is None
            ):
                raise VirtualDispatchFailure(f"{table_id} contains a malformed entry")

    hxcir = project.hxcir
    for marker in (
        'type "type.interface.',
        'layout "itable.layout.CounterView"',
        'layout "itable.layout.ResetView"',
        'table "itable.Counter.CounterView"',
        'table "itable.Counter.ResetView"',
        "construct-interface interface=",
        'dispatch=interface(type="instance.interface.',
    ):
        if marker not in hxcir:
            raise VirtualDispatchFailure(f"interface HxcIR omitted {marker!r}")
    if hxcir.count("construct-interface interface=") != 2 or hxcir.count(
        'dispatch=interface(type="instance.interface.'
    ) != 3:
        raise VirtualDispatchFailure("interface HxcIR cardinality drifted")

    for layout in layouts.values():
        c_tag = require_text(layout.get("cTag"), "interface layout C tag")
        value_tag = c_tag.removesuffix("_table_layout") + "_value"
        if (
            f"struct {c_tag} {{" not in project.header
            or f"struct {value_tag} {{" not in project.header
            or "void *object;" not in project.header
            or f"const struct {c_tag} *table;" not in project.header
        ):
            raise VirtualDispatchFailure(f"interface value {value_tag!r} lost its fat C shape")
    for table_id, table in tables.items():
        c_name = require_text(table.get("cName"), f"{table_id} C name")
        if f"&{c_name}" not in project.source or f" {c_name} =" not in project.source:
            raise VirtualDispatchFailure(f"interface table {table_id!r} was not emitted")
    if (
        project.source.count("void *hxc_receiver") != 6
        or project.source.count(".table->") != 3
        or "UnusedView" in project.header
        or "UnusedView" in project.source
        or "UnusedView" in hxcir
    ):
        raise VirtualDispatchFailure("interface adapters, calls, or reachability drifted")

    proof = require_dict(project.runtime_plan.get("noRuntimeProof"), "interface no-runtime proof")
    if (
        project.profile != expected_profile
        or project.runtime_plan.get("profile") != expected_profile
        or project.runtime_plan.get("status") != "analyzed-runtime-free"
        or project.runtime_plan.get("features") != []
        or "reachable-program-local-interface-dispatch"
        not in require_list(project.runtime_plan.get("directDecisions"), "direct decisions")
        or "compact program-local interface values" not in str(proof.get("semanticProof"))
        or project.manifest.get("compilationStatus")
        != "lowered-direct-value-executable"
        or project.abi.get("status") != "analyzed-no-public-exports"
        or project.abi.get("exports") != []
        or project.symbols.get("algorithm") != "hxc-c-symbol-v2"
    ):
        raise VirtualDispatchFailure("interface runtime, manifest, ABI, or symbols drifted")
    for path, content in project.artifacts.items():
        if str(ROOT).encode() in content or b"\\" in content or b"hxrt" in content.lower():
            raise VirtualDispatchFailure(
                f"interface artifact {path!r} leaked a host path or runtime dependency"
            )


def first_difference(left: bytes, right: bytes) -> int:
    for index, (left_byte, right_byte) in enumerate(zip(left, right)):
        if left_byte != right_byte:
            return index
    return min(len(left), len(right))


def assert_artifacts_equal(
    left: dict[str, bytes], right: dict[str, bytes], label: str
) -> None:
    if left.keys() != right.keys():
        raise VirtualDispatchFailure(
            f"{label} artifact sets differ: {sorted(left)!r} != {sorted(right)!r}"
        )
    for path in left:
        if left[path] != right[path]:
            raise VirtualDispatchFailure(
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


def normalized_profile(project: RenderedProject) -> dict[str, object]:
    hxcir = project.hxcir.replace('profile="portable"', 'profile="<profile>"').replace(
        'profile="metal"', 'profile="<profile>"'
    )
    return {
        "dispatch": project.dispatch,
        "hxcir": hxcir,
        "header": project.header,
        "source": project.source,
        "symbols": project.symbols,
    }


def check_determinism_and_policy(*, connect: str) -> RenderedProject:
    first = render("first virtual-dispatch render")
    repeated = render("repeated virtual-dispatch render")
    reversed_project = render(
        "reverse-input/locale virtual-dispatch render",
        reverse=True,
        locale=alternate_locale(),
        connect=connect,
    )
    assert_artifacts_equal(first.artifacts, repeated.artifacts, "repeated cold render")
    assert_artifacts_equal(
        first.artifacts,
        reversed_project.artifacts,
        "reverse-order/locale render",
    )
    if first.inspection_payload != repeated.inspection_payload:
        raise VirtualDispatchFailure("repeated dispatch inspection payloads differed")
    metal = render("metal virtual-dispatch render", profile="metal", connect=connect)
    runtime_none = render(
        "runtime-none virtual-dispatch render", runtime="none", connect=connect
    )
    if normalized_profile(first) != normalized_profile(metal):
        raise VirtualDispatchFailure("portable and metal dispatch compiler output diverged")
    for relative in (
        "hxc.dispatch.json",
        "hxc.symbols.json",
        "include/hxc/program.h",
        "src/program.c",
    ):
        if first.artifacts[relative] != runtime_none.artifacts[relative]:
            raise VirtualDispatchFailure(
                f"runtime-none changed dispatch compiler artifact {relative!r}"
            )
    return first


def check_interface_determinism(*, connect: str) -> RenderedProject:
    first = render(
        "first interface-dispatch render",
        fixture=INTERFACE_POSITIVE,
        interface=True,
        connect=connect,
    )
    reversed_project = render(
        "reverse-input/locale interface-dispatch render",
        fixture=INTERFACE_POSITIVE,
        interface=True,
        reverse=True,
        locale=alternate_locale(),
        connect=connect,
    )
    assert_artifacts_equal(
        first.artifacts,
        reversed_project.artifacts,
        "interface reverse-order/locale render",
    )
    if first.inspection_payload != reversed_project.inspection_payload:
        raise VirtualDispatchFailure(
            "interface dispatch inspection changed with module order or locale"
        )
    return first


def snapshot_values(project: RenderedProject) -> dict[str, object]:
    return {
        "dispatch.hxcir": project.hxcir,
        "hxc.dispatch.json": project.dispatch,
        "program.h": project.header,
        "program.c": project.source,
        "symbols.json": project.symbols,
    }


def interface_snapshot_values(project: RenderedProject) -> dict[str, object]:
    return {
        "interface-dispatch.hxcir": project.hxcir,
        "interface-hxc.dispatch.json": project.dispatch,
        "interface-program.h": project.header,
        "interface-program.c": project.source,
        "interface-symbols.json": project.symbols,
    }


def all_snapshot_values(
    class_project: RenderedProject, interface_project: RenderedProject
) -> dict[str, object]:
    return {**snapshot_values(class_project), **interface_snapshot_values(interface_project)}


def render_snapshot() -> dict[str, object]:
    with haxe_server() as endpoint:
        class_project = check_determinism_and_policy(connect=endpoint)
        interface_project = check_interface_determinism(connect=endpoint)
        return all_snapshot_values(class_project, interface_project)


def difference(expected: str, actual: str, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def check_snapshots(
    class_project: RenderedProject, interface_project: RenderedProject
) -> None:
    for name, actual in all_snapshot_values(class_project, interface_project).items():
        path = EXPECTED / name
        if not path.is_file():
            raise VirtualDispatchFailure(
                "virtual-dispatch snapshots are missing; run "
                "`npm run snapshots:update -- --suite virtual-dispatch`"
            )
        if name.endswith(".json"):
            expected = json.loads(path.read_text(encoding="utf-8"))
            if actual != expected:
                raise VirtualDispatchFailure(f"{name} semantic snapshot drifted")
        else:
            expected_text = path.read_text(encoding="utf-8")
            actual_text = require_text(actual, name)
            if actual_text != expected_text:
                raise VirtualDispatchFailure(
                    f"{name} drifted:\n" + difference(expected_text, actual_text, name)
                )


def check_negative_cases(*, connect: str) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-virtual-dispatch-negative-") as temporary:
        root = Path(temporary)
        for fixture, (source, prefix, detail) in NEGATIVE_CASES.items():
            output = root / fixture
            result = compile_fixture(FIXTURES / fixture, output, connect=connect)
            combined = result.stdout + result.stderr
            if (
                result.returncode == 0
                or "HXC1001" not in combined
                or source not in combined
                or prefix not in combined
                or detail not in combined
                or generated_files(output)
            ):
                raise VirtualDispatchFailure(
                    f"{fixture} did not fail closed at override representation validation\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
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
            raise VirtualDispatchFailure(
                f"Haxe server exited early\nstdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise VirtualDispatchFailure("Haxe server did not accept connections")


@contextmanager
def haxe_server() -> Iterator[str]:
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
        yield endpoint
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def require_silent_success(command: list[str], label: str) -> None:
    result = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise VirtualDispatchFailure(
            f"{label} failed\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def write_native_fixture(values: dict[str, object], destination: Path) -> None:
    header = destination / "include/hxc/program.h"
    source = destination / "src/program.c"
    header.parent.mkdir(parents=True, exist_ok=True)
    source.parent.mkdir(parents=True, exist_ok=True)
    header.write_text(
        require_text(values.get("program.h"), "snapshot header"),
        encoding="utf-8",
        newline="\n",
    )
    source.write_text(
        require_text(values.get("program.c"), "snapshot source"),
        encoding="utf-8",
        newline="\n",
    )


def check_cpp_header(
    fixture_root: Path, build_root: Path, *, requested_toolchain: str
) -> None:
    for toolchain in resolve_toolchains(requested_toolchain, repository_root=ROOT):
        command_name = CXX_COMMANDS[toolchain.family]
        compiler = shutil.which(command_name)
        if compiler is None:
            if requested_toolchain != "auto":
                raise VirtualDispatchFailure(
                    f"required C++ companion {command_name!r} is unavailable"
                )
            print(
                f"virtual-dispatch: SKIP optional {toolchain.family} C++17 header consumer"
            )
            continue
        for optimization in ("-O0", "-O2"):
            output = build_root / toolchain.family / optimization[1:].lower()
            output.mkdir(parents=True, exist_ok=True)
            require_silent_success(
                [
                    compiler,
                    *CXX_STRICT_FLAGS,
                    optimization,
                    f"-I{fixture_root / 'include'}",
                    "-c",
                    str(NATIVE / "dispatch_header_cpp.cpp"),
                    "-o",
                    str(output / "dispatch_header_cpp.o"),
                ],
                f"{toolchain.family} {optimization} dispatch C++17 header",
            )


def check_native(
    values: dict[str, object], *, requested_toolchain: str = "auto"
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-virtual-dispatch-native-") as temporary:
        root = Path(temporary)
        fixture_root = root / "fixture"
        write_native_fixture(values, fixture_root)
        project = CFixtureProject(
            "virtual-dispatch-positive",
            ("src/program.c",),
            ("include/hxc/program.h",),
            ("include",),
            "",
            (
                "closed-world-virtual-dispatch",
                "direct-call-preservation",
                "minimal-vtable",
                "override-adapters",
                "runtime-free",
                "strict-c11",
            ),
        )
        for optimization in ("-O0", "-O2"):
            native_report = run_c_fixture_corpus(
                suite=f"virtual-dispatch-{optimization[1:].lower()}",
                projects=(project,),
                fixture_root=fixture_root,
                build_root=root / f"c-build-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            validate_report(
                native_report, required_coverage=REQUIRED_NATIVE_COVERAGE
            )
            encoded = report_json(native_report, compact=True)
            for forbidden in (str(ROOT), str(fixture_root), str(root)):
                if forbidden in encoded:
                    raise VirtualDispatchFailure(
                        f"native report leaked absolute path {forbidden}"
                    )
        check_cpp_header(
            fixture_root, root / "cxx-build", requested_toolchain=requested_toolchain
        )


def check_interface_native(
    values: dict[str, object], *, requested_toolchain: str = "auto"
) -> None:
    """Compile and execute the generated interface program under strict C11.

    This is intentionally independent of Haxe execution: the generated program
    itself loops forever if any inherited or multiple-interface call returns the
    wrong value, so a successful native exit proves the emitted tables cooperate.
    """
    native_values = {
        "program.h": values.get("interface-program.h"),
        "program.c": values.get("interface-program.c"),
    }
    with tempfile.TemporaryDirectory(prefix="hxc-interface-dispatch-native-") as temporary:
        root = Path(temporary)
        fixture_root = root / "fixture"
        write_native_fixture(native_values, fixture_root)
        project = CFixtureProject(
            "interface-dispatch-positive",
            ("src/program.c",),
            ("include/hxc/program.h",),
            ("include",),
            "",
            tuple(sorted(REQUIRED_INTERFACE_NATIVE_COVERAGE)),
        )
        for optimization in ("-O0", "-O2"):
            report = run_c_fixture_corpus(
                suite=f"interface-dispatch-{optimization[1:].lower()}",
                projects=(project,),
                fixture_root=fixture_root,
                build_root=root / f"build-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            validate_report(report, required_coverage=REQUIRED_INTERFACE_NATIVE_COVERAGE)
            encoded = report_json(report, compact=True)
            for forbidden in (str(ROOT), str(fixture_root), str(root)):
                if forbidden in encoded:
                    raise VirtualDispatchFailure(
                        f"interface native report leaked absolute path {forbidden}"
                    )


def check_split_project(*, requested_toolchain: str) -> None:
    with tempfile.TemporaryDirectory(
        prefix="hxc-virtual-dispatch-split-"
    ) as temporary:
        root = Path(temporary)
        output = root / "generated"
        compiled = compile_fixture(POSITIVE, output, layout="split")
        if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
            raise VirtualDispatchFailure(
                "split virtual-dispatch project failed Haxe compilation\n"
                f"exit={compiled.returncode}\nstdout:\n{compiled.stdout}\n"
                f"stderr:\n{compiled.stderr}"
            )
        manifest = load_json(output / "hxc.manifest.json", "split manifest")
        configuration = require_dict(
            manifest.get("configuration"), "split manifest configuration"
        )
        build = require_dict(manifest.get("build"), "split manifest build")
        sources = tuple(
            require_text(value, "split source")
            for value in require_list(build.get("sources"), "split sources")
        )
        headers = tuple(
            require_text(value, "split private header")
            for value in require_list(
                build.get("privateHeaders"), "split private headers"
            )
        )
        if (
            configuration.get("projectLayout") != "split"
            or "src/hxc/support.c" not in sources
            or "src/hxc/main.c" not in sources
            or len(sources) < 3
            or len(headers) < 3
        ):
            raise VirtualDispatchFailure(
                "split virtual-dispatch manifest omitted module/support ownership"
            )
        dispatch = load_json(output / "hxc.dispatch.json", "split dispatch")
        layouts = {
            require_text(layout.get("id"), "split layout ID"): require_text(
                layout.get("cTag"), "split layout C tag"
            )
            for layout in (
                require_dict(value, "split layout")
                for value in require_list(dispatch.get("layouts"), "split layouts")
            )
        }
        types_header = (
            output / "include/hxc/detail/program_types.h"
        ).read_text(encoding="utf-8")
        support_source = (output / "src/hxc/support.c").read_text(encoding="utf-8")
        for value in require_list(dispatch.get("tables"), "split tables"):
            table = require_dict(value, "split table")
            c_name = require_text(table.get("cName"), "split table C name")
            layout_id = require_text(table.get("layoutId"), "split table layout")
            c_tag = layouts.get(layout_id)
            if c_tag is None:
                raise VirtualDispatchFailure(
                    f"split table {c_name!r} references unknown layout {layout_id!r}"
                )
            declaration = f"extern const struct {c_tag} {c_name};"
            definition = f"const struct {c_tag} {c_name} ="
            if (
                declaration not in types_header
                or definition not in support_source
                or f"static {definition}" in support_source
            ):
                raise VirtualDispatchFailure(
                    f"split table {c_name!r} lost cross-unit declaration/linkage"
                )

        project = CFixtureProject(
            "virtual-dispatch-split",
            sources,
            headers,
            ("include",),
            "",
            (*sorted(REQUIRED_NATIVE_COVERAGE), "split-project-layout"),
        )
        for optimization in ("-O0", "-O2"):
            report = run_c_fixture_corpus(
                suite=f"virtual-dispatch-split-{optimization[1:].lower()}",
                projects=(project,),
                fixture_root=output,
                build_root=root / f"build-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            validate_report(
                report,
                required_coverage=REQUIRED_NATIVE_COVERAGE
                | {"split-project-layout"},
            )


def snapshot_project() -> dict[str, object]:
    return {
        "program.h": (EXPECTED / "program.h").read_text(encoding="utf-8"),
        "program.c": (EXPECTED / "program.c").read_text(encoding="utf-8"),
    }


def interface_snapshot_project() -> dict[str, object]:
    return {
        "interface-program.h": (EXPECTED / "interface-program.h").read_text(
            encoding="utf-8"
        ),
        "interface-program.c": (EXPECTED / "interface-program.c").read_text(
            encoding="utf-8"
        ),
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    try:
        validate_schema_document()
        if args.native_only:
            check_native(snapshot_project(), requested_toolchain=args.toolchain)
            check_interface_native(
                interface_snapshot_project(), requested_toolchain=args.toolchain
            )
            print("virtual-dispatch: OK: required class/interface native matrix passed")
            return 0
        with haxe_server() as endpoint:
            project = check_determinism_and_policy(connect=endpoint)
            interface_project = check_interface_determinism(connect=endpoint)
            check_snapshots(project, interface_project)
            check_negative_cases(connect=endpoint)
            warm_after_rejection = render(
                "warm-server render after rejected overrides", connect=endpoint
            )
            assert_artifacts_equal(
                project.artifacts,
                warm_after_rejection.artifacts,
                "warm server after rejected override requests",
            )
            if project.inspection_payload != warm_after_rejection.inspection_payload:
                raise VirtualDispatchFailure(
                    "warm-server dispatch inspection changed after rejected requests"
                )
        check_native(snapshot_values(project), requested_toolchain=args.toolchain)
        check_interface_native(
            interface_snapshot_values(interface_project),
            requested_toolchain=args.toolchain,
        )
        check_split_project(requested_toolchain=args.toolchain)
    except (
        VirtualDispatchFailure,
        CFixtureFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"virtual-dispatch: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "virtual-dispatch: OK: direct-call preservation, minimal root-only tables, "
        "inherited/multiple-interface tables, deterministic slots, "
        "representation-checked overrides, explanatory reports, "
        "runtime-free strict C11/C++17 split/unity consumers, and fail-closed variance passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

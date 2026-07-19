#!/usr/bin/env python3
"""Check or explicitly update every catalog-owned snapshot with semantic diffs."""

from __future__ import annotations

import argparse
import difflib
import importlib.util
import json
import os
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from types import ModuleType
from typing import Any, Callable, Iterable


ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "docs/specs/fixture-taxonomy.json"


class SnapshotFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Artifact:
    path: Path
    format: str
    value: object


@dataclass(frozen=True)
class Change:
    suite: str
    path: Path
    format: str
    before: object | None
    after: object | None


Generator = Callable[[], list[Artifact]]


def load_module(name: str, relative: str) -> ModuleType:
    path = ROOT / relative
    spec = importlib.util.spec_from_file_location(f"hxc_snapshot_{name}", path)
    if spec is None or spec.loader is None:
        raise SnapshotFailure(f"cannot load snapshot driver {relative}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def bootstrap_artifacts() -> list[Artifact]:
    module = load_module("bootstrap", "test/bootstrap/run.py")
    first = module.run(
        module.source_command(c_build=True, target_report=True),
        cwd=module.FIXTURE,
        label="snapshot first bootstrap target contract",
    )
    second = module.run(
        module.source_command(c_build=True, target_report=True),
        cwd=module.FIXTURE,
        label="snapshot second bootstrap target contract",
    )
    report = module.extract_target_report(
        first, "snapshot first bootstrap target contract"
    )
    repeated = module.extract_target_report(
        second, "snapshot second bootstrap target contract"
    )
    if report != repeated:
        raise SnapshotFailure("two bootstrap target-contract renders differed")
    return [
        Artifact(
            Path("test/bootstrap/expected/target-contract.json"),
            "json",
            report,
        )
    ]


def hxc_configuration_artifacts() -> list[Artifact]:
    module = load_module("hxc_configuration", "test/hxc_config/run.py")
    first = module.render_snapshot()
    repeated = module.render_snapshot(reverse=True)
    if first != repeated:
        raise SnapshotFailure(
            "hxc configuration snapshot changed with direct-define input order"
        )
    return [
        Artifact(
            Path("test/hxc_config/expected/effective-config.json"),
            "json",
            first,
        )
    ]


def typed_c_artifacts() -> list[Artifact]:
    module = load_module("typed_c", "test/typed_c/run.py")
    forward = module.compile_fixture("positive", "Main", report=True)
    reverse = module.compile_fixture("positive", "MainReverse", report=True)
    repeated = module.compile_fixture("positive", "Main", report=True)
    module.require_success(forward, "snapshot forward-order typed C fixture")
    module.require_success(reverse, "snapshot reverse-order typed C fixture")
    module.require_success(repeated, "snapshot repeated typed C fixture")
    forward_payload, forward_report = module.extract_report(
        forward, "snapshot forward-order typed C fixture"
    )
    reverse_payload, _ = module.extract_report(
        reverse, "snapshot reverse-order typed C fixture"
    )
    repeated_payload, repeated_report = module.extract_report(
        repeated, "snapshot repeated typed C fixture"
    )
    if forward_payload != reverse_payload:
        raise SnapshotFailure(
            "typed C snapshot changed with root-module reference order"
        )
    if forward_payload != repeated_payload or forward_report != repeated_report:
        raise SnapshotFailure("two typed C snapshot renders differed")
    return [
        Artifact(
            Path("test/typed_c/expected/typed-c-contract.json"),
            "json",
            forward_report,
        )
    ]


def c_import_artifacts() -> list[Artifact]:
    module = load_module("c_import", "test/c_import/run.py")
    first = module.render_snapshot()
    second = module.render_snapshot()
    if first != second:
        raise SnapshotFailure("two C-import snapshot renders were not byte-identical")
    formats = {
        "program.h": "header",
        "program.c": "c",
        "build.json": "json",
        "runtime-plan.json": "json",
    }
    artifacts: list[Artifact] = []
    for name, format_name in formats.items():
        value = first.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(f"C-import snapshot omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"C-import snapshot omitted {name}")
        artifacts.append(
            Artifact(Path("test/c_import/expected") / name, format_name, value)
        )
    return artifacts


def raylib_provisioning_artifacts() -> list[Artifact]:
    module = load_module(
        "raylib_provisioning", "test/raylib_provisioning/run.py"
    )
    first = module.snapshot_values()
    second = module.snapshot_values()
    if first != second:
        raise SnapshotFailure(
            "two raylib-provisioning snapshot renders were not byte-identical"
        )
    formats = {
        "include/hxc/program.h": "header",
        "src/program.c": "c",
        "hxc.runtime-plan.json": "json",
        "build-linux-memory.json": "json",
        "build-linux-desktop.json": "json",
        "build-macos-desktop.json": "json",
        "build-windows-desktop.json": "json",
        "build-system-desktop.json": "json",
    }
    artifacts: list[Artifact] = []
    for name, format_name in formats.items():
        value = first.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(
                    f"raylib-provisioning snapshot omitted {name}"
                )
        elif not isinstance(value, str):
            raise SnapshotFailure(f"raylib-provisioning snapshot omitted {name}")
        artifacts.append(
            Artifact(
                Path("test/raylib_provisioning/expected") / name,
                format_name,
                value,
            )
        )
    binding = load_module("raylib_core_binding", "scripts/raylib/core_binding.py")
    lock = binding.load_lock()
    raw_first = binding.render_files(lock)
    raw_second = binding.render_files(lock)
    if raw_first != raw_second:
        raise SnapshotFailure("two raylib raw-binding renders were not byte-identical")
    for name, value in raw_first.items():
        artifacts.append(Artifact(Path("src/raylib/raw") / name, "text", value))
    probe_first = binding.render_abi_probe(lock)
    probe_second = binding.render_abi_probe(lock)
    if probe_first != probe_second:
        raise SnapshotFailure("two raylib core ABI-probe renders were not byte-identical")
    artifacts.append(
        Artifact(
            Path("test/raylib_provisioning/native/core_abi_probe.c"),
            "c",
            probe_first,
        )
    )
    return artifacts


def typed_ast_artifacts() -> list[Artifact]:
    module = load_module("typed_ast", "test/typed_ast/run.py")
    result = module.render_snapshot()
    return [
        Artifact(
            Path("test/typed_ast/expected/typed-ast-inventory.json"),
            "json",
            result.report,
        )
    ]


def c_ast_artifacts() -> list[Artifact]:
    module = load_module("c_ast", "test/c_ast/run.py")
    return [
        Artifact(artifact.relative_path, artifact.format, artifact.value)
        for artifact in module.render_snapshot()
    ]


def declaration_plan_artifacts() -> list[Artifact]:
    module = load_module("declaration_plan", "test/declaration_plan/run.py")
    first_payload, report = module.render("snapshot first declaration plan render")
    second_payload, _ = module.render("snapshot second declaration plan render")
    if first_payload != second_payload:
        raise SnapshotFailure(
            "two declaration-plan snapshot renders were not byte-identical"
        )

    artifacts = [
        Artifact(
            Path("test/declaration_plan/expected/plan.json"),
            "json",
            {
                "plan": report.get("plan"),
                "diagnostics": report.get("diagnostics"),
            },
        )
    ]
    files = report.get("files")
    if not isinstance(files, list):
        raise SnapshotFailure("declaration-plan report omitted generated headers")
    seen: set[str] = set()
    for entry in files:
        if (
            not isinstance(entry, dict)
            or not isinstance(entry.get("path"), str)
            or not isinstance(entry.get("content"), str)
        ):
            raise SnapshotFailure(f"invalid declaration header entry: {entry!r}")
        relative = entry["path"]
        if relative in seen:
            raise SnapshotFailure(f"duplicate declaration header path: {relative}")
        seen.add(relative)
        artifacts.append(
            Artifact(
                Path("test/declaration_plan/expected/include") / relative,
                "header",
                entry["content"],
            )
        )
    return artifacts


def symbol_registry_artifacts() -> list[Artifact]:
    module = load_module("symbol_registry", "test/symbol_registry/run.py")
    first_payload, report = module.render("snapshot first symbol registry render")
    second_payload, repeated = module.render("snapshot second symbol registry render")
    if first_payload != second_payload or report != repeated:
        raise SnapshotFailure("two symbol-registry snapshot renders were not byte-identical")
    module.validate(report)
    return [
        Artifact(
            Path("test/symbol_registry/expected/symbol-registry.json"),
            "json",
            report,
        )
    ]


def project_emitter_artifacts() -> list[Artifact]:
    module = load_module("project_emitter", "test/project_emitter/run.py")
    first = module.render_snapshot()
    second = module.render_snapshot()
    if first != second:
        raise SnapshotFailure(
            "two project-emitter snapshot renders were not byte-identical"
        )
    return [
        Artifact(
            Path("test/project_emitter/expected") / artifact.relative_path,
            artifact.format,
            artifact.value,
        )
        for artifact in first
    ]


def hxc_ir_artifacts() -> list[Artifact]:
    module = load_module("hxc_ir", "test/hxc_ir/run.py")
    module.check_oracle()
    first_payload, report = module.render("snapshot first HxcIR render")
    second_payload, _ = module.render("snapshot second HxcIR render")
    if first_payload != second_payload:
        raise SnapshotFailure("two HxcIR snapshot renders were not byte-identical")

    semantic = report.get("semantic")
    coverage = report.get("coverage")
    diagnostics = report.get("diagnostics")
    if not isinstance(semantic, str) or not isinstance(coverage, str):
        raise SnapshotFailure("HxcIR report omitted canonical text dumps")
    if not isinstance(diagnostics, dict):
        raise SnapshotFailure("HxcIR report omitted diagnostic JSON")
    return [
        Artifact(
            Path("test/hxc_ir/expected/semantic.hxcir"), "hxcir", semantic
        ),
        Artifact(
            Path("test/hxc_ir/expected/coverage.hxcir"), "hxcir", coverage
        ),
        Artifact(
            Path("test/hxc_ir/expected/diagnostics.json"), "json", diagnostics
        ),
    ]


def primitive_semantics_artifacts() -> list[Artifact]:
    module = load_module(
        "primitive_semantics", "test/primitive_semantics/run.py"
    )
    first = module.render("snapshot first primitive semantic render")
    second = module.render("snapshot second primitive semantic render")
    if (
        first.contract_payload != second.contract_payload
        or first.type_probe_payload != second.type_probe_payload
    ):
        raise SnapshotFailure(
            "two primitive semantic renders were not byte-identical"
        )
    module.validate_contract(first.contract)
    module.validate_type_probe(first.type_probe)
    module.validate_schema_document()
    return [
        Artifact(
            Path("docs/specs/primitive-semantics.json"),
            "json",
            first.contract,
        )
    ]


def stdlib_ledger_artifacts() -> list[Artifact]:
    module = load_module("stdlib_ledger", "test/stdlib_ledger/run.py")
    rendered = module.render_artifacts()
    return [
        Artifact(Path("docs/specs/stdlib-ledger.json"), "json", rendered.ledger),
        Artifact(Path("docs/specs/stdlib-ledger.csv"), "text", rendered.csv),
    ]


def body_lowering_artifacts() -> list[Artifact]:
    module = load_module("body_lowering", "test/body_lowering/run.py")
    first_payload, report = module.render("snapshot first body-lowering render")
    second_payload, repeated = module.render("snapshot second body-lowering render")
    reverse_payload, reversed_report = module.render(
        "snapshot reverse-input body-lowering render", reverse=True
    )
    if (
        first_payload != second_payload
        or report != repeated
        or first_payload != reverse_payload
        or report != reversed_report
    ):
        raise SnapshotFailure(
            "body-lowering snapshot changed across repeated or reverse-input renders"
        )
    module.validate(report)
    hxcir = report.get("hxcir")
    c_source = report.get("cSource")
    line_mapped = report.get("lineMappedCSource")
    symbols = report.get("symbols")
    if (
        not isinstance(hxcir, str)
        or not isinstance(c_source, str)
        or not isinstance(line_mapped, str)
        or not isinstance(symbols, dict)
    ):
        raise SnapshotFailure("body-lowering report omitted a managed artifact")
    return [
        Artifact(Path("test/body_lowering/expected/body.hxcir"), "hxcir", hxcir),
        Artifact(Path("test/body_lowering/expected/body.c"), "c", c_source),
        Artifact(
            Path("test/body_lowering/expected/body-lines.c"), "c", line_mapped
        ),
        Artifact(Path("test/body_lowering/expected/symbols.json"), "json", symbols),
    ]


def function_lowering_artifacts() -> list[Artifact]:
    module = load_module("function_lowering", "test/function_lowering/run.py")
    first_payload, report = module.render("snapshot first function render")
    second_payload, repeated = module.render("snapshot second function render")
    reverse_payload, reversed_report = module.render(
        "snapshot reverse-input function render", reverse=True
    )
    if (
        first_payload != second_payload
        or report != repeated
        or first_payload != reverse_payload
        or report != reversed_report
    ):
        raise SnapshotFailure(
            "function-lowering snapshot changed across repeated or reverse-input renders"
        )
    module.validate(report)
    values = module.snapshot_values(report)
    hxcir = values.get("functions.hxcir")
    header = values.get("functions.h")
    symbols = values.get("symbols.json")
    if (
        not isinstance(hxcir, str)
        or not isinstance(header, str)
        or not isinstance(symbols, dict)
    ):
        raise SnapshotFailure("function-lowering report omitted a managed artifact")
    artifacts = [
        Artifact(
            Path("test/function_lowering/expected/functions.hxcir"),
            "hxcir",
            hxcir,
        ),
        Artifact(
            Path("test/function_lowering/expected/functions.h"),
            "header",
            header,
        ),
        Artifact(
            Path("test/function_lowering/expected/symbols.json"),
            "json",
            symbols,
        ),
    ]
    for snapshot_name in module.SOURCE_SNAPSHOTS:
        source = values.get(snapshot_name)
        if not isinstance(source, str):
            raise SnapshotFailure(
                f"function-lowering report omitted source snapshot {snapshot_name}"
            )
        artifacts.append(
            Artifact(
                Path("test/function_lowering/expected") / snapshot_name,
                "c",
                source,
            )
        )
    return artifacts


def aggregate_lowering_artifacts() -> list[Artifact]:
    module = load_module("aggregate_lowering", "test/aggregate_lowering/run.py")
    first_payload, report = module.render("snapshot first aggregate render")
    second_payload, repeated = module.render("snapshot second aggregate render")
    reverse_payload, reversed_report = module.render(
        "snapshot reverse-input aggregate render", reverse=True
    )
    if (
        first_payload != second_payload
        or report != repeated
        or first_payload != reverse_payload
        or report != reversed_report
    ):
        raise SnapshotFailure(
            "aggregate-lowering snapshot changed across repeated or reverse-input renders"
        )
    module.validate(report)
    values = module.snapshot_values(report)
    artifacts: list[Artifact] = []
    for name, format_name in (
        ("aggregates.hxcir", "hxcir"),
        ("program.h", "header"),
        ("program.c", "c"),
        ("aggregates.json", "json"),
        ("symbols.json", "json"),
    ):
        value = values.get(name)
        if format_name == "json":
            if not isinstance(value, (dict, list)):
                raise SnapshotFailure(f"aggregate-lowering report omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"aggregate-lowering report omitted {name}")
        artifacts.append(
            Artifact(
                Path("test/aggregate_lowering/expected") / name,
                format_name,
                value,
            )
        )
    return artifacts


def class_layout_artifacts() -> list[Artifact]:
    module = load_module("class_layout", "test/class_layout/run.py")
    first_payload, report = module.render("snapshot first class-layout render")
    second_payload, repeated = module.render("snapshot second class-layout render")
    reverse_payload, reversed_report = module.render(
        "snapshot reverse-input class-layout render", reverse=True
    )
    if (
        first_payload != second_payload
        or report != repeated
        or first_payload != reverse_payload
        or report != reversed_report
    ):
        raise SnapshotFailure(
            "class-layout snapshot changed across repeated or reverse-input renders"
        )
    module.validate(report)
    values = module.snapshot_values(report)
    artifacts: list[Artifact] = []
    for name, format_name in (
        ("classes.hxcir", "hxcir"),
        ("program.h", "header"),
        ("program.c", "c"),
        ("classes.json", "json"),
        ("functions.json", "json"),
        ("symbols.json", "json"),
    ):
        value = values.get(name)
        if format_name == "json":
            if not isinstance(value, (dict, list)):
                raise SnapshotFailure(f"class-layout report omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"class-layout report omitted {name}")
        artifacts.append(
            Artifact(Path("test/class_layout/expected") / name, format_name, value)
        )
    return artifacts


def constructor_lowering_artifacts() -> list[Artifact]:
    module = load_module("constructor_lowering", "test/constructor_lowering/run.py")
    first_payload, report = module.render("snapshot first constructor render")
    second_payload, repeated = module.render("snapshot second constructor render")
    reverse_payload, reversed_report = module.render(
        "snapshot reverse-input constructor render", reverse=True
    )
    if (
        first_payload != second_payload
        or report != repeated
        or first_payload != reverse_payload
        or report != reversed_report
    ):
        raise SnapshotFailure(
            "constructor snapshot changed across repeated or reverse-input renders"
        )
    module.validate(report)
    values = module.snapshot_values(report)
    artifacts: list[Artifact] = []
    for name, format_name in (
        ("constructors.hxcir", "hxcir"),
        ("constructors.json", "json"),
        ("program.h", "header"),
        ("program.c", "c"),
        ("symbols.json", "json"),
    ):
        value = values.get(name)
        if format_name == "json":
            if not isinstance(value, (dict, list)):
                raise SnapshotFailure(f"constructor report omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"constructor report omitted {name}")
        artifacts.append(
            Artifact(Path("test/constructor_lowering/expected") / name, format_name, value)
        )
    return artifacts


def virtual_dispatch_artifacts() -> list[Artifact]:
    module = load_module("virtual_dispatch", "test/virtual_dispatch/run.py")
    values = module.render_snapshot()
    artifacts: list[Artifact] = []
    for name, format_name in (
        ("dispatch.hxcir", "hxcir"),
        ("hxc.dispatch.json", "json"),
        ("program.h", "header"),
        ("program.c", "c"),
        ("symbols.json", "json"),
    ):
        value = values.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(f"virtual-dispatch report omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"virtual-dispatch report omitted {name}")
        artifacts.append(
            Artifact(Path("test/virtual_dispatch/expected") / name, format_name, value)
        )
    return artifacts


def enum_lowering_artifacts() -> list[Artifact]:
    module = load_module("enum_lowering", "test/enum_lowering/run.py")
    first_payload, report = module.render("snapshot first enum render")
    second_payload, repeated = module.render("snapshot second enum render")
    reverse_payload, reversed_report = module.render(
        "snapshot reverse-input enum render", reverse=True
    )
    if (
        first_payload != second_payload
        or report != repeated
        or first_payload != reverse_payload
        or report != reversed_report
    ):
        raise SnapshotFailure(
            "enum-lowering snapshot changed across repeated or reverse-input renders"
        )
    module.validate(report)
    values = module.snapshot_values(report)
    artifacts: list[Artifact] = []
    for name, format_name in (
        ("enums.hxcir", "hxcir"),
        ("program.h", "header"),
        ("program.c", "c"),
        ("enums.json", "json"),
        ("symbols.json", "json"),
    ):
        value = values.get(name)
        if format_name == "json":
            if not isinstance(value, (dict, list)):
                raise SnapshotFailure(f"enum-lowering report omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"enum-lowering report omitted {name}")
        artifacts.append(
            Artifact(
                Path("test/enum_lowering/expected") / name,
                format_name,
                value,
            )
        )
    return artifacts


def generic_specialization_artifacts() -> list[Artifact]:
    module = load_module(
        "generic_specialization", "test/generic_specialization/run.py"
    )
    first = module.render_snapshot()
    second = module.render_snapshot()
    if first != second:
        raise SnapshotFailure(
            "two generic-specialization snapshot renders were not identical"
        )
    return [
        Artifact(
            Path("test/generic_specialization/expected/hxc.specializations.json"),
            "json",
            first,
        )
    ]


def evaluation_order_artifacts() -> list[Artifact]:
    module = load_module("evaluation_order", "test/evaluation_order/run.py")
    first_payload, report = module.render("snapshot first evaluation-order render")
    second_payload, repeated = module.render("snapshot second evaluation-order render")
    reverse_payload, reversed_report = module.render(
        "snapshot reverse-input evaluation-order render", reverse=True
    )
    if (
        first_payload != second_payload
        or report != repeated
        or first_payload != reverse_payload
        or report != reversed_report
    ):
        raise SnapshotFailure(
            "evaluation-order snapshot changed across repeated or reverse-input renders"
        )
    module.validate(report)
    values = module.snapshot_values(report)
    artifacts: list[Artifact] = []
    for name, format_name in (
        ("evaluation.hxcir", "hxcir"),
        ("program.h", "header"),
        ("program.c", "c"),
        ("symbols.json", "json"),
    ):
        value = values.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(f"evaluation-order report omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"evaluation-order report omitted {name}")
        artifacts.append(
            Artifact(Path("test/evaluation_order/expected") / name, format_name, value)
        )
    return artifacts


def static_initialization_artifacts() -> list[Artifact]:
    module = load_module(
        "static_initialization", "test/static_initialization/run.py"
    )
    first = module.snapshot_values()
    second = module.snapshot_values()
    if first != second:
        raise SnapshotFailure(
            "two static-initialization snapshot renders were not byte-identical"
        )
    artifacts: list[Artifact] = []
    for name, format_name in (
        ("initialization-plan.json", "json"),
        ("initialization.hxcir", "hxcir"),
        ("program.h", "header"),
        ("program.c", "c"),
    ):
        value = first.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(f"static-initialization report omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"static-initialization report omitted {name}")
        artifacts.append(
            Artifact(
                Path("test/static_initialization/expected") / name,
                format_name,
                value,
            )
        )
    return artifacts


def arithmetic_semantics_artifacts() -> list[Artifact]:
    module = load_module(
        "arithmetic_semantics", "test/arithmetic_semantics/run.py"
    )
    first_payload, report = module.render("snapshot first arithmetic render")
    second_payload, repeated = module.render("snapshot second arithmetic render")
    reverse_payload, reversed_report = module.render(
        "snapshot reverse-input arithmetic render", reverse=True
    )
    if (
        first_payload != second_payload
        or report != repeated
        or first_payload != reverse_payload
        or report != reversed_report
    ):
        raise SnapshotFailure(
            "arithmetic snapshot changed across repeated or reverse-input renders"
        )
    module.validate(report)
    values = module.snapshot_values(report)
    artifacts: list[Artifact] = []
    for name, format_name in (
        ("arithmetic.hxcir", "hxcir"),
        ("program.h", "header"),
        ("program.c", "c"),
        ("contract.json", "json"),
        ("symbols.json", "json"),
    ):
        value = values.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(f"arithmetic report omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"arithmetic report omitted {name}")
        artifacts.append(
            Artifact(
                Path("test/arithmetic_semantics/expected") / name,
                format_name,
                value,
            )
        )
    return artifacts


def primitive_differential_artifacts() -> list[Artifact]:
    module = load_module(
        "primitive_differential", "test/primitive_differential/run.py"
    )
    first = module.snapshot_values()
    second = module.snapshot_values()
    if first != second:
        raise SnapshotFailure(
            "two primitive-differential snapshot renders were not byte-identical"
        )
    formats = {
        "corpus.json": "json",
        "PrimitiveDifferentialFixture.hx": "text",
        "oracle.txt": "text",
        "divergence-oracle.txt": "text",
        "program.h": "header",
        "program.c": "c",
        "symbols.json": "json",
        "runtime-plan.json": "json",
    }
    artifacts: list[Artifact] = []
    for name, format_name in formats.items():
        value = first.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(
                    f"primitive-differential snapshot omitted {name}"
                )
        elif not isinstance(value, str):
            raise SnapshotFailure(f"primitive-differential snapshot omitted {name}")
        artifacts.append(
            Artifact(
                Path("test/primitive_differential/expected") / name,
                format_name,
                value,
            )
        )
    return artifacts


def span_lowering_artifacts() -> list[Artifact]:
    module = load_module("span_lowering", "test/span_lowering/run.py")
    first_payload, report = module.render("snapshot first span-lowering render")
    second_payload, repeated = module.render("snapshot second span-lowering render")
    reverse_payload, reversed_report = module.render(
        "snapshot reverse-input span-lowering render", reverse=True
    )
    if (
        first_payload != second_payload
        or report != repeated
        or first_payload != reverse_payload
        or report != reversed_report
    ):
        raise SnapshotFailure(
            "span-lowering snapshot changed across repeated or reverse-input renders"
        )
    module.validate(report, profile="portable", build="debug")
    values = {
        "span.hxcir": report.get("hxcir"),
        "symbols.json": report.get("symbols"),
        **module.production_snapshot(),
    }
    artifacts: list[Artifact] = []
    for name, format_name in (
        ("span.hxcir", "hxcir"),
        ("program.h", "header"),
        ("program.c", "c"),
        ("symbols.json", "json"),
    ):
        value = values.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(f"span-lowering report omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"span-lowering report omitted {name}")
        artifacts.append(
            Artifact(Path("test/span_lowering/expected") / name, format_name, value)
        )
    return artifacts


def runtime_feature_graph_artifacts() -> list[Artifact]:
    module = load_module(
        "runtime_feature_graph", "test/runtime/runtime-feature-graph/run.py"
    )
    rendered = module.render_reports()
    module.validate_schema_document()
    module.validate_catalog(rendered.catalog)
    module.validate_plans(rendered.plans)
    module.validate_package(rendered.package, rendered.plans)
    return [
        Artifact(Path("runtime/hxrt/features.json"), "json", rendered.catalog),
        Artifact(
            Path("test/runtime/runtime-feature-graph/expected/runtime-feature-plans.json"),
            "json",
            rendered.plans,
        ),
    ]


def string_output_artifacts() -> list[Artifact]:
    module = load_module("string_output", "test/string_output/run.py")
    first = module.snapshot_values()
    second = module.snapshot_values()
    if first != second:
        raise SnapshotFailure("two literal string-output snapshot renders were not byte-identical")
    artifacts: list[Artifact] = []
    formats = {
        "output.hxcir": "hxcir",
        "program.h": "header",
        "program.c": "c",
        "runtime-plan.json": "json",
        "stdlib-report.json": "json",
    }
    for name, format_name in formats.items():
        value = first.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(f"string-output snapshot omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"string-output snapshot omitted {name}")
        artifacts.append(Artifact(Path("test/string_output/expected") / name, format_name, value))
    return artifacts


def hello_artifacts() -> list[Artifact]:
    module = load_module("hello", "examples/hello/run.py")
    first = module.snapshot_values()
    second = module.snapshot_values()
    if first != second:
        raise SnapshotFailure("two hello snapshot renders were not byte-identical")
    artifacts: list[Artifact] = []
    formats = {
        "hello.hxcir": "hxcir",
        "include/hxc/program.h": "header",
        "src/program.c": "c",
        "hxc.runtime-plan.json": "json",
        "hxc.stdlib-report.json": "json",
    }
    for name, format_name in formats.items():
        value = first.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(f"hello snapshot omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"hello snapshot omitted {name}")
        artifacts.append(
            Artifact(Path("examples/hello/expected") / name, format_name, value)
        )
    return artifacts


def caxecraft_domain_artifacts() -> list[Artifact]:
    module = load_module("caxecraft_domain", "examples/caxecraft/run.py")
    first = module.snapshot_values()
    second = module.snapshot_values()
    if first != second:
        raise SnapshotFailure(
            "two Caxecraft-domain snapshot renders were not byte-identical"
        )
    formats = module.SNAPSHOT_FORMATS
    if not isinstance(formats, dict) or not formats:
        raise SnapshotFailure("Caxecraft-domain snapshot formats are malformed")
    artifacts: list[Artifact] = []
    for name, format_name in formats.items():
        value = first.get(name)
        if format_name == "json":
            if not isinstance(value, dict):
                raise SnapshotFailure(f"Caxecraft-domain snapshot omitted {name}")
        elif not isinstance(value, str):
            raise SnapshotFailure(f"Caxecraft-domain snapshot omitted {name}")
        artifacts.append(
            Artifact(Path("examples/caxecraft/expected") / name, format_name, value)
        )
    return artifacts


GENERATORS: dict[str, Generator] = {
    "bootstrap": bootstrap_artifacts,
    "hxc-configuration": hxc_configuration_artifacts,
    "typed-c": typed_c_artifacts,
    "c-import": c_import_artifacts,
    "raylib-provisioning": raylib_provisioning_artifacts,
    "typed-ast": typed_ast_artifacts,
    "c-ast": c_ast_artifacts,
    "declaration-plan": declaration_plan_artifacts,
    "symbol-registry": symbol_registry_artifacts,
    "project-emitter": project_emitter_artifacts,
    "hxc-ir": hxc_ir_artifacts,
    "primitive-semantics": primitive_semantics_artifacts,
    "stdlib-ledger": stdlib_ledger_artifacts,
    "body-lowering": body_lowering_artifacts,
    "function-lowering": function_lowering_artifacts,
    "aggregate-lowering": aggregate_lowering_artifacts,
    "class-layout": class_layout_artifacts,
    "constructor-lowering": constructor_lowering_artifacts,
    "virtual-dispatch": virtual_dispatch_artifacts,
    "enum-lowering": enum_lowering_artifacts,
    "generic-specialization": generic_specialization_artifacts,
    "evaluation-order": evaluation_order_artifacts,
    "static-initialization": static_initialization_artifacts,
    "arithmetic-semantics": arithmetic_semantics_artifacts,
    "primitive-differential": primitive_differential_artifacts,
    "runtime-feature-graph": runtime_feature_graph_artifacts,
    "string-output": string_output_artifacts,
    "hello": hello_artifacts,
    "caxecraft-domain": caxecraft_domain_artifacts,
    "span-lowering": span_lowering_artifacts,
}


def registered_suite_ids() -> tuple[str, ...]:
    return tuple(GENERATORS)


def read_catalog() -> dict[str, Any]:
    try:
        value = json.loads(CATALOG.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise SnapshotFailure(f"cannot read fixture catalog: {error}") from error
    if not isinstance(value, dict):
        raise SnapshotFailure("fixture catalog must be a JSON object")
    return value


def managed_entries(catalog: dict[str, Any]) -> dict[str, dict[str, Any]]:
    policy = catalog.get("snapshotPolicy")
    entries = policy.get("managedSuites") if isinstance(policy, dict) else None
    if not isinstance(entries, list):
        raise SnapshotFailure("fixture catalog omitted snapshot managedSuites")
    result: dict[str, dict[str, Any]] = {}
    for entry in entries:
        if not isinstance(entry, dict) or not isinstance(entry.get("id"), str):
            raise SnapshotFailure(f"invalid managed snapshot suite: {entry!r}")
        identifier = entry["id"]
        if identifier in result:
            raise SnapshotFailure(f"duplicate managed snapshot suite: {identifier}")
        result[identifier] = entry
    if set(result) != set(GENERATORS):
        raise SnapshotFailure(
            "catalog/runner snapshot suite mismatch: "
            f"catalog={sorted(result)!r}, runner={sorted(GENERATORS)!r}"
        )
    return result


def safe_repo_path(value: str) -> Path:
    if not value or "\\" in value:
        raise SnapshotFailure(f"snapshot path must be normalized POSIX relative: {value!r}")
    relative = Path(value)
    if relative.is_absolute() or any(part in ("", ".", "..") for part in relative.parts):
        raise SnapshotFailure(f"snapshot path must be normalized POSIX relative: {value!r}")
    candidate = ROOT / relative
    try:
        candidate.resolve(strict=False).relative_to(ROOT.resolve())
    except ValueError as error:
        raise SnapshotFailure(f"snapshot path escapes repository: {value!r}") from error
    return candidate


def entry_roots(entry: dict[str, Any]) -> list[Path]:
    values = entry.get("expectedRoots")
    if not isinstance(values, list) or not values:
        raise SnapshotFailure(f"snapshot suite {entry.get('id')!r} has no expected roots")
    roots: list[Path] = []
    for value in values:
        if not isinstance(value, str):
            raise SnapshotFailure(f"invalid expected root: {value!r}")
        roots.append(safe_repo_path(value))
    return roots


def path_is_owned(path: Path, roots: list[Path]) -> bool:
    resolved = path.resolve(strict=False)
    for root in roots:
        root_resolved = root.resolve(strict=False)
        if resolved == root_resolved:
            return True
        if root.is_dir():
            try:
                resolved.relative_to(root_resolved)
                return True
            except ValueError:
                pass
    return False


def expected_files(roots: list[Path]) -> set[Path]:
    files: set[Path] = set()
    for root in roots:
        if root.is_file() or root.is_symlink():
            files.add(root)
        elif root.is_dir():
            files.update(path for path in root.rglob("*") if path.is_file())
    return files


def read_expected(path: Path, format_name: str) -> object:
    if format_name == "json":
        try:
            return json.loads(path.read_text(encoding="utf-8"))
        except json.JSONDecodeError as error:
            raise SnapshotFailure(
                f"expected JSON snapshot is invalid: {path.relative_to(ROOT)}: {error}"
            ) from error
    return path.read_text(encoding="utf-8")


def canonical_lines(value: object | None, format_name: str) -> list[str]:
    if value is None:
        return []
    if format_name == "json":
        return (
            json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True) + "\n"
        ).splitlines(keepends=True)
    if not isinstance(value, str):
        raise SnapshotFailure(f"text snapshot value must be text, got {type(value).__name__}")
    return value.splitlines(keepends=True)


def render_diff(change: Change) -> str:
    relative = change.path.relative_to(ROOT).as_posix()
    before_name = f"expected/{relative}" if change.before is not None else "/dev/null"
    after_name = f"actual/{relative}" if change.after is not None else "/dev/null"
    return "".join(
        difflib.unified_diff(
            canonical_lines(change.before, change.format),
            canonical_lines(change.after, change.format),
            fromfile=before_name,
            tofile=after_name,
        )
    )


def collect_changes(
    suite: str, entry: dict[str, Any], artifacts: list[Artifact]
) -> list[Change]:
    roots = entry_roots(entry)
    formats = entry.get("formats")
    if not isinstance(formats, list) or not all(
        isinstance(format_name, str) and format_name for format_name in formats
    ):
        raise SnapshotFailure(f"snapshot suite {suite} has no declared formats")
    allowed_formats = set(formats)
    actual: dict[Path, Artifact] = {}
    for artifact in artifacts:
        path = safe_repo_path(artifact.path.as_posix())
        if artifact.format not in allowed_formats:
            raise SnapshotFailure(
                f"generator {suite} produced undeclared format {artifact.format!r}: "
                f"{artifact.path.as_posix()}"
            )
        if not path_is_owned(path, roots):
            raise SnapshotFailure(
                f"generator {suite} tried to write outside its expected roots: "
                f"{artifact.path.as_posix()}"
            )
        if path in actual:
            raise SnapshotFailure(
                f"generator {suite} produced duplicate artifact {artifact.path.as_posix()}"
            )
        actual[path] = Artifact(path.relative_to(ROOT), artifact.format, artifact.value)

    changes: list[Change] = []
    for path, artifact in sorted(actual.items(), key=lambda item: item[0].as_posix()):
        before = read_expected(path, artifact.format) if path.is_file() else None
        if before != artifact.value:
            changes.append(
                Change(suite, path, artifact.format, before, artifact.value)
            )

    for path in sorted(expected_files(roots) - set(actual), key=lambda item: item.as_posix()):
        format_name = {
            ".c": "c",
            ".h": "header",
            ".hxcir": "hxcir",
            ".json": "json",
        }.get(path.suffix, "text")
        if format_name not in allowed_formats:
            raise SnapshotFailure(
                f"snapshot suite {suite} owns an undeclared stale format "
                f"{format_name!r}: {path.relative_to(ROOT)}"
            )
        changes.append(
            Change(suite, path, format_name, read_expected(path, format_name), None)
        )
    return changes


def ensure_safe_write(path: Path) -> None:
    current = path
    while current != ROOT:
        if current.is_symlink():
            raise SnapshotFailure(
                f"refusing to update snapshot through symlink: {current.relative_to(ROOT)}"
            )
        current = current.parent


def write_artifact(change: Change) -> None:
    ensure_safe_write(change.path)
    if change.after is None:
        change.path.unlink()
        return
    change.path.parent.mkdir(parents=True, exist_ok=True)
    if change.format == "json":
        contents = json.dumps(change.after, ensure_ascii=False, indent=2) + "\n"
    else:
        if not isinstance(change.after, str):
            raise SnapshotFailure("text snapshot update received a non-text value")
        contents = change.after
    descriptor, temporary_name = tempfile.mkstemp(
        dir=change.path.parent,
        prefix=f".{change.path.name}.",
        suffix=".tmp",
        text=True,
    )
    temporary = Path(temporary_name)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="") as handle:
            handle.write(contents)
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(temporary, change.path)
    finally:
        if temporary.exists():
            temporary.unlink()


def validate_updated_suites(catalog: dict[str, Any], suites: list[str]) -> None:
    raw_suites = catalog.get("suites")
    if not isinstance(raw_suites, list):
        raise SnapshotFailure("fixture catalog omitted suite runners")
    commands = {
        entry.get("id"): entry.get("runner")
        for entry in raw_suites
        if isinstance(entry, dict)
    }
    for suite in suites:
        command = commands.get(suite)
        if not isinstance(command, list) or not all(
            isinstance(part, str) and part for part in command
        ):
            raise SnapshotFailure(f"snapshot suite {suite} has no validation runner")
        print(f"snapshot-validate: {suite}: {' '.join(command)}")
        result = subprocess.run(command, cwd=ROOT, check=False, timeout=300)
        if result.returncode != 0:
            raise SnapshotFailure(
                f"updated snapshot suite {suite} failed validation with exit {result.returncode}"
            )


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--check", action="store_true", help="compare without writing (default)")
    mode.add_argument("--update", action="store_true", help="print diffs, then update expected files")
    mode.add_argument("--list", action="store_true", help="list registered snapshot suites")
    mode.add_argument("--list-json", action="store_true", help=argparse.SUPPRESS)
    parser.add_argument("--suite", action="append", default=[], help="select one suite; repeatable")
    parser.add_argument("--all", action="store_true", help="select every suite explicitly")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        catalog = read_catalog()
        entries = managed_entries(catalog)
        if args.list or args.list_json:
            if args.suite or args.all or args.update or args.check:
                raise SnapshotFailure("--list cannot be combined with selection or check/update")
            if args.list_json:
                print(json.dumps({"suites": list(entries)}, separators=(",", ":")))
            else:
                for identifier, entry in entries.items():
                    roots = ", ".join(
                        path.relative_to(ROOT).as_posix()
                        for path in entry_roots(entry)
                    )
                    print(f"{identifier}: {roots}")
            return 0

        if args.all and args.suite:
            raise SnapshotFailure("use either --all or --suite, not both")
        unknown = sorted(set(args.suite) - set(entries))
        if unknown:
            raise SnapshotFailure("unknown snapshot suite(s): " + ", ".join(unknown))
        if args.update and not args.all and not args.suite:
            raise SnapshotFailure("update mode requires --suite <id> or explicit --all")
        if args.update and os.environ.get("CI", "").lower() not in ("", "0", "false", "no"):
            raise SnapshotFailure("snapshot update mode is forbidden when CI is set")

        selected = list(entries) if args.all or not args.suite else list(dict.fromkeys(args.suite))
        changes: list[Change] = []
        for suite in selected:
            print(f"snapshot-render: {suite}")
            changes.extend(collect_changes(suite, entries[suite], GENERATORS[suite]()))

        if not changes:
            print(f"snapshots: OK: {len(selected)} suite(s) matched")
            return 0

        for change in changes:
            relative = change.path.relative_to(ROOT).as_posix()
            print(f"snapshot-diff: {change.suite}: {relative}")
            difference = render_diff(change)
            print(difference, end="" if difference.endswith("\n") else "\n")

        if not args.update:
            selectors = " ".join(f"--suite {suite}" for suite in selected)
            print(
                "snapshots: ERROR: semantic drift detected; review it, then run "
                f"npm run snapshots:update -- {selectors}",
                file=sys.stderr,
            )
            return 1

        for change in changes:
            write_artifact(change)
        validate_updated_suites(catalog, selected)
        print(
            f"snapshots: UPDATED: {len(changes)} artifact(s) across {len(selected)} suite(s); "
            "review git diff before committing"
        )
        return 0
    except (
        SnapshotFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"snapshots: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

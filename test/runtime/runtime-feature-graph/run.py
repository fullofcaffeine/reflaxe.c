#!/usr/bin/env python3
"""Prove deterministic runtime closure, policy validation, and exact packaging."""

from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[3]
CASE = Path(__file__).resolve().parent
HXML = CASE / "runtime_feature_graph.hxml"
CATALOG_EXPECTED = ROOT / "runtime/hxrt/features.json"
PLANS_EXPECTED = CASE / "expected/runtime-feature-plans.json"
CATALOG_SCHEMA = ROOT / "docs/specs/runtime-features.schema.json"
RUNTIME_SOURCE_ROOT = ROOT / "runtime/hxrt"
ALLOC_CONSUMER = CASE / "alloc_consumer.c"
ARRAY_CONSUMER = CASE / "array_consumer.c"
BYTES_CONSUMER = CASE / "bytes_consumer.c"
OBJECT_CONSUMER = CASE / "object_consumer.c"
GC_CONSUMER = ROOT / "runtime/hxrt/test/gc_contract.c"
STRING_CONSUMER = CASE / "string_consumer.c"
IO_CONSUMER = CASE / "io_consumer.c"
CATALOG_PREFIX = "HXC_RUNTIME_FEATURE_CATALOG="
PLANS_PREFIX = "HXC_RUNTIME_FEATURE_PLANS="
PACKAGE_PREFIX = "HXC_RUNTIME_FEATURE_PACKAGE="
TOOLCHAINS = ("gcc", "clang")
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)


class RuntimeFeatureFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class RuntimeRender:
    catalog_payload: str
    plans_payload: str
    package_payload: str
    catalog: dict[str, object]
    plans: dict[str, object]
    package: dict[str, object]


@dataclass(frozen=True)
class Toolchain:
    family: str
    compiler: str
    version: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def extract_record(stdout: str, prefix: str, label: str) -> tuple[str, dict[str, object]]:
    values = [line[len(prefix) :] for line in stdout.splitlines() if line.startswith(prefix)]
    if len(values) != 1:
        raise RuntimeFeatureFailure(
            f"{label} emitted {len(values)} records for {prefix!r}\nstdout:\n{stdout}"
        )
    try:
        parsed = json.loads(values[0])
    except json.JSONDecodeError as error:
        raise RuntimeFeatureFailure(f"{label} emitted malformed JSON for {prefix}: {error}") from error
    if not isinstance(parsed, dict):
        raise RuntimeFeatureFailure(f"{label} record {prefix} must be an object")
    return values[0], parsed


def render(label: str) -> RuntimeRender:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [development_tool("haxe"), str(HXML)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if result.returncode != 0 or result.stderr:
        raise RuntimeFeatureFailure(
            f"{label} failed with {result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    catalog_payload, catalog = extract_record(result.stdout, CATALOG_PREFIX, label)
    plans_payload, plans = extract_record(result.stdout, PLANS_PREFIX, label)
    package_payload, package = extract_record(result.stdout, PACKAGE_PREFIX, label)
    return RuntimeRender(catalog_payload, plans_payload, package_payload, catalog, plans, package)


def render_reports() -> RuntimeRender:
    first = render("first runtime feature render")
    second = render("second runtime feature render")
    if (
        first.catalog_payload != second.catalog_payload
        or first.plans_payload != second.plans_payload
        or first.package_payload != second.package_payload
    ):
        raise RuntimeFeatureFailure("two runtime feature renders were not byte-identical")
    return first


def record(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict) or not all(isinstance(key, str) for key in value):
        raise RuntimeFeatureFailure(f"{label} must be an object")
    return value


def records(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise RuntimeFeatureFailure(f"{label} must be an array")
    return value


def text_list(value: object, label: str) -> list[str]:
    values = records(value, label)
    if not all(isinstance(entry, str) for entry in values):
        raise RuntimeFeatureFailure(f"{label} must contain only strings")
    return [entry for entry in values if isinstance(entry, str)]


def validate_schema_document() -> None:
    schema = record(json.loads(CATALOG_SCHEMA.read_text(encoding="utf-8")), "runtime feature schema")
    if schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        raise RuntimeFeatureFailure("runtime feature schema must use JSON Schema 2020-12")
    properties = record(schema.get("properties"), "runtime feature schema properties")
    if record(properties.get("schemaVersion"), "schemaVersion schema").get("const") != 3:
        raise RuntimeFeatureFailure("runtime feature schema must require version 3")
    for required in ("algorithm", "features", "reservedFeatures", "noUnconditionalCore", "runtimeAbi"):
        if required not in properties:
            raise RuntimeFeatureFailure(f"runtime feature schema omitted {required}")


def validate_catalog(catalog: dict[str, object]) -> None:
    if catalog.get("schemaVersion") != 3 or catalog.get("algorithm") != "hxc-runtime-feature-graph-v3":
        raise RuntimeFeatureFailure("runtime feature catalog schema or algorithm drifted")
    if catalog.get("status") != "selective-compiler-packaging":
        raise RuntimeFeatureFailure("runtime feature catalog readiness drifted")
    if catalog.get("noUnconditionalCore") is not True or catalog.get("compilerSelectableFeatures") != [
        "alloc",
        "array",
        "bytes",
        "gc",
        "io",
        "object",
        "runtime-base",
        "status",
        "string-literal",
    ]:
        raise RuntimeFeatureFailure("catalog compiler-selectable feature inventory drifted")
    runtime_abi = record(catalog.get("runtimeAbi"), "runtime ABI contract")
    version = record(runtime_abi.get("version"), "runtime ABI version")
    public_boundary = record(runtime_abi.get("publicBoundary"), "runtime public boundary")
    provenance = record(runtime_abi.get("releaseProvenance"), "runtime release provenance")
    if (
        runtime_abi.get("stability") != "internal-versioned"
        or version != {"major": 0, "minor": 8, "patch": 0}
        or runtime_abi.get("generatedCodeCompatibility") != "same-major"
        or runtime_abi.get("generatedCodeCheck") != "c11-static-assert"
        or runtime_abi.get("runtimeMajorMacro") != "HXC_RUNTIME_ABI_MAJOR"
        or public_boundary.get("applicationAbiStatus") != "unsupported"
        or public_boundary.get("exportTypePolicy") != "runtime-structs-forbidden"
        or provenance.get("sourceHashAlgorithm") != "sha256"
        or provenance.get("runtimeCStandard") != "c11"
        or provenance.get("publicHeaderCppStandard") != "c++17"
    ):
        raise RuntimeFeatureFailure("runtime ABI compatibility or public-boundary policy drifted")
    forbidden_types = text_list(public_boundary.get("forbiddenRuntimeTypes"), "forbidden runtime export types")
    declared_runtime_structs: set[str] = set()
    for header in (ROOT / "runtime/hxrt/include/hxrt").glob("*.h"):
        header_text = header.read_text(encoding="utf-8")
        declared_runtime_structs.update(re.findall(r"typedef struct (hxc_[A-Za-z0-9_]+)\s*\{", header_text))
        declared_runtime_structs.update(
            match.group(1)
            for match in re.finditer(
                r"typedef struct (hxc_[A-Za-z0-9_]+)\s+(hxc_[A-Za-z0-9_]+)\s*;",
                header_text,
            )
            if match.group(1) == match.group(2)
        )
    expected_forbidden_types = sorted(declared_runtime_structs, key=lambda value: value.encode("utf-8"))
    if forbidden_types != expected_forbidden_types:
        raise RuntimeFeatureFailure("runtime-private application export inventory drifted")
    feature_values = records(catalog.get("features"), "catalog features")
    features = {str(record(value, "feature").get("id")): record(value, "feature") for value in feature_values}
    if set(features) != {
        "runtime-base",
        "runtime-abi",
        "status",
        "status-name",
        "alloc",
        "array",
        "bytes",
        "gc",
        "object",
        "string-literal",
        "string",
        "io",
    }:
        raise RuntimeFeatureFailure(f"selective feature set drifted: {sorted(features)!r}")
    expected_dependencies = {
        "runtime-base": [],
        "runtime-abi": ["runtime-base"],
        "status": ["runtime-base"],
        "status-name": ["status"],
        "alloc": ["status"],
        "array": ["alloc"],
        "bytes": ["alloc", "string-literal"],
        "gc": ["alloc", "object"],
        "object": ["runtime-base"],
        "string-literal": ["runtime-base"],
        "string": ["alloc", "string-literal"],
        "io": ["status", "string-literal"],
    }
    expected_availability = {
        "runtime-base": "compiler-selectable",
        "runtime-abi": "native-seed-only",
        "status": "compiler-selectable",
        "status-name": "native-seed-only",
        "alloc": "compiler-selectable",
        "array": "compiler-selectable",
        "bytes": "compiler-selectable",
        "gc": "compiler-selectable",
        "object": "compiler-selectable",
        "string-literal": "compiler-selectable",
        "string": "native-seed-only",
        "io": "compiler-selectable",
    }
    source_records: list[tuple[str, str]] = []
    registered_sources: set[str] = set()
    registered_symbols: set[str] = set()
    for identifier, dependencies in expected_dependencies.items():
        feature = features[identifier]
        if feature.get("availability") != expected_availability[identifier] or feature.get("dependencies") != dependencies:
            raise RuntimeFeatureFailure(f"feature {identifier} availability/dependencies drifted")
        if feature.get("minimalAllowed") is not True:
            raise RuntimeFeatureFailure(f"seed feature {identifier} left the narrow allowlist")
        documentation = record(feature.get("documentation"), f"feature {identifier} documentation")
        expected_documentation_fields = {
            "contract",
            "selectionRoots",
            "directAlternative",
            "programLocalAlternative",
            "runtimeRationale",
            "referencePath",
            "evidence",
        }
        if set(documentation) != expected_documentation_fields:
            raise RuntimeFeatureFailure(f"feature {identifier} documentation fields drifted")
        for field in (
            "contract",
            "directAlternative",
            "programLocalAlternative",
            "runtimeRationale",
        ):
            if not isinstance(documentation.get(field), str) or not str(documentation.get(field)).strip():
                raise RuntimeFeatureFailure(f"feature {identifier} omitted documented {field}")
        reference_path = documentation.get("referencePath")
        if not isinstance(reference_path, str):
            raise RuntimeFeatureFailure(f"feature {identifier} omitted its documentation reference")
        reference = safe_output_path(ROOT, reference_path)
        if not reference.is_file() or f"<!-- hxrt-feature:{identifier} -->" not in reference.read_text(encoding="utf-8"):
            raise RuntimeFeatureFailure(f"feature {identifier} documentation reference is missing or stale")
        evidence = text_list(documentation.get("evidence"), f"feature {identifier} evidence")
        if not evidence:
            raise RuntimeFeatureFailure(f"feature {identifier} has no executable evidence")
        for evidence_path in evidence:
            evidence_file = safe_output_path(ROOT, evidence_path)
            if not evidence_file.is_file() or not evidence_path.startswith(
                ("test/", "runtime/hxrt/test/", "scripts/ci/", "examples/")
            ):
                raise RuntimeFeatureFailure(
                    f"feature {identifier} evidence is missing or not executable repository evidence: {evidence_path}"
                )
        roots = [
            record(value, f"feature {identifier} selection root")
            for value in records(documentation.get("selectionRoots"), f"feature {identifier} selection roots")
        ]
        if not roots or len({root.get("id") for root in roots}) != len(roots):
            raise RuntimeFeatureFailure(f"feature {identifier} selection roots are absent or duplicated")
        for root in roots:
            if set(root) != {"id", "kind", "description"} or not root.get("description"):
                raise RuntimeFeatureFailure(f"feature {identifier} has an incomplete selection root")
            kind = root.get("kind")
            if kind == "hxc-ir-operation" and expected_availability[identifier] != "compiler-selectable":
                raise RuntimeFeatureFailure(f"native feature {identifier} advertised a compiler root")
            if kind == "native-seed-fixture" and expected_availability[identifier] != "native-seed-only":
                raise RuntimeFeatureFailure(f"compiler feature {identifier} advertised a native-only root")
            if kind not in ("hxc-ir-operation", "transitive-dependency", "native-seed-fixture"):
                raise RuntimeFeatureFailure(f"feature {identifier} has an unknown selection-root kind")
        registered_symbols.update(text_list(feature.get("symbols"), f"feature {identifier} symbols"))
        for value in records(feature.get("artifacts"), f"feature {identifier} artifacts"):
            artifact = record(value, f"feature {identifier} artifact")
            source_path = artifact.get("sourcePath")
            source_sha256 = artifact.get("sourceSha256")
            if not isinstance(source_path, str) or not isinstance(source_sha256, str):
                raise RuntimeFeatureFailure(f"feature {identifier} artifact lost source provenance")
            source = safe_output_path(ROOT, source_path)
            source_text = source.read_text(encoding="utf-8")
            ownership_marker = (
                f"hxrt feature: {identifier}"
                if source.suffix == ".h"
                else f"feature `{identifier}`"
            )
            if ownership_marker not in source_text[:1600]:
                raise RuntimeFeatureFailure(
                    f"feature {identifier} artifact lacks its file-level source contract: {source_path}"
                )
            if hashlib.sha256(source.read_bytes()).hexdigest() != source_sha256:
                raise RuntimeFeatureFailure(f"feature {identifier} artifact source digest drifted: {source_path}")
            source_records.append((source_path, source_sha256))
            registered_sources.add(source_path)
    if [root.get("id") for root in records(record(features["io"].get("documentation"), "io documentation").get("selectionRoots"), "io roots")] != [
        "sys-println-literal",
        "trace-literal",
    ]:
        raise RuntimeFeatureFailure("io documented HxcIR roots drifted")
    production_sources = {
        str(path.relative_to(ROOT))
        for directory, suffix in ((RUNTIME_SOURCE_ROOT / "include/hxrt", "*.h"), (RUNTIME_SOURCE_ROOT / "src", "*.c"))
        for path in directory.glob(suffix)
    }
    if registered_sources != production_sources:
        raise RuntimeFeatureFailure(
            "production hxrt artifact ownership drifted: "
            f"missing={sorted(production_sources - registered_sources)!r} "
            f"stale={sorted(registered_sources - production_sources)!r}"
        )
    umbrella = RUNTIME_SOURCE_ROOT / "include/hxc_runtime.h"
    if (
        not umbrella.is_file()
        or "not a compiler-selectable hxrt feature" not in umbrella.read_text(encoding="utf-8")[:1600]
        or str(umbrella.relative_to(ROOT)) in registered_sources
    ):
        raise RuntimeFeatureFailure("the provisional hxc_runtime.h umbrella classification drifted")
    declared_symbols: set[str] = set()
    for header in (RUNTIME_SOURCE_ROOT / "include/hxrt").glob("*.h"):
        declared_symbols.update(
            re.findall(
                r"\bHXC_API\b[^;]*?\b(hxc_[A-Za-z0-9_]+)\s*\(",
                header.read_text(encoding="utf-8"),
                flags=re.DOTALL,
            )
        )
    if registered_symbols != declared_symbols:
        raise RuntimeFeatureFailure(
            "runtime public symbol documentation drifted: "
            f"missing={sorted(declared_symbols - registered_symbols)!r} "
            f"stale={sorted(registered_symbols - declared_symbols)!r}"
        )
    source_records.sort(key=lambda entry: entry[0].encode("utf-8"))
    source_set_payload = "".join(f"{path}\0{digest}\n" for path, digest in source_records).encode("utf-8")
    if provenance.get("sourceSetSha256") != hashlib.sha256(source_set_payload).hexdigest():
        raise RuntimeFeatureFailure("runtime release source-set digest drifted")
    reserved = {
        str(record(value, "reserved feature").get("id"))
        for value in records(catalog.get("reservedFeatures"), "reserved features")
    }
    for required in ("dynamic", "reflection", "exception", "thread"):
        if required not in reserved:
            raise RuntimeFeatureFailure(f"catalog omitted reserved independent feature {required}")
    if "io" in reserved:
        raise RuntimeFeatureFailure("compiler-selectable io remains reserved")
    serialized = json.dumps(catalog, sort_keys=True, ensure_ascii=False)
    if str(ROOT) in serialized or "/Users/" in serialized or "\\" in serialized:
        raise RuntimeFeatureFailure("runtime feature catalog leaked a host path")


def validate_selected_reasons(plan: dict[str, object], label: str) -> None:
    root_ids: set[str] = set()
    for value in records(plan.get("rootReasons"), f"{label} rootReasons"):
        reason = record(value, f"{label} root reason")
        reason_id = reason.get("id")
        operation_id = reason.get("operationId")
        source = record(reason.get("source"), f"{label} root reason source")
        start = record(source.get("start"), f"{label} root reason start")
        file_name = source.get("file")
        line_number = start.get("line")
        if (
            not isinstance(reason_id, str)
            or not isinstance(operation_id, str)
            or not isinstance(file_name, str)
            or not isinstance(line_number, int)
        ):
            raise RuntimeFeatureFailure(f"{label} root reason omitted its typed source anchor")
        source_path = safe_output_path(ROOT, file_name)
        source_lines = source_path.read_text(encoding="utf-8").splitlines()
        if line_number < 1 or line_number > len(source_lines) or reason_id not in source_lines[line_number - 1]:
            raise RuntimeFeatureFailure(f"{label} reason {reason_id} does not point at its declared source call")
        root_ids.add(reason_id)
    if not root_ids:
        raise RuntimeFeatureFailure(f"{label} selected features without root reasons")
    for value in records(plan.get("selectedFeatures"), f"{label} selectedFeatures"):
        feature = record(value, f"{label} selected feature")
        reason_ids = set(text_list(feature.get("reasonIds"), f"{label} feature reasonIds"))
        if not reason_ids or not reason_ids.issubset(root_ids):
            raise RuntimeFeatureFailure(f"{label} feature {feature.get('id')} lost source-root provenance")
    for value in records(plan.get("dependencyEdges"), f"{label} dependencyEdges"):
        edge = record(value, f"{label} dependency edge")
        reason_ids = set(text_list(edge.get("reasonIds"), f"{label} edge reasonIds"))
        if not reason_ids or not reason_ids.issubset(root_ids):
            raise RuntimeFeatureFailure(f"{label} dependency edge lost source-root provenance")


def validate_no_runtime_proof(plan: dict[str, object], label: str) -> None:
    proof = record(plan.get("noRuntimeProof"), f"{label} noRuntimeProof")
    reachability = record(proof.get("reachability"), f"{label} reachability")
    runtime_absence = record(proof.get("runtimeAbsence"), f"{label} runtime absence")
    if (
        proof.get("schemaVersion") != 1
        or proof.get("algorithm") != "hxc-no-runtime-eligibility-v1"
        or proof.get("status") != "eligible"
        or proof.get("scope") != "reachable-whole-program"
        or not proof.get("semanticProof")
        or reachability
        != {
            "modules": 1,
            "typeInstances": 0,
            "functions": 1,
            "blocks": 1,
            "instructions": 1,
            "cleanupActions": 0,
            "runtimeIntents": 0,
        }
        or proof.get("directDecisions") != plan.get("directDecisions")
        or proof.get("programLocalHelpers") != []
        or runtime_absence
        != {
            "features": [],
            "includes": [],
            "sources": [],
            "defines": [],
            "libraries": [],
            "symbols": [],
        }
    ):
        raise RuntimeFeatureFailure(f"{label} structured no-runtime proof drifted")


def validate_plans(plans: dict[str, object]) -> None:
    empty = record(plans.get("empty"), "empty plan")
    if (
        empty.get("schemaVersion") != 2
        or empty.get("algorithm") != "hxc-runtime-plan-v2"
        or empty.get("status") != "analyzed-runtime-free"
        or empty.get("planPurpose") != "compiler-program"
        or empty.get("features") != []
        or empty.get("artifacts") != []
        or empty.get("symbols") != []
        or not empty.get("noRuntimeProof")
    ):
        raise RuntimeFeatureFailure("empty compiler plan did not prove complete hxrt absence")
    validate_no_runtime_proof(empty, "empty plan")

    alloc = record(plans.get("alloc"), "alloc plan")
    array = record(plans.get("array"), "array plan")
    bytes_plan = record(plans.get("bytes"), "bytes plan")
    object_plan = record(plans.get("object"), "object plan")
    gc_plan = record(plans.get("gc"), "gc plan")
    string = record(plans.get("string"), "string plan")
    minimal = record(plans.get("minimalString"), "minimal string plan")
    compiler_io = record(plans.get("compilerIo"), "compiler io plan")
    if alloc.get("features") != ["runtime-base", "status", "alloc"]:
        raise RuntimeFeatureFailure("alloc closure is incomplete or nondeterministic")
    if array.get("features") != ["runtime-base", "status", "alloc", "array"]:
        raise RuntimeFeatureFailure("array closure is incomplete or nondeterministic")
    if bytes_plan.get("features") != ["runtime-base", "status", "alloc", "string-literal", "bytes"]:
        raise RuntimeFeatureFailure("Bytes closure is incomplete or nondeterministic")
    if object_plan.get("features") != ["runtime-base", "object"]:
        raise RuntimeFeatureFailure("object descriptor closure is incomplete or nondeterministic")
    if gc_plan.get("features") != ["runtime-base", "status", "alloc", "object", "gc"]:
        raise RuntimeFeatureFailure("collector closure is incomplete or nondeterministic")
    if string.get("features") != ["runtime-base", "status", "alloc", "string-literal", "string"]:
        raise RuntimeFeatureFailure("string closure is incomplete or nondeterministic")
    validate_selected_reasons(alloc, "alloc")
    validate_selected_reasons(array, "array")
    validate_selected_reasons(bytes_plan, "Bytes")
    validate_selected_reasons(object_plan, "object")
    validate_selected_reasons(gc_plan, "gc")
    validate_selected_reasons(string, "string")
    validate_selected_reasons(minimal, "minimal string")
    if (
        compiler_io.get("features") != ["runtime-base", "status", "string-literal", "io"]
        or compiler_io.get("status") != "analyzed-runtime-features"
        or compiler_io.get("planPurpose") != "compiler-program"
    ):
        raise RuntimeFeatureFailure("compiler io closure or purpose drifted")
    validate_selected_reasons(compiler_io, "compiler io")
    if "runtime/src/io.c" not in text_list(compiler_io.get("artifacts"), "compiler io artifacts"):
        raise RuntimeFeatureFailure("compiler io plan omitted its selected source")
    if "hxc_io_println" not in text_list(compiler_io.get("symbols"), "compiler io symbols"):
        raise RuntimeFeatureFailure("compiler io plan omitted its selected symbol")

    alloc_artifacts = text_list(alloc.get("artifacts"), "alloc artifacts")
    alloc_symbols = text_list(alloc.get("symbols"), "alloc symbols")
    if any("string" in path for path in alloc_artifacts) or any("string" in symbol for symbol in alloc_symbols):
        raise RuntimeFeatureFailure("alloc build plan retained an unselected string artifact or symbol")
    if "runtime/src/string.c" not in text_list(string.get("artifacts"), "string artifacts"):
        raise RuntimeFeatureFailure("string build plan omitted its selected source")
    if "runtime/src/array.c" not in text_list(array.get("artifacts"), "array artifacts"):
        raise RuntimeFeatureFailure("array build plan omitted its selected source")
    if "runtime/src/bytes.c" not in text_list(bytes_plan.get("artifacts"), "Bytes artifacts"):
        raise RuntimeFeatureFailure("Bytes build plan omitted its selected source")
    if "runtime/src/object.c" not in text_list(object_plan.get("artifacts"), "object artifacts"):
        raise RuntimeFeatureFailure("object build plan omitted its selected source")
    if "runtime/src/gc.c" not in text_list(gc_plan.get("artifacts"), "gc artifacts"):
        raise RuntimeFeatureFailure("collector build plan omitted its selected source")
    if "hxc_array_resize" not in text_list(array.get("symbols"), "array symbols"):
        raise RuntimeFeatureFailure("array build plan omitted its selected symbol")
    if "hxc_string_copy" not in text_list(string.get("symbols"), "string symbols"):
        raise RuntimeFeatureFailure("string build plan omitted its selected symbol")
    overrides = records(minimal.get("manualOverrides"), "minimal overrides")
    if (
        minimal.get("resolvedPolicy") != "minimal"
        or len(overrides) != 1
        or record(overrides[0], "minimal override").get("action") != "require"
    ):
        raise RuntimeFeatureFailure("valid manual confirmation lost policy provenance")

    synthetic = record(plans.get("syntheticLink"), "synthetic link plan")
    validate_selected_reasons(synthetic, "synthetic link")
    if synthetic.get("libraries") != ["m"] or synthetic.get("defines") != ["HXC_FIXTURE_LINK=1"]:
        raise RuntimeFeatureFailure("feature graph did not carry link and define selection")

    diagnostics = record(plans.get("diagnostics"), "runtime diagnostics")
    expected_ids = {
        "invalidId": "HXC9000",
        "cycle": "HXC9000",
        "unknownDependency": "HXC9000",
        "missingNoRuntimeProof": "HXC9000",
        "survivingRuntimeIntentProof": "HXC9000",
        "nonePolicy": "HXC2000",
        "minimalPolicy": "HXC2000",
        "unusedManualRequire": "HXC2000",
        "forbidRequired": "HXC2000",
        "compilerNativeSeed": "HXC2000",
        "reservedFeature": "HXC2000",
        "environment": "HXC2000",
        "tamperedPackage": "HXC9000",
        "tamperedSourceContent": "HXC9000",
        "tamperedNoRuntimeProof": "HXC9000",
    }
    for name, identifier in expected_ids.items():
        diagnostic = record(diagnostics.get(name), f"diagnostic {name}")
        if diagnostic.get("id") != identifier or not diagnostic.get("message"):
            raise RuntimeFeatureFailure(f"runtime diagnostic {name} drifted")
    cycle = record(diagnostics.get("cycle"), "cycle diagnostic")
    if cycle.get("featureIds") != ["cycle-a", "cycle-b", "cycle-a"]:
        raise RuntimeFeatureFailure("cycle path is not canonical")
    none_policy = record(diagnostics.get("nonePolicy"), "runtime-none diagnostic")
    blockers = [
        record(value, f"runtime-none blocker[{index}]")
        for index, value in enumerate(records(none_policy.get("blockers"), "runtime-none blockers"))
    ]
    if [blocker.get("id") for blocker in blockers] != ["fixture.none.a", "fixture.none.z"]:
        raise RuntimeFeatureFailure("runtime-none blockers are incomplete or not sorted by stable root ID")
    if blockers[0].get("dependencyChains") != [
        ["string", "alloc", "status", "runtime-base"],
        ["string", "string-literal", "runtime-base"],
    ] or blockers[1].get("dependencyChains") != [["alloc", "status", "runtime-base"]]:
        raise RuntimeFeatureFailure("runtime-none blockers lost dependency-chain provenance")
    for blocker in blockers:
        source = record(blocker.get("source"), "runtime-none blocker source")
        if (
            blocker.get("operationId") != "fixture-runtime-operation"
            or blocker.get("kind") != "fixture-semantic-gap"
            or blocker.get("surface") != "fixture.RuntimeFeatureGraph"
            or not blocker.get("alternative")
            or not isinstance(source.get("file"), str)
            or "\\" in str(source.get("file"))
        ):
            raise RuntimeFeatureFailure("runtime-none blocker lost typed source or semantic provenance")
    for name in expected_ids:
        diagnostic = record(diagnostics.get(name), f"diagnostic {name}")
        if name != "nonePolicy" and diagnostic.get("blockers") != []:
            raise RuntimeFeatureFailure(f"non-aggregate diagnostic {name} unexpectedly contains blockers")

    serialized = json.dumps(plans, sort_keys=True, ensure_ascii=False)
    if str(ROOT) in serialized or "/Users/" in serialized:
        raise RuntimeFeatureFailure("runtime feature plans leaked a host path")


def validate_package(package: dict[str, object], plans: dict[str, object]) -> None:
    for name in ("alloc", "array", "bytes", "object", "gc", "string", "io"):
        plan_key = "compilerIo" if name == "io" else name
        plan = record(plans.get(plan_key), f"{name} plan")
        expected_paths = text_list(plan.get("artifacts"), f"{name} plan artifacts")
        values = records(package.get(name), f"{name} package")
        actual_paths: list[str] = []
        for index, value in enumerate(values):
            entry = record(value, f"{name} package[{index}]")
            path = entry.get("path")
            contents = entry.get("contents")
            digest = entry.get("sha256")
            if not isinstance(path, str) or not isinstance(contents, str) or not isinstance(digest, str):
                raise RuntimeFeatureFailure(f"{name} package entry is incomplete")
            if hashlib.sha256(contents.encode("utf-8")).hexdigest() != digest:
                raise RuntimeFeatureFailure(f"{name} package digest mismatch for {path}")
            actual_paths.append(path)
        if actual_paths != expected_paths:
            raise RuntimeFeatureFailure(f"{name} package differs from selected artifact plan")


def semantic_snapshot(path: Path, actual: dict[str, object], label: str) -> None:
    if not path.is_file():
        raise RuntimeFeatureFailure(
            f"{label} snapshot is missing; use npm run snapshots:update -- --suite runtime-feature-graph"
        )
    expected = json.loads(path.read_text(encoding="utf-8"))
    if expected != actual:
        diff = "".join(
            difflib.unified_diff(
                (json.dumps(expected, indent=2, sort_keys=True, ensure_ascii=False) + "\n").splitlines(keepends=True),
                (json.dumps(actual, indent=2, sort_keys=True, ensure_ascii=False) + "\n").splitlines(keepends=True),
                fromfile=str(path.relative_to(ROOT)),
                tofile=f"actual {label}",
            )
        )
        raise RuntimeFeatureFailure(f"{label} snapshot drifted\n{diff}")


def compiler_identity(executable: str) -> tuple[str, str]:
    result = subprocess.run(
        [executable, "--version"], cwd=ROOT, check=False, capture_output=True, text=True, timeout=30
    )
    if result.returncode != 0:
        raise RuntimeFeatureFailure(f"cannot identify compiler {executable}")
    output = (result.stdout + result.stderr).strip()
    lowered = output.lower()
    family = "clang" if "clang" in lowered else "gcc" if "gcc" in lowered or "free software foundation" in lowered else "unknown"
    return family, output.splitlines()[0] if output else "unknown version"


def resolve_toolchain(family: str, required: bool) -> Toolchain | None:
    executable = shutil.which(family)
    if executable is None:
        if required:
            raise RuntimeFeatureFailure(f"required runtime feature compiler is missing: {family}")
        print(f"runtime-feature-graph: SKIP optional {family}: missing command")
        return None
    actual, version = compiler_identity(executable)
    if actual != family:
        if required:
            raise RuntimeFeatureFailure(f"required {family} command identifies as {actual}: {version}")
        print(f"runtime-feature-graph: SKIP optional {family}: command identifies as {actual}")
        return None
    return Toolchain(family, executable, version)


def selected_toolchains(selected: str) -> list[Toolchain]:
    families = list(TOOLCHAINS) if selected == "auto" else [selected]
    result = [
        toolchain
        for family in families
        if (toolchain := resolve_toolchain(family, required=selected != "auto")) is not None
    ]
    if not result:
        raise RuntimeFeatureFailure("no strict C11 compiler is available for runtime feature packaging")
    return result


def safe_output_path(root: Path, relative: str) -> Path:
    if not relative or "\\" in relative:
        raise RuntimeFeatureFailure(f"invalid packaged runtime path: {relative!r}")
    path = Path(relative)
    if path.is_absolute() or any(part in ("", ".", "..") for part in path.parts):
        raise RuntimeFeatureFailure(f"invalid packaged runtime path: {relative!r}")
    output = root / path
    output.resolve(strict=False).relative_to(root.resolve())
    return output


def load_snapshot(path: Path, label: str) -> dict[str, object]:
    if not path.is_file():
        raise RuntimeFeatureFailure(f"{label} snapshot is missing: {path.relative_to(ROOT)}")
    return record(json.loads(path.read_text(encoding="utf-8")), f"{label} snapshot")


def selected_artifacts_from_snapshot(
    catalog: dict[str, object], plan: dict[str, object], label: str
) -> list[dict[str, object]]:
    purpose = plan.get("planPurpose")
    expected_status = (
        "analyzed-runtime-features"
        if purpose == "compiler-program"
        else "analyzed-native-seed-features"
        if purpose == "native-seed-fixture"
        else None
    )
    if (
        plan.get("schemaVersion") != 2
        or plan.get("algorithm") != "hxc-runtime-plan-v2"
        or plan.get("status") != expected_status
        or plan.get("noRuntimeProof") is not None
    ):
        raise RuntimeFeatureFailure(f"{label} is not a packageable selected-feature plan")
    feature_by_id: dict[str, dict[str, object]] = {}
    for value in records(catalog.get("features"), "catalog features"):
        feature = record(value, "catalog feature")
        identifier = feature.get("id")
        if not isinstance(identifier, str) or identifier in feature_by_id:
            raise RuntimeFeatureFailure("catalog feature IDs must be unique strings")
        feature_by_id[identifier] = feature

    selected = text_list(plan.get("features"), f"{label} features")
    positions: dict[str, int] = {}
    expected: list[dict[str, object]] = []
    for index, identifier in enumerate(selected):
        if identifier in positions:
            raise RuntimeFeatureFailure(f"{label} repeats selected feature {identifier}")
        feature = feature_by_id.get(identifier)
        if feature is None:
            raise RuntimeFeatureFailure(f"{label} selects unknown feature {identifier}")
        availability = feature.get("availability")
        if purpose == "compiler-program" and availability != "compiler-selectable":
            raise RuntimeFeatureFailure(f"{label} selects a non-compiler feature {identifier}")
        if purpose == "native-seed-fixture" and availability not in ("compiler-selectable", "native-seed-only"):
            raise RuntimeFeatureFailure(f"{label} selects a feature with unknown availability {identifier}")
        positions[identifier] = index
        for value in records(feature.get("artifacts"), f"feature {identifier} artifacts"):
            artifact = record(value, f"feature {identifier} artifact")
            source_path = artifact.get("sourcePath")
            output_path = artifact.get("outputPath")
            kind = artifact.get("kind")
            source_sha256 = artifact.get("sourceSha256")
            if not all(isinstance(item, str) for item in (source_path, output_path, kind, source_sha256)):
                raise RuntimeFeatureFailure(f"feature {identifier} has an incomplete artifact")
            assert isinstance(source_path, str)
            assert isinstance(output_path, str)
            assert isinstance(kind, str)
            assert isinstance(source_sha256, str)
            source = safe_output_path(ROOT, source_path)
            try:
                source.resolve(strict=False).relative_to(RUNTIME_SOURCE_ROOT.resolve())
            except ValueError as error:
                raise RuntimeFeatureFailure(
                    f"feature {identifier} artifact escapes runtime/hxrt: {source_path}"
                ) from error
            safe_output_path(ROOT, output_path)
            if hashlib.sha256(source.read_bytes()).hexdigest() != source_sha256:
                raise RuntimeFeatureFailure(f"feature {identifier} artifact source provenance drifted")
            if kind not in ("runtime-header", "runtime-source"):
                raise RuntimeFeatureFailure(f"feature {identifier} has invalid artifact kind {kind}")
            expected_prefix = "runtime/include/" if kind == "runtime-header" else "runtime/src/"
            if not output_path.startswith(expected_prefix):
                raise RuntimeFeatureFailure(
                    f"feature {identifier} artifact {output_path} does not match kind {kind}"
                )
            expected.append(
                {
                    "featureId": identifier,
                    "kind": kind,
                    "outputPath": output_path,
                    "sourcePath": source_path,
                }
            )

    for identifier in selected:
        feature = feature_by_id[identifier]
        for dependency in text_list(feature.get("dependencies"), f"feature {identifier} dependencies"):
            if dependency not in positions or positions[dependency] >= positions[identifier]:
                raise RuntimeFeatureFailure(
                    f"{label} is not dependency-closed before {identifier} -> {dependency}"
                )

    expected.sort(key=lambda artifact: str(artifact["outputPath"]).encode("utf-8"))
    expected_paths = [str(artifact["outputPath"]) for artifact in expected]
    if len(expected_paths) != len(set(expected_paths)):
        raise RuntimeFeatureFailure(f"{label} registry selection repeats an output path")
    actual = [
        record(value, f"{label} artifactDetails[{index}]")
        for index, value in enumerate(records(plan.get("artifactDetails"), f"{label} artifactDetails"))
    ]
    if actual != expected or text_list(plan.get("artifacts"), f"{label} artifacts") != expected_paths:
        raise RuntimeFeatureFailure(f"{label} artifact selection differs from the checked-in registry")
    return expected


def package_from_snapshots(
    catalog: dict[str, object], plans: dict[str, object]
) -> dict[str, object]:
    package: dict[str, object] = {}
    for name in ("alloc", "array", "bytes", "object", "gc", "string", "io"):
        plan_key = "compilerIo" if name == "io" else name
        plan = record(plans.get(plan_key), f"{name} plan")
        files: list[dict[str, object]] = []
        for artifact in selected_artifacts_from_snapshot(catalog, plan, name):
            source_path = str(artifact["sourcePath"])
            contents = safe_output_path(ROOT, source_path).read_text(encoding="utf-8")
            if "\x00" in contents or "\r" in contents:
                raise RuntimeFeatureFailure(f"selected runtime artifact is not canonical text: {source_path}")
            files.append(
                {
                    "path": artifact["outputPath"],
                    "kind": artifact["kind"],
                    "sha256": hashlib.sha256(contents.encode("utf-8")).hexdigest(),
                    "contents": contents,
                }
            )
        package[name] = files
    return package


def materialize_package(root: Path, values: list[object]) -> tuple[Path, ...]:
    sources: list[Path] = []
    for index, value in enumerate(values):
        entry = record(value, f"package[{index}]")
        relative = entry.get("path")
        contents = entry.get("contents")
        if not isinstance(relative, str) or not isinstance(contents, str):
            raise RuntimeFeatureFailure("package entry omitted path or contents")
        output = safe_output_path(root, relative)
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_text(contents, encoding="utf-8", newline="\n")
        if output.suffix == ".c":
            sources.append(output)
    return tuple(sorted(sources))


def run_native_case(toolchain: Toolchain, name: str, package: list[object], consumer: Path, expected: str, build: Path) -> None:
    package_root = build / name
    sources = materialize_package(package_root, package)
    executable = package_root / f"{name}-consumer"
    command = [
        toolchain.compiler,
        *STRICT_FLAGS,
        f"-I{package_root / 'runtime/include'}",
        *(str(source) for source in sources),
        str(consumer),
        "-o",
        str(executable),
    ]
    compile_result = subprocess.run(command, cwd=ROOT, check=False, capture_output=True, text=True, timeout=60)
    if compile_result.returncode != 0 or compile_result.stdout or compile_result.stderr:
        raise RuntimeFeatureFailure(
            f"{toolchain.family} {name} package compile failed\ncommand={command!r}\n"
            f"stdout:\n{compile_result.stdout}\nstderr:\n{compile_result.stderr}"
        )
    run_result = subprocess.run([str(executable)], cwd=ROOT, check=False, capture_output=True, text=True, timeout=30)
    if run_result.returncode != 0 or run_result.stdout != expected or run_result.stderr:
        raise RuntimeFeatureFailure(
            f"{toolchain.family} {name} package execution drifted\n"
            f"exit={run_result.returncode}\nstdout={run_result.stdout!r}\nstderr={run_result.stderr!r}"
        )
    if name == "alloc":
        nm = shutil.which("nm")
        if nm is not None:
            symbols = subprocess.run([nm, str(executable)], cwd=ROOT, check=False, capture_output=True, text=True, timeout=30)
            if (
                symbols.returncode != 0
                or "hxc_array_" in symbols.stdout
                or "hxc_bytes_" in symbols.stdout
                or "hxc_string_" in symbols.stdout
                or "hxc_owned_string" in symbols.stdout
            ):
                raise RuntimeFeatureFailure(
                    f"{toolchain.family} alloc-only link retained an array/Bytes/string runtime symbol"
                )
    if name == "array":
        nm = shutil.which("nm")
        if nm is not None:
            symbols = subprocess.run([nm, str(executable)], cwd=ROOT, check=False, capture_output=True, text=True, timeout=30)
            if (
                symbols.returncode != 0
                or "hxc_bytes_" in symbols.stdout
                or "hxc_string_" in symbols.stdout
                or "hxc_owned_string" in symbols.stdout
            ):
                raise RuntimeFeatureFailure(
                    f"{toolchain.family} array link retained a Bytes/string runtime symbol"
                )


def run_native(package: dict[str, object], toolchains: list[Toolchain]) -> None:
    alloc = records(package.get("alloc"), "alloc package")
    array = records(package.get("array"), "array package")
    bytes_package = records(package.get("bytes"), "Bytes package")
    object_package = records(package.get("object"), "object package")
    gc_package = records(package.get("gc"), "gc package")
    string = records(package.get("string"), "string package")
    io = records(package.get("io"), "io package")
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-runtime-feature-") as temporary:
        root = Path(temporary)
        for toolchain in toolchains:
            family_root = root / toolchain.family
            run_native_case(toolchain, "alloc", alloc, ALLOC_CONSUMER, "runtime-feature-alloc: OK\n", family_root)
            run_native_case(toolchain, "array", array, ARRAY_CONSUMER, "runtime-feature-array: OK\n", family_root)
            run_native_case(toolchain, "bytes", bytes_package, BYTES_CONSUMER, "runtime-feature-bytes: OK\n", family_root)
            run_native_case(toolchain, "object", object_package, OBJECT_CONSUMER, "runtime-feature-object: OK\n", family_root)
            run_native_case(
                toolchain,
                "gc",
                gc_package,
                GC_CONSUMER,
                "gc-contract: OK allocations=265 collections=137 reclaimed=265 pause_ticks=973\n",
                family_root,
            )
            run_native_case(toolchain, "string", string, STRING_CONSUMER, "runtime-feature-string: OK\n", family_root)
            run_native_case(toolchain, "io", io, IO_CONSUMER, "runtime-feature-io\n", family_root)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", *TOOLCHAINS), default="auto")
    parser.add_argument(
        "--native-only",
        action="store_true",
        help="compile checked-in validated plans without requiring the Haxe toolchain",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        validate_schema_document()
        if args.native_only:
            catalog = load_snapshot(CATALOG_EXPECTED, "runtime feature catalog")
            plans = load_snapshot(PLANS_EXPECTED, "runtime feature plans")
        else:
            rendered = render_reports()
            catalog = rendered.catalog
            plans = rendered.plans
            package = rendered.package
            semantic_snapshot(CATALOG_EXPECTED, catalog, "runtime feature catalog")
            semantic_snapshot(PLANS_EXPECTED, plans, "runtime feature plans")
        validate_catalog(catalog)
        validate_plans(plans)
        if args.native_only:
            package = package_from_snapshots(catalog, plans)
        validate_package(package, plans)
        toolchains = selected_toolchains(args.toolchain)
        run_native(package, toolchains)
    except (OSError, UnicodeError, ValueError, RuntimeFeatureFailure, subprocess.TimeoutExpired) as error:
        print(f"runtime-feature-graph: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    evidence = "checked-in plans" if args.native_only else "deterministic Haxe renders"
    print(
        "runtime-feature-graph: OK: "
        f"{families} {evidence}, policy diagnostics, and selective native packaging passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

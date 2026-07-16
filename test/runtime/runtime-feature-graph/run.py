#!/usr/bin/env python3
"""Prove deterministic runtime closure, policy validation, and exact packaging."""

from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import os
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
ALLOC_CONSUMER = CASE / "alloc_consumer.c"
STRING_CONSUMER = CASE / "string_consumer.c"
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
    for required in ("algorithm", "features", "reservedFeatures", "noUnconditionalCore"):
        if required not in properties:
            raise RuntimeFeatureFailure(f"runtime feature schema omitted {required}")


def validate_catalog(catalog: dict[str, object]) -> None:
    if catalog.get("schemaVersion") != 1 or catalog.get("algorithm") != "hxc-runtime-feature-graph-v1":
        raise RuntimeFeatureFailure("runtime feature catalog schema or algorithm drifted")
    if catalog.get("status") != "provisional-native-seed-packaging":
        raise RuntimeFeatureFailure("runtime feature catalog overstated seed readiness")
    if catalog.get("noUnconditionalCore") is not True or catalog.get("compilerSelectableFeatures") != []:
        raise RuntimeFeatureFailure("catalog introduced a baseline or compiler-selectable provisional feature")
    feature_values = records(catalog.get("features"), "catalog features")
    features = {str(record(value, "feature").get("id")): record(value, "feature") for value in feature_values}
    if set(features) != {"runtime-abi", "status", "alloc", "string"}:
        raise RuntimeFeatureFailure(f"provisional feature set drifted: {sorted(features)!r}")
    expected_dependencies = {
        "runtime-abi": [],
        "status": ["runtime-abi"],
        "alloc": ["status"],
        "string": ["alloc"],
    }
    for identifier, dependencies in expected_dependencies.items():
        feature = features[identifier]
        if feature.get("availability") != "native-seed-only" or feature.get("dependencies") != dependencies:
            raise RuntimeFeatureFailure(f"feature {identifier} availability/dependencies drifted")
        if feature.get("minimalAllowed") is not True:
            raise RuntimeFeatureFailure(f"seed feature {identifier} left the narrow allowlist")
    reserved = {
        str(record(value, "reserved feature").get("id"))
        for value in records(catalog.get("reservedFeatures"), "reserved features")
    }
    for required in ("array", "object", "gc", "dynamic", "reflection", "exception", "thread"):
        if required not in reserved:
            raise RuntimeFeatureFailure(f"catalog omitted reserved independent feature {required}")
    serialized = json.dumps(catalog, sort_keys=True, ensure_ascii=False)
    if str(ROOT) in serialized or "/Users/" in serialized or "\\" in serialized:
        raise RuntimeFeatureFailure("runtime feature catalog leaked a host path")


def validate_selected_reasons(plan: dict[str, object], label: str) -> None:
    root_ids: set[str] = set()
    for value in records(plan.get("rootReasons"), f"{label} rootReasons"):
        reason = record(value, f"{label} root reason")
        reason_id = reason.get("id")
        source = record(reason.get("source"), f"{label} root reason source")
        start = record(source.get("start"), f"{label} root reason start")
        file_name = source.get("file")
        line_number = start.get("line")
        if not isinstance(reason_id, str) or not isinstance(file_name, str) or not isinstance(line_number, int):
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


def validate_plans(plans: dict[str, object]) -> None:
    empty = record(plans.get("empty"), "empty plan")
    if (
        empty.get("status") != "analyzed-runtime-free"
        or empty.get("planPurpose") != "compiler-program"
        or empty.get("features") != []
        or empty.get("artifacts") != []
        or empty.get("symbols") != []
        or not empty.get("noRuntimeProof")
    ):
        raise RuntimeFeatureFailure("empty compiler plan did not prove complete hxrt absence")

    alloc = record(plans.get("alloc"), "alloc plan")
    string = record(plans.get("string"), "string plan")
    minimal = record(plans.get("minimalString"), "minimal string plan")
    if alloc.get("features") != ["runtime-abi", "status", "alloc"]:
        raise RuntimeFeatureFailure("alloc closure is incomplete or nondeterministic")
    if string.get("features") != ["runtime-abi", "status", "alloc", "string"]:
        raise RuntimeFeatureFailure("string closure is incomplete or nondeterministic")
    validate_selected_reasons(alloc, "alloc")
    validate_selected_reasons(string, "string")
    validate_selected_reasons(minimal, "minimal string")

    alloc_artifacts = text_list(alloc.get("artifacts"), "alloc artifacts")
    alloc_symbols = text_list(alloc.get("symbols"), "alloc symbols")
    if any("string" in path for path in alloc_artifacts) or any("string" in symbol for symbol in alloc_symbols):
        raise RuntimeFeatureFailure("alloc build plan retained an unselected string artifact or symbol")
    if "runtime/src/string.c" not in text_list(string.get("artifacts"), "string artifacts"):
        raise RuntimeFeatureFailure("string build plan omitted its selected source")
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
        "nonePolicy": "HXC2000",
        "minimalPolicy": "HXC2000",
        "unusedManualRequire": "HXC2000",
        "forbidRequired": "HXC2000",
        "compilerSeed": "HXC2000",
        "reservedFeature": "HXC2000",
        "environment": "HXC2000",
        "tamperedPackage": "HXC9000",
    }
    for name, identifier in expected_ids.items():
        diagnostic = record(diagnostics.get(name), f"diagnostic {name}")
        if diagnostic.get("id") != identifier or not diagnostic.get("message"):
            raise RuntimeFeatureFailure(f"runtime diagnostic {name} drifted")
    cycle = record(diagnostics.get("cycle"), "cycle diagnostic")
    if cycle.get("featureIds") != ["cycle-a", "cycle-b", "cycle-a"]:
        raise RuntimeFeatureFailure("cycle path is not canonical")

    serialized = json.dumps(plans, sort_keys=True, ensure_ascii=False)
    if str(ROOT) in serialized or "/Users/" in serialized or "\\" in serialized:
        raise RuntimeFeatureFailure("runtime feature plans leaked a host path")


def validate_package(package: dict[str, object], plans: dict[str, object]) -> None:
    for name in ("alloc", "string"):
        plan = record(plans.get(name), f"{name} plan")
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
            if symbols.returncode != 0 or "hxc_string_" in symbols.stdout or "hxc_owned_string" in symbols.stdout:
                raise RuntimeFeatureFailure(f"{toolchain.family} alloc-only link retained a string runtime symbol")


def run_native(package: dict[str, object], toolchains: list[Toolchain]) -> None:
    alloc = records(package.get("alloc"), "alloc package")
    string = records(package.get("string"), "string package")
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-runtime-feature-") as temporary:
        root = Path(temporary)
        for toolchain in toolchains:
            family_root = root / toolchain.family
            run_native_case(toolchain, "alloc", alloc, ALLOC_CONSUMER, "runtime-feature-alloc: OK\n", family_root)
            run_native_case(toolchain, "string", string, STRING_CONSUMER, "runtime-feature-string: OK\n", family_root)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", *TOOLCHAINS), default="auto")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        rendered = render_reports()
        validate_schema_document()
        validate_catalog(rendered.catalog)
        validate_plans(rendered.plans)
        validate_package(rendered.package, rendered.plans)
        semantic_snapshot(CATALOG_EXPECTED, rendered.catalog, "runtime feature catalog")
        semantic_snapshot(PLANS_EXPECTED, rendered.plans, "runtime feature plans")
        toolchains = selected_toolchains(args.toolchain)
        run_native(rendered.package, toolchains)
    except (OSError, UnicodeError, ValueError, RuntimeFeatureFailure, subprocess.TimeoutExpired) as error:
        print(f"runtime-feature-graph: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    print(
        "runtime-feature-graph: OK: "
        f"{families} deterministic closure, policy diagnostics, and selective native packaging passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

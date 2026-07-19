#!/usr/bin/env python3
"""Prove closed anonymous-record lowering, layout, copies, and fail-closed edges."""

from __future__ import annotations

import argparse
import copy
import difflib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Iterable
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("aggregate_lowering.hxml")
FIXTURES = Path(__file__).with_name("fixtures")
POSITIVE = FIXTURES / "positive"
NATIVE = Path(__file__).with_name("native")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_AGGREGATE_LOWERING="
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


def haxe_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return environment


def render(
    label: str, *, reverse: bool = False, profile: str = "portable"
) -> tuple[str, dict[str, object]]:
    if profile not in ("portable", "metal"):
        raise AggregateLoweringFailure(f"unknown aggregate profile {profile!r}")
    command = [development_tool("haxe"), str(HXML)]
    if reverse:
        command.extend(["-D", "aggregate_lowering_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "aggregate_lowering_profile=metal"])
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
        raise AggregateLoweringFailure(
            f"{label} emitted an invalid report\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise AggregateLoweringFailure(f"{label} report is not an object")
    return payload, report


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
    if len(records) != 2:
        raise AggregateLoweringFailure(
            "OrderA/OrderB did not deduplicate to one pair plus one envelope"
        )
    pair, envelope = records
    pair_fields = pair.get("fields")
    envelope_fields = envelope.get("fields")
    if (
        not isinstance(pair_fields, list)
        or not all(isinstance(field, dict) for field in pair_fields)
        or not isinstance(envelope_fields, list)
        or not all(isinstance(field, dict) for field in envelope_fields)
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
    }
    if not all(isinstance(value, str) and value for value in values.values()):
        raise AggregateLoweringFailure("aggregate report omitted finalized C identifiers")
    return {key: str(value) for key, value in values.items()}


def function_section(hxcir: str, field: str) -> str:
    start_marker = f'  function "function.AggregateFixture.{field}"'
    end_marker = f'  end function "function.AggregateFixture.{field}"'
    start = hxcir.find(start_marker)
    end = hxcir.find(end_marker, start)
    if start == -1 or end == -1:
        raise AggregateLoweringFailure(f"HxcIR omitted function {field}")
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

    pair_type = f'type "type.closed-record.{aggregate_records(report)[0]["digest"]}"'
    envelope_type = f'type "type.closed-record.{aggregate_records(report)[1]["digest"]}"'
    if hxcir.find(pair_type) == -1 or hxcir.find(pair_type) > hxcir.find(envelope_type):
        raise AggregateLoweringFailure("nested HxcIR declarations are not dependency-first")
    if hxcir.count(" representation=direct ") != 2:
        raise AggregateLoweringFailure("closed record instances lost direct representation")

    make = function_section(hxcir, "make")
    calls = [make.find('call dispatch=direct("function.AggregateFixture.identity")')]
    calls.append(make.find('call dispatch=direct("function.AggregateFixture.identity")', calls[0] + 1))
    construction = make.find('fields=["a"="value.1","z"="value.0"]')
    if any(index == -1 for index in calls) or calls != sorted(calls) or calls[1] > construction:
        raise AggregateLoweringFailure(
            "object-literal evaluation order or canonical named construction drifted"
        )
    local_sum = function_section(hxcir, "localSum")
    address_a = local_sum.find('address place=field(local("local.2"),"a")')
    load_a = local_sum.find('load place=dereference("value.1")')
    address_z = local_sum.find('address place=field(local("local.2"),"z")')
    load_z = local_sum.find('load place=dereference("value.3")')
    if min(address_a, load_a, address_z, load_z) < 0 or not (
        address_a < load_a < address_z < load_z
    ):
        raise AggregateLoweringFailure("local field reads lost explicit address/dereference order")
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

    pair_definition = f"struct {names['pair_tag']} {{"
    envelope_definition = f"struct {names['envelope_tag']} {{"
    if (
        header.find(pair_definition) == -1
        or header.find(envelope_definition) == -1
        or header.find(pair_definition) > header.find(envelope_definition)
        or f"int32_t {names['pair_a']};" not in header
        or f"int32_t {names['pair_z']};" not in header
        or f"bool {names['envelope_enabled']};" not in header
        or f"struct {names['pair_tag']} {names['envelope_point']};" not in header
        or "#include <stddef.h>" not in header
    ):
        raise AggregateLoweringFailure("private dependency-first struct header drifted")
    if (
        source.count("_Static_assert(") != 10
        or source.count("offsetof(") < 8
        or f"(struct {names['pair_tag']}){{" not in source
        or f"(struct {names['envelope_tag']}){{" not in source
        or "int main(void)" not in source
    ):
        raise AggregateLoweringFailure("structural CAST construction/layout assertions drifted")
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v1":
        raise AggregateLoweringFailure("aggregate report omitted its finalized symbol table")


def normalized_profile(report: dict[str, object]) -> dict[str, object]:
    value = copy.deepcopy(report)
    value["profile"] = "<profile>"
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
    if lifecycle_probe:
        command.extend(["-D", "reflaxe_c_lifecycle_probe"])
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
        raise AggregateLoweringFailure(
            f"{label} failed\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def validate_production(root: Path, *, profile: str, policy: str) -> None:
    if generated_files(root) != PRODUCTION_FILES:
        raise AggregateLoweringFailure(
            f"{profile}/{policy} production artifact set drifted: "
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
        or not isinstance(proof, dict)
        or proof.get("status") != "eligible"
        or proof.get("directDecisions") != runtime_plan.get("directDecisions")
        or reachability is None
        or reachability.get("typeInstances") != 2
        or reachability.get("runtimeIntents") != 0
    ):
        raise AggregateLoweringFailure(f"{profile}/{policy} lost its runtime-free aggregate proof")
    combined = b"\n".join(
        path.read_bytes()
        for path in root.rglob("*")
        if path.is_file() and path.suffix in {".c", ".h"}
    ).lower()
    if b"hxrt" in combined or b"hxc_runtime" in combined:
        raise AggregateLoweringFailure("aggregate production project selected runtime code")


def check_production() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-aggregate-production-") as temporary:
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
                main="AggregateFixture",
                profile=profile,
                runtime=runtime,
            )
            require_compile_success(result, f"{name} aggregate production compile")
            validate_production(output, profile=profile, policy=policy)
        if generated_tree(root / "first") != generated_tree(root / "repeat"):
            raise AggregateLoweringFailure("two aggregate production roots were not byte-identical")
        for relative in ("include/hxc/program.h", "src/program.c"):
            if (root / "first" / relative).read_bytes() != (root / "metal" / relative).read_bytes():
                raise AggregateLoweringFailure(
                    f"portable and metal changed direct aggregate artifact {relative}"
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
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("aggregate-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
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
        check_production()
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
        "explicit address/copy IR, strict C11 and C++17 layout agreement, runtime-free "
        "production artifacts, and fail-closed identity/layout edges passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

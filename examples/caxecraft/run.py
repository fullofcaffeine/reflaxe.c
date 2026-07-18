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
from collections.abc import Iterable
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
BUILD_HXML = CASE / "build.hxml"
ORACLE_HXML = CASE / "oracle.hxml"
EXPECTED = CASE / "expected"
NATIVE = CASE / "test/native"
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
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
SNAPSHOT_FORMATS = {
    "include/hxc/program.h": "header",
    "src/program.c": "c",
    "hxc.runtime-plan.json": "json",
    "method-symbols.json": "json",
    "oracle.txt": "text",
}
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


def validate_hxcir(hxcir: str) -> None:
    for marker in (
        "hxcir schema=9",
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


def validate_generated_text(header: bytes, source: bytes) -> None:
    combined = header + b"\n" + source
    if str(ROOT).encode() in combined or b"\\" in combined or b"hxrt" in combined.lower():
        raise CaxecraftFailure("generated Caxecraft C leaked a host path or hxrt")
    text = combined.decode("utf-8")
    forbidden = re.compile(r"(?:\bhxrt_[A-Za-z0-9_]*\b|\b(?:malloc|calloc|realloc|free)\s*\()")
    match = forbidden.search(text)
    if match is not None:
        raise CaxecraftFailure(f"generated Caxecraft C selected {match.group(0)!r}")
    for marker in (
        "uint8_t hxc_local_caxecraft_domain_CaxecraftTrace_terrainTrace_storage",
        "hxc_method_caxecraft_domain_VoxelRaycast_trace",
        "hxc_method_caxecraft_domain_PlayerPhysics_step",
        "hxc_method_caxecraft_qa_DomainProbe_selfCheck",
    ):
        if marker not in text:
            raise CaxecraftFailure(f"generated Caxecraft C omitted {marker!r}")


def render_project(
    output: Path,
    *,
    label: str,
    reverse: bool = False,
    locale: str = "C",
    connect: str | None = None,
    report: bool = False,
) -> RenderedProject:
    result = compile_target(
        output,
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
    if actual_files != PRODUCTION_FILES:
        raise CaxecraftFailure(
            f"{label} generated file set drifted: {sorted(actual_files)!r}"
        )
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
    projection = method_symbol_projection(symbols)
    validate_method_symbols(projection)
    header = (output / "include/hxc/program.h").read_bytes()
    source = (output / "src/program.c").read_bytes()
    validate_generated_text(header, source)
    hxcir = extract_hxcir(result, label) if report else ""
    if report:
        validate_hxcir(hxcir)
    return RenderedProject(
        output,
        normal_artifacts(output),
        hxcir,
        runtime_plan,
        projection,
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
            "include/hxc/program.h": (
                project.output / "include/hxc/program.h"
            ).read_text(encoding="utf-8"),
            "src/program.c": (project.output / "src/program.c").read_text(
                encoding="utf-8"
            ),
            "hxc.runtime-plan.json": project.runtime_plan,
            "method-symbols.json": project.method_symbols,
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


def validate_expected(values: dict[str, object]) -> tuple[bytes, bytes, bytes]:
    header = values.get("include/hxc/program.h")
    source = values.get("src/program.c")
    oracle = values.get("oracle.txt")
    runtime_plan = values.get("hxc.runtime-plan.json")
    method_symbols = values.get("method-symbols.json")
    if not all(isinstance(value, str) for value in (header, source, oracle)):
        raise CaxecraftFailure("Caxecraft text baseline is malformed")
    if not isinstance(runtime_plan, dict) or not isinstance(method_symbols, dict):
        raise CaxecraftFailure("Caxecraft JSON baseline is malformed")
    validate_runtime_plan(runtime_plan)
    validate_method_symbols(method_symbols)
    header_bytes = header.encode("utf-8")
    source_bytes = source.encode("utf-8")
    oracle_bytes = oracle.encode("ascii")
    validate_generated_text(header_bytes, source_bytes)
    lines = oracle_bytes.splitlines()
    if len(lines) != 38 or lines[0] != b"0" or not oracle_bytes.endswith(b"\n"):
        raise CaxecraftFailure("checked-in Caxecraft oracle baseline drifted")
    return header_bytes, source_bytes, oracle_bytes


def validate_snapshots(project: RenderedProject, oracle: bytes) -> None:
    expected = expected_values()
    actual: dict[str, object] = {
        "include/hxc/program.h": (
            project.output / "include/hxc/program.h"
        ).read_text(encoding="utf-8"),
        "src/program.c": (project.output / "src/program.c").read_text(
            encoding="utf-8"
        ),
        "hxc.runtime-plan.json": project.runtime_plan,
        "method-symbols.json": project.method_symbols,
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
    fixture: Path, header: bytes, source: bytes
) -> None:
    (fixture / "generated/include/hxc").mkdir(parents=True)
    (fixture / "generated/src").mkdir(parents=True)
    (fixture / "native").mkdir(parents=True)
    (fixture / "generated/include/hxc/program.h").write_bytes(header)
    (fixture / "generated/src/program.c").write_bytes(source)
    shutil.copy2(NATIVE / "domain_harness.c", fixture / "native/domain_harness.c")
    shutil.copy2(NATIVE / "generated_program.c", fixture / "native/generated_program.c")


def native_project(oracle: bytes, *, sanitizer: bool) -> CFixtureProject:
    return CFixtureProject(
        identifier="caxecraft-domain",
        sources=("native/domain_harness.c", "native/generated_program.c"),
        headers=("generated/include/hxc/program.h", "generated/src/program.c"),
        include_directories=("generated/include",),
        expected_stdout=oracle.decode("ascii"),
        coverage=tuple(sorted(COVERAGE)),
        link_arguments=SANITIZER_FLAGS if sanitizer else (),
    )


def inspect_generated_object_symbols(
    build_root: Path, report: dict[str, object]
) -> None:
    toolchains = report.get("toolchains")
    if not isinstance(toolchains, list):
        raise CaxecraftFailure("native Caxecraft report omitted toolchains")
    for entry in toolchains:
        if not isinstance(entry, dict) or not isinstance(entry.get("family"), str):
            raise CaxecraftFailure("native Caxecraft report has a malformed toolchain")
        family = entry["family"]
        generated_object = (
            build_root
            / family
            / "caxecraft-domain/01-generated_program.o"
        )
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
        imported = {
            line.split()[-1].lstrip("_").lower()
            for line in result.stdout.splitlines()
            if line.split()
        }
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


def run_native(
    header: bytes,
    source: bytes,
    oracle: bytes,
    *,
    requested_toolchain: str,
    root: Path,
) -> None:
    fixture = root / "fixture"
    prepare_native_fixture(fixture, header, source)
    modes = (
        ("o0", ("-O0",), False),
        ("o2", ("-O2",), False),
        ("sanitizer", ("-O1", *SANITIZER_FLAGS), True),
    )
    for mode, extra_flags, sanitizer in modes:
        build_root = root / mode
        report = run_c_fixture_corpus(
            suite=f"caxecraft-domain-{mode}",
            projects=(native_project(oracle, sanitizer=sanitizer),),
            fixture_root=fixture,
            build_root=build_root,
            repository_root=ROOT,
            requested_toolchain=requested_toolchain,
            strict_flags=(*STRICT_FLAGS, *extra_flags),
            timeout_seconds=120,
        )
        validate_report(report, required_coverage=COVERAGE)
        inspect_generated_object_symbols(build_root, report)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument(
        "--native-only",
        action="store_true",
        help="compile and run the checked-in generated baseline without Haxe",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-domain-") as temporary:
            root = Path(temporary)
            if args.native_only:
                header, source, oracle = validate_expected(expected_values())
            else:
                oracle = run_oracle()
                first = render_project(
                    root / "first",
                    label="first cold Caxecraft render",
                    report=True,
                )
                check_determinism(first, root / "determinism")
                validate_snapshots(first, oracle)
                header = (first.output / "include/hxc/program.h").read_bytes()
                source = (first.output / "src/program.c").read_bytes()
            run_native(
                header,
                source,
                oracle,
                requested_toolchain=args.toolchain,
                root=root / "native",
            )
    except (
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

    mode = "checked-in C baseline" if args.native_only else "Eval/generated-C differential"
    print(
        "caxecraft-domain: OK: "
        f"{mode}, 32 seeded properties, exact traces, deterministic cold/reversed/"
        "locale/warm-server artifacts, zero hxrt/allocation symbols, strict O0/O2, "
        "and ASan/UBSan passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

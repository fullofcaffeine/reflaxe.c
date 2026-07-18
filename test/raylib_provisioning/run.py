#!/usr/bin/env python3
"""Validate locked raylib provisioning and generated-Haxe consumption."""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
import unittest
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Mapping, Sequence


ROOT = Path(__file__).resolve().parents[2]
SUITE = ROOT / "test/raylib_provisioning"
FIXTURE = SUITE / "fixtures/smoke"
SUPPORT_INCLUDE = SUITE / "support/include"
EXPECTED = SUITE / "expected"
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.raylib.provision import (  # noqa: E402
    CommandResult,
    ProvisionFailure,
    SourceBuild,
    assert_report_redacted,
    build_source,
    canonical_json,
    compiler_identity,
    host_platform_key,
    link_facts,
    load_lock,
    normalize_text,
    path_replacements,
    run_command,
    sha256_file,
    write_report,
)


STRICT_C_FLAGS = (
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
STRICT_CLANG_CL_FLAGS = (
    "/W4",
    "/WX",
    "/clang:-pedantic",
    "/clang:-Wshadow",
    "/clang:-Wconversion",
    "/clang:-Wsign-conversion",
    "/clang:-Wstrict-prototypes",
    "/clang:-Wmissing-prototypes",
    "/clang:-Wundef",
    "/clang:-Wformat=2",
    "/clang:-Wimplicit-fallthrough",
    "/clang:-Wcast-align",
    "/clang:-Wcast-qual",
)
CLANG_CL_RUNTIME_FLAG = "/MD"
EXPECTED_HEADLESS_STDOUT = "INFO: RLSW: Software renderer initialized successfully\n"
VARIANTS = (
    ("linux-memory", "linux", "memory-software", False),
    ("linux-desktop", "linux", "desktop", False),
    ("macos-desktop", "macos", "desktop", False),
    ("windows-desktop", "windows", "desktop", False),
    ("system-desktop", "linux", "desktop", True),
)


class RaylibTestFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class RenderedProject:
    artifacts: dict[str, bytes]
    manifest: dict[str, object]
    runtime_plan: dict[str, object]


def development_tool(name: str) -> str:
    local_name = f"{name}.cmd" if os.name == "nt" else name
    local = ROOT / "node_modules/.bin" / local_name
    return str(local) if local.is_file() else name


def haxe_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    environment["LC_ALL"] = "C"
    return environment


def haxe_defines(platform_name: str, configuration: str, system: bool) -> list[str]:
    if platform_name not in ("linux", "macos", "windows"):
        raise RaylibTestFailure(f"unknown fixture platform: {platform_name}")
    if configuration not in ("memory-software", "desktop"):
        raise RaylibTestFailure(f"unknown fixture configuration: {configuration}")
    definitions = [f"raylib_platform_{platform_name}"]
    definitions.append(
        "raylib_configuration_desktop"
        if configuration == "desktop"
        else "raylib_configuration_memory"
    )
    if system:
        definitions.append("raylib_system_pkg_config")
    return definitions


def haxe_command(
    output: Path,
    *,
    platform_name: str,
    configuration: str,
    system: bool,
) -> list[str]:
    command = [
        development_tool("haxe"),
        "--cwd",
        str(FIXTURE),
        "build.hxml",
        "-D",
        "hxc_runtime_diagnostics=off",
    ]
    for definition in haxe_defines(platform_name, configuration, system):
        command.extend(["-D", definition])
    command.extend(["--custom-target", f"c={output}"])
    return command


def compile_fixture(
    output: Path,
    *,
    platform_name: str,
    configuration: str,
    system: bool = False,
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        haxe_command(
            output,
            platform_name=platform_name,
            configuration=configuration,
            system=system,
        ),
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=90,
    )


def require_success(result: subprocess.CompletedProcess[str], label: str) -> None:
    if result.returncode != 0:
        raise RaylibTestFailure(
            f"{label} failed with exit {result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def read_json(path: Path, label: str) -> dict[str, object]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise RaylibTestFailure(f"cannot read {label}: {error}") from error
    if not isinstance(value, dict):
        raise RaylibTestFailure(f"{label} must contain a JSON object")
    return value


def normal_artifacts(root: Path) -> dict[str, bytes]:
    return {
        path.relative_to(root).as_posix(): path.read_bytes()
        for path in sorted(root.rglob("*"), key=lambda item: item.as_posix().encode("utf-8"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def rendered_project(root: Path) -> RenderedProject:
    manifest = read_json(root / "hxc.manifest.json", "generated raylib manifest")
    runtime_plan = read_json(root / "hxc.runtime-plan.json", "generated raylib runtime plan")
    return RenderedProject(normal_artifacts(root), manifest, runtime_plan)


def fact_names(value: object, label: str) -> list[str]:
    if not isinstance(value, list):
        raise RaylibTestFailure(f"{label} must be an array")
    result: list[str] = []
    for entry in value:
        if not isinstance(entry, dict) or not isinstance(entry.get("name"), str):
            raise RaylibTestFailure(f"{label} contains an invalid fact: {entry!r}")
        owners = entry.get("ownerModulePaths")
        if owners != ["RaylibNative"]:
            raise RaylibTestFailure(f"{label} lost RaylibNative provenance: {entry!r}")
        result.append(entry["name"])
    return result


def validate_build_plan(
    plan: object,
    lock: Mapping[str, object],
    *,
    platform_name: str,
    configuration: str,
    system: bool,
) -> None:
    if not isinstance(plan, dict):
        raise RaylibTestFailure("generated manifest omitted the neutral build plan")
    expected_common = {
        "schemaVersion": 1,
        "sources": ["src/program.c"],
        "publicHeaders": [],
        "privateHeaders": ["include/hxc/program.h"],
        "runtimeHeaders": [],
        "includeDirectories": ["include"],
        "cStandard": "c11",
        "extensions": False,
        "warningPolicy": "strict",
        "definitions": [],
        "artifact": {"kind": "executable", "targetName": "hxc_program"},
    }
    for key, expected in expected_common.items():
        if plan.get(key) != expected:
            raise RaylibTestFailure(f"neutral build plan {key} drifted: {plan.get(key)!r}")
    expected_headers = [
        {
            "path": "hxc_raylib_smoke_constants.h",
            "kind": "local",
            "ownerModulePaths": ["RaylibNative"],
        },
        {
            "path": "raylib.h",
            "kind": "system",
            "ownerModulePaths": ["Color", "RaylibNative", "TraceLogLevel"],
        },
    ]
    if plan.get("requiredHeaders") != expected_headers:
        raise RaylibTestFailure("neutral build plan lost exact raylib header facts")
    if system:
        if fact_names(plan.get("pkgConfigPackages"), "pkgConfigPackages") != ["raylib"]:
            raise RaylibTestFailure("system neutral plan must select exactly pkg-config raylib")
        if plan.get("libraries") != [] or plan.get("frameworks") != []:
            raise RaylibTestFailure("system neutral plan silently mixed pkg-config with source facts")
    else:
        expected_libraries, expected_frameworks = link_facts(lock, platform_name, configuration)
        actual_libraries = fact_names(plan.get("libraries"), "libraries")
        actual_frameworks = fact_names(plan.get("frameworks"), "frameworks")
        if len(actual_libraries) != len(expected_libraries) or set(actual_libraries) != set(expected_libraries):
            raise RaylibTestFailure(
                f"neutral libraries differ for {platform_name}/{configuration}: {actual_libraries!r}"
            )
        if len(actual_frameworks) != len(expected_frameworks) or set(actual_frameworks) != set(expected_frameworks):
            raise RaylibTestFailure(
                f"neutral frameworks differ for {platform_name}/{configuration}: {actual_frameworks!r}"
            )
        if plan.get("pkgConfigPackages") != []:
            raise RaylibTestFailure("source neutral plan silently selected pkg-config")
    rendered = canonical_json(plan)
    for unsafe in ("&&", ";", "`", "$(`", "\n-D", "\n-l"):
        if unsafe in rendered:
            raise RaylibTestFailure(f"neutral build plan contains shell/build syntax: {unsafe!r}")


def validate_runtime_free(project: RenderedProject) -> None:
    plan = project.runtime_plan
    if plan.get("schemaVersion") != 2:
        raise RaylibTestFailure("raylib smoke runtime plan must use schemaVersion 2")
    if plan.get("selectedFeatures") != [] or plan.get("artifacts") != []:
        raise RaylibTestFailure("direct raylib smoke unexpectedly selected hxrt")
    for path, content in project.artifacts.items():
        if path.endswith((".c", ".h")) and b"hxrt" in content:
            raise RaylibTestFailure(f"generated raylib artifact mentions hxrt: {path}")


def snapshot_values() -> dict[str, object]:
    lock = load_lock()
    with tempfile.TemporaryDirectory(prefix="hxc-raylib-snapshot-a-") as first_raw, tempfile.TemporaryDirectory(
        prefix="hxc-raylib-snapshot-b-"
    ) as second_raw:
        first_root = Path(first_raw) / "generated"
        second_root = Path(second_raw) / "generated"
        first_result = compile_fixture(
            first_root,
            platform_name="linux",
            configuration="memory-software",
        )
        second_result = compile_fixture(
            second_root,
            platform_name="linux",
            configuration="memory-software",
        )
        require_success(first_result, "first generated raylib smoke")
        require_success(second_result, "second generated raylib smoke")
        first = rendered_project(first_root)
        second = rendered_project(second_root)
        if first.artifacts != second.artifacts:
            raise RaylibTestFailure("generated raylib artifacts changed across unrelated output roots")
        validate_build_plan(
            first.manifest.get("build"),
            lock,
            platform_name="linux",
            configuration="memory-software",
            system=False,
        )
        validate_runtime_free(first)
        values: dict[str, object] = {
            "include/hxc/program.h": first.artifacts["include/hxc/program.h"].decode("utf-8"),
            "src/program.c": first.artifacts["src/program.c"].decode("utf-8"),
            "hxc.runtime-plan.json": first.runtime_plan,
            "build-linux-memory.json": first.manifest["build"],
        }

        for identifier, platform_name, configuration, system in VARIANTS[1:]:
            output = Path(first_raw) / identifier
            result = compile_fixture(
                output,
                platform_name=platform_name,
                configuration=configuration,
                system=system,
            )
            require_success(result, f"generated raylib plan {identifier}")
            project = rendered_project(output)
            validate_build_plan(
                project.manifest.get("build"),
                lock,
                platform_name=platform_name,
                configuration=configuration,
                system=system,
            )
            validate_runtime_free(project)
            if project.artifacts["src/program.c"] != first.artifacts["src/program.c"]:
                raise RaylibTestFailure(f"generated C changed with provisioning facts: {identifier}")
            values[f"build-{identifier}.json"] = project.manifest["build"]
        return values


def expected_value(path: Path, format_name: str) -> object:
    if format_name == "json":
        return read_json(path, path.relative_to(ROOT).as_posix())
    return path.read_text(encoding="utf-8")


def validate_expected(values: Mapping[str, object]) -> None:
    formats = {
        "include/hxc/program.h": "text",
        "src/program.c": "text",
        "hxc.runtime-plan.json": "json",
        "build-linux-memory.json": "json",
        "build-linux-desktop.json": "json",
        "build-macos-desktop.json": "json",
        "build-windows-desktop.json": "json",
        "build-system-desktop.json": "json",
    }
    actual_names = set(values)
    if actual_names != set(formats):
        raise RaylibTestFailure(f"raylib snapshot artifact set drifted: {sorted(actual_names)!r}")
    for relative, format_name in formats.items():
        path = EXPECTED / relative
        if not path.is_file():
            raise RaylibTestFailure(
                f"raylib snapshot is missing: {path.relative_to(ROOT)}; run the registered snapshot updater"
            )
        if expected_value(path, format_name) != values[relative]:
            raise RaylibTestFailure(
                f"raylib snapshot drifted: {path.relative_to(ROOT)}; use the registered snapshot updater"
            )


def run_unit_tests() -> None:
    suite = unittest.defaultTestLoader.discover(str(SUITE), pattern="test_*.py")
    result = unittest.TextTestRunner(verbosity=1).run(suite)
    if not result.wasSuccessful():
        raise RaylibTestFailure("raylib provisioning negative/unit tests failed")


def artifact_hashes(root: Path) -> list[dict[str, object]]:
    result: list[dict[str, object]] = []
    for path in sorted(root.rglob("*"), key=lambda item: item.as_posix().encode("utf-8")):
        if path.is_file() and path.name != "_GeneratedFiles.json":
            result.append(
                {
                    "path": path.relative_to(root).as_posix(),
                    "sha256": sha256_file(path),
                    "sizeBytes": path.stat().st_size,
                }
            )
    return result


def gcc_like_compile_arguments(
    compiler: str,
    generated_root: Path,
    include_directory: Path,
    native_root: Path,
) -> tuple[list[str], Path]:
    object_file = native_root / "program.o"
    arguments = [
        compiler,
        "-std=c11",
        *STRICT_C_FLAGS,
        "-I",
        str(generated_root / "include"),
        "-I",
        str(SUPPORT_INCLUDE),
        "-I",
        str(include_directory),
        "-c",
        str(generated_root / "src/program.c"),
        "-o",
        str(object_file),
    ]
    return arguments, object_file


def clang_cl_compile_arguments(
    compiler: str,
    generated_root: Path,
    include_directory: Path,
    native_root: Path,
) -> tuple[list[str], Path]:
    object_file = native_root / "program.obj"
    arguments = [
        compiler,
        "/nologo",
        "/TC",
        "/std:c11",
        CLANG_CL_RUNTIME_FLAG,
        *STRICT_CLANG_CL_FLAGS,
        f"/I{generated_root / 'include'}",
        f"/I{SUPPORT_INCLUDE}",
        f"/I{include_directory}",
        "/c",
        str(generated_root / "src/program.c"),
        f"/Fo{object_file}",
    ]
    return arguments, object_file


def gcc_like_link_arguments(
    compiler: str,
    object_file: Path,
    library_file: Path,
    libraries: Sequence[str],
    frameworks: Sequence[str],
    executable: Path,
) -> list[str]:
    arguments = [compiler, str(object_file), str(library_file)]
    for library in libraries:
        if library == "raylib":
            continue
        arguments.append("-pthread" if library == "pthread" else f"-l{library}")
    for framework in frameworks:
        arguments.extend(["-framework", framework])
    arguments.extend(["-o", str(executable)])
    return arguments


def clang_cl_link_arguments(
    compiler: str,
    object_file: Path,
    library_file: Path,
    libraries: Sequence[str],
    executable: Path,
) -> list[str]:
    arguments = [
        compiler,
        "/nologo",
        CLANG_CL_RUNTIME_FLAG,
        str(object_file),
        str(library_file),
    ]
    for library in libraries:
        if library != "raylib":
            arguments.append(f"{library}.lib")
    arguments.append(f"/Fe{executable}")
    return arguments


def integration_report(args: argparse.Namespace) -> dict[str, object]:
    lock = load_lock()
    cmake = args.cmake or "cmake"
    if args.authority not in ("pinned-source", "offline-source"):
        raise RaylibTestFailure("native integration currently requires a reviewed source authority")
    source_build = build_source(
        lock=lock,
        authority=args.authority,
        configuration=args.configuration,
        platform_name=args.platform,
        cache_root=args.cache_root,
        source_root=args.source,
        build_root=args.build_root,
        compiler=args.cc,
        cxx_compiler=args.cxx,
        cmake=cmake,
        generator=args.generator,
        allow_network=args.allow_network,
    )
    work_root = source_build.build_root.parent
    generated_root = work_root / f"{source_build.build_root.name}-generated-haxe"
    native_root = work_root / f"{source_build.build_root.name}-generated-native"
    if generated_root.exists() or native_root.exists():
        raise RaylibTestFailure("integration generated/native roots must not already exist")
    native_root.mkdir(parents=True)
    haxe = development_tool("haxe")
    replacements = path_replacements(
        {
            "${CACHE_ROOT}": args.cache_root.resolve() if args.cache_root is not None else "",
            "${RAYLIB_SOURCE}": source_build.source_root,
            "${RAYLIB_BUILD}": source_build.build_root,
            "${GENERATED_ROOT}": generated_root,
            "${FIXTURE_ROOT}": FIXTURE,
            "${SUPPORT_INCLUDE}": SUPPORT_INCLUDE,
            "${NATIVE_ROOT}": native_root,
            "${C_COMPILER}": args.cc,
            "${CXX_COMPILER}": args.cxx,
            "${CMAKE}": cmake,
            "${HAXE}": haxe,
        }
    )
    haxe_identity = run_command(
        [haxe, "--version"],
        cwd=ROOT,
        replacements=replacements,
        timeout=30,
        label="Haxe compiler identity",
        environment=haxe_environment(),
    )
    haxe_compile = run_command(
        haxe_command(
            generated_root,
            platform_name=args.platform,
            configuration=args.configuration,
            system=False,
        ),
        cwd=ROOT,
        replacements=replacements,
        timeout=90,
        label="generated Haxe raylib smoke",
        environment=haxe_environment(),
    )
    project = rendered_project(generated_root)
    validate_build_plan(
        project.manifest.get("build"),
        lock,
        platform_name=args.platform,
        configuration=args.configuration,
        system=False,
    )
    validate_runtime_free(project)
    compiler_info, compiler_commands = compiler_identity(
        args.cc,
        cwd=native_root,
        replacements=replacements,
    )
    family = compiler_info.get("family")
    if family == "clang-cl":
        compile_arguments, object_file = clang_cl_compile_arguments(
            args.cc,
            generated_root,
            source_build.include_directory,
            native_root,
        )
    elif family in ("gcc", "clang", "apple-clang"):
        compile_arguments, object_file = gcc_like_compile_arguments(
            args.cc,
            generated_root,
            source_build.include_directory,
            native_root,
        )
    else:
        raise RaylibTestFailure(f"unsupported generated-C compiler family: {family!r}")
    compile_result = run_command(
        compile_arguments,
        cwd=native_root,
        replacements=replacements,
        timeout=120,
        label="strict generated raylib C compile",
    )
    libraries, frameworks = link_facts(lock, args.platform, args.configuration)
    executable = native_root / ("raylib-smoke.exe" if args.platform == "windows" else "raylib-smoke")
    if family == "clang-cl":
        link_arguments = clang_cl_link_arguments(
            args.cc,
            object_file,
            source_build.library_file,
            libraries,
            executable,
        )
    else:
        link_arguments = gcc_like_link_arguments(
            args.cc,
            object_file,
            source_build.library_file,
            libraries,
            frameworks,
            executable,
        )
    link_result = run_command(
        link_arguments,
        cwd=native_root,
        replacements=replacements,
        timeout=120,
        label="generated raylib native link",
    )
    run_evidence: dict[str, object] = {"exercised": False}
    if args.run:
        if args.configuration != "memory-software":
            raise RaylibTestFailure("desktop provisioning is compile/link-only and cannot use --run")
        run_result = run_command(
            [str(executable)],
            cwd=native_root,
            replacements=replacements,
            timeout=30,
            label="generated headless raylib run",
        )
        stdout = normalize_text(run_result.process.stdout.decode("utf-8", errors="replace"), replacements)
        stderr = normalize_text(run_result.process.stderr.decode("utf-8", errors="replace"), replacements)
        if stdout != EXPECTED_HEADLESS_STDOUT or stderr != "":
            raise RaylibTestFailure(
                f"headless raylib output drifted: stdout={stdout!r}, stderr={stderr!r}"
            )
        run_evidence = dict(run_result.evidence)
        run_evidence.update(
            {
                "exercised": True,
                "stdoutText": stdout,
                "stderrText": stderr,
            }
        )

    provision_report = json.loads(canonical_json(source_build.report))
    claims = provision_report.get("claims")
    if not isinstance(claims, dict):
        raise RaylibTestFailure("provision report omitted claims")
    claims["generatedCCompiled"] = True
    claims["linked"] = True
    claims["executed"] = bool(args.run)
    report: dict[str, object] = {
        "schemaVersion": 1,
        "provision": provision_report,
        "generatedProject": {
            "haxe": {
                "command": haxe_identity.evidence,
                "version": haxe_identity.process.stdout.decode("utf-8", errors="replace").strip(),
                "compile": haxe_compile.evidence,
            },
            "buildPlan": project.manifest["build"],
            "runtimePlan": project.runtime_plan,
            "artifacts": artifact_hashes(generated_root),
        },
        "nativeConsumer": {
            "compiler": compiler_info,
            "compilerIdentity": compiler_commands,
            "compile": compile_result.evidence,
            "link": link_result.evidence,
            "run": run_evidence,
            "object": {
                "path": normalize_text(str(object_file), replacements),
                "sha256": sha256_file(object_file),
                "sizeBytes": object_file.stat().st_size,
            },
            "executable": {
                "path": normalize_text(str(executable), replacements),
                "sha256": sha256_file(executable),
                "sizeBytes": executable.stat().st_size,
            },
        },
        "claims": {
            "cacheAuthorityExplicit": True,
            "networkAfterProvision": False,
            "generatedCCompiled": True,
            "linked": True,
            "executed": bool(args.run),
            "runConfiguration": args.configuration if args.run else None,
        },
    }
    forbidden: list[Path | str] = [
        ROOT,
        source_build.source_root,
        source_build.build_root,
        generated_root,
        FIXTURE,
        SUPPORT_INCLUDE,
        native_root,
        args.cc,
        args.cxx,
        cmake,
        haxe,
    ]
    if args.cache_root is not None:
        forbidden.append(args.cache_root.resolve())
    assert_report_redacted(report, forbidden)
    return report


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--integration", action="store_true")
    parser.add_argument("--authority", choices=("pinned-source", "offline-source"))
    parser.add_argument("--configuration", choices=("memory-software", "desktop"))
    parser.add_argument("--platform", choices=("linux", "macos", "windows"))
    parser.add_argument("--cache-root", type=Path)
    parser.add_argument("--source", type=Path)
    parser.add_argument("--build-root", type=Path)
    parser.add_argument("--cc")
    parser.add_argument("--cxx")
    parser.add_argument("--cmake")
    parser.add_argument("--generator", choices=("Ninja", "Unix Makefiles"))
    parser.add_argument("--allow-network", action="store_true")
    parser.add_argument("--run", action="store_true")
    parser.add_argument("--report", type=Path)
    parser.add_argument("--print-report", action="store_true")
    return parser.parse_args(list(argv))


def require_integration_args(args: argparse.Namespace) -> None:
    missing = [
        name
        for name in ("authority", "configuration", "platform", "build_root", "cc", "cxx", "generator", "report")
        if getattr(args, name) is None
    ]
    if missing:
        raise RaylibTestFailure("integration mode requires: " + ", ".join(f"--{name.replace('_', '-')}" for name in missing))
    if args.authority == "pinned-source" and args.cache_root is None:
        raise RaylibTestFailure("pinned-source integration requires --cache-root")
    if args.authority == "pinned-source" and args.source is not None:
        raise RaylibTestFailure("pinned-source integration does not accept --source")
    if args.authority == "offline-source" and args.source is None:
        raise RaylibTestFailure("offline-source integration requires --source")
    if args.authority == "offline-source" and args.cache_root is not None:
        raise RaylibTestFailure("offline-source integration does not accept --cache-root")
    if args.run and args.configuration != "memory-software":
        raise RaylibTestFailure("desktop provisioning is compile/link-only and cannot use --run")


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        if args.integration:
            require_integration_args(args)
            report = integration_report(args)
            write_report(
                args.report,
                report,
                [
                    ROOT,
                    args.cache_root.resolve() if args.cache_root is not None else "",
                    args.build_root.resolve(),
                    args.source.resolve() if args.source is not None else "",
                    args.cc,
                    args.cxx,
                    args.cmake,
                ],
            )
            if args.print_report:
                print(canonical_json(report), end="")
            print(
                "raylib-provisioning: OK: generated Haxe compiled and linked against "
                f"raylib 6.0 ({args.platform}/{args.configuration}); executed={str(args.run).lower()}"
            )
            return 0
        if any(
            value
            for value in (
                args.authority,
                args.configuration,
                args.platform,
                args.cache_root,
                args.source,
                args.build_root,
                args.cc,
                args.cxx,
                args.cmake,
                args.generator,
                args.allow_network,
                args.run,
                args.report,
                args.print_report,
            )
        ):
            raise RaylibTestFailure("integration-only options require --integration")
        load_lock()
        run_unit_tests()
        values = snapshot_values()
        validate_expected(values)
        print(
            "raylib-provisioning: OK: lock, fail-closed authorities, five neutral plans, "
            "deterministic generated C, and zero-hxrt evidence"
        )
        return 0
    except (OSError, UnicodeError, json.JSONDecodeError, ProvisionFailure, RaylibTestFailure) as error:
        print(f"raylib-provisioning: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

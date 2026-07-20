#!/usr/bin/env python3
"""Build and launch the first native Caxecraft playable slice."""

from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import os
import platform
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
PROVISION_DIR = ROOT / "scripts/raylib"
sys.path.insert(0, str(PROVISION_DIR))
import provision  # type: ignore  # noqa: E402


STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic-errors",
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
PLATFORM_NAMES = {"Darwin": "macos", "Linux": "linux", "Windows": "windows"}
EXPECTED = CASE / "expected"
# Snapshots review generated structure, not whichever desktop runs the updater.
# Native build/play still selects the real host below, and the Raylib CI matrix
# separately validates each platform's libraries and frameworks.
SNAPSHOT_PLATFORM = "linux"
OUTPUT_MARKER = ".hxc-caxecraft-play-root.json"
PLAYABLE_SNAPSHOT_FORMATS = {
    "playable/hxc.manifest.json": "json",
    "playable/hxc.runtime-plan.json": "json",
    "playable/include/hxc/program.h": "header",
    "playable/src/modules/caxecraft/app/CaxecraftPalette.c": "c",
    "playable/src/modules/caxecraft/app/HudDigits.c": "c",
    "playable/src/modules/caxecraft/app/Main.c": "c",
}


class PlayFailure(RuntimeError):
    """The playable could not be built without weakening its contracts."""


def development_tool(name: str) -> str:
    local_name = f"{name}.cmd" if os.name == "nt" else name
    local = ROOT / "node_modules/.bin" / local_name
    return str(local) if local.is_file() else name


def run(arguments: list[str], *, cwd: Path, timeout: int, label: str) -> subprocess.CompletedProcess[str]:
    try:
        result = subprocess.run(
            arguments,
            cwd=cwd,
            env={**os.environ, "HAXE_NO_SERVER": "1", "LC_ALL": "C"},
            check=False,
            capture_output=True,
            text=True,
            timeout=timeout,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise PlayFailure(f"{label} could not run: {error}") from error
    if result.returncode != 0:
        detail = "\n".join(value.strip() for value in (result.stdout, result.stderr) if value.strip())
        suffix = f"\n{detail}" if detail else ""
        raise PlayFailure(f"{label} failed with exit {result.returncode}{suffix}")
    return result


def host_platform() -> str:
    value = PLATFORM_NAMES.get(platform.system())
    if value is None:
        raise PlayFailure(f"Caxecraft does not yet know how to link on {platform.system()!r}")
    return value


def tool_version(executable: str) -> str:
    result = run([executable, "--version"], cwd=ROOT, timeout=30, label=f"{executable} identity")
    for line in result.stdout.splitlines():
        if line.strip():
            return line.strip()
    raise PlayFailure(f"{executable} did not report a version")


def load_object(path: Path, label: str) -> dict[str, object]:
    try:
        value: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise PlayFailure(f"cannot read {label}: {error}") from error
    if not isinstance(value, dict):
        raise PlayFailure(f"{label} must contain a JSON object")
    return value


def text_array(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise PlayFailure(f"{label} must be a string array")
    return list(value)


def owned_fact_names(value: object, label: str) -> list[str]:
    if not isinstance(value, list):
        raise PlayFailure(f"{label} must be an array")
    names: list[str] = []
    for index, item in enumerate(value):
        if not isinstance(item, dict) or set(item) != {"name", "ownerModulePaths"}:
            raise PlayFailure(f"{label}[{index}] must contain a name and owners")
        name = item.get("name")
        owners = item.get("ownerModulePaths")
        if not isinstance(name, str) or not name or not text_array(owners, f"{label}[{index}].ownerModulePaths"):
            raise PlayFailure(f"{label}[{index}] contains an invalid name or empty owners")
        names.append(name)
    if len(names) != len(set(names)):
        raise PlayFailure(f"{label} contains duplicate names")
    return names


def prepare_output_root(path: Path) -> Path:
    forbidden = {Path(path.anchor), ROOT.resolve(), Path.home().resolve()}
    if path in forbidden:
        raise PlayFailure(f"--output-root cannot own broad directory {path}")
    if path.exists() and (not path.is_dir() or path.is_symlink()):
        raise PlayFailure(f"--output-root must be a real directory: {path}")
    path.mkdir(parents=True, exist_ok=True)
    marker = path / OUTPUT_MARKER
    if marker.is_file() and not marker.is_symlink():
        state = load_object(marker, "Caxecraft output ownership marker")
        if state != {"kind": "caxecraft-play-output", "schemaVersion": 1}:
            raise PlayFailure(f"Caxecraft output ownership marker is invalid: {marker}")
        return path
    if any(path.iterdir()):
        raise PlayFailure(f"unowned files occupy --output-root: {path}")
    marker.write_text(
        json.dumps({"kind": "caxecraft-play-output", "schemaVersion": 1}, sort_keys=True) + "\n",
        encoding="utf-8",
    )
    return path


def validated_relative(value: str, label: str) -> PurePosixPath:
    path = PurePosixPath(value)
    if path.is_absolute() or path.as_posix() != value or any(part in ("", ".", "..") for part in path.parts):
        raise PlayFailure(f"{label} must be a normalized relative path")
    return path


def compile_haxe(generated: Path, *, layout: str, platform_name: str) -> dict[str, object]:
    arguments = [
        development_tool("haxe"),
        "--cwd",
        str(CASE),
        "play.hxml",
        "-D",
        "hxc_runtime_diagnostics=off",
        "-D",
        f"raylib_platform_{platform_name}",
        "-D",
        "raylib_configuration_desktop",
    ]
    if layout != "split":
        arguments.extend(["-D", f"hxc_project_layout={layout}"])
    arguments.extend(["--custom-target", f"c={generated}"])
    run(arguments, cwd=ROOT, timeout=120, label="Caxecraft Haxe-to-C compile")

    manifest = load_object(generated / "hxc.manifest.json", "generated Caxecraft manifest")
    runtime_plan = load_object(generated / "hxc.runtime-plan.json", "generated Caxecraft runtime plan")
    configuration = manifest.get("configuration")
    build = manifest.get("build")
    if not isinstance(configuration, dict) or configuration.get("projectLayout") != layout:
        raise PlayFailure("generated Caxecraft manifest lost the requested project layout")
    if (
        manifest.get("compilationStatus") != "lowered-direct-value-executable"
        or not isinstance(build, dict)
        or build.get("artifact") != {"targetName": "hxc_program", "kind": "executable"}
    ):
        raise PlayFailure("generated Caxecraft manifest does not describe a direct executable")
    if runtime_plan.get("selectedFeatures") != [] or runtime_plan.get("artifacts") != []:
        raise PlayFailure("Caxecraft unexpectedly selected hxrt")
    validate_generated_playable(generated, layout=layout)
    return manifest


def validate_generated_playable(generated: Path, *, layout: str) -> None:
    sources = sorted(generated.glob("src/**/*.c"), key=lambda path: path.as_posix().encode("utf-8"))
    if not sources:
        raise PlayFailure("Caxecraft emitted no C sources")
    combined = "\n".join(path.read_text(encoding="utf-8") for path in sources)
    app_relative = {
        "split": "src/modules/caxecraft/app/Main.c",
        "package": "src/packages/caxecraft/app/package.c",
        "unity": "src/program.c",
    }.get(layout)
    if app_relative is None:
        raise PlayFailure(f"unknown generated Caxecraft layout {layout!r}")
    app_path = generated / app_relative
    if not app_path.is_file():
        raise PlayFailure(f"generated Caxecraft {layout} app source is missing: {app_relative}")
    app = app_path.read_text(encoding="utf-8")
    for required in (
        "InitWindow(",
        "WindowShouldClose(",
        "GetMouseDelta(",
        "BeginDrawing(",
        "BeginMode3D(",
        "DrawCube(",
        "DrawCubeWires(",
        "DrawText(",
    ):
        if required not in app:
            raise PlayFailure(f"generated Caxecraft app omitted direct Raylib call {required}")
    for forbidden in (r"\bgoto\b", r"\bmalloc\s*\(", r"\bcalloc\s*\(", r"\brealloc\s*\(", r"\bfree\s*\(", r"\bhxrt_"):
        if re.search(forbidden, combined):
            raise PlayFailure(f"generated Caxecraft sources contain forbidden pattern {forbidden}")


def snapshot_values() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-playable-snapshot-") as temporary:
        generated = Path(temporary) / "generated"
        compile_haxe(generated, layout="split", platform_name=SNAPSHOT_PLATFORM)
        values: dict[str, object] = {}
        for name, format_name in PLAYABLE_SNAPSHOT_FORMATS.items():
            relative = name.removeprefix("playable/")
            path = generated / relative
            values[name] = load_object(path, f"playable snapshot {name}") if format_name == "json" else path.read_text(encoding="utf-8")
        return values


def check_snapshots() -> None:
    actual = snapshot_values()
    for name, format_name in PLAYABLE_SNAPSHOT_FORMATS.items():
        expected_path = EXPECTED / name
        if not expected_path.is_file():
            raise PlayFailure(
                "playable snapshot is missing; run "
                "`npm run snapshots:update -- --suite caxecraft-domain`"
            )
        expected: object = load_object(expected_path, f"expected {name}") if format_name == "json" else expected_path.read_text(encoding="utf-8")
        if actual[name] == expected:
            continue
        detail = "semantic JSON differs"
        if isinstance(actual[name], str) and isinstance(expected, str):
            detail = "".join(
                difflib.unified_diff(
                    expected.splitlines(keepends=True),
                    actual[name].splitlines(keepends=True),
                    fromfile=f"expected/{name}",
                    tofile=f"actual/{name}",
                    n=3,
                )
            )[:4000]
        raise PlayFailure(
            f"playable snapshot {name!r} drifted; run "
            "`npm run snapshots:update -- --suite caxecraft-domain`\n"
            f"{detail}"
        )


def raylib_cache_key(
    *, authority: str, platform_name: str, source: Path | None, cc: str, cxx: str, cmake: str, generator: str
) -> str:
    identity = {
        "schemaVersion": 1,
        "authority": authority,
        "configuration": "desktop",
        "platform": platform_name,
        "source": str(source.resolve()) if source is not None else None,
        "cc": cc,
        "ccVersion": tool_version(cc),
        "cxx": cxx,
        "cxxVersion": tool_version(cxx),
        "cmake": cmake,
        "cmakeVersion": tool_version(cmake),
        "generator": generator,
        "raylibCommit": provision.PINNED_COMMIT,
    }
    encoded = json.dumps(identity, ensure_ascii=False, sort_keys=True, separators=(",", ":")).encode("utf-8")
    return hashlib.sha256(encoded).hexdigest()[:16]


def provision_raylib(
    *,
    authority: str,
    platform_name: str,
    source: Path | None,
    cache_root: Path,
    cc: str,
    cxx: str,
    cmake: str,
    generator: str,
    allow_network: bool,
    rebuild: bool,
) -> tuple[Path, Path]:
    lock = provision.load_lock()
    key = raylib_cache_key(
        authority=authority,
        platform_name=platform_name,
        source=source,
        cc=cc,
        cxx=cxx,
        cmake=cmake,
        generator=generator,
    )
    source_cache = cache_root / "source"
    build_root = cache_root / "build" / key
    state_path = build_root / "hxc-caxecraft-raylib.json"
    if rebuild and build_root.exists():
        if not state_path.is_file() or state_path.is_symlink():
            raise PlayFailure(f"cannot rebuild an unowned Raylib cache directory: {build_root}")
        state = load_object(state_path, "Caxecraft Raylib cache state")
        if state.get("schemaVersion") != 1 or state.get("raylibCommit") != provision.PINNED_COMMIT:
            raise PlayFailure(f"cannot rebuild an invalid Raylib cache directory: {build_root}")
        shutil.rmtree(build_root)

    if state_path.is_file() and not state_path.is_symlink():
        state = load_object(state_path, "Caxecraft Raylib cache state")
        if authority == "pinned-source":
            source_root = provision.pinned_source(source_cache, lock, allow_network=False)
        else:
            if source is None:
                raise PlayFailure("offline-source authority requires --source")
            source_root = source.resolve()
            provision.verify_source(source_root, lock)
        library = provision.locate_raylib_library(build_root, platform_name)
        if (
            state.get("schemaVersion") != 1
            or state.get("raylibCommit") != provision.PINNED_COMMIT
            or state.get("librarySha256") != provision.sha256_file(library)
        ):
            raise PlayFailure("cached Raylib build does not match its verified state; use --rebuild-raylib")
        return source_root / "src", library

    if build_root.exists():
        if any(build_root.iterdir()):
            raise PlayFailure(f"unowned files occupy the Raylib build cache: {build_root}")
    result = provision.build_source(
        lock=lock,
        authority=authority,
        configuration="desktop",
        platform_name=platform_name,
        cache_root=source_cache if authority == "pinned-source" else None,
        source_root=source if authority == "offline-source" else None,
        build_root=build_root,
        compiler=cc,
        cxx_compiler=cxx,
        cmake=cmake,
        generator=generator,
        allow_network=allow_network,
    )
    state = {
        "schemaVersion": 1,
        "raylibCommit": provision.PINNED_COMMIT,
        "librarySha256": provision.sha256_file(result.library_file),
        "provisioningReport": result.report,
    }
    state_path.write_text(json.dumps(state, ensure_ascii=False, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return result.include_directory, result.library_file


def resolve_prebuilt_raylib(*, cache_root: Path, build_root: Path, report_path: Path, platform_name: str) -> tuple[Path, Path]:
    lock = provision.load_lock()
    source_root = provision.pinned_source(cache_root.resolve(), lock, allow_network=False)
    library = provision.locate_raylib_library(build_root.resolve(), platform_name)
    integration_report = load_object(report_path, "prebuilt Raylib integration report")
    report = integration_report.get("provision")
    if not isinstance(report, dict):
        raise PlayFailure("prebuilt Raylib integration report omitted its provisioning record")
    target = report.get("target")
    configuration = report.get("configuration")
    outputs = report.get("outputs")
    claims = report.get("claims")
    library_report = outputs.get("library") if isinstance(outputs, dict) else None
    if (
        report.get("authority") != "pinned-source"
        or not isinstance(target, dict)
        or target.get("platform") != platform_name
        or not isinstance(configuration, dict)
        or configuration.get("id") != "desktop"
        or not isinstance(claims, dict)
        or claims.get("raylibBuilt") is not True
        or not isinstance(library_report, dict)
        or library_report.get("sha256") != provision.sha256_file(library)
    ):
        raise PlayFailure("prebuilt Raylib inputs do not match the pinned desktop provisioning report")
    return source_root / "src", library


def compile_native(
    generated: Path,
    manifest: dict[str, object],
    *,
    output: Path,
    include_directory: Path,
    library: Path,
    platform_name: str,
    cc: str,
    optimization: str,
) -> None:
    if platform_name == "windows":
        raise PlayFailure("the one-command Windows linker adapter is deferred; generated C remains available with --compile-only")
    build = manifest.get("build")
    if not isinstance(build, dict):
        raise PlayFailure("generated Caxecraft manifest omitted its build plan")
    source_values = text_array(build.get("sources"), "generated Caxecraft sources")
    object_root = output.parent / "obj"
    if object_root.exists():
        shutil.rmtree(object_root)
    object_root.mkdir(parents=True)
    objects: list[Path] = []
    for index, source_value in enumerate(source_values):
        relative = validated_relative(source_value, f"generated source {index}")
        source_path = generated.joinpath(*relative.parts)
        if not source_path.is_file():
            raise PlayFailure(f"generated source is missing: {source_value}")
        object_path = object_root / f"{index:03d}.o"
        run(
            [
                cc,
                *STRICT_FLAGS,
                f"-O{optimization}",
                "-I",
                str(generated / "include"),
                "-I",
                str(include_directory),
                "-c",
                str(source_path),
                "-o",
                str(object_path),
            ],
            cwd=ROOT,
            timeout=180,
            label=f"native compile of {source_value}",
        )
        objects.append(object_path)

    lock = provision.load_lock()
    libraries, frameworks = provision.link_facts(lock, platform_name, "desktop")
    manifest_libraries = owned_fact_names(build.get("libraries"), "generated Caxecraft libraries")
    manifest_frameworks = owned_fact_names(build.get("frameworks"), "generated Caxecraft frameworks")
    if len(manifest_libraries) != len(libraries) or set(manifest_libraries) != set(libraries):
        raise PlayFailure("generated Caxecraft libraries differ from the pinned Raylib link plan")
    if len(manifest_frameworks) != len(frameworks) or set(manifest_frameworks) != set(frameworks):
        raise PlayFailure("generated Caxecraft frameworks differ from the pinned Raylib link plan")
    arguments = [cc, *[str(path) for path in objects], str(library)]
    for name in libraries:
        if name != "raylib":
            arguments.append(f"-l{name}")
    for name in frameworks:
        arguments.extend(["-framework", name])
    output.parent.mkdir(parents=True, exist_ok=True)
    arguments.extend(["-o", str(output)])
    run(arguments, cwd=ROOT, timeout=180, label="Caxecraft native link")


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check-snapshots", action="store_true", help="compile once and compare the registered playable snapshots")
    parser.add_argument("--compile-only", action="store_true", help="emit and validate C without provisioning or linking Raylib")
    parser.add_argument("--build-only", action="store_true", help="link the native executable without opening a window")
    parser.add_argument("--allow-network", action="store_true", help="allow the first checksum-pinned Raylib archive download")
    parser.add_argument("--authority", choices=("pinned-source", "offline-source"), default="pinned-source")
    parser.add_argument("--source", type=Path, help="exact Raylib 6.0 source tree for offline-source authority")
    parser.add_argument("--layout", choices=("split", "package", "unity"), default="split")
    parser.add_argument("--optimization", choices=("0", "2"), default="2")
    parser.add_argument("--cc", default=os.environ.get("CC", "clang" if platform.system() == "Darwin" else "gcc"))
    parser.add_argument("--cxx", default=os.environ.get("CXX", "clang++" if platform.system() == "Darwin" else "g++"))
    parser.add_argument("--cmake", default="cmake")
    parser.add_argument("--generator", choices=("Ninja", "Unix Makefiles"), default="Ninja" if shutil.which("ninja") else "Unix Makefiles")
    parser.add_argument("--cache-root", type=Path, default=ROOT / ".cache/caxecraft/raylib")
    parser.add_argument("--output-root", type=Path, default=CASE / "_build/play")
    parser.add_argument("--rebuild-raylib", action="store_true")
    parser.add_argument("--prebuilt-raylib-cache", type=Path, help="verified pinned-source cache produced by the Raylib integration lane")
    parser.add_argument("--prebuilt-raylib-build", type=Path, help="verified desktop build produced by the Raylib integration lane")
    parser.add_argument("--prebuilt-raylib-report", type=Path, help="normalized report for the verified prebuilt Raylib library")
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    try:
        args = parse_args(argv)
        if args.check_snapshots:
            check_snapshots()
            print("caxecraft: playable snapshots and direct-C invariants passed")
            return 0
        platform_name = host_platform()
        if args.authority == "offline-source" and args.source is None:
            raise PlayFailure("--authority offline-source requires --source")
        if args.authority == "pinned-source" and args.source is not None:
            raise PlayFailure("--source is accepted only with --authority offline-source")
        output_root = prepare_output_root(args.output_root.resolve())
        generated = output_root / "generated"
        executable = output_root / "bin" / ("caxecraft.exe" if platform_name == "windows" else "caxecraft")
        manifest = compile_haxe(generated, layout=args.layout, platform_name=platform_name)
        print(f"caxecraft: generated {args.layout} C project at {generated}")
        if args.compile_only:
            print("caxecraft: compile-only proof passed (direct C, empty hxrt plan)")
            return 0

        prebuilt_values = (args.prebuilt_raylib_cache, args.prebuilt_raylib_build, args.prebuilt_raylib_report)
        if any(value is not None for value in prebuilt_values):
            if not all(value is not None for value in prebuilt_values):
                raise PlayFailure("prebuilt Raylib reuse requires cache, build, and report paths together")
            if args.allow_network or args.source is not None or args.rebuild_raylib:
                raise PlayFailure("prebuilt Raylib reuse rejects network, source, and rebuild options")
            include_directory, library = resolve_prebuilt_raylib(
                cache_root=args.prebuilt_raylib_cache,
                build_root=args.prebuilt_raylib_build,
                report_path=args.prebuilt_raylib_report,
                platform_name=platform_name,
            )
        else:
            include_directory, library = provision_raylib(
                authority=args.authority,
                platform_name=platform_name,
                source=args.source,
                cache_root=args.cache_root.resolve(),
                cc=args.cc,
                cxx=args.cxx,
                cmake=args.cmake,
                generator=args.generator,
                allow_network=args.allow_network,
                rebuild=args.rebuild_raylib,
            )
        compile_native(
            generated,
            manifest,
            output=executable,
            include_directory=include_directory,
            library=library,
            platform_name=platform_name,
            cc=args.cc,
            optimization=args.optimization,
        )
        print(f"caxecraft: built native executable at {executable}")
        if args.build_only:
            return 0
        print("caxecraft: launching; press Q to quit")
        return subprocess.run([str(executable)], cwd=CASE, check=False).returncode
    except (OSError, UnicodeError, provision.ProvisionFailure, PlayFailure) as error:
        print(f"caxecraft: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

#!/usr/bin/env python3
"""Build and run independent C/C++ consumers of pinned RayguiHx inputs."""

from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path
from typing import Sequence

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.raygui.provision import (  # noqa: E402
    RayguiProvisionFailure,
    build_static,
    load_lock as load_raygui_lock,
    pinned_source,
    sha256_file,
    verify_raygui_source,
)
from scripts.raylib.provision import link_facts, load_lock as load_raylib_lock  # noqa: E402


class NativeRayguiFailure(RuntimeError):
    """The selected headers, static libraries, or consumers disagreed."""


STRICT = (
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wpedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wundef",
    "-Wformat=2",
)


def run(arguments: Sequence[str], *, cwd: Path, label: str) -> subprocess.CompletedProcess[str]:
    try:
        result = subprocess.run(
            list(arguments),
            cwd=cwd,
            env={**os.environ, "LC_ALL": "C"},
            check=False,
            capture_output=True,
            text=True,
            timeout=120,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise NativeRayguiFailure(f"{label} could not run: {error}") from error
    if result.returncode != 0:
        detail = "\n".join(part.strip() for part in (result.stdout, result.stderr) if part.strip())
        raise NativeRayguiFailure(f"{label} failed with exit {result.returncode}\n{detail}".rstrip())
    return result


def link_arguments(
    *,
    compiler: str,
    object_file: Path,
    output: Path,
    raygui_library: Path,
    raylib_library: Path,
    libraries: tuple[str, ...],
    frameworks: tuple[str, ...],
) -> list[str]:
    # The archive that uses a symbol precedes the archive that defines it.
    arguments = [compiler, str(object_file), str(raygui_library), str(raylib_library)]
    for library in libraries:
        if library != "raylib":
            arguments.append(f"-l{library}")
    for framework in frameworks:
        arguments.extend(("-framework", framework))
    arguments.extend(("-o", str(output)))
    return arguments


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--raygui-source", type=Path)
    parser.add_argument("--raygui-cache-root", type=Path)
    parser.add_argument("--allow-network", action="store_true")
    parser.add_argument("--raylib-source", type=Path, required=True)
    parser.add_argument("--raylib-library", type=Path, required=True)
    parser.add_argument("--platform", choices=("linux", "macos"), required=True)
    parser.add_argument("--configuration", choices=("desktop", "memory-software"), required=True)
    parser.add_argument("--build-root", type=Path, required=True)
    parser.add_argument("--cc", default=os.environ.get("CC", "cc"))
    parser.add_argument("--cxx", default=os.environ.get("CXX", "c++"))
    parser.add_argument("--ar", default=os.environ.get("AR", "ar"))
    parser.add_argument("--report", type=Path)
    return parser.parse_args(argv)


def main(argv: Sequence[str]) -> int:
    try:
        args = parse_args(argv)
        if (args.raygui_source is None) == (args.raygui_cache_root is None):
            raise NativeRayguiFailure("choose exactly one of --raygui-source or --raygui-cache-root")
        if args.raygui_source is not None:
            if args.allow_network:
                raise NativeRayguiFailure("offline --raygui-source rejects --allow-network")
            raygui_source = verify_raygui_source(args.raygui_source, load_raygui_lock())
        else:
            raygui_source = pinned_source(args.raygui_cache_root, load_raygui_lock(), args.allow_network)
        root = args.build_root.resolve()
        if root.exists() and (root.is_symlink() or not root.is_dir() or any(root.iterdir())):
            raise NativeRayguiFailure(f"native Raygui build root must be empty: {root}")
        root.mkdir(parents=True, exist_ok=True)
        raygui = build_static(
            source_root=raygui_source,
            raylib_source=args.raylib_source,
            build_root=root / "raygui",
            compiler=args.cc,
            archiver=args.ar,
        )
        libraries, frameworks = link_facts(load_raylib_lock(), args.platform, args.configuration)
        results: list[dict[str, object]] = []
        for language, compiler, standard, source in (
            ("c", args.cc, "-std=c11", ROOT / "test/raygui_binding/native/core_consumer.c"),
            ("cpp", args.cxx, "-std=c++17", ROOT / "test/raygui_binding/native/core_consumer.cpp"),
        ):
            object_file = root / f"consumer-{language}.o"
            executable = root / f"consumer-{language}"
            # raylib 6.0's public header expands __STDC_VERSION__ in one C++
            # preprocessor condition. Keep -Wundef enabled and its diagnostic
            # in the report, but do not make that exact upstream warning fatal.
            upstream_exceptions = ("-Wno-error=undef",) if language == "cpp" else ()
            compiled = run(
                (
                    compiler,
                    standard,
                    *STRICT,
                    *upstream_exceptions,
                    "-I",
                    str(raygui.include_directory),
                    "-I",
                    str(args.raylib_source.resolve() / "src"),
                    "-c",
                    str(source),
                    "-o",
                    str(object_file),
                ),
                cwd=ROOT,
                label=f"Raygui {language} consumer compile",
            )
            run(
                link_arguments(
                    compiler=compiler,
                    object_file=object_file,
                    output=executable,
                    raygui_library=raygui.library_file,
                    raylib_library=args.raylib_library.resolve(),
                    libraries=libraries,
                    frameworks=frameworks,
                ),
                cwd=ROOT,
                label=f"Raygui {language} consumer link",
            )
            execution = run((str(executable),), cwd=root, label=f"Raygui {language} consumer run")
            expected = f"raygui-{language}-consumer: OK\n"
            if execution.stdout != expected or execution.stderr:
                raise NativeRayguiFailure(f"Raygui {language} consumer output drifted")
            results.append(
                {
                    "language": language,
                    "sourceSha256": sha256_file(source),
                    "objectSha256": sha256_file(object_file),
                    "executableSha256": sha256_file(executable),
                    "stdout": execution.stdout,
                    "compileDiagnostics": compiled.stderr,
                }
            )
        report = {
            "schemaVersion": 1,
            "raygui": raygui.report,
            "raylibLibrarySha256": sha256_file(args.raylib_library.resolve()),
            "platform": args.platform,
            "configuration": args.configuration,
            "consumers": results,
        }
        rendered = json.dumps(report, ensure_ascii=False, indent=2, sort_keys=True) + "\n"
        if args.report is not None:
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(rendered, encoding="utf-8", newline="\n")
        print("raygui-native: OK: independent C11 and C++17 consumers")
    except (NativeRayguiFailure, RayguiProvisionFailure, OSError, ValueError) as error:
        print(f"raygui-native: ERROR: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

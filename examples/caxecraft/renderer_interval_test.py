"""Run Haxe observer imports and reporting against independent native hooks.

These cases need native allocator and collector calls, so Eval cannot observe
their contract. Each HXML produces C once; all compiler modes reuse those bytes.
The regular Caxecraft test entry point owns this focused command.
"""
from __future__ import annotations

import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from dev_haxe_server import pinned_haxe_environment, pinned_haxe_installation
from renderer_interval import PREFIX, parse_interval_evidence
from scripts.test.bounded_process import run as run_bounded_process
from scripts.test.c_fixture_harness import C11_STRICT_FLAGS
from scripts.test.compiler_discovery import compiler_family, resolve_compiler, sanitizer_execution_environment

ROOT = Path(__file__).resolve().parents[2]
FIXTURE = ROOT / "examples/caxecraft/test/renderer_interval"
RUNTIME = ROOT / "runtime/hxrt"
SENTINEL = "interval-import: OK allocation=1 root-push=1 root-pop=1 wide=4294967297\n"
WIDE = 4294967297


def _run(arguments: list[str], *, timeout: int, environment: dict[str, str], label: str) -> subprocess.CompletedProcess[str]:
    """Keep compiler diagnostics and stop the whole process tree on timeout."""
    try:
        result = run_bounded_process(
            arguments, cwd=ROOT, env=environment, capture_output=True, text=True,
            timeout=timeout, phase=label,
        )
    except subprocess.TimeoutExpired as error:
        raise RuntimeError(
            f"{label}: {error}\nstdout:\n{error.output!r}\nstderr:\n{error.stderr!r}"
        ) from error
    if result.returncode != 0 or result.stderr:
        raise RuntimeError(
            f"{label}: exit={result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return result


def _paths(output: Path, value: object, *, directory: bool = False) -> list[Path]:
    """Accept only existing, unique inputs inside the generated project."""
    if not isinstance(value, list) or not value:
        raise RuntimeError("generated manifest omitted sources or include directories")
    paths: list[Path] = []
    for spelling in value:
        if not isinstance(spelling, str) or not spelling or "\\" in spelling:
            raise RuntimeError("generated manifest contains an invalid path")
        relative = Path(spelling)
        if relative.is_absolute() or ".." in relative.parts:
            raise RuntimeError("generated manifest path escapes its output root")
        path = (output / relative).resolve()
        path.relative_to(output.resolve())
        if not (path.is_dir() if directory else path.is_file()) or path in paths:
            raise RuntimeError("generated manifest input is missing or repeated")
        paths.append(path)
    return paths


def native_inputs(output: Path) -> tuple[list[Path], list[Path]]:
    """Use generated runtime sources first; add only the native fixture's needs.

    Reporting selects allocator and string support itself. Linking a second
    repository allocator would duplicate symbols and bypass generated packaging.
    The native consumer separately needs object and collector support.
    """
    manifest = json.loads((output / "hxc.manifest.json").read_text(encoding="utf-8"))
    build = manifest.get("build") if isinstance(manifest, dict) else None
    if not isinstance(build, dict) or build.get("schemaVersion") != 1 or build.get("cStandard") != "c11":
        raise RuntimeError("generated manifest lost its C11 build plan")
    if build.get("extensions") is not False or any(
        build.get(name) != [] for name in ("definitions", "libraries", "frameworks", "pkgConfigPackages")
    ):
        raise RuntimeError("observer fixture requested unsupported native build options")
    sources = _paths(output, build.get("sources"))
    includes = _paths(output, build.get("includeDirectories"), directory=True)
    provided = {
        path.name for path in sources
        if path.relative_to(output.resolve()).as_posix().startswith("runtime/src/")
    }
    sources.extend(
        RUNTIME / "src" / name
        for name in ("allocator.c", "object.c", "gc.c") if name not in provided
    )
    sources.extend((RUNTIME / "test/interval_observer.c", FIXTURE / "interval_fixture.c"))
    if len(set(sources)) != len(sources):
        raise RuntimeError("observer fixture repeats a native source")
    includes.extend((FIXTURE, RUNTIME / "include", RUNTIME / "test"))
    return sources, includes


def validate_output(stdout: str, *, reporting: bool) -> None:
    """Consume actual executable output through the benchmark's report parser."""
    if not reporting:
        if stdout != SENTINEL:
            raise RuntimeError(f"controlled observer result differs: {stdout!r}")
        return
    lines = stdout.splitlines(keepends=True)
    if len(lines) != 3 or not all(line.startswith(PREFIX) for line in lines[:2]) or lines[2] != SENTINEL:
        raise RuntimeError("reporting fixture lost its two snapshots or native sentinel")
    evidence = parse_interval_evidence(stdout, measured_frames=1)
    warmup = {
        "schemaVersion": 1, "phase": "warmup", "valid": True,
        "frames": 2, "portalFrames": 0, "completed": 2,
        "allocationAttempts": 1, "reallocationAttempts": 0, "collectionAttempts": 0,
        "rootPushAttempts": WIDE, "rootPopAttempts": 0,
    }
    measured = {
        "frames": 1, "portalFrames": 0, "completed": 1,
        "allocationAttempts": 0, "reallocationAttempts": 0, "collectionAttempts": 0,
        "rootPushAttempts": 1, "rootPopAttempts": 1,
    }
    total = {**warmup, "phase": "complete", "frames": 3, "completed": 3,
             "rootPushAttempts": WIDE + 1, "rootPopAttempts": 1}
    if evidence["warmup"] != warmup or evidence["measured"] != measured or evidence["total"] != total:
        raise RuntimeError("actual interval report differs from the controlled native observations")


def run_renderer_interval_tests(requested_cc: str | None, layout: str) -> None:
    """Generate each fixture once, then require strict native and sanitizer runs."""
    if requested_cc is None:
        compilers = [(family, resolve_compiler(family)) for family in ("gcc", "clang")]
    else:
        compiler = shutil.which(requested_cc)
        family = compiler_family(compiler) if compiler is not None else None
        compilers = [(family, compiler)]
    if any(family not in ("gcc", "clang") or compiler is None for family, compiler in compilers):
        raise RuntimeError("observer tests require identity-matching GCC and Clang; --cc selects one focused compiler")
    installation = pinned_haxe_installation(scope_root=ROOT)
    environment = pinned_haxe_environment("C", installation)
    environment["HAXE_NO_SERVER"] = "1"
    version = _run([str(installation.compiler), "--version"], timeout=10, environment=environment, label="Haxe identity")
    if (version.stdout + version.stderr).strip() != installation.version:
        raise RuntimeError("observer fixture compiler differs from .haxerc")
    shim = ROOT / "node_modules/.bin/haxe"
    if not shim.is_file():
        raise RuntimeError("observer fixture requires the checkout Haxe shim; run npm ci")
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-renderer-interval-") as temporary:
        root = Path(temporary)
        for hxml, reporting in (("build.hxml", False), ("reporting.hxml", True)):
            output = root / hxml.removesuffix(".hxml")
            _run(
                [str(shim), "--cwd", str(FIXTURE), hxml,
                 "-D", "hxc_runtime_diagnostics=off", "-D", f"hxc_project_layout={layout}",
                 "--custom-target", f"c={output}"],
                timeout=60, environment=environment, label=f"{hxml} Haxe generation",
            )
            sources, includes = native_inputs(output)
            for family, compiler in compilers:
                for optimization, sanitized in (("O0", False), ("O2", False), ("O1", True)):
                    executable = root / f"{hxml}-{family}-{optimization}"
                    flags = [f"-{optimization}"]
                    if sanitized:
                        flags.extend(("-g", "-fno-omit-frame-pointer", "-fsanitize=address,undefined"))
                    label = f"{hxml} {family} {optimization} sanitizers={sanitized}"
                    compiled = _run(
                        [compiler, *C11_STRICT_FLAGS, *flags,
                         *(f"-I{path}" for path in includes),
                         "-include", str(RUNTIME / "test/interval_observer_hooks.h"),
                         *(str(path) for path in sources), "-o", str(executable)],
                        timeout=30, environment=environment, label=f"{label} native compile",
                    )
                    if compiled.stdout:
                        raise RuntimeError(f"{label}: unexpected compiler stdout: {compiled.stdout!r}")
                    native_environment = {
                        **environment,
                        "ASAN_OPTIONS": ("detect_leaks=0" if sys.platform == "darwin" else "detect_leaks=1") + ":halt_on_error=1",
                        "UBSAN_OPTIONS": "halt_on_error=1:print_stacktrace=1",
                    }
                    result = _run([str(executable)], timeout=30, environment=sanitizer_execution_environment(
                        compiler, family, flags, environment=native_environment
                    ), label=f"{label} execution")
                    validate_output(result.stdout, reporting=reporting)

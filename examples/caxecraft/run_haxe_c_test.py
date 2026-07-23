#!/usr/bin/env python3
"""Run Haxe-authored Caxecraft tests on Eval and generated native C."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


CASE_ROOT = Path(__file__).resolve().parent
ROOT = CASE_ROOT.parents[1]
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
)
SANITIZER_FLAGS = (
    "-O1",
    "-g",
    "-fno-omit-frame-pointer",
    "-fsanitize=address,undefined",
)


@dataclass(frozen=True)
class HaxeCTestCase:
    """Closed host-side facts for one Haxe-authored native test."""

    case_id: str
    eval_hxml: str
    c_hxml: str
    native_harness: str
    generated_source: str
    required_source_markers: tuple[str, ...]
    forbidden_source_markers: tuple[str, ...]
    output_line_count: int
    success_line: str


CASES = {
    "app-screen": HaxeCTestCase(
        case_id="app-screen",
        eval_hxml="app-screen.hxml",
        c_hxml="app-screen-c.hxml",
        native_harness="test/native/app_screen_harness.c",
        generated_source="src/modules/caxecraft/app/AppScreen.c",
        required_source_markers=(
            "AppScreen_initialScreen",
            "AppScreen_startPlaying",
            "AppScreen_loseFocus",
            "AppScreen_togglePause",
            "AppScreen_recapture",
            "switch (hxc_screen)",
        ),
        forbidden_source_markers=("goto ",),
        output_line_count=1,
        success_line="0",
    ),
    "aquatics": HaxeCTestCase(
        case_id="aquatics",
        eval_hxml="aquatics.hxml",
        c_hxml="aquatics-c.hxml",
        native_harness="test/native/aquatics_harness.c",
        generated_source="src/modules/caxecraft/domain/GameSession.c",
        required_source_markers=("GameSession_tick", "GameSession *hxc_self"),
        forbidden_source_markers=("goto ",),
        output_line_count=2,
        success_line="0",
    ),
    "presentation": HaxeCTestCase(
        case_id="presentation",
        eval_hxml="presentation.hxml",
        c_hxml="presentation-c.hxml",
        native_harness="test/native/presentation_harness.c",
        generated_source="src/modules/caxecraft/app/MotionInterpolation.c",
        required_source_markers=(
            "MotionInterpolation_start",
            "MotionInterpolation_advance",
            "MotionInterpolation_sample",
        ),
        forbidden_source_markers=("goto ",),
        output_line_count=1,
        success_line="0",
    ),
    "water": HaxeCTestCase(
        case_id="water",
        eval_hxml="water.hxml",
        c_hxml="water-c.hxml",
        native_harness="test/native/water_harness.c",
        generated_source="src/modules/caxecraft/domain/WaterSimulation.c",
        required_source_markers=("WaterSimulation_tick",),
        forbidden_source_markers=("goto ",),
        output_line_count=2,
        success_line="0",
    ),
    "session": HaxeCTestCase(
        case_id="session",
        eval_hxml="session.hxml",
        c_hxml="session-c.hxml",
        native_harness="test/native/session_harness.c",
        generated_source="src/modules/caxecraft/domain/GameSession.c",
        required_source_markers=(
            "GameSession_writeTerrainRunDuringLoad",
            "GameSession_placeInitialWaterVolume",
            "GameSession_authoredItemIsActive",
            "GameSession_bindLocalPlayer",
            "GameSession_view",
            "hxc_completedTicks",
            "hxc_tickIndex",
        ),
        forbidden_source_markers=("goto ",),
        output_line_count=2,
        success_line="0",
    ),
    "terrain-chunks": HaxeCTestCase(
        case_id="terrain-chunks",
        eval_hxml="terrain-chunks.hxml",
        c_hxml="terrain-chunks-c.hxml",
        native_harness="test/native/terrain_chunks_harness.c",
        generated_source="src/modules/caxecraft/app/TerrainChunkCache.c",
        required_source_markers=(
            "TerrainChunkCache_prepare",
            "TerrainChunkCache_invalidate",
            "TerrainChunkCache_rebuild",
            "hxc_faceX",
            "hxc_packedFaces",
        ),
        forbidden_source_markers=("goto ", "malloc(", "calloc("),
        output_line_count=1,
        success_line="0",
    ),
}


class HaxeCTestFailure(RuntimeError):
    """The host could not prove a Haxe-authored test on both targets."""


def development_tool(name: str) -> str:
    """Resolve a checkout-owned tool before the host fallback."""

    local = ROOT / "node_modules/.bin" / name
    if local.is_file():
        return str(local)
    resolved = shutil.which(name)
    if resolved is None:
        raise HaxeCTestFailure(f"required tool {name!r} is unavailable")
    return resolved


def native_compiler(requested: str | None) -> str:
    """Resolve an explicit compiler or choose one fast local default."""

    resolved = shutil.which(requested) if requested is not None else (
        shutil.which("clang") or shutil.which("gcc") or shutil.which("cc")
    )
    if resolved is None:
        if requested is not None:
            raise HaxeCTestFailure(
                f"requested native C compiler {requested!r} is unavailable"
            )
        raise HaxeCTestFailure("no native C compiler is available")
    return resolved


def run(
    arguments: list[str],
    *,
    cwd: Path,
    timeout: int,
    label: str,
    environment: dict[str, str] | None = None,
) -> subprocess.CompletedProcess[str]:
    """Run one bounded process and preserve stdout and stderr on failure."""

    try:
        result = subprocess.run(
            arguments,
            cwd=cwd,
            env={
                **os.environ,
                "HAXE_NO_SERVER": "1",
                "LC_ALL": "C",
                **(environment or {}),
            },
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            timeout=timeout,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise HaxeCTestFailure(f"{label} could not run: {error}") from error
    if result.returncode != 0:
        raise HaxeCTestFailure(
            f"{label} failed with exit {result.returncode}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    return result


def checked_case_path(spelling: str, label: str) -> Path:
    """Resolve one reviewed case-relative path without accepting traversal."""

    parsed = PurePosixPath(spelling)
    if parsed.is_absolute() or ".." in parsed.parts or "\\" in spelling:
        raise HaxeCTestFailure(f"{label} is not a safe case-relative path")
    resolved = CASE_ROOT.joinpath(*parsed.parts)
    if not resolved.is_file():
        raise HaxeCTestFailure(f"{label} does not exist: {spelling}")
    return resolved


def checked_relative_parts(spelling: str, label: str) -> tuple[str, ...]:
    """Validate an output-relative path before resolving generated evidence."""

    parsed = PurePosixPath(spelling)
    if parsed.is_absolute() or ".." in parsed.parts or "\\" in spelling:
        raise HaxeCTestFailure(f"{label} is not a safe relative path")
    return parsed.parts


def validate_oracle(test_case: HaxeCTestCase, output: str) -> None:
    """Validate only the shared test protocol; Haxe owns mechanic assertions."""

    lines = output.splitlines()
    if (
        len(lines) != test_case.output_line_count
        or not lines
        or lines[0] != test_case.success_line
        or not output.endswith("\n")
    ):
        raise HaxeCTestFailure(
            f"{test_case.case_id} Eval test emitted an invalid result envelope: {output!r}"
        )


def sanitizer_supported(compiler: str, root: Path) -> bool:
    """Ask whether the selected compiler can link Address/Undefined sanitizers."""

    source = root / "sanitizer-probe.c"
    executable = root / "sanitizer-probe"
    source.write_text("int main(void) { return 0; }\n", encoding="utf-8", newline="\n")
    try:
        result = subprocess.run(
            [compiler, *SANITIZER_FLAGS, str(source), "-o", str(executable)],
            cwd=ROOT,
            env={**os.environ, "LC_ALL": "C"},
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            timeout=30,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise HaxeCTestFailure(f"sanitizer capability probe could not run: {error}") from error
    return result.returncode == 0


def generated_sources(root: Path) -> list[Path]:
    """Return generated modules while leaving compiler-owned main to the harness."""

    return sorted(
        (
            path
            for path in root.rglob("*.c")
            if path.relative_to(root).as_posix() != "src/hxc/main.c"
        ),
        key=lambda path: path.as_posix().encode("utf-8"),
    )


def compile_native(
    compiler: str,
    generated: Path,
    sources: list[Path],
    harness: Path,
    executable: Path,
    *,
    sanitized: bool,
) -> None:
    """Compile one generated test plus its independent native ABI consumer."""

    flags = [*STRICT_FLAGS, *(SANITIZER_FLAGS if sanitized else ())]
    run(
        [
            compiler,
            *flags,
            "-I",
            str(generated / "include"),
            *(str(path) for path in sources),
            str(harness),
            "-o",
            str(executable),
        ],
        cwd=ROOT,
        timeout=60,
        label=f"{'sanitized ' if sanitized else ''}strict native build",
    )


def execute(test_case: HaxeCTestCase, requested_compiler: str | None) -> bool:
    """Run one Haxe test on Eval, haxe.c, strict C, and optional sanitizers."""

    eval_hxml = checked_case_path(test_case.eval_hxml, "Eval HXML")
    c_hxml = checked_case_path(test_case.c_hxml, "C HXML")
    harness = checked_case_path(test_case.native_harness, "native harness")
    oracle = run(
        [development_tool("haxe"), "--cwd", str(CASE_ROOT), eval_hxml.name],
        cwd=ROOT,
        timeout=30,
        label=f"{test_case.case_id} Eval test",
    ).stdout
    validate_oracle(test_case, oracle)

    compiler = native_compiler(requested_compiler)
    with tempfile.TemporaryDirectory(prefix=f"hxc-caxecraft-{test_case.case_id}-") as temporary:
        temporary_root = Path(temporary)
        generated = temporary_root / "generated"
        run(
            [
                development_tool("haxe"),
                "--cwd",
                str(CASE_ROOT),
                c_hxml.name,
                "--custom-target",
                f"c={generated}",
            ],
            cwd=ROOT,
            timeout=60,
            label=f"{test_case.case_id} Haxe-to-C build",
        )
        runtime_plan = json.loads(
            (generated / "hxc.runtime-plan.json").read_text(encoding="utf-8")
        )
        if not isinstance(runtime_plan, dict):
            raise HaxeCTestFailure(
                f"{test_case.case_id} runtime plan is not a JSON object"
            )
        if runtime_plan.get("features") != []:
            raise HaxeCTestFailure(f"{test_case.case_id} unexpectedly selected hxrt")

        generated_source = generated.joinpath(
            *checked_relative_parts(
                test_case.generated_source, "generated source path"
            )
        )
        if not generated_source.is_file():
            raise HaxeCTestFailure(
                f"{test_case.case_id} omitted {test_case.generated_source}"
            )
        source_text = generated_source.read_text(encoding="utf-8")
        for marker in test_case.required_source_markers:
            if marker not in source_text:
                raise HaxeCTestFailure(
                    f"{test_case.case_id} generated source omitted {marker!r}"
                )
        for marker in test_case.forbidden_source_markers:
            if marker in source_text:
                raise HaxeCTestFailure(
                    f"{test_case.case_id} generated source retained {marker!r}"
                )

        sources = generated_sources(generated)
        executable = temporary_root / test_case.case_id
        compile_native(
            compiler, generated, sources, harness, executable, sanitized=False
        )
        native_output = run(
            [str(executable)],
            cwd=ROOT,
            timeout=20,
            label=f"{test_case.case_id} native test",
        ).stdout
        if native_output != oracle:
            raise HaxeCTestFailure(
                f"{test_case.case_id} Eval/native results differ\n"
                f"Eval={oracle!r}\nnative={native_output!r}"
            )

        sanitizer_ran = sanitizer_supported(compiler, temporary_root)
        if sanitizer_ran:
            sanitized = temporary_root / f"{test_case.case_id}-sanitized"
            compile_native(
                compiler, generated, sources, harness, sanitized, sanitized=True
            )
            sanitized_output = run(
                [str(sanitized)],
                cwd=ROOT,
                timeout=20,
                label=f"{test_case.case_id} sanitized native test",
                environment={
                    "ASAN_OPTIONS": "halt_on_error=1:abort_on_error=1",
                    "UBSAN_OPTIONS": "halt_on_error=1:print_stacktrace=1",
                },
            ).stdout
            if sanitized_output != oracle:
                raise HaxeCTestFailure(
                    f"{test_case.case_id} sanitizer run changed the test result"
                )
        return sanitizer_ran


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("case", choices=tuple(sorted(CASES)))
    parser.add_argument(
        "--cc",
        help="explicit C compiler command or path; the default keeps the focused local lane fast",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    test_case = CASES[args.case]
    try:
        sanitizer_ran = execute(test_case, args.cc)
    except (HaxeCTestFailure, OSError, UnicodeError, json.JSONDecodeError) as error:
        print(
            f"caxecraft-haxe-c-test: ERROR [{test_case.case_id}]: {error}",
            file=sys.stderr,
        )
        return 1
    sanitizer_status = "ran" if sanitizer_ran else "unavailable"
    print(
        f"caxecraft-haxe-c-test: OK [{test_case.case_id}]: "
        f"Haxe assertions passed on Eval/native C; runtime-free; sanitizers={sanitizer_status}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

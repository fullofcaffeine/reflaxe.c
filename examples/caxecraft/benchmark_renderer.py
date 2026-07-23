#!/usr/bin/env python3
"""Compare Caxecraft's chunk cache with its retained immediate renderer."""

from __future__ import annotations

import argparse
import copy
import difflib
import hashlib
import json
import os
import platform
import re
import shutil
import statistics
import subprocess
import sys
import tempfile
from pathlib import Path

import play as play_tool


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
PLAY = CASE / "play.py"
C_BASELINE = CASE / "benchmarks/renderer_immediate.c"
CAXEMAP = CASE / "scenarios/first-playable/map.caxemap"
CONTENT_PACK = CASE / "packs/caxecraft/base/content.json"
LEVEL_ADAPTER = CASE / "src/caxecraft/content/FirstPlayableLevel.hx"
PLATFORM_NAMES = {"Linux": "linux", "Darwin": "macos"}
SCRIPT_ID = "move-jump-edit"
RAYLIB_CONFIGURATION = "memory-software"
ALLOCATOR_CALL = re.compile(r"\b(?:malloc|calloc|realloc|free)\s*\(")
GOTO_STATEMENT = re.compile(r"\bgoto\s+[A-Za-z_]")
HXRT_REFERENCE = re.compile(r'(?:[<\"/]hxrt(?:[/>\"]|_)|\bhxrt_)')
C_BASELINE_RESULT = re.compile(
    r"^caxecraft-c-baseline: terrainMicroseconds=([0-9]+) measuredFrames=12 "
    r"faces=2744 visible=2430 drawCalls=2 streamHash=([0-9a-f]{8})$",
    re.MULTILINE,
)

# TerrainChunkCache owns four byte arrays with FACE_CAPACITY entries, three
# Int arrays with CHUNK_COUNT entries, and one dirty byte array. These are
# explicit carrier bytes. C ABI padding around the enclosing class is reported
# separately only after a native sizeof probe exists; guessing it here would
# make the cross-compiler report look more exact than its evidence.
FACE_CAPACITY = 49_152
CHUNK_COUNT = 16
CHUNK_CACHE_PAYLOAD = {
    "faceCoordinateAndMaterialBytes": 4 * FACE_CAPACITY,
    "chunkCounterBytes": 3 * CHUNK_COUNT * 4,
    "dirtyFlagBytes": CHUNK_COUNT,
}


class BenchmarkFailure(RuntimeError):
    """A reproducibility or comparison rule failed."""


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--allow-network",
        action="store_true",
        help="allow the first checksum-pinned Raylib download",
    )
    parser.add_argument("--authority", choices=("pinned-source", "offline-source"), default="pinned-source")
    parser.add_argument("--source", type=Path, help="exact Raylib 6.0 tree used with offline-source authority")
    parser.add_argument("--cc", default=os.environ.get("CC", "gcc"))
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    parser.add_argument("--cmake", default="cmake")
    parser.add_argument(
        "--generator",
        choices=("Ninja", "Unix Makefiles"),
        default="Ninja" if shutil.which("ninja") else "Unix Makefiles",
    )
    parser.add_argument("--cache-root", type=Path, default=ROOT / ".cache/caxecraft/raylib")
    parser.add_argument("--raygui-cache-root", type=Path, default=ROOT / ".cache/caxecraft/raygui")
    parser.add_argument("--output-root", type=Path, default=CASE / "_build/play")
    parser.add_argument("--prebuilt-raylib-cache", type=Path, help="verified pinned-source cache from the Raylib CI lane")
    parser.add_argument("--prebuilt-raylib-build", type=Path, help="verified Raylib build from the matching CI lane")
    parser.add_argument("--prebuilt-raylib-report", type=Path, help="normalized report for the verified Raylib build")
    parser.add_argument(
        "--report",
        type=Path,
        default=CASE / "_build/benchmarks/renderer-comparison.json",
        help="comparison report destination; generated build evidence is not checked in",
    )
    return parser.parse_args(argv)


def run_renderer(args: argparse.Namespace, renderer: str) -> Path:
    command = [
        sys.executable,
        str(PLAY),
        "--raylib-configuration",
        RAYLIB_CONFIGURATION,
        "--pilot",
        SCRIPT_ID,
        "--benchmark-renderer",
        "--renderer",
        renderer,
        "--optimization",
        "2",
        "--layout",
        "split",
        "--cc",
        args.cc,
        "--cxx",
        args.cxx,
        "--cmake",
        args.cmake,
        "--generator",
        args.generator,
        "--cache-root",
        str(args.cache_root.resolve()),
        "--raygui-cache-root",
        str(args.raygui_cache_root.resolve()),
        "--output-root",
        str(args.output_root.resolve()),
        "--authority",
        args.authority,
    ]
    if args.allow_network:
        command.append("--allow-network")
    if args.source is not None:
        command.extend(("--source", str(args.source.resolve())))
    prebuilt_values = (args.prebuilt_raylib_cache, args.prebuilt_raylib_build, args.prebuilt_raylib_report)
    if all(value is not None for value in prebuilt_values):
        command.extend(
            (
                "--prebuilt-raylib-cache",
                str(args.prebuilt_raylib_cache.resolve()),
                "--prebuilt-raylib-build",
                str(args.prebuilt_raylib_build.resolve()),
                "--prebuilt-raylib-report",
                str(args.prebuilt_raylib_report.resolve()),
            )
        )
    print(f"caxecraft benchmark: running {renderer}", flush=True)
    result = subprocess.run(command, cwd=ROOT, check=False)
    if result.returncode != 0:
        raise BenchmarkFailure(f"{renderer} benchmark failed with exit {result.returncode}")

    platform_name = PLATFORM_NAMES[platform.system()]
    renderer_part = "" if renderer == "chunk-cache" else "-immediate-baseline"
    variant = (
        args.output_root.resolve()
        / "variants"
        / f"{platform_name}-memory-software-split-pilot-{SCRIPT_ID}{renderer_part}-benchmark"
    )
    report = variant / "bin/caxecraft-pilot-report.json"
    if not report.is_file():
        raise BenchmarkFailure(f"{renderer} did not produce its expected report: {report}")
    return variant


def read_json_object(path: Path) -> dict[str, object]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise BenchmarkFailure(f"cannot read JSON evidence {path}: {error}") from error
    if not isinstance(value, dict):
        raise BenchmarkFailure(f"JSON evidence must contain one object: {path}")
    return value


def storage_codes() -> dict[str, int]:
    """Read only the reviewed content identities needed by benchmark data."""

    document = read_json_object(CONTENT_PACK)
    blocks = document.get("blocks")
    if not isinstance(blocks, list):
        raise BenchmarkFailure("Caxecraft content pack omitted its block array")
    result: dict[str, int] = {}
    for index, block in enumerate(blocks):
        if not isinstance(block, dict):
            raise BenchmarkFailure(f"content block {index} is not an object")
        content_id = block.get("id")
        storage_code = block.get("storageCode")
        if (
            not isinstance(content_id, str)
            or isinstance(storage_code, bool)
            or not isinstance(storage_code, int)
            or not 0 <= storage_code <= 255
            or content_id in result
        ):
            raise BenchmarkFailure(f"content block {index} has an invalid storage identity")
        result[content_id] = storage_code
    return result


def benchmark_scene() -> tuple[bytes, str]:
    """Decode the measured terrain chunk without duplicating gameplay rules."""

    try:
        source = CAXEMAP.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise BenchmarkFailure(f"cannot read benchmark CAXEMAP: {error}") from error
    palette: dict[int, str] = {}
    runs: list[tuple[int, int]] = []
    world_seen = False
    in_chunk = False
    chunk_seen = False
    for line_number, raw_line in enumerate(source.splitlines(), start=1):
        parts = raw_line.strip().split()
        if len(parts) == 4 and parts[0] == "world":
            if world_seen or parts[1:] != ["32", "16", "32"]:
                raise BenchmarkFailure("benchmark CAXEMAP must declare exactly one 32x16x32 world")
            world_seen = True
        elif len(parts) == 3 and parts[0] == "palette":
            try:
                palette_index = int(parts[1])
            except ValueError as error:
                raise BenchmarkFailure(f"invalid palette index on CAXEMAP line {line_number}") from error
            if palette_index in palette:
                raise BenchmarkFailure(f"duplicate palette index on CAXEMAP line {line_number}")
            palette[palette_index] = parts[2]
        elif parts[:2] == ["chunk", "world.base"]:
            expected = ["chunk", "world.base", "0", "0", "0", "32", "16", "32"]
            if in_chunk or chunk_seen or parts != expected:
                raise BenchmarkFailure("benchmark CAXEMAP terrain chunk identity drifted")
            in_chunk = True
            chunk_seen = True
        elif in_chunk and parts == ["end", "chunk"]:
            in_chunk = False
        elif in_chunk and len(parts) == 3 and parts[0] == "run":
            try:
                palette_index = int(parts[1])
                length = int(parts[2])
            except ValueError as error:
                raise BenchmarkFailure(f"invalid terrain run on CAXEMAP line {line_number}") from error
            if length <= 0:
                raise BenchmarkFailure(f"non-positive terrain run on CAXEMAP line {line_number}")
            runs.append((palette_index, length))
    if not world_seen or not chunk_seen or in_chunk:
        raise BenchmarkFailure("benchmark CAXEMAP terrain envelope is incomplete")

    codes = storage_codes()
    cells = bytearray()
    for palette_index, length in runs:
        content_id = palette.get(palette_index)
        if content_id is None or content_id not in codes:
            raise BenchmarkFailure(f"terrain run references unknown palette value {palette_index}")
        cells.extend([codes[content_id]] * length)
        if len(cells) > 32 * 16 * 32:
            raise BenchmarkFailure("benchmark terrain runs exceed the admitted world volume")
    if len(cells) != 32 * 16 * 32:
        raise BenchmarkFailure(f"benchmark terrain has {len(cells)} cells, expected 16384")
    source_hash = hashlib.sha256(source.encode("utf-8")).hexdigest()
    try:
        adapter = LEVEL_ADAPTER.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise BenchmarkFailure(f"cannot read generated level adapter: {error}") from error
    if f'inline final SOURCE_SHA256:String = "{source_hash}";' not in adapter:
        raise BenchmarkFailure(
            "handwritten C benchmark data and the Haxe level adapter do not name the same CAXEMAP bytes"
        )
    return bytes(cells), source_hash


def write_scene_header(path: Path, cells: bytes, source_hash: str) -> None:
    """Render immutable scene data; the handwritten C owns all benchmark logic."""

    lines = [
        "/* Generated benchmark data from the validated first-playable CAXEMAP. */",
        "#ifndef CAXECRAFT_BENCHMARK_SCENE_H_INCLUDED",
        "#define CAXECRAFT_BENCHMARK_SCENE_H_INCLUDED",
        "",
        f"/* CAXEMAP SHA-256: {source_hash} */",
        "static const unsigned char CAXECRAFT_BENCHMARK_SCENE_CELLS[16384] = {",
    ]
    for start in range(0, len(cells), 32):
        values = ", ".join(str(value) for value in cells[start : start + 32])
        lines.append(f"    {values},")
    lines.extend(("};", "", "#endif", ""))
    path.write_text("\n".join(lines), encoding="utf-8", newline="\n")


def raylib_inputs(args: argparse.Namespace) -> tuple[Path, Path]:
    """Reuse the exact Raylib build already selected for generated Haxe."""

    prebuilt = (
        args.prebuilt_raylib_cache,
        args.prebuilt_raylib_build,
        args.prebuilt_raylib_report,
    )
    platform_name = PLATFORM_NAMES[platform.system()]
    if all(value is not None for value in prebuilt):
        return play_tool.resolve_prebuilt_raylib(
            cache_root=args.prebuilt_raylib_cache,
            build_root=args.prebuilt_raylib_build,
            report_path=args.prebuilt_raylib_report,
            platform_name=platform_name,
            raylib_configuration=RAYLIB_CONFIGURATION,
        )
    return play_tool.provision_raylib(
        authority=args.authority,
        platform_name=platform_name,
        raylib_configuration=RAYLIB_CONFIGURATION,
        source=args.source,
        cache_root=args.cache_root.resolve(),
        cc=args.cc,
        cxx=args.cxx,
        cmake=args.cmake,
        generator=args.generator,
        allow_network=args.allow_network,
        rebuild=False,
    )


def run_handwritten_c_reference(args: argparse.Namespace) -> dict[str, object]:
    """Compile and repeat the independent C renderer under matching inputs."""

    cells, scene_hash = benchmark_scene()
    include_directory, library = raylib_inputs(args)
    platform_name = PLATFORM_NAMES[platform.system()]
    with tempfile.TemporaryDirectory(prefix="caxecraft-c-renderer-") as raw_root:
        root = Path(raw_root)
        header = root / "caxecraft_benchmark_scene.h"
        executable = root / "caxecraft-c-renderer"
        write_scene_header(header, cells, scene_hash)
        arguments = [
            args.cc,
            *play_tool.STRICT_FLAGS,
            "-O2",
            "-I",
            str(root),
            "-I",
            str(include_directory),
            str(C_BASELINE),
            str(library),
        ]
        lock = play_tool.provision.load_lock()
        libraries, frameworks = play_tool.provision.link_facts(
            lock, platform_name, RAYLIB_CONFIGURATION
        )
        for name in libraries:
            if name != "raylib":
                arguments.append(f"-l{name}")
        for name in frameworks:
            arguments.extend(("-framework", name))
        arguments.extend(("-o", str(executable)))
        compilation = subprocess.run(
            arguments,
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=180,
        )
        if compilation.returncode != 0:
            raise BenchmarkFailure(
                "handwritten C renderer failed strict compilation\n"
                f"stdout:\n{compilation.stdout}stderr:\n{compilation.stderr}"
            )

        samples: list[int] = []
        stream_hash: str | None = None
        for sample in range(7):
            result = subprocess.run(
                [str(executable)],
                cwd=CASE,
                check=False,
                capture_output=True,
                text=True,
                timeout=30,
            )
            match = C_BASELINE_RESULT.search(result.stdout)
            if result.returncode != 0 or result.stderr or match is None:
                raise BenchmarkFailure(
                    f"handwritten C renderer sample {sample} failed\n"
                    f"exit: {result.returncode}\nstdout:\n{result.stdout}stderr:\n{result.stderr}"
                )
            samples.append(int(match.group(1)))
            if stream_hash is None:
                stream_hash = match.group(2)
            elif stream_hash != match.group(2):
                raise BenchmarkFailure("handwritten C renderer face stream changed between samples")

        source_text = C_BASELINE.read_text(encoding="utf-8")
        median = statistics.median(samples)
        return {
            "method": "handwritten C immediate discovery plus two textured rlgl submissions",
            "terrainMicroseconds": samples,
            "terrainMedianMicroseconds": median,
            "terrainMedianMicrosecondsPerFrame": median / 12,
            "measuredFramesPerSample": 12,
            "sampleCount": 7,
            "faces": 2744,
            "visibleOpaqueBlocks": 2430,
            "drawCalls": 2,
            "faceStreamHash": stream_hash,
            "sceneSourceSha256": scene_hash,
            "artifacts": {
                "handwrittenSourceBytes": C_BASELINE.stat().st_size,
                "handwrittenSourceSha256": hashlib.sha256(C_BASELINE.read_bytes()).hexdigest(),
                "generatedSceneHeaderBytes": header.stat().st_size,
                "generatedSceneHeaderSha256": hashlib.sha256(header.read_bytes()).hexdigest(),
                "nativeBinaryBytes": executable.stat().st_size,
                "allocatorCallsInHandwrittenSource": len(ALLOCATOR_CALL.findall(source_text)),
                "gotoStatementsInHandwrittenSource": len(GOTO_STATEMENT.findall(source_text)),
            },
            "qualification": (
                "The reference matches terrain discovery/submission only. Its binary excludes the game, HUD, "
                "simulation, and telemetry, so binary size is recorded but not compared as an application ratio."
            ),
        }


def semantic_projection(report: dict[str, object]) -> dict[str, object]:
    """Remove only renderer-specific observations before parity comparison."""

    projected = copy.deepcopy(report)
    projected.pop("benchmark", None)
    render = projected.get("render")
    if not isinstance(render, dict):
        raise BenchmarkFailure("pilot report omitted its render object")
    for name in ("implementation", "rebuiltTerrainChunks", "totalRebuiltTerrainChunks", "terrainCacheValid"):
        render.pop(name, None)
    return projected


def require_semantic_parity(cache: dict[str, object], baseline: dict[str, object]) -> None:
    cache_projection = semantic_projection(cache)
    baseline_projection = semantic_projection(baseline)
    if cache_projection == baseline_projection:
        return
    before = json.dumps(baseline_projection, ensure_ascii=False, indent=2, sort_keys=True).splitlines()
    after = json.dumps(cache_projection, ensure_ascii=False, indent=2, sort_keys=True).splitlines()
    difference = "\n".join(
        difflib.unified_diff(before, after, fromfile="immediate-baseline", tofile="chunk-cache", lineterm="")
    )
    raise BenchmarkFailure(f"renderer variants changed gameplay or presented output:\n{difference}")


def require_dict(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise BenchmarkFailure(f"benchmark report omitted {label}")
    return value


def require_number(value: object, label: str) -> int | float:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise BenchmarkFailure(f"benchmark report field {label} is not numeric")
    return value


def source_evidence(variant: Path) -> dict[str, object]:
    generated = variant / "generated"
    sources = sorted(generated.rglob("*.c"))
    headers = sorted(generated.rglob("*.h"))
    if not sources or not headers:
        raise BenchmarkFailure(f"generated project is incomplete: {generated}")

    goto_count = 0
    allocation_count = 0
    hxrt_count = 0
    digest = hashlib.sha256()
    for path in (*sources, *headers):
        relative = path.relative_to(generated).as_posix()
        try:
            data = path.read_bytes()
            text = data.decode("utf-8")
        except (OSError, UnicodeError) as error:
            raise BenchmarkFailure(f"cannot inspect generated source {path}: {error}") from error
        digest.update(relative.encode("utf-8"))
        digest.update(b"\0")
        digest.update(data)
        digest.update(b"\0")
        goto_count += len(GOTO_STATEMENT.findall(text))
        allocation_count += len(ALLOCATOR_CALL.findall(text))
        hxrt_count += len(HXRT_REFERENCE.findall(text))

    if goto_count != 0 or allocation_count != 0 or hxrt_count != 0:
        raise BenchmarkFailure(
            "generated renderer project violated the steady-state structural budget: "
            f"goto={goto_count}, allocation calls={allocation_count}, hxrt references={hxrt_count}"
        )
    executable = variant / "bin/caxecraft"
    if not executable.is_file():
        raise BenchmarkFailure(f"native executable is missing: {executable}")
    return {
        "generatedCFileCount": len(sources),
        "generatedCBytes": sum(path.stat().st_size for path in sources),
        "generatedHeaderFileCount": len(headers),
        "generatedHeaderBytes": sum(path.stat().st_size for path in headers),
        "generatedProjectSha256": digest.hexdigest(),
        "nativeBinaryBytes": executable.stat().st_size,
        "generatedGotoStatements": goto_count,
        "generatedAllocatorCalls": allocation_count,
        "generatedHxrtReferences": hxrt_count,
    }


def renderer_record(report: dict[str, object], variant: Path) -> dict[str, object]:
    return {
        "timing": require_dict(report.get("benchmark"), "benchmark object"),
        "render": require_dict(report.get("render"), "render object"),
        "artifacts": source_evidence(variant),
    }


def relative_change(before: int | float, after: int | float) -> object:
    if before == 0:
        return None
    return (after - before) / before


def build_comparison(
    cache_report: dict[str, object],
    baseline_report: dict[str, object],
    cache_variant: Path,
    baseline_variant: Path,
    handwritten_c: dict[str, object],
) -> dict[str, object]:
    require_semantic_parity(cache_report, baseline_report)
    cache_benchmark = require_dict(cache_report.get("benchmark"), "chunk-cache benchmark")
    baseline_benchmark = require_dict(baseline_report.get("benchmark"), "immediate benchmark")
    cache_hash = cache_benchmark.get("reviewScreenshotSha256")
    baseline_hash = baseline_benchmark.get("reviewScreenshotSha256")
    if not isinstance(cache_hash, str) or cache_hash != baseline_hash:
        raise BenchmarkFailure("renderer variants did not present the same byte-exact review screenshot")

    cache_terrain = require_number(cache_benchmark.get("terrainMedianMicrosecondsPerFrame"), "cache terrain median")
    baseline_terrain = require_number(
        baseline_benchmark.get("terrainMedianMicrosecondsPerFrame"), "baseline terrain median"
    )
    cache_update = require_number(cache_benchmark.get("updateMedianMicrosecondsPerFrame"), "cache update median")
    baseline_update = require_number(
        baseline_benchmark.get("updateMedianMicrosecondsPerFrame"), "baseline update median"
    )
    cache_preparation = require_number(
        cache_benchmark.get("preparationMedianMicrosecondsPerFrame"), "cache preparation median"
    )
    baseline_preparation = require_number(
        baseline_benchmark.get("preparationMedianMicrosecondsPerFrame"), "baseline preparation median"
    )
    c_terrain = require_number(
        handwritten_c.get("terrainMedianMicrosecondsPerFrame"),
        "handwritten C terrain median",
    )
    cache_render = require_dict(cache_report.get("render"), "chunk-cache render object")
    if handwritten_c.get("faces") != cache_render.get("visibleTerrainFaces"):
        raise BenchmarkFailure("handwritten C and generated Haxe discovered different final terrain faces")
    c_artifacts = require_dict(handwritten_c.get("artifacts"), "handwritten C artifacts")
    if (
        c_artifacts.get("allocatorCallsInHandwrittenSource") != 0
        or c_artifacts.get("gotoStatementsInHandwrittenSource") != 0
    ):
        raise BenchmarkFailure("handwritten C reference violated its structural baseline budget")

    script = require_dict(cache_report.get("script"), "script object")
    world = require_dict(cache_report.get("world"), "world object")
    return {
        "schemaVersion": 1,
        "scope": {
            "subject": "finding visible terrain faces and sending them to Raylib for drawing",
            "comparison": (
                "generated Haxe chunk cache and retained generated Haxe immediate renderer "
                "versus an independent handwritten C immediate reference"
            ),
            "handwrittenCBaseline": (
                "same authored cells, remove/place sequence, two atlas submissions, warmup, sample count, "
                "compiler, optimization, Raylib build, and monotonic clock"
            ),
            "performanceClaim": "measurement evidence only; no general optimized or faster-than-C claim",
        },
        "workload": {
            "scene": "first-playable authored CaxeMap",
            "seed": None,
            "seedReason": "the measured level stores exact authored cells rather than procedural seed input",
            "sceneStateHash": world.get("stateHash"),
            "pilotScript": script.get("id"),
            "inputHash": script.get("inputHash"),
            "viewport": {"width": 1280, "height": 720},
            "optimization": "O2",
            "projectLayout": "split",
        },
        "environment": cache_report.get("native"),
        "method": {
            "clock": cache_benchmark.get("clock"),
            "warmupFrames": cache_benchmark.get("warmupFrames"),
            "sampleCount": cache_benchmark.get("sampleCount"),
            "measuredFramesPerSample": cache_benchmark.get("measuredFramesPerSample"),
            "sampleStatistic": "median of seven independent fixed-frame pilot executions",
            "parity": "complete non-renderer-specific pilot report plus byte-exact final screenshot",
        },
        "chunkCacheStorage": {
            "explicitPayloadBytes": sum(CHUNK_CACHE_PAYLOAD.values()),
            "breakdown": CHUNK_CACHE_PAYLOAD,
            "allocationCount": 0,
            "qualification": "excludes compiler-dependent enclosing-struct padding; no sizeof probe is claimed",
        },
        "immediateBaseline": renderer_record(baseline_report, baseline_variant),
        "chunkCache": renderer_record(cache_report, cache_variant),
        "handwrittenCReference": handwritten_c,
        "comparison": {
            "terrainMedianMicrosecondsPerFrameDelta": cache_terrain - baseline_terrain,
            "terrainMedianMicrosecondsPerFrameRelativeChange": relative_change(baseline_terrain, cache_terrain),
            "updateMedianMicrosecondsPerFrameDelta": cache_update - baseline_update,
            "updateMedianMicrosecondsPerFrameRelativeChange": relative_change(baseline_update, cache_update),
            "preparationMedianMicrosecondsPerFrameDelta": cache_preparation - baseline_preparation,
            "preparationMedianMicrosecondsPerFrameRelativeChange": relative_change(
                baseline_preparation, cache_preparation
            ),
            "generatedImmediateVersusHandwrittenCMicrosecondsPerFrameDelta": baseline_terrain - c_terrain,
            "generatedImmediateVersusHandwrittenCRelativeChange": relative_change(c_terrain, baseline_terrain),
            "chunkCacheVersusHandwrittenCMicrosecondsPerFrameDelta": cache_terrain - c_terrain,
            "chunkCacheVersusHandwrittenCRelativeChange": relative_change(c_terrain, cache_terrain),
            "reviewScreenshotSha256": cache_hash,
            "semanticParity": True,
            "terrainFaceParityWithHandwrittenC": True,
        },
    }


def main(argv: list[str]) -> int:
    try:
        args = parse_args(argv)
        if platform.system() not in PLATFORM_NAMES:
            raise BenchmarkFailure(
                "the reproducible renderer comparison currently supports Linux and macOS "
                "with the verified Raylib Memory clock"
            )
        if args.authority == "offline-source" and args.source is None:
            raise BenchmarkFailure("--authority offline-source requires --source")
        if args.authority == "pinned-source" and args.source is not None:
            raise BenchmarkFailure("--source is accepted only with --authority offline-source")
        prebuilt_values = (args.prebuilt_raylib_cache, args.prebuilt_raylib_build, args.prebuilt_raylib_report)
        if any(value is not None for value in prebuilt_values) and not all(
            value is not None for value in prebuilt_values
        ):
            raise BenchmarkFailure("prebuilt Raylib reuse requires cache, build, and report paths together")
        if all(value is not None for value in prebuilt_values) and (args.allow_network or args.source is not None):
            raise BenchmarkFailure("prebuilt Raylib reuse rejects network and source inputs")

        cache_variant = run_renderer(args, "chunk-cache")
        baseline_variant = run_renderer(args, "immediate-baseline")
        print("caxecraft benchmark: running handwritten-c reference", flush=True)
        handwritten_c = run_handwritten_c_reference(args)
        cache_report = read_json_object(cache_variant / "bin/caxecraft-pilot-report.json")
        baseline_report = read_json_object(baseline_variant / "bin/caxecraft-pilot-report.json")
        comparison = build_comparison(
            cache_report,
            baseline_report,
            cache_variant,
            baseline_variant,
            handwritten_c,
        )

        report_path = args.report.resolve()
        report_path.parent.mkdir(parents=True, exist_ok=True)
        report_path.write_text(
            json.dumps(comparison, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
            encoding="utf-8",
            newline="\n",
        )
        print(f"caxecraft benchmark: renderer comparison passed; report {report_path}")
        return 0
    except (OSError, UnicodeError, BenchmarkFailure) as error:
        print(f"caxecraft benchmark: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

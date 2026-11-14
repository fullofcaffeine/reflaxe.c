#!/usr/bin/env python3
"""Type-check every target-owned Haxe module and prove fail-closed production use."""

from __future__ import annotations

import argparse
import contextlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import TextIO


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402


HXML = Path(__file__).with_name("all_sources.hxml")
HXC_HXML = Path(__file__).with_name("hxc_sources.hxml")
SUCCESS_SENTINEL = "all-sources: OK"
HXC_SUCCESS_SENTINEL = "hxc-sources: OK"
LOWERING_DIAGNOSTIC_ID = "HXC1001"
LOWERING_DETAIL = (
    "Unsupported typed Haxe node `TField(static:SIDECAR_PATHS:reference-Array-non-null)`"
)
LOWERING_SOURCE = "CProjectEmitter.hx:305: lines 305-320"
MACRO_BRANCH_MARKERS = (
    "Typing macro reflaxe.c.CompilerBootstrap.Start",
    "Typing macro reflaxe.c.BuildDetection.isCBuild",
    "Typing macro reflaxe.c.macros.TypedCContractMacro.install",
    "Typing macro reflaxe.c.CompilerInit.Start",
    "Typing macro reflaxe.c.ProfileResolver.resolve",
    "Typing macro reflaxe.c.CReflaxeCompiler.onCompileEnd",
    "Typing macro reflaxe.c.frontend.TypedAstNormalizer.normalize",
    "Typing macro reflaxe.c.frontend.TypedAstInventory.snapshot",
    "Typing macro reflaxe.c.frontend.IncrementalInputInventory.snapshot",
    "Typing macro reflaxe.c.CCompiler.compileModules",
    "Typing macro reflaxe.c.TargetPlatform.configure",
)


class AllSourcesFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def retain_compiler_streams(
    log: TextIO | None,
    *,
    label: str,
    command: list[str],
    result: subprocess.CompletedProcess[str] | subprocess.TimeoutExpired,
) -> None:
    """Keep completed requests, including timeouts, without printing successful logs."""
    if log is None:
        return

    def text(value: str | bytes | None) -> str:
        return value.decode("utf-8", errors="replace") if isinstance(value, bytes) else value or ""

    timed_out = isinstance(result, subprocess.TimeoutExpired)
    log.write(json.dumps({
        "label": label,
        "command": command,
        "configuredTimeoutSeconds": 30,
        "expiredTimeoutSeconds": result.timeout if timed_out else None,
        "timedOut": timed_out,
        "returncode": None if timed_out else result.returncode,
        "stdout": text(result.stdout),
        "stderr": text(result.stderr),
    }) + "\n")
    log.flush()


def run_haxe(
    hxml: Path,
    arguments: list[str],
    *,
    expected_code: int,
    label: str,
    compiler_progress: bool = False,
    compiler_log: TextIO | None = None,
) -> subprocess.CompletedProcess[str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    command = [development_tool("haxe"), str(hxml), *arguments]
    try:
        result = run_bounded_process(
            command,
            phase=label,
            cwd=ROOT,
            env=environment,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
            compiler_progress=compiler_progress,
        )
    except subprocess.TimeoutExpired as error:
        try:
            retain_compiler_streams(compiler_log, label=label, command=command, result=error)
        except OSError as log_error:
            print(f"all-sources: could not retain compiler log: {log_error}", file=sys.stderr)
        raise
    try:
        retain_compiler_streams(compiler_log, label=label, command=command, result=result)
    except OSError as log_error:
        if result.returncode == expected_code:
            raise
        print(f"all-sources: could not retain compiler log: {log_error}", file=sys.stderr)
    if result.returncode != expected_code:
        raise AllSourcesFailure(
            f"{label} returned {result.returncode}, expected {expected_code}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return result


def owned_haxe_sources() -> list[Path]:
    return sorted((*ROOT.glob("src/**/*.hx"), *ROOT.glob("std/c/**/*.hx")))


def check_complete_eval_graph(compiler_log: TextIO | None = None) -> None:
    result = run_haxe(
        HXML, ["--interp", "-v"], expected_code=0,
        label="compiler-source Eval graph", compiler_log=compiler_log,
    )
    if result.stdout.strip().splitlines()[-1:] != [SUCCESS_SENTINEL]:
        raise AllSourcesFailure(
            "all-source Eval graph missed its runtime sentinel\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )

    hxc_result = run_haxe(
        HXC_HXML, ["--interp", "-v"], expected_code=0,
        label="hxc-source Eval graph", compiler_log=compiler_log,
    )
    if hxc_result.stdout.strip().splitlines()[-1:] != [HXC_SUCCESS_SENTINEL]:
        raise AllSourcesFailure(
            "hxc-source Eval graph missed its runtime sentinel\n"
            f"stdout:\n{hxc_result.stdout}\nstderr:\n{hxc_result.stderr}"
        )

    verbose_output = (result.stdout + result.stderr + hxc_result.stdout + hxc_result.stderr).replace("\\", "/")
    parsed_sources = [line.removeprefix("Parsed ") for line in verbose_output.splitlines() if line.startswith("Parsed ")]
    missing_sources = [
        source.relative_to(ROOT).as_posix()
        for source in owned_haxe_sources()
        # Haxe reports the checkout's logical path while Path.resolve() may
        # traverse a workspace or home-directory symlink. Match the stable
        # repository-relative suffix so the gate behaves identically locally
        # and in CI.
        if not any(
            parsed == source.relative_to(ROOT).as_posix()
            or parsed.endswith(f"/{source.relative_to(ROOT).as_posix()}")
            for parsed in parsed_sources
        )
    ]
    if missing_sources:
        raise AllSourcesFailure(
            "dedicated HXML did not parse every target-owned Haxe source:\n"
            + "\n".join(f"- {source}" for source in missing_sources)
        )

    missing_macro_branches = [
        marker for marker in MACRO_BRANCH_MARKERS if marker not in verbose_output
    ]
    if missing_macro_branches:
        raise AllSourcesFailure(
            "dedicated HXML did not type every compiler macro branch:\n"
            + "\n".join(f"- {marker}" for marker in missing_macro_branches)
        )


def check_production_boundary(compiler_log: TextIO | None = None) -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-all-sources-") as temporary:
        output = Path(temporary) / "generated"
        result = run_haxe(
            HXML,
            [
                "-D",
                "reflaxe_c_phase_progress",
                "--custom-target",
                f"c={output}",
                "-v",
            ],
            expected_code=1,
            label="all-source production boundary",
            compiler_progress=True,
            compiler_log=compiler_log,
        )
        combined = result.stdout + result.stderr
        if "Typing macro c.Init.init" not in combined:
            raise AllSourcesFailure("production HXML did not type the c.Init custom-target hook")
        if (
            LOWERING_DIAGNOSTIC_ID not in combined
            or LOWERING_DETAIL not in combined
            or LOWERING_SOURCE not in combined
        ):
            raise AllSourcesFailure(
                "production HXML missed its exact source-anchored HXC1001 boundary\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        emitted_files = [path for path in Path(temporary).rglob("*") if path.is_file()]
        if emitted_files:
            raise AllSourcesFailure(
                "HXC1001 left plausible generated artifacts:\n"
                + "\n".join(f"- {path.relative_to(temporary)}" for path in emitted_files)
            )


def main(arguments: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--compiler-log", type=Path,
        help="write verbose child streams as JSON lines to a new file (never overwrite)",
    )
    options = parser.parse_args([] if arguments is None else arguments)
    missing_hxml = [path for path in (HXML, HXC_HXML) if not path.is_file()]
    if missing_hxml:
        print(f"all-sources: ERROR: missing dedicated HXML: {missing_hxml[0]}", file=sys.stderr)
        return 1
    if shutil.which(development_tool("haxe")) is None:
        print("all-sources: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1

    try:
        log_context = (
            options.compiler_log.open("x", encoding="utf-8")
            if options.compiler_log else contextlib.nullcontext()
        )
        with log_context as compiler_log:
            check_complete_eval_graph(compiler_log)
            check_production_boundary(compiler_log)
    except (AllSourcesFailure, subprocess.TimeoutExpired, OSError) as error:
        print(f"all-sources: ERROR: {error}", file=sys.stderr)
        if isinstance(error, subprocess.TimeoutExpired):
            # The process owner retains partial output after cleanup. Keep the
            # verbose typing evidence visible even when no phase marker exists.
            for label, output in (("stdout", error.stdout), ("stderr", error.stderr)):
                if isinstance(output, bytes):
                    output = output.decode("utf-8", errors="replace")
                print(f"partial {label}:\n{output if output is not None else ''}", file=sys.stderr)
        return 1

    print(
        f"all-sources: OK: {len(owned_haxe_sources())} owned modules, macro/non-macro branches, and exact HXC1001 no-output boundary passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

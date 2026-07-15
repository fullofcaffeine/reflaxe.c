#!/usr/bin/env python3
"""Type-check every target-owned Haxe module and prove fail-closed production use."""

from __future__ import annotations

import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("all_sources.hxml")
SUCCESS_SENTINEL = "all-sources: OK"
LOWERING_DIAGNOSTIC_ID = "HXC1000"
LOWERING_DETAIL = "unimplemented whole-program lowering boundary"
MACRO_BRANCH_MARKERS = (
    "Typing macro reflaxe.c.CompilerBootstrap.Start",
    "Typing macro reflaxe.c.BuildDetection.isCBuild",
    "Typing macro reflaxe.c.macros.TypedCContractMacro.install",
    "Typing macro reflaxe.c.CompilerInit.Start",
    "Typing macro reflaxe.c.ProfileResolver.resolve",
    "Typing macro reflaxe.c.CReflaxeCompiler.onCompileEnd",
    "Typing macro reflaxe.c.frontend.TypedAstNormalizer.normalize",
    "Typing macro reflaxe.c.frontend.TypedAstInventory.snapshot",
    "Typing macro reflaxe.c.CCompiler.compileModules",
    "Typing macro reflaxe.c.TargetPlatform.configure",
)


class AllSourcesFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def run_haxe(
    arguments: list[str], *, expected_code: int, label: str
) -> subprocess.CompletedProcess[str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [development_tool("haxe"), str(HXML), *arguments],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != expected_code:
        raise AllSourcesFailure(
            f"{label} returned {result.returncode}, expected {expected_code}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return result


def owned_haxe_sources() -> list[Path]:
    return sorted((*ROOT.glob("src/**/*.hx"), *ROOT.glob("std/c/**/*.hx")))


def check_complete_eval_graph() -> None:
    result = run_haxe(["--interp", "-v"], expected_code=0, label="all-source Eval graph")
    if result.stdout.strip().splitlines()[-1:] != [SUCCESS_SENTINEL]:
        raise AllSourcesFailure(
            "all-source Eval graph missed its runtime sentinel\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )

    verbose_output = (result.stdout + result.stderr).replace("\\", "/")
    missing_sources = [
        source.relative_to(ROOT).as_posix()
        for source in owned_haxe_sources()
        # Haxe reports the checkout's logical path while Path.resolve() may
        # traverse a workspace or home-directory symlink. Match the stable
        # repository-relative suffix so the gate behaves identically locally
        # and in CI.
        if f"/{source.relative_to(ROOT).as_posix()}" not in verbose_output
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


def check_production_boundary() -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-all-sources-") as temporary:
        output = Path(temporary) / "generated"
        result = run_haxe(
            ["--custom-target", f"c={output}", "-v"],
            expected_code=1,
            label="all-source production boundary",
        )
        combined = result.stdout + result.stderr
        if "Typing macro c.Init.init" not in combined:
            raise AllSourcesFailure("production HXML did not type the c.Init custom-target hook")
        if (
            LOWERING_DIAGNOSTIC_ID not in combined
            or LOWERING_DETAIL not in combined
            or "AllSourcesProbe.hx" not in combined
        ):
            raise AllSourcesFailure(
                "production HXML missed its source-anchored HXC1000 boundary\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        emitted_files = [path for path in Path(temporary).rglob("*") if path.is_file()]
        if emitted_files:
            raise AllSourcesFailure(
                "HXC1000 left plausible generated artifacts:\n"
                + "\n".join(f"- {path.relative_to(temporary)}" for path in emitted_files)
            )


def main() -> int:
    if not HXML.is_file():
        print(f"all-sources: ERROR: missing dedicated HXML: {HXML}", file=sys.stderr)
        return 1
    if shutil.which(development_tool("haxe")) is None:
        print("all-sources: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1

    try:
        check_complete_eval_graph()
        check_production_boundary()
    except (AllSourcesFailure, subprocess.TimeoutExpired) as error:
        print(f"all-sources: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        f"all-sources: OK: {len(owned_haxe_sources())} owned modules, macro/non-macro branches, and HXC1000 no-output boundary passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

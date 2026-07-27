#!/usr/bin/env python3
"""Run the independent digest vectors with the repository's pinned Haxe."""

from __future__ import annotations

import os
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SUCCESS_SENTINEL = "content-digest: OK"


def development_tool(name: str) -> str:
    """Prefer the repository-installed tool so CI and local runs use one pin."""

    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def main() -> int:
    haxe = development_tool("haxe")
    if shutil.which(haxe) is None:
        print(
            "content-digest: ERROR: pinned Haxe executable is unavailable",
            file=sys.stderr,
        )
        return 1

    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [
            haxe,
            "--class-path",
            "src",
            "--class-path",
            "test/content_digest",
            "--main",
            "ContentDigestProbe",
            "--interp",
        ],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        print(
            "content-digest: ERROR: Haxe digest probe failed\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}",
            file=sys.stderr,
        )
        return 1
    if result.stdout.strip().splitlines() != [SUCCESS_SENTINEL]:
        print(
            "content-digest: ERROR: unexpected probe output\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}",
            file=sys.stderr,
        )
        return 1
    print(SUCCESS_SENTINEL)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

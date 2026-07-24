#!/usr/bin/env python3
"""Choose one pre-commit test route from normalized staged repository paths.

The hook normally maps each changed file to a small set of focused owners.
Compiler-wide semantic layers are different: one edit there already selects
most focused suites, and running those owners one after another can take tens
of minutes. This policy sends those high-fanout edits through the repository's
same exact toolchain partition, where four isolation-reviewed shards may run
concurrently and successful shards may be resumed safely.

The selector never decides how many workers to use and never caches a test.
Those safety decisions remain in ``run_toolchain_shard.py``.
"""

from __future__ import annotations

import re
import sys
from collections.abc import Iterable


FOCUSED = "focused"
PARALLEL_EXHAUSTIVE = "parallel-exhaustive"

PARALLEL_EXHAUSTIVE_PATH = re.compile(
    r"^(?:"
    r"\.github/workflows/"
    r"|package(?:-lock)?\.json"
    r"|docs/specs/(?:caxecraft-timing|fixture-taxonomy(?:\.schema)?|toolchain-timing)\.json"
    r"|scripts/ci/(?:check_ci_policy|check_toolchain|run_toolchain_shard|select_pre_commit_route)\.py"
    r"|scripts/hooks/pre-commit"
    r"|scripts/test/snapshots\.py"
    r"|test/governance/test_(?:caxecraft_timing|fixture_policy|toolchain_shard)\.py"
    r"|src/reflaxe/c/(?:CCompiler|CReflaxeCompiler|CompilationContext)\.hx"
    r"|src/reflaxe/c/(?:ast|emit|frontend|ir|lowering|naming|plan|runtime|semantics)/"
    r")"
)


def select_route(paths: Iterable[str]) -> str:
    """Return the one route required by the complete staged path set."""
    for raw_path in paths:
        path = raw_path.strip()
        if path and PARALLEL_EXHAUSTIVE_PATH.match(path):
            return PARALLEL_EXHAUSTIVE
    return FOCUSED


def main() -> int:
    print(select_route(sys.stdin))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

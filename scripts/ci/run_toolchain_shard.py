#!/usr/bin/env python3
"""Validate or run one isolated shard of the canonical toolchain gate."""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[2]
PACKAGE = ROOT / "package.json"

# Keep these as contiguous slices of package.json's canonical test:toolchain
# sequence. validate_partition() rejects missing, duplicated, unknown, or
# reordered entries before any suite executes.
SHARD_ORDER = (
    "contracts",
    "lowering-objects",
    "lowering-semantics",
    "caxecraft",
    "snapshots",
)

SHARDS: dict[str, tuple[str, ...]] = {
    "contracts": (
        "deps:verify",
        "test:beads-plan",
        "test:diagnostics",
        "test:hxc-config",
        "test:all-sources",
        "test:bootstrap",
        "test:typed-c",
        "test:c-import",
        "test:raylib-provisioning",
        "test:typed-ast",
        "test:c-ast",
        "test:declaration-plan",
        "test:symbol-registry",
        "test:project-emitter",
        "test:runtime-features",
        "test:array-runtime",
        "test:string-runtime",
        "test:string-output",
        "test:hello",
        "test:hxc-ir",
        "test:primitive-semantics",
        "test:stdlib-ledger",
    ),
    "lowering-objects": (
        "test:body-lowering",
        "test:function-lowering",
        "test:aggregate-lowering",
        "test:class-layout",
        "test:constructor-lowering",
        "test:virtual-dispatch",
        "test:enum-lowering",
        "test:generic-specialization",
    ),
    "lowering-semantics": (
        "test:evaluation-order",
        "test:static-initialization",
        "test:arithmetic-semantics",
        "test:primitive-differential",
        "test:span-lowering",
        "test:project-layout",
    ),
    "caxecraft": ("test:caxecraft-domain:full",),
    "snapshots": ("snapshots:check",),
}


class ToolchainShardFailure(Exception):
    """Raised when the shard contract or one of its commands fails."""


def load_scripts() -> dict[str, str]:
    try:
        package = json.loads(PACKAGE.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise ToolchainShardFailure(f"cannot read package.json: {error}") from error
    if not isinstance(package, dict) or not isinstance(package.get("scripts"), dict):
        raise ToolchainShardFailure("package.json scripts must be an object")
    scripts = package["scripts"]
    if not all(isinstance(name, str) and isinstance(value, str) for name, value in scripts.items()):
        raise ToolchainShardFailure("package.json scripts must map strings to strings")
    return scripts


def canonical_sequence(scripts: dict[str, str]) -> tuple[str, ...]:
    command = scripts.get("test:toolchain")
    if not isinstance(command, str) or not command:
        raise ToolchainShardFailure("package.json has no canonical test:toolchain command")

    sequence: list[str] = []
    for position, part in enumerate(command.split(" && "), start=1):
        prefix = "npm run "
        if not part.startswith(prefix):
            raise ToolchainShardFailure(
                f"canonical command {position} is not an npm script invocation: {part!r}"
            )
        script = part[len(prefix) :]
        if not script or any(character.isspace() for character in script):
            raise ToolchainShardFailure(
                f"canonical command {position} has unsupported arguments: {part!r}"
            )
        sequence.append(script)
    return tuple(sequence)


def validate_partition(scripts: dict[str, str]) -> tuple[str, ...]:
    if tuple(SHARDS) != SHARD_ORDER:
        raise ToolchainShardFailure("SHARDS keys must exactly follow SHARD_ORDER")

    flattened = tuple(script for shard in SHARD_ORDER for script in SHARDS[shard])
    canonical = canonical_sequence(scripts)
    if flattened != canonical:
        missing = [script for script in canonical if script not in flattened]
        extra = [script for script in flattened if script not in canonical]
        duplicate = sorted(
            script for script in set(flattened) if flattened.count(script) > 1
        )
        details = []
        if missing:
            details.append("missing=" + ",".join(missing))
        if extra:
            details.append("extra=" + ",".join(extra))
        if duplicate:
            details.append("duplicate=" + ",".join(duplicate))
        if not details:
            mismatch = next(
                index
                for index, (actual, expected) in enumerate(
                    zip(flattened, canonical), start=1
                )
                if actual != expected
            )
            details.append(
                f"order[{mismatch}]={flattened[mismatch - 1]!r},"
                f"expected={canonical[mismatch - 1]!r}"
            )
        raise ToolchainShardFailure(
            "toolchain shards do not exactly partition test:toolchain: "
            + "; ".join(details)
        )

    unknown = sorted(script for script in flattened if script not in scripts)
    if unknown:
        raise ToolchainShardFailure(
            "toolchain shards reference unknown package scripts: " + ", ".join(unknown)
        )
    return canonical


def run_shard(shard: str, scripts: dict[str, str]) -> None:
    if shard not in SHARDS:
        raise ToolchainShardFailure(
            f"unknown shard {shard!r}; choose one of: {', '.join(SHARD_ORDER)}"
        )
    commands = SHARDS[shard]
    for index, script in enumerate(commands, start=1):
        print(
            f"toolchain-shard: {shard}: [{index}/{len(commands)}] npm run {script}",
            flush=True,
        )
        result = subprocess.run(
            ["npm", "run", script],
            cwd=ROOT,
            check=False,
        )
        if result.returncode != 0:
            raise ToolchainShardFailure(
                f"shard {shard} stopped at {script} with exit {result.returncode}"
            )
    print(f"toolchain-shard: OK: {shard}: {len(commands)} command(s)")


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--check", action="store_true", help="validate exact partition coverage")
    mode.add_argument("--list", action="store_true", help="list shards and commands")
    mode.add_argument("--run", metavar="SHARD", help="run one validated shard")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        scripts = load_scripts()
        canonical = validate_partition(scripts)
        if args.list:
            for shard in SHARD_ORDER:
                print(f"{shard}: {' '.join(SHARDS[shard])}")
            return 0
        if args.check:
            print(
                "toolchain-shards: OK: "
                f"{len(canonical)} command(s) in {len(SHARD_ORDER)} exact shard(s)"
            )
            return 0
        run_shard(args.run, scripts)
        return 0
    except (ToolchainShardFailure, OSError) as error:
        print(f"toolchain-shards: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

#!/usr/bin/env python3
"""Validate or run one isolated shard of the canonical toolchain gate."""

from __future__ import annotations

import argparse
import concurrent.futures
import json
import os
import subprocess
import sys
import tempfile
import time
from pathlib import Path
from typing import Iterable, TextIO


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
    "caxecraft": (
        "test:caxecraft-domain:full",
        "snapshots:catalog",
    ),
}

# Each admitted local-parallel shard writes normal test output only below
# suite-owned TemporaryDirectory roots, binds compiler servers to ephemeral
# loopback ports, and treats repository inputs as read-only. Tests that rely on
# warm server reuse or stale-output transitions remain serial inside a shard.
LOCAL_PARALLEL_ISOLATION: dict[str, str] = {
    "contracts": "temporary-roots+ephemeral-ports",
    "lowering-objects": "temporary-roots+ephemeral-ports",
    "lowering-semantics": "temporary-roots+ephemeral-ports",
    "caxecraft": "temporary-roots+ephemeral-ports",
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
    if tuple(LOCAL_PARALLEL_ISOLATION) != SHARD_ORDER:
        raise ToolchainShardFailure(
            "local parallel isolation declarations must exactly cover SHARD_ORDER"
        )
    return canonical


def elapsed_milliseconds(start_ns: int) -> int:
    return max(0, (time.monotonic_ns() - start_ns + 500_000) // 1_000_000)


def write_timing_report(path: Path, payload: dict[str, object]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    descriptor, temporary_name = tempfile.mkstemp(
        dir=path.parent,
        prefix=f".{path.name}.",
        suffix=".tmp",
        text=True,
    )
    temporary = Path(temporary_name)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="") as handle:
            json.dump(payload, handle, ensure_ascii=False, indent=2)
            handle.write("\n")
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(temporary, path)
    finally:
        if temporary.exists():
            temporary.unlink()


def run_shard(
    shard: str,
    scripts: dict[str, str],
    *,
    timing_report: Path | None = None,
    stream: TextIO | None = None,
) -> None:
    if shard not in SHARDS:
        raise ToolchainShardFailure(
            f"unknown shard {shard!r}; choose one of: {', '.join(SHARD_ORDER)}"
        )
    commands = SHARDS[shard]
    shard_start = time.monotonic_ns()
    records: list[dict[str, object]] = []
    failure: ToolchainShardFailure | None = None
    for index, script in enumerate(commands, start=1):
        print(
            f"toolchain-shard: {shard}: [{index}/{len(commands)}] npm run {script}",
            file=stream,
            flush=True,
        )
        command_start = time.monotonic_ns()
        try:
            result = subprocess.run(
                ["npm", "run", script],
                cwd=ROOT,
                check=False,
                stdout=stream,
                stderr=subprocess.STDOUT if stream is not None else None,
            )
            exit_code: int | None = result.returncode
        except OSError as error:
            exit_code = None
            failure = ToolchainShardFailure(
                f"shard {shard} could not start {script}: {error}"
            )
        duration_ms = elapsed_milliseconds(command_start)
        outcome = "passed" if exit_code == 0 else "failed"
        records.append(
            {
                "script": script,
                "outcome": outcome,
                "exitCode": exit_code,
                "durationMs": duration_ms,
            }
        )
        print(
            f"toolchain-shard: {shard}: {script}: {outcome} in {duration_ms}ms",
            file=stream,
            flush=True,
        )
        if failure is not None:
            break
        if exit_code != 0:
            failure = ToolchainShardFailure(
                f"shard {shard} stopped at {script} with exit {result.returncode}"
            )
            break

    payload: dict[str, object] = {
        "schemaVersion": 1,
        "shard": shard,
        "outcome": "failed" if failure is not None else "passed",
        "durationMs": elapsed_milliseconds(shard_start),
        "commands": records,
    }
    if timing_report is not None:
        write_timing_report(timing_report, payload)
    if failure is not None:
        raise failure
    print(
        f"toolchain-shard: OK: {shard}: {len(commands)} command(s) in "
        f"{payload['durationMs']}ms",
        file=stream,
        flush=True,
    )


def requested_jobs(value: int | None) -> int:
    if value is None:
        raw = os.environ.get("HXC_TEST_JOBS", "2")
        try:
            value = int(raw)
        except ValueError as error:
            raise ToolchainShardFailure(
                f"HXC_TEST_JOBS must be an integer, got {raw!r}"
            ) from error
    if value < 1 or value > len(SHARD_ORDER):
        raise ToolchainShardFailure(
            f"parallel jobs must be between 1 and {len(SHARD_ORDER)}, got {value}"
        )
    return value


def run_all_shards(
    scripts: dict[str, str], *, jobs: int, timing_dir: Path | None = None
) -> None:
    parallel_start = time.monotonic_ns()
    print(
        "toolchain-parallel: starting "
        f"{len(SHARD_ORDER)} isolated shards with {jobs} worker(s)",
        flush=True,
    )
    failures: dict[str, str] = {}
    with tempfile.TemporaryDirectory(prefix="hxc-toolchain-parallel-") as temporary:
        temporary_root = Path(temporary)
        log_paths = {
            shard: temporary_root / f"{index:02d}-{shard}.log"
            for index, shard in enumerate(SHARD_ORDER)
        }
        if timing_dir is not None:
            report_paths = {
                shard: timing_dir / f"toolchain-{shard}-timing.json"
                for shard in SHARD_ORDER
            }
        else:
            report_paths = {
                shard: temporary_root / f"toolchain-{shard}-timing.json"
                for shard in SHARD_ORDER
            }

        def worker(shard: str) -> str | None:
            try:
                with log_paths[shard].open("w", encoding="utf-8", newline="") as log:
                    run_shard(
                        shard,
                        scripts,
                        timing_report=report_paths[shard],
                        stream=log,
                    )
                return None
            except (ToolchainShardFailure, OSError) as error:
                return str(error)

        with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as executor:
            futures = {
                executor.submit(worker, shard): shard for shard in SHARD_ORDER
            }
            pending = set(futures)
            while pending:
                done, pending = concurrent.futures.wait(
                    pending,
                    timeout=30,
                    return_when=concurrent.futures.FIRST_COMPLETED,
                )
                if not done:
                    waiting = ", ".join(
                        shard
                        for shard in SHARD_ORDER
                        if any(futures[future] == shard for future in pending)
                    )
                    print(f"toolchain-parallel: waiting: {waiting}", flush=True)
                    continue
                for future in sorted(done, key=lambda item: SHARD_ORDER.index(futures[item])):
                    shard = futures[future]
                    error = future.result()
                    if error is not None:
                        failures[shard] = error
                        outcome = "failed"
                    else:
                        outcome = "passed"
                    print(f"toolchain-parallel: {shard}: {outcome}", flush=True)

        reports: list[dict[str, object]] = []
        for shard in SHARD_ORDER:
            print(f"\n===== toolchain shard: {shard} =====", flush=True)
            try:
                print(log_paths[shard].read_text(encoding="utf-8"), end="", flush=True)
            except OSError as error:
                failures[shard] = f"cannot read isolated log: {error}"
            try:
                report = json.loads(report_paths[shard].read_text(encoding="utf-8"))
            except (OSError, UnicodeError, json.JSONDecodeError) as error:
                failures[shard] = f"cannot read timing report: {error}"
            else:
                if isinstance(report, dict):
                    reports.append(report)
                else:
                    failures[shard] = "timing report is not a JSON object"

        if timing_dir is not None:
            write_timing_report(
                timing_dir / "toolchain-parallel-summary.json",
                {
                    "schemaVersion": 1,
                    "outcome": "failed" if failures else "passed",
                    "durationMs": elapsed_milliseconds(parallel_start),
                    "jobs": jobs,
                    "shards": reports,
                },
            )

    if failures:
        details = "; ".join(
            f"{shard}: {failures[shard]}"
            for shard in SHARD_ORDER
            if shard in failures
        )
        raise ToolchainShardFailure("parallel toolchain failure: " + details)
    print(
        f"toolchain-parallel: OK: {len(SHARD_ORDER)} shard(s), {jobs} worker(s)",
        flush=True,
    )


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--check", action="store_true", help="validate exact partition coverage")
    mode.add_argument("--list", action="store_true", help="list shards and commands")
    mode.add_argument("--run", metavar="SHARD", help="run one validated shard")
    mode.add_argument(
        "--run-all",
        action="store_true",
        help="run every isolation-approved shard with bounded local concurrency",
    )
    parser.add_argument(
        "--timing-report",
        type=Path,
        help="write an ordered machine-readable timing report for --run",
    )
    parser.add_argument("--jobs", type=int, help="worker count for --run-all (default: 2)")
    parser.add_argument(
        "--timing-dir",
        type=Path,
        help="preserve per-shard and aggregate timing JSON for --run-all",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        scripts = load_scripts()
        canonical = validate_partition(scripts)
        if args.timing_report is not None and args.run is None:
            raise ToolchainShardFailure("--timing-report requires --run SHARD")
        if (args.jobs is not None or args.timing_dir is not None) and not args.run_all:
            raise ToolchainShardFailure("--jobs and --timing-dir require --run-all")
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
        if args.run_all:
            run_all_shards(
                scripts,
                jobs=requested_jobs(args.jobs),
                timing_dir=args.timing_dir,
            )
            return 0
        run_shard(args.run, scripts, timing_report=args.timing_report)
        return 0
    except (ToolchainShardFailure, OSError) as error:
        print(f"toolchain-shards: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

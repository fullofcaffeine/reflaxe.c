#!/usr/bin/env python3
"""Measure cold and warm Reflaxe.C work with the Caxecraft program."""

from __future__ import annotations

import argparse
import ctypes
import hashlib
import os
import re
import statistics
import subprocess
import sys
import tempfile
import time
from collections.abc import Iterable, Mapping, Sequence
from dataclasses import dataclass
from pathlib import Path


CASE = Path(__file__).resolve().parent
sys.path.insert(0, str(CASE))
from run import (  # noqa: E402
    CaxecraftFailure,
    HaxeInstallation,
    HaxeServerConnection,
    assert_artifacts_equal,
    haxe_compilation_server,
    normal_artifacts,
    pinned_haxe_environment,
    pinned_haxe_installation,
    resolve_haxe_arguments,
    target_arguments,
    verify_pinned_haxe,
    write_timing_report,
)


PHASE_PREFIX = "HXC_PHASE_TIMING\t"
DETAIL_PREFIX = "HXC_DETAIL_TIMING\t"
PINNED_HAXE_SOURCE_REVISION = "2c1e544e0a2c7524ef4c8e103f1b0580362ea538"
PHASES = (
    "typed input capture",
    "target pipeline",
    "configuration and contracts",
    "whole program analysis",
    "semantic lowering",
    "HxcIR construction",
    "HxcIR validation",
    "semantic analyses and naming",
    "CAST body construction",
    "optional reports",
    "runtime planning",
    "CAST project planning",
    "C printing",
    "artifact planning",
    "output ownership",
)
DETAIL_PHASES = (
    "body setup and value planning",
    "body control-flow planning",
    "body CAST emission",
)
SEMANTIC_CHILDREN = (
    "HxcIR construction",
    "HxcIR validation",
    "semantic analyses and naming",
    "CAST body construction",
)
TARGET_CHILDREN = (
    "configuration and contracts",
    "whole program analysis",
    "semantic lowering",
    "optional reports",
    "runtime planning",
    "CAST project planning",
    "C printing",
    "artifact planning",
)
ACCOUNTING_PHASES = (
    "host/frontend/setup remainder",
    "typed input capture",
    "configuration and contracts",
    "whole program analysis",
    *SEMANTIC_CHILDREN,
    "semantic lowering remainder",
    "optional reports",
    "runtime planning",
    "CAST project planning",
    "C printing",
    "artifact planning",
    "target pipeline remainder",
    "output ownership",
)
TIMER_ROW = re.compile(
    r"^(?P<label>.*?)\s+\|\s*(?P<seconds>[0-9]+(?:\.[0-9]+)?)\s*"
    r"\|\s*[0-9]+\s*\|\s*[0-9]+\s*\|\s*(?P<calls>[0-9]+)\s*\|.*$"
)


class CompilerProfileFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class TimerClock:
    scale: float
    kind: str
    numerator: int | None
    denominator: int | None

    def to_json(self) -> dict[str, object]:
        return {
            "kind": self.kind,
            "rawToWallScale": round(self.scale, 9),
            "numerator": self.numerator,
            "denominator": self.denominator,
        }


@dataclass(frozen=True)
class HaxeTimerRow:
    path: str
    depth: int
    raw_duration_ms: float
    adjusted_duration_ms: float
    calls: int

    def to_json(self) -> dict[str, object]:
        return {
            "path": self.path,
            "depth": self.depth,
            "rawDurationMs": round(self.raw_duration_ms, 3),
            "clockAdjustedDurationMs": round(self.adjusted_duration_ms, 3),
            "calls": self.calls,
        }


def elapsed_microseconds(started_ns: int) -> int:
    return max(0, (time.monotonic_ns() - started_ns + 500) // 1_000)


def timer_clock(installation: HaxeInstallation) -> TimerClock:
    """Describe the exact Haxe `--times` clock used by this pinned binary.

    Haxe 5.0.0-preview.1 revision 2c1e544 computes `elapsedNano` on macOS but
    returns the unscaled Mach counter. The custom phase records use `Sys.time`
    and need no correction; this scale applies only to Haxe's table.
    """

    if sys.platform != "darwin" or installation.version != "5.0.0-preview.1":
        return TimerClock(1.0, "native-seconds", None, None)

    class MachTimebaseInfo(ctypes.Structure):
        _fields_ = [
            ("numerator", ctypes.c_uint32),
            ("denominator", ctypes.c_uint32),
        ]

    info = MachTimebaseInfo()
    result = ctypes.CDLL(None).mach_timebase_info(ctypes.byref(info))
    if result != 0 or info.numerator == 0 or info.denominator == 0:
        raise CompilerProfileFailure(
            f"cannot read the macOS Mach timer scale (status {result})"
        )
    return TimerClock(
        info.numerator / info.denominator,
        "pinned-haxe-darwin-mach-counter-workaround",
        int(info.numerator),
        int(info.denominator),
    )


def parse_phase_records(stdout: str) -> dict[str, int]:
    records: dict[str, int] = {}
    for line in stdout.splitlines():
        if not line.startswith(PHASE_PREFIX):
            continue
        fields = line.split("\t")
        if len(fields) != 3 or fields[0] != PHASE_PREFIX.rstrip("\t"):
            raise CompilerProfileFailure(f"malformed compiler phase record: {line!r}")
        phase = fields[1]
        if phase not in PHASES or phase in records:
            raise CompilerProfileFailure(
                f"unknown or repeated compiler phase record: {phase!r}"
            )
        try:
            duration = int(fields[2])
        except ValueError as error:
            raise CompilerProfileFailure(
                f"compiler phase {phase!r} has a non-integer duration"
            ) from error
        if duration < 0:
            raise CompilerProfileFailure(
                f"compiler phase {phase!r} has a negative duration"
            )
        records[phase] = duration
    missing = [phase for phase in PHASES if phase not in records]
    if missing:
        raise CompilerProfileFailure(
            f"compiler phase report omitted {', '.join(missing)}"
        )
    return records


def parse_detail_records(stdout: str) -> dict[str, int]:
    records = {detail: 0 for detail in DETAIL_PHASES}
    counts = {detail: 0 for detail in DETAIL_PHASES}
    for line in stdout.splitlines():
        if not line.startswith(DETAIL_PREFIX):
            continue
        fields = line.split("\t")
        if len(fields) != 3 or fields[0] != DETAIL_PREFIX.rstrip("\t"):
            raise CompilerProfileFailure(
                f"malformed compiler detail record: {line!r}"
            )
        detail = fields[1]
        if detail not in records:
            raise CompilerProfileFailure(
                f"unknown compiler detail record: {detail!r}"
            )
        try:
            duration = int(fields[2])
        except ValueError as error:
            raise CompilerProfileFailure(
                f"compiler detail {detail!r} has a non-integer duration"
            ) from error
        if duration < 0:
            raise CompilerProfileFailure(
                f"compiler detail {detail!r} has a negative duration"
            )
        records[detail] += duration
        counts[detail] += 1
    missing = [detail for detail in DETAIL_PHASES if counts[detail] == 0]
    if missing:
        raise CompilerProfileFailure(
            f"compiler detail report omitted {', '.join(missing)}"
        )
    return records


def parse_haxe_timer_rows(stdout: str, clock: TimerClock) -> list[HaxeTimerRow]:
    if not any(line.startswith("name") and "time(s)" in line for line in stdout.splitlines()):
        raise CompilerProfileFailure("Haxe --times table was not emitted")
    parents: list[str] = []
    rows: list[HaxeTimerRow] = []
    for line in stdout.splitlines():
        match = TIMER_ROW.fullmatch(line)
        if match is None:
            continue
        label = match.group("label").rstrip()
        leading = len(label) - len(label.lstrip(" "))
        if leading % 2 != 0:
            raise CompilerProfileFailure(f"Haxe timer indentation drifted: {line!r}")
        depth = leading // 2
        name = label.strip()
        if not name:
            raise CompilerProfileFailure(f"Haxe timer name is empty: {line!r}")
        if name == "total":
            path = "total"
            depth = 0
        else:
            if depth > len(parents):
                raise CompilerProfileFailure(f"Haxe timer hierarchy jumped: {line!r}")
            parents = parents[:depth]
            path = "/".join((*parents, name))
            parents.append(name)
        raw_ms = float(match.group("seconds")) * 1000.0
        rows.append(
            HaxeTimerRow(
                path,
                depth,
                raw_ms,
                raw_ms * clock.scale,
                int(match.group("calls")),
            )
        )
    if not rows or rows[-1].path != "total":
        raise CompilerProfileFailure("Haxe --times table omitted its total row")
    return rows


def validate_haxe_timer_stream(stderr: str) -> None:
    """Reject diagnostics while allowing only the requested `--times` table."""

    for line in stderr.splitlines():
        if (
            not line
            or line.startswith("name")
            and "time(s)" in line
            or set(line) == {"-"}
            or TIMER_ROW.fullmatch(line) is not None
        ):
            continue
        raise CompilerProfileFailure(
            f"Haxe emitted non-timing stderr during profiling: {line!r}"
        )


def accounting_durations(wall_us: int, phases: Mapping[str, int]) -> dict[str, int]:
    semantic_remainder = max(
        0,
        phases["semantic lowering"]
        - sum(phases[phase] for phase in SEMANTIC_CHILDREN),
    )
    target_remainder = max(
        0,
        phases["target pipeline"]
        - sum(phases[phase] for phase in TARGET_CHILDREN),
    )
    host_remainder = max(
        0,
        wall_us
        - phases["typed input capture"]
        - phases["target pipeline"]
        - phases["output ownership"],
    )
    result = {
        "host/frontend/setup remainder": host_remainder,
        "typed input capture": phases["typed input capture"],
        "configuration and contracts": phases["configuration and contracts"],
        "whole program analysis": phases["whole program analysis"],
        **{phase: phases[phase] for phase in SEMANTIC_CHILDREN},
        "semantic lowering remainder": semantic_remainder,
        "optional reports": phases["optional reports"],
        "runtime planning": phases["runtime planning"],
        "CAST project planning": phases["CAST project planning"],
        "C printing": phases["C printing"],
        "artifact planning": phases["artifact planning"],
        "target pipeline remainder": target_remainder,
        "output ownership": phases["output ownership"],
    }
    if tuple(result) != ACCOUNTING_PHASES:
        raise CompilerProfileFailure("compiler accounting phase order drifted")
    return result


def artifact_digest(artifacts: Mapping[str, bytes]) -> str:
    digest = hashlib.sha256()
    for path, content in sorted(
        artifacts.items(), key=lambda item: item[0].encode("utf-8")
    ):
        encoded = path.encode("utf-8")
        digest.update(len(encoded).to_bytes(8, "big"))
        digest.update(encoded)
        digest.update(len(content).to_bytes(8, "big"))
        digest.update(content)
    return digest.hexdigest()


def compiler_command(
    installation: HaxeInstallation,
    resolved: Sequence[str],
    connection: HaxeServerConnection | None,
) -> list[str]:
    command = [str(installation.compiler)]
    if connection is not None:
        command.extend(["--connect", connection.endpoint])
    command.extend(resolved)
    return command


def run_sample(
    installation: HaxeInstallation,
    resolved: Sequence[str],
    clock: TimerClock,
    *,
    connection: HaxeServerConnection | None,
) -> dict[str, object]:
    environment = pinned_haxe_environment("C", installation)
    if connection is None:
        environment["HAXE_NO_SERVER"] = "1"
    started = time.monotonic_ns()
    result = subprocess.run(
        compiler_command(installation, resolved, connection),
        cwd=CASE,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=180,
    )
    wall_us = elapsed_microseconds(started)
    if result.returncode != 0:
        raise CompilerProfileFailure(
            "Caxecraft compiler sample failed\n"
            f"exit={result.returncode}\nstdout:\n{result.stdout}\n"
            f"stderr:\n{result.stderr}"
        )
    validate_haxe_timer_stream(result.stderr)
    phases = parse_phase_records(result.stdout)
    details = parse_detail_records(result.stdout)
    haxe_rows = parse_haxe_timer_rows(
        result.stdout + "\n" + result.stderr,
        clock,
    )
    accounting = accounting_durations(wall_us, phases)
    return {
        "wallDurationMs": round(wall_us / 1000.0, 3),
        "phases": [
            {"name": phase, "durationMs": round(phases[phase] / 1000.0, 3)}
            for phase in PHASES
        ],
        "details": [
            {"name": detail, "durationMs": round(details[detail] / 1000.0, 3)}
            for detail in DETAIL_PHASES
        ],
        "accounting": [
            {"name": phase, "durationMs": round(accounting[phase] / 1000.0, 3)}
            for phase in ACCOUNTING_PHASES
        ],
        "haxeTimers": [row.to_json() for row in haxe_rows],
    }


def sample_summary(samples: Sequence[dict[str, object]]) -> dict[str, object]:
    walls = [float(sample["wallDurationMs"]) for sample in samples]
    accounting_by_name: dict[str, list[float]] = {
        phase: [] for phase in ACCOUNTING_PHASES
    }
    details_by_name: dict[str, list[float]] = {
        detail: [] for detail in DETAIL_PHASES
    }
    for sample in samples:
        accounting = sample.get("accounting")
        if not isinstance(accounting, list):
            raise CompilerProfileFailure("sample accounting is malformed")
        for item in accounting:
            if not isinstance(item, dict):
                raise CompilerProfileFailure("sample accounting item is malformed")
            name = item.get("name")
            duration = item.get("durationMs")
            if name not in accounting_by_name or not isinstance(duration, (int, float)):
                raise CompilerProfileFailure("sample accounting fields are malformed")
            accounting_by_name[name].append(float(duration))
        details = sample.get("details")
        if not isinstance(details, list):
            raise CompilerProfileFailure("sample details are malformed")
        for item in details:
            if not isinstance(item, dict):
                raise CompilerProfileFailure("sample detail item is malformed")
            name = item.get("name")
            duration = item.get("durationMs")
            if name not in details_by_name or not isinstance(duration, (int, float)):
                raise CompilerProfileFailure("sample detail fields are malformed")
            details_by_name[name].append(float(duration))
    medians = {
        name: round(statistics.median(values), 3)
        for name, values in accounting_by_name.items()
    }
    dominant = max(ACCOUNTING_PHASES, key=lambda name: medians[name])
    detail_medians = {
        name: round(statistics.median(values), 3)
        for name, values in details_by_name.items()
    }
    dominant_detail = max(DETAIL_PHASES, key=lambda name: detail_medians[name])
    return {
        "wallMs": {
            "minimum": round(min(walls), 3),
            "median": round(statistics.median(walls), 3),
            "maximum": round(max(walls), 3),
        },
        "medianAccountingPhases": [
            {"name": name, "durationMs": medians[name]}
            for name in ACCOUNTING_PHASES
        ],
        "dominantAccountingPhase": dominant,
        "medianBodyDetails": [
            {"name": name, "durationMs": detail_medians[name]}
            for name in DETAIL_PHASES
        ],
        "dominantBodyDetail": dominant_detail,
    }


def load_average() -> float | None:
    try:
        value = float(os.getloadavg()[0])
    except (AttributeError, OSError):
        return None
    return round(value, 3)


def host_condition(load: float | None, logical_cpus: int) -> str:
    if load is None:
        return "load-unavailable"
    return "representative" if load / logical_cpus <= 0.5 else "contended"


def profile(runs: int) -> dict[str, object]:
    if runs < 3:
        raise CompilerProfileFailure("compiler profiling requires at least three runs")
    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    clock = timer_clock(installation)
    logical_cpus = os.cpu_count() or 1
    starting_load = load_average()

    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-compiler-profile-") as temporary:
        root = Path(temporary)
        cold_output = root / "cold"
        warm_output = root / "warm"
        common = {
            "layout": "split",
            "reverse": False,
            "report": False,
            "times": True,
            "phase_timing": True,
        }
        cold_resolved = resolve_haxe_arguments(
            target_arguments(cold_output, **common), locale="C"
        )
        warm_resolved = resolve_haxe_arguments(
            target_arguments(warm_output, **common), locale="C"
        )

        cold_samples: list[dict[str, object]] = []
        baseline: dict[str, bytes] | None = None
        for ordinal in range(1, runs + 1):
            print(f"caxecraft-compiler-profile: [cold {ordinal}/{runs}]", flush=True)
            sample = run_sample(installation, cold_resolved, clock, connection=None)
            artifacts = normal_artifacts(cold_output)
            if baseline is None:
                baseline = artifacts
            else:
                assert_artifacts_equal(
                    baseline, artifacts, f"cold compiler profile run {ordinal}"
                )
            sample["ordinal"] = ordinal
            cold_samples.append(sample)

        if baseline is None:
            raise CompilerProfileFailure("cold compiler profile produced no baseline")

        warm_samples: list[dict[str, object]] = []
        with haxe_compilation_server() as connection:
            print("caxecraft-compiler-profile: [warm cache priming]", flush=True)
            run_sample(installation, warm_resolved, clock, connection=connection)
            assert_artifacts_equal(
                baseline,
                normal_artifacts(warm_output),
                "cold/warm compiler profile priming",
            )
            for ordinal in range(1, runs + 1):
                print(
                    f"caxecraft-compiler-profile: [warm {ordinal}/{runs}]",
                    flush=True,
                )
                sample = run_sample(
                    installation, warm_resolved, clock, connection=connection
                )
                assert_artifacts_equal(
                    baseline,
                    normal_artifacts(warm_output),
                    f"warm compiler profile run {ordinal}",
                )
                sample["ordinal"] = ordinal
                warm_samples.append(sample)

        ending_load = load_average()
        return {
            "schemaVersion": 1,
            "suite": "caxecraft-compiler-profile",
            "haxe": {
                "version": installation.version,
                "sourceRevision": PINNED_HAXE_SOURCE_REVISION,
                "timerClock": clock.to_json(),
            },
            "workload": {
                "boundary": "haxe-source-to-generated-c",
                "nativeCompileIncluded": False,
                "layout": "split",
                "runtimePolicy": "none",
                "normalArtifactCount": len(baseline),
                "normalArtifactSha256": artifact_digest(baseline),
            },
            "host": {
                "platform": sys.platform,
                "logicalCpuCount": logical_cpus,
                "loadAverageOneMinuteStart": starting_load,
                "loadAverageOneMinuteEnd": ending_load,
                "condition": host_condition(starting_load, logical_cpus),
            },
            "cold": {
                "transport": "fresh-process",
                "primingRequests": 0,
                "summary": sample_summary(cold_samples),
                "samples": cold_samples,
            },
            "warm": {
                "transport": "owned-loopback-server",
                "primingRequests": 1,
                "summary": sample_summary(warm_samples),
                "samples": warm_samples,
            },
            "parity": "byte-identical-normal-artifacts",
        }


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runs", type=int, default=3)
    parser.add_argument(
        "--output",
        type=Path,
        default=CASE / "_build/compiler-profile.json",
        help="write the path-free JSON report here",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        report = profile(args.runs)
        write_timing_report(args.output, report)
    except (
        CaxecraftFailure,
        CompilerProfileFailure,
        OSError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"caxecraft-compiler-profile: ERROR: {error}", file=sys.stderr)
        return 1
    cold = report["cold"]
    warm = report["warm"]
    assert isinstance(cold, dict) and isinstance(warm, dict)
    cold_summary = cold["summary"]
    warm_summary = warm["summary"]
    assert isinstance(cold_summary, dict) and isinstance(warm_summary, dict)
    cold_wall = cold_summary["wallMs"]
    warm_wall = warm_summary["wallMs"]
    assert isinstance(cold_wall, dict) and isinstance(warm_wall, dict)
    print(
        "caxecraft-compiler-profile: OK: "
        f"cold median {cold_wall['median']}ms; warm median {warm_wall['median']}ms; "
        f"cold dominant {cold_summary['dominantAccountingPhase']}; "
        f"warm dominant {warm_summary['dominantAccountingPhase']}; "
        f"body detail {warm_summary['dominantBodyDetail']}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

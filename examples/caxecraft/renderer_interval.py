"""Validate native terrain observations without treating startup as rendering.

The Haxe game emits cumulative snapshots outside the timed region. Subtraction
keeps warmup visible and preserves the observer's sticky protocol errors. This
module proves observed runtime entries only; static call coverage and native
build provenance remain separate requirements of the renderer benchmark.
"""
from __future__ import annotations

import json
from typing import TypedDict

PREFIX = "CAXECRAFT_TERRAIN_INTERVAL="
COUNTERS = (
    "frames", "portalFrames", "completed", "allocationAttempts",
    "reallocationAttempts", "collectionAttempts", "rootPushAttempts", "rootPopAttempts",
)
FIELDS = frozenset(("schemaVersion", "phase", "valid", *COUNTERS))


class IntervalEvidenceFailure(RuntimeError):
    """The run cannot support a measured-interval claim."""


class IntervalSnapshot(TypedDict):
    """Validated integers retain the native observer's full unsigned range."""
    schemaVersion: int
    phase: str
    valid: bool
    frames: int
    portalFrames: int
    completed: int
    allocationAttempts: int
    reallocationAttempts: int
    collectionAttempts: int
    rootPushAttempts: int
    rootPopAttempts: int


def _unique_object(pairs: list[tuple[str, object]]) -> dict[str, object]:
    result: dict[str, object] = {}
    for key, value in pairs:
        if key in result:
            raise IntervalEvidenceFailure(f"duplicate interval field: {key}")
        result[key] = value
    return result


def _integer(value: object, name: str) -> int:
    if type(value) is not int or not 0 <= value < 2**64:
        raise IntervalEvidenceFailure(f"interval {name} must be an unsigned 64-bit integer")
    return value


def _snapshot(line: str, phase: str) -> IntervalSnapshot:
    try:
        value = json.loads(line, object_pairs_hook=_unique_object)
    except (ValueError, RecursionError) as error:
        raise IntervalEvidenceFailure(f"invalid interval JSON: {error}") from error
    if not isinstance(value, dict) or set(value) != FIELDS:
        raise IntervalEvidenceFailure("interval snapshot fields do not match schema 1")
    if type(value['schemaVersion']) is not int or value['schemaVersion'] != 1:
        raise IntervalEvidenceFailure("unknown interval snapshot schema")
    if value['phase'] != phase or value['valid'] is not True:
        raise IntervalEvidenceFailure(f"missing or invalid {phase} interval snapshot")
    result = IntervalSnapshot(
        schemaVersion=1, phase=phase, valid=True,
        frames=_integer(value['frames'], 'frames'),
        portalFrames=_integer(value['portalFrames'], 'portalFrames'),
        completed=_integer(value['completed'], 'completed'),
        allocationAttempts=_integer(value['allocationAttempts'], 'allocationAttempts'),
        reallocationAttempts=_integer(value['reallocationAttempts'], 'reallocationAttempts'),
        collectionAttempts=_integer(value['collectionAttempts'], 'collectionAttempts'),
        rootPushAttempts=_integer(value['rootPushAttempts'], 'rootPushAttempts'),
        rootPopAttempts=_integer(value['rootPopAttempts'], 'rootPopAttempts'),
    )
    if result['completed'] != result['frames'] or result['portalFrames'] > result['frames']:
        raise IntervalEvidenceFailure("interval markers do not cover the reported terrain frames")
    return result


def parse_interval_evidence(stdout: str, *, measured_frames: int | None, require_portal: bool = False, timing_enabled: bool = True) -> dict[str, object]:
    """Require two valid snapshots and no measured allocation/collection entries.

    An optional external frame expectation protects fixed benchmark runs. Runtime
    journeys derive their measured count from Haxe terrain frames, independently
    checked against native completed intervals at both snapshots.
    Root counters remain separate API-attempt counts. Unequal push/pop counts
    are not rejected: this observer does not measure successful root lifetimes.
    Portal frames report execution coverage; zero does not prove the portal path.
    """
    if measured_frames is not None and (type(measured_frames) is not int or measured_frames <= 0):
        raise IntervalEvidenceFailure("interval evidence requires positive measured frames")
    lines = [line[len(PREFIX):] for line in stdout.splitlines() if line.startswith(PREFIX)]
    if len(lines) != 2:
        raise IntervalEvidenceFailure("expected exactly warmup and complete interval snapshots")
    warmup = _snapshot(lines[0], 'warmup')
    complete = _snapshot(lines[1], 'complete')
    if warmup['frames'] != 2:
        raise IntervalEvidenceFailure("interval evidence must retain the two warmup frames")
    measured: dict[str, int] = {}
    for name in COUNTERS:
        before = _integer(warmup[name], name)
        after = _integer(complete[name], name)
        if after < before:
            raise IntervalEvidenceFailure(f"interval {name} decreased after warmup")
        measured[name] = after - before
    if (measured['frames'] <= 0 or measured['portalFrames'] > measured['frames']
            or (measured_frames is not None and measured['frames'] != measured_frames)):
        raise IntervalEvidenceFailure("interval evidence does not cover the measured terrain frames")
    if require_portal and measured['portalFrames'] == 0:
        raise IntervalEvidenceFailure("portal proof requires a portal draw after warmup")
    for name in ('allocationAttempts', 'reallocationAttempts', 'collectionAttempts'):
        if measured[name] != 0:
            raise IntervalEvidenceFailure(f"measured terrain interval contains {name}={measured[name]}")
    return {
        'schemaVersion': 1,
        'coverage': ('terrain timer, draw arguments and timing bookkeeping' if timing_enabled
                     else 'terrain draw arguments and rendering; no timing claim'),
        'warmup': warmup, 'total': complete, 'measured': measured,
    }

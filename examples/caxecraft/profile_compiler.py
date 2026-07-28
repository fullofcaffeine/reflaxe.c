#!/usr/bin/env python3
"""Measure cold and warm Reflaxe.C work with the Caxecraft program."""

from __future__ import annotations

import argparse
import ctypes
import hashlib
import json
import math
import os
import platform
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
ROOT = CASE.parents[1]
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
PROFILE_PREFIX = "HXC_PROFILE\t"
PROFILE_SCHEMA_VERSION = 9
PINNED_HAXE_SOURCE_REVISION = "2c1e544e0a2c7524ef4c8e103f1b0580362ea538"
PROFILE_WORKLOADS = ("runtime-free", "playable")
PROFILE_TRANSPORTS = ("both", "cold", "warm")
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
    "HxcIR function preparation",
    "HxcIR representation planning",
    "HxcIR function construction",
    "HxcIR typed-body lowering",
    "HxcIR function finalization",
    "HxcIR value coalescing",
    "HxcIR value-plan application",
    "HxcIR program assembly",
    "HxcIR managed-root planning",
    "HxcIR null-check coalescing",
    "semantic helper selection",
    "semantic name registration",
    "symbol finalization",
    "symbol request ordering",
    "symbol draft construction",
    "symbol collision resolution",
    "symbol table materialization",
    "semantic representation finalization",
    "semantic name projection",
    "body setup and value planning",
    "body control-flow planning",
    "body control-flow analysis",
    "body control-flow indexing",
    "body control-flow dominators",
    "body control-flow post-dominators",
    "body control-flow loop analysis",
    "body control-flow construction",
    "body control-flow validation",
    "body CAST emission",
    "runtime catalog construction",
    "runtime direct-evidence projection",
    "runtime requirement reconciliation",
    "runtime feature closure",
    "runtime reason canonicalization",
    "runtime dependency propagation",
    "runtime plan projection",
    "C translation-unit printing",
    "C generated-file construction",
    "artifact runtime packaging",
    "artifact specialization report",
    "artifact project emission",
    "artifact plan validation",
    "artifact unit canonicalization",
    "artifact sidecar construction",
    "artifact symbol-report serialization",
    "artifact initialization-report serialization",
    "artifact runtime-report projection",
    "artifact runtime-report serialization",
    "artifact contract-report serialization",
    "artifact specialization-dispatch serialization",
    "artifact build-adapter construction",
    "artifact manifest construction",
)
CONTROL_FLOW_PLAN_COMPUTATION_DETAILS = frozenset(
    {
        "body control-flow analysis",
        "body control-flow indexing",
        "body control-flow dominators",
        "body control-flow post-dominators",
        "body control-flow loop analysis",
        "body control-flow construction",
        "body control-flow validation",
    }
)
SYMBOL_TABLE_COMPUTATION_DETAILS = frozenset(
    {
        "symbol draft construction",
        "symbol collision resolution",
        "symbol table materialization",
    }
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
PROFILE_COUNTERS = (
    "artifacts.bytes",
    "artifacts.digest-cache-hits",
    "artifacts.digest-cache-misses",
    "artifacts.digest-cache-retained-bytes",
    "artifacts.digest-cache-retained-entries",
    "artifacts.files",
    "cast.control-flow-plan-cache-hits",
    "cast.control-flow-plan-cache-misses",
    "cast.control-flow-plan-cache-retained-functions",
    "cast.control-flow-plan-cache-retained-key-code-units",
    "cast.functions",
    "hxcir.blocks",
    "hxcir.exact-nominal-cache-hits",
    "hxcir.exact-nominal-cache-misses",
    "hxcir.functions",
    "hxcir.globals",
    "hxcir.instructions",
    "hxcir.managed-roots",
    "hxcir.modules",
    "hxcir.named-record-cache-hits",
    "hxcir.named-record-cache-misses",
    "hxcir.type-instances",
    "hxcir.types",
    "output.planned-files",
    "output.prefix-checks",
    "output.prior-owned-files",
    "output.saved-files",
    "runtime.requirements",
    "symbols.assigned-sort-comparisons",
    "symbols.candidate-sort-comparisons",
    "symbols.candidate-sort-utf8-code-units",
    "symbols.candidate-sort-utf8-encodings",
    "symbols.collision-candidates-rechecked",
    "symbols.collision-rounds",
    "symbols.collision-states-moved",
    "symbols.initial-candidates",
    "symbols.primary-request-sort-comparisons",
    "symbols.requests",
    "symbols.table-cache-hits",
    "symbols.table-cache-misses",
    "symbols.table-cache-retained-key-code-units",
    "symbols.table-cache-retained-requests",
    "symbols.table-records",
    "typed.declarations",
    "typed.expression-roots",
    "typed.modules",
)
EXCLUSIVE_ACCOUNTING_NAMES = (
    "external process/frontend remainder",
    "profile request unspanned remainder",
    *PHASES,
    *DETAIL_PHASES,
)
TIMER_ROW = re.compile(
    r"^(?P<label>.*?)\s+\|\s*(?P<seconds>[0-9]+(?:\.[0-9]+)?)\s*"
    r"\|\s*[0-9]+\s*\|\s*[0-9]+\s*\|\s*(?P<calls>[0-9]+)\s*\|.*$"
)


class CompilerProfileFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class CompilerSampleObservation:
    """One validated profile sample plus its opt-in diagnostic records."""

    sample: dict[str, object]
    stdout: str


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


@dataclass(frozen=True)
class CompilerProfileControlFlowWork:
    block_count: int
    normal_join_searches: int
    normal_join_candidate_proofs: int
    normal_join_distance_searches: int
    normal_join_distance_block_visits: int
    completion_set_searches: int
    completion_set_initial_block_scans: int
    completion_set_worklist_dequeues: int
    abrupt_completion_set_searches: int
    abrupt_completion_set_initial_block_scans: int
    abrupt_completion_set_worklist_dequeues: int
    forward_reachability_searches: int
    forward_reachability_block_visits: int
    prefix_disjoint_searches: int
    prefix_disjoint_block_visits: int

    def to_json(self) -> dict[str, object]:
        return {
            "blockCount": self.block_count,
            "normalJoinSearches": self.normal_join_searches,
            "normalJoinCandidateProofs": self.normal_join_candidate_proofs,
            "normalJoinDistanceSearches": self.normal_join_distance_searches,
            "normalJoinDistanceBlockVisits": self.normal_join_distance_block_visits,
            "completionSetSearches": self.completion_set_searches,
            "completionSetInitialBlockScans": self.completion_set_initial_block_scans,
            "completionSetWorklistDequeues": self.completion_set_worklist_dequeues,
            "abruptCompletionSetSearches": self.abrupt_completion_set_searches,
            "abruptCompletionSetInitialBlockScans": self.abrupt_completion_set_initial_block_scans,
            "abruptCompletionSetWorklistDequeues": self.abrupt_completion_set_worklist_dequeues,
            "forwardReachabilitySearches": self.forward_reachability_searches,
            "forwardReachabilityBlockVisits": self.forward_reachability_block_visits,
            "prefixDisjointSearches": self.prefix_disjoint_searches,
            "prefixDisjointBlockVisits": self.prefix_disjoint_block_visits,
        }


@dataclass(frozen=True)
class CompilerProfileTypedBodyWork:
    expression_node_count: int
    statement_lowering_calls: int
    value_lowering_calls: int
    body_value_type_requests: int
    direct_primitive_fast_paths: int
    string_type_classifications: int
    string_type_cpu_microseconds: float
    record_type_classifications: int
    record_type_cpu_microseconds: float
    collection_type_classifications: int
    collection_type_cpu_microseconds: float
    nominal_type_classifications: int
    nominal_type_cpu_microseconds: float
    callable_optional_type_classifications: int
    callable_optional_type_cpu_microseconds: float
    other_type_classifications: int
    other_type_cpu_microseconds: float
    specialization_requests: int
    specialization_cpu_microseconds: float
    coercion_requests: int
    coercion_type_comparison_cpu_microseconds: float
    source_span_requests: int
    source_span_computations: int
    source_span_cache_hits: int
    source_span_cpu_microseconds: float
    produced_block_count: int
    produced_instruction_count: int

    def to_json(self) -> dict[str, object]:
        return {
            "expressionNodeCount": self.expression_node_count,
            "statementLoweringCalls": self.statement_lowering_calls,
            "valueLoweringCalls": self.value_lowering_calls,
            "bodyValueTypeRequests": self.body_value_type_requests,
            "directPrimitiveFastPaths": self.direct_primitive_fast_paths,
            "stringTypeClassifications": self.string_type_classifications,
            "stringTypeCpuMicroseconds": self.string_type_cpu_microseconds,
            "recordTypeClassifications": self.record_type_classifications,
            "recordTypeCpuMicroseconds": self.record_type_cpu_microseconds,
            "collectionTypeClassifications": self.collection_type_classifications,
            "collectionTypeCpuMicroseconds": self.collection_type_cpu_microseconds,
            "nominalTypeClassifications": self.nominal_type_classifications,
            "nominalTypeCpuMicroseconds": self.nominal_type_cpu_microseconds,
            "callableOptionalTypeClassifications": self.callable_optional_type_classifications,
            "callableOptionalTypeCpuMicroseconds": self.callable_optional_type_cpu_microseconds,
            "otherTypeClassifications": self.other_type_classifications,
            "otherTypeCpuMicroseconds": self.other_type_cpu_microseconds,
            "specializationRequests": self.specialization_requests,
            "specializationCpuMicroseconds": self.specialization_cpu_microseconds,
            "coercionRequests": self.coercion_requests,
            "coercionTypeComparisonCpuMicroseconds": self.coercion_type_comparison_cpu_microseconds,
            "sourceSpanRequests": self.source_span_requests,
            "sourceSpanComputations": self.source_span_computations,
            "sourceSpanCacheHits": self.source_span_cache_hits,
            "sourceSpanCpuMicroseconds": self.source_span_cpu_microseconds,
            "producedBlockCount": self.produced_block_count,
            "producedInstructionCount": self.produced_instruction_count,
        }


@dataclass(frozen=True)
class CompilerProfilePrinterWork:
    declaration_count: int
    statement_count: int
    expression_count: int
    output_bytes: int
    indentation_requests: int
    indentation_unit_copies: int
    token_join_calls: int
    token_join_inputs: int
    token_join_outputs: int
    uniqueness_check_calls: int
    uniqueness_check_inputs: int
    utf8_encoding_calls: int
    utf8_input_code_units: int

    def to_json(self) -> dict[str, object]:
        return {
            "declarationCount": self.declaration_count,
            "statementCount": self.statement_count,
            "expressionCount": self.expression_count,
            "outputBytes": self.output_bytes,
            "indentationRequests": self.indentation_requests,
            "indentationUnitCopies": self.indentation_unit_copies,
            "tokenJoinCalls": self.token_join_calls,
            "tokenJoinInputs": self.token_join_inputs,
            "tokenJoinOutputs": self.token_join_outputs,
            "uniquenessCheckCalls": self.uniqueness_check_calls,
            "uniquenessCheckInputs": self.uniqueness_check_inputs,
            "utf8EncodingCalls": self.utf8_encoding_calls,
            "utf8InputCodeUnits": self.utf8_input_code_units,
        }


CompilerProfileSpanWork = (
    CompilerProfileControlFlowWork
    | CompilerProfileTypedBodyWork
    | CompilerProfilePrinterWork
)


@dataclass(frozen=True)
class CompilerProfileSpan:
    span_id: int
    parent_span_id: int | None
    category: str
    name: str
    subject: str | None
    work: CompilerProfileSpanWork | None
    status: str
    start_offset_us: float
    inclusive_wall_us: float
    exclusive_wall_us: float
    inclusive_cpu_us: float
    exclusive_cpu_us: float
    allocated_bytes_delta: float | None
    resident_bytes_at_end: float | None

    def to_json(self) -> dict[str, object]:
        return {
            "spanId": self.span_id,
            "parentSpanId": self.parent_span_id,
            "category": self.category,
            "name": self.name,
            "subject": self.subject,
            "work": (
                {
                    "kind": (
                        "normal-join-search-v1"
                        if isinstance(self.work, CompilerProfileControlFlowWork)
                        else (
                            "typed-body-lowering-v1"
                            if isinstance(self.work, CompilerProfileTypedBodyWork)
                            else "c-printer-v1"
                        )
                    ),
                    "controlFlow": (
                        self.work.to_json()
                        if isinstance(self.work, CompilerProfileControlFlowWork)
                        else None
                    ),
                    "typedBody": (
                        self.work.to_json()
                        if isinstance(self.work, CompilerProfileTypedBodyWork)
                        else None
                    ),
                    "printer": (
                        self.work.to_json()
                        if isinstance(self.work, CompilerProfilePrinterWork)
                        else None
                    ),
                }
                if self.work is not None
                else None
            ),
            "status": self.status,
            "startOffsetMs": round(self.start_offset_us / 1000.0, 3),
            "inclusiveWallMs": round(self.inclusive_wall_us / 1000.0, 3),
            "exclusiveWallMs": round(self.exclusive_wall_us / 1000.0, 3),
            "inclusiveCpuMs": round(self.inclusive_cpu_us / 1000.0, 3),
            "exclusiveCpuMs": round(self.exclusive_cpu_us / 1000.0, 3),
            "allocatedBytesDelta": self.allocated_bytes_delta,
            "residentBytesAtEnd": self.resident_bytes_at_end,
        }


@dataclass(frozen=True)
class CompilerProfileRequest:
    request_id: str
    status: str
    profile: str
    build_mode: str
    haxe_version: str
    wall_us: float
    cpu_us: float
    allocated_bytes_delta: float | None
    maximum_observed_resident_bytes: float | None
    span_count: int
    counter_count: int

    def to_json(self) -> dict[str, object]:
        return {
            "requestId": self.request_id,
            "status": self.status,
            "profile": self.profile,
            "buildMode": self.build_mode,
            "haxeVersion": self.haxe_version,
            "wallDurationMs": round(self.wall_us / 1000.0, 3),
            "cpuDurationMs": round(self.cpu_us / 1000.0, 3),
            "allocatedBytesDelta": self.allocated_bytes_delta,
            "maximumObservedResidentBytes": self.maximum_observed_resident_bytes,
            "spanCount": self.span_count,
            "counterCount": self.counter_count,
        }


@dataclass(frozen=True)
class StructuredCompilerProfile:
    request: CompilerProfileRequest
    spans: tuple[CompilerProfileSpan, ...]
    counters: tuple[tuple[str, float], ...]

    def to_json(self) -> dict[str, object]:
        return {
            "schemaVersion": PROFILE_SCHEMA_VERSION,
            "request": self.request.to_json(),
            "spans": [span.to_json() for span in self.spans],
            "counters": [
                {"name": name, "value": value} for name, value in self.counters
            ],
        }


SPAN_FIELDS = frozenset(
    {
        "schemaVersion",
        "recordKind",
        "requestId",
        "spanId",
        "parentSpanId",
        "category",
        "name",
        "subject",
        "work",
        "status",
        "startOffsetMicroseconds",
        "inclusiveWallMicroseconds",
        "exclusiveWallMicroseconds",
        "inclusiveCpuMicroseconds",
        "exclusiveCpuMicroseconds",
        "allocatedBytesDelta",
        "residentBytesAtEnd",
    }
)
SPAN_WORK_FIELDS = frozenset({"kind", "controlFlow", "typedBody", "printer"})
CONTROL_FLOW_WORK_FIELDS = frozenset(
    {
        "blockCount",
        "normalJoinSearches",
        "normalJoinCandidateProofs",
        "normalJoinDistanceSearches",
        "normalJoinDistanceBlockVisits",
        "completionSetSearches",
        "completionSetInitialBlockScans",
        "completionSetWorklistDequeues",
        "abruptCompletionSetSearches",
        "abruptCompletionSetInitialBlockScans",
        "abruptCompletionSetWorklistDequeues",
        "forwardReachabilitySearches",
        "forwardReachabilityBlockVisits",
        "prefixDisjointSearches",
        "prefixDisjointBlockVisits",
    }
)
TYPED_BODY_WORK_FIELDS = frozenset(
    {
        "expressionNodeCount",
        "statementLoweringCalls",
        "valueLoweringCalls",
        "bodyValueTypeRequests",
        "directPrimitiveFastPaths",
        "stringTypeClassifications",
        "stringTypeCpuMicroseconds",
        "recordTypeClassifications",
        "recordTypeCpuMicroseconds",
        "collectionTypeClassifications",
        "collectionTypeCpuMicroseconds",
        "nominalTypeClassifications",
        "nominalTypeCpuMicroseconds",
        "callableOptionalTypeClassifications",
        "callableOptionalTypeCpuMicroseconds",
        "otherTypeClassifications",
        "otherTypeCpuMicroseconds",
        "specializationRequests",
        "specializationCpuMicroseconds",
        "coercionRequests",
        "coercionTypeComparisonCpuMicroseconds",
        "sourceSpanRequests",
        "sourceSpanComputations",
        "sourceSpanCacheHits",
        "sourceSpanCpuMicroseconds",
        "producedBlockCount",
        "producedInstructionCount",
    }
)
PRINTER_WORK_FIELDS = frozenset(
    {
        "declarationCount",
        "statementCount",
        "expressionCount",
        "outputBytes",
        "indentationRequests",
        "indentationUnitCopies",
        "tokenJoinCalls",
        "tokenJoinInputs",
        "tokenJoinOutputs",
        "uniquenessCheckCalls",
        "uniquenessCheckInputs",
        "utf8EncodingCalls",
        "utf8InputCodeUnits",
    }
)
COUNTER_FIELDS = frozenset(
    {"schemaVersion", "recordKind", "requestId", "name", "value"}
)
REQUEST_FIELDS = frozenset(
    {
        "schemaVersion",
        "recordKind",
        "requestId",
        "status",
        "profile",
        "buildMode",
        "haxeVersion",
        "wallMicroseconds",
        "cpuMicroseconds",
        "allocatedBytesDelta",
        "maximumObservedResidentBytes",
        "spanCount",
        "counterCount",
    }
)
PROFILE_STATUSES = frozenset({"ok", "failed", "superseded"})


def profile_string(record: Mapping[str, object], field: str) -> str:
    value = record.get(field)
    if not isinstance(value, str) or not value:
        raise CompilerProfileFailure(
            f"compiler profile field {field!r} must be a non-empty string"
        )
    return value


def profile_number(
    record: Mapping[str, object], field: str, *, optional: bool = False
) -> float | None:
    value = record.get(field)
    if optional and value is None:
        return None
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise CompilerProfileFailure(
            f"compiler profile field {field!r} must be a number"
        )
    result = float(value)
    if not math.isfinite(result) or result < 0.0:
        raise CompilerProfileFailure(
            f"compiler profile field {field!r} must be finite and non-negative"
        )
    return result


def profile_integer(record: Mapping[str, object], field: str, *, minimum: int) -> int:
    value = record.get(field)
    if isinstance(value, bool) or not isinstance(value, int) or value < minimum:
        raise CompilerProfileFailure(
            f"compiler profile field {field!r} must be an integer >= {minimum}"
        )
    return value


def require_profile_fields(
    record: Mapping[str, object], expected: frozenset[str], kind: str
) -> None:
    observed = frozenset(record)
    if observed != expected:
        missing = sorted(expected - observed)
        extra = sorted(observed - expected)
        raise CompilerProfileFailure(
            f"compiler profile {kind} fields drifted; missing={missing}, extra={extra}"
        )
    if record.get("schemaVersion") != PROFILE_SCHEMA_VERSION:
        raise CompilerProfileFailure(
            f"compiler profile {kind} has unsupported schema "
            f"{record.get('schemaVersion')!r}"
        )
    if record.get("recordKind") != kind:
        raise CompilerProfileFailure(
            f"compiler profile record kind drifted from {kind!r}"
        )


def require_profile_fields_without_schema(
    record: Mapping[str, object], expected: frozenset[str], kind: str
) -> None:
    """Reject missing or accidental work-record fields before reading values."""

    observed = frozenset(record)
    if observed != expected:
        missing = sorted(expected - observed)
        extra = sorted(observed - expected)
        raise CompilerProfileFailure(
            f"compiler profile {kind} fields drifted; "
            f"missing={missing}, extra={extra}"
        )


def parse_profile_records(
    stdout: str, *, expected_status: str | None = None
) -> StructuredCompilerProfile:
    """Parse and independently validate one nested compiler-profile request.

    The compiler emits spans when they close, so children appear before their
    parent. This parser checks that ordering, containment, exclusive-time
    arithmetic, counters, and the final request summary rather than trusting
    the producer's JSON merely because it decoded.
    """

    decoded: list[Mapping[str, object]] = []
    for line in stdout.splitlines():
        if not line.startswith(PROFILE_PREFIX):
            continue
        payload = line.removeprefix(PROFILE_PREFIX)
        try:
            value = json.loads(payload)
        except json.JSONDecodeError as error:
            raise CompilerProfileFailure(
                f"malformed compiler profile JSON: {payload!r}"
            ) from error
        if not isinstance(value, dict):
            raise CompilerProfileFailure("compiler profile record must be an object")
        decoded.append(value)
    if not decoded:
        raise CompilerProfileFailure("compiler profile stream was not emitted")
    request_records = [
        (index, record)
        for index, record in enumerate(decoded)
        if record.get("recordKind") == "request"
    ]
    if len(request_records) != 1 or request_records[0][0] != len(decoded) - 1:
        raise CompilerProfileFailure(
            "compiler profile must end with exactly one request record"
        )

    request_record = request_records[0][1]
    require_profile_fields(request_record, REQUEST_FIELDS, "request")
    request_id = profile_string(request_record, "requestId")
    request_status = profile_string(request_record, "status")
    if request_status not in PROFILE_STATUSES:
        raise CompilerProfileFailure(
            f"compiler profile request has unknown status {request_status!r}"
        )
    if expected_status is not None and request_status != expected_status:
        raise CompilerProfileFailure(
            f"compiler profile request status was {request_status!r}, "
            f"expected {expected_status!r}"
        )
    request = CompilerProfileRequest(
        request_id=request_id,
        status=request_status,
        profile=profile_string(request_record, "profile"),
        build_mode=profile_string(request_record, "buildMode"),
        haxe_version=profile_string(request_record, "haxeVersion"),
        wall_us=profile_number(request_record, "wallMicroseconds") or 0.0,
        cpu_us=profile_number(request_record, "cpuMicroseconds") or 0.0,
        allocated_bytes_delta=profile_number(
            request_record, "allocatedBytesDelta", optional=True
        ),
        maximum_observed_resident_bytes=profile_number(
            request_record, "maximumObservedResidentBytes", optional=True
        ),
        span_count=profile_integer(request_record, "spanCount", minimum=0),
        counter_count=profile_integer(request_record, "counterCount", minimum=0),
    )

    spans: list[CompilerProfileSpan] = []
    span_record_positions: dict[int, int] = {}
    counters: list[tuple[str, float]] = []
    saw_counter = False
    for index, record in enumerate(decoded[:-1]):
        kind = record.get("recordKind")
        if kind == "span":
            if saw_counter:
                raise CompilerProfileFailure(
                    "compiler profile span appeared after its counter section"
                )
            require_profile_fields(record, SPAN_FIELDS, "span")
            if profile_string(record, "requestId") != request_id:
                raise CompilerProfileFailure(
                    "compiler profile span belongs to another request"
                )
            span_id = profile_integer(record, "spanId", minimum=1)
            if span_id in span_record_positions:
                raise CompilerProfileFailure(
                    f"compiler profile repeated span id {span_id}"
                )
            parent_value = record.get("parentSpanId")
            if parent_value is None:
                parent_span_id = None
            elif (
                isinstance(parent_value, bool)
                or not isinstance(parent_value, int)
                or parent_value < 1
            ):
                raise CompilerProfileFailure(
                    "compiler profile parentSpanId must be null or a positive integer"
                )
            else:
                parent_span_id = parent_value
            category = profile_string(record, "category")
            name = profile_string(record, "name")
            if category == "phase":
                if name not in PHASES:
                    raise CompilerProfileFailure(
                        f"compiler profile contains unknown phase {name!r}"
                    )
            elif category == "detail":
                if name not in DETAIL_PHASES:
                    raise CompilerProfileFailure(
                        f"compiler profile contains unknown detail {name!r}"
                    )
            else:
                raise CompilerProfileFailure(
                    f"compiler profile contains unknown category {category!r}"
                )
            subject_value = record.get("subject")
            if subject_value is not None and (
                not isinstance(subject_value, str) or not subject_value
            ):
                raise CompilerProfileFailure(
                    "compiler profile span subject must be null or a non-empty string"
                )
            work_value = record.get("work")
            if work_value is None:
                work = None
            else:
                if not isinstance(work_value, dict):
                    raise CompilerProfileFailure(
                        "compiler profile span work must be null or an object"
                    )
                require_profile_fields_without_schema(
                    work_value, SPAN_WORK_FIELDS, "span work"
                )
                kind = profile_string(work_value, "kind")
                control_flow_value = work_value.get("controlFlow")
                typed_body_value = work_value.get("typedBody")
                printer_value = work_value.get("printer")
                if kind == "normal-join-search-v1":
                    if (
                        not isinstance(control_flow_value, dict)
                        or typed_body_value is not None
                        or printer_value is not None
                    ):
                        raise CompilerProfileFailure(
                            "normal-join span work requires only a controlFlow payload"
                        )
                    require_profile_fields_without_schema(
                        control_flow_value,
                        CONTROL_FLOW_WORK_FIELDS,
                        "control-flow span work",
                    )
                    work = CompilerProfileControlFlowWork(
                        block_count=profile_integer(
                            control_flow_value, "blockCount", minimum=0
                        ),
                        normal_join_searches=profile_integer(
                            control_flow_value, "normalJoinSearches", minimum=0
                        ),
                        normal_join_candidate_proofs=profile_integer(
                            control_flow_value,
                            "normalJoinCandidateProofs",
                            minimum=0,
                        ),
                        normal_join_distance_searches=profile_integer(
                            control_flow_value,
                            "normalJoinDistanceSearches",
                            minimum=0,
                        ),
                        normal_join_distance_block_visits=profile_integer(
                            control_flow_value,
                            "normalJoinDistanceBlockVisits",
                            minimum=0,
                        ),
                        completion_set_searches=profile_integer(
                            control_flow_value, "completionSetSearches", minimum=0
                        ),
                        completion_set_initial_block_scans=profile_integer(
                            control_flow_value,
                            "completionSetInitialBlockScans",
                            minimum=0,
                        ),
                        completion_set_worklist_dequeues=profile_integer(
                            control_flow_value,
                            "completionSetWorklistDequeues",
                            minimum=0,
                        ),
                        abrupt_completion_set_searches=profile_integer(
                            control_flow_value,
                            "abruptCompletionSetSearches",
                            minimum=0,
                        ),
                        abrupt_completion_set_initial_block_scans=profile_integer(
                            control_flow_value,
                            "abruptCompletionSetInitialBlockScans",
                            minimum=0,
                        ),
                        abrupt_completion_set_worklist_dequeues=profile_integer(
                            control_flow_value,
                            "abruptCompletionSetWorklistDequeues",
                            minimum=0,
                        ),
                        forward_reachability_searches=profile_integer(
                            control_flow_value,
                            "forwardReachabilitySearches",
                            minimum=0,
                        ),
                        forward_reachability_block_visits=profile_integer(
                            control_flow_value,
                            "forwardReachabilityBlockVisits",
                            minimum=0,
                        ),
                        prefix_disjoint_searches=profile_integer(
                            control_flow_value,
                            "prefixDisjointSearches",
                            minimum=0,
                        ),
                        prefix_disjoint_block_visits=profile_integer(
                            control_flow_value,
                            "prefixDisjointBlockVisits",
                            minimum=0,
                        ),
                    )
                    if (
                        category != "detail"
                        or name != "body control-flow planning"
                        or subject_value is None
                    ):
                        raise CompilerProfileFailure(
                            "normal-join work belongs only to a function-scoped "
                            "body control-flow detail span"
                        )
                elif kind == "typed-body-lowering-v1":
                    if (
                        control_flow_value is not None
                        or not isinstance(typed_body_value, dict)
                        or printer_value is not None
                    ):
                        raise CompilerProfileFailure(
                            "typed-body span work requires only a typedBody payload"
                        )
                    require_profile_fields_without_schema(
                        typed_body_value,
                        TYPED_BODY_WORK_FIELDS,
                        "typed-body span work",
                    )
                    work = CompilerProfileTypedBodyWork(
                        expression_node_count=profile_integer(
                            typed_body_value, "expressionNodeCount", minimum=1
                        ),
                        statement_lowering_calls=profile_integer(
                            typed_body_value, "statementLoweringCalls", minimum=0
                        ),
                        value_lowering_calls=profile_integer(
                            typed_body_value, "valueLoweringCalls", minimum=0
                        ),
                        body_value_type_requests=profile_integer(
                            typed_body_value, "bodyValueTypeRequests", minimum=0
                        ),
                        direct_primitive_fast_paths=profile_integer(
                            typed_body_value, "directPrimitiveFastPaths", minimum=0
                        ),
                        string_type_classifications=profile_integer(
                            typed_body_value, "stringTypeClassifications", minimum=0
                        ),
                        string_type_cpu_microseconds=profile_number(
                            typed_body_value, "stringTypeCpuMicroseconds"
                        ),
                        record_type_classifications=profile_integer(
                            typed_body_value, "recordTypeClassifications", minimum=0
                        ),
                        record_type_cpu_microseconds=profile_number(
                            typed_body_value, "recordTypeCpuMicroseconds"
                        ),
                        collection_type_classifications=profile_integer(
                            typed_body_value,
                            "collectionTypeClassifications",
                            minimum=0,
                        ),
                        collection_type_cpu_microseconds=profile_number(
                            typed_body_value,
                            "collectionTypeCpuMicroseconds",
                        ),
                        nominal_type_classifications=profile_integer(
                            typed_body_value, "nominalTypeClassifications", minimum=0
                        ),
                        nominal_type_cpu_microseconds=profile_number(
                            typed_body_value, "nominalTypeCpuMicroseconds"
                        ),
                        callable_optional_type_classifications=profile_integer(
                            typed_body_value,
                            "callableOptionalTypeClassifications",
                            minimum=0,
                        ),
                        callable_optional_type_cpu_microseconds=profile_number(
                            typed_body_value,
                            "callableOptionalTypeCpuMicroseconds",
                        ),
                        other_type_classifications=profile_integer(
                            typed_body_value, "otherTypeClassifications", minimum=0
                        ),
                        other_type_cpu_microseconds=profile_number(
                            typed_body_value, "otherTypeCpuMicroseconds"
                        ),
                        specialization_requests=profile_integer(
                            typed_body_value, "specializationRequests", minimum=0
                        ),
                        specialization_cpu_microseconds=profile_number(
                            typed_body_value,
                            "specializationCpuMicroseconds",
                        ),
                        coercion_requests=profile_integer(
                            typed_body_value, "coercionRequests", minimum=0
                        ),
                        coercion_type_comparison_cpu_microseconds=profile_number(
                            typed_body_value,
                            "coercionTypeComparisonCpuMicroseconds",
                        ),
                        source_span_requests=profile_integer(
                            typed_body_value, "sourceSpanRequests", minimum=0
                        ),
                        source_span_computations=profile_integer(
                            typed_body_value, "sourceSpanComputations", minimum=0
                        ),
                        source_span_cache_hits=profile_integer(
                            typed_body_value, "sourceSpanCacheHits", minimum=0
                        ),
                        source_span_cpu_microseconds=profile_number(
                            typed_body_value, "sourceSpanCpuMicroseconds"
                        ),
                        produced_block_count=profile_integer(
                            typed_body_value, "producedBlockCount", minimum=0
                        ),
                        produced_instruction_count=profile_integer(
                            typed_body_value, "producedInstructionCount", minimum=0
                        ),
                    )
                    if (
                        work.source_span_computations + work.source_span_cache_hits
                        != work.source_span_requests
                    ):
                        raise CompilerProfileFailure(
                            "typed-body source-span computations and cache hits "
                            "must account for every request"
                        )
                    if (
                        category != "detail"
                        or name != "HxcIR typed-body lowering"
                        or subject_value is None
                    ):
                        raise CompilerProfileFailure(
                            "typed-body work belongs only to a function-scoped "
                            "HxcIR typed-body detail span"
                        )
                elif kind == "c-printer-v1":
                    if (
                        control_flow_value is not None
                        or typed_body_value is not None
                        or not isinstance(printer_value, dict)
                    ):
                        raise CompilerProfileFailure(
                            "C-printer span work requires only a printer payload"
                        )
                    require_profile_fields_without_schema(
                        printer_value,
                        PRINTER_WORK_FIELDS,
                        "C-printer span work",
                    )
                    work = CompilerProfilePrinterWork(
                        declaration_count=profile_integer(
                            printer_value, "declarationCount", minimum=0
                        ),
                        statement_count=profile_integer(
                            printer_value, "statementCount", minimum=0
                        ),
                        expression_count=profile_integer(
                            printer_value, "expressionCount", minimum=0
                        ),
                        output_bytes=profile_integer(
                            printer_value, "outputBytes", minimum=1
                        ),
                        indentation_requests=profile_integer(
                            printer_value, "indentationRequests", minimum=0
                        ),
                        indentation_unit_copies=profile_integer(
                            printer_value, "indentationUnitCopies", minimum=0
                        ),
                        token_join_calls=profile_integer(
                            printer_value, "tokenJoinCalls", minimum=0
                        ),
                        token_join_inputs=profile_integer(
                            printer_value, "tokenJoinInputs", minimum=0
                        ),
                        token_join_outputs=profile_integer(
                            printer_value, "tokenJoinOutputs", minimum=0
                        ),
                        uniqueness_check_calls=profile_integer(
                            printer_value, "uniquenessCheckCalls", minimum=0
                        ),
                        uniqueness_check_inputs=profile_integer(
                            printer_value, "uniquenessCheckInputs", minimum=0
                        ),
                        utf8_encoding_calls=profile_integer(
                            printer_value, "utf8EncodingCalls", minimum=0
                        ),
                        utf8_input_code_units=profile_integer(
                            printer_value, "utf8InputCodeUnits", minimum=0
                        ),
                    )
                    if (
                        category != "detail"
                        or name != "C translation-unit printing"
                        or subject_value is None
                    ):
                        raise CompilerProfileFailure(
                            "C-printer work belongs only to a file-scoped "
                            "C translation-unit printing detail span"
                        )
                else:
                    raise CompilerProfileFailure(
                        f"compiler profile span work has unknown kind {kind!r}"
                    )
            status = profile_string(record, "status")
            if status not in PROFILE_STATUSES:
                raise CompilerProfileFailure(
                    f"compiler profile span has unknown status {status!r}"
                )
            span = CompilerProfileSpan(
                span_id=span_id,
                parent_span_id=parent_span_id,
                category=category,
                name=name,
                subject=subject_value,
                work=work,
                status=status,
                start_offset_us=profile_number(
                    record, "startOffsetMicroseconds"
                )
                or 0.0,
                inclusive_wall_us=profile_number(
                    record, "inclusiveWallMicroseconds"
                )
                or 0.0,
                exclusive_wall_us=profile_number(
                    record, "exclusiveWallMicroseconds"
                )
                or 0.0,
                inclusive_cpu_us=profile_number(
                    record, "inclusiveCpuMicroseconds"
                )
                or 0.0,
                exclusive_cpu_us=profile_number(
                    record, "exclusiveCpuMicroseconds"
                )
                or 0.0,
                allocated_bytes_delta=profile_number(
                    record, "allocatedBytesDelta", optional=True
                ),
                resident_bytes_at_end=profile_number(
                    record, "residentBytesAtEnd", optional=True
                ),
            )
            if span.exclusive_wall_us > span.inclusive_wall_us + 0.001:
                raise CompilerProfileFailure(
                    f"compiler profile span {span_id} has exclusive wall time "
                    "greater than inclusive wall time"
                )
            if span.exclusive_cpu_us > span.inclusive_cpu_us + 0.001:
                raise CompilerProfileFailure(
                    f"compiler profile span {span_id} has exclusive CPU time "
                    "greater than inclusive CPU time"
                )
            span_record_positions[span_id] = index
            spans.append(span)
        elif kind == "counter":
            saw_counter = True
            require_profile_fields(record, COUNTER_FIELDS, "counter")
            if profile_string(record, "requestId") != request_id:
                raise CompilerProfileFailure(
                    "compiler profile counter belongs to another request"
                )
            name = profile_string(record, "name")
            if name not in PROFILE_COUNTERS:
                raise CompilerProfileFailure(
                    f"compiler profile contains unknown counter {name!r}"
                )
            value = profile_number(record, "value")
            assert value is not None
            counters.append((name, value))
        else:
            raise CompilerProfileFailure(
                f"compiler profile contains unknown record kind {kind!r}"
            )

    if len(spans) != request.span_count:
        raise CompilerProfileFailure(
            "compiler profile request span count does not match emitted spans"
        )
    if len(counters) != request.counter_count:
        raise CompilerProfileFailure(
            "compiler profile request counter count does not match emitted counters"
        )
    counter_names = [name for name, _ in counters]
    if counter_names != sorted(counter_names) or len(counter_names) != len(
        set(counter_names)
    ):
        raise CompilerProfileFailure(
            "compiler profile counters must be unique and UTF-8 sorted"
        )

    spans_by_id = {span.span_id: span for span in spans}
    children: dict[int, list[CompilerProfileSpan]] = {
        span.span_id: [] for span in spans
    }
    roots: list[CompilerProfileSpan] = []
    for span in spans:
        if span.parent_span_id is None:
            roots.append(span)
            continue
        parent = spans_by_id.get(span.parent_span_id)
        if parent is None:
            raise CompilerProfileFailure(
                f"compiler profile span {span.span_id} has missing parent "
                f"{span.parent_span_id}"
            )
        if span_record_positions[span.span_id] >= span_record_positions[parent.span_id]:
            raise CompilerProfileFailure(
                f"compiler profile span {span.span_id} was not emitted before "
                f"parent {parent.span_id}"
            )
        child_end = span.start_offset_us + span.inclusive_wall_us
        parent_end = parent.start_offset_us + parent.inclusive_wall_us
        if (
            span.start_offset_us + 0.001 < parent.start_offset_us
            or child_end > parent_end + 5.0
        ):
            raise CompilerProfileFailure(
                f"compiler profile span {span.span_id} escapes parent "
                f"{parent.span_id}"
            )
        children[parent.span_id].append(span)

    for span in spans:
        child_wall = sum(child.inclusive_wall_us for child in children[span.span_id])
        child_cpu = sum(child.inclusive_cpu_us for child in children[span.span_id])
        expected_wall = max(0.0, span.inclusive_wall_us - child_wall)
        expected_cpu = max(0.0, span.inclusive_cpu_us - child_cpu)
        tolerance = max(5.0, span.inclusive_wall_us * 1e-9)
        if abs(span.exclusive_wall_us - expected_wall) > tolerance:
            raise CompilerProfileFailure(
                f"compiler profile span {span.span_id} has inconsistent "
                "exclusive wall time"
            )
        cpu_tolerance = max(5.0, span.inclusive_cpu_us * 1e-9)
        if abs(span.exclusive_cpu_us - expected_cpu) > cpu_tolerance:
            raise CompilerProfileFailure(
                f"compiler profile span {span.span_id} has inconsistent "
                "exclusive CPU time"
            )
    root_wall = sum(span.inclusive_wall_us for span in roots)
    if root_wall > request.wall_us + 5.0:
        raise CompilerProfileFailure(
            "compiler profile root spans exceed the request wall duration"
        )
    if request_status == "ok" and any(span.status != "ok" for span in spans):
        raise CompilerProfileFailure(
            "successful compiler profile contains a non-success span"
        )
    if request_status != "ok" and spans and not any(
        span.status == request_status for span in spans
    ):
        raise CompilerProfileFailure(
            "failed compiler profile does not identify an active failed span"
        )
    return StructuredCompilerProfile(request, tuple(spans), tuple(counters))


def validate_success_profile_contract(profile: StructuredCompilerProfile) -> None:
    """Require the complete successful compiler pipeline and counter inventory."""

    phases = [span.name for span in profile.spans if span.category == "phase"]
    for phase in PHASES:
        if phases.count(phase) != 1:
            raise CompilerProfileFailure(
                f"successful compiler profile expected one {phase!r} span"
            )
    details = [span.name for span in profile.spans if span.category == "detail"]
    allowed_missing = allowed_missing_detail_phases(profile)
    for detail in DETAIL_PHASES:
        if detail not in details and detail not in allowed_missing:
            raise CompilerProfileFailure(
                f"successful compiler profile omitted detail {detail!r}"
            )
    actual_counters = tuple(name for name, _ in profile.counters)
    if actual_counters != PROFILE_COUNTERS:
        raise CompilerProfileFailure(
            "successful compiler profile counter inventory drifted: "
            f"expected={PROFILE_COUNTERS!r} actual={actual_counters!r}"
        )


def allowed_missing_detail_phases(
    profile: StructuredCompilerProfile,
) -> frozenset[str]:
    """Recognize computations deliberately skipped by exact validated hits.

    The broad control-flow-planning span still measures each lookup. Its seven
    internal computations are absent only when every function reused a prior
    validated plan. Exact symbol-table reuse likewise keeps request ordering
    but omits draft, collision, and table construction. Requiring fake
    zero-duration spans would misdescribe work that did not run.
    """

    counters = dict(profile.counters)
    hits = counters.get("cast.control-flow-plan-cache-hits", 0.0)
    misses = counters.get("cast.control-flow-plan-cache-misses", 0.0)
    missing: set[str] = set()
    if hits > 0.0 and misses == 0.0:
        missing.update(CONTROL_FLOW_PLAN_COMPUTATION_DETAILS)
    symbol_hits = counters.get("symbols.table-cache-hits", 0.0)
    symbol_misses = counters.get("symbols.table-cache-misses", 0.0)
    if symbol_hits > 0.0 and symbol_misses == 0.0:
        missing.update(SYMBOL_TABLE_COMPUTATION_DETAILS)
    return frozenset(missing)


def exclusive_accounting(
    wall_us: int, profile: StructuredCompilerProfile
) -> tuple[dict[str, float], dict[str, float]]:
    """Aggregate non-overlapping wall and CPU work using emitted span ownership."""

    wall = {name: 0.0 for name in EXCLUSIVE_ACCOUNTING_NAMES}
    cpu = {name: 0.0 for name in EXCLUSIVE_ACCOUNTING_NAMES}
    for span in profile.spans:
        wall[span.name] += span.exclusive_wall_us
        cpu[span.name] += span.exclusive_cpu_us
    root_wall = sum(
        span.inclusive_wall_us
        for span in profile.spans
        if span.parent_span_id is None
    )
    root_cpu = sum(
        span.inclusive_cpu_us
        for span in profile.spans
        if span.parent_span_id is None
    )
    wall["profile request unspanned remainder"] = max(
        0.0, profile.request.wall_us - root_wall
    )
    cpu["profile request unspanned remainder"] = max(
        0.0, profile.request.cpu_us - root_cpu
    )
    wall["external process/frontend remainder"] = max(
        0.0, float(wall_us) - profile.request.wall_us
    )
    return wall, cpu


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


def parse_detail_records(
    stdout: str, *, allowed_missing: frozenset[str] = frozenset()
) -> dict[str, int]:
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
    missing = [
        detail
        for detail in DETAIL_PHASES
        if counts[detail] == 0 and detail not in allowed_missing
    ]
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


def run_observed_sample(
    installation: HaxeInstallation,
    resolved: Sequence[str],
    clock: TimerClock,
    *,
    connection: HaxeServerConnection | None,
    timeout: int = 180,
) -> CompilerSampleObservation:
    environment = pinned_haxe_environment("C", installation)
    if connection is None:
        environment["HAXE_NO_SERVER"] = "1"
    logical_cpus = os.cpu_count() or 1
    starting_load = load_average()
    started = time.monotonic_ns()
    result = subprocess.run(
        compiler_command(installation, resolved, connection),
        cwd=CASE,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=timeout,
    )
    wall_us = elapsed_microseconds(started)
    ending_load = load_average()
    if result.returncode != 0:
        raise CompilerProfileFailure(
            "Caxecraft compiler sample failed\n"
            f"exit={result.returncode}\nstdout:\n{result.stdout}\n"
            f"stderr:\n{result.stderr}"
        )
    validate_haxe_timer_stream(result.stderr)
    phases = parse_phase_records(result.stdout)
    structured = parse_profile_records(result.stdout, expected_status="ok")
    validate_success_profile_contract(structured)
    details = parse_detail_records(
        result.stdout,
        allowed_missing=allowed_missing_detail_phases(structured),
    )
    haxe_rows = parse_haxe_timer_rows(
        result.stdout + "\n" + result.stderr,
        clock,
    )
    legacy_accounting = accounting_durations(wall_us, phases)
    exclusive_wall, exclusive_cpu = exclusive_accounting(wall_us, structured)
    sample = {
        "wallDurationMs": round(wall_us / 1000.0, 3),
        "host": {
            "loadAverageOneMinuteStart": starting_load,
            "loadAverageOneMinuteEnd": ending_load,
            "condition": host_condition(
                (starting_load, ending_load), logical_cpus
            ),
        },
        "profile": structured.to_json(),
        "phases": [
            {"name": phase, "durationMs": round(phases[phase] / 1000.0, 3)}
            for phase in PHASES
        ],
        "details": [
            {"name": detail, "durationMs": round(details[detail] / 1000.0, 3)}
            for detail in DETAIL_PHASES
        ],
        "accounting": [
            {
                "name": phase,
                "durationMs": round(legacy_accounting[phase] / 1000.0, 3),
            }
            for phase in ACCOUNTING_PHASES
        ],
        "exclusiveAccounting": [
            {
                "name": name,
                "wallDurationMs": round(exclusive_wall[name] / 1000.0, 3),
                "cpuDurationMs": round(exclusive_cpu[name] / 1000.0, 3),
            }
            for name in EXCLUSIVE_ACCOUNTING_NAMES
        ],
        "haxeTimers": [row.to_json() for row in haxe_rows],
    }
    return CompilerSampleObservation(sample, result.stdout)


def run_sample(
    installation: HaxeInstallation,
    resolved: Sequence[str],
    clock: TimerClock,
    *,
    connection: HaxeServerConnection | None,
    timeout: int = 180,
) -> dict[str, object]:
    """Return the stable sample surface used by the repeated profiler."""

    return run_observed_sample(
        installation,
        resolved,
        clock,
        connection=connection,
        timeout=timeout,
    ).sample


def distribution(values: Sequence[float]) -> dict[str, float | int]:
    """Summarize repeated samples without hiding spread behind one average."""

    if not values:
        raise CompilerProfileFailure("cannot summarize an empty sample set")
    median = statistics.median(values)
    deviations = [abs(value - median) for value in values]
    p95 = (
        values[0]
        if len(values) == 1
        else statistics.quantiles(values, n=100, method="inclusive")[94]
    )
    return {
        "samples": len(values),
        "minimum": round(min(values), 3),
        "median": round(median, 3),
        "medianAbsoluteDeviation": round(statistics.median(deviations), 3),
        "p95": round(p95, 3),
        "maximum": round(max(values), 3),
    }


def sample_summary(samples: Sequence[dict[str, object]]) -> dict[str, object]:
    walls = [float(sample["wallDurationMs"]) for sample in samples]
    request_cpu: list[float] = []
    allocated_bytes: list[float] = []
    resident_bytes: list[float] = []
    counters_by_name: dict[str, list[float]] = {
        name: [] for name in PROFILE_COUNTERS
    }
    condition_counts: dict[str, int] = {}
    exclusive_wall_by_name: dict[str, list[float]] = {
        phase: [] for phase in EXCLUSIVE_ACCOUNTING_NAMES
    }
    exclusive_cpu_by_name: dict[str, list[float]] = {
        phase: [] for phase in EXCLUSIVE_ACCOUNTING_NAMES
    }
    for sample in samples:
        profile = sample.get("profile")
        if not isinstance(profile, dict):
            raise CompilerProfileFailure("sample profile is malformed")
        request = profile.get("request")
        counters = profile.get("counters")
        if not isinstance(request, dict) or not isinstance(counters, list):
            raise CompilerProfileFailure("sample profile request is malformed")
        cpu_duration = request.get("cpuDurationMs")
        allocation = request.get("allocatedBytesDelta")
        resident = request.get("maximumObservedResidentBytes")
        if isinstance(cpu_duration, bool) or not isinstance(
            cpu_duration, (int, float)
        ):
            raise CompilerProfileFailure("sample request CPU duration is malformed")
        request_cpu.append(float(cpu_duration))
        if isinstance(allocation, (int, float)) and not isinstance(allocation, bool):
            allocated_bytes.append(float(allocation))
        if isinstance(resident, (int, float)) and not isinstance(resident, bool):
            resident_bytes.append(float(resident))
        for item in counters:
            if not isinstance(item, dict):
                raise CompilerProfileFailure("sample counter is malformed")
            name = item.get("name")
            value = item.get("value")
            if (
                name not in counters_by_name
                or isinstance(value, bool)
                or not isinstance(value, (int, float))
            ):
                raise CompilerProfileFailure("sample counter fields are malformed")
            counters_by_name[name].append(float(value))
        host = sample.get("host")
        condition = host.get("condition") if isinstance(host, dict) else None
        if not isinstance(condition, str):
            raise CompilerProfileFailure("sample host condition is malformed")
        condition_counts[condition] = condition_counts.get(condition, 0) + 1

        accounting = sample.get("exclusiveAccounting")
        if not isinstance(accounting, list):
            raise CompilerProfileFailure("sample exclusive accounting is malformed")
        for item in accounting:
            if not isinstance(item, dict):
                raise CompilerProfileFailure(
                    "sample exclusive accounting item is malformed"
                )
            name = item.get("name")
            wall_duration = item.get("wallDurationMs")
            cpu_duration = item.get("cpuDurationMs")
            if (
                name not in exclusive_wall_by_name
                or isinstance(wall_duration, bool)
                or not isinstance(wall_duration, (int, float))
                or isinstance(cpu_duration, bool)
                or not isinstance(cpu_duration, (int, float))
            ):
                raise CompilerProfileFailure(
                    "sample exclusive accounting fields are malformed"
                )
            exclusive_wall_by_name[name].append(float(wall_duration))
            exclusive_cpu_by_name[name].append(float(cpu_duration))
    exclusive_wall_medians = {
        name: round(statistics.median(values), 3)
        for name, values in exclusive_wall_by_name.items()
    }
    exclusive_cpu_medians = {
        name: round(statistics.median(values), 3)
        for name, values in exclusive_cpu_by_name.items()
    }
    dominant_wall = max(
        EXCLUSIVE_ACCOUNTING_NAMES,
        key=lambda name: exclusive_wall_medians[name],
    )
    dominant_cpu = max(
        EXCLUSIVE_ACCOUNTING_NAMES,
        key=lambda name: exclusive_cpu_medians[name],
    )
    detail_medians = {name: exclusive_wall_medians[name] for name in DETAIL_PHASES}
    dominant_detail = max(DETAIL_PHASES, key=lambda name: detail_medians[name])
    return {
        "wallMs": distribution(walls),
        "requestCpuMs": distribution(request_cpu),
        "allocatedBytes": distribution(allocated_bytes)
        if allocated_bytes
        else None,
        "maximumObservedResidentBytes": distribution(resident_bytes)
        if resident_bytes
        else None,
        "hostConditions": [
            {"condition": name, "samples": condition_counts[name]}
            for name in sorted(condition_counts)
        ],
        "medianCounters": [
            {
                "name": name,
                "value": round(statistics.median(counters_by_name[name]), 3),
            }
            for name in PROFILE_COUNTERS
        ],
        "medianExclusivePhases": [
            {
                "name": name,
                "wallDurationMs": exclusive_wall_medians[name],
                "cpuDurationMs": exclusive_cpu_medians[name],
            }
            for name in EXCLUSIVE_ACCOUNTING_NAMES
        ],
        "dominantExclusiveWallPhase": dominant_wall,
        "dominantExclusiveCpuPhase": dominant_cpu,
        "medianExclusiveDetails": [
            {"name": name, "durationMs": detail_medians[name]}
            for name in DETAIL_PHASES
        ],
        "dominantExclusiveDetail": dominant_detail,
    }


def load_average() -> float | None:
    try:
        value = float(os.getloadavg()[0])
    except (AttributeError, OSError):
        return None
    return round(value, 3)


def host_condition(loads: Sequence[float | None], logical_cpus: int) -> str:
    available = [load for load in loads if load is not None]
    if not available:
        return "load-unavailable"
    return (
        "representative"
        if max(available) / logical_cpus <= 0.5
        else "contended"
    )


def compiler_revision() -> dict[str, object]:
    """Identify the exact tracked compiler checkout without leaking its path."""

    head = subprocess.run(
        ["git", "rev-parse", "HEAD"],
        cwd=ROOT,
        check=True,
        capture_output=True,
        text=True,
        timeout=10,
    ).stdout.strip()
    tracked_diff = subprocess.run(
        ["git", "diff", "--binary", "HEAD"],
        cwd=ROOT,
        check=True,
        capture_output=True,
        timeout=20,
    ).stdout
    return {
        "gitHead": head,
        "dirtyTracked": bool(tracked_diff),
        "trackedDiffSha256": hashlib.sha256(tracked_diff).hexdigest(),
    }


def workload_arguments(output: Path, workload: str) -> tuple[str, ...]:
    """Resolve one admitted workload into native Haxe-server arguments."""

    if workload == "runtime-free":
        arguments = target_arguments(
            output,
            layout="split",
            reverse=False,
            report=False,
            times=True,
            phase_timing=True,
        )
    elif workload == "playable":
        platform_name = {
            "darwin": "macos",
            "linux": "linux",
            "win32": "windows",
        }.get(sys.platform)
        if platform_name is None:
            raise CompilerProfileFailure(
                f"playable profiling does not know platform {sys.platform!r}"
            )
        arguments = [
            "play.hxml",
            "-D",
            "hxc_runtime_diagnostics=off",
            "-D",
            f"raylib_platform_{platform_name}",
            "-D",
            "raylib_configuration_desktop",
            "-D",
            "reflaxe_c_phase_timing",
            "--times",
            "--custom-target",
            f"c={output}",
        ]
    else:
        raise CompilerProfileFailure(f"unknown compiler workload {workload!r}")
    return resolve_haxe_arguments(arguments, locale="C")


def profile(
    runs: int, *, workload: str = "runtime-free", transport: str = "both"
) -> dict[str, object]:
    if runs < 1:
        raise CompilerProfileFailure("compiler profiling requires at least one run")
    if workload not in PROFILE_WORKLOADS:
        raise CompilerProfileFailure(f"unknown compiler workload {workload!r}")
    if transport not in PROFILE_TRANSPORTS:
        raise CompilerProfileFailure(f"unknown compiler transport {transport!r}")
    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    clock = timer_clock(installation)
    revision = compiler_revision()
    logical_cpus = os.cpu_count() or 1
    starting_load = load_average()

    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-compiler-profile-") as temporary:
        root = Path(temporary)
        cold_output = root / "cold"
        warm_output = root / "warm"
        cold_resolved = workload_arguments(cold_output, workload)
        warm_resolved = workload_arguments(warm_output, workload)
        timeout = 360 if workload == "playable" else 180

        cold_samples: list[dict[str, object]] = []
        baseline: dict[str, bytes] | None = None
        if transport in ("both", "cold"):
            for ordinal in range(1, runs + 1):
                print(
                    f"caxecraft-compiler-profile: "
                    f"[{workload} cold {ordinal}/{runs}]",
                    flush=True,
                )
                sample = run_sample(
                    installation,
                    cold_resolved,
                    clock,
                    connection=None,
                    timeout=timeout,
                )
                artifacts = normal_artifacts(cold_output)
                if baseline is None:
                    baseline = artifacts
                else:
                    assert_artifacts_equal(
                        baseline,
                        artifacts,
                        f"{workload} cold compiler profile run {ordinal}",
                    )
                sample["ordinal"] = ordinal
                cold_samples.append(sample)

        warm_samples: list[dict[str, object]] = []
        if transport in ("both", "warm"):
            with haxe_compilation_server() as connection:
                print(
                    f"caxecraft-compiler-profile: [{workload} warm cache priming]",
                    flush=True,
                )
                run_sample(
                    installation,
                    warm_resolved,
                    clock,
                    connection=connection,
                    timeout=timeout,
                )
                warm_artifacts = normal_artifacts(warm_output)
                if baseline is None:
                    baseline = warm_artifacts
                else:
                    assert_artifacts_equal(
                        baseline,
                        warm_artifacts,
                        f"{workload} cold/warm compiler profile priming",
                    )
                for ordinal in range(1, runs + 1):
                    print(
                        f"caxecraft-compiler-profile: "
                        f"[{workload} warm {ordinal}/{runs}]",
                        flush=True,
                    )
                    sample = run_sample(
                        installation,
                        warm_resolved,
                        clock,
                        connection=connection,
                        timeout=timeout,
                    )
                    assert_artifacts_equal(
                        baseline,
                        normal_artifacts(warm_output),
                        f"{workload} warm compiler profile run {ordinal}",
                    )
                    sample["ordinal"] = ordinal
                    warm_samples.append(sample)

        if baseline is None:
            raise CompilerProfileFailure("compiler profile produced no baseline")

        ending_load = load_average()
        cold_report = (
            {
                "transport": "fresh-process",
                "primingRequests": 0,
                "summary": sample_summary(cold_samples),
                "samples": cold_samples,
            }
            if cold_samples
            else None
        )
        warm_report = (
            {
                "transport": "owned-loopback-server",
                "primingRequests": 1,
                "summary": sample_summary(warm_samples),
                "samples": warm_samples,
            }
            if warm_samples
            else None
        )
        return {
            "schemaVersion": 1,
            "suite": "caxecraft-compiler-profile",
            "haxe": {
                "version": installation.version,
                "sourceRevision": PINNED_HAXE_SOURCE_REVISION,
                "timerClock": clock.to_json(),
            },
            "haxeC": revision,
            "measurement": {
                "requestedRunsPerTransport": runs,
                "replication": "five-or-more" if runs >= 5 else "diagnostic-only",
                "transports": transport,
            },
            "workload": {
                "name": workload,
                "boundary": "haxe-source-to-generated-c",
                "nativeCompileIncluded": False,
                "layout": "split",
                "runtimePolicy": "none"
                if workload == "runtime-free"
                else "auto",
                "normalArtifactCount": len(baseline),
                "normalArtifactSha256": artifact_digest(baseline),
            },
            "host": {
                "platform": sys.platform,
                "machine": platform.machine(),
                "processor": platform.processor(),
                "logicalCpuCount": logical_cpus,
                "loadAverageOneMinuteStart": starting_load,
                "loadAverageOneMinuteEnd": ending_load,
                "condition": host_condition(
                    (starting_load, ending_load), logical_cpus
                ),
            },
            "cold": cold_report,
            "warm": warm_report,
            "parity": "byte-identical-normal-artifacts",
        }


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runs", type=int, default=5)
    parser.add_argument(
        "--workload",
        choices=PROFILE_WORKLOADS,
        default="runtime-free",
    )
    parser.add_argument(
        "--transport",
        choices=PROFILE_TRANSPORTS,
        default="both",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=None,
        help="write the path-free JSON report here",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    output = args.output or (
        CASE
        / "_build"
        / (
            "compiler-profile.json"
            if args.workload == "runtime-free"
            else f"compiler-profile-{args.workload}.json"
        )
    )
    try:
        report = profile(
            args.runs,
            workload=args.workload,
            transport=args.transport,
        )
        summaries: list[str] = []
        for name in ("cold", "warm"):
            result = report.get(name)
            if not isinstance(result, dict):
                continue
            summary = result.get("summary")
            wall = summary.get("wallMs") if isinstance(summary, dict) else None
            if not isinstance(summary, dict) or not isinstance(wall, dict):
                raise CompilerProfileFailure(f"{name} summary is malformed")
            summaries.append(
                f"{name} median {wall['median']}ms, "
                f"dominant {summary['dominantExclusiveWallPhase']}"
            )
        write_timing_report(output, report)
    except (
        CaxecraftFailure,
        CompilerProfileFailure,
        OSError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"caxecraft-compiler-profile: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "caxecraft-compiler-profile: OK: "
        + "; ".join(summaries)
        + f"; report {output}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

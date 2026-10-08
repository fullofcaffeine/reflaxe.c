"""Parse haxe.c phase progress without exposing profiling cost to the launcher."""

from __future__ import annotations

import json
from dataclasses import dataclass


PROGRESS_PREFIX = "HXC_PHASE_PROGRESS\t"


@dataclass(frozen=True)
class CompilerProgressRecord:
    """One validated phase marker emitted by the compiler macro process."""

    state: str
    phase: str | None
    profile: str | None
    build_mode: str | None
    status: str | None


def _text(value: str | bytes | None) -> str:
    """Normalize completed and timed-out subprocess output to readable text."""

    if value is None:
        return ""
    if isinstance(value, bytes):
        return value.decode("utf-8", errors="replace")
    return value


def _optional_text(document: dict[str, object], key: str) -> str | None:
    """Accept a nullable text field and reject any other progress shape."""

    value = document.get(key)
    if value is not None and not isinstance(value, str):
        raise ValueError(f"compiler progress field {key!r} must be text or null")
    return value


def partition_compiler_progress(
    value: str | bytes | None,
) -> tuple[str, tuple[CompilerProgressRecord, ...]]:
    """Remove valid progress lines while retaining ordinary diagnostics exactly."""

    kept: list[str] = []
    records: list[CompilerProgressRecord] = []
    for line in _text(value).splitlines():
        if not line.startswith(PROGRESS_PREFIX):
            kept.append(line)
            continue
        try:
            document = json.loads(line[len(PROGRESS_PREFIX) :])
            if not isinstance(document, dict) or document.get("schemaVersion") != 1:
                raise ValueError("unknown compiler progress schema")
            state = document.get("state")
            if not isinstance(state, str):
                raise ValueError("compiler progress state must be text")
            records.append(
                CompilerProgressRecord(
                    state=state,
                    phase=_optional_text(document, "phase"),
                    profile=_optional_text(document, "profile"),
                    build_mode=_optional_text(document, "buildMode"),
                    status=_optional_text(document, "status"),
                )
            )
        except (json.JSONDecodeError, ValueError):
            # A timeout can cut the final write in half. Keep malformed data in
            # the diagnostic instead of presenting it as trusted phase state.
            kept.append(line)
    return "\n".join(kept), tuple(records)


def summarize_compiler_progress(
    records: tuple[CompilerProgressRecord, ...],
) -> str | None:
    """Describe the last active phase from one ordered compiler request stream."""

    if not records:
        return None
    active_phases: list[str] = []
    profile: str | None = None
    build_mode: str | None = None
    for record in records:
        if record.profile is not None:
            profile = record.profile
        if record.build_mode is not None:
            build_mode = record.build_mode
        if record.state == "phase-started" and record.phase is not None:
            active_phases.append(record.phase)
        elif record.state == "phase-completed" and record.phase is not None:
            if active_phases and active_phases[-1] == record.phase:
                active_phases.pop()
        elif record.state in ("request-completed", "request-aborted"):
            active_phases.clear()

    scope = ""
    if profile is not None or build_mode is not None:
        scope = f" ({profile or 'unknown-profile'}/{build_mode or 'unknown-mode'})"
    latest = records[-1]
    if active_phases:
        return f"last active compiler phase: {active_phases[-1]}{scope}"
    if latest.state == "request-completed":
        return f"compiler request completed{scope}"
    if latest.state == "request-aborted":
        status = f": {latest.status}" if latest.status is not None else ""
        return f"compiler request aborted{status}{scope}"
    if latest.phase is not None:
        return f"last compiler phase status: {latest.phase} {latest.state}{scope}"
    return f"last compiler status: {latest.state}{scope}"


def compiler_process_detail(stdout: str | bytes | None, stderr: str | bytes | None) -> str:
    """Return concise compiler progress followed by the original non-progress output."""

    clean_stdout, stdout_records = partition_compiler_progress(stdout)
    clean_stderr, stderr_records = partition_compiler_progress(stderr)
    summary = summarize_compiler_progress((*stdout_records, *stderr_records))
    values: list[str] = []
    if summary is not None:
        values.append(f"compiler progress: {summary}")
    values.extend(value.strip() for value in (clean_stdout, clean_stderr) if value.strip())
    return "\n".join(values)


def compiler_timeout_suffix(
    stdout: str | bytes | None,
    stderr: str | bytes | None,
    *,
    missing_status: str | None = None,
) -> str:
    """Return one actionable timeout suffix when partial progress is available."""

    _, stdout_records = partition_compiler_progress(stdout)
    _, stderr_records = partition_compiler_progress(stderr)
    summary = summarize_compiler_progress((*stdout_records, *stderr_records))
    if summary is not None:
        return f"; {summary}"
    return "" if missing_status is None else f"; {missing_status}"

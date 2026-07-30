#!/usr/bin/env python3
"""Content-complete, fail-closed state for Caxecraft's unchanged-build fast path.

This module does not decide which files affect Haxe or native compilation.
`play.py` owns that product-specific list. Here we provide the smaller reusable
mechanism: turn reviewed files and directories into a deterministic inventory,
compare a new request with the one that produced the executable, validate every
reused output byte, and publish the state atomically.

The state is an optimization record, never build authority. Missing, malformed,
stale, or corrupt state produces a visible cache miss. The caller must then use
the ordinary cold build; this module never repairs output or chooses a
last-known-good binary after a failed requested build.
"""

from __future__ import annotations

import hashlib
import json
import os
import tempfile
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Mapping, Sequence


STATE_KIND = "caxecraft-play-build-state"
STATE_SCHEMA_VERSION = 2


class BuildStateFailure(RuntimeError):
    """A path or value cannot safely participate in reusable build state."""


@dataclass(frozen=True)
class InputPath:
    """One reviewed file or directory and its path-independent logical name."""

    logical_name: str
    path: Path


@dataclass(frozen=True)
class ExternalFile:
    """One native input outside the worktree that must still match exactly."""

    logical_name: str
    path: Path


@dataclass(frozen=True)
class ReuseDecision:
    """The result of validating a previous state against the requested build."""

    hit: bool
    reason: str


def sha256_file(path: Path) -> str:
    """Hash one regular file without loading a large compiler or library at once."""

    digest = hashlib.sha256()
    with path.open("rb") as source:
        while chunk := source.read(1024 * 1024):
            digest.update(chunk)
    return digest.hexdigest()


def _validated_logical(value: str, label: str) -> PurePosixPath:
    logical = PurePosixPath(value)
    if (
        logical.is_absolute()
        or logical.as_posix() != value
        or any(part in ("", ".", "..") for part in logical.parts)
    ):
        raise BuildStateFailure(f"{label} must be a normalized relative path")
    return logical


def _file_record(logical_name: str, path: Path) -> dict[str, object]:
    _validated_logical(logical_name, "build-state logical name")
    if path.is_symlink() or not path.is_file():
        raise BuildStateFailure(f"build input is missing or not a regular file: {logical_name}")
    return {
        "path": logical_name,
        "bytes": path.stat().st_size,
        "sha256": sha256_file(path),
    }


def inventory_inputs(inputs: Sequence[InputPath]) -> list[dict[str, object]]:
    """Build one canonical inventory while rejecting aliases and symlinks.

    Directories are recursive because a newly added Haxe module is as important
    as an edit to an existing one. Logical names, rather than checkout paths,
    enter the request digest so the same source bytes identify the same request
    in another worktree.
    """

    records: list[dict[str, object]] = []
    seen: set[str] = set()
    for item in inputs:
        root_name = _validated_logical(item.logical_name, "build input root").as_posix()
        path = item.path
        if path.is_symlink():
            raise BuildStateFailure(f"build input root is a symlink: {root_name}")
        if path.is_file():
            candidates = [(root_name, path)]
        elif path.is_dir():
            candidates = []
            for candidate in path.rglob("*"):
                relative = candidate.relative_to(path)
                logical = PurePosixPath(root_name, *relative.parts).as_posix()
                if candidate.is_symlink():
                    raise BuildStateFailure(f"build input contains a symlink: {logical}")
                if candidate.is_file():
                    candidates.append((logical, candidate))
        else:
            raise BuildStateFailure(f"build input root is missing: {root_name}")

        for logical, candidate in sorted(candidates, key=lambda pair: pair[0].encode("utf-8")):
            if logical in seen:
                raise BuildStateFailure(f"build input logical path is duplicated: {logical}")
            seen.add(logical)
            records.append(_file_record(logical, candidate))
    records.sort(key=lambda record: str(record["path"]).encode("utf-8"))
    return records


def canonical_digest(value: object) -> str:
    """Hash one JSON-compatible value with a single canonical byte spelling."""

    encoded = json.dumps(
        value,
        ensure_ascii=False,
        separators=(",", ":"),
        sort_keys=True,
    ).encode("utf-8")
    return hashlib.sha256(encoded).hexdigest()


def request_snapshot(
    *,
    configuration: Mapping[str, object],
    inputs: Sequence[InputPath],
    tools: Sequence[Mapping[str, object]],
    environment: Mapping[str, str],
) -> dict[str, object]:
    """Capture every reviewed input before Haxe or a native compiler runs."""

    body: dict[str, object] = {
        "schemaVersion": 1,
        "configuration": dict(configuration),
        "environment": dict(sorted(environment.items())),
        "tools": sorted(
            [dict(tool) for tool in tools],
            key=lambda tool: str(tool.get("name", "")).encode("utf-8"),
        ),
        "files": inventory_inputs(inputs),
    }
    return {**body, "sha256": canonical_digest(body)}


def external_file_snapshot(files: Sequence[ExternalFile]) -> list[dict[str, object]]:
    """Record native headers and libraries that live outside the output tree."""

    records: list[dict[str, object]] = []
    seen: set[str] = set()
    for item in sorted(files, key=lambda value: value.logical_name.encode("utf-8")):
        logical = _validated_logical(item.logical_name, "external native input").as_posix()
        if logical in seen:
            raise BuildStateFailure(f"external native input is duplicated: {logical}")
        seen.add(logical)
        record = _file_record(logical, item.path)
        record["source"] = str(item.path.resolve())
        records.append(record)
    return records


def output_snapshot(generated: Path, executable: Path) -> dict[str, object]:
    """Hash the generated project, staged assets, and executable.

    Runtime content is deliberately absent: `play.py` republishes the current
    authored files before every launch, so a map edit must not invalidate this
    executable-build record. Object files are also absent. The linked
    executable is the launchable product, while the native cache separately
    owns reusable object integrity and dependency files.
    """

    roots = (
        InputPath("generated", generated),
        InputPath("staged/assets", executable.parent / "assets"),
        InputPath("native/executable", executable),
    )
    files = inventory_inputs(roots)
    body = {"schemaVersion": 1, "files": files}
    return {**body, "sha256": canonical_digest(body)}


def build_state(
    *,
    request: Mapping[str, object],
    outputs: Mapping[str, object],
    external_native_files: Sequence[ExternalFile],
) -> dict[str, object]:
    """Create the complete state written only after a successful full build."""

    body: dict[str, object] = {
        "schemaVersion": STATE_SCHEMA_VERSION,
        "kind": STATE_KIND,
        "request": dict(request),
        "outputs": dict(outputs),
        "externalNativeFiles": external_file_snapshot(external_native_files),
    }
    return {**body, "sha256": canonical_digest(body)}


def atomic_write_state(path: Path, state: Mapping[str, object]) -> None:
    """Publish state with one same-directory rename after durable file flush."""

    path.parent.mkdir(parents=True, exist_ok=True)
    payload = json.dumps(
        dict(state),
        ensure_ascii=False,
        indent=2,
        sort_keys=True,
    ) + "\n"
    descriptor, temporary_name = tempfile.mkstemp(
        dir=path.parent,
        prefix=f".{path.name}.",
        suffix=".tmp",
    )
    temporary = Path(temporary_name)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="\n") as target:
            target.write(payload)
            target.flush()
            os.fsync(target.fileno())
        os.replace(temporary, path)
    except BaseException:
        temporary.unlink(missing_ok=True)
        raise


def load_state(path: Path) -> tuple[dict[str, object] | None, str | None]:
    """Decode state without converting corruption into a reusable hit."""

    if path.is_symlink():
        return None, "state file is a symlink"
    if not path.is_file():
        return None, "state file is missing"
    try:
        value: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError):
        return None, "state file is unreadable or malformed"
    if not isinstance(value, dict):
        return None, "state file is not a JSON object"
    return value, None


def _first_file_difference(
    expected: object,
    actual: object,
    *,
    family: str,
) -> str | None:
    if not isinstance(expected, list) or not isinstance(actual, list):
        return f"{family} inventory is malformed"
    expected_by_path = {
        record.get("path"): record
        for record in expected
        if isinstance(record, dict) and isinstance(record.get("path"), str)
    }
    actual_by_path = {
        record.get("path"): record
        for record in actual
        if isinstance(record, dict) and isinstance(record.get("path"), str)
    }
    if len(expected_by_path) != len(expected) or len(actual_by_path) != len(actual):
        return f"{family} inventory contains a malformed or duplicate path"
    paths = sorted(
        set(expected_by_path) | set(actual_by_path),
        key=lambda value: str(value).encode("utf-8"),
    )
    for path in paths:
        if path not in expected_by_path:
            return f"{family} added: {path}"
        if path not in actual_by_path:
            return f"{family} removed: {path}"
        if expected_by_path[path] != actual_by_path[path]:
            return f"{family} changed: {path}"
    return None


def _first_mapping_difference(expected: object, actual: object, *, family: str) -> str | None:
    if not isinstance(expected, dict) or not isinstance(actual, dict):
        return f"{family} is malformed"
    keys = sorted(set(expected) | set(actual), key=lambda value: value.encode("utf-8"))
    for key in keys:
        if key not in expected:
            return f"{family} added: {key}"
        if key not in actual:
            return f"{family} removed: {key}"
        if expected[key] != actual[key]:
            return f"{family} changed: {key}"
    return None


def _validate_state_digest(state: dict[str, object]) -> bool:
    digest = state.get("sha256")
    body = {key: value for key, value in state.items() if key != "sha256"}
    return isinstance(digest, str) and len(digest) == 64 and digest == canonical_digest(body)


def _current_external_snapshot(stored: object) -> list[dict[str, object]]:
    if not isinstance(stored, list):
        raise BuildStateFailure("external native input inventory is malformed")
    files: list[ExternalFile] = []
    for record in stored:
        if (
            not isinstance(record, dict)
            or not isinstance(record.get("path"), str)
            or not isinstance(record.get("source"), str)
        ):
            raise BuildStateFailure("external native input record is malformed")
        files.append(ExternalFile(str(record["path"]), Path(str(record["source"]))))
    return external_file_snapshot(files)


def validate_reuse(
    *,
    state_path: Path,
    current_request: Mapping[str, object],
    generated: Path,
    executable: Path,
) -> ReuseDecision:
    """Return a hit only after request, external input, and output parity."""

    state, load_error = load_state(state_path)
    if state is None:
        return ReuseDecision(False, load_error or "state file is unavailable")
    if state.get("schemaVersion") != STATE_SCHEMA_VERSION or state.get("kind") != STATE_KIND:
        return ReuseDecision(False, "state schema or kind is not supported")
    if not _validate_state_digest(state):
        return ReuseDecision(False, "state digest is invalid")

    stored_request = state.get("request")
    if not isinstance(stored_request, dict):
        return ReuseDecision(False, "request snapshot is malformed")
    difference = _first_mapping_difference(
        stored_request.get("configuration"),
        current_request.get("configuration"),
        family="configuration",
    )
    if difference is not None:
        return ReuseDecision(False, difference)
    difference = _first_mapping_difference(
        stored_request.get("environment"),
        current_request.get("environment"),
        family="environment",
    )
    if difference is not None:
        return ReuseDecision(False, difference)
    if stored_request.get("tools") != current_request.get("tools"):
        return ReuseDecision(False, "native or launcher tool identity changed")
    difference = _first_file_difference(
        stored_request.get("files"),
        current_request.get("files"),
        family="build input",
    )
    if difference is not None:
        return ReuseDecision(False, difference)
    if stored_request.get("sha256") != current_request.get("sha256"):
        return ReuseDecision(False, "request digest changed")

    try:
        current_external = _current_external_snapshot(state.get("externalNativeFiles"))
    except (OSError, BuildStateFailure):
        return ReuseDecision(False, "external native input is missing or malformed")
    difference = _first_file_difference(
        state.get("externalNativeFiles"),
        current_external,
        family="external native input",
    )
    if difference is not None:
        return ReuseDecision(False, difference)

    try:
        current_outputs = output_snapshot(generated, executable)
    except (OSError, BuildStateFailure):
        return ReuseDecision(False, "generated, staged, or executable output is missing")
    stored_outputs = state.get("outputs")
    if not isinstance(stored_outputs, dict):
        return ReuseDecision(False, "output snapshot is malformed")
    difference = _first_file_difference(
        stored_outputs.get("files"),
        current_outputs.get("files"),
        family="build output",
    )
    if difference is not None:
        return ReuseDecision(False, difference)
    if stored_outputs.get("sha256") != current_outputs.get("sha256"):
        return ReuseDecision(False, "output digest changed")
    return ReuseDecision(True, "request and every reusable output match")

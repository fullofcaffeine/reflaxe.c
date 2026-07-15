#!/usr/bin/env python3
"""Validate diagnostic allocation, typed-registry parity, and JSON records."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any, Iterable


ROOT = Path(__file__).resolve().parents[2]
CATALOG_RELATIVE = Path("docs/specs/diagnostics.json")
CATALOG_SCHEMA_RELATIVE = Path("docs/specs/diagnostics.schema.json")
EVENT_SCHEMA_RELATIVE = Path("docs/specs/diagnostic-event.schema.json")
HAXE_REGISTRY_RELATIVE = Path("src/reflaxe/c/CDiagnostic.hx")

ID_RE = re.compile(r"^HXC([0-9]{4})$")
ID_REFERENCE_RE = re.compile(r"\bHXC[0-9]{4}\b")
ENUM_ENTRY_RE = re.compile(r'^\s*var\s+[A-Za-z][A-Za-z0-9]*\s*=\s*"(HXC[0-9]{4})";', re.MULTILINE)
NAME_RE = re.compile(r"^[a-z0-9]+(?:-[a-z0-9]+)*$")

SEVERITIES = ("error", "warning", "info")
PROFILES = ("portable", "metal", "unresolved")
PHASES = (
    "bootstrap",
    "configuration",
    "lowering",
    "runtime-analysis",
    "abi",
    "interop",
    "target-contract",
    "internal",
)
KINDS = (
    "dependency-failure",
    "invalid-configuration",
    "compiler-capability",
    "unsupported-source",
    "runtime-policy",
    "runtime-selection",
    "invalid-abi",
    "unsafe-boundary",
    "invalid-target-contract",
    "internal-compiler-failure",
)
DIAGNOSTIC_KEYS = {
    "id",
    "name",
    "defaultSeverity",
    "allowedSeverities",
    "phase",
    "kind",
    "sourcePolicy",
    "idStability",
    "summary",
    "remediation",
}
EVENT_KEYS = {
    "schemaVersion",
    "id",
    "severity",
    "phase",
    "kind",
    "profile",
    "message",
    "remediation",
    "source",
    "context",
    "notes",
    "relatedLocations",
}
REFERENCE_SUFFIXES = {".c", ".h", ".hx", ".hxml", ".json", ".md", ".py", ".sh"}
REFERENCE_ROOTS = ("src", "std", "test", "scripts", "docs")


def load_json(path: Path, errors: list[str]) -> dict[str, Any]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        errors.append(f"cannot read {path}: {error}")
        return {}
    if not isinstance(value, dict):
        errors.append(f"{path} must contain a JSON object")
        return {}
    return value


def diagnostic_number(value: Any) -> int | None:
    if not isinstance(value, str):
        return None
    match = ID_RE.fullmatch(value)
    return int(match.group(1)) if match else None


def non_empty_string(value: Any) -> bool:
    return isinstance(value, str) and bool(value.strip())


def validate_catalog_document(
    catalog: dict[str, Any],
    *,
    enum_ids: Iterable[str] | None = None,
    referenced_ids: Iterable[str] | None = None,
) -> list[str]:
    errors: list[str] = []
    if catalog.get("$schema") != "diagnostics.schema.json":
        errors.append("diagnostic catalog must declare diagnostics.schema.json")
    if catalog.get("schemaVersion") != 2:
        errors.append("diagnostic catalog schemaVersion must be 2")
    if catalog.get("eventSchema") != "diagnostic-event.schema.json":
        errors.append("diagnostic catalog must point to diagnostic-event.schema.json")

    id_policy = catalog.get("idPolicy")
    expected_id_policy = {"stability", "wording", "retirement", "allocation"}
    if not isinstance(id_policy, dict) or set(id_policy) != expected_id_policy:
        errors.append("idPolicy must define stability, wording, retirement, and allocation")
    elif any(not non_empty_string(id_policy[field]) for field in expected_id_policy):
        errors.append("every idPolicy field must be non-empty")

    severity_policy = catalog.get("severityPolicy")
    if not isinstance(severity_policy, dict) or set(severity_policy) != {"levels", "escalation"}:
        errors.append("severityPolicy must define only levels and escalation")
    else:
        if severity_policy.get("levels") != list(SEVERITIES):
            errors.append("severityPolicy.levels must be error, warning, info in precedence order")
        if not non_empty_string(severity_policy.get("escalation")):
            errors.append("severityPolicy.escalation must be non-empty")

    source_policy = catalog.get("sourcePolicy")
    if not isinstance(source_policy, dict) or set(source_policy) != {"requirement", "pathForm", "coordinates"}:
        errors.append("sourcePolicy must define requirement, pathForm, and coordinates")
    elif any(not non_empty_string(value) for value in source_policy.values()):
        errors.append("every sourcePolicy field must be non-empty")

    profile_policy = catalog.get("profilePolicy")
    if not isinstance(profile_policy, dict) or set(profile_policy) != {"values", "unresolved"}:
        errors.append("profilePolicy must define values and unresolved")
    else:
        if profile_policy.get("values") != list(PROFILES):
            errors.append("profilePolicy.values must be portable, metal, unresolved")
        if not non_empty_string(profile_policy.get("unresolved")):
            errors.append("profilePolicy.unresolved must be non-empty")

    ranges = catalog.get("ranges")
    normalized_ranges: list[tuple[int, int, str, str]] = []
    if not isinstance(ranges, list):
        errors.append("diagnostic ranges must be an array")
        ranges = []
    for index, item in enumerate(ranges):
        label = f"ranges[{index}]"
        if not isinstance(item, dict) or set(item) != {"start", "end", "category", "status"}:
            errors.append(f"{label} must define only start, end, category, and status")
            continue
        start = diagnostic_number(item.get("start"))
        end = diagnostic_number(item.get("end"))
        if start is None or end is None or start > end:
            errors.append(f"{label} has an invalid diagnostic interval")
            continue
        if not non_empty_string(item.get("category")):
            errors.append(f"{label}.category must be non-empty")
        status = item.get("status")
        if status not in ("active", "reserved"):
            errors.append(f"{label}.status must be active or reserved")
        normalized_ranges.append((start, end, str(item.get("category", "")), str(status)))

    expected_start = 0
    for start, end, _category, _status in normalized_ranges:
        if start != expected_start:
            errors.append(f"diagnostic ranges must be contiguous; expected HXC{expected_start:04d}, found HXC{start:04d}")
        expected_start = end + 1
    if expected_start != 10000:
        errors.append("diagnostic ranges must cover HXC0000 through HXC9999 exactly once")

    diagnostics = catalog.get("diagnostics")
    if not isinstance(diagnostics, list) or not diagnostics:
        errors.append("diagnostics must be a non-empty array")
        diagnostics = []
    ids: list[str] = []
    names: list[str] = []
    catalog_by_id: dict[str, dict[str, Any]] = {}
    for index, item in enumerate(diagnostics):
        label = f"diagnostics[{index}]"
        if not isinstance(item, dict):
            errors.append(f"{label} must be an object")
            continue
        if set(item) != DIAGNOSTIC_KEYS:
            missing = sorted(DIAGNOSTIC_KEYS - set(item))
            extra = sorted(set(item) - DIAGNOSTIC_KEYS)
            errors.append(f"{label} fields drifted (missing={missing}, extra={extra})")
        diagnostic_id = item.get("id")
        number = diagnostic_number(diagnostic_id)
        if number is None:
            errors.append(f"{label}.id must match HXCdddd")
            continue
        ids.append(diagnostic_id)
        catalog_by_id[diagnostic_id] = item
        name = item.get("name")
        if not isinstance(name, str) or NAME_RE.fullmatch(name) is None:
            errors.append(f"{label}.name must be lowercase kebab-case")
        else:
            names.append(name)
        allowed = item.get("allowedSeverities")
        if not isinstance(allowed, list) or not allowed or len(set(allowed)) != len(allowed):
            errors.append(f"{label}.allowedSeverities must be a non-empty unique array")
            allowed = []
        elif any(value not in SEVERITIES for value in allowed):
            errors.append(f"{label}.allowedSeverities contains an unknown severity")
        default = item.get("defaultSeverity")
        if default not in SEVERITIES or default not in allowed:
            errors.append(f"{label}.defaultSeverity must be one of its allowed severities")
        if item.get("phase") not in PHASES:
            errors.append(f"{label}.phase is unknown")
        if item.get("kind") not in KINDS:
            errors.append(f"{label}.kind is unknown")
        if item.get("sourcePolicy") != "required":
            errors.append(f"{label}.sourcePolicy must be required")
        if item.get("idStability") != "stable":
            errors.append(f"{label}.idStability must be stable")
        for field in ("summary", "remediation"):
            if not non_empty_string(item.get(field)):
                errors.append(f"{label}.{field} must be non-empty")
        matching_ranges = [entry for entry in normalized_ranges if entry[0] <= number <= entry[1]]
        if len(matching_ranges) != 1:
            errors.append(f"{diagnostic_id} must belong to exactly one range")
        elif matching_ranges[0][3] != "active":
            errors.append(f"{diagnostic_id} cannot be allocated from a reserved range")

    if ids != sorted(ids):
        errors.append("diagnostics must be sorted by ID")
    if len(ids) != len(set(ids)):
        errors.append("diagnostic IDs must be unique")
    if len(names) != len(set(names)):
        errors.append("diagnostic names must be unique")

    unsupported = catalog_by_id.get("HXC1001")
    internal = catalog_by_id.get("HXC9000")
    if unsupported is None or unsupported.get("kind") != "unsupported-source" or unsupported.get("phase") != "lowering":
        errors.append("HXC1001 must remain a lowering/unsupported-source diagnostic")
    if internal is None or internal.get("kind") != "internal-compiler-failure" or internal.get("phase") != "internal":
        errors.append("HXC9000 must remain an internal/internal-compiler-failure diagnostic")
    for diagnostic_id, item in catalog_by_id.items():
        number = diagnostic_number(diagnostic_id)
        if item.get("kind") == "internal-compiler-failure" and (number is None or number < 9000):
            errors.append(f"{diagnostic_id} marks an internal failure outside the HXC9xxx range")
        if number is not None and number >= 9000 and item.get("kind") != "internal-compiler-failure":
            errors.append(f"{diagnostic_id} uses the internal range without internal-compiler-failure kind")

    runtime_selected = catalog_by_id.get("HXC2001")
    if runtime_selected is None or runtime_selected.get("defaultSeverity") != "info" or runtime_selected.get("allowedSeverities") != ["info", "warning"]:
        errors.append("HXC2001 must default to info and allow only info/warning escalation")

    if enum_ids is not None:
        enum_list = list(enum_ids)
        if len(enum_list) != len(set(enum_list)):
            errors.append("typed Haxe diagnostic IDs must be unique")
        if set(enum_list) != set(ids):
            errors.append(
                "typed Haxe diagnostic registry differs from catalog "
                f"(missing={sorted(set(ids) - set(enum_list))}, extra={sorted(set(enum_list) - set(ids))})"
            )
    if referenced_ids is not None:
        range_boundaries = {
            value
            for item in ranges
            if isinstance(item, dict)
            for value in (item.get("start"), item.get("end"))
            if isinstance(value, str)
        }
        unregistered = sorted(set(referenced_ids) - set(ids) - range_boundaries)
        if unregistered:
            errors.append("repository references unregistered diagnostic IDs: " + ", ".join(unregistered))

    return errors


def normalized_source_path(value: Any) -> bool:
    if not isinstance(value, str) or not value or value.startswith(("/", "~")) or "\\" in value or "\x00" in value:
        return False
    if re.match(r"^[A-Za-z]:", value):
        return False
    parts = value.split("/")
    return all(part not in ("", ".", "..") for part in parts) and all(ord(character) >= 0x20 and ord(character) != 0x7F for character in value)


def validate_source(source: Any, label: str, errors: list[str]) -> None:
    if not isinstance(source, dict) or set(source) != {"file", "start", "end"}:
        errors.append(f"{label} must define only file, start, and end")
        return
    if not normalized_source_path(source.get("file")):
        errors.append(f"{label}.file must be normalized and repository-relative")
    points: list[tuple[int, int]] = []
    for point_name in ("start", "end"):
        point = source.get(point_name)
        if not isinstance(point, dict) or set(point) != {"line", "column"}:
            errors.append(f"{label}.{point_name} must define line and column")
            continue
        line = point.get("line")
        column = point.get("column")
        if not isinstance(line, int) or isinstance(line, bool) or line < 1 or not isinstance(column, int) or isinstance(column, bool) or column < 1:
            errors.append(f"{label}.{point_name} coordinates must be positive integers")
            continue
        points.append((line, column))
    if len(points) == 2 and points[1] < points[0]:
        errors.append(f"{label} end must not precede start")


def validate_event(event: dict[str, Any], catalog: dict[str, Any]) -> list[str]:
    errors: list[str] = []
    if set(event) != EVENT_KEYS:
        errors.append(f"diagnostic event fields drifted (missing={sorted(EVENT_KEYS - set(event))}, extra={sorted(set(event) - EVENT_KEYS)})")
    if event.get("schemaVersion") != 1:
        errors.append("diagnostic event schemaVersion must be 1")
    diagnostics = catalog.get("diagnostics", [])
    catalog_by_id = {item.get("id"): item for item in diagnostics if isinstance(item, dict)}
    definition = catalog_by_id.get(event.get("id"))
    if definition is None:
        errors.append("diagnostic event ID is not registered")
    else:
        if event.get("severity") not in definition.get("allowedSeverities", []):
            errors.append("diagnostic event severity is not allowed by its catalog entry")
        for field in ("phase", "kind", "remediation"):
            if event.get(field) != definition.get(field):
                errors.append(f"diagnostic event {field} differs from its catalog entry")
    if event.get("profile") not in PROFILES:
        errors.append("diagnostic event profile is invalid")
    if not non_empty_string(event.get("message")):
        errors.append("diagnostic event message must be non-empty")
    if not isinstance(event.get("context"), str):
        errors.append("diagnostic event context must be a string")
    validate_source(event.get("source"), "diagnostic event source", errors)
    notes = event.get("notes")
    if not isinstance(notes, list) or any(not non_empty_string(note) for note in notes):
        errors.append("diagnostic event notes must be an array of non-empty strings")
    related = event.get("relatedLocations")
    if not isinstance(related, list):
        errors.append("diagnostic event relatedLocations must be an array")
    else:
        for index, location in enumerate(related):
            if not isinstance(location, dict) or set(location) != {"label", "source"}:
                errors.append(f"relatedLocations[{index}] must define label and source")
                continue
            if not non_empty_string(location.get("label")):
                errors.append(f"relatedLocations[{index}].label must be non-empty")
            validate_source(location.get("source"), f"relatedLocations[{index}].source", errors)
    return errors


def collect_enum_ids(root: Path, errors: list[str]) -> list[str]:
    path = root / HAXE_REGISTRY_RELATIVE
    try:
        source = path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        errors.append(f"cannot read typed diagnostic registry: {error}")
        return []
    return ENUM_ENTRY_RE.findall(source)


def candidate_reference_files(root: Path) -> Iterable[Path]:
    for relative_root in REFERENCE_ROOTS:
        base = root / relative_root
        if not base.exists():
            continue
        for path in base.rglob("*"):
            if path.is_file() and path.suffix in REFERENCE_SUFFIXES:
                yield path
    for name in ("README.md", "AGENTS.md"):
        path = root / name
        if path.is_file():
            yield path


def collect_referenced_ids(root: Path, errors: list[str]) -> set[str]:
    references: set[str] = set()
    for path in candidate_reference_files(root):
        try:
            references.update(ID_REFERENCE_RE.findall(path.read_text(encoding="utf-8")))
        except (OSError, UnicodeError) as error:
            errors.append(f"cannot scan {path.relative_to(root)}: {error}")
    return references


def check_raw_production_ids(root: Path, errors: list[str]) -> None:
    registry = root / HAXE_REGISTRY_RELATIVE
    for source_root in (root / "src", root / "std"):
        if not source_root.exists():
            continue
        for path in source_root.rglob("*.hx"):
            if path == registry:
                continue
            try:
                matches = ID_REFERENCE_RE.findall(path.read_text(encoding="utf-8"))
            except (OSError, UnicodeError) as error:
                errors.append(f"cannot scan {path.relative_to(root)}: {error}")
                continue
            if matches:
                errors.append(
                    f"{path.relative_to(root)} embeds raw diagnostic IDs {sorted(set(matches))}; use CDiagnosticId and CDiagnostic"
                )


def validate_schema_documents(root: Path, errors: list[str]) -> None:
    catalog_schema = load_json(root / CATALOG_SCHEMA_RELATIVE, errors)
    event_schema = load_json(root / EVENT_SCHEMA_RELATIVE, errors)
    if catalog_schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        errors.append("diagnostics.schema.json must use JSON Schema 2020-12")
    if catalog_schema.get("properties", {}).get("schemaVersion", {}).get("const") != 2:
        errors.append("diagnostics.schema.json must require catalog schemaVersion 2")
    if event_schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        errors.append("diagnostic-event.schema.json must use JSON Schema 2020-12")
    event_properties = event_schema.get("properties", {})
    if event_properties.get("schemaVersion", {}).get("const") != 1:
        errors.append("diagnostic-event.schema.json must require event schemaVersion 1")
    if event_properties.get("phase", {}).get("enum") != list(PHASES):
        errors.append("diagnostic-event.schema.json phase enum differs from policy")
    if event_properties.get("kind", {}).get("enum") != list(KINDS):
        errors.append("diagnostic-event.schema.json kind enum differs from policy")


def validate(root: Path = ROOT) -> list[str]:
    errors: list[str] = []
    catalog = load_json(root / CATALOG_RELATIVE, errors)
    validate_schema_documents(root, errors)
    enum_ids = collect_enum_ids(root, errors)
    referenced_ids = collect_referenced_ids(root, errors)
    errors.extend(validate_catalog_document(catalog, enum_ids=enum_ids, referenced_ids=referenced_ids))
    check_raw_production_ids(root, errors)
    return errors


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=ROOT)
    arguments = parser.parse_args()
    root = arguments.root.resolve()
    errors = validate(root)
    if errors:
        for error in errors:
            print(f"diagnostic-policy: ERROR: {error}", file=sys.stderr)
        return 1
    catalog = json.loads((root / CATALOG_RELATIVE).read_text(encoding="utf-8"))
    print(
        "diagnostic-policy: OK: "
        f"{len(catalog['diagnostics'])} typed IDs, 10 exhaustive ranges, catalog schema 2, event schema 1"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

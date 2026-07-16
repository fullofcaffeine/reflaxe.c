#!/usr/bin/env python3
"""Reject unreviewed untyped constructs in repository-owned Haxe."""

from __future__ import annotations

import argparse
import json
import re
import sys
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[2]
MANIFEST_RELATIVE = "docs/specs/typed-boundaries.json"
SCHEMA_RELATIVE = "docs/specs/typed-boundaries.schema.json"
MANIFEST_ID = "hxc-typed-boundaries-v1"

EXCLUDED_ROOTS = (
    ".beads",
    ".git",
    ".hxc",
    "node_modules",
    "runtime/third_party",
    "runtime/vendor",
    "third_party",
    "vendor",
)

CONSTRUCT_DESCRIPTIONS = {
    "any-type": "Haxe Any erases the static value contract.",
    "dynamic-type": "Haxe Dynamic defers member and conversion checks.",
    "json-parse": "haxe.Json.parse returns an untyped external value.",
    "open-record": "DynamicAccess is an open-ended string-keyed value carrier.",
    "private-access": "Private-access metadata bypasses the declared visibility contract.",
    "raw-code-injection": "Raw __c__ calls bypass typed IR and structural C AST lowering.",
    "reflect-api": "Reflect member access replaces a known structural type with runtime lookup.",
    "unchecked-cast": "The Haxe cast keyword can assert a type without validating the value shape.",
    "untyped-keyword": "The Haxe untyped keyword disables ordinary type checking.",
}

BOUNDARY_KINDS = (
    "compiler-preview",
    "external-json",
    "foreign-api",
    "language-semantics-fixture",
    "negative-test",
)

TEST_ONLY_BOUNDARIES = frozenset(
    ("language-semantics-fixture", "negative-test")
)

PATTERNS = {
    "any-type": re.compile(r"\bAny\b"),
    "dynamic-type": re.compile(r"\bDynamic\b"),
    "json-parse": re.compile(r"\bJson\s*\.\s*parse\s*\("),
    "open-record": re.compile(r"\b(?:haxe\s*\.\s*)?DynamicAccess\b"),
    "private-access": re.compile(r"@:\s*(?:privateAccess|access)\b"),
    "raw-code-injection": re.compile(r"\b__c__\s*\("),
    "reflect-api": re.compile(r"\bReflect\s*\."),
    "unchecked-cast": re.compile(r"\bcast\b"),
    "untyped-keyword": re.compile(r"\buntyped\b"),
}


@dataclass(frozen=True, order=True)
class Occurrence:
    path: str
    line: int
    column: int
    construct: str
    source_line: str

    @property
    def key(self) -> tuple[str, int, int, str]:
        return (self.path, self.line, self.column, self.construct)


@dataclass(frozen=True, order=True)
class Allowance:
    path: str
    line: int
    column: int
    construct: str
    boundary_kind: str
    symbol: str
    typed_model: str
    source_line: str
    justification: str
    validation: str
    test_evidence: tuple[str, ...]

    @property
    def key(self) -> tuple[str, int, int, str]:
        return (self.path, self.line, self.column, self.construct)


def read_json(path: Path, errors: list[str]) -> object:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        errors.append(f"cannot read {path.name}: {error}")
        return {}


def object_record(
    value: object, label: str, required: frozenset[str], errors: list[str]
) -> dict[str, object]:
    if not isinstance(value, dict):
        errors.append(f"{label} must be an object")
        return {}
    record = {str(key): item for key, item in value.items()}
    actual = frozenset(record)
    if actual != required:
        missing = sorted(required - actual)
        unknown = sorted(actual - required)
        if missing:
            errors.append(f"{label} is missing fields: {', '.join(missing)}")
        if unknown:
            errors.append(f"{label} has unknown fields: {', '.join(unknown)}")
    return record


def non_empty_string(value: object, label: str, errors: list[str]) -> str:
    if not isinstance(value, str) or not value.strip():
        errors.append(f"{label} must be a non-empty string")
        return ""
    return value


def positive_integer(value: object, label: str, errors: list[str]) -> int:
    if not isinstance(value, int) or isinstance(value, bool) or value < 1:
        errors.append(f"{label} must be a positive integer")
        return 0
    return value


def string_list(value: object, label: str, errors: list[str]) -> tuple[str, ...]:
    if not isinstance(value, list) or not value:
        errors.append(f"{label} must be a non-empty string array")
        return ()
    result: list[str] = []
    for index, item in enumerate(value):
        text = non_empty_string(item, f"{label}[{index}]", errors)
        if text:
            result.append(text)
    if len(set(result)) != len(result):
        errors.append(f"{label} must not contain duplicates")
    return tuple(result)


def normalized_repository_path(value: object, label: str, errors: list[str]) -> str:
    text = non_empty_string(value, label, errors)
    if not text:
        return ""
    path = PurePosixPath(text)
    if (
        path.is_absolute()
        or "\\" in text
        or any(part in ("", ".", "..") for part in path.parts)
    ):
        errors.append(f"{label} must be a normalized repository-relative path")
        return ""
    return text


def validate_schema(root: Path, errors: list[str]) -> None:
    value = read_json(root / SCHEMA_RELATIVE, errors)
    if not isinstance(value, dict):
        errors.append(f"{SCHEMA_RELATIVE} must contain an object")
        return
    definitions = value.get("$defs")
    if not isinstance(definitions, dict):
        errors.append(f"{SCHEMA_RELATIVE} must define $defs")
        return
    construct = definitions.get("constructId")
    boundary = definitions.get("boundaryKind")
    construct_values = construct.get("enum") if isinstance(construct, dict) else None
    boundary_values = boundary.get("enum") if isinstance(boundary, dict) else None
    if construct_values != sorted(CONSTRUCT_DESCRIPTIONS):
        errors.append("typed-boundary schema construct IDs drifted from the scanner")
    if boundary_values != list(BOUNDARY_KINDS):
        errors.append("typed-boundary schema boundary kinds drifted from the scanner")


def parse_manifest(root: Path, errors: list[str]) -> tuple[Allowance, ...]:
    value = read_json(root / MANIFEST_RELATIVE, errors)
    manifest = object_record(
        value,
        MANIFEST_RELATIVE,
        frozenset(
            (
                "$schema",
                "schemaVersion",
                "manifestId",
                "scope",
                "constructs",
                "allowlist",
            )
        ),
        errors,
    )
    if manifest.get("$schema") != "typed-boundaries.schema.json":
        errors.append("typed-boundary manifest must reference its checked-in schema")
    if manifest.get("schemaVersion") != 1:
        errors.append("typed-boundary manifest schemaVersion must be 1")
    if manifest.get("manifestId") != MANIFEST_ID:
        errors.append(f"typed-boundary manifestId must be {MANIFEST_ID}")

    scope = object_record(
        manifest.get("scope"),
        "typed-boundary scope",
        frozenset(("files", "excludedRoots", "policy")),
        errors,
    )
    non_empty_string(scope.get("files"), "typed-boundary scope.files", errors)
    non_empty_string(scope.get("policy"), "typed-boundary scope.policy", errors)
    excluded = scope.get("excludedRoots")
    if excluded != list(EXCLUDED_ROOTS):
        errors.append(
            "typed-boundary excludedRoots must match the narrow reviewed third-party/cache set"
        )

    construct_values = manifest.get("constructs")
    if not isinstance(construct_values, list):
        errors.append("typed-boundary constructs must be an array")
        construct_values = []
    parsed_constructs: list[tuple[str, str]] = []
    for index, item in enumerate(construct_values):
        record = object_record(
            item,
            f"typed-boundary constructs[{index}]",
            frozenset(("id", "description")),
            errors,
        )
        identifier = non_empty_string(
            record.get("id"), f"typed-boundary constructs[{index}].id", errors
        )
        description = non_empty_string(
            record.get("description"),
            f"typed-boundary constructs[{index}].description",
            errors,
        )
        parsed_constructs.append((identifier, description))
    expected_constructs = sorted(CONSTRUCT_DESCRIPTIONS.items())
    if parsed_constructs != expected_constructs:
        errors.append(
            "typed-boundary construct inventory must exactly match the scanner in ID order"
        )

    raw_allowlist = manifest.get("allowlist")
    if not isinstance(raw_allowlist, list):
        errors.append("typed-boundary allowlist must be an array")
        raw_allowlist = []
    allowances: list[Allowance] = []
    allowance_fields = frozenset(
        (
            "path",
            "line",
            "column",
            "construct",
            "boundaryKind",
            "symbol",
            "typedModel",
            "sourceLine",
            "justification",
            "validation",
            "testEvidence",
        )
    )
    for index, item in enumerate(raw_allowlist):
        label = f"typed-boundary allowlist[{index}]"
        record = object_record(item, label, allowance_fields, errors)
        path = normalized_repository_path(record.get("path"), f"{label}.path", errors)
        line = positive_integer(record.get("line"), f"{label}.line", errors)
        column = positive_integer(record.get("column"), f"{label}.column", errors)
        construct = non_empty_string(
            record.get("construct"), f"{label}.construct", errors
        )
        if construct not in CONSTRUCT_DESCRIPTIONS:
            errors.append(f"{label}.construct is not scanned: {construct}")
        boundary_kind = non_empty_string(
            record.get("boundaryKind"), f"{label}.boundaryKind", errors
        )
        if boundary_kind not in BOUNDARY_KINDS:
            errors.append(f"{label}.boundaryKind is not admitted: {boundary_kind}")
        symbol = non_empty_string(record.get("symbol"), f"{label}.symbol", errors)
        typed_model = non_empty_string(
            record.get("typedModel"), f"{label}.typedModel", errors
        )
        source_line = non_empty_string(
            record.get("sourceLine"), f"{label}.sourceLine", errors
        )
        justification = non_empty_string(
            record.get("justification"), f"{label}.justification", errors
        )
        validation = non_empty_string(
            record.get("validation"), f"{label}.validation", errors
        )
        test_evidence = string_list(
            record.get("testEvidence"), f"{label}.testEvidence", errors
        )

        if boundary_kind in TEST_ONLY_BOUNDARIES and not path.startswith("test/"):
            errors.append(f"{label} uses a test-only boundary outside test/")
        if path.startswith(("src/", "std/")) and boundary_kind in TEST_ONLY_BOUNDARIES:
            errors.append(f"{label} cannot excuse production Haxe as a test boundary")
        if construct == "json-parse" and boundary_kind != "external-json":
            errors.append(f"{label} must classify Json.parse as external-json")
        if boundary_kind == "external-json" and construct != "json-parse":
            errors.append(f"{label} external-json allowance must identify Json.parse")

        for evidence_index, evidence in enumerate(test_evidence):
            evidence_path = normalized_repository_path(
                evidence, f"{label}.testEvidence[{evidence_index}]", errors
            )
            if evidence_path and not evidence_path.startswith("test/"):
                errors.append(f"{label}.testEvidence must remain under test/")
            if evidence_path and not (root / evidence_path).is_file():
                errors.append(f"{label}.testEvidence path is missing: {evidence_path}")

        allowances.append(
            Allowance(
                path,
                line,
                column,
                construct,
                boundary_kind,
                symbol,
                typed_model,
                source_line,
                justification,
                validation,
                test_evidence,
            )
        )

    if allowances != sorted(allowances, key=lambda item: item.key):
        errors.append("typed-boundary allowlist must be sorted by path, line, column, construct")
    keys = [allowance.key for allowance in allowances]
    if len(keys) != len(set(keys)):
        errors.append("typed-boundary allowlist contains duplicate source occurrences")
    return tuple(allowances)


def mask_non_code(source: str) -> str:
    """Preserve code coordinates while masking comments, literals, and regex bodies."""

    masked = ["\n" if character == "\n" else " " for character in source]
    # Frames are (mode, interpolation brace depth). Nested strings/comments
    # return to the code frame beneath them.
    frames: list[tuple[str, int]] = [("code", 0)]
    block_depth = 0
    index = 0
    while index < len(source):
        mode, interpolation_depth = frames[-1]
        character = source[index]
        following = source[index + 1] if index + 1 < len(source) else ""

        if mode == "code":
            if interpolation_depth > 0 and character == "{":
                frames[-1] = (mode, interpolation_depth + 1)
                index += 1
                continue
            if interpolation_depth > 0 and character == "}":
                if interpolation_depth == 1:
                    frames.pop()
                else:
                    frames[-1] = (mode, interpolation_depth - 1)
                index += 1
                continue
            if character == "/" and following == "/":
                frames.append(("line-comment", 0))
                index += 2
                continue
            if character == "/" and following == "*":
                block_depth = 1
                frames.append(("block-comment", 0))
                index += 2
                continue
            if character == '"':
                frames.append(("double-string", 0))
                index += 1
                continue
            if character == "'":
                frames.append(("single-string", 0))
                index += 1
                continue
            if character == "~" and following == "/":
                frames.append(("regex", 0))
                index += 2
                continue
            masked[index] = character
            index += 1
            continue

        if mode == "line-comment":
            if character == "\n":
                frames.pop()
            index += 1
            continue

        if mode == "block-comment":
            if character == "/" and following == "*":
                block_depth += 1
                index += 2
                continue
            if character == "*" and following == "/":
                block_depth -= 1
                index += 2
                if block_depth == 0:
                    frames.pop()
                continue
            index += 1
            continue

        if mode in ("double-string", "single-string"):
            if character == "\\":
                index += 2
                continue
            if mode == "single-string" and character == "$" and following == "{":
                frames.append(("code", 1))
                index += 2
                continue
            closing = '"' if mode == "double-string" else "'"
            if character == closing:
                frames.pop()
            index += 1
            continue

        if mode == "regex":
            if character == "\\":
                index += 2
                continue
            if character == "/":
                frames.pop()
            index += 1
            continue

        raise AssertionError(f"unknown Haxe lexical state: {mode}")
    return "".join(masked)


def is_excluded(path: str) -> bool:
    return any(path == root or path.startswith(root + "/") for root in EXCLUDED_ROOTS)


def owned_haxe_files(root: Path) -> tuple[Path, ...]:
    files: list[Path] = []
    for path in root.rglob("*.hx"):
        if not path.is_file():
            continue
        relative = path.relative_to(root).as_posix()
        if not is_excluded(relative):
            files.append(path)
    return tuple(sorted(files, key=lambda path: path.relative_to(root).as_posix()))


def scan(root: Path, errors: list[str]) -> tuple[tuple[Path, ...], tuple[Occurrence, ...]]:
    files = owned_haxe_files(root)
    occurrences: list[Occurrence] = []
    for path in files:
        relative = path.relative_to(root).as_posix()
        try:
            source = path.read_text(encoding="utf-8")
        except (OSError, UnicodeError) as error:
            errors.append(f"cannot read owned Haxe source {relative}: {error}")
            continue
        code = mask_non_code(source)
        lines = source.splitlines()
        for construct, pattern in PATTERNS.items():
            for match in pattern.finditer(code):
                line = code.count("\n", 0, match.start()) + 1
                line_start = code.rfind("\n", 0, match.start()) + 1
                column = match.start() - line_start + 1
                source_line = lines[line - 1].strip() if line <= len(lines) else ""
                occurrences.append(
                    Occurrence(relative, line, column, construct, source_line)
                )
    occurrences.sort(key=lambda item: item.key)
    return files, tuple(occurrences)


def validate(root: Path) -> tuple[list[str], int, int]:
    errors: list[str] = []
    validate_schema(root, errors)
    allowances = parse_manifest(root, errors)
    files, occurrences = scan(root, errors)

    allowed_by_key = {allowance.key: allowance for allowance in allowances}
    found_keys: set[tuple[str, int, int, str]] = set()
    for occurrence in occurrences:
        allowance = allowed_by_key.get(occurrence.key)
        if allowance is None:
            errors.append(
                "unapproved typed-boundary construct "
                f"{occurrence.construct} at {occurrence.path}:{occurrence.line}:"
                f"{occurrence.column}: {occurrence.source_line}"
            )
            continue
        found_keys.add(occurrence.key)
        if allowance.source_line != occurrence.source_line:
            errors.append(
                f"typed-boundary sourceLine drifted at {occurrence.path}:"
                f"{occurrence.line}: expected `{allowance.source_line}`, "
                f"found `{occurrence.source_line}`"
            )

    for allowance in allowances:
        if allowance.key not in found_keys:
            errors.append(
                "stale typed-boundary allowance at "
                f"{allowance.path}:{allowance.line}:{allowance.column} "
                f"for {allowance.construct}"
            )
        if allowance.path and not (root / allowance.path).is_file():
            errors.append(f"typed-boundary allowance path is missing: {allowance.path}")

    return errors, len(files), len(occurrences)


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(
        description="Validate the narrow allowlist for untyped Haxe boundaries."
    )
    result.add_argument("--root", type=Path, default=ROOT)
    result.add_argument("--quiet", action="store_true")
    return result


def main(arguments: list[str] | None = None) -> int:
    options = parser().parse_args(arguments)
    root = options.root.resolve()
    errors, file_count, occurrence_count = validate(root)
    if errors:
        for error in errors:
            print(f"typed-boundaries: ERROR: {error}", file=sys.stderr)
        return 1
    if not options.quiet:
        print(
            "typed-boundaries: OK: "
            f"{file_count} repository-owned Haxe files, "
            f"{occurrence_count} narrow reviewed occurrences, and zero "
            "unapproved Dynamic/Any/Reflect/untyped/open-record/cast/raw seams"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

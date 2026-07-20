#!/usr/bin/env python3
"""Deterministic, semantic-neutral maintainability evidence for generated C.

This module deliberately does not decide whether generated C is correct.  Native,
Eval, IR, and other semantic gates remain authoritative.  The analyzer measures a
closed set of reviewability properties after those gates have selected an
artifact corpus.

Callers provide typed artifact ownership, source/function mappings, the symbol
ledger entries that explain compiler spellings, and the exact authority for each
admitted ``goto``.  The analyzer never guesses those semantic facts from names.
"""

from __future__ import annotations

from dataclasses import dataclass, fields
from enum import Enum
from functools import lru_cache
import hashlib
import json
from pathlib import Path
import re
from typing import Mapping, Sequence


REPORT_SCHEMA_VERSION = 1
POLICY_SCHEMA_VERSION = 1
ALGORITHM_ID = "hxc-generated-c-maintainability-v1"
CORRECTNESS_AUTHORITY = "semantic-native-eval-gates-are-independent-and-overriding"


class MaintainabilityError(ValueError):
    """Raised when an input or report violates the closed analyzer contract."""


class OwnerKind(str, Enum):
    SOURCE_MODULE = "source-module"
    COMPILER_SUPPORT = "compiler-support"
    COMPILER_ENTRY = "compiler-entry"
    AMALGAMATION = "amalgamation"
    SYNTHETIC_FIXTURE = "synthetic-fixture"
    HANDWRITTEN_BASELINE = "handwritten-baseline"


class GotoCategory(str, Enum):
    IRREDUCIBLE_CFG = "irreducible-cfg"
    LOOP_BREAK_THROUGH_SWITCH = "loop-break-through-switch"


@dataclass(frozen=True)
class SourceSpan:
    path: str
    start_line: int
    start_column: int
    end_line: int
    end_column: int


@dataclass(frozen=True)
class ArtifactOwner:
    kind: OwnerKind
    module_path: str | None = None
    source: SourceSpan | None = None


@dataclass(frozen=True)
class FunctionSourceMapping:
    function_id: str
    c_name: str
    owner: ArtifactOwner
    source: SourceSpan | None = None


@dataclass(frozen=True)
class GeneratedCArtifact:
    path: str
    content: bytes
    owner: ArtifactOwner
    functions: tuple[FunctionSourceMapping, ...] = ()


@dataclass(frozen=True)
class SymbolLedgerEntry:
    c_name: str
    source_symbol: str
    collision_resolved: bool = False
    escape_reasons: tuple[str, ...] = ()


@dataclass(frozen=True)
class GotoAuthority:
    path: str
    function_id: str
    target_label: str
    occurrence: int
    category: GotoCategory


@dataclass(frozen=True)
class MetricLimits:
    max_identifier_bytes: int
    max_file_lines: int
    max_function_lines: int
    max_branch_count: int
    max_nesting_depth: int
    max_temporary_declarations: int
    max_temporary_references: int
    max_temporary_declarations_per_k_function_lines: int
    max_function_temporary_declarations_per_k_lines: int
    max_unexplained_role_encoded_identifiers: int
    max_unexplained_byte_escaped_identifiers: int
    max_unexplained_digest_identifiers: int
    max_unexplained_hash_suffix_identifiers: int
    min_owner_mapped_artifacts_basis_points: int
    min_function_identity_mapped_basis_points: int
    min_source_span_mapped_functions_basis_points: int
    min_line_directive_count: int


@dataclass(frozen=True)
class FileLimitOverride:
    path: str
    max_file_lines: int
    reason: str


@dataclass(frozen=True)
class FunctionLimitOverride:
    function_id: str
    max_function_lines: int | None = None
    max_branch_count: int | None = None
    max_nesting_depth: int | None = None
    max_function_temporary_declarations_per_k_lines: int | None = None
    reason: str = ""


@dataclass(frozen=True)
class CorpusPolicy:
    policy_id: str
    corpus_id: str
    layout: str
    limits: MetricLimits
    goto_limits: tuple[tuple[GotoCategory, int], ...]
    file_overrides: tuple[FileLimitOverride, ...] = ()
    function_overrides: tuple[FunctionLimitOverride, ...] = ()


@dataclass(frozen=True)
class _Token:
    kind: str
    text: str
    line: int
    column: int
    end_line: int
    end_column: int


@dataclass(frozen=True)
class _FunctionRegion:
    c_name: str
    start_index: int
    open_index: int
    close_index: int
    start_line: int
    end_line: int


@dataclass(frozen=True)
class _LineDirective:
    physical_line: int
    mapped_line: int
    mapped_path: str


_C_IDENTIFIER = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
_TEMP_IDENTIFIER = re.compile(r"^hxc_tmp_[A-Za-z0-9_]+$")
_BYTE_ESCAPE = re.compile(r"(?:_x|zx)[0-9A-Fa-f]{2}")
_SEMANTIC_DIGEST = re.compile(r"[0-9a-f]{32,}")
_HASH_SUFFIX = re.compile(r"_h[0-9a-f]{12,64}$")
_ROLE_ENCODING = re.compile(r"^hxc_(?:type|field|method|local|temp|spec)_")

_TYPE_LEADERS = {
    "_Atomic",
    "_Bool",
    "auto",
    "bool",
    "char",
    "const",
    "double",
    "enum",
    "extern",
    "float",
    "int",
    "int8_t",
    "int16_t",
    "int32_t",
    "int64_t",
    "intptr_t",
    "long",
    "ptrdiff_t",
    "register",
    "restrict",
    "short",
    "signed",
    "size_t",
    "static",
    "struct",
    "uint8_t",
    "uint16_t",
    "uint32_t",
    "uint64_t",
    "uintptr_t",
    "union",
    "unsigned",
    "volatile",
    "void",
}

def _utf8_key(value: str) -> bytes:
    return value.encode("utf-8")


def _normalize_relative_path(value: str, field_name: str) -> str:
    if not isinstance(value, str) or not value:
        raise MaintainabilityError(f"{field_name} must be a non-empty string")
    try:
        value.encode("utf-8", "strict")
    except UnicodeError as error:
        raise MaintainabilityError(f"{field_name} is not valid UTF-8") from error
    if "\x00" in value or "\\" in value or value.startswith("/"):
        raise MaintainabilityError(f"{field_name} must be a normalized relative path: {value!r}")
    if re.match(r"^[A-Za-z]:", value):
        raise MaintainabilityError(f"{field_name} must not contain a drive prefix: {value!r}")
    parts = value.split("/")
    if any(part in ("", ".", "..") for part in parts):
        raise MaintainabilityError(f"{field_name} contains a non-normal segment: {value!r}")
    return value


def _require_positive(value: int, field_name: str, *, allow_zero: bool = False) -> int:
    if isinstance(value, bool) or not isinstance(value, int):
        raise MaintainabilityError(f"{field_name} must be an integer")
    minimum = 0 if allow_zero else 1
    if value < minimum:
        raise MaintainabilityError(f"{field_name} must be >= {minimum}")
    return value


def _validate_source_span(span: SourceSpan, field_name: str) -> None:
    _normalize_relative_path(span.path, f"{field_name}.path")
    for name in ("start_line", "start_column", "end_line", "end_column"):
        _require_positive(getattr(span, name), f"{field_name}.{name}")
    if (span.end_line, span.end_column) < (span.start_line, span.start_column):
        raise MaintainabilityError(f"{field_name} ends before it starts")


def _validate_owner(owner: ArtifactOwner, field_name: str) -> None:
    if not isinstance(owner.kind, OwnerKind):
        raise MaintainabilityError(f"{field_name}.kind must be OwnerKind")
    if owner.module_path is not None:
        _normalize_relative_path(owner.module_path, f"{field_name}.module_path")
    if owner.source is not None:
        _validate_source_span(owner.source, f"{field_name}.source")
    if owner.kind is OwnerKind.SOURCE_MODULE and owner.module_path is None:
        raise MaintainabilityError(f"{field_name} source-module owner needs module_path")


def _validate_c_identifier(value: str, field_name: str) -> None:
    if not isinstance(value, str) or _C_IDENTIFIER.fullmatch(value) is None:
        raise MaintainabilityError(f"{field_name} is not a C identifier: {value!r}")


def _lex_c(text: str, path: str) -> list[_Token]:
    """Lex enough ISO C to measure structure without inspecting comments/literals."""

    tokens: list[_Token] = []
    index = 0
    line = 1
    column = 1
    length = len(text)

    def advance(character: str) -> None:
        nonlocal line, column
        if character == "\n":
            line += 1
            column = 1
        else:
            column += 1

    def add(kind: str, start: int, start_line: int, start_column: int) -> None:
        tokens.append(_Token(kind, text[start:index], start_line, start_column, line, column))

    while index < length:
        character = text[index]
        if character.isspace():
            advance(character)
            index += 1
            continue

        if text.startswith("//", index):
            index += 2
            column += 2
            while index < length and text[index] != "\n":
                advance(text[index])
                index += 1
            continue

        if text.startswith("/*", index):
            start_line, start_column = line, column
            index += 2
            column += 2
            while index < length and not text.startswith("*/", index):
                advance(text[index])
                index += 1
            if index >= length:
                raise MaintainabilityError(
                    f"{path}:{start_line}:{start_column}: unterminated block comment"
                )
            index += 2
            column += 2
            continue

        literal_prefix = ""
        for candidate in ("u8", "u", "U", "L", ""):
            quote_index = index + len(candidate)
            if text.startswith(candidate, index) and quote_index < length and text[quote_index] in "\"'":
                literal_prefix = candidate
                break
        quote_index = index + len(literal_prefix)
        if quote_index < length and text[quote_index] in "\"'":
            start, start_line, start_column = index, line, column
            quote = text[quote_index]
            for _ in literal_prefix:
                advance(text[index])
                index += 1
            advance(text[index])
            index += 1
            terminated = False
            while index < length:
                current = text[index]
                if current == quote:
                    advance(current)
                    index += 1
                    terminated = True
                    break
                if current == "\n":
                    raise MaintainabilityError(
                        f"{path}:{start_line}:{start_column}: newline in C literal"
                    )
                if current == "\\":
                    advance(current)
                    index += 1
                    if index >= length:
                        break
                    escaped = text[index]
                    advance(escaped)
                    index += 1
                    if escaped == "\n":
                        continue
                    continue
                advance(current)
                index += 1
            if not terminated:
                raise MaintainabilityError(
                    f"{path}:{start_line}:{start_column}: unterminated C literal"
                )
            add("literal", start, start_line, start_column)
            continue

        if character.isalpha() or character == "_":
            start, start_line, start_column = index, line, column
            while index < length and (text[index].isalnum() or text[index] == "_"):
                advance(text[index])
                index += 1
            add("identifier", start, start_line, start_column)
            continue

        if character.isdigit():
            start, start_line, start_column = index, line, column
            while index < length and (text[index].isalnum() or text[index] in "._+-"):
                # Stop signs unless they are immediately part of an exponent.
                if text[index] in "+-" and index > start and text[index - 1] not in "eEpP":
                    break
                advance(text[index])
                index += 1
            add("number", start, start_line, start_column)
            continue

        start, start_line, start_column = index, line, column
        matched = False
        for punctuation in (
            "<<=", ">>=", "...", "->", "++", "--", "&&", "||", "==", "!=",
            "<=", ">=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<", ">>",
        ):
            if text.startswith(punctuation, index):
                for _ in punctuation:
                    advance(text[index])
                    index += 1
                matched = True
                break
        if not matched:
            advance(character)
            index += 1
        add("punctuation", start, start_line, start_column)

    return tokens


def _decode_c_path_literal(token: _Token, artifact_path: str) -> str:
    raw = token.text
    if raw.startswith("u8"):
        raw = raw[2:]
    elif raw.startswith(("u", "U", "L")):
        raw = raw[1:]
    if len(raw) < 2 or raw[0] != '"' or raw[-1] != '"':
        raise MaintainabilityError(
            f"{artifact_path}:{token.line}:{token.column}: #line path must be a string literal"
        )
    body = raw[1:-1]
    result: list[str] = []
    index = 0
    simple = {"\\": "\\", '"': '"', "'": "'", "?": "?", "a": "\a", "b": "\b", "f": "\f", "n": "\n", "r": "\r", "t": "\t", "v": "\v"}
    while index < len(body):
        if body[index] != "\\":
            result.append(body[index])
            index += 1
            continue
        index += 1
        if index >= len(body):
            raise MaintainabilityError(f"{artifact_path}:{token.line}:{token.column}: bad #line escape")
        escaped = body[index]
        if escaped in simple:
            result.append(simple[escaped])
            index += 1
            continue
        if escaped in "01234567":
            match = re.match(r"[0-7]{1,3}", body[index:])
            assert match is not None
            result.append(chr(int(match.group(0), 8)))
            index += len(match.group(0))
            continue
        if escaped == "x":
            match = re.match(r"x([0-9A-Fa-f]+)", body[index:])
            if match is None:
                raise MaintainabilityError(f"{artifact_path}:{token.line}:{token.column}: bad #line hex escape")
            result.append(chr(int(match.group(1), 16)))
            index += len(match.group(0))
            continue
        raise MaintainabilityError(
            f"{artifact_path}:{token.line}:{token.column}: unsupported #line escape \\{escaped}"
        )
    return _normalize_relative_path("".join(result), "#line mapped path")


def _line_directives(tokens: Sequence[_Token], artifact_path: str) -> list[_LineDirective]:
    result: list[_LineDirective] = []
    index = 0
    current_path = artifact_path
    while index < len(tokens):
        token = tokens[index]
        if token.text != "#":
            index += 1
            continue
        same_line: list[_Token] = []
        cursor = index + 1
        while cursor < len(tokens) and tokens[cursor].line == token.line:
            same_line.append(tokens[cursor])
            cursor += 1
        if not same_line:
            index = cursor
            continue
        number_index = 0
        if same_line[0].kind == "identifier" and same_line[0].text == "line":
            number_index = 1
        if number_index >= len(same_line) or same_line[number_index].kind != "number":
            index = cursor
            continue
        number_text = same_line[number_index].text
        if not number_text.isdigit() or int(number_text) < 1:
            raise MaintainabilityError(
                f"{artifact_path}:{token.line}:{token.column}: invalid #line number {number_text!r}"
            )
        path_index = number_index + 1
        if path_index < len(same_line) and same_line[path_index].kind == "literal":
            current_path = _decode_c_path_literal(same_line[path_index], artifact_path)
        result.append(_LineDirective(token.line, int(number_text), current_path))
        index = cursor
    return result


def _mapped_coordinate(
    physical_line: int, directives: Sequence[_LineDirective]
) -> tuple[str, int] | None:
    active: _LineDirective | None = None
    for directive in directives:
        if directive.physical_line >= physical_line:
            break
        active = directive
    if active is None:
        return None
    mapped_line = active.mapped_line + physical_line - active.physical_line - 1
    return active.mapped_path, mapped_line


def _find_functions(tokens: Sequence[_Token]) -> list[_FunctionRegion]:
    regions: list[_FunctionRegion] = []
    depth = 0
    declaration_start = 0
    index = 0
    while index < len(tokens):
        token = tokens[index]
        if token.text == "#" and depth == 0:
            directive_line = token.line
            index += 1
            while index < len(tokens) and tokens[index].line == directive_line:
                index += 1
            declaration_start = index
            continue
        if token.text == "{" and depth == 0:
            if index > 0 and tokens[index - 1].text == ")":
                close_paren = index - 1
                paren_depth = 1
                cursor = close_paren - 1
                while cursor >= declaration_start and paren_depth:
                    if tokens[cursor].text == ")":
                        paren_depth += 1
                    elif tokens[cursor].text == "(":
                        paren_depth -= 1
                    cursor -= 1
                name_index = cursor
                if paren_depth == 0 and name_index >= declaration_start:
                    name = tokens[name_index]
                    if name.kind == "identifier" and name.text not in ("if", "for", "while", "switch"):
                        brace_depth = 1
                        close_index = index + 1
                        while close_index < len(tokens) and brace_depth:
                            if tokens[close_index].text == "{":
                                brace_depth += 1
                            elif tokens[close_index].text == "}":
                                brace_depth -= 1
                            close_index += 1
                        if brace_depth:
                            raise MaintainabilityError(
                                f"unterminated function body for {name.text} at {name.line}:{name.column}"
                            )
                        actual_close = close_index - 1
                        regions.append(
                            _FunctionRegion(
                                name.text,
                                declaration_start,
                                index,
                                actual_close,
                                tokens[declaration_start].line,
                                tokens[actual_close].line,
                            )
                        )
                        index = close_index
                        declaration_start = index
                        continue
            depth += 1
        elif token.text == "}" and depth > 0:
            depth -= 1
            if depth == 0:
                declaration_start = index + 1
        elif token.text == ";" and depth == 0:
            declaration_start = index + 1
        index += 1
    return regions


def _temporary_declaration_indices(tokens: Sequence[_Token], start: int, end: int) -> set[int]:
    declarations: set[int] = set()
    for index in range(start, end):
        token = tokens[index]
        if token.kind != "identifier" or _TEMP_IDENTIFIER.fullmatch(token.text) is None:
            continue
        line_start = index
        while line_start > start and tokens[line_start - 1].line == token.line:
            line_start -= 1
        prefix = list(tokens[line_start:index])
        if not prefix:
            continue
        # Generated declarations begin with a type/qualifier token.  A cast such
        # as ``(void)hxc_tmp_n0`` is a reference, not a second declaration.
        if prefix[0].kind != "identifier":
            continue
        if any(item.text == "(" for item in prefix) and prefix[-1].text != "*":
            continue
        if any(item.text == "=" for item in prefix):
            continue
        previous = prefix[-1].text
        if previous in (".", "->"):
            continue
        first_identifier = next((item.text for item in prefix if item.kind == "identifier"), None)
        has_type_leader = first_identifier in _TYPE_LEADERS
        has_compiler_type = first_identifier is not None and first_identifier.startswith("hxc_")
        has_pointer_type = "*" in (item.text for item in prefix) and first_identifier is not None
        if has_type_leader or has_compiler_type or has_pointer_type:
            declarations.add(index)
    return declarations


def _source_span_json(span: SourceSpan | None) -> dict[str, object] | None:
    if span is None:
        return None
    return {
        "path": span.path,
        "startLine": span.start_line,
        "startColumn": span.start_column,
        "endLine": span.end_line,
        "endColumn": span.end_column,
    }


def _owner_json(owner: ArtifactOwner) -> dict[str, object]:
    return {
        "kind": owner.kind.value,
        "modulePath": owner.module_path,
        "source": _source_span_json(owner.source),
    }


def _limits_json(limits: MetricLimits) -> dict[str, int]:
    return {
        "maxIdentifierBytes": limits.max_identifier_bytes,
        "maxFileLines": limits.max_file_lines,
        "maxFunctionLines": limits.max_function_lines,
        "maxBranchCount": limits.max_branch_count,
        "maxNestingDepth": limits.max_nesting_depth,
        "maxTemporaryDeclarations": limits.max_temporary_declarations,
        "maxTemporaryReferences": limits.max_temporary_references,
        "maxTemporaryDeclarationsPerKFunctionLines": limits.max_temporary_declarations_per_k_function_lines,
        "maxFunctionTemporaryDeclarationsPerKLines": limits.max_function_temporary_declarations_per_k_lines,
        "maxUnexplainedRoleEncodedIdentifiers": limits.max_unexplained_role_encoded_identifiers,
        "maxUnexplainedByteEscapedIdentifiers": limits.max_unexplained_byte_escaped_identifiers,
        "maxUnexplainedDigestIdentifiers": limits.max_unexplained_digest_identifiers,
        "maxUnexplainedHashSuffixIdentifiers": limits.max_unexplained_hash_suffix_identifiers,
        "minOwnerMappedArtifactsBasisPoints": limits.min_owner_mapped_artifacts_basis_points,
        "minFunctionIdentityMappedBasisPoints": limits.min_function_identity_mapped_basis_points,
        "minSourceSpanMappedFunctionsBasisPoints": limits.min_source_span_mapped_functions_basis_points,
        "minLineDirectiveCount": limits.min_line_directive_count,
    }


def _policy_json(policy: CorpusPolicy) -> dict[str, object]:
    return {
        "policyId": policy.policy_id,
        "corpusId": policy.corpus_id,
        "layout": policy.layout,
        "limits": _limits_json(policy.limits),
        "gotoLimits": [
            {"category": category.value, "maxOccurrences": maximum}
            for category, maximum in policy.goto_limits
        ],
        "fileOverrides": [
            {"path": override.path, "maxFileLines": override.max_file_lines, "reason": override.reason}
            for override in policy.file_overrides
        ],
        "functionOverrides": [
            {
                "functionId": override.function_id,
                "maxFunctionLines": override.max_function_lines,
                "maxBranchCount": override.max_branch_count,
                "maxNestingDepth": override.max_nesting_depth,
                "maxFunctionTemporaryDeclarationsPerKLines": override.max_function_temporary_declarations_per_k_lines,
                "reason": override.reason,
            }
            for override in policy.function_overrides
        ],
    }


def _canonical_sha256(value: object) -> str:
    encoded = json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":")).encode("utf-8")
    return hashlib.sha256(encoded).hexdigest()


def _rate_per_k(count: int, lines: int) -> int:
    if count == 0:
        return 0
    if lines <= 0:
        return count * 1000
    return (count * 1000 + lines - 1) // lines


def _basis_points(numerator: int, denominator: int) -> int:
    return 10000 if denominator == 0 else numerator * 10000 // denominator


def _coordinate(path: str, line: int, column: int, function_id: str | None = None) -> dict[str, object]:
    return {"path": path, "line": line, "column": column, "functionId": function_id}


def _violation(
    rule: str,
    message: str,
    path: str,
    line: int,
    column: int,
    *,
    function_id: str | None = None,
    actual: int | str | None = None,
    limit: int | str | None = None,
) -> dict[str, object]:
    return {
        "rule": rule,
        "message": message,
        "actual": actual,
        "limit": limit,
        "coordinate": _coordinate(path, line, column, function_id),
    }


def _violation_key(item: Mapping[str, object]) -> tuple[bytes, int, int, bytes, bytes]:
    coordinate = item["coordinate"]
    assert isinstance(coordinate, Mapping)
    function_id = coordinate.get("functionId")
    return (
        _utf8_key(str(coordinate["path"])),
        int(coordinate["line"]),
        int(coordinate["column"]),
        _utf8_key(str(item["rule"])),
        _utf8_key("" if function_id is None else str(function_id)),
    )


def _validate_policy(policy: CorpusPolicy) -> None:
    if not policy.policy_id or not policy.corpus_id or not policy.layout:
        raise MaintainabilityError("policy id, corpus id, and layout must be non-empty")
    limit_values = [getattr(policy.limits, field.name) for field in fields(MetricLimits)]
    for field, value in zip(fields(MetricLimits), limit_values):
        _require_positive(value, f"limits.{field.name}", allow_zero=True)
    for basis_name in (
        "min_owner_mapped_artifacts_basis_points",
        "min_function_identity_mapped_basis_points",
        "min_source_span_mapped_functions_basis_points",
    ):
        if getattr(policy.limits, basis_name) > 10000:
            raise MaintainabilityError(f"limits.{basis_name} must be <= 10000")

    goto_values = [(category.value, maximum) for category, maximum in policy.goto_limits]
    if any(not isinstance(category, GotoCategory) for category, _ in policy.goto_limits):
        raise MaintainabilityError("goto_limits categories must be GotoCategory values")
    for category, maximum in policy.goto_limits:
        _require_positive(maximum, f"goto_limits.{category.value}", allow_zero=True)
    if goto_values != sorted(goto_values, key=lambda pair: _utf8_key(pair[0])):
        raise MaintainabilityError("goto_limits must be UTF-8 sorted")
    if len({category for category, _ in policy.goto_limits}) != len(policy.goto_limits):
        raise MaintainabilityError("goto_limits contains duplicate categories")
    if {category for category, _ in policy.goto_limits} != set(GotoCategory):
        raise MaintainabilityError("goto_limits must name every admitted goto category")

    file_paths: list[str] = []
    for override in policy.file_overrides:
        file_paths.append(_normalize_relative_path(override.path, "file override path"))
        _require_positive(override.max_file_lines, "file override max_file_lines")
        if not override.reason:
            raise MaintainabilityError("file override reason must be non-empty")
    if file_paths != sorted(file_paths, key=_utf8_key) or len(set(file_paths)) != len(file_paths):
        raise MaintainabilityError("file_overrides must be unique and UTF-8 sorted")

    function_ids: list[str] = []
    for override in policy.function_overrides:
        if not override.function_id:
            raise MaintainabilityError("function override id must be non-empty")
        function_ids.append(override.function_id)
        if not override.reason:
            raise MaintainabilityError("function override reason must be non-empty")
        for name in (
            "max_function_lines",
            "max_branch_count",
            "max_nesting_depth",
            "max_function_temporary_declarations_per_k_lines",
        ):
            value = getattr(override, name)
            if value is not None:
                _require_positive(value, f"function override {name}", allow_zero=True)
    if function_ids != sorted(function_ids, key=_utf8_key) or len(set(function_ids)) != len(function_ids):
        raise MaintainabilityError("function_overrides must be unique and UTF-8 sorted")


def analyze_generated_c(
    *,
    corpus_id: str,
    layout: str,
    artifacts: Sequence[GeneratedCArtifact],
    symbols: Sequence[SymbolLedgerEntry] = (),
    goto_authorities: Sequence[GotoAuthority] = (),
    policy: CorpusPolicy,
) -> dict[str, object]:
    """Analyze a complete generated-C corpus and return a closed JSON value."""

    _validate_policy(policy)
    if corpus_id != policy.corpus_id or layout != policy.layout:
        raise MaintainabilityError(
            f"policy selects {policy.corpus_id}/{policy.layout}, not {corpus_id}/{layout}"
        )
    if not artifacts:
        raise MaintainabilityError("artifacts must not be empty")

    normalized_artifacts: list[GeneratedCArtifact] = []
    seen_paths: set[str] = set()
    for artifact in artifacts:
        path = _normalize_relative_path(artifact.path, "artifact.path")
        if path in seen_paths:
            raise MaintainabilityError(f"duplicate artifact path: {path}")
        seen_paths.add(path)
        if not isinstance(artifact.content, bytes):
            raise MaintainabilityError(f"artifact {path} content must be bytes")
        try:
            text = artifact.content.decode("utf-8", "strict")
        except UnicodeDecodeError as error:
            raise MaintainabilityError(f"artifact {path} is not valid UTF-8") from error
        if "\r" in text or "\x00" in text:
            raise MaintainabilityError(f"artifact {path} must use LF text without NUL")
        _validate_owner(artifact.owner, f"artifact {path} owner")
        normalized_artifacts.append(artifact)
    normalized_artifacts.sort(key=lambda item: _utf8_key(item.path))

    ledger: dict[str, list[SymbolLedgerEntry]] = {}
    for symbol in symbols:
        _validate_c_identifier(symbol.c_name, "symbol.c_name")
        if not symbol.source_symbol:
            raise MaintainabilityError("symbol.source_symbol must be non-empty")
        reasons = tuple(sorted(symbol.escape_reasons, key=_utf8_key))
        if reasons != symbol.escape_reasons or len(set(reasons)) != len(reasons):
            raise MaintainabilityError("symbol.escape_reasons must be unique and UTF-8 sorted")
        ledger.setdefault(symbol.c_name, []).append(symbol)
    for entries in ledger.values():
        entries.sort(key=lambda item: _utf8_key(item.source_symbol))
        if len({entry.source_symbol for entry in entries}) != len(entries):
            raise MaintainabilityError(f"duplicate symbol ledger entry for {entries[0].c_name}")

    authorities: dict[tuple[str, str, str, int], GotoCategory] = {}
    for authority in goto_authorities:
        path = _normalize_relative_path(authority.path, "goto authority path")
        if not authority.function_id:
            raise MaintainabilityError("goto authority function_id must be non-empty")
        _validate_c_identifier(authority.target_label, "goto authority target_label")
        _require_positive(authority.occurrence, "goto authority occurrence")
        if not isinstance(authority.category, GotoCategory):
            raise MaintainabilityError("goto authority category must be GotoCategory")
        key = (path, authority.function_id, authority.target_label, authority.occurrence)
        if key in authorities:
            raise MaintainabilityError(f"duplicate goto authority: {key}")
        authorities[key] = authority.category

    policy_value = _policy_json(policy)
    policy_report = dict(policy_value)
    policy_report["sha256"] = _canonical_sha256(policy_value)

    file_reports: list[dict[str, object]] = []
    identifier_first: dict[str, tuple[str, _Token]] = {}
    all_function_reports: list[dict[str, object]] = []
    goto_rows: list[dict[str, object]] = []
    violations: list[dict[str, object]] = []
    used_authorities: set[tuple[str, str, str, int]] = set()
    used_file_overrides: set[str] = set()
    used_function_overrides: set[str] = set()
    total_physical_lines = 0
    total_code_lines = 0
    total_line_directives = 0
    all_mapped_paths: set[str] = set()
    total_temp_declarations = 0
    total_temp_references = 0
    total_branch_count = 0
    total_function_code_lines = 0
    function_identity_mapped_count = 0
    source_span_mapped_function_count = 0
    line_mapped_function_count = 0
    owner_mapped_artifact_count = 0
    first_temp_declaration: tuple[str, _Token, str] | None = None
    first_temp_reference: tuple[str, _Token, str] | None = None

    file_override_map = {override.path: override for override in policy.file_overrides}
    function_override_map = {override.function_id: override for override in policy.function_overrides}

    for artifact in normalized_artifacts:
        path = artifact.path
        text = artifact.content.decode("utf-8")
        tokens = _lex_c(text, path)
        directives = _line_directives(tokens, path)
        regions = _find_functions(tokens)
        physical_lines = len(text.splitlines())
        code_lines = len({token.line for token in tokens})
        total_physical_lines += physical_lines
        total_code_lines += code_lines
        total_line_directives += len(directives)
        mapped_paths = sorted({directive.mapped_path for directive in directives}, key=_utf8_key)
        all_mapped_paths.update(mapped_paths)
        owner_mapped_artifact_count += 1

        mappings: dict[str, FunctionSourceMapping] = {}
        function_ids: set[str] = set()
        for mapping in artifact.functions:
            if not mapping.function_id:
                raise MaintainabilityError(f"artifact {path} has empty function_id")
            _validate_c_identifier(mapping.c_name, f"artifact {path} function c_name")
            _validate_owner(mapping.owner, f"artifact {path} function {mapping.function_id} owner")
            if mapping.source is not None:
                _validate_source_span(mapping.source, f"artifact {path} function {mapping.function_id} source")
            if mapping.c_name in mappings:
                raise MaintainabilityError(f"artifact {path} duplicates function C name {mapping.c_name}")
            if mapping.function_id in function_ids:
                raise MaintainabilityError(f"artifact {path} duplicates function id {mapping.function_id}")
            mappings[mapping.c_name] = mapping
            function_ids.add(mapping.function_id)

        discovered_names: set[str] = set()
        file_temp_declarations = 0
        file_temp_references = 0
        file_function_reports: list[dict[str, object]] = []

        for token in tokens:
            if token.kind == "identifier" and token.text not in identifier_first:
                identifier_first[token.text] = (path, token)

        for region in regions:
            discovered_names.add(region.c_name)
            mapping = mappings.get(region.c_name)
            function_id = mapping.function_id if mapping is not None else f"c:{path}:{region.c_name}"
            function_owner = mapping.owner if mapping is not None else artifact.owner
            source = mapping.source if mapping is not None else function_owner.source
            if mapping is not None:
                function_identity_mapped_count += 1
            if source is not None:
                source_span_mapped_function_count += 1

            body_start = region.open_index + 1
            body_end = region.close_index
            body_tokens = tokens[body_start:body_end]
            declaration_indices = _temporary_declaration_indices(tokens, body_start, body_end)
            temp_declaration_tokens = [
                tokens[index] for index in sorted(declaration_indices)
            ]
            temp_reference_tokens = [
                token
                for offset, token in enumerate(body_tokens)
                if token.kind == "identifier"
                and _TEMP_IDENTIFIER.fullmatch(token.text) is not None
                and body_start + offset not in declaration_indices
            ]
            temp_declarations = len(temp_declaration_tokens)
            temp_references = len(temp_reference_tokens)
            if first_temp_declaration is None and temp_declaration_tokens:
                first_temp_declaration = (
                    path,
                    temp_declaration_tokens[0],
                    function_id,
                )
            if first_temp_reference is None and temp_reference_tokens:
                first_temp_reference = (path, temp_reference_tokens[0], function_id)
            function_code_lines = len({token.line for token in tokens[region.start_index : region.close_index + 1]})
            body_code_lines = len({token.line for token in body_tokens})
            total_function_code_lines += function_code_lines
            total_temp_declarations += temp_declarations
            total_temp_references += temp_references
            file_temp_declarations += temp_declarations
            file_temp_references += temp_references

            branch_count = sum(
                1
                for token in body_tokens
                if (token.kind == "identifier" and token.text in ("if", "for", "while", "do", "switch", "case", "default"))
                or token.text == "?"
            )
            total_branch_count += branch_count
            nesting = 0
            max_nesting = 0
            for token in body_tokens:
                if token.text == "{":
                    nesting += 1
                    max_nesting = max(max_nesting, nesting)
                elif token.text == "}":
                    nesting = max(0, nesting - 1)

            mapped_body_coordinates = {
                mapped
                for line in {token.line for token in body_tokens}
                if (mapped := _mapped_coordinate(line, directives)) is not None
            }
            if mapped_body_coordinates:
                line_mapped_function_count += 1
            function_mapped_paths = sorted({mapped[0] for mapped in mapped_body_coordinates}, key=_utf8_key)
            function_line_count = region.end_line - region.start_line + 1
            temp_density = _rate_per_k(temp_declarations, function_code_lines)
            function_report = {
                "functionId": function_id,
                "cName": region.c_name,
                "identityMapped": mapping is not None,
                "owner": _owner_json(function_owner),
                "source": _source_span_json(source),
                "startLine": region.start_line,
                "endLine": region.end_line,
                "physicalLineCount": function_line_count,
                "codeLineCount": function_code_lines,
                "bodyCodeLineCount": body_code_lines,
                "mappedBodyLineCount": len(mapped_body_coordinates),
                "mappedPaths": function_mapped_paths,
                "branchCount": branch_count,
                "maxNestingDepth": max_nesting,
                "temporaryDeclarationCount": temp_declarations,
                "temporaryReferenceCount": temp_references,
                "temporaryDeclarationsPerKCodeLines": temp_density,
            }
            file_function_reports.append(function_report)
            all_function_reports.append({"path": path, **function_report})

            override = function_override_map.get(function_id)
            if override is not None:
                used_function_overrides.add(function_id)
            max_function_lines = (
                override.max_function_lines
                if override is not None and override.max_function_lines is not None
                else policy.limits.max_function_lines
            )
            max_branch_count = (
                override.max_branch_count
                if override is not None and override.max_branch_count is not None
                else policy.limits.max_branch_count
            )
            max_nesting_depth = (
                override.max_nesting_depth
                if override is not None and override.max_nesting_depth is not None
                else policy.limits.max_nesting_depth
            )
            max_temp_density = (
                override.max_function_temporary_declarations_per_k_lines
                if override is not None
                and override.max_function_temporary_declarations_per_k_lines is not None
                else policy.limits.max_function_temporary_declarations_per_k_lines
            )
            for rule, actual, limit, label in (
                ("function-lines", function_line_count, max_function_lines, "function physical lines"),
                ("function-branches", branch_count, max_branch_count, "function branch count"),
                ("function-nesting", max_nesting, max_nesting_depth, "function nesting depth"),
                ("function-temporary-density", temp_density, max_temp_density, "function temporary declarations per 1000 code lines"),
            ):
                if actual > limit:
                    violations.append(
                        _violation(
                            rule,
                            f"{label} exceeds the reviewed policy",
                            path,
                            region.start_line,
                            tokens[region.start_index].column,
                            function_id=function_id,
                            actual=actual,
                            limit=limit,
                        )
                    )

            labels: dict[str, list[_Token]] = {}
            goto_tokens: list[tuple[_Token, str]] = []
            for local_index, token in enumerate(body_tokens):
                if token.kind == "identifier" and token.text == "goto":
                    cursor = local_index + 1
                    if cursor < len(body_tokens) and body_tokens[cursor].kind == "identifier":
                        goto_tokens.append((token, body_tokens[cursor].text))
                if (
                    token.kind == "identifier"
                    and local_index + 1 < len(body_tokens)
                    and body_tokens[local_index + 1].text == ":"
                    and (local_index == 0 or body_tokens[local_index - 1].text not in ("case", "default", "?"))
                ):
                    labels.setdefault(token.text, []).append(token)

            ordinal_by_target: dict[str, int] = {}
            for goto_token, target in goto_tokens:
                ordinal = ordinal_by_target.get(target, 0) + 1
                ordinal_by_target[target] = ordinal
                authority_key = (path, function_id, target, ordinal)
                category = authorities.get(authority_key)
                if category is not None:
                    used_authorities.add(authority_key)
                label_tokens = labels.get(target, [])
                goto_row = {
                    "path": path,
                    "line": goto_token.line,
                    "column": goto_token.column,
                    "functionId": function_id,
                    "targetLabel": target,
                    "occurrence": ordinal,
                    "category": None if category is None else category.value,
                    "labelLine": label_tokens[0].line if len(label_tokens) == 1 else None,
                    "labelColumn": label_tokens[0].column if len(label_tokens) == 1 else None,
                }
                goto_rows.append(goto_row)
                if category is None:
                    violations.append(
                        _violation(
                            "goto-unauthorized",
                            "generated goto has no exact typed authority",
                            path,
                            goto_token.line,
                            goto_token.column,
                            function_id=function_id,
                            actual=target,
                            limit="typed-authority-required",
                        )
                    )
                if len(label_tokens) != 1:
                    violations.append(
                        _violation(
                            "goto-label-resolution",
                            "goto target must resolve to exactly one label in its function",
                            path,
                            goto_token.line,
                            goto_token.column,
                            function_id=function_id,
                            actual=len(label_tokens),
                            limit=1,
                        )
                    )

        for c_name, mapping in mappings.items():
            if c_name not in discovered_names:
                violations.append(
                    _violation(
                        "function-mapping-unused",
                        "caller-supplied function mapping did not match a C definition",
                        path,
                        1,
                        1,
                        function_id=mapping.function_id,
                        actual=c_name,
                        limit="defined-function",
                    )
                )

        file_function_reports.sort(key=lambda row: (int(row["startLine"]), _utf8_key(str(row["functionId"]))))
        line_directive_rows = [
            {
                "physicalLine": directive.physical_line,
                "mappedLine": directive.mapped_line,
                "mappedPath": directive.mapped_path,
            }
            for directive in directives
        ]
        identifiers = [token for token in tokens if token.kind == "identifier"]
        max_identifier = None
        if identifiers:
            longest = min(
                identifiers,
                key=lambda token: (-len(token.text.encode("utf-8")), token.line, token.column, _utf8_key(token.text)),
            )
            max_identifier = {
                "name": longest.text,
                "bytes": len(longest.text.encode("utf-8")),
                "line": longest.line,
                "column": longest.column,
            }
        file_report = {
            "path": path,
            "sha256": hashlib.sha256(artifact.content).hexdigest(),
            "owner": _owner_json(artifact.owner),
            "physicalLineCount": physical_lines,
            "codeLineCount": code_lines,
            "lineDirectiveCount": len(directives),
            "mappedPathCount": len(mapped_paths),
            "mappedPaths": mapped_paths,
            "lineDirectives": line_directive_rows,
            "identifierCount": len(identifiers),
            "maxIdentifier": max_identifier,
            "temporaryDeclarationCount": file_temp_declarations,
            "temporaryReferenceCount": file_temp_references,
            "functions": file_function_reports,
        }
        file_reports.append(file_report)

        file_override = file_override_map.get(path)
        if file_override is not None:
            used_file_overrides.add(path)
        max_file_lines = file_override.max_file_lines if file_override is not None else policy.limits.max_file_lines
        if physical_lines > max_file_lines:
            violations.append(
                _violation(
                    "file-lines",
                    "file physical lines exceed the reviewed policy",
                    path,
                    1,
                    1,
                    actual=physical_lines,
                    limit=max_file_lines,
                )
            )

    identifier_findings: list[dict[str, object]] = []
    category_unexplained = {"compiler-role": 0, "byte-escape": 0, "semantic-digest": 0, "hash-suffix": 0}
    category_total = dict.fromkeys(category_unexplained, 0)
    category_explained = dict.fromkeys(category_unexplained, 0)

    for identifier, (path, token) in sorted(identifier_first.items(), key=lambda item: _utf8_key(item[0])):
        categories: list[str] = []
        if _ROLE_ENCODING.search(identifier):
            categories.append("compiler-role")
        if _BYTE_ESCAPE.search(identifier):
            categories.append("byte-escape")
        if _SEMANTIC_DIGEST.search(identifier):
            categories.append("semantic-digest")
        if _HASH_SUFFIX.search(identifier):
            categories.append("hash-suffix")
        entries = ledger.get(identifier, [])
        for category in categories:
            if category == "compiler-role":
                explained = bool(entries)
            elif category == "byte-escape":
                explained = any("non-c-identifier-byte" in entry.escape_reasons for entry in entries)
            elif category == "semantic-digest":
                explained = bool(entries)
            else:
                explained = any(entry.collision_resolved or "length-limit" in entry.escape_reasons for entry in entries)
            category_total[category] += 1
            category_explained[category] += int(explained)
            category_unexplained[category] += int(not explained)
            identifier_findings.append(
                {
                    "category": category,
                    "identifier": identifier,
                    "bytes": len(identifier.encode("utf-8")),
                    "explained": explained,
                    "sourceSymbols": [entry.source_symbol for entry in entries],
                    "escapeReasons": sorted(
                        {reason for entry in entries for reason in entry.escape_reasons}, key=_utf8_key
                    ),
                    "collisionResolved": any(entry.collision_resolved for entry in entries),
                    "coordinate": _coordinate(path, token.line, token.column),
                }
            )

    identifier_findings.sort(
        key=lambda row: (
            _utf8_key(str(row["identifier"])),
            _utf8_key(str(row["category"])),
            _utf8_key(str(row["coordinate"]["path"])),  # type: ignore[index]
        )
    )

    max_identifier_row: tuple[str, _Token] | None = None
    if identifier_first:
        max_identifier_row = min(
            identifier_first.items(),
            key=lambda item: (-len(item[0].encode("utf-8")), _utf8_key(item[0])),
        )
        identifier, (path, token) = max_identifier_row
        if len(identifier.encode("utf-8")) > policy.limits.max_identifier_bytes:
            violations.append(
                _violation(
                    "identifier-bytes",
                    "C identifier byte length exceeds the reviewed policy",
                    path,
                    token.line,
                    token.column,
                    actual=len(identifier.encode("utf-8")),
                    limit=policy.limits.max_identifier_bytes,
                )
            )

    unexplained_rules = (
        ("compiler-role", "unexplained-role-encoding", policy.limits.max_unexplained_role_encoded_identifiers),
        ("byte-escape", "unexplained-byte-escape", policy.limits.max_unexplained_byte_escaped_identifiers),
        ("semantic-digest", "unexplained-digest", policy.limits.max_unexplained_digest_identifiers),
        ("hash-suffix", "unexplained-hash-suffix", policy.limits.max_unexplained_hash_suffix_identifiers),
    )
    for category, rule, limit in unexplained_rules:
        if category_unexplained[category] > limit:
            first = next(
                finding
                for finding in identifier_findings
                if finding["category"] == category and not finding["explained"]
            )
            coordinate = first["coordinate"]
            assert isinstance(coordinate, Mapping)
            violations.append(
                _violation(
                    rule,
                    f"unexplained {category} identifiers exceed the reviewed policy",
                    str(coordinate["path"]),
                    int(coordinate["line"]),
                    int(coordinate["column"]),
                    actual=category_unexplained[category],
                    limit=limit,
                )
            )

    total_temp_density = _rate_per_k(total_temp_declarations, total_function_code_lines)
    if total_temp_declarations > policy.limits.max_temporary_declarations:
        coordinate = first_temp_declaration
        violations.append(
            _violation(
                "temporary-declarations",
                "aggregate temporary declarations exceed the reviewed policy",
                normalized_artifacts[0].path if coordinate is None else coordinate[0],
                1 if coordinate is None else coordinate[1].line,
                1 if coordinate is None else coordinate[1].column,
                function_id=None if coordinate is None else coordinate[2],
                actual=total_temp_declarations,
                limit=policy.limits.max_temporary_declarations,
            )
        )
    if total_temp_references > policy.limits.max_temporary_references:
        coordinate = first_temp_reference
        violations.append(
            _violation(
                "temporary-references",
                "aggregate temporary references exceed the reviewed policy",
                normalized_artifacts[0].path if coordinate is None else coordinate[0],
                1 if coordinate is None else coordinate[1].line,
                1 if coordinate is None else coordinate[1].column,
                function_id=None if coordinate is None else coordinate[2],
                actual=total_temp_references,
                limit=policy.limits.max_temporary_references,
            )
        )
    if total_temp_density > policy.limits.max_temporary_declarations_per_k_function_lines:
        violations.append(
            _violation(
                "temporary-density",
                "aggregate temporary declarations per 1000 function code lines exceed the reviewed policy",
                normalized_artifacts[0].path,
                1,
                1,
                actual=total_temp_density,
                limit=policy.limits.max_temporary_declarations_per_k_function_lines,
            )
        )

    owner_basis_points = _basis_points(owner_mapped_artifact_count, len(normalized_artifacts))
    identity_basis_points = _basis_points(function_identity_mapped_count, len(all_function_reports))
    source_span_basis_points = _basis_points(source_span_mapped_function_count, len(all_function_reports))
    line_mapped_basis_points = _basis_points(line_mapped_function_count, len(all_function_reports))
    if owner_basis_points < policy.limits.min_owner_mapped_artifacts_basis_points:
        violations.append(
            _violation(
                "artifact-owner-coverage",
                "artifact owner coverage is below the reviewed policy",
                normalized_artifacts[0].path,
                1,
                1,
                actual=owner_basis_points,
                limit=policy.limits.min_owner_mapped_artifacts_basis_points,
            )
        )
    if identity_basis_points < policy.limits.min_function_identity_mapped_basis_points:
        violations.append(
            _violation(
                "function-identity-coverage",
                "caller-supplied function identity coverage is below the reviewed policy",
                normalized_artifacts[0].path,
                1,
                1,
                actual=identity_basis_points,
                limit=policy.limits.min_function_identity_mapped_basis_points,
            )
        )
    if source_span_basis_points < policy.limits.min_source_span_mapped_functions_basis_points:
        violations.append(
            _violation(
                "function-source-span-coverage",
                "caller-supplied source-span coverage is below the reviewed policy",
                normalized_artifacts[0].path,
                1,
                1,
                actual=source_span_basis_points,
                limit=policy.limits.min_source_span_mapped_functions_basis_points,
            )
        )
    if total_line_directives < policy.limits.min_line_directive_count:
        violations.append(
            _violation(
                "line-directive-floor",
                "structural #line directive count is below the reviewed policy",
                normalized_artifacts[0].path,
                1,
                1,
                actual=total_line_directives,
                limit=policy.limits.min_line_directive_count,
            )
        )

    goto_rows.sort(
        key=lambda row: (
            _utf8_key(str(row["path"])),
            _utf8_key(str(row["functionId"])),
            _utf8_key(str(row["targetLabel"])),
            int(row["occurrence"]),
            int(row["line"]),
            int(row["column"]),
        )
    )
    goto_counts = {category: 0 for category in GotoCategory}
    for row in goto_rows:
        if row["category"] is not None:
            goto_counts[GotoCategory(str(row["category"]))] += 1
    for category, maximum in policy.goto_limits:
        if goto_counts[category] > maximum:
            first = next(row for row in goto_rows if row["category"] == category.value)
            violations.append(
                _violation(
                    "goto-category-budget",
                    f"{category.value} goto occurrences exceed the reviewed policy",
                    str(first["path"]),
                    int(first["line"]),
                    int(first["column"]),
                    function_id=str(first["functionId"]),
                    actual=goto_counts[category],
                    limit=maximum,
                )
            )

    for key in sorted(set(authorities) - used_authorities, key=lambda row: tuple(_utf8_key(str(value)) for value in row)):
        path, function_id, target, occurrence = key
        violations.append(
            _violation(
                "goto-authority-unused",
                "caller-supplied goto authority did not match an emitted occurrence",
                path,
                1,
                1,
                function_id=function_id,
                actual=f"{target}#{occurrence}",
                limit="emitted-goto",
            )
        )
    for path in sorted(set(file_override_map) - used_file_overrides, key=_utf8_key):
        violations.append(
            _violation(
                "policy-file-override-unused",
                "reviewed file override did not match an artifact",
                path,
                1,
                1,
                actual=path,
                limit="analyzed-artifact",
            )
        )
    for function_id in sorted(set(function_override_map) - used_function_overrides, key=_utf8_key):
        violations.append(
            _violation(
                "policy-function-override-unused",
                "reviewed function override did not match a definition",
                normalized_artifacts[0].path,
                1,
                1,
                function_id=function_id,
                actual=function_id,
                limit="analyzed-function",
            )
        )

    violations.sort(key=_violation_key)
    inputs = [
        {"path": artifact.path, "sha256": hashlib.sha256(artifact.content).hexdigest()}
        for artifact in normalized_artifacts
    ]
    max_identifier_summary = None
    if max_identifier_row is not None:
        identifier, (path, token) = max_identifier_row
        max_identifier_summary = {
            "name": identifier,
            "bytes": len(identifier.encode("utf-8")),
            "coordinate": _coordinate(path, token.line, token.column),
        }
    summary = {
        "artifactCount": len(normalized_artifacts),
        "physicalLineCount": total_physical_lines,
        "codeLineCount": total_code_lines,
        "lineDirectiveCount": total_line_directives,
        "mappedPathCount": len(all_mapped_paths),
        "mappedPaths": sorted(all_mapped_paths, key=_utf8_key),
        "functionCount": len(all_function_reports),
        "functionIdentityMappedCount": function_identity_mapped_count,
        "functionIdentityMappedBasisPoints": identity_basis_points,
        "sourceSpanMappedFunctionCount": source_span_mapped_function_count,
        "sourceSpanMappedFunctionBasisPoints": source_span_basis_points,
        "lineMappedFunctionCount": line_mapped_function_count,
        "lineMappedFunctionBasisPoints": line_mapped_basis_points,
        "ownerMappedArtifactCount": owner_mapped_artifact_count,
        "ownerMappedArtifactBasisPoints": owner_basis_points,
        "branchCount": total_branch_count,
        "temporaryDeclarationCount": total_temp_declarations,
        "temporaryReferenceCount": total_temp_references,
        "temporaryDeclarationsPerKFunctionCodeLines": total_temp_density,
        "maxIdentifier": max_identifier_summary,
        "roleEncodedIdentifierCount": category_total["compiler-role"],
        "explainedRoleEncodedIdentifierCount": category_explained["compiler-role"],
        "unexplainedRoleEncodedIdentifierCount": category_unexplained["compiler-role"],
        "byteEscapedIdentifierCount": category_total["byte-escape"],
        "explainedByteEscapedIdentifierCount": category_explained["byte-escape"],
        "unexplainedByteEscapedIdentifierCount": category_unexplained["byte-escape"],
        "digestIdentifierCount": category_total["semantic-digest"],
        "explainedDigestIdentifierCount": category_explained["semantic-digest"],
        "unexplainedDigestIdentifierCount": category_unexplained["semantic-digest"],
        "hashSuffixIdentifierCount": category_total["hash-suffix"],
        "explainedHashSuffixIdentifierCount": category_explained["hash-suffix"],
        "unexplainedHashSuffixIdentifierCount": category_unexplained["hash-suffix"],
        "gotoCount": len(goto_rows),
        "unauthorizedGotoCount": sum(1 for row in goto_rows if row["category"] is None),
        "gotoCategoryCounts": [
            {"category": category.value, "occurrences": goto_counts[category]}
            for category in sorted(GotoCategory, key=lambda item: _utf8_key(item.value))
        ],
        "violationCount": len(violations),
    }
    report: dict[str, object] = {
        "schemaVersion": REPORT_SCHEMA_VERSION,
        "algorithm": ALGORITHM_ID,
        "corpusId": corpus_id,
        "layout": layout,
        "correctnessAuthority": CORRECTNESS_AUTHORITY,
        "status": "within-reviewed-budgets" if not violations else "violations-found",
        "policy": policy_report,
        "inputs": inputs,
        "summary": summary,
        "identifierFindings": identifier_findings,
        "gotoOccurrences": goto_rows,
        "files": file_reports,
        "firstViolation": None if not violations else violations[0],
        "violations": violations,
    }
    validate_report(report)
    return report


_SPEC_DIR = Path(__file__).resolve().parents[2] / "docs" / "specs"


@lru_cache(maxsize=2)
def _load_schema(name: str) -> Mapping[str, object]:
    try:
        value = json.loads((_SPEC_DIR / name).read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise MaintainabilityError(f"cannot load {name}: {error}") from error
    if not isinstance(value, Mapping):
        raise MaintainabilityError(f"{name} must contain an object")
    return value


def _schema_validate(value: object, schema: object, root: Mapping[str, object], context: str) -> None:
    """Validate the small Draft 2020-12 subset used by the two checked-in schemas."""

    if schema is True:
        return
    if schema is False or not isinstance(schema, Mapping):
        raise MaintainabilityError(f"{context} is forbidden by the schema")
    if "$ref" in schema:
        reference = schema["$ref"]
        if not isinstance(reference, str) or not reference.startswith("#/"):
            raise MaintainabilityError(f"{context} uses an unsupported schema reference")
        target: object = root
        for segment in reference[2:].split("/"):
            if not isinstance(target, Mapping) or segment not in target:
                raise MaintainabilityError(f"{context} has an unresolved schema reference")
            target = target[segment]
        _schema_validate(value, target, root, context)
        return
    if "oneOf" in schema:
        alternatives = schema["oneOf"]
        if not isinstance(alternatives, list):
            raise MaintainabilityError(f"{context} has malformed oneOf")
        matches = 0
        for alternative in alternatives:
            try:
                _schema_validate(value, alternative, root, context)
                matches += 1
            except MaintainabilityError:
                pass
        if matches != 1:
            raise MaintainabilityError(f"{context} must match exactly one schema alternative")
        return
    if "const" in schema and value != schema["const"]:
        raise MaintainabilityError(f"{context} does not match the schema constant")
    if "enum" in schema and value not in schema["enum"]:  # type: ignore[operator]
        raise MaintainabilityError(f"{context} is not in the schema enum")

    expected_type = schema.get("type")
    if expected_type == "null":
        if value is not None:
            raise MaintainabilityError(f"{context} must be null")
        return
    if expected_type == "boolean":
        if not isinstance(value, bool):
            raise MaintainabilityError(f"{context} must be boolean")
        return
    if expected_type == "integer":
        if isinstance(value, bool) or not isinstance(value, int):
            raise MaintainabilityError(f"{context} must be integer")
        if "minimum" in schema and value < schema["minimum"]:  # type: ignore[operator]
            raise MaintainabilityError(f"{context} is below the schema minimum")
        if "maximum" in schema and value > schema["maximum"]:  # type: ignore[operator]
            raise MaintainabilityError(f"{context} exceeds the schema maximum")
        return
    if expected_type == "string":
        if not isinstance(value, str):
            raise MaintainabilityError(f"{context} must be string")
        if len(value) < int(schema.get("minLength", 0)):
            raise MaintainabilityError(f"{context} is shorter than the schema minimum")
        pattern = schema.get("pattern")
        if pattern is not None and re.search(str(pattern), value) is None:
            raise MaintainabilityError(f"{context} does not match the schema pattern")
        return
    if expected_type == "array":
        if not isinstance(value, list):
            raise MaintainabilityError(f"{context} must be an array")
        if len(value) < int(schema.get("minItems", 0)):
            raise MaintainabilityError(f"{context} has too few items")
        if "maxItems" in schema and len(value) > int(schema["maxItems"]):
            raise MaintainabilityError(f"{context} has too many items")
        prefix = schema.get("prefixItems", [])
        if not isinstance(prefix, list):
            raise MaintainabilityError(f"{context} has malformed prefixItems")
        for index, item_schema in enumerate(prefix):
            if index < len(value):
                _schema_validate(value[index], item_schema, root, f"{context}[{index}]")
        remaining_schema = schema.get("items", True)
        for index in range(len(prefix), len(value)):
            _schema_validate(value[index], remaining_schema, root, f"{context}[{index}]")
        return
    if expected_type == "object":
        if not isinstance(value, Mapping):
            raise MaintainabilityError(f"{context} must be an object")
        required = schema.get("required", [])
        properties = schema.get("properties", {})
        if not isinstance(required, list) or not isinstance(properties, Mapping):
            raise MaintainabilityError(f"{context} has a malformed object schema")
        missing = set(required) - set(value)
        unknown = set(value) - set(properties)
        if missing or (schema.get("additionalProperties") is False and unknown):
            raise MaintainabilityError(
                f"{context} fields differ: missing={sorted(missing)} unknown={sorted(unknown)}"
            )
        for key, item in value.items():
            if key in properties:
                _schema_validate(item, properties[key], root, f"{context}.{key}")
        return
    if expected_type is not None:
        raise MaintainabilityError(f"{context} uses unsupported schema type {expected_type!r}")


def _checked_path(value: object, context: str) -> str:
    if not isinstance(value, str):
        raise MaintainabilityError(f"{context} must be a string path")
    return _normalize_relative_path(value, context)


def _check_sorted_strings(values: object, context: str, *, paths: bool = False) -> list[str]:
    if not isinstance(values, list) or any(not isinstance(value, str) for value in values):
        raise MaintainabilityError(f"{context} must be a string array")
    result = list(values)
    if paths:
        result = [_checked_path(value, context) for value in result]
    if result != sorted(result, key=_utf8_key) or len(result) != len(set(result)):
        raise MaintainabilityError(f"{context} must be unique and UTF-8 sorted")
    return result


def _check_source_paths(value: object, context: str) -> None:
    if value is None:
        return
    assert isinstance(value, Mapping)
    _checked_path(value["path"], f"{context}.path")
    if (value["endLine"], value["endColumn"]) < (value["startLine"], value["startColumn"]):
        raise MaintainabilityError(f"{context} ends before it starts")


def _check_owner_paths(value: object, context: str) -> None:
    assert isinstance(value, Mapping)
    if value["modulePath"] is not None:
        _checked_path(value["modulePath"], f"{context}.modulePath")
    _check_source_paths(value["source"], f"{context}.source")
    if value["kind"] == OwnerKind.SOURCE_MODULE.value and value["modulePath"] is None:
        raise MaintainabilityError(f"{context} source-module needs modulePath")


def _check_coordinate_path(value: object, context: str) -> None:
    assert isinstance(value, Mapping)
    _checked_path(value["path"], f"{context}.path")


def validate_report(report: Mapping[str, object]) -> None:
    """Validate the closed schema, path rules, ordering, and derived totals."""

    schema = _load_schema("generated-c-maintainability.schema.json")
    _schema_validate(report, schema, schema, "report")
    policy = report["policy"]
    summary = report["summary"]
    inputs = report["inputs"]
    files_value = report["files"]
    findings = report["identifierFindings"]
    gotos = report["gotoOccurrences"]
    violations = report["violations"]
    assert isinstance(policy, Mapping) and isinstance(summary, Mapping)
    assert isinstance(inputs, list) and isinstance(files_value, list)
    assert isinstance(findings, list) and isinstance(gotos, list) and isinstance(violations, list)

    if policy["corpusId"] != report["corpusId"] or policy["layout"] != report["layout"]:
        raise MaintainabilityError("selected policy does not match the report corpus/layout")
    expected_policy_hash = _canonical_sha256({key: policy[key] for key in policy if key != "sha256"})
    if policy["sha256"] != expected_policy_hash:
        raise MaintainabilityError("report.policy.sha256 does not match the selected policy")
    goto_limit_categories = [str(row["category"]) for row in policy["gotoLimits"]]  # type: ignore[index]
    if goto_limit_categories != sorted(item.value for item in GotoCategory):
        raise MaintainabilityError("selected policy must contain every sorted goto category")
    file_override_paths = [
        _checked_path(row["path"], "policy file override path") for row in policy["fileOverrides"]  # type: ignore[index]
    ]
    function_override_ids = [str(row["functionId"]) for row in policy["functionOverrides"]]  # type: ignore[index]
    if file_override_paths != sorted(set(file_override_paths), key=_utf8_key):
        raise MaintainabilityError("policy file overrides are not unique and sorted")
    if function_override_ids != sorted(set(function_override_ids), key=_utf8_key):
        raise MaintainabilityError("policy function overrides are not unique and sorted")

    input_paths = [_checked_path(row["path"], "input path") for row in inputs]
    file_paths = [_checked_path(row["path"], "file path") for row in files_value]
    if input_paths != sorted(set(input_paths), key=_utf8_key) or file_paths != input_paths:
        raise MaintainabilityError("report inputs/files must be unique, matching, and UTF-8 sorted")
    if [row["sha256"] for row in inputs] != [row["sha256"] for row in files_value]:
        raise MaintainabilityError("report input and file hashes differ")

    aggregate = {
        "physicalLineCount": 0,
        "codeLineCount": 0,
        "lineDirectiveCount": 0,
        "functionCount": 0,
        "functionIdentityMappedCount": 0,
        "sourceSpanMappedFunctionCount": 0,
        "lineMappedFunctionCount": 0,
        "branchCount": 0,
        "temporaryDeclarationCount": 0,
        "temporaryReferenceCount": 0,
    }
    all_mapped_paths: set[str] = set()
    function_code_lines = 0
    function_ids: set[str] = set()
    for file_value in files_value:
        assert isinstance(file_value, Mapping)
        _check_owner_paths(file_value["owner"], "file owner")
        file_mapped_paths = _check_sorted_strings(file_value["mappedPaths"], "file mappedPaths", paths=True)
        if file_value["mappedPathCount"] != len(file_mapped_paths):
            raise MaintainabilityError("file mappedPathCount is inconsistent")
        all_mapped_paths.update(file_mapped_paths)
        directives = file_value["lineDirectives"]
        functions_value = file_value["functions"]
        assert isinstance(directives, list) and isinstance(functions_value, list)
        directive_lines = [int(row["physicalLine"]) for row in directives]
        if directive_lines != sorted(set(directive_lines)):
            raise MaintainabilityError("file line directives are not uniquely ordered")
        for directive in directives:
            _checked_path(directive["mappedPath"], "line directive mappedPath")
        if file_value["lineDirectiveCount"] != len(directives):
            raise MaintainabilityError("file lineDirectiveCount is inconsistent")
        function_order = [(int(row["startLine"]), _utf8_key(str(row["functionId"]))) for row in functions_value]
        if function_order != sorted(function_order):
            raise MaintainabilityError("file functions are not canonically ordered")
        for function in functions_value:
            assert isinstance(function, Mapping)
            function_id = str(function["functionId"])
            if function_id in function_ids:
                raise MaintainabilityError(f"duplicate function identity {function_id}")
            function_ids.add(function_id)
            _check_owner_paths(function["owner"], "function owner")
            _check_source_paths(function["source"], "function source")
            _check_sorted_strings(function["mappedPaths"], "function mappedPaths", paths=True)
            if int(function["endLine"]) < int(function["startLine"]):
                raise MaintainabilityError("function ends before it starts")
            if function["physicalLineCount"] != int(function["endLine"]) - int(function["startLine"]) + 1:
                raise MaintainabilityError("function physicalLineCount is inconsistent")
            aggregate["functionCount"] += 1
            aggregate["functionIdentityMappedCount"] += int(bool(function["identityMapped"]))
            aggregate["sourceSpanMappedFunctionCount"] += int(function["source"] is not None)
            aggregate["lineMappedFunctionCount"] += int(int(function["mappedBodyLineCount"]) > 0)
            for key in ("branchCount", "temporaryDeclarationCount", "temporaryReferenceCount"):
                aggregate[key] += int(function[key])
            function_code_lines += int(function["codeLineCount"])
        for key in ("physicalLineCount", "codeLineCount", "lineDirectiveCount"):
            aggregate[key] += int(file_value[key])

    aggregate["artifactCount"] = len(files_value)
    aggregate["ownerMappedArtifactCount"] = len(files_value)
    aggregate["mappedPathCount"] = len(all_mapped_paths)
    aggregate["functionIdentityMappedBasisPoints"] = _basis_points(
        aggregate["functionIdentityMappedCount"], aggregate["functionCount"]
    )
    aggregate["sourceSpanMappedFunctionBasisPoints"] = _basis_points(
        aggregate["sourceSpanMappedFunctionCount"], aggregate["functionCount"]
    )
    aggregate["lineMappedFunctionBasisPoints"] = _basis_points(
        aggregate["lineMappedFunctionCount"], aggregate["functionCount"]
    )
    aggregate["ownerMappedArtifactBasisPoints"] = _basis_points(len(files_value), len(files_value))
    aggregate["temporaryDeclarationsPerKFunctionCodeLines"] = _rate_per_k(
        aggregate["temporaryDeclarationCount"], function_code_lines
    )
    for key, value in aggregate.items():
        if summary[key] != value:
            raise MaintainabilityError(f"report.summary.{key} is inconsistent")
    summary_mapped_paths = _check_sorted_strings(summary["mappedPaths"], "summary mappedPaths", paths=True)
    if summary_mapped_paths != sorted(all_mapped_paths, key=_utf8_key):
        raise MaintainabilityError("summary mappedPaths is inconsistent")

    finding_order: list[tuple[bytes, bytes, bytes]] = []
    finding_counts: dict[str, tuple[int, int, int]] = {}
    for finding in findings:
        assert isinstance(finding, Mapping)
        coordinate = finding["coordinate"]
        assert isinstance(coordinate, Mapping)
        _check_coordinate_path(coordinate, "identifier coordinate")
        identifier = str(finding["identifier"])
        if finding["bytes"] != len(identifier.encode("utf-8")):
            raise MaintainabilityError("identifier byte count is inconsistent")
        _check_sorted_strings(finding["sourceSymbols"], "finding sourceSymbols")
        _check_sorted_strings(finding["escapeReasons"], "finding escapeReasons")
        category = str(finding["category"])
        total, explained, unexplained = finding_counts.get(category, (0, 0, 0))
        finding_counts[category] = (
            total + 1,
            explained + int(bool(finding["explained"])),
            unexplained + int(not bool(finding["explained"])),
        )
        finding_order.append((_utf8_key(identifier), _utf8_key(category), _utf8_key(str(coordinate["path"]))))
    if finding_order != sorted(finding_order):
        raise MaintainabilityError("identifier findings are not canonically ordered")
    summary_prefixes = {
        "compiler-role": "RoleEncoded",
        "byte-escape": "ByteEscaped",
        "semantic-digest": "Digest",
        "hash-suffix": "HashSuffix",
    }
    for category, prefix in summary_prefixes.items():
        total, explained, unexplained = finding_counts.get(category, (0, 0, 0))
        for key, value in (
            (f"{prefix[0].lower() + prefix[1:]}IdentifierCount", total),
            (f"explained{prefix}IdentifierCount", explained),
            (f"unexplained{prefix}IdentifierCount", unexplained),
        ):
            if summary[key] != value:
                raise MaintainabilityError(f"report.summary.{key} is inconsistent")

    goto_order = [
        (
            _utf8_key(_checked_path(row["path"], "goto path")),
            _utf8_key(str(row["functionId"])),
            _utf8_key(str(row["targetLabel"])),
            int(row["occurrence"]),
            int(row["line"]),
            int(row["column"]),
        )
        for row in gotos
    ]
    if goto_order != sorted(goto_order):
        raise MaintainabilityError("goto occurrences are not canonically ordered")
    goto_counts = {category.value: 0 for category in GotoCategory}
    for row in gotos:
        if row["category"] is not None:
            goto_counts[str(row["category"])] += 1
    expected_goto_counts = [
        {"category": category, "occurrences": goto_counts[category]}
        for category in sorted(goto_counts, key=_utf8_key)
    ]
    if summary["gotoCount"] != len(gotos) or summary["gotoCategoryCounts"] != expected_goto_counts:
        raise MaintainabilityError("goto summary is inconsistent")
    if summary["unauthorizedGotoCount"] != sum(row["category"] is None for row in gotos):
        raise MaintainabilityError("unauthorized goto summary is inconsistent")

    for violation in violations:
        _check_coordinate_path(violation["coordinate"], "violation coordinate")  # type: ignore[index]
    if violations != sorted(violations, key=_violation_key):
        raise MaintainabilityError("violations are not canonically ordered")
    if summary["violationCount"] != len(violations):
        raise MaintainabilityError("violation count is inconsistent")
    expected_status = "within-reviewed-budgets" if not violations else "violations-found"
    if report["status"] != expected_status:
        raise MaintainabilityError("report status is inconsistent")
    expected_first = None if not violations else violations[0]
    if report["firstViolation"] != expected_first:
        raise MaintainabilityError("first violation is inconsistent")


def _source_span_from_report(value: object, context: str) -> SourceSpan | None:
    if value is None:
        return None
    if not isinstance(value, Mapping):
        raise MaintainabilityError(f"{context} must be a source-span object")
    return SourceSpan(
        str(value["path"]),
        int(value["startLine"]),
        int(value["startColumn"]),
        int(value["endLine"]),
        int(value["endColumn"]),
    )


def _owner_from_report(value: object, context: str) -> ArtifactOwner:
    if not isinstance(value, Mapping):
        raise MaintainabilityError(f"{context} must be an owner object")
    module_path = value["modulePath"]
    return ArtifactOwner(
        OwnerKind(str(value["kind"])),
        None if module_path is None else str(module_path),
        _source_span_from_report(value["source"], f"{context}.source"),
    )


def _symbol_ledger_from_report(
    report: Mapping[str, object],
) -> tuple[SymbolLedgerEntry, ...]:
    findings = report["identifierFindings"]
    assert isinstance(findings, list)
    grouped: dict[tuple[str, str], tuple[bool, set[str]]] = {}
    for finding in findings:
        assert isinstance(finding, Mapping)
        source_symbols = finding["sourceSymbols"]
        escape_reasons = finding["escapeReasons"]
        assert isinstance(source_symbols, list) and isinstance(escape_reasons, list)
        for source_symbol in source_symbols:
            key = (str(finding["identifier"]), str(source_symbol))
            collision, reasons = grouped.get(key, (False, set()))
            reasons.update(str(reason) for reason in escape_reasons)
            grouped[key] = (
                collision or bool(finding["collisionResolved"]),
                reasons,
            )
    return tuple(
        SymbolLedgerEntry(
            c_name,
            source_symbol,
            collision,
            tuple(sorted(reasons, key=_utf8_key)),
        )
        for (c_name, source_symbol), (collision, reasons) in sorted(
            grouped.items(),
            key=lambda item: (_utf8_key(item[0][0]), _utf8_key(item[0][1])),
        )
    )


def _goto_authorities_from_report(
    report: Mapping[str, object],
) -> tuple[GotoAuthority, ...]:
    occurrences = report["gotoOccurrences"]
    assert isinstance(occurrences, list)
    return tuple(
        GotoAuthority(
            path=str(occurrence["path"]),
            function_id=str(occurrence["functionId"]),
            target_label=str(occurrence["targetLabel"]),
            occurrence=int(occurrence["occurrence"]),
            category=GotoCategory(str(occurrence["category"])),
        )
        for occurrence in occurrences
        if isinstance(occurrence, Mapping) and occurrence["category"] is not None
    )


def replay_report_from_c(
    report: Mapping[str, object],
    contents: Mapping[str, bytes],
    policy: CorpusPolicy,
) -> dict[str, object]:
    """Re-lex exact C bytes using only the typed facts preserved in a report."""

    validate_report(report)
    files = report["files"]
    assert isinstance(files, list)
    by_path = {
        str(file_value["path"]): file_value
        for file_value in files
        if isinstance(file_value, Mapping)
    }
    normalized_contents: dict[str, bytes] = {}
    for path, content in contents.items():
        normalized_path = _normalize_relative_path(path, "replay content path")
        if normalized_path in normalized_contents:
            raise MaintainabilityError(f"duplicate replay content path {normalized_path}")
        if not isinstance(content, bytes):
            raise MaintainabilityError(f"replay content {normalized_path} must be bytes")
        normalized_contents[normalized_path] = content
    if len(by_path) != len(files) or set(by_path) != set(normalized_contents):
        raise MaintainabilityError("replay C file set differs from the report")

    artifacts: list[GeneratedCArtifact] = []
    for path in sorted(normalized_contents, key=_utf8_key):
        file_value = by_path[path]
        assert isinstance(file_value, Mapping)
        functions_value = file_value["functions"]
        assert isinstance(functions_value, list)
        mappings = tuple(
            FunctionSourceMapping(
                str(function["functionId"]),
                str(function["cName"]),
                _owner_from_report(function["owner"], f"replay {path} function owner"),
                _source_span_from_report(
                    function["source"], f"replay {path} function source"
                ),
            )
            for function in functions_value
            if isinstance(function, Mapping)
        )
        if len(mappings) != len(functions_value):
            raise MaintainabilityError(f"replay {path} contains a malformed function")
        artifacts.append(
            GeneratedCArtifact(
                path,
                normalized_contents[path],
                _owner_from_report(file_value["owner"], f"replay {path} owner"),
                mappings,
            )
        )
    return analyze_generated_c(
        corpus_id=str(report["corpusId"]),
        layout=str(report["layout"]),
        artifacts=tuple(artifacts),
        symbols=_symbol_ledger_from_report(report),
        goto_authorities=_goto_authorities_from_report(report),
        policy=policy,
    )


def _parse_limits(value: Mapping[str, object]) -> MetricLimits:
    expected = _limits_json(MetricLimits(*([0] * len(fields(MetricLimits)))))
    return MetricLimits(*(int(value[name]) for name in expected))


def _policy_from_row(policy_id: str, row: Mapping[str, object]) -> CorpusPolicy:
    goto_values = row["gotoLimits"]
    file_values = row["fileOverrides"]
    function_values = row["functionOverrides"]
    assert isinstance(goto_values, list) and isinstance(file_values, list)
    assert isinstance(function_values, list) and isinstance(row["limits"], Mapping)
    policy = CorpusPolicy(
        policy_id=policy_id,
        corpus_id=str(row["corpusId"]),
        layout=str(row["layout"]),
        limits=_parse_limits(row["limits"]),
        goto_limits=tuple(
            (GotoCategory(str(value["category"])), int(value["maxOccurrences"]))
            for value in goto_values
        ),
        file_overrides=tuple(
            FileLimitOverride(str(value["path"]), int(value["maxFileLines"]), str(value["reason"]))
            for value in file_values
        ),
        function_overrides=tuple(
            FunctionLimitOverride(
                function_id=str(value["functionId"]),
                max_function_lines=value["maxFunctionLines"],
                max_branch_count=value["maxBranchCount"],
                max_nesting_depth=value["maxNestingDepth"],
                max_function_temporary_declarations_per_k_lines=value[
                    "maxFunctionTemporaryDeclarationsPerKLines"
                ],
                reason=str(value["reason"]),
            )
            for value in function_values
        ),
    )
    _validate_policy(policy)
    return policy


def validate_policy_document(document: Mapping[str, object]) -> None:
    schema = _load_schema("generated-c-maintainability-policy.schema.json")
    _schema_validate(document, schema, schema, "policy")
    corpora = document["corpora"]
    assert isinstance(corpora, list)
    policies = [_policy_from_row(str(document["policyId"]), row) for row in corpora]
    corpus_order = [(_utf8_key(policy.corpus_id), _utf8_key(policy.layout)) for policy in policies]
    if corpus_order != sorted(corpus_order) or len(set(corpus_order)) != len(corpus_order):
        raise MaintainabilityError("policy corpora must be unique and UTF-8 sorted")


def load_corpus_policy(path: Path, *, corpus_id: str, layout: str) -> CorpusPolicy:
    """Load and select one reviewed corpus policy from the repository authority."""

    try:
        document = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise MaintainabilityError(f"cannot load maintainability policy {path}: {error}") from error
    if not isinstance(document, Mapping):
        raise MaintainabilityError("policy document root must be an object")
    validate_policy_document(document)
    for row_value in document["corpora"]:  # type: ignore[index]
        assert isinstance(row_value, Mapping)
        if row_value["corpusId"] != corpus_id or row_value["layout"] != layout:
            continue
        return _policy_from_row(str(document["policyId"]), row_value)
    raise MaintainabilityError(f"no reviewed maintainability policy for {corpus_id}/{layout}")

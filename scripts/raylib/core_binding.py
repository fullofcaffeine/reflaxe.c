#!/usr/bin/env python3
"""Extract, verify, and render the precise raylib 6.0 Caxecraft core binding.

Clang's C AST is the declaration authority. The checked selection intentionally
limits the first binding to a coherent, ABI-probed core; declarations outside
that selection remain explicit omissions rather than guessed bindings.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from typing import Iterable, Mapping, Sequence


ROOT = Path(__file__).resolve().parents[2]
SELECTION_PATH = ROOT / "docs/specs/raylib-core-selection.json"
LOCK_PATH = ROOT / "docs/specs/raylib-core-binding-lock.json"
PROVISIONING_LOCK_PATH = ROOT / "docs/specs/raylib-provisioning-lock.json"
RAW_ROOT = ROOT / "src/raylib/raw"
PINNED_COMMIT = "dbc56a87da87d973a9c5baa4e7438a9d20121d28"
PINNED_HEADER_SHA256 = (
    "047e7255f93f8c34039cab906ad76136706b5c7b4c5b5b065d84141963ee9b6b"
)
CANONICAL_TARGET = "x86_64-unknown-linux-gnu"
GENERATOR_PATH = "scripts/raylib/core_binding.py"
HEADER_PLACEHOLDER = "${RAYLIB_HEADER}"
INCLUDE_PLACEHOLDER = "${RAYLIB_INCLUDE}"


class BindingFailure(RuntimeError):
    """The selected raw binding could not be proved exactly."""


def canonical_json(value: object) -> str:
    return json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True) + "\n"


def digest_json(value: object) -> str:
    encoded = json.dumps(
        value, ensure_ascii=False, separators=(",", ":"), sort_keys=True
    ).encode("utf-8")
    return hashlib.sha256(encoded).hexdigest()


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def read_object(path: Path, label: str) -> dict[str, object]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise BindingFailure(f"cannot read {label} at {path}: {error}") from error
    if not isinstance(value, dict):
        raise BindingFailure(f"{label} must contain one JSON object")
    return value


def require_exact_keys(
    value: Mapping[str, object], expected: Sequence[str], label: str
) -> None:
    actual = set(value)
    required = set(expected)
    if actual != required:
        raise BindingFailure(
            f"{label} keys drifted: expected {sorted(required)!r}, "
            f"found {sorted(actual)!r}"
        )


def require_names(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not value:
        raise BindingFailure(f"{label} must be a non-empty name array")
    names: list[str] = []
    for item in value:
        if not isinstance(item, str) or not item or not item.replace("_", "a").isalnum():
            raise BindingFailure(f"{label} contains an invalid name: {item!r}")
        names.append(item)
    if names != sorted(set(names), key=lambda item: item.encode("utf-8")):
        raise BindingFailure(f"{label} must be unique and UTF-8 sorted")
    return names


def require_mapping(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise BindingFailure(f"{label} must be an object")
    return value


def load_selection(path: Path = SELECTION_PATH) -> dict[str, object]:
    selection = read_object(path, "raylib core selection")
    validate_selection(selection)
    return selection


def validate_selection(selection: Mapping[str, object]) -> None:
    require_exact_keys(
        selection,
        (
            "schemaVersion",
            "records",
            "aliases",
            "enums",
            "functions",
            "canonicalAbi",
            "omissions",
        ),
        "raylib core selection",
    )
    if selection.get("schemaVersion") != 1:
        raise BindingFailure("raylib core selection schemaVersion must be 1")
    records = require_names(selection.get("records"), "selection.records")
    enums = require_names(selection.get("enums"), "selection.enums")
    require_names(selection.get("functions"), "selection.functions")

    aliases = require_mapping(selection.get("aliases"), "selection.aliases")
    if not aliases:
        raise BindingFailure("selection.aliases must not be empty")
    if list(aliases) != sorted(aliases, key=lambda item: item.encode("utf-8")):
        raise BindingFailure("selection.aliases must be UTF-8 sorted")
    for name, target in aliases.items():
        if not isinstance(name, str) or not isinstance(target, str) or target not in records:
            raise BindingFailure(f"selection alias {name!r} has unknown target {target!r}")

    abi = require_mapping(selection.get("canonicalAbi"), "selection.canonicalAbi")
    require_exact_keys(
        abi,
        ("target", "languageStandard", "defines", "records"),
        "selection.canonicalAbi",
    )
    if abi.get("target") != CANONICAL_TARGET:
        raise BindingFailure(
            f"selection canonical target must be {CANONICAL_TARGET!r}"
        )
    if abi.get("languageStandard") != "c11":
        raise BindingFailure("selection canonical language standard must be c11")
    if abi.get("defines") != ["RAYLIB_NO_DEPRECATED"]:
        raise BindingFailure("selection canonical defines drifted")
    layouts = require_mapping(abi.get("records"), "selection.canonicalAbi.records")
    if sorted(layouts) != records:
        raise BindingFailure("canonical ABI records must exactly match selected records")
    for name in records:
        layout = require_mapping(layouts.get(name), f"canonical ABI record {name}")
        require_exact_keys(layout, ("size", "alignment", "offsets"), f"layout {name}")
        for key in ("size", "alignment"):
            value = layout.get(key)
            if not isinstance(value, int) or isinstance(value, bool) or value <= 0:
                raise BindingFailure(f"layout {name}.{key} must be a positive integer")
        offsets = require_mapping(layout.get("offsets"), f"layout {name}.offsets")
        if not offsets:
            raise BindingFailure(f"layout {name}.offsets must not be empty")
        for field, offset in offsets.items():
            if not isinstance(field, str) or not isinstance(offset, int) or offset < 0:
                raise BindingFailure(f"layout {name} contains an invalid field offset")

    omissions = selection.get("omissions")
    if not isinstance(omissions, list) or not omissions:
        raise BindingFailure("selection.omissions must be a non-empty array")
    families: set[str] = set()
    for index, raw in enumerate(omissions):
        omission = require_mapping(raw, f"selection.omissions[{index}]")
        require_exact_keys(
            omission, ("family", "reason", "owner"), f"selection.omissions[{index}]"
        )
        family = omission.get("family")
        reason = omission.get("reason")
        owner = omission.get("owner")
        if not isinstance(family, str) or not family or family in families:
            raise BindingFailure("omission families must be non-empty and unique")
        if not isinstance(reason, str) or len(reason) < 20:
            raise BindingFailure(f"omission {family} needs a concrete reason")
        if not isinstance(owner, str) or not owner.startswith("haxe_c-"):
            raise BindingFailure(f"omission {family} needs a stable Beads owner")
        families.add(family)

    overlap = set(records) & set(enums)
    if overlap:
        raise BindingFailure(f"selected declaration kinds overlap: {sorted(overlap)!r}")


def clang_arguments(header: Path, target: str = CANONICAL_TARGET) -> list[str]:
    return [
        f"--target={target}",
        "-x",
        "c",
        "-std=c11",
        "-ffreestanding",
        "-DRAYLIB_NO_DEPRECATED",
        "-I",
        str(header.parent),
        "-Xclang",
        "-ast-dump=json",
        "-fsyntax-only",
        str(header),
    ]


def locked_clang_arguments() -> list[str]:
    return [
        f"--target={CANONICAL_TARGET}",
        "-x",
        "c",
        "-std=c11",
        "-ffreestanding",
        "-DRAYLIB_NO_DEPRECATED",
        "-I",
        INCLUDE_PLACEHOLDER,
        "-Xclang",
        "-ast-dump=json",
        "-fsyntax-only",
        HEADER_PLACEHOLDER,
    ]


def run_text(arguments: Sequence[str], label: str) -> str:
    process = subprocess.run(
        list(arguments),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
    )
    if process.returncode != 0:
        stdout = process.stdout.decode("utf-8", errors="replace").strip()
        stderr = process.stderr.decode("utf-8", errors="replace").strip()
        raise BindingFailure(
            f"{label} failed with exit {process.returncode}"
            + (f"\nstdout:\n{stdout}" if stdout else "")
            + (f"\nstderr:\n{stderr}" if stderr else "")
        )
    try:
        return process.stdout.decode("utf-8")
    except UnicodeDecodeError as error:
        raise BindingFailure(f"{label} emitted malformed UTF-8") from error


def clang_identity(clang: str) -> dict[str, object]:
    version_lines = run_text((clang, "--version"), "Clang identity").splitlines()
    if not version_lines:
        raise BindingFailure("Clang --version returned no identity")
    dump_machine = run_text((clang, "-dumpmachine"), "Clang dumpmachine").strip()
    return {
        "family": "clang",
        "version": version_lines[0].strip(),
        "dumpMachine": dump_machine,
    }


def header_path(source_root: Path) -> Path:
    header = source_root.resolve() / "src/raylib.h"
    if not header.is_file():
        raise BindingFailure(f"raylib source is missing src/raylib.h: {source_root}")
    actual = sha256_file(header)
    if actual != PINNED_HEADER_SHA256:
        raise BindingFailure(
            "pinned raylib.h hash drifted: "
            f"expected {PINNED_HEADER_SHA256}, found {actual}"
        )
    return header


def parse_clang_ast(clang: str, header: Path) -> dict[str, object]:
    output = run_text((clang, *clang_arguments(header)), "Clang raylib AST extraction")
    try:
        ast = json.loads(output)
    except json.JSONDecodeError as error:
        raise BindingFailure(f"Clang emitted malformed AST JSON: {error}") from error
    if not isinstance(ast, dict) or not isinstance(ast.get("inner"), list):
        raise BindingFailure("Clang AST root has an unexpected shape")
    return ast


def source_line(node: Mapping[str, object], label: str) -> int:
    loc = require_mapping(node.get("loc"), f"{label}.loc")
    line = loc.get("line")
    if not isinstance(line, int) or line <= 0:
        begin = require_mapping(
            require_mapping(node.get("range"), f"{label}.range").get("begin"),
            f"{label}.range.begin",
        )
        line = begin.get("line")
    if not isinstance(line, int) or line <= 0:
        raise BindingFailure(f"{label} has no stable source line")
    return line


def type_fact(type_node: object, label: str) -> dict[str, str]:
    mapping = require_mapping(type_node, f"{label}.type")
    canonical = mapping.get("desugaredQualType", mapping.get("qualType"))
    spelling = mapping.get("qualType")
    if not isinstance(canonical, str) or not isinstance(spelling, str):
        raise BindingFailure(f"{label} has no closed Clang type spelling")
    c_type = "bool" if spelling == "_Bool" else spelling
    haxe_types = {
        "void": "Void",
        "_Bool": "Bool",
        "bool": "Bool",
        "unsigned char": "c.UInt8",
        "int": "c.Int32",
        "unsigned int": "c.UInt32",
        "float": "c.Float32",
        "double": "Float",
        "const char *": "c.CString",
    }
    haxe_type = haxe_types.get(spelling)
    if haxe_type is None and spelling.startswith("struct "):
        name = spelling.removeprefix("struct ")
        haxe_type = f"raylib.raw.{name}"
    if haxe_type is None and spelling.replace("_", "a").isalnum():
        haxe_type = f"raylib.raw.{spelling}"
    if haxe_type is None:
        raise BindingFailure(
            f"{label} uses unsupported selected C type {spelling!r}; omit it explicitly"
        )
    return {
        "cType": c_type,
        "canonicalCType": canonical,
        "haxeType": haxe_type,
    }


def named_nodes(ast: Mapping[str, object], kind: str) -> dict[str, dict[str, object]]:
    result: dict[str, dict[str, object]] = {}
    inner = ast.get("inner")
    if not isinstance(inner, list):
        raise BindingFailure("Clang AST omitted top-level declarations")
    for raw in inner:
        if not isinstance(raw, dict) or raw.get("kind") != kind:
            continue
        name = raw.get("name")
        if not isinstance(name, str) or not name:
            continue
        if kind == "RecordDecl" and raw.get("completeDefinition") is not True:
            continue
        if name in result:
            raise BindingFailure(f"Clang AST contains duplicate {kind} {name}")
        result[name] = raw
    return result


def require_selected_node(
    nodes: Mapping[str, dict[str, object]], name: str, kind: str
) -> dict[str, object]:
    node = nodes.get(name)
    if node is None:
        raise BindingFailure(f"selected {kind} {name!r} is missing from pinned raylib.h")
    return node


def extract_records(
    ast: Mapping[str, object], selection: Mapping[str, object]
) -> list[dict[str, object]]:
    nodes = named_nodes(ast, "RecordDecl")
    layouts = require_mapping(
        require_mapping(selection.get("canonicalAbi"), "canonicalAbi").get("records"),
        "canonicalAbi.records",
    )
    records: list[dict[str, object]] = []
    for name in require_names(selection.get("records"), "selection.records"):
        node = require_selected_node(nodes, name, "record")
        raw_fields = node.get("inner")
        if not isinstance(raw_fields, list):
            raise BindingFailure(f"record {name} omitted its fields")
        fields: list[dict[str, object]] = []
        for raw in raw_fields:
            if not isinstance(raw, dict) or raw.get("kind") != "FieldDecl":
                continue
            field_name = raw.get("name")
            if not isinstance(field_name, str) or not field_name:
                raise BindingFailure(f"record {name} contains an unnamed field")
            fields.append(
                {
                    "name": field_name,
                    "sourceLine": source_line(raw, f"record {name}.{field_name}"),
                    "type": type_fact(raw.get("type"), f"record {name}.{field_name}"),
                }
            )
        if not fields:
            raise BindingFailure(f"selected record {name} has no fields")
        layout = require_mapping(layouts.get(name), f"canonical layout {name}")
        offsets = require_mapping(layout.get("offsets"), f"canonical layout {name}.offsets")
        if set(offsets) != {field["name"] for field in fields}:
            raise BindingFailure(
                f"canonical layout {name} fields do not match the Clang declaration"
            )
        records.append(
            {
                "name": name,
                "sourceLine": source_line(node, f"record {name}"),
                "fields": fields,
                "canonicalAbi": layout,
            }
        )
    return records


def typedef_nodes(ast: Mapping[str, object]) -> dict[str, dict[str, object]]:
    return named_nodes(ast, "TypedefDecl")


def extract_aliases(
    ast: Mapping[str, object], selection: Mapping[str, object]
) -> list[dict[str, object]]:
    nodes = typedef_nodes(ast)
    aliases = require_mapping(selection.get("aliases"), "selection.aliases")
    result: list[dict[str, object]] = []
    for name in sorted(aliases, key=lambda item: item.encode("utf-8")):
        target = aliases[name]
        node = require_selected_node(nodes, name, "typedef alias")
        type_mapping = require_mapping(node.get("type"), f"typedef alias {name}.type")
        spelling = type_mapping.get("qualType")
        if spelling != target:
            raise BindingFailure(
                f"typedef alias {name} drifted: expected {target!r}, found {spelling!r}"
            )
        result.append(
            {
                "name": name,
                "sourceLine": source_line(node, f"typedef alias {name}"),
                "target": target,
            }
        )
    return result


def enum_declarations_by_id(ast: Mapping[str, object]) -> dict[str, dict[str, object]]:
    result: dict[str, dict[str, object]] = {}
    inner = ast.get("inner")
    if not isinstance(inner, list):
        raise BindingFailure("Clang AST omitted top-level declarations")
    for raw in inner:
        if not isinstance(raw, dict) or raw.get("kind") != "EnumDecl":
            continue
        identifier = raw.get("id")
        if isinstance(identifier, str):
            result[identifier] = raw
    return result


def owned_tag_id(typedef: Mapping[str, object], label: str) -> str:
    inner = typedef.get("inner")
    if not isinstance(inner, list):
        raise BindingFailure(f"{label} omitted its enum ownership")
    for raw in inner:
        if not isinstance(raw, dict):
            continue
        owned = raw.get("ownedTagDecl")
        if isinstance(owned, dict) and owned.get("kind") == "EnumDecl":
            identifier = owned.get("id")
            if isinstance(identifier, str):
                return identifier
    raise BindingFailure(f"{label} is not the owner of a closed enum")


def constant_expr_value(node: Mapping[str, object]) -> int | None:
    inner = node.get("inner")
    if not isinstance(inner, list):
        return None
    stack = list(inner)
    while stack:
        raw = stack.pop(0)
        if not isinstance(raw, dict):
            continue
        if raw.get("kind") == "ConstantExpr":
            value = raw.get("value")
            if isinstance(value, str):
                try:
                    return int(value, 0)
                except ValueError as error:
                    raise BindingFailure(f"enum constant has invalid value {value!r}") from error
        nested = raw.get("inner")
        if isinstance(nested, list):
            stack.extend(nested)
    return None


def extract_enums(
    ast: Mapping[str, object], selection: Mapping[str, object]
) -> list[dict[str, object]]:
    typedefs = typedef_nodes(ast)
    declarations = enum_declarations_by_id(ast)
    result: list[dict[str, object]] = []
    for name in require_names(selection.get("enums"), "selection.enums"):
        typedef = require_selected_node(typedefs, name, "enum typedef")
        identifier = owned_tag_id(typedef, f"enum typedef {name}")
        declaration = declarations.get(identifier)
        if declaration is None:
            raise BindingFailure(f"enum typedef {name} lost its owned declaration")
        raw_constants = declaration.get("inner")
        if not isinstance(raw_constants, list):
            raise BindingFailure(f"enum {name} omitted its constants")
        constants: list[dict[str, object]] = []
        previous = -1
        for raw in raw_constants:
            if not isinstance(raw, dict) or raw.get("kind") != "EnumConstantDecl":
                continue
            constant_name = raw.get("name")
            if not isinstance(constant_name, str) or not constant_name:
                raise BindingFailure(f"enum {name} contains an unnamed constant")
            explicit = constant_expr_value(raw)
            value = previous + 1 if explicit is None else explicit
            constants.append(
                {
                    "name": constant_name,
                    "sourceLine": source_line(raw, f"enum {name}.{constant_name}"),
                    "value": value,
                }
            )
            previous = value
        if not constants:
            raise BindingFailure(f"selected enum {name} has no constants")
        result.append(
            {
                "name": name,
                "sourceLine": source_line(declaration, f"enum {name}"),
                "underlyingCanonicalCType": "int",
                "constants": constants,
            }
        )
    return result


def function_result_type(node: Mapping[str, object], label: str) -> dict[str, str]:
    type_mapping = require_mapping(node.get("type"), f"{label}.type")
    spelling = type_mapping.get("qualType")
    if not isinstance(spelling, str) or " (" not in spelling:
        raise BindingFailure(f"{label} has an unsupported Clang function type")
    return type_fact({"qualType": spelling.split(" (", 1)[0]}, f"{label}.result")


def extract_functions(
    ast: Mapping[str, object], selection: Mapping[str, object]
) -> list[dict[str, object]]:
    nodes = named_nodes(ast, "FunctionDecl")
    result: list[dict[str, object]] = []
    for name in require_names(selection.get("functions"), "selection.functions"):
        node = require_selected_node(nodes, name, "function")
        parameters: list[dict[str, object]] = []
        inner = node.get("inner", [])
        if not isinstance(inner, list):
            raise BindingFailure(f"function {name} has malformed parameters")
        for raw in inner:
            if not isinstance(raw, dict) or raw.get("kind") != "ParmVarDecl":
                continue
            parameter_name = raw.get("name")
            if not isinstance(parameter_name, str) or not parameter_name:
                raise BindingFailure(f"function {name} contains an unnamed parameter")
            parameters.append(
                {
                    "name": parameter_name,
                    "type": type_fact(
                        raw.get("type"), f"function {name}.{parameter_name}"
                    ),
                }
            )
        result.append(
            {
                "name": name,
                "sourceLine": source_line(node, f"function {name}"),
                "returnType": function_result_type(node, f"function {name}"),
                "parameters": parameters,
            }
        )
    return result


def extract_declarations(
    ast: Mapping[str, object], selection: Mapping[str, object]
) -> dict[str, object]:
    """Normalize selected declarations independently of Clang discovery order."""

    return {
        "records": extract_records(ast, selection),
        "aliases": extract_aliases(ast, selection),
        "enums": extract_enums(ast, selection),
        "functions": extract_functions(ast, selection),
    }


def extract_lock(
    source_root: Path,
    clang: str,
    *,
    selection_path: Path = SELECTION_PATH,
) -> dict[str, object]:
    selection = load_selection(selection_path)
    header = header_path(source_root)
    ast = parse_clang_ast(clang, header)
    declarations = extract_declarations(ast, selection)
    provisioning = read_object(PROVISIONING_LOCK_PATH, "raylib provisioning lock")
    upstream = require_mapping(provisioning.get("upstream"), "provisioning.upstream")
    if upstream.get("commit") != PINNED_COMMIT:
        raise BindingFailure("raylib provisioning and core binding commits disagree")
    selection_sha256 = sha256_file(selection_path)
    generated_paths = [
        f"src/raylib/raw/{name}.hx"
        for name in sorted(
            set(require_names(selection.get("records"), "selection.records"))
            | set(require_mapping(selection.get("aliases"), "selection.aliases"))
            | set(require_names(selection.get("enums"), "selection.enums"))
            | {"Raylib"},
            key=lambda item: item.encode("utf-8"),
        )
    ]
    lock: dict[str, object] = {
        "schemaVersion": 1,
        "generator": {
            "path": GENERATOR_PATH,
            "algorithm": "hxc-raylib-clang-core-v1",
        },
        "upstream": {
            "name": "raylib",
            "release": "6.0",
            "apiVersion": 600,
            "commit": PINNED_COMMIT,
            "header": {
                "path": "src/raylib.h",
                "sha256": PINNED_HEADER_SHA256,
            },
            "licenseExpression": "Zlib",
            "licenseUrl": upstream.get("licenseUrl"),
        },
        "extraction": {
            "authority": "clang-ast-json",
            "clang": clang_identity(clang),
            "requestedTarget": CANONICAL_TARGET,
            "arguments": locked_clang_arguments(),
        },
        "selection": {
            "path": "docs/specs/raylib-core-selection.json",
            "sha256": selection_sha256,
            "coverageState": "coherent-caxecraft-core",
            "counts": {
                "records": len(declarations["records"]),
                "aliases": len(declarations["aliases"]),
                "enums": len(declarations["enums"]),
                "enumConstants": sum(
                    len(item["constants"]) for item in declarations["enums"]
                ),
                "functions": len(declarations["functions"]),
                "omittedFamilies": len(selection["omissions"]),
            },
            "omissions": selection["omissions"],
        },
        "canonicalAbi": selection["canonicalAbi"],
        "declarations": declarations,
        "declarationSha256": digest_json(declarations),
        "generatedPaths": generated_paths,
    }
    validate_lock(lock, selection=selection, check_selection_hash=False)
    return lock


def declaration_names(value: object, label: str) -> list[str]:
    if not isinstance(value, list):
        raise BindingFailure(f"{label} must be an array")
    result: list[str] = []
    for index, raw in enumerate(value):
        item = require_mapping(raw, f"{label}[{index}]")
        name = item.get("name")
        if not isinstance(name, str) or not name:
            raise BindingFailure(f"{label}[{index}] has no name")
        result.append(name)
    return result


def validate_lock(
    lock: Mapping[str, object],
    *,
    selection: Mapping[str, object] | None = None,
    check_selection_hash: bool = True,
) -> None:
    require_exact_keys(
        lock,
        (
            "schemaVersion",
            "generator",
            "upstream",
            "extraction",
            "selection",
            "canonicalAbi",
            "declarations",
            "declarationSha256",
            "generatedPaths",
        ),
        "raylib core binding lock",
    )
    if lock.get("schemaVersion") != 1:
        raise BindingFailure("raylib core binding lock schemaVersion must be 1")
    generator = require_mapping(lock.get("generator"), "lock.generator")
    if generator != {
        "path": GENERATOR_PATH,
        "algorithm": "hxc-raylib-clang-core-v1",
    }:
        raise BindingFailure("raylib core binding generator identity drifted")
    upstream = require_mapping(lock.get("upstream"), "lock.upstream")
    if upstream.get("commit") != PINNED_COMMIT:
        raise BindingFailure("raylib core binding commit drifted")
    header = require_mapping(upstream.get("header"), "lock.upstream.header")
    if header.get("sha256") != PINNED_HEADER_SHA256:
        raise BindingFailure("raylib core binding header hash drifted")
    extraction = require_mapping(lock.get("extraction"), "lock.extraction")
    if extraction.get("authority") != "clang-ast-json":
        raise BindingFailure("raylib binding authority must remain Clang AST JSON")
    if extraction.get("requestedTarget") != CANONICAL_TARGET:
        raise BindingFailure("raylib binding canonical target drifted")
    if extraction.get("arguments") != locked_clang_arguments():
        raise BindingFailure("raylib binding Clang arguments drifted")
    clang = require_mapping(extraction.get("clang"), "lock.extraction.clang")
    if clang.get("family") != "clang":
        raise BindingFailure("raylib binding extraction was not produced by Clang")
    for key in ("version", "dumpMachine"):
        if not isinstance(clang.get(key), str) or not clang.get(key):
            raise BindingFailure(f"lock.extraction.clang.{key} must be recorded")

    selected = load_selection() if selection is None else dict(selection)
    validate_selection(selected)
    selection_lock = require_mapping(lock.get("selection"), "lock.selection")
    if check_selection_hash and selection_lock.get("sha256") != sha256_file(SELECTION_PATH):
        raise BindingFailure("raylib core selection hash is stale")
    if selection_lock.get("coverageState") != "coherent-caxecraft-core":
        raise BindingFailure("raylib core coverage state drifted")
    if selection_lock.get("omissions") != selected.get("omissions"):
        raise BindingFailure("raylib core omission inventory is stale")
    if lock.get("canonicalAbi") != selected.get("canonicalAbi"):
        raise BindingFailure("raylib core canonical ABI is stale")

    declarations = require_mapping(lock.get("declarations"), "lock.declarations")
    require_exact_keys(
        declarations, ("records", "aliases", "enums", "functions"), "lock.declarations"
    )
    expected_names = {
        "records": require_names(selected.get("records"), "selection.records"),
        "aliases": sorted(
            require_mapping(selected.get("aliases"), "selection.aliases"),
            key=lambda item: item.encode("utf-8"),
        ),
        "enums": require_names(selected.get("enums"), "selection.enums"),
        "functions": require_names(selected.get("functions"), "selection.functions"),
    }
    for kind, expected in expected_names.items():
        actual = declaration_names(declarations.get(kind), f"lock.declarations.{kind}")
        if actual != expected:
            raise BindingFailure(
                f"raylib core {kind} inventory drifted: expected {expected!r}, "
                f"found {actual!r}"
            )
    if lock.get("declarationSha256") != digest_json(declarations):
        raise BindingFailure("raylib core declaration digest is stale")

    records = declarations.get("records")
    if not isinstance(records, list):
        raise BindingFailure("lock declarations omitted records")
    layouts = require_mapping(
        require_mapping(selected.get("canonicalAbi"), "canonicalAbi").get("records"),
        "canonicalAbi.records",
    )
    for raw in records:
        record = require_mapping(raw, "lock record")
        name = record.get("name")
        if not isinstance(name, str) or record.get("canonicalAbi") != layouts.get(name):
            raise BindingFailure(f"record {name!r} canonical ABI is malformed")
        fields = record.get("fields")
        if not isinstance(fields, list):
            raise BindingFailure(f"record {name} fields are malformed")
        field_names = declaration_names(fields, f"record {name}.fields")
        offsets = require_mapping(
            require_mapping(layouts.get(name), f"layout {name}").get("offsets"),
            f"layout {name}.offsets",
        )
        if set(field_names) != set(offsets):
            raise BindingFailure(f"record {name} fields and ABI offsets disagree")

    counts = require_mapping(selection_lock.get("counts"), "lock.selection.counts")
    expected_counts = {
        "records": len(expected_names["records"]),
        "aliases": len(expected_names["aliases"]),
        "enums": len(expected_names["enums"]),
        "enumConstants": sum(
            len(require_mapping(item, "enum").get("constants", []))
            for item in declarations["enums"]
        ),
        "functions": len(expected_names["functions"]),
        "omittedFamilies": len(selected["omissions"]),
    }
    if counts != expected_counts:
        raise BindingFailure("raylib core coverage counts are stale")

    paths = lock.get("generatedPaths")
    if not isinstance(paths, list) or paths != sorted(set(paths)):
        raise BindingFailure("raylib generated paths must be unique and sorted")


def load_lock(path: Path = LOCK_PATH) -> dict[str, object]:
    lock = read_object(path, "raylib core binding lock")
    validate_lock(lock)
    return lock


def verification_projection(lock: Mapping[str, object]) -> dict[str, object]:
    """Drop only the host Clang identity while comparing semantic extraction."""

    return {
        key: value
        for key, value in lock.items()
        if key != "extraction"
    } | {
        "extraction": {
            key: value
            for key, value in require_mapping(lock.get("extraction"), "lock.extraction").items()
            if key != "clang"
        }
    }


def haxe_type(type_fact_value: object) -> str:
    mapping = require_mapping(type_fact_value, "rendered type")
    value = mapping.get("haxeType")
    if not isinstance(value, str) or not value:
        raise BindingFailure("rendered type omitted haxeType")
    return value.removeprefix("raylib.raw.")


def generated_preamble(kind: str, name: str, source_line_value: object) -> list[str]:
    if not isinstance(source_line_value, int):
        raise BindingFailure(f"{kind} {name} omitted its source line")
    return [
        "package raylib.raw;",
        "",
        "/**",
        f" * ABI-faithful raylib 6.0 {kind} `{name}` from raylib.h:{source_line_value}.",
        " *",
        f" * Generated by {GENERATOR_PATH} from commit {PINNED_COMMIT}.",
        " * Edit the selection/generator and run the registered snapshot updater;",
        " * do not hand-edit this raw declaration.",
        " */",
    ]


def render_record(record: Mapping[str, object]) -> str:
    name = record.get("name")
    if not isinstance(name, str):
        raise BindingFailure("record render omitted name")
    lines = generated_preamble("struct", name, record.get("sourceLine"))
    lines.extend(
        [
            "@:c.layout(c.Layout.Struct)",
            '@:c.include("raylib.h", c.IncludeKind.System)',
            f"extern class {name} {{",
        ]
    )
    fields = record.get("fields")
    if not isinstance(fields, list):
        raise BindingFailure(f"record {name} omitted fields during render")
    for index, raw in enumerate(fields):
        field = require_mapping(raw, f"record {name}.fields[{index}]")
        field_name = field.get("name")
        if not isinstance(field_name, str):
            raise BindingFailure(f"record {name} has an unnamed field")
        if index:
            lines.append("")
        lines.append(f"\tpublic var {field_name}:{haxe_type(field.get('type'))};")
    lines.extend(["}", ""])
    return "\n".join(lines)


def render_alias(alias: Mapping[str, object]) -> str:
    name = alias.get("name")
    target = alias.get("target")
    if not isinstance(name, str) or not isinstance(target, str):
        raise BindingFailure("alias render omitted name or target")
    lines = generated_preamble("typedef alias", name, alias.get("sourceLine"))
    lines.extend(
        [
            '@:c.include("raylib.h", c.IncludeKind.System)',
            f"typedef {name} = {target};",
            "",
        ]
    )
    return "\n".join(lines)


def render_enum(enum: Mapping[str, object]) -> str:
    name = enum.get("name")
    if not isinstance(name, str):
        raise BindingFailure("enum render omitted name")
    lines = generated_preamble("closed enum", name, enum.get("sourceLine"))
    lines.extend(
        [
            "@:c.layout(c.Layout.Enum)",
            '@:c.include("raylib.h", c.IncludeKind.System)',
            f"extern enum {name} {{",
        ]
    )
    constants = enum.get("constants")
    if not isinstance(constants, list):
        raise BindingFailure(f"enum {name} omitted constants during render")
    for index, raw in enumerate(constants):
        constant = require_mapping(raw, f"enum {name}.constants[{index}]")
        constant_name = constant.get("name")
        if not isinstance(constant_name, str):
            raise BindingFailure(f"enum {name} has an unnamed constant")
        if index:
            lines.append("")
        lines.append(f"\t{constant_name};")
    lines.extend(["}", ""])
    return "\n".join(lines)


def raylib_metadata() -> list[str]:
    return [
        '#if raylib_system_pkg_config',
        '@:c.pkgConfig("raylib")',
        '#elseif raylib_platform_linux',
        '@:c.link("raylib")',
        '@:c.link("m")',
        '\t#if raylib_configuration_desktop',
        '\t@:c.link("pthread") @:c.link("GL") @:c.link("X11") @:c.link("dl") @:c.link("rt")',
        '\t#end',
        '#elseif raylib_platform_macos',
        '@:c.link("raylib")',
        '\t#if raylib_configuration_desktop',
        '\t@:c.framework("OpenGL") @:c.framework("Cocoa") @:c.framework("IOKit") @:c.framework("CoreFoundation")',
        '\t#end',
        '#elseif raylib_platform_windows',
        '@:c.link("raylib")',
        '@:c.link("winmm")',
        '\t#if raylib_configuration_desktop',
        '\t@:c.link("opengl32") @:c.link("gdi32") @:c.link("shell32") @:c.link("user32")',
        '\t#end',
        '#end',
    ]


def render_raylib(functions: object) -> str:
    if not isinstance(functions, list) or not functions:
        raise BindingFailure("Raylib render needs selected functions")
    first_line = min(
        require_mapping(raw, "function").get("sourceLine", 0) for raw in functions
    )
    lines = generated_preamble("function namespace", "Raylib", first_line)
    lines.extend(raylib_metadata())
    lines.extend(
        [
            '@:c.include("raylib.h", c.IncludeKind.System)',
            "extern class Raylib {",
        ]
    )
    for index, raw in enumerate(functions):
        function = require_mapping(raw, f"functions[{index}]")
        name = function.get("name")
        if not isinstance(name, str):
            raise BindingFailure("function render omitted name")
        parameters = function.get("parameters")
        if not isinstance(parameters, list):
            raise BindingFailure(f"function {name} omitted parameters")
        rendered_parameters: list[str] = []
        for raw_parameter in parameters:
            parameter = require_mapping(raw_parameter, f"function {name} parameter")
            parameter_name = parameter.get("name")
            if not isinstance(parameter_name, str):
                raise BindingFailure(f"function {name} has an unnamed parameter")
            rendered_parameters.append(
                f"{parameter_name}:{haxe_type(parameter.get('type'))}"
            )
        if index:
            lines.append("")
        lines.extend(
            [
                f"\tpublic static function {name}({', '.join(rendered_parameters)}):{haxe_type(function.get('returnType'))};",
            ]
        )
    lines.extend(["}", ""])
    return "\n".join(lines)


def render_files(lock: Mapping[str, object]) -> dict[str, str]:
    validate_lock(lock)
    declarations = require_mapping(lock.get("declarations"), "lock.declarations")
    rendered: dict[str, str] = {}
    for kind, renderer in (
        ("records", render_record),
        ("aliases", render_alias),
        ("enums", render_enum),
    ):
        values = declarations.get(kind)
        if not isinstance(values, list):
            raise BindingFailure(f"lock declarations omitted {kind}")
        for raw in values:
            declaration = require_mapping(raw, f"lock.declarations.{kind}")
            name = declaration.get("name")
            if not isinstance(name, str):
                raise BindingFailure(f"lock.declarations.{kind} contains an unnamed item")
            rendered[f"{name}.hx"] = renderer(declaration)
    rendered["Raylib.hx"] = render_raylib(declarations.get("functions"))
    expected = [Path(path).name for path in lock["generatedPaths"]]
    if sorted(rendered) != sorted(expected):
        raise BindingFailure("rendered raw files do not match the locked generated paths")
    return {name: rendered[name] for name in sorted(rendered)}


def render_abi_probe(lock: Mapping[str, object]) -> str:
    """Render strict C11 assertions for every fact admitted by the core lock."""

    validate_lock(lock)
    declarations = require_mapping(lock.get("declarations"), "lock.declarations")
    lines = [
        "/*",
        " * Generated ABI probe for the precise raylib 6.0 Caxecraft core.",
        f" * Authority: {GENERATOR_PATH} and {PINNED_COMMIT}.",
        " */",
        "#include <float.h>",
        "#include <stddef.h>",
        "#include <stdio.h>",
        "#include \"raylib.h\"",
        "",
        '_Static_assert(sizeof(bool) == 1U, "raylib bool must occupy one byte");',
        '_Static_assert(sizeof(float) == 4U, "raylib float must be binary32-sized");',
        '_Static_assert(FLT_RADIX == 2, "raylib float radix must be binary");',
        '_Static_assert(FLT_MANT_DIG == 24, "raylib float mantissa must be binary32");',
        '_Static_assert(FLT_MAX_EXP == 128, "raylib float exponent must be binary32");',
        '_Static_assert(sizeof(int) == 4U, "raylib int must be 32-bit");',
        '_Static_assert(sizeof(unsigned int) == 4U, "raylib unsigned int must be 32-bit");',
        "",
    ]
    records = declarations.get("records")
    if not isinstance(records, list):
        raise BindingFailure("ABI probe render omitted records")
    for raw in records:
        record = require_mapping(raw, "ABI probe record")
        name = record.get("name")
        abi = require_mapping(record.get("canonicalAbi"), f"ABI probe {name}")
        if not isinstance(name, str):
            raise BindingFailure("ABI probe record omitted name")
        lines.extend(
            [
                f'_Static_assert(sizeof({name}) == {abi["size"]}U, "{name} size drifted");',
                f'_Static_assert(_Alignof({name}) == {abi["alignment"]}U, "{name} alignment drifted");',
            ]
        )
        offsets = require_mapping(abi.get("offsets"), f"ABI probe {name}.offsets")
        fields = record.get("fields")
        if not isinstance(fields, list):
            raise BindingFailure(f"ABI probe {name} omitted fields")
        for field in fields:
            field_name = require_mapping(field, f"ABI probe {name} field").get("name")
            if not isinstance(field_name, str):
                raise BindingFailure(f"ABI probe {name} contains an unnamed field")
            lines.append(
                f'_Static_assert(offsetof({name}, {field_name}) == {offsets[field_name]}U, "{name}.{field_name} offset drifted");'
            )
        lines.append("")

    aliases = declarations.get("aliases")
    if not isinstance(aliases, list):
        raise BindingFailure("ABI probe render omitted aliases")
    for raw in aliases:
        alias = require_mapping(raw, "ABI probe alias")
        name = alias.get("name")
        target = alias.get("target")
        if not isinstance(name, str) or not isinstance(target, str):
            raise BindingFailure("ABI probe alias omitted name or target")
        lines.extend(
            [
                f'_Static_assert(sizeof({name}) == sizeof({target}), "{name} alias size drifted");',
                f'_Static_assert(_Alignof({name}) == _Alignof({target}), "{name} alias alignment drifted");',
                f'_Static_assert(_Generic(({name}){{0}}, {target}: 1, default: 0) == 1, "{name} must alias {target}");',
                "",
            ]
        )

    enums = declarations.get("enums")
    if not isinstance(enums, list):
        raise BindingFailure("ABI probe render omitted enums")
    for raw in enums:
        enum = require_mapping(raw, "ABI probe enum")
        name = enum.get("name")
        constants = enum.get("constants")
        if not isinstance(name, str) or not isinstance(constants, list):
            raise BindingFailure("ABI probe enum is malformed")
        lines.append(
            f'_Static_assert(sizeof({name}) == sizeof(int), "{name} underlying ABI drifted");'
        )
        for raw_constant in constants:
            constant = require_mapping(raw_constant, f"ABI probe enum {name}")
            constant_name = constant.get("name")
            value = constant.get("value")
            if not isinstance(constant_name, str) or not isinstance(value, int):
                raise BindingFailure(f"ABI probe enum {name} contains a malformed constant")
            lines.append(
                f'_Static_assert({constant_name} == {value}, "{constant_name} value drifted");'
            )
        lines.append("")

    functions = declarations.get("functions")
    if not isinstance(functions, list):
        raise BindingFailure("ABI probe render omitted functions")
    lines.extend(["static void hxc_check_raylib_core_signatures(void);", "", "static void", "hxc_check_raylib_core_signatures(void)", "{"])
    signature_names: list[str] = []
    for raw in functions:
        function = require_mapping(raw, "ABI probe function")
        name = function.get("name")
        parameters = function.get("parameters")
        if not isinstance(name, str) or not isinstance(parameters, list):
            raise BindingFailure("ABI probe function is malformed")
        return_type = require_mapping(
            function.get("returnType"), f"ABI probe function {name}.returnType"
        ).get("cType")
        if not isinstance(return_type, str):
            raise BindingFailure(f"ABI probe function {name} omitted its return type")
        parameter_types: list[str] = []
        for parameter in parameters:
            c_type = require_mapping(
                require_mapping(parameter, f"ABI probe function {name} parameter").get("type"),
                f"ABI probe function {name} parameter type",
            ).get("cType")
            if not isinstance(c_type, str):
                raise BindingFailure(f"ABI probe function {name} has a malformed parameter")
            parameter_types.append(c_type)
        signature = f"hxc_signature_{name}"
        signature_names.append(signature)
        lines.append(
            f"    {return_type} (*const {signature})({', '.join(parameter_types) if parameter_types else 'void'}) = &{name};"
        )
    lines.append("")
    lines.extend(f"    (void){name};" for name in signature_names)
    lines.extend(
        [
            "}",
            "",
            "int",
            "main(void)",
            "{",
            "    hxc_check_raylib_core_signatures();",
            '    (void)puts("raylib-core-abi: OK");',
            "    return 0;",
            "}",
            "",
        ]
    )
    return "\n".join(lines)


def write_rendered(output_root: Path, rendered: Mapping[str, str]) -> None:
    output_root.mkdir(parents=True, exist_ok=True)
    expected = set(rendered)
    for existing in output_root.glob("*.hx"):
        if existing.name not in expected:
            existing.unlink()
    for name, value in rendered.items():
        (output_root / name).write_text(value, encoding="utf-8", newline="\n")


def check_rendered(output_root: Path, rendered: Mapping[str, str]) -> None:
    actual_names = sorted(path.name for path in output_root.glob("*.hx"))
    expected_names = sorted(rendered)
    if actual_names != expected_names:
        raise BindingFailure(
            f"generated raw file inventory drifted: expected {expected_names!r}, "
            f"found {actual_names!r}"
        )
    for name, expected in rendered.items():
        try:
            actual = (output_root / name).read_text(encoding="utf-8")
        except (OSError, UnicodeError) as error:
            raise BindingFailure(f"cannot read generated raw file {name}: {error}") from error
        if actual != expected:
            raise BindingFailure(
                f"generated raw file {name} is stale; run the registered snapshot updater"
            )


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    extract = subparsers.add_parser("extract", help="extract the checked core lock")
    extract.add_argument("--source-root", type=Path, required=True)
    extract.add_argument("--clang", default="clang")
    extract.add_argument("--output", type=Path)

    verify = subparsers.add_parser("verify", help="re-extract and compare semantic facts")
    verify.add_argument("--source-root", type=Path, required=True)
    verify.add_argument("--clang", default="clang")
    verify.add_argument("--lock", type=Path, default=LOCK_PATH)

    render = subparsers.add_parser("render", help="render raw Haxe from the lock")
    render.add_argument("--lock", type=Path, default=LOCK_PATH)
    render.add_argument("--output-root", type=Path, default=RAW_ROOT)
    render.add_argument("--check", action="store_true")

    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        if args.command == "extract":
            lock = extract_lock(args.source_root, args.clang)
            rendered = canonical_json(lock)
            if args.output is None:
                print(rendered, end="")
            else:
                args.output.parent.mkdir(parents=True, exist_ok=True)
                args.output.write_text(rendered, encoding="utf-8", newline="\n")
            return 0
        if args.command == "verify":
            expected = read_object(args.lock, "raylib core binding lock")
            validate_lock(expected)
            actual = extract_lock(args.source_root, args.clang)
            if verification_projection(expected) != verification_projection(actual):
                raise BindingFailure(
                    "pinned raylib declarations differ from the checked core binding lock"
                )
            print(
                "raylib-core-binding: OK: pinned Clang declarations match "
                f"{expected['declarationSha256']}"
            )
            return 0
        if args.command == "render":
            rendered = render_files(load_lock(args.lock))
            if args.check:
                check_rendered(args.output_root, rendered)
            else:
                write_rendered(args.output_root, rendered)
            print(
                f"raylib-core-binding: OK: {len(rendered)} deterministic raw Haxe files"
            )
            return 0
        raise BindingFailure(f"unknown command: {args.command}")
    except (BindingFailure, OSError, UnicodeError, json.JSONDecodeError) as error:
        print(f"raylib-core-binding: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

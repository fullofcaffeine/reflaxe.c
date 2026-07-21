#!/usr/bin/env python3
"""Extract and render the precise rlgl quad-batch slice used by Caxecraft.

``rlgl.h`` is a separate upstream API from ``raylib.h``.  This generator keeps
that boundary explicit: Clang supplies the eight function declarations and the
preprocessor value of ``RL_QUADS``; a generated C probe verifies the same facts
against a real pinned raylib build.
"""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path
from typing import Iterable, Mapping, Sequence

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.raylib.core_binding import (
    BindingFailure,
    CANONICAL_TARGET,
    canonical_json,
    clang_identity,
    digest_json,
    extract_functions,
    haxe_type,
    raylib_metadata,
    read_object,
    require_exact_keys,
    require_mapping,
    require_names,
    run_text,
    selection_sha256,
)


SELECTION_PATH = ROOT / "docs/specs/raylib-rlgl-selection.json"
LOCK_PATH = ROOT / "docs/specs/raylib-rlgl-binding-lock.json"
RAW_PATH = ROOT / "src/raylib/raw/Rlgl.hx"
PROBE_PATH = ROOT / "test/raylib_provisioning/native/rlgl_abi_probe.c"
PROVISIONING_LOCK_PATH = ROOT / "docs/specs/raylib-provisioning-lock.json"
PINNED_COMMIT = "dbc56a87da87d973a9c5baa4e7438a9d20121d28"
PINNED_HEADER_SHA256 = "1c9b802c219971dbf2ba9725fb8cea3f14616c039891810a2d31aa442aea4be3"
GENERATOR_PATH = "scripts/raylib/rlgl_binding.py"
HEADER_PLACEHOLDER = "${RLGL_HEADER}"
INCLUDE_PLACEHOLDER = "${RAYLIB_INCLUDE}"
EXPECTED_FUNCTIONS = (
    "rlBegin",
    "rlColor4ub",
    "rlDrawRenderBatchActive",
    "rlEnd",
    "rlNormal3f",
    "rlSetTexture",
    "rlTexCoord2f",
    "rlVertex3f",
)


def load_selection(path: Path = SELECTION_PATH) -> dict[str, object]:
    value = read_object(path, "raylib rlgl selection")
    require_exact_keys(value, ("schemaVersion", "constant", "functions", "omissions"), "raylib rlgl selection")
    if value.get("schemaVersion") != 1:
        raise BindingFailure("raylib rlgl selection schemaVersion must be 1")
    if tuple(require_names(value.get("functions"), "selection.functions")) != EXPECTED_FUNCTIONS:
        raise BindingFailure("raylib rlgl selection must retain the reviewed eight-function quad-and-flush slice")
    constant = require_mapping(value.get("constant"), "selection.constant")
    require_exact_keys(constant, ("name", "value"), "selection.constant")
    if constant != {"name": "RL_QUADS", "value": 7}:
        raise BindingFailure("raylib rlgl selection must retain RL_QUADS = 7")
    omissions = value.get("omissions")
    if not isinstance(omissions, list) or len(omissions) != 1:
        raise BindingFailure("raylib rlgl selection needs one bounded omission family")
    omission = require_mapping(omissions[0], "selection.omissions[0]")
    require_exact_keys(omission, ("family", "reason", "owner"), "selection.omissions[0]")
    if (
        omission.get("family") != "remaining-rlgl-api"
        or not isinstance(omission.get("reason"), str)
        or len(str(omission.get("reason"))) < 40
        or not str(omission.get("owner", "")).startswith("haxe_c-")
    ):
        raise BindingFailure("raylib rlgl omission policy is malformed")
    return value


def header_path(source_root: Path) -> Path:
    header = source_root.resolve() / "src/rlgl.h"
    if not header.is_file():
        raise BindingFailure(f"raylib source is missing src/rlgl.h: {source_root}")
    from scripts.raylib.core_binding import sha256_file

    actual = sha256_file(header)
    if actual != PINNED_HEADER_SHA256:
        raise BindingFailure(f"pinned rlgl.h hash drifted: expected {PINNED_HEADER_SHA256}, found {actual}")
    return header


def clang_arguments(header: Path) -> list[str]:
    return [
        f"--target={CANONICAL_TARGET}",
        "-x",
        "c",
        "-std=c11",
        "-ffreestanding",
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
        "-I",
        INCLUDE_PLACEHOLDER,
        "-Xclang",
        "-ast-dump=json",
        "-fsyntax-only",
        HEADER_PLACEHOLDER,
    ]


def parse_clang_ast(clang: str, header: Path) -> dict[str, object]:
    output = run_text((clang, *clang_arguments(header)), "Clang rlgl AST extraction")
    try:
        ast = json.loads(output)
    except json.JSONDecodeError as error:
        raise BindingFailure(f"Clang emitted malformed rlgl AST JSON: {error}") from error
    if not isinstance(ast, dict) or not isinstance(ast.get("inner"), list):
        raise BindingFailure("Clang rlgl AST root has an unexpected shape")
    return ast


def extract_macro_value(clang: str, header: Path, name: str) -> int:
    arguments = [
        clang,
        f"--target={CANONICAL_TARGET}",
        "-x",
        "c",
        "-std=c11",
        "-ffreestanding",
        "-I",
        str(header.parent),
        "-dM",
        "-E",
        str(header),
    ]
    output = run_text(arguments, "Clang rlgl macro extraction")
    prefix = f"#define {name} "
    matches = [line[len(prefix) :].strip() for line in output.splitlines() if line.startswith(prefix)]
    if len(matches) != 1:
        raise BindingFailure(f"Clang preprocessor did not expose exactly one {name} macro")
    try:
        return int(matches[0], 0)
    except ValueError as error:
        raise BindingFailure(f"Clang exposed a non-integer {name} value: {matches[0]!r}") from error


def extract_lock(source_root: Path, clang: str, *, selection_path: Path = SELECTION_PATH) -> dict[str, object]:
    selection = load_selection(selection_path)
    header = header_path(source_root)
    ast = parse_clang_ast(clang, header)
    functions = extract_functions(ast, selection)
    constant = dict(require_mapping(selection.get("constant"), "selection.constant"))
    actual_constant = extract_macro_value(clang, header, str(constant["name"]))
    if actual_constant != constant["value"]:
        raise BindingFailure(f"RL_QUADS drifted: expected {constant['value']}, found {actual_constant}")
    declarations = {"constant": constant, "functions": functions}
    provisioning = read_object(PROVISIONING_LOCK_PATH, "raylib provisioning lock")
    upstream = require_mapping(provisioning.get("upstream"), "provisioning.upstream")
    if upstream.get("commit") != PINNED_COMMIT:
        raise BindingFailure("raylib provisioning and rlgl binding commits disagree")
    lock: dict[str, object] = {
        "schemaVersion": 1,
        "generator": {"path": GENERATOR_PATH, "algorithm": "hxc-raylib-clang-rlgl-v1"},
        "upstream": {
            "name": "raylib",
            "release": "6.0",
            "commit": PINNED_COMMIT,
            "header": {"path": "src/rlgl.h", "sha256": PINNED_HEADER_SHA256},
            "licenseExpression": "Zlib",
            "licenseUrl": upstream.get("licenseUrl"),
        },
        "extraction": {
            "authority": "clang-ast-and-preprocessor",
            "clang": clang_identity(clang),
            "requestedTarget": CANONICAL_TARGET,
            "arguments": locked_clang_arguments(),
        },
        "selection": {
            "path": "docs/specs/raylib-rlgl-selection.json",
            "sha256": selection_sha256(selection_path),
            "coverageState": "caxecraft-quad-batch-only",
            "counts": {"constants": 1, "functions": len(functions), "omittedFamilies": 1},
            "omissions": selection["omissions"],
        },
        "declarations": declarations,
        "declarationSha256": digest_json(declarations),
        "generatedPaths": ["src/raylib/raw/Rlgl.hx", "test/raylib_provisioning/native/rlgl_abi_probe.c"],
    }
    validate_lock(lock, selection=selection, selection_path=selection_path, check_selection_hash=False)
    return lock


def validate_lock(
    lock: Mapping[str, object],
    *,
    selection: Mapping[str, object] | None = None,
    selection_path: Path = SELECTION_PATH,
    check_selection_hash: bool = True,
) -> None:
    require_exact_keys(
        lock,
        ("schemaVersion", "generator", "upstream", "extraction", "selection", "declarations", "declarationSha256", "generatedPaths"),
        "raylib rlgl binding lock",
    )
    if lock.get("schemaVersion") != 1:
        raise BindingFailure("raylib rlgl binding lock schemaVersion must be 1")
    if lock.get("generator") != {"path": GENERATOR_PATH, "algorithm": "hxc-raylib-clang-rlgl-v1"}:
        raise BindingFailure("raylib rlgl binding generator identity drifted")
    upstream = require_mapping(lock.get("upstream"), "lock.upstream")
    header = require_mapping(upstream.get("header"), "lock.upstream.header")
    if upstream.get("commit") != PINNED_COMMIT or header != {"path": "src/rlgl.h", "sha256": PINNED_HEADER_SHA256}:
        raise BindingFailure("raylib rlgl upstream authority drifted")
    extraction = require_mapping(lock.get("extraction"), "lock.extraction")
    if (
        extraction.get("authority") != "clang-ast-and-preprocessor"
        or extraction.get("requestedTarget") != CANONICAL_TARGET
        or extraction.get("arguments") != locked_clang_arguments()
    ):
        raise BindingFailure("raylib rlgl extraction contract drifted")
    clang = require_mapping(extraction.get("clang"), "lock.extraction.clang")
    if clang.get("family") != "clang" or not isinstance(clang.get("version"), str) or not isinstance(clang.get("dumpMachine"), str):
        raise BindingFailure("raylib rlgl Clang identity is malformed")
    selected = load_selection(selection_path) if selection is None else dict(selection)
    selection_lock = require_mapping(lock.get("selection"), "lock.selection")
    if check_selection_hash and selection_lock.get("sha256") != selection_sha256(selection_path):
        raise BindingFailure("raylib rlgl selection hash is stale")
    if selection_lock.get("path") != "docs/specs/raylib-rlgl-selection.json" or selection_lock.get("coverageState") != "caxecraft-quad-batch-only":
        raise BindingFailure("raylib rlgl selection identity drifted")
    if selection_lock.get("counts") != {"constants": 1, "functions": 8, "omittedFamilies": 1} or selection_lock.get("omissions") != selected["omissions"]:
        raise BindingFailure("raylib rlgl selection counts or omissions are stale")
    declarations = require_mapping(lock.get("declarations"), "lock.declarations")
    require_exact_keys(declarations, ("constant", "functions"), "lock.declarations")
    if declarations.get("constant") != selected["constant"]:
        raise BindingFailure("raylib rlgl constant is stale")
    functions = declarations.get("functions")
    if not isinstance(functions, list) or tuple(item.get("name") for item in functions if isinstance(item, dict)) != EXPECTED_FUNCTIONS:
        raise BindingFailure("raylib rlgl function inventory is stale")
    if lock.get("declarationSha256") != digest_json(declarations):
        raise BindingFailure("raylib rlgl declaration digest is stale")
    if lock.get("generatedPaths") != ["src/raylib/raw/Rlgl.hx", "test/raylib_provisioning/native/rlgl_abi_probe.c"]:
        raise BindingFailure("raylib rlgl generated paths drifted")


def load_lock(path: Path = LOCK_PATH) -> dict[str, object]:
    lock = read_object(path, "raylib rlgl binding lock")
    validate_lock(lock)
    return lock


def verification_projection(lock: Mapping[str, object]) -> dict[str, object]:
    extraction = require_mapping(lock.get("extraction"), "lock.extraction")
    return dict(lock) | {"extraction": {key: value for key, value in extraction.items() if key != "clang"}}


def render_raw(lock: Mapping[str, object]) -> str:
    validate_lock(lock)
    functions = require_mapping(lock.get("declarations"), "lock.declarations").get("functions")
    if not isinstance(functions, list):
        raise BindingFailure("raylib rlgl render omitted functions")
    first_line = min(require_mapping(raw, "rlgl function").get("sourceLine", 0) for raw in functions)
    lines = [
        "package raylib.raw;",
        "",
        "/**",
        f" * ABI-faithful raylib 6.0 rlgl quad-batch namespace from rlgl.h:{first_line}.",
        " *",
        f" * Generated by {GENERATOR_PATH} from commit {PINNED_COMMIT}.",
        " * Edit the selection/generator and run the registered snapshot updater;",
        " * do not hand-edit this raw declaration.",
        " */",
        *raylib_metadata(),
        '@:c.include("rlgl.h", c.IncludeKind.System)',
        "extern class Rlgl {",
    ]
    for index, raw in enumerate(functions):
        function = require_mapping(raw, f"functions[{index}]")
        name = function.get("name")
        parameters = function.get("parameters")
        if not isinstance(name, str) or not isinstance(parameters, list):
            raise BindingFailure("raylib rlgl function is malformed during render")
        rendered_parameters: list[str] = []
        for parameter_raw in parameters:
            parameter = require_mapping(parameter_raw, f"function {name} parameter")
            parameter_name = parameter.get("name")
            if not isinstance(parameter_name, str):
                raise BindingFailure(f"function {name} has an unnamed parameter")
            rendered_parameters.append(f"{parameter_name}:{haxe_type(parameter.get('type'))}")
        if index:
            lines.append("")
        lines.append(f"\tpublic static function {name}({', '.join(rendered_parameters)}):{haxe_type(function.get('returnType'))};")
    lines.extend(["}", ""])
    return "\n".join(lines)


def render_abi_probe(lock: Mapping[str, object]) -> str:
    validate_lock(lock)
    declarations = require_mapping(lock.get("declarations"), "lock.declarations")
    constant = require_mapping(declarations.get("constant"), "lock.declarations.constant")
    functions = declarations.get("functions")
    if not isinstance(functions, list):
        raise BindingFailure("raylib rlgl ABI probe omitted functions")
    lines = [
        "/*",
        " * Generated ABI probe for the precise raylib 6.0 rlgl quad-batch slice.",
        f" * Authority: {GENERATOR_PATH} and {PINNED_COMMIT}.",
        " */",
        "#include <stdio.h>",
        '#include "rlgl.h"',
        "",
        f'_Static_assert({constant["name"]} == {constant["value"]}, "{constant["name"]} value drifted");',
        "",
        "static void hxc_check_raylib_rlgl_signatures(void);",
        "",
        "static void",
        "hxc_check_raylib_rlgl_signatures(void)",
        "{",
    ]
    names: list[str] = []
    for raw in functions:
        function = require_mapping(raw, "rlgl ABI function")
        name = function.get("name")
        parameters = function.get("parameters")
        return_type = require_mapping(function.get("returnType"), f"function {name}.returnType").get("cType")
        if not isinstance(name, str) or not isinstance(parameters, list) or not isinstance(return_type, str):
            raise BindingFailure("raylib rlgl ABI function is malformed")
        parameter_types = [
            str(require_mapping(require_mapping(parameter, f"function {name} parameter").get("type"), "parameter type").get("cType"))
            for parameter in parameters
        ]
        signature = f"hxc_signature_{name}"
        names.append(signature)
        lines.append(f"    {return_type} (*const {signature})({', '.join(parameter_types) if parameter_types else 'void'}) = &{name};")
    lines.append("")
    lines.extend(f"    (void){name};" for name in names)
    lines.extend(
        [
            "}",
            "",
            "int",
            "main(void)",
            "{",
            "    hxc_check_raylib_rlgl_signatures();",
            '    (void)puts("raylib-rlgl-abi: OK");',
            "    return 0;",
            "}",
            "",
        ]
    )
    return "\n".join(lines)


def check_text(path: Path, expected: str, label: str) -> None:
    try:
        actual = path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise BindingFailure(f"cannot read {label} at {path}: {error}") from error
    if actual != expected:
        raise BindingFailure(f"{label} is stale; run the registered snapshot updater")


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    extract = subparsers.add_parser("extract")
    extract.add_argument("--source-root", type=Path, required=True)
    extract.add_argument("--clang", default="clang")
    extract.add_argument("--output", type=Path)
    verify = subparsers.add_parser("verify")
    verify.add_argument("--source-root", type=Path, required=True)
    verify.add_argument("--clang", default="clang")
    verify.add_argument("--lock", type=Path, default=LOCK_PATH)
    render = subparsers.add_parser("render")
    render.add_argument("--lock", type=Path, default=LOCK_PATH)
    render.add_argument("--check", action="store_true")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        if args.command == "extract":
            value = canonical_json(extract_lock(args.source_root, args.clang))
            if args.output is None:
                print(value, end="")
            else:
                args.output.parent.mkdir(parents=True, exist_ok=True)
                args.output.write_text(value, encoding="utf-8", newline="\n")
            return 0
        if args.command == "verify":
            expected = read_object(args.lock, "raylib rlgl binding lock")
            validate_lock(expected)
            actual = extract_lock(args.source_root, args.clang)
            if verification_projection(expected) != verification_projection(actual):
                raise BindingFailure("pinned rlgl declarations differ from the checked binding lock")
            print(f"raylib-rlgl-binding: OK: pinned Clang declarations match {expected['declarationSha256']}")
            return 0
        if args.command == "render":
            lock = load_lock(args.lock)
            raw = render_raw(lock)
            probe = render_abi_probe(lock)
            if args.check:
                check_text(RAW_PATH, raw, "generated raw rlgl binding")
                check_text(PROBE_PATH, probe, "generated rlgl ABI probe")
            else:
                RAW_PATH.parent.mkdir(parents=True, exist_ok=True)
                PROBE_PATH.parent.mkdir(parents=True, exist_ok=True)
                RAW_PATH.write_text(raw, encoding="utf-8", newline="\n")
                PROBE_PATH.write_text(probe, encoding="utf-8", newline="\n")
            print("raylib-rlgl-binding: OK: one raw namespace and one native ABI probe")
            return 0
        raise BindingFailure(f"unknown command: {args.command}")
    except (BindingFailure, OSError, UnicodeError, json.JSONDecodeError, subprocess.SubprocessError) as error:
        print(f"raylib-rlgl-binding: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

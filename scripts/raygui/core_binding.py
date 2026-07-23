#!/usr/bin/env python3
"""Extract and render the first pinned, by-value raygui 5.0 binding slice.

Clang reads the exact upstream header together with the pinned raylib 6.0
header. The checked selection decides which declarations are admitted; this
script never guesses neighboring coverage from a successful declaration.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from typing import Mapping, Sequence


ROOT = Path(__file__).resolve().parents[2]
SELECTION_PATH = ROOT / "docs/specs/raygui-core-selection.json"
LOCK_PATH = ROOT / "docs/specs/raygui-core-binding-lock.json"
RAW_PATH = ROOT / "src/raygui/raw/Raygui.hx"
PINNED_COMMIT = "020a61bebcbe288b4414de3416e219ef40af847a"
PINNED_HEADER_SHA256 = "9428525eef081eaec45f8fedbb68199bcbe876f2ce927d5e2a69e811a01ae3a9"
PINNED_LICENSE_SHA256 = "3d651cb17718213238247b60f599f6706b97183391ad36387ad9949ff8b08c0b"
PINNED_ARCHIVE_SHA256 = "8327ee8ec254ababfd76908cf39857384ad311e4ef43f9c2c7d94bec6e4a6389"
PINNED_ARCHIVE_SIZE = 5_468_832
IMPLEMENTATION_SOURCE_SHA256 = "3c206e58f97342b6b2359be7e28c49620b9045efc9b04fa89fe70102d0b7ce73"
PINNED_TREE = (
    "cf0baec6f439c3eeb72e695d16eeb08e622eea9e2d5d8bb534d39d187f424db2",
    281,
    10_010_338,
)
CANONICAL_TARGET = "x86_64-unknown-linux-gnu"
GENERATOR_PATH = "scripts/raygui/core_binding.py"
HEADER_PLACEHOLDER = "${RAYGUI_HEADER}"
RAYGUI_INCLUDE_PLACEHOLDER = "${RAYGUI_INCLUDE}"
RAYLIB_INCLUDE_PLACEHOLDER = "${RAYLIB_INCLUDE}"

if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.raylib.core_binding import (  # noqa: E402
    BindingFailure,
    canonical_json,
    clang_identity,
    digest_json,
    extract_functions,
    require_exact_keys,
    require_mapping,
    require_names,
    run_text,
    sha256_file,
)
from scripts.raylib.provision import canonical_tree_identity  # noqa: E402


def normalized_text_sha256(path: Path) -> str:
    try:
        text = path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise BindingFailure(f"cannot read UTF-8 selection {path}: {error}") from error
    normalized = text.replace("\r\n", "\n").replace("\r", "\n")
    return hashlib.sha256(normalized.encode("utf-8")).hexdigest()


def read_object(path: Path, label: str) -> dict[str, object]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise BindingFailure(f"cannot read {label} at {path}: {error}") from error
    if not isinstance(value, dict):
        raise BindingFailure(f"{label} must contain one JSON object")
    return value


def load_selection(path: Path = SELECTION_PATH) -> dict[str, object]:
    selection = read_object(path, "raygui selection")
    require_exact_keys(
        selection,
        (
            "schemaVersion",
            "functions",
            "implementation",
            "canonicalAbi",
            "omissions",
        ),
        "raygui selection",
    )
    if selection.get("schemaVersion") != 1:
        raise BindingFailure("raygui selection schemaVersion must be 1")
    require_names(selection.get("functions"), "selection.functions")
    implementation = require_mapping(selection.get("implementation"), "selection.implementation")
    require_exact_keys(
        implementation,
        ("macro", "owner", "translationUnits", "sourceTemplateSha256"),
        "selection.implementation",
    )
    if implementation != {
        "macro": "RAYGUI_IMPLEMENTATION",
        "owner": "raygui-static-library",
        "translationUnits": 1,
        "sourceTemplateSha256": IMPLEMENTATION_SOURCE_SHA256,
    }:
        raise BindingFailure("raygui must retain exactly one named implementation owner")
    abi = require_mapping(selection.get("canonicalAbi"), "selection.canonicalAbi")
    if abi != {
        "target": CANONICAL_TARGET,
        "languageStandard": "c11",
        "defines": [],
    }:
        raise BindingFailure("raygui canonical ABI configuration drifted")
    omissions = selection.get("omissions")
    if not isinstance(omissions, list) or not omissions:
        raise BindingFailure("raygui selection must retain explicit omitted families")
    families: set[str] = set()
    for index, raw in enumerate(omissions):
        item = require_mapping(raw, f"selection.omissions[{index}]")
        require_exact_keys(item, ("family", "reason", "owner"), f"selection.omissions[{index}]")
        family = item.get("family")
        reason = item.get("reason")
        owner = item.get("owner")
        if not isinstance(family, str) or not family or family in families:
            raise BindingFailure("raygui omission families must be unique")
        if not isinstance(reason, str) or len(reason) < 40:
            raise BindingFailure(f"raygui omission {family} needs a concrete reason")
        if not isinstance(owner, str) or not owner.startswith("haxe_c-"):
            raise BindingFailure(f"raygui omission {family} needs a Beads owner")
        families.add(family)
    return selection


def source_inputs(raygui_source: Path, raylib_source: Path) -> tuple[Path, Path, Path]:
    raygui_root = raygui_source.resolve()
    raylib_root = raylib_source.resolve()
    header = raygui_root / "src/raygui.h"
    license_path = raygui_root / "LICENSE"
    raylib_header = raylib_root / "src/raylib.h"
    for path, label in (
        (header, "raygui.h"),
        (license_path, "raygui license"),
        (raylib_header, "raylib.h"),
    ):
        if path.is_symlink() or not path.is_file():
            raise BindingFailure(f"{label} is missing or not a regular file: {path}")
    if sha256_file(header) != PINNED_HEADER_SHA256:
        raise BindingFailure("raygui.h does not match the pinned 5.0 header")
    if sha256_file(license_path) != PINNED_LICENSE_SHA256:
        raise BindingFailure("raygui LICENSE does not match the pinned source")
    identity = canonical_tree_identity(raygui_root)
    if (identity.sha256, identity.file_count, identity.size_bytes) != PINNED_TREE:
        raise BindingFailure(
            "raygui source tree identity drifted: "
            f"sha256={identity.sha256} files={identity.file_count} bytes={identity.size_bytes}"
        )
    return header, raygui_root / "src", raylib_root / "src"


def clang_arguments(header: Path, raygui_include: Path, raylib_include: Path) -> list[str]:
    return [
        f"--target={CANONICAL_TARGET}",
        "-x",
        "c",
        "-std=c11",
        "-I",
        str(raygui_include),
        "-I",
        str(raylib_include),
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
        "-I",
        RAYGUI_INCLUDE_PLACEHOLDER,
        "-I",
        RAYLIB_INCLUDE_PLACEHOLDER,
        "-Xclang",
        "-ast-dump=json",
        "-fsyntax-only",
        HEADER_PLACEHOLDER,
    ]


def parse_ast(clang: str, header: Path, raygui_include: Path, raylib_include: Path) -> dict[str, object]:
    output = run_text(
        (clang, *clang_arguments(header, raygui_include, raylib_include)),
        "Clang raygui AST extraction",
    )
    try:
        value = json.loads(output)
    except json.JSONDecodeError as error:
        raise BindingFailure(f"Clang emitted malformed raygui AST JSON: {error}") from error
    if not isinstance(value, dict) or not isinstance(value.get("inner"), list):
        raise BindingFailure("Clang raygui AST root has an unexpected shape")
    return value


def extract_lock(raygui_source: Path, raylib_source: Path, clang: str) -> dict[str, object]:
    selection = load_selection()
    header, raygui_include, raylib_include = source_inputs(raygui_source, raylib_source)
    ast = parse_ast(clang, header, raygui_include, raylib_include)
    functions = extract_functions(ast, selection)
    declarations = {"functions": functions}
    lock: dict[str, object] = {
        "schemaVersion": 1,
        "generator": {
            "path": GENERATOR_PATH,
            "algorithm": "hxc-raygui-clang-core-v1",
        },
        "upstream": {
            "name": "raygui",
            "release": "5.0",
            "commit": PINNED_COMMIT,
            "archive": {
                "url": f"https://codeload.github.com/raysan5/raygui/tar.gz/{PINNED_COMMIT}",
                "sha256": PINNED_ARCHIVE_SHA256,
                "sizeBytes": PINNED_ARCHIVE_SIZE,
                "rootDirectory": f"raygui-{PINNED_COMMIT}",
            },
            "sourceTree": {
                "algorithm": "hxc-path-size-content-sha256-v1",
                "sha256": PINNED_TREE[0],
                "fileCount": PINNED_TREE[1],
                "sizeBytes": PINNED_TREE[2],
            },
            "header": {"path": "src/raygui.h", "sha256": PINNED_HEADER_SHA256},
            "license": {
                "expression": "Zlib",
                "path": "LICENSE",
                "sha256": PINNED_LICENSE_SHA256,
            },
        },
        "extraction": {
            "authority": "clang-ast-json",
            "clang": clang_identity(clang),
            "requestedTarget": CANONICAL_TARGET,
            "arguments": locked_clang_arguments(),
        },
        "selection": {
            "path": "docs/specs/raygui-core-selection.json",
            "sha256": normalized_text_sha256(SELECTION_PATH),
            "coverageState": "coherent-editor-shell-core",
            "counts": {
                "functions": len(functions),
                "omittedFamilies": len(selection["omissions"]),
            },
            "omissions": selection["omissions"],
        },
        "implementation": selection["implementation"],
        "canonicalAbi": selection["canonicalAbi"],
        "declarations": declarations,
        "declarationSha256": digest_json(declarations),
        "generatedPaths": ["src/raygui/raw/Raygui.hx"],
    }
    validate_lock(lock)
    return lock


def validate_lock(lock: Mapping[str, object]) -> None:
    selection = load_selection()
    require_exact_keys(
        lock,
        (
            "schemaVersion",
            "generator",
            "upstream",
            "extraction",
            "selection",
            "implementation",
            "canonicalAbi",
            "declarations",
            "declarationSha256",
            "generatedPaths",
        ),
        "raygui binding lock",
    )
    if lock.get("schemaVersion") != 1:
        raise BindingFailure("raygui binding lock schemaVersion must be 1")
    generator = require_mapping(lock.get("generator"), "lock.generator")
    if generator != {
        "path": GENERATOR_PATH,
        "algorithm": "hxc-raygui-clang-core-v1",
    }:
        raise BindingFailure("raygui binding generator identity drifted")
    upstream = require_mapping(lock.get("upstream"), "lock.upstream")
    header = require_mapping(upstream.get("header"), "lock.upstream.header")
    license_fact = require_mapping(upstream.get("license"), "lock.upstream.license")
    archive = require_mapping(upstream.get("archive"), "lock.upstream.archive")
    tree = require_mapping(upstream.get("sourceTree"), "lock.upstream.sourceTree")
    if (
        upstream.get("name") != "raygui"
        or upstream.get("release") != "5.0"
        or upstream.get("commit") != PINNED_COMMIT
        or header != {"path": "src/raygui.h", "sha256": PINNED_HEADER_SHA256}
        or license_fact != {"expression": "Zlib", "path": "LICENSE", "sha256": PINNED_LICENSE_SHA256}
        or archive
        != {
            "url": f"https://codeload.github.com/raysan5/raygui/tar.gz/{PINNED_COMMIT}",
            "sha256": PINNED_ARCHIVE_SHA256,
            "sizeBytes": PINNED_ARCHIVE_SIZE,
            "rootDirectory": f"raygui-{PINNED_COMMIT}",
        }
        or tree
        != {
            "algorithm": "hxc-path-size-content-sha256-v1",
            "sha256": PINNED_TREE[0],
            "fileCount": PINNED_TREE[1],
            "sizeBytes": PINNED_TREE[2],
        }
    ):
        raise BindingFailure("raygui pinned upstream identity drifted")
    extraction = require_mapping(lock.get("extraction"), "lock.extraction")
    clang = require_mapping(extraction.get("clang"), "lock.extraction.clang")
    if (
        extraction.get("authority") != "clang-ast-json"
        or extraction.get("requestedTarget") != CANONICAL_TARGET
        or extraction.get("arguments") != locked_clang_arguments()
        or clang.get("family") != "clang"
        or not isinstance(clang.get("version"), str)
        or not clang.get("version")
        or not isinstance(clang.get("dumpMachine"), str)
        or not clang.get("dumpMachine")
    ):
        raise BindingFailure("raygui Clang extraction contract drifted")
    if lock.get("implementation") != selection["implementation"]:
        raise BindingFailure("raygui implementation ownership drifted")
    if lock.get("canonicalAbi") != selection["canonicalAbi"]:
        raise BindingFailure("raygui canonical ABI drifted")
    selection_fact = require_mapping(lock.get("selection"), "lock.selection")
    if (
        selection_fact.get("path") != "docs/specs/raygui-core-selection.json"
        or
        selection_fact.get("sha256") != normalized_text_sha256(SELECTION_PATH)
        or selection_fact.get("coverageState") != "coherent-editor-shell-core"
        or selection_fact.get("counts")
        != {
            "functions": len(selection["functions"]),
            "omittedFamilies": len(selection["omissions"]),
        }
        or selection_fact.get("omissions") != selection["omissions"]
    ):
        raise BindingFailure("raygui selection identity drifted")
    declarations = require_mapping(lock.get("declarations"), "lock.declarations")
    functions = declarations.get("functions")
    if not isinstance(functions, list):
        raise BindingFailure("raygui lock declarations.functions must be an array")
    actual_names = [item.get("name") for item in functions if isinstance(item, dict)]
    if actual_names != selection["functions"]:
        raise BindingFailure("raygui locked functions do not exactly match the selection")
    if lock.get("declarationSha256") != digest_json(declarations):
        raise BindingFailure("raygui declaration digest drifted")
    if lock.get("generatedPaths") != ["src/raygui/raw/Raygui.hx"]:
        raise BindingFailure("raygui generated path ownership drifted")


def metadata_lines() -> list[str]:
    return [
        "#if raylib_system_pkg_config",
        '@:c.pkgConfig("raylib")',
        '@:c.link("raygui")',
        "#elseif raylib_platform_linux",
        '@:c.link("raygui")',
        '@:c.link("raylib")',
        '@:c.link("m")',
        "\t#if raylib_configuration_desktop",
        '\t@:c.link("pthread") @:c.link("GL") @:c.link("X11") @:c.link("dl") @:c.link("rt")',
        "\t#end",
        "#elseif raylib_platform_macos",
        '@:c.link("raygui")',
        '@:c.link("raylib")',
        "\t#if raylib_configuration_desktop",
        '\t@:c.framework("OpenGL") @:c.framework("Cocoa") @:c.framework("IOKit") @:c.framework("CoreFoundation")',
        "\t#end",
        "#elseif raylib_platform_windows",
        '@:c.link("raygui")',
        '@:c.link("raylib")',
        '@:c.link("winmm")',
        "\t#if raylib_configuration_desktop",
        '\t@:c.link("opengl32") @:c.link("gdi32") @:c.link("shell32") @:c.link("user32")',
        "\t#end",
        "#end",
    ]


def render_raw(lock: Mapping[str, object]) -> str:
    validate_lock(lock)
    declarations = require_mapping(lock.get("declarations"), "lock.declarations")
    functions = declarations.get("functions")
    if not isinstance(functions, list):
        raise BindingFailure("raygui functions disappeared before rendering")
    lines = [
        "package raygui.raw;",
        "",
        "/**",
        " * ABI-faithful raygui 5.0 editor-shell function namespace.",
        " *",
        f" * Generated by {GENERATOR_PATH} from commit {PINNED_COMMIT}.",
        " * Edit the selection/generator and run its registered check; do not",
        " * hand-edit this raw declaration.",
        " */",
        *metadata_lines(),
        '@:c.include("raygui.h", c.IncludeKind.System)',
        "extern class Raygui {",
    ]
    for raw in functions:
        function = require_mapping(raw, "raygui function")
        name = function.get("name")
        result = require_mapping(function.get("returnType"), f"function {name}.returnType")
        parameters = function.get("parameters")
        if not isinstance(name, str) or not isinstance(parameters, list):
            raise BindingFailure("raygui function has malformed renderer input")
        rendered_parameters: list[str] = []
        for raw_parameter in parameters:
            parameter = require_mapping(raw_parameter, f"function {name}.parameter")
            parameter_type = require_mapping(parameter.get("type"), f"function {name}.parameter.type")
            rendered_parameters.append(f'{parameter.get("name")}:{parameter_type.get("haxeType")}')
        lines.append(
            f'\tpublic static function {name}({", ".join(rendered_parameters)}):{result.get("haxeType")};'
        )
        lines.append("")
    if lines[-1] == "":
        lines.pop()
    lines.extend(("}", ""))
    return "\n".join(lines)


def load_lock() -> dict[str, object]:
    lock = read_object(LOCK_PATH, "raygui binding lock")
    validate_lock(lock)
    return lock


def write_if_changed(path: Path, content: str) -> bool:
    current = path.read_text(encoding="utf-8") if path.is_file() else None
    if current == content:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8", newline="\n")
    return True


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("extract", "write", "check"))
    parser.add_argument("--raygui-source", type=Path)
    parser.add_argument("--raylib-source", type=Path)
    parser.add_argument("--clang", default="clang")
    return parser.parse_args(argv)


def main(argv: Sequence[str]) -> int:
    args = parse_args(argv)
    try:
        if args.command in ("extract", "write"):
            if args.raygui_source is None or args.raylib_source is None:
                raise BindingFailure("extract/write require --raygui-source and --raylib-source")
            lock = extract_lock(args.raygui_source, args.raylib_source, args.clang)
        else:
            lock = load_lock()
        raw = render_raw(lock)
        if args.command == "extract":
            sys.stdout.write(canonical_json(lock))
        elif args.command == "write":
            changed = []
            if write_if_changed(LOCK_PATH, canonical_json(lock)):
                changed.append(LOCK_PATH.relative_to(ROOT).as_posix())
            if write_if_changed(RAW_PATH, raw):
                changed.append(RAW_PATH.relative_to(ROOT).as_posix())
            print("raygui-binding: UPDATED: " + (", ".join(changed) if changed else "no changes"))
        else:
            if not RAW_PATH.is_file() or RAW_PATH.read_text(encoding="utf-8") != raw:
                raise BindingFailure("generated raygui raw binding drifted; run core_binding.py write")
            print("raygui-binding: OK: pinned lock and generated raw subset match")
    except (BindingFailure, OSError, UnicodeError) as error:
        print(f"raygui-binding: ERROR: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

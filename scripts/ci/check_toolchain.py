#!/usr/bin/env python3
"""Validate the immutable Haxe/Reflaxe/Lix development toolchain contract."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import subprocess
import sys
from pathlib import Path, PurePosixPath
from typing import Any, Iterable


HAXE_VERSION = "5.0.0-preview.1"
HAXE_REVISION = "2c1e544e0a2c7524ef4c8e103f1b0580362ea538"
HAXE_ARTIFACTS = {
    "linux64": "sha256:57710c7219c2d23bbd490cc5ed49e43686a946ab3a4910a7983a9d15fb078732",
    "macUniversal": "sha256:aeb033a1500ba971d0428db5bd8ef58a32266f900edcfc367b6c6851213d5d61",
    "windows64": "sha256:c223025518c6a527c66bd6c9ca51b4eff848ffcac97fc6c1833d1338cef1622e",
}
REFLAXE_VERSION = "4.0.0-beta"
REFLAXE_REVISION = "73a983112e039daad46b37912ab238df6bf0cf53"
REFLAXE_GIT_TREE = "05ab4abc670b6e3ed5424e78c622b92979b5affb"
LIX_VERSION = "17.0.2"
LIX_INTEGRITY = "sha512-Y8d2wU8e+p4M442dvHfvCGJSmwPQNHn07/qDQU2Fu+iNfJrWr5SRFQlN6/iHkBGgzbCdrVca6f+iutfcZgch8A=="
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")


def load_json(path: Path, errors: list[str]) -> dict[str, Any]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        errors.append(f"cannot read {path.name}: {error}")
        return {}
    if not isinstance(value, dict):
        errors.append(f"{path.name} must contain a JSON object")
        return {}
    return value


def safe_child(root: Path, value: Any, label: str, errors: list[str]) -> Path | None:
    if not isinstance(value, str) or not value or "\\" in value:
        errors.append(f"{label} must be a non-empty POSIX relative path")
        return None
    relative = PurePosixPath(value)
    if (
        relative.is_absolute()
        or relative.as_posix() != value
        or any(part in ("", ".", "..") for part in relative.parts)
    ):
        errors.append(f"{label} must be a normalized relative path")
        return None
    candidate = root.joinpath(*relative.parts)
    try:
        candidate.resolve(strict=False).relative_to(root.resolve())
    except ValueError:
        errors.append(f"{label} escapes its root")
        return None
    return candidate


def bundle_digest(
    root: Path, relative_paths: Any, errors: list[str]
) -> tuple[str, int]:
    if not isinstance(relative_paths, list) or not relative_paths:
        errors.append("reflaxe.bundlePaths must be a non-empty array")
        return "", 0

    files: set[Path] = set()
    for index, value in enumerate(relative_paths):
        path = safe_child(root, value, f"reflaxe.bundlePaths[{index}]", errors)
        if path is None:
            continue
        if path.is_symlink() or not path.exists():
            errors.append(f"bundled Reflaxe path is missing or a symlink: {value}")
            continue
        if path.is_file():
            files.add(path)
        elif path.is_dir():
            for child in path.rglob("*"):
                if child.is_symlink():
                    errors.append(
                        "bundled Reflaxe tree contains a symlink: "
                        + child.relative_to(root).as_posix()
                    )
                elif child.is_file():
                    files.add(child)
        else:
            errors.append(f"unsupported bundled Reflaxe path: {value}")

    digest = hashlib.sha256()
    for path in sorted(files, key=lambda candidate: candidate.relative_to(root).as_posix()):
        relative = path.relative_to(root).as_posix().encode("utf-8")
        contents = path.read_bytes()
        digest.update(relative)
        digest.update(b"\0")
        digest.update(len(contents).to_bytes(8, "big"))
        digest.update(contents)
    return digest.hexdigest(), len(files)


def meaningful_hxml_lines(path: Path, errors: list[str]) -> list[str]:
    try:
        raw_lines = path.read_text(encoding="utf-8").splitlines()
    except (OSError, UnicodeError) as error:
        errors.append(f"cannot read {path.name}: {error}")
        return []
    return [line.strip() for line in raw_lines if line.strip() and not line.lstrip().startswith("#")]


def check_macro_order(path: Path, errors: list[str], *, allow_classpaths: bool) -> None:
    lines = meaningful_hxml_lines(path, errors)
    bootstrap = "--macro reflaxe.c.CompilerBootstrap.Start()"
    initialize = "--macro reflaxe.c.CompilerInit.Start()"
    if lines.count(bootstrap) != 1 or lines.count(initialize) != 1:
        errors.append(f"{path.name} must invoke bootstrap and init exactly once")
    elif lines.index(bootstrap) >= lines.index(initialize):
        errors.append(f"{path.name} must invoke CompilerBootstrap before CompilerInit")
    if not allow_classpaths and any(line in ("-cp", "-p") or line.startswith(("-cp ", "-p ")) for line in lines):
        errors.append(f"{path.name} must not contain caller-relative classpaths")


def command_output(command: list[str], errors: list[str], root: Path) -> str:
    local_tool = root / "node_modules/.bin" / command[0]
    executable = str(local_tool) if local_tool.is_file() else shutil.which(command[0])
    if executable is None:
        errors.append(f"required tool is unavailable: {command[0]}")
        return ""
    result = subprocess.run(
        [executable, *command[1:]],
        cwd=root,
        check=False,
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        errors.append(f"{' '.join(command)} failed with exit code {result.returncode}")
        return ""
    return result.stdout.strip()


def validate(root: Path, *, require_tools: bool) -> list[str]:
    errors: list[str] = []
    lock = load_json(root / "docs/specs/toolchain-lock.json", errors)
    haxerc = load_json(root / ".haxerc", errors)
    haxelib = load_json(root / "haxelib.json", errors)
    package = load_json(root / "package.json", errors)
    package_lock = load_json(root / "package-lock.json", errors)

    if lock.get("schemaVersion") != 1:
        errors.append("toolchain-lock schemaVersion must be 1")

    haxe = lock.get("haxe", {})
    if not isinstance(haxe, dict) or haxe.get("version") != HAXE_VERSION:
        errors.append(f"toolchain lock must pin Haxe {HAXE_VERSION}")
    if not isinstance(haxe, dict) or haxe.get("sourceRevision") != HAXE_REVISION:
        errors.append("toolchain lock must pin the Haxe 5 preview tag revision")
    if not isinstance(haxe, dict) or haxe.get("releaseChannel") != "preview":
        errors.append("toolchain lock must identify the accepted Haxe preview channel")
    if not isinstance(haxe, dict) or haxe.get("artifacts") != HAXE_ARTIFACTS:
        errors.append("toolchain lock Haxe artifact digests do not match the official release")
    if haxerc != {"version": HAXE_VERSION, "resolveLibs": "scoped"}:
        errors.append(".haxerc must pin Haxe 5.0.0-preview.1 with scoped library resolution")

    reflaxe = lock.get("reflaxe", {})
    expected_reflaxe = {
        "version": REFLAXE_VERSION,
        "sourceRevision": REFLAXE_REVISION,
        "gitTree": REFLAXE_GIT_TREE,
    }
    if not isinstance(reflaxe, dict):
        errors.append("toolchain lock reflaxe entry must be an object")
        reflaxe = {}
    for field, expected in expected_reflaxe.items():
        if reflaxe.get(field) != expected:
            errors.append(f"toolchain lock reflaxe.{field} must equal {expected}")

    bundle_root = safe_child(root, reflaxe.get("bundleRoot"), "reflaxe.bundleRoot", errors)
    if bundle_root is not None:
        actual_digest, actual_count = bundle_digest(bundle_root, reflaxe.get("bundlePaths"), errors)
        expected_digest = reflaxe.get("bundleSha256")
        if not isinstance(expected_digest, str) or not SHA256_RE.fullmatch(expected_digest):
            errors.append("reflaxe.bundleSha256 must be a lowercase SHA-256 digest")
        elif actual_digest != expected_digest:
            errors.append("vendored Reflaxe bundle checksum does not match the lock")
        if reflaxe.get("bundleFileCount") != actual_count:
            errors.append("vendored Reflaxe bundle file count does not match the lock")
        vendor_manifest = load_json(bundle_root / "haxelib.json", errors)
        if vendor_manifest.get("name") != "reflaxe" or vendor_manifest.get("version") != REFLAXE_VERSION:
            errors.append("vendored Reflaxe haxelib metadata does not match the pin")

    if haxelib.get("name") != "reflaxe.c":
        errors.append("haxelib.json name must be reflaxe.c")
    if haxelib.get("license") != "GPL-3.0-only":
        errors.append("haxelib.json license must be GPL-3.0-only")
    if haxelib.get("classPath") != "src":
        errors.append("haxelib.json classPath must be src")
    if haxelib.get("dependencies") != {}:
        errors.append("haxelib.json must not float an external Reflaxe dependency")
    expected_target = {"name": "C", "abbv": "c", "stdPaths": ["std", "std/c/_std"]}
    if haxelib.get("reflaxe") != expected_target:
        errors.append("haxelib.json reflaxe target metadata does not match the C contract")

    dev_dependencies = package.get("devDependencies", {})
    if not isinstance(dev_dependencies, dict) or dev_dependencies.get("lix") != LIX_VERSION:
        errors.append(f"package.json must pin lix exactly to {LIX_VERSION}")
    scripts = package.get("scripts", {})
    if not isinstance(scripts, dict) or scripts.get("postinstall") != "lix download":
        errors.append("package.json postinstall must resolve the scoped Haxe toolchain with lix download")
    expected_all_sources_script = "python3 test/all_sources/run.py"
    expected_diagnostics_script = "python3 test/diagnostics/run.py"
    expected_c_ast_script = "python3 test/c_ast/run.py"
    expected_declaration_plan_script = "python3 test/declaration_plan/run.py"
    expected_symbol_registry_script = "python3 test/symbol_registry/run.py"
    expected_project_emitter_script = "python3 test/project_emitter/run.py"
    expected_runtime_features_script = "python3 test/runtime/runtime-feature-graph/run.py"
    expected_array_runtime_script = "python3 test/differential/array-runtime/run.py"
    expected_string_runtime_script = "python3 test/differential/string-runtime/run.py"
    expected_string_output_script = "python3 test/string_output/run.py"
    expected_hello_script = "python3 examples/hello/run.py"
    expected_hxc_ir_script = "python3 test/hxc_ir/run.py"
    expected_primitive_semantics_script = "python3 test/primitive_semantics/run.py"
    expected_stdlib_ledger_script = "python3 test/stdlib_ledger/run.py"
    expected_body_lowering_script = "python3 test/body_lowering/run.py"
    expected_function_lowering_script = "python3 test/function_lowering/run.py"
    expected_aggregate_lowering_script = "python3 test/aggregate_lowering/run.py"
    expected_class_layout_script = "python3 test/class_layout/run.py"
    expected_constructor_lowering_script = "python3 test/constructor_lowering/run.py"
    expected_virtual_dispatch_script = "python3 test/virtual_dispatch/run.py"
    expected_enum_lowering_script = "python3 test/enum_lowering/run.py"
    expected_generic_specialization_script = "python3 test/generic_specialization/run.py"
    expected_evaluation_order_script = "python3 test/evaluation_order/run.py"
    expected_static_initialization_script = "python3 test/static_initialization/run.py"
    expected_arithmetic_semantics_script = "python3 test/arithmetic_semantics/run.py"
    expected_primitive_differential_script = "python3 test/primitive_differential/run.py"
    expected_span_lowering_script = "python3 test/span_lowering/run.py"
    expected_typed_ast_script = "python3 test/typed_ast/run.py"
    expected_c_import_script = "python3 test/c_import/run.py"
    expected_snapshot_script = "python3 scripts/test/snapshots.py --check"
    expected_toolchain_script = (
        "npm run deps:verify && npm run test:beads-plan && npm run test:diagnostics && "
        "npm run test:all-sources && "
        "npm run test:bootstrap && npm run test:typed-c && npm run test:c-import && npm run test:typed-ast && npm run test:c-ast && "
        "npm run test:declaration-plan && npm run test:symbol-registry && npm run test:project-emitter && "
        "npm run test:runtime-features && npm run test:array-runtime && npm run test:string-runtime && npm run test:string-output && npm run test:hello && npm run test:hxc-ir && npm run test:primitive-semantics && "
        "npm run test:stdlib-ledger && "
        "npm run test:body-lowering && "
        "npm run test:function-lowering && npm run test:aggregate-lowering && npm run test:class-layout && npm run test:constructor-lowering && npm run test:virtual-dispatch && npm run test:enum-lowering && npm run test:generic-specialization && npm run test:evaluation-order && npm run test:static-initialization && "
        "npm run test:arithmetic-semantics && npm run test:primitive-differential && npm run test:span-lowering && npm run snapshots:check"
    )
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:all-sources") != expected_all_sources_script
    ):
        errors.append("package.json must retain the dedicated all-source Haxe gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:diagnostics") != expected_diagnostics_script
    ):
        errors.append("package.json must retain the typed diagnostic policy gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:toolchain") != expected_toolchain_script
    ):
        errors.append("package.json test:toolchain must run every required gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:c-ast") != expected_c_ast_script
    ):
        errors.append("package.json must retain the structural C AST golden gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:c-import") != expected_c_import_script
    ):
        errors.append("package.json must retain the generated direct C-import gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:declaration-plan") != expected_declaration_plan_script
    ):
        errors.append("package.json must retain the declaration planning gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:symbol-registry") != expected_symbol_registry_script
    ):
        errors.append("package.json must retain the deterministic symbol registry gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:project-emitter") != expected_project_emitter_script
    ):
        errors.append("package.json must retain the deterministic project emitter gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:runtime-features") != expected_runtime_features_script
    ):
        errors.append("package.json must retain the selective runtime feature gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:array-runtime") != expected_array_runtime_script
    ):
        errors.append("package.json must retain the typed array runtime gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:string-runtime") != expected_string_runtime_script
    ):
        errors.append("package.json must retain the UTF-8 scalar string runtime gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:string-output") != expected_string_output_script
    ):
        errors.append("package.json must retain the generated literal-output gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:hello") != expected_hello_script
    ):
        errors.append("package.json must retain the generated hello example gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:hxc-ir") != expected_hxc_ir_script
    ):
        errors.append("package.json must retain the HxcIR semantic gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:primitive-semantics")
        != expected_primitive_semantics_script
    ):
        errors.append("package.json must retain the primitive semantic gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:stdlib-ledger") != expected_stdlib_ledger_script
    ):
        errors.append("package.json must retain the pinned stdlib ledger gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:body-lowering") != expected_body_lowering_script
    ):
        errors.append("package.json must retain the typed body-lowering gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:function-lowering") != expected_function_lowering_script
    ):
        errors.append("package.json must retain the typed function-lowering gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:aggregate-lowering") != expected_aggregate_lowering_script
    ):
        errors.append("package.json must retain the closed aggregate-lowering gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:class-layout") != expected_class_layout_script
    ):
        errors.append("package.json must retain the concrete class-layout gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:constructor-lowering")
        != expected_constructor_lowering_script
    ):
        errors.append("package.json must retain the bounded constructor-lowering gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:virtual-dispatch") != expected_virtual_dispatch_script
    ):
        errors.append("package.json must retain the virtual-dispatch gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:enum-lowering") != expected_enum_lowering_script
    ):
        errors.append("package.json must retain the Haxe enum-lowering gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:generic-specialization")
        != expected_generic_specialization_script
    ):
        errors.append("package.json must retain the generic-specialization gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:evaluation-order") != expected_evaluation_order_script
    ):
        errors.append("package.json must retain the explicit evaluation-order gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:static-initialization")
        != expected_static_initialization_script
    ):
        errors.append("package.json must retain the deterministic static-initialization gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:arithmetic-semantics")
        != expected_arithmetic_semantics_script
    ):
        errors.append("package.json must retain the primitive arithmetic semantic gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:primitive-differential")
        != expected_primitive_differential_script
    ):
        errors.append("package.json must retain the seeded primitive differential gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:span-lowering") != expected_span_lowering_script
    ):
        errors.append("package.json must retain the fixed-array/span lowering gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("test:typed-ast") != expected_typed_ast_script
    ):
        errors.append("package.json must retain the typed-AST normalization gate")
    if (
        not isinstance(scripts, dict)
        or scripts.get("snapshots:check") != expected_snapshot_script
    ):
        errors.append("package.json must retain the central snapshot check gate")

    root_package = package_lock.get("packages", {}).get("", {}) if isinstance(package_lock.get("packages"), dict) else {}
    locked_lix = package_lock.get("packages", {}).get("node_modules/lix", {}) if isinstance(package_lock.get("packages"), dict) else {}
    if not isinstance(root_package, dict) or root_package.get("devDependencies", {}).get("lix") != LIX_VERSION:
        errors.append("package-lock root must retain the exact lix dependency")
    if not isinstance(locked_lix, dict) or locked_lix.get("version") != LIX_VERSION:
        errors.append("package-lock must resolve the pinned lix version")
    if not isinstance(locked_lix, dict) or locked_lix.get("integrity") != LIX_INTEGRITY:
        errors.append("package-lock lix integrity does not match the reviewed npm artifact")

    reflaxe_hxml = meaningful_hxml_lines(root / "haxe_libraries/reflaxe.hxml", errors)
    expected_framework_hxml = [
        "-cp ${SCOPE_DIR}/vendor/reflaxe/src/",
        f"-D reflaxe={REFLAXE_VERSION}",
    ]
    if reflaxe_hxml != expected_framework_hxml:
        errors.append("haxe_libraries/reflaxe.hxml must resolve only the pinned vendored framework")

    target_hxml = root / "haxe_libraries/reflaxe.c.hxml"
    target_lines = meaningful_hxml_lines(target_hxml, errors)
    if "-cp ${SCOPE_DIR}/src/" not in target_lines or "-lib reflaxe" not in target_lines:
        errors.append("reflaxe.c source library must use scoped target sources and the pinned Reflaxe library")
    for line in target_lines:
        if line.startswith(("-cp ", "-p ")) and "${SCOPE_DIR}" not in line:
            errors.append("reflaxe.c source library contains a caller-relative classpath")
    check_macro_order(target_hxml, errors, allow_classpaths=True)
    check_macro_order(root / "extraParams.hxml", errors, allow_classpaths=False)

    all_sources_hxml = root / "test/all_sources/all_sources.hxml"
    expected_all_sources_hxml = [
        "-cp test/all_sources/",
        "-lib reflaxe.c",
        '--macro include("reflaxe.c", true)',
        '--macro include("c", true)',
        "-main AllSourcesProbe",
    ]
    if meaningful_hxml_lines(all_sources_hxml, errors) != expected_all_sources_hxml:
        errors.append(
            "all_sources.hxml must include every reflaxe.c and c module through the scoped target library"
        )
    if not (root / "test/all_sources/run.py").is_file():
        errors.append("dedicated all-source Haxe gate runner is missing")

    custom_target_files = {
        root / "src/c/Init.hx": (
            "class Init",
            "reflaxe.c.TargetPlatform.configure()",
        ),
        root / "src/reflaxe/c/TargetPlatform.hx": (
            "Compiler.setPlatformConfiguration(config)",
            "staticTypeSystem: true",
            "usesUtf16: false",
            "supportsUnicode: true",
            "supportsThreads: false",
            "supportsAtomics: false",
        ),
    }
    for path, required_snippets in custom_target_files.items():
        try:
            contents = path.read_text(encoding="utf-8")
        except (OSError, UnicodeError) as error:
            errors.append(f"cannot read {path.relative_to(root)}: {error}")
            continue
        for snippet in required_snippets:
            if snippet not in contents:
                errors.append(
                    f"{path.relative_to(root)} must retain custom-target contract `{snippet}`"
                )

    if require_tools:
        if command_output(["haxe", "-version"], errors, root) != HAXE_VERSION:
            errors.append(f"active Haxe must be exactly {HAXE_VERSION}")
        if command_output(["lix", "--version"], errors, root) != LIX_VERSION:
            errors.append(f"active lix must be exactly {LIX_VERSION}")

    return errors


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[2],
        help="repository root",
    )
    parser.add_argument("--require-tools", action="store_true")
    parser.add_argument("--print-bundle-digest", action="store_true")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    root = args.root.resolve()
    if args.print_bundle_digest:
        lock = load_json(root / "docs/specs/toolchain-lock.json", [])
        reflaxe = lock.get("reflaxe", {})
        bundle_root = root / reflaxe.get("bundleRoot", "vendor/reflaxe")
        digest, count = bundle_digest(bundle_root, reflaxe.get("bundlePaths"), [])
        print(f"{digest} {count}")
        return 0

    errors = validate(root, require_tools=args.require_tools)
    if errors:
        for error in errors:
            print(f"toolchain-policy: ERROR: {error}", file=sys.stderr)
        return 1
    print("toolchain-policy: OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

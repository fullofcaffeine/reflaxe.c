#!/usr/bin/env python3
"""Prove pinned Vector and List behavior through Eval, generated C, and native tools."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402
from scripts.test.compiler_discovery import resolve_compiler  # noqa: E402

CASE = Path(__file__).resolve().parent
GENERATED = CASE / "generated"
LAYOUTS = ("split", "package", "unity")
TOOLCHAINS = ("gcc", "clang")
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)
SANITIZER_FLAGS = (
    "-O1",
    "-g",
    "-fno-omit-frame-pointer",
    "-fno-sanitize-recover=all",
    "-fsanitize=address,undefined",
)
EXPECTED_BASES = {
    "constructor.haxe.ds.List",
    "constructor.haxe.ds._List.ListNode",
    "function.haxe.ds._Vector.Vector_Impl_.blit",
    "function.haxe.ds._Vector.Vector_Impl_.toArray",
    "method.haxe.ds.List.add",
    "method.haxe.ds.List.clear",
    "method.haxe.ds.List.filter",
    "method.haxe.ds.List.first",
    "method.haxe.ds.List.isEmpty",
    "method.haxe.ds.List.join",
    "method.haxe.ds.List.last",
    "method.haxe.ds.List.map",
    "method.haxe.ds.List.pop",
    "method.haxe.ds.List.push",
    "method.haxe.ds.List.remove",
    "method.haxe.ds.List.toString",
}


class VectorListFailure(RuntimeError):
    """One bounded Vector/List contract failed."""


@dataclass(frozen=True)
class Toolchain:
    family: str
    compiler: str
    cpp_compiler: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    environment["LC_ALL"] = "C"
    return environment


def resolve_toolchains(selected: str) -> list[Toolchain]:
    families = TOOLCHAINS if selected == "auto" else (selected,)
    result: list[Toolchain] = []
    for family in families:
        compiler = resolve_compiler(family)
        cpp_compiler = resolve_compiler(family, "c++")
        if compiler is None or cpp_compiler is None:
            if selected != "auto":
                raise VectorListFailure(
                    f"required identity-matching C/C++ compilers are missing: {family}"
                )
            print(
                f"vector-list: SKIP optional {family}: "
                "missing identity-matching C/C++ commands"
            )
            continue
        result.append(Toolchain(family, compiler, cpp_compiler))
    if not result:
        raise VectorListFailure("no strict C11 compiler is available")
    return result


def run_eval_oracle() -> None:
    observations: list[tuple[int, str, str]] = []
    for _ in range(2):
        result = run_bounded_process(
            [development_tool("haxe"), "oracle.hxml"],
            cwd=GENERATED,
            env=haxe_environment(),
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        observations.append((result.returncode, result.stdout, result.stderr))
    if observations != [(0, "", ""), (0, "", "")]:
        raise VectorListFailure(f"pinned Eval Vector/List oracle drifted: {observations!r}")


def compile_haxe(
    output: Path,
    *,
    layout: str = "split",
    reverse: bool = False,
    report: bool = False,
    defines: tuple[str, ...] = (),
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(GENERATED),
        "-lib",
        "reflaxe.c",
        "-main",
        "Main",
        "-D",
        f"hxc_project_layout={layout}",
    ]
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_static_initialization_report"])
    for define in defines:
        command.extend(["-D", define])
    command.extend(["--custom-target", f"c={output}"])
    return run_bounded_process(
        command,
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=300,
    )


def generated_tree(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def extract_hxcir(result: subprocess.CompletedProcess[str]) -> str:
    reports = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(reports) != 1:
        raise VectorListFailure("generated compile omitted its one HxcIR report")
    report = json.loads(reports[0])
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise VectorListFailure("generated compile omitted validated HxcIR text")
    return hxcir


def require_dict(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise VectorListFailure(f"{label} is not an object")
    return value


def require_list(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise VectorListFailure(f"{label} is not an array")
    return value


def emitted_function_definition(output: Path, c_name: str) -> bytes:
    pattern = re.compile(rb"(?m)^[^\n]*\b" + re.escape(c_name.encode()) + rb"\([^\n]*\)\n\{")
    matches: list[tuple[bytes, re.Match[bytes]]] = []
    for path in sorted((output / "src").rglob("*.c")):
        content = path.read_bytes()
        matches.extend((content, match) for match in pattern.finditer(content))
    if len(matches) != 1:
        raise VectorListFailure(f"specialized C definition {c_name!r} appeared {len(matches)} times")
    content, match = matches[0]
    depth = 0
    for index in range(match.end() - 1, len(content)):
        if content[index] == ord("{"):
            depth += 1
        elif content[index] == ord("}"):
            depth -= 1
            if depth == 0:
                return content[match.start() : index + 1]
    raise VectorListFailure(f"specialized C definition {c_name!r} has no closing brace")


def validate_specializations(output: Path) -> dict[str, object]:
    report = require_dict(
        json.loads((output / "hxc.specializations.json").read_text(encoding="utf-8")),
        "specialization report",
    )
    if (
        report.get("schemaVersion") != 2
        or report.get("algorithm") != "hxc-generic-specialization-v2"
        or report.get("status") != "analyzed-closed-specializations"
        or report.get("keyEncoding") != "length-prefixed-utf8-full-semantic-key"
        or report.get("compactNameDigest") != "sha256-with-full-key-collision-check"
    ):
        raise VectorListFailure("generic specialization report contract drifted")
    limits = require_dict(report.get("limits"), "specialization limits")
    if limits != {
        "maxFunctionSpecializations": 64,
        "maxTypeSpecializations": 64,
        "maxEstimatedSpecializationCBytes": 524288,
    }:
        raise VectorListFailure("generic specialization hard budgets drifted")
    functions = require_list(report.get("functionSpecializations"), "function specializations")
    types = require_list(report.get("typeSpecializations"), "type specializations")
    if len(functions) != 22 or types:
        raise VectorListFailure("pinned Vector/List specialization count drifted")

    keys: list[str] = []
    bases: set[str] = set()
    representations: set[str] = set()
    function_bytes = 0
    by_base: dict[str, list[dict[str, object]]] = {}
    for value in functions:
        record = require_dict(value, "function specialization")
        base = record.get("baseFunctionId")
        key = record.get("specializationKey")
        c_name = record.get("cName")
        if not isinstance(base, str) or not isinstance(key, str) or not isinstance(c_name, str):
            raise VectorListFailure("specialization identity fields are malformed")
        digest = hashlib.sha256(key.encode("utf-8")).hexdigest()
        prefix = "constructor" if base.startswith("constructor.") else "function"
        if (
            record.get("semanticDigestSha256") != digest
            or record.get("instanceId") != f"{prefix}.specialization.{digest}"
            or not key.startswith("generic-function-v1(")
            or not c_name.startswith("hxc_")
        ):
            raise VectorListFailure(f"specialization full-key identity drifted for {base}")
        arguments = require_list(record.get("arguments"), f"{base} arguments")
        if not arguments:
            raise VectorListFailure(f"{base} lost its closed arguments")
        for argument_value in arguments:
            argument = require_dict(argument_value, f"{base} argument")
            representation = argument.get("representation")
            if representation not in {"direct-primitive", "managed-class"}:
                raise VectorListFailure(f"{base} selected unsupported argument {argument!r}")
            if not isinstance(argument.get("key"), str) or not isinstance(argument.get("parameter"), str):
                raise VectorListFailure(f"{base} argument identity is malformed")
            representations.add(str(representation))
        cost = require_dict(record.get("codeSize"), f"{base} code size")
        definition = emitted_function_definition(output, c_name)
        if (
            cost.get("metric") != "strict-c11-utf8-function-definition-bytes"
            or cost.get("definitionBytes") != len(definition)
            or cost.get("definitionSha256") != hashlib.sha256(definition).hexdigest()
        ):
            raise VectorListFailure(f"{base} code-size address drifted")
        function_bytes += len(definition)
        bases.add(base)
        keys.append(key)
        by_base.setdefault(base, []).append(record)
    if keys != sorted(keys) or len(set(keys)) != len(keys):
        raise VectorListFailure("specialization keys are not sorted and unique")
    if bases != EXPECTED_BASES or representations != {"direct-primitive", "managed-class"}:
        raise VectorListFailure("pinned Vector/List specialization surface drifted")

    def argument_keys(base: str) -> set[tuple[str, ...]]:
        return {
            tuple(str(require_dict(value, "argument").get("key")) for value in require_list(record.get("arguments"), "arguments"))
            for record in by_base.get(base, [])
        }

    if argument_keys("constructor.haxe.ds.List") != {("i32",), ("class(4:Node0:)",)}:
        raise VectorListFailure("List constructors lost primitive or managed ownership")
    if argument_keys("constructor.haxe.ds._List.ListNode") != {("i32",), ("class(4:Node0:)",)}:
        raise VectorListFailure("ListNode constructors lost primitive or managed ownership")
    if argument_keys("function.haxe.ds._Vector.Vector_Impl_.blit") != {("i32",), ("class(4:Node0:)",)}:
        raise VectorListFailure("Vector.blit lost primitive or managed closure")
    if argument_keys("method.haxe.ds.List.map") != {("i32", "i32")}:
        raise VectorListFailure("List.map lost owner-first method specialization order")

    summary = require_dict(report.get("summary"), "specialization summary")
    if (
        summary.get("functionSpecializations") != 22
        or summary.get("typeSpecializations") != 0
        or summary.get("recursiveSpecializations") != 0
        or summary.get("specializedFunctionDefinitionBytes") != function_bytes
        or summary.get("estimatedSpecializationCBytes") != function_bytes
    ):
        raise VectorListFailure("specialization summary or budget accounting drifted")
    return report


def validate_generated_project(output: Path, hxcir: str) -> dict[str, object]:
    for marker in (
        'representation=managed("gc")',
        'representation=managed("array")',
        'runtime(feature="array",operation="resize-default")',
        'runtime(feature="array",operation="get-checked")',
        'runtime(feature="array",operation="set")',
        'runtime(feature="array",operation="sort")',
        'managed-root "root.',
    ):
        if marker not in hxcir:
            raise VectorListFailure(f"validated HxcIR omitted {marker}")
    if " raw" in hxcir or str(ROOT) in hxcir:
        raise VectorListFailure("Vector/List HxcIR used raw syntax or leaked the checkout path")

    specialization_report = validate_specializations(output)
    plan = require_dict(
        json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8")),
        "runtime plan",
    )
    expected_features = [
        "runtime-base",
        "status",
        "alloc",
        "array",
        "object",
        "gc",
        "string-literal",
        "string-scalar",
        "string",
    ]
    if plan.get("features") != expected_features:
        raise VectorListFailure(f"Vector/List runtime closure drifted: {plan.get('features')!r}")
    decisions = set(require_list(plan.get("directDecisions"), "runtime direct decisions"))
    required_decisions = {
        "closed-generic-specializations",
        "concrete-class-reference-layouts",
        "direct-calls",
        "exact-traced-haxe-object-graph",
        "managed-haxe-arrays",
    }
    if not required_decisions.issubset(decisions):
        raise VectorListFailure("runtime plan omitted closed collection ownership decisions")
    if any("iterator" in str(decision) for decision in decisions):
        raise VectorListFailure("source-level List traversal selected iterator runtime machinery")

    stdlib = require_dict(
        json.loads((output / "hxc.stdlib-report.json").read_text(encoding="utf-8")),
        "stdlib report",
    )
    if stdlib.get("modules") != ["Array", "String", "gc", "string"]:
        raise VectorListFailure("stdlib report did not retain source-owned List and Vector")
    expected_capabilities = {
        "allocation",
        "class-object-header",
        "cleanup-release",
        "concat",
        "create-literal",
        "from-int",
        "get-checked",
        "length",
        "managed-type-representation",
        "resize-default",
        "retain",
        "root-frame",
        "set",
        "sort",
        "static-value",
    }
    if set(require_list(stdlib.get("capabilities"), "stdlib capabilities")) != expected_capabilities:
        raise VectorListFailure("stdlib report capability closure drifted")

    manifest = require_dict(
        json.loads((output / "hxc.manifest.json").read_text(encoding="utf-8")),
        "manifest",
    )
    build = require_dict(manifest.get("build"), "manifest build")
    artifact = require_dict(build.get("artifact"), "manifest artifact")
    if build.get("publicHeaders") != [] or artifact.get("kind") != "executable":
        raise VectorListFailure("Vector/List fixture unexpectedly exported a public ABI")

    application = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    if len(re.findall(r"struct hxc_haxe_ds_List_h[0-9a-f]+ \{", headers)) != 2:
        raise VectorListFailure("generated C lost its two exact List layouts")
    if len(re.findall(r"struct hxc_haxe_ds_List_ListNode_h[0-9a-f]+ \{", headers)) != 2:
        raise VectorListFailure("generated C lost its two exact ListNode layouts")
    for marker in (
        "hxc_haxe_ds_Vector_Vector_Impl_blit_",
        "hxc_haxe_ds_Vector_Vector_Impl_toArray",
        "hxc_array_ref_resize_default",
        "hxc_array_ref_get_copy",
        "hxc_array_ref_set_copy",
        "hxc_array_ref_sort",
        "hxc_gc_allocate",
        "HXC_TYPE_DESCRIPTOR_HAS_TRACE",
    ):
        if marker not in application:
            raise VectorListFailure(f"generated C omitted {marker}")
    for forbidden in (
        "hxc_dynamic",
        "hxc_iterator",
        "struct hxc_haxe_ds_Vector_",
        "runtime/include/hxrt/vector.h",
    ):
        if forbidden in application or forbidden in headers:
            raise VectorListFailure(f"generated C retained forbidden shape {forbidden!r}")
    return specialization_report


def render_projects(root: Path) -> tuple[dict[str, Path], Path]:
    projects: dict[str, Path] = {}
    split = root / "split"
    split_result = compile_haxe(split, report=True)
    if split_result.returncode != 0:
        raise VectorListFailure(f"split compile failed: {split_result.stderr!r}")
    projects["split"] = split
    reference_report = validate_generated_project(split, extract_hxcir(split_result))

    reverse = root / "split-reverse"
    reverse_result = compile_haxe(reverse, reverse=True)
    if reverse_result.returncode != 0:
        raise VectorListFailure(f"reverse-order split compile failed: {reverse_result.stderr!r}")
    if generated_tree(split) != generated_tree(reverse):
        raise VectorListFailure("split output changed under reversed typed-module discovery")

    for layout in ("package", "unity"):
        output = root / layout
        result = compile_haxe(output, layout=layout)
        if result.returncode != 0:
            raise VectorListFailure(f"{layout} compile failed: {result.stderr!r}")
        projects[layout] = output
        validate_specializations(output)

    metal = root / "metal"
    metal_result = compile_haxe(metal, defines=("reflaxe_c_profile=metal",))
    if metal_result.returncode != 0:
        raise VectorListFailure(f"metal profile compile failed: {metal_result.stderr!r}")
    metal_report = validate_specializations(metal)
    if metal_report.get("functionSpecializations") != reference_report.get("functionSpecializations"):
        raise VectorListFailure("portable and metal specialization identities diverged")

    rejected = root / "runtime-none"
    rejected_result = compile_haxe(rejected, defines=("hxc_runtime=none",))
    if rejected_result.returncode == 0 or "runtime policy `none`" not in rejected_result.stderr:
        raise VectorListFailure("runtime policy none did not reject managed Vector/List storage")
    if rejected.exists() and any(rejected.rglob("*")):
        raise VectorListFailure("runtime-policy rejection left plausible generated output")
    return projects, metal


def project_build_inputs(project: Path) -> tuple[list[Path], list[Path]]:
    manifest = require_dict(
        json.loads((project / "hxc.manifest.json").read_text(encoding="utf-8")),
        "manifest",
    )
    build = require_dict(manifest.get("build"), "manifest build")
    sources = [project / str(value) for value in require_list(build.get("sources"), "build sources")]
    includes = [project / str(value) for value in require_list(build.get("includeDirectories"), "include directories")]
    return sources, includes


def compile_and_run(
    compiler: str,
    project: Path,
    executable: Path,
    flags: tuple[str, ...],
) -> None:
    sources, includes = project_build_inputs(project)
    command = [
        compiler,
        *STRICT_FLAGS,
        *flags,
        *(f"-I{include}" for include in includes),
        *(str(source) for source in sources),
        "-o",
        str(executable),
    ]
    compiled = run_bounded_process(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=120,
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise VectorListFailure(
            f"strict native compile failed\ncommand={command!r}\n"
            f"stdout={compiled.stdout!r}\nstderr={compiled.stderr!r}"
        )
    executed = run_bounded_process(
        [str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if executed.returncode != 0 or executed.stdout or executed.stderr:
        raise VectorListFailure(
            f"native execution drifted: exit={executed.returncode} "
            f"stdout={executed.stdout!r} stderr={executed.stderr!r}"
        )


def validate_cpp_headers(project: Path, family: str, compiler: str, root: Path) -> None:
    source = root / f"{family}-headers.cpp"
    source.write_text('#include "hxc/program.h"\nint main() { return 0; }\n', encoding="utf-8")
    command = [
        compiler,
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-pedantic",
        f"-I{project / 'include'}",
        f"-I{project / 'runtime/include'}",
        "-fsyntax-only",
        str(source),
    ]
    result = run_bounded_process(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise VectorListFailure(f"{family} C++ private-header check failed: {result.stderr!r}")


def inspect_symbols(executable: Path, family: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise VectorListFailure(f"{family} Vector/List evidence requires nm")
    result = run_bounded_process(
        [nm, str(executable)],
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if result.returncode != 0:
        raise VectorListFailure(f"{family} could not inspect Vector/List symbols")
    for required in (
        "hxc_array_ref_get_copy",
        "hxc_array_ref_set_copy",
        "hxc_array_ref_resize_default",
        "hxc_gc_allocate",
        "hxc_haxe_ds_List_add_",
        "hxc_haxe_ds_Vector_Vector_Impl_blit_",
    ):
        if required not in result.stdout:
            raise VectorListFailure(f"{family} omitted required symbol {required}")
    for forbidden in ("hxc_dynamic", "hxc_iterator", "hxc_typed_map"):
        if forbidden in result.stdout:
            raise VectorListFailure(f"{family} retained unrelated symbol family {forbidden}")


def run_native(toolchains: list[Toolchain]) -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-vector-list-") as temporary:
        root = Path(temporary)
        projects, metal = render_projects(root)
        for toolchain in toolchains:
            build = root / f"native-{toolchain.family}"
            build.mkdir()
            validate_cpp_headers(
                projects["split"], toolchain.family, toolchain.cpp_compiler, build
            )
            for layout, project in projects.items():
                executable = build / layout
                compile_and_run(
                    toolchain.compiler,
                    project,
                    executable,
                    ("-O2" if layout == "unity" else "-O0",),
                )
                inspect_symbols(executable, toolchain.family)
            compile_and_run(toolchain.compiler, metal, build / "metal", ("-O0",))
            compile_and_run(
                toolchain.compiler,
                projects["split"],
                build / "sanitized",
                SANITIZER_FLAGS,
            )


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", *TOOLCHAINS), default="auto")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        toolchains = resolve_toolchains(args.toolchain)
        run_eval_oracle()
        run_native(toolchains)
    except (
        VectorListFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"vector-list: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    print(
        "vector-list: OK: "
        f"{families}; Eval/generated-C Vector and List parity, primitive/managed "
        "ownership, mutation traversal, Array carrier, closed generic identities, "
        "layouts, determinism, C++, sanitizers, runtime plans, and symbols passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

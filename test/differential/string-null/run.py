#!/usr/bin/env python3
"""Prove nullable and switch-based Haxe String semantics against Eval and C."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[3]
CASE = Path(__file__).resolve().parent
GENERATED = CASE / "generated"
NEGATIVE = CASE / "negative"
LAYOUTS = ("split", "package", "unity")
TOOLCHAINS = ("gcc", "clang")
EXPECTED_STDOUT = "string-null: OK\n"
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
CPP_STRICT_FLAGS = (
    "-std=c++17",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
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


class StringNullFailure(RuntimeError):
    """Reports a failed nullable-String semantic or build invariant."""


@dataclass(frozen=True)
class Toolchain:
    """Pairs one required compiler family with its resolved C executable."""

    family: str
    compiler: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return environment


def resolve_toolchains(selected: str) -> list[Toolchain]:
    families = TOOLCHAINS if selected == "auto" else (selected,)
    values: list[Toolchain] = []
    for family in families:
        compiler = shutil.which(family)
        if compiler is None:
            if selected != "auto":
                raise StringNullFailure(f"required C compiler is missing: {family}")
            print(f"string-null: SKIP optional {family}: missing command")
            continue
        identity = subprocess.run(
            [compiler, "--version"],
            check=False,
            capture_output=True,
            text=True,
            timeout=10,
        )
        version = (identity.stdout + identity.stderr).lower()
        actual = (
            "clang"
            if "clang" in version
            else "gcc"
            if "gcc" in version or "free software foundation" in version
            else "unknown"
        )
        if identity.returncode != 0 or actual != family:
            if selected != "auto":
                raise StringNullFailure(f"{family} command identifies as {actual}")
            print(f"string-null: SKIP optional {family}: command identifies as {actual}")
            continue
        values.append(Toolchain(family, compiler))
    if not values:
        raise StringNullFailure("no identity-matching strict C11 compiler is available")
    return values


def run_eval_oracle() -> None:
    for iteration in range(2):
        result = subprocess.run(
            [development_tool("haxe"), "oracle.hxml"],
            cwd=GENERATED,
            env=haxe_environment(),
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if (result.returncode, result.stdout, result.stderr) != (
            0,
            EXPECTED_STDOUT,
            "",
        ):
            raise StringNullFailure(
                f"pinned Eval nullable-String oracle drifted on run {iteration}: "
                f"{result.returncode}, {result.stdout!r}, {result.stderr!r}"
            )


def compile_haxe(
    fixture: Path,
    output: Path,
    *,
    layout: str = "split",
    reverse: bool = False,
    report: bool = False,
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        "Main",
        "-D",
        f"hxc_project_layout={layout}",
        "-D",
        "hxc_runtime_diagnostics=off",
    ]
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_static_initialization_report"])
    command.extend(["--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=45,
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
        raise StringNullFailure("nullable-String compile omitted its HxcIR report")
    value = json.loads(reports[0])
    hxcir = value.get("hxcir") if isinstance(value, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise StringNullFailure("nullable-String report omitted validated HxcIR")
    return hxcir


def hxcir_function(hxcir: str, function_id: str) -> str:
    """Return one printed HxcIR function so structural checks stay local."""
    marker = f'function "{function_id}"'
    start = hxcir.find(marker)
    if start < 0:
        raise StringNullFailure(f"validated HxcIR omitted {function_id}")
    end = hxcir.find(f'end function "{function_id}"', start)
    if end < 0:
        raise StringNullFailure(f"validated HxcIR truncated {function_id}")
    return hxcir[start:end]


def generated_c_function(source: str, function_name: str) -> str:
    """Return one generated C function body for focused syntax assertions."""
    marker = f"int32_t hxc_Main_{function_name}("
    start = source.find(marker)
    if start < 0:
        raise StringNullFailure(f"generated C omitted {function_name}")
    end = source.find("\n}\n", start)
    if end < 0:
        raise StringNullFailure(f"generated C truncated {function_name}")
    return source[start : end + 3]


def validate_project(output: Path, hxcir: str) -> None:
    for required in (
        'function "function.Main.choose"',
        'returns=string-utf8',
        'result="value.1":string-utf8 constant value=null',
        'function "function.Main.identity"',
        'parameter "parameter.0" type=string-utf8',
        'operation="haxe.string.equal"',
        'operation="haxe.string.not-equal"',
        'function "function.Main.commandValue"',
        'function "function.Main.commandStatement"',
        'function "function.Main.exhaustiveCommandValue"',
        'operation="haxe.string.equal.right-non-null"',
        'string-utf8(bytes=5,value="start")',
        'string-utf8(bytes=4,value="stop")',
        'string-utf8(bytes=0,value="")',
    ):
        if required not in hxcir:
            raise StringNullFailure(f"validated HxcIR omitted {required!r}")
    if "nullable(" in hxcir or "nullable-inject" in hxcir:
        raise StringNullFailure("reference-nullable String gained a redundant tagged wrapper")

    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if plan.get("features") != [
        "runtime-base",
        "status",
        "string-literal",
        "io",
    ]:
        raise StringNullFailure("nullable String selected an unrelated runtime slice")
    source = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "src").rglob("*.c"))
    )
    if "memcmp(" not in source or "goto " in source:
        raise StringNullFailure("nullable String C lost structural equality or added goto")
    if ".data == NULL" not in source or ".data != NULL" not in source:
        raise StringNullFailure("generated C does not keep null distinct from empty")
    command_ir = hxcir_function(hxcir, "function.Main.commandValue")
    if (
        command_ir.count("record-field-project") != 1
        or command_ir.count('operation="haxe.string.equal.right-non-null"') != 2
        or "terminator switch value=" in command_ir
    ):
        raise StringNullFailure(
            "record-carried String switch lost one-time evaluation or equality-branch lowering"
        )
    exhaustive_ir = hxcir_function(
        hxcir, "function.Main.exhaustiveCommandValue"
    )
    if "terminator unreachable" not in exhaustive_ir:
        raise StringNullFailure(
            "default-free String enum abstract lost its forged-value fail-stop"
        )
    command_c = generated_c_function(source, "commandValue")
    if (
        command_c.count("memcmp(") != 2
        or "switch (" in command_c
        or "goto " in command_c
    ):
        raise StringNullFailure(
            "String switch did not become two readable content-comparison branches"
        )
    statement_c = generated_c_function(source, "commandStatement")
    if statement_c.count("memcmp(") != 2 or "switch (" in statement_c:
        raise StringNullFailure(
            "statement String switch did not use the shared content-comparison lowering"
        )


def render_projects(root: Path) -> dict[str, Path]:
    projects: dict[str, Path] = {}
    for layout in LAYOUTS:
        normal = root / f"{layout}-normal"
        reverse = root / f"{layout}-reverse"
        first = compile_haxe(GENERATED, normal, layout=layout, report=layout == "split")
        second = compile_haxe(GENERATED, reverse, layout=layout, reverse=True)
        for label, result in ((f"{layout}-normal", first), (f"{layout}-reverse", second)):
            if result.returncode != 0 or result.stderr:
                raise StringNullFailure(
                    f"{label} nullable-String compile failed\n"
                    f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
                )
        if generated_tree(normal) != generated_tree(reverse):
            raise StringNullFailure(
                f"{layout} nullable-String output changed under reversed discovery"
            )
        projects[layout] = normal
        if layout == "split":
            validate_project(normal, extract_hxcir(first))
    return projects


def validate_fail_closed(root: Path) -> None:
    output = root / "unsupported"
    result = compile_haxe(NEGATIVE, output)
    if (
        result.returncode == 0
        or "HXC1001:" not in result.stderr
        or "TCall(String.toUpperCase:not-yet-admitted)" not in result.stderr
    ):
        raise StringNullFailure(
            f"unsupported runtime-created String path did not fail closed: {result.stderr!r}"
        )
    if output.exists() and any(output.rglob("*")):
        raise StringNullFailure("unsupported nullable-String path left plausible output")


def string_list(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise StringNullFailure(f"{label} must be a string array")
    return list(value)


def project_build_inputs(project: Path) -> tuple[list[Path], list[Path]]:
    manifest = json.loads((project / "hxc.manifest.json").read_text(encoding="utf-8"))
    build = manifest.get("build")
    if not isinstance(build, dict):
        raise StringNullFailure("generated manifest omitted its build plan")
    sources = [(project / item).resolve() for item in string_list(build.get("sources"), "sources")]
    includes = [
        (project / item).resolve()
        for item in string_list(build.get("includeDirectories"), "include directories")
    ]
    for path in (*sources, *includes):
        path.relative_to(project.resolve())
        if not path.exists():
            raise StringNullFailure(f"generated build input is missing: {path}")
    return sources, includes


def compile_and_run(
    toolchain: Toolchain,
    project: Path,
    executable: Path,
    flags: tuple[str, ...],
) -> None:
    sources, includes = project_build_inputs(project)
    command = [
        toolchain.compiler,
        *STRICT_FLAGS,
        *flags,
        *(f"-I{root}" for root in includes),
        *(str(source) for source in sources),
        "-o",
        str(executable),
    ]
    compilation = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if compilation.returncode != 0 or compilation.stdout or compilation.stderr:
        raise StringNullFailure(
            f"{toolchain.family} strict nullable-String compile failed\n"
            f"command={command!r}\nstdout={compilation.stdout!r}\nstderr={compilation.stderr!r}"
        )
    execution = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if (
        execution.returncode != 0
        or execution.stdout != EXPECTED_STDOUT
        or execution.stderr
    ):
        raise StringNullFailure(
            f"{toolchain.family} nullable-String execution drifted: "
            f"{execution.returncode}, {execution.stdout!r}, {execution.stderr!r}"
        )


def validate_cpp_header(toolchain: Toolchain, project: Path, build: Path) -> None:
    compiler = shutil.which("clang++" if toolchain.family == "clang" else "g++")
    if compiler is None:
        raise StringNullFailure(f"{toolchain.family} evidence requires its C++ compiler")
    source = build / f"{project.name}-header.cpp"
    executable = build / f"{project.name}-header"
    source.write_text(
        '#include "hxc/program.h"\nint main() { return 0; }\n',
        encoding="utf-8",
    )
    command = [
        compiler,
        *CPP_STRICT_FLAGS,
        f"-I{project / 'include'}",
        f"-I{project / 'runtime/include'}",
        str(source),
        "-o",
        str(executable),
    ]
    result = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise StringNullFailure(
            f"{toolchain.family} C++17 header consumer failed: {command!r}\n"
            f"{result.stdout!r}\n{result.stderr!r}"
        )


def run_native(toolchains: list[Toolchain], projects: dict[str, Path], root: Path) -> None:
    for toolchain in toolchains:
        build = root / toolchain.family
        build.mkdir()
        for layout, project in projects.items():
            for optimization in ("-O0", "-O2"):
                compile_and_run(
                    toolchain,
                    project,
                    build / f"{layout}-{optimization[1:].lower()}",
                    (optimization,),
                )
            validate_cpp_header(toolchain, project, build)
        if toolchain.family == "clang":
            compile_and_run(
                toolchain,
                projects["split"],
                build / "split-sanitized",
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
        with tempfile.TemporaryDirectory(prefix="reflaxe-c-string-null-") as temporary:
            root = Path(temporary)
            projects = render_projects(root)
            validate_fail_closed(root)
            run_native(toolchains, projects, root)
    except (
        StringNullFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"string-null: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    print(
        "string-null: OK: "
        f"{families}; Eval parity, null/empty/content identity, calls, aliases, "
        "nominal abstracts, String switches, early returns, HxcIR, layouts, "
        "determinism, C11, C++17 headers, sanitizers, and negative diagnostics "
        "passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

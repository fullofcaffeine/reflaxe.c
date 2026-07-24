#!/usr/bin/env python3
"""Prove ordinary Haxe Bytes lowering and the independent native Bytes contract."""

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
FIXTURE = CASE / "bytes_runtime.c"
INCLUDE = ROOT / "runtime/hxrt/include"
RUNTIME_SOURCES = (
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/bytes.c",
)
TOOLCHAINS = ("gcc", "clang")
LAYOUTS = ("split", "unity")
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


class BytesRuntimeFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Toolchain:
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
    result: list[Toolchain] = []
    for family in families:
        compiler = shutil.which(family)
        if compiler is None:
            if selected != "auto":
                raise BytesRuntimeFailure(f"required C compiler is missing: {family}")
            print(f"bytes-runtime: SKIP optional {family}: missing command")
            continue
        identity = subprocess.run(
            [compiler, "--version"],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=10,
        )
        text = (identity.stdout + identity.stderr).lower()
        actual = "clang" if "clang" in text else "gcc" if "gcc" in text else "unknown"
        if identity.returncode != 0 or actual != family:
            if selected != "auto":
                raise BytesRuntimeFailure(f"{family} command identifies as {actual}")
            print(f"bytes-runtime: SKIP optional {family}: command identifies as {actual}")
            continue
        result.append(Toolchain(family, compiler))
    if not result:
        raise BytesRuntimeFailure("no strict C11 compiler is available")
    return result


def run_eval_oracle() -> None:
    outputs: list[tuple[int, str, str]] = []
    for _ in range(2):
        execution = subprocess.run(
            [development_tool("haxe"), "oracle.hxml"],
            cwd=GENERATED,
            env=haxe_environment(),
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        outputs.append((execution.returncode, execution.stdout, execution.stderr))
    if outputs != [(0, "", ""), (0, "", "")]:
        raise BytesRuntimeFailure(f"pinned Eval Bytes oracle drifted: {outputs!r}")


def compile_haxe(
    fixture: Path,
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
        str(fixture),
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
        raise BytesRuntimeFailure("generated compile omitted its one HxcIR report")
    report = json.loads(reports[0])
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise BytesRuntimeFailure("generated compile omitted validated HxcIR text")
    return hxcir


def hxcir_function(hxcir: str, function_id: str) -> str:
    """Return one complete function section from the deterministic text dump."""
    start_marker = f'  function "{function_id}"'
    end_marker = f'  end function "{function_id}"'
    start = hxcir.find(start_marker)
    if start < 0:
        raise BytesRuntimeFailure(f"validated HxcIR omitted {function_id}")
    end = hxcir.find(end_marker, start)
    if end < 0:
        raise BytesRuntimeFailure(f"validated HxcIR did not close {function_id}")
    return hxcir[start : end + len(end_marker)]


def validate_generated_project(output: Path, hxcir: str) -> None:
    for marker in (
        'representation=managed("bytes")',
        'runtime(feature="bytes",operation="alloc")',
        'runtime(feature="bytes",operation="of-string-utf8")',
        'runtime(feature="bytes",operation="blit")',
        'runtime(feature="bytes",operation="compare")',
        "bytes-temporary.local.",
        "static-call-argument-0-owner-initialize",
        "static-call-argument-0-borrow",
        "bytes-compare-argument-owner-initialize",
        "bytes-get-receiver-owner-initialize",
        'retain place=local(',
        'release place=local(',
    ):
        if marker not in hxcir:
            raise BytesRuntimeFailure(f"validated HxcIR omitted {marker}")
    if " raw" in hxcir or str(ROOT) in hxcir:
        raise BytesRuntimeFailure("Bytes HxcIR used raw syntax or leaked the checkout path")

    early_return = hxcir_function(hxcir, "function.Main.readFreshText")
    if (
        "bytes-temporary.local." not in early_return
        or "terminator return" not in early_return
        or "bytes-temporary.local." not in next(
            (
                line
                for line in early_return.splitlines()
                if "terminator return" in line and "bytes-temporary.local." in line
            ),
            "",
        )
    ):
        raise BytesRuntimeFailure(
            "fresh Bytes early return lost its caller-owned cleanup"
        )

    runtime_string_copy = hxcir_function(hxcir, "function.Main.copyText")
    for marker in (
        'parameter "parameter.0" type=string-utf8',
        'runtime(feature="bytes",operation="of-string-utf8") arguments=["parameter.0"]',
        'terminator return value="value.0"',
    ):
        if marker not in runtime_string_copy:
            raise BytesRuntimeFailure(
                f"runtime String-to-Bytes HxcIR omitted {marker}"
            )
    if 'runtime(feature="string",' in runtime_string_copy:
        raise BytesRuntimeFailure(
            "runtime String-to-Bytes copy unnecessarily selected owned String operations"
        )

    main = hxcir_function(hxcir, "function.Main.main")
    owner_actions = [
        line.split('"', 2)[1]
        for line in main.splitlines()
        if " action " in line and '"bytes-temporary.' in line
    ]
    if len(owner_actions) < 5:
        raise BytesRuntimeFailure(
            "nested fresh Bytes calls did not create their exact caller owners"
        )
    return_lines = [
        line
        for line in main.splitlines()
        if "terminator return" in line
        and all(f'"{action}"' in line for action in owner_actions)
    ]
    if not return_lines or any(
        return_lines[0].index(f'"{later}"')
        >= return_lines[0].index(f'"{earlier}"')
        for earlier, later in zip(owner_actions, owner_actions[1:])
    ):
        raise BytesRuntimeFailure(
            "fresh Bytes argument owners lost reverse cleanup order"
        )

    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if plan.get("features") != ["runtime-base", "status", "alloc", "string-literal", "bytes"]:
        raise BytesRuntimeFailure("generated Bytes program selected the wrong runtime closure")
    operations = {
        reason.get("operationId")
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict) and reason.get("featureId") == "bytes"
    }
    expected = {
        "alloc",
        "blit",
        "cleanup-release",
        "compare",
        "fill",
        "get",
        "length",
        "managed-type-representation",
        "of-string-utf8",
        "retain",
        "set",
        "sub",
    }
    if operations != expected:
        raise BytesRuntimeFailure(f"generated Bytes operations drifted: {sorted(operations)!r}")
    sources = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    for marker in (
        "hxc_bytes_ref_create_zeroed",
        "hxc_bytes_ref_create_utf8_copy",
        "hxc_bytes_ref_blit",
        "hxc_bytes_ref_compare",
        "hxc_bytes_ref_retain",
        "hxc_bytes_ref_release",
    ):
        if marker not in sources:
            raise BytesRuntimeFailure(f"generated C omitted {marker}")
    if "goto " in sources:
        raise BytesRuntimeFailure("the structured Bytes fixture unexpectedly emitted goto")


def render_generated_projects(root: Path) -> dict[str, Path]:
    projects: dict[str, Path] = {}
    for layout in LAYOUTS:
        normal = root / f"generated-{layout}-normal"
        reverse = root / f"generated-{layout}-reverse"
        first = compile_haxe(GENERATED, normal, layout=layout, report=True)
        second = compile_haxe(GENERATED, reverse, layout=layout, reverse=True)
        for label, result in ((f"{layout}-normal", first), (f"{layout}-reverse", second)):
            if result.returncode != 0:
                raise BytesRuntimeFailure(
                    f"{label} generated Bytes compile failed\n"
                    f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
                )
        if generated_tree(normal) != generated_tree(reverse):
            raise BytesRuntimeFailure(
                f"generated Bytes {layout} project changed under reversed discovery"
            )
        validate_generated_project(normal, extract_hxcir(first))
        projects[layout] = normal
    return projects


def run_negative_cases(root: Path) -> None:
    expected = {
        "return_escape": "TReturn(managed-Bytes-borrowed-return-needs-retain)",
        "runtime_string_operation": "TCall(String.toUpperCase:not-yet-admitted)",
        "unsupported_method": "TCall(Bytes.getInt32:not-yet-admitted)",
    }
    for name, marker in expected.items():
        output = root / f"negative-{name}"
        result = compile_haxe(NEGATIVE / name, output)
        if result.returncode == 0 or "HXC1001" not in result.stderr or marker not in result.stderr:
            raise BytesRuntimeFailure(f"negative Bytes case {name} drifted: {result.stderr!r}")
        if output.exists() and any(output.rglob("*")):
            raise BytesRuntimeFailure(f"negative Bytes case {name} left plausible output")
    output = root / "runtime-none"
    result = compile_haxe(GENERATED, output, defines=("hxc_runtime=none",))
    if result.returncode == 0 or "runtime policy `none`" not in result.stderr:
        raise BytesRuntimeFailure("managed Bytes did not fail closed under runtime policy none")
    if output.exists() and any(output.rglob("*")):
        raise BytesRuntimeFailure("runtime-policy rejection left plausible output")


def prove_caxecraft_bytes_argument_boundary(root: Path) -> None:
    """Keep Caxecraft beyond the fresh Bytes call argument that found this work."""
    result = subprocess.run(
        [
            sys.executable,
            str(ROOT / "examples/caxecraft/play.py"),
            "--compile-only",
            "--output-root",
            str(root / "caxecraft-compile-boundary"),
        ],
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=90,
    )
    if result.returncode == 0:
        return
    # Runtime String parameters now reach Bytes.ofString as immutable UTF-8
    # views; legacy-nullable String flow, the String-backed action switch, and
    # the contextually typed optional-record result also lower. The next
    # reachable boundary is String.fromCharCode inside StringBuf.addChar, owned
    # by haxe_c-aml.5. Requiring that exact later diagnostic proves this lane
    # passed every prior boundary rather than accepting an arbitrary failure.
    if (
        "src/caxecraft/scenario/ScenarioLexer.hx:58:" not in result.stderr
        or "TCall(unavailable-static-target:function.String.fromCharCode)"
        not in result.stderr
        or "fresh-managed-Bytes-argument-needs-owner" in result.stderr
        or "Bytes.ofString:non-literal-String-not-yet-admitted" in result.stderr
        or "TConst(TNull:requires-nullable-reference-or-direct-optional-context)"
        in result.stderr
        or "TSwitch(subject-type):closed-record-not-admitted-in-primitive-operation"
        in result.stderr
        or "TObjectDecl(contextual-type):optional-payload:incompatible-closed-record-shapes"
        in result.stderr
    ):
        raise BytesRuntimeFailure(
            "Caxecraft did not compile past its former fresh Bytes argument boundary\n"
            f"exit={result.returncode} stdout={result.stdout!r} stderr={result.stderr!r}"
        )


def compile_and_run(
    compiler: str,
    sources: list[Path],
    include_roots: list[Path],
    executable: Path,
    flags: tuple[str, ...],
    defines: tuple[str, ...] = (),
) -> None:
    command = [
        compiler,
        *STRICT_FLAGS,
        *flags,
        *(f"-D{define}" for define in defines),
        *(f"-I{root}" for root in include_roots),
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
        raise BytesRuntimeFailure(
            f"strict native compile failed\ncommand={command!r}\nstdout={compilation.stdout!r}\nstderr={compilation.stderr!r}"
        )
    execution = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if execution.returncode != 0 or execution.stdout or execution.stderr:
        raise BytesRuntimeFailure(
            f"native Bytes execution drifted: exit={execution.returncode} stdout={execution.stdout!r} stderr={execution.stderr!r}"
        )


def validate_cpp_header(
    family: str,
    project: Path,
    build: Path,
    flags: tuple[str, ...],
) -> None:
    """Compile and run an independent C++17 consumer of generated C headers.

    haxe.c emits strict C11 implementation files; it does not promise that a
    C++ compiler can parse those `.c` files as C++. The interoperability
    contract is instead that C++ can include the generated C declarations and
    link to C-compiled objects. This small consumer proves the first half here;
    shared native ABI lanes own cross-language layout and linkage evidence.
    """
    compiler = shutil.which("clang++" if family == "clang" else "g++")
    if compiler is None:
        raise BytesRuntimeFailure(f"{family} evidence requires its C++ compiler")
    optimization = flags[0][1:].lower()
    source = build / f"bytes-header-{project.name}-{optimization}.cpp"
    executable = build / f"bytes-header-{project.name}-{optimization}"
    source.write_text(
        '#include "hxc/program.h"\n'
        "int main() { return 0; }\n",
        encoding="utf-8",
    )
    command = [
        compiler,
        *CPP_STRICT_FLAGS,
        *flags,
        f"-I{project / 'include'}",
        f"-I{project / 'runtime/include'}",
        str(source),
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
        raise BytesRuntimeFailure(
            "strict C++17 generated-header consumer failed\n"
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
    if execution.returncode != 0 or execution.stdout or execution.stderr:
        raise BytesRuntimeFailure(
            "C++17 generated-header consumer execution drifted: "
            f"exit={execution.returncode} stdout={execution.stdout!r} stderr={execution.stderr!r}"
        )


def inspect_symbols(executable: Path, family: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise BytesRuntimeFailure(f"{family} Bytes evidence requires nm")
    result = subprocess.run([nm, str(executable)], check=False, capture_output=True, text=True, timeout=20)
    if result.returncode != 0:
        raise BytesRuntimeFailure(f"{family} could not inspect Bytes symbols")
    for required in ("hxc_bytes_ref_blit", "hxc_bytes_ref_release", "hxc_bytes_ref_sub"):
        if required not in result.stdout:
            raise BytesRuntimeFailure(f"{family} omitted required symbol {required}")
    for forbidden in ("hxc_array", "hxc_gc", "hxc_object", "hxc_reflection", "hxc_dynamic"):
        if forbidden in result.stdout:
            raise BytesRuntimeFailure(f"{family} retained unrelated symbol family {forbidden}")


def run_native(toolchains: list[Toolchain], *, generated_haxe: bool) -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-bytes-runtime-") as temporary:
        root = Path(temporary)
        projects = render_generated_projects(root) if generated_haxe else {}
        if generated_haxe:
            run_negative_cases(root)
            prove_caxecraft_bytes_argument_boundary(root)
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir()
            native = build / "bytes-native"
            compile_and_run(
                toolchain.compiler,
                [*RUNTIME_SOURCES, FIXTURE],
                [INCLUDE],
                native,
                ("-O0",),
                ("HXC_FREESTANDING=1",),
            )
            compile_and_run(
                toolchain.compiler,
                [*RUNTIME_SOURCES, FIXTURE],
                [INCLUDE],
                build / "bytes-native-o2",
                ("-O2",),
                ("HXC_FREESTANDING=1",),
            )
            inspect_symbols(native, toolchain.family)
            for layout, generated in projects.items():
                generated_sources = sorted(
                    (generated / "runtime/src").glob("*.c")
                ) + sorted((generated / "src").rglob("*.c"))
                for optimization in ("-O0", "-O2"):
                    compile_and_run(
                        toolchain.compiler,
                        generated_sources,
                        [generated / "include", generated / "runtime/include"],
                        build
                        / f"bytes-generated-{layout}-{optimization[1:].lower()}",
                        (optimization,),
                    )
                    validate_cpp_header(
                        toolchain.family,
                        generated,
                        build,
                        (optimization,),
                    )
            if toolchain.family == "clang":
                compile_and_run(
                    toolchain.compiler,
                    [*RUNTIME_SOURCES, FIXTURE],
                    [INCLUDE],
                    build / "bytes-sanitized",
                    SANITIZER_FLAGS,
                    ("HXC_FREESTANDING=1",),
                )
                if projects:
                    generated = projects["split"]
                    generated_sources = sorted(
                        (generated / "runtime/src").glob("*.c")
                    ) + sorted((generated / "src").rglob("*.c"))
                    compile_and_run(
                        toolchain.compiler,
                        generated_sources,
                        [generated / "include", generated / "runtime/include"],
                        build / "bytes-generated-sanitized",
                        SANITIZER_FLAGS,
                    )


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", *TOOLCHAINS), default="auto")
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        toolchains = resolve_toolchains(args.toolchain)
        if not args.native_only:
            run_eval_oracle()
        run_native(toolchains, generated_haxe=not args.native_only)
    except (BytesRuntimeFailure, OSError, UnicodeError, json.JSONDecodeError, subprocess.TimeoutExpired) as error:
        print(f"bytes-runtime: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    mode = "native contract" if args.native_only else "Eval plus generated ordinary-Haxe Bytes"
    print(
        "bytes-runtime: OK: "
        f"{families}; {mode}; C11 O0/O2 and C++17 header consumers, aliasing, overlap, bounds, allocation rollback, ownership, sanitizers, and selective symbols passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

#!/usr/bin/env python3
"""Prove ordinary Haxe Map<String, Bool> lowering and its native runtime contract."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[3]
CASE = Path(__file__).resolve().parent
GENERATED = CASE / "generated"
NEGATIVE = CASE / "negative"
FIXTURE = CASE / "string_map_runtime.c"
INCLUDE = ROOT / "runtime/hxrt/include"
RUNTIME_SOURCES = (
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/string_map.c",
)
TOOLCHAINS = ("gcc", "clang")
LAYOUTS = ("split", "package", "unity")
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


class StringMapFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Toolchain:
    family: str
    compiler: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(*, server: bool = False) -> dict[str, str]:
    environment = os.environ.copy()
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


def resolve_toolchains(selected: str) -> list[Toolchain]:
    families = TOOLCHAINS if selected == "auto" else (selected,)
    result: list[Toolchain] = []
    for family in families:
        compiler = shutil.which(family)
        if compiler is None:
            if selected != "auto":
                raise StringMapFailure(f"required C compiler is missing: {family}")
            print(f"string-map: SKIP optional {family}: missing command")
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
                raise StringMapFailure(f"{family} command identifies as {actual}")
            print(f"string-map: SKIP optional {family}: command identifies as {actual}")
            continue
        result.append(Toolchain(family, compiler))
    if not result:
        raise StringMapFailure("no strict C11 compiler is available")
    return result


def run_eval_oracle() -> None:
    results: list[tuple[int, str, str]] = []
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
        results.append((execution.returncode, execution.stdout, execution.stderr))
    if results != [(0, "", ""), (0, "", "")]:
        raise StringMapFailure(f"pinned Eval StringMap oracle drifted: {results!r}")


def compile_haxe(
    fixture: Path,
    output: Path,
    *,
    layout: str = "split",
    reverse: bool = False,
    report: bool = False,
    defines: tuple[str, ...] = (),
    connect: str | None = None,
) -> subprocess.CompletedProcess[str]:
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
            "-cp",
            str(fixture),
            "-lib",
            "reflaxe.c",
            "-main",
            "Main",
            "-D",
            f"hxc_project_layout={layout}",
        ]
    )
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
        env=haxe_environment(server=connect is not None),
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
        raise StringMapFailure("generated compile omitted its one HxcIR report")
    report = json.loads(reports[0])
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise StringMapFailure("generated compile omitted validated HxcIR text")
    return hxcir


def validate_generated_project(output: Path, hxcir: str) -> None:
    for marker in (
        'representation=managed("string-map")',
        'arguments=[string-utf8,bool]',
        'runtime(feature="string-map",operation="create")',
        'runtime(feature="string-map",operation="set")',
        'runtime(feature="string-map",operation="get")',
        'runtime(feature="string-map",operation="remove")',
        'binary operation="haxe.string-map-reference.equal"',
        'binary operation="haxe.string-map-reference.not-equal"',
        'retain place=local(',
        'release place=local(',
    ):
        if marker not in hxcir:
            raise StringMapFailure(f"validated HxcIR omitted {marker}")
    if " raw" in hxcir or str(ROOT) in hxcir:
        raise StringMapFailure("StringMap HxcIR used raw syntax or leaked the checkout path")

    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if plan.get("features") != [
        "runtime-base",
        "status",
        "alloc",
        "string-literal",
        "string-map",
    ]:
        raise StringMapFailure("generated StringMap program selected the wrong runtime closure")
    operations = {
        reason.get("operationId")
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict) and reason.get("featureId") == "string-map"
    }
    expected = {
        "cleanup-release",
        "clear",
        "create",
        "exists",
        "get",
        "managed-type-representation",
        "remove",
        "retain",
        "set",
    }
    if operations != expected:
        raise StringMapFailure(
            f"generated StringMap operations drifted: {sorted(operations)!r}"
        )
    if "managed-haxe-string-maps" not in plan.get("directDecisions", []):
        raise StringMapFailure("runtime plan omitted the exact StringMap representation decision")

    sources = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "src").rglob("*.c"))
    )
    for marker in (
        "struct hxc_string_map_ref *",
        "hxc_string_map_ref_create",
        "hxc_string_map_ref_set_copy",
        "hxc_string_map_ref_get_copy",
        "hxc_string_map_ref_retain",
        "hxc_string_map_ref_release",
        "sizeof(bool)",
        "_Alignof(bool)",
    ):
        if marker not in sources:
            raise StringMapFailure(f"generated C omitted {marker}")
    for forbidden in ("hxc_dynamic", "void *hxc_value", "goto "):
        if forbidden in sources:
            raise StringMapFailure(f"generated C retained forbidden shape {forbidden!r}")


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10.0
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise StringMapFailure(
                f"Haxe server exited before determinism requests: {stdout!r} {stderr!r}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise StringMapFailure("Haxe server did not accept determinism requests")


def render_server_pair(root: Path) -> tuple[Path, Path]:
    port = available_port()
    endpoint = str(port)
    server = subprocess.Popen(
        [development_tool("haxe"), "--wait", endpoint],
        cwd=ROOT,
        env=haxe_environment(server=True),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_server(server, port)
        outputs = (root / "server-first", root / "server-second")
        for label, output in zip(("first", "second"), outputs):
            result = compile_haxe(GENERATED, output, connect=endpoint)
            if result.returncode != 0:
                raise StringMapFailure(
                    f"{label} warm-server compile failed: {result.stdout!r} {result.stderr!r}"
                )
        return outputs
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def render_projects(root: Path) -> dict[str, Path]:
    projects: dict[str, Path] = {}
    for layout in LAYOUTS:
        normal = root / f"{layout}-normal"
        reverse = root / f"{layout}-reverse"
        first = compile_haxe(GENERATED, normal, layout=layout, report=layout == "split")
        second = compile_haxe(GENERATED, reverse, layout=layout, reverse=True)
        for label, result in ((f"{layout}-normal", first), (f"{layout}-reverse", second)):
            if result.returncode != 0:
                raise StringMapFailure(
                    f"{label} compile failed\nstdout={result.stdout!r}\nstderr={result.stderr!r}"
                )
        if generated_tree(normal) != generated_tree(reverse):
            raise StringMapFailure(f"{layout} output changed under reversed discovery")
        projects[layout] = normal
        if layout == "split":
            validate_generated_project(normal, extract_hxcir(first))

    server_first, server_second = render_server_pair(root)
    split_tree = generated_tree(projects["split"])
    if generated_tree(server_first) != split_tree or generated_tree(server_second) != split_tree:
        raise StringMapFailure("split output changed under warm compiler-server reuse")
    return projects


def run_negative_cases(root: Path) -> None:
    expected = {
        "value_type": "StringMap-value-not-yet-admitted:int32_t",
        "key_type": "virtual-slot-generic-requires-specialization:slot.haxe.ds.IntMap.set",
        "iteration": "TVar(value:type).field:hasNext:method",
        "reassignment": "TBinop(OpAssign:managed-StringMap-reassignment-not-admitted)",
    }
    for name, marker in expected.items():
        output = root / f"negative-{name}"
        result = compile_haxe(NEGATIVE / name, output)
        if result.returncode == 0 or "HXC1001" not in result.stderr or marker not in result.stderr:
            raise StringMapFailure(f"negative case {name} drifted: {result.stderr!r}")
        if output.exists() and any(output.rglob("*")):
            raise StringMapFailure(f"negative case {name} left plausible generated output")

    output = root / "runtime-none"
    rejected = compile_haxe(GENERATED, output, defines=("hxc_runtime=none",))
    if rejected.returncode == 0 or "runtime policy `none`" not in rejected.stderr:
        raise StringMapFailure("runtime policy none did not reject managed StringMap")
    if output.exists() and any(output.rglob("*")):
        raise StringMapFailure("runtime-policy rejection left plausible output")


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
    compiled = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise StringMapFailure(
            f"strict native compile failed\ncommand={command!r}\n"
            f"stdout={compiled.stdout!r}\nstderr={compiled.stderr!r}"
        )
    executed = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if executed.returncode != 0 or executed.stdout or executed.stderr:
        raise StringMapFailure(
            f"native execution drifted: exit={executed.returncode} "
            f"stdout={executed.stdout!r} stderr={executed.stderr!r}"
        )


def validate_cpp_headers(project: Path, family: str, root: Path) -> None:
    compiler = shutil.which("clang++" if family == "clang" else "g++")
    if compiler is None:
        raise StringMapFailure(f"{family} evidence requires its C++ compiler")
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
    result = subprocess.run(command, cwd=ROOT, check=False, capture_output=True, text=True, timeout=30)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise StringMapFailure(f"{family} C++ private-header check failed: {result.stderr!r}")


def inspect_symbols(executable: Path, family: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise StringMapFailure(f"{family} StringMap evidence requires nm")
    result = subprocess.run([nm, str(executable)], check=False, capture_output=True, text=True, timeout=20)
    if result.returncode != 0:
        raise StringMapFailure(f"{family} could not inspect StringMap symbols")
    for required in (
        "hxc_string_map_ref_create",
        "hxc_string_map_ref_get_copy",
        "hxc_string_map_ref_release",
    ):
        if required not in result.stdout:
            raise StringMapFailure(f"{family} omitted required symbol {required}")
    for forbidden in ("hxc_array", "hxc_bytes", "hxc_gc", "hxc_dynamic"):
        if forbidden in result.stdout:
            raise StringMapFailure(f"{family} retained unrelated symbol family {forbidden}")


def run_native(toolchains: list[Toolchain], *, generated_haxe: bool) -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-string-map-") as temporary:
        root = Path(temporary)
        projects = render_projects(root) if generated_haxe else {}
        if generated_haxe:
            run_negative_cases(root)
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir()
            native = build / "native-o0"
            compile_and_run(
                toolchain.compiler,
                [*RUNTIME_SOURCES, FIXTURE],
                [INCLUDE],
                native,
                ("-O0",),
            )
            compile_and_run(
                toolchain.compiler,
                [*RUNTIME_SOURCES, FIXTURE],
                [INCLUDE],
                build / "native-o2",
                ("-O2",),
            )
            inspect_symbols(native, toolchain.family)
            if generated_haxe:
                for layout, project in projects.items():
                    sources = sorted((project / "runtime/src").glob("*.c")) + sorted(
                        (project / "src").rglob("*.c")
                    )
                    generated_executable = build / f"generated-{layout}"
                    compile_and_run(
                        toolchain.compiler,
                        sources,
                        [project / "include", project / "runtime/include"],
                        generated_executable,
                        ("-O2" if layout == "unity" else "-O0",),
                    )
                    inspect_symbols(generated_executable, toolchain.family)
                validate_cpp_headers(projects["split"], toolchain.family, build)
            if toolchain.family == "clang":
                compile_and_run(
                    toolchain.compiler,
                    [*RUNTIME_SOURCES, FIXTURE],
                    [INCLUDE],
                    build / "native-sanitized",
                    SANITIZER_FLAGS,
                )
                if generated_haxe:
                    project = projects["split"]
                    sources = sorted((project / "runtime/src").glob("*.c")) + sorted(
                        (project / "src").rglob("*.c")
                    )
                    compile_and_run(
                        toolchain.compiler,
                        sources,
                        [project / "include", project / "runtime/include"],
                        build / "generated-sanitized",
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
    except (
        StringMapFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"string-map: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    mode = "native contract" if args.native_only else "Eval plus generated Map<String, Bool>"
    print(
        "string-map: OK: "
        f"{families}; {mode}; missing-vs-false, replacement, removal, clear, aliases, "
        "nullable identity, empty keys, growth, allocation rollback, malformed-call rejection, layouts, determinism, "
        "sanitizers, C++ headers, runtime-none, and selective symbols passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

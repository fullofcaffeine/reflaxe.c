#!/usr/bin/env python3
"""Prove bounded ObjectMap identity and recursive EnumValueMap equality."""

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
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402

CASE = Path(__file__).resolve().parent
GENERATED = CASE / "generated"
NEGATIVE = CASE / "negative"
NATIVE_FIXTURE = CASE / "typed_map_runtime.c"
RUNTIME_INCLUDE = ROOT / "runtime/hxrt/include"
RUNTIME_SOURCES = (
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/array.c",
    ROOT / "runtime/hxrt/src/iterator.c",
    ROOT / "runtime/hxrt/src/object.c",
    ROOT / "runtime/hxrt/src/gc.c",
    ROOT / "runtime/hxrt/src/typed_map.c",
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


class TypedMapFailure(RuntimeError):
    """One bounded typed-map contract failed."""


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
                raise TypedMapFailure(f"required C compiler is missing: {family}")
            print(f"object-enum-map: SKIP optional {family}: missing command")
            continue
        identity = run_bounded_process(
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
                raise TypedMapFailure(f"{family} command identifies as {actual}")
            print(f"object-enum-map: SKIP optional {family}: command identifies as {actual}")
            continue
        result.append(Toolchain(family, compiler))
    if not result:
        raise TypedMapFailure("no strict C11 compiler is available")
    return result


def run_eval_oracle() -> None:
    results: list[tuple[int, str, str]] = []
    for _ in range(2):
        execution = run_bounded_process(
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
        raise TypedMapFailure(f"pinned Eval typed-map oracle drifted: {results!r}")


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
    return run_bounded_process(
        command,
        cwd=ROOT,
        env=haxe_environment(server=connect is not None),
        check=False,
        capture_output=True,
        text=True,
        timeout=180,
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
        raise TypedMapFailure("generated compile omitted its one HxcIR report")
    report = json.loads(reports[0])
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise TypedMapFailure("generated compile omitted validated HxcIR text")
    return hxcir


def validate_generated_project(output: Path, hxcir: str) -> None:
    for marker in (
        'representation=managed("gc")',
        'runtime(feature="object-map",operation="create")',
        'runtime(feature="object-map",operation="set")',
        'runtime(feature="object-map",operation="exists")',
        'runtime(feature="object-map",operation="get")',
        'runtime(feature="object-map",operation="remove")',
        'runtime(feature="object-map",operation="clear")',
        'runtime(feature="object-map",operation="copy")',
        'runtime(feature="object-map",operation="iterator")',
        'runtime(feature="object-map",operation="keys")',
        'runtime(feature="object-map",operation="key-value-iterator")',
        'runtime(feature="enum-value-map",operation="create")',
        'runtime(feature="enum-value-map",operation="set")',
        'runtime(feature="enum-value-map",operation="exists")',
        'runtime(feature="enum-value-map",operation="get")',
        'runtime(feature="enum-value-map",operation="remove")',
        'runtime(feature="enum-value-map",operation="clear")',
        'runtime(feature="enum-value-map",operation="copy")',
        'runtime(feature="enum-value-map",operation="iterator")',
        'runtime(feature="enum-value-map",operation="keys")',
        'managed-root "root.',
    ):
        if marker not in hxcir:
            raise TypedMapFailure(f"validated HxcIR omitted {marker}")
    if " raw" in hxcir or str(ROOT) in hxcir:
        raise TypedMapFailure("typed-map HxcIR used raw syntax or leaked the checkout path")

    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    expected_features = [
        "runtime-base",
        "status",
        "alloc",
        "array",
        "object",
        "gc",
        "iterator",
        "typed-map",
        "enum-value-map",
        "object-map",
    ]
    if plan.get("features") != expected_features:
        raise TypedMapFailure(f"typed-map runtime closure drifted: {plan.get('features')!r}")
    expected_operations = {
        "object-map": {
            "clear",
            "copy",
            "create",
            "exists",
            "get",
            "iterator",
            "key-value-iterator",
            "keys",
            "remove",
            "set",
        },
        "enum-value-map": {
            "clear",
            "copy",
            "create",
            "exists",
            "get",
            "iterator",
            "keys",
            "remove",
            "set",
        },
    }
    for feature, expected in expected_operations.items():
        actual = {
            reason.get("operationId")
            for reason in plan.get("rootReasons", [])
            if isinstance(reason, dict) and reason.get("featureId") == feature
        }
        if actual != expected:
            raise TypedMapFailure(f"{feature} operations drifted: {sorted(actual)!r}")
    decisions = plan.get("directDecisions", [])
    for decision in (
        "exact-traced-haxe-object-graph",
        "managed-haxe-enum-value-maps",
        "managed-haxe-iterators",
        "managed-haxe-object-maps",
    ):
        if decision not in decisions:
            raise TypedMapFailure(f"runtime plan omitted {decision}")
    for forbidden in ("managed-haxe-int-maps", "managed-haxe-string-maps", "managed-haxe-bytes"):
        if forbidden in decisions:
            raise TypedMapFailure(f"runtime plan selected unrelated decision {forbidden}")

    stdlib = json.loads((output / "hxc.stdlib-report.json").read_text(encoding="utf-8"))
    if stdlib.get("modules") != ["enum-value-map", "gc", "iterator", "object-map"]:
        raise TypedMapFailure("stdlib report did not name the exact typed-map modules")
    required_capabilities = {
        "allocation",
        "class-object-header",
        "cleanup-release",
        "clear",
        "copy",
        "create",
        "exists",
        "get",
        "has-next",
        "iterator",
        "key-value-iterator",
        "keys",
        "managed-type-representation",
        "next",
        "remove",
        "root-frame",
        "set",
    }
    if set(stdlib.get("capabilities", [])) != required_capabilities:
        raise TypedMapFailure("stdlib report did not name the exact typed-map capabilities")

    application_sources = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "src").rglob("*.c"))
    )
    support = (output / "src/hxc/support.c").read_text(encoding="utf-8")
    for marker in (
        "struct hxc_typed_map_ref *",
        "hxc_typed_map_init_collector_owned",
        "hxc_typed_map_ref_set_copy",
        "hxc_typed_map_ref_exists",
        "hxc_typed_map_ref_get_copy",
        "hxc_typed_map_ref_remove",
        "hxc_typed_map_ref_clear",
        "hxc_typed_map_copy_in_place",
        "hxc_typed_map_ref_value_iterator",
        "hxc_typed_map_ref_key_iterator",
        "hxc_typed_map_ref_pair_iterator",
        "hxc_typed_map_identity_hash",
        "hxc_typed_map_hash_mix",
        "sizeof(struct hxc_MapToken)",
        "_Alignof(struct hxc_MapToken)",
    ):
        if marker not in application_sources:
            raise TypedMapFailure(f"generated C omitted {marker}")
    for marker in (
        "hxc_typed_map_hash_",
        "hxc_typed_map_equal_",
        "hxc_typed_map_key_trace_",
        "hxc_typed_map_value_trace_",
    ):
        if marker not in support:
            raise TypedMapFailure(f"generated support policy omitted {marker}")
    for forbidden in (
        "hxc_dynamic",
        "BalancedTree",
        "Reflect.compare",
        "uintptr_t",
        "hxc_string_map",
        "hxc_int_bool_map",
    ):
        if forbidden in application_sources:
            raise TypedMapFailure(f"generated application C retained forbidden shape {forbidden!r}")


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


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
        deadline = time.monotonic() + 10.0
        while time.monotonic() < deadline:
            if server.poll() is not None:
                stdout, stderr = server.communicate()
                raise TypedMapFailure(
                    f"Haxe server exited before determinism requests: {stdout!r} {stderr!r}"
                )
            try:
                with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                    break
            except OSError:
                time.sleep(0.05)
        else:
            raise TypedMapFailure("Haxe server did not accept determinism requests")
        outputs = (root / "server-first", root / "server-second")
        for label, output in zip(("first", "second"), outputs):
            result = compile_haxe(GENERATED, output, connect=endpoint)
            if result.returncode != 0:
                raise TypedMapFailure(
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
                raise TypedMapFailure(
                    f"{label} compile failed\nstdout={result.stdout!r}\nstderr={result.stderr!r}"
                )
        if generated_tree(normal) != generated_tree(reverse):
            raise TypedMapFailure(f"{layout} output changed under reversed discovery")
        projects[layout] = normal
        if layout == "split":
            validate_generated_project(normal, extract_hxcir(first))
    server_first, server_second = render_server_pair(root)
    split_tree = generated_tree(projects["split"])
    if generated_tree(server_first) != split_tree or generated_tree(server_second) != split_tree:
        raise TypedMapFailure("split output changed under warm compiler-server reuse")
    return projects


def run_negative_cases(root: Path) -> None:
    expected = {
        "enum_float": "enum-value-map-key-not-admitted:FloatKey.Amount.value:double",
        "recursive_enum": "enum-value-map-key-not-admitted:recursive-enum:RecursiveKey",
        "interface_key": "object-map-key-not-admitted:haxe-interface-reference:",
        "dynamic_key": "typed-map-key:the dynamic source semantic type cannot stand in for a primitive",
    }
    for name, marker in expected.items():
        output = root / f"negative-{name}"
        result = compile_haxe(NEGATIVE / name, output)
        if result.returncode == 0 or "HXC1001" not in result.stderr or marker not in result.stderr:
            raise TypedMapFailure(f"negative case {name} drifted: {result.stderr!r}")
        if output.exists() and any(output.rglob("*")):
            raise TypedMapFailure(f"negative case {name} left plausible generated output")
    output = root / "runtime-none"
    rejected = compile_haxe(GENERATED, output, defines=("hxc_runtime=none",))
    if rejected.returncode == 0 or "runtime policy `none`" not in rejected.stderr:
        raise TypedMapFailure("runtime policy none did not reject collector-owned maps")
    if output.exists() and any(output.rglob("*")):
        raise TypedMapFailure("runtime-policy rejection left plausible output")


def compile_and_run(
    compiler: str,
    sources: list[Path],
    include_roots: list[Path],
    executable: Path,
    flags: tuple[str, ...],
) -> None:
    command = [
        compiler,
        *STRICT_FLAGS,
        *flags,
        *(f"-I{root}" for root in include_roots),
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
        timeout=90,
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise TypedMapFailure(
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
        raise TypedMapFailure(
            f"native execution drifted: exit={executed.returncode} "
            f"stdout={executed.stdout!r} stderr={executed.stderr!r}"
        )


def validate_cpp_headers(project: Path, family: str, root: Path) -> None:
    compiler = shutil.which("clang++" if family == "clang" else "g++")
    if compiler is None:
        raise TypedMapFailure(f"{family} evidence requires its C++ compiler")
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
        raise TypedMapFailure(f"{family} C++ private-header check failed: {result.stderr!r}")


def inspect_symbols(executable: Path, family: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise TypedMapFailure(f"{family} typed-map evidence requires nm")
    result = run_bounded_process(
        [nm, str(executable)],
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if result.returncode != 0:
        raise TypedMapFailure(f"{family} could not inspect typed-map symbols")
    for required in (
        "hxc_typed_map_type_descriptor",
        "hxc_typed_map_identity_hash",
        "hxc_typed_map_hash_mix",
        "hxc_typed_map_ref_set_copy",
        "hxc_typed_map_ref_get_copy",
        "hxc_typed_map_ref_pair_iterator",
        "hxc_iterator_ref_create_traced_snapshot",
        "hxc_gc_root_table_register",
    ):
        if required not in result.stdout:
            raise TypedMapFailure(f"{family} omitted required symbol {required}")
    for forbidden in ("hxc_string_map", "hxc_int_bool_map", "hxc_bytes", "hxc_dynamic"):
        if forbidden in result.stdout:
            raise TypedMapFailure(f"{family} retained unrelated symbol family {forbidden}")


def run_native(toolchains: list[Toolchain], *, generated_haxe: bool) -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-object-enum-map-") as temporary:
        root = Path(temporary)
        projects = render_projects(root) if generated_haxe else {}
        if generated_haxe:
            run_negative_cases(root)
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir()
            for optimization in ("-O0", "-O2"):
                native = build / f"native-{optimization[1:].lower()}"
                compile_and_run(
                    toolchain.compiler,
                    [*RUNTIME_SOURCES, NATIVE_FIXTURE],
                    [RUNTIME_INCLUDE],
                    native,
                    (optimization,),
                )
                inspect_symbols(native, toolchain.family)
            if generated_haxe:
                validate_cpp_headers(projects["split"], toolchain.family, build)
                for layout, project in projects.items():
                    sources = sorted((project / "runtime/src").glob("*.c")) + sorted(
                        (project / "src").rglob("*.c")
                    )
                    executable = build / f"generated-{layout}"
                    compile_and_run(
                        toolchain.compiler,
                        sources,
                        [project / "include", project / "runtime/include"],
                        executable,
                        ("-O2" if layout == "unity" else "-O0",),
                    )
                    inspect_symbols(executable, toolchain.family)
            if toolchain.family == "clang":
                compile_and_run(
                    toolchain.compiler,
                    [*RUNTIME_SOURCES, NATIVE_FIXTURE],
                    [RUNTIME_INCLUDE],
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
        TypedMapFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"object-enum-map: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    if args.native_only:
        evidence = (
            "native identity, collisions, replacement/removal, snapshot mutation, "
            "exact tracing, rollback, optimization, sanitizers, and symbols passed"
        )
    else:
        evidence = (
            "Eval and generated typed-map identity, recursive equality, collisions, "
            "aliases, replacement/removal, snapshot mutation, exact tracing, rollback, "
            "layouts, determinism, C++, sanitizers, runtime-none, diagnostics, and "
            "symbols passed"
        )
    print(f"object-enum-map: OK: {families}; {evidence}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

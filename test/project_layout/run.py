#!/usr/bin/env python3
"""Prove module, package, and unity generated-C layouts from one semantic plan."""

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


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.c_fixture_harness import (  # noqa: E402
    CFixtureFailure,
    CFixtureProject,
    resolve_toolchains,
    run_c_fixture_corpus,
    validate_report,
)


CASE = Path(__file__).resolve().parent
BUILD_HXML = CASE / "build.hxml"
ORACLE_HXML = CASE / "oracle.hxml"
PLANNER_HXML = CASE / "planner.hxml"
NATIVE = CASE / "native"
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
COMMON_FILES = {
    "_GeneratedFiles.json",
    "cmake/CMakeLists.txt",
    "hxc.abi.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
    "meson.build",
}
SPLIT_HEADERS = (
    "include/hxc/detail/program_types.h",
    "include/hxc/modules/layout/Main.h",
    "include/hxc/modules/layout/math/Numbers.h",
    "include/hxc/modules/layout/model/HardRecord.h",
    "include/hxc/modules/layout/model/Left.h",
    "include/hxc/modules/layout/model/Phase.h",
    "include/hxc/modules/layout/model/Point.h",
    "include/hxc/modules/layout/model/Right.h",
    "include/hxc/modules/layout/model/SoftRecord.h",
    "include/hxc/modules/layout/model/State.h",
    "include/hxc/modules/layout/platform/Device.h",
    "include/hxc/program.h",
)
SPLIT_SOURCES = (
    "src/hxc/main.c",
    "src/hxc/support.c",
    "src/modules/layout/Main.c",
    "src/modules/layout/math/Numbers.c",
    "src/modules/layout/model/Left.c",
    "src/modules/layout/model/Right.c",
    "src/modules/layout/model/SoftRecord.c",
    "src/modules/layout/model/State.c",
    "src/modules/layout/platform/Device.c",
)
PACKAGE_HEADERS = (
    "include/hxc/detail/program_types.h",
    "include/hxc/packages/layout/math/package.h",
    "include/hxc/packages/layout/model/package.h",
    "include/hxc/packages/layout/package.h",
    "include/hxc/packages/layout/platform/package.h",
    "include/hxc/program.h",
)
PACKAGE_SOURCES = (
    "src/hxc/main.c",
    "src/hxc/support.c",
    "src/packages/layout/math/package.c",
    "src/packages/layout/model/package.c",
    "src/packages/layout/package.c",
    "src/packages/layout/platform/package.c",
)
LAYOUTS = ("split", "package", "unity")
HEADERS_BY_LAYOUT = {
    "split": SPLIT_HEADERS,
    "package": PACKAGE_HEADERS,
    "unity": ("include/hxc/program.h",),
}
SOURCES_BY_LAYOUT = {
    "split": SPLIT_SOURCES,
    "package": PACKAGE_SOURCES,
    "unity": ("src/program.c",),
}
EXPECTED_FILES = {
    layout: COMMON_FILES | set(HEADERS_BY_LAYOUT[layout]) | set(SOURCES_BY_LAYOUT[layout])
    for layout in LAYOUTS
}
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-pedantic-errors",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
    "-Winfinite-recursion",
)


class LayoutFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Rendered:
    layout: str
    output: Path
    artifacts: dict[str, bytes]
    hxcir: str
    semantic_symbols: tuple[tuple[str, str], ...]
    runtime_plan: dict[str, object]


def tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def environment(locale: str, *, server: bool) -> dict[str, str]:
    result = os.environ.copy()
    result["LC_ALL"] = locale
    if server:
        result.pop("HAXE_NO_SERVER", None)
    else:
        result["HAXE_NO_SERVER"] = "1"
    return result


def compile_target(
    output: Path,
    layout: str,
    *,
    reverse: bool = False,
    locale: str = "C",
    connect: str | None = None,
    report: bool = True,
) -> subprocess.CompletedProcess[str]:
    command = [tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
            "--cwd",
            str(CASE),
            BUILD_HXML.name,
            "-D",
            "hxc_runtime_diagnostics=off",
        ]
    )
    if layout != "split" and layout in LAYOUTS:
        command.extend(["-D", f"hxc_project_layout={layout}"])
    elif layout != "split":
        raise LayoutFailure(f"unknown layout {layout!r}")
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_static_initialization_report"])
    command.extend(["--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=environment(locale, server=connect is not None),
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )


def load_object(path: Path, label: str) -> dict[str, object]:
    value: object = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise LayoutFailure(f"{label} must be a JSON object")
    return value


def generated_files(output: Path) -> set[str]:
    return {
        path.relative_to(output).as_posix()
        for path in output.rglob("*")
        if path.is_file()
    }


def normal_artifacts(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"), key=lambda item: item.as_posix().encode())
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def extract_hxcir(result: subprocess.CompletedProcess[str], label: str) -> str:
    lines = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(lines) != 1:
        raise LayoutFailure(f"{label} omitted its one HxcIR report")
    report = json.loads(lines[0])
    if not isinstance(report, dict) or not isinstance(report.get("hxcir"), str):
        raise LayoutFailure(f"{label} emitted a malformed HxcIR report")
    return report["hxcir"]


def semantic_symbols(output: Path) -> tuple[tuple[str, str], ...]:
    symbols = load_object(output / "hxc.symbols.json", "symbol table").get("symbols")
    if not isinstance(symbols, list):
        raise LayoutFailure("symbol table omitted symbols")
    result: list[tuple[str, str]] = []
    for entry in symbols:
        if not isinstance(entry, dict):
            raise LayoutFailure("symbol table contains a malformed entry")
        source = entry.get("sourceSymbol")
        c_name = entry.get("cName")
        namespace = entry.get("namespace")
        if (
            isinstance(source, str)
            and isinstance(c_name, str)
            and isinstance(namespace, dict)
            and namespace.get("kind") != "preprocessor"
        ):
            result.append((source, c_name))
    return tuple(sorted(result, key=lambda pair: pair[0].encode("utf-8")))


def render(
    output: Path,
    layout: str,
    label: str,
    *,
    reverse: bool = False,
    locale: str = "C",
    connect: str | None = None,
) -> Rendered:
    result = compile_target(
        output, layout, reverse=reverse, locale=locale, connect=connect
    )
    if result.returncode != 0 or result.stderr:
        raise LayoutFailure(
            f"{label} failed\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    actual = generated_files(output)
    if actual != EXPECTED_FILES[layout]:
        raise LayoutFailure(f"{label} file set drifted: {sorted(actual)!r}")
    manifest = load_object(output / "hxc.manifest.json", f"{label} manifest")
    configuration = manifest.get("configuration")
    build = manifest.get("build")
    expected_sources = list(SOURCES_BY_LAYOUT[layout])
    expected_headers = list(HEADERS_BY_LAYOUT[layout])
    if (
        not isinstance(configuration, dict)
        or configuration.get("projectLayout") != layout
        or not isinstance(build, dict)
        or build.get("sources") != expected_sources
        or build.get("privateHeaders") != expected_headers
    ):
        raise LayoutFailure(f"{label} manifest does not exactly describe {layout}")
    runtime_plan = load_object(output / "hxc.runtime-plan.json", f"{label} runtime")
    if runtime_plan.get("status") != "analyzed-runtime-free":
        raise LayoutFailure(f"{label} unexpectedly selected runtime support")
    if layout in ("split", "package"):
        check_type_dependencies(output, label, layout)
    return Rendered(
        layout,
        output,
        normal_artifacts(output),
        extract_hxcir(result, label),
        semantic_symbols(output),
        runtime_plan,
    )


def check_type_dependencies(output: Path, label: str, layout: str) -> None:
    if layout == "package":
        check_package_type_dependencies(output, label)
        return
    module_root = output / "include/hxc/modules/layout"
    phase_include = '#include "hxc/modules/layout/model/Phase.h"'
    numbers = (module_root / "math/Numbers.h").read_text(encoding="utf-8")
    state = (module_root / "model/State.h").read_text(encoding="utf-8")
    phase = (module_root / "model/Phase.h").read_text(encoding="utf-8")
    point = (module_root / "model/Point.h").read_text(encoding="utf-8")
    left = (module_root / "model/Left.h").read_text(encoding="utf-8")
    right = (module_root / "model/Right.h").read_text(encoding="utf-8")
    hard_record = (module_root / "model/HardRecord.h").read_text(encoding="utf-8")
    soft_record = (module_root / "model/SoftRecord.h").read_text(encoding="utf-8")
    common = (output / "include/hxc/detail/program_types.h").read_text(
        encoding="utf-8"
    )
    point_tag = "hxc_layout_model_Point"
    hard_record_tag = "hxc_layout_model_HardRecord"
    left_tag = "hxc_layout_model_Left"
    right_tag = "hxc_layout_model_Right"
    has_point_definition = f"struct {point_tag} {{" in point
    numbers_has_point_definition = f"struct {point_tag} {{" in numbers
    if (
        phase_include not in numbers
        or "model/Point.h" in numbers
        or phase_include not in state
        or "hxc_layout_model_Phase_Ready" not in phase
        or "hxc_layout_model_Phase_Ready" in common
        or not has_point_definition
        or numbers_has_point_definition
        or f"struct {point_tag} " not in numbers
        or f"struct {point_tag};" not in common
        or "model/Right.h" in left
        or "model/Left.h" in right
        or f"struct {right_tag} *" not in left
        or f"struct {left_tag} *" not in right
        or f"struct {left_tag};" not in common
        or f"struct {right_tag};" not in common
        or '#include "hxc/modules/layout/model/SoftRecord.h"' not in hard_record
        or "model/HardRecord.h" in soft_record
        or f"struct {hard_record_tag} " not in soft_record
        or f"struct {hard_record_tag};" not in common
        or "zx" in "\n".join((numbers, state, phase, point, left, right, hard_record, soft_record, common))
    ):
        raise LayoutFailure(
            f"{label} lost hard complete-type edges or soft declaration forwards"
        )


def check_package_type_dependencies(output: Path, label: str) -> None:
    package_root = output / "include/hxc/packages/layout"
    model_include = '#include "hxc/packages/layout/model/package.h"'
    numbers = (package_root / "math/package.h").read_text(encoding="utf-8")
    model = (package_root / "model/package.h").read_text(encoding="utf-8")
    common = (output / "include/hxc/detail/program_types.h").read_text(
        encoding="utf-8"
    )
    point_tag = "hxc_layout_model_Point"
    hard_record_tag = "hxc_layout_model_HardRecord"
    soft_record_tag = "hxc_layout_model_SoftRecord"
    left_tag = "hxc_layout_model_Left"
    right_tag = "hxc_layout_model_Right"
    soft_definition = model.find(f"struct {soft_record_tag} {{")
    hard_definition = model.find(f"struct {hard_record_tag} {{")
    if (
        model_include not in numbers
        or f"struct {point_tag} {{" in numbers
        or f"struct {point_tag} {{" not in model
        or "hxc_layout_model_Phase_Ready" not in model
        or "hxc_layout_model_Phase_Ready" in common
        or model_include in model
        or soft_definition == -1
        or hard_definition == -1
        or soft_definition >= hard_definition
        or f"struct {right_tag} *" not in model
        or f"struct {left_tag} *" not in model
        or f"struct {point_tag};" not in common
        or f"struct {hard_record_tag};" not in common
        or f"struct {left_tag};" not in common
        or f"struct {right_tag};" not in common
        or "zx" in "\n".join((numbers, model, common))
    ):
        raise LayoutFailure(
            f"{label} lost package-local type order, cross-package hard edges, "
            "or soft declaration forwards"
        )


def assert_equal(left: Rendered, right: Rendered, label: str) -> None:
    if left.artifacts.keys() != right.artifacts.keys():
        raise LayoutFailure(f"{label} artifact paths differ")
    for path, content in left.artifacts.items():
        if right.artifacts[path] != content:
            raise LayoutFailure(f"{label} first differs in {path!r}")


def alternate_locale() -> str:
    result = subprocess.run(
        ["locale", "-a"], check=False, capture_output=True, text=True, timeout=10
    )
    available = set(result.stdout.splitlines())
    for candidate in ("C.UTF-8", "C.utf8", "en_US.UTF-8", "en_US.utf8"):
        if candidate in available:
            return candidate
    return "C"


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            raise LayoutFailure("Haxe server exited before layout probes")
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise LayoutFailure("Haxe server did not accept layout probe connections")


def determinism(first: dict[str, Rendered], root: Path) -> None:
    locale = alternate_locale()
    for layout in LAYOUTS:
        repeated = render(
            root / f"{layout}-reversed",
            layout,
            f"{layout} reverse/locale render",
            reverse=True,
            locale=locale,
        )
        assert_equal(first[layout], repeated, f"{layout} cold/reverse/locale")

    port = available_port()
    server = subprocess.Popen(
        [tool("haxe"), "--wait", str(port)],
        cwd=ROOT,
        env=environment("C", server=True),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_server(server, port)
        for layout in LAYOUTS:
            warm = render(
                root / f"{layout}-warm",
                layout,
                f"{layout} warm-server render",
                connect=str(port),
            )
            assert_equal(first[layout], warm, f"{layout} cold/warm")
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def check_stale_switch(root: Path) -> None:
    initial = compile_target(root, "split", report=False)
    if initial.returncode != 0 or initial.stderr:
        raise LayoutFailure("initial stale-output split render failed")
    user = root / "user-note.txt"
    user.write_text("preserve me\n", encoding="utf-8")
    previous = "split"
    for layout in ("package", "unity", "split"):
        switched = compile_target(root, layout, report=False)
        if switched.returncode != 0 or switched.stderr:
            raise LayoutFailure(f"{previous}-to-{layout} stale-output switch failed")
        expected = EXPECTED_FILES[layout] | {"user-note.txt"}
        actual = generated_files(root)
        if actual != expected:
            raise LayoutFailure(
                f"{layout} switch did not replace the complete owned {previous} "
                f"file set: {sorted(actual)!r}"
            )
        previous = layout


def check_report_paths(report: dict[str, object], *roots: Path) -> None:
    serialized = json.dumps(report, ensure_ascii=False, sort_keys=True)
    leaked = [str(path) for path in (ROOT, *roots) if str(path) in serialized]
    if leaked:
        raise LayoutFailure(
            "native fixture report leaked absolute paths: " + ", ".join(leaked)
        )


def native(rendered: Rendered, oracle: str, root: Path, requested: str) -> None:
    fixture = root / rendered.layout
    shutil.copytree(rendered.output, fixture / "generated")
    (fixture / "native").mkdir(parents=True)
    shutil.copy2(NATIVE / "harness.c", fixture / "native/harness.c")
    shutil.copy2(NATIVE / "entry.h", fixture / "native/entry.h")
    trace_names = [
        c_name
        for source_symbol, c_name in rendered.semantic_symbols
        if source_symbol == "layout.Main.trace"
    ]
    if len(trace_names) != 1:
        raise LayoutFailure(
            f"cannot resolve one layout.Main.trace C symbol: {trace_names!r}"
        )
    (fixture / "native/method_symbols.h").write_text(
        "#ifndef HXC_LAYOUT_TEST_METHOD_SYMBOLS_H_INCLUDED\n"
        "#define HXC_LAYOUT_TEST_METHOD_SYMBOLS_H_INCLUDED\n\n"
        "/* Test-only alias projected from hxc.symbols.json. */\n"
        f"#define HXC_LAYOUT_TRACE {trace_names[0]}\n\n"
        "#endif /* HXC_LAYOUT_TEST_METHOD_SYMBOLS_H_INCLUDED */\n",
        encoding="utf-8",
        newline="\n",
    )
    sources = tuple(
        f"generated/{path}" for path in SOURCES_BY_LAYOUT[rendered.layout]
    )
    headers = tuple(
        f"generated/{path}" for path in HEADERS_BY_LAYOUT[rendered.layout]
    )
    include = fixture / "generated/include"
    for toolchain in resolve_toolchains(requested, repository_root=ROOT):
        for header in HEADERS_BY_LAYOUT[rendered.layout]:
            check = subprocess.run(
                [
                    toolchain.compiler,
                    *STRICT_FLAGS,
                    "-I",
                    str(include),
                    "-x",
                    "c",
                    "-fsyntax-only",
                    "-",
                ],
                input=f'#include "{header.removeprefix("include/")}"\n',
                text=True,
                capture_output=True,
                check=False,
                timeout=30,
            )
            if check.returncode != 0 or check.stdout or check.stderr:
                raise LayoutFailure(
                    f"{toolchain.family} rejected standalone header {header}\n"
                    f"{check.stdout}{check.stderr}"
                )
    requested_coverage = frozenset(
        {f"{rendered.layout}-layout", "strict-c11"}
    )
    for optimization in ("-O0", "-O2"):
        mode = optimization[1:].lower()
        entry_project = CFixtureProject(
            f"project-layout-{rendered.layout}-entry",
            sources,
            headers,
            ("generated/include",),
            "",
            (f"{rendered.layout}-layout", "generated-entry", "strict-c11"),
        )
        entry_report = run_c_fixture_corpus(
            suite=f"project-layout-{rendered.layout}-entry-{mode}",
            projects=(entry_project,),
            fixture_root=fixture,
            build_root=root / f"entry-{mode}",
            repository_root=ROOT,
            requested_toolchain=requested,
            strict_flags=(*STRICT_FLAGS, optimization),
            timeout_seconds=60,
        )
        validate_report(
            entry_report,
            required_coverage=requested_coverage | {"generated-entry"},
        )
        check_report_paths(entry_report, fixture, root / f"entry-{mode}")

        oracle_project = CFixtureProject(
            f"project-layout-{rendered.layout}-oracle",
            (*sources, "native/harness.c"),
            (*headers, "native/entry.h", "native/method_symbols.h"),
            ("generated/include", "native"),
            oracle,
            (f"{rendered.layout}-layout", "eval-differential", "strict-c11"),
        )
        oracle_report = run_c_fixture_corpus(
            suite=f"project-layout-{rendered.layout}-oracle-{mode}",
            projects=(oracle_project,),
            fixture_root=fixture,
            build_root=root / f"oracle-{mode}",
            repository_root=ROOT,
            requested_toolchain=requested,
            strict_flags=(
                *STRICT_FLAGS,
                optimization,
                "-Dmain=hxc_layout_generated_main",
                "-include",
                "entry.h",
            ),
            timeout_seconds=60,
        )
        validate_report(
            oracle_report,
            required_coverage=requested_coverage | {"eval-differential"},
        )
        check_report_paths(oracle_report, fixture, root / f"oracle-{mode}")


def oracle() -> str:
    result = subprocess.run(
        [tool("haxe"), "--cwd", str(CASE), ORACLE_HXML.name],
        cwd=ROOT,
        env=environment("C", server=False),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout != "35\n" or result.stderr:
        raise LayoutFailure("project-layout Eval oracle drifted")
    return result.stdout


def planner_probe() -> None:
    result = subprocess.run(
        [tool("haxe"), "--cwd", str(CASE), PLANNER_HXML.name],
        cwd=ROOT,
        env=environment("C", server=False),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise LayoutFailure(
            "project-layout synthetic path-policy probe failed\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def check_invalid_layout(output: Path) -> None:
    result = subprocess.run(
        [
            tool("haxe"),
            "--cwd",
            str(CASE),
            BUILD_HXML.name,
            "-D",
            "hxc_project_layout=invalid",
            "--custom-target",
            f"c={output}",
        ],
        cwd=ROOT,
        env=environment("C", server=False),
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    combined = result.stdout + result.stderr
    if (
        result.returncode != 1
        or "HXC0003" not in combined
        or "invalid hxc_project_layout `invalid`; expected split, package, or unity"
        not in combined
        or list(output.rglob("*"))
    ):
        raise LayoutFailure(
            "invalid project-layout define did not fail closed without output\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    args = parser.parse_args()
    try:
        planner_probe()
        with tempfile.TemporaryDirectory(prefix="hxc-project-layout-") as temporary:
            root = Path(temporary)
            check_invalid_layout(root / "invalid")
            expected = oracle()
            first = {
                layout: render(root / layout, layout, f"first {layout} render")
                for layout in LAYOUTS
            }
            baseline = first["split"]
            for layout in LAYOUTS[1:]:
                candidate = first[layout]
                if (
                    baseline.hxcir != candidate.hxcir
                    or baseline.semantic_symbols != candidate.semantic_symbols
                    or baseline.runtime_plan != candidate.runtime_plan
                ):
                    raise LayoutFailure(
                        f"{layout} file assignment changed semantic plans or finalized names"
                    )
            determinism(first, root / "determinism")
            check_stale_switch(root / "switch")
            for rendered in first.values():
                native(rendered, expected, root / "native", args.toolchain)
    except (
        CFixtureFailure,
        LayoutFailure,
        OSError,
        UnicodeError,
        ValueError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"project-layout: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "project-layout: OK: split-default/package/unity semantic parity, exact manifests, "
        "hard/soft dependency classification, cold/reverse/locale/warm determinism, "
        "stale ownership, standalone headers, "
        "and requested/available native toolchain O0/O2 Eval parity passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
"""Build and run the precise collector contract under strict native lanes."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import tempfile
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
INCLUDE = ROOT / "runtime/hxrt/include"
SOURCES = (
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/object.c",
    ROOT / "runtime/hxrt/src/gc.c",
)
CONTRACT = ROOT / "runtime/hxrt/test/gc_contract.c"
FREESTANDING = ROOT / "runtime/hxrt/test/gc_freestanding.c"
CPP_HEADER = ROOT / "runtime/hxrt/test/gc_header_cpp.cpp"
GENERATED_ROOT_HARNESS = ROOT / "runtime/hxrt/test/generated_root_frame_harness.c"
GENERATED_ROOT_HXML = ROOT / "test/runtime/gc/generated_root_frame.hxml"
GENERATED_ROOT_PREFIX = "HXC_GENERATED_ROOT_FRAME="

C_FLAGS = (
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
CXX_FLAGS = (
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


class GcTestFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Toolchain:
    family: str
    cc: str
    cxx: str


def run(command: list[str], *, timeout: int = 30) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=timeout,
    )


def compiler_family(command: str) -> str:
    result = run([command, "--version"])
    if result.returncode != 0:
        return "unknown"
    version = (result.stdout + result.stderr).lower()
    if "clang" in version:
        return "clang"
    if "free software foundation" in version or "gcc" in version or "g++" in version:
        return "gcc"
    return "unknown"


def resolve(requested: str) -> list[Toolchain]:
    names = (("gcc", "gcc", "g++"), ("clang", "clang", "clang++"))
    found: list[Toolchain] = []
    for family, cc_name, cxx_name in names:
        if requested != "auto" and requested != family:
            continue
        cc = shutil.which(cc_name)
        cxx = shutil.which(cxx_name)
        if cc is None or cxx is None or compiler_family(cc) != family or compiler_family(cxx) != family:
            if requested == family:
                raise GcTestFailure(f"requested {family} C/C++ toolchain is unavailable or misidentified")
            print(f"gc-runtime: SKIP optional {family}: unavailable or command identity mismatch")
            continue
        found.append(Toolchain(family, cc, cxx))
    if not found:
        raise GcTestFailure("no identified GCC or Clang C/C++ toolchain is available")
    return found


def require_success(result: subprocess.CompletedProcess[str], label: str) -> None:
    if result.returncode != 0:
        raise GcTestFailure(
            f"{label} failed with exit {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def render_generated_root_frame(
    label: str,
) -> tuple[str, list[dict[str, object]]]:
    """Render compiler-owned root operations in an isolated Haxe process."""
    haxe = ROOT / "node_modules/.bin/haxe"
    if not haxe.is_file():
        raise GcTestFailure("generated root-frame evidence requires pinned Haxe")
    environment = os.environ.copy()
    # This is a cold determinism probe, not the interactive warm-server lane.
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [str(haxe), str(GENERATED_ROOT_HXML)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    require_success(result, label)
    lines = [
        line
        for line in result.stdout.splitlines()
        if line.startswith(GENERATED_ROOT_PREFIX)
    ]
    if len(lines) != 1 or result.stderr:
        raise GcTestFailure(
            f"{label} emitted an invalid report envelope\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    try:
        report = json.loads(lines[0][len(GENERATED_ROOT_PREFIX) :])
    except json.JSONDecodeError as error:
        raise GcTestFailure(f"{label} emitted invalid JSON: {error}") from error
    source = report.get("source") if isinstance(report, dict) else None
    if not isinstance(source, str) or not source.endswith("\n"):
        raise GcTestFailure(f"{label} omitted normalized structural C source")
    if str(ROOT) in source or "\\" in source:
        raise GcTestFailure(f"{label} leaked a machine-local path")
    projects = report.get("projects") if isinstance(report, dict) else None
    if not isinstance(projects, list):
        raise GcTestFailure(f"{label} omitted project-layout evidence")
    validate_generated_projects(projects, label)
    return source, projects


def validate_generated_projects(
    projects: list[object],
    label: str,
) -> None:
    layouts: list[str] = []
    for project in projects:
        if not isinstance(project, dict) or set(project) != {"layout", "headers", "sources"}:
            raise GcTestFailure(f"{label} emitted an invalid project record")
        layout = project.get("layout")
        headers = project.get("headers")
        sources = project.get("sources")
        if not isinstance(layout, str) or not isinstance(headers, list) or not isinstance(sources, list):
            raise GcTestFailure(f"{label} emitted malformed project fields")
        layouts.append(layout)
        for collection_name, collection in (("headers", headers), ("sources", sources)):
            paths: list[str] = []
            for item in collection:
                if (
                    not isinstance(item, dict)
                    or set(item) != {"path", "content"}
                    or not isinstance(item.get("path"), str)
                    or not isinstance(item.get("content"), str)
                ):
                    raise GcTestFailure(f"{label} emitted an invalid {collection_name} record")
                path = item["path"]
                content = item["content"]
                if (
                    path.startswith("/")
                    or "\\" in path
                    or any(part in ("", ".", "..") for part in path.split("/"))
                    or str(ROOT) in content
                    or "\\" in content
                ):
                    raise GcTestFailure(f"{label} emitted a non-portable project artifact")
                paths.append(path)
            if len(paths) != len(set(paths)) or not paths:
                raise GcTestFailure(f"{label} emitted duplicate or empty {collection_name}")
    if layouts != ["unity", "split", "package"]:
        raise GcTestFailure(f"{label} project layouts drifted: {layouts!r}")


def generated_root_frame_lane(
    toolchain: Toolchain,
    build: Path,
    source: Path,
) -> None:
    """Run emitted root frames against the independent runtime observer."""
    for optimization in ("O0", "O2"):
        executable = build / f"generated-root-frame-{optimization}"
        command = [
            toolchain.cc,
            *C_FLAGS,
            f"-{optimization}",
            f"-I{INCLUDE}",
            *(str(path) for path in SOURCES),
            str(source),
            str(GENERATED_ROOT_HARNESS),
            "-o",
            str(executable),
        ]
        require_success(
            run(command),
            f"{toolchain.family} {optimization} generated root-frame build",
        )
        result = run([str(executable)])
        require_success(
            result,
            f"{toolchain.family} {optimization} generated root-frame run",
        )
        if result.stderr or result.stdout != "generated-root-frame: OK finalizers=2\n":
            raise GcTestFailure(
                f"{toolchain.family} {optimization} generated root-frame report drifted\n"
                f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
            )


def generated_project_lane(
    toolchain: Toolchain,
    build: Path,
    projects: list[dict[str, object]],
) -> None:
    """Compile, execute, and C++-consume all collector-owning project layouts."""
    for project in projects:
        layout = project["layout"]
        if not isinstance(layout, str):
            raise GcTestFailure("validated project layout lost its text identity")
        project_root = build / f"project-{layout}"
        source_paths: list[Path] = []
        for item in [*project["headers"], *project["sources"]]:
            if not isinstance(item, dict):
                raise GcTestFailure("validated project artifact lost its record shape")
            relative = item["path"]
            content = item["content"]
            if not isinstance(relative, str) or not isinstance(content, str):
                raise GcTestFailure("validated project artifact lost path/content text")
            destination = project_root / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_text(content, encoding="utf-8", newline="\n")
            if relative.endswith(".c"):
                source_paths.append(destination)

        executable = project_root / "managed-project"
        require_success(
            run(
                [
                    toolchain.cc,
                    *C_FLAGS,
                    "-O2",
                    f"-I{INCLUDE}",
                    f"-I{project_root / 'include'}",
                    *(str(path) for path in SOURCES),
                    *(str(path) for path in source_paths),
                    "-o",
                    str(executable),
                ]
            ),
            f"{toolchain.family} {layout} managed project build",
        )
        result = run([str(executable)])
        require_success(result, f"{toolchain.family} {layout} managed project run")
        if result.stdout or result.stderr:
            raise GcTestFailure(f"{toolchain.family} {layout} managed project wrote unexpected output")

        cpp_consumer = project_root / "header-consumer.cpp"
        cpp_consumer.write_text(
            '#include "hxc/program.h"\nint main() { return 0; }\n',
            encoding="utf-8",
            newline="\n",
        )
        require_success(
            run(
                [
                    toolchain.cxx,
                    *CXX_FLAGS,
                    f"-I{INCLUDE}",
                    f"-I{project_root / 'include'}",
                    str(cpp_consumer),
                    "-o",
                    str(project_root / "header-consumer"),
                ]
            ),
            f"{toolchain.family} {layout} managed project C++17 header",
        )


def compile_and_run(
    toolchain: Toolchain,
    build: Path,
    *,
    optimization: str,
    sanitizer: bool,
) -> None:
    suffix = f"{optimization}{'-san' if sanitizer else ''}"
    executable = build / f"gc-contract-{suffix}"
    command = [toolchain.cc, *C_FLAGS, f"-{optimization}", f"-I{INCLUDE}"]
    if sanitizer:
        command.extend(("-g", "-fsanitize=address,undefined", "-fno-omit-frame-pointer"))
    command.extend(str(path) for path in (*SOURCES, CONTRACT))
    command.extend(("-o", str(executable)))
    require_success(run(command), f"{toolchain.family} {suffix} collector build")
    result = run([str(executable)])
    require_success(result, f"{toolchain.family} {suffix} collector run")
    if result.stderr or result.stdout.strip() != (
        "gc-contract: OK allocations=265 collections=137 reclaimed=265 pause_ticks=973"
    ):
        raise GcTestFailure(
            f"{toolchain.family} {suffix} collector report drifted\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def freestanding_lane(toolchain: Toolchain, build: Path) -> None:
    executable = build / "gc-freestanding"
    command = [
        toolchain.cc,
        *C_FLAGS,
        "-O2",
        "-DHXC_FREESTANDING",
        f"-I{INCLUDE}",
        *(str(path) for path in (*SOURCES, FREESTANDING)),
        "-o",
        str(executable),
    ]
    require_success(run(command), f"{toolchain.family} freestanding collector build")
    result = run([str(executable)])
    require_success(result, f"{toolchain.family} freestanding collector run")
    if result.stderr or result.stdout != "gc-freestanding: OK\n":
        raise GcTestFailure(f"{toolchain.family} freestanding collector report drifted")

    gc_object = build / "gc-freestanding.o"
    require_success(
        run(
            [
                toolchain.cc,
                *C_FLAGS,
                "-O2",
                "-DHXC_FREESTANDING",
                f"-I{INCLUDE}",
                "-c",
                str(SOURCES[-1]),
                "-o",
                str(gc_object),
            ]
        ),
        f"{toolchain.family} freestanding collector object",
    )
    nm = shutil.which("nm")
    if nm is None:
        raise GcTestFailure("freestanding collector evidence requires nm")
    symbols = run([nm, "-u", str(gc_object)])
    require_success(symbols, f"{toolchain.family} freestanding collector symbol scan")
    forbidden = {"malloc", "calloc", "realloc", "free", "aligned_alloc", "posix_memalign"}
    observed = {
        line.split()[-1].lstrip("_").split("@", 1)[0]
        for line in symbols.stdout.splitlines()
        if line.split()
    }
    if forbidden & observed:
        raise GcTestFailure(
            f"{toolchain.family} freestanding collector retained hosted allocation symbols"
        )


def cpp_lane(toolchain: Toolchain, build: Path) -> None:
    executable = build / "gc-header-cpp"
    result = run(
        [
            toolchain.cxx,
            *CXX_FLAGS,
            f"-I{INCLUDE}",
            str(CPP_HEADER),
            "-o",
            str(executable),
        ]
    )
    require_success(result, f"{toolchain.family} C++17 collector header")
    require_success(run([str(executable)]), f"{toolchain.family} C++17 collector header run")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    arguments = parser.parse_args()
    toolchains = resolve(arguments.toolchain)
    first_root_source, first_projects = render_generated_root_frame("first generated root-frame render")
    second_root_source, second_projects = render_generated_root_frame("second generated root-frame render")
    if first_root_source != second_root_source or first_projects != second_projects:
        raise GcTestFailure("generated root-frame C was not byte-identical across cold renders")
    with tempfile.TemporaryDirectory(prefix="haxe-c-gc-runtime-") as temporary:
        root = Path(temporary)
        generated_root_source = root / "generated-root-frame.c"
        generated_root_source.write_text(first_root_source, encoding="utf-8", newline="\n")
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir(parents=True)
            compile_and_run(toolchain, build, optimization="O0", sanitizer=False)
            compile_and_run(toolchain, build, optimization="O2", sanitizer=False)
            compile_and_run(toolchain, build, optimization="O1", sanitizer=True)
            freestanding_lane(toolchain, build)
            cpp_lane(toolchain, build)
            generated_root_frame_lane(toolchain, build, generated_root_source)
            generated_project_lane(toolchain, build, first_projects)
    families = ", ".join(toolchain.family for toolchain in toolchains)
    print(
        "gc-runtime: OK: precise cycles, exact stack/global/thread roots, pins, "
        "pressure, reports, generated root lifetimes, freestanding allocation, "
        f"C++17, and sanitizers passed ({families})"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except GcTestFailure as error:
        print(f"gc-runtime: ERROR: {error}")
        raise SystemExit(1)

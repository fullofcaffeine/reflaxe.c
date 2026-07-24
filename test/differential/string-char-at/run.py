#!/usr/bin/env python3
"""Prove ordinary Haxe String.charAt against Eval and strict generated C."""

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
LAYOUTS = ("split", "package", "unity")
TOOLCHAINS = ("gcc", "clang")
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
EXPECTED_STDOUT = "string-char-at: OK\n"
EXPECTED_FEATURES = [
    "runtime-base",
    "status",
    "string-literal",
    "io",
    "string-scalar",
]
EXPECTED_ARTIFACTS = [
    "runtime/include/hxrt/base.h",
    "runtime/include/hxrt/io.h",
    "runtime/include/hxrt/status.h",
    "runtime/include/hxrt/string_decode.h",
    "runtime/include/hxrt/string_literal.h",
    "runtime/include/hxrt/string_scalar.h",
    "runtime/src/io.c",
    "runtime/src/string_scalar.c",
]
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


class StringCharAtFailure(RuntimeError):
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
    values: list[Toolchain] = []
    for family in families:
        compiler = shutil.which(family)
        if compiler is None:
            if selected != "auto":
                raise StringCharAtFailure(f"required C compiler is missing: {family}")
            print(f"string-char-at: SKIP optional {family}: missing command")
            continue
        identity = subprocess.run(
            [compiler, "--version"],
            cwd=ROOT,
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
                raise StringCharAtFailure(f"{family} command identifies as {actual}")
            print(f"string-char-at: SKIP optional {family}: command identifies as {actual}")
            continue
        values.append(Toolchain(family, compiler))
    if not values:
        raise StringCharAtFailure("no identity-matching strict C11 compiler is available")
    return values


def run_eval_oracle() -> None:
    observations: list[tuple[int, str, str]] = []
    for _ in range(2):
        result = subprocess.run(
            [development_tool("haxe"), "oracle.hxml"],
            cwd=GENERATED,
            env=haxe_environment(),
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        observations.append((result.returncode, result.stdout, result.stderr))
    expected = (0, EXPECTED_STDOUT, "")
    if observations != [expected, expected]:
        raise StringCharAtFailure(f"pinned Eval charAt oracle drifted: {observations!r}")


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
            "-D",
            "hxc_runtime_diagnostics=off",
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
    values = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(values) != 1:
        raise StringCharAtFailure("charAt compile omitted its one HxcIR report")
    report = json.loads(values[0])
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise StringCharAtFailure("charAt compile omitted validated HxcIR text")
    return hxcir


def string_list(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise StringCharAtFailure(f"{label} must be a string array")
    return list(value)


def validate_project(output: Path, hxcir: str) -> None:
    marker = (
        'call dispatch=runtime(feature="string-scalar",operation="char-at") '
        'arguments=["parameter.0","parameter.1"] returns=string-utf8 failure=none'
    )
    if marker not in hxcir:
        raise StringCharAtFailure("validated HxcIR omitted the total String/Int charAt call")
    if hxcir.count('runtime(feature="string-scalar",operation="char-at")') != 1:
        raise StringCharAtFailure("validated HxcIR must retain exactly one shared charAt operation")
    for required in (
        'string-utf8(bytes=6,value="A\\u0000🙂")',
        'string-utf8(bytes=4,value="🙂")',
        "constant value=int(-1)",
        'string-utf8(bytes=0,value="")',
    ):
        if required not in hxcir:
            raise StringCharAtFailure(f"validated HxcIR omitted boundary evidence {required}")

    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("status") != "analyzed-runtime-features"
        or plan.get("planPurpose") != "compiler-program"
        or string_list(plan.get("features"), "runtime features") != EXPECTED_FEATURES
        or string_list(plan.get("artifacts"), "runtime artifacts") != EXPECTED_ARTIFACTS
    ):
        raise StringCharAtFailure("charAt selected the wrong dependency-closed runtime slice")
    reasons = [
        reason
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict) and reason.get("featureId") == "string-scalar"
    ]
    if (
        len(reasons) != 1
        or reasons[0].get("operationId") != "char-at"
        or reasons[0].get("kind") != "runtime-operation"
        or reasons[0].get("surface")
        != "ordinary Haxe String.charAt with Unicode-scalar indexing"
    ):
        raise StringCharAtFailure("charAt runtime root lost its typed source provenance")
    decisions = string_list(plan.get("directDecisions"), "runtime direct decisions")
    if "allocation-free-unicode-scalar-strings" not in decisions:
        raise StringCharAtFailure("runtime plan omitted the allocation-free String decision")
    for forbidden in ("alloc", "string", "object", "gc", "dynamic", "reflection"):
        if forbidden in EXPECTED_FEATURES:
            raise StringCharAtFailure(f"test expectation accidentally selected broad feature {forbidden}")

    stdlib = json.loads((output / "hxc.stdlib-report.json").read_text(encoding="utf-8"))
    if (
        stdlib.get("modules") != ["String", "Sys"]
        or stdlib.get("capabilities") != ["char-at", "static-value", "sys-println-literal"]
    ):
        raise StringCharAtFailure("bounded String stdlib report drifted")

    source_text = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "src").rglob("*.c"))
    )
    if source_text.count("hxc_string_char_at(") != 1:
        raise StringCharAtFailure("generated C omitted or duplicated the shared charAt call")
    for forbidden in ("hxc_alloc", "hxc_owned_string", "hxc_object", "hxc_gc", "goto "):
        if forbidden in source_text:
            raise StringCharAtFailure(f"generated charAt C retained forbidden shape {forbidden!r}")
    if (output / "runtime/src/string.c").exists() or (output / "runtime/src/allocator.c").exists():
        raise StringCharAtFailure("charAt packaged owned String or allocator source")


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10.0
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise StringCharAtFailure(
                f"Haxe server exited before charAt requests: {stdout!r} {stderr!r}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise StringCharAtFailure("Haxe server did not accept charAt determinism requests")


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
            if result.returncode != 0 or result.stdout or result.stderr:
                raise StringCharAtFailure(
                    f"{label} warm-server charAt compile failed: "
                    f"{result.stdout!r} {result.stderr!r}"
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
            if result.returncode != 0 or result.stderr:
                raise StringCharAtFailure(
                    f"{label} charAt compile failed\n"
                    f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
                )
        if generated_tree(normal) != generated_tree(reverse):
            raise StringCharAtFailure(f"{layout} charAt output changed under reversed discovery")
        projects[layout] = normal
        if layout == "split":
            validate_project(normal, extract_hxcir(first))

    server_first, server_second = render_server_pair(root)
    split_tree = generated_tree(projects["split"])
    if generated_tree(server_first) != split_tree or generated_tree(server_second) != split_tree:
        raise StringCharAtFailure("split charAt output changed under warm compiler-server reuse")
    return projects


def plausible_output_exists(output: Path) -> bool:
    return output.exists() and any(output.rglob("*"))


def validate_fail_closed(root: Path) -> None:
    unsupported_output = root / "unsupported-method"
    unsupported = compile_haxe(NEGATIVE, unsupported_output)
    expected = "TCall(String.toUpperCase:not-yet-admitted)"
    if unsupported.returncode == 0 or "HXC1001:" not in unsupported.stderr or expected not in unsupported.stderr:
        raise StringCharAtFailure(
            f"unsupported String method did not fail at its intrinsic owner: {unsupported.stderr!r}"
        )
    if plausible_output_exists(unsupported_output):
        raise StringCharAtFailure("unsupported String method left plausible generated output")

    none_output = root / "runtime-none"
    none = compile_haxe(GENERATED, none_output, defines=("hxc_runtime=none",))
    if (
        none.returncode == 0
        or "HXC2000:" not in none.stderr
        or "runtime.string-scalar.char-at." not in none.stderr
        or "string-scalar -> status -> runtime-base" not in none.stderr
        or "string-scalar -> string-literal -> runtime-base" not in none.stderr
    ):
        raise StringCharAtFailure(f"runtime=none did not explain the charAt closure: {none.stderr!r}")
    if plausible_output_exists(none_output):
        raise StringCharAtFailure("runtime=none charAt rejection left plausible generated output")


def project_build_inputs(project: Path) -> tuple[list[Path], list[Path]]:
    manifest = json.loads((project / "hxc.manifest.json").read_text(encoding="utf-8"))
    build = manifest.get("build")
    if not isinstance(build, dict):
        raise StringCharAtFailure("generated charAt manifest omitted its build plan")
    source_values = string_list(build.get("sources"), "generated sources")
    include_values = string_list(build.get("includeDirectories"), "generated include roots")
    sources = [(project / value).resolve() for value in source_values]
    includes = [(project / value).resolve() for value in include_values]
    for path in (*sources, *includes):
        path.relative_to(project.resolve())
        if not path.exists():
            raise StringCharAtFailure(f"generated build input is missing: {path}")
    return sources, includes


def compile_and_run(
    toolchain: Toolchain,
    project: Path,
    executable: Path,
    flags: tuple[str, ...],
) -> None:
    sources, include_roots = project_build_inputs(project)
    command = [
        toolchain.compiler,
        *STRICT_FLAGS,
        *flags,
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
        raise StringCharAtFailure(
            f"{toolchain.family} strict charAt compile failed\n"
            f"command={command!r}\nstdout={compiled.stdout!r}\nstderr={compiled.stderr!r}"
        )
    executed = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if executed.returncode != 0 or executed.stdout != EXPECTED_STDOUT or executed.stderr:
        raise StringCharAtFailure(
            f"{toolchain.family} charAt execution drifted: exit={executed.returncode} "
            f"stdout={executed.stdout!r} stderr={executed.stderr!r}"
        )


def inspect_symbols(toolchain: Toolchain, executable: Path) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise StringCharAtFailure(f"{toolchain.family} charAt evidence requires nm")
    result = subprocess.run(
        [nm, str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if result.returncode != 0 or "hxc_string_char_at" not in result.stdout:
        raise StringCharAtFailure(f"{toolchain.family} charAt executable omitted its selected symbol")
    for forbidden in ("hxc_allocator", "hxc_owned_string", "hxc_gc", "hxc_object"):
        if forbidden in result.stdout:
            raise StringCharAtFailure(
                f"{toolchain.family} charAt executable retained unrelated symbol {forbidden}"
            )


def run_native(toolchains: list[Toolchain], projects: dict[str, Path], root: Path) -> None:
    for toolchain in toolchains:
        build = root / toolchain.family
        build.mkdir()
        for layout, project in projects.items():
            for optimization in ("-O0", "-O2"):
                executable = build / f"{layout}-{optimization[1:].lower()}"
                compile_and_run(toolchain, project, executable, (optimization,))
                inspect_symbols(toolchain, executable)
        if toolchain.family == "clang":
            executable = build / "split-sanitized"
            compile_and_run(toolchain, projects["split"], executable, SANITIZER_FLAGS)
            inspect_symbols(toolchain, executable)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", *TOOLCHAINS), default="auto")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        toolchains = resolve_toolchains(args.toolchain)
        run_eval_oracle()
        with tempfile.TemporaryDirectory(prefix="reflaxe-c-string-char-at-") as temporary:
            root = Path(temporary)
            projects = render_projects(root)
            validate_fail_closed(root)
            run_native(toolchains, projects, root)
    except (
        StringCharAtFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"string-char-at: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    print(
        "string-char-at: OK: "
        f"{families}; Eval parity, ASCII/non-BMP/NUL/bounds, exact allocation-free "
        "runtime selection, split/package/unity, cold/reverse/warm determinism, "
        "unsupported-owner/runtime-none failures, strict C11, and sanitizers passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

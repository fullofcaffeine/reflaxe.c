#!/usr/bin/env python3
"""Compare the native UTF-8 runtime contract with the pinned Haxe scalar oracle."""

from __future__ import annotations

import argparse
import json
import os
import re
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
ORACLE_HXML = CASE / "oracle.hxml"
FIXTURE = CASE / "string_runtime.c"
GENERATED = CASE / "generated"
NEGATIVE = CASE / "negative"
NATIVE_TEXT = CASE / "native_text"
NATIVE_TEXT_PRIVATE_INLINE = CASE / "native_text_private_inline"
NATIVE_TEXT_NUL = CASE / "native_text_nul"
NATIVE_TEXT_UNTERMINATED = CASE / "native_text_unterminated"
TEXT_OBSERVER_SOURCE = CASE / "native/text_observer.c"
TEXT_OBSERVER_INCLUDE = CASE / "native/include"
INCLUDE = ROOT / "runtime/hxrt/include"
SOURCES = (
    ROOT / "runtime/hxrt/src/abi.c",
    ROOT / "runtime/hxrt/src/status.c",
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/array.c",
    ROOT / "runtime/hxrt/src/string.c",
    ROOT / "runtime/hxrt/src/string_scalar.c",
    ROOT / "runtime/hxrt/src/string_split.c",
)
EXPECTED_TRACE = "1,128512,3,1,2,😀\n"
EXPECTED_GENERATED_STDOUT = "string-owned: OK\n"
LAYOUTS = ("split", "package", "unity")
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
EXPECTED_GENERATED_FEATURES = [
    "runtime-base",
    "status",
    "alloc",
    "array",
    "string-literal",
    "io",
    "string-scalar",
    "string",
    "string-split",
]
EXPECTED_GENERATED_ARTIFACTS = [
    "runtime/include/hxrt/allocator.h",
    "runtime/include/hxrt/array.h",
    "runtime/include/hxrt/base.h",
    "runtime/include/hxrt/io.h",
    "runtime/include/hxrt/status.h",
    "runtime/include/hxrt/string.h",
    "runtime/include/hxrt/string_decode.h",
    "runtime/include/hxrt/string_literal.h",
    "runtime/include/hxrt/string_scalar.h",
    "runtime/include/hxrt/string_split.h",
    "runtime/src/allocator.c",
    "runtime/src/array.c",
    "runtime/src/io.c",
    "runtime/src/string.c",
    "runtime/src/string_scalar.c",
    "runtime/src/string_split.c",
]
TOOLCHAINS = ("gcc", "clang")
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
    "-DHXC_FREESTANDING=1",
)
SANITIZER_FLAGS = (
    "-O1",
    "-g",
    "-fno-omit-frame-pointer",
    "-fno-sanitize-recover=all",
    "-fsanitize=address,undefined",
)


class StringRuntimeFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Toolchain:
    family: str
    compiler: str
    version: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(*, server: bool = False) -> dict[str, str]:
    """Use an isolated process unless a test explicitly owns a warm server."""
    environment = os.environ.copy()
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


def compiler_identity(executable: str) -> tuple[str, str]:
    result = subprocess.run(
        [executable, "--version"],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        raise StringRuntimeFailure(f"cannot identify compiler {executable}")
    output = (result.stdout + result.stderr).strip()
    lowered = output.lower()
    family = (
        "clang"
        if "clang" in lowered
        else "gcc"
        if "gcc" in lowered or "free software foundation" in lowered
        else "unknown"
    )
    return family, output.splitlines()[0] if output else "unknown version"


def resolve_toolchain(family: str, *, required: bool) -> Toolchain | None:
    compiler = shutil.which(family)
    if compiler is None:
        if required:
            raise StringRuntimeFailure(
                f"required string-runtime compiler is missing: {family}"
            )
        print(f"string-runtime: SKIP optional {family}: missing command")
        return None
    actual, version = compiler_identity(compiler)
    if actual != family:
        if required:
            raise StringRuntimeFailure(
                f"required {family} command identifies as {actual}: {version}"
            )
        print(
            f"string-runtime: SKIP optional {family}: command identifies as {actual}"
        )
        return None
    return Toolchain(family, compiler, version)


def selected_toolchains(selected: str) -> list[Toolchain]:
    families = TOOLCHAINS if selected == "auto" else (selected,)
    toolchains = [
        toolchain
        for family in families
        if (
            toolchain := resolve_toolchain(
                family, required=selected != "auto"
            )
        )
        is not None
    ]
    if not toolchains:
        raise StringRuntimeFailure(
            "no strict C11 compiler is available for the string runtime contract"
        )
    return toolchains


def run_oracle() -> str:
    outputs: list[str] = []
    for label in ("first", "second"):
        result = subprocess.run(
            [development_tool("haxe"), str(ORACLE_HXML)],
            cwd=ROOT,
            env=haxe_environment(),
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if result.returncode != 0 or result.stderr:
            raise StringRuntimeFailure(
                f"{label} Haxe scalar-string oracle failed\n"
                f"exit={result.returncode} stdout={result.stdout!r} "
                f"stderr={result.stderr!r}"
            )
        outputs.append(result.stdout)
    if outputs[0] != outputs[1] or outputs[0] != EXPECTED_TRACE:
        raise StringRuntimeFailure(
            "Haxe scalar-string oracle drifted\n"
            f"first={outputs[0]!r} second={outputs[1]!r}"
        )
    return outputs[0]


def compile_native_fixture(
    toolchain: Toolchain,
    build: Path,
    flags: tuple[str, ...],
    label: str,
    expected_trace: str,
) -> Path:
    executable = build / label
    command = [
        toolchain.compiler,
        *STRICT_FLAGS,
        *flags,
        f"-I{INCLUDE}",
        *(str(source) for source in SOURCES),
        str(FIXTURE),
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
        raise StringRuntimeFailure(
            f"{toolchain.family} {label} compile failed\n"
            f"command={command!r}\nstdout={compiled.stdout!r}\n"
            f"stderr={compiled.stderr!r}"
        )
    executed = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if (
        executed.returncode != 0
        or executed.stdout != expected_trace
        or executed.stderr
    ):
        raise StringRuntimeFailure(
            f"{toolchain.family} {label} execution drifted\n"
            f"exit={executed.returncode} stdout={executed.stdout!r} "
            f"stderr={executed.stderr!r}"
        )
    return executable


def inspect_symbols(executable: Path, family: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise StringRuntimeFailure(f"{family} string runtime requires nm evidence")
    result = subprocess.run(
        [nm, str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if result.returncode != 0:
        raise StringRuntimeFailure(f"{family} could not inspect string symbols")
    symbols = result.stdout
    for required in (
        "hxc_string_concat",
        "hxc_string_from_utf8_lossy",
        "hxc_string_borrow_cstring",
    ):
        if required not in symbols:
            raise StringRuntimeFailure(
                f"{family} string link omitted required symbol {required}"
            )
    for forbidden in ("hxc_object", "hxc_gc", "hxc_reflection", "hxc_dynamic"):
        if forbidden in symbols:
            raise StringRuntimeFailure(
                f"{family} string link retained unrelated symbol family {forbidden}"
            )


def run_native(toolchains: list[Toolchain], expected_trace: str) -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-string-runtime-") as temporary:
        root = Path(temporary)
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir(parents=True)
            debug = compile_native_fixture(
                toolchain, build, ("-O0",), "string-runtime-o0", expected_trace
            )
            compile_native_fixture(
                toolchain, build, ("-O2",), "string-runtime-o2", expected_trace
            )
            compile_native_fixture(
                toolchain,
                build,
                SANITIZER_FLAGS,
                "string-runtime-sanitized",
                expected_trace,
            )
            inspect_symbols(debug, toolchain.family)


def run_generated_eval() -> None:
    """Run the ordinary-Haxe fixture twice before involving haxe.c."""
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
    expected = (0, EXPECTED_GENERATED_STDOUT, "")
    if observations != [expected, expected]:
        raise StringRuntimeFailure(
            f"ordinary-Haxe managed String Eval oracle drifted: {observations!r}"
        )


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
    """Compile one isolated ordinary-Haxe fixture through the real target."""
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
        timeout=60,
    )


def generated_tree(output: Path) -> dict[str, bytes]:
    """Read normal compiler artifacts while excluding Reflaxe activity metadata."""
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
        raise StringRuntimeFailure(
            "managed String compile omitted its one validated HxcIR report"
        )
    report = json.loads(values[0])
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise StringRuntimeFailure("managed String HxcIR report is empty")
    return hxcir


def string_list(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise StringRuntimeFailure(f"{label} must be a string array")
    return list(value)


def hxcir_function(hxcir: str, display_name: str) -> str:
    """Return one complete function from the deterministic HxcIR text report."""
    start_marker = f'name="{display_name}"'
    start = hxcir.find(start_marker)
    if start < 0:
        raise StringRuntimeFailure(
            f"managed String HxcIR omitted function {display_name}"
        )
    start = hxcir.rfind("\n  function ", 0, start)
    end_marker = "\n  end function "
    end = hxcir.find(end_marker, start)
    if start < 0 or end < 0:
        raise StringRuntimeFailure(
            f"managed String HxcIR malformed function {display_name}"
        )
    line_end = hxcir.find("\n", end + len(end_marker))
    return hxcir[start : len(hxcir) if line_end < 0 else line_end + 1]


def validate_switch_join_ownership(hxcir: str) -> None:
    """Prove each source switch has one explicit, balanced String owner join."""
    for display_name in (
        "Main.selectInt",
        "Main.selectString",
        "Main.selectEnum",
        "Main.selectAbstract",
    ):
        function = hxcir_function(hxcir, display_name)
        expected_counts = {
            "declare-managed-carrier": 1,
            "ownership=retain-borrowed(runtime(\"string\"))": 2,
            "ownership=move-fresh": 1,
            "move-managed-carrier": 1,
            "terminator throw": 1,
        }
        for operation, expected in expected_counts.items():
            actual = function.count(operation)
            if actual != expected:
                raise StringRuntimeFailure(
                    f"{display_name} HxcIR has {actual} {operation!r} "
                    f"operation(s); expected {expected}"
                )


def validate_std_string_identity_ownership(hxcir: str) -> None:
    """Prove String identity preserves, rather than invents, ownership work."""
    borrowed_return = hxcir_function(hxcir, "Main.renderString")
    if (
        borrowed_return.count("retain-string-return") != 1
        or "std-string" in borrowed_return
    ):
        raise StringRuntimeFailure(
            "Std.string over a borrowed String lost its ordinary return ownership"
        )

    fresh_return = hxcir_function(hxcir, "Main.renderFreshString")
    if (
        fresh_return.count('call dispatch=direct("function.Main.fromCode")') != 1
        or " retain " in fresh_return
        or " release " in fresh_return
        or "std-string" in fresh_return
    ):
        raise StringRuntimeFailure(
            "Std.string over a fresh String must transfer the existing owner "
            "without conversion, retain, or release"
        )

    borrowed_view_return = hxcir_function(hxcir, "Main.renderStringView")
    if (
        borrowed_view_return.count(
            'runtime(feature="string-scalar",operation="substring")'
        )
        != 1
        or borrowed_view_return.count("declare-managed-carrier") != 1
        or borrowed_view_return.count(
            'ownership=retain-borrowed(runtime("string"))'
        )
        != 1
        or borrowed_view_return.count("move-managed-carrier") != 1
        or "retain-string-return" in borrowed_view_return
        or "std-string" in borrowed_view_return
    ):
        raise StringRuntimeFailure(
            "Std.string over a borrowed String view lost its explicit "
            "retain-and-transfer ownership plan"
        )

    consumers = hxcir_function(hxcir, "Main.stringIdentityContractHolds")
    owner = re.search(
        r'string-length-receiver-owner-initialize".*place=local\("([^"]+)"\)',
        consumers,
    )
    cleanup_id = (
        None if owner is None else f"string-temporary.{owner.group(1)}.release"
    )
    if (
        consumers.count("string-length-receiver-owner-initialize") != 1
        or consumers.count("string-length-receiver-borrow") != 1
        or cleanup_id is None
        or consumers.count(cleanup_id) != 2
        or "std-string" in consumers
    ):
        raise StringRuntimeFailure(
            "a directly consumed fresh Std.string(String) result did not retain "
            "one balanced receiver lifetime"
        )


def validate_conditional_view_ownership(hxcir: str) -> None:
    """Prove a retained String view transfers through an outer conditional."""
    for display_name in (
        "Main.selectBorrowedOrView",
        "Main.selectViewOrBorrowed",
    ):
        function = hxcir_function(hxcir, display_name)
        expected_counts = {
            "declare-managed-carrier": 2,
            "ownership=retain-borrowed(runtime(\"string\"))": 2,
            "ownership=move-fresh": 1,
            "move-managed-carrier": 2,
        }
        for operation, expected in expected_counts.items():
            actual = function.count(operation)
            if actual != expected:
                raise StringRuntimeFailure(
                    f"{display_name} HxcIR has {actual} {operation!r} "
                    f"operation(s); expected {expected}"
                )


def validate_conditional_compound_ownership(hxcir: str) -> None:
    """Prove String compound assignment stages its old value across a join."""
    function = hxcir_function(hxcir, "Main.appendConditionalScalar")
    for operation, expected in {
        "compound-string-left-retain": 1,
        "compound-string-left-flow-initialize": 1,
        "compound-string-left-flow-load": 1,
        "compound-string-concat": 1,
        "release-compound-string-target": 1,
        "store-compound-string-result": 1,
    }.items():
        actual = function.count(operation)
        if actual != expected:
            raise StringRuntimeFailure(
                "conditional String compound assignment has "
                f"{actual} {operation!r} operation(s); expected {expected}"
            )


def validate_projected_enum_payload_ownership(hxcir: str) -> None:
    """Prove a fresh enum stays alive until its borrowed String is retained."""
    function = hxcir_function(hxcir, "Main.copiedAggregateAliasesHold")
    enum_owner = re.search(
        r'enum-parameter-ManagedDecoded-receiver-owner-initialize".*'
        r'place=local\("([^"]+)"\)',
        function,
    )
    string_owner = re.search(
        r'retain-string-alias".*place=local\("([^"]+)"\)',
        function,
    )
    operations = [
        function.find('call dispatch=direct("function.Main.decodeManagedText")'),
        function.find("enum-parameter-ManagedDecoded-receiver-owner-initialize"),
        function.find("enum-payload-project"),
        function.find("retain-string-alias"),
        function.find(
            'call dispatch=direct("function.Main.retainDecodedAliases")'
        ),
    ]
    if (
        enum_owner is None
        or string_owner is None
        or any(position < 0 for position in operations)
        or operations != sorted(operations)
    ):
        raise StringRuntimeFailure(
            "the decoded enum payload is not retained before its carrier can be "
            "destroyed"
        )
    expected_cleanup = (
        'cleanup=["cleanup.construction".'
        f'"string-{string_owner.group(1)}.release",'
        '"cleanup.construction".'
        f'"enum-temporary.{enum_owner.group(1)}.release"]'
    )
    if (
        function.count("enum-parameter-ManagedDecoded-receiver-owner-initialize")
        != 1
        or function.count("retain-string-alias") != 1
        or function.count(expected_cleanup) != 1
    ):
        raise StringRuntimeFailure(
            "the decoded payload and fresh enum carrier do not have one ordered "
            "cleanup each"
        )


def validate_generated_project(output: Path, hxcir: str) -> None:
    """Check semantic intent, exact runtime closure, and recognizable C calls."""
    for operation in (
        'runtime(feature="string",operation="from-scalar")',
        'runtime(feature="string",operation="concat")',
        'runtime(feature="string-scalar",operation="char-at")',
        'runtime(feature="string-scalar",operation="char-code-at")',
        'runtime(feature="string-scalar",operation="index-of")',
        'runtime(feature="string-scalar",operation="last-index-of")',
        'runtime(feature="string-scalar",operation="length")',
        'runtime(feature="string-scalar",operation="substring")',
        'runtime(feature="string-split",operation="split")',
        'unary operation="haxe.std.string.bool"',
        'implementation=runtime("string")',
    ):
        if operation not in hxcir:
            raise StringRuntimeFailure(
                f"validated HxcIR omitted managed String evidence {operation}"
            )
    if str(ROOT) in hxcir:
        raise StringRuntimeFailure("managed String HxcIR leaked the checkout path")
    validate_switch_join_ownership(hxcir)
    validate_std_string_identity_ownership(hxcir)
    validate_conditional_view_ownership(hxcir)
    validate_conditional_compound_ownership(hxcir)
    validate_projected_enum_payload_ownership(hxcir)

    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("status") != "analyzed-runtime-features"
        or plan.get("planPurpose") != "compiler-program"
        or string_list(plan.get("features"), "runtime features")
        != EXPECTED_GENERATED_FEATURES
        or string_list(plan.get("artifacts"), "runtime artifacts")
        != EXPECTED_GENERATED_ARTIFACTS
    ):
        raise StringRuntimeFailure(
            "ordinary Haxe managed Strings selected the wrong runtime closure"
        )
    reasons = [
        reason
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict) and reason.get("featureId") == "string"
    ]
    operations = {
        reason.get("operationId")
        for reason in reasons
        if reason.get("kind") == "runtime-operation"
    }
    if operations != {
        "cleanup-release",
        "concat",
        "from-int",
        "from-scalar",
        "retain",
    }:
        raise StringRuntimeFailure(
            f"managed String roots lost typed provenance: {sorted(operations)!r}"
        )
    scalar_operations = {
        reason.get("operationId")
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict)
        and reason.get("featureId") == "string-scalar"
        and reason.get("kind") == "runtime-operation"
    }
    if scalar_operations != {
        "char-at",
        "char-code-at",
        "index-of",
        "last-index-of",
        "length",
        "substring",
    }:
        raise StringRuntimeFailure(
            f"String scalar roots drifted: {sorted(scalar_operations)!r}"
        )
    split_operations = {
        reason.get("operationId")
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict)
        and reason.get("featureId") == "string-split"
        and reason.get("kind") == "runtime-operation"
    }
    if split_operations != {"split"}:
        raise StringRuntimeFailure(
            f"String split roots drifted: {sorted(split_operations)!r}"
        )

    stdlib = json.loads(
        (output / "hxc.stdlib-report.json").read_text(encoding="utf-8")
    )
    expected_modules = ["Array", "String", "Sys", "string"]
    expected_capabilities = [
        "char-at",
        "char-code-at",
        "cleanup-release",
        "concat",
        "copy",
        "create-literal",
        "from-int",
        "from-scalar",
        "get-checked",
        "index-of",
        "last-index-of",
        "length",
        "managed-type-representation",
        "push",
        "retain",
        "split",
        "static-value",
        "substring",
        "sys-println-literal",
        "type-carrier",
    ]
    if stdlib.get("modules") != expected_modules or stdlib.get(
        "capabilities"
    ) != expected_capabilities:
        raise StringRuntimeFailure(
            "managed String standard-library report drifted: "
            f"modules={stdlib.get('modules')!r} "
            f"capabilities={stdlib.get('capabilities')!r}"
        )

    source_text = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "src").rglob("*.c"))
    )
    for required in (
        "hxc_string_from_scalar(",
        "hxc_string_from_int32(",
        "hxc_string_concat_ref(",
        "hxc_string_retain(",
        "hxc_string_release(",
        "hxc_string_index_of(",
        "hxc_string_last_index_of(",
        "hxc_string_split(",
        "hxc_string_substring(",
    ):
        if required not in source_text:
            raise StringRuntimeFailure(
                f"generated C omitted managed String call {required}"
            )
    for forbidden in ("hxc_object", "hxc_gc", "hxc_dynamic", "__c__("):
        if forbidden in source_text:
            raise StringRuntimeFailure(
                f"managed String fixture acquired unrelated/generated escape {forbidden!r}"
            )


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10.0
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise StringRuntimeFailure(
                f"Haxe server exited before managed String requests: "
                f"{stdout!r} {stderr!r}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise StringRuntimeFailure(
        "Haxe server did not accept managed String determinism requests"
    )


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
                raise StringRuntimeFailure(
                    f"{label} warm-server managed String compile failed: "
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
        first = compile_haxe(
            GENERATED, normal, layout=layout, report=layout == "split"
        )
        second = compile_haxe(GENERATED, reverse, layout=layout, reverse=True)
        for label, result in (
            (f"{layout}-normal", first),
            (f"{layout}-reverse", second),
        ):
            if result.returncode != 0 or result.stderr:
                raise StringRuntimeFailure(
                    f"{label} managed String compile failed\n"
                    f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
                )
        if generated_tree(normal) != generated_tree(reverse):
            raise StringRuntimeFailure(
                f"{layout} managed String output changed under reversed discovery"
            )
        projects[layout] = normal
        if layout == "split":
            validate_generated_project(normal, extract_hxcir(first))

    server_first, server_second = render_server_pair(root)
    split_tree = generated_tree(projects["split"])
    if (
        generated_tree(server_first) != split_tree
        or generated_tree(server_second) != split_tree
    ):
        raise StringRuntimeFailure(
            "managed String output changed under warm compiler-server reuse"
        )
    return projects


def render_native_text_projects(root: Path) -> tuple[Path, Path, Path, Path, Path]:
    """Compile public/private successes and two fail-before-C text borrows."""
    success = root / "native-text"
    private_split = root / "native-text-private-split"
    private_unity = root / "native-text-private-unity"
    nul = root / "native-text-nul"
    unterminated = root / "native-text-unterminated"
    rendered = compile_haxe(NATIVE_TEXT, success, report=True)
    private_rendered = compile_haxe(
        NATIVE_TEXT_PRIVATE_INLINE, private_split, report=True
    )
    private_unity_rendered = compile_haxe(
        NATIVE_TEXT_PRIVATE_INLINE, private_unity, layout="unity"
    )
    rejected = compile_haxe(NATIVE_TEXT_NUL, nul)
    unavailable = compile_haxe(NATIVE_TEXT_UNTERMINATED, unterminated)
    for label, result in (
        ("native text", rendered),
        ("private inline native text", private_rendered),
        ("private inline unity native text", private_unity_rendered),
        ("embedded-NUL text", rejected),
        ("unterminated text view", unavailable),
    ):
        if result.returncode != 0 or result.stderr:
            raise StringRuntimeFailure(
                f"{label} compile failed\nstdout={result.stdout!r}\nstderr={result.stderr!r}"
            )
    hxcir = extract_hxcir(rendered)
    if 'runtime(feature="string",operation="borrow-cstring")' not in hxcir:
        raise StringRuntimeFailure("native text HxcIR omitted its checked String borrow")
    if ":cstring-call-borrow" not in hxcir or 'native("native.function.TextObserver.matches")' not in hxcir:
        raise StringRuntimeFailure("native text HxcIR lost its distinct carrier or consumer")
    sources = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((success / "src").rglob("*.c"))
    )
    if "hxc_string_borrow_cstring(" not in sources or ".data" not in sources:
        raise StringRuntimeFailure("native text generated C bypassed the checked hxrt borrow")
    private_hxcir = extract_hxcir(private_rendered)
    if (
        'runtime(feature="string",operation="borrow-cstring")'
        not in private_hxcir
        or 'native("native.function.RuntimeTextFacade.PrivateTextObserver.matches")'
        not in private_hxcir
    ):
        raise StringRuntimeFailure(
            "private inline native text lost its declaration or checked borrow"
        )
    private_sources = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((private_split / "src").rglob("*.c"))
    )
    if private_sources.count("fixture_text_matches(") != 2:
        raise StringRuntimeFailure(
            "compatible literal/runtime imports did not share one native function"
        )
    private_headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((private_split / "include").rglob("*.h"))
    )
    if "PrivateTextObserver" in private_headers or "StaticTextObserver" in private_headers:
        raise StringRuntimeFailure(
            "private inline fixture leaked a Haxe helper into generated C headers"
        )
    symbol_table = json.loads(
        (private_split / "hxc.symbols.json").read_text(encoding="utf-8")
    )
    shared = [
        symbol
        for symbol in symbol_table.get("symbols", [])
        if symbol.get("cName") == "fixture_text_matches"
    ]
    if len(shared) != 1:
        raise StringRuntimeFailure(
            "compatible aliases did not coalesce to one external symbol request"
        )
    return success, private_split, private_unity, nul, unterminated


def plausible_output_exists(output: Path) -> bool:
    return output.exists() and any(output.rglob("*"))


def validate_generated_failures(root: Path) -> None:
    unsupported_output = root / "unsupported-std-string-source"
    unsupported = compile_haxe(NEGATIVE, unsupported_output)
    expected = "TCall(Std.string:source-not-yet-admitted:double)"
    if (
        unsupported.returncode == 0
        or "HXC1001:" not in unsupported.stderr
        or expected not in unsupported.stderr
    ):
        raise StringRuntimeFailure(
            "unsupported Std.string source did not fail at its intrinsic owner: "
            f"{unsupported.stderr!r}"
        )
    if plausible_output_exists(unsupported_output):
        raise StringRuntimeFailure(
            "unsupported Std.string source left plausible generated output"
        )

    expected_cstring_failures = {
        "cstring_ref_escape": ("HXC1001", "TCall(c.CStringRef.to:requires-direct-import-argument)"),
        "cstring_ref_wrong_owner": ("Int should be String", "For function argument 'text'"),
        "c_import_alias_mismatch": ("HXC3000", "incompatible C ABI signature"),
    }
    for name, (diagnostic, marker) in expected_cstring_failures.items():
        output = root / f"negative-{name}"
        result = compile_haxe(NEGATIVE / name, output)
        if result.returncode == 0 or diagnostic not in result.stderr or marker not in result.stderr:
            raise StringRuntimeFailure(f"negative CStringRef case {name} drifted: {result.stderr!r}")
        if plausible_output_exists(output):
            raise StringRuntimeFailure(f"negative CStringRef case {name} left plausible output")

    none_output = root / "runtime-none"
    none = compile_haxe(
        GENERATED, none_output, defines=("hxc_runtime=none",)
    )
    if (
        none.returncode == 0
        or "HXC2000:" not in none.stderr
        or "runtime.string.from-scalar." not in none.stderr
        or "string -> alloc -> status -> runtime-base" not in none.stderr
        or "string -> string-scalar -> string-literal -> runtime-base"
        not in none.stderr
    ):
        raise StringRuntimeFailure(
            f"runtime=none did not explain managed String closure: {none.stderr!r}"
        )
    if plausible_output_exists(none_output):
        raise StringRuntimeFailure(
            "runtime=none managed String rejection left plausible output"
        )


def project_build_inputs(project: Path) -> tuple[list[Path], list[Path]]:
    manifest = json.loads(
        (project / "hxc.manifest.json").read_text(encoding="utf-8")
    )
    build = manifest.get("build")
    if not isinstance(build, dict):
        raise StringRuntimeFailure(
            "generated managed String manifest omitted its build plan"
        )
    sources = [
        (project / value).resolve()
        for value in string_list(build.get("sources"), "generated sources")
    ]
    includes = [
        (project / value).resolve()
        for value in string_list(
            build.get("includeDirectories"), "generated include roots"
        )
    ]
    for path in (*sources, *includes):
        path.relative_to(project.resolve())
        if not path.exists():
            raise StringRuntimeFailure(f"generated build input is missing: {path}")
    return sources, includes


def compile_generated_and_run(
    toolchain: Toolchain,
    project: Path,
    executable: Path,
    flags: tuple[str, ...],
    extra_sources: tuple[Path, ...] = (),
    extra_include_roots: tuple[Path, ...] = (),
    expect_runtime_rejection: bool = False,
    expected_stdout: str = EXPECTED_GENERATED_STDOUT,
) -> None:
    sources, include_roots = project_build_inputs(project)
    command = [
        toolchain.compiler,
        *(flag for flag in STRICT_FLAGS if flag != "-DHXC_FREESTANDING=1"),
        *flags,
        *(f"-I{root}" for root in include_roots),
        *(str(source) for source in sources),
        *(f"-I{root}" for root in extra_include_roots),
        *(str(source) for source in extra_sources),
        "-o",
        str(executable),
    ]
    compiled = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=90,
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise StringRuntimeFailure(
            f"{toolchain.family} generated managed String compile failed\n"
            f"command={command!r}\nstdout={compiled.stdout!r}\n"
            f"stderr={compiled.stderr!r}"
        )
    environment = os.environ.copy()
    if toolchain.family == "clang" and "-fsanitize=address,undefined" in flags:
        environment["ASAN_OPTIONS"] = "detect_leaks=0"
        environment["UBSAN_OPTIONS"] = "halt_on_error=1"
    executed = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if expect_runtime_rejection:
        if executed.returncode == 0:
            raise StringRuntimeFailure("invalid native text reached its C consumer")
        return
    if executed.returncode != 0 or executed.stdout != expected_stdout or executed.stderr:
        raise StringRuntimeFailure(
            f"{toolchain.family} generated managed String execution drifted: "
            f"exit={executed.returncode} stdout={executed.stdout!r} "
            f"stderr={executed.stderr!r}"
        )


def inspect_generated_symbols(executable: Path, family: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise StringRuntimeFailure(f"{family} managed String evidence requires nm")
    result = subprocess.run(
        [nm, str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if result.returncode != 0:
        raise StringRuntimeFailure(
            f"{family} could not inspect generated managed String symbols"
        )
    for required in (
        "hxc_string_from_scalar",
        "hxc_string_from_int32",
        "hxc_string_concat_ref",
        "hxc_string_retain",
        "hxc_string_release",
        "hxc_string_index_of",
        "hxc_string_last_index_of",
        "hxc_string_split",
        "hxc_string_substring",
    ):
        if required not in result.stdout:
            raise StringRuntimeFailure(
                f"{family} executable omitted selected symbol {required}"
            )
    for forbidden in ("hxc_gc", "hxc_object", "hxc_dynamic", "hxc_reflection"):
        if forbidden in result.stdout:
            raise StringRuntimeFailure(
                f"{family} executable retained unrelated symbol {forbidden}"
            )


def compile_cpp_headers(toolchain: Toolchain, project: Path, build: Path) -> None:
    cxx_name = "clang++" if toolchain.family == "clang" else "g++"
    compiler = shutil.which(cxx_name)
    if compiler is None:
        print(
            f"string-runtime: SKIP optional {toolchain.family} C++17 headers: "
            f"missing {cxx_name}"
        )
        return
    _, include_roots = project_build_inputs(project)
    source = build / f"{toolchain.family}-headers.cpp"
    source.write_text(
        '#include "hxc/program.h"\n'
        '#include "hxrt/string.h"\n'
        "int main() {\n"
        "  hxc_string value = HXC_STRING_EMPTY_INITIALIZER;\n"
        "  return value.owner == nullptr ? 0 : 1;\n"
        "}\n",
        encoding="utf-8",
    )
    executable = build / f"{toolchain.family}-headers"
    command = [
        compiler,
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-pedantic-errors",
        *(f"-I{root}" for root in include_roots),
        str(source),
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
        raise StringRuntimeFailure(
            f"{toolchain.family} C++17 header consumer failed\n"
            f"command={command!r}\nstdout={compiled.stdout!r}\n"
            f"stderr={compiled.stderr!r}"
        )
    executed = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if executed.returncode != 0 or executed.stdout or executed.stderr:
        raise StringRuntimeFailure(
            f"{toolchain.family} C++17 header consumer execution drifted"
        )


def run_generated_native(
    toolchains: list[Toolchain],
    projects: dict[str, Path],
    native_text: tuple[Path, Path, Path, Path, Path],
    root: Path,
) -> None:
    for toolchain in toolchains:
        build = root / f"generated-{toolchain.family}"
        build.mkdir()
        for layout, project in projects.items():
            for optimization in ("-O0", "-O2"):
                executable = build / f"{layout}-{optimization[1:].lower()}"
                compile_generated_and_run(
                    toolchain, project, executable, (optimization,)
                )
                inspect_generated_symbols(executable, toolchain.family)
        compile_cpp_headers(toolchain, projects["split"], build)
        (
            native_text_success,
            native_text_private_split,
            native_text_private_unity,
            native_text_nul,
            native_text_unterminated,
        ) = native_text
        for project, label, expect_runtime_rejection in (
            (native_text_success, "native-text", False),
            (native_text_private_split, "native-text-private-split", False),
            (native_text_private_unity, "native-text-private-unity", False),
            (native_text_nul, "native-text-nul", True),
            (native_text_unterminated, "native-text-unterminated", True),
        ):
            compile_generated_and_run(
                toolchain,
                project,
                build / label,
                ("-O0",),
                extra_sources=(TEXT_OBSERVER_SOURCE,),
                extra_include_roots=(TEXT_OBSERVER_INCLUDE,),
                expect_runtime_rejection=expect_runtime_rejection,
                expected_stdout="",
            )
        if toolchain.family == "clang":
            executable = build / "split-sanitized"
            compile_generated_and_run(
                toolchain, projects["split"], executable, SANITIZER_FLAGS
            )
            inspect_generated_symbols(executable, toolchain.family)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", *TOOLCHAINS), default="auto")
    parser.add_argument(
        "--native-only",
        action="store_true",
        help="use the checked semantic trace without requiring Haxe",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        expected_trace = EXPECTED_TRACE if args.native_only else run_oracle()
        toolchains = selected_toolchains(args.toolchain)
        run_native(toolchains, expected_trace)
        if not args.native_only:
            run_generated_eval()
            with tempfile.TemporaryDirectory(
                prefix="reflaxe-c-generated-string-runtime-"
            ) as temporary:
                root = Path(temporary)
                projects = render_projects(root)
                native_text = render_native_text_projects(root)
                validate_generated_failures(root)
                run_generated_native(toolchains, projects, native_text, root)
    except (
        OSError,
        UnicodeError,
        StringRuntimeFailure,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"string-runtime: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    oracle = (
        "checked native trace"
        if args.native_only
        else "native/Eval parity plus ordinary-Haxe generated projects"
    )
    print(
        "string-runtime: OK: "
        f"{families}; {oracle}; checked/lossy UTF-8, scalar indexing/search, "
        "owned aliases/fields/containers/returns, call-scoped immutable C text, "
        "split/package/unity "
        "determinism, C11/C++17, sanitizers, and selective symbols passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

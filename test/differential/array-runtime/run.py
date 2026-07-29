#!/usr/bin/env python3
"""Compare the internal typed-array slice with a pinned Haxe Array trace."""

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
FIXTURE = CASE / "array_runtime.c"
JOIN_FIXTURE = CASE / "array_join_runtime.c"
INCLUDE = ROOT / "runtime/hxrt/include"
SOURCES = (
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/array.c",
)
JOIN_SOURCES = (
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/array.c",
    ROOT / "runtime/hxrt/src/array_join.c",
    ROOT / "runtime/hxrt/src/string.c",
    ROOT / "runtime/hxrt/src/string_scalar.c",
)
EXPECTED_TRACE = "5:1,3,1,1,0\n"
GENERATED = CASE / "generated"
GENERATED_CLASS = CASE / "generated-class"
GENERATED_CLASS_GC_DRIVER = ROOT / "test/native/array_class_gc_driver.c"
NEGATIVE = CASE / "negative"
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
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
GENERATED_STRICT_FLAGS = tuple(
    flag for flag in STRICT_FLAGS if flag != "-DHXC_FREESTANDING=1"
)
JOIN_STRICT_FLAGS = GENERATED_STRICT_FLAGS


class ArrayRuntimeFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Toolchain:
    family: str
    compiler: str
    version: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


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
        raise ArrayRuntimeFailure(f"cannot identify compiler {executable}")
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
            raise ArrayRuntimeFailure(
                f"required array-runtime compiler is missing: {family}"
            )
        print(f"array-runtime: SKIP optional {family}: missing command")
        return None
    actual, version = compiler_identity(compiler)
    if actual != family:
        if required:
            raise ArrayRuntimeFailure(
                f"required {family} command identifies as {actual}: {version}"
            )
        print(f"array-runtime: SKIP optional {family}: command identifies as {actual}")
        return None
    return Toolchain(family, compiler, version)


def selected_toolchains(selected: str) -> list[Toolchain]:
    families = TOOLCHAINS if selected == "auto" else (selected,)
    toolchains = [
        toolchain
        for family in families
        if (toolchain := resolve_toolchain(family, required=selected != "auto"))
        is not None
    ]
    if not toolchains:
        raise ArrayRuntimeFailure(
            "no strict C11 compiler is available for the array runtime contract"
        )
    return toolchains


def run_oracle() -> str:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    outputs: list[str] = []
    for label in ("first", "second"):
        result = subprocess.run(
            [development_tool("haxe"), str(ORACLE_HXML)],
            cwd=ROOT,
            env=environment,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if result.returncode != 0 or result.stderr:
            raise ArrayRuntimeFailure(
                f"{label} Haxe Array oracle failed\n"
                f"exit={result.returncode} stdout={result.stdout!r} "
                f"stderr={result.stderr!r}"
            )
        outputs.append(result.stdout)
    if outputs[0] != outputs[1] or outputs[0] != EXPECTED_TRACE:
        raise ArrayRuntimeFailure(
            "Haxe Array oracle drifted\n"
            f"first={outputs[0]!r} second={outputs[1]!r}"
        )
    return outputs[0]


def run_generated_eval_oracle() -> None:
    """Run the ordinary-Haxe ownership fixture before compiling it to C."""

    result = subprocess.run(
        [development_tool("haxe"), "oracle.hxml"],
        cwd=GENERATED,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ArrayRuntimeFailure(
            "ordinary-Haxe Array Eval oracle drifted\n"
            f"exit={result.returncode} stdout={result.stdout!r} "
            f"stderr={result.stderr!r}"
        )


def haxe_environment(*, server: bool = False) -> dict[str, str]:
    environment = os.environ.copy()
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


def compile_generated_haxe(
    fixture: Path,
    output: Path,
    *,
    reverse: bool = False,
    layout: str = "split",
    defines: tuple[str, ...] = (),
    connect: str | None = None,
    report: bool = False,
) -> subprocess.CompletedProcess[str]:
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend([
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        "Main",
        "-D",
        f"hxc_project_layout={layout}",
    ])
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


def extract_hxcir(result: subprocess.CompletedProcess[str]) -> str:
    lines = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(lines) != 1:
        raise ArrayRuntimeFailure("generated Array compile omitted its one HxcIR report")
    try:
        report = json.loads(lines[0])
    except json.JSONDecodeError as error:
        raise ArrayRuntimeFailure("generated Array HxcIR report is invalid JSON") from error
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise ArrayRuntimeFailure("generated Array HxcIR report omitted the semantic tree")
    return hxcir


def hxcir_function(hxcir: str, function_id: str) -> str:
    """Return one complete function section from the deterministic text dump."""
    start_marker = f'  function "{function_id}"'
    end_marker = f'  end function "{function_id}"'
    start = hxcir.find(start_marker)
    if start < 0:
        raise ArrayRuntimeFailure(f"generated Array HxcIR omitted {function_id}")
    end = hxcir.find(end_marker, start)
    if end < 0:
        raise ArrayRuntimeFailure(
            f"generated Array HxcIR did not close {function_id}"
        )
    return hxcir[start : end + len(end_marker)]


def validate_generated_hxcir(hxcir: str) -> None:
    """Prove Array ownership before C syntax is selected."""
    for marker in (
        'representation=managed("array")',
        'runtime(feature="array",operation="create-literal")',
        'runtime(feature="array",operation="copy")',
        'runtime(feature="array",operation="length")',
        'runtime(feature="array",operation="get-checked")',
        'runtime(feature="array",operation="push")',
        'runtime(feature="array",operation="pop")',
        'runtime(feature="array",operation="shift")',
        'runtime(feature="array",operation="splice-one-discard")',
        'runtime(feature="array",operation="resize-zero")',
        'runtime(feature="array",operation="sort")',
        'function-reference target="function.lambda.function.Main.main.',
        'implementation=program-local("array-element-lifecycle:instance.closed-record.',
        'array-element-owner-initialize',
        'array-element-borrow',
        'haxe.array-reference.equal',
        'haxe.array-reference.not-equal',
        'retain place=local(',
        'release place=local(',
        'release place=field(',
        'implementation=runtime("array")',
    ):
        if marker not in hxcir:
            raise ArrayRuntimeFailure(f"generated Array HxcIR omitted {marker}")
    if " raw" in hxcir or str(ROOT) in hxcir:
        raise ArrayRuntimeFailure("generated Array HxcIR used raw syntax or leaked a local path")
    # The entry function now owns additional managed-enum locals after the
    # original Array/Bytes setup. Require the original actions as an ordered
    # subsequence of one return edge: newer owners may appear before them, but
    # the long-standing reverse-registration contract must remain unchanged.
    cleanup_lines = [
        line
        for line in hxcir.splitlines()
        if "terminator return" in line
        and ".array-field.entries.release" in line
    ]
    cleanup_line = "" if not cleanup_lines else cleanup_lines[0]
    history_owner = re.search(
        r'"cleanup\.construction"\."(construction\.\d+)\.array-field\.entries\.release"',
        cleanup_line,
    )
    history_cleanup = (
        ()
        if history_owner is None
        else (
            f'"cleanup.construction"."{history_owner.group(1)}.array-field.entries.release"',
            f'"cleanup.construction"."{history_owner.group(1)}.initialized"',
        )
    )
    byte_ids = [
        int(value)
        for value in re.findall(r'"cleanup\.construction"\."bytes-local\.(\d+)\.release"', cleanup_line)
    ]
    array_ids = [
        int(value)
        for value in re.findall(r'"cleanup\.construction"\."array-local\.(\d+)\.release"', cleanup_line)
    ]
    if (
        not cleanup_line
        or len(history_cleanup) != 2
        or any(marker not in cleanup_line for marker in history_cleanup)
        or len(byte_ids) < 2
        or byte_ids != sorted(byte_ids, reverse=True)
        or len(array_ids) < 3
        or array_ids != sorted(array_ids, reverse=True)
        or cleanup_line.index(f'"bytes-local.{byte_ids[-1]}.release"') >= cleanup_line.index(history_cleanup[0])
        or cleanup_line.index(history_cleanup[0]) >= cleanup_line.index(history_cleanup[1])
        or cleanup_line.index(history_cleanup[1]) >= cleanup_line.index(f'"array-local.{array_ids[-1]}.release"')
    ):
        raise ArrayRuntimeFailure(
            "generated Array HxcIR lost reverse ownership cleanup: "
            f"bytes={byte_ids!r} arrays={array_ids!r} line={cleanup_line!r}"
        )
    for marker in (
        'implementation=program-local("enum-lifecycle:',
        'enum-local.',
        'array-element-lifecycle:instance.enum.',
    ):
        if marker not in hxcir:
            raise ArrayRuntimeFailure(f"generated managed-enum HxcIR omitted {marker}")
    for managed_cleanup in (
        'cleanup=["cleanup.construction"."array-element.local.1.release"]',
        'cleanup=["cleanup.construction"."array-element.local.2.release",'
        '"cleanup.construction"."array-local.1.release"]',
    ):
        if managed_cleanup not in hxcir:
            raise ArrayRuntimeFailure(
                "generated Array HxcIR lost managed element-copy cleanup"
            )

    record_loop = hxcir_function(
        hxcir, "function.Main.countFirstEnabledRecord"
    )
    enum_loop = hxcir_function(
        hxcir, "function.Main.countFirstScheduledCommands"
    )
    entry = hxcir_function(hxcir, "function.Main.main")
    history_pop = hxcir_function(hxcir, "method.History.takeNewest")
    choose_array = hxcir_function(hxcir, "function.Main.chooseArray")
    selected_pair_sum = hxcir_function(hxcir, "function.Main.selectedPairSum")
    delayed_plan = hxcir_function(hxcir, "function.Main.delayedPlanLength")
    if (
        "declare-managed-carrier" not in choose_array
        or "ownership=move-fresh" not in choose_array
        or 'ownership=retain-borrowed(runtime("array"))' not in choose_array
        or "move-managed-carrier" not in choose_array
    ):
        raise ArrayRuntimeFailure(
            "Array conditional lost its exact fresh-move/borrowed-retain join"
        )
    if (
        "declare-managed-carrier" not in selected_pair_sum
        or 'dispatch=direct("function.Main.borrowedLength")' not in selected_pair_sum
        or '"array-local.' not in selected_pair_sum
        or "array-get-checked" not in selected_pair_sum
        or 'cleanup=["cleanup.construction"."array-local.' not in selected_pair_sum
    ):
        raise ArrayRuntimeFailure(
            "joined Array lost its cleanup-owned argument/index consumption"
        )
    if (
        delayed_plan.count("declare-managed-carrier") != 2
        or delayed_plan.count('ownership=retain-borrowed(runtime("array"))') != 2
        or delayed_plan.count("move-managed-carrier") != 2
        or delayed_plan.count("managed-flow-owner") < 2
        or 'implementation=runtime("array")' not in delayed_plan
    ):
        raise ArrayRuntimeFailure(
            "sequential Array switch carriers lost their cleanup-owned local transfer"
        )
    for role, target in (
        ("static-call-argument-0", "function.Main.borrowedLength"),
        ("instance-call-argument-0", "method.FreshArrayReader.length"),
    ):
        owner = re.search(
            rf'{role}-owner-initialize" result=- initialize '
            r'place=local\("([^"]+)"\)',
            entry,
        )
        if owner is None:
            raise ArrayRuntimeFailure(
                f"fresh direct Array call omitted its {role} owner"
            )
        owner_local = owner.group(1)
        borrow = re.search(
            rf'{role}-borrow" result="([^"]+)":[^\n]+'
            rf'load place=local\("{re.escape(owner_local)}"\)',
            entry,
        )
        if borrow is None:
            raise ArrayRuntimeFailure(
                f"fresh direct Array call omitted its {role} borrow"
            )
        borrowed_value = borrow.group(1)
        call_lines = [
            line
            for line in entry.splitlines()
            if f'dispatch=direct("{target}")' in line
        ]
        if (
            len(call_lines) != 1
            or f'"{borrowed_value}"' not in call_lines[0]
        ):
            raise ArrayRuntimeFailure(
                f"fresh direct Array call did not pass its {role} borrow"
            )
        action = (
            f'action "array-temporary.{owner_local}.release" '
            f'idempotence=exactly-once release place=local("{owner_local}") '
            'implementation=runtime("array")'
        )
        cleanup_step = (
            f'"cleanup.construction"."array-temporary.{owner_local}.release"'
        )
        if entry.count(action) != 1 or cleanup_line.count(cleanup_step) != 1:
            raise ArrayRuntimeFailure(
                f"fresh direct Array call lost exactly-once {role} cleanup"
            )
    if (
        history_pop.count('runtime(feature="array",operation="pop")') != 1
        or 'returns=nullable(tagged,instance("instance.closed-record.' not in history_pop
        or 'terminator return value="value.' not in history_pop
        or "cleanup=[]" not in history_pop
        or " retain " in history_pop
        or " release " in history_pop
    ):
        raise ArrayRuntimeFailure(
            "managed Array.pop did not transfer one fresh optional owner directly "
            "to the caller"
        )
    if entry.count('runtime(feature="array",operation="pop")') != 3:
        raise ArrayRuntimeFailure(
            "primitive Array.pop coverage no longer contains present, repeated, "
            "and empty mutations"
        )
    if entry.count('runtime(feature="array",operation="shift")') != 7:
        raise ArrayRuntimeFailure(
            "Array.shift coverage no longer contains primitive and managed "
            "present, repeated, and empty ownership transfers"
        )
    if entry.count('runtime(feature="array",operation="splice-one-discard")') != 6:
        raise ArrayRuntimeFailure(
            "discarded Array.splice coverage no longer contains primitive "
            "middle/negative/out-of-range/clamped/empty cases plus one managed "
            "String removal"
        )
    if (
        entry.count('runtime(feature="array",operation="resize-zero")') != 2
        or entry.count("array-resize-zero-receiver-null-check") != 2
    ):
        raise ArrayRuntimeFailure(
            "Array.resize(0) lost its two typed clear operations or their "
            "dominating receiver checks"
        )
    if (
        'action "optional-local.' not in entry
        or "optional-lifecycle:optional." not in entry
    ):
        raise ArrayRuntimeFailure(
            "managed Array edge-removal result lost independently owned optional cleanup"
        )
    if (
        'string-temporary.' not in entry
        or 'array-push-element-owner-initialize' not in entry
        or not re.search(
            r'array-push-element-borrow" result="[^"]+":managed-string-utf8',
            entry,
        )
    ):
        raise ArrayRuntimeFailure(
            "generated Array<String> comprehension lost its fresh element owner"
        )
    for label, section, element_owner, local_owner in (
        (
            "managed-record loop",
            record_loop,
            'array-element-lifecycle:instance.closed-record.',
            "record-local.",
        ),
        (
            "managed-enum loop",
            enum_loop,
            'array-element-lifecycle:instance.enum.',
            "enum-local.",
        ),
    ):
        for marker in (
            element_owner,
            local_owner,
            "release-branch-local-owner",
            "terminator return",
            "terminator jump",
        ):
            if marker not in section:
                raise ArrayRuntimeFailure(
                    f"generated {label} HxcIR omitted {marker}"
                )

    # A synthesized loop-edge release must keep the span of the expression that
    # created its owner. The enclosing loop decides when it runs, but replacing
    # the action span with the loop span breaks exact runtime provenance.
    cleanup_action_sources = {
        line.rsplit(" @", 1)[1]
        for line in hxcir.splitlines()
        if " action " in line and " release place=" in line and " @" in line
    }
    loop_release_sources = [
        line.rsplit(" @", 1)[1]
        for section in (record_loop, enum_loop)
        for line in section.splitlines()
        if "release-branch-local-owner" in line and " @" in line
    ]
    if not loop_release_sources or any(
        source not in cleanup_action_sources for source in loop_release_sources
    ):
        raise ArrayRuntimeFailure(
            "generated loop cleanup lost its original owner source span"
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
            raise ArrayRuntimeFailure(
                "Haxe server exited before the Array determinism requests\n"
                f"stdout={stdout!r} stderr={stderr!r}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise ArrayRuntimeFailure("Haxe server did not accept Array determinism requests")


def render_server_pair(
    root: Path,
    *,
    fixture: Path = GENERATED,
    prefix: str = "generated",
    layout: str = "split",
) -> tuple[Path, Path]:
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
        first_output = root / f"{prefix}-{layout}-server-first"
        second_output = root / f"{prefix}-{layout}-server-second"
        first = compile_generated_haxe(
            fixture, first_output, connect=endpoint, layout=layout
        )
        second = compile_generated_haxe(
            fixture, second_output, connect=endpoint, layout=layout
        )
        for label, result in (("server-first", first), ("server-second", second)):
            if result.returncode != 0:
                raise ArrayRuntimeFailure(
                    f"{label} generated Array compile failed\n"
                    f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
                )
        return first_output, second_output
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def generated_tree(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def validate_generated_project(output: Path) -> None:
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if plan.get("features") != [
        "runtime-base",
        "status",
        "alloc",
        "array",
        "string-literal",
        "string-scalar",
        "string",
        "array-join",
        "bytes",
    ]:
        raise ArrayRuntimeFailure("generated Array program selected the wrong runtime closure")
    reasons = plan.get("rootReasons")
    if not isinstance(reasons, list) or not reasons:
        raise ArrayRuntimeFailure("generated Array program omitted typed runtime reasons")
    retain_surfaces_by_source: dict[str, set[str]] = {}
    for reason in reasons:
        if (
            isinstance(reason, dict)
            and reason.get("featureId") == "string"
            and reason.get("operationId") == "retain"
            and isinstance(reason.get("surface"), str)
            and isinstance(reason.get("source"), dict)
        ):
            source_key = json.dumps(
                reason["source"], ensure_ascii=False, sort_keys=True
            )
            retain_surfaces_by_source.setdefault(source_key, set()).add(
                str(reason["surface"])
            )
    expected_same_span_surfaces = {
        "ordinary Haxe String local alias",
        "managed String captured by a closed record",
    }
    if not any(
        expected_same_span_surfaces.issubset(surfaces)
        for surfaces in retain_surfaces_by_source.values()
    ):
        raise ArrayRuntimeFailure(
            "same-span catalog copy lost its distinct local-alias and "
            "record-field String ownership reasons"
        )
    operations = {
        reason.get("operationId")
        for reason in reasons
        if isinstance(reason, dict) and reason.get("featureId") == "array"
    }
    expected = {
        "cleanup-release",
        "copy",
        "create-literal",
        "get-checked",
        "length",
        "managed-type-representation",
        "pop",
        "push",
        "retain",
        "resize-zero",
        "set",
        "shift",
        "sort",
        "splice-one-discard",
    }
    if operations != expected:
        raise ArrayRuntimeFailure(
            f"generated Array runtime operations drifted: {sorted(operations)!r}"
        )
    join_operations = {
        reason.get("operationId")
        for reason in reasons
        if isinstance(reason, dict) and reason.get("featureId") == "array-join"
    }
    if join_operations != {"join"}:
        raise ArrayRuntimeFailure(
            f"generated Array join operations drifted: {sorted(join_operations)!r}"
        )
    sources = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    for marker in (
        "hxc_array_ref_create(",
        "hxc_array_ref_create_trivial",
        "hxc_array_ref_copy(",
        "hxc_array_ref_retain",
        "hxc_array_ref_release",
        "hxc_array_ref_push_copy",
        "hxc_array_ref_pop_move",
        "hxc_array_ref_shift_move",
        "hxc_array_ref_splice_one_discard",
        "hxc_array_ref_get_copy",
        "hxc_array_resize",
        "hxc_array_ref_sort",
        "hxc_array_string_join",
        "_element_copy(",
        "_element_assign(",
        "_element_destroy(",
        "hxc_bytes_ref_retain",
        "hxc_bytes_ref_release",
        "hxc_string",
        "memcmp",
    ):
        if marker not in sources:
            raise ArrayRuntimeFailure(f"generated C omitted {marker}")
    if "struct hxc_array_ref *hxc_Main_maybeValues(bool" not in headers:
        raise ArrayRuntimeFailure(
            "Null<Array<Int>> acquired storage beyond the existing Array pointer"
        )
    if "goto " in sources:
        raise ArrayRuntimeFailure("the structured Array fixture unexpectedly emitted goto control flow")


def render_generated_pair(root: Path) -> Path:
    normal = root / "generated-normal"
    reverse = root / "generated-reverse"
    first = compile_generated_haxe(GENERATED, normal, report=True)
    second = compile_generated_haxe(GENERATED, reverse, reverse=True)
    for label, result in (("normal", first), ("reverse", second)):
        if result.returncode != 0:
            raise ArrayRuntimeFailure(
                f"{label} generated Array compile failed\n"
                f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
            )
    if generated_tree(normal) != generated_tree(reverse):
        raise ArrayRuntimeFailure("generated Array project changed under reversed discovery")
    metal_normal = root / "generated-metal-normal"
    metal_reverse = root / "generated-metal-reverse"
    metal_defines = ("reflaxe_c_profile=metal",)
    first_metal = compile_generated_haxe(
        GENERATED, metal_normal, defines=metal_defines
    )
    second_metal = compile_generated_haxe(
        GENERATED,
        metal_reverse,
        reverse=True,
        defines=metal_defines,
    )
    for label, result in (
        ("metal-normal", first_metal),
        ("metal-reverse", second_metal),
    ):
        if result.returncode != 0:
            raise ArrayRuntimeFailure(
                f"{label} generated Array compile failed\n"
                f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
            )
    if generated_tree(metal_normal) != generated_tree(metal_reverse):
        raise ArrayRuntimeFailure(
            "generated metal Array project changed under reversed discovery"
        )
    validate_generated_hxcir(extract_hxcir(first))
    server_first, server_second = render_server_pair(root)
    canonical = generated_tree(normal)
    if generated_tree(server_first) != canonical or generated_tree(server_second) != canonical:
        raise ArrayRuntimeFailure("generated Array project changed under warm compiler-server reuse")
    validate_generated_project(normal)
    oracle = subprocess.run(
        [development_tool("haxe"), "oracle.hxml"],
        cwd=GENERATED,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if oracle.returncode != 0 or oracle.stdout or oracle.stderr:
        raise ArrayRuntimeFailure(
            "ordinary-Haxe generated fixture oracle failed: "
            f"exit={oracle.returncode} stdout={oracle.stdout!r} stderr={oracle.stderr!r}"
        )
    return normal


def render_managed_class_pair(root: Path) -> Path:
    """Prove the collector-backed graph is deterministic in every file layout."""
    canonical_by_layout: dict[str, dict[str, bytes]] = {}
    first_result: subprocess.CompletedProcess[str] | None = None
    normal_split: Path | None = None
    for layout in ("split", "package", "unity"):
        normal = root / f"generated-class-{layout}-normal"
        reverse = root / f"generated-class-{layout}-reverse"
        first = compile_generated_haxe(
            GENERATED_CLASS, normal, report=layout == "split", layout=layout
        )
        second = compile_generated_haxe(
            GENERATED_CLASS, reverse, reverse=True, layout=layout
        )
        for label, result in ((f"{layout}-normal", first), (f"{layout}-reverse", second)):
            if result.returncode != 0:
                raise ArrayRuntimeFailure(
                    f"{label} generated Array<Class> compile failed\n"
                    f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
                )
        canonical = generated_tree(normal)
        if canonical != generated_tree(reverse):
            raise ArrayRuntimeFailure(
                f"generated Array<Class> {layout} project changed under reversed discovery"
            )
        canonical_by_layout[layout] = canonical
        if layout == "split":
            normal_split = normal
            first_result = first

    if normal_split is None or first_result is None:
        raise ArrayRuntimeFailure("generated Array<Class> lost its split reference build")
    server_first, server_second = render_server_pair(
        root, fixture=GENERATED_CLASS, prefix="generated-class", layout="split"
    )
    if (
        generated_tree(server_first) != canonical_by_layout["split"]
        or generated_tree(server_second) != canonical_by_layout["split"]
    ):
        raise ArrayRuntimeFailure(
            "generated Array<Class> project changed under warm compiler-server reuse"
        )

    oracle = subprocess.run(
        [development_tool("haxe"), "oracle.hxml"],
        cwd=GENERATED_CLASS,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if oracle.returncode != 0 or oracle.stdout or oracle.stderr:
        raise ArrayRuntimeFailure(
            "managed Array<Class> Haxe Eval oracle failed: "
            f"exit={oracle.returncode} stdout={oracle.stdout!r} stderr={oracle.stderr!r}"
        )
    hxcir = extract_hxcir(first_result)
    for marker in (
        'representation=managed("gc")',
        'allocate type=instance("instance.class.',
        'implementation=runtime("gc")',
        'runtime(feature="array",operation="copy")',
        'runtime(feature="array",operation="set")',
        'haxe.array-reference.equal',
        'haxe.array-reference.not-equal',
        'managed-root "root.',
    ):
        if marker not in hxcir:
            raise ArrayRuntimeFailure(f"generated Array<Class> HxcIR omitted {marker}")

    plan = json.loads((normal_split / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if plan.get("features") != [
        "runtime-base",
        "status",
        "alloc",
        "array",
        "object",
        "gc",
    ]:
        raise ArrayRuntimeFailure("generated Array<Class> selected the wrong runtime closure")
    if "exact-traced-haxe-object-graph" not in plan.get("directDecisions", []):
        raise ArrayRuntimeFailure("generated Array<Class> omitted its traced-graph decision")

    application = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((normal_split / "src").rglob("*.c"))
    )
    for marker in (
        "hxc_gc_allocate",
        "hxc_array_ref_copy_in_place",
        "hxc_array_ref_init_in_place",
        "hxc_array_ref_set_copy",
        "HXC_TYPE_DESCRIPTOR_HAS_TRACE",
        "HXC_TYPE_DESCRIPTOR_HAS_FINALIZER",
        "hxc_array_ref_dispose_in_place",
    ):
        if marker not in application:
            raise ArrayRuntimeFailure(f"generated Array<Class> C omitted {marker}")
    if "goto " in application:
        raise ArrayRuntimeFailure("the structured Array<Class> fixture unexpectedly emitted goto")

    rejected = compile_generated_haxe(
        GENERATED_CLASS,
        root / "generated-class-runtime-none",
        defines=("hxc_runtime=none",),
    )
    if (
        rejected.returncode == 0
        or "runtime policy `none`" not in rejected.stderr
        or "generated-class/Main.hx:" not in rejected.stderr
        or "source=Main.hx:" not in rejected.stderr
    ):
        raise ArrayRuntimeFailure("runtime-none did not reject traced Array<Class>")
    return normal_split


def run_generated_negative_cases(root: Path) -> None:
    expected = {
        "indirect_fresh_argument": "TCall(indirect-managed-argument-needs-explicit-ownership:0)",
        "join_non_string": "TCall(Array.join:element-not-managed-String:",
        "reassignment": "TBinop(OpAssign:managed-Array-reassignment-not-admitted)",
        "resize_dynamic": "TCall(Array.resize:only-literal-zero-admitted)",
        "resize_nonzero": "TCall(Array.resize:only-literal-zero-admitted)",
        "sort_capturing_comparator": "TFunction(capturing-closure:outer-local:direction)",
        "splice_return": "TCall(Array.splice:returned-Array-not-yet-admitted)",
    }
    for name, marker in expected.items():
        output = root / f"negative-{name}"
        result = compile_generated_haxe(NEGATIVE / name, output)
        if result.returncode == 0 or marker not in result.stderr:
            raise ArrayRuntimeFailure(
                f"negative generated Array case {name} drifted\n"
                f"exit={result.returncode} stdout={result.stdout!r} stderr={result.stderr!r}"
            )
        if output.exists() and any(output.rglob("*")):
            raise ArrayRuntimeFailure(f"negative generated Array case {name} left output")
    for label, defines in (
        ("runtime-none", ("hxc_runtime=none",)),
        ("metal-runtime-none", ("reflaxe_c_profile=metal", "hxc_runtime=none")),
    ):
        output = root / label
        result = compile_generated_haxe(GENERATED, output, defines=defines)
        if result.returncode == 0 or "runtime policy `none`" not in result.stderr:
            raise ArrayRuntimeFailure(f"{label} did not fail closed on managed Array")
        if output.exists() and any(output.rglob("*")):
            raise ArrayRuntimeFailure(f"{label} left plausible generated output")


def compile_and_run(
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
        raise ArrayRuntimeFailure(
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
    if executed.returncode != 0 or executed.stdout != expected_trace or executed.stderr:
        raise ArrayRuntimeFailure(
            f"{toolchain.family} {label} execution drifted\n"
            f"exit={executed.returncode} stdout={executed.stdout!r} "
            f"stderr={executed.stderr!r}"
        )
    return executable


def compile_and_run_join_contract(
    toolchain: Toolchain, build: Path, flags: tuple[str, ...], label: str
) -> None:
    """Run the independent runtime failure/byte-preservation contract."""
    executable = build / label
    command = [
        toolchain.compiler,
        *JOIN_STRICT_FLAGS,
        *flags,
        f"-I{INCLUDE}",
        *(str(source) for source in JOIN_SOURCES),
        str(JOIN_FIXTURE),
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
        raise ArrayRuntimeFailure(
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
    if executed.returncode != 0 or executed.stdout or executed.stderr:
        raise ArrayRuntimeFailure(
            f"{toolchain.family} {label} execution drifted\n"
            f"exit={executed.returncode} stdout={executed.stdout!r} "
            f"stderr={executed.stderr!r}"
        )


def compile_and_run_generated(
    toolchain: Toolchain,
    build: Path,
    generated: Path,
    flags: tuple[str, ...],
    label: str,
) -> None:
    executable = build / label
    sources = sorted((generated / "runtime/src").glob("*.c")) + sorted(
        (generated / "src").rglob("*.c")
    )
    command = [
        toolchain.compiler,
        *GENERATED_STRICT_FLAGS,
        *flags,
        f"-I{generated / 'include'}",
        f"-I{generated / 'runtime/include'}",
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
        raise ArrayRuntimeFailure(
            f"{toolchain.family} {label} generated compile failed\n"
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
    if executed.returncode != 0 or executed.stdout or executed.stderr:
        raise ArrayRuntimeFailure(
            f"{toolchain.family} {label} generated execution drifted\n"
            f"exit={executed.returncode} stdout={executed.stdout!r} stderr={executed.stderr!r}"
        )


def compile_and_run_generated_gc_reclamation(
    toolchain: Toolchain,
    build: Path,
    generated: Path,
    flags: tuple[str, ...],
    label: str,
) -> None:
    """Use an independent driver to prove the generated cycle is collected."""
    executable = build / label
    sources = sorted((generated / "runtime/src").glob("*.c")) + [
        source
        for source in sorted((generated / "src").rglob("*.c"))
        if source.relative_to(generated).as_posix() != "src/hxc/main.c"
    ]
    command = [
        toolchain.compiler,
        *GENERATED_STRICT_FLAGS,
        *flags,
        f"-I{generated / 'include'}",
        f"-I{generated / 'runtime/include'}",
        *(str(source) for source in sources),
        str(GENERATED_CLASS_GC_DRIVER),
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
        raise ArrayRuntimeFailure(
            f"{toolchain.family} {label} reclamation driver compile failed\n"
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
    if executed.returncode != 0 or executed.stdout or executed.stderr:
        raise ArrayRuntimeFailure(
            f"{toolchain.family} {label} reclamation evidence drifted\n"
            f"exit={executed.returncode} stdout={executed.stdout!r} stderr={executed.stderr!r}"
        )


def inspect_symbols(executable: Path, family: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise ArrayRuntimeFailure(f"{family} array runtime requires nm evidence")
    result = subprocess.run(
        [nm, str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if result.returncode != 0:
        raise ArrayRuntimeFailure(f"{family} could not inspect array symbols")
    symbols = result.stdout
    for required in (
        "hxc_array_insert_copy",
        "hxc_array_ref_create",
        "hxc_array_ref_create_trivial",
        "hxc_array_ref_dispose_in_place",
        "hxc_array_ref_init_in_place",
        "hxc_array_ref_release",
        "hxc_array_ref_retain",
        "hxc_array_pop_move",
        "hxc_array_ref_pop_move",
        "hxc_array_shift_move",
        "hxc_array_ref_shift_move",
        "hxc_array_ref_splice_one_discard",
        "hxc_array_resize",
        "hxc_array_remove_at",
    ):
        if required not in symbols:
            raise ArrayRuntimeFailure(
                f"{family} array link omitted required symbol {required}"
            )
    for forbidden in (
        "hxc_string",
        "hxc_object",
        "hxc_gc",
        "hxc_reflection",
        "hxc_dynamic",
    ):
        if forbidden in symbols:
            raise ArrayRuntimeFailure(
                f"{family} array link retained unrelated symbol family {forbidden}"
            )


def run_native(
    toolchains: list[Toolchain], expected_trace: str, *, generated_haxe: bool
) -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-array-runtime-") as temporary:
        root = Path(temporary)
        generated = render_generated_pair(root) if generated_haxe else None
        generated_class = (
            render_managed_class_pair(root) if generated_haxe else None
        )
        if generated_haxe:
            run_generated_negative_cases(root)
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir(parents=True)
            debug = compile_and_run(
                toolchain, build, ("-O0",), "array-runtime-o0", expected_trace
            )
            compile_and_run(
                toolchain, build, ("-O2",), "array-runtime-o2", expected_trace
            )
            compile_and_run(
                toolchain,
                build,
                SANITIZER_FLAGS,
                "array-runtime-sanitized",
                expected_trace,
            )
            compile_and_run_join_contract(
                toolchain,
                build,
                SANITIZER_FLAGS,
                "array-join-runtime-sanitized",
            )
            if generated is not None:
                compile_and_run_generated(
                    toolchain,
                    build,
                    generated,
                    ("-O0",),
                    "generated-array-o0",
                )
                compile_and_run_generated(
                    toolchain,
                    build,
                    generated,
                    SANITIZER_FLAGS,
                    "generated-array-sanitized",
                )
            if generated_class is not None:
                compile_and_run_generated(
                    toolchain,
                    build,
                    generated_class,
                    ("-O0",),
                    "generated-array-class-o0",
                )
                compile_and_run_generated(
                    toolchain,
                    build,
                    generated_class,
                    SANITIZER_FLAGS,
                    "generated-array-class-sanitized",
                )
                compile_and_run_generated_gc_reclamation(
                    toolchain,
                    build,
                    generated_class,
                    ("-O0",),
                    "generated-array-class-reclamation-o0",
                )
                compile_and_run_generated_gc_reclamation(
                    toolchain,
                    build,
                    generated_class,
                    SANITIZER_FLAGS,
                    "generated-array-class-reclamation-sanitized",
                )
            inspect_symbols(debug, toolchain.family)


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
        if not args.native_only:
            run_generated_eval_oracle()
        toolchains = selected_toolchains(args.toolchain)
        run_native(toolchains, expected_trace, generated_haxe=not args.native_only)
    except (
        OSError,
        UnicodeError,
        ArrayRuntimeFailure,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"array-runtime: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    oracle = "checked Array trace" if args.native_only else "pinned Haxe Eval oracle"
    generated = "" if args.native_only else "generated ordinary-Haxe Array ownership plus "
    print(
        "array-runtime: OK: "
        f"{families}; {oracle}; {generated}primitive/reference growth, traced class identity, live pressure tracing, cycle reclamation, aliasing, "
        "overflow, allocation failure, lifecycle rollback, sanitizers, and selective symbols passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

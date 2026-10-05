#!/usr/bin/env python3
"""Prove exact scalar, nominal-String, enum, and managed-record StringMaps."""

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
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402

CASE = Path(__file__).resolve().parent
GENERATED = CASE / "generated"
DIRECT_DECISION = CASE / "direct_decision"
DISPATCH_OWNED = CASE / "dispatch_owned"
COLLECTOR_RECORD = CASE / "collector_record"
NEGATIVE = CASE / "negative"
FIXTURE = CASE / "string_map_runtime.c"
INCLUDE = ROOT / "runtime/hxrt/include"
RUNTIME_SOURCES = (
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/array.c",
    ROOT / "runtime/hxrt/src/iterator.c",
    ROOT / "runtime/hxrt/src/string.c",
    ROOT / "runtime/hxrt/src/string_map.c",
    ROOT / "runtime/hxrt/src/string_scalar.c",
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
        raise StringMapFailure(f"pinned Eval StringMap oracle drifted: {results!r}")


def check_dispatch_owned_map(toolchains: list[Toolchain]) -> None:
    """Keep compiler-owned maps out of ordinary class dispatch discovery."""

    for label in ("first", "second"):
        oracle = run_bounded_process(
            [development_tool("haxe"), "-cp", str(DISPATCH_OWNED), "-main", "Main", "--interp"],
            cwd=ROOT,
            env=haxe_environment(),
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if oracle.returncode != 0 or oracle.stdout or oracle.stderr:
            raise StringMapFailure(
                f"{label} dispatch-owned Eval oracle failed: "
                f"{oracle.returncode} {oracle.stdout!r} {oracle.stderr!r}"
            )

    with tempfile.TemporaryDirectory(prefix="reflaxe-c-string-map-dispatch-owned-") as temporary:
        root = Path(temporary)
        normal = root / "normal"
        reverse = root / "reverse"
        first = compile_haxe(DISPATCH_OWNED, normal, report=True)
        second = compile_haxe(DISPATCH_OWNED, reverse, reverse=True, report=True)
        for label, result in (("normal", first), ("reverse", second)):
            if result.returncode != 0:
                raise StringMapFailure(
                    f"dispatch-owned {label} compile failed: "
                    f"{result.stdout!r} {result.stderr!r}"
                )
        if generated_tree(normal) != generated_tree(reverse):
            raise StringMapFailure("dispatch-owned output changed under reversed discovery")

        hxcir = extract_hxcir(first)
        for marker in (
            'representation=managed("string-map")',
            "static-haxe-string-view:_Main.ItemId",
            'runtime(feature="string-map",operation="create")',
            'runtime(feature="string-map",operation="set")',
            'runtime(feature="string-map",operation="get")',
        ):
            if marker not in hxcir:
                raise StringMapFailure(f"dispatch-owned HxcIR omitted {marker}")
        if "haxe.ds.StringMap" in hxcir or "vtable.haxe.ds.StringMap" in hxcir:
            raise StringMapFailure("dispatch-owned HxcIR retained ordinary StringMap class dispatch")

        sources = sorted((normal / "runtime/src").glob("*.c")) + sorted(
            (normal / "src").rglob("*.c")
        )
        source_text = "\n".join(path.read_text(encoding="utf-8") for path in sources)
        if "haxe_ds_StringMap" in source_text or "hxc_vtable_haxe_ds_StringMap" in source_text:
            raise StringMapFailure("dispatch-owned generated C retained an ordinary StringMap class")

        rejected = compile_haxe(
            NEGATIVE / "abstract_class_value",
            root / "negative-abstract-class",
        )
        if (
            rejected.returncode == 0
            or "HXC1001" not in rejected.stderr
            or "StringMap-value-not-yet-admitted:haxe-class-reference:" not in rejected.stderr
        ):
            raise StringMapFailure(
                "dispatch-owned admission weakened abstract-over-class rejection: "
                f"{rejected.stderr!r}"
            )

        include_roots = [normal / "include", normal / "runtime/include"]
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir()
            for optimization in ("-O0", "-O2"):
                compile_and_run(
                    toolchain.compiler,
                    sources,
                    include_roots,
                    build / f"dispatch-owned-{optimization[1:].lower()}",
                    (optimization,),
                )
            if toolchain.family == "clang":
                compile_and_run(
                    toolchain.compiler,
                    sources,
                    include_roots,
                    build / "dispatch-owned-sanitized",
                    SANITIZER_FLAGS,
                )


def check_collector_minimal(toolchains: list[Toolchain]) -> None:
    """Select key ownership without an incidental String operation or iterator."""
    fixture = CASE / "collector_minimal"
    interpreted = run_bounded_process(
        [development_tool("haxe"), "-cp", str(fixture), "-main", "Main", "--interp"],
        cwd=ROOT, env=haxe_environment(), check=False,
        capture_output=True, text=True, timeout=30,
    )
    if interpreted.returncode or interpreted.stdout or interpreted.stderr:
        raise StringMapFailure(f"minimal collector map Eval failed: {interpreted.stdout}{interpreted.stderr}")
    with tempfile.TemporaryDirectory(prefix="hxc-string-map-minimal-") as directory:
        root = Path(directory)
        output = root / "generated"
        compiled = compile_haxe(fixture, output, layout="unity")
        if compiled.returncode:
            raise StringMapFailure(f"minimal collector map compile failed: {compiled.stdout}{compiled.stderr}")
        sources = [*sorted((output / "runtime/src").glob("*.c")), *sorted((output / "src").rglob("*.c"))]
        includes = [output / "include", output / "runtime/include"]
        for toolchain in toolchains:
            compile_and_run(toolchain.compiler, sources, includes, root / toolchain.family, ("-O2",))


def check_collector_record(toolchains: list[Toolchain]) -> None:
    """Keep map-owned graphs and independent snapshots alive, then reclaim them."""
    interpreted = run_bounded_process(
        [development_tool("haxe"), "-cp", str(COLLECTOR_RECORD), "-main", "Main", "--interp"],
        cwd=ROOT, env=haxe_environment(), check=False,
        capture_output=True, text=True, timeout=30,
    )
    if interpreted.returncode or interpreted.stdout or interpreted.stderr:
        raise StringMapFailure(f"collector record Eval failed: {interpreted.stdout}{interpreted.stderr}")
    with tempfile.TemporaryDirectory(prefix="hxc-string-map-collector-") as temporary:
        root = Path(temporary)
        output = root / "generated"
        compiled = compile_haxe(COLLECTOR_RECORD, output, layout="unity")
        if compiled.returncode:
            raise StringMapFailure(f"collector record compile failed: {compiled.stdout}{compiled.stderr}")
        reverse = root / "reverse"
        reversed_compile = compile_haxe(COLLECTOR_RECORD, reverse, layout="unity", reverse=True)
        if reversed_compile.returncode:
            raise StringMapFailure(f"reversed collector record compile failed: {reversed_compile.stdout}{reversed_compile.stderr}")
        if generated_tree(output) != generated_tree(reverse):
            raise StringMapFailure("collector record output changed under reversed discovery")
        runtime = sorted((output / "runtime/src").glob("*.c"))
        sources = [*runtime, *sorted((output / "src").rglob("*.c"))]
        includes = [output / "include", output / "runtime/include"]
        for toolchain in toolchains:
            for label, flags in (("o0", ("-O0",)), ("o2", ("-O2",))):
                compile_and_run(toolchain.compiler, sources, includes, root / f"{toolchain.family}-{label}", flags)
            if toolchain.family == "clang":
                compile_and_run(toolchain.compiler, sources, includes, root / "sanitized", SANITIZER_FLAGS)
        check_collector_observer(root, output, toolchains)


def check_collector_observer(root: Path, output: Path, toolchains: list[Toolchain]) -> None:
    """Observe real GC pressure, zero surviving allocations, and map allocation abort."""
    symbols = json.loads((output / "hxc.symbols.json").read_text(encoding="utf-8"))
    template = (CASE / "collector_observer.c.in").read_text(encoding="utf-8")
    for marker, parts in (
        ("@COLLECTOR@", ["program", "gc"]),
        ("@THREAD@", ["program", "gc", "thread"]),
        ("@ENTRY@", ["Main", "main"]),
    ):
        names = [entry.get("cName") for entry in symbols["symbols"] if entry.get("readableName") == parts]
        if len(names) != 1 or not isinstance(names[0], str) or re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", names[0]) is None:
            raise StringMapFailure(f"collector observer lost exact symbol {parts!r}")
        template = template.replace(marker, names[0])
    observer = root / "observer/main.c"
    observer.parent.mkdir(exist_ok=True)
    observer.write_text(template, encoding="utf-8")
    sources = [*sorted((output / "runtime/src").glob("*.c")), observer]
    includes = [output / "include", output / "runtime/include"]
    for toolchain in toolchains:
        variants = [("lifecycle", ("-O2",), 0), ("allocation-failure", ("-O0", "-DFIXTURE_FAIL_MAP_ALLOCATION=1"), 77)]
        if toolchain.family == "clang":
            variants.append(("sanitized", SANITIZER_FLAGS, 0))
        for label, flags, expected_exit in variants:
            executable = root / f"observer-{toolchain.family}-{label}"
            command = [toolchain.compiler, *STRICT_FLAGS, *flags, *(f"-I{path}" for path in includes),
                       *(str(path) for path in sources), "-o", str(executable)]
            built = run_bounded_process(command, cwd=ROOT, check=False, capture_output=True, text=True, timeout=60)
            if built.returncode or built.stdout or built.stderr:
                raise StringMapFailure(f"collector observer compile failed: {built.stdout}{built.stderr}")
            observed = run_bounded_process([str(executable)], cwd=ROOT, check=False, capture_output=True, text=True, timeout=30)
            if observed.returncode != expected_exit or observed.stdout or observed.stderr:
                raise StringMapFailure(f"collector observer {label} failed: exit={observed.returncode} {observed.stdout}{observed.stderr}")


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
        'arguments=[string-utf8,i32]',
        'arguments=[string-utf8,managed-string-utf8]',
        'name="Map<String, managed-haxe-string-view:String>"',
        'name="Map<String, managed-haxe-string-view:_Main.StoredName>"',
        'name="Map<String, managed-haxe-string-view:_Main.StoredTag>"',
        'arguments=[string-utf8,instance("instance.enum.',
        'arguments=[string-utf8,instance("instance.closed-record.',
        'runtime(feature="string-map",operation="create")',
        'runtime(feature="string-map",operation="set")',
        'runtime(feature="string-map",operation="get")',
        'runtime(feature="string-map",operation="remove")',
        'runtime(feature="string-map",operation="copy")',
		'runtime(feature="string-map",operation="keys")',
		'runtime(feature="string-map",operation="key-value-iterator")',
		'runtime(feature="string-map",operation="to-string")',
        'binary operation="haxe.string-map-reference.equal"',
        'binary operation="haxe.string-map-reference.not-equal"',
        "static-call-argument-0-owner-initialize",
        "instance-call-argument-0-owner-initialize",
        "string-map-temporary.local.",
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
        "array",
        "iterator",
        "string-literal",
        "string-scalar",
        "string",
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
        "copy",
        "create",
        "exists",
        "get",
        "managed-type-representation",
        "remove",
        "retain",
        "set",
        "iterator",
        "keys",
        "key-value-iterator",
        "to-string",
    }
    if operations != expected:
        raise StringMapFailure(
            f"generated StringMap operations drifted: {sorted(operations)!r}"
        )
    string_operations = {
        reason.get("operationId")
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict) and reason.get("featureId") == "string"
    }
    if string_operations != {"cleanup-release", "concat", "from-int", "retain", "type-carrier"}:
        raise StringMapFailure(
            f"generated nominal-String operations drifted: {sorted(string_operations)!r}"
        )
    if "managed-haxe-string-maps" not in plan.get("directDecisions", []):
        raise StringMapFailure("runtime plan omitted the exact StringMap representation decision")
    if "managed-haxe-arrays" not in plan.get("directDecisions", []):
        raise StringMapFailure("managed record fixture omitted its nested Array representation")
    if "managed-haxe-iterators" not in plan.get("directDecisions", []):
        raise StringMapFailure("managed record fixture omitted its shared Iterator representation")

    iterator_operations = {
        reason.get("operationId")
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict) and reason.get("featureId") == "iterator"
    }
    if iterator_operations != {
        "cleanup-release",
        "has-next",
        "managed-type-representation",
        "next",
        "retain",
    }:
        raise StringMapFailure(
            f"generated Iterator operations drifted: {sorted(iterator_operations)!r}"
        )

    sources = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "src").rglob("*.c"))
    )
    for marker in (
        "struct hxc_string_map_ref *",
        "hxc_string_map_ref_create",
        "hxc_string_map_ref_create_with_ops",
        "hxc_string_map_value_ops",
        "hxc_string_map_ref_set_copy",
        "hxc_string_map_ref_get_copy",
        "hxc_string_map_ref_copy",
        "hxc_string_map_ref_retain",
        "hxc_string_map_ref_release",
        "hxc_string_map_ref_value_iterator",
		"hxc_string_map_ref_key_iterator",
		"hxc_string_map_ref_pair_iterator",
		"hxc_string_map_ref_to_string",
        "hxc_iterator_ref_has_next",
        "hxc_iterator_ref_next_move",
        "hxc_iterator_ref_retain",
        "hxc_iterator_ref_release",
        "sizeof(bool)",
        "_Alignof(bool)",
        "value_copy",
        "value_assign",
        "value_destroy",
        "hxc_array_ref_retain",
        "hxc_array_ref_release",
        "hxc_string_retain",
        "hxc_string_release",
        "sizeof(hxc_string)",
        "hxc_l_tmp_discarded_string_owner",
        "hxc_l_tmp_string_map_set_key_owner",
        "hxc_l_tmp_string_map_set_value_owner",
        "sizeof(int32_t)",
    ):
        if marker not in sources:
            raise StringMapFailure(f"generated C omitted {marker}")
    # Lifecycle callbacks use typed casts behind an ABI-required `void *`
    # boundary. That boundary is not Haxe `Dynamic`: the callback is generated
    # for one exact record type, and the checks above prove its complete
    # copy/assign/destroy family. Reject the actual dynamic runtime family
    # instead of rejecting every well-typed opaque callback parameter.
    for forbidden in ("hxc_dynamic", "goto "):
        if forbidden in sources:
            raise StringMapFailure(f"generated C retained forbidden shape {forbidden!r}")
    for owner in (
        "discarded_string_owner",
        "string_map_set_key_owner",
        "string_map_set_value_owner",
    ):
        if re.search(rf"hxc_string_release\(&hxc_l_tmp_{owner}_n[0-9]+\)", sources) is None:
            raise StringMapFailure(f"generated C did not release its {owner.replace('_', ' ')}")
    for role in ("static", "instance"):
        if re.search(rf"hxc_string_map_ref_release\(hxc_l_tmp_{role}_call_argument_0_owner_n[0-9]+\)", sources) is None:
            raise StringMapFailure(f"generated C did not release its fresh {role}-call StringMap owner")


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
    # Object keys are supported and covered by the object-enum-map positive suite.
    expected = {
        "value_type": "StringMap-value-not-yet-admitted:double",
        "class_value": "StringMap-value-not-yet-admitted:haxe-class-reference:",
        "abstract_class_value": "StringMap-value-not-yet-admitted:haxe-class-reference:",
        "payload_enum_value": "StringMap-value-not-yet-admitted:haxe-enum:",
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
    compiled = run_bounded_process(
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
    executed = run_bounded_process(
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


def check_direct_runtime_decisions(toolchains: list[Toolchain]) -> None:
    """Keep dependency artifacts distinct from source-reached behavior."""
    oracle = run_bounded_process(
        [
            development_tool("haxe"),
            "-cp",
            str(DIRECT_DECISION),
            "-main",
            "Main",
            "--interp",
        ],
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if oracle.returncode != 0 or oracle.stdout or oracle.stderr:
        raise StringMapFailure(
            "StringMap direct-decision Eval oracle failed: "
            f"{oracle.returncode} {oracle.stdout!r} {oracle.stderr!r}"
        )

    with tempfile.TemporaryDirectory(
        prefix="reflaxe-c-string-map-direct-decisions-"
    ) as temporary:
        root = Path(temporary)
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
            rendered: list[tuple[Path, str]] = []
            for label in ("server-cold", "server-warm"):
                output = root / label
                result = compile_haxe(
                    DIRECT_DECISION,
                    output,
                    layout="unity",
                    report=True,
                    connect=endpoint,
                )
                if result.returncode != 0:
                    raise StringMapFailure(
                        f"{label} direct-decision compile failed: "
                        f"{result.stdout!r} {result.stderr!r}"
                    )
                rendered.append((output, extract_hxcir(result)))
        finally:
            server.terminate()
            try:
                server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                server.kill()
                server.wait(timeout=5)

        output, hxcir = rendered[0]
        warm_output, warm_hxcir = rendered[1]
        if hxcir != warm_hxcir or generated_tree(output) != generated_tree(warm_output):
            raise StringMapFailure(
                "StringMap direct-decision output changed under warm server reuse"
            )
        if (
            'representation=managed("string-map")' not in hxcir
            or 'representation=managed("iterator")' in hxcir
            or 'runtime(feature="iterator"' in hxcir
        ):
            raise StringMapFailure(
                "StringMap-only HxcIR gained an unrequested Iterator value or operation"
            )

        plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
        expected_features = [
            "runtime-base",
            "status",
            "alloc",
            "array",
            "iterator",
            "string-literal",
            "string-scalar",
            "string",
            "string-map",
        ]
        if plan.get("features") != expected_features:
            raise StringMapFailure(
                "StringMap-only runtime dependency closure drifted: "
                f"{plan.get('features')!r}"
            )
        root_features = {
            reason.get("featureId")
            for reason in plan.get("rootReasons", [])
            if isinstance(reason, dict)
        }
        if root_features != {"string-literal", "string-map"}:
            raise StringMapFailure(
                f"StringMap-only runtime roots drifted: {sorted(root_features)!r}"
            )
        expected_decisions = [
            "direct-calls",
            "direct-utf8-string-literals",
            "executable-entry-point",
            "explicit-evaluation-order",
            "managed-haxe-string-maps",
            "primitive-static-storage",
            "primitive-values",
            "static-functions",
            "ub-safe-primitive-operations",
        ]
        if plan.get("directDecisions") != expected_decisions:
            raise StringMapFailure(
                "StringMap-only direct decisions confused dependencies with source use: "
                f"{plan.get('directDecisions')!r}"
            )
        iterator_feature = next(
            (
                feature
                for feature in plan.get("selectedFeatures", [])
                if isinstance(feature, dict) and feature.get("id") == "iterator"
            ),
            None,
        )
        if iterator_feature is None or iterator_feature.get("root") is not False:
            raise StringMapFailure(
                "StringMap-only runtime plan lost its transitive Iterator artifact"
            )

        program_sources = "\n".join(
            path.read_text(encoding="utf-8")
            for path in sorted((output / "src").rglob("*.c"))
        )
        if "hxc_iterator_ref_" in program_sources:
            raise StringMapFailure(
                "StringMap-only generated application C called an Iterator operation"
            )

        sources = sorted((output / "runtime/src").glob("*.c")) + sorted(
            (output / "src").rglob("*.c")
        )
        include_roots = [output / "include", output / "runtime/include"]
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir()
            for optimization in ("-O0", "-O2"):
                compile_and_run(
                    toolchain.compiler,
                    sources,
                    include_roots,
                    build / f"direct-decisions-{optimization[1:].lower()}",
                    (optimization,),
                )
            if toolchain.family == "clang":
                compile_and_run(
                    toolchain.compiler,
                    sources,
                    include_roots,
                    build / "direct-decisions-sanitized",
                    SANITIZER_FLAGS,
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
    result = run_bounded_process(command, cwd=ROOT, check=False, capture_output=True, text=True, timeout=30)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise StringMapFailure(f"{family} C++ private-header check failed: {result.stderr!r}")


def inspect_symbols(executable: Path, family: str, *, allow_array: bool = False) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise StringMapFailure(f"{family} StringMap evidence requires nm")
    result = run_bounded_process([nm, str(executable)], check=False, capture_output=True, text=True, timeout=20)
    if result.returncode != 0:
        raise StringMapFailure(f"{family} could not inspect StringMap symbols")
    for required in (
        "hxc_string_map_ref_create",
        "hxc_string_map_ref_create_with_ops",
        "hxc_string_map_ref_get_copy",
        "hxc_string_map_ref_copy",
        "hxc_string_map_ref_release",
        "hxc_string_map_ref_value_iterator",
        "hxc_iterator_ref_has_next",
        "hxc_iterator_ref_next_move",
        "hxc_string_map_value_ops_is_valid",
    ):
        if required not in result.stdout:
            raise StringMapFailure(f"{family} omitted required symbol {required}")
    forbidden_families = ["hxc_bytes", "hxc_gc", "hxc_dynamic"]
    if not allow_array:
        forbidden_families.append("hxc_array")
    for forbidden in forbidden_families:
        if forbidden in result.stdout:
            raise StringMapFailure(f"{family} retained unrelated symbol family {forbidden}")


def run_native(toolchains: list[Toolchain], *, generated_haxe: bool) -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-string-map-") as temporary:
        root = Path(temporary)
        if generated_haxe:
            check_direct_runtime_decisions(toolchains)
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
            inspect_symbols(native, toolchain.family, allow_array=True)
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
                    inspect_symbols(generated_executable, toolchain.family, allow_array=True)
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
    parser.add_argument("--direct-decision-only", action="store_true")
    parser.add_argument("--dispatch-owned-only", action="store_true")
    parser.add_argument("--collector-record-only", action="store_true")
    parser.add_argument("--collector-minimal-only", action="store_true")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        toolchains = resolve_toolchains(args.toolchain)
        if args.collector_minimal_only:
            check_collector_minimal(toolchains)
            print("string-map: OK: minimal collector map key ownership passed")
            return 0
        if args.collector_record_only:
            check_collector_minimal(toolchains)
            check_collector_record(toolchains)
            print("string-map: OK: collector record semantics, reclamation, and allocation failure")
            return 0
        if args.dispatch_owned_only:
            check_dispatch_owned_map(toolchains)
            print(
                "string-map: OK: compiler-owned map construction bypasses class "
                "dispatch while nominal String values retain exact typed storage"
            )
            return 0
        if args.direct_decision_only:
            check_direct_runtime_decisions(toolchains)
            print(
                "string-map: OK: dependency-closed Iterator artifacts remain "
                "distinct from source-reached direct decisions"
            )
            return 0
        if not args.native_only:
            run_eval_oracle()
            check_collector_minimal(toolchains)
            check_collector_record(toolchains)
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
    mode = (
        "native contract"
        if args.native_only
        else "Eval plus generated Bool/Int/nominal-String/fieldless-enum/managed-record StringMaps"
    )
    print(
        "string-map: OK: "
        f"{families}; {mode}; missing-vs-false, replacement, removal, clear, copy independence, aliases, "
        "nullable identity, empty keys, growth, allocation rollback, value-callback rollback, snapshot values/keys/pairs, toString, "
        "unsupported-class/abstract-class/payload-enum rejection, "
        "malformed-call rejection, layouts, determinism, sanitizers, C++ headers, runtime-none, and selective symbols passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

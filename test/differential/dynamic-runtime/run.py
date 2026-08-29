#!/usr/bin/env python3
"""Prove exact closed-world Dynamic lowering through generated native C."""

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


ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402

CASE = Path(__file__).resolve().parent
FIXTURE = CASE / "fixtures"
SCALAR = CASE / "scalar"
TYPED = CASE / "typed"
NEGATIVE = CASE / "negative"
MAP_KEY = ROOT / "test/differential/object-enum-map/negative/dynamic_key"
OPEN_GENERIC = ROOT / "test/generic_specialization/fixtures/dynamic"
DYNAMIC_PARAMETER = ROOT / "test/aggregate_lowering/fixtures/dynamic"
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
EXPECTED = (
    "7:2.5:true:caxe:3:6:15:15:10:caxe:held:changed:changed:"
    "true:true:true:true\n"
)
LAYOUTS = ("split", "package", "unity")
DYNAMIC_OPERATIONS = frozenset(
    ("box", "box-null", "box-type-token", "unbox", "get", "set", "call", "invoke", "equal")
)
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


class DynamicLoweringFailure(RuntimeError):
    """One exact Dynamic compiler or native contract failed."""


@dataclass(frozen=True)
class Toolchain:
    """One identified C and C++ compiler family."""

    family: str
    cc: str
    cxx: str


def development_tool(name: str) -> str:
    """Prefer the repository-pinned executable when it exists."""

    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(*, server: bool = False) -> dict[str, str]:
    """Select cold compilation or explicitly permit one owned warm server."""

    environment = os.environ.copy()
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


def compile_haxe(
    fixture: Path,
    output: Path,
    *,
    layout: str = "split",
    connect: str | None = None,
    report: bool = False,
    defines: tuple[str, ...] = (),
) -> subprocess.CompletedProcess[str]:
    """Compile one ordinary Haxe fixture through the production custom target."""

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
            "hxc_runtime_diagnostics=off",
            "-D",
            f"hxc_project_layout={layout}",
        ]
    )
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


def require_compile(
    result: subprocess.CompletedProcess[str], label: str
) -> None:
    """Report one compiler failure with its bounded diagnostics."""

    if result.returncode != 0:
        raise DynamicLoweringFailure(
            f"{label} failed\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def run_eval() -> None:
    """Pin the observable reference result to Haxe Eval once."""

    result = run_bounded_process(
        [development_tool("haxe"), "-cp", str(FIXTURE), "-main", "Main", "--interp"],
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=45,
    )
    if result.returncode != 0 or result.stdout != EXPECTED or result.stderr:
        raise DynamicLoweringFailure(
            "Dynamic Eval oracle drifted\n"
            f"exit={result.returncode} stdout={result.stdout!r} stderr={result.stderr!r}"
        )


def available_port() -> int:
    """Reserve one loopback port number for the immediately started server."""

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as reservation:
        reservation.bind(("127.0.0.1", 0))
        return int(reservation.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    """Wait only for the exact compiler process started by this runner."""

    deadline = time.monotonic() + 10.0
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise DynamicLoweringFailure(
                "Haxe server exited before accepting requests\n"
                f"stdout={stdout!r} stderr={stderr!r}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise DynamicLoweringFailure("Haxe server did not accept requests within 10 seconds")


def generated_tree(output: Path) -> dict[str, bytes]:
    """Return deterministic generated bytes while excluding the write ledger."""

    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def extract_hxcir(result: subprocess.CompletedProcess[str]) -> str:
    """Read the one complete semantic report emitted by the focused compile."""

    reports = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(reports) != 1:
        raise DynamicLoweringFailure("Dynamic compile omitted its one HxcIR report")
    try:
        payload = json.loads(reports[0])
    except json.JSONDecodeError as error:
        raise DynamicLoweringFailure("Dynamic HxcIR report is invalid JSON") from error
    hxcir = payload.get("hxcir") if isinstance(payload, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise DynamicLoweringFailure("Dynamic HxcIR report omitted semantic text")
    return hxcir


def validate_hxcir(hxcir: str) -> None:
    """Prove exact Dynamic meaning before generated C syntax is selected."""

    markers = (
        "hxcir schema=27\n",
        "dynamic\n",
        "category=int storage=inline-int32",
        "category=float storage=inline-float64",
        "category=bool storage=inline-bool",
        "category=string storage=managed-wrapper",
        "category=object storage=managed-reference",
        "category=enum storage=managed-wrapper",
        "category=function storage=managed-wrapper",
        "category=type-value storage=static-token",
        "dynamic-box ",
        "dynamic-box-null ",
        "dynamic-box-type-token ",
        "dynamic-unbox ",
        "dynamic-get ",
        "dynamic-set ",
        "dynamic-call ",
        "dynamic-invoke ",
        "dynamic-equal ",
        'path="dynamic-payload"',
    )
    for marker in markers:
        if marker not in hxcir:
            raise DynamicLoweringFailure(f"Dynamic HxcIR omitted {marker!r}")
    members = [line for line in hxcir.splitlines() if line.startswith("  member ")]
    if len(members) < 4 or any(re.search(r" token=[0-9]+ ", line) is None for line in members):
        raise DynamicLoweringFailure("Dynamic HxcIR lost exact numeric member tokens")
    if " raw" in hxcir or str(ROOT) in hxcir:
        raise DynamicLoweringFailure("Dynamic HxcIR used raw syntax or leaked a local path")


def runtime_plan(project: Path) -> dict[str, object]:
    """Read one full compiler-owned runtime plan."""

    plan = json.loads((project / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if not isinstance(plan, dict):
        raise DynamicLoweringFailure("runtime plan is not a JSON object")
    return plan


def selected_features(project: Path) -> set[str]:
    """Read the exact packaged runtime closure."""

    plan = runtime_plan(project)
    features = plan.get("selectedFeatures")
    if not isinstance(features, list):
        raise DynamicLoweringFailure("runtime plan omitted selectedFeatures")
    result: set[str] = set()
    for feature in features:
        if not isinstance(feature, dict) or not isinstance(feature.get("id"), str):
            raise DynamicLoweringFailure("runtime plan contains an invalid feature entry")
        result.add(str(feature["id"]))
    return result


def source_line_for_marker(marker: str) -> int:
    """Locate one independently named carrier declaration in the fixture."""

    lines = FIXTURE.joinpath("Main.hx").read_text(encoding="utf-8").splitlines()
    matches = [index for index, line in enumerate(lines, start=1) if marker in line]
    if len(matches) != 1:
        raise DynamicLoweringFailure(
            f"Dynamic fixture marker {marker!r} matched {len(matches)} lines"
        )
    return matches[0]


def runtime_semantic_projection(plan: dict[str, object]) -> dict[str, object]:
    """Remove only policy labels before comparing equivalent feature plans."""

    excluded = {"requestedPolicy", "resolvedPolicy", "policyProvenance"}
    return {key: value for key, value in plan.items() if key not in excluded}


def validate_dynamic_runtime_plan(
    project: Path, *, policy: str, provenance: str
) -> dict[str, object]:
    """Bind every admitted carrier and operation to full source evidence."""

    plan = runtime_plan(project)
    if (
        plan.get("schemaVersion") != 2
        or plan.get("algorithm") != "hxc-runtime-plan-v2"
        or plan.get("status") != "analyzed-runtime-features"
        or plan.get("planPurpose") != "compiler-program"
        or plan.get("requestedPolicy") != policy
        or plan.get("resolvedPolicy") != policy
        or plan.get("policyProvenance") != provenance
        or plan.get("diagnosticMode") != "off"
        or plan.get("diagnosticProvenance")
        != "direct-define:hxc_runtime_diagnostics"
        or plan.get("noRuntimeProof") is not None
    ):
        raise DynamicLoweringFailure(
            f"Dynamic runtime plan lost its full {policy} policy evidence"
        )

    root_reasons = plan.get("rootReasons")
    if not isinstance(root_reasons, list) or not root_reasons:
        raise DynamicLoweringFailure("Dynamic runtime plan omitted full root reasons")
    reasons = [
        reason
        for reason in root_reasons
        if isinstance(reason, dict) and reason.get("featureId") == "dynamic"
    ]
    operations = {reason.get("operationId") for reason in reasons}
    if operations != DYNAMIC_OPERATIONS:
        raise DynamicLoweringFailure(
            f"Dynamic runtime reasons cover {sorted(str(value) for value in operations)!r}"
        )

    reason_ids: list[str] = []
    operation_lines: set[tuple[str, int]] = set()
    for reason in reasons:
        reason_id = reason.get("id")
        source = reason.get("source")
        if (
            not isinstance(reason_id, str)
            or not reason_id
            or reason_id in reason_ids
            or reason.get("kind") != "runtime-operation"
            or reason.get("surface") != "closed-world Haxe Dynamic operation"
            or not isinstance(source, dict)
        ):
            raise DynamicLoweringFailure(
                "Dynamic runtime reason lost its unique typed operation contract"
            )
        source_file = source.get("file")
        start = source.get("start")
        end = source.get("end")
        if (
            not isinstance(source_file, str)
            or (
                source_file != "Main.hx"
                and not source_file.endswith("dynamic-runtime/fixtures/Main.hx")
            )
            or source_file.startswith("/")
            or str(ROOT) in source_file
            or not isinstance(start, dict)
            or not isinstance(end, dict)
            or not isinstance(start.get("line"), int)
            or not isinstance(start.get("column"), int)
            or not isinstance(end.get("line"), int)
            or not isinstance(end.get("column"), int)
            or int(start["line"]) < 1
            or int(start["column"]) < 1
            or (int(end["line"]), int(end["column"]))
            < (int(start["line"]), int(start["column"]))
        ):
            raise DynamicLoweringFailure(
                f"Dynamic runtime reason {reason_id!r} has no normalized source span"
            )
        reason_ids.append(reason_id)
        operation_lines.add((str(reason["operationId"]), int(start["line"])))

    carrier_markers = {
        "final integer:ReferenceDynamic": "box",
        "final floating:ReferenceDynamic": "box",
        "final boolean:ReferenceDynamic": "box",
        "final text:ReferenceDynamic": "box",
        "final absent:ReferenceDynamic": "box-null",
        "final point:ReferenceDynamic": "box",
        "final counter:ReferenceDynamic": "box",
        "final tone:ReferenceDynamic": "box",
        "final callable:ReferenceDynamic": "box",
        "final textCallable:ReferenceDynamic": "box",
        "final holder:ReferenceDynamic": "box",
        "final opaqueType:ReferenceDynamic": "box-type-token",
    }
    missing_carriers = [
        marker
        for marker, operation in carrier_markers.items()
        if (operation, source_line_for_marker(marker)) not in operation_lines
    ]
    if missing_carriers:
        raise DynamicLoweringFailure(
            "Dynamic runtime report omitted carrier reasons "
            f"{missing_carriers!r}; observed={sorted(operation_lines)!r}"
        )
    # The Array literal is immediately restored to the same exact Array<Int>
    # before its first element is read. Whole-program lowering therefore proves
    # that no Dynamic operation is observable and keeps its three direct values.
    # A runtime reason here would mean selective boxing regressed.
    elided_array_line = source_line_for_marker("final values:ReferenceDynamic")
    if any(line == elided_array_line for _, line in operation_lines):
        raise DynamicLoweringFailure(
            "non-observable exact Array round-trip gained a Dynamic runtime reason"
        )

    selected = plan.get("selectedFeatures")
    dynamic_feature = next(
        (
            feature
            for feature in selected
            if isinstance(feature, dict) and feature.get("id") == "dynamic"
        ),
        None,
    ) if isinstance(selected, list) else None
    if (
        not isinstance(dynamic_feature, dict)
        or dynamic_feature.get("root") is not True
        or set(dynamic_feature.get("reasonIds", [])) != set(reason_ids)
        or "runtime/include/hxrt/dynamic.h" not in dynamic_feature.get("artifacts", [])
        or "runtime/src/dynamic.c" not in dynamic_feature.get("artifacts", [])
    ):
        raise DynamicLoweringFailure(
            "selected Dynamic feature lost its exact reasons or packaged artifacts"
        )
    return plan


def validate_typed_runtime_none(project: Path) -> None:
    """Prove a typed neighbor succeeds with an explicit zero-runtime policy."""

    plan = runtime_plan(project)
    proof = plan.get("noRuntimeProof")
    if (
        plan.get("status") != "analyzed-runtime-free"
        or plan.get("requestedPolicy") != "none"
        or plan.get("resolvedPolicy") != "none"
        or plan.get("policyProvenance") != "direct-define:hxc_runtime"
        or plan.get("rootReasons") != []
        or plan.get("selectedFeatures") != []
        or plan.get("features") != []
        or plan.get("artifacts") != []
        or plan.get("symbols") != []
        or not isinstance(proof, dict)
        or proof.get("status") != "eligible"
    ):
        raise DynamicLoweringFailure(
            "ordinary typed control flow lost its exact runtime-none proof"
        )


def validate_runtime_none_rejection(
    result: subprocess.CompletedProcess[str], auto_plan: dict[str, object]
) -> None:
    """Require every Dynamic blocker to remain source-positioned under none."""

    root_reasons = auto_plan.get("rootReasons")
    expected_count = len(root_reasons) if isinstance(root_reasons, list) else -1
    if (
        result.returncode == 0
        or f"runtime policy `none` found {expected_count} deduplicated runtime blocker(s)"
        not in result.stderr
        or "kind=runtime-operation" not in result.stderr
        or (
            "source=Main.hx:" not in result.stderr
            and "source=test/differential/dynamic-runtime/fixtures/Main.hx:"
            not in result.stderr
        )
    ):
        raise DynamicLoweringFailure(
            "runtime-none lost its exact source-positioned blocker summary"
        )
    for operation in DYNAMIC_OPERATIONS:
        if (
            f"operation={operation} surface=`closed-world Haxe Dynamic operation`"
            not in result.stderr
        ):
            raise DynamicLoweringFailure(
                f"runtime-none omitted the Dynamic {operation!r} blocker"
            )


def application_text(project: Path) -> str:
    """Read generated application C without copied runtime implementation text."""

    return "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((project / "src").rglob("*.c"))
    )


def validate_dynamic_project(project: Path) -> None:
    """Check runtime closure, numeric adapters, and collector publication."""

    features = selected_features(project)
    for required in ("dynamic", "alloc", "object", "gc"):
        if required not in features:
            raise DynamicLoweringFailure(f"managed Dynamic project omitted {required}")
    source = application_text(project)
    if "hxc_value_managed_payload(" not in source:
        raise DynamicLoweringFailure(
            "generated Dynamic roots bypassed the checked managed-payload accessor"
        )
    for reflected_name in ("x", "value", "bump", "text"):
        if f'"{reflected_name}"' in source:
            raise DynamicLoweringFailure(
                f"generated Dynamic adapter retained reflection name {reflected_name!r}"
            )
    for forbidden in ("Reflect", "field_by_name", "member_name"):
        if forbidden in source:
            raise DynamicLoweringFailure(
                f"generated Dynamic adapter gained reflection surface {forbidden!r}"
            )


def validate_scalar_project(project: Path) -> None:
    """Keep scalar Dynamic allocation-free and collector-free."""

    features = selected_features(project)
    if "dynamic" not in features:
        raise DynamicLoweringFailure("scalar Dynamic fixture omitted the carrier runtime")
    forbidden = features.intersection({"alloc", "object", "gc", "array", "string"})
    if forbidden:
        raise DynamicLoweringFailure(
            f"scalar Dynamic fixture selected managed features: {sorted(forbidden)!r}"
        )
    if "hxc_value" not in application_text(project):
        raise DynamicLoweringFailure("scalar Dynamic fixture omitted its exact carrier")


def validate_typed_project(project: Path) -> None:
    """Prove neighboring typed source never enters the Dynamic carrier."""

    features = selected_features(project)
    source = application_text(project)
    if "dynamic" in features or "hxc_value" in source or "HXC_DYNAMIC" in source:
        raise DynamicLoweringFailure(
            "ordinary typed control flow was routed through the Dynamic carrier"
        )


def require_no_output(output: Path, label: str) -> None:
    """Fail if a rejected program left any plausible generated artifact."""

    if output.exists() and any(path.is_file() for path in output.rglob("*")):
        raise DynamicLoweringFailure(f"{label} left generated files after rejection")


def run_negative_cases(root: Path, endpoint: str) -> None:
    """Pin every unsupported shape to its first exact no-output diagnostic."""

    cases = (
        (
            "global",
            NEGATIVE / "global",
            "TField(static:stored:the dynamic source semantic type cannot stand in for a primitive)",
        ),
        (
            "polymorphic",
            NEGATIVE / "polymorphic",
            "Dynamic(box-unsupported-exact-type:haxe-interface-reference:",
        ),
        (
            "computed-name",
            NEGATIVE / "computed-name",
            "TCall(unavailable-static-target:function.Reflect.field)",
        ),
        (
            "bound-method",
            NEGATIVE / "bound-method",
            "Dynamic(field `bump` is-not-statically-known)",
        ),
        (
            "payload-enum-equality",
            NEGATIVE / "payload-enum-equality",
            "Dynamic(box-unsupported-exact-type:haxe-enum:",
        ),
        (
            "arithmetic",
            NEGATIVE / "arithmetic",
            "TBinop(OpDiv:left-type):closed-record-not-admitted-in-primitive-operation",
        ),
        (
            "dynamic-map-key",
            MAP_KEY,
            "object-map-key-not-admitted:haxe-dynamic",
        ),
        (
            "open-generic",
            OPEN_GENERIC,
            "generic-specialization:function.Main.identity:type-argument:T):dynamic-type-argument",
        ),
        (
            "unresolved-parameter",
            DYNAMIC_PARAMETER,
            "Dynamic(field `value` receiver has-unresolved-polymorphic-identity)",
        ),
    )
    for name, fixture, marker in cases:
        output = root / f"negative-{name}"
        result = compile_haxe(fixture, output, connect=endpoint)
        if result.returncode == 0 or "HXC1001:" not in result.stderr or marker not in result.stderr:
            raise DynamicLoweringFailure(
                f"negative Dynamic case {name} drifted\n"
                f"exit={result.returncode} stdout={result.stdout!r} stderr={result.stderr!r}"
            )
        require_no_output(output, f"negative Dynamic case {name}")


def render_projects(root: Path) -> Path:
    """Generate one cold reference and reuse one server for independent lanes."""

    split = root / "dynamic-split-cold"
    cold = compile_haxe(FIXTURE, split, report=True)
    require_compile(cold, "cold split Dynamic compile")
    hxcir = extract_hxcir(cold)
    validate_hxcir(hxcir)
    validate_dynamic_project(split)
    auto_plan = validate_dynamic_runtime_plan(
        split, policy="auto", provenance="profile-preset:portable"
    )
    cold_tree = generated_tree(split)

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
        for label in ("first", "second"):
            output = root / f"dynamic-split-warm-{label}"
            result = compile_haxe(FIXTURE, output, connect=endpoint)
            require_compile(result, f"warm split Dynamic compile {label}")
            if generated_tree(output) != cold_tree:
                raise DynamicLoweringFailure(
                    f"warm split Dynamic project {label} changed from the cold project"
                )
        for layout in LAYOUTS[1:]:
            output = root / f"dynamic-{layout}"
            result = compile_haxe(FIXTURE, output, layout=layout, connect=endpoint)
            require_compile(result, f"{layout} Dynamic compile")
            if not generated_tree(output):
                raise DynamicLoweringFailure(f"{layout} Dynamic project is empty")
            validate_dynamic_project(output)
            layout_plan = validate_dynamic_runtime_plan(
                output, policy="auto", provenance="profile-preset:portable"
            )
            if layout_plan != auto_plan:
                raise DynamicLoweringFailure(
                    f"{layout} Dynamic runtime plan changed from split"
                )

        minimal = root / "dynamic-minimal"
        minimal_result = compile_haxe(
            FIXTURE,
            minimal,
            connect=endpoint,
            defines=("hxc_runtime=minimal",),
        )
        require_compile(minimal_result, "minimal-policy Dynamic compile")
        validate_dynamic_project(minimal)
        minimal_plan = validate_dynamic_runtime_plan(
            minimal, policy="minimal", provenance="direct-define:hxc_runtime"
        )
        if runtime_semantic_projection(minimal_plan) != runtime_semantic_projection(
            auto_plan
        ):
            raise DynamicLoweringFailure(
                "minimal and auto selected different Dynamic semantics or packages"
            )

        scalar = root / "dynamic-scalar"
        scalar_result = compile_haxe(SCALAR, scalar, connect=endpoint)
        require_compile(scalar_result, "scalar Dynamic compile")
        validate_scalar_project(scalar)

        typed = root / "dynamic-typed"
        typed_result = compile_haxe(
            TYPED, typed, connect=endpoint, defines=("hxc_runtime=none",)
        )
        require_compile(typed_result, "typed control compile")
        validate_typed_project(typed)
        validate_typed_runtime_none(typed)

        run_negative_cases(root, endpoint)
        runtime_none = root / "dynamic-runtime-none"
        rejected = compile_haxe(
            FIXTURE,
            runtime_none,
            connect=endpoint,
            defines=("hxc_runtime=none",),
        )
        validate_runtime_none_rejection(rejected, auto_plan)
        require_no_output(runtime_none, "Dynamic runtime-none case")
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)
    return split


def compiler_family(command: str) -> str:
    """Identify a native compiler instead of trusting its executable name."""

    result = run_bounded_process(
        [command, "--version"],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        return "unknown"
    text = (result.stdout + result.stderr).lower()
    if "clang" in text:
        return "clang"
    if "free software foundation" in text or "gcc" in text or "g++" in text:
        return "gcc"
    return "unknown"


def resolve_toolchains(requested: str) -> list[Toolchain]:
    """Return every available identified strict C/C++ family."""

    names = (("gcc", "gcc", "g++"), ("clang", "clang", "clang++"))
    found: list[Toolchain] = []
    for family, cc_name, cxx_name in names:
        if requested != "auto" and requested != family:
            continue
        cc = shutil.which(cc_name)
        cxx = shutil.which(cxx_name)
        if (
            cc is None
            or cxx is None
            or compiler_family(cc) != family
            or compiler_family(cxx) != family
        ):
            if requested == family:
                raise DynamicLoweringFailure(
                    f"requested {family} C/C++ toolchain is unavailable or misidentified"
                )
            print(
                f"dynamic-lowering: SKIP optional {family}: unavailable or identity mismatch"
            )
            continue
        found.append(Toolchain(family, cc, cxx))
    if not found:
        raise DynamicLoweringFailure("no identified GCC or Clang C/C++ toolchain is available")
    return found


def project_sources(project: Path) -> list[Path]:
    """Read source ownership from the generated build-neutral manifest."""

    manifest = json.loads((project / "hxc.manifest.json").read_text(encoding="utf-8"))
    artifacts = manifest.get("artifacts")
    if not isinstance(artifacts, list):
        raise DynamicLoweringFailure("generated manifest omitted artifacts")
    sources: list[Path] = []
    for artifact in artifacts:
        if not isinstance(artifact, dict):
            continue
        if artifact.get("kind") not in {"source", "runtime-source"}:
            continue
        relative = artifact.get("path")
        if not isinstance(relative, str):
            raise DynamicLoweringFailure("generated source artifact omitted its path")
        source = project / relative
        if not source.is_file():
            raise DynamicLoweringFailure(f"generated source is missing: {relative}")
        sources.append(source)
    if not sources:
        raise DynamicLoweringFailure("generated manifest selected no C sources")
    return sources


def compile_and_run(
    toolchain: Toolchain,
    project: Path,
    build: Path,
    label: str,
    flags: tuple[str, ...],
) -> None:
    """Compile one generated project and require exact Eval parity."""

    executable = build / label
    command = [
        toolchain.cc,
        *STRICT_FLAGS,
        *flags,
        f"-I{project / 'include'}",
        f"-I{project / 'runtime/include'}",
        *(str(source) for source in project_sources(project)),
        "-o",
        str(executable),
    ]
    compiled = run_bounded_process(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=120,
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise DynamicLoweringFailure(
            f"{toolchain.family} {label} compile failed\n"
            f"command={command!r}\nstdout={compiled.stdout!r}\nstderr={compiled.stderr!r}"
        )
    environment = os.environ.copy()
    if "sanitize" in label:
        environment["ASAN_OPTIONS"] = "detect_leaks=0:halt_on_error=1"
        environment["UBSAN_OPTIONS"] = "halt_on_error=1:print_stacktrace=1"
    executed = run_bounded_process(
        [str(executable)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if executed.returncode != 0 or executed.stdout != EXPECTED or executed.stderr:
        raise DynamicLoweringFailure(
            f"{toolchain.family} {label} execution drifted\n"
            f"exit={executed.returncode} stdout={executed.stdout!r} stderr={executed.stderr!r}"
        )


def compile_cpp_header(toolchain: Toolchain, project: Path, build: Path) -> None:
    """Compile the generated public include graph as strict C++17."""

    probe = build / f"header-{toolchain.family}.cpp"
    probe.write_text('#include "hxc/program.h"\nint main() { return 0; }\n', encoding="utf-8")
    command = [
        toolchain.cxx,
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-pedantic",
        "-Wconversion",
        "-Wsign-conversion",
        "-Wundef",
        "-fsyntax-only",
        f"-I{project / 'include'}",
        f"-I{project / 'runtime/include'}",
        str(probe),
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
        raise DynamicLoweringFailure(
            f"{toolchain.family} generated C++ header check failed\n"
            f"command={command!r}\nstdout={result.stdout!r}\nstderr={result.stderr!r}"
        )


def run_native(toolchains: list[Toolchain], project: Path, build: Path) -> None:
    """Reuse the split project for strict O0/O2, C++17, and sanitizer proof."""

    for toolchain in toolchains:
        compile_and_run(toolchain, project, build, f"dynamic-{toolchain.family}-o0", ("-O0",))
        compile_and_run(toolchain, project, build, f"dynamic-{toolchain.family}-o2", ("-O2",))
        compile_cpp_header(toolchain, project, build)
        if toolchain.family == "clang":
            compile_and_run(
                toolchain,
                project,
                build,
                "dynamic-clang-sanitize",
                SANITIZER_FLAGS,
            )


def main() -> None:
    """Run the focused exact Dynamic acceptance lanes."""

    parser = argparse.ArgumentParser()
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    arguments = parser.parse_args()
    run_eval()
    toolchains = resolve_toolchains(arguments.toolchain)
    with tempfile.TemporaryDirectory(prefix="haxe-c-dynamic-lowering-") as temporary:
        root = Path(temporary)
        project = render_projects(root)
        build = root / "native"
        build.mkdir()
        run_native(toolchains, project, build)
    families = ",".join(toolchain.family for toolchain in toolchains)
    print(
        "dynamic-lowering: OK: Eval/native parity, forced collection, exact numeric "
        "adapters, typed isolation, scalar closure, layouts, cold/warm determinism, "
        "C++ headers, sanitizers, runtime-policy matrix, and unsupported-shape diagnostics "
        f"passed ({families})"
    )


if __name__ == "__main__":
    try:
        main()
    except (DynamicLoweringFailure, subprocess.TimeoutExpired) as error:
        print(f"dynamic-lowering: ERROR: {error}", file=sys.stderr)
        raise SystemExit(1) from error

#!/usr/bin/env python3
"""Verify HxcIR invariants, canonical dumps, cleanup order, and diagnostics."""

from __future__ import annotations

import difflib
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("hxc_ir.hxml")
ORACLE_HXML = Path(__file__).with_name("oracle.hxml")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_IR_REPORT="


class HxcIRFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def render(label: str) -> tuple[str, dict[str, object]]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [development_tool("haxe"), str(HXML)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        raise HxcIRFailure(
            f"{label} failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if len(lines) != 1 or result.stderr:
        raise HxcIRFailure(
            f"{label} emitted an invalid report envelope\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise HxcIRFailure(f"{label} report must be a JSON object")
    return payload, report


def check_oracle() -> None:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [development_tool("haxe"), str(ORACLE_HXML)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout != "nextIndex,produce:8\n" or result.stderr:
        raise HxcIRFailure(
            "Haxe side-effect oracle drifted\n"
            f"exit: {result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def difference(expected: str, actual: str, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def expected_text(name: str) -> str:
    return (EXPECTED / name).read_text(encoding="utf-8")


def check_dump(report: dict[str, object], key: str, name: str) -> str:
    actual = report.get(key)
    if not isinstance(actual, str):
        raise HxcIRFailure(f"report field {key!r} must be text")
    expected = expected_text(name)
    if actual != expected:
        raise HxcIRFailure(f"{key} dump drifted:\n" + difference(expected, actual, name))
    if str(ROOT) in actual or '@"/' in actual or re.search(r'@"[^"\n]*\\\\', actual):
        raise HxcIRFailure(f"{key} dump leaked an absolute/non-portable source path")
    return actual


def check_semantics(semantic: str, coverage: str) -> None:
    ordered_markers = (
        'instruction "i01.next-index"',
        'instruction "i02.slot-address"',
        'instruction "i03.current"',
        'instruction "i04.produce"',
        'instruction "i05.add"',
        'instruction "i06.store"',
    )
    positions = [semantic.find(marker) for marker in ordered_markers]
    if -1 in positions or positions != sorted(positions) or len(set(positions)) != len(positions):
        raise HxcIRFailure("semantic dump no longer makes the side-effect sequence explicit")

    cleanup = '["cleanup.inner"."release-token","cleanup.inner"."destroy-scratch","cleanup.outer"."destroy-array"]'
    if cleanup not in semantic:
        raise HxcIRFailure("semantic dump no longer exposes inner-to-outer reverse cleanup order")
    if "runtime(" in semantic or "hxrt" in semantic.lower() or "hxc_runtime" in semantic.lower():
        raise HxcIRFailure("runtime-free semantic fixture acquired an implicit runtime request")

    dispatches = (
        "dispatch=direct(",
        "dispatch=virtual(",
        "dispatch=interface(",
        "dispatch=closure(",
        "dispatch=native(",
        "dispatch=runtime(",
        "dispatch=intrinsic(",
    )
    missing = [marker for marker in dispatches if marker not in coverage]
    if missing:
        raise HxcIRFailure("coverage dump lost call classifications: " + ", ".join(missing))
    for explicit_runtime in ('runtime(feature="exception"', 'implementation=runtime("object")', 'implementation=runtime("gc")'):
        if explicit_runtime not in coverage:
            raise HxcIRFailure(f"coverage dump lost explicit runtime provenance: {explicit_runtime}")
    if 'result="value.string":string-utf8 constant value=string-utf8(bytes=11,value="line\\u0000é🙂")' not in coverage:
        raise HxcIRFailure("coverage dump lost validated UTF-8 string bytes or explicit embedded NUL")
    if 'runtime(feature="io",operation="sys-println-literal")' not in coverage:
        raise HxcIRFailure("coverage dump lost typed hosted-output runtime intent")
    if "kind=native-status,target=abort" not in coverage:
        raise HxcIRFailure("coverage dump lost hosted-output fail-stop policy")
    for failure_shape in (
        "kind=result-error,target=block(\"result-error\")",
        "kind=allocation-failure,target=propagate",
        "kind=native-status,target=propagate",
        "terminator throw value=\"value.thrown\" edge=failure(kind=exception,target=propagate",
    ):
        if failure_shape not in coverage:
            raise HxcIRFailure(f"coverage dump lost an explicit failure shape: {failure_shape}")
    if 'kind=box target=dynamic implementation=runtime("dynamic")' not in coverage:
        raise HxcIRFailure("coverage dump lost explicit boxing/runtime intent")
    primitive_shapes = (
        'type=abi-int(size)',
        'type=nullable(pointer,instance("instance.object"))',
        'kind=numeric-saturating target=i32 implementation=program-local("hxc.f64.to.i32.saturating") failure=none',
        'kind=numeric-round-binary32 target=f32 implementation=static failure=none',
        'kind=numeric-widen-binary64 target=f64 implementation=static failure=none',
        'kind=numeric-checked target=i8 implementation=program-local("hxc.i32.to.i8.checked") failure=failure(kind=result-error',
        'kind=nullable-inject target=nullable(tagged,i32) implementation=static failure=none',
        'kind=nullable-unwrap target=i32 implementation=static failure=failure(kind=result-error',
    )
    for primitive_shape in primitive_shapes:
        if primitive_shape not in coverage:
            raise HxcIRFailure(
                f"coverage dump lost an explicit primitive semantic shape: {primitive_shape}"
            )
    if 'branch condition="value.is-some" true=edge(target="success",arguments=["value.projected"]' not in coverage:
        raise HxcIRFailure("coverage dump lost typed cross-block argument flow")


def check_diagnostics(report: dict[str, object]) -> None:
    actual = report.get("diagnostics")
    expected = json.loads(expected_text("diagnostics.json"))
    if actual != expected:
        raise HxcIRFailure(
            "HxcIR diagnostic snapshot drifted\nexpected:\n"
            + json.dumps(expected, indent=2, sort_keys=True)
            + "\nactual:\n"
            + json.dumps(actual, indent=2, sort_keys=True)
        )
    serialized = json.dumps(actual, sort_keys=True)
    if "HXC1001" not in serialized or "HXC9000" not in serialized:
        raise HxcIRFailure("negative fixtures lost stable unsupported/internal diagnostic IDs")
    for diagnostic_key in (
        "uncheckedClassDereference",
        "unsafeClassUpcast",
        "mismatchedClassEquality",
        "primitiveRuntimeConversion",
        "invalidFloat32Narrow",
        "invalidFloat32Widen",
        "nullableUnwrapWithoutFailure",
        "switchCaseTypeMismatch",
    ):
        if diagnostic_key not in actual:
            raise HxcIRFailure(f"negative fixtures lost {diagnostic_key} validation")
    if str(ROOT) in serialized or re.search(r"\[profile=[^]]+\] (?:/|[A-Za-z]:\\)", serialized):
        raise HxcIRFailure("diagnostics leaked machine-local path spelling")


def main() -> int:
    if shutil.which(development_tool("haxe")) is None:
        print("hxc-ir: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        check_oracle()
        first_payload, first = render("first HxcIR render")
        second_payload, _ = render("second HxcIR render")
        if first_payload != second_payload:
            raise HxcIRFailure("two HxcIR reports were not byte-identical")
        semantic = check_dump(first, "semantic", "semantic.hxcir")
        coverage = check_dump(first, "coverage", "coverage.hxcir")
        check_semantics(semantic, coverage)
        check_diagnostics(first)
    except (HxcIRFailure, OSError, UnicodeError, json.JSONDecodeError, subprocess.TimeoutExpired) as error:
        print(f"hxc-ir: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "hxc-ir: OK: deterministic source-aware dumps, explicit side effects/cleanup, "
        "typed dispatch/runtime intent, Float32 conversions, switch validation, and stable negative diagnostics"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

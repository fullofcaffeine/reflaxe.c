#!/usr/bin/env python3
"""Verify typed primitive mappings, conversions, nullability, and strict C11 facts."""

from __future__ import annotations

import argparse
import difflib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("primitive_semantics.hxml")
EXPECTED = ROOT / "docs/specs/primitive-semantics.json"
SCHEMA = ROOT / "docs/specs/primitive-semantics.schema.json"
NATIVE_SOURCE = Path(__file__).with_name("native_contract.c")
CONTRACT_PREFIX = "HXC_PRIMITIVE_CONTRACT="
TYPE_PROBE_PREFIX = "HXC_PRIMITIVE_TYPE_PROBE="


class PrimitiveSemanticsFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class PrimitiveRender:
    contract_payload: str
    type_probe_payload: str
    contract: dict[str, object]
    type_probe: list[object]


@dataclass(frozen=True)
class PrimitiveToolchain:
    family: str
    compiler: str
    version: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def render(label: str) -> PrimitiveRender:
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
        raise PrimitiveSemanticsFailure(
            f"{label} failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if result.stderr:
        raise PrimitiveSemanticsFailure(
            f"{label} emitted unexpected stderr:\n{result.stderr}"
        )
    contract_payload = extract_line(result.stdout, CONTRACT_PREFIX, label)
    type_probe_payload = extract_line(result.stdout, TYPE_PROBE_PREFIX, label)
    contract_value = json.loads(contract_payload)
    type_probe_value = json.loads(type_probe_payload)
    if not isinstance(contract_value, dict) or not isinstance(type_probe_value, list):
        raise PrimitiveSemanticsFailure(f"{label} emitted invalid report shapes")
    return PrimitiveRender(
        contract_payload,
        type_probe_payload,
        contract_value,
        type_probe_value,
    )


def extract_line(output: str, prefix: str, label: str) -> str:
    lines = [line[len(prefix) :] for line in output.splitlines() if line.startswith(prefix)]
    if len(lines) != 1:
        raise PrimitiveSemanticsFailure(
            f"{label} emitted {len(lines)} records for {prefix!r}\nstdout:\n{output}"
        )
    return lines[0]


def object_record(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict) or not all(isinstance(key, str) for key in value):
        raise PrimitiveSemanticsFailure(f"{label} must be an object with text keys")
    return value


def object_list(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise PrimitiveSemanticsFailure(f"{label} must be an array")
    return value


def validate_contract(contract: dict[str, object]) -> None:
    if contract.get("schemaVersion") != 2:
        raise PrimitiveSemanticsFailure("primitive contract schema version drifted")
    if contract.get("algorithm") != "hxc-primitive-semantics-v2":
        raise PrimitiveSemanticsFailure("primitive contract algorithm drifted")
    if contract.get("status") != "production-primitive-arithmetic-slice":
        raise PrimitiveSemanticsFailure("primitive contract overstated production capability")
    if contract.get("requirements") != [
        "HXC-SEM-001",
        "HXC-SEM-002",
        "HXC-SEM-003",
        "HXC-SEM-005",
    ]:
        raise PrimitiveSemanticsFailure("primitive contract requirement ownership drifted")
    if contract.get("profiles") != ["portable", "metal"]:
        raise PrimitiveSemanticsFailure("primitive contract profile order or set drifted")

    expected_types = {
        "Void": "void",
        "Bool": "bool",
        "Int": "int32_t",
        "UInt": "uint32_t",
        "Float": "double",
        "c.Int8": "int8_t",
        "c.Int16": "int16_t",
        "c.Int32": "int32_t",
        "c.Int64": "int64_t",
        "c.UInt8": "uint8_t",
        "c.UInt16": "uint16_t",
        "c.UInt32": "uint32_t",
        "c.UInt64": "uint64_t",
        "c.Size": "size_t",
        "c.PtrDiff": "ptrdiff_t",
        "c.IntPtr": "intptr_t",
        "c.UIntPtr": "uintptr_t",
    }
    representations = object_list(contract.get("representations"), "representations")
    actual_types: dict[str, str] = {}
    for index, raw_record in enumerate(representations):
        record = object_record(raw_record, f"representations[{index}]")
        source_type = record.get("sourceType")
        c_type = record.get("cType")
        if not isinstance(source_type, str) or not isinstance(c_type, str):
            raise PrimitiveSemanticsFailure(f"representations[{index}] lacks typed names")
        if source_type in actual_types:
            raise PrimitiveSemanticsFailure(f"duplicate primitive representation {source_type}")
        actual_types[source_type] = c_type
        if record.get("profiles") != ["portable", "metal"] or record.get("profileDifference") != "none":
            raise PrimitiveSemanticsFailure(f"{source_type} changed representation by profile")
        if record.get("runtimeFeatures") != []:
            raise PrimitiveSemanticsFailure(f"{source_type} selected a hidden runtime feature")
        if "long" in c_type.split():
            raise PrimitiveSemanticsFailure(f"{source_type} leaked a host-dependent long spelling")
    if actual_types != expected_types:
        raise PrimitiveSemanticsFailure(
            f"primitive C representation table drifted\nexpected={expected_types!r}\nactual={actual_types!r}"
        )

    conversion_records = object_list(contract.get("conversions"), "conversions")
    conversions: dict[str, dict[str, object]] = {}
    for index, raw_record in enumerate(conversion_records):
        record = object_record(raw_record, f"conversions[{index}]")
        identifier = record.get("id")
        if not isinstance(identifier, str) or identifier in conversions:
            raise PrimitiveSemanticsFailure(f"invalid or duplicate conversion ID: {identifier!r}")
        conversions[identifier] = record
        if record.get("runtimeFeatures") != []:
            raise PrimitiveSemanticsFailure(f"conversion {identifier} selected hxrt")
        implementation = record.get("implementation")
        if not isinstance(implementation, str) or implementation.startswith("runtime:"):
            raise PrimitiveSemanticsFailure(f"conversion {identifier} bypassed compiler-first lowering")

    required_conversion_facts = {
        "haxe-int-to-float": ("exact", False),
        "haxe-uint-to-int-bits": ("low-32-bits-as-twos-complement", False),
        "haxe-std-int": ("truncate-toward-zero-with-defined-saturation", False),
        "exact-signed-narrow-wrapping": ("low-8-bits-as-twos-complement", False),
        "exact-checked-narrow": ("range-check-then-exact", True),
        "nullable-scalar-inject": ("construct-present-nullable", False),
        "nullable-scalar-unwrap": ("check-present-then-extract", True),
    }
    for identifier, (meaning, failure_required) in required_conversion_facts.items():
        record = conversions.get(identifier)
        if record is None or record.get("meaning") != meaning or record.get("failureEdgeRequired") is not failure_required:
            raise PrimitiveSemanticsFailure(f"conversion contract drifted: {identifier}")
    std_int_inputs = conversions["haxe-std-int"].get("exceptionalInputs")
    if std_int_inputs != [
        "NaN->0",
        "+infinity->2147483647",
        "-infinity->-2147483648",
        "positive-overflow->2147483647",
        "negative-overflow->-2147483648",
    ]:
        raise PrimitiveSemanticsFailure("Std.int exceptional conversion policy drifted")

    operation_records = object_list(contract.get("operations"), "operations")
    operations: dict[str, dict[str, object]] = {}
    for index, raw_record in enumerate(operation_records):
        record = object_record(raw_record, f"operations[{index}]")
        identifier = record.get("id")
        if not isinstance(identifier, str) or identifier in operations:
            raise PrimitiveSemanticsFailure(f"invalid or duplicate operation ID: {identifier!r}")
        operations[identifier] = record
        if record.get("runtimeFeatures") != []:
            raise PrimitiveSemanticsFailure(f"operation {identifier} selected hxrt")
        implementation = record.get("implementation")
        if not isinstance(implementation, str) or implementation.startswith("runtime:"):
            raise PrimitiveSemanticsFailure(f"operation {identifier} bypassed compiler-first lowering")
        if not object_list(record.get("edgeCases"), f"operation {identifier} edgeCases"):
            raise PrimitiveSemanticsFailure(f"operation {identifier} omitted its boundary contract")

    required_operation_facts = {
        "int-add": ("haxe.i32.add", "program-local:hxc.i32.add.wrapping"),
        "int-divide": ("haxe.f64.divide", "program-local:hxc.f64.divide.zero-safe"),
        "int-modulo": ("haxe.i32.modulo", "program-local:hxc.i32.modulo.zero-safe"),
        "int-shift-right": ("haxe.i32.shift-right.masked", "program-local:hxc.i32.shift-right.masked"),
        "int-bit-xor": ("haxe.i32.bit-xor", "program-local:hxc.i32.bit-xor"),
        "uint-add": ("haxe.u32.add", "direct-c"),
        "uint-shift-left": ("haxe.u32.shift-left.masked", "direct-c"),
        "float-divide": ("haxe.f64.divide", "program-local:hxc.f64.divide.zero-safe"),
        "float-modulo": ("haxe.f64.modulo", "program-local:hxc.f64.modulo"),
    }
    for identifier, (operation_id, implementation) in required_operation_facts.items():
        record = operations.get(identifier)
        if (
            record is None
            or record.get("operationId") != operation_id
            or record.get("implementation") != implementation
        ):
            raise PrimitiveSemanticsFailure(f"operation contract drifted: {identifier}")

    if "zero divisor returns 0" not in " ".join(
        str(value) for value in object_list(operations["int-modulo"].get("edgeCases"), "int-modulo edge cases")
    ):
        raise PrimitiveSemanticsFailure("Int modulo-by-zero refinement drifted")
    if "masked with 31" not in " ".join(
        str(value) for value in object_list(operations["int-shift-right"].get("edgeCases"), "int-shift edge cases")
    ):
        raise PrimitiveSemanticsFailure("Int shift masking contract drifted")

    nullability = object_list(contract.get("nullability"), "nullability")
    patterns = {
        object_record(record, "nullability record").get("sourcePattern")
        for record in nullability
    }
    if patterns != {"Null<non-null scalar>", "Null<reference-like T>", "c.NullablePtr<T>"}:
        raise PrimitiveSemanticsFailure("nullable representation rules are incomplete")
    for raw_record in nullability:
        record = object_record(raw_record, "nullability record")
        if record.get("profiles") != ["portable", "metal"] or record.get("runtimeFeatures") != []:
            raise PrimitiveSemanticsFailure("nullability changed by profile or selected hxrt")

    float_contract = object_record(contract.get("floatContract"), "floatContract")
    if float_contract.get("cType") != "double" or "binary64" not in str(float_contract.get("format")):
        raise PrimitiveSemanticsFailure("Float no longer has its exact binary64 contract")
    if "forbidden" not in str(float_contract.get("fastMath")):
        raise PrimitiveSemanticsFailure("Float contract no longer rejects unsafe fast-math")

    serialized = json.dumps(contract, ensure_ascii=False, sort_keys=True)
    if str(ROOT) in serialized or "/Users/" in serialized or "\\" in serialized:
        raise PrimitiveSemanticsFailure("primitive contract leaked a host path")
    if "hxrt" in serialized.lower() or "hxc_runtime" in serialized.lower():
        raise PrimitiveSemanticsFailure("primitive contract introduced a runtime dependency")


def validate_type_probe(type_probe: list[object]) -> None:
    records: dict[str, dict[str, object]] = {}
    for index, raw_record in enumerate(type_probe):
        record = object_record(raw_record, f"typeProbe[{index}]")
        field = record.get("field")
        if not isinstance(field, str) or field in records:
            raise PrimitiveSemanticsFailure(f"invalid or duplicate typed field: {field!r}")
        records[field] = record
        if record.get("profileInvariant") is not True:
            raise PrimitiveSemanticsFailure(f"typed mapping changed by profile: {field}")
        if record.get("kind") == "unsupported":
            raise PrimitiveSemanticsFailure(f"fixture primitive was not recognized: {field}")

    expected = {
        "boolValue": ("primitive", "Bool", False, "direct-scalar", "bool"),
        "intValue": ("primitive", "Int", False, "direct-scalar", "int32_t"),
        "uintValue": ("primitive", "UInt", False, "direct-scalar", "uint32_t"),
        "floatValue": ("primitive", "Float", False, "direct-scalar", "double"),
        "nullableInt": ("primitive", "Int", True, "tagged-optional", "int32_t"),
        "nullableFloat": ("primitive", "Float", True, "tagged-optional", "double"),
        "pointerValue": ("native-pointer", None, False, "pointer", None),
        "nullablePointer": ("native-pointer", None, True, "pointer", None),
        "nullWrappedPointer": ("native-pointer", None, True, "pointer", None),
        "referenceValue": ("reference", None, False, "pointer", None),
        "nullableReference": ("reference", None, True, "pointer", None),
        "returnsVoid": ("primitive", "Void", False, "no-value", "void"),
    }
    for field, values in expected.items():
        record = records.get(field)
        actual = None if record is None else (
            record.get("kind"),
            record.get("sourceType"),
            record.get("nullable"),
            record.get("storage"),
            record.get("cType"),
        )
        if actual != values:
            raise PrimitiveSemanticsFailure(
                f"real typed-AST mapping drifted for {field}: expected {values!r}, got {actual!r}"
            )
    required_fields = {
        "int8Value", "int16Value", "int32Value", "int64Value",
        "uint8Value", "uint16Value", "uint32Value", "uint64Value",
        "sizeValue", "ptrDiffValue", "intPtrValue", "uintPtrValue",
        "nullableBool", "nullableUInt", "nullableInt8",
    }
    if not required_fields.issubset(records):
        raise PrimitiveSemanticsFailure(
            f"typed primitive probe omitted fields: {sorted(required_fields - records.keys())!r}"
        )


def validate_schema_document() -> None:
    value = json.loads(SCHEMA.read_text(encoding="utf-8"))
    schema = object_record(value, "primitive schema")
    if schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        raise PrimitiveSemanticsFailure("primitive schema must use JSON Schema 2020-12")
    properties = object_record(schema.get("properties"), "primitive schema properties")
    for required in ("representations", "nullability", "conversions", "operations", "floatContract", "constraints"):
        if required not in properties:
            raise PrimitiveSemanticsFailure(f"primitive schema omitted {required}")


def check_snapshot(contract: dict[str, object]) -> None:
    if not EXPECTED.is_file():
        raise PrimitiveSemanticsFailure(
            "primitive contract snapshot is missing; use npm run snapshots:update -- --suite primitive-semantics"
        )
    expected_value = json.loads(EXPECTED.read_text(encoding="utf-8"))
    if contract != expected_value:
        diff = "".join(
            difflib.unified_diff(
                (json.dumps(expected_value, ensure_ascii=False, indent=2, sort_keys=True) + "\n").splitlines(keepends=True),
                (json.dumps(contract, ensure_ascii=False, indent=2, sort_keys=True) + "\n").splitlines(keepends=True),
                fromfile=str(EXPECTED.relative_to(ROOT)),
                tofile="actual primitive contract",
            )
        )
        raise PrimitiveSemanticsFailure("primitive contract snapshot drifted\n" + diff)


def compiler_identity(executable: str) -> tuple[str, str]:
    result = subprocess.run(
        [executable, "--version"],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        raise PrimitiveSemanticsFailure(
            f"cannot identify primitive compiler {executable}: exit {result.returncode}"
        )
    combined = (result.stdout + result.stderr).strip()
    first_line = combined.splitlines()[0] if combined else "unknown version"
    lowered = combined.lower()
    if "clang" in lowered:
        return "clang", first_line
    if "free software foundation" in lowered or "gcc" in lowered:
        return "gcc", first_line
    return "unknown", first_line


def resolve_native_toolchain(family: str, *, required: bool) -> PrimitiveToolchain | None:
    executable = shutil.which(family)
    qualifier = "required" if required else "optional"
    if executable is None:
        if required:
            raise PrimitiveSemanticsFailure(
                f"required primitive toolchain {family}: missing {family}"
            )
        print(f"primitive-semantics: SKIP {qualifier} {family}: missing {family}")
        return None
    actual_family, version = compiler_identity(executable)
    if actual_family != family:
        message = (
            f"{family}: command identity mismatch "
            f"({family}={actual_family})"
        )
        if required:
            raise PrimitiveSemanticsFailure(
                f"required primitive toolchain {message}"
            )
        print(f"primitive-semantics: SKIP {qualifier} {message}")
        return None
    print(
        f"primitive-semantics: {'REQUIRED' if required else 'AVAILABLE'} "
        f"{family}: cc={version}"
    )
    return PrimitiveToolchain(family, executable, version)


def native_toolchains(selected: str | None) -> list[PrimitiveToolchain]:
    requested = [selected] if selected is not None else ["gcc", "clang"]
    available = [
        toolchain
        for family in requested
        if (
            toolchain := resolve_native_toolchain(
                family, required=selected is not None
            )
        )
        is not None
    ]
    if not available:
        raise PrimitiveSemanticsFailure("no strict C11 compiler was available for primitive native evidence")
    return available


def run_native(selected: str | None = None) -> None:
    flags = [
        "-std=c11",
        "-pedantic-errors",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-Wconversion",
        "-Wsign-conversion",
        "-fno-fast-math",
    ]
    with tempfile.TemporaryDirectory(prefix="hxc-primitive-semantics-") as temporary:
        output_root = Path(temporary)
        for toolchain in native_toolchains(selected):
            for optimization in ("-O0", "-O2"):
                executable = output_root / f"{toolchain.family}-{optimization[1:]}"
                compile_result = subprocess.run(
                    [toolchain.compiler, *flags, optimization, str(NATIVE_SOURCE), "-o", str(executable)],
                    cwd=ROOT,
                    check=False,
                    capture_output=True,
                    text=True,
                    timeout=30,
                )
                if compile_result.returncode != 0:
                    raise PrimitiveSemanticsFailure(
                        f"{toolchain.family} {optimization} primitive native compile failed\n"
                        f"stdout:\n{compile_result.stdout}\nstderr:\n{compile_result.stderr}"
                    )
                run_result = subprocess.run(
                    [str(executable)],
                    cwd=ROOT,
                    check=False,
                    capture_output=True,
                    text=True,
                    timeout=30,
                )
                if (
                    run_result.returncode != 0
                    or run_result.stdout != "primitive-semantics-native: OK\n"
                    or run_result.stderr
                ):
                    raise PrimitiveSemanticsFailure(
                        f"{toolchain.family} {optimization} primitive native run failed\n"
                        f"exit: {run_result.returncode}\n"
                        f"stdout:\n{run_result.stdout}\nstderr:\n{run_result.stderr}"
                    )


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("gcc", "clang"))
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    try:
        if args.native_only:
            run_native(args.toolchain)
        else:
            validate_schema_document()
            first = render("first primitive semantic render")
            second = render("second primitive semantic render")
            if (
                first.contract_payload != second.contract_payload
                or first.type_probe_payload != second.type_probe_payload
            ):
                raise PrimitiveSemanticsFailure("two primitive semantic renders were not byte-identical")
            validate_contract(first.contract)
            validate_type_probe(first.type_probe)
            check_snapshot(first.contract)
            run_native(args.toolchain)
    except (
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
        PrimitiveSemanticsFailure,
    ) as error:
        print(f"primitive-semantics: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "primitive-semantics: OK: typed Haxe mappings, profile-invariant fixed/ABI scalars, "
        "defined conversions/nullability, zero-runtime policy, and strict C11 O0/O2 probes passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

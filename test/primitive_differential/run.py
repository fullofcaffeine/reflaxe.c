#!/usr/bin/env python3
"""Run the seeded generated-Haxe primitive differential and sanitizer corpus."""

from __future__ import annotations

import argparse
import copy
import difflib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Iterable, Mapping, Sequence
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[2]
EXPECTED = Path(__file__).with_name("expected")
SEED_PATH = Path(__file__).with_name("seed.json")
MINIMIZER_REGRESSION = Path(__file__).with_name("regressions") / "minimizer-regression.json"
LEDGER_PATH = ROOT / "docs/specs/primitive-divergences.json"
LEDGER_SCHEMA_PATH = ROOT / "docs/specs/primitive-divergences.schema.json"
TOOLCHAIN_LOCK = ROOT / "docs/specs/toolchain-lock.json"

sys.path.insert(0, str(ROOT / "scripts/test"))
from c_fixture_harness import (  # noqa: E402
    C11_STRICT_FLAGS,
    CFixtureFailure,
    CFixtureProject,
    CToolchain,
    resolve_toolchains,
    run_c_fixture_corpus,
    validate_report,
)


MASK64 = (1 << 64) - 1
I32_MIN = -(1 << 31)
I32_MAX = (1 << 31) - 1
U32_MAX = (1 << 32) - 1
SANITIZER_FLAGS = (
    "-fsanitize=address,undefined",
    "-fno-sanitize-recover=all",
    "-fno-omit-frame-pointer",
)
ORACLE_EXACT = "exact"
ORACLE_HOST_DEPENDENT_INT32 = "host-dependent-int32"
ORACLE_HOST_DEPENDENT_INT32_CANONICAL = "<host-dependent-int32>"
SNAPSHOT_FORMATS = {
    "corpus.json": "json",
    "PrimitiveDifferentialFixture.hx": "text",
    "oracle.txt": "text",
    "divergence-oracle.txt": "text",
    "program.h": "header",
    "program.c": "c",
    "symbols.json": "json",
    "runtime-plan.json": "json",
}


class PrimitiveDifferentialFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Operation:
    name: str
    arguments: tuple[str, ...]
    result_type: str
    haxe_body: str


OPERATIONS = (
    Operation("iadd", ("i32", "i32"), "i32", "left + right"),
    Operation("iand", ("i32", "i32"), "i32", "left & right"),
    Operation("idivInt", ("i32", "i32"), "i32", "Std.int(left / right)"),
    Operation("ieq", ("i32", "i32"), "bool", "left == right"),
    Operation("ilt", ("i32", "i32"), "bool", "left < right"),
    Operation("imod", ("i32", "i32"), "i32", "left % right"),
    Operation("imul", ("i32", "i32"), "i32", "left * right"),
    Operation("inot", ("i32",), "i32", "~value"),
    Operation("ior", ("i32", "i32"), "i32", "left | right"),
    Operation("ishl", ("i32", "i32"), "i32", "left << right"),
    Operation("ishr", ("i32", "i32"), "i32", "left >> right"),
    Operation("isub", ("i32", "i32"), "i32", "left - right"),
    Operation("iushr", ("i32", "i32"), "i32", "left >>> right"),
    Operation("ixor", ("i32", "i32"), "i32", "left ^ right"),
    Operation("faddInt", ("f64", "f64"), "i32", "Std.int(left + right)"),
    Operation("fdivInt", ("f64", "f64"), "i32", "Std.int(left / right)"),
    Operation("feq", ("f64", "f64"), "bool", "left == right"),
    Operation("flt", ("f64", "f64"), "bool", "left < right"),
    Operation("fmodInt", ("f64", "f64"), "i32", "Std.int(left % right)"),
    Operation("fmulInt", ("f64", "f64"), "i32", "Std.int(left * right)"),
    Operation("fsubInt", ("f64", "f64"), "i32", "Std.int(left - right)"),
    Operation("uadd", ("u32", "u32"), "u32", "left + right"),
    Operation("ult", ("u32", "u32"), "bool", "left < right"),
    Operation("umod", ("u32", "u32"), "u32", "left % right"),
    Operation("umul", ("u32", "u32"), "u32", "left * right"),
    Operation("ushl", ("u32", "i32"), "u32", "left << right"),
    Operation("ushr", ("u32", "i32"), "u32", "left >> right"),
    Operation("usub", ("u32", "u32"), "u32", "left - right"),
)
OPERATION_BY_NAME = {operation.name: operation for operation in OPERATIONS}


@dataclass(frozen=True)
class Argument:
    type: str
    value: int | str

    def json_value(self) -> dict[str, object]:
        return {"type": self.type, "value": self.value}


@dataclass(frozen=True)
class Case:
    identifier: str
    operation: str
    result_type: str
    arguments: tuple[Argument, ...]

    def json_value(self) -> dict[str, object]:
        return {
            "id": self.identifier,
            "operation": self.operation,
            "resultType": self.result_type,
            "arguments": [argument.json_value() for argument in self.arguments],
        }


@dataclass(frozen=True)
class Divergence:
    identifier: str
    operation: str
    haxe_call: str
    target_haxe_call: str
    c_call_arguments: tuple[str, ...]
    oracle_kind: str
    result_type: str = "i32"


DIVERGENCES = (
    Divergence(
        "div-int-modulo-zero",
        "imod",
        "imod(7, 0)",
        "imod(7, 0)",
        ("INT32_C(7)", "INT32_C(0)"),
        ORACLE_EXACT,
    ),
    Divergence(
        "div-std-int-positive-infinity",
        "fint",
        "fint(Math.POSITIVE_INFINITY)",
        "fint(0.0)",
        ("INFINITY",),
        ORACLE_HOST_DEPENDENT_INT32,
    ),
    Divergence(
        "div-std-int-positive-overflow",
        "fint",
        "fint(2147483648.0)",
        "fint(2147483648.0)",
        ("2147483648.0",),
        ORACLE_EXACT,
    ),
)


class SplitMix64:
    def __init__(self, seed: int) -> None:
        self.state = seed & MASK64

    def next(self) -> int:
        self.state = (self.state + 0x9E3779B97F4A7C15) & MASK64
        value = self.state
        value = ((value ^ (value >> 30)) * 0xBF58476D1CE4E5B9) & MASK64
        value = ((value ^ (value >> 27)) * 0x94D049BB133111EB) & MASK64
        return (value ^ (value >> 31)) & MASK64

    def bounded(self, upper: int) -> int:
        if upper <= 0:
            raise PrimitiveDifferentialFailure("SplitMix64 bound must be positive")
        return self.next() % upper


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def json_object(path: Path) -> dict[str, object]:
    try:
        value: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise PrimitiveDifferentialFailure(f"cannot read {path.relative_to(ROOT)}: {error}") from error
    if not isinstance(value, dict):
        raise PrimitiveDifferentialFailure(f"{path.relative_to(ROOT)} must contain an object")
    return value


def normalized_repository_path(value: str) -> bool:
    if not value or "\\" in value:
        return False
    path = PurePosixPath(value)
    return not path.is_absolute() and all(part not in ("", ".", "..") for part in path.parts)


def load_seed() -> tuple[int, int, int, str]:
    document = json_object(SEED_PATH)
    if set(document) != {
        "schemaVersion",
        "generator",
        "seed",
        "caseCount",
        "minimumCasesPerOperation",
    }:
        raise PrimitiveDifferentialFailure("seed manifest fields drifted")
    if document.get("schemaVersion") != 1 or document.get("generator") != "splitmix64-v1":
        raise PrimitiveDifferentialFailure("seed manifest identity drifted")
    seed_text = document.get("seed")
    case_count = document.get("caseCount")
    minimum = document.get("minimumCasesPerOperation")
    if (
        not isinstance(seed_text, str)
        or not seed_text.startswith("0x")
        or not isinstance(case_count, int)
        or isinstance(case_count, bool)
        or not isinstance(minimum, int)
        or isinstance(minimum, bool)
        or case_count < len(OPERATIONS) * minimum
        or minimum < 1
    ):
        raise PrimitiveDifferentialFailure("seed manifest bounds are invalid")
    try:
        seed = int(seed_text, 16)
    except ValueError as error:
        raise PrimitiveDifferentialFailure("seed manifest contains an invalid hexadecimal seed") from error
    if seed < 0 or seed > MASK64:
        raise PrimitiveDifferentialFailure("seed is outside the unsigned 64-bit range")
    return seed, case_count, minimum, seed_text.lower()


I32_BOUNDARIES = (
    I32_MIN,
    I32_MIN + 1,
    -65536,
    -33,
    -32,
    -31,
    -2,
    -1,
    0,
    1,
    2,
    31,
    32,
    33,
    65535,
    I32_MAX - 1,
    I32_MAX,
)
U32_BOUNDARIES = (
    0,
    1,
    2,
    31,
    32,
    33,
    (1 << 16) - 1,
    1 << 16,
    (1 << 31) - 1,
    1 << 31,
    U32_MAX - 1,
    U32_MAX,
)


def random_i32(generator: SplitMix64) -> int:
    if generator.bounded(3) != 0:
        return I32_BOUNDARIES[generator.bounded(len(I32_BOUNDARIES))]
    bits = generator.next() & U32_MAX
    return bits if bits <= I32_MAX else bits - (1 << 32)


def random_u32(generator: SplitMix64) -> int:
    if generator.bounded(3) != 0:
        return U32_BOUNDARIES[generator.bounded(len(U32_BOUNDARIES))]
    return generator.next() & U32_MAX


def quarter_text(units: int) -> str:
    sign = "-" if units < 0 else ""
    absolute = abs(units)
    whole, quarter = divmod(absolute, 4)
    suffix = (".0", ".25", ".5", ".75")[quarter]
    return f"{sign}{whole}{suffix}"


def random_f64(generator: SplitMix64) -> str:
    boundaries = (
        "-8192.0",
        "-1024.25",
        "-1.0",
        "-0.0",
        "0.0",
        "0.25",
        "1.0",
        "1024.5",
        "8191.75",
    )
    if generator.bounded(3) != 0:
        return boundaries[generator.bounded(len(boundaries))]
    return quarter_text(generator.bounded(65535) - 32767)


def arguments_for(operation: Operation, generator: SplitMix64) -> tuple[Argument, ...]:
    arguments: list[Argument] = []
    for type_name in operation.arguments:
        if type_name == "i32":
            arguments.append(Argument(type_name, random_i32(generator)))
        elif type_name == "u32":
            arguments.append(Argument(type_name, random_u32(generator)))
        elif type_name == "f64":
            arguments.append(Argument(type_name, random_f64(generator)))
        else:
            raise PrimitiveDifferentialFailure(f"unknown generator argument type {type_name}")

    if operation.name in ("idivInt", "imod"):
        right = arguments[1]
        while right.value == 0 or (
            operation.name == "idivInt"
            and arguments[0].value == I32_MIN
            and right.value == -1
        ):
            right = Argument("i32", random_i32(generator))
        arguments[1] = right
    elif operation.name == "umod":
        right = arguments[1]
        while right.value == 0:
            right = Argument("u32", random_u32(generator))
        arguments[1] = right
    elif operation.name in ("fdivInt", "fmodInt"):
        right = arguments[1]
        while right.value in ("0.0", "-0.0"):
            right = Argument("f64", random_f64(generator))
        arguments[1] = right
    return tuple(arguments)


def generate_cases() -> tuple[dict[str, object], tuple[Case, ...]]:
    seed, case_count, minimum, seed_text = load_seed()
    generator = SplitMix64(seed)
    schedule: list[Operation] = [
        operation for operation in OPERATIONS for _ in range(minimum)
    ]
    while len(schedule) < case_count:
        schedule.append(OPERATIONS[generator.bounded(len(OPERATIONS))])
    for index in range(len(schedule) - 1, 0, -1):
        other = generator.bounded(index + 1)
        schedule[index], schedule[other] = schedule[other], schedule[index]

    cases = tuple(
        Case(
            f"case-{index:03d}",
            operation.name,
            operation.result_type,
            arguments_for(operation, generator),
        )
        for index, operation in enumerate(schedule)
    )
    counts = {operation.name: 0 for operation in OPERATIONS}
    for case in cases:
        counts[case.operation] += 1
    if any(value < minimum for value in counts.values()):
        raise PrimitiveDifferentialFailure("generated corpus lost minimum operation coverage")
    corpus = {
        "schemaVersion": 1,
        "generator": "splitmix64-v1",
        "seed": seed_text,
        "caseCount": len(cases),
        "minimumCasesPerOperation": minimum,
        "oracle": {
            "kind": "haxe-eval",
            "version": "5.0.0-preview.1",
            "authority": "docs/specs/toolchain-lock.json",
        },
        "operationCounts": counts,
        "cases": [case.json_value() for case in cases],
    }
    return corpus, cases


def haxe_type(type_name: str) -> str:
    return {"i32": "Int", "u32": "UInt", "f64": "Float", "bool": "Bool"}[type_name]


def haxe_i32(value: int) -> str:
    return "(-2147483647 - 1)" if value == I32_MIN else str(value)


def haxe_argument(argument: Argument) -> str:
    if argument.type == "i32" and isinstance(argument.value, int):
        return haxe_i32(argument.value)
    if argument.type == "u32" and isinstance(argument.value, int):
        signed = argument.value if argument.value <= I32_MAX else argument.value - (1 << 32)
        return f"({haxe_i32(signed)} : UInt)"
    if argument.type == "f64" and isinstance(argument.value, str):
        return argument.value
    raise PrimitiveDifferentialFailure(f"invalid Haxe argument {argument!r}")


def operation_parameters(operation: Operation) -> str:
    names = ("value",) if len(operation.arguments) == 1 else ("left", "right")
    return ", ".join(
        f"{name}:{haxe_type(type_name)}"
        for name, type_name in zip(names, operation.arguments, strict=True)
    )


def haxe_call(case: Case) -> str:
    arguments = ", ".join(haxe_argument(argument) for argument in case.arguments)
    return f"{case.operation}({arguments})"


def render_fixture(cases: Sequence[Case]) -> str:
    lines = ["class PrimitiveDifferentialFixture {"]
    for operation in OPERATIONS:
        lines.extend(
            (
                f"\tstatic function {operation.name}({operation_parameters(operation)}):{haxe_type(operation.result_type)}",
                f"\t\treturn {operation.haxe_body};",
                "",
            )
        )
    lines.extend(
        (
            "\tstatic function fint(value:Float):Int",
            "\t\treturn Std.int(value);",
            "",
            "\tstatic function main():Void {",
            "\t\t#if primitive_differential_oracle",
        )
    )
    for case in cases:
        lines.append(
            f'\t\tSys.println("{case.identifier}:{case.result_type}:" + {haxe_call(case)});'
        )
    for divergence in DIVERGENCES:
        lines.append(
            f'\t\tSys.println("{divergence.identifier}:{divergence.result_type}:" + {divergence.haxe_call});'
        )
    lines.append("\t\t#else")
    for case in cases:
        lines.append(f"\t\t{haxe_call(case)};")
    for divergence in DIVERGENCES:
        lines.append(f"\t\t{divergence.target_haxe_call};")
    lines.extend(("\t\t#end", "\t}", "}", ""))
    return "\n".join(lines)


def run_haxe(command: list[str], *, label: str, timeout: int = 120) -> subprocess.CompletedProcess[str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=timeout,
    )
    if result.returncode != 0 or result.stderr:
        raise PrimitiveDifferentialFailure(
            f"{label} failed\nexit={result.returncode}\nstdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    return result


def render_oracle(source: str) -> tuple[str, str]:
    with tempfile.TemporaryDirectory(prefix="hxc-primitive-oracle-") as temporary:
        fixture_root = Path(temporary)
        (fixture_root / "PrimitiveDifferentialFixture.hx").write_text(
            source, encoding="utf-8", newline="\n"
        )
        result = run_haxe(
            [
                development_tool("haxe"),
                "-cp",
                str(fixture_root),
                "-D",
                "primitive_differential_oracle",
                "-main",
                "PrimitiveDifferentialFixture",
                "--interp",
            ],
            label="pinned Eval primitive oracle",
        )
    common: list[str] = []
    divergences: list[str] = []
    for line in result.stdout.splitlines():
        if line.startswith("case-"):
            common.append(line)
        elif line.startswith("div-"):
            divergences.append(line)
        else:
            raise PrimitiveDifferentialFailure(f"oracle emitted an unknown trace line: {line!r}")
    return "\n".join(common) + "\n", "\n".join(divergences) + "\n"


def generated_tree(root: Path) -> dict[str, bytes]:
    return {
        path.relative_to(root).as_posix(): path.read_bytes()
        for path in sorted(root.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def compile_production(source: str, output: Path, fixture_root: Path) -> None:
    fixture_root.mkdir(parents=True, exist_ok=True)
    (fixture_root / "PrimitiveDifferentialFixture.hx").write_text(
        source, encoding="utf-8", newline="\n"
    )
    run_haxe(
        [
            development_tool("haxe"),
            "-cp",
            str(fixture_root),
            "-lib",
            "reflaxe.c",
            "-main",
            "PrimitiveDifferentialFixture",
            "-D",
            "hxc_project_layout=unity",
            "--custom-target",
            f"c={output}",
        ],
        label="production primitive differential compile",
    )


def validate_production(output: Path) -> dict[str, object]:
    required = (
        "include/hxc/program.h",
        "src/program.c",
        "hxc.symbols.json",
        "hxc.runtime-plan.json",
        "hxc.manifest.json",
    )
    missing = [relative for relative in required if not (output / relative).is_file()]
    if missing:
        raise PrimitiveDifferentialFailure(f"production output omitted {missing!r}")
    runtime_plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    proof = runtime_plan.get("noRuntimeProof") if isinstance(runtime_plan, dict) else None
    reachability = proof.get("reachability") if isinstance(proof, dict) else None
    runtime_absence = proof.get("runtimeAbsence") if isinstance(proof, dict) else None
    if (
        not isinstance(runtime_plan, dict)
        or runtime_plan.get("schemaVersion") != 2
        or runtime_plan.get("algorithm") != "hxc-runtime-plan-v2"
        or runtime_plan.get("status") != "analyzed-runtime-free"
        or runtime_plan.get("features") != []
        or not isinstance(proof, dict)
        or proof.get("algorithm") != "hxc-no-runtime-eligibility-v1"
        or proof.get("status") != "eligible"
        or proof.get("scope") != "reachable-whole-program"
        or proof.get("directDecisions") != runtime_plan.get("directDecisions")
        or not proof.get("programLocalHelpers")
        or not isinstance(reachability, dict)
        or reachability.get("runtimeIntents") != 0
        or runtime_absence
        != {
            "features": [],
            "includes": [],
            "sources": [],
            "defines": [],
            "libraries": [],
            "symbols": [],
        }
        or "ub-safe-primitive-operations" not in runtime_plan.get("directDecisions", [])
        or "selected-program-local-helpers" not in runtime_plan.get("directDecisions", [])
    ):
        raise PrimitiveDifferentialFailure("generated corpus lost its zero-runtime proof")
    manifest = json.loads((output / "hxc.manifest.json").read_text(encoding="utf-8"))
    build = manifest.get("build", {}) if isinstance(manifest, dict) else {}
    libraries = build.get("libraries") if isinstance(build, dict) else None
    if libraries != [{"name": "m", "ownerModulePaths": ["PrimitiveDifferentialFixture"]}]:
        raise PrimitiveDifferentialFailure("generated corpus lost its exact math link fact")
    if not isinstance(build, dict) or build.get("runtimeHeaders") != [] or build.get("includeDirectories") != ["include"]:
        raise PrimitiveDifferentialFailure("generated corpus retained an hxrt build input")
    tree = generated_tree(output)
    if any(path.startswith("runtime/") for path in tree):
        raise PrimitiveDifferentialFailure("primitive differential corpus emitted an hxrt file")
    joined = b"\n".join(contents for path, contents in tree.items() if path.endswith((".c", ".h")))
    if b"hxrt" in joined.lower():
        raise PrimitiveDifferentialFailure("primitive differential corpus selected hxrt")
    for unstable in (str(ROOT).encode(), str(output).encode()):
        if unstable in joined:
            raise PrimitiveDifferentialFailure("generated corpus serialized an absolute path")
    return runtime_plan


def production_snapshot(source: str) -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-primitive-production-a-") as first_temporary, tempfile.TemporaryDirectory(
        prefix="hxc-primitive-production-b-"
    ) as second_temporary:
        first_root = Path(first_temporary)
        second_root = Path(second_temporary)
        first_output = first_root / "out"
        second_output = second_root / "different-out"
        compile_production(source, first_output, first_root / "fixture")
        compile_production(source, second_output, second_root / "other-fixture")
        first_plan = validate_production(first_output)
        validate_production(second_output)
        if generated_tree(first_output) != generated_tree(second_output):
            raise PrimitiveDifferentialFailure("generated corpus changed across unrelated output roots")
        return {
            "program.h": (first_output / "include/hxc/program.h").read_text(encoding="utf-8"),
            "program.c": (first_output / "src/program.c").read_text(encoding="utf-8"),
            "symbols.json": json.loads((first_output / "hxc.symbols.json").read_text(encoding="utf-8")),
            "runtime-plan.json": first_plan,
        }


def validate_schema_authority() -> None:
    schema = json_object(LEDGER_SCHEMA_PATH)
    if (
        schema.get("$id") != "https://reflaxe-c.dev/schemas/primitive-divergences.schema.json"
        or schema.get("type") != "object"
        or schema.get("additionalProperties") is not False
    ):
        raise PrimitiveDifferentialFailure("primitive divergence schema identity drifted")
    definitions = schema.get("$defs")
    entry = definitions.get("entry") if isinstance(definitions, dict) else None
    if not isinstance(entry, dict) or entry.get("additionalProperties") is not False:
        raise PrimitiveDifferentialFailure("primitive divergence schema must close every entry")
    expectation = definitions.get("oracleExpectation") if isinstance(definitions, dict) else None
    variants = expectation.get("oneOf") if isinstance(expectation, dict) else None
    if not isinstance(variants, list) or len(variants) != 2:
        raise PrimitiveDifferentialFailure(
            "primitive divergence schema must close the two oracle expectation variants"
        )


def validate_ledger_document(document: Mapping[str, object]) -> dict[str, dict[str, str]]:
    required_top = {"$schema", "schemaVersion", "ledgerId", "oracle", "entries"}
    if set(document) != required_top:
        raise PrimitiveDifferentialFailure("primitive divergence ledger fields drifted")
    if (
        document.get("$schema") != "primitive-divergences.schema.json"
        or document.get("schemaVersion") != 1
        or document.get("ledgerId") != "hxc-primitive-divergences-v1"
        or document.get("oracle")
        != {
            "kind": "haxe-eval",
            "version": "5.0.0-preview.1",
            "authority": "docs/specs/toolchain-lock.json",
        }
    ):
        raise PrimitiveDifferentialFailure("primitive divergence ledger identity drifted")
    entries = document.get("entries")
    if not isinstance(entries, list):
        raise PrimitiveDifferentialFailure("primitive divergence entries must be an array")
    expected_ids = [divergence.identifier for divergence in DIVERGENCES]
    actual_ids: list[str] = []
    result: dict[str, dict[str, str]] = {}
    required_entry = {
        "id",
        "status",
        "ownerBeads",
        "operation",
        "input",
        "oracleExpectation",
        "targetValue",
        "contract",
        "rationale",
        "tests",
    }
    for index, raw in enumerate(entries):
        if not isinstance(raw, dict) or set(raw) != required_entry:
            raise PrimitiveDifferentialFailure(f"primitive divergence entry {index} fields drifted")
        identifier = raw.get("id")
        if not isinstance(identifier, str) or not identifier.startswith("div-"):
            raise PrimitiveDifferentialFailure(f"primitive divergence entry {index} has an invalid ID")
        if identifier in result:
            raise PrimitiveDifferentialFailure(f"duplicate primitive divergence {identifier}")
        if raw.get("status") != "intentional-target-refinement" or raw.get("ownerBeads") != "E2.T11":
            raise PrimitiveDifferentialFailure(f"primitive divergence {identifier} lost status or ownership")
        strings = ("operation", "input", "targetValue", "contract", "rationale")
        if any(not isinstance(raw.get(key), str) or not raw.get(key) for key in strings):
            raise PrimitiveDifferentialFailure(f"primitive divergence {identifier} has an invalid text field")
        expectation = raw.get("oracleExpectation")
        if not isinstance(expectation, dict):
            raise PrimitiveDifferentialFailure(
                f"primitive divergence {identifier} has no closed oracle expectation"
            )
        kind = expectation.get("kind")
        if kind == ORACLE_EXACT:
            if (
                set(expectation) != {"kind", "value"}
                or not isinstance(expectation.get("value"), str)
                or not expectation.get("value")
            ):
                raise PrimitiveDifferentialFailure(
                    f"primitive divergence {identifier} has an invalid exact oracle expectation"
                )
            canonical_oracle = str(expectation["value"])
        elif kind == ORACLE_HOST_DEPENDENT_INT32:
            if (
                set(expectation) != {"kind", "canonicalValue"}
                or expectation.get("canonicalValue")
                != ORACLE_HOST_DEPENDENT_INT32_CANONICAL
            ):
                raise PrimitiveDifferentialFailure(
                    f"primitive divergence {identifier} has an invalid host-dependent oracle expectation"
                )
            canonical_oracle = ORACLE_HOST_DEPENDENT_INT32_CANONICAL
        else:
            raise PrimitiveDifferentialFailure(
                f"primitive divergence {identifier} uses unknown oracle expectation {kind!r}"
            )
        contract = raw["contract"]
        tests = raw.get("tests")
        if (
            not isinstance(contract, str)
            or not normalized_repository_path(contract)
            or not (ROOT / contract).is_file()
            or not isinstance(tests, list)
            or tests != ["test/primitive_differential/run.py"]
            or not all(isinstance(path, str) and normalized_repository_path(path) and (ROOT / path).is_file() for path in tests)
        ):
            raise PrimitiveDifferentialFailure(f"primitive divergence {identifier} has stale evidence")
        actual_ids.append(identifier)
        result[identifier] = {
            "oracleKind": str(kind),
            "oracleValue": canonical_oracle,
            "targetValue": str(raw["targetValue"]),
        }
    if actual_ids != expected_ids:
        raise PrimitiveDifferentialFailure(
            f"primitive divergence coverage drifted: expected={expected_ids!r} actual={actual_ids!r}"
        )
    for divergence in DIVERGENCES:
        if result[divergence.identifier]["oracleKind"] != divergence.oracle_kind:
            raise PrimitiveDifferentialFailure(
                f"primitive divergence {divergence.identifier} changed oracle expectation kind"
            )
    return result


def validate_ledger() -> dict[str, dict[str, str]]:
    validate_schema_authority()
    document = json_object(LEDGER_PATH)
    values = validate_ledger_document(document)

    duplicate = copy.deepcopy(document)
    duplicate_entries = duplicate.get("entries")
    if isinstance(duplicate_entries, list):
        duplicate_entries.append(copy.deepcopy(duplicate_entries[0]))
    stale = copy.deepcopy(document)
    stale_entries = stale.get("entries")
    if isinstance(stale_entries, list) and isinstance(stale_entries[0], dict):
        stale_entries[0]["tests"] = ["test/primitive_differential/missing.py"]
    unknown = copy.deepcopy(document)
    unknown_entries = unknown.get("entries")
    if isinstance(unknown_entries, list) and isinstance(unknown_entries[0], dict):
        unknown_entries[0]["id"] = "div-unregistered-edge"
    broad = copy.deepcopy(document)
    broad_entries = broad.get("entries")
    if isinstance(broad_entries, list) and isinstance(broad_entries[1], dict):
        broad_entries[1]["oracleExpectation"] = {
            "kind": ORACLE_HOST_DEPENDENT_INT32,
            "canonicalValue": "*",
        }
    wrong_kind = copy.deepcopy(document)
    wrong_kind_entries = wrong_kind.get("entries")
    if isinstance(wrong_kind_entries, list) and isinstance(wrong_kind_entries[0], dict):
        wrong_kind_entries[0]["oracleExpectation"] = {
            "kind": ORACLE_HOST_DEPENDENT_INT32,
            "canonicalValue": ORACLE_HOST_DEPENDENT_INT32_CANONICAL,
        }
    for label, malformed in (
        ("duplicate", duplicate),
        ("stale", stale),
        ("unknown", unknown),
        ("broad", broad),
        ("wrong-kind", wrong_kind),
    ):
        try:
            validate_ledger_document(malformed)
        except PrimitiveDifferentialFailure:
            continue
        raise PrimitiveDifferentialFailure(f"{label} divergence ledger self-test was accepted")
    validate_oracle_expectation_regression(values)
    return values


def trace_map(lines: Sequence[str], label: str) -> dict[str, str]:
    result: dict[str, str] = {}
    for line in lines:
        parts = line.split(":", 2)
        if len(parts) != 3 or not parts[0]:
            raise PrimitiveDifferentialFailure(f"{label} contains a malformed trace line: {line!r}")
        if parts[0] in result:
            raise PrimitiveDifferentialFailure(f"{label} repeats trace ID {parts[0]}")
        result[parts[0]] = line
    return result


def canonicalize_oracle_divergences(
    raw_trace: str, ledger: Mapping[str, Mapping[str, str]]
) -> str:
    observed = trace_map(raw_trace.splitlines(), "raw Eval divergence trace")
    expected_ids = {divergence.identifier for divergence in DIVERGENCES}
    if set(observed) != expected_ids:
        raise PrimitiveDifferentialFailure(
            "raw Eval divergence trace IDs drifted: "
            f"expected={sorted(expected_ids)!r} actual={sorted(observed)!r}"
        )
    canonical: list[str] = []
    for divergence in DIVERGENCES:
        prefix = f"{divergence.identifier}:{divergence.result_type}:"
        line = observed[divergence.identifier]
        if not line.startswith(prefix):
            raise PrimitiveDifferentialFailure(
                f"raw Eval divergence {divergence.identifier} changed result type: {line!r}"
            )
        value = line[len(prefix) :]
        expectation = ledger[divergence.identifier]
        kind = expectation["oracleKind"]
        if kind == ORACLE_EXACT:
            canonical_value = expectation["oracleValue"]
            if value != canonical_value:
                raise PrimitiveDifferentialFailure(
                    f"raw Eval divergence {divergence.identifier} expected exact "
                    f"{canonical_value!r}, observed {value!r}"
                )
        elif kind == ORACLE_HOST_DEPENDENT_INT32:
            try:
                parsed = int(value, 10)
            except ValueError as error:
                raise PrimitiveDifferentialFailure(
                    f"raw Eval divergence {divergence.identifier} expected a host-dependent "
                    f"Int32, observed {value!r}"
                ) from error
            if str(parsed) != value or not I32_MIN <= parsed <= I32_MAX:
                raise PrimitiveDifferentialFailure(
                    f"raw Eval divergence {divergence.identifier} observed a non-canonical "
                    f"or out-of-range Int32 {value!r}"
                )
            if value == expectation["targetValue"]:
                raise PrimitiveDifferentialFailure(
                    f"primitive divergence {divergence.identifier} disappeared on this host"
                )
            canonical_value = expectation["oracleValue"]
        else:
            raise PrimitiveDifferentialFailure(
                f"primitive divergence {divergence.identifier} has unknown oracle kind {kind!r}"
            )
        canonical.append(f"{prefix}{canonical_value}")
    return "\n".join(canonical) + "\n"


def validate_oracle_expectation_regression(
    ledger: Mapping[str, Mapping[str, str]]
) -> None:
    representative = "\n".join(
        (
            "div-int-modulo-zero:i32:nan",
            "div-std-int-positive-infinity:i32:-1",
            "div-std-int-positive-overflow:i32:-2147483648",
            "",
        )
    )
    if canonicalize_oracle_divergences(
        representative, ledger
    ) != expected_divergence_trace(ledger, "oracleValue"):
        raise PrimitiveDifferentialFailure("host-dependent oracle canonicalization drifted")
    malformed = representative.replace(
        "div-std-int-positive-infinity:i32:-1",
        "div-std-int-positive-infinity:i32:2147483648",
    )
    converged = representative.replace(
        "div-std-int-positive-infinity:i32:-1",
        f"div-std-int-positive-infinity:i32:{I32_MAX}",
    )
    for label, trace in (("out-of-range", malformed), ("converged", converged)):
        try:
            canonicalize_oracle_divergences(trace, ledger)
        except PrimitiveDifferentialFailure:
            continue
        raise PrimitiveDifferentialFailure(
            f"{label} host-dependent oracle self-test was accepted"
        )


def minimize_first_mismatch(
    cases: Sequence[Case], oracle_lines: Sequence[str], target_lines: Sequence[str]
) -> dict[str, object]:
    oracle = trace_map(oracle_lines, "oracle trace")
    target = trace_map(target_lines, "target trace")
    for case in cases:
        oracle_line = oracle.get(case.identifier, "<missing>")
        target_line = target.get(case.identifier, "<missing>")
        if oracle_line != target_line:
            return {
                "schemaVersion": 1,
                "reason": "first-semantic-mismatch",
                "case": case.json_value(),
                "oracleLine": oracle_line,
                "targetLine": target_line,
            }
    extra = sorted((set(oracle) | set(target)) - {case.identifier for case in cases})
    if extra:
        identifier = extra[0]
        return {
            "schemaVersion": 1,
            "reason": "first-semantic-mismatch",
            "case": {"id": identifier, "operation": "unknown", "resultType": "unknown", "arguments": []},
            "oracleLine": oracle.get(identifier, "<missing>"),
            "targetLine": target.get(identifier, "<missing>"),
        }
    raise PrimitiveDifferentialFailure("minimizer was invoked without a semantic mismatch")


def parse_case(raw: object, label: str) -> Case:
    if not isinstance(raw, dict) or set(raw) != {"id", "operation", "resultType", "arguments"}:
        raise PrimitiveDifferentialFailure(f"{label} is not a closed case record")
    identifier = raw.get("id")
    operation_name = raw.get("operation")
    result_type = raw.get("resultType")
    arguments = raw.get("arguments")
    operation = OPERATION_BY_NAME.get(operation_name) if isinstance(operation_name, str) else None
    if (
        not isinstance(identifier, str)
        or operation is None
        or result_type != operation.result_type
        or not isinstance(arguments, list)
        or len(arguments) != len(operation.arguments)
    ):
        raise PrimitiveDifferentialFailure(f"{label} does not match an admitted operation")
    parsed: list[Argument] = []
    for index, (argument, expected_type) in enumerate(zip(arguments, operation.arguments, strict=True)):
        if not isinstance(argument, dict) or set(argument) != {"type", "value"} or argument.get("type") != expected_type:
            raise PrimitiveDifferentialFailure(f"{label} argument {index} is malformed")
        value = argument.get("value")
        if expected_type == "i32" and (not isinstance(value, int) or isinstance(value, bool) or not I32_MIN <= value <= I32_MAX):
            raise PrimitiveDifferentialFailure(f"{label} argument {index} is not i32")
        if expected_type == "u32" and (not isinstance(value, int) or isinstance(value, bool) or not 0 <= value <= U32_MAX):
            raise PrimitiveDifferentialFailure(f"{label} argument {index} is not u32")
        if expected_type == "f64" and (not isinstance(value, str) or not value):
            raise PrimitiveDifferentialFailure(f"{label} argument {index} is not a decimal float")
        parsed.append(Argument(expected_type, value))
    return Case(identifier, operation.name, operation.result_type, tuple(parsed))


def validate_minimizer_regression() -> None:
    document = json_object(MINIMIZER_REGRESSION)
    if set(document) != {
        "schemaVersion",
        "id",
        "corpus",
        "oracleTrace",
        "targetTrace",
        "expectedMinimized",
    } or document.get("schemaVersion") != 1 or document.get("id") != "first-semantic-mismatch":
        raise PrimitiveDifferentialFailure("minimizer regression identity drifted")
    raw_cases = document.get("corpus")
    oracle = document.get("oracleTrace")
    target = document.get("targetTrace")
    if (
        not isinstance(raw_cases, list)
        or not isinstance(oracle, list)
        or not all(isinstance(line, str) for line in oracle)
        or not isinstance(target, list)
        or not all(isinstance(line, str) for line in target)
    ):
        raise PrimitiveDifferentialFailure("minimizer regression payload is malformed")
    cases = tuple(parse_case(raw, f"minimizer corpus[{index}]") for index, raw in enumerate(raw_cases))
    actual = minimize_first_mismatch(cases, oracle, target)
    if actual != document.get("expectedMinimized"):
        raise PrimitiveDifferentialFailure("stored minimizer regression drifted")


def expected_divergence_trace(
    ledger: Mapping[str, Mapping[str, str]], field: str
) -> str:
    return "".join(
        f"{divergence.identifier}:{divergence.result_type}:{ledger[divergence.identifier][field]}\n"
        for divergence in DIVERGENCES
    )


def snapshot_values() -> dict[str, object]:
    validate_minimizer_regression()
    ledger = validate_ledger()
    corpus, cases = generate_cases()
    repeated_corpus, repeated_cases = generate_cases()
    if corpus != repeated_corpus or cases != repeated_cases:
        raise PrimitiveDifferentialFailure("seeded corpus changed across repeated generation")
    source = render_fixture(cases)
    if source != render_fixture(repeated_cases):
        raise PrimitiveDifferentialFailure("generated Haxe source changed across repeated generation")
    oracle, raw_divergence_oracle = render_oracle(source)
    repeated_oracle, repeated_raw_divergence = render_oracle(source)
    if oracle != repeated_oracle or raw_divergence_oracle != repeated_raw_divergence:
        raise PrimitiveDifferentialFailure("pinned Eval oracle changed across repeated execution")
    divergence_oracle = canonicalize_oracle_divergences(
        raw_divergence_oracle, ledger
    )
    if divergence_oracle != expected_divergence_trace(ledger, "oracleValue"):
        raise PrimitiveDifferentialFailure(
            "canonical Eval divergence observations drifted from the ledger"
        )
    return {
        "corpus.json": corpus,
        "PrimitiveDifferentialFixture.hx": source,
        "oracle.txt": oracle,
        "divergence-oracle.txt": divergence_oracle,
        **production_snapshot(source),
    }


def difference(expected: str, actual: str, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def check_snapshots(values: Mapping[str, object]) -> None:
    if set(values) != set(SNAPSHOT_FORMATS):
        raise PrimitiveDifferentialFailure("primitive differential snapshot set drifted")
    for name, actual in values.items():
        path = EXPECTED / name
        if not path.is_file():
            raise PrimitiveDifferentialFailure(f"registered snapshot {name} is missing")
        if name.endswith(".json"):
            expected: object = json.loads(path.read_text(encoding="utf-8"))
            if actual != expected:
                raise PrimitiveDifferentialFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise PrimitiveDifferentialFailure(f"{name} snapshot is not text")
            expected_text = path.read_text(encoding="utf-8")
            if actual != expected_text:
                raise PrimitiveDifferentialFailure(
                    f"{name} drifted:\n" + difference(expected_text, actual, name)
                )


def symbol_entries(symbols: Mapping[str, object]) -> list[Mapping[str, object]]:
    values = symbols.get("symbols")
    if not isinstance(values, list) or not all(isinstance(item, dict) for item in values):
        raise PrimitiveDifferentialFailure("generated symbol table is malformed")
    return values


def function_c_name(symbols: Mapping[str, object], field: str) -> str:
    prefix = f"PrimitiveDifferentialFixture.{field}"
    matches = [
        item.get("cName")
        for item in symbol_entries(symbols)
        if item.get("kind") == "method"
        and isinstance(item.get("sourceSymbol"), str)
        and (item["sourceSymbol"] == prefix or item["sourceSymbol"].startswith(prefix + "("))
    ]
    if len(matches) != 1 or not isinstance(matches[0], str):
        raise PrimitiveDifferentialFailure(f"cannot resolve unique C name for {prefix}")
    return matches[0]


def c_i32(value: int) -> str:
    if value == I32_MIN:
        return "INT32_MIN"
    if value < 0:
        return f"-INT32_C({-value})"
    return f"INT32_C({value})"


def c_argument(argument: Argument) -> str:
    if argument.type == "i32" and isinstance(argument.value, int):
        return c_i32(argument.value)
    if argument.type == "u32" and isinstance(argument.value, int):
        return f"UINT32_C({argument.value})"
    if argument.type == "f64" and isinstance(argument.value, str):
        return argument.value
    raise PrimitiveDifferentialFailure(f"invalid C argument {argument!r}")


def harness_line(identifier: str, result_type: str, call: str) -> str:
    if result_type == "i32":
        return f'  (void)printf("{identifier}:i32:%" PRId32 "\\n", {call});'
    if result_type == "u32":
        return f'  (void)printf("{identifier}:u32:%" PRIu32 "\\n", {call});'
    if result_type == "bool":
        return f'  (void)printf("{identifier}:bool:%s\\n", {call} ? "true" : "false");'
    raise PrimitiveDifferentialFailure(f"unknown harness result type {result_type}")


def render_harness(cases: Sequence[Case], symbols: Mapping[str, object]) -> str:
    names = {operation.name: function_c_name(symbols, operation.name) for operation in OPERATIONS}
    names["fint"] = function_c_name(symbols, "fint")
    lines = [
        "#ifdef main",
        "#undef main",
        "#endif",
        '#include "hxc/program.h"',
        "",
        "#include <inttypes.h>",
        "#include <math.h>",
        "#include <stdint.h>",
        "#include <stdio.h>",
        "",
        "int main(void)",
        "{",
    ]
    for case in cases:
        arguments = ", ".join(c_argument(argument) for argument in case.arguments)
        lines.append(
            harness_line(case.identifier, case.result_type, f'{names[case.operation]}({arguments})')
        )
    for divergence in DIVERGENCES:
        arguments = ", ".join(divergence.c_call_arguments)
        lines.append(
            harness_line(
                divergence.identifier,
                divergence.result_type,
                f'{names[divergence.operation]}({arguments})',
            )
        )
    lines.extend(("  return 0;", "}", ""))
    return "\n".join(lines)


def write_native_fixture(
    root: Path,
    values: Mapping[str, object],
    cases: Sequence[Case],
) -> None:
    header = values.get("program.h")
    source = values.get("program.c")
    symbols = values.get("symbols.json")
    if not isinstance(header, str) or not isinstance(source, str) or not isinstance(symbols, dict):
        raise PrimitiveDifferentialFailure("native snapshot inputs are malformed")
    (root / "include/hxc").mkdir(parents=True)
    (root / "src").mkdir(parents=True)
    (root / "include/hxc/program.h").write_text(header, encoding="utf-8", newline="\n")
    (root / "src/program.c").write_text(source, encoding="utf-8", newline="\n")
    (root / "include/hxc/renamed_entry.h").write_text(
        "int hxc_generated_main(void);\n", encoding="utf-8", newline="\n"
    )
    (root / "harness.c").write_text(
        render_harness(cases, symbols), encoding="utf-8", newline="\n"
    )


def sanitizer_supported(toolchain: CToolchain, root: Path) -> bool:
    source = root / f"sanitizer-probe-{toolchain.family}.c"
    executable = root / f"sanitizer-probe-{toolchain.family}"
    source.write_text("int main(void) { return 0; }\n", encoding="utf-8", newline="\n")
    compiled = subprocess.run(
        [toolchain.compiler, *SANITIZER_FLAGS, str(source), "-o", str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if compiled.returncode != 0:
        return False
    executed = subprocess.run(
        [str(executable)],
        cwd=root,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    return executed.returncode == 0 and not executed.stdout and not executed.stderr


def mismatch_from_fixture_error(
    error: CFixtureFailure,
    cases: Sequence[Case],
    oracle_trace: str,
) -> PrimitiveDifferentialFailure:
    marker = "produced unexpected output:\n"
    message = str(error)
    if marker not in message:
        return PrimitiveDifferentialFailure(message)
    try:
        record = json.loads(message.split(marker, 1)[1])
        stdout = record.get("stdout") if isinstance(record, dict) else None
        if not isinstance(stdout, str):
            return PrimitiveDifferentialFailure(message)
        target_lines = [line for line in stdout.splitlines() if line.startswith("case-")]
        minimized = minimize_first_mismatch(cases, oracle_trace.splitlines(), target_lines)
    except (json.JSONDecodeError, PrimitiveDifferentialFailure):
        return PrimitiveDifferentialFailure(message)
    return PrimitiveDifferentialFailure(
        message
        + "\nminimized replay fixture:\n"
        + json.dumps(minimized, ensure_ascii=False, indent=2)
    )


def run_native_configuration(
    *,
    fixture_root: Path,
    build_root: Path,
    toolchain: CToolchain,
    optimization: str,
    expected_stdout: str,
    cases: Sequence[Case],
    oracle_trace: str,
    sanitizer: bool,
) -> None:
    qualifier = optimization[1:].lower() + ("-asan-ubsan" if sanitizer else "")
    coverage = ["known-divergences", "runtime-free", "seeded-differential"]
    extra_flags: tuple[str, ...] = ()
    link_arguments: tuple[str, ...] = ("-lm",)
    if sanitizer:
        coverage.append("asan-ubsan")
        extra_flags = SANITIZER_FLAGS
        link_arguments = (*SANITIZER_FLAGS, "-lm")
    project = CFixtureProject(
        identifier=f"primitive-differential-{qualifier}",
        sources=("src/program.c", "harness.c"),
        headers=("include/hxc/program.h", "include/hxc/renamed_entry.h"),
        include_directories=("include",),
        expected_stdout=expected_stdout,
        coverage=tuple(sorted(coverage)),
        link_arguments=link_arguments,
    )
    strict_flags = (
        *C11_STRICT_FLAGS,
        optimization,
        *extra_flags,
        "-Dmain=hxc_generated_main",
        "-include",
        "hxc/renamed_entry.h",
    )
    try:
        report = run_c_fixture_corpus(
            suite="primitive-differential",
            projects=(project,),
            fixture_root=fixture_root,
            build_root=build_root,
            repository_root=ROOT,
            requested_toolchain=toolchain.family,
            strict_flags=strict_flags,
            timeout_seconds=60,
        )
    except CFixtureFailure as error:
        raise mismatch_from_fixture_error(error, cases, oracle_trace) from error
    validate_report(report, required_coverage=frozenset(coverage))
    serialized = json.dumps(report, ensure_ascii=False, sort_keys=True)
    if str(fixture_root) in serialized or str(build_root) in serialized or str(ROOT) in serialized:
        raise PrimitiveDifferentialFailure("native argument-array report leaked a host path")


def cases_from_corpus(document: object) -> tuple[Case, ...]:
    if not isinstance(document, dict) or document.get("schemaVersion") != 1:
        raise PrimitiveDifferentialFailure("native corpus snapshot is malformed")
    raw_cases = document.get("cases")
    if not isinstance(raw_cases, list):
        raise PrimitiveDifferentialFailure("native corpus snapshot omitted cases")
    cases = tuple(parse_case(raw, f"corpus cases[{index}]") for index, raw in enumerate(raw_cases))
    if document.get("caseCount") != len(cases):
        raise PrimitiveDifferentialFailure("native corpus case count drifted")
    return cases


def run_native_matrix(
    values: Mapping[str, object],
    cases: Sequence[Case],
    ledger: Mapping[str, Mapping[str, str]],
    requested: str,
) -> None:
    oracle_trace = values.get("oracle.txt")
    if not isinstance(oracle_trace, str):
        raise PrimitiveDifferentialFailure("native matrix omitted the oracle trace")
    expected_stdout = oracle_trace + expected_divergence_trace(ledger, "targetValue")
    toolchains = resolve_toolchains(requested, repository_root=ROOT)
    with tempfile.TemporaryDirectory(prefix="hxc-primitive-native-") as temporary:
        root = Path(temporary)
        fixture_root = root / "fixture"
        write_native_fixture(fixture_root, values, cases)
        for toolchain in toolchains:
            print(
                f"primitive-differential: {'REQUIRED' if requested != 'auto' else 'AVAILABLE'} "
                f"{toolchain.family}: cc={toolchain.version_line}"
            )
            run_native_configuration(
                fixture_root=fixture_root,
                build_root=root / f"build-{toolchain.family}-o0",
                toolchain=toolchain,
                optimization="-O0",
                expected_stdout=expected_stdout,
                cases=cases,
                oracle_trace=oracle_trace,
                sanitizer=False,
            )
            run_native_configuration(
                fixture_root=fixture_root,
                build_root=root / f"build-{toolchain.family}-o2",
                toolchain=toolchain,
                optimization="-O2",
                expected_stdout=expected_stdout,
                cases=cases,
                oracle_trace=oracle_trace,
                sanitizer=False,
            )
            if sanitizer_supported(toolchain, root):
                run_native_configuration(
                    fixture_root=fixture_root,
                    build_root=root / f"build-{toolchain.family}-sanitizer",
                    toolchain=toolchain,
                    optimization="-O1",
                    expected_stdout=expected_stdout,
                    cases=cases,
                    oracle_trace=oracle_trace,
                    sanitizer=True,
                )
            elif requested != "auto":
                raise PrimitiveDifferentialFailure(
                    f"required {toolchain.family} address/undefined sanitizers are unavailable"
                )
            else:
                print(
                    f"primitive-differential: SKIP optional {toolchain.family} ASan/UBSan: probe unavailable"
                )


def load_expected_values() -> dict[str, object]:
    values: dict[str, object] = {}
    for name in SNAPSHOT_FORMATS:
        path = EXPECTED / name
        if not path.is_file():
            raise PrimitiveDifferentialFailure(f"native-only snapshot {name} is missing")
        values[name] = (
            json.loads(path.read_text(encoding="utf-8"))
            if name.endswith(".json")
            else path.read_text(encoding="utf-8")
        )
    return values


def validate_toolchain_oracle_pin() -> None:
    lock = json_object(TOOLCHAIN_LOCK)
    haxe = lock.get("haxe")
    if not isinstance(haxe, dict) or haxe.get("version") != "5.0.0-preview.1":
        raise PrimitiveDifferentialFailure("primitive oracle identity drifted from the Haxe lock")


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("gcc", "clang"))
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("primitive-differential: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        validate_toolchain_oracle_pin()
        validate_minimizer_regression()
        ledger = validate_ledger()
        requested = args.toolchain or "auto"
        if args.native_only:
            values = load_expected_values()
            cases = cases_from_corpus(values["corpus.json"])
            if values.get("divergence-oracle.txt") != expected_divergence_trace(ledger, "oracleValue"):
                raise PrimitiveDifferentialFailure("checked-in divergence oracle drifted")
            run_native_matrix(values, cases, ledger, requested)
            print(
                "primitive-differential: OK: required checked-in seeded corpus passed O0/O2 and ASan/UBSan"
            )
            return 0

        values = snapshot_values()
        check_snapshots(values)
        cases = cases_from_corpus(values["corpus.json"])
        run_native_matrix(values, cases, ledger, requested)
    except (
        CFixtureFailure,
        PrimitiveDifferentialFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"primitive-differential: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "primitive-differential: OK: reproducible seeded Eval differential, minimized regression, "
        "ledgered refinements, runtime-free C11, and ASan/UBSan passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

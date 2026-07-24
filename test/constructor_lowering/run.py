#!/usr/bin/env python3
"""Prove bounded constructor ordering, cleanup, elision, and fail-closed escapes."""

from __future__ import annotations

import argparse
import copy
import difflib
import json
import os
import shutil
import signal
import socket
import subprocess
import sys
import tempfile
import time
from collections.abc import Callable, Iterable
from dataclasses import replace
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURES = Path(__file__).with_name("fixtures")
POSITIVE = FIXTURES / "positive"
ORACLE = FIXTURES / "oracle"
MINIMAL = FIXTURES / "minimal"
FAILURE_RUNTIME = FIXTURES / "failure_runtime"
DEFAULT_RUNTIME = FIXTURES / "default_runtime"
RECORD_PARAMETER = FIXTURES / "record_parameter"
INTERFACE_PARAMETER = FIXTURES / "interface_parameter"
RETAINED_INTERFACE_PARAMETER = FIXTURES / "interface_parameter_retained"
DEFAULT_ARGUMENTS = FIXTURES / "default_arguments"
ARRAY_PARAMETER = FIXTURES / "array_parameter"
STRING_PARAMETER = FIXTURES / "string_parameter"
ENUM_PARAMETER = FIXTURES / "enum_parameter"
ENUM_PAYLOAD_PARAMETER = FIXTURES / "enum_payload_parameter"
DIRECT_RECEIVER = FIXTURES / "direct_receiver"
DIRECT_RECEIVER_FAILURE = FIXTURES / "direct_receiver_failure"
DIRECT_RECEIVER_ESCAPE = FIXTURES / "direct_receiver_escape"
NATIVE = Path(__file__).with_name("native")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_CONSTRUCTOR_LOWERING="
PRODUCTION_FILES = {
    "_GeneratedFiles.json",
    "cmake/CMakeLists.txt",
    "hxc.abi.json",
    "hxc.dispatch.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
    "include/hxc/program.h",
    "meson.build",
    "src/program.c",
}
EXPECTED_CONSTRUCTORS = {
    "constructor.BaseRecord": (False, False),
    "constructor.CounterOwner": (False, False),
    "constructor.EmptyBase": (True, False),
    "constructor.EmptyLeaf": (True, False),
    "constructor.FailableBase": (False, True),
    "constructor.FailableLeaf": (False, True),
    "constructor.LeafRecord": (False, False),
    "constructor.OwnedCounter": (False, False),
}
NEGATIVE_CASES = {
    "borrowed_alias": (
        "TNew(stack-reference-escape:static-call-argument:0,"
        "target=function.Main.alias)"
    ),
    "borrowed_constructor": (
        "TNew(stack-reference-escape:static-call-argument:0,"
        "target=function.Main.observe)"
    ),
    "borrowed_forward": (
        "TCall(owned-class-borrow-escape:instance-call-argument:0,"
        "target=method.BorrowedForwardSink.consume)"
    ),
    "borrowed_return": (
        "TNew(stack-reference-escape:static-call-argument:0,"
        "target=function.Main.expose)"
    ),
    "conditional": "TNew(stack-construction-requires-unconditional-entry-block)",
    "cycle": "TNew(constructor-cycle:CycleA -> CycleB -> CycleA)",
    "default_callable": (
        "TFunction(constructor-argument:callback):"
        "payload-requires-direct-unmanaged-value:direct-function:()->int32_t"
    ),
    "direct_receiver_escape": "TVar(escaped:owned-class-borrow-escape)",
    "escape_alias": "TNew(stack-reference-escape:assignment)",
    "escape_return": "TNew(stack-reference-escape:return)",
    "escape_self": "TNew(stack-reference-escape:assignment)",
    "generic": "TVar(box:type):generic-class-reference-requires-bounded-class-specialization:Box",
    "instance_parameter": "function-exit:unowned-fresh-managed-enum-value",
    "recursive_enum_parameter": "function-exit:unowned-fresh-managed-enum-value",
    "interface_parameter_escape": (
        "TFunction(constructor-argument:source:"
        "interface-retention-must-target-this-field)"
    ),
    "array_parameter_escape": "TBinop(OpAssign:managed-Array-reassignment-not-admitted)",
    "native_layout": "TNew(unsupported-native-layout:NativeRecord)",
    "owned_fallible": "TNew(owned-field-fallible-construction-not-admitted:constructor.Child)",
    "owned_mutable": "field:child:owned-class-field-must-be-final",
    "owned_return": "TReturn(owned-class-borrow-escape)",
}
REQUIRED_NATIVE_COVERAGE = frozenset(
    {
        "constructor-cleanup-failure",
        "constructor-default-field-values",
        "constructor-generated-executable",
        "constructor-runtime-free",
        "field-initialization-order",
        "owned-child-inline-layout",
        "owned-child-inline-method-borrow",
        "owned-child-lazy-borrow-alias",
        "owned-child-stable-identity",
        "strict-c11",
        "super-constructor-order",
        "trivial-constructor-elision",
    }
)
RECORD_NATIVE_COVERAGE = frozenset(
    {
        "constructor-record-parameter",
        "constructor-record-parameter-by-value",
        "constructor-record-parameter-stack-owner",
    }
)
INTERFACE_NATIVE_COVERAGE = frozenset(
    {
        "constructor-interface-parameter",
        "constructor-interface-parameter-by-value",
        "constructor-interface-parameter-call-borrow",
        "constructor-interface-parameter-dispatch",
    }
)
RETAINED_INTERFACE_NATIVE_COVERAGE = frozenset(
    {
        "constructor-retained-interface-field",
        "constructor-retained-interface-managed-owner",
        "constructor-retained-interface-managed-implementation",
        "constructor-retained-interface-trace",
        "constructor-retained-interface-dispatch-after-collection",
    }
)
DEFAULT_ARGUMENT_NATIVE_COVERAGE = frozenset(
    {
        "constructor-default-argument-omission",
        "constructor-default-argument-supplied-once",
        "constructor-explicit-null-argument",
        "constructor-optional-record-argument",
        "super-constructor-default-argument",
    }
)
ARRAY_PARAMETER_NATIVE_COVERAGE = frozenset(
    {
        "constructor-array-parameter",
        "constructor-array-parameter-call-borrow",
        "constructor-array-parameter-fresh-owner",
        "constructor-array-parameter-retained-field",
        "constructor-array-parameter-shared-identity",
    }
)
STRING_PARAMETER_NATIVE_COVERAGE = frozenset(
    {
        "constructor-nominal-string-parameter",
        "constructor-nominal-string-identity",
        "constructor-static-string-call-borrow",
        "constructor-static-string-retained-field",
        "constructor-static-string-allocation-free",
    }
)
ENUM_PARAMETER_NATIVE_COVERAGE = frozenset(
    {
        "constructor-fieldless-enum-parameter",
        "constructor-fieldless-enum-identity",
        "constructor-fieldless-enum-call-borrow",
        "constructor-fieldless-enum-final-field",
        "constructor-fieldless-enum-runtime-free",
    }
)
ENUM_PAYLOAD_PARAMETER_NATIVE_COVERAGE = frozenset(
    {
        "constructor-unmanaged-payload-enum-parameter",
        "constructor-unmanaged-payload-enum-identity",
        "constructor-unmanaged-payload-enum-call-borrow",
        "constructor-unmanaged-payload-enum-final-field",
        "constructor-unmanaged-payload-enum-active-tag",
        "constructor-unmanaged-payload-enum-allocation-free",
    }
)
DIRECT_RECEIVER_NATIVE_COVERAGE = frozenset(
    {
        "constructor-direct-receiver-automatic-storage",
        "constructor-direct-receiver-managed-cleanup",
        "constructor-direct-receiver-nested-argument",
        "constructor-direct-receiver-result-transfer",
    }
)
DIRECT_RECEIVER_FAILURE_NATIVE_COVERAGE = frozenset(
    {
        "constructor-direct-receiver-partial-cleanup",
        "constructor-direct-receiver-throw",
    }
)
RETAINED_INTERFACE_RUNTIME_FEATURES = [
    "runtime-base",
    "status",
    "alloc",
    "object",
    "gc",
]

if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.c_fixture_harness import (  # noqa: E402
    C11_STRICT_FLAGS,
    CFixtureFailure,
    CFixtureProject,
    report_json,
    resolve_toolchains,
    run_c_fixture_corpus,
    validate_report,
)


CXX_STRICT_FLAGS = (
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
CXX_COMMANDS = {"gcc": "g++", "clang": "clang++"}
SANITIZER_FLAGS = (
    "-O1",
    "-g",
    "-fno-omit-frame-pointer",
    "-fno-sanitize-recover=all",
    "-fsanitize=address,undefined",
)


class ConstructorLoweringFailure(RuntimeError):
    pass


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


def custom_target(
    fixture: Path,
    output: Path,
    *,
    profile: str = "portable",
    runtime: str | None = None,
    reverse: bool = False,
    report: bool = False,
    layout: str = "unity",
    connect: str | None = None,
    runtime_diagnostics: str | None = None,
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
        ]
    )
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    elif profile != "portable":
        raise ConstructorLoweringFailure(f"unknown profile {profile!r}")
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_constructor_lowering_report"])
    if runtime_diagnostics is not None:
        command.extend(["-D", f"hxc_runtime_diagnostics={runtime_diagnostics}"])
    command.extend(["-D", f"hxc_project_layout={layout}", "--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(server=connect is not None),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def generated_files(root: Path) -> set[str]:
    if not root.exists():
        return set()
    return {
        path.relative_to(root).as_posix()
        for path in root.rglob("*")
        if path.is_file()
    }


def generated_tree(root: Path) -> dict[str, bytes]:
    return {
        path.relative_to(root).as_posix(): path.read_bytes()
        for path in sorted(root.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def validate_record_parameter_project(output: Path) -> None:
    sources = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    symbols = json.loads((output / "hxc.symbols.json").read_text(encoding="utf-8"))
    constructors = [
        symbol
        for symbol in symbols.get("symbols", [])
        if isinstance(symbol, dict)
        and symbol.get("kind") == "method"
        and str(symbol.get("sourceSymbol", "")).startswith(
            "compiler.constructor.ConfiguredSpawn("
        )
    ]
    if len(constructors) != 1:
        raise ConstructorLoweringFailure(
            "record-parameter fixture omitted its one constructor symbol"
        )
    overload = constructors[0].get("overloadSignature")
    if (
        not isinstance(overload, list)
        or len(overload) != 2
        or not str(overload[0]).startswith("class-reference:nonnull:")
        or not str(overload[1]).startswith("closed-record:instance.closed-record.")
    ):
        raise ConstructorLoweringFailure(
            "record-parameter constructor lost its family-specific semantic key"
        )
    if (
        "struct hxc_SpawnPoint hxc_point" not in sources
        or "struct hxc_SpawnPoint hxc_point" not in headers
        or "struct hxc_ConfiguredSpawn" not in sources
        or " = { 0 };" not in sources
        or "struct hxc_SpawnPoint *hxc_point" in sources
        or "struct hxc_SpawnPoint *hxc_point" in headers
    ):
        raise ConstructorLoweringFailure(
            "record parameter was not passed by value to a stack-owned class"
        )
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("features") != []
        or "bounded-stack-construction" not in plan.get("directDecisions", [])
    ):
        raise ConstructorLoweringFailure(
            "record-parameter constructor lost its direct runtime-free plan"
        )
    for forbidden in ("hxrt", "malloc(", "calloc(", "realloc(", "goto "):
        if forbidden in (sources + headers).lower():
            raise ConstructorLoweringFailure(
                f"record-parameter constructor emitted forbidden shape {forbidden!r}"
            )


def validate_interface_parameter_project(output: Path) -> None:
    """Prove the constructor keeps the nominal interface and its bounded borrow."""

    sources = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    symbols = json.loads((output / "hxc.symbols.json").read_text(encoding="utf-8"))
    dispatch = json.loads((output / "hxc.dispatch.json").read_text(encoding="utf-8"))
    constructors = [
        symbol
        for symbol in symbols.get("symbols", [])
        if isinstance(symbol, dict)
        and symbol.get("kind") == "method"
        and str(symbol.get("sourceSymbol", "")).startswith(
            "compiler.constructor.ConfiguredScore("
        )
    ]
    if len(constructors) != 1:
        raise ConstructorLoweringFailure(
            "interface-parameter fixture omitted its one constructor symbol"
        )
    overload = constructors[0].get("overloadSignature")
    if (
        not isinstance(overload, list)
        or len(overload) != 3
        or not str(overload[0]).startswith("class-reference:nonnull:")
        or not str(overload[1]).startswith(
            "interface-reference:instance.interface."
        )
        or overload[2] != "i32"
    ):
        raise ConstructorLoweringFailure(
            "interface-parameter constructor lost its nominal semantic key"
        )
    interface_value = "struct hxc_compiler_interface_dispatch_ScoreSource_value"
    if (
        f"{interface_value} hxc_source" not in sources
        or f"{interface_value} hxc_source" not in headers
        or f"{interface_value} *hxc_source" in sources
        or f"{interface_value} *hxc_source" in headers
        or "hxc_source.table->hxc_interface_slot_ScoreSource_score("
        "hxc_source.object, hxc_seed)" not in sources
    ):
        raise ConstructorLoweringFailure(
            "interface parameter was not a by-value object/table pair with interface dispatch"
        )
    calls = dispatch.get("calls")
    if (
        not isinstance(calls, list)
        or not any(
            isinstance(call, dict)
            and call.get("callerFunctionId") == "constructor.ConfiguredScore"
            and call.get("methodFunctionId") == "method.ScoreSource.score"
            and call.get("dispatch") == "interface"
            for call in calls
        )
    ):
        raise ConstructorLoweringFailure(
            "interface-parameter constructor lost its typed interface-dispatch plan"
        )
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if plan.get("features") != []:
        raise ConstructorLoweringFailure(
            "interface-parameter constructor unexpectedly selected hxrt"
        )
    for forbidden in ("hxrt", "malloc(", "calloc(", "realloc(", "goto "):
        if forbidden in (sources + headers).lower():
            raise ConstructorLoweringFailure(
                f"interface-parameter constructor emitted forbidden shape {forbidden!r}"
            )


def validate_retained_interface_project(output: Path) -> None:
    """Prove a retained interface traces its collector-owned concrete object."""

    sources = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    interface_value = "struct hxc_compiler_interface_dispatch_ScoreSource_value"
    if (
        plan.get("features") != RETAINED_INTERFACE_RUNTIME_FEATURES
        or f"{interface_value} hxc_source" not in sources
        or f"{interface_value} hxc_source" not in headers
        or f"{interface_value} *hxc_source" in sources
        or f"{interface_value} *hxc_source" in headers
    ):
        raise ConstructorLoweringFailure(
            "retained interface fixture lost its by-value field or collector plan"
        )
    if (
        "hxc_source.object" not in sources
        or "table->hxc_interface_slot_ScoreSource_score(" not in sources
        or "hxc_trace_visit_fn" not in sources
    ):
        raise ConstructorLoweringFailure(
            "retained interface fixture lost exact object tracing or later dispatch"
        )


def validate_default_argument_project(output: Path) -> None:
    """Prove constructor calls settle defaults before fixed-arity C emission."""

    sources = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    symbols = json.loads((output / "hxc.symbols.json").read_text(encoding="utf-8"))
    constructors = [
        symbol
        for symbol in symbols.get("symbols", [])
        if isinstance(symbol, dict)
        and symbol.get("kind") == "method"
        and str(symbol.get("sourceSymbol", "")).startswith("compiler.constructor.")
        and ".DefaultedRecord(" in str(symbol.get("sourceSymbol", ""))
    ]
    if len(constructors) != 1:
        raise ConstructorLoweringFailure(
            "default-argument fixture omitted its fixed-arity constructor symbol"
        )
    overload = constructors[0].get("overloadSignature")
    if (
        not isinstance(overload, list)
        or len(overload) != 3
        or not str(overload[0]).startswith("class-reference:nonnull:")
        or overload[1] != "i32"
        or not str(overload[2]).startswith("direct-optional:optional.")
    ):
        raise ConstructorLoweringFailure(
            "default-argument constructor lost its settled optional-record signature"
        )
    if (
        "hxc_compiler_constructor_Main_DefaultedRecord("
        "struct hxc_Main_DefaultedRecord *hxc_self, int32_t hxc_value, "
        "struct hxc_optional_Main_RestorePoint hxc_restore)" not in headers
        or sources.count("= hxc_Main_DefaultSource_next();") != 1
        or sources.count(".hxc_has_value = true") != 1
        or sources.count(".hxc_has_value = false") < 4
    ):
        raise ConstructorLoweringFailure(
            "constructor omission, explicit null, presence, or once-only argument evaluation drifted"
        )
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("features") != []
        or "bounded-stack-construction" not in plan.get("directDecisions", [])
    ):
        raise ConstructorLoweringFailure(
            "default-argument fixture lost its direct runtime-free plan"
        )
    for forbidden in ("hxrt", "malloc(", "calloc(", "realloc(", "goto "):
        if forbidden in (sources + headers).lower():
            raise ConstructorLoweringFailure(
                f"default-argument constructor emitted forbidden shape {forbidden!r}"
            )


def validate_array_parameter_project(output: Path) -> None:
    """Prove constructor Array values borrow calls and retain stored identity."""

    source = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    symbols = json.loads((output / "hxc.symbols.json").read_text(encoding="utf-8"))
    constructors = [
        symbol
        for symbol in symbols.get("symbols", [])
        if isinstance(symbol, dict)
        and symbol.get("kind") == "method"
        and str(symbol.get("sourceSymbol", "")).startswith("compiler.constructor.")
        and (
            ".BorrowedArrayReader(" in str(symbol.get("sourceSymbol", ""))
            or ".RetainedArrayReader(" in str(symbol.get("sourceSymbol", ""))
        )
    ]
    array_constructors = [
        symbol
        for symbol in constructors
        if isinstance(symbol.get("overloadSignature"), list)
        and any(
            str(value).startswith("array-reference:instance.haxe-array.")
            for value in symbol["overloadSignature"]
        )
    ]
    if len(array_constructors) != 2:
        raise ConstructorLoweringFailure(
            "Array-parameter fixture lost its two family-specific constructor symbols"
        )
    parameter_spelling = "struct hxc_array_ref *hxc_values"
    retained_start = source.find(
        "hxc_compiler_constructor_Main_RetainedArrayReader("
    )
    retained_end = source.find("\n}\n", retained_start)
    borrowed_start = source.find(
        "hxc_compiler_constructor_Main_BorrowedArrayReader("
    )
    borrowed_end = source.find("\n}\n", borrowed_start)
    if min(retained_start, retained_end, borrowed_start, borrowed_end) < 0:
        raise ConstructorLoweringFailure(
            "Array-parameter fixture omitted a constructor definition"
        )
    retained_body = source[retained_start:retained_end]
    borrowed_body = source[borrowed_start:borrowed_end]
    if (
        parameter_spelling not in retained_body
        or parameter_spelling not in borrowed_body
        or parameter_spelling not in headers
        or retained_body.count("hxc_array_ref_retain(") != 1
        or "hxc_array_ref_retain(" in borrowed_body
        or "hxc_array_ref_get_copy(" not in borrowed_body
        or "hxc_array_ref_length(" not in borrowed_body
        or "constructor_argument_0_owner" not in source
        or source.count("hxc_array_ref_release(") < 4
    ):
        raise ConstructorLoweringFailure(
            "Array constructor borrow, retained field, fresh owner, or cleanup drifted"
        )
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("features") != ["runtime-base", "status", "alloc", "array"]
        or "managed-haxe-arrays" not in plan.get("directDecisions", [])
        or "bounded-stack-construction" not in plan.get("directDecisions", [])
    ):
        raise ConstructorLoweringFailure(
            "Array-parameter constructor lost its exact dependency-closed runtime plan"
        )
    for forbidden in ("goto ", "malloc(", "calloc(", "realloc("):
        if forbidden in source:
            raise ConstructorLoweringFailure(
                f"Array-parameter application module emitted forbidden shape {forbidden!r}"
            )


def validate_direct_receiver_project(output: Path) -> None:
    """Prove unnamed receivers use bounded storage and transfer method results."""

    source = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    parse_start = source.find("struct hxc_array_ref *hxc_Main_parse(")
    fresh_start = source.find("struct hxc_array_ref *hxc_Main_parseFresh(")
    values_start = source.find("struct hxc_array_ref *hxc_Main_valuesFrom(")
    if min(parse_start, fresh_start, values_start) < 0:
        raise ConstructorLoweringFailure(
            "direct-receiver fixture omitted parse, nested parse, or argument producer"
        )
    parse_body = source[parse_start:fresh_start]
    fresh_body = source[fresh_start:values_start]
    if (
        source.count("struct hxc_NumberReader hxc_tmp_object_storage_") != 2
        or source.count(" = { 0 };") < 2
        or source.count("hxc_compiler_constructor_NumberReader(") < 3
        or source.count("hxc_NumberReader_read(") < 3
        or "hxc_array_ref_release(hxc_tmp_object_storage_" not in parse_body
        or "hxc_array_ref_release(hxc_tmp_object_storage_" not in fresh_body
        or "constructed_receiver_argument_0_owner" not in fresh_body
        or "constructed_receiver_argument_0_borrow_result" not in fresh_body
        or "hxc_array_ref_retain(" in parse_body
        or "hxc_array_ref_retain(" in fresh_body
        or "struct hxc_NumberReader" not in headers
    ):
        raise ConstructorLoweringFailure(
            "direct receiver lost automatic storage, nested ownership, cleanup, or result transfer"
        )
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("features") != ["runtime-base", "status", "alloc", "array"]
        or "bounded-stack-construction" not in plan.get("directDecisions", [])
        or "managed-haxe-arrays" not in plan.get("directDecisions", [])
        or "gc" in plan.get("features", [])
    ):
        raise ConstructorLoweringFailure(
            "direct receiver lost its bounded stack and dependency-closed Array plan"
        )
    for forbidden in ("goto ", "malloc(", "calloc(", "realloc("):
        if forbidden in source:
            raise ConstructorLoweringFailure(
                f"direct-receiver application module emitted forbidden shape {forbidden!r}"
            )


def validate_direct_receiver_failure_project(output: Path) -> None:
    """Prove a throwing constructor releases its field and nested argument once."""

    source = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    failure_start = source.find(
        "if (!hxc_compiler_constructor_ExplodingReader("
    )
    failure_end = source.find("\n  if (hxc_tmp_class_object_address_", failure_start)
    if min(failure_start, failure_end) < 0:
        raise ConstructorLoweringFailure(
            "direct-receiver failure fixture omitted its checked constructor call"
        )
    failure_body = source[failure_start:failure_end]
    if (
        failure_body.count("hxc_array_ref_release(") != 2
        or "hxc_tmp_object_storage_" not in failure_body
        or "constructed_receiver_argument_0_owner" not in failure_body
        or failure_body.rfind("abort();") < failure_body.rfind("hxc_array_ref_release(")
    ):
        raise ConstructorLoweringFailure(
            "throwing direct receiver did not release its initialized field and nested argument before abort"
        )


def validate_string_parameter_project(output: Path) -> None:
    """Prove nominal literal String views stay exact, by-value, and allocation-free."""

    source = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    symbols = json.loads((output / "hxc.symbols.json").read_text(encoding="utf-8"))
    constructors = [
        symbol
        for symbol in symbols.get("symbols", [])
        if isinstance(symbol, dict)
        and symbol.get("kind") == "method"
        and str(symbol.get("sourceSymbol", "")).startswith("compiler.constructor.")
        and (
            ".BorrowedScenarioReader(" in str(symbol.get("sourceSymbol", ""))
            or ".RetainedScenarioReader(" in str(symbol.get("sourceSymbol", ""))
        )
    ]
    nominal_constructors = [
        symbol
        for symbol in constructors
        if isinstance(symbol.get("overloadSignature"), list)
        and any(
            str(value).startswith("string-utf8-static-view:")
            and str(value).endswith(".ScenarioId")
            for value in symbol["overloadSignature"]
        )
    ]
    if len(nominal_constructors) != 2:
        raise ConstructorLoweringFailure(
            "nominal String fixture lost its two exact constructor symbol identities"
        )
    parameter_spelling = "hxc_string hxc_id"
    if (
        source.count(parameter_spelling) < 2
        or parameter_spelling not in headers
        or "memcmp(" not in source
        or "(*hxc_self).hxc_id = hxc_id;" not in source
    ):
        raise ConstructorLoweringFailure(
            "nominal String constructor lost its by-value call, retained field, or comparison"
        )
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("features") != ["runtime-base", "string-literal"]
        or plan.get("symbols") != []
        or "direct-utf8-string-literals" not in plan.get("directDecisions", [])
    ):
        raise ConstructorLoweringFailure(
            "literal-backed nominal String constructor lost its header-only runtime plan"
        )
    for forbidden in (
        "malloc(",
        "calloc(",
        "realloc(",
        "retain(",
        "release(",
        "goto ",
    ):
        if forbidden in source.lower():
            raise ConstructorLoweringFailure(
                f"nominal String constructor emitted forbidden shape {forbidden!r}"
            )


def validate_enum_parameter_project(output: Path) -> None:
    """Prove fieldless enum constructor values stay nominal and runtime-free."""

    source = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    symbols = json.loads((output / "hxc.symbols.json").read_text(encoding="utf-8"))
    constructors = [
        symbol
        for symbol in symbols.get("symbols", [])
        if isinstance(symbol, dict)
        and symbol.get("kind") == "method"
        and str(symbol.get("sourceSymbol", "")).startswith("compiler.constructor.")
        and (
            ".BorrowedObjectiveReader(" in str(symbol.get("sourceSymbol", ""))
            or ".RetainedObjectiveReader(" in str(symbol.get("sourceSymbol", ""))
        )
    ]
    direct_constructors = [
        symbol
        for symbol in constructors
        if isinstance(symbol.get("overloadSignature"), list)
        and any(
            str(value).startswith("direct-enum:instance.enum.")
            for value in symbol["overloadSignature"]
        )
    ]
    if len(direct_constructors) != 2:
        raise ConstructorLoweringFailure(
            "fieldless enum fixture lost its two exact constructor symbol identities"
        )
    parameter_spelling = "enum hxc_Main_ObjectiveState hxc_state"
    if (
        source.count(parameter_spelling) < 2
        or parameter_spelling not in headers
        or "hxc_state == hxc_Main_ObjectiveState_Active" not in source
        or "(*hxc_self).hxc_state = hxc_state;" not in source
    ):
        raise ConstructorLoweringFailure(
            "fieldless enum constructor lost its by-value call, comparison, or final-field store"
        )
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("features") != []
        or plan.get("status") != "analyzed-runtime-free"
        or "bounded-haxe-enum-values" not in plan.get("directDecisions", [])
    ):
        raise ConstructorLoweringFailure(
            "fieldless enum constructor lost its exact runtime-free plan"
        )
    for forbidden in (
        "hxrt",
        "malloc(",
        "calloc(",
        "realloc(",
        "retain(",
        "release(",
        "goto ",
    ):
        if forbidden in (source + headers).lower():
            raise ConstructorLoweringFailure(
                f"fieldless enum constructor emitted forbidden shape {forbidden!r}"
            )


def validate_enum_payload_parameter_project(output: Path) -> None:
    """Prove unmanaged payload enums keep exact tags, payloads, and value copies."""

    source = "\n".join(
        path.read_text(encoding="utf-8") for path in sorted((output / "src").rglob("*.c"))
    )
    headers = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "include").rglob("*.h"))
    )
    symbols = json.loads((output / "hxc.symbols.json").read_text(encoding="utf-8"))
    constructors = [
        symbol
        for symbol in symbols.get("symbols", [])
        if isinstance(symbol, dict)
        and symbol.get("kind") == "method"
        and str(symbol.get("sourceSymbol", "")).startswith("compiler.constructor.")
        and (
            ".BorrowedFlowReader(" in str(symbol.get("sourceSymbol", ""))
            or ".RetainedFlowReader(" in str(symbol.get("sourceSymbol", ""))
        )
    ]
    payload_constructors = [
        symbol
        for symbol in constructors
        if isinstance(symbol.get("overloadSignature"), list)
        and any(
            str(value).startswith("unmanaged-payload-enum:instance.enum.")
            for value in symbol["overloadSignature"]
        )
    ]
    if len(payload_constructors) != 2:
        raise ConstructorLoweringFailure(
            "unmanaged payload-enum fixture lost its two exact constructor identities"
        )
    parameter_spelling = "struct hxc_Main_FlowValue hxc_value"
    if (
        source.count(parameter_spelling) < 2
        or parameter_spelling not in headers
        or "switch (hxc_value.hxc_tag)" not in source
        or "(*hxc_self).hxc_value = hxc_value;" not in source
        or "hxc_Main_FlowValue_Flag" not in source
        or "hxc_Main_FlowValue_Counter" not in source
        or "hxc_Main_FlowValue_State" not in source
    ):
        raise ConstructorLoweringFailure(
            "unmanaged payload-enum constructor lost its by-value call, active tags, or final-field store"
        )
    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("features") != ["runtime-base", "string-literal"]
        or plan.get("symbols") != []
        or "bounded-haxe-enum-values" not in plan.get("directDecisions", [])
    ):
        raise ConstructorLoweringFailure(
            "unmanaged payload-enum constructor lost its exact allocation-free plan"
        )
    for forbidden in (
        "malloc(",
        "calloc(",
        "realloc(",
        "retain(",
        "release(",
        "goto ",
    ):
        if forbidden in source.lower():
            raise ConstructorLoweringFailure(
                f"unmanaged payload-enum constructor emitted forbidden shape {forbidden!r}"
            )


def render_parameter_server_pair(
    root: Path, fixture: Path, slug: str
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
        deadline = time.monotonic() + 10.0
        while time.monotonic() < deadline:
            if server.poll() is not None:
                stdout, stderr = server.communicate()
                raise ConstructorLoweringFailure(
                    "Haxe server exited before constructor determinism requests: "
                    f"{stdout!r} {stderr!r}"
                )
            try:
                with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                    break
            except OSError:
                time.sleep(0.05)
        else:
            raise ConstructorLoweringFailure(
                "Haxe server did not accept constructor determinism requests"
            )

        outputs = (root / f"{slug}-server-first", root / f"{slug}-server-second")
        for label, output in zip(("first", "second"), outputs):
            result = custom_target(
                fixture,
                output,
                layout="split",
                connect=endpoint,
                runtime_diagnostics="off",
            )
            if result.returncode != 0 or result.stdout or result.stderr:
                raise ConstructorLoweringFailure(
                    f"{label} warm-server {slug} constructor compile failed\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
        return outputs
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def render_parameter_projects(
    fixture_root: Path,
    *,
    fixture: Path,
    slug: str,
    coverage: frozenset[str],
    validate_project: Callable[[Path], None],
) -> tuple[CFixtureProject, ...]:
    projects: list[CFixtureProject] = []
    split_tree: dict[str, bytes] | None = None
    for layout in ("split", "package", "unity"):
        normal = fixture_root / f"{slug}-{layout}"
        reverse = fixture_root / f"{slug}-{layout}-reverse"
        for label, result in (
            (
                f"{layout} {slug} constructor",
                custom_target(
                    fixture, normal, layout=layout, runtime_diagnostics="off"
                ),
            ),
            (
                f"{layout} reversed {slug} constructor",
                custom_target(
                    fixture,
                    reverse,
                    layout=layout,
                    reverse=True,
                    runtime_diagnostics="off",
                ),
            ),
        ):
            if result.returncode != 0 or result.stdout or result.stderr:
                raise ConstructorLoweringFailure(
                    f"{label} compile failed\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
        if generated_tree(normal) != generated_tree(reverse):
            raise ConstructorLoweringFailure(
                f"{layout} {slug} constructor changed with typed-module order"
            )
        validate_project(normal)
        if layout == "split":
            split_tree = generated_tree(normal)
        projects.append(
            CFixtureProject(
                f"constructor-{slug}-{layout}",
                tuple(
                    path.relative_to(fixture_root).as_posix()
                    for path in sorted(normal.rglob("*.c"))
                ),
                tuple(
                    path.relative_to(fixture_root).as_posix()
                    for path in sorted((normal / "include").rglob("*.h"))
                ),
                tuple(
                    path.relative_to(fixture_root).as_posix()
                    for path in (
                        normal / "include",
                        normal / "runtime" / "include",
                    )
                    if path.is_dir()
                ),
                "",
                tuple(sorted((*coverage, "strict-c11"))),
            )
        )

    server_first, server_second = render_parameter_server_pair(
        fixture_root, fixture, slug
    )
    if (
        split_tree is None
        or generated_tree(server_first) != split_tree
        or generated_tree(server_second) != split_tree
    ):
        raise ConstructorLoweringFailure(
            f"{slug} constructor output changed under warm compiler-server reuse"
        )
    return tuple(projects)


def render_direct_receiver_failure_project(
    fixture_root: Path,
) -> CFixtureProject:
    """Render the fail-closed constructor path once with order determinism."""

    normal = fixture_root / "direct-receiver-failure-split"
    reverse = fixture_root / "direct-receiver-failure-split-reverse"
    for label, result in (
        (
            "direct-receiver failure",
            custom_target(
                DIRECT_RECEIVER_FAILURE,
                normal,
                layout="split",
                runtime_diagnostics="off",
            ),
        ),
        (
            "reversed direct-receiver failure",
            custom_target(
                DIRECT_RECEIVER_FAILURE,
                reverse,
                layout="split",
                reverse=True,
                runtime_diagnostics="off",
            ),
        ),
    ):
        if result.returncode != 0 or result.stdout or result.stderr:
            raise ConstructorLoweringFailure(
                f"{label} compile failed\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
    if generated_tree(normal) != generated_tree(reverse):
        raise ConstructorLoweringFailure(
            "direct-receiver failure output changed with typed-module order"
        )
    validate_direct_receiver_failure_project(normal)
    return CFixtureProject(
        "constructor-direct-receiver-failure",
        tuple(
            path.relative_to(fixture_root).as_posix()
            for path in sorted(normal.rglob("*.c"))
        ),
        tuple(
            path.relative_to(fixture_root).as_posix()
            for path in sorted((normal / "include").rglob("*.h"))
        ),
        tuple(
            path.relative_to(fixture_root).as_posix()
            for path in (
                normal / "include",
                normal / "runtime" / "include",
            )
            if path.is_dir()
        ),
        "",
        tuple(
            sorted(
                (*DIRECT_RECEIVER_FAILURE_NATIVE_COVERAGE, "strict-c11")
            )
        ),
        expected_exit=-signal.SIGABRT,
    )


def required_text(value: object, label: str) -> str:
    if not isinstance(value, str):
        raise ConstructorLoweringFailure(f"{label} must be text")
    return value


def required_objects(value: object, label: str) -> list[dict[str, object]]:
    if not isinstance(value, list) or not all(isinstance(item, dict) for item in value):
        raise ConstructorLoweringFailure(f"{label} must be an object array")
    return value


def extract_report(
    result: subprocess.CompletedProcess[str], output: Path, label: str
) -> tuple[str, dict[str, object]]:
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if result.returncode != 0 or result.stderr or len(lines) != 1:
        raise ConstructorLoweringFailure(
            f"{label} emitted an invalid constructor report\n"
            f"exit={result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    other_lines = [line for line in result.stdout.splitlines() if not line.startswith(REPORT_PREFIX)]
    if other_lines:
        raise ConstructorLoweringFailure(f"{label} emitted unexpected stdout: {other_lines!r}")
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise ConstructorLoweringFailure(f"{label} report is not an object")
    if generated_files(output) != PRODUCTION_FILES:
        raise ConstructorLoweringFailure(
            f"{label} artifact set drifted: {sorted(generated_files(output))!r}"
        )
    report["header"] = (output / "include/hxc/program.h").read_text(encoding="utf-8")
    report["source"] = (output / "src/program.c").read_text(encoding="utf-8")
    report["symbols"] = json.loads(
        (output / "hxc.symbols.json").read_text(encoding="utf-8")
    )
    report["runtimePlan"] = json.loads(
        (output / "hxc.runtime-plan.json").read_text(encoding="utf-8")
    )
    report["manifest"] = json.loads(
        (output / "hxc.manifest.json").read_text(encoding="utf-8")
    )
    report["abi"] = json.loads((output / "hxc.abi.json").read_text(encoding="utf-8"))
    return payload, report


def render(
    label: str,
    *,
    reverse: bool = False,
    profile: str = "portable",
    runtime: str | None = None,
) -> tuple[str, dict[str, object]]:
    with tempfile.TemporaryDirectory(prefix="hxc-constructor-render-") as temporary:
        output = Path(temporary) / "generated"
        result = custom_target(
            POSITIVE,
            output,
            profile=profile,
            runtime=runtime,
            reverse=reverse,
            report=True,
        )
        return extract_report(result, output, label)


def function_section(hxcir: str, identifier: str) -> str:
    start_marker = f'  function "{identifier}"'
    end_marker = f'  end function "{identifier}"'
    start = hxcir.find(start_marker)
    end = hxcir.find(end_marker, start)
    if start == -1 or end == -1:
        raise ConstructorLoweringFailure(f"HxcIR omitted function {identifier}")
    return hxcir[start : end + len(end_marker)]


def ordered(section: str, markers: tuple[str, ...], label: str) -> None:
    offset = -1
    for marker in markers:
        next_offset = section.find(marker, offset + 1)
        if next_offset == -1:
            raise ConstructorLoweringFailure(f"{label} omitted {marker!r}")
        if next_offset <= offset:
            raise ConstructorLoweringFailure(f"{label} reordered {marker!r}")
        offset = next_offset


def constructor_map(report: dict[str, object]) -> dict[str, dict[str, object]]:
    records = required_objects(report.get("constructors"), "constructors")
    identifiers = [record.get("id") for record in records]
    if not all(isinstance(identifier, str) for identifier in identifiers):
        raise ConstructorLoweringFailure("constructor records omitted stable IDs")
    if identifiers != sorted(identifiers, key=lambda value: value.encode("utf-8")):
        raise ConstructorLoweringFailure("constructor records are not UTF-8 ordered")
    result = {str(record["id"]): record for record in records}
    if set(result) != set(EXPECTED_CONSTRUCTORS):
        raise ConstructorLoweringFailure(
            f"reachable constructor set drifted: {sorted(result)!r}"
        )
    return result


def validate_runtime_plan(report: dict[str, object], *, profile: str) -> None:
    plan = report.get("runtimePlan")
    manifest = report.get("manifest")
    abi = report.get("abi")
    if not isinstance(plan, dict) or not isinstance(manifest, dict) or not isinstance(abi, dict):
        raise ConstructorLoweringFailure("production report omitted plan, manifest, or ABI")
    proof = plan.get("noRuntimeProof")
    reachability = proof.get("reachability") if isinstance(proof, dict) else None
    if (
        plan.get("schemaVersion") != 2
        or plan.get("status") != "analyzed-runtime-free"
        or plan.get("profile") != profile
        or plan.get("features") != []
        or plan.get("artifacts") != []
        or "bounded-stack-construction" not in plan.get("directDecisions", [])
        or not isinstance(proof, dict)
        or proof.get("status") != "eligible"
        or "compiler-proven stack construction" not in str(proof.get("semanticProof"))
        or not isinstance(reachability, dict)
        or reachability.get("cleanupActions", 0) < 8
        or manifest.get("compilationStatus") != "lowered-direct-value-executable"
        or manifest.get("configuration", {}).get("profile") != profile
        or abi.get("status") != "analyzed-no-public-exports"
        or abi.get("exports") != []
        or abi.get("types") != []
    ):
        raise ConstructorLoweringFailure("constructor runtime-free production contract drifted")


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if report.get("schemaVersion") != 1 or report.get("profile") != profile:
        raise ConstructorLoweringFailure(f"constructor report contract drifted for {profile}")
    constructors = constructor_map(report)
    for identifier, (elided, can_fail) in EXPECTED_CONSTRUCTORS.items():
        record = constructors[identifier]
        c_name = record.get("cName")
        if (
            record.get("elided") is not elided
            or record.get("canFail") is not can_fail
            or not isinstance(record.get("haxePath"), str)
            or not isinstance(record.get("instanceId"), str)
            or (elided and c_name is not None)
            or (not elided and (not isinstance(c_name, str) or not c_name))
        ):
            raise ConstructorLoweringFailure(f"constructor record drifted: {identifier}")
        if isinstance(c_name, str) and ("::" in c_name or c_name.startswith("new ")):
            raise ConstructorLoweringFailure("constructor escaped ordinary C function naming")

    hxcir = required_text(report.get("hxcir"), "HxcIR")
    header = required_text(report.get("header"), "header")
    source = required_text(report.get("source"), "source")
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v2":
        raise ConstructorLoweringFailure("constructor report omitted finalized symbols")
    for label, text in (("HxcIR", hxcir), ("header", header), ("source", source)):
        if str(ROOT) in text or "\\" in text or "hxrt" in text.lower():
            raise ConstructorLoweringFailure(f"{label} leaked a host path or runtime")
    if not hxcir.startswith("hxcir schema=19\n"):
        raise ConstructorLoweringFailure("constructor lowering did not use schema-19 HxcIR")

    leaf = function_section(hxcir, "constructor.LeafRecord")
    ordered(
        leaf,
        (
            "constant value=int(3)",
            'store place=field(dereference("parameter.self"),"secondLeafField")',
            "constant value=int(2)",
            'store place=field(dereference("parameter.self"),"firstLeafField")',
            "constant value=int(6)",
            'dispatch=direct("constructor.BaseRecord")',
            "constant value=int(7)",
        ),
        "derived constructor",
    )
    base = function_section(hxcir, "constructor.BaseRecord")
    ordered(
        base,
        (
            "constant value=int(4)",
            'store place=field(dereference("parameter.self"),"baseField")',
            "constant value=int(5)",
            'store place=field(dereference("parameter.self"),"defaulted")',
        ),
        "base constructor",
    )
    failable_base = function_section(hxcir, "constructor.FailableBase")
    failable_leaf = function_section(hxcir, "constructor.FailableLeaf")
    counter_owner = function_section(hxcir, "constructor.CounterOwner")
    counter_run = function_section(hxcir, "method.CounterOwner.run")
    drive = function_section(hxcir, "function.Main.drive")
    main = function_section(hxcir, "function.Main.main")
    if drive.count("ownership=borrowed-class") != 1:
        raise ConstructorLoweringFailure(
            "direct helper lost its explicit caller-owned class parameter"
        )
    if (
        "failure=status(exception)" not in failable_base
        or "failure=status(exception)" not in failable_leaf
        or 'edge=failure(kind=exception,target=propagate' not in failable_base
        or 'cleanup=["cleanup.construction"."construction.0.initialized"]'
        not in failable_base
        or 'dispatch=direct("constructor.FailableBase")' not in failable_leaf
        or "target=propagate" not in failable_leaf
    ):
        raise ConstructorLoweringFailure("constructor status/cleanup propagation drifted")
    ordered(
        counter_owner,
        (
            "constant value=int(11)",
            'bind-virtual-table place=field(dereference("parameter.self"),"child")',
            "owned-class-field-address",
            'dispatch=direct("constructor.OwnedCounter")',
            'dispatch=virtual(slot="slot.OwnedCounter.add"',
            'store place=field(dereference("parameter.self"),"observedDuringConstruction")',
        ),
        "owned child constructor",
    )
    ordered(
        counter_run,
        (
            "owned-class-field-borrow",
            "compound-load",
            "compound-store",
            "class-field-load",
        ),
        "inlined owned child method",
    )
    if (
        "ownership=borrowed-class storage=automatic" not in counter_run
        or "initialize place=local" not in counter_run
        or counter_run.count("load place=local") < 2
        or "terminator branch" not in counter_run
    ):
        raise ConstructorLoweringFailure(
            "inlined owned-child receiver lost its explicit non-owning local alias"
        )
    ordered(
        main,
        (
            "constant value=int(1)",
            'dispatch=direct("function.ConstructorTrace.mark")',
            "class-default-initialize",
            'dispatch=direct("constructor.LeafRecord")',
            "constructor-complete",
        ),
        "construction call site",
    )
    if (
        'transition=uninitialized->initializing' not in main
        or 'transition=initializing->initialized reason="constructor completed"' not in main
        or 'transition=initializing->destroyed' not in main
        or 'transition=initialized->destroyed' not in main
        or 'target=abort' not in main
        or '"construction.2.partial"' not in main
        or '"construction.1.initialized"' not in main
        or '"construction.0.initialized"' not in main
    ):
        raise ConstructorLoweringFailure("partial-construction cleanup order drifted")

    emitted_names = {
        identifier: record.get("cName") for identifier, record in constructors.items()
    }
    for identifier in ("constructor.EmptyBase", "constructor.EmptyLeaf"):
        if emitted_names[identifier] is not None:
            raise ConstructorLoweringFailure(f"{identifier} was not elided")
    for identifier in (
        "constructor.BaseRecord",
        "constructor.CounterOwner",
        "constructor.FailableBase",
        "constructor.FailableLeaf",
        "constructor.LeafRecord",
        "constructor.OwnedCounter",
    ):
        name = emitted_names[identifier]
        if not isinstance(name, str) or f"{name}(" not in header or f"{name}(" not in source:
            raise ConstructorLoweringFailure(f"emitted constructor missing: {identifier}")
    child_definition = header.find("struct hxc_OwnedCounter {")
    parent_definition = header.find("struct hxc_CounterOwner {")
    if (
        child_definition == -1
        or parent_definition == -1
        or child_definition >= parent_definition
        or "struct hxc_OwnedCounter hxc_child;" not in header
        or "struct hxc_OwnedCounter *hxc_child;" in header
    ):
        raise ConstructorLoweringFailure(
            "owned child did not remain an inline, dependency-ordered class subobject"
        )
    child_bind = source.find(
        "hxc_child.hxc_vtable = &hxc_vtable_compiler_virtual_dispatch_OwnedCounter"
    )
    child_call = source.find("hxc_compiler_constructor_OwnedCounter(")
    if child_bind == -1 or child_call == -1 or child_bind >= child_call:
        raise ConstructorLoweringFailure(
            "owned child virtual identity was not bound before construction"
        )
    if (
        " = { 0 };" not in source
        or "if (!" not in source
        or "abort();" not in source
        or "bool " not in header
        or "int main(void)" not in source
    ):
        raise ConstructorLoweringFailure("structural constructor C emission drifted")
    for forbidden in ("goto ", "malloc(", "calloc(", "realloc(", "free("):
        if forbidden in source:
            raise ConstructorLoweringFailure(
                f"constructor fixture unexpectedly emitted {forbidden.strip()!r}"
            )
    validate_runtime_plan(report, profile=profile)


def normalized_profile(report: dict[str, object]) -> dict[str, object]:
    value = copy.deepcopy(report)
    value.pop("runtimePlan", None)
    value.pop("manifest", None)
    value.pop("abi", None)
    value["profile"] = "<profile>"
    hxcir = value.get("hxcir")
    if isinstance(hxcir, str):
        value["hxcir"] = hxcir.replace(
            'profile="portable"', 'profile="<profile>"'
        ).replace('profile="metal"', 'profile="<profile>"')
    return value


def snapshot_values(report: dict[str, object]) -> dict[str, object]:
    return {
        "constructors.hxcir": required_text(report.get("hxcir"), "HxcIR"),
        "constructors.json": required_objects(report.get("constructors"), "constructors"),
        "program.h": required_text(report.get("header"), "header"),
        "program.c": required_text(report.get("source"), "source"),
        "symbols.json": report.get("symbols"),
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


def check_snapshots(report: dict[str, object]) -> None:
    for name, actual in snapshot_values(report).items():
        path = EXPECTED / name
        if not path.is_file():
            raise ConstructorLoweringFailure(
                "constructor snapshots are missing; run "
                "npm run snapshots:update -- --suite constructor-lowering"
            )
        if name.endswith(".json"):
            expected = json.loads(path.read_text(encoding="utf-8"))
            if actual != expected:
                raise ConstructorLoweringFailure(f"{name} semantic snapshot drifted")
        else:
            actual_text = required_text(actual, name)
            expected_text = path.read_text(encoding="utf-8")
            if actual_text != expected_text:
                raise ConstructorLoweringFailure(
                    f"{name} drifted:\n" + difference(expected_text, actual_text, name)
                )


def require_silent_success(
    command: list[str], *, label: str, cwd: Path = ROOT
) -> None:
    result = subprocess.run(
        command,
        cwd=cwd,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ConstructorLoweringFailure(
            f"{label} failed\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def write_generated_fixture(report: dict[str, object], destination: Path) -> None:
    header = destination / "include/hxc/program.h"
    source = destination / "src/program.c"
    header.parent.mkdir(parents=True, exist_ok=True)
    source.parent.mkdir(parents=True, exist_ok=True)
    header.write_text(required_text(report.get("header"), "header"), encoding="utf-8", newline="\n")
    source.write_text(required_text(report.get("source"), "source"), encoding="utf-8", newline="\n")


def compile_failure_fixture(destination: Path) -> None:
    result = custom_target(FAILURE_RUNTIME, destination)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ConstructorLoweringFailure(
            "failure-runtime fixture did not compile cleanly\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def compile_default_fixture(destination: Path) -> None:
    result = custom_target(DEFAULT_RUNTIME, destination)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ConstructorLoweringFailure(
            "default-field runtime fixture did not compile cleanly\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    plan = json.loads((destination / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        "bounded-stack-construction" not in plan.get("directDecisions", [])
        or plan.get("features") != []
    ):
        raise ConstructorLoweringFailure(
            "default-field runtime fixture lost its direct runtime-free plan"
        )


def check_cpp_header(
    fixture_root: Path, build_root: Path, *, requested_toolchain: str
) -> None:
    check_cpp_consumers(
        build_root,
        requested_toolchain=requested_toolchain,
        consumers=(
            (
                "constructor",
                (fixture_root / "positive/include",),
                NATIVE / "constructor_header_cpp.cpp",
            ),
            (
                "direct-receiver",
                (
                    fixture_root / "direct-receiver-split/include",
                    fixture_root / "direct-receiver-split/runtime/include",
                ),
                NATIVE / "direct_receiver_header_cpp.cpp",
            ),
        ),
    )


def check_cpp_consumers(
    build_root: Path,
    *,
    requested_toolchain: str,
    consumers: tuple[tuple[str, tuple[Path, ...], Path], ...],
) -> None:
    """Compile one or more generated private headers as strict C++17."""

    for toolchain in resolve_toolchains(requested_toolchain, repository_root=ROOT):
        command_name = CXX_COMMANDS[toolchain.family]
        compiler = shutil.which(command_name)
        if compiler is None:
            if requested_toolchain != "auto":
                raise ConstructorLoweringFailure(
                    f"required C++ companion {command_name!r} is unavailable"
                )
            print(
                f"constructor-lowering: SKIP optional {toolchain.family} C++17 header consumer"
            )
            continue
        for consumer, includes, source in consumers:
            for optimization in ("-O0", "-O2"):
                output = (
                    build_root
                    / toolchain.family
                    / consumer
                    / optimization[1:].lower()
                )
                output.mkdir(parents=True, exist_ok=True)
                require_silent_success(
                    [
                        compiler,
                        *CXX_STRICT_FLAGS,
                        optimization,
                        *(f"-I{include}" for include in includes),
                        "-c",
                        str(source),
                        "-o",
                        str(output / f"{consumer}_header_cpp.o"),
                    ],
                    label=(
                        f"{toolchain.family} {optimization} {consumer} "
                        "C++17 header"
                    ),
                )


def check_native(
    report: dict[str, object],
    *,
    requested_toolchain: str = "auto",
    render_parameter_fixtures: bool = True,
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-constructor-native-") as temporary:
        root = Path(temporary)
        fixture_root = root / "fixture"
        write_generated_fixture(report, fixture_root / "positive")
        compile_failure_fixture(fixture_root / "failure")
        compile_default_fixture(fixture_root / "defaults")
        projects = [
            CFixtureProject(
                "constructor-default-fields",
                ("defaults/src/program.c",),
                ("defaults/include/hxc/program.h",),
                ("defaults/include",),
                "",
                ("constructor-default-field-values", "strict-c11"),
            ),
            CFixtureProject(
                "constructor-failure",
                ("failure/src/program.c",),
                ("failure/include/hxc/program.h",),
                ("failure/include",),
                "",
                ("constructor-cleanup-failure", "strict-c11"),
                expected_exit=-signal.SIGABRT,
            ),
            CFixtureProject(
                "constructor-positive",
                ("positive/src/program.c",),
                ("positive/include/hxc/program.h",),
                ("positive/include",),
                "",
                (
                    "constructor-generated-executable",
                    "constructor-runtime-free",
                    "field-initialization-order",
                    "owned-child-inline-layout",
                    "owned-child-inline-method-borrow",
                    "owned-child-lazy-borrow-alias",
                    "owned-child-stable-identity",
                    "super-constructor-order",
                    "trivial-constructor-elision",
                ),
            ),
        ]
        parameter_projects: tuple[CFixtureProject, ...] = ()
        if render_parameter_fixtures:
            record_projects = render_parameter_projects(
                fixture_root,
                fixture=RECORD_PARAMETER,
                slug="record-parameter",
                coverage=RECORD_NATIVE_COVERAGE,
                validate_project=validate_record_parameter_project,
            )
            interface_projects = render_parameter_projects(
                fixture_root,
                fixture=INTERFACE_PARAMETER,
                slug="interface-parameter",
                coverage=INTERFACE_NATIVE_COVERAGE,
                validate_project=validate_interface_parameter_project,
            )
            retained_interface_projects = render_parameter_projects(
                fixture_root,
                fixture=RETAINED_INTERFACE_PARAMETER,
                slug="retained-interface-parameter",
                coverage=RETAINED_INTERFACE_NATIVE_COVERAGE,
                validate_project=validate_retained_interface_project,
            )
            default_argument_projects = render_parameter_projects(
                fixture_root,
                fixture=DEFAULT_ARGUMENTS,
                slug="default-arguments",
                coverage=DEFAULT_ARGUMENT_NATIVE_COVERAGE,
                validate_project=validate_default_argument_project,
            )
            array_parameter_projects = render_parameter_projects(
                fixture_root,
                fixture=ARRAY_PARAMETER,
                slug="array-parameter",
                coverage=ARRAY_PARAMETER_NATIVE_COVERAGE,
                validate_project=validate_array_parameter_project,
            )
            string_parameter_projects = render_parameter_projects(
                fixture_root,
                fixture=STRING_PARAMETER,
                slug="string-parameter",
                coverage=STRING_PARAMETER_NATIVE_COVERAGE,
                validate_project=validate_string_parameter_project,
            )
            enum_parameter_projects = render_parameter_projects(
                fixture_root,
                fixture=ENUM_PARAMETER,
                slug="enum-parameter",
                coverage=ENUM_PARAMETER_NATIVE_COVERAGE,
                validate_project=validate_enum_parameter_project,
            )
            enum_payload_parameter_projects = render_parameter_projects(
                fixture_root,
                fixture=ENUM_PAYLOAD_PARAMETER,
                slug="enum-payload-parameter",
                coverage=ENUM_PAYLOAD_PARAMETER_NATIVE_COVERAGE,
                validate_project=validate_enum_payload_parameter_project,
            )
            direct_receiver_projects = render_parameter_projects(
                fixture_root,
                fixture=DIRECT_RECEIVER,
                slug="direct-receiver",
                coverage=DIRECT_RECEIVER_NATIVE_COVERAGE,
                validate_project=validate_direct_receiver_project,
            )
            direct_receiver_failure_project = (
                render_direct_receiver_failure_project(fixture_root)
            )
            parameter_projects = (
                record_projects
                + interface_projects
                + retained_interface_projects
                + default_argument_projects
                + array_parameter_projects
                + string_parameter_projects
                + enum_parameter_projects
                + enum_payload_parameter_projects
                + direct_receiver_projects
                + (direct_receiver_failure_project,)
            )
            projects.extend(parameter_projects)
        ordered_projects = tuple(
            sorted(projects, key=lambda project: project.identifier.encode("utf-8"))
        )
        for optimization in ("-O0", "-O2"):
            native_report = run_c_fixture_corpus(
                suite=f"constructor-lowering-{optimization[1:].lower()}",
                projects=ordered_projects,
                fixture_root=fixture_root,
                build_root=root / f"c-build-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            required_coverage = REQUIRED_NATIVE_COVERAGE
            if render_parameter_fixtures:
                required_coverage = (
                    required_coverage
                    | RECORD_NATIVE_COVERAGE
                    | INTERFACE_NATIVE_COVERAGE
                    | RETAINED_INTERFACE_NATIVE_COVERAGE
                    | DEFAULT_ARGUMENT_NATIVE_COVERAGE
                    | ARRAY_PARAMETER_NATIVE_COVERAGE
                    | STRING_PARAMETER_NATIVE_COVERAGE
                    | ENUM_PARAMETER_NATIVE_COVERAGE
                    | ENUM_PAYLOAD_PARAMETER_NATIVE_COVERAGE
                    | DIRECT_RECEIVER_NATIVE_COVERAGE
                    | DIRECT_RECEIVER_FAILURE_NATIVE_COVERAGE
                )
            validate_report(native_report, required_coverage=required_coverage)
            encoded = report_json(native_report, compact=True)
            for forbidden in (str(ROOT), str(fixture_root), str(root)):
                if forbidden in encoded:
                    raise ConstructorLoweringFailure(
                        f"native report leaked absolute path {forbidden}"
                    )
        available_families = {
            toolchain.family
            for toolchain in resolve_toolchains(
                requested_toolchain, repository_root=ROOT
            )
        }
        if parameter_projects and "clang" in available_families:
            sanitized_projects = tuple(
                replace(
                    project,
                    link_arguments=("-fsanitize=address,undefined",),
                )
                for project in sorted(
                    parameter_projects,
                    key=lambda project: project.identifier.encode("utf-8"),
                )
            )
            sanitizer_report = run_c_fixture_corpus(
                suite="constructor-parameter-families-sanitized",
                projects=sanitized_projects,
                fixture_root=fixture_root,
                build_root=root / "c-build-sanitized",
                repository_root=ROOT,
                requested_toolchain="clang",
                strict_flags=(*C11_STRICT_FLAGS, *SANITIZER_FLAGS),
            )
            validate_report(
                sanitizer_report,
                required_coverage=(
                    RECORD_NATIVE_COVERAGE
                    | INTERFACE_NATIVE_COVERAGE
                    | RETAINED_INTERFACE_NATIVE_COVERAGE
                    | DEFAULT_ARGUMENT_NATIVE_COVERAGE
                    | ARRAY_PARAMETER_NATIVE_COVERAGE
                    | STRING_PARAMETER_NATIVE_COVERAGE
                    | ENUM_PARAMETER_NATIVE_COVERAGE
                    | ENUM_PAYLOAD_PARAMETER_NATIVE_COVERAGE
                    | DIRECT_RECEIVER_NATIVE_COVERAGE
                    | DIRECT_RECEIVER_FAILURE_NATIVE_COVERAGE
                ),
            )
        check_cpp_header(
            fixture_root, root / "cxx-build", requested_toolchain=requested_toolchain
        )


def check_direct_receiver_oracles() -> None:
    """Compare the successful and throwing receiver paths with Haxe Eval."""

    success = subprocess.run(
        [
            development_tool("haxe"),
            "-cp",
            str(DIRECT_RECEIVER),
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
    if success.returncode != 0 or success.stdout or success.stderr:
        raise ConstructorLoweringFailure(
            "pinned Haxe direct-receiver oracle failed\n"
            f"stdout:\n{success.stdout}\nstderr:\n{success.stderr}"
        )
    failure = subprocess.run(
        [
            development_tool("haxe"),
            "-cp",
            str(DIRECT_RECEIVER_FAILURE),
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
    if (
        failure.returncode == 0
        or failure.stdout
        or "Uncaught exception 99" not in failure.stderr
    ):
        raise ConstructorLoweringFailure(
            "pinned Haxe direct-receiver failure oracle did not throw 99\n"
            f"stdout:\n{failure.stdout}\nstderr:\n{failure.stderr}"
        )


def check_eval_oracle() -> None:
    for label, fixture in (
        ("constructor oracle", ORACLE),
        ("record-parameter oracle", RECORD_PARAMETER),
        ("interface-parameter oracle", INTERFACE_PARAMETER),
        ("default-argument oracle", DEFAULT_ARGUMENTS),
        ("array-parameter oracle", ARRAY_PARAMETER),
        ("string-parameter oracle", STRING_PARAMETER),
        ("enum-parameter oracle", ENUM_PARAMETER),
        ("enum-payload-parameter oracle", ENUM_PAYLOAD_PARAMETER),
    ):
        result = subprocess.run(
            [
                development_tool("haxe"),
                "-cp",
                str(fixture),
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
        if result.returncode != 0 or result.stdout or result.stderr:
            raise ConstructorLoweringFailure(
                f"pinned Haxe {label} failed\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
    check_direct_receiver_oracles()


def check_direct_receiver_only(*, requested_toolchain: str) -> None:
    """Run the complete direct-receiver slice without unrelated constructors."""

    check_direct_receiver_oracles()
    with tempfile.TemporaryDirectory(
        prefix="hxc-direct-receiver-focused-"
    ) as temporary:
        root = Path(temporary)
        fixture_root = root / "fixture"
        projects = render_parameter_projects(
            fixture_root,
            fixture=DIRECT_RECEIVER,
            slug="direct-receiver",
            coverage=DIRECT_RECEIVER_NATIVE_COVERAGE,
            validate_project=validate_direct_receiver_project,
        )
        failure_project = render_direct_receiver_failure_project(fixture_root)
        ordered_projects = tuple(
            sorted(
                (*projects, failure_project),
                key=lambda project: project.identifier.encode("utf-8"),
            )
        )
        required_coverage = (
            DIRECT_RECEIVER_NATIVE_COVERAGE
            | DIRECT_RECEIVER_FAILURE_NATIVE_COVERAGE
            | {"strict-c11"}
        )
        for optimization in ("-O0", "-O2"):
            report = run_c_fixture_corpus(
                suite=f"constructor-direct-receiver-{optimization[1:].lower()}",
                projects=ordered_projects,
                fixture_root=fixture_root,
                build_root=root / f"c-build-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            validate_report(report, required_coverage=required_coverage)
        available_families = {
            toolchain.family
            for toolchain in resolve_toolchains(
                requested_toolchain, repository_root=ROOT
            )
        }
        if "clang" in available_families:
            sanitized_projects = tuple(
                replace(
                    project,
                    link_arguments=("-fsanitize=address,undefined",),
                )
                for project in ordered_projects
            )
            report = run_c_fixture_corpus(
                suite="constructor-direct-receiver-sanitized",
                projects=sanitized_projects,
                fixture_root=fixture_root,
                build_root=root / "c-build-sanitized",
                repository_root=ROOT,
                requested_toolchain="clang",
                strict_flags=(*C11_STRICT_FLAGS, *SANITIZER_FLAGS),
            )
            validate_report(report, required_coverage=required_coverage)
        check_cpp_consumers(
            root / "cxx-build",
            requested_toolchain=requested_toolchain,
            consumers=(
                (
                    "direct-receiver",
                    (
                        fixture_root / "direct-receiver-split/include",
                        fixture_root / "direct-receiver-split/runtime/include",
                    ),
                    NATIVE / "direct_receiver_header_cpp.cpp",
                ),
            ),
        )
        escape_output = root / "direct-receiver-escape"
        escape = custom_target(DIRECT_RECEIVER_ESCAPE, escape_output)
        combined = escape.stdout + escape.stderr
        if (
            escape.returncode == 0
            or "HXC1001" not in combined
            or "TVar(escaped:owned-class-borrow-escape)" not in combined
            or generated_files(escape_output)
        ):
            raise ConstructorLoweringFailure(
                "direct receiver escape did not fail closed with no output\n"
                f"stdout:\n{escape.stdout}\nstderr:\n{escape.stderr}"
            )


def check_minimal_example() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-constructor-minimal-") as temporary:
        output = Path(temporary) / "generated"
        result = custom_target(MINIMAL, output)
        if result.returncode != 0 or result.stdout or result.stderr:
            raise ConstructorLoweringFailure(
                "minimal constructor example did not compile cleanly\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        source = (output / "src/program.c").read_text(encoding="utf-8")
        plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
        if (
            "constructor_Item" not in source
            or " = { 0 };" not in source
            or "bounded-stack-construction" not in plan.get("directDecisions", [])
            or plan.get("features") != []
        ):
            raise ConstructorLoweringFailure("minimal constructor example lost direct runtime-free lowering")


def check_negative_cases() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-constructor-negative-") as temporary:
        root = Path(temporary)
        for directory, detail in NEGATIVE_CASES.items():
            output = root / directory
            result = custom_target(FIXTURES / directory, output)
            combined = result.stdout + result.stderr
            if (
                result.returncode == 0
                or "HXC1001" not in combined
                or detail not in combined
                or generated_files(output)
            ):
                raise ConstructorLoweringFailure(
                    f"{directory} did not fail closed with exact HXC1001 and no output\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
        cycle = custom_target(FIXTURES / "cycle", root / "cycle-repeat")
        if "reachable function and constructor graph discovery" not in cycle.stderr:
            raise ConstructorLoweringFailure("constructor-cycle diagnostic lost graph context")


def snapshot_report() -> dict[str, object]:
    return {
        "schemaVersion": 1,
        "profile": "portable",
        "constructors": json.loads(
            (EXPECTED / "constructors.json").read_text(encoding="utf-8")
        ),
        "hxcir": (EXPECTED / "constructors.hxcir").read_text(encoding="utf-8"),
        "header": (EXPECTED / "program.h").read_text(encoding="utf-8"),
        "source": (EXPECTED / "program.c").read_text(encoding="utf-8"),
        "symbols": json.loads((EXPECTED / "symbols.json").read_text(encoding="utf-8")),
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument("--native-only", action="store_true")
    parser.add_argument("--direct-receiver-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("constructor-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            report = snapshot_report()
            check_native(
                report,
                requested_toolchain=args.toolchain,
                render_parameter_fixtures=False,
            )
            print("constructor-lowering: OK: required constructor native matrix passed")
            return 0
        if args.direct_receiver_only:
            check_direct_receiver_only(requested_toolchain=args.toolchain)
            print(
                "constructor-lowering: OK: direct fresh receiver Eval/C11/C++17/"
                "sanitizer/determinism/escape matrix passed"
            )
            return 0

        first_payload, first = render("first constructor render")
        second_payload, second = render("second constructor render")
        reverse_payload, reverse = render("reverse-input constructor render", reverse=True)
        _, metal = render("metal constructor render", profile="metal")
        _, runtime_none = render("runtime-none constructor render", runtime="none")
        if first_payload != second_payload or first != second:
            raise ConstructorLoweringFailure("two constructor renders differed")
        if first_payload != reverse_payload or first != reverse:
            raise ConstructorLoweringFailure("constructor output changed with typed-module order")
        validate(first)
        validate(metal, profile="metal")
        validate(runtime_none)
        if normalized_profile(first) != normalized_profile(metal):
            raise ConstructorLoweringFailure("portable and metal constructor output diverged")
        for key in ("constructors", "hxcir", "header", "source", "symbols"):
            if first.get(key) != runtime_none.get(key):
                raise ConstructorLoweringFailure(
                    f"runtime-none changed constructor compiler artifact {key}"
                )
        check_snapshots(first)
        check_eval_oracle()
        check_minimal_example()
        check_native(first, requested_toolchain=args.toolchain)
        check_negative_cases()
    except (
        ConstructorLoweringFailure,
        CFixtureFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"constructor-lowering: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "constructor-lowering: OK: pinned super/field/body order, default storage, "
        "status cleanup, direct caller-owned class and closed-record parameters, "
        "call-bounded and collector-retained interface parameters and dispatch, "
        "borrowed and field-retained Array parameters, "
        "nominal literal-backed String parameters and final fields, "
        "fieldless enum parameters and final fields, "
        "unmanaged payload-enum parameters and final fields, "
        "fresh automatic method receivers with nested managed arguments, "
        "owned result transfer and throwing-constructor cleanup, "
        "trivial elision, runtime-free strict C11/C++17 consumers, determinism, "
        "and fail-closed borrow/escape/cycle/native-layout/generic/instance-family "
        "edges passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

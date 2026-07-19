#!/usr/bin/env python3
"""Prove bounded deterministic generic specialization through the production C target."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import socket
import subprocess
import tempfile
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
FIXTURES = CASE / "fixtures"
EXPECTED = CASE / "expected/hxc.specializations.json"
SCHEMA = ROOT / "docs/specs/generic-specialization-report.schema.json"
JSON_SCHEMA_DIALECT = "https://json-schema.org/draft/2020-12/schema"
PAYLOAD_KINDS = {
    "public-header",
    "private-header",
    "source",
    "runtime-header",
    "runtime-source",
}
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-pedantic-errors",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)


class GenericSpecializationFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class RenderedProject:
    artifacts: dict[str, bytes]
    report: dict[str, object]
    runtime_plan: dict[str, object]
    manifest: dict[str, object]


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def compile_fixture(
    fixture: str,
    output: Path,
    *,
    reverse: bool = False,
    locale: str = "C",
    connect: str | None = None,
    profile: str = "portable",
    runtime: str | None = None,
) -> subprocess.CompletedProcess[str]:
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
            "--cwd",
            str(FIXTURES / fixture),
            "build.hxml",
            "-D",
            "hxc_runtime_diagnostics=off",
        ]
    )
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    command.extend(["-D", "hxc_project_layout=unity", "--custom-target", f"c={output}"])
    environment = os.environ.copy()
    environment["LC_ALL"] = locale
    if connect is None:
        environment["HAXE_NO_SERVER"] = "1"
    else:
        environment.pop("HAXE_NO_SERVER", None)
    return subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )


def load_json(path: Path, label: str) -> dict[str, object]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise GenericSpecializationFailure(f"cannot read {label}: {error}") from error
    if not isinstance(value, dict):
        raise GenericSpecializationFailure(f"{label} is not a JSON object")
    return value


def validate_schema_document() -> None:
    schema = load_json(SCHEMA, "generic-specialization report schema")
    properties = require_dict(schema.get("properties"), "schema properties")
    version = require_dict(properties.get("schemaVersion"), "schema version")
    algorithm = require_dict(properties.get("algorithm"), "schema algorithm")
    if (
        schema.get("$schema") != JSON_SCHEMA_DIALECT
        or schema.get("$id")
        != "https://reflaxe-c.dev/schemas/generic-specialization-report.schema.json"
        or schema.get("additionalProperties") is not False
        or version.get("const") != 1
        or algorithm.get("const") != "hxc-generic-specialization-v1"
    ):
        raise GenericSpecializationFailure(
            "generic-specialization report schema identity or closed shape drifted"
        )


def read_artifacts(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def render_positive(
    output: Path,
    *,
    reverse: bool = False,
    locale: str = "C",
    connect: str | None = None,
    profile: str = "portable",
    runtime: str | None = None,
) -> RenderedProject:
    result = compile_fixture(
        "positive",
        output,
        reverse=reverse,
        locale=locale,
        connect=connect,
        profile=profile,
        runtime=runtime,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise GenericSpecializationFailure(
            "positive generic compile failed or emitted diagnostics\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    report = load_json(output / "hxc.specializations.json", "specialization report")
    runtime_plan = load_json(output / "hxc.runtime-plan.json", "runtime plan")
    manifest = load_json(output / "hxc.manifest.json", "compiler manifest")
    project = RenderedProject(read_artifacts(output), report, runtime_plan, manifest)
    validate_positive(project)
    return project


def require_list(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise GenericSpecializationFailure(f"{label} is not an array")
    return value


def require_dict(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise GenericSpecializationFailure(f"{label} is not an object")
    return value


def require_text(value: object, label: str) -> str:
    if not isinstance(value, str) or not value:
        raise GenericSpecializationFailure(f"{label} is not non-empty text")
    return value


def source_key(source: dict[str, object]) -> str:
    file = require_text(source.get("file"), "generic source file")
    values = [source.get(name) for name in ("startLine", "startColumn", "endLine", "endColumn")]
    if not all(isinstance(value, int) and value > 0 for value in values):
        raise GenericSpecializationFailure(f"generic source range is malformed: {source!r}")
    if file.startswith("/") or "\\" in file or str(ROOT) in file:
        raise GenericSpecializationFailure(f"generic source path is host-specific: {file!r}")
    return f"{file}:{values[0]}:{values[1]}-{values[2]}:{values[3]}"


def validate_argument(argument: object, expected_parameter: str | None = None) -> str:
    record = require_dict(argument, "generic argument")
    if (
        not isinstance(record.get("parameter"), str)
        or not record["parameter"]
        or expected_parameter is not None
        and record.get("parameter") != expected_parameter
        or record.get("representation") not in ("direct-primitive", "direct-enum")
    ):
        raise GenericSpecializationFailure(f"generic argument drifted: {record!r}")
    return require_text(record.get("key"), "generic argument key")


def emitted_function_definition(
    artifacts: dict[str, bytes], c_name: str
) -> bytes:
    encoded_name = re.escape(c_name.encode("utf-8"))
    pattern = re.compile(
        rb"(?m)^[^\n]*\b" + encoded_name + rb"\([^\n]*\)\n\{"
    )
    matches: list[tuple[str, bytes, re.Match[bytes]]] = []
    for path, content in artifacts.items():
        if not path.endswith(".c"):
            continue
        matches.extend((path, content, match) for match in pattern.finditer(content))
    if len(matches) != 1:
        raise GenericSpecializationFailure(
            f"specialized C definition {c_name!r} appeared {len(matches)} times"
        )
    path, content, match = matches[0]
    depth = 0
    for index in range(match.end() - 1, len(content)):
        byte = content[index]
        if byte == ord("{"):
            depth += 1
        elif byte == ord("}"):
            depth -= 1
            if depth == 0:
                return content[match.start() : index + 1]
    raise GenericSpecializationFailure(
        f"specialized C definition {c_name!r} in {path!r} has no closing brace"
    )


def validate_positive(project: RenderedProject) -> None:
    report = project.report
    if (
        report.get("schemaVersion") != 1
        or report.get("algorithm") != "hxc-generic-specialization-v1"
        or report.get("status") != "analyzed-closed-specializations"
        or report.get("keyEncoding") != "length-prefixed-utf8-full-semantic-key"
        or report.get("compactNameDigest")
        != "sha256-with-full-key-collision-check"
    ):
        raise GenericSpecializationFailure("specialization report contract drifted")

    functions = require_list(
        report.get("functionSpecializations"), "function specializations"
    )
    types = require_list(report.get("typeSpecializations"), "type specializations")
    summary = require_dict(report.get("summary"), "specialization summary")
    limits = require_dict(report.get("limits"), "specialization limits")
    if (
        len(functions) != 10
        or len(types) != 2
        or summary.get("functionSpecializations") != 10
        or summary.get("typeSpecializations") != 2
        or limits.get("maxFunctionSpecializations") != 64
        or limits.get("maxTypeSpecializations") != 64
        or limits.get("maxEstimatedSpecializationCBytes") != 524288
    ):
        raise GenericSpecializationFailure("specialization counts or hard limits drifted")

    function_keys: list[str] = []
    function_bytes = 0
    function_reasons = 0
    recursive = 0
    by_base_and_arguments: dict[tuple[str, tuple[str, ...]], dict[str, object]] = {}
    for value in functions:
        record = require_dict(value, "function specialization")
        key = require_text(record.get("specializationKey"), "function key")
        digest = hashlib.sha256(key.encode("utf-8")).hexdigest()
        c_name = require_text(record.get("cName"), "specialized C name")
        if (
            record.get("semanticDigestSha256") != digest
            or record.get("instanceId") != f"function.specialization.{digest}"
            or not key.startswith("generic-function-v1(")
            or not c_name.startswith("hxc_method_")
        ):
            raise GenericSpecializationFailure(f"function key/name drifted: {record!r}")
        arguments = require_list(record.get("arguments"), "function arguments")
        if not arguments:
            raise GenericSpecializationFailure("function specialization lost its arguments")
        argument_keys = tuple(validate_argument(argument) for argument in arguments)
        base = require_text(record.get("baseFunctionId"), "function base ID")
        by_base_and_arguments[(base, argument_keys)] = record
        reasons = require_list(record.get("reasons"), "function reasons")
        reason_keys: list[str] = []
        for reason in reasons:
            reason_record = require_dict(reason, "function reason")
            caller = require_text(reason_record.get("callerInstanceId"), "reason caller")
            reason_keys.append(
                caller
                + "\x00"
                + source_key(require_dict(reason_record.get("source"), "reason source"))
            )
        if reason_keys != sorted(reason_keys) or len(set(reason_keys)) != len(reason_keys):
            raise GenericSpecializationFailure("function reasons are not sorted and unique")
        cost = require_dict(record.get("codeSize"), "function code-size record")
        if (
            cost.get("metric") != "strict-c11-utf8-function-definition-bytes"
            or not isinstance(cost.get("definitionBytes"), int)
            or int(cost["definitionBytes"]) <= 0
            or not isinstance(cost.get("irBlocks"), int)
            or int(cost["irBlocks"]) <= 0
            or not isinstance(cost.get("irInstructions"), int)
            or int(cost["irInstructions"]) < 0
        ):
            raise GenericSpecializationFailure("function code-size record is malformed")
        emitted_definition = emitted_function_definition(project.artifacts, c_name)
        if (
            cost.get("definitionBytes") != len(emitted_definition)
            or cost.get("definitionSha256")
            != hashlib.sha256(emitted_definition).hexdigest()
        ):
            raise GenericSpecializationFailure(
                f"function code-size record does not address emitted C for {c_name!r}"
            )
        function_bytes += int(cost["definitionBytes"])
        function_reasons += len(reasons)
        recursive += int(record.get("recursive") is True)
        function_keys.append(key)
    if function_keys != sorted(function_keys) or len(set(function_keys)) != len(function_keys):
        raise GenericSpecializationFailure("function specialization keys are not sorted and unique")

    identity_int = by_base_and_arguments.get(("function.Main.identity", ("i32",)))
    identity_float = by_base_and_arguments.get(("function.Main.identity", ("f64",)))
    identity_bool = by_base_and_arguments.get(("function.Main.identity", ("bool",)))
    identity_uint = by_base_and_arguments.get(("function.Main.identity", ("u32",)))
    identity_enums = [
        record
        for (base, arguments), record in by_base_and_arguments.items()
        if base == "function.Main.identity"
        and len(arguments) == 1
        and arguments[0].startswith("enum(")
    ]
    choose_int_float = by_base_and_arguments.get(
        ("function.Main.choose", ("i32", "f64"))
    )
    recursive_int = by_base_and_arguments.get(("function.Main.recursive", ("i32",)))
    if (
        identity_int is None
        or len(require_list(identity_int.get("reasons"), "identity<Int> reasons")) != 3
        or identity_float is None
        or len(require_list(identity_float.get("reasons"), "identity<Float> reasons")) != 3
        or identity_bool is None
        or identity_uint is None
        or choose_int_float is None
        or [
            require_dict(argument, "choose argument").get("parameter")
            for argument in require_list(
                choose_int_float.get("arguments"), "choose arguments"
            )
        ]
        != ["A", "B"]
        or len(identity_enums) != 2
        or any(
            require_dict(
                require_list(record.get("arguments"), "identity<enum> arguments")[0],
                "identity<enum> argument",
            ).get("representation")
            != "direct-enum"
            for record in identity_enums
        )
        or recursive_int is None
        or recursive_int.get("recursive") is not True
        or len(by_base_and_arguments) != 10
    ):
        raise GenericSpecializationFailure(
            "alias sharing, distinct primitive instances, or recursive closure drifted"
        )

    type_keys: list[str] = []
    type_reasons = 0
    types_by_argument: dict[str, dict[str, object]] = {}
    for value in types:
        type_record = require_dict(value, "generic enum specialization")
        type_key = require_text(type_record.get("specializationKey"), "generic enum key")
        type_digest = hashlib.sha256(type_key.encode("utf-8")).hexdigest()
        arguments = require_list(type_record.get("arguments"), "enum arguments")
        if len(arguments) != 1:
            raise GenericSpecializationFailure(
                "generic enum specialization lost its one argument"
            )
        argument_key = validate_argument(arguments[0], "T")
        types_by_argument[argument_key] = type_record
        reasons = require_list(type_record.get("reasons"), "generic enum reasons")
        type_reason_keys = [
            source_key(require_dict(reason, "generic enum reason"))
            for reason in reasons
        ]
        if (
            type_record.get("haxePath") != "GenericBox"
            or type_record.get("representation") != "tagged-union"
            or type_record.get("semanticDigestSha256") != type_digest
            or type_record.get("instanceId") != f"instance.enum.{type_digest}"
            or type_record.get("declarationId") != f"type.enum.{type_digest}"
            or type_reason_keys != sorted(type_reason_keys)
            or len(set(type_reason_keys)) != len(type_reason_keys)
        ):
            raise GenericSpecializationFailure("generic enum specialization drifted")
        type_cost = require_dict(type_record.get("codeSize"), "generic enum cost")
        if (
            type_cost.get("metric") != "typed-enum-layout-structural-units"
            or type_cost.get("constructorCount") != 2
            or type_cost.get("payloadFieldCount") != 1
            or type_cost.get("structuralUnits") != 4
        ):
            raise GenericSpecializationFailure("generic enum structural cost drifted")
        type_keys.append(type_key)
        type_reasons += len(reasons)
        recursive += int(type_record.get("recursive") is True)
    nested_type_arguments = [
        argument for argument in types_by_argument if argument.startswith("enum(")
    ]
    if (
        type_keys != sorted(type_keys)
        or len(set(type_keys)) != len(type_keys)
        or "i32" not in types_by_argument
        or len(nested_type_arguments) != 1
        or len(types_by_argument) != 2
    ):
        raise GenericSpecializationFailure(
            "finite nested generic enum specialization did not remain distinct"
        )
    enum_definition_bytes = summary.get("dependencyClosedEnumDefinitionBytes")

    manifest_artifacts = require_list(
        project.manifest.get("artifacts"), "manifest artifacts"
    )
    payload_paths: list[str] = []
    specialization_addressed = False
    for value in manifest_artifacts:
        artifact = require_dict(value, "manifest artifact")
        path = require_text(artifact.get("path"), "manifest artifact path")
        content = project.artifacts.get(path)
        if content is None or artifact.get("sha256") != hashlib.sha256(content).hexdigest():
            raise GenericSpecializationFailure(f"manifest address drifted for {path!r}")
        if artifact.get("kind") in PAYLOAD_KINDS:
            payload_paths.append(path)
        if path == "hxc.specializations.json" and artifact.get("kind") == "specialization-report":
            specialization_addressed = True
    if not specialization_addressed:
        raise GenericSpecializationFailure("manifest omitted the specialization sidecar")
    payload_bytes = sum(len(project.artifacts[path]) for path in payload_paths)
    if (
        summary.get("mergedFunctionReasons") != function_reasons
        or summary.get("mergedTypeReasons") != type_reasons
        or summary.get("recursiveSpecializations") != recursive
        or summary.get("specializedFunctionDefinitionBytes") != function_bytes
        or not isinstance(enum_definition_bytes, int)
        or enum_definition_bytes <= 0
        or summary.get("estimatedSpecializationCBytes")
        != function_bytes + enum_definition_bytes
        or summary.get("generatedPayloadArtifacts") != len(payload_paths)
        or summary.get("generatedPayloadBytes") != payload_bytes
    ):
        raise GenericSpecializationFailure("specialization reason or code-size totals drifted")

    if (
        project.runtime_plan.get("status") != "analyzed-runtime-free"
        or project.runtime_plan.get("features") != []
        or project.runtime_plan.get("artifacts") != []
        or "closed-generic-specializations"
        not in require_list(project.runtime_plan.get("directDecisions"), "direct decisions")
    ):
        raise GenericSpecializationFailure("closed generics selected runtime machinery")
    for path, content in project.artifacts.items():
        if str(ROOT).encode() in content or b"\\" in content or b"hxrt" in content.lower():
            raise GenericSpecializationFailure(
                f"artifact {path!r} leaked a host path or runtime dependency"
            )


def first_difference(left: bytes, right: bytes) -> int:
    for index, (left_byte, right_byte) in enumerate(zip(left, right)):
        if left_byte != right_byte:
            return index
    return min(len(left), len(right))


def assert_artifacts_equal(
    left: dict[str, bytes], right: dict[str, bytes], label: str
) -> None:
    if left.keys() != right.keys():
        raise GenericSpecializationFailure(
            f"{label} artifact sets differ: {sorted(left)!r} != {sorted(right)!r}"
        )
    for path in left:
        if left[path] != right[path]:
            raise GenericSpecializationFailure(
                f"{label} first differs in {path!r} at byte "
                f"{first_difference(left[path], right[path])}"
            )


def alternate_locale() -> str:
    result = subprocess.run(
        ["locale", "-a"], check=False, capture_output=True, text=True, timeout=10
    )
    available = {line.strip() for line in result.stdout.splitlines()}
    for candidate in ("C.UTF-8", "C.utf8", "en_US.UTF-8", "en_US.utf8"):
        if candidate in available:
            return candidate
    return "C"


def render_snapshot() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-generic-snapshot-") as temporary:
        root = Path(temporary)
        first = render_positive(root / "first")
        repeated = render_positive(root / "repeated")
        reversed_project = render_positive(
            root / "reversed", reverse=True, locale=alternate_locale()
        )
        assert_artifacts_equal(first.artifacts, repeated.artifacts, "repeated cold render")
        assert_artifacts_equal(
            first.artifacts, reversed_project.artifacts, "reverse-order/locale render"
        )
        return first.report


def check_expected_snapshot() -> None:
    actual = render_snapshot()
    expected = load_json(EXPECTED, "expected specialization report")
    if actual != expected:
        raise GenericSpecializationFailure(
            "specialization snapshot drifted; run "
            "`npm run snapshots:update -- --suite generic-specialization`"
        )


NEGATIVE_EXPECTATIONS = {
    "dynamic": (
        "Main.hx:8: characters 3-25",
        "TCall(generic-specialization:function.Main.identity:type-argument:T):dynamic-type-argument",
    ),
    "open": (
        "Main.hx:7: characters 3-13",
        "TCall(generic-specialization:function.Main.phantom:open-type-argument:T)",
    ),
    "budget": (
        "Main.hx:7: characters 3-26",
        "TCall(generic-specialization-budget:64:expanding-or-excessive:function.Main.grow)",
    ),
    "type_budget": (
        "Main.hx:7: characters 2-32",
        "generic-enum-specialization-budget:64:expanding-or-excessive:Growing",
    ),
    "code_size": (
        "Main.hx:6: lines 6-8",
        "generic-specialization-code-size-budget:128-over-1",
    ),
}


def check_negative(fixture: str, *, connect: str | None = None) -> None:
    with tempfile.TemporaryDirectory(prefix=f"hxc-generic-{fixture}-") as temporary:
        output = Path(temporary) / "out"
        result = compile_fixture(fixture, output, connect=connect)
        emitted = [path for path in output.rglob("*") if path.is_file()] if output.exists() else []
    source, detail = NEGATIVE_EXPECTATIONS[fixture]
    combined = result.stdout + result.stderr
    if (
        result.returncode == 0
        or "HXC1001" not in combined
        or source not in combined
        or detail not in combined
        or emitted
    ):
        raise GenericSpecializationFailure(
            f"{fixture} did not fail closed at its exact generic boundary\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}\n"
            f"emitted: {[str(path) for path in emitted]!r}"
        )


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise GenericSpecializationFailure(
                f"Haxe server exited early\nstdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise GenericSpecializationFailure("Haxe server did not accept connections")


def check_server_isolation() -> None:
    port = available_port()
    endpoint = str(port)
    environment = os.environ.copy()
    environment.pop("HAXE_NO_SERVER", None)
    server = subprocess.Popen(
        [development_tool("haxe"), "--wait", endpoint],
        cwd=ROOT,
        env=environment,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_server(server, port)
        with tempfile.TemporaryDirectory(prefix="hxc-generic-server-") as temporary:
            root = Path(temporary)
            first = render_positive(root / "first", connect=endpoint)
            check_negative("dynamic", connect=endpoint)
            repeated = render_positive(root / "repeated", connect=endpoint)
            assert_artifacts_equal(
                first.artifacts, repeated.artifacts, "warm server after rejected request"
            )
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def check_conditional_sidecar_ownership() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-generic-stale-") as temporary:
        output = Path(temporary) / "out"
        render_positive(output)
        result = compile_fixture("plain", output)
        if result.returncode != 0 or result.stdout or result.stderr:
            raise GenericSpecializationFailure(
                "plain replacement compile failed\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        manifest = load_json(output / "hxc.manifest.json", "plain compiler manifest")
        runtime_plan = load_json(output / "hxc.runtime-plan.json", "plain runtime plan")
        artifact_paths = {
            require_text(require_dict(value, "plain artifact").get("path"), "plain path")
            for value in require_list(manifest.get("artifacts"), "plain artifacts")
        }
        if (
            (output / "hxc.specializations.json").exists()
            or "hxc.specializations.json" in artifact_paths
            or "closed-generic-specializations"
            in require_list(runtime_plan.get("directDecisions"), "plain direct decisions")
        ):
            raise GenericSpecializationFailure(
                "non-generic replacement retained stale specialization ownership"
            )


def payload_tree(project: RenderedProject) -> dict[str, bytes]:
    result: dict[str, bytes] = {}
    for value in require_list(project.manifest.get("artifacts"), "manifest artifacts"):
        artifact = require_dict(value, "manifest artifact")
        if artifact.get("kind") not in PAYLOAD_KINDS:
            continue
        path = require_text(artifact.get("path"), "payload path")
        result[path] = project.artifacts[path]
    return result


def check_profile_and_runtime_policy() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-generic-policy-") as temporary:
        root = Path(temporary)
        portable = render_positive(root / "portable")
        metal = render_positive(root / "metal", profile="metal")
        runtime_none = render_positive(root / "none", runtime="none")
    if portable.report != metal.report or portable.report != runtime_none.report:
        raise GenericSpecializationFailure(
            "profile or runtime policy changed the specialization report"
        )
    assert_artifacts_equal(
        payload_tree(portable), payload_tree(metal), "portable/metal payload"
    )
    assert_artifacts_equal(
        payload_tree(portable), payload_tree(runtime_none), "auto/none payload"
    )
    if (
        portable.runtime_plan.get("resolvedPolicy") != "auto"
        or metal.runtime_plan.get("resolvedPolicy") != "minimal"
        or runtime_none.runtime_plan.get("resolvedPolicy") != "none"
    ):
        raise GenericSpecializationFailure(
            "generic specialization runtime-policy provenance drifted"
        )


def compiler_family(command: str) -> str | None:
    result = subprocess.run(
        [command, "--version"], check=False, capture_output=True, text=True, timeout=10
    )
    identity = (result.stdout + result.stderr).lower()
    if result.returncode != 0:
        return None
    if "clang" in identity:
        return "clang"
    if "gcc" in identity or "free software foundation" in identity:
        return "gcc"
    return None


def native_compilers(requested_toolchain: str) -> list[tuple[str, str]]:
    found: list[tuple[str, str]] = []
    families: set[str] = set()
    requested_families = (
        ("gcc", "clang")
        if requested_toolchain == "auto"
        else (requested_toolchain,)
    )
    for requested in requested_families:
        command = shutil.which(requested)
        if command is None:
            if requested_toolchain != "auto":
                raise GenericSpecializationFailure(
                    f"required {requested} command is unavailable"
                )
            print(f"generic-specialization: SKIP optional {requested}: command unavailable")
            continue
        family = compiler_family(command)
        if family != requested:
            if requested_toolchain != "auto":
                raise GenericSpecializationFailure(
                    f"required {requested} command identifies as {family or 'unknown'}"
                )
            print(
                f"generic-specialization: SKIP optional {requested}: "
                f"command identifies as {family or 'unknown'}"
            )
            continue
        if family not in families:
            found.append((family, command))
            families.add(family)
    if not found and requested_toolchain == "auto":
        cc = shutil.which("cc")
        family = None if cc is None else compiler_family(cc)
        if cc is not None and family is not None:
            found.append((family, cc))
    if not found:
        raise GenericSpecializationFailure("no identity-matching C11 compiler is available")
    return found


def check_native(requested_toolchain: str) -> list[str]:
    compilers = native_compilers(requested_toolchain)
    with tempfile.TemporaryDirectory(prefix="hxc-generic-native-") as temporary:
        root = Path(temporary)
        output = root / "generated"
        project = render_positive(output)
        build = require_dict(project.manifest.get("build"), "manifest build plan")
        sources = [
            output / require_text(value, "build source")
            for value in require_list(build.get("sources"), "build sources")
        ]
        includes = [
            output / require_text(value, "include directory")
            for value in require_list(
                build.get("includeDirectories"), "include directories"
            )
        ]
        for family, compiler in compilers:
            for optimization in ("O0", "O2"):
                executable = root / f"program-{family}-{optimization}"
                command = [
                    compiler,
                    *STRICT_FLAGS,
                    f"-{optimization}",
                    *(f"-I{path}" for path in includes),
                    *(str(path) for path in sources),
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
                    raise GenericSpecializationFailure(
                        f"{family} {optimization} rejected generated generic C\n"
                        f"stdout:\n{compiled.stdout}\nstderr:\n{compiled.stderr}"
                    )
                executed = subprocess.run(
                    [str(executable)],
                    cwd=ROOT,
                    check=False,
                    capture_output=True,
                    timeout=10,
                )
                if executed.returncode != 0 or executed.stdout or executed.stderr:
                    raise GenericSpecializationFailure(
                        f"{family} {optimization} generic executable drifted: "
                        f"exit={executed.returncode}, stdout={executed.stdout!r}, "
                        f"stderr={executed.stderr!r}"
                    )
    return [family for family, _ in compilers]


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    try:
        validate_schema_document()
        if args.native_only:
            families = check_native(args.toolchain)
            print(
                "generic-specialization: OK: required "
                f"{'/'.join(families)} closed-generic C11 O0/O2 matrix passed"
            )
            return 0
        check_expected_snapshot()
        for fixture in NEGATIVE_EXPECTATIONS:
            check_negative(fixture)
        check_server_isolation()
        check_conditional_sidecar_ownership()
        check_profile_and_runtime_policy()
        families = check_native(args.toolchain)
    except (
        GenericSpecializationFailure,
        OSError,
        subprocess.TimeoutExpired,
        UnicodeError,
        json.JSONDecodeError,
    ) as error:
        print(f"generic-specialization: ERROR: {error}", file=os.sys.stderr)
        return 1
    print(
        "generic-specialization: OK: closed primitive/function/enum instances, "
        "alias sharing, finite recursion, full-key collision checks, bounded code-size "
        f"reports, exact dynamic/open/function/type-count/code-size HXC1001, stale ownership, profile/policy isolation, and strict "
        f"{'/'.join(families)} C11 O0/O2 passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(os.sys.argv[1:]))

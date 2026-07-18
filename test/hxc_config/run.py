#!/usr/bin/env python3
"""Validate strict hxc.json parsing, precedence, provenance, and path roots."""

from __future__ import annotations

import json
import os
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from pathlib import Path
from typing import Mapping


ROOT = Path(__file__).resolve().parents[2]
SUITE = Path(__file__).resolve().parent
VALID = SUITE / "fixtures/valid/hxc.json"
INVALID = SUITE / "fixtures/invalid"
EXPECTED = SUITE / "expected/effective-config.json"
SCHEMA = ROOT / "schemas/hxc.schema.json"
REPORT_PREFIX = "HXC_CONFIG_REPORT="
CONTRACT_PREFIX = "HXC_CONFIG_CONTRACT="
HOST_PREFIX = "HXC_CONFIG_HOST_PATHS="
SENTINEL = "hxc-config-haxe: OK"


class HxcConfigFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local_name = f"{name}.cmd" if os.name == "nt" else name
    local = ROOT / "node_modules/.bin" / local_name
    return str(local) if local.is_file() else name


def haxe_command(
    mode: str,
    config: Path | None = None,
    *,
    process_cwd: Path | None = None,
    connect: str | None = None,
) -> list[str]:
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
            "--cwd",
            str(ROOT),
            "-cp",
            "src",
            "-cp",
            "test/hxc_config",
            "--run",
            "ConfigProbe",
            mode,
        ]
    )
    if config is not None:
        command.append(str(config.resolve()))
    if process_cwd is not None:
        command.append(str(process_cwd.resolve()))
    return command


def run_probe(
    mode: str,
    config: Path | None = None,
    *,
    process_cwd: Path | None = None,
    connect: str | None = None,
    environment_updates: Mapping[str, str] | None = None,
    expected_code: int = 0,
    label: str,
) -> subprocess.CompletedProcess[str]:
    environment = os.environ.copy()
    if connect is None:
        environment["HAXE_NO_SERVER"] = "1"
    else:
        environment.pop("HAXE_NO_SERVER", None)
    environment["LC_ALL"] = "C"
    if environment_updates is not None:
        environment.update(environment_updates)
    result = subprocess.run(
        haxe_command(
            mode,
            config,
            process_cwd=process_cwd,
            connect=connect,
        ),
        cwd=process_cwd or ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != expected_code:
        raise HxcConfigFailure(
            f"{label} returned {result.returncode}, expected {expected_code}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return result


def extract_payload(
    result: subprocess.CompletedProcess[str], prefix: str, label: str
) -> str:
    if result.stderr:
        raise HxcConfigFailure(f"{label} wrote stderr on success:\n{result.stderr}")
    if result.stdout.count(prefix) != 1 or result.stdout.count(SENTINEL) != 1:
        raise HxcConfigFailure(f"{label} emitted an invalid envelope:\n{result.stdout}")
    return result.stdout.split(prefix, 1)[1].rsplit(f"\n{SENTINEL}", 1)[0]


def extract_json(result: subprocess.CompletedProcess[str], prefix: str, label: str) -> object:
    payload = extract_payload(result, prefix, label)
    try:
        return json.loads(payload)
    except json.JSONDecodeError as error:
        raise HxcConfigFailure(f"{label} emitted invalid JSON: {error}\n{payload}") from error


def canonical_json(value: object) -> bytes:
    return (json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":")) + "\n").encode("utf-8")


def render_snapshot_with_payload(
    *, connect: str | None = None, reverse: bool = False, config: Path = VALID
) -> tuple[dict[str, object], str]:
    mode = "resolve-reversed" if reverse else "resolve"
    result = run_probe(
        mode,
        config,
        process_cwd=ROOT,
        connect=connect,
        label=f"{mode} configuration",
    )
    payload = extract_payload(result, REPORT_PREFIX, f"{mode} configuration")
    try:
        value = json.loads(payload)
    except json.JSONDecodeError as error:
        raise HxcConfigFailure(
            f"{mode} configuration emitted invalid JSON: {error}\n{payload}"
        ) from error
    if not isinstance(value, dict):
        raise HxcConfigFailure("effective configuration report must be an object")
    return value, payload


def render_snapshot(
    *, connect: str | None = None, reverse: bool = False, config: Path = VALID
) -> dict[str, object]:
    value, _ = render_snapshot_with_payload(
        connect=connect, reverse=reverse, config=config
    )
    return value


def parser_contract() -> dict[str, object]:
    result = run_probe("contract", process_cwd=ROOT, label="parser contract")
    value = extract_json(result, CONTRACT_PREFIX, "parser contract")
    if not isinstance(value, dict):
        raise HxcConfigFailure("parser contract must be an object")
    return value


def require_object(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise HxcConfigFailure(f"{label} must be an object")
    return value


def require_string_array(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise HxcConfigFailure(f"{label} must be a string array")
    return list(value)


def check_schema_sync(contract: dict[str, object]) -> None:
    try:
        schema = json.loads(SCHEMA.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise HxcConfigFailure(f"cannot load hxc schema: {error}") from error
    root = require_object(schema, "schema")
    if root.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        raise HxcConfigFailure("hxc schema must use JSON Schema 2020-12")
    if root.get("$id") != contract.get("schemaId"):
        raise HxcConfigFailure("schema $id differs from the typed parser")
    if root.get("additionalProperties") is not False:
        raise HxcConfigFailure("root schema must reject unknown keys")
    if root.get("required") != ["schemaVersion"]:
        raise HxcConfigFailure("schema must require exactly schemaVersion")
    properties = require_object(root.get("properties"), "schema.properties")
    typed_root_keys = require_string_array(contract.get("rootKeys"), "contract.rootKeys")
    if sorted(properties) != typed_root_keys:
        raise HxcConfigFailure(
            f"schema/parser root keys drifted: schema={sorted(properties)!r}, parser={typed_root_keys!r}"
        )
    definitions = require_object(root.get("$defs"), "schema.$defs")
    overlay = require_object(definitions.get("overlay"), "schema.$defs.overlay")
    if overlay.get("additionalProperties") is not False:
        raise HxcConfigFailure("overlay schema must reject unknown keys")
    overlay_properties = require_object(overlay.get("properties"), "overlay.properties")
    typed_overlay_keys = require_string_array(
        contract.get("overlayKeys"), "contract.overlayKeys"
    )
    if sorted(overlay_properties) != typed_overlay_keys:
        raise HxcConfigFailure(
            "schema/parser overlay keys drifted: "
            f"schema={sorted(overlay_properties)!r}, parser={typed_overlay_keys!r}"
        )
    if overlay.get("allOf") != root.get("allOf"):
        raise HxcConfigFailure(
            "root and overlay cross-field JSON Schema constraints drifted"
        )
    cross_field_rules = root.get("allOf")
    if not isinstance(cross_field_rules, list) or len(cross_field_rules) != 2:
        raise HxcConfigFailure(
            "schema must retain both typed early cross-field constraints"
        )
    project_path = require_object(
        definitions.get("projectPath"), "schema.$defs.projectPath"
    )
    project_pattern = project_path.get("pattern")
    if not isinstance(project_pattern, str) or not all(
        fragment in project_pattern
        for fragment in ("[A-Za-z]:", "[/~]", "\\\\", "\\.\\.", "\\u0000")
    ):
        raise HxcConfigFailure(
            "projectPath schema lost a parser-owned portability/path-safety constraint"
        )
    if project_path.get("not") != {"pattern": r"^(?:\.?/)*\.?$"}:
        raise HxcConfigFailure(
            "projectPath schema must reject paths that normalize to the project root"
        )
    hxml_path = require_object(definitions.get("hxmlPath"), "schema.$defs.hxmlPath")
    if hxml_path.get("allOf") != [
        {"$ref": "#/$defs/projectPath"},
        {"pattern": r"\.hxml$"},
    ]:
        raise HxcConfigFailure("hxmlPath schema drifted from the typed suffix rule")
    enums = require_object(contract.get("enums"), "contract.enums")
    for name in (
        "artifact",
        "build",
        "cExtensions",
        "cStandard",
        "environment",
        "profile",
        "runtime",
        "runtimeDiagnostics",
    ):
        schema_enum = require_string_array(
            require_object(definitions.get(name), f"schema.$defs.{name}").get("enum"),
            f"schema.$defs.{name}.enum",
        )
        typed_enum = require_string_array(enums.get(name), f"contract.enums.{name}")
        if schema_enum != typed_enum or schema_enum != sorted(schema_enum):
            raise HxcConfigFailure(
                f"schema/parser {name} enum drifted or is not sorted: {schema_enum!r} != {typed_enum!r}"
            )
    examples = root.get("examples")
    if not isinstance(examples, list) or len(examples) != 1:
        raise HxcConfigFailure("schema must retain one reviewable valid example")
    if contract.get("schemaVersion") != 1:
        raise HxcConfigFailure("typed parser schemaVersion drifted")


def check_report(report: dict[str, object]) -> None:
    if report.get("schemaVersion") != 1 or report.get("configSchemaVersion") != 1:
        raise HxcConfigFailure("effective report schema versions drifted")
    if report.get("configurationFile") != "hxc.json" or report.get("selectedOverlay") != "dev":
        raise HxcConfigFailure("effective report lost its logical file or selected overlay")
    expected_precedence = [
        "compiler-default",
        "environment-preset",
        "project-file",
        "named-overlay",
        "cli-flag",
        "direct-define",
    ]
    if report.get("precedence") != expected_precedence:
        raise HxcConfigFailure("effective report precedence order drifted")
    effective = require_object(report.get("effective"), "report.effective")
    expected = {
        "hxml": ("haxe/build.hxml", "project-file", "hxc.json", 2),
        "output": ("build/dev", "named-overlay", "dev", 3),
        "profile": ("portable", "cli-flag", "--profile", 4),
        "runtime": ("none", "direct-define", "hxc_runtime", 5),
        "runtimeDiagnostics": ("off", "named-overlay", "dev", 3),
        "environment": ("hosted", "project-file", "hxc.json", 2),
        "cStandard": ("c23", "direct-define", "hxc_c_standard", 5),
        "cExtensions": ("gnu", "environment-preset", "host-toolchain", 1),
        "build": ("minsizerel", "direct-define", "hxc_build", 5),
        "artifact": ("static-library", "cli-flag", "--artifact", 4),
    }
    if set(effective) != set(expected):
        raise HxcConfigFailure(f"effective field inventory drifted: {sorted(effective)!r}")
    for field, (value, kind, detail, priority) in expected.items():
        setting = require_object(effective[field], f"effective.{field}")
        source = require_object(setting.get("source"), f"effective.{field}.source")
        if setting.get("value") != value or source != {
            "kind": kind,
            "detail": detail,
            "priority": priority,
        }:
            raise HxcConfigFailure(f"effective.{field} value/provenance drifted: {setting!r}")
    encoded = canonical_json(report).decode("utf-8")
    for forbidden in (str(ROOT), "\\", "temporary", "token", "password", "secret"):
        if forbidden in encoded:
            raise HxcConfigFailure(f"effective report contains forbidden host/secret text: {forbidden!r}")


def check_defaults() -> None:
    result = run_probe("defaults", process_cwd=ROOT, label="default configuration")
    report = require_object(
        extract_json(result, REPORT_PREFIX, "default configuration"),
        "default report",
    )
    effective = require_object(report.get("effective"), "default effective")
    expected = {
        "hxml": ("build.hxml", "compiler-default"),
        "output": ("build/c", "compiler-default"),
        "profile": ("portable", "compiler-default"),
        "runtime": ("auto", "profile-preset"),
        "runtimeDiagnostics": ("summary", "profile-preset"),
        "environment": ("hosted", "compiler-default"),
        "cStandard": ("c11", "compiler-default"),
        "cExtensions": ("none", "compiler-default"),
        "build": ("debug", "compiler-default"),
        "artifact": ("executable", "compiler-default"),
    }
    for field, (value, kind) in expected.items():
        setting = require_object(effective.get(field), f"default.{field}")
        source = require_object(setting.get("source"), f"default.{field}.source")
        if setting.get("value") != value or source.get("kind") != kind:
            raise HxcConfigFailure(f"default {field} drifted: {setting!r}")


def check_cwd_independence(report: dict[str, object], payload: str) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-config-cwd-") as temporary:
        other = Path(temporary) / "unrelated/deep"
        other.mkdir(parents=True)
        result = run_probe(
            "resolve",
            VALID,
            process_cwd=other,
            label="alternate-cwd configuration",
        )
        alternate_payload = extract_payload(
            result, REPORT_PREFIX, "alternate-cwd configuration"
        )
        alternate = require_object(
            json.loads(alternate_payload), "alternate-cwd report"
        )
        if (
            canonical_json(alternate) != canonical_json(report)
            or alternate_payload != payload
        ):
            raise HxcConfigFailure("config-relative effective values changed with process cwd")

        root_host = run_probe(
            "host-paths",
            VALID,
            process_cwd=ROOT,
            label="root-cwd host paths",
        )
        other_host = run_probe(
            "host-paths",
            VALID,
            process_cwd=other,
            label="alternate-cwd host paths",
        )
        root_payload = root_host.stdout.split(HOST_PREFIX, 1)[1].rsplit(
            f"\n{SENTINEL}", 1
        )[0].strip()
        other_payload = other_host.stdout.split(HOST_PREFIX, 1)[1].rsplit(
            f"\n{SENTINEL}", 1
        )[0].strip()
        if root_payload != other_payload:
            raise HxcConfigFailure("resolved host paths changed with process cwd")
        expected = "\n".join(
            (
                str((VALID.parent / "haxe/build.hxml").resolve()),
                str((VALID.parent / "build/dev").resolve()),
            )
        )
        if root_payload != expected:
            raise HxcConfigFailure(
                f"resolved host paths are not rooted at hxc.json: {root_payload!r}"
            )


def check_crlf_and_locale(report: dict[str, object], payload: str) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-config-crlf-") as temporary:
        root = Path(temporary)
        crlf = root / "hxc.json"
        crlf.write_bytes(VALID.read_bytes().replace(b"\n", b"\r\n"))
        result = run_probe(
            "resolve",
            crlf,
            process_cwd=root,
            environment_updates={"LC_ALL": "tr_TR.UTF-8"},
            label="CRLF alternate-locale configuration",
        )
        rendered_payload = extract_payload(
            result, REPORT_PREFIX, "CRLF alternate-locale configuration"
        )
        rendered = require_object(json.loads(rendered_payload), "CRLF report")
        if (
            canonical_json(rendered) != canonical_json(report)
            or rendered_payload != payload
        ):
            raise HxcConfigFailure("effective configuration changed with CRLF input or locale")


def check_invalid_inputs() -> None:
    fixtures = sorted(INVALID.glob("*.json"), key=lambda path: path.name.encode("utf-8"))
    if len(fixtures) < 12:
        raise HxcConfigFailure("malformed configuration corpus is unexpectedly small")
    for fixture in fixtures:
        result = run_probe(
            "resolve",
            fixture,
            process_cwd=ROOT,
            expected_code=1,
            label=f"invalid fixture {fixture.name}",
        )
        combined = result.stdout + result.stderr
        if "HXC0003" not in combined or fixture.name not in combined:
            raise HxcConfigFailure(
                f"invalid fixture {fixture.name} missed HXC0003/source context:\n{combined}"
            )
        if SENTINEL in combined or str(fixture.resolve()) in combined or str(ROOT) in combined:
            raise HxcConfigFailure(
                f"invalid fixture {fixture.name} succeeded or leaked a host path:\n{combined}"
            )
    for mode in (
        "invalid-overlay",
        "invalid-direct-duplicate",
        "invalid-direct-unknown",
        "invalid-freestanding-shared",
        "invalid-output-owns-hxml",
    ):
        config = VALID if mode == "invalid-overlay" else None
        result = run_probe(
            mode,
            config,
            process_cwd=ROOT,
            expected_code=1,
            label=mode,
        )
        combined = result.stdout + result.stderr
        if "HXC0003" not in combined or SENTINEL in combined or str(ROOT) in combined:
            raise HxcConfigFailure(f"{mode} did not fail closed and redacted:\n{combined}")


def free_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as probe:
        probe.bind(("127.0.0.1", 0))
        return int(probe.getsockname()[1])


def wait_for_server(port: int, process: subprocess.Popen[str]) -> None:
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if process.poll() is not None:
            stdout, stderr = process.communicate()
            raise HxcConfigFailure(
                f"Haxe server exited early with {process.returncode}\nstdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise HxcConfigFailure("timed out waiting for the Haxe compiler server")


def check_compiler_server(report: dict[str, object], payload: str) -> None:
    port = free_port()
    environment = os.environ.copy()
    environment.pop("HAXE_NO_SERVER", None)
    server = subprocess.Popen(
        [development_tool("haxe"), "--wait", str(port)],
        cwd=ROOT,
        env=environment,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_server(port, server)
        endpoint = str(port)
        first, first_payload = render_snapshot_with_payload(connect=endpoint)
        check_defaults_result = run_probe(
            "defaults",
            process_cwd=ROOT,
            connect=endpoint,
            label="warm-server default configuration",
        )
        extract_json(
            check_defaults_result,
            REPORT_PREFIX,
            "warm-server default configuration",
        )
        repeated, repeated_payload = render_snapshot_with_payload(
            connect=endpoint, reverse=True
        )
        if (
            canonical_json(first) != canonical_json(report)
            or canonical_json(repeated) != canonical_json(report)
            or first_payload != payload
            or repeated_payload != payload
        ):
            raise HxcConfigFailure("compiler-server reuse or define order changed effective configuration")
    finally:
        server.terminate()
        try:
            server.communicate(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.communicate(timeout=5)


def compare_snapshot(actual: dict[str, object]) -> None:
    try:
        expected = json.loads(EXPECTED.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise HxcConfigFailure(f"cannot read effective-config snapshot: {error}") from error
    if actual != expected:
        raise HxcConfigFailure(
            "effective-config snapshot drifted; run "
            "`npm run snapshots:update -- --suite hxc-configuration` after review"
        )


def main() -> int:
    if shutil.which(development_tool("haxe")) is None:
        print("hxc-config: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        contract = parser_contract()
        check_schema_sync(contract)
        first, first_payload = render_snapshot_with_payload()
        second, second_payload = render_snapshot_with_payload(reverse=True)
        if canonical_json(first) != canonical_json(second) or first_payload != second_payload:
            raise HxcConfigFailure("direct-define input order changed effective configuration")
        check_report(first)
        check_defaults()
        check_cwd_independence(first, first_payload)
        check_crlf_and_locale(first, first_payload)
        check_invalid_inputs()
        check_compiler_server(first, first_payload)
        compare_snapshot(first)
    except (HxcConfigFailure, OSError, subprocess.TimeoutExpired) as error:
        print(f"hxc-config: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "hxc-config: OK: strict schema/parser sync, six-layer precedence, per-value provenance, "
        "byte-stable cwd/CRLF/locale/order/server determinism, and malformed-input guards passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

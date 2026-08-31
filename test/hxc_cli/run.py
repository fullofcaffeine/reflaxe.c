#!/usr/bin/env python3
"""Verify the Eval bootstrap router, JSON contract, and exit propagation."""

from __future__ import annotations

import json
import os
import re
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402


HAXE = ROOT / "node_modules/.bin/haxe"
SCHEMA = ROOT / "schemas/hxc-cli-response.schema.json"
DIAGNOSTIC_SCHEMA = ROOT / "schemas/hxc-diagnostic-event.schema.json"
EXPECTED_FIELDS = {
    "schemaVersion",
    "command",
    "status",
    "exitCategory",
    "exitCode",
    "stdout",
    "stderr",
    "signal",
    "diagnostics",
}
EXIT_CATEGORIES = {"success", "usage", "unavailable", "command", "internal"}
DIAGNOSTIC_FIELDS = {
    "schemaVersion",
    "id",
    "severity",
    "phase",
    "profile",
    "kind",
    "message",
    "remediation",
    "source",
    "context",
    "notes",
    "relatedLocations",
    "commandContext",
    "attachments",
}
DIAGNOSTIC_PHASES = {
    "bootstrap",
    "configuration",
    "lowering",
    "runtime-analysis",
    "abi",
    "interop",
    "target-contract",
    "internal",
    "cli",
    "native-tool",
    "bindgen",
    "test",
}


class HxcCliFailure(RuntimeError):
    pass


def invoke(main: str, arguments: list[str]) -> subprocess.CompletedProcess[str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return run_bounded_process(
        [
            str(HAXE),
            "--cwd",
            str(ROOT),
            "-cp",
            "src",
            *(["-cp", "test/hxc_cli"] if main == "CliProbe" else []),
            "--run",
            main,
            *arguments,
        ],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def require(condition: bool, message: str) -> None:
    if not condition:
        raise HxcCliFailure(message)


def json_response(result: subprocess.CompletedProcess[str], expected_code: int) -> dict[str, object]:
    require(
        result.returncode == expected_code,
        f"expected exit {expected_code}, got {result.returncode}: {result.stderr}",
    )
    require(
        result.stdout.endswith("\n") and result.stdout.count("\n") == 1,
        f"JSON stdout is not one framed value: {result.stdout!r}",
    )
    try:
        value = json.loads(result.stdout)
    except json.JSONDecodeError as error:
        raise HxcCliFailure(f"invalid JSON stdout: {error}: {result.stdout!r}") from error
    require(
        isinstance(value, dict) and set(value) == EXPECTED_FIELDS,
        f"JSON response fields drifted: {value!r}",
    )
    require(value["schemaVersion"] == 2, "JSON response schema version drifted")
    require(
        isinstance(value["command"], str) and value["command"] != "",
        "JSON response command is invalid",
    )
    require(value["status"] in {"ok", "error"}, "JSON response status is invalid")
    require(
        value["exitCategory"] in EXIT_CATEGORIES,
        "JSON response exit category is invalid",
    )
    require(
        type(value["exitCode"]) is int and 0 <= value["exitCode"] <= 255,
        "JSON response exit code is invalid",
    )
    require(
        (value["status"] == "ok") == (value["exitCode"] == 0),
        "JSON status and exit code disagree",
    )
    require(
        isinstance(value["stdout"], str) and isinstance(value["stderr"], str),
        "JSON child streams are not strings",
    )
    require(
        value["signal"] is None or isinstance(value["signal"], str),
        "JSON signal is invalid",
    )
    diagnostics = value["diagnostics"]
    require(isinstance(diagnostics, list), "JSON diagnostics is not an array")
    for diagnostic in diagnostics:
        require(
            isinstance(diagnostic, dict)
            and set(diagnostic) == DIAGNOSTIC_FIELDS,
            "JSON diagnostic fields drifted",
        )
        require(
            diagnostic["schemaVersion"] == 1,
            "JSON diagnostic schema version drifted",
        )
        require(
            isinstance(diagnostic["id"], str)
            and re.fullmatch(r"(?:HXC[0-9]{4}|HXC-CLI-[0-9]{4})", diagnostic["id"])
            is not None,
            "JSON diagnostic ID is invalid",
        )
        require(diagnostic["severity"] in {"error", "warning", "info"}, "JSON diagnostic severity is invalid")
        require(diagnostic["phase"] in DIAGNOSTIC_PHASES, "JSON diagnostic phase is invalid")
        require(diagnostic["profile"] in {None, "portable", "metal", "unresolved"}, "JSON diagnostic profile is invalid")
        require(diagnostic["kind"] is None or isinstance(diagnostic["kind"], str), "JSON diagnostic kind is invalid")
        require(
            isinstance(diagnostic["message"], str)
            and diagnostic["message"] != "",
            "JSON diagnostic message is invalid",
        )
        require(
            isinstance(diagnostic["remediation"], str)
            and diagnostic["remediation"] != "",
            "JSON diagnostic remediation is invalid",
        )
        require(diagnostic["source"] is None or valid_source(diagnostic["source"]), "JSON diagnostic source is invalid")
        require(diagnostic["context"] is None or isinstance(diagnostic["context"], str) and diagnostic["context"] != "", "JSON diagnostic context is invalid")
        require(
            isinstance(diagnostic["notes"], list)
            and all(isinstance(note, str) and note != "" for note in diagnostic["notes"]),
            "JSON diagnostic notes are invalid",
        )
        related = diagnostic["relatedLocations"]
        require(
            isinstance(related, list)
            and all(
                isinstance(location, dict)
                and set(location) == {"label", "source"}
                and isinstance(location["label"], str)
                and location["label"] != ""
                and valid_source(location["source"])
                for location in related
            ),
            "JSON diagnostic related locations are invalid",
        )
        context = diagnostic["commandContext"]
        require(
            isinstance(context, dict)
            and set(context) == {"command", "arguments"}
            and isinstance(context["command"], str)
            and context["command"] != ""
            and isinstance(context["arguments"], list)
            and all(isinstance(argument, str) for argument in context["arguments"]),
            "JSON diagnostic command context is invalid",
        )
        attachments = diagnostic["attachments"]
        require(
            isinstance(attachments, list)
            and all(
                isinstance(attachment, dict)
                and set(attachment) == {"source", "channel", "tool", "text"}
                and attachment["source"] in {"compiler", "native-tool", "bindgen", "abi", "test"}
                and attachment["channel"] in {"stdout", "stderr", "report"}
                and isinstance(attachment["tool"], str)
                and attachment["tool"] != ""
                and isinstance(attachment["text"], str)
                and attachment["text"] != ""
                for attachment in attachments
            ),
            "JSON diagnostic attachments are invalid",
        )
    return value


def valid_source(value: object) -> bool:
    if not isinstance(value, dict) or set(value) != {"file", "start", "end"}:
        return False
    file = value["file"]
    if not isinstance(file, str) or file == "" or file.startswith(("/", "~")) or "\\" in file or ".." in file.split("/"):
        return False
    points: list[tuple[int, int]] = []
    for name in ("start", "end"):
        point = value[name]
        if not isinstance(point, dict) or set(point) != {"line", "column"}:
            return False
        line = point["line"]
        column = point["column"]
        if type(line) is not int or line < 1 or type(column) is not int or column < 1:
            return False
        points.append((line, column))
    return points[0] <= points[1]


def check_schema() -> None:
    value = json.loads(SCHEMA.read_text(encoding="utf-8"))
    diagnostic = json.loads(DIAGNOSTIC_SCHEMA.read_text(encoding="utf-8"))
    require(value.get("additionalProperties") is False, "CLI schema must reject unknown response fields")
    require(set(value.get("required", [])) == EXPECTED_FIELDS, "CLI schema required fields drifted")
    properties = value.get("properties")
    require(isinstance(properties, dict) and set(properties) == EXPECTED_FIELDS, "CLI schema properties drifted")
    require(properties["schemaVersion"] == {"const": 2}, "CLI schema version drifted")
    require(
        set(properties["exitCategory"]["enum"]) == EXIT_CATEGORIES,
        "CLI schema exit categories drifted",
    )
    require(
        properties["diagnostics"]["items"] == {"$ref": "hxc-diagnostic-event.schema.json"},
        "CLI response does not reference the standalone diagnostic event schema",
    )
    require(diagnostic.get("additionalProperties") is False, "diagnostic schema must reject unknown fields")
    require(set(diagnostic.get("required", [])) == DIAGNOSTIC_FIELDS, "diagnostic schema required fields drifted")
    diagnostic_properties = diagnostic.get("properties")
    require(
        isinstance(diagnostic_properties, dict)
        and set(diagnostic_properties) == DIAGNOSTIC_FIELDS,
        "diagnostic schema properties drifted",
    )
    require(diagnostic_properties["schemaVersion"] == {"const": 1}, "diagnostic event schema version drifted")
    require(set(diagnostic_properties["phase"]["enum"]) == DIAGNOSTIC_PHASES, "diagnostic phase schema drifted")


def check_human_help_and_version() -> None:
    help_result = invoke("Run", ["help"])
    require(help_result.returncode == 0 and help_result.stderr == "", f"human help failed: {help_result}")
    require(help_result.stdout.startswith("Usage: hxc [--json] <command>"), "human help lost its usage line")
    for command in ("build", "run", "test", "bindgen", "export", "version"):
        require(f"  {command}" in help_result.stdout, f"human help omitted {command}")
    require(
        "build          Generate C and build the selected artifact. [unavailable]"
        in help_result.stdout,
        "human help hid current command availability",
    )

    version = invoke("Run", ["version"])
    require(version.returncode == 0 and version.stderr == "", f"human version failed: {version}")
    require(version.stdout.startswith("hxc ") and version.stdout.endswith("(CLI schema 2)\n"), "version contract drifted")


def check_fail_closed_routing() -> None:
    unknown = invoke("Run", ["wat"])
    require(unknown.returncode == 64 and unknown.stdout == "", f"unknown command did not use usage exit: {unknown}")
    require(unknown.stderr.startswith("HXC-CLI-0001: unknown command `wat`"), "unknown command diagnostic drifted")

    unknown_option = invoke("Run", ["--mystery", "build"])
    require(unknown_option.returncode == 64 and "HXC-CLI-0002" in unknown_option.stderr, "unknown global option was ignored")

    experimental = json_response(invoke("Run", ["build", "--experimental-fast", "--json"]), 64)
    require(experimental["exitCategory"] == "usage", "experimental option did not fail as usage")

    unavailable_result = invoke("Run", ["build", "--json"])
    unavailable = json_response(unavailable_result, 69)
    require(unavailable["exitCategory"] == "unavailable", "recognized unfinished command lost unavailable category")
    require(unavailable["stdout"] == "" and unavailable["stderr"] == "", "unavailable command invented child output")
    require(unavailable_result.stderr.startswith("HXC-CLI-0004:"), "JSON mode did not keep human diagnostic on stderr")


def check_json_help_and_child_propagation() -> None:
    help_result = invoke("Run", ["help", "run", "--json"])
    help_value = json_response(help_result, 0)
    require(help_result.stderr == "", "successful JSON help wrote human stderr")
    require(help_value["command"] == "run" and str(help_value["stdout"]).startswith("Usage: hxc run"), "command help JSON drifted")

    child_result = invoke("CliProbe", ["run", "--json", "--", "alpha", "--json"])
    child = json_response(child_result, 23)
    require(child["exitCategory"] == "command" and child["signal"] == "SIGTERM", "child exit or signal was hidden")
    require(child["stdout"] == "child-out:run:--|alpha|--json\n", "child stdout or forwarded arguments drifted")
    child_stderr = "native line one\nnative line two\n"
    require(child["stderr"] == child_stderr, "child stderr was hidden from JSON")
    diagnostics = child["diagnostics"]
    require(isinstance(diagnostics, list) and len(diagnostics) == 2, "child diagnostic event count drifted")
    diagnostic = diagnostics[0]
    require(diagnostic["id"] == "HXC-CLI-0099" and diagnostic["phase"] == "native-tool", "child diagnostic identity drifted")
    require(diagnostic["profile"] is None and diagnostic["kind"] is None and diagnostic["context"] is None, "native event invented compiler facts")
    require(
        diagnostic["commandContext"] == {"command": "run", "arguments": ["--", "alpha", "--json"]},
        "child diagnostic lost exact command context",
    )
    require(
        diagnostic["attachments"]
        == [
            {
                "source": "native-tool",
                "channel": "stderr",
                "tool": "synthetic-native-tool",
                "text": child_stderr,
            }
        ],
        "child diagnostic did not preserve native stderr losslessly",
    )
    require(diagnostic["source"]["file"] == "test/hxc_cli/CliProbe.hx", "child diagnostic source drifted")
    require(len(diagnostic["notes"]) == 1 and len(diagnostic["relatedLocations"]) == 1, "child diagnostic notes or related location drifted")
    compiler = diagnostics[1]
    require(
        compiler["id"] == "HXC1001"
        and compiler["severity"] == "error"
        and compiler["phase"] == "lowering"
        and compiler["profile"] == "portable"
        and compiler["kind"] == "unsupported-source"
        and compiler["context"] == "synthetic compiler context",
        "typed compiler record was not preserved in the public event",
    )
    require(
        compiler["source"]["file"] == "test/hxc_cli/CliProbe.hx"
        and compiler["commandContext"] == {"command": "run", "arguments": ["--", "alpha", "--json"]}
        and compiler["attachments"] == []
        and compiler["notes"] == ["The compiler note remains structured."],
        "compiler event lost source, command, attachment, or note facts",
    )
    require(
        child_result.stderr
        == "probe-log\nnative line one\nnative line two\nHXC-CLI-0099: synthetic child failure "
        "Remediation: Inspect the propagated child result.\n"
        "HXC1001: synthetic compiler failure Remediation: Use a supported construct or implement its typed IR and C AST lowering with semantic tests.\n",
        "JSON human stderr order or separator drifted",
    )


def main() -> int:
    if not HAXE.is_file():
        raise HxcCliFailure("hxc CLI tests require the pinned Haxe executable")
    check_schema()
    check_human_help_and_version()
    check_fail_closed_routing()
    check_json_help_and_child_propagation()
    print(
        "hxc-cli: OK: schema-2 response framing, schema-1 diagnostic events, "
        "exact command context, native attachments, and stderr discipline passed"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except HxcCliFailure as error:
        print(f"hxc-cli: ERROR: {error}")
        raise SystemExit(1)

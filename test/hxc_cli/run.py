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
    require(value["schemaVersion"] == 1, "JSON response schema version drifted")
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
            and set(diagnostic) == {"code", "message", "remediation"},
            "JSON diagnostic fields drifted",
        )
        require(
            isinstance(diagnostic["code"], str)
            and re.fullmatch(r"HXC-CLI-[0-9]{4}", diagnostic["code"]) is not None,
            "JSON diagnostic code is invalid",
        )
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
    return value


def check_schema() -> None:
    value = json.loads(SCHEMA.read_text(encoding="utf-8"))
    require(value.get("additionalProperties") is False, "CLI schema must reject unknown response fields")
    require(set(value.get("required", [])) == EXPECTED_FIELDS, "CLI schema required fields drifted")
    properties = value.get("properties")
    require(isinstance(properties, dict) and set(properties) == EXPECTED_FIELDS, "CLI schema properties drifted")
    require(properties["schemaVersion"] == {"const": 1}, "CLI schema version drifted")
    require(
        set(properties["exitCategory"]["enum"]) == EXIT_CATEGORIES,
        "CLI schema exit categories drifted",
    )


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
    require(version.stdout.startswith("hxc ") and version.stdout.endswith("(CLI schema 1)\n"), "version contract drifted")


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
    require(child["stderr"] == "child-err", "child stderr was hidden from JSON")
    require(
        child_result.stderr
        == "probe-log\nchild-err\nHXC-CLI-0099: synthetic child failure "
        "Remediation: Inspect the propagated child result.\n",
        "JSON human stderr order or separator drifted",
    )


def main() -> int:
    if not HAXE.is_file():
        raise HxcCliFailure("hxc CLI tests require the pinned Haxe executable")
    check_schema()
    check_human_help_and_version()
    check_fail_closed_routing()
    check_json_help_and_child_propagation()
    print("hxc-cli: OK: stable routing, exits, JSON framing, stderr logs, fail-closed flags, and exact child propagation passed")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except HxcCliFailure as error:
        print(f"hxc-cli: ERROR: {error}")
        raise SystemExit(1)

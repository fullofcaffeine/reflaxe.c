#!/usr/bin/env python3
"""Prove compiler-owned inspection reports, redaction, and read-only validation."""

from __future__ import annotations

import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402


HAXE = ROOT / "node_modules/.bin/haxe"
HELLO = ROOT / "examples/hello"
SCHEMA = ROOT / "schemas/hxc-inspect-report.schema.json"
REPORTS = (
    "manifest",
    "config",
    "typed-inventory",
    "hxcir",
    "c-ast",
    "lowering",
    "runtime",
    "symbols",
    "includes",
    "build",
    "declarations",
    "macros",
    "stdlib",
    "abi",
    "sizes",
)
PAYLOAD_KINDS = {
    "public-header",
    "private-header",
    "source",
    "runtime-header",
    "runtime-source",
}


class InspectFailure(RuntimeError):
    pass


def require(condition: bool, message: str) -> None:
    if not condition:
        raise InspectFailure(message)


def environment() -> dict[str, str]:
    value = os.environ.copy()
    value["HAXE_NO_SERVER"] = "1"
    value["LC_ALL"] = "C"
    return value


def compile_hello(output: Path, *, inspection: bool) -> None:
    command = [
        str(HAXE),
        "--cwd",
        str(HELLO),
        "build.hxml",
        "-D",
        "hxc_runtime_diagnostics=off",
        "-D",
        "hxc_project_layout=unity",
    ]
    if inspection:
        command.extend(("-D", "hxc_inspection_reports"))
    command.extend(("--custom-target", f"c={output}"))
    result = run_bounded_process(
        command,
        cwd=ROOT,
        env=environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    require(result.returncode == 0, f"hello inspection compile failed:\n{result.stdout}\n{result.stderr}")


def invoke(root: Path, report: str, *options: str) -> subprocess.CompletedProcess[str]:
    return run_bounded_process(
        [
            str(HAXE),
            "--cwd",
            str(ROOT),
            "-cp",
            "src",
            "--run",
            "Run",
            "inspect",
            report,
            "--manifest",
            str(root / "hxc.manifest.json"),
            *options,
        ],
        cwd=ROOT,
        env=environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def machine_report(root: Path, report: str, *options: str) -> tuple[dict[str, object], str]:
    result = invoke(root, report, "--json", *options)
    require(result.returncode == 0 and result.stderr == "", f"{report} JSON inspection failed: {result.stderr}")
    envelope = json.loads(result.stdout)
    require(envelope.get("schemaVersion") == 1 and envelope.get("command") == "inspect", "CLI envelope drifted")
    payload_text = envelope.get("stdout")
    require(isinstance(payload_text, str) and payload_text.endswith("\n"), "inspect payload framing drifted")
    payload = json.loads(payload_text)
    require(
        isinstance(payload, dict)
        and set(payload) == {"schemaVersion", "report", "redacted", "manifest", "data"}
        and payload.get("schemaVersion") == 1
        and payload.get("report") == report,
        f"{report} schema drifted",
    )
    return payload, result.stdout


def tree_state(root: Path) -> dict[str, tuple[int, str]]:
    return {
        path.relative_to(root).as_posix(): (
            path.stat().st_mtime_ns,
            hashlib.sha256(path.read_bytes()).hexdigest(),
        )
        for path in sorted(root.rglob("*"))
        if path.is_file() and not path.is_symlink()
    }


def payload_state(root: Path) -> dict[str, str]:
    manifest = json.loads((root / "hxc.manifest.json").read_text(encoding="utf-8"))
    return {
        artifact["path"]: hashlib.sha256((root / artifact["path"]).read_bytes()).hexdigest()
        for artifact in manifest["artifacts"]
        if artifact["kind"] in PAYLOAD_KINDS
    }


def check_reports(root: Path) -> None:
    # The schema and executable report vocabulary must move together.
    schema = json.loads(SCHEMA.read_text(encoding="utf-8"))
    require(schema.get("additionalProperties") is False, "inspect schema must reject unknown fields")
    require(set(schema.get("required", ())) == {"schemaVersion", "report", "redacted", "manifest", "data"}, "inspect schema required fields drifted")
    require(tuple(schema["properties"]["report"]["enum"]) == (*REPORTS, "all"), "inspect report vocabulary drifted")
    before = tree_state(root)
    results = {name: machine_report(root, name)[0] for name in REPORTS}
    after = tree_state(root)
    require(before == after, "inspection changed artifact bytes or modification times")

    human = invoke(root, "runtime")
    require(
        human.returncode == 0
        and human.stderr == ""
        and human.stdout.startswith('{\n  "schemaVersion": 1,')
        and '"report": "runtime"' in human.stdout,
        "human inspection form drifted",
    )
    runtime = results["runtime"]["data"]
    require(isinstance(runtime, dict) and runtime.get("directDecisions"), "runtime decisions are absent")
    reasons = runtime.get("rootReasons")
    require(
        isinstance(reasons, list)
        and reasons
        and reasons[0].get("source", {}).get("file") == "Main.hx",
        "stable source-to-runtime-decision link is absent",
    )
    declarations = results["declarations"]["data"]
    require(
        isinstance(declarations, dict)
        and declarations.get("effects", {}).get("unsafe") == "none"
        and declarations.get("effects", {}).get("ownership"),
        "typed declaration effects omitted unsafe or ownership evidence",
    )
    c_ast = results["c-ast"]["data"]
    require(
        isinstance(c_ast, dict)
        and c_ast.get("format") == "structural-c-ast-summary-v1"
        and c_ast.get("producerPasses") == ["runtime-feature-planning", "c-ast-project-planning"]
        and c_ast.get("sources")
        and c_ast["sources"][0].get("declarations"),
        "structural C AST report is absent",
    )
    hxcir = results["hxcir"]["data"]
    require(
        isinstance(hxcir, dict)
        and hxcir.get("producerPasses", [])[-1:] == ["semantic-lowering"],
        "HxcIR producer pass evidence is absent",
    )


def check_relocation_and_redaction(source: Path, temporary: Path) -> None:
    first = temporary / "relocated-a"
    second = temporary / "relocated-b"
    shutil.copytree(source, first)
    shutil.copytree(source, second)
    _, first_bytes = machine_report(first, "runtime")
    _, second_bytes = machine_report(second, "runtime")
    require(first_bytes == second_bytes, "redacted report changed across absolute output roots")
    require(str(first) not in first_bytes and "$PROJECT/hxc.manifest.json" in first_bytes, "default report leaked its output root")

    manifest_path = first / "hxc.manifest.json"
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    secret = "/private/creator/workspace/secret-project"
    manifest["projectName"] = secret
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    redacted, _ = machine_report(first, "manifest")
    require(secret not in json.dumps(redacted) and "<redacted-path>" in json.dumps(redacted), "default redaction missed an absolute path")
    visible, _ = machine_report(first, "manifest", "--show-sensitive")
    require(secret in json.dumps(visible) and visible.get("redacted") is False, "explicit sensitive view did not retain the path")


def check_fail_closed(source: Path, temporary: Path) -> None:
    modified = temporary / "modified"
    shutil.copytree(source, modified)
    runtime = modified / "hxc.runtime-plan.json"
    runtime.write_bytes(runtime.read_bytes() + b" ")
    result = invoke(modified, "config", "--json")
    require(result.returncode == 1 and "does not match its SHA-256" in result.stderr, "modified artifact did not fail closed")

    traversing = temporary / "traversing"
    shutil.copytree(source, traversing)
    manifest_path = traversing / "hxc.manifest.json"
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    manifest["artifacts"].append({"path": "../escape", "kind": "source", "sha256": "0" * 64})
    manifest_path.write_text(json.dumps(manifest) + "\n", encoding="utf-8")
    result = invoke(traversing, "manifest", "--json")
    require(result.returncode == 1 and "unsafe or duplicated" in result.stderr, "traversing artifact did not fail closed")

    escaped = temporary / "escaped"
    shutil.copytree(source, escaped)
    outside = temporary / "outside.json"
    outside.write_text("{}\n", encoding="utf-8")
    target = escaped / "hxc.runtime-plan.json"
    target.unlink()
    target.symlink_to(outside)
    manifest = json.loads((escaped / "hxc.manifest.json").read_text(encoding="utf-8"))
    for artifact in manifest["artifacts"]:
        if artifact["path"] == "hxc.runtime-plan.json":
            artifact["sha256"] = hashlib.sha256(outside.read_bytes()).hexdigest()
    (escaped / "hxc.manifest.json").write_text(json.dumps(manifest) + "\n", encoding="utf-8")
    result = invoke(escaped, "config", "--json")
    require(result.returncode == 1 and "escapes the output root" in result.stderr, "symlink escape did not fail closed")


def main() -> int:
    require(HAXE.is_file(), "hxc inspection tests require the pinned Haxe executable")
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-hxc-inspect-") as temporary_text:
        temporary = Path(temporary_text)
        inspected = temporary / "inspected"
        ordinary = temporary / "ordinary"
        compile_hello(inspected, inspection=True)
        compile_hello(ordinary, inspection=False)
        require(payload_state(inspected) == payload_state(ordinary), "inspection publication changed generated C semantics")
        check_reports(inspected)
        check_relocation_and_redaction(inspected, temporary)
        check_fail_closed(inspected, temporary)
    print(
        "hxc-inspect: OK: versioned human/JSON reports, stable source links, redaction, hash/path confinement, semantic parity, and read-only behavior passed"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (InspectFailure, subprocess.TimeoutExpired) as error:
        print(f"hxc-inspect: ERROR: {error}", file=sys.stderr)
        raise SystemExit(1)

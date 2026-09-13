#!/usr/bin/env python3
"""Verify deterministic, safe, schema-valid `hxc new` project templates."""

from __future__ import annotations

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

SUITE = Path(__file__).resolve().parent
EXPECTED_FILES = {
    ".gitignore",
    "LICENSE.txt",
    "README.md",
    "build.hxml",
    "hxc.json",
}
PROBE_BINARY: Path | None = None


class HxcNewFailure(RuntimeError):
    """A focused template contract did not hold."""


def development_tool(name: str) -> str:
    local_name = f"{name}.cmd" if os.name == "nt" else name
    local = ROOT / "node_modules/.bin" / local_name
    return str(local) if local.is_file() else name


def run_command(
    command: list[str],
    *,
    cwd: Path = ROOT,
    expected_code: int = 0,
    label: str,
) -> subprocess.CompletedProcess[str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    environment["LC_ALL"] = "C"
    result = run_bounded_process(
        command,
        cwd=cwd,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != expected_code:
        raise HxcNewFailure(
            f"{label} returned {result.returncode}, expected {expected_code}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return result


def probe_command(mode: str, path: Path, arguments: list[str]) -> list[str]:
    if PROBE_BINARY is None:
        raise HxcNewFailure("project probe was not compiled")
    return [
        sys.executable,
        str(PROBE_BINARY),
        mode,
        str(path),
        *arguments,
    ]


def compile_probe(output: Path) -> None:
    result = run_command(
        [
            development_tool("haxe"),
            "--cwd",
            str(ROOT),
            "-cp",
            "src",
            "-cp",
            "test/hxc_new",
            "-main",
            "NewProjectProbe",
            "-python",
            str(output),
        ],
        label="compile project probe",
    )
    require(result.stdout == "" and result.stderr == "", "project probe compilation emitted output")


def run_cli(
    cwd: Path,
    arguments: list[str],
    *,
    expected_code: int = 0,
    label: str,
) -> subprocess.CompletedProcess[str]:
    return run_command(
        probe_command("cli", cwd, arguments),
        expected_code=expected_code,
        label=label,
    )


def files(root: Path) -> dict[str, bytes]:
    return {
        path.relative_to(root).as_posix(): path.read_bytes()
        for path in sorted(root.rglob("*"))
        if path.is_file()
    }


def require(condition: bool, message: str) -> None:
    if not condition:
        raise HxcNewFailure(message)


def create_kind(parent: Path, kind: str, name: str, module: str) -> Path:
    result = run_cli(
        parent,
        [
            "new",
            name,
            "--kind",
            kind,
            "--module",
            module,
            "--license",
            "MIT",
        ],
        label=f"create {kind}",
    )
    require(result.stderr == "", f"successful {kind} creation wrote stderr")
    require(
        result.stdout == f"Created {kind} project `{name}` in {name}/ (6 template files).\n",
        f"unexpected {kind} success output: {result.stdout!r}",
    )
    project = parent / name
    expected = EXPECTED_FILES | {f"src/{module}.hx"}
    require(set(files(project)) == expected, f"{kind} inventory drifted: {sorted(files(project))!r}")
    require(b"${" not in b"".join(files(project).values()), f"{kind} retained an unsubstituted placeholder")

    config = run_command(
        probe_command("config", project / "hxc.json", []),
        label=f"validate {kind} hxc.json",
    )
    require(config.stdout == "hxc-new-config: OK\n" and config.stderr == "", f"{kind} config probe envelope drifted")

    meaningful_hxml = [
        line
        for line in (project / "build.hxml").read_text(encoding="utf-8").splitlines()
        if line and not line.startswith("#")
    ]
    require(meaningful_hxml[:2] == ["-cp src", "-lib reflaxe.c"], f"{kind} hxml lost its source or library input")
    require(len(meaningful_hxml) == 7, f"{kind} hxml must contain exactly seven effective arguments")
    require(
        any(line == f"-main {module}" or line == f'--macro include("{module}")' for line in meaningful_hxml),
        f"{kind} hxml does not retain its selected module",
    )

    if kind == "app":
        type_arguments = ["-cp", str(project / "src"), "-main", module, "--no-output"]
    else:
        type_arguments = [
            "-cp",
            str(project / "src"),
            "--macro",
            f'include("{module}")',
            "--no-output",
        ]
    typecheck = run_command(
        [development_tool("haxe"), "--cwd", str(ROOT), *type_arguments],
        label=f"type-check {kind}",
    )
    require(typecheck.stdout == "" and typecheck.stderr == "", f"{kind} type-check emitted output")

    readme = (project / "README.md").read_text(encoding="utf-8")
    limitation_phrases = {
        "app": "not available yet",
        "library": "not complete yet",
        "embedded": "does not provide",
    }
    require(limitation_phrases[kind] in readme, f"{kind} README hides current limitations")
    require("MIT" in readme, f"{kind} README omitted the selected license")
    return project


def check_templates(root: Path) -> None:
    first = root / "first"
    second = root / "second"
    first.mkdir()
    second.mkdir()
    specifications = (
        ("app", "sample-app", "SampleApp"),
        ("library", "sample-library", "SampleLibrary"),
        ("embedded", "sample-embedded", "SampleEmbedded"),
    )
    for kind, name, module in specifications:
        left = create_kind(first, kind, name, module)
        right = create_kind(second, kind, name, module)
        require(files(left) == files(right), f"{kind} generation changed across clean directories")

    json_result = run_cli(
        root,
        ["new", "json-app", "--json"],
        label="JSON creation",
    )
    require(json_result.stderr == "", "JSON creation wrote stderr")
    response = json.loads(json_result.stdout)
    require(response["status"] == "ok" and response["exitCategory"] == "success", "JSON creation status drifted")
    require(response["diagnostics"] == [], "JSON creation emitted diagnostics")


def check_write_modes(root: Path) -> None:
    default_root = root / "default"
    default_root.mkdir()
    project = create_kind(default_root, "app", "existing-app", "ExistingApp")
    before = files(project)
    failure = run_cli(
        default_root,
        ["new", "existing-app"],
        expected_code=1,
        label="default existing-directory rejection",
    )
    require("HXC-CLI-0202" in failure.stderr, "existing directory lacks stable conflict diagnostic")
    require(files(project) == before, "default rejection changed existing files")

    merge_root = root / "merge"
    merge_project = merge_root / "merge-app"
    merge_project.mkdir(parents=True)
    readme = merge_project / "README.md"
    readme.write_text("user-owned\n", encoding="utf-8")
    merged = run_cli(
        merge_root,
        ["new", "merge-app", "--merge"],
        label="merge existing directory",
    )
    require(readme.read_text(encoding="utf-8") == "user-owned\n", "merge overwrote an existing template path")
    require("preserved 1 existing template file" in merged.stdout, "merge did not report preserved content")
    require(len(files(merge_project)) == 6, "merge did not create every missing template file")

    force_root = root / "force"
    force_root.mkdir()
    force_project = create_kind(force_root, "app", "force-app", "ForceApp")
    force_readme = force_project / "README.md"
    canonical_readme = force_readme.read_bytes()
    force_readme.write_text("replace me\n", encoding="utf-8")
    notes = force_project / "notes.txt"
    notes.write_text("keep me\n", encoding="utf-8")
    run_cli(
        force_root,
        ["new", "force-app", "--module", "ForceApp", "--license", "MIT", "--force"],
        label="force owned files",
    )
    require(force_readme.read_bytes() == canonical_readme, "force did not restore a template-owned file")
    require(notes.read_text(encoding="utf-8") == "keep me\n", "force changed an unrelated file")

    prefix_root = root / "prefix"
    prefix_project = prefix_root / "prefix-app"
    prefix_project.mkdir(parents=True)
    (prefix_project / "src").write_text("blocking file\n", encoding="utf-8")
    prefix_before = files(prefix_project)
    prefix = run_cli(
        prefix_root,
        ["new", "prefix-app", "--merge"],
        expected_code=1,
        label="prefix conflict preflight",
    )
    require("HXC-CLI-0202" in prefix.stderr, "prefix conflict lacks stable diagnostic")
    require(files(prefix_project) == prefix_before, "preflight conflict allowed a partial write")

    if hasattr(os, "symlink"):
        link_root = root / "link"
        link_root.mkdir()
        outside = root / "outside.txt"
        outside.write_text("outside\n", encoding="utf-8")
        link_project = link_root / "link-app"
        link_project.mkdir()
        os.symlink(outside, link_project / "README.md")
        link = run_cli(
            link_root,
            ["new", "link-app", "--force"],
            expected_code=1,
            label="symbolic-link rejection",
        )
        require("symbolic link" in link.stderr, "force did not explain symbolic-link rejection")
        require(outside.read_text(encoding="utf-8") == "outside\n", "force wrote through a symbolic link")


def check_invalid_input(root: Path) -> None:
    invalid_cases = (
        (["new"], "project name is required"),
        (["new", "../escape"], "invalid project name"),
        (["new", "BadName"], "invalid project name"),
        (["new", "con"], "invalid project name"),
        (["new", "a" * 65], "invalid project name"),
        (["new", "safe", "--module", "lowercase"], "invalid Haxe module name"),
        (["new", "safe", "--module", "String"], "invalid Haxe module name"),
        (["new", "safe", "--module", "Array"], "invalid Haxe module name"),
        (["new", "safe", "--module", "A" * 129], "invalid Haxe module name"),
        (["new", "safe", "--license", "../../MIT"], "unsupported license identifier"),
        (["new", "safe", "--kind", "plugin"], "invalid project kind"),
        (["new", "safe", "--merge", "--force"], "mutually exclusive"),
        (["new", "safe", "--kind", "app", "--kind", "library"], "may appear only once"),
        (["new", "safe", "--unknown"], "unknown hxc new option"),
    )
    for index, (arguments, message) in enumerate(invalid_cases):
        case_root = root / f"invalid-{index}"
        case_root.mkdir()
        result = run_cli(case_root, list(arguments), expected_code=64, label=f"invalid input {index}")
        require("HXC-CLI-0201" in result.stderr and message in result.stderr, f"invalid input {index} diagnostic drifted")
        require(files(case_root) == {}, f"invalid input {index} changed the filesystem")

    file_root = root / "target-file"
    file_root.mkdir()
    target = file_root / "occupied"
    target.write_text("keep\n", encoding="utf-8")
    result = run_cli(file_root, ["new", "occupied", "--force"], expected_code=1, label="target file")
    require("HXC-CLI-0202" in result.stderr, "target file lacks stable conflict diagnostic")
    require(target.read_text(encoding="utf-8") == "keep\n", "target file changed after rejection")


def main() -> int:
    global PROBE_BINARY
    if shutil.which(development_tool("haxe")) is None:
        raise HxcNewFailure("haxe is required")
    with tempfile.TemporaryDirectory(prefix="hxc-new-") as temporary:
        root = Path(temporary)
        PROBE_BINARY = root / "new_project_probe.py"
        compile_probe(PROBE_BINARY)
        project_root = root / "projects"
        project_root.mkdir()
        check_templates(project_root)
        check_write_modes(project_root)
        check_invalid_input(project_root)
        PROBE_BINARY = None
    print("hxc-new: OK: deterministic app, library, embedded, merge, force, and fail-closed contracts passed")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (HxcNewFailure, OSError, subprocess.SubprocessError, json.JSONDecodeError) as error:
        print(f"hxc-new: ERROR: {error}", file=sys.stderr)
        raise SystemExit(1)

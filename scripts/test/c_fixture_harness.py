#!/usr/bin/env python3
"""Compile, link, and run C fixture projects with reproducible argument records."""

from __future__ import annotations

import hashlib
import json
import os
import re
import shutil
import subprocess
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Iterable, Mapping


C11_STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)
TOOLCHAIN_COMPILERS = {"gcc": "gcc", "clang": "clang"}
PROJECT_ID = re.compile(r"^[a-z0-9][a-z0-9-]*$")


class CFixtureFailure(RuntimeError):
    pass


def normalized_relative_path(value: str) -> bool:
    if not value or "\\" in value:
        return False
    path = PurePosixPath(value)
    return not path.is_absolute() and all(
        part not in ("", ".", "..") for part in path.parts
    )


@dataclass(frozen=True)
class CFixtureProject:
    identifier: str
    sources: tuple[str, ...]
    headers: tuple[str, ...]
    include_directories: tuple[str, ...]
    expected_stdout: str
    coverage: tuple[str, ...]
    expected_stderr: str = ""
    expected_exit: int = 0
    link_arguments: tuple[str, ...] = ()

    def validate(self) -> None:
        if PROJECT_ID.fullmatch(self.identifier) is None:
            raise CFixtureFailure(
                f"invalid C fixture project ID: {self.identifier!r}"
            )
        if not self.sources:
            raise CFixtureFailure(
                f"C fixture project {self.identifier!r} has no sources"
            )
        paths = (*self.sources, *self.headers, *self.include_directories)
        invalid = [value for value in paths if not normalized_relative_path(value)]
        if invalid:
            raise CFixtureFailure(
                f"C fixture project {self.identifier!r} has unsafe relative paths: "
                f"{invalid!r}"
            )
        if len(set(self.sources)) != len(self.sources):
            raise CFixtureFailure(
                f"C fixture project {self.identifier!r} repeats a source"
            )
        if len(set(self.headers)) != len(self.headers):
            raise CFixtureFailure(
                f"C fixture project {self.identifier!r} repeats a header"
            )
        if len(set(self.include_directories)) != len(self.include_directories):
            raise CFixtureFailure(
                f"C fixture project {self.identifier!r} repeats an include directory"
            )
        if set(self.sources) & set(self.headers):
            raise CFixtureFailure(
                f"C fixture project {self.identifier!r} conflates a source and header"
            )
        if len(set(self.coverage)) != len(self.coverage) or not self.coverage:
            raise CFixtureFailure(
                f"C fixture project {self.identifier!r} needs unique coverage tags"
            )
        if any(PROJECT_ID.fullmatch(value) is None for value in self.coverage):
            raise CFixtureFailure(
                f"C fixture project {self.identifier!r} has an invalid coverage tag"
            )
        for argument in self.link_arguments:
            if (
                not argument
                or "\x00" in argument
                or "\n" in argument
                or "\r" in argument
            ):
                raise CFixtureFailure(
                    f"C fixture project {self.identifier!r} has an unsafe link argument"
                )


@dataclass(frozen=True)
class CToolchain:
    family: str
    compiler: str
    command_name: str
    version_line: str


@dataclass(frozen=True)
class CInvocationRecord:
    phase: str
    arguments: tuple[str, ...]
    working_directory: str
    exit_code: int
    stdout: str
    stderr: str

    def json_value(self) -> dict[str, object]:
        return {
            "phase": self.phase,
            "arguments": list(self.arguments),
            "workingDirectory": self.working_directory,
            "exitCode": self.exit_code,
            "stdout": self.stdout,
            "stderr": self.stderr,
        }


@dataclass(frozen=True)
class CInputRecord:
    path: str
    sha256: str

    def json_value(self) -> dict[str, str]:
        return {"path": self.path, "sha256": self.sha256}


def command_result(
    command: list[str], *, working_directory: Path, timeout_seconds: int
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        cwd=working_directory,
        check=False,
        capture_output=True,
        text=True,
        timeout=timeout_seconds,
    )


def compiler_identity(executable: str, *, repository_root: Path) -> tuple[str, str]:
    result = command_result(
        [executable, "--version"],
        working_directory=repository_root,
        timeout_seconds=30,
    )
    if result.returncode != 0:
        raise CFixtureFailure(
            f"cannot identify C compiler {executable}: exit {result.returncode}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    combined = (result.stdout + result.stderr).strip()
    first_line = combined.splitlines()[0] if combined else "unknown version"
    lowered = combined.lower()
    if "clang" in lowered:
        return "clang", first_line
    if "free software foundation" in lowered or "gcc" in lowered:
        return "gcc", first_line
    return "unknown", first_line


def resolve_toolchains(
    requested: str, *, repository_root: Path
) -> tuple[CToolchain, ...]:
    if requested not in ("auto", *TOOLCHAIN_COMPILERS):
        raise CFixtureFailure(
            f"unknown C fixture toolchain request: {requested!r}"
        )

    resolved: list[CToolchain] = []
    families: Iterable[str] = (
        TOOLCHAIN_COMPILERS if requested == "auto" else (requested,)
    )
    for family in families:
        command_name = TOOLCHAIN_COMPILERS[family]
        executable = shutil.which(command_name)
        if executable is None:
            if requested != "auto":
                raise CFixtureFailure(
                    f"required {family} C compiler {command_name!r} is missing"
                )
            continue
        actual_family, version_line = compiler_identity(
            executable, repository_root=repository_root
        )
        if actual_family != family:
            if requested != "auto":
                raise CFixtureFailure(
                    f"required {family} command {command_name!r} identifies as "
                    f"{actual_family}"
                )
            continue
        resolved.append(
            CToolchain(family, executable, command_name, version_line)
        )

    if not resolved:
        raise CFixtureFailure(
            "no identity-verified GCC or Clang C compiler is available"
        )
    return tuple(resolved)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(65536), b""):
            digest.update(block)
    return digest.hexdigest()


def normalize_path(value: str, roots: tuple[tuple[Path, str], ...]) -> str:
    for root, token in roots:
        root_text = str(root)
        if value == root_text:
            return token
        prefix = root_text + os.sep
        if value.startswith(prefix):
            relative = Path(value[len(prefix) :]).as_posix()
            return f"{token}/{relative}"
    return value


def normalize_argument(
    value: str,
    *,
    compiler: str,
    roots: tuple[tuple[Path, str], ...],
) -> str:
    if value == compiler:
        return "${CC}"
    if value.startswith("-I"):
        return "-I" + normalize_path(value[2:], roots)
    return normalize_path(value, roots)


def run_invocation(
    phase: str,
    command: list[str],
    *,
    compiler: str,
    working_directory: Path,
    roots: tuple[tuple[Path, str], ...],
    timeout_seconds: int,
    require_zero: bool = True,
) -> CInvocationRecord:
    result = command_result(
        command,
        working_directory=working_directory,
        timeout_seconds=timeout_seconds,
    )
    record = CInvocationRecord(
        phase,
        tuple(
            normalize_argument(argument, compiler=compiler, roots=roots)
            for argument in command
        ),
        normalize_path(str(working_directory), roots),
        result.returncode,
        result.stdout,
        result.stderr,
    )
    if require_zero and result.returncode != 0:
        raise CFixtureFailure(
            f"C fixture {phase} failed:\n"
            + json.dumps(record.json_value(), ensure_ascii=False, indent=2)
        )
    return record


def validate_fixture_files(
    root: Path, project: CFixtureProject
) -> tuple[CInputRecord, ...]:
    records: list[CInputRecord] = []
    for relative in sorted(
        {*project.sources, *project.headers}, key=lambda value: value.encode("utf-8")
    ):
        path = root / relative
        if not path.is_file() or path.is_symlink():
            raise CFixtureFailure(
                f"C fixture project {project.identifier!r} input is missing or "
                f"linked: {relative}"
            )
        records.append(CInputRecord(relative, sha256_file(path)))
    for relative in project.include_directories:
        path = root / relative
        if not path.is_dir() or path.is_symlink():
            raise CFixtureFailure(
                f"C fixture project {project.identifier!r} include directory is "
                f"missing or linked: {relative}"
            )
    return tuple(records)


def run_project(
    project: CFixtureProject,
    *,
    fixture_root: Path,
    build_root: Path,
    repository_root: Path,
    toolchain: CToolchain,
    strict_flags: tuple[str, ...],
    timeout_seconds: int,
) -> dict[str, object]:
    project.validate()
    inputs = validate_fixture_files(fixture_root, project)
    project_build = build_root / project.identifier
    project_build.mkdir(parents=True, exist_ok=True)
    roots = (
        (fixture_root.absolute(), "${FIXTURE_ROOT}"),
        (build_root.absolute(), "${BUILD_ROOT}"),
        (repository_root.absolute(), "${REPOSITORY_ROOT}"),
    )
    invocations: list[CInvocationRecord] = []
    objects: list[Path] = []
    include_arguments = [
        f"-I{fixture_root / relative}" for relative in project.include_directories
    ]
    for index, relative in enumerate(project.sources):
        source = fixture_root / relative
        output = project_build / f"{index:02d}-{source.stem}.o"
        command = [
            toolchain.compiler,
            *strict_flags,
            *include_arguments,
            "-c",
            str(source),
            "-o",
            str(output),
        ]
        invocations.append(
            run_invocation(
                f"compile:{relative}",
                command,
                compiler=toolchain.compiler,
                working_directory=repository_root,
                roots=roots,
                timeout_seconds=timeout_seconds,
            )
        )
        objects.append(output)

    executable = project_build / project.identifier
    invocations.append(
        run_invocation(
            "link",
            [
                toolchain.compiler,
                *(str(path) for path in objects),
                *project.link_arguments,
                "-o",
                str(executable),
            ],
            compiler=toolchain.compiler,
            working_directory=repository_root,
            roots=roots,
            timeout_seconds=timeout_seconds,
        )
    )
    execution = run_invocation(
        "run",
        [str(executable)],
        compiler=toolchain.compiler,
        working_directory=project_build,
        roots=roots,
        timeout_seconds=timeout_seconds,
        require_zero=False,
    )
    invocations.append(execution)
    if (
        execution.exit_code != project.expected_exit
        or execution.stdout != project.expected_stdout
        or execution.stderr != project.expected_stderr
    ):
        raise CFixtureFailure(
            f"C fixture project {project.identifier!r} produced unexpected output:\n"
            + json.dumps(execution.json_value(), ensure_ascii=False, indent=2)
        )

    return {
        "id": project.identifier,
        "coverage": list(project.coverage),
        "inputs": [record.json_value() for record in inputs],
        "expected": {
            "exitCode": project.expected_exit,
            "stdout": project.expected_stdout,
            "stderr": project.expected_stderr,
        },
        "invocations": [record.json_value() for record in invocations],
    }


def run_c_fixture_corpus(
    *,
    suite: str,
    projects: tuple[CFixtureProject, ...],
    fixture_root: Path,
    build_root: Path,
    repository_root: Path,
    requested_toolchain: str,
    strict_flags: tuple[str, ...] = C11_STRICT_FLAGS,
    timeout_seconds: int = 30,
) -> dict[str, object]:
    if PROJECT_ID.fullmatch(suite) is None:
        raise CFixtureFailure(f"invalid C fixture suite ID: {suite!r}")
    identifiers = [project.identifier for project in projects]
    if identifiers != sorted(identifiers, key=lambda value: value.encode("utf-8")):
        raise CFixtureFailure("C fixture projects must use canonical UTF-8 ID order")
    if len(set(identifiers)) != len(identifiers):
        raise CFixtureFailure("C fixture project IDs must be unique")
    if not strict_flags or len(set(strict_flags)) != len(strict_flags):
        raise CFixtureFailure("C fixture strict flags must be nonempty and unique")

    toolchains = resolve_toolchains(
        requested_toolchain, repository_root=repository_root
    )
    toolchain_reports: list[dict[str, object]] = []
    for toolchain in toolchains:
        toolchain_build = build_root / toolchain.family
        toolchain_reports.append(
            {
                "family": toolchain.family,
                "command": toolchain.command_name,
                "versionLine": toolchain.version_line,
                "strictFlags": list(strict_flags),
                "projects": [
                    run_project(
                        project,
                        fixture_root=fixture_root,
                        build_root=toolchain_build,
                        repository_root=repository_root,
                        toolchain=toolchain,
                        strict_flags=strict_flags,
                        timeout_seconds=timeout_seconds,
                    )
                    for project in projects
                ],
            }
        )
    return {"schemaVersion": 1, "suite": suite, "toolchains": toolchain_reports}


def validate_report(
    report: Mapping[str, object], *, required_coverage: frozenset[str]
) -> None:
    if report.get("schemaVersion") != 1 or not isinstance(
        report.get("suite"), str
    ):
        raise CFixtureFailure("C fixture report lost its schema-1 identity")
    toolchains = report.get("toolchains")
    if not isinstance(toolchains, list) or not toolchains:
        raise CFixtureFailure("C fixture report contains no completed toolchain")
    covered: set[str] = set()
    for toolchain in toolchains:
        if not isinstance(toolchain, dict):
            raise CFixtureFailure("C fixture report has a malformed toolchain record")
        family = toolchain.get("family")
        if not isinstance(family, str) or family not in TOOLCHAIN_COMPILERS:
            raise CFixtureFailure(
                f"C fixture report has unknown compiler family: {family!r}"
            )
        if toolchain.get("command") != TOOLCHAIN_COMPILERS[family]:
            raise CFixtureFailure("C fixture report has inconsistent compiler identity")
        if not isinstance(toolchain.get("versionLine"), str) or not toolchain[
            "versionLine"
        ]:
            raise CFixtureFailure("C fixture report omitted compiler version identity")
        projects = toolchain.get("projects")
        if not isinstance(projects, list) or not projects:
            raise CFixtureFailure("C fixture report contains no projects")
        for project in projects:
            if not isinstance(project, dict):
                raise CFixtureFailure("C fixture report has a malformed project")
            coverage = project.get("coverage")
            invocations = project.get("invocations")
            expected = project.get("expected")
            if not isinstance(coverage, list) or not all(
                isinstance(value, str) for value in coverage
            ):
                raise CFixtureFailure("C fixture report has malformed coverage")
            if (
                not isinstance(expected, dict)
                or not isinstance(expected.get("exitCode"), int)
                or not isinstance(expected.get("stdout"), str)
                or not isinstance(expected.get("stderr"), str)
            ):
                raise CFixtureFailure("C fixture report has malformed expectations")
            covered.update(coverage)
            if not isinstance(invocations, list) or len(invocations) < 3:
                raise CFixtureFailure(
                    "C fixture report omitted compile/link/run records"
                )
            phases: list[str] = []
            for invocation in invocations:
                if not isinstance(invocation, dict):
                    raise CFixtureFailure(
                        "C fixture report has a malformed invocation"
                    )
                arguments = invocation.get("arguments")
                if not isinstance(arguments, list) or not all(
                    isinstance(argument, str) for argument in arguments
                ):
                    raise CFixtureFailure(
                        "C fixture invocation is not an argument array"
                    )
                if not isinstance(invocation.get("stdout"), str) or not isinstance(
                    invocation.get("stderr"), str
                ):
                    raise CFixtureFailure(
                        "C fixture invocation omitted captured output"
                    )
                phase = invocation.get("phase")
                if not isinstance(phase, str):
                    raise CFixtureFailure(
                        "C fixture invocation omitted its phase"
                    )
                expected_exit = expected["exitCode"] if phase == "run" else 0
                if invocation.get("exitCode") != expected_exit:
                    raise CFixtureFailure(
                        "C fixture report recorded an unexpected invocation exit"
                    )
                if phase == "run" and (
                    invocation.get("stdout") != expected["stdout"]
                    or invocation.get("stderr") != expected["stderr"]
                ):
                    raise CFixtureFailure(
                        "C fixture report recorded unexpected process output"
                    )
                phases.append(phase)
            if "link" not in phases or "run" not in phases or not any(
                phase.startswith("compile:") for phase in phases
            ):
                raise CFixtureFailure(
                    "C fixture report did not record compile, link, and run"
                )
    missing = sorted(required_coverage - covered)
    if missing:
        raise CFixtureFailure(
            "C fixture corpus missed coverage: " + ", ".join(missing)
        )


def report_json(report: Mapping[str, object], *, compact: bool = False) -> str:
    if compact:
        return json.dumps(
            report, ensure_ascii=False, sort_keys=True, separators=(",", ":")
        )
    return json.dumps(report, ensure_ascii=False, indent=2) + "\n"


def write_report(path: Path, report: Mapping[str, object]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(report_json(report), encoding="utf-8")

#!/usr/bin/env python3
"""Validate fixture taxonomy, snapshot ownership, and explicit example assertions."""

from __future__ import annotations

import importlib.util
import json
import sys
from pathlib import Path, PurePosixPath
from types import ModuleType
from typing import Any


ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "docs/specs/fixture-taxonomy.json"
CATALOG_SCHEMA = ROOT / "docs/specs/fixture-taxonomy.schema.json"
CASE_SCHEMA = ROOT / "docs/specs/fixture-case.schema.json"
PACKAGE = ROOT / "package.json"
PRE_COMMIT = ROOT / "scripts/hooks/pre-commit"
WORKFLOW = ROOT / ".github/workflows/governance.yml"
EXPECTED_TYPES = {
    "positive",
    "negative",
    "ast",
    "snapshot",
    "runtime",
    "differential",
    "abi",
    "performance",
}
SEMANTIC_ASSERTIONS = {
    "stdout",
    "stderr",
    "diagnostics",
    "artifacts",
    "runtimePlan",
    "oracle",
    "metrics",
}


def load_json(path: Path, errors: list[str]) -> dict[str, Any]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        errors.append(f"cannot read {path.relative_to(ROOT)}: {error}")
        return {}
    if not isinstance(value, dict):
        errors.append(f"{path.relative_to(ROOT)} must contain a JSON object")
        return {}
    return value


def read_text(path: Path, errors: list[str]) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        errors.append(f"cannot read {path.relative_to(ROOT)}: {error}")
        return ""


def safe_path(value: Any, label: str, errors: list[str]) -> Path | None:
    if not isinstance(value, str) or not value or "\\" in value:
        errors.append(f"{label} must be a non-empty POSIX relative path")
        return None
    relative = PurePosixPath(value)
    if (
        relative.is_absolute()
        or relative.as_posix() != value
        or any(part in ("", ".", "..") for part in relative.parts)
    ):
        errors.append(f"{label} must be a normalized POSIX relative path")
        return None
    candidate = ROOT.joinpath(*relative.parts)
    try:
        candidate.resolve(strict=False).relative_to(ROOT.resolve())
    except ValueError:
        errors.append(f"{label} escapes the repository")
        return None
    return candidate


def load_snapshot_module(errors: list[str]) -> ModuleType | None:
    path = ROOT / "scripts/test/snapshots.py"
    spec = importlib.util.spec_from_file_location("hxc_snapshot_policy", path)
    if spec is None or spec.loader is None:
        errors.append("cannot load scripts/test/snapshots.py")
        return None
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    except Exception as error:  # policy must report an import failure cleanly
        errors.append(f"cannot import scripts/test/snapshots.py: {error}")
        return None
    return module


def validate_case(
    path: Path,
    case: dict[str, Any],
    *,
    expected_role: str | None,
    errors: list[str],
) -> None:
    label = path.relative_to(ROOT).as_posix()
    if case.get("schemaVersion") != 1:
        errors.append(f"{label} must use schemaVersion 1")
    if expected_role is not None and case.get("role") != expected_role:
        errors.append(f"{label} must declare role {expected_role!r}")
    if case.get("type") not in EXPECTED_TYPES:
        errors.append(f"{label} has an unknown fixture type")
    if case.get("status") not in {"active", "planned", "disabled"}:
        errors.append(f"{label} has an invalid status")
    sources = case.get("sources")
    if not isinstance(sources, list) or not sources:
        errors.append(f"{label} must declare at least one source")
    else:
        for index, source in enumerate(sources):
            source_path = safe_path(source, f"{label}.sources[{index}]", errors)
            if (
                source_path is not None
                and case.get("status") == "active"
                and not source_path.is_file()
            ):
                errors.append(
                    f"{label}.sources[{index}] is missing: "
                    f"{source_path.relative_to(ROOT)}"
                )
    runner = case.get("runner")
    if not isinstance(runner, dict):
        errors.append(f"{label} must declare a runner object")
    else:
        command = runner.get("command")
        if not isinstance(command, list) or not command or not all(
            isinstance(part, str) and part for part in command
        ):
            errors.append(f"{label} runner.command must be a non-empty argument array")
        if not isinstance(runner.get("timeoutSeconds"), int):
            errors.append(f"{label} runner.timeoutSeconds must be an integer")
        if not isinstance(runner.get("network"), bool):
            errors.append(f"{label} runner.network must be boolean")
        working_directory = safe_path(
            runner.get("workingDirectory"),
            f"{label}.runner.workingDirectory",
            errors,
        )
        if (
            working_directory is not None
            and case.get("status") == "active"
            and not working_directory.is_dir()
        ):
            errors.append(
                f"{label} runner working directory is missing: "
                f"{working_directory.relative_to(ROOT)}"
            )
    expected = case.get("expected")
    if not isinstance(expected, dict) or not isinstance(expected.get("exitCode"), int):
        errors.append(f"{label} expected must declare an integer exitCode")
    elif not SEMANTIC_ASSERTIONS.intersection(expected):
        errors.append(f"{label} must declare at least one semantic expected assertion")


def expected_output_files() -> set[Path]:
    files: set[Path] = set()
    test_root = ROOT / "test"
    for path in test_root.rglob("*"):
        if not path.is_file():
            continue
        relative = path.relative_to(test_root)
        parents = relative.parts[:-1]
        if (
            any(part == "expected" or part.startswith("expected_") for part in parents)
            or relative.name.startswith("expected.")
            or relative.name.startswith("expected_")
        ):
            files.add(path)
    return files


def validate() -> list[str]:
    errors: list[str] = []
    catalog = load_json(CATALOG, errors)
    catalog_schema = load_json(CATALOG_SCHEMA, errors)
    case_schema = load_json(CASE_SCHEMA, errors)
    package = load_json(PACKAGE, errors)

    if catalog.get("schemaVersion") != 1:
        errors.append("fixture taxonomy must use schemaVersion 1")
    if catalog_schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        errors.append("fixture taxonomy schema must use JSON Schema 2020-12")
    if case_schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        errors.append("fixture case schema must use JSON Schema 2020-12")
    case_schema_path = safe_path(catalog.get("caseSchema"), "caseSchema", errors)
    if case_schema_path is not None and case_schema_path != CASE_SCHEMA:
        errors.append("fixture taxonomy must point to the canonical fixture-case schema")

    runner_contract = catalog.get("runnerContract")
    if not isinstance(runner_contract, dict):
        errors.append("fixture taxonomy omitted runnerContract")
    else:
        if runner_contract.get("caseManifest") != "case.json":
            errors.append("runner contract must use case.json")
        if runner_contract.get("networkDefault") is not False:
            errors.append("fixture network access must default to false")
        required = runner_contract.get("requiredExpectedFields")
        if not isinstance(required, list) or "exitCode" not in required:
            errors.append("runner contract must require an expected exitCode")

    raw_types = catalog.get("types")
    type_entries: dict[str, dict[str, Any]] = {}
    if not isinstance(raw_types, list):
        errors.append("fixture taxonomy types must be an array")
        raw_types = []
    for index, entry in enumerate(raw_types):
        if not isinstance(entry, dict) or not isinstance(entry.get("id"), str):
            errors.append(f"types[{index}] must be an object with an ID")
            continue
        identifier = entry["id"]
        if identifier in type_entries:
            errors.append(f"duplicate fixture type {identifier}")
            continue
        type_entries[identifier] = entry
        directory = safe_path(entry.get("directory"), f"types[{index}].directory", errors)
        if directory is not None:
            if not directory.is_dir():
                errors.append(f"fixture type directory is missing: {directory.relative_to(ROOT)}")
            elif not (directory / "README.md").is_file():
                errors.append(f"fixture type directory lacks README.md: {directory.relative_to(ROOT)}")
        outputs = entry.get("expectedOutputs")
        if not isinstance(outputs, list) or not outputs:
            errors.append(f"fixture type {identifier} must name expected outputs")
        if entry.get("status") not in {"active", "seed-only", "contract-only"}:
            errors.append(f"fixture type {identifier} has an invalid status")
    if set(type_entries) != EXPECTED_TYPES:
        errors.append(
            f"fixture types must be exactly {sorted(EXPECTED_TYPES)!r}; got {sorted(type_entries)!r}"
        )

    case_ids: set[str] = set()
    for identifier, entry in type_entries.items():
        directory = safe_path(
            entry.get("directory"), f"fixture type {identifier} directory", errors
        )
        if directory is None or not directory.is_dir():
            continue
        for child in sorted(path for path in directory.iterdir() if path.is_dir()):
            manifest = child / "case.json"
            if not manifest.is_file():
                errors.append(
                    "canonical fixture case lacks case.json: "
                    + manifest.relative_to(ROOT).as_posix()
                )
                continue
            case = load_json(manifest, errors)
            validate_case(
                manifest,
                case,
                expected_role="fixture",
                errors=errors,
            )
            if case.get("type") != identifier:
                errors.append(
                    f"{manifest.relative_to(ROOT)} must declare type {identifier!r}"
                )
            case_id = case.get("id")
            if isinstance(case_id, str):
                if case_id in case_ids:
                    errors.append(f"duplicate canonical fixture case ID: {case_id}")
                case_ids.add(case_id)

    raw_suites = catalog.get("suites")
    suites: dict[str, dict[str, Any]] = {}
    if not isinstance(raw_suites, list):
        errors.append("fixture taxonomy suites must be an array")
        raw_suites = []
    for index, entry in enumerate(raw_suites):
        if not isinstance(entry, dict) or not isinstance(entry.get("id"), str):
            errors.append(f"suites[{index}] must be an object with an ID")
            continue
        identifier = entry["id"]
        if identifier in suites:
            errors.append(f"duplicate fixture suite {identifier}")
            continue
        suites[identifier] = entry
        directory = safe_path(entry.get("directory"), f"suites[{index}].directory", errors)
        if directory is not None and not directory.is_dir():
            errors.append(f"fixture suite directory is missing: {directory.relative_to(ROOT)}")
        runner = entry.get("runner")
        if not isinstance(runner, list) or not runner or not all(
            isinstance(part, str) and part for part in runner
        ):
            errors.append(f"fixture suite {identifier} runner must be an argument array")
        elif len(runner) >= 2 and runner[0].startswith("python"):
            runner_path = safe_path(runner[1], f"suite {identifier} runner", errors)
            if runner_path is not None and not runner_path.is_file():
                errors.append(f"fixture suite runner is missing: {runner_path.relative_to(ROOT)}")
        suite_types = entry.get("types")
        if not isinstance(suite_types, list) or not suite_types:
            errors.append(f"fixture suite {identifier} must map at least one type")
        else:
            unknown = sorted(set(suite_types) - EXPECTED_TYPES)
            if unknown:
                errors.append(f"fixture suite {identifier} maps unknown types: {unknown!r}")
        if not isinstance(entry.get("assertions"), list) or not entry["assertions"]:
            errors.append(f"fixture suite {identifier} must declare its assertions")

    for identifier, entry in type_entries.items():
        suite_ids = entry.get("suiteIds")
        if not isinstance(suite_ids, list):
            errors.append(f"fixture type {identifier} suiteIds must be an array")
            continue
        unknown = sorted(set(suite_ids) - set(suites))
        if unknown:
            errors.append(f"fixture type {identifier} maps unknown suites: {unknown!r}")
        for suite_id in suite_ids:
            suite_types = suites.get(suite_id, {}).get("types", [])
            if identifier not in suite_types:
                errors.append(f"fixture type/suite mapping is not reciprocal: {identifier}/{suite_id}")
    for suite_id, entry in suites.items():
        for identifier in entry.get("types", []):
            if suite_id not in type_entries.get(identifier, {}).get("suiteIds", []):
                errors.append(f"fixture suite/type mapping is not reciprocal: {suite_id}/{identifier}")

    policy = catalog.get("snapshotPolicy")
    managed_ids: set[str] = set()
    managed_files: set[Path] = set()
    if not isinstance(policy, dict):
        errors.append("fixture taxonomy omitted snapshotPolicy")
    else:
        if policy.get("checkCommand") != "npm run snapshots:check":
            errors.append("snapshot check command drifted")
        if policy.get("updateCommand") != "npm run snapshots:update -- --suite <suite-id>":
            errors.append("snapshot update command drifted")
        if policy.get("ciMayUpdate") is not False:
            errors.append("CI must not update snapshots")
        entries = policy.get("managedSuites")
        if not isinstance(entries, list) or not entries:
            errors.append("snapshot policy must register managed suites")
            entries = []
        for index, entry in enumerate(entries):
            if not isinstance(entry, dict) or not isinstance(entry.get("id"), str):
                errors.append(f"managedSuites[{index}] must have an ID")
                continue
            identifier = entry["id"]
            if identifier in managed_ids:
                errors.append(f"duplicate managed snapshot suite {identifier}")
            managed_ids.add(identifier)
            if identifier not in suites or "snapshot" not in suites[identifier].get("types", []):
                errors.append(f"managed snapshot suite {identifier} is not a snapshot suite")
            roots = entry.get("expectedRoots")
            if not isinstance(roots, list) or not roots:
                errors.append(f"managed snapshot suite {identifier} has no expected roots")
                continue
            for root_index, value in enumerate(roots):
                root = safe_path(value, f"managedSuites[{index}].expectedRoots[{root_index}]", errors)
                if root is None:
                    continue
                if not root.exists():
                    errors.append(f"managed expected root is missing: {root.relative_to(ROOT)}")
                elif root.is_file():
                    managed_files.add(root)
                elif root.is_dir():
                    managed_files.update(path for path in root.rglob("*") if path.is_file())

    snapshot_module = load_snapshot_module(errors)
    if snapshot_module is not None:
        runner_ids = set(snapshot_module.registered_suite_ids())
        if runner_ids != managed_ids:
            errors.append(
                f"snapshot catalog/runner mismatch: catalog={sorted(managed_ids)!r}, runner={sorted(runner_ids)!r}"
            )

    unowned_expected = sorted(
        expected_output_files() - managed_files,
        key=lambda path: path.relative_to(ROOT).as_posix(),
    )
    if unowned_expected:
        errors.append(
            "expected output files are not owned by the snapshot registry: "
            + ", ".join(path.relative_to(ROOT).as_posix() for path in unowned_expected)
        )

    examples = catalog.get("examplesPolicy")
    if not isinstance(examples, dict):
        errors.append("fixture taxonomy omitted examplesPolicy")
    else:
        root = safe_path(examples.get("directory"), "examplesPolicy.directory", errors)
        status = examples.get("status")
        if root is not None and status == "absent" and root.exists():
            errors.append("examplesPolicy says absent but examples/ exists; register every example case")
        elif root is not None and status == "active":
            if not root.is_dir():
                errors.append("examplesPolicy says active but examples/ is missing")
            else:
                children = sorted(path for path in root.iterdir() if path.is_dir())
                if not children:
                    errors.append("active examples/ must contain at least one example directory")
                for child in children:
                    manifest = child / str(examples.get("caseManifest", "case.json"))
                    if not manifest.is_file():
                        errors.append(f"example lacks declared assertions: {manifest.relative_to(ROOT)}")
                        continue
                    validate_case(
                        manifest,
                        load_json(manifest, errors),
                        expected_role="example",
                        errors=errors,
                    )
        elif status not in {"absent", "active"}:
            errors.append("examplesPolicy status must be absent or active")

    scripts = package.get("scripts")
    if not isinstance(scripts, dict):
        errors.append("package.json scripts must be an object")
        scripts = {}
    expected_scripts = {
        "test:fixture-policy": "python3 scripts/ci/check_fixture_policy.py",
        "snapshots:check": "python3 scripts/test/snapshots.py --check",
        "snapshots:update": "python3 scripts/test/snapshots.py --update",
    }
    for name, command in expected_scripts.items():
        if scripts.get(name) != command:
            errors.append(f"package.json {name} must equal {command!r}")
    if "npm run snapshots:check" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must check the central snapshot registry")
    if "npm run test:fixture-policy" not in str(scripts.get("test:governance", "")):
        errors.append("package.json test:governance must validate fixture policy")

    pre_commit = read_text(PRE_COMMIT, errors)
    if "scripts/ci/check_fixture_policy.py" not in pre_commit:
        errors.append("pre-commit must validate fixture policy for relevant changes")
    workflow = read_text(WORKFLOW, errors)
    if "snapshots:update" in workflow:
        errors.append("CI workflow must never invoke snapshot update mode")
    if "npm run test:toolchain" not in workflow:
        errors.append("CI workflow must retain the toolchain/snapshot gate")
    if not (ROOT / "docs/testing.md").is_file():
        errors.append("human-readable fixture policy is missing: docs/testing.md")

    return errors


def main() -> int:
    errors = validate()
    if errors:
        for error in errors:
            print(f"fixture-policy: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "fixture-policy: OK: 8 lanes, canonical case manifests, mapped runners/expected outputs, "
        "explicit snapshot updates, and example assertions"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

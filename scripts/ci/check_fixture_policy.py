#!/usr/bin/env python3
"""Validate fixture taxonomy, snapshot ownership, and explicit example assertions."""

from __future__ import annotations

import importlib.util
import json
import re
import sys
from pathlib import Path, PurePosixPath
from types import ModuleType
from typing import Any


ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "docs/specs/fixture-taxonomy.json"
CATALOG_SCHEMA = ROOT / "docs/specs/fixture-taxonomy.schema.json"
CASE_SCHEMA = ROOT / "docs/specs/fixture-case.schema.json"
SURFACE_SCORECARDS = ROOT / "docs/specs/test-surface-scorecards.json"
SURFACE_SCORECARDS_SCHEMA = ROOT / "docs/specs/test-surface-scorecards.schema.json"
CAPABILITY_MANIFEST = ROOT / "docs/specs/bootstrap-inventory.json"
BEADS_PLAN = ROOT / "docs/specs/beads-plan.json"
PACKAGE = ROOT / "package.json"
PRE_COMMIT = ROOT / "scripts/hooks/pre-commit"
WORKFLOW = ROOT / ".github/workflows/governance.yml"
SNAPSHOT_WORKFLOW = ROOT / ".github/workflows/snapshot-audit.yml"
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
EXPECTED_PRODUCT_SURFACES = {
    "compiler-admitted-slices",
    "c-abi-native-ffi",
    "runtime-memory-lifetime",
    "toolchain-platform-portability",
    "diagnostics-source-mapping-downstream",
}
EXAMPLE_TIERS = {
    "flagship-application",
    "capability-showcase",
    "compile-only-snippet",
}
REQUIRED_SCORECARD_FIELDS = {
    "id",
    "name",
    "archetype",
    "status",
    "ownerBeads",
    "protectedClaims",
    "capabilityIds",
    "focusedOwners",
    "verticalOwners",
    "systemOwners",
    "affectedExtendedOwners",
    "oracles",
    "evidence",
    "selectorOwnership",
    "fullBackstopCommand",
    "releaseCommand",
    "lastObserved",
    "residualRisks",
}
SCORECARD_ROOT_FIELDS = {
    "$schema",
    "schemaVersion",
    "manifestId",
    "capabilityAuthority",
    "policyOnlyOwners",
    "surfaces",
}
SCORECARD_ARCHETYPES = {
    "compiler-admitted-slices": "compiler",
    "c-abi-native-ffi": "native-metal",
    "runtime-memory-lifetime": "runtime",
    "toolchain-platform-portability": "portability",
    "diagnostics-source-mapping-downstream": "diagnostics-downstream",
}
SCORECARD_EVIDENCE_FIELDS = {"path", "proves", "doesNotProve"}
SCORECARD_SELECTOR_FIELDS = {"semanticOwners", "affectedRing", "unknownFallback"}
SCORECARD_OBSERVED_FIELDS = {"state", "evidence"}
CASE_FIELDS = {
    "schemaVersion",
    "id",
    "role",
    "type",
    "status",
    "requirements",
    "productSurfaces",
    "behaviorScenario",
    "oracleProvenance",
    "tracerBullet",
    "example",
    "sources",
    "runner",
    "expected",
}
SCENARIO_FIELDS = {
    "preconditions",
    "action",
    "observableResult",
    "edgeBehavior",
    "protectedClaim",
}
PROVENANCE_FIELDS = {"kind", "source", "independence", "reviewRule"}
TRACER_FIELDS = {"claim", "path"}
EXAMPLE_FIELDS = {
    "tier",
    "owner",
    "executionKind",
    "compilerToolchains",
    "platformAssumptions",
    "runtimeExpectation",
    "sanitizerRelevance",
    "advertisedClaim",
}
RUNNER_FIELDS = {"command", "workingDirectory", "timeoutSeconds", "network"}
EXPECTED_FIELDS = {
    "exitCode",
    "stdout",
    "stderr",
    "diagnostics",
    "artifacts",
    "runtimePlan",
    "oracle",
    "metrics",
}
STREAM_ASSERTION_FIELDS = {"mode", "value", "path"}
ARTIFACT_FIELDS = {"path", "format", "comparison"}
ARTIFACT_FORMATS = {"text", "json", "c", "header", "hxcir", "binary", "metrics"}
ARTIFACT_COMPARISONS = {"exact", "semantic-json", "symbols", "layout", "budget"}
FULL_BACKSTOP_COMMAND = "npm run test:toolchain:parallel -- --with-native"
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


def is_json_integer(value: Any) -> bool:
    """JSON integers exclude Python booleans even though bool subclasses int."""
    return isinstance(value, int) and not isinstance(value, bool)


def validate_stream_assertion(value: Any, label: str, errors: list[str]) -> None:
    if not isinstance(value, dict):
        errors.append(f"{label} must be an object")
        return
    extra_fields = sorted(set(value) - STREAM_ASSERTION_FIELDS)
    if extra_fields:
        errors.append(f"{label} has undeclared fields: {extra_fields!r}")
    mode = value.get("mode")
    if mode not in {"empty", "exact", "contains", "regex", "json"}:
        errors.append(f"{label}.mode is invalid")
        return
    has_value = "value" in value
    has_path = "path" in value
    if mode == "empty":
        if has_value or has_path:
            errors.append(f"{label} empty mode cannot declare value or path")
        return
    if has_value == has_path:
        errors.append(f"{label} must declare exactly one of value or path")
    if has_value and not isinstance(value.get("value"), str):
        errors.append(f"{label}.value must be a string")
    if has_path:
        assertion_path = safe_path(value.get("path"), f"{label}.path", errors)
        if assertion_path is not None and not assertion_path.is_file():
            errors.append(f"{label}.path is missing: {assertion_path.relative_to(ROOT)}")


def scorecard_owner_surfaces() -> tuple[dict[str, set[str]], set[str]]:
    """Return the checked product owners and explicit policy-only exclusions."""
    scorecards = json.loads(SURFACE_SCORECARDS.read_text(encoding="utf-8"))
    ownership: dict[str, set[str]] = {}
    for surface in scorecards.get("surfaces", []):
        if not isinstance(surface, dict) or not isinstance(surface.get("id"), str):
            continue
        for field in (
            "focusedOwners",
            "verticalOwners",
            "systemOwners",
            "affectedExtendedOwners",
        ):
            owners = surface.get(field, [])
            if not isinstance(owners, list):
                continue
            for owner in owners:
                if isinstance(owner, str):
                    ownership.setdefault(owner, set()).add(surface["id"])
    policy_only = {
        owner
        for owner in scorecards.get("policyOnlyOwners", [])
        if isinstance(owner, str)
    }
    return ownership, policy_only


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
    required_case_fields = {
        "schemaVersion",
        "id",
        "role",
        "type",
        "status",
        "sources",
        "runner",
        "expected",
    }
    missing_case_fields = sorted(required_case_fields - set(case))
    if missing_case_fields:
        errors.append(f"{label} omitted required case fields: {missing_case_fields!r}")
    extra_case_fields = sorted(set(case) - CASE_FIELDS)
    if extra_case_fields:
        errors.append(f"{label} has undeclared case fields: {extra_case_fields!r}")
    if case.get("schemaVersion") != 1:
        errors.append(f"{label} must use schemaVersion 1")
    if not isinstance(case.get("id"), str) or re.fullmatch(
        r"[a-z0-9][a-z0-9-]*", case.get("id", "")
    ) is None:
        errors.append(f"{label} must declare a canonical case ID")
    if case.get("role") not in {"fixture", "example"}:
        errors.append(f"{label} has an invalid role")
    if expected_role is not None and case.get("role") != expected_role:
        errors.append(f"{label} must declare role {expected_role!r}")
    if case.get("type") not in EXPECTED_TYPES:
        errors.append(f"{label} has an unknown fixture type")
    if case.get("status") not in {"active", "planned", "disabled"}:
        errors.append(f"{label} has an invalid status")
    requirements = case.get("requirements")
    if requirements is not None:
        if not isinstance(requirements, list) or not all(
            isinstance(requirement, str)
            and re.fullmatch(r"HXC-[A-Z]+-[0-9]{3}", requirement)
            for requirement in requirements
        ):
            errors.append(f"{label} requirements must use canonical requirement IDs")
        elif len(requirements) != len(set(requirements)):
            errors.append(f"{label} requirements must be unique")
    sources = case.get("sources")
    if not isinstance(sources, list) or not sources:
        errors.append(f"{label} must declare at least one source")
    else:
        if len(sources) != len(set(sources)):
            errors.append(f"{label} sources must be unique")
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
        extra_runner_fields = sorted(set(runner) - RUNNER_FIELDS)
        missing_runner_fields = sorted(RUNNER_FIELDS - set(runner))
        if extra_runner_fields:
            errors.append(f"{label} runner has undeclared fields: {extra_runner_fields!r}")
        if missing_runner_fields:
            errors.append(f"{label} runner omitted fields: {missing_runner_fields!r}")
        command = runner.get("command")
        if not isinstance(command, list) or not command or not all(
            isinstance(part, str) and part for part in command
        ):
            errors.append(f"{label} runner.command must be a non-empty argument array")
        timeout = runner.get("timeoutSeconds")
        if not is_json_integer(timeout) or not 1 <= timeout <= 3600:
            errors.append(f"{label} runner.timeoutSeconds must be an integer from 1 to 3600")
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
    if not isinstance(expected, dict):
        errors.append(f"{label} expected must be an object")
    else:
        if not is_json_integer(expected.get("exitCode")):
            errors.append(f"{label} expected must declare an integer exitCode")
        extra_expected_fields = sorted(set(expected) - EXPECTED_FIELDS)
        if extra_expected_fields:
            errors.append(
                f"{label} expected has undeclared fields: {extra_expected_fields!r}"
            )
        if not SEMANTIC_ASSERTIONS.intersection(expected):
            errors.append(f"{label} must declare at least one semantic expected assertion")
        for stream in ("stdout", "stderr"):
            if stream in expected:
                validate_stream_assertion(
                    expected[stream], f"{label}.expected.{stream}", errors
                )
        diagnostics = expected.get("diagnostics")
        if diagnostics is not None:
            if (
                not isinstance(diagnostics, list)
                or not diagnostics
                or not all(
                    isinstance(diagnostic, str)
                    and re.fullmatch(r"HXC[0-9]{4}", diagnostic)
                    for diagnostic in diagnostics
                )
                or len(diagnostics) != len(set(diagnostics))
            ):
                errors.append(
                    f"{label}.expected.diagnostics must be unique canonical IDs"
                )
        artifacts = expected.get("artifacts")
        if artifacts is not None:
            if not isinstance(artifacts, list) or not artifacts:
                errors.append(f"{label}.expected.artifacts must be a non-empty array")
            else:
                for index, artifact in enumerate(artifacts):
                    artifact_label = f"{label}.expected.artifacts[{index}]"
                    if not isinstance(artifact, dict):
                        errors.append(f"{artifact_label} must be an object")
                        continue
                    if set(artifact) != ARTIFACT_FIELDS:
                        errors.append(
                            f"{artifact_label} fields must match the schema exactly"
                        )
                    artifact_path = safe_path(
                        artifact.get("path"), f"{artifact_label}.path", errors
                    )
                    if artifact_path is not None and not artifact_path.exists():
                        errors.append(
                            f"{artifact_label}.path is missing: "
                            f"{artifact_path.relative_to(ROOT)}"
                        )
                    if artifact.get("format") not in ARTIFACT_FORMATS:
                        errors.append(f"{artifact_label}.format is invalid")
                    if artifact.get("comparison") not in ARTIFACT_COMPARISONS:
                        errors.append(f"{artifact_label}.comparison is invalid")
        runtime_plan = expected.get("runtimePlan")
        if isinstance(runtime_plan, dict):
            if set(runtime_plan) != {"policy", "features"}:
                errors.append(
                    f"{label}.expected.runtimePlan fields must match the schema exactly"
                )
            if runtime_plan.get("policy") not in {"auto", "minimal", "none"}:
                errors.append(f"{label}.expected.runtimePlan.policy is invalid")
            features = runtime_plan.get("features")
            if (
                not isinstance(features, list)
                or not all(isinstance(feature, str) and feature for feature in features)
                or len(features) != len(set(features))
            ):
                errors.append(
                    f"{label}.expected.runtimePlan.features must be unique strings"
                )
        for assertion in ("runtimePlan", "oracle", "metrics"):
            if assertion in expected:
                if assertion == "runtimePlan" and isinstance(expected[assertion], dict):
                    continue
                assertion_path = safe_path(
                    expected[assertion], f"{label}.expected.{assertion}", errors
                )
                if assertion_path is not None and not assertion_path.is_file():
                    errors.append(
                        f"{label}.expected.{assertion} is missing: "
                        f"{assertion_path.relative_to(ROOT)}"
                    )

    if case.get("role") != "example":
        return

    product_surfaces = case.get("productSurfaces")
    if not isinstance(product_surfaces, list) or not product_surfaces:
        errors.append(f"{label} example must declare productSurfaces")
    else:
        if len(product_surfaces) != len(set(product_surfaces)):
            errors.append(f"{label} example productSurfaces must be unique")
        unknown_surfaces = sorted(set(product_surfaces) - EXPECTED_PRODUCT_SURFACES)
        if unknown_surfaces:
            errors.append(
                f"{label} example declares unknown product surfaces: {unknown_surfaces!r}"
            )

    scenario = case.get("behaviorScenario")
    if not isinstance(scenario, dict) or not all(
        isinstance(scenario.get(field), str) and scenario[field]
        for field in SCENARIO_FIELDS
    ):
        errors.append(
            f"{label} example must declare a complete observable behaviorScenario"
        )
    elif set(scenario) != SCENARIO_FIELDS:
        errors.append(f"{label} behaviorScenario fields must match the schema exactly")

    provenance = case.get("oracleProvenance")
    if not isinstance(provenance, dict):
        errors.append(f"{label} example must declare oracleProvenance")
    else:
        if set(provenance) != PROVENANCE_FIELDS:
            errors.append(f"{label} oracleProvenance fields must match the schema exactly")
        if provenance.get("kind") not in {
            "specification",
            "manual-expectation",
            "pinned-differential-reference",
            "invariant",
            "reviewed-golden",
            "real-consumer",
        }:
            errors.append(f"{label} example has an invalid oracle provenance kind")
        oracle_source = safe_path(
            provenance.get("source"), f"{label}.oracleProvenance.source", errors
        )
        if oracle_source is not None and not oracle_source.is_file():
            errors.append(
                f"{label} oracle provenance source is missing: "
                f"{oracle_source.relative_to(ROOT)}"
            )
        for field in ("independence", "reviewRule"):
            if not isinstance(provenance.get(field), str) or not provenance[field]:
                errors.append(f"{label} oracleProvenance.{field} must be non-empty")

    example = case.get("example")
    if not isinstance(example, dict):
        errors.append(f"{label} must declare example tier and execution assumptions")
    else:
        if set(example) != EXAMPLE_FIELDS:
            errors.append(f"{label} example fields must match the schema exactly")
        if example.get("tier") not in EXAMPLE_TIERS:
            errors.append(f"{label} has an invalid example tier")
        if example.get("executionKind") not in {"portable", "native-metal", "mixed"}:
            errors.append(f"{label} has an invalid example executionKind")
        for field in ("compilerToolchains", "platformAssumptions"):
            values = example.get(field)
            if not isinstance(values, list) or not values or not all(
                isinstance(value, str) and value for value in values
            ):
                errors.append(f"{label} example.{field} must be a non-empty string array")
        for field in ("runtimeExpectation", "sanitizerRelevance", "advertisedClaim"):
            if not isinstance(example.get(field), str) or not example[field]:
                errors.append(f"{label} example.{field} must be non-empty")
        owner = example.get("owner")
        owner_surfaces, policy_only = scorecard_owner_surfaces()
        package_scripts = json.loads(PACKAGE.read_text(encoding="utf-8"))["scripts"]
        if not isinstance(owner, str) or owner not in package_scripts:
            errors.append(f"{label} example.owner must name a package script")
        elif owner in policy_only:
            errors.append(f"{label} example.owner cannot be policy-only")
        elif set(product_surfaces or []) != owner_surfaces.get(owner, set()):
            errors.append(
                f"{label} productSurfaces must exactly match scorecard ownership for {owner}"
            )

    tracer = case.get("tracerBullet")
    if tracer is not None:
        if not isinstance(tracer, dict) or set(tracer) != TRACER_FIELDS:
            errors.append(f"{label} tracerBullet fields must match the schema exactly")
        else:
            tracer_path = tracer.get("path")
            if (
                not isinstance(tracer.get("claim"), str)
                or not tracer["claim"]
                or not isinstance(tracer_path, list)
                or len(tracer_path) < 4
                or not all(isinstance(step, str) and step for step in tracer_path)
            ):
                errors.append(f"{label} tracerBullet must declare a claim and at least four steps")


def validate_surface_scorecards(
    scorecards: dict[str, Any],
    capability_manifest: dict[str, Any],
    package_scripts: dict[str, Any],
    stable_owner_ids: set[str],
    errors: list[str],
) -> None:
    """Keep claim-bearing evidence attached to one explicit product surface."""
    if scorecards.get("schemaVersion") != 1:
        errors.append("test surface scorecards must use schemaVersion 1")
    if scorecards.get("$schema") != SURFACE_SCORECARDS_SCHEMA.name:
        errors.append("test surface scorecards must point to their canonical schema")
    if scorecards.get("capabilityAuthority") != CAPABILITY_MANIFEST.relative_to(ROOT).as_posix():
        errors.append("test surface scorecards must reference the capability manifest authority")
    if scorecards.get("manifestId") != "hxc-test-surfaces-v1":
        errors.append("test surface scorecards must use the canonical manifest ID")
    missing_root_fields = sorted(SCORECARD_ROOT_FIELDS - set(scorecards))
    if missing_root_fields:
        errors.append(
            f"test surface scorecards omitted root fields: {missing_root_fields!r}"
        )
    extra_root_fields = sorted(set(scorecards) - SCORECARD_ROOT_FIELDS)
    if extra_root_fields:
        errors.append(
            f"test surface scorecards have undeclared root fields: {extra_root_fields!r}"
        )
    policy_only = scorecards.get("policyOnlyOwners")
    if (
        not isinstance(policy_only, list)
        or not policy_only
        or not all(isinstance(owner, str) and owner in package_scripts for owner in policy_only)
        or len(policy_only) != len(set(policy_only))
    ):
        errors.append(
            "test surface scorecards policyOnlyOwners must be unique package scripts"
        )
        policy_only_set: set[str] = set()
    else:
        policy_only_set = set(policy_only)

    capabilities = capability_manifest.get("capabilities")
    capability_ids = (
        {
            entry.get("id")
            for entry in capabilities
            if isinstance(entry, dict)
        }
        if isinstance(capabilities, list)
        else set()
    )
    surfaces = scorecards.get("surfaces")
    if not isinstance(surfaces, list):
        errors.append("test surface scorecards surfaces must be an array")
        return

    seen: set[str] = set()
    for index, surface in enumerate(surfaces):
        label = f"test surface scorecards surfaces[{index}]"
        if not isinstance(surface, dict):
            errors.append(f"{label} must be an object")
            continue
        missing_fields = sorted(REQUIRED_SCORECARD_FIELDS - set(surface))
        if missing_fields:
            errors.append(f"{label} omitted required fields: {missing_fields!r}")
        extra_fields = sorted(set(surface) - REQUIRED_SCORECARD_FIELDS)
        if extra_fields:
            errors.append(f"{label} has undeclared fields: {extra_fields!r}")
        identifier = surface.get("id")
        if not isinstance(identifier, str):
            errors.append(f"{label} must declare an ID")
            continue
        if identifier in seen:
            errors.append(f"duplicate test product surface {identifier}")
        seen.add(identifier)

        expected_archetype = SCORECARD_ARCHETYPES.get(identifier)
        if surface.get("archetype") != expected_archetype:
            errors.append(
                f"test product surface {identifier} must use archetype {expected_archetype!r}"
            )

        for text_field in ("name", "fullBackstopCommand"):
            if not isinstance(surface.get(text_field), str) or not surface[text_field]:
                errors.append(f"test product surface {identifier} {text_field} must be non-empty")
        if surface.get("status") not in {"admitted-slice", "partial", "unsupported"}:
            errors.append(f"test product surface {identifier} has an invalid status")
        for list_field in (
            "ownerBeads",
            "protectedClaims",
            "capabilityIds",
            "systemOwners",
            "oracles",
            "residualRisks",
        ):
            values = surface.get(list_field)
            if not isinstance(values, list) or not values or not all(
                isinstance(value, str) and value for value in values
            ):
                errors.append(
                    f"test product surface {identifier} {list_field} must be a non-empty string array"
                )
            elif len(values) != len(set(values)):
                errors.append(
                    f"test product surface {identifier} {list_field} must contain unique values"
                )

        unknown_owner_ids = sorted(set(surface.get("ownerBeads", [])) - stable_owner_ids)
        if unknown_owner_ids:
            errors.append(
                f"test product surface {identifier} references unknown stable Beads owners: "
                f"{unknown_owner_ids!r}"
            )

        unknown_capabilities = sorted(
            set(surface.get("capabilityIds", [])) - capability_ids
        )
        if unknown_capabilities:
            errors.append(
                f"test product surface {identifier} references unknown capabilities: "
                f"{unknown_capabilities!r}"
            )
        for owner_field in (
            "focusedOwners",
            "verticalOwners",
            "affectedExtendedOwners",
        ):
            owners = surface.get(owner_field)
            if not isinstance(owners, list) or not owners:
                errors.append(f"test product surface {identifier} omitted {owner_field}")
                continue
            unknown_scripts = sorted(
                owner for owner in owners if owner not in package_scripts
            )
            if unknown_scripts:
                errors.append(
                    f"test product surface {identifier} {owner_field} are not package scripts: "
                    f"{unknown_scripts!r}"
                )
            if len(owners) != len(set(owners)):
                errors.append(
                    f"test product surface {identifier} {owner_field} must be unique"
                )
            forbidden_policy_owners = sorted(set(owners) & policy_only_set)
            if forbidden_policy_owners:
                errors.append(
                    f"test product surface {identifier} {owner_field} contains policy-only owners: "
                    f"{forbidden_policy_owners!r}"
                )
        forbidden_system_policy_owners = sorted(
            set(surface.get("systemOwners", [])) & policy_only_set
        )
        if forbidden_system_policy_owners:
            errors.append(
                f"test product surface {identifier} systemOwners contains policy-only owners: "
                f"{forbidden_system_policy_owners!r}"
            )
        evidence = surface.get("evidence")
        if not isinstance(evidence, list) or not evidence:
            errors.append(f"test product surface {identifier} omitted evidence")
        else:
            for evidence_index, entry in enumerate(evidence):
                if not isinstance(entry, dict):
                    errors.append(
                        f"test product surface {identifier} evidence[{evidence_index}] "
                        "must be an object"
                    )
                    continue
                if set(entry) != SCORECARD_EVIDENCE_FIELDS:
                    errors.append(
                        f"test product surface {identifier} evidence[{evidence_index}] "
                        "fields must match the schema exactly"
                    )
                evidence_path = safe_path(
                    entry.get("path"),
                    f"test product surface {identifier} evidence[{evidence_index}].path",
                    errors,
                )
                if evidence_path is not None and not evidence_path.is_file():
                    errors.append(
                        f"test product surface {identifier} evidence is missing: "
                        f"{evidence_path.relative_to(ROOT)}"
                    )
                for field in ("proves", "doesNotProve"):
                    if not isinstance(entry.get(field), str) or not entry[field]:
                        errors.append(
                            f"test product surface {identifier} evidence[{evidence_index}].{field} "
                            "must be non-empty"
                        )
        selector = surface.get("selectorOwnership")
        if not isinstance(selector, dict):
            errors.append(f"test product surface {identifier} omitted selectorOwnership")
        else:
            if set(selector) != SCORECARD_SELECTOR_FIELDS:
                errors.append(
                    f"test product surface {identifier} selector fields must match the schema exactly"
                )
            semantic_owners = selector.get("semanticOwners")
            if (
                not isinstance(semantic_owners, list)
                or not semantic_owners
                or not all(isinstance(owner, str) and owner for owner in semantic_owners)
                or len(semantic_owners) != len(set(semantic_owners))
            ):
                errors.append(
                    f"test product surface {identifier} semantic owners must be unique non-empty strings"
                )
            if selector.get("affectedRing") != "R3" or selector.get("unknownFallback") != "R4":
                errors.append(
                    f"test product surface {identifier} must use R3 affected selection and R4 fallback"
                )
        observed = surface.get("lastObserved")
        if not isinstance(observed, dict):
            errors.append(f"test product surface {identifier} omitted lastObserved")
        else:
            if set(observed) != SCORECARD_OBSERVED_FIELDS:
                errors.append(
                    f"test product surface {identifier} lastObserved fields must match the schema exactly"
                )
            if observed.get("state") not in {
                "bounded-green",
                "known-red",
                "not-qualified",
            }:
                errors.append(f"test product surface {identifier} has an invalid observed state")
            if not isinstance(observed.get("evidence"), str) or not observed["evidence"]:
                errors.append(
                    f"test product surface {identifier} lastObserved.evidence must be non-empty"
                )
        if surface.get("fullBackstopCommand") != FULL_BACKSTOP_COMMAND:
            errors.append(
                f"test product surface {identifier} must use the canonical full backstop command"
            )
        release_command = surface.get("releaseCommand")
        if release_command is not None:
            if not isinstance(release_command, str):
                errors.append(
                    f"test product surface {identifier} releaseCommand must be a string or null"
                )
            else:
                match = re.fullmatch(r"npm run ([a-z0-9:-]+)(?: -- .+)?", release_command)
                if match is None or match.group(1) not in package_scripts:
                    errors.append(
                        f"test product surface {identifier} releaseCommand must name a package script"
                    )

    if seen != EXPECTED_PRODUCT_SURFACES:
        errors.append(
            "test product surfaces must be exactly "
            f"{sorted(EXPECTED_PRODUCT_SURFACES)!r}; got {sorted(seen)!r}"
        )


def expected_output_files() -> set[Path]:
    files: set[Path] = set()
    for tree_name in ("test", "examples"):
        tree_root = ROOT / tree_name
        if not tree_root.is_dir():
            continue
        for path in tree_root.rglob("*"):
            if not path.is_file():
                continue
            relative = path.relative_to(tree_root)
            parents = relative.parts[:-1]
            if (
                any(
                    part == "expected" or part.startswith("expected_")
                    for part in parents
                )
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
    surface_scorecards = load_json(SURFACE_SCORECARDS, errors)
    surface_scorecards_schema = load_json(SURFACE_SCORECARDS_SCHEMA, errors)
    capability_manifest = load_json(CAPABILITY_MANIFEST, errors)
    beads_plan = load_json(BEADS_PLAN, errors)
    package = load_json(PACKAGE, errors)

    if catalog.get("schemaVersion") != 1:
        errors.append("fixture taxonomy must use schemaVersion 1")
    if catalog_schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        errors.append("fixture taxonomy schema must use JSON Schema 2020-12")
    if case_schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        errors.append("fixture case schema must use JSON Schema 2020-12")
    if surface_scorecards_schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema":
        errors.append("test surface scorecard schema must use JSON Schema 2020-12")
    case_schema_path = safe_path(catalog.get("caseSchema"), "caseSchema", errors)
    if case_schema_path is not None and case_schema_path != CASE_SCHEMA:
        errors.append("fixture taxonomy must point to the canonical fixture-case schema")
    scorecards_path = safe_path(
        catalog.get("surfaceScorecards"), "surfaceScorecards", errors
    )
    if scorecards_path is not None and scorecards_path != SURFACE_SCORECARDS:
        errors.append("fixture taxonomy must point to the canonical test surface scorecards")

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
        snapshot_runner = entry.get("snapshotValidationRunner")
        if snapshot_runner is not None:
            if (
                not isinstance(snapshot_runner, list)
                or not snapshot_runner
                or not all(isinstance(part, str) and part for part in snapshot_runner)
            ):
                errors.append(
                    f"fixture suite {identifier} snapshotValidationRunner must be an argument array"
                )
            elif len(snapshot_runner) >= 2 and snapshot_runner[0].startswith("python"):
                snapshot_runner_path = safe_path(
                    snapshot_runner[1],
                    f"suite {identifier} snapshotValidationRunner",
                    errors,
                )
                if (
                    snapshot_runner_path is not None
                    and not snapshot_runner_path.is_file()
                ):
                    errors.append(
                        "fixture suite snapshot-validation runner is missing: "
                        f"{snapshot_runner_path.relative_to(ROOT)}"
                    )
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
        if policy.get("integratedCheckCommand") != "npm run snapshots:catalog":
            errors.append("snapshot integrated check command drifted")
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
        try:
            snapshot_module.validate_catalog_contract(catalog)
        except Exception as error:
            errors.append(f"snapshot integrated ownership contract failed: {error}")

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
    validate_surface_scorecards(
        surface_scorecards,
        capability_manifest,
        scripts,
        {
            entry.get("key")
            for section in (beads_plan.get("epics"), beads_plan.get("tasks"))
            if isinstance(section, list)
            for entry in section
            if isinstance(entry, dict) and isinstance(entry.get("key"), str)
        },
        errors,
    )
    expected_scripts = {
        "test:fixture-policy": "python3 scripts/ci/check_fixture_policy.py",
        "snapshots:check": "python3 scripts/test/snapshots.py --check",
        "snapshots:catalog": "python3 scripts/test/snapshots.py --catalog-check",
        "snapshots:update": "python3 scripts/test/snapshots.py --update",
    }
    for name, command in expected_scripts.items():
        if scripts.get(name) != command:
            errors.append(f"package.json {name} must equal {command!r}")
    toolchain = str(scripts.get("test:toolchain", ""))
    if "npm run snapshots:catalog" not in toolchain:
        errors.append("package.json test:toolchain must validate integrated snapshot ownership")
    if "npm run snapshots:check" in toolchain:
        errors.append("package.json test:toolchain must not duplicate cold snapshot renders")
    if "npm run test:fixture-policy" not in str(scripts.get("test:governance", "")):
        errors.append("package.json test:governance must validate fixture policy")

    pre_commit = read_text(PRE_COMMIT, errors)
    if "scripts/ci/check_fixture_policy.py" not in pre_commit:
        errors.append("pre-commit must validate fixture policy for relevant changes")
    workflow = read_text(WORKFLOW, errors)
    if "snapshots:update" in workflow:
        errors.append("CI workflow must never invoke snapshot update mode")
    if (
        "npm run test:toolchain:shard" not in workflow
        or "  pinned-toolchain:\n" not in workflow
    ):
        errors.append(
            "CI workflow must retain the isolated toolchain shards and "
            "fail-closed pinned-toolchain aggregate"
        )
    snapshot_workflow = read_text(SNAPSHOT_WORKFLOW, errors)
    if "npm run snapshots:check" not in snapshot_workflow:
        errors.append("scheduled snapshot workflow must retain the independent cold render")
    if "snapshots:update" in snapshot_workflow:
        errors.append("scheduled snapshot workflow must never update snapshots")
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
        "fixture-policy: OK: 8 lanes, 5 product-surface scorecards, canonical case manifests, "
        "mapped runners/expected outputs, explicit snapshot updates, and tiered example assertions"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
"""Validate the M0 product/infrastructure inventory and its human status view."""

from __future__ import annotations

import argparse
import json
import re
import sys
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[2]
MANIFEST_PATH = "docs/specs/bootstrap-inventory.json"
SCHEMA_PATH = "docs/specs/bootstrap-inventory.schema.json"
README_PATH = "README.md"
PLAN_PATH = "docs/specs/beads-plan.json"
BEGIN_MARKER = "<!-- BEGIN GENERATED BOOTSTRAP CAPABILITIES -->"
END_MARKER = "<!-- END GENERATED BOOTSTRAP CAPABILITIES -->"
STATUS_ORDER = ("implemented", "scaffold-only", "experimental", "unsupported")
STATUS_DISPOSITIONS = {
    "implemented": frozenset(("available",)),
    "scaffold-only": frozenset(("reserved",)),
    "experimental": frozenset(("opt-in",)),
    "unsupported": frozenset(("fails-closed", "not-exposed", "not-validated")),
}
CAPABILITY_AREAS = frozenset(
    (
        "cli",
        "compiler",
        "configuration",
        "diagnostics",
        "governance",
        "interop",
        "platform",
        "product",
        "quality",
        "release",
        "runtime",
        "stdlib",
        "target-api",
        "toolchain",
    )
)
LEDGER_AUTHORITIES = frozenset(
    (
        "implemented-contract",
        "test-policy",
        "generated-planning-contract",
        "scaffold-only-planning-seed",
    )
)
REQUIRED_REQUIREMENTS = ("HXC-SCAF-001", "HXC-SCAF-004", "HXC-DOC-007")
REQUIRED_CLAIM_GATES = (
    "full-standard-library",
    "general-haxe-to-c",
    "performance-validated",
    "production-ready",
    "public-abi-stability",
    "supported-platforms",
    "supported-release",
)
REQUIRED_CAPABILITIES = frozenset(
    (
        "bootstrap-capability-manifest",
        "build-adapter-seeds",
        "closed-anonymous-record-lowering",
        "general-haxe-lowering",
        "hxc-cli",
        "hxc-doctor",
        "performance-evidence",
        "platform-support-matrix",
        "primitive-executable-lowering",
        "release-artifacts",
        "runtime-feature-planning",
        "standard-library",
        "target-activation",
    )
)
FORBIDDEN_README_CLAIMS = (
    "is production-ready",
    "is production ready",
    "fully supported standard library",
    "full standard-library support is available",
    "complete haxe-to-c compiler",
    "faster than hxcpp",
    "performance has been validated",
    "supported release is available",
)
IDENTIFIER = re.compile(r"^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$")
OWNER_BEAD = re.compile(r"^E[0-9]+\.T[0-9]{2}$")


class ContractError(ValueError):
    """A deterministic manifest shape error."""


@dataclass(frozen=True)
class ClaimGate:
    id: str
    claimed: bool
    owners: tuple[str, ...]
    reason: str


@dataclass(frozen=True)
class DoctorContract:
    command: str
    status: str
    implemented: bool
    owner_beads: str
    manifest_consumer_required: bool
    remediation: str


@dataclass(frozen=True)
class Ledger:
    id: str
    path: str
    authority: str


@dataclass(frozen=True)
class Capability:
    id: str
    area: str
    status: str
    disposition: str
    summary: str
    scope: str
    evidence: tuple[str, ...]
    owners: tuple[str, ...]
    limitations: tuple[str, ...]


@dataclass(frozen=True)
class Manifest:
    schema: str
    schema_version: int
    manifest_id: str
    milestone: str
    project_status: str
    requirements: tuple[str, ...]
    status_definitions: tuple[tuple[str, str], ...]
    compatibility: tuple[tuple[str, str], ...]
    claim_gates: tuple[ClaimGate, ...]
    doctor: DoctorContract
    ledgers: tuple[Ledger, ...]
    capabilities: tuple[Capability, ...]


def object_value(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise ContractError(f"{label} must be an object")
    result: dict[str, object] = {}
    for key, item in value.items():
        if not isinstance(key, str):
            raise ContractError(f"{label} contains a non-string key")
        result[key] = item
    return result


def array_value(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise ContractError(f"{label} must be an array")
    result: list[object] = []
    for item in value:
        result.append(item)
    return result


def exact_keys(value: dict[str, object], expected: set[str], label: str) -> None:
    actual = set(value)
    missing = sorted(expected - actual)
    extra = sorted(actual - expected)
    if missing:
        raise ContractError(f"{label} is missing fields: {', '.join(missing)}")
    if extra:
        raise ContractError(f"{label} has unknown fields: {', '.join(extra)}")


def string_value(value: object, label: str) -> str:
    if not isinstance(value, str) or not value:
        raise ContractError(f"{label} must be a non-empty string")
    return value


def boolean_value(value: object, label: str) -> bool:
    if type(value) is not bool:
        raise ContractError(f"{label} must be boolean")
    return value


def integer_value(value: object, label: str) -> int:
    if type(value) is not int:
        raise ContractError(f"{label} must be an integer")
    return value


def string_array(value: object, label: str) -> tuple[str, ...]:
    result: list[str] = []
    for index, item in enumerate(array_value(value, label)):
        result.append(string_value(item, f"{label}[{index}]"))
    return tuple(result)


def read_json(path: Path, label: str) -> object:
    try:
        text = path.read_text(encoding="utf-8")
        value: object = json.loads(text)
        return value
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise ContractError(f"cannot read {label}: {error}") from error


def parse_claim_gate(value: object, index: int) -> ClaimGate:
    label = f"claimGates[{index}]"
    item = object_value(value, label)
    exact_keys(item, {"id", "claimed", "owners", "reason"}, label)
    return ClaimGate(
        id=string_value(item["id"], f"{label}.id"),
        claimed=boolean_value(item["claimed"], f"{label}.claimed"),
        owners=string_array(item["owners"], f"{label}.owners"),
        reason=string_value(item["reason"], f"{label}.reason"),
    )


def parse_doctor(value: object) -> DoctorContract:
    label = "doctor"
    item = object_value(value, label)
    exact_keys(
        item,
        {
            "command",
            "status",
            "implemented",
            "ownerBeads",
            "manifestConsumerRequired",
            "remediation",
        },
        label,
    )
    return DoctorContract(
        command=string_value(item["command"], "doctor.command"),
        status=string_value(item["status"], "doctor.status"),
        implemented=boolean_value(item["implemented"], "doctor.implemented"),
        owner_beads=string_value(item["ownerBeads"], "doctor.ownerBeads"),
        manifest_consumer_required=boolean_value(
            item["manifestConsumerRequired"], "doctor.manifestConsumerRequired"
        ),
        remediation=string_value(item["remediation"], "doctor.remediation"),
    )


def parse_ledger(value: object, index: int) -> Ledger:
    label = f"ledgers[{index}]"
    item = object_value(value, label)
    exact_keys(item, {"id", "path", "authority"}, label)
    return Ledger(
        id=string_value(item["id"], f"{label}.id"),
        path=string_value(item["path"], f"{label}.path"),
        authority=string_value(item["authority"], f"{label}.authority"),
    )


def parse_capability(value: object, index: int) -> Capability:
    label = f"capabilities[{index}]"
    item = object_value(value, label)
    exact_keys(
        item,
        {
            "id",
            "area",
            "status",
            "disposition",
            "summary",
            "scope",
            "evidence",
            "owners",
            "limitations",
        },
        label,
    )
    return Capability(
        id=string_value(item["id"], f"{label}.id"),
        area=string_value(item["area"], f"{label}.area"),
        status=string_value(item["status"], f"{label}.status"),
        disposition=string_value(item["disposition"], f"{label}.disposition"),
        summary=string_value(item["summary"], f"{label}.summary"),
        scope=string_value(item["scope"], f"{label}.scope"),
        evidence=string_array(item["evidence"], f"{label}.evidence"),
        owners=string_array(item["owners"], f"{label}.owners"),
        limitations=string_array(item["limitations"], f"{label}.limitations"),
    )


def parse_string_map(
    value: object, label: str, required_keys: tuple[str, ...]
) -> tuple[tuple[str, str], ...]:
    item = object_value(value, label)
    exact_keys(item, set(required_keys), label)
    return tuple(
        (key, string_value(item[key], f"{label}.{key}")) for key in required_keys
    )


def load_manifest(root: Path) -> Manifest:
    raw = object_value(read_json(root / MANIFEST_PATH, MANIFEST_PATH), "manifest")
    exact_keys(
        raw,
        {
            "$schema",
            "schemaVersion",
            "manifestId",
            "milestone",
            "projectStatus",
            "requirements",
            "statusDefinitions",
            "compatibility",
            "claimGates",
            "doctor",
            "ledgers",
            "capabilities",
        },
        "manifest",
    )
    claim_gates = tuple(
        parse_claim_gate(item, index)
        for index, item in enumerate(array_value(raw["claimGates"], "claimGates"))
    )
    ledgers = tuple(
        parse_ledger(item, index)
        for index, item in enumerate(array_value(raw["ledgers"], "ledgers"))
    )
    capabilities = tuple(
        parse_capability(item, index)
        for index, item in enumerate(
            array_value(raw["capabilities"], "capabilities")
        )
    )
    return Manifest(
        schema=string_value(raw["$schema"], "$schema"),
        schema_version=integer_value(raw["schemaVersion"], "schemaVersion"),
        manifest_id=string_value(raw["manifestId"], "manifestId"),
        milestone=string_value(raw["milestone"], "milestone"),
        project_status=string_value(raw["projectStatus"], "projectStatus"),
        requirements=string_array(raw["requirements"], "requirements"),
        status_definitions=parse_string_map(
            raw["statusDefinitions"], "statusDefinitions", STATUS_ORDER
        ),
        compatibility=parse_string_map(
            raw["compatibility"],
            "compatibility",
            ("profiles", "runtimePolicy", "environments", "generatedC", "publicAbi"),
        ),
        claim_gates=claim_gates,
        doctor=parse_doctor(raw["doctor"]),
        ledgers=ledgers,
        capabilities=capabilities,
    )


def safe_repository_path(value: str) -> bool:
    if "\\" in value:
        return False
    path = PurePosixPath(value)
    return (
        bool(path.parts)
        and not path.is_absolute()
        and path.as_posix() == value
        and all(part not in ("", ".", "..") for part in path.parts)
    )


def validate_file(root: Path, value: str, label: str, errors: list[str]) -> None:
    if not safe_repository_path(value):
        errors.append(f"{label} is not a normalized repository-relative path: {value}")
        return
    path = root.joinpath(*PurePosixPath(value).parts)
    if path.is_symlink() or not path.is_file():
        errors.append(f"{label} does not name a real repository file: {value}")


def load_plan_keys(root: Path, errors: list[str]) -> set[str]:
    try:
        plan = object_value(read_json(root / PLAN_PATH, PLAN_PATH), "beads plan")
        tasks = array_value(plan.get("tasks"), "beads plan tasks")
    except ContractError as error:
        errors.append(str(error))
        return set()
    result: set[str] = set()
    for index, raw_task in enumerate(tasks):
        try:
            task = object_value(raw_task, f"beads plan tasks[{index}]")
            result.add(string_value(task.get("key"), f"beads plan tasks[{index}].key"))
        except ContractError as error:
            errors.append(str(error))
    return result


def validate_owner(
    value: str, label: str, plan_keys: set[str], errors: list[str]
) -> None:
    if OWNER_BEAD.fullmatch(value) is None:
        errors.append(f"{label} is not a stable E<n>.T<nn> key: {value}")
    elif value not in plan_keys:
        errors.append(f"{label} is absent from {PLAN_PATH}: {value}")


def validate_schema_contract(schema: dict[str, object], errors: list[str]) -> None:
    try:
        if boolean_value(
            schema.get("additionalProperties"), "schema.additionalProperties"
        ):
            errors.append(f"{SCHEMA_PATH} must reject unknown manifest fields")
        properties = object_value(schema.get("properties"), "schema.properties")
        capabilities = object_value(
            properties.get("capabilities"), "schema.properties.capabilities"
        )
        if integer_value(
            capabilities.get("minItems"), "schema.properties.capabilities.minItems"
        ) != 30:
            errors.append(f"{SCHEMA_PATH} capabilities minItems must remain 30")

        definitions = object_value(schema.get("$defs"), "schema.$defs")
        capability = object_value(
            definitions.get("capability"), "schema.$defs.capability"
        )
        capability_properties = object_value(
            capability.get("properties"), "schema.$defs.capability.properties"
        )
        status = object_value(
            capability_properties.get("status"),
            "schema.$defs.capability.properties.status",
        )
        schema_statuses = string_array(
            status.get("enum"), "schema.$defs.capability.properties.status.enum"
        )
        if schema_statuses != STATUS_ORDER:
            errors.append(f"{SCHEMA_PATH} status enum drifted from the checker")

        area = object_value(
            capability_properties.get("area"),
            "schema.$defs.capability.properties.area",
        )
        schema_areas = string_array(
            area.get("enum"), "schema.$defs.capability.properties.area.enum"
        )
        if len(set(schema_areas)) != len(schema_areas) or set(schema_areas) != set(
            CAPABILITY_AREAS
        ):
            errors.append(f"{SCHEMA_PATH} area enum drifted from the checker")

        disposition = object_value(
            capability_properties.get("disposition"),
            "schema.$defs.capability.properties.disposition",
        )
        schema_dispositions = string_array(
            disposition.get("enum"),
            "schema.$defs.capability.properties.disposition.enum",
        )
        expected_dispositions: set[str] = set()
        for allowed_dispositions in STATUS_DISPOSITIONS.values():
            expected_dispositions.update(allowed_dispositions)
        if len(set(schema_dispositions)) != len(schema_dispositions) or set(
            schema_dispositions
        ) != expected_dispositions:
            errors.append(f"{SCHEMA_PATH} disposition enum drifted from the checker")

        ledger = object_value(definitions.get("ledger"), "schema.$defs.ledger")
        ledger_properties = object_value(
            ledger.get("properties"), "schema.$defs.ledger.properties"
        )
        authority = object_value(
            ledger_properties.get("authority"),
            "schema.$defs.ledger.properties.authority",
        )
        schema_authorities = string_array(
            authority.get("enum"),
            "schema.$defs.ledger.properties.authority.enum",
        )
        if len(set(schema_authorities)) != len(schema_authorities) or set(
            schema_authorities
        ) != set(LEDGER_AUTHORITIES):
            errors.append(f"{SCHEMA_PATH} authority enum drifted from the checker")
    except ContractError as error:
        errors.append(str(error))


def render_readme(manifest: Manifest) -> str:
    definitions = dict(manifest.status_definitions)
    counts = {
        status: sum(1 for capability in manifest.capabilities if capability.status == status)
        for status in STATUS_ORDER
    }
    lines = [
        BEGIN_MARKER,
        (
            "> Generated from "
            "[`docs/specs/bootstrap-inventory.json`](docs/specs/bootstrap-inventory.json) "
            "by `scripts/ci/check_capability_manifest.py`; edit the manifest, not this block."
        ),
        "",
        f"**Project stage:** `{manifest.project_status}` (`{manifest.milestone}`).",
        "",
        (
            "This inventory combines product capability boundaries with repository "
            "infrastructure and evidence machinery. Governance, quality, and toolchain "
            "rows are not Haxe-to-C language or runtime features."
        ),
        "",
        "| Status | Count | Meaning |",
        "| --- | ---: | --- |",
    ]
    for status in STATUS_ORDER:
        lines.append(f"| `{status}` | {counts[status]} | {definitions[status]} |")
    lines.extend(
        (
            "",
            "| Inventory item | Status | Current boundary |",
            "| --- | --- | --- |",
        )
    )
    for capability in manifest.capabilities:
        lines.append(
            f"| `{capability.id}` | `{capability.status}` | {capability.summary} |"
        )
    unclaimed = ", ".join(f"`{gate.id}`" for gate in manifest.claim_gates)
    lines.extend(
        (
            "",
            f"**Hard claim gates not passed:** {unclaimed}.",
            "",
            (
                f"**`{manifest.doctor.command}`:** `{manifest.doctor.status}`; "
                f"implementation is absent and owned by `{manifest.doctor.owner_beads}`. "
                f"{manifest.doctor.remediation}"
            ),
            END_MARKER,
        )
    )
    return "\n".join(lines)


def find_generated_block(text: str, errors: list[str]) -> str:
    begin_count = text.count(BEGIN_MARKER)
    end_count = text.count(END_MARKER)
    if begin_count != 1 or end_count != 1:
        errors.append(
            f"{README_PATH} must contain exactly one generated capability block"
        )
        return ""
    begin = text.index(BEGIN_MARKER)
    end = text.index(END_MARKER, begin) + len(END_MARKER)
    return text[begin:end]


def without_generated_block(text: str) -> str:
    if text.count(BEGIN_MARKER) != 1 or text.count(END_MARKER) != 1:
        return text
    begin = text.index(BEGIN_MARKER)
    end = text.index(END_MARKER, begin) + len(END_MARKER)
    return text[:begin] + text[end:]


def validate(root: Path, manifest: Manifest) -> list[str]:
    errors: list[str] = []
    validate_file(root, SCHEMA_PATH, "schema", errors)
    try:
        schema = object_value(read_json(root / SCHEMA_PATH, SCHEMA_PATH), "schema")
        if schema.get("$id") != (
            "https://reflaxe-c.dev/schemas/bootstrap-inventory.schema.json"
        ):
            errors.append(f"{SCHEMA_PATH} has the wrong stable $id")
        validate_schema_contract(schema, errors)
    except ContractError as error:
        errors.append(str(error))

    if manifest.schema != "bootstrap-inventory.schema.json":
        errors.append("manifest $schema must name bootstrap-inventory.schema.json")
    if manifest.schema_version != 1:
        errors.append("manifest schemaVersion must be 1")
    if manifest.manifest_id != "hxc-bootstrap-capabilities-v1":
        errors.append("manifestId must be hxc-bootstrap-capabilities-v1")
    if manifest.milestone != "M0":
        errors.append("bootstrap manifest milestone must remain M0")
    if manifest.project_status != "experimental-scaffold-with-bounded-executable-slices":
        errors.append("projectStatus lost the bounded M0 executable-slices claim")
    if manifest.requirements != REQUIRED_REQUIREMENTS:
        errors.append(
            "requirements must be exactly HXC-SCAF-001, HXC-SCAF-004, HXC-DOC-007"
        )

    plan_keys = load_plan_keys(root, errors)
    claim_ids = tuple(gate.id for gate in manifest.claim_gates)
    if claim_ids != REQUIRED_CLAIM_GATES:
        errors.append("claimGates must retain the sorted seven hard no-claim gates")
    for index, gate in enumerate(manifest.claim_gates):
        if IDENTIFIER.fullmatch(gate.id) is None:
            errors.append(f"claimGates[{index}].id is invalid: {gate.id}")
        if gate.claimed:
            errors.append(f"claim gate cannot be claimed at M0: {gate.id}")
        if not gate.owners:
            errors.append(f"claim gate has no owner: {gate.id}")
        for owner_index, owner in enumerate(gate.owners):
            validate_owner(
                owner,
                f"claimGates[{index}].owners[{owner_index}]",
                plan_keys,
                errors,
            )

    doctor = manifest.doctor
    if (
        doctor.command != "hxc doctor"
        or doctor.status != "unsupported"
        or doctor.implemented
        or doctor.owner_beads != "E8.T06"
        or not doctor.manifest_consumer_required
    ):
        errors.append("doctor contract must remain absent/unsupported and owned by E8.T06")
    if (root / "src/Run.hx").exists() and not doctor.implemented:
        errors.append(
            "src/Run.hx exists while the manifest still says hxc doctor is absent"
        )
    validate_owner(doctor.owner_beads, "doctor.ownerBeads", plan_keys, errors)

    ledger_ids = tuple(ledger.id for ledger in manifest.ledgers)
    if ledger_ids != tuple(sorted(ledger_ids)) or len(set(ledger_ids)) != len(ledger_ids):
        errors.append("ledgers must have unique UTF-8-sorted IDs")
    for index, ledger in enumerate(manifest.ledgers):
        if IDENTIFIER.fullmatch(ledger.id) is None:
            errors.append(f"ledgers[{index}].id is invalid: {ledger.id}")
        if ledger.authority not in LEDGER_AUTHORITIES:
            errors.append(
                f"ledgers[{index}].authority is invalid: {ledger.authority}"
            )
        validate_file(root, ledger.path, f"ledgers[{index}].path", errors)
    stdlib_ledgers = [ledger for ledger in manifest.ledgers if ledger.id == "standard-library"]
    if len(stdlib_ledgers) != 1 or stdlib_ledgers[0].authority != (
        "generated-planning-contract"
    ):
        errors.append(
            "standard-library ledger must remain a generated planning contract"
        )

    capability_ids = tuple(capability.id for capability in manifest.capabilities)
    if len(manifest.capabilities) < 30:
        errors.append("manifest must retain at least 30 explicit capability boundaries")
    if capability_ids != tuple(sorted(capability_ids)):
        errors.append("capabilities must be sorted by stable ID")
    if len(set(capability_ids)) != len(capability_ids):
        errors.append("capability IDs must be unique")
    missing_capabilities = sorted(REQUIRED_CAPABILITIES - set(capability_ids))
    if missing_capabilities:
        errors.append(
            "manifest lost required capability boundaries: "
            + ", ".join(missing_capabilities)
        )
    statuses = {capability.status for capability in manifest.capabilities}
    if statuses != set(STATUS_ORDER):
        errors.append("manifest must use all four exact capability statuses")
    unsupported_dispositions = {
        capability.disposition
        for capability in manifest.capabilities
        if capability.status == "unsupported"
    }
    if unsupported_dispositions != STATUS_DISPOSITIONS["unsupported"]:
        errors.append(
            "unsupported capabilities must distinguish fails-closed, not-exposed, "
            "and not-validated"
        )
    for index, capability in enumerate(manifest.capabilities):
        label = f"capabilities[{index}]"
        if IDENTIFIER.fullmatch(capability.id) is None:
            errors.append(f"{label}.id is invalid: {capability.id}")
        if capability.area not in CAPABILITY_AREAS:
            errors.append(f"{label}.area is invalid: {capability.area}")
        allowed_dispositions = STATUS_DISPOSITIONS.get(capability.status)
        if allowed_dispositions is None:
            errors.append(f"{label}.status is invalid: {capability.status}")
        elif capability.disposition not in allowed_dispositions:
            errors.append(
                f"{label} status {capability.status} cannot use disposition "
                f"{capability.disposition}"
            )
        if "|" in capability.summary or "\n" in capability.summary:
            errors.append(f"{label}.summary cannot break the generated Markdown table")
        if len(set(capability.evidence)) != len(capability.evidence):
            errors.append(f"{label}.evidence contains duplicates")
        if not capability.evidence:
            errors.append(f"{label}.evidence must not be empty")
        if capability.status == "implemented" and len(capability.evidence) < 2:
            errors.append(
                "implemented capability needs at least two evidence files: "
                + capability.id
            )
        for evidence_index, evidence in enumerate(capability.evidence):
            validate_file(root, evidence, f"{label}.evidence[{evidence_index}]", errors)
        if len(set(capability.owners)) != len(capability.owners):
            errors.append(f"{label}.owners contains duplicates")
        if not capability.owners:
            errors.append(f"{label}.owners must not be empty")
        for owner_index, owner in enumerate(capability.owners):
            validate_owner(
                owner,
                f"{label}.owners[{owner_index}]",
                plan_keys,
                errors,
            )
        if not capability.limitations:
            errors.append(f"{label}.limitations must not be empty")
        if len(set(capability.limitations)) != len(capability.limitations):
            errors.append(f"{label}.limitations contains duplicates")

    by_id = {capability.id: capability for capability in manifest.capabilities}
    doctor_capability = by_id.get("hxc-doctor")
    if doctor_capability is None or (
        doctor_capability.status != doctor.status
        or doctor.owner_beads not in doctor_capability.owners
    ):
        errors.append("hxc-doctor capability and doctor contract disagree")
    if by_id.get("hxc-cli") is None or by_id["hxc-cli"].status != "unsupported":
        errors.append("hxc-cli must remain explicitly unsupported while Run.hx is absent")

    try:
        readme = (root / README_PATH).read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        errors.append(f"cannot read {README_PATH}: {error}")
        readme = ""
    actual_block = find_generated_block(readme, errors)
    expected_block = render_readme(manifest)
    if actual_block and actual_block != expected_block:
        errors.append(
            f"{README_PATH} capability block drifted; run "
            "python3 scripts/ci/check_capability_manifest.py --write-readme"
        )
    readme_claim_text = without_generated_block(readme).lower()
    for forbidden in FORBIDDEN_README_CLAIMS:
        if forbidden in readme_claim_text:
            errors.append(f"{README_PATH} contains premature claim: {forbidden}")

    try:
        prd = (root / "docs/PRD.md").read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        errors.append(f"cannot read docs/PRD.md: {error}")
        prd = ""
    for required_prd_text in (
        "docs/specs/bootstrap-inventory.json",
        "Implemented | Scaffold-only | Experimental | Unsupported",
        "No CLI source, schema, or template is present",
    ):
        if required_prd_text not in prd:
            errors.append(f"docs/PRD.md lost capability contract: {required_prd_text}")
    if "| Development runner and future CLI (`development-cli`) | `src/Run.hx`" in prd:
        errors.append("docs/PRD.md still claims the absent Run.hx inventory seed")

    try:
        agents = (root / "AGENTS.md").read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        errors.append(f"cannot read AGENTS.md: {error}")
        agents = ""
    for required_agent_text in (
        "docs/specs/bootstrap-inventory.json",
        "npm run test:capabilities",
        "scaffold-only",
        "hxc doctor",
    ):
        if required_agent_text not in agents:
            errors.append(f"AGENTS.md lost capability rule: {required_agent_text}")
    return errors


def update_readme(root: Path, manifest: Manifest) -> None:
    path = root / README_PATH
    try:
        text = path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise ContractError(f"cannot read {README_PATH}: {error}") from error
    block = render_readme(manifest)
    if text.count(BEGIN_MARKER) == 1 and text.count(END_MARKER) == 1:
        begin = text.index(BEGIN_MARKER)
        end = text.index(END_MARKER, begin) + len(END_MARKER)
        updated = text[:begin] + block + text[end:]
    elif BEGIN_MARKER not in text and END_MARKER not in text:
        heading = "## What exists today\n"
        next_heading = "\n## Explore the scaffold"
        if heading not in text or next_heading not in text:
            raise ContractError(
                f"{README_PATH} lacks the expected What exists today section"
            )
        start = text.index(heading) + len(heading)
        end = text.index(next_heading, start)
        updated = text[:start] + "\n" + block + "\n" + text[end:]
    else:
        raise ContractError(f"{README_PATH} contains mismatched generated markers")
    if updated != text:
        path.write_text(updated, encoding="utf-8")


def doctor_contract_json(manifest: Manifest) -> str:
    value = {
        "schemaVersion": manifest.schema_version,
        "command": manifest.doctor.command,
        "status": manifest.doctor.status,
        "implemented": manifest.doctor.implemented,
        "ownerBeads": manifest.doctor.owner_beads,
        "capabilityManifest": MANIFEST_PATH,
        "manifestConsumerRequired": manifest.doctor.manifest_consumer_required,
        "remediation": manifest.doctor.remediation,
    }
    return json.dumps(value, ensure_ascii=False, indent=2) + "\n"


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(
        description="Validate the bootstrap product, infrastructure, and limitation inventory."
    )
    result.add_argument("--root", type=Path, default=ROOT)
    result.add_argument("--quiet", action="store_true")
    actions = result.add_mutually_exclusive_group()
    actions.add_argument("--write-readme", action="store_true")
    actions.add_argument("--print-doctor-contract", action="store_true")
    return result


def main(arguments: list[str] | None = None) -> int:
    options = parser().parse_args(arguments)
    root = options.root.resolve()
    try:
        manifest = load_manifest(root)
        if options.write_readme:
            update_readme(root, manifest)
        errors = validate(root, manifest)
    except ContractError as error:
        errors = [str(error)]
        manifest = None
    if errors:
        for error in errors:
            print(f"capability-manifest: ERROR: {error}", file=sys.stderr)
        return 1
    if manifest is None:
        print("capability-manifest: ERROR: manifest was not loaded", file=sys.stderr)
        return 1
    if options.print_doctor_contract:
        print(doctor_contract_json(manifest), end="")
    elif not options.quiet:
        counts = {
            status: sum(
                1 for capability in manifest.capabilities if capability.status == status
            )
            for status in STATUS_ORDER
        }
        print(
            "capability-manifest: OK: "
            + ", ".join(f"{status}={counts[status]}" for status in STATUS_ORDER)
            + "; README derived; hxc doctor explicitly unsupported"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

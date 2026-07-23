#!/usr/bin/env python3
"""Generate and check the pinned typed Haxe standard-library parity ledger."""

from __future__ import annotations

import copy
import csv
import difflib
import fnmatch
import hashlib
import io
import json
import os
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
LEDGER = ROOT / "docs/specs/stdlib-ledger.json"
CSV_LEDGER = ROOT / "docs/specs/stdlib-ledger.csv"
SCHEMA = ROOT / "docs/specs/stdlib-ledger.schema.json"
TOOLCHAIN_LOCK = ROOT / "docs/specs/toolchain-lock.json"
BEADS_PLAN = ROOT / "docs/specs/beads-plan.json"
DIAGNOSTICS = ROOT / "docs/specs/diagnostics.json"
RUNTIME_FEATURES = ROOT / "runtime/hxrt/features.json"
REPORT_PREFIX = "HXC_STDLIB_SURFACE="
API_ALGORITHM = "hxc-stdlib-api-v1"
ALL_PROFILES = ("portable", "metal")
ALL_ENVIRONMENTS = ("hosted", "freestanding", "wasi", "emscripten")
OWNERS = {
    "direct-compiler-lowering",
    "thin-c-os-wrapper",
    "target-source-override",
    "runtime-slice",
    "platform-adapter",
    "unsupported-target-specific",
}
STATUSES = {"planned", "partial", "conformant"}
POLICY_FIELDS = (
    "owner",
    "runtimeFeatures",
    "profiles",
    "environments",
    "status",
    "ownerBeads",
    "testOwnerBeads",
    "diagnosticPolicies",
    "tests",
    "notes",
)
ENTRY_FIELDS = (
    "id",
    "module",
    "source",
    "sourceSha256",
    "probeScope",
    "type",
    "member",
    "kind",
    "signature",
    "isStatic",
    *POLICY_FIELDS,
    "ruleId",
)
CSV_FIELDS = (
    "id",
    "module",
    "source",
    "type",
    "member",
    "kind",
    "static",
    "signature",
    "owner",
    "runtime_features",
    "profiles",
    "environments",
    "status",
    "owner_beads",
    "test_owner_beads",
    "diagnostic_policies",
    "tests",
    "rule_id",
    "notes",
)


class StdlibLedgerFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class ProbeRender:
    payload: str
    report: dict[str, object]


@dataclass(frozen=True)
class Surface:
    tree: dict[str, object]
    sources: tuple[dict[str, object], ...]
    apis: tuple[dict[str, object], ...]


@dataclass(frozen=True)
class LedgerArtifacts:
    ledger: dict[str, object]
    csv: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def record(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict) or not all(isinstance(key, str) for key in value):
        raise StdlibLedgerFailure(f"{label} must be an object with text keys")
    return value


def records(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise StdlibLedgerFailure(f"{label} must be an array")
    return value


def strings(value: object, label: str) -> list[str]:
    items = records(value, label)
    if not all(isinstance(item, str) and item for item in items):
        raise StdlibLedgerFailure(f"{label} must contain non-empty strings")
    return [item for item in items if isinstance(item, str)]


def exact_keys(value: dict[str, object], expected: set[str], label: str) -> None:
    actual = set(value)
    if actual != expected:
        raise StdlibLedgerFailure(
            f"{label} keys drifted: missing={sorted(expected - actual)!r}, "
            f"extra={sorted(actual - expected)!r}"
        )


def extract_report(stdout: str, label: str) -> ProbeRender:
    payloads = [
        line[len(REPORT_PREFIX) :]
        for line in stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(payloads) != 1:
        raise StdlibLedgerFailure(
            f"{label} emitted {len(payloads)} stdlib reports, expected one\nstdout:\n{stdout}"
        )
    try:
        parsed = json.loads(payloads[0])
    except json.JSONDecodeError as error:
        raise StdlibLedgerFailure(f"{label} emitted malformed JSON: {error}") from error
    return ProbeRender(payloads[0], record(parsed, f"{label} report"))


def run_probe(scope: str, label: str) -> ProbeRender:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    command = [
        development_tool("haxe"),
        "-cp",
        str(CASE.relative_to(ROOT)),
        "-D",
        f"hxc_stdlib_probe_scope={scope}",
        "--macro",
        "StdlibSurfaceProbe.install()",
        "-main",
        "StdlibLedgerFixture",
    ]
    with tempfile.TemporaryDirectory(prefix="hxc-stdlib-ledger-") as directory:
        output = Path(directory) / "out"
        if scope == "c-common":
            command[3:3] = ["-lib", "reflaxe.c", "-D", "reflaxe_c_lifecycle_probe"]
            command.extend(["--custom-target", f"c={output}"])
        elif scope == "capability-gated":
            command[3:3] = ["-D", "hxc_stdlib_capability_declaration_probe"]
            command.extend(["--custom-target", f"stdlibprobe={output}"])
        else:
            raise StdlibLedgerFailure(f"unknown probe scope: {scope}")
        result = subprocess.run(
            command,
            cwd=ROOT,
            env=environment,
            check=False,
            capture_output=True,
            text=True,
            timeout=60,
        )
        if scope == "capability-gated" and output.exists():
            raise StdlibLedgerFailure(
                "declaration-only capability probe unexpectedly emitted an artifact"
            )
    if result.returncode != 0 or result.stderr:
        raise StdlibLedgerFailure(
            f"{label} failed with {result.returncode}\n"
            f"command: {' '.join(command)}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return extract_report(result.stdout, label)


def probe_surface(label: str) -> tuple[Surface, tuple[str, str]]:
    common = run_probe("c-common", f"{label} C common-surface probe")
    capability = run_probe(
        "capability-gated", f"{label} capability declaration probe"
    )
    common_report = common.report
    capability_report = capability.report
    for report_value, scope, target_name in (
        (common_report, "c-common", "c"),
        (capability_report, "capability-gated", "stdlibprobe"),
    ):
        if (
            report_value.get("schemaVersion") != 1
            or report_value.get("scope") != scope
            or report_value.get("targetName") != target_name
        ):
            raise StdlibLedgerFailure(
                f"{label} {scope} probe lost its schema, scope, or carrier identity"
            )
    common_tree = record(common_report.get("tree"), f"{label} common tree")
    capability_tree = record(
        capability_report.get("tree"), f"{label} capability tree"
    )
    if common_tree != capability_tree:
        raise StdlibLedgerFailure(
            f"{label} C and capability declaration probes resolved different pinned std trees"
        )
    if common_report.get("haxeVersion") != capability_report.get("haxeVersion"):
        raise StdlibLedgerFailure(f"{label} probes used different Haxe versions")

    merged_sources: list[dict[str, object]] = []
    merged_apis: list[dict[str, object]] = []
    source_scope: dict[str, str] = {}
    for report_value, scope in (
        (common_report, "c-common"),
        (capability_report, "capability-gated"),
    ):
        for index, raw_source in enumerate(
            records(report_value.get("sources"), f"{label} {scope} sources")
        ):
            source = record(raw_source, f"{label} {scope} sources[{index}]")
            exact_keys(source, {"module", "path", "sha256"}, "probe source")
            path = source.get("path")
            module = source.get("module")
            digest = source.get("sha256")
            if (
                not isinstance(path, str)
                or not isinstance(module, str)
                or not isinstance(digest, str)
                or len(digest) != 64
                or "\\" in path
                or path.startswith("/")
            ):
                raise StdlibLedgerFailure(f"invalid probe source record: {source!r}")
            if path in source_scope:
                raise StdlibLedgerFailure(f"source appeared in both probe scopes: {path}")
            source_scope[path] = scope
            merged_sources.append(source)
        for index, raw_api in enumerate(
            records(report_value.get("apis"), f"{label} {scope} APIs")
        ):
            api = record(raw_api, f"{label} {scope} APIs[{index}]")
            exact_keys(
                api,
                {"module", "source", "type", "member", "kind", "signature", "isStatic"},
                "probe API",
            )
            source_path = api.get("source")
            if not isinstance(source_path, str) or source_scope.get(source_path) != scope:
                raise StdlibLedgerFailure(
                    f"API did not retain source/scope ownership: {api!r}"
                )
            merged_apis.append(api)

    merged_sources.sort(key=lambda item: str(item["path"]).encode("utf-8"))
    merged_apis.sort(key=api_sort_key)
    api_keys = [api_identity(api) for api in merged_apis]
    if len(api_keys) != len(set(api_keys)):
        raise StdlibLedgerFailure("typed stdlib probe emitted duplicate API identities")
    module_by_path = {str(item["path"]): str(item["module"]) for item in merged_sources}
    for api in merged_apis:
        source_path = str(api["source"])
        if module_by_path.get(source_path) != api.get("module"):
            raise StdlibLedgerFailure(f"API module/source mismatch: {api!r}")
        api["probeScope"] = source_scope[source_path]

    return (
        Surface(common_tree, tuple(merged_sources), tuple(merged_apis)),
        (common.payload, capability.payload),
    )


def deterministic_surface() -> Surface:
    first, first_payloads = probe_surface("first")
    second, second_payloads = probe_surface("second")
    if first_payloads != second_payloads or first != second:
        raise StdlibLedgerFailure(
            "two typed stdlib surface renders were not byte-identical"
        )
    return first


def api_sort_key(api: dict[str, object]) -> tuple[bytes, ...]:
    member = api.get("member")
    return tuple(
        value.encode("utf-8")
        for value in (
            str(api.get("module", "")),
            str(api.get("type", "")),
            "" if member is None else str(member),
            str(api.get("kind", "")),
            "1" if api.get("isStatic") is True else "0",
            str(api.get("signature", "")),
        )
    )


def api_identity(api: dict[str, object]) -> str:
    member = api.get("member")
    return "\0".join(
        (
            str(api.get("module", "")),
            str(api.get("type", "")),
            "" if member is None else str(member),
            str(api.get("kind", "")),
            "1" if api.get("isStatic") is True else "0",
            str(api.get("signature", "")),
        )
    )


def stable_api_id(api: dict[str, object]) -> str:
    digest = hashlib.sha256(
        (API_ALGORITHM + "\0" + api_identity(api)).encode("utf-8")
    ).hexdigest()
    return f"{API_ALGORITHM}:{digest}"


def load_json(path: Path, label: str) -> dict[str, object]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise StdlibLedgerFailure(f"cannot read {label}: {error}") from error
    return record(value, label)


def stable_plan_keys() -> set[str]:
    plan = load_json(BEADS_PLAN, "Beads plan")
    result: set[str] = set()
    for collection in ("epics", "tasks"):
        for item in records(plan.get(collection), f"Beads plan {collection}"):
            item_record = record(item, f"Beads plan {collection} entry")
            key = item_record.get("key")
            if isinstance(key, str):
                result.add(key)
    return result


def diagnostic_ids() -> set[str]:
    catalog = load_json(DIAGNOSTICS, "diagnostic catalog")
    result: set[str] = set()
    for item in records(catalog.get("diagnostics"), "diagnostic entries"):
        identifier = record(item, "diagnostic entry").get("id")
        if isinstance(identifier, str):
            result.add(identifier)
    return result


def runtime_feature_ids() -> set[str]:
    catalog = load_json(RUNTIME_FEATURES, "runtime feature catalog")
    result: set[str] = set()
    for collection in ("features", "reservedFeatures"):
        for item in records(catalog.get(collection), f"runtime {collection}"):
            identifier = record(item, f"runtime {collection} entry").get("id")
            if isinstance(identifier, str):
                result.add(identifier)
    return result


def validate_string_list(
    value: object,
    label: str,
    *,
    allowed: set[str] | None = None,
    canonical: tuple[str, ...] | None = None,
) -> list[str]:
    items = strings(value, label) if value != [] else []
    if len(items) != len(set(items)):
        raise StdlibLedgerFailure(f"{label} contains duplicates")
    if allowed is not None and not set(items).issubset(allowed):
        raise StdlibLedgerFailure(
            f"{label} contains unknown values: {sorted(set(items) - allowed)!r}"
        )
    if canonical is not None:
        expected = [entry for entry in canonical if entry in items]
        if items != expected:
            raise StdlibLedgerFailure(f"{label} is not in canonical order")
    return items


def validate_policy_fields(
    value: dict[str, object],
    label: str,
    *,
    plans: set[str],
    features: set[str],
    policies: set[str],
) -> None:
    owner = value.get("owner")
    status = value.get("status")
    if owner not in OWNERS:
        raise StdlibLedgerFailure(f"{label}.owner is invalid: {owner!r}")
    if status not in STATUSES:
        raise StdlibLedgerFailure(f"{label}.status is invalid: {status!r}")
    for field in ("ownerBeads", "testOwnerBeads"):
        if value.get(field) not in plans:
            raise StdlibLedgerFailure(
                f"{label}.{field} does not link a reproducible Beads issue: {value.get(field)!r}"
            )
    validate_string_list(
        value.get("runtimeFeatures"),
        f"{label}.runtimeFeatures",
        allowed=features,
    )
    validate_string_list(
        value.get("profiles"),
        f"{label}.profiles",
        allowed=set(ALL_PROFILES),
        canonical=ALL_PROFILES,
    )
    validate_string_list(
        value.get("environments"),
        f"{label}.environments",
        allowed=set(ALL_ENVIRONMENTS),
        canonical=ALL_ENVIRONMENTS,
    )
    row_policies = validate_string_list(
        value.get("diagnosticPolicies"),
        f"{label}.diagnosticPolicies",
        allowed=policies,
    )
    tests = validate_string_list(value.get("tests"), f"{label}.tests")
    for test_path in tests:
        candidate = ROOT / test_path
        if (
            "\\" in test_path
            or test_path.startswith("/")
            or ".." in Path(test_path).parts
            or not candidate.is_file()
        ):
            raise StdlibLedgerFailure(f"{label} references invalid test path {test_path!r}")
    notes = value.get("notes")
    if not isinstance(notes, str) or not notes.strip():
        raise StdlibLedgerFailure(f"{label}.notes must explain the practical policy")
    if status in {"planned", "partial"} and "planned-unsupported" not in row_policies:
        raise StdlibLedgerFailure(
            f"{label} is {status} without the source-positioned unsupported diagnostic policy"
        )
    if status in {"partial", "conformant"} and not tests:
        raise StdlibLedgerFailure(f"{label} is {status} without executable tests")


def validate_schema_document() -> None:
    schema = load_json(SCHEMA, "stdlib ledger schema")
    if (
        schema.get("$schema") != "https://json-schema.org/draft/2020-12/schema"
        or schema.get("$id")
        != "https://reflaxe-c.dev/schemas/stdlib-ledger.schema.json"
    ):
        raise StdlibLedgerFailure("stdlib ledger schema lost its stable 2020-12 identity")
    definitions = record(schema.get("$defs"), "stdlib ledger schema definitions")
    for required in (
        "source",
        "applicability",
        "loweringPolicy",
        "diagnosticPolicy",
        "ownershipRule",
        "apiOverride",
        "entry",
    ):
        if required not in definitions:
            raise StdlibLedgerFailure(f"stdlib ledger schema omitted {required}")


def validate_policy(ledger: dict[str, object]) -> tuple[
    list[dict[str, object]], list[dict[str, object]], dict[str, dict[str, object]]
]:
    expected_top_keys = {
        "$schema",
        "schemaVersion",
        "algorithm",
        "status",
        "sourceOfTruth",
        "requirements",
        "source",
        "applicability",
        "loweringPolicy",
        "diagnosticPolicies",
        "ownershipRules",
        "apiOverrides",
        "entries",
    }
    exact_keys(ledger, expected_top_keys, "stdlib ledger")
    if (
        ledger.get("$schema") != "stdlib-ledger.schema.json"
        or ledger.get("schemaVersion") != 2
        or ledger.get("algorithm") != "hxc-stdlib-ledger-v2"
        or ledger.get("status") != "generated-planning-contract"
        or ledger.get("sourceOfTruth") is not True
        or ledger.get("requirements")
        != ["HXC-PROD-006", "HXC-SEM-025", "HXC-STD-001"]
    ):
        raise StdlibLedgerFailure("stdlib ledger identity or requirement ownership drifted")

    toolchain = load_json(TOOLCHAIN_LOCK, "toolchain lock")
    haxe_lock = record(toolchain.get("haxe"), "toolchain Haxe lock")
    source = record(ledger.get("source"), "stdlib ledger source")
    if (
        source.get("haxeVersion") != haxe_lock.get("version")
        or source.get("sourceRevision") != haxe_lock.get("sourceRevision")
    ):
        raise StdlibLedgerFailure(
            "stdlib ledger does not name the exact Haxe version and source revision from the toolchain lock"
        )

    applicability = record(ledger.get("applicability"), "stdlib applicability")
    expected_applicability = {
        "publicSurface": "non-private typed declarations and fields visible to the pinned compiler; @:noCompletion declarations are excluded",
        "probeScopes": [
            {
                "id": "c-common",
                "carrier": "custom-target-c",
                "sourcePatterns": ["*.hx", "haxe/**/*.hx", "sys/**/*.hx"],
                "reason": "The real CustomTarget(c) carrier is authoritative for common and hosted C typing.",
            },
            {
                "id": "capability-gated",
                "carrier": "declaration-only-custom-target",
                "sourcePatterns": ["haxe/atomic/**/*.hx", "sys/thread/**/*.hx"],
                "reason": "A test-only custom target exposes only the pinned upstream declarations for modules that correctly reject C until target.threaded or target.atomics has adapter evidence; it has no generator and is not a production carrier or behavior oracle for these rows.",
            },
        ],
        "exclusions": [
            {
                "patterns": [
                    "cpp/**",
                    "eval/**",
                    "flash/**",
                    "hl/**",
                    "java/**",
                    "js/**",
                    "jvm/**",
                    "lua/**",
                    "neko/**",
                    "php/**",
                    "python/**",
                ],
                "reason": "These are implementations owned by other Haxe targets, not portable API declarations for C.",
            },
            {
                "patterns": ["haxe/display/**", "haxe/hxb/**", "haxe/macro/**"],
                "reason": "These APIs execute inside the Haxe compiler or language server and do not become generated C standard-library code.",
            },
            {
                "patterns": ["haxe/http/HttpJs.hx", "haxe/http/HttpNodeJs.hx"],
                "reason": "These are JavaScript-specific HTTP implementations; the portable haxe.Http contract and sys adapter remain included.",
            },
        ],
    }
    if applicability != expected_applicability:
        raise StdlibLedgerFailure(
            "stdlib applicability changed without updating the typed probe contract"
        )

    lowering = record(ledger.get("loweringPolicy"), "stdlib lowering policy")
    if lowering.get("order") != [
        "direct-idiomatic-c",
        "program-local-specialization",
        "smallest-dependency-closed-hxrt-slice",
        "source-positioned-diagnostic",
    ]:
        raise StdlibLedgerFailure("stdlib compiler-first lowering order drifted")
    for field in ("compileTimeWork", "runtimeRule", "runtimeFreeRule"):
        if not isinstance(lowering.get(field), str) or not str(lowering[field]).strip():
            raise StdlibLedgerFailure(f"stdlib lowering policy omitted {field}")

    known_diagnostics = diagnostic_ids()
    policy_map: dict[str, dict[str, object]] = {}
    for index, raw_policy in enumerate(
        records(ledger.get("diagnosticPolicies"), "stdlib diagnostic policies")
    ):
        policy = record(raw_policy, f"diagnosticPolicies[{index}]")
        exact_keys(
            policy,
            {"id", "diagnosticIds", "when", "remediation"},
            f"diagnosticPolicies[{index}]",
        )
        identifier = policy.get("id")
        if not isinstance(identifier, str) or identifier in policy_map:
            raise StdlibLedgerFailure(f"invalid duplicate diagnostic policy {identifier!r}")
        validate_string_list(
            policy.get("diagnosticIds"),
            f"diagnosticPolicies[{index}].diagnosticIds",
            allowed=known_diagnostics,
        )
        policy_map[identifier] = policy
    if set(policy_map) != {
        "planned-unsupported",
        "runtime-policy-after-implementation",
    }:
        raise StdlibLedgerFailure("stdlib diagnostic policy set drifted")

    plans = stable_plan_keys()
    features = runtime_feature_ids()
    rules: list[dict[str, object]] = []
    rule_ids: set[str] = set()
    for index, raw_rule in enumerate(
        records(ledger.get("ownershipRules"), "stdlib ownership rules")
    ):
        rule = record(raw_rule, f"ownershipRules[{index}]")
        exact_keys(
            rule,
            {"id", "modulePatterns", *POLICY_FIELDS},
            f"ownershipRules[{index}]",
        )
        identifier = rule.get("id")
        if not isinstance(identifier, str) or identifier in rule_ids:
            raise StdlibLedgerFailure(f"invalid duplicate ownership rule {identifier!r}")
        patterns = validate_string_list(
            rule.get("modulePatterns"), f"ownershipRules[{index}].modulePatterns"
        )
        if patterns != sorted(patterns, key=lambda item: item.encode("utf-8")):
            raise StdlibLedgerFailure(
                f"ownershipRules[{index}].modulePatterns must be UTF-8 sorted"
            )
        validate_policy_fields(
            rule,
            f"ownershipRules[{index}]",
            plans=plans,
            features=features,
            policies=set(policy_map),
        )
        rule_ids.add(identifier)
        rules.append(rule)

    overrides: list[dict[str, object]] = []
    override_ids: set[str] = set()
    for index, raw_override in enumerate(
        records(ledger.get("apiOverrides"), "stdlib API overrides")
    ):
        override = record(raw_override, f"apiOverrides[{index}]")
        exact_keys(
            override,
            {"id", "selector", *POLICY_FIELDS},
            f"apiOverrides[{index}]",
        )
        identifier = override.get("id")
        if (
            not isinstance(identifier, str)
            or identifier in override_ids
            or identifier in rule_ids
        ):
            raise StdlibLedgerFailure(f"invalid duplicate API override {identifier!r}")
        selector = record(override.get("selector"), f"apiOverrides[{index}].selector")
        if not selector or not set(selector).issubset(
            {"module", "type", "member", "kind", "isStatic", "signature"}
        ):
            raise StdlibLedgerFailure(f"apiOverrides[{index}] has an invalid selector")
        validate_policy_fields(
            override,
            f"apiOverrides[{index}]",
            plans=plans,
            features=features,
            policies=set(policy_map),
        )
        override_ids.add(identifier)
        overrides.append(override)
    return rules, overrides, policy_map


def matching_rules(module: str, rules: list[dict[str, object]]) -> list[dict[str, object]]:
    return [
        rule
        for rule in rules
        if any(
            fnmatch.fnmatchcase(module, pattern)
            for pattern in strings(rule.get("modulePatterns"), "module patterns")
        )
    ]


def selector_matches(api: dict[str, object], selector: dict[str, object]) -> bool:
    return all(api.get(field) == expected for field, expected in selector.items())


def build_ledger(policy: dict[str, object], surface: Surface) -> dict[str, object]:
    rules, overrides, _ = validate_policy(policy)
    source_by_path = {str(item["path"]): item for item in surface.sources}
    modules = sorted(
        {str(item["module"]) for item in surface.sources}, key=lambda item: item.encode("utf-8")
    )
    module_rule: dict[str, dict[str, object]] = {}
    for module in modules:
        matches = matching_rules(module, rules)
        if len(matches) != 1:
            raise StdlibLedgerFailure(
                f"applicable stdlib module {module!r} matched {len(matches)} ownership rules; expected exactly one"
            )
        module_rule[module] = matches[0]

    matched_overrides: dict[str, int] = {str(item["id"]): 0 for item in overrides}
    entries: list[dict[str, object]] = []
    for api in surface.apis:
        override_matches = [
            override
            for override in overrides
            if selector_matches(
                api, record(override.get("selector"), "API override selector")
            )
        ]
        if len(override_matches) > 1:
            raise StdlibLedgerFailure(
                f"API {api_identity(api)!r} matched multiple exact overrides"
            )
        owner_record = (
            override_matches[0]
            if override_matches
            else module_rule[str(api.get("module"))]
        )
        rule_id = str(owner_record["id"])
        if override_matches:
            matched_overrides[rule_id] += 1
        source_path = str(api["source"])
        source_record = source_by_path[source_path]
        entry: dict[str, object] = {
            "id": stable_api_id(api),
            "module": api["module"],
            "source": source_path,
            "sourceSha256": source_record["sha256"],
            "probeScope": api["probeScope"],
            "type": api["type"],
            "member": api["member"],
            "kind": api["kind"],
            "signature": api["signature"],
            "isStatic": api["isStatic"],
        }
        for field in POLICY_FIELDS:
            entry[field] = copy.deepcopy(owner_record[field])
        entry["ruleId"] = rule_id
        exact_keys(entry, set(ENTRY_FIELDS), "generated stdlib entry")
        entries.append(entry)

    stale_overrides = [
        identifier for identifier, count in matched_overrides.items() if count != 1
    ]
    if stale_overrides:
        raise StdlibLedgerFailure(
            "each API override must match exactly one pinned typed API: "
            + ", ".join(stale_overrides)
        )
    entries.sort(
        key=lambda item: api_sort_key(
            {
                "module": item["module"],
                "type": item["type"],
                "member": item["member"],
                "kind": item["kind"],
                "isStatic": item["isStatic"],
                "signature": item["signature"],
            }
        )
    )
    identifiers = [str(entry["id"]) for entry in entries]
    if len(identifiers) != len(set(identifiers)):
        raise StdlibLedgerFailure("generated stdlib API IDs collided")

    source_digest_input = "".join(
        f"{item['path']}\0{item['sha256']}\n" for item in surface.sources
    ).encode("utf-8")
    api_digest_input = "".join(
        api_identity(api) + "\n" for api in surface.apis
    ).encode("utf-8")
    toolchain = load_json(TOOLCHAIN_LOCK, "toolchain lock")
    haxe_lock = record(toolchain.get("haxe"), "toolchain Haxe lock")
    tree = surface.tree
    source = {
        "haxeVersion": haxe_lock["version"],
        "sourceRevision": haxe_lock["sourceRevision"],
        "probeSchemaVersion": 1,
        "stdlibSourceCount": tree["sourceCount"],
        "stdlibPathInventorySha256": tree["pathInventorySha256"],
        "topLevelRoots": copy.deepcopy(tree["roots"]),
        "applicableSourceCount": len(surface.sources),
        "applicableSourceSha256": hashlib.sha256(source_digest_input).hexdigest(),
        "apiCount": len(entries),
        "apiSurfaceSha256": hashlib.sha256(api_digest_input).hexdigest(),
    }
    generated = {
        "$schema": policy["$schema"],
        "schemaVersion": policy["schemaVersion"],
        "algorithm": policy["algorithm"],
        "status": policy["status"],
        "sourceOfTruth": policy["sourceOfTruth"],
        "requirements": copy.deepcopy(policy["requirements"]),
        "source": source,
        "applicability": copy.deepcopy(policy["applicability"]),
        "loweringPolicy": copy.deepcopy(policy["loweringPolicy"]),
        "diagnosticPolicies": copy.deepcopy(policy["diagnosticPolicies"]),
        "ownershipRules": copy.deepcopy(policy["ownershipRules"]),
        "apiOverrides": copy.deepcopy(policy["apiOverrides"]),
        "entries": entries,
    }
    validate_generated(generated)
    return generated


def validate_generated(ledger: dict[str, object]) -> None:
    rules, overrides, policy_map = validate_policy(ledger)
    del rules, overrides
    source = record(ledger.get("source"), "generated ledger source")
    entries = [
        record(item, f"entries[{index}]")
        for index, item in enumerate(records(ledger.get("entries"), "ledger entries"))
    ]
    if source.get("apiCount") != len(entries):
        raise StdlibLedgerFailure("generated ledger API count disagrees with entries")
    plans = stable_plan_keys()
    features = runtime_feature_ids()
    identifiers: set[str] = set()
    for index, entry in enumerate(entries):
        exact_keys(entry, set(ENTRY_FIELDS), f"entries[{index}]")
        identifier = entry.get("id")
        if not isinstance(identifier, str) or not identifier.startswith(
            API_ALGORITHM + ":"
        ):
            raise StdlibLedgerFailure(f"entries[{index}] has an invalid stable ID")
        if identifier in identifiers:
            raise StdlibLedgerFailure(f"entries[{index}] duplicates {identifier}")
        identifiers.add(identifier)
        validate_policy_fields(
            entry,
            f"entries[{index}]",
            plans=plans,
            features=features,
            policies=set(policy_map),
        )
        serialized = json.dumps(entry, ensure_ascii=False)
        if "\\" in serialized or str(ROOT) in serialized or "/Users/" in serialized:
            raise StdlibLedgerFailure(f"entries[{index}] leaked a host path")
    statuses = {status: 0 for status in STATUSES}
    for entry in entries:
        statuses[str(entry["status"])] += 1
    if statuses["conformant"] != 1 or statuses["partial"] != 12:
        raise StdlibLedgerFailure(
            "evidence should mark exactly Std.int conformant plus ten bounded Bytes APIs and literal Sys.println/trace partial; broader stdlib parity is not yet proven"
        )


def render_csv(ledger: dict[str, object]) -> str:
    output = io.StringIO(newline="")
    writer = csv.DictWriter(output, fieldnames=CSV_FIELDS, lineterminator="\n")
    writer.writeheader()
    for raw_entry in records(ledger.get("entries"), "ledger entries"):
        entry = record(raw_entry, "ledger CSV entry")
        writer.writerow(
            {
                "id": entry["id"],
                "module": entry["module"],
                "source": entry["source"],
                "type": entry["type"],
                "member": "<type>" if entry["member"] is None else entry["member"],
                "kind": entry["kind"],
                "static": "yes" if entry["isStatic"] else "no",
                "signature": entry["signature"],
                "owner": entry["owner"],
                "runtime_features": "|".join(strings(entry["runtimeFeatures"], "runtimeFeatures"))
                if entry["runtimeFeatures"]
                else "",
                "profiles": "|".join(strings(entry["profiles"], "profiles")),
                "environments": "|".join(strings(entry["environments"], "environments")),
                "status": entry["status"],
                "owner_beads": entry["ownerBeads"],
                "test_owner_beads": entry["testOwnerBeads"],
                "diagnostic_policies": "|".join(
                    strings(entry["diagnosticPolicies"], "diagnosticPolicies")
                )
                if entry["diagnosticPolicies"]
                else "",
                "tests": "|".join(strings(entry["tests"], "tests"))
                if entry["tests"]
                else "",
                "rule_id": entry["ruleId"],
                "notes": entry["notes"],
            }
        )
    return output.getvalue()


def render_artifacts() -> LedgerArtifacts:
    validate_schema_document()
    policy = load_json(LEDGER, "stdlib ledger")
    surface = deterministic_surface()
    generated = build_ledger(policy, surface)
    return LedgerArtifacts(generated, render_csv(generated))


def canonical_json(value: object) -> str:
    return json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True) + "\n"


def semantic_diff(expected: object, actual: object, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            canonical_json(expected).splitlines(keepends=True),
            canonical_json(actual).splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def expect_failure(callback: object, label: str) -> None:
    try:
        if callable(callback):
            callback()
        else:
            raise StdlibLedgerFailure(f"{label} callback is not callable")
    except StdlibLedgerFailure:
        return
    raise StdlibLedgerFailure(f"negative policy probe did not fail: {label}")


def check_negative_policy_guards(
    policy: dict[str, object], surface: Surface
) -> None:
    missing_rule = copy.deepcopy(policy)
    missing_rules = records(missing_rule["ownershipRules"], "negative ownership rules")
    missing_rule["ownershipRules"] = missing_rules[:-1]
    expect_failure(
        lambda: build_ledger(missing_rule, surface),
        "unowned pinned module",
    )

    overlapping_rule = copy.deepcopy(policy)
    overlapping_rules = records(
        overlapping_rule["ownershipRules"], "negative ownership rules"
    )
    first_rule = record(overlapping_rules[0], "negative first ownership rule")
    second_rule = record(overlapping_rules[1], "negative second ownership rule")
    overlapping_module = strings(
        second_rule["modulePatterns"], "negative second rule patterns"
    )[0]
    first_patterns = strings(
        first_rule["modulePatterns"], "negative first rule patterns"
    )
    first_rule["modulePatterns"] = sorted(
        [*first_patterns, overlapping_module], key=lambda item: item.encode("utf-8")
    )
    expect_failure(
        lambda: build_ledger(overlapping_rule, surface),
        "multiply owned pinned module",
    )

    stale_override = copy.deepcopy(policy)
    first_override = record(
        records(stale_override["apiOverrides"], "negative API overrides")[0],
        "negative first API override",
    )
    stale_selector = record(
        first_override["selector"], "negative first API selector"
    )
    stale_selector["signature"] = "(stale : Never) -> Never"
    expect_failure(
        lambda: build_ledger(stale_override, surface),
        "stale exact API override",
    )

    unknown_feature = copy.deepcopy(policy)
    first_rule = record(
        records(unknown_feature["ownershipRules"], "negative ownership rules")[0],
        "negative first ownership rule",
    )
    first_rule["runtimeFeatures"] = ["unregistered-runtime-feature"]
    expect_failure(lambda: validate_policy(unknown_feature), "unknown runtime feature")

    missing_diagnostic = copy.deepcopy(policy)
    first_rule = record(
        records(missing_diagnostic["ownershipRules"], "negative ownership rules")[0],
        "negative first ownership rule",
    )
    first_rule["diagnosticPolicies"] = []
    expect_failure(
        lambda: validate_policy(missing_diagnostic),
        "planned row without diagnostic",
    )

    missing_tests = copy.deepcopy(policy)
    first_override = record(
        records(missing_tests["apiOverrides"], "negative API overrides")[0],
        "negative first API override",
    )
    first_override["tests"] = []
    expect_failure(
        lambda: validate_policy(missing_tests),
        "conformant row without tests",
    )


def check_capability_authority_guard() -> None:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    with tempfile.TemporaryDirectory(prefix="hxc-stdlib-ledger-negative-") as directory:
        output = Path(directory) / "out"
        result = subprocess.run(
            [
                development_tool("haxe"),
                "-cp",
                str(CASE.relative_to(ROOT)),
                "-D",
                "hxc_stdlib_probe_scope=capability-gated",
                "--macro",
                "StdlibSurfaceProbe.install()",
                "-main",
                "StdlibLedgerFixture",
                "--custom-target",
                f"stdlibprobe={output}",
            ],
            cwd=ROOT,
            env=environment,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if (
            result.returncode == 0
            or "declaration-only test authority" not in result.stderr
            or output.exists()
        ):
            raise StdlibLedgerFailure(
                "capability declaration probe did not fail closed without its test authority"
            )


def main() -> int:
    try:
        validate_schema_document()
        policy = load_json(LEDGER, "stdlib ledger")
        surface = deterministic_surface()
        generated = build_ledger(policy, surface)
        check_negative_policy_guards(policy, surface)
        check_capability_authority_guard()
        expected = load_json(LEDGER, "checked-in stdlib ledger")
        if expected != generated:
            diff = semantic_diff(expected, generated, "docs/specs/stdlib-ledger.json")
            raise StdlibLedgerFailure(
                "checked-in stdlib ledger drifted from the pinned typed surface; "
                "run `npm run snapshots:update -- --suite stdlib-ledger` after review\n"
                + diff
            )
        expected_csv = CSV_LEDGER.read_text(encoding="utf-8")
        generated_csv = render_csv(generated)
        if expected_csv != generated_csv:
            diff = "".join(
                difflib.unified_diff(
                    expected_csv.splitlines(keepends=True),
                    generated_csv.splitlines(keepends=True),
                    fromfile="expected/docs/specs/stdlib-ledger.csv",
                    tofile="actual/docs/specs/stdlib-ledger.csv",
                )
            )
            raise StdlibLedgerFailure(
                "review CSV drifted from the JSON authority; run the targeted snapshot update\n"
                + diff
            )
        entries = records(generated["entries"], "generated entries")
        planned = sum(
            1 for item in entries if record(item, "entry").get("status") == "planned"
        )
        partial = sum(
            1 for item in entries if record(item, "entry").get("status") == "partial"
        )
        conformant = sum(
            1
            for item in entries
            if record(item, "entry").get("status") == "conformant"
        )
        print(
            "stdlib-ledger: OK: "
            f"Haxe {record(generated['source'], 'source')['haxeVersion']}; "
            f"sources={len(surface.sources)}, APIs={len(entries)}, "
            f"planned={planned}, partial={partial}, conformant={conformant}; typed C + declaration-only capability probes deterministic"
        )
        return 0
    except (OSError, UnicodeError, StdlibLedgerFailure) as error:
        print(f"stdlib-ledger: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())

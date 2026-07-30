#!/usr/bin/env python3
"""Choose one local pre-commit evidence route and its focused owners.

The hook normally maps each changed file to a small set of focused owners.
Known compiler-wide semantic layers use a deliberately bounded ``affected``
route: one reviewable set of source/semantic/integration sentinels plus focused
owners selected by the staged paths. The complete cold matrix remains a
separate pull-request, nightly, release, and explicit local command.

Unknown cross-cutting compiler or test-infrastructure paths fail closed to the
same conservative base sentinels. They do not silently select no evidence, but
they also do not make a local commit wait for the complete repository matrix.
Hosted CI keeps that cold exhaustive responsibility. The selector never caches
a test or changes the canonical CI partition.
"""

from __future__ import annotations

import json
import re
import sys
from collections.abc import Iterable
from dataclasses import dataclass
from pathlib import Path
from typing import Any


FOCUSED = "focused"
AFFECTED = "affected"
PLAN_SCHEMA_VERSION = 1
HOSTED_REQUIRED_CHECK = "Governance"
OFFICIAL_HAXE_READINESS_OWNER = "haxe_c-6k7"
ROOT = Path(__file__).resolve().parents[2]
PACKAGE_JSON = ROOT / "package.json"

KNOWN_AFFECTED_PATH = re.compile(
    r"^(?:"
    r"docs/test-performance\.md"
    r"|docs/specs/fixture-taxonomy(?:\.schema)?\.json"
    r"|scripts/ci/"
    r"|scripts/hooks/"
    r"|scripts/test/"
    r"|test/governance/"
    r"|examples/caxecraft/"
    r"|docs/caxecraft-"
    r"|docs/raygui-hx\.md"
    r"|docs/specs/raygui-core-(?:binding-lock|selection)(?:\.schema)?\.json"
    r"|scripts/raygui/"
    r"|src/raygui/"
    r"|std/c/Ref\.hx"
    r"|test/(?:c_import|raygui_binding)/"
    r"|src/reflaxe/c/(?:CCompiler|CPhaseTiming|CUtf8Order|CReflaxeCompiler|CompilationContext)\.hx"
    r"|src/reflaxe/c/(?:ast|emit|frontend|interop|ir|lowering|macros|naming|plan|runtime|semantics)/"
    r")"
)

UNKNOWN_CROSS_CUTTING_PATH = re.compile(
    r"^(?:"
    r"\.github/workflows/"
    r"|package(?:-lock)?\.json"
    r"|docs/specs/(?:caxecraft-timing|toolchain-timing)\.json"
    r"|src/reflaxe/c/"
    r")"
)


@dataclass(frozen=True)
class AffectedOwner:
    """One focused package-script owner and the staged family that selected it."""

    script: str
    reason: str


# Order is part of the local evidence contract: cheap structural checks run
# first, then increasingly integrated semantic/product checks.
AFFECTED_BASE_OWNERS = (
    AffectedOwner("test:all-sources", "all target-owned Haxe must still type-check"),
    AffectedOwner("test:hxc-ir", "validated semantic IR is the compiler boundary"),
    AffectedOwner("test:hello", "a compact generated-C product must still compile and run"),
    AffectedOwner("snapshots:catalog", "checked expected outputs retain one registered owner"),
)

AFFECTED_OWNER_RULES = (
    (
        re.compile(r"^(?:src/reflaxe/c/ir/|test/hxc_ir/|docs/hxc-ir\.md)"),
        AffectedOwner(
            "test:hxc-ir",
            "the validated semantic IR or its focused evidence changed",
        ),
    ),
    (
        re.compile(
            r"^(?:src/reflaxe/c/lowering/(?:CBodyLowering|CBodyFunctionReplayCache)\.hx"
            r"|test/typed_ast/(?:run\.py|fixtures/incremental/))"
        ),
        AffectedOwner(
            "test:incremental-backend",
            "semantic-function replay or its complete invalidation matrix changed",
        ),
    ),
    (
        re.compile(r"^src/reflaxe/c/(?:ast/|lowering/|CCompiler\.hx)"),
        AffectedOwner("test:body-lowering", "C body construction or its semantic input changed"),
    ),
    (
        re.compile(r"^src/reflaxe/c/(?:emit/|plan/|CCompiler\.hx)"),
        AffectedOwner("test:project-emitter", "project or artifact planning changed"),
    ),
    (
        re.compile(r"^src/reflaxe/c/CUtf8Order\.hx"),
        AffectedOwner("test:project-emitter", "shared deterministic compiler ordering changed"),
    ),
    (
        re.compile(r"^src/reflaxe/c/(?:frontend/|CReflaxeCompiler\.hx|CompilationContext\.hx)"),
        AffectedOwner("test:typed-ast", "typed-source capture or request context changed"),
    ),
    (
        re.compile(
            r"^(?:src/reflaxe/c/interop/|std/c/Ref\.hx|test/c_import/)"
        ),
        AffectedOwner(
            "test:c-import",
            "typed direct-C imports or call-scoped native references changed",
        ),
    ),
    (
        re.compile(r"^src/reflaxe/c/naming/"),
        AffectedOwner("test:symbol-registry", "deterministic C naming changed"),
    ),
    (
        re.compile(r"^src/reflaxe/c/CUtf8Order\.hx"),
        AffectedOwner("test:symbol-registry", "shared deterministic compiler ordering changed"),
    ),
    (
        re.compile(r"^src/reflaxe/c/macros/"),
        AffectedOwner("test:typed-c", "typed C macro contract capture changed"),
    ),
    (
        re.compile(r"^src/reflaxe/c/semantics/"),
        AffectedOwner("test:primitive-semantics", "primitive semantic planning changed"),
    ),
    (
        re.compile(r"^src/reflaxe/c/runtime/|^runtime/hxrt/"),
        AffectedOwner("test:runtime-features", "runtime requirement or packaging logic changed"),
    ),
    (
        re.compile(
            r"^(?:std/c/(?:CArray|ConstSpan|Span)\.hx"
            r"|src/reflaxe/c/(?:ir|lowering)/"
            r"|test/span_lowering/"
            r"|docs/span-lowering\.md)"
        ),
        AffectedOwner("test:span-lowering", "fixed-array/span semantics may have changed"),
    ),
    (
        re.compile(
            r"^src/reflaxe/c/lowering/(?:CBodyEnum|CGenericSpecialization)\.hx"
        ),
        AffectedOwner(
            "test:generic-specialization",
            "generic function or enum specialization ownership changed",
        ),
    ),
    (
        re.compile(
            r"^examples/caxecraft/(?!profile_(?:compiler|incremental_edit)\.py$)|^docs/caxecraft-"
        ),
        AffectedOwner("test:caxecraft-domain", "the flagship compiler/product path changed"),
    ),
    (
        re.compile(
            r"^(?:docs/raygui-hx\.md"
            r"|docs/specs/raygui-core-(?:binding-lock|selection)(?:\.schema)?\.json"
            r"|scripts/raygui/"
            r"|src/raygui/"
            r"|test/raygui_binding/)"
        ),
        AffectedOwner(
            "test:raygui-binding",
            "the generated raw binding or typed Raygui surface changed",
        ),
    ),
    (
        re.compile(
            r"^(?:docs/test-performance\.md"
            r"|package(?:-lock)?\.json"
            r"|scripts/ci/(?:check_ci_policy|select_pre_commit_route)\.py"
            r"|test/governance/test_toolchain_shard\.py)"
        ),
        AffectedOwner(
            "test:governance",
            "test selection, local-loop policy, or its fail-closed guard changed",
        ),
    ),
)


def select_route(paths: Iterable[str]) -> str:
    """Return the one route required by the complete staged path set."""
    saw_affected = False
    for raw_path in paths:
        path = raw_path.strip()
        if not path:
            continue
        if KNOWN_AFFECTED_PATH.match(path):
            saw_affected = True
            continue
        if UNKNOWN_CROSS_CUTTING_PATH.match(path):
            # Unknown central paths still run the conservative base owners.
            # The pull-request matrix, rather than a developer's commit hook,
            # owns the complete exhaustive replay.
            saw_affected = True
    return AFFECTED if saw_affected else FOCUSED


def select_affected_owners(paths: Iterable[str]) -> tuple[AffectedOwner, ...]:
    """Return deterministic deduplicated owners for a known affected change."""
    normalized = tuple(path.strip() for path in paths if path.strip())
    if select_route(normalized) != AFFECTED:
        raise ValueError("affected owners require the affected pre-commit route")
    owners = list(AFFECTED_BASE_OWNERS)
    selected_scripts = {owner.script for owner in owners}
    for pattern, owner in AFFECTED_OWNER_RULES:
        if any(pattern.match(path) for path in normalized):
            if owner.script not in selected_scripts:
                owners.append(owner)
                selected_scripts.add(owner.script)
    return tuple(owners)


def select_smoke_owners(paths: Iterable[str]) -> tuple[AffectedOwner, ...]:
    """Return the fixed bounded owners that every central local change runs."""
    normalized = tuple(path.strip() for path in paths if path.strip())
    if select_route(normalized) != AFFECTED:
        raise ValueError("smoke owners require the affected pre-commit route")
    return AFFECTED_BASE_OWNERS


def select_task_owners(paths: Iterable[str]) -> tuple[AffectedOwner, ...]:
    """Return focused owners for both narrow and conservative routes."""
    normalized = tuple(path.strip() for path in paths if path.strip())
    owners: list[AffectedOwner] = []
    selected_scripts: set[str] = set()
    paths_with_explicit_owners: set[str] = set()
    for pattern, owner in AFFECTED_OWNER_RULES:
        matched_paths = tuple(path for path in normalized if pattern.match(path))
        if not matched_paths:
            continue
        paths_with_explicit_owners.update(matched_paths)
        if owner.script not in selected_scripts:
            owners.append(owner)
            selected_scripts.add(owner.script)

    # A test fixture should name its own package-script owner even when it uses
    # the narrow route. Deriving these prefixes from package.json avoids a
    # second hand-maintained test-directory catalog. More specific exceptional
    # paths, such as incremental typed-AST fixtures, are handled above.
    package_scripts = json.loads(PACKAGE_JSON.read_text(encoding="utf-8"))["scripts"]
    runner_owners: dict[str, tuple[str, bool]] = {}
    for script, command in package_scripts.items():
        if not script.startswith("test:"):
            continue
        match = re.match(r"^python3 (test/\S+)", command)
        if match is None:
            continue
        runner = match.group(1)
        prefix = runner.rsplit("/", 1)[0] + "/"
        unqualified = command == f"python3 {runner}"
        candidate = (script, unqualified)
        current = runner_owners.get(prefix)
        if current is None:
            runner_owners[prefix] = candidate
            continue
        if (unqualified and not current[1]) or (
            unqualified == current[1] and script < current[0]
        ):
            runner_owners[prefix] = candidate

    for path in normalized:
        # An explicitly routed fixture can stay with its narrow special mode,
        # but edits to the shared runner itself must also run the unqualified
        # package command. For example, typed_ast/run.py owns both the normal
        # suite and its incremental-backend mode.
        if path in paths_with_explicit_owners and not path.endswith("/run.py"):
            continue
        matching_prefixes = tuple(
            prefix for prefix in runner_owners if path.startswith(prefix)
        )
        if not matching_prefixes:
            continue
        prefix = max(matching_prefixes, key=len)
        script = runner_owners[prefix][0]
        if script not in selected_scripts:
            owners.append(
                AffectedOwner(
                    script,
                    "the changed focused-test directory is owned by this package script",
                )
            )
            selected_scripts.add(script)
    return tuple(owners)


def build_test_plan(paths: Iterable[str]) -> dict[str, Any]:
    """Describe local and hosted evidence without executing any test."""
    normalized = tuple(path.strip() for path in paths if path.strip())
    route = select_route(normalized)
    task_owners = select_task_owners(normalized)
    local_smoke = (
        (
            AffectedOwner(
                "test:governance",
                "test policy, selection, provenance, and repository wiring agree",
            ),
            *AFFECTED_BASE_OWNERS,
        )
        if route == AFFECTED
        else ()
    )
    return {
        "schemaVersion": PLAN_SCHEMA_VERSION,
        "route": route,
        "changedPaths": list(normalized),
        "taskOwners": [
            {"script": owner.script, "reason": owner.reason}
            for owner in task_owners
        ],
        "taskOwnerSelection": (
            "mapped"
            if task_owners
            else "required-from-owning-issue-or-nearest-package-script"
        ),
        "localCommitSmoke": [
            {"script": owner.script, "reason": owner.reason}
            for owner in local_smoke
        ],
        "hostedRequired": {
            "check": HOSTED_REQUIRED_CHECK,
            "scope": (
                "four complete toolchain shards plus independent native, build, "
                "platform, provenance, and security jobs"
            ),
        },
        "coldSnapshotAudit": {
            "schedule": "path-triggered, weekly, or explicit dispatch",
            "script": "snapshots:check",
        },
        "officialHaxeQualification": {
            "status": "readiness-only-not-a-pass",
            "owner": OFFICIAL_HAXE_READINESS_OWNER,
        },
    }


def print_human_plan(plan: dict[str, Any]) -> None:
    """Print the smallest useful agent-facing explanation of a test plan."""
    print(f"route: {plan['route']}")
    print("R0 focused task evidence:")
    task_owners = plan["taskOwners"]
    if task_owners:
        for owner in task_owners:
            print(f"  npm run {owner['script']}  # {owner['reason']}")
    else:
        print(
            "  choose the owning issue's narrow package script; "
            "no changed-path owner is inferred"
        )
    print("R1 local commit smoke:")
    local_smoke = plan["localCommitSmoke"]
    if local_smoke:
        for owner in local_smoke:
            print(f"  npm run {owner['script']}  # {owner['reason']}")
    else:
        print("  use the existing path-specific pre-commit checks")
    print("R2 hosted required evidence:")
    hosted = plan["hostedRequired"]
    print(f"  {hosted['check']}: {hosted['scope']}")
    cold = plan["coldSnapshotAudit"]
    print(
        "R3 cold snapshot authority: "
        f"npm run {cold['script']} ({cold['schedule']})"
    )
    official = plan["officialHaxeQualification"]
    print(
        "official Haxe suite: "
        f"{official['status']} (owner {official['owner']})"
    )


def main(arguments: Iterable[str] | None = None) -> int:
    selected_arguments = tuple(sys.argv[1:] if arguments is None else arguments)
    paths = tuple(sys.stdin)
    if selected_arguments == ("--owners",):
        for owner in select_affected_owners(paths):
            print(f"{owner.script}\t{owner.reason}")
        return 0
    if selected_arguments == ("--smoke-owners",):
        for owner in select_smoke_owners(paths):
            print(f"{owner.script}\t{owner.reason}")
        return 0
    if selected_arguments == ("--plan",):
        print_human_plan(build_test_plan(paths))
        return 0
    if selected_arguments in (("--plan", "--json"), ("--json", "--plan")):
        print(
            json.dumps(
                build_test_plan(paths),
                ensure_ascii=True,
                indent=2,
                sort_keys=True,
            )
        )
        return 0
    if selected_arguments:
        raise SystemExit(
            "usage: select_pre_commit_route.py "
            "[--owners|--smoke-owners|--plan [--json]]"
        )
    print(select_route(paths))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

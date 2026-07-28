#!/usr/bin/env python3
"""Choose one local pre-commit evidence route and its focused owners.

The hook normally maps each changed file to a small set of focused owners.
Known compiler-wide semantic layers use a deliberately bounded ``affected``
route: one reviewable set of source/semantic/integration sentinels plus focused
owners selected by the staged paths. The complete cold matrix remains a
separate pull-request, nightly, release, and explicit local command.

Unknown cross-cutting compiler or test-infrastructure paths fail closed to the
complete bounded shard runner. The selector never caches a test or changes the
canonical CI partition.
"""

from __future__ import annotations

import re
import sys
from dataclasses import dataclass
from collections.abc import Iterable


FOCUSED = "focused"
AFFECTED = "affected"
PARALLEL_EXHAUSTIVE = "parallel-exhaustive"

KNOWN_AFFECTED_PATH = re.compile(
    r"^(?:"
    r"docs/specs/fixture-taxonomy(?:\.schema)?\.json"
    r"|scripts/ci/check_ci_policy\.py"
    r"|scripts/ci/select_pre_commit_route\.py"
    r"|scripts/hooks/pre-commit"
    r"|scripts/test/snapshots\.py"
    r"|test/governance/test_toolchain_shard\.py"
    r"|test/governance/test_caxecraft_timing\.py"
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
    r"|scripts/ci/(?:check_toolchain|run_toolchain_shard)\.py"
    r"|test/governance/test_fixture_policy\.py"
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
            return PARALLEL_EXHAUSTIVE
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


def main(arguments: Iterable[str] | None = None) -> int:
    selected_arguments = tuple(sys.argv[1:] if arguments is None else arguments)
    paths = tuple(sys.stdin)
    if selected_arguments == ("--owners",):
        for owner in select_affected_owners(paths):
            print(f"{owner.script}\t{owner.reason}")
        return 0
    if selected_arguments:
        raise SystemExit("usage: select_pre_commit_route.py [--owners]")
    print(select_route(paths))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

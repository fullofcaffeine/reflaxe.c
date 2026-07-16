#!/usr/bin/env python3
"""Fail closed when required toolchain and native-smoke wiring drifts."""

from __future__ import annotations

import json
import sys
from pathlib import Path
from typing import Any


ROOT = Path(__file__).resolve().parents[2]
WORKFLOW = ROOT / ".github/workflows/governance.yml"
PACKAGE = ROOT / "package.json"
PRE_COMMIT = ROOT / "scripts/hooks/pre-commit"

REQUIRED_GATE_FILES = (
    "CONTRIBUTING.md",
    "SECURITY.md",
    "scripts/ci/check_governance_policy.py",
    "test/governance/test_governance_policy.py",
    "docs/BEADS_PLAN.md",
    "docs/specs/beads-plan.json",
    "docs/specs/beads-plan.schema.json",
    "scripts/beads/hxc_beads_plan.py",
    "scripts/beads/validate_plan.py",
    "scripts/beads/bootstrap.py",
    "test/governance/test_beads_bootstrap.py",
    "src/reflaxe/c/CDiagnostic.hx",
    "docs/diagnostics.md",
    "docs/specs/diagnostics.json",
    "docs/specs/diagnostics.schema.json",
    "docs/specs/diagnostic-event.schema.json",
    "scripts/ci/check_diagnostic_policy.py",
    "test/diagnostics/DiagnosticGolden.hx",
    "test/diagnostics/README.md",
    "test/diagnostics/diagnostics.hxml",
    "test/diagnostics/run.py",
    "test/governance/test_diagnostic_policy.py",
    "runtime/hxrt/include/hxc_runtime.h",
    "runtime/hxrt/src/hxc_runtime.c",
    "runtime/hxrt/test/runtime_smoke.c",
    "runtime/hxrt/test/public_header_cpp.cpp",
    "test/c_ast/CASTGolden.hx",
    "test/c_ast/ExpressionGolden.hx",
    "test/c_ast/c_ast.hxml",
    "test/c_ast/expression.hxml",
    "test/c_ast/expected/declarators.c",
    "test/c_ast/expected/expressions.c",
    "test/c_ast/run.py",
    "test/declaration_plan/DeclarationPlanGolden.hx",
    "test/declaration_plan/declaration_plan.hxml",
    "test/declaration_plan/expected/plan.json",
    "test/declaration_plan/expected/include/api.h",
    "test/declaration_plan/expected/include/detail/state.h",
    "test/declaration_plan/expected/include/nodes.h",
    "test/declaration_plan/expected/include/types/value.h",
    "test/declaration_plan/support/include/clock_api.h",
    "test/declaration_plan/support/include/project/config.h",
    "test/declaration_plan/smoke.c",
    "test/declaration_plan/run.py",
    "src/reflaxe/c/naming/CSymbolRequest.hx",
    "src/reflaxe/c/naming/CSymbolRegistry.hx",
    "src/reflaxe/c/naming/CSymbolRegistryError.hx",
    "src/reflaxe/c/naming/TypedCNameFinalizer.hx",
    "docs/symbol-naming.md",
    "test/symbol_registry/SymbolRegistryGolden.hx",
    "test/symbol_registry/symbol_registry.hxml",
    "test/symbol_registry/expected/symbol-registry.json",
    "test/symbol_registry/run.py",
    "test/project_emitter/ProjectEmitterGolden.hx",
    "test/project_emitter/ProjectEmitterProbe.hx",
    "test/project_emitter/project_emitter.hxml",
    "test/project_emitter/expected/hxc.manifest.json",
    "test/project_emitter/expected/hxc.runtime-plan.json",
    "test/project_emitter/expected/hxc.abi.json",
    "test/project_emitter/expected/hxc.symbols.json",
    "test/project_emitter/expected/include/hxc/emitter_fixture.h",
    "test/project_emitter/expected/src/emitter_fixture.c",
    "test/project_emitter/expected/src/hxc_boot.c",
    "test/project_emitter/run.py",
    "test/hxc_ir/HxcIRGolden.hx",
    "test/hxc_ir/hxc_ir.hxml",
    "test/hxc_ir/oracle.hxml",
    "test/hxc_ir/fixtures/SideEffectSupport.hx",
    "test/hxc_ir/fixtures/SideEffects.hx",
    "test/hxc_ir/fixtures/IRCoverage.hx",
    "test/hxc_ir/expected/semantic.hxcir",
    "test/hxc_ir/expected/coverage.hxcir",
    "test/hxc_ir/expected/diagnostics.json",
    "test/hxc_ir/run.py",
    "src/reflaxe/c/semantics/CPrimitiveTypes.hx",
    "src/reflaxe/c/semantics/CPrimitiveSemantics.hx",
    "src/reflaxe/c/semantics/CPrimitiveTypeMapper.hx",
    "src/reflaxe/c/semantics/CPrimitiveContract.hx",
    "docs/adr/0008-primitive-representations-and-conversions.md",
    "docs/primitive-semantics.md",
    "docs/specs/primitive-semantics.schema.json",
    "docs/specs/primitive-semantics.json",
    "test/primitive_semantics/PrimitiveFixture.hx",
    "test/primitive_semantics/PrimitiveSemanticsGolden.hx",
    "test/primitive_semantics/PrimitiveTypeProbe.hx",
    "test/primitive_semantics/native_contract.c",
    "test/primitive_semantics/primitive_semantics.hxml",
    "test/primitive_semantics/run.py",
    "test/positive/primitive-semantics/case.json",
    "test/snapshot/primitive-semantics/case.json",
    "src/reflaxe/c/lowering/CBodyEmissionError.hx",
    "src/reflaxe/c/lowering/CBodyEmitter.hx",
    "src/reflaxe/c/lowering/CBodyLowering.hx",
    "src/reflaxe/c/lowering/CBodyLoweringError.hx",
    "src/reflaxe/c/lowering/CPrimitiveHelperEmitter.hx",
    "src/reflaxe/c/lowering/HaxeSourceSpan.hx",
    "docs/body-lowering.md",
    "test/body_lowering/BodyLoweringProbe.hx",
    "test/body_lowering/body_lowering.hxml",
    "test/body_lowering/fixtures/positive/BodyFixture.hx",
    "test/body_lowering/fixtures/unsupported/Main.hx",
    "test/body_lowering/expected/body.c",
    "test/body_lowering/expected/body-lines.c",
    "test/body_lowering/expected/body.hxcir",
    "test/body_lowering/expected/symbols.json",
    "test/body_lowering/run.py",
    "test/positive/body-lowering/case.json",
    "test/negative/body-lowering/case.json",
    "test/snapshot/body-lowering/case.json",
    "test/runtime/body-lowering/case.json",
    "src/reflaxe/c/emit/CStaticFunctionProjectEmitter.hx",
    "src/reflaxe/c/lowering/CStaticFunctionGraph.hx",
    "docs/function-lowering.md",
    "test/function_lowering/FunctionLoweringProbe.hx",
    "test/function_lowering/function_lowering.hxml",
    "test/function_lowering/fixtures/positive/FunctionFixture.hx",
    "test/function_lowering/fixtures/default/Main.hx",
    "test/function_lowering/fixtures/optional/Main.hx",
    "test/function_lowering/fixtures/rest/Main.hx",
    "test/function_lowering/expected/functions.h",
    "test/function_lowering/expected/functions.hxcir",
    "test/function_lowering/expected/nonreturn_0000.c",
    "test/function_lowering/expected/nonreturn_0001.c",
    "test/function_lowering/expected/nonreturn_0002.c",
    "test/function_lowering/expected/program.c",
    "test/function_lowering/expected/symbols.json",
    "test/function_lowering/run.py",
    "test/positive/function-lowering/case.json",
    "test/negative/function-lowering/case.json",
    "test/snapshot/function-lowering/case.json",
    "test/runtime/function-lowering/case.json",
    "docs/evaluation-order.md",
    "test/evaluation_order/EvaluationOrderProbe.hx",
    "test/evaluation_order/evaluation_order.hxml",
    "test/evaluation_order/oracle.hxml",
    "test/evaluation_order/fixtures/EvaluationFixture.hx",
    "test/evaluation_order/expected/evaluation.hxcir",
    "test/evaluation_order/expected/program.h",
    "test/evaluation_order/expected/program.c",
    "test/evaluation_order/expected/symbols.json",
    "test/evaluation_order/run.py",
    "test/positive/evaluation-order/case.json",
    "test/snapshot/evaluation-order/case.json",
    "test/runtime/evaluation-order/case.json",
    "test/differential/evaluation-order/case.json",
    "docs/arithmetic-semantics.md",
    "test/arithmetic_semantics/ArithmeticSemanticsProbe.hx",
    "test/arithmetic_semantics/arithmetic_semantics.hxml",
    "test/arithmetic_semantics/oracle.hxml",
    "test/arithmetic_semantics/fixtures/ArithmeticFixture.hx",
    "test/arithmetic_semantics/expected/arithmetic.hxcir",
    "test/arithmetic_semantics/expected/contract.json",
    "test/arithmetic_semantics/expected/program.h",
    "test/arithmetic_semantics/expected/program.c",
    "test/arithmetic_semantics/expected/symbols.json",
    "test/arithmetic_semantics/run.py",
    "test/positive/arithmetic-semantics/case.json",
    "test/snapshot/arithmetic-semantics/case.json",
    "test/runtime/arithmetic-semantics/case.json",
    "test/differential/arithmetic-semantics/case.json",
    "src/reflaxe/c/CBuildMode.hx",
    "src/reflaxe/c/BuildModeResolver.hx",
    "std/c/CArray.hx",
    "std/c/Span.hx",
    "std/c/ConstSpan.hx",
    "docs/span-lowering.md",
    "test/span_lowering/SpanLoweringProbe.hx",
    "test/span_lowering/span_lowering.hxml",
    "test/span_lowering/fixtures/Length4.hx",
    "test/span_lowering/fixtures/SpanFixture.hx",
    "test/span_lowering/fixtures/NonLiteralFixture.hx",
    "test/span_lowering/fixtures/LookalikeFixture.hx",
    "test/span_lowering/fixtures/ZeroLengthFixture.hx",
    "test/span_lowering/fixtures/UpperBoundsFixture.hx",
    "test/span_lowering/fixtures/NegativeBoundsFixture.hx",
    "test/span_lowering/expected/span.hxcir",
    "test/span_lowering/expected/program.h",
    "test/span_lowering/expected/program.c",
    "test/span_lowering/expected/symbols.json",
    "test/span_lowering/run.py",
    "test/positive/span-lowering/case.json",
    "test/negative/span-lowering/case.json",
    "test/snapshot/span-lowering/case.json",
    "test/runtime/span-lowering/case.json",
    "src/reflaxe/c/frontend/TypedProgramInput.hx",
    "src/reflaxe/c/frontend/TypedAstNormalizer.hx",
    "src/reflaxe/c/frontend/TypedAstInventory.hx",
    "docs/typed-ast-input.md",
    "test/typed_ast/README.md",
    "test/typed_ast/fixtures/rich/Main.hx",
    "test/typed_ast/fixtures/rich/FixtureTypes.hx",
    "test/typed_ast/fixtures/isolation/Main.hx",
    "test/typed_ast/expected/typed-ast-inventory.json",
    "test/typed_ast/run.py",
    "test/positive/README.md",
    "test/negative/README.md",
    "test/ast/README.md",
    "test/snapshot/README.md",
    "test/runtime/README.md",
    "test/differential/README.md",
    "test/abi/README.md",
    "test/performance/README.md",
    "docs/testing.md",
    "docs/specs/fixture-case.schema.json",
    "docs/specs/fixture-taxonomy.schema.json",
    "docs/specs/fixture-taxonomy.json",
    "scripts/test/snapshots.py",
    "scripts/ci/check_fixture_policy.py",
    "test/governance/test_fixture_policy.py",
    "test/native/pointlib/include/pointlib.h",
    "test/native/pointlib/src/pointlib.c",
    "test/native/pointlib/smoke.c",
    "test/native/cpp_shim/include/counter_shim.h",
    "test/native/cpp_shim/src/counter_shim.cpp",
    "test/native/cpp_shim/smoke.c",
    "scripts/ci/runtime_smoke.py",
)

REQUIRED_WORKFLOW_SNIPPETS = (
    "  pinned-toolchain:\n",
    "  native-smoke:\n",
    "  license-and-provenance:\n",
    "      fail-fast: false\n",
    "        toolchain:\n",
    "          - gcc\n",
    "          - clang\n",
    'python3 scripts/ci/runtime_smoke.py --toolchain "${{ matrix.toolchain }}"',
    'python3 test/primitive_semantics/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/body_lowering/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/function_lowering/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/evaluation_order/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/arithmetic_semantics/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/span_lowering/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    "python3 scripts/beads/validate_plan.py --json",
    "python3 scripts/beads/bootstrap.py --json",
    "python3 scripts/ci/check_governance_policy.py",
    "python3 scripts/ci/check_ci_policy.py",
)


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


def validate() -> list[str]:
    errors: list[str] = []
    missing_files = [path for path in REQUIRED_GATE_FILES if not (ROOT / path).is_file()]
    if missing_files:
        errors.append("required compiler/native gate files are missing: " + ", ".join(missing_files))

    package = load_json(PACKAGE, errors)
    scripts = package.get("scripts", {})
    if not isinstance(scripts, dict):
        errors.append("package.json scripts must be an object")
        scripts = {}
    if scripts.get("test:native") != "python3 scripts/ci/runtime_smoke.py":
        errors.append("package.json must retain the test:native entry point")
    if scripts.get("test:diagnostics") != "python3 test/diagnostics/run.py":
        errors.append("package.json must retain the test:diagnostics entry point")
    if scripts.get("test:c-ast") != "python3 test/c_ast/run.py":
        errors.append("package.json must retain the test:c-ast entry point")
    if scripts.get("test:declaration-plan") != "python3 test/declaration_plan/run.py":
        errors.append("package.json must retain the test:declaration-plan entry point")
    if scripts.get("test:symbol-registry") != "python3 test/symbol_registry/run.py":
        errors.append("package.json must retain the test:symbol-registry entry point")
    if scripts.get("test:project-emitter") != "python3 test/project_emitter/run.py":
        errors.append("package.json must retain the test:project-emitter entry point")
    if scripts.get("test:hxc-ir") != "python3 test/hxc_ir/run.py":
        errors.append("package.json must retain the test:hxc-ir entry point")
    if scripts.get("test:primitive-semantics") != "python3 test/primitive_semantics/run.py":
        errors.append("package.json must retain the test:primitive-semantics entry point")
    if scripts.get("test:body-lowering") != "python3 test/body_lowering/run.py":
        errors.append("package.json must retain the test:body-lowering entry point")
    if scripts.get("test:function-lowering") != "python3 test/function_lowering/run.py":
        errors.append("package.json must retain the test:function-lowering entry point")
    if scripts.get("test:evaluation-order") != "python3 test/evaluation_order/run.py":
        errors.append("package.json must retain the test:evaluation-order entry point")
    if scripts.get("test:arithmetic-semantics") != "python3 test/arithmetic_semantics/run.py":
        errors.append("package.json must retain the test:arithmetic-semantics entry point")
    if scripts.get("test:span-lowering") != "python3 test/span_lowering/run.py":
        errors.append("package.json must retain the test:span-lowering entry point")
    beads_plan_script = str(scripts.get("test:beads-plan", ""))
    for required_beads_command in (
        "python3 scripts/beads/validate_plan.py",
        "python3 scripts/beads/bootstrap.py",
        "test_beads_bootstrap.py",
    ):
        if required_beads_command not in beads_plan_script:
            errors.append(
                "package.json test:beads-plan must execute "
                + required_beads_command
            )
    if scripts.get("test:typed-ast") != "python3 test/typed_ast/run.py":
        errors.append("package.json must retain the test:typed-ast entry point")
    if scripts.get("test:fixture-policy") != "python3 scripts/ci/check_fixture_policy.py":
        errors.append("package.json must retain the test:fixture-policy entry point")
    if (
        scripts.get("test:governance-policy")
        != "python3 scripts/ci/check_governance_policy.py"
    ):
        errors.append("package.json must retain the governance policy entry point")
    if scripts.get("snapshots:check") != "python3 scripts/test/snapshots.py --check":
        errors.append("package.json must retain the snapshots:check entry point")
    if scripts.get("snapshots:update") != "python3 scripts/test/snapshots.py --update":
        errors.append("package.json must retain the explicit snapshots:update entry point")
    if "npm run test:c-ast" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:c-ast")
    if "npm run test:diagnostics" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:diagnostics")
    if "npm run test:declaration-plan" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:declaration-plan")
    if "npm run test:symbol-registry" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:symbol-registry")
    if "npm run test:project-emitter" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:project-emitter")
    if "npm run test:hxc-ir" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:hxc-ir")
    if "npm run test:primitive-semantics" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:primitive-semantics")
    if "npm run test:body-lowering" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:body-lowering")
    if "npm run test:function-lowering" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:function-lowering")
    if "npm run test:evaluation-order" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:evaluation-order")
    if "npm run test:arithmetic-semantics" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:arithmetic-semantics")
    if "npm run test:span-lowering" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:span-lowering")
    if "npm run test:typed-ast" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:typed-ast")
    if "npm run test:beads-plan" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:beads-plan")
    if "npm run snapshots:check" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute snapshots:check")
    if "npm run test:native" not in str(scripts.get("test", "")):
        errors.append("package.json test must execute test:native")
    if "python3 scripts/ci/check_ci_policy.py" not in str(
        scripts.get("test:governance", "")
    ):
        errors.append("package.json test:governance must execute the CI policy guard")
    if "npm run test:fixture-policy" not in str(
        scripts.get("test:governance", "")
    ):
        errors.append(
            "package.json test:governance must execute the public fixture policy guard"
        )
    if "npm run test:governance-policy" not in str(
        scripts.get("test:governance", "")
    ):
        errors.append(
            "package.json test:governance must execute the contribution and "
            "security policy guard"
        )

    workflow = read_text(WORKFLOW, errors)
    for snippet in REQUIRED_WORKFLOW_SNIPPETS:
        if snippet not in workflow:
            errors.append(f"governance workflow is missing required CI contract: {snippet.strip()}")

    pre_commit = read_text(PRE_COMMIT, errors)
    if "scripts/ci/runtime_smoke.py" not in pre_commit:
        errors.append("pre-commit must run the native smoke harness for relevant changes")
    if "scripts/ci/check_ci_policy.py" not in pre_commit:
        errors.append("pre-commit must validate required CI wiring")
    if "test/c_ast/run.py" not in pre_commit:
        errors.append("pre-commit must run the structural C AST golden test")
    if "test/diagnostics/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed diagnostic policy test")
    if "test/declaration_plan/run.py" not in pre_commit:
        errors.append("pre-commit must run the declaration planning golden test")
    if "test/symbol_registry/run.py" not in pre_commit:
        errors.append("pre-commit must run the deterministic symbol registry test")
    if "test/project_emitter/run.py" not in pre_commit:
        errors.append("pre-commit must run the deterministic project emitter test")
    if "test/hxc_ir/run.py" not in pre_commit:
        errors.append("pre-commit must run the HxcIR semantic golden test")
    if "test/primitive_semantics/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed primitive semantic test")
    if "test/body_lowering/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed body-lowering test")
    if "test/function_lowering/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed function-lowering test")
    if "test/evaluation_order/run.py" not in pre_commit:
        errors.append("pre-commit must run the explicit evaluation-order test")
    if "test/arithmetic_semantics/run.py" not in pre_commit:
        errors.append("pre-commit must run the primitive arithmetic semantic test")
    if "test/span_lowering/run.py" not in pre_commit:
        errors.append("pre-commit must run the fixed-array/span lowering test")
    if "test/typed_ast/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed-AST normalization test")
    if "scripts/ci/check_fixture_policy.py" not in pre_commit:
        errors.append("pre-commit must validate the fixture and example policy")
    if "scripts/test/snapshots.py" not in pre_commit:
        errors.append("pre-commit must check registered snapshot ownership and drift")
    if "npm run test:beads-plan" not in pre_commit:
        errors.append("pre-commit must validate the reproducible Beads graph")
    if "npm run test:governance-policy" not in pre_commit:
        errors.append(
            "pre-commit must validate contribution, disclosure, and release policy"
        )

    runner = read_text(ROOT / "scripts/ci/runtime_smoke.py", errors)
    for required_flag in ("-std=c11", "-std=c++17", "-Werror", "-pedantic"):
        if required_flag not in runner:
            errors.append(f"native smoke runner lost strict flag {required_flag}")
    if "required toolchain" not in runner or "SKIP" not in runner:
        errors.append("native smoke runner must distinguish required failures from optional skips")
    if "structural-c-ast-golden-run" not in runner or "C_AST_GOLDENS" not in runner:
        errors.append("native smoke runner must compile and execute the declarator C AST golden")
    if "expression-precedence-golden-run" not in runner or "expressions.c" not in runner:
        errors.append("native smoke runner must compile and execute the expression C AST golden")
    if "declaration-header-independent-compile" not in runner or "declaration-plan-header-run" not in runner:
        errors.append("native smoke runner must independently compile and execute declaration-plan headers")
    if "project-emitter-header-independent-compile" not in runner or "project-emitter-structural-run" not in runner:
        errors.append("native smoke runner must independently compile and execute project-emitter output")

    primitive_runner = read_text(ROOT / "test/primitive_semantics/run.py", errors)
    for required_primitive_flag in (
        "-std=c11",
        "-pedantic-errors",
        "-Werror",
        "-Wconversion",
        "-Wsign-conversion",
        "-fno-fast-math",
        "-O0",
        "-O2",
    ):
        if required_primitive_flag not in primitive_runner:
            errors.append(
                "primitive semantic runner lost strict flag "
                + required_primitive_flag
            )
    if "--native-only" not in primitive_runner:
        errors.append("primitive semantic runner must expose the native matrix seam")
    if (
        "command identity mismatch" not in primitive_runner
        or '"--version"' not in primitive_runner
    ):
        errors.append(
            "primitive semantic runner must verify required compiler-family identity"
        )

    body_runner = read_text(ROOT / "test/body_lowering/run.py", errors)
    for required_body_flag in (
        "-std=c11",
        "-pedantic-errors",
        "-Werror",
        "-Wconversion",
        "-Wsign-conversion",
        "-O0",
        "-O2",
    ):
        if required_body_flag not in body_runner:
            errors.append(
                "body-lowering native runner lost strict flag " + required_body_flag
            )
    if "--native-only" not in body_runner or "--toolchain" not in body_runner:
        errors.append("body-lowering runner must expose the required native matrix seam")
    if "command identity mismatch" not in body_runner or '"--version"' not in body_runner:
        errors.append(
            "body-lowering runner must verify required compiler-family identity"
        )

    function_runner = read_text(ROOT / "test/function_lowering/run.py", errors)
    for required_function_flag in (
        "-std=c11",
        "-pedantic-errors",
        "-Werror",
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
        "-O0",
        "-O2",
    ):
        if required_function_flag not in function_runner:
            errors.append(
                "function-lowering native runner lost strict flag "
                + required_function_flag
            )
    if "--native-only" not in function_runner or "--toolchain" not in function_runner:
        errors.append("function-lowering runner must expose the required native matrix seam")
    if "command identity" not in function_runner or '"--version"' not in function_runner:
        errors.append(
            "function-lowering runner must verify required compiler-family identity"
        )

    evaluation_runner = read_text(ROOT / "test/evaluation_order/run.py", errors)
    for required_evaluation_flag in (
        "-std=c11",
        "-pedantic-errors",
        "-Werror",
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
        "-O0",
        "-O2",
    ):
        if required_evaluation_flag not in evaluation_runner:
            errors.append(
                "evaluation-order native runner lost strict flag "
                + required_evaluation_flag
            )
    if "--native-only" not in evaluation_runner or "--toolchain" not in evaluation_runner:
        errors.append("evaluation-order runner must expose the required native matrix seam")
    if "compiler_identity" not in evaluation_runner or '"--version"' not in evaluation_runner:
        errors.append(
            "evaluation-order runner must verify required compiler-family identity"
        )

    arithmetic_runner = read_text(ROOT / "test/arithmetic_semantics/run.py", errors)
    for required_arithmetic_flag in (
        "-std=c11",
        "-pedantic-errors",
        "-Werror",
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
        "-fsanitize=undefined,float-divide-by-zero",
        "-O0",
        "-O2",
    ):
        if required_arithmetic_flag not in arithmetic_runner:
            errors.append(
                "arithmetic semantic native runner lost strict flag "
                + required_arithmetic_flag
            )
    if "--native-only" not in arithmetic_runner or "--toolchain" not in arithmetic_runner:
        errors.append("arithmetic semantic runner must expose the required native matrix seam")
    if "compiler_identity" not in arithmetic_runner or '"--version"' not in arithmetic_runner:
        errors.append(
            "arithmetic semantic runner must verify required compiler-family identity"
        )

    span_runner = read_text(ROOT / "test/span_lowering/run.py", errors)
    for required_span_flag in (
        "-std=c11",
        "-pedantic-errors",
        "-Werror",
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
        "-O0",
        "-O2",
    ):
        if required_span_flag not in span_runner:
            errors.append(
                "span-lowering native runner lost strict flag " + required_span_flag
            )
    if "--native-only" not in span_runner or "--toolchain" not in span_runner:
        errors.append("span-lowering runner must expose the required native matrix seam")
    if "compiler_identity" not in span_runner or '"--version"' not in span_runner:
        errors.append(
            "span-lowering runner must verify required compiler-family identity"
        )

    return errors


def main() -> int:
    errors = validate()
    if errors:
        for error in errors:
            print(f"ci-policy: ERROR: {error}", file=sys.stderr)
        return 1
    print("ci-policy: OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

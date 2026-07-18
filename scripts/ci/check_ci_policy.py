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
PRE_PUSH = ROOT / "scripts/hooks/pre-push"
BUILD_ADAPTER_REQUIREMENTS = ROOT / "scripts/ci/build-adapter-requirements.txt"
EXPECTED_BUILD_ADAPTER_REQUIREMENTS = """# Pure-Python Meson wheel used only by the required CI build-adapter proof.
meson==1.11.1 \\
    --hash=sha256:9b3a023657e393dbc5335b95c561337d49b7a458f5541e47ec44f2cc566e0d80
"""
PUBLIC_PREFLIGHT_COMMAND = (
    "npm run format:haxe:check && npm run security:gitleaks && "
    "npm run security:beads-history && npm run test:security-tooling && "
    "npm run test:governance"
)

REQUIRED_GATE_FILES = (
    ".gitignore",
    ".gitleaks.toml",
    ".beads/hooks/pre-commit",
    ".beads/hooks/pre-push",
    "scripts/hooks/install.sh",
    "scripts/hooks/pre-commit",
    "scripts/hooks/pre-push",
    "scripts/beads/push-safe.sh",
    "scripts/lint/hx_format_guard.sh",
    "scripts/lint/local_path_guard_staged.sh",
    "scripts/security/run-gitleaks.sh",
    "scripts/security/run-beads-gitleaks.sh",
    "scripts/ci/install-gitleaks.sh",
    "scripts/ci/check_security_tooling.py",
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
    "runtime/hxrt/include/hxrt/base.h",
    "runtime/hxrt/include/hxrt/abi.h",
    "runtime/hxrt/include/hxrt/status.h",
    "runtime/hxrt/include/hxrt/allocator.h",
    "runtime/hxrt/include/hxrt/array.h",
    "runtime/hxrt/include/hxrt/io.h",
    "runtime/hxrt/include/hxrt/string.h",
    "runtime/hxrt/include/hxrt/string_literal.h",
    "runtime/hxrt/src/abi.c",
    "runtime/hxrt/src/status.c",
    "runtime/hxrt/src/allocator.c",
    "runtime/hxrt/src/array.c",
    "runtime/hxrt/src/io.c",
    "runtime/hxrt/src/string.c",
    "runtime/hxrt/features.json",
    "docs/specs/runtime-features.schema.json",
    "docs/allocator-abi.md",
    "docs/array-runtime.md",
    "docs/string-runtime.md",
    "runtime/hxrt/test/allocator_abi.c",
    "runtime/hxrt/test/allocator_contract.c",
    "runtime/hxrt/test/runtime_smoke.c",
    "runtime/hxrt/test/public_header_cpp.cpp",
    "test/abi/allocator-contract/case.json",
    "test/differential/array-runtime/ArrayRuntimeOracle.hx",
    "test/differential/array-runtime/array_runtime.c",
    "test/differential/array-runtime/case.json",
    "test/differential/array-runtime/oracle.hxml",
    "test/differential/array-runtime/run.py",
    "test/differential/string-runtime/StringRuntimeOracle.hx",
    "test/differential/string-runtime/case.json",
    "test/differential/string-runtime/oracle.hxml",
    "test/differential/string-runtime/run.py",
    "test/differential/string-runtime/string_runtime.c",
    "test/runtime/runtime-feature-graph/RuntimeFeatureGraphGolden.hx",
    "test/runtime/runtime-feature-graph/case.json",
    "test/runtime/runtime-feature-graph/runtime_feature_graph.hxml",
    "test/runtime/runtime-feature-graph/expected/runtime-feature-plans.json",
    "test/runtime/runtime-feature-graph/alloc_consumer.c",
    "test/runtime/runtime-feature-graph/array_consumer.c",
    "test/runtime/runtime-feature-graph/io_consumer.c",
    "test/runtime/runtime-feature-graph/string_consumer.c",
    "test/runtime/runtime-feature-graph/run.py",
    "scripts/test/c_fixture_harness.py",
    "test/c_ast/ASTFixtureCompiler.hx",
    "test/c_ast/ASTFixtureCompilerProbe.hx",
    "test/c_ast/CASTGolden.hx",
    "test/c_ast/ExpressionGolden.hx",
    "test/c_ast/c_ast.hxml",
    "test/c_ast/expression.hxml",
    "test/c_ast/fixture_compiler.hxml",
    "test/c_ast/expected/attributes.c",
    "test/c_ast/expected/declarators.c",
    "test/c_ast/expected/expressions.c",
    "test/c_ast/expected/include/hxc/ast_fixture.h",
    "test/c_ast/expected/src/ast_fixture.c",
    "test/c_ast/expected/src/main.c",
    "test/c_ast/run.py",
    "test/ast/c-ast-roundtrip/case.json",
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
    "src/reflaxe/c/emit/CBuildPlan.hx",
    "src/reflaxe/c/emit/CBuildAdapterEmitter.hx",
    "scripts/ci/build-adapter-requirements.txt",
    "test/project_emitter/expected/hxc.manifest.json",
    "test/project_emitter/expected/cmake/CMakeLists.txt",
    "test/project_emitter/expected/meson.build",
    "test/project_emitter/expected/hxc.runtime-plan.json",
    "test/project_emitter/expected/hxc.abi.json",
    "test/project_emitter/expected/hxc.symbols.json",
    "test/project_emitter/expected/include/hxc/emitter_fixture.h",
    "test/project_emitter/expected/src/emitter_fixture.c",
    "test/project_emitter/expected/src/hxc_boot.c",
    "test/project_emitter/run.py",
    "src/reflaxe/c/CEnvironment.hx",
    "src/reflaxe/c/CRuntimeDiagnostics.hx",
    "src/reflaxe/c/CRuntimePolicy.hx",
    "src/reflaxe/c/runtime/RuntimeAbiContract.hx",
    "src/reflaxe/c/runtime/RuntimeFeatureModel.hx",
    "src/reflaxe/c/runtime/RuntimeFeatureError.hx",
    "src/reflaxe/c/runtime/RuntimeFeatureCatalog.hx",
    "src/reflaxe/c/runtime/RuntimeFeatureRegistry.hx",
    "src/reflaxe/c/runtime/RuntimeFeaturePlanner.hx",
    "src/reflaxe/c/runtime/RuntimeFeaturePackager.hx",
    "docs/runtime-feature-planning.md",
    "test/hxc_ir/HxcIRGolden.hx",
    "src/reflaxe/c/ir/HxcJsonString.hx",
    "src/reflaxe/c/ir/HxcUtf8.hx",
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
    "docs/stdlib-ledger.md",
    "docs/specs/stdlib-ledger.schema.json",
    "docs/specs/stdlib-ledger.json",
    "docs/specs/stdlib-ledger.csv",
    "test/stdlib_ledger/StdlibLedgerFixture.hx",
    "test/stdlib_ledger/StdlibSurfaceProbe.hx",
    "test/stdlib_ledger/stdlibprobe/Init.hx",
    "test/stdlib_ledger/run.py",
    "test/positive/stdlib-ledger/case.json",
    "test/negative/stdlib-ledger/case.json",
    "test/snapshot/stdlib-ledger/case.json",
    "src/reflaxe/c/lowering/CBodyEmissionError.hx",
    "src/reflaxe/c/lowering/CBodyAggregate.hx",
    "src/reflaxe/c/lowering/CBodyEmitter.hx",
    "src/reflaxe/c/lowering/CBodyLowering.hx",
    "src/reflaxe/c/lowering/CBodyLoweringError.hx",
    "src/reflaxe/c/lowering/CBodyRuntimeNames.hx",
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
    "docs/aggregate-lowering.md",
    "test/aggregate_lowering/AggregateLoweringProbe.hx",
    "test/aggregate_lowering/aggregate_lowering.hxml",
    "test/aggregate_lowering/fixtures/positive/AggregateFixture.hx",
    "test/aggregate_lowering/fixtures/mutation/Main.hx",
    "test/aggregate_lowering/fixtures/identity_equality/Main.hx",
    "test/aggregate_lowering/fixtures/dynamic/Main.hx",
    "test/aggregate_lowering/fixtures/void_field/Main.hx",
    "test/aggregate_lowering/native/layout_consumer.c",
    "test/aggregate_lowering/native/layout_consumer.cpp",
    "test/aggregate_lowering/native/layout_provider.c",
    "test/aggregate_lowering/expected/aggregates.hxcir",
    "test/aggregate_lowering/expected/aggregates.json",
    "test/aggregate_lowering/expected/program.h",
    "test/aggregate_lowering/expected/program.c",
    "test/aggregate_lowering/expected/symbols.json",
    "test/aggregate_lowering/run.py",
    "test/positive/aggregate-lowering/case.json",
    "test/negative/aggregate-lowering/case.json",
    "test/snapshot/aggregate-lowering/case.json",
    "test/runtime/aggregate-lowering/case.json",
    "test/typed_c/fixtures/metal_packed_struct/Main.hx",
    "src/reflaxe/c/lowering/CBodyClass.hx",
    "docs/class-layout.md",
    "test/class_layout/ClassLayoutProbe.hx",
    "test/class_layout/class_layout.hxml",
    "test/class_layout/fixtures/positive/ClassLayoutFixture.hx",
    "test/class_layout/fixtures/interface/Main.hx",
    "test/class_layout/fixtures/generic/Main.hx",
    "test/class_layout/fixtures/downcast/Main.hx",
    "test/class_layout/native/behavior_consumer.c",
    "test/class_layout/native/layout_consumer.cpp",
    "test/class_layout/native/layout_provider.c",
    "test/class_layout/expected/classes.hxcir",
    "test/class_layout/expected/classes.json",
    "test/class_layout/expected/functions.json",
    "test/class_layout/expected/program.h",
    "test/class_layout/expected/program.c",
    "test/class_layout/expected/symbols.json",
    "test/class_layout/run.py",
    "test/positive/class-layout/case.json",
    "test/negative/class-layout/case.json",
    "test/snapshot/class-layout/case.json",
    "test/runtime/class-layout/case.json",
    "src/reflaxe/c/lowering/CBodyConstructor.hx",
    "docs/constructor-lowering.md",
    "test/constructor_lowering/fixtures/positive/Main.hx",
    "test/constructor_lowering/fixtures/minimal/Main.hx",
    "test/constructor_lowering/fixtures/oracle/Main.hx",
    "test/constructor_lowering/fixtures/default_runtime/Main.hx",
    "test/constructor_lowering/fixtures/failure_runtime/Main.hx",
    "test/constructor_lowering/fixtures/conditional/Main.hx",
    "test/constructor_lowering/fixtures/cycle/Main.hx",
    "test/constructor_lowering/fixtures/escape_alias/Main.hx",
    "test/constructor_lowering/fixtures/escape_argument/Main.hx",
    "test/constructor_lowering/fixtures/escape_return/Main.hx",
    "test/constructor_lowering/fixtures/escape_self/Main.hx",
    "test/constructor_lowering/fixtures/generic/Main.hx",
    "test/constructor_lowering/fixtures/native_layout/Main.hx",
    "test/constructor_lowering/native/constructor_header_cpp.cpp",
    "test/constructor_lowering/expected/constructors.hxcir",
    "test/constructor_lowering/expected/constructors.json",
    "test/constructor_lowering/expected/program.h",
    "test/constructor_lowering/expected/program.c",
    "test/constructor_lowering/expected/symbols.json",
    "test/constructor_lowering/run.py",
    "test/positive/constructor-lowering/case.json",
    "test/negative/constructor-lowering/case.json",
    "test/snapshot/constructor-lowering/case.json",
    "test/runtime/constructor-lowering/case.json",
    "src/reflaxe/c/lowering/CBodyDispatch.hx",
    "src/reflaxe/c/lowering/CDispatchReport.hx",
    "docs/virtual-dispatch.md",
    "docs/specs/dispatch-report.schema.json",
    "test/virtual_dispatch/fixtures/positive/BaseWorker.hx",
    "test/virtual_dispatch/fixtures/positive/MiddleWorker.hx",
    "test/virtual_dispatch/fixtures/positive/LeafWorker.hx",
    "test/virtual_dispatch/fixtures/positive/FinalWorker.hx",
    "test/virtual_dispatch/fixtures/positive/Main.hx",
    "test/virtual_dispatch/fixtures/positive/build.hxml",
    "test/virtual_dispatch/fixtures/contravariant_argument/Main.hx",
    "test/virtual_dispatch/fixtures/contravariant_argument/build.hxml",
    "test/virtual_dispatch/fixtures/covariant_return/Main.hx",
    "test/virtual_dispatch/fixtures/covariant_return/build.hxml",
    "test/virtual_dispatch/native/dispatch_header_cpp.cpp",
    "test/virtual_dispatch/expected/dispatch.hxcir",
    "test/virtual_dispatch/expected/hxc.dispatch.json",
    "test/virtual_dispatch/expected/program.h",
    "test/virtual_dispatch/expected/program.c",
    "test/virtual_dispatch/expected/symbols.json",
    "test/virtual_dispatch/run.py",
    "test/positive/virtual-dispatch/case.json",
    "test/negative/virtual-dispatch/case.json",
    "test/snapshot/virtual-dispatch/case.json",
    "test/runtime/virtual-dispatch/case.json",
    "src/reflaxe/c/lowering/CBodyEnum.hx",
    "docs/enum-lowering.md",
    "test/enum_lowering/EnumLoweringProbe.hx",
    "test/enum_lowering/enum_lowering.hxml",
    "test/enum_lowering/fixtures/positive/EnumFixture.hx",
    "test/enum_lowering/fixtures/recursive_escape/Main.hx",
    "test/enum_lowering/fixtures/recursive_return/Main.hx",
    "test/enum_lowering/fixtures/reference_payload/Main.hx",
    "test/enum_lowering/fixtures/aggregate_payload/Main.hx",
    "test/enum_lowering/fixtures/nonexhaustive/Main.hx",
    "test/enum_lowering/native/layout_consumer.c",
    "test/enum_lowering/native/layout_consumer.cpp",
    "test/enum_lowering/native/layout_provider.c",
    "test/enum_lowering/expected/enums.hxcir",
    "test/enum_lowering/expected/enums.json",
    "test/enum_lowering/expected/program.h",
    "test/enum_lowering/expected/program.c",
    "test/enum_lowering/expected/symbols.json",
    "test/enum_lowering/run.py",
    "test/positive/enum-lowering/case.json",
    "test/negative/enum-lowering/case.json",
    "test/snapshot/enum-lowering/case.json",
    "test/runtime/enum-lowering/case.json",
    "src/reflaxe/c/lowering/CGenericSpecialization.hx",
    "src/reflaxe/c/lowering/CGenericSpecializationContract.hx",
    "src/reflaxe/c/lowering/CGenericSpecializationReport.hx",
    "docs/generic-specialization.md",
    "docs/specs/generic-specialization-report.schema.json",
    "test/generic_specialization/fixtures/positive/Main.hx",
    "test/generic_specialization/fixtures/positive/build.hxml",
    "test/generic_specialization/fixtures/plain/Main.hx",
    "test/generic_specialization/fixtures/plain/build.hxml",
    "test/generic_specialization/fixtures/dynamic/Main.hx",
    "test/generic_specialization/fixtures/dynamic/build.hxml",
    "test/generic_specialization/fixtures/open/Main.hx",
    "test/generic_specialization/fixtures/open/build.hxml",
    "test/generic_specialization/fixtures/budget/Main.hx",
    "test/generic_specialization/fixtures/budget/build.hxml",
    "test/generic_specialization/fixtures/type_budget/Main.hx",
    "test/generic_specialization/fixtures/type_budget/build.hxml",
    "test/generic_specialization/fixtures/code_size/Main.hx",
    "test/generic_specialization/fixtures/code_size/build.hxml",
    "test/generic_specialization/expected/hxc.specializations.json",
    "test/generic_specialization/run.py",
    "test/positive/generic-specialization/case.json",
    "test/negative/generic-specialization/case.json",
    "test/snapshot/generic-specialization/case.json",
    "test/runtime/generic-specialization/case.json",
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
    "src/reflaxe/c/plan/CStaticInitializationModel.hx",
    "src/reflaxe/c/plan/CStaticInitializationPlanner.hx",
    "src/reflaxe/c/plan/CStaticInitializationError.hx",
    "docs/static-initialization.md",
    "test/static_initialization/fixtures/positive/AStaticInitDependent.hx",
    "test/static_initialization/fixtures/positive/MStaticInitTrace.hx",
    "test/static_initialization/fixtures/positive/ZStaticInitPrerequisite.hx",
    "test/static_initialization/fixtures/positive/StaticInitializationFixture.hx",
    "test/static_initialization/fixtures/cycle/StaticInitializationCycleA.hx",
    "test/static_initialization/fixtures/cycle/StaticInitializationCycleB.hx",
    "test/static_initialization/fixtures/cycle/StaticInitializationCycleFixture.hx",
    "test/static_initialization/expected/initialization-plan.json",
    "test/static_initialization/expected/initialization.hxcir",
    "test/static_initialization/expected/program.h",
    "test/static_initialization/expected/program.c",
    "test/static_initialization/run.py",
    "test/positive/static-initialization/case.json",
    "test/negative/static-initialization/case.json",
    "test/snapshot/static-initialization/case.json",
    "test/runtime/static-initialization/case.json",
    "test/differential/static-initialization/case.json",
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
    "docs/primitive-differential.md",
    "docs/specs/primitive-divergences.schema.json",
    "docs/specs/primitive-divergences.json",
    "test/primitive_differential/seed.json",
    "test/primitive_differential/regressions/minimizer-regression.json",
    "test/primitive_differential/expected/PrimitiveDifferentialFixture.hx",
    "test/primitive_differential/expected/corpus.json",
    "test/primitive_differential/expected/divergence-oracle.txt",
    "test/primitive_differential/expected/oracle.txt",
    "test/primitive_differential/expected/program.h",
    "test/primitive_differential/expected/program.c",
    "test/primitive_differential/expected/runtime-plan.json",
    "test/primitive_differential/expected/symbols.json",
    "test/primitive_differential/run.py",
    "test/positive/primitive-differential/case.json",
    "test/snapshot/primitive-differential/case.json",
    "test/runtime/primitive-differential/case.json",
    "test/differential/primitive-differential/case.json",
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
    "test/string_output/fixtures/positive/Main.hx",
    "test/string_output/fixtures/nonliteral/Main.hx",
    "test/string_output/fixtures/nonstring/Main.hx",
    "test/string_output/fixtures/sys_print/Main.hx",
    "test/string_output/fixtures/trace_custom/Main.hx",
    "test/string_output/expected/output.hxcir",
    "test/string_output/expected/program.h",
    "test/string_output/expected/program.c",
    "test/string_output/expected/runtime-plan.json",
    "test/string_output/expected/stdlib-report.json",
    "test/string_output/run.py",
    "test/positive/string-output/case.json",
    "test/negative/string-output/case.json",
    "test/snapshot/string-output/case.json",
    "test/runtime/string-output/case.json",
    "test/differential/string-output/case.json",
    "examples/hello/Main.hx",
    "examples/hello/build.hxml",
    "examples/hello/oracle.hxml",
    "examples/hello/README.md",
    "examples/hello/case.json",
    "examples/hello/run.py",
    "examples/hello/expected/hello.hxcir",
    "examples/hello/expected/include/hxc/program.h",
    "examples/hello/expected/src/program.c",
    "examples/hello/expected/hxc.runtime-plan.json",
    "examples/hello/expected/hxc.stdlib-report.json",
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
    "docs/specs/bootstrap-inventory.json",
    "docs/specs/bootstrap-inventory.schema.json",
    "scripts/ci/check_capability_manifest.py",
    "test/governance/test_capability_manifest.py",
    "docs/typed-boundaries.md",
    "docs/specs/typed-boundaries.json",
    "docs/specs/typed-boundaries.schema.json",
    "scripts/ci/check_typed_boundaries.py",
    "test/governance/test_typed_boundaries.py",
    "test/native/pointlib/include/pointlib.h",
    "test/native/pointlib/src/pointlib.c",
    "test/native/pointlib/smoke.c",
    "test/c_import/expected/program.h",
    "test/c_import/expected/program.c",
    "test/c_import/expected/build.json",
    "test/c_import/expected/runtime-plan.json",
    "test/c_import/native/abi_probe.c",
    "test/c_import/run.py",
    "test/native/cpp_shim/include/counter_shim.h",
    "test/native/cpp_shim/src/counter_shim.cpp",
    "test/native/cpp_shim/smoke.c",
    "scripts/ci/runtime_smoke.py",
)

REQUIRED_WORKFLOW_SNIPPETS = (
    "  secret-scan:\n",
    "  haxe-format:\n",
    "  pinned-toolchain:\n",
    "  native-smoke:\n",
    "  build-adapters:\n",
    "  license-and-provenance:\n",
    "      fail-fast: false\n",
    "        toolchain:\n",
    "          - gcc\n",
    "          - clang\n",
    "fetch-depth: 0",
    "bash scripts/ci/install-gitleaks.sh --install-dir",
    "bash scripts/security/run-gitleaks.sh",
    "npx --no-install haxelib install formatter 1.18.0 --quiet",
    "npm run format:haxe:check",
    'python3 scripts/ci/runtime_smoke.py --toolchain "${{ matrix.toolchain }}"',
    'python3 test/primitive_semantics/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/body_lowering/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/function_lowering/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/aggregate_lowering/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/class_layout/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/constructor_lowering/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/virtual_dispatch/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/enum_lowering/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/generic_specialization/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/evaluation_order/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/static_initialization/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/arithmetic_semantics/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    'python3 test/span_lowering/run.py --native-only --toolchain "${{ matrix.toolchain }}"',
    "--require-hashes",
    "--no-input",
    "--only-binary=:all:",
    "-r scripts/ci/build-adapter-requirements.txt",
    'npm run test:build-adapters -- --toolchain "${{ matrix.toolchain }}"',
    "python3 scripts/beads/validate_plan.py --json",
    "python3 scripts/beads/bootstrap.py --json",
    "python3 scripts/ci/check_governance_policy.py",
    "python3 scripts/ci/check_capability_manifest.py",
    "python3 scripts/ci/check_typed_boundaries.py",
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
    if scripts.get("format:haxe:check") != "bash scripts/lint/hx_format_guard.sh":
        errors.append("package.json must retain the exact Haxe formatter gate")
    if scripts.get("security:gitleaks") != "bash scripts/security/run-gitleaks.sh":
        errors.append("package.json must retain the full-history Gitleaks gate")
    if (
        scripts.get("security:gitleaks:staged")
        != "bash scripts/security/run-gitleaks.sh --staged"
    ):
        errors.append("package.json must retain the staged Gitleaks gate")
    if (
        scripts.get("security:beads-history")
        != "bash scripts/security/run-beads-gitleaks.sh"
    ):
        errors.append("package.json must retain the decoded Beads history scan")
    if scripts.get("beads:push") != "bash scripts/beads/push-safe.sh":
        errors.append("package.json must retain the guarded Beads push entry point")
    if (
        scripts.get("test:security-tooling")
        != "python3 scripts/ci/check_security_tooling.py"
    ):
        errors.append("package.json must retain the security-tooling policy gate")
    if (
        scripts.get("public:preflight")
        != PUBLIC_PREFLIGHT_COMMAND
    ):
        errors.append("package.json must retain the complete public preflight gate")
    if scripts.get("test:diagnostics") != "python3 test/diagnostics/run.py":
        errors.append("package.json must retain the test:diagnostics entry point")
    if scripts.get("test:c-ast") != "python3 test/c_ast/run.py":
        errors.append("package.json must retain the test:c-ast entry point")
    if scripts.get("test:c-import") != "python3 test/c_import/run.py":
        errors.append("package.json must retain the test:c-import entry point")
    if scripts.get("test:declaration-plan") != "python3 test/declaration_plan/run.py":
        errors.append("package.json must retain the test:declaration-plan entry point")
    if scripts.get("test:symbol-registry") != "python3 test/symbol_registry/run.py":
        errors.append("package.json must retain the test:symbol-registry entry point")
    if scripts.get("test:project-emitter") != "python3 test/project_emitter/run.py":
        errors.append("package.json must retain the test:project-emitter entry point")
    if (
        scripts.get("test:build-adapters")
        != "python3 test/project_emitter/run.py --build-adapters required"
    ):
        errors.append("package.json must retain the required build-adapter entry point")
    if scripts.get("test:runtime-features") != "python3 test/runtime/runtime-feature-graph/run.py":
        errors.append("package.json must retain the test:runtime-features entry point")
    if scripts.get("test:array-runtime") != "python3 test/differential/array-runtime/run.py":
        errors.append("package.json must retain the test:array-runtime entry point")
    if scripts.get("test:string-runtime") != "python3 test/differential/string-runtime/run.py":
        errors.append("package.json must retain the test:string-runtime entry point")
    if scripts.get("test:string-output") != "python3 test/string_output/run.py":
        errors.append("package.json must retain the test:string-output entry point")
    if scripts.get("test:hello") != "python3 examples/hello/run.py":
        errors.append("package.json must retain the test:hello entry point")
    if scripts.get("test:hxc-ir") != "python3 test/hxc_ir/run.py":
        errors.append("package.json must retain the test:hxc-ir entry point")
    if scripts.get("test:primitive-semantics") != "python3 test/primitive_semantics/run.py":
        errors.append("package.json must retain the test:primitive-semantics entry point")
    if scripts.get("test:stdlib-ledger") != "python3 test/stdlib_ledger/run.py":
        errors.append("package.json must retain the test:stdlib-ledger entry point")
    if scripts.get("test:body-lowering") != "python3 test/body_lowering/run.py":
        errors.append("package.json must retain the test:body-lowering entry point")
    if scripts.get("test:function-lowering") != "python3 test/function_lowering/run.py":
        errors.append("package.json must retain the test:function-lowering entry point")
    if scripts.get("test:aggregate-lowering") != "python3 test/aggregate_lowering/run.py":
        errors.append("package.json must retain the test:aggregate-lowering entry point")
    if scripts.get("test:class-layout") != "python3 test/class_layout/run.py":
        errors.append("package.json must retain the test:class-layout entry point")
    if (
        scripts.get("test:constructor-lowering")
        != "python3 test/constructor_lowering/run.py"
    ):
        errors.append("package.json must retain the test:constructor-lowering entry point")
    if scripts.get("test:virtual-dispatch") != "python3 test/virtual_dispatch/run.py":
        errors.append("package.json must retain the test:virtual-dispatch entry point")
    if scripts.get("test:enum-lowering") != "python3 test/enum_lowering/run.py":
        errors.append("package.json must retain the test:enum-lowering entry point")
    if (
        scripts.get("test:generic-specialization")
        != "python3 test/generic_specialization/run.py"
    ):
        errors.append("package.json must retain the test:generic-specialization entry point")
    if scripts.get("test:evaluation-order") != "python3 test/evaluation_order/run.py":
        errors.append("package.json must retain the test:evaluation-order entry point")
    if scripts.get("test:static-initialization") != "python3 test/static_initialization/run.py":
        errors.append("package.json must retain the test:static-initialization entry point")
    if scripts.get("test:arithmetic-semantics") != "python3 test/arithmetic_semantics/run.py":
        errors.append("package.json must retain the test:arithmetic-semantics entry point")
    if scripts.get("test:primitive-differential") != "python3 test/primitive_differential/run.py":
        errors.append("package.json must retain the test:primitive-differential entry point")
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
        scripts.get("test:capabilities")
        != "python3 scripts/ci/check_capability_manifest.py"
    ):
        errors.append("package.json must retain the capability manifest entry point")
    if (
        scripts.get("test:typed-boundaries")
        != "python3 scripts/ci/check_typed_boundaries.py"
    ):
        errors.append("package.json must retain the typed-boundary guard entry point")
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
    if "npm run test:runtime-features" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:runtime-features")
    if "npm run test:array-runtime" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:array-runtime")
    if "npm run test:string-runtime" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:string-runtime")
    if "npm run test:string-output" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:string-output")
    if "npm run test:hello" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:hello")
    if "npm run test:hxc-ir" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:hxc-ir")
    if "npm run test:primitive-semantics" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:primitive-semantics")
    if "npm run test:stdlib-ledger" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:stdlib-ledger")
    if "npm run test:body-lowering" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:body-lowering")
    if "npm run test:function-lowering" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:function-lowering")
    if "npm run test:aggregate-lowering" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:aggregate-lowering")
    if "npm run test:class-layout" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:class-layout")
    if "npm run test:constructor-lowering" not in str(
        scripts.get("test:toolchain", "")
    ):
        errors.append("package.json test:toolchain must execute test:constructor-lowering")
    if "npm run test:virtual-dispatch" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:virtual-dispatch")
    if "npm run test:enum-lowering" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:enum-lowering")
    if "npm run test:generic-specialization" not in str(
        scripts.get("test:toolchain", "")
    ):
        errors.append("package.json test:toolchain must execute test:generic-specialization")
    if "npm run test:evaluation-order" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:evaluation-order")
    if "npm run test:static-initialization" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:static-initialization")
    if "npm run test:arithmetic-semantics" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:arithmetic-semantics")
    if "npm run test:primitive-differential" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:primitive-differential")
    if "npm run test:span-lowering" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:span-lowering")
    if "npm run test:typed-ast" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:typed-ast")
    if "npm run test:c-import" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:c-import")
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
    if "npm run test:security-tooling" not in str(
        scripts.get("test:governance", "")
    ):
        errors.append(
            "package.json test:governance must execute the security-tooling guard"
        )
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
    if "npm run test:capabilities" not in str(scripts.get("test:governance", "")):
        errors.append(
            "package.json test:governance must execute the capability manifest guard"
        )
    if "npm run test:typed-boundaries" not in str(
        scripts.get("test:governance", "")
    ):
        errors.append(
            "package.json test:governance must execute the typed-boundary guard"
        )

    workflow = read_text(WORKFLOW, errors)
    for snippet in REQUIRED_WORKFLOW_SNIPPETS:
        if snippet not in workflow:
            errors.append(f"governance workflow is missing required CI contract: {snippet.strip()}")

    if read_text(BUILD_ADAPTER_REQUIREMENTS, errors) != EXPECTED_BUILD_ADAPTER_REQUIREMENTS:
        errors.append("build-adapter Meson version and wheel SHA-256 pin drifted")

    pre_commit = read_text(PRE_COMMIT, errors)
    if "scripts/security/run-gitleaks.sh\" --staged" not in pre_commit:
        errors.append("pre-commit must scan staged content for secrets")
    if "scripts/lint/hx_format_guard.sh\" --tool-only" not in pre_commit:
        errors.append("pre-commit must require the exact Haxe formatter")
    if "scripts/ci/runtime_smoke.py" not in pre_commit:
        errors.append("pre-commit must run the native smoke harness for relevant changes")
    if "scripts/ci/check_ci_policy.py" not in pre_commit:
        errors.append("pre-commit must validate required CI wiring")
    if "test/c_ast/run.py" not in pre_commit:
        errors.append("pre-commit must run the structural C AST golden test")
    if "c_fixture_harness" not in pre_commit or "c-ast-roundtrip" not in pre_commit:
        errors.append(
            "pre-commit must route reusable C fixture harness and canonical AST case changes"
        )
    if "test/diagnostics/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed diagnostic policy test")
    if "test/declaration_plan/run.py" not in pre_commit:
        errors.append("pre-commit must run the declaration planning golden test")
    if "test/symbol_registry/run.py" not in pre_commit:
        errors.append("pre-commit must run the deterministic symbol registry test")
    if "test/project_emitter/run.py" not in pre_commit:
        errors.append("pre-commit must run the deterministic project emitter test")
    if "test/runtime/runtime-feature-graph/run.py" not in pre_commit:
        errors.append("pre-commit must run the selective runtime feature test")
    if "test/differential/array-runtime/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed array runtime test")
    if "test/differential/string-runtime/run.py" not in pre_commit:
        errors.append("pre-commit must run the UTF-8 scalar string runtime test")
    if "test/string_output/run.py" not in pre_commit:
        errors.append("pre-commit must run the generated literal-output test")
    if "examples/hello/run.py" not in pre_commit:
        errors.append("pre-commit must run the generated hello product example")
    if "test/hxc_ir/run.py" not in pre_commit:
        errors.append("pre-commit must run the HxcIR semantic golden test")
    if "test/primitive_semantics/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed primitive semantic test")
    if "test/stdlib_ledger/run.py" not in pre_commit:
        errors.append("pre-commit must run the pinned stdlib ledger test")
    if "test/body_lowering/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed body-lowering test")
    if "test/function_lowering/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed function-lowering test")
    if "test/aggregate_lowering/run.py" not in pre_commit:
        errors.append("pre-commit must run the closed aggregate-lowering test")
    if "test/class_layout/run.py" not in pre_commit:
        errors.append("pre-commit must run the concrete class-layout test")
    if "test/constructor_lowering/run.py" not in pre_commit:
        errors.append("pre-commit must run the bounded constructor-lowering test")
    if "test/virtual_dispatch/run.py" not in pre_commit:
        errors.append("pre-commit must run the closed-world virtual-dispatch test")
    if "test/enum_lowering/run.py" not in pre_commit:
        errors.append("pre-commit must run the Haxe enum-lowering test")
    if "test/generic_specialization/run.py" not in pre_commit:
        errors.append("pre-commit must run the generic-specialization test")
    if "test/evaluation_order/run.py" not in pre_commit:
        errors.append("pre-commit must run the explicit evaluation-order test")
    if "test/static_initialization/run.py" not in pre_commit:
        errors.append("pre-commit must run the deterministic static-initialization test")
    if "test/arithmetic_semantics/run.py" not in pre_commit:
        errors.append("pre-commit must run the primitive arithmetic semantic test")
    if "test/primitive_differential/run.py" not in pre_commit:
        errors.append("pre-commit must run the seeded primitive differential test")
    if "test/span_lowering/run.py" not in pre_commit:
        errors.append("pre-commit must run the fixed-array/span lowering test")
    if "test/typed_ast/run.py" not in pre_commit:
        errors.append("pre-commit must run the typed-AST normalization test")
    if "test/c_import/run.py" not in pre_commit:
        errors.append("pre-commit must run the generated direct C-import test")
    if "scripts/ci/check_fixture_policy.py" not in pre_commit:
        errors.append("pre-commit must validate the fixture and example policy")
    if "scripts/ci/check_capability_manifest.py" not in pre_commit:
        errors.append("pre-commit must validate bootstrap capability claims")
    if "scripts/ci/check_typed_boundaries.py" not in pre_commit:
        errors.append("pre-commit must reject unreviewed untyped Haxe boundaries")
    if "scripts/test/snapshots.py" not in pre_commit:
        errors.append("pre-commit must check registered snapshot ownership and drift")
    if "npm run test:beads-plan" not in pre_commit:
        errors.append("pre-commit must validate the reproducible Beads graph")
    if "npm run test:governance-policy" not in pre_commit:
        errors.append(
            "pre-commit must validate contribution, disclosure, and release policy"
        )

    pre_push = read_text(PRE_PUSH, errors)
    if "scripts/security/run-gitleaks.sh\"" not in pre_push or "--staged" in pre_push:
        errors.append("pre-push must scan every reachable Git revision for secrets")
    if "scripts/ci/check_security_tooling.py" not in pre_push:
        errors.append("pre-push must validate security-tool and workflow pins")

    runner = read_text(ROOT / "scripts/ci/runtime_smoke.py", errors)
    for required_flag in ("-std=c11", "-std=c++17", "-Werror", "-pedantic"):
        if required_flag not in runner:
            errors.append(f"native smoke runner lost strict flag {required_flag}")
    if "required toolchain" not in runner or "SKIP" not in runner:
        errors.append("native smoke runner must distinguish required failures from optional skips")
    for required_c_ast_lane in (
        "C_AST_RUNNER",
        "structural-c-ast-golden-run",
        "expression-precedence-golden-run",
        "c-ast-attribute-run",
        "c-ast-header-source-run",
    ):
        if required_c_ast_lane not in runner:
            errors.append(
                "native smoke runner lost reusable C AST lane: "
                + required_c_ast_lane
            )
    for required_import_lane in ("C_IMPORT", "generated-direct-c-import-run"):
        if required_import_lane not in runner:
            errors.append(
                "native smoke runner lost generated direct C-import lane: "
                + required_import_lane
            )
    if "declaration-header-independent-compile" not in runner or "declaration-plan-header-run" not in runner:
        errors.append("native smoke runner must independently compile and execute declaration-plan headers")
    if "project-emitter-header-independent-compile" not in runner or "project-emitter-structural-run" not in runner:
        errors.append("native smoke runner must independently compile and execute project-emitter output")
    if "runtime-feature-selective-packaging" not in runner or "RUNTIME_FEATURE_GRAPH" not in runner:
        errors.append("native smoke runner must execute selective runtime packaging in each toolchain lane")
    if "array-runtime-contract" not in runner or "ARRAY_RUNTIME" not in runner:
        errors.append("native smoke runner must execute the typed array contract in each toolchain lane")
    if "string-runtime-contract" not in runner or "STRING_RUNTIME" not in runner:
        errors.append("native smoke runner must execute the UTF-8 scalar string contract in each toolchain lane")
    if "generated-hello-example-run" not in runner or "HELLO_EXAMPLE" not in runner:
        errors.append("native smoke runner must execute the generated hello example in each toolchain lane")
    if "primitive-differential-sanitizer-run" not in runner or "PRIMITIVE_DIFFERENTIAL" not in runner:
        errors.append("native smoke runner must execute the seeded primitive sanitizer corpus in each toolchain lane")
    if '"--native-only"' not in runner:
        errors.append("native smoke must consume checked-in runtime plans without requiring Haxe")

    c_ast_runner = read_text(ROOT / "test/c_ast/run.py", errors)
    for required_c_ast_contract in (
        "ASTFixtureCompiler.run",
        "run_c_fixture_corpus",
        "validate_report",
        "REQUIRED_COVERAGE",
        "--native-only",
        "--report",
    ):
        if required_c_ast_contract not in c_ast_runner:
            errors.append(
                "C AST fixture compiler lost reproducibility contract: "
                + required_c_ast_contract
            )

    c_fixture_harness = read_text(ROOT / "scripts/test/c_fixture_harness.py", errors)
    for required_harness_contract in (
        "C11_STRICT_FLAGS",
        "compiler_identity",
        "arguments",
        "versionLine",
        "sha256",
        "stdout",
        "stderr",
    ):
        if required_harness_contract not in c_fixture_harness:
            errors.append(
                "reusable C fixture harness lost recorded field: "
                + required_harness_contract
            )

    build_adapter_runner = read_text(ROOT / "test/project_emitter/run.py", errors)
    for required_build_adapter_contract in (
        "--build-adapters",
        "compiler_identity",
        "direct_manifest_build",
        "cmake_build",
        "meson_build",
        "BUILD_ADAPTER_CASES",
    ):
        if required_build_adapter_contract not in build_adapter_runner:
            errors.append(
                "project-emitter runner lost required build-adapter contract "
                + required_build_adapter_contract
            )

    runtime_feature_runner = read_text(
        ROOT / "test/runtime/runtime-feature-graph/run.py", errors
    )
    for required_runtime_feature_contract in (
        "-std=c11",
        "-Werror",
        "-pedantic",
        "-Wconversion",
        "-Wsign-conversion",
        "--toolchain",
        "--native-only",
        "compiler_identity",
        "render_reports",
        "sourceSetSha256",
        "hxc_string_",
        "nm",
    ):
        if required_runtime_feature_contract not in runtime_feature_runner:
            errors.append(
                "runtime feature runner lost selective native evidence: "
                + required_runtime_feature_contract
            )

    string_runtime_runner = read_text(
        ROOT / "test/differential/string-runtime/run.py", errors
    )
    for required_string_runtime_contract in (
        "-std=c11",
        "-Werror",
        "-pedantic",
        "-Wconversion",
        "-Wsign-conversion",
        "--toolchain",
        "--native-only",
        "run_oracle",
        "-fsanitize=address,undefined",
        "hxc_string_concat",
        "hxc_gc",
        "nm",
    ):
        if required_string_runtime_contract not in string_runtime_runner:
            errors.append(
                "string runtime runner lost UTF-8/allocator/selective-link evidence: "
                + required_string_runtime_contract
            )

    array_runtime_runner = read_text(
        ROOT / "test/differential/array-runtime/run.py", errors
    )
    for required_array_runtime_contract in (
        "-std=c11",
        "-Werror",
        "-pedantic",
        "-Wconversion",
        "-Wsign-conversion",
        "--toolchain",
        "--native-only",
        "run_oracle",
        "-fsanitize=address,undefined",
        "hxc_array_insert_copy",
        "hxc_gc",
        "nm",
    ):
        if required_array_runtime_contract not in array_runtime_runner:
            errors.append(
                "array runtime runner lost growth/alias/lifecycle/selective-link evidence: "
                + required_array_runtime_contract
            )

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

    aggregate_runner = read_text(ROOT / "test/aggregate_lowering/run.py", errors)
    for required_aggregate_contract in (
        "C11_STRICT_FLAGS",
        "-std=c++17",
        "-pedantic",
        "-Werror",
        "-O0",
        "-O2",
        "run_c_fixture_corpus",
        "layout_consumer.cpp",
        "_Static_assert(",
    ):
        if required_aggregate_contract not in aggregate_runner:
            errors.append(
                "aggregate-lowering runner lost strict native/layout contract "
                + required_aggregate_contract
            )
    if "--native-only" not in aggregate_runner or "--toolchain" not in aggregate_runner:
        errors.append(
            "aggregate-lowering runner must expose the required native matrix seam"
        )
    if "compiler_identity" not in aggregate_runner or '"--version"' not in aggregate_runner:
        errors.append(
            "aggregate-lowering runner must verify required compiler-family identity"
        )

    class_runner = read_text(ROOT / "test/class_layout/run.py", errors)
    for required_class_contract in (
        "C11_STRICT_FLAGS",
        "-std=c++17",
        "-pedantic",
        "-Werror",
        "-O0",
        "-O2",
        "run_c_fixture_corpus",
        "layout_consumer.cpp",
        "_Static_assert(",
        "null-preserving-upcast",
        "analyzed-no-public-exports",
    ):
        if required_class_contract not in class_runner:
            errors.append(
                "class-layout runner lost strict semantic/native contract "
                + required_class_contract
            )
    if "--native-only" not in class_runner or "--toolchain" not in class_runner:
        errors.append("class-layout runner must expose the required native matrix seam")
    if "compiler_identity" not in class_runner or '"--version"' not in class_runner:
        errors.append(
            "class-layout runner must verify required compiler-family identity"
        )

    constructor_runner = read_text(ROOT / "test/constructor_lowering/run.py", errors)
    for required_constructor_contract in (
        "C11_STRICT_FLAGS",
        "-std=c++17",
        "-pedantic",
        "-Werror",
        "-O0",
        "-O2",
        "run_c_fixture_corpus",
        "constructor_header_cpp.cpp",
        "bounded-stack-construction",
        "failure=status(exception)",
        "reflaxe_c_constructor_lowering_report",
    ):
        if required_constructor_contract not in constructor_runner:
            errors.append(
                "constructor-lowering runner lost strict semantic/native contract "
                + required_constructor_contract
            )
    if "--native-only" not in constructor_runner or "--toolchain" not in constructor_runner:
        errors.append(
            "constructor-lowering runner must expose the required native matrix seam"
        )
    if "resolve_toolchains" not in constructor_runner:
        errors.append(
            "constructor-lowering runner must verify required compiler-family identity"
        )

    virtual_dispatch_runner = read_text(ROOT / "test/virtual_dispatch/run.py", errors)
    for required_virtual_dispatch_contract in (
        "C11_STRICT_FLAGS",
        "-std=c++17",
        "-pedantic",
        "-Werror",
        'for optimization in ("-O0", "-O2")',
        "run_c_fixture_corpus",
        "dispatch_header_cpp.cpp",
        "hxc.dispatch.json",
        "dispatch-report",
        "reflaxe_c_virtual_dispatch_report",
        "virtual-override-representation-mismatch",
        "one-root-layout-reachable-virtual-slots-only",
        "warm server after rejected override requests",
    ):
        if required_virtual_dispatch_contract not in virtual_dispatch_runner:
            errors.append(
                "virtual-dispatch runner lost strict semantic/native contract "
                + required_virtual_dispatch_contract
            )
    if (
        "--native-only" not in virtual_dispatch_runner
        or "--toolchain" not in virtual_dispatch_runner
    ):
        errors.append(
            "virtual-dispatch runner must expose the required native matrix seam"
        )
    if "resolve_toolchains" not in virtual_dispatch_runner:
        errors.append(
            "virtual-dispatch runner must verify required compiler-family identity"
        )

    enum_runner = read_text(ROOT / "test/enum_lowering/run.py", errors)
    for required_enum_contract in (
        "C11_STRICT_FLAGS",
        "-std=c++17",
        "-pedantic",
        "-Werror",
        "-O0",
        "-O2",
        "run_c_fixture_corpus",
        "layout_consumer.cpp",
        "_Static_assert(",
        "checked-abort",
        "recursive-enum-requires-escape-analysis",
    ):
        if required_enum_contract not in enum_runner:
            errors.append(
                "enum-lowering runner lost strict semantic/native contract "
                + required_enum_contract
            )
    if "--native-only" not in enum_runner or "--toolchain" not in enum_runner:
        errors.append(
            "enum-lowering runner must expose the required native matrix seam"
        )
    if "compiler_identity" not in enum_runner or '"--version"' not in enum_runner:
        errors.append(
            "enum-lowering runner must verify required compiler-family identity"
        )

    generic_runner = read_text(ROOT / "test/generic_specialization/run.py", errors)
    for required_generic_contract in (
        "STRICT_FLAGS",
        "-std=c11",
        "-pedantic-errors",
        "-Werror",
        'for optimization in ("O0", "O2")',
        "hxc.specializations.json",
        "generic-specialization-budget:64",
        "generic-specialization-code-size-budget",
        "hashlib.sha256(emitted_definition)",
        "check_server_isolation",
        "check_conditional_sidecar_ownership",
    ):
        if required_generic_contract not in generic_runner:
            errors.append(
                "generic-specialization runner lost bounded semantic/native contract "
                + required_generic_contract
            )
    if "--native-only" not in generic_runner or "--toolchain" not in generic_runner:
        errors.append(
            "generic-specialization runner must expose the required native matrix seam"
        )
    if "compiler_family" not in generic_runner or '"--version"' not in generic_runner:
        errors.append(
            "generic-specialization runner must verify required compiler-family identity"
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

    static_initialization_runner = read_text(
        ROOT / "test/static_initialization/run.py", errors
    )
    for required_static_initialization_flag in (
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
        if required_static_initialization_flag not in static_initialization_runner:
            errors.append(
                "static-initialization native runner lost strict flag "
                + required_static_initialization_flag
            )
    if (
        "--native-only" not in static_initialization_runner
        or "--toolchain" not in static_initialization_runner
    ):
        errors.append(
            "static-initialization runner must expose the required native matrix seam"
        )
    if (
        "compiler_identity" not in static_initialization_runner
        or '"--version"' not in static_initialization_runner
    ):
        errors.append(
            "static-initialization runner must verify required compiler-family identity"
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

    primitive_differential_runner = read_text(
        ROOT / "test/primitive_differential/run.py", errors
    )
    for required_primitive_differential_contract in (
        "SplitMix64",
        "splitmix64-v1",
        "host-dependent-int32",
        "minimize_first_mismatch",
        "primitive-divergences.json",
        "run_c_fixture_corpus",
        "-fsanitize=address,undefined",
        "-O0",
        "-O2",
        "--native-only",
        "--toolchain",
    ):
        if required_primitive_differential_contract not in primitive_differential_runner:
            errors.append(
                "primitive differential runner lost reproducibility/sanitizer contract: "
                + required_primitive_differential_contract
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

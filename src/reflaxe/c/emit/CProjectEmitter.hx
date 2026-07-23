package reflaxe.c.emit;

import haxe.Json;
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;
import reflaxe.c.emit.CProjectLayout;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.emit.CBuildPlan.CBuildPlanBuilder;
import reflaxe.c.emit.CBuildPlan.CBuildPlanSnapshot;
import reflaxe.c.emit.CBuildPlan.CBuildStandard;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.lowering.CDispatchReport.CDispatchReportBuilder;
import reflaxe.c.lowering.CDispatchReport.CDispatchReportSnapshot;
import reflaxe.c.lowering.CGenericSpecializationReport.CGenericSpecializationReportSnapshot;
import reflaxe.c.lowering.CGenericSpecializationContract;
import reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationSnapshot;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationStrategy;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationPhase;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanSnapshot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanStatus;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningPurpose;
import reflaxe.c.runtime.RuntimeNoRuntimeEligibilityAnalyzer;
import reflaxe.c.runtime.RuntimeFeaturePlanner;

/** Input status distinguishes structural fixtures, the admitted primitive slice, and unproven broader programs. */
enum abstract CProjectCompilationStatus(String) to String {
	var StructuralFixture = "structural-fixture-no-haxe-lowering";
	var PrimitiveExecutable = "lowered-primitive-executable";
	var DirectValueExecutable = "lowered-direct-value-executable";
	var LoweredProgram = "lowered-program";
}

typedef CProjectEnvironment = CEnvironment;
typedef CProjectStandard = CBuildStandard;
typedef CProjectRuntimePolicy = CRuntimePolicy;
typedef CProjectRuntimeDiagnostics = CRuntimeDiagnostics;

/** Complete logical input to deterministic project packaging; it contains no output root. */
typedef CProjectEmissionPlan = {
	final schemaVersion:Int;
	final projectName:String;
	final compilationStatus:CProjectCompilationStatus;
	final profile:CProfile;
	final environment:CProjectEnvironment;
	final cStandard:CProjectStandard;
	final projectLayout:CProjectLayout;
	final runtimePolicy:CProjectRuntimePolicy;
	final runtimeDiagnostics:CProjectRuntimeDiagnostics;
	final ?runtimePolicyProvenance:String;
	final ?runtimeDiagnosticsProvenance:String;
	final units:Array<GeneratedFile>;
	final buildFacts:Array<TypedCBuildFact>;
	final ?primitiveHelperIds:Array<String>;
	final ?directAggregateCount:Int;
	final ?directEnumCount:Int;
	final ?directClassCount:Int;
	final ?directInterfaceCount:Int;
	final ?directConstructorCount:Int;
	final ?directGenericFunctionCount:Int;
	final ?directGenericTypeCount:Int;
	final ?directImportCount:Int;
	final ?directImportTypeCount:Int;
	final ?directInstanceCallCount:Int;
	final ?indirectInstanceCallCount:Int;
	final ?dispatchReport:CDispatchReportSnapshot;
	final ?specializationReport:CGenericSpecializationReportSnapshot;
	final ?stdlibModules:Array<String>;
	final ?stdlibCapabilities:Array<String>;
	final ?staticInitialization:CStaticInitializationSnapshot;
	final ?runtimePlan:RuntimeFeaturePlanSnapshot;
	final symbolTable:CSymbolTableSnapshot;
}

private enum abstract PlaceholderStatus(String) to String {
	var RuntimeNotAnalyzed = "placeholder-no-runtime-analysis";
	var AbiNotAnalyzed = "placeholder-no-export-analysis";
	var StdlibNotAnalyzed = "placeholder-no-stdlib-analysis";
}

private enum abstract ResolvedAnalysisStatus(String) to String {
	var NoExports = "analyzed-no-public-exports";
	var NoStdlib = "analyzed-no-stdlib-use";
	var SelectedStdlib = "analyzed-selected-stdlib-use";
}

private enum abstract AbiStability(String) to String {
	var Experimental = "experimental";
}

private typedef AddressedArtifact = {
	final path:String;
	final kind:GeneratedFileKind;
	final sha256:String;
}

private typedef ProjectConfigurationRecord = {
	final profile:CProfile;
	final environment:CProjectEnvironment;
	final cStandard:CProjectStandard;
	final projectLayout:CProjectLayout;
	final runtimePolicy:CProjectRuntimePolicy;
	final runtimeDiagnostics:CProjectRuntimeDiagnostics;
}

private typedef ContentAddressingRecord = {
	final algorithm:String;
	final scope:String;
}

private typedef CompilerManifestRecord = {
	final schemaVersion:Int;
	final generator:String;
	final projectName:String;
	final compilationStatus:CProjectCompilationStatus;
	final configuration:ProjectConfigurationRecord;
	final contentAddressing:ContentAddressingRecord;
	final artifacts:Array<AddressedArtifact>;
	final build:CBuildPlanSnapshot;
	final ownershipManifest:String;
}

private typedef RuntimePlanPlaceholder = {
	final schemaVersion:Int;
	final status:PlaceholderStatus;
	final requestedPolicy:CProjectRuntimePolicy;
	final diagnosticMode:CProjectRuntimeDiagnostics;
	final environment:CProjectEnvironment;
	final rootReasons:Array<String>;
	final features:Array<String>;
	final artifacts:Array<String>;
	final symbols:Array<String>;
	final libraries:Array<String>;
	final defines:Array<String>;
	final noRuntimeProof:Null<String>;
}

private typedef AbiPlaceholder = {
	final schemaVersion:Int;
	final status:PlaceholderStatus;
	final stability:AbiStability;
	final profile:CProfile;
	final exports:Array<String>;
	final types:Array<String>;
}

private typedef AbiResolved = {
	final schemaVersion:Int;
	final status:ResolvedAnalysisStatus;
	final stability:AbiStability;
	final profile:CProfile;
	final exports:Array<String>;
	final types:Array<String>;
	final executableEntryPoint:String;
}

private typedef StdlibPlaceholder = {
	final schemaVersion:Int;
	final status:PlaceholderStatus;
	final profile:CProfile;
	final modules:Array<String>;
	final capabilities:Array<String>;
}

private typedef StdlibResolved = {
	final schemaVersion:Int;
	final status:ResolvedAnalysisStatus;
	final profile:CProfile;
	final modules:Array<String>;
	final capabilities:Array<String>;
}

/** Pure schema-1 project emitter. Filesystem ownership is handled separately. */
class CProjectEmitter {
	public static inline final SCHEMA_VERSION = 1;

	static final SIDECAR_PATHS = [
		"hxc.abi.json",
		"hxc.dispatch.json",
		"hxc.initialization-plan.json",
		"hxc.manifest.json",
		"hxc.runtime-plan.json",
		"hxc.specializations.json",
		"hxc.stdlib-report.json",
		"hxc.symbols.json",
		CBuildAdapterEmitter.CMAKE_PATH,
		CBuildAdapterEmitter.MESON_PATH
	];

	public function new() {}

	public function emit(plan:CProjectEmissionPlan):Array<GeneratedFile> {
		validatePlan(plan);
		final units = canonicalUnits(plan.units);
		final buildPlan = new CBuildPlanBuilder().build(plan.projectName, plan.cStandard, units, plan.buildFacts);
		final files = units.copy();

		files.push(jsonFile("hxc.symbols.json", GeneratedFileKind.SymbolTable, plan.symbolTable));
		switch plan.compilationStatus {
			case StructuralFixture:
				files.push(jsonFile("hxc.runtime-plan.json", GeneratedFileKind.RuntimePlan, runtimePlanPlaceholder(plan)));
				files.push(jsonFile("hxc.abi.json", GeneratedFileKind.AbiManifest, abiPlaceholder(plan)));
				files.push(jsonFile("hxc.stdlib-report.json", GeneratedFileKind.StdlibReport, stdlibPlaceholder(plan)));
			case PrimitiveExecutable | DirectValueExecutable:
				files.push(jsonFile("hxc.initialization-plan.json", GeneratedFileKind.InitializationPlan, requireStaticInitialization(plan)));
				files.push(jsonFile("hxc.runtime-plan.json", GeneratedFileKind.RuntimePlan, runtimePlanResolved(plan)));
				files.push(jsonFile("hxc.abi.json", GeneratedFileKind.AbiManifest, abiResolved(plan)));
				files.push(jsonFile("hxc.stdlib-report.json", GeneratedFileKind.StdlibReport, stdlibResolved(plan)));
				if (plan.specializationReport != null)
					files.push(jsonFile("hxc.specializations.json", GeneratedFileKind.SpecializationReport, plan.specializationReport));
				if (plan.dispatchReport != null)
					files.push(jsonFile("hxc.dispatch.json", GeneratedFileKind.DispatchReport, plan.dispatchReport));
			case LoweredProgram:
				throw new ProjectEmissionError("unreachable generic lowered-program plan passed validation");
			case _:
				throw new ProjectEmissionError('unreachable project compilation status `${Std.string(plan.compilationStatus)}`');
		}
		for (adapter in new CBuildAdapterEmitter().emit(plan.projectName, buildPlan)) {
			files.push(adapter);
		}

		files.sort(compareFiles);
		final addressedArtifacts:Array<AddressedArtifact> = files.map(file -> {
			path: file.relativePath,
			kind: file.kind,
			sha256: file.contentSha256
		});
		final manifest:CompilerManifestRecord = {
			schemaVersion: SCHEMA_VERSION,
			generator: "reflaxe.c",
			projectName: plan.projectName,
			compilationStatus: plan.compilationStatus,
			configuration: {
				profile: plan.profile,
				environment: plan.environment,
				cStandard: plan.cStandard,
				projectLayout: plan.projectLayout,
				runtimePolicy: plan.runtimePolicy,
				runtimeDiagnostics: plan.runtimeDiagnostics
			},
			contentAddressing: {
				algorithm: "sha256",
				scope: "all compiler artifacts except hxc.manifest.json and _GeneratedFiles.json"
			},
			artifacts: addressedArtifacts,
			build: buildPlan,
			ownershipManifest: GeneratedFile.OWNERSHIP_MANIFEST
		};
		files.push(jsonFile("hxc.manifest.json", GeneratedFileKind.CompilerManifest, manifest));
		files.sort(compareFiles);
		return files;
	}

	function validatePlan(plan:CProjectEmissionPlan):Void {
		if (plan.schemaVersion != SCHEMA_VERSION) {
			fail('project emission schema `${plan.schemaVersion}` is unsupported; expected `$SCHEMA_VERSION`');
		}
		validateLogicalText(plan.projectName, "project name");
		if (plan.projectName.indexOf("/") != -1 || plan.projectName.indexOf("\\") != -1) {
			fail('project name must be a logical identity rather than a filesystem path: `${plan.projectName}`');
		}
		switch plan.compilationStatus {
			case StructuralFixture:
			case PrimitiveExecutable | DirectValueExecutable:
				validateDirectExecutablePlan(plan);
			case LoweredProgram:
				fail("lowered-program project emission remains unavailable until semantic runtime and ABI analyses replace the honest placeholders");
			case _:
				fail('unknown project compilation status `${Std.string(plan.compilationStatus)}`');
		}
		switch plan.profile {
			case CProfile.Portable | CProfile.Metal:
			case _:
				fail('unknown project profile `${Std.string(plan.profile)}`');
		}
		switch plan.environment {
			case CProjectEnvironment.Hosted | CProjectEnvironment.Freestanding | CProjectEnvironment.Wasi | CProjectEnvironment.Emscripten:
			case _:
				fail('unknown project environment `${Std.string(plan.environment)}`');
		}
		switch plan.cStandard {
			case CProjectStandard.C11 | CProjectStandard.C17 | CProjectStandard.C23Experimental:
			case _:
				fail('unknown project C standard `${Std.string(plan.cStandard)}`');
		}
		switch plan.projectLayout {
			case CProjectLayout.Split | CProjectLayout.Package | CProjectLayout.Unity:
			case _:
				fail('unknown project layout `${Std.string(plan.projectLayout)}`');
		}
		switch plan.runtimePolicy {
			case CProjectRuntimePolicy.Auto | CProjectRuntimePolicy.Minimal | CProjectRuntimePolicy.None:
			case _:
				fail('unknown project runtime policy `${Std.string(plan.runtimePolicy)}`');
		}
		switch plan.runtimeDiagnostics {
			case CProjectRuntimeDiagnostics.Off | CProjectRuntimeDiagnostics.Summary | CProjectRuntimeDiagnostics.Warn:
			case _:
				fail('unknown project runtime diagnostic mode `${Std.string(plan.runtimeDiagnostics)}`');
		}
		if (plan.symbolTable.schemaVersion != CSymbolRegistry.SCHEMA_VERSION || plan.symbolTable.algorithm != CSymbolRegistry.ALGORITHM) {
			fail('project emission requires the finalized schema-${CSymbolRegistry.SCHEMA_VERSION} ${CSymbolRegistry.ALGORITHM} symbol table');
		}
		final helperIds = plan.primitiveHelperIds;
		if (helperIds != null) {
			final seen:Map<String, Bool> = [];
			for (helperId in helperIds) {
				validateLogicalText(helperId, "primitive helper ID");
				if (seen.exists(helperId)) {
					fail('primitive helper ID `${helperId}` is duplicated');
				}
				seen.set(helperId, true);
			}
		}
	}

	function validateDirectExecutablePlan(plan:CProjectEmissionPlan):Void {
		final aggregateCount = plan.directAggregateCount == null ? 0 : plan.directAggregateCount;
		final enumCount = plan.directEnumCount == null ? 0 : plan.directEnumCount;
		final classCount = plan.directClassCount == null ? 0 : plan.directClassCount;
		final interfaceCount = plan.directInterfaceCount == null ? 0 : plan.directInterfaceCount;
		final constructorCount = plan.directConstructorCount == null ? 0 : plan.directConstructorCount;
		final genericFunctionCount = plan.directGenericFunctionCount == null ? 0 : plan.directGenericFunctionCount;
		final genericTypeCount = plan.directGenericTypeCount == null ? 0 : plan.directGenericTypeCount;
		final importCount = plan.directImportCount == null ? 0 : plan.directImportCount;
		final importTypeCount = plan.directImportTypeCount == null ? 0 : plan.directImportTypeCount;
		final directValueCount = aggregateCount + enumCount + classCount + interfaceCount + genericFunctionCount + importCount;
		if (aggregateCount < 0
			|| enumCount < 0
			|| classCount < 0
			|| interfaceCount < 0
			|| constructorCount < 0
			|| constructorCount > 0
			&& classCount == 0
			|| genericFunctionCount < 0
			|| genericTypeCount < 0
			|| importCount < 0
			|| importTypeCount < 0
			|| importTypeCount > importCount
			|| genericTypeCount > enumCount
			|| plan.compilationStatus == DirectValueExecutable
			&& directValueCount == 0
			|| plan.compilationStatus == PrimitiveExecutable
			&& directValueCount != 0) {
			fail("direct value and generic-specialization counts must agree with the bounded executable compilation status");
		}
		validateSpecializationReport(plan, genericFunctionCount, genericTypeCount);
		validateDispatchReport(plan);
		if (plan.runtimePolicyProvenance == null || plan.runtimeDiagnosticsProvenance == null) {
			fail("direct executable emission requires resolved runtime-policy provenance");
		}
		if (plan.environment != CProjectEnvironment.Hosted) {
			fail('direct executable emission requires the hosted environment; found `${plan.environment}`');
		}
		final initialization = requireStaticInitialization(plan);
		final runtimePlan = requireRuntimePlan(plan);
		validateDirectRuntimePlan(plan, runtimePlan);
		if (initialization.schemaVersion != 1
			|| initialization.strategy != CStaticInitializationStrategy.EagerHaxeTypeOrder
			|| initialization.entryFunctionId == ""
			|| initialization.phaseOrder.join(",") != [
				CStaticInitializationPhase.ClassInitializers,
				CStaticInitializationPhase.StaticFieldInitializers,
				CStaticInitializationPhase.Entry
			].join(",")
			|| initialization.runtimeFeatures.length != 0) {
			fail("direct executable emission requires a valid schema-1 static-initialization plan with no independent initializer runtime roots");
		}
		if (initialization.executionOrder.length != initialization.initializers.length) {
			fail("static-initialization execution order must address every initializer exactly once");
		}
		validateLogicalText(initialization.entryFunctionId, "static-initialization entry function ID");
		validateLogicalText(initialization.cyclePolicy, "static-initialization cycle policy");
		validateLogicalText(initialization.oncePolicy, "static-initialization once policy");
		final typeIds:Map<String, Bool> = [];
		for (typeId in initialization.typeOrder) {
			validateLogicalText(typeId, "static-initialization type ID");
			if (typeIds.exists(typeId)) {
				fail('static-initialization type order repeats `$typeId`');
			}
			typeIds.set(typeId, true);
		}
		for (dependency in initialization.dependencies) {
			validateLogicalText(dependency.dependentTypeId, "static-initialization dependent type ID");
			validateLogicalText(dependency.prerequisiteTypeId, "static-initialization prerequisite type ID");
			validateLogicalText(dependency.reason, "static-initialization dependency reason");
			if (!typeIds.exists(dependency.dependentTypeId) || !typeIds.exists(dependency.prerequisiteTypeId)) {
				fail('static-initialization dependency `${dependency.dependentTypeId}` -> `${dependency.prerequisiteTypeId}` escapes type order');
			}
			validateInitializationSource(dependency.source, 'dependency `${dependency.dependentTypeId}` -> `${dependency.prerequisiteTypeId}`');
		}
		final initializerIds:Map<String, Bool> = [];
		for (index in 0...initialization.initializers.length) {
			final initializer = initialization.initializers[index];
			validateLogicalText(initializer.id, "static initializer ID");
			if (initializerIds.exists(initializer.id) || initialization.executionOrder[index] != initializer.id) {
				fail('static initializer `${initializer.id}` is duplicated or differs from its execution-order slot');
			}
			initializerIds.set(initializer.id, true);
			if (initializer.functionId != initializer.id || !typeIds.exists(initializer.typeId)) {
				fail('static initializer `${initializer.id}` lost its function or type-order identity');
			}
			validateInitializationSource(initializer.source, 'initializer `${initializer.id}`');
		}
		for (fact in plan.buildFacts) {
			if (fact.ownerModulePaths.length == 0)
				fail('direct executable build fact `${fact.kind}` `${fact.name}` has no source owner');
			switch fact.kind {
				case "include":
					if (importCount == 0 || fact.valueKind != "enum" || (fact.value != "system" && fact.value != "local"))
						fail('direct executable header fact `${fact.name}` is not a validated reached C import');
				case "link":
					if (fact.value != null || fact.valueKind != null || (importCount == 0 && fact.name != "m"))
						fail('direct executable link fact `${fact.name}` is neither the C math library nor a reached C import');
				case "pkg-config" | "framework":
					if (importCount == 0 || fact.value != null || fact.valueKind != null)
						fail('direct executable build fact `${fact.kind}` `${fact.name}` is not a validated reached C import');
				case _:
					fail('direct executable emission does not admit build fact `${fact.kind}` `${fact.name}`');
			}
		}
		var sources = 0;
		var privateHeaders = 0;
		for (unit in plan.units) {
			switch unit.kind {
				case Source:
					sources++;
				case PrivateHeader:
					privateHeaders++;
				case RuntimeHeader | RuntimeSource:
				case PublicHeader:
					fail('direct executable emission cannot package `${Std.string(unit.kind)}` `${unit.relativePath}`', [unit.relativePath]);
				case _:
					fail('direct executable payload has invalid kind `${Std.string(unit.kind)}`', [unit.relativePath]);
			}
		}
		if (sources < 1) {
			fail("direct executable emission requires at least one generated C source");
		}
		switch plan.projectLayout {
			case CProjectLayout.Unity:
				if (privateHeaders != 1)
					fail('unity direct executable emission requires exactly one private prototype header; found $privateHeaders');
			case CProjectLayout.Split:
				if (privateHeaders < 3)
					fail('split direct executable emission requires an umbrella, representation detail, and at least one module header; found $privateHeaders');
			case CProjectLayout.Package:
				if (privateHeaders < 3)
					fail('package direct executable emission requires an umbrella, representation detail, and at least one package header; found $privateHeaders');
		}
		var entryPoints = 0;
		for (symbol in plan.symbolTable.symbols) {
			if (symbol.cName == "main" && symbol.requestedName == "main") {
				entryPoints++;
			}
			if (StringTools.startsWith(symbol.cName, "hxrt_")) {
				fail('direct executable symbol table contains runtime symbol `${symbol.cName}`');
			}
		}
		if (entryPoints != 1) {
			fail('direct executable symbol table requires exactly one compiler-owned exact `main`; found $entryPoints');
		}
	}

	function validateDirectRuntimePlan(plan:CProjectEmissionPlan, runtimePlan:RuntimeFeaturePlanSnapshot):Void {
		final aggregateCount = plan.directAggregateCount == null ? 0 : plan.directAggregateCount;
		final enumCount = plan.directEnumCount == null ? 0 : plan.directEnumCount;
		final classCount = plan.directClassCount == null ? 0 : plan.directClassCount;
		final interfaceCount = plan.directInterfaceCount == null ? 0 : plan.directInterfaceCount;
		final constructorCount = plan.directConstructorCount == null ? 0 : plan.directConstructorCount;
		final genericFunctionCount = plan.directGenericFunctionCount == null ? 0 : plan.directGenericFunctionCount;
		final genericTypeCount = plan.directGenericTypeCount == null ? 0 : plan.directGenericTypeCount;
		final importCount = plan.directImportCount == null ? 0 : plan.directImportCount;
		final importTypeCount = plan.directImportTypeCount == null ? 0 : plan.directImportTypeCount;
		final indirectInstanceCallCount = plan.indirectInstanceCallCount == null ? 0 : plan.indirectInstanceCallCount;
		if (runtimePlan.schemaVersion != RuntimeFeaturePlanner.PLAN_SCHEMA_VERSION
			|| runtimePlan.algorithm != RuntimeFeaturePlanner.PLAN_ALGORITHM
			|| runtimePlan.planPurpose != RuntimePlanningPurpose.CompilerProgram) {
			fail("direct executable emission requires a compiler-program hxc-runtime-plan-v2 analysis");
		}
		if (runtimePlan.profile != plan.profile
			|| runtimePlan.environment != plan.environment
			|| runtimePlan.requestedPolicy != plan.runtimePolicy
			|| runtimePlan.resolvedPolicy != plan.runtimePolicy
			|| runtimePlan.diagnosticMode != plan.runtimeDiagnostics
			|| runtimePlan.policyProvenance != plan.runtimePolicyProvenance
			|| runtimePlan.diagnosticProvenance != plan.runtimeDiagnosticsProvenance) {
			fail("direct executable runtime analysis differs from resolved project configuration");
		}
		switch runtimePlan.status {
			case RuntimeFeaturePlanStatus.RuntimeFree:
				if (runtimePlan.rootReasons.length != 0
					|| runtimePlan.manualOverrides.length != 0
					|| runtimePlan.dependencyEdges.length != 0
					|| runtimePlan.selectedFeatures.length != 0
					|| runtimePlan.features.length != 0
					|| runtimePlan.artifactDetails.length != 0
					|| runtimePlan.artifacts.length != 0
					|| runtimePlan.symbols.length != 0
					|| runtimePlan.libraries.length != 0
					|| runtimePlan.defines.length != 0) {
					fail("runtime-free direct analysis must prove complete hxrt absence");
				}
				switch runtimePlan.noRuntimeProof {
					case null:
						fail("runtime-free direct analysis must prove complete hxrt absence");
					case proof:
						RuntimeNoRuntimeEligibilityAnalyzer.validateProof(proof, runtimePlan.planPurpose, runtimePlan.directDecisions,
							plan.primitiveHelperIds == null ? [] : plan.primitiveHelperIds);
						if (proof.reachability.typeInstances != aggregateCount + enumCount + classCount + interfaceCount + importTypeCount) {
							fail("runtime-free direct executable value-layout count differs from reachable HxcIR instances");
						}
				}
			case RuntimeFeaturePlanStatus.RuntimeFeatures:
				validateCompilerRuntimePlan(runtimePlan);
			case RuntimeFeaturePlanStatus.NativeSeedFeatures:
				fail("generated Haxe cannot use a native-seed runtime plan");
			case _:
				fail('direct executable runtime analysis has unknown status `${Std.string(runtimePlan.status)}`');
		}
		validateRuntimePayload(plan.units, runtimePlan);
		final helperIds = plan.primitiveHelperIds == null ? [] : plan.primitiveHelperIds;
		final expectedDirectDecisions = [
			"primitive-values",
			"ub-safe-primitive-operations",
			"primitive-static-storage",
			"static-functions",
			"direct-calls",
			"explicit-evaluation-order",
			"executable-entry-point"
		];
		if (aggregateCount > 0) {
			expectedDirectDecisions.push("closed-anonymous-value-records");
		}
		if (enumCount > 0) {
			expectedDirectDecisions.push("bounded-haxe-enum-values");
		}
		if (classCount > 0) {
			expectedDirectDecisions.push("concrete-class-reference-layouts");
		}
		if (constructorCount > 0) {
			expectedDirectDecisions.push("bounded-stack-construction");
		}
		if (genericFunctionCount + genericTypeCount > 0) {
			expectedDirectDecisions.push("closed-generic-specializations");
		}
		var virtualCallCount = 0;
		var interfaceCallCount = 0;
		if (plan.dispatchReport != null)
			for (call in plan.dispatchReport.calls)
				switch call.dispatch {
					case "virtual":
						virtualCallCount++;
					case "interface":
						interfaceCallCount++;
					case _:
				}
		if (virtualCallCount > 0) {
			expectedDirectDecisions.push("reachable-program-local-virtual-dispatch");
		}
		if (interfaceCallCount > 0)
			expectedDirectDecisions.push("reachable-program-local-interface-dispatch");
		if (importCount > 0) {
			expectedDirectDecisions.push("typed-header-owned-c-imports");
		}
		if (helperIds.length > 0) {
			expectedDirectDecisions.push("selected-program-local-helpers");
		}
		final initialization = requireStaticInitialization(plan);
		if (initialization.executionOrder.length > 0) {
			expectedDirectDecisions.push("compiler-planned-eager-static-initialization");
		}
		if (runtimePlan.features.indexOf("string-literal") != -1) {
			expectedDirectDecisions.push("direct-utf8-string-literals");
		}
		if (runtimePlan.features.indexOf("array") != -1)
			expectedDirectDecisions.push("managed-haxe-arrays");
		if (runtimePlan.features.indexOf("string-map") != -1)
			expectedDirectDecisions.push("managed-haxe-string-maps");
		if (runtimePlan.features.indexOf("bytes") != -1)
			expectedDirectDecisions.push("managed-haxe-bytes");
		if (runtimePlan.features.indexOf("gc") != -1)
			expectedDirectDecisions.push("exact-traced-haxe-object-graph");
		expectedDirectDecisions.sort(compareUtf8);
		if (runtimePlan.directDecisions.join("\n") != expectedDirectDecisions.join("\n")) {
			fail("direct executable runtime analysis differs from compiler-owned direct and program-local decisions");
		}
	}

	function validateCompilerRuntimePlan(runtimePlan:RuntimeFeaturePlanSnapshot):Void {
		if (runtimePlan.features.length == 0
			|| runtimePlan.rootReasons.length == 0
			|| runtimePlan.manualOverrides.length != 0
			|| runtimePlan.selectedFeatures.length != runtimePlan.features.length
			|| runtimePlan.artifactDetails.length == 0
			|| runtimePlan.artifacts.length != runtimePlan.artifactDetails.length
			|| runtimePlan.noRuntimeProof != null) {
			fail("the admitted runtime-using slice requires one complete dependency-closed compiler runtime plan");
		}
		for (reason in runtimePlan.rootReasons) {
			switch reason.featureId {
				case "string-literal" if (reason.kind == "direct-string-value"):
				case "io" if (reason.kind == "hosted-output"):
				case "array" if (reason.kind == "runtime-operation"):
				case "string-map" if (reason.kind == "runtime-operation"):
				case "bytes" if (reason.kind == "runtime-operation"):
				case "alloc" if (reason.kind == "runtime-operation" && reason.operationId == "allocation"):
				case "gc" if (reason.kind == "runtime-operation" && switch reason.operationId {
						case "allocation" | "class-object-header" | "managed-type-representation" | "root-frame": true;
						case _: false;
					}):
				case _:
					fail('compiler runtime plan contains an unadmitted root reason `${reason.id}`');
			}
		}
	}

	function validateSpecializationReport(plan:CProjectEmissionPlan, genericFunctionCount:Int, genericTypeCount:Int):Void {
		final report = plan.specializationReport;
		if (genericFunctionCount + genericTypeCount == 0) {
			if (report != null)
				fail("a specialization report requires at least one reachable generic specialization");
			return;
		}
		if (report == null)
			return fail("reachable generic specializations require hxc.specializations.json");
		if (report.schemaVersion != CGenericSpecializationContract.REPORT_SCHEMA_VERSION
			|| report.algorithm != CGenericSpecializationContract.REPORT_ALGORITHM
			|| report.status != CGenericSpecializationContract.REPORT_STATUS
			|| report.keyEncoding != CGenericSpecializationContract.KEY_ENCODING
			|| report.compactNameDigest != CGenericSpecializationContract.COMPACT_NAME_DIGEST
			|| report.codeSizeAttribution != CGenericSpecializationContract.CODE_SIZE_ATTRIBUTION
			|| report.limits.maxFunctionSpecializations != CGenericSpecializationContract.MAX_FUNCTION_SPECIALIZATIONS
			|| report.limits.maxTypeSpecializations != CGenericSpecializationContract.MAX_TYPE_SPECIALIZATIONS
			|| report.limits.maxEstimatedSpecializationCBytes != CGenericSpecializationContract.MAX_ESTIMATED_SPECIALIZATION_C_BYTES
			|| report.summary.functionSpecializations != genericFunctionCount
			|| report.summary.typeSpecializations != genericTypeCount
			|| report.functionSpecializations.length != genericFunctionCount
			|| report.typeSpecializations.length != genericTypeCount) {
			fail("generic specialization report schema, algorithm, status, or counts differ from the emission plan");
		}
		var priorFunctionKey:Null<String> = null;
		var functionBytes = 0;
		var functionReasons = 0;
		var recursiveCount = 0;
		for (specialization in report.functionSpecializations) {
			validateLogicalText(specialization.baseFunctionId, "generic function base ID");
			validateLogicalText(specialization.instanceId, "generic function instance ID");
			validateLogicalText(specialization.displayName, "generic function display name");
			validateLogicalText(specialization.cName, "generic function C name");
			if (specialization.arguments.length == 0
				|| specialization.reasons.length == 0
				|| specialization.semanticDigestSha256 != Sha256.encode(specialization.specializationKey)
				|| specialization.instanceId != 'function.specialization.${specialization.semanticDigestSha256}'
				|| priorFunctionKey != null
				&& compareUtf8(priorFunctionKey, specialization.specializationKey) >= 0
				|| specialization.codeSize.metric != "strict-c11-utf8-function-definition-bytes"
				|| specialization.codeSize.definitionBytes < 1
				|| specialization.codeSize.irBlocks < 1
				|| specialization.codeSize.irInstructions < 0
				|| !~/^[0-9a-f]{64}$/.match(specialization.codeSize.definitionSha256)) {
				fail('generic function specialization `${specialization.instanceId}` is malformed or out of canonical order');
			}
			priorFunctionKey = specialization.specializationKey;
			functionBytes += specialization.codeSize.definitionBytes;
			functionReasons += specialization.reasons.length;
			if (specialization.recursive)
				recursiveCount++;
			final argumentKeys:Array<String> = [];
			for (argument in specialization.arguments) {
				validateSpecializationArgument(argument.parameter, argument.key, argument.displayName, argument.representation);
				argumentKeys.push(argument.key);
			}
			if (specialization.specializationKey != CGenericSpecializationContract.functionKey(specialization.baseFunctionId, argumentKeys))
				fail('generic function specialization `${specialization.instanceId}` does not match its base function and arguments');
			var priorReasonKey:Null<String> = null;
			for (reason in specialization.reasons) {
				validateLogicalText(reason.callerInstanceId, "generic reachability caller ID");
				validateGenericSource(reason.source, "generic function reachability reason");
				final reasonKey = reason.callerInstanceId + "\x00" + genericSourceKey(reason.source);
				if (priorReasonKey != null && compareUtf8(priorReasonKey, reasonKey) >= 0)
					fail('generic function specialization `${specialization.instanceId}` has duplicate or unordered reasons');
				priorReasonKey = reasonKey;
			}
		}
		var priorTypeKey:Null<String> = null;
		var typeReasons = 0;
		for (specialization in report.typeSpecializations) {
			validateLogicalText(specialization.haxePath, "generic type Haxe path");
			validateLogicalText(specialization.displayName, "generic type display name");
			validateLogicalText(specialization.cName, "generic type C name");
			if (specialization.arguments.length == 0
				|| specialization.reasons.length == 0
				|| specialization.semanticDigestSha256 != Sha256.encode(specialization.specializationKey)
				|| specialization.instanceId != 'instance.enum.${specialization.semanticDigestSha256}'
				|| specialization.declarationId != 'type.enum.${specialization.semanticDigestSha256}'
				|| priorTypeKey != null
				&& compareUtf8(priorTypeKey, specialization.specializationKey) >= 0
				|| specialization.representation != "native-enum"
				&& specialization.representation != "tagged-union"
				|| specialization.codeSize.metric != "typed-enum-layout-structural-units"
				|| specialization.codeSize.structuralUnits != 1 + specialization.codeSize.constructorCount + specialization.codeSize.payloadFieldCount
				|| specialization.codeSize.constructorCount < 1
				|| specialization.codeSize.payloadFieldCount < 0) {
				fail('generic type specialization `${specialization.instanceId}` is malformed or out of canonical order');
			}
			priorTypeKey = specialization.specializationKey;
			typeReasons += specialization.reasons.length;
			if (specialization.recursive)
				recursiveCount++;
			final argumentKeys:Array<String> = [];
			for (argument in specialization.arguments) {
				validateSpecializationArgument(argument.parameter, argument.key, argument.displayName, argument.representation);
				argumentKeys.push(argument.key);
			}
			if (specialization.specializationKey != CGenericSpecializationContract.enumInstanceKey(specialization.haxePath, argumentKeys))
				fail('generic type specialization `${specialization.instanceId}` does not match its Haxe path and arguments');
			var priorReasonKey:Null<String> = null;
			for (reason in specialization.reasons) {
				validateGenericSource(reason, "generic type reachability reason");
				final reasonKey = genericSourceKey(reason);
				if (priorReasonKey != null && compareUtf8(priorReasonKey, reasonKey) >= 0)
					fail('generic type specialization `${specialization.instanceId}` has duplicate or unordered reasons');
				priorReasonKey = reasonKey;
			}
		}
		var payloadArtifacts = 0;
		var payloadBytes = 0;
		for (unit in plan.units) {
			if (GeneratedFile.isPayloadKind(unit.kind)) {
				payloadArtifacts++;
				payloadBytes += Bytes.ofString(unit.contents).length;
			}
		}
		if (report.summary.mergedFunctionReasons != functionReasons
			|| report.summary.mergedTypeReasons != typeReasons
			|| report.summary.recursiveSpecializations != recursiveCount
			|| report.summary.specializedFunctionDefinitionBytes != functionBytes
			|| report.summary.dependencyClosedEnumDefinitionBytes < 0
			|| report.summary.estimatedSpecializationCBytes != functionBytes + report.summary.dependencyClosedEnumDefinitionBytes
			|| report.summary.estimatedSpecializationCBytes > report.limits.maxEstimatedSpecializationCBytes
			|| report.summary.generatedPayloadArtifacts != payloadArtifacts
			|| report.summary.generatedPayloadBytes != payloadBytes
			|| report.limits.maxFunctionSpecializations < genericFunctionCount
			|| report.limits.maxTypeSpecializations < genericTypeCount) {
			fail("generic specialization report reason, recursion, code-size, or payload totals are inconsistent");
		}
	}

	function validateDispatchReport(plan:CProjectEmissionPlan):Void {
		final directCount = plan.directInstanceCallCount == null ? 0 : plan.directInstanceCallCount;
		final indirectCount = plan.indirectInstanceCallCount == null ? 0 : plan.indirectInstanceCallCount;
		if (directCount < 0 || indirectCount < 0)
			fail("instance-dispatch counts cannot be negative");
		final report = plan.dispatchReport;
		if (directCount + indirectCount == 0) {
			if (report != null)
				fail("hxc.dispatch.json requires at least one reachable instance call");
			return;
		}
		if (report == null)
			return fail("reachable instance calls require hxc.dispatch.json");
		if (report.schemaVersion != CDispatchReportBuilder.SCHEMA_VERSION
			|| report.algorithm != CDispatchReportBuilder.ALGORITHM
			|| report.status != CDispatchReportBuilder.STATUS
			|| report.tablePolicy != CDispatchReportBuilder.TABLE_POLICY
			|| report.adapterPolicy != CDispatchReportBuilder.ADAPTER_POLICY
			|| report.runtimeFeatures.length != 0
			|| report.summary.instanceCalls != directCount + indirectCount
			|| report.summary.directCalls != directCount
			|| report.summary.indirectCalls != indirectCount
			|| report.summary.layouts != report.layouts.length
			|| report.summary.slots != report.slots.length
			|| report.summary.tables != report.tables.length
			|| report.summary.adapters < 0
			|| report.calls.length != directCount + indirectCount) {
			fail("dispatch report schema, policy, counts, or runtime boundary differ from the emission plan");
		}
		final slotIds:Map<String, Bool> = [];
		var previousSlotId:Null<String> = null;
		for (slot in report.slots) {
			validateLogicalText(slot.id, "virtual slot ID");
			validateLogicalText(slot.ownerInstanceId, "virtual slot owner instance ID");
			validateCIdentifier(slot.cMember, "virtual slot C member");
			validateLogicalText(slot.returnRepresentation, "virtual slot return representation");
			switch previousSlotId {
				case null:
				case previous if (compareUtf8(previous, slot.id) >= 0):
					fail('dispatch report has duplicate or unordered virtual slot `${slot.id}`');
				case _:
			}
			if (slotIds.exists(slot.id))
				fail('dispatch report has duplicate or unordered virtual slot `${slot.id}`');
			for (parameter in slot.parameterRepresentations)
				validateLogicalText(parameter, "virtual slot parameter representation");
			slotIds.set(slot.id, true);
			previousSlotId = slot.id;
		}
		final layoutSlots:Map<String, Array<String>> = [];
		var previousLayoutId:Null<String> = null;
		for (layout in report.layouts) {
			validateLogicalText(layout.id, "virtual layout ID");
			validateLogicalText(layout.rootInstanceId, "virtual layout root instance ID");
			validateCIdentifier(layout.cTag, "virtual layout C tag");
			switch previousLayoutId {
				case null:
				case previous if (compareUtf8(previous, layout.id) >= 0):
					fail('dispatch report has empty, duplicate, or unordered virtual layout `${layout.id}`');
				case _:
			}
			if (layoutSlots.exists(layout.id) || layout.slotIds.length == 0)
				fail('dispatch report has empty, duplicate, or unordered virtual layout `${layout.id}`');
			var previousMember:Null<String> = null;
			for (slotId in layout.slotIds) {
				if (!slotIds.exists(slotId))
					fail('virtual layout `${layout.id}` has an unknown, duplicate, or unordered slot `$slotId`');
				switch previousMember {
					case null:
					case previous if (compareUtf8(previous, slotId) >= 0):
						fail('virtual layout `${layout.id}` has an unknown, duplicate, or unordered slot `$slotId`');
					case _:
				}
				previousMember = slotId;
			}
			layoutSlots.set(layout.id, layout.slotIds);
			previousLayoutId = layout.id;
		}
		final adapterNames:Map<String, Bool> = [];
		var previousTableId:Null<String> = null;
		for (table in report.tables) {
			validateLogicalText(table.id, "virtual table ID");
			validateLogicalText(table.layoutId, "virtual table layout ID");
			validateLogicalText(table.classInstanceId, "virtual table class instance ID");
			validateCIdentifier(table.cName, "virtual table C name");
			final expectedSlots = requireDispatchLayoutSlots(layoutSlots, table.layoutId, table.id);
			switch previousTableId {
				case null:
				case previous if (compareUtf8(previous, table.id) >= 0):
					fail('dispatch report has malformed or unordered virtual table `${table.id}`');
				case _:
			}
			if (table.entries.length != expectedSlots.length)
				fail('dispatch report has malformed or unordered virtual table `${table.id}`');
			for (index in 0...table.entries.length) {
				final entry = table.entries[index];
				if (entry.slotId != expectedSlots[index])
					fail('virtual table `${table.id}` entry $index differs from its layout slot order');
				if (entry.implementationFunctionId != null)
					validateLogicalText(entry.implementationFunctionId, "virtual table implementation function ID");
				if (entry.adapterCName != null) {
					if (entry.implementationFunctionId == null)
						fail('virtual table `${table.id}` has an adapter without an implementation');
					validateCIdentifier(entry.adapterCName, "virtual table adapter C name");
					adapterNames.set(entry.adapterCName, true);
				}
			}
			previousTableId = table.id;
		}
		var adapterCount = 0;
		for (_ in adapterNames)
			adapterCount++;
		if (adapterCount != report.summary.adapters)
			fail("dispatch adapter records differ from their summary count");
		var observedDirect = 0;
		var observedIndirect = 0;
		for (index in 0...report.calls.length) {
			final call = report.calls[index];
			if (call.id != 'dispatch.call.$index')
				fail('dispatch call record $index lost its stable ordinal ID');
			validateLogicalText(call.callerFunctionId, "dispatch caller function ID");
			validateLogicalText(call.methodFunctionId, "dispatch method function ID");
			validateLogicalText(call.receiverStaticClass, "dispatch receiver class");
			validateLogicalText(call.reason, "dispatch reason");
			validateDispatchSource(call.source, call.id);
			switch call.dispatch {
				case "direct":
					observedDirect++;
					if (call.slotId != null)
						fail('direct dispatch call `${call.id}` lost its target-only choice');
					switch call.targetFunctionId {
						case null: fail('direct dispatch call `${call.id}` lost its target-only choice');
						case target: validateLogicalText(target, "direct dispatch target function ID");
					}
				case "virtual" | "interface":
					observedIndirect++;
					if (call.targetFunctionId != null)
						fail('indirect dispatch call `${call.id}` lost its reachable slot-only choice');
					switch call.slotId {
						case null: fail('indirect dispatch call `${call.id}` lost its reachable slot-only choice');
						case slotId if (!slotIds.exists(slotId)):
							fail('indirect dispatch call `${call.id}` refers to unknown reachable slot `$slotId`');
						case _:
					}
				case invalid:
					fail('dispatch call `${call.id}` has unknown choice `$invalid`');
			}
		}
		if (observedDirect != directCount || observedIndirect != indirectCount)
			fail("dispatch call records differ from their direct and indirect summary counts");
		if (indirectCount > 0 && (report.layouts.length == 0 || report.slots.length == 0 || report.tables.length == 0))
			fail("indirect dispatch requires a non-empty reachable layout, slot, and table plan");
	}

	static function requireDispatchLayoutSlots(values:Map<String, Array<String>>, layoutId:String, tableId:String):Array<String> {
		final value = values.get(layoutId);
		if (value == null)
			throw new ProjectEmissionError('virtual table `$tableId` refers to unknown layout `$layoutId`');
		return value;
	}

	static function validateDispatchSource(source:reflaxe.c.lowering.CDispatchReport.CDispatchSourceSnapshot, label:String):Void {
		if (!reflaxe.c.ir.HxcSourceSpan.isNormalizedFile(source.file)
			|| source.startLine < 1
			|| source.startColumn < 1
			|| source.endLine < source.startLine
			|| source.endColumn < 1
			|| source.endLine == source.startLine
			&& source.endColumn < source.startColumn) {
			throw new ProjectEmissionError('dispatch call `$label` has malformed source `${source.file}`');
		}
	}

	function validateSpecializationArgument(parameter:String, key:String, displayName:String, representation:String):Void {
		validateLogicalText(parameter, "generic type parameter");
		validateLogicalText(key, "generic type argument key");
		validateLogicalText(displayName, "generic type argument display name");
		// Validate the complete closed vocabulary produced by the generic
		// canonicalizer. Project emission must not reject a representation that
		// body lowering has already admitted and recorded structurally.
		switch representation {
			case "direct-primitive" | "direct-enum" | "managed-array" | "direct-record" | "immutable-string" | "nullable-value":
			case _:
				fail('generic type argument `$parameter` has unknown representation `$representation`');
		}
	}

	static function validateGenericSource(source:reflaxe.c.lowering.CGenericSpecializationReport.CGenericSourceSnapshot, label:String):Void {
		if (!reflaxe.c.ir.HxcSourceSpan.isNormalizedFile(source.file)
			|| source.startLine < 1
			|| source.startColumn < 1
			|| source.endLine < source.startLine
			|| source.endColumn < 1
			|| source.endLine == source.startLine
			&& source.endColumn < source.startColumn) {
			throw new ProjectEmissionError('$label has malformed source `${source.file}`');
		}
	}

	static function genericSourceKey(source:reflaxe.c.lowering.CGenericSpecializationReport.CGenericSourceSnapshot):String
		return '${source.file}:${source.startLine}:${source.startColumn}-${source.endLine}:${source.endColumn}';

	function validateRuntimePayload(units:Array<GeneratedFile>, runtimePlan:RuntimeFeaturePlanSnapshot):Void {
		final runtimeUnits:Array<GeneratedFile> = [];
		for (unit in units) {
			if (unit.kind == RuntimeHeader || unit.kind == RuntimeSource) {
				runtimeUnits.push(unit);
			}
		}
		runtimeUnits.sort(compareFiles);
		if (runtimeUnits.map(unit -> unit.relativePath).join("\n") != runtimePlan.artifacts.join("\n")) {
			fail("packaged runtime payload differs from the analyzed artifact selection");
		}
		for (index in 0...runtimeUnits.length) {
			if (runtimeUnits[index].kind != runtimePlan.artifactDetails[index].kind) {
				fail('packaged runtime artifact `${runtimeUnits[index].relativePath}` has the wrong generated-file kind');
			}
		}
	}

	static function requireStaticInitialization(plan:CProjectEmissionPlan):CStaticInitializationSnapshot {
		final initialization = plan.staticInitialization;
		if (initialization == null) {
			throw new ProjectEmissionError("direct executable emission requires a static-initialization plan");
		}
		return initialization;
	}

	static function requireRuntimePlan(plan:CProjectEmissionPlan):RuntimeFeaturePlanSnapshot {
		final runtimePlan = plan.runtimePlan;
		if (runtimePlan == null) {
			throw new ProjectEmissionError("direct executable emission requires a runtime feature plan");
		}
		return runtimePlan;
	}

	static function validateInitializationSource(source:reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationSource, label:String):Void {
		if (!reflaxe.c.ir.HxcSourceSpan.isNormalizedFile(source.file)
			|| source.startLine < 1
			|| source.startColumn < 1
			|| source.endLine < source.startLine
			|| source.endColumn < 1
			|| source.endLine == source.startLine
			&& source.endColumn < source.startColumn) {
			throw new ProjectEmissionError('static-initialization $label has malformed source `${source.file}`');
		}
	}

	function canonicalUnits(input:Array<GeneratedFile>):Array<GeneratedFile> {
		final units = input.copy();
		units.sort(compareFiles);
		var previous:Null<String> = null;
		for (unit in units) {
			unit.verifyIntegrity();
			if (!GeneratedFile.isPayloadKind(unit.kind)) {
				fail('project payload `${unit.relativePath}` uses reserved sidecar kind `${Std.string(unit.kind)}`', [unit.relativePath]);
			}
			if (SIDECAR_PATHS.indexOf(unit.relativePath) != -1) {
				fail('project payload collides with compiler sidecar `${unit.relativePath}`', [unit.relativePath]);
			}
			validateLayout(unit);
			if (previous == unit.relativePath) {
				fail('duplicate generated artifact path `${unit.relativePath}`', [unit.relativePath]);
			}
			previous = unit.relativePath;
		}
		return units;
	}

	function validateLayout(file:GeneratedFile):Void {
		final valid = switch file.kind {
			case PublicHeader | PrivateHeader: StringTools.startsWith(file.relativePath, "include/") && StringTools.endsWith(file.relativePath, ".h");
			case Source: StringTools.startsWith(file.relativePath, "src/") && StringTools.endsWith(file.relativePath, ".c");
			case RuntimeHeader: StringTools.startsWith(file.relativePath, "runtime/include/") && StringTools.endsWith(file.relativePath, ".h");
			case RuntimeSource: StringTools.startsWith(file.relativePath, "runtime/src/") && StringTools.endsWith(file.relativePath, ".c");
			case _: false;
		};
		if (!valid) {
			fail('generated `${Std.string(file.kind)}` artifact has a non-canonical layout path `${file.relativePath}`', [file.relativePath]);
		}
	}

	function runtimePlanPlaceholder(plan:CProjectEmissionPlan):RuntimePlanPlaceholder {
		return {
			schemaVersion: SCHEMA_VERSION,
			status: PlaceholderStatus.RuntimeNotAnalyzed,
			requestedPolicy: plan.runtimePolicy,
			diagnosticMode: plan.runtimeDiagnostics,
			environment: plan.environment,
			rootReasons: [],
			features: [],
			artifacts: [],
			symbols: [],
			libraries: [],
			defines: [],
			noRuntimeProof: null
		};
	}

	function runtimePlanResolved(plan:CProjectEmissionPlan):RuntimeFeaturePlanSnapshot
		return requireRuntimePlan(plan);

	function abiPlaceholder(plan:CProjectEmissionPlan):AbiPlaceholder {
		return {
			schemaVersion: SCHEMA_VERSION,
			status: PlaceholderStatus.AbiNotAnalyzed,
			stability: AbiStability.Experimental,
			profile: plan.profile,
			exports: [],
			types: []
		};
	}

	function abiResolved(plan:CProjectEmissionPlan):AbiResolved {
		return {
			schemaVersion: SCHEMA_VERSION,
			status: ResolvedAnalysisStatus.NoExports,
			stability: AbiStability.Experimental,
			profile: plan.profile,
			exports: [],
			types: [],
			executableEntryPoint: "main"
		};
	}

	function stdlibPlaceholder(plan:CProjectEmissionPlan):StdlibPlaceholder {
		return {
			schemaVersion: SCHEMA_VERSION,
			status: PlaceholderStatus.StdlibNotAnalyzed,
			profile: plan.profile,
			modules: [],
			capabilities: []
		};
	}

	function stdlibResolved(plan:CProjectEmissionPlan):StdlibResolved {
		final modules = canonicalLogicalValues(plan.stdlibModules == null ? [] : plan.stdlibModules, "stdlib module");
		final capabilities = canonicalLogicalValues(plan.stdlibCapabilities == null ? [] : plan.stdlibCapabilities, "stdlib capability");
		return {
			schemaVersion: SCHEMA_VERSION,
			status: modules.length == 0
			&& capabilities.length == 0 ? ResolvedAnalysisStatus.NoStdlib : ResolvedAnalysisStatus.SelectedStdlib,
			profile: plan.profile,
			modules: modules,
			capabilities: capabilities
		};
	}

	function canonicalLogicalValues(input:Array<String>, label:String):Array<String> {
		final values = input.copy();
		values.sort(compareUtf8);
		var previous:Null<String> = null;
		for (value in values) {
			validateLogicalText(value, label);
			if (value == previous) {
				fail('$label `$value` is duplicated');
			}
			previous = value;
		}
		return values;
	}

	function jsonFile<T>(path:String, kind:GeneratedFileKind, value:T):GeneratedFile
		return new GeneratedFile(path, Json.stringify(value, null, "  ") + "\n", kind);

	function validateLogicalText(value:String, label:String):Void {
		if (StringTools.trim(value) == "" || value.indexOf("\x00") != -1 || value.indexOf("\r") != -1 || value.indexOf("\n") != -1) {
			fail('$label must be non-empty, single-line logical text');
		}
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null || code < 0x20 || code == 0x7F) {
				fail('$label contains a forbidden control character');
			}
		}
	}

	function validateCIdentifier(value:String, label:String):Void {
		try {
			new CIdentifier(value);
		} catch (_:String) {
			fail('$label must be one finalized lexical C11 identifier');
		}
	}

	static function compareFiles(left:GeneratedFile, right:GeneratedFile):Int
		return compareUtf8(left.relativePath, right.relativePath);

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...length) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}

	static function fail(detail:String, ?paths:Array<String>):Void
		throw new ProjectEmissionError(detail, paths);
}

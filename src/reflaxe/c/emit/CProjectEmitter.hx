package reflaxe.c.emit;

import haxe.Json;
import haxe.io.Bytes;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.emit.CBuildPlan.CBuildPlanBuilder;
import reflaxe.c.emit.CBuildPlan.CBuildPlanSnapshot;
import reflaxe.c.emit.CBuildPlan.CBuildStandard;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationSnapshot;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationStrategy;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationPhase;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanSnapshot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanStatus;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningPurpose;
import reflaxe.c.runtime.RuntimeFeaturePlanner;

/** Input status distinguishes structural fixtures, the admitted primitive slice, and unproven broader programs. */
enum abstract CProjectCompilationStatus(String) to String {
	var StructuralFixture = "structural-fixture-no-haxe-lowering";
	var PrimitiveExecutable = "lowered-primitive-executable";
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
	final runtimePolicy:CProjectRuntimePolicy;
	final runtimeDiagnostics:CProjectRuntimeDiagnostics;
	final ?runtimePolicyProvenance:String;
	final ?runtimeDiagnosticsProvenance:String;
	final units:Array<GeneratedFile>;
	final buildFacts:Array<TypedCBuildFact>;
	final ?primitiveHelperIds:Array<String>;
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
		"hxc.initialization-plan.json",
		"hxc.manifest.json",
		"hxc.runtime-plan.json",
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
			case PrimitiveExecutable:
				files.push(jsonFile("hxc.initialization-plan.json", GeneratedFileKind.InitializationPlan, requireStaticInitialization(plan)));
				files.push(jsonFile("hxc.runtime-plan.json", GeneratedFileKind.RuntimePlan, runtimePlanResolved(plan)));
				files.push(jsonFile("hxc.abi.json", GeneratedFileKind.AbiManifest, abiResolved(plan)));
				files.push(jsonFile("hxc.stdlib-report.json", GeneratedFileKind.StdlibReport, stdlibResolved(plan)));
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
			case PrimitiveExecutable:
				validatePrimitiveExecutablePlan(plan);
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
		if (plan.symbolTable.schemaVersion != 1 || plan.symbolTable.algorithm != "hxc-c-symbol-v1") {
			fail("project emission requires the finalized schema-1 hxc-c-symbol-v1 symbol table");
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

	function validatePrimitiveExecutablePlan(plan:CProjectEmissionPlan):Void {
		if (plan.runtimePolicyProvenance == null || plan.runtimeDiagnosticsProvenance == null) {
			fail("primitive executable emission requires resolved runtime-policy provenance");
		}
		if (plan.environment != CProjectEnvironment.Hosted) {
			fail('primitive executable emission requires the hosted environment; found `${plan.environment}`');
		}
		final initialization = requireStaticInitialization(plan);
		final runtimePlan = requireRuntimePlan(plan);
		validatePrimitiveRuntimePlan(plan, runtimePlan);
		if (initialization.schemaVersion != 1
			|| initialization.strategy != CStaticInitializationStrategy.EagerHaxeTypeOrder
			|| initialization.entryFunctionId == ""
			|| initialization.phaseOrder.join(",") != [
				CStaticInitializationPhase.ClassInitializers,
				CStaticInitializationPhase.StaticFieldInitializers,
				CStaticInitializationPhase.Entry
			].join(",")
			|| initialization.runtimeFeatures.length != 0) {
			fail("primitive executable emission requires a valid schema-1 static-initialization plan with no independent initializer runtime roots");
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
			if (fact.kind != "link" || fact.name != "m" || fact.value != null || fact.valueKind != null || fact.ownerModulePaths.length == 0) {
				fail('primitive executable emission only admits the compiler-selected C math link fact; found `${fact.kind}` `${fact.name}`');
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
					fail('primitive executable emission cannot package `${Std.string(unit.kind)}` `${unit.relativePath}`', [unit.relativePath]);
				case _:
					fail('primitive executable payload has invalid kind `${Std.string(unit.kind)}`', [unit.relativePath]);
			}
		}
		if (sources < 1 || privateHeaders != 1) {
			fail('primitive executable emission requires at least one source and exactly one private prototype header; found $sources source(s) and $privateHeaders header(s)');
		}
		var entryPoints = 0;
		for (symbol in plan.symbolTable.symbols) {
			if (symbol.cName == "main" && symbol.requestedName == "main") {
				entryPoints++;
			}
			if (StringTools.startsWith(symbol.cName, "hxrt_")) {
				fail('primitive executable symbol table contains runtime symbol `${symbol.cName}`');
			}
		}
		if (entryPoints != 1) {
			fail('primitive executable symbol table requires exactly one compiler-owned exact `main`; found $entryPoints');
		}
	}

	function validatePrimitiveRuntimePlan(plan:CProjectEmissionPlan, runtimePlan:RuntimeFeaturePlanSnapshot):Void {
		if (runtimePlan.schemaVersion != RuntimeFeaturePlanner.PLAN_SCHEMA_VERSION
			|| runtimePlan.algorithm != RuntimeFeaturePlanner.PLAN_ALGORITHM
			|| runtimePlan.planPurpose != RuntimePlanningPurpose.CompilerProgram) {
			fail("primitive executable emission requires a compiler-program hxc-runtime-plan-v1 analysis");
		}
		if (runtimePlan.profile != plan.profile
			|| runtimePlan.environment != plan.environment
			|| runtimePlan.requestedPolicy != plan.runtimePolicy
			|| runtimePlan.resolvedPolicy != plan.runtimePolicy
			|| runtimePlan.diagnosticMode != plan.runtimeDiagnostics
			|| runtimePlan.policyProvenance != plan.runtimePolicyProvenance
			|| runtimePlan.diagnosticProvenance != plan.runtimeDiagnosticsProvenance) {
			fail("primitive executable runtime analysis differs from resolved project configuration");
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
					|| runtimePlan.defines.length != 0
					|| !hasRuntimeProof(runtimePlan.noRuntimeProof)) {
					fail("runtime-free primitive analysis must prove complete hxrt absence");
				}
			case RuntimeFeaturePlanStatus.RuntimeFeatures:
				validateHostedOutputRuntimePlan(runtimePlan);
			case RuntimeFeaturePlanStatus.NativeSeedFeatures:
				fail("generated Haxe cannot use a native-seed runtime plan");
			case _:
				fail('primitive executable runtime analysis has unknown status `${Std.string(runtimePlan.status)}`');
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
		if (helperIds.length > 0) {
			expectedDirectDecisions.push("selected-program-local-helpers");
		}
		final initialization = requireStaticInitialization(plan);
		if (initialization.executionOrder.length > 0) {
			expectedDirectDecisions.push("compiler-planned-eager-static-initialization");
		}
		if (runtimePlan.status == RuntimeFeaturePlanStatus.RuntimeFeatures) {
			expectedDirectDecisions.push("direct-utf8-string-literals");
		}
		expectedDirectDecisions.sort(compareUtf8);
		if (runtimePlan.directDecisions.join("\n") != expectedDirectDecisions.join("\n")) {
			fail("primitive executable runtime analysis differs from compiler-owned direct and program-local decisions");
		}
	}

	function validateHostedOutputRuntimePlan(runtimePlan:RuntimeFeaturePlanSnapshot):Void {
		final expectedFeatures = ["runtime-base", "status", "string-literal", "io"];
		if (runtimePlan.features.join("\n") != expectedFeatures.join("\n")
			|| runtimePlan.rootReasons.length == 0
			|| runtimePlan.manualOverrides.length != 0
			|| runtimePlan.selectedFeatures.length != expectedFeatures.length
			|| runtimePlan.artifactDetails.length == 0
			|| runtimePlan.artifacts.length != runtimePlan.artifactDetails.length
			|| runtimePlan.symbols.indexOf("hxc_io_println") == -1
			|| runtimePlan.libraries.length != 0
			|| runtimePlan.defines.length != 0
			|| runtimePlan.noRuntimeProof != null) {
			fail("the admitted runtime-using primitive slice requires exactly the dependency-closed hosted String output plan");
		}
		for (reason in runtimePlan.rootReasons) {
			if (reason.featureId != "io" || reason.kind != "hosted-output") {
				fail('hosted output runtime plan contains unrelated root reason `${reason.id}`');
			}
		}
	}

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
			throw new ProjectEmissionError("primitive executable emission requires a static-initialization plan");
		}
		return initialization;
	}

	static function requireRuntimePlan(plan:CProjectEmissionPlan):RuntimeFeaturePlanSnapshot {
		final runtimePlan = plan.runtimePlan;
		if (runtimePlan == null) {
			throw new ProjectEmissionError("primitive executable emission requires a runtime feature plan");
		}
		return runtimePlan;
	}

	static function hasRuntimeProof(value:Null<String>):Bool {
		return switch value {
			case null: false;
			case proof: StringTools.trim(proof) != "";
		};
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

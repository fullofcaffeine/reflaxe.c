package reflaxe.c.runtime;

import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.ir.HxcSourceSpan;

/** Validated stable identity used by runtime definitions, reasons, and plans. */
abstract RuntimeFeatureId(String) {
	private inline function new(value:String) {
		this = value;
	}

	public static function parse(value:String):RuntimeFeatureId {
		if (!~/^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/.match(value)) {
			throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError, 'runtime feature ID must be lowercase kebab-case: `$value`');
		}
		return new RuntimeFeatureId(value);
	}

	public inline function text():String
		return this;

	public inline function toString():String
		return this;
}

/** Availability is evidence, not a request to promote a native seed. */
enum abstract RuntimeFeatureAvailability(String) to String {
	var CompilerSelectable = "compiler-selectable";
	var NativeSeedOnly = "native-seed-only";
}

enum abstract RuntimePlanningPurpose(String) to String {
	var CompilerProgram = "compiler-program";
	var NativeSeedFixture = "native-seed-fixture";
}

enum abstract RuntimeFeaturePlanStatus(String) to String {
	var RuntimeFree = "analyzed-runtime-free";
	var RuntimeFeatures = "analyzed-runtime-features";
	var NativeSeedFeatures = "analyzed-native-seed-features";
}

enum abstract RuntimeFeatureCatalogStatus(String) to String {
	var ProvisionalNativeSeedPackaging = "provisional-native-seed-packaging";
	var SelectiveCompilerPackaging = "selective-compiler-packaging";
}

enum abstract RuntimeAbiStability(String) to String {
	var InternalVersioned = "internal-versioned";
}

enum abstract RuntimeAbiCompatibility(String) to String {
	var SameMajor = "same-major";
}

enum abstract RuntimeGeneratedAbiCheck(String) to String {
	var C11StaticAssert = "c11-static-assert";
}

enum abstract RuntimeApplicationAbiStatus(String) to String {
	var Unsupported = "unsupported";
}

enum abstract RuntimePublicExportTypePolicy(String) to String {
	var RuntimeStructsForbidden = "runtime-structs-forbidden";
}

enum abstract RuntimeFeatureOverrideAction(String) to String {
	var Require = "require";
	var Forbid = "forbid";
}

enum abstract RuntimeNoRuntimeScope(String) to String {
	var ReachableWholeProgram = "reachable-whole-program";
	var NativeSeedFixture = "native-seed-fixture";
}

enum abstract RuntimeNoRuntimeStatus(String) to String {
	var Eligible = "eligible";
}

class RuntimeFeatureDefine {
	public final name:String;
	public final value:String;

	public function new(name:String, value:String) {
		this.name = name;
		this.value = value;
	}

	public function display():String
		return '$name=$value';
}

class RuntimeFeatureArtifact {
	public final sourcePath:String;
	public final outputPath:String;
	public final kind:GeneratedFileKind;
	public final sourceSha256:String;

	public function new(sourcePath:String, outputPath:String, kind:GeneratedFileKind, sourceSha256:String) {
		this.sourcePath = sourcePath;
		this.outputPath = outputPath;
		this.kind = kind;
		this.sourceSha256 = sourceSha256;
	}
}

class RuntimeFeatureDefinition {
	public final id:RuntimeFeatureId;
	public final summary:String;
	public final availability:RuntimeFeatureAvailability;
	public final minimalAllowed:Bool;
	public final environments:Array<CEnvironment>;
	public final dependencies:Array<RuntimeFeatureId>;
	public final artifacts:Array<RuntimeFeatureArtifact>;
	public final symbols:Array<String>;
	public final libraries:Array<String>;
	public final defines:Array<RuntimeFeatureDefine>;

	public function new(id:RuntimeFeatureId, summary:String, availability:RuntimeFeatureAvailability, minimalAllowed:Bool, environments:Array<CEnvironment>,
			dependencies:Array<RuntimeFeatureId>, artifacts:Array<RuntimeFeatureArtifact>, symbols:Array<String>, libraries:Array<String>,
			defines:Array<RuntimeFeatureDefine>) {
		this.id = id;
		this.summary = summary;
		this.availability = availability;
		this.minimalAllowed = minimalAllowed;
		this.environments = environments.copy();
		this.dependencies = dependencies.copy();
		this.artifacts = artifacts.copy();
		this.symbols = symbols.copy();
		this.libraries = libraries.copy();
		this.defines = defines.copy();
	}
}

class RuntimeFeatureReservation {
	public final id:RuntimeFeatureId;
	public final ownerTask:String;
	public final summary:String;

	public function new(id:RuntimeFeatureId, ownerTask:String, summary:String) {
		this.id = id;
		this.ownerTask = ownerTask;
		this.summary = summary;
	}
}

/** One analyzer input describing why a reachable runtime intent exists. */
class RuntimeRequirementCandidate {
	public final featureId:RuntimeFeatureId;
	public final operationId:String;
	public final kind:String;
	public final surface:String;
	public final source:HxcSourceSpan;
	public final alternative:Null<String>;

	public function new(featureId:RuntimeFeatureId, operationId:String, kind:String, surface:String, source:HxcSourceSpan, ?alternative:String) {
		this.featureId = featureId;
		this.operationId = operationId;
		this.kind = kind;
		this.surface = surface;
		this.source = source;
		this.alternative = alternative;
	}
}

/** One deduplicated actionable semantic root; dependencies inherit its ID. */
class RuntimeRequirementReason {
	public final id:String;
	public final featureId:RuntimeFeatureId;
	public final operationId:String;
	public final kind:String;
	public final surface:String;
	public final source:HxcSourceSpan;
	public final alternative:Null<String>;

	public function new(id:String, featureId:RuntimeFeatureId, operationId:String, kind:String, surface:String, source:HxcSourceSpan, ?alternative:String) {
		this.id = id;
		this.featureId = featureId;
		this.operationId = operationId;
		this.kind = kind;
		this.surface = surface;
		this.source = source;
		this.alternative = alternative;
	}
}

/** Reviewable coverage counts from the complete reachable validated HxcIR. */
class RuntimeReachabilityEvidence {
	public final moduleCount:Int;
	public final typeInstanceCount:Int;
	public final functionCount:Int;
	public final blockCount:Int;
	public final instructionCount:Int;
	public final cleanupActionCount:Int;
	public final runtimeIntentCount:Int;

	public function new(moduleCount:Int, typeInstanceCount:Int, functionCount:Int, blockCount:Int, instructionCount:Int, cleanupActionCount:Int,
			runtimeIntentCount:Int) {
		this.moduleCount = moduleCount;
		this.typeInstanceCount = typeInstanceCount;
		this.functionCount = functionCount;
		this.blockCount = blockCount;
		this.instructionCount = instructionCount;
		this.cleanupActionCount = cleanupActionCount;
		this.runtimeIntentCount = runtimeIntentCount;
	}
}

class RuntimeRequirementAnalysis {
	public final reasons:Array<RuntimeRequirementReason>;
	public final reachability:RuntimeReachabilityEvidence;

	public function new(reasons:Array<RuntimeRequirementReason>, reachability:RuntimeReachabilityEvidence) {
		this.reasons = reasons.copy();
		this.reachability = reachability;
	}
}

/** Compiler-owned evidence admitted only for an empty reachable runtime-root set. */
class RuntimeNoRuntimeEvidence {
	public final scope:RuntimeNoRuntimeScope;
	public final semanticProof:String;
	public final reachability:RuntimeReachabilityEvidence;
	public final programLocalHelpers:Array<String>;

	public function new(scope:RuntimeNoRuntimeScope, semanticProof:String, reachability:RuntimeReachabilityEvidence, programLocalHelpers:Array<String>) {
		this.scope = scope;
		this.semanticProof = semanticProof;
		this.reachability = reachability;
		this.programLocalHelpers = programLocalHelpers.copy();
	}
}

/** Manual policy can confirm or forbid inferred work, never invent it. */
class RuntimeFeatureOverride {
	public final featureId:RuntimeFeatureId;
	public final action:RuntimeFeatureOverrideAction;
	public final provenance:String;

	public function new(featureId:RuntimeFeatureId, action:RuntimeFeatureOverrideAction, provenance:String) {
		this.featureId = featureId;
		this.action = action;
		this.provenance = provenance;
	}
}

class RuntimePlanningRequest {
	public final purpose:RuntimePlanningPurpose;
	public final profile:CProfile;
	public final environment:CEnvironment;
	public final runtimePolicy:CRuntimePolicy;
	public final runtimePolicyProvenance:String;
	public final runtimeDiagnostics:CRuntimeDiagnostics;
	public final runtimeDiagnosticsProvenance:String;
	public final rootReasons:Array<RuntimeRequirementReason>;
	public final manualOverrides:Array<RuntimeFeatureOverride>;
	public final directDecisions:Array<String>;
	public final noRuntimeEvidence:Null<RuntimeNoRuntimeEvidence>;

	public function new(purpose:RuntimePlanningPurpose, profile:CProfile, environment:CEnvironment, runtimePolicy:CRuntimePolicy,
			runtimePolicyProvenance:String, runtimeDiagnostics:CRuntimeDiagnostics, runtimeDiagnosticsProvenance:String,
			rootReasons:Array<RuntimeRequirementReason>, manualOverrides:Array<RuntimeFeatureOverride>, directDecisions:Array<String>,
			?noRuntimeEvidence:RuntimeNoRuntimeEvidence) {
		this.purpose = purpose;
		this.profile = profile;
		this.environment = environment;
		this.runtimePolicy = runtimePolicy;
		this.runtimePolicyProvenance = runtimePolicyProvenance;
		this.runtimeDiagnostics = runtimeDiagnostics;
		this.runtimeDiagnosticsProvenance = runtimeDiagnosticsProvenance;
		this.rootReasons = rootReasons.copy();
		this.manualOverrides = manualOverrides.copy();
		this.directDecisions = directDecisions.copy();
		this.noRuntimeEvidence = noRuntimeEvidence;
	}
}

typedef RuntimeSourcePointRecord = {
	final line:Int;
	final column:Int;
}

typedef RuntimeSourceSpanRecord = {
	final file:String;
	final start:RuntimeSourcePointRecord;
	final end:RuntimeSourcePointRecord;
}

typedef RuntimeReasonRecord = {
	final id:String;
	final featureId:String;
	final operationId:String;
	final kind:String;
	final surface:String;
	final source:RuntimeSourceSpanRecord;
	final alternative:Null<String>;
}

typedef RuntimeReachabilityRecord = {
	final modules:Int;
	final typeInstances:Int;
	final functions:Int;
	final blocks:Int;
	final instructions:Int;
	final cleanupActions:Int;
	final runtimeIntents:Int;
}

typedef RuntimeHxrtAbsenceRecord = {
	final features:Array<String>;
	final includes:Array<String>;
	final sources:Array<String>;
	final defines:Array<String>;
	final libraries:Array<String>;
	final symbols:Array<String>;
}

typedef RuntimeNoRuntimeProofRecord = {
	final schemaVersion:Int;
	final algorithm:String;
	final status:RuntimeNoRuntimeStatus;
	final scope:RuntimeNoRuntimeScope;
	final semanticProof:String;
	final reachability:RuntimeReachabilityRecord;
	final directDecisions:Array<String>;
	final programLocalHelpers:Array<String>;
	final runtimeAbsence:RuntimeHxrtAbsenceRecord;
}

typedef RuntimePolicyBlockerRecord = {
	final id:String;
	final featureId:String;
	final operationId:String;
	final kind:String;
	final surface:String;
	final source:RuntimeSourceSpanRecord;
	final alternative:Null<String>;
	final dependencyChains:Array<Array<String>>;
}

typedef RuntimeOverrideRecord = {
	final featureId:String;
	final action:RuntimeFeatureOverrideAction;
	final provenance:String;
}

typedef RuntimeDependencyEdgeRecord = {
	final featureId:String;
	final dependencyId:String;
	final reasonIds:Array<String>;
}

typedef RuntimeSelectedFeatureRecord = {
	final id:String;
	final availability:RuntimeFeatureAvailability;
	final root:Bool;
	final reasonIds:Array<String>;
	final dependencies:Array<String>;
	final artifacts:Array<String>;
	final symbols:Array<String>;
	final libraries:Array<String>;
	final defines:Array<String>;
}

typedef RuntimeArtifactRecord = {
	final featureId:String;
	final sourcePath:String;
	final outputPath:String;
	final kind:GeneratedFileKind;
}

/** Stable report consumed by project emission and future inspection. */
typedef RuntimeFeaturePlanSnapshot = {
	final schemaVersion:Int;
	final algorithm:String;
	final status:RuntimeFeaturePlanStatus;
	final planPurpose:RuntimePlanningPurpose;
	final profile:CProfile;
	final requestedPolicy:CRuntimePolicy;
	final resolvedPolicy:CRuntimePolicy;
	final policyProvenance:String;
	final diagnosticMode:CRuntimeDiagnostics;
	final diagnosticProvenance:String;
	final environment:CEnvironment;
	final rootReasons:Array<RuntimeReasonRecord>;
	final manualOverrides:Array<RuntimeOverrideRecord>;
	final directDecisions:Array<String>;
	final dependencyEdges:Array<RuntimeDependencyEdgeRecord>;
	final selectedFeatures:Array<RuntimeSelectedFeatureRecord>;
	final features:Array<String>;
	final artifactDetails:Array<RuntimeArtifactRecord>;
	final artifacts:Array<String>;
	final symbols:Array<String>;
	final libraries:Array<String>;
	final defines:Array<String>;
	final noRuntimeProof:Null<RuntimeNoRuntimeProofRecord>;
}

typedef RuntimeFeatureDefinitionRecord = {
	final id:String;
	final summary:String;
	final availability:RuntimeFeatureAvailability;
	final minimalAllowed:Bool;
	final environments:Array<CEnvironment>;
	final dependencies:Array<String>;
	final artifacts:Array<RuntimeFeatureArtifactRecord>;
	final symbols:Array<String>;
	final libraries:Array<String>;
	final defines:Array<String>;
}

typedef RuntimeFeatureArtifactRecord = {
	final sourcePath:String;
	final outputPath:String;
	final kind:GeneratedFileKind;
	final sourceSha256:String;
}

typedef RuntimeAbiVersionRecord = {
	final major:Int;
	final minor:Int;
	final patch:Int;
}

typedef RuntimePublicBoundaryRecord = {
	final applicationAbiStatus:RuntimeApplicationAbiStatus;
	final exportTypePolicy:RuntimePublicExportTypePolicy;
	final forbiddenRuntimeTypes:Array<String>;
}

typedef RuntimeReleaseProvenanceRecord = {
	final sourceHashAlgorithm:String;
	final sourceSetSha256:String;
	final runtimeCStandard:String;
	final publicHeaderCppStandard:String;
}

typedef RuntimeAbiContractRecord = {
	final stability:RuntimeAbiStability;
	final version:RuntimeAbiVersionRecord;
	final generatedCodeCompatibility:RuntimeAbiCompatibility;
	final generatedCodeCheck:RuntimeGeneratedAbiCheck;
	final runtimeMajorMacro:String;
	final publicBoundary:RuntimePublicBoundaryRecord;
	final releaseProvenance:RuntimeReleaseProvenanceRecord;
}

typedef RuntimeFeatureReservationRecord = {
	final id:String;
	final ownerTask:String;
	final summary:String;
}

typedef RuntimeFeatureCatalogSnapshot = {
	final schemaVersion:Int;
	final algorithm:String;
	final status:RuntimeFeatureCatalogStatus;
	final requirements:Array<String>;
	final noUnconditionalCore:Bool;
	final runtimeAbi:RuntimeAbiContractRecord;
	final compilerSelectableFeatures:Array<String>;
	final features:Array<RuntimeFeatureDefinitionRecord>;
	final reservedFeatures:Array<RuntimeFeatureReservationRecord>;
}

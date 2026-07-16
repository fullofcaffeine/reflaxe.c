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
}

enum abstract RuntimeFeatureOverrideAction(String) to String {
	var Require = "require";
	var Forbid = "forbid";
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

	public function new(sourcePath:String, outputPath:String, kind:GeneratedFileKind) {
		this.sourcePath = sourcePath;
		this.outputPath = outputPath;
		this.kind = kind;
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

/** One actionable semantic root; dependencies inherit this reason by ID. */
class RuntimeRequirementReason {
	public final id:String;
	public final featureId:RuntimeFeatureId;
	public final kind:String;
	public final surface:String;
	public final source:HxcSourceSpan;
	public final alternative:Null<String>;

	public function new(id:String, featureId:RuntimeFeatureId, kind:String, surface:String, source:HxcSourceSpan, ?alternative:String) {
		this.id = id;
		this.featureId = featureId;
		this.kind = kind;
		this.surface = surface;
		this.source = source;
		this.alternative = alternative;
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
	public final noRuntimeProof:Null<String>;

	public function new(purpose:RuntimePlanningPurpose, profile:CProfile, environment:CEnvironment, runtimePolicy:CRuntimePolicy,
			runtimePolicyProvenance:String, runtimeDiagnostics:CRuntimeDiagnostics, runtimeDiagnosticsProvenance:String,
			rootReasons:Array<RuntimeRequirementReason>, manualOverrides:Array<RuntimeFeatureOverride>, directDecisions:Array<String>, ?noRuntimeProof:String) {
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
		this.noRuntimeProof = noRuntimeProof;
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
	final kind:String;
	final surface:String;
	final source:RuntimeSourceSpanRecord;
	final alternative:Null<String>;
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
	final noRuntimeProof:Null<String>;
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
	final compilerSelectableFeatures:Array<String>;
	final features:Array<RuntimeFeatureDefinitionRecord>;
	final reservedFeatures:Array<RuntimeFeatureReservationRecord>;
}

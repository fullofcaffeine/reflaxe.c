import haxe.Json;
import haxe.PosInfos;
import sys.FileSystem;
import sys.io.File;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.runtime.RuntimeFeatureCatalog;
import reflaxe.c.runtime.RuntimeFeatureError;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureAvailability;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureArtifact;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefine;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinition;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureOverride;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureOverrideAction;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanSnapshot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningPurpose;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningRequest;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementReason;
import reflaxe.c.runtime.RuntimeFeaturePackager;
import reflaxe.c.runtime.RuntimeFeaturePackager.RuntimeArtifactSource;
import reflaxe.c.runtime.RuntimeFeaturePlanner;
import reflaxe.c.runtime.RuntimeFeatureRegistry;

private typedef RuntimePackageFileRecord = {
	final path:String;
	final kind:GeneratedFileKind;
	final sha256:String;
	final contents:String;
}

private typedef RuntimeFailureRecord = {
	final id:CDiagnosticId;
	final message:String;
	final featureIds:Array<String>;
}

class RuntimeFeatureGraphGolden {
	static inline final CATALOG_PREFIX = "HXC_RUNTIME_FEATURE_CATALOG=";
	static inline final PLANS_PREFIX = "HXC_RUNTIME_FEATURE_PLANS=";
	static inline final PACKAGE_PREFIX = "HXC_RUNTIME_FEATURE_PACKAGE=";

	static function main():Void {
		final registry = RuntimeFeatureCatalog.registry();
		final definitions = RuntimeFeatureCatalog.definitions();
		final reservations = RuntimeFeatureCatalog.reservations();
		definitions.reverse();
		reservations.reverse();
		final reversedRegistry = new RuntimeFeatureRegistry(definitions, reservations);
		if (Json.stringify(registry.catalogSnapshot()) != Json.stringify(reversedRegistry.catalogSnapshot())) {
			throw "runtime feature catalog changed with discovery order";
		}

		final planner = new RuntimeFeaturePlanner(registry);
		final empty = planner.plan(emptyRequest());
		final alloc = planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.alloc", "alloc")], []));
		final stringReason = reason("fixture.string", "string");
		final ownerAllocationReason = reason("fixture.owner-allocation", "alloc");
		final stringReasons = [stringReason, ownerAllocationReason];
		final stringPlan = planner.plan(featureRequest(CRuntimePolicy.Auto, stringReasons, []));
		stringReasons.reverse();
		final reversedString = planner.plan(featureRequest(CRuntimePolicy.Auto, stringReasons, []));
		if (Json.stringify(stringPlan) != Json.stringify(reversedString)) {
			throw "runtime plan changed with root-reason order";
		}
		final minimalString = planner.plan(featureRequest(CRuntimePolicy.Minimal, [reason("fixture.minimal-string", "string")], [
			new RuntimeFeatureOverride(RuntimeFeatureId.parse("string"), RuntimeFeatureOverrideAction.Require, "fixture:manual-confirmation")
		]));
		final syntheticLink = syntheticLinkPlan();

		final repositorySource = new RepositoryRuntimeSource();
		final packager = new RuntimeFeaturePackager(registry);
		final allocFiles = packager.packageFiles(alloc, repositorySource);
		final stringFiles = packager.packageFiles(stringPlan, repositorySource);
		final emptySource = new CountingRuntimeSource();
		final emptyFiles = packager.packageFiles(empty, emptySource);
		if (emptyFiles.length != 0 || emptySource.readCount != 0) {
			throw "runtime-free packaging consulted an hxrt artifact";
		}
		final originalArtifact = alloc.artifactDetails[0];
		alloc.artifactDetails[0] = {
			featureId: originalArtifact.featureId,
			sourcePath: "runtime/hxrt/include/hxrt/string.h",
			outputPath: originalArtifact.outputPath,
			kind: originalArtifact.kind
		};
		final tamperedSource = new CountingRuntimeSource();
		final tamperedPackage = expectFailure(() -> {
			packager.packageFiles(alloc, tamperedSource);
		});
		alloc.artifactDetails[0] = originalArtifact;
		if (tamperedSource.readCount != 0) {
			throw "tampered runtime packaging consulted an artifact source";
		}

		Sys.println(CATALOG_PREFIX + Json.stringify(registry.catalogSnapshot()));
		Sys.println(PLANS_PREFIX + Json.stringify({
			empty: empty,
			alloc: alloc,
			string: stringPlan,
			minimalString: minimalString,
			syntheticLink: syntheticLink,
			diagnostics: {
				invalidId: expectFailure(() -> {
					RuntimeFeatureId.parse("bad_id");
				}),
				cycle: cycleFailure(),
				unknownDependency: unknownDependencyFailure(),
				nonePolicy: expectFailure(() -> planner.plan(featureRequest(CRuntimePolicy.None, [reason("fixture.none", "alloc")], []))),
				minimalPolicy: minimalPolicyFailure(),
				unusedManualRequire: expectFailure(() -> planner.plan(new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Portable,
					CEnvironment.Hosted, CRuntimePolicy.Auto, "fixture:auto", CRuntimeDiagnostics.Off, "fixture:off", [], [
						new RuntimeFeatureOverride(RuntimeFeatureId.parse("string"), RuntimeFeatureOverrideAction.Require, "fixture:unused")
					],
					["direct-c-fixture"], "no semantic runtime requirement"))),
				forbidRequired: expectFailure(() -> planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.forbid", "string")],
					[
						new RuntimeFeatureOverride(RuntimeFeatureId.parse("alloc"), RuntimeFeatureOverrideAction.Forbid, "fixture:forbid")
					]))),
				compilerSeed: expectFailure(() -> planner.plan(compilerFeatureRequest())),
				reservedFeature: expectFailure(() -> planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.reflection", "reflection")], []))),
				environment: expectFailure(() -> planner.plan(new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Portable,
					CEnvironment.Wasi, CRuntimePolicy.Auto, "fixture:auto", CRuntimeDiagnostics.Off, "fixture:off", [reason("fixture.wasi", "alloc")], [],
					["direct-c-fixture"]))),
				tamperedPackage: tamperedPackage
			}
		}));
		Sys.println(PACKAGE_PREFIX + Json.stringify({
			alloc: packageRecords(allocFiles),
			string: packageRecords(stringFiles)
		}));
	}

	static function emptyRequest():RuntimePlanningRequest {
		return new RuntimePlanningRequest(RuntimePlanningPurpose.CompilerProgram, CProfile.Portable, CEnvironment.Hosted, CRuntimePolicy.Auto,
			"profile-preset:portable", CRuntimeDiagnostics.Summary, "profile-preset:portable", [], [],
			["primitive-values", "program-local-specialization-considered"], "reachable fixture semantics use only direct C and program-local specialization");
	}

	static function featureRequest(policy:CRuntimePolicy, reasons:Array<RuntimeRequirementReason>,
			overrides:Array<RuntimeFeatureOverride>):RuntimePlanningRequest {
		return new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Portable, CEnvironment.Hosted, policy, "fixture:explicit-policy",
			CRuntimeDiagnostics.Off, "fixture:diagnostics-off", reasons, overrides, ["direct-c-considered", "program-local-specialization-considered"]);
	}

	static function compilerFeatureRequest():RuntimePlanningRequest {
		return new RuntimePlanningRequest(RuntimePlanningPurpose.CompilerProgram, CProfile.Portable, CEnvironment.Hosted, CRuntimePolicy.Auto,
			"profile-preset:portable", CRuntimeDiagnostics.Summary, "profile-preset:portable", [reason("fixture.compiler", "alloc")], [],
			["direct-c-considered"]);
	}

	static function reason(id:String, featureId:String, ?source:PosInfos):RuntimeRequirementReason {
		final anchor = switch source {
			case null: throw "runtime reason fixture lost its Haxe source anchor";
			case value: value;
		};
		return new RuntimeRequirementReason(id, RuntimeFeatureId.parse(featureId), "fixture-semantic-gap", "fixture.RuntimeFeatureGraph",
			new HxcSourceSpan(StringTools.replace(anchor.fileName, "\\", "/"), anchor.lineNumber, 1, anchor.lineNumber, 2),
			"use a direct fixture representation when it preserves semantics");
	}

	static function syntheticLinkPlan():RuntimeFeaturePlanSnapshot {
		final feature = new RuntimeFeatureDefinition(RuntimeFeatureId.parse("fixture-link"), "Synthetic link and define selection proof.",
			RuntimeFeatureAvailability.NativeSeedOnly, true, [CEnvironment.Hosted], [], [], ["hxc_fixture_link"], ["m"],
			[new RuntimeFeatureDefine("HXC_FIXTURE_LINK", "1")]);
		return new RuntimeFeaturePlanner(new RuntimeFeatureRegistry([feature],
			[])).plan(new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Portable, CEnvironment.Hosted, CRuntimePolicy.Auto,
				"fixture:auto", CRuntimeDiagnostics.Off, "fixture:off", [reason("fixture.link", "fixture-link")], [], ["direct-c-considered"]));
	}

	static function minimalPolicyFailure():RuntimeFailureRecord {
		final feature = new RuntimeFeatureDefinition(RuntimeFeatureId.parse("fixture-broad"), "Synthetic broad feature.", NativeSeedOnly, false,
			[CEnvironment.Hosted], [], [], [], [], []);
		final planner = new RuntimeFeaturePlanner(new RuntimeFeatureRegistry([feature], []));
		return expectFailure(() -> planner.plan(new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Metal, CEnvironment.Hosted,
			CRuntimePolicy.Minimal, "profile-preset:metal", CRuntimeDiagnostics.Warn, "profile-preset:metal", [reason("fixture.broad", "fixture-broad")], [],
			["direct-c-considered"])));
	}

	static function cycleFailure():RuntimeFailureRecord {
		final first = syntheticFeature("cycle-a", ["cycle-b"]);
		final second = syntheticFeature("cycle-b", ["cycle-a"]);
		final forward = expectFailure(() -> new RuntimeFeatureRegistry([first, second], []));
		final reverse = expectFailure(() -> new RuntimeFeatureRegistry([second, first], []));
		if (Json.stringify(forward) != Json.stringify(reverse)) {
			throw "runtime feature cycle diagnostic changed with discovery order";
		}
		return forward;
	}

	static function unknownDependencyFailure():RuntimeFailureRecord {
		final feature = syntheticFeature("known-feature", ["missing-feature"]);
		return expectFailure(() -> new RuntimeFeatureRegistry([feature], []));
	}

	static function syntheticFeature(id:String, dependencies:Array<String>):RuntimeFeatureDefinition {
		return new RuntimeFeatureDefinition(RuntimeFeatureId.parse(id), 'Synthetic `$id` graph fixture.', NativeSeedOnly, true, [CEnvironment.Hosted],
			dependencies.map(RuntimeFeatureId.parse), [], [], [], []);
	}

	static function expectFailure(operation:() -> Void):RuntimeFailureRecord {
		try {
			operation();
		} catch (error:RuntimeFeatureError) {
			return {id: error.diagnosticId, message: error.message, featureIds: error.featureIds};
		}
		throw "runtime feature negative fixture unexpectedly succeeded";
	}

	static function packageRecords(files:Array<GeneratedFile>):Array<RuntimePackageFileRecord> {
		return files.map(file -> {
			path: file.relativePath,
			kind: file.kind,
			sha256: file.contentSha256,
			contents: file.contents
		});
	}
}

private class RepositoryRuntimeSource implements RuntimeArtifactSource {
	public function new() {}

	public function read(sourcePath:String):Null<String>
		return FileSystem.exists(sourcePath) && !FileSystem.isDirectory(sourcePath) ? File.getContent(sourcePath) : null;
}

private class CountingRuntimeSource implements RuntimeArtifactSource {
	public var readCount(default, null):Int = 0;

	public function new() {}

	public function read(sourcePath:String):Null<String> {
		readCount++;
		return null;
	}
}

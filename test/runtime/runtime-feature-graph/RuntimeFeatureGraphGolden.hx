import haxe.Json;
import haxe.PosInfos;
import sys.FileSystem;
import sys.io.File;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.CObjectDescriptorEmitter;
import reflaxe.c.emit.CObjectDescriptorEmitter.CObjectDescriptorEmissionError;
import reflaxe.c.emit.CObjectDescriptorEmitter.CObjectDescriptorSpec;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.runtime.RuntimeFeatureCatalog;
import reflaxe.c.runtime.RuntimeFeatureError;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureAvailability;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureArtifact;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefine;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinition;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDocumentation;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureOverride;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureOverrideAction;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanSnapshot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeNoRuntimeEvidence;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeNoRuntimeScope;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningPurpose;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningRequest;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePolicyBlockerRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeReachabilityEvidence;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementReason;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureSelectionRoot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureSelectionRootKind;
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
	final blockers:Array<RuntimePolicyBlockerRecord>;
}

class RuntimeFeatureGraphGolden {
	static inline final CATALOG_PREFIX = "HXC_RUNTIME_FEATURE_CATALOG=";
	static inline final PLANS_PREFIX = "HXC_RUNTIME_FEATURE_PLANS=";
	static inline final PACKAGE_PREFIX = "HXC_RUNTIME_FEATURE_PACKAGE=";

	static function main():Void {
		verifyObjectDescriptorEmission();
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
		final array = planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.array", "array")], []));
		final intMap = planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.int-map", "int-map")], []));
		final stringMap = planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.string-map", "string-map")], []));
		final bytes = planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.bytes", "bytes")], []));
		final objectPlan = planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.object", "object")], []));
		final gc = planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.gc", "gc")], []));
		final stringScalar = planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.string-scalar", "string-scalar")], []));
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
		final compilerIo = planner.plan(compilerFeatureRequest());
		final syntheticLink = syntheticLinkPlan();

		final repositorySource = new RepositoryRuntimeSource();
		final packager = new RuntimeFeaturePackager(registry);
		final allocFiles = packager.packageFiles(alloc, repositorySource);
		final arrayFiles = packager.packageFiles(array, repositorySource);
		final intMapFiles = packager.packageFiles(intMap, repositorySource);
		final stringMapFiles = packager.packageFiles(stringMap, repositorySource);
		final bytesFiles = packager.packageFiles(bytes, repositorySource);
		final objectFiles = packager.packageFiles(objectPlan, repositorySource);
		final gcFiles = packager.packageFiles(gc, repositorySource);
		final stringScalarFiles = packager.packageFiles(stringScalar, repositorySource);
		final stringFiles = packager.packageFiles(stringPlan, repositorySource);
		final ioFiles = packager.packageFiles(compilerIo, repositorySource);
		final emptySource = new CountingRuntimeSource();
		final emptyFiles = packager.packageFiles(empty, emptySource);
		if (emptyFiles.length != 0 || emptySource.readCount != 0) {
			throw "runtime-free packaging consulted an hxrt artifact";
		}
		final emptyProof = empty.noRuntimeProof;
		if (emptyProof == null) {
			throw "runtime-free plan lost its structured proof";
		}
		emptyProof.runtimeAbsence.symbols.push("hxc_tampered_runtime_symbol");
		final tamperedEmptySource = new CountingRuntimeSource();
		final tamperedNoRuntimeProof = expectFailure(() -> {
			packager.packageFiles(empty, tamperedEmptySource);
		});
		emptyProof.runtimeAbsence.symbols.pop();
		if (tamperedEmptySource.readCount != 0) {
			throw "tampered no-runtime proof consulted an artifact source";
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
		final tamperedSourceContent = expectFailure(() -> {
			packager.packageFiles(alloc, new TamperedRuntimeSource());
		});

		Sys.println(CATALOG_PREFIX + Json.stringify(registry.catalogSnapshot()));
		Sys.println(PLANS_PREFIX + Json.stringify({
			empty: empty,
			alloc: alloc,
			array: array,
			intMap: intMap,
			stringMap: stringMap,
			bytes: bytes,
			object: objectPlan,
			gc: gc,
			stringScalar: stringScalar,
			string: stringPlan,
			minimalString: minimalString,
			compilerIo: compilerIo,
			syntheticLink: syntheticLink,
			diagnostics: {
				invalidId: expectFailure(() -> {
					RuntimeFeatureId.parse("bad_id");
				}),
				cycle: cycleFailure(),
				unknownDependency: unknownDependencyFailure(),
				missingNoRuntimeProof: expectFailure(() -> planner.plan(new RuntimePlanningRequest(RuntimePlanningPurpose.CompilerProgram, CProfile.Portable,
					CEnvironment.Hosted, CRuntimePolicy.None, "fixture:none", CRuntimeDiagnostics.Off, "fixture:off", [], [], ["direct-c-fixture"]))),
				survivingRuntimeIntentProof: survivingRuntimeIntentProof(planner),
				nonePolicy: nonePolicyFailure(planner),
				minimalPolicy: minimalPolicyFailure(),
				unusedManualRequire: expectFailure(() -> planner.plan(new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Portable,
					CEnvironment.Hosted, CRuntimePolicy.Auto, "fixture:auto", CRuntimeDiagnostics.Off, "fixture:off", [], [
						new RuntimeFeatureOverride(RuntimeFeatureId.parse("string"), RuntimeFeatureOverrideAction.Require, "fixture:unused")
					],
					["direct-c-fixture"],
					new RuntimeNoRuntimeEvidence(RuntimeNoRuntimeScope.NativeSeedFixture, "no semantic runtime requirement",
						new RuntimeReachabilityEvidence(1, 0, 1, 1, 1, 0, 0), [])))),
				forbidRequired: expectFailure(() -> planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.forbid", "string")],
					[
						new RuntimeFeatureOverride(RuntimeFeatureId.parse("alloc"), RuntimeFeatureOverrideAction.Forbid, "fixture:forbid")
					]))),
				compilerNativeSeed: compilerNativeSeedFailure(),
				reservedFeature: expectFailure(() -> planner.plan(featureRequest(CRuntimePolicy.Auto, [reason("fixture.reflection", "reflection")], []))),
				environment: expectFailure(() -> planner.plan(new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Portable,
					CEnvironment.Wasi, CRuntimePolicy.Auto, "fixture:auto", CRuntimeDiagnostics.Off, "fixture:off", [reason("fixture.wasi", "alloc")], [],
					["direct-c-fixture"]))),
				tamperedPackage: tamperedPackage,
				tamperedSourceContent: tamperedSourceContent,
				tamperedNoRuntimeProof: tamperedNoRuntimeProof
			}
		}));
		Sys.println(PACKAGE_PREFIX + Json.stringify({
			alloc: packageRecords(allocFiles),
			array: packageRecords(arrayFiles),
			intMap: packageRecords(intMapFiles),
			stringMap: packageRecords(stringMapFiles),
			bytes: packageRecords(bytesFiles),
			object: packageRecords(objectFiles),
			gc: packageRecords(gcFiles),
			stringScalar: packageRecords(stringScalarFiles),
			string: packageRecords(stringFiles),
			io: packageRecords(ioFiles)
		}));
	}

	/** Prove that only the selected, finalized descriptor plans reach C syntax. */
	static function verifyObjectDescriptorEmission():Void {
		final emitter = new CObjectDescriptorEmitter();
		if (emitter.declarations([]).length != 0)
			throw "empty object descriptor plan emitted C declarations";
		final leaf = descriptorSpec("fixture.leaf", "hxc_descriptor_leaf", "fixture_leaf", null, null);
		final node = descriptorSpec("fixture.node", "hxc_descriptor_node", "fixture_node", "fixture_trace", "fixture_finalize");
		final declarations = emitter.declarations([node, leaf]);
		if (declarations.length != 4)
			throw "selected object descriptor plan did not emit one assertion and definition per payload";
		final printer = new CASTPrinter();
		final rendered = declarations.map(printer.printDecl).join("\n");
		final leafOffset = rendered.indexOf("hxc_descriptor_leaf");
		final nodeOffset = rendered.indexOf("hxc_descriptor_node");
		if (leafOffset == -1 || nodeOffset == -1 || leafOffset >= nodeOffset)
			throw "object descriptor C emission changed with input discovery order";
		for (required in [
			"static const struct hxc_type_descriptor",
			".abi_version = HXC_TYPE_DESCRIPTOR_ABI_VERSION",
			".object_size = sizeof(struct fixture_node)",
			".object_alignment = _Alignof(struct fixture_node)",
			"HXC_TYPE_DESCRIPTOR_HAS_TRACE | HXC_TYPE_DESCRIPTOR_HAS_FINALIZER",
			".trace = fixture_trace",
			".finalize = fixture_finalize"
		])
			if (rendered.indexOf(required) == -1)
				throw 'object descriptor C emission omitted `$required`';
		expectObjectDescriptorFailure(() -> emitter.declarations([leaf, leaf]));
		final duplicateName = descriptorSpec("fixture.other", "hxc_descriptor_leaf", "fixture_leaf", null, null);
		expectObjectDescriptorFailure(() -> emitter.declarations([leaf, duplicateName]));
	}

	static function descriptorSpec(id:String, descriptorName:String, payloadName:String, traceName:Null<String>,
			finalizerName:Null<String>):CObjectDescriptorSpec {
		return new CObjectDescriptorSpec(id, new CIdentifier(descriptorName), {
			type: new CType(TStruct(new CIdentifier(payloadName))),
			declarator: DName(null)
		},
			traceName == null ? null : new CIdentifier(traceName), finalizerName == null ? null : new CIdentifier(finalizerName));
	}

	static function expectObjectDescriptorFailure(action:() -> Void):Void {
		try {
			action();
		} catch (_:CObjectDescriptorEmissionError) {
			return;
		}
		throw "malformed object descriptor plan was accepted";
	}

	static function emptyRequest():RuntimePlanningRequest {
		return new RuntimePlanningRequest(RuntimePlanningPurpose.CompilerProgram, CProfile.Portable, CEnvironment.Hosted, CRuntimePolicy.Auto,
			"profile-preset:portable", CRuntimeDiagnostics.Summary, "profile-preset:portable", [], [],
			["primitive-values", "program-local-specialization-considered"],
			new RuntimeNoRuntimeEvidence(RuntimeNoRuntimeScope.ReachableWholeProgram,
				"reachable fixture semantics use only direct C and program-local specialization", new RuntimeReachabilityEvidence(1, 0, 1, 1, 1, 0, 0), []));
	}

	static function featureRequest(policy:CRuntimePolicy, reasons:Array<RuntimeRequirementReason>,
			overrides:Array<RuntimeFeatureOverride>):RuntimePlanningRequest {
		return new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Portable, CEnvironment.Hosted, policy, "fixture:explicit-policy",
			CRuntimeDiagnostics.Off, "fixture:diagnostics-off", reasons, overrides, ["direct-c-considered", "program-local-specialization-considered"]);
	}

	static function compilerFeatureRequest():RuntimePlanningRequest {
		return new RuntimePlanningRequest(RuntimePlanningPurpose.CompilerProgram, CProfile.Portable, CEnvironment.Hosted, CRuntimePolicy.Auto,
			"profile-preset:portable", CRuntimeDiagnostics.Summary, "profile-preset:portable", [reason("fixture.compiler-io", "io")], [],
			["direct-utf8-string-literal", "program-local-output-considered"]);
	}

	static function compilerNativeSeedFailure():RuntimeFailureRecord {
		final feature = new RuntimeFeatureDefinition(RuntimeFeatureId.parse("fixture-native-seed"), "Synthetic native-only feature.", NativeSeedOnly, true,
			[CEnvironment.Hosted], [], [], [], [], [], fixtureDocumentation());
		final planner = new RuntimeFeaturePlanner(new RuntimeFeatureRegistry([feature], []));
		return expectFailure(() -> planner.plan(new RuntimePlanningRequest(RuntimePlanningPurpose.CompilerProgram, CProfile.Portable, CEnvironment.Hosted,
			CRuntimePolicy.Auto, "fixture:auto", CRuntimeDiagnostics.Off, "fixture:off", [reason("fixture.compiler-native-seed", "fixture-native-seed")], [],
			["direct-c-considered"])));
	}

	static function reason(id:String, featureId:String, ?source:PosInfos):RuntimeRequirementReason {
		final anchor = switch source {
			case null: throw "runtime reason fixture lost its Haxe source anchor";
			case value: value;
		};
		return new RuntimeRequirementReason(id, RuntimeFeatureId.parse(featureId), "fixture-runtime-operation", "fixture-semantic-gap",
			"fixture.RuntimeFeatureGraph", new HxcSourceSpan(StringTools.replace(anchor.fileName, "\\", "/"), anchor.lineNumber, 1, anchor.lineNumber, 2),
			"use a direct fixture representation when it preserves semantics");
	}

	static function syntheticLinkPlan():RuntimeFeaturePlanSnapshot {
		final feature = new RuntimeFeatureDefinition(RuntimeFeatureId.parse("fixture-link"), "Synthetic link and define selection proof.",
			RuntimeFeatureAvailability.NativeSeedOnly, true, [CEnvironment.Hosted], [], [], ["hxc_fixture_link"], ["m"],
			[new RuntimeFeatureDefine("HXC_FIXTURE_LINK", "1")], fixtureDocumentation());
		return new RuntimeFeaturePlanner(new RuntimeFeatureRegistry([feature],
			[])).plan(new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Portable, CEnvironment.Hosted, CRuntimePolicy.Auto,
				"fixture:auto", CRuntimeDiagnostics.Off, "fixture:off", [reason("fixture.link", "fixture-link")], [], ["direct-c-considered"]));
	}

	static function minimalPolicyFailure():RuntimeFailureRecord {
		final feature = new RuntimeFeatureDefinition(RuntimeFeatureId.parse("fixture-broad"), "Synthetic broad feature.", NativeSeedOnly, false,
			[CEnvironment.Hosted], [], [], [], [], [], fixtureDocumentation());
		final planner = new RuntimeFeaturePlanner(new RuntimeFeatureRegistry([feature], []));
		return expectFailure(() -> planner.plan(new RuntimePlanningRequest(RuntimePlanningPurpose.NativeSeedFixture, CProfile.Metal, CEnvironment.Hosted,
			CRuntimePolicy.Minimal, "profile-preset:metal", CRuntimeDiagnostics.Warn, "profile-preset:metal", [reason("fixture.broad", "fixture-broad")], [],
			["direct-c-considered"])));
	}

	static function nonePolicyFailure(planner:RuntimeFeaturePlanner):RuntimeFailureRecord {
		final reasons = [reason("fixture.none.z", "alloc"), reason("fixture.none.a", "string")];
		final forward = expectFailure(() -> planner.plan(featureRequest(CRuntimePolicy.None, reasons, [])));
		reasons.reverse();
		final reverse = expectFailure(() -> planner.plan(featureRequest(CRuntimePolicy.None, reasons, [])));
		if (Json.stringify(forward) != Json.stringify(reverse)) {
			throw "runtime-none blockers changed with root discovery order";
		}
		return forward;
	}

	static function survivingRuntimeIntentProof(planner:RuntimeFeaturePlanner):RuntimeFailureRecord {
		return expectFailure(() -> planner.plan(new RuntimePlanningRequest(RuntimePlanningPurpose.CompilerProgram, CProfile.Portable, CEnvironment.Hosted,
			CRuntimePolicy.None, "fixture:none", CRuntimeDiagnostics.Off, "fixture:off", [], [], ["direct-c-fixture"],
			new RuntimeNoRuntimeEvidence(RuntimeNoRuntimeScope.ReachableWholeProgram, "invalid surviving runtime intent",
				new RuntimeReachabilityEvidence(1, 0, 1, 1, 1, 0, 1), []))));
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
			dependencies.map(RuntimeFeatureId.parse), [], [], [], [], fixtureDocumentation());
	}

	static function fixtureDocumentation():RuntimeFeatureDocumentation
		return new RuntimeFeatureDocumentation("Synthetic feature used only to test graph validation.", [
			new RuntimeFeatureSelectionRoot("native-seed-fixture", RuntimeFeatureSelectionRootKind.NativeSeedFixture,
				"Selected only by this synthetic native fixture.")
		],
			"The fixture records that direct C was considered.", "The fixture records that local specialization was considered.",
			"A synthetic feature is required to exercise the graph branch.", "docs/hxrt.md", ["test/runtime/runtime-feature-graph/run.py"]);

	static function expectFailure(operation:() -> Void):RuntimeFailureRecord {
		try {
			operation();
		} catch (error:RuntimeFeatureError) {
			return {
				id: error.diagnosticId,
				message: error.message,
				featureIds: error.featureIds,
				blockers: error.blockers
			};
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

private class TamperedRuntimeSource implements RuntimeArtifactSource {
	public function new() {}

	public function read(sourcePath:String):Null<String> {
		return FileSystem.exists(sourcePath)
			&& !FileSystem.isDirectory(sourcePath) ? File.getContent(sourcePath) + "\n/* tampered fixture */\n" : null;
	}
}

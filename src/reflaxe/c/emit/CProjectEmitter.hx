package reflaxe.c.emit;

import haxe.Json;
import haxe.io.Bytes;
import reflaxe.c.CProfile;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;

/** Input status distinguishes direct structural fixtures from future lowered programs. */
enum abstract CProjectCompilationStatus(String) to String {
	var StructuralFixture = "structural-fixture-no-haxe-lowering";
	var LoweredProgram = "lowered-program";
}

enum abstract CProjectEnvironment(String) to String {
	var Hosted = "hosted";
	var Freestanding = "freestanding";
	var Wasi = "wasi";
	var Emscripten = "emscripten";
}

enum abstract CProjectStandard(String) to String {
	var C11 = "c11";
	var C17 = "c17";
	var C23Experimental = "c23-experimental";
}

enum abstract CProjectRuntimePolicy(String) to String {
	var Auto = "auto";
	var Minimal = "minimal";
	var None = "none";
}

enum abstract CProjectRuntimeDiagnostics(String) to String {
	var Off = "off";
	var Summary = "summary";
	var Warn = "warn";
}

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
	final units:Array<GeneratedFile>;
	final buildFacts:Array<TypedCBuildFact>;
	final symbolTable:CSymbolTableSnapshot;
}

private enum abstract BuildFactKind(String) to String {
	var Include = "include";
	var Link = "link";
	var PkgConfig = "pkg-config";
	var Framework = "framework";
	var Define = "define";
}

private enum abstract BuildFactValueKind(String) to String {
	var EnumValue = "enum";
	var StringValue = "string";
	var IntegerValue = "integer";
	var FloatValue = "float";
	var BooleanValue = "boolean";
}

private enum abstract PlaceholderStatus(String) to String {
	var RuntimeNotAnalyzed = "placeholder-no-runtime-analysis";
	var AbiNotAnalyzed = "placeholder-no-export-analysis";
	var StdlibNotAnalyzed = "placeholder-no-stdlib-analysis";
}

private enum abstract AbiStability(String) to String {
	var Experimental = "experimental";
}

private typedef CanonicalBuildFact = {
	final kind:BuildFactKind;
	final name:String;
	final value:Null<String>;
	final valueKind:Null<BuildFactValueKind>;
	final ownerModulePaths:Array<String>;
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

private typedef ProjectBuildMetadata = {
	final sources:Array<String>;
	final publicHeaders:Array<String>;
	final privateHeaders:Array<String>;
	final runtimeHeaders:Array<String>;
	final includeDirectories:Array<String>;
	final requirements:Array<CanonicalBuildFact>;
}

private typedef CompilerManifestRecord = {
	final schemaVersion:Int;
	final generator:String;
	final projectName:String;
	final compilationStatus:CProjectCompilationStatus;
	final configuration:ProjectConfigurationRecord;
	final contentAddressing:ContentAddressingRecord;
	final artifacts:Array<AddressedArtifact>;
	final build:ProjectBuildMetadata;
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

private typedef StdlibPlaceholder = {
	final schemaVersion:Int;
	final status:PlaceholderStatus;
	final profile:CProfile;
	final modules:Array<String>;
	final capabilities:Array<String>;
}

/** Pure schema-1 project emitter. Filesystem ownership is handled separately. */
class CProjectEmitter {
	public static inline final SCHEMA_VERSION = 1;

	static final SIDECAR_PATHS = [
		"hxc.abi.json",
		"hxc.manifest.json",
		"hxc.runtime-plan.json",
		"hxc.stdlib-report.json",
		"hxc.symbols.json"
	];

	public function new() {}

	public function emit(plan:CProjectEmissionPlan):Array<GeneratedFile> {
		validatePlan(plan);
		final units = canonicalUnits(plan.units);
		final buildFacts = canonicalBuildFacts(plan.buildFacts);
		final files = units.copy();

		files.push(jsonFile("hxc.symbols.json", GeneratedFileKind.SymbolTable, plan.symbolTable));
		files.push(jsonFile("hxc.runtime-plan.json", GeneratedFileKind.RuntimePlan, runtimePlan(plan)));
		files.push(jsonFile("hxc.abi.json", GeneratedFileKind.AbiManifest, abiPlaceholder(plan)));
		files.push(jsonFile("hxc.stdlib-report.json", GeneratedFileKind.StdlibReport, stdlibPlaceholder(plan)));

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
			build: buildMetadata(units, buildFacts),
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

	function canonicalBuildFacts(input:Array<TypedCBuildFact>):Array<CanonicalBuildFact> {
		final byKey:Map<String, CanonicalBuildFact> = [];
		for (fact in input) {
			final kind = buildFactKind(fact.kind);
			validateLogicalText(fact.name, 'build fact `${fact.kind}` name');
			if (kind == BuildFactKind.Include && !GeneratedFile.isNormalizedRelativePath(fact.name)) {
				fail('include build fact must use a normalized logical path: `${fact.name}`');
			}
			if (fact.value != null) {
				validateLogicalText(fact.value, 'build fact `${fact.kind}` value');
			}
			if (fact.valueKind != null) {
				validateLogicalText(fact.valueKind, 'build fact `${fact.kind}` value kind');
			}
			final valueKind = buildFactValueKind(kind, fact.value, fact.valueKind);
			final key = factKey(kind, fact.name, fact.value, valueKind);
			var canonical = byKey.get(key);
			if (canonical == null) {
				canonical = {
					kind: kind,
					name: fact.name,
					value: fact.value,
					valueKind: valueKind,
					ownerModulePaths: []
				};
				byKey.set(key, canonical);
			}
			if (fact.ownerModulePaths.length == 0) {
				fail('build fact `${fact.kind}` `${fact.name}` has no declaration provenance');
			}
			for (owner in fact.ownerModulePaths) {
				validateLogicalText(owner, 'build fact `${fact.kind}` owner');
				if (owner.indexOf("/") != -1 || owner.indexOf("\\") != -1 || ~/^[A-Za-z]:/.match(owner) || owner.split(".").indexOf("") != -1) {
					fail('build fact owner must be a logical module identity: `$owner`');
				}
				if (canonical.ownerModulePaths.indexOf(owner) == -1) {
					canonical.ownerModulePaths.push(owner);
				}
			}
		}
		final keys = [for (key in byKey.keys()) key];
		keys.sort(compareUtf8);
		final result:Array<CanonicalBuildFact> = [];
		for (key in keys) {
			final fact = byKey.get(key);
			if (fact != null) {
				fact.ownerModulePaths.sort(compareUtf8);
				result.push(fact);
			}
		}
		return result;
	}

	function buildFactKind(value:String):BuildFactKind {
		return switch value {
			case "include": BuildFactKind.Include;
			case "link": BuildFactKind.Link;
			case "pkg-config": BuildFactKind.PkgConfig;
			case "framework": BuildFactKind.Framework;
			case "define": BuildFactKind.Define;
			case _: throw new ProjectEmissionError('unknown typed build fact kind `$value`');
		};
	}

	function buildFactValueKind(kind:BuildFactKind, value:Null<String>, rawKind:Null<String>):Null<BuildFactValueKind> {
		return switch kind {
			case BuildFactKind.Include:
				if (rawKind != "enum" || (value != "system" && value != "local")) {
					throw new ProjectEmissionError("include build facts must carry c.IncludeKind.System or c.IncludeKind.Local");
				}
				BuildFactValueKind.EnumValue;
			case BuildFactKind.Define:
				if (value == null) {
					throw new ProjectEmissionError("define build facts require a literal value");
				}
				switch rawKind {
					case "string": BuildFactValueKind.StringValue;
					case "integer": BuildFactValueKind.IntegerValue;
					case "float": BuildFactValueKind.FloatValue;
					case "boolean": BuildFactValueKind.BooleanValue;
					case _: throw new ProjectEmissionError('define build fact has unknown literal kind `${Std.string(rawKind)}`');
				}
			case BuildFactKind.Link | BuildFactKind.PkgConfig | BuildFactKind.Framework:
				if (value != null || rawKind != null) {
					throw new ProjectEmissionError('`${Std.string(kind)}` build facts must not carry a value');
				}
				null;
		};
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

	function runtimePlan(plan:CProjectEmissionPlan):RuntimePlanPlaceholder {
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

	function stdlibPlaceholder(plan:CProjectEmissionPlan):StdlibPlaceholder {
		return {
			schemaVersion: SCHEMA_VERSION,
			status: PlaceholderStatus.StdlibNotAnalyzed,
			profile: plan.profile,
			modules: [],
			capabilities: []
		};
	}

	function buildMetadata(units:Array<GeneratedFile>, facts:Array<CanonicalBuildFact>):ProjectBuildMetadata {
		final sources = pathsOfKinds(units, [GeneratedFileKind.Source, GeneratedFileKind.RuntimeSource]);
		final publicHeaders = pathsOfKinds(units, [GeneratedFileKind.PublicHeader]);
		final privateHeaders = pathsOfKinds(units, [GeneratedFileKind.PrivateHeader]);
		final runtimeHeaders = pathsOfKinds(units, [GeneratedFileKind.RuntimeHeader]);
		final includeDirectories:Array<String> = [];
		if (publicHeaders.length > 0 || privateHeaders.length > 0) {
			includeDirectories.push("include");
		}
		if (runtimeHeaders.length > 0) {
			includeDirectories.push("runtime/include");
		}
		return {
			sources: sources,
			publicHeaders: publicHeaders,
			privateHeaders: privateHeaders,
			runtimeHeaders: runtimeHeaders,
			includeDirectories: includeDirectories,
			requirements: facts
		};
	}

	function pathsOfKinds(files:Array<GeneratedFile>, kinds:Array<GeneratedFileKind>):Array<String> {
		final result = [];
		for (file in files) {
			if (kinds.indexOf(file.kind) != -1) {
				result.push(file.relativePath);
			}
		}
		result.sort(compareUtf8);
		return result;
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

	static function factKey(kind:BuildFactKind, name:String, value:Null<String>, valueKind:Null<BuildFactValueKind>):String
		return canonicalPart(Std.string(kind))
			+ canonicalPart(name)
			+ canonicalPart(value == null ? "" : value)
			+ canonicalPart(valueKind == null ? "" : Std.string(valueKind));

	static function canonicalPart(value:String):String
		return '${Bytes.ofString(value).length}:$value';

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

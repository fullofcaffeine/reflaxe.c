package hxc.config;

import hxc.config.HxcJsonValue.HxcJsonField;
import hxc.config.HxcJsonValue.HxcJsonNode;
import hxc.config.HxcProjectConfig.HxcNamedConfigOverlay;
import reflaxe.c.CBuildMode;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;

typedef HxcConfigEnumContract = {
	final artifact:Array<String>;
	final build:Array<String>;
	final cExtensions:Array<String>;
	final cStandard:Array<String>;
	final projectLayout:Array<String>;
	final environment:Array<String>;
	final profile:Array<String>;
	final runtime:Array<String>;
	final runtimeDiagnostics:Array<String>;
}

typedef HxcConfigParserContract = {
	final schemaVersion:Int;
	final schemaId:String;
	final rootKeys:Array<String>;
	final overlayKeys:Array<String>;
	final enums:HxcConfigEnumContract;
}

/** Converts the closed JSON tree into the strict schema-1 project model. */
class HxcConfigParser {
	public static function parse(text:String, source:String = "hxc.json"):HxcProjectConfig {
		final root = new HxcJsonParser(text, source).parse();
		final fields = objectFields(root, "configuration root", source);
		var schemaVersionSeen = false;
		var overlays:Array<HxcNamedConfigOverlay> = [];
		final patchFields:Array<HxcJsonField> = [];
		for (field in fields) {
			switch field.name {
				case "schemaVersion":
					schemaVersionSeen = true;
					if (numberLexeme(field.value, "schemaVersion", source) != Std.string(HxcProjectConfig.SCHEMA_VERSION)) {
						failAt(field.value, 'schemaVersion must be the integer `${HxcProjectConfig.SCHEMA_VERSION}`', source);
					}
				case "$schema":
					final schemaId = stringValue(field.value, "$schema", source);
					if (schemaId != HxcProjectConfig.SCHEMA_ID) {
						failAt(field.value, 'unsupported schema `$schemaId`; expected `${HxcProjectConfig.SCHEMA_ID}`', source);
					}
				case "overlays":
					overlays = parseOverlays(field.value, source);
				case "hxml" | "output" | "profile" | "runtime" | "runtimeDiagnostics" | "environment" | "cStandard" | "projectLayout" | "cExtensions" |
					"build" | "artifact":
					patchFields.push(field);
				case unknown:
					throw new HxcConfigError('unknown hxc.json key `$unknown`', source, field.line, field.column);
			}
		}
		if (!schemaVersionSeen) {
			failAt(root, "configuration requires schemaVersion", source);
		}
		final patch = parsePatch(patchFields, source);
		validatePatch(patch, root, source);
		return new HxcProjectConfig(patch, overlays);
	}

	public static function contractSnapshot():HxcConfigParserContract {
		return {
			schemaVersion: HxcProjectConfig.SCHEMA_VERSION,
			schemaId: HxcProjectConfig.SCHEMA_ID,
			rootKeys: HxcProjectConfig.rootKeys(),
			overlayKeys: HxcProjectConfig.overlayKeys(),
			enums: {
				artifact: ["executable", "shared-library", "static-library"],
				build: ["debug", "minsizerel", "release"],
				cExtensions: ["gnu", "msvc", "none"],
				cStandard: ["c11", "c17", "c23"],
				projectLayout: ["package", "split", "unity"],
				environment: ["emscripten", "freestanding", "hosted", "wasi"],
				profile: ["metal", "portable"],
				runtime: ["auto", "minimal", "none"],
				runtimeDiagnostics: ["off", "summary", "warn"]
			}
		};
	}

	public static function profile(raw:String, source:String = "command-line", line:Int = 1, column:Int = 1):CProfile {
		return switch raw {
			case "portable": CProfile.Portable;
			case "metal": CProfile.Metal;
			case _: throw new HxcConfigError('invalid profile `$raw`; expected portable or metal', source, line, column);
		};
	}

	public static function runtime(raw:String, source:String = "command-line", line:Int = 1, column:Int = 1):CRuntimePolicy {
		return switch raw {
			case "auto": CRuntimePolicy.Auto;
			case "minimal": CRuntimePolicy.Minimal;
			case "none": CRuntimePolicy.None;
			case _: throw new HxcConfigError('invalid runtime policy `$raw`; expected auto, minimal, or none', source, line, column);
		};
	}

	public static function runtimeDiagnostics(raw:String, source:String = "command-line", line:Int = 1, column:Int = 1):CRuntimeDiagnostics {
		return switch raw {
			case "off": CRuntimeDiagnostics.Off;
			case "summary": CRuntimeDiagnostics.Summary;
			case "warn": CRuntimeDiagnostics.Warn;
			case _: throw new HxcConfigError('invalid runtime diagnostics `$raw`; expected off, summary, or warn', source, line, column);
		};
	}

	public static function environment(raw:String, source:String = "command-line", line:Int = 1, column:Int = 1):CEnvironment {
		return switch raw {
			case "hosted": CEnvironment.Hosted;
			case "freestanding": CEnvironment.Freestanding;
			case "wasi": CEnvironment.Wasi;
			case "emscripten": CEnvironment.Emscripten;
			case _:
				throw new HxcConfigError('invalid environment `$raw`; expected hosted, freestanding, wasi, or emscripten', source, line, column);
		};
	}

	public static function cStandard(raw:String, source:String = "command-line", line:Int = 1, column:Int = 1):HxcCStandard {
		return switch raw {
			case "c11": HxcCStandard.C11;
			case "c17": HxcCStandard.C17;
			case "c23": HxcCStandard.C23Experimental;
			case _: throw new HxcConfigError('invalid C standard `$raw`; expected c11, c17, or c23', source, line, column);
		};
	}

	public static function projectLayout(raw:String, source:String = "command-line", line:Int = 1, column:Int = 1):HxcProjectLayout {
		return switch raw {
			case "split": HxcProjectLayout.Split;
			case "package": HxcProjectLayout.Package;
			case "unity": HxcProjectLayout.Unity;
			case _: throw new HxcConfigError('invalid project layout `$raw`; expected split, package, or unity', source, line, column);
		};
	}

	public static function cExtensions(raw:String, source:String = "command-line", line:Int = 1, column:Int = 1):HxcCExtensionPolicy {
		return switch raw {
			case "none": HxcCExtensionPolicy.None;
			case "gnu": HxcCExtensionPolicy.Gnu;
			case "msvc": HxcCExtensionPolicy.Msvc;
			case _: throw new HxcConfigError('invalid C extension policy `$raw`; expected none, gnu, or msvc', source, line, column);
		};
	}

	public static function build(raw:String, source:String = "command-line", line:Int = 1, column:Int = 1):CBuildMode {
		return switch raw {
			case "debug": CBuildMode.Debug;
			case "release": CBuildMode.Release;
			case "minsizerel": CBuildMode.MinSizeRel;
			case _: throw new HxcConfigError('invalid build mode `$raw`; expected debug, release, or minsizerel', source, line, column);
		};
	}

	public static function artifact(raw:String, source:String = "command-line", line:Int = 1, column:Int = 1):HxcArtifactKind {
		return switch raw {
			case "executable": HxcArtifactKind.Executable;
			case "static-library": HxcArtifactKind.StaticLibrary;
			case "shared-library": HxcArtifactKind.SharedLibrary;
			case _: throw new HxcConfigError('invalid artifact kind `$raw`; expected executable, static-library, or shared-library', source, line, column);
		};
	}

	static function parseOverlays(node:HxcJsonNode, source:String):Array<HxcNamedConfigOverlay> {
		final overlays:Array<HxcNamedConfigOverlay> = [];
		for (field in objectFields(node, "overlays", source)) {
			if (!~/^[A-Za-z][A-Za-z0-9_-]*$/.match(field.name)) {
				throw new HxcConfigError('overlay name `${field.name}` must match [A-Za-z][A-Za-z0-9_-]*', source, field.line, field.column);
			}
			final patch = parsePatch(objectFields(field.value, 'overlay `${field.name}`', source), source);
			validatePatch(patch, field.value, source);
			overlays.push(new HxcNamedConfigOverlay(field.name, patch));
		}
		return overlays;
	}

	static function parsePatch(fields:Array<HxcJsonField>, source:String):HxcConfigPatch {
		var hxml:Null<HxcProjectPath> = null;
		var output:Null<HxcProjectPath> = null;
		var selectedProfile:Null<CProfile> = null;
		var selectedRuntime:Null<CRuntimePolicy> = null;
		var selectedRuntimeDiagnostics:Null<CRuntimeDiagnostics> = null;
		var selectedEnvironment:Null<CEnvironment> = null;
		var selectedCStandard:Null<HxcCStandard> = null;
		var selectedProjectLayout:Null<HxcProjectLayout> = null;
		var selectedCExtensions:Null<HxcCExtensionPolicy> = null;
		var selectedBuild:Null<CBuildMode> = null;
		var selectedArtifact:Null<HxcArtifactKind> = null;
		for (field in fields) {
			final raw = stringValue(field.value, field.name, source);
			switch field.name {
				case "hxml":
					hxml = projectPath(raw, "hxml", field.value, source);
				case "output":
					output = projectPath(raw, "output", field.value, source);
				case "profile":
					selectedProfile = profile(raw, source, field.value.line, field.value.column);
				case "runtime":
					selectedRuntime = runtime(raw, source, field.value.line, field.value.column);
				case "runtimeDiagnostics":
					selectedRuntimeDiagnostics = runtimeDiagnostics(raw, source, field.value.line, field.value.column);
				case "environment":
					selectedEnvironment = environment(raw, source, field.value.line, field.value.column);
				case "cStandard":
					selectedCStandard = cStandard(raw, source, field.value.line, field.value.column);
				case "projectLayout":
					selectedProjectLayout = projectLayout(raw, source, field.value.line, field.value.column);
				case "cExtensions":
					selectedCExtensions = cExtensions(raw, source, field.value.line, field.value.column);
				case "build":
					selectedBuild = build(raw, source, field.value.line, field.value.column);
				case "artifact":
					selectedArtifact = artifact(raw, source, field.value.line, field.value.column);
				case unknown:
					throw new HxcConfigError('unknown overlay key `$unknown`', source, field.line, field.column);
			}
		}
		return new HxcConfigPatch({
			hxml: hxml,
			output: output,
			profile: selectedProfile,
			runtime: selectedRuntime,
			runtimeDiagnostics: selectedRuntimeDiagnostics,
			environment: selectedEnvironment,
			cStandard: selectedCStandard,
			projectLayout: selectedProjectLayout,
			cExtensions: selectedCExtensions,
			build: selectedBuild,
			artifact: selectedArtifact
		});
	}

	static function projectPath(raw:String, label:String, node:HxcJsonNode, source:String):HxcProjectPath {
		try {
			return new HxcProjectPath(raw, label);
		} catch (error:HxcConfigError) {
			throw new HxcConfigError(error.detail, source, node.line, node.column);
		}
	}

	static function validatePatch(patch:HxcConfigPatch, node:HxcJsonNode, source:String):Void {
		try {
			HxcConfigRules.validatePatch(patch);
		} catch (error:HxcConfigError) {
			throw new HxcConfigError(error.detail, source, node.line, node.column);
		}
	}

	static function objectFields(node:HxcJsonNode, label:String, source:String):Array<HxcJsonField> {
		return switch node.value {
			case JObject(fields): fields;
			case _: failAt(node, '$label must be a JSON object', source);
		};
	}

	static function stringValue(node:HxcJsonNode, label:String, source:String):String {
		return switch node.value {
			case JString(value): value;
			case _: failAt(node, '$label must be a JSON string', source);
		};
	}

	static function numberLexeme(node:HxcJsonNode, label:String, source:String):String {
		return switch node.value {
			case JNumber(value): value;
			case _: failAt(node, '$label must be a JSON integer', source);
		};
	}

	static function failAt<T>(node:HxcJsonNode, detail:String, source:String):T
		throw new HxcConfigError(detail, source, node.line, node.column);
}

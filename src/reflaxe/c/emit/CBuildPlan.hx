package reflaxe.c.emit;

import haxe.io.Bytes;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;

/** C language mode recorded once for the manifest and every build adapter. */
enum abstract CBuildStandard(String) to String {
	var C11 = "c11";
	var C17 = "c17";
	var C23Experimental = "c23-experimental";
}

/** The M1 seed deliberately admits executable projects only. */
enum abstract CBuildArtifactKind(String) to String {
	var Executable = "executable";
}

enum abstract CBuildWarningPolicy(String) to String {
	var Strict = "strict";
}

enum abstract CBuildHeaderKind(String) to String {
	var System = "system";
	var Local = "local";
}

enum abstract CBuildDefinitionValueKind(String) to String {
	var StringValue = "string";
	var IntegerValue = "integer";
	var FloatValue = "float";
	var BooleanValue = "boolean";
}

typedef CBuildArtifact = {
	final kind:CBuildArtifactKind;
	final targetName:String;
}

typedef CBuildHeaderRequirement = {
	final path:String;
	final kind:CBuildHeaderKind;
	final ownerModulePaths:Array<String>;
}

typedef CBuildDefinition = {
	final name:String;
	final value:String;
	final valueKind:CBuildDefinitionValueKind;
	final compilerValue:String;
	final ownerModulePaths:Array<String>;
}

typedef CBuildLibraryRequirement = {
	final name:String;
	final ownerModulePaths:Array<String>;
}

/**
	Tool-neutral schema-1 compile/link plan embedded in hxc.manifest.json.
	CMake and Meson are derived views and never become configuration authority.
**/
typedef CBuildPlanSnapshot = {
	final schemaVersion:Int;
	final artifact:CBuildArtifact;
	final cStandard:CBuildStandard;
	final extensions:Bool;
	final warningPolicy:CBuildWarningPolicy;
	final sources:Array<String>;
	final publicHeaders:Array<String>;
	final privateHeaders:Array<String>;
	final runtimeHeaders:Array<String>;
	final includeDirectories:Array<String>;
	final requiredHeaders:Array<CBuildHeaderRequirement>;
	final definitions:Array<CBuildDefinition>;
	final libraries:Array<CBuildLibraryRequirement>;
	final pkgConfigPackages:Array<CBuildLibraryRequirement>;
	final frameworks:Array<CBuildLibraryRequirement>;
}

private enum abstract CanonicalBuildFactKind(String) to String {
	var Include = "include";
	var Link = "link";
	var PkgConfig = "pkg-config";
	var Framework = "framework";
	var Define = "define";
}

private typedef CanonicalBuildFact = {
	final kind:CanonicalBuildFactKind;
	final name:String;
	final value:Null<String>;
	final valueKind:Null<CBuildDefinitionValueKind>;
	final ownerModulePaths:Array<String>;
}

/** Builds and validates the one neutral model consumed by every adapter. */
class CBuildPlanBuilder {
	public static inline final SCHEMA_VERSION = 1;
	public static inline final EXECUTABLE_TARGET = "hxc_program";

	public function new() {}

	public function build(projectName:String, cStandard:CBuildStandard, units:Array<GeneratedFile>, facts:Array<TypedCBuildFact>):CBuildPlanSnapshot {
		validateProjectName(projectName);
		validateStandard(cStandard);
		final canonicalFacts = canonicalBuildFacts(facts);
		final requiredHeaders:Array<CBuildHeaderRequirement> = [];
		final definitions:Array<CBuildDefinition> = [];
		final libraries:Array<CBuildLibraryRequirement> = [];
		final pkgConfigPackages:Array<CBuildLibraryRequirement> = [];
		final frameworks:Array<CBuildLibraryRequirement> = [];
		final headerKinds:Map<String, CBuildHeaderKind> = [];
		final definitionNames:Map<String, Bool> = [];
		for (fact in canonicalFacts) {
			switch fact.kind {
				case Include:
					final value = fact.value;
					if (value == null) {
						fail('include build fact `${fact.name}` lost its kind');
					}
					final headerKind = value == "system" ? CBuildHeaderKind.System : CBuildHeaderKind.Local;
					if (headerKinds.exists(fact.name)) {
						fail('include build fact `${fact.name}` has conflicting system/local kinds');
					}
					headerKinds.set(fact.name, headerKind);
					requiredHeaders.push({
						path: fact.name,
						kind: headerKind,
						ownerModulePaths: fact.ownerModulePaths.copy()
					});
				case Define:
					if (definitionNames.exists(fact.name)) {
						fail('define build fact `${fact.name}` has conflicting literal values or kinds');
					}
					definitionNames.set(fact.name, true);
					switch [fact.value, fact.valueKind] {
						case [value, valueKind] if (value != null && valueKind != null):
							definitions.push({
								name: fact.name,
								value: value,
								valueKind: valueKind,
								compilerValue: compilerDefinitionValue(value, valueKind),
								ownerModulePaths: fact.ownerModulePaths.copy()
							});
						case _:
							fail('define build fact `${fact.name}` lost its typed value');
					}
				case Link:
					libraries.push(libraryRequirement(fact));
				case PkgConfig:
					pkgConfigPackages.push(libraryRequirement(fact));
				case Framework:
					frameworks.push(libraryRequirement(fact));
			}
		}

		final sources = pathsOfKinds(units, [GeneratedFileKind.Source, GeneratedFileKind.RuntimeSource]);
		for (source in sources) {
			validateAdapterPath(source, "source");
		}
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
			schemaVersion: SCHEMA_VERSION,
			artifact: {kind: CBuildArtifactKind.Executable, targetName: EXECUTABLE_TARGET},
			cStandard: cStandard,
			extensions: false,
			warningPolicy: CBuildWarningPolicy.Strict,
			sources: sources,
			publicHeaders: publicHeaders,
			privateHeaders: privateHeaders,
			runtimeHeaders: runtimeHeaders,
			includeDirectories: includeDirectories,
			requiredHeaders: requiredHeaders,
			definitions: definitions,
			libraries: libraries,
			pkgConfigPackages: pkgConfigPackages,
			frameworks: frameworks
		};
	}

	function canonicalBuildFacts(input:Array<TypedCBuildFact>):Array<CanonicalBuildFact> {
		final byKey:Map<String, CanonicalBuildFact> = [];
		for (fact in input) {
			final kind = buildFactKind(fact.kind);
			validateLogicalText(fact.name, 'build fact `${fact.kind}` name');
			if (fact.value != null) {
				validateLogicalText(fact.value, 'build fact `${fact.kind}` value');
			}
			final valueKind = buildFactValueKind(kind, fact.value, fact.valueKind);
			validateBuildFactName(kind, fact.name);
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
				validateOwner(owner, fact);
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

	static function buildFactKind(value:String):CanonicalBuildFactKind {
		return switch value {
			case "include": CanonicalBuildFactKind.Include;
			case "link": CanonicalBuildFactKind.Link;
			case "pkg-config": CanonicalBuildFactKind.PkgConfig;
			case "framework": CanonicalBuildFactKind.Framework;
			case "define": CanonicalBuildFactKind.Define;
			case _: throw new ProjectEmissionError('unknown typed build fact kind `$value`');
		};
	}

	static function buildFactValueKind(kind:CanonicalBuildFactKind, value:Null<String>, rawKind:Null<String>):Null<CBuildDefinitionValueKind> {
		return switch kind {
			case Include:
				if (rawKind != "enum" || (value != "system" && value != "local")) {
					fail("include build facts must carry c.IncludeKind.System or c.IncludeKind.Local");
				}
				null;
			case Define:
				if (value == null) {
					fail("define build facts require a literal value");
				}
				switch rawKind {
					case "string": CBuildDefinitionValueKind.StringValue;
					case "integer": CBuildDefinitionValueKind.IntegerValue;
					case "float": CBuildDefinitionValueKind.FloatValue;
					case "boolean": CBuildDefinitionValueKind.BooleanValue;
					case _: throw new ProjectEmissionError('define build fact has unknown literal kind `${Std.string(rawKind)}`');
				}
			case Link | PkgConfig | Framework:
				if (value != null || rawKind != null) {
					fail('`${Std.string(kind)}` build facts must not carry a value');
				}
				null;
		};
	}

	static function validateBuildFactName(kind:CanonicalBuildFactKind, name:String):Void {
		switch kind {
			case Include:
				if (!GeneratedFile.isNormalizedRelativePath(name)) {
					fail('include build fact must use a normalized logical path: `$name`');
				}
			case Define:
				if (!~/^[A-Za-z_][A-Za-z0-9_]*$/.match(name)) {
					fail('define build fact name must be a C identifier: `$name`');
				}
			case Link | PkgConfig | Framework:
				if (!~/^[A-Za-z0-9_][A-Za-z0-9_+.-]*$/.match(name)) {
					fail('logical build dependency contains unsafe flag or build-language syntax: `$name`');
				}
		}
	}

	static function validateOwner(owner:String, fact:TypedCBuildFact):Void {
		validateLogicalText(owner, 'build fact `${fact.kind}` owner');
		if (owner.indexOf("/") != -1
			|| owner.indexOf("\\") != -1
			|| ~/^[A-Za-z]:/.match(owner)
			|| owner.split(".").indexOf("") != -1) {
			fail('build fact owner must be a logical module identity: `$owner`');
		}
	}

	static function compilerDefinitionValue(value:String, kind:CBuildDefinitionValueKind):String {
		return switch kind {
			case StringValue: cStringLiteral(value);
			case IntegerValue:
				if (!~/^(?:0|[1-9][0-9]*|0[xX][0-9A-Fa-f]+)$/.match(value)) {
					fail('integer build definition is not a strict C11 literal: `$value`');
				}
				value;
			case FloatValue:
				if (!~/^(?:(?:[0-9]+\.[0-9]*|\.[0-9]+)(?:[eE][+-]?[0-9]+)?|[0-9]+[eE][+-]?[0-9]+)$/.match(value)) {
					fail('floating build definition is not a strict C11 literal: `$value`');
				}
				value;
			case BooleanValue:
				switch value {
					case "true": "1";
					case "false": "0";
					case _: throw new ProjectEmissionError('Boolean build definition must be `true` or `false`; found `$value`');
				}
		};
	}

	static function cStringLiteral(value:String):String {
		final bytes = Bytes.ofString(value);
		final result = new StringBuf();
		result.add('"');
		for (index in 0...bytes.length) {
			final byte = bytes.get(index);
			final safe = byte >= 48 && byte <= 57 || byte >= 65 && byte <= 90 || byte >= 97 && byte <= 122 || byte == 45 || byte == 46 || byte == 95;
			if (safe) {
				result.addChar(byte);
			} else {
				result.add("\\");
				result.add(octalByte(byte));
			}
		}
		result.add('"');
		return result.toString();
	}

	static function octalByte(value:Int):String {
		final first = Std.int(value / 64);
		final second = Std.int(value / 8) % 8;
		final third = value % 8;
		return String.fromCharCode(48 + first) + String.fromCharCode(48 + second) + String.fromCharCode(48 + third);
	}

	static function libraryRequirement(fact:CanonicalBuildFact):CBuildLibraryRequirement
		return {name: fact.name, ownerModulePaths: fact.ownerModulePaths.copy()};

	static function pathsOfKinds(files:Array<GeneratedFile>, kinds:Array<GeneratedFileKind>):Array<String> {
		final result:Array<String> = [];
		for (file in files) {
			if (kinds.indexOf(file.kind) != -1) {
				result.push(file.relativePath);
			}
		}
		result.sort(compareUtf8);
		return result;
	}

	static function validateProjectName(value:String):Void {
		if (!~/^[A-Za-z0-9_][A-Za-z0-9_.+-]*$/.match(value)) {
			fail('build project name must use only safe logical `[A-Za-z0-9_.+-]` characters: `$value`');
		}
	}

	static function validateStandard(value:CBuildStandard):Void {
		switch value {
			case C11 | C17 | C23Experimental:
			case _:
				fail('unknown build C standard `${Std.string(value)}`');
		}
	}

	static function validateAdapterPath(value:String, label:String):Void {
		if (!GeneratedFile.isNormalizedRelativePath(value) || value.indexOf("$") != -1 || value.indexOf(";") != -1) {
			fail('$label path cannot be represented safely by every generated build adapter: `$value`', [value]);
		}
	}

	static function validateLogicalText(value:String, label:String):Void {
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

	static function factKey(kind:CanonicalBuildFactKind, name:String, value:Null<String>, valueKind:Null<CBuildDefinitionValueKind>):String
		return canonicalPart(Std.string(kind))
			+ canonicalPart(name)
			+ canonicalPart(value == null ? "" : value)
			+ canonicalPart(valueKind == null ? "" : Std.string(valueKind));

	static function canonicalPart(value:String):String
		return '${Bytes.ofString(value).length}:$value';

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

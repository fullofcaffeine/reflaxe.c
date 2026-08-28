package hxc.bindgen;

import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.io.Path;
import hxc.bindgen.HxcBindgenOptions.HxcBindgenDefine;
import hxc.bindgen.HxcBindgenOptions.HxcBindgenLanguage;
import hxc.config.HxcJsonCodec.jsonArray;
import hxc.config.HxcJsonCodec.jsonField;
import hxc.config.HxcJsonCodec.jsonInt;
import hxc.config.HxcJsonCodec.jsonObject;
import hxc.config.HxcJsonCodec.jsonString;
import hxc.config.HxcJsonCodec.objectField;
import hxc.config.HxcJsonCodec.renderJson;
import hxc.config.HxcJsonValue;
import hxc.config.HxcJsonValue.HxcJsonField;
import hxc.config.HxcJsonValue.HxcJsonNode;
import reflaxe.c.CUtf8Order.compare as compareUtf8Bytes;
import sys.FileSystem;
import sys.io.File;

/**
	Normalize Clang's semantic JSON into a reproducible, versioned lock payload.

	Clang remains the declaration authority. This module removes only ephemeral
	pointer-like node identities, canonicalizes object-key order, and rewrites
	known source roots to logical names. It does not infer ABI facts from text.
**/
function normalizeClangAst(node:HxcJsonNode, paths:HxcBindgenPaths, fieldName:Null<String> = null):HxcJsonNode {
	final value = switch node.value {
		case JNull: HxcJsonValue.JNull;
		case JBool(value): HxcJsonValue.JBool(value);
		case JNumber(value): HxcJsonValue.JNumber(value);
		case JString(value): HxcJsonValue.JString(fieldName == "file" ? paths.logical(value) : value);
		case JArray(values): HxcJsonValue.JArray(values.map(value -> normalizeClangAst(value, paths)));
		case JObject(fields):
			final normalized:Array<HxcJsonField> = [];
			for (field in fields) {
				if (!isEphemeralIdentity(field.name))
					normalized.push(new HxcJsonField(field.name, normalizeClangAst(field.value, paths, field.name), 0, 0));
			}
			normalized.sort((left, right) -> compareUtf8(left.name, right.name));
			HxcJsonValue.JObject(normalized);
	};
	return new HxcJsonNode(value, 0, 0);
}

/** Named semantic-lock inputs keep exact pass/configuration facts from being misordered. */
typedef HxcBindgenLockRequest = {
	final ast:HxcJsonNode;
	final paths:HxcBindgenPaths;
	final clang:String;
	final version:String;
	final dumpMachine:String;
	final resourceDirectory:String;
	final target:String;
	final diagnosticArguments:Array<String>;
	final semanticArguments:Array<String>;
	final dependencyArguments:Array<String>;
	final dependencies:Array<String>;
	final language:HxcBindgenLanguage;
	final languageExplicit:Bool;
	final targetExplicit:Bool;
	final sysroot:Null<String>;
	final defines:Array<HxcBindgenDefine>;
}

/** Build the schema-2 lock from one canonical AST and its complete configured input set. */
function buildBindingLock(request:HxcBindgenLockRequest):HxcJsonNode {
	final ast = request.ast;
	final paths = request.paths;
	final kind = stringField(ast, "kind");
	if (kind != "TranslationUnitDecl")
		throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Internal, "HXC-CLI-0805", "Clang AST root is not a translation unit",
			"Check the selected Clang version and report its AST JSON shape.");
	final inner = objectField(ast, "inner");
	final declarationCount = switch inner == null ? HxcJsonValue.JNull : inner.value {
		case JArray(values): values.length;
		case _: 0;
	};
	final semanticText = renderJson(ast);
	final inputs = dependencyInputs(paths, request.dependencies);
	final inputDigestMaterial = new StringBuf();
	for (input in inputs) {
		inputDigestMaterial.add(stringField(input, "path"));
		inputDigestMaterial.add("\x00");
		inputDigestMaterial.add(stringField(input, "sha256"));
		inputDigestMaterial.add("\n");
	}
	final effectiveConfiguration = jsonObject([
		jsonField("entryHeaders", jsonArray(paths.entryHeaders.map(path -> jsonString(paths.logical(path))))),
		jsonField("language", jsonString(request.language)),
		jsonField("target", jsonString(request.target)),
		jsonField("sysroot", nullableString(request.sysroot == null ? null : paths.logical(request.sysroot))),
		jsonField("includeDirectories", jsonArray(paths.includeDirectories.map(path -> jsonString(paths.logical(path))))),
		jsonField("defines", jsonArray(request.defines.map(define -> jsonObject([
			jsonField("name", jsonString(define.name)),
			jsonField("value", nullableString(define.value))
		]))))
	]);
	final configuration = jsonObject([
		jsonField("effective", effectiveConfiguration),
		jsonField("provenance", jsonObject([
			jsonField("entryHeaders", jsonString("command-line")),
			jsonField("language", jsonString(request.languageExplicit ? "command-line" : "default")),
			jsonField("target", jsonString(request.targetExplicit ? "command-line" : "clang-default")),
			jsonField("sysroot", jsonString(request.sysroot == null ? "absent" : "command-line")),
			jsonField("includeDirectories", jsonString(paths.includeDirectories.length == 0 ? "absent" : "command-line")),
			jsonField("defines", jsonString(request.defines.length == 0 ? "absent" : "command-line"))
		]))
	]);
	return jsonObject([
		jsonField("schemaVersion", jsonInt(2)),
		jsonField("authority", jsonString("clang-ast-json")),
		jsonField("generator", jsonObject([
			jsonField("name", jsonString("hxc-bindgen")),
			jsonField("model", jsonString("clang-semantic-translation-unit-v2"))
		])),
		jsonField("toolchain",
			jsonObject([
				jsonField("executable", jsonString(paths.logicalExecutable(request.clang))),
				jsonField("version", jsonString(request.version)),
				jsonField("dumpMachine", jsonString(request.dumpMachine)),
				jsonField("resourceDirectory", jsonString(paths.logical(request.resourceDirectory)))
			])),
		jsonField("configuration", configuration),
		jsonField("configurationSha256", jsonString(Sha256.encode(renderJson(effectiveConfiguration)))),
		jsonField("invocation",
			jsonObject([
				jsonField("diagnosticArguments", jsonArray(request.diagnosticArguments.map(argument -> jsonString(paths.logicalArgument(argument))))),
				jsonField("semanticArguments", jsonArray(request.semanticArguments.map(argument -> jsonString(paths.logicalArgument(argument))))),
				jsonField("dependencyArguments", jsonArray(request.dependencyArguments.map(argument -> jsonString(paths.logicalArgument(argument)))))
			])),
		jsonField("inputs", jsonArray(inputs)),
		jsonField("inputSetSha256", jsonString(Sha256.encode(inputDigestMaterial.toString()))),
		jsonField("semanticModel", jsonObject([
			jsonField("schemaVersion", jsonInt(1)),
			jsonField("declarationCount", jsonInt(declarationCount)),
			jsonField("translationUnit", ast)
		])),
		jsonField("semanticSha256", jsonString(Sha256.encode(semanticText)))
	]);
}

function nullableString(value:Null<String>):HxcJsonNode
	return value == null ? new HxcJsonNode(HxcJsonValue.JNull, 0, 0) : jsonString(value);

function dependencyInputs(paths:HxcBindgenPaths, dependencies:Array<String>):Array<HxcJsonNode> {
	final unique:Map<String, String> = [];
	for (dependency in dependencies) {
		final full = FileSystem.fullPath(dependency);
		if (!FileSystem.exists(full) || FileSystem.isDirectory(full))
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0806", 'Clang dependency `$dependency` is not a readable file',
				"Check include paths and retry the same translation unit.");
		unique.set(paths.logical(full), full);
	}
	final logicalPaths = [for (path in unique.keys()) path];
	logicalPaths.sort(compareUtf8);
	final inputs:Array<HxcJsonNode> = [];
	for (logical in logicalPaths) {
		final full = unique.get(logical);
		if (full == null)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Internal, "HXC-CLI-0806", "binding input inventory lost a resolved path",
				"Retry the same translation unit and report this deterministic inventory failure.");
		inputs.push(jsonObject([
			jsonField("path", jsonString(logical)),
			jsonField("sha256", jsonString(Sha256.make(File.getBytes(full)).toHex()))
		]));
	}
	return inputs;
}

function stringField(node:HxcJsonNode, name:String):String {
	final value = objectField(node, name);
	return switch value == null ? HxcJsonValue.JNull : value.value {
		case JString(text): text;
		case _: "";
	};
}

function isEphemeralIdentity(name:String):Bool
	return name == "id" || name == "previousDecl" || name == "parentDeclContextId" || name == "typeAliasDeclId";

function compareUtf8(left:String, right:String):Int
	return compareUtf8Bytes(left, right);

/** Logical source-root mapping used by both AST locations and locked argv. */
class HxcBindgenPaths {
	/** Ordered canonical entry-header paths used while invoking Clang. */
	public final entryHeaders:Array<String>;

	/** Ordered unique source roots replaced by `$SOURCE` markers in the lock. */
	public final sourceRoots:Array<String>;

	/** Ordered canonical include roots replaced by logical markers. */
	public final includeDirectories:Array<String>;

	/** Optional target sysroot replaced by `$SYSROOT` in the lock. */
	public final sysroot:Null<String>;

	/** Clang resource root replaced by `$CLANG_RESOURCE` in the lock. */
	public final resourceDirectory:String;

	/** Build one path-normalization policy for a translation unit. */
	public function new(entryHeaders:Array<String>, includeDirectories:Array<String>, sysroot:Null<String>, resourceDirectory:String) {
		this.entryHeaders = entryHeaders.map(path -> FileSystem.fullPath(path));
		final roots:Array<String> = [];
		for (header in this.entryHeaders) {
			final root = normalize(Path.directory(header));
			if (roots.indexOf(root) < 0)
				roots.push(root);
		}
		this.sourceRoots = roots;
		this.includeDirectories = includeDirectories.map(path -> FileSystem.fullPath(path));
		this.sysroot = sysroot == null ? null : FileSystem.fullPath(sysroot);
		this.resourceDirectory = FileSystem.fullPath(resourceDirectory);
	}

	/** Replace a known absolute source root with a stable logical prefix. */
	public function logical(value:String):String {
		if (!isAbsolute(value))
			return StringTools.replace(value, "\\", "/");
		final normalized = normalize(FileSystem.fullPath(value));
		if (sysroot != null) {
			final rooted = below(normalized, normalize(sysroot), "$SYSROOT");
			if (rooted != null)
				return rooted;
		}
		for (index in 0...sourceRoots.length) {
			final source = below(normalized, sourceRoots[index], index == 0 ? "$SOURCE" : '$$SOURCE$index');
			if (source != null)
				return source;
		}
		for (index in 0...includeDirectories.length) {
			final included = below(normalized, normalize(includeDirectories[index]), '$$INCLUDE$index');
			if (included != null)
				return included;
		}
		final resource = below(normalized, normalize(resourceDirectory), "$CLANG_RESOURCE");
		return resource == null ? normalized : resource;
	}

	/** Normalize one argument while preserving its exact option spelling and order. */
	public function logicalArgument(argument:String):String {
		if (StringTools.startsWith(argument, "-I") && argument.length > 2)
			return "-I" + logical(argument.substr(2));
		if (StringTools.startsWith(argument, "--sysroot=") && argument.length > 10)
			return "--sysroot=" + logical(argument.substr(10));
		return isAbsolute(argument) ? logical(argument) : argument;
	}

	/** Keep a command name, but avoid embedding an absolute host tool path. */
	public function logicalExecutable(executable:String):String {
		if (!isAbsolute(executable))
			return executable;
		return '$$TOOL/${Path.withoutDirectory(executable)}';
	}

	static function below(path:String, root:String, marker:String):Null<String> {
		if (path == root)
			return marker;
		final prefix = StringTools.endsWith(root, "/") ? root : root + "/";
		return StringTools.startsWith(path, prefix) ? marker + "/" + path.substr(prefix.length) : null;
	}

	static function normalize(path:String):String
		return StringTools.replace(path, "\\", "/");

	static function isAbsolute(path:String):Bool
		return StringTools.startsWith(path, "/") || ~/^[A-Za-z]:[\\\/]/.match(path);
}

#if macro
import haxe.Json;
import haxe.crypto.Sha256;
import haxe.io.Path;
import haxe.macro.Compiler;
import haxe.macro.Context;
import haxe.macro.Expr;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import sys.FileSystem;
import sys.io.File;

private typedef StdlibProbeSource = {
	final module:String;
	final path:String;
	final sha256:String;
}

private typedef StdlibProbeApi = {
	final module:String;
	final source:String;
	final type:String;
	final member:Null<String>;
	final kind:String;
	final signature:String;
	final isStatic:Bool;
}

private typedef StdlibProbeRoot = {
	final name:String;
	final sourceCount:Int;
}

private typedef StdlibProbeTree = {
	final sourceCount:Int;
	final pathInventorySha256:String;
	final roots:Array<StdlibProbeRoot>;
}

private typedef StdlibProbeReport = {
	final schemaVersion:Int;
	final scope:String;
	final haxeVersion:Null<String>;
	final targetName:Null<String>;
	final tree:StdlibProbeTree;
	final sources:Array<StdlibProbeSource>;
	final apis:Array<StdlibProbeApi>;
}
#end

class StdlibSurfaceProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_STDLIB_SURFACE=";

	public static function install():Void {
		if (Context.definedValue("hxc_stdlib_probe_scope") == "capability-gated") {
			if (!Context.defined("hxc_stdlib_capability_declaration_probe")) {
				Context.fatalError("capability-gated stdlib inspection requires its declaration-only test authority", Context.currentPos());
			}
		}
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final scope = probeScope();
		final standardLibraryRoot = resolveStandardLibraryRoot();
		final allSourcePaths:Array<String> = [];
		collectSourcePaths(standardLibraryRoot, "", allSourcePaths);
		allSourcePaths.sort(compareStrings);
		final sources = collectSources(standardLibraryRoot, scope, allSourcePaths);
		final apis:Array<StdlibProbeApi> = [];
		for (source in sources) {
			collectModuleApis(source, apis);
		}
		apis.sort(compareApis);
		final report:StdlibProbeReport = {
			schemaVersion: 1,
			scope: scope,
			haxeVersion: Context.definedValue("haxe"),
			targetName: Context.definedValue("target.name"),
			tree: describeTree(allSourcePaths),
			sources: sources,
			apis: apis
		};
		Sys.println(REPORT_PREFIX + Json.stringify(report));
	}

	static function resolveStandardLibraryRoot():String {
		final configuration = Compiler.getConfiguration();
		if (configuration == null) {
			Context.fatalError("stdlib probe requires an initialized compiler configuration", Context.currentPos());
		}
		for (candidate in configuration.stdPath) {
			final normalized = Path.normalize(candidate);
			if (FileSystem.exists(Path.join([normalized, "Std.hx"]))) {
				return normalized;
			}
		}
		return Context.fatalError("stdlib probe could not resolve the pinned Haxe standard library", Context.currentPos());
	}

	static function collectSources(root:String, scope:String, paths:Array<String>):Array<StdlibProbeSource> {
		final sources:Array<StdlibProbeSource> = [];
		for (path in paths) {
			if (!isApplicableSource(path, scope)) {
				continue;
			}
			final absolute = Path.join([root, path]);
			sources.push({
				module: sourceModule(path),
				path: path,
				sha256: Sha256.make(File.getBytes(absolute)).toHex()
			});
		}
		return sources;
	}

	static function describeTree(paths:Array<String>):StdlibProbeTree {
		final digestInput = new StringBuf();
		final counts:Map<String, Int> = [];
		for (path in paths) {
			digestInput.add(path);
			digestInput.add("\n");
			final slash = path.indexOf("/");
			final topLevel = slash == -1 ? "<root>" : path.substr(0, slash);
			final previous = counts.get(topLevel);
			counts.set(topLevel, previous == null ? 1 : previous + 1);
		}
		final names = [for (name in counts.keys()) name];
		names.sort(compareStrings);
		final roots:Array<StdlibProbeRoot> = [];
		for (name in names) {
			final count = counts.get(name);
			if (count != null) {
				roots.push({name: name, sourceCount: count});
			}
		}
		return {
			sourceCount: paths.length,
			pathInventorySha256: Sha256.encode(digestInput.toString()),
			roots: roots
		};
	}

	static function collectSourcePaths(root:String, relative:String, result:Array<String>):Void {
		final directory = relative == "" ? root : Path.join([root, relative]);
		final names = FileSystem.readDirectory(directory);
		names.sort(compareStrings);
		for (name in names) {
			final child = relative == "" ? name : relative + "/" + name;
			final absolute = Path.join([root, child]);
			if (FileSystem.isDirectory(absolute)) {
				collectSourcePaths(root, child, result);
			} else if (StringTools.endsWith(name, ".hx")) {
				result.push(child);
			}
		}
	}

	static function isApplicableSource(path:String, scope:String):Bool {
		final capabilityGated = StringTools.startsWith(path, "haxe/atomic/") || StringTools.startsWith(path, "sys/thread/");
		if (scope == "capability-gated") {
			return capabilityGated;
		}
		if (scope != "c-common" || capabilityGated) {
			return false;
		}
		if (path.indexOf("/") == -1) {
			return true;
		}
		if (StringTools.startsWith(path, "sys/")) {
			return true;
		}
		if (!StringTools.startsWith(path, "haxe/")) {
			return false;
		}
		if (StringTools.startsWith(path, "haxe/display/")
			|| StringTools.startsWith(path, "haxe/hxb/")
			|| StringTools.startsWith(path, "haxe/macro/")) {
			return false;
		}
		return path != "haxe/http/HttpJs.hx" && path != "haxe/http/HttpNodeJs.hx";
	}

	static function probeScope():String {
		final scope = Context.definedValue("hxc_stdlib_probe_scope");
		return scope == null ? "c-common" : scope;
	}

	static function sourceModule(path:String):String {
		final withoutExtension = path.substr(0, path.length - 3);
		return withoutExtension.split("/").join(".");
	}

	static function collectModuleApis(source:StdlibProbeSource, result:Array<StdlibProbeApi>):Void {
		final moduleTypes = Context.getModule(source.module);
		for (moduleType in moduleTypes) {
			switch moduleType {
				case TInst(reference, _):
					collectClass(source, reference.get(), result);
				case TEnum(reference, _):
					collectEnum(source, reference.get(), result);
				case TType(reference, _):
					collectTypedef(source, reference.get(), result);
				case TAbstract(reference, _):
					collectAbstract(source, reference.get(), result);
				case _:
			}
		}
	}

	static function collectClass(source:StdlibProbeSource, type:ClassType, result:Array<StdlibProbeApi>):Void {
		if (!isPublicType(type)) {
			return;
		}
		final typeName = typePath(type.pack, type.name);
		addTypeApi(source, typeName, type.isInterface ? "interface" : "class", result);
		if (type.constructor != null) {
			addFieldApis(source, typeName, type.constructor.get(), false, "constructor", result);
		}
		for (field in type.fields.get()) {
			addFieldApis(source, typeName, field, false, fieldKind(field), result);
		}
		for (field in type.statics.get()) {
			addFieldApis(source, typeName, field, true, fieldKind(field), result);
		}
	}

	static function collectEnum(source:StdlibProbeSource, type:EnumType, result:Array<StdlibProbeApi>):Void {
		if (!isPublicType(type)) {
			return;
		}
		final typeName = typePath(type.pack, type.name);
		addTypeApi(source, typeName, "enum", result);
		for (name in type.names) {
			final constructor = type.constructs.get(name);
			if (constructor != null && !constructor.meta.has(":noCompletion")) {
				result.push(api(source, typeName, constructor.name, "enum-constructor", constructor.type, true));
			}
		}
	}

	static function collectTypedef(source:StdlibProbeSource, type:DefType, result:Array<StdlibProbeApi>):Void {
		if (!isPublicType(type)) {
			return;
		}
		final typeName = typePath(type.pack, type.name);
		addTypeApi(source, typeName, "typedef", result);
		switch type.type {
			case TAnonymous(reference):
				for (field in reference.get().fields) {
					addFieldApis(source, typeName, field, false, "typedef-field", result);
				}
			case _:
		}
	}

	static function collectAbstract(source:StdlibProbeSource, type:AbstractType, result:Array<StdlibProbeApi>):Void {
		if (!isPublicType(type)) {
			return;
		}
		final typeName = typePath(type.pack, type.name);
		addTypeApi(source, typeName, "abstract", result);
		if (type.impl != null) {
			final implementation = type.impl.get();
			for (field in implementation.fields.get()) {
				addAbstractFieldApis(source, typeName, field, result);
			}
			for (field in implementation.statics.get()) {
				addAbstractFieldApis(source, typeName, field, result);
			}
		}
		collectForwardedApis(source, type, typeName, result);
		collectAbstractOperations(source, type, typeName, result);
	}

	static function addAbstractFieldApis(source:StdlibProbeSource, typeName:String, field:ClassField, result:Array<StdlibProbeApi>):Void {
		if (!field.isPublic) {
			return;
		}
		final constructor = field.name == "_new" || field.name == "_hx_new";
		if (field.meta.has(":noCompletion") && !constructor) {
			return;
		}
		final isStatic = constructor ? false : abstractFieldIsStatic(field);
		addUniqueApi(result,
			apiWithSignature(source, typeName, constructor ? "new" : field.name, constructor ? "constructor" : fieldKind(field),
				abstractFieldSignature(field.type, !constructor && !isStatic), isStatic));
		for (overloadField in field.overloads.get()) {
			addAbstractFieldApis(source, typeName, overloadField, result);
		}
	}

	static function abstractFieldIsStatic(field:ClassField):Bool {
		return switch Context.follow(field.type) {
			case TFun(arguments, _): arguments.length == 0 || arguments[0].name != "this";
			case _:
				sourceDeclaresStatic(field);
		}
	}

	static function abstractFieldSignature(type:Type, stripReceiver:Bool):String {
		if (stripReceiver) {
			switch Context.follow(type) {
				case TFun(arguments, result) if (arguments.length > 0):
					return TypeTools.toString(TFun(arguments.slice(1), result));
				case _:
			}
		}
		return TypeTools.toString(type);
	}

	static function collectForwardedApis(source:StdlibProbeSource, type:AbstractType, typeName:String, result:Array<StdlibProbeApi>):Void {
		final forwards = type.meta.extract(":forward");
		if (forwards.length == 0) {
			return;
		}
		var forwardAll = false;
		final requested:Array<String> = [];
		for (forward in forwards) {
			if (forward.params.length == 0) {
				forwardAll = true;
				continue;
			}
			for (parameter in forward.params) {
				final name = switch parameter.expr {
					case EConst(CIdent(value)): value;
					case _: Context.fatalError("stdlib probe found a non-identifier @:forward member", parameter.pos);
				};
				if (requested.indexOf(name) == -1) {
					requested.push(name);
				}
			}
		}
		requested.sort(compareStrings);
		final underlying = Context.follow(type.type);
		switch underlying {
			case TInst(reference, parameters):
				final classType = reference.get();
				switch classType.kind {
					case KTypeParameter(_):
						addForwardWildcard(source, typeName, underlying, result);
					case _:
						final fields:Array<ClassField> = [];
						if (forwardAll) {
							for (field in classType.fields.get()) {
								if (field.isPublic && !field.meta.has(":noCompletion")) {
									fields.push(field);
								}
							}
						} else {
							for (name in requested) {
								final field = TypeTools.findField(classType, name);
								if (field == null || !field.isPublic || field.meta.has(":noCompletion")) {
									Context.fatalError('stdlib probe could not resolve forwarded member `$typeName.$name`', type.pos);
								}
								fields.push(field);
							}
						}
						for (field in fields) {
							if (!hasMember(result, typeName, field.name)) {
								final signature = TypeTools.applyTypeParameters(field.type, classType.params, parameters);
								addUniqueApi(result, api(source, typeName, field.name, "forwarded-" + fieldKind(field), signature, false));
							}
						}
				}
			case _:
				addForwardWildcard(source, typeName, underlying, result);
		}
	}

	static function addForwardWildcard(source:StdlibProbeSource, typeName:String, underlying:Type, result:Array<StdlibProbeApi>):Void {
		if (!hasMember(result, typeName, "*")) {
			addUniqueApi(result, apiWithSignature(source, typeName, "*", "forwarded-surface", TypeTools.toString(underlying), false));
		}
	}

	static function collectAbstractOperations(source:StdlibProbeSource, type:AbstractType, typeName:String, result:Array<StdlibProbeApi>):Void {
		for (operation in type.binops) {
			addAbstractOperation(source, typeName, Std.string(operation.op), "abstract-binary-operator", operation.field, result);
		}
		for (operation in type.unops) {
			final position = operation.postFix ? "postfix" : "prefix";
			addAbstractOperation(source, typeName, Std.string(operation.op) + ":" + position, "abstract-unary-operator", operation.field, result);
		}
		for (conversion in type.from) {
			final fromType = TypeTools.toString(conversion.t);
			final signature = conversion.field == null ? fromType + " -> " + typeName : TypeTools.toString(conversion.field.type);
			addUniqueApi(result, apiWithSignature(source, typeName, "from:" + fromType, "abstract-from-cast", signature, true));
		}
		for (conversion in type.to) {
			final toType = TypeTools.toString(conversion.t);
			final signature = if (conversion.field == null) {
				typeName + " -> " + toType;
			} else {
				final isStatic = abstractFieldIsStatic(conversion.field);
				abstractFieldSignature(conversion.field.type, !isStatic);
			};
			addUniqueApi(result, apiWithSignature(source, typeName, "to:" + toType, "abstract-to-cast", signature, true));
		}
		for (field in type.array) {
			addAbstractOperation(source, typeName, "[]", "abstract-array-access", field, result);
		}
		if (type.resolve != null) {
			addAbstractOperation(source, typeName, ".*", "abstract-resolve", type.resolve, result);
		}
		if (type.resolveWrite != null) {
			addAbstractOperation(source, typeName, ".*=", "abstract-resolve-write", type.resolveWrite, result);
		}
	}

	static function addAbstractOperation(source:StdlibProbeSource, typeName:String, member:String, kind:String, field:ClassField,
			result:Array<StdlibProbeApi>):Void {
		final isStatic = abstractFieldIsStatic(field);
		addUniqueApi(result, apiWithSignature(source, typeName, member, kind, abstractFieldSignature(field.type, !isStatic), isStatic));
	}

	static function hasMember(result:Array<StdlibProbeApi>, typeName:String, member:String):Bool {
		for (entry in result) {
			if (entry.type == typeName && entry.member == member) {
				return true;
			}
		}
		return false;
	}

	static function addUniqueApi(result:Array<StdlibProbeApi>, candidate:StdlibProbeApi):Void {
		for (entry in result) {
			if (entry.module == candidate.module
				&& entry.type == candidate.type
				&& entry.member == candidate.member
				&& entry.kind == candidate.kind
				&& entry.signature == candidate.signature
				&& entry.isStatic == candidate.isStatic) {
				return;
			}
		}
		result.push(candidate);
	}

	static function sourceDeclaresStatic(field:ClassField):Bool {
		final position = Context.getPosInfos(field.pos);
		final length = position.max - position.min;
		if (position.min < 0 || length < 0) {
			Context.fatalError("stdlib probe received an invalid abstract-field source range", field.pos);
		}
		final bytes = File.getBytes(position.file);
		if (position.max > bytes.length) {
			Context.fatalError("stdlib probe abstract-field source range exceeds its file", field.pos);
		}
		return containsToken(bytes.sub(position.min, length).toString(), "static");
	}

	static function containsToken(source:String, token:String):Bool {
		var offset = 0;
		while (offset <= source.length - token.length) {
			final found = source.indexOf(token, offset);
			if (found == -1) {
				return false;
			}
			final before = found == 0 ? -1 : source.charCodeAt(found - 1);
			final afterOffset = found + token.length;
			final after = afterOffset == source.length ? -1 : source.charCodeAt(afterOffset);
			if (!identifierCode(before) && !identifierCode(after)) {
				return true;
			}
			offset = found + token.length;
		}
		return false;
	}

	static function identifierCode(code:Int):Bool {
		return (code >= "a".code && code <= "z".code)
			|| (code >= "A".code && code <= "Z".code)
			|| (code >= "0".code && code <= "9".code)
			|| code == "_".code;
	}

	static function addTypeApi(source:StdlibProbeSource, typeName:String, kind:String, result:Array<StdlibProbeApi>):Void {
		result.push({
			module: source.module,
			source: source.path,
			type: typeName,
			member: null,
			kind: kind,
			signature: typeName,
			isStatic: true
		});
	}

	static function addFieldApis(source:StdlibProbeSource, typeName:String, field:ClassField, isStatic:Bool, kind:String, result:Array<StdlibProbeApi>):Void {
		if (!field.isPublic || field.meta.has(":noCompletion")) {
			return;
		}
		result.push(api(source, typeName, kind == "constructor" ? "new" : field.name, kind, field.type, isStatic));
		for (overloadField in field.overloads.get()) {
			if (overloadField.isPublic && !overloadField.meta.has(":noCompletion")) {
				result.push(api(source, typeName, overloadField.name, "overload", overloadField.type, isStatic));
			}
		}
	}

	static function api(source:StdlibProbeSource, typeName:String, member:String, kind:String, signature:Type, isStatic:Bool):StdlibProbeApi {
		return apiWithSignature(source, typeName, member, kind, TypeTools.toString(signature), isStatic);
	}

	static function apiWithSignature(source:StdlibProbeSource, typeName:String, member:String, kind:String, signature:String, isStatic:Bool):StdlibProbeApi {
		return {
			module: source.module,
			source: source.path,
			type: typeName,
			member: member,
			kind: kind,
			signature: signature,
			isStatic: isStatic
		};
	}

	static function isPublicType(type:BaseType):Bool {
		return !type.isPrivate && !type.meta.has(":noCompletion");
	}

	static function fieldKind(field:ClassField):String {
		return switch field.kind {
			case FVar(_, _): "field";
			case FMethod(MethNormal): "method";
			case FMethod(MethInline): "inline-method";
			case FMethod(MethDynamic): "dynamic-method";
			case FMethod(MethMacro): "macro-method";
		}
	}

	static function typePath(pack:Array<String>, name:String):String {
		return pack.length == 0 ? name : pack.join(".") + "." + name;
	}

	static function compareApis(left:StdlibProbeApi, right:StdlibProbeApi):Int {
		final leftKey = left.module + "\n" + left.type + "\n" + (left.member == null ? "" : left.member) + "\n" + left.kind + "\n"
			+ (left.isStatic ? "1" : "0") + "\n" + left.signature;
		final rightKey = right.module + "\n" + right.type + "\n" + (right.member == null ? "" : right.member) + "\n" + right.kind + "\n"
			+ (right.isStatic ? "1" : "0") + "\n" + right.signature;
		return compareStrings(leftKey, rightKey);
	}

	static function compareStrings(left:String, right:String):Int {
		return left < right ? -1 : (left > right ? 1 : 0);
	}
	#else
	public static function install():Void {}
	#end
}

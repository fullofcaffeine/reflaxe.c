package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueKind;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyEmissionError;

/**
	Recognizes Haxe's two typed spellings for one String-keyed Map value.

	Public annotations normally remain `Map<String, V>`, while construction and
	method bodies use `haxe.ds.StringMap<V>`. Both spellings describe the same
	shared mutable object. This module preserves that identity before following
	the `Map` abstract would erase it to the generic `haxe.IMap` interface.
**/
/** Module anchor for String-keyed Haxe Map recognition and representation. */
class CBodyStringMap {
	private function new() {}
}

/** Callback that maps one Haxe Map value type through the shared body-type boundary. */
typedef CBodyStringMapValueResolver = (Type, Position, String, String, (Position, String) -> Void, String) -> CBodyValueType;

/**
	One exact `Map<String, V>` specialization before C syntax is selected.

	The reference kind is intentional: assigning a Haxe Map aliases one table; it
	does not copy its entries. The two IR arguments keep the String key and exact
	value representation visible to validators and later runtime selection.
**/
class CPreparedBodyStringMap {
	/** Stable semantic identity used to deduplicate equivalent specializations. */
	public final semanticKey:String;

	/** Deterministic digest used in internal declaration and instance IDs. */
	public final digest:String;

	/** HxcIR declaration ID for this exact Map specialization. */
	public final declarationId:String;

	/** HxcIR instance ID that carries the String and value type arguments. */
	public final instanceId:String;

	/** Exact unboxed value representation stored in each table slot. */
	public final value:CBodyValueType;

	/** Source module that first requested this request-local specialization. */
	public final ownerModule:String;

	/** Stable source span used by diagnostics and deterministic HxcIR dumps. */
	public final source:HxcSourceSpan;

	/** Original macro position retained for source-positioned rejection. */
	public final position:Position;

	/** Create one immutable plan after its key and value contracts are validated. */
	public function new(semanticKey:String, digest:String, value:CBodyValueType, ownerModule:String, source:HxcSourceSpan, position:Position) {
		this.semanticKey = semanticKey;
		this.digest = digest;
		this.declarationId = 'type.haxe-string-map.$digest';
		this.instanceId = 'instance.haxe-string-map.$digest';
		this.value = value;
		this.ownerModule = ownerModule;
		this.source = source;
		this.position = position;
	}

	/** Describe the shared Map object as a reference type before choosing C syntax. */
	public function declaration():HxcIRTypeDeclaration
		return {
			id: declarationId,
			displayName: 'Map<String, ${value.cSpelling}>',
			kind: IRTKReference,
			source: source
		};

	/** Preserve exact String/Bool arguments and the managed StringMap runtime intent. */
	public function instance():HxcIRTypeInstance
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [IRTString, value.irType],
			representation: IRRManaged("string-map"),
			source: source
		};
}

/** Request-local registry for exact String-keyed Map value specializations. */
class CBodyStringMapRegistry {
	final resolveValue:CBodyStringMapValueResolver;
	final bySemanticKey:Map<String, CPreparedBodyStringMap> = [];
	final semanticKeysByDigest:Map<String, String> = [];

	/** Create request-local specialization storage around the shared value resolver. */
	public function new(resolveValue:CBodyStringMapValueResolver) {
		this.resolveValue = resolveValue;
	}

	/** Return null when the type is not Haxe's Map<String, V> specialization. */
	public function valueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):Null<CPreparedBodyStringMap> {
		final parameters = mapParameters(type);
		if (parameters == null)
			return null;
		if (parameters.length != 2)
			return rejected(fail, position, '$node:StringMap-arity:${parameters.length}');
		if (!isString(parameters[0]))
			return rejected(fail, position, '$node:StringMap-key-not-String');
		final value = resolveValue(parameters[1], position, ownerModule, sourcePath, fail, '$node.StringMap-value');
		// The first vertical slice intentionally owns Bool completely. Adding a
		// value category means adding its copy, replacement, lookup, and cleanup
		// contracts together rather than letting a void-pointer table erase them.
		switch value.kind {
			case CBVKPrimitive(mapping) if (mapping.irType == IRTBool):
			case _:
				return rejected(fail, position, '$node:StringMap-value-not-yet-admitted:${value.cSpelling}');
		}
		final semanticKey = 'haxe-string-map-v1(${canonicalPart(value.cSpelling)})';
		final existing = bySemanticKey.get(semanticKey);
		if (existing != null)
			return existing;
		final digest = Sha256.encode(semanticKey);
		final prior = semanticKeysByDigest.get(digest);
		if (prior != null && prior != semanticKey)
			throw new CBodyEmissionError('StringMap specialization digest collision `$digest` between `$prior` and `$semanticKey`');
		semanticKeysByDigest.set(digest, semanticKey);
		final prepared = new CPreparedBodyStringMap(semanticKey, digest, value, ownerModule, HaxeSourceSpan.fromPosition(position, sourcePath), position);
		bySemanticKey.set(semanticKey, prepared);
		return prepared;
	}

	/** Return all prepared maps in deterministic digest order for HxcIR assembly. */
	public function canonicalMaps():Array<CPreparedBodyStringMap> {
		final values = [for (value in bySemanticKey) value];
		values.sort((left, right) -> compareUtf8(left.digest, right.digest));
		return values;
	}

	static function mapParameters(type:Type):Null<Array<Type>>
		return switch type {
			case TAbstract(reference, parameters) if (isMapAbstract(reference.get())):
				parameters.length == 1 ? [ContextStringType.value(), parameters[0]] : parameters;
			case TInst(reference, parameters) if (isPath(reference.get().pack, reference.get().name, ["haxe", "ds"], "StringMap")):
				[ContextStringType.value()].concat(parameters);
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : mapParameters(resolved);
			case TLazy(resolve):
				mapParameters(resolve());
			case TType(reference, parameters):
				final definition = reference.get();
				mapParameters(TypeTools.applyTypeParameters(definition.type, definition.params, parameters));
			case _:
				null;
		};

	static function isString(type:Type):Bool
		return switch TypeTools.follow(type) {
			case TInst(reference, _): isPath(reference.get().pack, reference.get().name, [], "String");
			case TAbstract(reference, _): isPath(reference.get().pack, reference.get().name, [], "String");
			case _: false;
		};

	static function isPath(pack:Array<String>, name:String, expectedPack:Array<String>, expectedName:String):Bool
		return name == expectedName && pack.join(".") == expectedPack.join(".");

	static function isMapAbstract(value:AbstractType):Bool
		return value.name == "Map" && (value.pack.length == 0 || value.pack.join(".") == "haxe.ds");

	static function canonicalPart(value:String):String {
		final bytes = Bytes.ofString(value);
		return '${bytes.length}:$value';
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("StringMap rejection callback returned unexpectedly");
	}
}

/**
	Provides the canonical `String` type without allowing an untyped boundary to
	escape this recognition module.
**/
private class ContextStringType {
	public static function value():Type
		return haxe.macro.Context.getType("String");
}

/** Exact owner recognition keeps StringMap calls out of class dispatch. */
class CBodyStringMapRecognition {
	/** True only for Haxe's concrete `haxe.ds.StringMap` implementation class. */
	public static function isStringMap(reference:Ref<ClassType>):Bool {
		final value = reference.get();
		return value.pack.join(".") == "haxe.ds" && value.name == "StringMap";
	}
}
#else
class CBodyStringMap {
	private function new() {}
}
#end

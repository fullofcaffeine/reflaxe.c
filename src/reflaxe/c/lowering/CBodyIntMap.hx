package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CompilationContext;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueKind;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyEmissionError;

/**
	Recognizes the first integer-keyed Haxe Map specialization owned by haxe.c.

	Haxe writes the source type as `Map<Int, Bool>` but specializes its methods
	through `haxe.ds.IntMap<Bool>` in the typed tree. Both spellings name one
	shared mutable object. This module captures that exact identity before the
	generic `haxe.IMap` interface can hide the key and value types.

	This first slice deliberately admits only `Bool` values. The runtime can
	store membership without boxing, while unsupported value types still stop at
	their source use instead of entering an untyped `void *` boundary.
**/
class CBodyIntMap {
	private function new() {}
}

/** Callback that maps one Haxe value type through the shared body-type boundary. */
typedef CBodyIntMapValueResolver = (Type, Position, String, String, (Position, String) -> Void, String) -> CBodyValueType;

/**
	One exact `Map<Int, Bool>` specialization before C syntax is selected.

	The reference kind matters: assigning a Haxe Map aliases the same table; it
	does not copy its entries. The HxcIR arguments keep both `Int` and `Bool`
	visible so validation cannot confuse this bounded family with another map.
**/
class CPreparedBodyIntMap {
	/** Stable semantic identity used to deduplicate this request-local plan. */
	public final semanticKey:String;

	/** Deterministic digest used by internal declaration and instance IDs. */
	public final digest:String;

	/** HxcIR declaration ID for the shared map object. */
	public final declarationId:String;

	/** HxcIR instance ID carrying the exact key and value types. */
	public final instanceId:String;

	/** Exact unboxed value type; currently required to be Haxe `Bool`. */
	public final value:CBodyValueType;

	/** Source module that first requested the specialization. */
	public final ownerModule:String;

	/** Stable source span for validation and deterministic reports. */
	public final source:HxcSourceSpan;

	/** Original compiler position retained for precise rejection. */
	public final position:Position;

	/** Create the immutable semantic plan after key and value validation. */
	public function new(semanticKey:String, digest:String, value:CBodyValueType, ownerModule:String, source:HxcSourceSpan, position:Position) {
		this.semanticKey = semanticKey;
		this.digest = digest;
		this.declarationId = 'type.haxe-int-map.$digest';
		this.instanceId = 'instance.haxe-int-map.$digest';
		this.value = value;
		this.ownerModule = ownerModule;
		this.source = source;
		this.position = position;
	}

	/** Describe the shared Map object without selecting its C pointer spelling. */
	public function declaration():HxcIRTypeDeclaration
		return {
			id: declarationId,
			displayName: "Map<Int, Bool>",
			kind: IRTKReference,
			source: source
		};

	/** Preserve the exact types and the managed IntMap runtime intent. */
	public function instance():HxcIRTypeInstance
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [IRTInt(32, true), IRTBool],
			representation: IRRManaged("int-map"),
			source: source
		};
}

/** Request-local registry for the bounded integer-keyed Map specialization. */
class CBodyIntMapRegistry {
	final resolveValue:CBodyIntMapValueResolver;
	final bySemanticKey:Map<String, CPreparedBodyIntMap> = [];

	/** Create isolated specialization storage for one compiler request. */
	public function new(context:CompilationContext, resolveValue:CBodyIntMapValueResolver) {
		this.resolveValue = resolveValue;
	}

	/**
		Return null for unrelated types and reject unsupported IntMap values.

		`Map<Int, Bool>` and `haxe.ds.IntMap<Bool>` converge here. A Map with a
		different key belongs to another family; an IntMap with a different value
		is recognized but rejected so it cannot fall through to class dispatch.
	**/
	public function valueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):Null<CPreparedBodyIntMap> {
		final parameters = mapParameters(type);
		if (parameters == null)
			return null;
		if (parameters.length != 2)
			return rejected(fail, position, '$node:IntMap-arity:${parameters.length}');
		if (!isInt(parameters[0]))
			return null;
		final value = resolveValue(parameters[1], position, ownerModule, sourcePath, fail, '$node.IntMap-value');
		switch value.kind {
			case CBVKPrimitive(mapping) if (mapping.irType == IRTBool):
			case _:
				return rejected(fail, position, '$node:IntMap-value-not-yet-admitted:${value.cSpelling}');
		}
		final semanticKey = "haxe-int-map-v1(Bool)";
		final existing = bySemanticKey.get(semanticKey);
		if (existing != null)
			return existing;
		final digest = Sha256.encode(semanticKey);
		final prepared = new CPreparedBodyIntMap(semanticKey, digest, value, ownerModule, HaxeSourceSpan.fromPosition(position, sourcePath), position);
		bySemanticKey.set(semanticKey, prepared);
		return prepared;
	}

	/** Return the one specialization in deterministic semantic-key order. */
	public function canonicalMaps():Array<CPreparedBodyIntMap> {
		final values = [for (value in bySemanticKey) value];
		values.sort((left, right) -> compareUtf8(left.semanticKey, right.semanticKey));
		return values;
	}

	/** Recover exact Map key/value arguments without following away Map identity. */
	static function mapParameters(type:Type):Null<Array<Type>>
		return switch type {
			case TAbstract(reference, parameters) if (isMapAbstract(reference.get())):
				parameters.length == 1 ? [ContextIntType.value(), parameters[0]] : parameters;
			case TInst(reference, parameters) if (isPath(reference.get().pack, reference.get().name, ["haxe", "ds"], "IntMap")):
				[ContextIntType.value()].concat(parameters);
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

	static function isInt(type:Type):Bool
		return switch TypeTools.follow(type) {
			case TAbstract(reference, _): isPath(reference.get().pack, reference.get().name, [], "Int");
			case _: false;
		};

	static function isPath(pack:Array<String>, name:String, expectedPack:Array<String>, expectedName:String):Bool
		return name == expectedName && pack.join(".") == expectedPack.join(".");

	static function isMapAbstract(value:AbstractType):Bool
		return value.name == "Map" && (value.pack.length == 0 || value.pack.join(".") == "haxe.ds");

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
		throw new CBodyEmissionError("IntMap rejection callback returned unexpectedly");
	}
}

/** Provides the canonical core `Int` type without introducing an untyped value. */
private class ContextIntType {
	public static function value():Type
		return haxe.macro.Context.getType("Int");
}

/** Exact owner recognition keeps IntMap calls out of ordinary class dispatch. */
class CBodyIntMapRecognition {
	/** True only for Haxe's standard `haxe.ds.IntMap` implementation class. */
	public static function isIntMap(reference:Ref<ClassType>):Bool {
		final value = reference.get();
		return value.pack.join(".") == "haxe.ds" && value.name == "IntMap";
	}

	/** Recognize the generic interface view inserted around a specialized Map. */
	public static function isIMapType(type:Type):Bool
		return switch TypeTools.follow(type) {
			case TInst(reference, _): final value = reference.get(); value.pack.join(".") == "haxe" && value.name == "IMap";
			case _:
				false;
		};
}
#else
class CBodyIntMap {
	private function new() {}
}
#end

package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyEmissionError;

/**
	Owns the exact standard `Iterator<T>` representation used by collection APIs.

	Haxe defines Iterator as a structural typedef, but map-produced iterators need
	one shared cursor that can cross ordinary calls and returns. This registry
	recognizes the standard typedef before anonymous-record lowering can mistake
	its two methods for stored fields. Each element specialization remains visible
	in HxcIR while all specializations share one opaque runtime reference carrier.
**/
class CBodyIterator {
	private function new() {}
}

/** Maps one iterator element through the normal typed body-value boundary. */
typedef CBodyIteratorElementResolver = (Type, Position, String, String, (Position, String) -> Void, String) -> CBodyValueType;

/** One exact standard `Iterator<T>` specialization before C syntax is selected. */
class CPreparedBodyIterator {
	public final semanticKey:String;
	public final digest:String;
	public final declarationId:String;
	public final instanceId:String;
	public final element:CBodyValueType;
	public final ownerModule:String;
	public final source:HxcSourceSpan;
	public final position:Position;

	/** Create the immutable typed carrier plan after validating its element. */
	public function new(semanticKey:String, digest:String, element:CBodyValueType, ownerModule:String, source:HxcSourceSpan, position:Position) {
		this.semanticKey = semanticKey;
		this.digest = digest;
		this.declarationId = 'type.haxe-iterator.$digest';
		this.instanceId = 'instance.haxe-iterator.$digest';
		this.element = element;
		this.ownerModule = ownerModule;
		this.source = source;
		this.position = position;
	}

	/** Describe the shared-cursor reference without choosing its C pointer type. */
	public function declaration():HxcIRTypeDeclaration
		return {
			id: declarationId,
			displayName: 'Iterator<${element.cSpelling}>',
			kind: IRTKReference,
			source: source
		};

	/** Preserve the unboxed element type and managed iterator runtime intent. */
	public function instance():HxcIRTypeInstance
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [element.irType],
			representation: IRRManaged("iterator"),
			source: source
		};
}

/** Request-local registry for exact standard Iterator specializations. */
class CBodyIteratorRegistry {
	final resolveElement:CBodyIteratorElementResolver;
	final bySemanticKey:Map<String, CPreparedBodyIterator> = [];

	/** Create isolated iterator specialization storage for one compile request. */
	public function new(resolveElement:CBodyIteratorElementResolver) {
		this.resolveElement = resolveElement;
	}

	/** Count prepared Iterator specializations without exposing compiler objects. */
	@:noCompletion
	public function preparedCount():Int {
		var count = 0;
		for (_ in bySemanticKey)
			count++;
		return count;
	}

	/**
		Return null for unrelated types and map only the standard Iterator typedef.

		Arbitrary structures that happen to provide `hasNext` and `next` remain
		outside this runtime carrier. Their methods may own different state and must
		not silently acquire map-snapshot semantics.
	**/
	public function valueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):Null<CPreparedBodyIterator> {
		final parameters = iteratorParameters(type);
		if (parameters == null)
			return null;
		if (parameters.length != 1)
			return rejected(fail, position, '$node:Iterator-arity:${parameters.length}');
		final element = resolveElement(parameters[0], position, ownerModule, sourcePath, fail, '$node.Iterator-element');
		final semanticKey = 'haxe-iterator-v1(${canonicalPart(element.cSpelling)})';
		final existing = bySemanticKey.get(semanticKey);
		if (existing != null)
			return existing;
		final digest = Sha256.encode(semanticKey);
		final prepared = new CPreparedBodyIterator(semanticKey, digest, element, ownerModule, HaxeSourceSpan.fromPosition(position, sourcePath), position);
		bySemanticKey.set(semanticKey, prepared);
		return prepared;
	}

	/** Return iterator plans in deterministic semantic-key order. */
	public function canonicalIterators():Array<CPreparedBodyIterator> {
		final values = [for (value in bySemanticKey) value];
		values.sort((left, right) -> reflaxe.c.CUtf8Order.compare(left.semanticKey, right.semanticKey));
		return values;
	}

	static function iteratorParameters(type:Type):Null<Array<Type>>
		return switch type {
			case TType(reference, parameters) if (reference.get().pack.length == 0 && reference.get().name == "Iterator"):
				parameters;
			case TType(reference, parameters) if (reference.get().pack.length == 0 && reference.get().name == "KeyValueIterator"):
				final definition = reference.get();
				iteratorParameters(TypeTools.applyTypeParameters(definition.type, definition.params, parameters));
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : iteratorParameters(resolved);
			case TLazy(resolve): iteratorParameters(resolve());
			case _: null;
		};

	static function canonicalPart(value:String):String {
		final bytes = Bytes.ofString(value);
		return '${bytes.length}:$value';
	}

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("Iterator rejection callback returned unexpectedly");
	}
}
#else
class CBodyIterator {
	private function new() {}
}
#end

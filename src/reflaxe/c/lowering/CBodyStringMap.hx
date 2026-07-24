package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CPreparedBodyAggregate;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueKind;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRepresentation;
import reflaxe.c.lowering.CBodyEmissionError;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;

/**
	Recognizes Haxe's two typed spellings for one String-keyed Map value.

	Public annotations normally remain `Map<String, V>`, while construction and
	method bodies use `haxe.ds.StringMap<V>`. Both spellings describe the same
	shared mutable object. This module preserves that identity before following
	the `Map` abstract would erase it to the generic `haxe.IMap` interface.
**/
class CBodyStringMap {
	private function new() {}
}

/** Callback that maps one Haxe Map value type through the shared body-type boundary. */
typedef CBodyStringMapValueResolver = (Type, Position, String, String, (Position, String) -> Void, String) -> CBodyValueType;

/** The complete slot-lifetime strategy for one admitted StringMap value. */
enum CBodyStringMapValueLifecycle {
	/** The value and every nested field can be copied and relocated as bytes. */
	CBSMVLTrivial;

	/** A closed record owns one or more reference-counted nested values. */
	CBSMVLAggregate(value:CPreparedBodyAggregate);
}

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

	/** Copy/assign/destroy policy selected before the runtime table is emitted. */
	public final lifecycle:CBodyStringMapValueLifecycle;

	/** Source module that first requested this request-local specialization. */
	public final ownerModule:String;

	/** Stable source span used by diagnostics and deterministic HxcIR dumps. */
	public final source:HxcSourceSpan;

	/** Original macro position retained for source-positioned rejection. */
	public final position:Position;

	/** Program-local callbacks are present together for a managed record value. */
	public final copyRequest:Null<CSymbolRequest>;

	public final assignRequest:Null<CSymbolRequest>;
	public final destroyRequest:Null<CSymbolRequest>;
	public final copyParameterRequests:Array<CSymbolRequest>;
	public final assignParameterRequests:Array<CSymbolRequest>;
	public final destroyParameterRequests:Array<CSymbolRequest>;
	public final copyStatusRequest:Null<CSymbolRequest>;
	public final assignStatusRequest:Null<CSymbolRequest>;

	/** Create one immutable plan after its key and value contracts are validated. */
	public function new(semanticKey:String, digest:String, value:CBodyValueType, lifecycle:CBodyStringMapValueLifecycle, ownerModule:String,
			source:HxcSourceSpan, position:Position, copyRequest:Null<CSymbolRequest>, assignRequest:Null<CSymbolRequest>,
			destroyRequest:Null<CSymbolRequest>, copyParameterRequests:Array<CSymbolRequest>, assignParameterRequests:Array<CSymbolRequest>,
			destroyParameterRequests:Array<CSymbolRequest>, copyStatusRequest:Null<CSymbolRequest>, assignStatusRequest:Null<CSymbolRequest>) {
		this.semanticKey = semanticKey;
		this.digest = digest;
		this.declarationId = 'type.haxe-string-map.$digest';
		this.instanceId = 'instance.haxe-string-map.$digest';
		this.value = value;
		this.lifecycle = lifecycle;
		this.ownerModule = ownerModule;
		this.source = source;
		this.position = position;
		this.copyRequest = copyRequest;
		this.assignRequest = assignRequest;
		this.destroyRequest = destroyRequest;
		this.copyParameterRequests = copyParameterRequests.copy();
		this.assignParameterRequests = assignParameterRequests.copy();
		this.destroyParameterRequests = destroyParameterRequests.copy();
		this.copyStatusRequest = copyStatusRequest;
		this.assignStatusRequest = assignStatusRequest;
	}

	/** Describe the shared Map object as a reference type before choosing C syntax. */
	public function declaration():HxcIRTypeDeclaration
		return {
			id: declarationId,
			displayName: 'Map<String, ${value.cSpelling}>',
			kind: IRTKReference,
			source: source
		};

	/** Preserve exact String/value arguments and the managed StringMap runtime intent. */
	public function instance():HxcIRTypeInstance
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [IRTString, value.irType],
			representation: IRRManaged("string-map"),
			source: source
		};

	/** True when slot copies must run program-local ownership callbacks. */
	public function hasLifecycle():Bool
		return lifecycle != CBSMVLTrivial;
}

/**
	Names one exact StringMap specialization after collision-safe C naming.

	Prepared plans describe ownership without choosing final C identifiers. This
	value joins each managed callback and its locals with the identifiers chosen
	by the shared symbol registry, so emission does not repeat naming policy.
**/
class CLoweredBodyStringMap {
	/** Source/IR plan whose semantic decisions remain authoritative. */
	public final prepared:CPreparedBodyStringMap;

	/** Copy-construct callback, or null for a byte-copyable value. */
	public final copyName:Null<CIdentifier>;

	/** Failure-atomic replacement callback, or null for a byte-copyable value. */
	public final assignName:Null<CIdentifier>;

	/** Destruction callback, or null for a byte-copyable value. */
	public final destroyName:Null<CIdentifier>;

	/** Final names for the copy callback's context, destination, and source. */
	public final copyParameterNames:Array<CIdentifier>;

	/** Final names for the assign callback's context, destination, and source. */
	public final assignParameterNames:Array<CIdentifier>;

	/** Final names for the destroy callback's context and value. */
	public final destroyParameterNames:Array<CIdentifier>;

	/** Local status used to roll back a failed record copy. */
	public final copyStatusName:Null<CIdentifier>;

	/** Local status used to preserve an old record when replacement fails. */
	public final assignStatusName:Null<CIdentifier>;

	/** Join one prepared plan with the finalized identifiers it requested. */
	public function new(prepared:CPreparedBodyStringMap, copyName:Null<CIdentifier>, assignName:Null<CIdentifier>, destroyName:Null<CIdentifier>,
			copyParameterNames:Array<CIdentifier>, assignParameterNames:Array<CIdentifier>, destroyParameterNames:Array<CIdentifier>,
			copyStatusName:Null<CIdentifier>, assignStatusName:Null<CIdentifier>) {
		this.prepared = prepared;
		this.copyName = copyName;
		this.assignName = assignName;
		this.destroyName = destroyName;
		this.copyParameterNames = copyParameterNames.copy();
		this.assignParameterNames = assignParameterNames.copy();
		this.destroyParameterNames = destroyParameterNames.copy();
		this.copyStatusName = copyStatusName;
		this.assignStatusName = assignStatusName;
	}

	/** Managed strategies always own the complete callback trio. */
	public function hasLifecycle():Bool
		return prepared.hasLifecycle();
}

/** Request-local registry for exact String-keyed Map value specializations. */
class CBodyStringMapRegistry {
	final context:CompilationContext;
	final resolveValue:CBodyStringMapValueResolver;
	final bySemanticKey:Map<String, CPreparedBodyStringMap> = [];
	final semanticKeysByDigest:Map<String, String> = [];

	/** Create request-local specialization storage around the shared value resolver. */
	public function new(context:CompilationContext, resolveValue:CBodyStringMapValueResolver) {
		this.context = context;
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
			return null;
		final value = resolveValue(parameters[1], position, ownerModule, sourcePath, fail, '$node.StringMap-value');
		final lifecycle = valueLifecycle(value);
		if (lifecycle == null)
			return rejected(fail, position, '$node:StringMap-value-not-yet-admitted:${value.cSpelling}');
		final semanticKey = 'haxe-string-map-v1(${canonicalPart(value.cSpelling)})';
		final existing = bySemanticKey.get(semanticKey);
		if (existing != null)
			return existing;
		final digest = Sha256.encode(semanticKey);
		final prior = semanticKeysByDigest.get(digest);
		if (prior != null && prior != semanticKey)
			throw new CBodyEmissionError('StringMap specialization digest collision `$digest` between `$prior` and `$semanticKey`');
		semanticKeysByDigest.set(digest, semanticKey);
		var copyRequest:Null<CSymbolRequest> = null;
		var assignRequest:Null<CSymbolRequest> = null;
		var destroyRequest:Null<CSymbolRequest> = null;
		var copyParameters:Array<CSymbolRequest> = [];
		var assignParameters:Array<CSymbolRequest> = [];
		var destroyParameters:Array<CSymbolRequest> = [];
		var copyStatus:Null<CSymbolRequest> = null;
		var assignStatus:Null<CSymbolRequest> = null;
		if (lifecycle != CBSMVLTrivial) {
			copyRequest = lifecycleRequest(digest, "copy", 0);
			assignRequest = lifecycleRequest(digest, "assign", 1);
			destroyRequest = lifecycleRequest(digest, "destroy", 2);
			context.symbols.register(copyRequest);
			context.symbols.register(assignRequest);
			context.symbols.register(destroyRequest);
			copyParameters = registerParameters(copyRequest, ["context", "destination", "source"]);
			assignParameters = registerParameters(assignRequest, ["context", "destination", "source"]);
			destroyParameters = registerParameters(destroyRequest, ["context", "value"]);
			copyStatus = registerLocal(copyRequest, "operation_status", 0);
			assignStatus = registerLocal(assignRequest, "operation_status", 0);
		}
		final prepared = new CPreparedBodyStringMap(semanticKey, digest, value, lifecycle, ownerModule, HaxeSourceSpan.fromPosition(position, sourcePath),
			position, copyRequest, assignRequest, destroyRequest, copyParameters, assignParameters, destroyParameters, copyStatus, assignStatus);
		bySemanticKey.set(semanticKey, prepared);
		return prepared;
	}

	/**
		Admit only value families whose complete slot semantics are owned here.

		Bool and Haxe `Int` use their already-validated direct C scalar
		representations. A closed record is admitted only when every nested value
		already has a finite, byte-relocatable direct representation and no
		collector tracing is required. Managed records use the record's proven
		retain/destroy plan.

		A payload-free Haxe enum is also a direct value: its prepared enum plan
		selects one native C `enum`, so copying or moving a slot copies only that
		closed discriminant. A tagged enum is deliberately different because its
		active payload may need ownership or tracing work. Unsupported references
		and tagged enums still fail before the runtime's `void *` ABI can erase
		their exact type and lifetime rules.
	**/
	static function valueLifecycle(value:CBodyValueType):Null<CBodyStringMapValueLifecycle>
		return switch value.kind {
			case CBVKPrimitive(mapping):
				switch mapping.irType {
					case IRTBool | IRTInt(32, true): CBSMVLTrivial;
					case _: null;
				}
			case CBVKEnum(enumeration) if (enumeration.representation == CBERNativeEnum): CBSMVLTrivial;
			case CBVKAggregate(aggregate) if (aggregateIsStorable(aggregate)):
				aggregate.managedLifetime ? CBSMVLAggregate(aggregate) : CBSMVLTrivial;
			case _: null;
		};

	/**
		Prove that every field in one record has a finite direct representation.

		An empty Haxe anonymous structure has no useful admitted map value shape.
		Every real field must pass the same recursive rule before the map is
		allowed to store the enclosing record by value.
	**/
	static function aggregateIsStorable(value:CPreparedBodyAggregate):Bool {
		if (value.fields.length == 0)
			return false;
		for (field in value.fields)
			if (!nestedValueIsStorable(field.type))
				return false;
		return true;
	}

	/**
		Reject nested values whose lifetime needs a tracing collector or borrow.

		The accepted cases already own complete direct copy/destroy semantics.
		This recursive proof is intentionally narrower than “the compiler knows
		this type”: knowing a class or StringMap carrier is not enough to make it
		safe as an unboxed record field inside another StringMap slot.
	**/
	static function nestedValueIsStorable(value:CBodyValueType):Bool
		return switch value.kind {
			case CBVKPrimitive(mapping): mapping.irType != IRTVoid && mapping.irType != IRTString && mapping.irType != IRTManagedString;
			case CBVKStaticString(_) | CBVKManagedString(_) | CBVKImport(_): true;
			case CBVKAggregate(aggregate): aggregateIsStorable(aggregate);
			case CBVKArray(array): !array.managedByCollector;
			case CBVKBytes(_): true;
			case CBVKEnum(enumeration): !enumeration.collectorPayload;
			case CBVKOptional(optional): nestedValueIsStorable(optional.payload);
			case _: false;
		};

	/** Request one internal callback name under the exact specialization key. */
	function lifecycleRequest(digest:String, operation:String, ordinal:Int):CSymbolRequest
		return new CSymbolRequest(CSKMethod, ["compiler", "haxe-string-map", digest, "value", operation], CNSOrdinary("translation-unit"), CSVInternal, null,
			[], [], ordinal, ["string", "map", digest.substr(0, 8), "value", operation]);

	/** Register callback parameters in semantic role order, independent of spelling. */
	function registerParameters(owner:CSymbolRequest, roles:Array<String>):Array<CSymbolRequest> {
		final result:Array<CSymbolRequest> = [];
		for (index => role in roles) {
			final request = new CSymbolRequest(CSKLocal, owner.qualifiedName.concat([role]), CNSOrdinary(owner.stableKey()), CSVInternal, null, [], [], index,
				[role]);
			context.symbols.register(request);
			result.push(request);
		}
		return result;
	}

	/** Register one callback-local value inside its collision-safe C namespace. */
	function registerLocal(owner:CSymbolRequest, role:String, ordinal:Int):CSymbolRequest {
		final request = new CSymbolRequest(CSKLocal, owner.qualifiedName.concat([role]), CNSOrdinary(owner.stableKey()), CSVInternal, null, [], [], ordinal,
			[role]);
		context.symbols.register(request);
		return request;
	}

	/** Return all prepared maps in deterministic digest order for HxcIR assembly. */
	public function canonicalMaps():Array<CPreparedBodyStringMap> {
		final values = [for (value in bySemanticKey) value];
		values.sort((left, right) -> compareUtf8(left.digest, right.digest));
		return values;
	}

	/** Resolve every program-local value callback through the shared name registry. */
	public function finalize(symbols:CSymbolRegistry):Array<CLoweredBodyStringMap> {
		final result:Array<CLoweredBodyStringMap> = [];
		for (value in canonicalMaps()) {
			if (!value.hasLifecycle()) {
				result.push(new CLoweredBodyStringMap(value, null, null, null, [], [], [], null, null));
				continue;
			}
			if (value.copyRequest == null || value.assignRequest == null || value.destroyRequest == null)
				throw new CBodyEmissionError('managed StringMap specialization `${value.semanticKey}` lost its callback requests');
			if (value.copyStatusRequest == null || value.assignStatusRequest == null)
				throw new CBodyEmissionError('managed StringMap specialization `${value.semanticKey}` lost its callback locals');
			result.push(new CLoweredBodyStringMap(value, symbols.identifierFor(value.copyRequest), symbols.identifierFor(value.assignRequest),
				symbols.identifierFor(value.destroyRequest), value.copyParameterRequests.map(symbols.identifierFor),
				value.assignParameterRequests.map(symbols.identifierFor), value.destroyParameterRequests.map(symbols.identifierFor),
				symbols.identifierFor(value.copyStatusRequest), symbols.identifierFor(value.assignStatusRequest)));
		}
		return result;
	}

	/** Recover the exact key/value arguments from Map or concrete StringMap typing. */
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

	/**
		Recognize Haxe's generic Map interface without following away its identity.

		The frontend inserts this view around a class field declared as
		`Map<String, V>` before selecting `get`, `set`, and related methods. The
		compiler may erase that view only after the inner value has independently
		been proven to be an exact StringMap specialization.
	**/
	public static function isIMapType(type:Type):Bool
		return switch TypeTools.follow(type) {
			case TInst(reference, _): final value = reference.get(); value.pack.join(".") == "haxe" && value.name == "IMap";
			case _: false;
		};
}
#else
class CBodyStringMap {
	private function new() {}
}
#end

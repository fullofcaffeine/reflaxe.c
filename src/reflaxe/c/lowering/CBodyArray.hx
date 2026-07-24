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
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRepresentation;
import reflaxe.c.lowering.CBodyOptional.CPreparedBodyOptional;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;

/** Module anchor for ordinary Haxe `Array<T>` representation planning. */
class CBodyArray {
	private function new() {}
}

/** Resolve an element through the same closed body-type boundary as a field. */
typedef CBodyArrayElementResolver = (Type, Position, String, String, (Position, String) -> Void, String) -> CBodyValueType;

/** The smallest element-lifetime strategy required by one Array specialization. */
enum CBodyArrayElementLifecycle {
	/** Every element can be copied and relocated as plain bytes. */
	CBAELTrivial;

	/** One element is directly an owned `haxe.io.Bytes` reference. */
	CBAELBytes;

	/** One element is an immutable String view that may retain runtime storage. */
	CBAELString;

	/** One element is directly another shared Haxe `Array` reference. */
	CBAELArray;

	/** One element is an exact base pointer to a collector-managed Haxe class. */
	CBAELManagedClass(value:reflaxe.c.lowering.CBodyClass.CPreparedBodyClass);

	/** A closed record contains one or more supported managed fields. */
	CBAELAggregate(aggregate:CPreparedBodyAggregate);

	/** A tagged enum owns an `Array` only in the constructor selected by its tag. */
	CBAELEnum(value:reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance);

	/** A tagged optional owns its payload only while the presence flag is true. */
	CBAELOptional(value:CPreparedBodyOptional);
}

/**
	One closed `Array<T>` specialization before its C spelling is selected.

	Haxe Array values have shared identity: assigning one to another variable does
	not copy its elements. This managed instance therefore describes a reference
	to one resizable container. It must not become an inline `hxc_array`, because
	that runtime struct uniquely owns its allocation and an ordinary C copy would
	double-own the same storage.
**/
class CPreparedBodyArray {
	public final semanticKey:String;
	public final digest:String;
	public final declarationId:String;
	public final instanceId:String;
	public final element:CBodyValueType;
	public final ownerModule:String;
	public final source:HxcSourceSpan;
	public final position:Position;
	public final lifecycle:CBodyArrayElementLifecycle;
	public final copyRequest:Null<CSymbolRequest>;
	public final assignRequest:Null<CSymbolRequest>;
	public final destroyRequest:Null<CSymbolRequest>;
	public final copyParameterRequests:Array<CSymbolRequest>;
	public final assignParameterRequests:Array<CSymbolRequest>;
	public final destroyParameterRequests:Array<CSymbolRequest>;
	public final copyStatusRequest:Null<CSymbolRequest>;
	public final assignStatusRequest:Null<CSymbolRequest>;
	public final managedByCollector:Bool;
	public var descriptorRequest:Null<CSymbolRequest> = null;
	public var traceRequest:Null<CSymbolRequest> = null;
	public var finalizerRequest:Null<CSymbolRequest> = null;

	public function new(semanticKey:String, digest:String, element:CBodyValueType, ownerModule:String, source:HxcSourceSpan, position:Position,
			lifecycle:CBodyArrayElementLifecycle, copyRequest:Null<CSymbolRequest>, assignRequest:Null<CSymbolRequest>, destroyRequest:Null<CSymbolRequest>,
			copyParameterRequests:Array<CSymbolRequest>, assignParameterRequests:Array<CSymbolRequest>, destroyParameterRequests:Array<CSymbolRequest>,
			copyStatusRequest:Null<CSymbolRequest>, assignStatusRequest:Null<CSymbolRequest>) {
		this.semanticKey = semanticKey;
		this.digest = digest;
		this.declarationId = 'type.haxe-array.$digest';
		this.instanceId = 'instance.haxe-array.$digest';
		this.element = element;
		this.ownerModule = ownerModule;
		this.source = source;
		this.position = position;
		this.lifecycle = lifecycle;
		this.copyRequest = copyRequest;
		this.assignRequest = assignRequest;
		this.destroyRequest = destroyRequest;
		this.copyParameterRequests = copyParameterRequests.copy();
		this.assignParameterRequests = assignParameterRequests.copy();
		this.destroyParameterRequests = destroyParameterRequests.copy();
		this.copyStatusRequest = copyStatusRequest;
		this.assignStatusRequest = assignStatusRequest;
		this.managedByCollector = containsCollectorReference(element, []);
	}

	/** Keep shared identity explicit; later lowering owns allocation and lifetime. */
	public function declaration():HxcIRTypeDeclaration
		return {
			id: declarationId,
			displayName: 'Array<${element.cSpelling}>',
			kind: IRTKReference,
			source: source
		};

	public function instance():HxcIRTypeInstance
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [element.irType],
			representation: managedByCollector ? IRRManaged("gc") : IRRManaged("array"),
			source: source
		};

	/**
		Name the typed destroy plan used when Array indexing returns an owned copy.

		The element type remains in the identifier so HxcIR validation can prove
		that the cleanup place and the program-local callback agree before CAST is
		built. Trivial elements need no cleanup plan.
	**/
	public function destroyImplementationId():Null<String> {
		switch lifecycle {
			case CBAELTrivial | CBAELManagedClass(_):
				return null;
			case _:
		}
		return switch element.irType {
			case IRTInstance(instanceId): 'array-element-lifecycle:$instanceId:destroy';
			case IRTNullable(_, IRNTagged):
				final optional = element.optionalValue();
				if (optional == null || !optional.managedLifetime)
					throw new CBodyEmissionError('managed Array `${semanticKey}` lost its optional element plan');
				'array-element-lifecycle:${optional.planId}:destroy';
			case _:
				throw new CBodyEmissionError('managed Array `${semanticKey}` has a non-instance element type');
		};
	}

	public function managedElementClass():Null<reflaxe.c.lowering.CBodyClass.CPreparedBodyClass>
		return switch lifecycle {
			case CBAELManagedClass(value): value;
			case _: null;
		};

	/**
		Whether an element copied into this heap container can reach a GC object.

		The Array descriptor must trace these references even when the element is a
		by-value record, enum, or optional. Ownership callbacks are a separate fact:
		GC pointers copy trivially, while a mixed value may also own reference-counted
		Arrays or Bytes that still need copy/destroy callbacks.
	**/
	static function containsCollectorReference(type:CBodyValueType, visitedEnums:Map<String, Bool>):Bool {
		if (type.classValue() != null)
			return true;
		final array = type.arrayValue();
		if (array != null)
			return array.managedByCollector;
		final aggregate = type.aggregateValue();
		if (aggregate != null) {
			for (field in aggregate.fields)
				if (containsCollectorReference(field.type, visitedEnums))
					return true;
			return false;
		}
		final optional = type.optionalValue();
		if (optional != null)
			return containsCollectorReference(optional.payload, visitedEnums);
		final enumValue = type.enumValue();
		if (enumValue == null || visitedEnums.exists(enumValue.instanceId))
			return false;
		visitedEnums.set(enumValue.instanceId, true);
		return enumValue.collectorPayload;
	}
}

/** Finalized program-local names for one Array element-lifetime strategy. */
class CLoweredBodyArray {
	public final prepared:CPreparedBodyArray;
	public final copyName:Null<CIdentifier>;
	public final assignName:Null<CIdentifier>;
	public final destroyName:Null<CIdentifier>;
	public final copyParameterNames:Array<CIdentifier>;
	public final assignParameterNames:Array<CIdentifier>;
	public final destroyParameterNames:Array<CIdentifier>;
	public final copyStatusName:Null<CIdentifier>;
	public final assignStatusName:Null<CIdentifier>;
	public final descriptorName:Null<CIdentifier>;
	public final traceName:Null<CIdentifier>;
	public final finalizerName:Null<CIdentifier>;

	public function new(prepared:CPreparedBodyArray, copyName:Null<CIdentifier>, assignName:Null<CIdentifier>, destroyName:Null<CIdentifier>,
			copyParameterNames:Array<CIdentifier>, assignParameterNames:Array<CIdentifier>, destroyParameterNames:Array<CIdentifier>,
			copyStatusName:Null<CIdentifier>, assignStatusName:Null<CIdentifier>, descriptorName:Null<CIdentifier>, traceName:Null<CIdentifier>,
			finalizerName:Null<CIdentifier>) {
		this.prepared = prepared;
		this.copyName = copyName;
		this.assignName = assignName;
		this.destroyName = destroyName;
		this.copyParameterNames = copyParameterNames.copy();
		this.assignParameterNames = assignParameterNames.copy();
		this.destroyParameterNames = destroyParameterNames.copy();
		this.copyStatusName = copyStatusName;
		this.assignStatusName = assignStatusName;
		this.descriptorName = descriptorName;
		this.traceName = traceName;
		this.finalizerName = finalizerName;
	}

	/** Managed strategies always have the complete callback trio. */
	public function hasLifecycle():Bool
		return copyName != null;
}

/** Request-local, element-specialized Array identity registry. */
class CBodyArrayRegistry {
	final context:CompilationContext;
	final resolveElement:CBodyArrayElementResolver;
	final bySemanticKey:Map<String, CPreparedBodyArray> = [];
	final semanticKeysByDigest:Map<String, String> = [];

	public function new(context:CompilationContext, resolveElement:CBodyArrayElementResolver) {
		this.context = context;
		this.resolveElement = resolveElement;
	}

	/** Return null when `type` is not the standard Haxe `Array<T>`. */
	public function valueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):Null<CPreparedBodyArray> {
		return switch type {
			case TInst(reference, parameters) if (isCoreArray(reference.get())):
				require(parameters, position, ownerModule, sourcePath, fail, node);
			case _:
				null;
		};
	}

	function require(parameters:Array<Type>, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):CPreparedBodyArray {
		if (parameters.length != 1)
			return rejected(fail, position, '$node:Array-arity:${parameters.length}');
		final element = resolveElement(parameters[0], position, ownerModule, sourcePath, fail, '$node.Array-element');
		final lifecycle = elementLifecycle(element);
		if (lifecycle == null)
			return rejected(fail, position, '$node:Array-element-not-yet-admitted:${element.cSpelling}');
		// The source-level element identity is part of Array semantics even when two
		// element types happen to use the same C spelling. For example, a nominal
		// abstract and its underlying Int may both emit int32_t while still requiring
		// different Haxe method and assignment rules.
		final semanticKey = 'haxe-array-v1(${canonicalPart(elementSemanticKey(element))})';
		final existing = bySemanticKey.get(semanticKey);
		if (existing != null)
			return existing;
		final digest = Sha256.encode(semanticKey);
		final prior = semanticKeysByDigest.get(digest);
		if (prior != null && prior != semanticKey)
			throw new CBodyEmissionError('Array specialization digest collision `$digest` between `$prior` and `$semanticKey`');
		semanticKeysByDigest.set(digest, semanticKey);
		var copyRequest:Null<CSymbolRequest> = null;
		var assignRequest:Null<CSymbolRequest> = null;
		var destroyRequest:Null<CSymbolRequest> = null;
		var copyParameters:Array<CSymbolRequest> = [];
		var assignParameters:Array<CSymbolRequest> = [];
		var destroyParameters:Array<CSymbolRequest> = [];
		var copyStatus:Null<CSymbolRequest> = null;
		var assignStatus:Null<CSymbolRequest> = null;
		final needsCallbacks = switch lifecycle {
			case CBAELTrivial | CBAELManagedClass(_): false;
			case _: true;
		};
		if (needsCallbacks) {
			copyRequest = lifecycleRequest(digest, "copy", 0);
			assignRequest = lifecycleRequest(digest, "assign", 1);
			destroyRequest = lifecycleRequest(digest, "destroy", 2);
			context.symbols.register(copyRequest);
			context.symbols.register(assignRequest);
			context.symbols.register(destroyRequest);
			copyParameters = registerParameters(copyRequest, ["context", "destination", "source"]);
			assignParameters = registerParameters(assignRequest, ["context", "destination", "source"]);
			destroyParameters = registerParameters(destroyRequest, ["context", "element"]);
			// C typedef names and local variables share the ordinary identifier
			// namespace. Keep this local distinct from the public `hxc_status` type;
			// GCC's strict shadow check catches the otherwise legal but confusing name.
			copyStatus = registerLocal(copyRequest, "operation_status", 0);
			assignStatus = registerLocal(assignRequest, "operation_status", 0);
		}
		final prepared = new CPreparedBodyArray(semanticKey, digest, element, ownerModule, HaxeSourceSpan.fromPosition(position, sourcePath), position,
			lifecycle, copyRequest, assignRequest, destroyRequest, copyParameters, assignParameters, destroyParameters, copyStatus, assignStatus);
		if (prepared.managedByCollector)
			registerManagedNames(prepared);
		bySemanticKey.set(semanticKey, prepared);
		return prepared;
	}

	/**
		Choose the smallest complete lifetime rule for one element type.

		Plain scalars need no callbacks. Bytes and closed records containing Bytes
		use typed copy/assign/destroy callbacks so each logical copy retains its
		managed fields and each destruction releases them once. Another Array, a
		class reference, owned String storage, or another unsupported managed field still
		fails here: admitting those shapes requires their own proven lifetime and
		cycle policy, not a byte copy or a generic boxed fallback.
	**/
	static function elementLifecycle(element:CBodyValueType):Null<CBodyArrayElementLifecycle>
		return switch element.kind {
			case CBVKPrimitive(_): element.irType != IRTVoid && element.irType != IRTString ? CBAELTrivial : null;
			// The bytes behind this view belong to compiler-emitted static literals.
			// Copying the three scalar fields cannot outlive or double-free that storage.
			case CBVKStaticString(_): CBAELTrivial;
			case CBVKManagedString(_): CBAELString;
			case CBVKAggregate(aggregate): aggregateLifecycle(aggregate);
			case CBVKArray(_): CBAELArray;
			case CBVKBytes(_): CBAELBytes;
			case CBVKClass(value, _): CBAELManagedClass(value);
			case CBVKOptional(value): value.managedLifetime ? CBAELOptional(value) : CBAELTrivial;
			case CBVKEnum(value): enumLifecycle(value);
			case _: null;
		};

	function registerManagedNames(value:CPreparedBodyArray):Void {
		value.descriptorRequest = new CSymbolRequest(CSKTypeDescriptor, ["compiler", "gc", "array", value.digest, "descriptor"],
			CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 0, ["array", value.digest.substr(0, 8), "descriptor"]);
		value.traceRequest = new CSymbolRequest(CSKMethod, ["compiler", "gc", "array", value.digest, "trace"], CNSOrdinary("translation-unit"), CSVInternal,
			null, [], [], 1, ["array", value.digest.substr(0, 8), "trace"]);
		value.finalizerRequest = new CSymbolRequest(CSKMethod, ["compiler", "gc", "array", value.digest, "finalize"], CNSOrdinary("translation-unit"),
			CSVInternal, null, [], [], 2, ["array", value.digest.substr(0, 8), "finalize"]);
		context.symbols.register(value.descriptorRequest);
		context.symbols.register(value.traceRequest);
		context.symbols.register(value.finalizerRequest);
	}

	/**
		A tagged enum is still a plain C value when every payload is plain.

		For example, `FlowArgument` may carry an integer or a static String view.
		Copying its tag and union then has no retain/release work. Recursive enums
		contain pointers with scoped ownership, so they remain outside this rule.
	**/
	static function enumLifecycle(value:reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance):Null<CBodyArrayElementLifecycle> {
		if (value.representation == CBERNativeEnum)
			return CBAELTrivial;
		if (value.cases.length == 0)
			return null;
		return value.managedLifetime ? CBAELEnum(value) : CBAELTrivial;
	}

	static function aggregateLifecycle(aggregate:CPreparedBodyAggregate):Null<CBodyArrayElementLifecycle> {
		if (aggregate.fields.length == 0)
			return null;
		var managed = false;
		for (field in aggregate.fields) {
			final nested = elementLifecycle(field.type);
			if (nested == null)
				return null;
			if (nested != CBAELTrivial)
				managed = true;
		}
		return managed ? CBAELAggregate(aggregate) : CBAELTrivial;
	}

	function lifecycleRequest(digest:String, operation:String, ordinal:Int):CSymbolRequest
		return new CSymbolRequest(CSKMethod, ["compiler", "haxe-array", digest, "element", operation], CNSOrdinary("translation-unit"), CSVInternal, null, [],
			[], ordinal, ["array", digest.substr(0, 8), "element", operation]);

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

	function registerLocal(owner:CSymbolRequest, role:String, ordinal:Int):CSymbolRequest {
		final request = new CSymbolRequest(CSKLocal, owner.qualifiedName.concat([role]), CNSOrdinary(owner.stableKey()), CSVInternal, null, [], [], ordinal,
			[role]);
		context.symbols.register(request);
		return request;
	}

	static function elementSemanticKey(element:CBodyValueType):String
		return switch element.kind {
			case CBVKPrimitive(mapping): 'primitive:${mapping.sourceType}:${Std.string(mapping.irType)}';
			case CBVKStaticString(identity): 'static-string:$identity';
			case CBVKAggregate(aggregate): 'aggregate:${aggregate.digest}';
			case _: element.cSpelling;
		};

	public function canonicalArrays():Array<CPreparedBodyArray> {
		final values = [for (value in bySemanticKey) value];
		values.sort((left, right) -> compareUtf8(left.digest, right.digest));
		return values;
	}

	/** Resolve every compiler-owned callback through the shared symbol registry. */
	public function finalize(symbols:CSymbolRegistry):Array<CLoweredBodyArray> {
		final result:Array<CLoweredBodyArray> = [];
		for (value in canonicalArrays()) {
			final hasCallbacks = switch value.lifecycle {
				case CBAELTrivial | CBAELManagedClass(_): false;
				case _: true;
			};
			if (!hasCallbacks) {
				result.push(new CLoweredBodyArray(value, null, null, null, [], [], [], null, null, identifierOrNull(symbols, value.descriptorRequest),
					identifierOrNull(symbols, value.traceRequest), identifierOrNull(symbols, value.finalizerRequest)));
				continue;
			}
			if (value.copyRequest == null || value.assignRequest == null || value.destroyRequest == null)
				throw new CBodyEmissionError('managed Array specialization `${value.semanticKey}` lost its callback requests');
			if (value.copyStatusRequest == null || value.assignStatusRequest == null)
				throw new CBodyEmissionError('managed Array specialization `${value.semanticKey}` lost its callback local requests');
			result.push(new CLoweredBodyArray(value, symbols.identifierFor(value.copyRequest), symbols.identifierFor(value.assignRequest),
				symbols.identifierFor(value.destroyRequest), value.copyParameterRequests.map(symbols.identifierFor),
				value.assignParameterRequests.map(symbols.identifierFor), value.destroyParameterRequests.map(symbols.identifierFor),
				symbols.identifierFor(value.copyStatusRequest), symbols.identifierFor(value.assignStatusRequest),
				identifierOrNull(symbols, value.descriptorRequest), identifierOrNull(symbols, value.traceRequest),
				identifierOrNull(symbols, value.finalizerRequest)));
		}
		return result;
	}

	static function identifierOrNull(symbols:CSymbolRegistry, request:Null<CSymbolRequest>):Null<CIdentifier>
		return request == null ? null : symbols.identifierFor(request);

	static function isCoreArray(value:ClassType):Bool
		return value.pack.length == 0 && value.name == "Array";

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
		throw new CBodyEmissionError("Array rejection callback returned unexpectedly");
	}
}

/** Exact declaration identity used to keep Array methods out of class dispatch. */
class CBodyArrayRecognition {
	/** Recognize Array through typedef/lazy wrappers without unwrapping abstracts. */
	public static function isCoreArrayType(type:Type):Bool
		return switch TypeTools.follow(type) {
			case TInst(reference, _): isCoreArray(reference);
			case _: false;
		};

	public static function isCoreArray(reference:Ref<ClassType>):Bool {
		final value = reference.get();
		return value.pack.length == 0 && value.name == "Array";
	}
}
#else
class CBodyArray {
	private function new() {}
}
#end

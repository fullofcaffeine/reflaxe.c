package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;

/**
	Plans the closed set of source types and operations that may use Dynamic.

	Typed values keep their existing representations. This registry is consulted
	only when a source expression crosses an explicit Dynamic boundary. It assigns
	stable adapter identities before function replay freezes, then gives lowering
	the same records when it emits dedicated HxcIR Dynamic instructions.
**/
class CBodyDynamic {
	private function new() {}
}

/** One source type that can enter the private Dynamic carrier. */
class CPreparedBodyDynamicType {
	public final id:String;
	public final mapping:Null<CBodyValueType>;
	public final category:HxcIRDynamicCategory;
	public final storage:HxcIRDynamicStorage;
	public final source:HxcSourceSpan;
	public final typeValueKey:Null<String>;
	public var descriptorRequest:Null<CSymbolRequest> = null;
	public var wrapperTagRequest:Null<CSymbolRequest> = null;
	public var wrapperFieldRequest:Null<CSymbolRequest> = null;
	public var wrapperDescriptorRequest:Null<CSymbolRequest> = null;
	public var wrapperTraceRequest:Null<CSymbolRequest> = null;
	public var wrapperFinalizerRequest:Null<CSymbolRequest> = null;
	public var typeTokenRequest:Null<CSymbolRequest> = null;

	public function new(id:String, mapping:Null<CBodyValueType>, category:HxcIRDynamicCategory, storage:HxcIRDynamicStorage, source:HxcSourceSpan,
			?typeValueKey:String) {
		this.id = id;
		this.mapping = mapping;
		this.category = category;
		this.storage = storage;
		this.source = source;
		this.typeValueKey = typeValueKey;
	}

	public function irType(typeId:Int):HxcIRDynamicType
		return {
			id: id,
			typeId: typeId,
			sourceType: mapping == null ? null : mapping.irType,
			category: category,
			storage: storage,
			source: source
		};
}

/** Final C names for one exact Dynamic source-type adapter. */
class CLoweredBodyDynamicType {
	public final prepared:CPreparedBodyDynamicType;
	public final descriptorName:CIdentifier;
	public final wrapperTag:Null<CIdentifier>;
	public final wrapperFieldName:Null<CIdentifier>;
	public final wrapperDescriptorName:Null<CIdentifier>;
	public final wrapperTraceName:Null<CIdentifier>;
	public final wrapperFinalizerName:Null<CIdentifier>;
	public final typeTokenName:Null<CIdentifier>;

	public function new(prepared:CPreparedBodyDynamicType, descriptorName:CIdentifier, wrapperTag:Null<CIdentifier>, wrapperFieldName:Null<CIdentifier>,
			wrapperDescriptorName:Null<CIdentifier>, wrapperTraceName:Null<CIdentifier>, wrapperFinalizerName:Null<CIdentifier>,
			typeTokenName:Null<CIdentifier>) {
		this.prepared = prepared;
		this.descriptorName = descriptorName;
		this.wrapperTag = wrapperTag;
		this.wrapperFieldName = wrapperFieldName;
		this.wrapperDescriptorName = wrapperDescriptorName;
		this.wrapperTraceName = wrapperTraceName;
		this.wrapperFinalizerName = wrapperFinalizerName;
		this.typeTokenName = typeTokenName;
	}
}

/** Finalized adapter data consumed by structural C emission. */
class CLoweredBodyDynamicPlan {
	public final ir:HxcIRDynamicPlan;
	public final types:Array<CLoweredBodyDynamicType>;
	public final members:Array<CPreparedBodyDynamicMember>;
	public final callShapes:Array<CPreparedBodyDynamicCallShape>;
	public final operations:Array<CPreparedBodyDynamicOperation>;

	public function new(ir:HxcIRDynamicPlan, types:Array<CLoweredBodyDynamicType>, members:Array<CPreparedBodyDynamicMember>,
			callShapes:Array<CPreparedBodyDynamicCallShape>, operations:Array<CPreparedBodyDynamicOperation>) {
		this.ir = ir;
		this.types = types.copy();
		this.members = members.copy();
		this.callShapes = callShapes.copy();
		this.operations = operations.copy();
	}
}

/** One statically named field or method owned by an exact object adapter. */
class CPreparedBodyDynamicMember {
	public final id:String;
	public final owner:CPreparedBodyDynamicType;
	public final sourceName:String;
	public final kind:HxcIRDynamicMemberKind;
	public final targetFunctionId:Null<String>;
	public final source:HxcSourceSpan;

	public function new(id:String, owner:CPreparedBodyDynamicType, sourceName:String, kind:HxcIRDynamicMemberKind, targetFunctionId:Null<String>,
			source:HxcSourceSpan) {
		this.id = id;
		this.owner = owner;
		this.sourceName = sourceName;
		this.kind = kind;
		this.targetFunctionId = targetFunctionId;
		this.source = source;
	}
}

/** One exact typed call signature used by a function or method adapter. */
class CPreparedBodyDynamicCallShape {
	public final id:String;
	public final parameterTypes:Array<CPreparedBodyDynamicType>;
	public final resultType:Null<CPreparedBodyDynamicType>;
	public final source:HxcSourceSpan;

	public function new(id:String, parameterTypes:Array<CPreparedBodyDynamicType>, resultType:Null<CPreparedBodyDynamicType>, source:HxcSourceSpan) {
		this.id = id;
		this.parameterTypes = parameterTypes.copy();
		this.resultType = resultType;
		this.source = source;
	}

	public function irShape():HxcIRDynamicCallShape
		return {
			id: id,
			parameterTypeIds: parameterTypes.map(value -> value.id),
			resultTypeId: resultType == null ? null : resultType.id,
			source: source
		};
}

/** One instruction-visible operation with its exact closed-world contract. */
class CPreparedBodyDynamicOperation {
	public final id:String;
	public final kind:HxcIRDynamicOperationKind;
	public final source:HxcSourceSpan;

	public function new(id:String, kind:HxcIRDynamicOperationKind, source:HxcSourceSpan) {
		this.id = id;
		this.kind = kind;
		this.source = source;
	}

	public function irOperation():HxcIRDynamicOperation
		return {id: id, kind: kind, source: source};
}

/** Counts Dynamic contributions at the function-replay freeze boundary. */
typedef CBodyDynamicContributionInventory = {
	final types:Int;
	final members:Int;
	final callShapes:Int;
	final operations:Int;
	final operationKeys:String;
}

/**
	Request-local owner of every reachable Dynamic adapter and operation.

	All maps use semantic keys and all published arrays use UTF-8 order. A caller
	may ask for an existing record during authoritative lowering, but a new record
	after the discovery prepass is observable through the contribution inventory
	and stops replay publication.
**/
class CBodyDynamicRegistry {
	final context:CompilationContext;
	final typesByKey:Map<String, CPreparedBodyDynamicType> = [];
	final typesById:Map<String, CPreparedBodyDynamicType> = [];
	final membersByKey:Map<String, CPreparedBodyDynamicMember> = [];
	final shapesByKey:Map<String, CPreparedBodyDynamicCallShape> = [];
	final operationsByKey:Map<String, CPreparedBodyDynamicOperation> = [];

	public function new(context:CompilationContext) {
		this.context = context;
	}

	/** Return the canonical null adapter used by null boxes and Void call results. */
	public function requireNull(source:HxcSourceSpan):CPreparedBodyDynamicType {
		final existing = typesByKey.get("null");
		if (existing != null)
			return existing;
		final prepared = new CPreparedBodyDynamicType("dynamic.type.null", null, IRDCNull, IRDSInlineNull, source);
		registerTypeNames(prepared, "null");
		typesByKey.set("null", prepared);
		typesById.set(prepared.id, prepared);
		return prepared;
	}

	/**
		Return one admitted exact adapter, or null for a still-unsupported family.

		Only signed Haxe Int and binary64 Float use inline numeric storage. String,
		Array, fieldless enum, function, and direct-record values use exact generated
		wrappers. Exact collector-managed classes use their own allocation base.
	**/
	public function requireType(mapping:CBodyValueType, source:HxcSourceSpan):Null<CPreparedBodyDynamicType> {
		final shape = dynamicShape(mapping);
		if (shape == null)
			return null;
		final key = exactTypeKey(mapping.irType);
		final existing = typesByKey.get(key);
		if (existing != null)
			return existing;
		final digest = Sha256.encode(key);
		final prepared = new CPreparedBodyDynamicType('dynamic.type.${categoryKey(shape.category)}.$digest', mapping, shape.category, shape.storage, source);
		registerTypeNames(prepared, digest);
		typesByKey.set(key, prepared);
		typesById.set(prepared.id, prepared);
		return prepared;
	}

	/** Return the exact immutable token adapter for one source type expression. */
	public function requireTypeValue(typeValueKey:String, source:HxcSourceSpan):CPreparedBodyDynamicType {
		final key = 'type-value:$typeValueKey';
		final existing = typesByKey.get(key);
		if (existing != null)
			return existing;
		final digest = Sha256.encode(key);
		final prepared = new CPreparedBodyDynamicType('dynamic.type.type-value.$digest', null, IRDCTypeValue, IRDSStaticToken, source, typeValueKey);
		registerTypeNames(prepared, digest);
		typesByKey.set(key, prepared);
		typesById.set(prepared.id, prepared);
		return prepared;
	}

	/** Find one previously planned adapter from its stable identity. */
	public function typeById(id:String):Null<CPreparedBodyDynamicType>
		return typesById.get(id);

	/** Register or reuse an exact box operation. */
	public function requireBox(type:CPreparedBodyDynamicType, source:HxcSourceSpan):CPreparedBodyDynamicOperation
		return requireOperation('box:${type.id}', IRDOKBox(type.id), source);

	/** Register or reuse an exact checked unbox operation. */
	public function requireUnbox(type:CPreparedBodyDynamicType, source:HxcSourceSpan):CPreparedBodyDynamicOperation
		return requireOperation('unbox:${type.id}', IRDOKUnbox(type.id), source);

	/** Register or reuse exact Dynamic equality for one statically proven pair. */
	public function requireEqual(left:CPreparedBodyDynamicType, right:CPreparedBodyDynamicType, source:HxcSourceSpan):CPreparedBodyDynamicOperation
		return requireOperation('equal:${left.id}:${right.id}', IRDOKEqual(left.id, right.id), source);

	/** Register one exact field member and its numeric-token operation. */
	public function requireField(owner:CPreparedBodyDynamicType, name:String, value:CPreparedBodyDynamicType, mutable:Bool,
			source:HxcSourceSpan):CPreparedBodyDynamicMember {
		final key = 'field:${owner.id}:$name';
		final existing = membersByKey.get(key);
		if (existing != null)
			return existing;
		final member = new CPreparedBodyDynamicMember(stableId("dynamic.member.field", key), owner, name, IRDMField(value.id, mutable), null, source);
		membersByKey.set(key, member);
		return member;
	}

	/** Register one exact method member without creating a bound function value. */
	public function requireMethod(owner:CPreparedBodyDynamicType, name:String, shape:CPreparedBodyDynamicCallShape, targetFunctionId:String,
			source:HxcSourceSpan):CPreparedBodyDynamicMember {
		final key = 'method:${owner.id}:$name:${shape.id}:$targetFunctionId';
		final existing = membersByKey.get(key);
		if (existing != null)
			return existing;
		final member = new CPreparedBodyDynamicMember(stableId("dynamic.member.method", key), owner, name, IRDMMethod([shape.id]), targetFunctionId, source);
		membersByKey.set(key, member);
		return member;
	}

	/** Register the operation that reads one statically named field. */
	public function requireGet(member:CPreparedBodyDynamicMember, source:HxcSourceSpan):CPreparedBodyDynamicOperation
		return requireOperation('get:${member.id}', IRDOKGet(member.id), source);

	/** Register the operation that writes one statically named mutable field. */
	public function requireSet(member:CPreparedBodyDynamicMember, source:HxcSourceSpan):CPreparedBodyDynamicOperation
		return requireOperation('set:${member.id}', IRDOKSet(member.id), source);

	/** Register one exact function-pointer call shape. */
	public function requireCallShape(parameters:Array<CBodyValueType>, result:Null<CBodyValueType>, source:HxcSourceSpan):Null<CPreparedBodyDynamicCallShape> {
		final parameterAdapters:Array<CPreparedBodyDynamicType> = [];
		for (parameter in parameters) {
			final adapter = requireType(parameter, source);
			if (adapter == null)
				return null;
			parameterAdapters.push(adapter);
		}
		if (result == null || result.irType == IRTVoid)
			requireNull(source);
		final resultAdapter = result == null || result.irType == IRTVoid ? null : requireType(result, source);
		if (result != null && result.irType != IRTVoid && resultAdapter == null)
			return null;
		final key = '(${parameterAdapters.map(value -> value.id).join(",")})->${resultAdapter == null ? "void" : resultAdapter.id}';
		final existing = shapesByKey.get(key);
		if (existing != null)
			return existing;
		final shape = new CPreparedBodyDynamicCallShape(stableId("dynamic.shape", key), parameterAdapters, resultAdapter, source);
		shapesByKey.set(key, shape);
		return shape;
	}

	/** Register a checked call through one exact wrapped function type. */
	public function requireCall(callable:CPreparedBodyDynamicType, shape:CPreparedBodyDynamicCallShape, source:HxcSourceSpan):CPreparedBodyDynamicOperation
		return requireOperation('call:${callable.id}:${shape.id}', IRDOKCall(callable.id, shape.id), source);

	/** Register a checked direct member call that preserves its receiver. */
	public function requireInvoke(member:CPreparedBodyDynamicMember, shape:CPreparedBodyDynamicCallShape, source:HxcSourceSpan):CPreparedBodyDynamicOperation
		return requireOperation('invoke:${member.id}:${shape.id}', IRDOKInvoke(member.id, shape.id), source);

	/** Publish a deterministic program plan for replay keys and validation. */
	public function plan():HxcIRDynamicPlan {
		final types = canonicalTypes();
		final members = canonicalMembers();
		final tokensByMember:Map<String, Int> = [];
		final nextTokenByOwner:Map<String, Int> = [];
		for (member in members) {
			final next = nextTokenByOwner.exists(member.owner.id) ? nextTokenByOwner.get(member.owner.id) : 0;
			tokensByMember.set(member.id, next);
			nextTokenByOwner.set(member.owner.id, next + 1);
		}
		return {
			types: [for (index => value in types) value.irType(index)],
			members: members.map(value -> {
				id: value.id,
				ownerTypeId: value.owner.id,
				token: tokensByMember.get(value.id),
				sourceName: value.sourceName,
				kind: value.kind,
				source: value.source
			}),
			callShapes: canonicalShapes().map(value -> value.irShape()),
			operations: canonicalOperations().map(value -> value.irOperation())
		};
	}

	/** Snapshot only counts; any late semantic addition invalidates replay. */
	public function contributionInventory():CBodyDynamicContributionInventory
		return {
			types: count(typesByKey),
			members: count(membersByKey),
			callShapes: count(shapesByKey),
			operations: count(operationsByKey),
			operationKeys: sortedKeys(operationsByKey).join("\n")
		};

	public function canonicalTypes():Array<CPreparedBodyDynamicType>
		return sorted([for (value in typesByKey) value], value -> value.id);

	public function canonicalMembers():Array<CPreparedBodyDynamicMember>
		return sorted([for (value in membersByKey) value], value -> value.id);

	public function canonicalShapes():Array<CPreparedBodyDynamicCallShape>
		return sorted([for (value in shapesByKey) value], value -> value.id);

	public function canonicalOperations():Array<CPreparedBodyDynamicOperation>
		return sorted([for (value in operationsByKey) value], value -> value.id);

	/** Resolve all compiler-owned C names after whole-program discovery freezes. */
	public function finalize(symbols:CSymbolRegistry):CLoweredBodyDynamicPlan {
		final lowered = canonicalTypes().map(prepared -> new CLoweredBodyDynamicType(prepared, identifier(symbols, prepared.descriptorRequest),
			identifierOrNull(symbols, prepared.wrapperTagRequest), identifierOrNull(symbols, prepared.wrapperFieldRequest),
			identifierOrNull(symbols, prepared.wrapperDescriptorRequest), identifierOrNull(symbols, prepared.wrapperTraceRequest),
			identifierOrNull(symbols, prepared.wrapperFinalizerRequest), identifierOrNull(symbols, prepared.typeTokenRequest)));
		return new CLoweredBodyDynamicPlan(plan(), lowered, canonicalMembers(), canonicalShapes(), canonicalOperations());
	}

	function registerTypeNames(prepared:CPreparedBodyDynamicType, digest:String):Void {
		final short = digest.substr(0, 8);
		final root = ["compiler", "dynamic", prepared.id];
		prepared.descriptorRequest = new CSymbolRequest(CSKTypeDescriptor, root.concat(["type-descriptor"]), CNSOrdinary("translation-unit"), CSVInternal,
			null, [], [], 0, ["dynamic", categoryKey(prepared.category), short, "type"]);
		context.symbols.register(prepared.descriptorRequest);
		switch prepared.storage {
			case IRDSManagedWrapper:
				prepared.wrapperTagRequest = new CSymbolRequest(CSKType, root.concat(["wrapper"]), CNSTag("translation-unit"), CSVInternal, null, [], [], 1,
					["dynamic", categoryKey(prepared.category), short, "wrapper"]);
				context.symbols.register(prepared.wrapperTagRequest);
				prepared.wrapperFieldRequest = new CSymbolRequest(CSKField, root.concat(["wrapper", "value"]),
					CNSMember(prepared.wrapperTagRequest.stableKey()), CSVInternal, null, [], [], 0, ["value"]);
				context.symbols.register(prepared.wrapperFieldRequest);
				prepared.wrapperDescriptorRequest = new CSymbolRequest(CSKTypeDescriptor, root.concat(["wrapper", "descriptor"]),
					CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 2, ["dynamic", categoryKey(prepared.category), short, "descriptor"]);
				prepared.wrapperTraceRequest = new CSymbolRequest(CSKMethod, root.concat(["wrapper", "trace"]), CNSOrdinary("translation-unit"), CSVInternal,
					null, [], [], 3, ["dynamic", categoryKey(prepared.category), short, "trace"]);
				prepared.wrapperFinalizerRequest = new CSymbolRequest(CSKMethod, root.concat(["wrapper", "finalize"]), CNSOrdinary("translation-unit"),
					CSVInternal, null, [], [], 4, ["dynamic", categoryKey(prepared.category), short, "finalize"]);
				context.symbols.register(prepared.wrapperDescriptorRequest);
				context.symbols.register(prepared.wrapperTraceRequest);
				context.symbols.register(prepared.wrapperFinalizerRequest);
			case IRDSStaticToken:
				prepared.typeTokenRequest = new CSymbolRequest(CSKRuntimePrivate, root.concat(["type-token"]), CNSOrdinary("translation-unit"), CSVInternal,
					null, [], [], 1, ["dynamic", "type", short, "token"]);
				context.symbols.register(prepared.typeTokenRequest);
			case IRDSInlineNull | IRDSInlineBool | IRDSInlineInt32 | IRDSInlineFloat64 | IRDSManagedReference:
		}
	}

	static function identifier(symbols:CSymbolRegistry, request:Null<CSymbolRequest>):CIdentifier {
		if (request == null)
			throw new CBodyEmissionError("Dynamic adapter lost a required symbol request");
		return symbols.identifierFor(request);
	}

	static function identifierOrNull(symbols:CSymbolRegistry, request:Null<CSymbolRequest>):Null<CIdentifier>
		return request == null ? null : symbols.identifierFor(request);

	function requireOperation(key:String, kind:HxcIRDynamicOperationKind, source:HxcSourceSpan):CPreparedBodyDynamicOperation {
		final existing = operationsByKey.get(key);
		if (existing != null)
			return existing;
		final operation = new CPreparedBodyDynamicOperation(stableId("dynamic.operation", key), kind, source);
		operationsByKey.set(key, operation);
		return operation;
	}

	static function dynamicShape(mapping:CBodyValueType):Null<{category:HxcIRDynamicCategory, storage:HxcIRDynamicStorage}> {
		return switch mapping.kind {
			case CBVKPrimitive(value):
				switch value.irType {
					case IRTBool: {category: IRDCBool, storage: IRDSInlineBool};
					case IRTInt(32, true): {category: IRDCInt, storage: IRDSInlineInt32};
					case IRTFloat(64): {category: IRDCFloat, storage: IRDSInlineFloat64};
					case _: null;
				}
			case CBVKStaticString(_) | CBVKManagedString(_): {category: IRDCString, storage: IRDSManagedWrapper};
			case CBVKArray(_): {category: IRDCArray, storage: IRDSManagedWrapper};
			case CBVKAggregate(_): {category: IRDCObject, storage: IRDSManagedWrapper};
			case CBVKClass(_, _): {category: IRDCObject, storage: IRDSManagedReference};
			case CBVKEnum(value):
				var fieldless = true;
				for (tagCase in value.cases)
					if (tagCase.payload.length != 0)
						fieldless = false;
				fieldless ? {category: IRDCEnum, storage: IRDSManagedWrapper} : null;
			case CBVKFunction(_, _): {category: IRDCFunction, storage: IRDSManagedWrapper};
			case _:
				null;
		};
	}

	static function categoryKey(category:HxcIRDynamicCategory):String
		return switch category {
			case IRDCNull: "null";
			case IRDCBool: "bool";
			case IRDCInt: "int";
			case IRDCFloat: "float";
			case IRDCString: "string";
			case IRDCArray: "array";
			case IRDCObject: "object";
			case IRDCEnum: "enum";
			case IRDCFunction: "function";
			case IRDCTypeValue: "type-value";
		};

	static function stableId(prefix:String, key:String):String
		return '$prefix.${Sha256.encode(key)}';

	static function exactTypeKey(type:HxcIRTypeRef):String
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): 'int:$width:${signed ? "signed" : "unsigned"}';
			case IRTAbiInteger(kind): 'abi-int:$kind';
			case IRTFloat(width): 'float:$width';
			case IRTString: "string-utf8";
			case IRTManagedString: "managed-string-utf8";
			case IRTCString: "cstring";
			case IRTCallScopedCString: "cstring-call-borrow";
			case IRTMutableCStringBuffer: "mutable-cstring-buffer-call-borrow";
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(pointee, nullable): 'pointer:${nullable ? "nullable" : "non-null"}<${exactTypeKey(pointee)}>';
			case IRTNullable(inner, representation): 'nullable:$representation<${exactTypeKey(inner)}>';
			case IRTFunction(parameters, result): 'function(${parameters.map(exactTypeKey).join(",")})->${exactTypeKey(result)}';
			case IRTFixedArray(element, length, witnessId): 'fixed-array:$length:$witnessId<${exactTypeKey(element)}>';
			case IRTSpan(element, mutable): 'span:${mutable ? "mutable" : "const"}<${exactTypeKey(element)}>';
			case IRTDynamic: "dynamic";
		};

	static function sorted<T>(values:Array<T>, key:T->String):Array<T> {
		values.sort((left, right) -> reflaxe.c.CUtf8Order.compare(key(left), key(right)));
		return values;
	}

	static function count<T>(values:Map<String, T>):Int {
		var result = 0;
		for (_ in values)
			result++;
		return result;
	}

	/** Return deterministic semantic keys for drift diagnostics. */
	static function sortedKeys<T>(values:Map<String, T>):Array<String> {
		final result = [for (key in values.keys()) key];
		result.sort(reflaxe.c.CUtf8Order.compare);
		return result;
	}
}
#else

/** Generated-program stub; Dynamic planning runs only in compiler builds. */
class CBodyDynamic {
	private function new() {}
}
#end

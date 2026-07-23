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
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CGenericSpecialization.CGenericTypeArgument;
import reflaxe.c.lowering.CGenericSpecialization.CGenericTypeCanonicalizer;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.semantics.CPrimitiveTypeMapper;
import reflaxe.c.semantics.CPrimitiveTypes;

/** Module anchor for bounded Haxe-enum representation selection. */
class CBodyEnum {
	private function new() {}
}

enum CBodyEnumRepresentation {
	CBERNativeEnum;
	CBERTaggedUnion;
}

typedef CBodyValueResolver = (type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
	node:String) -> CBodyValueType;

/** One specialized constructor payload before recursive storage is selected. */
class CPreparedBodyEnumPayload {
	public final name:String;
	public final valueType:CBodyValueType;
	public final source:HxcSourceSpan;
	public final request:CSymbolRequest;
	public var indirect:Bool = false;

	public function new(name:String, valueType:CBodyValueType, source:HxcSourceSpan, request:CSymbolRequest) {
		this.name = name;
		this.valueType = valueType;
		this.source = source;
		this.request = request;
	}

	public function storageType():HxcIRTypeRef
		return indirect ? IRTPointer(valueType.irType, false) : valueType.irType;
}

/** One constructor in source discriminant order. */
class CPreparedBodyEnumCase {
	public final name:String;
	public final tagValue:Int;
	public final source:HxcSourceSpan;
	public final discriminantRequest:CSymbolRequest;
	public final payloadStructRequest:Null<CSymbolRequest>;
	public final unionMemberRequest:Null<CSymbolRequest>;
	public final payload:Array<CPreparedBodyEnumPayload> = [];

	public function new(name:String, tagValue:Int, source:HxcSourceSpan, discriminantRequest:CSymbolRequest, payloadStructRequest:Null<CSymbolRequest>,
			unionMemberRequest:Null<CSymbolRequest>) {
		this.name = name;
		this.tagValue = tagValue;
		this.source = source;
		this.discriminantRequest = discriminantRequest;
		this.payloadStructRequest = payloadStructRequest;
		this.unionMemberRequest = unionMemberRequest;
	}
}

/** One concrete generic enum instance, specialized before C layout. */
class CPreparedBodyEnumInstance {
	public final shapeKey:String;
	public final digest:String;
	public final declarationId:String;
	public final instanceId:String;
	public final haxePath:String;
	public final displayName:String;
	public final ownerModule:String;
	public final source:HxcSourceSpan;
	public final representation:CBodyEnumRepresentation;
	public final typeParameterNames:Array<String>;
	public final typeArguments:Array<CGenericTypeArgument>;
	public final arguments:Array<CBodyValueType>;
	public final reasons:Array<HxcSourceSpan> = [];
	public final valueTagRequest:CSymbolRequest;
	public final discriminantTagRequest:CSymbolRequest;
	public final payloadUnionRequest:Null<CSymbolRequest>;
	public final tagMemberRequest:Null<CSymbolRequest>;
	public final payloadMemberRequest:Null<CSymbolRequest>;
	public final cases:Array<CPreparedBodyEnumCase> = [];
	public var recursive:Bool = false;
	public var scopedLifetime:Bool = false;

	/**
		True when an active constructor owns any payload with managed lifetime.

		A tagged C union stores every constructor in overlapping memory. Retain and
		destroy helpers must therefore inspect the tag and visit only the active
		constructor. This explicit fact covers Arrays, Bytes, records, optionals,
		and nested enums without letting ordinary struct-copy code read an inactive
		union member.
	**/
	public var managedPayload:Bool = false;

	/**
		True when an active payload can reach an exact tracing-GC pointer.

		This is separate from `managedPayload`: garbage-collected class references
		copy as ordinary pointers and need no retain/destroy operation, but stack
		roots and heap-container trace callbacks must still visit them.
	**/
	public var collectorPayload:Bool = false;

	/**
		True when copying this value requires typed ownership work.

		Managed payloads retain or deep-copy their owned state. Recursive payloads
		use owned heap nodes and must be deep-copied. Keeping the combined fact
		separate from `managedPayload` preserves the distinction between ordinary
		active payload ownership and recursive enum storage.
	**/
	public var managedLifetime:Bool = false;

	public var retainRequest:Null<CSymbolRequest> = null;
	public var destroyRequest:Null<CSymbolRequest> = null;
	public var retainParameterRequest:Null<CSymbolRequest> = null;
	public var destroyParameterRequest:Null<CSymbolRequest> = null;
	public var retainStatusRequest:Null<CSymbolRequest> = null;

	public function new(shapeKey:String, digest:String, haxePath:String, displayName:String, ownerModule:String, source:HxcSourceSpan,
			representation:CBodyEnumRepresentation, typeParameterNames:Array<String>, typeArguments:Array<CGenericTypeArgument>,
			arguments:Array<CBodyValueType>, valueTagRequest:CSymbolRequest, discriminantTagRequest:CSymbolRequest, payloadUnionRequest:Null<CSymbolRequest>,
			tagMemberRequest:Null<CSymbolRequest>, payloadMemberRequest:Null<CSymbolRequest>) {
		this.shapeKey = shapeKey;
		this.digest = digest;
		this.declarationId = 'type.enum.$digest';
		this.instanceId = 'instance.enum.$digest';
		this.haxePath = haxePath;
		this.displayName = displayName;
		this.ownerModule = ownerModule;
		this.source = source;
		this.representation = representation;
		this.typeParameterNames = typeParameterNames.copy();
		this.typeArguments = typeArguments.copy();
		this.arguments = arguments.copy();
		this.valueTagRequest = valueTagRequest;
		this.discriminantTagRequest = discriminantTagRequest;
		this.payloadUnionRequest = payloadUnionRequest;
		this.tagMemberRequest = tagMemberRequest;
		this.payloadMemberRequest = payloadMemberRequest;
	}

	public function addReason(reason:HxcSourceSpan):Void {
		for (existing in reasons) {
			if (existing.display() == reason.display())
				return;
		}
		reasons.push(reason);
		reasons.sort((left, right) -> CGenericTypeCanonicalizer.compareUtf8(left.display(), right.display()));
	}

	public function declaration():HxcIRTypeDeclaration {
		return {
			id: declarationId,
			displayName: displayName,
			kind: IRTKTaggedUnion(cases.map(tagCase -> {
				name: tagCase.name,
				tagValue: tagCase.tagValue,
				payload: tagCase.payload.map(payload -> {
					name: payload.name,
					type: payload.storageType(),
					source: payload.source
				}),
				source: tagCase.source
			})),
			source: source
		};
	}

	public function instance():HxcIRTypeInstance {
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: arguments.map(argument -> argument.irType),
			representation: representation == CBERNativeEnum ? IRRDirect : IRRTagged,
			source: source
		};
	}

	public function tagCase(name:String):Null<CPreparedBodyEnumCase> {
		for (tagCase in cases) {
			if (tagCase.name == name) {
				return tagCase;
			}
		}
		return null;
	}

	public function retainImplementationId():Null<String>
		return managedLifetime ? 'enum-lifecycle:$instanceId:retain' : null;

	public function destroyImplementationId():Null<String>
		return managedLifetime ? 'enum-lifecycle:$instanceId:destroy' : null;
}

class CLoweredBodyEnumPayload {
	public final prepared:CPreparedBodyEnumPayload;
	public final cName:CIdentifier;

	public function new(prepared:CPreparedBodyEnumPayload, cName:CIdentifier) {
		this.prepared = prepared;
		this.cName = cName;
	}
}

class CLoweredBodyEnumCase {
	public final prepared:CPreparedBodyEnumCase;
	public final discriminant:CIdentifier;
	public final payloadStructTag:Null<CIdentifier>;
	public final unionMember:Null<CIdentifier>;
	public final payload:Array<CLoweredBodyEnumPayload>;

	public function new(prepared:CPreparedBodyEnumCase, discriminant:CIdentifier, payloadStructTag:Null<CIdentifier>, unionMember:Null<CIdentifier>,
			payload:Array<CLoweredBodyEnumPayload>) {
		this.prepared = prepared;
		this.discriminant = discriminant;
		this.payloadStructTag = payloadStructTag;
		this.unionMember = unionMember;
		this.payload = payload.copy();
	}
}

/** Finalized private C names for one concrete Haxe enum instance. */
class CLoweredBodyEnum {
	public final prepared:CPreparedBodyEnumInstance;
	public final valueTag:CIdentifier;
	public final discriminantTag:CIdentifier;
	public final payloadUnionTag:Null<CIdentifier>;
	public final tagMember:Null<CIdentifier>;
	public final payloadMember:Null<CIdentifier>;
	public final cases:Array<CLoweredBodyEnumCase>;
	public final retainName:Null<CIdentifier>;
	public final destroyName:Null<CIdentifier>;
	public final retainParameterName:Null<CIdentifier>;
	public final destroyParameterName:Null<CIdentifier>;
	public final retainStatusName:Null<CIdentifier>;

	public function new(prepared:CPreparedBodyEnumInstance, valueTag:CIdentifier, discriminantTag:CIdentifier, payloadUnionTag:Null<CIdentifier>,
			tagMember:Null<CIdentifier>, payloadMember:Null<CIdentifier>, cases:Array<CLoweredBodyEnumCase>, retainName:Null<CIdentifier>,
			destroyName:Null<CIdentifier>, retainParameterName:Null<CIdentifier>, destroyParameterName:Null<CIdentifier>, retainStatusName:Null<CIdentifier>) {
		this.prepared = prepared;
		this.valueTag = valueTag;
		this.discriminantTag = discriminantTag;
		this.payloadUnionTag = payloadUnionTag;
		this.tagMember = tagMember;
		this.payloadMember = payloadMember;
		this.cases = cases.copy();
		this.retainName = retainName;
		this.destroyName = destroyName;
		this.retainParameterName = retainParameterName;
		this.destroyParameterName = destroyParameterName;
		this.retainStatusName = retainStatusName;
	}

	public function tagCase(name:String):Null<CLoweredBodyEnumCase> {
		for (tagCase in cases) {
			if (tagCase.prepared.name == name) {
				return tagCase;
			}
		}
		return null;
	}
}

/** Request-local concrete-enum discovery, specialization, and recursion planning. */
class CBodyEnumRegistry {
	public static inline final MAX_GENERIC_ENUM_SPECIALIZATIONS = CGenericSpecializationContract.MAX_TYPE_SPECIALIZATIONS;

	final context:CompilationContext;
	final resolveValue:CBodyValueResolver;
	final byShape:Map<String, CPreparedBodyEnumInstance> = [];
	final shapeKeysByDigest:Map<String, String> = [];
	var preparationDepth = 0;
	var genericSpecializationCount = 0;

	public function new(context:CompilationContext, resolveValue:CBodyValueResolver) {
		this.context = context;
		this.resolveValue = resolveValue;
	}

	public function valueType(reference:Ref<EnumType>, parameters:Array<Type>, position:Position, ownerModule:String, ownerSourcePath:String,
			fail:(Position, String) -> Void, node:String):CBodyValueType {
		return CBodyValueType.enumeration(require(reference, parameters, position, ownerModule, ownerSourcePath, fail, node));
	}

	public function require(reference:Ref<EnumType>, parameters:Array<Type>, position:Position, ownerModule:String, ownerSourcePath:String,
			fail:(Position, String) -> Void, node:String):CPreparedBodyEnumInstance {
		final definition = reference.get();
		if (definition.isExtern) {
			return rejected(fail, position, '$node:extern-enum:${enumPath(definition)}');
		}
		if (definition.params.length != parameters.length) {
			return rejected(fail, position, '$node:enum-argument-count:${enumPath(definition)}:${parameters.length}-for-${definition.params.length}');
		}
		final canonicalizer = new CGenericTypeCanonicalizer(context.profile);
		final typeArguments = parameters.map(parameter -> canonicalizer.normalize(parameter, position, fail, '$node.type-argument'));
		final argumentKeys = typeArguments.map(argument -> argument.key);
		final shapeKey = CGenericSpecializationContract.enumInstanceKey(enumPath(definition), argumentKeys);
		final reason = HaxeSourceSpan.fromPosition(position, ownerSourcePath);
		final existing = byShape.get(shapeKey);
		if (existing != null) {
			if (existing.recursive && existing.collectorPayload)
				return rejected(fail, position, '$node:recursive-enum-with-collector-payload:${existing.haxePath}');
			existing.addReason(reason);
			return existing;
		}
		if (definition.params.length > 0 && genericSpecializationCount >= MAX_GENERIC_ENUM_SPECIALIZATIONS) {
			return rejected(fail, position,
				'$node:generic-enum-specialization-budget:$MAX_GENERIC_ENUM_SPECIALIZATIONS:expanding-or-excessive:${enumPath(definition)}');
		}
		final path = enumPath(definition);
		preparationDepth++;
		final digest = Sha256.encode(shapeKey);
		final priorShape = shapeKeysByDigest.get(digest);
		if (priorShape != null && priorShape != shapeKey) {
			throw new CBodyEmissionError('generic enum specialization digest collision `$digest` between `$priorShape` and `$shapeKey`');
		}
		shapeKeysByDigest.set(digest, shapeKey);
		final sourcePath = definition.module == ownerModule ? ownerSourcePath : moduleSourcePath(definition.module);
		final source = HaxeSourceSpan.fromPosition(definition.pos, sourcePath);
		final arguments = parameters.map(parameter -> {
			final value = resolveValue(parameter, position, definition.module, sourcePath, fail, '$node.type-argument');
			if (value.spanElement() != null)
				return rejected(fail, position, '$node.type-argument:borrowed-span-escape');
			if (value.irType == IRTVoid) {
				return rejected(fail, position, '$node:unsupported-enum-type-argument:${value.cSpelling}');
			}
			value;
		});
		final hasPayload = enumHasPayload(definition, parameters, position, fail, node);
		final representation = hasPayload ? CBERTaggedUnion : CBERNativeEnum;
		final symbolRoot = ["compiler", "haxe-enum", digest];
		final readableRoot = path.split(".");
		final valueTagRequest = new CSymbolRequest(CSKType, symbolRoot.concat(["value"]), CNSTag("translation-unit"), CSVInternal, null, [], argumentKeys,
			null, readableRoot);
		final discriminantTagRequest = representation == CBERNativeEnum ? valueTagRequest : new CSymbolRequest(CSKType, symbolRoot.concat(["tag"]),
			CNSTag("translation-unit"), CSVInternal, null, [], argumentKeys, null, readableRoot.concat(["tag"]));
		final payloadUnionRequest = representation == CBERNativeEnum ? null : new CSymbolRequest(CSKType, symbolRoot.concat(["payload"]),
			CNSTag("translation-unit"), CSVInternal, null, [], argumentKeys, null, readableRoot.concat(["payload"]));
		final tagMemberRequest = representation == CBERNativeEnum ? null : new CSymbolRequest(CSKField, symbolRoot.concat(["tag"]),
			CNSMember('type.enum.$digest'), CSVInternal, null, [], argumentKeys, 0, ["tag"]);
		final payloadMemberRequest = representation == CBERNativeEnum ? null : new CSymbolRequest(CSKField, symbolRoot.concat(["payload"]),
			CNSMember('type.enum.$digest'), CSVInternal, null, [], argumentKeys, 1, ["payload"]);
		context.symbols.register(valueTagRequest);
		context.symbols.register(discriminantTagRequest);
		if (payloadUnionRequest != null)
			context.symbols.register(payloadUnionRequest);
		if (tagMemberRequest != null)
			context.symbols.register(tagMemberRequest);
		if (payloadMemberRequest != null)
			context.symbols.register(payloadMemberRequest);
		final prepared = new CPreparedBodyEnumInstance(shapeKey, digest, path, displayName(definition, argumentKeys), definition.module, source,
			representation, definition.params.map(parameter -> parameter.name), typeArguments, arguments, valueTagRequest, discriminantTagRequest,
			payloadUnionRequest, tagMemberRequest, payloadMemberRequest);
		prepared.addReason(reason);
		byShape.set(shapeKey, prepared);
		if (definition.params.length > 0)
			genericSpecializationCount++;

		for (caseName in definition.names) {
			final field = definition.constructs.get(caseName);
			if (field == null) {
				return rejected(fail, position, '$node:enum-constructor-missing:$path.$caseName');
			}
			if (field.params.length != 0) {
				return rejected(fail, field.pos, '$node:constructor-generic-parameters:$path.$caseName');
			}
			final constructor = constructorSignature(definition, parameters, field, fail, node);
			final caseSource = HaxeSourceSpan.fromPosition(field.pos, sourcePath);
			final discriminantRequest = new CSymbolRequest(CSKField, symbolRoot.concat(["case", caseName]), CNSOrdinary("translation-unit"), CSVInternal,
				null, [], argumentKeys, field.index, readableRoot.concat([caseName]));
			final payloadStructRequest = constructor.arguments.length == 0 ? null : new CSymbolRequest(CSKType,
				symbolRoot.concat(["case", caseName, "payload"]), CNSTag("translation-unit"), CSVInternal, null, [], argumentKeys, field.index,
				readableRoot.concat([caseName, "payload"]));
			final unionMemberRequest = constructor.arguments.length == 0 ? null : new CSymbolRequest(CSKField, symbolRoot.concat(["case", caseName]),
				CNSMember('union.enum.$digest'), CSVInternal, null, [], argumentKeys, field.index, [caseName]);
			context.symbols.register(discriminantRequest);
			if (payloadStructRequest != null)
				context.symbols.register(payloadStructRequest);
			if (unionMemberRequest != null)
				context.symbols.register(unionMemberRequest);
			final tagCase = new CPreparedBodyEnumCase(caseName, field.index, caseSource, discriminantRequest, payloadStructRequest, unionMemberRequest);
			for (index in 0...constructor.arguments.length) {
				final argument = constructor.arguments[index];
				final valueType = resolveValue(argument.type, field.pos, definition.module, sourcePath, fail, '$node.$caseName.${argument.name}');
				// String is a Haxe reference type, but the admitted literal-backed view has
				// program-long storage and copies safely by value. Other direct references
				// still need an explicit ownership or recursive-layout policy.
				if (valueType.staticStringIdentity() == null && valueType.arrayValue() == null && valueType.classValue() == null)
					rejectDirectReference(argument.type, field.pos, fail, '$node.$caseName.${argument.name}');
				if (valueType.spanElement() != null)
					return rejected(fail, field.pos, '$node.$caseName.${argument.name}:borrowed-span-payload-escape');
				if (valueType.irType == IRTVoid) {
					return rejected(fail, field.pos, '$node:unsupported-payload:$path.$caseName.${argument.name}:${valueType.cSpelling}');
				}
				final request = new CSymbolRequest(CSKField, symbolRoot.concat(["case", caseName, "payload", argument.name]),
					CNSMember('case.enum.$digest.$caseName'), CSVInternal, null, [], argumentKeys, index, [argument.name]);
				context.symbols.register(request);
				tagCase.payload.push(new CPreparedBodyEnumPayload(argument.name, valueType, caseSource, request));
			}
			prepared.cases.push(tagCase);
		}
		preparationDepth--;
		if (preparationDepth == 0) {
			recomputeRecursion();
			if (prepared.recursive && prepared.collectorPayload)
				return rejected(fail, position, '$node:recursive-enum-with-collector-payload:$path');
		}
		return prepared;
	}

	function rejectDirectReference(type:Type, position:Position, fail:(Position, String) -> Void, node:String):Void {
		switch CPrimitiveTypeMapper.map(type, context.profile) {
			case CTReference(identity, nullable):
				rejected(fail, position, '$node:reference-$identity-${nullable ? "nullable" : "non-null"}');
			case CTNativePointer(identity, nullable):
				rejected(fail, position, '$node:native-pointer-$identity-${nullable ? "nullable" : "non-null"}');
			case CTPrimitive(_) | CTUnsupported(_):
		}
	}

	public function canonicalEnums():Array<CPreparedBodyEnumInstance> {
		recomputeRecursion();
		final values = [for (value in byShape) value];
		values.sort((left, right) -> compareUtf8(left.digest, right.digest));
		final result:Array<CPreparedBodyEnumInstance> = [];
		final emitted:Map<String, Bool> = [];
		for (value in values)
			appendDependencies(value, result, emitted);
		return result;
	}

	public function finalize(symbols:CSymbolRegistry):Array<CLoweredBodyEnum> {
		return canonicalEnums().map(prepared -> {
			final cases = prepared.cases.map(tagCase -> new CLoweredBodyEnumCase(tagCase, symbols.identifierFor(tagCase.discriminantRequest),
				identifierOrNull(symbols, tagCase.payloadStructRequest), identifierOrNull(symbols, tagCase.unionMemberRequest),
				tagCase.payload.map(payload -> new CLoweredBodyEnumPayload(payload, symbols.identifierFor(payload.request)))));
			return new CLoweredBodyEnum(prepared, symbols.identifierFor(prepared.valueTagRequest), symbols.identifierFor(prepared.discriminantTagRequest),
				identifierOrNull(symbols, prepared.payloadUnionRequest), identifierOrNull(symbols, prepared.tagMemberRequest),
				identifierOrNull(symbols, prepared.payloadMemberRequest), cases, identifierOrNull(symbols, prepared.retainRequest),
				identifierOrNull(symbols, prepared.destroyRequest), identifierOrNull(symbols, prepared.retainParameterRequest),
				identifierOrNull(symbols, prepared.destroyParameterRequest), identifierOrNull(symbols, prepared.retainStatusRequest));
		});
	}

	function recomputeRecursion():Void {
		for (value in byShape) {
			value.recursive = hasRecursiveEdge(value);
			for (tagCase in value.cases) {
				for (payload in tagCase.payload) {
					payload.indirect = switch payload.valueType.enumValue() {
						case null: false;
						case dependency: dependency == value || reaches(dependency, value, []);
					};
				}
			}
		}
		for (value in byShape)
			value.scopedLifetime = requiresScopedLifetime(value, []);
		for (value in byShape) {
			value.managedPayload = requiresManagedPayloadLifecycle(value, []);
			value.collectorPayload = requiresCollectorPayload(value, []);
			value.managedLifetime = value.managedPayload || value.recursive;
			if (value.managedLifetime)
				registerManagedLifecycle(value);
		}
	}

	/** Whether a direct finite payload shape contains a tracing-GC reference. */
	function requiresCollectorPayload(value:CPreparedBodyEnumInstance, visited:Map<String, Bool>):Bool {
		if (visited.exists(value.instanceId))
			return false;
		visited.set(value.instanceId, true);
		for (tagCase in value.cases)
			for (payload in tagCase.payload) {
				if (payload.indirect)
					continue;
				if (valueTypeContainsCollectorReference(payload.valueType, visited))
					return true;
			}
		return false;
	}

	function valueTypeContainsCollectorReference(type:CBodyValueType, visited:Map<String, Bool>):Bool {
		if (type.classValue() != null)
			return true;
		final array = type.arrayValue();
		if (array != null && array.managedByCollector)
			return true;
		final aggregate = type.aggregateValue();
		if (aggregate != null) {
			for (field in aggregate.fields)
				if (valueTypeContainsCollectorReference(field.type, visited))
					return true;
			return false;
		}
		final optional = type.optionalValue();
		if (optional != null)
			return valueTypeContainsCollectorReference(optional.payload, visited);
		final nested = type.enumValue();
		return nested != null && requiresCollectorPayload(nested, visited);
	}

	function requiresManagedPayloadLifecycle(value:CPreparedBodyEnumInstance, visited:Map<String, Bool>):Bool {
		if (visited.exists(value.instanceId))
			return false;
		visited.set(value.instanceId, true);
		for (tagCase in value.cases) {
			for (payload in tagCase.payload) {
				final array = payload.valueType.arrayValue();
				if (array != null && !array.managedByCollector || payload.valueType.bytesValue() != null)
					return true;
				final aggregate = payload.valueType.aggregateValue();
				if (aggregate != null && aggregate.managedLifetime)
					return true;
				final optional = payload.valueType.optionalValue();
				if (optional != null && optional.managedLifetime)
					return true;
				final nested = payload.valueType.enumValue();
				if (nested != null && !payload.indirect && requiresManagedPayloadLifecycle(nested, visited))
					return true;
			}
		}
		return false;
	}

	function registerManagedLifecycle(value:CPreparedBodyEnumInstance):Void {
		if (value.retainRequest != null)
			return;
		final root = ["compiler", "haxe-enum", value.digest, "lifecycle"];
		value.retainRequest = new CSymbolRequest(CSKMethod, root.concat(["retain"]), CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 0,
			["enum", value.digest.substr(0, 8), "retain"]);
		value.destroyRequest = new CSymbolRequest(CSKMethod, root.concat(["destroy"]), CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 1,
			["enum", value.digest.substr(0, 8), "destroy"]);
		context.symbols.register(value.retainRequest);
		context.symbols.register(value.destroyRequest);
		value.retainParameterRequest = lifecycleLocal(value.retainRequest, "value", 0);
		value.destroyParameterRequest = lifecycleLocal(value.destroyRequest, "value", 0);
		// Avoid shadowing the public `hxc_status` typedef in C's ordinary
		// identifier namespace. GCC diagnoses that ambiguity in the strict lane.
		value.retainStatusRequest = lifecycleLocal(value.retainRequest, "operation_status", 1);
	}

	function lifecycleLocal(owner:CSymbolRequest, role:String, ordinal:Int):CSymbolRequest {
		final request = new CSymbolRequest(CSKLocal, owner.qualifiedName.concat([role]), CNSOrdinary(owner.stableKey()), CSVInternal, null, [], [], ordinal,
			[role]);
		context.symbols.register(request);
		return request;
	}

	function requiresScopedLifetime(value:CPreparedBodyEnumInstance, visited:Map<String, Bool>):Bool {
		if (visited.exists(value.instanceId))
			return false;
		visited.set(value.instanceId, true);
		for (tagCase in value.cases) {
			for (payload in tagCase.payload) {
				if (payload.indirect)
					return true;
				final dependency = payload.valueType.enumValue();
				if (dependency != null && requiresScopedLifetime(dependency, visited))
					return true;
			}
		}
		return false;
	}

	function hasRecursiveEdge(value:CPreparedBodyEnumInstance):Bool {
		for (tagCase in value.cases) {
			for (payload in tagCase.payload) {
				final dependency = payload.valueType.enumValue();
				if (dependency != null && (dependency == value || reaches(dependency, value, [])))
					return true;
			}
		}
		return false;
	}

	function reaches(current:CPreparedBodyEnumInstance, target:CPreparedBodyEnumInstance, visited:Map<String, Bool>):Bool {
		if (visited.exists(current.instanceId))
			return false;
		visited.set(current.instanceId, true);
		for (tagCase in current.cases) {
			for (payload in tagCase.payload) {
				final dependency = payload.valueType.enumValue();
				if (dependency != null && (dependency == target || reaches(dependency, target, visited)))
					return true;
			}
		}
		return false;
	}

	function appendDependencies(value:CPreparedBodyEnumInstance, result:Array<CPreparedBodyEnumInstance>, emitted:Map<String, Bool>):Void {
		if (emitted.exists(value.instanceId))
			return;
		for (tagCase in value.cases) {
			for (payload in tagCase.payload) {
				final dependency = payload.valueType.enumValue();
				if (dependency != null && !payload.indirect)
					appendDependencies(dependency, result, emitted);
			}
		}
		emitted.set(value.instanceId, true);
		result.push(value);
	}

	static function enumHasPayload(definition:EnumType, parameters:Array<Type>, position:Position, fail:(Position, String) -> Void, node:String):Bool {
		for (name in definition.names) {
			final field = definition.constructs.get(name);
			if (field == null)
				return rejected(fail, position, '$node:enum-constructor-missing:${enumPath(definition)}.$name');
			if (constructorSignature(definition, parameters, field, fail, node).arguments.length > 0)
				return true;
		}
		return false;
	}

	static function constructorSignature(definition:EnumType, parameters:Array<Type>, field:EnumField, fail:(Position, String) -> Void,
			node:String):{arguments:Array<{name:String, type:Type}>, result:Type} {
		final applied = TypeTools.applyTypeParameters(field.type, definition.params, parameters);
		return switch applied {
			case TFun(arguments, result): {
					arguments: arguments.map(argument -> {name: argument.name, type: argument.t}),
					result: result
				};
			case TEnum(_, _): {arguments: [], result: applied};
			case _: rejected(fail, field.pos, '$node:invalid-enum-constructor-type:${enumPath(definition)}.${field.name}');
		};
	}

	static function displayName(definition:EnumType, argumentKeys:Array<String>):String
		return argumentKeys.length == 0 ? enumPath(definition) : enumPath(definition) + "<" + argumentKeys.join(",") + ">";

	static function enumPath(definition:EnumType):String
		return definition.pack.concat([definition.name]).join(".");

	static function moduleSourcePath(modulePath:String):String
		return modulePath.split(".").join("/") + ".hx";

	static function identifierOrNull(symbols:CSymbolRegistry, request:Null<CSymbolRequest>):Null<CIdentifier>
		return request == null ? null : symbols.identifierFor(request);

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
		throw new CBodyEmissionError("enum rejection callback returned unexpectedly");
	}
}
#else
class CBodyEnum {
	public function new() {}
}
#end

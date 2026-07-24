package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CProfile;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.semantics.CPrimitiveTypeMapper;
import reflaxe.c.semantics.CPrimitiveTypes;

/** Stable report vocabulary for one closed generic type argument. */
enum abstract CGenericArgumentRepresentation(String) to String {
	var DirectPrimitive = "direct-primitive";
	var DirectEnum = "direct-enum";

	/** Runtime-sized `Array<T>` whose closed element identity remains structural. */
	var ManagedArray = "managed-array";

	/** Closed anonymous record carried by value after aggregate planning. */
	var DirectRecord = "direct-record";

	/** Immutable Haxe String identity; storage policy remains a later decision. */
	var ImmutableString = "immutable-string";

	/** Haxe `Null<T>` identity; the later representation still depends on `T`. */
	var NullableValue = "nullable-value";
}

/** A validated closed type argument plus its raw compiler type for substitution. */
class CGenericTypeArgument {
	public final type:Type;
	public final key:String;
	public final displayName:String;
	public final representation:CGenericArgumentRepresentation;

	public function new(type:Type, key:String, displayName:String, representation:CGenericArgumentRepresentation) {
		this.type = type;
		this.key = key;
		this.displayName = displayName;
		this.representation = representation;
	}
}

/** One source-rooted reason a concrete function specialization is reachable. */
class CGenericSpecializationReason {
	public final callerInstanceId:String;
	public final source:HxcSourceSpan;
	public final position:Position;

	public function new(callerInstanceId:String, source:HxcSourceSpan, position:Position) {
		this.callerInstanceId = callerInstanceId;
		this.source = source;
		this.position = position;
	}

	public function key():String
		return callerInstanceId + "\x00" + source.display();
}

/** One concrete reachable generic static function before body lowering. */
class CGenericFunctionSpecialization {
	public final baseFunctionId:String;
	public final instanceId:String;
	public final key:String;
	public final digest:String;
	public final displayName:String;
	public final typeParameters:Array<TypeParameter>;
	public final arguments:Array<CGenericTypeArgument>;
	public final reasons:Array<CGenericSpecializationReason> = [];

	public function new(baseFunctionId:String, fieldName:String, typeParameters:Array<TypeParameter>, arguments:Array<CGenericTypeArgument>,
			initialReason:CGenericSpecializationReason) {
		if (typeParameters.length == 0 || typeParameters.length != arguments.length) {
			throw new CBodyEmissionError('generic specialization `$baseFunctionId` has an invalid closed argument set');
		}
		this.baseFunctionId = baseFunctionId;
		this.typeParameters = typeParameters.copy();
		this.arguments = arguments.copy();
		this.key = CGenericTypeCanonicalizer.functionKey(baseFunctionId, arguments);
		this.digest = Sha256.encode(key);
		this.instanceId = 'function.specialization.$digest';
		this.displayName = fieldName + "<" + arguments.map(argument -> argument.displayName).join(", ") + ">";
		addReason(initialReason);
	}

	public function apply(type:Type):Type
		return TypeTools.applyTypeParameters(type, typeParameters, arguments.map(argument -> argument.type));

	public function addReason(reason:CGenericSpecializationReason):Void {
		for (existing in reasons) {
			if (existing.key() == reason.key()) {
				return;
			}
		}
		reasons.push(reason);
		reasons.sort((left, right) -> CGenericTypeCanonicalizer.compareUtf8(left.key(), right.key()));
	}

	public function primaryPosition():Position
		return reasons[0].position;
}

/** A direct static call resolved to either its base function or one closed instance. */
class CResolvedGenericCall {
	public final baseFunctionId:String;
	public final arguments:Array<CGenericTypeArgument>;

	public function new(baseFunctionId:String, arguments:Array<CGenericTypeArgument>) {
		this.baseFunctionId = baseFunctionId;
		this.arguments = arguments.copy();
	}

	public function specializationKey():Null<String>
		return arguments.length == 0 ? null : CGenericTypeCanonicalizer.functionKey(baseFunctionId, arguments);

	public function instanceId():String {
		final specialization = specializationKey();
		return specialization == null ? baseFunctionId : 'function.specialization.${Sha256.encode(specialization)}';
	}
}

/**
	Canonical closed-type identity admitted by the E3.T03 specialization slice.

	The full length-prefixed key is authoritative. A digest is only a compact C/IR
	suffix and every registry checks that one digest never names two full keys.
 */
class CGenericTypeCanonicalizer {
	public static inline final MAX_TYPE_ARGUMENT_DEPTH = 64;
	public static inline final MAX_ALIAS_EXPANSIONS = 64;

	final profile:CProfile;

	public function new(profile:CProfile) {
		this.profile = profile;
	}

	public function normalize(type:Type, position:Position, fail:(Position, String) -> Void, node:String):CGenericTypeArgument
		return normalizeType(type, 0, [], position, fail, node, false);

	function normalizeType(type:Type, depth:Int, activeAnonymous:Array<Ref<AnonType>>, position:Position, fail:(Position, String) -> Void, node:String,
			allowTransparentRecordFieldAbstract:Bool):CGenericTypeArgument {
		if (depth > MAX_TYPE_ARGUMENT_DEPTH)
			return rejected(fail, position, '$node:type-argument-depth-budget:$MAX_TYPE_ARGUMENT_DEPTH');
		final resolved = unwrapAliases(type, MAX_ALIAS_EXPANSIONS, position, fail, node);
		return switch resolved {
			case TDynamic(_):
				rejected(fail, position, '$node:dynamic-type-argument');
			case TInst(reference, parameters):
				final definition = reference.get();
				switch definition.kind {
					case KTypeParameter(_): rejected(fail, position, '$node:open-type-argument:${qualifiedClassName(definition)}');
					case _:
						final path = qualifiedClassName(definition);
						if (path == "String") {
							if (parameters.length != 0)
								rejected(fail, position, '$node:String-type-argument-count:${parameters.length}');
							new CGenericTypeArgument(TInst(reference, []), CGenericSpecializationContract.stringArgumentKey(), "String", ImmutableString);
						} else if (path != "Array") {
							rejected(fail, position, '$node:class-type-argument:$path');
						} else {
							if (parameters.length != 1)
								rejected(fail, position, '$node:Array-type-argument-count:${parameters.length}');
							final element = normalizeType(parameters[0], depth + 1, activeAnonymous, position, fail, '$node:Array-element',
								allowTransparentRecordFieldAbstract);
							final normalizedType:Type = TInst(reference, [element.type]);
							new CGenericTypeArgument(normalizedType, CGenericSpecializationContract.arrayArgumentKey(element.key),
								'Array<${element.displayName}>', ManagedArray);
						}
				}
			case TEnum(reference, parameters):
				final definition = reference.get();
				final path = definition.pack.concat([definition.name]).join(".");
				final arguments = parameters.map(parameter -> normalizeType(parameter, depth + 1, activeAnonymous, position, fail, node,
					allowTransparentRecordFieldAbstract));
				final normalizedType:Type = TEnum(reference, arguments.map(argument -> argument.type));
				new CGenericTypeArgument(normalizedType, CGenericSpecializationContract.enumArgumentKey(path, arguments.map(argument -> argument.key)),
					arguments.length == 0 ? path : path
						+ "<"
						+ arguments.map(argument -> argument.displayName).join(", ")
						+ ">", DirectEnum);
			case TAbstract(reference, parameters) if (reference.get().pack.length == 0 && reference.get().name == "Null"):
				if (parameters.length != 1) rejected(fail, position, '$node:Null-type-argument-count:${parameters.length}'); else {
					final value = normalizeType(parameters[0], depth + 1, activeAnonymous, position, fail, '$node:Null-value',
						allowTransparentRecordFieldAbstract);
					new CGenericTypeArgument(TAbstract(reference, [value.type]), CGenericSpecializationContract.nullableArgumentKey(value.key),
						'Null<${value.displayName}>', NullableValue);
				}
			case TAbstract(reference, parameters) if (allowTransparentRecordFieldAbstract && !reference.get().meta.has(":coreType")):
				final directPrimitive = admittedPrimitiveArgument(resolved);
				directPrimitive == null ? normalizeTransparentAbstract(reference, parameters, depth, activeAnonymous, position, fail, node) : directPrimitive;
			case TAnonymous(reference):
				normalizeAnonymous(reference, depth, activeAnonymous, position, fail, node);
			case TFun(_, _):
				rejected(fail, position, '$node:function-type-argument-requires-E3.T08-closure-analysis');
			case _:
				primitiveArgument(resolved, position, fail, node);
		};
	}

	/**
		Preserve one ordinary Haxe abstract while proving its runtime carrier.

		An abstract such as `LogicalPath(String)` is a named source-level view, not
		a wrapper object. Haxe has already typed its constructors, conversions, and
		operators before this pass, so specialization may store it as `String` only
		after the existing String rule accepts that carrier. The nominal path and
		closed arguments remain in the key, which prevents two different abstracts
		from sharing a specialization merely because both happen to use the same C
		bytes. `@:coreType` abstracts and unsupported carriers never enter this
		path and continue to fail closed.
	**/
	function normalizeTransparentAbstract(reference:Ref<AbstractType>, parameters:Array<Type>, depth:Int, activeAnonymous:Array<Ref<AnonType>>,
			position:Position, fail:(Position, String) -> Void, node:String):CGenericTypeArgument {
		final definition = reference.get();
		final path = definition.pack.concat([definition.name]).join(".");
		final arguments = parameters.map(parameter -> normalizeType(parameter, depth + 1, activeAnonymous, position, fail, '$node.abstract-argument', true));
		final carrierType = TypeTools.applyTypeParameters(definition.type, definition.params, arguments.map(argument -> argument.type));
		final carrier = normalizeType(carrierType, depth + 1, activeAnonymous, position, fail, '$node.abstract-carrier', true);
		final normalizedType:Type = TAbstract(reference, arguments.map(argument -> argument.type));
		final displayName = arguments.length == 0 ? path : path + "<" + arguments.map(argument -> argument.displayName).join(", ") + ">";
		return new CGenericTypeArgument(normalizedType,
			CGenericSpecializationContract.transparentAbstractArgumentKey(path, arguments.map(argument -> argument.key), carrier.key), displayName,
			carrier.representation);
	}

	/**
		Build a stable structural identity for a closed anonymous record.

		This records only type facts. It does not claim the record or an enclosing
		Array can already be emitted; body lowering remains the representation gate.
	**/
	function normalizeAnonymous(reference:Ref<AnonType>, depth:Int, active:Array<Ref<AnonType>>, position:Position, fail:(Position, String) -> Void,
			node:String):CGenericTypeArgument {
		for (candidate in active)
			if (candidate == reference)
				return rejected(fail, position, '$node:recursive-anonymous-type-argument');
		final anonymous = reference.get();
		switch anonymous.status {
			case AClosed | AConst:
			case AOpened:
				return rejected(fail, position, '$node:open-anonymous-type-argument');
			case AExtend(_):
				return rejected(fail, position, '$node:extended-anonymous-type-argument');
			case AClassStatics(_) | AEnumStatics(_) | AAbstractStatics(_):
				return rejected(fail, position, '$node:static-container-type-argument');
		}
		if (anonymous.fields.length == 0)
			return rejected(fail, position, '$node:empty-anonymous-type-argument');
		final nextActive = active.copy();
		nextActive.push(reference);
		final fields = anonymous.fields.copy();
		fields.sort((left, right) -> compareUtf8(left.name, right.name));
		final fieldKeys:Array<String> = [];
		final fieldDisplays:Array<String> = [];
		for (field in fields) {
			final access = switch field.kind {
				case FVar(read, write): 'var:${accessKey(read)}:${accessKey(write)}:${field.isFinal ? "final" : "nonfinal"}';
				case FMethod(_): return rejected(fail, field.pos, '$node.field:${field.name}:method');
			};
			final fieldType = normalizeType(field.type, depth + 1, nextActive, field.pos, fail, '$node.field:${field.name}', true);
			fieldKeys.push(CGenericSpecializationContract.recordFieldKey(field.name, access, fieldType.key));
			fieldDisplays.push('${field.name}:${fieldType.displayName}');
		}
		final normalizedType:Type = TAnonymous(reference);
		return new CGenericTypeArgument(normalizedType, CGenericSpecializationContract.recordArgumentKey(fieldKeys), '{${fieldDisplays.join(", ")}}',
			DirectRecord);
	}

	function primitiveArgument(type:Type, position:Position, fail:(Position, String) -> Void, node:String):CGenericTypeArgument {
		final admitted = admittedPrimitiveArgument(type);
		if (admitted != null)
			return admitted;
		return switch CPrimitiveTypeMapper.map(type, profile) {
			case CTPrimitive(mapping):
				if (mapping.nullability != CPNonNullable) {
					return rejected(fail, position, '$node:nullable-type-argument:${mapping.cSpelling}');
				}
				rejected(fail, position, '$node:unsupported-primitive-type-argument:${mapping.cSpelling}');
			case CTReference(identity, nullable):
				rejected(fail, position, '$node:reference-type-argument:$identity:${nullable ? "nullable" : "non-null"}');
			case CTNativePointer(identity, nullable):
				rejected(fail, position, '$node:native-pointer-type-argument:$identity:${nullable ? "nullable" : "non-null"}');
			case CTUnsupported(reason):
				rejected(fail, position, '$node:$reason');
		};
	}

	/**
		Keep dedicated primitive contracts ahead of ordinary abstract erasure.

		`UInt` is implemented by Haxe as an abstract over `Int`, but haxe.c gives it
		a distinct unsigned C representation. Returning that existing mapping here
		prevents the general transparent-abstract rule from accidentally replacing
		the target's proven `u32` semantics with the carrier's signed `i32` view.
	**/
	function admittedPrimitiveArgument(type:Type):Null<CGenericTypeArgument> {
		return switch CPrimitiveTypeMapper.map(type, profile) {
			case CTPrimitive(mapping) if (mapping.nullability == CPNonNullable):
				final identity = switch mapping.sourceType {
					case CPHaxeBool: {key: "bool", display: "Bool"};
					case CPHaxeInt: {key: "i32", display: "Int"};
					case CPHaxeUInt: {key: "u32", display: "UInt"};
					case CPHaxeFloat: {key: "f64", display: "Float"};
					case CPCFloat32: {key: "c-f32", display: "c.Float32"};
					case _: null;
				};
				identity == null ? null : new CGenericTypeArgument(type, identity.key, identity.display, DirectPrimitive);
			case _: null;
		};
	}

	public static function functionKey(baseFunctionId:String, arguments:Array<CGenericTypeArgument>):String
		return CGenericSpecializationContract.functionKey(baseFunctionId, arguments.map(argument -> argument.key));

	public static function canonicalArray(values:Array<String>):String
		return CGenericSpecializationContract.canonicalArray(values);

	public static function canonicalPart(value:String):String
		return CGenericSpecializationContract.canonicalPart(value);

	public static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}

	static function unwrapAliases(type:Type, remaining:Int, position:Position, fail:(Position, String) -> Void, node:String):Type {
		return switch type {
			case TMono(reference):
				if (remaining < 1)
					return rejected(fail, position, '$node:alias-expansion-budget:$MAX_ALIAS_EXPANSIONS');
				final resolved = reference.get();
				resolved == null ? rejected(fail, position, '$node:unresolved-monomorph') : unwrapAliases(resolved, remaining - 1, position, fail, node);
			case TLazy(resolve):
				remaining < 1 ? rejected(fail, position,
					'$node:alias-expansion-budget:$MAX_ALIAS_EXPANSIONS') : unwrapAliases(resolve(), remaining - 1, position, fail, node);
			case TType(reference, parameters):
				if (remaining < 1)
					return rejected(fail, position, '$node:alias-expansion-budget:$MAX_ALIAS_EXPANSIONS');
				final definition = reference.get();
				unwrapAliases(TypeTools.applyTypeParameters(definition.type, definition.params, parameters), remaining - 1, position, fail, node);
			case _: type;
		};
	}

	static function qualifiedClassName(definition:ClassType):String
		return definition.pack.concat([definition.name]).join(".");

	static function accessKey(access:VarAccess):String {
		return switch access {
			case AccNormal: "normal";
			case AccNo: "none";
			case AccNever: "never";
			case AccResolve: "resolve";
			case AccCall: "call";
			case AccPrivateCall: "private-call";
			case AccInline: "inline";
			case AccRequire(requirement, message):
				'require:${CGenericSpecializationContract.canonicalPart(requirement)}:${CGenericSpecializationContract.canonicalPart(message == null ? "" : message)}';
			case AccCtor: "constructor";
		};
	}

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("generic type rejection callback returned unexpectedly");
	}
}

/** Infers method type parameters from the typed static-field function type. */
class CGenericCallResolver {
	public static function resolve(baseFunctionId:String, fieldType:Type, typeParameters:Array<TypeParameter>, calleeType:Type, callArgumentTypes:Array<Type>,
			caller:Null<CGenericFunctionSpecialization>, profile:CProfile, position:Position, fail:(Position, String) -> Void):CResolvedGenericCall {
		if (typeParameters.length == 0) {
			return new CResolvedGenericCall(baseFunctionId, []);
		}
		final actual = calleeType;
		final bindings:Array<Null<Type>> = [for (_ in typeParameters) null];
		final declaredFunction = unwrap(fieldType, position, fail, 'TCall(generic-specialization:$baseFunctionId:declared-function)');
		switch declaredFunction {
			case TFun(declaredArguments, _):
				if (callArgumentTypes.length > declaredArguments.length) {
					return rejected(fail, position,
						'TCall(generic-specialization:$baseFunctionId:argument-count:${callArgumentTypes.length}-for-${declaredArguments.length})');
				}
				for (index in callArgumentTypes.length...declaredArguments.length) {
					if (!declaredArguments[index].opt) {
						return rejected(fail, position, 'TCall(generic-specialization:$baseFunctionId:missing-required-argument:$index)');
					}
				}
				for (index in 0...callArgumentTypes.length) {
					match(declaredArguments[index].t, callArgumentTypes[index], typeParameters, bindings, caller, profile, position, fail,
						'TCall(generic-specialization:$baseFunctionId:argument:$index)');
				}
			case _:
				return rejected(fail, position, 'TCall(generic-specialization:$baseFunctionId:declared-type-not-function)');
		}
		if (hasUnbound(bindings)) {
			match(fieldType, actual, typeParameters, bindings, caller, profile, position, fail, 'TCall(generic-specialization:$baseFunctionId)');
		}
		final canonicalizer = new CGenericTypeCanonicalizer(profile);
		final arguments:Array<CGenericTypeArgument> = [];
		for (index in 0...typeParameters.length) {
			final bound = bindings[index];
			if (bound == null) {
				return rejected(fail, position, 'TCall(generic-specialization:$baseFunctionId:open-type-argument:${typeParameters[index].name})');
			}
			arguments.push(canonicalizer.normalize(bound, position, fail,
				'TCall(generic-specialization:$baseFunctionId:type-argument:${typeParameters[index].name})'));
		}
		return new CResolvedGenericCall(baseFunctionId, arguments);
	}

	static function hasUnbound(bindings:Array<Null<Type>>):Bool {
		for (binding in bindings) {
			if (binding == null)
				return true;
		}
		return false;
	}

	static function match(template:Type, actual:Type, parameters:Array<TypeParameter>, bindings:Array<Null<Type>>,
			caller:Null<CGenericFunctionSpecialization>, profile:CProfile, position:Position, fail:(Position, String) -> Void, node:String):Void {
		final left = unwrap(template, position, fail, node);
		final parameterIndex = typeParameterIndex(left, parameters);
		if (parameterIndex != -1) {
			bind(parameterIndex, actual, bindings, caller, profile, position, fail, node);
			return;
		}
		final right = unwrap(actual, position, fail, node);
		switch [left, right] {
			case [TFun(leftArguments, leftResult), TFun(rightArguments, rightResult)]:
				if (leftArguments.length != rightArguments.length) {
					rejected(fail, position, '$node:function-arity-mismatch');
				}
				for (index in 0...leftArguments.length) {
					match(leftArguments[index].t, rightArguments[index].t, parameters, bindings, caller, profile, position, fail, node);
				}
				match(leftResult, rightResult, parameters, bindings, caller, profile, position, fail, node);
			case [TEnum(leftReference, leftParameters), TEnum(rightReference, rightParameters)]:
				matchNominal(enumPath(leftReference.get()), enumPath(rightReference.get()), leftParameters, rightParameters, parameters, bindings, caller,
					profile, position, fail, node);
			case [
				TAbstract(leftReference, leftParameters),
				TAbstract(rightReference, rightParameters)
			]:
				matchNominal(basePath(leftReference.get()), basePath(rightReference.get()), leftParameters, rightParameters, parameters, bindings, caller,
					profile, position, fail, node);
			case [TAbstract(leftReference, [leftValue]), _] if (isNullAbstract(leftReference.get())):
				/*
					A non-null value is valid wherever Haxe accepts `Null<T>`.

					The typed front end has already proved that the call is legal. This
					resolver's narrower job is to recover closed generic arguments, so
					it must compare the nullable parameter's payload with the concrete
					argument rather than demanding identical wrappers. Body lowering
					still emits the explicit nullable injection required by HxcIR and C.
				**/
				match(leftValue, right, parameters, bindings, caller, profile, position, fail, '$node:nullable-value');
			case [TInst(leftReference, leftParameters), TInst(rightReference, rightParameters)]:
				matchNominal(basePath(leftReference.get()), basePath(rightReference.get()), leftParameters, rightParameters, parameters, bindings, caller,
					profile, position, fail, node);
			case [TDynamic(_), TDynamic(_)]:
			case _:
				final canonicalizer = new CGenericTypeCanonicalizer(profile);
				final leftValue = canonicalizer.normalize(left, position, fail, '$node:declared-type');
				final rightValue = canonicalizer.normalize(right, position, fail, '$node:typed-call');
				if (leftValue.key != rightValue.key) {
					rejected(fail, position, '$node:type-mismatch:${leftValue.key}->${rightValue.key}');
				}
		}
	}

	static function bind(index:Int, actual:Type, bindings:Array<Null<Type>>, caller:Null<CGenericFunctionSpecialization>, profile:CProfile, position:Position,
			fail:(Position, String) -> Void, node:String):Void {
		final resolved = caller == null ? actual : caller.apply(actual);
		final existing = bindings[index];
		if (existing == null) {
			bindings[index] = resolved;
			return;
		}
		final canonicalizer = new CGenericTypeCanonicalizer(profile);
		final left = canonicalizer.normalize(existing, position, fail, '$node:repeated-binding');
		final right = canonicalizer.normalize(resolved, position, fail, '$node:repeated-binding');
		if (left.key != right.key) {
			rejected(fail, position, '$node:conflicting-type-argument:${left.key}->${right.key}');
		}
	}

	static function matchNominal(leftPath:String, rightPath:String, left:Array<Type>, right:Array<Type>, parameters:Array<TypeParameter>,
			bindings:Array<Null<Type>>, caller:Null<CGenericFunctionSpecialization>, profile:CProfile, position:Position, fail:(Position, String) -> Void,
			node:String):Void {
		if (leftPath != rightPath || left.length != right.length) {
			rejected(fail, position, '$node:nominal-type-mismatch:$leftPath->$rightPath');
		}
		for (index in 0...left.length) {
			match(left[index], right[index], parameters, bindings, caller, profile, position, fail, node);
		}
	}

	static function typeParameterIndex(type:Type, parameters:Array<TypeParameter>):Int {
		return switch type {
			case TInst(reference, _):
				final definition = reference.get();
				final identity = typeParameterIdentity(definition);
				var result = -1;
				for (index in 0...parameters.length) {
					switch parameters[index].t {
						case TInst(parameterReference, _):
							if (typeParameterIdentity(parameterReference.get()) == identity) result = index;
						case _:
					}
				}
				result;
			case _: -1;
		};
	}

	static function typeParameterIdentity(definition:ClassType):String {
		return switch definition.kind {
			case KTypeParameter(_): definition.module + "\x00" + definition.pack.concat([definition.name]).join(".");
			case _: "not-a-type-parameter";
		};
	}

	static function unwrap(type:Type, position:Position, fail:(Position, String) -> Void, node:String):Type {
		return unwrapBounded(type, CGenericTypeCanonicalizer.MAX_ALIAS_EXPANSIONS, position, fail, node);
	}

	static function unwrapBounded(type:Type, remaining:Int, position:Position, fail:(Position, String) -> Void, node:String):Type {
		return switch type {
			case TMono(reference):
				if (remaining < 1)
					return rejected(fail, position, '$node:alias-expansion-budget:${CGenericTypeCanonicalizer.MAX_ALIAS_EXPANSIONS}');
				final resolved = reference.get();
				resolved == null ? rejected(fail, position, '$node:unresolved-monomorph') : unwrapBounded(resolved, remaining - 1, position, fail, node);
			case TLazy(resolve):
				remaining < 1 ? rejected(fail, position,
					'$node:alias-expansion-budget:${CGenericTypeCanonicalizer.MAX_ALIAS_EXPANSIONS}') : unwrapBounded(resolve(), remaining - 1, position,
					fail, node);
			case TType(reference, parameters):
				if (remaining < 1)
					return rejected(fail, position, '$node:alias-expansion-budget:${CGenericTypeCanonicalizer.MAX_ALIAS_EXPANSIONS}');
				final definition = reference.get();
				unwrapBounded(TypeTools.applyTypeParameters(definition.type, definition.params, parameters), remaining - 1, position, fail, node);
			case _: type;
		};
	}

	static function enumPath(definition:EnumType):String
		return definition.pack.concat([definition.name]).join(".");

	static function basePath(definition:BaseType):String
		return definition.pack.concat([definition.name]).join(".");

	static function isNullAbstract(definition:AbstractType):Bool
		return definition.pack.length == 0 && definition.name == "Null";

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("generic call rejection callback returned unexpectedly");
	}
}
#else
class CGenericSpecialization {
	public function new() {}
}
#end

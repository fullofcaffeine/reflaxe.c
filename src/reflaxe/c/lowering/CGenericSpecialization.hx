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
		return normalizeType(type, 0, position, fail, node);

	function normalizeType(type:Type, depth:Int, position:Position, fail:(Position, String) -> Void, node:String):CGenericTypeArgument {
		if (depth > MAX_TYPE_ARGUMENT_DEPTH)
			return rejected(fail, position, '$node:type-argument-depth-budget:$MAX_TYPE_ARGUMENT_DEPTH');
		final resolved = unwrapAliases(type, MAX_ALIAS_EXPANSIONS, position, fail, node);
		return switch resolved {
			case TDynamic(_):
				rejected(fail, position, '$node:dynamic-type-argument');
			case TInst(reference, _):
				final definition = reference.get();
				switch definition.kind {
					case KTypeParameter(_): rejected(fail, position, '$node:open-type-argument:${qualifiedClassName(definition)}');
					case _: rejected(fail, position, '$node:class-type-argument:${qualifiedClassName(definition)}');
				}
			case TEnum(reference, parameters):
				final definition = reference.get();
				final path = definition.pack.concat([definition.name]).join(".");
				final arguments = parameters.map(parameter -> normalizeType(parameter, depth + 1, position, fail, node));
				final normalizedType:Type = TEnum(reference, arguments.map(argument -> argument.type));
				new CGenericTypeArgument(normalizedType, CGenericSpecializationContract.enumArgumentKey(path, arguments.map(argument -> argument.key)),
					arguments.length == 0 ? path : path
						+ "<"
						+ arguments.map(argument -> argument.displayName).join(", ")
						+ ">", DirectEnum);
			case TAnonymous(_):
				rejected(fail, position, '$node:anonymous-type-argument-requires-E3.T04-representation-analysis');
			case TFun(_, _):
				rejected(fail, position, '$node:function-type-argument-requires-E3.T08-closure-analysis');
			case _:
				primitiveArgument(resolved, position, fail, node);
		};
	}

	function primitiveArgument(type:Type, position:Position, fail:(Position, String) -> Void, node:String):CGenericTypeArgument {
		return switch CPrimitiveTypeMapper.map(type, profile) {
			case CTPrimitive(mapping):
				if (mapping.nullability != CPNonNullable) {
					return rejected(fail, position, '$node:nullable-type-argument:${mapping.cSpelling}');
				}
				final identity = switch mapping.sourceType {
					case CPHaxeBool: {key: "bool", display: "Bool"};
					case CPHaxeInt: {key: "i32", display: "Int"};
					case CPHaxeUInt: {key: "u32", display: "UInt"};
					case CPHaxeFloat: {key: "f64", display: "Float"};
					case CPCFloat32: {key: "c-f32", display: "c.Float32"};
					case _: return rejected(fail, position, '$node:unsupported-primitive-type-argument:${mapping.cSpelling}');
				};
				new CGenericTypeArgument(type, identity.key, identity.display, DirectPrimitive);
			case CTReference(identity, nullable):
				rejected(fail, position, '$node:reference-type-argument:$identity:${nullable ? "nullable" : "non-null"}');
			case CTNativePointer(identity, nullable):
				rejected(fail, position, '$node:native-pointer-type-argument:$identity:${nullable ? "nullable" : "non-null"}');
			case CTUnsupported(reason):
				rejected(fail, position, '$node:$reason');
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
				if (declaredArguments.length != callArgumentTypes.length) {
					return rejected(fail, position,
						'TCall(generic-specialization:$baseFunctionId:argument-count:${callArgumentTypes.length}-for-${declaredArguments.length})');
				}
				for (index in 0...declaredArguments.length) {
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

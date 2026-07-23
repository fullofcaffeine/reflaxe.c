package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CProfile;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRFixedArrayPolicy;
import reflaxe.c.semantics.CPrimitiveTypeMapper;
import reflaxe.c.semantics.CPrimitiveTypes;

/**
	The source-level element and length identity of one `c.CArray<T, N>`.

	The actual element count comes from the validated initializer. `N` is a
	nominal witness: it prevents unrelated fixed-array families from becoming the
	same Haxe type, but it does not contain a runtime length.
**/
class CBodyFixedArrayShape {
	public final element:CPrimitiveTypeMapping;
	public final witnessId:String;

	public function new(element:CPrimitiveTypeMapping, witnessId:String) {
		this.element = element;
		this.witnessId = witnessId;
	}
}

/** Shared recognition and constant validation for local and owned fixed arrays. */
class CBodyFixedArray {
	/**
		Recognize the nominal `c.CArray` carrier without validating its payload.

		Early discovery uses this predicate only to assign ownership to the
		fixed-array lowering. The later `shape` call remains the single place that
		produces detailed element, witness, and storage diagnostics.
	**/
	public static function isCArrayType(type:Type):Bool {
		return switch type {
			case TMono(reference): final resolved = reference.get(); resolved != null && isCArrayType(resolved);
			case TLazy(resolve): isCArrayType(resolve());
			case TType(reference, parameters):
				final definition = reference.get();
				isCArrayType(TypeTools.applyTypeParameters(definition.type, definition.params, parameters));
			case TAbstract(reference, _):
				final definition = reference.get();
				definition.pack.concat([definition.name]).join(".") == "c.CArray";
			case _: false;
		};
	}

	/** Return the fixed-array shape, or null when the type is not `c.CArray`. */
	public static function shape(type:Type, profile:CProfile, position:Position, fail:(Position, String) -> Void, node:String):Null<CBodyFixedArrayShape> {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? rejected(fail, position, '$node:unresolved') : shape(resolved, profile, position, fail, node);
			case TLazy(resolve): shape(resolve(), profile, position, fail, node);
			case TType(reference, parameters):
				final definition = reference.get();
				shape(TypeTools.applyTypeParameters(definition.type, definition.params, parameters), profile, position, fail, node);
			case TAbstract(reference, parameters):
				final definition = reference.get();
				final path = definition.pack.concat([definition.name]).join(".");
				if (path != "c.CArray") {
					null;
				} else if (parameters.length != 2) {
					rejected(fail, position, '$node:type-argument-count:${parameters.length}');
				} else {
					new CBodyFixedArrayShape(element(parameters[0], profile, position, fail, node),
						namedIdentity(parameters[1], position, fail, '$node:length-witness'));
				}
			case _: null;
		};
	}

	/**
		Validate a direct `CArray.zero(constant)` initializer and return its length.

		Owned fields use this exact form so construction has one inspectable source
		of truth. General array values, calls, and copies remain unsupported.
	**/
	public static function zeroLength(expression:TypedExpr, element:HxcIRTypeRef, fail:(Position, String) -> Void, node:String):Int {
		final lengthExpression = zeroLengthExpression(expression);
		if (lengthExpression == null)
			return rejected(fail, expression.pos, '$node:requires-direct-CArray.zero');
		final length = foldLength(lengthExpression, lengthExpression, [], fail, node);
		validateStorage(element, length, lengthExpression.pos, fail, node);
		return length;
	}

	/** True only for the compiler intrinsic zero constructor, ignoring wrappers. */
	public static function isZeroInitializer(expression:TypedExpr):Bool
		return zeroLengthExpression(expression) != null;

	/** Identify the intrinsic at call-graph time without treating it as user code. */
	public static function isZeroCall(callee:TypedExpr, argumentCount:Int):Bool
		return argumentCount == 1 && isAbstractMethod(callee, "c.CArray", "zero");

	static function zeroLengthExpression(expression:TypedExpr):Null<TypedExpr> {
		return switch expression.expr {
			case TCall(callee, arguments) if (isZeroCall(callee, arguments.length)):
				arguments.length == 1 ? arguments[0] : null;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): zeroLengthExpression(inner);
			case _: null;
		};
	}

	static function foldLength(expression:TypedExpr, anchor:TypedExpr, activeInlineFields:Array<String>, fail:(Position, String) -> Void, node:String):Int {
		return switch expression.expr {
			case TConst(TInt(value)):
				if (value <= 0) rejected(fail, anchor.pos, '$node:length-must-be-positive:$value') else value;
			case TBinop(OpMult, left, right):
				final leftValue = foldLength(left, anchor, activeInlineFields, fail, node);
				final rightValue = foldLength(right, anchor, activeInlineFields, fail, node);
				if (leftValue <= 0 || rightValue <= 0)
					rejected(fail, anchor.pos, '$node:length-must-be-positive-product:$leftValue*$rightValue');
				if (leftValue > Std.int(2147483647 / rightValue))
					rejected(fail, anchor.pos, '$node:length-product-overflow:$leftValue*$rightValue');
				leftValue * rightValue;
			case TField(_, FStatic(classReference, fieldReference)):
				final field = fieldReference.get();
				final owner = classReference.get();
				final fieldId = owner.pack.concat([owner.name, field.name]).join(".");
				switch field.kind {
					case FVar(AccInline, _):
						if (activeInlineFields.indexOf(fieldId) != -1)
							rejected(fail, anchor.pos, '$node:recursive-inline-length:$fieldId');
						final value = field.expr();
						if (value == null)
							rejected(fail, anchor.pos, '$node:inline-length-without-value:$fieldId');
						foldLength(value, anchor, activeInlineFields.concat([fieldId]), fail, node);
					case _: rejected(fail, anchor.pos, '$node:length-must-be-compile-time-product:${nodeName(expression)}');
				}
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): foldLength(inner, anchor, activeInlineFields, fail, node);
			case TUnop(OpNeg, _, _): rejected(fail, anchor.pos, '$node:length-must-be-positive');
			case _: rejected(fail, anchor.pos, '$node:length-must-be-compile-time-product:${nodeName(expression)}');
		};
	}

	static function validateStorage(element:HxcIRTypeRef, length:Int, position:Position, fail:(Position, String) -> Void, node:String):Void {
		switch HxcIRFixedArrayPolicy.zeroStorage(element, length) {
			case IRFASAutomatic(_, _):
			case IRFASInvalidLength(invalidLength):
				rejected(fail, position, '$node:length-must-be-positive:$invalidLength');
			case IRFASUnsupportedElement:
				rejected(fail, position, '$node:element-requires-exact-storage-size:${typeKey(element)}');
			case IRFASSizeOverflow(elementBytes, invalidLength):
				rejected(fail, position, '$node:storage-size-overflow:$invalidLength*$elementBytes');
			case IRFASOverBudget(elementBytes, totalBytes, maximumBytes):
				rejected(fail, position,
					'$node:automatic-storage-over-budget:length=$length,element-bytes=$elementBytes,total-bytes=$totalBytes,limit-bytes=$maximumBytes');
		}
	}

	static function element(type:Type, profile:CProfile, position:Position, fail:(Position, String) -> Void, node:String):CPrimitiveTypeMapping {
		return switch CPrimitiveTypeMapper.map(type, profile) {
			case CTPrimitive(mapping):
				final admitted = mapping.nullability == CPNonNullable && switch mapping.irType {
					case IRTInt(_, _) | IRTFloat(32) | IRTFloat(64): true;
					case _: false;
				};
				admitted ? mapping : rejected(fail, position, '$node:collection-element:${mapping.cSpelling}');
			case CTReference(identity, nullable):
				rejected(fail, position, '$node:collection-element:reference-$identity-${nullable ? "nullable" : "non-null"}');
			case CTNativePointer(identity, nullable):
				rejected(fail, position, '$node:collection-element:native-pointer-$identity-${nullable ? "nullable" : "non-null"}');
			case CTUnsupported(reason): rejected(fail, position, '$node:collection-element:$reason');
		};
	}

	static function namedIdentity(type:Type, position:Position, fail:(Position, String) -> Void, node:String):String {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? rejected(fail, position, '$node:unresolved') : namedIdentity(resolved, position, fail, node);
			case TLazy(resolve): namedIdentity(resolve(), position, fail, node);
			case TType(reference, _):
				final value = reference.get();
				value.pack.concat([value.name]).join(".");
			case TAbstract(reference, _):
				final value = reference.get();
				value.pack.concat([value.name]).join(".");
			case TInst(reference, _):
				final value = reference.get();
				value.pack.concat([value.name]).join(".");
			case TEnum(reference, _):
				final value = reference.get();
				value.pack.concat([value.name]).join(".");
			case _: rejected(fail, position, '$node:requires-named-type');
		};
	}

	static function isAbstractMethod(callee:TypedExpr, ownerPath:String, methodName:String):Bool {
		return switch unwrap(callee).expr {
			case TField(_, FStatic(classReference, fieldReference)) if (fieldReference.get().name == methodName):
				switch classReference.get().kind {
					case KAbstractImpl(abstractReference):
						final owner = abstractReference.get();
						owner.pack.concat([owner.name]).join(".") == ownerPath;
					case _: false;
				};
			case _: false;
		};
	}

	static function unwrap(expression:TypedExpr):TypedExpr {
		return switch expression.expr {
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): unwrap(inner);
			case _: expression;
		};
	}

	static function nodeName(expression:TypedExpr):String {
		return switch expression.expr {
			case TConst(_): "TConst";
			case TField(_, _): "TField";
			case TBinop(operation, _, _): 'TBinop($operation)';
			case TParenthesis(_): "TParenthesis";
			case TMeta(_, _): "TMeta";
			case TCast(_, _): "TCast";
			case TCall(_, _): "TCall";
			case _: "typed-expression";
		};
	}

	static function typeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTFloat(width): 'f$width';
			case _: Std.string(type);
		};
	}

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("fixed-array rejection callback returned unexpectedly");
	}
}
#else
class CBodyFixedArray {}
#end

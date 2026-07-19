package reflaxe.c.semantics;

import reflaxe.c.CProfile;
import reflaxe.c.ir.HxcIR.HxcIRAbiIntegerKind;
import reflaxe.c.ir.HxcIR.HxcIRConversionKind;
import reflaxe.c.ir.HxcIR.HxcIRImplementation;
import reflaxe.c.ir.HxcIR.HxcIRTypeRef;
import reflaxe.c.ir.HxcIR.HxcIRNullableRepresentation;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionDecision;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionMeaning;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionUse;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveBinaryOperationDecision;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveBinaryOperationResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveBinaryOperator;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveHelperKind;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveMappingResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveNullability;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveSignedness;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveSourceType;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveStorage;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveTypeMapping;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveUnaryOperationDecision;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveUnaryOperationResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveUnaryOperator;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveWidth;

private typedef CIntegerFacts = {
	final width:Int;
	final signed:Bool;
}

/**
	The compiler-owned primitive representation and conversion authority.

	Both profiles call this same implementation. A source contract, never the
	profile preset alone, is what can select a C-native scalar or nullable pointer.
 */
class CPrimitiveSemantics {
	public static function mapping(profile:CProfile, sourceType:CPrimitiveSourceType,
			nullability:CPrimitiveNullability = CPNonNullable):CPrimitiveMappingResult {
		assertKnownProfile(profile);
		final base = baseMapping(sourceType);
		if (nullability == CPNonNullable) {
			return CPMappingAvailable(base);
		}
		if (sourceType == CPHaxeVoid) {
			return CPMappingRejected("Void has no value representation and cannot be nullable");
		}
		return CPMappingAvailable(new CPrimitiveTypeMapping(sourceType, CPNullable, base.cSpelling, base.requiredHeader, base.width, base.signedness,
			CPStorageTaggedOptional, IRTNullable(base.irType, IRNTagged)));
	}

	public static function conversion(source:CPrimitiveTypeMapping, target:CPrimitiveTypeMapping, use:CPrimitiveConversionUse):CPrimitiveConversionResult {
		if (use == CPUNullableInject) {
			if (sameValueType(source, target) && source.nullability == CPNonNullable && target.nullability == CPNullable) {
				return allowed(source, target, use, CPInjectPresent, IRCNullableInject, IRIStatic, false);
			}
			return CPConversionRejected("nullable injection requires the same non-null source and nullable target value type");
		}
		if (use == CPUNullableUnwrap) {
			if (sameValueType(source, target) && source.nullability == CPNullable && target.nullability == CPNonNullable) {
				return allowed(source, target, use, CPUnwrapPresent, IRCNullableUnwrap, IRIStatic, true);
			}
			return CPConversionRejected("nullable unwrap requires the same nullable source and non-null target value type");
		}
		if (sameMapping(source, target)) {
			return switch use {
				case CPUImplicit if (source.sourceType != CPHaxeVoid): CPConversionElided;
				case CPUWrapping | CPUChecked if (source.nullability == CPNonNullable && integerFacts(source.sourceType) != null):
					CPConversionElided;
				case _: CPConversionRejected("the requested operation is not a conversion for this identical primitive type");
			}
		}
		if (source.nullability != CPNonNullable || target.nullability != CPNonNullable) {
			return CPConversionRejected("nullable values require an explicit inject or checked unwrap conversion");
		}

		if (use == CPUImplicit) {
			return implicitConversion(source, target);
		}
		if (use == CPUStdInt) {
			if (source.sourceType == CPHaxeFloat && target.sourceType == CPHaxeInt) {
				return allowed(source, target, use, CPSaturatingTruncate, IRCNumericSaturating, IRIProgramLocal("hxc.f64.to.i32.saturating"), false);
			}
			return CPConversionRejected("Std.int is defined only from Haxe Float to Haxe Int");
		}
		if (use == CPUFloat32Narrow) {
			return source.sourceType == CPHaxeFloat
				&& target.sourceType == CPCFloat32 ? allowed(source, target, use, CPRoundToBinary32, IRCNumericRoundBinary32, IRIStatic,
					false) : CPConversionRejected("Float32 narrowing requires Haxe Float input and c.Float32 output");
		}
		if (use == CPUFloat32Widen) {
			return source.sourceType == CPCFloat32
				&& target.sourceType == CPHaxeFloat ? allowed(source, target, use, CPWidenToBinary64, IRCNumericWidenBinary64, IRIStatic,
					false) : CPConversionRejected("Float32 widening requires c.Float32 input and Haxe Float output");
		}

		final sourceInteger = integerFacts(source.sourceType);
		final targetInteger = integerFacts(target.sourceType);
		if (use == CPUChecked) {
			if (sourceInteger != null && targetInteger != null || source.sourceType == CPHaxeFloat && targetInteger != null) {
				return allowed(source, target, use, CPCheckedRange, IRCNumericChecked, IRIProgramLocal(checkedHelperId(target)), true);
			}
			return CPConversionRejected("checked primitive conversion requires an integer target and numeric source");
		}
		if (use == CPUWrapping && sourceInteger != null && targetInteger != null) {
			if (rangeIsSubset(sourceInteger, targetInteger)) {
				return allowed(source, target, use, CPExact, IRCNumericExact, IRIStatic, false);
			}
			if (targetInteger.signed) {
				return allowed(source, target, use, CPTwosComplementBits(targetInteger.width), IRCNumericWrapping,
					IRIProgramLocal('hxc.integer.to.i${targetInteger.width}.wrapping'), false);
			}
			return allowed(source, target, use, CPModuloBits(targetInteger.width), IRCNumericWrapping, IRIStatic, false);
		}
		return CPConversionRejected("conversion is not admitted by the selected typed primitive operation");
	}

	/** Decide one typed primitive unary operation before C syntax is selected. */
	public static function unaryOperation(operation:CPrimitiveUnaryOperator, operand:CPrimitiveTypeMapping,
			result:CPrimitiveTypeMapping):CPrimitiveUnaryOperationResult {
		if (!isOrdinaryNonNullable(operand) || !isOrdinaryNonNullable(result)) {
			return CPUOperationRejected("primitive unary operations require ordinary non-null Haxe scalar operands and results");
		}
		return switch operation {
			case CPUONegate:
				switch result.sourceType {
					case CPHaxeInt:
						unaryAllowed(requireSource(CPHaxeInt), result, "haxe.i32.negate", CPHI32Negate);
					case CPHaxeUInt:
						unaryAllowed(requireSource(CPHaxeUInt), result, "haxe.u32.negate", null);
					case CPHaxeFloat:
						unaryAllowed(requireSource(CPHaxeFloat), result, "haxe.f64.negate", null);
					case _:
						CPUOperationRejected("numeric negation requires an Int, UInt, or Float result");
				}
			case CPUOBitwiseNot:
				switch result.sourceType {
					case CPHaxeInt:
						unaryAllowed(requireSource(CPHaxeInt), result, "haxe.i32.bit-not", CPHI32BitwiseNot);
					case CPHaxeUInt:
						unaryAllowed(requireSource(CPHaxeUInt), result, "haxe.u32.bit-not", null);
					case _:
						CPUOperationRejected("bitwise complement requires an Int or UInt result");
				}
			case CPUOLogicalNot: operand.sourceType == CPHaxeBool && result.sourceType == CPHaxeBool ? unaryAllowed(operand, result, "haxe.bool.not",
					null) : CPUOperationRejected("logical not requires Bool input and result");
		};
	}

	/** Decide operand coercions, result type, and direct/helper ownership for a binary operation. */
	public static function binaryOperation(operation:CPrimitiveBinaryOperator, left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping,
			result:CPrimitiveTypeMapping):CPrimitiveBinaryOperationResult {
		if (!isOrdinaryNonNullable(left) || !isOrdinaryNonNullable(right) || !isOrdinaryNonNullable(result)) {
			return CPBOperationRejected("primitive binary operations require ordinary non-null Haxe scalar operands and results");
		}
		return switch operation {
			case CPBOAdd | CPBOSubtract | CPBOMultiply:
				arithmeticOperation(operation, left, right, result);
			case CPBODivide: result.sourceType == CPHaxeFloat && isNumeric(left) && isNumeric(right) ? binaryAllowed(requireSource(CPHaxeFloat),
					requireSource(CPHaxeFloat), result, "haxe.f64.divide",
					CPHF64Divide) : CPBOperationRejected("Haxe primitive division requires numeric operands and a Float result");
			case CPBOModulo:
				moduloOperation(left, right, result);
			case CPBOShiftLeft | CPBOShiftRight | CPBOUnsignedShiftRight:
				shiftOperation(operation, left, right, result);
			case CPBOBitAnd | CPBOBitOr | CPBOBitXor:
				bitOperation(operation, left, right, result);
			case CPBOEqual | CPBONotEqual:
				equalityOperation(operation, left, right, result);
			case CPBOLess | CPBOLessEqual | CPBOGreater | CPBOGreaterEqual:
				comparisonOperation(operation, left, right, result);
		};
	}

	public static function helperId(kind:CPrimitiveHelperKind):String {
		return switch kind {
			case CPHU32ToI32Bits: "hxc.u32.to.i32.bits";
			case CPHI32Add: "hxc.i32.add.wrapping";
			case CPHI32Subtract: "hxc.i32.subtract.wrapping";
			case CPHI32Multiply: "hxc.i32.multiply.wrapping";
			case CPHI32Negate: "hxc.i32.negate.wrapping";
			case CPHI32Modulo: "hxc.i32.modulo.zero-safe";
			case CPHU32Modulo: "hxc.u32.modulo.zero-safe";
			case CPHI32ShiftLeft: "hxc.i32.shift-left.masked";
			case CPHI32ShiftRight: "hxc.i32.shift-right.masked";
			case CPHI32UnsignedShiftRight: "hxc.i32.unsigned-shift-right.masked";
			case CPHI32BitAnd: "hxc.i32.bit-and";
			case CPHI32BitOr: "hxc.i32.bit-or";
			case CPHI32BitXor: "hxc.i32.bit-xor";
			case CPHI32BitwiseNot: "hxc.i32.bit-not";
			case CPHF64Divide: "hxc.f64.divide.zero-safe";
			case CPHF64Modulo: "hxc.f64.modulo";
			case CPHF64ToI32Saturating: "hxc.f64.to.i32.saturating";
		};
	}

	public static function helperKind(id:String):Null<CPrimitiveHelperKind> {
		return switch id {
			case "hxc.u32.to.i32.bits": CPHU32ToI32Bits;
			case "hxc.i32.add.wrapping": CPHI32Add;
			case "hxc.i32.subtract.wrapping": CPHI32Subtract;
			case "hxc.i32.multiply.wrapping": CPHI32Multiply;
			case "hxc.i32.negate.wrapping": CPHI32Negate;
			case "hxc.i32.modulo.zero-safe": CPHI32Modulo;
			case "hxc.u32.modulo.zero-safe": CPHU32Modulo;
			case "hxc.i32.shift-left.masked": CPHI32ShiftLeft;
			case "hxc.i32.shift-right.masked": CPHI32ShiftRight;
			case "hxc.i32.unsigned-shift-right.masked": CPHI32UnsignedShiftRight;
			case "hxc.i32.bit-and": CPHI32BitAnd;
			case "hxc.i32.bit-or": CPHI32BitOr;
			case "hxc.i32.bit-xor": CPHI32BitXor;
			case "hxc.i32.bit-not": CPHI32BitwiseNot;
			case "hxc.f64.divide.zero-safe": CPHF64Divide;
			case "hxc.f64.modulo": CPHF64Modulo;
			case "hxc.f64.to.i32.saturating": CPHF64ToI32Saturating;
			case _: null;
		};
	}

	public static function helperDependencies(kind:CPrimitiveHelperKind):Array<CPrimitiveHelperKind> {
		return switch kind {
			case CPHI32Add | CPHI32Subtract | CPHI32Multiply | CPHI32Negate | CPHI32ShiftLeft | CPHI32ShiftRight | CPHI32UnsignedShiftRight | CPHI32BitAnd |
				CPHI32BitOr | CPHI32BitXor | CPHI32BitwiseNot:
				[CPHU32ToI32Bits];
			case CPHU32ToI32Bits | CPHI32Modulo | CPHU32Modulo | CPHF64Divide | CPHF64Modulo | CPHF64ToI32Saturating:
				[];
		};
	}

	public static function sourceTypeKey(sourceType:CPrimitiveSourceType):String {
		return switch sourceType {
			case CPHaxeVoid: "Void";
			case CPHaxeBool: "Bool";
			case CPHaxeInt: "Int";
			case CPHaxeUInt: "UInt";
			case CPHaxeFloat: "Float";
			case CPCFloat32: "c.Float32";
			case CPCExactInteger(width, signed): 'c.${signed ? "Int" : "UInt"}$width';
			case CPCSize: "c.Size";
			case CPCPtrDiff: "c.PtrDiff";
			case CPCIntPtr: "c.IntPtr";
			case CPCUIntPtr: "c.UIntPtr";
		}
	}

	public static function sourceTypes():Array<CPrimitiveSourceType> {
		return [
			CPHaxeVoid,
			CPHaxeBool,
			CPHaxeInt,
			CPHaxeUInt,
			CPHaxeFloat,
			CPCFloat32,
			CPCExactInteger(8, true),
			CPCExactInteger(16, true),
			CPCExactInteger(32, true),
			CPCExactInteger(64, true),
			CPCExactInteger(8, false),
			CPCExactInteger(16, false),
			CPCExactInteger(32, false),
			CPCExactInteger(64, false),
			CPCSize,
			CPCPtrDiff,
			CPCIntPtr,
			CPCUIntPtr
		];
	}

	static function arithmeticOperation(operation:CPrimitiveBinaryOperator, left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping,
			result:CPrimitiveTypeMapping):CPrimitiveBinaryOperationResult {
		final suffix = switch operation {
			case CPBOAdd: "add";
			case CPBOSubtract: "subtract";
			case CPBOMultiply: "multiply";
			case _: return CPBOperationRejected("non-arithmetic operator reached arithmetic primitive selection");
		};
		return switch result.sourceType {
			case CPHaxeInt: final helper = switch operation {
					case CPBOAdd: CPHI32Add;
					case CPBOSubtract: CPHI32Subtract;
					case CPBOMultiply: CPHI32Multiply;
					case _: return CPBOperationRejected("non-arithmetic operator reached signed arithmetic selection");
				}; isIntegerLike(left) && isIntegerLike(right) ? binaryAllowed(requireSource(CPHaxeInt), requireSource(CPHaxeInt), result, 'haxe.i32.$suffix',
					helper) : CPBOperationRejected("Int arithmetic requires integer operands");
			case CPHaxeUInt: isIntegerLike(left) && isIntegerLike(right) ? binaryAllowed(requireSource(CPHaxeUInt), requireSource(CPHaxeUInt), result,
					'haxe.u32.$suffix', null) : CPBOperationRejected("UInt arithmetic requires integer operands");
			case CPHaxeFloat: isNumeric(left) && isNumeric(right) ? binaryAllowed(requireSource(CPHaxeFloat), requireSource(CPHaxeFloat), result,
					'haxe.f64.$suffix', null) : CPBOperationRejected("Float arithmetic requires numeric operands");
			case _:
				CPBOperationRejected("primitive arithmetic requires an Int, UInt, or Float result");
		};
	}

	static function moduloOperation(left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping, result:CPrimitiveTypeMapping):CPrimitiveBinaryOperationResult {
		return switch result.sourceType {
			case CPHaxeInt: isIntegerLike(left) && isIntegerLike(right) ? binaryAllowed(requireSource(CPHaxeInt), requireSource(CPHaxeInt), result,
					"haxe.i32.modulo", CPHI32Modulo) : CPBOperationRejected("Int modulo requires integer operands");
			case CPHaxeUInt: isIntegerLike(left) && isIntegerLike(right) ? binaryAllowed(requireSource(CPHaxeUInt), requireSource(CPHaxeUInt), result,
					"haxe.u32.modulo", CPHU32Modulo) : CPBOperationRejected("UInt modulo requires integer operands");
			case CPHaxeFloat: isNumeric(left) && isNumeric(right) ? binaryAllowed(requireSource(CPHaxeFloat), requireSource(CPHaxeFloat), result,
					"haxe.f64.modulo", CPHF64Modulo) : CPBOperationRejected("Float modulo requires numeric operands");
			case _:
				CPBOperationRejected("primitive modulo requires an Int, UInt, or Float result");
		};
	}

	static function shiftOperation(operation:CPrimitiveBinaryOperator, left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping,
			result:CPrimitiveTypeMapping):CPrimitiveBinaryOperationResult {
		if (!isIntegerLike(left) || !isIntegerLike(right)) {
			return CPBOperationRejected("shift operands must be Int or UInt");
		}
		final operationSuffix = switch operation {
			case CPBOShiftLeft: "shift-left";
			case CPBOShiftRight: "shift-right";
			case CPBOUnsignedShiftRight: "unsigned-shift-right";
			case _: return CPBOperationRejected("non-shift operator reached shift primitive selection");
		};
		return switch result.sourceType {
			case CPHaxeInt:
				final helper = switch operation {
					case CPBOShiftLeft: CPHI32ShiftLeft;
					case CPBOShiftRight: CPHI32ShiftRight;
					case CPBOUnsignedShiftRight: CPHI32UnsignedShiftRight;
					case _: return CPBOperationRejected("non-shift operator reached signed shift selection");
				};
				binaryAllowed(requireSource(CPHaxeInt), requireSource(CPHaxeInt), result, 'haxe.i32.$operationSuffix.masked', helper);
			case CPHaxeUInt:
				binaryAllowed(requireSource(CPHaxeUInt), requireSource(CPHaxeInt), result, 'haxe.u32.$operationSuffix.masked', null);
			case _:
				CPBOperationRejected("shift result must be Int or UInt");
		};
	}

	static function bitOperation(operation:CPrimitiveBinaryOperator, left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping,
			result:CPrimitiveTypeMapping):CPrimitiveBinaryOperationResult {
		if (!isIntegerLike(left) || !isIntegerLike(right)) {
			return CPBOperationRejected("bitwise operands must be Int or UInt");
		}
		final suffix = switch operation {
			case CPBOBitAnd: "bit-and";
			case CPBOBitOr: "bit-or";
			case CPBOBitXor: "bit-xor";
			case _: return CPBOperationRejected("non-bitwise operator reached bitwise primitive selection");
		};
		return switch result.sourceType {
			case CPHaxeInt:
				final helper = switch operation {
					case CPBOBitAnd: CPHI32BitAnd;
					case CPBOBitOr: CPHI32BitOr;
					case CPBOBitXor: CPHI32BitXor;
					case _: return CPBOperationRejected("non-bitwise operator reached signed bitwise selection");
				};
				binaryAllowed(requireSource(CPHaxeInt), requireSource(CPHaxeInt), result, 'haxe.i32.$suffix', helper);
			case CPHaxeUInt:
				binaryAllowed(requireSource(CPHaxeUInt), requireSource(CPHaxeUInt), result, 'haxe.u32.$suffix', null);
			case _:
				CPBOperationRejected("bitwise result must be Int or UInt");
		};
	}

	static function equalityOperation(operation:CPrimitiveBinaryOperator, left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping,
			result:CPrimitiveTypeMapping):CPrimitiveBinaryOperationResult {
		if (result.sourceType != CPHaxeBool) {
			return CPBOperationRejected("equality requires a Bool result");
		}
		final suffix = operation == CPBOEqual ? "equal" : "not-equal";
		if (left.sourceType == CPHaxeBool && right.sourceType == CPHaxeBool) {
			return binaryAllowed(left, right, result, 'haxe.bool.$suffix', null);
		}
		final common = commonNumericOperand(left, right);
		return common == null ? CPBOperationRejected("equality operands require a common primitive numeric type") : binaryAllowed(common, common, result,
			'haxe.${numericKey(common)}.$suffix', null);
	}

	static function comparisonOperation(operation:CPrimitiveBinaryOperator, left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping,
			result:CPrimitiveTypeMapping):CPrimitiveBinaryOperationResult {
		if (result.sourceType != CPHaxeBool) {
			return CPBOperationRejected("ordered comparison requires a Bool result");
		}
		final common = commonNumericOperand(left, right);
		if (common == null) {
			return CPBOperationRejected("ordered comparison operands require a common primitive numeric type");
		}
		final suffix = switch operation {
			case CPBOLess: "less";
			case CPBOLessEqual: "less-equal";
			case CPBOGreater: "greater";
			case CPBOGreaterEqual: "greater-equal";
			case _: return CPBOperationRejected("non-comparison operator reached ordered comparison selection");
		};
		return binaryAllowed(common, common, result, 'haxe.${numericKey(common)}.$suffix', null);
	}

	static function commonNumericOperand(left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping):Null<CPrimitiveTypeMapping> {
		if (!isNumeric(left) || !isNumeric(right)) {
			return null;
		}
		if (left.sourceType == CPHaxeFloat || right.sourceType == CPHaxeFloat) {
			return requireSource(CPHaxeFloat);
		}
		if (left.sourceType == CPHaxeUInt && right.sourceType == CPHaxeUInt) {
			return requireSource(CPHaxeUInt);
		}
		if (left.sourceType == CPHaxeInt && right.sourceType == CPHaxeInt) {
			return requireSource(CPHaxeInt);
		}
		return null;
	}

	static function numericKey(mapping:CPrimitiveTypeMapping):String {
		return switch mapping.sourceType {
			case CPHaxeInt: "i32";
			case CPHaxeUInt: "u32";
			case CPHaxeFloat: "f64";
			case _: throw "non-numeric primitive reached numeric operation identity";
		};
	}

	static function unaryAllowed(operand:CPrimitiveTypeMapping, result:CPrimitiveTypeMapping, operationId:String,
			helper:Null<CPrimitiveHelperKind>):CPrimitiveUnaryOperationResult {
		return CPUOperationAllowed(new CPrimitiveUnaryOperationDecision(operand, result, operationId,
			helper == null ? IRIStatic : IRIProgramLocal(helperId(helper))));
	}

	static function binaryAllowed(left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping, result:CPrimitiveTypeMapping, operationId:String,
			helper:Null<CPrimitiveHelperKind>):CPrimitiveBinaryOperationResult {
		return CPBOperationAllowed(new CPrimitiveBinaryOperationDecision(left, right, result, operationId,
			helper == null ? IRIStatic : IRIProgramLocal(helperId(helper))));
	}

	static function requireSource(sourceType:CPrimitiveSourceType):CPrimitiveTypeMapping
		return baseMapping(sourceType);

	static function isOrdinaryNonNullable(mapping:CPrimitiveTypeMapping):Bool {
		return mapping.nullability == CPNonNullable && switch mapping.sourceType {
			case CPHaxeBool | CPHaxeInt | CPHaxeUInt | CPHaxeFloat: true;
			case _: false;
		};
	}

	static function isNumeric(mapping:CPrimitiveTypeMapping):Bool {
		return switch mapping.sourceType {
			case CPHaxeInt | CPHaxeUInt | CPHaxeFloat: true;
			case _: false;
		};
	}

	static function isIntegerLike(mapping:CPrimitiveTypeMapping):Bool {
		return switch mapping.sourceType {
			case CPHaxeInt | CPHaxeUInt: true;
			case _: false;
		};
	}

	static function implicitConversion(source:CPrimitiveTypeMapping, target:CPrimitiveTypeMapping):CPrimitiveConversionResult {
		return switch [source.sourceType, target.sourceType] {
			case [CPHaxeInt, CPHaxeFloat] | [CPHaxeUInt, CPHaxeFloat]:
				allowed(source, target, CPUImplicit, CPExact, IRCNumericExact, IRIStatic, false);
			case [CPHaxeInt, CPHaxeUInt]:
				allowed(source, target, CPUImplicit, CPModuloBits(32), IRCNumericWrapping, IRIStatic, false);
			case [CPHaxeUInt, CPHaxeInt]:
				allowed(source, target, CPUImplicit, CPTwosComplementBits(32), IRCNumericWrapping, IRIProgramLocal("hxc.u32.to.i32.bits"), false);
			case _:
				CPConversionRejected("Haxe typing admits no implicit conversion for this primitive pair");
		}
	}

	static function baseMapping(sourceType:CPrimitiveSourceType):CPrimitiveTypeMapping {
		return switch sourceType {
			case CPHaxeVoid:
				new CPrimitiveTypeMapping(sourceType, CPNonNullable, "void", null, CPWidthNone, CPSignNone, CPStorageVoid, IRTVoid);
			case CPHaxeBool:
				new CPrimitiveTypeMapping(sourceType, CPNonNullable, "bool", "stdbool.h", CPWidthAbiDefined, CPSignBoolean, CPStorageScalar, IRTBool);
			case CPHaxeInt:
				fixedIntegerMapping(sourceType, 32, true);
			case CPHaxeUInt:
				fixedIntegerMapping(sourceType, 32, false);
			case CPHaxeFloat:
				new CPrimitiveTypeMapping(sourceType, CPNonNullable, "double", null, CPWidthExact(64), CPSignNone, CPStorageScalar, IRTFloat(64));
			case CPCFloat32:
				new CPrimitiveTypeMapping(sourceType, CPNonNullable, "float", null, CPWidthExact(32), CPSignNone, CPStorageScalar, IRTFloat(32));
			case CPCExactInteger(width, signed):
				fixedIntegerMapping(sourceType, width, signed);
			case CPCSize:
				abiIntegerMapping(sourceType, "size_t", "stddef.h", CPSignUnsigned, IRAKSize);
			case CPCPtrDiff:
				abiIntegerMapping(sourceType, "ptrdiff_t", "stddef.h", CPSignSigned, IRAKPtrDiff);
			case CPCIntPtr:
				abiIntegerMapping(sourceType, "intptr_t", "stdint.h", CPSignSigned, IRAKIntPtr);
			case CPCUIntPtr:
				abiIntegerMapping(sourceType, "uintptr_t", "stdint.h", CPSignUnsigned, IRAKUIntPtr);
		}
	}

	static function fixedIntegerMapping(sourceType:CPrimitiveSourceType, width:Int, signed:Bool):CPrimitiveTypeMapping {
		if (width != 8 && width != 16 && width != 32 && width != 64) {
			throw 'Unsupported exact primitive integer width: $width';
		}
		return new CPrimitiveTypeMapping(sourceType, CPNonNullable, '${signed ? "int" : "uint"}${width}_t', "stdint.h", CPWidthExact(width),
			signed ? CPSignSigned : CPSignUnsigned, CPStorageScalar, IRTInt(width, signed));
	}

	static function abiIntegerMapping(sourceType:CPrimitiveSourceType, cSpelling:String, header:String, signedness:CPrimitiveSignedness,
			kind:HxcIRAbiIntegerKind):CPrimitiveTypeMapping {
		return new CPrimitiveTypeMapping(sourceType, CPNonNullable, cSpelling, header, CPWidthTargetAbi, signedness, CPStorageScalar, IRTAbiInteger(kind));
	}

	static function integerFacts(sourceType:CPrimitiveSourceType):Null<CIntegerFacts> {
		return switch sourceType {
			case CPHaxeInt: {width: 32, signed: true};
			case CPHaxeUInt: {width: 32, signed: false};
			case CPCExactInteger(width, signed): {width: width, signed: signed};
			case CPHaxeVoid | CPHaxeBool | CPHaxeFloat | CPCFloat32 | CPCSize | CPCPtrDiff | CPCIntPtr | CPCUIntPtr: null;
		}
	}

	static function rangeIsSubset(source:CIntegerFacts, target:CIntegerFacts):Bool {
		if (source.signed == target.signed) {
			return source.width <= target.width;
		}
		return !source.signed && target.signed && source.width < target.width;
	}

	static function checkedHelperId(target:CPrimitiveTypeMapping):String
		return 'hxc.primitive.to.${sourceTypeKey(target.sourceType)}.checked';

	static function sameMapping(left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping):Bool
		return sameValueType(left, right) && left.nullability == right.nullability;

	static function sameValueType(left:CPrimitiveTypeMapping, right:CPrimitiveTypeMapping):Bool
		return sourceTypeKey(left.sourceType) == sourceTypeKey(right.sourceType);

	static function allowed(source:CPrimitiveTypeMapping, target:CPrimitiveTypeMapping, use:CPrimitiveConversionUse, meaning:CPrimitiveConversionMeaning,
			irKind:HxcIRConversionKind, implementation:HxcIRImplementation, failureRequired:Bool):CPrimitiveConversionResult {
		return CPConversionAllowed(new CPrimitiveConversionDecision(source, target, use, meaning, irKind, implementation, failureRequired));
	}

	static function assertKnownProfile(profile:CProfile):Void {
		switch profile {
			case Portable | Metal:
		}
	}
}

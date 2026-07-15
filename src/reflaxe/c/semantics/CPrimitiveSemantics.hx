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
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveMappingResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveNullability;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveSignedness;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveSourceType;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveStorage;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveTypeMapping;
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

	public static function sourceTypeKey(sourceType:CPrimitiveSourceType):String {
		return switch sourceType {
			case CPHaxeVoid: "Void";
			case CPHaxeBool: "Bool";
			case CPHaxeInt: "Int";
			case CPHaxeUInt: "UInt";
			case CPHaxeFloat: "Float";
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
			case CPHaxeVoid | CPHaxeBool | CPHaxeFloat | CPCSize | CPCPtrDiff | CPCIntPtr | CPCUIntPtr: null;
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

import haxe.Json;
import reflaxe.c.CProfile;
import reflaxe.c.ir.HxcIR.HxcIRConversionKind;
import reflaxe.c.semantics.CPrimitiveContract;
import reflaxe.c.semantics.CPrimitiveSemantics;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionUse;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveMappingResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveSourceType;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveTypeMapping;

class PrimitiveSemanticsGolden {
	public static inline final CONTRACT_PREFIX = "HXC_PRIMITIVE_CONTRACT=";

	static function main():Void {
		requireElidedIdentity();
		requireRejectedImplicitBoolConversion();
		requireRejectedAbiWrappingConversion();
		requireExplicitFloat32Conversions();
		Sys.println(CONTRACT_PREFIX + Json.stringify(CPrimitiveContract.snapshot()));
	}

	static function requireElidedIdentity():Void {
		final boolType = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, CPHaxeBool));
		switch CPrimitiveSemantics.conversion(boolType, boolType, CPUImplicit) {
			case CPConversionElided:
			case CPConversionAllowed(_) | CPConversionRejected(_):
				throw "Bool identity unexpectedly produced a conversion instruction";
		}
	}

	static function requireRejectedImplicitBoolConversion():Void {
		final boolType = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, CPHaxeBool));
		final intType = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, CPHaxeInt));
		switch CPrimitiveSemantics.conversion(boolType, intType, CPUImplicit) {
			case CPConversionRejected(_):
			case CPConversionElided | CPConversionAllowed(_):
				throw "Bool-to-Int unexpectedly bypassed Haxe typing";
		}
	}

	static function requireRejectedAbiWrappingConversion():Void {
		final sizeType = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, CPCSize));
		final intType = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, CPHaxeInt));
		switch CPrimitiveSemantics.conversion(sizeType, intType, CPUWrapping) {
			case CPConversionRejected(_):
			case CPConversionElided | CPConversionAllowed(_):
				throw "target-ABI integer conversion bypassed ABI fact resolution";
		}
	}

	static function requireExplicitFloat32Conversions():Void {
		final float64Type = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, CPHaxeFloat));
		final float32Type = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, CPCFloat32));
		switch CPrimitiveSemantics.conversion(float64Type, float32Type, CPUImplicit) {
			case CPConversionRejected(_):
			case CPConversionElided | CPConversionAllowed(_):
				throw "Float-to-Float32 unexpectedly became implicit";
		}
		switch CPrimitiveSemantics.conversion(float64Type, float32Type, CPUFloat32Narrow) {
			case CPConversionAllowed(decision):
				if (decision.irKind != IRCNumericRoundBinary32 || decision.failureRequired) {
					throw "Float32 narrowing lost its explicit infallible HxcIR operation";
				}
			case CPConversionElided | CPConversionRejected(_):
				throw "Float32 narrowing contract was rejected";
		}
		switch CPrimitiveSemantics.conversion(float32Type, float64Type, CPUFloat32Widen) {
			case CPConversionAllowed(decision):
				if (decision.irKind != IRCNumericWidenBinary64 || decision.failureRequired) {
					throw "Float32 widening lost its explicit infallible HxcIR operation";
				}
			case CPConversionElided | CPConversionRejected(_):
				throw "Float32 widening contract was rejected";
		}
	}

	static function requireMapping(result:CPrimitiveMappingResult):CPrimitiveTypeMapping {
		return switch result {
			case CPMappingAvailable(mapping): mapping;
			case CPMappingRejected(reason): throw reason;
		}
	}
}

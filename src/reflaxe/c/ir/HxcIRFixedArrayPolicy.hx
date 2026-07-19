package reflaxe.c.ir;

import reflaxe.c.ir.HxcIR.HxcIRTypeRef;

/** Closed result of validating zero-initialized automatic fixed storage. */
enum HxcIRFixedArrayStorageDecision {
	IRFASAutomatic(elementBytes:Int, totalBytes:Int);
	IRFASInvalidLength(length:Int);
	IRFASUnsupportedElement;
	IRFASSizeOverflow(elementBytes:Int, length:Int);
	IRFASOverBudget(elementBytes:Int, totalBytes:Int, maximumBytes:Int);
}

/**
	Shared policy for the bounded zero-initialized fixed-array slice.

	The ceiling is deliberately per array, not a whole-function stack promise.
	Whole-frame accounting remains future analysis; this bound prevents the new
	compact construction form from becoming an unbounded stack-allocation seam.
**/
class HxcIRFixedArrayPolicy {
	public static inline final MAX_AUTOMATIC_ZERO_BYTES = 64 * 1024;
	static inline final MAX_SIGNED_INT = 2147483647;

	public static function zeroStorage(element:HxcIRTypeRef, length:Int):HxcIRFixedArrayStorageDecision {
		if (length <= 0) {
			return IRFASInvalidLength(length);
		}
		final elementBytes = switch element {
			case IRTInt(width, _) if (width == 8 || width == 16 || width == 32 || width == 64): Std.int(width / 8);
			case IRTFloat(32): 4;
			case IRTFloat(64): 8;
			case _: return IRFASUnsupportedElement;
		};
		if (length > Std.int(MAX_SIGNED_INT / elementBytes)) {
			return IRFASSizeOverflow(elementBytes, length);
		}
		final totalBytes = length * elementBytes;
		return totalBytes > MAX_AUTOMATIC_ZERO_BYTES ? IRFASOverBudget(elementBytes, totalBytes,
			MAX_AUTOMATIC_ZERO_BYTES) : IRFASAutomatic(elementBytes, totalBytes);
	}
}

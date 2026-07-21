import c.CArray;
import c.ConstSpan;
import c.Span;
import c.UInt8;

class SpanFixture {
	static inline final GRID_WIDTH = 32;
	static inline final GRID_HEIGHT = 16;
	static inline final GRID_DEPTH = 32;

	static function mutableSum():Int {
		var values:CArray<Int, Length4> = [3, 5, 7, 11];
		var view:Span<Int> = values.span();
		view[1] = 13;
		var total:Int = 0;
		for (value in view) {
			total += value;
		}
		return total;
	}

	static function constSum():Int {
		var values:CArray<Int, Length4> = [2, 4, 8, 16];
		var view:ConstSpan<Int> = values.constSpan();
		var total:Int = view[0];
		for (value in view) {
			total += value;
		}
		return total;
	}

	static function checkedAt(index:Int):Int {
		var values:CArray<Int, Length4> = [1, 3, 8, 21];
		var view:ConstSpan<Int> = values.constSpan();
		return view[index];
	}

	static function readAt(values:ConstSpan<UInt8>, index:Int):UInt8 {
		return values[index];
	}

	static function replaceAt(values:Span<UInt8>, index:Int, replacement:UInt8):UInt8 {
		values[index] = replacement;
		return values[index];
	}

	static function forwardRead(values:ConstSpan<UInt8>, index:Int):UInt8 {
		return readAt(values, index);
	}

	static function forwardReplace(values:Span<UInt8>, index:Int, replacement:UInt8):UInt8 {
		return replaceAt(values, index, replacement);
	}

	/** Prove a span's pointer and length survive later argument control flow. */
	static function spanBeforeConditionalArgument(selectThird:Bool):UInt8 {
		var values:CArray<UInt8, Length4> = [
			c.IntConvert.modulo(2),
			c.IntConvert.modulo(4),
			c.IntConvert.modulo(8),
			c.IntConvert.modulo(16)
		];
		var readOnly:ConstSpan<UInt8> = values.constSpan();
		return readAt(readOnly, selectThird ? 2 : 1);
	}

	static function parameterRoundTrip(replacement:UInt8):UInt8 {
		var values:CArray<UInt8, Length4> = CArray.zero(4);
		var mutable:Span<UInt8> = values.span();
		forwardReplace(mutable, 2, replacement);
		var readOnly:ConstSpan<UInt8> = values.constSpan();
		return forwardRead(readOnly, 2);
	}

	static function linearIndex(x:Int, y:Int, z:Int):Int {
		return x + GRID_WIDTH * (y + GRID_HEIGHT * z);
	}

	static function zeroedGridCell():UInt8 {
		var values:CArray<UInt8, GridVolume> = CArray.zero(GRID_WIDTH * GRID_HEIGHT * GRID_DEPTH);
		var view:ConstSpan<UInt8> = values.constSpan();
		return view[linearIndex(0, 0, 0)];
	}

	static function mutatedGridCell(replacement:UInt8):UInt8 {
		var values:CArray<UInt8, GridVolume> = CArray.zero(GRID_WIDTH * GRID_HEIGHT * GRID_DEPTH);
		var index:Int = linearIndex(31, 15, 31);
		values[index] = replacement;
		var mutable:Span<UInt8> = values.span();
		mutable[index] = replacement;
		var observed:UInt8 = values[0];
		for (value in mutable) {
			observed = value;
		}
		var readOnly:ConstSpan<UInt8> = values.constSpan();
		observed = readOnly[index];
		return observed;
	}

	static function main():Void {
		mutableSum();
		constSum();
		checkedAt(2);
		parameterRoundTrip(zeroedGridCell());
		spanBeforeConditionalArgument(true);
		zeroedGridCell();
		mutatedGridCell(zeroedGridCell());
	}

	/** A seen but unreachable String signature must not become a runtime root. */
	static function runtimeTypeOnly(value:String):String {
		return value;
	}
}

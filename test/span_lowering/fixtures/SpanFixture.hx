import c.CArray;
import c.ConstSpan;
import c.Span;

class SpanFixture {
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

	static function main():Void {
		mutableSum();
		constSum();
		checkedAt(2);
	}

	/** A seen but unreachable String signature must not become a runtime root. */
	static function runtimeTypeOnly(value:String):String {
		return value;
	}
}

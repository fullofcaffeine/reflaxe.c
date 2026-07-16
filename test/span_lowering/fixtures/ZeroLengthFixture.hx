import c.CArray;
import c.ConstSpan;

class ZeroLengthFixture {
	static function zero():Int {
		var values:CArray<Int, Length4> = [];
		var view:ConstSpan<Int> = values.constSpan();
		return view[0];
	}

	static function main():Void {
		zero();
	}
}

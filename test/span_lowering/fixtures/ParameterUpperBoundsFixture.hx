import c.CArray;
import c.ConstSpan;

class ParameterUpperBoundsFixture {
	static function readAt(values:ConstSpan<Int>, index:Int):Int {
		return values[index];
	}

	static function main():Void {
		var values:CArray<Int, Length4> = [1, 2, 3, 4];
		var view:ConstSpan<Int> = values.constSpan();
		readAt(view, 4);
	}
}

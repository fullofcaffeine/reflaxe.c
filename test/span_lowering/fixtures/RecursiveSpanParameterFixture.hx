import c.CArray;
import c.ConstSpan;

class RecursiveSpanParameterFixture {
	static function recurse(values:ConstSpan<Int>):Int {
		return recurse(values);
	}

	static function main():Void {
		var values:CArray<Int, Length4> = [1, 2, 3, 4];
		var view:ConstSpan<Int> = values.constSpan();
		recurse(view);
	}
}

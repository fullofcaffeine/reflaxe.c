import c.CArray;
import c.ConstSpan;

class UpperBoundsFixture {
	static function read():Int {
		var values:CArray<Int, Length4> = [1, 2, 3, 4];
		var view:ConstSpan<Int> = values.constSpan();
		var index:Int = 4;
		return view[index];
	}

	public static function main():Void {
		read();
	}
}

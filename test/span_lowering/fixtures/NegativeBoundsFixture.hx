import c.CArray;
import c.ConstSpan;

class NegativeBoundsFixture {
	static function read():Int {
		var values:CArray<Int, Length4> = [1, 2, 3, 4];
		var view:ConstSpan<Int> = values.constSpan();
		var index:Int = -1;
		return view[index];
	}

	static function main():Void {
		read();
	}
}

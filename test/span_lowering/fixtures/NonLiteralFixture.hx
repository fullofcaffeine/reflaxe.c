import c.CArray;

class NonLiteralFixture {
	static function source():Array<Int> {
		return [1, 2, 3, 4];
	}

	static function main():Void {
		var values:CArray<Int, Length4> = source();
		values[0];
	}
}

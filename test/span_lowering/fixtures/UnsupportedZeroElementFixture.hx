import c.CArray;

class UnsupportedZeroElementFixture {
	static function main():Void {
		var values:CArray<Bool, Length4> = CArray.zero(4);
		values[0];
	}
}

import c.CArray;
import c.UInt8;

class NonConstantConstructionFixture {
	static function length():Int {
		return 16;
	}

	static function main():Void {
		var values:CArray<UInt8, GridVolume> = CArray.zero(length());
		values[0];
	}
}

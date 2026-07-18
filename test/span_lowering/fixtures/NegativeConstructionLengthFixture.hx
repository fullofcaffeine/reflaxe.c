import c.CArray;
import c.UInt8;

class NegativeConstructionLengthFixture {
	static function main():Void {
		var values:CArray<UInt8, GridVolume> = CArray.zero(-1);
		values[0];
	}
}

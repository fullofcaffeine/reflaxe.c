import c.CArray;
import c.UInt8;

class ZeroConstructionLengthFixture {
	static function main():Void {
		var values:CArray<UInt8, GridVolume> = CArray.zero(0);
		values[0];
	}
}

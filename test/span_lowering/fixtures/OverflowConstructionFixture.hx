import c.CArray;
import c.UInt8;

class OverflowConstructionFixture {
	static function main():Void {
		var values:CArray<UInt8, GridVolume> = CArray.zero(65536 * 65536);
		values[0];
	}
}

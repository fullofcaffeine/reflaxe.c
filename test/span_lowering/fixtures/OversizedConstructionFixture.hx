import c.CArray;
import c.UInt8;

class OversizedConstructionFixture {
	static function main():Void {
		var values:CArray<UInt8, GridVolume> = CArray.zero(65537);
		values[0];
	}
}

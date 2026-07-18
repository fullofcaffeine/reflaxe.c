import c.CArray;
import c.UInt8;

class StaticOutOfBoundsFixture {
	static function read():UInt8 {
		var values:CArray<UInt8, GridVolume> = CArray.zero(32 * 16 * 32);
		return values[16384];
	}

	static function main():Void {
		read();
	}
}

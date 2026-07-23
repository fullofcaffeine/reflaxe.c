import c.CArray;
import c.UInt8;

private final class ZeroLengthOwner {
	var bytes:CArray<UInt8, Length4> = CArray.zero(0);

	public function new() {}
}

class OwnedArrayZeroLengthFixture {
	static function main():Void {
		final owner = new ZeroLengthOwner();
	}
}

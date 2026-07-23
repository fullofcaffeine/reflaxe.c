import c.CArray;
import c.UInt8;

private final class OversizedOwner {
	var bytes:CArray<UInt8, Length4> = CArray.zero(65537);

	public function new() {}
}

class OwnedArrayOversizedFixture {
	static function main():Void {
		final owner = new OversizedOwner();
	}
}

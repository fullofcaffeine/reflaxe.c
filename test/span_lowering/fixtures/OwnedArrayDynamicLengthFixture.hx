import c.CArray;
import c.UInt8;

private final class DynamicLengthOwner {
	var bytes:CArray<UInt8, Length4> = CArray.zero(selectedLength());

	public function new() {}

	static function selectedLength():Int {
		return 4;
	}
}

class OwnedArrayDynamicLengthFixture {
	static function main():Void {
		final owner = new DynamicLengthOwner();
	}
}

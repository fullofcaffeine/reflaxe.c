import c.CArray;
import c.UInt8;

private final class MissingInitializerOwner {
	var bytes:CArray<UInt8, Length4>;

	public function new() {}
}

class OwnedArrayMissingInitializerFixture {
	static function main():Void {
		final owner = new MissingInitializerOwner();
	}
}

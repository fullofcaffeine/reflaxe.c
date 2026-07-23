import c.CArray;

private final class UnsupportedElementOwner {
	var flags:CArray<Bool, Length4> = CArray.zero(4);

	public function new() {}
}

class OwnedArrayUnsupportedElementFixture {
	static function main():Void {
		final owner = new UnsupportedElementOwner();
	}
}

import c.CArray;
import c.UInt8;

private final class WholeAssignmentOwner {
	var bytes:CArray<UInt8, Length4> = CArray.zero(4);

	public function new() {}

	public function reset():Void {
		bytes = CArray.zero(4);
	}
}

class OwnedArrayWholeAssignmentFixture {
	static function main():Void {
		final owner = new WholeAssignmentOwner();
		owner.reset();
	}
}

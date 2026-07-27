import c.CArray;
import c.ConstSpan;
import c.UInt8;

/**
	Proves that the receiver contract cannot disguise a function-local owner.

	`view()` is a final instance method, but the returned bytes belong to its
	local `values`, not to `self`. HxcIR validates the concrete field origin and
	rejects this dangling pointer before C syntax is selected.
**/
private final class LocalConstSpanOwner {
	public function new() {}

	public function view():ConstSpan<UInt8> {
		var values:CArray<UInt8, Length4> = CArray.zero(4);
		return values.constSpan();
	}
}

class LocalConstSpanReturnFixture {
	static function main():Void {
		final owner = new LocalConstSpanOwner();
		owner.view();
	}
}

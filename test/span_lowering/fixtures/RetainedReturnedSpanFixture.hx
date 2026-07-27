import c.CArray;
import c.ConstSpan;
import c.UInt8;

/**
	Proves that a returned receiver borrow cannot become caller-owned storage.

	The positive contract consumes `owner.view()` directly. Naming that result
	as a caller local would create a longer-lived view whose relationship to the
	receiver must be tracked across later statements, which this bounded
	capability deliberately does not admit.
**/
private final class RetainedSpanOwner {
	final values:CArray<UInt8, Length4> = CArray.zero(4);

	public function new() {}

	public function view():ConstSpan<UInt8> {
		return values.constSpan();
	}
}

class RetainedReturnedSpanFixture {
	static function readAt(values:ConstSpan<UInt8>, index:Int):UInt8 {
		return values[index];
	}

	static function main():Void {
		final owner = new RetainedSpanOwner();
		final retained = owner.view();
		readAt(retained, 0);
	}
}

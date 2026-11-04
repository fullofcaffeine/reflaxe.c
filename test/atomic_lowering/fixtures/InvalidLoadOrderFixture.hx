import c.Atomic;
import c.MemoryOrder;

/** Keeps invalid load ordering addressable so semantic IR validation sees it. */
private final class InvalidLoadOrderOwner {
	final value:Atomic<Int>;

	/** Initialize the location before requesting the invalid operation. */
	public function new() {
		value = new Atomic(0);
	}

	/** C11 forbids release ordering on an atomic load. */
	public function read():Int {
		return value.load(MemoryOrder.Release);
	}
}

/** Must fail before target code is published. */
class InvalidLoadOrderFixture {
	/** Reach the invalid operation. */
	static function main():Void {
		new InvalidLoadOrderOwner().read();
	}
}

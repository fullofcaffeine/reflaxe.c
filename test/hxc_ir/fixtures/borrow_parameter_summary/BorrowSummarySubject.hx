/** Ordinary typed inputs separate direct-reference escape from harmless reads. */
class BorrowSummarySubject {
	/** The focused macro owns verification; this entry point has no side effects. */
	public static function main():Void {}

	/** Same body, 1 reference parameters: unused arguments still need borrow verdicts. */
	public static function safe1(p0:BorrowBox):Int {
		var total = p0.value;
		for (index in 0...4) {
			total += index;
			if (total > 10)
				total -= 1;
			else
				total += 2;
		}
		return total;
	}

	/** Same body, 8 reference parameters: unused arguments still need borrow verdicts. */
	public static function safe8(p0:BorrowBox, p1:BorrowBox, p2:BorrowBox, p3:BorrowBox, p4:BorrowBox, p5:BorrowBox, p6:BorrowBox, p7:BorrowBox):Int {
		var total = p0.value;
		for (index in 0...4) {
			total += index;
			if (total > 10)
				total -= 1;
			else
				total += 2;
		}
		return total;
	}

	/** Same body, 32 reference parameters: unused arguments still need borrow verdicts. */
	public static function safe32(p0:BorrowBox, p1:BorrowBox, p2:BorrowBox, p3:BorrowBox, p4:BorrowBox, p5:BorrowBox, p6:BorrowBox, p7:BorrowBox,
			p8:BorrowBox, p9:BorrowBox, p10:BorrowBox, p11:BorrowBox, p12:BorrowBox, p13:BorrowBox, p14:BorrowBox, p15:BorrowBox, p16:BorrowBox,
			p17:BorrowBox, p18:BorrowBox, p19:BorrowBox, p20:BorrowBox, p21:BorrowBox, p22:BorrowBox, p23:BorrowBox, p24:BorrowBox, p25:BorrowBox,
			p26:BorrowBox, p27:BorrowBox, p28:BorrowBox, p29:BorrowBox, p30:BorrowBox, p31:BorrowBox):Int {
		var total = p0.value;
		for (index in 0...4) {
			total += index;
			if (total > 10)
				total -= 1;
			else
				total += 2;
		}
		return total;
	}

	/** A condition reads a; only the selected b value escapes. */
	public static function condition(a:BorrowBox, b:BorrowBox):BorrowBox
		return if (a != null) b else b;

	/** Only the last block value escapes; a's earlier value is discarded. */
	public static function blockLast(a:BorrowBox, b:BorrowBox):BorrowBox
		return {
			a;
			b;
		};

	/** Reading a to select an arm does not return a itself. */
	public static function switchValue(a:BorrowBox, b:BorrowBox):BorrowBox
		return switch (a.value) {
			case 0: b;
			default: b;
		};

	/** Both possible result references escape. */
	public static function bothArms(a:BorrowBox, b:BorrowBox, choose:Bool):BorrowBox
		return choose ? a : b;

	/** A local alias copies the exact reference. */
	public static function alias(a:BorrowBox):Int {
		final copy = a;
		return copy.value;
	}

	/** Storing a escapes it; using b as a field receiver does not escape b. */
	public static function store(a:BorrowBox, b:BorrowBox):Void {
		b.next = a;
	}

	/** Explicit constructor arguments are initially retaining, before forwarding refinement. */
	public static function construct(a:BorrowBox):Int
		return new BorrowHolder(a).value();

	/** Throwing the reference escapes it. */
	public static function thrown(a:BorrowBox):Void {
		throw a;
	}

	/** Even an immediately invoked closure captures the reference under the current rule. */
	public static function captured(a:BorrowBox):Int {
		final read = () -> a.value;
		return read();
	}

	/** Deep closure capture must not depend on a direct return-value occurrence. */
	public static function deepCapture(a:BorrowBox):Int {
		final outer = () -> {
			final inner = () -> a.value;
			return inner();
		};
		return outer();
	}

	/** Field mutation and a receiver call retain no copy of a. */
	public static function receiver(a:BorrowBox):Int {
		a.value = a.value + 1;
		return a.read();
	}

	/** Initial borrowing leaves explicit call-argument forwarding to the separate refiner. */
	public static function forward(a:BorrowBox):Int
		return receiver(a);

	/** Cast and metadata wrappers do not hide an escaping value. */
	public static function wrapped(a:BorrowBox):BorrowBox
		return (@:borrowSummaryWrapper (cast a : BorrowBox));
}

/** A concrete source reference; no target-specific or reflective test access. */
class BorrowBox {
	public var value:Int;
	public var next:Null<BorrowBox>;

	public function new(value:Int) {
		this.value = value;
		next = null;
	}

	public function read():Int
		return value;
}

/** A constructor capture keeps its ordinary authored class semantics. */
class BorrowHolder {
	final box:BorrowBox;

	public function new(box:BorrowBox)
		this.box = box;

	public function value():Int
		return box.value;
}

import haxe.atomic.AtomicBool;
import haxe.atomic.AtomicInt;
import haxe.atomic.AtomicObject;

/** Gives atomic-reference checks one observable object payload. */
private final class AtomicBox {
	public final value:Int;

	/** Store the immutable value observed after each atomic operation. */
	public function new(value:Int) {
		this.value = value;
	}
}

/** Exercises every integer operation and the shared-wrapper identity contract. */
class AtomicFixture {
	/** Return the holder so escape analysis selects collector-managed storage. */
	static function sharedBox(value:Int):AtomicObject<AtomicBox> {
		final box = new AtomicBox(value);
		return new AtomicObject(box);
	}

	/** Return one box with an independent collector-managed lifetime. */
	static function makeBox(value:Int):AtomicBox {
		return new AtomicBox(value);
	}

	/** Run the semantic checks through the ordinary Haxe standard-library API. */
	static function main():Void {
		final first = new AtomicInt(7);
		final alias = first;
		if (alias.add(5) != 7)
			AtomicExit.fail();
		if (first.sub(2) != 12)
			AtomicExit.fail();
		if (first.and(7) != 10)
			AtomicExit.fail();
		if (first.or(8) != 2)
			AtomicExit.fail();
		if (first.xor(3) != 10)
			AtomicExit.fail();
		if (first.compareExchange(9, 20) != 9)
			AtomicExit.fail();
		if (first.compareExchange(9, 30) != 20)
			AtomicExit.fail();
		if (first.exchange(4) != 20)
			AtomicExit.fail();
		if (first.store(11) != 11 || first.load() != 11)
			AtomicExit.fail();

		final flag = new AtomicBool(false);
		final flagAlias = flag;
		if (flagAlias.exchange(true))
			AtomicExit.fail();
		if (!flag.load())
			AtomicExit.fail();
		if (!flag.compareExchange(true, false))
			AtomicExit.fail();
		if (flag.store(true) != true || !flagAlias.load())
			AtomicExit.fail();

		final object = sharedBox(21);
		final firstBox = object.load();
		final secondBox = makeBox(34);
		final objectAlias = object;
		if (objectAlias.exchange(secondBox).value != 21)
			AtomicExit.fail();
		if (object.load().value != 34)
			AtomicExit.fail();
		if (object.compareExchange(secondBox, firstBox).value != 34)
			AtomicExit.fail();
		if (object.store(secondBox).value != 34 || objectAlias.load().value != 34)
			AtomicExit.fail();
	}
}

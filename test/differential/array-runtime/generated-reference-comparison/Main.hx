/**
 * A comparison borrows each Array operand until its Boolean result is known.
 * Fresh call results have no source local, but still need exactly one cleanup.
 */
final class Main {
	static var calls:Int = 0;

	/** Count evaluation and return either one fresh Array or an absent reference. */
	static function make(present:Bool):Array<Int> {
		calls++;
		return present ? [7] : null;
	}

	/** Match a direct nullable call comparison without an explicit Array local. */
	static function present():Bool
		return make(true) != null;

	/** A later branch must not consume or reevaluate the left operand. */
	static function distinct(choose:Bool):Bool
		return make(true) != (if (choose) make(true) else null);

	/** Protect both null operand orders, identity, branch joins and borrowed owners. */
	static function main():Void {
		if (!present() || null == make(true) || make(false) != null || null != make(false))
			throw "nullable Array comparison changed";
		if (calls != 4)
			throw "comparison reevaluated an operand";
		if (!distinct(true) || !distinct(false) || calls != 7)
			throw "branch comparison changed evaluation";
		final borrowed = make(true);
		final alias = borrowed;
		if (borrowed != alias || borrowed == make(true) || borrowed[0] != 7 || calls != 9)
			throw "comparison consumed a borrowed Array";
		if ([1] == [1])
			throw "Array comparison used contents instead of identity";
	}
}

/**
 * Proves that a nonescaping class can live entirely inside one statement `if` arm.
 *
 * The outer branch owns two values. Its fallthrough and return paths destroy
 * both in reverse construction order, while the sibling path owns neither.
 */

/** A small identity-bearing value whose constructor can also fail. */
final class ConditionalValue {
	final stored:Int;

	/** Store one value, or throw before this instance becomes initialized. */
	public function new(value:Int, shouldFail:Bool) {
		stored = value;
		if (shouldFail)
			throw 99;
	}

	/** Read the stored value while the branch still owns this instance. */
	public function read():Int
		return stored;
}

/** Exercises sibling, fallthrough, nested-return, and constructor-failure edges. */
final class Main {
	/** Use two path-scoped objects and preserve reverse cleanup on every exit. */
	static function branchSum(chooseBranch:Bool, returnInside:Bool):Int {
		var result = 7;
		if (chooseBranch) {
			final first = new ConditionalValue(20, false);
			final second = new ConditionalValue(22, false);
			if (returnInside)
				return first.read() + second.read();
			result = first.read() + second.read() + 1;
		}
		return result;
	}

	/** Keep a real failure edge after one earlier branch-local owner exists. */
	static function branchFailure(chooseBranch:Bool, shouldFail:Bool):Int {
		if (chooseBranch) {
			final first = new ConditionalValue(40, false);
			final second = new ConditionalValue(2, shouldFail);
			return first.read() + second.read();
		}
		return 7;
	}

	/** Stop only if Eval and generated native behavior disagree. */
	static function main():Void
		while (branchSum(false, false) != 7 || branchSum(true, false) != 43 || branchSum(true, true) != 42 || branchFailure(false, false) != 7
			|| branchFailure(true, false) != 42) {}
}

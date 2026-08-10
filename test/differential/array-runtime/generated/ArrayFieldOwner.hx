/**
 * Exercises whole-value replacement of mutable Array fields.
 *
 * An Array field owns one shared container reference. Replacing that field
 * must acquire the incoming container before releasing the previous one, so
 * aliases and `field = field` remain valid. The methods below cover borrowed,
 * fresh, and call-produced replacements without exposing runtime internals to
 * ordinary Haxe source.
 */
final class ArrayFieldOwner {
	/** The current integer sequence; public so the fixture can verify identity through aliases. */
	public var values:Array<Int> = [1];

	/** A managed-element companion that proves replacement does not skip String ownership. */
	public var labels:Array<String> = ["old"];

	/** Construct the owner with the field initializers above. */
	public function new() {}

	/**
	 * Replace both fields with caller-owned Arrays and return the numeric field.
	 *
	 * The return value is another alias to `nextValues`; no element copy or new
	 * container is expected.
	 */
	public function replaceBorrowed(nextValues:Array<Int>, nextLabels:Array<String>):Array<Int> {
		values = nextValues;
		labels = nextLabels;
		return values;
	}

	/** Replace both prior owners with newly allocated empty Arrays. */
	public function reset():Void {
		values = [];
		labels = [];
	}

	/**
	 * Replace the numeric field with a fresh call result.
	 *
	 * `copy()` returns one new owner. Moving it into the field must not add a
	 * redundant retain or release it when this method returns.
	 */
	public function replaceFromCall(source:Array<Int>):Void
		values = source.copy();

	/**
	 * Replace the field after control flow chooses a fresh or borrowed Array.
	 *
	 * The compiler must locate the destination once before the choice, then use
	 * that same field after both branches join.
	 */
	public function replaceConditional(useFresh:Bool, borrowed:Array<Int>):Void
		values = useFresh ? [73] : borrowed;

	/** Store and return one conditionally selected element through the owned field. */
	public function setConditional(index:Int, selectFirst:Bool):Int
		return values[index] = selectFirst ? 77 : 88;

	/** Reassign both fields through aliases to the same containers without invalidating them. */
	public function assignToSelf():Void {
		final sameValues = values;
		final sameLabels = labels;
		values = sameValues;
		labels = sameLabels;
	}
}

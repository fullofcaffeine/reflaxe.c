/**
 * Keeps a branch-local class borrow from escaping through an outer assignment.
 */

/** A value whose automatic storage must end with its branch. */
final class EscapingConditionalValue {
	/** Construct one otherwise empty value. */
	public function new() {}
}

/** Attempts the forbidden assignment to longer-lived static storage. */
final class Main {
	static var escaped:EscapingConditionalValue;

	/** The assignment must fail before target code is emitted. */
	static function main():Void {
		if (true) {
			final value = new EscapingConditionalValue();
			escaped = value;
		}
	}
}

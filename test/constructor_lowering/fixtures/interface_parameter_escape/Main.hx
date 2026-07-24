/** One interface whose value deliberately escapes its proven constructor use. */
interface ScoreSource {
	/** Returns a small implementation-owned score. */
	function score():Int;
}

/** A concrete caller value that would normally use automatic C storage. */
final class FixedScore implements ScoreSource {
	/** Creates a stateless implementation. */
	public function new() {}

	/** Supplies a visible value if the invalid graph ever reached execution. */
	public function score():Int
		return 42;
}

/**
 * Deliberately hides interface retention behind an unproved local alias.
 *
 * The compiler admits a direct first `this.source = source` initialization
 * because it can connect that store to the owner's collector trace. Introducing
 * `alias` breaks that exact proof: a future alias could be reassigned, captured,
 * or stored elsewhere, so this fixture must fail before generated C exists.
 */
final class EscapingScore {
	final source:ScoreSource;

	/** Attempts the unproved alias-to-field escape rejected by this fixture. */
	public function new(source:ScoreSource) {
		final alias = source;
		this.source = alias;
	}

	/** Would dispatch through the escaped value if unsafe output were emitted. */
	public function read():Int
		return source.score();
}

/** Reaches the unsafe constructor so its source diagnostic stays executable. */
final class Main {
	/** Builds the deliberately unproved retained-interface graph. */
	static function main():Void {
		final value = new EscapingScore(new FixedScore());
		while (value.read() != 42) {}
	}
}

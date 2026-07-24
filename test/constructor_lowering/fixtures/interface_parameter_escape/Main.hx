/** One interface whose value deliberately tries to escape a constructor call. */
interface ScoreSource {
	/** Return an implementation-defined score. */
	function score():Int;
}

/**
 * Deliberately invalid storage of a caller-owned interface view.
 *
 * The fixture must fail before C emission because `source.object` could point
 * at automatic storage that dies before this `EscapingScore` value.
 */
final class EscapingScore {
	public var source:ScoreSource;

	/** Attempt to retain the call-bounded interface value in an object field. */
	public function new(source:ScoreSource) {
		this.source = source;
	}
}

/** Reaches the unsafe constructor so the compiler must diagnose it. */
final class Main {
	static function main():Void {
		final value = new EscapingScore(null);
		while (value.source != null) {}
	}
}

/** One interface retained by a longer-lived class instance. */
interface ScoreSource {
	/** Add an implementation-owned base to the caller's seed. */
	function score(seed:Int):Int;
}

/**
 * Concrete score state whose lifetime is hidden behind `ScoreSource`.
 *
 * This class becomes collector-managed because it can inhabit the retained
 * interface field below. Call-only interface fixtures remain stack allocated,
 * proving that retention—not merely `implements`—selects the collector.
 */
final class FixedScore implements ScoreSource {
	var base:Int;

	/** Stores the small piece of state used by later interface dispatch. */
	public function new(base:Int) {
		this.base = base;
	}

	/** Returns the caller's seed plus this implementation's stored base. */
	public function score(seed:Int):Int
		return base + seed;
}

/**
 * Owns one interface value beyond the constructor call that supplied it.
 *
 * Generated C stores the interface's object/table pair by value. The object is
 * collector-owned, and this class's exact trace callback visits `source.object`
 * so later collection cannot leave the dispatch table pointing at dead storage.
 */
final class RetainedScore {
	final source:ScoreSource;

	/**
	 * Retains an interface past this constructor call.
	 *
	 * Unlike a call-only interface argument, this assignment requires the
	 * compiler to keep both this owner and the concrete `source` object alive.
	 */
	public function new(source:ScoreSource) {
		this.source = source;
	}

	/** Dispatches through the retained interface after construction has ended. */
	public function read(seed:Int):Int
		return source.score(seed);
}

/** Exercises delayed interface dispatch after deterministic collection pressure. */
final class Main {
	/** Builds the retained graph without exposing its concrete implementation. */
	static function build():RetainedScore
		return new RetainedScore(new FixedScore(40));

	/** Forces several collector cycles so a missing trace edge fails reliably. */
	static function forceCollectionPressure():Void {
		for (index in 0...40000)
			new FixedScore(index);
	}

	/** Keeps the process alive only when delayed interface dispatch returns 42. */
	static function main():Void {
		final value = build();
		forceCollectionPressure();
		while (value.read(2) != 42) {}
	}
}

/**
 * Describes the one operation used while constructing `ConfiguredScore`.
 *
 * Keeping this contract as an interface proves that the constructor receives
 * the interface value itself: a concrete class pointer would lose the
 * interface-table choice that makes ordinary Haxe dispatch work.
 */
interface ScoreSource {
	/** Compute a score from the implementation's state and the caller's seed. */
	function score(seed:Int):Int;
}

/** Supplies one small concrete implementation without heap allocation. */
final class FixedScore implements ScoreSource {
	var base:Int;

	/** Store the implementation state that later interface dispatch reads. */
	public function new(base:Int) {
		this.base = base;
	}

	/** Add the construction-time seed to the stored base value. */
	public function score(seed:Int):Int
		return base + seed;
}

/**
 * Proves that a constructor can call through an interface-typed parameter.
 *
 * `source` is borrowed for this call only. The constructor stores the computed
 * integer, not the interface value, so the caller-owned `FixedScore` remains
 * alive for the full dispatch without creating a new owner.
 */
final class ConfiguredScore {
	public var value:Int;

	/** Dispatch through `source`, then keep only the resulting plain integer. */
	public function new(source:ScoreSource, seed:Int) {
		value = source.score(seed);
	}
}

/** Executes the interface-parameter contract without runtime output. */
final class Main {
	/** Keep running only if generated behavior differs from the Eval oracle. */
	static function main():Void {
		final source = new FixedScore(40);
		final configured = new ConfiguredScore(source, 2);
		while (configured.value != 42) {}
	}
}

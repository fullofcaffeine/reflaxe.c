/** Reject Float payload equality until its exact hash and edge cases are pinned. */
final class Main {
	static function main():Void {
		final map:Map<FloatKey, Int> = [];
		map.set(Amount(1.5), 1);
	}
}

/** One deliberately unsupported floating-point enum key. */
enum FloatKey {
	Amount(value:Float);
}

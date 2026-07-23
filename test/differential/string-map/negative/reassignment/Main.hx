/**
	Isolates owner-replacing assignment from supported StringMap aliasing.

	The fixture must fail before C emission until assignment has an atomic
	retain/store/release ownership plan.
**/

/**
	Proves the first StringMap slice rejects replacement without losing ownership.

	The compiler already supports shared aliases, but replacing an owning local
	needs one atomic retain/store/release plan. Until that complete operation is
	lowered, haxe.c must reject the assignment instead of emitting a plain C
	pointer store that leaks the old table or releases the new one too early.
**/
final class Main {
	static function mapWith(key:String):Map<String, Bool> {
		final values:Map<String, Bool> = [];
		values.set(key, true);
		return values;
	}

	static function main():Void {
		var first = mapWith("first");
		final second = mapWith("second");
		first = second;
	}
}

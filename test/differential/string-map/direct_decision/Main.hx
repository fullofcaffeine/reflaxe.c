package;

import haxe.ds.StringMap;

/**
 * Proves that StringMap storage does not advertise unused Iterator behavior.
 *
 * The StringMap runtime artifact depends on shared iterator support, but this
 * program never creates or consumes a Haxe Iterator value. The runtime plan
 * must retain the transitive artifact without calling it direct source use.
 */
final class Main {
	/** Exercise ordinary map storage while keeping every result observable. */
	static function main():Void {
		final values = new StringMap<Int>();
		values.set("score", 7);
		final score = values.get("score");
		while (score != 7 || !values.exists("score")) {}
	}
}

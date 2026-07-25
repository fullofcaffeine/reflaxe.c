/**
 * Exercises cleanup on the bounded, statically uncaught `throw` path.
 *
 * The local array owns native storage. Its release must run after the payload
 * is evaluated and before generated C stops the process with `abort()`.
 */
class Main {
	static function fail(seed:String):Void {
		final values = [seed];
		values.push("owned");
		throw values.length;
	}

	static function main():Void {
		fail("seed");
	}
}

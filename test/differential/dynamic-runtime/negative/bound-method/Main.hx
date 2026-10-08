/** An exact class whose method can be invoked directly but not extracted. */
private final class Counter {
	/** Create the stateless fixture receiver. */
	public function new() {}

	/** Return one more than the supplied value. */
	public function bump(value:Int):Int
		return value + 1;
}

/** Rejects a bound method that would require a capturing closure adapter. */
class Main {
	/** Extracting the method must fail before plausible C is emitted. */
	static function main():Void {
		final counter:Dynamic = new Counter();
		final bound:Dynamic = counter.bump;
		Sys.println(bound(6));
	}
}

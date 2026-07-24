/**
 * Exercises constructor defaults at the ordinary Haxe call sites that use them.
 *
 * The fixture keeps omission distinct from an explicitly written `null`, proves
 * a supplied value replaces its declaration default, and observes that a
 * side-effecting supplied value runs exactly once. Haxe requires declaration
 * defaults themselves to be constants, so testing a call there would describe
 * source that the Haxe type checker rejects. The same omission rule is
 * exercised through `super()` so derived construction cannot silently use a
 * different completion path.
 */

/** A small value record used to prove nullable constructor parameters by value. */
private typedef RestorePoint = {
	final tick:Int;
}

/** Counts supplied argument evaluation so duplicate or skipped work is observable. */
private final class DefaultSource {
	public static var calls:Int = 0;

	/** Produce a value while recording exact evaluation. */
	public static function next():Int {
		calls += 1;
		return 6 + calls;
	}
}

/** Stores the settled values that haxe.c must pass to one fixed-arity C function. */
private class DefaultedRecord {
	public var value:Int;
	public var restoredTick:Int;

	/**
	 * Accept either declaration defaults or values written by the caller.
	 *
	 * Haxe.c inserts the typed constant `41` at an omitted call site. The
	 * generated constructor itself therefore receives one ordinary `Int` and
	 * one tagged optional record; it does not test argument presence at runtime.
	 */
	public function new(value:Int = 41, ?restore:RestorePoint) {
		this.value = value;
		this.restoredTick = restore == null ? -1 : restore.tick;
	}
}

/** Proves that an omitted `super()` argument follows the same completion rule. */
private final class DefaultedChild extends DefaultedRecord {
	public function new() {
		super();
	}
}

/** Runs the semantic checks without selecting output or another runtime feature. */
class Main {
	static function main():Void {
		DefaultSource.calls = 0;
		final omitted = new DefaultedRecord();
		final supplied = new DefaultedRecord(DefaultSource.next());
		final explicitNull = new DefaultedRecord(8, null);
		final present = new DefaultedRecord(9, {tick: 12});
		final inherited = new DefaultedChild();
		while (DefaultSource.calls != 1 || omitted.value != 41 || omitted.restoredTick != -1 || supplied.value != 7 || supplied.restoredTick != -1
			|| explicitNull.value != 8 || explicitNull.restoredTick != -1 || present.value != 9 || present.restoredTick != 12 || inherited.value != 41
			|| inherited.restoredTick != -1) {}
	}
}

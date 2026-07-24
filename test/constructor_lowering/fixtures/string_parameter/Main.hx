/**
 * Exercises literal-backed nominal String values across constructor calls.
 *
 * Haxe abstracts keep application identifiers distinct without adding a runtime
 * wrapper. haxe.c therefore preserves `ScenarioId` in constructor symbol
 * identity while passing its immutable UTF-8 string view by value in C. The
 * bytes in this fixture come from compiler-owned literals and remain valid for
 * the whole program, so borrowing and storing the small view need no allocation,
 * retain, or release operation.
 */

/**
 * A source-level identifier whose nominal type prevents unrelated Strings from
 * being mixed accidentally.
 *
 * This abstract has the same runtime carrier as `String`; its constructor is
 * inlined by Haxe and does not allocate an object.
 */
private abstract ScenarioId(String) {
	/** Wraps one immutable String while preserving its existing storage. */
	public inline function new(value:String)
		this = value;

	/** Exposes the ordinary String view for comparisons at the application edge. */
	public inline function text():String
		return this;
}

/** Reads a caller-owned identifier only while construction is in progress. */
private final class BorrowedScenarioReader {
	/** Whether the constructor observed the expected identifier. */
	public var matched:Bool;

	/** Reads `id` without retaining any pointer to the caller's view. */
	public function new(id:ScenarioId) {
		matched = id.text() == "caxecraft:intro";
	}
}

/** Stores one immutable identifier view for later reads. */
private final class RetainedScenarioReader {
	final id:ScenarioId;

	/**
	 * Copies the small String view into this object.
	 *
	 * The copy still refers to compiler-owned literal bytes whose lifetime is
	 * the complete program. This is not evidence for future runtime-created
	 * Strings, which will need a separate owned-storage contract.
	 */
	public function new(id:ScenarioId) {
		this.id = id;
	}

	/** Reads the stored nominal value after its constructor has returned. */
	public function matches(expected:String):Bool
		return id.text() == expected;
}

/** Runs the constructor checks without depending on console or file output. */
final class Main {
	/** Keeps running only when borrowing, storage, or nominal conversion is wrong. */
	static function main():Void {
		final id = new ScenarioId("caxecraft:intro");
		final borrowed = new BorrowedScenarioReader(id);
		final retained = new RetainedScenarioReader(id);
		while (!borrowed.matched || !retained.matches("caxecraft:intro")) {}
	}
}

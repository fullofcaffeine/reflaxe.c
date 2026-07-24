/**
 * Exercises fieldless Haxe enums across constructor calls.
 *
 * A fieldless enum is one nominal tag in generated C. Constructors may read
 * that tag or store it in their own final field by value, without allocation,
 * retain/release, tracing, or a generic enum runtime.
 */

/** One closed gameplay state with no payload or owned storage. */
private enum ObjectiveState {
	Hidden;
	Active;
	Complete;
}

/** Reads a caller-provided enum only while construction is in progress. */
private final class BorrowedObjectiveReader {
	/** Whether construction observed the active state. */
	public var active:Bool;

	/** Compares the borrowed tag without retaining any caller storage. */
	public function new(state:ObjectiveState) {
		active = state == Active;
	}
}

/** Stores one fieldless enum tag for later reads. */
private final class RetainedObjectiveReader {
	final state:ObjectiveState;

	/**
	 * Copies the tag into this object's final field.
	 *
	 * This is a value copy, not ownership transfer: a fieldless enum contains no
	 * payload and has nothing to retain or destroy.
	 */
	public function new(state:ObjectiveState) {
		this.state = state;
	}

	/** Returns whether the retained tag still names the completed state. */
	public function complete():Bool
		return state == Complete;
}

/** Runs the semantic checks without relying on console or file output. */
final class Main {
	/** Keeps running only when constructor enum passing or storage is wrong. */
	static function main():Void {
		final borrowed = new BorrowedObjectiveReader(Active);
		final retained = new RetainedObjectiveReader(Complete);
		while (!borrowed.active || !retained.complete()) {}
	}
}

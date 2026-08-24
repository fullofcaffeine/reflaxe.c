/** An interface whose implementation is deliberately absent from this program. */
interface DormantSource {
	/** Return one implementation-owned value when a real owner exists. */
	function score():Int;
}

/**
	Describes a possible retained interface owner that this program never builds.

	The class remains reachable as a field type, but no constructor call can put a
	`DormantSource` value into the program. Whole-program lowering therefore has no
	concrete dispatch table to retain or validate.
**/
final class DormantOwner {
	final source:DormantSource;

	/** Retain one source in programs that actually construct this owner. */
	public function new(source:DormantSource) {
		this.source = source;
	}

	/** Dispatch only after a real constructor has initialized the field. */
	public function read():Int
		return source.score();
}

/** A live shell whose nullable field makes the dormant owner type reachable. */
final class SessionShell {
	var dormant:Null<DormantOwner> = null;

	/** Start without constructing the optional retained-interface graph. */
	public function new() {}

	/** Prove the live shell does not invent a dormant owner. */
	public function ready():Bool
		return dormant == null;
}

/** Exercises the live shell without constructing any interface implementation. */
final class Main {
	/** Keep the native process alive only when the optional owner stays absent. */
	static function main():Void {
		final shell = new SessionShell();
		while (!shell.ready()) {}
	}
}

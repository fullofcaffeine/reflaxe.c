/** A managed payload that proves closed class arguments keep identity and tracing. */
final class Payload {
	public final value:Int;

	public function new(value:Int) {
		this.value = value;
	}
}

/**
	A generic owner with no target-specific behavior.

	The same unchanged Haxe declaration must produce distinct, closed layouts,
	constructors, and method bodies for primitive and managed payloads.
**/
class Box<T> {
	public var value(default, null):T;

	public function new(value:T) {
		this.value = value;
	}

	public function get():T {
		return value;
	}

	public function replace(value:T):T {
		final previous = this.value;
		this.value = value;
		return previous;
	}
}

/** Runs the focused closed generic-owner semantic proof. */
class Main {
	/** Construct a primitive owner in a separately discovered function body. */
	static function makeInteger():Box<Int> {
		return new Box<Int>(7);
	}

	/** Construct a managed owner in a separately discovered function body. */
	static function makeManaged(value:Payload):Box<Payload> {
		return new Box<Payload>(value);
	}

	static function main():Void {
		// Calls are visited before the factory bodies that prove construction. The
		// reachable graph, rather than source traversal order, owns admission.
		final integer = makeInteger();
		if (integer.get() != 7 || integer.replace(9) != 7 || integer.get() != 9)
			throw "closed primitive owner specialization failed";

		final first = new Payload(11);
		final second = new Payload(13);
		final managed = makeManaged(first);
		if (managed.get() != first || managed.replace(second) != first || managed.get().value != 13)
			throw "closed managed owner specialization failed";
	}
}

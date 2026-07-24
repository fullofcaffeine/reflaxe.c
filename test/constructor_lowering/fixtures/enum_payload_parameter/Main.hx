/**
 * Exercises unmanaged payload enums across constructor calls.
 *
 * The active variant may carry a primitive or a literal-backed nominal String,
 * but no variant owns runtime storage. haxe.c can therefore pass and store the
 * complete tagged value by value while preserving its exact enum identity.
 */

/** A nominal content identifier with the ordinary immutable String carrier. */
private abstract ContentId(String) {
	/** Wraps one literal-backed String without allocating another object. */
	public inline function new(value:String)
		this = value;

	/** Exposes the ordinary String view for an application comparison. */
	public inline function text():String
		return this;
}

/** Representative scenario values with distinct unmanaged payload shapes. */
private enum FlowValue {
	Flag(value:Bool);
	Counter(value:Int);
	State(value:ContentId);
}

/** Reads one active payload only during construction. */
private final class BorrowedFlowReader {
	/** A deterministic result derived from whichever variant was supplied. */
	public var score:Int;

	/** Projects the active payload without retaining the tagged value. */
	public function new(value:FlowValue) {
		switch value {
			case Flag(enabled):
				score = enabled ? 1 : 0;
			case Counter(count):
				score = count;
			case State(id):
				score = id.text() == "caxecraft:ready" ? 7 : -1;
		}
	}
}

/** Stores one complete unmanaged tagged value for later matching. */
private final class RetainedFlowReader {
	final value:FlowValue;

	/**
	 * Copies the tag and active payload into the final field.
	 *
	 * Every admitted payload is a direct value. The String view refers to
	 * compiler-owned literal bytes, so no variant needs retain or destruction.
	 */
	public function new(value:FlowValue) {
		this.value = value;
	}

	/** Projects the stored active payload after construction returns. */
	public function score():Int {
		return switch value {
			case Flag(enabled): enabled ? 1 : 0;
			case Counter(count): count;
			case State(id): id.text() == "caxecraft:ready" ? 7 : -1;
		};
	}
}

/** Runs all payload shapes without console or file output. */
final class Main {
	/** Keeps running only when a tag, payload, or final-field copy is wrong. */
	static function main():Void {
		final flag = new BorrowedFlowReader(Flag(true));
		final counter = new RetainedFlowReader(Counter(41));
		final state = new RetainedFlowReader(State(new ContentId("caxecraft:ready")));
		while (flag.score != 1 || counter.score() != 41 || state.score() != 7) {}
	}
}

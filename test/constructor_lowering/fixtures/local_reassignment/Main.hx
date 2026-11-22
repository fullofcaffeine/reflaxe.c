/**
 * Proves that one mutable direct-class local can be rebuilt in place.
 *
 * The replacement constructor arguments observe the old value first. Each
 * successful replacement then releases the old managed String field and reuses
 * the same function-lifetime object storage across a loop back edge.
 */

/** A direct class with one managed field whose releases make replacement observable. */
final class ReplaceableValue {
	final text:String;

	/** Keep the supplied managed String as this instance's owned state. */
	public function new(text:String, shouldFail:Bool) {
		this.text = text;
		if (shouldFail)
			throw 99;
	}

	/** Return the current text so the next constructor argument can derive from it. */
	public function read():String
		return text;
}

/** Exercises a general constructor and the StringBuf shape found by Caxecraft. */
final class Main {
	/** Keep the failing replacement reachable without invoking it during normal play. */
	static var exerciseFailure:Bool = false;

	/** Rebuild the same local repeatedly while preserving right-side evaluation order. */
	static function rebuildText():String {
		var value = new ReplaceableValue("a", false);
		var index = 0;
		while (index < 3) {
			value = new ReplaceableValue(value.read() + "b", false);
			index++;
		}
		return value.read();
	}

	/** Trigger one failing replacement for the native cleanup probe. */
	static function replacementFailure():Int {
		var value = new ReplaceableValue("old", false);
		value = new ReplaceableValue("new", true);
		return value.read().length;
	}

	/** Reset StringBuf after publishing each completed token. */
	static function finalToken():String {
		var token = new StringBuf();
		token.add("old");
		token = new StringBuf();
		token.add("new");
		return token.toString();
	}

	/** Stop only when Eval and generated native behavior disagree. */
	static function main():Void
		while (rebuildText() != "abbb" || finalToken() != "new" || exerciseFailure && replacementFailure() != 0) {}
}

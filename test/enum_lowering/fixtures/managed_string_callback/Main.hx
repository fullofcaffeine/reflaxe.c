package;

/**
	Prove that a synchronous enum-constructor callback owns its managed payload.

	The callback receives a borrowed nominal String value. Its returned enum must
	retain a separate owner before the caller releases the source local. Reading
	the payload afterward makes that ownership transfer observable without relying
	on generated-C snapshots as the semantic oracle.
**/
/** A nominal identifier that preserves ordinary immutable String behavior. */
abstract TextId(String) {
	public inline function new(value:String)
		this = value;

	/** Expose the immutable text while keeping storage ownership internal. */
	public inline function text():String
		return this;
}

/** A returned tagged value whose active payload owns one managed String. */
enum TextEvent {
	Entered(id:TextId);
}

/** Exercise the same synchronous constructor-callback shape as a real parser. */
class Main {
	/** Build text at runtime so static-literal lifetime cannot satisfy the test. */
	static function runtimeText():String {
		final output = new StringBuf();
		output.add("callback");
		output.addChar(45);
		output.add("owned");
		return output.toString();
	}

	/** Invoke a caller-provided constructor before this stack frame returns. */
	static function applyEvent(id:TextId, constructor:TextId->TextEvent):TextEvent
		return constructor(id);

	/** Observe the payload before the caller's normal scope cleanup begins. */
	static function isExpected(id:TextId):Bool
		return id.text() == "callback-owned";

	/**
		Drop the source owner before observing the callback's returned payload.

		Eval defines the expected value. Native sanitizer execution additionally
		proves that the enum did not retain only a dangling copy of the String.
	**/
	static function callbackResultSurvivesSourceRelease():Bool {
		var source = new TextId(runtimeText());
		final event = applyEvent(source, Entered);
		source = new TextId("replacement");
		final matches = switch event {
			case Entered(id): isExpected(id);
		};
		return matches;
	}

	static function main():Void {
		while (!callbackResultSurvivesSourceRelease()) {}
	}
}

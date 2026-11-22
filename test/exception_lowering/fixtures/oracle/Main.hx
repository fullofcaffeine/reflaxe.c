/**
 * Records the source-language order for typed catches and rethrow.
 *
 * The generated-C fixture must match this Eval-owned result. The first catch is
 * deliberately incompatible with the thrown `Int`, so the next catch handles
 * it and rethrows a new value to the enclosing region.
 */
class Main {
	static final events:Array<String> = [];

	/** Append one observable step without hiding control flow in a helper. */
	static function record(event:String):Void {
		events.push(event);
	}

	/** Exercise first-compatible catch selection and an enclosing rethrow. */
	static function nestedCatchAndRethrow():Void {
		try {
			try {
				record("body");
				throw 7;
			} catch (message:String) {
				record("inner-string:" + message);
			} catch (number:Int) {
				record("inner-int:" + number);
				throw number + 1;
			}
		} catch (number:Int) {
			record("outer-int:" + number);
		}
		record("after");
	}

	/** Print one stable line that native generated code can match exactly. */
	static function main():Void {
		nestedCatchAndRethrow();
		Sys.println(events.join("|"));
	}
}

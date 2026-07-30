/**
 * Exercises hosted output with both borrowed and freshly constructed Strings.
 *
 * The counter makes the fresh expression's one-time evaluation observable.
 * Returning that concatenation also makes the caller own runtime-allocated
 * bytes, so the generated failure path must release them before aborting.
 */
class Main {
	static var evaluationCount = 0;

	/** Print a caller-owned String without retaining or consuming it. */
	static function printValue(value:String):Void {
		Sys.println(value);
	}

	/** Build one fresh String and record exactly how many times this call ran. */
	static function nextValue():String {
		evaluationCount += 1;
		return "runtime-" + Std.string(evaluationCount);
	}

	/** Print borrowed and fresh values, then expose the evaluation count. */
	static function main():Void {
		Sys.println(nextValue());
		printValue("borrowed");
		if (evaluationCount == 1)
			Sys.println("once");
		else
			Sys.println("evaluated more than once");
	}
}

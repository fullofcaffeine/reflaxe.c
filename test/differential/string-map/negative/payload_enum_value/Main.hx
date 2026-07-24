/**
	Proves that direct fieldless-enum storage does not admit tagged payloads.

	A payload enum needs an active-union copy and destruction policy before a
	StringMap may relocate its slots. This fixture keeps that larger lifetime
	boundary fail-closed instead of letting the runtime copy a C union as bytes.
**/

/** One tagged constructor deliberately outside the direct-enum subset. */
private enum PayloadKind {
	Score(value:Int);
}

/** Keeps the unsupported payload-enum diagnostic isolated from valid enums. */
final class Main {
	static function main():Void {
		final values:Map<String, PayloadKind> = [];
		values.set("score", Score(7));
	}
}

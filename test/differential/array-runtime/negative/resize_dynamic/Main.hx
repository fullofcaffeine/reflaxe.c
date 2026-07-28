/**
	Proves that Array growth and a run-time resize length stay fail-closed.

	Even though `length` happens to contain zero here, it is an ordinary local at
	the call boundary. General resize must model both shrinking and target-typed
	default construction before haxe.c may admit that dynamic operation.
**/
final class Main {
	static function main():Void {
		final values = [1, 2];
		final length = 0;
		values.resize(length);
	}
}

/**
	Proves that the bounded Array clear slice does not pretend growth is complete.

	`resize(3)` must create a target-specific default `Int` element. That broader
	default-construction contract remains unsupported, so no C may be emitted.
**/
final class Main {
	static function main():Void {
		final values = [1, 2];
		values.resize(3);
	}
}

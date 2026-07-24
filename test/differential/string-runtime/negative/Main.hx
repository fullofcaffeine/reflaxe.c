/**
	Proves that an unimplemented ordinary String method still fails at its owner.

	The positive fixture admits a precise subset of String construction and
	inspection. This nearby negative case prevents that subset from accidentally
	turning every String method into a guessed runtime or virtual call.
**/
final class Main {
	/** Keep the unsupported receiver and result observable without C-specific code. */
	static function main():Void {
		final value = "not yet".toUpperCase();
		Sys.println(value);
	}
}

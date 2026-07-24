/**
	Proves that an unimplemented `Std.string` source family fails at its owner.

	The positive fixture admits statically typed Boolean conversion only. This
	nearby negative case prevents that narrow fact from silently becoming a
	generic Dynamic conversion, guessed integer formatter, or reflection path.
**/
final class Main {
	/** Preserve the integer source type at the conversion boundary. */
	static function render(value:Int):String
		return Std.string(value);

	/** Keep the unsupported integer source and String result observable. */
	static function main():Void {
		render(42);
	}
}

/**
	Proves that an unimplemented `Std.string` source family fails at its owner.

	The positive fixture admits statically typed Boolean and Int conversion. This
	nearby negative case prevents that narrow fact from silently becoming a
	generic Dynamic conversion, guessed floating formatter, or reflection path.
**/
final class Main {
	/** Preserve the floating source type at the conversion boundary. */
	static function render(value:Float):String
		return Std.string(value);

	/** Keep the unsupported floating source and String result observable. */
	static function main():Void {
		render(42.5);
	}
}

import haxe.io.Bytes;

/**
 * Proves that a Bytes snapshot can cross an ordinary Haxe function return.
 *
 * This fixture deliberately contains no other runtime String constructor.
 * `Bytes.getString` and `Bytes.toString` must therefore be the facts that select
 * the managed String ABI before haxe.c prepares either helper signature.
 */
final class Main {
	/**
	 * Decode all bytes after one prefix byte and return the independent snapshot.
	 *
	 * The returned String may outlive both this stack frame and later mutation of
	 * the Bytes owner, so generated C transfers one managed String owner to the
	 * caller rather than returning a view into mutable storage.
	 */
	static function decodeTail(value:Bytes):String
		return value.getString(1, value.length - 1);

	/** Decode the complete Bytes value through the neighboring convenience API. */
	static function decodeWhole(value:Bytes):String
		return value.toString();

	/** Keep both managed returns live after their source bytes have changed. */
	static function main():Void {
		final source = Bytes.ofString("xNiña 🐈");
		final tail = decodeTail(source);
		final whole = decodeWhole(Bytes.ofString("Haxe → C"));
		source.fill(1, source.length - 1, "X".code);
		while (tail != "Niña 🐈" || whole != "Haxe → C") {}
	}
}

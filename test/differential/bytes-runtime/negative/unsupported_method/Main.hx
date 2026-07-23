import haxe.io.Bytes;

/** The override types the full API, but declarations do not imply C support. */
final class Main {
	static function main():Void {
		final bytes = Bytes.alloc(4);
		while (bytes.getInt32(0) != 0) {}
	}
}

import haxe.io.Bytes;

/** A borrowed parameter needs retain-on-return, which this first slice rejects. */
final class Main {
	static function identity(bytes:Bytes):Bytes {
		return bytes;
	}

	static function main():Void {
		final original = Bytes.alloc(1);
		final bytes = identity(original);
		while (bytes.length != 1) {}
	}
}

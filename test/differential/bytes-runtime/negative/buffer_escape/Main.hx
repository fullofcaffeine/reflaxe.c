import haxe.io.Bytes;

/** Proves that the mutable-text intrinsic cannot become an ordinary Haxe value. */
final class Main {
	static function main():Void {
		final bytes = Bytes.alloc(16);
		final escaped = c.CStringBufferRef.to(bytes);
	}
}

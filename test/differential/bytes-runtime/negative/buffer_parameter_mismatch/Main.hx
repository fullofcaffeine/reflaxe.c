import haxe.io.Bytes;

/**
 * Proves that the mutable carrier cannot satisfy a read-only C-string parameter.
 *
 * Haxe's type checker should reject this before haxe.c can select an ABI or
 * generate output. The fixture deliberately uses no cast or raw target code.
 */
final class Main {
	static function main():Void {
		final bytes = Bytes.alloc(16);
		MutableTextApi.replace(c.CStringBufferRef.to(bytes), bytes.length);
	}
}

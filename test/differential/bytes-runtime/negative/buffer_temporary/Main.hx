import haxe.io.Bytes;

/** Proves that a fresh temporary cannot own storage for a native text borrow. */
final class Main {
	static function main():Void {
		MutableTextApi.replace(c.CStringBufferRef.to(Bytes.alloc(16)), 16);
	}
}

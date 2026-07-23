import haxe.io.Bytes;

/** An owned Bytes result cannot be borrowed by another call until it has an owner. */
final class Main {
	static function main():Void {
		final bytes = Bytes.alloc(4);
		while (bytes.compare(bytes.sub(0, 4)) != 0) {}
	}
}

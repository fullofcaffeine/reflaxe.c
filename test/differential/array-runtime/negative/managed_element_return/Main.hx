import haxe.io.Bytes;

/** A copied managed Array element cannot escape without transferring ownership. */
final class Main {
	static function take(values:Array<ManagedEntry>):ManagedEntry
		return values[0];

	static function main():Void {
		final bytes = Bytes.alloc(1);
		final values:Array<ManagedEntry> = [];
		values.push({bytes: bytes});
		final copy = take(values);
		while (copy.bytes.length != 1) {}
	}
}

private typedef ManagedEntry = {
	final bytes:Bytes;
}

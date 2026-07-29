/** Proves that Haxe rejects non-Bytes owners before target lowering begins. */
final class Main {
	static function main():Void {
		final value = 16;
		MutableTextApi.replace(c.CStringBufferRef.to(value), 16);
	}
}

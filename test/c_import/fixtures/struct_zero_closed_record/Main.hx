/**
 * Proves structural zero initialization cannot be used as an untyped shortcut
 * for ordinary Haxe records. Those records keep their normal Haxe construction
 * rules; `c.StructInit.zero()` is only for complete header-owned C structs.
 */
typedef OrdinaryRecord = {
	final value:Int;
}

/** Attempts the deliberately unsupported ordinary-record zero construction. */
class Main {
	static function main():Void {
		final value:OrdinaryRecord = c.StructInit.zero();
		if (value.value == 0) {}
	}
}

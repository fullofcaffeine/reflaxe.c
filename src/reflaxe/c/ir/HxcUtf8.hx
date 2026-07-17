package reflaxe.c.ir;

/** Target-owned validation for immutable Unicode-scalar UTF-8 String text. */
class HxcUtf8 {
	/** Returns null for surrogate or out-of-range scalar input. */
	public static function byteLength(value:String):Null<Int> {
		var byteLength = 0;
		var index = 0;
		while (index < value.length) {
			final first = value.charCodeAt(index++);
			if (first == null) {
				return null;
			}
			var scalar = first;
			if (first >= 0xD800 && first <= 0xDBFF) {
				if (index >= value.length) {
					return null;
				}
				final second = value.charCodeAt(index++);
				if (second == null || second < 0xDC00 || second > 0xDFFF) {
					return null;
				}
				scalar = 0x10000 + ((first - 0xD800) << 10) + (second - 0xDC00);
			} else if (first >= 0xDC00 && first <= 0xDFFF || first > 0x10FFFF) {
				return null;
			}
			byteLength += scalar <= 0x7F ? 1 : scalar <= 0x7FF ? 2 : scalar <= 0xFFFF ? 3 : 4;
		}
		return byteLength;
	}
}

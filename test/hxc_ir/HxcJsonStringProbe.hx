import reflaxe.c.ir.HxcJsonString;

/** Protect canonical IR string bytes, including characters excluded from the fast path. */
class HxcJsonStringProbe {
	/** Check every ASCII character and boundary combinations against explicit JSON spellings. */
	static function main():Void {
		check("", '""');
		for (code in 0...128) {
			final value = String.fromCharCode(code);
			final escaped = switch code {
				case 8: "\\b";
				case 9: "\\t";
				case 10: "\\n";
				case 12: "\\f";
				case 13: "\\r";
				case 34: "\\\"";
				case 92: "\\\\";
				case value if (value < 32): "\\u" + StringTools.hex(value, 4);
				case _: value;
			};
			check(value, '"' + escaped + '"');
			check("safe" + value, '"safe' + escaped + '"');
			check(value + "safe", '"' + escaped + 'safe"');
		}
		check("á雪", '"á雪"');
		check("path/with spaces:1-2", '"path/with spaces:1-2"');
		#if js
		check("😀", '"\\uD83D\\uDE00"');
		check(String.fromCharCode(0xD800), '"\\uD800"');
		#elseif eval
		check("😀", '"😀"');
		#end
		haxe.Log.trace("HXC_JSON_STRING_OK", null);
	}

	/** Exact equality protects cache keys from otherwise valid alternative JSON encodings. */
	static function check(value:String, expected:String):Void {
		final actual = HxcJsonString.quote(value);
		if (actual != expected)
			throw new haxe.Exception('canonical JSON string mismatch: expected $expected, got $actual');
	}
}

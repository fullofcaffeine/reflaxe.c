package caxecraft.content;

/**
 * Writes the small JSON strings used by typed content-manifest writers.
 *
 * Each manifest writer controls its own closed field order. This helper owns
 * only JSON string escaping, which keeps identifiers and future Unicode labels
 * valid without giving an untyped JSON value authority over the output.
 */
/** Append one JSON string, including quotes, to an existing output buffer. */
function appendJsonString(output:StringBuf, value:String):Void {
	output.add('"');
	for (index in 0...value.length) {
		final code = value.charCodeAt(index);
		if (code == 0x22)
			output.add('\\"');
		else if (code == 0x5c)
			output.add('\\\\');
		else if (code == 0x08)
			output.add('\\b');
		else if (code == 0x0c)
			output.add('\\f');
		else if (code == 0x0a)
			output.add('\\n');
		else if (code == 0x0d)
			output.add('\\r');
		else if (code == 0x09)
			output.add('\\t');
		else if (code < 0x20) {
			output.add('\\u00');
			appendHexByte(output, code);
		} else
			output.addChar(code);
	}
	output.add('"');
}

/** Append one byte as exactly two lowercase hexadecimal digits. */
private function appendHexByte(output:StringBuf, value:Int):Void {
	final digits = "0123456789abcdef";
	output.addChar(digits.charCodeAt(value >>> 4));
	output.addChar(digits.charCodeAt(value & 15));
}

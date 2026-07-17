package reflaxe.c.ir;

/** Deterministic JSON-string spelling for HxcIR dumps and sort keys. */
class HxcJsonString {
	public static function quote(value:String):String {
		final output = new StringBuf();
		output.add('"');
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null) {
				output.add("\\uFFFD");
				continue;
			}
			switch code {
				case 0x08:
					output.add("\\b");
				case 0x09:
					output.add("\\t");
				case 0x0A:
					output.add("\\n");
				case 0x0C:
					output.add("\\f");
				case 0x0D:
					output.add("\\r");
				case 0x22:
					output.add("\\\"");
				case 0x5C:
					output.add("\\\\");
				case value if (value < 0x20 || (value >= 0xD800 && value <= 0xDFFF)):
					output.add("\\u" + hex4(value));
				case _:
					output.add(value.charAt(index));
			}
		}
		output.add('"');
		return output.toString();
	}

	static function hex4(value:Int):String {
		final digits = "0123456789ABCDEF";
		return digits.charAt((value >> 12) & 0xF)
			+ digits.charAt((value >> 8) & 0xF)
			+ digits.charAt((value >> 4) & 0xF)
			+ digits.charAt(value & 0xF);
	}
}

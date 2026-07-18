package hxc.config;

import hxc.config.HxcJsonValue.HxcJsonField;
import hxc.config.HxcJsonValue.HxcJsonNode;
import reflaxe.c.ir.HxcUtf8;

/**
	Small complete JSON parser for the untrusted `hxc.json` ingress.

	It retains object fields as ordered typed values so duplicate and unknown
	keys can fail before an untyped value reaches configuration semantics.
**/
class HxcJsonParser {
	final text:String;
	final source:String;
	var index:Int = 0;
	var line:Int = 1;
	var column:Int = 1;

	public function new(text:String, source:String = "hxc.json") {
		if (HxcUtf8.byteLength(text) == null) {
			throw new HxcConfigError("configuration is not valid Unicode-scalar UTF-8", source);
		}
		this.text = text;
		this.source = source;
	}

	public function parse():HxcJsonNode {
		if (peekCode() == 0xFEFF) {
			fail("a UTF-8 BOM is not admitted; save the project file as plain UTF-8");
		}
		skipWhitespace();
		final value = parseValue();
		skipWhitespace();
		if (!atEnd()) {
			fail("unexpected content after the root JSON value");
		}
		return value;
	}

	function parseValue():HxcJsonNode {
		final startLine = line;
		final startColumn = column;
		return switch peekCode() {
			case 0x7B:
				new HxcJsonNode(JObject(parseObject()), startLine, startColumn);
			case 0x5B:
				new HxcJsonNode(JArray(parseArray()), startLine, startColumn);
			case 0x22:
				new HxcJsonNode(JString(parseString()), startLine, startColumn);
			case 0x74:
				expectKeyword("true");
				new HxcJsonNode(JBool(true), startLine, startColumn);
			case 0x66:
				expectKeyword("false");
				new HxcJsonNode(JBool(false), startLine, startColumn);
			case 0x6E:
				expectKeyword("null");
				new HxcJsonNode(JNull, startLine, startColumn);
			case value if (value == 0x2D || value != null && value >= 0x30 && value <= 0x39):
				new HxcJsonNode(JNumber(parseNumber()), startLine, startColumn);
			case null:
				fail("expected a JSON value, found end of file");
			case _:
				fail("expected an object, array, string, number, boolean, or null");
		};
	}

	function parseObject():Array<HxcJsonField> {
		expectCode(0x7B, "expected `{`");
		skipWhitespace();
		final fields:Array<HxcJsonField> = [];
		final names:Map<String, Bool> = [];
		if (consumeCode(0x7D)) {
			return fields;
		}
		while (true) {
			if (peekCode() != 0x22) {
				fail("object field names must be JSON strings");
			}
			final fieldLine = line;
			final fieldColumn = column;
			final name = parseString();
			if (names.exists(name)) {
				throw new HxcConfigError('duplicate JSON field `$name`', source, fieldLine, fieldColumn);
			}
			names.set(name, true);
			skipWhitespace();
			expectCode(0x3A, 'expected `:` after field `$name`');
			skipWhitespace();
			fields.push(new HxcJsonField(name, parseValue(), fieldLine, fieldColumn));
			skipWhitespace();
			if (consumeCode(0x7D)) {
				return fields;
			}
			expectCode(0x2C, "expected `,` or `}` after object field");
			skipWhitespace();
			if (peekCode() == 0x7D) {
				fail("trailing commas are not valid JSON");
			}
		}
		return fields;
	}

	function parseArray():Array<HxcJsonNode> {
		expectCode(0x5B, "expected `[`");
		skipWhitespace();
		final values:Array<HxcJsonNode> = [];
		if (consumeCode(0x5D)) {
			return values;
		}
		while (true) {
			values.push(parseValue());
			skipWhitespace();
			if (consumeCode(0x5D)) {
				return values;
			}
			expectCode(0x2C, "expected `,` or `]` after array item");
			skipWhitespace();
			if (peekCode() == 0x5D) {
				fail("trailing commas are not valid JSON");
			}
		}
		return values;
	}

	function parseString():String {
		expectCode(0x22, "expected string opening quote");
		final output = new StringBuf();
		while (!atEnd()) {
			final code = takeCode();
			switch code {
				case 0x22:
					return output.toString();
				case 0x5C:
					parseEscape(output);
				case value if (value < 0x20):
					fail("unescaped control character in JSON string");
				case value:
					output.add(String.fromCharCode(value));
			}
		}
		return fail("unterminated JSON string");
	}

	function parseEscape(output:StringBuf):Void {
		if (atEnd()) {
			fail("unterminated JSON escape");
		}
		switch takeCode() {
			case 0x22:
				output.add('"');
			case 0x5C:
				output.add("\\");
			case 0x2F:
				output.add("/");
			case 0x62:
				output.addChar(0x08);
			case 0x66:
				output.addChar(0x0C);
			case 0x6E:
				output.addChar(0x0A);
			case 0x72:
				output.addChar(0x0D);
			case 0x74:
				output.addChar(0x09);
			case 0x75:
				final first = parseHex4();
				if (first >= 0xD800 && first <= 0xDBFF) {
					if (!consumeCode(0x5C) || !consumeCode(0x75)) {
						fail("high surrogate must be followed by a low-surrogate Unicode escape");
					}
					final second = parseHex4();
					if (second < 0xDC00 || second > 0xDFFF) {
						fail("high surrogate must be followed by a low surrogate");
					}
					output.add(String.fromCharCode(0x10000 + ((first - 0xD800) << 10) + second - 0xDC00));
				} else if (first >= 0xDC00 && first <= 0xDFFF) {
					fail("unpaired low surrogate in Unicode escape");
				} else {
					output.add(String.fromCharCode(first));
				}
			case _:
				fail("unknown JSON string escape");
		}
	}

	function parseHex4():Int {
		var value = 0;
		for (_ in 0...4) {
			if (atEnd()) {
				fail("incomplete four-digit Unicode escape");
			}
			final digit = takeCode();
			final nibble = if (digit >= 0x30 && digit <= 0x39) digit - 0x30 else if (digit >= 0x41 && digit <= 0x46) digit - 0x41 + 10 else if (digit >= 0x61
				&& digit <= 0x66) digit
				- 0x61 + 10 else -1;
			if (nibble < 0) {
				fail("Unicode escape contains a non-hexadecimal digit");
			}
			value = (value << 4) | nibble;
		}
		return value;
	}

	function parseNumber():String {
		final start = index;
		consumeCode(0x2D);
		if (consumeCode(0x30)) {
			if (isDigit(peekCode())) {
				fail("JSON numbers cannot contain a leading zero");
			}
		} else {
			requireDigits("number requires an integer part");
		}
		if (consumeCode(0x2E)) {
			requireDigits("number requires digits after the decimal point");
		}
		final exponent = peekCode();
		if (exponent == 0x65 || exponent == 0x45) {
			takeCode();
			final sign = peekCode();
			if (sign == 0x2B || sign == 0x2D) {
				takeCode();
			}
			requireDigits("number requires exponent digits");
		}
		return text.substring(start, index);
	}

	function requireDigits(message:String):Void {
		if (!isDigit(peekCode())) {
			fail(message);
		}
		while (isDigit(peekCode())) {
			takeCode();
		}
	}

	function expectKeyword(keyword:String):Void {
		for (character in 0...keyword.length) {
			if (takeCode() != keyword.charCodeAt(character)) {
				fail('expected JSON keyword `$keyword`');
			}
		}
	}

	function skipWhitespace():Void {
		while (true) {
			switch peekCode() {
				case 0x20 | 0x09 | 0x0A | 0x0D:
					takeCode();
				case _:
					return;
			}
		}
	}

	function expectCode(expected:Int, message:String):Void {
		if (!consumeCode(expected)) {
			fail(message);
		}
	}

	function consumeCode(expected:Int):Bool {
		if (peekCode() != expected) {
			return false;
		}
		takeCode();
		return true;
	}

	function takeCode():Int {
		final value = peekCode();
		if (value == null) {
			fail("unexpected end of file");
		}
		index++;
		if (value == 0x0A) {
			line++;
			column = 1;
		} else {
			column++;
		}
		return value;
	}

	function peekCode():Null<Int>
		return index >= text.length ? null : text.charCodeAt(index);

	function atEnd():Bool
		return index >= text.length;

	static function isDigit(value:Null<Int>):Bool
		return value != null && value >= 0x30 && value <= 0x39;

	function fail<T>(detail:String):T
		throw new HxcConfigError(detail, source, line, column);
}

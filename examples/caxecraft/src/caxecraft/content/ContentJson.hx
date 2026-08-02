package caxecraft.content;

import caxecraft.text.Utf8Decoder;
import caxecraft.text.Utf8Decoder.Utf8DecodeResult;
import haxe.io.Bytes;

/**
 * Parses bounded package JSON into a closed, location-bearing value tree.
 *
 * Haxe's general JSON API returns `Dynamic`, which would let unchecked shapes
 * flow into gameplay. This module instead preserves every admitted JSON kind
 * in a closed enum. Pack and UI decoders can then reject missing, unknown, or
 * wrongly typed fields before constructing a registry or catalog.
 */
/**
 * Closed JSON kinds admitted at the package boundary.
 *
 * Numbers retain their source spelling. A schema decoder must explicitly admit
 * an integer or another numeric form instead of inheriting target-specific
 * floating-point conversion from a general JSON library.
 */
enum ContentJsonValue {
	JsonNull;
	JsonBoolean(value:Bool);
	JsonNumber(lexeme:String);
	JsonString(value:String);
	JsonArray(values:Array<ContentJsonNode>);
	JsonObject(fields:Array<ContentJsonField>);
}

/**
 * One JSON value and the location where it began.
 *
 * A class gives the recursive tree a stable managed identity while it is being
 * validated. The tree is request-local and cannot become a content registry by
 * itself; a successful typed schema decoder copies only admitted facts.
 */
final class ContentJsonNode {
	/** Closed value stored at this source location. */
	public final value:ContentJsonValue;

	/** One-based source line. */
	public final line:Int;

	/** One-based Unicode-string column. */
	public final column:Int;

	/** Construct one parser-owned immutable node. */
	public function new(value:ContentJsonValue, line:Int, column:Int) {
		this.value = value;
		this.line = line;
		this.column = column;
	}
}

/** One ordered object field and the location of its JSON name. */
final class ContentJsonField {
	/** Exact decoded field name. */
	public final name:String;

	/** Parsed field value. */
	public final value:ContentJsonNode;

	/** One-based source line of the field name. */
	public final line:Int;

	/** One-based source column of the field name. */
	public final column:Int;

	/** Construct one immutable, parser-owned object field. */
	public function new(name:String, value:ContentJsonNode, line:Int, column:Int) {
		this.name = name;
		this.value = value;
		this.line = line;
		this.column = column;
	}
}

/** Exact bounded syntax reason that prevented a JSON tree from escaping. */
enum ContentJsonErrorKind {
	/** Input exceeded the package JSON byte limit before decoding. */
	JsonFileTooLarge(maximumBytes:Int);

	/** External bytes were not admitted scalar-valid UTF-8. */
	JsonMalformedUtf8(byteOffset:Int);

	/** A value nested deeper than the parser's reviewed bound. */
	JsonDepthExceeded(maximumDepth:Int);

	/** The complete document contained too many values. */
	JsonNodeLimitExceeded(maximumNodes:Int);

	/** One array or object exceeded its reviewed entry bound. */
	JsonCollectionLimitExceeded(maximumEntries:Int);

	/** A JSON value was required at this location. */
	JsonExpectedValue;

	/** The document ended while a token was incomplete. */
	JsonUnexpectedEnd;

	/** A token began with a character not admitted by JSON. */
	JsonUnexpectedCharacter;

	/** The root value ended but additional non-whitespace text remained. */
	JsonTrailingContent;

	/** One object repeated a field name instead of silently replacing it. */
	JsonDuplicateField(name:String);

	/** A string contained an unterminated body or raw control character. */
	JsonInvalidString;

	/** A backslash escape or Unicode surrogate pair was malformed. */
	JsonInvalidEscape;

	/** A number did not use JSON's exact decimal grammar. */
	JsonInvalidNumber;

	/** An object or array separator was missing or trailing. */
	JsonInvalidSeparator;
}

/** Stable source coordinate and reason for one rejected JSON document. */
typedef ContentJsonDiagnostic = {
	/** One-based line, or one for byte-decoding failures. */
	final line:Int;

	/** One-based column, or one for byte-decoding failures. */
	final column:Int;

	/** Closed reason suitable for deterministic Eval/native comparison. */
	final kind:ContentJsonErrorKind;
}

/** Complete typed JSON tree or one fail-closed diagnostic. */
enum ContentJsonReadResult {
	/** Parsing consumed exactly one complete bounded JSON value. */
	ContentJsonReady(root:ContentJsonNode);

	/** No partial tree escaped the parser. */
	ContentJsonRejected(diagnostic:ContentJsonDiagnostic);
}

/**
 * Stateful cursor for one JSON document.
 *
 * A class is appropriate here because index, line, column, node count, and the
 * first failure change together during one parse. The public `read` entry point
 * creates a fresh cursor for every request, so no state survives a content
 * reload.
 */
final class ContentJson {
	/**
	 * Largest JSON source admitted by this bounded content schema.
	 *
	 * Twenty-four KiB admits the reviewed 17.2 KiB asset inventory while keeping
	 * untrusted syntax and allocation work bounded. The focused asset/export
	 * tracer supplies representative generated-C timing and sanitizer evidence.
	 */
	public static inline final MAXIMUM_BYTES:Int = 24 * 1024;

	/** Deep enough for the current schemas while bounding recursive work. */
	public static inline final MAXIMUM_DEPTH:Int = 24;

	/**
	 * Whole-document value bound, independent of source byte length.
	 *
	 * This leaves several times the current pack's headroom while keeping a
	 * limit-edge parse inside the native feedback budget. Raise it only with a
	 * representative generated-C timing and memory measurement.
	 */
	public static inline final MAXIMUM_NODES:Int = 1024;

	/** Per-object and per-array entry bound with room above shipped catalogs. */
	public static inline final MAXIMUM_COLLECTION_ENTRIES:Int = 128;

	final text:String;
	var index:Int = 0;
	var line:Int = 1;
	var column:Int = 1;
	var nodes:Int = 0;
	var failure:Null<ContentJsonDiagnostic> = null;

	private function new(text:String) {
		this.text = text;
	}

	/**
	 * Decode and parse one complete package JSON document.
	 *
	 * The input is borrowed for this synchronous call. Success owns a new typed
	 * tree; rejection owns only a scalar/string diagnostic and exposes no partial
	 * parser state.
	 */
	public static function read(input:Bytes):ContentJsonReadResult {
		if (input.length > MAXIMUM_BYTES)
			return ContentJsonRejected({line: 1, column: 1, kind: JsonFileTooLarge(MAXIMUM_BYTES)});
		final text = switch Utf8Decoder.decode(input, MAXIMUM_BYTES) {
			case Utf8Decoded(value): value;
			case Utf8Rejected(byteOffset):
				return ContentJsonRejected({line: 1, column: 1, kind: JsonMalformedUtf8(byteOffset)});
		};
		return new ContentJson(text).parse();
	}

	/** Parse one root and reject any trailing non-whitespace text. */
	function parse():ContentJsonReadResult {
		skipWhitespace();
		final root = parseValue(0);
		if (failure != null)
			return ContentJsonRejected(failure);
		if (root == null) {
			setFailure(JsonExpectedValue);
			return ContentJsonRejected(failure);
		}
		skipWhitespace();
		if (!atEnd()) {
			setFailure(JsonTrailingContent);
			return ContentJsonRejected(failure);
		}
		return ContentJsonReady(root);
	}

	/** Parse the value beginning at the current cursor. */
	function parseValue(depth:Int):Null<ContentJsonNode> {
		if (depth > MAXIMUM_DEPTH) {
			setFailure(JsonDepthExceeded(MAXIMUM_DEPTH));
			return null;
		}
		nodes++;
		if (nodes > MAXIMUM_NODES) {
			setFailure(JsonNodeLimitExceeded(MAXIMUM_NODES));
			return null;
		}
		final startLine = line;
		final startColumn = column;
		final code = peekCode();
		if (code == null) {
			setFailure(JsonExpectedValue);
			return null;
		}
		if (code == 0x7b) {
			final fields = parseObject(depth + 1);
			return fields == null ? null : new ContentJsonNode(JsonObject(fields), startLine, startColumn);
		}
		if (code == 0x5b) {
			final values = parseArray(depth + 1);
			return values == null ? null : new ContentJsonNode(JsonArray(values), startLine, startColumn);
		}
		if (code == 0x22) {
			final value = parseString();
			return value == null ? null : new ContentJsonNode(JsonString(value), startLine, startColumn);
		}
		if (code == 0x74)
			return expectKeyword("true") ? new ContentJsonNode(JsonBoolean(true), startLine, startColumn) : null;
		if (code == 0x66)
			return expectKeyword("false") ? new ContentJsonNode(JsonBoolean(false), startLine, startColumn) : null;
		if (code == 0x6e)
			return expectKeyword("null") ? new ContentJsonNode(JsonNull, startLine, startColumn) : null;
		if (code == 0x2d || isDigit(code)) {
			final lexeme = parseNumber();
			return lexeme == null ? null : new ContentJsonNode(JsonNumber(lexeme), startLine, startColumn);
		}
		setFailure(JsonUnexpectedCharacter);
		return null;
	}

	/** Parse an object while preserving source order and rejecting duplicates. */
	function parseObject(depth:Int):Null<Array<ContentJsonField>> {
		takeCode();
		skipWhitespace();
		final fields:Array<ContentJsonField> = [];
		if (consumeCode(0x7d))
			return fields;
		while (failure == null) {
			if (peekCode() != 0x22) {
				setFailure(JsonInvalidString);
				return null;
			}
			final fieldLine = line;
			final fieldColumn = column;
			final name = parseString();
			if (name == null)
				return null;
			for (field in fields)
				if (field.name == name) {
					setFailureAt(fieldLine, fieldColumn, JsonDuplicateField(name));
					return null;
				}
			skipWhitespace();
			if (!consumeCode(0x3a)) {
				setFailure(JsonInvalidSeparator);
				return null;
			}
			skipWhitespace();
			final value = parseValue(depth);
			if (value == null)
				return null;
			fields.push(new ContentJsonField(name, value, fieldLine, fieldColumn));
			if (fields.length > MAXIMUM_COLLECTION_ENTRIES) {
				setFailure(JsonCollectionLimitExceeded(MAXIMUM_COLLECTION_ENTRIES));
				return null;
			}
			skipWhitespace();
			if (consumeCode(0x7d))
				return fields;
			if (!consumeCode(0x2c)) {
				setFailure(JsonInvalidSeparator);
				return null;
			}
			skipWhitespace();
			if (peekCode() == 0x7d) {
				setFailure(JsonInvalidSeparator);
				return null;
			}
		}
		return null;
	}

	/** Parse an array and bound both recursion and per-container work. */
	function parseArray(depth:Int):Null<Array<ContentJsonNode>> {
		takeCode();
		skipWhitespace();
		final values:Array<ContentJsonNode> = [];
		if (consumeCode(0x5d))
			return values;
		while (failure == null) {
			final value = parseValue(depth);
			if (value == null)
				return null;
			values.push(value);
			if (values.length > MAXIMUM_COLLECTION_ENTRIES) {
				setFailure(JsonCollectionLimitExceeded(MAXIMUM_COLLECTION_ENTRIES));
				return null;
			}
			skipWhitespace();
			if (consumeCode(0x5d))
				return values;
			if (!consumeCode(0x2c)) {
				setFailure(JsonInvalidSeparator);
				return null;
			}
			skipWhitespace();
			if (peekCode() == 0x5d) {
				setFailure(JsonInvalidSeparator);
				return null;
			}
		}
		return null;
	}

	/** Parse a JSON string including exact Unicode escape pairing. */
	function parseString():Null<String> {
		if (!consumeCode(0x22)) {
			setFailure(JsonInvalidString);
			return null;
		}
		final output = new StringBuf();
		while (!atEnd()) {
			final code = takeCode();
			if (code == 0x22)
				return output.toString();
			if (code == 0x5c) {
				if (!parseEscape(output))
					return null;
			} else if (code < 0x20) {
				setFailure(JsonInvalidString);
				return null;
			} else {
				output.addChar(code);
			}
		}
		setFailure(JsonUnexpectedEnd);
		return null;
	}

	/** Append one validated JSON escape to the active string buffer. */
	function parseEscape(output:StringBuf):Bool {
		if (atEnd()) {
			setFailure(JsonUnexpectedEnd);
			return false;
		}
		final escape = takeCode();
		if (escape == 0x22) {
			output.add('"');
			return true;
		}
		if (escape == 0x5c) {
			output.add("\\");
			return true;
		}
		if (escape == 0x2f) {
			output.add("/");
			return true;
		}
		if (escape == 0x62) {
			output.addChar(0x08);
			return true;
		}
		if (escape == 0x66) {
			output.addChar(0x0c);
			return true;
		}
		if (escape == 0x6e) {
			output.addChar(0x0a);
			return true;
		}
		if (escape == 0x72) {
			output.addChar(0x0d);
			return true;
		}
		if (escape == 0x74) {
			output.addChar(0x09);
			return true;
		}
		if (escape != 0x75) {
			setFailure(JsonInvalidEscape);
			return false;
		}
		final first = parseHex4();
		if (first < 0)
			return false;
		if (first >= 0xd800 && first <= 0xdbff) {
			if (!consumeCode(0x5c) || !consumeCode(0x75)) {
				setFailure(JsonInvalidEscape);
				return false;
			}
			final second = parseHex4();
			if (second < 0xdc00 || second > 0xdfff) {
				setFailure(JsonInvalidEscape);
				return false;
			}
			output.addChar(0x10000 + ((first - 0xd800) << 10) + second - 0xdc00);
			return true;
		}
		if (first >= 0xdc00 && first <= 0xdfff) {
			setFailure(JsonInvalidEscape);
			return false;
		}
		output.addChar(first);
		return true;
	}

	/** Parse exactly four hexadecimal digits, or return -1 after recording failure. */
	function parseHex4():Int {
		var value = 0;
		for (_ in 0...4) {
			if (atEnd()) {
				setFailure(JsonUnexpectedEnd);
				return -1;
			}
			final code = takeCode();
			final nibble = hexDigit(code);
			if (nibble < 0) {
				setFailure(JsonInvalidEscape);
				return -1;
			}
			value = (value << 4) | nibble;
		}
		return value;
	}

	/** Parse JSON's decimal grammar while retaining the source lexeme. */
	function parseNumber():Null<String> {
		final start = index;
		consumeCode(0x2d);
		if (consumeCode(0x30)) {
			if (isDigit(peekCode())) {
				setFailure(JsonInvalidNumber);
				return null;
			}
		} else if (!consumeDigits()) {
			setFailure(JsonInvalidNumber);
			return null;
		}
		if (consumeCode(0x2e) && !consumeDigits()) {
			setFailure(JsonInvalidNumber);
			return null;
		}
		final exponent = peekCode();
		if (exponent == 0x65 || exponent == 0x45) {
			takeCode();
			final sign = peekCode();
			if (sign == 0x2b || sign == 0x2d)
				takeCode();
			if (!consumeDigits()) {
				setFailure(JsonInvalidNumber);
				return null;
			}
		}
		return text.substring(start, index);
	}

	/** Consume at least one ASCII decimal digit. */
	function consumeDigits():Bool {
		if (!isDigit(peekCode()))
			return false;
		while (isDigit(peekCode()))
			takeCode();
		return true;
	}

	/** Consume one exact JSON keyword. */
	function expectKeyword(keyword:String):Bool {
		for (character in 0...keyword.length) {
			if (atEnd()) {
				setFailure(JsonUnexpectedEnd);
				return false;
			}
			if (takeCode() != keyword.charCodeAt(character)) {
				setFailure(JsonUnexpectedCharacter);
				return false;
			}
		}
		return true;
	}

	/** Skip only JSON's four ASCII whitespace characters. */
	function skipWhitespace():Void {
		while (true) {
			final code = peekCode();
			if (code == 0x20 || code == 0x09 || code == 0x0a || code == 0x0d)
				takeCode();
			else
				return;
		}
	}

	/** Consume one expected code point without recording a failure. */
	function consumeCode(expected:Int):Bool {
		if (peekCode() != expected)
			return false;
		takeCode();
		return true;
	}

	/** Consume one code point and maintain one-based source coordinates. */
	function takeCode():Int {
		final value = peekCode();
		if (value == null) {
			setFailure(JsonUnexpectedEnd);
			return -1;
		}
		index++;
		if (value == 0x0d) {
			line++;
			column = 1;
		} else if (value == 0x0a) {
			// JSON admits CR, LF, and CRLF whitespace. A CRLF pair is one
			// source line even though the cursor consumes two code points.
			if (index < 2 || text.charCodeAt(index - 2) != 0x0d)
				line++;
			column = 1;
		} else {
			column++;
		}
		return value;
	}

	/** Current code point, or null after the last character. */
	inline function peekCode():Null<Int>
		return index >= text.length ? null : text.charCodeAt(index);

	/** True after the final decoded character. */
	inline function atEnd():Bool
		return index >= text.length;

	/** Record only the first syntax failure so diagnostics stay deterministic. */
	function setFailure(kind:ContentJsonErrorKind):Void {
		if (failure == null)
			failure = {line: line, column: column, kind: kind};
	}

	/** Record a failure whose source token began before the current cursor. */
	function setFailureAt(sourceLine:Int, sourceColumn:Int, kind:ContentJsonErrorKind):Void {
		if (failure == null)
			failure = {line: sourceLine, column: sourceColumn, kind: kind};
	}

	/** Map one ASCII hexadecimal digit to its numeric value. */
	static function hexDigit(code:Int):Int {
		if (code >= 0x30 && code <= 0x39)
			return code - 0x30;
		if (code >= 0x41 && code <= 0x46)
			return code - 0x41 + 10;
		if (code >= 0x61 && code <= 0x66)
			return code - 0x61 + 10;
		return -1;
	}

	/** True only for an ASCII decimal digit. */
	static inline function isDigit(code:Null<Int>):Bool
		return code != null && code >= 0x30 && code <= 0x39;
}

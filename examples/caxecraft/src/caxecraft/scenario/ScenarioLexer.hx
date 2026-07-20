package caxecraft.scenario;

import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexToken;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexTokenKind;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;
import haxe.io.Bytes;

/** Bounded UTF-8 decoder and tokenizer for the line-oriented CAXEMAP grammar. */
final class ScenarioLexer {
	public static function read(input:Bytes):ScenarioReadResult<Array<ScenarioLexRecord>> {
		if (input.length > ScenarioLimits.MAX_FILE_BYTES)
			return fail(1, 1, 0, LimitExceeded(FileBytes, ScenarioLimits.MAX_FILE_BYTES));
		final decoded = decodeUtf8(input);
		return switch decoded {
			case ReadError(diagnostics): ReadError(diagnostics);
			case ReadOk(text): tokenize(text);
		}
	}

	static function decodeUtf8(input:Bytes):ScenarioReadResult<String> {
		if (input.length >= 3 && input.get(0) == 0xef && input.get(1) == 0xbb && input.get(2) == 0xbf)
			return fail(1, 1, 0, MalformedUtf8(0));
		final output = new StringBuf();
		var offset = 0;
		while (offset < input.length) {
			final first = input.get(offset);
			var scalar = 0;
			var width = 0;
			if (first <= 0x7f) {
				scalar = first;
				width = 1;
			} else if (first >= 0xc2 && first <= 0xdf) {
				scalar = first & 0x1f;
				width = 2;
			} else if (first >= 0xe0 && first <= 0xef) {
				scalar = first & 0x0f;
				width = 3;
			} else if (first >= 0xf0 && first <= 0xf4) {
				scalar = first & 0x07;
				width = 4;
			} else {
				return fail(1, 1, 0, MalformedUtf8(offset));
			}
			if (offset + width > input.length)
				return fail(1, 1, 0, MalformedUtf8(offset));
			for (index in 1...width) {
				final continuation = input.get(offset + index);
				if ((continuation & 0xc0) != 0x80)
					return fail(1, 1, 0, MalformedUtf8(offset + index));
				scalar = (scalar << 6) | (continuation & 0x3f);
			}
			final overlong = (width == 2 && scalar < 0x80) || (width == 3 && scalar < 0x800) || (width == 4 && scalar < 0x10000);
			if (overlong || scalar > 0x10ffff || (scalar >= 0xd800 && scalar <= 0xdfff) || scalar == 0)
				return fail(1, 1, 0, MalformedUtf8(offset));
			output.addChar(scalar);
			offset += width;
		}
		return ReadOk(output.toString());
	}

	static function tokenize(text:String):ScenarioReadResult<Array<ScenarioLexRecord>> {
		final records:Array<ScenarioLexRecord> = [];
		var line = 1;
		var record = 0;
		var start = 0;
		var index = 0;
		while (index <= text.length) {
			if (index == text.length || text.charCodeAt(index) == 10) {
				var end = index;
				if (end > start && text.charCodeAt(end - 1) == 13)
					end--;
				final result = tokenizeLine(text.substring(start, end), line, record + 1);
				switch result {
					case ReadError(diagnostics):
						return ReadError(diagnostics);
					case ReadOk(null):
					case ReadOk(value):
						record++;
						if (record > ScenarioLimits.MAX_RECORDS)
							return fail(line, 1, record, LimitExceeded(LogicalRecords, ScenarioLimits.MAX_RECORDS));
						records.push(value);
				}
				line++;
				start = index + 1;
			}
			index++;
		}
		return ReadOk(records);
	}

	static function tokenizeLine(lineText:String, line:Int, record:Int):ScenarioReadResult<Null<ScenarioLexRecord>> {
		var index = 0;
		while (index < lineText.length && lineText.charCodeAt(index) == 32)
			index++;
		final indent = index;
		if (index == lineText.length || lineText.charCodeAt(index) == 35)
			return ReadOk(null);
		final tokens:Array<ScenarioLexToken> = [];
		while (index < lineText.length) {
			while (index < lineText.length && lineText.charCodeAt(index) == 32)
				index++;
			if (index == lineText.length)
				break;
			if (lineText.charCodeAt(index) == 9)
				return fail(line, index + 1, record, InvalidToken);
			final column = index + 1;
			if (lineText.charCodeAt(index) == 40 || lineText.charCodeAt(index) == 41) {
				tokens.push({text: lineText.charAt(index), kind: BareToken, coordinate: {line: line, column: column, record: record}});
				index++;
				continue;
			} else if (lineText.charCodeAt(index) == 34) {
				final quoted = readQuoted(lineText, index, line, record);
				switch quoted {
					case ReadError(diagnostics):
						return ReadError(diagnostics);
					case ReadOk(value):
						tokens.push({text: value.text, kind: QuotedText, coordinate: {line: line, column: column, record: record}});
						index = value.next;
				}
			} else {
				final begin = index;
				while (index < lineText.length && lineText.charCodeAt(index) != 32 && lineText.charCodeAt(index) != 40 && lineText.charCodeAt(index) != 41) {
					final code = lineText.charCodeAt(index);
					if (code == 9 || code == 34 || code == 13)
						return fail(line, index + 1, record, InvalidToken);
					index++;
				}
				tokens.push({text: lineText.substring(begin, index), kind: BareToken, coordinate: {line: line, column: column, record: record}});
			}
			if (index < lineText.length && lineText.charCodeAt(index) != 32 && lineText.charCodeAt(index) != 40 && lineText.charCodeAt(index) != 41)
				return fail(line, index + 1, record, InvalidToken);
		}
		return tokens.length == 0 ? ReadOk(null) : ReadOk({
			indent: indent,
			coordinate: {line: line, column: indent + 1, record: record},
			tokens: tokens
		});
	}

	static function readQuoted(lineText:String, start:Int, line:Int, record:Int):ScenarioReadResult<{text:String, next:Int}> {
		final output = new StringBuf();
		var scalars = 0;
		var index = start + 1;
		while (index < lineText.length) {
			final code = lineText.charCodeAt(index);
			if (code == 34)
				return ReadOk({text: output.toString(), next: index + 1});
			if (code < 32 || code == 127)
				return fail(line, index + 1, record, InvalidToken);
			if (code == 92) {
				index++;
				if (index >= lineText.length)
					return fail(line, index + 1, record, InvalidEscape);
				final escape = lineText.charCodeAt(index);
				switch escape {
					case 34:
						output.add('"');
					case 92:
						output.add("\\");
					case 110:
						output.add("\n");
					case 114:
						output.add("\r");
					case 116:
						output.add("\t");
					case 117:
						final unicode = readUnicodeEscape(lineText, index + 1, line, record);
						switch unicode {
							case ReadError(diagnostics): return ReadError(diagnostics);
							case ReadOk(value):
								output.addChar(value.scalar);
								index = value.last;
						}
					case _:
						return fail(line, index + 1, record, InvalidEscape);
				}
			} else {
				output.addChar(code);
			}
			scalars++;
			if (scalars > ScenarioLimits.MAX_TEXT_SCALARS)
				return fail(line, start + 1, record, LimitExceeded(TextScalars, ScenarioLimits.MAX_TEXT_SCALARS));
			index++;
		}
		return fail(line, start + 1, record, InvalidToken);
	}

	static function readUnicodeEscape(lineText:String, open:Int, line:Int, record:Int):ScenarioReadResult<{scalar:Int, last:Int}> {
		if (open >= lineText.length || lineText.charCodeAt(open) != 123)
			return fail(line, open + 1, record, InvalidEscape);
		var index = open + 1;
		var digits = 0;
		var scalar = 0;
		while (index < lineText.length && lineText.charCodeAt(index) != 125) {
			final digit = hexDigit(lineText.charCodeAt(index));
			if (digit < 0 || digits == 6)
				return fail(line, index + 1, record, InvalidEscape);
			scalar = (scalar << 4) | digit;
			digits++;
			index++;
		}
		if (digits == 0 || index >= lineText.length || scalar == 0 || scalar > 0x10ffff || (scalar >= 0xd800 && scalar <= 0xdfff))
			return fail(line, open + 1, record, InvalidEscape);
		return ReadOk({scalar: scalar, last: index});
	}

	static function hexDigit(code:Int):Int {
		if (code >= 48 && code <= 57)
			return code - 48;
		if (code >= 65 && code <= 70)
			return code - 55;
		return -1;
	}

	static function fail<T>(line:Int, column:Int, record:Int, kind:ScenarioDiagnosticKind):ScenarioReadResult<T> {
		return ReadError([{coordinate: {line: line, column: column, record: record}, kind: kind}]);
	}
}

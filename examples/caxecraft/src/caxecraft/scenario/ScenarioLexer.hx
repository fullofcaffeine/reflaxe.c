package caxecraft.scenario;

import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexToken;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexTokenKind;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;
import caxecraft.text.Utf8Decoder;
import caxecraft.text.Utf8Decoder.Utf8DecodeResult;
import haxe.io.Bytes;

/**
 * Turns bounded UTF-8 CAXEMAP bytes into source-located lexical records.
 *
 * The decoder admits the complete byte vector first. Tokenization then keeps
 * one String-based grammar so Unicode scalar columns and diagnostics cannot
 * disagree between a fast path and a fallback.
 */
final class ScenarioLexer {
	public static function read(input:Bytes):ScenarioReadResult<Array<ScenarioLexRecord>> {
		if (input.length > ScenarioLimits.MAX_FILE_BYTES)
			return fail(1, 1, 0, LimitExceeded(FileBytes, ScenarioLimits.MAX_FILE_BYTES));
		final decoded = Utf8Decoder.decode(input, ScenarioLimits.MAX_FILE_BYTES);
		return switch decoded {
			case Utf8Rejected(offset): fail(1, 1, 0, MalformedUtf8(offset));
			case Utf8Decoded(text, _): tokenize(text);
		}
	}

	/**
	 * Split the validated document once, then tokenize each short logical line.
	 *
	 * Haxe string indexes count Unicode scalar values. Repeatedly indexing the
	 * complete document can therefore rescan UTF-8 from its beginning on native
	 * targets. The standard split keeps that traversal linear while line-local
	 * token columns retain the same scalar-based diagnostics.
	 */
	static function tokenize(text:String):ScenarioReadResult<Array<ScenarioLexRecord>> {
		final records:Array<ScenarioLexRecord> = [];
		var record = 0;
		final lines = text.split("\n");
		for (lineIndex in 0...lines.length) {
			final line = lineIndex + 1;
			var lineText = lines[lineIndex];
			if (StringTools.endsWith(lineText, "\r"))
				lineText = lineText.substring(0, lineText.length - 1);
			final result = tokenizeLine(lineText, line, record + 1);
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
		}
		return ReadOk(records);
	}

	/**
	 * Tokenize one logical line and retain scalar-based source columns.
	 *
	 * Native strings derive scalar length and indexed codes from UTF-8. Keeping
	 * the length and current code in local values avoids repeated scans without
	 * changing the grammar or its coordinates.
	 */
	static function tokenizeLine(lineText:String, line:Int, record:Int):ScenarioReadResult<Null<ScenarioLexRecord>> {
		final lineLength = lineText.length;
		var index = 0;
		while (index < lineLength && lineText.charCodeAt(index) == 32)
			index++;
		final indent = index;
		if (index == lineLength || lineText.charCodeAt(index) == 35)
			return ReadOk(null);
		final tokens:Array<ScenarioLexToken> = [];
		while (index < lineLength) {
			while (index < lineLength && lineText.charCodeAt(index) == 32)
				index++;
			if (index == lineLength)
				break;
			final code = lineText.charCodeAt(index);
			if (code == 9)
				return fail(line, index + 1, record, InvalidToken);
			final column = index + 1;
			if (code == 40 || code == 41) {
				tokens.push({text: code == 40 ? "(" : ")", kind: BareToken, coordinate: {line: line, column: column, record: record}});
				index++;
				continue;
			} else if (code == 34) {
				final quoted = readQuoted(lineText, lineLength, index, line, record);
				switch quoted {
					case ReadError(diagnostics):
						return ReadError(diagnostics);
					case ReadOk(value):
						tokens.push({text: value.text, kind: QuotedText, coordinate: {line: line, column: column, record: record}});
						index = value.next;
				}
			} else {
				final begin = index;
				while (index < lineLength) {
					final bareCode = lineText.charCodeAt(index);
					if (bareCode == 32 || bareCode == 40 || bareCode == 41)
						break;
					if (bareCode == 9 || bareCode == 34 || bareCode == 13)
						return fail(line, index + 1, record, InvalidToken);
					index++;
				}
				tokens.push({text: lineText.substring(begin, index), kind: BareToken, coordinate: {line: line, column: column, record: record}});
			}
			if (index < lineLength) {
				final separator = lineText.charCodeAt(index);
				if (separator != 32 && separator != 40 && separator != 41)
					return fail(line, index + 1, record, InvalidToken);
			}
		}
		return tokens.length == 0 ? ReadOk(null) : ReadOk({
			indent: indent,
			coordinate: {line: line, column: indent + 1, record: record},
			tokens: tokens
		});
	}

	/** Read one quoted token while reusing the caller's scalar line length. */
	static function readQuoted(lineText:String, lineLength:Int, start:Int, line:Int, record:Int):ScenarioReadResult<{text:String, next:Int}> {
		final output = new StringBuf();
		var scalars = 0;
		var index = start + 1;
		while (index < lineLength) {
			final code = lineText.charCodeAt(index);
			if (code == 34)
				return ReadOk({text: output.toString(), next: index + 1});
			if (code < 32 || code == 127)
				return fail(line, index + 1, record, InvalidToken);
			if (code == 92) {
				index++;
				if (index >= lineLength)
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
						final unicode = readUnicodeEscape(lineText, lineLength, index + 1, line, record);
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

	/** Decode one braced escape without recounting the containing line. */
	static function readUnicodeEscape(lineText:String, lineLength:Int, open:Int, line:Int, record:Int):ScenarioReadResult<{scalar:Int, last:Int}> {
		if (open >= lineLength || lineText.charCodeAt(open) != 123)
			return fail(line, open + 1, record, InvalidEscape);
		var index = open + 1;
		var digits = 0;
		var scalar = 0;
		while (index < lineLength) {
			final code = lineText.charCodeAt(index);
			if (code == 125)
				break;
			final digit = hexDigit(code);
			if (digit < 0 || digits == 6)
				return fail(line, index + 1, record, InvalidEscape);
			scalar = (scalar << 4) | digit;
			digits++;
			index++;
		}
		if (digits == 0 || index >= lineLength || scalar == 0 || scalar > 0x10ffff || (scalar >= 0xd800 && scalar <= 0xdfff))
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

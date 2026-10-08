package caxecraft.editor;

import caxecraft.scenario.ScenarioLimits;
import caxecraft.text.Utf8Decoder;
import caxecraft.text.Utf8Decoder.Utf8DecodeResult;
import haxe.io.Bytes;

/**
	Owns one bounded, copy-owned CAXEMAP text draft for the advanced editor.

	The live `EditorSession` remains the only typed scenario owner. This class
	keeps source lines separate until the creator chooses Apply, which lets a
	malformed or semantically incomplete source remain editable without replacing
	the playable visual draft. A successful session apply rewrites this document
	from canonical bytes, so cards, text, Save, Test Play, and automation all meet
	again at the public CAXEMAP codec boundary.

	A stateful class is appropriate because selected source text has identity and
	a dirty lifetime across native frames. Stateless parsing and validation stay
	in `EditorSession` rather than becoming a second editor model.
**/
enum EditorTextDocumentError {
	InvalidLine(index:Int);
	LineTooLarge(bytes:Int, maximum:Int);
	TooManyLines(lines:Int, maximum:Int);
	DocumentTooLarge(bytes:Int, maximum:Int);
	CannotRemoveOnlyLine;
	MalformedSource(byteOffset:Int);
}

/** Result of opening copied UTF-8 source as an editable line document. */
enum EditorTextDocumentOpenResult {
	TextDocumentOpened(document:EditorTextDocument);
	TextDocumentOpenRejected(error:EditorTextDocumentError);
}

/** Result of one bounded source-line edit. */
enum EditorTextEditResult {
	TextEditApplied;
	TextEditUnchanged;
	TextEditRejected(error:EditorTextDocumentError);
}

/** Mutable source lines that cannot affect a scenario until session Apply. */
final class EditorTextDocument {
	/** Canonical CAXEMAP lines stay far below this native edit-buffer bound. */
	public static inline final MAX_LINE_BYTES:Int = 131072;

	final lines:Array<String>;
	var encodedBytes:Int;
	var changed:Bool;

	/**
		Open a copied source document with one visible line even for empty input.

		The visual editor calls this with canonical bytes. Accepting source without
		a final newline keeps the helper useful in focused tests; `snapshot()` always
		returns the canonical editor convention of one LF after every line.
	**/
	public static function open(source:Bytes):EditorTextDocumentOpenResult {
		if (source.length > ScenarioLimits.MAX_FILE_BYTES)
			return TextDocumentOpenRejected(DocumentTooLarge(source.length, ScenarioLimits.MAX_FILE_BYTES));
		return switch Utf8Decoder.decode(source, ScenarioLimits.MAX_FILE_BYTES) {
			case Utf8Rejected(offset): TextDocumentOpenRejected(MalformedSource(offset));
			case Utf8Decoded(text, _):
				final sourceLines = text.split("\n");
				if (sourceLines.length > 1 && sourceLines[sourceLines.length - 1] == "")
					sourceLines.pop();
				if (sourceLines.length == 0)
					sourceLines.push("");
				if (sourceLines.length > ScenarioLimits.MAX_RECORDS) TextDocumentOpenRejected(TooManyLines(sourceLines.length,
					ScenarioLimits.MAX_RECORDS)); else {
					var encoded = sourceLines.length;
					var failure:Null<EditorTextDocumentError> = null;
					for (line in sourceLines) {
						final length = Bytes.ofString(line).length;
						if (failure == null && length > MAX_LINE_BYTES)
							failure = LineTooLarge(length, MAX_LINE_BYTES);
						encoded += length;
					}
					if (failure != null)
						TextDocumentOpenRejected(failure);
					else if (encoded > ScenarioLimits.MAX_FILE_BYTES)
						TextDocumentOpenRejected(DocumentTooLarge(encoded, ScenarioLimits.MAX_FILE_BYTES));
					else
						TextDocumentOpened(new EditorTextDocument(sourceLines, encoded));
				}
		}
	}

	private function new(lines:Array<String>, encodedBytes:Int) {
		this.lines = lines;
		this.encodedBytes = encodedBytes;
		this.changed = false;
	}

	/** Number of editable lines, always at least one. */
	public inline function lineCount():Int
		return lines.length;

	/** Current UTF-8 file size including one LF after every line. */
	public inline function byteLength():Int
		return encodedBytes;

	/** True after a source edit and before a successful canonical refresh. */
	public inline function isDirty():Bool
		return changed;

	/** Return one immutable line snapshot, or `null` outside the document. */
	public function lineAt(index:Int):Null<String>
		return validLine(index) ? lines[index] : null;

	/**
		Replace one line only after its complete UTF-8 size fits every bound.

		Failure leaves the old source and dirty state untouched. Syntax is not
		checked here because invalid text must remain available for repair.
	**/
	public function replaceLine(index:Int, value:String):EditorTextEditResult {
		if (!validLine(index))
			return TextEditRejected(InvalidLine(index));
		final nextLength = Bytes.ofString(value).length;
		if (nextLength > MAX_LINE_BYTES)
			return TextEditRejected(LineTooLarge(nextLength, MAX_LINE_BYTES));
		final previous = lines[index];
		if (previous == value)
			return TextEditUnchanged;
		final total = encodedBytes - Bytes.ofString(previous).length + nextLength;
		if (total > ScenarioLimits.MAX_FILE_BYTES)
			return TextEditRejected(DocumentTooLarge(total, ScenarioLimits.MAX_FILE_BYTES));
		lines[index] = value;
		encodedBytes = total;
		changed = true;
		return TextEditApplied;
	}

	/** Insert one line after the selected line without changing another line. */
	public function insertLineAfter(index:Int, value:String = ""):EditorTextEditResult {
		if (!validLine(index))
			return TextEditRejected(InvalidLine(index));
		if (lines.length == ScenarioLimits.MAX_RECORDS)
			return TextEditRejected(TooManyLines(lines.length + 1, ScenarioLimits.MAX_RECORDS));
		final length = Bytes.ofString(value).length;
		if (length > MAX_LINE_BYTES)
			return TextEditRejected(LineTooLarge(length, MAX_LINE_BYTES));
		final total = encodedBytes + length + 1;
		if (total > ScenarioLimits.MAX_FILE_BYTES)
			return TextEditRejected(DocumentTooLarge(total, ScenarioLimits.MAX_FILE_BYTES));
		lines.insert(index + 1, value);
		encodedBytes = total;
		changed = true;
		return TextEditApplied;
	}

	/** Remove one selected line while retaining a visible repair surface. */
	public function removeLine(index:Int):EditorTextEditResult {
		if (!validLine(index))
			return TextEditRejected(InvalidLine(index));
		if (lines.length == 1)
			return TextEditRejected(CannotRemoveOnlyLine);
		final previous = lines[index];
		lines.splice(index, 1);
		encodedBytes -= Bytes.ofString(previous).length + 1;
		changed = true;
		return TextEditApplied;
	}

	/** Copy the complete draft with deterministic LF line endings. */
	public function snapshot():Bytes
		return Bytes.ofString(lines.join("\n") + "\n");

	/** Clear dirty presentation state after replacing from accepted canonical bytes. */
	public inline function markClean():Void
		changed = false;

	private inline function validLine(index:Int):Bool
		return index >= 0 && index < lines.length;
}

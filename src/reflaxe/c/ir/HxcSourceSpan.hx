package reflaxe.c.ir;

/**
	A stable, repository-relative Haxe source range.

	Lines and columns are one-based and the end point is exclusive. The IR
	validator rejects absolute, non-normalized, or malformed paths rather than
	letting checkout-local paths leak into dumps and diagnostics.
 */
class HxcSourceSpan {
	public final file:String;
	public final startLine:Int;
	public final startColumn:Int;
	public final endLine:Int;
	public final endColumn:Int;

	public function new(file:String, startLine:Int, startColumn:Int, endLine:Int, endColumn:Int) {
		this.file = file;
		this.startLine = startLine;
		this.startColumn = startColumn;
		this.endLine = endLine;
		this.endColumn = endColumn;
	}

	/** Formatted `display()` text, built on first use; every field it reads is final. */
	var displayText:Null<String> = null;

	/** Return `file:startLine:startColumn-endLine:endColumn`, formatting it at most once. */
	public function display():String {
		final known = displayText;
		if (known != null)
			return known;
		final text = '$file:$startLine:$startColumn-$endLine:$endColumn';
		displayText = text;
		return text;
	}

	public function stableForDiagnostic():HxcSourceSpan {
		return isNormalizedFile(file) ? this : new HxcSourceSpan("invalid/source", startLine, startColumn, endLine, endColumn);
	}

	/**
		Remembered answers for `isNormalizedFile`, keyed by the exact path text.

		Every HxcIR instruction carries a source span, so lowering and validation
		ask about the same few dozen file paths hundreds of thousands of times. The
		answer depends only on the string, so remembering it cannot go stale across
		compiler-server requests, and the map holds at most one entry per distinct
		source path the compiler has seen.
	**/
	static final normalizedFileAnswers:Map<String, Bool> = [];

	/** Whether `value` is a repository-relative `/`-separated path with no unsafe parts. */
	public static function isNormalizedFile(value:String):Bool {
		final known = normalizedFileAnswers.get(value);
		if (known != null)
			return known;
		final answer = computeIsNormalizedFile(value);
		normalizedFileAnswers.set(value, answer);
		return answer;
	}

	static function computeIsNormalizedFile(value:String):Bool {
		if (value == "" || StringTools.startsWith(value, "/") || StringTools.startsWith(value, "~") || value.indexOf("\\") != -1
			|| value.indexOf("\x00") != -1 || ~/^[A-Za-z]:/.match(value)) {
			return false;
		}
		for (part in value.split("/")) {
			if (part == "" || part == "." || part == "..") {
				return false;
			}
		}
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null || code < 0x20 || code == 0x7F) {
				return false;
			}
		}
		return true;
	}
}

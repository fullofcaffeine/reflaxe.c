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

	public function display():String
		return '$file:$startLine:$startColumn-$endLine:$endColumn';

	public function stableForDiagnostic():HxcSourceSpan {
		return isNormalizedFile(file) ? this : new HxcSourceSpan("invalid/source", startLine, startColumn, endLine, endColumn);
	}

	public static function isNormalizedFile(value:String):Bool {
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

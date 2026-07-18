package hxc.config;

import reflaxe.c.CDiagnostic;
import reflaxe.c.CDiagnostic.CDiagnosticId;

/**
	A fail-closed configuration error before Haxe typing or native compilation.

	The source name is logical and project-relative. Host paths are deliberately
	kept out of the message so JSON/human inspection cannot leak a checkout path.
**/
class HxcConfigError extends haxe.Exception {
	public final diagnosticId:CDiagnosticId;
	public final source:String;
	public final line:Int;
	public final column:Int;
	public final detail:String;

	public function new(detail:String, source:String = "hxc.json", line:Int = 1, column:Int = 1) {
		if (!isLogicalSource(source)) {
			throw CDiagnostic.codeMessage(CDiagnosticId.InternalCompilerError, 'invalid configuration source anchor `$source`');
		}
		if (line < 1 || column < 1 || StringTools.trim(detail) == "") {
			throw CDiagnostic.codeMessage(CDiagnosticId.InternalCompilerError, "invalid configuration diagnostic coordinates or detail");
		}
		this.diagnosticId = CDiagnosticId.InvalidConfiguration;
		this.source = source;
		this.line = line;
		this.column = column;
		this.detail = detail;
		super(CDiagnostic.codeMessage(diagnosticId, '$source:$line:$column: $detail'));
	}

	static function isLogicalSource(value:String):Bool {
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

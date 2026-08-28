package hxc.cli;

/** JSON shape for one stable command-line diagnostic. */
typedef HxcCliDiagnosticJson = {
	final code:String;
	final message:String;
	final remediation:String;
}

/** One validated diagnostic rendered identically by every command. */
class HxcCliDiagnostic {
	public final code:String;
	public final message:String;
	public final remediation:String;

	public function new(code:String, message:String, remediation:String) {
		if (!~/^HXC-CLI-[0-9]{4}$/.match(code) || StringTools.trim(message) == "" || StringTools.trim(remediation) == "") {
			throw "CLI diagnostics require a code, message, and remediation";
		}
		this.code = code;
		this.message = message;
		this.remediation = remediation;
	}

	/** Render the one-line human diagnostic contract. */
	public function render():String
		return '$code: $message Remediation: $remediation';

	/** Return a precise JSON value without exposing a dynamic shape. */
	public function toJsonValue():HxcCliDiagnosticJson
		return {code: code, message: message, remediation: remediation};
}

package caxecraft.scenario;

/** Stable identity of one object, rule, sequence, objective, or map record. */
abstract ScenarioId(String) {
	public inline function new(value:String)
		this = value;

	public inline function text():String
		return this;
}

/**
	Return true when text has the canonical lowercase scenario-identity shape.

	Each dot, underscore, or dash separates non-empty parts, and every part starts
	with an ASCII lowercase letter. The parser and visual rename field share this
	function so the editor cannot create an identity that canonical text rejects.
**/
function isValidScenarioIdText(value:String):Bool {
	if (value.length == 0)
		return false;
	var expectLetter = true;
	for (at in 0...value.length) {
		final code = value.charCodeAt(at);
		if (expectLetter) {
			if (code < 97 || code > 122)
				return false;
			expectLetter = false;
		} else if (code == 46 || code == 95 || code == 45) {
			expectLetter = true;
		} else if (!((code >= 97 && code <= 122) || (code >= 48 && code <= 57))) {
			return false;
		}
	}
	return !expectLetter;
}

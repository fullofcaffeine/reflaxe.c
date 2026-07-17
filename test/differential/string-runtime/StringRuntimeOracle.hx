class StringRuntimeOracle {
	static function main():Void {
		final emoji = "😀";
		final embedded = "a\x00b";
		final composed = "é";
		final decomposed = "é";
		final sequence = "x😀é";
		Sys.println(emoji.length + "," + emoji.charCodeAt(0) + "," + embedded.length + "," + composed.length + "," + decomposed.length + ","
			+ sequence.substr(1, 1));
	}
}

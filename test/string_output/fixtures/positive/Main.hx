class Main {
	static function main():Void {
		if (selectLabel(true) == "ASCII" && selectLabel(false) == "fallback")
			Sys.println("ASCII");
		else
			Sys.println("conditional string failure");
		Sys.println("é🙂");
		Sys.println("embedded\x00NUL");
		trace("traced");
	}

	/**
		Select one immutable Haxe String through ordinary expression-valued control
		flow. Both calls are observed so the native oracle executes each branch.

		`Std.string` receives an already-typed String here. The call is deliberately
		kept in this literal-only fixture so its generated plan proves that the
		conversion preserves the static view without allocation or a String runtime
		operation.
	**/
	static function selectLabel(primary:Bool):String {
		final selected = primary ? "ASCII" : "fallback";
		return Std.string(selected);
	}
}

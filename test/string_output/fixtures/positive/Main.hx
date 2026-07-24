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
	**/
	static function selectLabel(primary:Bool):String {
		final selected = primary ? "ASCII" : "fallback";
		return selected;
	}
}

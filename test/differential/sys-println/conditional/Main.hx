/** Proves direct conditional String arguments without a caller-written cast. */
class Main {
	static var conditions = 0;
	static var values = 0;

	/** Make one-time condition evaluation observable. */
	static function choose(selected:Bool):Bool {
		conditions++;
		return selected;
	}

	/** Count only the selected branch and return freshly allocated text. */
	static function value(label:String):String {
		values++;
		return label + "é\x00🙂";
	}

	/** Print both branches, nested selection, and a borrowed literal selection. */
	static function main():Void {
		Sys.println(choose(true) ? value("yes:") : value("wrong:"));
		Sys.println(choose(false) ? value("wrong:") : value("no:"));
		Sys.println(choose(true) ? (choose(false) ? "wrong" : "nested") : "wrong");
		if (conditions == 4 && values == 2)
			Sys.println("once");
		else
			Sys.println("evaluation failure");
	}
}

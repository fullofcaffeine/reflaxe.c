package layout;

/** Eval oracle deliberately stays outside the C entry-point reachability graph. */
class Oracle {
	static function main():Void {
		Sys.println(Main.trace());
	}
}

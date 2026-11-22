/** Keeps String-only conditional printing allocation-free. */
class Main {
	/** Preserve the conditional in the typed program for both branch outcomes. */
	static function print(selected:Bool):Void {
		Sys.println(selected ? "yes" : "no");
	}

	/** Exercise both branches without any operation that creates owned text. */
	static function main():Void {
		print(true);
		print(false);
		Sys.println(true);
		Sys.println(false);
	}
}

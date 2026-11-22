/** Proves that printing an Int preserves its decimal value and evaluates it once. */
class Main {
	static var calls = 0;

	/** Expose repeated argument evaluation through the next printed counter. */
	static function next():Int {
		calls++;
		return -2147483648;
	}

	/** Exercise both signed limits, zero, and a side-effecting call. */
	static function main():Void {
		Sys.println(next());
		Sys.println(calls);
		Sys.println(0);
		Sys.println(2147483647);
	}
}

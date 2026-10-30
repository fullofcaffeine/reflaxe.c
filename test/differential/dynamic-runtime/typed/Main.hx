/**
	Keeps ordinary typed control flow outside the Dynamic carrier.

	This neighboring program uses the same primitive families as the Dynamic
	fixture. Its HxcIR, generated C, and runtime plan must contain no Dynamic
	representation or helper merely because Dynamic support exists in the target.
**/
class Main {
	/** Keep typed arithmetic and conditions reachable without runtime I/O. */
	static function main():Void {
		final integer = 7;
		final floating = 2.5;
		final boolean = true;
		if (integer == 0 && floating == 0.0 && !boolean)
			return;
	}
}

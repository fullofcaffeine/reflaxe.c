/** Exercises existing Float formatting through ordinary hosted output. */
class Main {
	/** Check signs, fractions, precision, and exponent spellings against Eval. */
	static function main():Void {
		Sys.println(1.5);
		Sys.println(-0.0);
		Sys.println(0.0000001);
		Sys.println(100000000000000000000.0);
		Sys.println(1.2345678901234567);
	}
}

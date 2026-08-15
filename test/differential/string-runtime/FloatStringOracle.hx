/**
	Records Haxe Eval's observable `Std.string(Float)` spellings.

	The C runtime fixture uses this independently executed Haxe program as its
	oracle. The selected values cover signed zero, integral and fractional text,
	exponent thresholds, finite precision, and non-finite IEEE-754 values.
**/
final class FloatStringOracle {
	/** Print one labelled value so representation drift is easy to diagnose. */
	static function print(label:String, value:Float):Void
		Sys.println(label + "=" + Std.string(value));

	/** Emit the bounded Float spelling contract in a stable order. */
	static function main():Void {
		print("zero", 0.0);
		print("negative-zero", -0.0);
		print("one", 1.0);
		print("fraction", 1.5);
		print("small-fixed", 0.00001);
		print("small-exponent", 0.0000001);
		print("large-fixed", 1000000000000000.0);
		print("large-exponent", 100000000000000000000.0);
		print("precision", 1.2345678901234567);
		print("positive-infinity", Math.POSITIVE_INFINITY);
		print("negative-infinity", Math.NEGATIVE_INFINITY);
		print("nan", Math.NaN);
	}
}

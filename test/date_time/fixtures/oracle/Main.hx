/**
 * Prints the pinned Eval target's observable Date behavior.
 *
 * The harness launches this program in fresh processes with explicit POSIX
 * timezone rules. Those process boundaries keep timezone state out of the test
 * program and let native C reuse the same source as a differential oracle.
 */
class Main {
	/** Print one timestamp's preserved value plus its UTC and local projection. */
	static function printTimestamp(label:String, milliseconds:Float):Void {
		final value = Date.fromTime(milliseconds);
		Sys.println(label + "|time=" + value.getTime() + "|utc=" + value.getUTCFullYear() + "-" + (value.getUTCMonth() + 1) + "-" + value.getUTCDate() + "T"
			+ value.getUTCHours() + ":" + value.getUTCMinutes() + ":" + value.getUTCSeconds() + "|local=" + value.toString() + "|offset="
			+ value.getTimezoneOffset());
	}

	/** Print one local constructor around a daylight-saving transition. */
	static function printLocal(label:String, year:Int, month:Int, day:Int, hour:Int, minute:Int, second:Int):Void {
		final value = new Date(year, month, day, hour, minute, second);
		Sys.println(label + "|time=" + value.getTime() + "|local=" + value.toString() + "|offset=" + value.getTimezoneOffset());
	}

	/** Exercise values whose target behavior must be frozen before C lowering. */
	static function main():Void {
		printTimestamp("epoch", 0.0);
		printTimestamp("positive-fraction", 999.75);
		printTimestamp("negative-fraction", -0.25);
		printTimestamp("leap-day", 951827696000.5);
		printTimestamp("spring-before", 1710057599000.0);
		printTimestamp("spring-at", 1710057600000.0);
		printTimestamp("fall-first", 1730615400000.0);
		printTimestamp("fall-second", 1730619000000.0);
		printLocal("spring-gap", 2024, 2, 10, 2, 30, 0);
		printLocal("fall-overlap", 2024, 10, 3, 1, 30, 0);
		final first = Date.fromTime(0.0);
		final alias = first;
		final equalValue = Date.fromTime(0.0);
		Sys.println("identity|alias=" + (first == alias) + "|equal-value=" + (first == equalValue));
	}
}

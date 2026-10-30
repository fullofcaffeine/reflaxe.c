/** Proves deterministic UTC projection without selecting host calendar data. */
class Main {
	/** Compare all public UTC fields for one representative timestamp. */
	static function assertUtc(value:Date, year:Int, month:Int, day:Int, weekday:Int, hour:Int, minute:Int, second:Int):Void {
		if (value.getUTCFullYear() != year || value.getUTCMonth() != month || value.getUTCDate() != day || value.getUTCDay() != weekday
			|| value.getUTCHours() != hour || value.getUTCMinutes() != minute || value.getUTCSeconds() != second)
			throw "UTC Date projection mismatch";
	}

	/** Exercise epoch, fractional, negative, and Gregorian leap-day boundaries. */
	static function main():Void {
		assertUtc(Date.fromTime(0.0), 1970, 0, 1, 4, 0, 0, 0);
		assertUtc(Date.fromTime(999.75), 1970, 0, 1, 4, 0, 0, 0);
		assertUtc(Date.fromTime(-0.25), 1970, 0, 1, 4, 0, 0, 0);
		assertUtc(Date.fromTime(-1000.25), 1969, 11, 31, 3, 23, 59, 59);
		assertUtc(Date.fromTime(951827696000.5), 2000, 1, 29, 2, 12, 34, 56);
		Sys.println("date-time-utc-ok");
	}
}

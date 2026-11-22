import c.internal.DateHost;

/**
 * Declares the pinned ordinary Date API for the C target.
 *
 * Date values remain nominal objects whose canonical instant is stored as Float
 * milliseconds. Portable calendar calculations live here as ordinary Haxe;
 * the compiler intervenes only for fresh factory allocation and narrow host
 * clock or timezone services. This keeps platform policy out of the public
 * class while making the source implementation useful teaching material.
 */
@:coreApi
final class Date {
	/** Canonical Unix-epoch milliseconds shared by every Date operation. */
	private var milliseconds(default, null):Float;

	/** Construct one fresh Date from local civil fields. */
	public function new(year:Int, month:Int, day:Int, hour:Int, min:Int, sec:Int) {
		milliseconds = DateHost.localToMilliseconds(year, month, day, hour, min, sec);
	}

	/** Return this Date's Unix-epoch timestamp in milliseconds. */
	public function getTime():Float
		return milliseconds;

	/** Return the local hour in the range 0 through 23. */
	public function getHours():Int
		return localParts().hour;

	/** Return the local minute in the range 0 through 59. */
	public function getMinutes():Int
		return localParts().minute;

	/** Return the local second in the range 0 through 59. */
	public function getSeconds():Int
		return localParts().second;

	/** Return the local year. */
	public function getFullYear():Int
		return localParts().year;

	/** Return the zero-based local month. */
	public function getMonth():Int
		return localParts().month;

	/** Return the one-based local day of month. */
	public function getDate():Int
		return localParts().day;

	/** Return the local weekday, where Sunday is zero. */
	public function getDay():Int
		return localParts().weekday;

	/** Return the UTC hour in the range 0 through 23. */
	public function getUTCHours():Int
		return utcParts(milliseconds).hour;

	/** Return the UTC minute in the range 0 through 59. */
	public function getUTCMinutes():Int
		return utcParts(milliseconds).minute;

	/** Return the UTC second in the range 0 through 59. */
	public function getUTCSeconds():Int
		return utcParts(milliseconds).second;

	/** Return the UTC year. */
	public function getUTCFullYear():Int
		return utcParts(milliseconds).year;

	/** Return the zero-based UTC month. */
	public function getUTCMonth():Int
		return utcParts(milliseconds).month;

	/** Return the one-based UTC day of month. */
	public function getUTCDate():Int
		return utcParts(milliseconds).day;

	/** Return the UTC weekday, where Sunday is zero. */
	public function getUTCDay():Int
		return utcParts(milliseconds).weekday;

	/** Return local time minus UTC using Haxe's minute sign convention. */
	public function getTimezoneOffset():Int
		return DateHost.timezoneOffsetAt(milliseconds);

	/** Format local fields as `YYYY-MM-DD HH:MM:SS`. */
	public function toString():String {
		final value = localParts();
		return padYear(value.year) + "-" + padTwo(value.month + 1) + "-" + padTwo(value.day) + " " + padTwo(value.hour) + ":" + padTwo(value.minute) + ":"
			+ padTwo(value.second);
	}

	/** Construct one fresh Date from the current wall clock. */
	public static function now():Date
		return fromTime(DateHost.wallMilliseconds());

	/** Construct one fresh Date from Float Unix-epoch milliseconds. */
	public extern static function fromTime(t:Float):Date;

	/** Parse one of the three fixed formats admitted by the pinned Haxe API. */
	public extern static function fromString(s:String):Date;

	/** Truncate the instant as Eval does, apply the host offset, and reuse UTC arithmetic. */
	private function localParts():DateParts
		return utcParts(truncateToSecond(milliseconds) - DateHost.timezoneOffsetAt(milliseconds) * 60000.0);

	/**
	 * Remove sub-second precision without narrowing the complete timestamp to Int.
	 *
	 * Eval projects local fields from the instant truncated toward zero, then
	 * applies the timezone offset. Reducing the value to one day first keeps every
	 * Int conversion bounded even near Date's maximum admitted timestamp.
	 */
	private static function truncateToSecond(value:Float):Float {
		var epochDay = floorBounded(value / 86400000.0);
		final millisecondsInDay = value - epochDay * 86400000.0;
		var secondInDay = Std.int(millisecondsInDay / 1000.0);
		final projectedMilliseconds = millisecondsInDay - secondInDay * 1000.0;
		if (value < 0.0 && projectedMilliseconds > 0.0) {
			secondInDay++;
			if (secondInDay == 86400) {
				secondInDay = 0;
				epochDay++;
			}
		}
		return epochDay * 86400000.0 + secondInDay * 1000.0;
	}

	/** Format an ordinary non-negative two-digit calendar field. */
	private static function padTwo(value:Int):String
		return value < 10 ? "0" + Std.string(value) : Std.string(value);

	/** Format the common four-digit year range used by Date.toString. */
	private static function padYear(value:Int):String {
		if (value >= 1000)
			return Std.string(value);
		if (value >= 100)
			return "0" + Std.string(value);
		if (value >= 10)
			return "00" + Std.string(value);
		if (value >= 0)
			return "000" + Std.string(value);
		return Std.string(value);
	}

	/**
	 * Convert one validated timestamp to UTC civil fields without a host library.
	 *
	 * The algorithm uses a 400-year Gregorian era, so leap-year behavior is
	 * deterministic on every target. Intermediate Float values stay integral and
	 * exact throughout Date's admitted range; conversion to Int happens only after
	 * values have been reduced to a bounded year or calendar field.
	 */
	private static function utcParts(value:Float):DateParts {
		if (value != value || value < -8640000000000000.0 || value > 8640000000000000.0)
			throw "Date timestamp is outside the supported finite range";
		var epochDay = floorBounded(value / 86400000.0);
		final millisecondsInDay = value - epochDay * 86400000.0;
		var secondInDay = Std.int(millisecondsInDay / 1000.0);
		final projectedMilliseconds = millisecondsInDay - secondInDay * 1000.0;
		// Eval truncates the complete timestamp toward zero before projecting
		// fields. Correct the negative fractional case after reducing it to one day.
		if (value < 0.0 && projectedMilliseconds > 0.0) {
			secondInDay++;
			if (secondInDay == 86400) {
				secondInDay = 0;
				epochDay++;
			}
		}

		final shiftedDay = epochDay + 719468.0;
		final era = floorBounded(shiftedDay / 146097.0);
		final dayOfEra = shiftedDay - era * 146097.0;
		final yearOfEra = floorBounded((dayOfEra - floorBounded(dayOfEra / 1460.0) +
			floorBounded(dayOfEra / 36524.0) - floorBounded(dayOfEra / 146096.0)) / 365.0);
		var year = yearOfEra + era * 400.0;
		final dayOfYear = dayOfEra - (365.0 * yearOfEra + floorBounded(yearOfEra / 4.0) - floorBounded(yearOfEra / 100.0));
		final marchMonth = floorBounded((5.0 * dayOfYear + 2.0) / 153.0);
		final day = dayOfYear - floorBounded((153.0 * marchMonth + 2.0) / 5.0) + 1.0;
		final month = marchMonth + (marchMonth < 10.0 ? 3.0 : -9.0);
		if (month <= 2.0)
			year++;

		var weekday = Std.int(epochDay - floorBounded(epochDay / 7.0) * 7.0) + 4;
		if (weekday >= 7)
			weekday -= 7;
		if (weekday < 0)
			weekday += 7;
		return {
			year: Std.int(year),
			month: Std.int(month) - 1,
			day: Std.int(day),
			weekday: weekday,
			hour: Std.int(secondInDay / 3600),
			minute: Std.int((secondInDay - Std.int(secondInDay / 3600) * 3600) / 60),
			second: secondInDay - Std.int(secondInDay / 60) * 60
		};
	}

	/** Return mathematical floor for values already bounded to the Int range. */
	private static function floorBounded(value:Float):Float {
		final truncated = Std.int(value);
		return value < truncated ? truncated - 1.0 : truncated;
	}
}

/** Bounded UTC calendar projection used internally by Date accessors. */
private typedef DateParts = {
	final year:Int;
	final month:Int;
	final day:Int;
	final weekday:Int;
	final hour:Int;
	final minute:Int;
	final second:Int;
}

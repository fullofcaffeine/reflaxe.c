/** Proves host local-calendar behavior under an explicit POSIX timezone rule. */
class Main {
	/** Require one condition without coupling generated output to test details. */
	static function require(value:Bool):Void {
		if (!value)
			throw "local Date contract mismatch";
	}

	/** Exercise daylight-saving boundaries, normalization, wall time, and text. */
	static function main():Void {
		final beforeSpring = Date.fromTime(1710057599000.0);
		final atSpring = Date.fromTime(1710057600000.0);
		require(beforeSpring.getTimezoneOffset() == 360);
		require(atSpring.getTimezoneOffset() == 300);
		require(atSpring.getHours() == 3 && atSpring.getMinutes() == 0);

		final beforeFall = Date.fromTime(1730617199000.0);
		final atFall = Date.fromTime(1730617200000.0);
		require(beforeFall.getTimezoneOffset() == 300);
		require(atFall.getTimezoneOffset() == 360);

		final springGap = new Date(2024, 2, 10, 2, 30, 0);
		require(springGap.getHours() == 3 && springGap.getMinutes() == 30);
		final fallOverlap = new Date(2024, 10, 3, 1, 30, 0);
		require((fallOverlap.getTime() == 1730615400000.0 && fallOverlap.getTimezoneOffset() == 300)
			|| (fallOverlap.getTime() == 1730619000000.0 && fallOverlap.getTimezoneOffset() == 360));
		require(fallOverlap.toString() == "2024-11-03 01:30:00");
		require(atSpring.toString() == "2024-03-10 03:00:00");
		require(Date.now().getTime() > 1700000000000.0);
		final monotonicBefore = haxe.Timer.stamp();
		final monotonicAfter = haxe.Timer.stamp();
		require(monotonicAfter >= monotonicBefore);
		Sys.println("date-time-local-ok");
	}
}

/** Requests hosted Date services without using another platform-only API. */
class Main {
	/** Keep all four hosted operations reachable until runtime planning. */
	static function main():Void {
		final local = new Date(2024, 2, 10, 2, 30, 0);
		final wall = Date.now();
		final stamp = haxe.Timer.stamp();
		if (local.getTimezoneOffset() == 0 && wall.getTime() == stamp)
			throw "unreachable date-time probe";
	}
}

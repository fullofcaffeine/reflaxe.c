package c.internal;

/**
	Names the hosted services that portable Date arithmetic cannot provide.

	The class has no runtime identity. haxe.c recognizes these exact declarations
	and lowers each call to the selected date-time status/out boundary. Keeping the
	services outside the core Date prototype preserves Haxe's exact public API.
	A private class, instead of module-level functions, gives the compiler one
	nominal owner it can recognize without granting intrinsic behavior by name.
**/
final class DateHost {
	private function new() {}

	/** Convert local civil fields through host timezone and daylight rules. */
	public static function localToMilliseconds(year:Int, month:Int, day:Int, hour:Int, minute:Int, second:Int):Float
		throw "DateHost.localToMilliseconds must be lowered by haxe.c";

	/** Return Haxe's UTC-minus-local offset at one exact instant. */
	public static function timezoneOffsetAt(milliseconds:Float):Int
		throw "DateHost.timezoneOffsetAt must be lowered by haxe.c";

	/** Read adjustable Unix-epoch wall-clock milliseconds. */
	public static function wallMilliseconds():Float
		throw "DateHost.wallMilliseconds must be lowered by haxe.c";
}

package haxe;

import haxe.Int64;

/**
	Declares the C target's exact Timer surface while scheduling remains explicit.

	`stamp` is the admitted monotonic elapsed-time service. Timer construction,
	scheduling, cancellation, and callback measurement retain their ordinary Haxe
	types so unsupported calls reach haxe.c diagnostics instead of disappearing at
	the frontend. A later event-loop task can implement those methods without
	changing the clock contract established here.
**/
extern class Timer {
	/** Declare periodic scheduling; the current C target does not implement it. */
	public function new(time_ms:Int):Void;

	/** Declare cancellation for the future scheduling adapter. */
	public function stop():Void;

	/** Declare the callback slot used by ordinary Haxe Timer code. */
	public dynamic function run():Void;

	/** Declare one-shot scheduling for the future event-loop adapter. */
	public static function delay(f:Void->Void, time_ms:Int):Timer;

	/** Declare callback measurement; direct stamp calls are admitted today. */
	public static function measure<T>(f:Void->T, ?pos:PosInfos):T;

	/** Return monotonic elapsed seconds from an unspecified host epoch. */
	public static function stamp():Float;

	/** Declare millisecond monotonic time; the current C target does not implement it. */
	public static function milliseconds():Int64;
}

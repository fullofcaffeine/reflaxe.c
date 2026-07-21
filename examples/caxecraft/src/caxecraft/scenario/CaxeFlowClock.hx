package caxecraft.scenario;

import haxe.Int64;

/**
	The fixed-tick arithmetic shared by scheduling and cooldown checks.

	Keeping these calculations here makes their signed 64-bit behavior directly
	testable without exposing the executor's mutable clock. `@:noCompletion`
	keeps this implementation helper out of ordinary editor suggestions; it does
	not weaken typing, visibility, or runtime behavior.
**/
@:noCompletion
final class CaxeFlowClock {
	public static inline function next(current:Int64):Int64
		return current + 1;

	public static inline function dueTick(current:Int64, delay:Int):Int64
		return current + Int64.ofInt(delay);

	public static inline function cooldownHasElapsed(current:Int64, previous:Int64, requiredTicks:Int):Bool
		return current - previous >= Int64.ofInt(requiredTicks);
}

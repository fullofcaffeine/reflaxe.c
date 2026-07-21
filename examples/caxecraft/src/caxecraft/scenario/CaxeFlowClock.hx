package caxecraft.scenario;

import caxecraft.scenario.CaxeFlowRuntime.FlowTick;

/**
	Exact fixed-tick arithmetic shared by scheduling and cooldown checks.

	A tick has two ordinary non-negative `Int` fields. `offset` advances inside a
	one-billion-tick epoch; carrying into `epoch` is checked before addition. The
	clock therefore crosses the signed 32-bit boundary without `haxe.Int64`, and
	its final boundary fails explicitly instead of wrapping. At 60 ticks/second,
	the admitted range lasts for more than one billion years.

	`@:noCompletion` keeps this implementation helper out of ordinary editor
	suggestions; it does not weaken typing, visibility, or runtime behavior.
**/
@:noCompletion
final class CaxeFlowClock {
	public static inline final TICKS_PER_EPOCH:Int = 1000000000;
	public static inline final MAX_EPOCH:Int = 2147483647;

	public static inline function start():FlowTick
		return {epoch: 0, offset: 0};

	/** True only for a clock value that can be restored without normalization. */
	public static inline function isValid(value:FlowTick):Bool
		return value.epoch >= 0 && value.epoch <= MAX_EPOCH && value.offset >= 0 && value.offset < TICKS_PER_EPOCH;

	/** Return the next exact tick, or null at the final representable value. */
	public static function next(current:FlowTick):Null<FlowTick> {
		if (current.offset < TICKS_PER_EPOCH - 1)
			return {epoch: current.epoch, offset: current.offset + 1};
		if (current.epoch == MAX_EPOCH)
			return null;
		return {epoch: current.epoch + 1, offset: 0};
	}

	/**
		Add one admitted positive 32-bit delay without overflowing either field.

		The loop executes at most twice because an authored delay cannot exceed
		2,147,483,647 while each epoch contains one billion ticks.
	**/
	public static function dueTick(current:FlowTick, delay:Int):Null<FlowTick> {
		if (delay <= 0)
			return null;
		var completeEpochs = 0;
		var remainder = delay;
		while (remainder >= TICKS_PER_EPOCH) {
			remainder -= TICKS_PER_EPOCH;
			completeEpochs++;
		}
		final offsetSum = current.offset + remainder;
		final carry = offsetSum >= TICKS_PER_EPOCH ? 1 : 0;
		final requiredEpochs = completeEpochs + carry;
		if (current.epoch > MAX_EPOCH - requiredEpochs)
			return null;
		return {
			epoch: current.epoch + requiredEpochs,
			offset: carry == 0 ? offsetSum : offsetSum - TICKS_PER_EPOCH
		};
	}

	public static inline function isDue(ready:FlowTick, current:FlowTick):Bool
		return ready.epoch < current.epoch || (ready.epoch == current.epoch && ready.offset <= current.offset);

	public static function cooldownHasElapsed(current:FlowTick, previous:FlowTick, requiredTicks:Int):Bool {
		final ready = dueTick(previous, requiredTicks);
		return ready != null && isDue(ready, current);
	}
}

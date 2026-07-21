/**
 * Reproduces Haxe's optimized temporary around a branch-valued lazy Boolean.
 *
 * The source is ordinary `||` and `&&`. Haxe inlines `mode` and rewrites the
 * first two operands into a temporary assigned by both sides of an `if` before
 * haxe.c sees the typed tree. The compiler must recognize that exhaustive
 * assignment, keep the final helper call lazy, and emit direct C without a
 * runtime or heap allocation.
 */
class FlowCarrierFixture {
	static var observedCalls:UInt = 0;

	static inline function alive(state:FlowCarrierState):Bool
		return state.health > 0;

	static inline function mode(state:FlowCarrierState):FlowCarrierMode
		return FlowCarrierMode.fromCode(state.modeCode);

	/** Pure helper used on the final right-hand side. */
	static inline function isImpactPhase(state:FlowCarrierState):Bool
		return state.phaseTicks == 1;

	/** Makes skipped versus evaluated right-hand paths observable at runtime. */
	static function observe(value:Bool):Bool {
		observedCalls++;
		return value;
	}

	static function carrierOr(state:FlowCarrierState):Bool {
		if (!alive(state) || mode(state) != FlowCarrierMode.Windup || observe(!isImpactPhase(state)))
			return true;
		return false;
	}

	static function carrierAnd(state:FlowCarrierState):Bool {
		if (alive(state) && mode(state) == FlowCarrierMode.Windup && observe(isImpactPhase(state)))
			return true;
		return false;
	}

	/** Returns two only when both lazy chains preserve skip and evaluation order. */
	static function run():UInt {
		observedCalls = 0;
		if (!carrierOr({health: 0, modeCode: 4, phaseTicks: 1}))
			return 90;
		if (!carrierOr({health: 1, modeCode: 3, phaseTicks: 1}))
			return 91;
		if (carrierOr({health: 1, modeCode: 4, phaseTicks: 1}))
			return 92;
		if (carrierAnd({health: 0, modeCode: 4, phaseTicks: 1}))
			return 93;
		if (carrierAnd({health: 1, modeCode: 3, phaseTicks: 1}))
			return 94;
		if (!carrierAnd({health: 1, modeCode: 4, phaseTicks: 1}))
			return 95;
		return observedCalls;
	}

	static function main():Void
		run();
}

private typedef FlowCarrierState = {
	final health:Int;
	final modeCode:Int;
	final phaseTicks:Int;
}

private enum abstract FlowCarrierMode(Int) to Int {
	var Resting = 0;
	var Chasing = 1;
	var Returning = 2;
	var Wandering = 3;
	var Windup = 4;
	var Recovering = 5;

	public static inline function fromCode(code:Int):FlowCarrierMode {
		if (code == 1)
			return Chasing;
		if (code == 2)
			return Returning;
		if (code == 3)
			return Wandering;
		if (code == 4)
			return Windup;
		if (code == 5)
			return Recovering;
		return Resting;
	}
}

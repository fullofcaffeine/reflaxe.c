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

	/** Non-inline projections make Haxe hoist prerequisites after its carrier. */
	static function itemCode(state:FlowCarrierState):Int
		return state.modeCode;

	static function itemQuantity(state:FlowCarrierState):Int
		return state.health;

	static function itemX(state:FlowCarrierState):Int
		return state.phaseTicks;

	static function itemY(state:FlowCarrierState):Int
		return state.health;

	static function itemZ(state:FlowCarrierState):Int
		return state.modeCode;

	static inline function validCode(code:Int):Bool
		return code >= 0 && code < 9;

	static inline function inside(x:Int, y:Int, z:Int):Bool
		return x >= 0 && x < 32 && y >= 0 && y < 16 && z >= 0 && z < 32;

	/** Ordinary inline arithmetic whose three lazy checks create two carriers. */
	static inline function near(actual:Float, expected:Float):Bool {
		final difference = actual - expected;
		return difference >= -0.000001 && difference <= 0.000001;
	}

	/** Reproduce consecutive compiler carriers before their assigning branches. */
	static function consecutiveInlineCarriers():Bool
		return near(0.0, 0.0) && near(2.0, 2.0) && near(-4.0, -4.0);

	/** Reproduces a carrier declared before the call result used to assign it. */
	static function carrierWithPrelude(state:FlowCarrierState):Bool
		return !validCode(itemCode(state)) || itemQuantity(state) <= 0 || !inside(itemX(state), itemY(state), itemZ(state));

	/** Keep the same carrier inside a loop, where its join must remain loop-local. */
	static function carrierWithPreludeLoop(state:FlowCarrierState):Bool {
		var index = 0;
		while (index < 1) {
			if (!validCode(itemCode(state)) || itemQuantity(state) <= 0 || !inside(itemX(state), itemY(state), itemZ(state)))
				return true;
			index++;
		}
		return false;
	}

	/** Stress the nested lazy joins produced by one realistic validation guard. */
	static function longValidationLoop(state:FlowCarrierState):Bool {
		var index = 0;
		while (index < 1) {
			final code = itemCode(state);
			final quantity = itemQuantity(state);
			final x = itemX(state);
			final y = itemY(state);
			final z = itemZ(state);
			if (code < 0 || code >= 9 || quantity <= 0 || x < 0 || y < 0 || z < 0 || x >= 32 || y >= 16 || z >= 32 || code == 8 || quantity > 6)
				return true;
			index++;
		}
		return false;
	}

	/** Two consecutive guards reproduce a loader that validates items then fluids. */
	static function combinedValidationLoops(state:FlowCarrierState):Bool {
		var itemIndex = 0;
		while (itemIndex < 1) {
			if (!validCode(itemCode(state)) || itemQuantity(state) <= 0 || !inside(itemX(state), itemY(state), itemZ(state)))
				return true;
			itemIndex++;
		}
		var fluidIndex = 0;
		while (fluidIndex < 1) {
			final code = itemCode(state);
			final quantity = itemQuantity(state);
			final x = itemX(state);
			final y = itemY(state);
			final z = itemZ(state);
			if (code < 0 || code >= 9 || quantity <= 0 || x < 0 || y < 0 || z < 0 || x >= 32 || y >= 16 || z >= 32 || code == 8 || quantity > 6)
				return true;
			fluidIndex++;
		}
		return false;
	}

	/** A future loop iteration must not hide this iteration's abrupt mismatch arm. */
	static function loopLocalNormalJoin(state:FlowCarrierState):Bool {
		var presentation = -1;
		var index = 0;
		while (index < 2) {
			final cell = itemCode(state);
			if (presentation < 0)
				presentation = cell;
			else if (presentation != cell)
				return true;
			index++;
		}
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
		if (!carrierWithPrelude({health: 0, modeCode: 4, phaseTicks: 1}))
			return 96;
		if (carrierWithPrelude({health: 1, modeCode: 4, phaseTicks: 1}))
			return 97;
		if (!carrierWithPreludeLoop({health: 0, modeCode: 4, phaseTicks: 1}))
			return 98;
		if (carrierWithPreludeLoop({health: 1, modeCode: 4, phaseTicks: 1}))
			return 99;
		if (!longValidationLoop({health: 0, modeCode: 4, phaseTicks: 1}))
			return 100;
		if (!combinedValidationLoops({health: 0, modeCode: 4, phaseTicks: 1}))
			return 101;
		if (loopLocalNormalJoin({health: 1, modeCode: 4, phaseTicks: 1}))
			return 102;
		if (!consecutiveInlineCarriers())
			return 103;
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

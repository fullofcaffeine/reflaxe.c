package layout;

import layout.math.Numbers;
import layout.model.State;
import layout.model.Left;
import layout.model.Right;
import layout.model.SoftRecord.SoftRecordApi;
import layout.platform.Device;

/** Small multi-package program used only to prove generated-C file layout. */
class Main {
	public static function trace():Int {
		return Numbers.twice(State.current()) + Device.offset() + Numbers.phaseValue(State.phase()) + Numbers.pointValue(Numbers.samplePoint())
			+ SoftRecordApi.read({
				soft: {value: 11}
			});
	}

	/**
		Keep both sides of the nullable class cycle reachable without allocating.

		This fixture owns header topology, not managed-class runtime selection.
		Nullable parameters force both pointer-shaped declarations into the C plan
		while `null` arguments keep the independent layout proof runtime-free.
	**/
	static function touchMutualTypes(left:Null<Left>, right:Null<Right>):Void {
		left;
		right;
	}

	static function main():Void {
		touchMutualTypes(null, null);
		trace();
	}
}

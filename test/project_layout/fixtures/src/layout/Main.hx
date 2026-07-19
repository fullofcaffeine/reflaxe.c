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

	static function touchMutualTypes():Void {
		var left = new Left();
		left;
		var right = new Right();
		right;
	}

	static function main():Void {
		touchMutualTypes();
		trace();
	}
}

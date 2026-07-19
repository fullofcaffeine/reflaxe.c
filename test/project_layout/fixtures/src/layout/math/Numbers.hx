package layout.math;

import layout.model.Phase;
import layout.model.Point;

/** A second package whose reachable definition must own its generated C unit. */
class Numbers {
	public static function twice(value:Int):Int {
		return value * 2;
	}

	public static function phaseValue(value:Phase):Int {
		return switch value {
			case Ready: 0;
		};
	}

	public static function pointValue(value:Point):Int {
		return value.x + value.y;
	}

	public static function samplePoint():Point {
		return {x: 2, y: 5};
	}
}

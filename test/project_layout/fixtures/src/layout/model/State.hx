package layout.model;

import layout.model.Phase;

/** Cross-module static storage exercises private extern/prototype planning. */
class State {
	public static var seed:Int = 7;

	public static function current():Int {
		return seed;
	}

	public static function phase():Phase {
		return Ready;
	}
}

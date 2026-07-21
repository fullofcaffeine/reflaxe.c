package caxecraft.gameplay;

/** Observable, compact movement modes for the first small hostile creature. */
enum abstract MosslingMode(Int) to Int {
	var Resting = 0;
	var Chasing = 1;
	var Returning = 2;
	var Wandering = 3;
	var Windup = 4;
	var Recovering = 5;

	public static inline function fromCode(code:Int):MosslingMode {
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

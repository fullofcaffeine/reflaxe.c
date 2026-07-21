package caxecraft.gameplay;

/** Observable, compact movement modes for the first small hostile creature. */
enum abstract MosslingMode(Int) to Int {
	var Resting = 0;
	var Chasing = 1;
	var Returning = 2;

	public static inline function fromCode(code:Int):MosslingMode {
		if (code == 1)
			return Chasing;
		if (code == 2)
			return Returning;
		return Resting;
	}
}

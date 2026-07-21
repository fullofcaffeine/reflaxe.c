package caxecraft.gameplay;

/** The three durable, compact conversation steps for Nia's first encounter. */
enum abstract GuidePhase(Int) to Int {
	var Waiting = 0;
	var Welcomed = 1;
	var SharedBerries = 2;

	/** Validate a stored carrier at the one gameplay boundary that owns it. */
	public static inline function fromCode(code:Int):GuidePhase {
		if (code == 1)
			return Welcomed;
		if (code == 2)
			return SharedBerries;
		return Waiting;
	}
}

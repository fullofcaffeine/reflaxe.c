package caxecraft.domain;

/** Observable work performed by one bounded water-simulation tick. */
typedef WaterTickResult = {
	final processed:Int;
	final changed:Int;
	final remaining:Int;
}

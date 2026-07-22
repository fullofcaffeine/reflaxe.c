package caxecraft.domain;

/**
	Renderer-independent swimming input sampled once for a fixed simulation tick.

	`jump` means rise while wet and keeps its ordinary land meaning when dry.
	`descend` is water-specific, so it stays out of the smaller `StepInput` record
	used by ordinary land movement.
**/
typedef AquaticInput = {
	final moveX:Float;
	final moveZ:Float;
	final jump:Bool;
	final descend:Bool;
}

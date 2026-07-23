package caxecraft.domain;

/**
	The storage owned by one `WaterSimulation` for its complete lifetime.

	The C build embeds one fixed byte array directly in the simulation object.
	Eval uses an ordinary Haxe array as the independent behavior oracle. Unlike
	`WaterPendingCells`, this value is an owner rather than a borrowed view: the
	queue marks cannot outlive or become detached from the scheduler counters
	that describe them.
**/
#if c
typedef WaterPendingOwner = c.CArray<c.UInt8, WorldVolume>;
#else
typedef WaterPendingOwner = Array<Int>;
#end

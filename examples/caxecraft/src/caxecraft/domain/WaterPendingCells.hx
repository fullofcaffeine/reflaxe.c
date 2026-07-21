package caxecraft.domain;

/**
	The compile-time storage seam for water cells waiting to be recomputed.

	The C build uses one fixed native byte per world cell. Eval uses an ordinary
	Haxe array as the behavioral oracle. A byte is only a queued/not-queued mark;
	the simulation chooses work by the lowest marked world index, so insertion
	order cannot change the result.
**/
#if c
typedef WaterPendingCells = c.Span<c.UInt8>;
#else
typedef WaterPendingCells = Array<Int>;
#end

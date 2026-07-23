package caxecraft.domain;

/**
	A caller-owned snapshot buffer for water cells waiting to be recomputed.

	The live queue belongs to `WaterSimulation`; this borrowed view is used only
	while copying queue marks into or out of save/test storage. The C build uses
	a span over fixed native bytes. Eval uses an ordinary Haxe array as the
	behavioral oracle. A byte is only a queued/not-queued mark.
**/
#if c
typedef WaterPendingCells = c.Span<c.UInt8>;
#else
typedef WaterPendingCells = Array<Int>;
#end

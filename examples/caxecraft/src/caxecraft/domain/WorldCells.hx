package caxecraft.domain;

/**
	Target-shaped storage seam for the otherwise ordinary Haxe domain.

	The C build borrows fixed one-byte storage. Eval uses an ordinary integer
	array as the differential oracle. Domain algorithms consume only this alias
	and `WorldStorage`, so raylib and platform state never enter the core.
**/
#if c
typedef WorldCells = c.Span<c.UInt8>;
#else
typedef WorldCells = Array<Int>;
#end

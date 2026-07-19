package caxecraft.domain;

/**
	The one compile-time target seam used by the gameplay algorithms.

	A verified `--custom-target c=...` build exposes Haxe's `c` conditional
	define. Haxe resolves `#if c` while compiling: this is not a runtime branch,
	and only the selected typedef is typed and emitted. Defining `c` manually is
	not a substitute for activating the custom target.

	The C branch borrows compact fixed one-byte storage through `Span<UInt8>`.
	The non-C branch uses an ordinary Haxe `Array<Int>` and is currently verified
	under Eval as the differential oracle. Other targets may be experimented with
	later, but the fallback alone is not a portability claim.

	Keep target representation choices here and in `WorldStorage`; world
	generation, edits, ray traversal, and collision must remain ordinary shared
	Haxe. See `docs/caxecraft-domain.md` for the complete portability plan.
**/
#if c
typedef WorldCells = c.Span<c.UInt8>;
#else
typedef WorldCells = Array<Int>;
#end

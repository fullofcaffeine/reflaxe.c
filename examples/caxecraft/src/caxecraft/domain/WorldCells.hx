package caxecraft.domain;

/**
	The one compile-time target seam used by the gameplay algorithms.

	A verified `--custom-target c=...` build exposes Haxe's `c` conditional
	define. Haxe resolves `#if c` while compiling: this is not a runtime branch,
	and only the selected typedef is typed and emitted. Defining `c` manually is
	not a substitute for activating the custom target.

	The C branch borrows compact fixed one-byte storage through `Span<UInt8>` so
	the bounded world can be backed by one ordinary native C byte array. This is
	both a useful representation and deliberate showcase evidence: the generated
	program needs no Haxe `Array`, heap allocation, collector, or `hxrt` for world
	storage. The non-C branch uses an ordinary Haxe `Array<Int>` and is currently
	verified under Eval as the independent behavioral oracle. Other targets may
	be experimented with later, but the fallback alone is not a portability claim
	and their native storage adapters need not imitate `CArray`.

	Keep target representation choices here and in `WorldStorage`; world
	generation, edits, ray traversal, and collision must remain ordinary shared
	Haxe. See `docs/caxecraft-domain.md` for the complete portability plan.
**/
#if c
typedef WorldCells = c.Span<c.UInt8>;
#else
typedef WorldCells = Array<Int>;
#end

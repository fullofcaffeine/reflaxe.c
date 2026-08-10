# Caxecraft outdoor world envelope

Status: implemented for the first connected outdoor chapter

Owner: `haxe_c-xge.20.1.8.1`

Measured: 2026-08-10 on Apple arm64 with Clang

## Outcome

Caxecraft uses a `64×16×32` physical world for the next outdoor chapter.
This space can hold Evergrove and Western Falls as two connected regions.

The level resolver admits authored maps at `32×16×32` and `64×16×32`.
The simulation uses the same larger storage for both sizes.

The game still loads CaxeMap files at launch. A content edit does not
require a new Haxe or C build.

This stage does not add world streaming. It also does not claim that the first
connected map is the complete campaign.

## Why the old world was too small

The old playable engine accepted only `32×16×32` maps. Each shipped Adventure
map uses that size, so each region looks and plays like a separate arena.

The CaxeMap validator and editor already understood larger dimensions. The old
playable engine rejected them because its storage and renderer used one fixed
size.

## Measured baseline

The generated C gives these exact structure sizes:

| Owner | Current size | Main fixed storage |
| --- | ---: | --- |
| `GameSession` | 33,968 bytes | 16 KiB terrain and 16 KiB water work |
| `TerrainChunkCache` | 196,816 bytes | four 49,152-byte face arrays |

The old renderer divided the world into sixteen `8×16×8` chunks. Its
checkerboard test proves a maximum of 3,072 visible faces per chunk.

The wider world has 32,768 cells and 32 chunks. The measured structures
have these sizes:

| Owner | Measured size | Change |
| --- | ---: | ---: |
| `GameSession` | 66,736 bytes | +32,768 bytes |
| `TerrainChunkCache` | 393,632 bytes | +196,816 bytes |

The level resolver and editor each use 32,768 temporary integer entries.
Generated C uses a 128 KiB element payload for each such array.

A full terrain rebuild inspects twice as many chunks. Ordinary block edits
still rebuild only the changed chunk and its direct neighbors.

## Options

### General runtime dimensions inside fixed maximum storage

This option keeps one large storage buffer and records a smaller active width,
height, and depth for each map.

This option can support maps of several sizes. It requires new dimensions in
world indexing, water, physics, raycasts, rendering, snapshots, and loading.
The larger physical renderer cache still needs a separate storage solution.

This design remains useful for a later editor and mod milestone. It is too
broad for the first connected chapter.

### One larger fixed envelope

This option changes the physical engine size to `64×16×32`. New outdoor maps
can use the complete size.

This option preserves simple world indexing, deterministic water work, and
fixed C storage. It also keeps the current chunk rebuild model.

The resolver keeps the current 32-wide maps valid. It adds an unbreakable
boundary at `x=32` and leaves the unreachable eastern storage empty.

This is the accepted option. It gives two current regions a shared world
without a new loading or save architecture. It also admits two authored widths
without adding dimensions to each gameplay operation.

### Region streaming

This option loads and removes world regions while the player moves.

Streaming requires region lifetime rules for terrain, water, actors, edits,
saves, rendering, and authored events. The first connected chapter does not
need that complexity.

Streaming remains deferred until measured content or memory evidence requires
it.

## Fixed-array limit

Haxe.c currently admits at most 65,536 bytes in one fixed C array. The larger
terrain cache needs 98,304 face entries for each stored face value.

The renderer uses two banks of sixteen chunks. Each bank keeps 49,152
entries for each face value, which remains inside the compiler limit.

The bank changes storage only. Chunk numbers, face order, invalidation, draw
submission, and the exact checkerboard limit remain the same.

## Why the envelope is not square

A `64×16×32` world places two current regions side by side. It doubles the
world area and does not multiply unused depth.

A `64×16×64` world has 65,536 cells. It would quadruple current terrain, water,
temporary level, editor, and renderer work before content needs that space.

## Evidence contract

The focused engine tests prove these facts:

- world indexing reaches the new eastern edge and rejects the next cell;
- terrain cache invalidation crosses the old `x=31` boundary;
- the exact per-chunk face limit remains valid in both cache banks;
- water work, raycasts, physics, and snapshots use the complete new volume;
- the resolver loads both admitted map widths into complete physical storage;
- a 32-wide map has a safe boundary at `x=32`;
- generated C keeps each fixed array at or below 65,536 bytes;
- a runtime-loaded map still launches without a Haxe or C rebuild.

The first broad content map will have separate evidence. One Piloscript journey
will protect its main route. Free play will evaluate art, pacing, and fun.

## Revisit conditions

Revisit this decision if one of these conditions becomes true:

- two useful outdoor regions do not fit without empty or compressed design;
- measured rebuild or frame time becomes unacceptable;
- creators need several playable world dimensions in one engine build;
- fixed storage prevents a required save, platform, or memory claim;
- a later finite-world plan has clear evidence for bounded region streaming.

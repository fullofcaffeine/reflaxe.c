# Caxecraft deterministic domain

Caxecraft is both a future Raylib game and a compiler end-to-end workload. Its
domain is kept independent of Raylib so renderer/input integration cannot make
world, ray, or collision semantics depend on frame rate, window state, native
handles, or platform facts. This document fixes the contract implemented by
`examples/caxecraft/src/caxecraft/domain`.

## Boundary and representation

The shared algorithm layer uses ordinary Haxe records, a fieldless Haxe enum,
functions, loops, and primitive arithmetic. It does not use raw C injection,
macros, reflection, `Dynamic`, `hxrt`, or a C-shaped game DSL.

`BlockKind` is the semantic material type: `Air`, `Grass`, `Dirt`, `Stone`, and
`Bedrock`. A material is not exposed as an arbitrary integer. `WorldStorage`
alone maps those cases to stable codes 0 through 4. The C carrier stores each
code in one `UInt8`; exact widening and explicit modulo narrowing go through
`c.IntConvert`. This gives the 32 x 16 x 32 world a 16 KiB cell payload.

`WorldCells` is the only target-shaped seam:

| Build | Carrier | Purpose |
| --- | --- | --- |
| generated C | borrowed `Span<UInt8>` over a fixed `CArray` | compact, bounded, allocation-free native storage |
| Eval oracle | `Array<Int>` | execute the same domain algorithms under the pinned reference target |

The alias is not a universal runtime abstraction. A later cross-target
experiment may supply another carrier, but the current authority is efficient,
direct C. The portability value comes from keeping semantics in ordinary Haxe
and isolating representation-specific operations, not from forcing every
backend through one universal IR or one runtime container.

The checked linear index is:

```text
x + 32 * (y + 16 * z)
```

Bounds are proven before this arithmetic. Therefore hostile negative or large
coordinates never enter a multiplication, and the largest admitted result is
16383. `query` returns `Air` outside the finite world for convenient read-only
gameplay queries; collision separately treats every out-of-bounds AABB cell as
solid, so a player cannot leave the world. `replace`, `remove`, and `place`
return explicit success. Bedrock and air cannot be removed, bedrock/air cannot
be placed by the gameplay operation, and placement requires an in-bounds air
cell.

Terrain generation first clears every cell, then derives one height in 3...7
for each x/z column from a stable integer hash. The bottom is bedrock, the top
is grass, the two cells below are dirt when present, and the remainder is
stone. All Haxe `Int` overflow-sensitive operations lower through the compiler's
reviewed wrapping/bit helpers rather than C signed-overflow behavior.

## Voxel ray traversal

`VoxelRaycast.trace` is a bounded Amanatides-Woo traversal. It returns an
explicit `hit` bit plus:

- the solid hit cell;
- the immediately previous visited cell;
- the outward unit face normal;
- the ray parameter at entry;
- the number of visited cells.

Callers that need world-space distance pass a normalized direction. An all-zero
direction is an explicit miss unless the origin cell is already solid. Starting
inside a solid is an immediate hit at distance zero, with the origin cell also
recorded as previous and a zero normal.

The origin cell is selected with mathematical floor, including negative
values. Zero-component axes are excluded from selection and never divide by
zero, even when another component is extremely small. A negative ray beginning
exactly on an integer boundary enters the lower neighbor at distance zero.
Maximum distance is inclusive: an entry at exactly the supplied bound may hit;
the first entry greater than the bound misses. Leaving the finite world is an
explicit miss.

Ties are resolved X, then Y, then Z. Each tied plane crossing is processed as
one adjacent-cell step before the next axis, which keeps the trace deterministic
and avoids jumping directly across an edge/corner. The traversal is monotonic
and capped at `WIDTH + HEIGHT + DEPTH + 8` visits, comfortably beyond every
possible in-world path without permitting an accidental infinite loop.

## Fixed-step player collision

`PlayerState` stores a feet-center position, three velocities, and grounded
state. The controller advances exactly 50 ms per call; wall clock and render
delta never enter the domain. Input is a small record containing horizontal
movement and a jump request. The future Raylib shell will sample real input and
feed a fixed-step accumulator rather than changing these semantics.

The player is an upright AABB with half-width 0.30 and height 1.80. Movement is
resolved X, then Z, then Y, allowing wall sliding. Each axis delta is divided
into steps no larger than 0.20 world units. This is the documented tunneling
policy: it catches the one-voxel obstacles exercised by normal movement and by
the deliberately oversized QA command without requiring a continuous swept
solver. A blocked downward Y move establishes grounded state and clears
vertical velocity; a small downward overlap probe retains stable floor contact.
A grounded jump begins at 6.5 units/second and gravity contributes -18
units/second squared on later fixed ticks. Ceiling contact clears upward
velocity.

An invalid spawn is lifted by whole blocks for at most the world height. The
operation never searches horizontally or allocates. If no free vertical
position exists, the returned state remains explicit and the next collision
step stays bounded; level construction is responsible for providing a valid
spawn column.

## Determinism and executable evidence

`DomainProbe.selfCheck` returns zero or the stable number of the first failed
case. Its named checks cover:

- minimum, maximum, hostile negative, and out-of-range indexing;
- axis-aligned, diagonal, negative, zero-direction, exact-boundary,
  inside-solid, maximum-distance, world-edge, hit, and miss DDA behavior;
- query/replace/remove/place rules including immutable bedrock;
- floor contact, jump, ceiling, wall sliding, corner/world boundaries,
  spawn recovery, and substep tunneling policy.

`CaxecraftTrace` adds stable terrain/edit/ray/collision hashes and 32 seeded
property traces. Each property seed regenerates terrain, performs a bounded
edit, traverses a ray, recovers a player spawn, and advances collision state.
Eval prints the canonical lines. Generated C performs no output itself; an
independent fixture-only C consumer calls the generated functions and prints
the same signed 32-bit values. The runner compares raw stdout bytes and reports
the first exact difference—there is no normalization allowance.

The production compiler output is compared byte-for-byte across isolated
roots, repeated cold requests, reversed typed-module discovery, another
available locale, and repeated warm compiler-server requests. Reflaxe's
`_GeneratedFiles.json` activity metadata is excluded; every normal compiler
artifact is included. Centrally owned snapshots retain the private header,
translation unit, runtime plan, compact method-symbol projection, and Eval
oracle.

Native evidence uses the shared argument-array C fixture harness. Available
identity-matched GCC and Clang compile and execute at O0, O2, and combined
ASan/UBSan settings. The reports retain compiler identity, normalized argument
arrays, input hashes, exits, stdout, and stderr. The test also inspects undefined
symbols in the generated translation unit's object file and rejects
`malloc`, `calloc`, `realloc`, `free`, or any `hxrt_` symbol. Inspecting that
object rather than a sanitizer-linked executable keeps the proof scoped to code
the compiler emitted; sanitizer support libraries legitimately allocate. The
compiler runtime plan must be the complete `hxc_runtime=none`
proof, the stdlib report must stay empty, and HxcIR must contain no runtime,
allocation, or cleanup instruction.

Run the complete contract with:

```sh
npm run test:caxecraft-domain
```

Regenerate reviewable evidence only after inspecting a semantic change:

```sh
npm run snapshots:update -- --suite caxecraft-domain
```

This evidence proves the bounded domain slice. It does not yet claim a playable
Raylib shell, infinite chunks, persistence, networking, general collection or
String support, a public C ABI, or cross-target equivalence beyond the declared
Eval oracle.

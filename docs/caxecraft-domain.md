# Caxecraft deterministic domain

Caxecraft is both a growing Raylib game and haxe.c's current flagship
product-level end-to-end workload. It is the main integrated QA path used to
find reusable compiler, generated-C, runtime, interop, and tooling
improvements—not an example that may paper over those problems locally. Its
domain is kept independent of Raylib so renderer/input integration cannot make
world, ray, or collision semantics depend on frame rate, window state, native
handles, or platform facts. This document fixes the contract implemented by
`examples/caxecraft/src/caxecraft/domain`. The player-facing modes, story,
Ivvy companion, editor, localization, art direction, and release definition
live in the [Caxecraft game design document](caxecraft-game-design.md).

## Compile-time target seam: why `#if c` exists

Haxe conditional compilation is used here to select a representation at the
edge of the domain without turning the domain itself into a lowest-common-
denominator framework. In a conforming production invocation:

```text
haxe ... --custom-target c=<generated-directory>
```

the pinned Haxe compiler invokes this target's activation hook and exposes the
public `c` define after validating the custom-target platform contract. Haxe
resolves `#if c` during compilation. The inactive branch is not a runtime path:
its imports and expressions are excluded before the selected program is typed
and emitted. Consequently the generated C contains no `if (target_is_c)` test,
no dormant `Array` fallback, and no cost for the conditional.

Application code must test `#if c`, not the internal `reflaxe_c` marker or the
path-bearing `c_output` transport. Manually supplying `-D c` does not activate
or validate the custom target and is not a supported shortcut. The activation
contract is documented in [configuration](configuration.md#target-activation-and-native-tuple).

### Exact current conditional inventory

| Location | C branch | Non-C branch | Why the difference is admitted |
| --- | --- | --- | --- |
| `WorldCells.hx` | `Span<UInt8>` | `Array<Int>` | C needs compact fixed native storage; the oracle needs an ordinary Haxe carrier. |
| `WorldStorage.hx` | exact `UInt8` widening and explicit narrowing | direct `Int` array access | C ABI-width conversions must remain explicit without infecting algorithms. |
| `CaxecraftTrace.hx` | fixed `CArray<UInt8, WorldVolume>` backing | freshly filled Haxe array | Each trace needs owned test storage, but all operations after construction stay shared. |
| `DomainProbe.hx` | fixed storage and no Haxe `Sys` output | Haxe array and printed oracle lines | The C slice is runtime-free, so an independent fixture C consumer prints returned values. |

The complete `caxecraft.app` source set is also guarded by `#if c`. That is a
different kind of seam: the whole window/input/render adapter exists only for
the C application because it speaks directly to Raylib's C ABI. Shared world,
ray, edit, and physics algorithms do not import that adapter. A future browser
or Rust presentation should supply its own narrow application adapter rather
than putting `#if c` tests inside gameplay or frame-by-frame control flow.

The import guards are part of the same boundary: non-C compilation never sees
the C-only carrier imports. The repeated storage setup in the trace is visible
on purpose; it makes the representational difference reviewable rather than
hiding it in an untyped factory or runtime service.

No `#if c` appears in `World`, `VoxelRaycast`, or `PlayerPhysics`. Bounds,
terrain, edits, material rules, DDA traversal, collision, and fixed-step state
updates are one ordinary Haxe implementation. That placement is the central
architectural rule: select carriers and platform services at a narrow edge,
then pass typed data into shared algorithms.

### What the non-C branch proves—and does not prove

The fallback uses standard Haxe `Array<Int>` and is currently executed under
the exact pinned Eval target. Comparing its traces with generated C proves the
declared domain behavior for the exercised inputs, including integer results,
floating calculations used by ray/collision code, iteration order, and edit
rules. Eval is a differential oracle, not a second shipped Caxecraft platform.

Because the fallback is ordinary Haxe, it is a useful starting point for other
targets. It is not evidence that JavaScript, Rust, Go, or another backend can
already build the complete game. Those targets may differ in numeric edge
behavior, storage layout, ownership, clocks, input, rendering, resource
lifecycle, standard-library coverage, and backend maturity. The Raylib shell is
specifically a C adapter and is not made portable by the array fallback.

### Long-term cross-target plan

The C showcase remains first and authoritative. It should emit direct,
inspectable raylib calls, use `CArray`/`Span` and exact ABI types where useful,
avoid steady-state allocation, and never accept a slower generic abstraction
merely to make a hypothetical port look uniform. Once that path is playable,
measured, and covered by headless QA, the separate cross-target experiment will
proceed in these stages:

1. **Freeze observable domain behavior.** Retain fixed seeds, recorded inputs,
   fixed timesteps, state hashes, and explicit floating-point tolerances. These
   fixtures—not a shared backend IR—are the portable semantic contract.
2. **Name four adapter boundaries.** Keep storage/allocation, clock and input,
   window/rendering, and native-resource/build ownership separate. Raylib owns
   the first C presentation adapter; another target may choose a genuinely
   native library or browser API.
3. **Add target carriers at the edge.** A JavaScript experiment may prefer a
   typed array; Rust may prefer its backend's array/slice representation. Use
   separate small adapter modules or target-specific source roots when that is
   clearer than adding more branches to shared files.
4. **Run semantic differential traces before sharing APIs.** Compare world,
   ray, edit, movement, and collision state independently of pixels. Report
   integer, float, iteration, and failure differences rather than normalizing
   them away.
5. **Keep presentation idiomatic per target.** Input events, graphics commands,
   GPU resources, and cleanup remain target-owned. Cross-platform state does
   not require a universal renderer, runtime, or Reflaxe IR.
6. **Extract only proven common abstractions.** After at least two working
   adapters expose the same stable Haxe-level contract, move that smallest
   contract into shared code. Until then, duplication at a narrow target edge
   is safer than a speculative framework.

The preferred end state is mostly conditional-free shared domain source plus a
small set of explicit adapter modules. A target conditional is still reasonable
for a closed compile-time type alias or exact conversion that cannot be chosen
at runtime. It should be replaced or isolated further when branches begin to
contain algorithms, acquire target-specific side effects, multiply beyond a
small closed set, or require different ownership/lifetime rules.

### Guardrails for future changes

- A target branch may choose representation, conversion, test plumbing, or a
  platform adapter; it must not silently change gameplay rules.
- Every new `#if c` site needs a documented reason and differential evidence.
  Prefer adding an operation to the existing adapter over branching at each
  call site.
- C-only imports and `c.*` ABI types stay inside the selected branch or a
  C-specific adapter module. They must not become fake portable types.
- The fallback must not be described as allocation- or performance-equivalent:
  its current job is semantic oracle execution.
- Renderer screenshots are target evidence. Shared state traces are the
  cross-target oracle; GPU pixels are not assumed identical.
- HxcIR remains the C compiler's semantic lowering contract. Portable game
  semantics live in Haxe source and tests, so another backend need not consume
  HxcIR to reuse the domain.

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
artifact is included. Centrally owned snapshots retain the default
source-shaped tree: the common types header, package/module headers and
translation units, umbrella header, small entry and support sources, runtime
plan, compact method-symbol projection, a schema-validated maintainability
report, and the Eval oracle. Unity keeps its complete header, implementation,
and report as separate exact snapshots. The reports are computed again from the
checked-in C rather than trusted as prose. They bound file/function complexity,
identifier length, and compiler-temporary pressure; require declared ownership
and source identity; and reject unexplained encoded names, digests, labels, and
`goto` statements.

Generated locals and record members retain the short compiler prefix while
keeping the source word: `x` becomes `hxc_x`, for example. This is intentionally
slightly different from a handwritten private struct. C expands preprocessor
macros before it understands local or member scope, so bare generated names can
be corrupted by current or future macros from raylib or another included header.
The prefix makes that impossible without returning to digest-heavy names.

The old counter reported 356 globally unique temporary spellings. That number
was retired because the same spelling can be declared in several functions and
because a text search can mistake comments or string contents for C syntax. The
lexer-backed report instead sees 53 functions, 514 actual `hxc_tmp_`
declarations, 757 non-declaration references, and 262 declarations per 1,000
function code lines. The longest identifier is 50 bytes and the generated
domain has no `goto`. Split mode owns 22 analyzed C/header artifacts and 2,410
physical lines; unity owns two artifacts and 2,303 physical lines while
retaining the same 53 function metrics. The QA-only `DomainProbe.selfCheck`
function and the unity implementation have exact, named size overrides rather
than wildcard exemptions. These measurements describe this fixed corpus, not a
claim that every Haxe program has the same readability.

The default `split` tree mirrors the Haxe module ownership under
`include/hxc/modules/` and `src/modules/`. It exists so generated C is
navigable, diffable, incrementally compilable, and easier to inspect beside
the Haxe modules that own it. The bounded domain now passes the structural
control-flow, human-oriented naming, conservative temporary-cleanup, and
[generated-C maintainability](generated-c-maintainability.md) gates. This is
strong evidence that this particular generated tree remains navigable; it is
not a claim that arbitrary Haxe output is already a comfortable handwritten-C
handoff. “Conservative” matters here: a temporary is removed only when the
compiler can prove that moving its expression to the use site cannot change
evaluation order, aliasing, failure, or lifetime behavior.
The optional `unity` layout puts the same declaration/function plan in
`src/program.c` for single-file inspection or simple build systems. The runner
also checks `package`, which combines the domain and QA modules into one
header/source pair per Haxe package while preserving function-level source
ownership. It compares HxcIR, runtime intent, externally visible symbols, and
behavior across all three layouts. Layout selection is therefore not allowed
to become a second semantic pipeline.

Native evidence uses the shared argument-array C fixture harness. Available
identity-matched GCC and Clang compile and execute at O0, O2, and combined
ASan/UBSan settings. The reports retain compiler identity, normalized argument
arrays, input hashes, exits, stdout, and stderr. The test also inspects undefined
symbols in every generated module/support object and rejects `malloc`,
`calloc`, `realloc`, `free`, or any `hxrt_` symbol. Inspecting compiler-emitted
objects rather than a sanitizer-linked executable keeps the proof scoped to
code the compiler emitted; sanitizer support libraries legitimately allocate. The
compiler runtime plan must be the complete `hxc_runtime=none`
proof, the stdlib report must stay empty, and HxcIR must contain no runtime,
allocation, or cleanup instruction.

Run the fast edit-time contract with:

```sh
npm run test:caxecraft-domain
```

It runs Eval, all three layouts, exact split/package/unity snapshots,
semantic-layout parity, and one strict optimized native differential for each
layout. The exhaustive lane additionally repeats the real split and package
outputs across cold builds, reversed discovery order, an alternate locale, and
warm compiler-server reuse, then runs the complete native matrix used by CI:

```sh
npm run test:caxecraft-domain:full
```

Regenerate reviewable evidence only after inspecting a semantic change:

```sh
npm run snapshots:update -- --suite caxecraft-domain
```

This evidence proves the bounded domain slice. A separate registered playable
snapshot proves the current C adapter emits direct Raylib window, input, and
draw calls with no `hxrt`, allocation call, or generated `goto`; the documented
native command also compiles and links it against pinned Raylib. Neither proof
claims infinite chunks, persistence, networking, general collection or String
support, a public C ABI, optimized rendering, or cross-target equivalence
beyond the declared Eval oracle.

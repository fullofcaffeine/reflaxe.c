# Caxecraft

Caxecraft is the repository's small original voxel-sandbox Raylib showcase and
haxe.c's current flagship product-level E2E/QA workload. It is expected to find
reusable compiler and tooling improvements, not hide their absence behind
game-only workarounds. The repository now contains a first playable feasibility
slice: a finite voxel world, authored terrain and water, block edits, voxel raycasting,
fixed-step first-person collision, and a C-specific Raylib window/input/render
adapter. Its authored spawn meadow now contains a friendly Nia interaction,
one-time berry gift, and a Mossling that notices, chases, and returns on the
fixed clock. It also wanders on a deterministic home route, warns before one
bounded attack, and pauses to recover afterward. Adventure can strike that
Mossling with a fixed-step Copper Sword, collect its visible berry drop, and
return to the meadow after defeat. Selecting berries and using the secondary
action consumes one
bundle only when it can restore one heart. A full stack preserves Nia's gift,
any uncollected part of a world drop, and a mined block whose matching stack
cannot accept it. The complete
Creative/Adventure/editor/Ivvy direction and its honest
present-versus-planned gates live in the
[game design document](../../docs/caxecraft-game-design.md).
Creators and prospective modders should start with the concise
[creator and modding guide](../../docs/caxecraft-modding.md). It explains which
facts belong to CaxeMap, CaxeFlow, content definitions, Haxe mechanics, or test
pilots, and clearly marks the authoring pieces that are not connected to the
native playable yet.

The staged engine structure is recorded in the
[runtime architecture guide](../../docs/caxecraft-architecture.md). `Main` is
now only the executable handoff. `CaxecraftApp` owns the Raylib lifetime and
outer frame loop, while `GameSession` owns the admitted fixed-step simulation
slice. Their run/tick bodies still contain named migration seams;
`haxe_c-xge.20.4.2` owns moving those pieces behind smaller input,
presentation, content, and system boundaries without changing behavior.
The beginner-friendly
[Haxe code architecture guide](../../docs/haxe-code-architecture.md) explains
why that design uses records, nominal abstracts, enums, module functions, and
real class instances in different places, with TypeScript comparisons and
simplified examples of the C that haxe.c selects.

The core is ordinary Haxe by design. This is the default for gameplay,
simulation, content, localization, and editor code: haxe.c should preserve the
familiar Haxe operation and select efficient C, rather than make application
code imitate C manually. Typed low-level APIs belong at focused boundaries
where representation really matters, such as Raylib interop, fixed hot-path
buffers, borrowed views, or measured performance-sensitive world storage.

Today, `WorldCells` and `WorldStorage` form the main-world storage boundary:
the C build uses a stack-backed `CArray<UInt8>`/`Span<UInt8>` while Eval uses
`Array<Int>`. The water scheduler uses the same focused pattern for its fixed
pending-work buffer. Those narrow adapters keep gameplay readable and give the
compiler realistic, allocation-free C workloads without hiding C behind a
portability framework. The Raylib-facing application modules separately use
target conditionals where the foreign ABI requires C-specific value types.
Choosing the `metal` profile does not change ordinary Haxe semantics:
`Array.copy()`, for example, must still produce a distinct shallow-copied Array.
The source chooses a visible typed `c.*` carrier when it genuinely wants
C-shaped storage or borrowing. The broader rule and current admitted surfaces
are documented in
[the typed C authoring contract](../../docs/typed-c-authoring.md#choosing-haxe-or-c-shaped-semantics).

Reusable content now has one checked-in source of truth:
[`packs/caxecraft/base/content.json`](packs/caxecraft/base/content.json). It
defines the current blocks, items, actors, behavior profiles, drops, effects,
and logical visuals. A strict build-time validator rejects malformed or
unresolved data and generates a typed Haxe adapter; CaxeMap and the editor use
that same registry. The exact JSON is also packaged with the executable. This
is not yet arbitrary runtime mod loading—the generated adapter is a temporary
bridge until native JSON and filesystem support can load the same format.

The shared domain also contains the first deterministic water simulation:
authored sources fall, spread with bounded strength, leak through openings,
recede when removed, and repair after a dam closes. It already runs as the same
ordinary Haxe under Eval and generated C. The next domain slice also implements
continuous submersion, wading/floating/submerged states, swimming forces,
buoyancy, breath, drowning requests, and generic equipment capabilities. Both
focused lanes pass Eval/native parity and sanitizer checks. The native playable
now loads its terrain, pond, and player spawn from a typed adapter generated by
the validated CaxeMap. That adapter is a bounded experiment: it proves the
validated authored model can drive the native loop, but it is not the intended
loading architecture. `haxe_c-xge.39` owns replacing it with runtime CAXEMAP
loading so the game and editor can open changed content without invoking Haxe.
The playable advances water on the fixed clock, draws translucent surfaces,
swims, applies Adventure drowning damage, fades the underwater camera, and shows
fixed-tick breath bubbles. `WaterSnapshot` preserves mutable water for the future
complete save format. Tideweave pickup/equipment is connected through validated
content and the deterministic pilot protocol; its display-backed visual
checkpoint is still pending on a runner with a usable desktop session. Audio,
the broader actor/rule composition, and native runtime map loading remain later
work.

After editing the built-in pack, run:

```sh
python3 examples/caxecraft/content_pack.py
npm run test:caxecraft-content-pack
npm run test:caxecraft-water
npm run test:caxecraft-aquatics
npm run test:caxecraft-terrain-chunks
```

## Why the source contains `#if c`

`c` is a Haxe compile-time define exposed by the verified
`--custom-target c=<output>` activation. Haxe resolves `#if c` before normal
typing and code generation, so it is not a runtime platform check and adds no
branch to the generated executable. Only the selected imports, types, and
expressions exist in that compilation. Passing `-D c` manually does not start
the C target and is not a supported build.

It helps to read the directive as “choose one source shape for this build,” not
as an `if` statement. For example, `WorldCells` is effectively one of these two
programs after Haxe has selected the target:

```haxe
// Production C build: a borrowed view over fixed native bytes.
typedef WorldCells = c.Span<c.UInt8>;

// Eval oracle build: an ordinary managed Haxe collection.
typedef WorldCells = Array<Int>;
```

The compiler never tries to unify both storage types in one executable. The C
build does not contain the fallback, and Eval never needs to understand the C
ABI types. This differs from a TypeScript runtime check such as
`if (platform === "c")`: there is no target string, dormant branch, or per-frame
decision in generated C.

Why choose different storage at all? The world is finite and its block codes
fit in one byte. The C showcase can therefore prove a direct fixed byte array,
borrowed safely as a span, with no heap allocation, garbage collector, or
general array runtime for that buffer. Eval's `Array<Int>` is intentionally the
simplest independent carrier for executing the same algorithms and comparing
their results. It is a semantic oracle, not a claim that its memory layout or
performance matches C. This seam demonstrates Haxe's “metal” option while the
gameplay on either side remains normal high-level Haxe.

The current conditionals are deliberately narrow:

- `WorldCells` selects compact borrowed `Span<UInt8>` storage for C and an
  ordinary `Array<Int>` fallback for the Haxe oracle;
- `WorldStorage` performs the exact C integer conversions required by that
  carrier;
- `CaxecraftTrace` and `DomainProbe` construct fixed C storage or ordinary Haxe
  test storage and choose the appropriate output plumbing;
- `WaterPendingOwner` selects the live queue embedded in `WaterSimulation`,
  while `WaterPendingCells`, the tiny storage adapter, and `WaterProbe` select
  short-lived snapshot/test views over the same deterministic marks.

Terrain generation, block rules, DDA picking, player collision, and trace
hashing contain no target conditional. The non-C path is tested under pinned
Haxe Eval; it is useful portability evidence, but it does not by itself prove
the game on JavaScript, Rust, or every Haxe target.

The interactive Raylib window, input, and renderer are a C-specific adapter
around that shared domain. Guarding the whole adapter with `#if c` means a
non-C target never tries to type Raylib's C ABI; it does not put a platform
branch inside the game loop. A later Rust/JavaScript experiment will provide
target-appropriate storage, clock/input, rendering, and resource adapters and
compare the same deterministic domain traces. We will not grow
target branches throughout gameplay or impose one universal rendering API/IR.
Only abstractions demonstrated by at least two working adapters should move
into shared code. The detailed boundary rules, limitations, and migration
sequence live in [the domain design](../../docs/caxecraft-domain.md).

A new `#if c` is justified only when a compile-time representation, ABI, native
resource, or platform service truly differs. It is not a shortcut for missing
compiler support and must not duplicate game rules. If both branches start
implementing terrain, combat, quests, or editor behavior, the seam is in the
wrong place: that behavior belongs in shared typed Haxe, with the differing
storage or platform operation moved behind a small adapter.

From the repository root, run the fast development proof:

```sh
npm ci
npm run test:caxecraft-domain
```

## Build and play

The first run may download the exact 52,562,578-byte Raylib 6.0 source archive
recorded in the repository lock. Network use is never implicit:

```sh
npm run caxecraft:play -- --allow-network
```

After that verified archive and native build are cached, use:

```sh
npm run caxecraft:play
```

The command compiles `play.hxml` through `--custom-target c=...`, validates the
direct-C manifest and exact selective `hxrt` plan, reuses or builds the pinned
Raylib static library, then compiles, links, and launches the generated split C
project. The game now exercises ordinary managed Haxe arrays, strings, maps,
recursive enums, and escaping classes, so `hxc_runtime=auto` includes only the
runtime features proven reachable rather than claiming a runtime-free build.
It never consults an undeclared sibling checkout or writes hand-authored C into
the generated project.

After a successful interactive build, the launcher records which exact inputs
produced the game and hashes every output it may reuse. A second unchanged
`caxecraft:play` checks that record, the complete Haxe/compiler/runtime/content
input inventory, the native tool identities and foreign headers/libraries, the
generated C project, staged files, and executable. Only an exact match skips
Haxe, haxe.c, C compilation, and linking. A local diagnostic sample validated
3,067 files in 0.31–0.91 seconds and reached launch preparation in 1.09
seconds. The host was busy, so those figures show that the fast path meets its
intended order of magnitude; they are not an authoritative percentile
benchmark.

The record is a local optimization, not build authority. A source edit, new
module, changed define or layout, compiler/runtime/binding change, content
change, native-tool or library change, missing output, or altered executable
prints the first detected miss and runs the normal build. A failed requested
build exits as a failure; it never presents the previous executable as if it
contained the edit.

Use these explicit escape hatches when measuring or diagnosing the build:

```sh
# Rebuild, but keep the normal Haxe compilation-server policy.
npm run caxecraft:play -- --no-build-cache

# Authoritative cold path: bypass executable reuse and force a fresh Haxe process.
npm run caxecraft:play -- --cold

# Rebuild through a fresh process without changing the other cold-path choices.
npm run caxecraft:play -- --haxe-server off --no-build-cache

# Stop only this worktree's exact auto-owned Haxe server.
npm run caxecraft:play -- --stop-haxe-server

# Rebuild all native objects through the uncached reference path.
npm run caxecraft:play -- --build-only --native-cache off
```

An existing `HAXE_NO_SERVER=1` also bypasses executable reuse because it
expresses a request for fresh-process evidence. Deterministic pilots, snapshot
checks, sanitizer runs, and compile/build-only modes retain their own explicit
behavior and never claim an interactive unchanged-build hit.

Interactive and deterministic pilot builds use separate marked output
directories. Changing a pilot define therefore cannot leave stale generated
files in another profile.

Each changed Haxe build also gets a private generated-project transaction.
After the normal Reflaxe ownership and generated-C checks pass, the launcher
renames it into a content-addressed `generations/` directory and atomically
updates a JSON pointer. The previously selected project is never edited in
place. This makes a compiler interruption safe and gives the planned native
object cache stable source bytes to depend on. The generated folders are
internal build evidence rather than multiple game editions; `layout`, Raylib
configuration, sanitizer, pilot, and renderer choices still use separate
variant folders because those choices genuinely produce different C or native
behavior.

Native compilation consumes those immutable projects through compiler-produced
dependency files. On a hit, every header in the previous dependency closure is
re-hashed and the ordered include roots are checked for newly shadowing paths;
an object is never reused merely because its `.c` file looks unchanged.
Independent misses compile in a bounded pool, then a separate link key checks
the ordered objects and exact Raylib/Raygui libraries. The launcher prints the
object hit/miss count and whether the link was reused. `--native-jobs` adjusts
the bounded worker count, while `--native-cache off` remains the clean native
reference.

Useful non-interactive forms are:

```sh
# Haxe -> C only; no Raylib provisioning or window
npm run caxecraft:play -- --compile-only

# Reuse the already-generated project, produce the native executable, and do
# not open a window. Run compile-only or normal play first after Haxe changes.
npm run caxecraft:play -- --build-only

# Open the real Raylib renderer, capture the textured title, and exit automatically
npm run caxecraft:play -- --smoke

# Exercise movement/editing/hotbar input, capture the game view, and exit
npm run caxecraft:play -- --pilot move-jump-edit

# Exercise the same real game through Raylib's deterministic in-memory
# software renderer; no window or GPU is required.
npm run caxecraft:play -- \
  --raylib-configuration memory-software \
  --pilot move-jump-edit

# Linux only: instrument generated C with AddressSanitizer and
# UndefinedBehaviorSanitizer while running the same real Raylib pilot.
npm run caxecraft:play -- \
  --raylib-configuration memory-software \
  --pilot move-jump-edit \
  --sanitizers

# Linux/macOS headless comparison: run the chunk cache, former complete-world
# Haxe loop, and focused handwritten-C reference under the same Raylib build
# and 2-warmup/12-measured-frame protocol.
npm run benchmark:caxecraft-renderer

# Prove authored Tideweave pickup, aquatic-profile equipment, and HUD feedback
npm run caxecraft:play -- --pilot aquatic-gear

# Prove real move-and-jump rendering between varied display intervals
npm run caxecraft:play -- --pilot smooth-motion

# Prove the generated native application enters and presents the raygui editor
npm run caxecraft:play -- --pilot editor-shell

# Verify every source byte in an already extracted Raylib tree
npm run caxecraft:play -- \
  --authority offline-source \
  --source /absolute/path/to/raylib-6.0
```

`--build-only` is the focused C-toolchain loop. It does not invoke Haxe: it
loads the existing generated manifest and runtime plan, reruns all generated-C
shape checks, validates every manifest-owned include directory and source, then
compiles and links those artifacts. This makes a native linker or binding
iteration cheap while failing closed on missing or malformed generated output.
It cannot infer whether a Haxe source changed after the project was generated,
so use `--compile-only` (or normal play) first whenever Haxe input, defines,
compiler code, or content-generation inputs changed.

Use `WASD` to move, the mouse to look, Space to jump, and left click to use the
selected item's primary action: remove a block normally or strike while the
Copper Sword is selected. Right click uses the selected item's secondary
action: it places a selected block or eats selected berries when health is not
full. Number keys `1` through `8` or the
mouse wheel to select the hotbar, Escape to pause or release the cursor, and
`Q` to quit. Creative mode does not consume placed blocks; the current
Adventure feasibility path has finite stacks and returns eligible mined blocks
to them. It checks room before removal, so a full matching stack leaves the
block in the world and shows a capacity message. Losing focus pauses and releases the cursor; clicking the paused
window captures it again without also mining. The window may be resized down
to 800 by 450.

Movement uses a deterministic 50 ms simulation tick, but the camera does not
jump only twenty times per second. `MotionInterpolation` keeps the previous
and current committed positions and draws between them using the leftover
fraction of the frame accumulator. Collision, selection raycasts, edits,
telemetry, and saves still read the current committed character. Presentation
therefore becomes smooth without making gameplay depend on GPU or monitor
speed. The visual copy resets after pause/focus changes, start or mode changes,
spawn recovery, and future teleports so it never sweeps through an invalid
path. The focused cross-target check is:

```sh
npm run test:caxecraft-presentation
```

It proves endpoints, midpoint, clamping, adjacent fixed ticks, and teleport
reset on Eval and generated native C with sanitizers and no `hxrt` or heap.
The `smooth-motion` native pilot additionally walks and jumps through the real
outer loop at deterministic 8, 17, and 25 ms display intervals. It requires a
camera sample strictly between committed positions and an airborne final
gameplay state, proving that the jump was consumed. Control feel still requires the real
interactive smoke below; automated timing evidence does not pretend to grade
whether movement feels pleasant to a person.

Before closing the playable-slice Bead, record one human smoke pass with the
OS, C compiler, window sizes, and pass/fail result. Check all of these behaviors:

- launch and quit with `Q` and the window close button;
- mouse look, `WASD`, gravity, collision, and jumping;
- block selection, removal, valid placement, and rejected player-overlap placement;
- Escape release/pause, click recapture without click-through, and focus-loss pause;
- resizing at 800 by 450, 1280 by 720, and one larger comfortable size;
- readable terrain edges, selection outline, crosshair, counters, and status text.

The `--smoke` mode is the unattended title/presentation check. It runs the
generated native executable, captures a completed framebuffer, and exits
within a 15-second wall-clock limit plus the in-game frame limit. The validator
checks PNG structure, the admitted logical/Retina dimensions, and independent
wordmark/panorama color regions. The movement pilot deliberately aims at its
edited block, so it requires a real terrain/HUD scene and exact
pick/remove/place telemetry instead of forcing the sun to remain visible.
Other near-horizon gameplay pilots also require sky and sun regions. Linux
desktop CI uses a virtual display—an in-memory
desktop for graphical tests without a monitor. These checks catch renderer
stalls, blank frames, missing assets/terrain/HUD, and framebuffer-size mistakes;
they do not judge control feel or artistic quality. See the
[game-pilot guide](../../docs/caxecraft-game-pilot.md).

The separate Linux memory/software lane needs no display at all. It reuses the
checksum-verified Raylib build, runs the real movement/edit game twice, compares
the reports, and preserves a normalized software-rendered review image. The
normalization corrects the pinned backend's screenshot orientation/channel
convention; it does not alter game state or desktop rendering.

Raylib's pinned memory platform has a fixed framebuffer and deliberately does
not implement `SetWindowSize`. Use the desktop/Xvfb lane for the
`resize-layout` pilot; the runner rejects that unsupported software-backend
combination before invoking Haxe.

The required headless CI job runs isolated GCC and Clang sanitizer variants.
They instrument all generated game C and the final link, fail on the first
memory/undefined-behavior error, retain stack-trace frame pointers, and check
leaks. The pinned Raylib static library is verified and exercised but is not
itself rebuilt with sanitizer compiler flags in this bounded lane.

## Textured terrain without distance pop-in

`BlockKind` remains gameplay data; it does not contain texture filenames or
atlas coordinates. `TerrainAtlas` is one typed presentation table that chooses
the owning image and outward-face picture for grass, soil, stone, foundation,
sand, logs, leaves, snow, and ash. Its focused Eval probe checks those choices
and the half-pixel texture inset through `npm run test:caxecraft-gameplay`.

The C renderer divides the complete current 32 by 32 world into sixteen
8-by-16-by-8 rendering chunks. It discovers faces that touch a non-solid cell
when a chunk is dirty, stores those faces in fixed byte arrays, and reuses them
on unchanged frames. A successful edit dirties its owner plus any chunk that
shares the edited boundary. It then places the cached faces inside at most one
texture-bound `rlgl` quad region for each of the two ready terrain atlases.
Here `rlgl` means Raylib's low-level drawing layer: the game supplies vertices
and texture coordinates, while Raylib owns the graphics buffer and safely
flushes it at quad boundaries when full. Generated C calls the seven selected
`rl*` functions directly. It has no per-block `DrawCube`, heap allocation,
raw-C injection, or `hxrt` dependency.

Those coherent regions remove the old moving nine-block render radius, so blocks no
longer appear and disappear beside the player. Face-direction shading and two
distant color bands soften the finite edge; a simple sun gives the sky a stable
landmark. The real movement pilot requires a presented, color-rich terrain
frame, exact cache/rebuild counters, and a bounded exit. The focused
`test:caxecraft-terrain-chunks` command reaches the exact checkerboard capacity,
checks same-chunk and cross-boundary edits under Eval and generated C, and runs
the native C under sanitizers when available. The current cache still replays
vertices every frame; it does not claim GPU-resident chunk meshes or arbitrary-
distance streaming. Those resource-owning techniques require separate
measurement and cleanup design.

The pinned Raylib build uses its bundled GLFW backend on every desktop and its
ordinary frame loop. In that mode `EndDrawing` puts the completed frame on
screen, limits the frame rate, and polls window/input events. Raylib also offers
custom frame control for applications that perform those three jobs manually;
Caxecraft does not. The provisioning lock therefore disables that mode
explicitly. Leaving it enabled produces a deceptive failure: Raylib can save a
valid internal screenshot while the user sees a black, unresponsive window,
because no frame is ever presented and no event is ever polled. The graphical
smoke retains this behavior as a regression check.

The adapter also omits Raylib 6.0's optional `WindowHighDpi` flag on macOS. On
the pinned backend it double-applies Retina scaling, producing a 5120 by 2880
framebuffer whose useful 1280 by 720 image occupies one quadrant. The visual
smoke rejects that shape; ordinary resizing and the logical 1280 by 720 game
remain available while high-DPI support is fixed separately.

This is a finite playable feasibility slice with a textured title, typed
eight-slot hotbar, original item/HUD art, Creative/Adventure menu choice, and
bounded collect/consume/place rules. Nia provides the first two-step friendly
interaction and one-time gift; one original Mossling provides bounded
rest/wander/chase/return movement, a warned attack and recovery cycle, aimed
fixed-step sword combat, health, defeat, revive, one visible collectible drop,
bounded berry recovery, and clear
full/empty feedback. It still has no finished Adventure, broader enemy roster,
broader consumable catalog, visual editor, persistence, complete
localization, audio, cached chunk meshes, or controller support.
Passing this slice is integrated evidence, not a claim that the planned game
is complete.

## Localization and authored dialogue

The renderer no longer selects English or Spanish with gameplay conditionals.
It carries an opaque locale cursor and asks for typed message IDs. Reusable
menus, controls, pause, and generic feedback live in
[`locales/ui.json`](locales/ui.json). Nia's dialogue, Mossling feedback, the
Adventure tagline, named items, and other authored prose live with the
scenario in
[`scenarios/first-playable/map.caxemap`](scenarios/first-playable/map.caxemap).

That distinction matters: moving or sharing a scenario must also move the text
that gives its characters and objectives meaning. A `.caxemap` refers to those
strings by stable message ID, while saves and gameplay state remain independent
of the chosen language. The future editor will edit the map and its catalog as
one package.

Today the native String/Bytes/filesystem path is not complete, so the build
validates the CaxeMap's embedded catalog and generates a renderer-independent
typed lookup. Eval receives ordinary Haxe `String`; C receives only a
`c.CString` selected from generated literals with static lifetime. The
application owns Raylib drawing and never learns the supported locale names.
The source catalogs are copied
to `bin/content/` as part of native packaging (the reusable UI JSON plus the
complete scenario CaxeMap), but the running binary still
uses the validated embedded mirror. This is a documented transition, not a
claim that runtime catalog loading or the complete bilingual game is finished.

After editing the UI catalog or embedded CaxeMap messages, regenerate and check
the temporary built-in lookup catalog with:

```sh
python3 examples/caxecraft/localization_catalog.py
npm run test:caxecraft-localization
```

Terrain, authored fluid records, fluid presentation, and player spawn use a
second typed adapter generated by a Haxe Eval tool from the same validated
CaxeMap. Python only starts that Haxe tool and compares bytes; it does not parse
or reimplement the format. Regenerate and check it after a world edit:

```sh
python3 examples/caxecraft/level_adapter.py
npm run test:caxecraft-level-adapter
```

The play launcher checks the adapter's exact source hash before invoking Haxe,
so an edited map cannot silently launch with stale terrain. Actor/item
placements and CaxeFlow composition are not all consumed by the native game yet;
`haxe_c-xge.20.4` owns removing that remaining application coupling.

The focused check rejects incomplete locales, duplicate IDs or JSON keys,
unknown fields, invalid UTF-8/control text, stale generated catalogs, and any
return of language-specific prose or branching to
`CaxecraftApp.hx`/`Main.hx`/`TitleMenu.hx`.
The deterministic secondary-locale graphical pilot is:

```sh
python3 examples/caxecraft/play.py --pilot secondary-locale
```

It selects the next catalog without naming its language in application code,
presents the localized title screen, captures a real framebuffer, and exits
within the normal 15-second bound.

Collection is lossless at the fixed stack boundary. If only one slot remains,
one berry enters the inventory and the rest stays visible in the world. If the
stack is full, a Mossling drop remains available and Nia keeps her gift at the
same dialogue step until the player uses a berry. Mining follows the same
lossless rule: a full Grass, Dirt, or Stone stack leaves the matching block in
the world.

It compares the same 38-line semantic trace under pinned Haxe Eval and all three
generated-C layouts, checks the registered split/package/unity snapshots, and
compiles and runs one strict optimized native lane for each layout. The
generated program has an empty `hxrt` plan and imports no allocation symbol.

The exhaustive lane retains cold/reversed/locale/warm-server determinism,
standalone-header checks, GCC/Clang coverage where available, O0/O2, and
ASan/UBSan:

```sh
npm run test:caxecraft-domain:full
```

CI and the full repository toolchain gate use that exhaustive command. The
short command is deliberately useful while developing the game or compiler; it
is not a weaker replacement for release evidence.

The warm-server part of the exhaustive command is a correctness and
request-isolation proof, not merely a speed claim. Its runner starts the exact Haxe
`5.0.0-preview.1` binary from this checkout's Lix installation, binds it only
to `127.0.0.1` on an ephemeral port, and always stops it. HaxeShim still expands
the scoped HXML/library arguments, so connecting directly to the native server
does not bypass the pin or dependency rules. A global Haxe 4.3.7 process cannot
be selected accidentally.

A compiler server can reuse parsed and typed Haxe modules when the source,
classpaths, and compile-time defines permit it. It still checks changed files,
and Reflaxe.C still performs its whole-program HxcIR lowering, validation,
project planning, and file emission for every request. Split, package, and
unity builds also have different layout defines, so they do not form one shared
cache context. Local and hosted measurements found no meaningful end-to-end
speedup for the current Caxecraft builds, including repeated same-layout output.
Required determinism lanes therefore keep an explicit cold build as independent
evidence. On a cache miss, the interactive `play.py` launcher now owns one
exact-pin, loopback-only server per worktree. Its compatibility cookie includes
the Haxe/haxe.c/Reflaxe/HXML infrastructure identity and exact process-start
and executable identities; a stale cookie cannot authorize killing an
unrelated process.
`--haxe-server off` uses a fresh process, while `attach` borrows an explicit
endpoint without owning its lifecycle. An exact unchanged-build hit skips Haxe
and the native toolchain altogether, `--no-build-cache` bypasses only that hit,
`--cold` also forces a fresh Haxe process, and `--build-only` skips Haxe for a
reviewed generated project. See
[test performance](../../docs/test-performance.md) for the measurements. A
future `hxc dev`/`hxc watch` command may extract this proven lifecycle from the
Caxecraft runner; it must preserve the same cold/server byte parity and fresh
request-local compiler state.

For a focused compiler-latency investigation, run:

```sh
npm run profile:caxecraft-compiler
```

This is a profiler, not another correctness gate. It builds the same runtime-
free split Caxecraft program five times in fresh Haxe processes, primes one
owned loopback compiler server, then measures five same-context warm requests.
Every run must produce the same complete normal-artifact set byte-for-byte; the
report records the current set instead of making its incidental file count part
of the compiler contract. The path-free report is written to ignored
`examples/caxecraft/_build/compiler-profile.json`; it records host load, wall
and CPU time, allocation and resident-memory observations, exact target
phases, bounded program/output counts, Haxe's raw timer table, and a
non-overlapping phase summary. “Non-overlapping” means the summary uses each
span's exclusive time--the time left after its nested child spans are
subtracted--so it never ranks the same work once under a child and again under
its parent. The consumer validates the whole span tree and final request totals
before accepting a sample.

These durations stop when Haxe and Reflaxe.C have written the generated C
project. They do **not** include Clang/GCC compilation, native linking with
Raylib, or launching the game. The Caxecraft native differential lanes exercise
those later steps separately; keeping the boundaries separate tells us whether
a delay belongs to Haxe-to-C generation or the native toolchain.

To inspect one realistic source edit instead of repeated unchanged builds, run:

```sh
npm run profile:caxecraft-incremental-edit
```

This diagnostic runs five independent sequences by default. Each sequence owns
one temporary Caxecraft source copy and one Haxe server. It first performs a
cold build, then an unchanged warm build, changes the damage constant in
`Vitals.hx`, and performs one more warm build. It compares the frontend rebuild
closure, every HxcIR function, every normal generated artifact, the semantic
JSON reports, and every C translation unit. The report includes the median,
median absolute deviation (the usual distance from the median), and 95th
percentile for each request kind. It goes to ignored
`examples/caxecraft/_build/incremental-edit-profile.json`; it contains logical
repository paths but never the temporary checkout path.

The same run now verifies the first backend reuse boundary rather than merely
listing a future opportunity. The cold prime must plan every function, the
unchanged warm request must reuse every validated control-flow plan, and the
fixed `Vitals.hx` edit must miss exactly the one HxcIR function whose semantic
text changed. Generated artifacts are still compared independently, so cache
statistics cannot substitute for byte parity.

The first run established the boundary now used by the cache. The edit made
Haxe rebuild 14 of 135 Caxecraft class declarations, but only
`Vitals.applyAttack` changed among 531 HxcIR functions. The backend therefore
reuses 530 plans and rebuilds the changed one. Of 229 generated artifacts, only
`src/modules/caxecraft/domain/Vitals.c` and the manifest change; all generated
headers and 83 of 84 C translation units remain byte-identical. The native
object and link projection is intentionally labelled **unmeasured** until the
depfile-backed native cache proves the complete header and toolchain
dependency closure.

That experiment also found and forced closure of a correctness prerequisite.
Haxe's warm typed graph could attach a named anonymous-record field to its
typedef line in one request and to an object-literal line in another. Normal C
remained stable, but HxcIR source provenance did not. Beads issue
`haxe_c-5sd.8.4.1` now owns the implemented exact declaration-position
contract. The cache key includes that corrected canonical HxcIR text, and the
profiler fails if cold and warm provenance drifts instead of pretending that
matching C bytes is sufficient.

The first optimized sample on a busy 12-logical-CPU Mac began at load 9.88.
Fresh-process median was 18.408s and post-prime warm median was 16.486s. The
remaining dominant phase was structural C-body construction at about 11.4-
11.7s. These are contended diagnostic numbers, not a portable p50/p95 promise.
They do show why the game remains central to compiler work: Caxecraft exposed
three identical body traversals in the ordinary acyclic path. Production now
constructs one body, the source-map fixture explicitly asks for its diagnostic
copy, and only a proven non-returning call cycle triggers specialized
re-emission. Generated bytes and the recursive-cycle behavior remain separately
gated.

That historical profile uses the smaller runtime-free domain workload; it is
not the current full playable's compile time. On 2026-07-26, after the playable
admitted managed collections and escaping classes, a fresh
`play.py --compile-only` journey took 97.48 seconds on this Mac. Instrumentation
at the start of the optimization showed about 54 seconds in semantic lowering,
26 seconds in runtime planning, 46 seconds in artifact planning, and 38 seconds
in output ownership, with overlap and host contention making those phase
figures unsuitable for addition into one wall time. The full symbol ledger had
22,801 records and occupied about 30.4 MB. Cached naming identities, lazy
collision hashing, runtime-reason deduplication, immutable-output hash reuse,
and Caxecraft's explicit summary report reduced that emitted ledger to 389,641
bytes and brought the build under its bounded compiler timeout. This is
progress, not the performance target: Beads issue `haxe_c-5sd.5` owns the
remaining full-plan profiling and budget work.

The playable app has registered generated-C snapshots too. They cover its
manifest, exact selective runtime plan, generated program header, typed level
adapter, inventory HUD, terrain/water renderers, and Raylib application module.
The check rejects generated `goto` and direct allocator calls from application
sources; validates the complete reviewed `hxrt` feature closure; requires direct
window, input, texture, and draw calls; and pins eight load/check/unload
ownership sites, three typed world-billboard borrows, and the precise direct
`rlgl` terrain slice. The snapshot manifest uses Linux as its explicit
canonical link platform so its bytes do not depend on the updater's host;
native CI separately validates each admitted desktop's real link facts:

```sh
npm run test:caxecraft-playable
```

The first original design art pack is checked in and validated independently:

```sh
npm run test:caxecraft-assets
```

That command proves exact offline primary-source PNG bytes, dimensions, alpha
contracts, semantic atlas-cell order, a complete no-sidecar file inventory,
minimal PNG metadata, and repository-scoped generation/privacy records. The
domain runner invokes the same validator before compiling.
The playable currently packages the panorama, wordmark, HUD, base/Adventure
item, entity, and base/Adventure terrain atlases as exact verified primary
bytes. The remaining character, Ivvy, and cutscene-editor atlases remain
design-only, so the
asset gate alone is still not gameplay or visual-polish evidence. See
[`assets/README.md`](assets/README.md) for the exact boundary.

The bounded inventory has its own sub-second renderer-independent proof:

```sh
npm run test:caxecraft-inventory
```

It covers the fixed eight-slot catalog, exact selection/wrap behavior, finite
stack clamping, collect/consume, empty/full edges, and target-neutral source
boundary under two locales. The native movement pilot then proves that the
same inventory selection reaches the real textured hotbar. The gameplay probe
also exercises the complete lossless mining transition, and the dedicated
full-inventory mining pilot proves its visible native-C feedback.

The first actor loop has a separate sub-second renderer-independent proof:

```sh
npm run test:caxecraft-gameplay
```

It checks the authored meadow height, Nia's bounded welcome/gift states,
lossless berry collection, deterministic Mossling wandering and pursuit, its
warned one-impact/recovery cycle, paced sword decisions, health, and recovery
in two locales. The native movement and combat pilots then require exact actor,
damage-feedback, and drop evidence in real presented framebuffers.

The CAXEMAP 1 authoring foundation has a separate fast model contract:

```sh
npm run test:caxecraft-scenario-model
```

It compiles every closed scenario and CaxeFlow family under Eval, proves that
object IDs cannot be mixed with registry content IDs, and rejects Raylib/C
target leakage from the shared model. It also verifies the 18-entry action
catalog, ordered editor fields, unknown-syntax rejection, consumer authority,
and negative duplicate/missing/wrong-schema cases without changing the model
trace. The same fast command exercises the staged codec: bounded UTF-8/token
decoding, syntax parsing, typed content-registry validation, byte-identical
canonical writing, CRLF and input-order convergence, two readable fixtures,
and fail-closed malformed families.
The slower reference check keeps repeated compiler behavior out of that edit
loop while proving the same complete codec probe twice from cold Haxe, once
under an alternate installed locale, and twice through one exact-pin Haxe
compilation server:

```sh
npm run test:caxecraft-scenario-determinism
```

Each request still performs the byte-identical fixture, repeated-write, CRLF,
reverse-order, exact diagnostic, and bounded-work assertions inside Haxe; a
matching one-line trace is therefore evidence that all five requests reached
the same checked result, not merely that they printed the same fixture size.
The fixed-tick rule executor has its own fast command:

```sh
npm run test:caxecraft-caxeflow
```

That probe executes all 10 events, 12 predicates, and 18 actions, plus
priority/ID ordering, once/repeat/cooldown behavior, local and parameterized
sequences, captured delays, deferred signals, explicit seeded choice, and every
runtime budget. It runs the same result under C and Spanish process locales and
compares normal versus reversed rule registration. It is target-neutral Eval
evidence today, not yet proof that the complete executor compiles and runs in
the playable generated-C application.

The renderer-independent editor core and first viewport rules have a focused
command:

```sh
npm run test:caxecraft-editor
```

It creates a complete small map through public typed commands and proves exact
undo/redo, bounded history and gestures, canonical in-memory reload,
last-playable recovery, disposable test play, one-layer projection, exact pixel
mapping, and Select/Paint/Erase/Fill translation under C and a second installed
locale (Spanish when available). The reusable editor package imports no Raylib
or C target API.

The title screen's Editor button now opens the first native Raylib/Raygui
viewport: a bounded 12-by-12 top-down layer whose pointer gestures become the
same typed `EditorCommand` values. The screen caches a read-only projection
between accepted edits; it does not serialize the draft every frame. Its
deterministic generated-C graphical proof is:

```sh
python3 examples/caxecraft/play.py \
  --pilot editor-shell \
  --raylib-configuration memory-software \
  --allow-network
```

This is the first truthful visual editing slice, not a claim that native file
save, multi-layer navigation, or the complete child-friendly event/cutscene
tools are available. Its design is explained in
[the editor semantics guide](../../docs/caxecraft-editor.md).

Native persistence and the remaining visual editor work continue as ordered
`haxe_c-xge.19.*` slices. The readable
[CAXEMAP 1 reference](../../docs/caxemap-1.md) is their shared contract.

To compile only the C project through the direct recovery path:

```sh
node_modules/.bin/haxe \
  --cwd examples/caxecraft \
  build.hxml \
  -D hxc_runtime_diagnostics=off \
  --custom-target c=examples/caxecraft/_build/c
```

The default is the source-shaped `split` layout. It mirrors Haxe package/module
ownership under `include/hxc/modules/` and `src/modules/`, with a common private
types header and small `src/hxc/main.c` entry wrapper. The checked-in split,
package, and unity forms now pass the [generated-C maintainability
rubric](../../docs/generated-c-maintainability.md): every function has a stable
ownership/identity record, source spans cover the source-backed domain methods,
ordinary names stay bounded, temporary pressure is measured per function, and
this reducible game-domain output contains no `goto`. This is a bounded
Caxecraft result, not a promise that arbitrary Haxe already looks handwritten.

To group modules into one header/source pair per Haxe package, add:

```sh
-D hxc_project_layout=package
```

For Caxecraft this produces `caxecraft/domain/package.h` plus `package.c` and a
smaller `caxecraft/qa` pair. It is the middle ground between per-module files
and amalgamation: package ownership stays visible while the native build sees
fewer translation units.

To request the compact single-implementation form, add:

```sh
-D hxc_project_layout=unity
```

Unity deliberately retains the single `src/program.c` mode. Split, package,
and unity are file assignments over the same validated semantic and declaration
plan, so choosing a layout does not change names, runtime policy, or game
behavior. Reviewable snapshots retain all three layouts byte-for-byte and a
schema-validated maintainability report for each.

`_build/` is scratch output. Reviewable generated evidence lives in
[`expected/`](expected/) and is updated only through:

```sh
npm run snapshots:update -- --suite caxecraft-domain
```

The eventual developer workflow is one project-level `hxc dev`/`hxc watch`
command that owns measured compiler-server reuse, canonical watched inputs,
debounce, last-known-good output, optional process restart, and structured
events. “Measured” matters: the server experiment above shows that keeping a
process alive is not enough when target lowering dominates. That surface is
intentionally being implemented with the target-neutral `hxc` CLI foundations
rather than as a Caxecraft-only watcher. Until then, the direct HXML command
above and the one-command proof remain the supported recovery paths.

See [the domain design](../../docs/caxecraft-domain.md) for storage, target
conditionals, the long-term adapter plan, DDA, collision, determinism, and QA
details, and [the game design document](../../docs/caxecraft-game-design.md)
for the product, story, content, editor, localization, accessibility, art, and
release contract. This slice proves that the exercised host can generate and
link the native executable; a human must still run the documented interactive
smoke before that host is marked play-tested. It does not prove full Haxe
portability, showcase visual quality, optimized rendering, or general compiler
support beyond the exercised language subset.

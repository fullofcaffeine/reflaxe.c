# Caxecraft

Caxecraft is the repository's small original voxel-sandbox Raylib showcase and
haxe.c's current flagship product-level E2E/QA workload. It is expected to find
reusable compiler and tooling improvements, not hide their absence behind
game-only workarounds. The repository now contains a first playable feasibility
slice: a finite voxel world, seeded terrain, block edits, voxel raycasting,
fixed-step first-person collision, and a C-specific Raylib window/input/render
adapter. The complete Creative/Adventure/editor/Ivvy direction and its honest
present-versus-planned gates live in the
[game design document](../../docs/caxecraft-game-design.md).

The core is ordinary Haxe. Only `WorldCells` and `WorldStorage` know that the C
build uses a stack-backed `CArray<UInt8>`/`Span<UInt8>` while Eval uses
`Array<Int>`. That narrow seam keeps gameplay readable and gives the compiler a
realistic, allocation-free C workload without hiding C behind a portability
framework.

## Why the source contains `#if c`

`c` is a Haxe compile-time define exposed by the verified
`--custom-target c=<output>` activation. Haxe resolves `#if c` before normal
typing and code generation, so it is not a runtime platform check and adds no
branch to the generated executable. Only the selected imports, types, and
expressions exist in that compilation. Passing `-D c` manually does not start
the C target and is not a supported build.

The current conditionals are deliberately narrow:

- `WorldCells` selects compact borrowed `Span<UInt8>` storage for C and an
  ordinary `Array<Int>` fallback for the Haxe oracle;
- `WorldStorage` performs the exact C integer conversions required by that
  carrier;
- `CaxecraftTrace` and `DomainProbe` construct fixed C storage or ordinary Haxe
  test storage and choose the appropriate output plumbing.

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
direct-C manifest and empty `hxrt` plan, reuses or builds the pinned Raylib
static library, then compiles, links, and launches the generated split C
project. It never consults an undeclared sibling checkout or writes
hand-authored C into the generated project.

Useful non-interactive forms are:

```sh
# Haxe -> C only; no Raylib provisioning or window
npm run caxecraft:play -- --compile-only

# Produce the native executable but do not open a window
npm run caxecraft:play -- --build-only

# Verify every source byte in an already extracted Raylib tree
npm run caxecraft:play -- \
  --authority offline-source \
  --source /absolute/path/to/raylib-6.0
```

Use `WASD` to move, the mouse to look, Space to jump, left click to remove the
selected block, right click to place dirt, Escape to pause or release the
cursor, and `Q` to quit. Losing focus pauses and releases the cursor; clicking
the paused window captures it again without also mining. The window may be
resized down to 800 by 450.

Before closing the playable-slice Bead, record one human smoke pass with the
OS, C compiler, window sizes, and pass/fail result. Check all of these behaviors:

- launch and quit with `Q` and the window close button;
- mouse look, `WASD`, gravity, collision, and jumping;
- block selection, removal, valid placement, and rejected player-overlap placement;
- Escape release/pause, click recapture without click-through, and focus-loss pause;
- resizing at 800 by 450, 1280 by 720, and one larger comfortable size;
- readable terrain edges, selection outline, crosshair, counters, and status text.

Automation intentionally cannot substitute for this list: a successful native
link does not prove how the window, mouse, or graphics feel in a real desktop
session.

This is a finite, texture-free feasibility slice. It has no inventory,
persistence, enemies, NPCs, Creative/Adventure modes, editor, localization,
audio, or chunk renderer yet. Its original palette and crisp HUD are
intentional, but the later visual/UX task owns flagship art integration.

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

The playable app has registered generated-C snapshots too. They cover its
manifest, empty runtime plan, generated program header, palette,
allocation-free counter HUD, and Raylib application module. The check also
rejects generated `goto`, `hxrt`, and allocation calls and requires the direct
window, input, and draw calls. The snapshot manifest uses Linux as its explicit
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
The images are not loaded by a playable renderer yet, so passing this gate is
asset-inventory evidence rather than a gameplay or visual-polish claim. See
[`assets/README.md`](assets/README.md) for the boundary.

The CAXEMAP 1 authoring foundation has a separate fast model contract:

```sh
npm run test:caxecraft-scenario-model
```

It compiles every closed scenario and CaxeFlow model family under Eval, proves
that object IDs cannot be mixed with registry content IDs, checks the first
canonical fixture, and rejects Raylib/C target leakage from the shared model.
This is model and format evidence only: the parser, native persistence, rule
executor, and visual editor remain the ordered `haxe_c-xge.19.*` slices. The
readable [CAXEMAP 1 reference](../../docs/caxemap-1.md) defines their common
contract.

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
command that owns compiler-server reuse, canonical watched inputs, debounce,
last-known-good output, optional process restart, and structured events. That
surface is intentionally being implemented with the target-neutral `hxc` CLI
foundations rather than as a Caxecraft-only watcher. Until then, the direct HXML
command above and the one-command proof remain the supported recovery paths.

See [the domain design](../../docs/caxecraft-domain.md) for storage, target
conditionals, the long-term adapter plan, DDA, collision, determinism, and QA
details, and [the game design document](../../docs/caxecraft-game-design.md)
for the product, story, content, editor, localization, accessibility, art, and
release contract. This slice proves that the exercised host can generate and
link the native executable; a human must still run the documented interactive
smoke before that host is marked play-tested. It does not prove full Haxe
portability, showcase visual quality, optimized rendering, or general compiler
support beyond the exercised language subset.

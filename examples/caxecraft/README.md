# Caxecraft

Caxecraft is the repository's small original voxel-sandbox Raylib showcase and
haxe.c's current flagship product-level E2E/QA workload. It is expected to find
reusable compiler and tooling improvements, not hide their absence behind
game-only workarounds. This first slice is deliberately the
renderer-independent game core: a finite
voxel world, seeded terrain, block edits, voxel raycasting, and a fixed-step
first-person collision controller. The interactive Raylib shell follows in the
next task. The complete Creative/Adventure/editor/Ivvy direction and its honest
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

The interactive Raylib window, input, renderer, and native resources will be a
C-specific adapter around that shared domain. A later Rust/JavaScript experiment
will provide target-appropriate storage, clock/input, rendering, and resource
adapters and compare the same deterministic domain traces. We will not grow
target branches throughout gameplay or impose one universal rendering API/IR.
Only abstractions demonstrated by at least two working adapters should move
into shared code. The detailed boundary rules, limitations, and migration
sequence live in [the domain design](../../docs/caxecraft-domain.md).

From the repository root, run the fast development proof:

```sh
npm ci
npm run test:caxecraft-domain
```

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
release contract. This slice does not yet prove a playable window, public
RaylibHx bindings, full Haxe portability, or general compiler support beyond
the exercised language subset.

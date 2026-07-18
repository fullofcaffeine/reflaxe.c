# Caxecraft

Caxecraft is the repository's small Minecraft-like Raylib showcase. This first
slice is deliberately the renderer-independent game core: a finite voxel world,
seeded terrain, block edits, voxel raycasting, and a fixed-step first-person
collision controller. The interactive Raylib shell follows in the next task.

The core is ordinary Haxe. Only `WorldCells` and `WorldStorage` know that the C
build uses a stack-backed `CArray<UInt8>`/`Span<UInt8>` while Eval uses
`Array<Int>`. That narrow seam keeps gameplay readable and gives the compiler a
realistic, allocation-free C workload without hiding C behind a portability
framework.

From the repository root, run the complete differential and native proof:

```sh
npm ci
npm run test:caxecraft-domain
```

It compares the same 38-line semantic trace under pinned Haxe Eval and generated
C, checks deterministic output across cold and warm compiler-server builds,
and runs strict O0/O2 plus ASan/UBSan native lanes. The generated program has an
empty `hxrt` plan and imports no allocation symbol.

To compile only the C project through the direct recovery path:

```sh
node_modules/.bin/haxe \
  --cwd examples/caxecraft \
  build.hxml \
  -D hxc_runtime_diagnostics=off \
  --custom-target c=examples/caxecraft/_build/c
```

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

See [the domain design](../../docs/caxecraft-domain.md) for storage, DDA,
collision, determinism, and QA details. This slice does not yet prove a playable
window, public RaylibHx bindings, full Haxe portability, or general compiler
support beyond the exercised language subset.

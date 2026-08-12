# Caxecraft art pack

This directory contains the first original Caxecraft visual-design pack. The
native game now packages and loads the title panorama, wordmark, HUD atlas,
base and Adventure item atlases, entity atlas, base and Adventure terrain
atlases, and three voxel world props. Three atlases remain design inputs: their
presence is not evidence that Adventure characters, Ivvy, or cutscene-editor
icons are used by the native game yet. The
complete scope is described by the
[game design document](../../../docs/caxecraft-game-design.md) and
[domain design](../../../docs/caxecraft-domain.md).

`manifest.json` is the machine-readable authority for every asset byte, size,
atlas grid, semantic cell name, source record, and
rights record. Validate it offline with:

```bash
python3 examples/caxecraft/check_assets.py
```

The raster originals were produced with the built-in OpenAI image-generation
workflow and selected through human review. The checked-in PNGs are explicitly
classified as primary design-source inputs, licensed under the repository's
`GPL-3.0-only` license; their interactive creation history is provenance, not a
byte-regeneration command. Online generation is never part of a clean build.
Any future resize, packing, compression, conversion, or runtime-ready derivative
must have deterministic source-to-byte regeneration. `haxe_c-xge.15` still owns
those derived-asset tools, runtime sampling/lifetime decisions, visual goldens,
Git/LFS growth policy, and human review before these assets can be called
shipped art. The primary runtime files need no regeneration recipe because
packaging copies the hash-verified primary bytes without changing them.

## Voxel world props

`models/forge-relay.vox`, `models/gate-winch.vox`, and
`models/field-note.vox` are original voxel props. The `.vox` files use the
MagicaVoxel 150 format. Raylib reads this standard format directly.

The Haxe source in `tools/ForgeRelayVox.hx` is the editable authority. It
defines each colored cell in a 32×32×32 volume. The `.vox` files are generated
game assets. They do not contain hidden LLM state.

The workflow has two separate parts:

1. A person or an agent designs the shape, colors, scale, and silhouette.
2. The Haxe builder converts that exact source into the same `.vox` bytes.

An LLM can help with the first part, but it is not required. The second part is
fully deterministic and does not use a network connection or image model.

Regenerate all three files with:

```bash
npm run caxecraft:voxels:refresh
```

Make sure that the checked-in files match the Haxe source with:

```bash
npm run caxecraft:voxels:check
npm run test:caxecraft-assets
```

The first command rebuilds each model in memory and compares its exact bytes.
The second command checks its dimensions, hash, source record, and package
inventory. Update `manifest.json` after an intentional asset change.

Use these steps for a new prop:

1. Start with a clear purpose and a readable outline from several angles.
2. Use broad voxel forms before small surface details.
3. Keep important colors in one reviewed palette.
4. Put the model path and world bounds in reloadable content.
5. Keep a 2D atlas cell for the HUD or a bounded load-error fallback.
6. Review the result from a real gameplay camera before publication.

Do not extrude every sprite pixel into a cube. That process usually creates a
flat slab with noisy edges. Treat the sprite as a reference, then author a real
volume with a useful side and back silhouette.

Current `.vox` support draws static models. It does not animate them. A future
animation system can use named clips made from voxel frames, rigid voxel parts,
or a documented skeleton. That system must keep timing and clip selection in
reloadable content. It must also define interpolation, bounds, collisions, and
resource ownership before Caxecraft uses it for characters or enemies.

The pack deliberately contains no official Haxe logo binary. The Haxe branding
page makes marks downloadable and governs visual use, but the reviewed page did
not state an SPDX redistribution license. A future “built with Haxe” badge needs
explicit permission/provenance review first. The current engineering wordmark
and future Haxeforge tool must remain original Caxecraft designs, not altered or
embellished Haxe marks.

The Ivvy atlas was visually inspired by a private user-provided photograph of
the family's Siamese cat. Repository evidence proves that the photograph, its
path, and identifying metadata are not included in or distributed from this
repository. Only the original stylized output and a deliberately non-identifying
reference description are checked in.

All atlases use row-major coordinates from the top left. The validator pins the
exact semantic cell order, the complete asset-directory file set, and a minimal
PNG metadata profile containing only sRGB intent and pixel dimensions. Future
Runtime coordinates are centralized as typed `ItemKind`/hotbar mappings in
`CaxecraftAtlas` and material/face mappings in `TerrainAtlas`; adding a used
cell requires updating one presentation table and the manifest-owned semantic
order rather than scattering integer rectangles through gameplay.

`atlases/cutscene-editor.png` is the design source for the planned visual
cinematic tools. Its 16 cells cover camera placement and movement, shot
transitions, waits, actor staging, dialogue, music/effects, branching, and
skip. It is deliberately classified as design-only until the public editor
selects those cells through a typed presentation table and native UI tests.

# Caxecraft art pack

This directory contains the first original Caxecraft visual-design pack. It is
checked-in design input for the future raylib game, not evidence that the game
currently loads textures or is playable. The current executable evidence is
still the renderer-independent domain described by the
[game design document](../../../docs/caxecraft-game-design.md) and
[domain design](../../../docs/caxecraft-domain.md).

`manifest.json` is the machine-readable authority for every PNG byte, size,
alpha expectation, atlas grid, semantic cell name, generation record, and
rights record. Validate it offline with:

```bash
python3 examples/caxecraft/check_assets.py
```

The Caxecraft originals were produced with the built-in OpenAI image-generation
workflow and selected through human review. The checked-in PNGs are explicitly
classified as primary design-source inputs, licensed under the repository's
`GPL-3.0-only` license; their interactive creation history is provenance, not a
byte-regeneration command. Online generation is never part of a clean build.
Any future resize, packing, compression, conversion, or runtime-ready derivative
must have deterministic source-to-byte regeneration. `haxe_c-xge.15` still owns
those derived-asset tools, runtime sampling/lifetime decisions, visual goldens,
Git/LFS growth policy, and human review before these assets can be called
shipped art.

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
runtime code should generate or validate typed coordinates from that inventory
instead of scattering integer rectangles through rendering code.

# Caxecraft Project Instructions

`AGENTS.md` in this directory is the canonical scoped contract for Caxecraft.
Read and follow it together with the repository-root `AGENTS.md` before changing
the game, engine, content formats, editor, tests, tooling, assets, or generated
evidence.

In particular, preserve the engine/content boundary: reusable Haxe implements
mechanics, while validated content packs, CaxeMap levels, CaxeFlow rules,
localization catalogs, asset packs, and campaign manifests own concrete
characters, placement, prose, presentation identity, rewards, encounters, and
progression. Do not duplicate authored content in `Main.hx` or a character-
specific gameplay helper. Construct stateful runtime instances from validated
content and expose new mechanics through typed shared registries and the editor.

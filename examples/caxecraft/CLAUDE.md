# Caxecraft Project Instructions

`AGENTS.md` in this directory is the canonical scoped contract for Caxecraft.
Read and follow it together with the repository-root `AGENTS.md` before changing
the game, engine, content formats, editor, tests, tooling, assets, or generated
evidence.

In particular, preserve the engine/content boundary: reusable Haxe implements
mechanics, while validated CaxeMap levels, CaxeFlow rules, localization data,
resolved content registries, reviewed asset manifests, and planned campaign
manifests own concrete characters, placement, prose, presentation identity,
rewards, encounters, and progression. CAXEMAP has an `asset-pack` path today,
but the general pack format/loader is planned under `haxe_c-xge.20.4.1`; do not
describe it as shipped. Do not duplicate authored content in `Main.hx` or a
character-specific gameplay helper. Construct stateful runtime instances from
validated content and expose new mechanics through typed shared registries and
the editor.

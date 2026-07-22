# Caxecraft base content pack

`content.json` is the checked-in source for Caxecraft's current built-in blocks,
fluids, movement profiles, items, non-player characters (NPCs), enemies, drops,
and effects. CaxeMap files refer to these definitions by namespaced IDs such as
`caxecraft:stone` or `caxecraft:water`; they do not require a matching Haxe
class for every piece of game content.

Run the focused check after editing the pack:

```sh
npm run test:caxecraft-content-pack
```

The check rejects unknown fields, duplicate IDs, wrong-kind or missing
references, unsupported mechanic profiles, invalid atlas cells, and a stale
generated adapter. It then resolves the real first-playable map with Eval and
compiles the generated adapter through haxe.c and strict C11 without `hxrt`.

## Why a generated Haxe adapter exists

The engine does not yet have the general filesystem and JSON runtime needed to
load this file in native C. `content_pack.py` therefore validates the JSON at
build time and generates `BaseContentPack.hx`, which is ordinary typed Haxe.
The game and compiler tests consume that Haxe; there is no copied hand-written
C table. The JSON is also packaged beside the game so the eventual runtime
loader can adopt the same format without moving the authored data.

Do not edit `BaseContentPack.hx` directly. Change `content.json`, run
`python3 examples/caxecraft/content_pack.py`, and review both the data and the
generated typed surface.

## Water and aquatic profiles

Schema 2 keeps fluids separate from solid terrain blocks. The built-in water
entry selects the already implemented bounded-flow simulation and names its
render, camera, audio, and reviewed atlas profiles. It does not encode water as
a decorative passable block.

Aquatic profiles contain target-neutral integer facts for breath, buoyancy,
drag, movement control, mining, and cold protection. The ordinary profile is
the default. The Tideweave suit is an item that selects the stronger profile;
its location belongs to a CaxeMap, not this reusable definition.

These declarations now validate and generate typed Haxe. The native playable
does not yet consume the complete authored level, render water, or persist its
mutable fluid state to a file. The engine now exposes a validated in-memory
water snapshot for the future full save codec. The remaining product
integrations are owned by `haxe_c-xge.20.5.3`; the schema does not claim they
already work.

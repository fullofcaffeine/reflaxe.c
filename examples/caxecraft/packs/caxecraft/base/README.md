# Caxecraft base content pack

`content.json` is the checked-in source for Caxecraft's current built-in blocks,
items, non-player characters (NPCs), enemies, drops, and effects. CaxeMap files
refer to these definitions by namespaced IDs such as `caxecraft:stone`; they do
not require a matching Haxe class for every piece of game content.

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

## Current boundary

Schema 1 describes only mechanics that exist now. In particular, it has one
passable block—air—and no decorative “water” entry. Water will be added with
its real fluid kind and engine behavior: deterministic voxel flow, leaks,
swimming and buoyancy, breath, underwater presentation, saving, and tests.
Keeping the entry out until those mechanics exist prevents data from claiming
a capability the game cannot yet honor.

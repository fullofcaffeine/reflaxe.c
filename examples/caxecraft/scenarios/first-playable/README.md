# First-playable scenario package

This directory is the content home for the small Adventure slice that currently
contains Nia and the Mossling. [`map.caxemap`](map.caxemap) owns its finite
world, initial water, Tideweave placement, story references, and its complete
English and Mexican Spanish prose.
Nia's lines are scenario content: they do not belong in the reusable game UI
catalog or in the Haxe game loop.

The current playable reads this file after startup, validates it through the
shared Haxe CaxeMap codec, resolves its content IDs, and builds a complete
candidate session before opening Raylib. Actor/item placements and some
CaxeFlow presentation still have temporary application wiring, but there is no
second generated level format. The scenario package is:

```text
first-playable/
  map.caxemap       # terrain, fluids, objects, CaxeFlow, and embedded locales
```

The editor reads, validates, and writes this same model. Packaging copies the
complete CaxeMap beside the native executable, and a map edit is picked up on
the next launch without running Haxe or a C compiler. After editing
`map.caxemap`, run `npm run test:caxecraft-scenario-model` and
`npm run test:caxecraft-runtime-level-loader`. The atomic package tracer also
loads the reusable UI catalog and this map into one unpublished generation;
run `npm run test:caxecraft-runtime-content-generation` after changing either
source. The active renderer still uses the separately owned generated lookup
catalog until the publication and renderer-boundary migration lands.

`fluid ... source` records place permanent simulation sources. `fluid ...
volume` records fill a finite box once when the level starts, so that water may
then flow or drain. The map owns this designed starting state; a player save
will own the later mutable cells and pending flow work.

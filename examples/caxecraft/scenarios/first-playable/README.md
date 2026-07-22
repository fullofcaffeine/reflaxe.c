# First-playable scenario package

This directory is the content home for the small Adventure slice that currently
contains Nia and the Mossling. [`map.caxemap`](map.caxemap) owns its finite
world, initial water, Tideweave placement, story references, and its complete
English and Mexican Spanish prose.
Nia's lines are scenario content: they do not belong in the reusable game UI
catalog or in the Haxe game loop.

The current playable derives its terrain, initial fluid cells, fluid atlas
selection, and player spawn from this file through a checked-in typed Haxe
adapter. It does not yet parse the file at run time, and actor/item placements
and CaxeFlow composition still have temporary application wiring. That partial
bridge is not a second accepted content format. The durable scenario package is:

```text
first-playable/
  map.caxemap       # terrain, fluids, objects, CaxeFlow, and embedded locales
```

The editor reads, validates, and writes this same model. Until native file
loading is available, build-time Haxe tools generate narrow typed localization
and level adapters, and packaging copies the complete CaxeMap beside the native
executable. Edit `map.caxemap`, run `python3 examples/caxecraft/localization_catalog.py`
and `python3 examples/caxecraft/level_adapter.py`, then run their focused tests.
Never hand-edit either generated Haxe adapter.

`fluid ... source` records place permanent simulation sources. `fluid ...
volume` records fill a finite box once when the level starts, so that water may
then flow or drain. The map owns this designed starting state; a player save
will own the later mutable cells and pending flow work.

# First-playable scenario package

This directory is the content home for the small Adventure slice that currently
contains Nia and the Mossling. [`map.caxemap`](map.caxemap) owns its finite
world/story references and its complete English and Mexican Spanish prose.
Nia's lines are scenario content: they do not belong in the reusable game UI
catalog or in the Haxe game loop.

The current playable still renders an earlier hardcoded meadow while native
CaxeMap loading is implemented. That is temporary application scaffolding, not
a second accepted content format. The durable scenario package is now:

```text
first-playable/
  map.caxemap       # world, story/CaxeFlow references, and embedded locales
```

The editor reads, validates, and writes this same model. Until native file
loading is available, the build extracts the validated embedded catalog,
generates a narrow typed rendering adapter, and copies the complete CaxeMap
beside the native executable. Edit `map.caxemap` and run
`npm run test:caxecraft-localization`; never edit the generated Haxe adapter by
hand.

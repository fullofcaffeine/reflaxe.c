# First-playable scenario package

This directory is the content home for the small Adventure slice that currently
contains Nia and the Mossling. [`messages.json`](messages.json) owns its English
and Mexican Spanish prose. Nia's lines are scenario content: they do not belong
in the reusable game UI catalog or in the Haxe game loop.

The matching `.caxemap` is not here yet because the current playable still uses
an earlier hardcoded meadow while native CaxeMap loading is implemented. That is
temporary scaffolding, not a second accepted content format. The intended
package is:

```text
first-playable/
  map.caxemap       # world, actors, message IDs, objectives, and CaxeFlow
  messages.json     # complete translated prose for those message IDs
```

The editor will save and validate both files as one scenario. Until native file
loading is available, the build validates `messages.json`, generates a narrow
typed rendering adapter, and also copies the source catalog beside the native
executable. Edit the JSON and run `npm run test:caxecraft-localization`; never
edit the generated Haxe adapter by hand.

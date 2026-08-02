# Caxecraft base content pack

`content.json` is the checked-in source for Caxecraft's current built-in blocks,
fluids, movement profiles, items, non-player characters (NPCs), enemies, drops,
and effects. CaxeMap files refer to these definitions by namespaced IDs such as
`caxecraft:stone` or `caxecraft:water`; they do not require a matching Haxe
class for every piece of game content.

Run the focused check after editing the pack:

```sh
npm run test:caxecraft-runtime-schemas
npm run test:caxecraft-runtime-content-generation
```

The checks read the real packaged bytes, reject unknown fields, duplicate IDs,
wrong-kind or missing references, unsupported mechanic profiles, and invalid
atlas cells. They then resolve the first-playable map with Eval and with real
generated C compiled under the host's strict C toolchain and sanitizers where
available.

## Runtime authority

The launcher copies this file beside the executable. The Haxe package loader
reads, parses, validates, and publishes it after startup, so editing the pack
does not run Haxe, haxe.c, the C compiler, or the linker. The executable rejects
bad content before publishing a new runtime generation. There is no generated
Haxe product mirror to refresh.

Older focused compiler tests retain a manually reviewed compatibility fixture
under `test/`. It is not packaged or compiled into the game and is tracked for
replacement by a runtime-registry fixture.

## Water and aquatic profiles

Schema 2 keeps fluids separate from solid terrain blocks. The built-in water
entry selects the already implemented bounded-flow simulation and names its
render, camera, audio, and reviewed atlas profiles. It does not encode water as
a decorative passable block.

Aquatic profiles contain target-neutral integer facts for breath, buoyancy,
drag, movement control, mining, and cold protection. The ordinary profile is
the default. The Tideweave suit is an item that selects the stronger profile;
its location belongs to a CaxeMap, not this reusable definition.

These declarations now validate into typed runtime models. The native playable
consumes the authored level and renders and simulates water, while persistence
of mutable fluid state remains future save-codec work. The schema does not
claim broader mod, campaign, or platform qualification.

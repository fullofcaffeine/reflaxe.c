# Caxecraft creator and modding guide

Caxecraft is being built so a creator can make a world, story, and rules without
editing the engine. The public authoring model is CaxeMap plus CaxeFlow. Haxe
implements reusable mechanics; authored data decides where and when those
mechanics are used.

This guide separates what works now from the intended workflow. That boundary
matters: the current native game packages the first CaxeMap and uses generated,
validated adapters for its localization, terrain, initial water, fluid visual,
and player spawn. Inventory setup, Nia/Mossling construction, item placements,
and rule composition still contain temporary Haxe wiring. `haxe_c-xge.20.4`
owns removing that remaining coupling. Dropping a new map beside the executable
does **not** yet replace the playable world because native file loading is not
implemented.

## Where each kind of change belongs

| You want to change | Owner | Why |
|---|---|---|
| terrain, dimensions, initial fluids, placed objects, spawn points, tags | CaxeMap | these describe one level |
| dialogue, objectives, and translated story text | the level's CaxeMap | prose travels with the content that uses it |
| triggers, gifts, state changes, encounters, and progression | CaxeFlow rules inside CaxeMap | these are authored cause-and-effect rules |
| item, actor, behavior, render, audio, and effect definitions | validated content pack | levels reference stable definitions rather than copying their implementation |
| level order and transitions | campaign data | campaigns connect levels; this format is planned, not shipped |
| movement, inventory transfer, combat, dialogue playback, and rule execution | ordinary typed Haxe | these are reusable game mechanics |
| menus and product-wide interface text | `locales/ui.json` | global interface text is not level story content |
| asset bytes and provenance | `assets/manifest.json` | this lock proves which reviewed files are packaged |
| automated player input and assertions | CaxeTest/native pilot | tests exercise content; they are not content |

The built-in schema-2 content manifest is
[`packs/caxecraft/base/content.json`](../examples/caxecraft/packs/caxecraft/base/content.json).
A strict build-time validator turns it into the typed
`ScenarioContentRegistry` used by CaxeMap and editor validation. The separate
`assets/manifest.json` proves the reviewed visual bytes that those logical
definitions reference. Arbitrary runtime pack discovery/loading is not
implemented yet, so copying a new JSON file beside the game does not install a
mod.

## CaxeFlow versus the native pilot

CaxeFlow is part of the game. It answers questions such as: “What happens when
the player talks to this object?” A rule may use the validated `give-item`
action without putting a Nia or berry branch in Haxe:

```text
rule nia-gift priority 10 once
  when interact guide.nia
  if (always)
  do dialogue dialogue.nia.welcome
  do give-item player.start caxecraft:berries 2
end rule
```

The exact IDs must exist with the right kinds in the validated scenario and
content registry. Unknown actions, missing objects, wrong item kinds, and
invalid quantities fail before the scenario becomes live.

Starting inventory is authored setup too. The completed loader will establish
it through the same validated CaxeFlow action path when a campaign/level session
starts. It must not remain in `Inventory.starter()` or be reconstructed by the
application loop. The entry-trigger spelling and campaign ownership are still
being implemented, so this guide does not invent syntax for them.

`PilotScript` has different authority. It says “press interact on this frame,
observe the result, and quit.” A pilot may request a clearly marked test-only
setup—such as a full berry stack—to exercise a boundary. It must load the real
authored baseline first, cannot become a second story language, and is removed
from ordinary builds. See the [game-pilot guide](caxecraft-game-pilot.md).

Cutscenes are CaxeFlow content too; they are not `PilotScript` sequences. A
cutscene may use authorized world and presentation actions plus the shared
fixed clock. A pilot tests it through player-visible input such as start,
advance, or skip, then observes the resulting state. Keeping those models
separate prevents a test from becoming the privileged implementation of the
story it is supposed to verify.

## Events can start any admitted mechanic

CaxeFlow does not define one trigger language for cinematics and another for
doors or enemies. Its general relationship is:

```text
typed event -> optional conditions -> ordered actions
```

Entering a spatial volume is one event. A rule may respond with any compatible
registered action: dialogue, a bridge state, an encounter, music, weather, an
objective, a reusable sequence, a level transition, or a cutscene. Several
actions may be combined in their authored order. Cutscenes get a focused
camera/actor timeline in the editor because that presentation is easier to
author visually, but “start cutscene” remains an ordinary action on the same
rule card.

New engine mechanics follow the same path. Typed Haxe observes the real engine
fact or implements the action, the shared registry describes its closed fields,
the validator checks every reference, and the executor handles it exhaustively.
Content cannot add arbitrary code, a host callback, or an untyped event payload.
The generalized spatial/event editor is planned under `haxe_c-xge.19.10`; the
current CAXEMAP 1 implementation already supplies bounded trigger zones and ten
closed event kinds but not the complete visual workflow described here.

The editor will expose that same model at three depths: friendly sentence cards,
advanced visual logic, and exact CaxeMap/CaxeFlow text. An experienced modder or
agent can edit nested rules efficiently in text, then return to a visual graph
without losing behavior. Text receives completion and diagnostics from the same
registries used by the parser and cards; it is not a more powerful hidden API.
Invalid text remains a draft, and no view can install arbitrary code or access
the host machine.

## What creators can validate today

The first authored package is
[`examples/caxecraft/scenarios/first-playable/map.caxemap`](../examples/caxecraft/scenarios/first-playable/map.caxemap).
It already uses the public version-1 grammar for a finite world, initial water,
localized messages, placements, dialogue, and an objective. The codec, validator,
canonical writer, CaxeFlow executor, and renderer-independent editor model run
under the pinned Haxe oracle.

From the repository root:

```sh
# Parse, validate, execute, and round-trip the scenario/CaxeFlow model.
npm run test:caxecraft-scenario-model
npm run test:caxecraft-caxeflow

# Check editor commands, history, and disposable test play.
npm run test:caxecraft-editor

# Validate built-in definitions, editor resolution, packaging, Eval, and C.
npm run test:caxecraft-content-pack

# Check canonical bytes across locale and repeated builds.
npm run test:caxecraft-scenario-determinism

# Exercise the currently integrated generated-C game.
npm run caxecraft:play -- --pilot launch-smoke
```

The first three commands validate authoring semantics. The native pilot proves
the current generated-C application, but it does not yet prove that arbitrary
CaxeMap geometry or rules were loaded into that application. The
[CAXEMAP 1 reference](caxemap-1.md) documents the exact grammar, bounds,
canonical order, validation, and current compiler limitations.

## Extending the engine safely

When content needs a mechanic that does not exist—entering a vehicle, changing
weather, or lowering a bridge—add one small typed Haxe capability and expose a
validated action descriptor. The editor, writer, console, cutscene system, and
tests should learn its schema from that shared descriptor, subject to their
different authority. Do not add a character-name branch, arbitrary script
callback, raw C fragment, or duplicated mini-simulator.

After the mechanic exists, a creator should be able to select it in the visual
editor and save ordinary CaxeFlow data. Haxe stays the readable engine language;
the level remains portable, inspectable data.

## Planned creator workflow

The intended loop is:

1. choose or create a campaign and level;
2. sculpt terrain and place typed objects in the visual editor;
3. add English/Spanish text and CaxeFlow cards;
4. validate references and limits immediately;
5. test-play through the same engine systems;
6. save canonical CaxeMap/campaign data; and
7. run CaxeTest/native pilots for repeatable regression checks.

The visual Raygui editor, arbitrary runtime content-pack loader, campaign format,
direct native CaxeMap loading, save overlays, and child-friendly visual cards
are planned Beads work. The renderer-independent editor model and CaxeMap 1
format are implemented foundations, not a claim that the complete creator UI
already ships.

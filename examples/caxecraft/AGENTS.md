# Caxecraft Agent Instructions

The repository-root `AGENTS.md` remains authoritative. These additional rules
apply to every file under `examples/caxecraft`.

## Engine Mechanics and Authored Content

Caxecraft engine code implements reusable mechanics. Validated levels and their
resolved content registries decide which concrete content uses those mechanics;
campaign data will connect levels once that planned format exists. Do not put a
character, location, quest, encounter, reward, dialogue, visual identity, or
story progression directly in a reusable gameplay system or the application
loop when the public CaxeMap, CaxeFlow, localization, asset, or campaign model
can own it.

Keep this rule evidence-bounded. CAXEMAP 1 already stores an `asset-pack`
logical path, `ScenarioContentRegistry` is the typed validation boundary, and
`assets/manifest.json` validates reviewed visual bytes and provenance. A
general content-pack manifest and loader do not exist yet; `haxe_c-xge.20.4.1`
owns that planned capability. Until it closes, call the current layer a
compile-time content registry or asset manifest as appropriate, never a shipped
content-pack system. Generated adapters may bridge the gap only from validated
canonical data.

- A validated CaxeMap owns placed object identity, type, transform, tags,
  dialogue and message references, objectives, initial state, and rule wiring.
  CaxeFlow owns deterministic authored orchestration such as interaction,
  gifts, state transitions, signals, and encounter progression. The resolved
  content registry owns logical mechanic and presentation identities; the
  reviewed asset manifest owns actual visual bytes and provenance. Planned
  campaign data will own level membership and typed transitions. Saved state
  keeps stable IDs and mutable progress; it never stores translated prose or
  native pointers.
- Haxe owns reusable mechanics: movement, interaction distance, inventory
  transfer, combat, navigation, dialogue playback, rule execution, and other
  validated capabilities. When authored content needs a new mechanic, add one
  small typed engine capability and expose it through the shared registry and
  editor. Do not add a character-name branch, duplicate simulator, arbitrary
  script hook, or raw-C shortcut.
- Build runtime instances from the validated content model at the level-loading
  boundary. A runtime `Npc`, `Enemy`, `DialogueSession`, or similar class is
  appropriate when each value has real instance state, identity, and lifetime.
  Prefer composition and registered behavior over a subclass per character.
  Do not create a class merely because an authored entity is a noun, and do not
  use a static-only class as a substitute for either module functions or a real
  stateful instance.
- Character personality is authored data: localized dialogue, presentation
  profile, interests or tags, relationship and quest state, and selected
  registered behaviors. General algorithms remain Haxe. The data format never
  embeds arbitrary Haxe, C, shell commands, or host paths.
- The application composition root may select a built-in campaign or content
  package, but it must not reconstruct that package with hardcoded coordinates,
  IDs, dialogue stages, rewards, or encounters. Generated adapters may embed a
  validated package temporarily while native loading is incomplete; the
  package remains the sole editable source of truth and the adapter must be
  deterministic and replaceable by the ordinary loader.
- Tests exercise the same loader, runtime systems, and semantic input path used
  by the game. Focused fixtures may define tiny test content, but production
  engine code must not recognize fixture, character, map, or path names.
- Treat duplicated or content-specific application logic as migration debt.
  Record its owning Beads task and remove it through a vertical slice: load the
  authored data, construct the generic runtime value, execute the shared
  mechanic, render through logical presentation IDs, and retain focused plus
  flagship end-to-end evidence.

For example, Nia should be a `ScenarioObject.Npc` instantiated through the
generic NPC runtime. Her placement, dialogue, gift, and conversation progress
belong to the shipped map and CaxeFlow rules. A reusable NPC interaction system
may be a class if it carries per-instance state, but `GuideNpc` must not remain
a Nia-specific static API beside a second hardcoded copy in `Main.hx`.

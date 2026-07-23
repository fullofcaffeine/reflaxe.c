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

Follow the composition-first runtime direction in
[`docs/caxecraft-architecture.md`](../../docs/caxecraft-architecture.md).
Use the repository's
[`docs/haxe-code-architecture.md`](../../docs/haxe-code-architecture.md) when
choosing between records, abstracts, enums, classes, module functions, externs,
and macros. Keep the specific reason in the declaration's own HxDoc when a real
choice exists, so the source teaches the decision without requiring a separate
architecture reading.
`Main` is an executable entry, not the long-term owner of gameplay. A
`CaxecraftApp` owns the Raylib frame loop and screens; a `GameSession` owns one
loaded simulation and its fixed-step loop; one shared character model composes
movement, aquatics, vitals, inventory, equipment, combat, and interaction;
player, NPC, companion, enemy, boss, cutscene, and test controllers produce the
same typed intents for those systems. Runtime actors use stable IDs and typed
content-selected capabilities; deterministic systems receive only the state
they need; renderers observe read-only views. Use classes for identity,
lifetime, cohesive mutable aggregates, and resource ownership. Use records,
closed enums, and module functions for snapshots, inputs, results, events, and
stateless rules. A weapon, life rule, water rule, inventory operation, or
status effect that applies to more than one role has one shared Haxe
implementation. Do not create player/NPC copies, macro-injected copies,
role-name branches, or a subclass hierarchy where explicit composition and a
typed profile/controller are sufficient. Favor composition over inheritance.
Do not introduce a full entity-component system (ECS), service locator,
dynamic component bag, or
string event bus without the measured admission evidence required by that
architecture record.

Keep this rule evidence-bounded. CAXEMAP 1 already stores an `asset-pack`
logical path, `packs/caxecraft/base/content.json` is the strict schema-2 content
manifest, and its generated `BaseContentRegistry` is the typed validation
boundary used by CaxeMap and renderer-independent editor validation.
`assets/manifest.json` separately validates reviewed visual bytes and
provenance. The native game does not load arbitrary packs at runtime yet: a
build-time validator generates ordinary typed Haxe and packages the exact JSON
beside the executable. Call that boundary a validated built-in content pack,
not a general runtime loader, dependency system, or mod marketplace.

- A validated CaxeMap owns placed object identity, type, transform, tags,
  dialogue and message references, objectives, initial state, and rule wiring.
  CaxeFlow owns deterministic authored orchestration such as interaction,
  gifts, state transitions, signals, and encounter progression. The resolved
  validated content pack owns logical mechanic and presentation identities; the
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
- Connect each reusable capability to authored content through one explicit
  typed descriptor: stable ID, validated arguments/profile, execution owner,
  allowed CaxeFlow/cutscene/console/test authorities, editor presentation, and
  localization references. All surfaces must invoke the normal `GameSession`
  mechanic; none may maintain a private simulator or privileged behavior path.
- Prefer ordinary Haxe composition, nominal abstracts, closed enums,
  exhaustive matching, records, final classes, module functions, generics, and
  narrow interfaces before macros. A macro is admitted only for demonstrated
  repetitive compile-time glue such as deriving registry, codec/schema,
  editor, console, and test descriptors from one typed declaration. It must
  generate deterministic inspectable typed Haxe with source-positioned errors
  and must not copy gameplay behavior into entity types, hide update order,
  scan arbitrary methods, use runtime reflection, or execute user content.
  Macro-generated mixin-like forwarding requires specific evidence that plain
  composition is materially less clear; the ordinary-Haxe system remains the
  one behavior owner.
- Use one general event-to-action path for authored behavior. An engine event
  reports that something happened, optional CaxeFlow conditions decide whether
  a rule applies, and its ordered actions invoke any compatible admitted
  mechanic. A trigger volume, switch, timer, quest change, or cutscene must not
  grow a private scripting system or a feature-specific callback path. Extend
  the closed typed event/action registries and exhaustive executor instead.
- Guided cards, advanced visual logic, and text editing are views of the same
  validated CaxeMap/CaxeFlow model. Do not give text mode privileged mechanics,
  silently simplify a rule when returning to cards, or maintain a second file
  format for agents. “Script” means the bounded data-only CaxeFlow language
  here; it never grants arbitrary Haxe, C, Lua, shell, filesystem, or host API
  execution.
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

## Test Ownership and Automation

Write the rule being tested in Haxe whenever the rule belongs to Caxecraft.
This includes unit assertions for mechanics, parsers, content validation,
simulation, save data, and CaxeFlow. When that Haxe code uses only the admitted
target-neutral surface, run the same assertion entry point on Eval and through
haxe.c. Eval is a quick independent behavior check; generated native C remains
the product proof. The complete Raylib game does not currently run on Eval.

Keep host automation deliberately thin. Python may start Haxe and native
compilers, create isolated temporary directories, enforce timeouts, select an
available C compiler, launch processes, capture screenshots, and check a small
versioned result envelope. It must not calculate the expected water cells,
inventory contents, quest state, movement, or other game semantics again.
That would create a second implementation which could agree with itself while
the shipped Haxe code is wrong. Add a case to the shared
`run_haxe_c_test.py` runner instead of creating `check_<mechanic>.py` scripts.

A native C harness is allowed only as a small independent consumer of the
generated C boundary. It may call exported test functions and compare their
result with the Haxe-produced envelope; it must not reimplement the mechanic.
Direct C remains appropriate for independently authored ABI consumers, not for
repository-owned game behavior.

Use PilotScript for a small number of representative journeys through the real
game: start a level, issue player or console actions, observe stable semantic
state, and close cleanly. Pilots prove that loading, input, simulation,
rendering, and presentation are connected. They do not duplicate every Haxe
unit case or assert fragile individual pixels when semantic telemetry can state
the invariant directly. A water pilot, for example, should prove that a player
can enter authored water and that breath changes; the Haxe water tests own the
complete spread and recession rule table.

Use deterministic seeds, fixed simulation steps, explicit work budgets, and
bounded timeouts. Generated-C snapshots own compiler output shape, not game
correctness. Keep a fast focused command for each mechanic and retain the
flagship native/pilot lane for integration. Every shipped mechanic must be
reached by an appropriate combination of Haxe tests and representative native
game evidence; a test does not become stronger merely because its orchestration
is written in Python.

## Original Art and Audio Assets

Create original visual and audio assets on demand when an implemented game or
editor feature has a concrete need for them. An agent does not need separate
permission for each asset, but must not generate a speculative asset dump or
describe an unused file as integrated game content. Match Caxecraft's reviewed
voxel-adventure direction and the purpose of the scene, then inspect and test
the result before it becomes a project asset.

Music and sound should evoke a warm block-building adventure and a playful
fantasy role-playing game without copying another game's melody, sound,
instrument arrangement, logo, character, or other recognizable trade dress.
Use those genres as mood references, not as source material. The soundtrack
should eventually provide distinct original cues for at least the title/menu,
village safety, broad exploration, mystery or puzzle solving, underwater
travel, snow and ash regions, castle danger, Browser's boss battle, victory,
and game over. Transitions should support the player's changing mood instead
of treating one loop as background for the whole game. Sound effects should
share that identity across user-interface feedback, footsteps and materials,
blocks, inventory and pickups, water, creatures, Ivvy, combat, mechanisms, and
important story events.

Keep an editable source when it adds value. MIDI means the note, timing, and
instrument instructions used to edit a composition; it is not a recording and
Raylib does not play it directly. Preserve type-1 MIDI or another pinned source
project for suitable music, then export loop-ready Ogg Vorbis files for the
game. Preserve synthesis recipes or lossless masters for effects and export
short PCM WAV files. Do not add a runtime MIDI synthesizer merely to play the
editable source. Record the render tool and version, instruments or synthesis
recipe, export settings, loop points, license, and final hashes so another
developer can reproduce and audit the shipped bytes without an online service.

Every admitted asset must use a stable logical content ID and the reviewed
asset manifest. Gameplay and CaxeFlow request that ID; they never hardcode a
host path or own a Raylib resource handle. Update the cue sheet, semantic atlas
cells or audio metadata, provenance, rights, and focused validation together.
Generated design input is not runtime-integration evidence: only claim a cue,
effect, or image is playable after the ordinary loader/adapter, packaging, and
native game checks use it. Preserve existing contributor artwork and sources;
replace them only when the active work explicitly owns that change.

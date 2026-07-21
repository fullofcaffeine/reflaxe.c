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

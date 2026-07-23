# Caxecraft runtime architecture

Status: accepted migration direction; not yet fully implemented. Bead
`haxe_c-xge.20.4.2` owns the staged migration and executable evidence.

## Outcome

Caxecraft should read like a small, carefully designed game engine written in
Haxe. A contributor should be able to find:

- one application object that owns platform resources and screen changes;
- one explicit game loop;
- one game session that owns the loaded simulation;
- one shared character model composed from focused capabilities, with player,
  NPC, companion, enemy, and boss roles layered on top;
- one runtime actor path for NPCs, enemies, companions, and pickups;
- small deterministic systems for mechanics;
- presentation code that observes gameplay without changing it; and
- authored content that creates runtime values without character-specific Haxe
  branches.

The design is a **functional core with a stateful shell**. The functional core
contains calculations such as collision, breath, damage, and inventory
transfer: each takes explicit input and returns an explicit result. The stateful
shell contains a few real objects that own the live game, resources, and update
order. This uses object-oriented and functional styles where each is clearest.
The companion [Haxe code architecture guide](haxe-code-architecture.md)
explains how records, abstracts, enums, classes, modules, externs, and macros
express those roles and what haxe.c currently emits for each one.

## Why change the current shape?

The low-level mechanics are mostly cohesive and testable. For example,
`CharacterPhysics` owns collision while `Aquatics` owns water forces and
breath. Before the first character slice, the missing layer was above them:

- before the outer-owner slice, `app/Main.hx` was nearly 1,000 lines and
  imported about 100 modules;
- it directly coordinates motion, aquatics, vitals, inventory, equipment,
  combat, actors, feedback, input, fixed updates, rendering, and screens;
- focused records such as `CharacterBody`, `AquaticState`,
  `VitalsState`, and `InventoryState` are carried as unrelated variables;
  and
- Nia, the Mossling, drops, combat, and HUD rules still meet directly in
  the application loop, although validated content is intended to select those
  actors and interactions.

Small types are not the problem. Without a higher-level value, Adventure,
Ivvy, saves, audio, cutscenes, the editor, and a later second player would
multiply that coordination.

The implemented slice now has a `Character` value that composes body, aquatics,
capability, and vitals; a stable `EntityId`; a narrow `EntityStore`; and a
`PlayerAgent` that adds only the local-player binding. `GameSession` owns that
entity store, the water simulation, voxel storage, pending-water marks, and
authored-item flags. `CaxecraftApp` commits its local character through the session,
while the aquatics fixture advances a separate non-player character with the
same movement and damage functions. This proves the first composition and
storage boundary, but not yet the later multi-actor controllers,
inventory/equipment composition, content runtime, or read-only game view.

## The game loop

Caxecraft has one explicit outer frame loop and an inner fixed-step simulation:

```text
CaxecraftApp.run()
  while the application is open
    poll the window and input once
    add bounded elapsed time to the accumulator
    while one fixed step is available
      GameSession.tick(normalized intent)
      subtract one fixed step
    sample presentation from previous/current committed bodies
    render the read-only GameSession.view plus that visual position
    present audio and the frame
```

The outer loop owns Raylib, real frame time, focus, and device input.
`GameSession.tick` advances exactly one deterministic gameplay step. Rendering
may happen more or less often than simulation and must never insert a hidden
tick.

The application currently keeps the local character's previous and current
committed positions in a small `MotionHistory` value. Rendering interpolates
(moves proportionally) between them using the unconsumed fraction of the
accumulator. This makes walking look smooth while collision, block selection,
saves, telemetry, tests, and rules continue to use only authoritative
fixed-step state. The renderer deliberately trails that state by no more than
one 50 ms tick instead of guessing a future position.

`MotionInterpolation` has no Raylib or session authority. Its focused
`test:caxecraft-presentation` lane runs the same endpoint, midpoint, clamping,
adjacent-tick, and teleport/reset assertions on Eval and generated native C.
`CaxecraftApp` resets the history and the partial accumulator on spawn recovery,
pause/resume, focus loss, pointer recapture, and mode/start transitions. A reset
sets both endpoints to the same committed body, so the camera cannot glide
through walls or across a discontinuous transition.

## Small mental model

```text
validated campaign, level, and content
                 |
                 v
        LevelLoader -> GameSession
                          |
input adapter -> intent -> fixed simulation tick -> typed events
                          |                         |
                          v                         v
                    current state           audio/UI/telemetry
                          |
                          v
                   read-only GameView
                          |
                          v
                      renderer
```

Only the session changes gameplay. Input describes what a player wants.
Rendering, audio, the heads-up display (HUD), and telemetry observe committed
results.

## Ownership and dependency direction

| Layer | Owns | Must not own |
| --- | --- | --- |
| Platform adapter | Raylib window, devices, GPU/audio handles, frame clock, host I/O | quests, damage, inventory, actor behavior |
| Application | frame loop, screens, load/unload, pause/focus, resource lifetime | fixed-tick mechanics or authored character facts |
| Game session | world, entities, controllers, CaxeFlow runtime, fixed clock, update order | Raylib handles or translated prose |
| Simulation systems | one focused rule and typed result | rendering, host I/O, hidden global state |
| Runtime entities | stable identity and composed live state | character-specific source branches |
| Authored content | definitions, placements, profiles, text IDs, objectives, triggers | native pointers or arbitrary code |
| Presentation | world drawing, camera, HUD, animation, audio playback | simulation mutation |
| Save layer | versioned stable semantic state | resources, device state, temporary addresses |
| Editor/test driver | commands against shared content and runtime contracts | a second simulation implementation |

Dependencies point inward toward typed game meaning. Raylib depends on the
simulation's public view; the simulation never imports Raylib.

## Application and screen ownership

`Main` remains the executable entry function, but now only constructs one
`CaxecraftApp` and calls `run()`. The shipped application object owns the
complete current Raylib lifetime. Its `run` method remains a migration surface:
input sampling, presentation, and first-playable coordination still need
smaller owners, but they are no longer disguised as entry-point work.

- `CaxecraftApp` owns Raylib startup/shutdown, presentation resources, input
  sampling, the outer game loop, the current screen, and one final
  `GameSession`. Haxe.c embeds that session directly in the generated
  application struct; this ownership boundary adds no heap allocation.
- The shipped target-neutral `AppScreen` enum now represents the implemented
  `Title`, `Playing`, and `Paused` states instead of three mutable Boolean
  values. Its exhaustive functions own valid transitions and answer whether
  simulation pauses or the pointer is captured; `CaxecraftApp` performs the
  actual Raylib cursor calls. Add loading, editor, settings, or exit variants
  only when those screens and their tested transitions exist.
- A future `PlayScreen` owner will receive the `GameSession` plus the
  presentation-only camera and motion history that currently live in
  `CaxecraftApp`.
- `EditorScreen` owns an `EditorSession`.
- Level changes construct and validate a complete replacement session before
  publishing it, so invalid content cannot partially change the live game.

## GameSession: one loaded simulation

`GameSession` is a class because it has identity, lifetime, and cohesive
mutable state. A design sketch is:

```haxe
final class GameSession {
	final worldStorage:CArray<UInt8, WorldVolume>;
	final pendingWaterStorage:CArray<UInt8, WorldVolume>;
	final authoredItemStorage:CArray<Int, AuthoredItemSlots>;
	final entities:EntityStore;
	var localPlayer:PlayerAgent;
	var completedTicks:Int;
	public final water:WaterSimulation;

	public function tick(input:GameTickInput):GameTickResult;
	public function view():GameView;
}
```

This is the current shipped shape, abbreviated to show ownership. On C, each
fixed buffer is embedded directly in the generated session struct. On Eval,
ordinary Haxe arrays provide the same semantic surface for differential tests.
`tick` creates short-lived views internally and owns water, movement, aquatics,
health, commit order, and the deterministic completed-step clock; callers no
longer pass world storage or an authoritative tick number into the tick.

`CaxecraftApp` is the application composition root. Haxe's `@:allow`
friend-access rule lets it create scoped views while loading and rendering;
those views cannot be returned or stored, and the annotation emits no C
ownership or runtime mechanism. The runtime `LevelLoader` and read-only
`GameView` will replace that migration seam. The session now owns the shipped
`localPlayer` binding and deterministic completed-tick clock. It resolves the
binding internally and advances the clock only after a successful commit;
`view()` now publishes the first immutable local-character/clock snapshot.
The first presentation-owned `HudView` is also shipped: `CaxecraftApp` constructs it
from committed values after fixed updates, and HUD drawing receives that value
instead of a long positional parameter list. It is still a migration seam, not
a `GameSession` publication; world, actor, event, and `flow` views remain
planned rather than shipped claims.

The contract is:

- construction resolves required content before the session becomes visible;
- `tick` is the normal fixed-step mutation entry;
- all mutable state belongs to the session, not process statics;
- `TickResult` reports typed game and presentation events; and
- `view` exposes only committed data required for presentation.

The session is not a service locator. It passes systems only the state and
context their focused rule needs.

## One character model, composed roles

The live player should be one coherent value, but its movement, life, water,
inventory, equipment, and weapon rules must not be player-only implementations.
Player-controlled, artificial-intelligence-controlled, companion, cutscene, and
boss characters reuse the same primitive state and systems.

A design sketch is:

```haxe
final class Character {
	public final id:EntityId;
	public var body:KinematicBodyState;
	public var aquatics:AquaticState;
	public var vitals:VitalsState;
	public var inventory:InventoryState;
	public var equipment:EquipmentState;
	public var combat:CombatState;
}

typedef PlayerAgent = {
	final characterId:EntityId;
}
```

`EntityStore` owns each `Character` exactly once. `PlayerAgent` binds human
input, camera, and player-only policy to a character ID; it does not own a
second copy of that character. An NPC or companion binds the same character
model to an artificial-intelligence, dialogue, companion, or scripted
controller. Empty equipment, a disabled combat profile, water immunity, or no
usable inventory are ordinary validated capability states, not reasons to
create another movement, weapon, life, or water implementation. The exact
component names and storage are earned by vertical migrations.

The shipped `PlayerAgent` is deliberately an immutable typedef because it only
names the controlled character. Controller assignment, input buffering, and
later local-player state are planned capabilities, but a class conversion is
not predetermined. Promote the binding only if that work gives it persistent
mutable state and a meaningful lifetime; otherwise keep the smaller value.

The rules are:

- every character has one stable save/debug identity;
- player/NPC/boss roles select intent and optional capabilities rather than
  replacing common mechanics;
- focused records stay small, serializable, and independently testable;
- movement receives body, input, and collision world—not the whole player;
- inventory receives inventory and a typed request—not the whole session;
- the session applies returned component state in an explicit order; and
- UI receives a read-only `PlayerView`, not a mutable player reference.

Passing `PlayerAgent` or `Character` at an ownership boundary is useful.
Passing either into every function would create a new form of coupling. Shared
systems receive only their required components. This avoids both the current
loose variables and a giant `Player.updateEverything()` object.

The term **component** means one focused piece of live state here, such as
vitals or equipment. It does not imply that Caxecraft already has a general
entity-component-system framework.

## Reusable primitives and variation hooks

Primitive components and systems should make useful behavior emerge from their
combinations:

| Primitive composition | Shared result |
| --- | --- |
| controller + body + movement profile | player, NPC, companion, or cutscene movement through one collision system |
| body + aquatic response + breath profile | buoyancy, swimming, drowning, amphibious behavior, or immunity through one water system |
| combatant + equipment + weapon profile | the same sword, tool, projectile, or impact rules for any eligible player, NPC, companion, enemy, or boss |
| targetable + vitals + defense profile | shared life, damage, safe periods, defeat, and recovery |
| inventory + item definition | shared carrying, transfer, pickup, drop, and use rules |
| interaction + faction/tags | dialogue, friendly actions, hostility, targeting, and CaxeFlow conditions |

Use four deliberate hook points instead of arbitrary overrides:

1. A **controller** converts player input, AI, a cutscene, or a test script into
   the same typed intent.
2. A validated **profile** supplies bounded values and permissions such as move
   speed, breath capacity, weapon reach, or amphibious behavior.
3. A closed **modifier/effect** changes an admitted result in stable order, such
   as armor reducing damage or a powerup changing swim control.
4. A typed **event/action** reports the committed result to CaxeFlow,
   presentation, objectives, and audio.

These hooks are data and closed Haxe types, not arbitrary callbacks. They make
variation inspectable, deterministic, serializable, editor-visible, and easier
to lower into readable C.

Dependencies are injected explicitly at construction or function calls. For
example, a movement step receives a collision view and movement profile; it
does not retrieve them from a global container. Dependency injection here means
the caller supplies required collaborators. It does not mean adding a service
locator or dependency-injection framework.

## Runtime entities and composition

The player is special only where human input, camera/HUD ownership, or
single-player policy truly requires it. That does not justify a hierarchy such
as `Player extends LivingEntity`, mixins that copy weapon/water code into each
role, or one subclass per NPC/enemy.

Runtime entities instead have:

- a stable `EntityId` derived from validated scenario identity;
- a small character, world-item, trigger, or other closed runtime composition;
- small typed capability groups selected by validated content;
- registered behavior and presentation profiles; and
- mutable state owned by an actor store or cohesive actor instance.

Nia, the Fallskeeper, Ivvy, Mosslings, and Browser become authored compositions
of capabilities. Avoid a record with dozens of unrelated nullable fields. Start
with the smallest closed roles and component groups the shipped game needs.
Storage may later evolve behind `ActorStore` without changing content files.

Weapons demonstrate the rule. A content pack defines a weapon profile and
presentation IDs. An inventory/equipment component records who holds it. A
player, NPC, companion, or boss controller may request the same typed use
intent. `Combat.resolve` applies one hit, cooldown, damage, and event contract.
CaxeFlow may grant, remove, equip, or request an admitted use action; the editor
shows those operations from the same descriptor. No `PlayerWeapon`,
`NpcWeapon`, or script-only combat implementation is introduced. Vitals,
aquatics, inventory, interaction, and status effects follow the same pattern.

## Systems and state changes

A system is one focused rule in the fixed simulation. Stateless systems should
normally remain module functions:

```text
Movement.step(body, intent, profile, world) -> MovementStep
Aquatics.step(body, aquatic, profile, water) -> AquaticStep
Combat.resolve(attacker, target, attack) -> CombatResult
Inventory.transfer(inventory, request) -> TransferResult
```

The first shared vertical slice earned the role-neutral names
`CharacterPhysics`, `Aquatics`, and `Vitals`: both the local player and a
non-player fixture consume the same functions, and the focused Eval/native C
gate compares their results. Apply that same evidence rule to later systems.
Do not rename a player-specific mechanic merely because a future actor might
reuse it; add the second real consumer and retain its executable proof first.

Use a class when a subsystem truly owns state across calls, such as
`GameSession`, `ActorStore`, `WaterSimulation`, `CaxeFlowRuntime`, a loaded
resource set, or `EditorSession`. Do not use a static-only class as a namespace,
and do not create an instance merely because a mechanic has a noun.

## Typed events and commands

Use direct calls inside one owner. Use a typed boundary when one system must
report a fact that independent systems consume.

- A **game event** states what happened, such as `ActorDefeated` or
  `ItemCollected`.
- A **game command** asks the session to make a structural change at the defined
  phase boundary, such as spawn, despawn, grant an item, change level, or start
  a cutscene.

Both are closed Haxe enums or validated records. They are not strings, dynamic
maps, callbacks hidden in content, or a global event bus. Events must not replace
an obvious local return value.

CaxeFlow consumes admitted game events and plans actions through its one typed
action registry. Console commands, CaxeTest/PilotScript, cutscenes, and authored
triggers reuse that command/action boundary where their meanings overlap.

## Fixed-tick order

Update order is observable behavior. The initial planned schedule is:

1. accept normalized player intent;
2. advance bounded environment work such as water;
3. evaluate player and actor intent/artificial intelligence;
4. resolve movement and collision;
5. resolve interactions, pickups, tools, and combat;
6. advance vitals, breath, cooldowns, and status effects;
7. publish typed events;
8. let CaxeFlow plan admitted actions;
9. apply structural commands in stable order; and
10. commit state and presentation events.

A change to this order needs a focused semantic test explaining why.

## Rendering, HUD, camera, and audio

Presentation consumes a read-only `GameView` plus the frame interpolation
fraction. It owns camera placement, drawing, animation selection, HUD/menu
layout, and sound playback from logical IDs. It does not decide health, pickups,
underwater permissions, dialogue progress, or actor behavior.

The shipped HUD boundary now receives one immutable, Raylib-free `HudView`
instead of thirty unrelated arguments. A separate C-only `HudResources` value
borrows the atlas handles for the draw call; loading, unloading, and readiness
remain application responsibilities. The current `HudView` still carries
explicit first-playable guide and Mossling observations. Those fields are a
documented content-migration seam, not a claim that authored prompts already
come from runtime content packs.

## Content, loading, saves, and editor

```text
Caxe campaign/world/level + content packs
        -> parse and validate
        -> resolve stable IDs and profiles
        -> construct complete GameSession
        -> publish only on success
```

Content selects registered mechanics; it cannot embed arbitrary Haxe, C, shell
code, or pointers. Runtime loading is owned by `haxe_c-xge.39`, and
content-to-actor composition by `haxe_c-xge.20.4`.

One engine capability must connect all authoring surfaces without becoming
five implementations:

```text
ordinary typed Haxe mechanic
        -> explicit typed descriptor and execution owner
        -> validated content profile or CaxeFlow action
        -> editor card/text completion + console/test authority views
        -> the same GameSession system at runtime
```

For example, adding a vehicle first adds a reusable Haxe mechanic and typed
intent/result. Its descriptor then states the stable ID, argument schema,
allowed callers, editor labels, and validation rules. CaxeFlow, cutscenes, the
console, and CaxeTest may expose different authorized views, but all reach the
same session command and mechanic. The editor never reimplements the vehicle,
and authored data never installs executable callbacks.

Saves contain versioned semantic state: level/entity IDs, character components,
actor state, world changes, CaxeFlow state, objectives, and deterministic
clocks. They exclude Raylib handles, translated prose, input devices, temporary
addresses, and interpolation-only values. The editor and game share the same
loader, validation, runtime construction, and commands.

## Choosing OOP or functional style

| Prefer a class instance when | Prefer a record/enum/module function when |
| --- | --- |
| the value has stable identity or lifetime | it is a snapshot, input, result, or event |
| it owns cohesive mutable state across calls | the rule is a deterministic transformation |
| it owns a resource that must be released | the data should serialize/compare directly |
| construction/disposal has invariants | the concept is a closed set of variants |
| callers benefit from one aggregate | independent identity adds no meaning |

Inheritance is allowed only when substitutability is the real relationship.
Variation and optional behavior normally use composition: `Character has
Vitals` is clearer than a deep
`DamageableLivingActor` hierarchy.

This design does not need a mixin convention initially. Mixins tend to copy or
inject behavior into several types, obscure ordering, and make generated
ownership harder to inspect. Use shared systems plus explicit
components/profiles.
Interfaces remain useful at narrow boundaries when multiple implementations
truly share one contract, such as an input provider or storage adapter; they
are not the default mechanism for reusing gameplay code.

## Haxe features and macro admission

Use the least surprising Haxe feature that expresses the rule completely:

1. nominal abstracts for stable IDs, units, and validated scalar values;
2. closed enums and exhaustive matching for intents, effects, actions, events,
   roles, and failure reasons;
3. records and final classes for focused data and cohesive ownership;
4. module functions and generics for reusable deterministic algorithms; and
5. narrow interfaces only where callers genuinely need interchangeable
   implementations.

A compile-time macro is justified only after ordinary typed Haxe exposes
repeated mechanical glue. The strongest candidate is deriving several views of
one already-typed capability declaration: registry entries, codecs and schema,
editor palette facts, localization references, console help, and test-driver
descriptors. The macro runs while building the engine; it does not run user
content or turn CaxeFlow into arbitrary Haxe.

Do not use a macro merely to simulate inheritance or make a familiar method
appear on every entity. If a macro-generated mixin is ever proposed, it must
first prove that explicit composition and forwarding are materially worse. Its
expanded fields and methods must be deterministic, collision-checked,
source-positioned, inspectable in tests, and limited to forwarding or deriving
facts; shared state and behavior still have one ordinary-Haxe owner. A macro
must not hide update order, allocate unexpected runtime objects, scan arbitrary
methods, use runtime reflection, or generate a second gameplay implementation.

Every admitted macro follows the repository macro contract: typed inputs and
outputs, malformed-input diagnostics at the authored source, deterministic
expansion, no hidden file or network access, documented allocation/ownership
and portability effects, Eval and generated-C evidence, and a before/after
boilerplate inventory. If the explicit declaration remains clearer, keep it.
Bead `haxe_c-xge.19.9` owns this decision for the shared action catalog; the
runtime component migration must not invent a competing macro system.

## Why not a full ECS yet?

An entity-component-system (ECS) represents an entity as an ID, stores typed
data components, and runs systems over matching components. Caxecraft borrows
the useful ideas—IDs, composed state, focused systems, and a schedule—without
first building a query language, archetype storage, reflection registry,
dynamic component bag, or parallel scheduler.

The current demonstrated problem is ownership, not query throughput. A fuller
ECS becomes a justified research decision if evidence shows one or more of:

- several systems repeatedly scan a heterogeneous actor population for
  different component combinations;
- actors add/remove capabilities often enough that closed roles become awkward;
- profiling identifies actor layout or iteration as a frame-time/cache problem;
- editor/modding needs generic typed component inspection; or
- multiplayer/rollback needs bulk snapshots for many entities.

Then compare a small target-owned store, a Haxe ECS library, and Flecs through
typed bindings. Do not select one by fashion or entity count alone.

## File and module organization

Organize by feature and dependency, not declaration kind:

```text
caxecraft/
  app/                 startup, screens, outer game loop
  simulation/
    GameSession.hx
    events/
    characters/
    control/
    actors/
    world/
    systems/
  presentation/        renderer, HUD, camera, audio-facing views
  platform/raylib/     resources, input, drawing, playback
  content/             definitions, resolution, runtime construction
  scenario/            CaxeMap and CaxeFlow models
  editor/
  pilot/
```

This is a migration guide, not a mechanical package rename. Move one ownership
slice only when behavior and generated C are being verified. Small independent
types may keep their files, but each feature needs one obvious public entry so
the application does not import every implementation detail.

## Incremental migration

1. Freeze Eval, native, PilotScript, snapshot, and timing evidence.
2. Define shared character primitives and bind the local `PlayerAgent` to one
   stored character without changing mechanics. Prove the shape with the
   current movement/aquatics/vitals slice and a focused non-player fixture; do
   not wait for every future mechanic before introducing the session.
3. Extend the shipped `GameSession` slice: it already owns entity/water state,
   fixed world/work/item buffers, and the fixed water/character tick; move the
   remaining fixed-tick mechanics out of `CaxecraftApp` as their shared system
   boundaries are admitted, then replace friend buffer access with the runtime
   loader and read-only view.
4. Add only the typed events/commands required by real cross-system effects.
5. Instantiate generic actors from validated content. Generalize inventory,
   equipment, weapons, interaction, effects, and any remaining character rule
   as each gains a real non-player consumer. Expose only admitted
   descriptor-backed operations to CaxeFlow/editor/test surfaces; remove
   Nia/Mossling application branches under `haxe_c-xge.20.4`.
6. Extend the shipped first `GameView` and `HudView` slices, add `PlayerView`
   when it has a focused consumer, and separate rendering/audio.
7. Keep the shipped outer loop in `CaxecraftApp`; separate its input,
   presentation, and screen implementations as real consumers appear. Extend
   the closed `AppScreen` state only when real screens arrive.
8. Connect runtime loading, saves, editor test-play, cutscenes, and later
   multiplayer through these boundaries.

Each step is a behavior-preserving vertical slice. Do not combine it with a
repository-wide package move, full ECS, new campaign, and broad snapshot refresh.

## Compiler and QA obligations

This architecture intentionally exercises ordinary Haxe classes. Every slice
checks constructor, field, method, identity, and null behavior; escape/allocation
decisions; readable split C; runtime features; generated size; compile time;
Eval/native traces; strict GCC/Clang; sanitizers; and the real Raylib pilot path.

If the natural Haxe design exposes a compiler defect, reduce and fix the
compiler. Do not flatten the application back into unrelated scalar locals.

## Rejected alternatives

- **Keep growing one application method:** every feature learns every other
  state variable even if the entry class has a better name.
- **One giant mutable `Game` or `Player`:** hides fragmentation in god objects.
- **Inheritance-heavy actors:** couples authored variation to source classes.
- **Immutable whole-world copies everywhere:** complicates voxel state and
  resource lifetime without a demonstrated benefit.
- **Full ECS immediately:** adds storage/query/scheduling machinery before its
  scale problem exists.
- **Raylib objects throughout gameplay:** blocks clean saves, headless tests,
  portability, and deterministic semantics.

## Research basis

- [Godot scene organization](https://docs.godotengine.org/en/stable/tutorials/best_practices/scene_organization.html)
  supports a clear entry point, separate world/GUI ownership, focused systems,
  and parent-supplied dependencies.
- [Godot nodes and scenes](https://docs.godotengine.org/en/stable/getting_started/step_by_step/nodes_and_scenes.html)
  shows focused behavior composed into coherent game objects.
- [Bevy ECS concepts](https://docs.rs/bevy/latest/bevy/ecs/index.html) define
  entities as IDs, components as ordinary data, systems as functions, unique
  resources, and explicit schedules.
- [Flecs entities and components](https://www.flecs.dev/flecs/md_docs_2EntitiesComponents.html)
  provides a C-side comparison and demonstrates the additional machinery of a
  general ECS.

These sources support composition, ownership, and scheduled systems. They do
not prove Caxecraft needs their complete node tree or ECS implementation.

## Oracle threshold

An Oracle review is not needed to choose this direction. The current coupling
and established patterns give us a reversible, incremental path.

An Oracle review becomes worthwhile after the shared-character and
`GameSession` migrations produce generated-C, allocation, compile-time, and
update-time evidence, if we must then choose among a target-owned actor store,
a full ECS, a rollback-oriented snapshot model, or a foreign C ECS. Give the
Oracle profiles, actor/query counts, save/multiplayer requirements, and
generated C. Asking now would mostly collect framework preferences rather than
resolve measured pressure.

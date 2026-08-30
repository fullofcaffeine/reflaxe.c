# Caxecraft editor semantics

Status: the renderer-independent command, history, validation, save, and
test-play layer is implemented under `haxe_c-xge.19.5` and
`haxe_c-xge.19.6.3.12`. The native Raylib/Raygui screen opens the same CAXEMAP
bytes as the active game generation. Build shows the map's visible height
surface with the terrain renderer and atlases from ordinary play. It also shows
a grid at the selected layer. Plan shows the exact cells and objects that cross
that layer. The screen shows the CaxeFlow rule count and each object's stable
ID. Build uses validated pack art for NPCs, entities, items, and stateful
objects. Player starts and checkpoints use clear editor markers.

The editor core provides revision-checked changes, bounded command groups, and
copy-owned observations. Its World Name field commits literal titles through
the same command and history boundary. Tab and Shift-Tab move through one
device-neutral focus order. Enter or Space activates the focused control. A
high-contrast ring shows the next target.

The Text button, or `T`, opens the complete canonical CAXEMAP document. Source
edits stay separate until Apply + Format succeeds. Invalid or stale text stays
editable and cannot replace the visual draft or its last playable snapshot.

The native screen can select and edit each horizontal voxel layer. The minus
and plus controls change only presentation state; they do not create history,
mark the package dirty, or alter Test Play. It moves and rotates selected
objects and edits the world environment through typed commands. Its Test Play
button starts a fresh ordinary game level from the editor's in-memory CAXEMAP
bytes. The editor keeps the draft, camera, selection, tools, panels, history,
and recovery state.

Save and Ctrl/Cmd+S validate the draft, replace its package level, and update
the campaign, runtime-content, and outer-package receipts as one planned
operation. The native editor uses one package-backed `EditorSession`; Save and
the visual controls cannot drift into separate drafts. A failed publication
keeps the draft, history, and previous clean-state marker so the creator can
retry. The current visual CaxeFlow slice edits canonical rules; the broader
child card library and cutscene editor remain separate work. A local JSON Lines
process can also open, inspect, edit,
validate, and save one verified package level.

## What this layer owns

The editor core answers “what does this edit mean?” without knowing how a
button, mouse drag, controller focus ring, or voxel highlight is drawn. Both the
headless test and the current first Raylib viewport use the same public
operations:

```text
Scenario -> EditorSession -> closed EditorCommand -> updated draft
                         \-> validate -> last playable snapshot
                         \-> local test -> disposable CaxeFlow simulation
                         \-> native Test Play bytes -> ordinary game runtime
```

An `EditorCommand` is a closed Haxe enum. Closed means the possible edit kinds
are listed in the type rather than encoded as strings. Haxe can therefore make
callers handle new command kinds intentionally. The first command set covers:

- the authored scenario title, with a stable changed-title identity;
- bounded world resize, palette entries, fluid sources, and initial fluid
  volumes;
- single-voxel and bounded multi-voxel paint/erase plus selection fill;
- bounded selection and clear-selection;
- typed prefab stamps and general object placements;
- dialogue, objective, and CaxeFlow rule replacement/removal; and
- explicit recovery to the last validated playable scenario.

The command model deliberately contains no Raylib event, C pointer, file path,
raw target text, or `Dynamic` payload. The native screen translates pointer and
Raygui tool input into these commands. `PaintVoxels` and `EraseVoxels` commit
one bounded drag as one edit, rather than creating history on every rendered
frame.

## One command boundary for humans and automation

The visual editor primarily serves human and child creators. It must make
terrain, objects, triggers, flow links, cutscenes, and transitions visible and
selectable. Agents can edit the canonical Caxe files directly.

The visual editor and an optional automation client must not have different
rules for changing a map. Both use `EditorSession.mutate`, which combines an
operation with the draft revision its caller inspected:

```text
query state -> revision 12
                  |
                  v
mutate(base 12, commands [place house, place trigger, connect rule])
                  |
          +-------+--------+
          | all succeed    | any command fails
          v                v
   revision 13       revision remains 12
   one undo entry    no draft/history change
```

A **revision** is a non-negative counter for committed draft changes. A caller
includes its last observed revision with a mutation. If the editor is already
at a newer revision, it returns `RevisionConflict` before applying anything.
This prevents an agent, delayed UI gesture, or second editor view from
overwriting work created after it last read the draft. Successful single edits,
transactions, undo, and redo each advance the counter once. Rejected and
semantically unchanged work does not.

An **atomic transaction** means a group of typed `EditorCommand` values becomes
visible all at once or not at all. `EditorSession` applies each command to a
private deep scenario image, passes every intermediate result through the
ordinary reducer and canonical CAXEMAP snapshot boundary, and publishes one
history entry only after the complete group succeeds. A later failing command
therefore cannot leave an earlier terrain, object, or rule edit behind.
Transactions are limited to 128 commands by default and can never exceed 256;
the limit bounds one request's CPU, memory, and history work.

`EditorSession.query` is the matching read-only side. Its closed `EditorQuery`
values return small session state, a deep typed draft, copied canonical bytes,
or a deterministic campaign tree. The tree is a flat list with explicit typed
parent references: a toolbar can draw it hierarchically without receiving
nested mutable arrays, and each palette, chunk, fluid, object, story record,
flow record, locale, message, and extension retains its real semantic ID.
`InspectNode` returns one compact row by that typed ID, while
`InspectValidation` returns fresh playable bytes or source-linked diagnostics
without updating the session's last-known-playable recovery snapshot. Every
observation carries the revision it describes. Returned arrays, diagnostics,
and bytes belong to the caller, so changing them cannot change the live editor.

An applied mutation also returns a typed `EditorChangeId` list. For example,
two replacements of the same object in one transaction report that object once
in first-command order. The exact list is stored with the history entry, so
undo and redo can identify the restored object set without asking a UI or
automation adapter to compare two complete maps. Whole-document recovery says
`ChangedDocument` explicitly because pretending to know a narrower change
would be misleading. The native 3D screen sends voxel edits, undo, and redo
through this revision-aware path.

The local JSON Lines adapter now uses this implemented boundary. It opens one
verified package level and returns one JSON response for each input line. It
supports state, terrain-surface, and single-column queries. It also supports
bounded paint, erase, undo, redo, validation, save, and quit commands.

The save command uses the same content-refresh planner as normal content work.
It validates the complete draft before it replaces the map and its receipts as
one group. A stale revision or invalid draft leaves all package files unchanged.

Run this command from the repository root:

```sh
npm run caxecraft:editor -- \
  --level scenarios/first-adventure/frostmere.caxemap
```

The process first writes a `ready` response. Then send one request per line:

```json
{"schemaVersion":1,"requestId":1,"command":"column","x":54,"z":23}
{"schemaVersion":1,"requestId":2,"command":"erase","baseRevision":0,"points":[{"x":54,"y":5,"z":23}]}
{"schemaVersion":1,"requestId":3,"command":"validate"}
{"schemaVersion":1,"requestId":4,"command":"save","baseRevision":1}
{"schemaVersion":1,"requestId":5,"command":"quit"}
```

This process is local and uses standard input and standard output. It opens no
network port. A later MCP adapter can translate tools into the same protocol.
It must not add a private mutation path or unrestricted host access. Issue
`haxe_c-xge.19.6.3` owns the broader visual authoring surface. The adapter is a
lower-priority convenience because direct file editing remains supported.

## First 3D visual viewport

The first 3D slice lets a creator see depth, fly around the finite voxel world,
and use Select, Paint, Erase, or Fill without creating a second editable copy
of the map.

```text
current CAXEMAP draft
    |
    | accepted edit / undo / redo / New World
    v
cached read-only voxel volume
    |
    +-- playable shape -> fixed-layout presentation copy -> TerrainRenderer
    +-- custom shape -> compact surface fallback
    +-- camera ray -> visible voxel or empty floor cell
                         |
                         v
                 EditorCommand -> EditorSession.mutate
```

A **projection** means a read-only shape prepared for presentation.
`EditorWorldViewport.projectWorld` decodes the complete finite draft into one
volume ordered as `(z * height + y) * width + x`.
`CaxecraftEditorScreen` caches that projection. For a playable world shape, it
also resolves palette codes into the fixed byte order used by gameplay. Build
passes this read-only copy to the ordinary `TerrainRenderer`. This gives Build
the same textured terrain while the CAXEMAP draft remains the only editable
world.

The projection also contains a compact height surface. This surface is a
fallback for custom-size or incomplete drafts that gameplay cannot represent.
Exact voxel cells remain available for picking and edits in both paths. Plan
reads one exact horizontal slice, so a creator can inspect hidden cells.

A normal displayed frame reads both caches. It does not serialize the CAXEMAP
draft or allocate a replacement volume. New World, an accepted edit, undo, or
redo rebuilds both caches from the session's new draft.

Moving the pointer between cells also reads this cache. The screen translates
the selected tool into a possible command, but it does not serialize the map or
run a complete transaction for each new hover cell. A green placement ghost
means that the visible gesture has its required local inputs. The click remains
authoritative: `EditorSession.mutate` checks the revision, reducer, canonical
format, and history budget before it changes the draft. If one of those checks
rejects the command, the draft remains unchanged and the editor shows the
invalid state.

Voxel edits rebuild their changed world arrays from scalar coordinates and
palette codes. They retain no caller-owned records. The session can therefore
write canonical bytes and record history without parsing those bytes again on
the click path. Validate, Save, and Test Play reconstruct exact source
coordinates from the canonical bytes before they report diagnostics. Commands
for placement deep-copy each retained record and tag array. These commands also
defer the parse. Other structured commands keep the complete write-and-parse
boundary.

The interaction hierarchy puts direct in-world editing first. Creators can
point at textured terrain, place or remove cells and objects, and see the result
immediately. Authored NPCs, enemies, items, and mechanisms use the same
validated atlas cells as ordinary play. Exact selection bounds appear when an
object is selected or targeted.

Build has one Ground card because the two mouse buttons select the terrain
operation. The left button removes terrain. The right button places terrain.
Plan keeps separate Ground and Erase cards for precise work.

The Things to Add card opens a searchable asset browser. Press `B` from either
editor view to open or close it. The browser lists every admitted terrain
material, item, character, enemy, and editor mechanism. Its rows come from the
validated content pack; translated names and help come from the UI catalog.
Choosing terrain selects the Ground tool and adds a normal undoable map-palette
entry only when the map does not have that material. Choosing another row
selects the ordinary object-placement tool.

The Text button opens advanced source authoring over the same scenario. Press
`T` from Build or Plan to open or close it. Select a numbered row, edit its
complete line, then use Apply + Format to publish valid source as one undoable
document change. Add Line and Delete Line change only the isolated source
draft. Reset from Visual discards those source-only edits.

Apply runs the public UTF-8 decoder, lexer, parser, semantic validator, and
canonical writer. A source-located diagnostic selects the first failing line.
Malformed, incomplete, oversized, stale, or semantically invalid source stays
in Text and leaves the visual model, history, selection, and Test Play snapshot
unchanged. A successful Apply refreshes every visual view from the one typed
scenario. Syntax colors distinguish structural records, flow events,
conditions, and actions. Registry completion and jump-to-world references
remain planned work.

Plan is an advanced tool for hidden layers, trigger volumes, logic links, large
selections, and fast navigation. It is not the default authoring experience.
Build shows trigger bounds when the trigger tool is active or the creator
targets that trigger. Plan keeps these volumes visible for precise work. Issue
`haxe_c-xge.19.6.3` owns the remaining navigation and authoring work.

History assigns a small state identity to each accepted edit, undo, and redo.
Save records that identity only after publication succeeds, so a normal frame
does not serialize the draft to decide whether it is dirty. The CAXEMAP lexer
also splits the decoded document once before it tokenizes short lines. This
avoids repeatedly scanning the full UTF-8 document from its beginning during
an interaction.

Raylib turns one screen pixel into a **ray**: a starting point and direction in
the 3D world. `EditorWorldViewport.pickWorld` enters the finite map once. It
then visits only the voxel cells crossed by that ray. Map size no longer
controls hover-picking cost.

The picker returns the nearest solid cell. If it finds no solid cell, it checks
the floor of the current edit layer. This path lets a creator paint an empty
cell. The selected tool converts the typed `VoxelPoint` into the same
`EditorCommand` that history uses. An invalid pick changes nothing.

Build now starts with the pointer released so the creator can use the toolbar.
A click inside the 3D world captures the pointer and changes Build to direct
first-person control. The first click only enters the world; it does not also
apply a tool. A centered crosshair then owns the target ray. This removes the
old need to hold the right mouse button while looking.

The current controls are:

- use Tab and Shift-Tab to move the visible focus ring through editor controls;
- press Enter or Space to activate the focused control;
- use a connected controller's D-pad or left stick to move the same ring;
- press the south face button to activate or the east face button to return;
- click inside Build to capture the pointer, then move the mouse to look;
- press Escape once to release the pointer; press it again to cancel the
  selected tool or leave through the normal editor flow;
- press 1 through 5 to choose the five visible Build cards;
- press B to open or close Things to Add;
- press T to open or close the complete CAXEMAP Text workspace;
- in Things to Add, use left/right to change category, up/down to choose a row,
  Enter to use it, and Tab to enter or leave search text;
- use W/S to move forward/back, A/D to strafe, and Q/E to move vertically;
- use the wheel to move along the view direction;
- press F to restore the deterministic whole-world view;
- use the minus and plus layer controls to move the Build grid and Plan slice;
  and
- left-click at the crosshair to remove terrain or use the selected object tool;
  and
- right-click at the crosshair to place terrain.

Switching to Plan, opening the environment panel, starting Test Play, leaving
the editor, or losing window focus also releases the pointer. Plan keeps its
free pointer because it is the precise overview for hidden layers, trigger
volumes, logic links, and navigation. Build hotkeys change only the active
tool. They do not change draft bytes or history until a click is accepted.

The focus order is target-neutral: it names editor actions, not Raylib key
codes, controller brands, or screen coordinates. Keyboard and controller
adapters produce the same small navigation commands. Up/left move backward,
down/right move forward, Confirm activates, and Cancel returns to the title.
The list control then moves its own selected item when it receives activation.

Controller sampling and interaction policy have separate owners:

```text
Raylib device state
  -> normalized direction/button snapshot
  -> dead zone + held-direction repeat
  -> device-neutral navigation command
  -> editor focus/action
```

A **dead zone** is the quiet center of an analog stick. Caxecraft ignores the
inner 35% so small hardware drift cannot move focus. A new direction moves
immediately, repeats after 350 ms, then at 100 ms intervals. One rendered frame
can produce at most one repeated move, so a stall cannot jump across several
controls. Releasing the stick, pressing opposite directions, changing screens,
or disconnecting resets the repeat clock. Reconnecting therefore starts with
one fresh move instead of inheriting an invisible held action.

The deterministic Haxe editor probe owns those timing and disconnect rules on
Eval. The real native editor pilot feeds controller-shaped snapshots through
the same repeater and screen handler, then verifies the visible focus ring and
activation result. The pinned Raylib integration separately compiles strict
C11 and C++17 consumers of the exact gamepad ABI. No automated test claims to
press a particular physical controller through the host operating system.

The localized viewport heading shows these controls in English and
Mexican Spanish. Camera state and the current hover are presentation values:
they are not saved in CAXEMAP, do not participate in undo, and cannot mutate
terrain. Raylib's scissor region clips all 3D drawing to the canvas, so even a
nearby voxel cannot cover the toolbar, sidebar, or status bar.

The renderer-independent `EditorViewport` module owns exact top-down layer
projection and pixel-edge mapping. The shipped Plan view uses it directly.
Layer changes copy one compact slice from the cached volume instead of parsing
or serializing CAXEMAP again.

Opening the editor starts from a copy of the active runtime generation. The
copy prevents an editor change from changing the running game without a
validated publication step. The New World button creates a separate 12 by 1 by
12 draft. Caxecraft supplies its Air and default-editor block IDs at the
application edge. The reusable editor factory does not know campaign names.

## Draft versus playable scenario

An editor must let a person pass through an incomplete state. Removing the only
player spawn, for example, is a useful edit even though the result cannot be
played yet. `EditorSession` therefore keeps two independent values:

- the **draft**, which may temporarily fail semantic validation; and
- the **last playable snapshot**, updated only after the complete draft passes
  `ScenarioValidator`.

The session never exposes its mutable arrays directly. `draftSnapshot()` and
`lastPlayableSnapshot()` return deep in-memory copies. An invalid draft does
not replace the last known-good scenario, and test play never silently falls
back to old content: pressing Test validates the current draft and reports its
exact typed diagnostics if it is not playable.

`ScenarioWriter` provides the deterministic byte spelling used for in-memory
draft copies as well as valid saved scenarios. Structural serialization alone
does not declare a draft playable. Persistence must first pass
`ScenarioValidator`; the future native persistence layer then owns temporary
files, flushing, atomic replacement, and cleanup.

## Exact undo and redo

Each accepted command records canonical CAXEMAP bytes before and after the
edit, plus the selection before and after it. Re-reading those bytes provides a
deep copy and makes undo/redo exact even for temporarily invalid drafts. It also
exercises the same public codec used by imported maps instead of adding a
private object serializer.

History is bounded in two ways:

- at most 64 entries; and
- at most 64 MiB under the absolute policy, with a 32 MiB default.

Both undo and redo share those limits. A new edit discards the redo branch and
evicts the oldest undo entries deterministically. If one entry cannot fit the
configured byte budget, the command is rejected before the draft changes. The
byte figure counts the exact before/after CAXEMAP payload; the separate entry
bound also caps the small bookkeeping and selection records around those bytes.
Adjacent entries share their private immutable state buffer instead of copying
the same middle state twice. This reduces interaction-time allocation without
weakening the logical byte budget or exposing mutable history storage.
Selections have their own 65,536-cell absolute bound. The same setting limits
the number of points submitted by one batch paint or erase gesture, including
duplicate points; it is the editor's shared “one gesture” work budget. Smaller
bounds may be selected when opening a session, which makes device- or
mode-specific limits testable without weakening the format limits.

The separate transaction-command bound limits how many typed commands may be
grouped into one all-or-nothing edit. It does not raise the voxel gesture,
selection, history-entry, or history-byte bounds: every command inside the
transaction must still satisfy those existing limits.

The editor keeps exact canonical bytes after each accepted edit. Voxel and
placement reducers also own all changed arrays and records. These reducers can
defer the parse that restores source coordinates until validation, Save, or
Test Play needs it. Other commands still use the complete write-and-parse path.

This boundary reduced a repeated Frostmere object rotation from an 18.811 ms
median to 2.144 ms on the same loaded development host. The diagnostic used 60
accepted edits before and after the change. This result describes interaction
latency on that host, not an uncontended compiler or game benchmark.

The native screen does not request a complete scenario copy after an accepted
edit. It asks `EditorSession` for fresh presentation values from the retained
draft. Exact history continues to use canonical before-and-after bytes.

If history snapshots become the next bottleneck, a later slice can introduce
typed command-specific inverse data. It must retain bounded paint gestures,
exact undo bytes, hard memory limits, and the same public commands. A later
optimization must not trade correctness for an unmeasured speedup.

## Reversible test play

The editor core keeps a small renderer-independent CaxeFlow test. The
`enterTestPlay()` function deep-copies a valid scenario and creates a new
`CaxeFlowExecutor`. The `leaveTestPlay()` function removes this local test.

The native button uses a separate ordinary-engine path. First, the editor
returns copy-owned canonical bytes without changing `lastPlayable`. Then the
application uses the normal parser, resolver, `GameSession`, and presentation
checks. The application locks editing only after all these checks pass.

Escape or focus loss removes the disposable runtime before another game tick.
The application then restores the exact normal play state and returns to the
same editor object. A second start creates a new runtime generation.

One process-owned sequence issues generation IDs for normal loads, campaign
preloads, transitions, and Test Play. A rejected load still consumes its ID.
Therefore, renderer caches cannot confuse a later level with rejected or
retired content. This is a process-lifetime guarantee, not a persistent or
cross-process identity. If the positive integer range is exhausted, allocation
returns an invalid ID and level construction fails before publication.

There is intentionally no “keep whatever happened while playing” operation in
this version. Importing selected play changes later would need its own closed
command and clear ownership rules. Silent import would make a test run mutate
the map and defeat reversible experimentation.

## Current visual event authoring and planned cinematic depth

The native editor will not have separate trigger systems for doors, encounters,
music, quests, and cutscenes. They all use one CaxeFlow relationship:

```text
event source -> conditions -> ordered actions
```

A spatial volume is one event source. The world view can place, name, resize,
duplicate, and select its visible gizmo. A selected volume shows localized
WHEN/IF/DO cards projected from the canonical typed rule. The current compact
controls can switch enter/leave, replace a condition with a picked event actor
or `Always`, insert a picked-object `Spawn` action, switch `Spawn` and
`Despawn`, reorder or remove actions, and replace compatible object references
by selecting the object in the world. The renderer-independent command also
supports replacing any closed event, predicate, or admitted action plus changing
priority and repeat policy. Every gesture commits the ordinary `PutRule`
command, so Undo, Redo, validation, save, and Advanced mode observe the same
CaxeFlow data. A world-pick gesture records the document revision and rejects a
stale target instead of applying it to a changed card.

The shared renderer-independent trace projection bounds and classifies source,
predicate, action, and deferred-event rows. Native Test Play retains the latest
non-empty bounded trace and draws it under the selected cards after the creator
returns. A live overlay while the game screen is still open, polished
registry-driven field forms, filters/enabled controls, and non-world document
pickers remain planned work. The current screen is a complete minimal spatial
rule editor, not yet the complete child-facing card library.

The same typed draft has three authoring depths. **Guided** mode uses large
icon-and-sentence cards, templates, and world picking. **Advanced visual** mode
will reveal nested predicates, event context, variables, branches, sequences,
and timing while preserving those cards. **Text** mode now edits the exact
bounded CaxeMap/CaxeFlow source. It provides line editing, syntax colors,
canonical formatting, and source-positioned diagnostics. It is useful for
experienced creators and automation agents. Shared-registry completion and
jump-to-world references remain planned.

Moving between views must parse and validate the same model. Text mode cannot
call a mechanic unavailable to cards, and the visual views cannot flatten or
hide advanced logic behind an opaque custom-script block. Invalid text remains
an editor draft and cannot replace the last playable scenario. “Text script”
therefore means the data-only CaxeFlow language, not arbitrary Haxe, C, Lua,
shell commands, file access, or native callbacks.

Starting a cutscene is one possible action, not a privileged trigger. Its
focused editor will arrange named camera anchors, actor staging markers,
ordered beats, limited parallel movement/camera/audio lanes, localized cards,
fades, choices, and persistent CaxeFlow changes. Normal and skip previews must
reach the same required persistent state and restore camera and controls. The
remaining visual depth is tracked under `haxe_c-xge.19.6` and cinematic
authoring under `haxe_c-xge.20.3`. The current native trigger-card slice does not
yet provide the complete card library, advanced visual tree, registry-backed
text completion, jump-to-world references, or cinematic timeline.

## Executable evidence

Run the focused proof from the repository root:

```sh
npm run test:caxecraft-editor
```

The probe builds a small complete scenario through the public command API. It
checks revision advancement, stale-request rejection, complete batch rollback,
one-entry transaction undo/redo, copied observations, exact undo and redo for
every command family, canonical serialize/reload, invalid-draft recovery,
deterministic history eviction, byte and gesture limits, two independent
test-play sessions, the optional top-down projection, complete-volume
projection, camera bounds, solid and empty-space ray picking, and all four tool
translations. It also checks complete Text round trips, invalid and stale
recovery, advanced CaxeFlow forms, undo/redo, and Test Play. It runs under C and
a second installed locale (Spanish when available). It scans reusable editor
sources for C, Raylib, target-condition, raw-code, and untyped-boundary leakage.

The native graphical proof uses the real renderer in Raylib's deterministic
in-memory configuration:

```sh
python3 examples/caxecraft/play.py \
  --pilot editor-shell \
  --raylib-configuration memory-software \
  --allow-network
```

That pilot compiles the application through haxe.c. It enters the editor from
the title screen and opens the active level bytes. It changes one literal
title, selects layer 2, moves the production camera, paints the first available
air cell, and saves the resulting package. It applies one valid Text edit, then
keeps one invalid closing record isolated with its visible diagnostic. It then
selects the painted cell through `CaxecraftEditorScreen` and `EditorSession`.

The framebuffer check requires the toolbar, sidebar, scene list, textured
terrain, sky, and selection outline. It requires enough terrain color variation
to reject the former flat overview. The pilot repeats the journey and requires
identical reports and screenshots. The headless software renderer has a
90-second process limit for this complete editor and game journey.

The pilot proves active-level presentation, a revision-neutral layer change,
one terrain change, the object list, scene gizmos, the rule count, the title
path, and native package Save. It checks the changed map and refreshed receipts
and rejects leftover staging or backup files. The runner restores the source
package before each repeat, so both runs must publish the same bytes. It also
starts and stops two ordinary-engine Test Play runs in one process. Each run
completes a fixed game tick and uses a new disposable generation.

The final report also proves that the normal generation and publication count
did not change. The renderer-independent proof covers canonical CaxeFlow card
edits and typed world picks; native pointer-control coverage, cutscenes, and
crash-durable filesystem publication remain separate evidence.

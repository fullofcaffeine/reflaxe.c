# Caxecraft editor semantics

Status: the renderer-independent command, history, validation, and test-play
layer is implemented under `haxe_c-xge.19.5`. The native Raylib/Raygui screen
now opens the same CAXEMAP bytes as the active game generation. It shows the
map's visible height surface, all authored object placements, and the CaxeFlow
rule count. It also lists each object's stable ID and draws a colored 3D gizmo
for each closed placement role.

The editor core provides revision-checked changes, bounded command groups, and
copy-owned observations. Its World Name field commits literal titles through
the same command and history boundary. Tab and Shift-Tab move through one
device-neutral focus order. Enter or Space activates the focused control. A
high-contrast ring shows the next target.

The native screen edits one voxel layer. Its Test Play button runs the
disposable CaxeFlow model, not the complete game engine. Native source save,
object transforms, visual CaxeFlow editing, cutscene editing, and layer tools
remain separate work. A local JSON Lines process can open, inspect, edit,
validate, and save one verified package level.

## What this layer owns

The editor core answers “what does this edit mean?” without knowing how a
button, mouse drag, controller focus ring, or voxel highlight is drawn. Both the
headless test and the current first Raylib viewport use the same public
operations:

```text
Scenario -> EditorSession -> closed EditorCommand -> updated draft
                         \-> validate -> last playable snapshot
                         \-> test play -> disposable CaxeFlow simulation
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
    +-- perspective drawing
    +-- camera ray -> visible voxel or empty floor cell
                         |
                         v
                 EditorCommand -> EditorSession.mutate
```

A **projection** means a read-only shape prepared for presentation.
`EditorWorldViewport.projectWorld` decodes the complete finite draft into one
volume ordered as `(z * height + y) * width + x`.
`CaxecraftEditorScreen` caches that projection. The cache also contains a
compact height surface for the native overview. Equal height and material cells
merge into rectangular patches. This keeps a full authored map responsive in
the headless renderer. Exact voxel cells remain available for picking and
edits. The overview does not show hidden caves because layer inspection is not
implemented.

A normal displayed frame reads the cache. It does not serialize the CAXEMAP
draft or allocate a replacement volume. New World, an accepted edit, undo, or
redo rebuilds the cache from the session's new draft.

Raylib turns one screen pixel into a **ray**: a starting point and direction in
the 3D world. `EditorWorldViewport.pickWorld` enters the finite map once. It
then visits only the voxel cells crossed by that ray. Map size no longer
controls hover-picking cost.

The picker returns the nearest solid cell. If it finds no solid cell, it checks
the floor of the current edit layer. This path lets a creator paint an empty
cell. The selected tool converts the typed `VoxelPoint` into the same
`EditorCommand` that history uses. An invalid pick changes nothing.

The current controls are:

- use Tab and Shift-Tab to move the visible focus ring through editor controls;
- press Enter or Space to activate the focused control;
- use a connected controller's D-pad or left stick to move the same ring;
- press the south face button to activate or the east face button to return;
- hold the right mouse button and move the pointer to look;
- use W/S to move forward/back, A/D to strafe, and Q/E to move vertically;
- use the wheel to move along the view direction;
- press F to restore the deterministic whole-world view; and
- left-click to apply the selected tool.

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

The older renderer-independent `EditorViewport` module still owns exact
top-down layer projection and pixel-edge mapping. It remains tested as the
foundation for a later optional planning view or minimap, but the shipped
native canvas no longer uses it as the primary editor.

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
Selections have their own 65,536-cell absolute bound. The same setting limits
the number of points submitted by one batch paint or erase gesture, including
duplicate points; it is the editor's shared “one gesture” work budget. Smaller
bounds may be selected when opening a session, which makes device- or
mode-specific limits testable without weakening the format limits.

The separate transaction-command bound limits how many typed commands may be
grouped into one all-or-nothing edit. It does not raise the voxel gesture,
selection, history-entry, or history-byte bounds: every command inside the
transaction must still satisfy those existing limits.

This full-snapshot strategy favors simple, trustworthy recovery for the first
bounded editor. Later editor slices must measure real map sizes and gesture
latency. If snapshots become the bottleneck, they may introduce typed
command-specific inverse data while retaining bounded paint gestures, exact
undo bytes, hard memory limits, and the same public commands. A later
optimization must not trade correctness for an unmeasured speedup.

## Reversible test play

`enterTestPlay()` validates the current draft, deep-copies the resulting
scenario, and constructs a new `CaxeFlowExecutor`. Rules can then change
objectives, inventory, variables, objects, and scheduled work inside that
disposable simulation. `leaveTestPlay()` drops it. Entering test play again
starts from the authored values, not from the prior simulation.

There is intentionally no “keep whatever happened while playing” operation in
this version. Importing selected play changes later would need its own closed
command and clear ownership rules. Silent import would make a test run mutate
the map and defeat reversible experimentation.

## Planned visual event and cinematic authoring

The native editor will not have separate trigger systems for doors, encounters,
music, quests, and cutscenes. They all use one CaxeFlow relationship:

```text
event source -> conditions -> ordered actions
```

A spatial volume is one event source. The world view will let a creator place,
name, resize, filter, enable, duplicate, and select its visible gizmo. The card
view will show the same data as icon-and-text WHEN/IF/DO sentences and will pick
objects or actions from the world and shared registries instead of asking a
child to type IDs. An event-flow overlay and bounded test trace will explain
which source fired, why each condition passed or failed, which actions ran, and
which signals or state changes were deferred.

The same typed draft has three authoring depths. **Guided** mode uses large
icon-and-sentence cards, templates, and world picking. **Advanced visual** mode
reveals nested predicates, event context, variables, branches, sequences, and
timing while preserving those cards. **Text** mode edits the exact bounded
CaxeMap/CaxeFlow source with syntax coloring, shared-registry completion,
formatting, source-positioned diagnostics, and jump-to-world references. It is
especially useful for experienced creators and automation agents.

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
reach the same required persistent state and restore camera and controls. This
work is planned under `haxe_c-xge.19.10`, `haxe_c-xge.19.6`, and
`haxe_c-xge.20.3`; the implemented renderer-independent editor described above
does not yet provide these native visual tools.

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
translations. It runs under C and a second installed locale (Spanish when
available) and scans the reusable editor sources for C, Raylib,
target-condition, raw-code, and untyped-boundary leakage.

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
title, moves the production camera, and paints the first available air cell.
It then selects that cell through `CaxecraftEditorScreen` and `EditorSession`.

The framebuffer check requires the toolbar, sidebar, scene list, authored
terrain colors, sky, and selection outline. The pilot repeats the journey and
requires identical semantic reports and screenshots. The headless software
renderer has a 40-second process limit for each detailed editor frame.

The pilot proves active-level presentation, one real terrain change, the object
list, scene gizmos, the rule count, and the title path. It does not prove native
source save, object transforms, localized-title editing, layer tools, visual
CaxeFlow editing, cutscenes, or complete game-engine Test Play.

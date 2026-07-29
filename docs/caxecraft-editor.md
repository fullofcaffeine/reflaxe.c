# Caxecraft editor semantics

Status: the renderer-independent command, history, validation, and test-play
layer is implemented under `haxe_c-xge.19.5`. The first native Raylib/Raygui
slice now presents the complete finite draft in a clipped perspective viewport,
with a fly camera and typed voxel picking. The editor core also provides
revision-checked mutations, bounded atomic command batches, and copy-owned
observations for visual and future automation clients. Its World Name field now
commits literal titles through that same typed command/history boundary. It
also has one device-neutral focus order: Tab and Shift-Tab move through every
current control, Enter or Space activates the focused control, and a
high-contrast ring shows where the next activation will go. It still edits
layer zero only. Native map-file persistence, localized message-title editing,
JSONL/MCP adapters, multi-layer controls, game-controller input, object gizmos,
and the fuller child-friendly authoring experience remain separate work.

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

The visual editor and a future agent must not have different rules for changing
a map. Both use `EditorSession.mutate`, which combines an operation with the
draft revision its caller inspected:

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

This is the implemented in-process semantic boundary, not a claim that remote
automation already ships. A planned local JSON Lines adapter will validate
bounded external requests into these closed Haxe types. It must use the general
`haxe.Json` and hosted input/file capabilities owned by `haxe_c-0bx` and
`haxe_c-fwg`; a Caxecraft-only JSON parser or direct libc input shim would be a
compiler-shaped workaround, not the intended architecture. A later optional
MCP adapter may translate discoverable tools into the same operations. Neither
adapter will receive a private mutation path, arbitrary code execution, or
unrestricted filesystem and network access. Issue `haxe_c-xge.19.6.3` owns
those later adapters and the broader authoring surface.

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
`CaxecraftEditorScreen` caches that projection. A normal displayed frame reads
the cache; it does not serialize the CAXEMAP draft or allocate a replacement
volume. New World, an accepted edit, undo, or redo rebuilds the cache from the
session's new draft.

Raylib turns one screen pixel into a **ray**: a starting point and direction in
the 3D world. `EditorWorldViewport.pickWorld` tests that ray against visible
solid voxel boxes and chooses the nearest hit. If no solid is hit, it intersects
the floor of the current editable layer so a creator can paint an empty cell.
The result is a typed `VoxelPoint`; the selected tool then creates the same
`EditorCommand` used by tests and history. Invalid picks and rejected commands
leave both the draft and its presentation cache unchanged.

The current controls are:

- use Tab and Shift-Tab to move the visible focus ring through editor controls;
- press Enter or Space to activate the focused control;
- hold the right mouse button and move the pointer to look;
- use W/S to move forward/back, A/D to strafe, and Q/E to move vertically;
- use the wheel to move along the view direction;
- press F to restore the deterministic whole-world view; and
- left-click to apply the selected tool.

The focus order is target-neutral: it names editor actions, not Raylib key
codes or screen coordinates. The native screen translates keyboard input into
forward movement, backward movement, or activation. The list control then moves
its own selected item when it receives activation. This separation lets the
planned game-controller adapter reuse the same order without creating a second
set of editor navigation rules. Controller polling itself is not implemented
yet; issue `haxe_c-xge.19.6.1.6` owns the required pinned Raylib binding and
native proof.

The localized viewport heading shows these controls in English and
Mexican Spanish. Camera state and the current hover are presentation values:
they are not saved in CAXEMAP, do not participate in undo, and cannot mutate
terrain. Raylib's scissor region clips all 3D drawing to the canvas, so even a
nearby voxel cannot cover the toolbar, sidebar, or status bar.

The older renderer-independent `EditorViewport` module still owns exact
top-down layer projection and pixel-edge mapping. It remains tested as the
foundation for a later optional planning view or minimap, but the shipped
native canvas no longer uses it as the primary editor.

The built-in blank world is 12 by 1 by 12 cells. Caxecraft supplies its Air and
Grass content IDs at the application composition edge; the reusable editor
factory receives those typed values and does not know game-specific content
names.

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

That pilot compiles the application through haxe.c, confirms one literal title,
moves the production camera, and submits typed Paint and Select gestures
through `CaxecraftEditorScreen` and `EditorSession`. The rejected-edit counter
must stay zero, while the terrain counter observes exactly one paint. Its
framebuffer check requires the real toolbar and sidebar plus broad 3D evidence:
sky, a perspective ground plane, one solid voxel, and its distinct selection
outline. It repeats semantic execution, captures the review frame, and exits
within the bounded timeout. It proves this first perspective viewport and title
slice, not native map-file save, localized-title editing, multi-layer controls,
controller navigation, or the complete planned visual event/cutscene editor.

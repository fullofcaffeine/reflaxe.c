# Caxecraft editor semantics

Status: the renderer-independent command, history, validation, and test-play
layer is implemented under `haxe_c-xge.19.5`. The first native Raylib/Raygui
slice now displays and edits one bounded top-down voxel layer. Native map-file
persistence, multi-layer navigation, and the fuller child-friendly authoring
experience remain separate work.

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

## First visual viewport

The first visual slice deliberately has one small job: show one horizontal
world layer and let Select, Paint, Erase, and Fill use the existing command
language. It does not introduce a second editable grid.

```text
CAXEMAP draft
    |
    | accepted edit / undo / redo / New World
    v
cached read-only layer projection
    |
    +-- draw colored cells and selection
    |
pointer pixel -> typed VoxelPoint -> EditorCommand -> EditorSession.apply
```

`EditorViewport.project` decodes the complete draft and copies only the visible
layer into a compact array. `CaxecraftEditorScreen` caches that projection. A
steady frame only reads the cached cells; it does not serialize the CAXEMAP
draft or allocate a replacement projection. The cache is rebuilt after New
World or after an accepted edit, undo, or redo.

The same integer `EditorViewportLayout` controls drawing and hit testing. Its
right and bottom edges are excluded, so a pixel on a cell boundary has one
deterministic meaning in Eval tests and generated C. An invalid tool gesture
does not touch the draft or cache and changes the visible status to the invalid
notice.

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
checks exact undo and redo for every command family, canonical
serialize/reload, invalid-draft recovery, deterministic history eviction, byte
and gesture limits, two independent test-play sessions, viewport projection,
pixel-edge mapping, and all four tool translations. It runs under C and a
second installed locale (Spanish when available) and scans the reusable editor
sources for C, Raylib, target-condition, raw-code, and untyped-boundary leakage.

The native graphical proof uses the real renderer in Raylib's deterministic
in-memory configuration:

```sh
python3 examples/caxecraft/play.py \
  --pilot editor-shell \
  --raylib-configuration memory-software \
  --allow-network
```

That pilot compiles the application through haxe.c, submits one typed Paint
gesture through `CaxecraftEditorScreen` and `EditorSession`, observes exactly
one accepted edit, validates the visible editor pixels and telemetry, captures
two byte-identical screenshots, and exits within the bounded timeout. It proves
this first viewport slice, not native map-file save, multi-layer editing, or the
complete planned visual event/cutscene editor.

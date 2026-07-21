# Caxecraft editor semantics

Status: the renderer-independent command, history, validation, and test-play
layer is implemented under `haxe_c-xge.19.5`. Native map-file persistence and
the child-friendly Raylib interface remain separate work.

## What this layer owns

The editor core answers “what does this edit mean?” without knowing how a
button, mouse drag, controller focus ring, or voxel highlight is drawn. Both a
headless test and the future Raylib interface use the same public operations:

```text
Scenario -> EditorSession -> closed EditorCommand -> updated draft
                         \-> validate -> last playable snapshot
                         \-> test play -> disposable CaxeFlow simulation
```

An `EditorCommand` is a closed Haxe enum. Closed means the possible edit kinds
are listed in the type rather than encoded as strings. Haxe can therefore make
callers handle new command kinds intentionally. The first command set covers:

- bounded world resize and palette entries;
- single-voxel and bounded multi-voxel paint/erase plus selection fill;
- bounded selection and clear-selection;
- typed prefab stamps and general object placements;
- dialogue, objective, and CaxeFlow rule replacement/removal; and
- explicit recovery to the last validated playable scenario.

The command model deliberately contains no Raylib event, C pointer, file path,
raw target text, or `Dynamic` payload. A future UI translates input gestures
into these commands. `PaintVoxels` and `EraseVoxels` commit one bounded drag as
one edit, rather than creating history on every rendered frame.

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
bounded editor. The Raylib UI task must measure real map sizes and gesture
latency. If snapshots become the bottleneck, it may introduce typed
command-specific inverse data while retaining bounded paint gestures, exact
undo bytes, hard memory limits, and the same public commands. It must not trade
correctness for an unmeasured optimization.

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
and gesture limits, and two independent test-play sessions. It runs under C
and a second installed locale (Spanish when available) and scans the editor
sources for C, Raylib, target-condition, raw-code, and untyped-boundary leakage.

This is Eval evidence for shared editor semantics. It does not claim that the
Raylib interface exists, that maps are written to disk, or that the complete
editor already runs as generated C. Those proofs retain their separate owners.

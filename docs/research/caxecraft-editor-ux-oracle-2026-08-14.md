# Caxecraft editor UX decision

This record reconciles Oracle request
`orq_20260814T184909Z_793b39cf` with the current checkout. Oracle supplied
advice. The haxe.c repository remains the implementation authority.

## Local baseline

The editor already has a strong data boundary. `EditorSession` owns the draft,
typed changes, validation, exact undo and redo, and disposable Test Play.

The current screen does not expose that model well. It uses a desktop-style
toolbar, a small text tool list, a global Advanced toggle, and a raw object-ID
list. The world is visible, but creation is not the main visual action.

The review found two semantic errors:

- Selection used `EditorCommand`. A selection change could add an undo entry
  without changing authored content.
- New World, Back, Escape, and window close can leave or replace an edited
  in-memory draft without a dirty-state decision.

## Oracle claim matrix

| Oracle claim | Local decision | Local evidence and result |
| --- | --- | --- |
| Keep the world central and embed recipes as Quick Builds. | Retained | This uses the existing 3D projection and typed batch command boundary. Recipes will expand into ordinary content changes. |
| Keep Build and Plan as editor views. Keep Play as a separate primary action. | Retained | Build and Plan can share one draft and selection. Test Play already enters a separate disposable runtime. |
| Move selection out of document history. | Retained and implemented first | `EditorSession.select` now owns semantic workspace selection. Selection changes no document bytes, revision, or history. |
| Replace selection-dependent fill with explicit bounds. | Retained and implemented first | `FillBounds` carries its target. A content command no longer depends on a temporary cursor owned by a view. |
| Use reducer-backed, side-effect-free previews. | Retained and implemented first | `EditorSession.preview` uses the commit reducer and limits. It does not publish the staged draft. |
| Remove the global Advanced toggle. | Retained | The current toggle changes no visible controls. Each inspector section will use a local More details disclosure. |
| Keep an optional World List. | Retained | Hidden triggers and overlapping objects need a searchable path. The list will select a stable ID instead of an array index. |
| Add package-backed Save before the new shell. | Deferred | The first slice must not show a fake Save action. Persistence remains a separate stage with stale-file detection. |
| Build recipes, object transforms, CaxeFlow editing, and cutscenes in the first slice. | Rejected | These features do not prove the first child-facing terrain loop. Their accepted stages remain unchanged. |
| Add a general UI, layout, inspector, or recipe framework. | Rejected | A few closed Haxe UI-state types are sufficient for the first slice. |
| Use pixel-exact UI assertions. | Rejected | Focused checks will protect state and command semantics. Real rendered frames and short usability tasks will assess the interface. |

## Integrated conclusion

The editor will use one world-first shell over one `EditorSession`.

- The top document bar contains Back, the world name, undo, redo, Build, Plan,
  Problems, and a prominent Play action.
- Build and Plan share the same draft, semantic selection, active creation
  card, diagnostics, and inspector.
- The bottom creation shelf starts with Select, one real Ground card, and
  Erase. Later categories add Quick Builds, Things, Characters, Goals, and
  Story.
- A selected voxel opens a small contextual inspector. Exact fields appear
  under More details on the same selection.
- The World List is a closed drawer by default. It exists for hidden or
  overlapping content.
- A placement shows a snapped ghost and an exact valid or invalid reason.
- Undo and redo cover authored content only. They never cover camera movement,
  selection, view changes, palette navigation, or disclosure state.

The first visual implementation slice remains one-layer terrain authoring. It
must prove this complete loop:

```text
choose Ground -> preview -> paint -> select -> inspect -> undo
              -> change Build or Plan -> Play -> return
```

The first Ground card means paint or replace on the active layer. It does not
promise general face-adjacent block placement. The current ray hit does not yet
contain a face or an adjacent cell.

## Verification and open decisions

The editor-core change now proves these checks:

- Selection changes no canonical bytes, document revision, or undo depth.
- Explicit fill bounds do not depend on workspace selection.
- Preview and commit agree at the same revision and target.
- An invalid preview and click add no history entry.
- Undo and redo do not restore old workspace selection.
- Removing a selected object clears its stable node selection.

The visual slice still needs these checks:

- Build and Plan changes add no history entry.
- One paint gesture adds one undo entry.
- Test Play returns to the prior editor workspace.

The rendered pilot must show Build, Plan, a selected creation card, valid and
invalid previews, a selected voxel inspector, visible focus, and es-MX text.
The pilot will report semantic state. It will not compare exact pixels.

The initial usability review targets children who can read short labels. Icons
and visible state must also help younger creators. The maintainer still needs
to select the final target age, minimum resolution, UI scale range, default
camera, and full controller scope before publication.

Native Save, crash recovery, external-file conflicts, general 3D placement,
object transforms, recipes, visual logic, dialogue, localization authoring,
and cutscenes remain later stages of `haxe_c-xge.19.6.3`.

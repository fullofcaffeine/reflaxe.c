package caxecraft.editor;

import caxecraft.editor.EditorSession;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.ScenarioDiagnostic;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioTag;
import caxecraft.scenario.ScenarioWorld.ScenarioFluid;
import caxecraft.scenario.ScenarioMessages.ScenarioLocaleCatalog;
import caxecraft.scenario.ScenarioMessages.ScenarioMessage;
import haxe.io.Bytes;

/** Resource limits selected when an editor session opens. */
typedef EditorSettings = {
	final historyEntries:Int;
	final historyBytes:Int;
	final selectionCells:Int;

	/** Largest number of commands that may commit as one atomic edit. */
	final transactionCommands:Int;
}

/** The editor's closed, renderer-independent command vocabulary. */
enum EditorCommand {
	ResizeWorld(size:VoxelSize);
	SetPaletteEntry(code:Int, blockType:ContentId);
	PaintVoxel(point:VoxelPoint, paletteCode:Int);
	EraseVoxel(point:VoxelPoint);
	PaintVoxels(points:Array<VoxelPoint>, paletteCode:Int);
	EraseVoxels(points:Array<VoxelPoint>);
	Select(bounds:VoxelBounds);
	ClearSelection;
	FillSelection(paletteCode:Int);
	PutFluid(fluid:ScenarioFluid);
	RemoveFluid(id:ScenarioId);
	StampPrefab(id:ScenarioId, prefabType:ContentId, tags:Array<ScenarioTag>, transform:ScenarioTransform);
	PutObject(object:ScenarioObject);
	RemoveObject(id:ScenarioId);
	PutDialogue(dialogue:ScenarioDialogue);
	RemoveDialogue(id:ScenarioId);
	PutObjective(objective:ScenarioObjective);
	RemoveObjective(id:ScenarioId);
	PutRule(rule:FlowRule);
	RemoveRule(id:ScenarioId);
	SetDefaultLocale(locale:LocaleId);
	PutLocale(locale:ScenarioLocaleCatalog);
	RemoveLocale(locale:LocaleId);
	PutMessage(locale:LocaleId, message:ScenarioMessage);
	RemoveMessage(locale:LocaleId, message:MessageId);
	RestoreLastPlayable;
}

/** Stable command groups used by history, UI labels, and acceptance traces. */
enum EditorCommandFamily {
	WorldShape;
	Voxel;
	Selection;
	Fluid;
	Prefab;
	Placement;
	Dialogue;
	Objective;
	Rule;
	Localization;
	Recovery;

	/** One history entry containing several individually typed commands. */
	Transaction;
}

enum EditorSetting {
	HistoryEntries;
	HistoryBytes;
	SelectionCells;
	TransactionCommands;
}

/** Exact reasons an edit can be refused before it changes the draft. */
enum EditorError {
	InvalidSetting(setting:EditorSetting, minimum:Int, maximum:Int);
	UnsupportedFormatVersion(actual:Int, supported:Int);
	SnapshotRejected(diagnostics:Array<ScenarioDiagnostic>);
	NestedChoiceIsNotRepresentable;
	DraftWorldIsNotEditable;
	NotEditing;
	InvalidWorldSize(size:VoxelSize);
	PointOutsideWorld(point:VoxelPoint);
	BoundsOutsideWorld(bounds:VoxelBounds);
	SelectionTooLarge(cells:Int, maximum:Int);
	VoxelEditTooLarge(cells:Int, maximum:Int);
	NoSelection;
	InvalidPaletteCode(code:Int);
	UnknownPaletteCode(code:Int);
	MissingFluid(id:ScenarioId);
	DuplicateObject(id:ScenarioId);
	MissingObject(id:ScenarioId);
	MissingDialogue(id:ScenarioId);
	MissingObjective(id:ScenarioId);
	MissingRule(id:ScenarioId);
	MissingLocale(id:LocaleId);
	MissingMessage(locale:LocaleId, message:MessageId);
	CannotRemoveDefaultLocale(id:LocaleId);
	HistoryEntryTooLarge(bytes:Int, maximum:Int);

	/**
		The caller's observed revision no longer names the current draft.

		`expected` is the revision the editor currently requires; `actual` is
		the older or otherwise different revision supplied by the caller.
	**/
	RevisionConflict(expected:Int, actual:Int);

	/** The signed 32-bit revision counter cannot advance safely again. */
	RevisionExhausted;

	/** An empty transaction cannot describe an edit or history entry. */
	EmptyTransaction;

	/** One transaction exceeded the session's fixed command-work budget. */
	TransactionTooLarge(commands:Int, maximum:Int);

	NothingToUndo;
	NothingToRedo;
	NoPlayableScenario;
}

/**
	Every state-changing operation admitted by the shared editor boundary.

	`ApplyBatch` means all commands commit as one history entry or none commit.
	Undo and redo use the same revision check as authored edits, which prevents
	an automation client from undoing a newer human change accidentally.
**/
enum EditorMutation {
	Apply(command:EditorCommand);
	ApplyBatch(commands:Array<EditorCommand>);
	Undo;
	Redo;
}

/**
	A mutation paired with the exact draft revision its caller observed.

	The editor compares `baseRevision` before doing any work. This is optimistic
	concurrency: callers do not lock the editor while thinking, but a stale
	caller must reread state rather than overwrite a newer edit.
**/
typedef EditorMutationRequest = {
	final baseRevision:Int;
	final mutation:EditorMutation;
}

/**
	Result of one revision-checked mutation.

	An applied result advances the revision exactly once. `families` lists the
	individual command groups for a batch; undo and redo report the single
	history family they restored.
**/
enum EditorMutationResult {
	MutationApplied(families:Array<EditorCommandFamily>, revision:Int, undoDepth:Int, redoDepth:Int);
	MutationUnchanged(families:Array<EditorCommandFamily>, revision:Int);
	MutationRejected(error:EditorError, revision:Int);
}

/** Closed read-only questions supported by the shared editor boundary. */
enum EditorQuery {
	/** Read small session facts used by a toolbar, CLI, or agent. */
	InspectState;

	/** Read one deep copy of the current typed scenario draft. */
	InspectDraft;

	/** Read one copied deterministic CAXEMAP spelling of the current draft. */
	InspectCanonicalDraft;
}

/**
	Small immutable session facts tagged with the draft revision they describe.

	Selection is copied before it leaves the session. `editing` is false during
	disposable Test Play, when authored mutations are intentionally blocked.
**/
typedef EditorStateObservation = {
	final revision:Int;
	final selection:Null<VoxelBounds>;
	final undoDepth:Int;
	final redoDepth:Int;
	final historyEntries:Int;
	final historyBytes:Int;
	final editing:Bool;
}

/**
	Copy-owned answers returned by `EditorSession.query`.

	Each answer carries one revision so a caller can submit its next mutation
	against the exact state it inspected. Changing returned scenario arrays or
	bytes cannot change the live editor draft.
**/
enum EditorObservation {
	StateObserved(state:EditorStateObservation);
	DraftObserved(revision:Int, draft:Scenario);
	CanonicalDraftObserved(revision:Int, canonical:Bytes);
}

enum EditorOpenResult {
	EditorOpened(session:EditorSession);
	EditorOpenRejected(error:EditorError);
}

enum EditorEditResult {
	EditApplied(family:EditorCommandFamily, undoDepth:Int, redoDepth:Int);
	EditUnchanged(family:EditorCommandFamily);
	EditRejected(error:EditorError);
}

enum EditorHistoryResult {
	HistoryApplied(family:EditorCommandFamily, undoDepth:Int, redoDepth:Int);
	HistoryRejected(error:EditorError);
}

enum EditorValidationResult {
	ValidationPassed(canonical:Bytes);
	ValidationFailed(diagnostics:Array<ScenarioDiagnostic>);
	ValidationBlocked(error:EditorError);
}

enum EditorTestPlayResult {
	TestPlayStarted;
	TestPlayRejected(diagnostics:Array<ScenarioDiagnostic>);
	TestPlayBlocked(error:EditorError);
}

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
import caxecraft.scenario.ScenarioText;
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
	/**
		Replace the authored scenario title through the normal history boundary.

		The payload remains `ScenarioText`, so a text editor or later
		localization tool can preserve either a literal title or a message
		reference instead of flattening both forms into display text.
	**/
	SetTitle(title:ScenarioText);

	ResizeWorld(size:VoxelSize);
	SetPaletteEntry(code:Int, blockType:ContentId);
	PaintVoxel(point:VoxelPoint, paletteCode:Int);
	EraseVoxel(point:VoxelPoint);
	PaintVoxels(points:Array<VoxelPoint>, paletteCode:Int);
	EraseVoxels(points:Array<VoxelPoint>);
	FillBounds(bounds:VoxelBounds, paletteCode:Int);
	PutFluid(fluid:ScenarioFluid);
	RemoveFluid(id:ScenarioId);
	StampPrefab(id:ScenarioId, prefabType:ContentId, tags:Array<ScenarioTag>, transform:ScenarioTransform);
	PutObject(object:ScenarioObject);

	/** Move one existing placement by whole voxel cells without changing its identity or role. */
	MoveObjectBy(id:ScenarioId, delta:VoxelPoint);

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
	DocumentMetadata;
	WorldShape;
	Voxel;
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

/**
	Stable semantic identities reported after a committed editor mutation.

	The variants preserve each identifier's real type instead of flattening
	everything into a prefixed string. `ChangedDocument` deliberately means the
	whole draft, which is the honest result for restoring a complete snapshot.
**/
enum EditorChangeId {
	ChangedDocument;
	ChangedTitle;
	ChangedWorldShape;
	ChangedTerrain;
	ChangedPalette(code:Int);
	ChangedFluid(id:ScenarioId);
	ChangedObject(id:ScenarioId);
	ChangedDialogue(id:ScenarioId);
	ChangedObjective(id:ScenarioId);
	ChangedRule(id:ScenarioId);
	ChangedLocalization;
	ChangedLocale(id:LocaleId);
	ChangedMessage(locale:LocaleId, message:MessageId);
}

/** Fixed editor-tree groups whose children retain their own semantic IDs. */
enum EditorSection {
	World;
	Palette;
	Chunks;
	Fluids;
	Objects;
	Story;
	Dialogues;
	Journal;
	Objectives;
	Routes;
	Flow;
	Variables;
	Sequences;
	Rules;
	Localization;
	Extensions;
}

/**
	One typed identity in the editor's flat campaign-tree projection.

	A flat list plus explicit `parent` links is easy to render as a tree without
	giving callers nested mutable arrays. Every authored record present in the
	current CAXEMAP schema has a distinct variant.
**/
enum EditorNodeRef {
	ScenarioNode(id:ScenarioId);
	SectionNode(section:EditorSection);
	PaletteNode(code:Int);
	ChunkNode(id:ScenarioId);
	FluidNode(id:ScenarioId);
	ObjectNode(id:ScenarioId);
	DialogueNode(id:ScenarioId);
	JournalNode(id:ScenarioId);
	ObjectiveNode(id:ScenarioId);
	RouteNode(id:ScenarioId);
	VariableNode(id:ScenarioId);
	SequenceNode(id:ScenarioId);
	RuleNode(id:ScenarioId);
	LocaleNode(id:LocaleId);
	MessageNode(locale:LocaleId, message:MessageId);
	ExtensionNode(feature:ContentId, id:ScenarioId);
}

/**
	One semantic target shared by every editor view.

	Selection is workspace state. Looking at a voxel or authored node does not
	change CAXEMAP bytes, consume undo history, or advance the document revision.
	Build, Plan, the World List, and later diagnostic links all use this same
	typed value instead of keeping unrelated widget indices.
**/
enum EditorSelection {
	NoEditorSelection;
	VoxelSelection(bounds:VoxelBounds);
	NodeSelection(ref:EditorNodeRef);
}

/**
	One copy-owned row in the deterministic campaign-tree projection.

	`childCount` lets a compact UI draw disclosure controls without rescanning
	the complete draft. The source arrays retain canonical CAXEMAP order.
**/
typedef EditorTreeNode = {
	final ref:EditorNodeRef;
	final parent:Null<EditorNodeRef>;
	final childCount:Int;
}

enum EditorSetting {
	HistoryEntries;
	HistoryBytes;
	SelectionCells;
	TransactionCommands;
}

/** Exact reasons an edit can be refused before it changes the draft. */
enum EditorError {
	/** A pack-driven placement tool had no validated selected recipe. */
	MissingEditorObjectRecipe;

	InvalidSetting(setting:EditorSetting, minimum:Int, maximum:Int);
	UnsupportedFormatVersion(actual:Int, supported:Int);
	SnapshotRejected(diagnostics:Array<ScenarioDiagnostic>);
	NestedChoiceIsNotRepresentable;
	DraftWorldIsNotEditable;
	NotEditing;
	InvalidTitle;
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

	/** The requested whole-voxel move would place some or all of the object outside the finite world. */
	ObjectMoveOutsideWorld(id:ScenarioId, delta:VoxelPoint);

	MissingDialogue(id:ScenarioId);
	MissingObjective(id:ScenarioId);
	MissingRule(id:ScenarioId);
	MissingLocale(id:LocaleId);
	MissingMessage(locale:LocaleId, message:MessageId);
	MissingEditorNode(ref:EditorNodeRef);
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

/** A workspace selection paired with the document revision that exposed it. */
typedef EditorSelectionRequest = {
	final baseRevision:Int;
	final selection:EditorSelection;
}

/**
	Result of one workspace selection request.

	Applied and unchanged results keep the document revision as-is. Rejections
	leave both the previous selection and the authored draft untouched.
**/
enum EditorSelectionResult {
	SelectionApplied(selection:EditorSelection, revision:Int);
	SelectionUnchanged(selection:EditorSelection, revision:Int);
	SelectionRejected(error:EditorError, revision:Int);
}

/** A bounded edit preview paired with the exact document revision it observed. */
typedef EditorPreviewRequest = {
	final baseRevision:Int;
	final commands:Array<EditorCommand>;
}

/**
	Side-effect-free result of reducing one command list against a draft copy.

	A preview uses the same reducer as commit. It never changes canonical bytes,
	selection, history, revision, recovery state, or Test Play state.
**/
enum EditorPreviewResult {
	PreviewAccepted(families:Array<EditorCommandFamily>, changes:Array<EditorChangeId>, revision:Int);
	PreviewUnchanged(families:Array<EditorCommandFamily>, revision:Int);
	PreviewRejected(error:EditorError, revision:Int);
}

/**
	Result of one revision-checked mutation.

	An applied result advances the revision exactly once. `families` lists the
	individual command groups for a batch. `changes` is a deterministic,
	deduplicated list of semantic identities stored with history and therefore
	available again on undo and redo.
**/
enum EditorMutationResult {
	MutationApplied(families:Array<EditorCommandFamily>, changes:Array<EditorChangeId>, revision:Int, undoDepth:Int, redoDepth:Int);
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

	/** Read every authored record as a deterministic flat tree. */
	InspectTree;

	/** Read one compact tree/property row by its typed semantic identity. */
	InspectNode(ref:EditorNodeRef);

	/** Validate the draft without changing the last-known-playable snapshot. */
	InspectValidation;
}

/** Read-only validation state returned without mutating editor recovery state. */
enum EditorValidationObservation {
	DraftPlayable(canonical:Bytes);
	DraftInvalid(diagnostics:Array<ScenarioDiagnostic>);
	DraftUnreadable(error:EditorError);
}

/**
	Small immutable session facts tagged with the draft revision they describe.

	Selection is copied before it leaves the session. `editing` is false during
	disposable Test Play, when authored mutations are intentionally blocked.
**/
typedef EditorStateObservation = {
	final revision:Int;
	final selection:EditorSelection;
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
	TreeObserved(revision:Int, nodes:Array<EditorTreeNode>);
	NodeObserved(revision:Int, node:Null<EditorTreeNode>);
	ValidationObserved(revision:Int, validation:EditorValidationObservation);
}

enum EditorOpenResult {
	EditorOpened(session:EditorSession);
	EditorOpenRejected(error:EditorError);
}

enum EditorEditResult {
	EditApplied(family:EditorCommandFamily, changes:Array<EditorChangeId>, undoDepth:Int, redoDepth:Int);
	EditUnchanged(family:EditorCommandFamily);
	EditRejected(error:EditorError);
}

enum EditorHistoryResult {
	HistoryApplied(family:EditorCommandFamily, changes:Array<EditorChangeId>, undoDepth:Int, redoDepth:Int);
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

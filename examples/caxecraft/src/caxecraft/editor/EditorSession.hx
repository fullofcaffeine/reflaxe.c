package caxecraft.editor;

import caxecraft.editor.EditorCommandReducer.EditorReductionResult;
import caxecraft.editor.EditorCommandReducer.apply as reduceCommand;
import caxecraft.editor.EditorHistory.EditorHistoryEntry;
import caxecraft.editor.EditorObservationPlan.buildTree;
import caxecraft.editor.EditorObservationPlan.changesFor;
import caxecraft.editor.EditorObservationPlan.findNode;
import caxecraft.editor.EditorObservationPlan.mergeChanges;
import caxecraft.editor.EditorObservationPlan.sameNodeRef;
import caxecraft.editor.EditorPolicy.defaults as defaultEditorSettings;
import caxecraft.editor.EditorPolicy.validate as validateEditorSettings;
import caxecraft.editor.EditorScenarioSnapshot.EditorScenarioImage;
import caxecraft.editor.EditorScenarioSnapshot.EditorScenarioImageResult;
import caxecraft.editor.EditorScenarioSnapshot.capture as captureScenario;
import caxecraft.editor.EditorScenarioSnapshot.restore as restoreScenario;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorCommandFamily;
import caxecraft.editor.EditorTypes.EditorChangeId;
import caxecraft.editor.EditorTypes.EditorEditResult;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorHistoryResult;
import caxecraft.editor.EditorTypes.EditorMutationRequest;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorObservation;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorPreviewRequest;
import caxecraft.editor.EditorTypes.EditorPreviewResult;
import caxecraft.editor.EditorTypes.EditorQuery;
import caxecraft.editor.EditorTypes.EditorSelection;
import caxecraft.editor.EditorTypes.EditorSelectionRequest;
import caxecraft.editor.EditorTypes.EditorSelectionResult;
import caxecraft.editor.EditorTypes.EditorSettings;
import caxecraft.editor.EditorTypes.EditorTestPlayResult;
import caxecraft.editor.EditorTypes.EditorValidationObservation;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioValidator;
import caxecraft.editor.EditorWorldGrid.containsBounds as containsWorldBounds;
import caxecraft.editor.EditorWorldGrid.volume as voxelVolume;
import haxe.io.Bytes;

/** One mutually exclusive way in which the editor draft is locked for play. */
private enum EditorSessionPlayState {
	/** The renderer-independent editor core owns a disposable CaxeFlow run. */
	LocalCaxeFlowTest(test:EditorTestPlay);

	/** The native application owns a disposable ordinary-engine run. */
	ExternalRuntimeTest;
}

/** One isolated reducer pass before a preview or transaction publishes state. */
private enum EditorStageResult {
	StageReady(after:EditorScenarioImage, families:Array<EditorCommandFamily>, changes:Array<EditorChangeId>);
	StageRejected(error:EditorError);
}

/** Validation result for a workspace target against the current draft. */
private enum EditorSelectionValidation {
	WorkspaceSelectionReady(selection:EditorSelection);
	WorkspaceSelectionRejected(error:EditorError);
}

/**
	Renderer-independent editing, validation, history, and test-play state.

	The mutable draft is never exposed directly. Public snapshots are deep
	CAXEMAP round trips, so UI code cannot accidentally change history or test
	play by holding an old array reference.
**/
final class EditorSession {
	final registry:ScenarioContentRegistry;
	final settings:EditorSettings;
	final history:EditorHistory;
	var draft:Scenario;
	var selection:EditorSelection;
	var lastPlayable:Null<Scenario>;
	var playState:Null<EditorSessionPlayState>;
	var currentRevision:Int;

	function new(image:EditorScenarioImage, registry:ScenarioContentRegistry, settings:EditorSettings) {
		this.registry = registry;
		this.settings = settings;
		this.history = new EditorHistory(settings);
		this.draft = image.parsed.candidate;
		this.selection = NoEditorSelection;
		this.lastPlayable = validatedScenario(image);
		this.playState = null;
		this.currentRevision = 0;
	}

	/** Open even a semantically invalid draft so the editor can repair it. */
	public static function open(initial:Scenario, registry:ScenarioContentRegistry, ?requested:EditorSettings):EditorOpenResult {
		final settings = requested == null ? defaultEditorSettings() : requested;
		final invalidSetting = validateEditorSettings(settings);
		if (invalidSetting != null)
			return EditorOpenRejected(invalidSetting);
		return switch captureScenario(initial) {
			case ImageRejected(error): EditorOpenRejected(error);
			case ImageReady(image): EditorOpened(new EditorSession(image, registry, settings));
		}
	}

	/**
	 * Open one CAXEMAP byte owner as an isolated editable draft.
	 *
	 * Runtime content can give the editor the exact validated bytes it already
	 * owns. The snapshot parser copies those bytes before this session publishes
	 * mutable history, so later caller changes cannot alter the draft.
	 */
	public static function openBytes(source:Bytes, registry:ScenarioContentRegistry, ?requested:EditorSettings):EditorOpenResult {
		final settings = requested == null ? defaultEditorSettings() : requested;
		final invalidSetting = validateEditorSettings(settings);
		if (invalidSetting != null)
			return EditorOpenRejected(invalidSetting);
		return switch restoreScenario(source) {
			case ImageRejected(error): EditorOpenRejected(error);
			case ImageReady(image): EditorOpened(new EditorSession(image, registry, settings));
		}
	}

	/**
		Apply one command against the session's current in-process draft.

		This is the small convenience API for code that already owns the session
		and performs one synchronous edit. It still records history and advances
		the revision. A UI view, automation client, or future remote adapter must
		use `mutate` instead so a revision check can reject stale work.
	**/
	public function apply(command:EditorCommand):EditorEditResult {
		if (playState != null)
			return EditRejected(NotEditing);
		return switch captureScenario(draft) {
			case ImageRejected(error): EditRejected(error);
			case ImageReady(before): applyToImage(before, command);
		}
	}

	/**
		Apply one revision-checked operation through the shared editor boundary.

		A caller first reads a revision with `query`, then sends that value here.
		If another human or tool changed the draft meanwhile, the request is
		rejected before touching the draft or history. A batch stages every
		command on a private deep copy and publishes one history entry only after
		the complete list succeeds.
	**/
	public function mutate(request:EditorMutationRequest):EditorMutationResult {
		if (request.baseRevision != currentRevision)
			return MutationRejected(RevisionConflict(currentRevision, request.baseRevision), currentRevision);
		return switch request.mutation {
			case Apply(command): mutationFromEdit(apply(command));
			case ApplyBatch(commands): applyBatch(commands);
			case Undo: mutationFromHistory(undo());
			case Redo: mutationFromHistory(redo());
		}
	}

	/**
		Change the shared workspace target without changing authored content.

		The request carries the document revision that exposed the target. A stale
		view therefore cannot select an object or voxel that a newer edit removed.
		Success changes no canonical bytes, revision, or undo/redo entry.
	**/
	public function select(request:EditorSelectionRequest):EditorSelectionResult {
		if (request.baseRevision != currentRevision)
			return SelectionRejected(RevisionConflict(currentRevision, request.baseRevision), currentRevision);
		if (playState != null)
			return SelectionRejected(NotEditing, currentRevision);
		return switch validateSelection(request.selection) {
			case WorkspaceSelectionRejected(error): SelectionRejected(error, currentRevision);
			case WorkspaceSelectionReady(next):
				if (selectionsEqual(selection, next)) SelectionUnchanged(copySelection(selection), currentRevision); else {
					selection = copySelection(next);
					SelectionApplied(copySelection(selection), currentRevision);
				}
		};
	}

	/**
		Reduce a bounded command list against an isolated draft copy.

		The result uses the same reducer, canonical snapshot, revision, limits, and
		history budget as commit. It never publishes the staged draft or changes
		workspace state, which lets a renderer show an honest placement ghost.
	**/
	public function preview(request:EditorPreviewRequest):EditorPreviewResult {
		if (request.baseRevision != currentRevision)
			return PreviewRejected(RevisionConflict(currentRevision, request.baseRevision), currentRevision);
		if (playState != null)
			return PreviewRejected(NotEditing, currentRevision);
		if (request.commands.length == 0)
			return PreviewRejected(EmptyTransaction, currentRevision);
		if (request.commands.length > settings.transactionCommands)
			return PreviewRejected(TransactionTooLarge(request.commands.length, settings.transactionCommands), currentRevision);
		return switch captureScenario(draft) {
			case ImageRejected(error): PreviewRejected(error, currentRevision);
			case ImageReady(before):
				switch stageCommands(before, request.commands) {
					case StageRejected(error): PreviewRejected(error, currentRevision);
					case StageReady(after, families, changes): previewStaged(before, after, families, changes);
				}
		};
	}

	/**
		Return a copy-owned answer tagged with the revision it describes.

		The query is synchronous: the revision and copied value come from the
		same editor state. Future JSONL or MCP adapters may encode this answer,
		but transport types and permissions do not enter the editor core.
	**/
	public function query(request:EditorQuery):EditorObservation {
		return switch request {
			case InspectState:
				StateObserved({
					revision: currentRevision,
					selection: copySelection(selection),
					undoDepth: history.undoDepth(),
					redoDepth: history.redoDepth(),
					historyEntries: history.entryCount(),
					historyBytes: history.byteCount(),
					editing: playState == null
				});
			case InspectDraft:
				DraftObserved(currentRevision, draftSnapshot());
			case InspectCanonicalDraft:
				CanonicalDraftObserved(currentRevision, canonicalDraft());
			case InspectTree:
				TreeObserved(currentRevision, buildTree(draft));
			case InspectNode(ref):
				NodeObserved(currentRevision, findNode(draft, ref));
			case InspectValidation:
				ValidationObserved(currentRevision, inspectValidation());
		}
	}

	/**
		Validate for observation without changing the recovery snapshot.

		The mutating `validate` command records a new last-known-playable draft.
		A property panel or agent query must not gain that side effect merely by
		inspecting diagnostics, so this path returns fresh copied evidence only.
	**/
	function inspectValidation():EditorValidationObservation {
		return switch captureScenario(draft) {
			case ImageRejected(error): DraftUnreadable(error);
			case ImageReady(image):
				switch ScenarioValidator.validate(image.parsed, registry) {
					case ReadError(diagnostics): DraftInvalid(diagnostics.copy());
					case ReadOk(_): DraftPlayable(image.bytes.sub(0, image.bytes.length));
				}
		}
	}

	/** Current revision for an in-process caller preparing its next mutation. */
	public inline function revision():Int
		return currentRevision;

	/**
		Stage a bounded command list and commit it as one reversible edit.

		Each intermediate command still passes the ordinary reducer and canonical
		CAXEMAP snapshot boundary. If any step fails, only the private staged
		image was touched; the live draft, selection, history, and revision remain
		exactly as they were before the call.
	**/
	function applyBatch(commands:Array<EditorCommand>):EditorMutationResult {
		if (playState != null)
			return MutationRejected(NotEditing, currentRevision);
		if (commands.length == 0)
			return MutationRejected(EmptyTransaction, currentRevision);
		if (commands.length > settings.transactionCommands)
			return MutationRejected(TransactionTooLarge(commands.length, settings.transactionCommands), currentRevision);
		return switch captureScenario(draft) {
			case ImageRejected(error): MutationRejected(error, currentRevision);
			case ImageReady(before):
				switch stageCommands(before, commands) {
					case StageRejected(error): MutationRejected(error, currentRevision);
					case StageReady(after, families, changes):
						switch accept(before, after, Transaction, changes) {
							case EditApplied(_, committedChanges, undoDepth, redoDepth):
								MutationApplied(families.copy(), committedChanges, currentRevision, undoDepth, redoDepth);
							case EditUnchanged(_): MutationUnchanged(families.copy(), currentRevision);
							case EditRejected(error): MutationRejected(error, currentRevision);
						}
				}
		}
	}

	/** Reduce commands on copy-owned images without publishing any staged value. */
	function stageCommands(before:EditorScenarioImage, commands:Array<EditorCommand>):EditorStageResult {
		var staged = before;
		final families:Array<EditorCommandFamily> = [];
		final changes:Array<EditorChangeId> = [];
		for (command in commands) {
			switch command {
				case RestoreLastPlayable:
					if (lastPlayable == null)
						return StageRejected(NoPlayableScenario);
					switch captureScenario(lastPlayable) {
						case ImageRejected(error): return StageRejected(error);
						case ImageReady(image):
							staged = image;
							families.push(Recovery);
							mergeChanges(changes, [ChangedDocument]);
					}
				case _:
					switch reduceCommand(staged.parsed.candidate, command, settings) {
						case ReductionRejected(error): return StageRejected(error);
						case ReductionReady(reduction):
							switch captureScenario(reduction.scenario) {
								case ImageRejected(error): return StageRejected(error);
								case ImageReady(image):
									staged = image;
									families.push(reduction.family);
									mergeChanges(changes, changesFor(command));
							}
					}
			}
		}
		return StageReady(staged, families.copy(), changes.copy());
	}

	/** Match the commit gate without recording or publishing the candidate. */
	function previewStaged(before:EditorScenarioImage, after:EditorScenarioImage, families:Array<EditorCommandFamily>,
			changes:Array<EditorChangeId>):EditorPreviewResult {
		if (before.bytes.compare(after.bytes) == 0)
			return PreviewUnchanged(families.copy(), currentRevision);
		if (currentRevision == 2147483647)
			return PreviewRejected(RevisionExhausted, currentRevision);
		final byteCost = before.bytes.length + after.bytes.length;
		if (!history.canRecord(byteCost))
			return PreviewRejected(HistoryEntryTooLarge(byteCost, settings.historyBytes), currentRevision);
		return PreviewAccepted(families.copy(), changes.copy(), currentRevision);
	}

	function mutationFromEdit(result:EditorEditResult):EditorMutationResult {
		return switch result {
			case EditApplied(family, changes, undoDepth, redoDepth):
				MutationApplied([family], changes, currentRevision, undoDepth, redoDepth);
			case EditUnchanged(family):
				MutationUnchanged([family], currentRevision);
			case EditRejected(error):
				MutationRejected(error, currentRevision);
		}
	}

	function mutationFromHistory(result:EditorHistoryResult):EditorMutationResult {
		return switch result {
			case HistoryApplied(family, changes, undoDepth, redoDepth):
				MutationApplied([family], changes, currentRevision, undoDepth, redoDepth);
			case HistoryRejected(error):
				MutationRejected(error, currentRevision);
		}
	}

	function applyToImage(before:EditorScenarioImage, command:EditorCommand):EditorEditResult {
		switch command {
			case RestoreLastPlayable:
				return restorePlayable(before);
			case _:
		}
		return switch reduceCommand(before.parsed.candidate, command, settings) {
			case ReductionRejected(error): EditRejected(error);
			case ReductionReady(reduction):
				switch captureScenario(reduction.scenario) {
					case ImageRejected(error): EditRejected(error);
					case ImageReady(after):
						accept(before, after, reduction.family, changesFor(command));
				}
		}
	}

	/**
		Restore the state before the newest history entry.

		A successful restore advances the revision because observers now see a
		different draft. Use `mutate({mutation: Undo, ...})` when the caller does
		not exclusively own this in-process session.
	**/
	public function undo():EditorHistoryResult {
		if (playState != null)
			return HistoryRejected(NotEditing);
		if (currentRevision == 2147483647)
			return HistoryRejected(RevisionExhausted);
		final entry = history.takeUndo();
		if (entry == null)
			return HistoryRejected(NothingToUndo);
		return switch restoreScenario(entry.before) {
			case ImageRejected(error):
				history.takeRedo();
				HistoryRejected(error);
			case ImageReady(image):
				draft = image.parsed.candidate;
				selection = selectionForScenario(selection, draft);
				advanceRevision();
				HistoryApplied(entry.family, entry.changes.copy(), history.undoDepth(), history.redoDepth());
		}
	}

	/**
		Reapply the newest entry removed by `undo`.

		Like every committed state change, a successful redo advances the
		revision. Revision-aware callers should request it through `mutate`.
	**/
	public function redo():EditorHistoryResult {
		if (playState != null)
			return HistoryRejected(NotEditing);
		if (currentRevision == 2147483647)
			return HistoryRejected(RevisionExhausted);
		final entry = history.takeRedo();
		if (entry == null)
			return HistoryRejected(NothingToRedo);
		return switch restoreScenario(entry.after) {
			case ImageRejected(error):
				history.takeUndo();
				HistoryRejected(error);
			case ImageReady(image):
				draft = image.parsed.candidate;
				selection = selectionForScenario(selection, draft);
				advanceRevision();
				HistoryApplied(entry.family, entry.changes.copy(), history.undoDepth(), history.redoDepth());
		}
	}

	/** Validate the draft and update the separate last-known-playable snapshot. */
	public function validate():EditorValidationResult {
		return switch captureScenario(draft) {
			case ImageRejected(error): ValidationBlocked(error);
			case ImageReady(image):
				switch ScenarioValidator.validate(image.parsed, registry) {
					case ReadError(diagnostics): ValidationFailed(diagnostics);
					case ReadOk(scenario):
						lastPlayable = cloneScenario(scenario);
						ValidationPassed(image.bytes.sub(0, image.bytes.length));
				}
		}
	}

	/** Start a disposable simulation only when the current draft validates. */
	public function enterTestPlay():EditorTestPlayResult {
		if (playState != null)
			return TestPlayBlocked(NotEditing);
		return switch validate() {
			case ValidationFailed(diagnostics): TestPlayRejected(diagnostics);
			case ValidationBlocked(error): TestPlayBlocked(error);
			case ValidationPassed(_):
				if (lastPlayable == null) TestPlayBlocked(NoPlayableScenario); else {
					final snapshot = cloneScenario(lastPlayable);
					if (snapshot == null)
						TestPlayBlocked(NoPlayableScenario);
					else {
						playState = LocalCaxeFlowTest(new EditorTestPlay(snapshot));
						TestPlayStarted;
					}
				}
		}
	}

	/** Discard every test-play mutation and return to the untouched draft. */
	public function leaveTestPlay():Bool {
		return switch playState {
			case LocalCaxeFlowTest(_):
				playState = null;
				true;
			case ExternalRuntimeTest | null:
				false;
		};
	}

	/** Borrow the local CaxeFlow run without exposing the external runtime lock. */
	public function testPlay():Null<EditorTestPlay> {
		return switch playState {
			case LocalCaxeFlowTest(test): test;
			case ExternalRuntimeTest | null: null;
		};
	}

	/**
	 * Validate copy-owned bytes for ordinary-engine Test Play without side effects.
	 *
	 * A later runtime rejection must leave history and `lastPlayable` unchanged.
	 * The application acquires the lock only after the complete runtime accepts
	 * these bytes.
	 */
	public function prepareExternalTestPlay():EditorValidationResult {
		if (playState != null)
			return ValidationBlocked(NotEditing);
		return switch inspectValidation() {
			case DraftPlayable(canonical): ValidationPassed(canonical);
			case DraftInvalid(diagnostics): ValidationFailed(diagnostics);
			case DraftUnreadable(error): ValidationBlocked(error);
		};
	}

	/** Lock editing after the application has created the ordinary-engine run. */
	public function beginExternalTestPlay():Bool {
		if (playState != null)
			return false;
		playState = ExternalRuntimeTest;
		return true;
	}

	/** Release only the external runtime lock; local editor tests keep ownership. */
	public function finishExternalTestPlay():Bool {
		return switch playState {
			case ExternalRuntimeTest:
				playState = null;
				true;
			case LocalCaxeFlowTest(_) | null:
				false;
		};
	}

	public function draftSnapshot():Scenario {
		final snapshot = cloneScenario(draft);
		if (snapshot == null)
			throw "editor draft became unreadable";
		return snapshot;
	}

	public function lastPlayableSnapshot():Null<Scenario>
		return cloneScenario(lastPlayable);

	/**
		Return the deterministic in-memory draft spelling for comparison or UI
		state. A caller must validate successfully before treating these bytes as
		a playable or persistable map.
	**/
	public function canonicalDraft():Bytes {
		return switch captureScenario(draft) {
			case ImageReady(image): image.bytes.sub(0, image.bytes.length);
			case ImageRejected(_): throw "editor draft became unreadable";
		}
	}

	/** Return a copy of the workspace target shared by every editor view. */
	public function selectionSnapshot():EditorSelection
		return copySelection(selection);

	/** Return selected voxel bounds for spatial views, or null for another target. */
	public function selectedBounds():Null<VoxelBounds> {
		return switch selection {
			case VoxelSelection(bounds): copyBounds(bounds);
			case NoEditorSelection | NodeSelection(_): null;
		};
	}

	public inline function undoDepth():Int
		return history.undoDepth();

	public inline function redoDepth():Int
		return history.redoDepth();

	public inline function historyEntries():Int
		return history.entryCount();

	public inline function historyBytes():Int
		return history.byteCount();

	function restorePlayable(before:EditorScenarioImage):EditorEditResult {
		if (lastPlayable == null)
			return EditRejected(NoPlayableScenario);
		return switch captureScenario(lastPlayable) {
			case ImageRejected(error): EditRejected(error);
			case ImageReady(after): accept(before, after, Recovery, [ChangedDocument]);
		}
	}

	function accept(before:EditorScenarioImage, after:EditorScenarioImage, family:EditorCommandFamily, changes:Array<EditorChangeId>):EditorEditResult {
		if (before.bytes.compare(after.bytes) == 0)
			return EditUnchanged(family);
		if (currentRevision == 2147483647)
			return EditRejected(RevisionExhausted);
		final byteCost = before.bytes.length + after.bytes.length;
		if (!history.canRecord(byteCost))
			return EditRejected(HistoryEntryTooLarge(byteCost, settings.historyBytes));
		final entry:EditorHistoryEntry = {
			family: family,
			changes: changes.copy(),
			before: before.bytes.sub(0, before.bytes.length),
			after: after.bytes.sub(0, after.bytes.length),
			byteCost: byteCost
		};
		history.record(entry);
		draft = after.parsed.candidate;
		selection = selectionForScenario(selection, draft);
		advanceRevision();
		return EditApplied(family, changes.copy(), history.undoDepth(), history.redoDepth());
	}

	/**
		Advance the in-process edit counter after one committed state change.

		Haxe `Int` lowers to a signed 32-bit value in generated C. The mutation
		paths reject revision exhaustion before changing state, so this increment
		cannot wrap to a negative value that could match an old request.
	**/
	function advanceRevision():Void {
		currentRevision++;
	}

	function validatedScenario(image:EditorScenarioImage):Null<Scenario> {
		return switch ScenarioValidator.validate(image.parsed, registry) {
			case ReadError(_): null;
			case ReadOk(scenario): cloneScenario(scenario);
		}
	}

	static function cloneScenario(scenario:Null<Scenario>):Null<Scenario> {
		if (scenario == null)
			return null;
		return switch captureScenario(scenario) {
			case ImageReady(image): image.parsed.candidate;
			case ImageRejected(_): null;
		}
	}

	function validateSelection(value:EditorSelection):EditorSelectionValidation {
		return switch value {
			case NoEditorSelection: WorkspaceSelectionReady(NoEditorSelection);
			case VoxelSelection(bounds):
				if (!containsWorldBounds(draft.world.size, bounds)) WorkspaceSelectionRejected(BoundsOutsideWorld(bounds)); else {
					final cells = voxelVolume(bounds.size);
					if (cells > settings.selectionCells)
						WorkspaceSelectionRejected(SelectionTooLarge(cells, settings.selectionCells));
					else
						WorkspaceSelectionReady(VoxelSelection(copyBounds(bounds)));
				}
			case NodeSelection(ref):
				if (findNode(draft, ref) == null) WorkspaceSelectionRejected(MissingEditorNode(ref)); else WorkspaceSelectionReady(NodeSelection(ref));
		};
	}

	/** Keep a workspace target only while the changed draft still contains it. */
	function selectionForScenario(value:EditorSelection, scenario:Scenario):EditorSelection {
		return switch value {
			case NoEditorSelection: NoEditorSelection;
			case VoxelSelection(bounds): containsWorldBounds(scenario.world.size, bounds) ? VoxelSelection(copyBounds(bounds)) : NoEditorSelection;
			case NodeSelection(ref): findNode(scenario, ref) == null ? NoEditorSelection : NodeSelection(ref);
		};
	}

	static function copySelection(value:EditorSelection):EditorSelection {
		return switch value {
			case NoEditorSelection: NoEditorSelection;
			case VoxelSelection(bounds): VoxelSelection(copyBounds(bounds));
			case NodeSelection(ref): NodeSelection(ref);
		};
	}

	static function copyBounds(value:VoxelBounds):VoxelBounds {
		return {
			origin: {x: value.origin.x, y: value.origin.y, z: value.origin.z},
			size: {width: value.size.width, height: value.size.height, depth: value.size.depth}
		};
	}

	static function selectionsEqual(left:EditorSelection, right:EditorSelection):Bool {
		return switch left {
			case NoEditorSelection:
				switch right {
					case NoEditorSelection: true;
					case VoxelSelection(_) | NodeSelection(_): false;
				}
			case VoxelSelection(bounds):
				switch right {
					case VoxelSelection(other): boundsEqual(bounds, other);
					case NoEditorSelection | NodeSelection(_): false;
				}
			case NodeSelection(ref):
				switch right {
					case NodeSelection(other): sameNodeRef(ref, other);
					case NoEditorSelection | VoxelSelection(_): false;
				}
		};
	}

	static function boundsEqual(left:VoxelBounds, right:VoxelBounds):Bool
		return left.origin.x == right.origin.x
			&& left.origin.y == right.origin.y
			&& left.origin.z == right.origin.z
			&& left.size.width == right.size.width
			&& left.size.height == right.size.height
			&& left.size.depth == right.size.depth;
}

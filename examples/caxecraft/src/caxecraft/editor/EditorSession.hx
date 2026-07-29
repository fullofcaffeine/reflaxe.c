package caxecraft.editor;

import caxecraft.editor.EditorCommandReducer.EditorReductionResult;
import caxecraft.editor.EditorCommandReducer.apply as reduceCommand;
import caxecraft.editor.EditorHistory.EditorHistoryEntry;
import caxecraft.editor.EditorPolicy.defaults as defaultEditorSettings;
import caxecraft.editor.EditorPolicy.validate as validateEditorSettings;
import caxecraft.editor.EditorScenarioSnapshot.EditorScenarioImage;
import caxecraft.editor.EditorScenarioSnapshot.EditorScenarioImageResult;
import caxecraft.editor.EditorScenarioSnapshot.capture as captureScenario;
import caxecraft.editor.EditorScenarioSnapshot.restore as restoreScenario;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorCommandFamily;
import caxecraft.editor.EditorTypes.EditorEditResult;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorHistoryResult;
import caxecraft.editor.EditorTypes.EditorMutationRequest;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorObservation;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorQuery;
import caxecraft.editor.EditorTypes.EditorSettings;
import caxecraft.editor.EditorTypes.EditorTestPlayResult;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioValidator;
import haxe.io.Bytes;

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
	var selection:Null<VoxelBounds>;
	var lastPlayable:Null<Scenario>;
	var activeTestPlay:Null<EditorTestPlay>;
	var currentRevision:Int;

	function new(image:EditorScenarioImage, registry:ScenarioContentRegistry, settings:EditorSettings) {
		this.registry = registry;
		this.settings = settings;
		this.history = new EditorHistory(settings);
		this.draft = image.parsed.candidate;
		this.selection = null;
		this.lastPlayable = validatedScenario(image);
		this.activeTestPlay = null;
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
		Apply one command against the session's current in-process draft.

		This is the small convenience API for code that already owns the session
		and performs one synchronous edit. It still records history and advances
		the revision. A UI view, automation client, or future remote adapter must
		use `mutate` instead so a revision check can reject stale work.
	**/
	public function apply(command:EditorCommand):EditorEditResult {
		if (activeTestPlay != null)
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
					editing: activeTestPlay == null
				});
			case InspectDraft:
				DraftObserved(currentRevision, draftSnapshot());
			case InspectCanonicalDraft:
				CanonicalDraftObserved(currentRevision, canonicalDraft());
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
		if (activeTestPlay != null)
			return MutationRejected(NotEditing, currentRevision);
		if (commands.length == 0)
			return MutationRejected(EmptyTransaction, currentRevision);
		if (commands.length > settings.transactionCommands)
			return MutationRejected(TransactionTooLarge(commands.length, settings.transactionCommands), currentRevision);
		return switch captureScenario(draft) {
			case ImageRejected(error): MutationRejected(error, currentRevision);
			case ImageReady(before): stageBatch(before, commands);
		}
	}

	function stageBatch(before:EditorScenarioImage, commands:Array<EditorCommand>):EditorMutationResult {
		var staged = before;
		var stagedSelection = copySelection(selection);
		final families:Array<EditorCommandFamily> = [];
		for (command in commands) {
			switch command {
				case RestoreLastPlayable:
					if (lastPlayable == null)
						return MutationRejected(NoPlayableScenario, currentRevision);
					switch captureScenario(lastPlayable) {
						case ImageRejected(error):
							return MutationRejected(error, currentRevision);
						case ImageReady(image):
							staged = image;
							stagedSelection = null;
							families.push(Recovery);
					}
				case _:
					switch reduceCommand(staged.parsed.candidate, stagedSelection, command, settings) {
						case ReductionRejected(error):
							return MutationRejected(error, currentRevision);
						case ReductionReady(reduction):
							switch captureScenario(reduction.scenario) {
								case ImageRejected(error):
									return MutationRejected(error, currentRevision);
								case ImageReady(image):
									staged = image;
									stagedSelection = copySelection(reduction.selection);
									families.push(reduction.family);
							}
					}
			}
		}
		return switch accept(before, selection, staged, stagedSelection, Transaction) {
			case EditApplied(_, undoDepth, redoDepth):
				MutationApplied(families.copy(), currentRevision, undoDepth, redoDepth);
			case EditUnchanged(_):
				MutationUnchanged(families.copy(), currentRevision);
			case EditRejected(error):
				MutationRejected(error, currentRevision);
		}
	}

	function mutationFromEdit(result:EditorEditResult):EditorMutationResult {
		return switch result {
			case EditApplied(family, undoDepth, redoDepth):
				MutationApplied([family], currentRevision, undoDepth, redoDepth);
			case EditUnchanged(family):
				MutationUnchanged([family], currentRevision);
			case EditRejected(error):
				MutationRejected(error, currentRevision);
		}
	}

	function mutationFromHistory(result:EditorHistoryResult):EditorMutationResult {
		return switch result {
			case HistoryApplied(family, undoDepth, redoDepth):
				MutationApplied([family], currentRevision, undoDepth, redoDepth);
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
		return switch reduceCommand(before.parsed.candidate, selection, command, settings) {
			case ReductionRejected(error): EditRejected(error);
			case ReductionReady(reduction):
				switch captureScenario(reduction.scenario) {
					case ImageRejected(error): EditRejected(error);
					case ImageReady(after): accept(before, selection, after, reduction.selection, reduction.family);
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
		if (activeTestPlay != null)
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
				selection = copySelection(entry.beforeSelection);
				advanceRevision();
				HistoryApplied(entry.family, history.undoDepth(), history.redoDepth());
		}
	}

	/**
		Reapply the newest entry removed by `undo`.

		Like every committed state change, a successful redo advances the
		revision. Revision-aware callers should request it through `mutate`.
	**/
	public function redo():EditorHistoryResult {
		if (activeTestPlay != null)
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
				selection = copySelection(entry.afterSelection);
				advanceRevision();
				HistoryApplied(entry.family, history.undoDepth(), history.redoDepth());
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
		if (activeTestPlay != null)
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
						activeTestPlay = new EditorTestPlay(snapshot);
						TestPlayStarted;
					}
				}
		}
	}

	/** Discard every test-play mutation and return to the untouched draft. */
	public function leaveTestPlay():Bool {
		if (activeTestPlay == null)
			return false;
		activeTestPlay = null;
		return true;
	}

	public inline function testPlay():Null<EditorTestPlay>
		return activeTestPlay;

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

	public function selectedBounds():Null<VoxelBounds>
		return copySelection(selection);

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
			case ImageReady(after): accept(before, selection, after, null, Recovery);
		}
	}

	function accept(before:EditorScenarioImage, beforeSelection:Null<VoxelBounds>, after:EditorScenarioImage, afterSelection:Null<VoxelBounds>,
			family:EditorCommandFamily):EditorEditResult {
		if (before.bytes.compare(after.bytes) == 0 && selectionsEqual(beforeSelection, afterSelection))
			return EditUnchanged(family);
		if (currentRevision == 2147483647)
			return EditRejected(RevisionExhausted);
		final byteCost = before.bytes.length + after.bytes.length;
		if (!history.canRecord(byteCost))
			return EditRejected(HistoryEntryTooLarge(byteCost, settings.historyBytes));
		final entry:EditorHistoryEntry = {
			family: family,
			before: before.bytes.sub(0, before.bytes.length),
			beforeSelection: copySelection(beforeSelection),
			after: after.bytes.sub(0, after.bytes.length),
			afterSelection: copySelection(afterSelection),
			byteCost: byteCost
		};
		history.record(entry);
		draft = after.parsed.candidate;
		selection = copySelection(afterSelection);
		advanceRevision();
		return EditApplied(family, history.undoDepth(), history.redoDepth());
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

	static function copySelection(value:Null<VoxelBounds>):Null<VoxelBounds> {
		if (value == null)
			return null;
		return {
			origin: {x: value.origin.x, y: value.origin.y, z: value.origin.z},
			size: {width: value.size.width, height: value.size.height, depth: value.size.depth}
		};
	}

	static function selectionsEqual(left:Null<VoxelBounds>, right:Null<VoxelBounds>):Bool {
		if (left == null || right == null)
			return left == null && right == null;
		return left.origin.x == right.origin.x
			&& left.origin.y == right.origin.y
			&& left.origin.z == right.origin.z
			&& left.size.width == right.size.width
			&& left.size.height == right.size.height
			&& left.size.depth == right.size.depth;
	}
}

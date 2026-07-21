package caxecraft.editor;

import caxecraft.editor.EditorCommandReducer.EditorReductionResult;
import caxecraft.editor.EditorHistory.EditorHistoryEntry;
import caxecraft.editor.EditorScenarioSnapshot.EditorScenarioImage;
import caxecraft.editor.EditorScenarioSnapshot.EditorScenarioImageResult;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorCommandFamily;
import caxecraft.editor.EditorTypes.EditorEditResult;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorHistoryResult;
import caxecraft.editor.EditorTypes.EditorOpenResult;
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

	function new(image:EditorScenarioImage, registry:ScenarioContentRegistry, settings:EditorSettings) {
		this.registry = registry;
		this.settings = settings;
		this.history = new EditorHistory(settings);
		this.draft = image.parsed.candidate;
		this.selection = null;
		this.lastPlayable = validatedScenario(image);
		this.activeTestPlay = null;
	}

	/** Open even a semantically invalid draft so the editor can repair it. */
	public static function open(initial:Scenario, registry:ScenarioContentRegistry, ?requested:EditorSettings):EditorOpenResult {
		final settings = requested == null ? EditorPolicy.defaults() : requested;
		final invalidSetting = EditorPolicy.validate(settings);
		if (invalidSetting != null)
			return EditorOpenRejected(invalidSetting);
		return switch EditorScenarioSnapshot.capture(initial) {
			case ImageRejected(error): EditorOpenRejected(error);
			case ImageReady(image): EditorOpened(new EditorSession(image, registry, settings));
		}
	}

	public function apply(command:EditorCommand):EditorEditResult {
		if (activeTestPlay != null)
			return EditRejected(NotEditing);
		return switch EditorScenarioSnapshot.capture(draft) {
			case ImageRejected(error): EditRejected(error);
			case ImageReady(before): applyToImage(before, command);
		}
	}

	function applyToImage(before:EditorScenarioImage, command:EditorCommand):EditorEditResult {
		switch command {
			case RestoreLastPlayable:
				return restorePlayable(before);
			case _:
		}
		return switch EditorCommandReducer.apply(before.parsed.candidate, selection, command, settings) {
			case ReductionRejected(error): EditRejected(error);
			case ReductionReady(reduction):
				switch EditorScenarioSnapshot.capture(reduction.scenario) {
					case ImageRejected(error): EditRejected(error);
					case ImageReady(after): accept(before, selection, after, reduction.selection, reduction.family);
				}
		}
	}

	public function undo():EditorHistoryResult {
		if (activeTestPlay != null)
			return HistoryRejected(NotEditing);
		final entry = history.takeUndo();
		if (entry == null)
			return HistoryRejected(NothingToUndo);
		return switch EditorScenarioSnapshot.restore(entry.before) {
			case ImageRejected(error):
				history.takeRedo();
				HistoryRejected(error);
			case ImageReady(image):
				draft = image.parsed.candidate;
				selection = copySelection(entry.beforeSelection);
				HistoryApplied(entry.family, history.undoDepth(), history.redoDepth());
		}
	}

	public function redo():EditorHistoryResult {
		if (activeTestPlay != null)
			return HistoryRejected(NotEditing);
		final entry = history.takeRedo();
		if (entry == null)
			return HistoryRejected(NothingToRedo);
		return switch EditorScenarioSnapshot.restore(entry.after) {
			case ImageRejected(error):
				history.takeUndo();
				HistoryRejected(error);
			case ImageReady(image):
				draft = image.parsed.candidate;
				selection = copySelection(entry.afterSelection);
				HistoryApplied(entry.family, history.undoDepth(), history.redoDepth());
		}
	}

	/** Validate the draft and update the separate last-known-playable snapshot. */
	public function validate():EditorValidationResult {
		return switch EditorScenarioSnapshot.capture(draft) {
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
		return switch EditorScenarioSnapshot.capture(draft) {
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
		return switch EditorScenarioSnapshot.capture(lastPlayable) {
			case ImageRejected(error): EditRejected(error);
			case ImageReady(after): accept(before, selection, after, null, Recovery);
		}
	}

	function accept(before:EditorScenarioImage, beforeSelection:Null<VoxelBounds>, after:EditorScenarioImage, afterSelection:Null<VoxelBounds>,
			family:EditorCommandFamily):EditorEditResult {
		if (before.bytes.compare(after.bytes) == 0 && selectionsEqual(beforeSelection, afterSelection))
			return EditUnchanged(family);
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
		return EditApplied(family, history.undoDepth(), history.redoDepth());
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
		return switch EditorScenarioSnapshot.capture(scenario) {
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

package caxecraft.qa;

import caxecraft.editor.EditorPolicy;
import caxecraft.editor.EditorScenarioFactory;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorCommandFamily;
import caxecraft.editor.EditorTypes.EditorEditResult;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorHistoryResult;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorSettings;
import caxecraft.editor.EditorTypes.EditorTestPlayResult;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioStory.ObjectiveState;
import caxecraft.scenario.ScenarioTag;
import caxecraft.scenario.ScenarioText;
import caxecraft.scenario.ScenarioValidator;
import caxecraft.scenario.ScenarioWriter;
import haxe.io.Bytes;

/** Complete public-command acceptance proof for renderer-independent editing. */
final class EditorProbe {
	static final AIR = content("caxecraft:air");
	static final STONE = content("caxecraft:stone");
	static final PREFAB = content("caxecraft:small-house");
	static final NPC = content("caxecraft:ivvy");
	static final PLAYER = id("player.spawn");
	static final CHECKPOINT = id("checkpoint.first");
	static final ZONE = id("zone.finish");
	static final IVVY = id("npc.ivvy");
	static final DIALOGUE = id("dialogue.ivvy");
	static final OBJECTIVE = id("objective.finish");
	static final RULE = id("rule.finish");

	static function main():Void {
		final session = open(EditorPolicy.defaults());
		var commandChecks = 0;
		commandChecks += roundTrip(session, ResizeWorld({width: 4, height: 2, depth: 4}), WorldShape);
		commandChecks += roundTrip(session, SetPaletteEntry(1, STONE), Voxel);
		commandChecks += roundTrip(session, PaintVoxel({x: 1, y: 0, z: 1}, 1), Voxel);
		commandChecks += roundTrip(session, EraseVoxel({x: 1, y: 0, z: 1}), Voxel);
		commandChecks += roundTrip(session, PaintVoxels([{x: 2, y: 0, z: 1}, {x: 2, y: 0, z: 2}], 1), Voxel);
		commandChecks += roundTrip(session, EraseVoxels([{x: 2, y: 0, z: 1}, {x: 2, y: 0, z: 2}]), Voxel);
		commandChecks += roundTrip(session, Select({origin: {x: 0, y: 0, z: 0}, size: {width: 2, height: 1, depth: 2}}), Selection);
		commandChecks += roundTrip(session, FillSelection(1), Voxel);
		commandChecks += roundTrip(session, ClearSelection, Selection);
		commandChecks += roundTrip(session, Select({origin: {x: 0, y: 0, z: 0}, size: {width: 1, height: 1, depth: 1}}), Selection);
		commandChecks += roundTrip(session, StampPrefab(id("prefab.house"), PREFAB, [new ScenarioTag("landmark")], transform(2500, 0, 2500)), Prefab);
		commandChecks += roundTrip(session, PutDialogue({
			id: DIALOGUE,
			lines: [{speaker: null, text: Message(new MessageId("dialogue.ivvy.hello"))}]
		}), Dialogue);
		commandChecks += roundTrip(session, PutObject({id: CHECKPOINT, tags: [], placement: Checkpoint(transform(1500, 0, 1500))}), Placement);
		commandChecks += roundTrip(session, PutObject({
			id: ZONE,
			tags: [new ScenarioTag("finish")],
			placement: TriggerZone({origin: {x: 3, y: 0, z: 3}, size: {width: 1, height: 1, depth: 1}})
		}), Placement);
		commandChecks += roundTrip(session, PutObject({id: IVVY, tags: [], placement: Npc(NPC, DIALOGUE, transform(500, 0, 1500))}), Placement);
		commandChecks += roundTrip(session, PutObjective({
			id: OBJECTIVE,
			title: Message(new MessageId("objective.finish.title")),
			body: Message(new MessageId("objective.finish.body")),
			initialState: Active
		}), Objective);
		commandChecks += roundTrip(session, PutRule({
			id: RULE,
			priority: 10,
			repeat: Once,
			event: Interact(CHECKPOINT),
			predicate: Always,
			actions: [SetObjective(OBJECTIVE, Complete)]
		}), Rule);

		final canonical = expectValid(session, "complete command-built scenario");
		expectCodecRoundTrip(canonical);
		checkTestPlayIsolation(session);
		checkInvalidRecovery(session, canonical);
		checkRemoveCommands(session);
		checkHardBounds();
		checkHistoryStateChanges();
		checkSnapshotFidelity();
		checkTestPlayLocksEditing();
		checkImmediateRejections(session);

		final finalBytes = expectValid(session, "final recovered scenario");
		final trace = hash(finalBytes) ^ (commandChecks * 65537) ^ session.historyEntries();
		Sys.println('caxemap-editor: $commandChecks command round trips, ${finalBytes.length} canonical bytes; bounded history/test-play/recovery; trace=$trace');
	}

	static function checkTestPlayIsolation(session:EditorSession):Void {
		requireTestStarted(session.enterTestPlay(), "first test play");
		final test = session.testPlay();
		require(test != null, "test play did not publish its disposable simulation");
		require(test.objectiveState(OBJECTIVE) == Active, "test play did not start from authored objective state");
		final result = test.runTick({events: [Interact(CHECKPOINT)], positions: []});
		require(result.diagnostics.length == 0, "test-play rule execution failed");
		require(test.objectiveState(OBJECTIVE) == Complete, "test-play rule did not mutate disposable state");
		require(session.leaveTestPlay(), "leaving active test play failed");
		requireTestStarted(session.enterTestPlay(), "second test play");
		final fresh = session.testPlay();
		require(fresh != null && fresh.objectiveState(OBJECTIVE) == Active, "test-play changes leaked into the editor draft");
		require(session.leaveTestPlay(), "leaving second test play failed");
	}

	static function checkInvalidRecovery(session:EditorSession, lastValid:Bytes):Void {
		expectApplied(session.apply(RemoveObject(PLAYER)), Placement, "remove required spawn");
		final diagnostics = switch session.validate() {
			case ValidationFailed(values): values;
			case _: throw "draft without a player spawn unexpectedly validated";
		};
		require(hasMissingSpawn(diagnostics), "invalid draft lost the exact missing-spawn diagnostic");
		switch session.enterTestPlay() {
			case TestPlayRejected(values):
				require(hasMissingSpawn(values), "test play rejected the wrong invalid-draft reason");
			case _:
				throw "invalid draft entered test play";
		}
		final retained = session.lastPlayableSnapshot();
		require(retained != null
			&& ScenarioWriter.write(retained).compare(lastValid) == 0, "invalid edit replaced the last playable snapshot");

		expectApplied(session.apply(RestoreLastPlayable), Recovery, "restore last playable");
		expectHistory(session.undo(), Recovery, "undo recovery");
		requireValidationFailure(session, "undo recovery should restore the invalid draft");
		expectHistory(session.redo(), Recovery, "redo recovery");
		expectValid(session, "redo recovery");
	}

	static function checkRemoveCommands(session:EditorSession):Void {
		for (entry in [
			{command: RemoveRule(RULE), family: Rule, label: "remove rule"},
			{command: RemoveObjective(OBJECTIVE), family: Objective, label: "remove objective"},
			{command: RemoveDialogue(DIALOGUE), family: Dialogue, label: "remove dialogue"}
		]) {
			expectApplied(session.apply(entry.command), entry.family, entry.label);
			expectHistory(session.undo(), entry.family, 'undo ${entry.label}');
			expectHistory(session.redo(), entry.family, 'redo ${entry.label}');
			expectHistory(session.undo(), entry.family, 'restore after ${entry.label}');
		}
	}

	static function checkHardBounds():Void {
		final settings:EditorSettings = {historyEntries: 3, historyBytes: 1048576, selectionCells: 4};
		final session = open(settings);
		expectApplied(session.apply(ResizeWorld({width: 3, height: 1, depth: 3})), WorldShape, "bounded resize");
		for (x in 0...3)
			for (z in 0...2)
				expectApplied(session.apply(Select({origin: {x: x, y: 0, z: z}, size: {width: 1, height: 1, depth: 1}})), Selection,
					"bounded selection history");
		require(session.historyEntries() == 3 && session.undoDepth() == 3, "history did not evict to its exact entry bound");
		require(session.historyBytes() <= settings.historyBytes, "history exceeded its byte bound");
		expectRejected(session.apply(Select({origin: {x: 0, y: 0, z: 0}, size: {width: 3, height: 1, depth: 2}})), error -> switch error {
			case SelectionTooLarge(6, 4): true;
			case _: false;
		}, "oversized selection");
		expectRejected(session.apply(PaintVoxels([
			{x: 0, y: 0, z: 0},
			{x: 1, y: 0, z: 0},
			{x: 2, y: 0, z: 0},
			{x: 0, y: 0, z: 1},
			{x: 1, y: 0, z: 1}
		], 0)), error -> switch error {
			case VoxelEditTooLarge(5, 4): true;
			case _: false;
		}, "oversized paint gesture");

		final tiny = open({historyEntries: 3, historyBytes: 1, selectionCells: 4});
		final before = tiny.canonicalDraft();
		expectRejected(tiny.apply(ResizeWorld({width: 2, height: 1, depth: 1})), error -> switch error {
			case HistoryEntryTooLarge(_, 1): true;
			case _: false;
		}, "history byte budget");
		require(tiny.canonicalDraft().compare(before) == 0, "rejected history entry changed the draft");

		final invalidSettings:EditorSettings = {
			historyEntries: EditorPolicy.MAX_HISTORY_ENTRIES + 1,
			historyBytes: 1,
			selectionCells: 1
		};
		switch EditorSession.open(baseScenario(), new Registry(), invalidSettings) {
			case EditorOpenRejected(InvalidSetting(HistoryEntries, 1, EditorPolicy.MAX_HISTORY_ENTRIES)):
			case _:
				throw "editor accepted settings above the hard history-entry bound";
		}
	}

	static function checkSnapshotFidelity():Void {
		final unsupported = withFormatVersion(baseScenario(), 2);
		switch EditorSession.open(unsupported, new Registry()) {
			case EditorOpenRejected(UnsupportedFormatVersion(2, ScenarioWriter.FORMAT_VERSION)):
			case _:
				throw "editor silently normalized an unsupported CAXEMAP version";
		}

		final session = open(EditorPolicy.defaults());
		expectApplied(session.apply(PutObject({id: id("narrator"), tags: [], placement: Checkpoint(transform(0, 0, 0))})), Placement,
			"place narrator-named speaker");
		expectApplied(session.apply(PutDialogue({
			id: id("dialogue.narrator-object"),
			lines: [{speaker: id("narrator"), text: Literal("I am an object, not narration.")}]
		})), Dialogue, "author narrator-named speaker");
		final copy = session.draftSnapshot();
		final speaker = copy.story.dialogues[0].lines[0].speaker;
		require(speaker != null && speaker.text() == "narrator", "editor snapshot changed narrator-named speaker into narration");
	}

	static function checkHistoryStateChanges():Void {
		final session = open(EditorPolicy.defaults());
		expectApplied(session.apply(ResizeWorld({width: 2, height: 1, depth: 1})), WorldShape, "history accounting edit");
		final recordedBytes = session.historyBytes();
		require(recordedBytes > 0 && session.historyEntries() == 1, "accepted edit was not counted in history");
		expectHistory(session.undo(), WorldShape, "history accounting undo");
		require(session.historyBytes() == recordedBytes
			&& session.historyEntries() == 1
			&& session.undoDepth() == 0
			&& session.redoDepth() == 1,
			"moving an entry to redo changed shared history accounting");
		expectApplied(session.apply(SetPaletteEntry(1, STONE)), Voxel, "new branch after undo");
		require(session.redoDepth() == 0, "a new edit retained the abandoned redo branch");
		switch session.redo() {
			case HistoryRejected(NothingToRedo):
			case _:
				throw "redo restored an abandoned history branch";
		}
	}

	static function checkTestPlayLocksEditing():Void {
		final session = open(EditorPolicy.defaults());
		expectApplied(session.apply(ResizeWorld({width: 2, height: 1, depth: 1})), WorldShape, "pre-test-play edit");
		requireTestStarted(session.enterTestPlay(), "editing lock test play");
		expectRejected(session.apply(SetPaletteEntry(1, STONE)), error -> switch error {
			case NotEditing: true;
			case _: false;
		}, "edit during test play");
		for (result in [session.undo(), session.redo()])
			switch result {
				case HistoryRejected(NotEditing):
				case _:
					throw "history changed while test play was active";
			}
		require(session.leaveTestPlay(), "editing lock test play did not close");
	}

	static function checkImmediateRejections(session:EditorSession):Void {
		expectRejected(session.apply(PaintVoxel({x: 99, y: 0, z: 0}, 1)), error -> switch error {
			case PointOutsideWorld(_): true;
			case _: false;
		}, "outside paint");
		expectRejected(session.apply(PaintVoxel({x: 0, y: 0, z: 0}, 99)), error -> switch error {
			case UnknownPaletteCode(99): true;
			case _: false;
		}, "unknown palette paint");
		expectRejected(session.apply(ResizeWorld({width: 129, height: 1, depth: 1})), error -> switch error {
			case InvalidWorldSize(_): true;
			case _: false;
		}, "oversized world");
		expectRejected(session.apply(Select({
			origin: {x: 1, y: 0, z: 0},
			size: {width: 2147483647, height: 1, depth: 1}
		})), error -> switch error {
			case BoundsOutsideWorld(_): true;
			case _: false;
		}, "overflow-shaped selection");
	}

	static function roundTrip(session:EditorSession, command:EditorCommand, family:EditorCommandFamily):Int {
		final before = session.canonicalDraft();
		final beforeSelection = selectionKey(session);
		expectApplied(session.apply(command), family, "apply command");
		final after = session.canonicalDraft();
		final afterSelection = selectionKey(session);
		require(before.compare(after) != 0 || beforeSelection != afterSelection, "accepted command made no observable change");
		expectHistory(session.undo(), family, "undo command");
		require(session.canonicalDraft().compare(before) == 0
			&& selectionKey(session) == beforeSelection, "undo did not restore exact prior state");
		expectHistory(session.redo(), family, "redo command");
		require(session.canonicalDraft().compare(after) == 0
			&& selectionKey(session) == afterSelection, "redo did not restore exact command state");
		return 1;
	}

	static function expectCodecRoundTrip(bytes:Bytes):Void {
		final records = switch ScenarioLexer.read(bytes) {
			case ReadOk(value): value;
			case ReadError(_): throw "editor bytes did not lex";
		};
		final parsed = switch ScenarioParser.parse(records) {
			case ReadOk(value): value;
			case ReadError(_): throw "editor bytes did not parse";
		};
		final scenario = switch ScenarioValidator.validate(parsed, new Registry()) {
			case ReadOk(value): value;
			case ReadError(_): throw "editor bytes did not validate after reload";
		};
		require(ScenarioWriter.write(scenario).compare(bytes) == 0, "editor save/reload changed canonical bytes");
	}

	static function expectValid(session:EditorSession, label:String):Bytes {
		return switch session.validate() {
			case ValidationPassed(bytes): bytes;
			case ValidationFailed(diagnostics): throw '$label failed with ${diagnostics.length} semantic diagnostics';
			case ValidationBlocked(_): throw '$label could not be represented';
		}
	}

	static function requireValidationFailure(session:EditorSession, label:String):Void {
		switch session.validate() {
			case ValidationFailed(_):
			case _:
				throw label;
		}
	}

	static function expectApplied(result:EditorEditResult, family:EditorCommandFamily, label:String):Void {
		switch result {
			case EditApplied(actual, _, _):
				require(actual == family, '$label reported the wrong command family');
			case EditUnchanged(_):
				throw '$label unexpectedly made no change';
			case EditRejected(error):
				throw '$label was rejected: $error';
		}
	}

	static function expectHistory(result:EditorHistoryResult, family:EditorCommandFamily, label:String):Void {
		switch result {
			case HistoryApplied(actual, _, _):
				require(actual == family, '$label reported the wrong command family');
			case HistoryRejected(error):
				throw '$label was rejected: $error';
		}
	}

	static function expectRejected(result:EditorEditResult, matches:EditorError->Bool, label:String):Void {
		switch result {
			case EditRejected(error):
				require(matches(error), '$label returned the wrong error: $error');
			case _:
				throw '$label unexpectedly changed the draft';
		}
	}

	static function requireTestStarted(result:EditorTestPlayResult, label:String):Void {
		switch result {
			case TestPlayStarted:
			case TestPlayRejected(values):
				throw '$label failed with ${values.length} diagnostics';
			case TestPlayBlocked(error):
				throw '$label was blocked: $error';
		}
	}

	static function hasMissingSpawn(values:Array<caxecraft.scenario.ScenarioDiagnostic>):Bool {
		for (value in values)
			switch value.kind {
				case MissingRecord(SinglePlayerSpawn):
					return true;
				case _:
			}
		return false;
	}

	static function selectionKey(session:EditorSession):String {
		final value = session.selectedBounds();
		return value == null ? "none" : '${value.origin.x},${value.origin.y},${value.origin.z}:${value.size.width},${value.size.height},${value.size.depth}';
	}

	static function open(settings:EditorSettings):EditorSession {
		return switch EditorSession.open(baseScenario(), new Registry(), settings) {
			case EditorOpened(session): session;
			case EditorOpenRejected(error): throw 'editor did not open: $error';
		}
	}

	static function baseScenario():Scenario
		return EditorScenarioFactory.create(id("editor.qa"), new LogicalPath("packs/caxecraft/base"), Message(new MessageId("scenario.editor.title")),
			Creative, AIR, PLAYER);

	static function withFormatVersion(source:Scenario, formatVersion:Int):Scenario
		return {
			formatVersion: formatVersion,
			requiredFeatures: source.requiredFeatures,
			optionalFeatures: source.optionalFeatures,
			id: source.id,
			assetPack: source.assetPack,
			title: source.title,
			mode: source.mode,
			world: source.world,
			objects: source.objects,
			story: source.story,
			flow: source.flow,
			extensions: source.extensions
		};

	static inline function transform(x:Int, y:Int, z:Int):caxecraft.scenario.ScenarioGeometry.ScenarioTransform
		return {
			xMilli: x,
			yMilli: y,
			zMilli: z,
			yawDegrees: 0
		};

	static function hash(bytes:Bytes):Int {
		var value = 17;
		for (index in 0...bytes.length)
			value = value * 31 + bytes.get(index);
		return value;
	}

	static inline function id(value:String):ScenarioId
		return new ScenarioId(value);

	static inline function content(value:String):ContentId
		return new ContentId(value);

	static function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}

private final class Registry implements ScenarioContentRegistry {
	public function new() {}

	public function supportsFeature(id:ContentId):Bool
		return id.text() == "caxecraft:core";

	public function isAirBlock(id:ContentId):Bool
		return id.text() == "caxecraft:air";

	public function hasBlock(id:ContentId):Bool
		return id.text() == "caxecraft:air" || id.text() == "caxecraft:stone";

	public function hasItem(id:ContentId):Bool
		return false;

	public function hasEntity(id:ContentId):Bool
		return false;

	public function hasNpc(id:ContentId):Bool
		return id.text() == "caxecraft:ivvy";

	public function hasPrefab(id:ContentId):Bool
		return id.text() == "caxecraft:small-house";

	public function hasStatefulObject(id:ContentId):Bool
		return false;

	public function hasState(id:ContentId):Bool
		return false;

	public function hasEffect(id:ContentId):Bool
		return false;

	public function hasSignal(id:ContentId):Bool
		return false;

	public function maximumItemQuantity(id:ContentId):Int
		return 64;
}

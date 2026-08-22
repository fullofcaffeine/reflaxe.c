package caxecraft.qa;

import caxecraft.editor.EditorActionPalette.availableScenarioActions;
import caxecraft.editor.EditorBuildControls.EditorBuildPointerState;
import caxecraft.editor.EditorBuildControls.EditorBuildTerrainAction;
import caxecraft.editor.EditorBuildControls.moveBuildFocus;
import caxecraft.editor.EditorBuildControls.nextPointerState;
import caxecraft.editor.EditorBuildControls.normalizeBuildFocus;
import caxecraft.editor.EditorBuildControls.normalizeBuildTool;
import caxecraft.editor.EditorBuildControls.terrainAction;
import caxecraft.editor.EditorBuildControls.toolForBuildHotbarSlot;
import caxecraft.editor.EditorBuildControls.usesDirectTerrainControls;
import caxecraft.editor.EditorFocus.EditorFocusMove;
import caxecraft.editor.EditorFocus.EditorFocusTarget;
import caxecraft.editor.EditorFocus.initialFocus;
import caxecraft.editor.EditorFocus.moveFocus;
import caxecraft.editor.EditorFlowProjection.EditorZoneRuleProjection;
import caxecraft.editor.EditorFlowProjection.projectZoneRules;
import caxecraft.editor.EditorEnvironment.EditorEnvironmentControl;
import caxecraft.editor.EditorEnvironment.EditorEnvironmentDirection;
import caxecraft.editor.EditorEnvironment.editEnvironment;
import caxecraft.editor.EditorEnvironment.firstEnvironmentControl;
import caxecraft.editor.EditorEnvironment.moveEnvironmentControl;
import caxecraft.editor.EditorPolicy.MAX_HISTORY_ENTRIES;
import caxecraft.editor.EditorPolicy.MAX_TRANSACTION_COMMANDS;
import caxecraft.editor.EditorPolicy.defaults as defaultEditorSettings;
import caxecraft.editor.EditorPlacement.checkpointCommand;
import caxecraft.editor.EditorPlacement.checkpointTemplate;
import caxecraft.editor.EditorObjectDuplicate.duplicateObject;
import caxecraft.editor.EditorPlacement.objectRecipeCommand;
import caxecraft.editor.EditorPlacement.triggerZoneCommand;
import caxecraft.editor.EditorObservationPlan.changesFor;
import caxecraft.editor.EditorPresentation.EditorPresentationWorld;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipe;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipeKind;
import caxecraft.editor.EditorScenarioFactory.create as createEditorScenario;
import caxecraft.editor.EditorRuntimeTerrain.EditorRuntimeTerrainResult;
import caxecraft.editor.EditorRuntimeTerrain.projectRuntimeTerrain;
import caxecraft.editor.EditorRuntimeTerrain.runtimeCodeForPalette;
import caxecraft.editor.EditorTerrainRefresh.EditorTerrainRefreshRequest;
import caxecraft.editor.EditorTerrainRefresh.forBatch as terrainRefreshForBatch;
import caxecraft.editor.EditorTerrainRefresh.forCommand as terrainRefreshForCommand;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorCommandFamily;
import caxecraft.editor.EditorTypes.EditorChangeId;
import caxecraft.editor.EditorTypes.EditorEditResult;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorHistoryResult;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorNodeRef;
import caxecraft.editor.EditorTypes.EditorObservation;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorPreviewResult;
import caxecraft.editor.EditorTypes.EditorSelection;
import caxecraft.editor.EditorTypes.EditorSelectionResult;
import caxecraft.editor.EditorTypes.EditorSettings;
import caxecraft.editor.EditorTypes.EditorTestPlayResult;
import caxecraft.editor.EditorTypes.EditorValidationObservation;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.editor.EditorViewport.EditorTool;
import caxecraft.editor.EditorViewport.EditorToolCommandResult;
import caxecraft.editor.EditorViewport.commandFor as commandForTool;
import caxecraft.editor.EditorViewport.boundsIntersectLayer;
import caxecraft.editor.EditorViewport.clampLayer;
import caxecraft.editor.EditorViewport.layout as layoutViewport;
import caxecraft.editor.EditorViewport.paletteCodeAt;
import caxecraft.editor.EditorViewport.paletteCodeForBlock;
import caxecraft.editor.EditorViewport.pointAt as viewportPointAt;
import caxecraft.editor.EditorViewport.project as projectViewport;
import caxecraft.editor.EditorViewport.projectFromCells;
import caxecraft.editor.EditorViewport.projectFromWorld;
import caxecraft.editor.EditorViewport.toolFromIndex;
import caxecraft.editor.EditorWorldViewport.cameraTarget;
import caxecraft.editor.EditorWorldViewport.cameraMode;
import caxecraft.editor.EditorWorldViewport.cameraPose;
import caxecraft.editor.EditorWorldViewport.cycleCameraMode;
import caxecraft.editor.EditorWorldViewport.EditorCameraMode;
import caxecraft.editor.EditorWorldViewport.EditorObjectFacing;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmo;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmoKind;
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.editor.EditorWorldViewport.EditorWorldVector;
import caxecraft.editor.EditorWorldViewport.EditorWorldHit;
import caxecraft.editor.EditorWorldViewport.focusCamera;
import caxecraft.editor.EditorWorldViewport.gizmoIntersectsLayer;
import caxecraft.editor.EditorWorldViewport.paletteCodeAtWorld;
import caxecraft.editor.EditorWorldViewport.pickObject;
import caxecraft.editor.EditorWorldViewport.pickWorld;
import caxecraft.editor.EditorWorldViewport.projectObjects;
import caxecraft.editor.EditorWorldViewport.projectWorld;
import caxecraft.editor.EditorWorldViewport.retargetOrbitCamera;
import caxecraft.editor.EditorWorldViewport.surfaceTopAt;
import caxecraft.editor.EditorWorldViewport.stepCamera;
import caxecraft.input.NavigationInput.NavigationCommand;
import caxecraft.input.NavigationInput.NavigationRepeater;
import caxecraft.input.NavigationInput.NavigationSample;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionArgumentRoles;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioEnvironment.ScenarioHorizonEdge;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioWorld;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioMessages;
import caxecraft.scenario.ScenarioMessages.ScenarioLocaleCatalog;
import caxecraft.scenario.ScenarioMessages.ScenarioMessage;
import caxecraft.scenario.ScenarioMessages.resolveScenarioMessage;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioStory.ObjectiveState;
import caxecraft.scenario.ScenarioTag;
import caxecraft.scenario.ScenarioText;
import caxecraft.scenario.ScenarioValidator;
import caxecraft.scenario.ScenarioWriter;
import caxecraft.scenario.ScenarioWorld.ScenarioFluidPlacement;
import haxe.io.Bytes;
import sys.io.File;

/** Complete public-command acceptance proof for renderer-independent editing. */
final class EditorProbe {
	static final AIR = content("caxecraft:air");
	static final STONE = content("caxecraft:stone");
	static final WATER = content("caxecraft:water");
	static final PREFAB = content("caxecraft:small-house");
	static final NPC = content("caxecraft:ivvy");
	static final PLAYER = id("player.spawn");
	static final CHECKPOINT = id("checkpoint.first");
	static final ZONE = id("zone.finish");
	static final IVVY = id("npc.ivvy");
	static final DIALOGUE = id("dialogue.ivvy");
	static final OBJECTIVE = id("objective.finish");
	static final RULE = id("rule.finish");
	static final WATER_POOL = id("water.pool");
	static final WATER_SOURCE = id("water.source");
	static final EN = new LocaleId("en");
	static final ES_MX = new LocaleId("es-mx");
	static final FR = new LocaleId("fr");
	static final TITLE_MESSAGE = new MessageId("scenario.editor.title");
	static final DIALOGUE_MESSAGE = new MessageId("dialogue.ivvy.hello");
	static final OBJECTIVE_TITLE_MESSAGE = new MessageId("objective.finish.title");
	static final OBJECTIVE_BODY_MESSAGE = new MessageId("objective.finish.body");

	static function main():Void {
		checkActionPalette();
		final protocolChecks = checkRevisionedProtocol() + checkTitleProtocol();
		final focusChecks = checkFocusNavigation();
		final navigationChecks = checkNavigationInput();
		final buildControlChecks = checkBuildControls();
		final viewportChecks = checkViewport();
		final worldViewportChecks = checkWorldViewport();
		final runtimeTerrainChecks = checkRuntimeTerrainProjection();
		checkZoneRuleProjection();
		final activeLevelChecks = checkActiveLevelProjection();
		checkEnvironmentTextRoundTrip();
		checkObjectMovement();
		checkObjectRotation();
		checkTriggerResize();
		checkCheckpointPlacement();
		checkTriggerPlacement();
		checkCatalogObjectPlacement();
		checkObjectDuplication();
		final session = open(defaultEditorSettings());
		var commandChecks = 0;
		commandChecks += roundTrip(session, SetTitle(Literal("Ivvy's workshop")), DocumentMetadata);
		commandChecks += roundTrip(session, ResizeWorld({width: 4, height: 2, depth: 4}), WorldShape);
		commandChecks += roundTrip(session, PutFluid({
			id: WATER_SOURCE,
			fluidType: WATER,
			placement: Source({x: 3, y: 1, z: 3})
		}), Fluid);
		commandChecks += roundTrip(session, PutFluid({
			id: WATER_POOL,
			fluidType: WATER,
			placement: InitialVolume({origin: {x: 2, y: 1, z: 2}, size: {width: 2, height: 1, depth: 2}})
		}), Fluid);
		commandChecks += roundTrip(session, SetPaletteEntry(1, STONE), Voxel);
		commandChecks += roundTrip(session, PaintVoxel({x: 1, y: 0, z: 1}, 1), Voxel);
		commandChecks += roundTrip(session, EraseVoxel({x: 1, y: 0, z: 1}), Voxel);
		commandChecks += roundTrip(session, PaintVoxels([{x: 2, y: 0, z: 1}, {x: 2, y: 0, z: 2}], 1), Voxel);
		commandChecks += roundTrip(session, EraseVoxels([{x: 2, y: 0, z: 1}, {x: 2, y: 0, z: 2}]), Voxel);
		final fillBounds:VoxelBounds = {origin: {x: 0, y: 0, z: 0}, size: {width: 2, height: 1, depth: 2}};
		expectSelection(session, VoxelSelection(fillBounds), "select fill bounds");
		commandChecks += roundTrip(session, FillBounds(fillBounds, 1), Voxel);
		expectSelection(session, NoEditorSelection, "clear workspace selection");
		expectSelection(session, VoxelSelection({origin: {x: 0, y: 0, z: 0}, size: {width: 1, height: 1, depth: 1}}), "select one voxel");
		commandChecks += roundTrip(session, StampPrefab(id("prefab.house"), PREFAB, [new ScenarioTag("landmark")], transform(2500, 0, 2500)), Prefab);
		commandChecks += roundTrip(session, SetDefaultLocale(ES_MX), Localization);
		commandChecks += roundTrip(session, PutMessage(EN, message(TITLE_MESSAGE, "Editor QA map, revised")), Localization);
		commandChecks += roundTrip(session,
			PutLocale(locale(FR, "Carte QA de l'editeur", "Bonjour, Haxirio.", "Reach the marker", "Use the checkpoint to finish.")), Localization);
		commandChecks += roundTrip(session, PutDialogue({
			id: DIALOGUE,
			lines: [{speaker: null, text: Message(DIALOGUE_MESSAGE)}]
		}), Dialogue);
		commandChecks += roundTrip(session, PutObject({id: CHECKPOINT, tags: [], placement: Checkpoint(transform(1500, 0, 1500))}), Placement);
		commandChecks += roundTrip(session, MoveObjectBy(CHECKPOINT, {x: 1, y: 0, z: 0}), Placement);
		commandChecks += roundTrip(session, RotateObjectBy(CHECKPOINT, 90), Placement);
		commandChecks += roundTrip(session, PutObject({
			id: ZONE,
			tags: [new ScenarioTag("finish")],
			placement: TriggerZone({origin: {x: 3, y: 0, z: 3}, size: {width: 1, height: 1, depth: 1}})
		}), Placement);
		commandChecks += roundTrip(session, PutObject({id: IVVY, tags: [], placement: Npc(NPC, DIALOGUE, transform(500, 0, 1500))}), Placement);
		commandChecks += roundTrip(session, PutObjective({
			id: OBJECTIVE,
			title: Message(OBJECTIVE_TITLE_MESSAGE),
			body: Message(OBJECTIVE_BODY_MESSAGE),
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
		checkLocalization(session);
		expectCodecRoundTrip(canonical);
		checkTestPlayIsolation(session);
		checkInvalidRecovery(session, canonical);
		checkRemoveCommands(session);
		checkHardBounds();
		checkHistoryStateChanges();
		checkSnapshotFidelity();
		checkPlacementInputIsolation();
		checkDeferredPlacementValidation();
		checkTestPlayLocksEditing();
		checkExternalTestPlayAtomicity();
		checkImmediateRejections(session);

		final finalBytes = expectValid(session, "final recovered scenario");
		final trace = hash(finalBytes) ^ (commandChecks * 65537) ^ (protocolChecks * 8191) ^ (focusChecks * 2053) ^ (navigationChecks * 1031) ^ (buildControlChecks * 521) ^ (viewportChecks * 4099) ^ (worldViewportChecks * 257) ^ (runtimeTerrainChecks * 67) ^ (activeLevelChecks * 131) ^ session.historyEntries();
		Sys.println('caxemap-editor: $commandChecks command round trips, $protocolChecks protocol checks, $focusChecks focus checks, $navigationChecks navigation checks, $buildControlChecks Build-control checks, $viewportChecks 2D checks, $worldViewportChecks 3D checks, $runtimeTerrainChecks runtime-terrain checks, $activeLevelChecks active-level checks, ${finalBytes.length} canonical bytes; bounded history/test-play/recovery; trace=$trace');
	}

	/** Prove that a valid editor draft becomes the gameplay renderer's fixed layout. */
	static function checkRuntimeTerrainProjection():Int {
		final registry = new Registry();
		final width = 32;
		final height = 16;
		final depth = 32;
		final sourceCells = [for (_ in 0...width * height * depth) 0];
		final authoredStoneIndex = (7 * height + 5) * width + 31;
		sourceCells[authoredStoneIndex] = 1;
		final world:EditorPresentationWorld = {
			size: {width: width, height: height, depth: depth},
			palette: [{code: 0, blockType: AIR}, {code: 1, blockType: STONE}]
		};
		final projection:EditorWorldProjection = {
			width: width,
			height: height,
			depth: depth,
			cells: sourceCells,
			columns: [],
			surfaceTops: [],
			surfacePatches: []
		};
		var checks = 0;
		switch projectRuntimeTerrain(world, projection, registry) {
			case RuntimeTerrainReady(cells):
				require(cells.length == 64 * 16 * 32, "runtime terrain did not fill the fixed gameplay volume");
				checks++;
				final runtimeStoneIndex = 31 + 64 * (5 + 16 * 7);
				require(cells[runtimeStoneIndex] == 3, "runtime terrain changed palette resolution or cell order");
				checks++;
				final paddedIndex = 32 + 64 * (5 + 16 * 7);
				require(cells[paddedIndex] == 0, "compact runtime terrain did not pad the unused half with air");
				checks++;
			case RuntimeTerrainUnavailable:
				throw "an admitted editor world did not project for the gameplay renderer";
		}
		final fullWidthCells = [for (_ in 0...64 * height * depth) 0];
		final fullWidthStoneIndex = 63 + 64 * (15 + height * 31);
		fullWidthCells[fullWidthStoneIndex] = 1;
		final fullWidthWorld:EditorPresentationWorld = {
			size: {width: 64, height: height, depth: depth},
			palette: world.palette
		};
		final fullWidthProjection:EditorWorldProjection = {
			width: 64,
			height: height,
			depth: depth,
			cells: fullWidthCells,
			columns: [],
			surfaceTops: [],
			surfacePatches: []
		};
		switch projectRuntimeTerrain(fullWidthWorld, fullWidthProjection, registry) {
			case RuntimeTerrainReady(cells):
				require(cells[fullWidthStoneIndex] == 3, "full-width runtime terrain lost its far boundary cell");
				checks++;
			case RuntimeTerrainUnavailable:
				throw "the full gameplay world did not project for the ordinary terrain renderer";
		}
		final unsupportedProjection:EditorWorldProjection = {
			width: 12,
			height: 1,
			depth: 12,
			cells: [for (_ in 0...144) 0],
			columns: [],
			surfaceTops: [],
			surfacePatches: []
		};
		final unsupportedWorld:EditorPresentationWorld = {
			size: {width: 12, height: 1, depth: 12},
			palette: [{code: 0, blockType: AIR}]
		};
		require(projectRuntimeTerrain(unsupportedWorld, unsupportedProjection, registry) == RuntimeTerrainUnavailable,
			"a custom-size draft bypassed the editor overview fallback");
		checks++;
		final missingPaletteCells = sourceCells.copy();
		missingPaletteCells[0] = 2;
		final missingPaletteProjection:EditorWorldProjection = {
			width: width,
			height: height,
			depth: depth,
			cells: missingPaletteCells,
			columns: [],
			surfaceTops: [],
			surfacePatches: []
		};
		require(projectRuntimeTerrain(world, missingPaletteProjection, registry) == RuntimeTerrainUnavailable,
			"a cell without a palette mapping became plausible runtime terrain");
		checks++;
		require(runtimeCodeForPalette(world, 0, registry) == 0 && runtimeCodeForPalette(world, 1, registry) == 3,
			"incremental terrain palette resolution disagreed with the complete projection");
		checks++;
		require(runtimeCodeForPalette(world, 2, registry) == -1, "incremental terrain palette resolution admitted a missing code");
		checks++;
		final duplicatePaletteWorld:EditorPresentationWorld = {
			size: world.size,
			palette: [{code: 0, blockType: AIR}, {code: 1, blockType: STONE}, {code: 1, blockType: AIR}]
		};
		require(runtimeCodeForPalette(duplicatePaletteWorld, 1, registry) == -1, "incremental terrain palette resolution admitted a duplicate code");
		checks++;
		switch terrainRefreshForCommand(PaintVoxel({x: 4, y: 5, z: 6}, 1)) {
			case RefreshTerrainVoxel(point):
				require(point.x == 4 && point.y == 5 && point.z == 6, "paint lost its incremental terrain coordinate");
			case KeepTerrain | RefreshAllTerrain:
				throw "paint requested a broad terrain refresh";
		}
		checks++;
		switch terrainRefreshForCommand(EraseVoxel({x: 7, y: 8, z: 9})) {
			case RefreshTerrainVoxel(point):
				require(point.x == 7 && point.y == 8 && point.z == 9, "erase lost its incremental terrain coordinate");
			case KeepTerrain | RefreshAllTerrain:
				throw "erase requested a broad terrain refresh";
		}
		checks++;
		for (command in [
			PaintVoxels([{x: 1, y: 0, z: 1}], 1),
			EraseVoxels([{x: 1, y: 0, z: 1}]),
			FillBounds({origin: {x: 0, y: 0, z: 0}, size: {width: 1, height: 1, depth: 1}}, 1),
			ResizeWorld({width: 64, height: 16, depth: 32}),
			SetPaletteEntry(1, STONE),
			RestoreLastPlayable
		]) {
			switch terrainRefreshForCommand(command) {
				case RefreshAllTerrain:
				case KeepTerrain | RefreshTerrainVoxel(_):
					throw "a broad terrain change requested a narrow refresh";
			}
			checks++;
		}
		switch terrainRefreshForCommand(SetTitle(Literal("Presentation only"))) {
			case KeepTerrain:
			case RefreshTerrainVoxel(_) | RefreshAllTerrain:
				throw "a title edit invalidated terrain";
		}
		checks++;
		switch terrainRefreshForBatch([SetTitle(Literal("Metadata batch"))]) {
			case KeepTerrain:
			case RefreshTerrainVoxel(_) | RefreshAllTerrain:
				throw "a metadata-only batch invalidated terrain";
		}
		checks++;
		switch terrainRefreshForBatch([SetTitle(Literal("Mixed batch")), PaintVoxel({x: 1, y: 0, z: 1}, 1)]) {
			case RefreshAllTerrain:
			case KeepTerrain | RefreshTerrainVoxel(_):
				throw "a terrain batch requested an incremental refresh";
		}
		checks++;
		return checks;
	}

	/** Prove that a creator gesture becomes one collision-free reloadable object. */
	static function checkCheckpointPlacement():Void {
		final existing:Array<ScenarioObject> = [
			{id: id("editor.checkpoint.n1"), tags: [], placement: Checkpoint(transform(500, 0, 500))},
			{id: id("editor.checkpoint.n2"), tags: [], placement: Checkpoint(transform(1500, 0, 500))}
		];
		switch checkpointCommand({x: 2, y: 1, z: 3}, existing) {
			case PutObject(object):
				require(object.id.text() == "editor.checkpoint.n3", "checkpoint placement reused an authored object ID");
				switch object.placement {
					case Checkpoint(position):
						require(position.xMilli == 2500 && position.yMilli == 1000 && position.zMilli == 3500 && position.yawDegrees == 0,
							"checkpoint placement changed the independently authored snapped transform");
					case _: throw "checkpoint placement emitted the wrong CAXEMAP role";
				}
			case _:
				throw "checkpoint placement did not use the normal object command";
		}
		final reservedRuleIds = [id("editor.rule.checkpoint.n3")];
		final allocated = checkpointTemplate({x: 2, y: 1, z: 3}, existing, reservedRuleIds);
		require(allocated.objectId.text() == "editor.checkpoint.n4", "checkpoint template reused a suffix reserved by a rule");
		switch allocated.commands {
			case [PutObject(object), PutRule(rule)]:
				require(object.id.text() == allocated.objectId.text() && rule.id.text() == "editor.rule.checkpoint.n4",
					"checkpoint template did not keep deterministic paired identities");
				switch [rule.event, rule.actions] {
					case [Interact(eventId), [SetCheckpoint(actionId)]]:
						require(eventId.text() == allocated.objectId.text() && actionId.text() == allocated.objectId.text(),
							"checkpoint template did not connect interaction to checkpoint state");
					case _: throw "checkpoint template changed its playable CaxeFlow rule";
				}
			case _:
				throw "checkpoint template did not emit object and rule commands";
		}

		final session = open(defaultEditorSettings());
		roundTrip(session, checkpointCommand({x: 0, y: 0, z: 0}, session.draftSnapshot().objects), Placement);

		final draft = session.draftSnapshot();
		final template = checkpointTemplate({x: 0, y: 0, z: 0}, draft.objects, [for (rule in draft.flow.rules) rule.id]);
		require(template.commands.length == 2, "checkpoint template did not keep object and behavior in one batch");
		final beforeBytes = session.canonicalDraft();
		final beforeHistory = session.historyEntries();
		switch session.preview({baseRevision: session.revision(), commands: template.commands}) {
			case PreviewAccepted(families, _, _):
				require(families.length == 2 && families[0] == Placement && families[1] == Rule, "checkpoint template preview changed command ownership");
			case other:
				throw 'checkpoint template preview failed: $other';
		}
		require(session.canonicalDraft().compare(beforeBytes) == 0 && session.historyEntries() == beforeHistory,
			"checkpoint template preview changed the live draft");
		final missingObject = id("editor.missing.checkpoint-template");
		switch session.mutate({baseRevision: session.revision(), mutation: ApplyBatch([template.commands[0], RemoveObject(missingObject)])}) {
			case MutationRejected(MissingObject(id), _):
				require(id.text() == missingObject.text(), "checkpoint template partial failure reported the wrong object");
			case other:
				throw 'checkpoint template partial failure was not atomic: $other';
		}
		require(session.canonicalDraft().compare(beforeBytes) == 0 && session.historyEntries() == beforeHistory,
			"checkpoint template partial failure changed bytes or history");
		switch session.mutate({baseRevision: session.revision(), mutation: ApplyBatch(template.commands)}) {
			case MutationApplied(families, _, _, undoDepth, redoDepth):
				require(families.length == 2 && families[0] == Placement && families[1] == Rule && undoDepth == beforeHistory + 1 && redoDepth == 0,
					"checkpoint template did not commit as one reversible transaction");
			case other:
				throw 'checkpoint template commit failed: $other';
		}
		final committed = expectValid(session, "playable checkpoint template");
		requireTestStarted(session.enterTestPlay(), "playable checkpoint template");
		final test = session.testPlay();
		require(test != null, "checkpoint template Test Play did not start");
		test.runTick({events: [Interact(template.objectId)], positions: []});
		final activeCheckpoint = test.checkpoint();
		require(activeCheckpoint != null && activeCheckpoint.text() == template.objectId.text(),
			"checkpoint template interaction did not change Test Play checkpoint state");
		require(session.leaveTestPlay(), "checkpoint template Test Play did not return to editing");
		require(session.canonicalDraft().compare(committed) == 0, "checkpoint template Test Play changed canonical bytes");
		switch session.mutate({baseRevision: session.revision(), mutation: Undo}) {
			case MutationApplied(_, _, _, _, _):
			case other:
				throw 'checkpoint template undo failed: $other';
		}
		require(session.canonicalDraft().compare(beforeBytes) == 0, "checkpoint template undo left a partial object or rule");
	}

	/** Prove one visual gesture creates exact one-cell trigger bounds. */
	static function checkTriggerPlacement():Void {
		final existing:Array<ScenarioObject> = [
			{id: id("editor.trigger.n1"), tags: [], placement: TriggerZone({origin: {x: 0, y: 0, z: 0}, size: {width: 1, height: 1, depth: 1}})},
			{id: id("editor.trigger.n3"), tags: [], placement: TriggerZone({origin: {x: 2, y: 0, z: 0}, size: {width: 1, height: 1, depth: 1}})}
		];
		final command = triggerZoneCommand({x: 2, y: 1, z: 3}, existing);
		final triggerId = id("editor.trigger.n2");
		switch command {
			case PutObject(object):
				require(object.id.text() == triggerId.text(), "trigger placement did not fill the first available ID gap");
				require(object.tags.length == 0, "trigger placement invented campaign-specific tags");
				switch object.placement {
					case TriggerZone(bounds):
						require(bounds.origin.x == 2 && bounds.origin.y == 1 && bounds.origin.z == 3, "trigger placement changed the selected voxel");
						require(bounds.size.width == 1 && bounds.size.height == 1 && bounds.size.depth == 1,
							"trigger placement did not create one-cell bounds");
					case _: throw "trigger placement emitted the wrong CAXEMAP role";
				}
			case _:
				throw "trigger placement did not use the normal object command";
		}
		final changes = changesFor(command);
		require(changes.length == 1 && isObjectChange(changes[0], triggerId), "trigger placement lost its changed-object identity");

		final session = open(defaultEditorSettings());
		expectApplied(session.apply(ResizeWorld({width: 4, height: 3, depth: 4})), WorldShape, "prepare trigger placement world");
		roundTrip(session, triggerZoneCommand({x: 2, y: 1, z: 3}, session.draftSnapshot().objects), Placement);
		final canonical = expectValid(session, "placed trigger");
		require(canonical.compare(session.canonicalDraft()) == 0, "trigger placement changed canonical save bytes during validation");
		requireTestStarted(session.enterTestPlay(), "placed trigger Test Play");
		require(session.leaveTestPlay(), "placed trigger Test Play did not return to editing");
		require(session.canonicalDraft().compare(canonical) == 0, "placed trigger Test Play changed the editor draft");
	}

	/** Prove one reloadable recipe crosses the canonical editor history boundary. */
	static function checkCatalogObjectPlacement():Void {
		final recipe = new EditorObjectRecipe("mechanism", "MECHANISM", "MECANISMO",
			EditorStatefulObject(new ContentId("caxecraft:mechanism"), new ContentId("caxecraft:idle")));
		final session = open(defaultEditorSettings());
		final command = objectRecipeCommand(recipe, {x: 1, y: 0, z: 2}, session.draftSnapshot().objects);
		switch command {
			case PutObject(object):
				require(object.id.text() == "editor.mechanism.n1", "catalog placement chose the wrong independent identity");
				switch object.placement {
					case StatefulObject(objectType, initialState, position):
						require(objectType.text() == "caxecraft:mechanism"
							&& initialState.text() == "caxecraft:idle"
							&& position.xMilli == 1500
							&& position.yMilli == 0
							&& position.zMilli == 2500,
							"catalog placement changed the independently authored payload");
					case _: throw "catalog placement changed the recipe kind";
				}
			case _:
				throw "catalog placement did not use the canonical object command";
		}
		roundTrip(session, command, Placement);
	}

	/** Prove one selected object becomes a distinct canonical copy with the same payload. */
	static function checkObjectDuplication():Void {
		final sourceId = id("machine.source");
		final source:ScenarioObject = {
			id: sourceId,
			tags: [new ScenarioTag("puzzle")],
			placement: StatefulObject(content("caxecraft:machine"), content("caxecraft:ready"), transform(1500, 1000, 2500))
		};
		final duplicate = duplicateObject(sourceId, [
			source,
			{
				id: id("machine.source.copy.n1"),
				tags: [],
				placement: Checkpoint(transform(500, 0, 500))
			}
		]);
		require(duplicate != null && duplicate.id.text() == "machine.source.copy.n2", "object duplication reused an authored identity");
		source.tags.push(new ScenarioTag("source-only"));
		switch duplicate.command {
			case PutObject(copy):
				require(copy.id.text() == duplicate.id.text() && copy.tags.length == 1 && copy.tags[0].text() == "puzzle",
					"object duplication lost identity or tags");
				switch copy.placement {
					case StatefulObject(objectType, initialState, position):
						require(objectType.text() == "caxecraft:machine"
							&& initialState.text() == "caxecraft:ready"
							&& position.xMilli == 1500
							&& position.yMilli == 1000
							&& position.zMilli == 2500,
							"object duplication changed the independently authored placement payload");
					case _: throw "object duplication changed the placement role";
				}
			case _:
				throw "object duplication did not use the canonical object command";
		}

		final session = open(defaultEditorSettings());
		final checkpointId = id("duplicate.checkpoint");
		expectApplied(session.apply(PutObject({id: checkpointId, tags: [], placement: Checkpoint(transform(500, 0, 500))})), Placement,
			"prepare duplicate history");
		final checkpointCopy = duplicateObject(checkpointId, session.draftSnapshot().objects);
		require(checkpointCopy != null, "object duplication lost a selected checkpoint");
		roundTrip(session, checkpointCopy.command, Placement);
	}

	/** Preserve an optional environment through the editor's text-byte boundary. */
	static function checkEnvironmentTextRoundTrip():Void {
		final source = File.getBytes("test/fixtures/caxemap/environment.caxemap");
		final opened = switch EditorSession.openBytes(source, new Registry(), defaultEditorSettings()) {
			case EditorOpened(value): value;
			case EditorOpenRejected(error): throw 'editor rejected the environment fixture: $error';
		};
		final environment = opened.draftSnapshot().environment;
		require(environment != null && environment.edges.length == 0 && environment.sun == null, "editor text import lost the optional environment choices");
		require(opened.canonicalDraft().compare(source) == 0, "editor text round-trip changed the environment bytes");

		var focus = firstEnvironmentControl();
		for (_ in 0...17)
			focus = moveEnvironmentControl(focus, EditorEnvironmentDirection.Increase);
		require(focus == EditorEnvironmentControl.Done
			&& moveEnvironmentControl(focus, EditorEnvironmentDirection.Increase) == EditorEnvironmentControl.Enabled
			&& moveEnvironmentControl(EditorEnvironmentControl.Enabled, EditorEnvironmentDirection.Decrease) == EditorEnvironmentControl.Done,
			"environment controls did not remain reachable in both directions");

		applyEnvironmentEdit(opened, EditorEnvironmentControl.SkyRed, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.SkyGreen, EditorEnvironmentDirection.Decrease);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.SkyBlue, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.SunEnabled, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.SunX, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.SunY, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.SunZ, EditorEnvironmentDirection.Decrease);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.SunRadius, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.CloudCount, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.CloudSpeed, EditorEnvironmentDirection.Decrease);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.CloudSeed, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.NorthEdge, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.SouthEdge, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.EastEdge, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.WestEdge, EditorEnvironmentDirection.Increase);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.SouthEdge, EditorEnvironmentDirection.Decrease);
		applyEnvironmentEdit(opened, EditorEnvironmentControl.ContinueWater, EditorEnvironmentDirection.Increase);
		final edited = opened.draftSnapshot().environment;
		require(edited != null
			&& edited.sky.red == 97
			&& edited.sky.green == 147
			&& edited.sky.blue == 172
			&& edited.sun != null
			&& edited.sun.x == -299
			&& edited.sun.y == 701
			&& edited.sun.z == 249
			&& edited.sun.radiusMilli == 301
			&& edited.clouds.count == 3
			&& edited.clouds.speedMilli == 499
			&& edited.clouds.seed == 74
			&& hasEnvironmentEdge(edited.edges, North)
			&& !hasEnvironmentEdge(edited.edges, South)
			&& hasEnvironmentEdge(edited.edges, East)
			&& hasEnvironmentEdge(edited.edges, West)
			&& edited.continueWater,
			"environment controls changed the wrong field or lost an authored neighbor");
		final editedBytes = opened.canonicalDraft();
		final observedPresentationEnvironment = switch opened.query(InspectPresentation) {
			case PresentationObserved(_, value):
				switch value.environment {
					case null: throw "presentation query lost the edited environment";
					case environment: environment;
				}
			case _: throw "presentation query lost the edited environment";
		};
		observedPresentationEnvironment.edges.resize(0);
		final freshPresentationEnvironment = switch opened.query(InspectPresentation) {
			case PresentationObserved(_, value):
				switch value.environment {
					case null: throw "second presentation query lost the edited environment";
					case environment: environment;
				}
			case _: throw "second presentation query lost the edited environment";
		};
		require(hasEnvironmentEdge(freshPresentationEnvironment.edges, North)
			&& hasEnvironmentEdge(freshPresentationEnvironment.edges, East)
			&& hasEnvironmentEdge(freshPresentationEnvironment.edges, West)
			&& opened.canonicalDraft().compare(editedBytes) == 0,
			"mutating presentation environment edges changed the editor draft or the next view");
		final reopened = switch EditorSession.openBytes(editedBytes, new Registry(), defaultEditorSettings()) {
			case EditorOpened(value): value;
			case EditorOpenRejected(error): throw 'editor rejected its environment save: $error';
		};
		require(reopened.canonicalDraft().compare(editedBytes) == 0, "environment save and reload changed canonical bytes");
		final playable = open(defaultEditorSettings());
		expectApplied(playable.apply(SetEnvironment(edited)), DocumentMetadata, "prepare environment Test Play");
		final playableBytes = playable.canonicalDraft();
		requireTestStarted(playable.enterTestPlay(), "environment test play");
		require(playable.leaveTestPlay()
			&& playable.canonicalDraft().compare(playableBytes) == 0, "environment Test Play changed the editor draft");

		expectApplied(opened.apply(SetEnvironment(null)), DocumentMetadata, "remove environment");
		require(opened.draftSnapshot().environment == null, "environment None choice did not restore fallback sky");
		switch opened.mutate({baseRevision: opened.revision(), mutation: Undo}) {
			case MutationApplied(_, _, _, _, _):
			case other:
				throw 'undo environment removal failed: $other';
		}
		require(opened.canonicalDraft().compare(editedBytes) == 0, "undo did not restore exact environment bytes");
		switch opened.mutate({baseRevision: opened.revision(), mutation: Redo}) {
			case MutationApplied(_, _, _, _, _):
			case other:
				throw 'redo environment removal failed: $other';
		}
		require(opened.draftSnapshot().environment == null, "redo did not remove the environment");
	}

	/** Submit one field-preserving environment value through normal history. */
	static function applyEnvironmentEdit(session:EditorSession, control:EditorEnvironmentControl, direction:EditorEnvironmentDirection):Void {
		final environment = editEnvironment(session.draftSnapshot().environment, control, direction);
		expectApplied(session.apply(SetEnvironment(environment)), DocumentMetadata, 'edit environment control $control');
	}

	/** True when one closed horizon edge is present. */
	static function hasEnvironmentEdge(edges:Array<ScenarioHorizonEdge>, expected:ScenarioHorizonEdge):Bool {
		for (edge in edges)
			if (edge == expected)
				return true;
		return false;
	}

	/** Prove every admitted placement role moves through one shared command. */
	static function checkObjectMovement():Void {
		final session = open(defaultEditorSettings());
		expectApplied(session.apply(ResizeWorld({width: 4, height: 3, depth: 4})), WorldShape, "prepare object movement world");
		expectApplied(session.apply(PutDialogue({
			id: DIALOGUE,
			lines: [{speaker: null, text: Message(DIALOGUE_MESSAGE)}]
		})), Dialogue, "prepare moving NPC dialogue");
		final objects:Array<ScenarioObject> = [
			{id: id("move.checkpoint"), tags: [], placement: Checkpoint(transform(500, 500, 500))},
			{id: id("move.item"), tags: [], placement: Item(content("caxecraft:item"), 2, transform(500, 500, 500))},
			{id: id("move.entity"), tags: [], placement: Entity(content("caxecraft:entity"), transform(500, 500, 500))},
			{id: id("move.npc"), tags: [], placement: Npc(NPC, DIALOGUE, transform(500, 500, 500))},
			{id: id("move.prefab"), tags: [], placement: Prefab(PREFAB, transform(500, 500, 500))},
			{id: id("move.trigger"), tags: [], placement: TriggerZone({origin: {x: 0, y: 0, z: 0}, size: {width: 2, height: 2, depth: 2}})},
			{
				id: id("move.stateful"),
				tags: [],
				placement: StatefulObject(content("caxecraft:mechanism"), content("caxecraft:idle"), transform(500, 500, 500))
			}
		];
		for (object in objects)
			expectApplied(session.apply(PutObject(object)), Placement, 'prepare ${object.id.text()}');
		final ids:Array<ScenarioId> = [PLAYER];
		for (object in objects)
			ids.push(object.id);
		for (objectId in ids) {
			roundTrip(session, MoveObjectBy(objectId, {x: 1, y: 0, z: 0}), Placement);
			final moved = projectObjects(session.draftSnapshot().objects);
			var found = false;
			for (gizmo in moved)
				if (gizmo.id.text() == objectId.text()) {
					found = true;
					require(gizmo.x >= 1.0, 'object move did not update ${objectId.text()}');
				}
			require(found, 'object move lost ${objectId.text()}');
		}
		requireMovedObjectPayloads(session.draftSnapshot());

		final beforeRejected = session.canonicalDraft();
		final beforeRevision = session.revision();
		final beforeUndo = session.undoDepth();
		expectRejected(session.apply(MoveObjectBy(PLAYER, {x: -2, y: 0, z: 0})), error -> switch error {
			case ObjectMoveOutsideWorld(id, delta): id.text() == PLAYER.text() && delta.x == -2;
			case _: false;
		}, "out-of-world object move");
		expectRejected(session.apply(MoveObjectBy(id("move.missing"), {x: 1, y: 0, z: 0})), error -> switch error {
			case MissingObject(id): id.text() == "move.missing";
			case _: false;
		}, "missing object move");
		require(session.canonicalDraft().compare(beforeRejected) == 0
			&& session.revision() == beforeRevision
			&& session.undoDepth() == beforeUndo,
			"rejected object movement changed bytes, revision, or history");
	}

	/** Check that movement changed only placement coordinates. */
	static function requireMovedObjectPayloads(scenario:Scenario):Void {
		var payloadChecks = 0;
		for (object in scenario.objects)
			switch object.id.text() {
				case "move.item":
					switch object.placement {
						case Item(itemType, 2, _): require(itemType.text() == "caxecraft:item", "object move changed item type");
						case _: throw "object move changed item placement role or quantity";
					}
					payloadChecks++;
				case "move.npc":
					switch object.placement {
						case Npc(npcType, dialogue, _):
							require(npcType.text() == NPC.text() && dialogue.text() == DIALOGUE.text(), "object move changed NPC links");
						case _: throw "object move changed NPC placement role";
					}
					payloadChecks++;
				case "move.trigger":
					switch object.placement {
						case TriggerZone(bounds): require(bounds.size.width == 2 && bounds.size.height == 2 && bounds.size.depth == 2,
								"object move changed trigger size");
						case _: throw "object move changed trigger placement role";
					}
					payloadChecks++;
				case "move.stateful":
					switch object.placement {
						case StatefulObject(objectType, initialState, _):
							require(objectType.text() == "caxecraft:mechanism" && initialState.text() == "caxecraft:idle",
								"object move changed stateful-object payload");
						case _: throw "object move changed stateful-object placement role";
					}
					payloadChecks++;
				case _:
			}
		require(payloadChecks == 4, "object movement payload proof did not inspect every representative record");
	}

	/** Prove every directional placement rotates while bounds-only volumes fail closed. */
	static function checkObjectRotation():Void {
		final session = open(defaultEditorSettings());
		expectApplied(session.apply(PutDialogue({
			id: DIALOGUE,
			lines: [{speaker: null, text: Message(DIALOGUE_MESSAGE)}]
		})), Dialogue, "prepare rotating NPC dialogue");
		final objects:Array<ScenarioObject> = [
			{id: id("rotate.checkpoint"), tags: [], placement: Checkpoint(transformYaw(500, 500, 500, 350))},
			{id: id("rotate.item"), tags: [], placement: Item(content("caxecraft:item"), 2, transformYaw(500, 500, 500, 350))},
			{id: id("rotate.entity"), tags: [], placement: Entity(content("caxecraft:entity"), transformYaw(500, 500, 500, 350))},
			{id: id("rotate.npc"), tags: [], placement: Npc(NPC, DIALOGUE, transformYaw(500, 500, 500, 350))},
			{id: id("rotate.prefab"), tags: [], placement: Prefab(PREFAB, transformYaw(500, 500, 500, 350))},
			{
				id: id("rotate.stateful"),
				tags: [],
				placement: StatefulObject(content("caxecraft:mechanism"), content("caxecraft:idle"), transformYaw(500, 500, 500, 350))
			}
		];
		for (object in objects)
			expectApplied(session.apply(PutObject(object)), Placement, 'prepare ${object.id.text()}');
		final ids:Array<ScenarioId> = [PLAYER];
		for (object in objects)
			ids.push(object.id);
		for (objectId in ids) {
			roundTrip(session, RotateObjectBy(objectId, 370), Placement);
			expectApplied(session.apply(RotateObjectBy(objectId, -10)), Placement, 'normalize negative yaw for ${objectId.text()}');
		}
		var rotated = 0;
		for (object in session.draftSnapshot().objects)
			for (objectId in ids)
				if (object.id.text() == objectId.text()) {
					switch objectFacing(object) {
						case ObjectYaw(yaw):
							require(yaw == (objectId.text() == PLAYER.text() ? 0 : 350), 'object rotation lost normalized yaw for ${objectId.text()}');
						case NoObjectFacing:
							throw 'object rotation lost facing for ${objectId.text()}';
					}
					rotated++;
				}
		require(rotated == ids.length, "object rotation lost a transform-backed placement");
		requireRotatedObjectPayloads(session.draftSnapshot());

		final triggerId = id("rotate.trigger");
		expectApplied(session.apply(PutObject({
			id: triggerId,
			tags: [new ScenarioTag("volume")],
			placement: TriggerZone({origin: {x: 0, y: 0, z: 0}, size: {width: 2, height: 1, depth: 2}})
		})), Placement, "prepare non-rotatable trigger");
		final beforeRejected = session.canonicalDraft();
		final beforeRevision = session.revision();
		final beforeUndo = session.undoDepth();
		expectRejected(session.apply(RotateObjectBy(triggerId, 90)), error -> switch error {
			case ObjectCannotRotate(id): id.text() == triggerId.text();
			case _: false;
		}, "bounds-only trigger rotation");
		expectRejected(session.apply(RotateObjectBy(id("rotate.missing"), 90)), error -> switch error {
			case MissingObject(id): id.text() == "rotate.missing";
			case _: false;
		}, "missing object rotation");
		require(session.canonicalDraft().compare(beforeRejected) == 0
			&& session.revision() == beforeRevision
			&& session.undoDepth() == beforeUndo,
			"rejected object rotation changed bytes, revision, or history");
	}

	/** Prove trigger resizing preserves identity and rejects every unsafe shape. */
	static function checkTriggerResize():Void {
		final session = open(defaultEditorSettings());
		expectApplied(session.apply(ResizeWorld({width: 4, height: 3, depth: 4})), WorldShape, "prepare trigger resize world");
		final triggerId = id("resize.trigger");
		expectApplied(session.apply(PutObject({
			id: triggerId,
			tags: [new ScenarioTag("volume")],
			placement: TriggerZone({origin: {x: 1, y: 1, z: 1}, size: {width: 1, height: 1, depth: 1}})
		})), Placement, "prepare resizable trigger");
		switch session.select({baseRevision: session.revision(), selection: NodeSelection(ObjectNode(triggerId))}) {
			case SelectionApplied(_, _) | SelectionUnchanged(_, _):
			case other:
				throw 'could not select trigger before resize: $other';
		}

		final beforeResize = session.canonicalDraft();
		final beforeSelection = selectionKey(session);
		switch session.mutate({baseRevision: session.revision(), mutation: Apply(ResizeTriggerTo(triggerId, {width: 2, height: 2, depth: 2}))}) {
			case MutationApplied(families, changes, _, _, _):
				require(families.length == 1 && families[0] == Placement, "trigger resize reported the wrong command family");
				require(changes.length == 1 && isObjectChange(changes[0], triggerId), "trigger resize lost its changed-object identity");
			case other:
				throw 'trigger resize did not commit exactly once: $other';
		}
		final afterResize = session.canonicalDraft();
		require(beforeResize.compare(afterResize) != 0, "trigger resize changed no authored bytes");
		switch session.mutate({baseRevision: session.revision(), mutation: Undo}) {
			case MutationApplied(_, changes, _, _, _):
				require(changes.length == 1 && isObjectChange(changes[0], triggerId), "trigger resize undo lost its changed-object identity");
			case other:
				throw 'trigger resize undo failed: $other';
		}
		require(session.canonicalDraft().compare(beforeResize) == 0 && selectionKey(session) == beforeSelection,
			"trigger resize undo did not restore the exact prior state");
		switch session.mutate({baseRevision: session.revision(), mutation: Redo}) {
			case MutationApplied(_, changes, _, _, _):
				require(changes.length == 1 && isObjectChange(changes[0], triggerId), "trigger resize redo lost its changed-object identity");
			case other:
				throw 'trigger resize redo failed: $other';
		}
		require(session.canonicalDraft().compare(afterResize) == 0 && selectionKey(session) == beforeSelection,
			"trigger resize redo did not restore the exact command state");
		require(expectValid(session, "resized trigger").compare(afterResize) == 0, "trigger resize changed canonical save bytes during validation");
		requireTestStarted(session.enterTestPlay(), "resized trigger Test Play");
		require(session.leaveTestPlay(), "resized trigger Test Play did not return to editing");
		require(session.canonicalDraft().compare(afterResize) == 0, "resized trigger Test Play changed the editor draft");
		var found = false;
		for (object in session.draftSnapshot().objects)
			if (object.id.text() == triggerId.text()) {
				found = true;
				require(object.tags.length == 1 && object.tags[0].text() == "volume", "trigger resize changed tags");
				switch object.placement {
					case TriggerZone(bounds):
						require(bounds.origin.x == 1 && bounds.origin.y == 1 && bounds.origin.z == 1, "trigger resize changed origin");
						require(bounds.size.width == 2 && bounds.size.height == 2 && bounds.size.depth == 2, "trigger resize lost target size");
					case _:
						throw "trigger resize changed placement role";
				}
			}
		require(found, "trigger resize lost stable object identity");

		final beforeRejected = session.canonicalDraft();
		final beforeRevision = session.revision();
		final beforeUndo = session.undoDepth();
		expectRejected(session.apply(ResizeTriggerTo(PLAYER, {width: 1, height: 1, depth: 1})), error -> switch error {
			case ObjectCannotResize(id): id.text() == PLAYER.text();
			case _: false;
		}, "transform-backed trigger resize");
		expectRejected(session.apply(ResizeTriggerTo(id("resize.missing"), {width: 1, height: 1, depth: 1})), error -> switch error {
			case MissingObject(id): id.text() == "resize.missing";
			case _: false;
		}, "missing trigger resize");
		final invalidSize:VoxelSize = {width: 0, height: 1, depth: 1};
		expectRejected(session.apply(ResizeTriggerTo(triggerId, invalidSize)), error -> switch error {
			case InvalidTriggerSize(id, size): id.text() == triggerId.text() && size.width == 0;
			case _: false;
		}, "non-positive trigger resize");
		final outsideSize:VoxelSize = {width: 4, height: 2, depth: 2};
		expectRejected(session.apply(ResizeTriggerTo(triggerId, outsideSize)), error -> switch error {
			case ObjectResizeOutsideWorld(id, size): id.text() == triggerId.text() && size.width == 4;
			case _: false;
		}, "out-of-world trigger resize");
		require(session.canonicalDraft().compare(beforeRejected) == 0
			&& session.revision() == beforeRevision
			&& session.undoDepth() == beforeUndo,
			"rejected trigger resize changed bytes, revision, or history");
		switch session.selectionSnapshot() {
			case NodeSelection(ObjectNode(id)):
				require(id.text() == triggerId.text(), "trigger resize changed shared selection");
			case _:
				throw "trigger resize cleared shared selection";
		}
	}

	/** Return the same closed facing model used by the visible editor marker. */
	static function objectFacing(object:ScenarioObject):EditorObjectFacing {
		return switch object.placement {
			case PlayerSpawn(transform) | Checkpoint(transform) | Item(_, _, transform) | Entity(_, transform) | Npc(_, _, transform) | Prefab(_, transform) |
				StatefulObject(_, _, transform): ObjectYaw(transform.yawDegrees);
			case TriggerZone(_): NoObjectFacing;
		};
	}

	/** Check that rotation changed neither role data nor authored position. */
	static function requireRotatedObjectPayloads(scenario:Scenario):Void {
		var payloadChecks = 0;
		for (object in scenario.objects)
			switch object.id.text() {
				case "rotate.item":
					switch object.placement {
						case Item(itemType, 2, transform):
							require(itemType.text() == "caxecraft:item" && unchangedRotatedPosition(transform), "rotation changed item payload or position");
						case _: throw "rotation changed item role or quantity";
					}
					payloadChecks++;
				case "rotate.npc":
					switch object.placement {
						case Npc(npcType, dialogue, transform):
							require(npcType.text() == NPC.text() && dialogue.text() == DIALOGUE.text() && unchangedRotatedPosition(transform),
								"rotation changed NPC links or position");
						case _: throw "rotation changed NPC role";
					}
					payloadChecks++;
				case "rotate.stateful":
					switch object.placement {
						case StatefulObject(objectType, initialState, transform):
							require(objectType.text() == "caxecraft:mechanism"
								&& initialState.text() == "caxecraft:idle"
								&& unchangedRotatedPosition(transform),
								"rotation changed stateful-object payload or position");
						case _: throw "rotation changed stateful-object role";
					}
					payloadChecks++;
				case _:
			}
		require(payloadChecks == 3, "rotation payload checks did not inspect every linked role");
	}

	/** True when rotation preserved all three authored position coordinates. */
	static inline function unchangedRotatedPosition(transform:caxecraft.scenario.ScenarioGeometry.ScenarioTransform):Bool
		return transform.xMilli == 500 && transform.yMilli == 500 && transform.zMilli == 500;

	/**
	 * Prove that validated map bytes and every CAXEMAP object role remain visible.
	 *
	 * The expected values are authored independently from the projection code.
	 * They protect the generic editor seam without naming a shipped campaign.
	 */
	static function checkActiveLevelProjection():Int {
		final source = ScenarioWriter.write(baseScenario());
		final opened = switch EditorSession.openBytes(source, new Registry(), defaultEditorSettings()) {
			case EditorOpened(value): value;
			case EditorOpenRejected(error): throw 'editor did not open validated CAXEMAP bytes: $error';
		};
		require(opened.draftSnapshot().id.text() == "editor.qa", "byte-open changed the authored map identity");
		source.set(0, 0);
		require(opened.canonicalDraft().get(0) == "C".code, "byte-open retained a mutable caller-owned source alias");

		final objects = projectObjects([
			{id: id("gizmo.spawn"), tags: [], placement: PlayerSpawn(transform(500, 0, 500))},
			{id: id("gizmo.checkpoint"), tags: [], placement: Checkpoint(transform(1500, 0, 500))},
			{id: id("gizmo.item"), tags: [], placement: Item(content("caxecraft:item"), 2, transform(2500, 0, 500))},
			{id: id("gizmo.entity"), tags: [], placement: Entity(content("caxecraft:entity"), transform(3500, 0, 500))},
			{id: id("gizmo.npc"), tags: [], placement: Npc(NPC, DIALOGUE, transform(500, 0, 1500))},
			{id: id("gizmo.prefab"), tags: [], placement: Prefab(PREFAB, transform(1500, 0, 1500))},
			{
				id: id("gizmo.trigger"),
				tags: [],
				placement: TriggerZone({origin: {x: 2, y: 1, z: 1}, size: {width: 3, height: 2, depth: 4}})
			},
			{
				id: id("gizmo.stateful"),
				tags: [],
				placement: StatefulObject(content("caxecraft:door"), content("caxecraft:closed"), transform(5500, 0, 500))
			}
		]);
		final kinds = [
			EditorObjectGizmoKind.PlayerSpawnGizmo,
			EditorObjectGizmoKind.CheckpointGizmo,
			EditorObjectGizmoKind.ItemGizmo,
			EditorObjectGizmoKind.EntityGizmo,
			EditorObjectGizmoKind.NpcGizmo,
			EditorObjectGizmoKind.PrefabGizmo,
			EditorObjectGizmoKind.TriggerZoneGizmo,
			EditorObjectGizmoKind.StatefulObjectGizmo
		];
		require(objects.length == kinds.length, "object projection omitted an admitted placement role");
		for (index in 0...kinds.length) {
			require(objects[index].kind == kinds[index], 'object projection changed role $index');
			require(objects[index].id.text() == [
				"gizmo.spawn",
				"gizmo.checkpoint",
				"gizmo.item",
				"gizmo.entity",
				"gizmo.npc",
				"gizmo.prefab",
				"gizmo.trigger",
				"gizmo.stateful"
			][index], 'object projection changed identity $index');
		}
		require(close(objects[0].x, 0.5) && close(objects[0].y, 0.5) && close(objects[0].z, 0.5),
			"point-object projection changed authored thousandth-block coordinates");
		require(close(objects[6].x, 3.5) && close(objects[6].y, 2.0) && close(objects[6].z, 3.0) && close(objects[6].width, 3.0)
			&& close(objects[6].height, 2.0) && close(objects[6].depth, 4.0),
			"trigger projection changed its exact half-open authored bounds");
		return 13;
	}

	static function checkActionPalette():Void {
		final descriptors = availableScenarioActions();
		final expected = [
			"dialogue",
			"journal",
			"set-flag",
			"set-counter",
			"add-counter",
			"set-state",
			"give-item",
			"take-item",
			"spawn",
			"despawn",
			"set-object-state",
			"checkpoint",
			"objective",
			"effect",
			"campaign-exit",
			"signal",
			"schedule",
			"call",
			"choose"
		];
		require(descriptors.length == expected.length, "editor action palette lost constructor coverage");
		for (index in 0...expected.length) {
			final descriptor = descriptors[index];
			require(descriptor.id.text() == expected[index], "editor action palette order drifted");
			require(descriptor.editorLabel.text() == 'editor.action.${expected[index]}.label', "editor action label key drifted");
			require(descriptor.editorHelp.text() == 'editor.action.${expected[index]}.help', "editor action help key drifted");
			require(flowActionArgumentRoles(descriptor.schema).length > 0, "editor action lost its typed form fields");
		}
	}

	/**
	 * Prove every editor control is reachable in both semantic directions.
	 *
	 * This test deliberately knows nothing about keyboard keys, controllers, or
	 * screen coordinates. Device adapters may change independently, while this
	 * closed order remains the shared accessibility contract.
	 */
	static function checkFocusNavigation():Int {
		final forward:Array<EditorFocusTarget> = [
			EditorFocusTarget.Save,
			EditorFocusTarget.WorldName,
			EditorFocusTarget.Undo,
			EditorFocusTarget.Redo,
			EditorFocusTarget.Build,
			EditorFocusTarget.Plan,
			EditorFocusTarget.CameraMode,
			EditorFocusTarget.PreviousLayer,
			EditorFocusTarget.NextLayer,
			EditorFocusTarget.Environment,
			EditorFocusTarget.Play,
			EditorFocusTarget.SelectTool,
			EditorFocusTarget.GroundTool,
			EditorFocusTarget.EraseTool,
			EditorFocusTarget.CheckpointTool,
			EditorFocusTarget.CatalogObjectTool,
			EditorFocusTarget.TriggerZoneTool,
			EditorFocusTarget.MoreDetails,
			EditorFocusTarget.WorldList,
			EditorFocusTarget.Back
		];
		final backward:Array<EditorFocusTarget> = [
			EditorFocusTarget.WorldList,
			EditorFocusTarget.MoreDetails,
			EditorFocusTarget.TriggerZoneTool,
			EditorFocusTarget.CatalogObjectTool,
			EditorFocusTarget.CheckpointTool,
			EditorFocusTarget.EraseTool,
			EditorFocusTarget.GroundTool,
			EditorFocusTarget.SelectTool,
			EditorFocusTarget.Play,
			EditorFocusTarget.Environment,
			EditorFocusTarget.NextLayer,
			EditorFocusTarget.PreviousLayer,
			EditorFocusTarget.CameraMode,
			EditorFocusTarget.Plan,
			EditorFocusTarget.Build,
			EditorFocusTarget.Redo,
			EditorFocusTarget.Undo,
			EditorFocusTarget.WorldName,
			EditorFocusTarget.Save,
			EditorFocusTarget.Back
		];
		var checks = 1;
		var focus = initialFocus();
		require(focus == EditorFocusTarget.Back, "editor focus did not start on the visible Back action");
		for (expected in forward) {
			focus = moveFocus(focus, EditorFocusMove.Forward);
			require(focus == expected, "forward editor focus order drifted");
			checks++;
		}
		for (expected in backward) {
			focus = moveFocus(focus, EditorFocusMove.Backward);
			require(focus == expected, "backward editor focus order drifted");
			checks++;
		}
		require(moveFocus(EditorFocusTarget.KeepEditing, EditorFocusMove.Forward) == EditorFocusTarget.LeaveWithoutSaving,
			"leave prompt did not reach its destructive choice");
		require(moveFocus(EditorFocusTarget.LeaveWithoutSaving, EditorFocusMove.Forward) == EditorFocusTarget.KeepEditing,
			"leave prompt did not wrap to its safe choice");
		require(moveFocus(EditorFocusTarget.KeepEditing, EditorFocusMove.Backward) == EditorFocusTarget.LeaveWithoutSaving,
			"reverse leave-prompt focus did not reach its destructive choice");
		require(moveFocus(EditorFocusTarget.LeaveWithoutSaving, EditorFocusMove.Backward) == EditorFocusTarget.KeepEditing,
			"reverse leave-prompt focus did not wrap to its safe choice");
		checks += 4;
		return checks;
	}

	/**
	 * Prove device-neutral dead-zone, repeat, edge, and disconnect behavior.
	 *
	 * These deterministic snapshots exercise the same `NavigationRepeater`
	 * instance used by the native Raylib editor. Eval owns the interaction rule;
	 * the graphical pilot separately proves that a resulting command reaches the
	 * rendered focus ring and production editor action.
	 */
	static function checkNavigationInput():Int {
		final repeater = new NavigationRepeater();
		var checks = 0;
		require(repeater.advance(navigation(false), 0.0) == NavigationCommand.None, "disconnected navigation produced an action");
		checks++;
		require(repeater.advance(navigation(true, 0.34, -0.34), 0.0) == NavigationCommand.None, "stick noise inside the dead zone moved focus");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8), 0.0) == NavigationCommand.Down, "new analog direction did not move immediately");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8), 0.34) == NavigationCommand.None, "held direction repeated before its initial delay");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8), 0.01) == NavigationCommand.Down, "held direction did not repeat at its initial delay");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8), 0.09) == NavigationCommand.None, "held direction repeated before its steady interval");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8), 0.01) == NavigationCommand.Down, "held direction did not repeat at its steady interval");
		checks++;
		require(repeater.advance(navigation(true), 0.0) == NavigationCommand.None, "released direction produced an action");
		checks++;
		require(repeater.advance(navigation(true, -0.8), 0.0) == NavigationCommand.Left, "new direction after release did not move immediately");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.0, true, false, true, false), 0.0) == NavigationCommand.None,
			"opposite vertical directions did not cancel");
		checks++;
		require(repeater.advance(navigation(true, 0.8, 0.8), 0.0) == NavigationCommand.Down, "diagonal input did not use deterministic vertical priority");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8, false, false, false, false, true), 0.0) == NavigationCommand.Confirm,
			"confirm edge did not take priority over held movement");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8, false, false, false, false, false, true), 0.0) == NavigationCommand.Cancel,
			"cancel edge did not take priority over held movement");
		checks++;
		require(repeater.advance(navigation(false), 0.0) == NavigationCommand.None, "disconnect did not suppress input");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8), 0.0) == NavigationCommand.Down, "reconnect inherited stale repeat state");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8), 2.0) == NavigationCommand.Down, "stalled frame lost its bounded repeat");
		checks++;
		require(repeater.advance(navigation(true, 0.0, 0.8), 0.0) == NavigationCommand.None, "stalled frame queued a burst of repeat actions");
		checks++;
		repeater.release();
		require(repeater.advance(navigation(true, 0.0, 0.8), 0.0) == NavigationCommand.Down, "screen release did not clear the repeat clock");
		checks++;
		return checks;
	}

	/** Prove that direct Build capture and hotbar input stay finite and explicit. */
	static function checkBuildControls():Int {
		var checks = 0;
		var pointer = EditorBuildPointerState.Released;
		pointer = nextPointerState(pointer, true, true, true, false);
		require(pointer == EditorBuildPointerState.Captured, "a focused Build click did not capture the pointer");
		checks++;
		pointer = nextPointerState(pointer, true, true, false, false);
		require(pointer == EditorBuildPointerState.Captured, "steady Build input released the pointer");
		checks++;
		pointer = nextPointerState(pointer, true, true, false, true);
		require(pointer == EditorBuildPointerState.Released, "cancel did not release Build before leaving the editor");
		checks++;
		pointer = nextPointerState(EditorBuildPointerState.Captured, false, true, false, false);
		require(pointer == EditorBuildPointerState.Released, "Plan retained first-person pointer capture");
		checks++;
		pointer = nextPointerState(EditorBuildPointerState.Captured, true, false, false, false);
		require(pointer == EditorBuildPointerState.Released, "window focus loss retained first-person pointer capture");
		checks++;
		final solidHit:EditorWorldHit = {
			point: {x: 3, y: 2, z: 1},
			placement: {x: 3, y: 3, z: 1},
			distance: 4.0,
			solid: true
		};
		switch terrainAction(true, false, solidHit) {
			case RemoveTerrain(point):
				require(point.x == 3 && point.y == 2 && point.z == 1, "Build primary action did not remove the solid target");
			case NoTerrainAction | PlaceTerrain(_):
				require(false, "Build primary action did not resolve to terrain removal");
		}
		checks++;
		switch terrainAction(false, true, solidHit) {
			case PlaceTerrain(point):
				require(point.x == 3 && point.y == 3 && point.z == 1, "Build secondary action did not use the adjacent empty cell");
			case NoTerrainAction | RemoveTerrain(_):
				require(false, "Build secondary action did not resolve to terrain placement");
		}
		checks++;
		switch terrainAction(true, true, solidHit) {
			case RemoveTerrain(_):
			case NoTerrainAction | PlaceTerrain(_):
				require(false, "Build did not give primary removal deterministic priority");
		}
		checks++;
		final emptyHit:EditorWorldHit = {
			point: {x: 2, y: 0, z: 2},
			placement: {x: 2, y: 0, z: 2},
			distance: 3.0,
			solid: false
		};
		switch terrainAction(true, false, emptyHit) {
			case NoTerrainAction:
			case RemoveTerrain(_) | PlaceTerrain(_):
				require(false, "Build primary action tried to remove an empty cell");
		}
		checks++;
		switch terrainAction(false, true, emptyHit) {
			case PlaceTerrain(point):
				require(point.x == 2 && point.y == 0 && point.z == 2, "Build could not place in an empty world");
			case NoTerrainAction | RemoveTerrain(_):
				require(false, "Build rejected the empty-floor placement target");
		}
		checks++;
		final blockedHit:EditorWorldHit = {
			point: {x: 0, y: 1, z: 0},
			placement: null,
			distance: 0.0,
			solid: true
		};
		switch terrainAction(false, true, blockedHit) {
			case NoTerrainAction:
			case RemoveTerrain(_) | PlaceTerrain(_):
				require(false, "Build placed outside the finite world");
		}
		checks++;
		switch terrainAction(false, false, solidHit) {
			case NoTerrainAction:
			case RemoveTerrain(_) | PlaceTerrain(_):
				require(false, "Build edited terrain without a mouse-button edge");
		}
		checks++;
		switch terrainAction(true, true, null) {
			case NoTerrainAction:
			case RemoveTerrain(_) | PlaceTerrain(_):
				require(false, "Build edited terrain without a world target");
		}
		checks++;
		require(usesDirectTerrainControls(SelectTool)
			&& usesDirectTerrainControls(PaintTool)
			&& usesDirectTerrainControls(EraseTool)
			&& !usesDirectTerrainControls(FillTool)
			&& !usesDirectTerrainControls(CheckpointTool)
			&& !usesDirectTerrainControls(CatalogObjectTool)
			&& !usesDirectTerrainControls(TriggerZoneTool),
			"Build direct terrain controls leaked into object or volume placement");
		checks++;
		require(toolForBuildHotbarSlot(1) == SelectTool
			&& toolForBuildHotbarSlot(2) == PaintTool
			&& toolForBuildHotbarSlot(3) == CheckpointTool
			&& toolForBuildHotbarSlot(4) == CatalogObjectTool
			&& toolForBuildHotbarSlot(5) == TriggerZoneTool
			&& toolForBuildHotbarSlot(0) == null
			&& toolForBuildHotbarSlot(6) == null,
			"Build hotbar slots drifted from the five visible creation cards");
		checks++;
		require(normalizeBuildTool(EraseTool) == PaintTool
			&& normalizeBuildTool(SelectTool) == SelectTool, "Build retained Plan's hidden Erase mode");
		checks++;
		require(normalizeBuildFocus(EditorFocusTarget.EraseTool) == EditorFocusTarget.GroundTool
			&& normalizeBuildFocus(EditorFocusTarget.CheckpointTool) == EditorFocusTarget.CheckpointTool,
			"Build retained focus on Plan's hidden Erase card");
		checks++;
		require(moveBuildFocus(EditorFocusTarget.GroundTool, EditorFocusMove.Forward) == EditorFocusTarget.CheckpointTool
			&& moveBuildFocus(EditorFocusTarget.CheckpointTool, EditorFocusMove.Backward) == EditorFocusTarget.GroundTool,
			"Build focus navigation visited Plan's hidden Erase card");
		checks++;
		return checks;
	}

	/** Build one complete normalized input snapshot with concise test defaults. */
	static function navigation(connected:Bool, horizontal:Float = 0.0, vertical:Float = 0.0, up:Bool = false, right:Bool = false, down:Bool = false,
			left:Bool = false, confirmPressed:Bool = false, cancelPressed:Bool = false):NavigationSample
		return {
			connected: connected,
			up: up,
			right: right,
			down: down,
			left: left,
			confirmPressed: confirmPressed,
			cancelPressed: cancelPressed,
			horizontal: horizontal,
			vertical: vertical
		};

	/**
	 * Prove stale-edit rejection, atomic batches, and copy-owned observations.
	 *
	 * This is the transport-independent contract a visual editor, local JSONL
	 * process, or later MCP adapter will share. A batch deliberately stages one
	 * valid edit before a failing edit below; the unchanged live bytes prove
	 * that staging never exposes a partial result.
	 */
	static function checkRevisionedProtocol():Int {
		final session = open(defaultEditorSettings());
		final initialState = switch session.query(InspectState) {
			case StateObserved(value): value;
			case _: throw "state query returned the wrong observation";
		};
		require(initialState.revision == 0 && initialState.editing && initialState.undoDepth == 0 && initialState.redoDepth == 0,
			"new editor protocol state was not revision zero");
		switch session.query(InspectValidation) {
			case ValidationObserved(0, DraftPlayable(bytes)):
				require(bytes.length > 0, "playable validation observation lost its canonical bytes");
			case _:
				throw "initial valid draft did not produce a playable validation observation";
		}

		switch session.mutate({
			baseRevision: initialState.revision,
			mutation: Apply(ResizeWorld({width: 2, height: 1, depth: 2}))
		}) {
			case MutationApplied(families, changes, 1, 1, 0):
				require(families.length == 1 && families[0] == WorldShape, "single mutation lost its command family");
				require(changes.length == 1 && isWorldShapeChange(changes[0]), "single mutation lost its changed world identity");
			case _:
				throw "revisioned single mutation did not commit exactly once";
		}
		final afterResize = session.canonicalDraft();
		switch session.mutate({baseRevision: 0, mutation: Apply(SetPaletteEntry(1, STONE))}) {
			case MutationRejected(RevisionConflict(1, 0), 1):
			case _:
				throw "stale editor request did not report required and supplied revisions";
		}
		require(session.canonicalDraft().compare(afterResize) == 0
			&& session.undoDepth() == 1, "stale editor request changed draft or history");

		final selection:VoxelBounds = {origin: {x: 0, y: 0, z: 0}, size: {width: 1, height: 1, depth: 1}};
		final beforeSelectionBytes = session.canonicalDraft();
		final beforeSelectionUndo = session.undoDepth();
		switch session.select({baseRevision: 1, selection: VoxelSelection(selection)}) {
			case SelectionApplied(VoxelSelection(_), 1):
			case _:
				throw "workspace selection did not apply at the observed revision";
		}
		require(session.revision() == 1
			&& session.undoDepth() == beforeSelectionUndo
			&& session.canonicalDraft().compare(beforeSelectionBytes) == 0,
			"workspace selection changed document bytes, revision, or history");

		final batchCommands = [SetPaletteEntry(1, STONE), PaintVoxel({x: 0, y: 0, z: 0}, 1)];
		switch session.preview({baseRevision: 1, commands: batchCommands}) {
			case PreviewAccepted(families, changes, 1):
				require(families.length == 2 && families[0] == Voxel && families[1] == Voxel, "preview lost its ordered command families");
				require(changes.length == 2 && isPaletteChange(changes[0], 1) && isTerrainChange(changes[1]), "preview lost its semantic change identities");
			case _:
				throw "valid editor preview was not accepted";
		}
		require(session.revision() == 1
			&& session.undoDepth() == beforeSelectionUndo
			&& session.canonicalDraft().compare(beforeSelectionBytes) == 0,
			"preview changed live editor state");
		switch session.mutate({
			baseRevision: 1,
			mutation: ApplyBatch(batchCommands)
		}) {
			case MutationApplied(families, changes, 2, 2, 0):
				require(families.length == 2 && families[0] == Voxel && families[1] == Voxel, "atomic mutation lost its ordered command families");
				require(changes.length == 2 && isPaletteChange(changes[0], 1) && isTerrainChange(changes[1]),
					"atomic mutation did not deduplicate changed semantic identities in command order");
			case _:
				throw "atomic editor mutation did not commit as one revision";
		}
		final afterBatch = session.canonicalDraft();
		require(session.historyEntries() == 2, "two-command transaction created more than one history entry");
		switch session.select({baseRevision: 2, selection: VoxelSelection(selection)}) {
			case SelectionUnchanged(VoxelSelection(_), 2):
			case _:
				throw "unchanged workspace selection did not preserve the revision";
		}
		final movedSelection:VoxelBounds = {origin: {x: 1, y: 0, z: 1}, size: {width: 1, height: 1, depth: 1}};
		expectSelection(session, VoxelSelection(movedSelection), "move workspace selection after edit");
		final selectionBeforeHistory = selectionKey(session);
		switch session.select({baseRevision: 1, selection: NoEditorSelection}) {
			case SelectionRejected(RevisionConflict(2, 1), 2):
			case _:
				throw "stale workspace selection did not fail closed";
		}
		switch session.preview({baseRevision: 2, commands: [PaintVoxel({x: 99, y: 0, z: 0}, 1)]}) {
			case PreviewRejected(PointOutsideWorld(_), 2):
			case _:
				throw "invalid editor preview returned the wrong rejection";
		}

		final rollbackBytes = session.canonicalDraft();
		final rollbackSelection = selectionKey(session);
		final rollbackUndo = session.undoDepth();
		final rollbackRedo = session.redoDepth();
		switch session.mutate({
			baseRevision: 2,
			mutation: ApplyBatch([PaintVoxel({x: 1, y: 0, z: 0}, 1), PaintVoxel({x: 99, y: 0, z: 0}, 1)])
		}) {
			case MutationRejected(PointOutsideWorld(_), 2):
			case _:
				throw "failing atomic editor mutation returned the wrong rejection";
		}
		require(session.canonicalDraft().compare(rollbackBytes) == 0
			&& selectionKey(session) == rollbackSelection
			&& session.undoDepth() == rollbackUndo
			&& session.redoDepth() == rollbackRedo
			&& session.revision() == 2,
			"failing atomic mutation leaked staged state");

		switch session.mutate({baseRevision: 2, mutation: ApplyBatch([])}) {
			case MutationRejected(EmptyTransaction, 2):
			case _:
				throw "empty editor transaction did not fail closed";
		}
		final oversized:Array<EditorCommand> = [];
		for (_ in 0...defaultEditorSettings().transactionCommands + 1)
			oversized.push(EraseVoxel({x: 0, y: 0, z: 0}));
		switch session.mutate({baseRevision: 2, mutation: ApplyBatch(oversized)}) {
			case MutationRejected(TransactionTooLarge(129, 128), 2):
			case _:
				throw "oversized editor transaction did not report its exact bound";
		}

		switch session.mutate({baseRevision: 2, mutation: Undo}) {
			case MutationApplied(families, changes, 3, 1, 1):
				require(families.length == 1 && families[0] == Transaction, "transaction undo lost its history family");
				require(changes.length == 2, "transaction undo lost the stored changed identities");
			case _:
				throw "transaction undo did not advance one revision";
		}
		require(session.canonicalDraft().compare(afterResize) == 0, "transaction undo restored a partial batch");
		require(selectionKey(session) == selectionBeforeHistory, "document undo rewound workspace selection");
		switch session.mutate({baseRevision: 3, mutation: Redo}) {
			case MutationApplied(families, changes, 4, 2, 0):
				require(families.length == 1 && families[0] == Transaction, "transaction redo lost its history family");
				require(changes.length == 2, "transaction redo lost the stored changed identities");
			case _:
				throw "transaction redo did not advance one revision";
		}
		require(session.canonicalDraft().compare(afterBatch) == 0, "transaction redo did not restore the complete batch");
		require(selectionKey(session) == selectionBeforeHistory, "document redo rewound workspace selection");

		final observedBytes = switch session.query(InspectCanonicalDraft) {
			case CanonicalDraftObserved(4, value): value;
			case _: throw "canonical query lost its revision";
		};
		final originalByte = observedBytes.get(0);
		observedBytes.set(0, originalByte == 0 ? 1 : 0);
		final freshBytes = switch session.query(InspectCanonicalDraft) {
			case CanonicalDraftObserved(4, value): value;
			case _: throw "second canonical query lost its revision";
		};
		require(freshBytes.compare(afterBatch) == 0, "mutating observed bytes changed the editor draft");

		final observedDraft = switch session.query(InspectDraft) {
			case DraftObserved(4, value): value;
			case _: throw "draft query lost its revision";
		};
		final objectCount = observedDraft.objects.length;
		observedDraft.objects.resize(0);
		final freshDraft = switch session.query(InspectDraft) {
			case DraftObserved(4, value): value;
			case _: throw "second draft query lost its revision";
		};
		require(objectCount > 0 && freshDraft.objects.length == objectCount, "mutating an observed scenario changed the editor draft");

		final observedPresentation = switch session.query(InspectPresentation) {
			case PresentationObserved(4, value): value;
			case _: throw "presentation query lost its revision";
		};
		final presentationObjectCount = observedPresentation.objects.length;
		final presentationPaletteCount = observedPresentation.world.palette.length;
		final presentationRuleCount = observedPresentation.ruleIds.length;
		final observedProjection = switch observedPresentation.projection {
			case null: throw "presentation query lost the finite world";
			case value: value;
		};
		require(observedProjection.cells.length > 0, "presentation query returned an empty finite world");
		final firstCell = observedProjection.cells[0];
		observedProjection.cells[0] = firstCell == 0 ? 1 : 0;
		observedPresentation.world.palette.resize(0);
		observedPresentation.objects[0].tags.push(new ScenarioTag("caller-owned"));
		observedPresentation.objects.resize(0);
		observedPresentation.ruleIds.resize(0);
		final freshPresentation = switch session.query(InspectPresentation) {
			case PresentationObserved(4, value): value;
			case _: throw "second presentation query lost its revision";
		};
		require(freshPresentation.objects.length == presentationObjectCount
			&& freshPresentation.world.palette.length == presentationPaletteCount
			&& freshPresentation.ruleIds.length == presentationRuleCount
			&& freshPresentation.objects[0].tags.length == 0
			&& freshPresentation.projection != null
			&& freshPresentation.projection.cells[0] == firstCell
			&& session.canonicalDraft().compare(afterBatch) == 0,
			"mutating a presentation observation changed the editor draft or the next view");

		final tree = switch session.query(InspectTree) {
			case TreeObserved(4, nodes): nodes;
			case _: throw "campaign-tree query lost its revision";
		};
		final currentDraft = session.draftSnapshot();
		require(tree.length == expectedTreeNodes(currentDraft), "campaign-tree projection omitted or duplicated authored records");
		require(hasTreeRoot(tree, currentDraft.id) && hasPaletteNode(tree, 1), "campaign-tree projection lost its root hierarchy or edited palette entry");
		tree.resize(0);
		final freshTree = switch session.query(InspectTree) {
			case TreeObserved(4, nodes): nodes;
			case _: throw "second campaign-tree query lost its revision";
		};
		require(freshTree.length == expectedTreeNodes(currentDraft), "mutating an observed campaign tree changed the editor session");

		final objectId = id("tree.agent-marker");
		switch session.mutate({
			baseRevision: 4,
			mutation: ApplyBatch([
				PutObject({id: objectId, tags: [], placement: Checkpoint(transform(1000, 0, 1000))}),
				PutObject({id: objectId, tags: [new ScenarioTag("updated")], placement: Checkpoint(transform(2000, 0, 1000))})
			])
		}) {
			case MutationApplied(_, changes, 5, _, _):
				require(changes.length == 1 && isObjectChange(changes[0], objectId), "replacement batch did not deduplicate its stable object identity");
			case _:
				throw "replacement batch did not commit";
		}
		final addedTree = switch session.query(InspectTree) {
			case TreeObserved(5, nodes): nodes;
			case _: throw "tree after object creation lost its revision";
		};
		require(hasObjectNode(addedTree, objectId), "campaign tree did not expose a newly authored object");
		switch session.query(InspectNode(ObjectNode(objectId))) {
			case NodeObserved(5, node):
				require(node != null && node.childCount == 0, "object property query returned the wrong compact tree row");
			case _:
				throw "object property query lost its revision";
		}
		expectSelection(session, NodeSelection(ObjectNode(objectId)), "select authored object by stable identity");
		switch session.query(InspectNode(ObjectNode(id("missing.tree-object")))) {
			case NodeObserved(5, null):
			case _:
				throw "missing object property query did not return an explicit empty result";
		}
		expectSelectionRejected(session.select({
			baseRevision: session.revision(),
			selection: NodeSelection(ObjectNode(id("missing.tree-object")))
		}), error -> switch error {
			case MissingEditorNode(ObjectNode(_)): true;
			case _: false;
		}, "select missing authored object");
		switch session.mutate({baseRevision: 5, mutation: Apply(RemoveObject(objectId))}) {
			case MutationApplied(_, changes, 6, _, _):
				require(changes.length == 1 && isObjectChange(changes[0], objectId), "object deletion lost its stable identity");
			case _:
				throw "object deletion did not commit";
		}
		require(selectionKey(session) == "none", "deleting a selected object left a stale workspace target");
		switch session.mutate({baseRevision: 6, mutation: Undo}) {
			case MutationApplied(_, changes, 7, _, _):
				require(changes.length == 1 && isObjectChange(changes[0], objectId), "deletion undo lost its stored object identity");
			case _:
				throw "object deletion undo did not commit";
		}
		require(hasObjectNode(switch session.query(InspectTree) {
			case TreeObserved(7, nodes): nodes;
			case _: throw "tree after deletion undo lost its revision";
		}, objectId), "undo did not restore the authored object to the campaign tree");
		require(selectionKey(session) == "none", "document undo resurrected workspace selection from history");
		final invalid = open(defaultEditorSettings());
		expectApplied(invalid.apply(RemoveMessage(EN, OBJECTIVE_BODY_MESSAGE)), Localization, "prepare invalid validation observation");
		final diagnostics = switch invalid.query(InspectValidation) {
			case ValidationObserved(1, DraftInvalid(values)): values;
			case _:
				throw "invalid draft did not expose validation diagnostics";
		};
		final diagnosticCount = diagnostics.length;
		require(diagnosticCount > 0, "invalid validation observation contained no diagnostics");
		diagnostics.resize(0);
		switch invalid.query(InspectValidation) {
			case ValidationObserved(1, DraftInvalid(values)):
				require(values.length == diagnosticCount, "mutating observed diagnostics changed later validation state");
			case _:
				throw "second invalid validation observation changed shape";
		}
		return 42;
	}

	/**
	 * Prove a visible title edit is canonical, reversible, and revision-safe.
	 *
	 * The native text box owns only temporary bytes. This renderer-free check
	 * exercises the `EditorSession` command it confirms, including the exact
	 * change identity later used by visual and automation clients.
	 */
	static function checkTitleProtocol():Int {
		final session = open(defaultEditorSettings());
		final before = session.canonicalDraft();
		switch session.mutate({baseRevision: 0, mutation: Apply(SetTitle(Literal("Bosque de Ivvy")))}) {
			case MutationApplied(families, changes, 1, 1, 0):
				require(families.length == 1 && families[0] == DocumentMetadata, "title mutation lost its document-metadata family");
				require(changes.length == 1 && isTitleChange(changes[0]), "title mutation lost its stable changed-title identity");
			case _:
				throw "title mutation did not commit exactly once";
		}
		final renamed = session.canonicalDraft();
		require(before.compare(renamed) != 0 && hasLiteralTitle(session.draftSnapshot(), "Bosque de Ivvy"),
			"title mutation did not reach canonical CAXEMAP state");
		switch session.mutate({baseRevision: 0, mutation: Apply(SetTitle(Literal("stale")))}) {
			case MutationRejected(RevisionConflict(1, 0), 1):
			case _:
				throw "stale title mutation did not fail before changing the draft";
		}
		switch session.mutate({baseRevision: 1, mutation: Apply(SetTitle(Literal("")))}) {
			case MutationRejected(InvalidTitle, 1):
			case _:
				throw "empty title mutation did not fail closed";
		}
		require(session.canonicalDraft().compare(renamed) == 0
			&& session.undoDepth() == 1, "rejected title input changed canonical state or history");
		switch session.mutate({baseRevision: 1, mutation: Undo}) {
			case MutationApplied(families, changes, 2, 0, 1):
				require(families.length == 1 && families[0] == DocumentMetadata && changes.length == 1 && isTitleChange(changes[0]),
					"title undo lost its family or changed identity");
			case _:
				throw "title undo did not restore the original draft";
		}
		require(session.canonicalDraft().compare(before) == 0, "title undo changed unrelated canonical bytes");
		switch session.mutate({baseRevision: 2, mutation: Redo}) {
			case MutationApplied(families, changes, 3, 1, 0):
				require(families.length == 1 && families[0] == DocumentMetadata && changes.length == 1 && isTitleChange(changes[0]),
					"title redo lost its family or changed identity");
			case _:
				throw "title redo did not restore the accepted title";
		}
		require(session.canonicalDraft().compare(renamed) == 0, "title redo did not restore exact canonical bytes");
		return 8;
	}

	static function expectedTreeNodes(scenario:Scenario):Int {
		var localeRecords = 0;
		switch scenario.messages {
			case NoMessageCatalog:
			case EmbeddedMessageCatalog(catalog):
				for (locale in catalog.locales)
					localeRecords += 1 + locale.messages.length;
		}
		return 1 + 6 + 3 + scenario.world.palette.length + scenario.world.chunks.length + scenario.world.fluids.length + scenario.objects.length + 4
			+ scenario.story.dialogues.length + scenario.story.journal.length + scenario.story.objectives.length + scenario.story.routes.length + 3
			+ scenario.flow.variables.length + scenario.flow.sequences.length + scenario.flow.rules.length + localeRecords + scenario.extensions.length;
	}

	static function hasTreeRoot(nodes:Array<caxecraft.editor.EditorTypes.EditorTreeNode>, id:ScenarioId):Bool {
		for (node in nodes)
			switch node.ref {
				case ScenarioNode(actual):
					if (actual.text() == id.text() && node.parent == null && node.childCount == 6)
						return true;
				case _:
			}
		return false;
	}

	static function hasPaletteNode(nodes:Array<caxecraft.editor.EditorTypes.EditorTreeNode>, code:Int):Bool {
		for (node in nodes)
			switch node.ref {
				case PaletteNode(actual):
					if (actual == code)
						return switch node.parent {
							case SectionNode(Palette): node.childCount == 0;
							case _: false;
						};
				case _:
			}
		return false;
	}

	static function hasObjectNode(nodes:Array<caxecraft.editor.EditorTypes.EditorTreeNode>, id:ScenarioId):Bool {
		for (node in nodes)
			switch node.ref {
				case ObjectNode(actual):
					if (actual.text() == id.text())
						return switch node.parent {
							case SectionNode(Objects): node.childCount == 0;
							case _: false;
						};
				case _:
			}
		return false;
	}

	static function isWorldShapeChange(value:EditorChangeId):Bool
		return switch value {
			case ChangedWorldShape: true;
			case _: false;
		};

	static function isTitleChange(value:EditorChangeId):Bool
		return switch value {
			case ChangedTitle: true;
			case _: false;
		};

	static function hasLiteralTitle(scenario:Scenario, expected:String):Bool
		return switch scenario.title {
			case Literal(value): value == expected;
			case Message(_): false;
		};

	static function isPaletteChange(value:EditorChangeId, code:Int):Bool
		return switch value {
			case ChangedPalette(actual): actual == code;
			case _: false;
		};

	static function isTerrainChange(value:EditorChangeId):Bool
		return switch value {
			case ChangedTerrain: true;
			case _: false;
		};

	static function isObjectChange(value:EditorChangeId, id:ScenarioId):Bool
		return switch value {
			case ChangedObject(actual): actual.text() == id.text();
			case _: false;
		};

	/**
	 * Prove one cached layer, its pixel mapping, and all four visual tools.
	 *
	 * These checks run without Raylib. The native editor consumes the same
	 * projection and command functions, so a changed grid edge or tool index is
	 * caught before a graphical pilot has to diagnose it from pixels.
	 */
	static function checkViewport():Int {
		final session = open(defaultEditorSettings());
		expectApplied(session.apply(ResizeWorld({width: 4, height: 2, depth: 3})), WorldShape, "viewport world size");
		expectApplied(session.apply(SetPaletteEntry(7, STONE)), Voxel, "viewport palette");
		require(paletteCodeForBlock(session.draftSnapshot().world.palette, STONE) == 7, "viewport brush assumed a global palette code");
		expectApplied(session.apply(PaintVoxel({x: 3, y: 1, z: 2}, 7)), Voxel, "viewport upper-layer paint");
		final upper = projectViewport(session.draftSnapshot().world, 1);
		require(upper != null && upper.width == 4 && upper.depth == 3 && upper.cells.length == 12, "viewport projection lost its exact layer dimensions");
		final volume = projectWorld(session.draftSnapshot().world);
		final reused = volume == null ? null : projectFromWorld(volume, 1);
		require(reused != null
			&& reused.cells.join(",") == upper.cells.join(","), "viewport projection changed when it reused the decoded 3D cells");
		final lower = volume == null ? null : projectFromWorld(volume, 0);
		require(lower != null && lower.layerY == 0 && paletteCodeAt(lower, 3, 2) == 0, "selected-layer projection reused the wrong horizontal cells");
		require(volume != null && projectFromWorld(volume, 2) == null, "selected-layer projection admitted a layer outside the cached world");
		require(projectFromCells(session.draftSnapshot().world, [0], 1) == null, "viewport projection admitted a malformed decoded cell array");
		require(clampLayer(-1, 2) == 0
			&& clampLayer(0, 2) == 0
			&& clampLayer(1, 2) == 1
			&& clampLayer(2, 2) == 1
			&& clampLayer(9, 0) == 0,
			"selected-layer bounds did not clamp to the finite world");
		require(boundsIntersectLayer({origin: {x: 0, y: 1, z: 0}, size: {width: 1, height: 2, depth: 1}}, 1)
			&& boundsIntersectLayer({origin: {x: 0, y: 1, z: 0}, size: {width: 1, height: 2, depth: 1}}, 2)
			&& !boundsIntersectLayer({origin: {x: 0, y: 1, z: 0}, size: {width: 1, height: 2, depth: 1}}, 0),
			"selected-layer bounds used column overlap instead of vertical overlap");
		require(paletteCodeAt(upper, 3, 2) == 7 && paletteCodeAt(upper, 0, 0) == 0 && paletteCodeAt(upper, 4, 0) == -1,
			"viewport projection lost painted, air, or out-of-range cell semantics");
		require(projectViewport(session.draftSnapshot().world, 2) == null, "viewport admitted a layer outside the world");

		final grid = layoutViewport(10, 20, 410, 180, upper);
		require(grid != null && grid.left == 95 && grid.top == 20 && grid.width == 240 && grid.height == 180 && grid.cellSize == 60,
			"viewport did not center the largest square-cell grid");
		final first = viewportPointAt(upper, grid, 95, 20);
		final last = viewportPointAt(upper, grid, 334, 199);
		require(first != null && first.x == 0 && first.y == 1 && first.z == 0, "viewport mapped its included top-left pixel incorrectly");
		require(last != null && last.x == 3 && last.y == 1 && last.z == 2, "viewport mapped its included bottom-right pixel incorrectly");
		require(viewportPointAt(upper, grid, 335, 199) == null
			&& viewportPointAt(upper, grid, 94, 20) == null, "viewport admitted an excluded grid edge");

		require(toolFromIndex(0) == SelectTool
			&& toolFromIndex(1) == PaintTool
			&& toolFromIndex(2) == EraseTool
			&& toolFromIndex(3) == FillTool
			&& toolFromIndex(4) == CheckpointTool
			&& toolFromIndex(5) == CatalogObjectTool
			&& toolFromIndex(6) == TriggerZoneTool
			&& toolFromIndex(-1) == null
			&& toolFromIndex(7) == null,
			"raygui tool indices drifted from the closed editor tool type");

		final point:VoxelPoint = {x: 2, y: 1, z: 1};
		switch commandForTool(SelectTool, point, 1, null, [], [], null) {
			case ToolSelectionReady(bounds):
				require(bounds.origin.x == 2 && bounds.origin.y == 1 && bounds.origin.z == 1 && bounds.size.width == 1 && bounds.size.height == 1
					&& bounds.size.depth == 1,
					"select tool did not create one exact voxel selection");
			case _:
				throw "select tool did not produce workspace bounds";
		}
		switch commandForTool(PaintTool, point, 1, null, [], [], null) {
			case ToolCommandReady(PaintVoxel(actual, 1)):
				require(actual.x == point.x && actual.y == point.y && actual.z == point.z, "paint tool changed the pointed voxel");
			case _:
				throw "paint tool did not produce a PaintVoxel command";
		}
		switch commandForTool(EraseTool, point, 1, null, [], [], null) {
			case ToolCommandReady(EraseVoxel(actual)):
				require(actual.x == point.x && actual.y == point.y && actual.z == point.z, "erase tool changed the pointed voxel");
			case _:
				throw "erase tool did not produce an EraseVoxel command";
		}
		switch commandForTool(FillTool, point, 1, null, [], [], null) {
			case ToolCommandRejected(NoSelection):
			case _:
				throw "fill tool did not reject a missing selection exactly";
		}
		final selected:VoxelBounds = {origin: {x: 1, y: 0, z: 1}, size: {width: 2, height: 1, depth: 2}};
		switch commandForTool(FillTool, point, 1, selected, [], [], null) {
			case ToolCommandReady(FillBounds(bounds, 1)):
				require(bounds.origin.x == 1 && bounds.origin.z == 1 && bounds.size.width == 2 && bounds.size.depth == 2,
					"fill tool changed its explicit workspace bounds");
			case _:
				throw "fill tool did not carry explicit typed bounds";
		}
		switch commandForTool(CheckpointTool, point, 1, null, [], [], null) {
			case ToolBatchReady(commands, selectedObject):
				require(commands.length == 2 && selectedObject.text() == "editor.checkpoint.n1",
					"checkpoint tool did not produce one selectable atomic template");
			case _:
				throw "checkpoint tool did not produce a canonical command batch";
		}
		switch commandForTool(TriggerZoneTool, point, 1, null, [], [], null) {
			case ToolCommandReady(PutObject(object)):
				require(object.id.text() == "editor.trigger.n1", "trigger tool changed its deterministic object ID");
				switch object.placement {
					case TriggerZone(bounds):
						require(bounds.origin.x == point.x && bounds.origin.y == point.y && bounds.origin.z == point.z,
							"trigger tool changed the selected layer or cell");
					case _: throw "trigger tool changed its placement role";
				}
			case _:
				throw "trigger tool did not produce one canonical object command";
		}
		return 20;
	}

	/**
	 * Prove full-volume projection, fly-camera bounds, and deterministic picking.
	 *
	 * Raylib supplies native screen rays, but it does not decide which authored
	 * cell they mean. These target-neutral checks keep the 3D editor and future
	 * automation on the same finite CAXEMAP coordinates.
	 */
	static function checkWorldViewport():Int {
		final session = open(defaultEditorSettings());
		expectApplied(session.apply(ResizeWorld({width: 4, height: 2, depth: 3})), WorldShape, "3D viewport world size");
		expectApplied(session.apply(SetPaletteEntry(1, STONE)), Voxel, "3D viewport palette");
		expectApplied(session.apply(PaintVoxel({x: 1, y: 0, z: 1}, 1)), Voxel, "3D viewport lower block");
		expectApplied(session.apply(PaintVoxel({x: 1, y: 1, z: 1}, 1)), Voxel, "3D viewport upper block");
		expectApplied(session.apply(PaintVoxel({x: 3, y: 0, z: 2}, 1)), Voxel, "3D viewport distant block");
		final projection = projectWorld(session.draftSnapshot().world);
		require(projection != null && projection.width == 4 && projection.height == 2 && projection.depth == 3 && projection.cells.length == 24,
			"3D viewport projection lost finite volume dimensions");
		require(projection.columns.length == 2 && projection.columns[0].x == 1 && projection.columns[0].z == 1 && projection.columns[0].topY == 1
			&& projection.columns[1].x == 3 && projection.columns[1].z == 2 && projection.columns[1].topY == 0,
			"3D viewport surface overview lost canonical columns or top heights");
		require(projection.surfacePatches.length == 2
			&& projection.surfacePatches[0].x == 1
			&& projection.surfacePatches[0].z == 1
			&& projection.surfacePatches[0].width == 1
			&& projection.surfacePatches[0].depth == 1
			&& surfaceTopAt(projection, 1, 1) == 1
			&& surfaceTopAt(projection, 0, 0) == -1,
			"3D viewport surface patches changed authored height or empty columns");
		require(paletteCodeAtWorld(projection, 1, 0, 1) == 1
			&& paletteCodeAtWorld(projection, 1, 1, 1) == 1
			&& paletteCodeAtWorld(projection, 0, 0, 0) == 0
			&& paletteCodeAtWorld(projection, 4, 0, 0) == -1,
			"3D viewport projection lost solid, air, or excluded coordinates");

		final focused = focusCamera(projection);
		final focusedPose = cameraPose(focused);
		require(cameraMode(focused) == EditorCameraMode.FlyCamera
			&& close(focusedPose.x, 2.0)
			&& close(focusedPose.y, 5.6)
			&& close(focusedPose.z, 5.6)
			&& close(focusedPose.lookX, 0.0)
			&& close(focusedPose.lookY, -0.5)
			&& close(focusedPose.lookZ, -0.8660254037844386),
			"3D viewport focus did not frame the finite world deterministically");
		final target = cameraTarget(focused);
		require(close(target.x, focusedPose.x + focusedPose.lookX)
			&& close(target.y, focusedPose.y + focusedPose.lookY)
			&& close(target.z, focusedPose.z + focusedPose.lookZ),
			"3D camera target drifted from its direction snapshot");
		final moved = stepCamera(projection, focused, {
			forward: 1.0,
			right: 0.5,
			vertical: 0.25,
			yaw: 0.10,
			pitch: 0.05,
			wheel: 1.0
		}, 0.05);
		final movedPose = cameraPose(moved);
		require(movedPose.x != focusedPose.x && movedPose.y != focusedPose.y && movedPose.z != focusedPose.z && movedPose.lookX < 0.0
			&& movedPose.lookY > focusedPose.lookY,
			"3D camera step ignored movement or look input");
		final clamped = stepCamera(projection, moved, {
			forward: 10000.0,
			right: -10000.0,
			vertical: -10000.0,
			yaw: 10.0,
			pitch: -10.0,
			wheel: 10000.0
		}, 10.0);
		final clampedPose = cameraPose(clamped);
		require(clampedPose.x >= -128.0
			&& clampedPose.x <= projection.width + 128.0
			&& clampedPose.y >= 0.25
			&& clampedPose.y <= projection.height + 128.0
			&& clampedPose.z >= -128.0
			&& clampedPose.z <= projection.depth + 128.0
			&& close(clampedPose.lookY, -0.90),
			"3D camera failed to clamp frame time, position, yaw, or pitch");

		final walking = focusCamera(projection, EditorCameraMode.WalkCamera);
		final walkingPose = cameraPose(walking);
		require(cameraMode(walking) == EditorCameraMode.WalkCamera
			&& close(walkingPose.x, 2.0)
			&& close(walkingPose.y, 1.62)
			&& close(walkingPose.z, 2.5),
			"Walk did not start inside the draft at player-like eye height");
		final walked = stepCamera(projection, walking, {
			forward: 0.0,
			right: 1.0,
			vertical: 1.0,
			yaw: 0.0,
			pitch: 0.0,
			wheel: 100.0
		}, 0.1);
		final walkedAgain = stepCamera(projection, walked, {
			forward: 0.0,
			right: 1.0,
			vertical: -1.0,
			yaw: 0.0,
			pitch: 0.0,
			wheel: -100.0
		}, 0.1);
		final walkedPose = cameraPose(walkedAgain);
		require(walkedPose.x > walkingPose.x && close(walkedPose.y, 2.62) && close(walkedPose.z, walkingPose.z),
			"Walk did not follow the authored surface or ignored its no-flight contract");

		final orbitTarget:EditorWorldVector = {x: 1.5, y: 1.0, z: 1.5};
		final orbiting = focusCamera(projection, EditorCameraMode.OrbitCamera, orbitTarget, 4.0);
		final orbitingPose = cameraPose(orbiting);
		require(cameraMode(orbiting) == EditorCameraMode.OrbitCamera
			&& close(cameraTarget(orbiting).x, orbitTarget.x)
			&& close(cameraTarget(orbiting).y, orbitTarget.y)
			&& close(cameraTarget(orbiting).z, orbitTarget.z),
			"Orbit did not retain its explicit authored target");
		final orbited = stepCamera(projection, orbiting, {
			forward: 1.0,
			right: 1.0,
			vertical: 1.0,
			yaw: 0.1,
			pitch: 0.05,
			wheel: 1.0
		}, 0.1);
		final orbitedPose = cameraPose(orbited);
		require(orbitedPose.x != orbitingPose.x
			&& orbitedPose.y != orbitingPose.y
			&& orbitedPose.z != orbitingPose.z
			&& close(cameraTarget(orbited).x, orbitTarget.x)
			&& close(cameraTarget(orbited).y, orbitTarget.y)
			&& close(cameraTarget(orbited).z, orbitTarget.z),
			"Orbit did not rotate and zoom around its fixed target");
		final nextOrbitTarget:EditorWorldVector = {x: 3.5, y: 1.0, z: 2.5};
		final retargeted = retargetOrbitCamera(orbited, nextOrbitTarget);
		final retargetedPose = cameraPose(retargeted);
		require(close(cameraTarget(retargeted).x, nextOrbitTarget.x)
			&& close(cameraTarget(retargeted).y, nextOrbitTarget.y)
			&& close(cameraTarget(retargeted).z, nextOrbitTarget.z)
			&& close(retargetedPose.lookX, orbitedPose.lookX)
			&& close(retargetedPose.lookY, orbitedPose.lookY)
			&& close(retargetedPose.lookZ, orbitedPose.lookZ),
			"Orbit changed its viewing angle when selection moved its target");
		require(cycleCameraMode(EditorCameraMode.WalkCamera) == EditorCameraMode.FlyCamera
			&& cycleCameraMode(EditorCameraMode.FlyCamera) == EditorCameraMode.OrbitCamera
			&& cycleCameraMode(EditorCameraMode.OrbitCamera) == EditorCameraMode.WalkCamera,
			"the camera control did not cycle through one closed mode order");

		final stacked = pickWorld(projection, {x: 1.5, y: 4.0, z: 1.5}, {x: 0.0, y: -1.0, z: 0.0}, 0, 16.0);
		require(stacked != null && stacked.solid && stacked.point.x == 1 && stacked.point.y == 1 && stacked.point.z == 1 && stacked.placement == null
			&& close(stacked.distance, 2.0),
			"3D picking did not reject placement outside the world above a solid");
		final side = pickWorld(projection, {x: 0.25, y: 0.5, z: 1.5}, {x: 1.0, y: 0.0, z: 0.0}, 0, 16.0);
		require(side != null && side.solid && side.point.x == 1 && side.point.y == 0 && side.point.z == 1 && side.placement != null
			&& side.placement.x == 0 && side.placement.y == 0 && side.placement.z == 1,
			"3D picking did not retain the adjacent empty cell before a solid");
		final emptyFloor = pickWorld(projection, {x: 0.5, y: 4.0, z: 0.5}, {x: 0.0, y: -1.0, z: 0.0}, 0, 16.0);
		require(emptyFloor != null && !emptyFloor.solid && emptyFloor.point.x == 0 && emptyFloor.point.y == 0 && emptyFloor.point.z == 0
			&& emptyFloor.placement != null && emptyFloor.placement.x == 0 && emptyFloor.placement.y == 0 && emptyFloor.placement.z == 0
			&& close(emptyFloor.distance, 4.0),
			"3D picking did not preserve an editable empty-floor cell");
		final upperEmpty = pickWorld(projection, {x: 0.5, y: 4.0, z: 0.5}, {x: 0.0, y: -1.0, z: 0.0}, 1, 16.0);
		require(upperEmpty != null && !upperEmpty.solid && upperEmpty.point.y == 1 && close(upperEmpty.distance, 3.0),
			"3D picking did not use the selected empty-cell layer");
		require(pickWorld(projection, {x: -1.0, y: 2.0, z: -1.0}, {x: 0.0, y: -1.0, z: 0.0}, 0, 16.0) == null,
			"3D picking admitted a floor point outside the draft");
		require(pickWorld(projection, {x: 0.5, y: 4.0, z: 0.5}, {x: 1.0, y: 0.0, z: 0.0}, 0, 16.0) == null,
			"3D picking invented a floor point for a parallel ray");
		require(pickWorld(projection, {x: 0.5, y: 4.0, z: 0.5}, {x: 0.0, y: -1.0, z: 0.0}, 2, 16.0) == null, "3D picking admitted an unavailable edit layer");

		final objectGizmos:Array<EditorObjectGizmo> = [
			{
				id: id("object.near"),
				kind: EditorObjectGizmoKind.CheckpointGizmo,
				x: 1.5,
				y: 1.0,
				z: 1.5,
				width: 1.0,
				height: 2.0,
				depth: 1.0,
				facing: ObjectYaw(0)
			},
			{
				id: id("object.far"),
				kind: EditorObjectGizmoKind.NpcGizmo,
				x: 1.5,
				y: 1.0,
				z: 3.5,
				width: 1.0,
				height: 2.0,
				depth: 1.0,
				facing: ObjectYaw(90)
			},
			{
				id: id("object.overlap"),
				kind: EditorObjectGizmoKind.ItemGizmo,
				x: 1.5,
				y: 1.0,
				z: 1.5,
				width: 1.0,
				height: 2.0,
				depth: 1.0,
				facing: ObjectYaw(180)
			}
		];
		final objectHit = pickObject(objectGizmos, {x: 1.5, y: 1.0, z: -2.0}, {x: 0.0, y: 0.0, z: 1.0}, 16.0);
		require(objectHit != null && objectHit.id.text() == "object.near" && close(objectHit.distance, 3.0),
			"3D object picking did not choose the nearest authored object with a stable tie");
		require(pickObject(objectGizmos, {x: 5.0, y: 1.0, z: -2.0}, {x: 0.0, y: 0.0, z: 1.0}, 16.0) == null,
			"3D object picking admitted a parallel ray outside every object");
		require(pickObject(objectGizmos, {x: 1.5, y: 1.0, z: -2.0}, {x: 0.0, y: 0.0, z: 1.0}, 2.0) == null,
			"3D object picking ignored the bounded ray distance");
		require(gizmoIntersectsLayer(objectGizmos[0], 0)
			&& gizmoIntersectsLayer(objectGizmos[0], 1)
			&& !gizmoIntersectsLayer(objectGizmos[0], 2),
			"Plan object filtering lost exact vertical overlap");
		return 27;
	}

	/** Prove that Plan logic links retain rule order and fail closed. */
	static function checkZoneRuleProjection():Void {
		final zone = id("zone.workshop");
		final missing = id("zone.missing");
		final bounds:VoxelBounds = {origin: {x: 1, y: 0, z: 2}, size: {width: 2, height: 3, depth: 4}};
		final rules = [
			{
				id: id("rule.enter"),
				priority: 0,
				repeat: Once,
				event: EnterZone(zone),
				predicate: Always,
				actions: []
			},
			{
				id: id("rule.interact"),
				priority: 1,
				repeat: Once,
				event: Interact(CHECKPOINT),
				predicate: Always,
				actions: []
			},
			{
				id: id("rule.leave"),
				priority: 2,
				repeat: Repeat,
				event: LeaveZone(missing),
				predicate: Always,
				actions: []
			}
		];
		final links = projectZoneRules(rules, [{id: zone, tags: [], placement: TriggerZone(bounds)}]);
		require(links.length == 2, "zone-rule projection included an unrelated event");
		switch links[0] {
			case ResolvedZoneRule(ruleId, zoneId, projected):
				require(ruleId.text() == "rule.enter" && zoneId.text() == zone.text(), "resolved zone-rule projection lost stable IDs");
				require(projected.origin.x == 1 && projected.origin.z == 2 && projected.size.width == 2 && projected.size.height == 3
					&& projected.size.depth == 4,
					"resolved zone-rule projection changed trigger bounds");
			case _:
				throw "valid zone-rule projection did not resolve";
		}
		switch links[1] {
			case UnresolvedZoneRule(ruleId, zoneId):
				require(ruleId.text() == "rule.leave" && zoneId.text() == missing.text(), "unresolved zone-rule projection lost stable IDs");
			case _:
				throw "missing zone-rule projection invented geometry";
		}
	}

	static inline function close(actual:Float, expected:Float):Bool
		return actual > expected - 0.000001 && actual < expected + 0.000001;

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
			{command: RemoveFluid(WATER_SOURCE), family: Fluid, label: "remove fluid source"},
			{command: RemoveRule(RULE), family: Rule, label: "remove rule"},
			{command: RemoveObjective(OBJECTIVE), family: Objective, label: "remove objective"},
			{command: RemoveDialogue(DIALOGUE), family: Dialogue, label: "remove dialogue"},
			{command: RemoveMessage(EN, TITLE_MESSAGE), family: Localization, label: "remove localized message"},
			{command: RemoveLocale(FR), family: Localization, label: "remove locale"}
		]) {
			expectApplied(session.apply(entry.command), entry.family, entry.label);
			expectHistory(session.undo(), entry.family, 'undo ${entry.label}');
			expectHistory(session.redo(), entry.family, 'redo ${entry.label}');
			expectHistory(session.undo(), entry.family, 'restore after ${entry.label}');
		}
	}

	static function checkLocalization(session:EditorSession):Void {
		final messages = session.draftSnapshot().messages;
		require(resolveScenarioMessage(messages, EN, TITLE_MESSAGE) == "Editor QA map, revised", "editor did not retain an updated English message");
		require(resolveScenarioMessage(messages, new LocaleId("de"), TITLE_MESSAGE) == "Mapa QA del editor",
			"an unavailable locale did not fall back to the selected default");

		expectApplied(session.apply(RemoveMessage(EN, OBJECTIVE_BODY_MESSAGE)), Localization, "make one locale incomplete");
		final diagnostics = switch session.validate() {
			case ValidationFailed(values): values;
			case _: throw "an incomplete translation set unexpectedly validated";
		};
		require(hasMissingTranslation(diagnostics, EN, OBJECTIVE_BODY_MESSAGE), "incomplete translation lost its exact locale and message diagnostic");
		expectHistory(session.undo(), Localization, "restore removed translation");
		expectValid(session, "restored translation catalog");
	}

	static function checkHardBounds():Void {
		final settings:EditorSettings = {
			historyEntries: 3,
			historyBytes: 1048576,
			selectionCells: 4,
			transactionCommands: 3
		};
		final session = open(settings);
		expectApplied(session.apply(ResizeWorld({width: 3, height: 1, depth: 3})), WorldShape, "bounded resize");
		for (index in 0...6)
			expectApplied(session.apply(SetTitle(Literal('History $index'))), DocumentMetadata, "bounded document history");
		require(session.historyEntries() == 3 && session.undoDepth() == 3, "history did not evict to its exact entry bound");
		require(session.historyBytes() <= settings.historyBytes, "history exceeded its byte bound");
		expectSelectionRejected(session.select({
			baseRevision: session.revision(),
			selection: VoxelSelection({origin: {x: 0, y: 0, z: 0}, size: {width: 3, height: 1, depth: 2}})
		}), error -> switch error {
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

		final tiny = open({
			historyEntries: 3,
			historyBytes: 1,
			selectionCells: 4,
			transactionCommands: 3
		});
		final before = tiny.canonicalDraft();
		expectRejected(tiny.apply(ResizeWorld({width: 2, height: 1, depth: 1})), error -> switch error {
			case HistoryEntryTooLarge(_, 1): true;
			case _: false;
		}, "history byte budget");
		require(tiny.canonicalDraft().compare(before) == 0, "rejected history entry changed the draft");

		final invalidSettings:EditorSettings = {
			historyEntries: MAX_HISTORY_ENTRIES + 1,
			historyBytes: 1,
			selectionCells: 1,
			transactionCommands: 1
		};
		switch EditorSession.open(baseScenario(), new Registry(), invalidSettings) {
			case EditorOpenRejected(InvalidSetting(HistoryEntries, 1, MAX_HISTORY_ENTRIES)):
			case _:
				throw "editor accepted settings above the hard history-entry bound";
		}

		final invalidTransactionSettings:EditorSettings = {
			historyEntries: 1,
			historyBytes: 1,
			selectionCells: 1,
			transactionCommands: MAX_TRANSACTION_COMMANDS + 1
		};
		switch EditorSession.open(baseScenario(), new Registry(), invalidTransactionSettings) {
			case EditorOpenRejected(InvalidSetting(TransactionCommands, 1, MAX_TRANSACTION_COMMANDS)):
			case _:
				throw "editor accepted settings above the hard transaction-command bound";
		}
	}

	static function checkSnapshotFidelity():Void {
		final unsupported = withFormatVersion(baseScenario(), 2);
		switch EditorSession.open(unsupported, new Registry()) {
			case EditorOpenRejected(UnsupportedFormatVersion(2, ScenarioWriter.FORMAT_VERSION)):
			case _:
				throw "editor silently normalized an unsupported CAXEMAP version";
		}

		final session = open(defaultEditorSettings());
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

	/** Prove a caller-owned mutable placement payload cannot mutate an accepted draft. */
	static function checkPlacementInputIsolation():Void {
		final session = open(defaultEditorSettings());
		final tags = [new ScenarioTag("before")];
		final objectId = id("placement.input-isolation");
		expectApplied(session.apply(PutObject({
			id: objectId,
			tags: tags,
			placement: Checkpoint(transform(1000, 0, 1000))
		})), Placement, "place caller-owned object payload");
		final accepted = session.canonicalDraft();

		tags.push(new ScenarioTag("after"));
		require(session.canonicalDraft().compare(accepted) == 0, "caller mutation changed accepted placement bytes");

		var observed:Null<ScenarioObject> = null;
		for (object in session.draftSnapshot().objects)
			if (object.id.text() == objectId.text())
				observed = object;
		require(observed != null && observed.tags.length == 1, "caller tag mutation entered the accepted placement");
	}

	/** Prove a deferred placement snapshot reconstructs exact diagnostic coordinates. */
	static function checkDeferredPlacementValidation():Void {
		final session = open(defaultEditorSettings());
		expectApplied(session.apply(ResizeWorld({width: 4, height: 1, depth: 1})), WorldShape, "resize deferred-validation world");
		expectApplied(session.apply(SetPaletteEntry(1, STONE)), Voxel, "add deferred-validation palette entry");
		final missing = id("missing.deferred-target");
		expectApplied(session.apply(PutRule({
			id: id("rule.deferred-validation"),
			priority: 1,
			repeat: Once,
			event: Interact(missing),
			predicate: Always,
			actions: [SetCheckpoint(PLAYER)]
		})), Rule, "add deferred-validation diagnostic");
		expectApplied(session.apply(RotateObjectBy(PLAYER, 90)), Placement, "rotate through deferred snapshot path");

		final observed = switch session.query(InspectValidation) {
			case ValidationObserved(_, DraftInvalid(diagnostics)): diagnostics;
			case _: throw "deferred placement snapshot did not retain its semantic diagnostic";
		};
		final expected = validationDiagnostics(session.canonicalDraft());
		require(sameDiagnosticCoordinates(observed, expected),
			'deferred placement validation did not reconstruct canonical source coordinates: observed=${diagnosticCoordinatesText(observed)} expected=${diagnosticCoordinatesText(expected)}');
	}

	/** Compare every semantic diagnostic location from the same canonical bytes. */
	static function sameDiagnosticCoordinates(left:Array<caxecraft.scenario.ScenarioDiagnostic>, right:Array<caxecraft.scenario.ScenarioDiagnostic>):Bool {
		if (left.length == 0 || left.length != right.length)
			return false;
		for (index in 0...left.length) {
			final actual = left[index].coordinate;
			final expected = right[index].coordinate;
			if (actual.line != expected.line || actual.column != expected.column || actual.record != expected.record)
				return false;
		}
		return true;
	}

	/** Format diagnostic coordinates only when the exact comparison fails. */
	static function diagnosticCoordinatesText(values:Array<caxecraft.scenario.ScenarioDiagnostic>):String
		return [
			for (value in values)
				'${value.coordinate.line}:${value.coordinate.column}:${value.coordinate.record}'
		].join(",");

	static function checkHistoryStateChanges():Void {
		final session = open(defaultEditorSettings());
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
		final session = open(defaultEditorSettings());
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

	static function checkExternalTestPlayAtomicity():Void {
		final session = open(defaultEditorSettings());
		final lastPlayableBefore = session.lastPlayableSnapshot();
		require(lastPlayableBefore != null, "external Test Play fixture has no recovery snapshot");
		final lastPlayableBytes = ScenarioWriter.write(lastPlayableBefore);
		expectApplied(session.apply(SetTitle(Literal("External runtime candidate"))), DocumentMetadata, "external Test Play draft edit");
		final canonicalBefore = session.canonicalDraft();
		final revisionBefore = session.revision();
		final historyEntriesBefore = session.historyEntries();
		final historyBytesBefore = session.historyBytes();
		final undoBefore = session.undoDepth();
		final redoBefore = session.redoDepth();
		final prepared = switch session.prepareExternalTestPlay() {
			case ValidationPassed(canonical): canonical;
			case ValidationFailed(_) | ValidationBlocked(_): throw "valid external Test Play candidate was rejected";
		};
		require(prepared.compare(canonicalBefore) == 0, "external Test Play preparation changed canonical bytes");
		final lastPlayableAfterPrepare = session.lastPlayableSnapshot();
		require(lastPlayableAfterPrepare != null && ScenarioWriter.write(lastPlayableAfterPrepare).compare(lastPlayableBytes) == 0,
			"side-effect-free external preparation replaced lastPlayable");
		require(session.revision() == revisionBefore
			&& session.historyEntries() == historyEntriesBefore
			&& session.historyBytes() == historyBytesBefore
			&& session.undoDepth() == undoBefore
			&& session.redoDepth() == redoBefore,
			"rejected external runtime start would change editor recovery state");
		require(session.beginExternalTestPlay(), "accepted external runtime could not lock editing");
		expectRejected(session.apply(SetPaletteEntry(1, STONE)), error -> switch error {
			case NotEditing: true;
			case _: false;
		}, "edit during external Test Play");
		require(session.finishExternalTestPlay(), "external Test Play lock did not close");
		require(!session.finishExternalTestPlay(), "external Test Play lock closed twice");
		require(session.canonicalDraft().compare(canonicalBefore) == 0
			&& session.revision() == revisionBefore
			&& session.historyEntries() == historyEntriesBefore
			&& session.historyBytes() == historyBytesBefore
			&& session.undoDepth() == undoBefore
			&& session.redoDepth() == redoBefore,
			"external Test Play changed the editor workspace");
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
		expectRejected(session.apply(SetDefaultLocale(new LocaleId("missing"))), error -> switch error {
			case MissingLocale(_): true;
			case _: false;
		}, "unknown default locale");
		expectRejected(session.apply(PutMessage(new LocaleId("missing"), message(TITLE_MESSAGE, "missing"))), error -> switch error {
			case MissingLocale(_): true;
			case _: false;
		}, "message for unknown locale");
		expectRejected(session.apply(RemoveMessage(EN, new MessageId("missing.message"))), error -> switch error {
			case MissingMessage(_, _): true;
			case _: false;
		}, "remove unknown message");
		expectRejected(session.apply(RemoveLocale(ES_MX)), error -> switch error {
			case CannotRemoveDefaultLocale(_): true;
			case _: false;
		}, "remove current default locale");
		expectRejected(session.apply(RemoveFluid(id("water.missing"))), error -> switch error {
			case MissingFluid(_): true;
			case _: false;
		}, "remove unknown fluid");
		expectSelectionRejected(session.select({
			baseRevision: session.revision(),
			selection: VoxelSelection({
				origin: {x: 1, y: 0, z: 0},
				size: {width: 2147483647, height: 1, depth: 1}
			})
		}), error -> switch error {
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
		require(before.compare(after) != 0, "accepted content command changed no authored bytes");
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

	/** Read validation diagnostics directly from canonical bytes as an oracle. */
	static function validationDiagnostics(bytes:Bytes):Array<caxecraft.scenario.ScenarioDiagnostic> {
		final records = switch ScenarioLexer.read(bytes) {
			case ReadOk(value): value;
			case ReadError(_): throw "deferred-validation bytes did not lex";
		};
		final parsed = switch ScenarioParser.parse(records) {
			case ReadOk(value): value;
			case ReadError(_): throw "deferred-validation bytes did not parse";
		};
		return switch ScenarioValidator.validate(parsed, new Registry()) {
			case ReadError(diagnostics): diagnostics;
			case ReadOk(_): throw "deferred-validation oracle unexpectedly accepted the invalid rule";
		}
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
			case EditApplied(actual, _, _, _):
				require(actual == family, '$label reported the wrong command family');
			case EditUnchanged(_):
				throw '$label unexpectedly made no change';
			case EditRejected(error):
				throw '$label was rejected: $error';
		}
	}

	static function expectHistory(result:EditorHistoryResult, family:EditorCommandFamily, label:String):Void {
		switch result {
			case HistoryApplied(actual, _, _, _):
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

	/** Prove that one workspace target changes no authored document state. */
	static function expectSelection(session:EditorSession, selection:EditorSelection, label:String):Void {
		final beforeBytes = session.canonicalDraft();
		final beforeRevision = session.revision();
		final beforeUndo = session.undoDepth();
		final beforeRedo = session.redoDepth();
		switch session.select({baseRevision: beforeRevision, selection: selection}) {
			case SelectionApplied(_, actualRevision) | SelectionUnchanged(_, actualRevision):
				require(actualRevision == beforeRevision, '$label changed the document revision');
			case SelectionRejected(error, _):
				throw '$label was rejected: $error';
		}
		require(session.canonicalDraft().compare(beforeBytes) == 0
			&& session.revision() == beforeRevision
			&& session.undoDepth() == beforeUndo
			&& session.redoDepth() == beforeRedo,
			'$label changed canonical bytes or history');
	}

	static function expectSelectionRejected(result:EditorSelectionResult, matches:EditorError->Bool, label:String):Void {
		switch result {
			case SelectionRejected(error, _):
				require(matches(error), '$label returned the wrong error: $error');
			case SelectionApplied(_, _) | SelectionUnchanged(_, _):
				throw '$label unexpectedly changed workspace selection';
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

	static function hasMissingTranslation(values:Array<caxecraft.scenario.ScenarioDiagnostic>, locale:LocaleId, message:MessageId):Bool {
		for (value in values)
			switch value.kind {
				case MissingTranslation(actualLocale, actualMessage):
					if (actualLocale.text() == locale.text() && actualMessage.text() == message.text())
						return true;
				case _:
			}
		return false;
	}

	static function selectionKey(session:EditorSession):String {
		return switch session.selectionSnapshot() {
			case NoEditorSelection: "none";
			case VoxelSelection(value):
				'voxel:${value.origin.x},${value.origin.y},${value.origin.z}:${value.size.width},${value.size.height},${value.size.depth}';
			case NodeSelection(ObjectNode(id)): 'object:${id.text()}';
			case NodeSelection(_): "node";
		};
	}

	static function open(settings:EditorSettings):EditorSession {
		return switch EditorSession.open(baseScenario(), new Registry(), settings) {
			case EditorOpened(session): session;
			case EditorOpenRejected(error): throw 'editor did not open: $error';
		}
	}

	static function baseScenario():Scenario
		return createEditorScenario(id("editor.qa"), new LogicalPath("packs/caxecraft/base"), Message(TITLE_MESSAGE), Creative, AIR, PLAYER,
			EmbeddedMessageCatalog({
				defaultLocale: EN,
				locales: [
					locale(EN, "Editor QA map", "Hello, Haxirio.", "Reach the marker", "Use the checkpoint to finish."),
					locale(ES_MX, "Mapa QA del editor", "Hola, Haxirio.", "Llega al marcador", "Usa el punto de control para terminar.")
				]
			}));

	static function locale(id:LocaleId, title:String, dialogue:String, objectiveTitle:String, objectiveBody:String):ScenarioLocaleCatalog
		return {
			id: id,
			messages: [
				message(TITLE_MESSAGE, title),
				message(DIALOGUE_MESSAGE, dialogue),
				message(OBJECTIVE_TITLE_MESSAGE, objectiveTitle),
				message(OBJECTIVE_BODY_MESSAGE, objectiveBody)
			]
		};

	static inline function message(id:MessageId, text:String):ScenarioMessage
		return {id: id, text: text};

	static function withFormatVersion(source:Scenario, formatVersion:Int):Scenario
		return {
			formatVersion: formatVersion,
			requiredFeatures: source.requiredFeatures,
			optionalFeatures: source.optionalFeatures,
			id: source.id,
			assetPack: source.assetPack,
			messages: source.messages,
			title: source.title,
			mode: source.mode,
			environment: source.environment,
			world: source.world,
			objects: source.objects,
			story: source.story,
			flow: source.flow,
			extensions: source.extensions
		};

	static inline function transform(x:Int, y:Int, z:Int):caxecraft.scenario.ScenarioGeometry.ScenarioTransform
		return transformYaw(x, y, z, 0);

	static inline function transformYaw(x:Int, y:Int, z:Int, yawDegrees:Int):caxecraft.scenario.ScenarioGeometry.ScenarioTransform
		return {
			xMilli: x,
			yMilli: y,
			zMilli: z,
			yawDegrees: yawDegrees
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

	public function blockStorageCode(id:ContentId):Int {
		if (id.text() == "caxecraft:air")
			return 0;
		if (id.text() == "caxecraft:stone")
			return 3;
		return -1;
	}

	public function hasFluid(id:ContentId):Bool
		return id.text() == "caxecraft:water";

	public function hasItem(id:ContentId):Bool
		return id.text() == "caxecraft:item";

	public function itemStorageCode(id:ContentId):Int
		return -1;

	public function hasEntity(id:ContentId):Bool
		return id.text() == "caxecraft:entity";

	public function hasNpc(id:ContentId):Bool
		return id.text() == "caxecraft:ivvy";

	public function hasPrefab(id:ContentId):Bool
		return id.text() == "caxecraft:small-house";

	public function hasStatefulObject(id:ContentId):Bool
		return id.text() == "caxecraft:mechanism";

	public function hasState(id:ContentId):Bool
		return id.text() == "caxecraft:idle";

	public function hasEffect(id:ContentId):Bool
		return false;

	public function hasSignal(id:ContentId):Bool
		return false;

	public function maximumItemQuantity(id:ContentId):Int
		return 64;
}

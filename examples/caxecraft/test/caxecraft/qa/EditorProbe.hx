package caxecraft.qa;

import caxecraft.editor.EditorActionPalette.availableScenarioActions;
import caxecraft.editor.EditorFocus.EditorFocusMove;
import caxecraft.editor.EditorFocus.EditorFocusTarget;
import caxecraft.editor.EditorFocus.initialFocus;
import caxecraft.editor.EditorFocus.moveFocus;
import caxecraft.editor.EditorPolicy.MAX_HISTORY_ENTRIES;
import caxecraft.editor.EditorPolicy.MAX_TRANSACTION_COMMANDS;
import caxecraft.editor.EditorPolicy.defaults as defaultEditorSettings;
import caxecraft.editor.EditorPlacement.checkpointCommand;
import caxecraft.editor.EditorScenarioFactory.create as createEditorScenario;
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
import caxecraft.editor.EditorViewport.layout as layoutViewport;
import caxecraft.editor.EditorViewport.paletteCodeAt;
import caxecraft.editor.EditorViewport.paletteCodeForBlock;
import caxecraft.editor.EditorViewport.pointAt as viewportPointAt;
import caxecraft.editor.EditorViewport.project as projectViewport;
import caxecraft.editor.EditorViewport.toolFromIndex;
import caxecraft.editor.EditorWorldViewport.cameraTarget;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmoKind;
import caxecraft.editor.EditorWorldViewport.focusCamera;
import caxecraft.editor.EditorWorldViewport.paletteCodeAtWorld;
import caxecraft.editor.EditorWorldViewport.pickObject;
import caxecraft.editor.EditorWorldViewport.pickWorld;
import caxecraft.editor.EditorWorldViewport.projectObjects;
import caxecraft.editor.EditorWorldViewport.projectWorld;
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
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
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
		final viewportChecks = checkViewport();
		final worldViewportChecks = checkWorldViewport();
		final activeLevelChecks = checkActiveLevelProjection();
		checkEnvironmentTextRoundTrip();
		checkObjectMovement();
		checkCheckpointPlacement();
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
		checkTestPlayLocksEditing();
		checkExternalTestPlayAtomicity();
		checkImmediateRejections(session);

		final finalBytes = expectValid(session, "final recovered scenario");
		final trace = hash(finalBytes) ^ (commandChecks * 65537) ^ (protocolChecks * 8191) ^ (focusChecks * 2053) ^ (navigationChecks * 1031) ^ (viewportChecks * 4099) ^ (worldViewportChecks * 257) ^ (activeLevelChecks * 131) ^ session.historyEntries();
		Sys.println('caxemap-editor: $commandChecks command round trips, $protocolChecks protocol checks, $focusChecks focus checks, $navigationChecks navigation checks, $viewportChecks 2D checks, $worldViewportChecks 3D checks, $activeLevelChecks active-level checks, ${finalBytes.length} canonical bytes; bounded history/test-play/recovery; trace=$trace');
	}

	/** Prove that a creator gesture becomes one collision-free reloadable object. */
	static function checkCheckpointPlacement():Void {
		final existing:Array<ScenarioObject> = [
			{id: id("editor.checkpoint.1"), tags: [], placement: Checkpoint(transform(500, 0, 500))},
			{id: id("editor.checkpoint.2"), tags: [], placement: Checkpoint(transform(1500, 0, 500))}
		];
		switch checkpointCommand({x: 2, y: 1, z: 3}, existing) {
			case PutObject(object):
				require(object.id.text() == "editor.checkpoint.3", "checkpoint placement reused an authored object ID");
				switch object.placement {
					case Checkpoint(position):
						require(position.xMilli == 2500 && position.yMilli == 1000 && position.zMilli == 3500 && position.yawDegrees == 0,
							"checkpoint placement changed the independently authored snapped transform");
					case _: throw "checkpoint placement emitted the wrong CAXEMAP role";
				}
			case _:
				throw "checkpoint placement did not use the normal object command";
		}
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
			EditorFocusTarget.WorldName,
			EditorFocusTarget.Undo,
			EditorFocusTarget.Redo,
			EditorFocusTarget.Build,
			EditorFocusTarget.Plan,
			EditorFocusTarget.Play,
			EditorFocusTarget.SelectTool,
			EditorFocusTarget.GroundTool,
			EditorFocusTarget.EraseTool,
			EditorFocusTarget.CheckpointTool,
			EditorFocusTarget.MoreDetails,
			EditorFocusTarget.WorldList,
			EditorFocusTarget.Back
		];
		final backward:Array<EditorFocusTarget> = [
			EditorFocusTarget.WorldList,
			EditorFocusTarget.MoreDetails,
			EditorFocusTarget.CheckpointTool,
			EditorFocusTarget.EraseTool,
			EditorFocusTarget.GroundTool,
			EditorFocusTarget.SelectTool,
			EditorFocusTarget.Play,
			EditorFocusTarget.Plan,
			EditorFocusTarget.Build,
			EditorFocusTarget.Redo,
			EditorFocusTarget.Undo,
			EditorFocusTarget.WorldName,
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
		require(paletteCodeForBlock(session.draftSnapshot().world, STONE) == 7, "viewport brush assumed a global palette code");
		expectApplied(session.apply(PaintVoxel({x: 3, y: 1, z: 2}, 7)), Voxel, "viewport upper-layer paint");
		final upper = projectViewport(session.draftSnapshot().world, 1);
		require(upper != null && upper.width == 4 && upper.depth == 3 && upper.cells.length == 12, "viewport projection lost its exact layer dimensions");
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

		require(toolFromIndex(0) == SelectTool && toolFromIndex(1) == PaintTool && toolFromIndex(2) == EraseTool && toolFromIndex(3) == FillTool
			&& toolFromIndex(4) == CheckpointTool && toolFromIndex(-1) == null && toolFromIndex(5) == null,
			"raygui tool indices drifted from the closed editor tool type");

		final point:VoxelPoint = {x: 2, y: 1, z: 1};
		switch commandForTool(SelectTool, point, 1, null, []) {
			case ToolSelectionReady(bounds):
				require(bounds.origin.x == 2 && bounds.origin.y == 1 && bounds.origin.z == 1 && bounds.size.width == 1 && bounds.size.height == 1
					&& bounds.size.depth == 1,
					"select tool did not create one exact voxel selection");
			case _:
				throw "select tool did not produce workspace bounds";
		}
		switch commandForTool(PaintTool, point, 1, null, []) {
			case ToolCommandReady(PaintVoxel(actual, 1)):
				require(actual.x == point.x && actual.y == point.y && actual.z == point.z, "paint tool changed the pointed voxel");
			case _:
				throw "paint tool did not produce a PaintVoxel command";
		}
		switch commandForTool(EraseTool, point, 1, null, []) {
			case ToolCommandReady(EraseVoxel(actual)):
				require(actual.x == point.x && actual.y == point.y && actual.z == point.z, "erase tool changed the pointed voxel");
			case _:
				throw "erase tool did not produce an EraseVoxel command";
		}
		switch commandForTool(FillTool, point, 1, null, []) {
			case ToolCommandRejected(NoSelection):
			case _:
				throw "fill tool did not reject a missing selection exactly";
		}
		final selected:VoxelBounds = {origin: {x: 1, y: 0, z: 1}, size: {width: 2, height: 1, depth: 2}};
		switch commandForTool(FillTool, point, 1, selected, []) {
			case ToolCommandReady(FillBounds(bounds, 1)):
				require(bounds.origin.x == 1 && bounds.origin.z == 1 && bounds.size.width == 2 && bounds.size.depth == 2,
					"fill tool changed its explicit workspace bounds");
			case _:
				throw "fill tool did not carry explicit typed bounds";
		}
		return 13;
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
		require(close(focused.x, 2.0)
			&& close(focused.y, 5.6)
			&& close(focused.z, 5.6)
			&& close(focused.lookX, 0.0)
			&& close(focused.lookY, -0.5)
			&& close(focused.lookZ, -0.8660254037844386),
			"3D viewport focus did not frame the finite world deterministically");
		final target = cameraTarget(focused);
		require(close(target.x, focused.x + focused.lookX)
			&& close(target.y, focused.y + focused.lookY)
			&& close(target.z, focused.z + focused.lookZ),
			"3D camera target drifted from its direction snapshot");
		final moved = stepCamera(projection, focused, {
			forward: 1.0,
			right: 0.5,
			vertical: 0.25,
			yaw: 0.10,
			pitch: 0.05,
			wheel: 1.0
		}, 0.05);
		require(moved.x != focused.x && moved.y != focused.y && moved.z != focused.z && moved.lookX < 0.0 && moved.lookY > focused.lookY,
			"3D camera step ignored movement or look input");
		final clamped = stepCamera(projection, moved, {
			forward: 10000.0,
			right: -10000.0,
			vertical: -10000.0,
			yaw: 10.0,
			pitch: -10.0,
			wheel: 10000.0
		}, 10.0);
		require(clamped.x >= -128.0
			&& clamped.x <= projection.width + 128.0
			&& clamped.y >= 0.25
			&& clamped.y <= projection.height + 128.0
			&& clamped.z >= -128.0
			&& clamped.z <= projection.depth + 128.0
			&& close(clamped.lookY, -0.90),
			"3D camera failed to clamp frame time, position, yaw, or pitch");

		final stacked = pickWorld(projection, {x: 1.5, y: 4.0, z: 1.5}, {x: 0.0, y: -1.0, z: 0.0}, 0, 16.0);
		require(stacked != null && stacked.solid && stacked.point.x == 1 && stacked.point.y == 1 && stacked.point.z == 1 && close(stacked.distance, 2.0),
			"3D picking did not choose the nearest visible solid");
		final emptyFloor = pickWorld(projection, {x: 0.5, y: 4.0, z: 0.5}, {x: 0.0, y: -1.0, z: 0.0}, 0, 16.0);
		require(emptyFloor != null && !emptyFloor.solid && emptyFloor.point.x == 0 && emptyFloor.point.y == 0 && emptyFloor.point.z == 0
			&& close(emptyFloor.distance, 4.0),
			"3D picking did not preserve an editable empty-floor cell");
		require(pickWorld(projection, {x: -1.0, y: 2.0, z: -1.0}, {x: 0.0, y: -1.0, z: 0.0}, 0, 16.0) == null,
			"3D picking admitted a floor point outside the draft");
		require(pickWorld(projection, {x: 0.5, y: 4.0, z: 0.5}, {x: 1.0, y: 0.0, z: 0.0}, 0, 16.0) == null,
			"3D picking invented a floor point for a parallel ray");
		require(pickWorld(projection, {x: 0.5, y: 4.0, z: 0.5}, {x: 0.0, y: -1.0, z: 0.0}, 2, 16.0) == null, "3D picking admitted an unavailable edit layer");

		final objectGizmos = [
			{
				id: id("object.near"),
				kind: EditorObjectGizmoKind.CheckpointGizmo,
				x: 1.5,
				y: 1.0,
				z: 1.5,
				width: 1.0,
				height: 2.0,
				depth: 1.0
			},
			{
				id: id("object.far"),
				kind: EditorObjectGizmoKind.NpcGizmo,
				x: 1.5,
				y: 1.0,
				z: 3.5,
				width: 1.0,
				height: 2.0,
				depth: 1.0
			},
			{
				id: id("object.overlap"),
				kind: EditorObjectGizmoKind.ItemGizmo,
				x: 1.5,
				y: 1.0,
				z: 1.5,
				width: 1.0,
				height: 2.0,
				depth: 1.0
			}
		];
		final objectHit = pickObject(objectGizmos, {x: 1.5, y: 1.0, z: -2.0}, {x: 0.0, y: 0.0, z: 1.0}, 16.0);
		require(objectHit != null && objectHit.id.text() == "object.near" && close(objectHit.distance, 3.0),
			"3D object picking did not choose the nearest authored object with a stable tie");
		require(pickObject(objectGizmos, {x: 5.0, y: 1.0, z: -2.0}, {x: 0.0, y: 0.0, z: 1.0}, 16.0) == null,
			"3D object picking admitted a parallel ray outside every object");
		require(pickObject(objectGizmos, {x: 1.5, y: 1.0, z: -2.0}, {x: 0.0, y: 0.0, z: 1.0}, 2.0) == null,
			"3D object picking ignored the bounded ray distance");
		return 19;
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

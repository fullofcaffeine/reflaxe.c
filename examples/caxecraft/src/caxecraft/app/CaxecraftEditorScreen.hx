package caxecraft.app;

#if c
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipe;
import caxecraft.editor.EditorBuildControls.EditorBuildPointerState;
import caxecraft.editor.EditorBuildControls.EditorBuildTerrainAction;
import caxecraft.editor.EditorBuildControls.nextPointerState;
import caxecraft.editor.EditorBuildControls.moveBuildFocus;
import caxecraft.editor.EditorBuildControls.normalizeBuildFocus;
import caxecraft.editor.EditorBuildControls.normalizeBuildTool;
import caxecraft.editor.EditorBuildControls.terrainAction;
import caxecraft.editor.EditorBuildControls.toolForBuildHotbarSlot;
import caxecraft.editor.EditorBuildControls.usesDirectTerrainControls;
import caxecraft.editor.EditorPackageSession;
import caxecraft.editor.EditorPackageSession.EditorPackageSaveResult;
import caxecraft.editor.EditorPackageSession.editorPackageErrorMessage;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorTerrainRefresh.EditorTerrainRefreshRequest;
import caxecraft.editor.EditorTerrainRefresh.forBatch as terrainRefreshForBatch;
import caxecraft.editor.EditorTerrainRefresh.forCommand as terrainRefreshForCommand;
import caxecraft.editor.EditorEnvironment.EditorEnvironmentControl;
import caxecraft.editor.EditorEnvironment.EditorEnvironmentDirection;
import caxecraft.editor.EditorEnvironment.editEnvironment;
import caxecraft.editor.EditorEnvironment.firstEnvironmentControl;
import caxecraft.editor.EditorEnvironment.moveEnvironmentControl;
import caxecraft.editor.EditorFocus.EditorFocusTarget;
import caxecraft.editor.EditorFocus.initialFocus;
import caxecraft.editor.EditorFocus.moveFocus;
import caxecraft.editor.EditorFlowProjection.EditorZoneRuleProjection;
import caxecraft.editor.EditorObjectDuplicate.duplicateObject;
import caxecraft.editor.EditorObjectPresentation.EditorObjectVisual;
import caxecraft.editor.EditorObjectPresentation.visualFor as objectVisualFor;
import caxecraft.editor.EditorObjectPresentation.visualUsesBillboard;
import caxecraft.editor.EditorPresentation.EditorPresentationSnapshot;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorNodeRef;
import caxecraft.editor.EditorTypes.EditorObservation;
import caxecraft.editor.EditorTypes.EditorQuery;
import caxecraft.editor.EditorTypes.EditorSelection;
import caxecraft.editor.EditorTypes.EditorSelectionResult;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.editor.EditorViewport.EditorViewportLayout;
import caxecraft.editor.EditorViewport.EditorViewportProjection;
import caxecraft.editor.EditorViewport.EditorTool;
import caxecraft.editor.EditorViewport.EditorToolCommandResult;
import caxecraft.editor.EditorViewport.boundsIntersectLayer;
import caxecraft.editor.EditorViewport.clampLayer;
import caxecraft.editor.EditorViewport.commandFor as commandForTool;
import caxecraft.editor.EditorViewport.layout as layoutPlan;
import caxecraft.editor.EditorViewport.paletteCodeAt as paletteCodeAtPlan;
import caxecraft.editor.EditorViewport.pointAt as pointAtPlan;
import caxecraft.editor.EditorViewport.paletteCodeForBlock;
import caxecraft.editor.EditorViewport.projectFromWorld;
import caxecraft.editor.EditorWorldViewport.EditorCameraInput;
import caxecraft.editor.EditorWorldViewport.EditorCameraState;
import caxecraft.editor.EditorWorldViewport.EditorObjectFacing;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmo;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmoKind;
import caxecraft.editor.EditorWorldViewport.EditorWorldHit;
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.editor.EditorWorldViewport.cameraTarget;
import caxecraft.editor.EditorWorldViewport.focusCamera;
import caxecraft.editor.EditorWorldViewport.gizmoIntersectsLayer;
import caxecraft.editor.EditorWorldViewport.paletteCodeAtWorld;
import caxecraft.editor.EditorWorldViewport.pickObject;
import caxecraft.editor.EditorWorldViewport.pickWorld;
import caxecraft.editor.EditorWorldViewport.projectObjects;
import caxecraft.editor.EditorWorldViewport.stepCamera;
import caxecraft.editor.EditorWorldViewport.surfaceTopAt;
import caxecraft.input.NavigationInput.NavigationCommand;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioEnvironment;
import caxecraft.scenario.ScenarioEnvironment.ScenarioHorizonEdge;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioText;
import caxecraft.app.EditorObjectRenderer.drawEditorObject;
import caxecraft.app.EditorObjectRenderer.EditorRenderResources;
import haxe.io.Bytes;
import raygui.GuiListViewState;
import raygui.GuiResult;
import raygui.GuiTextBoxState;
import raygui.Raygui;
import raylib.Camera3D;
import raylib.CameraProjection;
import raylib.Color;
import raylib.KeyboardKey;
import raylib.MouseButton;
import raylib.Raylib;
import raylib.Rectangle;
import raylib.Rlgl;
import raylib.Vector2;
import raylib.Vector3;

/** What the application should do after handling one editor frame. */
enum EditorScreenAction {
	StayInEditor;
	ReturnToTitle;
	StartTestPlay(canonical:Bytes);
}

private enum EditorNotice {
	Ready;
	Valid;
	Invalid;
	Testing;
	Saved;
	SaveFailed;
}

/** One editable view over the same canonical editor draft. */
private enum abstract EditorWorkspaceView(Int) {
	var BuildView = 0;
	var PlanView = 1;
}

/** The exact baseline exists only after the editor opens a valid document. */
/**
 * Native visual editor over the real renderer-independent editor session.
 *
 * This stateful class owns one mutable draft/session and small presentation
 * state. Raygui remains immediate-mode: every frame redraws controls, while
 * `EditorSession` continues to own validation, undo/redo, and the editing
 * lock. The application owns the disposable ordinary-engine runtime. The
 * screen opens a copy of the active level bytes. A cached
 * `EditorWorldProjection` stores exact terrain cells, object gizmos, and the
 * content-logic count. For a playable world shape, Build gives a read-only copy
 * to the ordinary `TerrainRenderer`. Custom-size drafts use a compact overview
 * until they fit a shape that play accepts. Steady frames never serialize the
 * draft or maintain a second editable world.
 *
 * The base-pack IDs and Raylib colors below belong at this Caxecraft
 * composition edge; the reusable editor package knows neither. Build and Plan
 * are two views over one draft, selection, active tool, and history. The first
 * child-facing slice edits terrain and creates checkpoints, pack objects, and
 * one-cell trigger volumes. Creators can move objects by whole cells and rotate
 * transform-backed objects in quarter turns. They can also resize trigger
 * volumes by one cell on each axis. The reducer rejects a size that leaves the
 * finite world. Native source save and bounded horizontal layer controls are
 * available. Flow authoring and cinematic tools remain separate. Test Play
 * uses a disposable ordinary game runtime while this class keeps the exact
 * editor workspace alive.
 */
final class CaxecraftEditorScreen {
	final contentRegistry:RuntimeContentRegistry;
	final uiCatalog:RuntimeUiCatalog;
	final editorPackage:EditorPackageSession;
	final terrainPresentation:EditorTerrainPresentation;
	var session:Null<EditorSession>;
	var notice:EditorNotice;
	var presentationDraft:Null<EditorPresentationSnapshot>;
	var projection:Null<EditorWorldProjection>;
	var planProjection:Null<EditorViewportProjection>;
	var objectGizmos:Array<EditorObjectGizmo>;
	var objectVisuals:Array<EditorObjectVisual>;
	var objectLabels:String;
	var flowRuleCount:Int;
	var zoneRuleLinks:Array<EditorZoneRuleProjection>;
	var camera:Null<EditorCameraState>;
	var selection:Null<VoxelBounds>;
	var focusedControl:EditorFocusTarget;
	var workspaceView:EditorWorkspaceView;
	var buildPointerState:EditorBuildPointerState;
	var editLayerY:Int;
	var activeTool:EditorTool;
	var detailsOpen:Bool;
	var worldListOpen:Bool;
	var environmentPanelOpen:Bool;
	var environmentControl:EditorEnvironmentControl;
	var environment:Null<ScenarioEnvironment>;
	var leavePromptOpen:Bool;
	var previewPoint:Null<VoxelPoint>;
	var previewRevision:Int;
	var previewTool:EditorTool;
	var previewAllowed:Bool;
	var objectList:GuiListViewState;

	#if caxecraft_pilot
	/** Newly dirty terrain chunks from the most recent incremental editor refresh. */
	var pilotTerrainPatchDirtyChunks:Int = 0;

	/** True when the most recent requested voxel patch had to rebuild all terrain. */
	var pilotTerrainPatchFellBack:Bool = false;
	#end

	/**
	 * Owns the temporary native editing bytes for the authored scenario title.
	 *
	 * Raygui edits this buffer during one frame, but `EditorSession` remains the
	 * document owner. Leaving edit mode submits one revision-checked `SetTitle`
	 * command, so validation, canonical CAXEMAP output, undo, and redo all see
	 * the same change. Rejected input is replaced with the accepted draft title.
	 */
	final worldName:Null<GuiTextBoxState>;

	/** Start with the package-backed session opened from the active game generation. */
	public function new(contentRegistry:RuntimeContentRegistry, uiCatalog:RuntimeUiCatalog, editorPackage:EditorPackageSession) {
		this.contentRegistry = contentRegistry;
		this.uiCatalog = uiCatalog;
		this.editorPackage = editorPackage;
		terrainPresentation = new EditorTerrainPresentation();
		session = editorPackage.workspace();
		notice = Ready;
		presentationDraft = null;
		projection = null;
		planProjection = null;
		objectGizmos = [];
		objectVisuals = [];
		objectLabels = "";
		flowRuleCount = 0;
		zoneRuleLinks = [];
		camera = null;
		selection = null;
		focusedControl = initialFocus();
		workspaceView = BuildView;
		buildPointerState = EditorBuildPointerState.Released;
		editLayerY = 0;
		activeTool = SelectTool;
		detailsOpen = false;
		worldListOpen = false;
		environmentPanelOpen = false;
		environmentControl = firstEnvironmentControl();
		environment = null;
		leavePromptOpen = false;
		previewPoint = null;
		previewRevision = -1;
		previewTool = SelectTool;
		previewAllowed = false;
		objectList = new GuiListViewState(-1);
		worldName = GuiTextBoxState.create(64);
		refreshProjection(true, RefreshAllTerrain);
	}

	/**
	 * Draw one responsive editor frame and apply device-neutral navigation.
	 *
	 * The application supplies controller or pilot navigation as one semantic
	 * command. This screen reads keyboard input into the same command set, then
	 * routes both sources through `applyNavigation`.
	 */
	public function draw(locale:LocaleCursor, externalNavigation:NavigationCommand, resources:EditorRenderResources):EditorScreenAction {
		final width = Raylib.GetScreenWidth();
		final height = Raylib.GetScreenHeight();
		if (!Raylib.IsWindowFocused())
			setBuildPointerState(nextPointerState(buildPointerState, workspaceView == BuildView, false, false, false));
		if (saveShortcutPressed())
			requestSave();
		final keyboardNavigation = readKeyboardNavigation();
		final navigation = externalNavigation != NavigationCommand.None ? externalNavigation : keyboardNavigation;
		final navigationAction = applyNavigation(navigation);
		switch navigationAction {
			case StayInEditor:
			case ReturnToTitle | StartTestPlay(_):
				return navigationAction;
		}
		final editedName = worldName;
		if (!leavePromptOpen && (editedName == null || !editedName.isEditing()) && Raylib.IsKeyPressed(KeyboardKey.Backspace))
			deleteSelectedObject();
		Raylib.ClearBackground(Color.rgba(12, 28, 36));
		if (environmentPanelOpen) {
			drawEnvironmentPanel(locale, width, height);
			return StayInEditor;
		}
		final outer = Rectangle.fromFloat(16.0, 16.0, width - 32.0, height - 32.0);
		if (Raygui.WindowBoxString(outer, uiCatalog.text(locale, UiMessage.EditorTitle)).has(GuiResult.Pressed)) {
			focusedControl = EditorFocusTarget.Back;
			final leaveAction = requestLeave();
			switch leaveAction {
				case StayInEditor:
				case ReturnToTitle | StartTestPlay(_):
					return leaveAction;
			}
		}

		final toolbarTop = 58.0;
		if (focusedButtonSized(EditorFocusTarget.Back, 32.0, toolbarTop, 82.0, 38.0, uiCatalog.text(locale, UiMessage.EditorBack))) {
			final leaveAction = requestLeave();
			switch leaveAction {
				case StayInEditor:
				case ReturnToTitle | StartTestPlay(_):
					return leaveAction;
			}
		}
		if (focusedButtonSized(EditorFocusTarget.Save, 122.0, toolbarTop, 72.0, 38.0, uiCatalog.text(locale, UiMessage.EditorSave)))
			requestSave();
		final historyLeft = width >= 1180 ? 402.0 : 328.0;
		final name = worldName;
		if (name != null) {
			final nameLeft = 206.0;
			final nameWidth = historyLeft - nameLeft - 16.0;
			final result = name.draw(Rectangle.fromFloat(nameLeft, toolbarTop, nameWidth, 38.0));
			if (result.has(GuiResult.Pressed)) {
				focusedControl = EditorFocusTarget.WorldName;
				if (!name.isEditing())
					commitWorldName(name.text());
			}
			drawFocusRing(EditorFocusTarget.WorldName, Std.int(nameLeft), Std.int(toolbarTop), Std.int(nameWidth), 38);
		}

		if (focusedButtonSized(EditorFocusTarget.Undo, historyLeft, toolbarTop, 88.0, 38.0, uiCatalog.text(locale, UiMessage.EditorUndo)))
			undo();
		if (focusedButtonSized(EditorFocusTarget.Redo, historyLeft + 96.0, toolbarTop, 88.0, 38.0, uiCatalog.text(locale, UiMessage.EditorRedo)))
			redo();

		final viewLeft = Std.int(width * 0.5) - 104.0;
		if (focusedButtonSized(EditorFocusTarget.Build, viewLeft, toolbarTop, 100.0, 38.0, uiCatalog.text(locale, UiMessage.EditorBuild)))
			setWorkspaceView(BuildView);
		if (focusedButtonSized(EditorFocusTarget.Plan, viewLeft + 108.0, toolbarTop, 100.0, 38.0, uiCatalog.text(locale, UiMessage.EditorPlan)))
			setWorkspaceView(PlanView);
		drawActiveControl(workspaceView == BuildView, Std.int(viewLeft), Std.int(toolbarTop), 100, 38);
		drawActiveControl(workspaceView == PlanView, Std.int(viewLeft + 108.0), Std.int(toolbarTop), 100, 38);

		final playWidth = 136.0;
		final playLeft = width - playWidth - 32.0;
		final environmentWidth = 116.0;
		final environmentLeft = playLeft - environmentWidth - 12.0;
		if (focusedButtonSized(EditorFocusTarget.Environment, environmentLeft, toolbarTop, environmentWidth, 38.0,
			uiCatalog.text(locale, UiMessage.EditorEnvironment)))
			openEnvironmentPanel();
		if (focusedButtonSized(EditorFocusTarget.Play, playLeft, toolbarTop - 2.0, playWidth, 42.0, uiCatalog.text(locale, UiMessage.EditorTest))) {
			final testAction = requestTestPlay();
			switch testAction {
				case StayInEditor:
				case ReturnToTitle | StartTestPlay(_):
					return testAction;
			}
		}

		final canvasTop = 116;
		final shelfTop = height - 154;
		final inspectorVisible = selection != null || selectedObjectIndex() >= 0 || worldListOpen;
		final inspectorWidth = inspectorVisible && width >= 900 ? 250 : 0;
		final canvasLeft = 32;
		final canvasWidth = width - 64 - inspectorWidth - (inspectorWidth > 0 ? 12 : 0);
		final canvasHeight = shelfTop - canvasTop - 12;
		Raygui.PanelString(Rectangle.fromFloat(canvasLeft, canvasTop, canvasWidth, canvasHeight), uiCatalog.text(locale, UiMessage.EditorCanvasHelp));
		drawLayerControls(locale, canvasLeft + canvasWidth - 190, canvasTop + 4);
		final innerLeft = canvasLeft + 12;
		final innerTop = canvasTop + 36;
		final innerWidth = canvasWidth - 24;
		final innerHeight = canvasHeight - 48;
		if (workspaceView == BuildView)
			drawWorldViewport(innerLeft, innerTop, innerWidth, innerHeight, resources);
		else
			drawPlanViewport(innerLeft, innerTop, innerWidth, innerHeight);
		if (inspectorWidth > 0)
			drawInspector(locale, canvasLeft + canvasWidth + 12, canvasTop, inspectorWidth, canvasHeight);
		drawCreationShelf(locale, 32, shelfTop, width - 64, 112);

		if (environmentPanelOpen) {
			drawEnvironmentPanel(locale, width, height);
			return StayInEditor;
		}
		if (leavePromptOpen)
			return drawLeavePrompt(locale, width, height);
		return StayInEditor;
	}

	/**
	 * Draw one sized button, remember pointer focus, and paint keyboard focus.
	 *
	 * Raygui still owns hit testing for the immediate native control. This
	 * screen owns semantic focus, so mouse and keyboard routes converge before
	 * the existing editor action runs.
	 */
	function focusedButtonSized(target:EditorFocusTarget, x:Float, y:Float, width:Float, height:Float, text:String):Bool {
		final pressed = Raygui.ButtonString(Rectangle.fromFloat(x, y, width, height), text).has(GuiResult.Pressed);
		if (pressed)
			focusedControl = target;
		drawFocusRing(target, Std.int(x), Std.int(y), Std.int(width), Std.int(height));
		return pressed;
	}

	/** Draw bounded layer controls in the canvas title bar. */
	function drawLayerControls(locale:LocaleCursor, left:Int, top:Int):Void {
		if (focusedButtonSized(EditorFocusTarget.PreviousLayer, left, top, 34.0, 28.0, "-"))
			selectEditLayer(editLayerY - 1);
		final height = switch projection {
			case null: 0;
			case value: value.height;
		};
		final label = '${uiCatalog.text(locale, UiMessage.EditorLayer)} ${editLayerY + 1} / $height';
		Raylib.DrawTextString(label, left + 42, top + 7, 14, CaxecraftPalette.hudText());
		if (focusedButtonSized(EditorFocusTarget.NextLayer, left + 154, top, 34.0, 28.0, "+"))
			selectEditLayer(editLayerY + 1);
	}

	/** Draw a clear second border around a selected view or creation card. */
	static function drawActiveControl(active:Bool, x:Int, y:Int, width:Int, height:Int):Void {
		if (!active)
			return;
		final color = CaxecraftPalette.selection();
		Raylib.DrawRectangleLines(x + 2, y + 2, width - 4, height - 4, color);
		Raylib.DrawRectangleLines(x + 3, y + 3, width - 6, height - 6, color);
	}

	/** Draw the large creation actions and small workspace disclosures. */
	function drawCreationShelf(locale:LocaleCursor, left:Int, top:Int, width:Int, height:Int):Void {
		Raygui.PanelString(Rectangle.fromFloat(left, top, width, height), "");
		final cardTop = top + 12;
		final disclosureWidth = 150.0;
		final disclosureLeft = left + width - Std.int(disclosureWidth) - 12;
		final cardGap = 10;
		final cardCount = workspaceView == BuildView ? 5 : 6;
		final cardWidth = Std.int((disclosureLeft - 10 - (left + 12) - cardGap * (cardCount - 1)) / cardCount);
		drawToolCard(locale, EditorFocusTarget.SelectTool, EditorTool.SelectTool, 1, left + 12, cardTop, cardWidth, 68, UiMessage.EditorSelect,
			Color.rgba(84, 191, 205));
		drawToolCard(locale, EditorFocusTarget.GroundTool, EditorTool.PaintTool, 2, left + 12 + (cardWidth + cardGap), cardTop, cardWidth, 68,
			UiMessage.EditorGround, Color.rgba(111, 174, 91));
		if (workspaceView == PlanView)
			drawToolCard(locale, EditorFocusTarget.EraseTool, EditorTool.EraseTool, 3, left + 12 + (cardWidth + cardGap) * 2, cardTop, cardWidth, 68,
				UiMessage.EditorErase, Color.rgba(218, 103, 78));
		final checkpointSlot = workspaceView == BuildView ? 3 : 4;
		final catalogSlot = checkpointSlot + 1;
		final triggerSlot = catalogSlot + 1;
		drawToolCard(locale, EditorFocusTarget.CheckpointTool, EditorTool.CheckpointTool, checkpointSlot,
			left + 12 + (cardWidth + cardGap) * (checkpointSlot - 1), cardTop, cardWidth, 68, UiMessage.EditorCheckpoint, Color.rgba(76, 209, 198));
		final recipe = contentRegistry.editorObjectAt(0);
		if (recipe != null)
			drawToolCardText(EditorFocusTarget.CatalogObjectTool, EditorTool.CatalogObjectTool, catalogSlot,
				left + 12 + (cardWidth + cardGap) * (catalogSlot - 1), cardTop, cardWidth, 68, locale == Locale0 ? recipe.labelEn : recipe.labelEsMx,
				Color.rgba(226, 151, 72));
		drawToolCard(locale, EditorFocusTarget.TriggerZoneTool, EditorTool.TriggerZoneTool, triggerSlot,
			left + 12 + (cardWidth + cardGap) * (triggerSlot - 1), cardTop, cardWidth, 68, UiMessage.EditorTrigger, Color.rgba(210, 105, 230));

		if (focusedButtonSized(EditorFocusTarget.WorldList, disclosureLeft, cardTop, disclosureWidth, 30.0, uiCatalog.text(locale, UiMessage.EditorWorldList)))
			worldListOpen = !worldListOpen;
		if (focusedButtonSized(EditorFocusTarget.MoreDetails, disclosureLeft, cardTop + 38.0, disclosureWidth, 30.0,
			uiCatalog.text(locale, UiMessage.EditorMoreDetails))
			&& (selection != null || selectedObjectIndex() >= 0))
			detailsOpen = !detailsOpen;
		drawActiveControl(worldListOpen, disclosureLeft, cardTop, Std.int(disclosureWidth), 30);
		drawActiveControl(detailsOpen
			&& (selection != null || selectedObjectIndex() >= 0), disclosureLeft, cardTop + 38, Std.int(disclosureWidth), 30);

		final status = switch notice {
			case Ready: UiMessage.EditorReady;
			case Valid: UiMessage.EditorValid;
			case Invalid: UiMessage.EditorInvalid;
			case Testing: UiMessage.EditorTesting;
			case Saved: UiMessage.EditorSaved;
			case SaveFailed: UiMessage.EditorSaveFailed;
		};
		Raylib.DrawTextString(uiCatalog.text(locale, status), left + 12, top + height - 24, 14,
			notice == Invalid ? Color.rgba(255, 154, 112) : CaxecraftPalette.hudText());
	}

	/** Draw one large tool card with a non-text color mark and selected border. */
	function drawToolCard(locale:LocaleCursor, focus:EditorFocusTarget, tool:EditorTool, slot:Int, left:Int, top:Int, width:Int, height:Int,
			message:UiMessage, color:Color):Void {
		drawToolCardText(focus, tool, slot, left, top, width, height, uiCatalog.text(locale, message), color);
	}

	/** Draw one pack-labeled tool card without moving content names into the UI catalog. */
	function drawToolCardText(focus:EditorFocusTarget, tool:EditorTool, slot:Int, left:Int, top:Int, width:Int, height:Int, text:String, color:Color):Void {
		final pressed = Raygui.ButtonString(Rectangle.fromFloat(left, top, width, height), "").has(GuiResult.Pressed);
		if (pressed) {
			focusedControl = focus;
			setActiveTool(tool);
		}
		Raylib.DrawRectangle(left + 12, top + 14, 36, 36, color);
		Raylib.DrawRectangleLines(left + 12, top + 14, 36, 36, CaxecraftPalette.hudText());
		Raylib.DrawTextString(Std.string(slot), left + 25, top + 23, 18, Color.rgba(10, 24, 30));
		Raylib.DrawTextString(text, left + 58, top + 23, 17, CaxecraftPalette.hudText());
		drawFocusRing(focus, left, top, width, height);
		drawActiveControl(activeTool == tool, left, top, width, height);
	}

	/** Show only the properties and authored records that help the current task. */
	function drawInspector(locale:LocaleCursor, left:Int, top:Int, width:Int, height:Int):Void {
		Raygui.PanelString(Rectangle.fromFloat(left, top, width, height), uiCatalog.text(locale, UiMessage.EditorMoreDetails));
		var cursorTop = top + 42;
		final selected = selection;
		if (selected != null) {
			final point = selected.origin;
			Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorCoordinates), left + 14, cursorTop, 14, Color.rgba(126, 205, 209));
			cursorTop += 22;
			Raylib.DrawTextString('${point.x}, ${point.y}, ${point.z}', left + 14, cursorTop, 18, CaxecraftPalette.hudText());
			cursorTop += 34;
			final current = projection;
			final paletteCode = current == null ? -1 : paletteCodeAtWorld(current, point.x, point.y, point.z);
			Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorMaterial), left + 14, cursorTop, 14, Color.rgba(126, 205, 209));
			cursorTop += 22;
			Raylib.DrawTextString(paletteCode < 0 ? "-" : '$paletteCode', left + 14, cursorTop, 18, CaxecraftPalette.hudText());
			cursorTop += 34;
			if (detailsOpen) {
				Raylib.DrawTextString('${selected.size.width} x ${selected.size.height} x ${selected.size.depth}', left + 14, cursorTop, 15,
					CaxecraftPalette.hudText());
				cursorTop += 28;
				Raylib.DrawTextString('${objectGizmos.length} / $flowRuleCount', left + 14, cursorTop, 15, CaxecraftPalette.hudText());
				cursorTop += 34;
			}
		}
		final objectIndex = selectedObjectIndex();
		if (objectIndex >= 0) {
			final gizmo = objectGizmos[objectIndex];
			Raylib.DrawTextString(gizmo.id.text(), left + 14, cursorTop, 16, CaxecraftPalette.selection());
			cursorTop += 28;
			Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorCoordinates), left + 14, cursorTop, 14, Color.rgba(126, 205, 209));
			cursorTop += 22;
			Raylib.DrawTextString('${Std.int(gizmo.x)}, ${Std.int(gizmo.y)}, ${Std.int(gizmo.z)}', left + 14, cursorTop, 18, CaxecraftPalette.hudText());
			cursorTop += 34;
			drawObjectMoveControls(left + 14, cursorTop, width - 28);
			cursorTop += 34;
			switch gizmo.facing {
				case ObjectYaw(yawDegrees):
					drawObjectRotationControls(left + 14, cursorTop, width - 28, yawDegrees);
					cursorTop += 34;
				case NoObjectFacing:
			}
			if (gizmo.kind == TriggerZoneGizmo) {
				drawTriggerResizeControls(left + 14, cursorTop, width - 28, {
					width: Std.int(gizmo.width),
					height: Std.int(gizmo.height),
					depth: Std.int(gizmo.depth)
				});
				cursorTop += 34;
			}
			final actionWidth = Std.int((width - 32) / 2);
			if (Raygui.ButtonString(Rectangle.fromFloat(left + 14, cursorTop, actionWidth, 30), uiCatalog.text(locale, UiMessage.EditorDuplicate))
				.has(GuiResult.Pressed))
				duplicateSelectedObject();
			if (Raygui.ButtonString(Rectangle.fromFloat(left + 18 + actionWidth, cursorTop, actionWidth, 30), uiCatalog.text(locale, UiMessage.EditorDelete))
				.has(GuiResult.Pressed))
				deleteSelectedObject();
			cursorTop += 38;
			if (detailsOpen) {
				Raylib.DrawTextString('${gizmo.width} x ${gizmo.height} x ${gizmo.depth}', left + 14, cursorTop, 15, CaxecraftPalette.hudText());
				cursorTop += 28;
			}
		}
		if (worldListOpen) {
			Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorWorldList), left + 14, cursorTop, 15, CaxecraftPalette.selection());
			cursorTop += 24;
			final listHeight = height - (cursorTop - top) - 14;
			if (listHeight > 40) {
				final result = objectList.drawString(Rectangle.fromFloat(left + 12, cursorTop, width - 24, listHeight), objectLabels);
				if (result.has(GuiResult.Pressed)) {
					focusedControl = EditorFocusTarget.WorldList;
					selectObjectFromWorldList();
				}
			}
		}
	}

	/** Draw every admitted environment field in one visible two-column modal. */
	function drawEnvironmentPanel(locale:LocaleCursor, width:Int, height:Int):Void {
		Raylib.DrawRectangle(0, 0, width, height, Color.rgba(4, 10, 14, 230));
		final panelWidth = width >= 900 ? 820 : width - 48;
		final panelHeight = height >= 640 ? 560 : height - 48;
		final left = Std.int((width - panelWidth) / 2);
		final top = Std.int((height - panelHeight) / 2);
		Raygui.PanelString(Rectangle.fromFloat(left, top, panelWidth, panelHeight), uiCatalog.text(locale, UiMessage.EditorEnvironment));
		final current = environment;
		if (current != null) {
			Raylib.DrawRectangle(left + panelWidth - 58, top + 14, 28, 20, Color.rgbaClamped(current.sky.red, current.sky.green, current.sky.blue));
			Raylib.DrawRectangleLines(left + panelWidth - 58, top + 14, 28, 20, CaxecraftPalette.hudText());
		}
		final columnGap = 24;
		final columnWidth = Std.int((panelWidth - 48 - columnGap) / 2);
		for (index in 0...18) {
			final control = environmentControlAt(index);
			final column = index < 9 ? 0 : 1;
			final row = index < 9 ? index : index - 9;
			final rowLeft = left + 24 + column * (columnWidth + columnGap);
			final rowTop = top + 54 + row * 50;
			if (control == EditorEnvironmentControl.Done) {
				if (Raygui.ButtonString(Rectangle.fromFloat(rowLeft, rowTop + 4, columnWidth, 36), uiCatalog.text(locale, UiMessage.EditorEnvironmentDone))
					.has(GuiResult.Pressed)) {
					environmentControl = control;
					closeEnvironmentPanel();
				}
				drawEnvironmentFocusRing(control, rowLeft, rowTop + 4, columnWidth, 36);
			} else
				drawEnvironmentRow(locale, control, rowLeft, rowTop, columnWidth);
		}
	}

	/** Draw one label, current value, and pointer decrement/increment actions. */
	function drawEnvironmentRow(locale:LocaleCursor, control:EditorEnvironmentControl, left:Int, top:Int, width:Int):Void {
		Raylib.DrawTextString(environmentControlLabel(locale, control), left + 4, top + 4, 14, CaxecraftPalette.hudText());
		final buttonWidth = 32;
		final valueWidth = 72;
		final decreaseLeft = left + width - buttonWidth * 2 - valueWidth - 8;
		if (Raygui.ButtonString(Rectangle.fromFloat(decreaseLeft, top, buttonWidth, 34), "-").has(GuiResult.Pressed)) {
			environmentControl = control;
			applyEnvironmentControl(control, EditorEnvironmentDirection.Decrease);
		}
		Raylib.DrawTextString(environmentControlValue(locale, control), decreaseLeft + buttonWidth + 6, top + 8, 15, CaxecraftPalette.selection());
		if (Raygui.ButtonString(Rectangle.fromFloat(left + width - buttonWidth, top, buttonWidth, 34), "+").has(GuiResult.Pressed)) {
			environmentControl = control;
			applyEnvironmentControl(control, EditorEnvironmentDirection.Increase);
		}
		drawEnvironmentFocusRing(control, left, top, width, 34);
	}

	/** Draw modal focus independently from the controls behind the overlay. */
	function drawEnvironmentFocusRing(control:EditorEnvironmentControl, left:Int, top:Int, width:Int, height:Int):Void {
		if (environmentControl != control)
			return;
		final color = CaxecraftPalette.editorFocus();
		Raylib.DrawRectangleLines(left - 2, top - 2, width + 4, height + 4, color);
		Raylib.DrawRectangleLines(left - 3, top - 3, width + 6, height + 6, color);
	}

	/** Localized label for one closed environment field. */
	function environmentControlLabel(locale:LocaleCursor, control:EditorEnvironmentControl):String {
		final sky = uiCatalog.text(locale, UiMessage.EditorEnvironmentSky);
		final sun = uiCatalog.text(locale, UiMessage.EditorEnvironmentSun);
		final clouds = uiCatalog.text(locale, UiMessage.EditorEnvironmentClouds);
		return switch control {
			case Enabled: uiCatalog.text(locale, UiMessage.EditorEnvironmentEnabled);
			case SkyRed: '$sky R';
			case SkyGreen: '$sky G';
			case SkyBlue: '$sky B';
			case SunEnabled: sun;
			case SunX: '$sun X';
			case SunY: '$sun Y';
			case SunZ: '$sun Z';
			case SunRadius: '$sun ${uiCatalog.text(locale, UiMessage.EditorEnvironmentRadius)}';
			case CloudCount: '$clouds #';
			case CloudSpeed: '$clouds >>';
			case CloudSeed: '$clouds ${uiCatalog.text(locale, UiMessage.EditorEnvironmentSeed)}';
			case NorthEdge: uiCatalog.text(locale, UiMessage.EditorEnvironmentNorth);
			case SouthEdge: uiCatalog.text(locale, UiMessage.EditorEnvironmentSouth);
			case EastEdge: uiCatalog.text(locale, UiMessage.EditorEnvironmentEast);
			case WestEdge: uiCatalog.text(locale, UiMessage.EditorEnvironmentWest);
			case ContinueWater: uiCatalog.text(locale, UiMessage.EditorEnvironmentWater);
			case Done: uiCatalog.text(locale, UiMessage.EditorEnvironmentDone);
		};
	}

	/** Current authored value shown beside one environment field. */
	function environmentControlValue(locale:LocaleCursor, control:EditorEnvironmentControl):String {
		final current = environment;
		if (control == EditorEnvironmentControl.Enabled)
			return onOff(locale, current != null);
		if (current == null)
			return "-";
		return switch control {
			case Enabled: onOff(locale, true);
			case SkyRed: '${current.sky.red}';
			case SkyGreen: '${current.sky.green}';
			case SkyBlue: '${current.sky.blue}';
			case SunEnabled: onOff(locale, current.sun != null);
			case SunX: current.sun == null ? "-" : '${current.sun.x}';
			case SunY: current.sun == null ? "-" : '${current.sun.y}';
			case SunZ: current.sun == null ? "-" : '${current.sun.z}';
			case SunRadius: current.sun == null ? "-" : '${current.sun.radiusMilli}';
			case CloudCount: '${current.clouds.count}';
			case CloudSpeed: '${current.clouds.speedMilli}';
			case CloudSeed: '${current.clouds.seed}';
			case NorthEdge: onOff(locale, hasEnvironmentEdge(current.edges, North));
			case SouthEdge: onOff(locale, hasEnvironmentEdge(current.edges, South));
			case EastEdge: onOff(locale, hasEnvironmentEdge(current.edges, East));
			case WestEdge: onOff(locale, hasEnvironmentEdge(current.edges, West));
			case ContinueWater: onOff(locale, current.continueWater);
			case Done: "";
		};
	}

	/** Localized Boolean value shared by environment toggles. */
	function onOff(locale:LocaleCursor, enabled:Bool):String
		return uiCatalog.text(locale, enabled ? UiMessage.EditorEnvironmentOn : UiMessage.EditorEnvironmentOff);

	/** True when the current environment includes one closed finite-world edge. */
	static function hasEnvironmentEdge(edges:Array<ScenarioHorizonEdge>, expected:ScenarioHorizonEdge):Bool {
		for (edge in edges)
			if (edge == expected)
				return true;
		return false;
	}

	/** Map each visible row without converting an unchecked integer to an enum. */
	static function environmentControlAt(index:Int):EditorEnvironmentControl {
		return switch index {
			case 0: Enabled;
			case 1: SkyRed;
			case 2: SkyGreen;
			case 3: SkyBlue;
			case 4: SunEnabled;
			case 5: SunX;
			case 6: SunY;
			case 7: SunZ;
			case 8: SunRadius;
			case 9: CloudCount;
			case 10: CloudSpeed;
			case 11: CloudSeed;
			case 12: NorthEdge;
			case 13: SouthEdge;
			case 14: EastEdge;
			case 15: WestEdge;
			case 16: ContinueWater;
			case _: Done;
		};
	}

	/** Draw six compact axis controls for the selected authored object. */
	function drawObjectMoveControls(left:Int, top:Int, width:Int):Void {
		final gap = 4;
		final buttonWidth = Std.int((width - gap * 5) / 6);
		moveObjectButton("X-", left, top, buttonWidth, {x: -1, y: 0, z: 0});
		moveObjectButton("X+", left + buttonWidth + gap, top, buttonWidth, {x: 1, y: 0, z: 0});
		moveObjectButton("Y-", left + (buttonWidth + gap) * 2, top, buttonWidth, {x: 0, y: -1, z: 0});
		moveObjectButton("Y+", left + (buttonWidth + gap) * 3, top, buttonWidth, {x: 0, y: 1, z: 0});
		moveObjectButton("Z-", left + (buttonWidth + gap) * 4, top, buttonWidth, {x: 0, y: 0, z: -1});
		moveObjectButton("Z+", left + (buttonWidth + gap) * 5, top, buttonWidth, {x: 0, y: 0, z: 1});
	}

	/** Submit one axis button without keeping widget-local movement state. */
	function moveObjectButton(label:String, left:Int, top:Int, width:Int, delta:VoxelPoint):Void {
		if (Raygui.ButtonString(Rectangle.fromFloat(left, top, width, 26), label).has(GuiResult.Pressed))
			moveSelectedObject(delta);
	}

	/** Show the current facing direction between two quarter-turn actions. */
	function drawObjectRotationControls(left:Int, top:Int, width:Int, yawDegrees:Int):Void {
		final gap = 4;
		final valueWidth = 70;
		final buttonWidth = Std.int((width - valueWidth - gap * 2) / 2);
		if (Raygui.ButtonString(Rectangle.fromFloat(left, top, buttonWidth, 26), "-90").has(GuiResult.Pressed))
			rotateSelectedObject(-90);
		Raylib.DrawTextString('${yawDegrees} deg', left + buttonWidth + gap + 8, top + 6, 14, CaxecraftPalette.selection());
		if (Raygui.ButtonString(Rectangle.fromFloat(left + buttonWidth + gap * 2 + valueWidth, top, buttonWidth, 26), "+90").has(GuiResult.Pressed))
			rotateSelectedObject(90);
	}

	/** Draw exact width, height, and depth actions only for a trigger volume. */
	function drawTriggerResizeControls(left:Int, top:Int, width:Int, size:VoxelSize):Void {
		final gap = 4;
		final buttonWidth = Std.int((width - gap * 5) / 6);
		resizeTriggerButton("W-", left, top, buttonWidth, {width: size.width - 1, height: size.height, depth: size.depth});
		resizeTriggerButton("W+", left + buttonWidth + gap, top, buttonWidth, {width: size.width + 1, height: size.height, depth: size.depth});
		resizeTriggerButton("H-", left + (buttonWidth + gap) * 2, top, buttonWidth, {width: size.width, height: size.height - 1, depth: size.depth});
		resizeTriggerButton("H+", left + (buttonWidth + gap) * 3, top, buttonWidth, {width: size.width, height: size.height + 1, depth: size.depth});
		resizeTriggerButton("D-", left + (buttonWidth + gap) * 4, top, buttonWidth, {width: size.width, height: size.height, depth: size.depth - 1});
		resizeTriggerButton("D+", left + (buttonWidth + gap) * 5, top, buttonWidth, {width: size.width, height: size.height, depth: size.depth + 1});
	}

	/** Submit one exact trigger size without keeping widget-local object state. */
	function resizeTriggerButton(label:String, left:Int, top:Int, width:Int, target:VoxelSize):Void {
		if (Raygui.ButtonString(Rectangle.fromFloat(left, top, width, 26), label).has(GuiResult.Pressed))
			resizeSelectedTrigger(target);
	}

	/** Draw a modal leave decision because this editor does not yet claim Save. */
	function drawLeavePrompt(locale:LocaleCursor, width:Int, height:Int):EditorScreenAction {
		Raylib.DrawRectangle(0, 0, width, height, Color.rgba(4, 10, 14, 210));
		final panelWidth = width >= 700 ? 560 : width - 80;
		final panelHeight = 176;
		final left = Std.int((width - panelWidth) / 2);
		final top = Std.int((height - panelHeight) / 2);
		Raygui.PanelString(Rectangle.fromFloat(left, top, panelWidth, panelHeight), uiCatalog.text(locale, UiMessage.EditorTitle));
		Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorUnsavedChanges), left + 28, top + 52, 20, CaxecraftPalette.hudText());
		if (focusedButtonSized(EditorFocusTarget.KeepEditing, left + 28, top + 106, 196.0, 40.0, uiCatalog.text(locale, UiMessage.EditorKeepEditing))) {
			leavePromptOpen = false;
			focusedControl = EditorFocusTarget.Back;
			return StayInEditor;
		}
		if (focusedButtonSized(EditorFocusTarget.LeaveWithoutSaving, left + panelWidth - 264, top + 106, 236.0, 40.0,
			uiCatalog.text(locale, UiMessage.EditorLeaveWithoutSaving)))
			return ReturnToTitle;
		return StayInEditor;
	}

	/**
	 * Read one device-neutral keyboard command without stealing text-box input.
	 *
	 * Tab moves forward and Shift-Tab moves backward. Enter or Space activates
	 * the focused control. While the World Name buffer is editing, Raygui keeps
	 * those keys so Enter can finish text entry instead of pressing a toolbar
	 * action in the same frame.
	 */
	function readKeyboardNavigation():NavigationCommand {
		final name = worldName;
		if (name != null && name.isEditing())
			return NavigationCommand.None;
		if (environmentPanelOpen) {
			if (Raylib.IsKeyPressed(KeyboardKey.Up))
				return NavigationCommand.Up;
			if (Raylib.IsKeyPressed(KeyboardKey.Down))
				return NavigationCommand.Down;
			if (Raylib.IsKeyPressed(KeyboardKey.Left))
				return NavigationCommand.Left;
			if (Raylib.IsKeyPressed(KeyboardKey.Right))
				return NavigationCommand.Right;
		}
		if (Raylib.IsKeyPressed(KeyboardKey.Tab)) {
			final backward = Raylib.IsKeyDown(KeyboardKey.LeftShift) || Raylib.IsKeyDown(KeyboardKey.RightShift);
			return backward ? NavigationCommand.Up : NavigationCommand.Down;
		}
		if (Raylib.IsKeyPressed(KeyboardKey.Escape))
			return NavigationCommand.Cancel;
		if (Raylib.IsKeyPressed(KeyboardKey.Enter) || Raylib.IsKeyPressed(KeyboardKey.Space))
			return NavigationCommand.Confirm;
		return NavigationCommand.None;
	}

	/** True for one platform save chord without consuming ordinary text input. */
	function saveShortcutPressed():Bool {
		if (!Raylib.IsKeyPressed(KeyboardKey.S))
			return false;
		return Raylib.IsKeyDown(KeyboardKey.LeftControl)
			|| Raylib.IsKeyDown(KeyboardKey.RightControl)
			|| Raylib.IsKeyDown(KeyboardKey.LeftSuper)
			|| Raylib.IsKeyDown(KeyboardKey.RightSuper);
	}

	/**
	 * Apply one navigation command to the editor's existing focus and actions.
	 *
	 * Up/left and down/right traverse the same cyclic order used by Tab.
	 * Confirm invokes the focused action, and Cancel returns to the title
	 * through the same typed screen transition as the keyboard Escape key.
	 * Keyboard, controller, and pilot commands all enter this one handler.
	 */
	public function applyNavigation(command:NavigationCommand):EditorScreenAction {
		if (command == NavigationCommand.Cancel && buildPointerState == EditorBuildPointerState.Captured) {
			setBuildPointerState(nextPointerState(buildPointerState, workspaceView == BuildView, true, false, true));
			return StayInEditor;
		}
		if (environmentPanelOpen) {
			switch command {
				case Up:
					environmentControl = moveEnvironmentControl(environmentControl, EditorEnvironmentDirection.Decrease);
				case Down:
					environmentControl = moveEnvironmentControl(environmentControl, EditorEnvironmentDirection.Increase);
				case Left:
					applyEnvironmentControl(environmentControl, EditorEnvironmentDirection.Decrease);
				case Right:
					applyEnvironmentControl(environmentControl, EditorEnvironmentDirection.Increase);
				case Confirm:
					if (environmentControl == EditorEnvironmentControl.Done)
						closeEnvironmentPanel();
					else
						applyEnvironmentControl(environmentControl, EditorEnvironmentDirection.Increase);
				case Cancel:
					closeEnvironmentPanel();
				case None:
			}
			return StayInEditor;
		}
		if (leavePromptOpen) {
			switch command {
				case Up | Left | Right | Down:
					focusedControl = focusedControl == EditorFocusTarget.KeepEditing ? EditorFocusTarget.LeaveWithoutSaving : EditorFocusTarget.KeepEditing;
				case Confirm:
					return activateFocusedControl();
				case Cancel:
					leavePromptOpen = false;
					focusedControl = EditorFocusTarget.Back;
				case None:
			}
			return StayInEditor;
		}
		switch command {
			case Up | Left:
				focusedControl = workspaceView == BuildView ? moveBuildFocus(focusedControl, Backward) : moveFocus(focusedControl, Backward);
			case Right | Down:
				focusedControl = workspaceView == BuildView ? moveBuildFocus(focusedControl, Forward) : moveFocus(focusedControl, Forward);
			case Confirm:
				return activateFocusedControl();
			case Cancel:
				return cancelEditorAction();
			case None:
		}
		return StayInEditor;
	}

	/** Draw a two-line high-contrast ring around the current semantic target. */
	function drawFocusRing(target:EditorFocusTarget, x:Int, y:Int, width:Int, height:Int):Void {
		if (focusedControl != target)
			return;
		final color = CaxecraftPalette.editorFocus();
		Raylib.DrawRectangleLines(x - 2, y - 2, width + 4, height + 4, color);
		Raylib.DrawRectangleLines(x - 3, y - 3, width + 6, height + 6, color);
	}

	/**
	 * Route one semantic activation to the action named by current focus.
	 *
	 * Keyboard, and the deterministic graphical pilot, enter through this one
	 * exhaustive switch. Pointer clicks still call the same small action
	 * methods after Raygui hit testing. Adding a focus target therefore cannot
	 * silently leave keyboard activation without an owner.
	 */
	function activateFocusedControl():EditorScreenAction {
		switch focusedControl {
			case Back:
				return requestLeave();
			case Save:
				requestSave();
			case WorldName:
				final name = worldName;
				if (name != null)
					name.setEditing(true);
			case Undo:
				undo();
			case Redo:
				redo();
			case Build:
				setWorkspaceView(BuildView);
			case Plan:
				setWorkspaceView(PlanView);
			case PreviousLayer:
				selectEditLayer(editLayerY - 1);
			case NextLayer:
				selectEditLayer(editLayerY + 1);
			case Environment:
				openEnvironmentPanel();
			case Play:
				return requestTestPlay();
			case SelectTool:
				setActiveTool(EditorTool.SelectTool);
			case GroundTool:
				setActiveTool(EditorTool.PaintTool);
			case EraseTool:
				setActiveTool(EditorTool.EraseTool);
			case CheckpointTool:
				setActiveTool(EditorTool.CheckpointTool);
			case CatalogObjectTool:
				setActiveTool(EditorTool.CatalogObjectTool);
			case TriggerZoneTool:
				setActiveTool(EditorTool.TriggerZoneTool);
			case MoreDetails:
				detailsOpen = !detailsOpen;
			case WorldList:
				worldListOpen = !worldListOpen;
			case KeepEditing:
				leavePromptOpen = false;
				focusedControl = EditorFocusTarget.Back;
			case LeaveWithoutSaving:
				return ReturnToTitle;
		}
		return StayInEditor;
	}

	/**
	 * Publish the current draft and update the clean baseline after success.
	 *
	 * A save first commits the temporary title buffer. Package validation and
	 * publication then run through `EditorPackageSession`. Rejection keeps the
	 * draft, history, and previous clean baseline available for another attempt.
	 */
	function requestSave():Bool {
		final current = session;
		if (current == null) {
			notice = SaveFailed;
			return false;
		}
		final name = worldName;
		if (name != null && name.isEditing()) {
			name.setEditing(false);
			if (!commitWorldName(name.text())) {
				notice = SaveFailed;
				return false;
			}
		}
		return switch editorPackage.save(current.revision()) {
			case EditorPackageSaved(_, _, warnings):
				for (warning in warnings)
					Sys.println('caxecraft: editor save cleanup warning: $warning');
				leavePromptOpen = false;
				focusedControl = EditorFocusTarget.Save;
				notice = Saved;
				true;
			case EditorPackageSaveRejected(error):
				Sys.println('caxecraft: editor save rejected: ${editorPackageErrorMessage(error)}');
				notice = SaveFailed;
				false;
		};
	}

	/** Close the nearest presentation layer before offering to leave the draft. */
	function cancelEditorAction():EditorScreenAction {
		if (environmentPanelOpen) {
			closeEnvironmentPanel();
			return StayInEditor;
		}
		if (activeTool != EditorTool.SelectTool) {
			setActiveTool(EditorTool.SelectTool);
			return StayInEditor;
		}
		if (detailsOpen) {
			detailsOpen = false;
			return StayInEditor;
		}
		if (worldListOpen) {
			worldListOpen = false;
			return StayInEditor;
		}
		return requestLeave();
	}

	/** Change views without touching document bytes, history, or selection. */
	function setWorkspaceView(view:EditorWorkspaceView):Void {
		workspaceView = view;
		if (view == BuildView) {
			activeTool = normalizeBuildTool(activeTool);
			focusedControl = normalizeBuildFocus(focusedControl);
		}
		setBuildPointerState(nextPointerState(buildPointerState, view == BuildView, Raylib.IsWindowFocused(), false, false));
		invalidatePreview();
	}

	/**
	 * Select one finite presentation layer without changing the draft.
	 *
	 * The complete world projection already owns every decoded cell. A layer
	 * change copies only one compact horizontal slice and leaves revision,
	 * history, dirty state, Save, and Test Play untouched.
	 */
	function selectEditLayer(requested:Int):Bool {
		final world = projection;
		if (world == null)
			return false;
		final selected = clampLayer(requested, world.height);
		if (selected == editLayerY)
			return false;
		final plan = projectFromWorld(world, selected);
		if (plan == null)
			return false;
		editLayerY = selected;
		planProjection = plan;
		invalidatePreview();
		return true;
	}

	/** Open the environment modal at its explicit enabled control. */
	function openEnvironmentPanel():Void {
		setBuildPointerState(EditorBuildPointerState.Released);
		environmentPanelOpen = true;
		environmentControl = firstEnvironmentControl();
	}

	/** Close the modal and return semantic focus to its visible toolbar button. */
	function closeEnvironmentPanel():Void {
		environmentPanelOpen = false;
		focusedControl = EditorFocusTarget.Environment;
	}

	/** Submit one field-preserving environment edit through normal history. */
	function applyEnvironmentControl(control:EditorEnvironmentControl, direction:EditorEnvironmentDirection):Void {
		if (control == EditorEnvironmentControl.Done)
			return;
		final current = session;
		if (current == null)
			return;
		final replacement = editEnvironment(environment, control, direction);
		switch current.mutate({baseRevision: current.revision(), mutation: Apply(SetEnvironment(replacement))}) {
			case MutationApplied(_, _, _, _, _):
				notice = Ready;
				refreshProjection(false, KeepTerrain);
			case MutationUnchanged(_, _):
				notice = Ready;
			case MutationRejected(_, _):
				notice = Invalid;
		}
	}

	/** Choose one creation card while preserving the current semantic selection. */
	function setActiveTool(tool:EditorTool):Void {
		activeTool = tool;
		invalidatePreview();
	}

	/**
	 * Apply one pointer-ownership transition at the native window boundary.
	 *
	 * The renderer-independent policy decides the state. This method performs a
	 * Raylib effect only when ownership changes, so steady Build frames do not
	 * repeatedly alter the operating-system cursor.
	 */
	function setBuildPointerState(next:EditorBuildPointerState):Void {
		if (next == buildPointerState)
			return;
		buildPointerState = next;
		if (next == EditorBuildPointerState.Captured)
			Raylib.DisableCursor();
		else
			Raylib.EnableCursor();
	}

	/** Select one of the five visible Build cards from its number key. */
	function selectBuildHotbarTool():Void {
		var slot = 0;
		if (Raylib.IsKeyPressed(KeyboardKey.One))
			slot = 1;
		else if (Raylib.IsKeyPressed(KeyboardKey.Two))
			slot = 2;
		else if (Raylib.IsKeyPressed(KeyboardKey.Three))
			slot = 3;
		else if (Raylib.IsKeyPressed(KeyboardKey.Four))
			slot = 4;
		else if (Raylib.IsKeyPressed(KeyboardKey.Five))
			slot = 5;
		final tool = toolForBuildHotbarSlot(slot);
		if (tool != null)
			setActiveTool(tool);
	}

	/** Leave immediately only when the package draft equals its last saved bytes. */
	function requestLeave():EditorScreenAction {
		setBuildPointerState(EditorBuildPointerState.Released);
		if (!isDirty())
			return ReturnToTitle;
		leavePromptOpen = true;
		focusedControl = EditorFocusTarget.KeepEditing;
		return StayInEditor;
	}

	/** Compare the current history-state identity with the last successful save. */
	function isDirty():Bool
		return editorPackage.hasUnsavedChanges();

	/** Select a World List object through the same stable workspace identity. */
	function selectObjectFromWorldList():Void {
		final index = objectList.activeIndex();
		if (index < 0 || index >= objectGizmos.length)
			return;
		selectObject(objectGizmos[index].id);
	}

	/** Select one authored object through the shared semantic workspace target. */
	function selectObject(id:ScenarioId):Void {
		final current = session;
		final index = objectIndex(id);
		if (current == null || index < 0)
			return;
		switch current.select({baseRevision: current.revision(), selection: NodeSelection(ObjectNode(id))}) {
			case SelectionApplied(_, _) | SelectionUnchanged(_, _):
				selection = current.selectedBounds();
				objectList = new GuiListViewState(index);
				notice = Ready;
			case SelectionRejected(_, _):
				notice = Invalid;
		}
	}

	/** Move the shared object target through the same revisioned history path. */
	function moveSelectedObject(delta:VoxelPoint):Void {
		final current = session;
		if (current == null)
			return;
		final selected = current.selectionSnapshot();
		final id = switch selected {
			case NodeSelection(ObjectNode(value)): value;
			case NoEditorSelection | VoxelSelection(_) | NodeSelection(_): return;
		};
		switch current.mutate({baseRevision: current.revision(), mutation: Apply(MoveObjectBy(id, delta))}) {
			case MutationApplied(_, _, _, _, _):
				notice = Ready;
				refreshProjection(false, KeepTerrain);
			case MutationUnchanged(_, _):
				notice = Ready;
			case MutationRejected(_, _):
				notice = Invalid;
		}
	}

	/** Rotate the selected transform-backed object through revisioned history. */
	function rotateSelectedObject(degrees:Int):Void {
		final current = session;
		if (current == null)
			return;
		final id = switch current.selectionSnapshot() {
			case NodeSelection(ObjectNode(value)): value;
			case NoEditorSelection | VoxelSelection(_) | NodeSelection(_): return;
		};
		switch current.mutate({baseRevision: current.revision(), mutation: Apply(RotateObjectBy(id, degrees))}) {
			case MutationApplied(_, _, _, _, _):
				notice = Ready;
				refreshProjection(false, KeepTerrain);
			case MutationUnchanged(_, _):
				notice = Ready;
			case MutationRejected(_, _):
				notice = Invalid;
		}
	}

	/** Resize the selected trigger through the same revisioned history path. */
	function resizeSelectedTrigger(size:VoxelSize):Void {
		final current = session;
		if (current == null)
			return;
		final id = switch current.selectionSnapshot() {
			case NodeSelection(ObjectNode(value)): value;
			case NoEditorSelection | VoxelSelection(_) | NodeSelection(_): return;
		};
		switch current.mutate({baseRevision: current.revision(), mutation: Apply(ResizeTriggerTo(id, size))}) {
			case MutationApplied(_, _, _, _, _):
				notice = Ready;
				refreshProjection(false, KeepTerrain);
			case MutationUnchanged(_, _):
				notice = Ready;
			case MutationRejected(_, _):
				notice = Invalid;
		}
	}

	/** Delete the shared object target through canonical history and clear stale UI state. */
	function deleteSelectedObject():Void {
		final current = session;
		if (current == null)
			return;
		final id = switch current.selectionSnapshot() {
			case NodeSelection(ObjectNode(value)): value;
			case NoEditorSelection | VoxelSelection(_) | NodeSelection(_): return;
		};
		switch current.mutate({baseRevision: current.revision(), mutation: Apply(RemoveObject(id))}) {
			case MutationApplied(_, _, _, _, _):
				selection = current.selectedBounds();
				objectList = new GuiListViewState(-1);
				detailsOpen = false;
				notice = Ready;
				refreshProjection(false, KeepTerrain);
			case MutationUnchanged(_, _):
				notice = Ready;
			case MutationRejected(_, _):
				notice = Invalid;
		}
	}

	/** Copy the shared object target through canonical history, then select the copy. */
	function duplicateSelectedObject():Void {
		final current = session;
		final draft = presentationDraft;
		if (current == null || draft == null)
			return;
		final sourceId = switch current.selectionSnapshot() {
			case NodeSelection(ObjectNode(value)): value;
			case NoEditorSelection | VoxelSelection(_) | NodeSelection(_): return;
		};
		final duplicate = duplicateObject(sourceId, draft.objects);
		if (duplicate == null) {
			notice = Invalid;
			return;
		}
		switch current.mutate({baseRevision: current.revision(), mutation: Apply(duplicate.command)}) {
			case MutationApplied(_, _, _, _, _):
				notice = Ready;
				refreshProjection(false, KeepTerrain);
				selectObject(duplicate.id);
			case MutationUnchanged(_, _):
				notice = Ready;
			case MutationRejected(_, _):
				notice = Invalid;
		}
	}

	/** Return the shared selected object's projection index, or `-1`. */
	function selectedObjectIndex():Int {
		final current = session;
		if (current == null)
			return -1;
		return switch current.selectionSnapshot() {
			case NodeSelection(ObjectNode(id)): objectIndex(id);
			case NoEditorSelection | VoxelSelection(_) | NodeSelection(_): -1;
		};
	}

	/** Resolve one stable object identity without trusting a widget index. */
	function objectIndex(id:ScenarioId):Int {
		final expected = id.text();
		for (index in 0...objectGizmos.length)
			if (objectGizmos[index].id.text() == expected)
				return index;
		return -1;
	}

	function undo():Void {
		final current = session;
		if (current == null)
			return;
		switch current.mutate({baseRevision: current.revision(), mutation: Undo}) {
			case MutationApplied(_, _, _, _, _):
				notice = Ready;
				refreshProjection();
			case MutationUnchanged(_, _):
				notice = Ready;
			case MutationRejected(_, _):
				notice = Invalid;
		}
	}

	function redo():Void {
		final current = session;
		if (current == null)
			return;
		switch current.mutate({baseRevision: current.revision(), mutation: Redo}) {
			case MutationApplied(_, _, _, _, _):
				notice = Ready;
				refreshProjection();
			case MutationUnchanged(_, _):
				notice = Ready;
			case MutationRejected(_, _):
				notice = Invalid;
		}
	}

	/**
	 * Give side-effect-free validated draft bytes to the application.
	 *
	 * The editor keeps its session, camera, selection, tools, panels, and history.
	 * The application creates a separate game runtime from the returned bytes and
	 * acquires the editing lock only after that stronger runtime accepts them.
	 */
	function requestTestPlay():EditorScreenAction {
		setBuildPointerState(EditorBuildPointerState.Released);
		final current = session;
		if (current == null) {
			notice = Invalid;
			return StayInEditor;
		}
		return switch current.prepareExternalTestPlay() {
			case ValidationPassed(canonical):
				notice = Testing;
				StartTestPlay(canonical);
			case ValidationFailed(_) | ValidationBlocked(_):
				notice = Invalid;
				StayInEditor;
		};
	}

	/** Lock the workspace after the complete ordinary-engine runtime accepts it. */
	public function beginTestPlay():Bool {
		final current = session;
		if (current == null)
			return false;
		final started = current.beginExternalTestPlay();
		notice = started ? Testing : Invalid;
		return started;
	}

	/** Unlock the same workspace after the disposable game runtime stops. */
	public function finishTestPlay(started:Bool):Void {
		final current = session;
		if (current != null)
			current.finishExternalTestPlay();
		notice = started ? Valid : Invalid;
	}

	/**
		Commit the temporary text-box value through the shared editor model.

		This is the boundary between presentation state and authored content:
		the screen proposes one literal title, while `EditorSession` decides
		whether it becomes canonical history. Every result resynchronizes the
		text field from the accepted draft so rejected or undone text cannot
		survive as a hidden second document.
	**/
	function commitWorldName(value:String):Bool {
		final current = session;
		if (current == null) {
			notice = Invalid;
			return false;
		}
		final accepted = switch current.mutate({
			baseRevision: current.revision(),
			mutation: Apply(SetTitle(ScenarioText.Literal(value)))
		}) {
			case MutationApplied(_, _, _, _, _) | MutationUnchanged(_, _):
				notice = Ready;
				true;
			case MutationRejected(_, _):
				notice = Invalid;
				false;
		};
		refreshProjection(false, KeepTerrain);
		return accepted;
	}

	/** Draw and edit the exact selected layer over the same draft used by Build. */
	function drawPlanViewport(left:Int, top:Int, width:Int, height:Int):Void {
		var currentPlan = planProjection;
		if (currentPlan == null || width <= 0 || height <= 0)
			return;
		var grid = layoutPlan(left, top, width, height, currentPlan);
		if (grid == null)
			return;

		final mouse = Raylib.GetMousePosition();
		final mouseX = Std.int(mouse.x.toFloat());
		final mouseY = Std.int(mouse.y.toFloat());
		var hover = pointAtPlan(currentPlan, grid, mouseX, mouseY);
		if (hover == null)
			invalidatePreview();
		else {
			updatePreview(hover, activeTool);
			if (Raylib.IsMouseButtonPressed(MouseButton.Left)) {
				final hoverX = hover.x;
				final hoverZ = hover.z;
				final objectIndex = activeTool == SelectTool ? objectIndexAtPlan(hoverX, hoverZ) : -1;
				if (objectIndex >= 0)
					selectObject(objectGizmos[objectIndex].id);
				else
					applyToolAt(activeTool, hover);
				currentPlan = planProjection;
				if (currentPlan == null)
					return;
				grid = layoutPlan(left, top, width, height, currentPlan);
				if (grid == null)
					return;
				hover = {x: hoverX, y: currentPlan.layerY, z: hoverZ};
				if (hover != null)
					updatePreview(hover, activeTool);
			}
		}

		Raylib.DrawRectangle(left, top, width, height, Color.rgba(18, 34, 42));
		for (z in 0...currentPlan.depth)
			for (x in 0...currentPlan.width) {
				final paletteCode = paletteCodeAtPlan(currentPlan, x, z);
				final cellLeft = grid.left + x * grid.cellSize;
				final cellTop = grid.top + z * grid.cellSize;
				final color = paletteCode == 0 ? Color.rgba(25, 48, 56) : terrainOverviewColor(paletteCode);
				Raylib.DrawRectangle(cellLeft + 1, cellTop + 1, grid.cellSize - 2, grid.cellSize - 2, color);
				Raylib.DrawRectangleLines(cellLeft, cellTop, grid.cellSize, grid.cellSize, Color.rgba(48, 78, 84));
				if (selectedPlanCell(x, z)) {
					Raylib.DrawRectangleLines(cellLeft + 1, cellTop + 1, grid.cellSize - 2, grid.cellSize - 2, CaxecraftPalette.selection());
					Raylib.DrawRectangleLines(cellLeft + 2, cellTop + 2, grid.cellSize - 4, grid.cellSize - 4, CaxecraftPalette.selection());
				}
			}
		final selectedObject = selectedObjectIndex();
		for (index in 0...objectGizmos.length) {
			final gizmo = objectGizmos[index];
			if (!gizmoIntersectsLayer(gizmo, editLayerY))
				continue;
			final x = Std.int(gizmo.x);
			final z = Std.int(gizmo.z);
			if (x >= 0 && z >= 0 && x < currentPlan.width && z < currentPlan.depth) {
				final markerSize = grid.cellSize > 10 ? 8 : 4;
				final markerLeft = grid.left + x * grid.cellSize + Std.int((grid.cellSize - markerSize) / 2);
				final markerTop = grid.top + z * grid.cellSize + Std.int((grid.cellSize - markerSize) / 2);
				Raylib.DrawRectangle(markerLeft, markerTop, markerSize, markerSize,
					index == selectedObject ? CaxecraftPalette.selection() : gizmoColor(gizmo.kind));
			}
		}
		for (link in zoneRuleLinks)
			switch link {
				case ResolvedZoneRule(ruleId, _, bounds) if (boundsIntersectLayer(bounds, editLayerY)):
					final cellLeft = grid.left + bounds.origin.x * grid.cellSize;
					final cellTop = grid.top + bounds.origin.z * grid.cellSize;
					Raylib.DrawRectangleLines(cellLeft + 2, cellTop + 2, bounds.size.width * grid.cellSize - 4, bounds.size.depth * grid.cellSize - 4,
						Color.rgba(236, 114, 255));
					Raylib.DrawTextString(ruleId.text(), cellLeft + 4, cellTop + 4, 10, Color.rgba(255, 210, 255));
				case ResolvedZoneRule(_, _, _) | UnresolvedZoneRule(_, _):
			}
		if (hover != null) {
			final hoverLeft = grid.left + hover.x * grid.cellSize;
			final hoverTop = grid.top + hover.z * grid.cellSize;
			final color = previewAllowed ? Color.rgba(92, 240, 186) : Color.rgba(255, 104, 82);
			Raylib.DrawRectangleLines(hoverLeft + 1, hoverTop + 1, grid.cellSize - 2, grid.cellSize - 2, color);
			Raylib.DrawRectangleLines(hoverLeft + 2, hoverTop + 2, grid.cellSize - 4, grid.cellSize - 4, color);
			if (!previewAllowed) {
				Raylib.DrawLine(hoverLeft + 3, hoverTop + 3, hoverLeft + grid.cellSize - 3, hoverTop + grid.cellSize - 3, color);
				Raylib.DrawLine(hoverLeft + grid.cellSize - 3, hoverTop + 3, hoverLeft + 3, hoverTop + grid.cellSize - 3, color);
			}
		}
	}

	/** Prefer the first canonical object marker in one Plan cell. */
	function objectIndexAtPlan(x:Int, z:Int):Int {
		for (index in 0...objectGizmos.length) {
			final gizmo = objectGizmos[index];
			if (gizmoIntersectsLayer(gizmo, editLayerY) && Std.int(gizmo.x) == x && Std.int(gizmo.z) == z)
				return index;
		}
		return -1;
	}

	/** True when the current semantic voxel target covers one Plan column. */
	function selectedPlanCell(x:Int, z:Int):Bool {
		final current = selection;
		if (current == null)
			return false;
		return boundsIntersectLayer(current, editLayerY)
			&& x >= current.origin.x
			&& z >= current.origin.z
			&& x < current.origin.x + current.size.width
			&& z < current.origin.z + current.size.depth;
	}

	/**
	 * Recompute the placement ghost from the cached presentation draft.
	 *
	 * Pointer movement must stay frame-local. This path translates the selected
	 * tool and shows whether that gesture has the inputs it needs. A click still
	 * enters `EditorSession.mutate`, which performs the complete reducer,
	 * canonical CAXEMAP, revision, and history checks before it changes the draft.
	 * The ghost can therefore look available immediately and still fail closed
	 * if the authoritative commit rejects it.
	 */
	function updatePreview(point:VoxelPoint, tool:EditorTool):Void {
		final current = session;
		final draft = presentationDraft;
		if (current == null || draft == null) {
			invalidatePreview();
			return;
		}
		if (previewPoint != null
			&& previewPoint.x == point.x
			&& previewPoint.y == point.y
			&& previewPoint.z == point.z
			&& previewRevision == current.revision()
			&& previewTool == tool)
			return;
		previewPoint = {x: point.x, y: point.y, z: point.z};
		previewRevision = current.revision();
		previewTool = tool;
		var paletteCode = 0;
		if (tool == PaintTool || tool == FillTool)
			paletteCode = paletteCodeForBlock(draft.world.palette, contentRegistry.defaultEditorBlockId());
		if (paletteCode < 0) {
			previewAllowed = false;
			return;
		}
		previewAllowed = switch commandForTool(tool, point, paletteCode, current.selectedBounds(), draft.objects, draft.ruleIds, activeRecipeFor(tool)) {
			case ToolCommandRejected(_): false;
			case ToolSelectionReady(_): true;
			case ToolCommandReady(_) | ToolBatchReady(_, _): true;
		};
	}

	/** Return the pack-selected recipe only for its matching creation tool. */
	function activeRecipeFor(tool:EditorTool):Null<EditorObjectRecipe>
		return tool == CatalogObjectTool ? contentRegistry.editorObjectAt(0) : null;

	/** Drop the last ghost when a view, tool, or draft transition changes meaning. */
	function invalidatePreview():Void {
		previewPoint = null;
		previewRevision = -1;
		previewAllowed = false;
	}

	/**
	 * Draw and operate the cached draft through a clipped perspective viewport.
	 *
	 * Raylib supplies device state, a screen ray, clipping, and drawing. Camera
	 * movement, volume lookup, ray picking, and command translation remain
	 * renderer-independent. Build shows the ordinary terrain atlases when the
	 * draft fits the gameplay world. A click gives Build the pointer. Mouse
	 * movement then looks without a held button. In terrain mode, the primary
	 * button removes a solid and the secondary button places adjacent ground.
	 * WASD/QE flies, number keys select the visible tool hotbar, and F focuses the
	 * world. Escape releases the pointer before the editor handles another cancel.
	 */
	function drawWorldViewport(left:Int, top:Int, width:Int, height:Int, resources:EditorRenderResources):Void {
		var current = projection;
		var currentCamera = camera;
		if (current == null || currentCamera == null || width <= 0 || height <= 0)
			return;
		final mouse = Raylib.GetMousePosition();
		final mouseX = Std.int(mouse.x.toFloat());
		final mouseY = Std.int(mouse.y.toFloat());
		final inside = mouseX >= left && mouseY >= top && mouseX < left + width && mouseY < top + height;
		final name = worldName;
		final pointerAvailable = inside && Raylib.IsWindowFocused() && (name == null || !name.isEditing());
		final leftPressed = Raylib.IsMouseButtonPressed(MouseButton.Left);
		final rightPressed = Raylib.IsMouseButtonPressed(MouseButton.Right);
		final capturePressed = buildPointerState == EditorBuildPointerState.Released && pointerAvailable && (leftPressed || rightPressed);
		if (capturePressed)
			setBuildPointerState(nextPointerState(buildPointerState, true, true, true, false));
		final cameraInputEnabled = buildPointerState == EditorBuildPointerState.Captured
			&& Raylib.IsWindowFocused()
			&& (name == null || !name.isEditing());
		if (cameraInputEnabled)
			selectBuildHotbarTool();
		if (cameraInputEnabled && Raylib.IsKeyPressed(KeyboardKey.F))
			currentCamera = focusCamera(current);
		else if (cameraInputEnabled) {
			final delta = Raylib.GetMouseDelta();
			currentCamera = stepCamera(current, currentCamera, {
				forward: axis(Raylib.IsKeyDown(KeyboardKey.W), Raylib.IsKeyDown(KeyboardKey.S)),
				right: axis(Raylib.IsKeyDown(KeyboardKey.D), Raylib.IsKeyDown(KeyboardKey.A)),
				vertical: axis(Raylib.IsKeyDown(KeyboardKey.E), Raylib.IsKeyDown(KeyboardKey.Q)),
				yaw: capturePressed ? 0.0 : -delta.x.toFloat() * 0.004,
				pitch: capturePressed ? 0.0 : -delta.y.toFloat() * 0.004,
				wheel: Raylib.GetMouseWheelMove().toFloat()
			}, Raylib.GetFrameTime().toFloat());
		}
		camera = currentCamera;
		final target = cameraTarget(currentCamera);
		final nativeCamera = Camera3D.make(Vector3.fromFloat(currentCamera.x, currentCamera.y, currentCamera.z),
			Vector3.fromFloat(target.x, target.y, target.z), Vector3.fromFloat(0.0, 1.0, 0.0), c.Float32.fromFloat(52.0), CameraProjection.Perspective);
		var hover:Null<EditorWorldHit> = null;
		var hoveredObject = -1;
		final aiming = buildPointerState == EditorBuildPointerState.Captured;
		if (inside || aiming) {
			final pointer = aiming ? Vector2.fromFloat(left + width * 0.5, top + height * 0.5) : mouse;
			final ray = Raylib.GetScreenToWorldRay(pointer, nativeCamera);
			final origin = ray.position;
			final direction = ray.direction;
			hover = pickWorld(current, {x: origin.x.toFloat(), y: origin.y.toFloat(), z: origin.z.toFloat()}, {
				x: direction.x.toFloat(),
				y: direction.y.toFloat(),
				z: direction.z.toFloat()
			}, editLayerY, 512.0);
			if (activeTool == SelectTool) {
				final objectHit = pickObject(objectGizmos, {x: origin.x.toFloat(), y: origin.y.toFloat(), z: origin.z.toFloat()}, {
					x: direction.x.toFloat(),
					y: direction.y.toFloat(),
					z: direction.z.toFloat()
				}, 512.0);
				if (objectHit != null && (hover == null || objectHit.distance <= hover.distance))
					hoveredObject = objectIndex(objectHit.id);
			}
		}
		final terrainMode = usesDirectTerrainControls(activeTool);
		final previewTool = activeTool == PaintTool ? PaintTool : (terrainMode ? EraseTool : activeTool);
		final previewPoint:Null<VoxelPoint> = if (hover == null) null else if (activeTool == PaintTool) hover.placement else hover.point;
		if (previewPoint == null || hoveredObject >= 0)
			invalidatePreview();
		else
			updatePreview(previewPoint, previewTool);
		if (!capturePressed && aiming && hoveredObject >= 0 && leftPressed) {
			selectObject(objectGizmos[hoveredObject].id);
		} else if (!capturePressed && aiming && hoveredObject < 0 && terrainMode && hover != null && (leftPressed || rightPressed)) {
			final edited = switch terrainAction(leftPressed, rightPressed, hover) {
				case NoTerrainAction: false;
				case RemoveTerrain(point): applyToolAt(EditorTool.EraseTool, point);
				case PlaceTerrain(point): applyToolAt(EditorTool.PaintTool, point);
			};
			current = projection;
			if (current == null)
				return;
			if (edited)
				invalidatePreview();
		} else if (!capturePressed && aiming && (hover != null || hoveredObject >= 0) && leftPressed) {
			if (hover != null)
				applyToolAt(activeTool, hover.point);
			current = projection;
			if (current == null)
				return;
			if (hoveredObject < 0 && hover != null)
				updatePreview(hover.point, activeTool);
		}

		Raylib.DrawRectangle(left, top, width, height, CaxecraftPalette.sky());
		Raylib.BeginScissorMode(left, top, width, height);
		Raylib.BeginMode3D(nativeCamera);
		Raylib.DrawCube(Vector3.fromFloat(current.width * 0.5, -0.04, current.depth * 0.5), c.Float32.fromFloat(current.width), c.Float32.fromFloat(0.08),
			c.Float32.fromFloat(current.depth), Color.rgba(26, 43, 50));
		final layerGridY = editLayerY + 0.002;
		for (x in 0...current.width + 1)
			Raylib.DrawLine3D(Vector3.fromFloat(x, layerGridY, 0.0), Vector3.fromFloat(x, layerGridY, current.depth), Color.rgba(78, 137, 143));
		for (z in 0...current.depth + 1)
			Raylib.DrawLine3D(Vector3.fromFloat(0.0, layerGridY, z), Vector3.fromFloat(current.width, layerGridY, z), Color.rgba(78, 137, 143));
		if (!terrainPresentation.draw(resources.terrainTexture, resources.terrainTextureReady, resources.adventureTerrainTexture,
			resources.adventureTerrainTextureReady, currentCamera.x, currentCamera.z))
			drawTerrainOverview(current);
		final selected = selection;
		if (selected != null)
			for (z in selected.origin.z...selected.origin.z + selected.size.depth)
				for (y in selected.origin.y...selected.origin.y + selected.size.height)
					for (x in selected.origin.x...selected.origin.x + selected.size.width)
						drawCellOutline(x, y, z, paletteCodeAtWorld(current, x, y, z) != 0, CaxecraftPalette.selection(), 1.05);
		final selectedObject = selectedObjectIndex();
		for (index in 0...objectGizmos.length)
			if (!visualUsesBillboard(objectVisuals[index]))
				drawEditorObject(nativeCamera, objectVisuals[index], objectGizmos[index], resources);
		for (index in 0...objectGizmos.length)
			if (visualUsesBillboard(objectVisuals[index]))
				drawEditorObject(nativeCamera, objectVisuals[index], objectGizmos[index], resources);
		for (index in 0...objectGizmos.length) {
			final gizmo = objectGizmos[index];
			final color = index == selectedObject || index == hoveredObject ? CaxecraftPalette.selection() : gizmoColor(gizmo.kind);
			final triggerAuthoringVisible = gizmo.kind == TriggerZoneGizmo && activeTool == TriggerZoneTool;
			if (triggerAuthoringVisible || index == selectedObject || index == hoveredObject)
				Raylib.DrawCubeWires(Vector3.fromFloat(gizmo.x, gizmo.y, gizmo.z), c.Float32.fromFloat(gizmo.width), c.Float32.fromFloat(gizmo.height),
					c.Float32.fromFloat(gizmo.depth), color);
			if (index == selectedObject || index == hoveredObject)
				Raylib.DrawCubeWires(Vector3.fromFloat(gizmo.x, gizmo.y, gizmo.z), c.Float32.fromFloat(gizmo.width + 0.10),
					c.Float32.fromFloat(gizmo.height + 0.10), c.Float32.fromFloat(gizmo.depth + 0.10), color);
		}
		if (previewPoint != null && hoveredObject < 0 && !selectedCell(previewPoint.x, previewPoint.y, previewPoint.z)) {
			final previewColor = previewAllowed ? Color.rgba(92, 240, 186) : Color.rgba(255, 104, 82);
			final previewSolid = paletteCodeAtWorld(current, previewPoint.x, previewPoint.y, previewPoint.z) != 0;
			drawCellOutline(previewPoint.x, previewPoint.y, previewPoint.z, previewSolid, previewColor, 1.08);
			if (!previewAllowed) {
				final markerY = previewSolid ? previewPoint.y + 1.02 : previewPoint.y + 0.08;
				Raylib.DrawLine3D(Vector3.fromFloat(previewPoint.x + 0.1, markerY, previewPoint.z + 0.1),
					Vector3.fromFloat(previewPoint.x + 0.9, markerY, previewPoint.z + 0.9), previewColor);
				Raylib.DrawLine3D(Vector3.fromFloat(previewPoint.x + 0.9, markerY, previewPoint.z + 0.1),
					Vector3.fromFloat(previewPoint.x + 0.1, markerY, previewPoint.z + 0.9), previewColor);
			}
		}
		Raylib.EndMode3D();
		Raylib.EndScissorMode();
		if (aiming)
			drawBuildCrosshair(left + Std.int(width / 2), top + Std.int(height / 2));
	}

	/** Draw a compact high-contrast target at the captured Build ray origin. */
	static function drawBuildCrosshair(centerX:Int, centerY:Int):Void {
		final shadow = Color.rgba(8, 20, 24);
		final light = CaxecraftPalette.hudText();
		Raylib.DrawLine(centerX - 8, centerY, centerX + 8, centerY, shadow);
		Raylib.DrawLine(centerX, centerY - 8, centerX, centerY + 8, shadow);
		Raylib.DrawLine(centerX - 6, centerY, centerX + 6, centerY, light);
		Raylib.DrawLine(centerX, centerY - 6, centerX, centerY + 6, light);
	}

	/**
	 * Draw a compact terrain fallback behind the selected edit grid.
	 *
	 * This path keeps incomplete and custom-size drafts editable when the gameplay
	 * renderer cannot represent their shape. It omits hidden caves. The selected
	 * grid and the exact Plan view still show hidden layers.
	 */
	static function drawTerrainOverview(world:EditorWorldProjection):Void {
		Rlgl.BeginSolidQuads();
		Rlgl.TexCoord(0.5, 0.5);
		for (patch in world.surfacePatches) {
			final top = patch.topY + 1.0;
			Rlgl.Color(terrainOverviewColor(patch.paletteCode));
			Rlgl.Normal(0.0, 1.0, 0.0);
			overviewVertex(patch.x, top, patch.z);
			overviewVertex(patch.x, top, patch.z + patch.depth);
			overviewVertex(patch.x + patch.width, top, patch.z + patch.depth);
			overviewVertex(patch.x + patch.width, top, patch.z);
		}
		Rlgl.EndQuads();
	}

	/** Submit one vertex after the current batch has selected its color. */
	static inline function overviewVertex(x:Float, y:Float, z:Float):Void
		Rlgl.Vertex(x, y, z);

	/** Give palette codes stable editor colors without knowing pack-owned IDs. */
	static function terrainOverviewColor(paletteCode:Int):Color {
		final family = paletteCode % 6;
		return switch family {
			case 0: Color.rgba(132, 157, 167);
			case 1: Color.rgba(108, 164, 103);
			case 2: Color.rgba(180, 153, 102);
			case 3: Color.rgba(102, 159, 174);
			case 4: Color.rgba(172, 174, 187);
			case _: Color.rgba(176, 119, 91);
		};
	}

	/** Give each closed object role one stable high-contrast editor color. */
	static function gizmoColor(kind:EditorObjectGizmoKind):Color
		return switch kind {
			case PlayerSpawnGizmo: Color.rgba(79, 224, 235);
			case CheckpointGizmo: Color.rgba(255, 214, 92);
			case ItemGizmo: Color.rgba(255, 244, 178);
			case EntityGizmo: Color.rgba(232, 83, 79);
			case NpcGizmo: Color.rgba(102, 224, 133);
			case PrefabGizmo: Color.rgba(194, 126, 72);
			case TriggerZoneGizmo: Color.rgba(210, 105, 230);
			case StatefulObjectGizmo: Color.rgba(255, 145, 55);
		};

	/** Draw one visible solid box or a shallow empty-cell cursor. */
	static function drawCellOutline(x:Int, y:Int, z:Int, solid:Bool, color:Color, scale:Float):Void {
		if (solid) {
			Raylib.DrawCubeWires(Vector3.fromFloat(x + 0.5, y + 0.5, z + 0.5), c.Float32.fromFloat(scale), c.Float32.fromFloat(scale),
				c.Float32.fromFloat(scale), color);
		} else {
			Raylib.DrawCubeWires(Vector3.fromFloat(x + 0.5, y + 0.04, z + 0.5), c.Float32.fromFloat(scale), c.Float32.fromFloat(0.08),
				c.Float32.fromFloat(scale), color);
		}
	}

	/** Convert two held keys into one closed negative/zero/positive axis. */
	static inline function axis(positive:Bool, negative:Bool):Float {
		if (positive == negative)
			return 0.0;
		return positive ? 1.0 : -1.0;
	}

	/** True when one displayed cell lies inside the session's current selection. */
	function selectedCell(x:Int, y:Int, z:Int):Bool {
		final current = selection;
		if (current == null)
			return false;
		return x >= current.origin.x
			&& y >= current.origin.y
			&& z >= current.origin.z
			&& x < current.origin.x + current.size.width
			&& y < current.origin.y + current.size.height
			&& z < current.origin.z + current.size.depth;
	}

	/**
	 * Apply one visual tool through the same typed session path used by history.
	 *
	 * The cached view changes only after `EditorSession` accepts the command.
	 * Rejected tools leave both draft and projection untouched and publish a
	 * visible invalid notice.
	 */
	function applyToolAt(tool:EditorTool, point:VoxelPoint):Bool {
		final current = session;
		final draft = presentationDraft;
		if (current == null || draft == null) {
			notice = Invalid;
			return false;
		}
		var paletteCode = 0;
		var needsPalette = false;
		switch tool {
			case PaintTool:
				needsPalette = true;
			case FillTool:
				needsPalette = true;
			case SelectTool:
			case EraseTool:
			case CheckpointTool | CatalogObjectTool | TriggerZoneTool:
		}
		if (needsPalette) {
			paletteCode = paletteCodeForBlock(draft.world.palette, contentRegistry.defaultEditorBlockId());
			if (paletteCode < 0) {
				notice = Invalid;
				return false;
			}
		}
		final toolResult = commandForTool(tool, point, paletteCode, current.selectedBounds(), draft.objects, draft.ruleIds, activeRecipeFor(tool));
		return switch toolResult {
			case ToolCommandRejected(_):
				notice = Invalid;
				false;
			case ToolSelectionReady(bounds):
				switch current.select({baseRevision: current.revision(), selection: VoxelSelection(bounds)}) {
					case SelectionApplied(_, _) | SelectionUnchanged(_, _):
						selection = current.selectedBounds();
						detailsOpen = false;
						invalidatePreview();
						notice = Ready;
						true;
					case SelectionRejected(_, _):
						notice = Invalid;
						false;
				}
			case ToolCommandReady(value):
				switch current.mutate({baseRevision: current.revision(), mutation: Apply(value)}) {
					case MutationApplied(_, _, _, _, _):
						notice = Ready;
						refreshProjection(false, terrainRefreshForCommand(value));
						switch value {
							case PutObject(object): selectObject(object.id);
							case _:
						}
						true;
					case MutationUnchanged(_, _):
						invalidatePreview();
						notice = Ready;
						true;
					case MutationRejected(_, _):
						notice = Invalid;
						false;
				}
			case ToolBatchReady(commands, selectedObject):
				switch current.mutate({baseRevision: current.revision(), mutation: ApplyBatch(commands)}) {
					case MutationApplied(_, _, _, _, _):
						notice = Ready;
						refreshProjection(false, terrainRefreshForBatch(commands));
						selectObject(selectedObject);
						true;
					case MutationUnchanged(_, _):
						notice = Ready;
						true;
					case MutationRejected(_, _):
						notice = Invalid;
						false;
				}
		};
	}

	/**
	 * Rebuild presentation state after a session transition.
	 *
	 * The presentation query copies only values that this screen can draw or use
	 * for a command. It does not parse CAXEMAP or expose the session's mutable
	 * draft arrays. This method runs after New World, an accepted edit, undo, or
	 * redo, not from every frame.
	 */
	function refreshProjection(resetCamera:Bool = false, terrainRefresh:EditorTerrainRefreshRequest = RefreshAllTerrain):Void {
		final current = session;
		if (current == null) {
			terrainPresentation.clear();
			presentationDraft = null;
			projection = null;
			planProjection = null;
			editLayerY = 0;
			objectGizmos = [];
			objectVisuals = [];
			objectLabels = "";
			flowRuleCount = 0;
			zoneRuleLinks = [];
			environment = null;
			camera = null;
			selection = null;
			invalidatePreview();
			notice = Invalid;
			return;
		}
		final draft = switch current.query(InspectPresentation) {
			case PresentationObserved(_, value): value;
			case _: throw "editor returned the wrong presentation observation";
		};
		presentationDraft = draft;
		syncWorldName(draft.title);
		environment = draft.environment;
		final previous = projection;
		projection = draft.projection;
		final runtimeProjection = projection;
		#if caxecraft_pilot
		pilotTerrainPatchDirtyChunks = 0;
		pilotTerrainPatchFellBack = false;
		#end
		if (runtimeProjection == null)
			terrainPresentation.clear();
		else
			switch terrainRefresh {
				case KeepTerrain:
				case RefreshTerrainVoxel(point):
					final dirtyChunks = terrainPresentation.refreshVoxel(draft.world, runtimeProjection, contentRegistry, point);
					if (dirtyChunks < 0) {
						terrainPresentation.refresh(draft.world, runtimeProjection, contentRegistry);
						#if caxecraft_pilot
						pilotTerrainPatchFellBack = true;
						#end
					} else {
						#if caxecraft_pilot
						pilotTerrainPatchDirtyChunks = dirtyChunks;
						#end
					}
				case RefreshAllTerrain:
					terrainPresentation.refresh(draft.world, runtimeProjection, contentRegistry);
			}
		planProjection = switch projection {
			case null:
				editLayerY = 0;
				null;
			case value:
				editLayerY = clampLayer(editLayerY, value.height);
				projectFromWorld(value, editLayerY);
		};
		objectGizmos = projectObjects(draft.objects);
		objectVisuals = [for (object in draft.objects) objectVisualFor(contentRegistry, object)];
		flowRuleCount = draft.flowRuleCount;
		zoneRuleLinks = draft.zoneRuleLinks;
		final labels:Array<String> = [];
		for (gizmo in objectGizmos)
			labels.push(gizmo.id.text());
		objectLabels = labels.join(";");
		if (objectList.activeIndex() < 0 || objectList.activeIndex() >= objectGizmos.length)
			objectList = new GuiListViewState(-1);
		selection = current.selectedBounds();
		invalidatePreview();
		final next = projection;
		if (next == null) {
			camera = null;
			notice = Invalid;
		} else if (resetCamera || camera == null || previous == null || previous.width != next.width || previous.height != next.height
			|| previous.depth != next.depth) {
			camera = focusCamera(next);
		}
	}

	/**
		Copy an authored literal title into Raygui's temporary editing buffer.

		Message-backed titles keep their localization identity in the document.
		This first text field edits only literal titles; a later localization
		panel can resolve and edit message catalogs without replacing a message
		reference with whichever language happened to be displayed.
	**/
	function syncWorldName(title:ScenarioText):Void {
		final name = worldName;
		if (name == null)
			return;
		switch title {
			case Literal(value):
				name.replace(value);
			case Message(_):
		}
	}

	#if caxecraft_pilot
	/**
	 * Commit one deterministic title through the production text-field path.
	 *
	 * The graphical pilot bypasses only operating-system keyboard delivery. It
	 * still writes the owned Raygui buffer and submits the same typed session
	 * command that an Enter key or outside click confirms.
	 */
	public function applyPilotWorldName(value:String):Bool {
		final name = worldName;
		if (name == null || !name.replace(value))
			return false;
		name.setEditing(false);
		return commitWorldName(name.text());
	}

	/** Publish pilot edits through the same package save action as the toolbar. */
	public function applyPilotSave():Bool
		return requestSave();

	/**
	 * Select a layer and prove that this presentation action changed no draft state.
	 *
	 * The native pilot uses this narrow seam instead of synthesizing a mouse
	 * click. It still runs the production layer-selection path and checks the
	 * complete document and history observations on both sides.
	 */
	public function applyPilotLayer(layerY:Int):Bool {
		final current = session;
		if (current == null)
			return false;
		final beforeRevision = current.revision();
		final beforeIdentity = current.stateIdentity();
		final beforeCanonical = current.canonicalDraft();
		final beforeUndoDepth = current.undoDepth();
		final beforeRedoDepth = current.redoDepth();
		final beforeHistoryEntries = current.historyEntries();
		final beforeHistoryBytes = current.historyBytes();
		final beforeDirty = isDirty();
		return selectEditLayer(layerY)
			&& editLayerY == layerY
			&& current.revision() == beforeRevision
			&& current.stateIdentity() == beforeIdentity
			&& current.canonicalDraft().compare(beforeCanonical) == 0
			&& current.undoDepth() == beforeUndoDepth
			&& current.redoDepth() == beforeRedoDepth
			&& current.historyEntries() == beforeHistoryEntries
			&& current.historyBytes() == beforeHistoryBytes
			&& isDirty() == beforeDirty;
	}

	/**
	 * Submit one deterministic pilot gesture through the production tool path.
	 *
	 * The graphical runner bypasses only operating-system pointer delivery. It
	 * still exercises command translation, revision checking, `EditorSession`
	 * history, cache refresh, and the real renderer. Ordinary builds omit this
	 * method.
	 */
	public function applyPilotTool(tool:EditorTool, point:VoxelPoint):Bool
		return applyToolAt(tool, point);

	/** Return the exact chunk invalidation count from the last pilot voxel edit. */
	public inline function pilotPatchDirtyChunks():Int
		return pilotTerrainPatchDirtyChunks;

	/** True when the last pilot voxel edit could not use the incremental path. */
	public inline function pilotPatchFellBack():Bool
		return pilotTerrainPatchFellBack;

	/** Place through the direct secondary action, then select the new terrain. */
	public function applyPilotPaintFirstAir():Bool {
		final current = projection;
		if (current == null)
			return false;
		for (z in 0...current.depth)
			for (x in 0...current.width) {
				final y = surfaceTopAt(current, x, z) + 1;
				if (y >= 0 && y < current.height && paletteCodeAtWorld(current, x, y, z) == 0) {
					final point:VoxelPoint = {x: x, y: y, z: z};
					final hit:EditorWorldHit = {
						point: point,
						placement: point,
						distance: 0.0,
						solid: false
					};
					return switch terrainAction(false, true, hit) {
						case PlaceTerrain(target): applyToolAt(EditorTool.PaintTool, target) && applyToolAt(EditorTool.SelectTool, target);
						case NoTerrainAction | RemoveTerrain(_): false;
					};
				}
			}
		return false;
	}

	/** Place the pack's first catalog recipe on a visible authored surface. */
	public function applyPilotCatalogObject():Bool {
		final current = projection;
		if (current == null || contentRegistry.editorObjectAt(0) == null)
			return false;
		var z = current.depth - 1;
		while (z >= 0) {
			var x = current.width - 1;
			while (x >= 0) {
				final y = surfaceTopAt(current, x, z) + 1;
				if (y >= 0 && y < current.height) {
					final point:VoxelPoint = {x: x, y: y, z: z};
					if (applyToolAt(EditorTool.CatalogObjectTool, point))
						return true;
				}
				x--;
			}
			z--;
		}
		return false;
	}

	/** Select the first authored object so a rendered review can inspect its real controls. */
	public function applyPilotSelectFirstObject():Bool {
		if (objectGizmos.length == 0)
			return false;
		selectObject(objectGizmos[0].id);
		return selectedObjectIndex() == 0;
	}

	/**
	 * Move the production camera from deterministic pilot input.
	 *
	 * This bypasses only operating-system device delivery. The same
	 * renderer-neutral camera step is used by interactive keyboard and mouse
	 * input, and ordinary builds omit the method.
	 */
	public function applyPilotCamera(input:EditorCameraInput, frameSeconds:Float):Bool {
		final currentProjection = projection;
		final currentCamera = camera;
		if (currentProjection == null || currentCamera == null)
			return false;
		camera = stepCamera(currentProjection, currentCamera, input, frameSeconds);
		return true;
	}

	/** Capture Build through the production policy so the review frame shows direct aiming. */
	public function applyPilotBuildCapture():Bool {
		setBuildPointerState(nextPointerState(buildPointerState, true, true, true, false));
		return buildPointerState == EditorBuildPointerState.Captured;
	}
	#end
}
#end

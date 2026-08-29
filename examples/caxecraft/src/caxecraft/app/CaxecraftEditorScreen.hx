package caxecraft.app;

#if c
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipe;
import caxecraft.editor.EditorBuildControls.EditorBuildObjectAction;
import caxecraft.editor.EditorBuildControls.EditorBuildObjectGrab;
import caxecraft.editor.EditorBuildControls.EditorBuildPointerState;
import caxecraft.editor.EditorBuildControls.EditorBuildPrompt;
import caxecraft.editor.EditorBuildControls.EditorBuildTerrainAction;
import caxecraft.editor.EditorBuildControls.EditorObjectShortcutAction;
import caxecraft.editor.EditorBuildControls.buildPrompt as buildPromptFor;
import caxecraft.editor.EditorBuildControls.cycleBuildHotbarTool;
import caxecraft.editor.EditorBuildControls.nextPointerState;
import caxecraft.editor.EditorBuildControls.nextObjectGrab;
import caxecraft.editor.EditorBuildControls.immersiveWorkspaceActive;
import caxecraft.editor.EditorBuildControls.moveBuildFocus;
import caxecraft.editor.EditorBuildControls.normalizeBuildFocus;
import caxecraft.editor.EditorBuildControls.normalizeBuildTool;
import caxecraft.editor.EditorBuildControls.normalizeBuildPaletteCode;
import caxecraft.editor.EditorBuildControls.objectAction;
import caxecraft.editor.EditorBuildControls.objectGrabActive;
import caxecraft.editor.EditorBuildControls.objectGrabCandidate;
import caxecraft.editor.EditorBuildControls.objectPlacementDelta;
import caxecraft.editor.EditorBuildControls.objectShortcutAction;
import caxecraft.editor.EditorBuildControls.terrainAction;
import caxecraft.editor.EditorBuildControls.cycleBuildPaletteCode;
import caxecraft.editor.EditorBuildControls.pickBuildPaletteCode;
import caxecraft.editor.EditorBuildControls.toolForBuildHotbarSlot;
import caxecraft.editor.EditorBuildControls.usesDirectTerrainControls;
import caxecraft.editor.EditorPackageSession;
import caxecraft.editor.EditorPackageSession.EditorPackageSaveResult;
import caxecraft.editor.EditorPackageSession.editorPackageErrorMessage;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorTerrainRefresh.EditorTerrainRefreshRequest;
import caxecraft.editor.EditorTerrainRefresh.forBatch as terrainRefreshForBatch;
import caxecraft.editor.EditorTerrainRefresh.forCommand as terrainRefreshForCommand;
import caxecraft.editor.EditorTerrainRefresh.forTerrainChange as terrainRefreshForTerrainChange;
import caxecraft.editor.EditorTextAuthoring.EditorTextDocument;
import caxecraft.editor.EditorTextAuthoring.EditorTextDocumentError;
import caxecraft.editor.EditorTextAuthoring.EditorTextDocumentOpenResult;
import caxecraft.editor.EditorTextAuthoring.EditorTextEditResult;
import caxecraft.editor.EditorEnvironment.EditorEnvironmentControl;
import caxecraft.editor.EditorEnvironment.EditorEnvironmentDirection;
import caxecraft.editor.EditorEnvironment.editEnvironment;
import caxecraft.editor.EditorEnvironment.firstEnvironmentControl;
import caxecraft.editor.EditorEnvironment.moveEnvironmentControl;
import caxecraft.editor.EditorAssetBrowser.EditorAssetCategory;
import caxecraft.editor.EditorAssetBrowser.EditorAssetEntry;
import caxecraft.editor.EditorAssetBrowser.EditorAssetUse;
import caxecraft.editor.EditorAssetBrowser.allEditorAssetCategories;
import caxecraft.editor.EditorAssetBrowser.availableEditorAssets;
import caxecraft.editor.EditorAssetBrowser.editorAssetHelp;
import caxecraft.editor.EditorAssetBrowser.editorAssetLabel;
import caxecraft.editor.EditorAssetBrowser.filterEditorAssets;
import caxecraft.editor.EditorAssetBrowser.moveEditorAssetSelection;
import caxecraft.editor.EditorFocus.EditorFocusTarget;
import caxecraft.editor.EditorFocus.initialFocus;
import caxecraft.editor.EditorFocus.moveFocus;
import caxecraft.editor.EditorFlowProjection.EditorZoneRuleProjection;
import caxecraft.editor.EditorFlowProjection.EditorFlowCard;
import caxecraft.editor.EditorFlowProjection.EditorFlowNestedCard;
import caxecraft.editor.EditorFlowReferences.EditorFlowReference;
import caxecraft.editor.EditorFlowReferences.EditorFlowReferenceRole;
import caxecraft.editor.EditorFlowProjection.EditorFlowRuleProjection;
import caxecraft.editor.EditorFlowProjection.EditorFlowTraceProjection;
import caxecraft.editor.EditorFlowProjection.EditorFlowTraceRow;
import caxecraft.editor.EditorFlowProjection.EditorTriggerOverlap;
import caxecraft.editor.EditorFlowProjection.EditorFlowUiMessage;
import caxecraft.editor.EditorFlowProjection.retainLatestFlowTrace;
import caxecraft.editor.EditorFlowAuthoring.EditorFlowAuthoringResult;
import caxecraft.editor.EditorFlowAuthoring.EditorFlowCardAddress;
import caxecraft.editor.EditorFlowAuthoring.EditorFlowCardEdit;
import caxecraft.editor.EditorFlowAuthoring.applyFlowWorldPick;
import caxecraft.editor.EditorFlowAuthoring.applyFlowDocumentPick;
import caxecraft.editor.EditorFlowAuthoring.connectZone;
import caxecraft.editor.EditorFlowAuthoring.editFlowCard;
import caxecraft.editor.EditorFlowAuthoring.isWorldPickableFlowRole;
import caxecraft.editor.EditorFlowAuthoring.nextZoneConnectionRuleId;
import caxecraft.editor.EditorFlowAuthoring.worldPickFor;
import caxecraft.editor.EditorFlowCardLibrary.EditorFlowActionChoice;
import caxecraft.editor.EditorFlowCardLibrary.EditorFlowContentChoices;
import caxecraft.editor.EditorFlowCardLibrary.EditorFlowEventChoice;
import caxecraft.editor.EditorFlowCardLibrary.EditorFlowPredicateChoice;
import caxecraft.editor.EditorFlowCardLibrary.actionCardChoices;
import caxecraft.editor.EditorFlowCardLibrary.editorFlowContentChoices;
import caxecraft.editor.EditorFlowCardLibrary.eventCardChoices;
import caxecraft.editor.EditorFlowCardLibrary.isDocumentFlowReferenceRole;
import caxecraft.editor.EditorFlowCardLibrary.predicateCardChoices;
import caxecraft.app.CaxecraftFlowCardPanel.CaxecraftFlowCardPanelAction;
import caxecraft.app.CaxecraftFlowCardPanel.CaxecraftFlowCardPanelTarget;
import caxecraft.app.CaxecraftFlowCardPanel.CaxecraftFlowDocumentPanelAction;
import caxecraft.app.CaxecraftFlowCardPanel.CaxecraftFlowDocumentPanelTarget;
import caxecraft.app.CaxecraftFlowCardPanel.drawFlowCardPanel;
import caxecraft.app.CaxecraftFlowCardPanel.drawFlowDocumentPanel;
import caxecraft.app.CaxecraftFlowCardPanel.flowCardPanelOpen;
import caxecraft.app.CaxecraftFlowCardPanel.flowDocumentPanelOpen;
import caxecraft.editor.EditorObjectDuplicate.duplicateObjectWithConnectedRules;
import caxecraft.editor.EditorObjectDelete.deleteObjectWithConnectedRules;
import caxecraft.editor.EditorObjectPresentation.EditorObjectVisual;
import caxecraft.editor.EditorObjectPresentation.visualFor as objectVisualFor;
import caxecraft.editor.EditorObjectPresentation.visualUsesBillboard;
import caxecraft.editor.EditorPresentation.EditorPresentationSnapshot;
import caxecraft.editor.EditorPresentation.EditorPresentationDetails;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorNodeRef;
import caxecraft.editor.EditorTypes.EditorObservation;
import caxecraft.editor.EditorTypes.EditorQuery;
import caxecraft.editor.EditorTypes.EditorSelection;
import caxecraft.editor.EditorTypes.EditorSelectionResult;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.editor.EditorTypes.EditorValidationObservation;
import caxecraft.editor.EditorViewport.EditorViewportLayout;
import caxecraft.editor.EditorViewport.EditorViewportProjection;
import caxecraft.editor.EditorViewport.EditorTool;
import caxecraft.editor.EditorViewport.EditorToolCommandResult;
import caxecraft.editor.EditorViewport.EditorToolContext;
import caxecraft.editor.EditorViewport.boundsIntersectLayer;
import caxecraft.editor.EditorViewport.clampLayer;
import caxecraft.editor.EditorViewport.commandFor as commandForTool;
import caxecraft.editor.EditorViewport.layout as layoutPlan;
import caxecraft.editor.EditorViewport.inspectorVisible as shouldShowInspector;
import caxecraft.editor.EditorViewport.paletteCodeAt as paletteCodeAtPlan;
import caxecraft.editor.EditorViewport.pointAt as pointAtPlan;
import caxecraft.editor.EditorViewport.paletteCodeForBlock;
import caxecraft.editor.EditorViewport.projectFromWorld;
import caxecraft.editor.EditorWorldViewport.EditorCameraInput;
import caxecraft.editor.EditorWorldViewport.EditorCameraMode;
import caxecraft.editor.EditorWorldViewport.EditorCameraState;
import caxecraft.editor.EditorWorldViewport.EditorObjectFacing;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmo;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmoKind;
import caxecraft.editor.EditorWorldViewport.EditorWorldHit;
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.editor.EditorWorldViewport.EditorWorldVector;
import caxecraft.editor.EditorWorldViewport.cameraMode;
import caxecraft.editor.EditorWorldViewport.cameraPose;
import caxecraft.editor.EditorWorldViewport.cameraTarget;
import caxecraft.editor.EditorWorldViewport.cycleCameraMode;
import caxecraft.editor.EditorWorldViewport.focusCamera;
import caxecraft.editor.EditorWorldViewport.gizmoIntersectsLayer;
import caxecraft.editor.EditorWorldViewport.paletteCodeAtWorld;
import caxecraft.editor.EditorWorldViewport.patchProjectedVoxel;
import caxecraft.editor.EditorWorldViewport.pickObject;
import caxecraft.editor.EditorWorldViewport.pickWorld;
import caxecraft.editor.EditorWorldViewport.projectObjects;
import caxecraft.editor.EditorWorldViewport.retargetOrbitCamera;
import caxecraft.editor.EditorWorldViewport.stepCamera;
import caxecraft.editor.EditorWorldViewport.surfaceTopAt;
import caxecraft.input.NavigationInput.NavigationCommand;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlowRuntime.FlowTraceEntry;
import caxecraft.scenario.ScenarioEnvironment;
import caxecraft.scenario.ScenarioEnvironment.ScenarioHorizonEdge;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioDiagnostic;
import caxecraft.scenario.ScenarioDiagnosticText.scenarioDiagnosticMessage;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioObject;
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

/** Visible state of the isolated advanced-text draft. */
private enum EditorTextNotice {
	TextClean;
	TextDirty;
	TextInvalid;
	TextStale;
}

/** One editable view over the same canonical editor draft. */
private enum abstract EditorWorkspaceView(Int) {
	var BuildView = 0;
	var PlanView = 1;
}

/** One bounded two-step gesture that fills a typed card from the world view. */
private enum EditorFlowWorldPickMode {
	NoFlowWorldPick;
	ConnectZoneWorldPick(zone:ScenarioId, revision:Int);
	ReplaceFlowReferenceWorldPick(zone:ScenarioId, rule:ScenarioId, card:EditorFlowCardAddress, referenceIndex:Int, revision:Int);
}

/** One object or world point that Orbit keeps in view. */
private typedef EditorCameraFocus = {
	final target:EditorWorldVector;
	final orbitDistance:Float;
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
 * volumes by one cell on each axis. The reducer rejects a size outside the
 * finite world. A selected trigger shows its registry-backed WHEN / IF / DO
 * cards. Their compact controls replace conditions, insert, replace, move, or
 * remove actions, and bind typed world references. These edits write the same
 * typed rules that Advanced mode uses. Native source save and bounded
 * horizontal layer controls are available.
 * Test Play uses a disposable game runtime and keeps the editor workspace.
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
	var flowRules:Array<EditorFlowRuleProjection>;
	var zoneRuleLinks:Array<EditorZoneRuleProjection>;
	var flowOverlaps:Array<EditorTriggerOverlap>;
	var flowTrace:EditorFlowTraceProjection;
	var flowWorldPickMode:EditorFlowWorldPickMode;
	var flowCardLibraryTarget:CaxecraftFlowCardPanelTarget;
	var flowDocumentPickMode:CaxecraftFlowDocumentPanelTarget;
	final flowContentChoices:EditorFlowContentChoices;
	var camera:Null<EditorCameraState>;
	var selection:Null<VoxelBounds>;
	var focusedControl:EditorFocusTarget;
	var workspaceView:EditorWorkspaceView;
	var buildPointerState:EditorBuildPointerState;
	var objectGrab:EditorBuildObjectGrab;
	var editLayerY:Int;
	var activeTool:EditorTool;
	var groundPaletteCode:Int;
	final assetCategories:Array<EditorAssetCategory>;
	final assetEntries:Array<EditorAssetEntry>;
	final assetSearch:Null<GuiTextBoxState>;
	var assetBrowserOpen:Bool;
	var assetCategory:EditorAssetCategory;
	var assetSelection:Int;
	var assetQuery:String;
	var visibleAssets:Array<EditorAssetEntry>;
	var selectedObjectAsset:Null<EditorAssetEntry>;
	var textWorkspaceOpen:Bool;
	var textDocument:Null<EditorTextDocument>;
	var textBaseRevision:Int;
	var textSelectedLine:Int;
	var textScrollLine:Int;
	var textDiagnostics:Array<ScenarioDiagnostic>;
	var textNotice:EditorTextNotice;
	final textLineEditor:Null<GuiTextBoxState>;
	var detailsOpen:Bool;
	var worldListOpen:Bool;
	var environmentPanelOpen:Bool;
	var environmentControl:EditorEnvironmentControl;
	var environment:Null<ScenarioEnvironment>;
	var leavePromptOpen:Bool;
	var previewPoint:Null<VoxelPoint>;
	var previewRevision:Int;
	var previewTool:EditorTool;
	var previewPaletteCode:Int;
	var previewAllowed:Bool;

	/** Remaining view-only time for the in-world edit confirmation. */
	var buildConfirmationSeconds:Float;

	var objectList:GuiListViewState;

	#if caxecraft_pilot
	/** Newly dirty terrain chunks from the most recent incremental editor refresh. */
	var pilotTerrainPatchDirtyChunks:Int = 0;

	/** True when the most recent requested voxel patch had to rebuild all terrain. */
	var pilotTerrainPatchFellBack:Bool = false;

	/** Total synchronous time spent refreshing presentation that retained terrain. */
	var pilotKeepTerrainRefreshMicroseconds:Int = 0;

	/** Number of retained-terrain refreshes included in the pilot timing. */
	var pilotKeepTerrainRefreshCount:Int = 0;

	/** Total synchronous time spent refreshing one accepted terrain voxel. */
	var pilotVoxelRefreshMicroseconds:Int = 0;

	/** Number of one-voxel refreshes included in the pilot timing. */
	var pilotVoxelRefreshCount:Int = 0;

	/** Total synchronous time for one object undo and matching redo. */
	var pilotHistoryRoundTripMicroseconds:Int = 0;

	/** Total synchronous time for one terrain undo and matching redo. */
	var pilotTerrainHistoryRoundTripMicroseconds:Int = 0;
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

	/** Temporary native buffer for the selected object's canonical identity. */
	final objectName:Null<GuiTextBoxState>;

	/** Stable object whose identity currently appears in `objectName`. */
	var objectNameTarget:Null<ScenarioId>;

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
		flowRules = [];
		zoneRuleLinks = [];
		flowOverlaps = [];
		flowTrace = {rows: [], truncated: false};
		flowWorldPickMode = NoFlowWorldPick;
		flowCardLibraryTarget = NoFlowCardPanel;
		flowDocumentPickMode = NoFlowDocumentPanel;
		flowContentChoices = editorFlowContentChoices(contentRegistry);
		camera = null;
		selection = null;
		focusedControl = initialFocus();
		workspaceView = BuildView;
		buildPointerState = EditorBuildPointerState.Released;
		objectGrab = NoObjectGrab;
		editLayerY = 0;
		activeTool = SelectTool;
		groundPaletteCode = -1;
		assetCategories = allEditorAssetCategories();
		assetEntries = availableEditorAssets(contentRegistry, uiCatalog);
		assetSearch = GuiTextBoxState.create(64);
		assetBrowserOpen = false;
		assetCategory = EditorAssetCategory.TerrainAssets;
		assetSelection = 0;
		assetQuery = "";
		visibleAssets = filterEditorAssets(assetEntries, assetCategory, assetQuery);
		selectedObjectAsset = firstObjectAsset(assetEntries);
		textWorkspaceOpen = false;
		textDocument = null;
		textBaseRevision = -1;
		textSelectedLine = 0;
		textScrollLine = 0;
		textDiagnostics = [];
		textNotice = TextClean;
		textLineEditor = GuiTextBoxState.create(EditorTextDocument.MAX_LINE_BYTES + 1);
		detailsOpen = false;
		worldListOpen = false;
		environmentPanelOpen = false;
		environmentControl = firstEnvironmentControl();
		environment = null;
		leavePromptOpen = false;
		previewPoint = null;
		previewRevision = -1;
		previewTool = SelectTool;
		previewPaletteCode = -1;
		previewAllowed = false;
		buildConfirmationSeconds = 0.0;
		objectList = new GuiListViewState(-1);
		worldName = GuiTextBoxState.create(64);
		objectName = GuiTextBoxState.create(64);
		objectNameTarget = null;
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
		final editedName = worldName;
		final editedObjectName = objectName;
		final editedAssetSearch = assetSearch;
		final editedTextLine = textLineEditor;
		if (!leavePromptOpen
			&& !environmentPanelOpen
			&& !assetBrowserOpen
			&& !flowCardLibraryOpen()
			&& !flowDocumentPickerOpen()
			&& (editedName == null || !editedName.isEditing())
			&& (editedObjectName == null || !editedObjectName.isEditing())
			&& (editedAssetSearch == null || !editedAssetSearch.isEditing())
			&& (editedTextLine == null || !editedTextLine.isEditing())
			&& !shortcutModifierDown()
			&& Raylib.IsKeyPressed(KeyboardKey.T)) {
			if (textWorkspaceOpen)
				closeTextWorkspace();
			else
				openTextWorkspace();
		}
		if (!leavePromptOpen
			&& !environmentPanelOpen
			&& !textWorkspaceOpen
			&& !flowCardLibraryOpen()
			&& !flowDocumentPickerOpen()
			&& (editedName == null || !editedName.isEditing())
			&& (editedObjectName == null || !editedObjectName.isEditing())
			&& (editedAssetSearch == null || !editedAssetSearch.isEditing())
			&& !shortcutModifierDown()
			&& Raylib.IsKeyPressed(KeyboardKey.B)) {
			if (assetBrowserOpen)
				closeAssetBrowser();
			else
				openAssetBrowser();
		}
		if (!assetBrowserOpen && !textWorkspaceOpen && !flowCardLibraryOpen() && !flowDocumentPickerOpen() && saveShortcutPressed())
			requestSave();
		final keyboardNavigation = readKeyboardNavigation();
		final navigation = externalNavigation != NavigationCommand.None ? externalNavigation : keyboardNavigation;
		final navigationAction = applyNavigation(navigation);
		switch navigationAction {
			case StayInEditor:
			case ReturnToTitle | StartTestPlay(_):
				return navigationAction;
		}
		final shortcutInputAvailable = !leavePromptOpen
			&& !environmentPanelOpen
			&& !assetBrowserOpen
			&& !textWorkspaceOpen
			&& !flowCardLibraryOpen()
			&& !flowDocumentPickerOpen()
			&& (editedName == null || !editedName.isEditing())
			&& (editedObjectName == null || !editedObjectName.isEditing());
		switch objectShortcutAction({
			inputAvailable: shortcutInputAvailable,
			buildActive: workspaceView == BuildView,
			pointerCaptured: buildPointerState == EditorBuildPointerState.Captured,
			selectToolActive: activeTool == SelectTool,
			objectSelected: selectedObjectIndex() >= 0,
			objectHeld: objectGrabActive(objectGrab),
			duplicatePressed: duplicateShortcutPressed(),
			deletePressed: Raylib.IsKeyPressed(KeyboardKey.Backspace)
		}) {
			case NoObjectShortcut:
			case DuplicateSelectedObject:
				duplicateSelectedObject();
			case DeleteSelectedObject:
				deleteSelectedObject();
		}
		Raylib.ClearBackground(Color.rgba(12, 28, 36));
		if (textWorkspaceOpen) {
			drawTextWorkspace(locale, width, height);
			return StayInEditor;
		}
		if (assetBrowserOpen) {
			drawAssetBrowser(locale, width, height);
			return StayInEditor;
		}
		if (environmentPanelOpen) {
			drawEnvironmentPanel(locale, width, height);
			return StayInEditor;
		}
		if (flowCardLibraryOpen()) {
			final draft = currentDraftScenario();
			if (draft == null)
				flowCardLibraryTarget = NoFlowCardPanel;
			else
				applyFlowCardPanelAction(drawFlowCardPanel(uiCatalog, locale, flowCardLibraryTarget, draft, flowContentChoices, width, height));
			return StayInEditor;
		}
		if (flowDocumentPickerOpen()) {
			final draft = currentDraftScenario();
			if (draft == null)
				flowDocumentPickMode = NoFlowDocumentPanel;
			else
				applyFlowDocumentPanelAction(drawFlowDocumentPanel(uiCatalog, locale, flowDocumentPickMode, draft, width, height));
			return StayInEditor;
		}
		if (immersiveWorkspaceActive(workspaceView == BuildView, buildPointerState == EditorBuildPointerState.Captured)) {
			drawImmersiveBuild(locale, width, height, resources);
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

		final viewLeft = Std.int(width * 0.5) - 158.0;
		if (focusedButtonSized(EditorFocusTarget.Build, viewLeft, toolbarTop, 96.0, 38.0, uiCatalog.text(locale, UiMessage.EditorBuild)))
			setWorkspaceView(BuildView);
		if (focusedButtonSized(EditorFocusTarget.Plan, viewLeft + 104.0, toolbarTop, 96.0, 38.0, uiCatalog.text(locale, UiMessage.EditorPlan)))
			setWorkspaceView(PlanView);
		if (focusedButtonSized(EditorFocusTarget.Text, viewLeft + 208.0, toolbarTop, 96.0, 38.0, uiCatalog.text(locale, UiMessage.EditorText)))
			openTextWorkspace();
		final toolbarCamera = camera;
		if (toolbarCamera != null
			&& focusedButtonSized(EditorFocusTarget.CameraMode, viewLeft + 312.0, toolbarTop, 142.0, 38.0,
				cameraControlText(locale, cameraMode(toolbarCamera))))
			cycleEditorCamera();
		drawActiveControl(workspaceView == BuildView, Std.int(viewLeft), Std.int(toolbarTop), 96, 38);
		drawActiveControl(workspaceView == PlanView, Std.int(viewLeft + 104.0), Std.int(toolbarTop), 96, 38);

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
		final hasSelection = selection != null || selectedObjectIndex() >= 0;
		final inspectorVisible = shouldShowInspector(workspaceView == BuildView, hasSelection, detailsOpen, worldListOpen);
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
			drawWorldViewport(locale, innerLeft, innerTop, innerWidth, innerHeight, resources);
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

	/** Draw direct world editing with a compact five-tool hotbar. */
	function drawImmersiveBuild(locale:LocaleCursor, width:Int, height:Int, resources:EditorRenderResources):Void {
		final inset = 16;
		drawWorldViewport(locale, inset, inset, width - inset * 2, height - inset * 2, resources);
		Raylib.DrawRectangleLines(inset, inset, width - inset * 2, height - inset * 2, CaxecraftPalette.selection());
		drawImmersiveHotbar(locale, width, height, inset);
	}

	/** Show every direct Build tool in the same order as number-key input. */
	function drawImmersiveHotbar(locale:LocaleCursor, width:Int, height:Int, inset:Int):Void {
		final gap = 8;
		final availableWidth = width - inset * 2 - 40;
		final fittedWidth = Std.int((availableWidth - gap * 4) / 5);
		final slotWidth = fittedWidth < 170 ? fittedWidth : 170;
		final barWidth = slotWidth * 5 + gap * 4;
		final left = Std.int((width - barWidth) / 2);
		final top = height - inset - 78;
		for (slot in 1...6) {
			final tool = toolForBuildHotbarSlot(slot);
			if (tool != null) {
				final slotLeft = left + (slot - 1) * (slotWidth + gap);
				Raylib.DrawRectangle(slotLeft, top, slotWidth, 42, Color.rgba(8, 20, 24));
				Raylib.DrawRectangle(slotLeft + 6, top + 7, 28, 28, immersiveToolColor(slot));
				Raylib.DrawTextString(Std.string(slot), slotLeft + 16, top + 13, 16, Color.rgba(10, 24, 30));
				Raylib.DrawTextString(immersiveToolLabel(locale, tool), slotLeft + 42, top + 13, 16, CaxecraftPalette.hudText());
				if (activeTool == tool) {
					Raylib.DrawRectangleLines(slotLeft, top, slotWidth, 42, CaxecraftPalette.selection());
					Raylib.DrawRectangleLines(slotLeft + 1, top + 1, slotWidth - 2, 40, CaxecraftPalette.selection());
				}
			}
		}
	}

	/** Return the localized name for one visible Build hotbar tool. */
	function immersiveToolLabel(locale:LocaleCursor, tool:EditorTool):String {
		return switch tool {
			case SelectTool: uiCatalog.text(locale, UiMessage.EditorSelect);
			case PaintTool | EraseTool | FillTool: groundMaterialLabel(locale);
			case CheckpointTool: uiCatalog.text(locale, UiMessage.EditorCheckpoint);
			case CatalogObjectTool: selectedObjectAssetLabel(locale);
			case TriggerZoneTool: uiCatalog.text(locale, UiMessage.EditorTrigger);
		};
	}

	/** Name the active map material in the Ground hotbar slot. */
	function groundMaterialLabel(locale:LocaleCursor):String {
		final draft = presentationDraft;
		if (draft == null)
			return uiCatalog.text(locale, UiMessage.EditorGround);
		for (entry in draft.world.palette)
			if (entry.code == groundPaletteCode)
				return '${uiCatalog.text(locale, UiMessage.EditorGround)} · ${entry.blockType.text()}';
		return uiCatalog.text(locale, UiMessage.EditorGround);
	}

	/** Return the current map's compact code for the pack's default brush. */
	function defaultGroundPaletteCode():Int {
		final draft = presentationDraft;
		return draft == null ? -1 : paletteCodeForBlock(draft.world.palette, contentRegistry.defaultEditorBlockId());
	}

	/** Select one admitted Ground material without changing document history. */
	function selectGroundPalette(next:Int):Void {
		if (next < 0 || next == groundPaletteCode)
			return;
		groundPaletteCode = next;
		invalidatePreview();
	}

	/** Match each compact slot to its released-pointer creation card. */
	static function immersiveToolColor(slot:Int):Color {
		return switch slot {
			case 1: Color.rgba(84, 191, 205);
			case 2: Color.rgba(111, 174, 91);
			case 3: Color.rgba(76, 209, 198);
			case 4: Color.rgba(226, 151, 72);
			case 5: Color.rgba(210, 105, 230);
			case _: CaxecraftPalette.hudText();
		};
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
		drawAssetBrowserCard(locale, catalogSlot, left + 12 + (cardWidth + cardGap) * (catalogSlot - 1), cardTop, cardWidth, 68);
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

	/** Open the complete content-derived browser from its visible creation card. */
	function drawAssetBrowserCard(locale:LocaleCursor, slot:Int, left:Int, top:Int, width:Int, height:Int):Void {
		final pressed = Raygui.ButtonString(Rectangle.fromFloat(left, top, width, height), "").has(GuiResult.Pressed);
		if (pressed) {
			focusedControl = EditorFocusTarget.CatalogObjectTool;
			openAssetBrowser();
		}
		Raylib.DrawRectangle(left + 12, top + 14, 36, 36, Color.rgba(226, 151, 72));
		Raylib.DrawRectangleLines(left + 12, top + 14, 36, 36, CaxecraftPalette.hudText());
		Raylib.DrawTextString(Std.string(slot), left + 25, top + 23, 18, Color.rgba(10, 24, 30));
		Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorAssetBrowser), left + 58, top + 16, 16, CaxecraftPalette.hudText());
		Raylib.DrawTextString(selectedObjectAssetLabel(locale), left + 58, top + 39, 13, Color.rgba(126, 205, 209));
		drawFocusRing(EditorFocusTarget.CatalogObjectTool, left, top, width, height);
		drawActiveControl(activeTool == EditorTool.CatalogObjectTool, left, top, width, height);
	}

	/** Draw one modal list over the editor without creating a second document. */
	function drawAssetBrowser(locale:LocaleCursor, width:Int, height:Int):Void {
		final panelWidth = width - 64 < 880 ? width - 64 : 880;
		final panelHeight = height - 64 < 620 ? height - 64 : 620;
		final left = Std.int((width - panelWidth) / 2);
		final top = Std.int((height - panelHeight) / 2);
		Raygui.PanelString(Rectangle.fromFloat(left, top, panelWidth, panelHeight), uiCatalog.text(locale, UiMessage.EditorAssetBrowser));

		final categories = assetCategories;
		final categoryGap = 8;
		final categoryWidth = Std.int((panelWidth - 48 - categoryGap * (categories.length - 1)) / categories.length);
		for (index in 0...categories.length) {
			final category = categories[index];
			final categoryLeft = left + 24 + index * (categoryWidth + categoryGap);
			if (Raygui.ButtonString(Rectangle.fromFloat(categoryLeft, top + 46, categoryWidth, 38), uiCatalog.text(locale, assetCategoryMessage(category)))
				.has(GuiResult.Pressed))
				setAssetCategory(category);
			if (assetCategory == category)
				drawActiveControl(true, categoryLeft, top + 46, categoryWidth, 38);
		}

		Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorAssetSearch), left + 24, top + 102, 15, Color.rgba(126, 205, 209));
		final search = assetSearch;
		if (search != null) {
			search.draw(Rectangle.fromFloat(left + 168, top + 94, panelWidth - 192, 34));
			final nextQuery = search.text();
			if (nextQuery != assetQuery) {
				assetQuery = nextQuery;
				refreshVisibleAssets(true);
			}
		}

		final visible = visibleAssetEntries();
		if (assetSelection >= visible.length)
			assetSelection = visible.length == 0 ? -1 : visible.length - 1;
		if (assetSelection < 0 && visible.length > 0)
			assetSelection = 0;
		final rowHeight = 55;
		final rowGap = 6;
		final listTop = top + 144;
		final availableRows = Std.int((panelHeight - 218) / (rowHeight + rowGap));
		final rowCount = availableRows < 1 ? 1 : availableRows;
		var firstRow = assetSelection < rowCount ? 0 : assetSelection - rowCount + 1;
		if (firstRow + rowCount > visible.length)
			firstRow = visible.length - rowCount < 0 ? 0 : visible.length - rowCount;
		if (visible.length == 0)
			Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorAssetEmpty), left + 28, listTop + 18, 19, CaxecraftPalette.hudText());
		else {
			final lastRow = firstRow + rowCount < visible.length ? firstRow + rowCount : visible.length;
			for (index in firstRow...lastRow) {
				final entry = visible[index];
				final rowTop = listTop + (index - firstRow) * (rowHeight + rowGap);
				if (Raygui.ButtonString(Rectangle.fromFloat(left + 24, rowTop, panelWidth - 48, rowHeight), "").has(GuiResult.Pressed)) {
					assetSelection = index;
					chooseAsset(entry);
				}
				drawAssetMark(entry.category, left + 36, rowTop + 10, 34);
				Raylib.DrawTextString(editorAssetLabel(entry, locale), left + 84, rowTop + 8, 19, CaxecraftPalette.hudText());
				Raylib.DrawTextString(editorAssetHelp(entry, locale), left + 84, rowTop + 32, 13, Color.rgba(126, 205, 209));
				if (assetSelection == index)
					drawActiveControl(true, left + 24, rowTop, panelWidth - 48, rowHeight);
			}
		}

		Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorAssetShortcut), left + 24, top + panelHeight - 40, 14, Color.rgba(126, 205, 209));
		if (Raygui.ButtonString(Rectangle.fromFloat(left + panelWidth - 132, top + panelHeight - 48, 108, 30),
			uiCatalog.text(locale, UiMessage.EditorAssetClose))
			.has(GuiResult.Pressed))
			closeAssetBrowser();
	}

	/**
		Draw the complete copied CAXEMAP draft without making it a second model.

		Rows are read-only navigation targets. The selected row uses the same owned
		Raygui UTF-8 buffer as other native fields. Apply is the only operation that
		can cross into `EditorSession`, and it does so through `ApplyText`.
	**/
	function drawTextWorkspace(locale:LocaleCursor, width:Int, height:Int):Void {
		final document = textDocument;
		if (document == null) {
			closeTextWorkspace();
			return;
		}
		final panelLeft = 24;
		final panelTop = 24;
		final panelWidth = width - 48;
		final panelHeight = height - 48;
		Raygui.PanelString(Rectangle.fromFloat(panelLeft, panelTop, panelWidth, panelHeight), uiCatalog.text(locale, UiMessage.EditorTextTitle));
		Raylib.DrawTextString(uiCatalog.text(locale, UiMessage.EditorTextHelp), panelLeft + 20, panelTop + 38, 14, Color.rgba(126, 205, 209));

		final buttonTop = panelTop + 64;
		if (Raygui.ButtonString(Rectangle.fromFloat(panelLeft + 20, buttonTop, 154, 32), uiCatalog.text(locale, UiMessage.EditorTextApply))
			.has(GuiResult.Pressed))
			applyTextWorkspace();
		if (Raygui.ButtonString(Rectangle.fromFloat(panelLeft + 184, buttonTop, 154, 32), uiCatalog.text(locale, UiMessage.EditorTextReset))
			.has(GuiResult.Pressed))
			resetTextWorkspace();
		if (Raygui.ButtonString(Rectangle.fromFloat(panelLeft + 348, buttonTop, 126, 32), uiCatalog.text(locale, UiMessage.EditorTextAddLine))
			.has(GuiResult.Pressed))
			insertTextLine();
		if (Raygui.ButtonString(Rectangle.fromFloat(panelLeft + 484, buttonTop, 142, 32), uiCatalog.text(locale, UiMessage.EditorTextDeleteLine))
			.has(GuiResult.Pressed))
			removeTextLine();
		if (Raygui.ButtonString(Rectangle.fromFloat(panelLeft + panelWidth - 126, buttonTop, 106, 32), uiCatalog.text(locale, UiMessage.EditorTextClose))
			.has(GuiResult.Pressed))
			closeTextWorkspace();
		if (!textWorkspaceOpen)
			return;

		final sourceTop = panelTop + 108;
		final editorTop = panelTop + panelHeight - 112;
		final sourceBottom = editorTop - 12;
		final rowHeight = 24;
		final visibleRows = Std.int((sourceBottom - sourceTop) / rowHeight);
		ensureTextLineVisible(visibleRows);
		final wheel = Raylib.GetMouseWheelMove().toFloat();
		if (wheel > 0.0)
			textScrollLine -= 3;
		else if (wheel < 0.0)
			textScrollLine += 3;
		clampTextScroll(visibleRows);
		Raylib.BeginScissorMode(panelLeft + 16, sourceTop, panelWidth - 32, sourceBottom - sourceTop);
		final last = textScrollLine + visibleRows < document.lineCount() ? textScrollLine + visibleRows : document.lineCount();
		for (lineIndex in textScrollLine...last) {
			final rowTop = sourceTop + (lineIndex - textScrollLine) * rowHeight;
			final diagnostic = textLineHasDiagnostic(lineIndex);
			if (lineIndex == textSelectedLine)
				Raylib.DrawRectangle(panelLeft + 16, rowTop, panelWidth - 32, rowHeight - 1, Color.rgba(27, 56, 65));
			else if (diagnostic)
				Raylib.DrawRectangle(panelLeft + 16, rowTop, panelWidth - 32, rowHeight - 1, Color.rgba(71, 35, 24));
			if (Raygui.ButtonString(Rectangle.fromFloat(panelLeft + 16, rowTop, panelWidth - 32, rowHeight - 1), "").has(GuiResult.Pressed))
				selectTextLine(lineIndex);
			Raylib.DrawTextString('${lineIndex + 1}', panelLeft + 24, rowTop + 5, 13, diagnostic ? Color.rgba(255, 154, 112) : Color.rgba(100, 143, 151));
			final line = document.lineAt(lineIndex);
			final visible = line == null ? "" : visibleTextLine(line);
			Raylib.DrawTextString(visible, panelLeft + 82, rowTop + 5, 13, textLineColor(visible));
		}
		Raylib.EndScissorMode();

		final lineEditor = textLineEditor;
		if (lineEditor != null) {
			Raylib.DrawTextString('${textSelectedLine + 1}', panelLeft + 20, editorTop + 9, 14, Color.rgba(126, 205, 209));
			final result = lineEditor.draw(Rectangle.fromFloat(panelLeft + 68, editorTop, panelWidth - 88, 34));
			if (result.has(GuiResult.Pressed) && !lineEditor.isEditing())
				commitTextLine();
		}
		final statusTop = editorTop + 44;
		final statusMessage = switch textNotice {
			case TextClean: UiMessage.EditorTextClean;
			case TextDirty: UiMessage.EditorTextDirty;
			case TextInvalid: UiMessage.EditorTextInvalid;
			case TextStale: UiMessage.EditorTextStale;
		};
		final statusColor = textNotice == TextInvalid || textNotice == TextStale ? Color.rgba(255, 154, 112) : CaxecraftPalette.hudText();
		Raylib.DrawTextString(uiCatalog.text(locale, statusMessage), panelLeft + 20, statusTop, 14, statusColor);
		if (textDiagnostics.length > 0) {
			final diagnostic = scenarioDiagnosticMessage(textDiagnostics[0]);
			Raylib.DrawTextString(visibleTextLine(uiCatalog.format(locale, diagnostic.message, diagnostic.arguments)), panelLeft + 20, statusTop + 24, 13,
				Color.rgba(255, 190, 132));
		}
	}

	/** Open a retained source draft, refreshing clean source after visual edits. */
	function openTextWorkspace():Void {
		final current = session;
		if (current == null)
			return;
		final retained = textDocument;
		if (retained == null || (!retained.isDirty() && textBaseRevision != current.revision()))
			resetTextWorkspace();
		else if (textBaseRevision != current.revision())
			textNotice = TextStale;
		textWorkspaceOpen = textDocument != null;
		assetBrowserOpen = false;
		environmentPanelOpen = false;
		flowCardLibraryTarget = NoFlowCardPanel;
		flowDocumentPickMode = NoFlowDocumentPanel;
		setBuildPointerState(nextPointerState(buildPointerState, false, Raylib.IsWindowFocused(), false, false));
		focusedControl = EditorFocusTarget.Text;
	}

	/** Keep unapplied invalid source in memory while returning to visual editing. */
	function closeTextWorkspace():Void {
		commitTextLine();
		final lineEditor = textLineEditor;
		if (lineEditor != null)
			lineEditor.setEditing(false);
		textWorkspaceOpen = false;
		focusedControl = EditorFocusTarget.Text;
	}

	/** Discard source-only edits and copy the current canonical visual draft. */
	function resetTextWorkspace():Bool {
		final current = session;
		if (current == null)
			return false;
		return switch EditorTextDocument.open(current.canonicalDraft()) {
			case TextDocumentOpenRejected(_):
				textNotice = TextInvalid;
				false;
			case TextDocumentOpened(document):
				textDocument = document;
				textBaseRevision = current.revision();
				textDiagnostics = [];
				textNotice = TextClean;
				textSelectedLine = textSelectedLine < document.lineCount() ? textSelectedLine : document.lineCount() - 1;
				if (textSelectedLine < 0)
					textSelectedLine = 0;
				textScrollLine = 0;
				syncTextLineEditor();
				true;
		}
	}

	/** Publish valid source as one canonical whole-document session mutation. */
	function applyTextWorkspace():Bool {
		final current = session;
		final document = textDocument;
		if (current == null || document == null)
			return false;
		commitTextLine();
		return switch current.mutate({baseRevision: textBaseRevision, mutation: ApplyText(document.snapshot())}) {
			case MutationApplied(_, _, _, _, _, _) | MutationUnchanged(_, _):
				notice = Ready;
				refreshProjection(false, RefreshAllTerrain);
				resetTextWorkspace();
			case MutationRejected(RevisionConflict(_, _), _):
				textDiagnostics = [];
				textNotice = TextStale;
				notice = Invalid;
				false;
			case MutationRejected(SnapshotRejected(diagnostics), _):
				textDiagnostics = diagnostics.copy();
				textNotice = TextInvalid;
				notice = Invalid;
				if (diagnostics.length > 0)
					selectTextLine(diagnostics[0].coordinate.line - 1);
				false;
			case MutationRejected(_, _):
				textDiagnostics = [];
				textNotice = TextInvalid;
				notice = Invalid;
				false;
		}
	}

	/** Commit the selected native edit buffer into the isolated source owner. */
	function commitTextLine():Bool {
		final document = textDocument;
		final lineEditor = textLineEditor;
		if (document == null || lineEditor == null)
			return false;
		return switch document.replaceLine(textSelectedLine, lineEditor.text()) {
			case TextEditApplied:
				textDiagnostics = [];
				textNotice = textBaseRevision == currentSessionRevision() ? TextDirty : TextStale;
				true;
			case TextEditUnchanged:
				true;
			case TextEditRejected(_):
				textNotice = TextInvalid;
				false;
		}
	}

	/** Insert one blank source line and move editing focus to it. */
	function insertTextLine():Void {
		final document = textDocument;
		if (document == null || !commitTextLine())
			return;
		switch document.insertLineAfter(textSelectedLine) {
			case TextEditApplied:
				textSelectedLine++;
				textNotice = textBaseRevision == currentSessionRevision() ? TextDirty : TextStale;
				syncTextLineEditor();
				final editor = textLineEditor;
				if (editor != null)
					editor.setEditing(true);
			case TextEditUnchanged:
			case TextEditRejected(_):
				textNotice = TextInvalid;
		}
	}

	/** Remove one line and keep selection inside the remaining source. */
	function removeTextLine():Void {
		final document = textDocument;
		if (document == null || !commitTextLine())
			return;
		switch document.removeLine(textSelectedLine) {
			case TextEditApplied:
				if (textSelectedLine >= document.lineCount())
					textSelectedLine = document.lineCount() - 1;
				textNotice = textBaseRevision == currentSessionRevision() ? TextDirty : TextStale;
				syncTextLineEditor();
			case TextEditUnchanged:
			case TextEditRejected(_):
				textNotice = TextInvalid;
		}
	}

	/** Select one source row after preserving edits in the old row. */
	function selectTextLine(index:Int):Void {
		final document = textDocument;
		if (document == null || index < 0 || index >= document.lineCount())
			return;
		if (index != textSelectedLine && !commitTextLine())
			return;
		textSelectedLine = index;
		syncTextLineEditor();
	}

	/** Copy the selected line into the fixed native UTF-8 edit buffer. */
	function syncTextLineEditor():Void {
		final document = textDocument;
		final lineEditor = textLineEditor;
		if (document == null || lineEditor == null)
			return;
		final line = document.lineAt(textSelectedLine);
		lineEditor.setEditing(false);
		if (!lineEditor.replace(line == null ? "" : line))
			textNotice = TextInvalid;
	}

	/** Keep the selected line inside one bounded visible source window. */
	function ensureTextLineVisible(visibleRows:Int):Void {
		if (textSelectedLine < textScrollLine)
			textScrollLine = textSelectedLine;
		else if (textSelectedLine >= textScrollLine + visibleRows)
			textScrollLine = textSelectedLine - visibleRows + 1;
		clampTextScroll(visibleRows);
	}

	/** Clamp wheel and selection scrolling to the copied document. */
	function clampTextScroll(visibleRows:Int):Void {
		final document = textDocument;
		if (document == null) {
			textScrollLine = 0;
			return;
		}
		final maximum = document.lineCount() > visibleRows ? document.lineCount() - visibleRows : 0;
		if (textScrollLine < 0)
			textScrollLine = 0;
		else if (textScrollLine > maximum)
			textScrollLine = maximum;
	}

	/** True when any current parser or validator diagnostic points at this row. */
	function textLineHasDiagnostic(index:Int):Bool {
		for (diagnostic in textDiagnostics)
			if (diagnostic.coordinate.line - 1 == index)
				return true;
		return false;
	}

	/** Draw a bounded preview; the full selected line remains in the edit box. */
	static function visibleTextLine(value:String):String
		return value.length <= 150 ? value : value.substring(0, 147) + "...";

	/** Use a small stable syntax palette without maintaining a grammar duplicate. */
	static function textLineColor(value:String):Color {
		var index = 0;
		while (index < value.length && value.charCodeAt(index) == 32)
			index++;
		final token = index < value.length ? value.substring(index) : "";
		if (StringTools.startsWith(token, "#"))
			return Color.rgba(100, 143, 151);
		if (StringTools.startsWith(token, "rule ")
			|| StringTools.startsWith(token, "sequence ")
			|| StringTools.startsWith(token, "variable "))
			return Color.rgba(210, 105, 230);
		if (StringTools.startsWith(token, "when ") || StringTools.startsWith(token, "if "))
			return Color.rgba(84, 191, 205);
		if (StringTools.startsWith(token, "do ") || StringTools.startsWith(token, "choice "))
			return Color.rgba(111, 174, 91);
		return CaxecraftPalette.hudText();
	}

	/** Current revision without leaking the mutable session into text helpers. */
	function currentSessionRevision():Int {
		final current = session;
		return current == null ? -1 : current.revision();
	}

	/** Give every category a stable shape and color without duplicating asset art. */
	static function drawAssetMark(category:EditorAssetCategory, left:Int, top:Int, size:Int):Void {
		final color = switch category {
			case TerrainAssets: Color.rgba(111, 174, 91);
			case ItemAssets: Color.rgba(226, 151, 72);
			case NpcAssets: Color.rgba(84, 191, 205);
			case EnemyAssets: Color.rgba(218, 103, 78);
			case MechanismAssets: Color.rgba(210, 105, 230);
		};
		Raylib.DrawRectangle(left, top, size, size, color);
		Raylib.DrawRectangleLines(left, top, size, size, CaxecraftPalette.hudText());
		final inset = 5 + categoryIndex(category) * 2;
		Raylib.DrawRectangleLines(left + inset, top + inset, size - inset * 2, size - inset * 2, Color.rgba(10, 24, 30));
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
			syncObjectName(gizmo.id);
			final selectedName = objectName;
			if (selectedName != null) {
				final result = selectedName.draw(Rectangle.fromFloat(left + 14, cursorTop, width - 28, 30));
				if (result.has(GuiResult.Pressed) && !selectedName.isEditing())
					commitObjectName(selectedName.text());
			} else
				Raylib.DrawTextString(gizmo.id.text(), left + 14, cursorTop, 16, CaxecraftPalette.selection());
			cursorTop += 36;
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
				if (gizmo.kind == TriggerZoneGizmo)
					cursorTop = drawSelectedTriggerFlowCards(locale, gizmo.id, left + 14, cursorTop, width - 28, top + height - 12);
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

	/** Draw registry-backed WHEN / IF / DO cards connected to one trigger. */
	function drawSelectedTriggerFlowCards(locale:LocaleCursor, zone:ScenarioId, left:Int, top:Int, width:Int, bottom:Int):Int {
		var cursor = top;
		if (cursor + 34 < bottom) {
			final connectLabel = uiCatalog.format(locale, EditorFlowUiMessage.ConnectObjectMessage.messageId(), []);
			if (Raygui.ButtonString(Rectangle.fromFloat(left, cursor, width, 30), connectLabel).has(GuiResult.Pressed))
				beginZoneConnection(zone);
			cursor += 38;
		}
		for (overlap in flowOverlaps)
			if (overlap.first.text() == zone.text() || overlap.second.text() == zone.text()) {
				if (cursor + 26 >= bottom)
					return cursor;
				Raylib.DrawRectangle(left, cursor, width, 22, Color.rgba(71, 48, 15));
				Raylib.DrawTextString(uiCatalog.format(locale, EditorFlowUiMessage.OverlapMessage.messageId(), [overlap.first.text(), overlap.second.text()]),
					left + 5, cursor + 4, 11, Color.rgba(255, 211, 103));
				cursor += 26;
			}
		for (rule in flowRules) {
			if (!flowRuleUsesZone(rule, zone))
				continue;
			if (cursor + 20 < bottom) {
				Raylib.DrawTextString(rule.ruleId.text(), left, cursor, 13, Color.rgba(236, 114, 255));
				if (Raygui.ButtonString(Rectangle.fromFloat(left + width - 26, cursor - 3, 24, 20), "+").has(GuiResult.Pressed)) {
					flowCardLibraryTarget = InsertDoFlowCardPanel(zone, rule.ruleId, rule.cards.length - 2);
					return cursor + 20;
				}
				cursor += 20;
			}
			for (card in rule.cards) {
				if (cursor + 34 >= bottom)
					return cursor;
				final visual = flowCardVisual(locale, card);
				Raylib.DrawRectangle(left, cursor, width, 30, Color.rgba(14, 31, 38));
				Raylib.DrawRectangle(left + 6, cursor + 6, 18, 18, visual.color);
				Raylib.DrawTextString(visual.keyword, left + 30, cursor + 7, 13, visual.color);
				Raylib.DrawTextString(visual.summary, left + 88, cursor + 7, 13, CaxecraftPalette.hudText());
				if (drawFlowCardEditControls(locale, zone, rule.ruleId, card, left, cursor, width))
					return cursor + 34;
				drawFlowReferencePickers(zone, rule.ruleId, card, left, cursor, width, flowCardControlWidth(card));
				cursor += 34;
			}
			cursor = drawNestedFlowCards(locale, zone, rule.ruleId, rule.nestedCards, left, cursor, width, bottom);
		}
		return drawFlowTraceOverlay(locale, left, cursor, width, bottom);
	}

	/** Draw explicit editable rows for nested predicates and weighted actions. */
	function drawNestedFlowCards(locale:LocaleCursor, zone:ScenarioId, rule:ScenarioId, cards:Array<EditorFlowNestedCard>, left:Int, top:Int, width:Int,
			bottom:Int):Int {
		var cursor = top;
		for (card in cards) {
			if (cursor + 30 >= bottom)
				return cursor;
			final visual = nestedFlowCardVisual(locale, card);
			final inset = visual.depth * 12;
			Raylib.DrawRectangle(left + inset, cursor, width - inset, 26, Color.rgba(11, 25, 31));
			Raylib.DrawTextString("↳", left + inset + 5, cursor + 5, 13, visual.color);
			Raylib.DrawTextString(visual.summary, left + inset + 24, cursor + 5, 12, CaxecraftPalette.hudText());
			if (Raygui.ButtonString(Rectangle.fromFloat(left + width - 50, cursor + 2, 47, 22),
				uiCatalog.format(locale, EditorFlowUiMessage.EditMessage.messageId(), []))
				.has(GuiResult.Pressed)) {
				switch card {
					case NestedIfFlowCard(path, _, _, _, _):
						flowCardLibraryTarget = NestedIfFlowCardPanel(zone, rule, path.copy());
					case NestedDoFlowCard(parentActionIndex, choiceIndex, actionIndex, _, _, _, _):
						flowCardLibraryTarget = NestedDoFlowCardPanel(zone, rule, parentActionIndex, choiceIndex, actionIndex);
				}
				return cursor + 30;
			}
			cursor += 30;
		}
		return cursor;
	}

	/** Draw the latest non-empty ordinary-engine trace under the selected cards. */
	function drawFlowTraceOverlay(locale:LocaleCursor, left:Int, top:Int, width:Int, bottom:Int):Int {
		var cursor = top;
		for (row in flowTrace.rows) {
			if (cursor + 20 >= bottom)
				return cursor;
			Raylib.DrawRectangle(left, cursor, width, 18, Color.rgba(9, 23, 29));
			Raylib.DrawTextString(flowTraceText(locale, row), left + 6, cursor + 2, 12, CaxecraftPalette.hudText());
			cursor += 20;
		}
		if (flowTrace.truncated && cursor + 20 < bottom) {
			Raylib.DrawTextString("…", left + 6, cursor + 2, 12, CaxecraftPalette.hudText());
			cursor += 20;
		}
		return cursor;
	}

	/** Format stable trace IDs with localized card keywords and technical symbols. */
	function flowTraceText(locale:LocaleCursor, row:EditorFlowTraceRow):String
		return switch row {
			case SourceTrace(event, actor):
				uiCatalog.format(locale, EditorFlowUiMessage.WhenMessage.messageId(), [])
				+ "  "
				+ event
				+ (actor == null ? "" : "  " + actor.text());
			case PredicateTrace(rule, event, actor, passed):
				uiCatalog.format(locale, EditorFlowUiMessage.IfMessage.messageId(), [])
				+ "  "
				+ rule.text()
				+ "  "
				+ event
				+ (actor == null ? "" : "  " + actor.text())
				+ (passed ? "  ✓" : "  ×");
			case ActionTrace(owner, action):
				uiCatalog.format(locale, EditorFlowUiMessage.DoMessage.messageId(), [])
				+ "  "
				+ owner.text()
				+ "  "
				+ action;
			case FollowUpTrace(owner, event, readyTick):
				'→  ${owner.text()}  $event  @${readyTick.epoch}:${readyTick.offset}';
			case SequenceTrace(owner, timer, sequence, readyTick):
				'→  ${owner.text()}  ${timer.text()}  ${sequence.text()}  @${readyTick.epoch}:${readyTick.offset}';
		};

	/** Draw one numbered target button for each world-pickable card reference. */
	function drawFlowReferencePickers(zone:ScenarioId, rule:ScenarioId, card:EditorFlowCard, left:Int, top:Int, width:Int, rightInset:Int):Void {
		final references = flowCardReferences(card);
		var visibleIndex = 0;
		for (referenceIndex in 0...references.length) {
			final reference = references[referenceIndex];
			final worldPick = isWorldPickableFlowRole(reference.role);
			final documentPick = isDocumentFlowReferenceRole(reference.role);
			if (!worldPick && !documentPick)
				continue;
			final buttonLeft = left + width - rightInset - 29 * (visibleIndex + 1);
			if (Raygui.ButtonString(Rectangle.fromFloat(buttonLeft, top + 3, 26, 24), '${worldPick ? "◎" : "#"}${referenceIndex + 1}')
				.has(GuiResult.Pressed)) {
				if (worldPick)
					beginFlowReferencePick(zone, rule, flowCardAddress(card), referenceIndex);
				else
					beginFlowDocumentPick(zone, rule, flowCardAddress(card), referenceIndex, reference.role);
			}
			visibleIndex++;
		}
	}

	/** Keep fixed card controls separate from typed field-picker buttons. */
	static function flowCardControlWidth(card:EditorFlowCard):Int
		return switch card {
			case WhenFlowCard(_, _, _): 56;
			case IfFlowCard(_, _, _): 56;
			case DoFlowCard(_, _, _, _): 148;
		};

	/** Draw minimal complete mutation controls over the canonical card model. */
	function drawFlowCardEditControls(locale:LocaleCursor, zone:ScenarioId, ruleId:ScenarioId, card:EditorFlowCard, left:Int, top:Int, width:Int):Bool {
		return switch card {
			case WhenFlowCard(_, _, _):
				if (Raygui.ButtonString(Rectangle.fromFloat(left + width - 53, top + 3, 50, 24),
					uiCatalog.format(locale, EditorFlowUiMessage.EditMessage.messageId(), []))
					.has(GuiResult.Pressed)) {
					flowCardLibraryTarget = WhenFlowCardPanel(zone, ruleId);
					true;
				} else false;
			case IfFlowCard(_, _, _):
				if (Raygui.ButtonString(Rectangle.fromFloat(left + width - 53, top + 3, 50, 24),
					uiCatalog.format(locale, EditorFlowUiMessage.EditMessage.messageId(), []))
					.has(GuiResult.Pressed)) {
					flowCardLibraryTarget = IfFlowCardPanel(zone, ruleId);
					true;
				} else false;
			case DoFlowCard(index, _, _, _):
				final first = left + width - 145;
				if (Raygui.ButtonString(Rectangle.fromFloat(first, top + 3, 50, 24), uiCatalog.format(locale, EditorFlowUiMessage.EditMessage.messageId(), []))
					.has(GuiResult.Pressed)) {
					flowCardLibraryTarget = DoFlowCardPanel(zone, ruleId, index);
					true;
				} else if (Raygui.ButtonString(Rectangle.fromFloat(first + 53, top + 3, 26, 24), "^")
					.has(GuiResult.Pressed)) commitFlowEdit(zone, ruleId,
						MoveDo(index,
							index - 1)); else if (Raygui.ButtonString(Rectangle.fromFloat(first + 82, top + 3, 26, 24), "v")
					.has(GuiResult.Pressed)) commitFlowEdit(zone, ruleId,
						MoveDo(index,
							index + 1)); else if (Raygui.ButtonString(Rectangle.fromFloat(first + 111, top + 3, 26, 24), "X")
					.has(GuiResult.Pressed)) commitFlowEdit(zone, ruleId, RemoveDo(index)); else false;
		};
	}

	/** Return the immutable references carried by any projected card kind. */
	static function flowCardReferences(card:EditorFlowCard):Array<EditorFlowReference>
		return switch card {
			case WhenFlowCard(_, _, references) | IfFlowCard(_, _, references) | DoFlowCard(_, _, _, references): references;
		};

	/** Convert a projected row to the canonical card address used by mutation. */
	static function flowCardAddress(card:EditorFlowCard):EditorFlowCardAddress
		return switch card {
			case WhenFlowCard(_, _, _): WhenCardAddress;
			case IfFlowCard(_, _, _): IfCardAddress;
			case DoFlowCard(index, _, _, _): DoCardAddress(index);
		};

	/** True when a projected rule's source card points at this trigger. */
	static function flowRuleUsesZone(rule:EditorFlowRuleProjection, zone:ScenarioId):Bool {
		for (card in rule.cards)
			switch card {
				case WhenFlowCard(_, _, references):
					for (reference in references)
						if (reference.id.text() == zone.text())
							return true;
				case IfFlowCard(_, _, _) | DoFlowCard(_, _, _, _):
			}
		return false;
	}

	/** Resolve data-owned card prose while retaining descriptor-backed content. */
	function flowCardVisual(locale:LocaleCursor, card:EditorFlowCard):{final keyword:String; final summary:String; final color:Color;} {
		return switch card {
			case WhenFlowCard(_, cardText, _): {
					keyword: uiCatalog.format(locale, EditorFlowUiMessage.WhenMessage.messageId(), []),
					summary: uiCatalog.format(locale, cardText.message, cardText.arguments),
					color: Color.rgba(210, 105, 230)
				};
			case IfFlowCard(_, cardText, _): {
					keyword: uiCatalog.format(locale, EditorFlowUiMessage.IfMessage.messageId(), []),
					summary: uiCatalog.format(locale, cardText.message, cardText.arguments),
					color: Color.rgba(84, 191, 205)
				};
			case DoFlowCard(_, _, cardText, _): {
					keyword: uiCatalog.format(locale, EditorFlowUiMessage.DoMessage.messageId(), []),
					summary: uiCatalog.format(locale, cardText.message, cardText.arguments),
					color: Color.rgba(111, 174, 91)
				};
		};
	}

	/** Resolve one nested row without losing its typed descriptor or address. */
	function nestedFlowCardVisual(locale:LocaleCursor, card:EditorFlowNestedCard):{final depth:Int; final summary:String; final color:Color;} {
		return switch card {
			case NestedIfFlowCard(_, depth, _, text, _): {
					depth: depth,
					summary: uiCatalog.format(locale, text.message, text.arguments),
					color: Color.rgba(84, 191, 205)
				};
			case NestedDoFlowCard(_, choiceIndex, actionIndex, depth, _, text, _): {
					depth: depth,
					summary: '${choiceIndex + 1}.${actionIndex + 1}  ${uiCatalog.format(locale, text.message, text.arguments)}',
					color: Color.rgba(111, 174, 91)
				};
		};
	}

	/** Apply one typed palette decision through the canonical rule command. */
	function applyFlowCardPanelAction(action:CaxecraftFlowCardPanelAction):Void {
		switch action {
			case KeepFlowCardPanel:
			case CloseFlowCardPanel:
				flowCardLibraryTarget = NoFlowCardPanel;
			case SelectFlowEvent(zone, rule, value):
				if (commitFlowEdit(zone, rule, ReplaceWhen(value)))
					flowCardLibraryTarget = NoFlowCardPanel;
			case SelectFlowPredicate(zone, rule, path, value):
				final edit = path == null ? ReplaceIf(value) : ReplaceNestedIf(path, value);
				if (commitFlowEdit(zone, rule, edit))
					flowCardLibraryTarget = NoFlowCardPanel;
			case SelectFlowAction(zone, rule, actionIndex, choiceIndex, nestedActionIndex, insert, value):
				final edit = choiceIndex >= 0 ? ReplaceNestedChoiceDo(actionIndex, choiceIndex, nestedActionIndex,
					value) : insert ? InsertDo(actionIndex, value) : ReplaceDo(actionIndex, value);
				if (commitFlowEdit(zone, rule, edit))
					flowCardLibraryTarget = NoFlowCardPanel;
		}
	}

	/** Apply one revision-bound document selection through typed reference traversal. */
	function applyFlowDocumentPanelAction(action:CaxecraftFlowDocumentPanelAction):Void {
		switch action {
			case KeepFlowDocumentPanel:
			case CloseFlowDocumentPanel:
				flowDocumentPickMode = NoFlowDocumentPanel;
			case SelectFlowDocumentReference(zone, ruleId, card, referenceIndex, role, revision, value):
				final current = session;
				final rule = current != null && current.revision() == revision ? currentFlowRule(ruleId) : null;
				if (rule == null)
					staleFlowPick();
				else
					switch applyFlowDocumentPick(rule, card, referenceIndex, value, role, current.draftSnapshot()) {
						case FlowRuleAuthored(next):
							if (commitFlowRule(zone, next)) flowDocumentPickMode = NoFlowDocumentPanel;
						case FlowRuleUnchanged:
							flowDocumentPickMode = NoFlowDocumentPanel;
						case FlowRuleAuthoringRejected(_): notice = Invalid;
					}
		}
	}

	/** True while the extracted card panel owns input and drawing. */
	function flowCardLibraryOpen():Bool
		return flowCardPanelOpen(flowCardLibraryTarget);

	/** True while the extracted document panel owns input and drawing. */
	function flowDocumentPickerOpen():Bool
		return flowDocumentPanelOpen(flowDocumentPickMode);

	/** Read one complete copy-owned draft for typed choice projection. */
	function currentDraftScenario():Null<Scenario> {
		final current = session;
		if (current == null)
			return null;
		return switch current.query(InspectDraft) {
			case DraftObserved(_, draft): draft;
			case _: null;
		};
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
		if (textWorkspaceOpen) {
			final lineEditor = textLineEditor;
			if (lineEditor != null && lineEditor.isEditing()) {
				if (Raylib.IsKeyPressed(KeyboardKey.Enter)
					|| Raylib.IsKeyPressed(KeyboardKey.Escape)
					|| Raylib.IsKeyPressed(KeyboardKey.Tab)) {
					lineEditor.setEditing(false);
					commitTextLine();
				}
				return NavigationCommand.None;
			}
			if (Raylib.IsKeyPressed(KeyboardKey.Up))
				return NavigationCommand.Up;
			if (Raylib.IsKeyPressed(KeyboardKey.Down))
				return NavigationCommand.Down;
			if (Raylib.IsKeyPressed(KeyboardKey.Escape))
				return NavigationCommand.Cancel;
			if (Raylib.IsKeyPressed(KeyboardKey.Enter) || Raylib.IsKeyPressed(KeyboardKey.Space))
				return NavigationCommand.Confirm;
			return NavigationCommand.None;
		}
		if (assetBrowserOpen) {
			final search = assetSearch;
			if (search != null && search.isEditing()) {
				if (Raylib.IsKeyPressed(KeyboardKey.Enter)
					|| Raylib.IsKeyPressed(KeyboardKey.Escape)
					|| Raylib.IsKeyPressed(KeyboardKey.Tab))
					search.setEditing(false);
				return NavigationCommand.None;
			}
			if (Raylib.IsKeyPressed(KeyboardKey.Tab)) {
				if (search != null)
					search.setEditing(true);
				return NavigationCommand.None;
			}
			if (Raylib.IsKeyPressed(KeyboardKey.Up))
				return NavigationCommand.Up;
			if (Raylib.IsKeyPressed(KeyboardKey.Down))
				return NavigationCommand.Down;
			if (Raylib.IsKeyPressed(KeyboardKey.Left))
				return NavigationCommand.Left;
			if (Raylib.IsKeyPressed(KeyboardKey.Right))
				return NavigationCommand.Right;
			if (Raylib.IsKeyPressed(KeyboardKey.Escape))
				return NavigationCommand.Cancel;
			if (Raylib.IsKeyPressed(KeyboardKey.Enter) || Raylib.IsKeyPressed(KeyboardKey.Space))
				return NavigationCommand.Confirm;
			return NavigationCommand.None;
		}
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
		return shortcutModifierDown();
	}

	/** True when the platform copy/save modifier is held. */
	function shortcutModifierDown():Bool
		return Raylib.IsKeyDown(KeyboardKey.LeftControl)
			|| Raylib.IsKeyDown(KeyboardKey.RightControl)
			|| Raylib.IsKeyDown(KeyboardKey.LeftSuper)
			|| Raylib.IsKeyDown(KeyboardKey.RightSuper);

	/** True for one duplicate chord without consuming an ordinary D key edge. */
	function duplicateShortcutPressed():Bool
		return shortcutModifierDown() && Raylib.IsKeyPressed(KeyboardKey.D);

	/**
	 * Apply one navigation command to the editor's existing focus and actions.
	 *
	 * Up/left and down/right traverse the same cyclic order used by Tab.
	 * Confirm invokes the focused action, and Cancel returns to the title
	 * through the same typed screen transition as the keyboard Escape key.
	 * Keyboard, controller, and pilot commands all enter this one handler.
	 */
	public function applyNavigation(command:NavigationCommand):EditorScreenAction {
		if (textWorkspaceOpen) {
			applyTextNavigation(command);
			return StayInEditor;
		}
		if (assetBrowserOpen) {
			applyAssetBrowserNavigation(command);
			return StayInEditor;
		}
		if (flowCardLibraryOpen() || flowDocumentPickerOpen()) {
			if (command == NavigationCommand.Cancel) {
				flowCardLibraryTarget = NoFlowCardPanel;
				flowDocumentPickMode = NoFlowDocumentPanel;
			}
			return StayInEditor;
		}
		if (command == NavigationCommand.Cancel && objectGrabActive(objectGrab)) {
			objectGrab = NoObjectGrab;
			return StayInEditor;
		}
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

	/** Route keyboard, controller, and pilot movement inside the Text workspace. */
	function applyTextNavigation(command:NavigationCommand):Void {
		final document = textDocument;
		if (document == null)
			return;
		switch command {
			case Up | Left:
				selectTextLine(textSelectedLine > 0 ? textSelectedLine - 1 : document.lineCount() - 1);
			case Down | Right:
				selectTextLine(textSelectedLine + 1 < document.lineCount() ? textSelectedLine + 1 : 0);
			case Confirm:
				final editor = textLineEditor;
				if (editor != null)
					editor.setEditing(true);
			case Cancel:
				closeTextWorkspace();
			case None:
		}
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
			case Text:
				openTextWorkspace();
			case CameraMode:
				cycleEditorCamera();
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
				openAssetBrowser();
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
	 * A save first commits the temporary title and object-name buffers. Package validation and
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
		final selectedName = objectName;
		if (selectedName != null && selectedName.isEditing()) {
			selectedName.setEditing(false);
			if (!commitObjectName(selectedName.text())) {
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
		if (textWorkspaceOpen) {
			closeTextWorkspace();
			return StayInEditor;
		}
		if (assetBrowserOpen) {
			closeAssetBrowser();
			return StayInEditor;
		}
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

	/** Cycle the visible mode and frame the selected object or complete world. */
	function cycleEditorCamera():Void {
		final world = projection;
		final current = camera;
		if (world == null || current == null)
			return;
		final focus = cameraFocus(world);
		camera = focusCamera(world, cycleCameraMode(cameraMode(current)), focus.target, focus.orbitDistance);
		if (workspaceView != BuildView)
			setWorkspaceView(BuildView);
		invalidatePreview();
	}

	/** Focus the active mode on the selected object or the complete world. */
	function focusActiveCamera(world:EditorWorldProjection, current:EditorCameraState):EditorCameraState {
		final focus = cameraFocus(world);
		return focusCamera(world, cameraMode(current), focus.target, focus.orbitDistance);
	}

	/** Resolve one copy-owned Orbit target without campaign-specific identities. */
	function cameraFocus(world:EditorWorldProjection):EditorCameraFocus {
		final selected = selectedObjectIndex();
		if (selected >= 0) {
			final gizmo = objectGizmos[selected];
			var extent = gizmo.width;
			if (gizmo.height > extent)
				extent = gizmo.height;
			if (gizmo.depth > extent)
				extent = gizmo.depth;
			return {
				target: {x: gizmo.x, y: gizmo.y, z: gizmo.z},
				orbitDistance: extent * 2.5 + 1.0
			};
		}
		return {
			target: {x: world.width * 0.5, y: world.height * 0.5, z: world.depth * 0.5},
			orbitDistance: 0.0
		};
	}

	/** Build one short localized label for the camera selector. */
	function cameraControlText(locale:LocaleCursor, mode:EditorCameraMode):String {
		final value = switch mode {
			case WalkCamera: uiCatalog.text(locale, UiMessage.EditorCameraWalk);
			case FlyCamera: uiCatalog.text(locale, UiMessage.EditorCameraFly);
			case OrbitCamera: uiCatalog.text(locale, UiMessage.EditorCameraOrbit);
		};
		return '${uiCatalog.text(locale, UiMessage.EditorCamera)}: $value (C)';
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

	/** Open asset discovery and release the mouse from direct world control. */
	function openAssetBrowser():Void {
		if (textWorkspaceOpen)
			closeTextWorkspace();
		setBuildPointerState(EditorBuildPointerState.Released);
		assetBrowserOpen = true;
		focusedControl = EditorFocusTarget.CatalogObjectTool;
		final search = assetSearch;
		if (search != null)
			search.setEditing(false);
		normalizeAssetSelection();
	}

	/** Close asset discovery while preserving its category and query for return. */
	function closeAssetBrowser():Void {
		assetBrowserOpen = false;
		final search = assetSearch;
		if (search != null)
			search.setEditing(false);
		focusedControl = EditorFocusTarget.CatalogObjectTool;
	}

	/** Route keyboard, controller, and pilot commands through one modal policy. */
	function applyAssetBrowserNavigation(command:NavigationCommand):Void {
		switch command {
			case Up:
				assetSelection = moveEditorAssetSelection(assetSelection, visibleAssetEntries().length, -1);
			case Down:
				assetSelection = moveEditorAssetSelection(assetSelection, visibleAssetEntries().length, 1);
			case Left:
				moveAssetCategory(-1);
			case Right:
				moveAssetCategory(1);
			case Confirm:
				final visible = visibleAssetEntries();
				if (assetSelection >= 0 && assetSelection < visible.length)
					chooseAsset(visible[assetSelection]);
			case Cancel:
				closeAssetBrowser();
			case None:
		}
	}

	/** Change category once, wrapping through the visible deterministic order. */
	function moveAssetCategory(direction:Int):Void {
		final categories = assetCategories;
		if (categories.length == 0)
			return;
		var index = categoryIndex(assetCategory);
		index += direction < 0 ? -1 : 1;
		if (index < 0)
			index = categories.length - 1;
		else if (index >= categories.length)
			index = 0;
		setAssetCategory(categories[index]);
	}

	/** Select one category and place its first filtered row under focus. */
	function setAssetCategory(category:EditorAssetCategory):Void {
		assetCategory = category;
		refreshVisibleAssets(true);
	}

	/** Rebuild the small filtered row cache only after its inputs change. */
	function refreshVisibleAssets(resetSelection:Bool):Void {
		visibleAssets = filterEditorAssets(assetEntries, assetCategory, assetQuery);
		if (resetSelection)
			assetSelection = visibleAssets.length == 0 ? -1 : 0;
		else
			normalizeAssetSelection();
	}

	/** Keep modal selection valid after reopening or changing its query. */
	function normalizeAssetSelection():Void {
		final count = visibleAssetEntries().length;
		if (count == 0)
			assetSelection = -1;
		else if (assetSelection < 0 || assetSelection >= count)
			assetSelection = 0;
	}

	/** Return the cached read-only row list used by this private screen. */
	function visibleAssetEntries():Array<EditorAssetEntry>
		return visibleAssets;

	/** Select one browser row through the existing terrain or object tool. */
	function chooseAsset(entry:EditorAssetEntry):Void {
		switch entry.use {
			case PaintTerrainAsset(blockType):
				if (selectTerrainAsset(blockType)) {
					setActiveTool(EditorTool.PaintTool);
					closeAssetBrowser();
				}
			case PlaceObjectAsset(_):
				selectedObjectAsset = entry;
				setActiveTool(EditorTool.CatalogObjectTool);
				closeAssetBrowser();
		}
	}

	/**
	 * Select a terrain material, adding one draft-local palette code when needed.
	 *
	 * The new palette row is a normal revision-checked command. Undo, redo,
	 * validation, Save, and reopen therefore observe the same canonical edit as
	 * a manually authored palette change. No global content code is assumed.
	 */
	function selectTerrainAsset(blockType:ContentId):Bool {
		var draft = presentationDraft;
		final current = session;
		if (draft == null || current == null)
			return false;
		var code = paletteCodeForBlock(draft.world.palette, blockType);
		if (code < 0) {
			code = firstFreePaletteCode(draft);
			if (code < 0) {
				notice = Invalid;
				return false;
			}
			switch current.mutate({baseRevision: current.revision(), mutation: Apply(SetPaletteEntry(code, blockType))}) {
				case MutationApplied(_, _, _, _, _, _):
					refreshProjection(false, RefreshAllTerrain);
				case MutationUnchanged(_, _):
				case MutationRejected(_, _):
					notice = Invalid;
					return false;
			}
			draft = presentationDraft;
			if (draft == null)
				return false;
			code = paletteCodeForBlock(draft.world.palette, blockType);
		}
		if (code < 0)
			return false;
		selectGroundPalette(code);
		notice = Ready;
		return true;
	}

	/** Find the smallest unused positive palette code in the CAXEMAP domain. */
	static function firstFreePaletteCode(draft:EditorPresentationSnapshot):Int {
		for (code in 1...256) {
			var used = false;
			for (entry in draft.world.palette)
				if (entry.code == code)
					used = true;
			if (!used)
				return code;
		}
		return -1;
	}

	/** Select the first mechanism recipe, or another placeable row as fallback. */
	static function firstObjectAsset(entries:Array<EditorAssetEntry>):Null<EditorAssetEntry> {
		for (entry in entries)
			if (entry.category == EditorAssetCategory.MechanismAssets)
				switch entry.use {
					case PlaceObjectAsset(_):
						return entry;
					case PaintTerrainAsset(_):
				}
		for (entry in entries)
			switch entry.use {
				case PlaceObjectAsset(_):
					return entry;
				case PaintTerrainAsset(_):
			}
		return null;
	}

	/** Name the selected object recipe while the browser remains closed. */
	function selectedObjectAssetLabel(locale:LocaleCursor):String {
		final selected = selectedObjectAsset;
		return selected == null ? uiCatalog.text(locale, UiMessage.EditorAssetBrowser) : editorAssetLabel(selected, locale);
	}

	/** Map a stable category to its data-owned localized heading. */
	static function assetCategoryMessage(category:EditorAssetCategory):UiMessage
		return switch category {
			case TerrainAssets: UiMessage.EditorAssetCategoryTerrain;
			case ItemAssets: UiMessage.EditorAssetCategoryItem;
			case NpcAssets: UiMessage.EditorAssetCategoryNpc;
			case EnemyAssets: UiMessage.EditorAssetCategoryEnemy;
			case MechanismAssets: UiMessage.EditorAssetCategoryMechanism;
		};

	/** Return the fixed category order as an integer for navigation and marks. */
	static function categoryIndex(category:EditorAssetCategory):Int
		return switch category {
			case TerrainAssets: 0;
			case ItemAssets: 1;
			case NpcAssets: 2;
			case EnemyAssets: 3;
			case MechanismAssets: 4;
		};

	/** Open the environment modal at its explicit enabled control. */
	function openEnvironmentPanel():Void {
		if (textWorkspaceOpen)
			closeTextWorkspace();
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
			case MutationApplied(_, _, _, _, _, _):
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
		if (tool != SelectTool)
			objectGrab = NoObjectGrab;
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
		else {
			objectGrab = NoObjectGrab;
			Raylib.EnableCursor();
		}
	}

	/** Select a visible Build card from a number key or one mouse-wheel step. */
	function selectBuildHotbarTool(wheelDirection:Int):Void {
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
		else if (wheelDirection != 0)
			setActiveTool(cycleBuildHotbarTool(activeTool, wheelDirection));
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
		objectGrab = NoObjectGrab;
		switch current.select({baseRevision: current.revision(), selection: NodeSelection(ObjectNode(id))}) {
			case SelectionApplied(_, _) | SelectionUnchanged(_, _):
				selection = current.selectedBounds();
				objectList = new GuiListViewState(index);
				final world = projection;
				final currentCamera = camera;
				if (world != null && currentCamera != null && cameraMode(currentCamera) == EditorCameraMode.OrbitCamera)
					camera = retargetOrbitCamera(currentCamera, cameraFocus(world).target);
				notice = Ready;
			case SelectionRejected(_, _):
				notice = Invalid;
		}
	}

	/**
	 * Commit one direct Build object action through the inspector command path.
	 *
	 * The action has no separate drag or preview state. A successful key edge
	 * therefore creates the same single revision and history entry as one exact
	 * inspector button. The return value tells the viewport to read the refreshed
	 * projection and Orbit target before it draws the current frame.
	 */
	function applyBuildObjectAction(action:EditorBuildObjectAction):Bool {
		final current = session;
		if (current == null)
			return false;
		final beforeRevision = current.revision();
		switch action {
			case NoObjectAction:
				return false;
			case NudgeSelectedObject(delta):
				moveSelectedObject(delta);
			case TurnSelectedObject(degrees):
				rotateSelectedObject(degrees);
		}
		return current.revision() == beforeRevision + 1;
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
		moveObject(id, delta);
	}

	/** Commit one stable object's whole-cell translation through shared history. */
	function moveObject(id:ScenarioId, delta:VoxelPoint):Bool {
		final current = session;
		if (current == null)
			return false;
		return switch current.mutate({baseRevision: current.revision(), mutation: Apply(MoveObjectBy(id, delta))}) {
			case MutationApplied(_, _, _, _, _, _):
				notice = Ready;
				refreshProjection(false, KeepTerrain);
				true;
			case MutationUnchanged(_, _):
				notice = Ready;
				false;
			case MutationRejected(_, _):
				notice = Invalid;
				false;
		};
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
			case MutationApplied(_, _, _, _, _, _):
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
			case MutationApplied(_, _, _, _, _, _):
				notice = Ready;
				refreshProjection(false, KeepTerrain);
			case MutationUnchanged(_, _):
				notice = Ready;
			case MutationRejected(_, _):
				notice = Invalid;
		}
	}

	/** Start a two-step connection: keep the trigger selected, then pick its target. */
	function beginZoneConnection(zone:ScenarioId):Void {
		final current = session;
		if (current == null)
			return;
		flowWorldPickMode = ConnectZoneWorldPick(zone, current.revision());
		setWorkspaceView(BuildView);
		setActiveTool(EditorTool.SelectTool);
		setBuildPointerState(EditorBuildPointerState.Released);
		detailsOpen = true;
		notice = Ready;
	}

	/** Start replacing one typed card reference from a visible world object. */
	function beginFlowReferencePick(zone:ScenarioId, rule:ScenarioId, card:EditorFlowCardAddress, referenceIndex:Int):Void {
		final current = session;
		if (current == null)
			return;
		flowWorldPickMode = ReplaceFlowReferenceWorldPick(zone, rule, card, referenceIndex, current.revision());
		setWorkspaceView(BuildView);
		setActiveTool(EditorTool.SelectTool);
		setBuildPointerState(EditorBuildPointerState.Released);
		detailsOpen = true;
		notice = Ready;
	}

	/** Open a revision-bound picker containing only IDs valid for this document role. */
	function beginFlowDocumentPick(zone:ScenarioId, rule:ScenarioId, card:EditorFlowCardAddress, referenceIndex:Int, role:EditorFlowReferenceRole):Void {
		final current = session;
		if (current == null)
			return;
		flowDocumentPickMode = FlowDocumentPanel(zone, rule, card, referenceIndex, role, current.revision());
		flowCardLibraryTarget = NoFlowCardPanel;
		flowWorldPickMode = NoFlowWorldPick;
		notice = Ready;
	}

	/** Apply the pending connection or card-field replacement through `PutRule`. */
	function applyFlowObjectPick(object:ScenarioObject):Bool {
		final current = session;
		if (current == null)
			return false;
		return switch flowWorldPickMode {
			case NoFlowWorldPick:
				false;
			case ConnectZoneWorldPick(zone, revision):
				if (current.revision() != revision) {
					flowWorldPickMode = NoFlowWorldPick;
					notice = Invalid;
					false;
				} else if (zone.text() == object.id.text()) {
					notice = Invalid;
					false;
				} else {
					final draft = presentationDraft;
					if (draft == null) {
						notice = Invalid;
						false;
					} else
						commitFlowRule(zone, connectZone({
							ruleId: nextZoneConnectionRuleId(zone, draft.ruleIds),
							priority: 0,
							repeat: Once,
							zone: zone,
							predicate: Always,
							actions: [Spawn(object.id)]
						}));
				}
			case ReplaceFlowReferenceWorldPick(zone, ruleId, card, referenceIndex, revision):
				final rule = current.revision() == revision ? currentFlowRule(ruleId) : null;
				if (rule == null) {
					flowWorldPickMode = NoFlowWorldPick;
					notice = Invalid;
					false;
				} else switch applyFlowWorldPick(rule, card, referenceIndex, worldPickFor(object)) {
					case FlowRuleAuthored(next): commitFlowRule(zone, next);
					case FlowRuleUnchanged:
						flowWorldPickMode = NoFlowWorldPick;
						notice = Ready;
						true;
					case FlowRuleAuthoringRejected(_):
						notice = Invalid;
						false;
				}
		};
	}

	/** Reject a picker whose target changed while the creator was in the world. */
	function staleFlowPick():Bool {
		flowWorldPickMode = NoFlowWorldPick;
		notice = Invalid;
		return false;
	}

	/** Commit one authored rule, retain trigger selection, and expose its cards. */
	function commitFlowRule(zone:ScenarioId, rule:FlowRule):Bool {
		final current = session;
		if (current == null)
			return false;
		return switch current.mutate({baseRevision: current.revision(), mutation: Apply(PutRule(rule))}) {
			case MutationApplied(_, _, _, _, _, _):
				flowWorldPickMode = NoFlowWorldPick;
				notice = Ready;
				refreshProjection(false, KeepTerrain);
				selectObject(zone);
				detailsOpen = true;
				true;
			case MutationUnchanged(_, _):
				flowWorldPickMode = NoFlowWorldPick;
				notice = Ready;
				true;
			case MutationRejected(_, _):
				notice = Invalid;
				false;
		};
	}

	/** Apply one narrow card edit through the canonical whole-rule command. */
	function commitFlowEdit(zone:ScenarioId, ruleId:ScenarioId, edit:EditorFlowCardEdit):Bool {
		final rule = currentFlowRule(ruleId);
		if (rule == null)
			return staleFlowPick();
		return switch editFlowCard(rule, edit) {
			case FlowRuleAuthored(next): commitFlowRule(zone, next);
			case FlowRuleUnchanged:
				notice = Ready;
				false;
			case FlowRuleAuthoringRejected(_):
				notice = Invalid;
				false;
		};
	}

	/** Read one copy-owned rule only when a card gesture needs to mutate it. */
	function currentFlowRule(expected:ScenarioId):Null<FlowRule> {
		final current = session;
		if (current == null)
			return null;
		return switch current.query(InspectDraft) {
			case DraftObserved(_, draft):
				var found:Null<FlowRule> = null;
				for (rule in draft.flow.rules)
					if (rule.id.text() == expected.text())
						found = rule;
				found;
			case _:
				null;
		};
	}

	/** True while the next visible object click belongs to a card gesture. */
	function flowWorldPickActive():Bool
		return switch flowWorldPickMode {
			case NoFlowWorldPick: false;
			case ConnectZoneWorldPick(_, _) | ReplaceFlowReferenceWorldPick(_, _, _, _, _): true;
		};

	/** Delete the shared object target through canonical history and clear stale UI state. */
	function deleteSelectedObject():Void {
		final current = session;
		if (current == null)
			return;
		final id = switch current.selectionSnapshot() {
			case NodeSelection(ObjectNode(value)): value;
			case NoEditorSelection | VoxelSelection(_) | NodeSelection(_): return;
		};
		final plan = deleteObjectWithConnectedRules(id, current.draftSnapshot().flow.rules);
		switch current.mutate({baseRevision: current.revision(), mutation: ApplyBatch(plan.commands)}) {
			case MutationApplied(_, _, _, _, _, _):
				flowWorldPickMode = NoFlowWorldPick;
				objectNameTarget = null;
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
		final canonicalDraft = current.draftSnapshot();
		final duplicate = duplicateObjectWithConnectedRules(sourceId, draft.objects, canonicalDraft.flow.rules);
		if (duplicate == null) {
			notice = Invalid;
			return;
		}
		switch current.mutate({baseRevision: current.revision(), mutation: ApplyBatch(duplicate.commands)}) {
			case MutationApplied(_, _, _, _, _, _):
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

	/** True when the projected object owns a transform-facing quarter turn. */
	function objectCanTurn(index:Int):Bool {
		if (index < 0 || index >= objectGizmos.length)
			return false;
		return switch objectGizmos[index].facing {
			case ObjectYaw(_): true;
			case NoObjectFacing: false;
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

	/** Resolve a picker target from the copy-owned presentation, never a widget index. */
	function presentationObject(id:ScenarioId):Null<ScenarioObject> {
		final draft = presentationDraft;
		if (draft == null)
			return null;
		for (object in draft.objects)
			if (object.id.text() == id.text())
				return object;
		return null;
	}

	/** Select the crosshair object, then apply the shared presentation-only grab transition. */
	function updateObjectGrab(selected:Null<ScenarioId>, hovered:Null<ScenarioId>, grabPressed:Bool):Void {
		final candidate = objectGrabCandidate(selected, hovered, grabPressed);
		if (grabPressed && hovered != null && (selected == null || selected.text() != hovered.text()))
			selectObject(hovered);
		objectGrab = nextObjectGrab(objectGrab, candidate, grabPressed, false);
	}

	function undo():Void {
		final current = session;
		if (current == null)
			return;
		switch current.mutate({baseRevision: current.revision(), mutation: Undo}) {
			case MutationApplied(_, _, terrain, _, _, _):
				notice = Ready;
				refreshProjection(false, terrainRefreshForTerrainChange(terrain));
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
			case MutationApplied(_, _, terrain, _, _, _):
				notice = Ready;
				refreshProjection(false, terrainRefreshForTerrainChange(terrain));
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
		if (started)
			flowTrace = {rows: [], truncated: false};
		notice = started ? Testing : Invalid;
		return started;
	}

	/** Retain the latest non-empty bounded trace from ordinary external Test Play. */
	public function observeFlowTrace(trace:Array<FlowTraceEntry>):Void {
		flowTrace = retainLatestFlowTrace(flowTrace, trace, 12);
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
			case MutationApplied(_, _, _, _, _, _) | MutationUnchanged(_, _):
				notice = Ready;
				true;
			case MutationRejected(_, _):
				notice = Invalid;
				false;
		};
		refreshProjection(false, KeepTerrain);
		return accepted;
	}

	/** Commit one selected object name and every typed reference in one history step. */
	function commitObjectName(value:String):Bool {
		final current = session;
		final before = objectNameTarget;
		if (current == null || before == null) {
			notice = Invalid;
			return false;
		}
		final after = new ScenarioId(value);
		return switch current.mutate({baseRevision: current.revision(), mutation: Apply(RenameObject(before, after))}) {
			case MutationApplied(_, _, _, _, _, _):
				flowWorldPickMode = NoFlowWorldPick;
				objectNameTarget = after;
				notice = Ready;
				refreshProjection(false, KeepTerrain);
				selectObject(after);
				true;
			case MutationUnchanged(_, _):
				objectNameTarget = before;
				syncObjectName(before);
				notice = Ready;
				true;
			case MutationRejected(_, _):
				objectNameTarget = null;
				syncObjectName(before);
				notice = Invalid;
				false;
		};
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
		final paletteCode = tool == PaintTool || tool == FillTool ? groundPaletteCode : 0;
		if (previewPoint != null
			&& previewPoint.x == point.x
			&& previewPoint.y == point.y
			&& previewPoint.z == point.z
			&& previewRevision == current.revision()
			&& previewTool == tool
			&& previewPaletteCode == paletteCode)
			return;
		previewPoint = {x: point.x, y: point.y, z: point.z};
		previewRevision = current.revision();
		previewTool = tool;
		previewPaletteCode = paletteCode;
		if (paletteCode < 0) {
			previewAllowed = false;
			return;
		}
		previewAllowed = switch commandForTool(tool, point, toolContextFor(tool, draft, paletteCode)) {
			case ToolCommandRejected(_): false;
			case ToolSelectionReady(_): true;
			case ToolCommandReady(_) | ToolBatchReady(_, _): true;
		};
	}

	/** Return the browser-selected recipe only for its matching creation tool. */
	function activeRecipeFor(tool:EditorTool):Null<EditorObjectRecipe> {
		if (tool != CatalogObjectTool)
			return null;
		final selected = selectedObjectAsset;
		if (selected == null)
			return null;
		return switch selected.use {
			case PlaceObjectAsset(recipe): recipe;
			case PaintTerrainAsset(_): null;
		};
	}

	/** Gather one named tool snapshot so new templates do not grow positional calls. */
	function toolContextFor(tool:EditorTool, draft:EditorPresentationSnapshot, paletteCode:Int):EditorToolContext {
		final current = session;
		if (current == null)
			throw "editor tool context requested without an active session";
		return {
			scenarioId: draft.id,
			worldSize: draft.world.size,
			paletteCode: paletteCode,
			selection: current.selectedBounds(),
			objects: draft.objects,
			ruleIds: draft.ruleIds,
			dialogueIds: draft.dialogueIds,
			recipe: activeRecipeFor(tool)
		};
	}

	/** Drop the last ghost when a view, tool, or draft transition changes meaning. */
	function invalidatePreview():Void {
		previewPoint = null;
		previewRevision = -1;
		previewAllowed = false;
		previewPaletteCode = -1;
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
	 * Bracket keys cycle map materials. The middle button picks aimed terrain.
	 * WASD moves the active camera, and Fly also uses Q/E. The mouse wheel moves
	 * through the visible hotbar. C changes camera mode, and F refocuses it.
	 * With Select and one object active, G holds it for one crosshair placement.
	 * Arrow keys still move it by one camera-relative cell, and R turns an
	 * eligible object clockwise by one quarter turn.
	 * Escape cancels a held object first. Another press releases the pointer.
	 */
	function drawWorldViewport(locale:LocaleCursor, left:Int, top:Int, width:Int, height:Int, resources:EditorRenderResources):Void {
		if (buildConfirmationSeconds > 0.0) {
			final elapsed = Raylib.GetFrameTime().toFloat();
			buildConfirmationSeconds = buildConfirmationSeconds > elapsed ? buildConfirmationSeconds - elapsed : 0.0;
		}
		var current = projection;
		var currentCamera = camera;
		final draft = presentationDraft;
		if (current == null || currentCamera == null || draft == null || width <= 0 || height <= 0)
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
		final wheel = cameraInputEnabled ? Raylib.GetMouseWheelMove().toFloat() : 0.0;
		final wheelDirection = wheel > 0.0 ? -1 : wheel < 0.0 ? 1 : 0;
		if (cameraInputEnabled)
			selectBuildHotbarTool(wheelDirection);
		if (cameraInputEnabled && activeTool == PaintTool) {
			final materialDirection = Raylib.IsKeyPressed(KeyboardKey.LeftBracket) ? -1 : Raylib.IsKeyPressed(KeyboardKey.RightBracket) ? 1 : 0;
			if (materialDirection != 0)
				selectGroundPalette(cycleBuildPaletteCode(draft.world.palette, groundPaletteCode, defaultGroundPaletteCode(), materialDirection));
		}
		if (cameraInputEnabled && Raylib.IsKeyPressed(KeyboardKey.C)) {
			cycleEditorCamera();
			final cycled = camera;
			if (cycled != null)
				currentCamera = cycled;
		}
		if (cameraInputEnabled && Raylib.IsKeyPressed(KeyboardKey.F))
			currentCamera = focusActiveCamera(current, currentCamera);
		else if (cameraInputEnabled) {
			final delta = Raylib.GetMouseDelta();
			final shortcutModified = shortcutModifierDown();
			currentCamera = stepCamera(current, currentCamera, {
				forward: axis(!shortcutModified && Raylib.IsKeyDown(KeyboardKey.W), !shortcutModified && Raylib.IsKeyDown(KeyboardKey.S)),
				right: axis(!shortcutModified && Raylib.IsKeyDown(KeyboardKey.D), !shortcutModified && Raylib.IsKeyDown(KeyboardKey.A)),
				vertical: axis(Raylib.IsKeyDown(KeyboardKey.E), Raylib.IsKeyDown(KeyboardKey.Q)),
				yaw: capturePressed ? 0.0 : -delta.x.toFloat() * 0.004,
				pitch: capturePressed ? 0.0 : -delta.y.toFloat() * 0.004,
				wheel: 0.0
			}, Raylib.GetFrameTime().toFloat());
		}
		camera = currentCamera;
		var currentPose = cameraPose(currentCamera);
		final directObjectIndex = selectedObjectIndex();
		final grabPressed = cameraInputEnabled && Raylib.IsKeyPressed(KeyboardKey.G);
		var holdingObject = objectGrabActive(objectGrab);
		final directObjectEdited = applyBuildObjectAction(objectAction({
			pointerCaptured: cameraInputEnabled,
			selectToolActive: activeTool == SelectTool,
			objectSelected: directObjectIndex >= 0 && !holdingObject && !grabPressed,
			objectCanTurn: objectCanTurn(directObjectIndex),
			upPressed: Raylib.IsKeyPressed(KeyboardKey.Up),
			rightPressed: Raylib.IsKeyPressed(KeyboardKey.Right),
			downPressed: Raylib.IsKeyPressed(KeyboardKey.Down),
			leftPressed: Raylib.IsKeyPressed(KeyboardKey.Left),
			turnPressed: Raylib.IsKeyPressed(KeyboardKey.R),
			lookX: currentPose.lookX,
			lookZ: currentPose.lookZ
		}));
		if (directObjectEdited) {
			showBuildConfirmation();
			final refreshedProjection = projection;
			final refreshedCamera = camera;
			if (refreshedProjection == null || refreshedCamera == null)
				return;
			current = refreshedProjection;
			currentCamera = refreshedCamera;
			currentPose = cameraPose(refreshedCamera);
		}
		final target = cameraTarget(currentCamera);
		final nativeCamera = Camera3D.make(Vector3.fromFloat(currentPose.x, currentPose.y, currentPose.z), Vector3.fromFloat(target.x, target.y, target.z),
			Vector3.fromFloat(0.0, 1.0, 0.0), c.Float32.fromFloat(52.0), CameraProjection.Perspective);
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
			if ((activeTool == SelectTool || flowWorldPickActive()) && !holdingObject) {
				final objectHit = pickObject(objectGizmos, {x: origin.x.toFloat(), y: origin.y.toFloat(), z: origin.z.toFloat()}, {
					x: direction.x.toFloat(),
					y: direction.y.toFloat(),
					z: direction.z.toFloat()
				}, 512.0);
				if (objectHit != null && (hover == null || objectHit.distance <= hover.distance))
					hoveredObject = objectIndex(objectHit.id);
			}
		}
		if (cameraInputEnabled && activeTool == PaintTool && Raylib.IsMouseButtonPressed(MouseButton.Middle) && hover != null)
			selectGroundPalette(pickBuildPaletteCode(draft.world.palette, groundPaletteCode, defaultGroundPaletteCode(), hover.solid,
				paletteCodeAtWorld(current, hover.point.x, hover.point.y, hover.point.z)));
		final selectedGrabIndex = selectedObjectIndex();
		final selectedGrabId:Null<ScenarioId> = if (cameraInputEnabled && activeTool == SelectTool && !flowWorldPickActive() && selectedGrabIndex >= 0)
			objectGizmos[selectedGrabIndex].id else null;
		final hoveredGrabId:Null<ScenarioId> = if (!flowWorldPickActive() && hoveredObject >= 0) objectGizmos[hoveredObject].id else null;
		updateObjectGrab(selectedGrabId, hoveredGrabId, grabPressed);
		holdingObject = objectGrabActive(objectGrab);
		final grabbedIndex = switch objectGrab {
			case NoObjectGrab: -1;
			case HoldingObject(id): objectIndex(id);
		};
		if (grabbedIndex < 0 && holdingObject)
			objectGrab = NoObjectGrab;
		final objectTarget:Null<VoxelPoint> = if (grabbedIndex < 0 || hover == null) null else if (hover.placement != null) hover.placement else
			if (!hover.solid) hover.point else null;
		var objectPreview:Null<EditorObjectGizmo> = null;
		if (objectTarget != null) {
			final source = objectGizmos[grabbedIndex];
			final delta = objectPlacementDelta(source, objectTarget);
			objectPreview = {
				id: source.id,
				kind: source.kind,
				origin: objectTarget,
				x: source.x + delta.x,
				y: source.y + delta.y,
				z: source.z + delta.z,
				width: source.width,
				height: source.height,
				depth: source.depth,
				facing: source.facing
			};
		}
		final terrainMode = usesDirectTerrainControls(activeTool);
		final previewTool = activeTool == PaintTool ? PaintTool : (terrainMode ? EraseTool : activeTool);
		final previewPoint:Null<VoxelPoint> = if (hover == null) null else if (activeTool == PaintTool) hover.placement else hover.point;
		if (holdingObject || previewPoint == null || hoveredObject >= 0)
			invalidatePreview();
		else
			updatePreview(previewPoint, previewTool);
		if (!capturePressed && !holdingObject && flowWorldPickActive() && hoveredObject >= 0 && leftPressed) {
			final picked = presentationObject(objectGizmos[hoveredObject].id);
			if (picked != null && applyFlowObjectPick(picked))
				showBuildConfirmation();
			current = projection;
			if (current == null)
				return;
		} else if (!capturePressed && aiming && objectPreview != null && leftPressed) {
			final preview = objectPreview;
			final delta = objectPlacementDelta(objectGizmos[grabbedIndex], preview.origin);
			final id = objectGizmos[grabbedIndex].id;
			if (moveObject(id, delta)) {
				showBuildConfirmation();
				objectGrab = NoObjectGrab;
				current = projection;
				if (current == null)
					return;
			}
		} else if (!capturePressed && aiming && !holdingObject && hoveredObject >= 0 && leftPressed) {
			selectObject(objectGizmos[hoveredObject].id);
		} else if (!capturePressed && aiming && !holdingObject && hoveredObject < 0 && terrainMode && hover != null && (leftPressed || rightPressed)) {
			final edited = switch terrainAction(leftPressed, rightPressed, hover) {
				case NoTerrainAction: false;
				case RemoveTerrain(point): applyToolAt(EditorTool.EraseTool, point);
				case PlaceTerrain(point): applyToolAt(EditorTool.PaintTool, point);
			};
			current = projection;
			if (current == null)
				return;
			if (edited) {
				showBuildConfirmation();
				invalidatePreview();
			}
		} else if (!capturePressed && aiming && !holdingObject && (hover != null || hoveredObject >= 0) && leftPressed) {
			final applied = hover != null && applyToolAt(activeTool, hover.point);
			if (applied && activeTool != SelectTool)
				showBuildConfirmation();
			current = projection;
			if (current == null)
				return;
			if (hoveredObject < 0 && hover != null)
				updatePreview(hover.point, activeTool);
		}

		final prompt = buildPromptFor({
			tool: activeTool,
			targetAvailable: hover != null,
			targetSolid: hover != null && hover.solid,
			placementAvailable: hover != null && hover.placement != null,
			objectTarget: hoveredObject >= 0,
			objectHeld: objectGrabActive(objectGrab),
			heldPlacementAvailable: objectPreview != null,
			previewAllowed: previewAllowed
		});
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
			resources.adventureTerrainTextureReady, currentPose.x, currentPose.z))
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
		if (objectPreview != null) {
			final preview = objectPreview;
			Raylib.DrawCubeWires(Vector3.fromFloat(preview.x, preview.y, preview.z), c.Float32.fromFloat(preview.width + 0.12),
				c.Float32.fromFloat(preview.height + 0.12), c.Float32.fromFloat(preview.depth + 0.12), Color.rgba(255, 214, 92));
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
		if (aiming && selectedObjectIndex() >= 0) {
			final shortcutText = '${uiCatalog.text(locale, UiMessage.EditorDuplicate)}: CTRL/CMD+D  ·  ${uiCatalog.text(locale, UiMessage.EditorDelete)}: BACKSPACE';
			Raylib.DrawRectangle(left + 8, top + height - 28, width - 16, 22, Color.rgba(8, 20, 24));
			Raylib.DrawTextString(shortcutText, left + 14, top + height - 24, 14, CaxecraftPalette.hudText());
		}
		if (flowWorldPickActive()) {
			final pickText = uiCatalog.format(locale, EditorFlowUiMessage.PickObjectMessage.messageId(), []);
			Raylib.DrawRectangle(left + 8, top + height - 54, width - 16, 22, Color.rgba(8, 20, 24));
			Raylib.DrawTextString(pickText, left + 14, top + height - 50, 14, CaxecraftPalette.selection());
		}
		if (aiming) {
			final centerX = left + Std.int(width / 2);
			final centerY = top + Std.int(height / 2);
			drawBuildCrosshair(centerX, centerY);
			drawBuildPrompt(locale, left, width, centerY, prompt);
		}
	}

	/** Keep one accepted in-world edit visible long enough to read and Undo. */
	function showBuildConfirmation():Void
		buildConfirmationSeconds = 1.2;

	/** Draw the exact typed action below the crosshair, or recent edit success. */
	function drawBuildPrompt(locale:LocaleCursor, viewportLeft:Int, viewportWidth:Int, centerY:Int, prompt:EditorBuildPrompt):Void {
		final label = if (buildConfirmationSeconds > 0.0)
			'✓ ${uiCatalog.text(locale, UiMessage.EditorReady)}  ·  CTRL/CMD+Z ${uiCatalog.text(locale, UiMessage.EditorUndo)}'; else switch prompt {
			case NoBuildTarget:
				uiCatalog.text(locale, UiMessage.NoBlockInReach);
			case TerrainBuildPrompt(canRemove, canPlace):
				final materialControls = '[MMB] PICK  ·  [ / ] MATERIAL';
				if (canRemove && canPlace)
					'[LMB] ${uiCatalog.text(locale, UiMessage.EditorErase)}  ·  [RMB] ${uiCatalog.text(locale, UiMessage.EditorGround)}  ·  $materialControls'; else
					if (canRemove) '[LMB] ${uiCatalog.text(locale, UiMessage.EditorErase)}  ·  $materialControls'; else
						'[RMB] ${uiCatalog.text(locale, UiMessage.EditorGround)}  ·  $materialControls';
			case SelectBuildPrompt(objectTarget):
				if (objectTarget) '[LMB] ${uiCatalog.text(locale, UiMessage.EditorSelect)}  ·  [G] <->'; else
					'[LMB] ${uiCatalog.text(locale, UiMessage.EditorSelect)}';
			case CreateBuildPrompt(allowed):
				if (allowed) '[LMB] ${immersiveToolLabel(locale, activeTool)}'; else uiCatalog.text(locale, UiMessage.PlaceBlocked);
			case MoveBuildPrompt(allowed):
				if (allowed) '[LMB] ✓  ·  [G / ESC] ×'; else '${uiCatalog.text(locale, UiMessage.PlaceBlocked)}  ·  [G / ESC] ×';
		};
		final availableWidth = viewportWidth - 32;
		final width = availableWidth < 820 ? availableWidth : 820;
		final left = viewportLeft + Std.int((viewportWidth - width) / 2);
		Raylib.DrawRectangle(left, centerY + 20, width, 30, Color.rgba(8, 20, 24));
		Raylib.DrawRectangleLines(left, centerY + 20, width, 30, CaxecraftPalette.selection());
		final tool = immersiveToolLabel(locale, activeTool);
		Raylib.DrawTextString('${uiCatalog.text(locale, UiMessage.EditorBuild)}  ·  $tool  ·  $label', left
			+ 12, centerY
			+ 27, 16, CaxecraftPalette.hudText());
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
			paletteCode = groundPaletteCode;
			if (paletteCode < 0) {
				notice = Invalid;
				return false;
			}
		}
		final toolResult = commandForTool(tool, point, toolContextFor(tool, draft, paletteCode));
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
					case MutationApplied(_, _, _, _, _, _):
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
					case MutationApplied(_, _, _, _, _, _):
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
			flowWorldPickMode = NoFlowWorldPick;
			presentationDraft = null;
			projection = null;
			planProjection = null;
			editLayerY = 0;
			objectGizmos = [];
			objectVisuals = [];
			objectLabels = "";
			flowRuleCount = 0;
			flowRules = [];
			zoneRuleLinks = [];
			flowOverlaps = [];
			environment = null;
			camera = null;
			selection = null;
			invalidatePreview();
			notice = Invalid;
			return;
		}
		#if caxecraft_pilot
		final refreshStarted = switch terrainRefresh {
			case KeepTerrain | RefreshTerrainVoxel(_, _): Raylib.GetTime();
			case RefreshAllTerrain: -1.0;
		};
		#end
		final previous = projection;
		var voxelProjectionPatched = false;
		final draft:EditorPresentationSnapshot = switch terrainRefresh {
			case KeepTerrain:
				switch current.query(InspectPresentationDetails) {
					case PresentationDetailsObserved(_, value): presentationWithProjection(value, previous);
					case _: throw "editor returned the wrong presentation-details observation";
				}
			case RefreshTerrainVoxel(point, paletteCode):
				switch current.query(InspectPresentationDetails) {
					case PresentationDetailsObserved(_, value):
						voxelProjectionPatched = previous != null && patchProjectedVoxel(previous, point, paletteCode);
						if (voxelProjectionPatched) presentationWithProjection(value, previous); else switch current.query(InspectPresentation) {
							case PresentationObserved(_, complete): complete;
							case _: throw "editor returned the wrong presentation observation";
						}
					case _: throw "editor returned the wrong presentation-details observation";
				}
			case RefreshAllTerrain:
				switch current.query(InspectPresentation) {
					case PresentationObserved(_, value): value;
					case _: throw "editor returned the wrong presentation observation";
				}
		};
		presentationDraft = draft;
		groundPaletteCode = normalizeBuildPaletteCode(draft.world.palette, groundPaletteCode,
			paletteCodeForBlock(draft.world.palette, contentRegistry.defaultEditorBlockId()));
		syncWorldName(draft.title);
		environment = draft.environment;
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
				case RefreshTerrainVoxel(point, _):
					final dirtyChunks = if (voxelProjectionPatched) terrainPresentation.refreshVoxel(draft.world, runtimeProjection, contentRegistry,
						point); else -1;
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
		flowRules = draft.flowRules;
		zoneRuleLinks = draft.zoneRuleLinks;
		flowOverlaps = draft.flowOverlaps;
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
			final previousCamera = camera;
			final mode = previousCamera == null ? EditorCameraMode.WalkCamera : cameraMode(previousCamera);
			final focus = cameraFocus(next);
			camera = focusCamera(next, mode, focus.target, focus.orbitDistance);
		} else {
			final currentCamera = camera;
			if (currentCamera != null && cameraMode(currentCamera) == EditorCameraMode.OrbitCamera)
				camera = retargetOrbitCamera(currentCamera, cameraFocus(next).target);
		}
		#if caxecraft_pilot
		if (refreshStarted >= 0.0)
			switch terrainRefresh {
				case KeepTerrain:
					pilotKeepTerrainRefreshMicroseconds += Std.int((Raylib.GetTime() - refreshStarted) * 1000000.0);
					pilotKeepTerrainRefreshCount++;
				case RefreshTerrainVoxel(_, _):
					pilotVoxelRefreshMicroseconds += Std.int((Raylib.GetTime() - refreshStarted) * 1000000.0);
					pilotVoxelRefreshCount++;
				case RefreshAllTerrain:
			}
		#end
	}

	/** Combine copied non-terrain details with the retained world projection. */
	function presentationWithProjection(details:EditorPresentationDetails, retained:Null<EditorWorldProjection>):EditorPresentationSnapshot {
		return {
			id: details.id,
			title: details.title,
			environment: details.environment,
			world: details.world,
			projection: retained,
			objects: details.objects,
			ruleIds: details.ruleIds,
			dialogueIds: details.dialogueIds,
			flowRuleCount: details.flowRuleCount,
			flowRules: details.flowRules,
			zoneRuleLinks: details.zoneRuleLinks,
			flowOverlaps: details.flowOverlaps
		};
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

	/** Replace the temporary object-name buffer only when selection identity changes. */
	function syncObjectName(id:ScenarioId):Void {
		final name = objectName;
		if (name == null)
			return;
		final current = objectNameTarget;
		if (current != null && current.text() == id.text())
			return;
		objectNameTarget = id;
		name.setEditing(false);
		name.replace(id.text());
	}

	#if caxecraft_pilot
	/** True when a fresh Build screen uses the direct surface-following mode. */
	public function pilotUsesWalkCamera():Bool {
		final current = camera;
		return current != null && cameraMode(current) == EditorCameraMode.WalkCamera;
	}

	/** Cycle through the production Camera control until Orbit owns the view. */
	public function applyPilotOrbitCamera():Bool {
		var attempts = 0;
		var current = camera;
		while (attempts < 3 && current != null && cameraMode(current) != EditorCameraMode.OrbitCamera) {
			cycleEditorCamera();
			attempts++;
			current = camera;
		}
		return current != null && cameraMode(current) == EditorCameraMode.OrbitCamera;
	}

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

	/**
	 * Exercise valid and invalid source through the production Text workspace.
	 *
	 * The pilot bypasses only operating-system key delivery. It selects real
	 * source rows, writes the owned Raygui line buffer, and uses the same Apply
	 * boundary as a creator. The valid title must publish once. The malformed
	 * closing record must then remain visible without changing playable state.
	 */
	public function applyPilotTextAuthoring():Bool {
		final current = session;
		final lineEditor = textLineEditor;
		if (current == null || lineEditor == null)
			return false;
		openTextWorkspace();
		var document = textDocument;
		if (document == null)
			return false;
		final titleLine = pilotTextLineStartingWith(document, "title ");
		if (titleLine < 0)
			return false;
		selectTextLine(titleLine);
		if (!lineEditor.replace('title literal "Pilot text workspace"'))
			return false;
		lineEditor.setEditing(false);
		if (!commitTextLine() || !applyTextWorkspace())
			return false;
		final accepted = current.canonicalDraft();
		if (accepted.toString().indexOf('title literal "Pilot text workspace"') < 0)
			return false;

		document = textDocument;
		if (document == null)
			return false;
		final endLine = pilotTextLineStartingWith(document, "end-map");
		if (endLine < 0)
			return false;
		final beforeRevision = current.revision();
		final beforeIdentity = current.stateIdentity();
		selectTextLine(endLine);
		if (!lineEditor.replace("end-map broken"))
			return false;
		lineEditor.setEditing(false);
		if (!commitTextLine() || applyTextWorkspace())
			return false;
		return textWorkspaceOpen
			&& document.isDirty()
			&& document.lineAt(endLine) == "end-map broken"
			&& textNotice == TextInvalid
			&& textDiagnostics.length > 0
			&& current.canonicalDraft().compare(accepted) == 0
			&& current.revision() == beforeRevision
			&& current.stateIdentity() == beforeIdentity;
	}

	/** Find one canonical source row without duplicating CAXEMAP grammar rules. */
	static function pilotTextLineStartingWith(document:EditorTextDocument, prefix:String):Int {
		for (index in 0...document.lineCount()) {
			final line = document.lineAt(index);
			if (line != null && StringTools.startsWith(line, prefix))
				return index;
		}
		return -1;
	}

	/** Publish pilot edits through the same package save action as the toolbar. */
	public function applyPilotSave():Bool
		return requestSave();

	/**
	 * Select a layer and prove that this presentation action changed no draft state.
	 *
	 * The native pilot uses this narrow seam instead of synthesizing a mouse
	 * click. It still runs the production layer-selection path and checks the
	 * canonical document, revision, undo/redo depth, and dirty state on both sides.
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
		final beforeDirty = isDirty();
		return selectEditLayer(layerY)
			&& editLayerY == layerY
			&& current.revision() == beforeRevision
			&& current.stateIdentity() == beforeIdentity
			&& current.canonicalDraft().compare(beforeCanonical) == 0
			&& current.undoDepth() == beforeUndoDepth
			&& current.redoDepth() == beforeRedoDepth
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

	/** Return total retained-terrain presentation time for this pilot run. */
	public inline function pilotKeepTerrainMicroseconds():Int
		return pilotKeepTerrainRefreshMicroseconds;

	/** Return the number of retained-terrain refreshes measured by this pilot. */
	public inline function pilotKeepTerrainCount():Int
		return pilotKeepTerrainRefreshCount;

	/** Return total one-voxel presentation time for this pilot run. */
	public inline function pilotVoxelMicroseconds():Int
		return pilotVoxelRefreshMicroseconds;

	/** Return the number of one-voxel refreshes measured by this pilot. */
	public inline function pilotVoxelCount():Int
		return pilotVoxelRefreshCount;

	/** Return total object undo and redo time for this pilot run. */
	public inline function pilotHistoryMicroseconds():Int
		return pilotHistoryRoundTripMicroseconds;

	/** Return total one-voxel terrain undo and redo time for this pilot run. */
	public inline function pilotTerrainHistoryMicroseconds():Int
		return pilotTerrainHistoryRoundTripMicroseconds;

	/** Undo and redo the newest voxel edit, then prove its canonical state returned. */
	public function applyPilotTerrainHistoryRoundTrip():Bool {
		final current = session;
		if (current == null || current.undoDepth() <= 0)
			return false;
		final beforeCanonical = current.canonicalDraft();
		final beforeUndoDepth = current.undoDepth();
		final beforeRedoDepth = current.redoDepth();
		final started = Raylib.GetTime();
		undo();
		redo();
		pilotTerrainHistoryRoundTripMicroseconds += Std.int((Raylib.GetTime() - started) * 1000000.0);
		return current.canonicalDraft().compare(beforeCanonical) == 0
			&& current.undoDepth() == beforeUndoDepth
			&& current.redoDepth() == beforeRedoDepth;
	}

	/** Undo and redo the newest edit, then prove that its canonical state returned. */
	public function applyPilotHistoryRoundTrip():Bool {
		final current = session;
		if (current == null || current.undoDepth() <= 0)
			return false;
		final beforeCanonical = current.canonicalDraft();
		final beforeUndoDepth = current.undoDepth();
		final beforeRedoDepth = current.redoDepth();
		final started = Raylib.GetTime();
		undo();
		redo();
		pilotHistoryRoundTripMicroseconds += Std.int((Raylib.GetTime() - started) * 1000000.0);
		return current.canonicalDraft().compare(beforeCanonical) == 0
			&& current.undoDepth() == beforeUndoDepth
			&& current.redoDepth() == beforeRedoDepth;
	}

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

	/** Open and search the browser while leaving its real modal visible to the pilot. */
	public function applyPilotCatalogSearch():Bool {
		final search = assetSearch;
		if (projection == null || search == null)
			return false;
		search.clear();
		assetQuery = "";
		openAssetBrowser();
		setAssetCategory(EditorAssetCategory.TerrainAssets);
		for (_ in 0...4)
			applyNavigation(NavigationCommand.Right);
		if (assetCategory != EditorAssetCategory.MechanismAssets)
			return false;
		final unfiltered = visibleAssetEntries();
		if (unfiltered.length == 0)
			return false;
		final fullLabel = unfiltered[0].labelEn;
		final queryLength = fullLabel.length < 3 ? fullLabel.length : 3;
		if (queryLength == 0 || !search.replace(fullLabel.substr(0, queryLength)))
			return false;
		assetQuery = search.text();
		refreshVisibleAssets(true);
		final visible = visibleAssetEntries();
		if (visible.length == 0)
			return false;
		assetSelection = 0;
		return assetBrowserOpen && search.text().length == queryLength;
	}

	/** Choose the searched pilot row, then place its recipe on authored terrain. */
	public function applyPilotCatalogObject():Bool {
		final current = projection;
		if (current == null || !assetBrowserOpen)
			return false;
		final visible = visibleAssetEntries();
		if (assetSelection < 0 || assetSelection >= visible.length)
			return false;
		chooseAsset(visible[assetSelection]);
		if (assetBrowserOpen || activeRecipeFor(EditorTool.CatalogObjectTool) == null)
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

	/** Select the first validated actor visual for the Orbit screenshot. */
	public function applyPilotSelectFirstActor():Bool {
		for (index in 0...objectVisuals.length)
			switch objectVisuals[index] {
				case ActorVisual(_, _):
					setActiveTool(SelectTool);
					selectObject(objectGizmos[index].id);
					return selectedObjectIndex() == index;
				case PlayerSpawnVisual | CheckpointVisual | ItemVisual(_, _) | StatefulObjectVisual(_, _) | TriggerVolumeVisual | FallbackObjectVisual:
			}
		return false;
	}

	/**
	 * Place, turn, copy, and delete the selected actor through Build actions.
	 *
	 * The pilot bypasses only operating-system key delivery. Pointer ownership,
	 * Select mode, camera-relative direction, revisioned commands, projection
	 * refresh, and the Orbit target use the same path as interactive input.
	 */
	public function applyPilotDirectObjectEdits():Bool {
		var currentCamera = camera;
		var selected = selectedObjectIndex();
		if (currentCamera == null || selected < 0)
			return false;
		var aimed = -1;
		for (index in 0...objectGizmos.length)
			if (aimed < 0 && index != selected)
				switch objectVisuals[index] {
					case ActorVisual(_, _):
						aimed = index;
					case PlayerSpawnVisual | CheckpointVisual | ItemVisual(_, _) | StatefulObjectVisual(_, _) | TriggerVolumeVisual | FallbackObjectVisual:
				}
		if (aimed < 0)
			return false;
		updateObjectGrab(objectGizmos[selected].id, objectGizmos[aimed].id, true);
		selected = selectedObjectIndex();
		if (selected != aimed || !objectGrabActive(objectGrab))
			return false;
		final source = objectGizmos[selected];
		final target:VoxelPoint = {x: source.origin.x + 1, y: source.origin.y, z: source.origin.z};
		if (!moveObject(source.id, objectPlacementDelta(source, target)))
			return false;
		objectGrab = NoObjectGrab;
		currentCamera = camera;
		selected = selectedObjectIndex();
		if (currentCamera == null || selected < 0)
			return false;
		final pose = cameraPose(currentCamera);
		if (!applyBuildObjectAction(objectAction({
			pointerCaptured: buildPointerState == EditorBuildPointerState.Captured,
			selectToolActive: activeTool == SelectTool,
			objectSelected: true,
			objectCanTurn: objectCanTurn(selected),
			upPressed: false,
			rightPressed: false,
			downPressed: false,
			leftPressed: false,
			turnPressed: true,
			lookX: pose.lookX,
			lookZ: pose.lookZ
		})))
			return false;
		final editedId = source.id;
		final beforeDuplicate = objectGizmos.length;
		duplicateSelectedObject();
		final duplicateIndex = selectedObjectIndex();
		if (objectGizmos.length != beforeDuplicate + 1 || duplicateIndex < 0 || objectGizmos[duplicateIndex].id.text() == editedId.text())
			return false;
		deleteSelectedObject();
		if (objectGizmos.length != beforeDuplicate || selectedObjectIndex() >= 0)
			return false;
		selectObject(editedId);
		return selectedObjectIndex() >= 0;
	}

	/**
		Exercise native trigger and WHEN/IF/DO authoring through production commands.

		The pilot selects real projected objects and bypasses only operating-system
		pointer delivery. Resize, connect, typed world pick, card edits, copy cleanup,
		validation, history, canonical save, and Test Play keep their ordinary owners.
	**/
	public function applyPilotFlowAuthoring():Bool {
		var triggerIndex = -1;
		var actorIndex = -1;
		for (index in 0...objectGizmos.length)
			switch objectVisuals[index] {
				case TriggerVolumeVisual if (triggerIndex < 0):
					triggerIndex = index;
				case ActorVisual(_, _) if (actorIndex < 0):
					actorIndex = index;
				case _:
			}
		if (triggerIndex < 0 || actorIndex < 0)
			return false;
		final zone = objectGizmos[triggerIndex];
		final actor = objectGizmos[actorIndex];
		final draft = presentationDraft;
		if (draft == null)
			return false;
		var actorObject:Null<ScenarioObject> = null;
		for (object in draft.objects)
			if (object.id.text() == actor.id.text())
				actorObject = object;
		if (actorObject == null)
			return false;
		final connected = nextZoneConnectionRuleId(zone.id, draft.ruleIds);

		selectObject(zone.id);
		resizeSelectedTrigger({
			width: Std.int(zone.width) + 1,
			height: Std.int(zone.height),
			depth: Std.int(zone.depth)
		});
		beginZoneConnection(zone.id);
		if (!applyFlowObjectPick(actorObject))
			return false;
		final current = session;
		if (current == null)
			return false;
		final supportCounter = new ScenarioId('editor.counter.${connected.text()}');
		final supportSequence = new ScenarioId('editor.sequence.${connected.text()}');
		switch current.mutate({
			baseRevision: current.revision(),
			mutation: ApplyBatch([
				PutFlowVariable({id: supportCounter, scope: Map, initial: Counter(0)}),
				PutFlowSequence({id: supportSequence, parameters: [], actions: []})
			])
		}) {
			case MutationApplied(_, _, _, _, _, _):
				refreshProjection(false, KeepTerrain);
			case MutationUnchanged(_, _) | MutationRejected(_, _):
				return false;
		}
		final authoredDraft = currentDraftScenario();
		if (authoredDraft == null)
			return false;
		final event = pilotEventCard(authoredDraft, "leave-zone");
		final predicate = pilotPredicateCard(authoredDraft, EnterZone(zone.id), "all");
		final contentAction = pilotActionCard(authoredDraft, connected, "give-item", false);
		final delayedAction = pilotActionCard(authoredDraft, connected, "schedule", false);
		final nestedAction = pilotActionCard(authoredDraft, connected, "choose", false);
		if (event == null || predicate == null || contentAction == null || delayedAction == null || nestedAction == null)
			return false;
		if (!commitFlowEdit(zone.id, connected, ReplaceWhen(event))
			|| !commitFlowEdit(zone.id, connected, ReplaceIf(predicate))
			|| !commitFlowEdit(zone.id, connected, ReplaceDo(0, contentAction))
			|| !commitFlowEdit(zone.id, connected, InsertDo(1, delayedAction))
			|| !commitFlowEdit(zone.id, connected, InsertDo(2, nestedAction))
			|| !commitFlowEdit(zone.id, connected, InsertDo(3, Spawn(actor.id)))
			|| !commitFlowEdit(zone.id, connected, MoveDo(3, 0)))
			return false;

		final beforeObjects = objectGizmos.length;
		final beforeRules = flowRuleCount;
		var copiedRuleCount = 0;
		for (rule in flowRules)
			if (flowRuleUsesZone(rule, zone.id))
				copiedRuleCount++;
		if (copiedRuleCount < 1)
			return false;
		duplicateSelectedObject();
		if (objectGizmos.length != beforeObjects + 1 || flowRuleCount != beforeRules + copiedRuleCount)
			return false;
		deleteSelectedObject();
		if (objectGizmos.length != beforeObjects || flowRuleCount != beforeRules)
			return false;
		selectObject(zone.id);
		flowCardLibraryTarget = DoFlowCardPanel(zone.id, connected, 2);
		var authoredNestedCards = -1;
		for (rule in flowRules)
			if (rule.ruleId.text() == connected.text())
				authoredNestedCards = rule.nestedCards.length;
		return switch current.query(InspectValidation) {
			case ValidationObserved(_, DraftPlayable(_)): authoredNestedCards >= 2;
			case _:
				false;
		};
	}

	/** Find one complete event from the same registry palette drawn for creators. */
	function pilotEventCard(draft:Scenario, expected:String):Null<FlowEvent> {
		for (choice in eventCardChoices(draft, flowContentChoices))
			switch choice {
				case ReadyEventChoice(descriptor, value) if (descriptor.id.text() == expected):
					return value;
				case _:
			}
		return null;
	}

	/** Find one complete predicate from the same registry palette drawn for creators. */
	function pilotPredicateCard(draft:Scenario, event:FlowEvent, expected:String):Null<FlowPredicate> {
		for (choice in predicateCardChoices(draft, event, flowContentChoices))
			switch choice {
				case ReadyPredicateChoice(descriptor, value) if (descriptor.id.text() == expected):
					return value;
				case _:
			}
		return null;
	}

	/** Find one complete action from the same registry palette drawn for creators. */
	function pilotActionCard(draft:Scenario, owner:ScenarioId, expected:String, insideChoice:Bool):Null<FlowAction> {
		for (choice in actionCardChoices(draft, owner, flowContentChoices, insideChoice))
			switch choice {
				case ReadyActionChoice(descriptor, value) if (descriptor.id.text() == expected):
					return value;
				case _:
			}
		return null;
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

	/** Select Ground and capture Build for a representative contextual terrain frame. */
	public function applyPilotGroundPrompt():Bool {
		setActiveTool(PaintTool);
		return applyPilotBuildCapture() && activeTool == PaintTool;
	}
	#end
}
#end

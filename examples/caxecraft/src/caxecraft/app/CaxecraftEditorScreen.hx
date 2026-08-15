package caxecraft.app;

#if c
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorFocus.EditorFocusTarget;
import caxecraft.editor.EditorFocus.initialFocus;
import caxecraft.editor.EditorFocus.moveFocus;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorNodeRef;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorPreviewResult;
import caxecraft.editor.EditorTypes.EditorSelection;
import caxecraft.editor.EditorTypes.EditorSelectionResult;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.editor.EditorViewport.EditorViewportLayout;
import caxecraft.editor.EditorViewport.EditorViewportProjection;
import caxecraft.editor.EditorViewport.EditorTool;
import caxecraft.editor.EditorViewport.EditorToolCommandResult;
import caxecraft.editor.EditorViewport.commandFor as commandForTool;
import caxecraft.editor.EditorViewport.layout as layoutPlan;
import caxecraft.editor.EditorViewport.pointAt as pointAtPlan;
import caxecraft.editor.EditorViewport.project as projectPlan;
import caxecraft.editor.EditorViewport.paletteCodeForBlock;
import caxecraft.editor.EditorWorldViewport.EditorCameraInput;
import caxecraft.editor.EditorWorldViewport.EditorCameraState;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmo;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmoKind;
import caxecraft.editor.EditorWorldViewport.EditorWorldHit;
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.editor.EditorWorldViewport.cameraTarget;
import caxecraft.editor.EditorWorldViewport.focusCamera;
import caxecraft.editor.EditorWorldViewport.paletteCodeAtWorld;
import caxecraft.editor.EditorWorldViewport.pickWorld;
import caxecraft.editor.EditorWorldViewport.projectObjects;
import caxecraft.editor.EditorWorldViewport.projectWorld;
import caxecraft.editor.EditorWorldViewport.stepCamera;
import caxecraft.editor.EditorWorldViewport.surfaceTopAt;
import caxecraft.input.NavigationInput.NavigationCommand;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioText;
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
}

/** One editable view over the same canonical editor draft. */
private enum abstract EditorWorkspaceView(Int) {
	var BuildView = 0;
	var PlanView = 1;
}

/** The exact baseline exists only after the editor opens a valid document. */
private enum EditorOpeningState {
	NoOpenedEditor;
	OpenedEditor(canonical:Bytes);
}

/**
 * Native visual editor over the real renderer-independent editor session.
 *
 * This stateful class owns one mutable draft/session and small presentation
 * state. Raygui remains immediate-mode: every frame redraws controls, while
 * `EditorSession` continues to own validation, undo/redo, and the editing
 * lock. The application owns the disposable ordinary-engine runtime. The
 * screen opens a copy of the active level bytes. A cached
 * `EditorWorldProjection` stores its exact terrain, compact surface overview,
 * object gizmos, and content-logic count. Steady frames read that cache instead
 * of serializing the draft or maintaining a second editable world.
 *
 * The base-pack IDs and Raylib colors below belong at this Caxecraft
 * composition edge; the reusable editor package knows neither. Build and Plan
 * are two views over one draft, selection, active tool, and history. The first
 * child-facing slice edits terrain only. Native source save, object transforms,
 * layer controls, flow authoring, and cinematic tools remain separate. Test
 * Play uses a disposable ordinary game runtime while this class keeps the
 * exact editor workspace alive.
 */
final class CaxecraftEditorScreen {
	final contentRegistry:RuntimeContentRegistry;
	final uiCatalog:RuntimeUiCatalog;
	var session:Null<EditorSession>;
	var notice:EditorNotice;
	var projection:Null<EditorWorldProjection>;
	var planProjection:Null<EditorViewportProjection>;
	var objectGizmos:Array<EditorObjectGizmo>;
	var objectLabels:String;
	var flowRuleCount:Int;
	var camera:Null<EditorCameraState>;
	var selection:Null<VoxelBounds>;
	var focusedControl:EditorFocusTarget;
	var workspaceView:EditorWorkspaceView;
	var activeTool:EditorTool;
	var detailsOpen:Bool;
	var worldListOpen:Bool;
	var leavePromptOpen:Bool;
	var openingState:EditorOpeningState;
	var previewPoint:Null<VoxelPoint>;
	var previewRevision:Int;
	var previewTool:EditorTool;
	var previewAllowed:Bool;
	var objectList:GuiListViewState;

	/**
	 * Owns the temporary native editing bytes for the authored scenario title.
	 *
	 * Raygui edits this buffer during one frame, but `EditorSession` remains the
	 * document owner. Leaving edit mode submits one revision-checked `SetTitle`
	 * command, so validation, canonical CAXEMAP output, undo, and redo all see
	 * the same change. Rejected input is replaced with the accepted draft title.
	 */
	final worldName:Null<GuiTextBoxState>;

	/** Start with the same copy-owned CAXEMAP bytes as the active game generation. */
	public function new(contentRegistry:RuntimeContentRegistry, uiCatalog:RuntimeUiCatalog, activeLevelSource:Bytes) {
		this.contentRegistry = contentRegistry;
		this.uiCatalog = uiCatalog;
		session = switch EditorSession.openBytes(activeLevelSource, contentRegistry) {
			case EditorOpened(value): value;
			case EditorOpenRejected(_): null;
		};
		notice = session == null ? Invalid : Ready;
		projection = null;
		planProjection = null;
		objectGizmos = [];
		objectLabels = "";
		flowRuleCount = 0;
		camera = null;
		selection = null;
		focusedControl = initialFocus();
		workspaceView = BuildView;
		activeTool = SelectTool;
		detailsOpen = false;
		worldListOpen = false;
		leavePromptOpen = false;
		openingState = NoOpenedEditor;
		final openedSession = session;
		if (openedSession != null)
			openingState = OpenedEditor(openedSession.canonicalDraft());
		previewPoint = null;
		previewRevision = -1;
		previewTool = SelectTool;
		previewAllowed = false;
		objectList = new GuiListViewState();
		worldName = GuiTextBoxState.create(64);
		refreshProjection(true);
	}

	/**
	 * Draw one responsive editor frame and apply device-neutral navigation.
	 *
	 * The application supplies controller or pilot navigation as one semantic
	 * command. This screen reads keyboard input into the same command set, then
	 * routes both sources through `applyNavigation`.
	 */
	public function draw(locale:LocaleCursor, externalNavigation:NavigationCommand):EditorScreenAction {
		final width = Raylib.GetScreenWidth();
		final height = Raylib.GetScreenHeight();
		final keyboardNavigation = readKeyboardNavigation();
		final navigation = externalNavigation != NavigationCommand.None ? externalNavigation : keyboardNavigation;
		final navigationAction = applyNavigation(navigation);
		switch navigationAction {
			case StayInEditor:
			case ReturnToTitle | StartTestPlay(_):
				return navigationAction;
		}
		Raylib.ClearBackground(Color.rgba(12, 28, 36));
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
		final name = worldName;
		if (name != null) {
			final nameLeft = 126.0;
			final nameWidth = width >= 1180 ? 260.0 : 190.0;
			final result = name.draw(Rectangle.fromFloat(nameLeft, toolbarTop, nameWidth, 38.0));
			if (result.has(GuiResult.Pressed)) {
				focusedControl = EditorFocusTarget.WorldName;
				if (!name.isEditing())
					commitWorldName(name.text());
			}
			drawFocusRing(EditorFocusTarget.WorldName, Std.int(nameLeft), Std.int(toolbarTop), Std.int(nameWidth), 38);
		}

		final historyLeft = width >= 1180 ? 402.0 : 328.0;
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
		final inspectorVisible = selection != null || worldListOpen;
		final inspectorWidth = inspectorVisible && width >= 900 ? 250 : 0;
		final canvasLeft = 32;
		final canvasWidth = width - 64 - inspectorWidth - (inspectorWidth > 0 ? 12 : 0);
		final canvasHeight = shelfTop - canvasTop - 12;
		Raygui.PanelString(Rectangle.fromFloat(canvasLeft, canvasTop, canvasWidth, canvasHeight), uiCatalog.text(locale, UiMessage.EditorCanvasHelp));
		final innerLeft = canvasLeft + 12;
		final innerTop = canvasTop + 36;
		final innerWidth = canvasWidth - 24;
		final innerHeight = canvasHeight - 48;
		if (workspaceView == BuildView)
			drawWorldViewport(innerLeft, innerTop, innerWidth, innerHeight);
		else
			drawPlanViewport(innerLeft, innerTop, innerWidth, innerHeight);
		if (inspectorWidth > 0)
			drawInspector(locale, canvasLeft + canvasWidth + 12, canvasTop, inspectorWidth, canvasHeight);
		drawCreationShelf(locale, 32, shelfTop, width - 64, 112);

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

	/** Draw a clear second border around a selected view or creation card. */
	static function drawActiveControl(active:Bool, x:Int, y:Int, width:Int, height:Int):Void {
		if (!active)
			return;
		final color = CaxecraftPalette.selection();
		Raylib.DrawRectangleLines(x + 2, y + 2, width - 4, height - 4, color);
		Raylib.DrawRectangleLines(x + 3, y + 3, width - 6, height - 6, color);
	}

	/** Draw the three large terrain actions and small workspace disclosures. */
	function drawCreationShelf(locale:LocaleCursor, left:Int, top:Int, width:Int, height:Int):Void {
		Raygui.PanelString(Rectangle.fromFloat(left, top, width, height), "");
		final cardTop = top + 12;
		final cardWidth = width >= 940 ? 154 : 130;
		drawToolCard(locale, EditorFocusTarget.SelectTool, EditorTool.SelectTool, left + 12, cardTop, cardWidth, 68, UiMessage.EditorSelect,
			Color.rgba(84, 191, 205));
		drawToolCard(locale, EditorFocusTarget.GroundTool, EditorTool.PaintTool, left + 22 + cardWidth, cardTop, cardWidth, 68, UiMessage.EditorGround,
			Color.rgba(111, 174, 91));
		drawToolCard(locale, EditorFocusTarget.EraseTool, EditorTool.EraseTool, left + 32 + cardWidth * 2, cardTop, cardWidth, 68, UiMessage.EditorErase,
			Color.rgba(218, 103, 78));

		final disclosureWidth = 150.0;
		final disclosureLeft = left + width - Std.int(disclosureWidth) - 12;
		if (focusedButtonSized(EditorFocusTarget.WorldList, disclosureLeft, cardTop, disclosureWidth, 30.0, uiCatalog.text(locale, UiMessage.EditorWorldList)))
			worldListOpen = !worldListOpen;
		if (focusedButtonSized(EditorFocusTarget.MoreDetails, disclosureLeft, cardTop + 38.0, disclosureWidth, 30.0,
			uiCatalog.text(locale, UiMessage.EditorMoreDetails))
			&& selection != null)
			detailsOpen = !detailsOpen;
		drawActiveControl(worldListOpen, disclosureLeft, cardTop, Std.int(disclosureWidth), 30);
		drawActiveControl(detailsOpen && selection != null, disclosureLeft, cardTop + 38, Std.int(disclosureWidth), 30);

		final status = switch notice {
			case Ready: UiMessage.EditorReady;
			case Valid: UiMessage.EditorValid;
			case Invalid: UiMessage.EditorInvalid;
			case Testing: UiMessage.EditorTesting;
		};
		Raylib.DrawTextString(uiCatalog.text(locale, status), left + 12, top + height - 24, 14,
			notice == Invalid ? Color.rgba(255, 154, 112) : CaxecraftPalette.hudText());
	}

	/** Draw one large tool card with a non-text color mark and selected border. */
	function drawToolCard(locale:LocaleCursor, focus:EditorFocusTarget, tool:EditorTool, left:Int, top:Int, width:Int, height:Int, message:UiMessage,
			color:Color):Void {
		final pressed = Raygui.ButtonString(Rectangle.fromFloat(left, top, width, height), "").has(GuiResult.Pressed);
		if (pressed) {
			focusedControl = focus;
			setActiveTool(tool);
		}
		Raylib.DrawRectangle(left + 12, top + 14, 36, 36, color);
		Raylib.DrawRectangleLines(left + 12, top + 14, 36, 36, CaxecraftPalette.hudText());
		Raylib.DrawTextString(uiCatalog.text(locale, message), left + 58, top + 23, 17, CaxecraftPalette.hudText());
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

	/**
	 * Apply one navigation command to the editor's existing focus and actions.
	 *
	 * Up/left and down/right traverse the same cyclic order used by Tab.
	 * Confirm invokes the focused action, and Cancel returns to the title
	 * through the same typed screen transition as the keyboard Escape key.
	 * Keyboard, controller, and pilot commands all enter this one handler.
	 */
	public function applyNavigation(command:NavigationCommand):EditorScreenAction {
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
				focusedControl = moveFocus(focusedControl, Backward);
			case Right | Down:
				focusedControl = moveFocus(focusedControl, Forward);
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
			case Play:
				return requestTestPlay();
			case SelectTool:
				setActiveTool(EditorTool.SelectTool);
			case GroundTool:
				setActiveTool(EditorTool.PaintTool);
			case EraseTool:
				setActiveTool(EditorTool.EraseTool);
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

	/** Close the nearest presentation layer before offering to leave the draft. */
	function cancelEditorAction():EditorScreenAction {
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
		invalidatePreview();
	}

	/** Choose one creation card while preserving the current semantic selection. */
	function setActiveTool(tool:EditorTool):Void {
		activeTool = tool;
		invalidatePreview();
	}

	/** Leave immediately only when the in-memory draft still equals its opening bytes. */
	function requestLeave():EditorScreenAction {
		if (!isDirty())
			return ReturnToTitle;
		leavePromptOpen = true;
		focusedControl = EditorFocusTarget.KeepEditing;
		return StayInEditor;
	}

	/** Compare canonical bytes only when the user requests a destructive transition. */
	function isDirty():Bool {
		final current = session;
		if (current == null)
			return false;
		return switch openingState {
			case NoOpenedEditor: false;
			case OpenedEditor(opened): current.canonicalDraft().compare(opened) != 0;
		};
	}

	/** Select a World List object through the same stable workspace identity. */
	function selectObjectFromWorldList():Void {
		final current = session;
		final index = objectList.activeIndex();
		if (current == null || index < 0 || index >= objectGizmos.length)
			return;
		switch current.select({baseRevision: current.revision(), selection: NodeSelection(ObjectNode(objectGizmos[index].id))}) {
			case SelectionApplied(_, _) | SelectionUnchanged(_, _):
				selection = current.selectedBounds();
				notice = Ready;
			case SelectionRejected(_, _):
				notice = Invalid;
		}
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
		refreshProjection();
		return accepted;
	}

	/** Draw a top-down surface plan over the same draft used by Build. */
	function drawPlanViewport(left:Int, top:Int, width:Int, height:Int):Void {
		var currentPlan = planProjection;
		var currentWorld = projection;
		if (currentPlan == null || currentWorld == null || width <= 0 || height <= 0)
			return;
		var grid = layoutPlan(left, top, width, height, currentPlan);
		if (grid == null)
			return;

		final mouse = Raylib.GetMousePosition();
		final mouseX = Std.int(mouse.x.toFloat());
		final mouseY = Std.int(mouse.y.toFloat());
		final basePoint = pointAtPlan(currentPlan, grid, mouseX, mouseY);
		var hover = basePoint == null ? null : planToolPoint(currentWorld, basePoint.x, basePoint.z);
		if (hover == null)
			invalidatePreview();
		else {
			updatePreview(hover);
			if (Raylib.IsMouseButtonPressed(MouseButton.Left)) {
				final hoverX = hover.x;
				final hoverZ = hover.z;
				applyToolAt(activeTool, hover);
				currentPlan = planProjection;
				currentWorld = projection;
				if (currentPlan == null || currentWorld == null)
					return;
				grid = layoutPlan(left, top, width, height, currentPlan);
				if (grid == null)
					return;
				hover = planToolPoint(currentWorld, hoverX, hoverZ);
				if (hover != null)
					updatePreview(hover);
			}
		}

		Raylib.DrawRectangle(left, top, width, height, Color.rgba(18, 34, 42));
		for (z in 0...currentPlan.depth)
			for (x in 0...currentPlan.width) {
				final surfaceY = surfaceTopAt(currentWorld, x, z);
				final paletteCode = surfaceY < 0 ? 0 : paletteCodeAtWorld(currentWorld, x, surfaceY, z);
				final cellLeft = grid.left + x * grid.cellSize;
				final cellTop = grid.top + z * grid.cellSize;
				final color = surfaceY < 0 ? Color.rgba(25, 48, 56) : terrainOverviewColor(paletteCode);
				Raylib.DrawRectangle(cellLeft + 1, cellTop + 1, grid.cellSize - 2, grid.cellSize - 2, color);
				Raylib.DrawRectangleLines(cellLeft, cellTop, grid.cellSize, grid.cellSize, Color.rgba(48, 78, 84));
				if (selectedPlanCell(x, z)) {
					Raylib.DrawRectangleLines(cellLeft + 1, cellTop + 1, grid.cellSize - 2, grid.cellSize - 2, CaxecraftPalette.selection());
					Raylib.DrawRectangleLines(cellLeft + 2, cellTop + 2, grid.cellSize - 4, grid.cellSize - 4, CaxecraftPalette.selection());
				}
			}
		for (gizmo in objectGizmos) {
			final x = Std.int(gizmo.x);
			final z = Std.int(gizmo.z);
			if (x >= 0 && z >= 0 && x < currentPlan.width && z < currentPlan.depth) {
				final markerSize = grid.cellSize > 10 ? 8 : 4;
				final markerLeft = grid.left + x * grid.cellSize + Std.int((grid.cellSize - markerSize) / 2);
				final markerTop = grid.top + z * grid.cellSize + Std.int((grid.cellSize - markerSize) / 2);
				Raylib.DrawRectangle(markerLeft, markerTop, markerSize, markerSize, gizmoColor(gizmo.kind));
			}
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

	/** Choose the top visible cell or the first air cell for one Plan gesture. */
	function planToolPoint(world:EditorWorldProjection, x:Int, z:Int):Null<VoxelPoint> {
		final top = surfaceTopAt(world, x, z);
		var y = top < 0 ? 0 : top;
		switch activeTool {
			case PaintTool:
				final above = top + 1;
				if (above >= 0 && above < world.height)
					y = above;
			case SelectTool | EraseTool | FillTool:
		}
		return y < 0 || y >= world.height ? null : {x: x, y: y, z: z};
	}

	/** True when the current semantic voxel target covers one Plan column. */
	function selectedPlanCell(x:Int, z:Int):Bool {
		final current = selection;
		if (current == null)
			return false;
		return x >= current.origin.x
			&& z >= current.origin.z
			&& x < current.origin.x + current.size.width
			&& z < current.origin.z + current.size.depth;
	}

	/** Recompute a reducer-backed ghost only after its snapped input changes. */
	function updatePreview(point:VoxelPoint):Void {
		final current = session;
		if (current == null) {
			invalidatePreview();
			return;
		}
		if (previewPoint != null
			&& previewPoint.x == point.x
			&& previewPoint.y == point.y
			&& previewPoint.z == point.z
			&& previewRevision == current.revision()
			&& previewTool == activeTool)
			return;
		previewPoint = {x: point.x, y: point.y, z: point.z};
		previewRevision = current.revision();
		previewTool = activeTool;
		var paletteCode = 0;
		if (activeTool == PaintTool || activeTool == FillTool)
			paletteCode = paletteCodeForBlock(current.draftSnapshot().world, contentRegistry.defaultEditorBlockId());
		if (paletteCode < 0) {
			previewAllowed = false;
			return;
		}
		previewAllowed = switch commandForTool(activeTool, point, paletteCode, current.selectedBounds()) {
			case ToolCommandRejected(_): false;
			case ToolSelectionReady(_): true;
			case ToolCommandReady(command):
				switch current.preview({baseRevision: current.revision(), commands: [command]}) {
					case PreviewAccepted(_, _, _) | PreviewUnchanged(_, _): true;
					case PreviewRejected(_, _): false;
				}
		};
	}

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
	 * renderer-independent. The right mouse button looks around, WASD/QE flies,
	 * the wheel moves along the view, and F restores the deterministic world
	 * focus. Left click submits the selected tool through `applyToolAt`.
	 */
	function drawWorldViewport(left:Int, top:Int, width:Int, height:Int):Void {
		var current = projection;
		var currentCamera = camera;
		if (current == null || currentCamera == null || width <= 0 || height <= 0)
			return;
		final mouse = Raylib.GetMousePosition();
		final mouseX = Std.int(mouse.x.toFloat());
		final mouseY = Std.int(mouse.y.toFloat());
		final inside = mouseX >= left && mouseY >= top && mouseX < left + width && mouseY < top + height;
		final name = worldName;
		final cameraInputEnabled = inside && (name == null || !name.isEditing());
		if (cameraInputEnabled && Raylib.IsKeyPressed(KeyboardKey.F))
			currentCamera = focusCamera(current);
		else if (cameraInputEnabled) {
			final delta = Raylib.GetMouseDelta();
			final looking = Raylib.IsMouseButtonDown(MouseButton.Right);
			currentCamera = stepCamera(current, currentCamera, {
				forward: axis(Raylib.IsKeyDown(KeyboardKey.W), Raylib.IsKeyDown(KeyboardKey.S)),
				right: axis(Raylib.IsKeyDown(KeyboardKey.D), Raylib.IsKeyDown(KeyboardKey.A)),
				vertical: axis(Raylib.IsKeyDown(KeyboardKey.E), Raylib.IsKeyDown(KeyboardKey.Q)),
				yaw: looking ? -delta.x.toFloat() * 0.004 : 0.0,
				pitch: looking ? -delta.y.toFloat() * 0.004 : 0.0,
				wheel: Raylib.GetMouseWheelMove().toFloat()
			}, Raylib.GetFrameTime().toFloat());
		}
		camera = currentCamera;
		final target = cameraTarget(currentCamera);
		final nativeCamera = Camera3D.make(Vector3.fromFloat(currentCamera.x, currentCamera.y, currentCamera.z),
			Vector3.fromFloat(target.x, target.y, target.z), Vector3.fromFloat(0.0, 1.0, 0.0), c.Float32.fromFloat(52.0), CameraProjection.Perspective);
		var hover:Null<EditorWorldHit> = null;
		if (inside) {
			final ray = Raylib.GetScreenToWorldRay(mouse, nativeCamera);
			final origin = ray.position;
			final direction = ray.direction;
			hover = pickWorld(current, {x: origin.x.toFloat(), y: origin.y.toFloat(), z: origin.z.toFloat()}, {
				x: direction.x.toFloat(),
				y: direction.y.toFloat(),
				z: direction.z.toFloat()
			}, 0, 512.0);
		}
		if (hover == null)
			invalidatePreview();
		else
			updatePreview(hover.point);
		if (hover != null && Raylib.IsMouseButtonPressed(MouseButton.Left)) {
			applyToolAt(activeTool, hover.point);
			current = projection;
			if (current == null)
				return;
			updatePreview(hover.point);
		}

		Raylib.DrawRectangle(left, top, width, height, CaxecraftPalette.sky());
		Raylib.BeginScissorMode(left, top, width, height);
		Raylib.BeginMode3D(nativeCamera);
		Raylib.DrawCube(Vector3.fromFloat(current.width * 0.5, -0.04, current.depth * 0.5), c.Float32.fromFloat(current.width), c.Float32.fromFloat(0.08),
			c.Float32.fromFloat(current.depth), Color.rgba(26, 43, 50));
		for (x in 0...current.width + 1)
			Raylib.DrawLine3D(Vector3.fromFloat(x, 0.002, 0.0), Vector3.fromFloat(x, 0.002, current.depth), Color.rgba(55, 79, 85));
		for (z in 0...current.depth + 1)
			Raylib.DrawLine3D(Vector3.fromFloat(0.0, 0.002, z), Vector3.fromFloat(current.width, 0.002, z), Color.rgba(55, 79, 85));
		drawTerrainOverview(current);
		final selected = selection;
		if (selected != null)
			for (z in selected.origin.z...selected.origin.z + selected.size.depth)
				for (y in selected.origin.y...selected.origin.y + selected.size.height)
					for (x in selected.origin.x...selected.origin.x + selected.size.width)
						drawCellOutline(x, y, z, paletteCodeAtWorld(current, x, y, z) != 0, CaxecraftPalette.selection(), 1.05);
		final selectedObject = objectList.activeIndex();
		for (index in 0...objectGizmos.length) {
			final gizmo = objectGizmos[index];
			final color = index == selectedObject ? CaxecraftPalette.selection() : gizmoColor(gizmo.kind);
			Raylib.DrawCubeWires(Vector3.fromFloat(gizmo.x, gizmo.y, gizmo.z), c.Float32.fromFloat(gizmo.width), c.Float32.fromFloat(gizmo.height),
				c.Float32.fromFloat(gizmo.depth), color);
			if (index == selectedObject)
				Raylib.DrawCubeWires(Vector3.fromFloat(gizmo.x, gizmo.y, gizmo.z), c.Float32.fromFloat(gizmo.width + 0.10),
					c.Float32.fromFloat(gizmo.height + 0.10), c.Float32.fromFloat(gizmo.depth + 0.10), color);
		}
		if (hover != null && !selectedCell(hover.point.x, hover.point.y, hover.point.z)) {
			final previewColor = previewAllowed ? Color.rgba(92, 240, 186) : Color.rgba(255, 104, 82);
			drawCellOutline(hover.point.x, hover.point.y, hover.point.z, hover.solid, previewColor, 1.08);
			if (!previewAllowed) {
				final markerY = hover.solid ? hover.point.y + 1.02 : hover.point.y + 0.08;
				Raylib.DrawLine3D(Vector3.fromFloat(hover.point.x + 0.1, markerY, hover.point.z + 0.1),
					Vector3.fromFloat(hover.point.x + 0.9, markerY, hover.point.z + 0.9), previewColor);
				Raylib.DrawLine3D(Vector3.fromFloat(hover.point.x + 0.9, markerY, hover.point.z + 0.1),
					Vector3.fromFloat(hover.point.x + 0.1, markerY, hover.point.z + 0.9), previewColor);
			}
		}
		Raylib.EndMode3D();
		Raylib.EndScissorMode();
	}

	/**
	 * Draw the map's visible height surface in one Raylib batch.
	 *
	 * The exact voxel cache still owns picking and edits. This compact shell
	 * omits hidden caves so a large authored map remains responsive while the
	 * editor does not yet have layer inspection.
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
		if (current == null) {
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
		}
		if (needsPalette) {
			paletteCode = paletteCodeForBlock(current.draftSnapshot().world, contentRegistry.defaultEditorBlockId());
			if (paletteCode < 0) {
				notice = Invalid;
				return false;
			}
		}
		final toolResult = commandForTool(tool, point, paletteCode, current.selectedBounds());
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
						refreshProjection();
						true;
					case MutationUnchanged(_, _):
						invalidatePreview();
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
	 * `draftSnapshot` performs a defensive CAXEMAP round trip, so this method is
	 * intentionally called after New World, an accepted edit, undo, or redo—not
	 * from every frame.
	 */
	function refreshProjection(resetCamera:Bool = false):Void {
		final current = session;
		if (current == null) {
			projection = null;
			planProjection = null;
			objectGizmos = [];
			objectLabels = "";
			flowRuleCount = 0;
			camera = null;
			selection = null;
			invalidatePreview();
			notice = Invalid;
			return;
		}
		final draft = current.draftSnapshot();
		syncWorldName(draft.title);
		final previous = projection;
		projection = projectWorld(draft.world);
		planProjection = projectPlan(draft.world, 0);
		objectGizmos = projectObjects(draft.objects);
		flowRuleCount = draft.flow.rules.length;
		final labels:Array<String> = [];
		for (gizmo in objectGizmos)
			labels.push(gizmo.id.text());
		objectLabels = labels.join(";");
		if (objectList.activeIndex() < 0 || objectList.activeIndex() >= objectGizmos.length)
			objectList = new GuiListViewState();
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

	/** Paint and select the first visible air cell above authored terrain. */
	public function applyPilotPaintFirstAir():Bool {
		final current = projection;
		if (current == null)
			return false;
		for (z in 0...current.depth)
			for (x in 0...current.width) {
				final y = surfaceTopAt(current, x, z) + 1;
				if (y >= 0 && y < current.height && paletteCodeAtWorld(current, x, y, z) == 0) {
					final point:VoxelPoint = {x: x, y: y, z: z};
					return applyToolAt(EditorTool.PaintTool, point) && applyToolAt(EditorTool.SelectTool, point);
				}
			}
		return false;
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
	#end
}
#end

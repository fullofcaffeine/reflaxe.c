package caxecraft.app;

#if c
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.editor.EditorScenarioFactory.createBlank as createBlankEditorScenario;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorFocus.EditorFocusTarget;
import caxecraft.editor.EditorFocus.initialFocus;
import caxecraft.editor.EditorFocus.moveFocus;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorTestPlayResult;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.editor.EditorViewport.EditorTool;
import caxecraft.editor.EditorViewport.EditorToolCommandResult;
import caxecraft.editor.EditorViewport.commandFor as commandForTool;
import caxecraft.editor.EditorViewport.toolFromIndex;
import caxecraft.editor.EditorWorldViewport.EditorCameraInput;
import caxecraft.editor.EditorWorldViewport.EditorCameraState;
import caxecraft.editor.EditorWorldViewport.EditorWorldHit;
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.editor.EditorWorldViewport.cameraTarget;
import caxecraft.editor.EditorWorldViewport.focusCamera;
import caxecraft.editor.EditorWorldViewport.paletteCodeAtWorld;
import caxecraft.editor.EditorWorldViewport.pickWorld;
import caxecraft.editor.EditorWorldViewport.projectWorld;
import caxecraft.editor.EditorWorldViewport.stepCamera;
import caxecraft.input.NavigationInput.NavigationCommand;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioText;
import raygui.GuiListViewState;
import raygui.GuiResult;
import raygui.GuiTextBoxState;
import raygui.GuiToggleState;
import raygui.Raygui;
import raylib.Camera3D;
import raylib.CameraProjection;
import raylib.Color;
import raylib.KeyboardKey;
import raylib.MouseButton;
import raylib.Raylib;
import raylib.Rectangle;
import raylib.Vector3;

/** What the application should do after handling one editor frame. */
enum EditorScreenAction {
	StayInEditor;
	ReturnToTitle;
}

private enum EditorNotice {
	Ready;
	Valid;
	Invalid;
	Testing;
}

/**
 * Native visual editor over the real renderer-independent editor session.
 *
 * This stateful class owns one mutable draft/session and small presentation
 * state. Raygui remains immediate-mode: every frame redraws controls, while
 * `EditorSession` continues to own validation, undo/redo, and disposable test
 * play. A cached `EditorWorldProjection` reads the complete finite terrain
 * volume after a successful edit; steady frames draw and pick that cache
 * instead of serializing the draft or maintaining a second editable world.
 *
 * The base-pack IDs and Raylib colors below belong at this Caxecraft
 * composition edge; the reusable editor package knows neither. The first 3D
 * slice edits layer zero while the cache and camera already describe a volume.
 * Native save/load, layer controls, object gizmos, and cinematic tools remain
 * planned separately.
 */
final class CaxecraftEditorScreen {
	final contentRegistry:RuntimeContentRegistry;
	final uiCatalog:RuntimeUiCatalog;
	var session:Null<EditorSession>;
	var notice:EditorNotice;
	var projection:Null<EditorWorldProjection>;
	var camera:Null<EditorCameraState>;
	var selection:Null<VoxelBounds>;
	var focusedControl:EditorFocusTarget;
	final advancedTools:GuiToggleState;
	final toolList:GuiListViewState;

	/**
	 * Owns the temporary native editing bytes for the authored scenario title.
	 *
	 * Raygui edits this buffer during one frame, but `EditorSession` remains the
	 * document owner. Leaving edit mode submits one revision-checked `SetTitle`
	 * command, so validation, canonical CAXEMAP output, undo, and redo all see
	 * the same change. Rejected input is replaced with the accepted draft title.
	 */
	final worldName:Null<GuiTextBoxState>;

	/** Start an editor whose mechanics and labels come from one runtime generation. */
	public function new(contentRegistry:RuntimeContentRegistry, uiCatalog:RuntimeUiCatalog) {
		this.contentRegistry = contentRegistry;
		this.uiCatalog = uiCatalog;
		session = openNewWorld();
		notice = Ready;
		projection = null;
		camera = null;
		selection = null;
		focusedControl = initialFocus();
		advancedTools = new GuiToggleState(false);
		toolList = new GuiListViewState();
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
		if (navigationAction == ReturnToTitle)
			return ReturnToTitle;
		Raylib.ClearBackground(Color.rgba(12, 28, 36));
		final outer = Rectangle.fromFloat(16.0, 16.0, width - 32.0, height - 32.0);
		if (Raygui.WindowBoxString(outer, uiCatalog.text(locale, UiMessage.EditorTitle)).has(GuiResult.Pressed)) {
			focusedControl = EditorFocusTarget.Back;
			return ReturnToTitle;
		}

		final toolbarTop = 56.0;
		final buttonWidth = 116.0;
		final buttonGap = 10.0;
		var buttonLeft = 32.0;
		if (focusedButton(EditorFocusTarget.NewWorld, buttonLeft, toolbarTop, buttonWidth, uiCatalog.text(locale, UiMessage.EditorNewWorld))) {
			session = openNewWorld();
			notice = Ready;
			refreshProjection(true);
		}
		buttonLeft += buttonWidth + buttonGap;
		if (focusedButton(EditorFocusTarget.Undo, buttonLeft, toolbarTop, buttonWidth, uiCatalog.text(locale, UiMessage.EditorUndo)))
			undo();
		buttonLeft += buttonWidth + buttonGap;
		if (focusedButton(EditorFocusTarget.Redo, buttonLeft, toolbarTop, buttonWidth, uiCatalog.text(locale, UiMessage.EditorRedo)))
			redo();
		buttonLeft += buttonWidth + buttonGap;
		if (focusedButton(EditorFocusTarget.Validate, buttonLeft, toolbarTop, buttonWidth, uiCatalog.text(locale, UiMessage.EditorValidate)))
			validate();
		buttonLeft += buttonWidth + buttonGap;
		final testing = session != null && session.testPlay() != null;
		final testLabel = testing ? UiMessage.EditorStopTest : UiMessage.EditorTest;
		if (focusedButton(EditorFocusTarget.TestPlay, buttonLeft, toolbarTop, buttonWidth, uiCatalog.text(locale, testLabel)))
			toggleTestPlay();

		final viewportTop = 104.0;
		final sidebarWidth = 230;
		Raygui.PanelString(Rectangle.fromFloat(32.0, viewportTop, width - sidebarWidth - 80.0, height - viewportTop - 70.0),
			uiCatalog.text(locale, UiMessage.EditorCanvasHelp));
		Raygui.PanelString(Rectangle.fromFloat(width - sidebarWidth - 32.0, viewportTop, sidebarWidth, height - viewportTop - 70.0),
			uiCatalog.text(locale, UiMessage.EditorReady));
		final toolLeft = width - sidebarWidth - 16;
		final toolWidth = sidebarWidth - 32;
		final toolListResult = toolList.drawString(Rectangle.fromFloat(toolLeft, viewportTop + 44.0, toolWidth, 116.0),
			uiCatalog.text(locale, UiMessage.EditorToolList));
		if (toolListResult.has(GuiResult.Pressed))
			focusedControl = EditorFocusTarget.ToolList;
		drawFocusRing(EditorFocusTarget.ToolList, toolLeft, Std.int(viewportTop + 44.0), toolWidth, 116);
		final toggleResult = Raygui.ToggleString(Rectangle.fromFloat(toolLeft, viewportTop + 172.0, toolWidth, 32.0),
			uiCatalog.text(locale, UiMessage.EditorAdvanced), advancedTools);
		if (toggleResult.has(GuiResult.Pressed))
			focusedControl = EditorFocusTarget.AdvancedTools;
		drawFocusRing(EditorFocusTarget.AdvancedTools, toolLeft, Std.int(viewportTop + 172.0), toolWidth, 32);
		Raygui.LabelString(Rectangle.fromFloat(width - sidebarWidth - 16.0, viewportTop + 216.0, sidebarWidth - 32.0, 24.0),
			uiCatalog.text(locale, UiMessage.EditorName));
		final name = worldName;
		if (name != null) {
			final result = name.draw(Rectangle.fromFloat(toolLeft, viewportTop + 242.0, toolWidth, 32.0));
			if (result.has(GuiResult.Pressed)) {
				focusedControl = EditorFocusTarget.WorldName;
				if (!name.isEditing())
					commitWorldName(name.text());
			}
			drawFocusRing(EditorFocusTarget.WorldName, toolLeft, Std.int(viewportTop + 242.0), toolWidth, 32);
		}
		drawWorldViewport(48, 144, width - sidebarWidth - 112, height - 230);

		final status = switch notice {
			case Ready: UiMessage.EditorReady;
			case Valid: UiMessage.EditorValid;
			case Invalid: UiMessage.EditorInvalid;
			case Testing: UiMessage.EditorTesting;
		};
		Raygui.StatusBarString(Rectangle.fromFloat(32.0, height - 54.0, width - 190.0, 28.0), uiCatalog.text(locale, status));
		if (focusedButton(EditorFocusTarget.Back, width - 142.0, height - 54.0, 110.0, uiCatalog.text(locale, UiMessage.EditorBack)))
			return ReturnToTitle;
		return StayInEditor;
	}

	static inline function button(x:Float, y:Float, width:Float, text:String):Bool
		return Raygui.ButtonString(Rectangle.fromFloat(x, y, width, 32.0), text).has(GuiResult.Pressed);

	/**
	 * Draw one button, remember pointer focus, and paint keyboard focus.
	 *
	 * Raygui still owns hit testing for the immediate native control. This
	 * screen owns semantic focus, so mouse and keyboard routes converge before
	 * the existing editor action runs.
	 */
	function focusedButton(target:EditorFocusTarget, x:Float, y:Float, width:Float, text:String):Bool {
		final pressed = button(x, y, width, text);
		if (pressed)
			focusedControl = target;
		drawFocusRing(target, Std.int(x), Std.int(y), Std.int(width), 32);
		return pressed;
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
		switch command {
			case Up | Left:
				focusedControl = moveFocus(focusedControl, Backward);
			case Right | Down:
				focusedControl = moveFocus(focusedControl, Forward);
			case Confirm:
				return activateFocusedControl();
			case Cancel:
				return ReturnToTitle;
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
			case NewWorld:
				session = openNewWorld();
				notice = Ready;
				refreshProjection(true);
			case Undo:
				undo();
			case Redo:
				redo();
			case Validate:
				validate();
			case TestPlay:
				toggleTestPlay();
			case ToolList:
				toolList.moveSelection(4, 1);
			case AdvancedTools:
				advancedTools.active = !advancedTools.active;
			case WorldName:
				final name = worldName;
				if (name != null)
					name.setEditing(true);
			case Back:
				return ReturnToTitle;
		}
		return StayInEditor;
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

	function validate():Void {
		if (session == null) {
			notice = Invalid;
			return;
		}
		notice = switch session.validate() {
			case ValidationPassed(_): Valid;
			case ValidationFailed(_) | ValidationBlocked(_): Invalid;
		};
	}

	function toggleTestPlay():Void {
		if (session == null) {
			notice = Invalid;
			return;
		}
		if (session.testPlay() != null) {
			session.leaveTestPlay();
			notice = Valid;
			return;
		}
		notice = switch session.enterTestPlay() {
			case TestPlayStarted: Testing;
			case TestPlayRejected(_) | TestPlayBlocked(_): Invalid;
		};
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
		if (hover != null && Raylib.IsMouseButtonPressed(MouseButton.Left)) {
			final tool = toolFromIndex(toolList.activeIndex());
			if (tool == null)
				notice = Invalid;
			else
				applyToolAt(tool, hover.point);
			current = projection;
			if (current == null)
				return;
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
		for (z in 0...current.depth)
			for (y in 0...current.height)
				for (x in 0...current.width) {
					final code = paletteCodeAtWorld(current, x, y, z);
					if (code != 0)
						Raylib.DrawCube(Vector3.fromFloat(x + 0.5, y + 0.5, z + 0.5), c.Float32.fromFloat(1.0), c.Float32.fromFloat(1.0),
							c.Float32.fromFloat(1.0), code == 1 ? Color.rgba(83, 145, 92) : Color.rgba(91, 107, 117));
					if (selectedCell(x, y, z))
						drawCellOutline(x, y, z, code != 0, CaxecraftPalette.selection(), 1.05);
				}
		if (hover != null && !selectedCell(hover.point.x, hover.point.y, hover.point.z))
			drawCellOutline(hover.point.x, hover.point.y, hover.point.z, hover.solid, Color.rgba(109, 223, 232), 1.08);
		Raylib.EndMode3D();
		Raylib.EndScissorMode();
	}

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
		final command = commandForTool(tool, point, 1, current.selectedBounds());
		return switch command {
			case ToolCommandRejected(_):
				notice = Invalid;
				false;
			case ToolCommandReady(value):
				switch current.mutate({baseRevision: current.revision(), mutation: Apply(value)}) {
					case MutationApplied(_, _, _, _, _):
						notice = Ready;
						refreshProjection();
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
	 * `draftSnapshot` performs a defensive CAXEMAP round trip, so this method is
	 * intentionally called after New World, an accepted edit, undo, or redo—not
	 * from every frame.
	 */
	function refreshProjection(resetCamera:Bool = false):Void {
		final current = session;
		if (current == null) {
			projection = null;
			camera = null;
			selection = null;
			notice = Invalid;
			return;
		}
		final draft = current.draftSnapshot();
		syncWorldName(draft.title);
		final previous = projection;
		projection = projectWorld(draft.world);
		selection = current.selectedBounds();
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

	/** Create the built-in blank draft without teaching the generic editor a pack ID. */
	function openNewWorld():Null<EditorSession> {
		final draft = createBlankEditorScenario(new ScenarioId("editor.new-world"), new LogicalPath(contentRegistry.logicalPath()),
			ScenarioText.Literal("Untitled world"), ScenarioMode.Creative, contentRegistry.airBlockId(), new ScenarioId("player.spawn"), {
				width: 12,
				height: 1,
				depth: 12
			}, [{code: 1, blockType: contentRegistry.defaultEditorBlockId()}]);
		return switch EditorSession.open(draft, contentRegistry) {
			case EditorOpened(value): value;
			case EditorOpenRejected(_): null;
		};
	}
}
#end

package caxecraft.app;

#if c
import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.BaseBlock;
import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.editor.EditorScenarioFactory.createBlank as createBlankEditorScenario;
import caxecraft.editor.EditorSession;
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
import caxecraft.localization.UiCatalog;
import caxecraft.localization.UiCatalog.LocaleCursor;
import caxecraft.localization.UiCatalog.UiMessage;
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
	var session:Null<EditorSession>;
	var notice:EditorNotice;
	var projection:Null<EditorWorldProjection>;
	var camera:Null<EditorCameraState>;
	var selection:Null<VoxelBounds>;
	final advancedTools:GuiToggleState;
	final toolList:GuiListViewState;

	/**
	 * Owns the visible draft-name text without claiming persistence support.
	 *
	 * This first field proves safe native editing and localization-aware layout.
	 * It deliberately does not rename the CAXEMAP yet: a later typed session
	 * command must make that change undoable, validated, and serializable.
	 */
	final worldName:Null<GuiTextBoxState>;

	public function new() {
		session = openNewWorld();
		notice = Ready;
		projection = null;
		camera = null;
		selection = null;
		advancedTools = new GuiToggleState(false);
		toolList = new GuiListViewState();
		worldName = GuiTextBoxState.create(64);
		refreshProjection(true);
	}

	/** Draw one responsive editor frame and apply controls to the real session. */
	public function draw(locale:LocaleCursor):EditorScreenAction {
		final width = Raylib.GetScreenWidth();
		final height = Raylib.GetScreenHeight();
		Raylib.ClearBackground(Color.rgba(12, 28, 36));
		final outer = Rectangle.fromFloat(16.0, 16.0, width - 32.0, height - 32.0);
		if (Raygui.WindowBox(outer, UiCatalog.text(locale, UiMessage.EditorTitle)).has(GuiResult.Pressed))
			return ReturnToTitle;

		final toolbarTop = 56.0;
		final buttonWidth = 116.0;
		final buttonGap = 10.0;
		var buttonLeft = 32.0;
		if (button(buttonLeft, toolbarTop, buttonWidth, UiCatalog.text(locale, UiMessage.EditorNewWorld))) {
			session = openNewWorld();
			notice = Ready;
			final name = worldName;
			if (name != null)
				name.clear();
			refreshProjection(true);
		}
		buttonLeft += buttonWidth + buttonGap;
		if (button(buttonLeft, toolbarTop, buttonWidth, UiCatalog.text(locale, UiMessage.EditorUndo)))
			undo();
		buttonLeft += buttonWidth + buttonGap;
		if (button(buttonLeft, toolbarTop, buttonWidth, UiCatalog.text(locale, UiMessage.EditorRedo)))
			redo();
		buttonLeft += buttonWidth + buttonGap;
		if (button(buttonLeft, toolbarTop, buttonWidth, UiCatalog.text(locale, UiMessage.EditorValidate)))
			validate();
		buttonLeft += buttonWidth + buttonGap;
		final testing = session != null && session.testPlay() != null;
		final testLabel = testing ? UiMessage.EditorStopTest : UiMessage.EditorTest;
		if (button(buttonLeft, toolbarTop, buttonWidth, UiCatalog.text(locale, testLabel)))
			toggleTestPlay();

		final viewportTop = 104.0;
		final sidebarWidth = 230;
		Raygui.Panel(Rectangle.fromFloat(32.0, viewportTop, width - sidebarWidth - 80.0, height - viewportTop - 70.0),
			UiCatalog.text(locale, UiMessage.EditorCanvasHelp));
		Raygui.Panel(Rectangle.fromFloat(width - sidebarWidth - 32.0, viewportTop, sidebarWidth, height - viewportTop - 70.0),
			UiCatalog.text(locale, UiMessage.EditorReady));
		toolList.draw(Rectangle.fromFloat(width - sidebarWidth - 16.0, viewportTop + 44.0, sidebarWidth - 32.0, 116.0),
			UiCatalog.text(locale, UiMessage.EditorToolList));
		Raygui.Toggle(Rectangle.fromFloat(width - sidebarWidth - 16.0, viewportTop + 172.0, sidebarWidth - 32.0, 32.0),
			UiCatalog.text(locale, UiMessage.EditorAdvanced), advancedTools);
		Raygui.Label(Rectangle.fromFloat(width - sidebarWidth - 16.0, viewportTop + 216.0, sidebarWidth - 32.0, 24.0),
			UiCatalog.text(locale, UiMessage.EditorName));
		final name = worldName;
		if (name != null)
			name.draw(Rectangle.fromFloat(width - sidebarWidth - 16.0, viewportTop + 242.0, sidebarWidth - 32.0, 32.0));
		drawWorldViewport(48, 144, width - sidebarWidth - 112, height - 230);

		final status = switch notice {
			case Ready: UiMessage.EditorReady;
			case Valid: UiMessage.EditorValid;
			case Invalid: UiMessage.EditorInvalid;
			case Testing: UiMessage.EditorTesting;
		};
		Raygui.StatusBar(Rectangle.fromFloat(32.0, height - 54.0, width - 190.0, 28.0), UiCatalog.text(locale, status));
		if (button(width - 142.0, height - 54.0, 110.0, UiCatalog.text(locale, UiMessage.EditorBack)))
			return ReturnToTitle;
		return StayInEditor;
	}

	static inline function button(x:Float, y:Float, width:Float, text:c.CString):Bool
		return Raygui.Button(Rectangle.fromFloat(x, y, width, 32.0), text).has(GuiResult.Pressed);

	function undo():Void {
		final current = session;
		if (current == null)
			return;
		switch current.mutate({baseRevision: current.revision(), mutation: Undo}) {
			case MutationApplied(_, _, _, _):
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
			case MutationApplied(_, _, _, _):
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
					case MutationApplied(_, _, _, _):
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

	#if caxecraft_pilot
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
	static function openNewWorld():Null<EditorSession> {
		final draft = createBlankEditorScenario(new ScenarioId("editor.new-world"), new LogicalPath("packs/caxecraft/base"),
			ScenarioText.Literal("Untitled world"), ScenarioMode.Creative, BaseContentPack.blockId(BaseBlock.Air), new ScenarioId("player.spawn"), {
				width: 12,
				height: 1,
				depth: 12
			}, [{code: 1, blockType: BaseContentPack.blockId(BaseBlock.Grass)}]);
		return switch EditorSession.open(draft, new BaseContentRegistry()) {
			case EditorOpened(value): value;
			case EditorOpenRejected(_): null;
		};
	}
}
#end

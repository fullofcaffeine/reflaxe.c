package caxecraft.app;

#if c
import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.BaseBlock;
import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.editor.EditorScenarioFactory.createBlank as createBlankEditorScenario;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorTypes.EditorEditResult;
import caxecraft.editor.EditorTypes.EditorHistoryResult;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorTestPlayResult;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.editor.EditorViewport.EditorTool;
import caxecraft.editor.EditorViewport.EditorToolCommandResult;
import caxecraft.editor.EditorViewport.EditorViewportProjection;
import caxecraft.editor.EditorViewport.commandFor as commandForTool;
import caxecraft.editor.EditorViewport.layout as layoutViewport;
import caxecraft.editor.EditorViewport.paletteCodeAt;
import caxecraft.editor.EditorViewport.pointAt as viewportPointAt;
import caxecraft.editor.EditorViewport.project as projectViewport;
import caxecraft.editor.EditorViewport.toolFromIndex;
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
import raygui.GuiToggleState;
import raygui.Raygui;
import raylib.Color;
import raylib.Raylib;
import raylib.Rectangle;

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
 * play. A cached `EditorViewportProjection` reads one terrain layer after a
 * successful edit; steady frames draw that cache instead of serializing the
 * draft or maintaining a second editable world.
 *
 * The base-pack IDs and Raylib colors below belong at this Caxecraft
 * composition edge; the reusable editor package knows neither. Native
 * save/load and multi-layer navigation remain planned separately.
 */
final class CaxecraftEditorScreen {
	var session:Null<EditorSession>;
	var notice:EditorNotice;
	var projection:Null<EditorViewportProjection>;
	var selection:Null<VoxelBounds>;
	final advancedTools:GuiToggleState;
	final toolList:GuiListViewState;

	public function new() {
		session = openNewWorld();
		notice = Ready;
		projection = null;
		selection = null;
		advancedTools = new GuiToggleState(false);
		toolList = new GuiListViewState();
		refreshProjection();
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
			refreshProjection();
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
		drawViewport(48, 144, width - sidebarWidth - 112, height - 230);

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
		if (session == null)
			return;
		switch session.undo() {
			case HistoryApplied(_, _, _):
				notice = Ready;
				refreshProjection();
			case HistoryRejected(_):
				notice = Invalid;
		}
	}

	function redo():Void {
		if (session == null)
			return;
		switch session.redo() {
			case HistoryApplied(_, _, _):
				notice = Ready;
				refreshProjection();
			case HistoryRejected(_):
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
	 * Draw and operate one cached top-down terrain layer.
	 *
	 * The pointer mapper and command translator are renderer-independent. This
	 * method only reads native mouse state, paints pixels, and submits the
	 * resulting typed command through `applyToolAt`.
	 */
	function drawViewport(left:Int, top:Int, width:Int, height:Int):Void {
		var current = projection;
		if (current == null)
			return;
		var grid = layoutViewport(left, top, width, height, current);
		if (grid == null)
			return;

		final mouse = Raylib.GetMousePosition();
		var hover = viewportPointAt(current, grid, Std.int(mouse.x.toFloat()), Std.int(mouse.y.toFloat()));
		if (hover != null && Raylib.IsMouseButtonPressed(raylib.MouseButton.Left)) {
			final tool = toolFromIndex(toolList.activeIndex());
			if (tool == null)
				notice = Invalid;
			else
				applyToolAt(tool, hover);
			current = projection;
			if (current == null)
				return;
			grid = layoutViewport(left, top, width, height, current);
			if (grid == null)
				return;
			hover = viewportPointAt(current, grid, Std.int(mouse.x.toFloat()), Std.int(mouse.y.toFloat()));
		}

		final inset = grid.cellSize > 2 ? 1 : 0;
		final fillSize = grid.cellSize - inset * 2;
		for (z in 0...current.depth)
			for (x in 0...current.width) {
				final cellLeft = grid.left + x * grid.cellSize;
				final cellTop = grid.top + z * grid.cellSize;
				final code = paletteCodeAt(current, x, z);
				final color = if (code == 0) {
					if ((x + z) % 2 == 0)
						Color.rgba(29, 47, 55)
					else
						Color.rgba(35, 55, 63);
				} else if (code == 1) {
					Color.rgba(83, 145, 92);
				} else {
					Color.rgba(91, 107, 117);
				};
				Raylib.DrawRectangle(cellLeft + inset, cellTop + inset, fillSize, fillSize, color);
				Raylib.DrawRectangleLines(cellLeft, cellTop, grid.cellSize, grid.cellSize, Color.rgba(49, 72, 79));
				if (selectedCell(x, current.layerY, z))
					Raylib.DrawRectangleLines(cellLeft + 2, cellTop + 2, grid.cellSize - 4, grid.cellSize - 4, CaxecraftPalette.selection());
			}
		if (hover != null)
			Raylib.DrawRectangleLines(grid.left + hover.x * grid.cellSize, grid.top + hover.z * grid.cellSize, grid.cellSize, grid.cellSize,
				Color.rgba(109, 223, 232));
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
				switch current.apply(value) {
					case EditApplied(_, _, _):
						notice = Ready;
						refreshProjection();
						true;
					case EditUnchanged(_):
						notice = Ready;
						true;
					case EditRejected(_):
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
	function refreshProjection():Void {
		final current = session;
		if (current == null) {
			projection = null;
			selection = null;
			notice = Invalid;
			return;
		}
		final draft = current.draftSnapshot();
		projection = projectViewport(draft.world, 0);
		selection = current.selectedBounds();
		if (projection == null)
			notice = Invalid;
	}

	#if caxecraft_pilot
	/**
	 * Submit one deterministic pilot gesture through the production tool path.
	 *
	 * The graphical runner bypasses only operating-system pointer delivery. It
	 * still exercises command translation, `EditorSession.apply`, history, cache
	 * refresh, and the real renderer. Ordinary builds omit this method.
	 */
	public function applyPilotTool(tool:EditorTool, point:VoxelPoint):Bool
		return applyToolAt(tool, point);
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

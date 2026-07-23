package caxecraft.app;

#if c
import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.editor.EditorScenarioFactory;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorTypes.EditorHistoryResult;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorTestPlayResult;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.localization.UiCatalog;
import caxecraft.localization.UiCatalog.LocaleCursor;
import caxecraft.localization.UiCatalog.UiMessage;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioText;
import raygui.GuiResult;
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
 * First native visual shell over the real renderer-independent editor session.
 *
 * This stateful class owns one mutable draft/session and small presentation
 * state. Raygui remains immediate-mode: every frame redraws controls, while
 * `EditorSession` continues to own validation, undo/redo, and disposable test
 * play. The base-pack IDs below belong at this Caxecraft composition edge; the
 * reusable editor package does not know them.
 *
 * This slice intentionally stops before block painting and native save/load.
 * Its viewport label makes that limit visible rather than presenting a fake
 * editor. The next vertical slice connects the existing typed editor commands
 * to mouse tools in this same screen.
 */
final class CaxecraftEditorScreen {
	var session:Null<EditorSession>;
	var notice:EditorNotice;

	public function new() {
		session = openNewWorld();
		notice = Ready;
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
		final sidebarWidth = 230.0;
		Raygui.Panel(Rectangle.fromFloat(32.0, viewportTop, width - sidebarWidth - 80.0, height - viewportTop - 70.0),
			UiCatalog.text(locale, UiMessage.EditorCanvasHelp));
		Raygui.Panel(Rectangle.fromFloat(width - sidebarWidth - 32.0, viewportTop, sidebarWidth, height - viewportTop - 70.0),
			UiCatalog.text(locale, UiMessage.EditorReady));

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

	/** Create the built-in blank draft without teaching the generic editor a pack ID. */
	static function openNewWorld():Null<EditorSession> {
		final draft = EditorScenarioFactory.create(new ScenarioId("editor.new-world"), new LogicalPath("packs/caxecraft/base"),
			ScenarioText.Literal("Untitled world"), ScenarioMode.Creative, new ContentId("caxecraft:air"), new ScenarioId("player.spawn"));
		return switch EditorSession.open(draft, new BaseContentRegistry()) {
			case EditorOpened(value): value;
			case EditorOpenRejected(_): null;
		};
	}
}
#end

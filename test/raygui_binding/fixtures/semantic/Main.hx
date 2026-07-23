import raygui.GuiControl;
import raygui.GuiControlProperty;
import raygui.GuiDefaultProperty;
import raygui.GuiResult;
import raygui.GuiState;
import raygui.Raygui;
import raylib.Rectangle;

class Main {
	static function main():Void {
		Raygui.LoadStyleDefault();
		Raygui.SetState(GuiState.Normal);
		Raygui.SetControlStyle(GuiControl.Button, GuiControlProperty.BorderWidth, 2);
		Raygui.SetDefaultStyle(GuiDefaultProperty.TextSize, 18);
		final panel = Rectangle.fromFloat(8.0, 8.0, 240.0, 120.0);
		final button = Rectangle.fromFloat(24.0, 72.0, 120.0, 32.0);
		Raygui.Panel(panel, "Editor");
		if (Raygui.Button(button, "Create world").has(GuiResult.Pressed)) {
			Raygui.StatusBar(Rectangle.fromFloat(8.0, 136.0, 240.0, 24.0), "Ready");
		}
	}
}

import raygui.GuiControl;
import raygui.GuiControlProperty;
import raygui.GuiDefaultProperty;
import raygui.GuiListViewState;
import raygui.GuiResult;
import raygui.GuiState;
import raygui.GuiTextBoxState;
import raygui.GuiToggleState;
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
		final advanced = new GuiToggleState(false);
		final tools = new GuiListViewState();
		final maybeName = GuiTextBoxState.create(64, "Bosque nevado");
		if (maybeName == null)
			while (true) {}
		final name:GuiTextBoxState = maybeName;
		Raygui.Panel(panel, "Editor");
		Raygui.Toggle(Rectangle.fromFloat(24.0, 32.0, 120.0, 32.0), "Advanced", advanced);
		tools.draw(Rectangle.fromFloat(152.0, 32.0, 80.0, 72.0), "Paint;Erase;Fill");
		name.draw(Rectangle.fromFloat(24.0, 112.0, 208.0, 32.0));
		if (name.text() != "Bosque nevado")
			while (true) {}
		if (Raygui.Button(button, "Create world").has(GuiResult.Pressed)) {
			Raygui.StatusBar(Rectangle.fromFloat(8.0, 136.0, 240.0, 24.0), "Ready");
		}
	}
}

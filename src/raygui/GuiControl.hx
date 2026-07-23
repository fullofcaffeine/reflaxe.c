package raygui;

#if c
/**
 * Closed control families understood by raygui's shared style table.
 *
 * This is a value, not a live widget. Raygui is an immediate-mode interface:
 * the game asks for each visible control again every frame, and raygui returns
 * what happened during that frame.
 */
enum abstract GuiControl(Int) {
	var Default = 0;
	var Label = 1;
	var Button = 2;
	var Toggle = 3;
	var Slider = 4;
	var ProgressBar = 5;
	var CheckBox = 6;
	var ComboBox = 7;
	var DropdownBox = 8;
	var TextBox = 9;
	var ValueBox = 10;
	var TabBar = 11;
	var ListView = 12;
	var ColorPicker = 13;
	var ScrollBar = 14;
	var StatusBar = 15;
}
#end

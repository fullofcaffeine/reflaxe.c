package raygui;

#if c
/** Style properties shared by every raygui control family. */
enum abstract GuiControlProperty(Int) {
	var BorderColorNormal = 0;
	var BaseColorNormal = 1;
	var TextColorNormal = 2;
	var BorderColorFocused = 3;
	var BaseColorFocused = 4;
	var TextColorFocused = 5;
	var BorderColorPressed = 6;
	var BaseColorPressed = 7;
	var TextColorPressed = 8;
	var BorderColorDisabled = 9;
	var BaseColorDisabled = 10;
	var TextColorDisabled = 11;
	var BorderWidth = 12;
	var TextPadding = 13;
	var TextAlignment = 14;
}
#end

package raygui;

#if c
/** The four interaction states used when raygui chooses control styling. */
enum abstract GuiState(Int) {
	var Normal = 0;
	var Focused = 1;
	var Pressed = 2;
	var Disabled = 3;

	@:allow(raygui.Raygui)
	private static inline function fromRaw(value:Int):GuiState
		return new GuiState(value);

	private inline function new(value:Int)
		this = value;
}
#end

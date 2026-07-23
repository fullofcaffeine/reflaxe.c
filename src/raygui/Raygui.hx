package raygui;

// This facade is C-only for the same reason as RaylibHx: it describes a C ABI,
// not a portable game-UI contract. Caxecraft's later UI adapter owns the
// target-neutral concepts; there is no run-time target branch in generated C.

#if c
/**
 * Allocation-free Haxe facade for the reviewed, by-value raygui core.
 *
 * The static namespace is intentional here: raygui itself has process-wide
 * immediate-mode state and exposes C functions, so inventing Haxe widget
 * instances would imply ownership and lifetimes that do not exist. Each inline
 * method becomes the corresponding C call. Pointer-mutating controls such as
 * text boxes remain absent until their buffer capacity and borrow rules have a
 * typed owner.
 */
class Raygui {
	public static inline function Enable():Void
		raygui.raw.Raygui.GuiEnable();

	public static inline function Disable():Void
		raygui.raw.Raygui.GuiDisable();

	public static inline function Lock():Void
		raygui.raw.Raygui.GuiLock();

	public static inline function Unlock():Void
		raygui.raw.Raygui.GuiUnlock();

	public static inline function IsLocked():Bool
		return raygui.raw.Raygui.GuiIsLocked();

	public static inline function SetState(state:GuiState):Void
		raygui.raw.Raygui.GuiSetState(c.IntConvert.exact(state));

	public static inline function GetState():GuiState
		return GuiState.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiGetState()));

	public static inline function SetControlStyle(control:GuiControl, property:GuiControlProperty, value:Int):Void
		raygui.raw.Raygui.GuiSetStyle(c.IntConvert.exact(control), c.IntConvert.exact(property), c.IntConvert.exact(value));

	public static inline function GetControlStyle(control:GuiControl, property:GuiControlProperty):Int
		return c.IntConvert.exact(raygui.raw.Raygui.GuiGetStyle(c.IntConvert.exact(control), c.IntConvert.exact(property)));

	public static inline function SetDefaultStyle(property:GuiDefaultProperty, value:Int):Void
		raygui.raw.Raygui.GuiSetStyle(c.IntConvert.exact(GuiControl.Default), c.IntConvert.exact(property), c.IntConvert.exact(value));

	public static inline function GetDefaultStyle(property:GuiDefaultProperty):Int
		return c.IntConvert.exact(raygui.raw.Raygui.GuiGetStyle(c.IntConvert.exact(GuiControl.Default), c.IntConvert.exact(property)));

	/** Restores raygui's built-in style without loading a file or resource. */
	public static inline function LoadStyleDefault():Void
		raygui.raw.Raygui.GuiLoadStyleDefault();

	/** Sets global opacity, explicitly rounding Haxe's Float to C binary32. */
	public static inline function SetAlpha(alpha:Float):Void
		raygui.raw.Raygui.GuiSetAlpha(c.Float32.fromFloat(alpha));

	public static inline function GetTextWidth(text:c.CString):Int
		return c.IntConvert.exact(raygui.raw.Raygui.GuiGetTextWidth(text));

	public static inline function WindowBox(bounds:raylib.Rectangle, title:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiWindowBox(bounds, title)));

	public static inline function GroupBox(bounds:raylib.Rectangle, text:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiGroupBox(bounds, text)));

	public static inline function Line(bounds:raylib.Rectangle, text:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiLine(bounds, text)));

	public static inline function Panel(bounds:raylib.Rectangle, text:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiPanel(bounds, text)));

	public static inline function Label(bounds:raylib.Rectangle, text:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiLabel(bounds, text)));

	public static inline function Button(bounds:raylib.Rectangle, text:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiButton(bounds, text)));

	public static inline function LabelButton(bounds:raylib.Rectangle, text:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiLabelButton(bounds, text)));

	public static inline function Dummy(bounds:raylib.Rectangle, text:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiDummyRec(bounds, text)));

	public static inline function StatusBar(bounds:raylib.Rectangle, text:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiStatusBar(bounds, text)));
}
#end

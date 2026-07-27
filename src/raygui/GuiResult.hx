package raygui;

#if c
/**
 * Allocation-free result flags returned by raygui controls.
 *
 * raygui uses small integer bits instead of allocating an event object on
 * every frame. The abstract keeps that efficient representation in generated
 * C while giving Haxe code named values and a readable `has(...)` query.
 * Ordinary application code cannot construct an arbitrary result integer.
 * Only the two reviewed ABI adapters named by `@:allow` below can convert a
 * foreign return value. This narrow compile-time friendship keeps the raw
 * constructor sealed without exposing mutable application state or granting
 * any run-time ownership.
 */
enum abstract GuiResult(Int) to Int {
	var None = 0;
	var Pressed = 1;
	var Changed = 2;
	var TabClose = 4;

	/** Reports whether this result contains the requested raygui result bit. */
	public inline function has(expected:GuiResult):Bool
		return (this & unwrap(expected)) != 0;

	@:allow(raygui.Raygui)
	@:allow(raygui.GuiListViewState)
	private static inline function fromRaw(value:Int):GuiResult
		return new GuiResult(value);

	private static inline function unwrap(value:GuiResult):Int
		return value;

	private inline function new(value:Int)
		this = value;
}
#end

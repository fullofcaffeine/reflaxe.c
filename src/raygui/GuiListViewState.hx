package raygui;

#if c
/** Direct list-view alias for one runtime-owned item string. */
@:c.include("raygui.h", c.IncludeKind.System)
private extern class GuiListViewRuntimeText {
	/** Draw without retaining the text or either mutable integer pointer. */
	@:c.name("GuiListView")
	public static function draw(bounds:raylib.raw.Rectangle, items:c.CStringRef, scrollIndex:c.Ref<c.Int32>, active:c.Ref<c.Int32>):c.Int32;
}

/**
 * Persistent selection and scrolling for one immediate-mode list.
 *
 * Raygui does not allocate or retain a list widget. This Haxe value keeps the
 * two integers that must survive between frames, then lends their addresses to
 * `GuiListView` only while `draw` is running. Application code sees ordinary
 * Haxe `Int` values; the exact-width C carriers stay inside this ABI adapter.
 *
 * A stateful class is useful here because selection has identity and a lifetime
 * across frames. A module function would force every caller to expose the two
 * mutable C integers itself.
 */
final class GuiListViewState {
	var scrollIndex:c.Int32;
	var active:c.Int32;

	/**
	 * Start a list at one selected item and scroll position.
	 *
	 * Raygui uses `-1` for “no active item.” It owns range handling relative to
	 * the semicolon-separated item text supplied to `draw`.
	 */
	public function new(activeIndex:Int = 0, firstVisibleIndex:Int = 0) {
		active = c.IntConvert.exact(activeIndex);
		scrollIndex = c.IntConvert.exact(firstVisibleIndex);
	}

	/** Return the currently selected zero-based item, or `-1` when none is selected. */
	public inline function activeIndex():Int
		return c.IntConvert.exact(active);

	/** Return the zero-based item currently at the top of the visible list. */
	public inline function firstVisibleIndex():Int
		return c.IntConvert.exact(scrollIndex);

	/**
	 * Move the active item without fabricating pointer or keyboard input.
	 *
	 * A screen uses this for semantic keyboard/controller navigation after it
	 * has chosen the visible list's exact item count. The selection wraps so a
	 * child can keep pressing one direction. Invalid counts and a zero movement
	 * leave state unchanged and report `false`.
	 */
	public function moveSelection(itemCount:Int, delta:Int):Bool {
		if (itemCount <= 0 || delta == 0)
			return false;
		var next = activeIndex() + delta;
		while (next < 0)
			next += itemCount;
		while (next >= itemCount)
			next -= itemCount;
		active = c.IntConvert.exact(next);
		return true;
	}

	/**
	 * Draw the list and update this state from mouse or keyboard input.
	 *
	 * `items` follows raygui's compact `item;item;item` spelling. Both pointers
	 * passed below are call-scoped borrows: raygui may replace the integers
	 * during this call, but it cannot keep their addresses afterwards.
	 */
	public inline function draw(bounds:raylib.Rectangle, items:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiListView(bounds, items, c.Ref.to(scrollIndex), c.Ref.to(active))));

	/** Draw runtime-owned list text while retaining this value's native state. */
	public inline function drawString(bounds:raylib.Rectangle, items:String):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(GuiListViewRuntimeText.draw(bounds, c.CStringRef.to(items), c.Ref.to(scrollIndex), c.Ref.to(active))));
}
#end

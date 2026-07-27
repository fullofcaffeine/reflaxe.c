package raygui;

#if c
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
	 * Draw the list and update this state from mouse or keyboard input.
	 *
	 * `items` follows raygui's compact `item;item;item` spelling. Both pointers
	 * passed below are call-scoped borrows: raygui may replace the integers
	 * during this call, but it cannot keep their addresses afterwards.
	 */
	public inline function draw(bounds:raylib.Rectangle, items:c.CString):GuiResult
		return GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiListView(bounds, items, c.Ref.to(scrollIndex), c.Ref.to(active))));
}
#end

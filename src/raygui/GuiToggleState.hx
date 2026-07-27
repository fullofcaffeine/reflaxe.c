package raygui;

#if c
/**
 * Caller-owned Boolean state for one immediate-mode toggle.
 *
 * Raygui redraws controls every frame rather than creating persistent widget
 * objects. This small class gives the value itself a clear lifetime across
 * those frames. `Raygui.Toggle` lends its `active` field to C only until the
 * native call returns; no allocation or hidden widget registry is involved.
 *
 * The field is intentionally writable because both application code and the
 * native control are legitimate owners of state changes. A read-only property
 * would falsely imply that only this class may mutate the value.
 */
final class GuiToggleState {
	/** Whether the associated toggle is currently selected. */
	public var active:Bool;

	/** Create persistent toggle state with the requested initial selection. */
	public function new(active:Bool)
		this.active = active;
}
#end

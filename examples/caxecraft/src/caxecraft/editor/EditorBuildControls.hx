package caxecraft.editor;

import caxecraft.editor.EditorViewport.EditorTool;

/**
 * Owns the small device-neutral interaction policy for direct 3D editing.
 *
 * The native screen still translates Raylib events at the application edge.
 * These functions decide only when Build owns pointer look and which of its six
 * visible creation cards a number key selects. Keeping that policy here lets
 * the Eval probe protect Escape, focus loss, and Plan transitions without
 * imitating an operating-system mouse.
 */
/** Whether the Build camera or the surrounding editor controls own the pointer. */
enum abstract EditorBuildPointerState(Int) {
	var Released = 0;
	var Captured = 1;
}

/**
 * Resolve one Build pointer transition from a normalized input edge.
 *
 * A focused click captures only while Build is active. Cancel, focus loss, or
 * switching to Plan always releases. The caller compares the old and new value
 * before changing the native cursor, so steady frames have no windowing effect.
 */
function nextPointerState(current:EditorBuildPointerState, buildActive:Bool, windowFocused:Bool, capturePressed:Bool,
		cancelPressed:Bool):EditorBuildPointerState {
	if (!buildActive || !windowFocused || cancelPressed)
		return Released;
	if (current == Released && capturePressed)
		return Captured;
	return current;
}

/**
 * Map the six visible creation cards to direct Build hotbar slots.
 *
 * Fill remains an advanced selection action rather than an invisible seventh
 * slot. Values outside the visible range return `null` and change no tool.
 */
function toolForHotbarSlot(slot:Int):Null<EditorTool> {
	return switch slot {
		case 1: SelectTool;
		case 2: PaintTool;
		case 3: EraseTool;
		case 4: CheckpointTool;
		case 5: CatalogObjectTool;
		case 6: TriggerZoneTool;
		case _: null;
	};
}

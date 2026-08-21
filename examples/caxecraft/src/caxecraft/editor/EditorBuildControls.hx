package caxecraft.editor;

import caxecraft.editor.EditorViewport.EditorTool;
import caxecraft.editor.EditorWorldViewport.EditorWorldHit;
import caxecraft.editor.EditorFocus.EditorFocusMove;
import caxecraft.editor.EditorFocus.EditorFocusTarget;
import caxecraft.editor.EditorFocus.moveFocus;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;

/**
 * Owns the small device-neutral interaction policy for direct 3D editing.
 *
 * The native screen still translates Raylib events at the application edge.
 * These functions decide when Build owns pointer look, what the number keys
 * select, which controls keyboard focus visits, and what a terrain mouse button
 * does. The native screen converts the result into an ordinary editor command.
 * This separation lets the Eval probe protect the input rules without
 * imitating an operating-system mouse.
 */
/** Whether the Build camera or the surrounding editor controls own the pointer. */
enum abstract EditorBuildPointerState(Int) {
	var Released = 0;
	var Captured = 1;
}

/** One direct terrain action, or no action when the target is not safe. */
enum EditorBuildTerrainAction {
	NoTerrainAction;
	RemoveTerrain(point:VoxelPoint);
	PlaceTerrain(point:VoxelPoint);
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
 * Convert the two terrain mouse buttons into one exact edit.
 *
 * The primary button removes the solid under the crosshair. The secondary
 * button places ground in the empty cell before that solid. Primary wins if
 * both buttons start in one frame. A missing target produces no edit.
 */
function terrainAction(primaryPressed:Bool, secondaryPressed:Bool, hit:Null<EditorWorldHit>):EditorBuildTerrainAction {
	if (hit == null)
		return NoTerrainAction;
	if (primaryPressed && hit.solid)
		return RemoveTerrain(hit.point);
	if (secondaryPressed && hit.placement != null)
		return PlaceTerrain(hit.placement);
	return NoTerrainAction;
}

/** True when Build uses direct remove and place controls for the selected tool. */
function usesDirectTerrainControls(tool:EditorTool):Bool {
	return switch tool {
		case SelectTool | PaintTool | EraseTool: true;
		case FillTool | CheckpointTool | CatalogObjectTool | TriggerZoneTool: false;
	};
}

/**
 * Map the five visible creation cards to direct Build hotbar slots.
 *
 * One terrain card owns both mouse buttons, so Build does not expose a separate
 * Erase mode. Plan keeps the precise Paint and Erase cards. Fill remains an
 * advanced selection action. Values outside the visible range return `null`.
 */
function toolForBuildHotbarSlot(slot:Int):Null<EditorTool> {
	return switch slot {
		case 1: SelectTool;
		case 2: PaintTool;
		case 3: CheckpointTool;
		case 4: CatalogObjectTool;
		case 5: TriggerZoneTool;
		case _: null;
	};
}

/** Replace Plan's hidden Erase mode with Build's direct terrain mode. */
function normalizeBuildTool(tool:EditorTool):EditorTool
	return tool == EraseTool ? PaintTool : tool;

/** Keep semantic focus on a control that the Build shelf shows. */
function normalizeBuildFocus(focus:EditorFocusTarget):EditorFocusTarget
	return focus == EraseTool ? GroundTool : focus;

/** Move through the shared focus order and skip Plan's hidden Erase card. */
function moveBuildFocus(current:EditorFocusTarget, direction:EditorFocusMove):EditorFocusTarget {
	final next = moveFocus(current, direction);
	return next == EraseTool ? moveFocus(next, direction) : next;
}

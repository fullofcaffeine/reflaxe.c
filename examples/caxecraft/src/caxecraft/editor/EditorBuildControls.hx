package caxecraft.editor;

import caxecraft.editor.EditorViewport.EditorTool;
import caxecraft.editor.EditorWorldViewport.EditorWorldHit;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmo;
import caxecraft.editor.EditorWorldViewport.absolute;
import caxecraft.editor.EditorFocus.EditorFocusMove;
import caxecraft.editor.EditorFocus.EditorFocusTarget;
import caxecraft.editor.EditorFocus.moveFocus;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioWorld.BlockPaletteEntry;

/**
 * Owns the small device-neutral interaction policy for direct 3D editing.
 *
 * The native screen still translates Raylib events at the application edge.
 * These functions decide when Build owns pointer look, what the number keys
 * select, which controls keyboard focus visits, and what direct terrain or
 * object input does. The native screen converts the result into an ordinary
 * editor command. This separation lets the Eval probe protect the input rules
 * without imitating an operating-system mouse or keyboard.
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

/** Player-visible meaning of the next captured Build gesture. */
enum EditorBuildPrompt {
	/** The crosshair has no editable world target. */
	NoBuildTarget;

	/** Ground mode can remove, place, or do both at the current target. */
	TerrainBuildPrompt(canRemove:Bool, canPlace:Bool);

	/** Select mode points at terrain or at one authored object. */
	SelectBuildPrompt(objectTarget:Bool);

	/** One creation tool has a target, which can still fail its typed preview. */
	CreateBuildPrompt(allowed:Bool);

	/** A held object can or cannot move to the current crosshair target. */
	MoveBuildPrompt(allowed:Bool);
}

/** Copy-free frame facts needed to explain the next Build input. */
typedef EditorBuildPromptInput = {
	final tool:EditorTool;
	final targetAvailable:Bool;
	final targetSolid:Bool;
	final placementAvailable:Bool;
	final objectTarget:Bool;
	final objectHeld:Bool;
	final heldPlacementAvailable:Bool;
	final previewAllowed:Bool;
}

/**
 * Explain the next direct gesture without inspecting Raylib or editor bytes.
 *
 * This is a view of state the native screen already computed for command
 * preview. Keeping the policy here makes the prompt agree with the actual
 * terrain, selection, creation, and held-object branches without adding a
 * second mutation decision.
 */
function buildPrompt(input:EditorBuildPromptInput):EditorBuildPrompt {
	if (input.objectHeld)
		return MoveBuildPrompt(input.heldPlacementAvailable);
	return switch input.tool {
		case PaintTool | EraseTool:
			if (!input.targetAvailable) NoBuildTarget; else {
				final canRemove = input.targetSolid;
				final canPlace = input.placementAvailable;
				if (!canRemove && !canPlace)
					NoBuildTarget;
				else
					TerrainBuildPrompt(canRemove, canPlace);
			}
		case SelectTool: input.objectTarget || input.targetAvailable ? SelectBuildPrompt(input.objectTarget) : NoBuildTarget;
		case FillTool | CheckpointTool | CatalogObjectTool | TriggerZoneTool:
			input.targetAvailable ? CreateBuildPrompt(input.previewAllowed) : NoBuildTarget;
	};
}

/** One selected-object edit, or no edit when Build does not own the gesture. */
enum EditorBuildObjectAction {
	NoObjectAction;
	NudgeSelectedObject(delta:VoxelPoint);
	TurnSelectedObject(degrees:Int);
}

/** One recoverable object shortcut, or no action when another UI owns input. */
enum EditorObjectShortcutAction {
	NoObjectShortcut;
	DuplicateSelectedObject;
	DeleteSelectedObject;
}

/** One device-independent shortcut frame for the shared object selection. */
typedef EditorObjectShortcutInput = {
	final inputAvailable:Bool;
	final buildActive:Bool;
	final pointerCaptured:Bool;
	final selectToolActive:Bool;
	final objectSelected:Bool;
	final objectHeld:Bool;
	final duplicatePressed:Bool;
	final deletePressed:Bool;
}

/**
 * Admit one copy or delete key edge only when the active editor view owns it.
 *
 * Plan accepts precise selection shortcuts without pointer capture. Build
 * requires captured Select mode and rejects shortcuts during a hold. Duplicate
 * wins if both keys start together, so one frame creates at most one revision.
 */
function objectShortcutAction(input:EditorObjectShortcutInput):EditorObjectShortcutAction {
	if (!input.inputAvailable || !input.objectSelected)
		return NoObjectShortcut;
	if (input.buildActive && (!input.pointerCaptured || !input.selectToolActive || input.objectHeld))
		return NoObjectShortcut;
	if (input.duplicatePressed)
		return DuplicateSelectedObject;
	if (input.deletePressed)
		return DeleteSelectedObject;
	return NoObjectShortcut;
}

/** Temporary Build ownership of one stable authored object. */
enum EditorBuildObjectGrab {
	NoObjectGrab;
	HoldingObject(id:ScenarioId);
}

/**
 * Choose the stable object that one Grab key edge owns.
 *
 * A visible crosshair object wins only when Grab starts. Steady frames retain
 * the selected object, so aiming across another marker cannot transfer an
 * active hold or change selection.
 */
function objectGrabCandidate(selected:Null<ScenarioId>, hovered:Null<ScenarioId>, grabPressed:Bool):Null<ScenarioId>
	return grabPressed && hovered != null ? hovered : selected;

/**
 * Start, retain, or cancel one selected-object grab without editing the draft.
 *
 * The stable ID must remain selected and Build must retain captured Select
 * input. Pressing Grab a second time cancels. This value owns no transform, so
 * aiming cannot create revisions or stale document copies.
 */
function nextObjectGrab(current:EditorBuildObjectGrab, eligible:Null<ScenarioId>, grabPressed:Bool, cancelPressed:Bool):EditorBuildObjectGrab {
	if (cancelPressed || eligible == null)
		return NoObjectGrab;
	return switch current {
		case NoObjectGrab: grabPressed ? HoldingObject(eligible) : NoObjectGrab;
		case HoldingObject(id):
			if (id.text() != eligible.text() || grabPressed) NoObjectGrab; else current;
	};
}

/** True while Build owns one stable object for crosshair placement. */
function objectGrabActive(current:EditorBuildObjectGrab):Bool
	return switch current {
		case NoObjectGrab: false;
		case HoldingObject(_): true;
	};

/** Translate one cached object origin to the chosen world cell. */
function objectPlacementDelta(gizmo:EditorObjectGizmo, target:VoxelPoint):VoxelPoint
	return {
		x: target.x - gizmo.origin.x,
		y: target.y - gizmo.origin.y,
		z: target.z - gizmo.origin.z
	};

/** One frame of device-independent input for a selected object in Build. */
typedef EditorBuildObjectInput = {
	final pointerCaptured:Bool;
	final selectToolActive:Bool;
	final objectSelected:Bool;
	final objectCanTurn:Bool;
	final upPressed:Bool;
	final rightPressed:Bool;
	final downPressed:Bool;
	final leftPressed:Bool;
	final turnPressed:Bool;
	final lookX:Float;
	final lookZ:Float;
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

/** True when direct Build input replaces the desktop editor controls. */
function immersiveWorkspaceActive(buildActive:Bool, pointerCaptured:Bool):Bool
	return buildActive && pointerCaptured;

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

/**
 * Convert arrow or turn key edges into at most one selected-object edit.
 *
 * Arrow movement follows the nearest horizontal camera axis. This keeps each
 * edit on the voxel grid while Up still means away from the creator. Ties use
 * the Z axis, which gives the default camera a stable forward direction. A
 * vertical or malformed look vector cannot move an object. Arrow priority is
 * Up, Right, Down, then Left; a simultaneous turn waits for another key edge.
 */
function objectAction(input:EditorBuildObjectInput):EditorBuildObjectAction {
	if (!input.pointerCaptured || !input.selectToolActive || !input.objectSelected)
		return NoObjectAction;
	final horizontalMagnitude = absolute(input.lookX) + absolute(input.lookZ);
	if (horizontalMagnitude > 0.000001 && (input.upPressed || input.rightPressed || input.downPressed || input.leftPressed)) {
		var forwardX = 0;
		var forwardZ = 0;
		if (absolute(input.lookX) > absolute(input.lookZ))
			forwardX = input.lookX >= 0.0 ? 1 : -1;
		else
			forwardZ = input.lookZ >= 0.0 ? 1 : -1;
		if (input.upPressed)
			return NudgeSelectedObject({x: forwardX, y: 0, z: forwardZ});
		if (input.rightPressed)
			return NudgeSelectedObject({x: -forwardZ, y: 0, z: forwardX});
		if (input.downPressed)
			return NudgeSelectedObject({x: -forwardX, y: 0, z: -forwardZ});
		return NudgeSelectedObject({x: forwardZ, y: 0, z: -forwardX});
	}
	if (input.turnPressed && input.objectCanTurn)
		return TurnSelectedObject(90);
	return NoObjectAction;
}

/** True when Build uses direct remove and place controls for the Ground tool. */
function usesDirectTerrainControls(tool:EditorTool):Bool {
	return switch tool {
		case PaintTool | EraseTool: true;
		case SelectTool | FillTool | CheckpointTool | CatalogObjectTool | TriggerZoneTool: false;
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

/** Move by one visible Build hotbar slot and wrap at either end. */
function cycleBuildHotbarTool(tool:EditorTool, direction:Int):EditorTool {
	if (direction == 0)
		return normalizeBuildTool(tool);
	var slot = switch tool {
		case SelectTool: 1;
		case PaintTool | EraseTool | FillTool: 2;
		case CheckpointTool: 3;
		case CatalogObjectTool: 4;
		case TriggerZoneTool: 5;
	};
	slot += direction > 0 ? 1 : -1;
	if (slot < 1)
		slot = 5;
	else if (slot > 5)
		slot = 1;
	final next = toolForBuildHotbarSlot(slot);
	return next == null ? normalizeBuildTool(tool) : next;
}

/**
 * Keep one terrain brush inside the current map's non-air palette.
 *
 * A map owns its compact palette codes. The requested code wins when that map
 * admits it. Otherwise the default code wins, followed by the first non-air
 * entry. `-1` reports that the map has no material that Build can place.
 */
function normalizeBuildPaletteCode(palette:Array<BlockPaletteEntry>, requested:Int, defaultCode:Int):Int {
	if (containsBuildPaletteCode(palette, requested))
		return requested;
	if (containsBuildPaletteCode(palette, defaultCode))
		return defaultCode;
	for (entry in palette)
		if (entry.code != 0)
			return entry.code;
	return -1;
}

/** Move through the map's non-air terrain materials and wrap at each end. */
function cycleBuildPaletteCode(palette:Array<BlockPaletteEntry>, current:Int, defaultCode:Int, direction:Int):Int {
	final normalized = normalizeBuildPaletteCode(palette, current, defaultCode);
	if (normalized < 0 || direction == 0)
		return normalized;
	var currentIndex = -1;
	var materialCount = 0;
	for (entry in palette) {
		if (entry.code != 0) {
			if (entry.code == normalized)
				currentIndex = materialCount;
			materialCount++;
		}
	}
	if (materialCount < 2)
		return normalized;
	var nextIndex = currentIndex + (direction > 0 ? 1 : -1);
	if (nextIndex < 0)
		nextIndex = materialCount - 1;
	else if (nextIndex >= materialCount)
		nextIndex = 0;
	var candidateIndex = 0;
	for (entry in palette) {
		if (entry.code != 0) {
			if (candidateIndex == nextIndex)
				return entry.code;
			candidateIndex++;
		}
	}
	return normalized;
}

/** Pick one aimed solid material, or retain the valid current brush. */
function pickBuildPaletteCode(palette:Array<BlockPaletteEntry>, current:Int, defaultCode:Int, targetSolid:Bool, targetCode:Int):Int {
	if (targetSolid && containsBuildPaletteCode(palette, targetCode))
		return targetCode;
	return normalizeBuildPaletteCode(palette, current, defaultCode);
}

/** True when one non-air compact code belongs to the current map. */
private function containsBuildPaletteCode(palette:Array<BlockPaletteEntry>, code:Int):Bool {
	if (code == 0)
		return false;
	for (entry in palette)
		if (entry.code == code)
			return true;
	return false;
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

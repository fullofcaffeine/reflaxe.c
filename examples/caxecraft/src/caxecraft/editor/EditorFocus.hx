package caxecraft.editor;

/**
	Defines device-neutral keyboard and controller focus for editor controls.

	Focus means “the control that receives the next semantic activation.” This
	module knows only stable control identities and their cyclic order; it knows
	nothing about Raylib keys, gamepad buttons, screen coordinates, or drawing.
	The native screen maps device input into `Forward`/`Backward` movement and
	draws the visible ring, while later controller input can reuse this exact
	order without inventing another editor-navigation model.
**/
enum abstract EditorFocusTarget(Int) {
	var Back = 0;
	var WorldName = 1;
	var Undo = 2;
	var Redo = 3;
	var Build = 4;
	var Plan = 5;
	var Play = 6;
	var SelectTool = 7;
	var GroundTool = 8;
	var EraseTool = 9;
	var MoreDetails = 10;
	var WorldList = 11;
	var KeepEditing = 12;
	var LeaveWithoutSaving = 13;
}

/**
	The two semantic directions through the closed editor focus order.

	A zero-allocation enum abstract fits because direction is only a named
	integer choice: it carries no payload, identity, or lifecycle. The generated
	C therefore passes one ordinary integer rather than constructing an enum
	value.
**/
enum abstract EditorFocusMove(Int) {
	var Forward = 0;
	var Backward = 1;
}

/** Start keyboard or controller navigation at the visible Back action. */
function initialFocus():EditorFocusTarget
	return Back;

/**
	Move exactly once through the editor controls, wrapping at either end.

	The exhaustive switch makes adding a new focus target deliberate: a
	contributor must place it in both directions rather than letting it become
	unreachable from one device.
**/
function moveFocus(current:EditorFocusTarget, direction:EditorFocusMove):EditorFocusTarget {
	return switch direction {
		case Forward:
			switch current {
				case Back: WorldName;
				case WorldName: Undo;
				case Undo: Redo;
				case Redo: Build;
				case Build: Plan;
				case Plan: Play;
				case Play: SelectTool;
				case SelectTool: GroundTool;
				case GroundTool: EraseTool;
				case EraseTool: MoreDetails;
				case MoreDetails: WorldList;
				case WorldList: Back;
				case KeepEditing: LeaveWithoutSaving;
				case LeaveWithoutSaving: KeepEditing;
			}
		case Backward:
			switch current {
				case Back: WorldList;
				case WorldName: Back;
				case Undo: WorldName;
				case Redo: Undo;
				case Build: Redo;
				case Plan: Build;
				case Play: Plan;
				case SelectTool: Play;
				case GroundTool: SelectTool;
				case EraseTool: GroundTool;
				case MoreDetails: EraseTool;
				case WorldList: MoreDetails;
				case KeepEditing: LeaveWithoutSaving;
				case LeaveWithoutSaving: KeepEditing;
			}
	};
}

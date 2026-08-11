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
	var NewWorld = 0;
	var Undo = 1;
	var Redo = 2;
	var Validate = 3;
	var TestPlay = 4;
	var ToolList = 5;
	var AdvancedTools = 6;
	var WorldName = 7;
	var SceneObjects = 8;
	var Back = 9;
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

/** Start keyboard or controller navigation at the first toolbar action. */
function initialFocus():EditorFocusTarget
	return NewWorld;

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
				case NewWorld: Undo;
				case Undo: Redo;
				case Redo: Validate;
				case Validate: TestPlay;
				case TestPlay: ToolList;
				case ToolList: AdvancedTools;
				case AdvancedTools: WorldName;
				case WorldName: SceneObjects;
				case SceneObjects: Back;
				case Back: NewWorld;
			}
		case Backward:
			switch current {
				case NewWorld: Back;
				case Undo: NewWorld;
				case Redo: Undo;
				case Validate: Redo;
				case TestPlay: Validate;
				case ToolList: TestPlay;
				case AdvancedTools: ToolList;
				case WorldName: AdvancedTools;
				case SceneObjects: WorldName;
				case Back: SceneObjects;
			}
	};
}

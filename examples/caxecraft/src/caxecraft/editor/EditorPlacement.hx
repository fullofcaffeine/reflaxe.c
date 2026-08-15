package caxecraft.editor;

import caxecraft.content.EditorObjectCatalog.EditorObjectRecipe;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipeKind;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;

/**
	Builds reloadable CAXEMAP objects from simple creator gestures.

	The visual editor supplies a snapped voxel and the current authored objects.
	This module owns stable editor IDs and exact scenario coordinates, so Plan,
	Build, and future device adapters cannot create subtly different records.
**/
/**
	Create one checkpoint command at the center of a snapped world cell.

	The first unused `editor.checkpoint.nN` identity is deterministic for the
	current draft. The returned command still passes through `EditorSession`,
	which owns validation, canonical bytes, history, undo, and redo.
**/
function checkpointCommand(point:VoxelPoint, objects:Array<ScenarioObject>):EditorCommand {
	return PutObject({
		id: nextCheckpointId(objects),
		tags: [],
		placement: Checkpoint({
			xMilli: point.x * 1000 + 500,
			yMilli: point.y * 1000,
			zMilli: point.z * 1000 + 500,
			yawDegrees: 0
		})
	});
}

/** Create one pack-defined object through the same canonical placement path. */
function objectRecipeCommand(recipe:EditorObjectRecipe, point:VoxelPoint, objects:Array<ScenarioObject>):EditorCommand {
	final id = nextRecipeId(recipe.id, objects);
	final transform:ScenarioTransform = {
		xMilli: point.x * 1000 + 500,
		yMilli: point.y * 1000,
		zMilli: point.z * 1000 + 500,
		yawDegrees: 0
	};
	return PutObject({
		id: id,
		tags: [],
		placement: switch recipe.kind {
			case EditorStatefulObject(objectType, initialState): StatefulObject(objectType, initialState, transform);
		}
	});
}

/** Find the first valid source-derived identity absent from the draft. */
private function nextRecipeId(recipeId:String, objects:Array<ScenarioObject>):ScenarioId {
	final prefix = 'editor.$recipeId.n';
	var number = 1;
	while (hasObjectId(objects, prefix + number))
		number++;
	return new ScenarioId(prefix + number);
}

/** Find the first positive editor checkpoint number not used by any object. */
private function nextCheckpointId(objects:Array<ScenarioObject>):ScenarioId {
	var number = 1;
	while (hasObjectId(objects, 'editor.checkpoint.n$number'))
		number++;
	return new ScenarioId('editor.checkpoint.n$number');
}

/** Compare stable IDs without depending on object order or placement role. */
private function hasObjectId(objects:Array<ScenarioObject>, expected:String):Bool {
	for (object in objects)
		if (object.id.text() == expected)
			return true;
	return false;
}

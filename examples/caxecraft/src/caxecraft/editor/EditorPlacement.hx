package caxecraft.editor;

import caxecraft.content.EditorObjectCatalog.EditorObjectRecipe;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipeKind;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;

/** One atomic checkpoint template and the object that the editor selects. */
typedef EditorCheckpointTemplate = {
	final objectId:ScenarioId;
	final commands:Array<EditorCommand>;
}

/**
	Builds reloadable CAXEMAP objects from simple creator gestures.

	The visual editor supplies a snapped voxel and current object or rule IDs.
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

/**
	Create a checkpoint marker and its playable interaction as one command list.

	The object and rule share one numeric suffix. The allocator skips a suffix if
	either identity already exists, so applying both commands cannot replace an
	authored record. `EditorSession` validates and commits the list atomically.
**/
function checkpointTemplate(point:VoxelPoint, objects:Array<ScenarioObject>, ruleIds:Array<ScenarioId>):EditorCheckpointTemplate {
	final number = nextCheckpointTemplateNumber(objects, ruleIds);
	final objectId = new ScenarioId('editor.checkpoint.n$number');
	return {
		objectId: objectId,
		commands: [
			PutObject({
				id: objectId,
				tags: [],
				placement: Checkpoint({
					xMilli: point.x * 1000 + 500,
					yMilli: point.y * 1000,
					zMilli: point.z * 1000 + 500,
					yawDegrees: 0
				})
			}),
			PutRule({
				id: new ScenarioId('editor.rule.checkpoint.n$number'),
				priority: 0,
				repeat: Repeat,
				event: Interact(objectId),
				predicate: Always,
				actions: [SetCheckpoint(objectId)]
			})
		]
	};
}

/** Create one one-cell trigger at the selected voxel through normal history. */
function triggerZoneCommand(point:VoxelPoint, objects:Array<ScenarioObject>):EditorCommand {
	return PutObject({
		id: nextTriggerId(objects),
		tags: [],
		placement: TriggerZone({
			origin: {x: point.x, y: point.y, z: point.z},
			size: {width: 1, height: 1, depth: 1}
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

/** Find one suffix that is free in both the object and rule namespaces. */
private function nextCheckpointTemplateNumber(objects:Array<ScenarioObject>, ruleIds:Array<ScenarioId>):Int {
	var number = 1;
	while (hasObjectId(objects, 'editor.checkpoint.n$number') || hasRuleId(ruleIds, 'editor.rule.checkpoint.n$number'))
		number++;
	return number;
}

/** Find the first positive editor trigger number not used by any object. */
private function nextTriggerId(objects:Array<ScenarioObject>):ScenarioId {
	var number = 1;
	while (hasObjectId(objects, 'editor.trigger.n$number'))
		number++;
	return new ScenarioId('editor.trigger.n$number');
}

/** Compare stable IDs without depending on object order or placement role. */
private function hasObjectId(objects:Array<ScenarioObject>, expected:String):Bool {
	for (object in objects)
		if (object.id.text() == expected)
			return true;
	return false;
}

/** Compare stable rule IDs without depending on canonical rule order. */
private function hasRuleId(ruleIds:Array<ScenarioId>, expected:String):Bool {
	for (id in ruleIds)
		if (id.text() == expected)
			return true;
	return false;
}

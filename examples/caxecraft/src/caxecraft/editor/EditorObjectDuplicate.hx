package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/**
	Copies one authored object without creating a second editor-only model.

	The helper reads canonical CAXEMAP objects and returns the ordinary
	`PutObject` command already owned by `EditorSession`. It deep-copies record
	and array containers so later edits cannot mutate the source through aliases.
**/
/** One fresh identity paired with the canonical command that creates it. */
typedef EditorObjectDuplicate = {
	final id:ScenarioId;
	final command:EditorCommand;
}

/**
	Copy one exact object in place, or return `null` when its ID is absent.

	Copy-in-place is deliberate: it cannot fail at a world edge and commits as
	one history entry. The editor selects the copy so existing movement controls
	can place it immediately.
**/
function duplicateObject(sourceId:ScenarioId, objects:Array<ScenarioObject>):Null<EditorObjectDuplicate> {
	final source = findObject(sourceId, objects);
	if (source == null)
		return null;
	final id = nextCopyId(source.id, objects);
	return {
		id: id,
		command: PutObject({
			id: id,
			tags: source.tags.copy(),
			placement: copyPlacement(source.placement)
		})
	};
}

/** Find one object by semantic ID without trusting a presentation index. */
private function findObject(expected:ScenarioId, objects:Array<ScenarioObject>):Null<ScenarioObject> {
	final text = expected.text();
	for (object in objects)
		if (object.id.text() == text)
			return object;
	return null;
}

/** Choose the first positive source-derived copy ID absent from the draft. */
private function nextCopyId(sourceId:ScenarioId, objects:Array<ScenarioObject>):ScenarioId {
	// A CAXEMAP ID segment must start with a lowercase letter. Keep `n` before
	// the counter so every generated identity remains valid after save/reload.
	final prefix = sourceId.text() + ".copy.n";
	var number = 1;
	while (hasObjectId(prefix + number, objects))
		number++;
	return new ScenarioId(prefix + number);
}

/** True when any placement role already owns one exact semantic identity. */
private function hasObjectId(expected:String, objects:Array<ScenarioObject>):Bool {
	for (object in objects)
		if (object.id.text() == expected)
			return true;
	return false;
}

/** Deep-copy every closed placement role while preserving its semantic links. */
private function copyPlacement(placement:ObjectPlacement):ObjectPlacement {
	return switch placement {
		case PlayerSpawn(transform): PlayerSpawn(copyTransform(transform));
		case Checkpoint(transform): Checkpoint(copyTransform(transform));
		case Item(itemType, quantity, transform): Item(itemType, quantity, copyTransform(transform));
		case Entity(entityType, transform): Entity(entityType, copyTransform(transform));
		case Npc(npcType, dialogue, transform): Npc(npcType, dialogue, copyTransform(transform));
		case Prefab(prefabType, transform): Prefab(prefabType, copyTransform(transform));
		case TriggerZone(bounds): TriggerZone(copyBounds(bounds));
		case StatefulObject(objectType, initialState, transform): StatefulObject(objectType, initialState, copyTransform(transform));
	};
}

/** Copy one millimeter transform into a separately owned record. */
private function copyTransform(value:ScenarioTransform):ScenarioTransform
	return {
		xMilli: value.xMilli,
		yMilli: value.yMilli,
		zMilli: value.zMilli,
		yawDegrees: value.yawDegrees
	};

/** Copy nested trigger records so resizing the copy cannot resize the source. */
private function copyBounds(value:VoxelBounds):VoxelBounds
	return {
		origin: {x: value.origin.x, y: value.origin.y, z: value.origin.z},
		size: {width: value.size.width, height: value.size.height, depth: value.size.depth}
	};

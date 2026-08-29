package caxecraft.editor;

import caxecraft.content.EditorObjectCatalog.EditorObjectRecipe;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipeKind;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/** One atomic checkpoint template and the object that the editor selects. */
typedef EditorCheckpointTemplate = {
	final objectId:ScenarioId;
	final commands:Array<EditorCommand>;
}

/** One asset-browser placement and every command needed to make it playable. */
typedef EditorObjectTemplate = {
	final objectId:ScenarioId;
	final commands:Array<EditorCommand>;
}

/** Current draft facts needed to expand one asset recipe without hidden globals. */
typedef EditorObjectTemplateContext = {
	final scenarioId:ScenarioId;
	final worldSize:VoxelSize;
	final objects:Array<ScenarioObject>;
	final dialogueIds:Array<ScenarioId>;
	final ruleIds:Array<ScenarioId>;
}

/** Exact outcome of expanding one validated asset-browser recipe. */
enum EditorObjectTemplateResult {
	ObjectTemplateReady(template:EditorObjectTemplate);
	ObjectTemplateRejected(error:EditorError);
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

/**
	Create one pack-defined object and its required content logic atomically.

	Items, enemies, and simple mechanisms need one placement command. NPC and
	linked-object recipes also create the CaxeFlow rules that make them immediately
	playable. Every multi-record recipe uses one free shared suffix, so a template
	cannot replace an authored object or rule.
**/
function objectRecipeTemplate(recipe:EditorObjectRecipe, point:VoxelPoint, context:EditorObjectTemplateContext):EditorObjectTemplateResult {
	final transform = transformAt(point);
	return switch recipe.kind {
		case EditorNpc(npcType):
			if (context.dialogueIds.length == 0) ObjectTemplateRejected(MissingEditorDialogue); else {
				final number = nextRecipeTemplateNumber(recipe.id, context.objects, context.ruleIds);
				final objectId = new ScenarioId('editor.${recipe.id}.n$number');
				ObjectTemplateReady({
					objectId: objectId,
					commands: [
						putRecipeObject(objectId, Npc(npcType, context.dialogueIds[0], transform)),
						PutRule({
							id: new ScenarioId('editor.rule.${recipe.id}.n$number'),
							priority: 0,
							repeat: Repeat,
							event: Interact(objectId),
							predicate: Always,
							actions: [ShowDialogue(context.dialogueIds[0])]
						})
					]
				});
			}
		case EditorItem(itemType, quantity):
			ObjectTemplateReady(singleObjectTemplate(nextRecipeId(recipe.id, context.objects), Item(itemType, quantity, transform)));
		case EditorEnemy(entityType):
			ObjectTemplateReady(singleObjectTemplate(nextRecipeId(recipe.id, context.objects), Entity(entityType, transform)));
		case EditorEnemyWave(entityType):
			final enemyPoints = neighboringPoints(point, context.worldSize, 3);
			if (enemyPoints == null) ObjectTemplateRejected(EditorEnemyWaveNeedsSpace); else {
				ObjectTemplateReady(enemyWaveTemplate(recipe.id, entityType, point, enemyPoints, context));
			}
		case EditorStatefulObject(objectType, initialState):
			ObjectTemplateReady(singleObjectTemplate(nextRecipeId(recipe.id, context.objects), StatefulObject(objectType, initialState, transform)));
		case EditorLinkedStatefulPair(source, target):
			final targetPoint = adjacentPoint(point, context.worldSize);
			if (targetPoint == null) ObjectTemplateRejected(EditorTemplateNeedsAdjacentCell); else {
				final number = nextLinkedRecipeTemplateNumber(recipe.id, context.objects, context.ruleIds);
				final sourceId = new ScenarioId('editor.${recipe.id}.source.n$number');
				final targetId = new ScenarioId('editor.${recipe.id}.target.n$number');
				ObjectTemplateReady({
					objectId: sourceId,
					commands: [
						putRecipeObject(sourceId, StatefulObject(source.objectType, source.initialState, transform)),
						putRecipeObject(targetId, StatefulObject(target.objectType, target.initialState, transformAt(targetPoint))),
						PutRule({
							id: new ScenarioId('editor.rule.${recipe.id}.n$number'),
							priority: 0,
							repeat: Repeat,
							event: Interact(sourceId),
							predicate: All([
								ObjectStateIs(sourceId, source.initialState),
								ObjectStateIs(targetId, target.initialState)
							]),
							actions: [
								SetObjectState(sourceId, source.activeState),
								SetObjectState(targetId, target.activeState)
							]
						})
					]
				});
			}
	};
}

/** Build one hidden enemy group and the two rules that reveal it after entry. */
private function enemyWaveTemplate(recipeId:String, entityType:caxecraft.scenario.ContentId, triggerPoint:VoxelPoint, enemyPoints:Array<VoxelPoint>,
		context:EditorObjectTemplateContext):EditorObjectTemplate {
	final number = nextEnemyWaveTemplateNumber(recipeId, enemyPoints.length, context.objects, context.ruleIds);
	final zoneId = new ScenarioId('editor.$recipeId.zone.n$number');
	final commands:Array<EditorCommand> = [
		PutObject({
			id: zoneId,
			tags: [],
			placement: TriggerZone({origin: triggerPoint, size: {width: 1, height: 1, depth: 1}})
		})
	];
	final hideActions:Array<FlowAction> = [];
	final spawnActions:Array<FlowAction> = [];
	for (index in 0...enemyPoints.length) {
		final enemyId = enemyWaveEnemyId(recipeId, index, number);
		commands.push(putRecipeObject(enemyId, Entity(entityType, transformAt(enemyPoints[index]))));
		hideActions.push(Despawn(enemyId));
		spawnActions.push(Spawn(enemyId));
	}
	commands.push(PutRule({
		id: new ScenarioId('editor.rule.$recipeId.hide.n$number'),
		priority: 0,
		repeat: Once,
		event: LevelEntered(context.scenarioId),
		predicate: Always,
		actions: hideActions
	}));
	commands.push(PutRule({
		id: new ScenarioId('editor.rule.$recipeId.spawn.n$number'),
		priority: 0,
		repeat: Once,
		event: EnterZone(zoneId),
		predicate: Always,
		actions: spawnActions
	}));
	return {objectId: zoneId, commands: commands};
}

/** Convert one snapped cell to the canonical centered object transform. */
private function transformAt(point:VoxelPoint):ScenarioTransform {
	return {
		xMilli: point.x * 1000 + 500,
		yMilli: point.y * 1000,
		zMilli: point.z * 1000 + 500,
		yawDegrees: 0
	};
}

/** Choose a deterministic neighboring cell while staying inside the finite world. */
private function adjacentPoint(point:VoxelPoint, size:VoxelSize):Null<VoxelPoint> {
	if (point.z + 1 < size.depth)
		return {x: point.x, y: point.y, z: point.z + 1};
	if (point.z > 0)
		return {x: point.x, y: point.y, z: point.z - 1};
	if (point.x + 1 < size.width)
		return {x: point.x + 1, y: point.y, z: point.z};
	if (point.x > 0)
		return {x: point.x - 1, y: point.y, z: point.z};
	return null;
}

/** Find the nearest distinct horizontal cells in deterministic square rings. */
private function neighboringPoints(point:VoxelPoint, size:VoxelSize, count:Int):Null<Array<VoxelPoint>> {
	final result:Array<VoxelPoint> = [];
	final maximum = size.width > size.depth ? size.width : size.depth;
	for (distance in 1...maximum) {
		for (zOffset in -distance...distance + 1) {
			for (xOffset in -distance...distance + 1) {
				if (xOffset != -distance && xOffset != distance && zOffset != -distance && zOffset != distance)
					continue;
				final x = point.x + xOffset;
				final z = point.z + zOffset;
				if (x >= 0 && x < size.width && z >= 0 && z < size.depth)
					result.push({x: x, y: point.y, z: z});
				if (result.length == count)
					return result;
			}
		}
	}
	return null;
}

/** Wrap one ordinary placement in the same template result used by NPCs. */
private function singleObjectTemplate(objectId:ScenarioId, placement:ObjectPlacement):EditorObjectTemplate {
	return {
		objectId: objectId,
		commands: [putRecipeObject(objectId, placement)]
	};
}

/** Build one canonical placement command without retaining caller-owned arrays. */
private function putRecipeObject(objectId:ScenarioId, placement:ObjectPlacement):EditorCommand
	return PutObject({id: objectId, tags: [], placement: placement});

/** Find the first valid source-derived identity absent from the draft. */
private function nextRecipeId(recipeId:String, objects:Array<ScenarioObject>):ScenarioId {
	final prefix = 'editor.$recipeId.n';
	var number = 1;
	while (hasObjectId(objects, prefix + number))
		number++;
	return new ScenarioId(prefix + number);
}

/** Find one NPC suffix absent from both object and rule namespaces. */
private function nextRecipeTemplateNumber(recipeId:String, objects:Array<ScenarioObject>, ruleIds:Array<ScenarioId>):Int {
	final objectPrefix = 'editor.$recipeId.n';
	final rulePrefix = 'editor.rule.$recipeId.n';
	var number = 1;
	while (hasObjectId(objects, objectPrefix + number) || hasRuleId(ruleIds, rulePrefix + number))
		number++;
	return number;
}

/** Find one linked-pair suffix absent from both object roles and the rule. */
private function nextLinkedRecipeTemplateNumber(recipeId:String, objects:Array<ScenarioObject>, ruleIds:Array<ScenarioId>):Int {
	final sourcePrefix = 'editor.$recipeId.source.n';
	final targetPrefix = 'editor.$recipeId.target.n';
	final rulePrefix = 'editor.rule.$recipeId.n';
	var number = 1;
	while (hasObjectId(objects, sourcePrefix + number)
		|| hasObjectId(objects, targetPrefix + number)
		|| hasRuleId(ruleIds, rulePrefix + number))
		number++;
	return number;
}

/** Find one wave suffix absent from its trigger, enemies, and both rules. */
private function nextEnemyWaveTemplateNumber(recipeId:String, enemyCount:Int, objects:Array<ScenarioObject>, ruleIds:Array<ScenarioId>):Int {
	var number = 1;
	while (enemyWaveTemplateNumberUsed(recipeId, enemyCount, number, objects, ruleIds))
		number++;
	return number;
}

/** True when any record owned by one enemy-wave suffix already exists. */
private function enemyWaveTemplateNumberUsed(recipeId:String, enemyCount:Int, number:Int, objects:Array<ScenarioObject>, ruleIds:Array<ScenarioId>):Bool {
	if (hasObjectId(objects, 'editor.$recipeId.zone.n$number')
		|| hasRuleId(ruleIds, 'editor.rule.$recipeId.hide.n$number')
		|| hasRuleId(ruleIds, 'editor.rule.$recipeId.spawn.n$number'))
		return true;
	for (index in 0...enemyCount)
		if (hasObjectId(objects, enemyWaveEnemyId(recipeId, index, number).text()))
			return true;
	return false;
}

/** Derive one stable one-based enemy identity inside a wave. */
private function enemyWaveEnemyId(recipeId:String, index:Int, number:Int):ScenarioId
	return new ScenarioId('editor.$recipeId.enemy.n$number.i${index + 1}');

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

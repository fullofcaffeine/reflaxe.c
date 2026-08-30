package caxecraft.editor;

import caxecraft.editor.EditorEnvironment.copyEnvironment;
import caxecraft.editor.EditorFlowProjection.EditorZoneRuleProjection;
import caxecraft.editor.EditorFlowProjection.EditorFlowRuleProjection;
import caxecraft.editor.EditorFlowProjection.EditorTriggerOverlap;
import caxecraft.editor.EditorFlowProjection.projectFlowRules;
import caxecraft.editor.EditorFlowProjection.projectTriggerOverlaps;
import caxecraft.editor.EditorFlowProjection.projectZoneRules;
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.editor.EditorWorldViewport.projectWorld;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioEnvironment;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioText;
import caxecraft.scenario.ScenarioWorld.BlockPaletteEntry;

/**
 * Builds the small, copy-owned view that the visual editor draws and inspects.
 *
 * `EditorSession` keeps the parsed draft private because its arrays also feed
 * canonical history. This module reads that draft once and returns only fresh
 * presentation values and placement references. The screen can retain or
 * change these arrays without changing the draft, undo bytes, validation, or
 * Test Play state.
 */
/** Terrain facts needed for drawing and palette-local editor tools. */
typedef EditorPresentationWorld = {
	final size:VoxelSize;
	final palette:Array<BlockPaletteEntry>;
}

/** Copy-owned visual and placement values that do not project terrain cells. */
typedef EditorPresentationDetails = {
	final id:ScenarioId;
	final title:ScenarioText;
	final environment:Null<ScenarioEnvironment>;
	final world:EditorPresentationWorld;
	final objects:Array<ScenarioObject>;
	final dialogueIds:Array<ScenarioId>;
	final ruleIds:Array<ScenarioId>;
	final flowRuleCount:Int;
	final flowRules:Array<EditorFlowRuleProjection>;
	final zoneRuleLinks:Array<EditorZoneRuleProjection>;
	final flowOverlaps:Array<EditorTriggerOverlap>;
}

/** One complete revision-independent visual view of the current typed draft. */
typedef EditorPresentationSnapshot = {
	final id:ScenarioId;
	final title:ScenarioText;
	final environment:Null<ScenarioEnvironment>;
	final world:EditorPresentationWorld;
	final projection:Null<EditorWorldProjection>;
	final objects:Array<ScenarioObject>;
	final dialogueIds:Array<ScenarioId>;
	final ruleIds:Array<ScenarioId>;
	final flowRuleCount:Int;
	final flowRules:Array<EditorFlowRuleProjection>;
	final zoneRuleLinks:Array<EditorZoneRuleProjection>;
	final flowOverlaps:Array<EditorTriggerOverlap>;
}

/**
 * Project one private session draft without writing or parsing CAXEMAP bytes.
 *
 * Every mutable array and nested record in the result has a separate owner.
 * `projection` is `null` when chunk data cannot describe the declared world.
 */
function project(scenario:Scenario):EditorPresentationSnapshot {
	final details = projectDetails(scenario);
	return {
		id: details.id,
		title: details.title,
		environment: details.environment,
		world: details.world,
		projection: projectWorld(scenario.world),
		objects: details.objects,
		dialogueIds: details.dialogueIds,
		ruleIds: details.ruleIds,
		flowRuleCount: details.flowRuleCount,
		flowRules: details.flowRules,
		zoneRuleLinks: details.zoneRuleLinks,
		flowOverlaps: details.flowOverlaps
	};
}

/** Copy the visual values for an edit that cannot change terrain. */
function projectDetails(scenario:Scenario):EditorPresentationDetails {
	return {
		id: scenario.id,
		title: scenario.title,
		environment: scenario.environment == null ? null : copyEnvironment(scenario.environment),
		world: {
			size: copySize(scenario.world.size),
			palette: [
				for (entry in scenario.world.palette)
					{code: entry.code, blockType: entry.blockType}
			]
		},
		objects: [for (object in scenario.objects) copyObject(object)],
		dialogueIds: [for (dialogue in scenario.story.dialogues) dialogue.id],
		ruleIds: [for (rule in scenario.flow.rules) rule.id],
		flowRuleCount: scenario.flow.rules.length,
		flowRules: projectFlowRules(scenario.flow.rules),
		zoneRuleLinks: projectZoneRules(scenario.flow.rules, scenario.objects),
		flowOverlaps: projectTriggerOverlaps(scenario.flow.rules, scenario.objects)
	};
}

/** Copy one authored object and every record nested in its closed placement. */
private function copyObject(value:ScenarioObject):ScenarioObject
	return {id: value.id, tags: value.tags.copy(), placement: copyPlacement(value.placement)};

/** Copy each placement role without changing its semantic links or values. */
private function copyPlacement(value:ObjectPlacement):ObjectPlacement {
	return switch value {
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

/** Copy one millimeter transform into a separate record. */
private function copyTransform(value:ScenarioTransform):ScenarioTransform
	return {
		xMilli: value.xMilli,
		yMilli: value.yMilli,
		zMilli: value.zMilli,
		yawDegrees: value.yawDegrees
	};

/** Copy both records inside one half-open voxel box. */
private function copyBounds(value:VoxelBounds):VoxelBounds
	return {origin: {x: value.origin.x, y: value.origin.y, z: value.origin.z}, size: copySize(value.size)};

/** Copy positive or temporarily invalid dimensions without repairing them. */
private function copySize(value:VoxelSize):VoxelSize
	return {width: value.width, height: value.height, depth: value.depth};

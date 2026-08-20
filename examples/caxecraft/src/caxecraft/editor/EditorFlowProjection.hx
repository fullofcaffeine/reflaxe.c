package caxecraft.editor;

import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/**
	Projects zone events onto the trigger volumes that creators see in Plan view.

	The CaxeFlow rule keeps the stable reference. This module resolves that
	reference without changing the rule or copying logic into renderer state.
**/
enum EditorZoneRuleProjection {
	/** The rule points to one authored trigger with exact voxel bounds. */
	ResolvedZoneRule(ruleId:ScenarioId, zoneId:ScenarioId, bounds:VoxelBounds);

	/** The rule points to no trigger, so the editor can report the broken link. */
	UnresolvedZoneRule(ruleId:ScenarioId, zoneId:ScenarioId);
}

/** Return zone-event links in canonical rule order and omit unrelated events. */
function projectZoneRules(rules:Array<FlowRule>, objects:Array<ScenarioObject>):Array<EditorZoneRuleProjection> {
	final result:Array<EditorZoneRuleProjection> = [];
	for (rule in rules) {
		final zone = switch rule.event {
			case EnterZone(id) | LeaveZone(id): id;
			case _: null;
		};
		if (zone == null)
			continue;
		final bounds = triggerBounds(objects, zone);
		if (bounds == null)
			result.push(UnresolvedZoneRule(rule.id, zone));
		else
			result.push(ResolvedZoneRule(rule.id, zone, bounds));
	}
	return result;
}

/** Find only the trigger role. Another object with the same ID is not a zone. */
private function triggerBounds(objects:Array<ScenarioObject>, expected:ScenarioId):Null<VoxelBounds> {
	for (object in objects)
		if (object.id.text() == expected.text())
			return switch object.placement {
				case TriggerZone(bounds): bounds;
				case _: null;
			};
	return null;
}

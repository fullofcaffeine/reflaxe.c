package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.ScenarioId;

/**
	Removes one authored object with behavior that belongs to its trigger source.

	A trigger's enter and leave rules cannot run without that trigger. This module
	returns ordinary editor commands that remove those rules and the object in one
	atomic history entry. Other rule references stay explicit, so object deletion
	does not silently remove behavior that another event source owns.
**/
/** One object removal and its trigger-owned rule removals. */
typedef EditorObjectDeletePlan = {
	final commands:Array<EditorCommand>;
}

/**
	Plan one object removal and each rule whose event source is that trigger.

	The object command remains last. If the object no longer exists, the atomic
	batch rejects without exposing the earlier rule removals.
**/
function deleteObjectWithConnectedRules(sourceId:ScenarioId, rules:Array<FlowRule>):EditorObjectDeletePlan {
	final commands:Array<EditorCommand> = [];
	for (rule in rules)
		switch rule.event {
			case EnterZone(id) | LeaveZone(id) if (id.text() == sourceId.text()):
				commands.push(RemoveRule(rule.id));
			case _:
		}
	commands.push(RemoveObject(sourceId));
	return {commands: commands};
}

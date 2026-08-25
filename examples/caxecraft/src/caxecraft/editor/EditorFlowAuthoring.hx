package caxecraft.editor;

import caxecraft.editor.EditorFlowReferences.EditorFlowReferenceRole;
import caxecraft.editor.EditorFlowReferences.replaceActionFlowReference;
import caxecraft.editor.EditorFlowReferences.replaceEventFlowReference;
import caxecraft.editor.EditorFlowReferences.replacePredicateFlowReference;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlowCopy.copyFlowAction;
import caxecraft.scenario.CaxeFlowCopy.copyFlowActions;
import caxecraft.scenario.CaxeFlowCopy.copyFlowPredicate;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/**
	Edits child-facing CaxeFlow cards without creating another document format.

	The visual editor supplies typed card addresses and picker results. This
	module returns a new `FlowRule`, which the existing `PutRule` command commits
	through validation, canonical CAXEMAP bytes, undo, and redo. World picks carry
	the roles of the selected object, so a trigger cannot silently replace an
	actor field or a document-only level reference.
**/
/** One complete typed rule created when a trigger is connected to an action. */
typedef EditorZoneConnection = {
	final ruleId:ScenarioId;
	final priority:Int;
	final repeat:FlowRepeatPolicy;
	final zone:ScenarioId;
	final predicate:FlowPredicate;
	final actions:Array<FlowAction>;
}

/** The canonical card whose reference field a picker will replace. */
enum EditorFlowCardAddress {
	WhenCardAddress;
	IfCardAddress;
	DoCardAddress(actionIndex:Int);
}

/** One copy-owned card mutation; nested forms remain typed predicates/actions. */
enum EditorFlowCardEdit {
	ReplaceWhen(event:FlowEvent);
	ReplaceIf(predicate:FlowPredicate);
	InsertDo(index:Int, action:FlowAction);
	ReplaceDo(index:Int, action:FlowAction);
	MoveDo(fromIndex:Int, toIndex:Int);
	RemoveDo(index:Int);
	SetPriority(priority:Int);
	SetRepeat(repeat:FlowRepeatPolicy);
}

/** One world selection plus every semantic role its placement can satisfy. */
typedef EditorFlowWorldPick = {
	final id:ScenarioId;
	final roles:Array<EditorFlowReferenceRole>;
}

/** Exact refusal reason shown by the visual authoring controller. */
enum EditorFlowAuthoringError {
	InvalidDoIndex(index:Int);
	InvalidReferenceIndex(index:Int);
	WrongReferenceRole(expected:EditorFlowReferenceRole);
}

/** Result of one side-effect-free card edit or world-reference replacement. */
enum EditorFlowAuthoringResult {
	FlowRuleAuthored(rule:FlowRule);
	FlowRuleUnchanged;
	FlowRuleAuthoringRejected(error:EditorFlowAuthoringError);
}

/** Build a zone rule whose action list can contain every admitted typed action. */
function connectZone(connection:EditorZoneConnection):FlowRule
	return {
		id: connection.ruleId,
		priority: connection.priority,
		repeat: connection.repeat,
		event: EnterZone(connection.zone),
		predicate: copyFlowPredicate(connection.predicate),
		actions: copyFlowActions(connection.actions)
	};

/** Choose the first stable connection identity absent from the current draft. */
function nextZoneConnectionRuleId(zone:ScenarioId, ruleIds:Array<ScenarioId>):ScenarioId {
	final prefix = 'editor.rule.${zone.text()}.n';
	var number = 1;
	while (containsRuleId(ruleIds, prefix + number))
		number++;
	return new ScenarioId(prefix + number);
}

/** Replace, insert, or remove one canonical card without mutating the input. */
function editFlowCard(rule:FlowRule, edit:EditorFlowCardEdit):EditorFlowAuthoringResult {
	return switch edit {
		case ReplaceWhen(event): authored(rule, event, copyFlowPredicate(rule.predicate), copyFlowActions(rule.actions));
		case ReplaceIf(predicate): authored(rule, rule.event, copyFlowPredicate(predicate), copyFlowActions(rule.actions));
		case InsertDo(index, action):
			if (index < 0 || index > rule.actions.length) FlowRuleAuthoringRejected(InvalidDoIndex(index)); else {
				final actions = copyFlowActions(rule.actions);
				actions.insert(index, copyFlowAction(action));
				authored(rule, rule.event, copyFlowPredicate(rule.predicate), actions);
			}
		case ReplaceDo(index, action):
			if (!validActionIndex(rule.actions, index)) FlowRuleAuthoringRejected(InvalidDoIndex(index)); else {
				final actions = copyFlowActions(rule.actions);
				actions[index] = copyFlowAction(action);
				authored(rule, rule.event, copyFlowPredicate(rule.predicate), actions);
			}
		case MoveDo(fromIndex, toIndex):
			if (!validActionIndex(rule.actions, fromIndex)
				|| !validActionIndex(rule.actions,
					toIndex)) FlowRuleAuthoringRejected(InvalidDoIndex(!validActionIndex(rule.actions,
					fromIndex) ? fromIndex : toIndex)); else if (fromIndex == toIndex) FlowRuleUnchanged; else {
				final actions = copyFlowActions(rule.actions);
				final moved = actions.splice(fromIndex, 1)[0];
				actions.insert(toIndex, moved);
				authored(rule, rule.event, copyFlowPredicate(rule.predicate), actions);
			}
		case RemoveDo(index):
			if (!validActionIndex(rule.actions, index)) FlowRuleAuthoringRejected(InvalidDoIndex(index)); else {
				final actions = copyFlowActions(rule.actions);
				actions.splice(index, 1);
				authored(rule, rule.event, copyFlowPredicate(rule.predicate), actions);
			}
		case SetPriority(priority):
			if (rule.priority == priority) FlowRuleUnchanged; else FlowRuleAuthored({
				id: rule.id,
				priority: priority,
				repeat: rule.repeat,
				event: rule.event,
				predicate: copyFlowPredicate(rule.predicate),
				actions: copyFlowActions(rule.actions)
			});
		case SetRepeat(value):
			if (sameRepeat(rule.repeat, value)) FlowRuleUnchanged; else FlowRuleAuthored({
				id: rule.id,
				priority: rule.priority,
				repeat: value,
				event: rule.event,
				predicate: copyFlowPredicate(rule.predicate),
				actions: copyFlowActions(rule.actions)
			});
	};
}

/**
	Replace one projected reference with a compatible world pick.

	`referenceIndex` uses the same depth-first order as `EditorFlowProjection`.
	Document-only fields therefore remain available to a later tree picker while
	world fields can be chosen directly in Build or Plan view.
**/
function applyFlowWorldPick(rule:FlowRule, card:EditorFlowCardAddress, referenceIndex:Int, pick:EditorFlowWorldPick):EditorFlowAuthoringResult {
	if (referenceIndex < 0)
		return FlowRuleAuthoringRejected(InvalidReferenceIndex(referenceIndex));
	var event = rule.event;
	var predicate = copyFlowPredicate(rule.predicate);
	var actions = copyFlowActions(rule.actions);
	var found = false;
	var accepted = false;
	var changed = false;
	var expected:Null<EditorFlowReferenceRole> = null;
	switch card {
		case WhenCardAddress:
			final outcome = replaceEventFlowReference(event, referenceIndex, pick.id, pick.roles);
			event = outcome.value;
			found = outcome.found;
			accepted = outcome.accepted;
			changed = outcome.changed;
			expected = outcome.expected;
		case IfCardAddress:
			final outcome = replacePredicateFlowReference(rule.predicate, referenceIndex, pick.id, pick.roles);
			predicate = outcome.value;
			found = outcome.found;
			accepted = outcome.accepted;
			changed = outcome.changed;
			expected = outcome.expected;
		case DoCardAddress(actionIndex):
			if (!validActionIndex(rule.actions, actionIndex))
				return FlowRuleAuthoringRejected(InvalidDoIndex(actionIndex));
			final outcome = replaceActionFlowReference(rule.actions[actionIndex], referenceIndex, pick.id, pick.roles);
			actions[actionIndex] = outcome.value;
			found = outcome.found;
			accepted = outcome.accepted;
			changed = outcome.changed;
			expected = outcome.expected;
	}
	if (!found || expected == null)
		return FlowRuleAuthoringRejected(InvalidReferenceIndex(referenceIndex));
	if (!accepted)
		return FlowRuleAuthoringRejected(WrongReferenceRole(expected));
	if (!changed)
		return FlowRuleUnchanged;
	return authored(rule, event, predicate, actions);
}

/** Return the world-picker roles admitted by one closed placement kind. */
function worldPickFor(object:ScenarioObject):EditorFlowWorldPick {
	final roles = switch object.placement {
		case TriggerZone(_): [ZoneFlowReference, WorldObjectFlowReference];
		case PlayerSpawn(_) | Entity(_, _) | Npc(_, _, _): [ActorFlowReference, WorldObjectFlowReference];
		case Checkpoint(_) | Item(_, _, _) | Prefab(_, _) | StatefulObject(_, _, _): [WorldObjectFlowReference];
	};
	return {id: object.id, roles: roles};
}

/** True when a card role can be filled by selecting a visible world object. */
function isWorldPickableFlowRole(role:EditorFlowReferenceRole):Bool
	return switch role {
		case ZoneFlowReference | WorldObjectFlowReference | ActorFlowReference: true;
		case VariableFlowReference | ObjectiveFlowReference | StoryFlowReference | SequenceFlowReference | TimerFlowReference | CampaignExitFlowReference |
			LevelFlowReference: false;
	};

private function authored(rule:FlowRule, event:FlowEvent, predicate:FlowPredicate, actions:Array<FlowAction>):EditorFlowAuthoringResult
	return FlowRuleAuthored({
		id: rule.id,
		priority: rule.priority,
		repeat: rule.repeat,
		event: event,
		predicate: predicate,
		actions: actions
	});

private inline function validActionIndex(actions:Array<FlowAction>, index:Int):Bool
	return index >= 0 && index < actions.length;

/** Compare repeat policy values without relying on enum object identity. */
private function sameRepeat(left:FlowRepeatPolicy, right:FlowRepeatPolicy):Bool
	return switch [left, right] {
		case [Once, Once] | [Repeat, Repeat] | [OncePerActor, OncePerActor]: true;
		case [Cooldown(leftTicks), Cooldown(rightTicks)] | [CooldownPerActor(leftTicks), CooldownPerActor(rightTicks)]: leftTicks == rightTicks;
		case _: false;
	};

private function containsRuleId(ruleIds:Array<ScenarioId>, expected:String):Bool {
	for (id in ruleIds)
		if (id.text() == expected)
			return true;
	return false;
}

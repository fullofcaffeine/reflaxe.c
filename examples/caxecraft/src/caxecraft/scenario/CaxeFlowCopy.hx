package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowChoice;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRule;

/**
	Builds copy-owned CaxeFlow values at editor and persistence boundaries.

	Most CaxeFlow constructors contain immutable scalar values. Predicates,
	actions, arguments, and seeded choices can contain mutable Haxe arrays. These
	functions copy those arrays recursively so a widget or automation caller
	cannot change an accepted draft after its history entry is committed.
**/
/** Return one rule whose nested predicate and action arrays have a new owner. */
function copyFlowRule(rule:FlowRule):FlowRule
	return {
		id: rule.id,
		priority: rule.priority,
		repeat: rule.repeat,
		event: rule.event,
		predicate: copyFlowPredicate(rule.predicate),
		actions: copyFlowActions(rule.actions)
	};

/** Recursively copy one closed condition tree without changing its meaning. */
function copyFlowPredicate(predicate:FlowPredicate):FlowPredicate
	return switch predicate {
		case Always: Always;
		case All(children): All([for (child in children) copyFlowPredicate(child)]);
		case AnyOf(children): AnyOf([for (child in children) copyFlowPredicate(child)]);
		case Not(child): Not(copyFlowPredicate(child));
		case FlagIs(variable, expected): FlagIs(variable, expected);
		case CounterCompare(variable, comparison, value): CounterCompare(variable, comparison, value);
		case StateIs(variable, expected): StateIs(variable, expected);
		case ObjectStateIs(objectId, expected): ObjectStateIs(objectId, expected);
		case InventoryHas(owner, itemType, comparison, quantity): InventoryHas(owner, itemType, comparison, quantity);
		case ObjectiveIs(objective, expected): ObjectiveIs(objective, expected);
		case NearObject(actor, objectId, maximumMilliBlocks): NearObject(actor, objectId, maximumMilliBlocks);
		case ModeIs(mode): ModeIs(mode);
		case EventActorIs(actor): EventActorIs(actor);
		case EventSweptIs(expected): EventSweptIs(expected);
	};

/** Recursively copy an ordered action list, including choices and arguments. */
function copyFlowActions(actions:Array<FlowAction>):Array<FlowAction>
	return [for (action in actions) copyFlowAction(action)];

/** Copy one action and every mutable collection nested below it. */
function copyFlowAction(action:FlowAction):FlowAction
	return switch action {
		case ShowDialogue(dialogue): ShowDialogue(dialogue);
		case AddJournal(entry): AddJournal(entry);
		case SetFlag(variable, value): SetFlag(variable, value);
		case SetCounter(variable, value): SetCounter(variable, value);
		case AddCounter(variable, delta): AddCounter(variable, delta);
		case SetState(variable, value): SetState(variable, value);
		case GiveItem(owner, itemType, quantity): GiveItem(owner, itemType, quantity);
		case TakeItem(owner, itemType, quantity): TakeItem(owner, itemType, quantity);
		case Spawn(objectId): Spawn(objectId);
		case Despawn(objectId): Despawn(objectId);
		case SetObjectState(objectId, value): SetObjectState(objectId, value);
		case SetCheckpoint(checkpoint): SetCheckpoint(checkpoint);
		case SetObjective(objective, value): SetObjective(objective, value);
		case PlayEffect(effect, objectId): PlayEffect(effect, objectId);
		case RequestCampaignExit(exit): RequestCampaignExit(exit);
		case EmitSignal(signal): EmitSignal(signal);
		case Schedule(timer, ticks, sequence, arguments): Schedule(timer, ticks, sequence, copyArguments(arguments));
		case CallSequence(sequence, arguments): CallSequence(sequence, copyArguments(arguments));
		case ChooseSeeded(seedVariable, choices): ChooseSeeded(seedVariable, [for (choice in choices) copyChoice(choice)]);
	};

/** Copy argument records so the enclosing action never retains caller arrays. */
private function copyArguments(arguments:Array<FlowArgument>):Array<FlowArgument>
	return [
		for (argument in arguments)
			switch argument {
				case Value(value):
					Value(value);
				case Variable(variable):
					Variable(variable);
			}
	];

/** Copy one weighted branch and its ordered nested actions. */
private function copyChoice(choice:FlowChoice):FlowChoice
	return {weight: choice.weight, actions: copyFlowActions(choice.actions)};

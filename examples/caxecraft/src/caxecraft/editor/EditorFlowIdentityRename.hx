package caxecraft.editor;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowChoice;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;

/**
	Renames one CaxeFlow declaration and every reference with the same role.

	Stable IDs are links, not labels. A visual rename must therefore update the
	declaration and its complete closed reference set in one candidate document.
	The reducer validates the requested identity first, then publishes these
	copy-owned arrays as one undoable history entry.
**/
/** Copy-owned Flow arrays produced by one complete identity rewrite. */
typedef EditorFlowIdentityRename = {
	final variables:Array<FlowVariable>;
	final sequences:Array<FlowSequence>;
	final rules:Array<FlowRule>;
}

/** Rename one persistent variable and every typed variable-role reference. */
function renameFlowVariable(scenario:Scenario, before:ScenarioId, after:ScenarioId):EditorFlowIdentityRename
	return renameFlowIdentity(scenario, before, after, null, null);

/** Rename one reusable sequence, its local scopes, and every call or schedule. */
function renameFlowSequence(scenario:Scenario, before:ScenarioId, after:ScenarioId):EditorFlowIdentityRename
	return renameFlowIdentity(scenario, null, null, before, after);

private function renameFlowIdentity(scenario:Scenario, variableBefore:Null<ScenarioId>, variableAfter:Null<ScenarioId>, sequenceBefore:Null<ScenarioId>,
		sequenceAfter:Null<ScenarioId>):EditorFlowIdentityRename {
	return {
		variables: [
			for (variable in scenario.flow.variables)
				{
					id: replaceNullable(variable.id, variableBefore, variableAfter),
					scope: renameScope(variable.scope, sequenceBefore, sequenceAfter),
					initial: variable.initial
				}
		],
		sequences: [
			for (sequence in scenario.flow.sequences)
				{
					id: replaceNullable(sequence.id, sequenceBefore, sequenceAfter),
					parameters: [
						for (parameter in sequence.parameters) {id: parameter.id, initial: parameter.initial}
					],
					actions: [
						for (action in sequence.actions)
							renameAction(action, variableBefore, variableAfter, sequenceBefore, sequenceAfter)
					]
				}
		],
		rules: [
			for (rule in scenario.flow.rules)
				{
					id: rule.id,
					priority: rule.priority,
					repeat: rule.repeat,
					event: renameEvent(rule.event, variableBefore, variableAfter),
					predicate: renamePredicate(rule.predicate, variableBefore, variableAfter),
					actions: [
						for (action in rule.actions)
							renameAction(action, variableBefore, variableAfter, sequenceBefore, sequenceAfter)
					]
				}
		]
	};
}

private function renameScope(scope:FlowScope, before:Null<ScenarioId>, after:Null<ScenarioId>):FlowScope
	return switch scope {
		case Local(sequence): Local(replaceNullable(sequence, before, after));
		case Map: Map;
		case Player: Player;
		case Quest: Quest;
	};

private function renameEvent(event:FlowEvent, before:Null<ScenarioId>, after:Null<ScenarioId>):FlowEvent
	return switch event {
		case StateChanged(variable): StateChanged(replaceNullable(variable, before, after));
		case EnterZone(zone): EnterZone(zone);
		case LeaveZone(zone): LeaveZone(zone);
		case Interact(objectId): Interact(objectId);
		case BlockChanged(zone, blockType): BlockChanged(zone, blockType);
		case UseItem(itemType): UseItem(itemType);
		case ItemCollected(itemType): ItemCollected(itemType);
		case EntityDefeated(entity): EntityDefeated(entity);
		case SignalReceived(signal): SignalReceived(signal);
		case TimerExpired(timer): TimerExpired(timer);
		case ObjectiveChanged(objective): ObjectiveChanged(objective);
		case LevelEntered(level): LevelEntered(level);
		case CampaignExitRequested(exit): CampaignExitRequested(exit);
	};

private function renamePredicate(predicate:FlowPredicate, before:Null<ScenarioId>, after:Null<ScenarioId>):FlowPredicate
	return switch predicate {
		case Always: Always;
		case All(children): All([for (child in children) renamePredicate(child, before, after)]);
		case AnyOf(children): AnyOf([for (child in children) renamePredicate(child, before, after)]);
		case Not(child): Not(renamePredicate(child, before, after));
		case FlagIs(variable, expected): FlagIs(replaceNullable(variable, before, after), expected);
		case CounterCompare(variable, comparison, value): CounterCompare(replaceNullable(variable, before, after), comparison, value);
		case StateIs(variable, expected): StateIs(replaceNullable(variable, before, after), expected);
		case ObjectStateIs(objectId, expected): ObjectStateIs(objectId, expected);
		case InventoryHas(owner, itemType, comparison, quantity): InventoryHas(owner, itemType, comparison, quantity);
		case ObjectiveIs(objective, expected): ObjectiveIs(objective, expected);
		case NearObject(actor, objectId, maximumMilliBlocks): NearObject(actor, objectId, maximumMilliBlocks);
		case ModeIs(mode): ModeIs(mode);
		case EventActorIs(actor): EventActorIs(actor);
		case EventSweptIs(expected): EventSweptIs(expected);
	};

private function renameAction(action:FlowAction, variableBefore:Null<ScenarioId>, variableAfter:Null<ScenarioId>, sequenceBefore:Null<ScenarioId>,
		sequenceAfter:Null<ScenarioId>):FlowAction
	return switch action {
		case SetFlag(variable, value): SetFlag(replaceNullable(variable, variableBefore, variableAfter), value);
		case SetCounter(variable, value): SetCounter(replaceNullable(variable, variableBefore, variableAfter), value);
		case AddCounter(variable, delta): AddCounter(replaceNullable(variable, variableBefore, variableAfter), delta);
		case SetState(variable, value): SetState(replaceNullable(variable, variableBefore, variableAfter), value);
		case Schedule(timer, ticks, sequence, arguments):
			Schedule(timer, ticks, replaceNullable(sequence, sequenceBefore, sequenceAfter), renameArguments(arguments, variableBefore, variableAfter));
		case CallSequence(sequence, arguments):
			CallSequence(replaceNullable(sequence, sequenceBefore, sequenceAfter), renameArguments(arguments, variableBefore, variableAfter));
		case ChooseSeeded(seed, choices):
			final renamed:Array<FlowChoice> = [];
			for (choice in choices)
				renamed.push({
					weight: choice.weight,
					actions: [
						for (nested in choice.actions)
							renameAction(nested, variableBefore, variableAfter, sequenceBefore, sequenceAfter)
					]
				});
			ChooseSeeded(replaceNullable(seed, variableBefore, variableAfter), renamed);
		case ShowDialogue(dialogue): ShowDialogue(dialogue);
		case AddJournal(entry): AddJournal(entry);
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
	};

private function renameArguments(arguments:Array<FlowArgument>, before:Null<ScenarioId>, after:Null<ScenarioId>):Array<FlowArgument>
	return [
		for (argument in arguments)
			switch argument {
				case Value(value):
					Value(value);
				case Variable(variable):
					Variable(replaceNullable(variable, before, after));
			}
	];

private inline function replaceNullable(value:ScenarioId, before:Null<ScenarioId>, after:Null<ScenarioId>):ScenarioId
	return before != null && after != null && value.text() == before.text() ? after : value;

package caxecraft.editor;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;

/**
	Renames one world object and every typed reference that points to it.

	An object ID is both its canonical editor name and the stable link used by
	dialogue speakers and CaxeFlow. Renaming only the placement would leave stale
	links. This module rewrites the complete closed set in one candidate scenario;
	`EditorSession` then publishes one history entry or none.
**/
/** Return a new scenario whose object identity and object-role links agree. */
function renameScenarioObject(scenario:Scenario, before:ScenarioId, after:ScenarioId):Scenario {
	return {
		formatVersion: scenario.formatVersion,
		requiredFeatures: scenario.requiredFeatures.copy(),
		optionalFeatures: scenario.optionalFeatures.copy(),
		id: scenario.id,
		assetPack: scenario.assetPack,
		messages: scenario.messages,
		title: scenario.title,
		mode: scenario.mode,
		environment: scenario.environment,
		world: scenario.world,
		objects: [
			for (object in scenario.objects)
				{
					id: replace(object.id, before, after),
					tags: object.tags.copy(),
					placement: object.placement
				}
		],
		story: {
			speakerNames: [
				for (speaker in scenario.story.speakerNames)
					{
						speaker: replace(speaker.speaker, before, after),
						name: speaker.name
					}
			],
			dialogues: [
				for (dialogue in scenario.story.dialogues)
					{
						id: dialogue.id,
						lines: [
							for (line in dialogue.lines)
								{
									speaker: line.speaker == null ? null : replace(line.speaker, before, after),
									text: line.text
								}
						]
					}
			],
			journal: scenario.story.journal.copy(),
			objectives: scenario.story.objectives.copy(),
			routes: scenario.story.routes.copy()
		},
		flow: {
			variables: scenario.flow.variables.copy(),
			sequences: [
				for (sequence in scenario.flow.sequences)
					renameSequence(sequence, before, after)
			],
			rules: [for (rule in scenario.flow.rules) renameRule(rule, before, after)]
		},
		extensions: scenario.extensions.copy()
	};
}

private function renameSequence(sequence:FlowSequence, before:ScenarioId, after:ScenarioId):FlowSequence
	return {
		id: sequence.id,
		parameters: sequence.parameters.copy(),
		actions: [for (action in sequence.actions) renameAction(action, before, after)]
	};

private function renameRule(rule:FlowRule, before:ScenarioId, after:ScenarioId):FlowRule
	return {
		id: rule.id,
		priority: rule.priority,
		repeat: rule.repeat,
		event: renameEvent(rule.event, before, after),
		predicate: renamePredicate(rule.predicate, before, after),
		actions: [for (action in rule.actions) renameAction(action, before, after)]
	};

private function renameEvent(event:FlowEvent, before:ScenarioId, after:ScenarioId):FlowEvent
	return switch event {
		case EnterZone(zone): EnterZone(replace(zone, before, after));
		case LeaveZone(zone): LeaveZone(replace(zone, before, after));
		case Interact(objectId): Interact(replace(objectId, before, after));
		case BlockChanged(zone, blockType): BlockChanged(replace(zone, before, after), blockType);
		case EntityDefeated(entity): EntityDefeated(replace(entity, before, after));
		case UseItem(itemType): UseItem(itemType);
		case ItemCollected(itemType): ItemCollected(itemType);
		case SignalReceived(signal): SignalReceived(signal);
		case TimerExpired(timer): TimerExpired(timer);
		case ObjectiveChanged(objective): ObjectiveChanged(objective);
		case StateChanged(variable): StateChanged(variable);
		case LevelEntered(level): LevelEntered(level);
		case CampaignExitRequested(exit): CampaignExitRequested(exit);
	};

private function renamePredicate(predicate:FlowPredicate, before:ScenarioId, after:ScenarioId):FlowPredicate
	return switch predicate {
		case Always: Always;
		case All(children): All([for (child in children) renamePredicate(child, before, after)]);
		case AnyOf(children): AnyOf([for (child in children) renamePredicate(child, before, after)]);
		case Not(child): Not(renamePredicate(child, before, after));
		case FlagIs(variable, expected): FlagIs(variable, expected);
		case CounterCompare(variable, comparison, value): CounterCompare(variable, comparison, value);
		case StateIs(variable, expected): StateIs(variable, expected);
		case ObjectStateIs(objectId, expected): ObjectStateIs(replace(objectId, before, after), expected);
		case InventoryHas(owner, itemType, comparison, quantity): InventoryHas(replace(owner, before, after), itemType, comparison, quantity);
		case ObjectiveIs(objective, expected): ObjectiveIs(objective, expected);
		case NearObject(actor, objectId, maximumMilliBlocks):
			NearObject(replace(actor, before, after), replace(objectId, before, after), maximumMilliBlocks);
		case ModeIs(mode): ModeIs(mode);
		case EventActorIs(actor): EventActorIs(replace(actor, before, after));
		case EventSweptIs(expected): EventSweptIs(expected);
	};

private function renameAction(action:FlowAction, before:ScenarioId, after:ScenarioId):FlowAction
	return switch action {
		case ShowDialogue(dialogue): ShowDialogue(dialogue);
		case AddJournal(entry): AddJournal(entry);
		case SetFlag(variable, value): SetFlag(variable, value);
		case SetCounter(variable, value): SetCounter(variable, value);
		case AddCounter(variable, delta): AddCounter(variable, delta);
		case SetState(variable, value): SetState(variable, value);
		case GiveItem(owner, itemType, quantity): GiveItem(replace(owner, before, after), itemType, quantity);
		case TakeItem(owner, itemType, quantity): TakeItem(replace(owner, before, after), itemType, quantity);
		case Spawn(objectId): Spawn(replace(objectId, before, after));
		case Despawn(objectId): Despawn(replace(objectId, before, after));
		case SetObjectState(objectId, value): SetObjectState(replace(objectId, before, after), value);
		case SetCheckpoint(checkpoint): SetCheckpoint(replace(checkpoint, before, after));
		case SetObjective(objective, value): SetObjective(objective, value);
		case PlayEffect(effect, objectId): PlayEffect(effect, objectId == null ? null : replace(objectId, before, after));
		case RequestCampaignExit(exit): RequestCampaignExit(exit);
		case EmitSignal(signal): EmitSignal(signal);
		case Schedule(timer, ticks, sequence, arguments): Schedule(timer, ticks, sequence, copyArguments(arguments));
		case CallSequence(sequence, arguments): CallSequence(sequence, copyArguments(arguments));
		case ChooseSeeded(seedVariable, choices): ChooseSeeded(seedVariable, [
				for (choice in choices)
					{
						weight: choice.weight,
						actions: [for (nested in choice.actions) renameAction(nested, before, after)]
					}
			]);
	};

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

private inline function replace(value:ScenarioId, before:ScenarioId, after:ScenarioId):ScenarioId
	return value.text() == before.text() ? after : value;

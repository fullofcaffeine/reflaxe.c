package caxecraft.editor;

import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.editor.EditorFlowReferences.EditorFlowReferenceRole;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowValueKind;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionDescriptor;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionId;
import caxecraft.scenario.CaxeFlowActionRegistry.allFlowActionDescriptors;
import caxecraft.scenario.CaxeFlowEventRegistry.FlowEventDescriptor;
import caxecraft.scenario.CaxeFlowEventRegistry.FlowEventId;
import caxecraft.scenario.CaxeFlowEventRegistry.allFlowEventDescriptors;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventSupportsActor;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventSupportsSwept;
import caxecraft.scenario.CaxeFlowPredicateRegistry.FlowPredicateDescriptor;
import caxecraft.scenario.CaxeFlowPredicateRegistry.FlowPredicateId;
import caxecraft.scenario.CaxeFlowPredicateRegistry.allFlowPredicateDescriptors;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/**
	Builds child-facing card choices from the executable CaxeFlow registries.

	The registries own which WHEN, IF, and DO constructors exist. This module only
	binds those typed schemas to references already admitted by the current map and
	content pack. A choice is either a complete enum value or explicitly
	unavailable; an incomplete form can therefore never replace the playable rule.
**/
/** Copy-owned content identities that closed card fields can select. */
typedef EditorFlowContentChoices = {
	final blocks:Array<ContentId>;
	final items:Array<ContentId>;
	final states:Array<ContentId>;
	final effects:Array<ContentId>;
	final signals:Array<ContentId>;
}

/** Why one registry-admitted card cannot yet form a valid typed value. */
enum EditorFlowChoiceUnavailable {
	MissingWorldReference;
	MissingActorReference;
	MissingEntityReference;
	MissingZoneReference;
	MissingCheckpointReference;
	MissingStatefulObjectReference;
	MissingDialogueReference;
	MissingJournalReference;
	MissingObjectiveReference;
	MissingVariableReference(kind:FlowValueKind);
	MissingSequenceReference;
	MissingTimerReference;
	MissingCampaignExitReference;
	MissingBlockContent;
	MissingItemContent;
	MissingStateContent;
	MissingEffectContent;
	MissingSignalContent;
	EventHasNoActorContext;
	EventHasNoSweptContext;
	NestedChoiceNotAllowed;
}

/** One event descriptor paired with a complete value or an exact blocker. */
enum EditorFlowEventChoice {
	ReadyEventChoice(descriptor:FlowEventDescriptor, value:FlowEvent);
	UnavailableEventChoice(descriptor:FlowEventDescriptor, reason:EditorFlowChoiceUnavailable);
}

/** One predicate descriptor paired with a complete value or an exact blocker. */
enum EditorFlowPredicateChoice {
	ReadyPredicateChoice(descriptor:FlowPredicateDescriptor, value:FlowPredicate);
	UnavailablePredicateChoice(descriptor:FlowPredicateDescriptor, reason:EditorFlowChoiceUnavailable);
}

/** One action descriptor paired with a complete value or an exact blocker. */
enum EditorFlowActionChoice {
	ReadyActionChoice(descriptor:FlowActionDescriptor, value:FlowAction);
	UnavailableActionChoice(descriptor:FlowActionDescriptor, reason:EditorFlowChoiceUnavailable);
}

/** Project content-field choices directly from one validated runtime registry. */
function editorFlowContentChoices(registry:RuntimeContentRegistry):EditorFlowContentChoices {
	final blocks:Array<ContentId> = [];
	for (index in 0...registry.blockCount()) {
		final value = registry.blockIdAt(index);
		if (value != null && !registry.isAirBlock(value))
			blocks.push(value);
	}
	final items:Array<ContentId> = [];
	for (index in 0...registry.itemCount()) {
		final value = registry.itemIdAt(index);
		if (value != null)
			items.push(value);
	}
	final states:Array<ContentId> = [];
	for (index in 0...registry.stateCount()) {
		final value = registry.stateIdAt(index);
		if (value != null)
			states.push(value);
	}
	final effects:Array<ContentId> = [];
	for (index in 0...registry.effectCount()) {
		final value = registry.effectIdAt(index);
		if (value != null)
			effects.push(value);
	}
	return {
		blocks: blocks,
		items: items,
		states: states,
		effects: effects,
		signals: []
	};
}

/** Return every event descriptor exactly once in canonical registry order. */
function eventCardChoices(scenario:Scenario, content:EditorFlowContentChoices):Array<EditorFlowEventChoice> {
	final result:Array<EditorFlowEventChoice> = [];
	for (descriptor in allFlowEventDescriptors()) {
		final value = buildEvent(descriptor.id, scenario, content);
		result.push(value == null ? UnavailableEventChoice(descriptor,
			unavailableEventReason(descriptor.id, scenario, content)) : ReadyEventChoice(descriptor, value));
	}
	return result;
}

/** Return every predicate descriptor exactly once in canonical registry order. */
function predicateCardChoices(scenario:Scenario, event:FlowEvent, content:EditorFlowContentChoices):Array<EditorFlowPredicateChoice> {
	final result:Array<EditorFlowPredicateChoice> = [];
	for (descriptor in allFlowPredicateDescriptors()) {
		final value = buildPredicate(descriptor.id, scenario, event, content);
		result.push(value == null ? UnavailablePredicateChoice(descriptor,
			unavailablePredicateReason(descriptor.id, scenario, event, content)) : ReadyPredicateChoice(descriptor, value));
	}
	return result;
}

/**
	Return every action descriptor once, including disabled nested-only choices.

	`insideChoice` keeps the visible inventory lossless while refusing the one
	constructor that validation forbids inside a weighted branch.
**/
function actionCardChoices(scenario:Scenario, owner:ScenarioId, content:EditorFlowContentChoices, insideChoice:Bool = false):Array<EditorFlowActionChoice> {
	final result:Array<EditorFlowActionChoice> = [];
	for (descriptor in allFlowActionDescriptors()) {
		final value = insideChoice
			&& descriptor.id == FlowActionId.ChooseAction ? null : buildAction(descriptor.id, scenario, owner, content);
		result.push(value == null ? UnavailableActionChoice(descriptor,
			unavailableActionReason(descriptor.id, scenario, content, insideChoice)) : ReadyActionChoice(descriptor, value));
	}
	return result;
}

/** Read the descriptor from either availability branch without duplicating UI logic. */
function eventChoiceDescriptor(choice:EditorFlowEventChoice):FlowEventDescriptor
	return switch choice {
		case ReadyEventChoice(descriptor, _) | UnavailableEventChoice(descriptor, _): descriptor;
	};

/** Read the descriptor from either availability branch without duplicating UI logic. */
function predicateChoiceDescriptor(choice:EditorFlowPredicateChoice):FlowPredicateDescriptor
	return switch choice {
		case ReadyPredicateChoice(descriptor, _) | UnavailablePredicateChoice(descriptor, _): descriptor;
	};

/** Read the descriptor from either availability branch without duplicating UI logic. */
function actionChoiceDescriptor(choice:EditorFlowActionChoice):FlowActionDescriptor
	return switch choice {
		case ReadyActionChoice(descriptor, _) | UnavailableActionChoice(descriptor, _): descriptor;
	};

/**
	Return only document identities compatible with one typed reference role.

	World roles stay in the 3D picker. Content IDs stay in the validated content
	choice record. This function therefore cannot accidentally offer an objective
	where a dialogue, checkpoint, or typed variable is required.
**/
function documentFlowReferenceChoices(scenario:Scenario, role:EditorFlowReferenceRole):Array<ScenarioId> {
	final result:Array<ScenarioId> = [];
	switch role {
		case TypedVariableFlowReference(kind):
			for (variable in scenario.flow.variables)
				if (persistentScope(variable.scope) && valueKind(variable.initial) == kind)
					result.push(variable.id);
		case AnyVariableFlowReference:
			for (variable in scenario.flow.variables)
				if (persistentScope(variable.scope))
					result.push(variable.id);
		case ObjectiveFlowReference:
			for (objective in scenario.story.objectives)
				result.push(objective.id);
		case DialogueFlowReference:
			for (dialogue in scenario.story.dialogues)
				result.push(dialogue.id);
		case JournalFlowReference:
			for (entry in scenario.story.journal)
				result.push(entry.id);
		case SequenceFlowReference:
			for (sequence in scenario.flow.sequences)
				result.push(sequence.id);
		case TimerFlowReference:
			collectOwnedReferences(scenario, true, result);
		case CampaignExitFlowReference:
			collectOwnedReferences(scenario, false, result);
		case LevelFlowReference:
			result.push(scenario.id);
		case ZoneFlowReference | WorldObjectFlowReference | ActorFlowReference | EntityFlowReference | StatefulObjectFlowReference | CheckpointFlowReference |
			InventoryOwnerFlowReference:
	}
	return result;
}

/** True when a reference belongs in the typed document picker. */
function isDocumentFlowReferenceRole(role:EditorFlowReferenceRole):Bool
	return switch role {
		case TypedVariableFlowReference(_) | ObjectiveFlowReference | DialogueFlowReference | JournalFlowReference | SequenceFlowReference |
			TimerFlowReference | CampaignExitFlowReference | LevelFlowReference: true;
		case AnyVariableFlowReference | ZoneFlowReference | WorldObjectFlowReference | ActorFlowReference | EntityFlowReference |
			StatefulObjectFlowReference | CheckpointFlowReference | InventoryOwnerFlowReference: false;
	};

private function buildEvent(id:FlowEventId, scenario:Scenario, content:EditorFlowContentChoices):Null<FlowEvent> {
	return switch id {
		case EnterZoneEvent:
			final zone = firstZone(scenario);
			zone == null ? null : EnterZone(zone);
		case LeaveZoneEvent:
			final zone = firstZone(scenario);
			zone == null ? null : LeaveZone(zone);
		case InteractEvent:
			final objectId = firstObject(scenario);
			objectId == null ? null : Interact(objectId);
		case BlockChangedEvent: final zone = firstZone(scenario); final block = firstContent(content.blocks); zone == null || block == null ? null : BlockChanged(zone,
				block);
		case UseItemEvent: contentEvent(firstContent(content.items), true);
		case ItemCollectedEvent: contentEvent(firstContent(content.items), false);
		case EntityDefeatedEvent:
			final entity = firstEntity(scenario);
			entity == null ? null : EntityDefeated(entity);
		case SignalEvent:
			final signal = firstContent(content.signals);
			signal == null ? null : SignalReceived(signal);
		case TimerEvent:
			final timer = firstOwnedTimer(scenario);
			timer == null ? null : TimerExpired(timer);
		case ObjectiveChangedEvent:
			final objective = firstObjective(scenario);
			objective == null ? null : ObjectiveChanged(objective);
		case StateChangedEvent:
			final variable = firstVariable(scenario, StateValue);
			variable == null ? null : StateChanged(variable);
		case LevelEnteredEvent: LevelEntered(scenario.id);
		case CampaignExitRequestedEvent:
			final exit = firstOwnedExit(scenario);
			exit == null ? null : CampaignExitRequested(exit);
	};
}

private function buildPredicate(id:FlowPredicateId, scenario:Scenario, event:FlowEvent, content:EditorFlowContentChoices):Null<FlowPredicate> {
	return switch id {
		case AlwaysPredicate: Always;
		case AllPredicate: All([Always]);
		case AnyPredicate: AnyOf([Always]);
		case NotPredicate: Not(Always);
		case FlagPredicate:
			final variable = firstVariable(scenario, FlagValue);
			variable == null ? null : FlagIs(variable, true);
		case CounterPredicate:
			final variable = firstVariable(scenario, CounterValue);
			variable == null ? null : CounterCompare(variable, GreaterOrEqual, 1);
		case StatePredicate: final variable = firstVariable(scenario,
				StateValue); final state = firstStateForVariable(scenario, variable, content); variable == null || state == null ? null : StateIs(variable, state);
		case ObjectStatePredicate:
			final pair = firstStatefulObject(scenario);
			pair == null ? null : ObjectStateIs(pair.id, pair.state);
		case InventoryPredicate: final owner = firstObject(scenario); final item = firstContent(content.items); owner == null || item == null ? null : InventoryHas(owner,
				item, GreaterOrEqual, 1);
		case ObjectivePredicate:
			final objective = firstObjective(scenario);
			objective == null ? null : ObjectiveIs(objective, Active);
		case NearPredicate: final actor = firstActor(scenario); final objectId = firstObject(scenario); actor == null || objectId == null ? null : NearObject(actor,
				objectId, 2000);
		case ModePredicate: ModeIs(scenario.mode);
		case EventActorPredicate: final actor = firstActor(scenario); !flowEventSupportsActor(event) || actor == null ? null : EventActorIs(actor);
		case EventSweptPredicate: flowEventSupportsSwept(event) ? EventSweptIs(true) : null;
	};
}

private function buildAction(id:FlowActionId, scenario:Scenario, owner:ScenarioId, content:EditorFlowContentChoices):Null<FlowAction> {
	return switch id {
		case DialogueAction:
			final dialogue = firstDialogue(scenario);
			dialogue == null ? null : ShowDialogue(dialogue);
		case JournalAction:
			final journal = firstJournal(scenario);
			journal == null ? null : AddJournal(journal);
		case SetFlagAction:
			final variable = firstVariable(scenario, FlagValue);
			variable == null ? null : SetFlag(variable, true);
		case SetCounterAction:
			final variable = firstVariable(scenario, CounterValue);
			variable == null ? null : SetCounter(variable, 1);
		case AddCounterAction:
			final variable = firstVariable(scenario, CounterValue);
			variable == null ? null : AddCounter(variable, 1);
		case SetStateAction: final variable = firstVariable(scenario,
				StateValue); final state = firstStateForVariable(scenario, variable,
				content); variable == null || state == null ? null : SetState(variable, state);
		case GiveItemAction:
			inventoryAction(scenario, content, true);
		case TakeItemAction:
			inventoryAction(scenario, content, false);
		case SpawnAction:
			final objectId = firstObject(scenario);
			objectId == null ? null : Spawn(objectId);
		case DespawnAction:
			final objectId = firstObject(scenario);
			objectId == null ? null : Despawn(objectId);
		case SetObjectStateAction:
			final pair = firstStatefulObject(scenario);
			pair == null ? null : SetObjectState(pair.id, pair.state);
		case CheckpointAction:
			final checkpoint = firstCheckpoint(scenario);
			checkpoint == null ? null : SetCheckpoint(checkpoint);
		case ObjectiveAction:
			final objective = firstObjective(scenario);
			objective == null ? null : SetObjective(objective, Active);
		case EffectAction:
			final effect = firstContent(content.effects);
			effect == null ? null : PlayEffect(effect, null);
		case CampaignExitAction: RequestCampaignExit(new ScenarioId('editor.exit.${owner.text()}'));
		case SignalAction:
			final signal = firstContent(content.signals);
			signal == null ? null : EmitSignal(signal);
		case ScheduleAction:
			final sequence = firstSequence(scenario);
			sequence == null ? null : Schedule(new ScenarioId('editor.timer.${owner.text()}'), 1, sequence.id, argumentsFor(sequence.parameters));
		case CallAction:
			final sequence = firstSequence(scenario);
			sequence == null ? null : CallSequence(sequence.id, argumentsFor(sequence.parameters));
		case ChooseAction:
			final seed = firstVariable(scenario, CounterValue);
			seed == null ? null : ChooseSeeded(seed, [{weight: 1, actions: [SetCounter(seed, 1)]}]);
	};
}

private function unavailableEventReason(id:FlowEventId, scenario:Scenario, content:EditorFlowContentChoices):EditorFlowChoiceUnavailable
	return switch id {
		case EnterZoneEvent | LeaveZoneEvent: MissingZoneReference;
		case InteractEvent: MissingWorldReference;
		case BlockChangedEvent: firstZone(scenario) == null ? MissingZoneReference : MissingBlockContent;
		case UseItemEvent | ItemCollectedEvent: MissingItemContent;
		case EntityDefeatedEvent: MissingEntityReference;
		case SignalEvent: MissingSignalContent;
		case TimerEvent: MissingTimerReference;
		case ObjectiveChangedEvent: MissingObjectiveReference;
		case StateChangedEvent: MissingVariableReference(StateValue);
		case CampaignExitRequestedEvent: MissingCampaignExitReference;
		case LevelEnteredEvent: MissingWorldReference;
	};

private function unavailablePredicateReason(id:FlowPredicateId, scenario:Scenario, event:FlowEvent,
		content:EditorFlowContentChoices):EditorFlowChoiceUnavailable
	return switch id {
		case FlagPredicate: MissingVariableReference(FlagValue);
		case CounterPredicate: MissingVariableReference(CounterValue);
		case StatePredicate: firstVariable(scenario, StateValue) == null ? MissingVariableReference(StateValue) : MissingStateContent;
		case ObjectStatePredicate: MissingStatefulObjectReference;
		case InventoryPredicate: firstObject(scenario) == null ? MissingWorldReference : MissingItemContent;
		case ObjectivePredicate: MissingObjectiveReference;
		case NearPredicate: firstActor(scenario) == null ? MissingActorReference : MissingWorldReference;
		case EventActorPredicate: !flowEventSupportsActor(event) ? EventHasNoActorContext : MissingActorReference;
		case EventSweptPredicate: EventHasNoSweptContext;
		case AlwaysPredicate | AllPredicate | AnyPredicate | NotPredicate | ModePredicate: MissingWorldReference;
	};

private function unavailableActionReason(id:FlowActionId, scenario:Scenario, content:EditorFlowContentChoices, insideChoice:Bool):EditorFlowChoiceUnavailable
	return switch id {
		case DialogueAction: MissingDialogueReference;
		case JournalAction: MissingJournalReference;
		case SetFlagAction: MissingVariableReference(FlagValue);
		case SetCounterAction | AddCounterAction: MissingVariableReference(CounterValue);
		case ChooseAction: insideChoice ? NestedChoiceNotAllowed : MissingVariableReference(CounterValue);
		case SetStateAction: firstVariable(scenario, StateValue) == null ? MissingVariableReference(StateValue) : MissingStateContent;
		case GiveItemAction | TakeItemAction: firstObject(scenario) == null ? MissingWorldReference : MissingItemContent;
		case SpawnAction | DespawnAction: MissingWorldReference;
		case SetObjectStateAction: MissingStatefulObjectReference;
		case CheckpointAction: MissingCheckpointReference;
		case ObjectiveAction: MissingObjectiveReference;
		case EffectAction: MissingEffectContent;
		case SignalAction: MissingSignalContent;
		case ScheduleAction | CallAction: MissingSequenceReference;
		case CampaignExitAction: MissingCampaignExitReference;
	};

private function firstObject(scenario:Scenario):Null<ScenarioId>
	return scenario.objects.length == 0 ? null : scenario.objects[0].id;

private function firstZone(scenario:Scenario):Null<ScenarioId> {
	for (object in scenario.objects)
		switch object.placement {
			case TriggerZone(_):
				return object.id;
			case _:
		}
	return null;
}

private function firstActor(scenario:Scenario):Null<ScenarioId> {
	for (object in scenario.objects)
		switch object.placement {
			case PlayerSpawn(_) | Entity(_, _) | Npc(_, _, _):
				return object.id;
			case _:
		}
	return null;
}

private function firstEntity(scenario:Scenario):Null<ScenarioId> {
	for (object in scenario.objects)
		switch object.placement {
			case Entity(_, _):
				return object.id;
			case _:
		}
	return null;
}

private function firstCheckpoint(scenario:Scenario):Null<ScenarioId> {
	for (object in scenario.objects)
		switch object.placement {
			case Checkpoint(_):
				return object.id;
			case _:
		}
	return null;
}

private function firstStatefulObject(scenario:Scenario):Null<{final id:ScenarioId; final state:ContentId;}> {
	for (object in scenario.objects)
		switch object.placement {
			case StatefulObject(_, state, _):
				return {id: object.id, state: state};
			case _:
		}
	return null;
}

private function firstVariable(scenario:Scenario, kind:FlowValueKind):Null<ScenarioId> {
	for (variable in scenario.flow.variables)
		if (persistentScope(variable.scope) && valueKind(variable.initial) == kind)
			return variable.id;
	return null;
}

private function persistentScope(scope:FlowScope):Bool
	return switch scope {
		case Local(_): false;
		case Map | Player | Quest: true;
	};

private function valueKind(value:FlowValue):FlowValueKind
	return switch value {
		case Flag(_): FlagValue;
		case Counter(_): CounterValue;
		case State(_): StateValue;
	};

private function firstStateForVariable(scenario:Scenario, variable:Null<ScenarioId>, content:EditorFlowContentChoices):Null<ContentId> {
	final fallback = firstContent(content.states);
	var selected:Null<ContentId> = null;
	if (variable != null)
		for (candidate in scenario.flow.variables)
			if (candidate.id.text() == variable.text())
				switch candidate.initial {
					case State(value):
						selected = value;
					case _:
				}
	return selected == null ? fallback : selected;
}

private function firstDialogue(scenario:Scenario):Null<ScenarioId>
	return scenario.story.dialogues.length == 0 ? null : scenario.story.dialogues[0].id;

private function firstJournal(scenario:Scenario):Null<ScenarioId>
	return scenario.story.journal.length == 0 ? null : scenario.story.journal[0].id;

private function firstObjective(scenario:Scenario):Null<ScenarioId>
	return scenario.story.objectives.length == 0 ? null : scenario.story.objectives[0].id;

private function firstSequence(scenario:Scenario):Null<caxecraft.scenario.CaxeFlow.FlowSequence>
	return scenario.flow.sequences.length == 0 ? null : scenario.flow.sequences[0];

private function firstOwnedTimer(scenario:Scenario):Null<ScenarioId> {
	for (rule in scenario.flow.rules) {
		final value = timerInActions(rule.actions);
		if (value != null)
			return value;
	}
	for (sequence in scenario.flow.sequences) {
		final value = timerInActions(sequence.actions);
		if (value != null)
			return value;
	}
	return null;
}

private function timerInActions(actions:Array<FlowAction>):Null<ScenarioId> {
	for (action in actions)
		switch action {
			case Schedule(timer, _, _, _):
				return timer;
			case ChooseSeeded(_, choices):
				for (choice in choices) {
					final value = timerInActions(choice.actions);
					if (value != null)
						return value;
				}
			case _:
		}
	return null;
}

private function firstOwnedExit(scenario:Scenario):Null<ScenarioId> {
	for (rule in scenario.flow.rules) {
		final value = exitInActions(rule.actions);
		if (value != null)
			return value;
	}
	for (sequence in scenario.flow.sequences) {
		final value = exitInActions(sequence.actions);
		if (value != null)
			return value;
	}
	return null;
}

private function exitInActions(actions:Array<FlowAction>):Null<ScenarioId> {
	for (action in actions)
		switch action {
			case RequestCampaignExit(exit):
				return exit;
			case ChooseSeeded(_, choices):
				for (choice in choices) {
					final value = exitInActions(choice.actions);
					if (value != null)
						return value;
				}
			case _:
		}
	return null;
}

/** Collect every distinct timer or campaign-exit identity in authored order. */
private function collectOwnedReferences(scenario:Scenario, timers:Bool, result:Array<ScenarioId>):Void {
	for (rule in scenario.flow.rules)
		collectOwnedReferencesInActions(rule.actions, timers, result);
	for (sequence in scenario.flow.sequences)
		collectOwnedReferencesInActions(sequence.actions, timers, result);
}

/** Walk weighted actions because their delayed events are ordinary document IDs. */
private function collectOwnedReferencesInActions(actions:Array<FlowAction>, timers:Bool, result:Array<ScenarioId>):Void {
	for (action in actions)
		switch action {
			case Schedule(timer, _, _, _) if (timers):
				pushUniqueId(result, timer);
			case RequestCampaignExit(exit) if (!timers):
				pushUniqueId(result, exit);
			case ChooseSeeded(_, choices):
				for (choice in choices)
					collectOwnedReferencesInActions(choice.actions, timers, result);
			case _:
		}
}

/** Keep picker rows stable when several rules observe one owned event identity. */
private function pushUniqueId(result:Array<ScenarioId>, value:ScenarioId):Void {
	for (existing in result)
		if (existing.text() == value.text())
			return;
	result.push(value);
}

private function argumentsFor(parameters:Array<caxecraft.scenario.CaxeFlow.FlowParameter>):Array<FlowArgument>
	return [for (parameter in parameters) Value(parameter.initial)];

private function firstContent(values:Array<ContentId>):Null<ContentId>
	return values.length == 0 ? null : values[0];

private function contentEvent(value:Null<ContentId>, use:Bool):Null<FlowEvent>
	return value == null ? null : use ? UseItem(value) : ItemCollected(value);

private function inventoryAction(scenario:Scenario, content:EditorFlowContentChoices, give:Bool):Null<FlowAction> {
	final owner = firstObject(scenario);
	final item = firstContent(content.items);
	return owner == null || item == null ? null : give ? GiveItem(owner, item, 1) : TakeItem(owner, item, 1);
}

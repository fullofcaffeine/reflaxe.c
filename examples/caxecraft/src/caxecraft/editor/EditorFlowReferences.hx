package caxecraft.editor;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowValueKind;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;

/**
	Owns the one typed traversal of references inside CaxeFlow values.

	Cards use this traversal to display references, and picker edits use the same
	traversal to replace them. Keeping those two operations here means a new event,
	predicate, or action cannot acquire two subtly different field orders or roles.
	The mapper returns the ID to keep, so callers can observe without mutation or
	construct a changed copy without a second interpretation of the Flow value.
**/
/** Stable semantic roles that let cards choose a world or document picker. */
enum EditorFlowReferenceRole {
	ZoneFlowReference;
	WorldObjectFlowReference;
	ActorFlowReference;
	EntityFlowReference;
	StatefulObjectFlowReference;
	CheckpointFlowReference;
	InventoryOwnerFlowReference;
	TypedVariableFlowReference(kind:FlowValueKind);
	AnyVariableFlowReference;
	ObjectiveFlowReference;
	DialogueFlowReference;
	JournalFlowReference;
	SequenceFlowReference;
	TimerFlowReference;
	CampaignExitFlowReference;
	LevelFlowReference;
}

/** One typed reference shown by a card; its role selects the correct picker. */
typedef EditorFlowReference = {
	final id:ScenarioId;
	final role:EditorFlowReferenceRole;
}

/** Result of replacing one depth-first reference without mutating the source. */
typedef EditorFlowReferenceReplacement<T> = {
	final value:T;
	final found:Bool;
	final accepted:Bool;
	final changed:Bool;
	final ?expected:EditorFlowReferenceRole;
}

/** Collect event references in the same stable order used by picker edits. */
function collectEventFlowReferences(event:FlowEvent):Array<EditorFlowReference>
	return collectEvent(event);

/** Collect nested predicate references in depth-first picker order. */
function collectPredicateFlowReferences(predicate:FlowPredicate):Array<EditorFlowReference>
	return collectPredicate(predicate);

/** Collect action and nested-choice references in depth-first picker order. */
function collectActionFlowReferences(action:FlowAction):Array<EditorFlowReference>
	return collectAction(action);

/** Replace one event reference in the same order used by card projection. */
function replaceEventFlowReference(event:FlowEvent, target:Int, replacement:ScenarioId,
		roles:Array<EditorFlowReferenceRole>):EditorFlowReferenceReplacement<FlowEvent> {
	final cursor = replacementCursor(target, replacement, roles, null);
	return replacementResult(mapEventReferences(event, cursor), cursor);
}

/** Replace one nested predicate reference in depth-first card order. */
function replacePredicateFlowReference(predicate:FlowPredicate, target:Int, replacement:ScenarioId,
		roles:Array<EditorFlowReferenceRole>):EditorFlowReferenceReplacement<FlowPredicate> {
	final cursor = replacementCursor(target, replacement, roles, null);
	return replacementResult(mapPredicateReferences(predicate, cursor), cursor);
}

/** Replace one action reference, including references in nested choices. */
function replaceActionFlowReference(action:FlowAction, target:Int, replacement:ScenarioId,
		roles:Array<EditorFlowReferenceRole>):EditorFlowReferenceReplacement<FlowAction> {
	final cursor = replacementCursor(target, replacement, roles, null);
	return replacementResult(mapActionReferences(action, cursor), cursor);
}

/**
	Replace one action reference selected from the current document.

	When the selected identity is a sequence, its arguments are rebuilt from that
	sequence's typed parameter defaults. The editor can therefore never retain an
	argument list that belonged to a different sequence schema.
**/
function replaceActionDocumentFlowReference(action:FlowAction, target:Int, replacement:ScenarioId, role:EditorFlowReferenceRole,
		scenario:Scenario):EditorFlowReferenceReplacement<FlowAction> {
	final cursor = replacementCursor(target, replacement, [role], scenario);
	return replacementResult(mapActionReferences(action, cursor), cursor);
}

/** Map every scenario reference in one event while preserving its typed shape. */
private function mapEventReferences(event:FlowEvent, cursor:EditorFlowReferenceCursor):FlowEvent
	return switch event {
		case EnterZone(zone): EnterZone(replaceOne(zone, ZoneFlowReference, cursor));
		case LeaveZone(zone): LeaveZone(replaceOne(zone, ZoneFlowReference, cursor));
		case Interact(objectId): Interact(replaceOne(objectId, WorldObjectFlowReference, cursor));
		case BlockChanged(zone, blockType): BlockChanged(replaceOne(zone, ZoneFlowReference, cursor), blockType);
		case EntityDefeated(entity): EntityDefeated(replaceOne(entity, EntityFlowReference, cursor));
		case TimerExpired(timer): TimerExpired(replaceOne(timer, TimerFlowReference, cursor));
		case ObjectiveChanged(objective): ObjectiveChanged(replaceOne(objective, ObjectiveFlowReference, cursor));
		case StateChanged(variable): StateChanged(replaceOne(variable, AnyVariableFlowReference, cursor));
		case LevelEntered(level): LevelEntered(replaceOne(level, LevelFlowReference, cursor));
		case CampaignExitRequested(exit): CampaignExitRequested(replaceOne(exit, CampaignExitFlowReference, cursor));
		case UseItem(itemType): UseItem(itemType);
		case ItemCollected(itemType): ItemCollected(itemType);
		case SignalReceived(signal): SignalReceived(signal);
	};

/** Map every nested condition reference while retaining predicate structure. */
private function mapPredicateReferences(predicate:FlowPredicate, cursor:EditorFlowReferenceCursor):FlowPredicate
	return switch predicate {
		case Always: Always;
		case All(children): All([for (child in children) mapPredicateReferences(child, cursor)]);
		case AnyOf(children): AnyOf([for (child in children) mapPredicateReferences(child, cursor)]);
		case Not(child): Not(mapPredicateReferences(child, cursor));
		case FlagIs(variable, expected): FlagIs(replaceOne(variable, TypedVariableFlowReference(FlagValue), cursor), expected);
		case CounterCompare(variable, comparison, value):
			CounterCompare(replaceOne(variable, TypedVariableFlowReference(CounterValue), cursor), comparison, value);
		case StateIs(variable, expected): StateIs(replaceOne(variable, TypedVariableFlowReference(StateValue), cursor), expected);
		case ObjectStateIs(objectId, expected): ObjectStateIs(replaceOne(objectId, StatefulObjectFlowReference, cursor), expected);
		case InventoryHas(owner, itemType, comparison, quantity):
			InventoryHas(replaceOne(owner, InventoryOwnerFlowReference, cursor), itemType, comparison, quantity);
		case ObjectiveIs(objective, expected): ObjectiveIs(replaceOne(objective, ObjectiveFlowReference, cursor), expected);
		case NearObject(actor, objectId, maximumMilliBlocks):
			NearObject(replaceOne(actor, ActorFlowReference, cursor), replaceOne(objectId, WorldObjectFlowReference, cursor), maximumMilliBlocks);
		case ModeIs(mode): ModeIs(mode);
		case EventActorIs(actor): EventActorIs(replaceOne(actor, ActorFlowReference, cursor));
		case EventSweptIs(expected): EventSweptIs(expected);
	};

/** Map every action reference, including sequence variables and nested choices. */
private function mapActionReferences(action:FlowAction, cursor:EditorFlowReferenceCursor):FlowAction
	return switch action {
		case ShowDialogue(dialogue): ShowDialogue(replaceOne(dialogue, DialogueFlowReference, cursor));
		case AddJournal(entry): AddJournal(replaceOne(entry, JournalFlowReference, cursor));
		case SetFlag(variable, value): SetFlag(replaceOne(variable, TypedVariableFlowReference(FlagValue), cursor), value);
		case SetCounter(variable, value): SetCounter(replaceOne(variable, TypedVariableFlowReference(CounterValue), cursor), value);
		case AddCounter(variable, delta): AddCounter(replaceOne(variable, TypedVariableFlowReference(CounterValue), cursor), delta);
		case SetState(variable, value): SetState(replaceOne(variable, TypedVariableFlowReference(StateValue), cursor), value);
		case GiveItem(owner, itemType, quantity): GiveItem(replaceOne(owner, InventoryOwnerFlowReference, cursor), itemType, quantity);
		case TakeItem(owner, itemType, quantity): TakeItem(replaceOne(owner, InventoryOwnerFlowReference, cursor), itemType, quantity);
		case Spawn(objectId): Spawn(replaceOne(objectId, WorldObjectFlowReference, cursor));
		case Despawn(objectId): Despawn(replaceOne(objectId, WorldObjectFlowReference, cursor));
		case SetObjectState(objectId, value): SetObjectState(replaceOne(objectId, StatefulObjectFlowReference, cursor), value);
		case SetCheckpoint(checkpoint): SetCheckpoint(replaceOne(checkpoint, CheckpointFlowReference, cursor));
		case SetObjective(objective, value): SetObjective(replaceOne(objective, ObjectiveFlowReference, cursor), value);
		case PlayEffect(effect, objectId):
			PlayEffect(effect, objectId == null ? null : replaceOne(objectId, WorldObjectFlowReference, cursor));
		case RequestCampaignExit(exit): RequestCampaignExit(replaceOne(exit, CampaignExitFlowReference, cursor));
		case EmitSignal(signal): EmitSignal(signal);
		case Schedule(timer, ticks, sequence, arguments):
			final nextTimer = replaceOne(timer, TimerFlowReference, cursor);
			final nextSequence = replaceOne(sequence, SequenceFlowReference, cursor);
			final nextArguments = mapArgumentReferences(arguments, cursor);
			Schedule(nextTimer, ticks, nextSequence, rebindSequenceArguments(sequence, nextSequence, nextArguments, cursor));
		case CallSequence(sequence, arguments):
			final nextSequence = replaceOne(sequence, SequenceFlowReference, cursor);
			final nextArguments = mapArgumentReferences(arguments, cursor);
			CallSequence(nextSequence, rebindSequenceArguments(sequence, nextSequence, nextArguments, cursor));
		case ChooseSeeded(seedVariable, choices):
			ChooseSeeded(replaceOne(seedVariable, TypedVariableFlowReference(CounterValue), cursor), [
				for (choice in choices)
					{
						weight: choice.weight,
						actions: [for (nested in choice.actions) mapActionReferences(nested, cursor)]
					}
			]);
	};

/** Observe one event through the same traversal used by replacement. */
private function collectEvent(event:FlowEvent):Array<EditorFlowReference> {
	final cursor = collectionCursor();
	mapEventReferences(event, cursor);
	return cursor.collected;
}

/** Observe one predicate through the same traversal used by replacement. */
private function collectPredicate(predicate:FlowPredicate):Array<EditorFlowReference> {
	final cursor = collectionCursor();
	mapPredicateReferences(predicate, cursor);
	return cursor.collected;
}

/** Observe one action through the same traversal used by replacement. */
private function collectAction(action:FlowAction):Array<EditorFlowReference> {
	final cursor = collectionCursor();
	mapActionReferences(action, cursor);
	return cursor.collected;
}

/** Map variable-valued sequence arguments; literal values contain no identity. */
private function mapArgumentReferences(arguments:Array<FlowArgument>, cursor:EditorFlowReferenceCursor):Array<FlowArgument>
	return [
		for (argument in arguments)
			switch argument {
				case Value(value):
					Value(value);
				case Variable(variable):
					Variable(replaceOne(variable, AnyVariableFlowReference, cursor));
			}
	];

/** Mutable traversal state shared only for the duration of one public call. */
private typedef EditorFlowReferenceCursor = {
	final target:Int;
	var seen:Int;
	final replacement:Null<ScenarioId>;
	final roles:Array<EditorFlowReferenceRole>;
	var expected:Null<EditorFlowReferenceRole>;
	var accepted:Bool;
	var changed:Bool;
	final collected:Array<EditorFlowReference>;
	final scenario:Null<Scenario>;
}

private function replacementCursor(target:Int, replacement:ScenarioId, roles:Array<EditorFlowReferenceRole>, scenario:Null<Scenario>):EditorFlowReferenceCursor
	return {
		target: target,
		seen: 0,
		replacement: replacement,
		roles: roles,
		expected: null,
		accepted: false,
		changed: false,
		collected: [],
		scenario: scenario
	};

private function collectionCursor():EditorFlowReferenceCursor
	return {
		target: -1,
		seen: 0,
		replacement: null,
		roles: [],
		expected: null,
		accepted: false,
		changed: false,
		collected: [],
		scenario: null
	};

/** Rebuild arguments only when this exact sequence field changed. */
private function rebindSequenceArguments(original:ScenarioId, replacement:ScenarioId, mapped:Array<FlowArgument>,
		cursor:EditorFlowReferenceCursor):Array<FlowArgument> {
	if (original.text() == replacement.text() || cursor.scenario == null)
		return mapped;
	final rebound:Array<FlowArgument> = [];
	var found = false;
	for (sequence in cursor.scenario.flow.sequences)
		if (sequence.id.text() == replacement.text()) {
			found = true;
			for (parameter in sequence.parameters)
				rebound.push(Value(parameter.initial));
		}
	return found ? rebound : mapped;
}

private function replacementResult<T>(value:T, cursor:EditorFlowReferenceCursor):EditorFlowReferenceReplacement<T>
	return {
		value: value,
		found: cursor.expected != null,
		accepted: cursor.accepted,
		changed: cursor.changed,
		expected: cursor.expected
	};

/** Record one stable traversal position and replace it when its role permits. */
private function replaceOne(original:ScenarioId, role:EditorFlowReferenceRole, cursor:EditorFlowReferenceCursor):ScenarioId {
	cursor.collected.push({id: original, role: role});
	final current = cursor.seen;
	cursor.seen++;
	if (current != cursor.target)
		return original;
	cursor.expected = role;
	cursor.accepted = supportsRole(cursor.roles, role);
	final replacement = cursor.replacement;
	if (!cursor.accepted || replacement == null || original.text() == replacement.text())
		return original;
	cursor.changed = true;
	return replacement;
}

private function supportsRole(roles:Array<EditorFlowReferenceRole>, expected:EditorFlowReferenceRole):Bool {
	for (role in roles)
		if (sameRole(role, expected))
			return true;
	return false;
}

private function sameRole(left:EditorFlowReferenceRole, right:EditorFlowReferenceRole):Bool
	return switch [left, right] {
		case [ZoneFlowReference, ZoneFlowReference] | [WorldObjectFlowReference, WorldObjectFlowReference] | [ActorFlowReference, ActorFlowReference] |
			[EntityFlowReference, EntityFlowReference] | [StatefulObjectFlowReference, StatefulObjectFlowReference] |
			[CheckpointFlowReference, CheckpointFlowReference] | [InventoryOwnerFlowReference, InventoryOwnerFlowReference] |
			[AnyVariableFlowReference, AnyVariableFlowReference] | [ObjectiveFlowReference, ObjectiveFlowReference] |
			[DialogueFlowReference, DialogueFlowReference] | [JournalFlowReference, JournalFlowReference] | [SequenceFlowReference, SequenceFlowReference] |
			[TimerFlowReference, TimerFlowReference] | [CampaignExitFlowReference, CampaignExitFlowReference] | [LevelFlowReference, LevelFlowReference]: true;
		case [TypedVariableFlowReference(leftKind), TypedVariableFlowReference(rightKind)]: leftKind == rightKind;
		case _: false;
	};

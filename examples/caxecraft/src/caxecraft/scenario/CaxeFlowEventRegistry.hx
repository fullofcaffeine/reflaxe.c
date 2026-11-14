package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowEventContext;
import caxecraft.scenario.CaxeFlow.FlowEventOccurrence;

/**
	Owns the inspectable catalog for CaxeFlow event sources and runtime context.

	`FlowEvent` remains the closed authored source value. This module gives the
	parser, validator, executor, editor, and trace one shared answer for syntax,
	argument meaning, and required runtime context. Engine adapters may construct
	these typed values, but content cannot register callbacks or payload fields.
**/
enum abstract FlowEventId(String) {
	var EnterZoneEvent = "enter-zone";
	var LeaveZoneEvent = "leave-zone";
	var InteractEvent = "interact";
	var BlockChangedEvent = "block-changed";
	var UseItemEvent = "use-item";
	var ItemCollectedEvent = "collect-item";
	var EntityDefeatedEvent = "entity-defeated";
	var SignalEvent = "signal";
	var TimerEvent = "timer";
	var ObjectiveChangedEvent = "objective-changed";
	var StateChangedEvent = "state-changed";
	var LevelEnteredEvent = "level-entered";
	var CampaignExitRequestedEvent = "campaign-exit-requested";

	/** Return the exact external spelling used in canonical CAXEMAP bytes. */
	public inline function text():String
		return this;
}

/** Closed world/content role used to select the correct editor picker. */
enum FlowEventArgumentRole {
	ZoneReference;
	ObjectReference;
	BlockContent;
	ItemContent;
	EntityReference;
	SignalContent;
	TimerReference;
	ObjectiveReference;
	VariableReference;
	LevelReference;
	CampaignExitReference;
}

/** Exact runtime context admitted for one event source. */
enum FlowEventContextKind {
	NoContext;
	RequiredActorContext;
	OptionalActorContext;
	SpatialActorContext;
}

/** Immutable source description shared by syntax, validation, cards, and trace. */
typedef FlowEventDescriptor = {
	final id:FlowEventId;
	final arguments:Array<FlowEventArgumentRole>;
	final context:FlowEventContextKind;
	final editorLabel:MessageId;
	final editorHelp:MessageId;
	final traceName:String;
}

/** Exact reason the fixed descriptor inventory is unsafe to use. */
enum FlowEventRegistryError {
	DuplicateEventId(id:FlowEventId);
	MissingEventId(id:FlowEventId);
	WrongEventDescriptor(id:FlowEventId);
}

/** Canonical order used by text completion, visual cards, and deterministic QA. */
function allFlowEventIds():Array<FlowEventId>
	return [
		EnterZoneEvent,
		LeaveZoneEvent,
		InteractEvent,
		BlockChangedEvent,
		UseItemEvent,
		ItemCollectedEvent,
		EntityDefeatedEvent,
		SignalEvent,
		TimerEvent,
		ObjectiveChangedEvent,
		StateChangedEvent,
		LevelEnteredEvent,
		CampaignExitRequestedEvent
	];

/** Resolve one immutable event descriptor without a mutable global registry. */
function flowEventDescriptorById(id:FlowEventId):FlowEventDescriptor {
	final arguments = switch id {
		case EnterZoneEvent | LeaveZoneEvent: [ZoneReference];
		case InteractEvent: [ObjectReference];
		case BlockChangedEvent: [ZoneReference, BlockContent];
		case UseItemEvent | ItemCollectedEvent: [ItemContent];
		case EntityDefeatedEvent: [EntityReference];
		case SignalEvent: [SignalContent];
		case TimerEvent: [TimerReference];
		case ObjectiveChangedEvent: [ObjectiveReference];
		case StateChangedEvent: [VariableReference];
		case LevelEnteredEvent: [LevelReference];
		case CampaignExitRequestedEvent: [CampaignExitReference];
	};
	final context = switch id {
		case EnterZoneEvent | LeaveZoneEvent: SpatialActorContext;
		case InteractEvent | UseItemEvent | ItemCollectedEvent: RequiredActorContext;
		case BlockChangedEvent | EntityDefeatedEvent: OptionalActorContext;
		case SignalEvent | TimerEvent | ObjectiveChangedEvent | StateChangedEvent | LevelEnteredEvent | CampaignExitRequestedEvent: NoContext;
	};
	final syntax = id.text();
	return {
		id: id,
		arguments: arguments,
		context: context,
		editorLabel: new MessageId('editor.event.$syntax.label'),
		editorHelp: new MessageId('editor.event.$syntax.help'),
		traceName: syntax
	};
}

/** Build the complete event catalog in stable authoring order. */
function allFlowEventDescriptors():Array<FlowEventDescriptor>
	return [for (id in allFlowEventIds()) flowEventDescriptorById(id)];

/** Unknown syntax remains unknown; the parser never guesses another source. */
function flowEventDescriptorForSyntax(syntax:String):Null<FlowEventDescriptor> {
	for (id in allFlowEventIds())
		if (id.text() == syntax)
			return flowEventDescriptorById(id);
	return null;
}

/** Link one closed authored source to its shared descriptor identity. */
function flowEventId(source:FlowEvent):FlowEventId
	return switch source {
		case EnterZone(_): EnterZoneEvent;
		case LeaveZone(_): LeaveZoneEvent;
		case Interact(_): InteractEvent;
		case BlockChanged(_, _): BlockChangedEvent;
		case UseItem(_): UseItemEvent;
		case ItemCollected(_): ItemCollectedEvent;
		case EntityDefeated(_): EntityDefeatedEvent;
		case SignalReceived(_): SignalEvent;
		case TimerExpired(_): TimerEvent;
		case ObjectiveChanged(_): ObjectiveChangedEvent;
		case StateChanged(_): StateChangedEvent;
		case LevelEntered(_): LevelEnteredEvent;
		case CampaignExitRequested(_): CampaignExitRequestedEvent;
	};

/** Resolve metadata for a source without inspecting its payload twice. */
function flowEventDescriptor(source:FlowEvent):FlowEventDescriptor
	return flowEventDescriptorById(flowEventId(source));

/** Construct one occurrence. Validation remains explicit at the engine boundary. */
function flowEventOccurrence(source:FlowEvent, context:FlowEventContext = NoEventContext):FlowEventOccurrence
	return {source: source, context: context};

/** True only when runtime context has the descriptor-required closed shape. */
function flowEventContextMatches(source:FlowEvent, context:FlowEventContext):Bool
	return switch [flowEventDescriptor(source).context, context] {
		case [NoContext, NoEventContext]: true;
		case [RequiredActorContext, ActorEventContext(_)]: true;
		case [OptionalActorContext, NoEventContext] | [OptionalActorContext, ActorEventContext(_)]: true;
		case [SpatialActorContext, SpatialEventContext(_, _, _, _)]: true;
		case _: false;
	};

/** Return the actor carried by this occurrence, when its source admits one. */
function flowEventActor(occurrence:FlowEventOccurrence):Null<ScenarioId>
	return switch occurrence.context {
		case NoEventContext: null;
		case ActorEventContext(actor) | SpatialEventContext(actor, _, _, _): actor;
	};

/** True when one spatial event records an outside-to-outside swept crossing. */
function flowEventWasSwept(occurrence:FlowEventOccurrence):Bool
	return switch occurrence.context {
		case SpatialEventContext(_, _, _, swept): swept;
		case _: false;
	};

/** True when this source can supply an actor to predicates and per-actor policy. */
function flowEventSupportsActor(source:FlowEvent):Bool
	return flowEventDescriptor(source).context != NoContext;

/** True only for enter/leave sources that carry deterministic movement context. */
function flowEventSupportsSwept(source:FlowEvent):Bool
	return flowEventDescriptor(source).context == SpatialActorContext;

/**
	Return slots available to non-spatial adapters after reserving swept movement.

	Each zone/actor pair can emit enter and leave on one fast segment. Validation
	requires this result to stay positive, so runtime queue admission cannot make
	two separate segments collapse into one later crossing.
**/
function externalFlowEventCapacity(zoneCount:Int, actorCount:Int):Int {
	final reserved = zoneCount * actorCount * 2;
	final available = ScenarioLimits.MAX_EVENTS_PER_TICK - reserved;
	return available > 0 ? available : 0;
}

/** Match two authored source patterns without comparing runtime context. */
function flowEventSourcesMatch(expected:FlowEvent, actual:FlowEvent):Bool
	return switch [expected, actual] {
		case [EnterZone(left), EnterZone(right)] | [LeaveZone(left), LeaveZone(right)] | [Interact(left), Interact(right)] |
			[EntityDefeated(left), EntityDefeated(right)] | [TimerExpired(left), TimerExpired(right)] | [ObjectiveChanged(left), ObjectiveChanged(right)] |
			[StateChanged(left), StateChanged(right)] | [LevelEntered(left), LevelEntered(right)] | [CampaignExitRequested(left), CampaignExitRequested(right)]:
			sameScenarioId(left, right);
		case [BlockChanged(leftZone, leftBlock), BlockChanged(rightZone, rightBlock)]: sameScenarioId(leftZone,
				rightZone) && sameContentId(leftBlock, rightBlock);
		case [UseItem(left), UseItem(right)] | [ItemCollected(left), ItemCollected(right)] | [SignalReceived(left), SignalReceived(right)]:
			sameContentId(left, right);
		case _: false;
	};

/** Validate descriptor uniqueness and every closed constructor mapping. */
function validateFlowEventDescriptors(values:Array<FlowEventDescriptor>):Array<FlowEventRegistryError> {
	final errors:Array<FlowEventRegistryError> = [];
	final seen:Map<String, Bool> = [];
	for (descriptor in values) {
		final text = descriptor.id.text();
		if (seen.exists(text))
			errors.push(DuplicateEventId(descriptor.id));
		seen.set(text, true);
		final expected = flowEventDescriptorById(descriptor.id);
		if (!sameDescriptor(descriptor, expected))
			errors.push(WrongEventDescriptor(descriptor.id));
	}
	for (id in allFlowEventIds())
		if (!seen.exists(id.text()))
			errors.push(MissingEventId(id));
	return errors;
}

private function sameDescriptor(left:FlowEventDescriptor, right:FlowEventDescriptor):Bool {
	if (left.id != right.id
		|| left.context != right.context
		|| left.traceName != right.traceName
		|| left.editorLabel.text() != right.editorLabel.text()
		|| left.editorHelp.text() != right.editorHelp.text()
		|| left.arguments.length != right.arguments.length)
		return false;
	for (index in 0...left.arguments.length)
		if (left.arguments[index] != right.arguments[index])
			return false;
	return true;
}

private inline function sameScenarioId(left:ScenarioId, right:ScenarioId):Bool
	return left.text() == right.text();

private inline function sameContentId(left:ContentId, right:ContentId):Bool
	return left.text() == right.text();

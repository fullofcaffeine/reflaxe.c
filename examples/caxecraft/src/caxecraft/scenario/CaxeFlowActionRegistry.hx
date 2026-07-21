package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowValueKind;

/**
	Owns the inspectable catalog for CaxeFlow actions.

	The typed `FlowAction` enum still owns action payloads and exhaustive engine
	execution. This module gives text-facing tools one shared answer for syntax,
	argument order, editor presentation, and authoring authority. Consumers query
	the closed records below; content cannot register callbacks or execute code.
	`FlowActionId` begins that catalog with the stable CAXEMAP spelling for each
	closed action constructor.
**/
enum abstract FlowActionId(String) {
	var DialogueAction = "dialogue";
	var JournalAction = "journal";
	var SetFlagAction = "set-flag";
	var SetCounterAction = "set-counter";
	var AddCounterAction = "add-counter";
	var SetStateAction = "set-state";
	var GiveItemAction = "give-item";
	var TakeItemAction = "take-item";
	var SpawnAction = "spawn";
	var DespawnAction = "despawn";
	var SetObjectStateAction = "set-object-state";
	var CheckpointAction = "checkpoint";
	var ObjectiveAction = "objective";
	var EffectAction = "effect";
	var SignalAction = "signal";
	var ScheduleAction = "schedule";
	var CallAction = "call";
	var ChooseAction = "choose";

	/** Return the exact external spelling used in canonical CAXEMAP bytes. */
	public inline function text():String
		return this;
}

/** The part of the engine whose state or control flow an action changes. */
enum FlowActionFamily {
	WorldAction;
	PresentationAction;
	ControlAction;
}

/** A possible authoring surface that must receive explicit permission. */
enum FlowActionConsumer {
	CaxeFlowDocument;
	CutsceneDocument;
	DeveloperConsole;
	CaxeTestSetup;
}

/** Closed scenario-identity roles, kept separate from content registry keys. */
enum FlowScenarioArgumentRole {
	DialogueReference;
	JournalReference;
	VariableReference(kind:FlowValueKind);
	InventoryOwnerReference;
	ObjectReference;
	CheckpointReference;
	ObjectiveReference;
	TimerReference;
	SequenceReference;
	SeedVariableReference;
}

/** Closed content-registry roles. These are not scenario object identities. */
enum FlowContentArgumentRole {
	StateContent;
	ItemContent;
	EffectContent;
	SignalContent;
}

/** Meaning of an integer slot, used by the editor for the right input control. */
enum FlowIntegerArgumentRole {
	CounterValue;
	CounterDelta;
	Quantity;
	DelayTicks;
}

/** One position in the ordered argument list shown to tools and authors. */
enum FlowActionArgumentRole {
	ScenarioArgument(role:FlowScenarioArgumentRole);
	ContentArgument(role:FlowContentArgumentRole);
	IntegerArgument(role:FlowIntegerArgumentRole);
	FlagArgument;
	ObjectiveStateArgument;
	OptionalScenarioArgument(role:FlowScenarioArgumentRole);
	SequenceArguments;
	WeightedChoices;
}

/**
	Closed shapes admitted by CAXEMAP 1.

	The role order is derived by `flowActionArgumentRoles`. A visual editor can
	therefore render a form without guessing that two plain strings mean
	different things such as an object ID and an item type.
**/
enum FlowActionSchema {
	OneScenario(role:FlowScenarioArgumentRole);
	ScenarioAndFlag(role:FlowScenarioArgumentRole);
	ScenarioAndInteger(role:FlowScenarioArgumentRole, integer:FlowIntegerArgumentRole);
	ScenarioAndContent(role:FlowScenarioArgumentRole, content:FlowContentArgumentRole);
	ScenarioContentInteger(role:FlowScenarioArgumentRole, content:FlowContentArgumentRole, integer:FlowIntegerArgumentRole);
	OneContent(role:FlowContentArgumentRole);
	EffectWithOptionalObject;
	ObjectiveTransition;
	ScheduleSequence;
	SequenceCall;
	SeededChoices;
}

/**
	Explicit authority for each supported authoring surface.

	Only CaxeFlow documents exist today. The other fields are deliberately false
	until their owning features add validation and execution evidence.
**/
typedef FlowActionConsumers = {
	final caxeFlow:Bool;
	final cutscene:Bool;
	final console:Bool;
	final testSetup:Bool;
}

/** Immutable description shared by syntax, validation, and editor tooling. */
typedef FlowActionDescriptor = {
	final id:FlowActionId;
	final family:FlowActionFamily;
	final schema:FlowActionSchema;
	final consumers:FlowActionConsumers;
	final editorLabel:MessageId;
	final editorHelp:MessageId;
	final traceName:String;
}

/** Exact reason a descriptor inventory is not safe to use. */
enum FlowActionRegistryError {
	DuplicateActionId(id:FlowActionId);
	MissingActionId(id:FlowActionId);
	WrongActionSchema(id:FlowActionId);
	MissingCaxeFlowAuthority(id:FlowActionId);
	UnimplementedConsumerAuthority(id:FlowActionId, consumer:FlowActionConsumer);
	WrongEditorMessages(id:FlowActionId);
	WrongTraceName(id:FlowActionId);
}

/** Canonical order used by validation, editor palettes, and deterministic QA. */
function allFlowActionIds():Array<FlowActionId>
	return [
		DialogueAction,
		JournalAction,
		SetFlagAction,
		SetCounterAction,
		AddCounterAction,
		SetStateAction,
		GiveItemAction,
		TakeItemAction,
		SpawnAction,
		DespawnAction,
		SetObjectStateAction,
		CheckpointAction,
		ObjectiveAction,
		EffectAction,
		SignalAction,
		ScheduleAction,
		CallAction,
		ChooseAction
	];

/** Resolve one descriptor without exposing a mutable global registry. */
function flowActionDescriptorById(id:FlowActionId):FlowActionDescriptor {
	final family = switch id {
		case DialogueAction, EffectAction: PresentationAction;
		case SignalAction, ScheduleAction, CallAction, ChooseAction: ControlAction;
		case _: WorldAction;
	};
	final schema = switch id {
		case DialogueAction: OneScenario(DialogueReference);
		case JournalAction: OneScenario(JournalReference);
		case SetFlagAction: ScenarioAndFlag(VariableReference(FlagValue));
		case SetCounterAction: ScenarioAndInteger(VariableReference(CounterValue), CounterValue);
		case AddCounterAction: ScenarioAndInteger(VariableReference(CounterValue), CounterDelta);
		case SetStateAction: ScenarioAndContent(VariableReference(StateValue), StateContent);
		case GiveItemAction, TakeItemAction: ScenarioContentInteger(InventoryOwnerReference, ItemContent, Quantity);
		case SpawnAction, DespawnAction: OneScenario(ObjectReference);
		case SetObjectStateAction: ScenarioAndContent(ObjectReference, StateContent);
		case CheckpointAction: OneScenario(CheckpointReference);
		case ObjectiveAction: ObjectiveTransition;
		case EffectAction: EffectWithOptionalObject;
		case SignalAction: OneContent(SignalContent);
		case ScheduleAction: ScheduleSequence;
		case CallAction: SequenceCall;
		case ChooseAction: SeededChoices;
	};
	final syntax = id.text();
	return {
		id: id,
		family: family,
		schema: schema,
		consumers: {
			caxeFlow: true,
			cutscene: false,
			console: false,
			testSetup: false
		},
		editorLabel: new MessageId('editor.action.$syntax.label'),
		editorHelp: new MessageId('editor.action.$syntax.help'),
		traceName: syntax
	};
}

/** Build the complete catalog in its stable editor and validation order. */
function allFlowActionDescriptors():Array<FlowActionDescriptor>
	return [for (id in allFlowActionIds()) flowActionDescriptorById(id)];

/** Unknown text stays unknown; no fallback action is guessed. */
function flowActionDescriptorForSyntax(syntax:String):Null<FlowActionDescriptor> {
	for (id in allFlowActionIds())
		if (id.text() == syntax)
			return flowActionDescriptorById(id);
	return null;
}

/** Link one typed action value to its external descriptor. */
function flowActionId(action:FlowAction):FlowActionId
	return switch action {
		case ShowDialogue(_): DialogueAction;
		case AddJournal(_): JournalAction;
		case SetFlag(_, _): SetFlagAction;
		case SetCounter(_, _): SetCounterAction;
		case AddCounter(_, _): AddCounterAction;
		case SetState(_, _): SetStateAction;
		case GiveItem(_, _, _): GiveItemAction;
		case TakeItem(_, _, _): TakeItemAction;
		case Spawn(_): SpawnAction;
		case Despawn(_): DespawnAction;
		case SetObjectState(_, _): SetObjectStateAction;
		case SetCheckpoint(_): CheckpointAction;
		case SetObjective(_, _): ObjectiveAction;
		case PlayEffect(_, _): EffectAction;
		case EmitSignal(_): SignalAction;
		case Schedule(_, _, _, _): ScheduleAction;
		case CallSequence(_, _): CallAction;
		case ChooseSeeded(_, _): ChooseAction;
	};

/** Resolve metadata for a typed action without re-reading its payload. */
function flowActionDescriptor(action:FlowAction):FlowActionDescriptor
	return flowActionDescriptorById(flowActionId(action));

/** The ordered form fields for one closed action schema. */
function flowActionArgumentRoles(schema:FlowActionSchema):Array<FlowActionArgumentRole>
	return switch schema {
		case OneScenario(role): [ScenarioArgument(role)];
		case ScenarioAndFlag(role): [ScenarioArgument(role), FlagArgument];
		case ScenarioAndInteger(role, integer): [ScenarioArgument(role), IntegerArgument(integer)];
		case ScenarioAndContent(role, content): [ScenarioArgument(role), ContentArgument(content)];
		case ScenarioContentInteger(role, content, integer):
			[ScenarioArgument(role), ContentArgument(content), IntegerArgument(integer)];
		case OneContent(role): [ContentArgument(role)];
		case EffectWithOptionalObject: [ContentArgument(EffectContent), OptionalScenarioArgument(ObjectReference)];
		case ObjectiveTransition: [ScenarioArgument(ObjectiveReference), ObjectiveStateArgument];
		case ScheduleSequence:
			[
				ScenarioArgument(TimerReference),
				IntegerArgument(DelayTicks),
				ScenarioArgument(SequenceReference),
				SequenceArguments
			];
		case SequenceCall: [ScenarioArgument(SequenceReference), SequenceArguments];
		case SeededChoices: [ScenarioArgument(SeedVariableReference), WeightedChoices];
	};

/** Check the descriptor's explicit permission for one authoring surface. */
function flowActionAllowed(descriptor:FlowActionDescriptor, consumer:FlowActionConsumer):Bool
	return switch consumer {
		case CaxeFlowDocument: descriptor.consumers.caxeFlow;
		case CutsceneDocument: descriptor.consumers.cutscene;
		case DeveloperConsole: descriptor.consumers.console;
		case CaxeTestSetup: descriptor.consumers.testSetup;
	};

/** Deterministic palette for one authoring surface. */
function flowActionPalette(consumer:FlowActionConsumer):Array<FlowActionDescriptor> {
	final result:Array<FlowActionDescriptor> = [];
	for (descriptor in allFlowActionDescriptors())
		if (flowActionAllowed(descriptor, consumer))
			result.push(descriptor);
	return result;
}

/** Guard against accidentally pairing typed payloads with the wrong metadata. */
function flowActionMatchesDescriptor(action:FlowAction, descriptor:FlowActionDescriptor):Bool
	return flowActionId(action).text() == descriptor.id.text() && schemaMatchesId(descriptor.id, descriptor.schema);

/** Validate a supplied inventory so negative tests can exercise corrupt copies. */
function validateFlowActionDescriptors(descriptors:Array<FlowActionDescriptor>):Array<FlowActionRegistryError> {
	final errors:Array<FlowActionRegistryError> = [];
	final counts:Map<String, Int> = [];
	for (descriptor in descriptors) {
		final syntax = descriptor.id.text();
		final count = counts.exists(syntax) ? counts.get(syntax) + 1 : 1;
		counts.set(syntax, count);
		if (count == 2)
			errors.push(DuplicateActionId(descriptor.id));
		if (!schemaMatchesId(descriptor.id, descriptor.schema))
			errors.push(WrongActionSchema(descriptor.id));
		if (!descriptor.consumers.caxeFlow)
			errors.push(MissingCaxeFlowAuthority(descriptor.id));
		for (consumer in [CutsceneDocument, DeveloperConsole, CaxeTestSetup])
			if (flowActionAllowed(descriptor, consumer))
				errors.push(UnimplementedConsumerAuthority(descriptor.id, consumer));
		if (descriptor.editorLabel.text() != 'editor.action.$syntax.label' || descriptor.editorHelp.text() != 'editor.action.$syntax.help')
			errors.push(WrongEditorMessages(descriptor.id));
		if (descriptor.traceName != syntax)
			errors.push(WrongTraceName(descriptor.id));
	}
	for (id in allFlowActionIds())
		if (!counts.exists(id.text()))
			errors.push(MissingActionId(id));
	return errors;
}

/** Independent schema check: a catalog typo fails before parsing game data. */
private function schemaMatchesId(id:FlowActionId, schema:FlowActionSchema):Bool
	return switch [id, schema] {
		case [DialogueAction, OneScenario(DialogueReference)]: true;
		case [JournalAction, OneScenario(JournalReference)]: true;
		case [SetFlagAction, ScenarioAndFlag(VariableReference(FlagValue))]: true;
		case [
			SetCounterAction,
			ScenarioAndInteger(VariableReference(CounterValue), CounterValue)
		]: true;
		case [
			AddCounterAction,
			ScenarioAndInteger(VariableReference(CounterValue), CounterDelta)
		]: true;
		case [SetStateAction, ScenarioAndContent(VariableReference(StateValue), StateContent)]: true;
		case [
			GiveItemAction,
			ScenarioContentInteger(InventoryOwnerReference, ItemContent, Quantity)
		]: true;
		case [
			TakeItemAction,
			ScenarioContentInteger(InventoryOwnerReference, ItemContent, Quantity)
		]: true;
		case [SpawnAction, OneScenario(ObjectReference)]: true;
		case [DespawnAction, OneScenario(ObjectReference)]: true;
		case [SetObjectStateAction, ScenarioAndContent(ObjectReference, StateContent)]: true;
		case [CheckpointAction, OneScenario(CheckpointReference)]: true;
		case [ObjectiveAction, ObjectiveTransition]: true;
		case [EffectAction, EffectWithOptionalObject]: true;
		case [SignalAction, OneContent(SignalContent)]: true;
		case [ScheduleAction, ScheduleSequence]: true;
		case [CallAction, SequenceCall]: true;
		case [ChooseAction, SeededChoices]: true;
		case _: false;
	};

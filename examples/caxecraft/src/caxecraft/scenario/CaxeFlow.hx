package caxecraft.scenario;

import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioStory.ObjectiveState;

/** Typed, deterministic WHEN / IF / DO rules for one scenario. */
typedef CaxeFlow = {
	final variables:Array<FlowVariable>;
	final sequences:Array<FlowSequence>;
	final rules:Array<FlowRule>;
}

enum FlowScope {
	Map;
	Player;
	Quest;
	Local(sequence:ScenarioId);
}

enum FlowValue {
	Flag(value:Bool);
	Counter(value:Int);
	State(value:ContentId);
}

/** Closed value type used for parameters and variable compatibility checks. */
enum FlowValueKind {
	FlagValue;
	CounterValue;
	StateValue;
}

typedef FlowVariable = {
	final id:ScenarioId;
	final scope:FlowScope;
	final initial:FlowValue;
}

enum FlowEvent {
	EnterZone(zone:ScenarioId);
	LeaveZone(zone:ScenarioId);
	Interact(objectId:ScenarioId);
	BlockChanged(zone:ScenarioId, blockType:ContentId);
	UseItem(itemType:ContentId);
	EntityDefeated(entity:ScenarioId);
	SignalReceived(signal:ContentId);
	TimerExpired(timer:ScenarioId);
	ObjectiveChanged(objective:ScenarioId);
	StateChanged(variable:ScenarioId);
}

enum FlowComparison {
	Equal;
	NotEqual;
	Less;
	LessOrEqual;
	Greater;
	GreaterOrEqual;
}

enum FlowPredicate {
	Always;
	All(children:Array<FlowPredicate>);
	AnyOf(children:Array<FlowPredicate>);
	Not(child:FlowPredicate);
	FlagIs(variable:ScenarioId, expected:Bool);
	CounterCompare(variable:ScenarioId, comparison:FlowComparison, value:Int);
	StateIs(variable:ScenarioId, expected:ContentId);
	ObjectStateIs(objectId:ScenarioId, expected:ContentId);
	InventoryHas(owner:ScenarioId, itemType:ContentId, comparison:FlowComparison, quantity:Int);
	ObjectiveIs(objective:ScenarioId, expected:ObjectiveState);
	NearObject(actor:ScenarioId, objectId:ScenarioId, maximumMilliBlocks:Int);
	ModeIs(mode:ScenarioMode);
}

enum FlowArgument {
	Value(value:FlowValue);
	Variable(variable:ScenarioId);
}

typedef FlowChoice = {
	final weight:Int;
	final actions:Array<FlowAction>;
}

enum FlowAction {
	ShowDialogue(dialogue:ScenarioId);
	AddJournal(entry:ScenarioId);
	SetFlag(variable:ScenarioId, value:Bool);
	SetCounter(variable:ScenarioId, value:Int);
	AddCounter(variable:ScenarioId, delta:Int);
	SetState(variable:ScenarioId, value:ContentId);
	GiveItem(owner:ScenarioId, itemType:ContentId, quantity:Int);
	TakeItem(owner:ScenarioId, itemType:ContentId, quantity:Int);
	Spawn(objectId:ScenarioId);
	Despawn(objectId:ScenarioId);
	SetObjectState(objectId:ScenarioId, value:ContentId);
	SetCheckpoint(checkpoint:ScenarioId);
	SetObjective(objective:ScenarioId, value:ObjectiveState);
	PlayEffect(effect:ContentId, objectId:Null<ScenarioId>);
	EmitSignal(signal:ContentId);
	Schedule(timer:ScenarioId, ticks:Int, sequence:ScenarioId, arguments:Array<FlowArgument>);
	CallSequence(sequence:ScenarioId, arguments:Array<FlowArgument>);
	ChooseSeeded(seedVariable:ScenarioId, choices:Array<FlowChoice>);
}

typedef FlowParameter = {
	final id:ScenarioId;
	final initial:FlowValue;
}

typedef FlowSequence = {
	final id:ScenarioId;
	final parameters:Array<FlowParameter>;
	final actions:Array<FlowAction>;
}

enum FlowRepeatPolicy {
	Once;
	Repeat;
	Cooldown(ticks:Int);
}

typedef FlowRule = {
	final id:ScenarioId;
	final priority:Int;
	final repeat:FlowRepeatPolicy;
	final event:FlowEvent;
	final predicate:FlowPredicate;
	final actions:Array<FlowAction>;
}

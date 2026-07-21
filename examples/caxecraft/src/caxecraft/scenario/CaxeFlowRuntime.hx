package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.ScenarioStory.ObjectiveState;

/**
	An exact fixed-tick number built only from ordinary non-negative `Int` values.

	`epoch` counts complete one-billion-tick groups and `offset` identifies one
	tick inside that group. `CaxeFlowClock` constructs and compares these values;
	callers should treat the fields as a readable saved-state representation.
**/
typedef FlowTick = {
	final epoch:Int;
	final offset:Int;
}

/** A game-supplied object position at the start of one fixed simulation tick. */
typedef FlowPosition = {
	final objectId:ScenarioId;
	final xMilli:Int;
	final yMilli:Int;
	final zMilli:Int;
}

/**
	Events and moving-object positions observed at one fixed tick boundary.

	The executor reads this complete input before it changes scenario state. That
	keeps every rule predicate on the same stable view of the tick.
**/
typedef FlowTickInput = {
	final events:Array<FlowEvent>;
	final positions:Array<FlowPosition>;
}

/** Closed kinds of bounded work guarded by the version-1 executor. */
enum FlowExecutionLimit {
	FixedTickEpochs;
	TickEvents;
	RuleExecutions;
	Actions;
	SequenceCalls;
	SequenceDepth;
	SpawnedObjects;
	ScheduledWork;
	PredicateEvaluations;
	DeferredWork;
}

/** Visible, deterministic failure from one fixed-tick execution attempt. */
enum FlowRuntimeDiagnostic {
	LimitExceeded(kind:FlowExecutionLimit, maximum:Int, owner:Null<ScenarioId>);
	InvalidRuntimeReference(id:ScenarioId);
	InvalidRuntimeAction(owner:ScenarioId);
}

/**
	Semantic requests for the game shell, user interface, audio layer, or editor.

	The portable rule engine never calls Raylib or mutates a renderer directly.
	A platform adapter consumes these typed requests after the tick completes.
**/
enum FlowPresentationEvent {
	DialogueRequested(id:ScenarioId);
	JournalAdded(id:ScenarioId);
	VariableChanged(id:ScenarioId, value:FlowValue);
	InventoryChanged(owner:ScenarioId, itemType:ContentId, quantity:Int);
	ObjectSpawned(id:ScenarioId);
	ObjectDespawned(id:ScenarioId);
	ObjectStateChanged(id:ScenarioId, value:ContentId);
	CheckpointChanged(id:ScenarioId);
	ObjectiveChanged(id:ScenarioId, value:ObjectiveState);
	EffectRequested(effect:ContentId, objectId:Null<ScenarioId>);
}

/** Complete observable result of one fixed CaxeFlow tick. */
typedef FlowTickResult = {
	final tick:FlowTick;
	final firedRules:Array<ScenarioId>;
	final presentation:Array<FlowPresentationEvent>;
	final diagnostics:Array<FlowRuntimeDiagnostic>;
}

package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowEventOccurrence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionId;
import caxecraft.scenario.CaxeFlowEventRegistry.FlowEventId;
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
	final events:Array<FlowEventOccurrence>;
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
	TraceEntries;
}

/** Visible, deterministic failure from one fixed-tick execution attempt. */
enum FlowRuntimeDiagnostic {
	LimitExceeded(kind:FlowExecutionLimit, maximum:Int, owner:Null<ScenarioId>);
	InvalidRuntimeReference(id:ScenarioId);
	InvalidRuntimeAction(owner:ScenarioId);
	InvalidRuntimeEvent(id:FlowEventId);
}

/** Closed recovery policy attached to every terminal runtime fault. */
enum FlowFailureDisposition {
	/** Keep the completed prefix visible, stop future ticks, and require restore. */
	TerminalFaultRetainedPrefix;
}

/**
	Exact work and visible prefix retained when a Flow tick becomes terminal.

	`attempted` is the rejected count for a bounded limit and zero for semantic
	failures. Prefix counts let the editor explain what completed without
	presenting those partial effects as a successful tick.
**/
typedef FlowFailure = {
	final diagnostic:FlowRuntimeDiagnostic;
	final disposition:FlowFailureDisposition;
	final attempted:Int;
	final completedRules:Int;
	final completedActions:Int;
	final presentationEvents:Int;
	final traceEntries:Int;
}

/**
	One bounded, deterministic explanation of a fixed-tick CaxeFlow decision.

	The editor can show these values as WHEN / IF / DO progress without reading
	executor internals. Entries contain stable semantic IDs only; they never carry
	host paths, raw input keys, wall-clock values, or renderer state.
**/
enum FlowTraceEntry {
	EventObserved(event:FlowEventId, actor:Null<ScenarioId>);
	PredicateEvaluated(rule:ScenarioId, event:FlowEventId, actor:Null<ScenarioId>, passed:Bool);
	ActionExecuted(owner:ScenarioId, action:FlowActionId);
	FollowUpDeferred(owner:ScenarioId, event:FlowEventId, readyTick:FlowTick);
	SequenceDeferred(owner:ScenarioId, timer:ScenarioId, sequence:ScenarioId, readyTick:FlowTick);
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

	/** Ask the campaign shell to resolve one map-authored exit ID. */
	CampaignExitRequested(exit:ScenarioId);
}

/**
	Complete observable result of one fixed CaxeFlow tick.

	`activeObjective` is the first active objective in authored order after a
	successful tick. It is null when no objective is active. A diagnostic also
	forces it to null, so presentation cannot accidentally publish a partially
	executed action prefix; consumers must inspect `diagnostics` first.
**/
typedef FlowTickResult = {
	final tick:FlowTick;
	final firedRules:Array<ScenarioId>;
	final presentation:Array<FlowPresentationEvent>;
	final activeObjective:Null<ScenarioId>;
	final diagnostics:Array<FlowRuntimeDiagnostic>;

	/** Terminal failure detail, or null when this tick completed normally. */
	final failure:Null<FlowFailure>;

	final trace:Array<FlowTraceEntry>;
}

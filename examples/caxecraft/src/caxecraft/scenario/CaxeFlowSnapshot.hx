package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowEventOccurrence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlowRuntime.FlowTick;
import caxecraft.scenario.ScenarioStory.ObjectiveState;

/**
	Copy-owned CaxeFlow state used by save systems and deterministic reload tests.

	This module stores semantic IDs and closed Haxe values only. It deliberately
	does not choose a file format or perform host I/O; the persistence layer can
	encode a validated snapshot without becoming another rule engine.
**/
typedef FlowVariableSnapshot = {
	final id:ScenarioId;
	final value:FlowValue;
}

/** Exact active, visual-state, and predicate position facts for one object. */
typedef FlowObjectSnapshot = {
	final id:ScenarioId;
	final active:Bool;
	final state:Null<ContentId>;
	final hasPosition:Bool;
	final xMilli:Int;
	final yMilli:Int;
	final zMilli:Int;
}

/** One materialized inventory stack. Zero quantities remain explicit. */
typedef FlowInventorySnapshot = {
	final owner:ScenarioId;
	final itemType:ContentId;
	final quantity:Int;
}

/** One objective's current state, paired with its canonical authored ID. */
typedef FlowObjectiveSnapshot = {
	final id:ScenarioId;
	final state:ObjectiveState;
}

/** Complete mutable rule-visible facts for one executor. */
typedef CaxeFlowStateSnapshot = {
	final variables:Array<FlowVariableSnapshot>;
	final objects:Array<FlowObjectSnapshot>;
	final inventory:Array<FlowInventorySnapshot>;
	final objectives:Array<FlowObjectiveSnapshot>;
	final journal:Array<ScenarioId>;
	final checkpoint:Null<ScenarioId>;
}

/** Distinguish map-wide history from one actor's repeat/cooldown history. */
enum FlowRuleHistoryScope {
	GlobalRuleHistory;
	ActorRuleHistory(actor:ScenarioId);
}

/** Saved once/cooldown state for one rule and scope. */
typedef FlowRuleHistorySnapshot = {
	final rule:ScenarioId;
	final scope:FlowRuleHistoryScope;
	final hasFired:Bool;
	final lastTick:FlowTick;
}

/** Work that remains scheduled after the saved fixed-tick boundary. */
enum FlowDeferredSnapshot {
	DeferredEventSnapshot(readyTick:FlowTick, event:FlowEventOccurrence);
	DeferredSequenceSnapshot(readyTick:FlowTick, timer:ScenarioId, sequence:ScenarioId, arguments:Array<FlowValue>);
}

/**
	Complete target-neutral executor snapshot.

	`scenario` prevents a save from being restored into another map merely because
	its arrays happen to have compatible lengths. Every nested array is copy-owned.
**/
typedef CaxeFlowSnapshot = {
	final scenario:ScenarioId;
	final tick:FlowTick;
	final state:CaxeFlowStateSnapshot;
	final ruleHistory:Array<FlowRuleHistorySnapshot>;
	final deferred:Array<FlowDeferredSnapshot>;
}

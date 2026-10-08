package caxecraft.domain;

import caxecraft.scenario.CaxeFlow.FlowEventOccurrence;
import caxecraft.scenario.CaxeFlow.FlowEventPosition;
import caxecraft.scenario.CaxeFlowSnapshot.CaxeFlowSnapshot;
import caxecraft.scenario.ScenarioId;

/**
	Save-owned Flow facts that belong to `GameSession` rather than the executor.

	The executor snapshot stores rule state. This companion value stores queued
	engine events and spatial observations, so loading while an actor is inside a
	trigger does not invent a second enter event. Character bodies remain owned by
	the wider game-session save contract.
**/
typedef TriggerActorSnapshot = {
	final id:ScenarioId;

	/** Last committed position, or null before this actor has a spatial sample. */
	final previous:Null<FlowEventPosition>;
}

/** One zone/actor membership in deterministic zone-major order. */
typedef TriggerMembershipSnapshot = {
	final zone:ScenarioId;
	final actor:ScenarioId;
	final inside:Bool;
}

/** Complete reload seam for CaxeFlow state embedded in one game session. */
typedef GameSessionFlowSnapshot = {
	final executor:CaxeFlowSnapshot;
	final pendingEvents:Array<FlowEventOccurrence>;
	final actors:Array<TriggerActorSnapshot>;
	final memberships:Array<TriggerMembershipSnapshot>;
}

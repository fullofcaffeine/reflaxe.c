package caxecraft.qa;

import caxecraft.pilot.AgentWorldObservation.renderAgentWorldObservation;

/**
	Checks the wire distinction between a checkpoint and a completed action batch.

	Synthetic state keeps this writer contract independent of campaign content.
	The runtime Piloscript probe runs it under both Eval and generated native C.
**/
function checkAgentWorldObservations():Void {
	for (terminal in [false, true]) {
		final encoded = renderAgentWorldObservation({
			sequence: 7,
			terminal: terminal,
			frame: 3,
			tick: 3,
			screen: "playing",
			mode: "synthetic",
			level: "synthetic-level",
			objective: "none",
			dialogue: "none",
			journal: "none",
			interaction: "none",
			aquaticMedium: "dry",
			aquaticEquipment: "none",
			position: {
				xMilli: 0,
				yMilli: 0,
				zMilli: 0,
				cellX: 0,
				cellY: 0,
				cellZ: 0
			},
			heading: {xMilli: 0, yMilli: 0, zMilli: 1000},
			vitals: {
				health: 1,
				safeTicks: 0,
				breathTicks: 1,
				maximumBreathTicks: 1
			},
			inventory: [],
			target: {
				hit: false,
				material: "none",
				cellX: 0,
				cellY: 0,
				cellZ: 0,
				distanceMilli: 0
			},
			nearby: [],
			terrainRadius: 0,
			terrain: [],
			events: [],
			screenshot: "none"
		});
		final expected = terminal ? '"terminal":true' : '"terminal":false';
		if (encoded.indexOf(expected) < 0 || encoded.indexOf('"sequence":7') < 0)
			throw "agent observation lost its batch-completion marker or sequence";
	}
}

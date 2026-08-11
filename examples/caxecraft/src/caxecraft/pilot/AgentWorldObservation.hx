package caxecraft.pilot;

import caxecraft.content.ContentJsonWriter.appendJsonString;

/**
 * Describes the game state that an automated player cannot learn from pixels alone.
 *
 * Caxecraft creates this value from the committed simulation state after an
 * action. The JSON writer only encodes that value. It does not calculate game
 * results or keep a second copy of the world.
 *
 * The default report stays small. It includes nearby actors, mechanisms, and a
 * surface-height map. A later live request can ask for a more detailed bounded
 * voxel region without sending the complete level on every frame.
 */
/** One integer position in thousandths of a block and its containing voxel. */
typedef AgentPlayerPosition = {
	final xMilli:Int;
	final yMilli:Int;
	final zMilli:Int;
	final cellX:Int;
	final cellY:Int;
	final cellZ:Int;
}

/** The camera direction, scaled by 1000 to keep JSON output deterministic. */
typedef AgentPlayerHeading = {
	final xMilli:Int;
	final yMilli:Int;
	final zMilli:Int;
}

/** Health and breath values from the committed local character. */
typedef AgentPlayerVitals = {
	final health:Int;
	final safeTicks:Int;
	final breathTicks:Int;
	final maximumBreathTicks:Int;
}

/** One hotbar item and whether the player selected it. */
typedef AgentInventorySlot = {
	final id:String;
	final count:Int;
	final selected:Bool;
}

/** The terrain cell under the crosshair, or an explicit miss. */
typedef AgentTargetObservation = {
	final hit:Bool;
	final material:String;
	final cellX:Int;
	final cellY:Int;
	final cellZ:Int;
	final distanceMilli:Int;
}

/** One nearby character or authored mechanism from the active level. */
typedef AgentNearbyObservation = {
	final id:String;
	final kind:String;
	final xMilli:Int;
	final yMilli:Int;
	final zMilli:Int;
	final distanceSquaredMilliBlocks:Int;
	final interactable:Bool;
	final state:String;
}

/** One column in the bounded surface map around the player. */
typedef AgentTerrainColumn = {
	final x:Int;
	final z:Int;
	final surfaceY:Int;
	final material:String;
}

/**
 * One versioned observation from the real game loop.
 *
 * `sequence` orders replies in a persistent session. `screenshot` names the
 * framebuffer capture paired with this state, or `none` when no image exists.
 * Content IDs remain data from the active package and are not compiled here.
 */
typedef AgentWorldObservation = {
	final sequence:Int;
	final frame:Int;
	final tick:Int;
	final screen:String;
	final mode:String;
	final level:String;
	final objective:String;
	final dialogue:String;
	final journal:String;
	final interaction:String;
	final aquaticMedium:String;
	final aquaticEquipment:String;
	final position:AgentPlayerPosition;
	final heading:AgentPlayerHeading;
	final vitals:AgentPlayerVitals;
	final inventory:Array<AgentInventorySlot>;
	final target:AgentTargetObservation;
	final nearby:Array<AgentNearbyObservation>;
	final terrainRadius:Int;
	final terrain:Array<AgentTerrainColumn>;
	final events:Array<String>;
	final screenshot:String;
}

/** Render one stable JSON object through the closed observation fields. */
function renderAgentWorldObservation(observation:AgentWorldObservation):String {
	final output = new StringBuf();
	output.add('{"schemaVersion":1,"sequence":${observation.sequence},"frame":${observation.frame},"tick":${observation.tick}');
	addStringField(output, "screen", observation.screen);
	addStringField(output, "mode", observation.mode);
	addStringField(output, "level", observation.level);
	addStringField(output, "objective", observation.objective);
	addStringField(output, "dialogue", observation.dialogue);
	addStringField(output, "journal", observation.journal);
	addStringField(output, "interaction", observation.interaction);
	addStringField(output, "aquaticMedium", observation.aquaticMedium);
	addStringField(output, "aquaticEquipment", observation.aquaticEquipment);
	output.add(',"position":{"xMilli":${observation.position.xMilli},"yMilli":${observation.position.yMilli},"zMilli":${observation.position.zMilli},"cellX":${observation.position.cellX},"cellY":${observation.position.cellY},"cellZ":${observation.position.cellZ}}');
	output.add(',"heading":{"xMilli":${observation.heading.xMilli},"yMilli":${observation.heading.yMilli},"zMilli":${observation.heading.zMilli}}');
	output.add(',"vitals":{"health":${observation.vitals.health},"safeTicks":${observation.vitals.safeTicks},"breathTicks":${observation.vitals.breathTicks},"maximumBreathTicks":${observation.vitals.maximumBreathTicks}}');
	output.add(',"inventory":[');
	for (index in 0...observation.inventory.length) {
		if (index > 0)
			output.add(',');
		final slot = observation.inventory[index];
		output.add('{"id":');
		appendJsonString(output, slot.id);
		output.add(',"count":${slot.count},"selected":${slot.selected ? "true" : "false"}}');
	}
	output.add('],"target":{"hit":${observation.target.hit ? "true" : "false"},"material":');
	appendJsonString(output, observation.target.material);
	output.add(',"cellX":${observation.target.cellX},"cellY":${observation.target.cellY},"cellZ":${observation.target.cellZ},"distanceMilli":${observation.target.distanceMilli}}');
	output.add(',"nearby":[');
	for (index in 0...observation.nearby.length) {
		if (index > 0)
			output.add(',');
		final item = observation.nearby[index];
		output.add('{"id":');
		appendJsonString(output, item.id);
		output.add(',"kind":');
		appendJsonString(output, item.kind);
		output.add(',"xMilli":${item.xMilli},"yMilli":${item.yMilli},"zMilli":${item.zMilli},"distanceSquaredMilliBlocks":${item.distanceSquaredMilliBlocks},"interactable":${item.interactable ? "true" : "false"},"state":');
		appendJsonString(output, item.state);
		output.add('}');
	}
	output.add('],"terrainRadius":${observation.terrainRadius},"terrain":[');
	for (index in 0...observation.terrain.length) {
		if (index > 0)
			output.add(',');
		final column = observation.terrain[index];
		output.add('{"x":${column.x},"z":${column.z},"surfaceY":${column.surfaceY},"material":');
		appendJsonString(output, column.material);
		output.add('}');
	}
	output.add('],"events":[');
	for (index in 0...observation.events.length) {
		if (index > 0)
			output.add(',');
		appendJsonString(output, observation.events[index]);
	}
	output.add('],"screenshot":');
	appendJsonString(output, observation.screenshot);
	output.add('}');
	return output.toString();
}

/** Add one comma-prefixed JSON string field in the schema-owned order. */
private function addStringField(output:StringBuf, name:String, value:String):Void {
	output.add(',"$name":');
	appendJsonString(output, value);
}

package caxecraft.app;

import caxecraft.app.ActivePlayableLevel.PlayableLevelView;
import caxecraft.domain.BlockKind;
import caxecraft.domain.Character;
import caxecraft.domain.GameSession;
import caxecraft.domain.RaycastHit;
import caxecraft.domain.World;
import caxecraft.domain.WorldRead.query as queryWorld;
import caxecraft.domain.WorldRead.surfaceY as worldSurfaceY;
import caxecraft.domain.WorldView;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.InventoryState;
import caxecraft.pilot.AgentWorldObservation.AgentInventorySlot;
import caxecraft.pilot.AgentWorldObservation.AgentNearbyObservation;
import caxecraft.pilot.AgentWorldObservation.AgentTargetObservation;
import caxecraft.pilot.AgentWorldObservation.AgentTerrainColumn;

/**
 * Converts committed game values into the small read-only view used by agents.
 *
 * These functions borrow state for one call. They cannot move the player,
 * change terrain, or advance content logic. The agent protocol therefore sees
 * the same state as the renderer without receiving mutation authority.
 */
/** Copy all fixed hotbar slots into stable item IDs and counts. */
function agentInventory(state:InventoryState):Array<AgentInventorySlot> {
	final slots:Array<AgentInventorySlot> = [];
	for (index in 0...Inventory.SLOT_COUNT)
		slots.push({id: itemName(index), count: Inventory.countAt(state, index), selected: state.selected == index});
	return slots;
}

/** Describe the block under the crosshair without inventing a target on a miss. */
function agentTarget(hit:RaycastHit, world:WorldView):AgentTargetObservation {
	return {
		hit: hit.hit,
		material: hit.hit ? blockName(queryWorld(world, World.coord(hit.cellX, hit.cellY, hit.cellZ))) : "none",
		cellX: hit.hit ? hit.cellX : 0,
		cellY: hit.hit ? hit.cellY : 0,
		cellZ: hit.hit ? hit.cellZ : 0,
		distanceMilli: hit.hit ? Std.int(hit.distance * 1000.0) : 0
	};
}

/**
 * Describe actors and mechanisms no more than 12 blocks from the player.
 *
 * Authored IDs make this result useful to content tools. Distance and current
 * interaction availability still come from the live session.
 */
function agentNearby(session:GameSession, level:PlayableLevelView, player:Character):Array<AgentNearbyObservation> {
	final nearby:Array<AgentNearbyObservation> = [];
	for (index in 0...level.dialogueActorCount()) {
		final entityId = level.dialogueActorIdAt(index);
		final actor = session.readCharacter(entityId);
		final distanceSquared = horizontalDistanceSquaredMilliBlocks(player.body.x, player.body.z, actor.body.x, actor.body.z);
		if (distanceSquared <= 144000000)
			nearby.push({
				id: level.dialogueActorAuthoredIdAt(index).text(),
				kind: "actor",
				xMilli: Std.int(actor.body.x * 1000.0),
				yMilli: Std.int(actor.body.y * 1000.0),
				zMilli: Std.int(actor.body.z * 1000.0),
				distanceSquaredMilliBlocks: distanceSquared,
				interactable: session.actorInteractionAvailable(entityId),
				state: actor.vitals.health > 0 ? "active" : "defeated"
			});
	}
	for (index in 0...level.statefulObjectCount()) {
		final id = level.statefulObjectIdAt(index);
		final transform = level.statefulObjectTransformAt(index);
		final x = transform.xMilli / 1000.0;
		final z = transform.zMilli / 1000.0;
		final distanceSquared = horizontalDistanceSquaredMilliBlocks(player.body.x, player.body.z, x, z);
		if (distanceSquared <= 144000000) {
			final state = session.statefulObjectState(id);
			nearby.push({
				id: id.text(),
				kind: "mechanism",
				xMilli: transform.xMilli,
				yMilli: transform.yMilli,
				zMilli: transform.zMilli,
				distanceSquaredMilliBlocks: distanceSquared,
				interactable: session.statefulObjectInteractionAvailable(id),
				state: state == null ? "missing" : state.text()
			});
		}
	}
	return nearby;
}

/** Build a clipped seven-by-seven surface map around the player. */
function agentTerrain(world:WorldView, playerCellX:Int, playerCellZ:Int):Array<AgentTerrainColumn> {
	final radius = 3;
	final terrain:Array<AgentTerrainColumn> = [];
	var z = playerCellZ - radius;
	while (z <= playerCellZ + radius) {
		var x = playerCellX - radius;
		while (x <= playerCellX + radius) {
			if (x >= 0 && x < World.WIDTH && z >= 0 && z < World.DEPTH) {
				final surfaceY = worldSurfaceY(world, x, z);
				terrain.push({
					x: x,
					z: z,
					surfaceY: surfaceY,
					material: surfaceY < 0 ? "air" : blockName(queryWorld(world, World.coord(x, surfaceY, z)))
				});
			}
			x++;
		}
		z++;
	}
	return terrain;
}

/** Convert a hotbar index to its stable item ID. */
private function itemName(index:Int):String
	return switch index {
		case 0: "grass-block";
		case 1: "dirt-block";
		case 2: "stone-block";
		case 3: "haxeforge";
		case 4: "copper-sword";
		case 5: "berries";
		case 6: "bread";
		case 7: "lantern";
		case _: "sand-block";
	};

/** Convert one closed terrain value to its stable external spelling. */
private function blockName(kind:BlockKind):String
	return switch kind {
		case Air: "air";
		case Grass: "grass";
		case Dirt: "dirt";
		case Stone: "stone";
		case Bedrock: "bedrock";
		case Sand: "sand";
		case Wood: "wood";
		case Leaves: "leaves";
		case Snow: "snow";
		case Ash: "ash";
	};

/** Calculate one exact squared distance after milliblock conversion. */
private function horizontalDistanceSquaredMilliBlocks(fromX:Float, fromZ:Float, toX:Float, toZ:Float):Int {
	final dxMilli = Std.int((toX - fromX) * 1000.0);
	final dzMilli = Std.int((toZ - fromZ) * 1000.0);
	return dxMilli * dxMilli + dzMilli * dzMilli;
}

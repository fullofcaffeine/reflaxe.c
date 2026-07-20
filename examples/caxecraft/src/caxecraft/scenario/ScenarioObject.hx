package caxecraft.scenario;

import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;

/** One stable, tagged object placed through the public editor path. */
typedef ScenarioObject = {
	final id:ScenarioId;
	final tags:Array<ScenarioTag>;
	final placement:ObjectPlacement;
}

/** Closed set of placement roles understood by CAXEMAP 1. */
enum ObjectPlacement {
	PlayerSpawn(transform:ScenarioTransform);
	Checkpoint(transform:ScenarioTransform);
	Item(itemType:ContentId, quantity:Int, transform:ScenarioTransform);
	Entity(entityType:ContentId, transform:ScenarioTransform);
	Npc(npcType:ContentId, dialogue:ScenarioId, transform:ScenarioTransform);
	Prefab(prefabType:ContentId, transform:ScenarioTransform);
	TriggerZone(bounds:VoxelBounds);
	StatefulObject(objectType:ContentId, initialState:ContentId, transform:ScenarioTransform);
}

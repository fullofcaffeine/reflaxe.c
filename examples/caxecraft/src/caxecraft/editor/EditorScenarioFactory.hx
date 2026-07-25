package caxecraft.editor;

import caxecraft.scenario.ContentId;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioText;
import caxecraft.scenario.ScenarioMessages;

/**
	Builds a minimal playable draft from ordinary typed Haxe values.

	The function has no hidden factory configuration or persistent state. Keeping
	it at module level makes every input visible and avoids a static-only class
	whose name would merely repeat the source module.
**/
/** Create a one-voxel world with one player spawn and optional messages. */
function create(mapId:ScenarioId, assetPack:LogicalPath, title:ScenarioText, mode:ScenarioMode, airBlock:ContentId, playerSpawnId:ScenarioId,
		?messageCatalog:ScenarioMessages):Scenario {
	return {
		formatVersion: 1,
		requiredFeatures: [new ContentId("caxecraft:core")],
		optionalFeatures: [],
		id: mapId,
		assetPack: assetPack,
		messages: messageCatalog == null ? NoMessageCatalog : messageCatalog,
		title: title,
		mode: mode,
		world: {
			size: {width: 1, height: 1, depth: 1},
			palette: [{code: 0, blockType: airBlock}],
			chunks: [
				{
					id: new ScenarioId("editor.chunk.x0.y0.z0"),
					origin: {x: 0, y: 0, z: 0},
					size: {width: 1, height: 1, depth: 1},
					runs: [{paletteCode: 0, count: 1}]
				}
			],
			fluids: []
		},
		objects: [
			{
				id: playerSpawnId,
				tags: [],
				placement: PlayerSpawn({
					xMilli: 500,
					yMilli: 0,
					zMilli: 500,
					yawDegrees: 0
				})
			}
		],
		story: {
			dialogues: [],
			journal: [],
			objectives: [],
			routes: []
		},
		flow: {variables: [], sequences: [], rules: []},
		extensions: []
	};
}

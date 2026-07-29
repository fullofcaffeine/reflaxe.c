package caxecraft.editor;

import caxecraft.scenario.ContentId;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioText;
import caxecraft.scenario.ScenarioMessages;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioWorld.BlockPaletteEntry;

/**
	Builds a minimal playable draft from ordinary typed Haxe values.

	The function has no hidden factory configuration or persistent state. Keeping
	it at module level makes every input visible and avoids a static-only class
	whose name would merely repeat the source module.
**/
/** Create a one-voxel world with one player spawn and optional messages. */
function create(mapId:ScenarioId, assetPack:LogicalPath, title:ScenarioText, mode:ScenarioMode, airBlock:ContentId, playerSpawnId:ScenarioId,
		?messageCatalog:ScenarioMessages):Scenario {
	return createBlank(mapId, assetPack, title, mode, airBlock, playerSpawnId, {width: 1, height: 1, depth: 1}, [], messageCatalog);
}

/**
	Create a bounded blank world with an explicit content palette.

	The caller chooses the size and additional block IDs, so the generic editor
	factory never learns Caxecraft's built-in content. Code zero remains the
	required air entry and every cell starts as air. `EditorSession.open`
	performs the authoritative format and content validation before publishing
	the draft; this function only assembles the typed candidate.
**/
function createBlank(mapId:ScenarioId, assetPack:LogicalPath, title:ScenarioText, mode:ScenarioMode, airBlock:ContentId, playerSpawnId:ScenarioId,
		size:VoxelSize, additionalPalette:Array<BlockPaletteEntry>, ?messageCatalog:ScenarioMessages):Scenario {
	final palette:Array<BlockPaletteEntry> = [{code: 0, blockType: airBlock}];
	for (entry in additionalPalette)
		palette.push({code: entry.code, blockType: entry.blockType});
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
			size: size,
			palette: palette,
			chunks: [
				{
					id: new ScenarioId("editor.chunk.x0.y0.z0"),
					origin: {x: 0, y: 0, z: 0},
					size: size,
					runs: [{paletteCode: 0, count: size.width * size.height * size.depth}]
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

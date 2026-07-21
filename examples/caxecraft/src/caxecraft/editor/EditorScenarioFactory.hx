package caxecraft.editor;

import caxecraft.scenario.ContentId;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioText;

/** Creates the smallest playable draft from ordinary typed Haxe values. */
final class EditorScenarioFactory {
	public static function create(mapId:ScenarioId, assetPack:LogicalPath, title:ScenarioText, mode:ScenarioMode, airBlock:ContentId,
			playerSpawnId:ScenarioId):Scenario {
		return {
			formatVersion: 1,
			requiredFeatures: [new ContentId("caxecraft:core")],
			optionalFeatures: [],
			id: mapId,
			assetPack: assetPack,
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
				]
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
}

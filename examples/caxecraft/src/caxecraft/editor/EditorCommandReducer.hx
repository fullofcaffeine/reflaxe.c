package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorCommandFamily;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorSettings;
import caxecraft.editor.EditorWorldGrid.EditorWorldResult;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioTag;
import caxecraft.scenario.ScenarioWorld.BlockPaletteEntry;

/** Internal candidate state produced before a command is committed to history. */
@:noCompletion
typedef EditorReduction = {
	final scenario:Scenario;
	final selection:Null<VoxelBounds>;
	final family:EditorCommandFamily;
}

/** Internal typed result for candidate command application. */
@:noCompletion
enum EditorReductionResult {
	ReductionReady(value:EditorReduction);
	ReductionRejected(error:EditorError);
}

/**
	Applies one closed command to an isolated draft copy.

	`@:noCompletion` keeps this reducer out of application suggestions because
	`EditorSession` owns snapshot isolation and history around it.
**/
@:noCompletion
final class EditorCommandReducer {
	public static function apply(scenario:Scenario, selection:Null<VoxelBounds>, command:EditorCommand, settings:EditorSettings):EditorReductionResult {
		return switch command {
			case ResizeWorld(size):
				switch EditorWorldGrid.resize(scenario.world, size) {
					case WorldRejected(error): ReductionRejected(error);
					case WorldReady(world):
						final nextSelection = selection != null && EditorWorldGrid.containsBounds(size, selection) ? selection : null;
						ready(withWorld(scenario, world), nextSelection, WorldShape);
				}
			case SetPaletteEntry(code, blockType):
				setPaletteEntry(scenario, selection, code, blockType);
			case PaintVoxel(point, paletteCode):
				paint(scenario, selection, [point], paletteCode, settings.selectionCells);
			case EraseVoxel(point):
				paint(scenario, selection, [point], 0, settings.selectionCells);
			case PaintVoxels(points, paletteCode):
				paint(scenario, selection, points, paletteCode, settings.selectionCells);
			case EraseVoxels(points):
				paint(scenario, selection, points, 0, settings.selectionCells);
			case Select(bounds):
				select(scenario, bounds, settings.selectionCells);
			case ClearSelection:
				ready(scenario, null, Selection);
			case FillSelection(paletteCode):
				fillSelection(scenario, selection, paletteCode);
			case StampPrefab(id, prefabType, tags, transform):
				stampPrefab(scenario, selection, id, prefabType, tags, transform);
			case PutObject(object):
				ready(withObjects(scenario, putObject(scenario.objects, object)), selection, Placement);
			case RemoveObject(id):
				removePlacedObject(scenario, selection, id);
			case PutDialogue(dialogue):
				ready(withDialogues(scenario, putDialogue(scenario.story.dialogues, dialogue)), selection, Dialogue);
			case RemoveDialogue(id):
				removeStoryDialogue(scenario, selection, id);
			case PutObjective(objective):
				ready(withObjectives(scenario, putObjective(scenario.story.objectives, objective)), selection, Objective);
			case RemoveObjective(id):
				removeStoryObjective(scenario, selection, id);
			case PutRule(rule):
				ready(withRules(scenario, putRule(scenario.flow.rules, rule)), selection, Rule);
			case RemoveRule(id):
				removeFlowRule(scenario, selection, id);
			case RestoreLastPlayable:
				ReductionRejected(NoPlayableScenario);
		}
	}

	static function setPaletteEntry(scenario:Scenario, selection:Null<VoxelBounds>, code:Int, blockType:ContentId):EditorReductionResult {
		if (code < 0 || code > 255)
			return ReductionRejected(InvalidPaletteCode(code));
		return ready(withWorld(scenario, {
			size: scenario.world.size,
			palette: putPalette(scenario.world.palette, {code: code, blockType: blockType}),
			chunks: scenario.world.chunks.copy()
		}), selection, Voxel);
	}

	static function select(scenario:Scenario, bounds:VoxelBounds, maximumCells:Int):EditorReductionResult {
		if (!EditorWorldGrid.containsBounds(scenario.world.size, bounds))
			return ReductionRejected(BoundsOutsideWorld(bounds));
		final cells = EditorWorldGrid.volume(bounds.size);
		if (cells > maximumCells)
			return ReductionRejected(SelectionTooLarge(cells, maximumCells));
		return ready(scenario, bounds, Selection);
	}

	static function fillSelection(scenario:Scenario, selection:Null<VoxelBounds>, paletteCode:Int):EditorReductionResult {
		if (selection == null)
			return ReductionRejected(NoSelection);
		if (!hasPaletteCode(scenario, paletteCode))
			return ReductionRejected(UnknownPaletteCode(paletteCode));
		return switch EditorWorldGrid.fill(scenario.world, selection, paletteCode) {
			case WorldRejected(error): ReductionRejected(error);
			case WorldReady(world): ready(withWorld(scenario, world), selection, Voxel);
		}
	}

	static function stampPrefab(scenario:Scenario, selection:Null<VoxelBounds>, id:ScenarioId, prefabType:ContentId, tags:Array<ScenarioTag>,
			transform:ScenarioTransform):EditorReductionResult {
		if (hasObject(scenario, id))
			return ReductionRejected(DuplicateObject(id));
		final objects = scenario.objects.copy();
		objects.push({id: id, tags: tags.copy(), placement: Prefab(prefabType, transform)});
		return ready(withObjects(scenario, objects), selection, Prefab);
	}

	static function removePlacedObject(scenario:Scenario, selection:Null<VoxelBounds>, id:ScenarioId):EditorReductionResult {
		if (!hasObject(scenario, id))
			return ReductionRejected(MissingObject(id));
		return ready(withObjects(scenario, removeObject(scenario.objects, id)), selection, Placement);
	}

	static function removeStoryDialogue(scenario:Scenario, selection:Null<VoxelBounds>, id:ScenarioId):EditorReductionResult {
		if (!hasDialogue(scenario, id))
			return ReductionRejected(MissingDialogue(id));
		return ready(withDialogues(scenario, removeDialogue(scenario.story.dialogues, id)), selection, Dialogue);
	}

	static function removeStoryObjective(scenario:Scenario, selection:Null<VoxelBounds>, id:ScenarioId):EditorReductionResult {
		if (!hasObjective(scenario, id))
			return ReductionRejected(MissingObjective(id));
		return ready(withObjectives(scenario, removeObjective(scenario.story.objectives, id)), selection, Objective);
	}

	static function removeFlowRule(scenario:Scenario, selection:Null<VoxelBounds>, id:ScenarioId):EditorReductionResult {
		if (!hasRule(scenario, id))
			return ReductionRejected(MissingRule(id));
		return ready(withRules(scenario, removeRule(scenario.flow.rules, id)), selection, Rule);
	}

	static function paint(scenario:Scenario, selection:Null<VoxelBounds>, points:Array<VoxelPoint>, paletteCode:Int, maximumCells:Int):EditorReductionResult {
		if (points.length > maximumCells)
			return ReductionRejected(VoxelEditTooLarge(points.length, maximumCells));
		if (!hasPaletteCode(scenario, paletteCode))
			return ReductionRejected(UnknownPaletteCode(paletteCode));
		return switch EditorWorldGrid.paintMany(scenario.world, points, paletteCode) {
			case WorldRejected(error): ReductionRejected(error);
			case WorldReady(world): ready(withWorld(scenario, world), selection, Voxel);
		}
	}

	static function ready(scenario:Scenario, selection:Null<VoxelBounds>, family:EditorCommandFamily):EditorReductionResult
		return ReductionReady({scenario: scenario, selection: selection, family: family});

	static function hasPaletteCode(scenario:Scenario, code:Int):Bool {
		for (entry in scenario.world.palette)
			if (entry.code == code)
				return true;
		return false;
	}

	static function hasObject(scenario:Scenario, id:ScenarioId):Bool {
		for (value in scenario.objects)
			if (same(value.id, id))
				return true;
		return false;
	}

	static function hasDialogue(scenario:Scenario, id:ScenarioId):Bool {
		for (value in scenario.story.dialogues)
			if (same(value.id, id))
				return true;
		return false;
	}

	static function hasObjective(scenario:Scenario, id:ScenarioId):Bool {
		for (value in scenario.story.objectives)
			if (same(value.id, id))
				return true;
		return false;
	}

	static function hasRule(scenario:Scenario, id:ScenarioId):Bool {
		for (value in scenario.flow.rules)
			if (same(value.id, id))
				return true;
		return false;
	}

	static function putPalette(values:Array<BlockPaletteEntry>, entry:BlockPaletteEntry):Array<BlockPaletteEntry> {
		final result:Array<BlockPaletteEntry> = [];
		var replaced = false;
		for (value in values)
			if (value.code == entry.code) {
				if (!replaced)
					result.push(entry);
				replaced = true;
			} else
				result.push(value);
		if (!replaced)
			result.push(entry);
		return result;
	}

	static function putObject(values:Array<ScenarioObject>, replacement:ScenarioObject):Array<ScenarioObject> {
		final result:Array<ScenarioObject> = [];
		var replaced = false;
		for (value in values)
			if (same(value.id, replacement.id)) {
				if (!replaced)
					result.push({id: replacement.id, tags: replacement.tags.copy(), placement: replacement.placement});
				replaced = true;
			} else
				result.push(value);
		if (!replaced)
			result.push({id: replacement.id, tags: replacement.tags.copy(), placement: replacement.placement});
		return result;
	}

	static function removeObject(values:Array<ScenarioObject>, id:ScenarioId):Array<ScenarioObject>
		return [for (value in values) if (!same(value.id, id)) value];

	static function putDialogue(values:Array<ScenarioDialogue>, replacement:ScenarioDialogue):Array<ScenarioDialogue> {
		final result = [for (value in values) if (!same(value.id, replacement.id)) value];
		result.push({id: replacement.id, lines: replacement.lines.copy()});
		return result;
	}

	static function removeDialogue(values:Array<ScenarioDialogue>, id:ScenarioId):Array<ScenarioDialogue>
		return [for (value in values) if (!same(value.id, id)) value];

	static function putObjective(values:Array<ScenarioObjective>, replacement:ScenarioObjective):Array<ScenarioObjective> {
		final result = [for (value in values) if (!same(value.id, replacement.id)) value];
		result.push(replacement);
		return result;
	}

	static function removeObjective(values:Array<ScenarioObjective>, id:ScenarioId):Array<ScenarioObjective>
		return [for (value in values) if (!same(value.id, id)) value];

	static function putRule(values:Array<FlowRule>, replacement:FlowRule):Array<FlowRule> {
		final result = [for (value in values) if (!same(value.id, replacement.id)) value];
		result.push({
			id: replacement.id,
			priority: replacement.priority,
			repeat: replacement.repeat,
			event: replacement.event,
			predicate: replacement.predicate,
			actions: replacement.actions.copy()
		});
		return result;
	}

	static function removeRule(values:Array<FlowRule>, id:ScenarioId):Array<FlowRule>
		return [for (value in values) if (!same(value.id, id)) value];

	static inline function same(left:ScenarioId, right:ScenarioId):Bool
		return left.text() == right.text();

	static function withWorld(scenario:Scenario, world:caxecraft.scenario.ScenarioWorld):Scenario
		return copy(scenario, world, scenario.objects, scenario.story.dialogues, scenario.story.objectives, scenario.flow.rules);

	static function withObjects(scenario:Scenario, objects:Array<ScenarioObject>):Scenario
		return copy(scenario, scenario.world, objects, scenario.story.dialogues, scenario.story.objectives, scenario.flow.rules);

	static function withDialogues(scenario:Scenario, dialogues:Array<ScenarioDialogue>):Scenario
		return copy(scenario, scenario.world, scenario.objects, dialogues, scenario.story.objectives, scenario.flow.rules);

	static function withObjectives(scenario:Scenario, objectives:Array<ScenarioObjective>):Scenario
		return copy(scenario, scenario.world, scenario.objects, scenario.story.dialogues, objectives, scenario.flow.rules);

	static function withRules(scenario:Scenario, rules:Array<FlowRule>):Scenario
		return copy(scenario, scenario.world, scenario.objects, scenario.story.dialogues, scenario.story.objectives, rules);

	static function copy(scenario:Scenario, world:caxecraft.scenario.ScenarioWorld, objects:Array<ScenarioObject>, dialogues:Array<ScenarioDialogue>,
			objectives:Array<ScenarioObjective>, rules:Array<FlowRule>):Scenario {
		return {
			formatVersion: scenario.formatVersion,
			requiredFeatures: scenario.requiredFeatures.copy(),
			optionalFeatures: scenario.optionalFeatures.copy(),
			id: scenario.id,
			assetPack: scenario.assetPack,
			title: scenario.title,
			mode: scenario.mode,
			world: world,
			objects: objects,
			story: {
				dialogues: dialogues,
				journal: scenario.story.journal.copy(),
				objectives: objectives,
				routes: scenario.story.routes.copy()
			},
			flow: {
				variables: scenario.flow.variables.copy(),
				sequences: scenario.flow.sequences.copy(),
				rules: rules
			},
			extensions: scenario.extensions.copy()
		};
	}
}

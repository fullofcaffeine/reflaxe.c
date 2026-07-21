package caxecraft.scenario;

import caxecraft.scenario.ScenarioDiagnostic.ScenarioCoordinate;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/**
	Checks voxel geometry, registered content, and placed world objects.

	This class is used only by `ScenarioValidator`. `@:noCompletion` keeps the
	implementation helper out of editor suggestions; it does not change Haxe
	visibility, runtime behavior, or type safety.
**/
@:noCompletion
final class ScenarioWorldValidator {
	final context:ScenarioValidationContext;

	public function new(context:ScenarioValidationContext)
		this.context = context;

	public function validateWorld():Void {
		final scenario = context.scenario;
		final size = scenario.world.size;
		final worldCoordinate = context.coordinateForWorld();
		var invalidEdge = false;
		if (size.width <= 0 || size.width > ScenarioLimits.MAX_WORLD_WIDTH) {
			context.addAtCoordinate(LimitExceeded(WorldWidth, ScenarioLimits.MAX_WORLD_WIDTH), worldCoordinate);
			invalidEdge = true;
		}
		if (size.height <= 0 || size.height > ScenarioLimits.MAX_WORLD_HEIGHT) {
			context.addAtCoordinate(LimitExceeded(WorldHeight, ScenarioLimits.MAX_WORLD_HEIGHT), worldCoordinate);
			invalidEdge = true;
		}
		if (size.depth <= 0 || size.depth > ScenarioLimits.MAX_WORLD_DEPTH) {
			context.addAtCoordinate(LimitExceeded(WorldDepth, ScenarioLimits.MAX_WORLD_DEPTH), worldCoordinate);
			invalidEdge = true;
		}
		if (invalidEdge)
			return;
		final cells = size.width * size.height * size.depth;
		if (cells <= 0 || cells > ScenarioLimits.MAX_WORLD_CELLS) {
			context.addAtCoordinate(LimitExceeded(WorldCells, ScenarioLimits.MAX_WORLD_CELLS), worldCoordinate);
			return;
		}
		if (scenario.world.palette.length > ScenarioLimits.MAX_PALETTE_ENTRIES)
			context.addAtCoordinate(LimitExceeded(PaletteEntries, ScenarioLimits.MAX_PALETTE_ENTRIES),
				context.coordinateForPalette(scenario.world.palette[scenario.world.palette.length - 1].code));
		final palette:Map<Int, Bool> = [];
		var zeroIsAir = false;
		for (entry in scenario.world.palette) {
			final coordinate = context.coordinateForPalette(entry.code);
			if (entry.code < 0 || entry.code > 255)
				context.addAtCoordinate(IntegerOutOfRange, coordinate);
			else if (palette.exists(entry.code))
				context.addAtCoordinate(DuplicatePaletteCode(entry.code), coordinate);
			palette.set(entry.code, true);
			if (!context.registry.hasBlock(entry.blockType))
				context.addAtCoordinate(UnresolvedContent(entry.blockType), coordinate);
			if (entry.code == 0 && context.registry.isAirBlock(entry.blockType))
				zeroIsAir = true;
		}
		if (!zeroIsAir)
			context.addAtCoordinate(MissingRecord(AirPaletteRecord), worldCoordinate);

		final covered = [for (_ in 0...cells) false];
		final chunkIds:Map<String, Bool> = [];
		for (chunk in scenario.world.chunks) {
			final chunkCoordinate = context.coordinateForIdentity(chunk.id, ChunkIdentity);
			if (chunkIds.exists(chunk.id.text()))
				context.addAtCoordinate(DuplicateId(chunk.id), chunkCoordinate);
			chunkIds.set(chunk.id.text(), true);
			final width = chunk.size.width;
			final height = chunk.size.height;
			final depth = chunk.size.depth;
			if (width <= 0
				|| width > ScenarioLimits.MAX_CHUNK_EDGE
				|| height <= 0
				|| height > ScenarioLimits.MAX_CHUNK_EDGE
				|| depth <= 0
				|| depth > ScenarioLimits.MAX_CHUNK_EDGE
				|| chunk.origin.x < 0
				|| chunk.origin.y < 0
				|| chunk.origin.z < 0
				|| chunk.origin.x + width > size.width
				|| chunk.origin.y + height > size.height
				|| chunk.origin.z + depth > size.depth) {
				context.addAtCoordinate(ImpossiblePlacement(chunk.id), chunkCoordinate);
				continue;
			}
			final expected = width * height * depth;
			var actual = 0;
			for (run in chunk.runs) {
				if (run.count <= 0 || !palette.exists(run.paletteCode) || actual > expected - run.count) {
					actual = -1;
					break;
				}
				actual += run.count;
			}
			if (actual != expected) {
				context.addAtCoordinate(InvalidRunTotal(chunk.id, expected, actual), chunkCoordinate);
				continue;
			}
			for (z in chunk.origin.z...chunk.origin.z + depth)
				for (y in chunk.origin.y...chunk.origin.y + height)
					for (x in chunk.origin.x...chunk.origin.x + width) {
						final cell = (z * size.height + y) * size.width + x;
						if (covered[cell])
							context.addAtCoordinate(ImpossiblePlacement(chunk.id), chunkCoordinate);
						covered[cell] = true;
					}
		}
		for (value in covered)
			if (!value) {
				context.addAtCoordinate(MissingRecord(CompleteChunkCoverage), worldCoordinate);
				break;
			}
	}

	public function validateObjects():Void {
		final scenario = context.scenario;
		var playerSpawns = 0;
		for (object in scenario.objects) {
			final objectCoordinate = context.coordinateForIdentity(object.id, ObjectIdentity);
			if (object.tags.length > ScenarioLimits.MAX_TAGS_PER_OBJECT)
				context.addAtCoordinate(LimitExceeded(ObjectTags, ScenarioLimits.MAX_TAGS_PER_OBJECT), objectCoordinate);
			final tags:Map<String, Bool> = [];
			for (tag in object.tags) {
				if (tags.exists(tag.text()))
					context.addAtCoordinate(DuplicateTag(object.id, tag), context.coordinateForTag(object.id, tag));
				tags.set(tag.text(), true);
			}
			switch object.placement {
				case PlayerSpawn(position):
					playerSpawns++;
					validateTransform(object.id, position, objectCoordinate);
				case Checkpoint(position):
					validateTransform(object.id, position, objectCoordinate);
				case Item(item, quantity, position):
					if (!context.registry.hasItem(item))
						context.addAtCoordinate(UnresolvedContent(item), objectCoordinate);
					else if (quantity <= 0 || quantity > context.registry.maximumItemQuantity(item))
						context.addAtCoordinate(ImpossiblePlacement(object.id), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
				case Entity(entity, position):
					if (!context.registry.hasEntity(entity))
						context.addAtCoordinate(UnresolvedContent(entity), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
				case Npc(npc, dialogue, position):
					if (!context.registry.hasNpc(npc))
						context.addAtCoordinate(UnresolvedContent(npc), objectCoordinate);
					if (!context.hasDialogue(dialogue))
						context.addAtCoordinate(UnresolvedReference(dialogue), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
				case Prefab(prefab, position):
					if (!context.registry.hasPrefab(prefab))
						context.addAtCoordinate(UnresolvedContent(prefab), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
				case TriggerZone(bounds):
					if (bounds.origin.x < 0
						|| bounds.origin.y < 0
						|| bounds.origin.z < 0
						|| bounds.size.width <= 0
						|| bounds.size.height <= 0
						|| bounds.size.depth <= 0
						|| bounds.origin.x + bounds.size.width > scenario.world.size.width
						|| bounds.origin.y + bounds.size.height > scenario.world.size.height
						|| bounds.origin.z + bounds.size.depth > scenario.world.size.depth)
						context.addAtCoordinate(ImpossiblePlacement(object.id), objectCoordinate);
				case StatefulObject(kind, state, position):
					if (!context.registry.hasStatefulObject(kind))
						context.addAtCoordinate(UnresolvedContent(kind), objectCoordinate);
					if (!context.registry.hasState(state))
						context.addAtCoordinate(UnresolvedContent(state), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
			}
		}
		if (playerSpawns != 1)
			context.addAtCoordinate(MissingRecord(SinglePlayerSpawn), context.coordinateForWorld());
	}

	function validateTransform(id:ScenarioId, value:caxecraft.scenario.ScenarioGeometry.ScenarioTransform, coordinate:ScenarioCoordinate):Void {
		final size = context.scenario.world.size;
		if (value.xMilli < 0 || value.yMilli < 0 || value.zMilli < 0 || value.xMilli >= size.width * 1000 || value.yMilli >= size.height * 1000
			|| value.zMilli >= size.depth * 1000 || value.yawDegrees < 0 || value.yawDegrees > 359)
			context.addAtCoordinate(ImpossiblePlacement(id), coordinate);
	}
}

package caxecraft.editor;

import caxecraft.content.LevelContentResolver.ActorPresentationResolution;
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioObject;

/**
 * Selects a creator-facing visual for one authored object.
 *
 * The selected content registry owns all asset and atlas-cell validation. This
 * module only maps the closed CAXEMAP placement roles to those validated facts
 * or to a role-specific editor marker. Rendering remains a separate concern.
 */
/** One validated atlas visual or editor-owned marker for an authored object. */
enum EditorObjectVisual {
	/** Show the authored player start with an editor marker. */
	PlayerSpawnVisual;

	/** Show an authored checkpoint with an editor marker. */
	CheckpointVisual;

	/** Show a collectible as a camera-facing atlas cell. */
	ItemVisual(asset:String, cellIndex:Int);

	/** Show an NPC or entity as a camera-facing atlas cell. */
	ActorVisual(asset:String, cellIndex:Int);

	/** Show a stateful mechanism on its authored box. */
	StatefulObjectVisual(asset:String, cellIndex:Int);

	/** Show the exact trigger bounds as an editor-only volume. */
	TriggerVolumeVisual;

	/** Show a solid role marker when the pack has no admitted visual. */
	FallbackObjectVisual;
}

/** Resolve one object without retaining its mutable tag array or loading art. */
function visualFor(registry:RuntimeContentRegistry, object:ScenarioObject):EditorObjectVisual {
	return switch object.placement {
		case PlayerSpawn(_): PlayerSpawnVisual;
		case Checkpoint(_): CheckpointVisual;
		case Item(itemType, _, _):
			final presentation = registry.itemPresentation(registry.itemStorageCode(itemType));
			presentation == null ? FallbackObjectVisual : ItemVisual(presentation.asset, presentation.cellIndex);
		case Entity(entityType, _): actorVisual(registry, entityType);
		case Npc(npcType, _, _): actorVisual(registry, npcType);
		case Prefab(_, _): FallbackObjectVisual;
		case TriggerZone(_): TriggerVolumeVisual;
		case StatefulObject(objectType, initialState, _): statefulObjectVisual(registry, objectType, initialState);
	};
}

/** True when transparent sprite art must draw after opaque editor geometry. */
function visualUsesBillboard(visual:EditorObjectVisual):Bool
	return switch visual {
		case ItemVisual(_, _) | ActorVisual(_, _): true;
		case _: false;
	};

/** Resolve either actor role through the pack's shared NPC/enemy visual table. */
private function actorVisual(registry:RuntimeContentRegistry, id:ContentId):EditorObjectVisual
	return switch registry.resolveActorPresentation(id) {
		case ActorPresentationResolved(asset, cellIndex): ActorVisual(asset, cellIndex);
		case UnknownActorPresentation: FallbackObjectVisual;
	};

/** Resolve one mechanism only when its authored state asks for presentation. */
private function statefulObjectVisual(registry:RuntimeContentRegistry, objectType:ContentId, initialState:ContentId):EditorObjectVisual {
	final presentation = registry.statefulObjectPresentation(objectType, initialState);
	if (presentation == null || !registry.statefulObjectVisible(objectType, initialState))
		return FallbackObjectVisual;
	return StatefulObjectVisual(presentation.asset, presentation.cellIndex);
}

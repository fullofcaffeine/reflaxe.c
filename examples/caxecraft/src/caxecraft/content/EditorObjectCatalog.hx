package caxecraft.content;

import caxecraft.scenario.ContentId;

/** One named state transition used by a linked creator recipe. */
typedef EditorStatefulTransitionRecipe = {
	final objectType:ContentId;
	final initialState:ContentId;
	final activeState:ContentId;
}

/**
	Describes pack-owned objects that a creator may place in a level.

	The content pack owns names and initial behavior. The editor receives this
	immutable recipe and never needs a branch for a campaign object.
**/
/** One admitted object recipe with every payload needed by a placement command. */
enum EditorObjectRecipeKind {
	/** Place one inventory item with a small validated default quantity. */
	EditorItem(itemType:ContentId, quantity:Int);

	/** Place one NPC; the editor binds its first dialogue and interaction rule. */
	EditorNpc(npcType:ContentId);

	/** Place one ordinary hostile or ambient entity. */
	EditorEnemy(entityType:ContentId);

	/** Place a hidden three-enemy group that appears when its trigger is entered. */
	EditorEnemyWave(entityType:ContentId);

	/** Place one stateful object with a pack-validated initial state. */
	EditorStatefulObject(objectType:ContentId, initialState:ContentId);

	/** Place two stateful objects and connect their admitted state changes. */
	EditorLinkedStatefulPair(source:EditorStatefulTransitionRecipe, target:EditorStatefulTransitionRecipe);
}

/** One child-readable catalog entry with a stable authoring identity. */
final class EditorObjectRecipe {
	/** Lower-case recipe ID used only to derive new authored object IDs. */
	public final id:String;

	/** English shelf label owned by reloadable content. */
	public final labelEn:String;

	/** Mexican Spanish shelf label owned by reloadable content. */
	public final labelEsMx:String;

	/** Closed placement payload consumed by the generic editor. */
	public final kind:EditorObjectRecipeKind;

	/** Construct one recipe only after its content pack validates all fields. */
	public function new(id:String, labelEn:String, labelEsMx:String, kind:EditorObjectRecipeKind) {
		this.id = id;
		this.labelEn = labelEn;
		this.labelEsMx = labelEsMx;
		this.kind = kind;
	}
}

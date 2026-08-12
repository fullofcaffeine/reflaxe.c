package caxecraft.content;

import caxecraft.content.ActorContentResolver.ActorContentKind;
import caxecraft.content.ActorContentResolver.ActorContentResolution;
import caxecraft.content.ContentJson.ContentJsonField;
import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentJson.ContentJsonValue;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.LevelContentResolver.FluidContentResolution;
import caxecraft.content.LevelContentResolver.ItemContentResolution;
import caxecraft.content.LevelContentResolver.ItemStorageCode;
import caxecraft.content.LevelContentResolver.ActorPresentationResolution;
import caxecraft.content.LevelContentResolver.LevelFluidSimulation;
import caxecraft.content.LevelContentResolver.TerrainContentResolution;
import caxecraft.content.LevelContentResolver.TerrainStorageCode;
import caxecraft.content.LevelContentResolver.StatefulObjectContentResolution;
import caxecraft.content.LevelContentResolver.StatefulObjectBounds;
import caxecraft.content.LevelContentResolver.StatefulObjectCollisionProfile;
import caxecraft.content.LevelContentResolver.StatefulObjectStateMechanics;
import caxecraft.content.RuntimeSchema.LocatedOptionalString;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaReader;
import caxecraft.content.RuntimeAssetInventory.RuntimeAssetFacts;
import caxecraft.domain.ActorControllerProfile;
import caxecraft.domain.ActorControllerProfile.WanderChaseMeleeProfile;
import caxecraft.domain.AquaticProfile;
import caxecraft.domain.Aquatics.profile as createAquaticProfile;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioContentRegistry;
import haxe.io.Bytes;

/**
 * Admits the exact Caxecraft content-pack schema into an immutable registry.
 *
 * The decoder accepts owned bytes and independently reviewed visual facts. It
 * checks every field, mechanic, bound, canonical order, and cross-reference
 * before a registry can escape. The JSON tree and source coordinates remain
 * request-local; gameplay receives only closed mechanics and copied values.
 */
/** Complete runtime registry or one located fail-closed diagnostic. */
enum RuntimeContentPackResult {
	/** Every definition and reference was admitted atomically. */
	RuntimeContentPackReady(registry:RuntimeContentRegistry);

	/** No partial registry escaped the failed candidate. */
	RuntimeContentPackRejected(diagnostic:RuntimeSchemaDiagnostic);
}

/** Closed item actions implemented by the current game engine. */
enum RuntimeItemUseProfile {
	ConsumeOneHeart;
	EquipAquatic;
	HaxeforgeTool;
	LightSource;
	MeleeSword;
	NoItemUse;
	PlaceBlock;
}

/** Reviewed visual asset/cell pair with its manifest-ordered cell index. */
final class RuntimePresentation {
	/** Stable visual asset ID. */
	public final asset:String;

	/** Stable semantic cell ID. */
	public final cell:String;

	/** Reviewed zero-based cell index used by the native renderer. */
	public final cellIndex:Int;

	/** Optional true-voxel model that can replace the atlas fallback. */
	public final model:RuntimeModelPresentation;

	/** Construct one presentation after the reviewed inventory resolved it. */
	public function new(asset:String, cell:String, cellIndex:Int, model:RuntimeModelPresentation) {
		this.asset = asset;
		this.cell = cell;
		this.cellIndex = cellIndex;
		this.model = model;
	}
}

/** Closed model choice for a presentation that always keeps an atlas fallback. */
enum RuntimeModelPresentation {
	/** Draw the existing atlas presentation. */
	NoRuntimeModel;

	/** Load one package-relative cubic MagicaVoxel model at its authored resolution. */
	RuntimeVoxelModel(path:String, cellsPerAxis:Int);
}

/**
 * Immutable content registry shared by authoring validation and level loading.
 *
 * Arrays stay private and are never returned. Bounded linear lookup is
 * deliberate for the current reviewed pack (10 blocks and 10 items): it keeps
 * one deterministic ownership model and avoids a second mutable index whose
 * consistency would need separate publication rules.
 */
final class RuntimeContentRegistry implements ScenarioContentRegistry implements LevelContentResolver {
	final packIdValue:String;
	final packVersionValue:Int;
	final logicalPathValue:String;
	final assetManifestIdValue:String;
	final airBlock:String;
	final defaultAquaticProfileId:String;
	final features:Array<RuntimeLocatedId>;
	final blocks:Array<RuntimeBlockDefinition>;
	final fluids:Array<RuntimeFluidDefinition>;
	final aquaticProfiles:Array<RuntimeAquaticDefinition>;
	final items:Array<RuntimeItemDefinition>;
	final npcs:Array<RuntimeNpcDefinition>;
	final enemies:Array<RuntimeEnemyDefinition>;
	final drops:Array<RuntimeDropDefinition>;
	final effects:Array<RuntimeEffectDefinition>;
	final statefulObjects:Array<RuntimeStatefulObjectDefinition>;
	final states:Array<RuntimeLocatedId>;

	/** Construct one complete registry after every candidate check passed. */
	public function new(packId:String, packVersion:Int, logicalPath:String, assetManifestId:String, airBlock:String, defaultAquaticProfileId:String,
			features:Array<RuntimeLocatedId>, blocks:Array<RuntimeBlockDefinition>, fluids:Array<RuntimeFluidDefinition>,
			aquaticProfiles:Array<RuntimeAquaticDefinition>, items:Array<RuntimeItemDefinition>, npcs:Array<RuntimeNpcDefinition>,
			enemies:Array<RuntimeEnemyDefinition>, drops:Array<RuntimeDropDefinition>, effects:Array<RuntimeEffectDefinition>,
			statefulObjects:Array<RuntimeStatefulObjectDefinition>, states:Array<RuntimeLocatedId>) {
		this.packIdValue = packId;
		this.packVersionValue = packVersion;
		this.logicalPathValue = logicalPath;
		this.assetManifestIdValue = assetManifestId;
		this.airBlock = airBlock;
		this.defaultAquaticProfileId = defaultAquaticProfileId;
		this.features = features;
		this.blocks = blocks;
		this.fluids = fluids;
		this.aquaticProfiles = aquaticProfiles;
		this.items = items;
		this.npcs = npcs;
		this.enemies = enemies;
		this.drops = drops;
		this.effects = effects;
		this.statefulObjects = statefulObjects;
		this.states = states;
	}

	/** Stable pack identity copied from the admitted document. */
	public inline function packId():String
		return packIdValue;

	/** Positive monotonic pack version copied from the admitted document. */
	public inline function packVersion():Int
		return packVersionValue;

	/** Canonical package logical path copied from the admitted document. */
	public inline function logicalPath():String
		return logicalPathValue;

	/** Asset-manifest identity named by the same verified content bytes. */
	public inline function assetManifestId():String
		return assetManifestIdValue;

	/** Number of admitted terrain definitions. */
	public inline function blockCount():Int
		return blocks.length;

	/** Return the schema-validated air ID used to initialize blank terrain. */
	public inline function airBlockId():ContentId
		return new ContentId(airBlock);

	/**
	 * Return the first admitted non-air block for a new editor brush.
	 *
	 * The current schema does not yet name a preferred editor brush. Choosing
	 * the first canonical non-air definition keeps that bounded policy outside
	 * generated source while allowing a future schema field to replace it.
	 */
	public function defaultEditorBlockId():ContentId {
		for (block in blocks)
			if (block.id != airBlock)
				return new ContentId(block.id);
		return new ContentId(airBlock);
	}

	/** Number of admitted item definitions and valid item storage codes. */
	public inline function itemCount():Int
		return items.length;

	/** Number of admitted fluid definitions. */
	public inline function fluidCount():Int
		return fluids.length;

	/** True when the pack registers one exact scenario feature. */
	public function supportsFeature(id:ContentId):Bool {
		for (feature in features)
			if (feature.id == id.text())
				return true;
		return false;
	}

	/** True only for the one schema-validated air terrain definition. */
	public inline function isAirBlock(id:ContentId):Bool
		return id.text() == airBlock;

	/** True when the selected pack owns one terrain ID. */
	public function hasBlock(id:ContentId):Bool
		return findBlock(id.text()) != null;

	/** Return a known terrain byte, or -1 for an unknown ID. */
	public function blockStorageCode(id:ContentId):Int {
		final block = findBlock(id.text());
		return block == null ? -1 : block.storageCode;
	}

	/** True when the admitted pack allows this terrain to become its declared drop. */
	public function blockIsCollectable(id:ContentId):Bool {
		final block = findBlock(id.text());
		return block != null && block.collectable;
	}

	/**
		Return the pack-local item code produced by one terrain definition.

		A pack-local code is the validated item's position in the registry, not a
		hotbar slot. `-1` means the block is unknown or deliberately has no drop.
	**/
	public function blockDropItemStorageCode(id:ContentId):Int {
		final block = findBlock(id.text());
		if (block == null)
			return -1;
		return switch block.dropItem {
			case RuntimeReferenceReady(reference): itemStorageCode(new ContentId(reference.id));
			case NoRuntimeReference: -1;
		};
	}

	/** Resolve one terrain ID into its nominal engine storage value. */
	public function resolveTerrain(id:ContentId):TerrainContentResolution {
		final block = findBlock(id.text());
		return block == null ? UnknownTerrainContent : TerrainContentResolved(TerrainStorageCode.fromValidated(block.storageCode));
	}

	/** True when the selected pack owns one fluid ID. */
	public function hasFluid(id:ContentId):Bool
		return findFluid(id.text()) != null;

	/** Return a known fluid's reviewed visual cell, or -1 when unknown. */
	public function fluidPresentationCell(id:ContentId):Int {
		final fluid = findFluid(id.text());
		return fluid == null ? -1 : fluid.presentation.cellIndex;
	}

	/** Resolve one fluid into the current closed simulator and presentation. */
	public function resolveFluid(id:ContentId):FluidContentResolution {
		final fluid = findFluid(id.text());
		return fluid == null ? UnknownFluidContent : FluidContentResolved(LevelFluidSimulation.BoundedWater, fluid.presentation.cellIndex);
	}

	/** True when the selected pack owns one item ID. */
	public function hasItem(id:ContentId):Bool
		return findItem(id.text()) != null;

	/** Return the canonical-array item index, or -1 for an unknown ID. */
	public function itemStorageCode(id:ContentId):Int {
		for (index in 0...items.length)
			if (items[index].id == id.text())
				return index;
		return -1;
	}

	/** Resolve one item ID into its nominal pack-local storage code. */
	public function resolveItem(id:ContentId):ItemContentResolution {
		final code = itemStorageCode(id);
		return code < 0 ? UnknownItemContent : ItemContentResolved(ItemStorageCode.fromValidated(code));
	}

	/** Resolve one NPC or enemy visual from the same validated pack definition. */
	public function resolveActorPresentation(id:ContentId):ActorPresentationResolution {
		final npc = findNpc(id.text());
		if (npc != null)
			return ActorPresentationResolved(npc.presentation.asset, npc.presentation.cellIndex);
		final enemy = findEnemy(id.text());
		return enemy == null ? UnknownActorPresentation : ActorPresentationResolved(enemy.presentation.asset, enemy.presentation.cellIndex);
	}

	/** True when a runtime integer can safely index the item registry. */
	public inline function isValidItemStorageCode(code:Int):Bool
		return code >= 0 && code < items.length;

	/** Return the selected item's closed action after validating its code. */
	public function itemUseProfile(code:Int):RuntimeItemUseProfile
		return isValidItemStorageCode(code) ? items[code].useProfile : NoItemUse;

	/** Return the selected item's stable content ID, or an empty value for an invalid code. */
	public function itemIdForStorageCode(code:Int):String
		return isValidItemStorageCode(code) ? items[code].id : "";

	/** Return the terrain storage code placed by this item, or `-1` when it places none. */
	public function itemPlacementBlockStorageCode(code:Int):Int {
		if (!isValidItemStorageCode(code))
			return -1;
		return switch items[code].placementBlock {
			case RuntimeReferenceReady(reference): blockStorageCode(new ContentId(reference.id));
			case NoRuntimeReference: -1;
		};
	}

	/** Return the selected item's reviewed icon, or null for an invalid code. */
	public function itemPresentation(code:Int):Null<RuntimePresentation>
		return isValidItemStorageCode(code) ? items[code].presentation : null;

	/** True when the selected item supplies an aquatic-profile override. */
	public function itemProvidesAquaticProfile(code:Int):Bool
		return isValidItemStorageCode(code) && switch items[code].aquaticProfile {
			case NoRuntimeReference: false;
			case RuntimeReferenceReady(_): true;
		};

	/** Return an item's aquatic profile, falling back only for an invalid caller code. */
	public function itemAquaticProfile(code:Int):AquaticProfile {
		if (isValidItemStorageCode(code)) {
			switch items[code].aquaticProfile {
				case RuntimeReferenceReady(reference):
					final definition = findAquatic(reference.id);
					if (definition != null)
						return definition.profile();
				case NoRuntimeReference:
			}
		}
		return defaultAquaticProfile();
	}

	/** Return the schema-validated default movement/survival profile. */
	public function defaultAquaticProfile():AquaticProfile {
		final definition = findAquatic(defaultAquaticProfileId);
		return definition == null ? createAquaticProfile(1, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 1, false, false) : definition.profile();
	}

	/** True when the pack owns one enemy ID used by CAXEMAP `Entity`. */
	public function hasEntity(id:ContentId):Bool
		return findEnemy(id.text()) != null;

	/** True when the pack owns one NPC ID. */
	public function hasNpc(id:ContentId):Bool
		return findNpc(id.text()) != null;

	/** Resolve an NPC or report a known enemy as the wrong authored kind. */
	public function resolveNpc(id:ContentId):ActorContentResolution {
		final npc = findNpc(id.text());
		if (npc != null)
			return ActorContentResolved({
				maximumHealth: npc.maximumHealth,
				aquaticProfile: defaultAquaticProfile(),
				controller: StationaryDialogue(npc.interactionRadiusMilli)
			});
		return findEnemy(id.text()) != null ? WrongActorContentKind(EnemyContent) : UnknownActorContent;
	}

	/** Resolve an enemy or report a known NPC as the wrong authored kind. */
	public function resolveEnemy(id:ContentId):ActorContentResolution {
		final enemy = findEnemy(id.text());
		if (enemy != null) {
			final profile:WanderChaseMeleeProfile = {
				noticeRadiusMilli: enemy.noticeRadiusMilli,
				strikeRadiusMilli: enemy.strikeRadiusMilli,
				attackRadiusMilli: enemy.attackRadiusMilli,
				windupTicks: enemy.windupTicks,
				recoveryTicks: enemy.recoveryTicks,
				stepMilli: enemy.stepMilli,
				drop: new ContentId(enemy.drop.id)
			};
			return ActorContentResolved({
				maximumHealth: enemy.maximumHealth,
				aquaticProfile: defaultAquaticProfile(),
				controller: enemy.behaviorProfile == "telegraphed-charge" ? TelegraphedCharge(profile) : WanderChaseMelee(profile)
			});
		}
		return findNpc(id.text()) != null ? WrongActorContentKind(NpcContent) : UnknownActorContent;
	}

	/** Reserved prefabs remain unsupported and therefore always absent. */
	public function hasPrefab(id:ContentId):Bool
		return false;

	/** True when the pack owns one bounded interactive object profile. */
	public function hasStatefulObject(id:ContentId):Bool
		return findStatefulObject(id.text()) != null;

	/** True when the pack owns one closed authored state value. */
	public function hasState(id:ContentId):Bool {
		for (state in states)
			if (state.id == id.text())
				return true;
		return false;
	}

	/** Return one interactive object's bounded proximity, or zero when unknown. */
	public function statefulObjectInteractionRadiusMilli(id:ContentId):Int {
		final object = findStatefulObject(id.text());
		return object == null ? 0 : object.interactionRadiusMilli;
	}

	/** Return the visual for one valid object/state pair, or null when incompatible. */
	public function statefulObjectPresentation(id:ContentId, state:ContentId):Null<RuntimePresentation> {
		final object = findStatefulObject(id.text());
		if (object == null)
			return null;
		return object.presentationFor(state.text());
	}

	/** True when one compatible state asks presentation to draw the object. */
	public function statefulObjectVisible(id:ContentId, state:ContentId):Bool {
		final object = findStatefulObject(id.text());
		return object != null && object.visibleFor(state.text());
	}

	/** Resolve one compatible object/state pair for level construction. */
	public function resolveStatefulObject(id:ContentId, state:ContentId):StatefulObjectContentResolution {
		final object = findStatefulObject(id.text());
		if (object == null)
			return UnknownStatefulObjectContent;
		final presentation = object.presentationFor(state.text());
		if (presentation == null)
			return UnknownStatefulObjectContent;
		return StatefulObjectContentResolved(object.interactionRadiusMilli, object.copyBounds(), object.copyStateMechanics(), presentation.asset,
			presentation.cellIndex);
	}

	/** Reserved signals remain unsupported and therefore always absent. */
	public function hasSignal(id:ContentId):Bool
		return false;

	/** True when the selected pack owns one closed feedback effect. */
	public function hasEffect(id:ContentId):Bool {
		for (effect in effects)
			if (effect.id == id.text())
				return true;
		return false;
	}

	/** Return an item's stack bound, or zero for an unknown ID. */
	public function maximumItemQuantity(id:ContentId):Int {
		final item = findItem(id.text());
		return item == null ? 0 : item.maximumStack;
	}

	/** Resolve one controller drop request without a content-name branch in gameplay. */
	public function dropQuantityById(id:ContentId):Int {
		for (drop in drops)
			if (drop.id == id.text())
				return drop.quantity;
		return 0;
	}

	/**
	 * Fold independent reviewed facts into the tracer's manually expected scalar.
	 *
	 * This is not a hash oracle. The test owns the literal 132089 and checks
	 * individual lookups too; this compact value makes Eval/native comparison
	 * inexpensive for the complete real package path.
	 */
	public function semanticProof():Int {
		final enemy = findEnemy("caxecraft:mossling");
		final item = findItem("caxecraft:berries");
		final drop = findDrop("caxecraft:mossling-berries");
		final aquatic = findAquatic("caxecraft:tideweave-aquatics");
		final fluid = findFluid("caxecraft:water");
		if (enemy == null || item == null || drop == null || aquatic == null || fluid == null)
			return 0;
		return packVersionValue * 100000 + enemy.maximumHealth * 10000 + enemy.windupTicks * 100 + drop.quantity * 10 + item.maximumStack
			+ aquatic.maximumBreathTicks + fluid.presentation.cellIndex;
	}

	/** Find one terrain definition without exposing the backing array. */
	function findBlock(id:String):Null<RuntimeBlockDefinition> {
		for (block in blocks)
			if (block.id == id)
				return block;
		return null;
	}

	/** Find one fluid definition without exposing the backing array. */
	function findFluid(id:String):Null<RuntimeFluidDefinition> {
		for (fluid in fluids)
			if (fluid.id == id)
				return fluid;
		return null;
	}

	/** Find one aquatic definition without exposing the backing array. */
	function findAquatic(id:String):Null<RuntimeAquaticDefinition> {
		for (profile in aquaticProfiles)
			if (profile.id == id)
				return profile;
		return null;
	}

	/** Find one item definition without exposing the backing array. */
	function findItem(id:String):Null<RuntimeItemDefinition> {
		for (item in items)
			if (item.id == id)
				return item;
		return null;
	}

	/** Find one NPC definition without exposing the backing array. */
	function findNpc(id:String):Null<RuntimeNpcDefinition> {
		for (npc in npcs)
			if (npc.id == id)
				return npc;
		return null;
	}

	/** Find one enemy definition without exposing the backing array. */
	function findEnemy(id:String):Null<RuntimeEnemyDefinition> {
		for (enemy in enemies)
			if (enemy.id == id)
				return enemy;
		return null;
	}

	/** Find one drop definition without exposing the backing array. */
	function findDrop(id:String):Null<RuntimeDropDefinition> {
		for (drop in drops)
			if (drop.id == id)
				return drop;
		return null;
	}

	/** Find one stateful-object profile without exposing its state table. */
	function findStatefulObject(id:String):Null<RuntimeStatefulObjectDefinition> {
		for (object in statefulObjects)
			if (object.id == id)
				return object;
		return null;
	}
}

/** Stateless entry point for one bounded content-pack admission request. */
final class RuntimeContentPack {
	/** Decode one complete content-pack candidate without filesystem authority. */
	public static function decode(input:Bytes):RuntimeContentPackResult {
		final reader = new RuntimeSchemaReader();
		final root = reader.parse(input);
		if (root == null)
			return rejected(reader);
		final fields = reader.object(root, "content pack", [
			"schemaVersion",
			"logicalPath",
			"packId",
			"packVersion",
			"assetManifestId",
			"assetCells",
			"airBlock",
			"defaultAquaticProfile",
			"features",
			"blocks",
			"fluids",
			"aquaticProfiles",
			"items",
			"npcs",
			"enemies",
			"drops",
			"effects",
			"prefabs",
			"statefulObjects",
			"states",
			"signals"
		]);
		if (fields == null)
			return rejected(reader);

		final schemaNode = reader.field(fields, "schemaVersion");
		final schemaVersion = reader.integer(schemaNode, "schemaVersion", 0, 2147483647);
		if (schemaVersion == null)
			return rejected(reader);
		if (schemaVersion != 2) {
			reader.reject(schemaNode, SchemaUnsupportedVersion("schemaVersion", 2));
			return rejected(reader);
		}

		final packId = readContentId(reader, reader.field(fields, "packId"), "packId");
		if (packId == null)
			return rejected(reader);
		final logicalPath = readLogicalPath(reader, reader.field(fields, "logicalPath"), "logicalPath");
		if (logicalPath == null)
			return rejected(reader);
		final packVersion = reader.integer(reader.field(fields, "packVersion"), "packVersion", 1, 2147483647);
		if (packVersion == null)
			return rejected(reader);
		final manifestNode = reader.field(fields, "assetManifestId");
		final manifestId = reader.string(manifestNode, "assetManifestId", 128);
		if (manifestId == null)
			return rejected(reader);
		final airNode = reader.field(fields, "airBlock");
		final air = readContentId(reader, airNode, "airBlock");
		if (air == null)
			return rejected(reader);
		final defaultAquaticNode = reader.field(fields, "defaultAquaticProfile");
		final defaultAquatic = readContentId(reader, defaultAquaticNode, "defaultAquaticProfile");
		if (defaultAquatic == null)
			return rejected(reader);
		final assets = readAssetInventory(reader, reader.field(fields, "assetCells"), manifestId);
		if (assets == null)
			return rejected(reader);

		final features = readIdArray(reader, reader.field(fields, "features"), "features", 1, 32);
		if (features == null)
			return rejected(reader);
		final blocks = readBlocks(reader, reader.field(fields, "blocks"));
		if (blocks == null)
			return rejected(reader);
		final fluids = readFluids(reader, reader.field(fields, "fluids"), assets);
		if (fluids == null)
			return rejected(reader);
		final aquaticProfiles = readAquaticProfiles(reader, reader.field(fields, "aquaticProfiles"));
		if (aquaticProfiles == null)
			return rejected(reader);
		final items = readItems(reader, reader.field(fields, "items"), assets);
		if (items == null)
			return rejected(reader);
		final npcs = readNpcs(reader, reader.field(fields, "npcs"), assets);
		if (npcs == null)
			return rejected(reader);
		final enemies = readEnemies(reader, reader.field(fields, "enemies"), assets);
		if (enemies == null)
			return rejected(reader);
		final drops = readDrops(reader, reader.field(fields, "drops"), assets);
		if (drops == null)
			return rejected(reader);
		final effects = readEffects(reader, reader.field(fields, "effects"));
		if (effects == null)
			return rejected(reader);
		final states = readIdArray(reader, reader.field(fields, "states"), "states", 0, 128);
		if (states == null)
			return rejected(reader);
		final statefulObjects = readStatefulObjects(reader, reader.field(fields, "statefulObjects"), assets);
		if (statefulObjects == null)
			return rejected(reader);

		for (reserved in ["prefabs", "signals"]) {
			final reservedNode = reader.field(fields, reserved);
			final values = reader.array(reservedNode, reserved, 0, 128);
			if (values == null)
				return rejected(reader);
			if (values.length != 0) {
				reader.reject(reservedNode, SchemaUnsupportedReservedKind(reserved));
				return rejected(reader);
			}
		}

		if (features.length != 1 || features[0].id != "caxecraft:core") {
			reader.reject(reader.field(fields, "features"), SchemaInvalidInvariant("features"));
			return rejected(reader);
		}
		final airReference = new RuntimeReference(air, airNode.line, airNode.column);
		final defaultAquaticReference = new RuntimeReference(defaultAquatic, defaultAquaticNode.line, defaultAquaticNode.column);
		if (!validatePack(reader, airReference, defaultAquaticReference, features, blocks, fluids, aquaticProfiles, items, npcs, enemies, drops, effects,
			statefulObjects, states))
			return rejected(reader);
		return RuntimeContentPackReady(new RuntimeContentRegistry(packId, packVersion, logicalPath, manifestId, air, defaultAquatic, features, blocks, fluids,
			aquaticProfiles, items, npcs, enemies, drops, effects, statefulObjects, states));
	}

	/** Decode the ordered atlas cells that presentation records can name. */
	static function readAssetInventory(reader:RuntimeSchemaReader, node:ContentJsonNode, manifestId:String):Null<RuntimeAssetInventory> {
		final values = reader.array(node, "assetCells", 1, 32);
		if (values == null)
			return null;
		final assets:Array<RuntimeAssetFacts> = [];
		final located:Array<RuntimeLocatedId> = [];
		for (index in 0...values.length) {
			final path = "assetCells[" + index + "]";
			final fields = reader.object(values[index], path, ["id", "cells"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = reader.string(idNode, path + ".id", 128);
			if (id == null || !RuntimeSchemaReader.validProfile(id)) {
				reader.reject(idNode, SchemaInvalidString(path + ".id"));
				return null;
			}
			final cellNodes = reader.array(reader.field(fields, "cells"), path + ".cells", 1, 256);
			if (cellNodes == null)
				return null;
			final cells:Array<String> = [];
			for (cellIndex in 0...cellNodes.length) {
				final cellPath = path + ".cells[" + cellIndex + "]";
				final cell = reader.string(cellNodes[cellIndex], cellPath, 128);
				if (cell == null || !RuntimeSchemaReader.validProfile(cell)) {
					reader.reject(cellNodes[cellIndex], SchemaInvalidString(cellPath));
					return null;
				}
				for (previous in cells)
					if (previous == cell) {
						reader.reject(cellNodes[cellIndex], SchemaDuplicateValue(path + ".cells", cell));
						return null;
					}
				cells.push(cell);
			}
			assets.push(new RuntimeAssetFacts(id, cells));
			located.push(new RuntimeLocatedId(id, idNode.line, idNode.column));
		}
		if (!validateOrder(reader, node, "assetCells", located))
			return null;
		return new RuntimeAssetInventory(manifestId, assets);
	}

	/** Parse one canonical namespaced ID and retain its source coordinate. */
	static function readContentId(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String):Null<String> {
		final value = reader.string(node, path, 128);
		if (value == null)
			return null;
		if (!RuntimeSchemaReader.validContentId(value)) {
			reader.reject(node, SchemaInvalidContentId(path));
			return null;
		}
		return value;
	}

	/** Parse one canonical package-relative logical path. */
	static function readLogicalPath(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String):Null<String> {
		final value = reader.string(node, path, 128);
		if (value == null)
			return null;
		if (!RuntimeSchemaReader.validLogicalPath(value)) {
			reader.reject(node, SchemaInvalidLogicalPath(path));
			return null;
		}
		return value;
	}

	/** Parse one UTF-8-ordered unique array of content IDs. */
	static function readIdArray(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String, minimum:Int, maximum:Int):Null<Array<RuntimeLocatedId>> {
		final values = reader.array(node, path, minimum, maximum);
		if (values == null)
			return null;
		final result:Array<RuntimeLocatedId> = [];
		for (index in 0...values.length) {
			final value = readContentId(reader, values[index], path + "[" + index + "]");
			if (value == null)
				return null;
			result.push(new RuntimeLocatedId(value, values[index].line, values[index].column));
		}
		return validateOrder(reader, node, path, result) ? result : null;
	}

	/** Parse all closed terrain definitions. */
	static function readBlocks(reader:RuntimeSchemaReader, node:ContentJsonNode):Null<Array<RuntimeBlockDefinition>> {
		final values = reader.array(node, "blocks", 0, 128);
		if (values == null)
			return null;
		final result:Array<RuntimeBlockDefinition> = [];
		for (index in 0...values.length) {
			final path = "blocks[" + index + "]";
			final fields = reader.object(values[index], path, ["id", "storageCode", "collision", "edit", "dropItem", "renderProfile"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readContentId(reader, idNode, path + ".id");
			final storage = reader.integer(reader.field(fields, "storageCode"), path + ".storageCode", 0, 255);
			final collision = readClosed(reader, reader.field(fields, "collision"), path + ".collision", ["passable", "solid"]);
			final edit = readClosed(reader, reader.field(fields, "edit"), path + ".edit", ["collectable", "immutable"]);
			final dropItem = reader.nullableContentId(reader.field(fields, "dropItem"), path + ".dropItem");
			final renderProfile = readClosed(reader, reader.field(fields, "renderProfile"), path + ".renderProfile", [
				"air",
				"ash-field",
				"foundation-rock",
				"forest-leaves",
				"forest-wood",
				"meadow-grass",
				"rich-soil",
				"river-sand",
				"slate-stone",
				"snow-field"
			]);
			if (reader.failure != null || id == null || storage == null || collision == null || edit == null || dropItem == null || renderProfile == null)
				return null;
			if ((edit == "collectable") != (dropItem.value != null)) {
				reader.reject(values[index], SchemaInvalidInvariant(path + ".dropItem"));
				return null;
			}
			result.push(new RuntimeBlockDefinition(id, idNode.line, idNode.column, storage, collision == "passable", edit == "collectable",
				optionalReference(dropItem), renderProfile));
		}
		return validateBlockOrder(reader, "blocks", result) ? result : null;
	}

	/** Parse all closed fluid definitions and resolve their visual cells. */
	static function readFluids(reader:RuntimeSchemaReader, node:ContentJsonNode, assets:RuntimeAssetInventory):Null<Array<RuntimeFluidDefinition>> {
		final values = reader.array(node, "fluids", 1, 128);
		if (values == null)
			return null;
		final result:Array<RuntimeFluidDefinition> = [];
		for (index in 0...values.length) {
			final path = "fluids[" + index + "]";
			final fields = reader.object(values[index], path, [
				"id",
				"simulationProfile",
				"renderProfile",
				"cameraProfile",
				"audioProfile",
				"presentation"
			]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readContentId(reader, idNode, path + ".id");
			final simulation = readClosed(reader, reader.field(fields, "simulationProfile"), path + ".simulationProfile", ["bounded-water"]);
			final render = readClosed(reader, reader.field(fields, "renderProfile"), path + ".renderProfile", ["translucent-voxel"]);
			final camera = readClosed(reader, reader.field(fields, "cameraProfile"), path + ".cameraProfile", ["clear-submersion"]);
			final audio = readClosed(reader, reader.field(fields, "audioProfile"), path + ".audioProfile", ["fresh-water"]);
			final presentation = readPresentation(reader, reader.field(fields, "presentation"), path + ".presentation", assets);
			if (reader.failure != null || id == null || simulation == null || render == null || camera == null || audio == null || presentation == null)
				return null;
			result.push(new RuntimeFluidDefinition(id, idNode.line, idNode.column, presentation));
		}
		return validateFluidOrder(reader, "fluids", result) ? result : null;
	}

	/** Parse all bounded movement and survival profiles. */
	static function readAquaticProfiles(reader:RuntimeSchemaReader, node:ContentJsonNode):Null<Array<RuntimeAquaticDefinition>> {
		final values = reader.array(node, "aquaticProfiles", 0, 128);
		if (values == null)
			return null;
		final result:Array<RuntimeAquaticDefinition> = [];
		for (index in 0...values.length) {
			final path = "aquaticProfiles[" + index + "]";
			final fields = reader.object(values[index], path, [
				"id",
				"maximumBreathTicks",
				"breathRecoveryPerTick",
				"horizontalControlMilli",
				"ascentAccelerationMilli",
				"descentAccelerationMilli",
				"buoyancyAccelerationMilli",
				"dragPerTickMilli",
				"drowningIntervalTicks",
				"underwaterMining",
				"coldProtection"
			]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readContentId(reader, idNode, path + ".id");
			final maximumBreath = reader.integer(reader.field(fields, "maximumBreathTicks"), path + ".maximumBreathTicks", 1, 12000);
			final recovery = reader.integer(reader.field(fields, "breathRecoveryPerTick"), path + ".breathRecoveryPerTick", 1, 120);
			final horizontal = reader.integer(reader.field(fields, "horizontalControlMilli"), path + ".horizontalControlMilli", 0, 1000);
			final ascent = reader.integer(reader.field(fields, "ascentAccelerationMilli"), path + ".ascentAccelerationMilli", 0, 40000);
			final descent = reader.integer(reader.field(fields, "descentAccelerationMilli"), path + ".descentAccelerationMilli", 0, 40000);
			final buoyancy = reader.integer(reader.field(fields, "buoyancyAccelerationMilli"), path + ".buoyancyAccelerationMilli", 0, 30000);
			final drag = reader.integer(reader.field(fields, "dragPerTickMilli"), path + ".dragPerTickMilli", 0, 900);
			final drowning = reader.integer(reader.field(fields, "drowningIntervalTicks"), path + ".drowningIntervalTicks", 1, 1200);
			final mining = reader.boolean(reader.field(fields, "underwaterMining"), path + ".underwaterMining");
			final cold = reader.boolean(reader.field(fields, "coldProtection"), path + ".coldProtection");
			if (reader.failure != null || id == null || maximumBreath == null || recovery == null || horizontal == null || ascent == null
				|| descent == null || buoyancy == null || drag == null || drowning == null || mining == null || cold == null)
				return null;
			result.push(new RuntimeAquaticDefinition(id, idNode.line, idNode.column, maximumBreath, recovery, horizontal, ascent, descent, buoyancy, drag,
				drowning, mining, cold));
		}
		return validateAquaticOrder(reader, "aquaticProfiles", result) ? result : null;
	}

	/** Parse all item definitions and their optional typed references. */
	static function readItems(reader:RuntimeSchemaReader, node:ContentJsonNode, assets:RuntimeAssetInventory):Null<Array<RuntimeItemDefinition>> {
		final values = reader.array(node, "items", 0, 128);
		if (values == null)
			return null;
		final result:Array<RuntimeItemDefinition> = [];
		for (index in 0...values.length) {
			final path = "items[" + index + "]";
			final fields = reader.object(values[index], path, ["id", "maxStack", "useProfile", "placementBlock", "aquaticProfile", "icon"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readContentId(reader, idNode, path + ".id");
			final maximumStack = reader.integer(reader.field(fields, "maxStack"), path + ".maxStack", 1, 64);
			final useText = readClosed(reader, reader.field(fields, "useProfile"), path + ".useProfile", [
				"consume-one-heart",
				"equip-aquatic",
				"haxeforge-tool",
				"light-source",
				"melee-sword",
				"none",
				"place-block"
			]);
			final placement = reader.nullableContentId(reader.field(fields, "placementBlock"), path + ".placementBlock");
			final aquatic = reader.nullableContentId(reader.field(fields, "aquaticProfile"), path + ".aquaticProfile");
			final presentation = readPresentation(reader, reader.field(fields, "icon"), path + ".icon", assets);
			if (reader.failure != null || id == null || maximumStack == null || useText == null || placement == null || aquatic == null || presentation == null)
				return null;
			if ((useText == "place-block") != (placement.value != null) || (useText == "equip-aquatic") != (aquatic.value != null)) {
				reader.reject(values[index], SchemaInvalidInvariant(path + ".useProfile"));
				return null;
			}
			result.push(new RuntimeItemDefinition(id, idNode.line, idNode.column, maximumStack, itemUse(useText), optionalReference(placement),
				optionalReference(aquatic), presentation));
		}
		return validateItemOrder(reader, "items", result) ? result : null;
	}

	/** Parse all current stationary-dialogue NPC definitions. */
	static function readNpcs(reader:RuntimeSchemaReader, node:ContentJsonNode, assets:RuntimeAssetInventory):Null<Array<RuntimeNpcDefinition>> {
		final values = reader.array(node, "npcs", 0, 128);
		if (values == null)
			return null;
		final result:Array<RuntimeNpcDefinition> = [];
		for (index in 0...values.length) {
			final path = "npcs[" + index + "]";
			final fields = reader.object(values[index], path, ["id", "behaviorProfile", "maxHealth", "interactionRadiusMilli", "presentation"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readContentId(reader, idNode, path + ".id");
			final behavior = readClosed(reader, reader.field(fields, "behaviorProfile"), path + ".behaviorProfile", ["stationary-dialogue"]);
			final health = reader.integer(reader.field(fields, "maxHealth"), path + ".maxHealth", 1, 10000);
			final radius = reader.integer(reader.field(fields, "interactionRadiusMilli"), path + ".interactionRadiusMilli", 250, 32000);
			final presentation = readActorPresentation(reader, reader.field(fields, "presentation"), path + ".presentation", assets);
			if (reader.failure != null || id == null || behavior == null || health == null || radius == null || presentation == null)
				return null;
			result.push(new RuntimeNpcDefinition(id, idNode.line, idNode.column, health, radius, presentation));
		}
		return validateNpcOrder(reader, "npcs", result) ? result : null;
	}

	/** Parse all current wander/chase/melee enemy definitions. */
	static function readEnemies(reader:RuntimeSchemaReader, node:ContentJsonNode, assets:RuntimeAssetInventory):Null<Array<RuntimeEnemyDefinition>> {
		final values = reader.array(node, "enemies", 0, 128);
		if (values == null)
			return null;
		final result:Array<RuntimeEnemyDefinition> = [];
		for (index in 0...values.length) {
			final path = "enemies[" + index + "]";
			final fields = reader.object(values[index], path, [
				"id",
				"behaviorProfile",
				"maxHealth",
				"noticeRadiusMilli",
				"strikeRadiusMilli",
				"attackRadiusMilli",
				"windupTicks",
				"recoveryTicks",
				"stepMilli",
				"drop",
				"presentation"
			]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readContentId(reader, idNode, path + ".id");
			final behavior = readClosed(reader, reader.field(fields, "behaviorProfile"), path + ".behaviorProfile",
				["telegraphed-charge", "wander-chase-melee"]);
			final health = reader.integer(reader.field(fields, "maxHealth"), path + ".maxHealth", 1, 10000);
			final notice = reader.integer(reader.field(fields, "noticeRadiusMilli"), path + ".noticeRadiusMilli", 250, 64000);
			final strike = reader.integer(reader.field(fields, "strikeRadiusMilli"), path + ".strikeRadiusMilli", 250, 64000);
			final attack = reader.integer(reader.field(fields, "attackRadiusMilli"), path + ".attackRadiusMilli", 250, 64000);
			final windup = reader.integer(reader.field(fields, "windupTicks"), path + ".windupTicks", 1, 1200);
			final recovery = reader.integer(reader.field(fields, "recoveryTicks"), path + ".recoveryTicks", 1, 1200);
			final step = reader.integer(reader.field(fields, "stepMilli"), path + ".stepMilli", 1, 10000);
			final dropNode = reader.field(fields, "drop");
			final drop = readContentId(reader, dropNode, path + ".drop");
			final presentation = readActorPresentation(reader, reader.field(fields, "presentation"), path + ".presentation", assets);
			if (reader.failure != null || id == null || behavior == null || health == null || notice == null || strike == null || attack == null
				|| windup == null || recovery == null || step == null || drop == null || presentation == null)
				return null;
			if (!(attack <= strike && strike <= notice)) {
				reader.reject(values[index], SchemaInvalidInvariant(path + ".attackRadiusMilli"));
				return null;
			}
			result.push(new RuntimeEnemyDefinition(id, idNode.line, idNode.column, behavior, health, notice, strike, attack, windup, recovery, step,
				new RuntimeReference(drop, dropNode.line, dropNode.column), presentation));
		}
		return validateEnemyOrder(reader, "enemies", result) ? result : null;
	}

	/** Parse all bounded item-drop definitions. */
	static function readDrops(reader:RuntimeSchemaReader, node:ContentJsonNode, assets:RuntimeAssetInventory):Null<Array<RuntimeDropDefinition>> {
		final values = reader.array(node, "drops", 0, 128);
		if (values == null)
			return null;
		final result:Array<RuntimeDropDefinition> = [];
		for (index in 0...values.length) {
			final path = "drops[" + index + "]";
			final fields = reader.object(values[index], path, ["id", "item", "quantity", "pickupRadiusMilli", "presentation"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readContentId(reader, idNode, path + ".id");
			final itemNode = reader.field(fields, "item");
			final item = readContentId(reader, itemNode, path + ".item");
			final quantity = reader.integer(reader.field(fields, "quantity"), path + ".quantity", 1, 64);
			final radius = reader.integer(reader.field(fields, "pickupRadiusMilli"), path + ".pickupRadiusMilli", 100, 32000);
			final presentation = readPresentation(reader, reader.field(fields, "presentation"), path + ".presentation", assets);
			if (reader.failure != null || id == null || item == null || quantity == null || radius == null || presentation == null)
				return null;
			result.push(new RuntimeDropDefinition(id, idNode.line, idNode.column, new RuntimeReference(item, itemNode.line, itemNode.column), quantity,
				radius, presentation));
		}
		return validateDropOrder(reader, "drops", result) ? result : null;
	}

	/** Parse all closed feedback-effect definitions. */
	static function readEffects(reader:RuntimeSchemaReader, node:ContentJsonNode):Null<Array<RuntimeEffectDefinition>> {
		final values = reader.array(node, "effects", 0, 128);
		if (values == null)
			return null;
		final result:Array<RuntimeEffectDefinition> = [];
		for (index in 0...values.length) {
			final path = "effects[" + index + "]";
			final fields = reader.object(values[index], path, ["id", "profile"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readContentId(reader, idNode, path + ".id");
			final profile = readClosed(reader, reader.field(fields, "profile"), path + ".profile", ["melee-feedback", "pickup-feedback"]);
			if (reader.failure != null || id == null || profile == null)
				return null;
			result.push(new RuntimeEffectDefinition(id, idNode.line, idNode.column));
		}
		return validateEffectOrder(reader, "effects", result) ? result : null;
	}

	/** Parse bounded interactive profiles and each profile's closed state visuals. */
	static function readStatefulObjects(reader:RuntimeSchemaReader, node:ContentJsonNode,
			assets:RuntimeAssetInventory):Null<Array<RuntimeStatefulObjectDefinition>> {
		final values = reader.array(node, "statefulObjects", 0, 128);
		if (values == null)
			return null;
		final result:Array<RuntimeStatefulObjectDefinition> = [];
		for (index in 0...values.length) {
			final path = "statefulObjects[" + index + "]";
			final fields = reader.object(values[index], path, ["id", "interaction", "interactionRadiusMilli", "bounds", "states"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readContentId(reader, idNode, path + ".id");
			final interaction = readClosed(reader, reader.field(fields, "interaction"), path + ".interaction", ["activate", "none"]);
			final radius = reader.integer(reader.field(fields, "interactionRadiusMilli"), path + ".interactionRadiusMilli", 0, 32000);
			final boundsPath = path + ".bounds";
			final boundsFields = reader.object(reader.field(fields, "bounds"), boundsPath, ["widthMilli", "heightMilli", "depthMilli"]);
			if (boundsFields == null)
				return null;
			final width = reader.integer(reader.field(boundsFields, "widthMilli"), boundsPath + ".widthMilli", 1, 32000);
			final height = reader.integer(reader.field(boundsFields, "heightMilli"), boundsPath + ".heightMilli", 1, 32000);
			final depth = reader.integer(reader.field(boundsFields, "depthMilli"), boundsPath + ".depthMilli", 1, 32000);
			final stateNodes = reader.array(reader.field(fields, "states"), path + ".states", 1, 32);
			if (id == null || interaction == null || radius == null || width == null || height == null || depth == null || stateNodes == null)
				return null;
			if ((interaction == "none") != (radius == 0)) {
				reader.reject(values[index], SchemaInvalidInvariant(path + ".interactionRadiusMilli"));
				return null;
			}
			final objectStates:Array<RuntimeStatefulObjectState> = [];
			for (stateIndex in 0...stateNodes.length) {
				final statePath = path + ".states[" + stateIndex + "]";
				final stateFields = reader.object(stateNodes[stateIndex], statePath, ["id", "collision", "render", "presentation"]);
				if (stateFields == null)
					return null;
				final stateNode = reader.field(stateFields, "id");
				final stateId = readContentId(reader, stateNode, statePath + ".id");
				final collision = readClosed(reader, reader.field(stateFields, "collision"), statePath + ".collision", ["passable", "solid"]);
				final render = readClosed(reader, reader.field(stateFields, "render"), statePath + ".render", ["hidden", "visible"]);
				final presentation = readPresentation(reader, reader.field(stateFields, "presentation"), statePath + ".presentation", assets);
				if (stateId == null || collision == null || render == null || presentation == null)
					return null;
				objectStates.push(new RuntimeStatefulObjectState(stateId, stateNode.line, stateNode.column, collision == "solid", render == "visible",
					presentation));
			}
			if (!validateStatefulObjectStateOrder(reader, path + ".states", objectStates))
				return null;
			result.push(new RuntimeStatefulObjectDefinition(id, idNode.line, idNode.column, radius, width, height, depth, objectStates));
		}
		return validateStatefulObjectOrder(reader, "statefulObjects", result) ? result : null;
	}

	/** Resolve one presentation against independently reviewed manifest facts. */
	static function readPresentation(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String, assets:RuntimeAssetInventory):Null<RuntimePresentation> {
		final hasModel = switch node.value {
			case JsonObject(values):
				var found = false;
				for (field in values)
					if (field.name == "model")
						found = true;
				found;
			case _: false;
		};
		final expected = ["asset", "cell"];
		if (hasModel)
			expected.push("model");
		final fields = reader.object(node, path, expected);
		if (fields == null)
			return null;
		final assetNode = reader.field(fields, "asset");
		final cellNode = reader.field(fields, "cell");
		final asset = reader.string(assetNode, path + ".asset", 128);
		final cell = reader.string(cellNode, path + ".cell", 128);
		if (asset == null || cell == null)
			return null;
		if (!assets.hasAsset(asset)) {
			reader.reject(assetNode, SchemaUnknownAsset(path + ".asset", asset));
			return null;
		}
		final index = assets.cellIndex(asset, cell);
		if (index < 0) {
			reader.reject(cellNode, SchemaUnknownAssetCell(path + ".cell", asset, cell));
			return null;
		}
		var model = NoRuntimeModel;
		if (hasModel) {
			final modelNode = reader.field(fields, "model");
			final modelPathPrefix = path + ".model";
			final modelFields = reader.object(modelNode, modelPathPrefix, ["path", "cellsPerAxis"]);
			if (modelFields == null)
				return null;
			final modelPathNode = reader.field(modelFields, "path");
			final modelPath = reader.string(modelPathNode, modelPathPrefix + ".path", 128);
			final cellsPerAxis = reader.integer(reader.field(modelFields, "cellsPerAxis"), modelPathPrefix + ".cellsPerAxis", 1, 128);
			if (modelPath == null || cellsPerAxis == null)
				return null;
			final pathAccepted = switch ContentPackagePath.parse(modelPath) {
				case PathAccepted(_): true;
				case PathRejected(_): false;
			};
			if (!pathAccepted || !StringTools.startsWith(modelPath, "assets/") || !StringTools.endsWith(modelPath, ".vox")) {
				reader.reject(modelPathNode, SchemaInvalidLogicalPath(modelPathPrefix + ".path"));
				return null;
			}
			model = RuntimeVoxelModel(modelPath, cellsPerAxis);
		}
		return new RuntimePresentation(asset, cell, index, model);
	}

	/** Resolve one actor visual through the same validated asset inventory as other world presentation. */
	static function readActorPresentation(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String, assets:RuntimeAssetInventory):Null<RuntimePresentation>
		return readPresentation(reader, node, path, assets);

	/** Parse one registered lower-case mechanic/profile value. */
	static function readClosed(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String, admitted:Array<String>):Null<String> {
		final value = reader.string(node, path, 128);
		if (value == null)
			return null;
		var found = false;
		for (choice in admitted)
			if (choice == value)
				found = true;
		if (!RuntimeSchemaReader.validProfile(value) || !found) {
			reader.reject(node, SchemaInvalidClosedValue(path, value));
			return null;
		}
		return value;
	}

	/** Convert a validated item profile spelling into a closed runtime choice. */
	static function itemUse(value:String):RuntimeItemUseProfile {
		if (value == "consume-one-heart")
			return ConsumeOneHeart;
		if (value == "equip-aquatic")
			return EquipAquatic;
		if (value == "haxeforge-tool")
			return HaxeforgeTool;
		if (value == "light-source")
			return LightSource;
		if (value == "melee-sword")
			return MeleeSword;
		if (value == "place-block")
			return PlaceBlock;
		return NoItemUse;
	}

	/** Convert an admitted nullable ID to a private immutable reference. */
	static function optionalReference(value:LocatedOptionalString):RuntimeOptionalReference {
		if (value.value == null)
			return NoRuntimeReference;
		return RuntimeReferenceReady(new RuntimeReference(value.value, value.line, value.column));
	}

	/** Validate array order and duplicate IDs for any private identified model. */
	static function validateOrder(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String, values:Array<RuntimeLocatedId>):Bool {
		for (index in 1...values.length) {
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		}
		return true;
	}

	/** Validate one adjacent ID pair without generic array covariance. */
	static function validatePair(reader:RuntimeSchemaReader, path:String, previous:RuntimeLocatedId, current:RuntimeLocatedId):Bool {
		final comparison = RuntimeSchemaReader.compareUtf8(previous.id, current.id);
		if (comparison > 0) {
			reader.rejectAt(current.line, current.column, SchemaNonCanonicalOrder(path));
			return false;
		}
		if (comparison == 0) {
			reader.rejectAt(current.line, current.column, SchemaDuplicateId(path, current.id));
			return false;
		}
		return true;
	}

	/** Validate terrain IDs in canonical order. */
	static function validateBlockOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeBlockDefinition>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate fluid IDs in canonical order. */
	static function validateFluidOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeFluidDefinition>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate aquatic-profile IDs in canonical order. */
	static function validateAquaticOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeAquaticDefinition>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate item IDs in canonical order. */
	static function validateItemOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeItemDefinition>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate NPC IDs in canonical order. */
	static function validateNpcOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeNpcDefinition>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate enemy IDs in canonical order. */
	static function validateEnemyOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeEnemyDefinition>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate drop IDs in canonical order. */
	static function validateDropOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeDropDefinition>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate effect IDs in canonical order. */
	static function validateEffectOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeEffectDefinition>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate stateful-object IDs in canonical order. */
	static function validateStatefulObjectOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeStatefulObjectDefinition>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate one object's state references in canonical order. */
	static function validateStatefulObjectStateOrder(reader:RuntimeSchemaReader, path:String, values:Array<RuntimeStatefulObjectState>):Bool {
		for (index in 1...values.length)
			if (!validatePair(reader, path, values[index - 1], values[index]))
				return false;
		return true;
	}

	/** Validate cross-kind uniqueness, terrain invariants, and every reference. */
	static function validatePack(reader:RuntimeSchemaReader, airReference:RuntimeReference, defaultAquaticReference:RuntimeReference,
			features:Array<RuntimeLocatedId>, blocks:Array<RuntimeBlockDefinition>, fluids:Array<RuntimeFluidDefinition>,
			aquatic:Array<RuntimeAquaticDefinition>, items:Array<RuntimeItemDefinition>, npcs:Array<RuntimeNpcDefinition>,
			enemies:Array<RuntimeEnemyDefinition>, drops:Array<RuntimeDropDefinition>, effects:Array<RuntimeEffectDefinition>,
			statefulObjects:Array<RuntimeStatefulObjectDefinition>, states:Array<RuntimeLocatedId>):Bool {
		final kinds:Array<RuntimeKindId> = [];
		for (entry in features)
			if (!addKind(reader, kinds, entry, "feature"))
				return false;
		for (entry in blocks)
			if (!addKind(reader, kinds, entry, "block"))
				return false;
		for (entry in fluids)
			if (!addKind(reader, kinds, entry, "fluid"))
				return false;
		for (entry in aquatic)
			if (!addKind(reader, kinds, entry, "aquatic profile"))
				return false;
		for (entry in items)
			if (!addKind(reader, kinds, entry, "item"))
				return false;
		for (entry in npcs)
			if (!addKind(reader, kinds, entry, "npc"))
				return false;
		for (entry in enemies)
			if (!addKind(reader, kinds, entry, "enemy"))
				return false;
		for (entry in drops)
			if (!addKind(reader, kinds, entry, "drop"))
				return false;
		for (entry in effects)
			if (!addKind(reader, kinds, entry, "effect"))
				return false;
		for (entry in statefulObjects)
			if (!addKind(reader, kinds, entry, "stateful object"))
				return false;
		for (entry in states)
			if (!addKind(reader, kinds, entry, "state"))
				return false;

		var air:Null<RuntimeBlockDefinition> = null;
		var passable = 0;
		for (block in blocks) {
			if (block.id == airReference.id)
				air = block;
			if (block.passable)
				passable++;
			for (other in blocks)
				if (block != other && block.storageCode == other.storageCode) {
					reader.rejectAt(other.line, other.column, SchemaDuplicateStorageCode(other.storageCode));
					return false;
				}
		}
		if (air == null) {
			reader.rejectAt(airReference.line, airReference.column, SchemaUnresolvedReference("airBlock", airReference.id, "block"));
			return false;
		}
		if (air.storageCode != 0 || !air.passable || air.renderProfile != "air" || passable != 1) {
			reader.rejectAt(air.line, air.column, SchemaInvalidInvariant("airBlock"));
			return false;
		}
		if (kindOf(kinds, defaultAquaticReference.id) != "aquatic profile") {
			return rejectReference(reader, defaultAquaticReference, "defaultAquaticProfile", "aquatic profile", kinds);
		}

		for (block in blocks)
			switch block.dropItem {
				case NoRuntimeReference:
				case RuntimeReferenceReady(reference):
					if (kindOf(kinds, reference.id) != "item" && !rejectReference(reader, reference, "block.dropItem", "item", kinds))
						return false;
			}
		for (item in items) {
			switch item.placementBlock {
				case NoRuntimeReference:
				case RuntimeReferenceReady(reference):
					if (kindOf(kinds, reference.id) != "block"
						&& !rejectReference(reader, reference, "item.placementBlock", "block", kinds))
						return false;
			}
			switch item.aquaticProfile {
				case NoRuntimeReference:
				case RuntimeReferenceReady(reference):
					if (kindOf(kinds, reference.id) != "aquatic profile"
						&& !rejectReference(reader, reference, "item.aquaticProfile", "aquatic profile", kinds))
						return false;
			}
		}
		for (drop in drops) {
			final item = findItemDefinition(items, drop.item.id);
			if (item == null) {
				if (!rejectReference(reader, drop.item, "drop.item", "item", kinds))
					return false;
			} else if (drop.quantity > item.maximumStack) {
				reader.rejectAt(drop.line, drop.column, SchemaInvalidInvariant("drop.quantity"));
				return false;
			}
		}
		for (enemy in enemies)
			if (kindOf(kinds, enemy.drop.id) != "drop" && !rejectReference(reader, enemy.drop, "enemy.drop", "drop", kinds))
				return false;
		for (object in statefulObjects)
			for (state in object.states)
				if (kindOf(kinds, state.id) != "state"
					&& !rejectReference(reader, state.reference(), "statefulObject.states", "state", kinds))
					return false;
		return true;
	}

	/** Add one located ID to the cross-kind index or reject its collision. */
	static function addKind(reader:RuntimeSchemaReader, kinds:Array<RuntimeKindId>, entry:RuntimeLocatedId, kind:String):Bool {
		for (existing in kinds)
			if (existing.id == entry.id) {
				reader.rejectAt(entry.line, entry.column, SchemaCrossKindId(entry.id));
				return false;
			}
		kinds.push(new RuntimeKindId(entry.id, kind));
		return true;
	}

	/** Reject one missing reference as unknown or as a precise wrong kind. */
	static function rejectReference(reader:RuntimeSchemaReader, reference:RuntimeReference, path:String, expectedKind:String, kinds:Array<RuntimeKindId>):Bool {
		for (entry in kinds)
			if (entry.id == reference.id) {
				reader.rejectAt(reference.line, reference.column, SchemaWrongReferenceKind(path, reference.id, entry.kind));
				return false;
			}
		reader.rejectAt(reference.line, reference.column, SchemaUnresolvedReference(path, reference.id, expectedKind));
		return false;
	}

	/** Return one indexed content kind, or an empty string when the ID is absent. */
	static function kindOf(kinds:Array<RuntimeKindId>, id:String):String {
		for (entry in kinds)
			if (entry.id == id)
				return entry.kind;
		return "";
	}

	/** Find one item definition while validating drop stack bounds. */
	static function findItemDefinition(values:Array<RuntimeItemDefinition>, id:String):Null<RuntimeItemDefinition> {
		for (value in values)
			if (value.id == id)
				return value;
		return null;
	}

	/** Return the reader's first failure, with an unreachable defensive fallback. */
	static function rejected(reader:RuntimeSchemaReader):RuntimeContentPackResult {
		final diagnostic = reader.failure;
		return diagnostic == null ? RuntimeContentPackRejected({
			line: 1,
			column: 1,
			kind: SchemaInvalidInvariant("decoder")
		}) : RuntimeContentPackRejected(diagnostic);
	}
}

/** Shared stable ID and source coordinate for private definition arrays. */
private class RuntimeLocatedId {
	/** Stable content ID. */
	public final id:String;

	/** One-based source line of the ID value. */
	public final line:Int;

	/** One-based source column of the ID value. */
	public final column:Int;

	/** Construct one immutable located ID. */
	public function new(id:String, line:Int, column:Int) {
		this.id = id;
		this.line = line;
		this.column = column;
	}
}

/** Stable non-null reference and its source coordinate. */
private final class RuntimeReference {
	/** Referenced stable ID. */
	public final id:String;

	/** One-based source line. */
	public final line:Int;

	/** One-based source column. */
	public final column:Int;

	/** Construct one reference copied from an admitted JSON scalar. */
	public function new(id:String, line:Int, column:Int) {
		this.id = id;
		this.line = line;
		this.column = column;
	}
}

/** Explicit optional-reference state used instead of a nullable class field. */
private enum RuntimeOptionalReference {
	/** JSON supplied null and therefore owns no reference. */
	NoRuntimeReference;

	/** JSON supplied one canonical reference with a source coordinate. */
	RuntimeReferenceReady(reference:RuntimeReference);
}

/** One stable ID and its closed content-kind label for reference diagnostics. */
private final class RuntimeKindId {
	/** Stable content ID. */
	public final id:String;

	/** Plain-language closed kind already selected by the decoder. */
	public final kind:String;

	/** Construct one immutable cross-kind index entry. */
	public function new(id:String, kind:String) {
		this.id = id;
		this.kind = kind;
	}
}

/** One admitted state and the visual selected while an object has that state. */
private final class RuntimeStatefulObjectState extends RuntimeLocatedId {
	/** Whether this state contributes the profile's collision box. */
	public final solid:Bool;

	/** Whether this state contributes its reviewed visual. */
	public final visible:Bool;

	/** Atlas cell resolved before this state can enter the registry. */
	public final presentation:RuntimePresentation;

	/** Construct one state reference with its validated presentation. */
	public function new(id:String, line:Int, column:Int, solid:Bool, visible:Bool, presentation:RuntimePresentation) {
		super(id, line, column);
		this.solid = solid;
		this.visible = visible;
		this.presentation = presentation;
	}

	/** Preserve the source coordinate when cross-reference validation rejects. */
	public inline function reference():RuntimeReference
		return new RuntimeReference(id, line, column);
}

/** One generic interactive-object profile with a closed visual state table. */
private final class RuntimeStatefulObjectDefinition extends RuntimeLocatedId {
	/** Maximum player distance for one semantic interaction. */
	public final interactionRadiusMilli:Int;

	/** Axis-aligned width centered on the placement before cardinal yaw. */
	public final widthMilli:Int;

	/** Height measured upward from the placement's Y position. */
	public final heightMilli:Int;

	/** Axis-aligned depth centered on the placement before cardinal yaw. */
	public final depthMilli:Int;

	/** Canonical state references; this array never escapes the registry module. */
	public final states:Array<RuntimeStatefulObjectState>;

	/** Retain one checked profile and a private copy of its states. */
	public function new(id:String, line:Int, column:Int, interactionRadiusMilli:Int, widthMilli:Int, heightMilli:Int, depthMilli:Int,
			states:Array<RuntimeStatefulObjectState>) {
		super(id, line, column);
		this.interactionRadiusMilli = interactionRadiusMilli;
		this.widthMilli = widthMilli;
		this.heightMilli = heightMilli;
		this.depthMilli = depthMilli;
		this.states = states.copy();
	}

	/** Return the matching visual, or null when this profile excludes the state. */
	public function presentationFor(stateId:String):Null<RuntimePresentation> {
		for (state in states)
			if (state.id == stateId)
				return state.presentation;
		return null;
	}

	/** True only for a compatible state whose visual is enabled. */
	public function visibleFor(stateId:String):Bool {
		for (state in states)
			if (state.id == stateId)
				return state.visible;
		return false;
	}

	/** Copy the box dimensions across the private registry boundary. */
	public function copyBounds():StatefulObjectBounds
		return {widthMilli: widthMilli, heightMilli: heightMilli, depthMilli: depthMilli};

	/** Copy every closed state mechanic for later CaxeFlow changes. */
	public function copyStateMechanics():Array<StatefulObjectStateMechanics> {
		final result:Array<StatefulObjectStateMechanics> = [];
		for (state in states)
			result.push({
				state: new ContentId(state.id),
				collision: state.solid ? StatefulObjectSolid : StatefulObjectPassable,
				visible: state.visible
			});
		return result;
	}
}

/** Immutable terrain facts needed by validation and level resolution. */
private final class RuntimeBlockDefinition extends RuntimeLocatedId {
	/** Compact terrain byte stored in level cells. */
	public final storageCode:Int;

	/** Whether actors may move through this terrain. */
	public final passable:Bool;

	/** Whether mining may replace this terrain and collect its drop. */
	public final collectable:Bool;

	/** Optional item produced when collectable terrain is removed. */
	public final dropItem:RuntimeOptionalReference;

	/** Closed renderer profile selected by the terrain schema. */
	public final renderProfile:String;

	/** Construct one fully validated terrain definition. */
	public function new(id:String, line:Int, column:Int, storageCode:Int, passable:Bool, collectable:Bool, dropItem:RuntimeOptionalReference,
			renderProfile:String) {
		super(id, line, column);
		this.storageCode = storageCode;
		this.passable = passable;
		this.collectable = collectable;
		this.dropItem = dropItem;
		this.renderProfile = renderProfile;
	}
}

/** Immutable fluid facts needed by level resolution and presentation. */
private final class RuntimeFluidDefinition extends RuntimeLocatedId {
	/** Reviewed visual selected for this bounded fluid profile. */
	public final presentation:RuntimePresentation;

	/** Construct one fully validated fluid definition. */
	public function new(id:String, line:Int, column:Int, presentation:RuntimePresentation) {
		super(id, line, column);
		this.presentation = presentation;
	}
}

/** Immutable bounded aquatic mechanics copied from one profile record. */
private final class RuntimeAquaticDefinition extends RuntimeLocatedId {
	/** Breath capacity in fixed simulation ticks. */
	public final maximumBreathTicks:Int;

	/** Breath restored on each tick outside the fluid. */
	public final breathRecoveryPerTick:Int;

	/** Horizontal control stored as integer thousandths. */
	public final horizontalControlMilli:Int;

	/** Upward input acceleration stored as integer thousandths. */
	public final ascentAccelerationMilli:Int;

	/** Downward input acceleration stored as integer thousandths. */
	public final descentAccelerationMilli:Int;

	/** Passive upward acceleration stored as integer thousandths. */
	public final buoyancyAccelerationMilli:Int;

	/** Per-tick movement drag stored as integer thousandths. */
	public final dragPerTickMilli:Int;

	/** Ticks between damage events after breath reaches zero. */
	public final drowningIntervalTicks:Int;

	/** Whether this profile permits ordinary underwater mining. */
	public final underwaterMining:Bool;

	/** Whether this profile protects its actor from cold water. */
	public final coldProtection:Bool;

	/** Construct one fully validated aquatic definition. */
	public function new(id:String, line:Int, column:Int, maximumBreathTicks:Int, breathRecoveryPerTick:Int, horizontalControlMilli:Int,
			ascentAccelerationMilli:Int, descentAccelerationMilli:Int, buoyancyAccelerationMilli:Int, dragPerTickMilli:Int, drowningIntervalTicks:Int,
			underwaterMining:Bool, coldProtection:Bool) {
		super(id, line, column);
		this.maximumBreathTicks = maximumBreathTicks;
		this.breathRecoveryPerTick = breathRecoveryPerTick;
		this.horizontalControlMilli = horizontalControlMilli;
		this.ascentAccelerationMilli = ascentAccelerationMilli;
		this.descentAccelerationMilli = descentAccelerationMilli;
		this.buoyancyAccelerationMilli = buoyancyAccelerationMilli;
		this.dragPerTickMilli = dragPerTickMilli;
		this.drowningIntervalTicks = drowningIntervalTicks;
		this.underwaterMining = underwaterMining;
		this.coldProtection = coldProtection;
	}

	/** Convert reviewed integer thousandths into the generic domain profile. */
	public function profile():AquaticProfile
		return createAquaticProfile(maximumBreathTicks, breathRecoveryPerTick, horizontalControlMilli / 1000.0, ascentAccelerationMilli / 1000.0,
			descentAccelerationMilli / 1000.0, buoyancyAccelerationMilli / 1000.0, dragPerTickMilli / 1000.0, drowningIntervalTicks, underwaterMining,
			coldProtection);
}

/** Immutable item mechanics, references, and presentation. */
private final class RuntimeItemDefinition extends RuntimeLocatedId {
	/** Largest quantity admitted in one inventory stack. */
	public final maximumStack:Int;

	/** Closed gameplay action performed when the item is used. */
	public final useProfile:RuntimeItemUseProfile;

	/** Optional terrain created by the place-block action. */
	public final placementBlock:RuntimeOptionalReference;

	/** Optional aquatic profile supplied while this item is equipped. */
	public final aquaticProfile:RuntimeOptionalReference;

	/** Reviewed inventory icon and its stable cell index. */
	public final presentation:RuntimePresentation;

	/** Construct one fully validated item definition. */
	public function new(id:String, line:Int, column:Int, maximumStack:Int, useProfile:RuntimeItemUseProfile, placementBlock:RuntimeOptionalReference,
			aquaticProfile:RuntimeOptionalReference, presentation:RuntimePresentation) {
		super(id, line, column);
		this.maximumStack = maximumStack;
		this.useProfile = useProfile;
		this.placementBlock = placementBlock;
		this.aquaticProfile = aquaticProfile;
		this.presentation = presentation;
	}
}

/** Immutable stationary-dialogue NPC mechanics and presentation. */
private final class RuntimeNpcDefinition extends RuntimeLocatedId {
	/** Initial and maximum health for the spawned character. */
	public final maximumHealth:Int;

	/** Dialogue activation radius stored as integer thousandths. */
	public final interactionRadiusMilli:Int;

	/** Reviewed character sprite selected for presentation. */
	public final presentation:RuntimePresentation;

	/** Construct one fully validated NPC definition. */
	public function new(id:String, line:Int, column:Int, maximumHealth:Int, interactionRadiusMilli:Int, presentation:RuntimePresentation) {
		super(id, line, column);
		this.maximumHealth = maximumHealth;
		this.interactionRadiusMilli = interactionRadiusMilli;
		this.presentation = presentation;
	}
}

/** Immutable wander/chase/melee enemy mechanics and presentation. */
private final class RuntimeEnemyDefinition extends RuntimeLocatedId {
	/** Closed reusable controller recipe selected by this content definition. */
	public final behaviorProfile:String;

	/** Initial and maximum health for the spawned enemy. */
	public final maximumHealth:Int;

	/** Distance at which the enemy begins tracking an actor, in thousandths. */
	public final noticeRadiusMilli:Int;

	/** Distance at which the enemy begins a strike, in thousandths. */
	public final strikeRadiusMilli:Int;

	/** Distance at which an active strike can connect, in thousandths. */
	public final attackRadiusMilli:Int;

	/** Fixed simulation ticks before an announced strike becomes active. */
	public final windupTicks:Int;

	/** Fixed simulation ticks before the enemy may strike again. */
	public final recoveryTicks:Int;

	/** Movement distance per simulation step, in thousandths. */
	public final stepMilli:Int;

	/** Validated drop-table entry used when this enemy is defeated. */
	public final drop:RuntimeReference;

	/** Reviewed enemy sprite selected for presentation. */
	public final presentation:RuntimePresentation;

	/** Construct one fully validated enemy definition. */
	public function new(id:String, line:Int, column:Int, behaviorProfile:String, maximumHealth:Int, noticeRadiusMilli:Int, strikeRadiusMilli:Int,
			attackRadiusMilli:Int, windupTicks:Int, recoveryTicks:Int, stepMilli:Int, drop:RuntimeReference, presentation:RuntimePresentation) {
		super(id, line, column);
		this.behaviorProfile = behaviorProfile;
		this.maximumHealth = maximumHealth;
		this.noticeRadiusMilli = noticeRadiusMilli;
		this.strikeRadiusMilli = strikeRadiusMilli;
		this.attackRadiusMilli = attackRadiusMilli;
		this.windupTicks = windupTicks;
		this.recoveryTicks = recoveryTicks;
		this.stepMilli = stepMilli;
		this.drop = drop;
		this.presentation = presentation;
	}
}

/** Immutable item-drop mechanics and presentation. */
private final class RuntimeDropDefinition extends RuntimeLocatedId {
	/** Validated item supplied by this drop entry. */
	public final item:RuntimeReference;

	/** Positive quantity, bounded by the referenced item's stack limit. */
	public final quantity:Int;

	/** Distance at which the drop may be collected, in thousandths. */
	public final pickupRadiusMilli:Int;

	/** Reviewed world sprite selected for the visible drop. */
	public final presentation:RuntimePresentation;

	/** Construct one fully validated drop definition. */
	public function new(id:String, line:Int, column:Int, item:RuntimeReference, quantity:Int, pickupRadiusMilli:Int, presentation:RuntimePresentation) {
		super(id, line, column);
		this.item = item;
		this.quantity = quantity;
		this.pickupRadiusMilli = pickupRadiusMilli;
		this.presentation = presentation;
	}
}

/** Immutable identity for one admitted closed feedback effect. */
private final class RuntimeEffectDefinition extends RuntimeLocatedId {
	/** Construct one effect after its closed profile was admitted. */
	public function new(id:String, line:Int, column:Int)
		super(id, line, column);
}

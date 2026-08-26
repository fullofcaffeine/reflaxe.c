package caxecraft.qa;

import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageModel.LoadedPackageBytes;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.ContentJson;
import caxecraft.content.ContentJson.ContentJsonField;
import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentJson.ContentJsonReadResult;
import caxecraft.content.ContentJson.ContentJsonValue;
import caxecraft.content.LevelContentResolver.FluidContentResolution;
import caxecraft.content.LevelContentResolver.ActorPresentationResolution;
import caxecraft.content.LevelContentResolver.StatefulObjectCollisionProfile;
import caxecraft.content.LevelContentResolver.StatefulObjectContentResolution;
import caxecraft.content.RuntimeContentPack;
import caxecraft.content.RuntimeContentPack.RuntimeContentPackResult;
import caxecraft.content.RuntimeContentPack.RuntimeItemUseProfile;
import caxecraft.content.RuntimeContentPack.RuntimeModelPresentation;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipeKind;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaErrorKind;
import caxecraft.app.RuntimeInventoryBinding.inventoryKindForRuntimeItem;
import caxecraft.app.VoxelFrameAnimation.VoxelFrameAnimationPlayer;
import caxecraft.editor.EditorObjectPresentation.EditorObjectVisual;
import caxecraft.editor.EditorObjectPresentation.visualFor as editorObjectVisualFor;
import caxecraft.editor.EditorAssetBrowser.EditorAssetCategory;
import caxecraft.editor.EditorAssetBrowser.EditorAssetEntry;
import caxecraft.editor.EditorAssetBrowser.EditorAssetUse;
import caxecraft.editor.EditorAssetBrowser.availableEditorAssets;
import caxecraft.editor.EditorAssetBrowser.filterEditorAssets;
import caxecraft.editor.EditorAssetBrowser.moveEditorAssetSelection;
import caxecraft.editor.EditorFlowProjection.allEditorFlowUiMessages;
import caxecraft.gameplay.ItemKind;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.RuntimeUiCatalog.RuntimeUiCatalogResult;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.CaxeFlowActionRegistry.allFlowActionDescriptors;
import caxecraft.scenario.CaxeFlowEventRegistry.allFlowEventDescriptors;
import caxecraft.scenario.CaxeFlowPredicateRegistry.allFlowPredicateDescriptors;
import caxecraft.scenario.CaxeFlowDiagnosticText.requiredCaxeFlowDiagnosticMessageIds;
import caxecraft.scenario.ScenarioDiagnosticText.requiredScenarioDiagnosticMessageIds;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import haxe.io.Bytes;

/**
 * Proves the shipped content and UI schemas through one real package boundary.
 *
 * Preconditions: the process starts in the Caxecraft root and the checked-in
 * `content.json` and `ui.json` are present. The test reads their exact bytes,
 * admits the closed schemas, observes manually reviewed registry/catalog
 * values, and then applies one mutation for each distinct rejection family.
 * Eval and strict generated C execute this same Haxe owner; the C harness only
 * observes four scalar results.
 */
/** First broken invariant, or zero after every schema check passes. */
var observed:Int = 0;

/** Manually expected semantic proof from the reviewed base content JSON. */
var tracePack:Int = 0;

/** Data-derived locale shape and one non-empty translated-message observation. */
var traceUi:Int = 0;

/** Source line from a representative version diagnostic. */
var traceDiagnosticLine:Int = 0;

/** Run the shared tracer under Eval or publish its envelope to native C. */
function main():Void {
	final status = selfCheck();
	#if c
	observed = status;
	#else
	Sys.println(status);
	Sys.println(tracePack);
	Sys.println(traceUi);
	Sys.println(traceDiagnosticLine);
	#end
}

/** Return zero only when the real positive path and focused negatives agree. */
function selfCheck():Int {
	final store = switch ContentPackageStore.open(".", "caxecraft-runtime-schema", ContentJson.MAXIMUM_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 1;
	};
	final content = readRequired(store, "packs/caxecraft/base/content.json");
	if (content == null)
		return 2;
	final ui = readRequired(store, "locales/ui.json");
	if (ui == null)
		return 3;

	final registry = switch RuntimeContentPack.decode(content.bytes) {
		case RuntimeContentPackReady(value): value;
		case RuntimeContentPackRejected(_): return 4;
	};
	final catalog = switch RuntimeUiCatalog.decode(ui.bytes) {
		case RuntimeUiCatalogReady(value): value;
		case RuntimeUiCatalogRejected(_): return 5;
	};
	final editorObject = registry.editorObjectAt(0);
	if (registry.editorObjectCount() != 1 || editorObject == null || editorObject.id != "forge-relay" || editorObject.labelEn.length == 0
		|| editorObject.labelEsMx.length == 0)
		return 58;
	switch editorObject.kind {
		case EditorStatefulObject(objectType, initialState):
			if (objectType.text() != "caxecraft:gate-relay" || initialState.text() != "caxecraft:waiting")
				return 58;
		case EditorItem(_, _) | EditorNpc(_) | EditorEnemy(_):
			return 58;
	}
	final assets = availableEditorAssets(registry, catalog);
	var terrainAssets = 0;
	var itemAssets = 0;
	var npcAssets = 0;
	var enemyAssets = 0;
	var mechanismAssets = 0;
	var editorObjectLabelsMatch = false;
	for (left in 0...assets.length) {
		final entry = assets[left];
		if (entry.labelEn.length == 0 || entry.labelEsMx.length == 0 || entry.helpEn.length == 0 || entry.helpEsMx.length == 0)
			return 76;
		switch entry.category {
			case TerrainAssets:
				terrainAssets++;
				switch entry.use {
					case PaintTerrainAsset(_):
					case PlaceObjectAsset(_): return 76;
				}
			case ItemAssets:
				itemAssets++;
				switch entry.use {
					case PlaceObjectAsset({kind: EditorItem(_, 1)}):
					case _: return 76;
				}
			case NpcAssets:
				npcAssets++;
				switch entry.use {
					case PlaceObjectAsset({kind: EditorNpc(_)}):
					case _: return 76;
				}
			case EnemyAssets:
				enemyAssets++;
				switch entry.use {
					case PlaceObjectAsset({kind: EditorEnemy(_)}):
					case _: return 76;
				}
			case MechanismAssets:
				mechanismAssets++;
				switch entry.use {
					case PlaceObjectAsset(recipe):
						switch recipe.kind {
							case EditorStatefulObject(_, _):
								editorObjectLabelsMatch = recipe.id == editorObject.id
									&& entry.labelEn == editorObject.labelEn
									&& entry.labelEsMx == editorObject.labelEsMx;
							case _: return 76;
						}
					case PaintTerrainAsset(_): return 76;
				}
		}
		for (right in left + 1...assets.length)
			if (entry.id == assets[right].id)
				return 76;
	}
	var firstItem:Null<EditorAssetEntry> = null;
	for (entry in assets)
		if (firstItem == null && entry.category == EditorAssetCategory.ItemAssets)
			firstItem = entry;
	final searchedItem = switch firstItem {
		case null: return 76;
		case value: value;
	};
	final translatedSearch = filterEditorAssets(assets, EditorAssetCategory.ItemAssets, searchedItem.labelEsMx.toLowerCase());
	var foundFirstItem = false;
	for (entry in translatedSearch)
		if (entry.id == searchedItem.id)
			foundFirstItem = true;
	if (assets.length != registry.blockCount()
		- 1
		+ registry.itemCount()
		+ registry.npcCount()
		+ registry.enemyCount()
		+ registry.editorObjectCount() || terrainAssets != registry.blockCount() - 1
		|| itemAssets != registry.itemCount()
		|| npcAssets != registry.npcCount()
		|| enemyAssets != registry.enemyCount()
		|| mechanismAssets != registry.editorObjectCount()
		|| !editorObjectLabelsMatch
		|| !foundFirstItem
		|| moveEditorAssetSelection(0, 3, -1) != 2
		|| moveEditorAssetSelection(2, 3, 1) != 0
		|| moveEditorAssetSelection(0, 0, 1) != -1)
		return 76;

	tracePack = registry.semanticProof();
	final sand = new ContentId("caxecraft:sand");
	final sandBlock = new ContentId("caxecraft:sand-block");
	final sandBlockCode = registry.itemStorageCode(sandBlock);
	final breadCode = registry.itemStorageCode(new ContentId("caxecraft:bread"));
	final sandPresentation = registry.itemPresentation(sandBlockCode);
	if (tracePack != 132089
		|| registry.packId() != "caxecraft:base"
		|| registry.packVersion() != 1
		|| registry.blockCount() != 10
		|| registry.itemCount() != 10
		|| registry.blockStorageCode(new ContentId("caxecraft:grass")) != 1
		|| registry.defaultEditorBlockId().text() != "caxecraft:dirt"
		|| registry.blockStorageCode(sand) != 5
		|| !registry.blockIsCollectable(sand)
		|| registry.blockDropItemStorageCode(sand) != sandBlockCode
		|| sandBlockCode != 7
		|| registry.itemUseProfile(sandBlockCode) != RuntimeItemUseProfile.PlaceBlock
		|| registry.itemPlacementBlockStorageCode(sandBlockCode) != 5
		|| registry.maximumItemQuantity(sandBlock) != 64
		|| breadCode < 0
		|| inventoryKindForRuntimeItem(registry, breadCode) != ItemKind.Bread
		|| sandPresentation == null
		|| sandPresentation.asset != "items"
		|| sandPresentation.cell != "sand-block"
		|| sandPresentation.cellIndex != 14
		|| registry.itemStorageCode(new ContentId("caxecraft:tideweave-suit")) != 9
		|| registry.maximumItemQuantity(new ContentId("caxecraft:berries")) != 64)
		return 6;
	switch registry.resolveFluid(new ContentId("caxecraft:water")) {
		case FluidContentResolved(_, 5):
		case _:
			return 7;
	}
	switch [
		registry.resolveActorPresentation(new ContentId("caxecraft:nia")),
		registry.resolveActorPresentation(new ContentId("caxecraft:mossling")),
		registry.resolveActorPresentation(new ContentId("caxecraft:ceesh"))
	] {
		case [
			ActorPresentationResolved("entities", 4),
			ActorPresentationResolved("entities", 8),
			ActorPresentationResolved("adventure-characters", 4)
		]:
		case _:
			return 39;
	}
	final markerId = new ScenarioId("editor.marker");
	final transform:ScenarioTransform = {
		xMilli: 1000,
		yMilli: 0,
		zMilli: 1000,
		yawDegrees: 0
	};
	switch [
		editorObjectVisualFor(registry, {id: markerId, tags: [], placement: Npc(new ContentId("caxecraft:nia"), markerId, transform)}),
		editorObjectVisualFor(registry, {id: markerId, tags: [], placement: Item(new ContentId("caxecraft:sand-block"), 1, transform)}),
		editorObjectVisualFor(registry, {
			id: markerId,
			tags: [],
			placement: StatefulObject(new ContentId("caxecraft:vault-gate"), new ContentId("caxecraft:sealed"), transform)
		})
	] {
		case [
			ActorVisual("entities", 4),
			ItemVisual("items", 14),
			StatefulObjectVisual("terrain", 10)
		]:
		case _:
			return 69;
	}
	switch [
		editorObjectVisualFor(registry, {id: markerId, tags: [], placement: PlayerSpawn(transform)}),
		editorObjectVisualFor(registry, {id: markerId, tags: [], placement: Checkpoint(transform)}),
		editorObjectVisualFor(registry, {
			id: markerId,
			tags: [],
			placement: TriggerZone({origin: {x: 0, y: 0, z: 0}, size: {width: 1, height: 1, depth: 1}})
		}),
		editorObjectVisualFor(registry, {id: markerId, tags: [], placement: Entity(new ContentId("caxecraft:missing"), transform)})
	] {
		case [PlayerSpawnVisual, CheckpointVisual, TriggerVolumeVisual, FallbackObjectVisual]:
		case _:
			return 70;
	}
	final gateId = new ContentId("caxecraft:vault-gate");
	final gateOpen = new ContentId("caxecraft:open");
	final gateSealed = new ContentId("caxecraft:sealed");
	if (registry.statefulObjectInteractionRadiusMilli(gateId) != 0
		|| registry.statefulObjectVisible(gateId, gateOpen)
		|| !registry.statefulObjectVisible(gateId, gateSealed))
		return 52;
	switch registry.resolveStatefulObject(gateId, gateSealed) {
		case StatefulObjectContentResolved(0, bounds, states, "terrain", 10)
			if (bounds.widthMilli == 7000 && bounds.heightMilli == 3000 && bounds.depthMilli == 500 && states.length == 2):
		case _:
			return 53;
	}

	final adventureEn = catalog.text(LocaleCursor.Locale0, UiMessage.MenuAdventure);
	final adventureEsMx = catalog.text(LocaleCursor.Locale1, UiMessage.MenuAdventure);
	if (catalog.localeCount() != 2
		|| catalog.messageCount() <= 0
		|| adventureEn.length == 0
		|| adventureEsMx.length == 0
		|| catalog.text(LocaleCursor.Locale1, UiMessage.EditorTitle).length == 0)
		return 8;
	if (catalog.templateCount() <= 0 || !allRequiredTemplatesExist(catalog))
		return 71;
	final eventArgument = "zone.harbor";
	final eventEn = catalog.format(Locale0, allFlowEventDescriptors()[0].editorLabel, [eventArgument]);
	final eventEsMx = catalog.format(Locale1, allFlowEventDescriptors()[0].editorLabel, [eventArgument]);
	final diagnostic = catalog.format(Locale0, new MessageId("scenario.diagnostic.stale-reference"), ["object.gone", "21", "5"]);
	if (eventEn.indexOf(eventArgument) < 0
		|| eventEsMx.indexOf(eventArgument) < 0
		|| diagnostic.indexOf("object.gone") < 0
		|| diagnostic.indexOf("21") < 0
		|| diagnostic.indexOf("5") < 0
		|| catalog.format(Locale0, new MessageId("scenario.diagnostic.stale-reference"), ["object.gone"]) != "")
		return 72;
	final mismatchedPlaceholders = removeFirstOccurrence(ui.bytes.toString(), "{1}");
	if (!rejectsUi(mismatchedPlaceholders, IncompatibleTypedCatalog))
		return 73;
	traceUi = catalog.localeCount() * 10 + adventureEsMx.length;
	if (traceUi <= catalog.localeCount() * 10)
		return 36;

	return negativeChecks();
}

/** Remove one placeholder without depending on the owning catalog sentence. */
function removeFirstOccurrence(source:String, needle:String):String {
	final at = source.indexOf(needle);
	return at < 0 ? "" : source.substring(0, at) + source.substring(at + needle.length);
}

/** Prove every code-owned key used by this slice exists in the data catalog. */
function allRequiredTemplatesExist(catalog:RuntimeUiCatalog):Bool {
	for (message in requiredCaxeFlowDiagnosticMessageIds())
		if (!catalog.hasTemplate(message))
			return false;
	for (message in requiredScenarioDiagnosticMessageIds())
		if (!catalog.hasTemplate(message))
			return false;
	for (message in allEditorFlowUiMessages())
		if (!catalog.hasTemplate(message.messageId()))
			return false;
	for (descriptor in allFlowEventDescriptors())
		if (!catalog.hasTemplate(descriptor.editorLabel))
			return false;
	for (descriptor in allFlowPredicateDescriptors())
		if (!catalog.hasTemplate(descriptor.editorLabel))
			return false;
	for (descriptor in allFlowActionDescriptors())
		if (!catalog.hasTemplate(descriptor.editorLabel))
			return false;
	return true;
}

/** Read one required package file without exposing the store's host root. */
function readRequired(store:ContentPackageStore, logicalPath:String):Null<LoadedPackageBytes> {
	return switch store.read(logicalPath) {
		case PackageBytesRead(value): value;
		case PackageBytesRejected(_): null;
	};
}

/** Exercise one representative mutation for every distinct pack family. */
function negativeChecks():Int {
	// This owner has the same function-lifetime shape as the application renderer.
	// Individual assertions below reuse it; they do not construct conditional
	// owners whose cleanup belongs to the separate haxe_c-71g capability.
	final animations = new VoxelFrameAnimationPlayer();
	final version = expectPackRejection(locatedUnsupportedVersionPack(), UnsupportedVersion);
	if (version == null)
		return 10;
	traceDiagnosticLine = version.line;
	if (version.line != 2 || version.column <= 0)
		return 11;
	final minimal = minimalPack();
	switch RuntimeContentPack.decode(Bytes.ofString(minimal)) {
		case RuntimeContentPackReady(registry):
			final objectId = new ContentId("caxecraft:glyph-control");
			final active = new ContentId("caxecraft:active");
			final idle = new ContentId("caxecraft:idle");
			final activePresentation = registry.statefulObjectPresentation(objectId, active);
			final idlePresentation = registry.statefulObjectPresentation(objectId, idle);
			if (!registry.hasStatefulObject(objectId)
				|| !registry.hasState(active)
				|| !registry.hasState(idle)
				|| registry.statefulObjectInteractionRadiusMilli(objectId) != 2500
				|| activePresentation == null
				|| activePresentation.cell != "glyph-leaf"
				|| idlePresentation == null
				|| idlePresentation.cell != "glyph-wave"
				|| !registry.statefulObjectVisible(objectId, active)
				|| registry.statefulObjectVisible(objectId, idle))
				return 45;
			switch [activePresentation.model, idlePresentation.model] {
				case [RuntimeVoxelAnimation(clip), RuntimeVoxelModel("assets/models/idle.vox", 32)]:
					if (clip.frameCount() != 3
						|| clip.pathAt(0) != "assets/models/idle.vox"
						|| clip.pathAt(1) != "assets/models/moving.vox"
						|| clip.pathAt(2) != "assets/models/active.vox"
						|| clip.durationAt(0) != 3
						|| clip.durationAt(1) != 2
						|| clip.durationAt(2) != 1
						|| clip.cellsPerAxis() != 32)
						return 55;
					final initial = animations.sample("control", "idle", 1, 20, idlePresentation.model);
					final activated = animations.sample("control", "active", 1, 21, activePresentation.model);
					final moving = animations.sample("control", "active", 1, 24, activePresentation.model);
					final held = animations.sample("control", "active", 1, 40, activePresentation.model);
					final reloaded = animations.sample("control", "active", 2, 41, activePresentation.model);
					if (initial == null
						|| initial.path != "assets/models/idle.vox"
						|| activated == null
						|| activated.path != "assets/models/idle.vox"
						|| moving == null
						|| moving.path != "assets/models/moving.vox"
						|| held == null
						|| held.path != "assets/models/active.vox"
						|| reloaded == null
						|| reloaded.path != "assets/models/active.vox") return 57;
				case _:
					return 55;
			}
			switch registry.resolveStatefulObject(objectId, active) {
				case StatefulObjectContentResolved(_, bounds, states, _, _):
					if (bounds.widthMilli != 1000
						|| bounds.heightMilli != 1000
						|| bounds.depthMilli != 1000
						|| states.length != 2
						|| states[0].state != active
						|| states[0].collision != StatefulObjectSolid
						|| !states[0].visible
						|| states[1].state != idle
						|| states[1].collision != StatefulObjectPassable
						|| states[1].visible) return 48;
				case UnknownStatefulObjectContent:
					return 48;
			}
		case RuntimeContentPackRejected(_):
			return 37;
	}
	final parsed = switch ContentJson.read(Bytes.ofString(minimal)) {
		case ContentJsonReady(root): root;
		case ContentJsonRejected(_): return 63;
	};
	if (!rejectsParsedPack(parsed,
		replaceValue([field("statefulObjects"), index(0), field("states"), index(1), field("id")], JsonString("caxecraft:missing")), UnresolvedReference))
		return 46;
	if (!rejectsParsedPack(parsed, replaceValue([field("statefulObjects"), index(0), field("states"), index(0), field("id")], JsonString("caxecraft:idle")),
		DuplicateId))
		return 47;
	if (!rejectsParsedPack(parsed, replaceValue([field("statefulObjects"), index(0), field("bounds"), field("widthMilli")], JsonNumber("0")), InvalidInteger))
		return 49;
	if (!rejectsParsedPack(parsed, replaceValue([
		field("statefulObjects"),
		index(0),
		field("states"),
		index(0),
		field("collision")
	], JsonString("blocking")), InvalidClosedValue))
		return 50;
	if (!rejectsParsedPack(parsed, replaceValue([field("statefulObjects"), index(0), field("states"), index(1), field("render")], JsonString("sometimes")),
		InvalidClosedValue))
		return 51;
	if (!rejectsParsedPack(parsed, replaceValue([
		field("statefulObjects"),
		index(0),
		field("states"),
		index(0),
		field("presentation"),
		field("model"),
		field("frames"),
		index(1),
		field("durationTicks")
	], JsonNumber("0")), InvalidInteger))
		return 56;
	if (!rejectsParsedPack(parsed, replaceValue([field("statefulObjects"), index(0), field("interaction")], JsonString("none")), InvalidInvariant))
		return 54;
	if (!rejectsParsedPack(parsed, replaceValue([field("assetCells"), index(1), field("id")], JsonString("adventure-items")), DuplicateId))
		return 41;
	if (!rejectsParsedPack(parsed, replaceValue([field("assetCells"), index(2), field("cells"), index(1)], JsonString("berries")), DuplicateValue))
		return 42;
	if (!rejectsParsedPack(parsed, replaceValue([field("assetCells"), index(0), field("id")], JsonString("zz-assets")), NonCanonicalOrder))
		return 43;
	if (!rejectsParsedPack(parsed, replaceValue([field("assetCells"), index(1), field("cells"), index(0)], JsonString("Mossling")), InvalidString))
		return 44;
	if (!rejectsParsedPack(parsed, removeField([], "packVersion"), MissingField))
		return 12;
	if (!rejectsParsedPack(parsed, renameField([], "packVersion", "surprise"), UnknownField))
		return 13;
	if (!rejectsParsedPack(parsed, replaceValue([field("packVersion")], JsonString("1")), WrongType))
		return 14;
	if (!rejectsParsedPack(parsed, replaceValue([field("blocks"), index(1), field("id")], JsonString("caxecraft:air")), DuplicateId))
		return 15;
	if (!rejectsParsedPack(parsed, replaceValue([field("effects"), index(0), field("id")], JsonString("caxecraft:core")), CrossKindId))
		return 16;
	if (!rejectsParsedPack(parsed, replaceValue([field("fluids"), index(0), field("simulationProfile")], JsonString("unbounded-water")), InvalidClosedValue))
		return 17;
	final missingAirPath = [field("airBlock")];
	final missingAirDiagnostic = expectParsedPackRejection(parsed, replaceValue(missingAirPath, JsonString("caxecraft:missing")), UnresolvedReference);
	if (!pointsAtNode(missingAirDiagnostic, nodeAt(parsed, missingAirPath)))
		return 34;
	final missingDefaultPath = [field("defaultAquaticProfile")];
	final missingDefaultDiagnostic = expectParsedPackRejection(parsed, replaceValue(missingDefaultPath, JsonString("caxecraft:missing")), UnresolvedReference);
	if (!pointsAtNode(missingDefaultDiagnostic, nodeAt(parsed, missingDefaultPath)))
		return 35;
	if (!rejectsParsedPack(parsed, replaceValue([field("items"), index(0), field("placementBlock")], JsonString("caxecraft:missing")), UnresolvedReference))
		return 18;
	if (!rejectsParsedPack(parsed, replaceValue([field("blocks"), index(1), field("dropItem")], JsonString("caxecraft:water")), WrongReferenceKind))
		return 19;
	if (!rejectsParsedPack(parsed, replaceValue([field("items"), index(1), field("aquaticProfile")], JsonString("caxecraft:missing")), UnresolvedReference))
		return 20;
	if (!rejectsParsedPack(parsed, replaceValue([field("enemies"), index(0), field("drop")], JsonString("caxecraft:missing")), UnresolvedReference))
		return 21;
	if (!rejectsParsedPack(parsed, replaceValue([field("fluids"), index(0), field("presentation"), field("asset")], JsonString("missing")), UnknownAsset))
		return 22;
	if (!rejectsParsedPack(parsed, replaceValue([field("fluids"), index(0), field("presentation"), field("cell")], JsonString("missing")), UnknownAssetCell))
		return 23;
	if (!rejectsParsedPack(parsed, replaceValue([field("blocks"), index(0), field("id")], JsonString("caxecraft:zz-air")), NonCanonicalOrder))
		return 24;
	if (!rejectsParsedPack(parsed, replaceValue([field("items"), index(0), field("maxStack")], JsonNumber("65")), InvalidInteger))
		return 25;
	if (!rejectsParsedPack(parsed, replaceValue([field("blocks"), index(1), field("storageCode")], JsonNumber("0")), DuplicateStorageCode))
		return 38;
	final prefabs = nodeAt(parsed, [field("prefabs")]);
	if (prefabs == null
		|| !rejectsParsedPack(parsed, replaceValue([field("prefabs")], JsonArray([new ContentJsonNode(JsonNull, prefabs.line, prefabs.column)])),
			UnsupportedReservedKind))
		return 26;
	if (!rejectsParsedPack(parsed, replaceValue([field("editorObjects"), index(0), field("objectType")], JsonString("caxecraft:missing")), UnresolvedReference))
		return 61;
	if (!rejectsParsedPack(parsed, replaceValue([field("editorObjects"), index(0), field("initialState")], JsonString("caxecraft:other")), InvalidInvariant))
		return 62;
	return uiNegativeChecks(minimalUiCatalog());
}

/** Build a tiny exact root whose unsupported version begins on source line two. */
function locatedUnsupportedVersionPack():String
	return '{\n"schemaVersion":3,"logicalPath":null,"packId":null,"packVersion":null,"assetManifestId":null,"assetCells":null,"airBlock":null,'
		+ '"defaultAquaticProfile":null,"features":null,"blocks":null,"fluids":null,"aquaticProfiles":null,"items":null,"npcs":null,"enemies":null,'
		+ '"drops":null,"effects":null,"editorObjects":null,"prefabs":null,"statefulObjects":null,"states":null,"signals":null}';

/**
 * Return one manually authored valid pack for fast negative sensitivity.
 *
 * The real positive path above remains the product tracer. This smaller source
 * contains every current reference family and closed mechanic but no copied
 * decoder algorithm, so each mutation can isolate one failure without paying
 * to rebuild the complete showcase registry repeatedly.
 */
function minimalPack():String
	return '{"schemaVersion":2,"logicalPath":"packs/test","packId":"caxecraft:test","packVersion":1,"assetManifestId":"caxecraft-showcase-v1-draft",'
		+ '"assetCells":[{"id":"adventure-items","cells":["glyph-wave","glyph-leaf","tideweave-suit-folded"]},'
		+ '{"id":"entities","cells":["mossling-front"]},'
		+ '{"id":"items","cells":["berries","grass-block"]},{"id":"terrain","cells":["teal-water"]}],'
		+ '"airBlock":"caxecraft:air","defaultAquaticProfile":"caxecraft:standard","features":["caxecraft:core"],'
		+ '"blocks":[{"id":"caxecraft:air","storageCode":0,"collision":"passable","edit":"immutable","dropItem":null,"renderProfile":"air"},'
		+ '{"id":"caxecraft:dirt","storageCode":1,"collision":"solid","edit":"collectable","dropItem":"caxecraft:block-item","renderProfile":"rich-soil"}],'
		+ '"fluids":[{"id":"caxecraft:water","simulationProfile":"bounded-water","renderProfile":"translucent-voxel","cameraProfile":"clear-submersion",'
		+ '"audioProfile":"fresh-water","presentation":{"asset":"terrain","cell":"teal-water"}}],'
		+ '"aquaticProfiles":[{"id":"caxecraft:standard","maximumBreathTicks":120,"breathRecoveryPerTick":4,"horizontalControlMilli":350,'
		+ '"ascentAccelerationMilli":14000,"descentAccelerationMilli":20000,"buoyancyAccelerationMilli":12000,"dragPerTickMilli":180,'
		+ '"drowningIntervalTicks":20,"underwaterMining":false,"coldProtection":false}],'
		+ '"items":[{"id":"caxecraft:block-item","maxStack":64,"useProfile":"place-block","placementBlock":"caxecraft:dirt","aquaticProfile":null,'
		+ '"icon":{"asset":"items","cell":"grass-block"}},{"id":"caxecraft:gear","maxStack":1,"useProfile":"equip-aquatic","placementBlock":null,'
		+ '"aquaticProfile":"caxecraft:standard","icon":{"asset":"adventure-items","cell":"tideweave-suit-folded"}},'
		+ '{"id":"caxecraft:item","maxStack":64,"useProfile":"none","placementBlock":null,"aquaticProfile":null,"icon":{"asset":"items","cell":"berries"}}],'
		+ '"npcs":[],"enemies":[{"id":"caxecraft:enemy","behaviorProfile":"wander-chase-melee","maxHealth":3,"noticeRadiusMilli":6000,'
		+ '"strikeRadiusMilli":3000,"attackRadiusMilli":1400,"windupTicks":8,"recoveryTicks":12,"stepMilli":80,"drop":"caxecraft:drop",'
		+ '"presentation":{"asset":"entities","cell":"mossling-front"}}],'
		+ '"drops":[{"id":"caxecraft:drop","item":"caxecraft:item","quantity":1,"pickupRadiusMilli":1500,"presentation":{"asset":"items","cell":"berries"}}],'
		+ '"effects":[{"id":"caxecraft:feedback","profile":"pickup-feedback"}],'
		+ '"editorObjects":[{"id":"glyph-control","kind":"stateful-object","label":{"en":"GLYPH CONTROL","es-MX":"CONTROL DE GLIFO"},'
		+ '"objectType":"caxecraft:glyph-control","initialState":"caxecraft:idle"}],"prefabs":[],'
		+ '"statefulObjects":[{"id":"caxecraft:glyph-control","interaction":"activate","interactionRadiusMilli":2500,'
		+ '"bounds":{"widthMilli":1000,"heightMilli":1000,"depthMilli":1000},'
		+ '"states":[{"id":"caxecraft:active","collision":"solid","render":"visible","presentation":{"asset":"adventure-items","cell":"glyph-leaf",'
		+ '"model":{"frames":[{"path":"assets/models/idle.vox","durationTicks":3},{"path":"assets/models/moving.vox","durationTicks":2},'
		+ '{"path":"assets/models/active.vox","durationTicks":1}],"cellsPerAxis":32}}},'
		+ '{"id":"caxecraft:idle","collision":"passable","render":"hidden","presentation":{"asset":"adventure-items","cell":"glyph-wave",'
		+ '"model":{"path":"assets/models/idle.vox","cellsPerAxis":32}}}]}],'
		+ '"states":["caxecraft:active","caxecraft:idle","caxecraft:other"],"signals":[]}';

/**
 * Return the first two correctly shaped typed messages for fast UI negatives.
 *
 * These mutations fail before catalog compatibility is checked. The complete
 * positive path proves that all shipped text maps to a typed key.
 */
function minimalUiCatalog():String
	return '{"schemaVersion":1,"catalogId":"caxecraft.ui","defaultLocale":"en","locales":["en","es-MX"],"messages":['
		+ '{"id":"aquatic_gear_equipped","text":{"en":"AQUATIC GEAR EQUIPPED","es-MX":"EQUIPO ACUATICO ACTIVADO"}},'
		+ '{"id":"brand","text":{"en":"CAXECRAFT  //  C + HAXE","es-MX":"CAXECRAFT  //  C + HAXE"}}],'
		+ '"templates":[{"id":"test.template","text":{"en":"TEST {0}","es-MX":"PRUEBA {0}"}}]}';

/** Exercise catalog identity, ordering, locale, message, and text bounds. */
function uiNegativeChecks(ui:String):Int {
	if (!rejectsUi(replaceOnce(ui, '"schemaVersion":1', '"schemaVersion":2'), UnsupportedVersion))
		return 27;
	if (!rejectsUi(replaceOnce(ui, '"defaultLocale":"en"', '"defaultLocale":"fr"'), InvalidLocale))
		return 28;
	if (!rejectsUi(replaceOnce(ui, '"id":"brand"', '"id":"aquatic_gear_equipped"'), DuplicateId))
		return 29;
	if (!rejectsUi(replaceOnce(ui, '"id":"aquatic_gear_equipped"', '"id":"zz_aquatic_gear_equipped"'), NonCanonicalOrder))
		return 30;
	if (!rejectsUiAt(replaceOnce(ui, '"id":"brand"', '"extra":"duplicate-owner","id":"brand"'), UnknownField, "messages[1].extra"))
		return 31;
	if (!rejectsUi(replaceOnce(ui, '"es-MX":"CAXECRAFT  //  C + HAXE"', '"fr":"CAXECRAFT  //  C + HAXE"'), InvalidLocale))
		return 32;
	if (!rejectsUi(replaceOnce(ui, '"en":"CAXECRAFT  //  C + HAXE"', '"en":""'), InvalidText))
		return 33;
	return 0;
}

/** One exact traversal step through the parser's closed JSON tree. */
private enum JsonPathStep {
	/** Select a named object field. */
	JsonField(name:String);

	/** Select a zero-based array element. */
	JsonIndex(index:Int);
}

/** One immutable edit applied at the end of a JSON-tree path. */
private enum JsonTreeOperation {
	/** Replace a value while preserving its source coordinate. */
	ReplaceJsonValue(value:ContentJsonValue);

	/** Remove one required field from an object. */
	RemoveJsonField(name:String);

	/** Rename one known field into an unknown field. */
	RenameJsonField(from:String, to:String);
}

/** A schema mutation whose path and operation are kept together. */
private typedef JsonTreeMutation = {
	/** Exact field/index path from the pack root. */
	final path:Array<JsonPathStep>;

	/** Immutable edit to apply at that path. */
	final operation:JsonTreeOperation;
}

/** Construct one field path step without exposing enum spelling at call sites. */
inline function field(name:String):JsonPathStep
	return JsonField(name);

/** Construct one array path step without exposing enum spelling at call sites. */
inline function index(value:Int):JsonPathStep
	return JsonIndex(value);

/** Describe one value replacement at an exact schema path. */
function replaceValue(path:Array<JsonPathStep>, value:ContentJsonValue):JsonTreeMutation
	return {path: path, operation: ReplaceJsonValue(value)};

/** Describe removal of one field from the object at an exact schema path. */
function removeField(path:Array<JsonPathStep>, name:String):JsonTreeMutation
	return {path: path, operation: RemoveJsonField(name)};

/** Describe renaming one field in the object at an exact schema path. */
function renameField(path:Array<JsonPathStep>, from:String, to:String):JsonTreeMutation
	return {path: path, operation: RenameJsonField(from, to)};

/**
 * Copy only the containers along one mutation path.
 *
 * Untouched parser-owned nodes remain shared and immutable. This keeps each
 * schema case independent without reparsing the same 3.3 KiB JSON source.
 */
function mutateNode(node:ContentJsonNode, path:Array<JsonPathStep>, operation:JsonTreeOperation):Null<ContentJsonNode> {
	return mutateNodeAt(node, path, 0, operation);
}

/** Copy the next path container without allocating a sliced path. */
function mutateNodeAt(node:ContentJsonNode, path:Array<JsonPathStep>, pathIndex:Int, operation:JsonTreeOperation):Null<ContentJsonNode> {
	if (pathIndex == path.length)
		return applyTreeOperation(node, operation);
	return switch path[pathIndex] {
		case JsonField(name):
			switch node.value {
				case JsonObject(fields):
					final copied = fields.copy();
					var found = false;
					for (fieldIndex in 0...copied.length) {
						final current = copied[fieldIndex];
						if (!found && current.name == name) {
							final changed = mutateNodeAt(current.value, path, pathIndex + 1, operation);
							if (changed == null)
								return null;
							copied[fieldIndex] = new ContentJsonField(current.name, changed, current.line, current.column);
							found = true;
						}
					}
					found ? new ContentJsonNode(JsonObject(copied), node.line, node.column) : null;
				case _: null;
			}
		case JsonIndex(arrayIndex):
			switch node.value {
				case JsonArray(values):
					if (arrayIndex < 0 || arrayIndex >= values.length)
						return null;
					final changed = mutateNodeAt(values[arrayIndex], path, pathIndex + 1, operation);
					if (changed == null)
						return null;
					final copied = values.copy();
					copied[arrayIndex] = changed;
					new ContentJsonNode(JsonArray(copied), node.line, node.column);
				case _: null;
			}
	};
}

/** Apply one mutation after its complete path has resolved. */
function applyTreeOperation(node:ContentJsonNode, operation:JsonTreeOperation):Null<ContentJsonNode> {
	return switch operation {
		case ReplaceJsonValue(value): new ContentJsonNode(value, node.line, node.column);
		case RemoveJsonField(name):
			switch node.value {
				case JsonObject(fields):
					final copied:Array<ContentJsonField> = [];
					var removed = false;
					for (current in fields) {
						if (!removed && current.name == name)
							removed = true;
						else
							copied.push(current);
					}
					removed ? new ContentJsonNode(JsonObject(copied), node.line, node.column) : null;
				case _: null;
			}
		case RenameJsonField(from, to):
			switch node.value {
				case JsonObject(fields):
					final copied = fields.copy();
					var renamed = false;
					for (fieldIndex in 0...copied.length) {
						final current = copied[fieldIndex];
						if (!renamed && current.name == from) {
							copied[fieldIndex] = new ContentJsonField(to, current.value, current.line, current.column);
							renamed = true;
						}
					}
					renamed ? new ContentJsonNode(JsonObject(copied), node.line, node.column) : null;
				case _: null;
			}
	};
}

/** Resolve one source-bearing node so location assertions stay explicit. */
function nodeAt(root:ContentJsonNode, path:Array<JsonPathStep>):Null<ContentJsonNode> {
	var current = root;
	for (step in path) {
		final next = switch step {
			case JsonField(name):
				switch current.value {
					case JsonObject(fields):
						var found:Null<ContentJsonNode> = null;
						for (field in fields)
							if (field.name == name)
								found = field.value;
						found;
					case _: null;
				}
			case JsonIndex(arrayIndex):
				switch current.value {
					case JsonArray(values):
						if (arrayIndex < 0 || arrayIndex >= values.length)
							return null;
						values[arrayIndex];
					case _: null;
				}
		};
		if (next == null)
			return null;
		current = next;
	}
	return current;
}

/** Return one located pack diagnostic when its family is the expected one. */
function expectPackRejection(source:String, family:ExpectedSchemaFamily):Null<RuntimeSchemaDiagnostic> {
	return switch RuntimeContentPack.decode(Bytes.ofString(source)) {
		case RuntimeContentPackRejected(diagnostic) if (sameFamily(diagnostic.kind, family) && diagnostic.line > 0 && diagnostic.column > 0): diagnostic;
		case _: null;
	};
}

/** Compare one immutable tree mutation with its intended rejection family. */
function rejectsParsedPack(root:ContentJsonNode, mutation:JsonTreeMutation, family:ExpectedSchemaFamily):Bool
	return expectParsedPackRejection(root, mutation, family) != null;

/** Apply one schema-only mutation and return its located rejection. */
function expectParsedPackRejection(root:ContentJsonNode, mutation:JsonTreeMutation, family:ExpectedSchemaFamily):Null<RuntimeSchemaDiagnostic> {
	final candidate = mutateNode(root, mutation.path, mutation.operation);
	if (candidate == null)
		return null;
	return switch RuntimeContentPack.decodeParsed(candidate) {
		case RuntimeContentPackRejected(diagnostic) if (sameFamily(diagnostic.kind, family) && diagnostic.line > 0 && diagnostic.column > 0): diagnostic;
		case _: null;
	};
}

/** Prove one rejection points at the exact mutated value node. */
function pointsAtNode(diagnostic:Null<RuntimeSchemaDiagnostic>, node:Null<ContentJsonNode>):Bool
	return diagnostic != null && node != null && diagnostic.line == node.line && diagnostic.column == node.column;

/** Decode one mutated UI catalog and compare its intended rejection family. */
function rejectsUi(source:String, family:ExpectedSchemaFamily):Bool {
	return switch RuntimeUiCatalog.decode(Bytes.ofString(source)) {
		case RuntimeUiCatalogRejected(diagnostic): sameFamily(diagnostic.kind, family) && diagnostic.line > 0 && diagnostic.column > 0;
		case RuntimeUiCatalogReady(_): false;
	};
}

/** Require one UI rejection family and exact schema path for sensitivity. */
function rejectsUiAt(source:String, family:ExpectedSchemaFamily, expectedPath:String):Bool {
	return switch RuntimeUiCatalog.decode(Bytes.ofString(source)) {
		case RuntimeUiCatalogRejected(diagnostic): final pathMatches = switch diagnostic.kind {
				case SchemaIncompatibleTypedCatalog(path): path == expectedPath;
				case SchemaUnknownField(path, field): '$path.$field' == expectedPath;
				case _: false;
			}; sameFamily(diagnostic.kind, family) && pathMatches && diagnostic.line > 0 && diagnostic.column > 0;
		case RuntimeUiCatalogReady(_): false;
	};
}

/** Test-only family names keep assertions precise without copying payload text. */
private enum ExpectedSchemaFamily {
	UnsupportedVersion;
	MissingField;
	UnknownField;
	WrongType;
	DuplicateId;
	DuplicateValue;
	InvalidString;
	CrossKindId;
	InvalidClosedValue;
	InvalidInvariant;
	UnresolvedReference;
	WrongReferenceKind;
	UnknownAsset;
	UnknownAssetCell;
	NonCanonicalOrder;
	InvalidInteger;
	DuplicateStorageCode;
	UnsupportedReservedKind;
	InvalidLocale;
	IncompatibleTypedCatalog;
	InvalidText;
}

/** Match one closed diagnostic constructor while retaining source location. */
function sameFamily(actual:RuntimeSchemaErrorKind, expected:ExpectedSchemaFamily):Bool {
	return switch [actual, expected] {
		case [SchemaUnsupportedVersion(_, _), UnsupportedVersion]: true;
		case [SchemaMissingField(_, _), MissingField]: true;
		case [SchemaUnknownField(_, _), UnknownField]: true;
		case [SchemaWrongType(_, _), WrongType]: true;
		case [SchemaDuplicateId(_, _), DuplicateId]: true;
		case [SchemaDuplicateValue(_, _), DuplicateValue]: true;
		case [SchemaInvalidString(_), InvalidString]: true;
		case [SchemaCrossKindId(_), CrossKindId]: true;
		case [SchemaInvalidClosedValue(_, _), InvalidClosedValue]: true;
		case [SchemaInvalidInvariant(_), InvalidInvariant]: true;
		case [SchemaUnresolvedReference(_, _, _), UnresolvedReference]: true;
		case [SchemaWrongReferenceKind(_, _, _), WrongReferenceKind]: true;
		case [SchemaUnknownAsset(_, _), UnknownAsset]: true;
		case [SchemaUnknownAssetCell(_, _, _), UnknownAssetCell]: true;
		case [SchemaNonCanonicalOrder(_), NonCanonicalOrder]: true;
		case [SchemaInvalidInteger(_, _, _), InvalidInteger]: true;
		case [SchemaDuplicateStorageCode(_), DuplicateStorageCode]: true;
		case [SchemaUnsupportedReservedKind(_), UnsupportedReservedKind]: true;
		case [SchemaInvalidLocale(_), InvalidLocale]: true;
		case [SchemaIncompatibleTypedCatalog(_), IncompatibleTypedCatalog]: true;
		case [SchemaInvalidText(_), InvalidText]: true;
		case _: false;
	};
}

/** Replace exactly the first reviewed token and leave later references intact. */
function replaceOnce(source:String, needle:String, replacement:String):String {
	final at = source.indexOf(needle);
	if (at < 0)
		return source;
	return source.substring(0, at) + replacement + source.substring(at + needle.length);
}

package caxecraft.editor;

import caxecraft.content.EditorObjectCatalog.EditorObjectRecipe;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipeKind;
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.MessageId;

/**
	Builds the child-facing asset browser from validated content registries.

	Runtime content remains authoritative for available blocks, items, actors,
	and mechanisms. The UI catalog owns translated names and help. This module
	joins those two data owners into copy-owned rows, then provides deterministic
	category, search, and selection operations without Raylib or widget state.
**/
/** Stable browser groups in the order children traverse them. */
enum abstract EditorAssetCategory(Int) {
	var TerrainAssets = 0;
	var ItemAssets = 1;
	var NpcAssets = 2;
	var EnemyAssets = 3;
	var MechanismAssets = 4;
}

/** The typed editor operation selected by one browser row. */
enum EditorAssetUse {
	/** Select or add one terrain palette material, then use the Ground tool. */
	PaintTerrainAsset(blockType:ContentId);

	/** Use the existing canonical object-placement path. */
	PlaceObjectAsset(recipe:EditorObjectRecipe);
}

/** One immutable localized row projected from authoritative data. */
typedef EditorAssetEntry = {
	final id:String;
	final category:EditorAssetCategory;
	final labelEn:String;
	final labelEsMx:String;
	final searchEn:String;
	final searchEsMx:String;
	final helpEn:String;
	final helpEsMx:String;
	final use:EditorAssetUse;
}

/** Return every stable category in child-facing navigation order. */
function allEditorAssetCategories():Array<EditorAssetCategory>
	return [TerrainAssets, ItemAssets, NpcAssets, EnemyAssets, MechanismAssets];

/** Stable technical token used only to derive catalog keys and traces. */
function editorAssetCategoryToken(category:EditorAssetCategory):String
	return switch category {
		case TerrainAssets: "terrain";
		case ItemAssets: "item";
		case NpcAssets: "npc";
		case EnemyAssets: "enemy";
		case MechanismAssets: "mechanism";
	};

/** Resolve one entry label for the selected validated locale. */
function editorAssetLabel(entry:EditorAssetEntry, locale:LocaleCursor):String
	return locale == LocaleCursor.Locale0 ? entry.labelEn : entry.labelEsMx;

/** Resolve one entry explanation for the selected validated locale. */
function editorAssetHelp(entry:EditorAssetEntry, locale:LocaleCursor):String
	return locale == LocaleCursor.Locale0 ? entry.helpEn : entry.helpEsMx;

/**
	Project every currently admitted terrain and placeable object recipe.

	Air is intentionally absent because Erase already owns that operation. The
	registry order is canonical, and category order is fixed above, so filters
	and controller navigation remain deterministic on Eval and generated C.
**/
function availableEditorAssets(registry:RuntimeContentRegistry, catalog:RuntimeUiCatalog):Array<EditorAssetEntry> {
	final result:Array<EditorAssetEntry> = [];
	for (index in 0...registry.blockCount()) {
		final id = registry.blockIdAt(index);
		if (id != null && id.text() != registry.airBlockId().text())
			result.push(contentEntry(catalog, TerrainAssets, id, PaintTerrainAsset(id), "terrain-" + localId(id)));
	}
	for (index in 0...registry.itemCount()) {
		final id = registry.itemIdAt(index);
		if (id != null)
			result.push(contentObjectEntry(catalog, ItemAssets, id, EditorItem(id, 1), "item-" + localId(id)));
	}
	for (index in 0...registry.npcCount()) {
		final id = registry.npcIdAt(index);
		if (id != null)
			result.push(contentObjectEntry(catalog, NpcAssets, id, EditorNpc(id), "npc-" + localId(id)));
	}
	for (index in 0...registry.enemyCount()) {
		final id = registry.enemyIdAt(index);
		if (id != null)
			result.push(contentObjectEntry(catalog, EnemyAssets, id, EditorEnemy(id), "enemy-" + localId(id)));
	}
	for (index in 0...registry.editorObjectCount()) {
		final recipe = registry.editorObjectAt(index);
		if (recipe != null)
			result.push({
				id: "mechanism-" + recipe.id,
				category: MechanismAssets,
				labelEn: recipe.labelEn,
				labelEsMx: recipe.labelEsMx,
				searchEn: recipe.labelEn.toLowerCase(),
				searchEsMx: recipe.labelEsMx.toLowerCase(),
				helpEn: categoryHelp(catalog, MechanismAssets, LocaleCursor.Locale0),
				helpEsMx: categoryHelp(catalog, MechanismAssets, LocaleCursor.Locale1),
				use: PlaceObjectAsset(recipe)
			});
	}
	return result;
}

/** Return rows in one category whose translated names contain the query. */
function filterEditorAssets(entries:Array<EditorAssetEntry>, category:EditorAssetCategory, query:String):Array<EditorAssetEntry> {
	final wanted = query.toLowerCase();
	final result:Array<EditorAssetEntry> = [];
	for (entry in entries)
		if (entry.category == category
			&& (wanted.length == 0 || entry.searchEn.indexOf(wanted) >= 0 || entry.searchEsMx.indexOf(wanted) >= 0))
			result.push(entry);
	return result;
}

/** Move once through a visible result list and wrap at both ends. */
function moveEditorAssetSelection(current:Int, count:Int, direction:Int):Int {
	if (count <= 0)
		return -1;
	if (current < 0 || current >= count)
		return direction < 0 ? count - 1 : 0;
	if (direction < 0)
		return current == 0 ? count - 1 : current - 1;
	if (direction > 0)
		return current + 1 == count ? 0 : current + 1;
	return current;
}

/** Build one terrain row and resolve both validated locales once. */
private function contentEntry(catalog:RuntimeUiCatalog, category:EditorAssetCategory, contentId:ContentId, use:EditorAssetUse,
		entryId:String):EditorAssetEntry {
	final labelEn = label(catalog, contentId, LocaleCursor.Locale0);
	final labelEsMx = label(catalog, contentId, LocaleCursor.Locale1);
	return {
		id: entryId,
		category: category,
		labelEn: labelEn,
		labelEsMx: labelEsMx,
		searchEn: labelEn.toLowerCase(),
		searchEsMx: labelEsMx.toLowerCase(),
		helpEn: categoryHelp(catalog, category, LocaleCursor.Locale0),
		helpEsMx: categoryHelp(catalog, category, LocaleCursor.Locale1),
		use: use
	};
}

/** Share one localized name pair between a browser row and its typed recipe. */
private function contentObjectEntry(catalog:RuntimeUiCatalog, category:EditorAssetCategory, contentId:ContentId, kind:EditorObjectRecipeKind,
		entryId:String):EditorAssetEntry {
	final labelEn = label(catalog, contentId, LocaleCursor.Locale0);
	final labelEsMx = label(catalog, contentId, LocaleCursor.Locale1);
	return {
		id: entryId,
		category: category,
		labelEn: labelEn,
		labelEsMx: labelEsMx,
		searchEn: labelEn.toLowerCase(),
		searchEsMx: labelEsMx.toLowerCase(),
		helpEn: categoryHelp(catalog, category, LocaleCursor.Locale0),
		helpEsMx: categoryHelp(catalog, category, LocaleCursor.Locale1),
		use: PlaceObjectAsset(new EditorObjectRecipe(entryId, labelEn, labelEsMx, kind))
	};
}

/** Resolve one content ID through a mechanically derived data-catalog key. */
private function label(catalog:RuntimeUiCatalog, contentId:ContentId, locale:LocaleCursor):String
	return catalog.format(locale, new MessageId("editor.asset." + contentId.text().split(":").join(".") + ".label"), []);

/** Resolve one shared category explanation without copying prose into code. */
private function categoryHelp(catalog:RuntimeUiCatalog, category:EditorAssetCategory, locale:LocaleCursor):String
	return catalog.format(locale, new MessageId("editor.asset.category." + editorAssetCategoryToken(category) + ".help"), []);

/** Keep generated object IDs readable without preserving a namespace colon. */
private function localId(id:ContentId):String {
	final parts = id.text().split(":");
	return parts.length == 2 ? parts[1] : parts.join("-");
}

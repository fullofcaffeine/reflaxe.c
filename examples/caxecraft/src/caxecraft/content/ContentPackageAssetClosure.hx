package caxecraft.content;

import caxecraft.content.ContentJson.ContentJsonField;
import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentJson.ContentJsonValue;
import caxecraft.content.ContentPackageManifest.ContentPackageEntry;
import caxecraft.content.ContentPackageManifest.ContentPackageEntryKind;
import caxecraft.content.ContentPackageManifest.ContentPackageManifest;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaErrorKind;
import caxecraft.content.RuntimeSchema.RuntimeSchemaReader;
import haxe.io.Bytes;

/**
 * Checks that package-owned asset files match the asset inventory's selection.
 *
 * The package manifest owns transport: it lists every file that must travel.
 * The asset manifest independently owns which visual IDs ship at runtime and
 * the path and digest for each ID. This module joins those two authored views
 * before export. An omitted or substituted atlas or voxel model cannot produce
 * a plausible complete package.
 */
/** Exact cross-manifest reason a package is not a complete asset closure. */
enum ContentPackageAssetClosureError {
	AssetInventorySchemaRejected(diagnostic:RuntimeSchemaDiagnostic);
	AssetManifestEntryMissing;
	MultipleAssetManifestEntries;
	SelectedAssetUnknown(id:String);
	SelectedAssetEntryMissing(path:String);
	SelectedAssetReceiptMismatch(path:String);
	UnselectedAssetEntry(path:String);
}

/** Asset closure validation either accepts the exact count or rejects it. */
enum ContentPackageAssetClosureResult {
	AssetClosureReady(selectedCount:Int);
	AssetClosureRejected(error:ContentPackageAssetClosureError);
}

/** One typed inventory fact needed to bind a selected ID to package bytes. */
private final class ContentPackageAssetFact {
	/** Stable visual ID used by `packagedPrimaryAssets`. */
	public final id:String;

	/** Path relative to the asset manifest's containing directory. */
	public final relativePath:ContentPackagePath;

	/** Independently authored lowercase payload digest. */
	public final sha256:String;

	public function new(id:String, relativePath:ContentPackagePath, sha256:String) {
		this.id = id;
		this.relativePath = relativePath;
		this.sha256 = sha256;
	}
}

/** Parsed runtime selection plus every inventory record it may name. */
private final class ContentPackageAssetInventory {
	/** Selected stable IDs in authored order. */
	public final selected:Array<String>;

	/** Complete validated asset records from the inventory. */
	public final assets:Array<ContentPackageAssetFact>;

	public function new(selected:Array<String>, assets:Array<ContentPackageAssetFact>) {
		this.selected = selected;
		this.assets = assets;
	}
}

/** Typed inventory parsing returns complete facts or one located diagnostic. */
private enum ContentPackageAssetInventoryResult {
	AssetInventoryReady(inventory:ContentPackageAssetInventory);
	AssetInventoryRejected(diagnostic:RuntimeSchemaDiagnostic);
}

/**
 * Cross-check one package manifest against its declared asset inventory bytes.
 *
 * The inventory path is discovered from the package itself. Every selected
 * asset must have one package `asset` entry below that inventory's directory,
 * with the same digest; every package `asset` entry must be selected.
 */
function verifyContentPackageAssetClosure(manifest:ContentPackageManifest, assetInventoryBytes:Bytes):ContentPackageAssetClosureResult {
	var assetManifest:Null<ContentPackageEntry> = null;
	for (index in 0...manifest.entryCount()) {
		final entry = manifest.entryAt(index);
		switch entry.kind {
			case AssetManifest:
				if (assetManifest != null)
					return AssetClosureRejected(MultipleAssetManifestEntries);
				assetManifest = entry;
			case _:
		}
	}
	if (assetManifest == null)
		return AssetClosureRejected(AssetManifestEntryMissing);
	final inventory = switch decodeAssetInventory(assetInventoryBytes) {
		case AssetInventoryRejected(diagnostic): return AssetClosureRejected(AssetInventorySchemaRejected(diagnostic));
		case AssetInventoryReady(value): value;
	};
	final manifestEntry:ContentPackageEntry = assetManifest;
	final manifestPath = manifestEntry.logicalPath.text();
	final slash = manifestPath.lastIndexOf("/");
	final prefix = slash < 0 ? "" : manifestPath.substring(0, slash + 1);
	for (selectedId in inventory.selected) {
		var selected:Null<ContentPackageAssetFact> = null;
		for (asset in inventory.assets)
			if (asset.id == selectedId)
				selected = asset;
		if (selected == null)
			return AssetClosureRejected(SelectedAssetUnknown(selectedId));
		final fact:ContentPackageAssetFact = selected;
		final expectedPath = prefix + fact.relativePath.text();
		var packageEntry:Null<ContentPackageEntry> = null;
		for (index in 0...manifest.entryCount()) {
			final candidate = manifest.entryAt(index);
			switch candidate.kind {
				case Asset if (candidate.logicalPath.text() == expectedPath):
					packageEntry = candidate;
				case _:
			}
		}
		if (packageEntry == null)
			return AssetClosureRejected(SelectedAssetEntryMissing(expectedPath));
		final matched:ContentPackageEntry = packageEntry;
		if (matched.sha256 != fact.sha256)
			return AssetClosureRejected(SelectedAssetReceiptMismatch(expectedPath));
	}
	for (index in 0...manifest.entryCount()) {
		final entry = manifest.entryAt(index);
		switch entry.kind {
			case Asset:
				var selected = false;
				for (selectedId in inventory.selected)
					for (asset in inventory.assets)
						if (asset.id == selectedId && prefix + asset.relativePath.text() == entry.logicalPath.text())
							selected = true;
				if (!selected)
					return AssetClosureRejected(UnselectedAssetEntry(entry.logicalPath.text()));
			case _:
		}
	}
	return AssetClosureReady(inventory.selected.length);
}

/** Decode only the exact inventory facts that package export is allowed to use. */
private function decodeAssetInventory(input:Bytes):ContentPackageAssetInventoryResult {
	final reader = new RuntimeSchemaReader();
	final root = reader.parse(input);
	if (root == null)
		return inventoryRejected(reader);
	final rootFields = reader.object(root, "assetManifest", [
		"schemaVersion",
		"packId",
		"status",
		"coordinateOrder",
		"reproducibility",
		"runtimeIntegration",
		"rights",
		"generationRecords",
		"assets"
	]);
	if (rootFields == null)
		return inventoryRejected(reader);
	final version = reader.integer(reader.field(rootFields, "schemaVersion"), "schemaVersion", 1, 1);
	if (version == null)
		return inventoryRejected(reader);
	final runtimeNode = reader.field(rootFields, "runtimeIntegration");
	final runtimeFields = reader.object(runtimeNode, "runtimeIntegration", [
		"owner",
		"packagedPrimaryAssets",
		"designOnlyAssets",
		"pathPolicy",
		"lifetimePolicy",
		"derivation"
	]);
	if (runtimeFields == null)
		return inventoryRejected(reader);
	final selectedNodes = reader.array(reader.field(runtimeFields, "packagedPrimaryAssets"), "runtimeIntegration.packagedPrimaryAssets", 1, 128);
	if (selectedNodes == null)
		return inventoryRejected(reader);
	final selected:Array<String> = [];
	for (index in 0...selectedNodes.length) {
		final path = "runtimeIntegration.packagedPrimaryAssets[" + index + "]";
		final id = reader.string(selectedNodes[index], path, 64);
		if (id == null)
			return inventoryRejected(reader);
		for (previous in selected)
			if (previous == id) {
				reader.reject(selectedNodes[index], SchemaDuplicateId("runtimeIntegration.packagedPrimaryAssets", id));
				return inventoryRejected(reader);
			}
		selected.push(id);
	}
	final assetNodes = reader.array(reader.field(rootFields, "assets"), "assets", 1, 128);
	if (assetNodes == null)
		return inventoryRejected(reader);
	final assets:Array<ContentPackageAssetFact> = [];
	for (index in 0...assetNodes.length) {
		final base = "assets[" + index + "]";
		final fields = assetFields(reader, assetNodes[index], base);
		if (fields == null)
			return inventoryRejected(reader);
		final id = reader.string(reader.field(fields, "id"), base + ".id", 64);
		final pathNode = reader.field(fields, "path");
		final pathText = reader.string(pathNode, base + ".path", ContentPackagePath.MAXIMUM_PATH_BYTES);
		final shaNode = reader.field(fields, "sha256");
		final sha = reader.string(shaNode, base + ".sha256", 64);
		if (id == null || pathText == null || sha == null)
			return inventoryRejected(reader);
		for (previous in assets)
			if (previous.id == id) {
				reader.reject(assetNodes[index], SchemaDuplicateId("assets", id));
				return inventoryRejected(reader);
			}
		final logicalPath = switch ContentPackagePath.parse(pathText) {
			case PathRejected(_):
				reader.reject(pathNode, SchemaInvalidLogicalPath(base + ".path"));
				return inventoryRejected(reader);
			case PathAccepted(value): value;
		};
		if (!validSha256(sha)) {
			reader.reject(shaNode, SchemaInvalidString(base + ".sha256"));
			return inventoryRejected(reader);
		}
		assets.push(new ContentPackageAssetFact(id, logicalPath, sha));
	}
	return AssetInventoryReady(new ContentPackageAssetInventory(selected, assets));
}

/** Admit either exact asset-record shape, depending on whether it owns a grid. */
private function assetFields(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String):Null<Array<ContentJsonField>> {
	final hasGrid = switch node.value {
		case JsonObject(fields):
			var found = false;
			for (field in fields)
				if (field.name == "grid")
					found = true;
			found;
		case _: false;
	};
	final hasDepth = switch node.value {
		case JsonObject(fields):
			var found = false;
			for (field in fields)
				if (field.name == "depth")
					found = true;
			found;
		case _: false;
	};
	final names = ["id", "path", "kind", "width", "height", "sha256", "generationRecord", "rights"];
	if (hasDepth)
		names.push("depth");
	else
		names.push("alpha");
	if (hasGrid)
		names.push("grid");
	return reader.object(node, path, names);
}

/** Match the package manifest's lowercase SHA-256 spelling exactly. */
private function validSha256(value:String):Bool {
	if (value.length != 64)
		return false;
	for (index in 0...value.length) {
		final code = value.charCodeAt(index);
		if (!((code >= 48 && code <= 57) || (code >= 97 && code <= 102)))
			return false;
	}
	return true;
}

/** Preserve the first schema diagnostic after a failed inventory decode. */
private function inventoryRejected(reader:RuntimeSchemaReader):ContentPackageAssetInventoryResult {
	final failure = reader.failure;
	return failure == null ? AssetInventoryRejected({line: 1, column: 1, kind: SchemaInvalidInvariant("assetManifest")}) : AssetInventoryRejected(failure);
}

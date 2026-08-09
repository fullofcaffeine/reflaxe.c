package caxecraft.content;

/**
 * Owns the visual names decoded from one verified runtime content pack.
 *
 * Creators add atlas cells in data, so a content-only reload can use them
 * without rebuilding Caxecraft. The decoder validates the ordered names before
 * it constructs this value. The backing arrays never escape this module.
 */
final class RuntimeAssetInventory {
	final manifestIdValue:String;
	final assets:Array<RuntimeAssetFacts>;

	/** Retain one validated manifest identity and a copy of its runtime cells. */
	@:allow(caxecraft.content.RuntimeContentPack)
	private function new(manifestId:String, assets:Array<RuntimeAssetFacts>) {
		this.manifestIdValue = manifestId;
		this.assets = assets.copy();
	}

	/** Exact asset-package ID named by the decoded content pack. */
	public inline function manifestId():String
		return manifestIdValue;

	/** True when the decoded catalog contains one asset ID. */
	public function hasAsset(id:String):Bool {
		for (asset in assets)
			if (asset.id == id)
				return true;
		return false;
	}

	/** Return the exact cell index, or -1 when the asset/cell pair is absent. */
	public function cellIndex(assetId:String, cellId:String):Int {
		for (asset in assets)
			if (asset.id == assetId)
				for (index in 0...asset.cellCount())
					if (asset.cellAt(index) == cellId)
						return index;
		return -1;
	}
}

/** One decoded visual asset and its ordered semantic cell names. */
final class RuntimeAssetFacts {
	/** Stable asset ID used by content presentation records. */
	public final id:String;

	/** Manifest-ordered cell IDs; callers receive no mutable reference. */
	final cells:Array<String>;

	/** Construct one immutable asset fact set from schema-checked content. */
	public function new(id:String, cells:Array<String>) {
		this.id = id;
		this.cells = cells.copy();
	}

	/** Number of ordered cells in this asset. */
	public inline function cellCount():Int
		return cells.length;

	/** Read one bounds-checked cell name. */
	public inline function cellAt(index:Int):String
		return cells[index];
}

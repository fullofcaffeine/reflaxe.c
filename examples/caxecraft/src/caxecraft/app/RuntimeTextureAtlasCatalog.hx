package caxecraft.app;

#if c
import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.ContentPackagePath;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.RuntimeSchema.RuntimeSchemaReader;
import raylib.Camera3D;
import raylib.Texture2D;
import raylib.Vector3;

/**
 * Owns reloadable tile atlases that are not part of the original fixed renderer.
 *
 * The launcher writes a validated runtime-asset receipt beside the executable.
 * This catalog reads that receipt, loads every additional tile atlas, and keeps
 * the GPU handle paired with its authored grid dimensions. New campaign art can
 * therefore be staged and drawn without adding its ID to compiled Haxe. The
 * original renderer atlases remain separate for now so this focused migration
 * does not load the same GPU resource twice.
 */
final class RuntimeTextureAtlasCatalog {
	final atlases:Array<RuntimeTextureAtlas>;

	/** Retain sole ownership of the already-loaded atlas records. */
	private function new(atlases:Array<RuntimeTextureAtlas>)
		this.atlases = atlases;

	/**
	 * Parse the staged receipt and load every additional tile atlas.
	 *
	 * A malformed receipt, duplicate ID, unsafe path, or failed texture load
	 * returns `null`. Any handles loaded before a later failure are released
	 * before returning, so callers either receive the complete catalog or no GPU
	 * ownership at all.
	 */
	public static function load(path:String):Null<RuntimeTextureAtlasCatalog> {
		final store = switch ContentPackageStore.open(".", "runtime-assets", 1024 * 1024) {
			case PackageStoreOpened(value): value;
			case PackageStoreRejected(_): return null;
		};
		final bytes = switch store.read(path) {
			case PackageBytesRead(content): content.bytes;
			case PackageBytesRejected(_): return null;
		};
		final reader = new RuntimeSchemaReader();
		final root = reader.parse(bytes);
		if (root == null)
			return null;
		final fields = reader.object(root, "runtimeAssets", ["schemaVersion", "packId", "assets"]);
		if (fields == null)
			return null;
		final version = reader.integer(reader.field(fields, "schemaVersion"), "schemaVersion", 1, 1);
		final packId = reader.string(reader.field(fields, "packId"), "packId", 128);
		final values = reader.array(reader.field(fields, "assets"), "assets", 1, 32);
		if (version == null || packId == null || !RuntimeSchemaReader.validProfile(packId) || values == null)
			return null;

		final loaded:Array<RuntimeTextureAtlas> = [];
		final seen:Array<String> = [];
		for (index in 0...values.length) {
			final record = readRecord(reader, values[index], index);
			if (record == null) {
				unloadLoaded(loaded);
				return null;
			}
			for (id in seen)
				if (id == record.id) {
					unloadLoaded(loaded);
					return null;
				}
			seen.push(record.id);
			if (record.kind != "tile-atlas" || isLegacyRendererAtlas(record.id))
				continue;
			final texture = CaxecraftTextures.loadRuntime("assets/" + record.path);
			if (!CaxecraftTextures.isValid(texture)) {
				unloadLoaded(loaded);
				return null;
			}
			loaded.push(new RuntimeTextureAtlas(record.id, record.columns, record.rows, texture));
		}
		return new RuntimeTextureAtlasCatalog(loaded);
	}

	/** Draw one authored atlas cell over a box, or report that this catalog does not own the asset. */
	public function drawBox(asset:String, cellIndex:Int, position:Vector3, width:Float, height:Float, depth:Float):Bool {
		final atlas = find(asset);
		return atlas == null ? false : CaxecraftAtlas.drawAtlasBox(atlas.texture, cellIndex, atlas.columns, atlas.rows, position, width, height, depth);
	}

	/** Draw one authored atlas cell as a world billboard when this catalog owns the asset. */
	public function drawSprite(camera:Camera3D, asset:String, cellIndex:Int, position:Vector3, width:Float, height:Float):Bool {
		final atlas = find(asset);
		if (atlas == null)
			return false;
		CaxecraftAtlas.drawAtlasSprite(camera, atlas.texture, cellIndex, atlas.columns, atlas.rows, position, width, height);
		return true;
	}

	/** Release every valid GPU texture exactly once before the graphics device closes. */
	public function unload():Void
		unloadLoaded(atlases);

	/** Return the one matching owned atlas without exposing the mutable backing array. */
	private function find(id:String):Null<RuntimeTextureAtlas> {
		for (atlas in atlases)
			if (atlas.id == id)
				return atlas;
		return null;
	}

	/** Decode one closed launcher receipt record. */
	private static function readRecord(reader:RuntimeSchemaReader, node:ContentJsonNode, index:Int):Null<RuntimeTextureAtlasRecord> {
		final path = "assets[" + index + "]";
		final fields = reader.object(node, path, ["columns", "id", "kind", "path", "rows", "sha256"]);
		if (fields == null)
			return null;
		final id = reader.string(reader.field(fields, "id"), path + ".id", 128);
		final kind = reader.string(reader.field(fields, "kind"), path + ".kind", 64);
		final logicalPath = reader.string(reader.field(fields, "path"), path + ".path", 256);
		final columns = reader.integer(reader.field(fields, "columns"), path + ".columns", 1, 64);
		final rows = reader.integer(reader.field(fields, "rows"), path + ".rows", 1, 64);
		final sha256 = reader.string(reader.field(fields, "sha256"), path + ".sha256", 64);
		if (id == null
			|| kind == null
			|| logicalPath == null
			|| columns == null
			|| rows == null
			|| sha256 == null
			|| !RuntimeSchemaReader.validProfile(id)
			|| !runtimeAssetPathIsValid(logicalPath)
			|| sha256.length != 64)
			return null;
		return {
			id: id,
			kind: kind,
			path: logicalPath,
			columns: columns,
			rows: rows
		};
	}

	/** Validate the eventual executable-relative spelling, including its file extension. */
	private static function runtimeAssetPathIsValid(path:String):Bool
		return switch ContentPackagePath.parse("assets/" + path) {
			case PathAccepted(_): true;
			case PathRejected(_): false;
		};

	/** Keep original fixed atlas owners out of this migration to avoid loading one GPU resource twice. */
	private static function isLegacyRendererAtlas(id:String):Bool
		return id == "hud"
			|| id == "items"
			|| id == "adventure-items"
			|| id == "adventure-terrain"
			|| id == "entities"
			|| id == "terrain";

	/** Release a complete or partially loaded list in reverse acquisition order. */
	private static function unloadLoaded(values:Array<RuntimeTextureAtlas>):Void {
		var index = values.length - 1;
		while (index >= 0) {
			CaxecraftTextures.unload(values[index].texture);
			index--;
		}
	}
}

/** Validated launcher facts needed before a native texture is loaded. */
private typedef RuntimeTextureAtlasRecord = {
	final id:String;
	final kind:String;
	final path:String;
	final columns:Int;
	final rows:Int;
}

/** One owned GPU texture paired with its authored atlas layout. */
private final class RuntimeTextureAtlas {
	public final id:String;
	public final columns:Int;
	public final rows:Int;
	public final texture:Texture2D;

	/** Construct one owner only after Raylib confirms that the handle is valid. */
	public function new(id:String, columns:Int, rows:Int, texture:Texture2D) {
		this.id = id;
		this.columns = columns;
		this.rows = rows;
		this.texture = texture;
	}
}
#end

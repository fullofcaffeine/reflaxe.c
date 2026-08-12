package caxecraft.app;

#if c
import raylib.Vector3;
import raylib.raw.Model;
import raylib.raw.Raylib as RawRaylib;

/**
 * Loads, draws, and releases voxel models named by reloadable game content.
 *
 * A model loads on its first visible frame. The catalog keeps one native
 * owner for that path. Every later draw uses a borrowed copy of the handle.
 * The application calls `unload` once before it closes the Raylib window.
 */
final class RuntimeVoxelModelCatalog {
	/** Model-space width of one source cell in the pinned Raylib VOX loader. */
	static inline final RAYLIB_VOX_CELL_UNITS = 0.25;

	final loaded:Array<RuntimeVoxelModel>;
	final rejectedPaths:Array<String>;
	var released:Bool;

	/** Create an empty catalog. No model loads before content asks for it. */
	public function new() {
		loaded = [];
		rejectedPaths = [];
		released = false;
	}

	/**
	 * Draw one cubic VOX model inside the authored world bounds.
	 *
	 * The model path comes from the validated content pack. An invalid model
	 * returns false, so the caller can draw its atlas fallback instead.
	 */
	public function draw(path:String, centerX:Float, groundY:Float, centerZ:Float, width:Float, height:Float, depth:Float, yawDegrees:Int,
			cellsPerAxis:Int):Bool {
		if (released)
			return false;
		final halfX = width * 0.5;
		final halfZ = depth * 0.5;
		var rotatedHalfX = halfX;
		var rotatedHalfZ = halfZ;
		switch yawDegrees {
			case 0:
			case 90:
				rotatedHalfX = halfZ;
				rotatedHalfZ = -halfX;
			case 180:
				rotatedHalfX = -halfX;
				rotatedHalfZ = -halfZ;
			case 270:
				rotatedHalfX = -halfZ;
				rotatedHalfZ = halfX;
			case _:
				return false;
		}
		for (rejected in rejectedPaths)
			if (rejected == path)
				return false;
		var asset = find(path);
		if (asset == null) {
			final model = RawRaylib.LoadModel(c.CStringRef.to(path));
			// Raylib's software renderer retains CPU mesh data but creates no GPU
			// buffers. Its IsModelValid check therefore rejects every usable model.
			// The package verifier already proved these exact asset bytes. Desktop
			// builds keep Raylib's stronger GPU-backed validity check.
			#if raylib_configuration_desktop
			if (!RawRaylib.IsModelValid(model)) {
				rejectedPaths.push(path);
				Sys.println("caxecraft: voxel model rejected: " + path);
				return false;
			}
			#end
			final created = new RuntimeVoxelModel(path, model);
			loaded.push(created);
			asset = created;
		}
		final owned:RuntimeVoxelModel = asset;
		final sourceUnits = cellsPerAxis * RAYLIB_VOX_CELL_UNITS;
		RawRaylib.DrawModelEx(owned.model, Vector3.fromFloat(centerX - rotatedHalfX, groundY, centerZ - rotatedHalfZ), Vector3.fromFloat(0.0, 1.0, 0.0),
			c.Float32.fromFloat(yawDegrees), Vector3.fromFloat(width / sourceUnits, height / sourceUnits, depth / sourceUnits), CaxecraftPalette.textureTint());
		return true;
	}

	/** Release each valid native model exactly once. */
	public function unload():Void {
		if (released)
			return;
		for (asset in loaded)
			RawRaylib.UnloadModel(asset.model);
		released = true;
	}

	/** Find the sole owner for a path, or null before its first load. */
	function find(path:String):Null<RuntimeVoxelModel> {
		for (asset in loaded)
			if (asset.path == path)
				return asset;
		return null;
	}
}

/** One valid model and its stable package path. */
private final class RuntimeVoxelModel {
	/** Package-relative path used as the cache key. */
	public final path:String;

	/** Native model owned by this entry until catalog shutdown. */
	public final model:Model;

	/** Retain one valid native model without copying its ownership. */
	public function new(path:String, model:Model) {
		this.path = path;
		this.model = model;
	}
}
#end

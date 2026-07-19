package hxc.config;

/** One named overlay, sorted by name before it enters the resolver. */
class HxcNamedConfigOverlay {
	public final name:String;
	public final patch:HxcConfigPatch;

	public function new(name:String, patch:HxcConfigPatch) {
		if (!~/^[A-Za-z][A-Za-z0-9_-]*$/.match(name)) {
			throw new HxcConfigError('overlay name `$name` must match [A-Za-z][A-Za-z0-9_-]*');
		}
		this.name = name;
		this.patch = patch;
	}
}

/** Strict schema-1 contents of one checked-in `hxc.json`. */
class HxcProjectConfig {
	public static inline final SCHEMA_VERSION = 1;
	public static inline final SCHEMA_ID = "https://reflaxe-c.dev/schemas/hxc.schema.json";

	public final patch:HxcConfigPatch;
	public final overlays:Array<HxcNamedConfigOverlay>;

	public function new(patch:HxcConfigPatch, overlays:Array<HxcNamedConfigOverlay>) {
		this.patch = patch;
		this.overlays = overlays.copy();
		this.overlays.sort((left, right) -> compareUtf8(left.name, right.name));
		var prior:Null<String> = null;
		for (overlay in this.overlays) {
			if (prior == overlay.name) {
				throw new HxcConfigError('duplicate named overlay `${overlay.name}`');
			}
			prior = overlay.name;
		}
	}

	public function overlay(name:String):HxcConfigPatch {
		for (candidate in overlays) {
			if (candidate.name == name) {
				return candidate.patch;
			}
		}
		throw new HxcConfigError('unknown configuration overlay `$name`');
	}

	public static function rootKeys():Array<String>
		return [
			"$schema",
			"artifact",
			"build",
			"cExtensions",
			"cStandard",
			"environment",
			"hxml",
			"output",
			"overlays",
			"profile",
			"projectLayout",
			"runtime",
			"runtimeDiagnostics",
			"schemaVersion"
		];

	public static function overlayKeys():Array<String>
		return [
			"artifact",
			"build",
			"cExtensions",
			"cStandard",
			"environment",
			"hxml",
			"output",
			"profile",
			"projectLayout",
			"runtime",
			"runtimeDiagnostics"
		];

	public static function compareUtf8(left:String, right:String):Int {
		final leftBytes = haxe.io.Bytes.ofString(left);
		final rightBytes = haxe.io.Bytes.ofString(right);
		final count = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...count) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}
}

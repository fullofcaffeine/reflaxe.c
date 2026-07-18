package hxc.config;

/**
	A normalized path rooted at the directory containing `hxc.json`.

	Only portable `/` spelling enters the semantic model. Host-native absolute
	paths are derived by `HxcLoadedProjectConfig` and never serialized.
**/
class HxcProjectPath {
	public final value:String;

	public function new(raw:String, label:String = "project path") {
		value = normalize(raw, label);
	}

	public function toString():String
		return value;

	public static function normalize(raw:String, label:String):String {
		if (raw == null || raw == "" || StringTools.trim(raw) != raw) {
			throw new HxcConfigError('$label must be a non-empty path without surrounding whitespace');
		}
		if (StringTools.startsWith(raw, "/") || StringTools.startsWith(raw, "~") || raw.indexOf("\\") != -1 || raw.indexOf("\x00") != -1
			|| ~/^[A-Za-z]:/.match(raw)) {
			throw new HxcConfigError('$label must use portable config-root-relative `/` spelling: `$raw`');
		}

		final parts:Array<String> = [];
		for (part in raw.split("/")) {
			switch part {
				case "" | ".":
				case "..":
					throw new HxcConfigError('$label cannot contain a parent segment: `$raw`');
				case value:
					for (index in 0...value.length) {
						final code = value.charCodeAt(index);
						if (code == null || code < 0x20 || code == 0x7F) {
							throw new HxcConfigError('$label contains a control character');
						}
					}
					parts.push(value);
			}
		}
		if (parts.length == 0) {
			throw new HxcConfigError('$label resolves to the project root; name a child path');
		}
		return parts.join("/");
	}
}

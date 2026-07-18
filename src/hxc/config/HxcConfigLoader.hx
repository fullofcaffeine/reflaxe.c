package hxc.config;

import haxe.io.Path;
import sys.FileSystem;
import sys.io.File;

/** Filesystem adapter that roots every project path at the config directory. */
class HxcConfigLoader {
	public static function load(configPath:String):HxcLoadedProjectConfig {
		final source = logicalSource(configPath);
		final fullPath = try {
			FileSystem.fullPath(configPath);
		} catch (_:haxe.Exception) {
			throw new HxcConfigError("cannot normalize the configuration path", source);
		};
		if (!FileSystem.exists(fullPath) || FileSystem.isDirectory(fullPath)) {
			throw new HxcConfigError("configuration file does not exist or is not a regular file", source);
		}
		final text = try {
			File.getContent(fullPath);
		} catch (_:haxe.Exception) {
			throw new HxcConfigError("configuration file cannot be read", source);
		};
		return new HxcLoadedProjectConfig(HxcConfigParser.parse(text, source), Path.directory(fullPath), source);
	}

	static function logicalSource(configPath:String):String {
		final normalized = StringTools.replace(configPath, "\\", "/");
		final name = normalized.substring(normalized.lastIndexOf("/") + 1);
		if (name == "" || name == "." || name == ".." || StringTools.startsWith(name, "~") || ~/^[A-Za-z]:/.match(name)) {
			return "hxc.json";
		}
		for (index in 0...name.length) {
			final code = name.charCodeAt(index);
			if (code == null || code < 0x20 || code == 0x7F) {
				return "hxc.json";
			}
		}
		return name;
	}
}

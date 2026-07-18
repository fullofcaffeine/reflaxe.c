package hxc.config;

import haxe.io.Path;

/**
	A parsed project plus its private host root.

	`rootDirectory` is for host I/O only. Inspection uses the stable logical
	`configSource` and project-relative `HxcProjectPath` values instead.
**/
class HxcLoadedProjectConfig {
	public final config:HxcProjectConfig;
	public final rootDirectory:String;
	public final configSource:String;

	public function new(config:HxcProjectConfig, rootDirectory:String, configSource:String) {
		this.config = config;
		this.rootDirectory = Path.normalize(rootDirectory);
		this.configSource = configSource;
	}

	public function resolveHostPath(path:HxcProjectPath):String
		return Path.normalize(Path.join([rootDirectory, path.value]));
}

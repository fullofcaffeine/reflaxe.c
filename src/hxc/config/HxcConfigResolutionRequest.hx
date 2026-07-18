package hxc.config;

/** All optional layers consumed by the target-neutral resolver. */
class HxcConfigResolutionRequest {
	public final project:Null<HxcLoadedProjectConfig>;
	public final environmentPreset:Null<HxcConfigPatch>;
	public final environmentPresetName:Null<String>;
	public final selectedOverlay:Null<String>;
	public final cli:Null<HxcConfigPatch>;
	public final directDefines:Array<HxcDirectDefine>;

	public function new(?project:HxcLoadedProjectConfig, ?environmentPreset:HxcConfigPatch, ?environmentPresetName:String, ?selectedOverlay:String,
			?cli:HxcConfigPatch, ?directDefines:Array<HxcDirectDefine>) {
		if ((environmentPreset == null) != (environmentPresetName == null)) {
			throw new HxcConfigError("an environment preset and its stable name must be supplied together", "command-line");
		}
		if (selectedOverlay != null && project == null) {
			throw new HxcConfigError("a named overlay requires an hxc.json project file", "command-line");
		}
		this.project = project;
		this.environmentPreset = environmentPreset;
		this.environmentPresetName = environmentPresetName;
		this.selectedOverlay = selectedOverlay;
		this.cli = cli;
		this.directDefines = directDefines == null ? [] : directDefines.copy();
	}
}

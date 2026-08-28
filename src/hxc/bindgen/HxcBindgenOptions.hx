package hxc.bindgen;

/** Validated command inputs for one Clang translation-unit capture. */
class HxcBindgenOptions {
	/** Entry header passed to Clang. */
	public final header:String;

	/** Exact Clang executable spelling selected by the caller. */
	public final clang:String;

	/** Optional requested target; absence selects Clang's reported machine. */
	public final target:Null<String>;

	/** Ordered include search roots, because order can change declarations. */
	public final includeDirectories:Array<String>;

	/** Ordered preprocessor definitions passed as individual arguments. */
	public final defines:Array<String>;

	/** Directory that owns the resulting binding lock. */
	public final outputDirectory:String;

	/** If true, return lock bytes without writing the output directory. */
	public final dryRun:Bool;

	/** Copy validated parser values into one immutable request. */
	public function new(header:String, clang:String, target:Null<String>, includeDirectories:Array<String>, defines:Array<String>, outputDirectory:String,
			dryRun:Bool) {
		this.header = header;
		this.clang = clang;
		this.target = target;
		this.includeDirectories = includeDirectories.copy();
		this.defines = defines.copy();
		this.outputDirectory = outputDirectory;
		this.dryRun = dryRun;
	}
}

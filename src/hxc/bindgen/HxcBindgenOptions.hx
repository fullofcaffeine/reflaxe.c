package hxc.bindgen;

/** Closed Clang language modes admitted by the semantic-lock stage. */
enum abstract HxcBindgenLanguage(String) to String {
	var C = "c";
	var Cxx = "c++";

	/** Parse one command-line language spelling without admitting Clang aliases. */
	public static function parse(value:String):Null<HxcBindgenLanguage>
		return switch value {
			case "c": C;
			case "c++": Cxx;
			case _: null;
		};
}

/** One validated preprocessor definition with stable name/value identity. */
class HxcBindgenDefine {
	/** C preprocessor identifier. */
	public final name:String;

	/** Replacement text, or absence for a valueless `-DNAME`. */
	public final value:Null<String>;

	/** Retain one validated definition. */
	public function new(name:String, value:Null<String>) {
		this.name = name;
		this.value = value;
	}

	/** Render the exact Clang argument payload. */
	public function spelling():String
		return value == null ? name : '$name=$value';
}

/** Named parser result used to construct one immutable bindgen request safely. */
typedef HxcBindgenOptionValues = {
	final entryHeaders:Array<String>;
	final clang:String;
	final target:Null<String>;
	final language:HxcBindgenLanguage;
	final languageExplicit:Bool;
	final sysroot:Null<String>;
	final includeDirectories:Array<String>;
	final defines:Array<HxcBindgenDefine>;
	final outputDirectory:String;
	final dryRun:Bool;
}

/** Validated command inputs for one Clang translation-unit capture. */
class HxcBindgenOptions {
	/** Ordered entry headers that define the reachable declaration set. */
	public final entryHeaders:Array<String>;

	/** Exact Clang executable spelling selected by the caller. */
	public final clang:String;

	/** Optional requested target; absence selects Clang's reported machine. */
	public final target:Null<String>;

	/** Exact Clang parser language. */
	public final language:HxcBindgenLanguage;

	/** Whether the language came from `--language` rather than the C default. */
	public final languageExplicit:Bool;

	/** Optional target sysroot selected by the caller. */
	public final sysroot:Null<String>;

	/** Ordered include search roots, because order can change declarations. */
	public final includeDirectories:Array<String>;

	/** Ordered preprocessor definitions passed as individual arguments. */
	public final defines:Array<HxcBindgenDefine>;

	/** Directory that owns the resulting binding lock. */
	public final outputDirectory:String;

	/** If true, return lock bytes without writing the output directory. */
	public final dryRun:Bool;

	/** Copy validated parser values into one immutable request. */
	public function new(values:HxcBindgenOptionValues) {
		this.entryHeaders = values.entryHeaders.copy();
		this.clang = values.clang;
		this.target = values.target;
		this.language = values.language;
		this.languageExplicit = values.languageExplicit;
		this.sysroot = values.sysroot;
		this.includeDirectories = values.includeDirectories.copy();
		this.defines = values.defines.copy();
		this.outputDirectory = values.outputDirectory;
		this.dryRun = values.dryRun;
	}
}

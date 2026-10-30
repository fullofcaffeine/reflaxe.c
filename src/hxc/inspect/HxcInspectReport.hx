package hxc.inspect;

/** Closed names for the stable schema-1 inspection views. */
enum abstract HxcInspectReport(String) to String {
	var Config = "config";
	var Manifest = "manifest";
	var TypedInventory = "typed-inventory";
	var HxcIR = "hxcir";
	var CAst = "c-ast";
	var Lowering = "lowering";
	var Runtime = "runtime";
	var Symbols = "symbols";
	var Includes = "includes";
	var Build = "build";
	var Macros = "macros";
	var Declarations = "declarations";
	var Stdlib = "stdlib";
	var Abi = "abi";
	var Sizes = "sizes";
	var All = "all";

	/** Parse one public report name without accepting aliases that could drift. */
	public static function parse(value:String):Null<HxcInspectReport> {
		return switch value {
			case "manifest": Manifest;
			case "config": Config;
			case "typed-inventory": TypedInventory;
			case "hxcir": HxcIR;
			case "c-ast": CAst;
			case "lowering": Lowering;
			case "runtime": Runtime;
			case "symbols": Symbols;
			case "includes": Includes;
			case "build": Build;
			case "declarations": Declarations;
			case "macros": Macros;
			case "stdlib": Stdlib;
			case "abi": Abi;
			case "sizes": Sizes;
			case "all": All;
			case _: null;
		};
	}

	/** Return names in stable help and `all` order. */
	public static function all():Array<HxcInspectReport>
		return [
			Manifest,
			Config,
			TypedInventory,
			HxcIR,
			CAst,
			Lowering,
			Runtime,
			Symbols,
			Includes,
			Build,
			Declarations,
			Macros,
			Stdlib,
			Abi,
			Sizes,
			All
		];
}

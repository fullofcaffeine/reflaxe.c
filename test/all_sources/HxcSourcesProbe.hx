import Run;

/**
	Type-check the complete host-side `hxc` product independently of compiler macros.

	The command router is an Eval host tool today. Keeping its graph separate
	from the custom-target compiler graph gives each owner a bounded check while
	the runner still verifies the union of every repository-owned Haxe source.
**/
class HxcSourcesProbe {
	static function main():Void {
		if (Type.getClassName(Run) != "Run")
			throw "the hxc bootstrap entry point lost its root module identity";
		Sys.println("hxc-sources: OK");
	}
}

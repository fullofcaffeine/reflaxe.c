package hxc.config;

/** One direct Haxe define admitted at the highest configuration precedence. */
class HxcDirectDefine {
	public final name:String;
	public final value:String;

	public function new(name:String, value:String) {
		if (name == null || name == "" || value == null || value == "") {
			throw new HxcConfigError("direct Haxe define names and values must be non-empty", "command-line");
		}
		this.name = name;
		this.value = value;
	}
}

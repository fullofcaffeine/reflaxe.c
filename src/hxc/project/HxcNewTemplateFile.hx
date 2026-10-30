package hxc.project;

/**
	One immutable file embedded from the authoritative project-template data.

	The compiler macro creates these records at build time. Runtime generation
	only substitutes reviewed placeholders and never carries a second copy of a
	template in authored Haxe.
**/
class HxcNewTemplateFile {
	/** Template kind that owns this file. */
	public final kind:HxcNewKind;

	/** Normalized project-relative output path, which may contain a placeholder. */
	public final path:String;

	/** UTF-8 text read from the corresponding authoritative template file. */
	public final contents:String;

	/** Construct one macro-validated embedded template file. */
	public function new(kind:HxcNewKind, path:String, contents:String) {
		this.kind = kind;
		this.path = path;
		this.contents = contents;
	}
}

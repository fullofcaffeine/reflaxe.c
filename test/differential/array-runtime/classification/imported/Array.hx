package imported;

/** A header-owned namesake must keep its import representation. */
@:c.layout(c.Layout.Struct)
@:c.name("probe_array")
@:c.include("probe.h", c.IncludeKind.Local)
extern class Array {
	public var value:Int;
}

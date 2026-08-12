/** Invalid field-hidden struct without the required extern ownership marker. */
@:c.layout(c.Layout.Struct)
@:c.include("pointlib.h", c.IncludeKind.Local)
@:c.name("pointlib_hidden_point")
class HiddenPoint {}

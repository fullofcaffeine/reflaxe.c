/**
 * Header-complete point whose private C fields are not part of the Haxe API.
 *
 * Haxe can store and pass this value. Only pointlib can construct or inspect
 * its fields, so a header revision does not leak pointer or layout details.
 */
@:c.layout(c.Layout.Struct)
@:c.include("pointlib.h", c.IncludeKind.Local)
@:c.name("pointlib_hidden_point")
extern class HiddenPoint {}

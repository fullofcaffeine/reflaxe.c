package;

@:c.layout(c.Layout.Opaque)
@:c.name("sqlite3")
@:c.include("sqlite3.h", c.IncludeKind.System)
@:c.include("sqlite3.h", c.IncludeKind.System)
@:c.link("sqlite3")
@:c.pkgConfig("sqlite3")
@:c.define("SQLITE_THREADSAFE", 1)
extern class SQLite3 {}

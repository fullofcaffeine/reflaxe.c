/**
	Proves compiler-owned StringMap construction never enters class dispatch.

	The map lives in an ordinary Haxe class field and stores a nominal String
	identity. Eval and generated C must both preserve that value while haxe.c's
	dedicated StringMap owner selects representation, lifetime, and operations.
**/

/** A domain identity that keeps its immutable String carrier nominal. */
private abstract ItemId(String) {
	public inline function new(value:String)
		this = value;

	/** Return visible text without erasing the storage type at the map boundary. */
	public inline function text():String
		return this;
}

/** Own one shared typed table through a normal constructor and class field. */
private final class ItemTable {
	final values:Map<String, ItemId> = [];

	/** Construct an empty table; the field initializer creates its StringMap. */
	public function new() {}

	/** Insert or replace one exact domain identity. */
	public function set(key:String, value:ItemId):Void
		values.set(key, value);

	/** Inlining must preserve StringMap ownership through Haxe's generic map view. */
	public inline function get(key:String):Null<ItemId>
		return values.get(key);
}

/** Executable semantic oracle shared by Eval and strict generated C. */
final class Main {
	static function main():Void {
		final table = new ItemTable();
		table.set("item", new ItemId("caxecraft:item"));
		final found = table.get("item");
		if (found == null || found.text() != "caxecraft:item")
			throw "nominal StringMap value changed";
		if (table.get("missing") != null)
			throw "missing StringMap value became present";
	}
}

/**
 * A collected map owns its String keys even when the source uses only literals.
 * No String operation or iterator should incidentally select that key runtime.
 */
private enum Tree {
	Leaf(value:Int);
	Group(children:Array<Tree>);
}

/** Keep the key-runtime selection independent of separately managed fields. */
private typedef Entry = {final tree:Tree;}

/** Exercise only map storage and lookup with independently stated payload data. */
final class Main {
	/** A literal key still requires the map's generated copy and release callbacks. */
	static function main():Void {
		final map:Map<String, Entry> = [];
		map.set("entry", {tree: Group([Leaf(7)])});
		final found = map.get("entry");
		if (found == null)
			throw "entry missing";
		switch found.tree {
			case Group(children):
				switch children[0] {
					case Leaf(value): if (value != 7) throw "payload changed";
					case _: throw "leaf changed";
				}
			case _:
				throw "group changed";
		}
		map.clear();
	}
}

import haxe.io.Bytes;

/**
	Keep recursive values alive through a map after their creating function returns.
	The record combines collector edges with a separately owned Bytes buffer.
	Copies share the nested values but retain independent map membership.
**/
private enum Tree {
	Leaf(value:Int);
	Group(children:Array<Tree>);
	Wrapped(child:Tree);
}

/** An immutable record keeps identity in its shared children, not its carrier. */
private typedef Entry = {
	final tree:Tree;
	final bytes:Bytes;
}

/** A class descriptor must trace its map after the creating function returns. */
private final class EntryTable {
	final entries:Map<String, Entry> = [];

	/** Construct one table through the ordinary field initializer. */
	public function new() {}

	/** Preserve exact record ownership across a normal method boundary. */
	public function set(value:Entry):Void
		entries.set("item", value);

	/** An inlined generic map view must keep the collector table's identity. */
	public inline function get():Null<Entry>
		return entries.get("item");
}

/** Exercise map ownership using ordinary Haxe and independently stated results. */
final class Main {
	/** Return the only remaining owner of a cyclic child graph and a byte buffer. */
	static function make():Map<String, Entry> {
		final children:Array<Tree> = [Leaf(7)];
		children.push(Wrapped(Group(children)));
		final bytes = Bytes.alloc(1);
		bytes.set(0, 3);
		final map:Map<String, Entry> = [];
		map.set("item", {tree: Wrapped(Group(children)), bytes: bytes});
		return map;
	}

	/** Return a class whose map is the only remaining owner of another graph. */
	static function table():EntryTable {
		final source = make();
		final value = source.get("item");
		if (value == null)
			throw "table source missing";
		final result = new EntryTable();
		result.set(value);
		return result;
	}

	/** Force allocations while the only retained tree is stored in map slots. */
	static function pressure():Void {
		for (_ in 0...40000) {
			final temporary:Array<Tree> = [Leaf(1)];
			if (temporary.length != 1)
				throw "temporary length changed";
		}
	}

	/** Return a snapshot after all source map entries have been removed. */
	static function snapshot():Iterator<Entry> {
		final map = make();
		final result = map.iterator();
		map.clear();
		return result;
	}

	/** Returned key bytes outlive their table and remain ordinary owned Strings. */
	static function keys():Iterator<String> {
		final map = make();
		final result = map.keys();
		map.clear();
		return result;
	}

	/** A pair snapshot owns both its key and its exact nested record value. */
	static function pairs():KeyValueIterator<String, Entry> {
		final map = make();
		final result = map.keyValueIterator();
		return result;
	}

	/** Mutate shared contents, then remove all map roots and retain a snapshot. */
	static function main():Void {
		final map = make();
		final copy = map.copy();
		final table = table();
		pressure();
		final stored = table.get();
		if (stored == null || stored.bytes.get(0) != 3)
			throw "class field lost map values";
		final found = copy.get("item");
		if (found == null)
			throw "map lost its entry";
		found.bytes.set(0, 11);
		final original = map.get("item");
		if (original == null || original.bytes.get(0) != 11)
			throw "map copy lost shared bytes";
		final replacement = Bytes.alloc(1);
		replacement.set(0, 17);
		map.set("it" + String.fromCharCode(101) + "m", {tree: Leaf(23), bytes: replacement});
		final replaced = map.get("item");
		if (replaced == null || replaced.bytes.get(0) != 17 || original.bytes.get(0) != 11)
			throw "replacement changed the copied value";
		if (!map.remove("item") || !copy.exists("item"))
			throw "map copy shared membership";
		final snapshot = snapshot();
		final keys = keys();
		final pairs = pairs();
		copy.clear();
		pressure();
		if (!snapshot.hasNext())
			throw "snapshot lost its entry";
		final retained = snapshot.next();
		if (retained.bytes.get(0) != 3)
			throw "snapshot lost bytes";
		if (!keys.hasNext() || keys.next() != "item" || keys.hasNext())
			throw "key snapshot changed";
		if (!pairs.hasNext())
			throw "pair snapshot lost entry";
		final pair = pairs.next();
		if (pair.key != "item" || pair.value.bytes.get(0) != 3 || pairs.hasNext())
			throw "pair snapshot changed";
		switch retained.tree {
			case Wrapped(Group(children)):
				if (children.length != 2)
					throw "snapshot lost recursive children";
			case _:
				throw "tree constructor changed";
		}
		if (snapshot.hasNext() || copy.get("item") != null)
			throw "map clear changed snapshot bounds";
	}
}

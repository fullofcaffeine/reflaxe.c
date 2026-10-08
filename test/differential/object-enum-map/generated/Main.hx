/**
	Provides the ordinary-Haxe oracle for typed ObjectMap and EnumValueMap keys.

	The probe keeps object identity distinct from field equality and combines
	fieldless, primitive-payload, nested-enum, and object-payload enum keys. Its
	collector pressure leaves important objects reachable only through a map, so
	the generated program must trace occupied key and value slots exactly.
**/
final class Main {
	/** Build an object-keyed map whose aliases share one mutable identity. */
	static function objectIdentity():Map<MapNode, MapNode> {
		final map:Map<MapNode, MapNode> = [];
		final alias = map;
		final first = new MapNode(7);
		final sameFields = new MapNode(7);
		final value = new MapNode(70);
		map.set(first, value);
		alias.set(sameFields, new MapNode(71));
		map.set(first, new MapNode(72));
		final removedKey = new MapNode(8);
		map.set(removedKey, new MapNode(80));
		if (!alias.exists(removedKey) || !map.remove(removedKey) || map.exists(removedKey))
			while (true) {}
		final copied = map.copy();
		copied.set(first, new MapNode(73));
		if (map.get(first).value != 72 || copied.get(first).value != 73 || map.get(sameFields).value != 71)
			while (true) {}
		return map;
	}

	/**
		Keep one key and one value alive only through occupied ObjectMap slots.

		The returned map has no surviving authored aliases to its inserted objects.
	**/
	static function objectReachability():Map<MapNode, MapNode> {
		final map:Map<MapNode, MapNode> = [];
		map.set(new MapNode(11), new MapNode(12));
		return map;
	}

	/** Compare enum keys recursively while keeping object payloads identity-based. */
	static function enumIdentity():Map<MapToken, MapNode> {
		final map:Map<MapToken, MapNode> = [];
		final held = new MapNode(20);
		final sameFields = new MapNode(20);
		map.set(Unit, new MapNode(1));
		map.set(Other, new MapNode(6));
		map.set(Pair(2, false), new MapNode(2));
		map.set(Nested(Number(3)), new MapNode(3));
		map.set(Held(held), new MapNode(4));
		map.set(Pair(2, false), new MapNode(22));
		map.set(Held(sameFields), new MapNode(5));
		map.set(Nested(Empty), new MapNode(30));
		if (!map.exists(Nested(Empty)) || !map.remove(Nested(Empty)) || map.exists(Nested(Empty)))
			while (true) {}
		if (map.get(Unit).value != 1
			|| map.get(Other).value != 6
			|| map.get(Pair(2, false)).value != 22
			|| map.get(Nested(Number(3))).value != 3
			|| map.get(Held(held)).value != 4
			|| map.get(Held(sameFields)).value != 5)
			while (true) {}
		return map;
	}

	/** Allocate enough short-lived objects to force the deterministic collector. */
	static function forceCollectionPressure():Void {
		for (index in 0...40000)
			new MapNode(index);
	}

	/** Count one ObjectMap snapshot without depending on hash-table order. */
	static function objectSnapshot(map:Map<MapNode, MapNode>):Int {
		final keys = map.keys();
		final values = map.iterator();
		final pairs = map.keyValueIterator();
		var total = 0;
		// Pinned Eval performs live value lookup for key/value cursors. Consume this
		// cursor before mutation; the separately authored native test owns haxe.c's
		// complete entry-snapshot contract after remove and clear.
		while (pairs.hasNext()) {
			final pair = pairs.next();
			total += pair.key.value + pair.value.value;
		}
		map.clear();
		while (keys.hasNext())
			total += keys.next().value;
		while (values.hasNext())
			total += values.next().value;
		return total;
	}

	/** Count EnumValueMap snapshots after source mutation and release pressure. */
	static function enumSnapshot(map:Map<MapToken, MapNode>):Int {
		final copied = map.copy();
		final values = copied.iterator();
		final pairs = copied.keyValueIterator();
		var total = 0;
		while (pairs.hasNext())
			total += pairs.next().value.value;
		copied.remove(Unit);
		copied.clear();
		while (values.hasNext())
			total += values.next().value;
		return total;
	}

	/** Run identity, recursive equality, tracing, copy, mutation, and snapshots. */
	static function main():Void {
		final identities = objectIdentity();
		final retained = objectReachability();
		final enumValues = enumIdentity();
		forceCollectionPressure();

		final retainedPair = retained.keyValueIterator().next();
		final objectTotal = objectSnapshot(identities);
		final enumTotal = enumSnapshot(enumValues);
		while (retainedPair.key.value != 11 || retainedPair.value.value != 12 || objectTotal != 314 || enumTotal != 82) {}
	}
}

/** An identity-bearing key/value with mutable state visible through map aliases. */
final class MapNode {
	public var value:Int;

	/** Create one object whose fields deliberately do not define key equality. */
	public function new(value:Int) {
		this.value = value;
	}
}

/** Nested enum payload used to prove recursive value comparison. */
enum MapAtom {
	Empty;
	Number(value:Int);
}

/** Closed key family spanning fieldless, primitive, nested, and object payloads. */
enum MapToken {
	Unit;
	Other;
	Pair(number:Int, enabled:Bool);
	Nested(value:MapAtom);
	Held(value:MapNode);
}

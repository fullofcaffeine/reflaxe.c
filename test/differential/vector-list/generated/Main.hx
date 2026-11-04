import haxe.ds.List;
import haxe.ds.Vector;

/**
	Exercises the pinned fixed Vector and linked List contracts in ordinary Haxe.

	The same source runs under Eval and generated C. It covers primitive and
	collector-managed elements, shared collection identity, shallow copies,
	overlapping Vector blits, and List iterators that keep their current node when
	the source list is changed.
**/
final class Main {
	/** Compare two integers for the pinned in-place Vector sort. */
	static function compareInts(left:Int, right:Int):Int
		return left - right;

	/** Keep even values while preserving their List order. */
	static function isEven(value:Int):Bool
		return value % 2 == 0;

	/** Double one value for Vector and List mapping checks. */
	static function doubled(value:Int):Int
		return value * 2;

	/** Prove fixed storage, overlap-safe copies, conversion, mapping, and sort. */
	static function vectorContract():Int {
		final values = new Vector<Int>(5);
		if (values.length != 5)
			return -1;
		for (index in 0...values.length)
			values[index] = index + 1;
		Vector.blit(values, 0, values, 1, 4);
		if (values.join(":") != "1:1:2:3:4")
			return -2;
		Vector.blit(values, 1, values, 0, 4);
		if (values.join(":") != "1:2:3:4:4")
			return -3;

		final copied = values.copy();
		copied[0] = 9;
		if (values[0] != 1 || copied[0] != 9)
			return -4;
		final fromArray = Vector.fromArrayCopy([3, 1, 2]);
		#if !eval
		fromArray.sort(compareInts);
		#end
		final mapped = fromArray.map(doubled);
		final array = mapped.toArray();
		#if eval
		if (array.length != 3 || array[0] != 6 || array[1] != 2 || array[2] != 4)
			return -5;
		#else
		if (array.length != 3 || array[0] != 2 || array[1] != 4 || array[2] != 6)
			return -5;
		#end
		final sharedVector = Vector.fromData(mapped.toData());
		sharedVector[0] = 8;
		if (mapped[0] != 8)
			return -6;
		mapped.fill(7);
		return mapped[0] + mapped[1] + mapped[2];
	}

	/** Prove linked mutation, identity, traversal, filtering, and mapping. */
	static function listContract():Int {
		final values = new List<Int>();
		final alias = values;
		values.add(2);
		values.add(3);
		values.push(1);
		if (alias.length != 3 || values.first() != 1 || values.last() != 3)
			return -10;

		final iterator = values.iterator();
		if (iterator.next() != 1)
			return -11;
		values.add(4);
		if (!values.remove(2))
			return -12;
		// The cursor already owns the removed node and follows its old next link.
		if (iterator.next() != 2)
			return -13;
		if (iterator.next() != 3)
			return -13;
		if (iterator.next() != 4)
			return -13;
		if (iterator.hasNext())
			return -13;

		final keyValues = values.keyValueIterator();
		var indexed = 0;
		while (keyValues.hasNext()) {
			final pair = keyValues.next();
			indexed += pair.key * 10 + pair.value;
		}
		if (indexed != 38 || values.join(":") != "1:3:4" || values.toString() != "{1, 3, 4}")
			return -14;

		final evens = values.filter(isEven);
		final mapped = values.map(doubled);
		if (evens.join(":") != "4" || mapped.join(":") != "2:6:8")
			return -15;
		if (values.pop() != 1 || values.length != 2)
			return -16;
		values.clear();
		if (!values.isEmpty() || values.first() != null || values.last() != null || values.pop() != null)
			return -17;
		return indexed;
	}

	/** Prove shallow managed-element identity across both collection families. */
	static function managedContract():Int {
		final shared = new Node(5);
		final vector = new Vector<Node>(2, shared);
		final copied = vector.copy();
		copied[0].value = 6;
		if (vector[0] != shared || vector[1] != shared || copied[0] != shared)
			return -20;

		final list = new List<Node>();
		list.add(shared);
		list.push(new Node(4));
		final iterator = list.iterator();
		list.clear();
		final first = iterator.next();
		final second = iterator.next();
		if (first.value != 4 || second != shared || iterator.hasNext())
			return -21;
		return first.value + second.value;
	}

	/** Run every collection contract and reject the first observable mismatch. */
	static function main():Void {
		final vector = vectorContract();
		final list = listContract();
		final managed = managedContract();
		if (vector != 21 || list != 38 || managed != 10)
			throw 'vector/list contract failed: $vector,$list,$managed';
	}
}

/** One collector-managed element whose identity must survive shallow copies. */
final class Node {
	/** Mutable state observed through every collection alias. */
	public var value:Int;

	/** Create one identity-bearing managed value. */
	public function new(value:Int) {
		this.value = value;
	}
}

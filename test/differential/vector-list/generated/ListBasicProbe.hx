import haxe.ds.List;

/**
	Isolates non-generic linked List behavior from `List.map<X>` specialization.

	The probe checks whether ordinary generic classes, private nodes, shared
	identity, and retained-node cursors already lower before the generic virtual
	method seam is selected.
**/
final class ListBasicProbe {
	/** Exercise linked mutation and the cursor's retained current node. */
	static function main():Void {
		final values = new List<Int>();
		final alias = values;
		values.add(2);
		values.add(3);
		values.push(1);
		final iterator = values.iterator();
		if (iterator.next() != 1)
			throw "List first cursor value failed";
		values.add(4);
		if (!values.remove(2))
			throw "List removal failed";
		if (iterator.next() != 2)
			throw "List retained removed node failed";
		if (iterator.next() != 3)
			throw "List retained successor failed";
		if (iterator.next() != 4)
			throw "List appended successor failed";
		if (iterator.hasNext())
			throw "List retained-node cursor termination failed";
		if (alias.length != 3 || values.first() != 1 || values.last() != 4 || values.join(":") != "1:3:4")
			throw "List identity failed";
		values.clear();
		if (!alias.isEmpty())
			throw "List clear failed";
	}
}

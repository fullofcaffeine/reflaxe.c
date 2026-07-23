/** Generated-program probe for exact traced classes stored in ordinary Haxe Arrays. */
final class Main {
	static function main():Void {
		final first = new ManagedNode(10);
		final second = new ManagedNode(20);
		final nodes:Array<ManagedNode> = [];
		final absent = maybeNodes(false, first);
		final present = maybeNodes(true, first);
		nodes.push(first);
		nodes.push(second);
		nodes.push(null);
		// Cross both the Array growth boundary and the collector's deterministic
		// one-mebibyte pressure threshold. The final assertions then prove that a
		// collection traced the live Array slots and the first<->second cycle rather
		// than merely surviving until process teardown.
		for (index in 0...40000)
			nodes.push(new ManagedNode(index));

		final alias = nodes[0];
		alias.add(5);
		first.connect(second);
		second.connect(first);
		nodes[1] = first;

		while (nodes.length != 40003 || nodes[0].value != 15 || nodes[1] != first || nodes[2] != null || first.value != 15 || first.linkCount() != 1
			|| second.linkCount() != 1 || first.sampleCount() != 1 || absent != null || present == null || present.length != 1 || present[0] != first) {}
	}

	/**
		Prove that `Null<Array<Class>>` reuses the collector-backed Array pointer.

		The returned present branch remains reachable through the caller's exact GC
		root; the absent branch remains a literal null pointer and needs no wrapper.
	**/
	static function maybeNodes(present:Bool, node:ManagedNode):Null<Array<ManagedNode>> {
		if (!present)
			return null;
		return [node];
	}
}

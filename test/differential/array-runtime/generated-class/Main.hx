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
		final copied = nodes.copy();
		copied[0] = second;
		copied.push(first);
		final found = findNode(nodes, 20);
		final missing = findNode(nodes, 99);
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
		final originalFirstLinks = first.links;
		final replacementFirstLinks = [second];
		first.replaceLinks(replacementFirstLinks);
		first.connect(first);
		nodes[1] = first;

		while (nodes.length != 40003 || copied.length != 4 || copied[0] != second || copied[3] != first || nodes[0].value != 15 || nodes[1] != first
			|| nodes[2] != null || first.value != 15 || originalFirstLinks.length != 1 || originalFirstLinks[0] != second
			|| replacementFirstLinks.length != 2 || first.linkCount() != 2 || second.linkCount() != 1 || first.sampleCount() != 1 || absent != null
			|| present == null || present.length != 1 || present[0] != first || found != second || missing != null) {}
	}

	/** Return one matching class reference directly from an Array iteration. */
	static function findNode(nodes:Array<ManagedNode>, wanted:Int):Null<ManagedNode> {
		for (node in nodes)
			if (node != null && node.value == wanted)
				return node;
		return null;
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

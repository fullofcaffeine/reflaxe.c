import HistoryEntry.HistoryBounds;

/**
 * Class-owned Array shape reduced from Caxecraft's editor history.
 *
 * The field proves that the useful compiler slice cannot treat every Array as
 * function-local fixed storage. `alias` also proves Haxe reference semantics:
 * both variables must observe mutations to the same resizable container.
 */
final class History {
	final entries:Array<HistoryEntry> = [];

	public function new() {}

	public function add(revision:Int, before:haxe.io.Bytes, after:haxe.io.Bytes, bounds:Null<HistoryBounds>):Void
		entries.push({
			revision: revision,
			before: before,
			after: after,
			bounds: bounds
		});

	public function depth():Int
		return entries.length;

	public function lastRevision():Int {
		final alias = entries;
		return alias[alias.length - 1].revision;
	}

	/** Read through a copied managed record; the temporary copy must be cleaned. */
	public function lastAfterByte():Int
		return entries[entries.length - 1].after.get(0);

	/** Guard and read the direct optional nested inside the managed Array record. */
	public function lastMinimum():Int {
		final bounds = entries[entries.length - 1].bounds;
		return bounds == null ? -1 : bounds.minimum;
	}
}

package;

/**
 * A recursive value owns its child link and deep-copies it when retained.
 *
 * That copy can allocate and fail, so the current infallible managed-carrier
 * protocol must reject this conditional until acquisition carries a typed
 * failure edge.
 */
enum Chain {
	End(value:Int);
	Link(value:Int, next:Chain);
}

/** Keeps the unsupported recursive join reachable without requiring output. */
final class Main {
	/** The borrowed arm would require a fallible deep retain. */
	static function choose(condition:Bool, existing:Chain):Chain {
		final selected = condition ? End(1) : existing;
		return selected;
	}

	/** Reach the conditional through ordinary typed Haxe. */
	static function main():Void {
		final existing = Link(2, End(3));
		choose(false, existing);
	}
}

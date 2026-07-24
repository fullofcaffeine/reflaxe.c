/**
 * Keeps unrelated `IRTInstance` constructor parameters fail-closed.
 *
 * A fieldless Haxe enum also becomes an IR instance, but it is not a closed
 * record. This fixture ensures the record admission does not accidentally
 * authorize every value that happens to share that low-level IR constructor.
 */
enum Choice {
	First;
}

/** A deliberately unsupported constructor parameter family. */
final class EnumConfigured {
	public final selected:Bool;

	/** This constructor must remain rejected until enum parameters are proven. */
	public function new(choice:Choice) {
		selected = choice == First;
	}
}

/** Reaches the unsupported constructor so the compiler must diagnose it. */
final class Main {
	static function main():Void {
		final configured = new EnumConfigured(First);
		while (!configured.selected) {}
	}
}

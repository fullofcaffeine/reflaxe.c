/**
 * Keeps payload-enum `IRTInstance` constructor parameters fail-closed.
 *
 * A payload enum is a tagged C union whose copy may need nested ownership work.
 * Admitting a fieldless enum tag must not silently authorize this wider family
 * merely because both values use `IRTInstance` in HxcIR.
 */
enum Choice {
	First(values:Array<Int>);
}

/** A deliberately unsupported constructor parameter family. */
final class EnumConfigured {
	public final selected:Bool;

	/** This constructor must remain rejected until enum parameters are proven. */
	public function new(choice:Choice) {
		selected = switch choice {
			case First(values): values[0] == 1;
		};
	}
}

/** Reaches the unsupported constructor so the compiler must diagnose it. */
final class Main {
	static function main():Void {
		final configured = new EnumConfigured(First([1]));
		while (!configured.selected) {}
	}
}

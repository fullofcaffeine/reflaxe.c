/**
 * Keeps fresh payload-enum constructor arguments fail-closed until their
 * ownership transfer has a complete lifecycle.
 *
 * The compiler now understands this constructor parameter's tagged-union
 * representation, but `First([1])` also creates an owned Array. Passing that
 * fresh value safely requires the call to transfer or release its nested
 * owner. The function-exit validator rejects the program instead of leaking
 * the Array or treating a managed enum like a plain integer tag.
 */
enum Choice {
	First(values:Array<Int>);
}

/** A constructor whose managed enum argument still needs call-lifecycle work. */
final class EnumConfigured {
	public final selected:Bool;

	/** Reads the payload without claiming ownership of the caller's enum value. */
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

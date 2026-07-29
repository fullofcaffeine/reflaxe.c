enum GenericValue<T> {
	Present(value:T);
}

/**
 * Baseline for specialization-size attribution.
 *
 * The neighboring fixture keeps this generic source at the same lines and adds
 * an ordinary enum afterward. Its report must charge only this specialization.
 */
class Main {
	static function identity<T>(value:T):T {
		return value;
	}

	static function main():Void {
		final value:GenericValue<Int> = Present(7);
		identity(value);
	}
}

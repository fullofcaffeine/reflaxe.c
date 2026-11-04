/** Two source functions share one file identity during each normalization pass. */
class SourceIdentitySubject {
	/** The first independently captured function. */
	public static function first():Int {
		return 1;
	}

	/** The second function must retain its own source positions. */
	public static function second():Int {
		return 2;
	}
}

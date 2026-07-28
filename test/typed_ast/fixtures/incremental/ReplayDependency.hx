/**
	Provides one ordinary classpath-owned dependency for rebuild tests.

	The invalidation catalog first shadows this module from an earlier classpath,
	then renames it. Those edits prove that backend replay follows Haxe's current
	module graph instead of retaining a function prepared for a removed owner.
**/
class ReplayDependency {
	/** Return a recognizable baseline value without adding runtime machinery. */
	public static function value():Int
		return 0;
}

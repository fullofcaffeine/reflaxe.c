package caxecraft.domain;

/**
	Controller-neutral movement intent for one fixed simulation tick.

	A keyboard, non-player artificial-intelligence controller, cutscene, or test
	script can produce this same value. It describes desired movement, not which
	device or character role requested it.

	A `typedef` record fits because the intent is a one-tick message with no
	identity, mutable lifetime, or resource. A controller class may own input
	history later, but it still produces this small value for the shared session.
	Haxe.c passes the admitted record as a direct C value.
**/
typedef CharacterIntent = {
	final moveX:Float;
	final moveZ:Float;

	/** Rise while wet or request an ordinary jump while dry. */
	final jump:Bool;

	/** Descend while swimming; dry-land movement ignores this field. */
	final descend:Bool;
}

package caxecraft.domain;

/**
	Validated movement and survival capabilities supplied by resolved content.

	The engine never checks an item or campaign name. A built-in suit, a future
	mod, or a temporary effect may resolve to this same bounded shape. Values are
	created through `PlayerAquatics.profile`, which clamps malformed numeric input
	before it can affect fixed-tick physics.
**/
typedef AquaticProfile = {
	final maximumBreathTicks:Int;
	final breathRecoveryPerTick:Int;
	final horizontalControl:Float;
	final ascentAcceleration:Float;
	final descentAcceleration:Float;
	final buoyancyAcceleration:Float;
	final dragPerTick:Float;
	final drowningIntervalTicks:Int;
	final underwaterMining:Bool;
	final coldProtection:Bool;
}

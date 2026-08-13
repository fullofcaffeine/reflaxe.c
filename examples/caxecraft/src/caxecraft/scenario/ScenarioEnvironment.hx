package caxecraft.scenario;

/**
	Optional visual world beyond a finite CaxeMap.

	When this value is absent, the game draws only its plain fallback sky. A
	declared environment selects generic presentation features; it cannot add
	collision, fluid simulation, objects, rules, or campaign behavior.
**/
typedef ScenarioEnvironment = {
	/** First bounded renderer profile. Future profiles remain explicit choices. */
	final profile:ScenarioEnvironmentProfile;

	/** Clear-sky color selected by this level. */
	final sky:ScenarioRgb;

	/** Optional stable direction to the sun, relative to the player. */
	final sun:Null<ScenarioSun>;

	/** Deterministic moving block-cloud layer; zero count disables it. */
	final clouds:ScenarioCloudLayer;

	/** Boundaries where presentation may continue authored edge terrain. */
	final edges:Array<ScenarioHorizonEdge>;

	/** Whether authored water touching an enabled edge continues visually. */
	final continueWater:Bool;
}

/** Closed environment renderer selected by authored content. */
enum ScenarioEnvironmentProfile {
	VoxelHorizon;
}

/** Eight-bit color channels validated by the CaxeMap parser. */
typedef ScenarioRgb = {
	final red:Int;
	final green:Int;
	final blue:Int;
}

/** Player-relative direction and apparent size for one celestial sun. */
typedef ScenarioSun = {
	final x:Int;
	final y:Int;
	final z:Int;
	final radiusMilli:Int;
}

/** Bounded procedural cloud settings owned by one level. */
typedef ScenarioCloudLayer = {
	final count:Int;
	final speedMilli:Int;
	final seed:Int;
}

/** One finite-map edge that can receive non-colliding distant presentation. */
enum ScenarioHorizonEdge {
	North;
	South;
	East;
	West;
}

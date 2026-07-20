package caxecraft.scenario;

/**
	One bounded optional-feature record retained even when the game does not
	understand that feature. The payload is data, never executable code.
**/
typedef ScenarioExtension = {
	final feature:ContentId;
	final id:ScenarioId;
	final data:String;
}

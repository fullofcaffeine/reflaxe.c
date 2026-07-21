package caxecraft.scenario;

/**
	Complete typed scenario shape shared by editor, game, Eval, and C.

	A parsed or edited value may still contain unresolved references or another
	semantic error. Only a value returned by `ScenarioValidator` is playable or
	eligible for persistence.
**/
typedef Scenario = {
	final formatVersion:Int;
	final requiredFeatures:Array<ContentId>;
	final optionalFeatures:Array<ContentId>;
	final id:ScenarioId;
	final assetPack:LogicalPath;
	final messages:ScenarioMessages;
	final title:ScenarioText;
	final mode:ScenarioMode;
	final world:ScenarioWorld;
	final objects:Array<ScenarioObject>;
	final story:ScenarioStory;
	final flow:CaxeFlow;
	final extensions:Array<ScenarioExtension>;
}

enum ScenarioMode {
	Creative;
	Adventure;
}

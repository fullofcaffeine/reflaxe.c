package caxecraft.scenario;

/** Complete validated semantic model shared by editor, game, Eval, and C. */
typedef Scenario = {
	final formatVersion:Int;
	final requiredFeatures:Array<ContentId>;
	final optionalFeatures:Array<ContentId>;
	final id:ScenarioId;
	final assetPack:LogicalPath;
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

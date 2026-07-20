package caxecraft.scenario;

import caxecraft.scenario.ScenarioDiagnostic.ScenarioCoordinate;

/** Result of one bounded CAXEMAP stage. Rejected input never carries a value. */
enum ScenarioReadResult<T> {
	ReadOk(value:T);
	ReadError(diagnostics:Array<ScenarioDiagnostic>);
}

/** One token after byte decoding, comment removal, and escape processing. */
typedef ScenarioLexToken = {
	final text:String;
	final kind:ScenarioLexTokenKind;
	final coordinate:ScenarioCoordinate;
}

/** Whether a token came from ordinary grammar text or a quoted text value. */
enum ScenarioLexTokenKind {
	BareToken;
	QuotedText;
}

/** One non-comment logical record. `indent` counts leading ASCII spaces. */
typedef ScenarioLexRecord = {
	final indent:Int;
	final coordinate:ScenarioCoordinate;
	final tokens:Array<ScenarioLexToken>;
}

/** Typed identity of a source record retained for semantic diagnostics. */
enum ScenarioSourceSubject {
	Header;
	Feature(id:ContentId);
	MapIdentity(id:ScenarioId);
	AssetPack;
	Title;
	Mode;
	World;
	Palette(code:Int);
	Chunk(id:ScenarioId);
	Object(id:ScenarioId);
	ObjectTag(objectId:ScenarioId, tag:ScenarioTag);
	Dialogue(id:ScenarioId);
	Journal(id:ScenarioId);
	Objective(id:ScenarioId);
	Route(id:ScenarioId);
	Variable(id:ScenarioId);
	Sequence(id:ScenarioId);
	SequenceParameter(sequenceId:ScenarioId, parameterId:ScenarioId);
	SequenceAction(sequenceId:ScenarioId, actionIndex:Int);
	Rule(id:ScenarioId);
	RuleEvent(ruleId:ScenarioId);
	RulePredicate(ruleId:ScenarioId);
	RuleAction(ruleId:ScenarioId, actionIndex:Int);
	Extension(feature:ContentId, id:ScenarioId);
}

typedef ScenarioSourceLocation = {
	final subject:ScenarioSourceSubject;
	final coordinate:ScenarioCoordinate;
}

/**
	A syntactically complete candidate that has not passed semantic validation.

	Keeping this wrapper distinct from `Scenario` makes it impossible for the
	loader to accidentally install a merely parsed map as the live world.
**/
typedef ParsedScenario = {
	final candidate:Scenario;
	final recordCoordinates:Array<ScenarioCoordinate>;
	final sourceLocations:Array<ScenarioSourceLocation>;
}

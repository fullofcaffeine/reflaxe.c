package caxecraft.scenario;

/** Stable source coordinate in a decoded CAXEMAP document. */
typedef ScenarioCoordinate = {
	final line:Int;
	final column:Int;
	final record:Int;
}

/** Typed loading, validation, execution, and persistence failures. */
typedef ScenarioDiagnostic = {
	final coordinate:ScenarioCoordinate;
	final kind:ScenarioDiagnosticKind;
}

enum ScenarioDiagnosticKind {
	MalformedUtf8(byteOffset:Int);
	UnknownVersion(version:Int);
	UnknownRequiredFeature(feature:ContentId);
	InvalidToken;
	InvalidEscape;
	UnexpectedRecord(recordType:String);
	MissingRecord(recordType:String);
	IntegerOutOfRange;
	LimitExceeded(limit:String, maximum:Int);
	InvalidRunTotal(chunk:ScenarioId, expected:Int, actual:Int);
	DuplicateId(id:ScenarioId);
	DuplicateTag(tag:ScenarioTag);
	UnresolvedReference(id:ScenarioId);
	ImpossiblePlacement(id:ScenarioId);
	InvalidRule(id:ScenarioId);
	RuleCycle(id:ScenarioId);
	InvalidExtension(id:ScenarioId);
	EventBudgetExhausted(maximum:Int);
	PersistenceFailed(stage:PersistenceStage);
}

enum PersistenceStage {
	CreateTemporary;
	WriteTemporary;
	FlushTemporary;
	ReplaceDestination;
	CleanupTemporary;
}

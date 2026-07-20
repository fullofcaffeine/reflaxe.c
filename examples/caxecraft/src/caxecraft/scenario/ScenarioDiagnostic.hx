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
	MissingRecord(expected:ScenarioExpectedRecord);
	IntegerOutOfRange;
	LimitExceeded(limit:ScenarioLimitKind, maximum:Int);
	InvalidRunTotal(chunk:ScenarioId, expected:Int, actual:Int);
	DuplicateContentId(id:ContentId);
	DuplicatePaletteCode(code:Int);
	DuplicateId(id:ScenarioId);
	DuplicateTag(objectId:ScenarioId, tag:ScenarioTag);
	UnresolvedReference(id:ScenarioId);
	UnresolvedContent(id:ContentId);
	ImpossiblePlacement(id:ScenarioId);
	InvalidRule(id:ScenarioId);
	RuleCycle(id:ScenarioId);
	InvalidExtension(id:ScenarioId);
	EventBudgetExhausted(maximum:Int);
	PersistenceFailed(stage:PersistenceStage);
}

/** Closed list of required records so diagnostics never depend on prose tags. */
enum ScenarioExpectedRecord {
	FormatHeader;
	EndMapRecord;
	MapRecord;
	AssetPackRecord;
	TitleRecord;
	ModeRecord;
	WorldRecord;
	EndChunkRecord;
	EndObjectRecord;
	ObjectPlacementRecord;
	EndDialogueRecord;
	JournalBodyRecord;
	EndJournalRecord;
	ObjectiveBodyRecord;
	EndObjectiveRecord;
	EndRouteRecord;
	EndSequenceRecord;
	EndRuleRecord;
	ChoiceRecord;
	EndChoiceRecord;
	ExtensionDataRecord;
	EndExtensionRecord;
	CoreFeatureRecord;
	AirPaletteRecord;
	CompleteChunkCoverage;
	SinglePlayerSpawn;
}

/** Closed resource or work bound carried by a `LimitExceeded` diagnostic. */
enum ScenarioLimitKind {
	FileBytes;
	LogicalRecords;
	TextScalars;
	WorldWidth;
	WorldHeight;
	WorldDepth;
	WorldCells;
	PaletteEntries;
	Objects;
	ObjectTags;
	Dialogues;
	DialogueLines;
	Objectives;
	Routes;
	Sequences;
	Variables;
	Rules;
	RuleActions;
	SequenceCallDepth;
}

enum PersistenceStage {
	CreateTemporary;
	WriteTemporary;
	FlushTemporary;
	ReplaceDestination;
	CleanupTemporary;
}

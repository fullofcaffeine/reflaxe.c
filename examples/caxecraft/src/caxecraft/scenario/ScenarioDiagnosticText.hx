package caxecraft.scenario;

import caxecraft.scenario.ScenarioDiagnostic.PersistenceStage;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnostic;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;

/**
	Maps CAXEMAP failures to locale-data keys and typed replacement arguments.

	Coordinates and semantic values remain typed until this boundary. The result
	contains no prose; the validated UI catalog is the only translation owner.
**/
typedef ScenarioDiagnosticMessage = {
	final message:MessageId;
	final arguments:Array<String>;
}

/** Convert one located diagnostic to a catalog key and ordered arguments. */
function scenarioDiagnosticMessage(diagnostic:ScenarioDiagnostic):ScenarioDiagnosticMessage {
	final position = ['${diagnostic.coordinate.line}', '${diagnostic.coordinate.column}'];
	return switch diagnostic.kind {
		case MalformedUtf8(byteOffset): located("malformed-utf8", ['$byteOffset'], position);
		case UnknownVersion(version): located("unknown-version", ['$version'], position);
		case UnknownRequiredFeature(feature): located("unknown-feature", [feature.text()], position);
		case InvalidToken: located("invalid-token", [], position);
		case InvalidEscape: located("invalid-escape", [], position);
		case UnexpectedRecord(recordType): located("unexpected-record", [recordType], position);
		case MissingRecord(expected): located("missing-record", [expectedRecordId(expected)], position);
		case IntegerOutOfRange: located("integer-range", [], position);
		case LimitExceeded(limit, maximum): located("limit", [limitId(limit), '$maximum'], position);
		case InvalidRunTotal(chunk, expected, actual): located("run-total", [chunk.text(), '$expected', '$actual'], position);
		case DuplicateContentId(id): located("duplicate-content", [id.text()], position);
		case DuplicatePaletteCode(code): located("duplicate-palette", ['$code'], position);
		case DuplicateId(id): located("duplicate-id", [id.text()], position);
		case DuplicateTag(objectId, tag): located("duplicate-tag", [objectId.text(), tag.text()], position);
		case DuplicateLocale(id): located("duplicate-locale", [id.text()], position);
		case DuplicateMessage(locale, messageId): located("duplicate-message", [messageId.text(), locale.text()], position);
		case UnknownDefaultLocale(id): located("default-locale", [id.text()], position);
		case UnresolvedMessage(id): located("missing-message", [id.text()], position);
		case MissingTranslation(locale, messageId): located("missing-translation", [messageId.text(), locale.text()], position);
		case UnknownTranslation(locale, messageId): located("unknown-translation", [messageId.text(), locale.text()], position);
		case UnresolvedReference(id): located("stale-reference", [id.text()], position);
		case UnresolvedContent(id): located("missing-content", [id.text()], position);
		case ImpossiblePlacement(id): located("placement", [id.text()], position);
		case InvalidRule(id): located("invalid-rule", [id.text()], position);
		case InvalidRuleReference(id, field, reference, expected):
			located("invalid-rule-reference", [id.text(), field, reference.text(), expected], position);
		case RuleCycle(id): located("rule-cycle", [id.text()], position);
		case InvalidExtension(id): located("invalid-extension", [id.text()], position);
		case EventBudgetExhausted(maximum): located("event-budget", ['$maximum'], position);
		case PersistenceFailed(stage): located("persistence", [persistenceStageId(stage)], position);
	};
}

/** Return every catalog key this CAXEMAP diagnostic boundary can emit. */
function requiredScenarioDiagnosticMessageIds():Array<MessageId>
	return [
		new MessageId("scenario.diagnostic.default-locale"),
		new MessageId("scenario.diagnostic.duplicate-content"),
		new MessageId("scenario.diagnostic.duplicate-id"),
		new MessageId("scenario.diagnostic.duplicate-locale"),
		new MessageId("scenario.diagnostic.duplicate-message"),
		new MessageId("scenario.diagnostic.duplicate-palette"),
		new MessageId("scenario.diagnostic.duplicate-tag"),
		new MessageId("scenario.diagnostic.event-budget"),
		new MessageId("scenario.diagnostic.integer-range"),
		new MessageId("scenario.diagnostic.invalid-escape"),
		new MessageId("scenario.diagnostic.invalid-extension"),
		new MessageId("scenario.diagnostic.invalid-rule"),
		new MessageId("scenario.diagnostic.invalid-rule-reference"),
		new MessageId("scenario.diagnostic.invalid-token"),
		new MessageId("scenario.diagnostic.limit"),
		new MessageId("scenario.diagnostic.malformed-utf8"),
		new MessageId("scenario.diagnostic.missing-content"),
		new MessageId("scenario.diagnostic.missing-message"),
		new MessageId("scenario.diagnostic.missing-record"),
		new MessageId("scenario.diagnostic.missing-translation"),
		new MessageId("scenario.diagnostic.persistence"),
		new MessageId("scenario.diagnostic.placement"),
		new MessageId("scenario.diagnostic.rule-cycle"),
		new MessageId("scenario.diagnostic.run-total"),
		new MessageId("scenario.diagnostic.stale-reference"),
		new MessageId("scenario.diagnostic.unexpected-record"),
		new MessageId("scenario.diagnostic.unknown-feature"),
		new MessageId("scenario.diagnostic.unknown-translation"),
		new MessageId("scenario.diagnostic.unknown-version")
	];

private function located(id:String, values:Array<String>, position:Array<String>):ScenarioDiagnosticMessage {
	final arguments = values.copy();
	for (value in position)
		arguments.push(value);
	return {message: new MessageId('scenario.diagnostic.$id'), arguments: arguments};
}

/** Stable CAXEMAP term interpolated inside localized missing-record prose. */
private function expectedRecordId(value:ScenarioExpectedRecord):String
	return switch value {
		case FormatHeader: "format-header";
		case EndMapRecord: "end-map";
		case MapRecord: "map";
		case AssetPackRecord: "asset-pack";
		case DefaultLocaleRecord: "default-locale";
		case TitleRecord: "title";
		case ModeRecord: "mode";
		case EndEnvironmentRecord: "end-environment";
		case WorldRecord: "world";
		case EndChunkRecord: "end-chunk";
		case EndObjectRecord: "end-object";
		case ObjectPlacementRecord: "object-placement";
		case EndDialogueRecord: "end-dialogue";
		case EndLocaleRecord: "end-locale";
		case JournalBodyRecord: "journal-body";
		case EndJournalRecord: "end-journal";
		case ObjectiveBodyRecord: "objective-body";
		case EndObjectiveRecord: "end-objective";
		case EndRouteRecord: "end-route";
		case EndSequenceRecord: "end-sequence";
		case EndRuleRecord: "end-rule";
		case ChoiceRecord: "choice";
		case EndChoiceRecord: "end-choice";
		case ExtensionDataRecord: "extension-data";
		case EndExtensionRecord: "end-extension";
		case CoreFeatureRecord: "core-feature";
		case AirPaletteRecord: "air-palette";
		case CompleteChunkCoverage: "complete-chunk-coverage";
		case SinglePlayerSpawn: "single-player-spawn";
	};

/** Stable resource term interpolated inside localized limit prose. */
private function limitId(value:ScenarioLimitKind):String
	return switch value {
		case FileBytes: "file-bytes";
		case LogicalRecords: "logical-records";
		case TextScalars: "text-scalars";
		case WorldWidth: "world-width";
		case WorldHeight: "world-height";
		case WorldDepth: "world-depth";
		case WorldCells: "world-cells";
		case PaletteEntries: "palette-entries";
		case Fluids: "fluids";
		case Objects: "objects";
		case ObjectTags: "object-tags";
		case Dialogues: "dialogues";
		case DialogueLines: "dialogue-lines";
		case Locales: "locales";
		case MessagesPerLocale: "messages-per-locale";
		case Objectives: "objectives";
		case Routes: "routes";
		case Sequences: "sequences";
		case Variables: "variables";
		case Rules: "rules";
		case RuleActions: "rule-actions";
		case SequenceCallDepth: "sequence-call-depth";
	};

/** Stable persistence phase interpolated inside localized save-failure prose. */
private function persistenceStageId(value:PersistenceStage):String
	return switch value {
		case CreateTemporary: "create-temporary";
		case WriteTemporary: "write-temporary";
		case FlushTemporary: "flush-temporary";
		case ReplaceDestination: "replace-destination";
		case CleanupTemporary: "cleanup-temporary";
	};

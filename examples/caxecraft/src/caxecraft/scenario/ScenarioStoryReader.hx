package caxecraft.scenario;

import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioDialogueLine;
import caxecraft.scenario.ScenarioStory.ScenarioJournalEntry;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioStory.ScenarioRoute;

/** Reads player-facing dialogue, journal, objective, and route records. */
// Used only by the CAXEMAP parser. Game and editor code should call
// ScenarioParser.parse(...) instead of constructing this class.
@:noCompletion
final class ScenarioStoryReader {
	final cursor:ScenarioRecordCursor;

	public function new(cursor:ScenarioRecordCursor)
		this.cursor = cursor;

	public function readDialogue():ScenarioReadResult<ScenarioDialogue> {
		final header = cursor.current();
		if (!ScenarioTokenGrammar.hasTokenCount(header, 2))
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		if (id == null)
			return cursor.failToken(header.tokens[1], InvalidToken);
		cursor.locate(Dialogue(id), header);
		cursor.advance();

		final lines:Array<ScenarioDialogueLine> = [];
		while (cursor.hasRecord() && !ScenarioTokenGrammar.isEnd(cursor.current(), "dialogue")) {
			final record = cursor.current();
			if (record.indent != 2 || record.tokens.length < 4 || ScenarioTokenGrammar.firstText(record) != "line")
				return cursor.failAt(record, UnexpectedRecord(ScenarioTokenGrammar.firstText(record)));
			final narrator = ScenarioTokenGrammar.isBare(record.tokens[1], "narrator");
			// The earlier spelling has four tokens (`line <id> <text>`), while
			// the explicit spelling has five (`line speaker <id> <text>`). The
			// count keeps an older object whose ID is literally `speaker`
			// readable without making the two forms ambiguous.
			final explicitSpeaker = ScenarioTokenGrammar.isBare(record.tokens[1], "speaker") && record.tokens.length == 5;
			final speaker = narrator ? null : ScenarioTokenGrammar.scenarioId(record.tokens[explicitSpeaker ? 2 : 1]);
			if (!narrator && speaker == null)
				return cursor.failToken(record.tokens[explicitSpeaker ? 2 : 1], InvalidToken);
			final parsed = ScenarioTokenGrammar.text(record, explicitSpeaker ? 3 : 2);
			if (parsed == null || parsed.next != record.tokens.length)
				return cursor.failAt(record, InvalidToken);
			lines.push({speaker: speaker, text: parsed.value});
			cursor.advance();
		}
		if (!cursor.hasRecord())
			return cursor.failAt(header, MissingRecord(EndDialogueRecord));
		cursor.advance();
		return ReadOk({id: id, lines: lines});
	}

	public function readJournal():ScenarioReadResult<ScenarioJournalEntry> {
		final header = cursor.current();
		if (header.tokens.length < 5 || !ScenarioTokenGrammar.isBare(header.tokens[2], "title"))
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		final title = ScenarioTokenGrammar.text(header, 3);
		if (id == null || title == null || title.next != header.tokens.length)
			return cursor.failAt(header, InvalidToken);
		cursor.locate(Journal(id), header);
		cursor.advance();

		if (!cursor.hasRecord() || cursor.current().indent != 2 || ScenarioTokenGrammar.firstText(cursor.current()) != "body")
			return cursor.failAt(header, MissingRecord(JournalBodyRecord));
		final bodyRecord = cursor.current();
		final body = ScenarioTokenGrammar.text(bodyRecord, 1);
		if (body == null || body.next != bodyRecord.tokens.length)
			return cursor.failAt(bodyRecord, InvalidToken);
		cursor.advance();
		if (!cursor.hasRecord() || !ScenarioTokenGrammar.isEnd(cursor.current(), "journal"))
			return cursor.failAt(header, MissingRecord(EndJournalRecord));
		cursor.advance();
		return ReadOk({id: id, title: title.value, body: body.value});
	}

	public function readObjective():ScenarioReadResult<ScenarioObjective> {
		final header = cursor.current();
		if (header.tokens.length < 5)
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		final state = ScenarioTokenGrammar.objectiveState(header.tokens[2]);
		if (id == null || state == null || !ScenarioTokenGrammar.isBare(header.tokens[3], "title"))
			return cursor.failAt(header, InvalidToken);
		cursor.locate(Objective(id), header);
		final title = ScenarioTokenGrammar.text(header, 4);
		if (title == null || title.next != header.tokens.length)
			return cursor.failAt(header, InvalidToken);
		cursor.advance();

		if (!cursor.hasRecord() || cursor.current().indent != 2 || ScenarioTokenGrammar.firstText(cursor.current()) != "body")
			return cursor.failAt(header, MissingRecord(ObjectiveBodyRecord));
		final bodyRecord = cursor.current();
		final body = ScenarioTokenGrammar.text(bodyRecord, 1);
		if (body == null || body.next != bodyRecord.tokens.length)
			return cursor.failAt(bodyRecord, InvalidToken);
		cursor.advance();
		if (!cursor.hasRecord() || !ScenarioTokenGrammar.isEnd(cursor.current(), "objective"))
			return cursor.failAt(header, MissingRecord(EndObjectiveRecord));
		cursor.advance();
		return ReadOk({
			id: id,
			title: title.value,
			body: body.value,
			initialState: state
		});
	}

	public function readRoute():ScenarioReadResult<ScenarioRoute> {
		final header = cursor.current();
		if (header.tokens.length < 5 || !ScenarioTokenGrammar.isBare(header.tokens[2], "title"))
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		final title = ScenarioTokenGrammar.text(header, 3);
		if (id == null || title == null || title.next != header.tokens.length)
			return cursor.failAt(header, InvalidToken);
		cursor.locate(Route(id), header);
		cursor.advance();

		final objectives:Array<ScenarioId> = [];
		while (cursor.hasRecord() && !ScenarioTokenGrammar.isEnd(cursor.current(), "route")) {
			final record = cursor.current();
			if (record.indent != 2
				|| !ScenarioTokenGrammar.hasTokenCount(record, 2)
				|| ScenarioTokenGrammar.firstText(record) != "objective")
				return cursor.failAt(record, UnexpectedRecord(ScenarioTokenGrammar.firstText(record)));
			final objective = ScenarioTokenGrammar.scenarioId(record.tokens[1]);
			if (objective == null)
				return cursor.failToken(record.tokens[1], InvalidToken);
			objectives.push(objective);
			cursor.advance();
		}
		if (!cursor.hasRecord())
			return cursor.failAt(header, MissingRecord(EndRouteRecord));
		cursor.advance();
		return ReadOk({id: id, title: title.value, objectives: objectives});
	}
}

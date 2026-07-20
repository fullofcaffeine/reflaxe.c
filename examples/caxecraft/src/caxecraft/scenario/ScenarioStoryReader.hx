package caxecraft.scenario;

import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioDialogueLine;
import caxecraft.scenario.ScenarioStory.ScenarioJournalEntry;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioStory.ScenarioRoute;

/** Reads player-facing dialogue, journal, objective, and route records. */
// Internal helper. Application code should call ScenarioParser.parse(...) instead.
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
			final speaker = record.tokens[1].text == "narrator" ? null : ScenarioTokenGrammar.scenarioId(record.tokens[1]);
			if (record.tokens[1].text != "narrator" && speaker == null)
				return cursor.failToken(record.tokens[1], InvalidToken);
			final parsed = ScenarioTokenGrammar.text(record, 2);
			if (parsed == null || parsed.next != record.tokens.length)
				return cursor.failAt(record, InvalidToken);
			lines.push({speaker: speaker, text: parsed.value});
			cursor.advance();
		}
		if (!cursor.hasRecord())
			return cursor.failAt(header, MissingRecord("end dialogue"));
		cursor.advance();
		return ReadOk({id: id, lines: lines});
	}

	public function readJournal():ScenarioReadResult<ScenarioJournalEntry> {
		final header = cursor.current();
		if (header.tokens.length < 5 || header.tokens[2].text != "title")
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		final title = ScenarioTokenGrammar.text(header, 3);
		if (id == null || title == null || title.next != header.tokens.length)
			return cursor.failAt(header, InvalidToken);
		cursor.locate(Journal(id), header);
		cursor.advance();

		if (!cursor.hasRecord() || cursor.current().indent != 2 || ScenarioTokenGrammar.firstText(cursor.current()) != "body")
			return cursor.failAt(header, MissingRecord("journal body"));
		final bodyRecord = cursor.current();
		final body = ScenarioTokenGrammar.text(bodyRecord, 1);
		if (body == null || body.next != bodyRecord.tokens.length)
			return cursor.failAt(bodyRecord, InvalidToken);
		cursor.advance();
		if (!cursor.hasRecord() || !ScenarioTokenGrammar.isEnd(cursor.current(), "journal"))
			return cursor.failAt(header, MissingRecord("end journal"));
		cursor.advance();
		return ReadOk({id: id, title: title.value, body: body.value});
	}

	public function readObjective():ScenarioReadResult<ScenarioObjective> {
		final header = cursor.current();
		if (header.tokens.length < 5)
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		final state = ScenarioTokenGrammar.objectiveState(header.tokens[2]);
		if (id == null || state == null || header.tokens[3].text != "title")
			return cursor.failAt(header, InvalidToken);
		cursor.locate(Objective(id), header);
		final title = ScenarioTokenGrammar.text(header, 4);
		if (title == null || title.next != header.tokens.length)
			return cursor.failAt(header, InvalidToken);
		cursor.advance();

		if (!cursor.hasRecord() || cursor.current().indent != 2 || ScenarioTokenGrammar.firstText(cursor.current()) != "body")
			return cursor.failAt(header, MissingRecord("objective body"));
		final bodyRecord = cursor.current();
		final body = ScenarioTokenGrammar.text(bodyRecord, 1);
		if (body == null || body.next != bodyRecord.tokens.length)
			return cursor.failAt(bodyRecord, InvalidToken);
		cursor.advance();
		if (!cursor.hasRecord() || !ScenarioTokenGrammar.isEnd(cursor.current(), "objective"))
			return cursor.failAt(header, MissingRecord("end objective"));
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
		if (header.tokens.length < 5 || header.tokens[2].text != "title")
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
			return cursor.failAt(header, MissingRecord("end route"));
		cursor.advance();
		return ReadOk({id: id, title: title.value, objectives: objectives});
	}
}

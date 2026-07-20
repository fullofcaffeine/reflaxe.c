package caxecraft.scenario;

import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioWorld.VoxelChunk;
import caxecraft.scenario.ScenarioWorld.VoxelRun;

/** Reads world chunks and placed objects from the shared record cursor. */
// Internal helper. Application code should call ScenarioParser.parse(...) instead.
@:noCompletion
final class ScenarioWorldReader {
	final cursor:ScenarioRecordCursor;

	public function new(cursor:ScenarioRecordCursor)
		this.cursor = cursor;

	public function readChunk():ScenarioReadResult<VoxelChunk> {
		final header = cursor.current();
		if (!ScenarioTokenGrammar.hasTokenCount(header, 8))
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		final origin = ScenarioTokenGrammar.point(header, 2);
		final chunkSize = ScenarioTokenGrammar.size(header, 5);
		if (id == null || origin == null || chunkSize == null)
			return cursor.failAt(header, InvalidToken);
		cursor.locate(Chunk(id), header);
		cursor.advance();

		final runs:Array<VoxelRun> = [];
		while (cursor.hasRecord() && !ScenarioTokenGrammar.isEnd(cursor.current(), "chunk")) {
			final record = cursor.current();
			if (record.indent != 2 || !ScenarioTokenGrammar.hasTokenCount(record, 3) || ScenarioTokenGrammar.firstText(record) != "run")
				return cursor.failAt(record, UnexpectedRecord(ScenarioTokenGrammar.firstText(record)));
			final code = ScenarioTokenGrammar.integer(record.tokens[1]);
			final runCount = ScenarioTokenGrammar.integer(record.tokens[2]);
			if (code == null || runCount == null)
				return cursor.failAt(record, IntegerOutOfRange);
			runs.push({paletteCode: code, count: runCount});
			cursor.advance();
		}
		if (!cursor.hasRecord())
			return cursor.failAt(header, MissingRecord("end chunk"));
		cursor.advance();
		return ReadOk({
			id: id,
			origin: origin,
			size: chunkSize,
			runs: runs
		});
	}

	public function readObject():ScenarioReadResult<ScenarioObject> {
		final header = cursor.current();
		if (!ScenarioTokenGrammar.hasTokenCount(header, 2))
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		if (id == null)
			return cursor.failToken(header.tokens[1], InvalidToken);
		cursor.locate(Object(id), header);
		cursor.advance();

		final tags:Array<ScenarioTag> = [];
		var placement:Null<ObjectPlacement> = null;
		while (cursor.hasRecord() && !ScenarioTokenGrammar.isEnd(cursor.current(), "object")) {
			final record = cursor.current();
			if (record.indent != 2)
				return cursor.failAt(record, UnexpectedRecord(ScenarioTokenGrammar.firstText(record)));
			switch ScenarioTokenGrammar.firstText(record) {
				case "tag":
					if (!ScenarioTokenGrammar.hasTokenCount(record, 2))
						return cursor.failAt(record, InvalidToken);
					final tag = ScenarioTokenGrammar.scenarioTag(record.tokens[1]);
					if (tag == null)
						return cursor.failToken(record.tokens[1], InvalidToken);
					tags.push(tag);
				case "placement":
					if (placement != null)
						return cursor.failAt(record, InvalidToken);
					placement = placementValue(record);
					if (placement == null)
						return cursor.failAt(record, InvalidToken);
				case other:
					return cursor.failAt(record, UnexpectedRecord(other));
			}
			cursor.advance();
		}
		if (!cursor.hasRecord())
			return cursor.failAt(header, MissingRecord("end object"));
		if (placement == null)
			return cursor.failAt(header, MissingRecord("placement"));
		cursor.advance();
		return ReadOk({id: id, tags: tags, placement: placement});
	}

	function placementValue(record:ScenarioLexRecord):Null<ObjectPlacement> {
		if (record.tokens.length < 2)
			return null;
		return switch record.tokens[1].text {
			case "player-spawn" if (ScenarioTokenGrammar.hasTokenCount(record, 6)): transformPlacement(record, PlayerSpawn);
			case "checkpoint" if (ScenarioTokenGrammar.hasTokenCount(record, 6)): transformPlacement(record, Checkpoint);
			case "item" if (ScenarioTokenGrammar.hasTokenCount(record, 8)): itemPlacement(record);
			case "entity" if (ScenarioTokenGrammar.hasTokenCount(record, 7)): contentPlacement(record, Entity);
			case "npc" if (ScenarioTokenGrammar.hasTokenCount(record, 8)): npcPlacement(record);
			case "prefab" if (ScenarioTokenGrammar.hasTokenCount(record, 7)): contentPlacement(record, Prefab);
			case "trigger-zone" if (ScenarioTokenGrammar.hasTokenCount(record, 8)): triggerZone(record);
			case "stateful" if (ScenarioTokenGrammar.hasTokenCount(record, 8)): statefulPlacement(record);
			case _:
				null;
		}
	}

	function transformPlacement(record:ScenarioLexRecord, make:caxecraft.scenario.ScenarioGeometry.ScenarioTransform->ObjectPlacement):Null<ObjectPlacement> {
		final position = ScenarioTokenGrammar.transform(record, 2);
		return position == null ? null : make(position);
	}

	function itemPlacement(record:ScenarioLexRecord):Null<ObjectPlacement> {
		final content = ScenarioTokenGrammar.contentId(record.tokens[2]);
		final quantity = ScenarioTokenGrammar.integer(record.tokens[3]);
		final position = ScenarioTokenGrammar.transform(record, 4);
		return content == null || quantity == null || position == null ? null : Item(content, quantity, position);
	}

	function contentPlacement(record:ScenarioLexRecord,
			make:(ContentId, caxecraft.scenario.ScenarioGeometry.ScenarioTransform) -> ObjectPlacement):Null<ObjectPlacement> {
		final content = ScenarioTokenGrammar.contentId(record.tokens[2]);
		final position = ScenarioTokenGrammar.transform(record, 3);
		return content == null || position == null ? null : make(content, position);
	}

	function npcPlacement(record:ScenarioLexRecord):Null<ObjectPlacement> {
		final content = ScenarioTokenGrammar.contentId(record.tokens[2]);
		final dialogue = ScenarioTokenGrammar.scenarioId(record.tokens[3]);
		final position = ScenarioTokenGrammar.transform(record, 4);
		return content == null || dialogue == null || position == null ? null : Npc(content, dialogue, position);
	}

	function triggerZone(record:ScenarioLexRecord):Null<ObjectPlacement> {
		final origin = ScenarioTokenGrammar.point(record, 2);
		final zoneSize = ScenarioTokenGrammar.size(record, 5);
		return origin == null || zoneSize == null ? null : TriggerZone({origin: origin, size: zoneSize});
	}

	function statefulPlacement(record:ScenarioLexRecord):Null<ObjectPlacement> {
		final content = ScenarioTokenGrammar.contentId(record.tokens[2]);
		final state = ScenarioTokenGrammar.contentId(record.tokens[3]);
		final position = ScenarioTokenGrammar.transform(record, 4);
		return content == null || state == null || position == null ? null : StatefulObject(content, state, position);
	}
}

package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowChoice;
import caxecraft.scenario.CaxeFlow.FlowComparison;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowParameter;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioCodecModel.ParsedScenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexToken;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceLocation;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioCoordinate;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioStory.ObjectiveState;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioDialogueLine;
import caxecraft.scenario.ScenarioStory.ScenarioJournalEntry;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioStory.ScenarioRoute;
import caxecraft.scenario.ScenarioWorld.BlockPaletteEntry;
import caxecraft.scenario.ScenarioWorld.VoxelChunk;
import caxecraft.scenario.ScenarioWorld.VoxelRun;

/** Syntactic CAXEMAP parser. Reference resolution belongs to the validator. */
final class ScenarioParser {
	public static function parse(records:Array<ScenarioLexRecord>):ScenarioReadResult<ParsedScenario> {
		return new ScenarioParserState(records).parse();
	}
}

private typedef ParsedText = {
	final value:ScenarioText;
	final next:Int;
}

private typedef ParsedPredicate = {
	final value:FlowPredicate;
	final next:Int;
}

private typedef ParsedArgument = {
	final value:FlowArgument;
	final next:Int;
}

private final class ScenarioParserState {
	final records:Array<ScenarioLexRecord>;
	final locations:Array<ScenarioSourceLocation> = [];
	var index:Int = 0;

	public function new(records:Array<ScenarioLexRecord>)
		this.records = records;

	public function parse():ScenarioReadResult<ParsedScenario> {
		if (records.length == 0)
			return failAt(null, MissingRecord("CAXEMAP 1"));
		final header = records[0];
		if (header.indent != 0 || header.tokens.length != 2 || header.tokens[0].text != "CAXEMAP")
			return failAt(header, UnexpectedRecord(firstText(header)));
		final version = integer(header.tokens[1]);
		if (version == null)
			return failToken(header.tokens[1], IntegerOutOfRange);
		if (version != 1)
			return failToken(header.tokens[1], UnknownVersion(version));
		index = 1;
		locate(Header, header);

		final requiredFeatures:Array<ContentId> = [];
		final optionalFeatures:Array<ContentId> = [];
		var mapId:Null<ScenarioId> = null;
		var assetPack:Null<LogicalPath> = null;
		var title:Null<ScenarioText> = null;
		var mode:Null<ScenarioMode> = null;
		var worldSize:Null<VoxelSize> = null;
		final palette:Array<BlockPaletteEntry> = [];
		final chunks:Array<VoxelChunk> = [];
		final objects:Array<ScenarioObject> = [];
		final dialogues:Array<ScenarioDialogue> = [];
		final journal:Array<ScenarioJournalEntry> = [];
		final objectives:Array<ScenarioObjective> = [];
		final routes:Array<ScenarioRoute> = [];
		final variables:Array<FlowVariable> = [];
		final sequences:Array<FlowSequence> = [];
		final rules:Array<FlowRule> = [];
		final extensions:Array<ScenarioExtension> = [];
		var sawEnd = false;

		while (index < records.length) {
			final record = records[index];
			if (record.indent != 0)
				return failAt(record, UnexpectedRecord(firstText(record)));
			switch firstText(record) {
				case "feature":
					if (!count(record, 3))
						return failAt(record, InvalidToken);
					final feature = contentId(record.tokens[2]);
					if (feature == null)
						return failToken(record.tokens[2], InvalidToken);
					switch record.tokens[1].text {
						case "required": requiredFeatures.push(feature);
						case "optional": optionalFeatures.push(feature);
						case _: return failToken(record.tokens[1], InvalidToken);
					}
					locate(Feature(feature), record);
					index++;
				case "map":
					if (!count(record, 2) || mapId != null)
						return failAt(record, InvalidToken);
					mapId = scenarioId(record.tokens[1]);
					if (mapId == null)
						return failToken(record.tokens[1], InvalidToken);
					locate(MapIdentity(mapId), record);
					index++;
				case "asset-pack":
					if (!count(record, 2) || assetPack != null)
						return failAt(record, InvalidToken);
					assetPack = logicalPath(record.tokens[1]);
					if (assetPack == null)
						return failToken(record.tokens[1], InvalidToken);
					locate(AssetPack, record);
					index++;
				case "title":
					if (title != null)
						return failAt(record, InvalidToken);
					final parsed = text(record, 1);
					if (parsed == null || parsed.next != record.tokens.length)
						return failAt(record, InvalidToken);
					title = parsed.value;
					locate(Title, record);
					index++;
				case "mode":
					if (!count(record, 2) || mode != null)
						return failAt(record, InvalidToken);
					mode = switch record.tokens[1].text {
						case "creative": Creative;
						case "adventure": Adventure;
						case _: null;
					}
					if (mode == null)
						return failToken(record.tokens[1], InvalidToken);
					locate(Mode, record);
					index++;
				case "world":
					if (!count(record, 4) || worldSize != null)
						return failAt(record, InvalidToken);
					worldSize = size(record, 1);
					if (worldSize == null)
						return failAt(record, IntegerOutOfRange);
					locate(World, record);
					index++;
				case "palette":
					if (!count(record, 3))
						return failAt(record, InvalidToken);
					final code = integer(record.tokens[1]);
					final block = contentId(record.tokens[2]);
					if (code == null || block == null)
						return failAt(record, InvalidToken);
					palette.push({code: code, blockType: block});
					locate(Palette(code), record);
					index++;
				case "chunk":
					final parsed = parseChunk();
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(chunk): chunks.push(chunk);
					}
				case "object":
					final parsed = parseObject();
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(object): objects.push(object);
					}
				case "dialogue":
					final parsed = parseDialogue();
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(dialogue): dialogues.push(dialogue);
					}
				case "journal":
					final parsed = parseJournal();
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(entry): journal.push(entry);
					}
				case "objective":
					final parsed = parseObjective();
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(objective): objectives.push(objective);
					}
				case "route":
					final parsed = parseRoute();
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(route): routes.push(route);
					}
				case "variable":
					final parsed = parseVariable(record);
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(variable): variables.push(variable);
					}
					locate(Variable(variables[variables.length - 1].id), record);
					index++;
				case "sequence":
					final parsed = parseSequence();
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(sequence): sequences.push(sequence);
					}
				case "rule":
					final parsed = parseRule();
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(rule): rules.push(rule);
					}
				case "extension":
					final parsed = parseExtension();
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(extension): extensions.push(extension);
					}
				case "end-map":
					if (!count(record, 1))
						return failAt(record, InvalidToken);
					sawEnd = true;
					index++;
					if (index != records.length)
						return failAt(records[index], UnexpectedRecord(firstText(records[index])));
				case other:
					return failAt(record, UnexpectedRecord(other));
			}
		}

		if (!sawEnd)
			return failAt(lastRecord(), MissingRecord("end-map"));
		if (mapId == null)
			return failAt(header, MissingRecord("map"));
		if (assetPack == null)
			return failAt(header, MissingRecord("asset-pack"));
		if (title == null)
			return failAt(header, MissingRecord("title"));
		if (mode == null)
			return failAt(header, MissingRecord("mode"));
		if (worldSize == null)
			return failAt(header, MissingRecord("world"));

		final coordinates = [for (record in records) record.coordinate];
		return ReadOk({
			candidate: {
				formatVersion: 1,
				requiredFeatures: requiredFeatures,
				optionalFeatures: optionalFeatures,
				id: mapId,
				assetPack: assetPack,
				title: title,
				mode: mode,
				world: {
					size: worldSize,
					palette: palette,
					chunks: chunks
				},
				objects: objects,
				story: {
					dialogues: dialogues,
					journal: journal,
					objectives: objectives,
					routes: routes
				},
				flow: {variables: variables, sequences: sequences, rules: rules},
				extensions: extensions
			},
			recordCoordinates: coordinates,
			sourceLocations: locations
		});
	}

	function parseChunk():ScenarioReadResult<VoxelChunk> {
		final header = records[index];
		if (!count(header, 8))
			return failAt(header, InvalidToken);
		final id = scenarioId(header.tokens[1]);
		final origin = point(header, 2);
		final chunkSize = size(header, 5);
		if (id == null || origin == null || chunkSize == null)
			return failAt(header, InvalidToken);
		locate(Chunk(id), header);
		index++;
		final runs:Array<VoxelRun> = [];
		while (index < records.length && !isEnd(records[index], "chunk")) {
			final record = records[index];
			if (record.indent != 2 || !count(record, 3) || firstText(record) != "run")
				return failAt(record, UnexpectedRecord(firstText(record)));
			final code = integer(record.tokens[1]);
			final runCount = integer(record.tokens[2]);
			if (code == null || runCount == null)
				return failAt(record, IntegerOutOfRange);
			runs.push({paletteCode: code, count: runCount});
			index++;
		}
		if (index >= records.length)
			return failAt(header, MissingRecord("end chunk"));
		index++;
		return ReadOk({
			id: id,
			origin: origin,
			size: chunkSize,
			runs: runs
		});
	}

	function parseObject():ScenarioReadResult<ScenarioObject> {
		final header = records[index];
		if (!count(header, 2))
			return failAt(header, InvalidToken);
		final id = scenarioId(header.tokens[1]);
		if (id == null)
			return failToken(header.tokens[1], InvalidToken);
		locate(Object(id), header);
		index++;
		final tags:Array<ScenarioTag> = [];
		var placement:Null<ObjectPlacement> = null;
		while (index < records.length && !isEnd(records[index], "object")) {
			final record = records[index];
			if (record.indent != 2)
				return failAt(record, UnexpectedRecord(firstText(record)));
			switch firstText(record) {
				case "tag":
					if (!count(record, 2))
						return failAt(record, InvalidToken);
					final tag = scenarioTag(record.tokens[1]);
					if (tag == null)
						return failToken(record.tokens[1], InvalidToken);
					tags.push(tag);
				case "placement":
					if (placement != null)
						return failAt(record, InvalidToken);
					placement = placementValue(record);
					if (placement == null)
						return failAt(record, InvalidToken);
				case other:
					return failAt(record, UnexpectedRecord(other));
			}
			index++;
		}
		if (index >= records.length)
			return failAt(header, MissingRecord("end object"));
		if (placement == null)
			return failAt(header, MissingRecord("placement"));
		index++;
		return ReadOk({id: id, tags: tags, placement: placement});
	}

	function placementValue(record:ScenarioLexRecord):Null<ObjectPlacement> {
		if (record.tokens.length < 2)
			return null;
		return switch record.tokens[1].text {
			case "player-spawn" if (count(record, 6)): transform(record, 2) == null ? null : PlayerSpawn(transform(record, 2));
			case "checkpoint" if (count(record, 6)): transform(record, 2) == null ? null : Checkpoint(transform(record, 2));
			case "item" if (count(record, 8)): final content = contentId(record.tokens[2]); final quantity = integer(record.tokens[3]); final position = transform(record,
					4); content == null || quantity == null || position == null ? null : Item(content, quantity, position);
			case "entity" if (count(record, 7)): final content = contentId(record.tokens[2]); final position = transform(record,
					3); content == null || position == null ? null : Entity(content, position);
			case "npc" if (count(record, 8)): final content = contentId(record.tokens[2]); final dialogue = scenarioId(record.tokens[3]); final position = transform(record,
					4); content == null || dialogue == null || position == null ? null : Npc(content, dialogue, position);
			case "prefab" if (count(record, 7)): final content = contentId(record.tokens[2]); final position = transform(record,
					3); content == null || position == null ? null : Prefab(content, position);
			case "trigger-zone" if (count(record, 8)): final origin = point(record,
					2); final zoneSize = size(record, 5); origin == null || zoneSize == null ? null : TriggerZone({origin: origin, size: zoneSize});
			case "stateful" if (count(record, 8)): final content = contentId(record.tokens[2]); final state = contentId(record.tokens[3]); final position = transform(record,
					4); content == null || state == null || position == null ? null : StatefulObject(content, state, position);
			case _: null;
		}
	}

	function parseDialogue():ScenarioReadResult<ScenarioDialogue> {
		final header = records[index];
		if (!count(header, 2))
			return failAt(header, InvalidToken);
		final id = scenarioId(header.tokens[1]);
		if (id == null)
			return failToken(header.tokens[1], InvalidToken);
		locate(Dialogue(id), header);
		index++;
		final lines:Array<ScenarioDialogueLine> = [];
		while (index < records.length && !isEnd(records[index], "dialogue")) {
			final record = records[index];
			if (record.indent != 2 || record.tokens.length < 4 || firstText(record) != "line")
				return failAt(record, UnexpectedRecord(firstText(record)));
			final speaker = record.tokens[1].text == "narrator" ? null : scenarioId(record.tokens[1]);
			if (record.tokens[1].text != "narrator" && speaker == null)
				return failToken(record.tokens[1], InvalidToken);
			final parsed = text(record, 2);
			if (parsed == null || parsed.next != record.tokens.length)
				return failAt(record, InvalidToken);
			lines.push({speaker: speaker, text: parsed.value});
			index++;
		}
		if (index >= records.length)
			return failAt(header, MissingRecord("end dialogue"));
		index++;
		return ReadOk({id: id, lines: lines});
	}

	function parseJournal():ScenarioReadResult<ScenarioJournalEntry> {
		final header = records[index];
		if (header.tokens.length < 5 || header.tokens[2].text != "title")
			return failAt(header, InvalidToken);
		final id = scenarioId(header.tokens[1]);
		final title = text(header, 3);
		if (id == null || title == null || title.next != header.tokens.length)
			return failAt(header, InvalidToken);
		locate(Journal(id), header);
		index++;
		if (index >= records.length || records[index].indent != 2 || firstText(records[index]) != "body")
			return failAt(header, MissingRecord("journal body"));
		final bodyRecord = records[index];
		final body = text(bodyRecord, 1);
		if (body == null || body.next != bodyRecord.tokens.length)
			return failAt(bodyRecord, InvalidToken);
		index++;
		if (index >= records.length || !isEnd(records[index], "journal"))
			return failAt(header, MissingRecord("end journal"));
		index++;
		return ReadOk({id: id, title: title.value, body: body.value});
	}

	function parseObjective():ScenarioReadResult<ScenarioObjective> {
		final header = records[index];
		if (header.tokens.length < 5)
			return failAt(header, InvalidToken);
		final id = scenarioId(header.tokens[1]);
		final state = objectiveState(header.tokens[2]);
		if (id == null || state == null || header.tokens[3].text != "title")
			return failAt(header, InvalidToken);
		locate(Objective(id), header);
		final title = text(header, 4);
		if (title == null || title.next != header.tokens.length)
			return failAt(header, InvalidToken);
		index++;
		if (index >= records.length || records[index].indent != 2 || firstText(records[index]) != "body")
			return failAt(header, MissingRecord("objective body"));
		final bodyRecord = records[index];
		final body = text(bodyRecord, 1);
		if (body == null || body.next != bodyRecord.tokens.length)
			return failAt(bodyRecord, InvalidToken);
		index++;
		if (index >= records.length || !isEnd(records[index], "objective"))
			return failAt(header, MissingRecord("end objective"));
		index++;
		return ReadOk({
			id: id,
			title: title.value,
			body: body.value,
			initialState: state
		});
	}

	function parseRoute():ScenarioReadResult<ScenarioRoute> {
		final header = records[index];
		if (header.tokens.length < 5 || header.tokens[2].text != "title")
			return failAt(header, InvalidToken);
		final id = scenarioId(header.tokens[1]);
		final title = text(header, 3);
		if (id == null || title == null || title.next != header.tokens.length)
			return failAt(header, InvalidToken);
		locate(Route(id), header);
		index++;
		final objectives:Array<ScenarioId> = [];
		while (index < records.length && !isEnd(records[index], "route")) {
			final record = records[index];
			if (record.indent != 2 || !count(record, 2) || firstText(record) != "objective")
				return failAt(record, UnexpectedRecord(firstText(record)));
			final objective = scenarioId(record.tokens[1]);
			if (objective == null)
				return failToken(record.tokens[1], InvalidToken);
			objectives.push(objective);
			index++;
		}
		if (index >= records.length)
			return failAt(header, MissingRecord("end route"));
		index++;
		return ReadOk({id: id, title: title.value, objectives: objectives});
	}

	function parseVariable(record:ScenarioLexRecord):ScenarioReadResult<FlowVariable> {
		if (record.tokens.length < 5)
			return failAt(record, InvalidToken);
		final id = scenarioId(record.tokens[1]);
		if (id == null)
			return failToken(record.tokens[1], InvalidToken);
		var valueIndex = 3;
		final scope:Null<FlowScope> = switch record.tokens[2].text {
			case "map": Map;
			case "player": Player;
			case "quest": Quest;
			case "local" if (record.tokens.length >= 6):
				valueIndex = 4;
				final sequence = scenarioId(record.tokens[3]);
				sequence == null ? null : Local(sequence);
			case _: null;
		}
		if (scope == null)
			return failToken(record.tokens[2], InvalidToken);
		final value = flowValue(record, valueIndex);
		if (value == null || valueIndex + 2 != record.tokens.length)
			return failAt(record, InvalidToken);
		return ReadOk({id: id, scope: scope, initial: value});
	}

	function parseSequence():ScenarioReadResult<FlowSequence> {
		final header = records[index];
		if (!count(header, 2))
			return failAt(header, InvalidToken);
		final id = scenarioId(header.tokens[1]);
		if (id == null)
			return failToken(header.tokens[1], InvalidToken);
		locate(Sequence(id), header);
		index++;
		final parameters:Array<FlowParameter> = [];
		final actions:Array<FlowAction> = [];
		while (index < records.length && !isEnd(records[index], "sequence")) {
			final record = records[index];
			if (record.indent != 2)
				return failAt(record, UnexpectedRecord(firstText(record)));
			switch firstText(record) {
				case "parameter":
					if (record.tokens.length != 4)
						return failAt(record, InvalidToken);
					final parameterId = scenarioId(record.tokens[1]);
					final initial = flowValue(record, 2);
					if (parameterId == null || initial == null)
						return failAt(record, InvalidToken);
					parameters.push({id: parameterId, initial: initial});
					index++;
				case "do":
					final parsed = parseActionRecord(record, 1, 2);
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(action): actions.push(action);
					}
				case other:
					return failAt(record, UnexpectedRecord(other));
			}
		}
		if (index >= records.length)
			return failAt(header, MissingRecord("end sequence"));
		index++;
		return ReadOk({id: id, parameters: parameters, actions: actions});
	}

	function parseRule():ScenarioReadResult<FlowRule> {
		final header = records[index];
		if (header.tokens.length < 5 || header.tokens[2].text != "priority")
			return failAt(header, InvalidToken);
		final id = scenarioId(header.tokens[1]);
		final priority = integer(header.tokens[3]);
		var repeat:Null<FlowRepeatPolicy> = null;
		if (header.tokens[4].text == "once" && header.tokens.length == 5)
			repeat = Once;
		else if (header.tokens[4].text == "repeat" && header.tokens.length == 5)
			repeat = Repeat;
		else if (header.tokens[4].text == "cooldown" && header.tokens.length == 6) {
			final ticks = integer(header.tokens[5]);
			if (ticks != null)
				repeat = Cooldown(ticks);
		}
		if (id == null || priority == null || repeat == null)
			return failAt(header, InvalidToken);
		locate(Rule(id), header);
		index++;
		var event:Null<FlowEvent> = null;
		var predicate:Null<FlowPredicate> = null;
		final actions:Array<FlowAction> = [];
		while (index < records.length && !isEnd(records[index], "rule")) {
			final record = records[index];
			if (record.indent != 2)
				return failAt(record, UnexpectedRecord(firstText(record)));
			switch firstText(record) {
				case "when":
					if (event != null)
						return failAt(record, InvalidRule(id));
					event = eventValue(record);
					if (event == null)
						return failAt(record, InvalidToken);
				case "if":
					if (predicate != null)
						return failAt(record, InvalidRule(id));
					final parsed = predicateValue(record, 1, 0);
					if (parsed == null || parsed.next != record.tokens.length)
						return failAt(record, InvalidToken);
					predicate = parsed.value;
				case "do":
					final parsed = parseActionRecord(record, 1, 2);
					switch parsed {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(action): actions.push(action);
					}
				case other:
					return failAt(record, UnexpectedRecord(other));
			}
			if (firstText(record) != "do")
				index++;
		}
		if (index >= records.length)
			return failAt(header, MissingRecord("end rule"));
		if (event == null || predicate == null)
			return failAt(header, InvalidRule(id));
		index++;
		return ReadOk({
			id: id,
			priority: priority,
			repeat: repeat,
			event: event,
			predicate: predicate,
			actions: actions
		});
	}

	function parseExtension():ScenarioReadResult<ScenarioExtension> {
		final header = records[index];
		if (!count(header, 3))
			return failAt(header, InvalidToken);
		final feature = contentId(header.tokens[1]);
		final id = scenarioId(header.tokens[2]);
		if (feature == null || id == null)
			return failAt(header, InvalidToken);
		locate(Extension(feature, id), header);
		index++;
		if (index >= records.length || records[index].indent != 2 || !count(records[index], 2) || firstText(records[index]) != "data"
			|| !records[index].tokens[1].quoted)
			return failAt(header, MissingRecord("extension data"));
		final data = records[index].tokens[1].text;
		index++;
		if (index >= records.length || !isEnd(records[index], "extension"))
			return failAt(header, MissingRecord("end extension"));
		index++;
		return ReadOk({feature: feature, id: id, data: data});
	}

	function parseActionRecord(record:ScenarioLexRecord, at:Int, indent:Int):ScenarioReadResult<FlowAction> {
		if (at >= record.tokens.length)
			return failAt(record, InvalidToken);
		if (record.tokens[at].text != "choose") {
			final action = actionValue(record, at);
			if (action == null)
				return failAt(record, InvalidToken);
			index++;
			return ReadOk(action);
		}
		if (record.tokens.length != at + 3)
			return failAt(record, InvalidToken);
		final seed = scenarioId(record.tokens[at + 1]);
		final choiceCount = integer(record.tokens[at + 2]);
		if (seed == null || choiceCount == null || choiceCount <= 0)
			return failAt(record, InvalidToken);
		index++;
		final choices:Array<FlowChoice> = [];
		while (choices.length < choiceCount) {
			if (index >= records.length)
				return failAt(record, MissingRecord("choice"));
			final choiceHeader = records[index];
			if (choiceHeader.indent != indent || choiceHeader.tokens.length != 3 || firstText(choiceHeader) != "choice"
				|| choiceHeader.tokens[1].text != "weight")
				return failAt(choiceHeader, UnexpectedRecord(firstText(choiceHeader)));
			final weight = integer(choiceHeader.tokens[2]);
			if (weight == null || weight <= 0)
				return failAt(choiceHeader, IntegerOutOfRange);
			index++;
			final actions:Array<FlowAction> = [];
			while (index < records.length && !isEndAt(records[index], "choice", indent)) {
				final actionRecord = records[index];
				if (actionRecord.indent != indent + 2 || firstText(actionRecord) != "do" || actionRecord.tokens.length < 2
					|| actionRecord.tokens[1].text == "choose")
					return failAt(actionRecord, UnexpectedRecord(firstText(actionRecord)));
				final action = actionValue(actionRecord, 1);
				if (action == null)
					return failAt(actionRecord, InvalidToken);
				actions.push(action);
				index++;
			}
			if (index >= records.length)
				return failAt(choiceHeader, MissingRecord("end choice"));
			index++;
			choices.push({weight: weight, actions: actions});
		}
		return ReadOk(ChooseSeeded(seed, choices));
	}

	function eventValue(record:ScenarioLexRecord):Null<FlowEvent> {
		if (record.tokens.length < 3)
			return null;
		return switch record.tokens[1].text {
			case "enter-zone" if (count(record, 3)): idEvent(record.tokens[2], EnterZone);
			case "leave-zone" if (count(record, 3)): idEvent(record.tokens[2], LeaveZone);
			case "interact" if (count(record, 3)): idEvent(record.tokens[2], Interact);
			case "entity-defeated" if (count(record, 3)): idEvent(record.tokens[2], EntityDefeated);
			case "objective-changed" if (count(record, 3)): idEvent(record.tokens[2], ObjectiveChanged);
			case "state-changed" if (count(record, 3)): idEvent(record.tokens[2], StateChanged);
			case "timer" if (count(record, 3)): idEvent(record.tokens[2], TimerExpired);
			case "use-item" if (count(record, 3)):
				final content = contentId(record.tokens[2]);
				content == null ? null : UseItem(content);
			case "signal" if (count(record, 3)):
				final content = contentId(record.tokens[2]);
				content == null ? null : SignalReceived(content);
			case "block-changed" if (count(record, 4)): final zone = scenarioId(record.tokens[2]); final block = contentId(record.tokens[3]); zone == null || block == null ? null : BlockChanged(zone,
					block);
			case _: null;
		}
	}

	function predicateValue(record:ScenarioLexRecord, at:Int, depth:Int):Null<ParsedPredicate> {
		if (depth >= ScenarioLimits.MAX_PREDICATE_DEPTH || at >= record.tokens.length || record.tokens[at].text != "(")
			return null;
		var next = at + 1;
		if (next >= record.tokens.length)
			return null;
		final name = record.tokens[next++].text;
		var value:Null<FlowPredicate> = null;
		switch name {
			case "always":
				value = Always;
			case "all", "any":
				final children:Array<FlowPredicate> = [];
				while (next < record.tokens.length && record.tokens[next].text == "(") {
					final child = predicateValue(record, next, depth + 1);
					if (child == null)
						return null;
					children.push(child.value);
					next = child.next;
				}
				value = name == "all" ? All(children) : AnyOf(children);
			case "not":
				final child = predicateValue(record, next, depth + 1);
				if (child == null)
					return null;
				value = Not(child.value);
				next = child.next;
			case "flag" if (next + 2 <= record.tokens.length):
				final id = scenarioId(record.tokens[next]);
				final expected = boolean(record.tokens[next + 1]);
				if (id == null || expected == null)
					return null;
				value = FlagIs(id, expected);
				next += 2;
			case "counter" if (next + 3 <= record.tokens.length):
				final id = scenarioId(record.tokens[next]);
				final comparison = comparison(record.tokens[next + 1]);
				final expected = integer(record.tokens[next + 2]);
				if (id == null || comparison == null || expected == null)
					return null;
				value = CounterCompare(id, comparison, expected);
				next += 3;
			case "state" if (next + 2 <= record.tokens.length):
				final id = scenarioId(record.tokens[next]);
				final expected = contentId(record.tokens[next + 1]);
				if (id == null || expected == null)
					return null;
				value = StateIs(id, expected);
				next += 2;
			case "object-state" if (next + 2 <= record.tokens.length):
				final id = scenarioId(record.tokens[next]);
				final expected = contentId(record.tokens[next + 1]);
				if (id == null || expected == null)
					return null;
				value = ObjectStateIs(id, expected);
				next += 2;
			case "objective" if (next + 2 <= record.tokens.length):
				final id = scenarioId(record.tokens[next]);
				final expected = objectiveState(record.tokens[next + 1]);
				if (id == null || expected == null)
					return null;
				value = ObjectiveIs(id, expected);
				next += 2;
			case "inventory-has" if (next + 4 <= record.tokens.length):
				final owner = scenarioId(record.tokens[next]);
				final item = contentId(record.tokens[next + 1]);
				final operation = comparison(record.tokens[next + 2]);
				final quantity = integer(record.tokens[next + 3]);
				if (owner == null || item == null || operation == null || quantity == null)
					return null;
				value = InventoryHas(owner, item, operation, quantity);
				next += 4;
			case "near" if (next + 3 <= record.tokens.length):
				final actor = scenarioId(record.tokens[next]);
				final objectId = scenarioId(record.tokens[next + 1]);
				final maximum = integer(record.tokens[next + 2]);
				if (actor == null || objectId == null || maximum == null)
					return null;
				value = NearObject(actor, objectId, maximum);
				next += 3;
			case "mode" if (next < record.tokens.length):
				value = switch record.tokens[next].text {
					case "creative": ModeIs(Creative);
					case "adventure": ModeIs(Adventure);
					case _: null;
				}
				next++;
			case _:
		}
		if (value == null || next >= record.tokens.length || record.tokens[next].text != ")")
			return null;
		return {value: value, next: next + 1};
	}

	function actionValue(record:ScenarioLexRecord, at:Int):Null<FlowAction> {
		if (at >= record.tokens.length)
			return null;
		final name = record.tokens[at].text;
		final remaining = record.tokens.length - at;
		return switch name {
			case "dialogue" if (remaining == 2): actionId(record.tokens[at + 1], ShowDialogue);
			case "journal" if (remaining == 2): actionId(record.tokens[at + 1], AddJournal);
			case "spawn" if (remaining == 2): actionId(record.tokens[at + 1], Spawn);
			case "despawn" if (remaining == 2): actionId(record.tokens[at + 1], Despawn);
			case "checkpoint" if (remaining == 2): actionId(record.tokens[at + 1], SetCheckpoint);
			case "set-flag" if (remaining == 3): final id = scenarioId(record.tokens[at + 1]); final value = boolean(record.tokens[at + 2]); id == null || value == null ? null : SetFlag(id,
					value);
			case "set-counter",
				"add-counter" if (remaining == 3): final id = scenarioId(record.tokens[at + 1]); final value = integer(record.tokens[at + 2]); id == null || value == null ? null : (name == "set-counter" ? SetCounter(id,
					value) : AddCounter(id, value));
			case "set-state" if (remaining == 3): final id = scenarioId(record.tokens[at + 1]); final value = contentId(record.tokens[at + 2]); id == null || value == null ? null : SetState(id,
					value);
			case "set-object-state" if (remaining == 3): final id = scenarioId(record.tokens[at + 1]); final value = contentId(record.tokens[at + 2]); id == null || value == null ? null : SetObjectState(id,
					value);
			case "objective" if (remaining == 3): final id = scenarioId(record.tokens[at + 1]); final value = objectiveState(record.tokens[at + 2]); id == null || value == null ? null : SetObjective(id,
					value);
			case "give-item",
				"take-item" if (remaining == 4): final owner = scenarioId(record.tokens[at + 1]); final item = contentId(record.tokens[at + 2]); final quantity = integer(record.tokens[at
					+ 3]); owner == null || item == null || quantity == null ? null : (name == "give-item" ? GiveItem(owner, item,
					quantity) : TakeItem(owner, item, quantity));
			case "signal" if (remaining == 2):
				final signal = contentId(record.tokens[at + 1]);
				signal == null ? null : EmitSignal(signal);
			case "effect" if (remaining == 2 || (remaining == 4 && record.tokens[at + 2].text == "at")): final effect = contentId(record.tokens[at + 1]); final target = remaining == 4 ? scenarioId(record.tokens[at
					+ 3]) : null; effect == null || (remaining == 4 && target == null) ? null : PlayEffect(effect, target);
			case "schedule" if (remaining >= 4):
				final timer = scenarioId(record.tokens[at + 1]);
				final ticks = integer(record.tokens[at + 2]);
				final sequence = scenarioId(record.tokens[at + 3]);
				final arguments = arguments(record, at + 4);
				timer == null
				|| ticks == null
				|| sequence == null
				|| arguments == null ? null : Schedule(timer, ticks, sequence, arguments);
			case "call" if (remaining >= 2): final sequence = scenarioId(record.tokens[at + 1]); final arguments = arguments(record,
					at + 2); sequence == null || arguments == null ? null : CallSequence(sequence, arguments);
			case _: null;
		}
	}

	function arguments(record:ScenarioLexRecord, at:Int):Null<Array<FlowArgument>> {
		final result:Array<FlowArgument> = [];
		var next = at;
		while (next < record.tokens.length) {
			final parsed = argument(record, next);
			if (parsed == null)
				return null;
			result.push(parsed.value);
			next = parsed.next;
		}
		return result;
	}

	function argument(record:ScenarioLexRecord, at:Int):Null<ParsedArgument> {
		if (at >= record.tokens.length)
			return null;
		return switch record.tokens[at].text {
			case "value":
				final value = flowValue(record, at + 1);
				value == null ? null : {value: Value(value), next: at + 3};
			case "variable" if (at + 1 < record.tokens.length):
				final id = scenarioId(record.tokens[at + 1]);
				id == null ? null : {value: Variable(id), next: at + 2};
			case "object" if (at + 1 < record.tokens.length):
				final id = scenarioId(record.tokens[at + 1]);
				id == null ? null : {value: Object(id), next: at + 2};
			case _: null;
		}
	}

	function flowValue(record:ScenarioLexRecord, at:Int):Null<FlowValue> {
		if (at + 1 >= record.tokens.length)
			return null;
		return switch record.tokens[at].text {
			case "flag":
				final value = boolean(record.tokens[at + 1]);
				value == null ? null : Flag(value);
			case "counter":
				final value = integer(record.tokens[at + 1]);
				value == null ? null : Counter(value);
			case "state":
				final value = contentId(record.tokens[at + 1]);
				value == null ? null : State(value);
			case _: null;
		}
	}

	function text(record:ScenarioLexRecord, at:Int):Null<ParsedText> {
		if (at + 1 >= record.tokens.length)
			return null;
		return switch record.tokens[at].text {
			case "message":
				final id = messageId(record.tokens[at + 1]);
				id == null ? null : {value: Message(id), next: at + 2};
			case "literal" if (record.tokens[at + 1].quoted): {value: Literal(record.tokens[at + 1].text), next: at + 2};
			case _: null;
		}
	}

	function transform(record:ScenarioLexRecord, at:Int):Null<ScenarioTransform> {
		if (at + 3 >= record.tokens.length)
			return null;
		final x = integer(record.tokens[at]);
		final y = integer(record.tokens[at + 1]);
		final z = integer(record.tokens[at + 2]);
		final yaw = integer(record.tokens[at + 3]);
		return x == null || y == null || z == null || yaw == null ? null : {
			xMilli: x,
			yMilli: y,
			zMilli: z,
			yawDegrees: yaw
		};
	}

	function point(record:ScenarioLexRecord, at:Int):Null<VoxelPoint> {
		if (at + 2 >= record.tokens.length)
			return null;
		final x = integer(record.tokens[at]);
		final y = integer(record.tokens[at + 1]);
		final z = integer(record.tokens[at + 2]);
		return x == null || y == null || z == null ? null : {x: x, y: y, z: z};
	}

	function size(record:ScenarioLexRecord, at:Int):Null<VoxelSize> {
		if (at + 2 >= record.tokens.length)
			return null;
		final width = integer(record.tokens[at]);
		final height = integer(record.tokens[at + 1]);
		final depth = integer(record.tokens[at + 2]);
		return width == null || height == null || depth == null ? null : {width: width, height: height, depth: depth};
	}

	function integer(token:ScenarioLexToken):Null<Int> {
		final text = token.text;
		if (token.quoted || text.length == 0 || text.charCodeAt(0) == 43)
			return null;
		var negative = false;
		var at = 0;
		if (text.charCodeAt(0) == 45) {
			negative = true;
			at = 1;
		}
		if (at == text.length || (text.charCodeAt(at) == 48 && at + 1 != text.length) || (negative && text == "-0"))
			return null;
		final limit = negative ? -2147483648 : -2147483647;
		final multiplyLimit = -214748364;
		var result = 0;
		while (at < text.length) {
			final code = text.charCodeAt(at);
			if (code < 48 || code > 57 || result < multiplyLimit)
				return null;
			result *= 10;
			final digit = code - 48;
			if (result < limit + digit)
				return null;
			result -= digit;
			at++;
		}
		return negative ? result : -result;
	}

	function boolean(token:ScenarioLexToken):Null<Bool> {
		return switch token.text {
			case "true": true;
			case "false": false;
			case _: null;
		}
	}

	function objectiveState(token:ScenarioLexToken):Null<ObjectiveState> {
		return switch token.text {
			case "hidden": Hidden;
			case "active": Active;
			case "complete": Complete;
			case "failed": Failed;
			case _: null;
		}
	}

	function comparison(token:ScenarioLexToken):Null<FlowComparison> {
		return switch token.text {
			case "equal": Equal;
			case "not-equal": NotEqual;
			case "less": Less;
			case "less-or-equal": LessOrEqual;
			case "greater": Greater;
			case "greater-or-equal": GreaterOrEqual;
			case _: null;
		}
	}

	function scenarioId(token:ScenarioLexToken):Null<ScenarioId>
		return !token.quoted && validId(token.text) ? new ScenarioId(token.text) : null;

	function scenarioTag(token:ScenarioLexToken):Null<ScenarioTag>
		return !token.quoted && validId(token.text) ? new ScenarioTag(token.text) : null;

	function messageId(token:ScenarioLexToken):Null<MessageId>
		return !token.quoted && validId(token.text) ? new MessageId(token.text) : null;

	function contentId(token:ScenarioLexToken):Null<ContentId> {
		if (token.quoted)
			return null;
		final separator = token.text.indexOf(":");
		return separator <= 0
			|| separator != token.text.lastIndexOf(":")
			|| !validId(token.text.substring(0, separator))
			|| !validId(token.text.substring(separator + 1)) ? null : new ContentId(token.text);
	}

	function logicalPath(token:ScenarioLexToken):Null<LogicalPath> {
		if (token.quoted || token.text.length == 0 || token.text.indexOf("\\") != -1 || token.text.charAt(0) == "/")
			return null;
		for (part in token.text.split("/"))
			if (part == "." || part == ".." || !validId(part))
				return null;
		return new LogicalPath(token.text);
	}

	function validId(value:String):Bool {
		if (value.length == 0)
			return false;
		var expectLetter = true;
		for (at in 0...value.length) {
			final code = value.charCodeAt(at);
			if (expectLetter) {
				if (code < 97 || code > 122)
					return false;
				expectLetter = false;
			} else if (code == 46 || code == 95 || code == 45) {
				expectLetter = true;
			} else if (!((code >= 97 && code <= 122) || (code >= 48 && code <= 57))) {
				return false;
			}
		}
		return !expectLetter;
	}

	function idEvent(token:ScenarioLexToken, make:ScenarioId->FlowEvent):Null<FlowEvent> {
		final id = scenarioId(token);
		return id == null ? null : make(id);
	}

	function actionId(token:ScenarioLexToken, make:ScenarioId->FlowAction):Null<FlowAction> {
		final id = scenarioId(token);
		return id == null ? null : make(id);
	}

	function isEnd(record:ScenarioLexRecord, kind:String):Bool
		return isEndAt(record, kind, 0);

	function isEndAt(record:ScenarioLexRecord, kind:String, indent:Int):Bool
		return record.indent == indent && count(record, 2) && firstText(record) == "end" && record.tokens[1].text == kind;

	function count(record:ScenarioLexRecord, expected:Int):Bool
		return record.tokens.length == expected;

	function firstText(record:ScenarioLexRecord):String
		return record.tokens.length == 0 ? "" : record.tokens[0].text;

	function lastRecord():ScenarioLexRecord
		return records[records.length - 1];

	function locate(subject:ScenarioSourceSubject, record:ScenarioLexRecord):Void
		locations.push({subject: subject, coordinate: record.coordinate});

	function failAt<T>(record:Null<ScenarioLexRecord>, kind:ScenarioDiagnosticKind):ScenarioReadResult<T> {
		final coordinate:ScenarioCoordinate = record == null ? {line: 1, column: 1, record: 0} : record.coordinate;
		return ReadError([{coordinate: coordinate, kind: kind}]);
	}

	function failToken<T>(token:ScenarioLexToken, kind:ScenarioDiagnosticKind):ScenarioReadResult<T>
		return ReadError([{coordinate: token.coordinate, kind: kind}]);
}

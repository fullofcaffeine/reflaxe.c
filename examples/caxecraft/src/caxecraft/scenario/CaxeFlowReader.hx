package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowChoice;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowParameter;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;

/** Reads CaxeFlow declarations and the block structure around typed values. */
// Internal helper. Application code should call ScenarioParser.parse(...) instead.
@:noCompletion
final class CaxeFlowReader {
	final cursor:ScenarioRecordCursor;

	public function new(cursor:ScenarioRecordCursor)
		this.cursor = cursor;

	public function readVariable():ScenarioReadResult<FlowVariable> {
		final record = cursor.current();
		if (record.tokens.length < 5)
			return cursor.failAt(record, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(record.tokens[1]);
		if (id == null)
			return cursor.failToken(record.tokens[1], InvalidToken);
		var valueIndex = 3;
		final scope:Null<FlowScope> = switch record.tokens[2].text {
			case "map": Map;
			case "player": Player;
			case "quest": Quest;
			case "local" if (record.tokens.length >= 6):
				valueIndex = 4;
				final sequence = ScenarioTokenGrammar.scenarioId(record.tokens[3]);
				sequence == null ? null : Local(sequence);
			case _:
				null;
		}
		if (scope == null)
			return cursor.failToken(record.tokens[2], InvalidToken);
		final value = CaxeFlowValueReader.flowValue(record, valueIndex);
		if (value == null || valueIndex + 2 != record.tokens.length)
			return cursor.failAt(record, InvalidToken);
		cursor.locate(Variable(id), record);
		cursor.advance();
		return ReadOk({id: id, scope: scope, initial: value});
	}

	public function readSequence():ScenarioReadResult<FlowSequence> {
		final header = cursor.current();
		if (!ScenarioTokenGrammar.hasTokenCount(header, 2))
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		if (id == null)
			return cursor.failToken(header.tokens[1], InvalidToken);
		cursor.locate(Sequence(id), header);
		cursor.advance();

		final parameters:Array<FlowParameter> = [];
		final actions:Array<FlowAction> = [];
		while (cursor.hasRecord() && !ScenarioTokenGrammar.isEnd(cursor.current(), "sequence")) {
			final record = cursor.current();
			if (record.indent != 2)
				return cursor.failAt(record, UnexpectedRecord(ScenarioTokenGrammar.firstText(record)));
			switch ScenarioTokenGrammar.firstText(record) {
				case "parameter":
					if (record.tokens.length != 4)
						return cursor.failAt(record, InvalidToken);
					final parameterId = ScenarioTokenGrammar.scenarioId(record.tokens[1]);
					final initial = CaxeFlowValueReader.flowValue(record, 2);
					if (parameterId == null || initial == null)
						return cursor.failAt(record, InvalidToken);
					parameters.push({id: parameterId, initial: initial});
					cursor.advance();
				case "do":
					switch readActionRecord(record, 1, 2) {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(action): actions.push(action);
					}
				case other:
					return cursor.failAt(record, UnexpectedRecord(other));
			}
		}
		if (!cursor.hasRecord())
			return cursor.failAt(header, MissingRecord("end sequence"));
		cursor.advance();
		return ReadOk({id: id, parameters: parameters, actions: actions});
	}

	public function readRule():ScenarioReadResult<FlowRule> {
		final header = cursor.current();
		if (header.tokens.length < 5 || header.tokens[2].text != "priority")
			return cursor.failAt(header, InvalidToken);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[1]);
		final priority = ScenarioTokenGrammar.integer(header.tokens[3]);
		var repeat:Null<FlowRepeatPolicy> = null;
		if (header.tokens[4].text == "once" && header.tokens.length == 5)
			repeat = Once;
		else if (header.tokens[4].text == "repeat" && header.tokens.length == 5)
			repeat = Repeat;
		else if (header.tokens[4].text == "cooldown" && header.tokens.length == 6) {
			final ticks = ScenarioTokenGrammar.integer(header.tokens[5]);
			if (ticks != null)
				repeat = Cooldown(ticks);
		}
		if (id == null || priority == null || repeat == null)
			return cursor.failAt(header, InvalidToken);
		cursor.locate(Rule(id), header);
		cursor.advance();

		var event:Null<FlowEvent> = null;
		var predicate:Null<FlowPredicate> = null;
		final actions:Array<FlowAction> = [];
		while (cursor.hasRecord() && !ScenarioTokenGrammar.isEnd(cursor.current(), "rule")) {
			final record = cursor.current();
			if (record.indent != 2)
				return cursor.failAt(record, UnexpectedRecord(ScenarioTokenGrammar.firstText(record)));
			switch ScenarioTokenGrammar.firstText(record) {
				case "when":
					if (event != null)
						return cursor.failAt(record, InvalidRule(id));
					event = CaxeFlowValueReader.event(record);
					if (event == null)
						return cursor.failAt(record, InvalidToken);
					cursor.advance();
				case "if":
					if (predicate != null)
						return cursor.failAt(record, InvalidRule(id));
					final parsed = CaxeFlowValueReader.predicate(record, 1, 0);
					if (parsed == null || parsed.next != record.tokens.length)
						return cursor.failAt(record, InvalidToken);
					predicate = parsed.value;
					cursor.advance();
				case "do":
					switch readActionRecord(record, 1, 2) {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(action): actions.push(action);
					}
				case other:
					return cursor.failAt(record, UnexpectedRecord(other));
			}
		}
		if (!cursor.hasRecord())
			return cursor.failAt(header, MissingRecord("end rule"));
		if (event == null || predicate == null)
			return cursor.failAt(header, InvalidRule(id));
		cursor.advance();
		return ReadOk({
			id: id,
			priority: priority,
			repeat: repeat,
			event: event,
			predicate: predicate,
			actions: actions
		});
	}

	function readActionRecord(record:ScenarioLexRecord, at:Int, choiceIndent:Int):ScenarioReadResult<FlowAction> {
		if (at >= record.tokens.length)
			return cursor.failAt(record, InvalidToken);
		if (record.tokens[at].text != "choose") {
			final action = CaxeFlowValueReader.action(record, at);
			if (action == null)
				return cursor.failAt(record, InvalidToken);
			cursor.advance();
			return ReadOk(action);
		}
		if (record.tokens.length != at + 3)
			return cursor.failAt(record, InvalidToken);
		final seed = ScenarioTokenGrammar.scenarioId(record.tokens[at + 1]);
		final choiceCount = ScenarioTokenGrammar.integer(record.tokens[at + 2]);
		if (seed == null || choiceCount == null || choiceCount <= 0)
			return cursor.failAt(record, InvalidToken);
		cursor.advance();

		final choices:Array<FlowChoice> = [];
		while (choices.length < choiceCount) {
			if (!cursor.hasRecord())
				return cursor.failAt(record, MissingRecord("choice"));
			final choiceHeader = cursor.current();
			if (choiceHeader.indent != choiceIndent
				|| choiceHeader.tokens.length != 3
				|| ScenarioTokenGrammar.firstText(choiceHeader) != "choice"
				|| choiceHeader.tokens[1].text != "weight")
				return cursor.failAt(choiceHeader, UnexpectedRecord(ScenarioTokenGrammar.firstText(choiceHeader)));
			final weight = ScenarioTokenGrammar.integer(choiceHeader.tokens[2]);
			if (weight == null || weight <= 0)
				return cursor.failAt(choiceHeader, IntegerOutOfRange);
			cursor.advance();

			final actions:Array<FlowAction> = [];
			while (cursor.hasRecord() && !ScenarioTokenGrammar.isEndAt(cursor.current(), "choice", choiceIndent)) {
				final actionRecord = cursor.current();
				if (actionRecord.indent != choiceIndent + 2
					|| ScenarioTokenGrammar.firstText(actionRecord) != "do"
					|| actionRecord.tokens.length < 2
					|| actionRecord.tokens[1].text == "choose")
					return cursor.failAt(actionRecord, UnexpectedRecord(ScenarioTokenGrammar.firstText(actionRecord)));
				final action = CaxeFlowValueReader.action(actionRecord, 1);
				if (action == null)
					return cursor.failAt(actionRecord, InvalidToken);
				actions.push(action);
				cursor.advance();
			}
			if (!cursor.hasRecord())
				return cursor.failAt(choiceHeader, MissingRecord("end choice"));
			cursor.advance();
			choices.push({weight: weight, actions: actions});
		}
		return ReadOk(ChooseSeeded(seed, choices));
	}
}

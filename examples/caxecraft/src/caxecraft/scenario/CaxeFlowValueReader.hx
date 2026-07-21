package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionId;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionDescriptorForSyntax;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexToken;

typedef ParsedFlowPredicate = {
	final value:FlowPredicate;
	final next:Int;
}

private typedef ParsedFlowArgument = {
	final value:FlowArgument;
	final next:Int;
}

/**
	Reads the closed CaxeFlow value language inside one lexical record.

	These functions do not move the document cursor. The record-level reader can
	therefore decide atomically whether a whole WHEN, IF, or DO record is valid.
**/
// Used only by the CAXEMAP parser. Game and editor code should call
// ScenarioParser.parse(...) instead of calling this class.

@:noCompletion
final class CaxeFlowValueReader {
	public static function event(record:ScenarioLexRecord):Null<FlowEvent> {
		if (record.tokens.length < 3)
			return null;
		return switch ScenarioTokenGrammar.bareText(record.tokens[1]) {
			case "enter-zone" if (ScenarioTokenGrammar.hasTokenCount(record, 3)):
				idEvent(record.tokens[2], EnterZone);
			case "leave-zone" if (ScenarioTokenGrammar.hasTokenCount(record, 3)):
				idEvent(record.tokens[2], LeaveZone);
			case "interact" if (ScenarioTokenGrammar.hasTokenCount(record, 3)):
				idEvent(record.tokens[2], Interact);
			case "entity-defeated" if (ScenarioTokenGrammar.hasTokenCount(record, 3)):
				idEvent(record.tokens[2], EntityDefeated);
			case "objective-changed" if (ScenarioTokenGrammar.hasTokenCount(record, 3)):
				idEvent(record.tokens[2], ObjectiveChanged);
			case "state-changed" if (ScenarioTokenGrammar.hasTokenCount(record, 3)):
				idEvent(record.tokens[2], StateChanged);
			case "timer" if (ScenarioTokenGrammar.hasTokenCount(record, 3)):
				idEvent(record.tokens[2], TimerExpired);
			case "use-item" if (ScenarioTokenGrammar.hasTokenCount(record, 3)):
				final content = ScenarioTokenGrammar.contentId(record.tokens[2]);
				content == null ? null : UseItem(content);
			case "signal" if (ScenarioTokenGrammar.hasTokenCount(record, 3)):
				final content = ScenarioTokenGrammar.contentId(record.tokens[2]);
				content == null ? null : SignalReceived(content);
			case "block-changed" if (ScenarioTokenGrammar.hasTokenCount(record, 4)): final zone = ScenarioTokenGrammar.scenarioId(record.tokens[2]); final block = ScenarioTokenGrammar.contentId(record.tokens[3]); zone == null || block == null ? null : BlockChanged(zone,
					block);
			case _:
				null;
		}
	}

	public static function predicate(record:ScenarioLexRecord, at:Int, depth:Int):Null<ParsedFlowPredicate> {
		if (depth >= ScenarioLimits.MAX_PREDICATE_DEPTH
			|| at >= record.tokens.length
			|| !ScenarioTokenGrammar.isBare(record.tokens[at], "("))
			return null;
		var next = at + 1;
		if (next >= record.tokens.length)
			return null;
		final name = ScenarioTokenGrammar.bareText(record.tokens[next++]);
		var value:Null<FlowPredicate> = null;
		switch name {
			case "always":
				value = Always;
			case "all", "any":
				final children:Array<FlowPredicate> = [];
				while (next < record.tokens.length && ScenarioTokenGrammar.isBare(record.tokens[next], "(")) {
					final child = predicate(record, next, depth + 1);
					if (child == null)
						return null;
					children.push(child.value);
					next = child.next;
				}
				value = name == "all" ? All(children) : AnyOf(children);
			case "not":
				final child = predicate(record, next, depth + 1);
				if (child == null)
					return null;
				value = Not(child.value);
				next = child.next;
			case "flag" if (next + 2 <= record.tokens.length):
				final id = ScenarioTokenGrammar.scenarioId(record.tokens[next]);
				final expected = ScenarioTokenGrammar.boolean(record.tokens[next + 1]);
				if (id == null || expected == null)
					return null;
				value = FlagIs(id, expected);
				next += 2;
			case "counter" if (next + 3 <= record.tokens.length):
				final id = ScenarioTokenGrammar.scenarioId(record.tokens[next]);
				final comparison = ScenarioTokenGrammar.comparison(record.tokens[next + 1]);
				final expected = ScenarioTokenGrammar.integer(record.tokens[next + 2]);
				if (id == null || comparison == null || expected == null)
					return null;
				value = CounterCompare(id, comparison, expected);
				next += 3;
			case "state" if (next + 2 <= record.tokens.length):
				final id = ScenarioTokenGrammar.scenarioId(record.tokens[next]);
				final expected = ScenarioTokenGrammar.contentId(record.tokens[next + 1]);
				if (id == null || expected == null)
					return null;
				value = StateIs(id, expected);
				next += 2;
			case "object-state" if (next + 2 <= record.tokens.length):
				final id = ScenarioTokenGrammar.scenarioId(record.tokens[next]);
				final expected = ScenarioTokenGrammar.contentId(record.tokens[next + 1]);
				if (id == null || expected == null)
					return null;
				value = ObjectStateIs(id, expected);
				next += 2;
			case "objective" if (next + 2 <= record.tokens.length):
				final id = ScenarioTokenGrammar.scenarioId(record.tokens[next]);
				final expected = ScenarioTokenGrammar.objectiveState(record.tokens[next + 1]);
				if (id == null || expected == null)
					return null;
				value = ObjectiveIs(id, expected);
				next += 2;
			case "inventory-has" if (next + 4 <= record.tokens.length):
				final owner = ScenarioTokenGrammar.scenarioId(record.tokens[next]);
				final item = ScenarioTokenGrammar.contentId(record.tokens[next + 1]);
				final operation = ScenarioTokenGrammar.comparison(record.tokens[next + 2]);
				final quantity = ScenarioTokenGrammar.integer(record.tokens[next + 3]);
				if (owner == null || item == null || operation == null || quantity == null)
					return null;
				value = InventoryHas(owner, item, operation, quantity);
				next += 4;
			case "near" if (next + 3 <= record.tokens.length):
				final actor = ScenarioTokenGrammar.scenarioId(record.tokens[next]);
				final objectId = ScenarioTokenGrammar.scenarioId(record.tokens[next + 1]);
				final maximum = ScenarioTokenGrammar.integer(record.tokens[next + 2]);
				if (actor == null || objectId == null || maximum == null)
					return null;
				value = NearObject(actor, objectId, maximum);
				next += 3;
			case "mode" if (next < record.tokens.length):
				value = switch ScenarioTokenGrammar.bareText(record.tokens[next]) {
					case "creative": ModeIs(Creative);
					case "adventure": ModeIs(Adventure);
					case _: null;
				}
				next++;
			case _:
		}
		if (value == null || next >= record.tokens.length || !ScenarioTokenGrammar.isBare(record.tokens[next], ")"))
			return null;
		return {value: value, next: next + 1};
	}

	/** Read a non-branching action. Seeded choices are record-level blocks. */
	public static function action(record:ScenarioLexRecord, at:Int):Null<FlowAction> {
		if (at >= record.tokens.length)
			return null;
		final descriptor = flowActionDescriptorForSyntax(ScenarioTokenGrammar.bareText(record.tokens[at]));
		if (descriptor == null)
			return null;
		final remaining = record.tokens.length - at;
		return switch descriptor.id {
			case DialogueAction if (remaining == 2): actionId(record.tokens[at + 1], ShowDialogue);
			case JournalAction if (remaining == 2): actionId(record.tokens[at + 1], AddJournal);
			case SpawnAction if (remaining == 2): actionId(record.tokens[at + 1], Spawn);
			case DespawnAction if (remaining == 2): actionId(record.tokens[at + 1], Despawn);
			case CheckpointAction if (remaining == 2): actionId(record.tokens[at + 1], SetCheckpoint);
			case SetFlagAction if (remaining == 3): setFlagAction(record, at);
			case SetCounterAction if (remaining == 3): counterAction(record, at, SetCounter);
			case AddCounterAction if (remaining == 3): counterAction(record, at, AddCounter);
			case SetStateAction if (remaining == 3): stateAction(record, at, SetState);
			case SetObjectStateAction if (remaining == 3): stateAction(record, at, SetObjectState);
			case ObjectiveAction if (remaining == 3): objectiveAction(record, at);
			case GiveItemAction if (remaining == 4): inventoryAction(record, at, GiveItem);
			case TakeItemAction if (remaining == 4): inventoryAction(record, at, TakeItem);
			case SignalAction if (remaining == 2):
				final signal = ScenarioTokenGrammar.contentId(record.tokens[at + 1]);
				signal == null ? null : EmitSignal(signal);
			case EffectAction if (remaining == 2 || (remaining == 4 && ScenarioTokenGrammar.isBare(record.tokens[at + 2], "at"))):
				effectAction(record, at);
			case ScheduleAction if (remaining >= 4): scheduleAction(record, at);
			case CallAction if (remaining >= 2): callAction(record, at);
			case _:
				null;
		}
	}

	static function setFlagAction(record:ScenarioLexRecord, at:Int):Null<FlowAction> {
		final id = ScenarioTokenGrammar.scenarioId(record.tokens[at + 1]);
		final value = ScenarioTokenGrammar.boolean(record.tokens[at + 2]);
		return id == null || value == null ? null : SetFlag(id, value);
	}

	static function counterAction(record:ScenarioLexRecord, at:Int, make:(ScenarioId, Int) -> FlowAction):Null<FlowAction> {
		final id = ScenarioTokenGrammar.scenarioId(record.tokens[at + 1]);
		final value = ScenarioTokenGrammar.integer(record.tokens[at + 2]);
		return id == null || value == null ? null : make(id, value);
	}

	static function stateAction(record:ScenarioLexRecord, at:Int, make:(ScenarioId, ContentId) -> FlowAction):Null<FlowAction> {
		final id = ScenarioTokenGrammar.scenarioId(record.tokens[at + 1]);
		final value = ScenarioTokenGrammar.contentId(record.tokens[at + 2]);
		return id == null || value == null ? null : make(id, value);
	}

	static function objectiveAction(record:ScenarioLexRecord, at:Int):Null<FlowAction> {
		final id = ScenarioTokenGrammar.scenarioId(record.tokens[at + 1]);
		final value = ScenarioTokenGrammar.objectiveState(record.tokens[at + 2]);
		return id == null || value == null ? null : SetObjective(id, value);
	}

	static function inventoryAction(record:ScenarioLexRecord, at:Int, make:(ScenarioId, ContentId, Int) -> FlowAction):Null<FlowAction> {
		final owner = ScenarioTokenGrammar.scenarioId(record.tokens[at + 1]);
		final item = ScenarioTokenGrammar.contentId(record.tokens[at + 2]);
		final quantity = ScenarioTokenGrammar.integer(record.tokens[at + 3]);
		return owner == null || item == null || quantity == null ? null : make(owner, item, quantity);
	}

	static function effectAction(record:ScenarioLexRecord, at:Int):Null<FlowAction> {
		final effect = ScenarioTokenGrammar.contentId(record.tokens[at + 1]);
		final hasTarget = record.tokens.length - at == 4;
		final target = hasTarget ? ScenarioTokenGrammar.scenarioId(record.tokens[at + 3]) : null;
		return effect == null || (hasTarget && target == null) ? null : PlayEffect(effect, target);
	}

	static function scheduleAction(record:ScenarioLexRecord, at:Int):Null<FlowAction> {
		final timer = ScenarioTokenGrammar.scenarioId(record.tokens[at + 1]);
		final ticks = ScenarioTokenGrammar.integer(record.tokens[at + 2]);
		final sequence = ScenarioTokenGrammar.scenarioId(record.tokens[at + 3]);
		final parsedArguments = arguments(record, at + 4);
		return timer == null
			|| ticks == null
			|| sequence == null
			|| parsedArguments == null ? null : Schedule(timer, ticks, sequence, parsedArguments);
	}

	static function callAction(record:ScenarioLexRecord, at:Int):Null<FlowAction> {
		final sequence = ScenarioTokenGrammar.scenarioId(record.tokens[at + 1]);
		final parsedArguments = arguments(record, at + 2);
		return sequence == null || parsedArguments == null ? null : CallSequence(sequence, parsedArguments);
	}

	public static function flowValue(record:ScenarioLexRecord, at:Int):Null<FlowValue> {
		if (at + 1 >= record.tokens.length)
			return null;
		return switch ScenarioTokenGrammar.bareText(record.tokens[at]) {
			case "flag":
				final value = ScenarioTokenGrammar.boolean(record.tokens[at + 1]);
				value == null ? null : Flag(value);
			case "counter":
				final value = ScenarioTokenGrammar.integer(record.tokens[at + 1]);
				value == null ? null : Counter(value);
			case "state":
				final value = ScenarioTokenGrammar.contentId(record.tokens[at + 1]);
				value == null ? null : State(value);
			case _:
				null;
		}
	}

	static function arguments(record:ScenarioLexRecord, at:Int):Null<Array<FlowArgument>> {
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

	static function argument(record:ScenarioLexRecord, at:Int):Null<ParsedFlowArgument> {
		if (at >= record.tokens.length)
			return null;
		return switch ScenarioTokenGrammar.bareText(record.tokens[at]) {
			case "value":
				final value = flowValue(record, at + 1);
				value == null ? null : {value: Value(value), next: at + 3};
			case "variable" if (at + 1 < record.tokens.length):
				final id = ScenarioTokenGrammar.scenarioId(record.tokens[at + 1]);
				id == null ? null : {value: Variable(id), next: at + 2};
			case _:
				null;
		}
	}

	static function idEvent(token:ScenarioLexToken, make:ScenarioId->FlowEvent):Null<FlowEvent> {
		final id = ScenarioTokenGrammar.scenarioId(token);
		return id == null ? null : make(id);
	}

	static function actionId(token:ScenarioLexToken, make:ScenarioId->FlowAction):Null<FlowAction> {
		final id = ScenarioTokenGrammar.scenarioId(token);
		return id == null ? null : make(id);
	}
}

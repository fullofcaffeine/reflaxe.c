package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowValueKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioCoordinate;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;

/**
	Checks CaxeFlow variables, references, actions, and the sequence call graph.

	This class is used only by `ScenarioValidator`. `@:noCompletion` keeps the
	implementation helper out of editor suggestions; it does not change Haxe
	visibility, runtime behavior, or type safety.
**/
@:noCompletion
final class CaxeFlowValidator {
	final context:ScenarioValidationContext;

	public function new(context:ScenarioValidationContext)
		this.context = context;

	public function validate():Void {
		final flow = context.scenario.flow;
		if (flow.rules.length > ScenarioLimits.MAX_RULES)
			context.addAtCoordinate(LimitExceeded(Rules, ScenarioLimits.MAX_RULES),
				context.coordinateForIdentity(flow.rules[flow.rules.length - 1].id, RuleIdentity));
		for (variable in flow.variables) {
			final coordinate = context.coordinateForIdentity(variable.id, VariableIdentity);
			validateFlowValue(variable.initial, coordinate);
			switch variable.scope {
				case Local(sequence) if (!context.hasSequence(sequence)):
					context.addAtCoordinate(UnresolvedReference(sequence), coordinate);
				case _:
			}
		}
		for (sequence in flow.sequences) {
			for (parameter in sequence.parameters) {
				final parameterCoordinate = context.coordinateForIdentity(parameter.id, SequenceParameterIdentity(sequence.id));
				validateFlowValue(parameter.initial, parameterCoordinate);
			}
			for (actionIndex in 0...sequence.actions.length)
				validateAction(sequence.id, context.coordinateForSequenceAction(sequence.id, actionIndex), sequence.actions[actionIndex], false);
		}
		validateSequenceGraph();
		final ruleIds:Map<String, Bool> = [];
		for (rule in flow.rules) {
			final coordinate = context.coordinateForIdentity(rule.id, RuleIdentity);
			if (ruleIds.exists(rule.id.text()))
				context.addAtCoordinate(DuplicateId(rule.id), coordinate);
			ruleIds.set(rule.id.text(), true);
			if (consumeActionBudget(rule.actions, ScenarioLimits.MAX_ACTIONS_PER_RULE) < 0)
				context.addAtCoordinate(LimitExceeded(RuleActions, ScenarioLimits.MAX_ACTIONS_PER_RULE), coordinate);
			switch rule.repeat {
				case Cooldown(ticks) if (ticks <= 0):
					context.addAtCoordinate(InvalidRule(rule.id), coordinate);
				case _:
			}
			validateEvent(rule.id, context.coordinateForRuleEvent(rule.id), rule.event);
			validatePredicate(rule.id, context.coordinateForRulePredicate(rule.id), rule.predicate, 1);
			for (actionIndex in 0...rule.actions.length)
				validateAction(rule.id, context.coordinateForRuleAction(rule.id, actionIndex), rule.actions[actionIndex], false);
		}
	}

	function validateEvent(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowEvent):Void {
		switch value {
			case EnterZone(id), LeaveZone(id):
				if (!context.hasZone(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case Interact(id):
				if (!context.hasObject(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case EntityDefeated(id):
				if (!context.hasEntity(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case BlockChanged(zone, block):
				if (!context.hasZone(zone))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				if (!context.registry.hasBlock(block))
					context.addAtCoordinate(UnresolvedContent(block), coordinate);
			case UseItem(item):
				if (!context.registry.hasItem(item))
					context.addAtCoordinate(UnresolvedContent(item), coordinate);
			case SignalReceived(signal):
				if (!context.registry.hasSignal(signal))
					context.addAtCoordinate(UnresolvedContent(signal), coordinate);
			case TimerExpired(_):
			case ObjectiveChanged(id):
				if (!context.hasObjective(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case StateChanged(id):
				if (!context.hasVariable(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validatePredicate(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowPredicate, depth:Int):Void {
		if (depth > ScenarioLimits.MAX_PREDICATE_DEPTH) {
			context.addAtCoordinate(InvalidRule(owner), coordinate);
			return;
		}
		switch value {
			case Always, ModeIs(_):
			case All(children), AnyOf(children):
				for (child in children)
					validatePredicate(owner, coordinate, child, depth + 1);
			case Not(child):
				validatePredicate(owner, coordinate, child, depth + 1);
			case FlagIs(id, _):
				requireVariable(owner, coordinate, id, FlagValue);
			case CounterCompare(id, _, _):
				requireVariable(owner, coordinate, id, CounterValue);
			case StateIs(id, state):
				requireVariable(owner, coordinate, id, StateValue);
				if (!context.registry.hasState(state))
					context.addAtCoordinate(UnresolvedContent(state), coordinate);
			case ObjectStateIs(id, state):
				if (!context.hasObject(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				if (!context.registry.hasState(state))
					context.addAtCoordinate(UnresolvedContent(state), coordinate);
			case InventoryHas(id, item, _, quantity):
				if (!context.hasObject(id) || quantity < 0)
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				if (!context.registry.hasItem(item))
					context.addAtCoordinate(UnresolvedContent(item), coordinate);
			case ObjectiveIs(id, _):
				if (!context.hasObjective(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case NearObject(actor, objectId, maximum):
				if (!context.hasObject(actor) || !context.hasObject(objectId) || maximum < 0)
					context.addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validateAction(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowAction, insideChoice:Bool):Void {
		switch value {
			case ShowDialogue(id):
				if (!context.hasDialogue(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case AddJournal(id):
				if (!context.hasJournal(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case SetFlag(id, _):
				requireVariable(owner, coordinate, id, FlagValue);
			case SetCounter(id, _), AddCounter(id, _):
				requireVariable(owner, coordinate, id, CounterValue);
			case SetState(id, state):
				requireVariable(owner, coordinate, id, StateValue);
				if (!context.registry.hasState(state))
					context.addAtCoordinate(UnresolvedContent(state), coordinate);
			case GiveItem(id, item, quantity), TakeItem(id, item, quantity):
				if (!context.hasObject(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				if (!context.registry.hasItem(item))
					context.addAtCoordinate(UnresolvedContent(item), coordinate);
				else if (quantity <= 0 || quantity > context.registry.maximumItemQuantity(item))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case Spawn(id), Despawn(id):
				if (!context.hasObject(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case SetObjectState(id, state):
				if (!context.hasObject(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				if (!context.registry.hasState(state))
					context.addAtCoordinate(UnresolvedContent(state), coordinate);
			case SetCheckpoint(id):
				if (!context.hasCheckpoint(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case SetObjective(id, _):
				if (!context.hasObjective(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case PlayEffect(effect, target):
				if (!context.registry.hasEffect(effect))
					context.addAtCoordinate(UnresolvedContent(effect), coordinate);
				if (target != null && !context.hasObject(target))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case EmitSignal(signal):
				if (!context.registry.hasSignal(signal))
					context.addAtCoordinate(UnresolvedContent(signal), coordinate);
			case Schedule(_, ticks, sequence, arguments):
				if (ticks <= 0)
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				for (argument in arguments)
					validateArgument(owner, coordinate, argument);
				validateSequenceCall(owner, coordinate, sequence, arguments);
			case CallSequence(sequence, arguments):
				for (argument in arguments)
					validateArgument(owner, coordinate, argument);
				validateSequenceCall(owner, coordinate, sequence, arguments);
			case ChooseSeeded(seed, choices):
				if (insideChoice) {
					context.addAtCoordinate(InvalidRule(owner), coordinate);
					return;
				}
				requireVariable(owner, coordinate, seed, CounterValue);
				if (choices.length == 0)
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				for (choice in choices) {
					if (choice.weight <= 0)
						context.addAtCoordinate(InvalidRule(owner), coordinate);
					for (entry in choice.actions)
						validateAction(owner, coordinate, entry, true);
				}
		}
	}

	function validateSequenceCall(owner:ScenarioId, coordinate:ScenarioCoordinate, id:ScenarioId, arguments:Array<FlowArgument>):Void {
		final sequence = context.sequence(id);
		if (sequence == null || sequence.parameters.length != arguments.length) {
			context.addAtCoordinate(InvalidRule(owner), coordinate);
			return;
		}
		for (index in 0...arguments.length) {
			final expected = ScenarioValidationContext.flowValueKind(sequence.parameters[index].initial);
			final actual = switch arguments[index] {
				case Value(value): ScenarioValidationContext.flowValueKind(value);
				case Variable(variable): context.variableKind(variable);
			}
			if (actual != expected)
				context.addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validateSequenceGraph():Void {
		final visiting:Map<String, Bool> = [];
		final depths:Map<String, Int> = [];
		final reportedCycles:Map<String, Bool> = [];
		var reportedDepth = false;
		for (sequence in context.scenario.flow.sequences) {
			final depth = sequenceDepth(sequence, visiting, depths, reportedCycles);
			if (!reportedDepth && depth > ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH) {
				context.addAtCoordinate(LimitExceeded(SequenceCallDepth, ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH),
					context.coordinateForIdentity(sequence.id, SequenceIdentity));
				reportedDepth = true;
			}
		}
	}

	function sequenceDepth(sequence:FlowSequence, visiting:Map<String, Bool>, depths:Map<String, Int>, reportedCycles:Map<String, Bool>):Int {
		final key = sequence.id.text();
		if (depths.exists(key))
			return depths.get(key);
		if (visiting.exists(key)) {
			if (!reportedCycles.exists(key)) {
				context.addAtCoordinate(RuleCycle(sequence.id), context.coordinateForIdentity(sequence.id, SequenceIdentity));
				reportedCycles.set(key, true);
			}
			return 0;
		}
		visiting.set(key, true);
		var maximum = 1;
		for (action in sequence.actions)
			maximum = maximumSequenceDepth(maximum, action, visiting, depths, reportedCycles);
		visiting.remove(key);
		depths.set(key, maximum);
		return maximum;
	}

	function maximumSequenceDepth(current:Int, action:FlowAction, visiting:Map<String, Bool>, depths:Map<String, Int>, reportedCycles:Map<String, Bool>):Int {
		var maximum = current;
		switch action {
			case CallSequence(id, _):
				final target = context.sequence(id);
				if (target != null)
					maximum = max(maximum, 1 + sequenceDepth(target, visiting, depths, reportedCycles));
			case ChooseSeeded(_, choices):
				for (choice in choices)
					for (entry in choice.actions)
						maximum = maximumSequenceDepth(maximum, entry, visiting, depths, reportedCycles);
			case _:
		}
		return maximum;
	}

	function validateArgument(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowArgument):Void {
		switch value {
			case Value(value):
				validateFlowValue(value, coordinate);
			case Variable(id):
				if (!context.hasVariable(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validateFlowValue(value:FlowValue, coordinate:ScenarioCoordinate):Void
		switch value {
			case State(state) if (!context.registry.hasState(state)):
				context.addAtCoordinate(UnresolvedContent(state), coordinate);
			case _:
		}

	function requireVariable(owner:ScenarioId, coordinate:ScenarioCoordinate, id:ScenarioId, kind:FlowValueKind):Void
		if (!context.hasVariable(id) || context.variableKind(id) != kind)
			context.addAtCoordinate(InvalidRule(owner), coordinate);

	function consumeActionBudget(actions:Array<FlowAction>, remaining:Int):Int {
		var available = remaining;
		for (action in actions) {
			if (available == 0)
				return -1;
			available--;
			switch action {
				case ChooseSeeded(_, choices):
					for (choice in choices) {
						available = consumeActionBudget(choice.actions, available);
						if (available < 0)
							return -1;
					}
				case _:
			}
		}
		return available;
	}

	function max(left:Int, right:Int):Int
		return left > right ? left : right;
}

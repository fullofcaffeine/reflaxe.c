package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowValueKind;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionConsumer;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionAllowed;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionDescriptor;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionMatchesDescriptor;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventSupportsActor;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventSupportsSwept;
import caxecraft.scenario.CaxeFlowEventRegistry.externalFlowEventCapacity;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventSourcesMatch;
import caxecraft.scenario.CaxeFlowEventOwnership.flowOwnsCampaignExit;
import caxecraft.scenario.CaxeFlowEventOwnership.flowOwnsTimer;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioCoordinate;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

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
		validateSpatialEventCapacity();
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
				if (context.hasVariable(parameter.id))
					context.addAtCoordinate(DuplicateId(parameter.id), parameterCoordinate);
			}
			for (actionIndex in 0...sequence.actions.length)
				validateAction(sequence.id, context.coordinateForSequenceAction(sequence.id, actionIndex), sequence.actions[actionIndex], false, sequence.id);
		}
		validateSequenceGraph();
		validateDeferredEventGraph();
		final ruleIds:Map<String, Bool> = [];
		for (rule in flow.rules) {
			final coordinate = context.coordinateForIdentity(rule.id, RuleIdentity);
			if (ruleIds.exists(rule.id.text()))
				context.addAtCoordinate(DuplicateId(rule.id), coordinate);
			ruleIds.set(rule.id.text(), true);
			if (consumeActionBudget(rule.actions, ScenarioLimits.MAX_ACTIONS_PER_RULE) < 0)
				context.addAtCoordinate(LimitExceeded(RuleActions, ScenarioLimits.MAX_ACTIONS_PER_RULE), coordinate);
			switch rule.repeat {
				case Cooldown(ticks) | CooldownPerActor(ticks) if (ticks <= 0):
					context.addAtCoordinate(InvalidRule(rule.id), coordinate);
				case _:
			}
			switch rule.repeat {
				case OncePerActor | CooldownPerActor(_) if (!flowEventSupportsActor(rule.event)):
					context.addAtCoordinate(InvalidRule(rule.id), coordinate);
				case _:
			}
			validateEvent(rule.id, context.coordinateForRuleEvent(rule.id), rule.event);
			validatePredicate(rule.id, context.coordinateForRulePredicate(rule.id), rule.event, rule.predicate, 1);
			for (actionIndex in 0...rule.actions.length)
				validateAction(rule.id, context.coordinateForRuleAction(rule.id, actionIndex), rule.actions[actionIndex], false, null);
		}
	}

	/**
		Reserve enough per-tick input space for every simultaneous zone crossing.

		One outside-to-outside swept move emits enter and leave. Reserving that
		worst case prevents queue pressure from joining separate movement segments
		and inventing a crossing on a later tick.
	**/
	function validateSpatialEventCapacity():Void {
		var actorCount = 0;
		final zones:Array<ScenarioId> = [];
		for (object in context.scenario.objects)
			switch object.placement {
				case PlayerSpawn(_) | Entity(_, _) | Npc(_, _, _):
					actorCount++;
				case TriggerZone(_):
					zones.push(object.id);
				case _:
			}
		if (externalFlowEventCapacity(zones.length, actorCount) > 0)
			return;
		final owner = zones[zones.length - 1];
		context.addAtCoordinate(EventBudgetExhausted(ScenarioLimits.MAX_EVENTS_PER_TICK), context.coordinateForIdentity(owner, ObjectIdentity));
	}

	function validateEvent(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowEvent):Void {
		switch value {
			case EnterZone(id), LeaveZone(id):
				if (!context.hasZone(id))
					invalidReference(owner, coordinate, "event.zone", id, "trigger-zone");
			case Interact(id):
				if (!context.hasObject(id))
					invalidReference(owner, coordinate, "event.object", id, "object");
			case EntityDefeated(id):
				if (!context.hasEntity(id))
					invalidReference(owner, coordinate, "event.entity", id, "entity");
			case BlockChanged(zone, block):
				if (!context.hasZone(zone))
					invalidReference(owner, coordinate, "event.zone", zone, "trigger-zone");
				if (!context.registry.hasBlock(block))
					context.addAtCoordinate(UnresolvedContent(block), coordinate);
			case UseItem(item), ItemCollected(item):
				if (!context.registry.hasItem(item))
					context.addAtCoordinate(UnresolvedContent(item), coordinate);
			case SignalReceived(signal):
				if (!context.registry.hasSignal(signal))
					context.addAtCoordinate(UnresolvedContent(signal), coordinate);
			case TimerExpired(id):
				if (!flowOwnsTimer(context.scenario.flow, id))
					invalidReference(owner, coordinate, "event.timer", id, "scheduled-timer");
			case ObjectiveChanged(id):
				if (!context.hasObjective(id))
					invalidReference(owner, coordinate, "event.objective", id, "objective");
			case StateChanged(id):
				if (!context.hasPersistentVariable(id))
					invalidReference(owner, coordinate, "event.variable", id, "persistent-variable");
			case LevelEntered(id):
				if (id.text() != context.scenario.id.text())
					invalidReference(owner, coordinate, "event.level", id, "current-level");
			case CampaignExitRequested(id):
				if (!flowOwnsCampaignExit(context.scenario.flow, id))
					invalidReference(owner, coordinate, "event.exit", id, "campaign-exit-action");
		}
	}

	function validatePredicate(owner:ScenarioId, coordinate:ScenarioCoordinate, event:FlowEvent, value:FlowPredicate, depth:Int):Void {
		if (depth > ScenarioLimits.MAX_PREDICATE_DEPTH) {
			context.addAtCoordinate(InvalidRule(owner), coordinate);
			return;
		}
		switch value {
			case Always, ModeIs(_):
			case All(children), AnyOf(children):
				for (child in children)
					validatePredicate(owner, coordinate, event, child, depth + 1);
			case Not(child):
				validatePredicate(owner, coordinate, event, child, depth + 1);
			case FlagIs(id, _):
				requireVariable(owner, coordinate, id, FlagValue, null);
			case CounterCompare(id, _, _):
				requireVariable(owner, coordinate, id, CounterValue, null);
			case StateIs(id, state):
				requireVariable(owner, coordinate, id, StateValue, null);
				if (!context.registry.hasState(state))
					context.addAtCoordinate(UnresolvedContent(state), coordinate);
			case ObjectStateIs(id, state):
				if (!context.hasStatefulObject(id))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				if (!context.registry.hasState(state))
					context.addAtCoordinate(UnresolvedContent(state), coordinate);
				else if (!context.statefulObjectHasState(id, state))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
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
			case EventActorIs(actor):
				if (!flowEventSupportsActor(event) || !context.hasObject(actor))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case EventSweptIs(_):
				if (!flowEventSupportsSwept(event))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validateAction(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowAction, insideChoice:Bool, sequenceOwner:Null<ScenarioId>):Void {
		final descriptor = flowActionDescriptor(value);
		if (!flowActionAllowed(descriptor, FlowActionConsumer.CaxeFlowDocument) || !flowActionMatchesDescriptor(value, descriptor)) {
			context.addAtCoordinate(InvalidRule(owner), coordinate);
			return;
		}
		switch value {
			case ShowDialogue(id):
				if (!context.hasDialogue(id))
					invalidReference(owner, coordinate, "action.dialogue", id, "dialogue");
			case AddJournal(id):
				if (!context.hasJournal(id))
					invalidReference(owner, coordinate, "action.journal", id, "journal-entry");
			case SetFlag(id, _):
				requireVariable(owner, coordinate, id, FlagValue, sequenceOwner);
			case SetCounter(id, _), AddCounter(id, _):
				requireVariable(owner, coordinate, id, CounterValue, sequenceOwner);
			case SetState(id, state):
				requireVariable(owner, coordinate, id, StateValue, sequenceOwner);
				if (!context.registry.hasState(state))
					context.addAtCoordinate(UnresolvedContent(state), coordinate);
			case GiveItem(id, item, quantity), TakeItem(id, item, quantity):
				if (!context.hasObject(id))
					invalidReference(owner, coordinate, "action.inventory-owner", id, "object");
				if (!context.registry.hasItem(item))
					context.addAtCoordinate(UnresolvedContent(item), coordinate);
				else if (quantity <= 0 || quantity > context.registry.maximumItemQuantity(item))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case Spawn(id), Despawn(id):
				if (!context.hasObject(id))
					invalidReference(owner, coordinate, "action.object", id, "object");
			case SetObjectState(id, state):
				if (!context.hasStatefulObject(id))
					invalidReference(owner, coordinate, "action.stateful-object", id, "stateful-object");
				if (!context.registry.hasState(state))
					context.addAtCoordinate(UnresolvedContent(state), coordinate);
				else if (!context.statefulObjectHasState(id, state))
					context.addAtCoordinate(InvalidRule(owner), coordinate);
			case SetCheckpoint(id):
				if (!context.hasCheckpoint(id))
					invalidReference(owner, coordinate, "action.checkpoint", id, "checkpoint");
			case SetObjective(id, _):
				if (!context.hasObjective(id))
					invalidReference(owner, coordinate, "action.objective", id, "objective");
			case PlayEffect(effect, target):
				if (!context.registry.hasEffect(effect))
					context.addAtCoordinate(UnresolvedContent(effect), coordinate);
				if (target != null && !context.hasObject(target))
					invalidReference(owner, coordinate, "action.effect-target", target, "object");
			case RequestCampaignExit(_):
				// Campaign edges are outside an independently valid CaxeMap. The
				// campaign loader resolves this stable request after map validation.
			case EmitSignal(signal):
				if (!context.registry.hasSignal(signal))
					context.addAtCoordinate(UnresolvedContent(signal), coordinate);
			case Schedule(_, ticks, sequence, arguments):
				if (ticks <= 0)
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				for (argument in arguments)
					validateArgument(owner, coordinate, argument, sequenceOwner);
				validateSequenceCall(owner, coordinate, sequence, arguments, sequenceOwner);
			case CallSequence(sequence, arguments):
				for (argument in arguments)
					validateArgument(owner, coordinate, argument, sequenceOwner);
				validateSequenceCall(owner, coordinate, sequence, arguments, sequenceOwner);
			case ChooseSeeded(seed, choices):
				if (insideChoice) {
					context.addAtCoordinate(InvalidRule(owner), coordinate);
					return;
				}
				requireVariable(owner, coordinate, seed, CounterValue, sequenceOwner);
				if (choices.length == 0)
					context.addAtCoordinate(InvalidRule(owner), coordinate);
				var totalWeight = 0;
				for (choice in choices) {
					if (choice.weight <= 0 || totalWeight > 2147483647 - choice.weight)
						context.addAtCoordinate(InvalidRule(owner), coordinate);
					else
						totalWeight += choice.weight;
					for (entry in choice.actions)
						validateAction(owner, coordinate, entry, true, sequenceOwner);
				}
		}
	}

	function validateSequenceCall(owner:ScenarioId, coordinate:ScenarioCoordinate, id:ScenarioId, arguments:Array<FlowArgument>,
			sequenceOwner:Null<ScenarioId>):Void {
		final sequence = context.sequence(id);
		if (sequence == null || sequence.parameters.length != arguments.length) {
			context.addAtCoordinate(InvalidRule(owner), coordinate);
			return;
		}
		for (index in 0...arguments.length) {
			final expected = ScenarioValidationContext.flowValueKind(sequence.parameters[index].initial);
			final actual = switch arguments[index] {
				case Value(value): ScenarioValidationContext.flowValueKind(value);
				case Variable(variable): context.variableKindInScope(variable, sequenceOwner);
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

	/**
		Reject unconditional repeat paths that must feed deferred work forever.

		The graph deliberately excludes guarded, once, cooldown, and idempotent set
		actions. Those paths can terminate from state and must remain authorable. An
		always/repeat signal, exit, timer, or non-zero counter feedback loop has no
		such stopping condition and is malformed before runtime budgets are relevant.
	**/
	function validateDeferredEventGraph():Void {
		final rules = context.scenario.flow.rules;
		final edges:Array<Array<Int>> = [for (_ in rules) []];
		for (sourceIndex in 0...rules.length) {
			final source = rules[sourceIndex];
			if (!isUnconditionalRepeat(source))
				continue;
			final emitted:Array<FlowEvent> = [];
			collectGuaranteedDeferredEvents(source.actions, emitted, []);
			for (event in emitted)
				for (targetIndex in 0...rules.length)
					if (isUnconditionalRepeat(rules[targetIndex]) && flowEventSourcesMatch(rules[targetIndex].event, event))
						edges[sourceIndex].push(targetIndex);
		}
		final state:Array<Int> = [for (_ in rules) 0];
		for (index in 0...rules.length)
			if (state[index] == 0)
				visitDeferredRule(index, edges, state, rules);
	}

	/** Return true only for a rule whose next matching event always runs again. */
	static function isUnconditionalRepeat(rule:FlowRule):Bool
		return rule.repeat == Repeat && switch rule.predicate {
			case Always: true;
			case _: false;
		};

	/** Collect only actions that necessarily create a later semantic event. */
	function collectGuaranteedDeferredEvents(actions:Array<FlowAction>, result:Array<FlowEvent>, visiting:Map<String, Bool>):Void {
		for (action in actions)
			switch action {
				case AddCounter(variable, delta) if (delta != 0):
					result.push(StateChanged(variable));
				case EmitSignal(signal):
					result.push(SignalReceived(signal));
				case RequestCampaignExit(exit):
					result.push(CampaignExitRequested(exit));
				case Schedule(timer, _, sequence, _):
					result.push(TimerExpired(timer));
					collectSequenceDeferredEvents(sequence, result, visiting);
				case CallSequence(sequence, _):
					collectSequenceDeferredEvents(sequence, result, visiting);
				case ChooseSeeded(_, choices):
					for (choice in choices)
						collectGuaranteedDeferredEvents(choice.actions, result, visiting);
				case _:
			}
	}

	/** Follow one validated sequence once while collecting its deferred outputs. */
	function collectSequenceDeferredEvents(id:ScenarioId, result:Array<FlowEvent>, visiting:Map<String, Bool>):Void {
		if (visiting.exists(id.text()))
			return;
		final sequence = context.sequence(id);
		if (sequence == null)
			return;
		visiting.set(id.text(), true);
		collectGuaranteedDeferredEvents(sequence.actions, result, visiting);
		visiting.remove(id.text());
	}

	/** Depth-first search with one deterministic diagnostic per discovered cycle. */
	function visitDeferredRule(index:Int, edges:Array<Array<Int>>, state:Array<Int>, rules:Array<FlowRule>):Void {
		state[index] = 1;
		for (target in edges[index]) {
			if (state[target] == 0)
				visitDeferredRule(target, edges, state, rules);
			else if (state[target] == 1)
				context.addAtCoordinate(RuleCycle(rules[target].id), context.coordinateForIdentity(rules[target].id, RuleIdentity));
		}
		state[index] = 2;
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

	function validateArgument(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowArgument, sequenceOwner:Null<ScenarioId>):Void {
		switch value {
			case Value(value):
				validateFlowValue(value, coordinate);
			case Variable(id):
				if (context.variableKindInScope(id, sequenceOwner) == null)
					context.addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validateFlowValue(value:FlowValue, coordinate:ScenarioCoordinate):Void
		switch value {
			case State(state) if (!context.registry.hasState(state)):
				context.addAtCoordinate(UnresolvedContent(state), coordinate);
			case _:
		}

	function requireVariable(owner:ScenarioId, coordinate:ScenarioCoordinate, id:ScenarioId, kind:FlowValueKind, sequenceOwner:Null<ScenarioId>):Void
		if (context.variableKindInScope(id, sequenceOwner) != kind)
			context.addAtCoordinate(InvalidRule(owner), coordinate);

	/** Report the exact field role instead of collapsing a wrong-kind ID to prose. */
	function invalidReference(owner:ScenarioId, coordinate:ScenarioCoordinate, field:String, reference:ScenarioId, expected:String):Void
		context.addAtCoordinate(InvalidRuleReference(owner, field, reference, expected), coordinate);

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

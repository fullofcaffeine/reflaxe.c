package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowComparison;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlowRuntime.FlowRuntimeDiagnostic;
import haxe.io.Bytes;
import haxe.Int64;

private final class CaxeFlowRuleState {
	public final id:ScenarioId;
	public var hasFired:Bool;
	public var lastTick:Int64;

	public function new(id:ScenarioId) {
		this.id = id;
		hasFired = false;
		lastTick = 0;
	}
}

typedef CaxeFlowAdmission = {
	final rules:Array<FlowRule>;
	final diagnostic:Null<FlowRuntimeDiagnostic>;
}

/**
	Selects rules against the stable state at the start of one fixed tick.

	This helper owns event matching, predicate evaluation, priority ordering, and
	once/cooldown history. It never executes an action. Keeping that boundary
	separate makes it impossible for an earlier rule action to change a later
	rule's predicate during the same admission pass.
**/
@:noCompletion
final class CaxeFlowRulePlanner {
	final scenario:Scenario;
	final state:CaxeFlowState;
	final orderedRules:Array<FlowRule>;
	final ruleStates:Array<CaxeFlowRuleState> = [];
	var predicateCount:Int = 0;
	var diagnostic:Null<FlowRuntimeDiagnostic> = null;

	public function new(scenario:Scenario, state:CaxeFlowState) {
		this.scenario = scenario;
		this.state = state;
		orderedRules = scenario.flow.rules.copy();
		orderedRules.sort(compareRules);
		for (rule in scenario.flow.rules)
			ruleStates.push(new CaxeFlowRuleState(rule.id));
	}

	public function admit(events:Array<FlowEvent>, tick:Int64):CaxeFlowAdmission {
		predicateCount = 0;
		diagnostic = null;
		final admitted:Array<FlowRule> = [];
		final reserved:Array<ScenarioId> = [];
		for (rule in orderedRules) {
			if (!ruleCanFire(rule, tick) || containsId(reserved, rule.id))
				continue;
			for (event in events) {
				if (!eventsMatch(rule.event, event))
					continue;
				final matches = predicateMatches(rule.predicate, rule.id);
				if (diagnostic != null)
					return {rules: admitted, diagnostic: diagnostic};
				if (!matches)
					continue;
				if (admitted.length >= ScenarioLimits.MAX_RULE_EXECUTIONS_PER_TICK) {
					diagnostic = LimitExceeded(RuleExecutions, ScenarioLimits.MAX_RULE_EXECUTIONS_PER_TICK, rule.id);
					return {rules: admitted, diagnostic: diagnostic};
				}
				admitted.push(rule);
				switch rule.repeat {
					case Repeat:
					case Once | Cooldown(_):
						reserved.push(rule.id);
						break;
				}
			}
		}
		return {rules: admitted, diagnostic: null};
	}

	public function markFired(rule:FlowRule, tick:Int64):Void {
		final runtime = findRuleState(rule.id);
		if (runtime != null) {
			runtime.hasFired = true;
			runtime.lastTick = tick;
		}
	}

	function predicateMatches(predicate:FlowPredicate, owner:ScenarioId):Bool {
		if (predicateCount >= ScenarioLimits.MAX_PREDICATE_EVALUATIONS_PER_TICK) {
			diagnostic = LimitExceeded(PredicateEvaluations, ScenarioLimits.MAX_PREDICATE_EVALUATIONS_PER_TICK, owner);
			return false;
		}
		predicateCount++;
		return switch predicate {
			case Always: true;
			case All(children):
				var matches = true;
				for (child in children)
					if (!predicateMatches(child, owner)) {
						matches = false;
						break;
					}
				matches;
			case AnyOf(children):
				var matches = false;
				for (child in children)
					if (predicateMatches(child, owner)) {
						matches = true;
						break;
					}
				matches;
			case Not(child): !predicateMatches(child, owner);
			case FlagIs(variable, expected):
				switch state.variable(variable) {
					case Flag(value): value == expected;
					case _: false;
				}
			case CounterCompare(variable, comparison, expected):
				switch state.variable(variable) {
					case Counter(value): compareIntegers(value, comparison, expected);
					case _: false;
				}
			case StateIs(variable, expected):
				switch state.variable(variable) {
					case State(value): sameContent(value, expected);
					case _: false;
				}
			case ObjectStateIs(objectId, expected): final value = state.objectState(objectId); value != null && sameContent(value, expected);
			case InventoryHas(ownerId, itemType, comparison, quantity):
				compareIntegers(state.inventoryQuantity(ownerId, itemType), comparison, quantity);
			case ObjectiveIs(objective, expected): state.objectiveState(objective) == expected;
			case NearObject(actor, objectId, maximum): state.objectsAreNear(actor, objectId, maximum);
			case ModeIs(mode): scenario.mode == mode;
		}
	}

	function ruleCanFire(rule:FlowRule, tick:Int64):Bool {
		final runtime = findRuleState(rule.id);
		if (runtime == null)
			return false;
		return switch rule.repeat {
			case Once: !runtime.hasFired;
			case Repeat: true;
			case Cooldown(ticks): !runtime.hasFired || CaxeFlowClock.cooldownHasElapsed(tick, runtime.lastTick, ticks);
		}
	}

	function findRuleState(id:ScenarioId):Null<CaxeFlowRuleState> {
		for (runtime in ruleStates)
			if (sameId(runtime.id, id))
				return runtime;
		return null;
	}

	static function compareRules(left:FlowRule, right:FlowRule):Int {
		if (left.priority != right.priority)
			return left.priority < right.priority ? -1 : 1;
		return compareUtf8(left.id.text(), right.id.text());
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final shared = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...shared) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}

	static function eventsMatch(expected:FlowEvent, actual:FlowEvent):Bool
		return switch [expected, actual] {
			case [EnterZone(left), EnterZone(right)] | [LeaveZone(left), LeaveZone(right)] | [Interact(left), Interact(right)] |
				[EntityDefeated(left), EntityDefeated(right)] | [TimerExpired(left), TimerExpired(right)] |
				[ObjectiveChanged(left), ObjectiveChanged(right)] | [StateChanged(left), StateChanged(right)]:
				sameId(left, right);
			case [BlockChanged(leftZone, leftBlock), BlockChanged(rightZone, rightBlock)]: sameId(leftZone, rightZone) && sameContent(leftBlock, rightBlock);
			case [UseItem(left), UseItem(right)] | [SignalReceived(left), SignalReceived(right)]:
				sameContent(left, right);
			case _: false;
		};

	static function compareIntegers(left:Int, comparison:FlowComparison, right:Int):Bool
		return switch comparison {
			case Equal: left == right;
			case NotEqual: left != right;
			case Less: left < right;
			case LessOrEqual: left <= right;
			case Greater: left > right;
			case GreaterOrEqual: left >= right;
		};

	static function containsId(values:Array<ScenarioId>, id:ScenarioId):Bool {
		for (value in values)
			if (sameId(value, id))
				return true;
		return false;
	}

	static inline function sameId(left:ScenarioId, right:ScenarioId):Bool
		return left.text() == right.text();

	static inline function sameContent(left:ContentId, right:ContentId):Bool
		return left.text() == right.text();
}

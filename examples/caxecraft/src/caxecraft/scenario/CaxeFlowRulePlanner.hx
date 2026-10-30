package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowComparison;
import caxecraft.scenario.CaxeFlow.FlowEventOccurrence;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventActor;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventId;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventSourcesMatch;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventWasSwept;
import caxecraft.scenario.CaxeFlowRuntime.FlowRuntimeDiagnostic;
import caxecraft.scenario.CaxeFlowRuntime.FlowTick;
import caxecraft.scenario.CaxeFlowRuntime.FlowTraceEntry;
import caxecraft.scenario.CaxeFlowSnapshot.FlowRuleHistoryScope;
import caxecraft.scenario.CaxeFlowSnapshot.FlowRuleHistorySnapshot;
import haxe.io.Bytes;

private final class CaxeFlowActorRuleState {
	public final actor:ScenarioId;
	public var hasFired:Bool;
	public var lastTick:FlowTick;

	public function new(actor:ScenarioId) {
		this.actor = actor;
		hasFired = false;
		lastTick = CaxeFlowClock.start();
	}
}

private final class CaxeFlowRuleState {
	public final id:ScenarioId;
	public var hasFired:Bool;
	public var lastTick:FlowTick;
	public final actors:Array<CaxeFlowActorRuleState> = [];

	public function new(id:ScenarioId) {
		this.id = id;
		hasFired = false;
		lastTick = CaxeFlowClock.start();
	}
}

/** One same-tick reservation, with actor scope kept explicit rather than null-coded. */
private final class CaxeFlowRuleReservation {
	public final rule:ScenarioId;
	public final perActor:Bool;
	public final actor:Null<ScenarioId>;

	public function new(rule:ScenarioId, perActor:Bool, actor:Null<ScenarioId>) {
		this.rule = rule;
		this.perActor = perActor;
		this.actor = actor;
	}
}

/** One rule paired with the exact occurrence admitted for its action context. */
@:noCompletion
final class CaxeFlowAdmittedRule {
	public final rule:FlowRule;
	public final occurrence:FlowEventOccurrence;

	public function new(rule:FlowRule, occurrence:FlowEventOccurrence) {
		this.rule = rule;
		this.occurrence = occurrence;
	}
}

typedef CaxeFlowAdmission = {
	final rules:Array<CaxeFlowAdmittedRule>;
	final trace:Array<FlowTraceEntry>;
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

	public function admit(events:Array<FlowEventOccurrence>, tick:FlowTick):CaxeFlowAdmission {
		predicateCount = 0;
		diagnostic = null;
		final admitted:Array<CaxeFlowAdmittedRule> = [];
		final trace:Array<FlowTraceEntry> = [];
		final reservations:Array<CaxeFlowRuleReservation> = [];
		for (rule in orderedRules) {
			for (occurrence in events) {
				if (!flowEventSourcesMatch(rule.event, occurrence.source))
					continue;
				final actor = flowEventActor(occurrence);
				if (!ruleCanFire(rule, actor, tick) || containsReservation(reservations, rule, actor))
					continue;
				final matches = predicateMatches(rule.predicate, rule.id, occurrence);
				trace.push(PredicateEvaluated(rule.id, flowEventId(occurrence.source), actor, matches));
				if (diagnostic != null)
					return {rules: admitted, trace: trace, diagnostic: diagnostic};
				if (!matches)
					continue;
				if (admitted.length >= ScenarioLimits.MAX_RULE_EXECUTIONS_PER_TICK) {
					diagnostic = LimitExceeded(RuleExecutions, ScenarioLimits.MAX_RULE_EXECUTIONS_PER_TICK, rule.id);
					return {rules: admitted, trace: trace, diagnostic: diagnostic};
				}
				admitted.push(new CaxeFlowAdmittedRule(rule, occurrence));
				switch rule.repeat {
					case Repeat:
					case Once | Cooldown(_):
						reservations.push(new CaxeFlowRuleReservation(rule.id, false, null));
						break;
					case FlowRepeatPolicy.OncePerActor | FlowRepeatPolicy.CooldownPerActor(_):
						reservations.push(new CaxeFlowRuleReservation(rule.id, true, actor));
				}
			}
		}
		return {rules: admitted, trace: trace, diagnostic: null};
	}

	public function markFired(rule:FlowRule, occurrence:FlowEventOccurrence, tick:FlowTick):Void {
		final runtime = findRuleState(rule.id);
		if (runtime == null)
			return;
		switch rule.repeat {
			case FlowRepeatPolicy.OncePerActor | FlowRepeatPolicy.CooldownPerActor(_):
				final actor = flowEventActor(occurrence);
				if (actor != null) {
					final actorState = actorRuleState(runtime, actor, true);
					if (actorState != null) {
						actorState.hasFired = true;
						actorState.lastTick = tick;
					}
				}
			case Once | Repeat | Cooldown(_):
				runtime.hasFired = true;
				runtime.lastTick = tick;
		}
	}

	/** Return global and actor-local once/cooldown history in canonical rule order. */
	public function snapshot():Array<FlowRuleHistorySnapshot> {
		final result:Array<FlowRuleHistorySnapshot> = [];
		for (runtime in ruleStates) {
			result.push({
				rule: runtime.id,
				scope: GlobalRuleHistory,
				hasFired: runtime.hasFired,
				lastTick: runtime.lastTick
			});
			for (actor in runtime.actors)
				result.push({
					rule: runtime.id,
					scope: ActorRuleHistory(actor.actor),
					hasFired: actor.hasFired,
					lastTick: actor.lastTick
				});
		}
		return result;
	}

	/** Restore history whose recorded firings do not exceed the saved tick. */
	public function restore(values:Array<FlowRuleHistorySnapshot>, maximumTick:FlowTick):Bool {
		if (!historyIsValid(values, maximumTick))
			return false;
		for (runtime in ruleStates) {
			runtime.actors.resize(0);
			for (value in values)
				if (sameId(value.rule, runtime.id))
					switch value.scope {
						case GlobalRuleHistory:
							runtime.hasFired = value.hasFired;
							runtime.lastTick = value.lastTick;
						case ActorRuleHistory(actor):
							final restored = new CaxeFlowActorRuleState(actor);
							restored.hasFired = value.hasFired;
							restored.lastTick = value.lastTick;
							runtime.actors.push(restored);
					}
		}
		return true;
	}

	function predicateMatches(predicate:FlowPredicate, owner:ScenarioId, occurrence:FlowEventOccurrence):Bool {
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
					if (!predicateMatches(child, owner, occurrence)) {
						matches = false;
						break;
					}
				matches;
			case AnyOf(children):
				var matches = false;
				for (child in children)
					if (predicateMatches(child, owner, occurrence)) {
						matches = true;
						break;
					}
				matches;
			case Not(child): !predicateMatches(child, owner, occurrence);
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
			case EventActorIs(expected): final actor = flowEventActor(occurrence); actor != null && sameId(actor, expected);
			case EventSweptIs(expected): flowEventWasSwept(occurrence) == expected;
		}
	}

	function ruleCanFire(rule:FlowRule, actor:Null<ScenarioId>, tick:FlowTick):Bool {
		final runtime = findRuleState(rule.id);
		if (runtime == null)
			return false;
		return switch rule.repeat {
			case Once: !runtime.hasFired;
			case Repeat: true;
			case Cooldown(ticks): !runtime.hasFired || CaxeFlowClock.cooldownHasElapsed(tick, runtime.lastTick, ticks);
			case FlowRepeatPolicy.OncePerActor:
				if (actor == null) false; else {
					final value = actorRuleState(runtime, actor, false);
					value == null || !value.hasFired
					;
				}
			case FlowRepeatPolicy.CooldownPerActor(ticks):
				if (actor == null) false; else {
					final value = actorRuleState(runtime, actor, false);
					value == null || !value.hasFired || CaxeFlowClock.cooldownHasElapsed(tick, value.lastTick, ticks)
					;
				}
		}
	}

	function actorRuleState(runtime:CaxeFlowRuleState, actor:ScenarioId, create:Bool):Null<CaxeFlowActorRuleState> {
		for (value in runtime.actors)
			if (sameId(value.actor, actor))
				return value;
		if (!create || runtime.actors.length >= ScenarioLimits.MAX_OBJECTS)
			return null;
		final value = new CaxeFlowActorRuleState(actor);
		runtime.actors.push(value);
		return value;
	}

	function findRuleState(id:ScenarioId):Null<CaxeFlowRuleState> {
		for (runtime in ruleStates)
			if (sameId(runtime.id, id))
				return runtime;
		return null;
	}

	/** Reject stale rules, duplicate scopes, impossible actors, and bad clocks. */
	function historyIsValid(values:Array<FlowRuleHistorySnapshot>, maximumTick:FlowTick):Bool {
		if (values.length < scenario.flow.rules.length || values.length > scenario.flow.rules.length * (scenario.objects.length + 1))
			return false;
		for (rule in scenario.flow.rules) {
			var globals = 0;
			for (value in values)
				if (sameId(value.rule, rule.id))
					switch value.scope {
						case GlobalRuleHistory:
							globals++;
							if (!CaxeFlowClock.isValid(value.lastTick) || !CaxeFlowClock.isDue(value.lastTick, maximumTick))
								return false;
						case ActorRuleHistory(actor):
							if (!actorHistoryAllowed(rule)
								|| !value.hasFired
								|| !CaxeFlowClock.isValid(value.lastTick)
								|| !CaxeFlowClock.isDue(value.lastTick, maximumTick)
								|| !scenarioHasObject(actor))
								return false;
					}
			if (globals != 1)
				return false;
		}
		for (index in 0...values.length) {
			if (findScenarioRule(values[index].rule) == null)
				return false;
			for (earlier in 0...index)
				if (sameId(values[earlier].rule, values[index].rule) && sameHistoryScope(values[earlier].scope, values[index].scope))
					return false;
		}
		return true;
	}

	/** Return a rule from this exact scenario, or null for stale save data. */
	function findScenarioRule(id:ScenarioId):Null<FlowRule> {
		for (rule in scenario.flow.rules)
			if (sameId(rule.id, id))
				return rule;
		return null;
	}

	/** Only actor-scoped repeat policies may own actor-local history. */
	static function actorHistoryAllowed(rule:FlowRule):Bool
		return switch rule.repeat {
			case FlowRepeatPolicy.OncePerActor | FlowRepeatPolicy.CooldownPerActor(_): true;
			case Once | Repeat | Cooldown(_): false;
		};

	/** True when one saved actor still belongs to this scenario. */
	function scenarioHasObject(id:ScenarioId):Bool {
		for (object in scenario.objects)
			if (sameId(object.id, id))
				return true;
		return false;
	}

	/** Compare explicit global/actor history identities without null sentinels. */
	static function sameHistoryScope(left:FlowRuleHistoryScope, right:FlowRuleHistoryScope):Bool
		return switch [left, right] {
			case [GlobalRuleHistory, GlobalRuleHistory]: true;
			case [ActorRuleHistory(leftActor), ActorRuleHistory(rightActor)]: sameId(leftActor, rightActor);
			case _: false;
		};

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

	static function containsReservation(values:Array<CaxeFlowRuleReservation>, rule:FlowRule, actor:Null<ScenarioId>):Bool {
		final perActor = switch rule.repeat {
			case FlowRepeatPolicy.OncePerActor | FlowRepeatPolicy.CooldownPerActor(_): true;
			case Once | Repeat | Cooldown(_): false;
		};
		for (value in values)
			if (sameId(value.rule, rule.id) && value.perActor == perActor && (!perActor || sameOptionalId(value.actor, actor)))
				return true;
		return false;
	}

	static function sameOptionalId(left:Null<ScenarioId>, right:Null<ScenarioId>):Bool {
		if (left == null)
			return right == null;
		if (right == null)
			return false;
		return left.text() == right.text();
	}

	static inline function sameId(left:ScenarioId, right:ScenarioId):Bool
		return left.text() == right.text();

	static inline function sameContent(left:ContentId, right:ContentId):Bool
		return left.text() == right.text();
}

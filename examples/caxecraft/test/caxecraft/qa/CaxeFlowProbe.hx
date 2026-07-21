package caxecraft.qa;

import caxecraft.scenario.CaxeFlow;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowChoice;
import caxecraft.scenario.CaxeFlow.FlowComparison;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.CaxeFlowClock;
import caxecraft.scenario.CaxeFlowExecutor;
import caxecraft.scenario.CaxeFlowRuntime.FlowExecutionLimit;
import caxecraft.scenario.CaxeFlowRuntime.FlowPresentationEvent;
import caxecraft.scenario.CaxeFlowRuntime.FlowRuntimeDiagnostic;
import caxecraft.scenario.CaxeFlowRuntime.FlowTick;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickInput;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickResult;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioLimits;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioStory.ObjectiveState;
import caxecraft.scenario.ScenarioText;

/** Eval evidence for deterministic fixed-tick CaxeFlow execution. */
final class CaxeFlowProbe {
	static final PLAYER = id("player.one");
	static final IVVY = id("npc.ivvy");
	static final BROWSER = id("entity.browser");
	static final BRIDGE = id("state.bridge");
	static final CHECKPOINT = id("checkpoint.start");
	static final ZONE = id("zone.test");
	static final DIALOGUE = id("dialogue.ivvy");
	static final JOURNAL = id("journal.bridge");
	static final OBJECTIVE = id("objective.bridge");
	static final FLAG = id("map.flag");
	static final COUNTER = id("map.counter");
	static final FOLLOW_COUNT = id("map.follow-count");
	static final SEED = id("map.seed");
	static final STATE = id("map.state");
	static final LOCAL_FLAG = id("local.flag");
	static final IMMEDIATE_SEQUENCE = id("sequence.immediate");
	static final SCHEDULED_SEQUENCE = id("sequence.scheduled");
	static final TIMER = id("timer.scheduled");

	static final AIR = content("caxecraft:air");
	static final BEDROCK = content("caxecraft:bedrock");
	static final CLOSED = content("caxecraft:closed");
	static final OPEN = content("caxecraft:open");
	static final PICK = content("caxecraft:haxe-pick");
	static final SPARK = content("caxecraft:spark");
	static final SIGNAL = content("caxecraft:bridge-lowered");

	static function main():Void {
		final forward = runCoverage(false);
		final reversed = runCoverage(true);
		require(forward == reversed, "reversing rule registration changed execution");
		checkTickEdges();
		checkBudgets();
		checkScheduledFailurePolicy();
		Sys.println('caxeflow: 10 events, 12 predicates, 18 actions; stable order/repeat/defer/sequence/budgets; trace=$forward');
	}

	static function runCoverage(reverseRules:Bool):String {
		final scenario = coverageScenario(reverseRules);
		final executor = new CaxeFlowExecutor(scenario);
		final tickOne = executor.runTick({
			events: [
				EnterZone(ZONE),
				EnterZone(ZONE),
				LeaveZone(ZONE),
				Interact(BRIDGE),
				Interact(BRIDGE),
				BlockChanged(ZONE, BEDROCK),
				UseItem(PICK),
				UseItem(PICK),
				EntityDefeated(BROWSER),
				SignalReceived(content("caxecraft:coverage-signal")),
				TimerExpired(id("timer.coverage")),
				ObjectiveChanged(OBJECTIVE),
				StateChanged(FLAG)
			],
			positions: [position(PLAYER, 500, 1000, 500), position(IVVY, 2000, 1000, 1500)]
		});
		requireNoDiagnostic(tickOne, "coverage tick 1");
		require(tickOne.firedRules.length == 15, 'coverage tick 1 fired ${tickOne.firedRules.length} rules instead of 15: ${ruleNames(tickOne)}');
		require(ruleNames(tickOne).indexOf("rule.action") >= 0, "the complete action rule did not fire");
		require(counter(executor, COUNTER) == 1112, "source-ordered actions produced the wrong counter");
		require(counter(executor, SEED) == 1, "seeded choice did not advance its explicit seed once");
		require(flag(executor, FLAG), "persistent flag action did not commit");
		require(stateValue(executor, STATE) == OPEN.text(), "state action did not commit");
		require(executor.inventoryQuantity(PLAYER, PICK) == 2, "give/take actions lost their source order");
		require(!executor.objectActive(BROWSER), "spawn/despawn actions lost their source order");
		require(executor.objectState(BRIDGE).text() == OPEN.text(), "object-state action did not commit");
		require(executor.objectiveState(OBJECTIVE) == Complete, "objective action did not commit");
		require(executor.hasJournal(JOURNAL), "journal action did not commit");
		require(executor.checkpoint().text() == CHECKPOINT.text(), "checkpoint action did not commit");
		require(hasPresentation(tickOne, "dialogue"), "dialogue request was not published");
		require(hasPresentation(tickOne, "effect"), "effect request was not published");

		final tickTwo = executor.runTick({
			events: [EnterZone(ZONE), UseItem(PICK), Interact(BRIDGE)],
			positions: []
		});
		requireNoDiagnostic(tickTwo, "coverage tick 2");
		require(countRule(tickTwo, "rule.once.near") == 0, "once rule fired a second time");
		require(countRule(tickTwo, "rule.repeat") == 1, "repeat rule did not fire for the new event");
		require(countRule(tickTwo, "rule.cooldown") == 0, "cooldown rule fired one tick too early");
		require(countRule(tickTwo, "rule.follow.signal") == 1, "emitted signal did not arrive at the next tick");
		require(countRule(tickTwo, "rule.follow.objective") == 1, "objective change did not arrive at the next tick");
		require(countRule(tickTwo, "rule.follow.counter") == 4, "persistent counter changes were not deferred in source order");
		require(counter(executor, FOLLOW_COUNT) == 4, "deferred state-change rules produced the wrong state");

		final tickThree = executor.runTick({events: [Interact(BRIDGE)], positions: []});
		requireNoDiagnostic(tickThree, "coverage tick 3");
		require(countRule(tickThree, "rule.cooldown") == 1, "cooldown rule did not reopen at its exact fixed tick");
		require(countRule(tickThree, "rule.follow.timer") == 1, "scheduled timer did not become a rule event");
		require(stateValue(executor, STATE) == CLOSED.text(), "scheduled sequence did not use its captured argument");

		return coverageHash([tickOne, tickTwo, tickThree], executor);
	}

	static function coverageScenario(reverseRules:Bool):Scenario {
		final variables:Array<FlowVariable> = [
			{id: FLAG, scope: Map, initial: Flag(false)},
			{id: COUNTER, scope: Map, initial: Counter(0)},
			{id: FOLLOW_COUNT, scope: Map, initial: Counter(0)},
			{id: SEED, scope: Map, initial: Counter(0)},
			{id: STATE, scope: Quest, initial: State(CLOSED)},
			{id: LOCAL_FLAG, scope: Local(IMMEDIATE_SEQUENCE), initial: Flag(false)}
		];
		final immediate:FlowSequence = {
			id: IMMEDIATE_SEQUENCE,
			parameters: [{id: id("parameter.seed"), initial: Counter(0)}],
			actions: [
				SetFlag(LOCAL_FLAG, true),
				ChooseSeeded(id("parameter.seed"), [choice(1, [AddCounter(COUNTER, 100)]), choice(1, [PlayEffect(SPARK, null)])])
			]
		};
		final scheduled:FlowSequence = {
			id: SCHEDULED_SEQUENCE,
			parameters: [{id: id("parameter.captured"), initial: Counter(0)}],
			actions: [
				ChooseSeeded(id("parameter.captured"), [choice(1, [SetState(STATE, CLOSED)]), choice(1, [PlayEffect(SPARK, BRIDGE)])])
			]
		};
		final completeActions:Array<FlowAction> = [
			ShowDialogue(DIALOGUE),
			AddJournal(JOURNAL),
			SetFlag(FLAG, true),
			SetCounter(COUNTER, 10),
			AddCounter(COUNTER, 2),
			SetState(STATE, OPEN),
			GiveItem(PLAYER, PICK, 3),
			TakeItem(PLAYER, PICK, 1),
			Spawn(BROWSER),
			Despawn(BROWSER),
			SetObjectState(BRIDGE, OPEN),
			SetCheckpoint(CHECKPOINT),
			SetObjective(OBJECTIVE, Complete),
			PlayEffect(SPARK, BRIDGE),
			EmitSignal(SIGNAL),
			Schedule(TIMER, 2, SCHEDULED_SEQUENCE, [Variable(COUNTER)]),
			CallSequence(IMMEDIATE_SEQUENCE, [Variable(COUNTER)]),
			ChooseSeeded(SEED, [choice(1, [AddCounter(COUNTER, 1000)]), choice(2, [PlayEffect(SPARK, null)])])
		];
		final rules:Array<FlowRule> = [
			rule("rule.cooldown", 5, Cooldown(2), Interact(BRIDGE), Always, []),
			rule("rule.repeat", 5, Repeat, UseItem(PICK), Always, []),
			rule("rule.once.all", 10, Once, LeaveZone(ZONE), All([Always, ModeIs(Adventure)]), []),
			rule("rule.once.any", 10, Once, Interact(BRIDGE), AnyOf([FlagIs(FLAG, true), Always]), []),
			rule("rule.once.counter", 10, Once, EntityDefeated(BROWSER), CounterCompare(COUNTER, Equal, 0), []),
			rule("rule.once.flag", 10, Once, UseItem(PICK), FlagIs(FLAG, false), []),
			rule("rule.once.inventory", 10, Once, ObjectiveChanged(OBJECTIVE), InventoryHas(PLAYER, PICK, Equal, 0), []),
			rule("rule.once.mode", 10, Once, EnterZone(ZONE), ModeIs(Adventure), []),
			rule("rule.once.near", 10, Once, EnterZone(ZONE), NearObject(PLAYER, IVVY, 4000), []),
			rule("rule.once.not", 10, Once, BlockChanged(ZONE, BEDROCK), Not(FlagIs(FLAG, true)), []),
			rule("rule.once.object", 10, Once, TimerExpired(id("timer.coverage")), ObjectStateIs(BRIDGE, CLOSED), []),
			rule("rule.once.objective", 10, Once, StateChanged(FLAG), ObjectiveIs(OBJECTIVE, Active), []),
			rule("rule.once.state", 10, Once, SignalReceived(content("caxecraft:coverage-signal")), StateIs(STATE, CLOSED), []),
			rule("rule.action", 20, Once, EnterZone(ZONE), Always, completeActions),
			rule("rule.follow.counter", 30, Repeat, StateChanged(COUNTER), Always, [AddCounter(FOLLOW_COUNT, 1)]),
			rule("rule.follow.objective", 30, Repeat, ObjectiveChanged(OBJECTIVE), ObjectiveIs(OBJECTIVE, Complete), []),
			rule("rule.follow.signal", 30, Repeat, SignalReceived(SIGNAL), Always, []),
			rule("rule.follow.timer", 30, Repeat, TimerExpired(TIMER), Always, [PlayEffect(SPARK, BRIDGE)])
		];
		if (reverseRules)
			rules.reverse();
		return scenario({variables: variables, sequences: [immediate, scheduled], rules: rules});
	}

	static function checkTickEdges():Void {
		final delayedSequence:FlowSequence = {
			id: id("sequence.maximum-delay"),
			parameters: [],
			actions: [SetState(STATE, OPEN)]
		};
		final delayedFlow:CaxeFlow = {
			variables: [{id: STATE, scope: Map, initial: State(CLOSED)}],
			sequences: [delayedSequence],
			rules: [
				rule("rule.maximum-delay", 0, Once, EnterZone(ZONE), Always, [Schedule(TIMER, 2147483647, delayedSequence.id, [])]),
				rule("rule.maximum-delay-timer", 1, Repeat, TimerExpired(TIMER), Always, [])
			]
		};
		final delayed = new CaxeFlowExecutor(scenario(delayedFlow));
		requireNoDiagnostic(delayed.runTick(oneEvent()), "maximum-delay schedule tick");
		final next = delayed.runTick({events: [], positions: []});
		requireNoDiagnostic(next, "maximum-delay next tick");
		require(stateValue(delayed, STATE) == CLOSED.text(), "maximum positive delay wrapped and ran early");
		require(countRule(next, "rule.maximum-delay-timer") == 0, "maximum positive delay emitted its timer early");

		final beforeBoundary:FlowTick = {epoch: 2, offset: 147483647};
		final atBoundary = requireTick(CaxeFlowClock.next(beforeBoundary), "32-bit boundary tick");
		final afterBoundary = requireTick(CaxeFlowClock.next(atBoundary), "post-boundary tick");
		final latestDueTick = requireTick(CaxeFlowClock.dueTick(beforeBoundary, 2147483647), "maximum delay");
		require(atBoundary.epoch == 2 && atBoundary.offset == 147483648, "fixed tick wrapped at the 32-bit boundary");
		require(latestDueTick.epoch == 4 && latestDueTick.offset == 294967294, "maximum delay wrapped when added beyond the 32-bit boundary");
		require(!CaxeFlowClock.cooldownHasElapsed(atBoundary, beforeBoundary, 2), "boundary cooldown reopened one tick early");
		require(CaxeFlowClock.cooldownHasElapsed(afterBoundary, beforeBoundary, 2), "boundary cooldown did not reopen after two ticks");

		final last:FlowTick = {epoch: CaxeFlowClock.MAX_EPOCH, offset: CaxeFlowClock.TICKS_PER_EPOCH - 1};
		final beforeLast:FlowTick = {epoch: CaxeFlowClock.MAX_EPOCH, offset: CaxeFlowClock.TICKS_PER_EPOCH - 2};
		final exactLast = requireTick(CaxeFlowClock.next(beforeLast), "final clock tick");
		require(CaxeFlowClock.isDue(exactLast, last), "final safe tick comparison changed");
		require(CaxeFlowClock.next(last) == null, "fixed clock wrapped after its explicit final tick");
		require(CaxeFlowClock.dueTick(last, 1) == null, "deferred clock addition wrapped after its explicit final tick");

		// Reaching this boundary through ordinary ticks would take more than a
		// billion years. The same validated restoration seam planned for save-game
		// loading positions only the clock; scheduling and failure behavior still
		// run through the public tick operation.
		final edgeSequence:FlowSequence = {id: id("sequence.clock-edge"), parameters: [], actions: []};
		final edgeRule = rule("rule.clock-edge", 0, Once, EnterZone(ZONE), Always, [Schedule(TIMER, 1, edgeSequence.id, [])]);
		final edgeExecutor = new CaxeFlowExecutor(scenario({variables: [], sequences: [edgeSequence], rules: [edgeRule]}), beforeLast);
		final scheduleFailure = edgeExecutor.runTick(oneEvent());
		expectLimitResult(scheduleFailure, FixedTickEpochs, CaxeFlowClock.MAX_EPOCH, "rule.clock-edge");
		require(scheduleFailure.presentation.length == 0, "clock-edge schedule published a partial presentation event");
		final exhausted = edgeExecutor.runTick({events: [], positions: []});
		expectLimitResult(exhausted, FixedTickEpochs, CaxeFlowClock.MAX_EPOCH);
		require(exhausted.tick.epoch == last.epoch && exhausted.tick.offset == last.offset, "exhausted clock changed its last valid tick");
	}

	static function checkScheduledFailurePolicy():Void {
		final overflowing:Array<FlowAction> = [];
		for (_ in 0...ScenarioLimits.MAX_ACTIONS_PER_TICK + 1)
			overflowing.push(PlayEffect(SPARK, null));
		final first:FlowSequence = {id: id("sequence.fail-first"), parameters: [], actions: overflowing};
		final second:FlowSequence = {id: id("sequence.skipped-second"), parameters: [], actions: [SetFlag(FLAG, true)]};
		final flow:CaxeFlow = {
			variables: [{id: FLAG, scope: Map, initial: Flag(false)}],
			sequences: [first, second],
			rules: [
				rule("rule.schedule-failure", 0, Once, EnterZone(ZONE), Always, [
					Schedule(id("timer.fail-first"), 1, first.id, []),
					Schedule(id("timer.skipped-second"), 1, second.id, [])
				])
			]
		};
		final executor = new CaxeFlowExecutor(scenario(flow));
		requireNoDiagnostic(executor.runTick(oneEvent()), "scheduled failure setup");
		final failed = executor.runTick({events: [], positions: []});
		expectLimitResult(failed, Actions, ScenarioLimits.MAX_ACTIONS_PER_TICK, "sequence.fail-first");
		require(!flag(executor, FLAG), "later due sequence ran after an earlier due sequence exhausted the budget");
		final following = executor.runTick({events: [], positions: []});
		requireNoDiagnostic(following, "post-failure tick");
		require(!flag(executor, FLAG), "accepted due-sequence suffix was retried after the documented partial failure");
	}

	static function checkBudgets():Void {
		final tooManyEvents:Array<FlowEvent> = [];
		for (_ in 0...ScenarioLimits.MAX_EVENTS_PER_TICK + 1)
			tooManyEvents.push(EnterZone(ZONE));
		expectLimit(scenario(emptyFlow()), {events: tooManyEvents, positions: []}, TickEvents, ScenarioLimits.MAX_EVENTS_PER_TICK);

		final ruleFlood:Array<FlowRule> = [];
		for (index in 0...ScenarioLimits.MAX_RULE_EXECUTIONS_PER_TICK + 1)
			ruleFlood.push(rule('rule.flood.${StringTools.lpad(Std.string(index), "0", 4)}', 0, Repeat, EnterZone(ZONE), Always, []));
		expectLimit(scenario({variables: [], sequences: [], rules: ruleFlood}), oneEvent(), RuleExecutions, ScenarioLimits.MAX_RULE_EXECUTIONS_PER_TICK,
			"rule.flood.2048");

		final actionFlood:Array<FlowAction> = [];
		for (_ in 0...ScenarioLimits.MAX_ACTIONS_PER_TICK + 1)
			actionFlood.push(PlayEffect(SPARK, null));
		final actionResult = expectActionLimit(actionFlood, Actions, ScenarioLimits.MAX_ACTIONS_PER_TICK);
		require(actionResult.presentation.length == ScenarioLimits.MAX_ACTIONS_PER_TICK, "action-budget failure did not preserve the exact completed prefix");

		final emptySequence:FlowSequence = {id: id("sequence.empty"), parameters: [], actions: []};
		final callFlood:Array<FlowAction> = [];
		for (_ in 0...ScenarioLimits.MAX_SEQUENCE_CALLS_PER_TICK + 1)
			callFlood.push(CallSequence(emptySequence.id, []));
		expectLimit(scenario({variables: [], sequences: [emptySequence], rules: [budgetRule(callFlood)]}), oneEvent(), SequenceCalls,
			ScenarioLimits.MAX_SEQUENCE_CALLS_PER_TICK, "rule.budget");

		final recursive:FlowSequence = {id: id("sequence.recursive"), parameters: [], actions: []};
		recursive.actions.push(CallSequence(recursive.id, []));
		expectLimit(scenario({variables: [], sequences: [recursive], rules: [budgetRule([CallSequence(recursive.id, [])])]}), oneEvent(), SequenceDepth,
			ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH, "sequence.recursive");

		final spawnFlood:Array<FlowAction> = [];
		for (_ in 0...ScenarioLimits.MAX_SPAWNED_OBJECTS_PER_TICK + 1)
			spawnFlood.push(Spawn(BROWSER));
		expectActionLimit(spawnFlood, SpawnedObjects, ScenarioLimits.MAX_SPAWNED_OBJECTS_PER_TICK);

		final scheduleFlood:Array<FlowAction> = [];
		for (_ in 0...ScenarioLimits.MAX_SCHEDULED_WORK_PER_TICK + 1)
			scheduleFlood.push(Schedule(TIMER, 1, emptySequence.id, []));
		expectLimit(scenario({variables: [], sequences: [emptySequence], rules: [budgetRule(scheduleFlood)]}), oneEvent(), ScheduledWork,
			ScenarioLimits.MAX_SCHEDULED_WORK_PER_TICK, "rule.budget");

		final predicateFlood:Array<FlowPredicate> = [];
		for (_ in 0...ScenarioLimits.MAX_PREDICATE_EVALUATIONS_PER_TICK)
			predicateFlood.push(Always);
		final predicateRule = rule("rule.predicate-budget", 0, Repeat, EnterZone(ZONE), All(predicateFlood), []);
		expectLimit(scenario({variables: [], sequences: [], rules: [predicateRule]}), oneEvent(), PredicateEvaluations,
			ScenarioLimits.MAX_PREDICATE_EVALUATIONS_PER_TICK, "rule.predicate-budget");

		final deferredFlood:Array<FlowAction> = [];
		for (_ in 0...ScenarioLimits.MAX_DEFERRED_EVENTS + 1)
			deferredFlood.push(EmitSignal(SIGNAL));
		expectActionLimit(deferredFlood, DeferredWork, ScenarioLimits.MAX_DEFERRED_EVENTS);

		final malformedFlow:CaxeFlow = {
			variables: [{id: SEED, scope: Map, initial: Counter(0)}],
			sequences: [],
			rules: [budgetRule([ChooseSeeded(SEED, [])])]
		};
		final malformedResult = new CaxeFlowExecutor(scenario(malformedFlow)).runTick(oneEvent());
		require(malformedResult.diagnostics.length == 1, "malformed runtime action did not fail exactly once");
		switch malformedResult.diagnostics[0] {
			case InvalidRuntimeAction(owner):
				require(owner.text() == "rule.budget", "malformed action lost its owning rule");
			case diagnostic:
				throw 'expected invalid runtime action, got ${Std.string(diagnostic)}';
		}

		final missingObject = id("object.missing");
		final missingResult = new CaxeFlowExecutor(scenario(emptyFlow())).runTick({
			events: [],
			positions: [position(missingObject, 0, 0, 0)]
		});
		require(missingResult.diagnostics.length == 1, "unknown position object did not fail exactly once");
		switch missingResult.diagnostics[0] {
			case InvalidRuntimeReference(value):
				require(value.text() == missingObject.text(), "unknown object diagnostic lost its identity");
			case diagnostic:
				throw 'expected invalid runtime reference, got ${Std.string(diagnostic)}';
		}
	}

	static function expectActionLimit(actions:Array<FlowAction>, kind:FlowExecutionLimit, maximum:Int):FlowTickResult
		return expectLimit(scenario({variables: [], sequences: [], rules: [budgetRule(actions)]}), oneEvent(), kind, maximum, "rule.budget");

	static function expectLimit(source:Scenario, input:FlowTickInput, expected:FlowExecutionLimit, maximum:Int, ?expectedOwner:String):FlowTickResult {
		final result = new CaxeFlowExecutor(source).runTick(input);
		expectLimitResult(result, expected, maximum, expectedOwner);
		return result;
	}

	static function expectLimitResult(result:FlowTickResult, expected:FlowExecutionLimit, maximum:Int, ?expectedOwner:String):Void {
		require(result.diagnostics.length == 1, 'expected one ${Std.string(expected)} diagnostic');
		switch result.diagnostics[0] {
			case LimitExceeded(kind, actualMaximum, owner):
				require(kind == expected && actualMaximum == maximum, 'expected ${Std.string(expected)}($maximum), got ${Std.string(kind)}($actualMaximum)');
				final actualOwner = owner == null ? null : owner.text();
				require(actualOwner == expectedOwner, 'expected ${Std.string(expected)} owner ${Std.string(expectedOwner)}, got ${Std.string(actualOwner)}');
			case diagnostic:
				throw 'expected ${Std.string(expected)}, got ${Std.string(diagnostic)}';
		}
	}

	static function scenario(flow:CaxeFlow):Scenario {
		final transform:ScenarioTransform = {
			xMilli: 500,
			yMilli: 1000,
			zMilli: 500,
			yawDegrees: 0
		};
		final nearby:ScenarioTransform = {
			xMilli: 2000,
			yMilli: 1000,
			zMilli: 1500,
			yawDegrees: 0
		};
		final bounds:VoxelBounds = {origin: {x: 0, y: 0, z: 0}, size: {width: 2, height: 2, depth: 2}};
		return {
			formatVersion: 1,
			requiredFeatures: [content("caxecraft:core")],
			optionalFeatures: [],
			id: id("probe.map"),
			assetPack: new LogicalPath("packs/caxecraft/base"),
			title: ScenarioText.Literal("CaxeFlow probe"),
			mode: ScenarioMode.Adventure,
			world: {
				size: {width: 2, height: 2, depth: 2},
				palette: [{code: 0, blockType: AIR}],
				chunks: []
			},
			objects: [
				{id: PLAYER, tags: [], placement: PlayerSpawn(transform)},
				{id: IVVY, tags: [], placement: Npc(content("caxecraft:ivvy"), DIALOGUE, nearby)},
				{id: BROWSER, tags: [], placement: Entity(content("caxecraft:browser"), nearby)},
				{id: BRIDGE, tags: [], placement: StatefulObject(content("caxecraft:bridge"), CLOSED, nearby)},
				{id: CHECKPOINT, tags: [], placement: Checkpoint(transform)},
				{id: ZONE, tags: [], placement: TriggerZone(bounds)}
			],
			story: {
				dialogues: [{id: DIALOGUE, lines: [{speaker: IVVY, text: ScenarioText.Literal("Ready!")}]}],
				journal: [
					{id: JOURNAL, title: ScenarioText.Literal("Bridge"), body: ScenarioText.Literal("Lower it.")}
				],
				objectives: [
					{
						id: OBJECTIVE,
						title: ScenarioText.Literal("Bridge"),
						body: ScenarioText.Literal("Reach it."),
						initialState: ObjectiveState.Active
					}
				],
				routes: []
			},
			flow: flow,
			extensions: []
		};
	}

	static function emptyFlow():CaxeFlow
		return {variables: [], sequences: [], rules: []};

	static function budgetRule(actions:Array<FlowAction>):FlowRule
		return rule("rule.budget", 0, Repeat, EnterZone(ZONE), Always, actions);

	static function oneEvent():FlowTickInput
		return {events: [EnterZone(ZONE)], positions: []};

	static function rule(name:String, priority:Int, repeat:FlowRepeatPolicy, event:FlowEvent, predicate:FlowPredicate, actions:Array<FlowAction>):FlowRule
		return {
			id: id(name),
			priority: priority,
			repeat: repeat,
			event: event,
			predicate: predicate,
			actions: actions
		};

	static function choice(weight:Int, actions:Array<FlowAction>):FlowChoice
		return {weight: weight, actions: actions};

	static function position(objectId:ScenarioId, x:Int, y:Int, z:Int):caxecraft.scenario.CaxeFlowRuntime.FlowPosition
		return {
			objectId: objectId,
			xMilli: x,
			yMilli: y,
			zMilli: z
		};

	static function requireNoDiagnostic(result:FlowTickResult, label:String):Void
		require(result.diagnostics.length == 0, '$label failed: ${Std.string(result.diagnostics[0])}');

	static function countRule(result:FlowTickResult, expected:String):Int {
		var count = 0;
		for (ruleId in result.firedRules)
			if (ruleId.text() == expected)
				count++;
		return count;
	}

	static function ruleNames(result:FlowTickResult):String {
		final names:Array<String> = [];
		for (ruleId in result.firedRules)
			names.push(ruleId.text());
		return names.join(",");
	}

	static function hasPresentation(result:FlowTickResult, expected:String):Bool {
		for (event in result.presentation) {
			final name = switch event {
				case DialogueRequested(_): "dialogue";
				case EffectRequested(_, _): "effect";
				case _: "other";
			}
			if (name == expected)
				return true;
		}
		return false;
	}

	static function coverageHash(results:Array<FlowTickResult>, executor:CaxeFlowExecutor):String {
		var hash = 17;
		for (result in results) {
			hash = mix(hash, result.tick.epoch);
			hash = mix(hash, result.tick.offset);
			for (ruleId in result.firedRules)
				hash = mixText(hash, ruleId.text());
			for (event in result.presentation)
				hash = mixText(hash, presentationText(event));
		}
		hash = mix(hash, counter(executor, COUNTER));
		hash = mix(hash, counter(executor, FOLLOW_COUNT));
		hash = mixText(hash, stateValue(executor, STATE));
		return Std.string(hash);
	}

	static function presentationText(event:FlowPresentationEvent):String
		return switch event {
			case DialogueRequested(id): 'dialogue:${id.text()}';
			case JournalAdded(id): 'journal:${id.text()}';
			case VariableChanged(id, value): 'variable:${id.text()}:${flowValueText(value)}';
			case InventoryChanged(owner, itemType, quantity): 'inventory:${owner.text()}:${itemType.text()}:$quantity';
			case ObjectSpawned(id): 'spawn:${id.text()}';
			case ObjectDespawned(id): 'despawn:${id.text()}';
			case ObjectStateChanged(id, value): 'object-state:${id.text()}:${value.text()}';
			case CheckpointChanged(id): 'checkpoint:${id.text()}';
			case ObjectiveChanged(id, value): 'objective:${id.text()}:${objectiveStateText(value)}';
			case EffectRequested(effect, objectId): 'effect:${effect.text()}:${objectId == null ? "none" : objectId.text()}';
		};

	static function flowValueText(value:FlowValue):String
		return switch value {
			case Flag(enabled): enabled ? "flag:true" : "flag:false";
			case Counter(counter): 'counter:$counter';
			case State(state): 'state:${state.text()}';
		};

	static function objectiveStateText(value:ObjectiveState):String
		return switch value {
			case Hidden: "hidden";
			case Active: "active";
			case Complete: "complete";
			case Failed: "failed";
		};

	static function requireTick(value:Null<FlowTick>, label:String):FlowTick {
		if (value == null)
			throw '$label unexpectedly exceeded the fixed clock';
		return value;
	}

	static function counter(executor:CaxeFlowExecutor, variable:ScenarioId):Int
		return switch executor.variable(variable) {
			case Counter(value): value;
			case _: throw 'missing counter ${variable.text()}';
		};

	static function flag(executor:CaxeFlowExecutor, variable:ScenarioId):Bool
		return switch executor.variable(variable) {
			case Flag(value): value;
			case _: throw 'missing flag ${variable.text()}';
		};

	static function stateValue(executor:CaxeFlowExecutor, variable:ScenarioId):String
		return switch executor.variable(variable) {
			case State(value): value.text();
			case _: throw 'missing state ${variable.text()}';
		};

	static function mix(current:Int, value:Int):Int
		return (current * 31) ^ value;

	static function mixText(current:Int, value:String):Int {
		var result = mix(current, value.length);
		for (index in 0...value.length)
			result = mix(result, value.charCodeAt(index));
		return result;
	}

	static inline function id(value:String):ScenarioId
		return new ScenarioId(value);

	static inline function content(value:String):ContentId
		return new ContentId(value);

	static function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}

package caxecraft.qa;

import caxecraft.scenario.CaxeFlow;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowChoice;
import caxecraft.scenario.CaxeFlow.FlowComparison;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowEventContext;
import caxecraft.scenario.CaxeFlow.FlowEventOccurrence;
import caxecraft.scenario.CaxeFlow.FlowEventPosition;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.CaxeFlowClock;
import caxecraft.scenario.CaxeFlowDiagnosticText.caxeFlowDiagnosticMessage;
import caxecraft.scenario.CaxeFlowExecutor;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventOccurrence;
import caxecraft.scenario.CaxeFlowEventRegistry.allFlowEventDescriptors;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventContextMatches;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventDescriptorForSyntax;
import caxecraft.scenario.CaxeFlowEventRegistry.externalFlowEventCapacity;
import caxecraft.scenario.CaxeFlowEventRegistry.validateFlowEventDescriptors;
import caxecraft.scenario.CaxeFlowEventOwnership.flowOwnsCampaignExit;
import caxecraft.scenario.CaxeFlowEventOwnership.flowOwnsTimer;
import caxecraft.scenario.CaxeFlowRuntime.FlowExecutionLimit;
import caxecraft.scenario.CaxeFlowRuntime.FlowFailureDisposition;
import caxecraft.scenario.CaxeFlowRuntime.FlowPresentationEvent;
import caxecraft.scenario.CaxeFlowRuntime.FlowRuntimeDiagnostic;
import caxecraft.scenario.CaxeFlowRuntime.FlowTick;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickInput;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickResult;
import caxecraft.scenario.CaxeFlowRuntime.FlowTraceEntry;
import caxecraft.scenario.CaxeFlowSnapshot.CaxeFlowSnapshot;
import caxecraft.scenario.CaxeFlowSnapshot.FlowDeferredSnapshot;
import caxecraft.scenario.CaxeFlowSnapshot.FlowInventorySnapshot;
import caxecraft.scenario.CaxeFlowSnapshot.FlowRuleHistorySnapshot;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioContentRegistry;
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
	static final FIRST_OBJECTIVE = id("objective.first");
	static final SECOND_OBJECTIVE = id("objective.second");
	static final THIRD_OBJECTIVE = id("objective.third");
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
	static final BROKEN = content("caxecraft:broken");
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
		checkActiveObjectiveProjection();
		checkEventRegistry();
		checkExecutorOwnedEventIdentities();
		checkEventContextAndPerActorPolicy();
		checkRejectedInputIsAtomic();
		checkSnapshotBoundaries();
		checkLocalizedDiagnostics();
		Sys.println('caxeflow: 13 events, 14 predicates, 19 actions; context/trace/per-actor/repeat/defer/sequence/budgets; trace=$forward');
	}

	/** Timer/exit events must come from matching actions; defeat may be environmental. */
	static function checkExecutorOwnedEventIdentities():Void {
		final source = coverageScenario(false);
		require(flowOwnsTimer(source.flow, TIMER), "scheduled timer lost its action owner");
		require(flowOwnsCampaignExit(source.flow, id("exit.coverage")), "campaign exit lost its action owner");
		require(!flowOwnsTimer(source.flow, id("timer.missing")), "unknown timer acquired an owner");

		final rejected = newExecutor(source).runTick({events: [observed(TimerExpired(id("timer.missing")))], positions: []});
		require(rejected.diagnostics.length == 1, "unowned timer event was admitted");
		switch rejected.diagnostics[0] {
			case InvalidRuntimeEvent(_):
			case diagnostic:
				throw 'unowned timer reported ${Std.string(diagnostic)}';
		}

		final environmental = newExecutor(source).runTick({
			events: [flowEventOccurrence(EntityDefeated(BROWSER), NoEventContext)],
			positions: []
		});
		requireNoDiagnostic(environmental, "environmental defeat context");
	}

	/** Keep runtime failures linked to data-owned messages and ordered arguments. */
	static function checkLocalizedDiagnostics():Void {
		final eventText = caxeFlowDiagnosticMessage(InvalidRuntimeEvent(allFlowEventDescriptors()[0].id));
		require(eventText.message.text() == "caxeflow.runtime.invalid-event-context"
			&& eventText.arguments.length == 1
			&& eventText.arguments[0] == "enter-zone",
			"invalid event context lost its stable catalog key or argument");
		final budgetText = caxeFlowDiagnosticMessage(LimitExceeded(FlowExecutionLimit.Actions, 12, id("rule.busy")));
		require(budgetText.message.text() == "caxeflow.runtime.limit-owner"
			&& budgetText.arguments.length == 3
			&& budgetText.arguments[0] == "rule.busy"
			&& budgetText.arguments[1] == "actions"
			&& budgetText.arguments[2] == "12",
			"work-budget failure lost its owner, limit, or catalog key");
	}

	/** Keep syntax, editor metadata, runtime context, and trace IDs on one catalog. */
	static function checkEventRegistry():Void {
		final descriptors = allFlowEventDescriptors();
		require(descriptors.length == 13, 'event registry exposed ${descriptors.length} sources instead of 13');
		require(validateFlowEventDescriptors(descriptors).length == 0, "canonical event descriptors did not validate");
		final duplicate = descriptors.copy();
		duplicate.push(descriptors[0]);
		require(validateFlowEventDescriptors(duplicate).length > 0, "event registry admitted a duplicate stable ID");
		require(flowEventDescriptorForSyntax("enter-zone") != null && flowEventDescriptorForSyntax("unknown-event") == null,
			"event syntax lookup guessed an unknown source");
		require(externalFlowEventCapacity(1, 3) == ScenarioLimits.MAX_EVENTS_PER_TICK - 6 && externalFlowEventCapacity(2, 64) == 0,
			"spatial reservation did not preserve exact movement events under queue pressure");
		final point:FlowEventPosition = {xMilli: 0, yMilli: 0, zMilli: 0};
		require(flowEventContextMatches(EnterZone(ZONE), SpatialEventContext(PLAYER, point, point, false))
			&& !flowEventContextMatches(EnterZone(ZONE), ActorEventContext(PLAYER))
			&& flowEventContextMatches(BlockChanged(ZONE, BEDROCK), NoEventContext)
			&& flowEventContextMatches(BlockChanged(ZONE, BEDROCK), ActorEventContext(PLAYER)),
			"event registry accepted the wrong closed context shape");
	}

	/** Prove actor context, swept context, trace, and per-actor history together. */
	static function checkEventContextAndPerActorPolicy():Void {
		final countId = id("map.context-count");
		final flow:CaxeFlow = {
			variables: [{id: countId, scope: Map, initial: Counter(0)}],
			sequences: [],
			rules: [
				rule("rule.actor.ivvy", 0, Repeat, EnterZone(ZONE), EventActorIs(IVVY), [AddCounter(countId, 10)]),
				rule("rule.actor.once", 1, OncePerActor, EnterZone(ZONE), EventSweptIs(true), [AddCounter(countId, 1), EmitSignal(SIGNAL)]),
				rule("rule.actor.cooldown", 2, CooldownPerActor(2), EnterZone(ZONE), Always, [])
			]
		};
		final executor = newExecutor(scenario(flow));
		final first = executor.runTick({
			events: [
				spatial(EnterZone(ZONE), PLAYER, true),
				spatial(EnterZone(ZONE), IVVY, true),
				spatial(EnterZone(ZONE), PLAYER, true)
			],
			positions: []
		});
		requireNoDiagnostic(first, "per-actor context tick 1");
		final firstCount = counter(executor, countId);
		require(firstCount == 12, 'actor and swept predicates produced $firstCount instead of 12; fired=${ruleNames(first)} trace=${Std.string(first.trace)}');
		require(countRule(first, "rule.actor.once") == 2, "once-per-actor did not reserve each actor independently");
		require(countRule(first, "rule.actor.cooldown") == 2, "cooldown-per-actor did not admit two actors independently");
		require(traceCount(first, "event") == 3 && traceCount(first, "predicate") >= 3 && traceCount(first, "action") == 5
			&& traceCount(first, "deferred") == 5,
			"event trace omitted WHEN, IF, DO, or deferred evidence");

		final second = executor.runTick({events: [spatial(EnterZone(ZONE), PLAYER, true), spatial(EnterZone(ZONE), BROWSER, true)], positions: []});
		requireNoDiagnostic(second, "per-actor context tick 2");
		require(countRule(second, "rule.actor.once") == 1, "once-per-actor forgot or shared the wrong actor history");
		require(countRule(second, "rule.actor.cooldown") == 1, "cooldown-per-actor did not admit the new actor only");

		final third = executor.runTick({events: [spatial(EnterZone(ZONE), PLAYER, false)], positions: []});
		requireNoDiagnostic(third, "per-actor context tick 3");
		require(countRule(third, "rule.actor.once") == 0, "once-per-actor reopened for an existing actor");
		require(countRule(third, "rule.actor.cooldown") == 1, "cooldown-per-actor did not reopen at its exact actor-local tick");

		final malformed = newExecutor(scenario(flow)).runTick({events: [flowEventOccurrence(Interact(BRIDGE))], positions: []});
		require(malformed.diagnostics.length == 1, "malformed event context did not fail exactly once");
		switch malformed.diagnostics[0] {
			case InvalidRuntimeEvent(id) if (id.text() == "interact"):
			case _:
				throw "malformed event context failed with the wrong diagnostic";
		}
	}

	/** Reject malformed tick input before the clock or any position can change. */
	static function checkRejectedInputIsAtomic():Void {
		final source = scenario(emptyFlow());
		final unknownExecutor = newExecutor(source);
		final initial = unknownExecutor.snapshot();
		final unknown = unknownExecutor.runTick({
			events: [],
			positions: [position(PLAYER, 9000, 9000, 9000), position(id("object.missing"), 0, 0, 0)]
		});
		require(unknown.diagnostics.length == 1
			&& sameTick(unknownExecutor.tick(), initial.tick)
			&& sameObjectSnapshot(unknownExecutor.snapshot(), initial, PLAYER),
			"unknown position changed the clock or an earlier valid position before rejection");
		final latched = unknownExecutor.runTick(oneEvent());
		require(latched.diagnostics.length == 1
			&& sameTick(unknownExecutor.tick(), initial.tick)
			&& sameObjectSnapshot(unknownExecutor.snapshot(), initial, PLAYER),
			"executor continued after a terminal runtime-input fault");

		final duplicateExecutor = newExecutor(source);
		final duplicate = duplicateExecutor.runTick({
			events: [],
			positions: [position(PLAYER, 1000, 1000, 1000), position(PLAYER, 2000, 2000, 2000)]
		});
		require(duplicate.diagnostics.length == 1
			&& sameTick(duplicateExecutor.tick(), initial.tick)
			&& sameObjectSnapshot(duplicateExecutor.snapshot(), initial, PLAYER),
			"duplicate position changed state before rejection");

		final point:FlowEventPosition = {xMilli: 0, yMilli: 0, zMilli: 0};
		final staleExecutor = newExecutor(source);
		final staleEvent = staleExecutor.runTick({
			events: [
				flowEventOccurrence(EnterZone(id("zone.missing")), SpatialEventContext(PLAYER, point, point, false))
			],
			positions: []
		});
		require(staleEvent.diagnostics.length == 1
			&& sameTick(staleExecutor.tick(), initial.tick), "stale runtime event advanced the clock before rejection");
	}

	/** Reject future history and deferred event kinds the executor cannot produce. */
	static function checkSnapshotBoundaries():Void {
		final source = scenario({variables: [], sequences: [], rules: [rule("rule.saved", 0, Once, EnterZone(ZONE), Always, [])]});
		final executor = newExecutor(source);
		requireNoDiagnostic(executor.runTick(oneEvent()), "snapshot boundary setup");
		final saved = executor.snapshot();
		final futureTick = requireTick(CaxeFlowClock.next(saved.tick), "future history tick");
		final futureHistory:Array<FlowRuleHistorySnapshot> = [];
		for (entry in saved.ruleHistory)
			futureHistory.push({
				rule: entry.rule,
				scope: entry.scope,
				hasFired: entry.hasFired,
				lastTick: futureTick
			});
		final futureSnapshot:CaxeFlowSnapshot = {
			scenario: saved.scenario,
			tick: saved.tick,
			state: saved.state,
			ruleHistory: futureHistory,
			deferred: saved.deferred
		};
		final futureCandidate = newExecutor(source);
		require(!futureCandidate.restore(futureSnapshot) && sameTick(futureCandidate.tick(), CaxeFlowClock.start()),
			"snapshot admitted rule history from after its saved tick");

		final forbiddenDeferred:Array<FlowDeferredSnapshot> = [DeferredEventSnapshot(futureTick, observed(EnterZone(ZONE)))];
		final deferredSnapshot:CaxeFlowSnapshot = {
			scenario: saved.scenario,
			tick: saved.tick,
			state: saved.state,
			ruleHistory: saved.ruleHistory,
			deferred: forbiddenDeferred
		};
		require(!newExecutor(source).restore(deferredSnapshot), "snapshot admitted an event kind the executor can never defer");

		final incompatibleObjects = [
			for (object in saved.state.objects)
				{
					id: object.id,
					active: object.active,
					state: object.id.text() == BRIDGE.text() ? BROKEN : object.state,
					hasPosition: object.hasPosition,
					xMilli: object.xMilli,
					yMilli: object.yMilli,
					zMilli: object.zMilli
				}
		];
		final incompatibleState:CaxeFlowSnapshot = {
			scenario: saved.scenario,
			tick: saved.tick,
			state: {
				variables: saved.state.variables,
				objects: incompatibleObjects,
				inventory: saved.state.inventory,
				objectives: saved.state.objectives,
				journal: saved.state.journal,
				checkpoint: saved.state.checkpoint
			},
			ruleHistory: saved.ruleHistory,
			deferred: saved.deferred
		};
		require(!newExecutor(source).restore(incompatibleState), "snapshot admitted a state that its object content type does not own");

		final invalidEntries:Array<FlowInventorySnapshot> = [
			{owner: PLAYER, itemType: content("caxecraft:unknown-item"), quantity: 1},
			{owner: PLAYER, itemType: PICK, quantity: 65}
		];
		for (entry in invalidEntries) {
			final invalidInventory:CaxeFlowSnapshot = {
				scenario: saved.scenario,
				tick: saved.tick,
				state: {
					variables: saved.state.variables,
					objects: saved.state.objects,
					inventory: [entry],
					objectives: saved.state.objectives,
					journal: saved.state.journal,
					checkpoint: saved.state.checkpoint
				},
				ruleHistory: saved.ruleHistory,
				deferred: saved.deferred
			};
			require(!newExecutor(source).restore(invalidInventory), "snapshot admitted an unknown item or a quantity above its content stack bound");
		}

		final invalidStateFlow:CaxeFlow = {
			variables: [],
			sequences: [],
			rules: [
				rule("rule.invalid-object-state", 0, Once, EnterZone(ZONE), Always, [SetObjectState(BROWSER, OPEN)])
			]
		};
		final invalidState = newExecutor(scenario(invalidStateFlow));
		final result = invalidState.runTick(oneEvent());
		require(result.diagnostics.length == 1
			&& invalidState.objectState(BROWSER) == null, "runtime attached persistent state to a non-stateful object");
	}

	/**
		Prove the HUD-facing objective is derived from complete authored state.

		More than one objective can be active while rules run. The first active
		objective in authored order is the deterministic public choice. Completing or
		failing that objective must reveal an objective that was already active; an
		unrelated presentation event must not change the choice.
	**/
	static function checkActiveObjectiveProjection():Void {
		final source = scenario({
			variables: [],
			sequences: [],
			rules: [
				rule("rule.complete-second", 0, Once, Interact(BRIDGE), Always, [ShowDialogue(DIALOGUE), SetObjective(SECOND_OBJECTIVE, Complete)]),
				rule("rule.fail-third", 0, Once, UseItem(PICK), Always, [SetObjective(THIRD_OBJECTIVE, Failed)]),
				rule("rule.activate-first", 0, Once, SignalReceived(SIGNAL), Always, [SetObjective(FIRST_OBJECTIVE, Active)])
			]
		});
		source.story.objectives.resize(0);
		source.story.objectives.push(objective(FIRST_OBJECTIVE, Hidden));
		source.story.objectives.push(objective(SECOND_OBJECTIVE, Active));
		source.story.objectives.push(objective(THIRD_OBJECTIVE, Active));
		final executor = newExecutor(source);

		final initial = executor.runTick({events: [], positions: []});
		requireNoDiagnostic(initial, "initial objective projection");
		require(objectiveName(initial) == SECOND_OBJECTIVE.text(), "initial projection ignored authored objective order");

		final fallback = executor.runTick({events: [observed(Interact(BRIDGE))], positions: []});
		requireNoDiagnostic(fallback, "completed objective fallback");
		require(hasPresentation(fallback, "dialogue"), "unrelated presentation event was not exercised");
		require(objectiveName(fallback) == THIRD_OBJECTIVE.text(), "completing the selected objective did not reveal an already-active objective");

		final empty = executor.runTick({events: [observed(UseItem(PICK))], positions: []});
		requireNoDiagnostic(empty, "failed objective fallback");
		require(objectiveName(empty) == "", "projection retained an objective after every objective became inactive");

		final reactivated = executor.runTick({events: [observed(SignalReceived(SIGNAL))], positions: []});
		requireNoDiagnostic(reactivated, "reactivated objective projection");
		require(objectiveName(reactivated) == FIRST_OBJECTIVE.text(), "projection did not publish a newly active objective");
	}

	static function runCoverage(reverseRules:Bool):String {
		final scenario = coverageScenario(reverseRules);
		final executor = newExecutor(scenario);
		final tickOne = executor.runTick({
			events: [
				observed(EnterZone(ZONE)),
				observed(EnterZone(ZONE)),
				observed(LeaveZone(ZONE)),
				observed(Interact(BRIDGE)),
				observed(Interact(BRIDGE)),
				observed(BlockChanged(ZONE, BEDROCK)),
				observed(UseItem(PICK)),
				observed(UseItem(PICK)),
				observed(EntityDefeated(BROWSER)),
				observed(SignalReceived(content("caxecraft:coverage-signal"))),
				observed(TimerExpired(id("timer.coverage"))),
				observed(ObjectiveChanged(OBJECTIVE)),
				observed(StateChanged(FLAG))
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

		final saved = executor.snapshot();
		final wrongScenario:CaxeFlowSnapshot = {
			scenario: id("probe.wrong-map"),
			tick: saved.tick,
			state: saved.state,
			ruleHistory: saved.ruleHistory,
			deferred: saved.deferred
		};
		final rejected = newExecutor(scenario);
		require(!rejected.restore(wrongScenario) && rejected.tick().epoch == 0 && rejected.tick().offset == 0,
			"wrong-map snapshot changed a fresh executor before rejection");
		final restored = newExecutor(scenario);
		require(restored.restore(saved), "complete CaxeFlow snapshot did not restore");
		require(sameExecutorState(executor, restored), "restored mutable CaxeFlow state diverged before continuation");

		final tickTwoInput:FlowTickInput = {
			events: [observed(EnterZone(ZONE)), observed(UseItem(PICK)), observed(Interact(BRIDGE))],
			positions: []
		};
		final tickTwo = executor.runTick(tickTwoInput);
		final restoredTickTwo = restored.runTick(tickTwoInput);
		requireNoDiagnostic(tickTwo, "coverage tick 2");
		requireNoDiagnostic(restoredTickTwo, "restored coverage tick 2");
		require(countRule(tickTwo, "rule.once.near") == 0, "once rule fired a second time");
		require(countRule(tickTwo, "rule.repeat") == 1, "repeat rule did not fire for the new event");
		require(countRule(tickTwo, "rule.cooldown") == 0, "cooldown rule fired one tick too early");
		require(countRule(tickTwo, "rule.follow.signal") == 1, "emitted signal did not arrive at the next tick");
		require(countRule(tickTwo, "rule.follow.objective") == 1, "objective change did not arrive at the next tick");
		require(countRule(tickTwo, "rule.follow.counter") == 4, "persistent counter changes were not deferred in source order");
		require(counter(executor, FOLLOW_COUNT) == 4, "deferred state-change rules produced the wrong state");
		require(coverageHash([tickTwo], executor) == coverageHash([restoredTickTwo], restored) && sameExecutorState(executor, restored),
			"restored tick 2 did not preserve deferred order, history, or state");

		final tickThree = executor.runTick({events: [observed(Interact(BRIDGE))], positions: []});
		final restoredTickThree = restored.runTick({events: [observed(Interact(BRIDGE))], positions: []});
		requireNoDiagnostic(tickThree, "coverage tick 3");
		requireNoDiagnostic(restoredTickThree, "restored coverage tick 3");
		require(countRule(tickThree, "rule.cooldown") == 1, "cooldown rule did not reopen at its exact fixed tick");
		require(countRule(tickThree, "rule.follow.timer") == 1, "scheduled timer did not become a rule event");
		require(stateValue(executor, STATE) == CLOSED.text(), "scheduled sequence did not use its captured argument");
		require(coverageHash([tickThree], executor) == coverageHash([restoredTickThree], restored)
			&& sameExecutorState(executor, restored),
			"restored tick 3 did not preserve scheduled sequence continuation");

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
			RequestCampaignExit(id("exit.coverage")),
			EmitSignal(SIGNAL),
			Schedule(TIMER, 2, SCHEDULED_SEQUENCE, [Variable(COUNTER)]),
			CallSequence(IMMEDIATE_SEQUENCE, [Variable(COUNTER)]),
			ChooseSeeded(SEED, [choice(1, [AddCounter(COUNTER, 1000)]), choice(2, [PlayEffect(SPARK, null)])])
		];
		final rules:Array<FlowRule> = [
			rule("rule.coverage-timer-owner", 1, Once, LevelEntered(id("probe.map")), Always,
				[Schedule(id("timer.coverage"), 1, SCHEDULED_SEQUENCE, [Value(Counter(0))])]),
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
		final delayed = newExecutor(scenario(delayedFlow));
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
		final edgeExecutor = newExecutor(scenario({variables: [], sequences: [edgeSequence], rules: [edgeRule]}), beforeLast);
		final scheduleFailure = edgeExecutor.runTick(oneEvent());
		expectLimitResult(scheduleFailure, FixedTickEpochs, CaxeFlowClock.MAX_EPOCH, "rule.clock-edge");
		require(scheduleFailure.presentation.length == 0, "clock-edge schedule published a partial presentation event");
		final exhausted = edgeExecutor.runTick({events: [], positions: []});
		expectLimitResult(exhausted, FixedTickEpochs, CaxeFlowClock.MAX_EPOCH, "rule.clock-edge", true);
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
		final executor = newExecutor(scenario(flow));
		requireNoDiagnostic(executor.runTick(oneEvent()), "scheduled failure setup");
		final failed = executor.runTick({events: [], positions: []});
		expectLimitResult(failed, Actions, ScenarioLimits.MAX_ACTIONS_PER_TICK, "sequence.fail-first");
		require(!flag(executor, FLAG), "later due sequence ran after an earlier due sequence exhausted the budget");
		final following = executor.runTick({events: [], positions: []});
		expectLimitResult(following, Actions, ScenarioLimits.MAX_ACTIONS_PER_TICK, "sequence.fail-first", true);
		require(sameTick(following.tick, failed.tick), "terminal failure advanced the fixed clock on a later call");
		require(!flag(executor, FLAG), "executor continued with a due-sequence suffix after a terminal failure");
	}

	static function checkBudgets():Void {
		final tooManyEvents:Array<FlowEventOccurrence> = [];
		for (_ in 0...ScenarioLimits.MAX_EVENTS_PER_TICK + 1)
			tooManyEvents.push(observed(EnterZone(ZONE)));
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
		require(actionResult.activeObjective == null, "action-budget failure published a partial active-objective projection");

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
		final malformedResult = newExecutor(scenario(malformedFlow)).runTick(oneEvent());
		require(malformedResult.diagnostics.length == 1, "malformed runtime action did not fail exactly once");
		switch malformedResult.diagnostics[0] {
			case InvalidRuntimeAction(owner):
				require(owner.text() == "rule.budget", "malformed action lost its owning rule");
			case diagnostic:
				throw 'expected invalid runtime action, got ${Std.string(diagnostic)}';
		}

		final missingObject = id("object.missing");
		final missingResult = newExecutor(scenario(emptyFlow())).runTick({
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
		final result = newExecutor(source).runTick(input);
		expectLimitResult(result, expected, maximum, expectedOwner);
		return result;
	}

	static function expectLimitResult(result:FlowTickResult, expected:FlowExecutionLimit, maximum:Int, ?expectedOwner:String, latched:Bool = false):Void {
		require(result.diagnostics.length == 1, 'expected one ${Std.string(expected)} diagnostic');
		final expectedAttempted = maximum == 2147483647 ? maximum : maximum + 1;
		require(result.failure != null
			&& result.failure.disposition == FlowFailureDisposition.TerminalFaultRetainedPrefix
			&& result.failure.attempted == expectedAttempted
			&& (latched
				|| (result.failure.completedRules == result.firedRules.length
					&& result.failure.presentationEvents == result.presentation.length
					&& result.failure.traceEntries == result.trace.length)),
			'${Std.string(expected)} lost attempted-work or retained-prefix detail');
		switch result.diagnostics[0] {
			case LimitExceeded(kind, actualMaximum, owner):
				require(kind == expected && actualMaximum == maximum, 'expected ${Std.string(expected)}($maximum), got ${Std.string(kind)}($actualMaximum)');
				final actualOwner = owner == null ? null : owner.text();
				require(actualOwner == expectedOwner, 'expected ${Std.string(expected)} owner ${Std.string(expectedOwner)}, got ${Std.string(actualOwner)}');
			case diagnostic:
				throw 'expected ${Std.string(expected)}, got ${Std.string(diagnostic)}';
		}
	}

	/** Construct the executor with the same closed content limits used in play. */
	static function newExecutor(source:Scenario, ?restoredTick:FlowTick):CaxeFlowExecutor
		return new CaxeFlowExecutor(source, new CaxeFlowProbeRegistry(), restoredTick);

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
			messages: NoMessageCatalog,
			title: ScenarioText.Literal("CaxeFlow probe"),
			mode: ScenarioMode.Adventure,
			environment: null,
			world: {
				size: {width: 4, height: 2, depth: 4},
				palette: [{code: 0, blockType: AIR}],
				chunks: [],
				fluids: []
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
				speakerNames: [],
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
		return {events: [observed(EnterZone(ZONE))], positions: []};

	/** Construct one spatial occurrence for an explicit authored actor. */
	static function spatial(source:FlowEvent, actor:ScenarioId, swept:Bool):FlowEventOccurrence {
		final previous:FlowEventPosition = {xMilli: 0, yMilli: 1000, zMilli: 0};
		final current:FlowEventPosition = {xMilli: 1000, yMilli: 1000, zMilli: 1000};
		return flowEventOccurrence(source, SpatialEventContext(actor, previous, current, swept));
	}

	/** Add the exact closed runtime context required by each focused source. */
	static function observed(source:FlowEvent):FlowEventOccurrence {
		final context:FlowEventContext = switch source {
			case EnterZone(_) | LeaveZone(_):
				final point:FlowEventPosition = {xMilli: 500, yMilli: 1000, zMilli: 500};
				SpatialEventContext(PLAYER, point, point, false);
			case Interact(_) | UseItem(_) | ItemCollected(_) | EntityDefeated(_) | BlockChanged(_, _):
				ActorEventContext(PLAYER);
			case _:
				NoEventContext;
		};
		return flowEventOccurrence(source, context);
	}

	/** Count one public trace family without depending on enum ordinals. */
	static function traceCount(result:FlowTickResult, expected:String):Int {
		var count = 0;
		for (entry in result.trace)
			switch entry {
				case EventObserved(_, _) if (expected == "event"):
					count++;
				case PredicateEvaluated(_, _, _, _) if (expected == "predicate"):
					count++;
				case ActionExecuted(_, _) if (expected == "action"):
					count++;
				case FollowUpDeferred(_, _, _) | SequenceDeferred(_, _, _, _) if (expected == "deferred"):
					count++;
				case _:
			}
		return count;
	}

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

	static function objective(objectiveId:ScenarioId, state:ObjectiveState):caxecraft.scenario.ScenarioStory.ScenarioObjective
		return {
			id: objectiveId,
			title: ScenarioText.Literal(objectiveId.text()),
			body: ScenarioText.Literal(objectiveId.text()),
			initialState: state
		};

	static function position(objectId:ScenarioId, x:Int, y:Int, z:Int):caxecraft.scenario.CaxeFlowRuntime.FlowPosition
		return {
			objectId: objectId,
			xMilli: x,
			yMilli: y,
			zMilli: z
		};

	/** Compare two exact fixed-clock values without relying on record identity. */
	static inline function sameTick(left:FlowTick, right:FlowTick):Bool
		return left.epoch == right.epoch && left.offset == right.offset;

	/** Compare one object's saved state across an expected atomic rejection. */
	static function sameObjectSnapshot(left:CaxeFlowSnapshot, right:CaxeFlowSnapshot, objectId:ScenarioId):Bool {
		for (leftObject in left.state.objects)
			if (leftObject.id.text() == objectId.text())
				for (rightObject in right.state.objects)
					if (rightObject.id.text() == objectId.text())
						return leftObject.active == rightObject.active
							&& leftObject.state == rightObject.state
							&& leftObject.hasPosition == rightObject.hasPosition
							&& leftObject.xMilli == rightObject.xMilli
							&& leftObject.yMilli == rightObject.yMilli
							&& leftObject.zMilli == rightObject.zMilli;
		return false;
	}

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

	static function objectiveName(result:FlowTickResult):String {
		final active = result.activeObjective;
		return active == null ? "" : active.text();
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

	/** Compare every public mutable-state projection exercised by coverage rules. */
	static function sameExecutorState(left:CaxeFlowExecutor, right:CaxeFlowExecutor):Bool
		return counter(left, COUNTER) == counter(right, COUNTER)
			&& counter(left, FOLLOW_COUNT) == counter(right, FOLLOW_COUNT)
			&& counter(left, SEED) == counter(right, SEED)
			&& flag(left, FLAG) == flag(right, FLAG)
			&& stateValue(left, STATE) == stateValue(right, STATE)
			&& left.inventoryQuantity(PLAYER, PICK) == right.inventoryQuantity(PLAYER, PICK)
			&& left.objectActive(BROWSER) == right.objectActive(BROWSER)
			&& left.objectState(BRIDGE).text() == right.objectState(BRIDGE).text()
			&& left.objectiveState(OBJECTIVE) == right.objectiveState(OBJECTIVE)
			&& left.hasJournal(JOURNAL) == right.hasJournal(JOURNAL)
			&& left.checkpoint().text() == right.checkpoint().text();

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
			case CampaignExitRequested(exit): 'campaign-exit:${exit.text()}';
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

/** Closed content facts used by the executor's independent runtime checks. */
private final class CaxeFlowProbeRegistry implements ScenarioContentRegistry {
	public function new() {}

	public function supportsFeature(id:ContentId):Bool
		return id.text() == "caxecraft:core";

	public function isAirBlock(id:ContentId):Bool
		return id.text() == "caxecraft:air";

	public function hasBlock(id:ContentId):Bool
		return id.text() == "caxecraft:air" || id.text() == "caxecraft:bedrock";

	public function blockStorageCode(id:ContentId):Int
		return id.text() == "caxecraft:air" ? 0 : id.text() == "caxecraft:bedrock" ? 1 : -1;

	public function blockContentIdForStorageCode(code:Int):Null<ContentId>
		return code == 0 ? new ContentId("caxecraft:air") : code == 1 ? new ContentId("caxecraft:bedrock") : null;

	public function hasFluid(id:ContentId):Bool
		return false;

	public function hasItem(id:ContentId):Bool
		return id.text() == "caxecraft:haxe-pick";

	public function itemStorageCode(id:ContentId):Int
		return hasItem(id) ? 0 : -1;

	public function hasEntity(id:ContentId):Bool
		return id.text() == "caxecraft:browser";

	public function hasNpc(id:ContentId):Bool
		return id.text() == "caxecraft:ivvy";

	public function hasPrefab(id:ContentId):Bool
		return false;

	public function hasStatefulObject(id:ContentId):Bool
		return id.text() == "caxecraft:bridge";

	public function hasState(id:ContentId):Bool
		return id.text() == "caxecraft:closed" || id.text() == "caxecraft:open" || id.text() == "caxecraft:broken";

	public function statefulObjectHasState(objectType:ContentId, state:ContentId):Bool
		return hasStatefulObject(objectType) && (state.text() == "caxecraft:closed" || state.text() == "caxecraft:open");

	public function hasEffect(id:ContentId):Bool
		return id.text() == "caxecraft:spark";

	public function hasSignal(id:ContentId):Bool
		return id.text() == "caxecraft:bridge-lowered" || id.text() == "caxecraft:coverage-signal";

	public function maximumItemQuantity(id:ContentId):Int
		return hasItem(id) ? 64 : 0;
}

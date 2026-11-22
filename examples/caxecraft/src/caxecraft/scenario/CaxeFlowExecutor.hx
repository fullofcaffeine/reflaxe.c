package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowEventOccurrence;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionConsumer;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionAllowed;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionDescriptor;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionId;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionMatchesDescriptor;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventActor;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventContextMatches;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventId;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventOccurrence;
import caxecraft.scenario.CaxeFlowEventOwnership.flowOwnsCampaignExit;
import caxecraft.scenario.CaxeFlowEventOwnership.flowOwnsTimer;
import caxecraft.scenario.CaxeFlowRuntime.FlowExecutionLimit;
import caxecraft.scenario.CaxeFlowRuntime.FlowFailure;
import caxecraft.scenario.CaxeFlowRuntime.FlowFailureDisposition;
import caxecraft.scenario.CaxeFlowRuntime.FlowPresentationEvent;
import caxecraft.scenario.CaxeFlowRuntime.FlowRuntimeDiagnostic;
import caxecraft.scenario.CaxeFlowRuntime.FlowTick;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickInput;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickResult;
import caxecraft.scenario.CaxeFlowRuntime.FlowTraceEntry;
import caxecraft.scenario.CaxeFlowSnapshot.CaxeFlowSnapshot;
import caxecraft.scenario.CaxeFlowSnapshot.FlowDeferredSnapshot;

private enum DeferredFlowWork {
	DeferredEvent(readyTick:FlowTick, event:FlowEventOccurrence);
	DeferredSequence(readyTick:FlowTick, timer:ScenarioId, sequence:ScenarioId, arguments:Array<FlowValue>);
}

private final class ReadySequence {
	public final sequence:ScenarioId;
	public final arguments:Array<FlowValue>;

	public function new(sequence:ScenarioId, arguments:Array<FlowValue>) {
		this.sequence = sequence;
		this.arguments = arguments;
	}
}

/**
	Runs the small CaxeFlow WHEN / IF / DO language at fixed tick boundaries.

	The executor is target-neutral: it changes typed scenario state and returns
	typed presentation requests, but never calls Raylib, the filesystem, or a C
	escape hatch. A game shell can therefore use this same implementation in the
	Eval oracle and in generated C.

	Every matching predicate is evaluated before any admitted rule action runs.
	That is the "stable tick view": an earlier rule cannot change what a later
	rule observed during the same tick. Actions still run in deterministic order
	and their already-completed prefix remains visible in the failed result if a
	runtime budget stops the tick. The terminal fault latch then prevents that
	prefix from becoming an implicit continuation point.
**/
final class CaxeFlowExecutor {
	final scenario:Scenario;
	final registry:ScenarioContentRegistry;
	var state:CaxeFlowState;
	var rulePlanner:CaxeFlowRulePlanner;
	var deferred:Array<DeferredFlowWork> = [];
	var currentTick:FlowTick = CaxeFlowClock.start();

	var presentation:Array<FlowPresentationEvent> = [];
	var diagnostics:Array<FlowRuntimeDiagnostic> = [];
	var runtimeFault:Null<FlowRuntimeDiagnostic> = null;
	var runtimeFailure:Null<FlowFailure> = null;
	var tickFailure:Null<FlowFailure> = null;
	var firedRules:Array<ScenarioId> = [];
	var trace:Array<FlowTraceEntry> = [];
	var actionCount:Int = 0;
	var sequenceCallCount:Int = 0;
	var spawnCount:Int = 0;
	var scheduledCount:Int = 0;

	/**
	 * Create a rule executor from a scenario and its validating content registry.
	 *
	 * Normal play starts at zero. The optional value is the narrow restoration
	 * seam used by future save-game loading and by boundary tests that cannot
	 * advance a billion years one tick at a time.
	 */
	public function new(scenario:Scenario, registry:ScenarioContentRegistry, ?restoredTick:FlowTick) {
		this.scenario = scenario;
		this.registry = registry;
		if (restoredTick != null) {
			if (!CaxeFlowClock.isValid(restoredTick))
				throw "CaxeFlow restored tick is outside the fixed clock";
			currentTick = restoredTick;
		}
		state = new CaxeFlowState(scenario, registry);
		rulePlanner = new CaxeFlowRulePlanner(scenario, state);
	}

	/**
		Advance one fixed simulation tick.

		Tick numbers begin at one. Deferred signals and state notifications become
		input no earlier than the next call. A delay of one therefore means exactly
		one fixed-tick boundary, without consulting wall-clock time. A diagnostic
		latches a terminal runtime fault: later calls return that same fault without
		advancing or consuming more work. A validated restore is the explicit recovery
		boundary. This keeps a partially completed action prefix from becoming a new
		implicit starting state while avoiding a full-state copy on every healthy tick.
	**/
	public function runTick(input:FlowTickInput):FlowTickResult {
		resetTickOutput();
		if (runtimeFault != null) {
			diagnostics.push(runtimeFault);
			tickFailure = runtimeFailure;
			return result();
		}
		final nextTick = CaxeFlowClock.next(currentTick);
		if (nextTick == null) {
			fail(LimitExceeded(FixedTickEpochs, CaxeFlowClock.MAX_EPOCH, null));
			return result();
		}
		final unknownPosition = state.validatePositions(input.positions);
		if (unknownPosition != null) {
			fail(InvalidRuntimeReference(unknownPosition));
			return result();
		}

		final events:Array<FlowEventOccurrence> = [];
		final readySequences:Array<ReadySequence> = [];
		final pending:Array<DeferredFlowWork> = [];
		for (work in deferred)
			switch work {
				case DeferredEvent(readyTick, event) if (CaxeFlowClock.isDue(readyTick, nextTick)):
					events.push(event);
				case DeferredSequence(readyTick, timer, sequence, arguments) if (CaxeFlowClock.isDue(readyTick, nextTick)):
					events.push(flowEventOccurrence(TimerExpired(timer)));
					readySequences.push(new ReadySequence(sequence, arguments));
				case _:
					pending.push(work);
			}
		for (event in input.events)
			events.push(event);
		if (events.length > ScenarioLimits.MAX_EVENTS_PER_TICK) {
			fail(LimitExceeded(TickEvents, ScenarioLimits.MAX_EVENTS_PER_TICK, null), events.length);
			return result();
		}
		for (event in events) {
			if (!acceptsRuntimeEvent(event)) {
				fail(InvalidRuntimeEvent(flowEventId(event.source)));
				return result();
			}
		}
		currentTick = nextTick;
		state.updatePositions(input.positions);
		for (event in events)
			if (!recordTrace(EventObserved(flowEventId(event.source), flowEventActor(event))))
				return result();
		final admission = rulePlanner.admit(events, currentTick);
		if (admission.diagnostic != null) {
			fail(admission.diagnostic);
			return result();
		}
		for (entry in admission.trace)
			if (!recordTrace(entry))
				return result();
		// Ready work is consumed only after the complete stable-state admission
		// pass succeeds. A predicate or rule budget failure can therefore be
		// inspected without accidentally discarding a due sequence.
		deferred = pending;

		for (ready in readySequences) {
			executeSequence(ready.sequence, ready.arguments, ready.sequence, 1);
			if (hasFailed())
				return result();
		}
		for (admitted in admission.rules) {
			final rule = admitted.rule;
			rulePlanner.markFired(rule, admitted.occurrence, currentTick);
			firedRules.push(rule.id);
			executeActions(rule.actions, null, rule.id, 0);
			if (hasFailed())
				break;
		}
		return result();
	}

	public inline function tick():FlowTick
		return currentTick;

	/** Return the terminal runtime fault, or null while ticks remain admissible. */
	public inline function fault():Null<FlowRuntimeDiagnostic>
		return runtimeFault;

	public inline function variable(id:ScenarioId):Null<FlowValue>
		return state.variable(id);

	public inline function inventoryQuantity(owner:ScenarioId, itemType:ContentId):Int
		return state.inventoryQuantity(owner, itemType);

	public inline function objectActive(id:ScenarioId):Bool
		return state.objectActive(id);

	public inline function objectState(id:ScenarioId):Null<ContentId>
		return state.objectState(id);

	public inline function objectiveState(id:ScenarioId):Null<ScenarioStory.ObjectiveState>
		return state.objectiveState(id);

	public inline function hasJournal(id:ScenarioId):Bool
		return state.hasJournal(id);

	public inline function checkpoint():Null<ScenarioId>
		return state.currentCheckpoint();

	/**
		Validate one adapter or save-owned occurrence against this exact scenario.

		The check covers both the event's required context shape and every scenario
		identity carried by its source or actor context. Game-session persistence uses
		this same boundary before it installs pending events, so restored input cannot
		bypass the executor's normal fail-closed admission rule.
	**/
	public function acceptsRuntimeEvent(event:FlowEventOccurrence):Bool
		return flowEventContextMatches(event.source, event.context) && eventReferencesScenario(event);

	/** Return every deterministic executor fact as a copy-owned typed value. */
	public function snapshot():CaxeFlowSnapshot {
		final savedDeferred:Array<FlowDeferredSnapshot> = [];
		for (work in deferred)
			switch work {
				case DeferredEvent(readyTick, event):
					savedDeferred.push(DeferredEventSnapshot(readyTick, event));
				case DeferredSequence(readyTick, timer, sequence, arguments):
					savedDeferred.push(DeferredSequenceSnapshot(readyTick, timer, sequence, arguments.copy()));
			}
		return {
			scenario: scenario.id,
			tick: currentTick,
			state: state.snapshot(),
			ruleHistory: rulePlanner.snapshot(),
			deferred: savedDeferred
		};
	}

	/**
		Atomically restore a snapshot for this exact validated scenario.

		Candidate state and history are built separately. A stale ID, malformed event
		context, incompatible sequence argument, due work item, or invalid clock leaves
		the live executor unchanged and returns false. A successful restore also clears
		a latched runtime fault, making save restoration the sole explicit recovery path.
	**/
	public function restore(snapshot:CaxeFlowSnapshot):Bool {
		if (!sameId(snapshot.scenario, scenario.id)
			|| !CaxeFlowClock.isValid(snapshot.tick)
			|| snapshot.deferred.length > ScenarioLimits.MAX_DEFERRED_EVENTS)
			return false;
		final restoredState = new CaxeFlowState(scenario, registry);
		if (!restoredState.restore(snapshot.state))
			return false;
		final restoredPlanner = new CaxeFlowRulePlanner(scenario, restoredState);
		if (!restoredPlanner.restore(snapshot.ruleHistory, snapshot.tick))
			return false;
		final restoredDeferred:Array<DeferredFlowWork> = [];
		for (saved in snapshot.deferred)
			switch saved {
				case DeferredEventSnapshot(readyTick, event):
					if (!savedTickIsFuture(readyTick, snapshot.tick) || !eventCanBeDeferred(event.source) || !acceptsRuntimeEvent(event))
						return false;
					restoredDeferred.push(DeferredEvent(readyTick, event));
				case DeferredSequenceSnapshot(readyTick, timer, sequenceId, arguments):
					final sequence = findSequence(sequenceId);
					if (!savedTickIsFuture(readyTick, snapshot.tick) || sequence == null || !sequenceAcceptsArguments(sequence, arguments))
						return false;
					restoredDeferred.push(DeferredSequence(readyTick, timer, sequenceId, arguments.copy()));
			}
		state = restoredState;
		rulePlanner = restoredPlanner;
		deferred = restoredDeferred;
		currentTick = snapshot.tick;
		runtimeFault = null;
		runtimeFailure = null;
		resetTickOutput();
		return true;
	}

	function executeActions(actions:Array<FlowAction>, frame:Null<CaxeFlowFrame>, owner:ScenarioId, depth:Int):Void {
		for (action in actions) {
			if (actionCount >= ScenarioLimits.MAX_ACTIONS_PER_TICK) {
				fail(LimitExceeded(Actions, ScenarioLimits.MAX_ACTIONS_PER_TICK, owner), actionCount + 1);
				return;
			}
			actionCount++;
			if (!recordTrace(ActionExecuted(owner, flowActionId(action))))
				return;
			executeAction(action, frame, owner, depth);
			if (hasFailed())
				return;
		}
	}

	function executeAction(action:FlowAction, frame:Null<CaxeFlowFrame>, owner:ScenarioId, depth:Int):Void {
		final descriptor = flowActionDescriptor(action);
		if (!flowActionAllowed(descriptor, FlowActionConsumer.CaxeFlowDocument) || !flowActionMatchesDescriptor(action, descriptor)) {
			fail(InvalidRuntimeAction(owner));
			return;
		}
		switch action {
			case ShowDialogue(dialogue):
				presentation.push(DialogueRequested(dialogue));
			case AddJournal(entry):
				if (state.addJournal(entry))
					presentation.push(JournalAdded(entry));
			case SetFlag(variable, value):
				setTypedVariable(variable, Flag(value), frame, owner);
			case SetCounter(variable, value):
				setTypedVariable(variable, Counter(value), frame, owner);
			case AddCounter(variable, delta):
				switch resolveVariable(variable, frame) {
					case Counter(value): setTypedVariable(variable, Counter(value + delta), frame, owner);
					case _: fail(InvalidRuntimeReference(variable));
				}
			case SetState(variable, value):
				setTypedVariable(variable, State(value), frame, owner);
			case GiveItem(inventoryOwner, itemType, quantity):
				final current = state.inventoryQuantity(inventoryOwner, itemType);
				final maximum = registry.maximumItemQuantity(itemType);
				if (quantity <= 0 || maximum <= 0 || current > maximum - quantity) {
					fail(InvalidRuntimeAction(owner));
					return;
				}
				final next = current + quantity;
				if (!state.setInventory(inventoryOwner, itemType, next)) {
					fail(InvalidRuntimeAction(owner));
					return;
				}
				presentation.push(InventoryChanged(inventoryOwner, itemType, next));
			case TakeItem(inventoryOwner, itemType, quantity):
				final maximum = registry.maximumItemQuantity(itemType);
				if (quantity <= 0 || maximum <= 0 || quantity > maximum) {
					fail(InvalidRuntimeAction(owner));
					return;
				}
				final current = state.inventoryQuantity(inventoryOwner, itemType);
				final next = quantity >= current ? 0 : current - quantity;
				if (!state.setInventory(inventoryOwner, itemType, next)) {
					fail(InvalidRuntimeAction(owner));
					return;
				}
				presentation.push(InventoryChanged(inventoryOwner, itemType, next));
			case Spawn(objectId):
				if (spawnCount >= ScenarioLimits.MAX_SPAWNED_OBJECTS_PER_TICK) {
					fail(LimitExceeded(SpawnedObjects, ScenarioLimits.MAX_SPAWNED_OBJECTS_PER_TICK, owner), spawnCount + 1);
					return;
				}
				spawnCount++;
				if (!state.setObjectActive(objectId, true)) {
					fail(InvalidRuntimeReference(objectId));
					return;
				}
				presentation.push(ObjectSpawned(objectId));
			case Despawn(objectId):
				if (!state.setObjectActive(objectId, false)) {
					fail(InvalidRuntimeReference(objectId));
					return;
				}
				presentation.push(ObjectDespawned(objectId));
			case SetObjectState(objectId, value):
				if (!state.setObjectState(objectId, value)) {
					fail(InvalidRuntimeReference(objectId));
					return;
				}
				presentation.push(ObjectStateChanged(objectId, value));
			case SetCheckpoint(checkpoint):
				state.setCheckpoint(checkpoint);
				presentation.push(CheckpointChanged(checkpoint));
			case SetObjective(objective, value):
				final previous = state.objectiveState(objective);
				if (previous == null || !state.setObjectiveState(objective, value)) {
					fail(InvalidRuntimeReference(objective));
					return;
				}
				if (previous != value) {
					presentation.push(ObjectiveChanged(objective, value));
					enqueueEventAfter(1, ObjectiveChanged(objective), owner);
				}
			case PlayEffect(effect, objectId):
				presentation.push(EffectRequested(effect, objectId));
			case RequestCampaignExit(exit):
				presentation.push(CampaignExitRequested(exit));
				enqueueEventAfter(1, FlowEvent.CampaignExitRequested(exit), owner);
			case EmitSignal(signal):
				enqueueEventAfter(1, SignalReceived(signal), owner);
			case Schedule(timer, ticks, sequence, arguments):
				if (ticks <= 0) {
					fail(InvalidRuntimeAction(owner));
					return;
				}
				if (scheduledCount >= ScenarioLimits.MAX_SCHEDULED_WORK_PER_TICK) {
					fail(LimitExceeded(ScheduledWork, ScenarioLimits.MAX_SCHEDULED_WORK_PER_TICK, owner), scheduledCount + 1);
					return;
				}
				final captured = resolveArguments(arguments, frame);
				if (captured == null)
					return;
				final readyTick = CaxeFlowClock.dueTick(currentTick, ticks);
				if (readyTick == null) {
					fail(LimitExceeded(FixedTickEpochs, CaxeFlowClock.MAX_EPOCH, owner));
					return;
				}
				scheduledCount++;
				enqueue(DeferredSequence(readyTick, timer, sequence, captured), owner);
			case CallSequence(sequence, arguments):
				final resolved = resolveArguments(arguments, frame);
				if (resolved == null)
					return;
				executeSequence(sequence, resolved, owner, depth + 1);
			case ChooseSeeded(seedVariable, choices):
				final seed = switch resolveVariable(seedVariable, frame) {
					case Counter(value): value;
					case _:
						fail(InvalidRuntimeReference(seedVariable));
						return;
				}
				var totalWeight = 0;
				for (choice in choices) {
					if (choice.weight <= 0 || totalWeight > 2147483647 - choice.weight) {
						fail(InvalidRuntimeAction(owner));
						return;
					}
					totalWeight += choice.weight;
				}
				if (totalWeight == 0) {
					fail(InvalidRuntimeAction(owner));
					return;
				}
				var selected = seed % totalWeight;
				if (selected < 0)
					selected += totalWeight;
				final nextSeed = seed == 2147483647 ? -2147483647 - 1 : seed + 1;
				setTypedVariable(seedVariable, Counter(nextSeed), frame, owner);
				if (hasFailed())
					return;
				for (choice in choices) {
					if (selected < choice.weight) {
						executeActions(choice.actions, frame, owner, depth);
						return;
					}
					selected -= choice.weight;
				}
		}
	}

	function executeSequence(id:ScenarioId, arguments:Array<FlowValue>, owner:ScenarioId, depth:Int):Void {
		if (depth > ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH) {
			fail(LimitExceeded(SequenceDepth, ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH, owner), depth);
			return;
		}
		if (sequenceCallCount >= ScenarioLimits.MAX_SEQUENCE_CALLS_PER_TICK) {
			fail(LimitExceeded(SequenceCalls, ScenarioLimits.MAX_SEQUENCE_CALLS_PER_TICK, owner), sequenceCallCount + 1);
			return;
		}
		final sequence = findSequence(id);
		if (sequence == null || !sequenceAcceptsArguments(sequence, arguments)) {
			fail(InvalidRuntimeReference(id));
			return;
		}
		sequenceCallCount++;
		final frame = new CaxeFlowFrame(scenario, sequence, arguments);
		executeActions(sequence.actions, frame, sequence.id, depth);
	}

	function resolveArguments(arguments:Array<FlowArgument>, frame:Null<CaxeFlowFrame>):Null<Array<FlowValue>> {
		final result:Array<FlowValue> = [];
		for (argument in arguments)
			switch argument {
				case Value(value):
					result.push(value);
				case Variable(variable):
					final value = resolveVariable(variable, frame);
					if (value == null) {
						fail(InvalidRuntimeReference(variable));
						return null;
					}
					result.push(value);
			}
		return result;
	}

	function resolveVariable(id:ScenarioId, frame:Null<CaxeFlowFrame>):Null<FlowValue> {
		if (frame != null && frame.contains(id))
			return frame.value(id);
		return state.variable(id);
	}

	function setTypedVariable(id:ScenarioId, value:FlowValue, frame:Null<CaxeFlowFrame>, owner:ScenarioId):Void {
		final previous = resolveVariable(id, frame);
		if (previous == null || !sameValueKind(previous, value)) {
			fail(InvalidRuntimeReference(id));
			return;
		}
		if (flowValuesEqual(previous, value))
			return;
		final isLocal = frame != null && frame.contains(id);
		final changed = isLocal ? frame.set(id, value) : state.setVariable(id, value);
		if (!changed) {
			fail(InvalidRuntimeReference(id));
			return;
		}
		presentation.push(VariableChanged(id, value));
		if (!isLocal)
			enqueueEventAfter(1, StateChanged(id), owner);
	}

	function enqueueEventAfter(delay:Int, event:FlowEvent, owner:ScenarioId):Void {
		final readyTick = CaxeFlowClock.dueTick(currentTick, delay);
		if (readyTick == null) {
			fail(LimitExceeded(FixedTickEpochs, CaxeFlowClock.MAX_EPOCH, owner));
			return;
		}
		if (!recordTrace(FollowUpDeferred(owner, flowEventId(event), readyTick)))
			return;
		enqueue(DeferredEvent(readyTick, flowEventOccurrence(event)), owner);
	}

	function enqueue(work:DeferredFlowWork, owner:ScenarioId):Void {
		if (deferred.length >= ScenarioLimits.MAX_DEFERRED_EVENTS) {
			fail(LimitExceeded(DeferredWork, ScenarioLimits.MAX_DEFERRED_EVENTS, owner), deferred.length + 1);
			return;
		}
		switch work {
			case DeferredSequence(readyTick, timer, sequence, _):
				if (!recordTrace(SequenceDeferred(owner, timer, sequence, readyTick)))
					return;
			case DeferredEvent(_, _):
		}
		deferred.push(work);
	}

	function findSequence(id:ScenarioId):Null<FlowSequence> {
		for (sequence in scenario.flow.sequences)
			if (sameId(sequence.id, id))
				return sequence;
		return null;
	}

	/** Deferred work must remain strictly after the captured fixed-tick boundary. */
	static function savedTickIsFuture(ready:FlowTick, current:FlowTick):Bool
		return CaxeFlowClock.isValid(ready) && !CaxeFlowClock.isDue(ready, current);

	/**
		Reject stale semantic IDs before deferred engine events re-enter execution.

		Content IDs were validated with the scenario and remain closed values. Object,
		objective, variable, actor, and level references must still belong to this map.
	**/
	function eventReferencesScenario(event:FlowEventOccurrence):Bool {
		final sourceMatches = switch event.source {
			case EnterZone(zone) | LeaveZone(zone): scenarioHasTriggerZone(zone);
			case Interact(objectId) | EntityDefeated(objectId): scenarioHasObject(objectId);
			case BlockChanged(zone, _): scenarioHasTriggerZone(zone);
			case ObjectiveChanged(objective): scenarioHasObjective(objective);
			case StateChanged(variable): scenarioHasVariable(variable);
			case LevelEntered(level): sameId(level, scenario.id);
			case TimerExpired(timer): flowOwnsTimer(scenario.flow, timer);
			case CampaignExitRequested(exit): flowOwnsCampaignExit(scenario.flow, exit);
			case UseItem(_) | ItemCollected(_) | SignalReceived(_): true;
		};
		if (!sourceMatches)
			return false;
		final actor = flowEventActor(event);
		return actor == null || scenarioHasActor(actor);
	}

	/** Only executor-produced follow-up sources may survive in deferred storage. */
	static function eventCanBeDeferred(source:FlowEvent):Bool
		return switch source {
			case SignalReceived(_) | ObjectiveChanged(_) | StateChanged(_) | CampaignExitRequested(_): true;
			case _: false;
		};

	/** True when an event reference names any object in this scenario. */
	function scenarioHasObject(id:ScenarioId):Bool {
		for (object in scenario.objects)
			if (sameId(object.id, id))
				return true;
		return false;
	}

	/** True only for an authored placement that can carry runtime actor context. */
	function scenarioHasActor(id:ScenarioId):Bool {
		for (object in scenario.objects)
			if (sameId(object.id, id))
				return switch object.placement {
					case ScenarioObject.ObjectPlacement.PlayerSpawn(_) | ScenarioObject.ObjectPlacement.Entity(_,
						_) | ScenarioObject.ObjectPlacement.Npc(_, _, _): true;
					case _: false;
				};
		return false;
	}

	/** True only for a trigger-volume object. */
	function scenarioHasTriggerZone(id:ScenarioId):Bool {
		for (object in scenario.objects)
			if (sameId(object.id, id))
				return switch object.placement {
					case ScenarioObject.ObjectPlacement.TriggerZone(_): true;
					case _: false;
				};
		return false;
	}

	/** True when one objective survives in this exact authored map. */
	function scenarioHasObjective(id:ScenarioId):Bool {
		for (objective in scenario.story.objectives)
			if (sameId(objective.id, id))
				return true;
		return false;
	}

	/** True when one non-local variable survives in this exact authored map. */
	function scenarioHasVariable(id:ScenarioId):Bool {
		for (variable in scenario.flow.variables)
			if (sameId(variable.id, id))
				return true;
		return false;
	}

	static function sequenceAcceptsArguments(sequence:FlowSequence, arguments:Array<FlowValue>):Bool {
		if (sequence.parameters.length != arguments.length)
			return false;
		for (index in 0...arguments.length)
			if (!sameValueKind(sequence.parameters[index].initial, arguments[index]))
				return false;
		return true;
	}

	function resetTickOutput():Void {
		presentation = [];
		diagnostics = [];
		tickFailure = null;
		firedRules = [];
		trace = [];
		actionCount = 0;
		sequenceCallCount = 0;
		spawnCount = 0;
		scheduledCount = 0;
	}

	function result():FlowTickResult
		return {
			tick: currentTick,
			firedRules: firedRules,
			presentation: presentation,
			activeObjective: hasFailed() ? null : state.activeObjectiveId(),
			diagnostics: diagnostics,
			failure: tickFailure,
			trace: trace
		};

	/** Append one trace entry or fail before unbounded explanation state grows. */
	function recordTrace(entry:FlowTraceEntry):Bool {
		if (trace.length >= ScenarioLimits.MAX_FLOW_TRACE_ENTRIES_PER_TICK) {
			fail(LimitExceeded(TraceEntries, ScenarioLimits.MAX_FLOW_TRACE_ENTRIES_PER_TICK, null), trace.length + 1);
			return false;
		}
		trace.push(entry);
		return true;
	}

	function fail(diagnostic:FlowRuntimeDiagnostic, ?attempted:Int):Void {
		if (!hasFailed()) {
			final rejected = attempted == null ? switch diagnostic {
				case LimitExceeded(_, maximum, _): maximum == 2147483647 ? maximum : maximum + 1;
				case _: 0;
			} : attempted;
			final detail:FlowFailure = {
				diagnostic: diagnostic,
				disposition: TerminalFaultRetainedPrefix,
				attempted: rejected,
				completedRules: firedRules.length,
				completedActions: actionCount,
				presentationEvents: presentation.length,
				traceEntries: trace.length
			};
			diagnostics.push(diagnostic);
			runtimeFault = diagnostic;
			runtimeFailure = detail;
			tickFailure = detail;
		}
	}

	inline function hasFailed():Bool
		return diagnostics.length != 0;

	static function sameValueKind(left:FlowValue, right:FlowValue):Bool
		return switch [left, right] {
			case [Flag(_), Flag(_)] | [Counter(_), Counter(_)] | [State(_), State(_)]: true;
			case _: false;
		};

	static function flowValuesEqual(left:FlowValue, right:FlowValue):Bool
		return switch [left, right] {
			case [Flag(leftValue), Flag(rightValue)]: leftValue == rightValue;
			case [Counter(leftValue), Counter(rightValue)]: leftValue == rightValue;
			case [State(leftValue), State(rightValue)]: sameContent(leftValue, rightValue);
			case _: false;
		};

	static inline function sameId(left:ScenarioId, right:ScenarioId):Bool
		return left.text() == right.text();

	static inline function sameContent(left:ContentId, right:ContentId):Bool
		return left.text() == right.text();
}

package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlowRuntime.FlowExecutionLimit;
import caxecraft.scenario.CaxeFlowRuntime.FlowPresentationEvent;
import caxecraft.scenario.CaxeFlowRuntime.FlowRuntimeDiagnostic;
import caxecraft.scenario.CaxeFlowRuntime.FlowTick;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickInput;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickResult;

private enum DeferredFlowWork {
	DeferredEvent(readyTick:FlowTick, event:FlowEvent);
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
	and their already-completed prefix remains visible if a runtime budget stops
	the tick.
**/
final class CaxeFlowExecutor {
	final scenario:Scenario;
	final state:CaxeFlowState;
	final rulePlanner:CaxeFlowRulePlanner;
	var deferred:Array<DeferredFlowWork> = [];
	var currentTick:FlowTick = CaxeFlowClock.start();

	var presentation:Array<FlowPresentationEvent> = [];
	var diagnostics:Array<FlowRuntimeDiagnostic> = [];
	var firedRules:Array<ScenarioId> = [];
	var actionCount:Int = 0;
	var sequenceCallCount:Int = 0;
	var spawnCount:Int = 0;
	var scheduledCount:Int = 0;

	/**
	 * Create a rule executor, optionally at an exact validated saved tick.
	 *
	 * Normal play starts at zero. The optional value is the narrow restoration
	 * seam used by future save-game loading and by boundary tests that cannot
	 * advance a billion years one tick at a time.
	 */
	public function new(scenario:Scenario, ?restoredTick:FlowTick) {
		this.scenario = scenario;
		if (restoredTick != null) {
			if (!CaxeFlowClock.isValid(restoredTick))
				throw "CaxeFlow restored tick is outside the fixed clock";
			currentTick = restoredTick;
		}
		state = new CaxeFlowState(scenario);
		rulePlanner = new CaxeFlowRulePlanner(scenario, state);
	}

	/**
		Advance one fixed simulation tick.

		Tick numbers begin at one. Deferred signals and state notifications become
		input no earlier than the next call. A delay of one therefore means exactly
		one fixed-tick boundary, without consulting wall-clock time.
	**/
	public function runTick(input:FlowTickInput):FlowTickResult {
		resetTickOutput();
		final nextTick = CaxeFlowClock.next(currentTick);
		if (nextTick == null) {
			fail(LimitExceeded(FixedTickEpochs, CaxeFlowClock.MAX_EPOCH, null));
			return result();
		}
		currentTick = nextTick;

		final unknownPosition = state.updatePositions(input.positions);
		if (unknownPosition != null) {
			fail(InvalidRuntimeReference(unknownPosition));
			return result();
		}

		final events:Array<FlowEvent> = [];
		final readySequences:Array<ReadySequence> = [];
		final pending:Array<DeferredFlowWork> = [];
		for (work in deferred)
			switch work {
				case DeferredEvent(readyTick, event) if (CaxeFlowClock.isDue(readyTick, currentTick)):
					events.push(event);
				case DeferredSequence(readyTick, timer, sequence, arguments) if (CaxeFlowClock.isDue(readyTick, currentTick)):
					events.push(TimerExpired(timer));
					readySequences.push(new ReadySequence(sequence, arguments));
				case _:
					pending.push(work);
			}
		for (event in input.events)
			events.push(event);
		if (events.length > ScenarioLimits.MAX_EVENTS_PER_TICK) {
			fail(LimitExceeded(TickEvents, ScenarioLimits.MAX_EVENTS_PER_TICK, null));
			return result();
		}
		final admission = rulePlanner.admit(events, currentTick);
		if (admission.diagnostic != null) {
			fail(admission.diagnostic);
			return result();
		}
		// Ready work is consumed only after the complete stable-state admission
		// pass succeeds. A predicate or rule budget failure can therefore be
		// inspected without accidentally discarding a due sequence.
		deferred = pending;

		for (ready in readySequences) {
			executeSequence(ready.sequence, ready.arguments, ready.sequence, 1);
			if (hasFailed())
				return result();
		}
		for (rule in admission.rules) {
			rulePlanner.markFired(rule, currentTick);
			firedRules.push(rule.id);
			executeActions(rule.actions, null, rule.id, 0);
			if (hasFailed())
				break;
		}
		return result();
	}

	public inline function tick():FlowTick
		return currentTick;

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

	function executeActions(actions:Array<FlowAction>, frame:Null<CaxeFlowFrame>, owner:ScenarioId, depth:Int):Void {
		for (action in actions) {
			if (actionCount >= ScenarioLimits.MAX_ACTIONS_PER_TICK) {
				fail(LimitExceeded(Actions, ScenarioLimits.MAX_ACTIONS_PER_TICK, owner));
				return;
			}
			actionCount++;
			executeAction(action, frame, owner, depth);
			if (hasFailed())
				return;
		}
	}

	function executeAction(action:FlowAction, frame:Null<CaxeFlowFrame>, owner:ScenarioId, depth:Int):Void {
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
				if (quantity <= 0) {
					fail(InvalidRuntimeAction(owner));
					return;
				}
				final next = state.inventoryQuantity(inventoryOwner, itemType) + quantity;
				presentation.push(InventoryChanged(inventoryOwner, itemType, state.setInventory(inventoryOwner, itemType, next)));
			case TakeItem(inventoryOwner, itemType, quantity):
				if (quantity <= 0) {
					fail(InvalidRuntimeAction(owner));
					return;
				}
				final next = state.inventoryQuantity(inventoryOwner, itemType) - quantity;
				presentation.push(InventoryChanged(inventoryOwner, itemType, state.setInventory(inventoryOwner, itemType, next)));
			case Spawn(objectId):
				if (spawnCount >= ScenarioLimits.MAX_SPAWNED_OBJECTS_PER_TICK) {
					fail(LimitExceeded(SpawnedObjects, ScenarioLimits.MAX_SPAWNED_OBJECTS_PER_TICK, owner));
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
			case EmitSignal(signal):
				enqueueEventAfter(1, SignalReceived(signal), owner);
			case Schedule(timer, ticks, sequence, arguments):
				if (ticks <= 0) {
					fail(InvalidRuntimeAction(owner));
					return;
				}
				if (scheduledCount >= ScenarioLimits.MAX_SCHEDULED_WORK_PER_TICK) {
					fail(LimitExceeded(ScheduledWork, ScenarioLimits.MAX_SCHEDULED_WORK_PER_TICK, owner));
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
			fail(LimitExceeded(SequenceDepth, ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH, owner));
			return;
		}
		if (sequenceCallCount >= ScenarioLimits.MAX_SEQUENCE_CALLS_PER_TICK) {
			fail(LimitExceeded(SequenceCalls, ScenarioLimits.MAX_SEQUENCE_CALLS_PER_TICK, owner));
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
		enqueue(DeferredEvent(readyTick, event), owner);
	}

	function enqueue(work:DeferredFlowWork, owner:ScenarioId):Void {
		if (deferred.length >= ScenarioLimits.MAX_DEFERRED_EVENTS) {
			fail(LimitExceeded(DeferredWork, ScenarioLimits.MAX_DEFERRED_EVENTS, owner));
			return;
		}
		deferred.push(work);
	}

	function findSequence(id:ScenarioId):Null<FlowSequence> {
		for (sequence in scenario.flow.sequences)
			if (sameId(sequence.id, id))
				return sequence;
		return null;
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
		firedRules = [];
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
			diagnostics: diagnostics
		};

	function fail(diagnostic:FlowRuntimeDiagnostic):Void {
		if (!hasFailed())
			diagnostics.push(diagnostic);
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

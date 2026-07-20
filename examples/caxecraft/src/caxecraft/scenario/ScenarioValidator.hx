package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowValueKind;
import caxecraft.scenario.ScenarioCodecModel.ParsedScenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioCoordinate;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/** Semantic gate between a parsed candidate and a playable Scenario. */
final class ScenarioValidator {
	public static function validate(parsed:ParsedScenario, registry:ScenarioContentRegistry):ScenarioReadResult<Scenario> {
		final state = new ScenarioValidationState(parsed, registry);
		state.validate();
		return state.diagnostics.length == 0 ? ReadOk(parsed.candidate) : ReadError(state.diagnostics);
	}
}

private final class ScenarioValidationState {
	public final diagnostics:Array<ScenarioDiagnostic> = [];

	final parsed:ParsedScenario;
	final scenario:Scenario;
	final registry:ScenarioContentRegistry;
	final objectIds:Map<String, Bool> = [];
	final dialogueIds:Map<String, Bool> = [];
	final journalIds:Map<String, Bool> = [];
	final objectiveIds:Map<String, Bool> = [];
	final sequenceIds:Map<String, Bool> = [];
	final sequenceTable:Map<String, FlowSequence> = [];
	final variableKinds:Map<String, FlowValueKind> = [];

	public function new(parsed:ParsedScenario, registry:ScenarioContentRegistry) {
		this.parsed = parsed;
		this.scenario = parsed.candidate;
		this.registry = registry;
	}

	public function validate():Void {
		validateFeatures();
		validateWorld();
		indexIdentities();
		validateObjects();
		validateStory();
		validateFlow();
		validateExtensions();
	}

	function validateFeatures():Void {
		final seen:Map<String, Bool> = [];
		var core = false;
		for (feature in scenario.requiredFeatures) {
			final key = feature.text();
			if (seen.exists(key))
				add(DuplicateId(new ScenarioId(key)));
			seen.set(key, true);
			if (key == "caxecraft:core")
				core = true;
			if (!registry.supportsFeature(feature))
				add(UnknownRequiredFeature(feature));
		}
		for (feature in scenario.optionalFeatures) {
			final key = feature.text();
			if (seen.exists(key))
				add(DuplicateId(new ScenarioId(key)));
			seen.set(key, true);
		}
		if (!core)
			add(MissingRecord("feature required caxecraft:core"));
	}

	function validateWorld():Void {
		final size = scenario.world.size;
		if (size.width <= 0
			|| size.width > ScenarioLimits.MAX_WORLD_WIDTH
			|| size.height <= 0
			|| size.height > ScenarioLimits.MAX_WORLD_HEIGHT
			|| size.depth <= 0
			|| size.depth > ScenarioLimits.MAX_WORLD_DEPTH) {
			add(LimitExceeded("world-edge", ScenarioLimits.MAX_WORLD_WIDTH));
			return;
		}
		final cells = size.width * size.height * size.depth;
		if (cells <= 0 || cells > ScenarioLimits.MAX_WORLD_CELLS) {
			add(LimitExceeded("world-cells", ScenarioLimits.MAX_WORLD_CELLS));
			return;
		}
		if (scenario.world.palette.length == 0 || scenario.world.palette.length > ScenarioLimits.MAX_PALETTE_ENTRIES)
			add(LimitExceeded("palette", ScenarioLimits.MAX_PALETTE_ENTRIES));
		final palette:Map<Int, Bool> = [];
		var zeroIsAir = false;
		for (entry in scenario.world.palette) {
			if (entry.code < 0 || entry.code > 255 || palette.exists(entry.code))
				add(IntegerOutOfRange);
			palette.set(entry.code, true);
			if (!registry.hasBlock(entry.blockType))
				add(UnresolvedReference(new ScenarioId(entry.blockType.text())));
			if (entry.code == 0 && registry.isAirBlock(entry.blockType))
				zeroIsAir = true;
		}
		if (!zeroIsAir)
			add(MissingRecord("palette 0 <registered-air-block>"));

		final covered = [for (_ in 0...cells) false];
		final chunkIds:Map<String, Bool> = [];
		for (chunk in scenario.world.chunks) {
			if (chunkIds.exists(chunk.id.text()))
				add(DuplicateId(chunk.id));
			chunkIds.set(chunk.id.text(), true);
			final width = chunk.size.width;
			final height = chunk.size.height;
			final depth = chunk.size.depth;
			if (width <= 0
				|| width > ScenarioLimits.MAX_CHUNK_EDGE
				|| height <= 0
				|| height > ScenarioLimits.MAX_CHUNK_EDGE
				|| depth <= 0
				|| depth > ScenarioLimits.MAX_CHUNK_EDGE
				|| chunk.origin.x < 0
				|| chunk.origin.y < 0
				|| chunk.origin.z < 0
				|| chunk.origin.x + width > size.width
				|| chunk.origin.y + height > size.height
				|| chunk.origin.z + depth > size.depth) {
				add(ImpossiblePlacement(chunk.id));
				continue;
			}
			final expected = width * height * depth;
			var actual = 0;
			for (run in chunk.runs) {
				if (run.count <= 0 || !palette.exists(run.paletteCode) || actual > expected - run.count) {
					actual = -1;
					break;
				}
				actual += run.count;
			}
			if (actual != expected) {
				add(InvalidRunTotal(chunk.id, expected, actual));
				continue;
			}
			for (z in chunk.origin.z...chunk.origin.z + depth)
				for (y in chunk.origin.y...chunk.origin.y + height)
					for (x in chunk.origin.x...chunk.origin.x + width) {
						final cell = (z * size.height + y) * size.width + x;
						if (covered[cell])
							add(ImpossiblePlacement(chunk.id));
						covered[cell] = true;
					}
		}
		for (value in covered)
			if (!value) {
				add(MissingRecord("complete chunk coverage"));
				break;
			}
	}

	function indexIdentities():Void {
		if (scenario.objects.length > ScenarioLimits.MAX_OBJECTS)
			add(LimitExceeded("objects", ScenarioLimits.MAX_OBJECTS));
		for (object in scenario.objects)
			unique(objectIds, object.id);
		if (scenario.story.dialogues.length > ScenarioLimits.MAX_DIALOGUES)
			add(LimitExceeded("dialogues", ScenarioLimits.MAX_DIALOGUES));
		for (dialogue in scenario.story.dialogues)
			unique(dialogueIds, dialogue.id);
		for (entry in scenario.story.journal)
			unique(journalIds, entry.id);
		if (scenario.story.objectives.length > ScenarioLimits.MAX_OBJECTIVES)
			add(LimitExceeded("objectives", ScenarioLimits.MAX_OBJECTIVES));
		for (objective in scenario.story.objectives)
			unique(objectiveIds, objective.id);
		if (scenario.story.routes.length > ScenarioLimits.MAX_ROUTES)
			add(LimitExceeded("routes", ScenarioLimits.MAX_ROUTES));
		if (scenario.flow.sequences.length > ScenarioLimits.MAX_SEQUENCES)
			add(LimitExceeded("sequences", ScenarioLimits.MAX_SEQUENCES));
		for (sequence in scenario.flow.sequences) {
			unique(sequenceIds, sequence.id);
			sequenceTable.set(sequence.id.text(), sequence);
		}
		if (scenario.flow.variables.length > ScenarioLimits.MAX_VARIABLES)
			add(LimitExceeded("variables", ScenarioLimits.MAX_VARIABLES));
		for (variable in scenario.flow.variables) {
			if (variableKinds.exists(variable.id.text()))
				add(DuplicateId(variable.id));
			variableKinds.set(variable.id.text(), valueKind(variable.initial));
		}
	}

	function validateObjects():Void {
		var playerSpawns = 0;
		for (object in scenario.objects) {
			if (object.tags.length > ScenarioLimits.MAX_TAGS_PER_OBJECT)
				add(LimitExceeded("object-tags", ScenarioLimits.MAX_TAGS_PER_OBJECT));
			final tags:Map<String, Bool> = [];
			for (tag in object.tags) {
				if (tags.exists(tag.text()))
					add(DuplicateTag(tag));
				tags.set(tag.text(), true);
			}
			switch object.placement {
				case PlayerSpawn(position):
					playerSpawns++;
					validateTransform(object.id, position);
				case Checkpoint(position):
					validateTransform(object.id, position);
				case Item(item, quantity, position):
					if (!registry.hasItem(item) || quantity <= 0 || quantity > registry.maximumItemQuantity(item))
						add(ImpossiblePlacement(object.id));
					validateTransform(object.id, position);
				case Entity(entity, position):
					if (!registry.hasEntity(entity))
						add(UnresolvedReference(object.id));
					validateTransform(object.id, position);
				case Npc(npc, dialogue, position):
					if (!registry.hasNpc(npc) || !dialogueIds.exists(dialogue.text()))
						addAt(UnresolvedReference(dialogue), object.id);
					validateTransform(object.id, position);
				case Prefab(prefab, position):
					if (!registry.hasPrefab(prefab))
						add(UnresolvedReference(object.id));
					validateTransform(object.id, position);
				case TriggerZone(bounds):
					if (bounds.origin.x < 0
						|| bounds.origin.y < 0
						|| bounds.origin.z < 0
						|| bounds.size.width <= 0
						|| bounds.size.height <= 0
						|| bounds.size.depth <= 0
						|| bounds.origin.x + bounds.size.width > scenario.world.size.width
						|| bounds.origin.y + bounds.size.height > scenario.world.size.height
						|| bounds.origin.z + bounds.size.depth > scenario.world.size.depth)
						add(ImpossiblePlacement(object.id));
				case StatefulObject(kind, state, position):
					if (!registry.hasStatefulObject(kind) || !registry.hasState(state))
						add(UnresolvedReference(object.id));
					validateTransform(object.id, position);
			}
		}
		if (playerSpawns != 1)
			add(MissingRecord("exactly one player-spawn"));
	}

	function validateTransform(id:ScenarioId, value:caxecraft.scenario.ScenarioGeometry.ScenarioTransform):Void {
		if (value.xMilli < 0
			|| value.yMilli < 0
			|| value.zMilli < 0
			|| value.xMilli >= scenario.world.size.width * 1000
			|| value.yMilli >= scenario.world.size.height * 1000
			|| value.zMilli >= scenario.world.size.depth * 1000
			|| value.yawDegrees < 0
			|| value.yawDegrees > 359)
			add(ImpossiblePlacement(id));
	}

	function validateStory():Void {
		for (dialogue in scenario.story.dialogues) {
			if (dialogue.lines.length > ScenarioLimits.MAX_DIALOGUE_LINES)
				add(LimitExceeded("dialogue-lines", ScenarioLimits.MAX_DIALOGUE_LINES));
			for (line in dialogue.lines)
				if (line.speaker != null && !objectIds.exists(line.speaker.text()))
					add(UnresolvedReference(line.speaker));
		}
		for (route in scenario.story.routes)
			for (objective in route.objectives)
				if (!objectiveIds.exists(objective.text()))
					add(UnresolvedReference(objective));
	}

	function validateFlow():Void {
		if (scenario.flow.rules.length > ScenarioLimits.MAX_RULES)
			add(LimitExceeded("rules", ScenarioLimits.MAX_RULES));
		for (variable in scenario.flow.variables)
			switch variable.scope {
				case Local(sequence) if (!sequenceIds.exists(sequence.text())):
					add(UnresolvedReference(sequence));
				case _:
			}
		for (sequence in scenario.flow.sequences) {
			for (parameter in sequence.parameters)
				validateFlowValue(parameter.initial);
			for (action in sequence.actions)
				validateAction(sequence.id, action);
		}
		validateSequenceCycles();
		final ruleIds:Map<String, Bool> = [];
		for (rule in scenario.flow.rules) {
			unique(ruleIds, rule.id);
			if (rule.actions.length > ScenarioLimits.MAX_ACTIONS_PER_RULE)
				addAt(LimitExceeded("rule-actions", ScenarioLimits.MAX_ACTIONS_PER_RULE), rule.id);
			switch rule.repeat {
				case Cooldown(ticks) if (ticks <= 0):
					add(InvalidRule(rule.id));
				case _:
			}
			validateEvent(rule.id, rule.event);
			validatePredicate(rule.id, rule.predicate, 1);
			for (action in rule.actions)
				validateAction(rule.id, action);
		}
	}

	function validateExtensions():Void {
		final optional:Map<String, Bool> = [];
		for (feature in scenario.optionalFeatures)
			optional.set(feature.text(), true);
		final identities:Map<String, Bool> = [];
		for (extension in scenario.extensions) {
			final key = extension.feature.text() + ":" + extension.id.text();
			if (!optional.exists(extension.feature.text()) || identities.exists(key))
				addAt(InvalidExtension(extension.id), extension.id);
			identities.set(key, true);
		}
	}

	function validateEvent(owner:ScenarioId, value:FlowEvent):Void {
		switch value {
			case EnterZone(id), LeaveZone(id), Interact(id), EntityDefeated(id):
				if (!objectIds.exists(id.text()))
					add(InvalidRule(owner));
			case BlockChanged(zone, block):
				if (!objectIds.exists(zone.text()) || !registry.hasBlock(block))
					add(InvalidRule(owner));
			case UseItem(item):
				if (!registry.hasItem(item))
					add(InvalidRule(owner));
			case SignalReceived(signal):
				if (!registry.hasSignal(signal))
					add(InvalidRule(owner));
			case TimerExpired(_):
			case ObjectiveChanged(id):
				if (!objectiveIds.exists(id.text()))
					add(InvalidRule(owner));
			case StateChanged(id):
				if (!variableKinds.exists(id.text()))
					add(InvalidRule(owner));
		}
	}

	function validatePredicate(owner:ScenarioId, value:FlowPredicate, depth:Int):Void {
		if (depth > ScenarioLimits.MAX_PREDICATE_DEPTH) {
			add(InvalidRule(owner));
			return;
		}
		switch value {
			case Always, ModeIs(_):
			case All(children), AnyOf(children):
				for (child in children)
					validatePredicate(owner, child, depth + 1);
			case Not(child):
				validatePredicate(owner, child, depth + 1);
			case FlagIs(id, _):
				requireVariable(owner, id, FlagValue);
			case CounterCompare(id, _, _):
				requireVariable(owner, id, CounterValue);
			case StateIs(id, state):
				requireVariable(owner, id, StateValue);
				if (!registry.hasState(state))
					add(InvalidRule(owner));
			case ObjectStateIs(id, state):
				if (!objectIds.exists(id.text()) || !registry.hasState(state))
					add(InvalidRule(owner));
			case InventoryHas(id, item, _, quantity):
				if (!objectIds.exists(id.text()) || !registry.hasItem(item) || quantity < 0)
					add(InvalidRule(owner));
			case ObjectiveIs(id, _):
				if (!objectiveIds.exists(id.text()))
					add(InvalidRule(owner));
			case NearObject(actor, objectId, maximum):
				if (!objectIds.exists(actor.text()) || !objectIds.exists(objectId.text()) || maximum < 0)
					add(InvalidRule(owner));
		}
	}

	function validateAction(owner:ScenarioId, value:FlowAction):Void {
		switch value {
			case ShowDialogue(id):
				if (!dialogueIds.exists(id.text()))
					add(InvalidRule(owner));
			case AddJournal(id):
				if (!journalIds.exists(id.text()))
					add(InvalidRule(owner));
			case SetFlag(id, _):
				requireVariable(owner, id, FlagValue);
			case SetCounter(id, _), AddCounter(id, _):
				requireVariable(owner, id, CounterValue);
			case SetState(id, state):
				requireVariable(owner, id, StateValue);
				if (!registry.hasState(state))
					add(InvalidRule(owner));
			case GiveItem(id, item, quantity), TakeItem(id, item, quantity):
				if (!objectIds.exists(id.text())
					|| !registry.hasItem(item)
					|| quantity <= 0
					|| quantity > registry.maximumItemQuantity(item))
					add(InvalidRule(owner));
			case Spawn(id), Despawn(id):
				if (!objectIds.exists(id.text()))
					add(InvalidRule(owner));
			case SetObjectState(id, state):
				if (!objectIds.exists(id.text()) || !registry.hasState(state))
					add(InvalidRule(owner));
			case SetCheckpoint(id):
				if (!objectIds.exists(id.text()))
					add(InvalidRule(owner));
			case SetObjective(id, _):
				if (!objectiveIds.exists(id.text()))
					add(InvalidRule(owner));
			case PlayEffect(effect, target):
				if (!registry.hasEffect(effect) || (target != null && !objectIds.exists(target.text())))
					add(InvalidRule(owner));
			case EmitSignal(signal):
				if (!registry.hasSignal(signal))
					add(InvalidRule(owner));
			case Schedule(_, ticks, sequence, arguments):
				if (ticks <= 0)
					add(InvalidRule(owner));
				for (argument in arguments)
					validateArgument(owner, argument);
				validateSequenceCall(owner, sequence, arguments);
			case CallSequence(sequence, arguments):
				for (argument in arguments)
					validateArgument(owner, argument);
				validateSequenceCall(owner, sequence, arguments);
			case ChooseSeeded(seed, choices):
				requireVariable(owner, seed, CounterValue);
				if (choices.length == 0)
					add(InvalidRule(owner));
				for (choice in choices) {
					if (choice.weight <= 0)
						add(InvalidRule(owner));
					for (entry in choice.actions)
						validateAction(owner, entry);
				}
		}
	}

	function validateSequenceCall(owner:ScenarioId, id:ScenarioId, arguments:Array<FlowArgument>):Void {
		final sequence = sequenceTable.get(id.text());
		if (sequence == null || sequence.parameters.length != arguments.length) {
			add(InvalidRule(owner));
			return;
		}
		for (index in 0...arguments.length) {
			final expected = valueKind(sequence.parameters[index].initial);
			final actual = switch arguments[index] {
				case Value(value): valueKind(value);
				case Variable(variable): variableKinds.get(variable.text());
				case Object(_): null;
			}
			if (actual != expected)
				add(InvalidRule(owner));
		}
	}

	function validateSequenceCycles():Void {
		final visiting:Map<String, Bool> = [];
		final complete:Map<String, Bool> = [];
		for (sequence in scenario.flow.sequences)
			visitSequence(sequence, visiting, complete);
	}

	function visitSequence(sequence:FlowSequence, visiting:Map<String, Bool>, complete:Map<String, Bool>):Void {
		final key = sequence.id.text();
		if (complete.exists(key))
			return;
		if (visiting.exists(key)) {
			add(RuleCycle(sequence.id));
			return;
		}
		visiting.set(key, true);
		for (action in sequence.actions)
			visitSequenceAction(sequence.id, action, visiting, complete);
		visiting.remove(key);
		complete.set(key, true);
	}

	function visitSequenceAction(owner:ScenarioId, action:FlowAction, visiting:Map<String, Bool>, complete:Map<String, Bool>):Void {
		switch action {
			case CallSequence(id, _):
				final target = sequenceTable.get(id.text());
				if (target != null) {
					if (visiting.exists(id.text()))
						add(RuleCycle(owner));
					else
						visitSequence(target, visiting, complete);
				}
			case ChooseSeeded(_, choices):
				for (choice in choices)
					for (entry in choice.actions)
						visitSequenceAction(owner, entry, visiting, complete);
			case _:
		}
	}

	function validateArgument(owner:ScenarioId, value:FlowArgument):Void {
		switch value {
			case Value(value):
				validateFlowValue(value);
			case Variable(id):
				if (!variableKinds.exists(id.text()))
					add(InvalidRule(owner));
			case Object(id):
				if (!objectIds.exists(id.text()))
					add(InvalidRule(owner));
		}
	}

	function validateFlowValue(value:FlowValue):Void
		switch value {
			case State(state) if (!registry.hasState(state)):
				add(UnresolvedReference(new ScenarioId(state.text())));
			case _:
		}

	function requireVariable(owner:ScenarioId, id:ScenarioId, kind:FlowValueKind):Void
		if (!variableKinds.exists(id.text()) || variableKinds.get(id.text()) != kind)
			add(InvalidRule(owner));

	function valueKind(value:FlowValue):FlowValueKind
		return switch value {
			case Flag(_): FlagValue;
			case Counter(_): CounterValue;
			case State(_): StateValue;
		}

	function unique(target:Map<String, Bool>, id:ScenarioId):Void {
		if (target.exists(id.text()))
			add(DuplicateId(id));
		target.set(id.text(), true);
	}

	function add(kind:ScenarioDiagnosticKind):Void {
		final coordinate = switch kind {
			case UnknownRequiredFeature(feature): coordinateForFeature(feature);
			case InvalidRunTotal(id, _, _), DuplicateId(id), UnresolvedReference(id), ImpossiblePlacement(id), InvalidRule(id), RuleCycle(id),
				InvalidExtension(id):
				coordinateForId(id);
			case _: fallbackCoordinate();
		}
		diagnostics.push({coordinate: coordinate, kind: kind});
	}

	function addAt(kind:ScenarioDiagnosticKind, source:ScenarioId):Void
		diagnostics.push({coordinate: coordinateForId(source), kind: kind});

	function coordinateForFeature(feature:ContentId):ScenarioCoordinate {
		for (location in parsed.sourceLocations)
			switch location.subject {
				case Feature(id) if (id.text() == feature.text()):
					return location.coordinate;
				case _:
			}
		return fallbackCoordinate();
	}

	function coordinateForId(id:ScenarioId):ScenarioCoordinate {
		for (location in parsed.sourceLocations) {
			final matches = switch location.subject {
				case MapIdentity(candidate), Chunk(candidate), Object(candidate), Dialogue(candidate), Journal(candidate), Objective(candidate),
					Route(candidate), Variable(candidate), Sequence(candidate), Rule(candidate), Extension(_, candidate): candidate.text() == id.text();
				case _: false;
			}
			if (matches)
				return location.coordinate;
		}
		return fallbackCoordinate();
	}

	function fallbackCoordinate():ScenarioCoordinate
		return parsed.recordCoordinates.length == 0 ? {line: 1, column: 1, record: 0} : parsed.recordCoordinates[0];
}

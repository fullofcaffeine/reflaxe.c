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
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;
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
	final zoneIds:Map<String, Bool> = [];
	final entityIds:Map<String, Bool> = [];
	final checkpointIds:Map<String, Bool> = [];
	final dialogueIds:Map<String, Bool> = [];
	final journalIds:Map<String, Bool> = [];
	final objectiveIds:Map<String, Bool> = [];
	final routeIds:Map<String, Bool> = [];
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
				addAtCoordinate(DuplicateContentId(feature), lastCoordinateForFeature(feature));
			seen.set(key, true);
			if (key == "caxecraft:core")
				core = true;
			if (!registry.supportsFeature(feature))
				add(UnknownRequiredFeature(feature));
		}
		for (feature in scenario.optionalFeatures) {
			final key = feature.text();
			if (seen.exists(key))
				addAtCoordinate(DuplicateContentId(feature), lastCoordinateForFeature(feature));
			seen.set(key, true);
		}
		if (!core)
			add(MissingRecord(CoreFeatureRecord));
	}

	function validateWorld():Void {
		final size = scenario.world.size;
		final worldCoordinate = lastCoordinateForWorld();
		var invalidEdge = false;
		if (size.width <= 0 || size.width > ScenarioLimits.MAX_WORLD_WIDTH) {
			addAtCoordinate(LimitExceeded(WorldWidth, ScenarioLimits.MAX_WORLD_WIDTH), worldCoordinate);
			invalidEdge = true;
		}
		if (size.height <= 0 || size.height > ScenarioLimits.MAX_WORLD_HEIGHT) {
			addAtCoordinate(LimitExceeded(WorldHeight, ScenarioLimits.MAX_WORLD_HEIGHT), worldCoordinate);
			invalidEdge = true;
		}
		if (size.depth <= 0 || size.depth > ScenarioLimits.MAX_WORLD_DEPTH) {
			addAtCoordinate(LimitExceeded(WorldDepth, ScenarioLimits.MAX_WORLD_DEPTH), worldCoordinate);
			invalidEdge = true;
		}
		if (invalidEdge)
			return;
		final cells = size.width * size.height * size.depth;
		if (cells <= 0 || cells > ScenarioLimits.MAX_WORLD_CELLS) {
			addAtCoordinate(LimitExceeded(WorldCells, ScenarioLimits.MAX_WORLD_CELLS), worldCoordinate);
			return;
		}
		if (scenario.world.palette.length > ScenarioLimits.MAX_PALETTE_ENTRIES)
			addAtCoordinate(LimitExceeded(PaletteEntries, ScenarioLimits.MAX_PALETTE_ENTRIES),
				lastCoordinateForPalette(scenario.world.palette[scenario.world.palette.length - 1].code));
		final palette:Map<Int, Bool> = [];
		var zeroIsAir = false;
		for (entry in scenario.world.palette) {
			final coordinate = lastCoordinateForPalette(entry.code);
			if (entry.code < 0 || entry.code > 255)
				addAtCoordinate(IntegerOutOfRange, coordinate);
			else if (palette.exists(entry.code))
				addAtCoordinate(DuplicatePaletteCode(entry.code), coordinate);
			palette.set(entry.code, true);
			if (!registry.hasBlock(entry.blockType))
				addAtCoordinate(UnresolvedContent(entry.blockType), coordinate);
			if (entry.code == 0 && registry.isAirBlock(entry.blockType))
				zeroIsAir = true;
		}
		if (!zeroIsAir)
			addAtCoordinate(MissingRecord(AirPaletteRecord), worldCoordinate);

		final covered = [for (_ in 0...cells) false];
		final chunkIds:Map<String, Bool> = [];
		for (chunk in scenario.world.chunks) {
			unique(chunkIds, chunk.id, ChunkIdentity);
			final chunkCoordinate = lastCoordinateForIdentity(chunk.id, ChunkIdentity);
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
				addAtCoordinate(ImpossiblePlacement(chunk.id), chunkCoordinate);
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
				addAtCoordinate(InvalidRunTotal(chunk.id, expected, actual), chunkCoordinate);
				continue;
			}
			for (z in chunk.origin.z...chunk.origin.z + depth)
				for (y in chunk.origin.y...chunk.origin.y + height)
					for (x in chunk.origin.x...chunk.origin.x + width) {
						final cell = (z * size.height + y) * size.width + x;
						if (covered[cell])
							addAtCoordinate(ImpossiblePlacement(chunk.id), chunkCoordinate);
						covered[cell] = true;
					}
		}
		for (value in covered)
			if (!value) {
				addAtCoordinate(MissingRecord(CompleteChunkCoverage), worldCoordinate);
				break;
			}
	}

	function indexIdentities():Void {
		if (scenario.objects.length > ScenarioLimits.MAX_OBJECTS)
			addAtCoordinate(LimitExceeded(Objects, ScenarioLimits.MAX_OBJECTS),
				lastCoordinateForIdentity(scenario.objects[scenario.objects.length - 1].id, ObjectIdentity));
		for (object in scenario.objects) {
			unique(objectIds, object.id, ObjectIdentity);
			switch object.placement {
				case Checkpoint(_):
					checkpointIds.set(object.id.text(), true);
				case Entity(_, _):
					entityIds.set(object.id.text(), true);
				case TriggerZone(_):
					zoneIds.set(object.id.text(), true);
				case _:
			}
		}
		if (scenario.story.dialogues.length > ScenarioLimits.MAX_DIALOGUES)
			addAtCoordinate(LimitExceeded(Dialogues, ScenarioLimits.MAX_DIALOGUES),
				lastCoordinateForIdentity(scenario.story.dialogues[scenario.story.dialogues.length - 1].id, DialogueIdentity));
		for (dialogue in scenario.story.dialogues)
			unique(dialogueIds, dialogue.id, DialogueIdentity);
		for (entry in scenario.story.journal)
			unique(journalIds, entry.id, JournalIdentity);
		if (scenario.story.objectives.length > ScenarioLimits.MAX_OBJECTIVES)
			addAtCoordinate(LimitExceeded(Objectives, ScenarioLimits.MAX_OBJECTIVES),
				lastCoordinateForIdentity(scenario.story.objectives[scenario.story.objectives.length - 1].id, ObjectiveIdentity));
		for (objective in scenario.story.objectives)
			unique(objectiveIds, objective.id, ObjectiveIdentity);
		if (scenario.story.routes.length > ScenarioLimits.MAX_ROUTES)
			addAtCoordinate(LimitExceeded(Routes, ScenarioLimits.MAX_ROUTES),
				lastCoordinateForIdentity(scenario.story.routes[scenario.story.routes.length - 1].id, RouteIdentity));
		for (route in scenario.story.routes)
			unique(routeIds, route.id, RouteIdentity);
		if (scenario.flow.sequences.length > ScenarioLimits.MAX_SEQUENCES)
			addAtCoordinate(LimitExceeded(Sequences, ScenarioLimits.MAX_SEQUENCES),
				lastCoordinateForIdentity(scenario.flow.sequences[scenario.flow.sequences.length - 1].id, SequenceIdentity));
		for (sequence in scenario.flow.sequences) {
			unique(sequenceIds, sequence.id, SequenceIdentity);
			sequenceTable.set(sequence.id.text(), sequence);
			final parameterIds:Map<String, Bool> = [];
			for (parameter in sequence.parameters)
				unique(parameterIds, parameter.id, SequenceParameterIdentity(sequence.id));
		}
		if (scenario.flow.variables.length > ScenarioLimits.MAX_VARIABLES)
			addAtCoordinate(LimitExceeded(Variables, ScenarioLimits.MAX_VARIABLES),
				lastCoordinateForIdentity(scenario.flow.variables[scenario.flow.variables.length - 1].id, VariableIdentity));
		for (variable in scenario.flow.variables) {
			if (variableKinds.exists(variable.id.text()))
				addAtCoordinate(DuplicateId(variable.id), lastCoordinateForIdentity(variable.id, VariableIdentity));
			variableKinds.set(variable.id.text(), valueKind(variable.initial));
		}
	}

	function validateObjects():Void {
		var playerSpawns = 0;
		for (object in scenario.objects) {
			final objectCoordinate = lastCoordinateForIdentity(object.id, ObjectIdentity);
			if (object.tags.length > ScenarioLimits.MAX_TAGS_PER_OBJECT)
				addAtCoordinate(LimitExceeded(ObjectTags, ScenarioLimits.MAX_TAGS_PER_OBJECT), objectCoordinate);
			final tags:Map<String, Bool> = [];
			for (tag in object.tags) {
				if (tags.exists(tag.text()))
					addAtCoordinate(DuplicateTag(object.id, tag), lastCoordinateForTag(object.id, tag));
				tags.set(tag.text(), true);
			}
			switch object.placement {
				case PlayerSpawn(position):
					playerSpawns++;
					validateTransform(object.id, position, objectCoordinate);
				case Checkpoint(position):
					validateTransform(object.id, position, objectCoordinate);
				case Item(item, quantity, position):
					if (!registry.hasItem(item))
						addAtCoordinate(UnresolvedContent(item), objectCoordinate);
					else if (quantity <= 0 || quantity > registry.maximumItemQuantity(item))
						addAtCoordinate(ImpossiblePlacement(object.id), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
				case Entity(entity, position):
					if (!registry.hasEntity(entity))
						addAtCoordinate(UnresolvedContent(entity), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
				case Npc(npc, dialogue, position):
					if (!registry.hasNpc(npc))
						addAtCoordinate(UnresolvedContent(npc), objectCoordinate);
					if (!dialogueIds.exists(dialogue.text()))
						addAtCoordinate(UnresolvedReference(dialogue), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
				case Prefab(prefab, position):
					if (!registry.hasPrefab(prefab))
						addAtCoordinate(UnresolvedContent(prefab), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
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
						addAtCoordinate(ImpossiblePlacement(object.id), objectCoordinate);
				case StatefulObject(kind, state, position):
					if (!registry.hasStatefulObject(kind))
						addAtCoordinate(UnresolvedContent(kind), objectCoordinate);
					if (!registry.hasState(state))
						addAtCoordinate(UnresolvedContent(state), objectCoordinate);
					validateTransform(object.id, position, objectCoordinate);
			}
		}
		if (playerSpawns != 1)
			addAtCoordinate(MissingRecord(SinglePlayerSpawn), lastCoordinateForWorld());
	}

	function validateTransform(id:ScenarioId, value:caxecraft.scenario.ScenarioGeometry.ScenarioTransform, coordinate:ScenarioCoordinate):Void {
		if (value.xMilli < 0
			|| value.yMilli < 0
			|| value.zMilli < 0
			|| value.xMilli >= scenario.world.size.width * 1000
			|| value.yMilli >= scenario.world.size.height * 1000
			|| value.zMilli >= scenario.world.size.depth * 1000
			|| value.yawDegrees < 0
			|| value.yawDegrees > 359)
			addAtCoordinate(ImpossiblePlacement(id), coordinate);
	}

	function validateStory():Void {
		for (dialogue in scenario.story.dialogues) {
			final coordinate = lastCoordinateForIdentity(dialogue.id, DialogueIdentity);
			if (dialogue.lines.length > ScenarioLimits.MAX_DIALOGUE_LINES)
				addAtCoordinate(LimitExceeded(DialogueLines, ScenarioLimits.MAX_DIALOGUE_LINES), coordinate);
			for (line in dialogue.lines)
				if (line.speaker != null && !objectIds.exists(line.speaker.text()))
					addAtCoordinate(UnresolvedReference(line.speaker), coordinate);
		}
		for (route in scenario.story.routes) {
			final coordinate = lastCoordinateForIdentity(route.id, RouteIdentity);
			for (objective in route.objectives)
				if (!objectiveIds.exists(objective.text()))
					addAtCoordinate(UnresolvedReference(objective), coordinate);
		}
	}

	function validateFlow():Void {
		if (scenario.flow.rules.length > ScenarioLimits.MAX_RULES)
			addAtCoordinate(LimitExceeded(Rules, ScenarioLimits.MAX_RULES),
				lastCoordinateForIdentity(scenario.flow.rules[scenario.flow.rules.length - 1].id, RuleIdentity));
		for (variable in scenario.flow.variables) {
			final coordinate = lastCoordinateForIdentity(variable.id, VariableIdentity);
			validateFlowValue(variable.initial, coordinate);
			switch variable.scope {
				case Local(sequence) if (!sequenceIds.exists(sequence.text())):
					addAtCoordinate(UnresolvedReference(sequence), coordinate);
				case _:
			}
		}
		for (sequence in scenario.flow.sequences) {
			final coordinate = lastCoordinateForIdentity(sequence.id, SequenceIdentity);
			for (parameter in sequence.parameters) {
				final parameterCoordinate = lastCoordinateForIdentity(parameter.id, SequenceParameterIdentity(sequence.id));
				validateFlowValue(parameter.initial, parameterCoordinate);
			}
			for (actionIndex in 0...sequence.actions.length)
				validateAction(sequence.id, lastCoordinateForSequenceAction(sequence.id, actionIndex), sequence.actions[actionIndex], false);
		}
		validateSequenceGraph();
		final ruleIds:Map<String, Bool> = [];
		for (rule in scenario.flow.rules) {
			unique(ruleIds, rule.id, RuleIdentity);
			final coordinate = lastCoordinateForIdentity(rule.id, RuleIdentity);
			if (consumeActionBudget(rule.actions, ScenarioLimits.MAX_ACTIONS_PER_RULE) < 0)
				addAtCoordinate(LimitExceeded(RuleActions, ScenarioLimits.MAX_ACTIONS_PER_RULE), coordinate);
			switch rule.repeat {
				case Cooldown(ticks) if (ticks <= 0):
					addAtCoordinate(InvalidRule(rule.id), coordinate);
				case _:
			}
			validateEvent(rule.id, lastCoordinateForRuleEvent(rule.id), rule.event);
			validatePredicate(rule.id, lastCoordinateForRulePredicate(rule.id), rule.predicate, 1);
			for (actionIndex in 0...rule.actions.length)
				validateAction(rule.id, lastCoordinateForRuleAction(rule.id, actionIndex), rule.actions[actionIndex], false);
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
				addAtCoordinate(InvalidExtension(extension.id), lastCoordinateForExtension(extension.feature, extension.id));
			identities.set(key, true);
		}
	}

	function validateEvent(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowEvent):Void {
		switch value {
			case EnterZone(id), LeaveZone(id):
				if (!zoneIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case Interact(id):
				if (!objectIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case EntityDefeated(id):
				if (!entityIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case BlockChanged(zone, block):
				if (!zoneIds.exists(zone.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
				if (!registry.hasBlock(block))
					addAtCoordinate(UnresolvedContent(block), coordinate);
			case UseItem(item):
				if (!registry.hasItem(item))
					addAtCoordinate(UnresolvedContent(item), coordinate);
			case SignalReceived(signal):
				if (!registry.hasSignal(signal))
					addAtCoordinate(UnresolvedContent(signal), coordinate);
			case TimerExpired(_):
			case ObjectiveChanged(id):
				if (!objectiveIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case StateChanged(id):
				if (!variableKinds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validatePredicate(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowPredicate, depth:Int):Void {
		if (depth > ScenarioLimits.MAX_PREDICATE_DEPTH) {
			addAtCoordinate(InvalidRule(owner), coordinate);
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
				if (!registry.hasState(state))
					addAtCoordinate(UnresolvedContent(state), coordinate);
			case ObjectStateIs(id, state):
				if (!objectIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
				if (!registry.hasState(state))
					addAtCoordinate(UnresolvedContent(state), coordinate);
			case InventoryHas(id, item, _, quantity):
				if (!objectIds.exists(id.text()) || quantity < 0)
					addAtCoordinate(InvalidRule(owner), coordinate);
				if (!registry.hasItem(item))
					addAtCoordinate(UnresolvedContent(item), coordinate);
			case ObjectiveIs(id, _):
				if (!objectiveIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case NearObject(actor, objectId, maximum):
				if (!objectIds.exists(actor.text()) || !objectIds.exists(objectId.text()) || maximum < 0)
					addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validateAction(owner:ScenarioId, coordinate:ScenarioCoordinate, value:FlowAction, insideChoice:Bool):Void {
		switch value {
			case ShowDialogue(id):
				if (!dialogueIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case AddJournal(id):
				if (!journalIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case SetFlag(id, _):
				requireVariable(owner, coordinate, id, FlagValue);
			case SetCounter(id, _), AddCounter(id, _):
				requireVariable(owner, coordinate, id, CounterValue);
			case SetState(id, state):
				requireVariable(owner, coordinate, id, StateValue);
				if (!registry.hasState(state))
					addAtCoordinate(UnresolvedContent(state), coordinate);
			case GiveItem(id, item, quantity), TakeItem(id, item, quantity):
				if (!objectIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
				if (!registry.hasItem(item))
					addAtCoordinate(UnresolvedContent(item), coordinate);
				else if (quantity <= 0 || quantity > registry.maximumItemQuantity(item))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case Spawn(id), Despawn(id):
				if (!objectIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case SetObjectState(id, state):
				if (!objectIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
				if (!registry.hasState(state))
					addAtCoordinate(UnresolvedContent(state), coordinate);
			case SetCheckpoint(id):
				if (!checkpointIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case SetObjective(id, _):
				if (!objectiveIds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case PlayEffect(effect, target):
				if (!registry.hasEffect(effect))
					addAtCoordinate(UnresolvedContent(effect), coordinate);
				if (target != null && !objectIds.exists(target.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
			case EmitSignal(signal):
				if (!registry.hasSignal(signal))
					addAtCoordinate(UnresolvedContent(signal), coordinate);
			case Schedule(_, ticks, sequence, arguments):
				if (ticks <= 0)
					addAtCoordinate(InvalidRule(owner), coordinate);
				for (argument in arguments)
					validateArgument(owner, coordinate, argument);
				validateSequenceCall(owner, coordinate, sequence, arguments);
			case CallSequence(sequence, arguments):
				for (argument in arguments)
					validateArgument(owner, coordinate, argument);
				validateSequenceCall(owner, coordinate, sequence, arguments);
			case ChooseSeeded(seed, choices):
				if (insideChoice) {
					addAtCoordinate(InvalidRule(owner), coordinate);
					return;
				}
				requireVariable(owner, coordinate, seed, CounterValue);
				if (choices.length == 0)
					addAtCoordinate(InvalidRule(owner), coordinate);
				for (choice in choices) {
					if (choice.weight <= 0)
						addAtCoordinate(InvalidRule(owner), coordinate);
					for (entry in choice.actions)
						validateAction(owner, coordinate, entry, true);
				}
		}
	}

	function validateSequenceCall(owner:ScenarioId, coordinate:ScenarioCoordinate, id:ScenarioId, arguments:Array<FlowArgument>):Void {
		final sequence = sequenceTable.get(id.text());
		if (sequence == null || sequence.parameters.length != arguments.length) {
			addAtCoordinate(InvalidRule(owner), coordinate);
			return;
		}
		for (index in 0...arguments.length) {
			final expected = valueKind(sequence.parameters[index].initial);
			final actual = switch arguments[index] {
				case Value(value): valueKind(value);
				case Variable(variable): variableKinds.get(variable.text());
			}
			if (actual != expected)
				addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validateSequenceGraph():Void {
		final visiting:Map<String, Bool> = [];
		final depths:Map<String, Int> = [];
		final reportedCycles:Map<String, Bool> = [];
		var reportedDepth = false;
		for (sequence in scenario.flow.sequences) {
			final depth = sequenceDepth(sequence, visiting, depths, reportedCycles);
			if (!reportedDepth && depth > ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH) {
				addAtCoordinate(LimitExceeded(SequenceCallDepth, ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH),
					lastCoordinateForIdentity(sequence.id, SequenceIdentity));
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
				addAtCoordinate(RuleCycle(sequence.id), lastCoordinateForIdentity(sequence.id, SequenceIdentity));
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
				final target = sequenceTable.get(id.text());
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
				if (!variableKinds.exists(id.text()))
					addAtCoordinate(InvalidRule(owner), coordinate);
		}
	}

	function validateFlowValue(value:FlowValue, coordinate:ScenarioCoordinate):Void
		switch value {
			case State(state) if (!registry.hasState(state)):
				addAtCoordinate(UnresolvedContent(state), coordinate);
			case _:
		}

	function requireVariable(owner:ScenarioId, coordinate:ScenarioCoordinate, id:ScenarioId, kind:FlowValueKind):Void
		if (!variableKinds.exists(id.text()) || variableKinds.get(id.text()) != kind)
			addAtCoordinate(InvalidRule(owner), coordinate);

	function valueKind(value:FlowValue):FlowValueKind
		return switch value {
			case Flag(_): FlagValue;
			case Counter(_): CounterValue;
			case State(_): StateValue;
		}

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

	function unique(target:Map<String, Bool>, id:ScenarioId, source:ScenarioIdentitySource):Void {
		if (target.exists(id.text()))
			addAtCoordinate(DuplicateId(id), lastCoordinateForIdentity(id, source));
		target.set(id.text(), true);
	}

	function add(kind:ScenarioDiagnosticKind):Void {
		final coordinate = switch kind {
			case UnknownRequiredFeature(feature): lastCoordinateForFeature(feature);
			case _: fallbackCoordinate();
		}
		diagnostics.push({coordinate: coordinate, kind: kind});
	}

	function addAtCoordinate(kind:ScenarioDiagnosticKind, coordinate:ScenarioCoordinate):Void
		diagnostics.push({coordinate: coordinate, kind: kind});

	function lastCoordinateForFeature(feature:ContentId):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case Feature(candidate): candidate.text() == feature.text();
			case _: false;
		});

	function lastCoordinateForWorld():ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case World: true;
			case _: false;
		});

	function lastCoordinateForPalette(code:Int):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case Palette(candidate): candidate == code;
			case _: false;
		});

	function lastCoordinateForTag(objectId:ScenarioId, tag:ScenarioTag):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case ObjectTag(candidateObject, candidateTag): candidateObject.text() == objectId.text() && candidateTag.text() == tag.text();
			case _: false;
		});

	function lastCoordinateForExtension(feature:ContentId, id:ScenarioId):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case Extension(candidateFeature, candidateId): candidateFeature.text() == feature.text() && candidateId.text() == id.text();
			case _: false;
		});

	function lastCoordinateForSequenceAction(sequenceId:ScenarioId, actionIndex:Int):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case SequenceAction(candidate, index): candidate.text() == sequenceId.text() && index == actionIndex;
			case _: false;
		});

	function lastCoordinateForRuleEvent(ruleId:ScenarioId):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case RuleEvent(candidate): candidate.text() == ruleId.text();
			case _: false;
		});

	function lastCoordinateForRulePredicate(ruleId:ScenarioId):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case RulePredicate(candidate): candidate.text() == ruleId.text();
			case _: false;
		});

	function lastCoordinateForRuleAction(ruleId:ScenarioId, actionIndex:Int):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case RuleAction(candidate, index): candidate.text() == ruleId.text() && index == actionIndex;
			case _: false;
		});

	function lastCoordinateForIdentity(id:ScenarioId, source:ScenarioIdentitySource):ScenarioCoordinate
		return lastCoordinateMatching(subject -> {
			final candidate:Null<ScenarioId> = switch [source, subject] {
				case [ChunkIdentity, Chunk(value)]: value;
				case [ObjectIdentity, Object(value)]: value;
				case [DialogueIdentity, Dialogue(value)]: value;
				case [JournalIdentity, Journal(value)]: value;
				case [ObjectiveIdentity, Objective(value)]: value;
				case [RouteIdentity, Route(value)]: value;
				case [VariableIdentity, Variable(value)]: value;
				case [SequenceIdentity, Sequence(value)]: value;
				case [SequenceParameterIdentity(owner), SequenceParameter(candidateOwner, value)] if (owner.text() == candidateOwner.text()): value;
				case [RuleIdentity, Rule(value)]: value;
				case _: null;
			}
			return candidate != null && candidate.text() == id.text();
		});

	function lastCoordinateMatching(matches:ScenarioSourceSubject->Bool):ScenarioCoordinate {
		var result:Null<ScenarioCoordinate> = null;
		for (location in parsed.sourceLocations)
			if (matches(location.subject))
				result = location.coordinate;
		return result == null ? fallbackCoordinate() : result;
	}

	function fallbackCoordinate():ScenarioCoordinate
		return parsed.recordCoordinates.length == 0 ? {line: 1, column: 1, record: 0} : parsed.recordCoordinates[0];
}

/** The declaration family whose source coordinate owns a scenario ID. */
private enum ScenarioIdentitySource {
	ChunkIdentity;
	ObjectIdentity;
	DialogueIdentity;
	JournalIdentity;
	ObjectiveIdentity;
	RouteIdentity;
	VariableIdentity;
	SequenceIdentity;
	SequenceParameterIdentity(sequenceId:ScenarioId);
	RuleIdentity;
}

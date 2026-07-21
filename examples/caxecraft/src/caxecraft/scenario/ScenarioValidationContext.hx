package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowValueKind;
import caxecraft.scenario.ScenarioCodecModel.ParsedScenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioCoordinate;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/**
	Shared identities, diagnostics, and source coordinates for semantic validation.

	Domain validators ask this context questions such as “does this object exist?”
	instead of maintaining competing indexes. It also keeps diagnostic lookup in
	one place, so a split validator still points at the same source record.

	This class is an implementation detail of `ScenarioValidator`.
	`@:noCompletion` hides it from ordinary editor suggestions only; it does not
	change Haxe visibility, runtime behavior, or type safety.
**/
@:noCompletion
final class ScenarioValidationContext {
	public final diagnostics:Array<ScenarioDiagnostic> = [];
	public final scenario:Scenario;
	public final registry:ScenarioContentRegistry;

	final parsed:ParsedScenario;
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

	/**
		Build every cross-domain lookup before validators resolve references.

		The sequence here intentionally matches the original single-class
		validator because duplicate and limit diagnostics are ordered output.
	**/
	public function indexIdentities():Void {
		if (scenario.objects.length > ScenarioLimits.MAX_OBJECTS)
			addAtCoordinate(LimitExceeded(Objects, ScenarioLimits.MAX_OBJECTS),
				coordinateForIdentity(scenario.objects[scenario.objects.length - 1].id, ObjectIdentity));
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
				coordinateForIdentity(scenario.story.dialogues[scenario.story.dialogues.length - 1].id, DialogueIdentity));
		for (dialogue in scenario.story.dialogues)
			unique(dialogueIds, dialogue.id, DialogueIdentity);
		for (entry in scenario.story.journal)
			unique(journalIds, entry.id, JournalIdentity);
		if (scenario.story.objectives.length > ScenarioLimits.MAX_OBJECTIVES)
			addAtCoordinate(LimitExceeded(Objectives, ScenarioLimits.MAX_OBJECTIVES),
				coordinateForIdentity(scenario.story.objectives[scenario.story.objectives.length - 1].id, ObjectiveIdentity));
		for (objective in scenario.story.objectives)
			unique(objectiveIds, objective.id, ObjectiveIdentity);
		if (scenario.story.routes.length > ScenarioLimits.MAX_ROUTES)
			addAtCoordinate(LimitExceeded(Routes, ScenarioLimits.MAX_ROUTES),
				coordinateForIdentity(scenario.story.routes[scenario.story.routes.length - 1].id, RouteIdentity));
		for (route in scenario.story.routes)
			unique(routeIds, route.id, RouteIdentity);
		if (scenario.flow.sequences.length > ScenarioLimits.MAX_SEQUENCES)
			addAtCoordinate(LimitExceeded(Sequences, ScenarioLimits.MAX_SEQUENCES),
				coordinateForIdentity(scenario.flow.sequences[scenario.flow.sequences.length - 1].id, SequenceIdentity));
		for (sequence in scenario.flow.sequences) {
			unique(sequenceIds, sequence.id, SequenceIdentity);
			sequenceTable.set(sequence.id.text(), sequence);
			final parameterIds:Map<String, Bool> = [];
			for (parameter in sequence.parameters)
				unique(parameterIds, parameter.id, SequenceParameterIdentity(sequence.id));
		}
		if (scenario.flow.variables.length > ScenarioLimits.MAX_VARIABLES)
			addAtCoordinate(LimitExceeded(Variables, ScenarioLimits.MAX_VARIABLES),
				coordinateForIdentity(scenario.flow.variables[scenario.flow.variables.length - 1].id, VariableIdentity));
		for (variable in scenario.flow.variables) {
			if (variableKinds.exists(variable.id.text()))
				addAtCoordinate(DuplicateId(variable.id), coordinateForIdentity(variable.id, VariableIdentity));
			variableKinds.set(variable.id.text(), flowValueKind(variable.initial));
		}
	}

	public inline function hasObject(id:ScenarioId):Bool
		return objectIds.exists(id.text());

	public inline function hasZone(id:ScenarioId):Bool
		return zoneIds.exists(id.text());

	public inline function hasEntity(id:ScenarioId):Bool
		return entityIds.exists(id.text());

	public inline function hasCheckpoint(id:ScenarioId):Bool
		return checkpointIds.exists(id.text());

	public inline function hasDialogue(id:ScenarioId):Bool
		return dialogueIds.exists(id.text());

	public inline function hasJournal(id:ScenarioId):Bool
		return journalIds.exists(id.text());

	public inline function hasObjective(id:ScenarioId):Bool
		return objectiveIds.exists(id.text());

	public inline function hasSequence(id:ScenarioId):Bool
		return sequenceIds.exists(id.text());

	public inline function hasVariable(id:ScenarioId):Bool
		return variableKinds.exists(id.text());

	/**
		Return the value kind visible from a rule or one sequence call frame.

		A null sequence means persistent rule scope, where only map, player, and
		quest variables exist. Inside a sequence, that sequence's parameters and
		locals are visible too. Validation rejects name collisions, so lookup order
		is a defensive runtime rule rather than an authored shadowing feature.
	**/
	public function variableKindInScope(id:ScenarioId, sequenceOwner:Null<ScenarioId>):Null<FlowValueKind> {
		if (sequenceOwner != null) {
			final owner = sequence(sequenceOwner);
			if (owner != null)
				for (parameter in owner.parameters)
					if (parameter.id.text() == id.text())
						return flowValueKind(parameter.initial);
		}
		for (variable in scenario.flow.variables) {
			if (variable.id.text() != id.text())
				continue;
			return switch variable.scope {
				case Local(owner) if (sequenceOwner != null && owner.text() == sequenceOwner.text()): flowValueKind(variable.initial);
				case Local(_): null;
				case Map | Player | Quest: flowValueKind(variable.initial);
			};
		}
		return null;
	}

	public inline function hasPersistentVariable(id:ScenarioId):Bool
		return variableKindInScope(id, null) != null;

	public inline function sequence(id:ScenarioId):Null<FlowSequence>
		return sequenceTable.get(id.text());

	public static function flowValueKind(value:FlowValue):FlowValueKind
		return switch value {
			case Flag(_): FlagValue;
			case Counter(_): CounterValue;
			case State(_): StateValue;
		}

	public function add(kind:ScenarioDiagnosticKind):Void {
		final coordinate = switch kind {
			case UnknownRequiredFeature(feature): coordinateForFeature(feature);
			case _: fallbackCoordinate();
		}
		diagnostics.push({coordinate: coordinate, kind: kind});
	}

	public function addAtCoordinate(kind:ScenarioDiagnosticKind, coordinate:ScenarioCoordinate):Void
		diagnostics.push({coordinate: coordinate, kind: kind});

	public function coordinateForFeature(feature:ContentId):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case Feature(candidate): candidate.text() == feature.text();
			case _: false;
		});

	public function coordinateForWorld():ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case World: true;
			case _: false;
		});

	public function coordinateForPalette(code:Int):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case Palette(candidate): candidate == code;
			case _: false;
		});

	public function coordinateForTag(objectId:ScenarioId, tag:ScenarioTag):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case ObjectTag(candidateObject, candidateTag): candidateObject.text() == objectId.text() && candidateTag.text() == tag.text();
			case _: false;
		});

	public function coordinateForExtension(feature:ContentId, id:ScenarioId):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case Extension(candidateFeature, candidateId): candidateFeature.text() == feature.text() && candidateId.text() == id.text();
			case _: false;
		});

	public function coordinateForSequenceAction(sequenceId:ScenarioId, actionIndex:Int):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case SequenceAction(candidate, index): candidate.text() == sequenceId.text() && index == actionIndex;
			case _: false;
		});

	public function coordinateForRuleEvent(ruleId:ScenarioId):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case RuleEvent(candidate): candidate.text() == ruleId.text();
			case _: false;
		});

	public function coordinateForRulePredicate(ruleId:ScenarioId):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case RulePredicate(candidate): candidate.text() == ruleId.text();
			case _: false;
		});

	public function coordinateForRuleAction(ruleId:ScenarioId, actionIndex:Int):ScenarioCoordinate
		return lastCoordinateMatching(subject -> switch subject {
			case RuleAction(candidate, index): candidate.text() == ruleId.text() && index == actionIndex;
			case _: false;
		});

	public function coordinateForIdentity(id:ScenarioId, source:ScenarioIdentitySource):ScenarioCoordinate
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

	function unique(target:Map<String, Bool>, id:ScenarioId, source:ScenarioIdentitySource):Void {
		if (target.exists(id.text()))
			addAtCoordinate(DuplicateId(id), coordinateForIdentity(id, source));
		target.set(id.text(), true);
	}

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

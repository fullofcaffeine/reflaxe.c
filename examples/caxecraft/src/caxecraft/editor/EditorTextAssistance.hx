package caxecraft.editor;

import caxecraft.editor.EditorFlowCardLibrary.EditorFlowContentChoices;
import caxecraft.scenario.CaxeFlow.FlowValueKind;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionArgumentRole;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowContentArgumentRole;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowScenarioArgumentRole;
import caxecraft.scenario.CaxeFlowActionRegistry.allFlowActionDescriptors;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionArgumentRoles;
import caxecraft.scenario.CaxeFlowEventRegistry.FlowEventArgumentRole;
import caxecraft.scenario.CaxeFlowEventRegistry.allFlowEventDescriptors;
import caxecraft.scenario.CaxeFlowPredicateRegistry.FlowPredicateSchema;
import caxecraft.scenario.CaxeFlowPredicateRegistry.allFlowPredicateDescriptors;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;

/**
	Provides bounded completion and world-reference lookup for exact CAXEMAP text.

	Syntax comes from the executable CaxeFlow descriptor registries. Identity
	choices come from the current typed scenario and validated content choices.
	The helper edits only the final bare token on one line, so indentation and all
	other source bytes stay owned by the advanced text draft.
**/
/** Why one completion is available to the advanced author. */
enum EditorTextCompletionKind {
	EventSyntaxCompletion;
	PredicateSyntaxCompletion;
	ActionSyntaxCompletion;
	ScenarioIdentityCompletion;
	ContentIdentityCompletion;
}

/** One deterministic token replacement projected from shared typed data. */
typedef EditorTextCompletion = {
	final replacement:String;
	final kind:EditorTextCompletionKind;
}

/** Return matching completions for the final token of one source line. */
function editorTextCompletions(line:String, scenario:Scenario, content:EditorFlowContentChoices):Array<EditorTextCompletion> {
	if (commentStart(line) >= 0)
		return [];
	final tokens = sourceTokens(line);
	if (tokens.length == 0)
		return [];
	final prefix = activePrefix(line, tokens);
	return switch tokens[0] {
		case "when": eventCompletions(tokens, prefix, lineEndsAtDelimiter(line), scenario, content);
		case "if": predicateCompletions(tokens, prefix, lineEndsAtDelimiter(line), scenario, content);
		case "do": actionCompletions(tokens, prefix, lineEndsAtDelimiter(line), scenario, content);
		case _: [];
	};
}

/** Replace only the final token while preserving every other source byte. */
function applyEditorTextCompletion(line:String, completion:EditorTextCompletion):String {
	var start = line.length;
	while (start > 0 && !isTokenDelimiter(line.charCodeAt(start - 1)))
		start--;
	return line.substr(0, start) + completion.replacement;
}

/** Return unique world objects referenced on one line in source order. */
function editorTextWorldReferences(line:String, scenario:Scenario):Array<ScenarioId> {
	final result:Array<ScenarioId> = [];
	for (token in sourceTokens(line))
		for (object in scenario.objects)
			if (object.id.text() == token && !containsScenarioId(result, object.id))
				result.push(object.id);
	return result;
}

private function eventCompletions(tokens:Array<String>, prefix:String, ended:Bool, scenario:Scenario,
		content:EditorFlowContentChoices):Array<EditorTextCompletion> {
	if (tokens.length == 1 || (tokens.length == 2 && !ended)) {
		final result:Array<EditorTextCompletion> = [];
		for (descriptor in allFlowEventDescriptors())
			addMatching(result, descriptor.id.text(), prefix, EventSyntaxCompletion);
		return result;
	}
	final descriptor = tokens.length > 1 ? eventDescriptor(tokens[1]) : null;
	if (descriptor == null)
		return [];
	final argument = activeArgumentIndex(tokens.length, ended);
	return argument < 0
		|| argument >= descriptor.arguments.length ? [] : eventRoleCompletions(descriptor.arguments[argument], prefix, scenario, content);
}

private function predicateCompletions(tokens:Array<String>, prefix:String, ended:Bool, scenario:Scenario,
		content:EditorFlowContentChoices):Array<EditorTextCompletion> {
	if (tokens.length == 1 || (tokens.length == 2 && !ended)) {
		final result:Array<EditorTextCompletion> = [];
		for (descriptor in allFlowPredicateDescriptors())
			addMatching(result, descriptor.id.text(), prefix, PredicateSyntaxCompletion);
		return result;
	}
	final descriptor = tokens.length > 1 ? predicateDescriptor(tokens[1]) : null;
	if (descriptor == null)
		return [];
	final argument = activeArgumentIndex(tokens.length, ended);
	return predicateRoleCompletions(descriptor.schema, argument, prefix, scenario, content);
}

private function actionCompletions(tokens:Array<String>, prefix:String, ended:Bool, scenario:Scenario,
		content:EditorFlowContentChoices):Array<EditorTextCompletion> {
	if (tokens.length == 1 || (tokens.length == 2 && !ended)) {
		final result:Array<EditorTextCompletion> = [];
		for (descriptor in allFlowActionDescriptors())
			addMatching(result, descriptor.id.text(), prefix, ActionSyntaxCompletion);
		return result;
	}
	final descriptor = tokens.length > 1 ? actionDescriptor(tokens[1]) : null;
	if (descriptor == null)
		return [];
	final roles = flowActionArgumentRoles(descriptor.schema);
	final argument = activeArgumentIndex(tokens.length, ended);
	return argument < 0 || argument >= roles.length ? [] : actionRoleCompletions(roles[argument], prefix, scenario, content);
}

private function eventRoleCompletions(role:FlowEventArgumentRole, prefix:String, scenario:Scenario,
		content:EditorFlowContentChoices):Array<EditorTextCompletion> {
	return switch role {
		case ZoneReference: matchingScenarioIds(zones(scenario), prefix);
		case ObjectReference | EntityReference: matchingScenarioIds(objects(scenario), prefix);
		case BlockContent: matchingContentIds(content.blocks, prefix);
		case ItemContent: matchingContentIds(content.items, prefix);
		case SignalContent: matchingContentIds(content.signals, prefix);
		case TimerReference: matchingScenarioIds(timerIds(scenario), prefix);
		case ObjectiveReference: matchingScenarioIds(objectiveIds(scenario), prefix);
		case VariableReference: matchingScenarioIds(variableIds(scenario, null), prefix);
		case LevelReference: matchingScenarioIds([scenario.id], prefix);
		case CampaignExitReference: [];
	};
}

private function actionRoleCompletions(role:FlowActionArgumentRole, prefix:String, scenario:Scenario,
		content:EditorFlowContentChoices):Array<EditorTextCompletion> {
	return switch role {
		case ScenarioArgument(value) | OptionalScenarioArgument(value): scenarioRoleCompletions(value, prefix, scenario);
		case ContentArgument(value): contentRoleCompletions(value, prefix, content);
		case IntegerArgument(_) | FlagArgument | ObjectiveStateArgument | SequenceArguments | WeightedChoices: [];
	};
}

private function scenarioRoleCompletions(role:FlowScenarioArgumentRole, prefix:String, scenario:Scenario):Array<EditorTextCompletion> {
	return switch role {
		case DialogueReference: matchingScenarioIds(dialogueIds(scenario), prefix);
		case JournalReference: matchingScenarioIds(journalIds(scenario), prefix);
		case VariableReference(kind): matchingScenarioIds(variableIds(scenario, kind), prefix);
		case InventoryOwnerReference | ObjectReference: matchingScenarioIds(objects(scenario), prefix);
		case CheckpointReference: matchingScenarioIds(checkpoints(scenario), prefix);
		case ObjectiveReference: matchingScenarioIds(objectiveIds(scenario), prefix);
		case CampaignExitReference: [];
		case TimerReference: matchingScenarioIds(timerIds(scenario), prefix);
		case SequenceReference: matchingScenarioIds(sequenceIds(scenario), prefix);
		case SeedVariableReference: matchingScenarioIds(variableIds(scenario, CounterValue), prefix);
	};
}

private function contentRoleCompletions(role:FlowContentArgumentRole, prefix:String, content:EditorFlowContentChoices):Array<EditorTextCompletion> {
	return switch role {
		case StateContent: matchingContentIds(content.states, prefix);
		case ItemContent: matchingContentIds(content.items, prefix);
		case EffectContent: matchingContentIds(content.effects, prefix);
		case SignalContent: matchingContentIds(content.signals, prefix);
	};
}

private function predicateRoleCompletions(schema:FlowPredicateSchema, argument:Int, prefix:String, scenario:Scenario,
		content:EditorFlowContentChoices):Array<EditorTextCompletion> {
	if (argument < 0)
		return [];
	return switch schema {
		case VariableAndFlag: argument == 0 ? matchingScenarioIds(variableIds(scenario, FlagValue), prefix) : [];
		case VariableComparisonAndInteger: argument == 0 ? matchingScenarioIds(variableIds(scenario, CounterValue), prefix) : [];
		case VariableAndContent:
			argument == 0 ? matchingScenarioIds(variableIds(scenario, StateValue), prefix) : argument == 1 ? matchingContentIds(content.states, prefix) : [];
		case ObjectAndContent:
			argument == 0 ? matchingScenarioIds(objects(scenario), prefix) : argument == 1 ? matchingContentIds(content.states, prefix) : [];
		case OwnerItemComparisonAndQuantity:
			argument == 0 ? matchingScenarioIds(objects(scenario), prefix) : argument == 1 ? matchingContentIds(content.items, prefix) : [];
		case ObjectiveAndState: argument == 0 ? matchingScenarioIds(objectiveIds(scenario), prefix) : [];
		case ActorObjectAndDistance: argument < 2 ? matchingScenarioIds(objects(scenario), prefix) : [];
		case EventActorReference: argument == 0 ? matchingScenarioIds(objects(scenario), prefix) : [];
		case NoPredicateArguments | PredicateChildren | OnePredicateChild | ScenarioModeValue | EventSweptFlag: [];
	};
}

private function sourceTokens(line:String):Array<String> {
	final result:Array<String> = [];
	var token = new StringBuf();
	var quoted = false;
	var escaped = false;
	for (index in 0...line.length) {
		final code = line.charCodeAt(index);
		if (quoted) {
			if (escaped)
				escaped = false;
			else if (code == 92)
				escaped = true;
			else if (code == 34)
				quoted = false;
		} else if (code == 35)
			break;
		else if (code == 34) {
			if (token.length > 0) {
				result.push(token.toString());
				token = new StringBuf();
			}
			quoted = true;
		} else if (isTokenDelimiter(code)) {
			if (token.length > 0) {
				result.push(token.toString());
				token = new StringBuf();
			}
		} else
			token.addChar(code);
	}
	if (token.length > 0)
		result.push(token.toString());
	return result;
}

private function commentStart(line:String):Int {
	var quoted = false;
	var escaped = false;
	for (index in 0...line.length) {
		final code = line.charCodeAt(index);
		if (quoted) {
			if (escaped)
				escaped = false;
			else if (code == 92)
				escaped = true;
			else if (code == 34)
				quoted = false;
		} else if (code == 34)
			quoted = true;
		else if (code == 35)
			return index;
	}
	return -1;
}

private inline function isTokenDelimiter(code:Int):Bool
	return code == 32 || code == 9 || code == 40 || code == 41;

private function lineEndsAtDelimiter(line:String):Bool
	return line.length > 0 && isTokenDelimiter(line.charCodeAt(line.length - 1));

private function activePrefix(line:String, tokens:Array<String>):String
	return lineEndsAtDelimiter(line) || tokens.length == 0 ? "" : tokens[tokens.length - 1];

private inline function activeArgumentIndex(tokenCount:Int, ended:Bool):Int
	return ended ? tokenCount - 2 : tokenCount - 3;

private function addMatching(result:Array<EditorTextCompletion>, value:String, prefix:String, kind:EditorTextCompletionKind):Void {
	if (prefix.length == 0 || value.indexOf(prefix.toLowerCase()) == 0)
		result.push({replacement: value, kind: kind});
}

private function matchingScenarioIds(values:Array<ScenarioId>, prefix:String):Array<EditorTextCompletion> {
	final result:Array<EditorTextCompletion> = [];
	for (value in values)
		addMatching(result, value.text(), prefix, ScenarioIdentityCompletion);
	return result;
}

private function matchingContentIds(values:Array<ContentId>, prefix:String):Array<EditorTextCompletion> {
	final result:Array<EditorTextCompletion> = [];
	for (value in values)
		addMatching(result, value.text(), prefix, ContentIdentityCompletion);
	return result;
}

private function objects(scenario:Scenario):Array<ScenarioId>
	return [for (object in scenario.objects) object.id];

private function zones(scenario:Scenario):Array<ScenarioId> {
	final result:Array<ScenarioId> = [];
	for (object in scenario.objects)
		switch object.placement {
			case TriggerZone(_):
				result.push(object.id);
			case _:
		}
	return result;
}

private function checkpoints(scenario:Scenario):Array<ScenarioId> {
	final result:Array<ScenarioId> = [];
	for (object in scenario.objects)
		switch object.placement {
			case Checkpoint(_):
				result.push(object.id);
			case _:
		}
	return result;
}

private function dialogueIds(scenario:Scenario):Array<ScenarioId>
	return [for (value in scenario.story.dialogues) value.id];

private function journalIds(scenario:Scenario):Array<ScenarioId>
	return [for (value in scenario.story.journal) value.id];

private function objectiveIds(scenario:Scenario):Array<ScenarioId>
	return [for (value in scenario.story.objectives) value.id];

private function sequenceIds(scenario:Scenario):Array<ScenarioId>
	return [for (value in scenario.flow.sequences) value.id];

private function timerIds(scenario:Scenario):Array<ScenarioId> {
	final result:Array<ScenarioId> = [];
	for (rule in scenario.flow.rules)
		switch rule.event {
			case TimerExpired(timer):
				if (!containsScenarioId(result, timer))
					result.push(timer);
			case _:
		}
	return result;
}

private function variableIds(scenario:Scenario, expected:Null<FlowValueKind>):Array<ScenarioId> {
	final result:Array<ScenarioId> = [];
	for (variable in scenario.flow.variables)
		if (expected == null || flowValueKind(variable.initial) == expected)
			result.push(variable.id);
	return result;
}

private function flowValueKind(value:caxecraft.scenario.CaxeFlow.FlowValue):FlowValueKind
	return switch value {
		case Flag(_): FlagValue;
		case Counter(_): CounterValue;
		case State(_): StateValue;
	};

private function containsScenarioId(values:Array<ScenarioId>, expected:ScenarioId):Bool {
	for (value in values)
		if (value.text() == expected.text())
			return true;
	return false;
}

private function eventDescriptor(syntax:String):Null<caxecraft.scenario.CaxeFlowEventRegistry.FlowEventDescriptor> {
	for (descriptor in allFlowEventDescriptors())
		if (descriptor.id.text() == syntax)
			return descriptor;
	return null;
}

private function predicateDescriptor(syntax:String):Null<caxecraft.scenario.CaxeFlowPredicateRegistry.FlowPredicateDescriptor> {
	for (descriptor in allFlowPredicateDescriptors())
		if (descriptor.id.text() == syntax)
			return descriptor;
	return null;
}

private function actionDescriptor(syntax:String):Null<caxecraft.scenario.CaxeFlowActionRegistry.FlowActionDescriptor> {
	for (descriptor in allFlowActionDescriptors())
		if (descriptor.id.text() == syntax)
			return descriptor;
	return null;
}

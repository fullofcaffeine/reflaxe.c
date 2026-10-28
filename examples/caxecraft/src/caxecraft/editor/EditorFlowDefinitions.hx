package caxecraft.editor;

import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioId;
import caxecraft.editor.EditorFlowReferences.EditorFlowReference;
import caxecraft.editor.EditorFlowReferences.EditorFlowReferenceRole;
import caxecraft.editor.EditorFlowReferences.collectActionFlowReferences;
import caxecraft.editor.EditorFlowReferences.collectEventFlowReferences;
import caxecraft.editor.EditorFlowReferences.collectPredicateFlowReferences;
import caxecraft.scenario.Scenario;

/**
	Builds safe visual edits for CaxeFlow variable and sequence declarations.

	The native panel asks this module for typed replacements and then submits the
	existing `PutFlowVariable` or `PutFlowSequence` command. No widget state is a
	second document model. Registry-owned states arrive as typed IDs, and session
	validation remains the authority when a type or scope change would break a
	rule, call, or parameter.
**/
/** One variable field changed by a child-facing declaration control. */
enum EditorFlowVariableEdit {
	NextVariableKind;
	NextVariableScope;
	PreviousVariableValue;
	NextVariableValue;
}

/** Create the first collision-free visual variable identity of one value kind. */
function nextFlowVariable(initial:FlowValue, variables:Array<FlowVariable>):FlowVariable {
	final family = switch initial {
		case Flag(_): "flag";
		case Counter(_): "counter";
		case State(_): "state";
	};
	return {
		id: nextVariableId('editor.variable.$family.n', variables),
		scope: Map,
		initial: initial
	};
}

/** Create one empty sequence whose stable ID does not replace an existing declaration. */
function nextFlowSequence(sequences:Array<FlowSequence>):FlowSequence
	return {id: nextSequenceId("editor.sequence.n", sequences), parameters: [], actions: []};

/** Create one flag parameter absent from global variables and this sequence. */
function nextFlowParameter(sequence:FlowSequence, variables:Array<FlowVariable>):{final id:ScenarioId; final initial:FlowValue;} {
	var number = 1;
	while (hasParameterIdentity('editor.parameter.n$number', sequence, variables))
		number++;
	return {id: new ScenarioId('editor.parameter.n$number'), initial: Flag(false)};
}

/** Return a copy-owned variable with exactly one visual field changed. */
function editFlowVariable(variable:FlowVariable, edit:EditorFlowVariableEdit, sequences:Array<FlowSequence>, states:Array<ContentId>):FlowVariable {
	return {
		id: variable.id,
		scope: switch edit {
			case NextVariableScope: nextScope(variable.scope, sequences);
			case _: variable.scope;
		},
		initial: switch edit {
			case NextVariableKind: nextKind(variable.initial, states);
			case PreviousVariableValue: stepValue(variable.initial, -1, states);
			case NextVariableValue: stepValue(variable.initial, 1, states);
			case NextVariableScope: variable.initial;
		}
	};
}

/** True when deleting one global variable would leave any typed Flow use behind. */
function flowVariableIsReferenced(scenario:Scenario, expected:ScenarioId):Bool {
	for (rule in scenario.flow.rules)
		if (referencesVariable(collectEventFlowReferences(rule.event), expected)
			|| referencesVariable(collectPredicateFlowReferences(rule.predicate), expected)
			|| actionsReferenceVariable(rule.actions, expected))
			return true;
	for (sequence in scenario.flow.sequences)
		if (actionsReferenceVariable(sequence.actions, expected))
			return true;
	return false;
}

/** True when deleting one sequence would break a local scope, call, or schedule. */
function flowSequenceIsReferenced(scenario:Scenario, expected:ScenarioId):Bool {
	for (variable in scenario.flow.variables)
		switch variable.scope {
			case Local(sequence) if (sameId(sequence, expected)):
				return true;
			case _:
		}
	for (rule in scenario.flow.rules)
		if (actionsReferenceSequence(rule.actions, expected))
			return true;
	for (sequence in scenario.flow.sequences)
		if (!sameId(sequence.id, expected) && actionsReferenceSequence(sequence.actions, expected))
			return true;
	return false;
}

private function actionsReferenceVariable(actions:Array<caxecraft.scenario.CaxeFlow.FlowAction>, expected:ScenarioId):Bool {
	for (action in actions)
		if (referencesVariable(collectActionFlowReferences(action), expected))
			return true;
	return false;
}

private function actionsReferenceSequence(actions:Array<caxecraft.scenario.CaxeFlow.FlowAction>, expected:ScenarioId):Bool {
	for (action in actions)
		if (referencesSequence(collectActionFlowReferences(action), expected))
			return true;
	return false;
}

private function referencesVariable(references:Array<EditorFlowReference>, expected:ScenarioId):Bool {
	for (reference in references)
		if (sameId(reference.id, expected))
			switch reference.role {
				case TypedVariableFlowReference(_) | AnyVariableFlowReference:
					return true;
				case _:
			}
	return false;
}

private function referencesSequence(references:Array<EditorFlowReference>, expected:ScenarioId):Bool {
	for (reference in references)
		if (sameId(reference.id, expected))
			switch reference.role {
				case SequenceFlowReference:
					return true;
				case _:
			}
	return false;
}

private inline function sameId(left:ScenarioId, right:ScenarioId):Bool
	return left.text() == right.text();

/** Cycle map/player/quest/local scopes without inventing an unresolved sequence. */
private function nextScope(scope:FlowScope, sequences:Array<FlowSequence>):FlowScope
	return switch scope {
		case Map: Player;
		case Player: Quest;
		case Quest: sequences.length == 0 ? Map : Local(sequences[0].id);
		case Local(current): final index = sequenceIndex(sequences, current); index >= 0 && index + 1 < sequences.length ? Local(sequences[index + 1].id) : Map;
	};

/** Cycle the closed value family and use only registry-owned state IDs. */
private function nextKind(value:FlowValue, states:Array<ContentId>):FlowValue
	return switch value {
		case Flag(_): Counter(0);
		case Counter(_): states.length == 0 ? Flag(false) : State(states[0]);
		case State(_): Flag(false);
	};

/** Toggle flags, step counters safely, or move through the shared state registry. */
private function stepValue(value:FlowValue, direction:Int, states:Array<ContentId>):FlowValue
	return switch value {
		case Flag(current): Flag(!current);
		case Counter(current): Counter(stepInt(current, direction));
		case State(current): State(stepState(current, direction, states));
	};

/** Keep a visual counter step inside Haxe's signed 32-bit `Int`. */
private function stepInt(value:Int, direction:Int):Int {
	if (direction < 0)
		return value == -2147483648 ? value : value - 1;
	if (direction > 0)
		return value == 2147483647 ? value : value + 1;
	return value;
}

/** Move cyclically through validated state IDs; retain unknown repair-mode data. */
private function stepState(current:ContentId, direction:Int, states:Array<ContentId>):ContentId {
	if (states.length == 0)
		return current;
	var index = contentIndex(states, current);
	if (index < 0)
		return current;
	index = (index + (direction < 0 ? -1 : direction > 0 ? 1 : 0)) % states.length;
	if (index < 0)
		index += states.length;
	return states[index];
}

private function nextVariableId(prefix:String, variables:Array<FlowVariable>):ScenarioId {
	var number = 1;
	while (hasVariableId(variables, prefix + number))
		number++;
	return new ScenarioId(prefix + number);
}

private function nextSequenceId(prefix:String, sequences:Array<FlowSequence>):ScenarioId {
	var number = 1;
	while (hasSequenceId(sequences, prefix + number))
		number++;
	return new ScenarioId(prefix + number);
}

private function hasVariableId(variables:Array<FlowVariable>, expected:String):Bool {
	for (variable in variables)
		if (variable.id.text() == expected)
			return true;
	return false;
}

private function hasSequenceId(sequences:Array<FlowSequence>, expected:String):Bool {
	for (sequence in sequences)
		if (sequence.id.text() == expected)
			return true;
	return false;
}

private function hasParameterIdentity(expected:String, sequence:FlowSequence, variables:Array<FlowVariable>):Bool {
	if (hasVariableId(variables, expected))
		return true;
	for (parameter in sequence.parameters)
		if (parameter.id.text() == expected)
			return true;
	return false;
}

private function sequenceIndex(sequences:Array<FlowSequence>, expected:ScenarioId):Int {
	for (index in 0...sequences.length)
		if (sequences[index].id.text() == expected.text())
			return index;
	return -1;
}

private function contentIndex(values:Array<ContentId>, expected:ContentId):Int {
	for (index in 0...values.length)
		if (values[index].text() == expected.text())
			return index;
	return -1;
}

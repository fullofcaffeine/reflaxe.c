package caxecraft.editor;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowChoice;
import caxecraft.scenario.CaxeFlow.FlowParameter;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.CaxeFlowCopy.copyFlowAction;
import caxecraft.scenario.CaxeFlowCopy.copyFlowPredicate;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;
import caxecraft.editor.EditorFlowDefinitions.flowParameterIsReferenced;

/**
	Rebinds one reusable sequence signature and every typed caller atomically.

	A parameter declaration is positional at runtime but has a stable authored
	ID. This module maps old arguments by that ID, preserves compatible values,
	uses the new default for a new or type-changed input, and renames parameter
	references inside the owning sequence body. The reducer publishes the returned
	arrays as one history entry, so no visible draft can contain half a signature.
**/
/** One exact visual edit to an ordered sequence parameter list. */
enum EditorFlowParameterEdit {
	InsertFlowParameter(index:Int, parameter:FlowParameter);
	ReplaceFlowParameter(index:Int, parameter:FlowParameter);
	RemoveFlowParameter(index:Int);
}

/** Copy-owned Flow arrays or one exact refusal reason. */
enum EditorFlowSignatureResult {
	FlowSignatureUpdated(variables:Array<FlowVariable>, sequences:Array<FlowSequence>, rules:Array<FlowRule>);
	FlowSignatureMissingSequence(id:ScenarioId);
	FlowSignatureInvalidIndex(id:ScenarioId, index:Int);
	FlowSignatureDuplicateParameter(id:ScenarioId);
	FlowSignatureParameterInUse(id:ScenarioId);
}

/** Apply one signature edit and rebind every matching call or schedule. */
function editFlowSequenceSignature(scenario:Scenario, sequenceId:ScenarioId, edit:EditorFlowParameterEdit):EditorFlowSignatureResult {
	final source = findSequence(scenario.flow.sequences, sequenceId);
	if (source == null)
		return FlowSignatureMissingSequence(sequenceId);
	final oldParameters = copyParameters(source.parameters);
	final newParameters = copyParameters(source.parameters);
	var renameBefore:Null<ScenarioId> = null;
	var renameAfter:Null<ScenarioId> = null;
	switch edit {
		case InsertFlowParameter(index, parameter):
			if (index < 0 || index > newParameters.length)
				return FlowSignatureInvalidIndex(sequenceId, index);
			newParameters.insert(index, copyParameter(parameter));
		case ReplaceFlowParameter(index, parameter):
			if (index < 0 || index >= newParameters.length)
				return FlowSignatureInvalidIndex(sequenceId, index);
			if (!sameValueKind(newParameters[index].initial, parameter.initial)
				&& flowParameterIsReferenced(source, newParameters[index].id))
				return FlowSignatureParameterInUse(newParameters[index].id);
			renameBefore = newParameters[index].id;
			renameAfter = parameter.id;
			newParameters[index] = copyParameter(parameter);
		case RemoveFlowParameter(index):
			if (index < 0 || index >= newParameters.length)
				return FlowSignatureInvalidIndex(sequenceId, index);
			if (flowParameterIsReferenced(source, newParameters[index].id))
				return FlowSignatureParameterInUse(newParameters[index].id);
			newParameters.splice(index, 1);
	}
	final duplicate = duplicateParameterId(newParameters, scenario.flow.variables);
	if (duplicate != null)
		return FlowSignatureDuplicateParameter(duplicate);
	final sequences:Array<FlowSequence> = [];
	for (sequence in scenario.flow.sequences) {
		final ownsParameters = sameId(sequence.id, sequenceId);
		sequences.push({
			id: sequence.id,
			parameters: ownsParameters ? copyParameters(newParameters) : copyParameters(sequence.parameters),
			actions: [
				for (action in sequence.actions)
					mapAction(action, sequenceId, oldParameters, newParameters, ownsParameters ? renameBefore : null, ownsParameters ? renameAfter : null)
			]
		});
	}
	final rules:Array<FlowRule> = [];
	for (rule in scenario.flow.rules)
		rules.push({
			id: rule.id,
			priority: rule.priority,
			repeat: rule.repeat,
			event: rule.event,
			predicate: copyFlowPredicate(rule.predicate),
			actions: [
				for (action in rule.actions)
					mapAction(action, sequenceId, oldParameters, newParameters, null, null)
			]
		});
	return FlowSignatureUpdated([
		for (variable in scenario.flow.variables)
			{id: variable.id, scope: variable.scope, initial: variable.initial}
	], sequences, rules);
}

/** Map parameter references and call arguments through every nested action. */
private function mapAction(action:FlowAction, target:ScenarioId, oldParameters:Array<FlowParameter>, newParameters:Array<FlowParameter>,
		renameBefore:Null<ScenarioId>, renameAfter:Null<ScenarioId>):FlowAction {
	return switch action {
		case SetFlag(variable, value): SetFlag(renamedNullable(variable, renameBefore, renameAfter), value);
		case SetCounter(variable, value): SetCounter(renamedNullable(variable, renameBefore, renameAfter), value);
		case AddCounter(variable, delta): AddCounter(renamedNullable(variable, renameBefore, renameAfter), delta);
		case SetState(variable, value): SetState(renamedNullable(variable, renameBefore, renameAfter), value);
		case Schedule(timer, ticks, sequence, arguments):
			Schedule(timer, ticks, sequence,
				sameId(sequence, target) ? rebindArguments(arguments, oldParameters, newParameters) : mapArguments(arguments, renameBefore, renameAfter));
		case CallSequence(sequence, arguments):
			CallSequence(sequence,
				sameId(sequence, target) ? rebindArguments(arguments, oldParameters, newParameters) : mapArguments(arguments, renameBefore, renameAfter));
		case ChooseSeeded(seed, choices):
			final mapped:Array<FlowChoice> = [];
			for (choice in choices)
				mapped.push({
					weight: choice.weight,
					actions: [
						for (nested in choice.actions)
							mapAction(nested, target, oldParameters, newParameters, renameBefore, renameAfter)
					]
				});
			ChooseSeeded(renamedNullable(seed, renameBefore, renameAfter), mapped);
		case _: copyFlowAction(action);
	};
}

/** Preserve arguments by stable parameter ID, then by compatible replacement position. */
private function rebindArguments(arguments:Array<FlowArgument>, oldParameters:Array<FlowParameter>, newParameters:Array<FlowParameter>):Array<FlowArgument> {
	final result:Array<FlowArgument> = [];
	for (newIndex in 0...newParameters.length) {
		final parameter = newParameters[newIndex];
		final oldIndex = parameterIndex(oldParameters, parameter.id);
		if (oldIndex >= 0 && oldIndex < arguments.length && sameValueKind(oldParameters[oldIndex].initial, parameter.initial))
			result.push(arguments[oldIndex]);
		else if (newParameters.length == oldParameters.length
			&& newIndex < oldParameters.length
			&& newIndex < arguments.length
			&& sameValueKind(oldParameters[newIndex].initial, parameter.initial))
			result.push(arguments[newIndex]);
		else
			result.push(Value(parameter.initial));
	}
	return result;
}

/** Rename variable-valued arguments only inside the owning sequence body. */
private function mapArguments(arguments:Array<FlowArgument>, before:Null<ScenarioId>, after:Null<ScenarioId>):Array<FlowArgument> {
	if (before == null || after == null)
		return arguments.copy();
	return [
		for (argument in arguments)
			switch argument {
				case Value(value):
					Value(value);
				case Variable(variable):
					Variable(renamed(variable, before, after));
			}
	];
}

private function duplicateParameterId(parameters:Array<FlowParameter>, variables:Array<FlowVariable>):Null<ScenarioId> {
	for (index in 0...parameters.length) {
		for (variable in variables)
			if (sameId(parameters[index].id, variable.id))
				return parameters[index].id;
		for (earlier in 0...index)
			if (sameId(parameters[index].id, parameters[earlier].id))
				return parameters[index].id;
	}
	return null;
}

private function findSequence(sequences:Array<FlowSequence>, expected:ScenarioId):Null<FlowSequence> {
	for (sequence in sequences)
		if (sameId(sequence.id, expected))
			return sequence;
	return null;
}

private function parameterIndex(parameters:Array<FlowParameter>, expected:ScenarioId):Int {
	for (index in 0...parameters.length)
		if (sameId(parameters[index].id, expected))
			return index;
	return -1;
}

private function sameValueKind(left:FlowValue, right:FlowValue):Bool
	return switch [left, right] {
		case [Flag(_), Flag(_)] | [Counter(_), Counter(_)] | [State(_), State(_)]: true;
		case _: false;
	};

private inline function renamed(value:ScenarioId, before:ScenarioId, after:ScenarioId):ScenarioId
	return sameId(value, before) ? after : value;

private inline function renamedNullable(value:ScenarioId, before:Null<ScenarioId>, after:Null<ScenarioId>):ScenarioId
	return before == null || after == null ? value : renamed(value, before, after);

private inline function copyParameter(parameter:FlowParameter):FlowParameter
	return {id: parameter.id, initial: parameter.initial};

private function copyParameters(parameters:Array<FlowParameter>):Array<FlowParameter>
	return [for (parameter in parameters) copyParameter(parameter)];

private inline function sameId(left:ScenarioId, right:ScenarioId):Bool
	return left.text() == right.text();

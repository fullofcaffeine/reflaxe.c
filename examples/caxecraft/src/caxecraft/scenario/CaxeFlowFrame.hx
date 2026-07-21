package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;

private final class CaxeFlowBinding {
	public final id:ScenarioId;
	public var value:FlowValue;

	public function new(id:ScenarioId, value:FlowValue) {
		this.id = id;
		this.value = value;
	}
}

/**
	One fresh set of parameters and local variables for a sequence call.

	The validator rejects same-named parameters and locals. Parameters are still
	added first so a hand-built invalid scenario behaves deterministically instead
	of making lookup depend on how the frame happened to be assembled.
**/
@:noCompletion
final class CaxeFlowFrame {
	final bindings:Array<CaxeFlowBinding> = [];

	public function new(scenario:Scenario, sequence:FlowSequence, arguments:Array<FlowValue>) {
		for (index in 0...sequence.parameters.length)
			bindings.push(new CaxeFlowBinding(sequence.parameters[index].id, arguments[index]));
		for (variable in scenario.flow.variables)
			switch variable.scope {
				case Local(owner) if (sameId(owner, sequence.id)):
					if (!contains(variable.id))
						bindings.push(new CaxeFlowBinding(variable.id, variable.initial));
				case _:
			}
	}

	public function contains(id:ScenarioId):Bool
		return find(id) != null;

	public function value(id:ScenarioId):Null<FlowValue> {
		final binding = find(id);
		return binding == null ? null : binding.value;
	}

	public function set(id:ScenarioId, value:FlowValue):Bool {
		final binding = find(id);
		if (binding == null)
			return false;
		binding.value = value;
		return true;
	}

	function find(id:ScenarioId):Null<CaxeFlowBinding> {
		for (binding in bindings)
			if (sameId(binding.id, id))
				return binding;
		return null;
	}

	static inline function sameId(left:ScenarioId, right:ScenarioId):Bool
		return left.text() == right.text();
}

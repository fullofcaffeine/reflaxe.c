package reflaxe.c.ir;

import reflaxe.c.ir.HxcIR;

/**
	Build the exact per-function root plan after whole-program representations settle.

	A value is rooted only when its type instance explicitly selects the `gc`
	representation. Parameters are live at function entry. Instruction results
	become live immediately after their defining instruction. Values stay rooted
	until the function exits in this first conservative plan; a later liveness pass
	may shorten those ranges without changing reachability semantics.
**/
class HxcIRManagedRootPlanner {
	public function new() {}

	public function run(program:HxcIRProgram):Void {
		final paths = new HxcIRManagedRootPaths(program);

		for (module in program.modules)
			for (fn in module.functions) {
				if (fn.managedRoots == null)
					throw new HxcIRManagedRootPlanningError('function `${fn.id}` has no schema-19 managed-root list');
				fn.managedRoots.resize(0);
				var ordinal = 0;
				for (parameter in fn.parameters)
					for (path in paths.collect(parameter.type))
						fn.managedRoots.push(root(ordinal++, parameter.id, path, parameter.source));
				for (block in fn.blocks) {
					for (parameter in block.parameters)
						if (paths.collect(parameter.type).length > 0)
							throw new HxcIRManagedRootPlanningError('function `${fn.id}` block `${block.id}` has managed parameter `${parameter.id}`; edge-owned root updates are not admitted yet');
					for (instruction in block.instructions)
						if (instruction.result != null)
							for (path in paths.collect(instruction.result.type))
								fn.managedRoots.push(root(ordinal++, instruction.result.id, path, instruction.source));
				}
			}
	}

	static function root(ordinal:Int, valueId:String, projections:Array<HxcIRManagedRootProjection>, source:HxcSourceSpan):HxcIRManagedRoot
		return {
			id: 'root.$ordinal',
			valueId: valueId,
			projections: projections,
			source: source
		};
}

/** A whole-program representation/root mismatch found before validation. */
class HxcIRManagedRootPlanningError extends haxe.Exception {}

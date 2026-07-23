package reflaxe.c.lowering;

import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRControlFlowAnalysis;
import reflaxe.c.ir.HxcIRControlFlowAnalysis.HxcIRControlFlowFacts;

/**
	Removes repeated class-reference null checks only when an earlier check proves
	the exact same immutable HxcIR value non-null on every route.

	Haxe class references are nullable, so field access and instance calls must
	fail predictably instead of letting C dereference a null pointer. The body
	builder deliberately starts with a check at every use because that local rule
	is easy to audit. This pass is the one owner that coalesces those conservative
	checks after the complete control-flow graph exists.

	Checks in the same block are processed in statement order. A check in another
	block is reusable only when that whole block strictly dominates the current
	block. A branch-local check therefore never escapes through a join, and a
	different load or block parameter has a different value ID and keeps its own
	check. Policy/profile differences also keep separate nodes so validation still
	sees them. The remaining `IRIONullCheck` stays explicit for validation and C
	AST emission; this pass never edits generated C text.
**/
class CBodyNullCheckCoalescing {
	public function new() {}

	/** Optimize every reachable function without changing the HxcIR schema. */
	public function run(program:HxcIRProgram):Void {
		for (module in program.modules)
			for (fn in module.functions)
				coalesceFunction(fn);
	}

	function coalesceFunction(fn:HxcIRFunction):Void {
		final controlFlow = new HxcIRControlFlowAnalysis().analyze(fn);
		final proofsByBlock = collectWholeBlockProofs(fn, controlFlow);
		for (block in fn.blocks) {
			final available:Map<String, Bool> = [];
			for (dominator in controlFlow.strictDominatorsOf(block.id)) {
				final proofs = proofsByBlock.get(dominator);
				if (proofs != null)
					for (proofKey in proofs.keys())
						available.set(proofKey, true);
			}

			final kept:Array<HxcIRInstruction> = [];
			for (instruction in block.instructions)
				switch instruction.kind {
					case IRIONullCheck(valueId, policy):
						final proofKey = nullProofKey(valueId, policy);
						if (!available.exists(proofKey)) {
							kept.push(instruction);
							available.set(proofKey, true);
						}
					case _:
						kept.push(instruction);
				}
			if (kept.length != block.instructions.length) {
				block.instructions.splice(0, block.instructions.length);
				for (instruction in kept)
					block.instructions.push(instruction);
			}
		}
	}

	/**
		Record proofs safe to reuse after leaving a block.

		An instruction-level failure edge can jump away before a later check, so a
		block with such an edge is conservatively denied whole-block proof credit.
		Local statement-order coalescing inside that block remains valid.
	**/
	static function collectWholeBlockProofs(fn:HxcIRFunction, controlFlow:HxcIRControlFlowFacts):Map<String, Map<String, Bool>> {
		final result:Map<String, Map<String, Bool>> = [];
		for (block in fn.blocks) {
			final proofs:Map<String, Bool> = [];
			if (!controlFlow.hasInstructionFailureJump(block.id))
				for (instruction in block.instructions)
					switch instruction.kind {
						case IRIONullCheck(valueId, policy):
							proofs.set(nullProofKey(valueId, policy), true);
						case _:
					}
			result.set(block.id, proofs);
		}
		return result;
	}

	/** Keep policy differences visible so validation cannot lose a bad policy. */
	static function nullProofKey(valueId:String, policy:HxcIRNullCheckPolicy):String
		return switch policy {
			case IRNCPCheckedAbort(profile, buildMode):
				'${valueId.length}:$valueId\x00${profile.length}:$profile\x00${buildMode.length}:$buildMode';
		};
}

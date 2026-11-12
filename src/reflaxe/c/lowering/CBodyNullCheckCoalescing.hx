package reflaxe.c.lowering;

import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIROwnedChildProofs;
import reflaxe.c.ir.HxcIROwnedChildFlow;
import reflaxe.c.ir.HxcIRValidator;
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
		final retainedWitnesses = removeOwnedChildChecks(fn, controlFlow);
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
						if (!available.exists(proofKey) || retainedWitnesses.exists(instruction.id)) {
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
		Remove child guards only from independently reconstructed branch-local facts.
		The shared transfer analysis intersects exact retained origins across every
		predecessor and cuts cycle/failure/cleanup edges. Its local-content facts never
		become legacy explicit checks. Keep guards on operands used outside their
		block, and retain all parent witnesses before the older dominance pass runs.
	**/
	function removeOwnedChildChecks(fn:HxcIRFunction, controlFlow:HxcIRControlFlowFacts):Map<String, Bool> {
		final flow = new HxcIROwnedChildFlow(fn, controlFlow);
		final witnesses = flow.retainedWitnesses();
		if (!witnesses.iterator().hasNext() || fn.cleanupRegions.length != 0)
			return witnesses;
		final retainedValues = HxcIRValidator.valuesNeedingExplicitNullChecks(fn);
		final counts:Map<String, Int> = [];
		for (block in fn.blocks)
			for (instruction in block.instructions) {
				final count = counts.get(instruction.id);
				counts.set(instruction.id, count == null ? 1 : count + 1);
			}
		for (block in fn.blocks) {
			if (controlFlow.hasInstructionFailureJump(block.id))
				continue;
			final facts = flow.forBlock(block.id);
			if (facts == null)
				continue;
			final types:Map<String, HxcIRTypeRef> = [];
			for (parameter in fn.parameters)
				types.set(parameter.id, parameter.type);
			for (parameter in block.parameters)
				types.set(parameter.id, parameter.type);
			final kept:Array<HxcIRInstruction> = [];
			for (instruction in block.instructions) {
				var remove = false;
				switch instruction.kind {
					case IRIONullCheck(valueId, policy):
						final known = facts.policy(valueId);
						final witness = facts.witness(valueId);
						if (!witnesses.exists(instruction.id)
							&& !retainedValues.exists(valueId)
							&& known != null
							&& witness != null
							&& HxcIROwnedChildProofs.samePolicy(known, policy)
							&& HxcIRValidator.canDiscardNullCheckEnvelope(instruction, counts.get(instruction.id) == 1)) {
							remove = true;
							witnesses.set(witness, true);
						}
					case _:
				}
				if (!remove) {
					kept.push(instruction);
					facts.observe(instruction, types);
					// Keep origins of already optimized uses when this pass runs again.
					switch instruction.kind {
						case IRIOBorrowClassField(_) if (instruction.result != null):
							final witness = facts.witness(instruction.result.id);
							if (witness != null)
								witnesses.set(witness, true);
						case _:
					}
				}
				if (instruction.result != null)
					types.set(instruction.result.id, instruction.result.type);
			}
			if (kept.length != block.instructions.length) {
				block.instructions.splice(0, block.instructions.length);
				for (instruction in kept)
					block.instructions.push(instruction);
			}
		}
		return witnesses;
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

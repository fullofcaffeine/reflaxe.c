package reflaxe.c.ir;

import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRControlFlowAnalysis.HxcIRControlFlowFacts;
import reflaxe.c.ir.HxcIROwnedChildProofs.OwnedChildFact;
import reflaxe.c.ir.HxcIRTraversal.instructionFailureEdge;

/**
	Carry an embedded-child borrow through a branch without inferring loop invariants.

	A local fact crosses a join only when every reachable predecessor has the same
	retained borrow and parent-check witness. Edges going backward in the existing
	reverse-postorder traversal contribute an empty fact set. Forward edges thus
	form an acyclic region even inside a loop; a fresh borrow in one iteration can
	cross its inner diamond, but cannot establish a fact for the next iteration.

	This analysis retains candidate evidence only. The validator reconstructs it
	from retained instructions and still validates every producer and lifetime.
	Loaded immutable values never cross an edge or enter legacy dominance maps.
	Cleanup edges clear transferred facts. Functions with implicit finally or
	setjmp entry paths keep embedded-child guards until this analysis models those paths.
**/
class HxcIROwnedChildFlow {
	final fn:HxcIRFunction;
	final incoming:Map<String, Map<String, OwnedChildFact>> = [];
	final active:Map<String, Bool> = [];
	final witnesses:Map<String, Bool> = [];

	/** Inspect only functions containing an embedded-child borrow; unrelated bodies stay cheap. */
	public function new(fn:HxcIRFunction, flow:HxcIRControlFlowFacts) {
		this.fn = fn;
		// The ordinary graph omits implicit entries into finally blocks. Such a
		// block can also reach a normal join, so clearing only its entry is unsafe.
		// Keep this exclusion shared with validation: an omitted guard must fail
		// validation even when no optimizer produced the input.
		for (region in fn.cleanupRegions)
			for (action in region.actions)
				switch action.kind {
					case IRCAFinally(_):
						return;
					case _:
				}
		if (fn.exceptionStrategy == IRESContainedRuntime)
			return;
		var hasBorrow = false;
		final blocks:Map<String, HxcIRBlock> = [];
		final instructions:Map<String, Bool> = [];
		final values:Map<String, Bool> = [];
		for (block in fn.blocks)
			for (instruction in block.instructions)
				switch instruction.kind {
					case IRIOBorrowClassField(_):
						hasBorrow = true;
					// C emission can defer setjmp until its result is consumed.
					// Until validation fixes that placement, the IR instruction is
					// not a sufficient boundary for facts after runtime re-entry.
					case IRIOException(IREFrameSetJmp(_)):
						return;
					case _:
				}
		if (!hasBorrow)
			return;
		// Ambiguous identities must remain visible to ordinary validation, never
		// become a reason to erase a check before that validation occurs.
		for (parameter in fn.parameters) {
			if (values.exists(parameter.id))
				return;
			values.set(parameter.id, true);
		}
		final locals:Map<String, Bool> = [];
		for (local in fn.locals) {
			if (locals.exists(local.id))
				return;
			locals.set(local.id, true);
		}
		for (block in fn.blocks) {
			if (blocks.exists(block.id))
				return;
			blocks.set(block.id, block);
			for (parameter in block.parameters) {
				if (values.exists(parameter.id))
					return;
				values.set(parameter.id, true);
			}
			for (instruction in block.instructions) {
				if (instructions.exists(instruction.id))
					return;
				instructions.set(instruction.id, true);
				if (instruction.result != null) {
					if (values.exists(instruction.result.id))
						return;
					values.set(instruction.result.id, true);
				}
			}
		}
		for (block in fn.blocks)
			for (target in HxcIRControlFlowAnalysis.blockTargets(block))
				if (!blocks.exists(target))
					return;
		final order = flow.orderedBlockIds();
		final rank:Map<String, Int> = [];
		for (index => id in order)
			rank.set(id, index);
		final outgoing:Map<String, Map<String, OwnedChildFact>> = [];
		for (id in order) {
			final block = blocks.get(id);
			if (block == null)
				continue;
			var joined:Null<Map<String, OwnedChildFact>> = null;
			if (id != fn.entryBlockId)
				for (predecessor in flow.predecessorsOf(id)) {
					// A cut edge participates as EMPTY, even if another predecessor
					// has already established the same local's type and policy.
					final predecessorRank = rank.get(predecessor);
					final blockRank = rank.get(id);
					final prior = predecessorRank != null
						&& blockRank != null
						&& predecessorRank < blockRank ? outgoing.get(predecessor) : null;
					if (prior == null) {
						joined = [];
						break;
					}
					if (joined == null)
						joined = prior.copy();
					else
						for (local in [for (key in joined.keys()) key]) {
							final candidate = prior.get(local);
							final current = joined.get(local);
							if (current == null)
								joined.remove(local);
							else if (candidate == null)
								joined.remove(local);
							else if (!HxcIROwnedChildProofs.sameOrigin(current, candidate))
								joined.remove(local);
						}
				}
			if (joined == null)
				joined = [];
			final tracker = reconstructBlock(block, joined);
			// Any exceptional/cleanup route empties the whole source's output.
			// This deliberately handles parallel clean and unsafe edges to the
			// same target without relying on the graph's deduplicated adjacency.
			outgoing.set(id, tracker != null && cleanExit(block) ? tracker.localSnapshot() : []);
		}
		// Validation also checks unreachable blocks. Preserve their existing
		// straight-line proof domain without inventing predecessor transport.
		for (block in fn.blocks)
			if (!rank.exists(block.id))
				reconstructBlock(block, []);
	}

	/** Reconstruct one local scope; callers alone decide whether its contents can leave. */
	function reconstructBlock(block:HxcIRBlock, joined:Map<String, OwnedChildFact>):Null<HxcIROwnedChildProofs> {
		var needed = joined.iterator().hasNext();
		for (instruction in block.instructions)
			switch instruction.kind {
				case IRIOBorrowClassField(_):
					needed = true;
				case _:
			}
		if (!needed)
			return null;
		incoming.set(block.id, joined);
		active.set(block.id, true);
		final tracker = new HxcIROwnedChildProofs(fn, joined);
		final available:Map<String, HxcIRTypeRef> = [];
		for (parameter in fn.parameters)
			available.set(parameter.id, parameter.type);
		for (parameter in block.parameters)
			available.set(parameter.id, parameter.type);
		for (fact in joined)
			witnesses.set(fact.witness, true);
		for (instruction in block.instructions) {
			tracker.observe(instruction, available);
			if (instruction.result != null) {
				final witness = tracker.witness(instruction.result.id);
				if (witness != null)
					witnesses.set(witness, true);
				available.set(instruction.result.id, instruction.result.type);
			}
		}
		return tracker;
	}

	/** A fresh block tracker; no optimizer-owned mutable state is shared with validation. */
	public function forBlock(id:String):Null<HxcIROwnedChildProofs>
		return active.exists(id) ? new HxcIROwnedChildProofs(fn, incoming.get(id)) : null;

	/** Keep every reconstructed origin explicit, including on repeated optimization passes. */
	public function retainedWitnesses():Map<String, Bool>
		return witnesses.copy();

	static function cleanExit(block:HxcIRBlock):Bool {
		for (instruction in block.instructions)
			if (instructionFailureEdge(instruction.kind) != null)
				return false;
		if (block.terminator == null)
			return false;
		return switch block.terminator.kind {
			case IRTJump(edge): edge.cleanup.length == 0;
			case IRTBranch(_, left, right): left.cleanup.length == 0 && right.cleanup.length == 0;
			case IRTSwitch(_, cases, otherwise): otherwise.cleanup.length == 0 && !cases.filter(item -> item.edge.cleanup.length != 0)
					.iterator()
					.hasNext();
			case IRTTagSwitch(_, cases, otherwise): (otherwise == null || otherwise.cleanup.length == 0) && !cases.filter(item -> item.edge.cleanup.length != 0)
					.iterator()
					.hasNext();
			case IRTReturn(_, cleanup): cleanup.length == 0;
			case IRTThrow(_, _): false;
			case IRTUnreachable: true;
		};
	}
}

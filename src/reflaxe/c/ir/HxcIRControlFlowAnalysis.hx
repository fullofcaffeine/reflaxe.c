package reflaxe.c.ir;

import reflaxe.c.ir.HxcIR;

/**
	Immutable control-flow facts for one HxcIR function.

	A block `A` dominates block `B` when every route from the function entry to
	`B` passes through `A`. Lowering and validation use that simple fact to reuse
	safety checks only when the check is guaranteed to have run first.
**/
class HxcIRControlFlowFacts {
	final blockIndices:Map<String, Int>;
	final strictDominators:Array<Array<String>>;
	final instructionFailureJumps:Map<String, Bool>;

	public function new(blockIndices:Map<String, Int>, strictDominators:Array<Array<String>>, instructionFailureJumps:Map<String, Bool>) {
		this.blockIndices = blockIndices;
		this.strictDominators = strictDominators;
		this.instructionFailureJumps = instructionFailureJumps;
	}

	/** True when `candidateBlockId` lies on every route to `blockId`. */
	public function dominates(candidateBlockId:String, blockId:String):Bool {
		final blockIndex = blockIndices.get(blockId);
		if (blockIndex == null)
			return false;
		if (candidateBlockId == blockId)
			return true;
		return containsUtf8(strictDominators[blockIndex], candidateBlockId);
	}

	/** True when the candidate dominates a different block. */
	public inline function strictlyDominates(candidateBlockId:String, blockId:String):Bool
		return candidateBlockId != blockId && dominates(candidateBlockId, blockId);

	/**
		Return strict dominators in stable order for deterministic proof searches.
	**/
	public function strictDominatorsOf(blockId:String):Array<String> {
		final blockIndex = blockIndices.get(blockId);
		return blockIndex == null ? [] : strictDominators[blockIndex].copy();
	}

	/**
		True when an instruction can jump to another block before the terminator.

		Such a block can still use a proof locally after it is established, but the
		whole block is not credited as one indivisible proof for later blocks.
	**/
	public inline function hasInstructionFailureJump(blockId:String):Bool
		return instructionFailureJumps.exists(blockId);

	static function containsUtf8(values:Array<String>, expected:String):Bool {
		var low = 0;
		var high = values.length;
		while (low < high) {
			final middle = low + ((high - low) >> 1);
			final comparison = compareUtf8(values[middle], expected);
			if (comparison < 0)
				low = middle + 1;
			else if (comparison > 0)
				high = middle;
			else
				return true;
		}
		return false;
	}

	static function compareUtf8(left:String, right:String):Int {
		return reflaxe.c.CUtf8Order.compare(left, right);
	}
}

/**
	Builds the shared control-flow proof used by HxcIR passes and validation.

	The analysis includes normal terminator edges and explicit instruction failure
	edges. It considers only blocks reachable from the declared entry. Blocks are
	assigned small integer indices, visited in reverse postorder, and reduced to
	one immediate-dominator tree. Walking that tree yields the same “which blocks
	must have run first?” answers without repeatedly copying a hash-map set for
	every block.

	This is the Cooper-Harvey-Kennedy iterative algorithm. Its tree-shaped result
	is especially useful here because callers ask only dominance membership and
	the stable list of strict dominators; they do not need mutable set algebra.
**/
class HxcIRControlFlowAnalysis {
	public function new() {}

	public function analyze(fn:HxcIRFunction):HxcIRControlFlowFacts {
		final blockIds:Array<String> = [];
		final blocks:Array<HxcIRBlock> = [];
		final allBlockIndices:Map<String, Int> = [];
		final instructionFailureJumps:Map<String, Bool> = [];
		for (block in fn.blocks) {
			final previousIndex = allBlockIndices.get(block.id);
			if (previousIndex == null) {
				allBlockIndices.set(block.id, blocks.length);
				blockIds.push(block.id);
				blocks.push(block);
			} else {
				// Invalid duplicate IDs are diagnosed later by HxcIRValidator.
				// Matching the former StringMap behavior here keeps the last
				// declaration as the graph node without hiding that diagnostic.
				blocks[previousIndex] = block;
			}
			if (blockHasInstructionFailureJump(block))
				instructionFailureJumps.set(block.id, true);
		}

		final successors:Array<Array<Int>> = [for (_ in blocks) []];
		final predecessors:Array<Array<Int>> = [for (_ in blocks) []];
		for (sourceIndex => source in blocks) {
			for (targetId in blockTargets(source)) {
				final targetIndex = allBlockIndices.get(targetId);
				if (targetIndex == null || successors[sourceIndex].indexOf(targetIndex) != -1)
					continue;
				successors[sourceIndex].push(targetIndex);
				if (predecessors[targetIndex].indexOf(sourceIndex) == -1)
					predecessors[targetIndex].push(sourceIndex);
			}
		}

		final entryIndex = allBlockIndices.get(fn.entryBlockId);
		if (entryIndex == null)
			return new HxcIRControlFlowFacts([], [for (_ in blocks) []], instructionFailureJumps);

		final reversePostorder = computeReversePostorder(entryIndex, successors);
		final immediateDominators = computeImmediateDominators(entryIndex, reversePostorder, predecessors, blocks.length);
		final reachableIndices:Map<String, Int> = [];
		final strictDominators:Array<Array<String>> = [for (_ in blocks) []];
		for (blockIndex in reversePostorder) {
			reachableIndices.set(blockIds[blockIndex], blockIndex);
			if (blockIndex == entryIndex)
				continue;
			final immediate = immediateDominators[blockIndex];
			if (immediate < 0)
				continue;
			final values = strictDominators[immediate].copy();
			insertUtf8(values, blockIds[immediate]);
			strictDominators[blockIndex] = values;
		}
		return new HxcIRControlFlowFacts(reachableIndices, strictDominators, instructionFailureJumps);
	}

	/**
		Return reachable blocks in reverse postorder without recursive host calls.

		Parents usually appear before children in this order. That lets the
		immediate-dominator fixed point converge quickly even for loops, while an
		explicit stack keeps deeply generated control flow from overflowing the
		Eval host's call stack.
	**/
	static function computeReversePostorder(entryIndex:Int, successors:Array<Array<Int>>):Array<Int> {
		final visited:Array<Bool> = [for (_ in successors) false];
		final nodes = [entryIndex];
		final nextSuccessors = [0];
		final postorder:Array<Int> = [];
		visited[entryIndex] = true;
		while (nodes.length > 0) {
			final stackIndex = nodes.length - 1;
			final blockIndex = nodes[stackIndex];
			final successorIndex = nextSuccessors[stackIndex];
			if (successorIndex < successors[blockIndex].length) {
				nextSuccessors[stackIndex] = successorIndex + 1;
				final targetIndex = successors[blockIndex][successorIndex];
				if (!visited[targetIndex]) {
					visited[targetIndex] = true;
					nodes.push(targetIndex);
					nextSuccessors.push(0);
				}
			} else {
				nodes.pop();
				nextSuccessors.pop();
				postorder.push(blockIndex);
			}
		}
		postorder.reverse();
		return postorder;
	}

	/**
		Compute one immediate parent in the dominator tree for each reachable block.

		`-1` means that the block has not been reached by the fixed point. The entry
		points to itself, giving `intersectDominators` a finite root.
	**/
	static function computeImmediateDominators(entryIndex:Int, reversePostorder:Array<Int>, predecessors:Array<Array<Int>>, blockCount:Int):Array<Int> {
		final positions:Array<Int> = [for (_ in 0...blockCount) -1];
		for (position => blockIndex in reversePostorder)
			positions[blockIndex] = position;
		final immediateDominators:Array<Int> = [for (_ in 0...blockCount) -1];
		immediateDominators[entryIndex] = entryIndex;

		var changed = true;
		while (changed) {
			changed = false;
			for (position in 1...reversePostorder.length) {
				final blockIndex = reversePostorder[position];
				var nextImmediate = -1;
				for (predecessor in predecessors[blockIndex]) {
					if (immediateDominators[predecessor] < 0)
						continue;
					nextImmediate = nextImmediate < 0 ? predecessor : intersectDominators(predecessor, nextImmediate, immediateDominators, positions);
				}
				if (nextImmediate >= 0 && immediateDominators[blockIndex] != nextImmediate) {
					immediateDominators[blockIndex] = nextImmediate;
					changed = true;
				}
			}
		}
		return immediateDominators;
	}

	/** Walk two known parent chains until they meet at the nearest shared proof. */
	static function intersectDominators(leftIndex:Int, rightIndex:Int, immediateDominators:Array<Int>, positions:Array<Int>):Int {
		var left = leftIndex;
		var right = rightIndex;
		while (left != right) {
			while (positions[left] > positions[right])
				left = immediateDominators[left];
			while (positions[right] > positions[left])
				right = immediateDominators[right];
		}
		return left;
	}

	/** Insert one new tree parent while preserving byte-wise UTF-8 order. */
	static function insertUtf8(values:Array<String>, value:String):Void {
		var low = 0;
		var high = values.length;
		while (low < high) {
			final middle = low + ((high - low) >> 1);
			if (compareUtf8(values[middle], value) < 0)
				low = middle + 1;
			else
				high = middle;
		}
		values.insert(low, value);
	}

	/** Collect normal and explicit failure successors without trusting their IDs. */
	static function blockTargets(block:HxcIRBlock):Array<String> {
		final result:Array<String> = [];
		function add(target:String):Void {
			if (result.indexOf(target) == -1)
				result.push(target);
		}
		for (instruction in block.instructions) {
			final failure = instructionFailure(instruction.kind);
			if (failure != null)
				switch failure.target {
					case IRFTBlock(target):
						add(target);
					case IRFTPropagate | IRFTAbort:
				}
		}
		if (block.terminator != null)
			switch block.terminator.kind {
				case IRTJump(edge):
					add(edge.targetBlockId);
				case IRTBranch(_, whenTrue, whenFalse):
					add(whenTrue.targetBlockId);
					add(whenFalse.targetBlockId);
				case IRTSwitch(_, cases, defaultEdge):
					for (item in cases)
						add(item.edge.targetBlockId);
					add(defaultEdge.targetBlockId);
				case IRTTagSwitch(_, cases, defaultEdge):
					for (item in cases)
						add(item.edge.targetBlockId);
					if (defaultEdge != null)
						add(defaultEdge.targetBlockId);
				case IRTThrow(_, failure):
					switch failure.target {
						case IRFTBlock(target): add(target);
						case IRFTPropagate | IRFTAbort:
					}
				case IRTReturn(_, _) | IRTUnreachable:
			}
		return result;
	}

	static function blockHasInstructionFailureJump(block:HxcIRBlock):Bool {
		for (instruction in block.instructions) {
			final failure = instructionFailure(instruction.kind);
			if (failure != null)
				switch failure.target {
					case IRFTBlock(_):
						return true;
					case IRFTPropagate | IRFTAbort:
				}
		}
		return false;
	}

	static function instructionFailure(kind:HxcIRInstructionKind):Null<HxcIRFailureEdge>
		return switch kind {
			case IRIOCall(call): call.failure;
			case IRIOConvert(_, _, _, _, failure) | IRIOAllocate(_, _, _, failure): failure;
			case _: null;
		};

	static function compareUtf8(left:String, right:String):Int {
		return reflaxe.c.CUtf8Order.compare(left, right);
	}
}

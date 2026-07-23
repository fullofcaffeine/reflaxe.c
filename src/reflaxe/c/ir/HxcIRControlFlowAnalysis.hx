package reflaxe.c.ir;

import reflaxe.c.ir.HxcIR;

/**
	Immutable control-flow facts for one HxcIR function.

	A block `A` dominates block `B` when every route from the function entry to
	`B` passes through `A`. Lowering and validation use that simple fact to reuse
	safety checks only when the check is guaranteed to have run first.
**/
class HxcIRControlFlowFacts {
	final dominators:Map<String, Map<String, Bool>>;
	final instructionFailureJumps:Map<String, Bool>;
	final blockOrder:Array<String>;

	public function new(dominators:Map<String, Map<String, Bool>>, instructionFailureJumps:Map<String, Bool>) {
		this.dominators = dominators;
		this.instructionFailureJumps = instructionFailureJumps;
		this.blockOrder = [for (blockId in dominators.keys()) blockId];
		this.blockOrder.sort(compareUtf8);
	}

	/** True when `candidateBlockId` lies on every route to `blockId`. */
	public function dominates(candidateBlockId:String, blockId:String):Bool {
		final values = dominators.get(blockId);
		return values != null && values.exists(candidateBlockId);
	}

	/** True when the candidate dominates a different block. */
	public inline function strictlyDominates(candidateBlockId:String, blockId:String):Bool
		return candidateBlockId != blockId && dominates(candidateBlockId, blockId);

	/**
		Return strict dominators in stable order for deterministic proof searches.
	**/
	public function strictDominatorsOf(blockId:String):Array<String> {
		final result:Array<String> = [];
		final values = dominators.get(blockId);
		if (values != null)
			for (candidate in blockOrder)
				if (candidate != blockId)
					if (values.exists(candidate))
						result.push(candidate);
		return result;
	}

	/**
		True when an instruction can jump to another block before the terminator.

		Such a block can still use a proof locally after it is established, but the
		whole block is not credited as one indivisible proof for later blocks.
	**/
	public inline function hasInstructionFailureJump(blockId:String):Bool
		return instructionFailureJumps.exists(blockId);

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = haxe.io.Bytes.ofString(left);
		final rightBytes = haxe.io.Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}
}

/**
	Builds the shared control-flow proof used by HxcIR passes and validation.

	The analysis includes normal terminator edges and explicit instruction failure
	edges. It considers only blocks reachable from the declared entry, then uses
	the standard predecessor-intersection algorithm to find dominators.
**/
class HxcIRControlFlowAnalysis {
	public function new() {}

	public function analyze(fn:HxcIRFunction):HxcIRControlFlowFacts {
		final blocks:Map<String, HxcIRBlock> = [];
		final successors:Map<String, Array<String>> = [];
		final predecessors:Map<String, Array<String>> = [];
		final instructionFailureJumps:Map<String, Bool> = [];
		for (block in fn.blocks) {
			blocks.set(block.id, block);
			predecessors.set(block.id, []);
			successors.set(block.id, blockTargets(block));
			if (blockHasInstructionFailureJump(block))
				instructionFailureJumps.set(block.id, true);
		}
		for (source in fn.blocks) {
			final targets = successors.get(source.id);
			if (targets == null)
				continue;
			for (target in targets) {
				final incoming = predecessors.get(target);
				if (incoming != null && incoming.indexOf(source.id) == -1)
					incoming.push(source.id);
			}
		}

		final reachable:Map<String, Bool> = [];
		if (blocks.exists(fn.entryBlockId)) {
			final pending = [fn.entryBlockId];
			var next = 0;
			while (next < pending.length) {
				final blockId = pending[next++];
				if (reachable.exists(blockId))
					continue;
				reachable.set(blockId, true);
				final outgoing = successors.get(blockId);
				if (outgoing != null)
					for (target in outgoing)
						if (blocks.exists(target) && !reachable.exists(target))
							pending.push(target);
			}
		}

		final allReachable:Map<String, Bool> = [];
		for (blockId in reachable.keys())
			allReachable.set(blockId, true);
		final dominators:Map<String, Map<String, Bool>> = [];
		for (blockId in reachable.keys())
			dominators.set(blockId, blockId == fn.entryBlockId ? singleton(blockId) : copySet(allReachable));

		var changed = true;
		while (changed) {
			changed = false;
			for (blockId in reachable.keys()) {
				if (blockId == fn.entryBlockId)
					continue;
				final incoming = predecessors.get(blockId);
				if (incoming == null)
					continue;
				final reachableIncoming = incoming.filter(value -> reachable.exists(value));
				if (reachableIncoming.length == 0)
					continue;
				final first = dominators.get(reachableIncoming[0]);
				if (first == null)
					continue;
				var nextSet = copySet(first);
				for (index in 1...reachableIncoming.length) {
					final candidate = dominators.get(reachableIncoming[index]);
					if (candidate != null)
						nextSet = intersectSets(nextSet, candidate);
				}
				nextSet.set(blockId, true);
				final current = dominators.get(blockId);
				if (current == null || !sameSet(current, nextSet)) {
					dominators.set(blockId, nextSet);
					changed = true;
				}
			}
		}
		return new HxcIRControlFlowFacts(dominators, instructionFailureJumps);
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

	static function singleton(value:String):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		result.set(value, true);
		return result;
	}

	static function copySet(source:Map<String, Bool>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		for (key in source.keys())
			result.set(key, true);
		return result;
	}

	static function intersectSets(left:Map<String, Bool>, right:Map<String, Bool>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		for (key in left.keys())
			if (right.exists(key))
				result.set(key, true);
		return result;
	}

	static function sameSet(left:Map<String, Bool>, right:Map<String, Bool>):Bool {
		var leftCount = 0;
		for (key in left.keys()) {
			leftCount++;
			if (!right.exists(key))
				return false;
		}
		var rightCount = 0;
		for (_ in right.keys())
			rightCount++;
		return leftCount == rightCount;
	}
}

package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import reflaxe.c.ir.HxcIR;

/** Module anchor; the public planner and plan are nested types below. */
class CBodyValueCoalescing {}

/** Why a generated HxcIR value must remain a named C temporary. */
enum CBodyValueMaterializationReason {
	CBVMRMultipleUses(count:Int);
	CBVMRCrossBlock;
	CBVMRMutableOrForeignPlace;
	CBVMREffectOrReadBarrier(instructionId:String);
	CBVMRExpressionFanout;
	CBVMRFailureOrLifetimeBoundary;
	CBVMRUnsupportedProducer;
}

/** The closed, reviewable decision for one HxcIR result. */
enum CBodyValueDisposition {
	/** A total compiler-owned value expression can stay structural in CAST. */
	CBVDInlinePure;

	/** A local read can move to its one final sink without crossing another read/effect. */
	CBVDInlineSequencedLoad;

	CBVDMaterialize(reason:CBodyValueMaterializationReason);
}

private enum CBodyValueUseSite {
	CBVUInstruction(blockId:String, instructionIndex:Int);
	CBVUTerminator(blockId:String, instructionIndex:Int);
	CBVUCleanup;
}

private typedef CBodyValueProducer = {
	final blockId:String;
	final instructionIndex:Int;
	final instruction:HxcIRInstruction;
}

/** Immutable per-function decisions consumed before names and C syntax are chosen. */
class CBodyValueCoalescingPlan {
	final dispositions:Map<String, CBodyValueDisposition>;

	public function new(dispositions:Map<String, CBodyValueDisposition>)
		this.dispositions = dispositions;

	public function disposition(valueId:String):CBodyValueDisposition
		return dispositions.get(valueId) ?? CBVDMaterialize(CBVMRUnsupportedProducer);

	public function shouldInline(valueId:String):Bool
		return switch disposition(valueId) {
			case CBVDInlinePure | CBVDInlineSequencedLoad: true;
			case CBVDMaterialize(_): false;
		};

	public function shouldInlinePure(valueId:String):Bool
		return disposition(valueId) == CBVDInlinePure;

	public function shouldInlineSequencedLoad(valueId:String):Bool
		return disposition(valueId) == CBVDInlineSequencedLoad;
}

/**
	Find only the easy, mechanically safe temporary eliminations.

	This is deliberately narrower than a general optimizer. Mutable reads remain
	separate unless one local read reaches one sink in the same block without
	crossing any other read, call, store, check, failure, alias, or lifetime event.
	Compiler-owned record/enum expressions are total and non-volatile, so a
	single-use value can remain structural without changing when an observable
	operation happens.
**/
class CBodyValueCoalescingPlanner {
	var uses:Map<String, Array<CBodyValueUseSite>> = [];
	var producers:Map<String, CBodyValueProducer> = [];
	var blocks:Map<String, HxcIRBlock> = [];

	public function new() {}

	public function plan(fn:HxcIRFunction):CBodyValueCoalescingPlan {
		// A planner is cheap, but resetting here also makes accidental reuse safe.
		// No function may inherit uses or block identities from an earlier build.
		uses = [];
		producers = [];
		blocks = [];
		collect(fn);
		final dispositions:Map<String, CBodyValueDisposition> = [];

		for (valueId => producer in producers) {
			dispositions.set(valueId, CBVDMaterialize(CBVMRUnsupportedProducer));
			if (isPureStructuralProducer(producer.instruction.kind)) {
				final valueUses = valueUses(valueId);
				if (valueUses.length > 1) {
					dispositions.set(valueId, CBVDMaterialize(CBVMRMultipleUses(valueUses.length)));
				} else if (valueUses.length == 0 || useIsInBlock(valueUses[0], producer.blockId)) {
					dispositions.set(valueId, CBVDInlinePure);
				} else {
					dispositions.set(valueId, CBVDMaterialize(CBVMRCrossBlock));
				}
			}
		}

		for (valueId => producer in producers) {
			switch producer.instruction.kind {
				case IRIOLoad(place):
					dispositions.set(valueId, loadDisposition(valueId, producer, place, dispositions));
				case _:
			}
		}
		return new CBodyValueCoalescingPlan(dispositions);
	}

	function collect(fn:HxcIRFunction):Void {
		for (block in fn.blocks) {
			blocks.set(block.id, block);
			for (index in 0...block.instructions.length) {
				final instruction = block.instructions[index];
				if (instruction.result != null) {
					producers.set(instruction.result.id, {
						blockId: block.id,
						instructionIndex: index,
						instruction: instruction
					});
				}
				collectInstructionUses(instruction.kind, CBVUInstruction(block.id, index));
			}
			if (block.terminator != null)
				collectTerminatorUses(block.terminator.kind, CBVUTerminator(block.id, block.instructions.length));
		}
		for (region in fn.cleanupRegions) {
			for (action in region.actions) {
				switch action.kind {
					case IRCADestroy(place, _, _) | IRCARelease(place, _) | IRCADeallocate(place, _):
						collectPlaceUses(place, CBVUCleanup);
					case IRCAFinally(_):
				}
			}
		}
	}

	function loadDisposition(valueId:String, producer:CBodyValueProducer, place:HxcIRPlace,
			dispositions:Map<String, CBodyValueDisposition>):CBodyValueDisposition {
		switch producer.instruction.result.type {
			case IRTSpan(_, _):
				return CBVDMaterialize(CBVMRMutableOrForeignPlace);
			case _:
		}
		if (!isPrivateLocalPlace(place))
			return CBVDMaterialize(CBVMRMutableOrForeignPlace);
		final valueUses = valueUses(valueId);
		if (valueUses.length != 1)
			return CBVDMaterialize(CBVMRMultipleUses(valueUses.length));
		final sink = traceSink(producer, valueUses[0], dispositions);
		if (sink == null)
			return CBVDMaterialize(CBVMRExpressionFanout);
		if (sink.crossesCleanup)
			return CBVDMaterialize(CBVMRFailureOrLifetimeBoundary);
		final sinkBlock = sink.blockId;
		if (sinkBlock != producer.blockId)
			return CBVDMaterialize(CBVMRCrossBlock);
		final block = blocks.get(producer.blockId);
		if (block == null)
			return CBVDMaterialize(CBVMRUnsupportedProducer);
		for (index in producer.instructionIndex + 1...sink.instructionIndex) {
			final crossed = block.instructions[index];
			if (isReadOrEffectBarrier(crossed.kind))
				return CBVDMaterialize(barrierReason(crossed));
		}
		return CBVDInlineSequencedLoad;
	}

	function traceSink(producer:CBodyValueProducer, firstUse:CBodyValueUseSite,
			dispositions:Map<String, CBodyValueDisposition>):Null<{blockId:String, instructionIndex:Int, crossesCleanup:Bool}> {
		var use = firstUse;
		final visited:Map<String, Bool> = [];
		while (true) {
			switch use {
				case CBVUCleanup:
					return null;
				case CBVUTerminator(blockId, instructionIndex):
					return blockId == producer.blockId && instructionIndex > producer.instructionIndex ? {
						blockId: blockId,
						instructionIndex: instructionIndex,
						crossesCleanup: terminatorHasCleanup(blockId)
					} : null;
				case CBVUInstruction(blockId, instructionIndex):
					if (blockId != producer.blockId || instructionIndex <= producer.instructionIndex)
						return null;
					final block = blocks.get(blockId);
					if (block == null || instructionIndex >= block.instructions.length)
						return null;
					final consumer = block.instructions[instructionIndex];
					final result = consumer.result;
					if (result == null || !isTransparentExpression(consumer.kind, result.id, dispositions)) {
						return {blockId: blockId, instructionIndex: instructionIndex, crossesCleanup: false};
					}
					if (visited.exists(result.id))
						return null;
					visited.set(result.id, true);
					final resultUses = valueUses(result.id);
					if (resultUses.length != 1)
						return null;
					use = resultUses[0];
			}
		}
	}

	/**
		A return/throw value is evaluated before its edge cleanup in Haxe semantics.
		The C emitter currently emits the already-computed value after cleanup, so a
		load must remain materialized whenever its terminator owns cleanup work.
		Treat every cleanup-carrying edge conservatively here; this stays correct as
		more than today's no-op direct-storage cleanup becomes admissible.
	**/
	function terminatorHasCleanup(blockId:String):Bool {
		final block = blocks.get(blockId);
		if (block == null || block.terminator == null)
			return true;
		return switch block.terminator.kind {
			case IRTJump(edge): edge.cleanup.length > 0;
			case IRTBranch(_, whenTrue, whenFalse): whenTrue.cleanup.length > 0 || whenFalse.cleanup.length > 0;
			case IRTSwitch(_, cases, defaultEdge): defaultEdge.cleanup.length > 0 || switchCasesHaveCleanup(cases);
			case IRTTagSwitch(_, cases, defaultEdge): (defaultEdge != null && defaultEdge.cleanup.length > 0) || tagSwitchCasesHaveCleanup(cases);
			case IRTReturn(_, cleanup): cleanup.length > 0;
			case IRTThrow(_, edge): edge.cleanup.length > 0;
			case IRTUnreachable: false;
		};
	}

	static function switchCasesHaveCleanup(cases:Array<HxcIRSwitchCase>):Bool {
		for (item in cases) {
			if (item.edge.cleanup.length > 0)
				return true;
		}
		return false;
	}

	static function tagSwitchCasesHaveCleanup(cases:Array<HxcIRTagSwitchCase>):Bool {
		for (item in cases) {
			if (item.edge.cleanup.length > 0)
				return true;
		}
		return false;
	}

	function isTransparentExpression(kind:HxcIRInstructionKind, resultId:String, dispositions:Map<String, CBodyValueDisposition>):Bool {
		return switch kind {
			case IRIOUnary(_, _, IRIStatic | IRIProgramLocal(_)) | IRIOBinary(_, _, _, IRIStatic | IRIProgramLocal(_)): true;
			case IRIOConvert(_, _, _, IRIStatic | IRIProgramLocal(_), null): true;
			case IRIOConstructAggregate(_, _) | IRIOProject(_, _) | IRIOConstructTag(_, _, _) | IRIOMatchTag(_, _):
				dispositions.get(resultId) == CBVDInlinePure;
			case _: false;
		};
	}

	static function isPureStructuralProducer(kind:HxcIRInstructionKind):Bool {
		return switch kind {
			case IRIOConstructAggregate(_, _) | IRIOProject(_, _) | IRIOConstructTag(_, _, _) | IRIOMatchTag(_, _): true;
			case _: false;
		};
	}

	static function isPrivateLocalPlace(place:HxcIRPlace):Bool {
		return switch place {
			case IRPLocal(_): true;
			case IRPField(base, _): isPrivateLocalPlace(base);
			case IRPGlobal(_) | IRPDereference(_) | IRPIndex(_, _): false;
		};
	}

	static function isReadOrEffectBarrier(kind:HxcIRInstructionKind):Bool {
		return switch kind {
			case IRIOConstant(_) | IRIOUnary(_, _, IRIStatic | IRIProgramLocal(_)) | IRIOBinary(_, _, _, IRIStatic | IRIProgramLocal(_)) |
				IRIOConvert(_, _, _, IRIStatic | IRIProgramLocal(_), null) | IRIOConstructAggregate(_, _) | IRIOProject(_, _) | IRIOConstructTag(_, _, _) |
				IRIOMatchTag(_, _):
				false;
			case _: true;
		};
	}

	static function barrierReason(instruction:HxcIRInstruction):CBodyValueMaterializationReason {
		return switch instruction.kind {
			case IRIOLifetime(_, _, _, _) | IRIOAddress(_) | IRIOBorrowClassField(_) | IRIOProjectTag(_, _, _, _):
				CBVMRFailureOrLifetimeBoundary;
			case IRIOConvert(_, _, _, _, failure) if (failure != null):
				CBVMRFailureOrLifetimeBoundary;
			case _: CBVMREffectOrReadBarrier(instruction.id);
		};
	}

	function valueUses(valueId:String):Array<CBodyValueUseSite> {
		final result = uses.get(valueId);
		return result == null ? [] : result;
	}

	static function useIsInBlock(use:CBodyValueUseSite, blockId:String):Bool {
		return switch use {
			case CBVUInstruction(owner, _) | CBVUTerminator(owner, _): owner == blockId;
			case CBVUCleanup: false;
		};
	}

	function addUse(valueId:String, site:CBodyValueUseSite):Void {
		var valueUses = uses.get(valueId);
		if (valueUses == null) {
			valueUses = [];
			uses.set(valueId, valueUses);
		}
		valueUses.push(site);
	}

	function collectPlaceUses(place:HxcIRPlace, site:CBodyValueUseSite):Void {
		switch place {
			case IRPDereference(pointerValueId):
				addUse(pointerValueId, site);
			case IRPField(base, _):
				collectPlaceUses(base, site);
			case IRPIndex(base, indexValueId):
				collectPlaceUses(base, site);
				addUse(indexValueId, site);
			case IRPLocal(_) | IRPGlobal(_):
		}
	}

	function collectFailureUses(edge:Null<HxcIRFailureEdge>, site:CBodyValueUseSite):Void {
		if (edge == null)
			return;
		for (argument in edge.arguments)
			addUse(argument, site);
	}

	function collectCallUses(call:HxcIRCall, site:CBodyValueUseSite):Void {
		switch call.dispatch {
			case IRCDVirtual(_, receiverValueId) | IRCDInterface(_, _, receiverValueId):
				addUse(receiverValueId, site);
			case IRCDClosure(callableValueId):
				addUse(callableValueId, site);
			case IRCDDirect(_) | IRCDNative(_) | IRCDRuntime(_, _) | IRCDIntrinsic(_):
		}
		for (argument in call.arguments)
			addUse(argument, site);
		collectFailureUses(call.failure, site);
	}

	function collectInstructionUses(kind:HxcIRInstructionKind, site:CBodyValueUseSite):Void {
		switch kind {
			case IRIOSequence(_) | IRIOConstant(_) | IRIOFunctionReference(_):
			case IRIOLoad(place) | IRIOAddress(place) | IRIOBorrowClassField(place) | IRIODeallocate(place, _) | IRIORetain(place, _) |
				IRIORelease(place, _) | IRIOTrace(place, _) | IRIODefaultInitialize(place, _, _) | IRIOBindVirtualTable(place, _) |
				IRIOLifetime(place, _, _, _):
				collectPlaceUses(place, site);
			case IRIOStore(place, valueId) | IRIOInitialize(place, valueId, _, _):
				collectPlaceUses(place, site);
				addUse(valueId, site);
			case IRIOUnary(_, valueId, _):
				addUse(valueId, site);
			case IRIOBinary(_, leftValueId, rightValueId, _):
				addUse(leftValueId, site);
				addUse(rightValueId, site);
			case IRIOConvert(valueId, _, _, _, failure):
				addUse(valueId, site);
				collectFailureUses(failure, site);
			case IRIOCall(call):
				collectCallUses(call, site);
			case IRIOConstructAggregate(_, fields):
				for (field in fields)
					addUse(field.valueId, site);
			case IRIOConstructInterface(_, objectValueId, _):
				addUse(objectValueId, site);
			case IRIOProject(valueId, _) | IRIOMatchTag(valueId, _) | IRIOProjectTag(valueId, _, _, _):
				addUse(valueId, site);
			case IRIOConstructTag(_, _, payload):
				for (valueId in payload)
					addUse(valueId, site);
			case IRIOAllocate(_, _, _, failure):
				collectFailureUses(failure, site);
			case IRIOInitializeFixedArray(place, values, _, _):
				collectPlaceUses(place, site);
				for (valueId in values)
					addUse(valueId, site);
			case IRIOZeroInitializeFixedArray(place, _, _):
				collectPlaceUses(place, site);
			case IRIOInitializeSpan(place, sourceArray, _, _):
				collectPlaceUses(place, site);
				collectPlaceUses(sourceArray, site);
			case IRIOBoundsCheck(collection, indexValueId, _):
				collectPlaceUses(collection, site);
				addUse(indexValueId, site);
			case IRIONullCheck(valueId, _):
				addUse(valueId, site);
		}
	}

	function collectEdgeUses(edge:HxcIRBlockEdge, site:CBodyValueUseSite):Void {
		for (argument in edge.arguments)
			addUse(argument, site);
	}

	function collectTerminatorUses(kind:HxcIRTerminatorKind, site:CBodyValueUseSite):Void {
		switch kind {
			case IRTJump(edge):
				collectEdgeUses(edge, site);
			case IRTBranch(conditionValueId, whenTrue, whenFalse):
				addUse(conditionValueId, site);
				collectEdgeUses(whenTrue, site);
				collectEdgeUses(whenFalse, site);
			case IRTSwitch(valueId, cases, defaultEdge):
				addUse(valueId, site);
				for (item in cases)
					collectEdgeUses(item.edge, site);
				collectEdgeUses(defaultEdge, site);
			case IRTTagSwitch(valueId, cases, defaultEdge):
				addUse(valueId, site);
				for (item in cases)
					collectEdgeUses(item.edge, site);
				if (defaultEdge != null)
					collectEdgeUses(defaultEdge, site);
			case IRTReturn(valueId, _):
				if (valueId != null)
					addUse(valueId, site);
			case IRTThrow(valueId, edge):
				addUse(valueId, site);
				collectFailureUses(edge, site);
			case IRTUnreachable:
		}
	}
}
#else
class CBodyValueCoalescing {}
class CBodyValueCoalescingPlan {}
class CBodyValueCoalescingPlanner {}
#end

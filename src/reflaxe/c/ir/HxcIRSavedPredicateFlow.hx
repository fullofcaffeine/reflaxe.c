package reflaxe.c.ir;

import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRControlFlowAnalysis.HxcIRControlFlowFacts;
import reflaxe.c.ir.HxcIRTraversal.HxcIRTraversalSite;
import reflaxe.c.ir.HxcIRTraversal.HxcIRTraversalVisitor;
import reflaxe.c.ir.HxcIRTraversal.walkFunction;

/**
	Recover the meaning of a saved Boolean before a nullable class field read.
	For example, if `visible = item != null && item.enabled()`, every path
	that sets `visible` to true had a present item. Replacing the item invalidates
	that implication. A call can change the item itself, but can only replace the
	local pointer if the program exposes its storage.

	The analysis follows forward jump and branch edges. All incoming edges must
	agree before a fact survives a join. Each Boolean carries at most one implied
	pointer, which bounds the size of the proof even for nested conditions.
**/
class HxcIRSavedPredicateFlow {
	final fields:Map<String, String> = [];

	/** Compute facts from ordinary instructions; neither consumer supplies proof annotations. */
	public function new(fn:HxcIRFunction, graph:HxcIRControlFlowFacts) {
		if (fn.exceptionStrategy == IRESContainedRuntime || !hasPresenceTest(fn))
			return;
		final slots = new PredicateSlots(fn);
		walkFunction(fn, new HxcIRTraversalSite(null, fn.id, null, fn.source), slots);
		if (!slots.valid)
			return;
		final exits:Map<String, PredicateExit> = [];
		for (id in graph.orderedBlockIds()) {
			final block = slots.blocks.get(id);
			if (block == null || block.terminator == null || slots.failureBlocks.exists(id))
				continue;
			var state:Null<PresenceFacts> = null;
			if (id != fn.entryBlockId)
				for (prior in graph.predecessorsOf(id)) {
					final exit = exits.get(prior);
					// A missing exit includes backedges, cleanup and failure transfers.
					if (exit == null) {
						state = new PresenceFacts();
						break;
					}
					final incoming = exit.base.copy();
					if (exit.trueTarget == id && exit.implied != null)
						incoming.present = exit.implied;
					if (state == null)
						state = incoming;
					else
						state.intersect(incoming);
				}
			final facts = state == null ? new PresenceFacts() : state;
			final transfer = new PredicateBlock(slots, facts);
			for (instruction in block.instructions) {
				final receiver = transfer.fieldReceiver(instruction);
				if (receiver != null)
					fields.set(instruction.id, receiver);
				transfer.observe(instruction);
			}
			switch block.terminator.kind {
				case IRTJump(edge) if (edge.cleanup.length == 0):
					exits.set(id, {base: facts, trueTarget: null, implied: null});
				case IRTBranch(condition, yes, no) if (yes.cleanup.length == 0 && no.cleanup.length == 0):
					exits.set(id, {
						base: facts,
						trueTarget: yes.targetBlockId == no.targetBlockId ? null : yes.targetBlockId,
						implied: transfer.pointerWhenTrue(condition)
					});
				case _:
			}
		}
	}

	/** Only an actual field-use instruction can consume this independently reconstructed fact. */
	public function forUse(instruction:HxcIRInstruction, durable:Map<String, Bool>):Map<String, Bool> {
		final receiver = fields.get(instruction.id);
		if (receiver == null || durable.exists(receiver))
			return durable;
		final result = durable.copy();
		result.set(receiver, true);
		return result;
	}

	/** A cold function or an unsupported control-flow shape requires no extra operand scan. */
	public function hasProvenFields():Bool
		return fields.iterator().hasNext();

	/** The optimizer may discard only a guard whose next field read uses this exact value. */
	public function fieldValue(instruction:HxcIRInstruction):Null<String>
		return fields.get(instruction.id);

	static function hasPresenceTest(fn:HxcIRFunction):Bool {
		for (block in fn.blocks)
			for (instruction in block.instructions)
				switch instruction.kind {
					case IRIOBinary("haxe.class-reference.not-equal", _, _, IRIStatic):
						return true;
					case _:
				}
		return false;
	}
}

/** Edge-local evidence. A true branch may additionally establish one pointer's presence. */
private typedef PredicateExit = {
	final base:PresenceFacts;
	final trueTarget:Null<String>;
	final implied:Null<String>;
}

/** A false literal cannot reach a true branch. Other known decisions require one present pointer. */
private enum SavedDecision {
	AlwaysFalse;
	RequiresPresence(pointer:String);
}

/** Only facts shared by all incoming paths can protect a field read after the paths meet. */
private class PresenceFacts {
	public var present:Null<String>;
	public var saved:Map<String, SavedDecision> = [];

	public function new() {}

	public function copy():PresenceFacts {
		final result = new PresenceFacts();
		result.present = present;
		result.saved = saved.copy();
		return result;
	}

	/** A differing incoming decision cannot authorize a field read after a join. */
	public function intersect(other:PresenceFacts):Void {
		if (present != other.present)
			present = null;
		for (id in [for (id in saved.keys()) id]) {
			// If one arm saves false, a later true value came from the other arm.
			// An unknown arm can save true without establishing pointer presence.
			final common = switch [saved.get(id), other.saved.get(id)] {
				case [AlwaysFalse, decision] | [decision, AlwaysFalse]: decision;
				case [RequiresPresence(left), RequiresPresence(right)] if (left == right): RequiresPresence(left);
				case _: null;
			};
			if (common == null)
				saved.remove(id);
			else
				saved.set(id, common);
		}
	}
}

/** Track immutable values only while interpreting their defining block. */
private class PredicateBlock {
	final slots:PredicateSlots;
	final facts:PresenceFacts;
	final loads:Map<String, String> = [];
	final nulls:Map<String, String> = [];
	final implications:Map<String, SavedDecision> = [];

	public function new(slots:PredicateSlots, facts:PresenceFacts) {
		this.slots = slots;
		this.facts = facts;
	}

	/** Only a decision tied to a specific slot can add a presence fact on a true edge. */
	public function pointerWhenTrue(value:String):Null<String>
		return switch implications.get(value) {
			case RequiresPresence(pointer): pointer;
			case AlwaysFalse | null: null;
		};

	/** The field must read through an immutable load of the currently proven pointer. */
	public function fieldReceiver(instruction:HxcIRInstruction):Null<String>
		return switch instruction.kind {
			case IRIOLoad(IRPField(IRPDereference(value), _)) if (facts.present != null && loads.get(value) == facts.present): value;
			case _: null;
		};

	/** Constant, comparison and copy operations retain their source-language meanings. */
	public function observe(instruction:HxcIRInstruction):Void {
		final result = instruction.result;
		switch instruction.kind {
			case IRIOConstant(IRCNull) if (result != null):
				switch result.type {
					case IRTPointer(IRTInstance(instance), true): nulls.set(result.id, instance);
					case _:
				}
			case IRIOConstant(IRCBool(false)) if (result != null && result.type == IRTBool):
				implications.set(result.id, AlwaysFalse);
			case IRIOLoad(IRPLocal(id)) if (result != null):
				switch result.type {
					case IRTPointer(IRTInstance(instance), true) if (slots.pointers.get(id) == instance):
						loads.set(result.id, id);
					case IRTBool if (slots.booleans.exists(id)):
						final implied = facts.saved.get(id);
						if (implied != null) implications.set(result.id, implied);
					case _:
				}
			case IRIOBinary("haxe.class-reference.not-equal", left, right, IRIStatic) if (result != null && result.type == IRTBool):
				final pointer = nulls.exists(right) ? loads.get(left) : loads.get(right);
				final instance = nulls.exists(right) ? nulls.get(right) : nulls.get(left);
				if (instance != null && pointer != null && slots.pointers.get(pointer) == instance)
					implications.set(result.id, RequiresPresence(pointer));
			case IRIODeclareUninitialized(IRPLocal(id)) | IRIODefaultInitialize(IRPLocal(id), _, _):
				clearSlot(id);
			case IRIOInitialize(IRPLocal(id), value, _, _) | IRIOStore(IRPLocal(id), value):
				if (slots.pointers.exists(id))
					clearSlot(id);
				else if (slots.booleans.exists(id)) {
					final decision = implications.get(value);
					if (decision != null)
						facts.saved.set(id, decision);
					else if (facts.present != null)
						facts.saved.set(id, RequiresPresence(facts.present));
					else
						facts.saved.remove(id);
				}
			case _:
		}
	}

	/** A declaration or write starts a new slot lifetime for the purposes of this proof. */
	function clearSlot(id:String):Void {
		if (slots.booleans.exists(id))
			facts.saved.remove(id);
		if (!slots.pointers.exists(id))
			return;
		if (facts.present == id)
			facts.present = null;
		for (value in [for (value in loads.keys()) if (loads.get(value) == id) value])
			loads.remove(value);
		discardDecisions(implications, id);
		discardDecisions(facts.saved, id);
	}

	static function discardDecisions(decisions:Map<String, SavedDecision>, pointer:String):Void {
		for (id in [for (id in decisions.keys()) id])
			switch decisions.get(id) {
				case RequiresPresence(source) if (source == pointer):
					decisions.remove(id);
				case _:
			}
	}
}

/** Exclude exposed storage and ambiguous identities before inferring any local facts. */
private class PredicateSlots extends HxcIRTraversalVisitor {
	public final pointers:Map<String, String> = [];
	public final booleans:Map<String, Bool> = [];
	public final blocks:Map<String, HxcIRBlock> = [];
	public final failureBlocks:Map<String, Bool> = [];
	public var valid(default, null):Bool = true;

	final values:Map<String, Bool> = [];
	final instructions:Map<String, Bool> = [];
	var direct:Null<String>;

	public function new(fn:HxcIRFunction) {
		super();
		final locals:Map<String, Bool> = [];
		for (local in fn.locals) {
			record(locals, local.id);
			if (local.storage == IRLSAutomatic)
				switch local.type {
					case IRTPointer(IRTInstance(instance), true):
						pointers.set(local.id, instance);
					case IRTBool:
						booleans.set(local.id, true);
					case _:
				}
		}
		for (parameter in fn.parameters)
			record(values, parameter.id);
	}

	function record(index:Map<String, Bool>, id:String):Void {
		if (index.exists(id))
			valid = false;
		index.set(id, true);
	}

	override public function onBlock(block:HxcIRBlock, site:HxcIRTraversalSite):Void {
		if (blocks.exists(block.id))
			valid = false;
		blocks.set(block.id, block);
		for (parameter in block.parameters)
			record(values, parameter.id);
		direct = null;
	}

	override public function onInstruction(instruction:HxcIRInstruction, site:HxcIRTraversalSite):Void {
		record(instructions, instruction.id);
		if (instruction.result != null)
			record(values, instruction.result.id);
		direct = switch instruction.kind {
			case IRIOLoad(IRPLocal(id)) | IRIOInitialize(IRPLocal(id), _, _, _) | IRIOStore(IRPLocal(id), _) | IRIODeclareUninitialized(IRPLocal(id)) |
				IRIODefaultInitialize(IRPLocal(id), _, _): id;
			case IRIOException(IREFrameSetJmp(_)):
				valid = false;
				null;
			case _: null;
		};
	}

	override public function onTerminator(terminator:HxcIRTerminator, site:HxcIRTraversalSite):Void
		direct = null;

	override public function onFailureEdge(edge:HxcIRFailureEdge, site:HxcIRTraversalSite):Void {
		if (site.blockId != null)
			failureBlocks.set(site.blockId, true);
	}

	override public function onCleanupAction(action:HxcIRCleanupAction, site:HxcIRTraversalSite):Void {
		direct = switch action.kind {
			case IRCADestroy(IRPLocal(id), _, _): id;
			case IRCAFinally(_):
				valid = false;
				null;
			case _: null;
		};
	}

	override public function onPlace(place:HxcIRPlace, site:HxcIRTraversalSite):Void {
		switch place {
			case IRPLocal(id) if (id != direct):
				pointers.remove(id);
				booleans.remove(id);
			case _:
		}
	}
}

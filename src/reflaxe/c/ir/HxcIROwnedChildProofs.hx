package reflaxe.c.ir;

import reflaxe.c.ir.HxcIR;

/**
	Remember a checked owner's embedded child through one straight-line borrow.

	The producer, conversion, and local operations remain in HxcIR for independent
	validation. This tracker supplies candidate facts, never authority from a type
	annotation alone. Only local contents with the exact same origin may enter from predecessors.
	Calls, alias-capable writes, address-taking,
	lifetime changes, and unknown effects forget local contents and parent witnesses.
	Already loaded immutable values stay nonnull; their lifetime is still checked
	by the existing independent borrow/lifetime rules.
**/
class HxcIROwnedChildProofs {
	final locals:Map<String, HxcIRLocal> = [];
	final borrowedLocals:Map<String, Bool> = [];
	var checked:Map<String, ParentCheck> = [];
	var values:Map<String, OwnedChildFact> = [];
	var localValues:Map<String, OwnedChildFact> = [];

	/** Start a block with independently reconstructed local contents, never immutable value facts. */
	public function new(fn:HxcIRFunction, ?incoming:Map<String, OwnedChildFact>) {
		if (incoming != null)
			localValues = incoming.copy();
		for (local in fn.locals)
			locals.set(local.id, local);
		for (id in fn.borrowedClassLocalIds)
			borrowedLocals.set(id, true);
	}

	/** Snapshot local contents only; loaded values and parent checks never cross an edge. */
	public function localSnapshot():Map<String, OwnedChildFact>
		return localValues.copy();

	/** Require the same retained producer and witness, not merely equal pointer types. */
	public static function sameOrigin(left:OwnedChildFact, right:OwnedChildFact):Bool
		return left.borrow == right.borrow && left.result == right.result && left.witness == right.witness && left.instance == right.instance
			&& left.nullable == right.nullable && samePolicy(left.policy, right.policy);

	/** Instruction ID of the retained owner check from which this value came. */
	public function witness(valueId:String):Null<String> {
		final fact = values.get(valueId);
		return fact == null ? null : fact.witness;
	}

	/** Exact policy of the retained owner check. */
	public function policy(valueId:String):Null<HxcIRNullCheckPolicy> {
		final fact = values.get(valueId);
		return fact == null || !fact.nullable ? null : fact.policy;
	}

	/**
		Give use validation an independent union without mutating durable proofs.
		Instructions that update durable proofs keep the original map. All other
		uses receive a copy, so place consumers share the same complete validation
		boundary without leaking derived facts into legacy local transport.
	**/
	public function forUse(instruction:HxcIRInstruction, durable:Map<String, Bool>):Map<String, Bool> {
		final consumes = switch instruction.kind {
			case IRIONullCheck(_, _) | IRIOLoad(IRPLocal(_)) | IRIOStore(IRPLocal(_), _) | IRIOInitialize(IRPLocal(_), _, _, _): false;
			case _: true;
		};
		if (!consumes || !values.iterator().hasNext())
			return durable;
		final result = durable.copy();
		for (id in values.keys())
			result.set(id, true);
		return result;
	}

	/** Process the retained instruction after its ordinary validation/use. */
	public function observe(instruction:HxcIRInstruction, available:Map<String, HxcIRTypeRef>):Void {
		final result = instruction.result;
		switch instruction.kind {
			case IRIONullCheck(valueId, policy):
				checked.set(valueId, {policy: policy, id: instruction.id});
			case IRIOBorrowClassField(IRPField(IRPDereference(parentId), _)):
				final parent = checked.get(parentId);
				final instance = result == null ? null : pointerInstance(result.type, false);
				if (result != null && parent != null && instance != null && pointerInstance(available.get(parentId), true) != null)
					values.set(result.id, {
						instance: instance,
						nullable: false,
						policy: parent.policy,
						witness: parent.id,
						borrow: instruction.id,
						result: result.id
					});
			case IRIOConvert(valueId, IRCNullableInject, target, IRIStatic, null):
				final fact = values.get(valueId);
				if (result != null
					&& fact != null
					&& !fact.nullable
					&& pointerInstance(available.get(valueId), false) == fact.instance
					&& pointerInstance(target, true) == fact.instance
					&& pointerInstance(result.type, true) == fact.instance)
					values.set(result.id, {
						instance: fact.instance,
						nullable: true,
						policy: fact.policy,
						witness: fact.witness,
						borrow: fact.borrow,
						result: fact.result
					});
			case IRIOInitialize(IRPLocal(localId), valueId, IRISUninitialized, IRISInitialized):
				final fact = values.get(valueId);
				final local = locals.get(localId);
				localValues.remove(localId);
				if (fact != null
					&& fact.nullable
					&& local != null
					&& borrowedLocals.exists(localId)
					&& local.storage == IRLSAutomatic
					&& local.initialState == IRISUninitialized
					&& pointerInstance(local.type, true) == fact.instance
					&& pointerInstance(available.get(valueId), true) == fact.instance)
					localValues.set(localId, fact);
			case IRIOInitialize(IRPLocal(localId), _, _, _) | IRIOStore(IRPLocal(localId), _) | IRIODefaultInitialize(IRPLocal(localId), _, _):
				localValues.remove(localId);
			case IRIOLoad(IRPLocal(localId)):
				final fact = localValues.get(localId);
				if (result != null && fact != null && pointerInstance(result.type, true) == fact.instance)
					values.set(result.id, fact);
			case IRIOLoad(_) | IRIOConstant(_) | IRIOSequence(_):
			case IRIOBinary(_, _, _, _):
				// Only the validator's exact scalar comparison contract preserves
				// existing facts. Helper-backed and malformed operations stay barriers.
				if (!HxcIRValidator.isStaticI32Less(instruction, available))
					clear();
			case _:
				clear();
		}
	}

	/** Keep policy differences, including malformed policy text, observable. */
	public static function samePolicy(left:HxcIRNullCheckPolicy, right:HxcIRNullCheckPolicy):Bool
		return switch [left, right] {
			case [IRNCPCheckedAbort(lp, lb), IRNCPCheckedAbort(rp, rb)]: lp == rp && lb == rb;
		};

	static function pointerInstance(type:Null<HxcIRTypeRef>, nullable:Bool):Null<String>
		return switch type {
			case IRTPointer(IRTInstance(id), actual) if (actual == nullable): id;
			case _: null;
		};

	function clear():Void {
		checked = [];
		localValues = [];
	}
}

/** One checked embedded-child identity; it never asserts source-level nonnullness. */
typedef OwnedChildFact = {
	final borrow:String;
	final result:String;
	final instance:String;
	final nullable:Bool;
	final policy:HxcIRNullCheckPolicy;
	final witness:String;
}

/** The actual instruction remains available for policy and metadata validation. */
private typedef ParentCheck = {
	final policy:HxcIRNullCheckPolicy;
	final id:String;
}

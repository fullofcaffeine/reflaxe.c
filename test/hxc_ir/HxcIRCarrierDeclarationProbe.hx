import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRValidator;
import reflaxe.c.ir.HxcSourceSpan;

/**
	Protect declaration identity while validating managed ownership transfers.

	Each carrier temporarily owns a retained String before moving it into a local.
	These small raw-IR fixtures isolate declaration errors from Haxe lowering and
	check that function-local identities cannot leak between validation requests.
**/
private enum DeclarationCase {
	Valid;
	Missing;
	Duplicate;
	DuplicateInstruction;
	DuplicateBlock;
}

/** Independent declaration contracts run through the public validator boundary. */
class HxcIRCarrierDeclarationProbe {
	static final source = new HxcSourceSpan("test/hxc_ir/fixtures/CarrierDeclarations.hx", 1, 1, 1, 2);
	static final missingAcquire = "Invalid HxcIR: managed carrier acquisition requires exactly one matching declaration";
	static final missingMove = "Invalid HxcIR: managed carrier move requires exactly one matching declaration";
	static final duplicateDeclaration = "Invalid HxcIR: managed carrier storage must have exactly one declaration";

	/** Check complete diagnostic sets, then reuse identities across functions and requests. */
	static function main():Void {
		final validator = new HxcIRValidator();
		check(validator, "valid", [fn("one", 1, Valid)], []);
		check(validator, "missing", [fn("one", 1, Missing)], [missingAcquire, missingMove]);
		check(validator, "duplicate", [fn("one", 1, Duplicate)], [duplicateDeclaration, duplicateDeclaration, missingAcquire, missingMove]);
		check(validator, "duplicate-instruction", [fn("one", 1, DuplicateInstruction)], [
			duplicateDeclaration,
			duplicateDeclaration,
			missingAcquire,
			missingMove,
			"Invalid HxcIR: duplicate instruction ID `first.declare.0`"
		]);
		// Each duplicate block has one declaration. Counting both raw blocks would
		// add false declaration errors beyond the required duplicate-block error.
		check(validator, "duplicate-block", [fn("one", 1, DuplicateBlock)], ["Invalid HxcIR: duplicate basic block ID `entry`"]);
		check(validator, "function-local-identities", [fn("first", 1, Valid), fn("second", 3, Valid)], []);
		check(validator, "missing-in-next-function", [fn("first", 1, Valid), fn("second", 1, Missing)], [missingAcquire, missingMove]);
		check(validator, "duplicate-function-identities", [fn("same", 1, Valid), fn("same", 1, Missing)],
			["Invalid HxcIR: duplicate function ID `same`", missingAcquire, missingMove]);
		// The same validator and function/local IDs now describe different graphs.
		check(validator, "reused-request-more-locals", [fn("one", 3, Valid)], []);
		check(validator, "reused-request-missing", [fn("one", 1, Missing)], [missingAcquire, missingMove]);
		check(validator, "reused-request-valid-again", [fn("one", 1, Valid)], []);
		Sys.println("HXC_IR_CARRIER_DECLARATIONS_OK");
	}

	/** Match every message and its multiplicity; unrelated diagnostics must also fail. */
	static function check(validator:HxcIRValidator, name:String, functions:Array<HxcIRFunction>, expected:Array<String>):Void {
		final diagnostics = validator.validate(program(functions), "portable");
		final actual = diagnostics.map(diagnostic -> diagnostic.message);
		actual.sort((left, right) -> left < right ? -1 : left > right ? 1 : 0);
		final orderedExpected = expected.copy();
		orderedExpected.sort((left, right) -> left < right ? -1 : left > right ? 1 : 0);
		if (actual.join("\n") != orderedExpected.join("\n"))
			throw new haxe.Exception(name + ": unexpected diagnostics\n" + diagnostics.map(diagnostic -> diagnostic.render()).join("\n"));
	}

	/** Build one complete retain/move/release sequence for each independent local. */
	static function instructions(prefix:String, count:Int, mutation:DeclarationCase):Array<HxcIRInstruction> {
		final result:Array<HxcIRInstruction> = [];
		for (index in 0...count) {
			final key = Std.string(index);
			final carrier = IRPLocal("carrier." + key);
			final owner = IRPLocal("owner." + key);
			final declareId = prefix + ".declare." + key;
			if (mutation != Missing)
				result.push(instruction(declareId, IRIODeclareManagedCarrier(carrier, IRIRuntime("string"))));
			if (mutation == Duplicate || mutation == DuplicateInstruction)
				result.push(instruction(mutation == DuplicateInstruction ? declareId : prefix + ".duplicate." + key,
					IRIODeclareManagedCarrier(carrier, IRIRuntime("string"))));
			result.push(instruction(prefix + ".acquire." + key, IRIOAcquireManagedCarrier(carrier, "borrowed", IRMCARetainBorrowed(IRIRuntime("string")))));
			final moved = prefix + ".moved." + key;
			result.push(instruction(prefix + ".move." + key, IRIOMoveManagedCarrier(carrier), {id: moved, type: IRTManagedString}));
			result.push(instruction(prefix + ".initialize." + key, IRIOInitialize(owner, moved, IRISUninitialized, IRISInitialized)));
			result.push(instruction(prefix + ".release." + key, IRIORelease(owner, IRIRuntime("string"))));
		}
		return result;
	}

	/** Give each function its own locals, even when another function uses the same IDs. */
	static function fn(id:String, count:Int, mutation:DeclarationCase):HxcIRFunction {
		final locals:Array<HxcIRLocal> = [];
		for (index in 0...count)
			for (prefix in ["carrier.", "owner."])
				locals.push({
					id: prefix + index,
					type: IRTManagedString,
					storage: IRLSAutomatic,
					initialState: IRISUninitialized,
					source: source
				});
		final blocks = [block(instructions("first", count, mutation))];
		if (mutation == DuplicateBlock)
			blocks.push(block(instructions("second", count, Valid)));
		return {
			id: id,
			displayName: id,
			parameters: [{id: "borrowed", type: IRTManagedString, source: source}],
			borrowedClassParameterIds: [],
			borrowedClassLocalIds: [],
			managedRoots: [],
			locals: locals,
			returnType: IRTVoid,
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: blocks,
			cleanupRegions: [],
			source: source
		};
	}

	static function instruction(id:String, kind:HxcIRInstructionKind, ?result:HxcIRResult):HxcIRInstruction
		return {
			id: id,
			kind: kind,
			result: result,
			source: source
		};

	static function block(instructions:Array<HxcIRInstruction>):HxcIRBlock
		return {
			id: "entry",
			parameters: [],
			instructions: instructions,
			terminator: {kind: IRTReturn(null, []), source: source},
			source: source
		};

	static function program(functions:Array<HxcIRFunction>):HxcIRProgram
		return {
			schemaVersion: HxcIRValidator.SCHEMA_VERSION,
			dynamicPlan: {
				types: [],
				members: [],
				callShapes: [],
				operations: []
			},
			dispatch: {layouts: [], slots: [], tables: []},
			modules: [
				{
					id: "declarations",
					types: [],
					typeInstances: [],
					globals: [],
					functions: functions,
					source: source
				}
			]
		};
}

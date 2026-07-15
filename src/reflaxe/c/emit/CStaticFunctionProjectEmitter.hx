package reflaxe.c.emit;

#if (macro || reflaxe_runtime)
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.lowering.CBodyEmitter;
import reflaxe.c.lowering.CBodyLowering.CBodyLoweringResult;
import reflaxe.c.lowering.CBodyLowering.CLoweredBodyFunction;

/** Structural function prototype/definition plan for the first executable slice. */
class CStaticFunctionDeclarationPlan {
	public final headerPath:String;
	public final sourcePath:String;
	public final header:CHeaderUnit;
	public final source:CTranslationUnit;

	public function new(headerPath:String, sourcePath:String, header:CHeaderUnit, source:CTranslationUnit) {
		this.headerPath = headerPath;
		this.sourcePath = sourcePath;
		this.header = header;
		this.source = source;
	}
}

/** Emits a runtime-free private prototype header and one strict-C11 source unit. */
class CStaticFunctionProjectEmitter {
	public static inline final HEADER_PATH = "include/hxc/program.h";
	public static inline final HEADER_INCLUDE = "hxc/program.h";
	public static inline final SOURCE_PATH = "src/program.c";

	public function new() {}

	public function plan(lowered:CBodyLoweringResult, entryFunctionId:String, entryName:CIdentifier, headerGuard:CIdentifier):CStaticFunctionDeclarationPlan {
		final entry = findFunction(lowered.functions, entryFunctionId);
		if (entry.ir.parameters.length != 0 || entry.ir.returnType != IRTVoid) {
			throw new ProjectEmissionError('Haxe executable entry `${entry.ir.id}` must have signature `static function main():Void`');
		}

		final bodyEmitter = new CBodyEmitter();
		final nonReturningFunctionIds = nonReturningCallCycles(lowered.functions);
		final functionNames:Map<String, CIdentifier> = [];
		for (fn in lowered.functions) {
			functionNames.set(fn.ir.id, fn.cName);
		}
		final headerUnit = new CTranslationUnit();
		final headers:Array<String> = [];
		for (fn in lowered.functions) {
			for (header in fn.requiredHeaders) {
				if (headers.indexOf(header) == -1) {
					headers.push(header);
				}
			}
		}
		headers.sort(compareStrings);
		for (header in headers) {
			headerUnit.includes.push({path: header, kind: System});
		}
		for (fn in lowered.functions) {
			final functionSpecifiers = nonReturningFunctionIds.exists(fn.ir.id) ? [FNoReturn] : [];
			headerUnit.declarations.push(DPrototype([], functionSpecifiers, bodyEmitter.cType(fn.ir.returnType),
				DFunction(DName(fn.cName), FPPrototype(bodyEmitter.parameters(fn.ir, fn.parameterNames), false)), []));
		}

		final sourceUnit = new CTranslationUnit();
		sourceUnit.includes.push({path: HEADER_INCLUDE, kind: Local});
		for (fn in lowered.functions) {
			final functionSpecifiers = nonReturningFunctionIds.exists(fn.ir.id) ? [FNoReturn] : [];
			sourceUnit.declarations.push(DFunction({
				storage: [],
				functionSpecifiers: functionSpecifiers,
				returnType: bodyEmitter.cType(fn.ir.returnType),
				declarator: DFunction(DName(fn.cName), FPPrototype(bodyEmitter.parameters(fn.ir, fn.parameterNames), false)),
				body: bodyEmitter.emitBody(fn.ir, fn.parameterNames, fn.localNames, fn.temporaryNames, functionNames, false, nonReturningFunctionIds),
				attributes: []
			}));
		}
		sourceUnit.declarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNativeInt(IRInt, true)),
			declarator: DFunction(DName(entryName), FPPrototype([], false)),
			body: SBlock([
				SExpr(ECall(EIdentifier(entry.cName), [])),
				SReturn(EInt(CIntegerLiteral.decimal("0")))
			]),
			attributes: []
		}));
		return new CStaticFunctionDeclarationPlan(HEADER_PATH, SOURCE_PATH, new CHeaderUnit(headerGuard, headerUnit), sourceUnit);
	}

	public function emit(lowered:CBodyLoweringResult, entryFunctionId:String, entryName:CIdentifier, headerGuard:CIdentifier):Array<GeneratedFile> {
		final declarationPlan = plan(lowered, entryFunctionId, entryName, headerGuard);
		final printer = new CASTPrinter();
		return [
			new GeneratedFile(declarationPlan.headerPath, printer.printHeader(declarationPlan.header), GeneratedFileKind.PrivateHeader),
			new GeneratedFile(declarationPlan.sourcePath, printer.printTranslationUnit(declarationPlan.source), GeneratedFileKind.Source)
		];
	}

	static function findFunction(functions:Array<CLoweredBodyFunction>, id:String):CLoweredBodyFunction {
		for (fn in functions) {
			if (fn.ir.id == id) {
				return fn;
			}
		}
		throw new ProjectEmissionError('static function project cannot resolve executable entry `$id`');
	}

	/**
		Every admitted instruction is unconditional. A closed direct-call cycle
		therefore cannot reach its HxcIR return and may use C11 `_Noreturn`.
	**/
	static function nonReturningCallCycles(functions:Array<CLoweredBodyFunction>):Map<String, Bool> {
		final known:Map<String, Bool> = [];
		for (fn in functions) {
			known.set(fn.ir.id, true);
		}

		final targetsByFunction:Map<String, Array<String>> = [];
		for (fn in functions) {
			final targets:Array<String> = [];
			for (block in fn.ir.blocks) {
				for (instruction in block.instructions) {
					switch instruction.kind {
						case IRIOCall({dispatch: IRCDDirect(targetId)}):
							if (known.exists(targetId) && targets.indexOf(targetId) == -1) {
								targets.push(targetId);
							}
						case _:
					}
				}
			}
			targets.sort(compareStrings);
			targetsByFunction.set(fn.ir.id, targets);
		}

		final result:Map<String, Bool> = [];
		for (fn in functions) {
			final directTargets = targetsByFunction.get(fn.ir.id);
			if (directTargets == null) {
				throw new ProjectEmissionError('static function call graph lost `${fn.ir.id}`');
			}
			for (targetId in directTargets) {
				if (canReach(targetId, fn.ir.id, targetsByFunction)) {
					result.set(fn.ir.id, true);
					break;
				}
			}
		}
		return result;
	}

	static function canReach(initialId:String, targetId:String, targetsByFunction:Map<String, Array<String>>):Bool {
		final visited:Map<String, Bool> = [];
		final pending = [initialId];
		var index = 0;
		while (index < pending.length) {
			final currentId = pending[index++];
			if (currentId == targetId) {
				return true;
			}
			if (visited.exists(currentId)) {
				continue;
			}
			visited.set(currentId, true);
			final directTargets = targetsByFunction.get(currentId);
			if (directTargets != null) {
				for (nextId in directTargets) {
					pending.push(nextId);
				}
			}
		}
		return false;
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);
}
#else
class CStaticFunctionProjectEmitter {
	public function new() {}
}
#end

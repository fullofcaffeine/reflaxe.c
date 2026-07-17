package reflaxe.c.emit;

#if (macro || reflaxe_runtime)
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.lowering.CBodyEmitter;
import reflaxe.c.lowering.CBodyLowering.CBodyLoweringResult;
import reflaxe.c.lowering.CBodyLowering.CLoweredBodyFunction;
import reflaxe.c.lowering.CPrimitiveHelperEmitter;
import reflaxe.c.runtime.RuntimeAbiContract;

/** Structural function prototype/definition plan for the first executable slice. */
typedef CStaticFunctionSourcePlan = {
	final path:String;
	final unit:CTranslationUnit;
}

class CStaticFunctionDeclarationPlan {
	public final headerPath:String;
	public final header:CHeaderUnit;
	public final sources:Array<CStaticFunctionSourcePlan>;

	public function new(headerPath:String, header:CHeaderUnit, sources:Array<CStaticFunctionSourcePlan>) {
		this.headerPath = headerPath;
		this.header = header;
		this.sources = sources.copy();
	}
}

/** Emits a runtime-free private prototype header and strict-C11 source units. */
class CStaticFunctionProjectEmitter {
	public static inline final HEADER_PATH = "include/hxc/program.h";
	public static inline final HEADER_INCLUDE = "hxc/program.h";
	public static inline final SOURCE_PATH = "src/program.c";
	public static inline final NON_RETURNING_SOURCE_PREFIX = "src/nonreturn_";

	public function new() {}

	public function plan(lowered:CBodyLoweringResult, entryFunctionId:String, entryName:CIdentifier, headerGuard:CIdentifier,
			?initializerFunctionIds:Array<String>, ?initializationName:CIdentifier, ?runtimeAbiMajor:Int):CStaticFunctionDeclarationPlan {
		final entry = findFunction(lowered.functions, entryFunctionId);
		if (entry.ir.parameters.length != 0 || entry.ir.returnType != IRTVoid) {
			throw new ProjectEmissionError('Haxe executable entry `${entry.ir.id}` must have signature `static function main():Void`');
		}
		final initializerIds:Map<String, Bool> = [];
		final orderedInitializers:Array<CLoweredBodyFunction> = [];
		if (initializerFunctionIds != null && initializerFunctionIds.length > 0) {
			if (initializationName == null) {
				throw new ProjectEmissionError("static initialization function IDs require a compiler-owned initialization name");
			}
			for (initializerFunctionId in initializerFunctionIds) {
				if (initializerIds.exists(initializerFunctionId)) {
					throw new ProjectEmissionError('static initialization order repeats `$initializerFunctionId`');
				}
				final initializer = findFunction(lowered.functions, initializerFunctionId);
				if (initializer.ir.parameters.length != 0 || initializer.ir.returnType != IRTVoid) {
					throw new ProjectEmissionError('static initializer `${initializer.ir.id}` must have signature `():Void`');
				}
				initializerIds.set(initializerFunctionId, true);
				orderedInitializers.push(initializer);
			}
		} else if (initializationName != null) {
			throw new ProjectEmissionError("a compiler-owned initialization name requires an explicit initialization order");
		}

		final bodyEmitter = new CBodyEmitter(lowered.aggregates);
		final helperEmitter = new CPrimitiveHelperEmitter(lowered.helpers);
		final nonReturningFunctionIds = nonReturningCallCycles(lowered.functions);
		final functionNames:Map<String, CIdentifier> = [];
		for (fn in lowered.functions) {
			functionNames.set(fn.ir.id, fn.cName);
		}
		final globalNames:Map<String, CIdentifier> = [];
		for (global in lowered.globals) {
			globalNames.set(global.ir.id, global.cName);
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
		for (global in lowered.globals) {
			for (header in bodyEmitter.requiredGlobalHeaders(global.ir)) {
				if (headers.indexOf(header) == -1) {
					headers.push(header);
				}
			}
		}
		for (header in helperEmitter.requiredHeaders(lowered.helpers)) {
			if (headers.indexOf(header) == -1) {
				headers.push(header);
			}
		}
		if (lowered.aggregates.length > 0 && headers.indexOf("stddef.h") == -1) {
			headers.push("stddef.h");
		}
		headers.sort(compareStrings);
		for (header in headers) {
			headerUnit.includes.push({path: header, kind: System});
		}
		final hasRuntimeHeader = headers.filter(header -> StringTools.startsWith(header, "hxrt/")).length > 0;
		if (hasRuntimeHeader != (runtimeAbiMajor != null)) {
			throw new ProjectEmissionError("runtime headers and the generated runtime ABI check must be selected together");
		}
		if (runtimeAbiMajor != null) {
			if (runtimeAbiMajor != RuntimeAbiContract.MAJOR) {
				throw new ProjectEmissionError('generated runtime ABI major `$runtimeAbiMajor` differs from the compiler contract `${RuntimeAbiContract.MAJOR}`');
			}
			headerUnit.declarations.push(DStaticAssert(EBinary(Equal, EIdentifier(new CIdentifier(RuntimeAbiContract.MAJOR_MACRO)),
				EInt(CIntegerLiteral.decimal(Std.string(runtimeAbiMajor), ISUnsigned))),
				'incompatible hxrt ABI major: generated code requires $runtimeAbiMajor'));
		}
		for (definition in helperEmitter.definitions(lowered.helpers)) {
			headerUnit.declarations.push(definition);
		}
		for (definition in bodyEmitter.aggregateDefinitions()) {
			headerUnit.declarations.push(definition);
		}
		for (global in lowered.globals) {
			headerUnit.declarations.push(DVariable({
				storage: [SExtern],
				alignments: [],
				type: bodyEmitter.cType(global.ir.type),
				declarator: DName(global.cName),
				initializer: null,
				attributes: []
			}));
		}
		for (fn in lowered.functions) {
			if (!initializerIds.exists(fn.ir.id)) {
				final functionSpecifiers = nonReturningFunctionIds.exists(fn.ir.id) ? [FNoReturn] : [];
				headerUnit.declarations.push(DPrototype([], functionSpecifiers, bodyEmitter.cType(fn.ir.returnType),
					DFunction(DName(fn.cName), FPPrototype(bodyEmitter.parameters(fn.ir, fn.parameterNames), false)), []));
			}
		}

		final programUnit = sourceUnit();
		for (assertion in bodyEmitter.aggregateLayoutAssertions()) {
			programUnit.declarations.push(assertion);
		}
		final helperNames:Map<String, CIdentifier> = [];
		for (helper in lowered.helpers) {
			helperNames.set(helper.helperId, helper.cName);
		}
		for (global in lowered.globals) {
			programUnit.declarations.push(DVariable({
				storage: [],
				alignments: [],
				type: bodyEmitter.cType(global.ir.type),
				declarator: DName(global.cName),
				initializer: bodyEmitter.globalInitializer(global.ir),
				attributes: []
			}));
		}
		final sources:Array<CStaticFunctionSourcePlan> = [];
		var nonReturningOrdinal = 0;
		for (fn in lowered.functions) {
			final isInitializer = initializerIds.exists(fn.ir.id);
			if (isInitializer && nonReturningFunctionIds.exists(fn.ir.id)) {
				throw new ProjectEmissionError('static initializer `${fn.ir.id}` unexpectedly participates in a closed call cycle');
			}
			final functionSpecifiers = nonReturningFunctionIds.exists(fn.ir.id) ? [FNoReturn] : [];
			final definition:CDecl = DFunction({
				storage: isInitializer ? [SStatic] : [],
				functionSpecifiers: functionSpecifiers,
				returnType: bodyEmitter.cType(fn.ir.returnType),
				declarator: DFunction(DName(fn.cName), FPPrototype(bodyEmitter.parameters(fn.ir, fn.parameterNames), false)),
				body: bodyEmitter.emitBody(fn.ir, fn.parameterNames, fn.localNames, fn.temporaryNames, functionNames, globalNames, helperNames, false,
					fn.tailArgumentNames, fn.labelNames, nonReturningFunctionIds, fn.spanLengthNames, lowered.boundsAbortName),
				attributes: []
			});
			if (nonReturningFunctionIds.exists(fn.ir.id)) {
				final unit = sourceUnit();
				unit.declarations.push(definition);
				sources.push({path: nonReturningSourcePath(nonReturningOrdinal++), unit: unit});
			} else {
				programUnit.declarations.push(definition);
			}
		}
		final entryStatements:Array<CStmt> = [];
		if (orderedInitializers.length > 0) {
			if (initializationName == null) {
				throw new ProjectEmissionError("validated static initialization order lost its compiler-owned name");
			}
			final initializationStatements:Array<CStmt> = [];
			for (initializer in orderedInitializers) {
				initializationStatements.push(SExpr(ECall(EIdentifier(initializer.cName), [])));
			}
			programUnit.declarations.push(DFunction({
				storage: [SStatic],
				functionSpecifiers: [],
				returnType: new CType(TVoid),
				declarator: DFunction(DName(initializationName), FPPrototype([], false)),
				body: SBlock(initializationStatements),
				attributes: []
			}));
			entryStatements.push(SExpr(ECall(EIdentifier(initializationName), [])));
		}
		entryStatements.push(SExpr(ECall(EIdentifier(entry.cName), [])));
		entryStatements.push(SReturn(EInt(CIntegerLiteral.decimal("0"))));
		programUnit.declarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNativeInt(IRInt, true)),
			declarator: DFunction(DName(entryName), FPPrototype([], false)),
			body: SBlock(entryStatements),
			attributes: []
		}));
		sources.push({path: SOURCE_PATH, unit: programUnit});
		sources.sort((left, right) -> compareStrings(left.path, right.path));
		return new CStaticFunctionDeclarationPlan(HEADER_PATH, new CHeaderUnit(headerGuard, headerUnit), sources);
	}

	public function emit(lowered:CBodyLoweringResult, entryFunctionId:String, entryName:CIdentifier, headerGuard:CIdentifier,
			?initializerFunctionIds:Array<String>, ?initializationName:CIdentifier, ?runtimeAbiMajor:Int):Array<GeneratedFile> {
		final declarationPlan = plan(lowered, entryFunctionId, entryName, headerGuard, initializerFunctionIds, initializationName, runtimeAbiMajor);
		final printer = new CASTPrinter();
		final files = [
			new GeneratedFile(declarationPlan.headerPath, printer.printHeader(declarationPlan.header), GeneratedFileKind.PrivateHeader)
		];
		for (source in declarationPlan.sources) {
			files.push(new GeneratedFile(source.path, printer.printTranslationUnit(source.unit), GeneratedFileKind.Source));
		}
		return files;
	}

	static function sourceUnit():CTranslationUnit {
		final unit = new CTranslationUnit();
		unit.includes.push({path: HEADER_INCLUDE, kind: Local});
		return unit;
	}

	static function nonReturningSourcePath(index:Int):String {
		if (index < 0 || index > 9999) {
			throw new ProjectEmissionError('static function project cannot address non-returning source ordinal `$index`');
		}
		return NON_RETURNING_SOURCE_PREFIX + StringTools.lpad(Std.string(index), "0", 4) + ".c";
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
		Only a one-block admitted body proves every call instruction unconditional.
		Multi-block bodies are excluded conservatively. A closed direct-call cycle
		in the remaining graph cannot reach its HxcIR return and may use C11
		`_Noreturn`.
	**/
	static function nonReturningCallCycles(functions:Array<CLoweredBodyFunction>):Map<String, Bool> {
		final known:Map<String, Bool> = [];
		for (fn in functions) {
			known.set(fn.ir.id, true);
		}

		final targetsByFunction:Map<String, Array<String>> = [];
		for (fn in functions) {
			final targets:Array<String> = [];
			if (fn.ir.blocks.length == 1) {
				for (instruction in fn.ir.blocks[0].instructions) {
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

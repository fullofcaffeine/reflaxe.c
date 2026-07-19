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

/** One exact finalized function definition and the source unit that owns it. */
typedef CStaticFunctionDefinitionPlan = {
	final functionId:String;
	final sourcePath:String;
	final declaration:CDecl;
}

class CStaticFunctionDeclarationPlan {
	public final headerPath:String;
	public final header:CHeaderUnit;
	public final sources:Array<CStaticFunctionSourcePlan>;
	public final functionDefinitions:Array<CStaticFunctionDefinitionPlan>;

	public function new(headerPath:String, header:CHeaderUnit, sources:Array<CStaticFunctionSourcePlan>,
			functionDefinitions:Array<CStaticFunctionDefinitionPlan>) {
		this.headerPath = headerPath;
		this.header = header;
		this.sources = sources.copy();
		this.functionDefinitions = functionDefinitions.copy();
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

		final bodyEmitter = new CBodyEmitter(lowered.aggregates, lowered.enums, lowered.classes, lowered.dispatch, lowered.imports);
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
		final usesFloat32 = programUsesFloat32(lowered.program);
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
		if (usesFloat32) {
			for (header in ["float.h", "limits.h"]) {
				if (headers.indexOf(header) == -1) {
					headers.push(header);
				}
			}
		}
		if ((lowered.aggregates.length > 0 || lowered.enums.length > 0 || lowered.classes.length > 0)
			&& headers.indexOf("stddef.h") == -1) {
			headers.push("stddef.h");
		}
		headers.sort(compareStrings);
		for (header in headers) {
			headerUnit.includes.push({path: header, kind: System});
		}
		for (include in lowered.imports.includes) {
			var duplicate = false;
			for (existing in headerUnit.includes) {
				if (existing.path != include.path)
					continue;
				if (existing.kind != include.kind)
					throw new ProjectEmissionError('imported header `${include.path}` has conflicting local/system delimiter policy');
				duplicate = true;
			}
			if (!duplicate)
				headerUnit.includes.push(include);
		}
		headerUnit.includes.sort((left, right) -> {
			final kindOrder = (left.kind == System ? 0 : 1) - (right.kind == System ? 0 : 1);
			return kindOrder != 0 ? kindOrder : compareStrings(left.path, right.path);
		});
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
		if (usesFloat32) {
			for (assertion in float32AbiAssertions()) {
				headerUnit.declarations.push(assertion);
			}
		}
		for (definition in helperEmitter.definitions(lowered.helpers)) {
			headerUnit.declarations.push(definition);
		}
		for (definition in bodyEmitter.aggregateDefinitions()) {
			headerUnit.declarations.push(definition);
		}
		for (definition in bodyEmitter.enumDefinitions()) {
			headerUnit.declarations.push(definition);
		}
		for (declaration in bodyEmitter.virtualTableForwardDeclarations()) {
			headerUnit.declarations.push(declaration);
		}
		for (definition in bodyEmitter.classDefinitions()) {
			headerUnit.declarations.push(definition);
		}
		for (definition in bodyEmitter.virtualTableDefinitions()) {
			headerUnit.declarations.push(definition);
		}
		for (global in lowered.globals) {
			final declaration = bodyEmitter.typedDeclarator(global.ir.type, DName(global.cName));
			headerUnit.declarations.push(DVariable({
				storage: [SExtern],
				alignments: [],
				type: declaration.type,
				declarator: declaration.declarator,
				initializer: null,
				attributes: []
			}));
		}
		for (fn in lowered.functions) {
			if (!initializerIds.exists(fn.ir.id)) {
				final functionSpecifiers = nonReturningFunctionIds.exists(fn.ir.id) ? [FNoReturn] : [];
				final declaration = bodyEmitter.functionDeclarator(fn.ir,
					DFunction(DName(fn.cName), FPPrototype(bodyEmitter.parameters(fn.ir, fn.parameterNames, fn.spanLengthNames), false)));
				headerUnit.declarations.push(DPrototype([], functionSpecifiers, declaration.type, declaration.declarator, []));
			}
		}

		final programUnit = sourceUnit();
		for (assertion in bodyEmitter.aggregateLayoutAssertions()) {
			programUnit.declarations.push(assertion);
		}
		for (assertion in bodyEmitter.enumLayoutAssertions()) {
			programUnit.declarations.push(assertion);
		}
		for (assertion in bodyEmitter.classLayoutAssertions()) {
			programUnit.declarations.push(assertion);
		}
		for (prototype in bodyEmitter.virtualThunkPrototypes()) {
			programUnit.declarations.push(prototype);
		}
		final helperNames:Map<String, CIdentifier> = [];
		for (helper in lowered.helpers) {
			helperNames.set(helper.helperId, helper.cName);
		}
		for (global in lowered.globals) {
			final declaration = bodyEmitter.typedDeclarator(global.ir.type, DName(global.cName));
			programUnit.declarations.push(DVariable({
				storage: [],
				alignments: [],
				type: declaration.type,
				declarator: declaration.declarator,
				initializer: bodyEmitter.globalInitializer(global.ir),
				attributes: []
			}));
		}
		for (table in bodyEmitter.virtualTableObjects(functionNames)) {
			programUnit.declarations.push(table);
		}
		for (thunk in bodyEmitter.virtualThunkDefinitions(functionNames)) {
			programUnit.declarations.push(thunk);
		}
		final sources:Array<CStaticFunctionSourcePlan> = [];
		final functionDefinitions:Array<CStaticFunctionDefinitionPlan> = [];
		var nonReturningOrdinal = 0;
		for (fn in lowered.functions) {
			final isInitializer = initializerIds.exists(fn.ir.id);
			if (isInitializer && nonReturningFunctionIds.exists(fn.ir.id)) {
				throw new ProjectEmissionError('static initializer `${fn.ir.id}` unexpectedly participates in a closed call cycle');
			}
			final functionSpecifiers = nonReturningFunctionIds.exists(fn.ir.id) ? [FNoReturn] : [];
			final signature = bodyEmitter.functionDeclarator(fn.ir,
				DFunction(DName(fn.cName), FPPrototype(bodyEmitter.parameters(fn.ir, fn.parameterNames, fn.spanLengthNames), false)));
			final definition:CDecl = DFunction({
				storage: isInitializer ? [SStatic] : [],
				functionSpecifiers: functionSpecifiers,
				returnType: signature.type,
				declarator: signature.declarator,
				body: bodyEmitter.emitBody(fn.ir, fn.parameterNames, fn.localNames, fn.temporaryNames, functionNames, globalNames, helperNames, false,
					fn.tailArgumentNames, fn.labelNames, nonReturningFunctionIds, fn.spanLengthNames, lowered.boundsAbortName),
				attributes: []
			});
			if (nonReturningFunctionIds.exists(fn.ir.id)) {
				final sourcePath = nonReturningSourcePath(nonReturningOrdinal++);
				final unit = sourceUnit();
				unit.declarations.push(definition);
				sources.push({path: sourcePath, unit: unit});
				functionDefinitions.push({functionId: fn.ir.id, sourcePath: sourcePath, declaration: definition});
			} else {
				programUnit.declarations.push(definition);
				functionDefinitions.push({functionId: fn.ir.id, sourcePath: SOURCE_PATH, declaration: definition});
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
		functionDefinitions.sort((left, right) -> compareStrings(left.functionId, right.functionId));
		return new CStaticFunctionDeclarationPlan(HEADER_PATH, new CHeaderUnit(headerGuard, headerUnit), sources, functionDefinitions);
	}

	public function emit(lowered:CBodyLoweringResult, entryFunctionId:String, entryName:CIdentifier, headerGuard:CIdentifier,
			?initializerFunctionIds:Array<String>, ?initializationName:CIdentifier, ?runtimeAbiMajor:Int):Array<GeneratedFile> {
		final declarationPlan = plan(lowered, entryFunctionId, entryName, headerGuard, initializerFunctionIds, initializationName, runtimeAbiMajor);
		return emitPlan(declarationPlan);
	}

	public function emitPlan(declarationPlan:CStaticFunctionDeclarationPlan):Array<GeneratedFile> {
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

	static function float32AbiAssertions():Array<CDecl> {
		final integer = (value:String) -> EInt(CIntegerLiteral.decimal(value));
		final macroValue = (value:String) -> EIdentifier(new CIdentifier(value));
		final equals = (left:CExpr, right:CExpr, message:String) -> DStaticAssert(EBinary(Equal, left, right), message);
		return [
			equals(macroValue("CHAR_BIT"), integer("8"), "hxc c.Float32 requires 8-bit C bytes"),
			equals(EBinary(Multiply, ESizeOfType(new CType(TFloat), DName(null)), macroValue("CHAR_BIT")), integer("32"),
				"hxc c.Float32 requires 32-bit C float storage"),
			equals(macroValue("FLT_RADIX"), integer("2"), "hxc c.Float32 requires radix-2 C floating point"),
			equals(macroValue("FLT_MANT_DIG"), integer("24"), "hxc c.Float32 requires a 24-bit binary32 significand"),
			equals(macroValue("FLT_MAX_EXP"), integer("128"), "hxc c.Float32 requires the binary32 maximum exponent"),
			equals(macroValue("FLT_MIN_EXP"), EUnary(Minus, integer("125")), "hxc c.Float32 requires the binary32 minimum exponent"),
			equals(macroValue("FLT_HAS_SUBNORM"), integer("1"), "hxc c.Float32 requires binary32 subnormal support")
		];
	}

	static function programUsesFloat32(program:HxcIRProgram):Bool {
		for (slot in program.dispatch.slots) {
			if (typesUseFloat32(slot.parameterTypes) || typeUsesFloat32(slot.returnType)) {
				return true;
			}
		}
		for (module in program.modules) {
			for (declaration in module.types) {
				if (typeKindUsesFloat32(declaration.kind)) {
					return true;
				}
			}
			for (instance in module.typeInstances) {
				if (typesUseFloat32(instance.arguments)) {
					return true;
				}
			}
			for (global in module.globals) {
				if (typeUsesFloat32(global.type)) {
					return true;
				}
			}
			for (fn in module.functions) {
				if (functionUsesFloat32(fn)) {
					return true;
				}
			}
		}
		return false;
	}

	static function functionUsesFloat32(fn:HxcIRFunction):Bool {
		if (typeUsesFloat32(fn.returnType)) {
			return true;
		}
		for (parameter in fn.parameters) {
			if (typeUsesFloat32(parameter.type)) {
				return true;
			}
		}
		for (local in fn.locals) {
			if (typeUsesFloat32(local.type)) {
				return true;
			}
		}
		for (block in fn.blocks) {
			for (parameter in block.parameters) {
				if (typeUsesFloat32(parameter.type)) {
					return true;
				}
			}
			for (instruction in block.instructions) {
				if (instruction.result != null && typeUsesFloat32(instruction.result.type)) {
					return true;
				}
				switch instruction.kind {
					case IRIOConvert(_, _, targetType, _, _) | IRIOAllocate(targetType, _, _, _):
						if (typeUsesFloat32(targetType))
							return true;
					case IRIOCall(call):
						if (typeUsesFloat32(call.returnType))
							return true;
					case _:
				}
			}
		}
		return false;
	}

	static function typeKindUsesFloat32(kind:HxcIRTypeKind):Bool {
		return switch kind {
			case IRTKAggregate(fields): fieldsUseFloat32(fields);
			case IRTKTaggedUnion(cases):
				var found = false;
				for (tagCase in cases) {
					for (payload in tagCase.payload) {
						if (typeUsesFloat32(payload.type)) {
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				found;
			case IRTKClass(layout): fieldsUseFloat32(layout.fields);
			case IRTKPrimitive | IRTKReference | IRTKFunction | IRTKExtern: false;
		}
	}

	static function fieldsUseFloat32(fields:Array<HxcIRTypeField>):Bool {
		for (field in fields) {
			if (typeUsesFloat32(field.type))
				return true;
		}
		return false;
	}

	static function typesUseFloat32(types:Array<HxcIRTypeRef>):Bool {
		for (type in types) {
			if (typeUsesFloat32(type))
				return true;
		}
		return false;
	}

	static function typeUsesFloat32(type:HxcIRTypeRef):Bool {
		return switch type {
			case IRTFloat(32): true;
			case IRTPointer(pointee, _) | IRTNullable(pointee, _) | IRTFixedArray(pointee, _, _) | IRTSpan(pointee, _):
				typeUsesFloat32(pointee);
			case IRTFunction(parameters, result): typesUseFloat32(parameters) || typeUsesFloat32(result);
			case _: false;
		}
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

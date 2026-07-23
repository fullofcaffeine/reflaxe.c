package reflaxe.c.emit;

#if (macro || reflaxe_runtime)
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.CProjectLayout.CProjectLayoutPlan;
import reflaxe.c.emit.CProjectLayout.CProjectModuleLayout;
import reflaxe.c.emit.CProjectLayout.CProjectPackageLayout;
import reflaxe.c.emit.CProjectLayout.CProjectLayoutPlanner;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.lowering.CBodyAggregate.CLoweredBodyAggregate;
import reflaxe.c.lowering.CBodyClass.CLoweredBodyClass;
import reflaxe.c.lowering.CBodyEmitter;
import reflaxe.c.lowering.CBodyEnum.CLoweredBodyEnum;
import reflaxe.c.lowering.CBodyLowering.CBodyLoweringResult;
import reflaxe.c.lowering.CBodyLowering.CLoweredBodyFunction;
import reflaxe.c.lowering.CPrimitiveHelperEmitter;
import reflaxe.c.lowering.CBodyRuntimeNames;
import reflaxe.c.lowering.CBodyRuntimeNames.CBodyRuntimeName;
import reflaxe.c.runtime.RuntimeAbiContract;

/** One guarded private header assigned before printing. */
typedef CStaticFunctionHeaderPlan = {
	final path:String;
	final unit:CHeaderUnit;
}

/** Structural function source plan for the first executable slice. */
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
	public final headers:Array<CStaticFunctionHeaderPlan>;

	/** Compatibility view for callers that intentionally request unity output. */
	public final headerPath:String;

	/** Compatibility view for callers that intentionally request unity output. */
	public final header:CHeaderUnit;

	public final sources:Array<CStaticFunctionSourcePlan>;
	public final functionDefinitions:Array<CStaticFunctionDefinitionPlan>;

	public function new(headers:Array<CStaticFunctionHeaderPlan>, sources:Array<CStaticFunctionSourcePlan>,
			functionDefinitions:Array<CStaticFunctionDefinitionPlan>) {
		if (headers.length == 0)
			throw new ProjectEmissionError("static function declaration plan requires at least one private header");
		this.headers = headers.copy();
		this.headerPath = headers[0].path;
		this.header = headers[0].unit;
		this.sources = sources.copy();
		this.functionDefinitions = functionDefinitions.copy();
	}
}

private typedef CModuleDeclaration = {
	final modulePath:String;
	final declaration:CDecl;
}

/** One complete generated value type and the types it embeds by value. */
private class CTypeSemanticPlan {
	public final instanceId:String;
	public final modulePath:String;
	public final declarations:Array<CDecl>;
	public final completeDependencies:Array<String>;

	public function new(instanceId:String, modulePath:String, declarations:Array<CDecl>, completeDependencies:Array<String>) {
		this.instanceId = instanceId;
		this.modulePath = modulePath;
		this.declarations = declarations.copy();
		this.completeDependencies = completeDependencies.copy();
	}
}

private class CFunctionSemanticPlan {
	public final functionId:String;
	public final modulePath:String;
	public final isNonReturning:Bool;
	public final prototype:CDecl;
	public final returnType:CType;
	public final declarator:CDeclarator;
	public final body:CStmt;

	public function new(functionId:String, modulePath:String, isNonReturning:Bool, prototype:CDecl, returnType:CType, declarator:CDeclarator, body:CStmt) {
		this.functionId = functionId;
		this.modulePath = modulePath;
		this.isNonReturning = isNonReturning;
		this.prototype = prototype;
		this.returnType = returnType;
		this.declarator = declarator;
		this.body = body;
	}

	public function definition():CDecl {
		return DFunction({
			storage: [],
			functionSpecifiers: isNonReturning ? [FNoReturn] : [],
			returnType: returnType,
			declarator: declarator,
			body: body,
			attributes: []
		});
	}
}

private class CStaticFunctionSemanticPlan {
	public final common:CTranslationUnit;
	public final aggregateForwards:Array<CDecl>;
	public final aggregateTypes:Array<CTypeSemanticPlan>;
	public final optionalForwards:Array<CDecl>;
	public final optionalTypes:Array<CTypeSemanticPlan>;
	public final enumForwards:Array<CDecl>;
	public final enumTypes:Array<CTypeSemanticPlan>;
	public final virtualForwards:Array<CDecl>;
	public final classForwards:Array<CDecl>;
	public final classTypes:Array<CTypeSemanticPlan>;
	public final virtualDefinitions:Array<CDecl>;
	public final virtualObjectDeclarations:Array<CDecl>;
	public final moduleDependencies:Map<String, Array<String>>;
	public final support:Array<CDecl>;
	public final supportGlobalSplit:Int;
	public final globalDeclarations:Array<CModuleDeclaration>;
	public final globalDefinitions:Array<CModuleDeclaration>;
	public final functions:Array<CFunctionSemanticPlan>;
	public final entry:Array<CDecl>;

	public function new(common:CTranslationUnit, aggregateForwards:Array<CDecl>, aggregateTypes:Array<CTypeSemanticPlan>, optionalForwards:Array<CDecl>,
			optionalTypes:Array<CTypeSemanticPlan>, enumForwards:Array<CDecl>, enumTypes:Array<CTypeSemanticPlan>, virtualForwards:Array<CDecl>,
			classForwards:Array<CDecl>, classTypes:Array<CTypeSemanticPlan>, virtualDefinitions:Array<CDecl>, virtualObjectDeclarations:Array<CDecl>,
			moduleDependencies:Map<String, Array<String>>, support:Array<CDecl>, supportGlobalSplit:Int, globalDeclarations:Array<CModuleDeclaration>,
			globalDefinitions:Array<CModuleDeclaration>, functions:Array<CFunctionSemanticPlan>, entry:Array<CDecl>) {
		this.common = common;
		this.aggregateForwards = aggregateForwards.copy();
		this.aggregateTypes = aggregateTypes.copy();
		this.optionalForwards = optionalForwards.copy();
		this.optionalTypes = optionalTypes.copy();
		this.enumForwards = enumForwards.copy();
		this.enumTypes = enumTypes.copy();
		this.virtualForwards = virtualForwards.copy();
		this.classForwards = classForwards.copy();
		this.classTypes = classTypes.copy();
		this.virtualDefinitions = virtualDefinitions.copy();
		this.virtualObjectDeclarations = virtualObjectDeclarations.copy();
		this.moduleDependencies = moduleDependencies;
		this.support = support.copy();
		this.supportGlobalSplit = supportGlobalSplit;
		this.globalDeclarations = globalDeclarations.copy();
		this.globalDefinitions = globalDefinitions.copy();
		this.functions = functions.copy();
		this.entry = entry.copy();
	}
}

/** Emits a runtime-free private prototype header and strict-C11 source units. */
class CStaticFunctionProjectEmitter {
	public static inline final HEADER_PATH = CProjectLayoutPlan.UMBRELLA_HEADER_PATH;
	public static inline final HEADER_INCLUDE = CProjectLayoutPlan.UMBRELLA_HEADER_INCLUDE;
	public static inline final SOURCE_PATH = CProjectLayoutPlan.UNITY_SOURCE_PATH;
	public static inline final NON_RETURNING_SOURCE_PREFIX = "src/nonreturn_";

	public function new() {}

	public function plan(lowered:CBodyLoweringResult, entryFunctionId:String, entryName:CIdentifier, headerGuard:CIdentifier,
			?initializerFunctionIds:Array<String>, ?initializationName:CIdentifier, ?runtimeAbiMajor:Int):CStaticFunctionDeclarationPlan {
		final layout = new CProjectLayoutPlanner().plan(CProjectLayout.Unity, modulePaths(lowered));
		final guards:Map<String, CIdentifier> = [];
		guards.set(HEADER_PATH, headerGuard);
		return planWithLayout(lowered, entryFunctionId, entryName, layout, guards, initializerFunctionIds, initializationName, runtimeAbiMajor);
	}

	public function planWithLayout(lowered:CBodyLoweringResult, entryFunctionId:String, entryName:CIdentifier, layout:CProjectLayoutPlan,
			headerGuards:Map<String, CIdentifier>, ?initializerFunctionIds:Array<String>, ?initializationName:CIdentifier,
			?runtimeAbiMajor:Int):CStaticFunctionDeclarationPlan {
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

		final bodyEmitter = new CBodyEmitter(lowered.aggregates, lowered.enums, lowered.classes, lowered.arrays, lowered.bytes, lowered.optionals,
			lowered.dispatch, lowered.imports, lowered.managedProgram);
		final helperEmitter = new CPrimitiveHelperEmitter(lowered.helpers);
		final nonReturningFunctionIds = nonReturningCallCycles(lowered.functions);
		var hasNonReturningFunctions = false;
		for (_ in nonReturningFunctionIds) {
			hasNonReturningFunctions = true;
			break;
		}
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
			// A negative array bound is rejected by both strict C11 and C++17. This
			// header-level ABI check therefore stays portable to both consumers;
			// `_Static_assert` itself is C-only under a pedantic C++ compiler.
			headerUnit.declarations.push(DTypedef(new CType(TChar(null)),
				DArray(DName(new CIdentifier(RuntimeAbiContract.MAJOR_CHECK_TYPE)),
					ABFixed(EConditional(EBinary(Equal, EIdentifier(new CIdentifier(RuntimeAbiContract.MAJOR_MACRO)),
						EInt(CIntegerLiteral.decimal(Std.string(runtimeAbiMajor), ISUnsigned))),
						EInt(CIntegerLiteral.decimal("1", ISNone)), EUnary(Minus, EInt(CIntegerLiteral.decimal("1", ISNone))))),
					[]),
				[]));
		}
		for (declaration in managedProgramDeclarations(lowered))
			headerUnit.declarations.push(declaration);
		for (declaration in bodyEmitter.managedObjectDescriptorDeclarations())
			headerUnit.declarations.push(declaration);
		if (usesFloat32) {
			for (assertion in float32AbiAssertions()) {
				headerUnit.declarations.push(assertion);
			}
		}
		for (definition in helperEmitter.definitions(lowered.helpers)) {
			headerUnit.declarations.push(definition);
		}
		for (prototype in bodyEmitter.arrayElementLifecyclePrototypes()) {
			headerUnit.declarations.push(prototype);
		}
		final typeOwners = typeOwnerModules(lowered);
		final aggregateForwards = bodyEmitter.aggregateForwardDeclarations();
		final aggregateTypes = aggregateTypePlans(lowered, bodyEmitter);
		final optionalForwards = bodyEmitter.optionalForwardDeclarations();
		final optionalTypes = optionalTypePlans(lowered, bodyEmitter);
		final enumForwards = bodyEmitter.enumForwardDeclarations();
		final enumTypes = enumTypePlans(lowered, bodyEmitter);
		final virtualForwards = bodyEmitter.virtualTableForwardDeclarations();
		final classForwards = bodyEmitter.classForwardDeclarations();
		final classTypes = classTypePlans(lowered, bodyEmitter);
		final virtualDefinitions = bodyEmitter.virtualTableDefinitions();
		final virtualObjectDeclarations = bodyEmitter.virtualTableObjectDeclarations();
		final moduleDependencies = completeModuleDependencies(lowered, aggregateTypes.concat(optionalTypes).concat(enumTypes).concat(classTypes), typeOwners,
			bodyEmitter);
		final globalDeclarations:Array<CModuleDeclaration> = [];
		final globalDefinitions:Array<CModuleDeclaration> = [];
		for (global in lowered.globals) {
			layout.module(global.modulePath);
			final declaration = bodyEmitter.typedDeclarator(global.ir.type, DName(global.cName));
			globalDeclarations.push({
				modulePath: global.modulePath,
				declaration: DVariable({
					storage: [SExtern],
					alignments: [],
					type: declaration.type,
					declarator: declaration.declarator,
					initializer: null,
					attributes: []
				})
			});
			globalDefinitions.push({
				modulePath: global.modulePath,
				declaration: DVariable({
					storage: [],
					alignments: [],
					type: declaration.type,
					declarator: declaration.declarator,
					initializer: bodyEmitter.globalInitializer(global.ir),
					attributes: []
				})
			});
		}

		final support:Array<CDecl> = [];
		for (definition in managedProgramDefinitions(lowered))
			support.push(definition);
		for (assertion in bodyEmitter.aggregateLayoutAssertions()) {
			support.push(assertion);
		}
		for (assertion in bodyEmitter.enumLayoutAssertions()) {
			support.push(assertion);
		}
		for (assertion in bodyEmitter.classLayoutAssertions()) {
			support.push(assertion);
		}
		for (definition in bodyEmitter.managedObjectDefinitions()) {
			support.push(definition);
		}
		for (prototype in bodyEmitter.virtualThunkPrototypes()) {
			support.push(prototype);
		}
		for (definition in bodyEmitter.arrayElementLifecycleDefinitions()) {
			support.push(definition);
		}
		final helperNames:Map<String, CIdentifier> = [];
		for (helper in lowered.helpers) {
			helperNames.set(helper.helperId, helper.cName);
		}
		final supportGlobalSplit = support.length;
		for (table in bodyEmitter.virtualTableObjects(functionNames)) {
			support.push(table);
		}
		for (thunk in bodyEmitter.virtualThunkDefinitions(functionNames)) {
			support.push(thunk);
		}
		final functions:Array<CFunctionSemanticPlan> = [];
		for (fn in lowered.functions) {
			layout.module(fn.modulePath);
			final isInitializer = initializerIds.exists(fn.ir.id);
			if (isInitializer && nonReturningFunctionIds.exists(fn.ir.id)) {
				throw new ProjectEmissionError('static initializer `${fn.ir.id}` unexpectedly participates in a closed call cycle');
			}
			final isNonReturning = nonReturningFunctionIds.exists(fn.ir.id);
			final functionSpecifiers = isNonReturning ? [FNoReturn] : [];
			final signature = bodyEmitter.functionDeclarator(fn.ir,
				DFunction(DName(fn.cName), FPPrototype(bodyEmitter.parameters(fn.ir, fn.parameterNames, fn.spanLengthNames), false)));
			// Body lowering has already built the ordinary structural C body. Rebuild
			// only when a closed call cycle changes call/tail-call emission; the common
			// acyclic program must not pay for an identical third traversal.
			final body = hasNonReturningFunctions ? bodyEmitter.emitBody(fn.ir, fn.parameterNames, fn.localNames, fn.temporaryNames, functionNames,
				globalNames, helperNames, false, fn.tailArgumentNames, fn.labelNames, nonReturningFunctionIds, fn.spanLengthNames,
				lowered.boundsAbortName) : fn.body;
			functions.push(new CFunctionSemanticPlan(fn.ir.id, fn.modulePath, isNonReturning,
				DPrototype([], functionSpecifiers, signature.type, signature.declarator, []), signature.type, signature.declarator, body));
		}
		final entryDeclarations:Array<CDecl> = [];
		final entryStatements:Array<CStmt> = [];
		appendManagedProgramStart(entryStatements, lowered);
		if (orderedInitializers.length > 0) {
			if (initializationName == null) {
				throw new ProjectEmissionError("validated static initialization order lost its compiler-owned name");
			}
			final initializationStatements:Array<CStmt> = [];
			for (initializer in orderedInitializers) {
				initializationStatements.push(SExpr(ECall(EIdentifier(initializer.cName), [])));
			}
			entryDeclarations.push(DFunction({
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
		appendManagedProgramStop(entryStatements, lowered);
		entryStatements.push(SReturn(EInt(CIntegerLiteral.decimal("0"))));
		entryDeclarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNativeInt(IRInt, true)),
			declarator: DFunction(DName(entryName), FPPrototype([], false)),
			body: SBlock(entryStatements),
			attributes: []
		}));
		final semantic = new CStaticFunctionSemanticPlan(headerUnit, aggregateForwards, aggregateTypes, optionalForwards, optionalTypes, enumForwards,
			enumTypes, virtualForwards, classForwards, classTypes, virtualDefinitions, virtualObjectDeclarations, moduleDependencies, support,
			supportGlobalSplit, globalDeclarations, globalDefinitions, functions, entryDeclarations);
		return switch layout.layout {
			case Unity: assignUnity(semantic, layout, headerGuards);
			case Split: assignSplit(semantic, layout, headerGuards);
			case Package: assignPackage(semantic, layout, headerGuards);
		};
	}

	public function emit(lowered:CBodyLoweringResult, entryFunctionId:String, entryName:CIdentifier, headerGuard:CIdentifier,
			?initializerFunctionIds:Array<String>, ?initializationName:CIdentifier, ?runtimeAbiMajor:Int):Array<GeneratedFile> {
		final declarationPlan = plan(lowered, entryFunctionId, entryName, headerGuard, initializerFunctionIds, initializationName, runtimeAbiMajor);
		return emitPlan(declarationPlan);
	}

	public function emitPlan(declarationPlan:CStaticFunctionDeclarationPlan):Array<GeneratedFile> {
		final printer = new CASTPrinter();
		final files:Array<GeneratedFile> = [];
		for (header in declarationPlan.headers)
			files.push(new GeneratedFile(header.path, printer.printHeader(header.unit), GeneratedFileKind.PrivateHeader));
		for (source in declarationPlan.sources) {
			files.push(new GeneratedFile(source.path, printer.printTranslationUnit(source.unit), GeneratedFileKind.Source));
		}
		files.sort((left, right) -> compareStrings(left.relativePath, right.relativePath));
		return files;
	}

	/** Declare the generated executable's request-local collector state. */
	static function managedProgramDeclarations(lowered:CBodyLoweringResult):Array<CDecl> {
		final names = lowered.managedProgram;
		if (names == null)
			return [];
		return [
			DVariable({
				storage: [SExtern],
				alignments: [],
				type: runtimeStruct(CBRNGcType),
				declarator: DName(names.collector),
				initializer: null,
				attributes: []
			}),
			DVariable({
				storage: [SExtern],
				alignments: [],
				type: runtimeStruct(CBRNGcThreadType),
				declarator: DName(names.thread),
				initializer: null,
				attributes: []
			})
		];
	}

	/** Define zero-state storage once, in the compiler-owned support unit. */
	static function managedProgramDefinitions(lowered:CBodyLoweringResult):Array<CDecl> {
		final names = lowered.managedProgram;
		if (names == null)
			return [];
		return [
			DVariable({
				storage: [],
				alignments: [],
				type: runtimeStruct(CBRNGcType),
				declarator: DName(names.collector),
				initializer: IExpr(EIdentifier(runtimeName(CBRNGcInitializer))),
				attributes: []
			}),
			DVariable({
				storage: [],
				alignments: [],
				type: runtimeStruct(CBRNGcThreadType),
				declarator: DName(names.thread),
				initializer: IExpr(EIdentifier(runtimeName(CBRNGcThreadInitializer))),
				attributes: []
			})
		];
	}

	/** Initialize the collector and register the main execution-context roots. */
	static function appendManagedProgramStart(statements:Array<CStmt>, lowered:CBodyLoweringResult):Void {
		final names = lowered.managedProgram;
		if (names == null)
			return;
		final config = ECompoundLiteral(runtimeStruct(CBRNGcConfigType), DName(null), IList([
			{designators: [], value: IExpr(ECall(EIdentifier(runtimeName(CBRNDefaultAllocator)), []))},
			// One mebibyte is a deterministic initial pressure threshold, not a heap cap.
			{designators: [], value: IExpr(EInt(CIntegerLiteral.decimal("1048576", ISUnsigned)))},
			{designators: [], value: IExpr(ENull)},
			{designators: [], value: IExpr(ENull)}
		]));
		statements.push(checkedRuntimeCall(ECall(EIdentifier(runtimeName(CBRNGcInit)),
			[EUnary(AddressOf, config), EUnary(AddressOf, EIdentifier(names.collector))])));
		statements.push(checkedRuntimeCall(ECall(EIdentifier(runtimeName(CBRNGcThreadRegister)), [
			EUnary(AddressOf, EIdentifier(names.collector)),
			EUnary(AddressOf, EIdentifier(names.thread))
		])));
	}

	/** Unregister roots before disposing the executable-owned collector. */
	static function appendManagedProgramStop(statements:Array<CStmt>, lowered:CBodyLoweringResult):Void {
		final names = lowered.managedProgram;
		if (names == null)
			return;
		statements.push(checkedRuntimeCall(ECall(EIdentifier(runtimeName(CBRNGcThreadUnregister)), [EUnary(AddressOf, EIdentifier(names.thread))])));
		statements.push(checkedRuntimeCall(ECall(EIdentifier(runtimeName(CBRNGcDispose)), [EUnary(AddressOf, EIdentifier(names.collector))])));
	}

	static function checkedRuntimeCall(call:CExpr):CStmt
		return SIf(EBinary(NotEqual, call, EIdentifier(runtimeName(CBRNStatusOk))), SExpr(ECall(EIdentifier(runtimeName(CBRNAbort)), [])), null);

	static function runtimeStruct(name:CBodyRuntimeName):CType
		return new CType(TStruct(runtimeName(name)));

	static function runtimeName(name:CBodyRuntimeName):CIdentifier
		return CBodyRuntimeNames.identifier(name);

	static function sourceUnit(?headerInclude:String):CTranslationUnit {
		final unit = new CTranslationUnit();
		unit.includes.push({path: headerInclude == null ? HEADER_INCLUDE : headerInclude, kind: Local});
		return unit;
	}

	static function assignUnity(semantic:CStaticFunctionSemanticPlan, layout:CProjectLayoutPlan,
			headerGuards:Map<String, CIdentifier>):CStaticFunctionDeclarationPlan {
		final headerUnit = copyUnit(semantic.common);
		appendDeclarations(headerUnit, semantic.enumForwards);
		appendDeclarations(headerUnit, semantic.optionalForwards);
		appendDeclarations(headerUnit, semantic.virtualForwards);
		appendDeclarations(headerUnit, semantic.classForwards);
		appendTypeDeclarations(headerUnit, semantic.aggregateTypes.concat(semantic.optionalTypes).concat(semantic.enumTypes).concat(semantic.classTypes));
		appendDeclarations(headerUnit, semantic.virtualDefinitions);
		appendDeclarations(headerUnit, semantic.virtualObjectDeclarations);
		for (global in semantic.globalDeclarations)
			headerUnit.declarations.push(global.declaration);
		for (fn in semantic.functions)
			headerUnit.declarations.push(fn.prototype);

		final programUnit = sourceUnit();
		for (index in 0...semantic.supportGlobalSplit)
			programUnit.declarations.push(semantic.support[index]);
		for (global in semantic.globalDefinitions)
			programUnit.declarations.push(global.declaration);
		for (index in semantic.supportGlobalSplit...semantic.support.length)
			programUnit.declarations.push(semantic.support[index]);

		final sources:Array<CStaticFunctionSourcePlan> = [];
		final functionDefinitions:Array<CStaticFunctionDefinitionPlan> = [];
		var nonReturningOrdinal = 0;
		for (fn in semantic.functions) {
			final definition = fn.definition();
			if (fn.isNonReturning) {
				final sourcePath = nonReturningSourcePath(nonReturningOrdinal++);
				final unit = sourceUnit();
				unit.declarations.push(definition);
				sources.push({path: sourcePath, unit: unit});
				functionDefinitions.push({functionId: fn.functionId, sourcePath: sourcePath, declaration: definition});
			} else {
				programUnit.declarations.push(definition);
				functionDefinitions.push({functionId: fn.functionId, sourcePath: SOURCE_PATH, declaration: definition});
			}
		}
		for (declaration in semantic.entry)
			programUnit.declarations.push(declaration);
		sources.push({path: SOURCE_PATH, unit: programUnit});
		sources.sort((left, right) -> compareStrings(left.path, right.path));
		functionDefinitions.sort((left, right) -> compareStrings(left.functionId, right.functionId));
		return new CStaticFunctionDeclarationPlan([
			{path: HEADER_PATH, unit: new CHeaderUnit(requireGuard(layout, headerGuards, HEADER_PATH), headerUnit)}
		], sources, functionDefinitions);
	}

	static function assignSplit(semantic:CStaticFunctionSemanticPlan, layout:CProjectLayoutPlan,
			headerGuards:Map<String, CIdentifier>):CStaticFunctionDeclarationPlan {
		final headers:Array<CStaticFunctionHeaderPlan> = [];
		final typesUnit = copyUnit(semantic.common);
		appendDeclarations(typesUnit, semantic.aggregateForwards);
		appendDeclarations(typesUnit, semantic.optionalForwards);
		appendDeclarations(typesUnit, semantic.enumForwards);
		appendDeclarations(typesUnit, semantic.virtualForwards);
		appendDeclarations(typesUnit, semantic.classForwards);
		appendDeclarations(typesUnit, semantic.virtualObjectDeclarations);
		headers.push({
			path: CProjectLayoutPlan.TYPES_HEADER_PATH,
			unit: new CHeaderUnit(requireGuard(layout, headerGuards, CProjectLayoutPlan.TYPES_HEADER_PATH), typesUnit)
		});

		final moduleHeaderUnits:Map<String, CTranslationUnit> = [];
		for (module in layout.modules) {
			final unit = new CTranslationUnit();
			unit.includes.push({path: CProjectLayoutPlan.TYPES_HEADER_INCLUDE, kind: Local});
			final dependencies = semantic.moduleDependencies.get(module.modulePath);
			if (dependencies == null)
				throw new ProjectEmissionError('split project lost complete-type dependencies for `${module.modulePath}`');
			for (dependency in dependencies)
				unit.includes.push({path: layout.module(dependency).headerInclude, kind: Local});
			moduleHeaderUnits.set(module.modulePath, unit);
		}
		appendModuleTypes(moduleHeaderUnits, semantic.aggregateTypes.concat(semantic.optionalTypes).concat(semantic.enumTypes).concat(semantic.classTypes));
		for (global in semantic.globalDeclarations)
			requireModuleUnit(moduleHeaderUnits, global.modulePath).declarations.push(global.declaration);
		for (fn in semantic.functions)
			requireModuleUnit(moduleHeaderUnits, fn.modulePath).declarations.push(fn.prototype);

		final umbrella = new CTranslationUnit();
		for (module in layout.modules) {
			final unit = requireModuleUnit(moduleHeaderUnits, module.modulePath);
			headers.push({path: module.headerPath, unit: new CHeaderUnit(requireGuard(layout, headerGuards, module.headerPath), unit)});
		}
		for (module in dependencyOrderedModules(layout, semantic.moduleDependencies)) {
			umbrella.includes.push({path: module.headerInclude, kind: Local});
		}
		appendDeclarations(umbrella, semantic.virtualDefinitions);
		headers.push({
			path: HEADER_PATH,
			unit: new CHeaderUnit(requireGuard(layout, headerGuards, HEADER_PATH), umbrella)
		});

		final moduleSources:Map<String, CTranslationUnit> = [];
		for (global in semantic.globalDefinitions) {
			moduleSource(moduleSources, layout.module(global.modulePath)).declarations.push(global.declaration);
		}
		final sources:Array<CStaticFunctionSourcePlan> = [];
		final functionDefinitions:Array<CStaticFunctionDefinitionPlan> = [];
		final nonReturningOrdinals:Map<String, Int> = [];
		for (fn in semantic.functions) {
			final definition = fn.definition();
			if (fn.isNonReturning) {
				final module = layout.module(fn.modulePath);
				final ordinal = nonReturningOrdinals.exists(fn.modulePath) ? nonReturningOrdinals.get(fn.modulePath) : 0;
				if (ordinal == null)
					throw new ProjectEmissionError('split project lost non-returning ordinal for `${fn.modulePath}`');
				nonReturningOrdinals.set(fn.modulePath, ordinal + 1);
				final sourcePath = module.nonReturningSourcePath(ordinal);
				final unit = sourceUnit();
				unit.declarations.push(definition);
				sources.push({path: sourcePath, unit: unit});
				functionDefinitions.push({functionId: fn.functionId, sourcePath: sourcePath, declaration: definition});
			} else {
				final module = layout.module(fn.modulePath);
				moduleSource(moduleSources, module).declarations.push(definition);
				functionDefinitions.push({functionId: fn.functionId, sourcePath: module.sourcePath, declaration: definition});
			}
		}
		for (module in layout.modules) {
			final unit = moduleSources.get(module.modulePath);
			if (unit != null && unit.declarations.length > 0)
				sources.push({path: module.sourcePath, unit: unit});
		}
		if (semantic.support.length > 0) {
			final supportUnit = sourceUnit();
			for (declaration in semantic.support)
				supportUnit.declarations.push(declaration);
			sources.push({path: CProjectLayoutPlan.SUPPORT_SOURCE_PATH, unit: supportUnit});
		}
		final entryUnit = sourceUnit();
		for (declaration in semantic.entry)
			entryUnit.declarations.push(declaration);
		sources.push({path: CProjectLayoutPlan.ENTRY_SOURCE_PATH, unit: entryUnit});

		headers.sort((left, right) -> {
			if (left.path == right.path)
				return 0;
			if (left.path == HEADER_PATH)
				return -1;
			if (right.path == HEADER_PATH)
				return 1;
			return compareStrings(left.path, right.path);
		});
		sources.sort((left, right) -> compareStrings(left.path, right.path));
		functionDefinitions.sort((left, right) -> compareStrings(left.functionId, right.functionId));
		return new CStaticFunctionDeclarationPlan(headers, sources, functionDefinitions);
	}

	/**
		Assigns declarations and definitions by Haxe package after semantic
		lowering is complete. A package is only a file-ownership boundary: the
		validated HxcIR, finalized C names, runtime plan, and declaration facts are
		the same values consumed by all three output layouts.
	**/
	static function assignPackage(semantic:CStaticFunctionSemanticPlan, layout:CProjectLayoutPlan,
			headerGuards:Map<String, CIdentifier>):CStaticFunctionDeclarationPlan {
		final headers:Array<CStaticFunctionHeaderPlan> = [];
		final typesUnit = copyUnit(semantic.common);
		appendDeclarations(typesUnit, semantic.aggregateForwards);
		appendDeclarations(typesUnit, semantic.optionalForwards);
		appendDeclarations(typesUnit, semantic.enumForwards);
		appendDeclarations(typesUnit, semantic.virtualForwards);
		appendDeclarations(typesUnit, semantic.classForwards);
		appendDeclarations(typesUnit, semantic.virtualObjectDeclarations);
		headers.push({
			path: CProjectLayoutPlan.TYPES_HEADER_PATH,
			unit: new CHeaderUnit(requireGuard(layout, headerGuards, CProjectLayoutPlan.TYPES_HEADER_PATH), typesUnit)
		});

		final packageDependencies = completePackageDependencies(layout, semantic.moduleDependencies);
		final packageHeaderUnits:Map<String, CTranslationUnit> = [];
		for (pack in layout.packages) {
			final unit = new CTranslationUnit();
			unit.includes.push({path: CProjectLayoutPlan.TYPES_HEADER_INCLUDE, kind: Local});
			final dependencies = packageDependencies.get(pack.packagePath);
			if (dependencies == null)
				throw new ProjectEmissionError('package project lost complete-type dependencies for `${pack.packagePath}`');
			for (dependency in dependencies)
				unit.includes.push({path: layout.packageLayout(dependency).headerInclude, kind: Local});
			packageHeaderUnits.set(pack.packagePath, unit);
		}
		appendPackageTypes(packageHeaderUnits, layout,
			semantic.aggregateTypes.concat(semantic.optionalTypes).concat(semantic.enumTypes).concat(semantic.classTypes));
		for (global in semantic.globalDeclarations) {
			requirePackageUnit(packageHeaderUnits, layout.packageForModule(global.modulePath).packagePath).declarations.push(global.declaration);
		}
		for (fn in semantic.functions) {
			requirePackageUnit(packageHeaderUnits, layout.packageForModule(fn.modulePath).packagePath).declarations.push(fn.prototype);
		}

		final umbrella = new CTranslationUnit();
		for (pack in layout.packages) {
			final unit = requirePackageUnit(packageHeaderUnits, pack.packagePath);
			headers.push({path: pack.headerPath, unit: new CHeaderUnit(requireGuard(layout, headerGuards, pack.headerPath), unit)});
		}
		for (pack in dependencyOrderedPackages(layout, packageDependencies))
			umbrella.includes.push({path: pack.headerInclude, kind: Local});
		appendDeclarations(umbrella, semantic.virtualDefinitions);
		headers.push({
			path: HEADER_PATH,
			unit: new CHeaderUnit(requireGuard(layout, headerGuards, HEADER_PATH), umbrella)
		});

		final packageSources:Map<String, CTranslationUnit> = [];
		for (pack in layout.packages)
			packageSources.set(pack.packagePath, sourceUnit());
		for (global in semantic.globalDefinitions) {
			packageSource(packageSources, layout.packageForModule(global.modulePath)).declarations.push(global.declaration);
		}
		final sources:Array<CStaticFunctionSourcePlan> = [];
		final functionDefinitions:Array<CStaticFunctionDefinitionPlan> = [];
		final nonReturningOrdinals:Map<String, Int> = [];
		for (fn in semantic.functions) {
			final definition = fn.definition();
			final pack = layout.packageForModule(fn.modulePath);
			if (fn.isNonReturning) {
				final ordinal = nonReturningOrdinals.exists(pack.packagePath) ? nonReturningOrdinals.get(pack.packagePath) : 0;
				if (ordinal == null)
					throw new ProjectEmissionError('package project lost non-returning ordinal for `${pack.packagePath}`');
				nonReturningOrdinals.set(pack.packagePath, ordinal + 1);
				final sourcePath = pack.nonReturningSourcePath(ordinal);
				final unit = sourceUnit();
				unit.declarations.push(definition);
				sources.push({path: sourcePath, unit: unit});
				functionDefinitions.push({functionId: fn.functionId, sourcePath: sourcePath, declaration: definition});
			} else {
				packageSource(packageSources, pack).declarations.push(definition);
				functionDefinitions.push({functionId: fn.functionId, sourcePath: pack.sourcePath, declaration: definition});
			}
		}
		for (pack in layout.packages) {
			final unit = packageSources.get(pack.packagePath);
			if (unit == null)
				throw new ProjectEmissionError('package project lost source unit for `${pack.packagePath}`');
			sources.push({path: pack.sourcePath, unit: unit});
		}
		if (semantic.support.length > 0) {
			final supportUnit = sourceUnit();
			for (declaration in semantic.support)
				supportUnit.declarations.push(declaration);
			sources.push({path: CProjectLayoutPlan.SUPPORT_SOURCE_PATH, unit: supportUnit});
		}
		final entryUnit = sourceUnit();
		for (declaration in semantic.entry)
			entryUnit.declarations.push(declaration);
		sources.push({path: CProjectLayoutPlan.ENTRY_SOURCE_PATH, unit: entryUnit});

		headers.sort(compareHeaderPlans);
		sources.sort((left, right) -> compareStrings(left.path, right.path));
		functionDefinitions.sort((left, right) -> compareStrings(left.functionId, right.functionId));
		return new CStaticFunctionDeclarationPlan(headers, sources, functionDefinitions);
	}

	static function moduleSource(units:Map<String, CTranslationUnit>, module:CProjectModuleLayout):CTranslationUnit {
		var unit = units.get(module.modulePath);
		if (unit == null) {
			unit = sourceUnit();
			units.set(module.modulePath, unit);
		}
		return unit;
	}

	static function packageSource(units:Map<String, CTranslationUnit>, pack:CProjectPackageLayout):CTranslationUnit {
		final unit = units.get(pack.packagePath);
		if (unit == null)
			throw new ProjectEmissionError('package project cannot resolve source unit for `${pack.packagePath}`');
		return unit;
	}

	static function requireModuleUnit(units:Map<String, CTranslationUnit>, modulePath:String):CTranslationUnit {
		final unit = units.get(modulePath);
		if (unit == null)
			throw new ProjectEmissionError('split project cannot resolve header unit for Haxe module `$modulePath`');
		return unit;
	}

	static function requirePackageUnit(units:Map<String, CTranslationUnit>, packagePath:String):CTranslationUnit {
		final unit = units.get(packagePath);
		if (unit == null)
			throw new ProjectEmissionError('package project cannot resolve header unit for Haxe package `$packagePath`');
		return unit;
	}

	static function requireGuard(layout:CProjectLayoutPlan, guards:Map<String, CIdentifier>, path:String):CIdentifier {
		if (layout.headerPaths.indexOf(path) == -1)
			throw new ProjectEmissionError('project layout does not own header guard path `$path`');
		final guard = guards.get(path);
		if (guard == null)
			throw new ProjectEmissionError('project layout has no finalized header guard for `$path`');
		return guard;
	}

	static function copyUnit(source:CTranslationUnit):CTranslationUnit {
		final copy = new CTranslationUnit();
		for (include in source.includes)
			copy.includes.push(include);
		for (declaration in source.declarations)
			copy.declarations.push(declaration);
		return copy;
	}

	static function appendDeclarations(unit:CTranslationUnit, declarations:Array<CDecl>):Void {
		for (declaration in declarations)
			unit.declarations.push(declaration);
	}

	static function appendTypeDeclarations(unit:CTranslationUnit, plans:Array<CTypeSemanticPlan>):Void {
		for (plan in dependencyOrderedTypePlans(plans))
			appendDeclarations(unit, plan.declarations);
	}

	static function appendModuleTypes(units:Map<String, CTranslationUnit>, plans:Array<CTypeSemanticPlan>):Void {
		for (plan in dependencyOrderedTypePlans(plans))
			appendDeclarations(requireModuleUnit(units, plan.modulePath), plan.declarations);
	}

	/**
		Orders complete generated types before any owning type that stores them by
		value. A native C enum cannot be forward-declared in strict C11, so grouping
		all aggregate, enum, and class plans is required when one semantic family
		contains another—for example, a Haxe record with an enum-valued field.
	**/
	static function dependencyOrderedTypePlans(plans:Array<CTypeSemanticPlan>):Array<CTypeSemanticPlan> {
		final byId:Map<String, CTypeSemanticPlan> = [];
		final roots = plans.copy();
		roots.sort((left, right) -> compareStrings(left.instanceId, right.instanceId));
		for (plan in roots) {
			if (byId.exists(plan.instanceId))
				throw new ProjectEmissionError('project repeats complete type `${plan.instanceId}`');
			byId.set(plan.instanceId, plan);
		}
		final result:Array<CTypeSemanticPlan> = [];
		final state:Map<String, Int> = [];
		for (plan in roots)
			visitTypePlan(plan, byId, state, result);
		return result;
	}

	static function visitTypePlan(plan:CTypeSemanticPlan, byId:Map<String, CTypeSemanticPlan>, state:Map<String, Int>, result:Array<CTypeSemanticPlan>):Void {
		final current = state.get(plan.instanceId);
		if (current == 2)
			return;
		if (current == 1)
			throw new ProjectEmissionError('complete-type dependency cycle reaches `${plan.instanceId}`');
		state.set(plan.instanceId, 1);
		for (dependencyId in plan.completeDependencies) {
			final dependency = byId.get(dependencyId);
			if (dependency != null)
				visitTypePlan(dependency, byId, state, result);
		}
		state.set(plan.instanceId, 2);
		result.push(plan);
	}

	/**
		Orders complete type definitions inside each package. Cross-package hard
		edges are header includes; dependencies inside one package must instead
		appear earlier in that same header.
	**/
	static function appendPackageTypes(units:Map<String, CTranslationUnit>, layout:CProjectLayoutPlan, plans:Array<CTypeSemanticPlan>):Void {
		final byId:Map<String, CTypeSemanticPlan> = [];
		final byPackage:Map<String, Array<CTypeSemanticPlan>> = [];
		for (plan in plans) {
			if (byId.exists(plan.instanceId))
				throw new ProjectEmissionError('package project repeats complete type `${plan.instanceId}`');
			byId.set(plan.instanceId, plan);
			final packagePath = layout.packageForModule(plan.modulePath).packagePath;
			var grouped = byPackage.get(packagePath);
			if (grouped == null) {
				grouped = [];
				byPackage.set(packagePath, grouped);
			}
			grouped.push(plan);
		}
		for (pack in layout.packages) {
			final grouped = byPackage.get(pack.packagePath);
			if (grouped == null)
				continue;
			grouped.sort((left, right) -> compareStrings(left.instanceId, right.instanceId));
			final ordered:Array<CTypeSemanticPlan> = [];
			final state:Map<String, Int> = [];
			for (plan in grouped)
				visitPackageType(plan, pack.packagePath, layout, byId, state, ordered);
			final unit = requirePackageUnit(units, pack.packagePath);
			for (plan in ordered)
				appendDeclarations(unit, plan.declarations);
		}
	}

	static function visitPackageType(plan:CTypeSemanticPlan, packagePath:String, layout:CProjectLayoutPlan, byId:Map<String, CTypeSemanticPlan>,
			state:Map<String, Int>, result:Array<CTypeSemanticPlan>):Void {
		final current = state.get(plan.instanceId);
		if (current == 2)
			return;
		if (current == 1)
			throw new ProjectEmissionError('complete-type dependency cycle reaches `${plan.instanceId}` inside Haxe package `$packagePath`');
		state.set(plan.instanceId, 1);
		for (dependencyId in plan.completeDependencies) {
			final dependency = byId.get(dependencyId);
			if (dependency != null && layout.packageForModule(dependency.modulePath).packagePath == packagePath)
				visitPackageType(dependency, packagePath, layout, byId, state, result);
		}
		state.set(plan.instanceId, 2);
		result.push(plan);
	}

	static function typeOwnerModules(lowered:CBodyLoweringResult):Map<String, String> {
		final result:Map<String, String> = [];
		for (aggregate in lowered.aggregates)
			addTypeOwner(result, aggregate.prepared.instanceId, aggregate.prepared.ownerModule);
		for (value in lowered.enums)
			addTypeOwner(result, value.prepared.instanceId, value.prepared.ownerModule);
		for (value in lowered.classes)
			addTypeOwner(result, value.prepared.instanceId, value.prepared.ownerModule);
		for (value in lowered.optionals)
			addTypeOwner(result, value.prepared.planId, value.prepared.ownerModule);
		return result;
	}

	static function addTypeOwner(owners:Map<String, String>, instanceId:String, modulePath:String):Void {
		final previous = owners.get(instanceId);
		if (previous != null && previous != modulePath)
			throw new ProjectEmissionError('type instance `$instanceId` has conflicting module owners `$previous` and `$modulePath`');
		owners.set(instanceId, modulePath);
	}

	static function aggregateTypePlans(lowered:CBodyLoweringResult, emitter:CBodyEmitter):Array<CTypeSemanticPlan> {
		final byId:Map<String, CLoweredBodyAggregate> = [];
		for (aggregate in lowered.aggregates)
			byId.set(aggregate.prepared.instanceId, aggregate);
		final result:Array<CTypeSemanticPlan> = [];
		for (instanceId in emitter.orderedAggregateInstanceIds()) {
			final aggregate = byId.get(instanceId);
			if (aggregate == null)
				throw new ProjectEmissionError('aggregate definition order contains unknown instance `$instanceId`');
			final dependencies:Array<String> = [];
			for (field in aggregate.fields)
				addDefinitionTypeDependencies(field.type.irType, dependencies, emitter);
			dependencies.sort(compareStrings);
			result.push(new CTypeSemanticPlan(instanceId, aggregate.prepared.ownerModule, [emitter.aggregateDefinition(instanceId)], dependencies));
		}
		return result;
	}

	static function enumTypePlans(lowered:CBodyLoweringResult, emitter:CBodyEmitter):Array<CTypeSemanticPlan> {
		final byId:Map<String, CLoweredBodyEnum> = [];
		for (value in lowered.enums)
			byId.set(value.prepared.instanceId, value);
		final result:Array<CTypeSemanticPlan> = [];
		for (instanceId in emitter.orderedEnumInstanceIds()) {
			final value = byId.get(instanceId);
			if (value == null)
				throw new ProjectEmissionError('enum definition order contains unknown instance `$instanceId`');
			final dependencies:Array<String> = [];
			for (tagCase in value.cases)
				for (payload in tagCase.payload)
					addDefinitionTypeDependencies(payload.prepared.storageType(), dependencies, emitter);
			dependencies.sort(compareStrings);
			result.push(new CTypeSemanticPlan(instanceId, value.prepared.ownerModule, emitter.enumDefinitionsFor(instanceId), dependencies));
		}
		return result;
	}

	/**
		Give every direct optional record one ordinary generated-type owner.

		The wrapper depends on its payload record being complete first. Any record or
		function that then uses the wrapper by value depends on this plan ID, which is
		what keeps split and package headers valid without duplicating definitions.
	**/
	static function optionalTypePlans(lowered:CBodyLoweringResult, emitter:CBodyEmitter):Array<CTypeSemanticPlan> {
		final result:Array<CTypeSemanticPlan> = [];
		for (planId in emitter.orderedOptionalPlanIds()) {
			var loweredOptional = null;
			for (candidate in lowered.optionals)
				if (candidate.prepared.planId == planId)
					loweredOptional = candidate;
			if (loweredOptional == null)
				throw new ProjectEmissionError('optional definition order contains unknown plan `$planId`');
			final dependencies:Array<String> = [];
			addDefinitionTypeDependencies(emitter.optionalPayloadType(planId), dependencies, emitter);
			dependencies.sort(compareStrings);
			result.push(new CTypeSemanticPlan(planId, loweredOptional.prepared.ownerModule, [emitter.optionalDefinition(planId)], dependencies));
		}
		return result;
	}

	static function classTypePlans(lowered:CBodyLoweringResult, emitter:CBodyEmitter):Array<CTypeSemanticPlan> {
		final byId:Map<String, CLoweredBodyClass> = [];
		for (value in lowered.classes)
			byId.set(value.prepared.instanceId, value);
		final result:Array<CTypeSemanticPlan> = [];
		for (instanceId in emitter.orderedClassInstanceIds()) {
			final value = byId.get(instanceId);
			if (value == null)
				throw new ProjectEmissionError('class definition order contains unknown instance `$instanceId`');
			final dependencies:Array<String> = [];
			if (value.prepared.base != null)
				addDefinitionTypeDependencies(IRTInstance(value.prepared.base.instanceId), dependencies, emitter);
			for (field in value.fields)
				addDefinitionTypeDependencies(field.prepared.type.irType, dependencies, emitter);
			dependencies.sort(compareStrings);
			result.push(new CTypeSemanticPlan(instanceId, value.prepared.ownerModule, [emitter.classDefinition(instanceId)], dependencies));
		}
		return result;
	}

	/** Types that must be complete while laying out an owning definition. */
	static function addDefinitionTypeDependencies(type:HxcIRTypeRef, dependencies:Array<String>, emitter:CBodyEmitter):Void {
		switch type {
			case IRTInstance(instanceId):
				if (dependencies.indexOf(instanceId) == -1)
					dependencies.push(instanceId);
			case IRTNullable(_, IRNTagged):
				final planId = emitter.optionalPlanId(type);
				if (dependencies.indexOf(planId) == -1)
					dependencies.push(planId);
			case IRTFixedArray(element, _, _):
				addDefinitionTypeDependencies(element, dependencies, emitter);
			case IRTPointer(pointee, _) | IRTNullable(pointee, IRNPointer) | IRTSpan(pointee, _):
				addDeclarationHeaderDependencies(pointee, dependencies, emitter);
			case IRTFunction(parameters, result):
				for (parameter in parameters)
					addDeclarationHeaderDependencies(parameter, dependencies, emitter);
				addDeclarationHeaderDependencies(result, dependencies, emitter);
			case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_) | IRTString | IRTCString | IRTVoid | IRTDynamic:
		}
	}

	static function completeModuleDependencies(lowered:CBodyLoweringResult, typePlans:Array<CTypeSemanticPlan>, typeOwners:Map<String, String>,
			emitter:CBodyEmitter):Map<String, Array<String>> {
		final result:Map<String, Array<String>> = [];
		for (module in lowered.program.modules)
			result.set(module.id, []);
		for (plan in typePlans) {
			requireModuleDependencyList(result, plan.modulePath);
			for (instanceId in plan.completeDependencies)
				addModuleDependencyForInstance(result, plan.modulePath, instanceId, typeOwners);
		}
		for (fn in lowered.functions) {
			addModuleDeclarationTypeDependency(result, fn.modulePath, fn.ir.returnType, typeOwners, emitter);
			for (parameter in fn.ir.parameters)
				addModuleDeclarationTypeDependency(result, fn.modulePath, parameter.type, typeOwners, emitter);
		}
		for (global in lowered.globals)
			addModuleDeclarationTypeDependency(result, global.modulePath, global.ir.type, typeOwners, emitter);
		for (dependencies in result)
			dependencies.sort(compareStrings);
		return result;
	}

	/**
		Header declarations only need defining headers for types that strict C11
		cannot forward-declare. Forward-declarable structs remain soft edges; every
		module source includes the umbrella after all complete definitions exist.
	**/
	static function addModuleDeclarationTypeDependency(result:Map<String, Array<String>>, modulePath:String, type:HxcIRTypeRef,
			typeOwners:Map<String, String>, emitter:CBodyEmitter):Void {
		final instances:Array<String> = [];
		addDeclarationHeaderDependencies(type, instances, emitter);
		for (instanceId in instances)
			addModuleDependencyForInstance(result, modulePath, instanceId, typeOwners);
	}

	static function addDeclarationHeaderDependencies(type:HxcIRTypeRef, dependencies:Array<String>, emitter:CBodyEmitter):Void {
		switch type {
			case IRTInstance(instanceId):
				if (!emitter.typeInstanceIsForwardDeclarable(instanceId) && dependencies.indexOf(instanceId) == -1)
					dependencies.push(instanceId);
			case IRTNullable(_, IRNTagged):
				final planId = emitter.optionalPlanId(type);
				if (dependencies.indexOf(planId) == -1)
					dependencies.push(planId);
			case IRTFixedArray(value, _, _):
				addDeclarationHeaderDependencies(value, dependencies, emitter);
			case IRTFunction(parameters, result):
				for (parameter in parameters)
					addDeclarationHeaderDependencies(parameter, dependencies, emitter);
				addDeclarationHeaderDependencies(result, dependencies, emitter);
			case IRTPointer(pointee, _) | IRTNullable(pointee, IRNPointer) | IRTSpan(pointee, _):
				// Struct tags remain soft; native enums still need their owner header.
				addDeclarationHeaderDependencies(pointee, dependencies, emitter);
			case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_) | IRTString | IRTCString | IRTVoid | IRTDynamic:
		}
	}

	static function addModuleDependencyForInstance(result:Map<String, Array<String>>, modulePath:String, instanceId:String,
			typeOwners:Map<String, String>):Void {
		final owner = typeOwners.get(instanceId);
		if (owner == null || owner == modulePath)
			return;
		final dependencies = requireModuleDependencyList(result, modulePath);
		if (dependencies.indexOf(owner) == -1)
			dependencies.push(owner);
	}

	static function requireModuleDependencyList(result:Map<String, Array<String>>, modulePath:String):Array<String> {
		var dependencies = result.get(modulePath);
		if (dependencies == null) {
			dependencies = [];
			result.set(modulePath, dependencies);
		}
		return dependencies;
	}

	static function dependencyOrderedModules(layout:CProjectLayoutPlan, dependencies:Map<String, Array<String>>):Array<CProjectModuleLayout> {
		final result:Array<CProjectModuleLayout> = [];
		final state:Map<String, Int> = [];
		for (module in layout.modules)
			visitModule(module.modulePath, layout, dependencies, state, result);
		return result;
	}

	static function visitModule(modulePath:String, layout:CProjectLayoutPlan, dependencies:Map<String, Array<String>>, state:Map<String, Int>,
			result:Array<CProjectModuleLayout>):Void {
		final current = state.get(modulePath);
		if (current == 2)
			return;
		if (current == 1)
			throw new ProjectEmissionError('complete-type header dependency cycle reaches `$modulePath`');
		state.set(modulePath, 1);
		final direct = dependencies.get(modulePath);
		if (direct == null)
			throw new ProjectEmissionError('complete-type dependency plan omitted module `$modulePath`');
		for (dependency in direct)
			visitModule(dependency, layout, dependencies, state, result);
		state.set(modulePath, 2);
		result.push(layout.module(modulePath));
	}

	static function completePackageDependencies(layout:CProjectLayoutPlan, moduleDependencies:Map<String, Array<String>>):Map<String, Array<String>> {
		final result:Map<String, Array<String>> = [];
		for (pack in layout.packages)
			result.set(pack.packagePath, []);
		for (module in layout.modules) {
			final direct = moduleDependencies.get(module.modulePath);
			if (direct == null)
				throw new ProjectEmissionError('package project lost complete-type dependencies for Haxe module `${module.modulePath}`');
			final owner = layout.packageForModule(module.modulePath);
			final grouped = result.get(owner.packagePath);
			if (grouped == null)
				throw new ProjectEmissionError('package project lost dependency set for Haxe package `${owner.packagePath}`');
			for (dependencyModulePath in direct) {
				final dependency = layout.packageForModule(dependencyModulePath);
				if (dependency.packagePath != owner.packagePath && grouped.indexOf(dependency.packagePath) == -1)
					grouped.push(dependency.packagePath);
			}
		}
		for (dependencies in result)
			dependencies.sort(compareStrings);
		return result;
	}

	static function dependencyOrderedPackages(layout:CProjectLayoutPlan, dependencies:Map<String, Array<String>>):Array<CProjectPackageLayout> {
		final result:Array<CProjectPackageLayout> = [];
		final state:Map<String, Int> = [];
		for (pack in layout.packages)
			visitPackage(pack.packagePath, layout, dependencies, state, result);
		return result;
	}

	static function visitPackage(packagePath:String, layout:CProjectLayoutPlan, dependencies:Map<String, Array<String>>, state:Map<String, Int>,
			result:Array<CProjectPackageLayout>):Void {
		final current = state.get(packagePath);
		if (current == 2)
			return;
		if (current == 1)
			throw new ProjectEmissionError('complete-type package dependency cycle reaches `$packagePath`');
		state.set(packagePath, 1);
		final direct = dependencies.get(packagePath);
		if (direct == null)
			throw new ProjectEmissionError('complete-type dependency plan omitted Haxe package `$packagePath`');
		for (dependency in direct)
			visitPackage(dependency, layout, dependencies, state, result);
		state.set(packagePath, 2);
		result.push(layout.packageLayout(packagePath));
	}

	static function modulePaths(lowered:CBodyLoweringResult):Array<String> {
		final paths:Array<String> = [];
		for (module in lowered.program.modules)
			if (paths.indexOf(module.id) == -1)
				paths.push(module.id);
		for (fn in lowered.functions)
			if (paths.indexOf(fn.modulePath) == -1)
				paths.push(fn.modulePath);
		for (global in lowered.globals)
			if (paths.indexOf(global.modulePath) == -1)
				paths.push(global.modulePath);
		paths.sort(compareStrings);
		return paths;
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

	static function compareHeaderPlans(left:CStaticFunctionHeaderPlan, right:CStaticFunctionHeaderPlan):Int {
		if (left.path == right.path)
			return 0;
		if (left.path == HEADER_PATH)
			return -1;
		if (right.path == HEADER_PATH)
			return 1;
		return compareStrings(left.path, right.path);
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);
}
#else
class CStaticFunctionProjectEmitter {
	public function new() {}
}
#end

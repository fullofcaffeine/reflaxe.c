package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Expr.Binop;
import haxe.macro.Expr.Unop;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CProfile;
import reflaxe.c.CompilationContext;
import reflaxe.c.CPhaseTiming;
import reflaxe.c.CPhaseTiming.CPhaseTimingId;
import reflaxe.c.ast.CAST;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.contract.TypedCContract.TypedCContractSnapshot;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRFixedArrayPolicy;
import reflaxe.c.ir.HxcIRFixedArrayPolicy.HxcIRFixedArrayStorageDecision;
import reflaxe.c.ir.HxcIRDiagnostic;
import reflaxe.c.ir.HxcIRValidator;
import reflaxe.c.ir.HxcUtf8;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.interop.CImportRegistry.CLoweredImports;
import reflaxe.c.interop.CImportRegistry.CPreparedImportFunction;
import reflaxe.c.interop.CImportRegistry.CPreparedImportType;
import reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.lowering.CBodyAggregate.CBodyAggregateRegistry;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueKind;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyAggregate.CLoweredBodyAggregate;
import reflaxe.c.lowering.CBodyAggregate.CPreparedBodyAggregate;
import reflaxe.c.lowering.CBodyAggregate.CPreparedBodyAggregateField;
import reflaxe.c.lowering.CBodyClass.CLoweredBodyClass;
import reflaxe.c.lowering.CBodyClass.CPreparedBodyClass;
import reflaxe.c.lowering.CBodyClass.CPreparedBodyClassField;
import reflaxe.c.lowering.CBodyConstructor.CBodyConstructorInput;
import reflaxe.c.lowering.CBodyConstructor.CLoweredBodyConstructor;
import reflaxe.c.lowering.CBodyDispatch.CBodyDispatchGraph;
import reflaxe.c.lowering.CBodyDispatch.CBodyDispatchCatalog;
import reflaxe.c.lowering.CBodyDispatch.CBodyDispatchPreparer;
import reflaxe.c.lowering.CBodyDispatch.CLoweredBodyDispatch;
import reflaxe.c.lowering.CBodyDispatch.CPreparedBodyDispatch;
import reflaxe.c.lowering.CBodyEnum.CLoweredBodyEnum;
import reflaxe.c.lowering.CBodyValueCoalescing.CBodyValueCoalescingPlanner;
import reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumCase;
import reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance;
import reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumPayload;
import reflaxe.c.lowering.CGenericSpecialization.CGenericCallResolver;
import reflaxe.c.lowering.CGenericSpecialization.CGenericFunctionSpecialization;
import reflaxe.c.lowering.CPrimitiveHelperEmitter.CPrimitiveHelperPlan;
import reflaxe.c.lowering.CPrimitiveHelperEmitter.CPrimitiveHelperSelection;
import reflaxe.c.semantics.CPrimitiveTypeMapper;
import reflaxe.c.semantics.CPrimitiveSemantics;
import reflaxe.c.semantics.CPrimitiveTypes;

/** Stable semantic coordinates plus one real pinned-compiler function expression. */
typedef CBodyFunctionInput = {
	final modulePath:String;
	final declarationPath:String;
	final sourcePath:String;
	final fieldName:String;
	final sourceOrder:Int;
	final fieldType:Type;
	final expression:TypedExpr;
	final ?typeParameters:Array<TypeParameter>;
	final ?specialization:CGenericFunctionSpecialization;
	final ?instanceOwner:Ref<ClassType>;
}

/** Captured primitive static-field facts; expressions must come from `filterTypes`. */
typedef CBodyGlobalInput = {
	final modulePath:String;
	final declarationPath:String;
	final sourcePath:String;
	final fieldName:String;
	final sourceOrder:Int;
	final fieldType:Type;
	final mutable:Bool;
	final position:Position;
	final expression:Null<TypedExpr>;
}

/** The semantic work performed by one compiler-owned initializer body. */
enum CBodyInitializerKind {
	CBIClass;
	CBIStaticField(globalId:String);
}

/** Stable coordinates plus one real typed initializer expression. */
typedef CBodyInitializerInput = {
	final id:String;
	final modulePath:String;
	final declarationPath:String;
	final sourcePath:String;
	final displayName:String;
	final sourceOrder:Int;
	final expression:TypedExpr;
	final kind:CBodyInitializerKind;
}

/** Choose whether a caller needs the diagnostic-only `#line` body copy. */
enum abstract CBodySourceMappingMode(Int) {
	var CBSMNormalOnly = 0;
	var CBSMNormalAndLineMapped = 1;
}

/** One validated body with finalized C names and an optional mapped copy. */
class CLoweredBodyFunction {
	public final modulePath:String;
	public final declarationPath:String;
	public final fieldName:String;
	public final ir:HxcIRFunction;
	public final cName:CIdentifier;
	public final parameterNames:Map<String, CIdentifier>;
	public final localNames:Map<String, CIdentifier>;
	public final spanLengthNames:Map<String, CIdentifier>;
	public final temporaryNames:Map<String, CIdentifier>;
	public final tailArgumentNames:Map<String, Array<CIdentifier>>;
	public final labelNames:Map<String, CIdentifier>;
	public final requiredHeaders:Array<String>;
	public final body:CStmt;
	public final lineMappedBody:Null<CStmt>;

	public function new(modulePath:String, declarationPath:String, fieldName:String, ir:HxcIRFunction, cName:CIdentifier,
			parameterNames:Map<String, CIdentifier>, localNames:Map<String, CIdentifier>, spanLengthNames:Map<String, CIdentifier>,
			temporaryNames:Map<String, CIdentifier>, tailArgumentNames:Map<String, Array<CIdentifier>>, labelNames:Map<String, CIdentifier>,
			requiredHeaders:Array<String>, body:CStmt, lineMappedBody:Null<CStmt>) {
		this.modulePath = modulePath;
		this.declarationPath = declarationPath;
		this.fieldName = fieldName;
		this.ir = ir;
		this.cName = cName;
		this.parameterNames = parameterNames;
		this.localNames = localNames;
		this.spanLengthNames = spanLengthNames;
		this.temporaryNames = temporaryNames;
		this.tailArgumentNames = tailArgumentNames;
		this.labelNames = labelNames;
		this.requiredHeaders = requiredHeaders.copy();
		this.body = body;
		this.lineMappedBody = lineMappedBody;
	}
}

/** One referenced primitive static field with a finalized internal C object name. */
class CLoweredBodyGlobal {
	public final modulePath:String;
	public final ir:HxcIRGlobal;
	public final cName:CIdentifier;

	public function new(modulePath:String, ir:HxcIRGlobal, cName:CIdentifier) {
		this.modulePath = modulePath;
		this.ir = ir;
		this.cName = cName;
	}
}

/** One source-rooted hosted-output requirement retained for planning/diagnostics. */
class CBodyRuntimeRequirement {
	public final featureId:String;
	public final operationId:String;
	public final surface:String;
	public final source:HxcSourceSpan;
	public final position:Position;

	public function new(featureId:String, operationId:String, surface:String, source:HxcSourceSpan, position:Position) {
		this.featureId = featureId;
		this.operationId = operationId;
		this.surface = surface;
		this.source = source;
		this.position = position;
	}
}

/** Complete deterministic result for the admitted body subset. */
class CBodyLoweringResult {
	public final program:HxcIRProgram;
	public final functions:Array<CLoweredBodyFunction>;
	public final globals:Array<CLoweredBodyGlobal>;
	public final aggregates:Array<CLoweredBodyAggregate>;
	public final enums:Array<CLoweredBodyEnum>;
	public final classes:Array<CLoweredBodyClass>;
	public final constructors:Array<CLoweredBodyConstructor>;
	public final dispatch:CLoweredBodyDispatch;
	public final imports:CLoweredImports;
	public final helpers:Array<CPrimitiveHelperPlan>;
	public final buildFacts:Array<TypedCBuildFact>;
	public final symbolTable:CSymbolTableSnapshot;
	public final boundsAbortName:Null<CIdentifier>;
	public final runtimeRequirements:Array<CBodyRuntimeRequirement>;

	public function new(program:HxcIRProgram, functions:Array<CLoweredBodyFunction>, globals:Array<CLoweredBodyGlobal>,
			aggregates:Array<CLoweredBodyAggregate>, enums:Array<CLoweredBodyEnum>, classes:Array<CLoweredBodyClass>,
			constructors:Array<CLoweredBodyConstructor>, dispatch:CLoweredBodyDispatch, imports:CLoweredImports, helpers:Array<CPrimitiveHelperPlan>,
			buildFacts:Array<TypedCBuildFact>, symbolTable:CSymbolTableSnapshot, boundsAbortName:Null<CIdentifier>,
			runtimeRequirements:Array<CBodyRuntimeRequirement>) {
		this.program = program;
		this.functions = functions.copy();
		this.globals = globals.copy();
		this.aggregates = aggregates.copy();
		this.enums = enums.copy();
		this.classes = classes.copy();
		this.constructors = constructors.copy();
		this.dispatch = dispatch;
		this.imports = imports;
		this.helpers = helpers.copy();
		this.buildFacts = buildFacts.copy();
		this.symbolTable = symbolTable;
		this.boundsAbortName = boundsAbortName;
		this.runtimeRequirements = runtimeRequirements.copy();
	}
}

/** Typed Haxe body -> HxcIR -> validated structural C body pipeline. */
class CBodyLowering {
	final context:CompilationContext;
	final sourceMappingMode:CBodySourceMappingMode;

	public function new(context:CompilationContext, ?sourceMappingMode:CBodySourceMappingMode) {
		this.context = context;
		this.sourceMappingMode = sourceMappingMode == null ? CBSMNormalOnly : sourceMappingMode;
	}

	public function lower(inputFunctions:Array<CBodyFunctionInput>, ?inputGlobals:Array<CBodyGlobalInput>, ?inputInitializers:Array<CBodyInitializerInput>,
			?inputConstructors:Array<CBodyConstructorInput>, ?inputDispatch:CBodyDispatchGraph, ?typedProgram:TypedProgramInput,
			?typedContract:TypedCContractSnapshot):CBodyLoweringResult {
		if (inputFunctions.length == 0) {
			throw new CBodyEmissionError("body lowering requires at least one typed function input");
		}
		// The profiler enables these request-local clocks explicitly. They describe
		// semantic boundaries rather than helper calls, so reports survive refactors.
		final hxcIRConstructionTimer = CPhaseTiming.start(CPHxcIRConstruction);
		final inputs = inputFunctions.copy();
		inputs.sort(compareInputs);
		final aggregateRegistry = new CBodyAggregateRegistry(context, typedProgram, typedContract);
		final prepared:Array<PreparedBodyFunction> = [];
		final preparedById:Map<String, PreparedBodyFunction> = [];
		for (input in inputs) {
			final fn = new FunctionPreparer(context, input, aggregateRegistry).prepare();
			if (preparedById.exists(fn.irId)) {
				throw new CBodyEmissionError('body lowering received duplicate semantic function `${fn.irId}`');
			}
			prepared.push(fn);
			preparedById.set(fn.irId, fn);
		}
		final preparedDispatch:CPreparedBodyDispatch = new CBodyDispatchPreparer(context, inputDispatch == null ? CBodyDispatchGraph.empty() : inputDispatch,
			aggregateRegistry).prepare();
		final constructorInputs = inputConstructors == null ? [] : inputConstructors.copy();
		constructorInputs.sort((left, right) -> compareUtf8(left.id, right.id));
		final constructorSignaturesById:Map<String, PreparedConstructorSignature> = [];
		for (input in constructorInputs) {
			final constructorPreparer = new ConstructorPreparer(context, input, aggregateRegistry);
			final signature = constructorPreparer.prepareSignature();
			if (constructorSignaturesById.exists(input.id)) {
				throw new CBodyEmissionError('body lowering received duplicate constructor `${input.id}`');
			}
			constructorSignaturesById.set(input.id, signature);
			if (!input.elided) {
				final fn = constructorPreparer.prepareFunction(signature);
				if (preparedById.exists(fn.irId)) {
					throw new CBodyEmissionError('body lowering received duplicate semantic function `${fn.irId}`');
				}
				prepared.push(fn);
				preparedById.set(fn.irId, fn);
			}
		}
		final initializers = inputInitializers == null ? [] : inputInitializers.copy();
		initializers.sort(compareInitializerInputs);
		final deferredInitializersByGlobal:Map<String, String> = [];
		for (input in initializers) {
			final fn = new InitializerPreparer(context, input).prepare();
			if (preparedById.exists(fn.irId)) {
				throw new CBodyEmissionError('body lowering received duplicate semantic function `${fn.irId}`');
			}
			switch input.kind {
				case CBIStaticField(globalId):
					if (deferredInitializersByGlobal.exists(globalId)) {
						throw new CBodyEmissionError('body lowering received duplicate initializer for global `$globalId`');
					}
					deferredInitializersByGlobal.set(globalId, input.id);
				case CBIClass:
			}
			prepared.push(fn);
			preparedById.set(fn.irId, fn);
		}
		final globalRegistry = new BodyGlobalRegistry(context, inputGlobals == null ? [] : inputGlobals, deferredInitializersByGlobal);
		final built:Array<BuiltBodyFunction> = [];
		for (fn in prepared) {
			built.push(new FunctionBuilder(context, fn, preparedById, constructorSignaturesById, globalRegistry, aggregateRegistry, preparedDispatch).build());
		}
		aggregateRegistry.completeClassLayouts();
		final preparedGlobals = globalRegistry.canonicalGlobals();
		final preparedAggregates = aggregateRegistry.canonicalAggregates();
		final preparedEnums = aggregateRegistry.canonicalEnums();
		final preparedClasses = aggregateRegistry.canonicalClasses();
		final preparedImports = aggregateRegistry.canonicalImports();
		final program = buildProgram(built, preparedGlobals, preparedAggregates, preparedEnums, preparedClasses, preparedImports, preparedDispatch);
		CPhaseTiming.stop(hxcIRConstructionTimer);
		final hxcIRValidationTimer = CPhaseTiming.start(CPHxcIRValidation);
		new HxcIRValidator().requireValid(program, Std.string(context.profile));
		CPhaseTiming.stop(hxcIRValidationTimer);
		final analysisTimer = CPhaseTiming.start(CPSemanticAnalysesAndNaming);
		final helperSelection = new CPrimitiveHelperSelection();
		helperSelection.collect(program);
		helperSelection.register(context.symbols);
		final boundsAbortRequest = registerBoundsAbort(program);
		final symbolTable = context.symbols.finalizeSymbols();
		final loweredAggregates = aggregateRegistry.finalize(context.symbols);
		final loweredEnums = aggregateRegistry.finalizeEnums(context.symbols);
		final loweredClasses = aggregateRegistry.finalizeClasses(context.symbols);
		final loweredDispatch = preparedDispatch.finalize(context.symbols);
		final loweredImports = aggregateRegistry.finalizeImports(context.symbols);
		final loweredConstructors:Array<CLoweredBodyConstructor> = [];
		for (input in constructorInputs) {
			final signature = constructorSignaturesById.get(input.id);
			if (signature == null)
				throw new CBodyEmissionError('constructor lowering lost signature `${input.id}`');
			final preparedConstructor = preparedById.get(input.id);
			final cName = preparedConstructor == null ? null : context.symbols.identifierFor(preparedConstructor.functionRequest);
			loweredConstructors.push(new CLoweredBodyConstructor(input.id, input.declarationPath, signature.classValue.instanceId, input.elided,
				input.canFail, cName));
		}
		final boundsAbortName = boundsAbortRequest == null ? null : context.symbols.identifierFor(boundsAbortRequest);
		final helpers = helperSelection.finalize(context.symbols);
		final helperNames:Map<String, CIdentifier> = [];
		for (helper in helpers) {
			helperNames.set(helper.helperId, helper.cName);
		}
		final functionNames:Map<String, CIdentifier> = [];
		for (item in built) {
			functionNames.set(item.ir.id, context.symbols.identifierFor(item.prepared.functionRequest));
		}
		final globalNames:Map<String, CIdentifier> = [];
		final loweredGlobals:Array<CLoweredBodyGlobal> = [];
		for (global in preparedGlobals) {
			final cName = context.symbols.identifierFor(global.request);
			globalNames.set(global.ir.id, cName);
			loweredGlobals.push(new CLoweredBodyGlobal(global.modulePath, global.ir, cName));
		}
		CPhaseTiming.stop(analysisTimer);
		final castBodyTimer = CPhaseTiming.start(CPCASTBodyConstruction);
		final emitter = new CBodyEmitter(loweredAggregates, loweredEnums, loweredClasses, loweredDispatch, loweredImports);
		final lowered:Array<CLoweredBodyFunction> = [];
		for (item in built) {
			final parameterNames:Map<String, CIdentifier> = [];
			for (parameterId => request in item.prepared.parameterRequests) {
				parameterNames.set(parameterId, context.symbols.identifierFor(request));
			}
			final localNames:Map<String, CIdentifier> = [];
			for (localId => request in item.localRequests) {
				localNames.set(localId, context.symbols.identifierFor(request));
			}
			final spanLengthNames:Map<String, CIdentifier> = [];
			for (localId => request in item.spanLengthRequests) {
				spanLengthNames.set(localId, context.symbols.identifierFor(request));
			}
			final temporaryNames:Map<String, CIdentifier> = [];
			for (valueId => request in item.temporaryRequests) {
				temporaryNames.set(valueId, context.symbols.identifierFor(request));
			}
			final tailArgumentNames:Map<String, Array<CIdentifier>> = [];
			for (instructionId => requests in item.tailArgumentRequests) {
				tailArgumentNames.set(instructionId, requests.map(request -> context.symbols.identifierFor(request)));
			}
			final labelNames:Map<String, CIdentifier> = [];
			for (blockId => request in item.labelRequests) {
				labelNames.set(blockId, context.symbols.identifierFor(request));
			}
			final preparedFunction = item.prepared;
			final body = emitter.emitBody(item.ir, parameterNames, localNames, temporaryNames, functionNames, globalNames, helperNames, false,
				tailArgumentNames, labelNames, null, spanLengthNames, boundsAbortName);
			final lineMappedBody = sourceMappingMode == CBSMNormalAndLineMapped ? emitter.emitBody(item.ir, parameterNames, localNames, temporaryNames,
				functionNames, globalNames, helperNames, true, tailArgumentNames, labelNames, null, spanLengthNames, boundsAbortName) : null;
			lowered.push(new CLoweredBodyFunction(preparedFunction.modulePath, preparedFunction.declarationPath, preparedFunction.displayName, item.ir,
				context.symbols.identifierFor(item.prepared.functionRequest), parameterNames, localNames, spanLengthNames, temporaryNames, tailArgumentNames,
				labelNames, emitter.requiredHeaders(item.ir), body, lineMappedBody));
		}
		lowered.sort((left, right) -> compareUtf8(left.ir.id, right.ir.id));
		CPhaseTiming.stop(castBodyTimer);
		final runtimeRequirements:Array<CBodyRuntimeRequirement> = [];
		for (item in built) {
			for (requirement in item.runtimeRequirements) {
				runtimeRequirements.push(requirement);
			}
		}
		runtimeRequirements.sort(compareRuntimeRequirements);
		return new CBodyLoweringResult(program, lowered, loweredGlobals, loweredAggregates, loweredEnums, loweredClasses, loweredConstructors,
			loweredDispatch, loweredImports, helpers, helperSelection.buildFacts().concat(loweredImports.buildFacts), symbolTable, boundsAbortName,
			runtimeRequirements);
	}

	function registerBoundsAbort(program:HxcIRProgram):Null<CSymbolRequest> {
		for (module in program.modules) {
			for (fn in module.functions) {
				for (block in fn.blocks) {
					for (instruction in block.instructions) {
						switch instruction.kind {
							case IRIOBoundsCheck(_, _, IRBPCheckedAbort(_, _)) | IRIOProjectTag(_, _, _, IRTCPCheckedAbort(_, _)) |
								IRIONullCheck(_, IRNCPCheckedAbort(_, _)):
								final request = new CSymbolRequest(CSKMethod, ["c-standard-library", "abort"], CNSOrdinary("translation-unit"), CSVExternal,
									"abort");
								context.symbols.register(request);
								return request;
							case IRIOCall({failure: {target: IRFTAbort}}):
								final request = new CSymbolRequest(CSKMethod, ["c-standard-library", "abort"], CNSOrdinary("translation-unit"), CSVExternal,
									"abort");
								context.symbols.register(request);
								return request;
							case _:
						}
					}
					if (block.terminator != null) {
						switch block.terminator.kind {
							case IRTThrow(_, {target: IRFTAbort}):
								final request = new CSymbolRequest(CSKMethod, ["c-standard-library", "abort"], CNSOrdinary("translation-unit"), CSVExternal,
									"abort");
								context.symbols.register(request);
								return request;
							case _:
						}
					}
				}
			}
		}
		return null;
	}

	static function buildProgram(functions:Array<BuiltBodyFunction>, globals:Array<PreparedBodyGlobal>, aggregates:Array<CPreparedBodyAggregate>,
			enums:Array<CPreparedBodyEnumInstance>, classes:Array<CPreparedBodyClass>, imports:Array<CPreparedImportType>,
			dispatch:CPreparedBodyDispatch):HxcIRProgram {
		final byModule:Map<String, Array<BuiltBodyFunction>> = [];
		for (fn in functions) {
			var moduleFunctions = byModule.get(fn.prepared.modulePath);
			if (moduleFunctions == null) {
				moduleFunctions = [];
				byModule.set(fn.prepared.modulePath, moduleFunctions);
			}
			moduleFunctions.push(fn);
		}
		final globalsByModule:Map<String, Array<PreparedBodyGlobal>> = [];
		for (global in globals) {
			var moduleGlobals = globalsByModule.get(global.modulePath);
			if (moduleGlobals == null) {
				moduleGlobals = [];
				globalsByModule.set(global.modulePath, moduleGlobals);
			}
			moduleGlobals.push(global);
		}
		final aggregatesByModule:Map<String, Array<CPreparedBodyAggregate>> = [];
		for (aggregate in aggregates) {
			var moduleAggregates = aggregatesByModule.get(aggregate.ownerModule);
			if (moduleAggregates == null) {
				moduleAggregates = [];
				aggregatesByModule.set(aggregate.ownerModule, moduleAggregates);
			}
			moduleAggregates.push(aggregate);
		}
		final enumsByModule:Map<String, Array<CPreparedBodyEnumInstance>> = [];
		for (value in enums) {
			var moduleEnums = enumsByModule.get(value.ownerModule);
			if (moduleEnums == null) {
				moduleEnums = [];
				enumsByModule.set(value.ownerModule, moduleEnums);
			}
			moduleEnums.push(value);
		}
		final classesByModule:Map<String, Array<CPreparedBodyClass>> = [];
		for (value in classes) {
			var moduleClasses = classesByModule.get(value.ownerModule);
			if (moduleClasses == null) {
				moduleClasses = [];
				classesByModule.set(value.ownerModule, moduleClasses);
			}
			moduleClasses.push(value);
		}
		final importsByModule:Map<String, Array<CPreparedImportType>> = [];
		for (value in imports) {
			var moduleImports = importsByModule.get(value.ownerModule);
			if (moduleImports == null) {
				moduleImports = [];
				importsByModule.set(value.ownerModule, moduleImports);
			}
			moduleImports.push(value);
		}
		final moduleIdSet:Map<String, Bool> = [];
		for (moduleId in byModule.keys()) {
			moduleIdSet.set(moduleId, true);
		}
		for (moduleId in globalsByModule.keys()) {
			moduleIdSet.set(moduleId, true);
		}
		for (moduleId in aggregatesByModule.keys()) {
			moduleIdSet.set(moduleId, true);
		}
		for (moduleId in enumsByModule.keys()) {
			moduleIdSet.set(moduleId, true);
		}
		for (moduleId in classesByModule.keys()) {
			moduleIdSet.set(moduleId, true);
		}
		for (moduleId in importsByModule.keys()) {
			moduleIdSet.set(moduleId, true);
		}
		final moduleIds = [for (moduleId in moduleIdSet.keys()) moduleId];
		moduleIds.sort(compareUtf8);
		final modules:Array<HxcIRModule> = [];
		for (moduleId in moduleIds) {
			final entries = byModule.get(moduleId);
			final moduleFunctions = entries == null ? [] : entries;
			moduleFunctions.sort((left, right) -> compareUtf8(left.ir.id, right.ir.id));
			final globalEntries = globalsByModule.get(moduleId);
			final moduleGlobals = globalEntries == null ? [] : globalEntries;
			moduleGlobals.sort((left, right) -> compareUtf8(left.ir.id, right.ir.id));
			final aggregateEntries = aggregatesByModule.get(moduleId);
			final moduleAggregates = aggregateEntries == null ? [] : aggregateEntries;
			moduleAggregates.sort((left, right) -> compareUtf8(left.declarationId, right.declarationId));
			final enumEntries = enumsByModule.get(moduleId);
			final moduleEnums = enumEntries == null ? [] : enumEntries;
			moduleEnums.sort((left, right) -> compareUtf8(left.declarationId, right.declarationId));
			final classEntries = classesByModule.get(moduleId);
			final moduleClasses = classEntries == null ? [] : classEntries;
			moduleClasses.sort((left, right) -> compareUtf8(left.declarationId, right.declarationId));
			final importEntries = importsByModule.get(moduleId);
			final moduleImports = importEntries == null ? [] : importEntries;
			moduleImports.sort((left, right) -> compareUtf8(left.declarationId, right.declarationId));
			final spans = moduleFunctions.map(entry -> entry.ir.source)
				.concat(moduleGlobals.map(global -> global.ir.source))
				.concat(moduleAggregates.map(aggregate -> aggregate.source))
				.concat(moduleEnums.map(value -> value.source))
				.concat(moduleClasses.map(value -> value.source))
				.concat(moduleImports.map(value -> value.source));
			if (spans.length == 0) {
				throw new CBodyEmissionError('body lowering lost module `$moduleId` while building HxcIR');
			}
			modules.push({
				id: moduleId,
				types: moduleAggregates.map(aggregate -> aggregate.declaration())
					.concat(moduleEnums.map(value -> value.declaration()))
					.concat(moduleClasses.map(value -> value.declaration()))
					.concat(moduleImports.map(value -> value.declaration())),
				typeInstances: moduleAggregates.map(aggregate -> aggregate.instance())
					.concat(moduleEnums.map(value -> value.instance()))
					.concat(moduleClasses.map(value -> value.instance()))
					.concat(moduleImports.map(value -> value.instance())),
				globals: moduleGlobals.map(global -> global.ir),
				functions: moduleFunctions.map(entry -> entry.ir),
				source: enclosingSpan(spans)
			});
		}
		return {schemaVersion: HxcIRValidator.SCHEMA_VERSION, dispatch: dispatch.ir(), modules: modules};
	}

	static function enclosingSpan(spans:Array<HxcSourceSpan>):HxcSourceSpan {
		if (spans.length == 0) {
			throw new CBodyEmissionError("cannot construct a module span from no function spans");
		}
		final file = spans[0].file;
		var startLine = spans[0].startLine;
		var startColumn = spans[0].startColumn;
		var endLine = spans[0].endLine;
		var endColumn = spans[0].endColumn;
		for (span in spans) {
			if (span.file != file) {
				throw new CBodyEmissionError('logical HxcIR module crosses source files `$file` and `${span.file}`');
			}
			if (span.startLine < startLine || span.startLine == startLine && span.startColumn < startColumn) {
				startLine = span.startLine;
				startColumn = span.startColumn;
			}
			if (span.endLine > endLine || span.endLine == endLine && span.endColumn > endColumn) {
				endLine = span.endLine;
				endColumn = span.endColumn;
			}
		}
		return new HxcSourceSpan(file, startLine, startColumn, endLine, endColumn);
	}

	static function compareInputs(left:CBodyFunctionInput, right:CBodyFunctionInput):Int {
		final identity = compareUtf8(functionInputId(left), functionInputId(right));
		return identity != 0 ? identity : left.sourceOrder - right.sourceOrder;
	}

	static function compareInitializerInputs(left:CBodyInitializerInput, right:CBodyInitializerInput):Int {
		final identity = compareUtf8(left.id, right.id);
		return identity != 0 ? identity : left.sourceOrder - right.sourceOrder;
	}

	static function compareRuntimeRequirements(left:CBodyRuntimeRequirement, right:CBodyRuntimeRequirement):Int {
		final semantic = compareUtf8('${left.featureId}\x00${left.operationId}\x00${left.surface}\x00${left.source.display()}',
			'${right.featureId}\x00${right.operationId}\x00${right.surface}\x00${right.source.display()}');
		return semantic;
	}

	public static function functionId(declarationPath:String, fieldName:String):String
		return 'function.$declarationPath.$fieldName';

	public static function methodId(declarationPath:String, fieldName:String):String
		return 'method.$declarationPath.$fieldName';

	public static function functionInputId(input:CBodyFunctionInput):String
		return input.instanceOwner != null ? methodId(input.declarationPath,
			input.fieldName) : input.specialization == null ? functionId(input.declarationPath, input.fieldName) : input.specialization.instanceId;

	public static function functionInputDisplayName(input:CBodyFunctionInput):String
		return input.specialization == null ? input.fieldName : input.specialization.displayName;

	public static function applyFunctionType(input:CBodyFunctionInput, type:Type):Type
		return input.specialization == null ? type : input.specialization.apply(type);

	public static function globalId(declarationPath:String, fieldName:String):String
		return 'global.$declarationPath.$fieldName';

	public static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}
}

private typedef BuiltBodyFunction = {
	final prepared:PreparedBodyFunction;
	final ir:HxcIRFunction;
	final localRequests:Map<String, CSymbolRequest>;
	final spanLengthRequests:Map<String, CSymbolRequest>;
	final temporaryRequests:Map<String, CSymbolRequest>;
	final tailArgumentRequests:Map<String, Array<CSymbolRequest>>;
	final labelRequests:Map<String, CSymbolRequest>;
	final runtimeRequirements:Array<CBodyRuntimeRequirement>;
}

private typedef BodyConstructedObject = {
	final backingLocalId:String;
	final partialActionId:String;
	final initializedActionId:String;
	final source:HxcSourceSpan;
}

private enum BodyCollectionKind {
	BCKFixedArray(witnessId:String);
	BCKSpan(mutable:Bool);
}

private enum BodyFixedArrayInitializer {
	BFAIValues(values:Array<String>);
	BFAIZero;
}

private typedef BodyCollectionType = {
	final kind:BodyCollectionKind;
	final element:CPrimitiveTypeMapping;
}

private typedef BodyCollectionBinding = {
	final localId:String;
	final kind:BodyCollectionKind;
	final element:CPrimitiveTypeMapping;
	final length:Null<Int>;
}

private typedef SpanLoopPattern = {
	final iteratorCompilerId:Int;
	final loopVariable:TVar;
	final span:BodyCollectionBinding;
	final length:Int;
	final body:Array<TypedExpr>;
	final sourceExpression:TypedExpr;
}

private typedef LoweredValue = {
	final id:String;
	final type:HxcIRTypeRef;
	final mapping:CBodyValueType;
}

private typedef LoweredPlace = {
	final place:HxcIRPlace;
	final mapping:CBodyValueType;
	final mutable:Bool;
}

private enum UIntIntrinsicResult {
	UIIntrinsicLowered(value:LoweredValue);
	UIIntrinsicNotMatched;
}

private enum IntegerConversionMode {
	ICExact;
	ICModulo;
}

private enum Float32ConversionMode {
	FCNarrow;
	FCWiden;
}

private typedef MutableBodyBlock = {
	final id:String;
	final source:HxcSourceSpan;
	final instructions:Array<HxcIRInstruction>;
	final generatedOrdinal:Null<Int>;
	final generatedRole:Null<String>;
	var active:Bool;
	var terminator:Null<HxcIRTerminator>;
}

private typedef LoopControlTargets = {
	final breakTargetBlockId:String;
	final continueTargetBlockId:String;
	var usedBreak:Bool;
	var usedContinue:Bool;
}

private typedef TypedSwitchArm = {
	final values:Array<TypedExpr>;
	final expr:TypedExpr;
}

private typedef EnumConstructorAccess = {
	final reference:Ref<EnumType>;
	final field:EnumField;
}

private typedef BodyNewExpression = {
	final classReference:Ref<ClassType>;
	final parameters:Array<Type>;
	final arguments:Array<TypedExpr>;
}

private typedef PreparedParameter = {
	final compilerId:Int;
	final ir:HxcIRParameter;
	final mapping:CBodyValueType;
}

private typedef PreparedConstructorSignature = {
	final input:CBodyConstructorInput;
	final classValue:CPreparedBodyClass;
	final selfMapping:CBodyValueType;
	final arguments:Array<PreparedParameter>;
}

private typedef PreparedBodyFunction = {
	final modulePath:String;
	final declarationPath:String;
	final sourcePath:String;
	final displayName:String;
	final fieldName:String;
	final specialization:Null<CGenericFunctionSpecialization>;
	final sourceExpression:TypedExpr;
	final bodyExpression:TypedExpr;
	final role:PreparedBodyRole;
	final irId:String;
	final parameters:Array<PreparedParameter>;
	final returnMapping:CBodyValueType;
	final functionRequest:CSymbolRequest;
	final parameterRequests:Map<String, CSymbolRequest>;
}

private enum PreparedBodyRole {
	PBRFunction;
	PBRConstructor(signature:PreparedConstructorSignature);
	PBRClassInitializer;
	PBRStaticFieldInitializer(globalId:String);
}

private typedef PreparedBodyGlobal = {
	final modulePath:String;
	final declarationPath:String;
	final fieldName:String;
	final mapping:CPrimitiveTypeMapping;
	final ir:HxcIRGlobal;
	final request:CSymbolRequest;
}

/** Request-local registry for referenced primitive static fields. */
private class BodyGlobalRegistry {
	final context:CompilationContext;
	final byId:Map<String, PreparedBodyGlobal> = [];
	final inputsById:Map<String, CBodyGlobalInput> = [];
	final deferredInitializersByGlobal:Map<String, String>;

	public function new(context:CompilationContext, inputs:Array<CBodyGlobalInput>, deferredInitializersByGlobal:Map<String, String>) {
		this.context = context;
		this.deferredInitializersByGlobal = deferredInitializersByGlobal;
		for (input in inputs) {
			final id = globalId(input.declarationPath, input.fieldName);
			if (inputsById.exists(id)) {
				throw new CBodyEmissionError('body lowering received duplicate captured global `$id`');
			}
			inputsById.set(id, input);
		}
	}

	public function require(classReference:Ref<ClassType>, fieldReference:Ref<ClassField>, expression:TypedExpr,
			fail:(Position, String) -> Void):PreparedBodyGlobal {
		final owner = classReference.get();
		final field = fieldReference.get();
		final declarationPath = owner.pack.concat([owner.name]).join(".");
		final id = globalId(declarationPath, field.name);
		return requireId(id, expression, fail);
	}

	public function requireId(id:String, expression:TypedExpr, fail:(Position, String) -> Void):PreparedBodyGlobal {
		final existing = byId.get(id);
		if (existing != null) {
			return existing;
		}
		final input = inputsById.get(id);
		if (input == null) {
			return rejected(fail, expression.pos, 'TField(static:$id:outside-captured-program)');
		}
		final fieldName = input.fieldName;
		final mapping = switch CPrimitiveTypeMapper.map(input.fieldType, context.profile) {
			case CTPrimitive(value):
				final admitted = value.nullability == CPNonNullable && switch value.sourceType {
					case CPHaxeBool | CPHaxeInt | CPHaxeUInt | CPHaxeFloat: true;
					case _: false;
				};
				if (!admitted) {
					return rejected(fail, expression.pos, 'TField(static:$fieldName:type:${value.cSpelling})');
				}
				value;
			case CTReference(identity, nullable):
				return rejected(fail, expression.pos, 'TField(static:$fieldName:reference-$identity-${nullable ? "nullable" : "non-null"})');
			case CTNativePointer(identity, nullable):
				return rejected(fail, expression.pos, 'TField(static:$fieldName:native-pointer-$identity-${nullable ? "nullable" : "non-null"})');
			case CTUnsupported(reason):
				return rejected(fail, expression.pos, 'TField(static:$fieldName:$reason)');
		};
		final deferredInitializerId = deferredInitializersByGlobal.get(id);
		final initializer = if (deferredInitializerId != null) {
			IRGIDeferred(deferredInitializerId);
		} else if (input.expression == null) {
			return rejected(fail, input.position, 'TField(static:$fieldName:uninitialized)');
		} else {
			globalInitializer(input.expression, mapping.irType, fail, fieldName);
		}
		final declarationPath = input.declarationPath;
		final request = new CSymbolRequest(CSKField, declarationPath.split(".").concat([fieldName]), CNSOrdinary("translation-unit"), CSVInternal);
		context.symbols.register(request);
		final prepared:PreparedBodyGlobal = {
			modulePath: input.modulePath,
			declarationPath: declarationPath,
			fieldName: fieldName,
			mapping: mapping,
			ir: {
				id: id,
				type: mapping.irType,
				mutable: input.mutable,
				initialization: initializer,
				source: HaxeSourceSpan.fromPosition(input.position, input.sourcePath)
			},
			request: request
		};
		byId.set(id, prepared);
		return prepared;
	}

	public function canonicalGlobals():Array<PreparedBodyGlobal> {
		final result = [for (global in byId) global];
		result.sort((left, right) -> CBodyLowering.compareUtf8(left.ir.id, right.ir.id));
		return result;
	}

	public static function globalId(declarationPath:String, fieldName:String):String
		return CBodyLowering.globalId(declarationPath, fieldName);

	static function globalInitializer(expression:TypedExpr, type:HxcIRTypeRef, fail:(Position, String) -> Void, fieldName:String):HxcIRGlobalInitialization {
		return switch expression.expr {
			case TConst(constant): IRGIConstant(globalConstant(constant, type, expression.pos, fail, fieldName));
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): globalInitializer(inner, type, fail, fieldName);
			case _: rejected(fail, expression.pos, 'TField(static:${fieldName}:non-constant-initializer)');
		};
	}

	static function globalConstant(constant:TConstant, type:HxcIRTypeRef, position:Position, fail:(Position, String) -> Void, fieldName:String):HxcIRConstant {
		return switch constant {
			case TInt(value):
				switch type {
					case IRTInt(_, _): IRCInt(Std.string(value));
					case _: return rejected(fail, position, 'TField(static:${fieldName}:integer-initializer-type-mismatch)');
				}
			case TBool(value):
				type == IRTBool ? IRCBool(value) : return rejected(fail, position, 'TField(static:${fieldName}:boolean-initializer-type-mismatch)');
			case TFloat(_):
				return rejected(fail, position, 'TField(static:${fieldName}:floating-initializer-not-yet-canonicalized)');
			case TString(_) | TNull | TThis | TSuper:
				return rejected(fail, position, 'TField(static:${fieldName}:unsupported-initializer)');
		};
	}

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("body-global rejection callback returned unexpectedly");
	}
}

private class FunctionPreparer {
	final context:CompilationContext;
	final input:CBodyFunctionInput;
	final aggregateRegistry:CBodyAggregateRegistry;
	final functionContext:String;

	public function new(context:CompilationContext, input:CBodyFunctionInput, aggregateRegistry:CBodyAggregateRegistry) {
		this.context = context;
		this.input = input;
		this.aggregateRegistry = aggregateRegistry;
		this.functionContext = 'function ${input.declarationPath}.${input.fieldName} signature';
	}

	public function prepare():PreparedBodyFunction {
		final functionValue = switch input.expression.expr {
			case TFunction(value): value;
			case _: unsupported(input.expression.pos, FunctionBuilder.nodeName(input.expression));
		};
		final declaredSignature = switch TypeTools.follow(CBodyLowering.applyFunctionType(input, input.fieldType)) {
			case TFun(arguments, result): {arguments: arguments, result: result};
			case _: unsupported(input.expression.pos, "TFunction(field-type-not-function)");
		};
		final declaredArguments = declaredSignature.arguments;
		if (declaredArguments.length != functionValue.args.length) {
			unsupported(input.expression.pos,
				'TFunction(signature-argument-count=${declaredArguments.length},body-argument-count=${functionValue.args.length})');
		}

		final parameters:Array<PreparedParameter> = [];
		final parameterRequests:Map<String, CSymbolRequest> = [];
		for (index in 0...functionValue.args.length) {
			final argument = functionValue.args[index];
			final declared = declaredArguments[index];
			if (isRestType(argument.v.t)) {
				unsupported(input.expression.pos, 'TFunction(rest-argument:${argument.v.name})');
			}
			if (argument.value != null) {
				final optionalNull = declared.opt && switch argument.value.expr {
					case TConst(TNull): true;
					case _: false;
				};
				unsupported(input.expression.pos, 'TFunction(${optionalNull ? "optional" : "default"}-argument:${argument.v.name})');
			}
			if (declared.opt) {
				unsupported(input.expression.pos, 'TFunction(optional-argument:${argument.v.name})');
			}
			final mapping = admittedValueType(declared.t, input.expression.pos, 'TFunction(argument:${argument.v.name})');
			if (mapping.irType == IRTVoid) {
				unsupported(input.expression.pos, 'TFunction(argument:${argument.v.name}:Void)');
			}
			if (mapping.spanElement() != null) {
				if (input.instanceOwner != null) {
					unsupported(input.expression.pos, 'TFunction(argument:${argument.v.name}:borrowed-span-requires-static-function)');
				}
				if (input.specialization != null) {
					unsupported(input.expression.pos, 'TFunction(argument:${argument.v.name}:borrowed-span-generic-specialization-not-admitted)');
				}
			}
			final enumArgument = mapping.enumValue();
			if (enumArgument != null && enumArgument.scopedLifetime) {
				unsupported(input.expression.pos, 'TFunction(argument:${argument.v.name}:recursive-enum-requires-escape-analysis)');
			}
			final parameterId = 'parameter.$index';
			final source = HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath);
			parameters.push({
				compilerId: argument.v.id,
				ir: {id: parameterId, type: mapping.irType, source: source},
				mapping: mapping
			});
		}
		if (isBorrowedSpanType(declaredSignature.result)) {
			unsupported(input.expression.pos, "TFunction(return-type:borrowed-span-escape)");
		}
		final returnMapping = admittedValueType(declaredSignature.result, input.expression.pos, "TFunction(return-type)");
		final returnEnum = returnMapping.enumValue();
		if (returnEnum != null && returnEnum.scopedLifetime) {
			unsupported(input.expression.pos, "TFunction(return-type:recursive-enum-requires-escape-analysis)");
		}
		final instanceOwner = input.instanceOwner;
		var selfParameter:Null<PreparedParameter> = null;
		if (instanceOwner != null) {
			if (instanceOwner.get().params.length != 0)
				unsupported(input.expression.pos, 'TFunction(instance-owner-generic:${input.declarationPath})');
			final selfType = admittedValueType(TInst(instanceOwner, []), input.expression.pos, "TFunction(instance-self-type)");
			final selfClass = selfType.classValue();
			if (selfClass == null)
				unsupported(input.expression.pos, "TFunction(instance-self-not-concrete-class)");
			final selfMapping = CBodyValueType.classReference(selfClass, true);
			selfParameter = {
				compilerId: -1,
				ir: {id: "parameter.self", type: selfMapping.irType, source: HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath)},
				mapping: selfMapping
			};
		}
		final signatureParameters = parameters.copy();
		if (selfParameter != null)
			signatureParameters.unshift(selfParameter);
		final overloadSignature = signatureParameters.length == 0 ? [] : signatureParameters.map(parameter -> valueTypeKey(parameter.ir.type));
		final specializationArguments = input.specialization == null ? [] : input.specialization.arguments.map(argument -> argument.key);
		final functionRequest = new CSymbolRequest(CSKMethod, input.declarationPath.split(".").concat([input.fieldName]), CNSOrdinary("translation-unit"),
			CSVInternal, null, overloadSignature, specializationArguments);
		context.symbols.register(functionRequest);
		if (selfParameter != null) {
			final request = new CSymbolRequest(CSKLocal, input.declarationPath.split(".").concat([input.fieldName, "self"]),
				CNSOrdinary(functionRequest.stableKey()), CSVInternal, null, [], [], 0);
			context.symbols.register(request);
			parameterRequests.set(selfParameter.ir.id, request);
		}
		for (index in 0...parameters.length) {
			final parameter = parameters[index];
			final argument = functionValue.args[index];
			final request = new CSymbolRequest(CSKLocal, input.declarationPath.split(".").concat([input.fieldName, argument.v.name]),
				CNSOrdinary(functionRequest.stableKey()), CSVInternal, null, [], [], index + (selfParameter == null ? 0 : 1));
			context.symbols.register(request);
			parameterRequests.set(parameter.ir.id, request);
		}
		return {
			modulePath: input.modulePath,
			declarationPath: input.declarationPath,
			sourcePath: input.sourcePath,
			displayName: CBodyLowering.functionInputDisplayName(input),
			fieldName: input.fieldName,
			specialization: input.specialization,
			sourceExpression: input.expression,
			bodyExpression: functionValue.expr,
			role: PBRFunction,
			irId: CBodyLowering.functionInputId(input),
			parameters: signatureParameters,
			returnMapping: returnMapping,
			functionRequest: functionRequest,
			parameterRequests: parameterRequests
		};
	}

	function admittedValueType(type:Type, position:Position, node:String):CBodyValueType
		return aggregateRegistry.valueType(type, position, input.modulePath, input.sourcePath, reject, node);

	function reject(position:Position, node:String):Void
		unsupported(position, node);

	function unsupported<T>(position:Position, node:String):T {
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile), node, functionContext, source), position);
	}

	static function isRestType(type:Type):Bool {
		return switch type {
			case TAbstract(reference, _): final abstractType = reference.get(); abstractType.name == "Rest" && (abstractType.pack.join(".") == "haxe"
					|| abstractType.pack.join(".") == "haxe.extern");
			case TType(reference, parameters):
				final definition = reference.get();
				if (definition.name == "Rest" && definition.pack.join(".") == "haxe.extern") {
					true;
				} else {
					isRestType(TypeTools.applyTypeParameters(definition.type, definition.params, parameters));
				}
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? false : isRestType(resolved);
			case TLazy(resolve): isRestType(resolve());
			case _: false;
		};
	}

	static function isBorrowedSpanType(type:Type):Bool {
		return switch type {
			case TAbstract(reference, _): final abstractType = reference.get(); abstractType.pack.join(".") == "c" && (abstractType.name == "Span"
					|| abstractType.name == "ConstSpan");
			case TType(reference, parameters):
				final definition = reference.get();
				isBorrowedSpanType(TypeTools.applyTypeParameters(definition.type, definition.params, parameters));
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? false : isBorrowedSpanType(resolved);
			case TLazy(resolve): isBorrowedSpanType(resolve());
			case _: false;
		};
	}

	static function valueTypeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTFloat(width): 'f$width';
			case IRTSpan(element, mutable): 'span:${mutable ? "mutable" : "const"}<${valueTypeKey(element)}>';
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(IRTInstance(instanceId), nullable): 'class-reference:${nullable ? "nullable" : "nonnull"}:$instanceId';
			case _: throw new CBodyEmissionError("function signature contains a non-admitted body type");
		};
	}
}

private class ConstructorPreparer {
	final context:CompilationContext;
	final input:CBodyConstructorInput;
	final aggregateRegistry:CBodyAggregateRegistry;
	final functionContext:String;

	public function new(context:CompilationContext, input:CBodyConstructorInput, aggregateRegistry:CBodyAggregateRegistry) {
		this.context = context;
		this.input = input;
		this.aggregateRegistry = aggregateRegistry;
		this.functionContext = 'constructor ${input.declarationPath} signature';
	}

	public function prepareSignature():PreparedConstructorSignature {
		final functionValue = switch input.expression.expr {
			case TFunction(value): value;
			case _: unsupported(input.expression.pos, FunctionBuilder.nodeName(input.expression));
		};
		final declaredSignature = switch TypeTools.follow(input.fieldType) {
			case TFun(arguments, result): {arguments: arguments, result: result};
			case _: unsupported(input.expression.pos, "TFunction(constructor-field-type-not-function)");
		};
		if (declaredSignature.arguments.length != functionValue.args.length) {
			unsupported(input.expression.pos,
				'TFunction(constructor-signature-argument-count=${declaredSignature.arguments.length},body-argument-count=${functionValue.args.length})');
		}
		final returnMapping = admittedValueType(declaredSignature.result, input.expression.pos, "TFunction(constructor-return-type)");
		if (returnMapping.irType != IRTVoid)
			unsupported(input.expression.pos, "TFunction(constructor-return-type-not-Void)");

		final classMapping = admittedValueType(TInst(input.classReference, []), input.expression.pos, "TFunction(constructor-owner-type)");
		final classValue = classMapping.classValue();
		if (classValue == null)
			return unsupported(input.expression.pos, "TFunction(constructor-owner-not-concrete-class)");
		final selfMapping = CBodyValueType.classReference(classValue, false);
		final arguments:Array<PreparedParameter> = [];
		for (index in 0...functionValue.args.length) {
			final argument = functionValue.args[index];
			final declared = declaredSignature.arguments[index];
			if (isRestType(argument.v.t))
				unsupported(input.expression.pos, 'TFunction(constructor-rest-argument:${argument.v.name})');
			if (argument.value != null || declared.opt)
				unsupported(input.expression.pos, 'TFunction(constructor-optional-or-default-argument:${argument.v.name})');
			final mapping = admittedValueType(declared.t, input.expression.pos, 'TFunction(constructor-argument:${argument.v.name})');
			if (mapping.irType == IRTVoid)
				unsupported(input.expression.pos, 'TFunction(constructor-argument:${argument.v.name}:Void)');
			if (mapping.spanElement() != null)
				unsupported(input.expression.pos, 'TFunction(constructor-argument:${argument.v.name}:borrowed-span-constructor-not-admitted)');
			arguments.push({
				compilerId: argument.v.id,
				ir: {
					id: 'parameter.$index',
					type: mapping.irType,
					source: HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath)
				},
				mapping: mapping
			});
		}
		return {
			input: input,
			classValue: classValue,
			selfMapping: selfMapping,
			arguments: arguments
		};
	}

	public function prepareFunction(signature:PreparedConstructorSignature):PreparedBodyFunction {
		if (input.elided)
			throw new CBodyEmissionError('elided constructor `${input.id}` requested a generated function');
		final functionValue = switch input.expression.expr {
			case TFunction(value): value;
			case _: unsupported(input.expression.pos, FunctionBuilder.nodeName(input.expression));
		};
		final overloadSignature = [constructorTypeKey(signature.selfMapping.irType)].concat(signature.arguments.map(parameter ->
			constructorTypeKey(parameter.ir.type)));
		final functionRequest = new CSymbolRequest(CSKMethod, ["compiler", "constructor"].concat(input.declarationPath.split(".")),
			CNSOrdinary("translation-unit"), CSVInternal, null, overloadSignature, [], input.sourceOrder);
		context.symbols.register(functionRequest);
		final source = HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath);
		final self:PreparedParameter = {
			compilerId: -1,
			ir: {id: "parameter.self", type: signature.selfMapping.irType, source: source},
			mapping: signature.selfMapping
		};
		final parameters = [self].concat(signature.arguments);
		final parameterRequests:Map<String, CSymbolRequest> = [];
		for (index in 0...parameters.length) {
			final parameter = parameters[index];
			final sourceName = index == 0 ? "self" : functionValue.args[index - 1].v.name;
			final request = new CSymbolRequest(CSKLocal, input.declarationPath.split(".").concat(["new", sourceName]),
				CNSOrdinary(functionRequest.stableKey()), CSVInternal, null, [], [], index);
			context.symbols.register(request);
			parameterRequests.set(parameter.ir.id, request);
		}
		final voidMapping = switch CPrimitiveTypeMapper.map(Context.getType("Void"), context.profile) {
			case CTPrimitive(mapping) if (mapping.sourceType == CPHaxeVoid && mapping.nullability == CPNonNullable): CBodyValueType.primitive(mapping);
			case _: throw new CBodyEmissionError('constructor `${input.id}` could not resolve target Void');
		};
		return {
			modulePath: input.modulePath,
			declarationPath: input.declarationPath,
			sourcePath: input.sourcePath,
			displayName: "new",
			fieldName: "new",
			specialization: null,
			sourceExpression: input.expression,
			bodyExpression: functionValue.expr,
			role: PBRConstructor(signature),
			irId: input.id,
			parameters: parameters,
			returnMapping: voidMapping,
			functionRequest: functionRequest,
			parameterRequests: parameterRequests
		};
	}

	function admittedValueType(type:Type, position:Position, node:String):CBodyValueType
		return aggregateRegistry.valueType(type, position, input.modulePath, input.sourcePath, reject, node);

	function reject(position:Position, node:String):Void
		unsupported(position, node);

	function unsupported<T>(position:Position, node:String):T {
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile), node, functionContext, source), position);
	}

	static function isRestType(type:Type):Bool {
		return switch type {
			case TAbstract(reference, _): final value = reference.get(); value.name == "Rest" && (value.pack.join(".") == "haxe"
					|| value.pack.join(".") == "haxe.extern");
			case TType(reference, parameters): final definition = reference.get(); definition.name == "Rest" && definition.pack.join(".") == "haxe.extern" ? true : isRestType(TypeTools.applyTypeParameters(definition.type,
					definition.params, parameters));
			case TMono(reference): final resolved = reference.get(); resolved != null && isRestType(resolved);
			case TLazy(resolve): isRestType(resolve());
			case _: false;
		};
	}

	static function constructorTypeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTFloat(width): 'f$width';
			case IRTPointer(IRTInstance(instanceId), nullable): 'class-reference:${nullable ? "nullable" : "nonnull"}:$instanceId';
			case _: throw new CBodyEmissionError("constructor signature contains a non-admitted body type");
		};
	}
}

private class InitializerPreparer {
	final context:CompilationContext;
	final input:CBodyInitializerInput;

	public function new(context:CompilationContext, input:CBodyInitializerInput) {
		this.context = context;
		this.input = input;
	}

	public function prepare():PreparedBodyFunction {
		final returnMapping = switch CPrimitiveTypeMapper.map(Context.getType("Void"), context.profile) {
			case CTPrimitive(mapping) if (mapping.sourceType == CPHaxeVoid && mapping.nullability == CPNonNullable): CBodyValueType.primitive(mapping);
			case _: throw new CBodyEmissionError('static initializer `${input.id}` could not resolve the target Void representation');
		};
		final role = switch input.kind {
			case CBIClass: PBRClassInitializer;
			case CBIStaticField(globalId): PBRStaticFieldInitializer(globalId);
		};
		final symbolPath = ["compiler", "static-initialization"].concat(input.declarationPath.split(".")).concat([input.displayName]);
		final functionRequest = new CSymbolRequest(CSKStaticInitializer, symbolPath, CNSOrdinary("translation-unit"), CSVInternal, null, [], [],
			input.sourceOrder);
		context.symbols.register(functionRequest);
		return {
			modulePath: input.modulePath,
			declarationPath: input.declarationPath,
			sourcePath: input.sourcePath,
			displayName: input.displayName,
			fieldName: input.displayName,
			specialization: null,
			sourceExpression: input.expression,
			bodyExpression: input.expression,
			role: role,
			irId: input.id,
			parameters: [],
			returnMapping: returnMapping,
			functionRequest: functionRequest,
			parameterRequests: []
		};
	}
}

private class FunctionBuilder {
	final context:CompilationContext;
	final prepared:PreparedBodyFunction;
	final input:PreparedBodyFunction;
	final functionsById:Map<String, PreparedBodyFunction>;
	final constructorSignaturesById:Map<String, PreparedConstructorSignature>;
	final globalRegistry:BodyGlobalRegistry;
	final aggregateRegistry:CBodyAggregateRegistry;
	final dispatch:CPreparedBodyDispatch;
	final functionContext:String;
	final parameterValuesByCompilerId:Map<Int, LoweredValue> = [];
	final localIdsByCompilerId:Map<Int, String> = [];
	final localTypesByCompilerId:Map<Int, CBodyValueType> = [];
	final collectionBindingsByCompilerId:Map<Int, BodyCollectionBinding> = [];
	final localRequests:Map<String, CSymbolRequest> = [];
	final spanLengthRequests:Map<String, CSymbolRequest> = [];
	final temporaryRequests:Map<String, CSymbolRequest> = [];
	final tailArgumentRequests:Map<String, Array<CSymbolRequest>> = [];
	final labelRequests:Map<String, CSymbolRequest> = [];
	final locals:Array<HxcIRLocal> = [];
	final blocks:Array<MutableBodyBlock> = [];
	final loopControlStack:Array<LoopControlTargets> = [];
	final runtimeRequirements:Array<CBodyRuntimeRequirement> = [];
	final constructionCleanupActions:Array<HxcIRCleanupAction> = [];
	final constructedObjects:Array<BodyConstructedObject> = [];
	final stackConstructedCompilerIds:Map<Int, Bool> = [];
	var selfValue:Null<LoweredValue> = null;
	var localOrdinal = 0;
	var temporaryOrdinal = 0;
	var instructionOrdinal = 0;
	var valueOrdinal = 0;
	var blockOrdinal = 0;
	var currentBlock:MutableBodyBlock;

	public function new(context:CompilationContext, prepared:PreparedBodyFunction, functionsById:Map<String, PreparedBodyFunction>,
			constructorSignaturesById:Map<String, PreparedConstructorSignature>, globalRegistry:BodyGlobalRegistry, aggregateRegistry:CBodyAggregateRegistry,
			dispatch:CPreparedBodyDispatch) {
		this.context = context;
		this.prepared = prepared;
		this.input = prepared;
		this.functionsById = functionsById;
		this.constructorSignaturesById = constructorSignaturesById;
		this.globalRegistry = globalRegistry;
		this.aggregateRegistry = aggregateRegistry;
		this.dispatch = dispatch;
		this.functionContext = 'function ${input.declarationPath}.${input.displayName} body';
		this.localOrdinal = prepared.parameters.length;
		this.currentBlock = createEntryBlock(HaxeSourceSpan.fromPosition(prepared.bodyExpression.pos, input.sourcePath));
		for (parameter in prepared.parameters) {
			final value:LoweredValue = {id: parameter.ir.id, type: parameter.ir.type, mapping: parameter.mapping};
			if (parameter.ir.id == "parameter.self") {
				selfValue = value;
			} else {
				parameterValuesByCompilerId.set(parameter.compilerId, value);
			}
			switch parameter.mapping.kind {
				case CBVKSpan(element, mutable):
					final parameterRequest = prepared.parameterRequests.get(parameter.ir.id);
					if (parameterRequest == null)
						throw new CBodyEmissionError('span parameter `${parameter.ir.id}` in `${prepared.irId}` has no symbol request');
					final parameterLengthRequest = new CSymbolRequest(CSKLocal, parameterRequest.qualifiedName.concat(["length"]), parameterRequest.namespace,
						CSVInternal, null, [], [], parameterRequest.sourceOrdinal);
					context.symbols.register(parameterLengthRequest);
					spanLengthRequests.set(parameter.ir.id, parameterLengthRequest);

					// HxcIR keeps the borrow as one semantic value. A local span place gives
					// indexing and bounds checks the same representation used by local CArray
					// borrows; only the final C signature expands it to pointer + length.
					final ordinal = localOrdinal++;
					final localId = 'local.$ordinal';
					final localRequest = new CSymbolRequest(CSKLocal, parameterRequest.qualifiedName.concat(["borrow"]), parameterRequest.namespace,
						CSVInternal, null, [], [], ordinal);
					context.symbols.register(localRequest);
					localRequests.set(localId, localRequest);
					final localLengthRequest = new CSymbolRequest(CSKTemporary, parameterRequest.qualifiedName.concat(["borrow", "length"]),
						parameterRequest.namespace, CSVInternal, null, [], [], ordinal);
					context.symbols.register(localLengthRequest);
					spanLengthRequests.set(localId, localLengthRequest);
					locals.push({
						id: localId,
						type: parameter.ir.type,
						storage: IRLSAutomatic,
						initialState: IRISUninitialized,
						source: parameter.ir.source
					});
					appendInstruction(null, IRIOInitialize(IRPLocal(localId), parameter.ir.id, IRISUninitialized, IRISInitialized), parameter.ir.source,
						"span-parameter-borrow");
					collectionBindingsByCompilerId.set(parameter.compilerId, {
						localId: localId,
						kind: BCKSpan(mutable),
						element: element,
						length: null
					});
				case _:
			}
		}
	}

	public function build():BuiltBodyFunction {
		final bodyExpression = prepared.bodyExpression;
		switch prepared.role {
			case PBRFunction | PBRConstructor(_) | PBRClassInitializer:
				lowerStatement(bodyExpression);
			case PBRStaticFieldInitializer(globalId):
				final global = globalRegistry.requireId(globalId, bodyExpression, unsupportedAt);
				final globalType = CBodyValueType.primitive(global.mapping);
				final value = coerce(lowerValue(bodyExpression, globalType), globalType, bodyExpression.pos, "static-field-initializer");
				appendInstruction(null, IRIOInitialize(IRPGlobal(global.ir.id), value.id, IRISUninitialized, IRISInitialized),
					HaxeSourceSpan.fromPosition(bodyExpression.pos, input.sourcePath), "initialize-global");
		}
		if (currentBlock.terminator == null) {
			currentBlock.terminator = {
				kind: IRTReturn(null, normalCleanupSteps()),
				source: HaxeSourceSpan.fromPosition(bodyExpression.pos, input.sourcePath)
			};
		}
		final functionSpan = HaxeSourceSpan.fromPosition(input.sourceExpression.pos, input.sourcePath);
		final ir:HxcIRFunction = {
			id: prepared.irId,
			displayName: '${input.declarationPath}.${input.displayName}',
			parameters: prepared.parameters.map(parameter -> parameter.ir),
			locals: locals,
			returnType: prepared.returnMapping.irType,
			failureConvention: switch prepared.role {
				case PBRConstructor(signature) if (signature.input.canFail): IRFCStatus(IRFException);
				case _: IRFCInfallible;
			},
			entryBlockId: "entry",
			blocks: blocks.map(block -> {
				id: block.id,
				parameters: [],
				instructions: block.instructions,
				terminator: block.terminator,
				source: block.source
			}),
			cleanupRegions: constructionCleanupActions.length == 0 ? [] : [
				{
					id: "cleanup.construction",
					parentId: null,
					actions: constructionCleanupActions,
					source: functionSpan
				}
			],
			source: functionSpan
		};
		final coalescing = new CBodyValueCoalescingPlanner().plan(ir);
		final inlinedValueIds:Array<String> = [];
		for (valueId => request in temporaryRequests) {
			if (coalescing.shouldInline(valueId)) {
				inlinedValueIds.push(valueId);
			} else {
				context.symbols.register(request);
			}
		}
		for (valueId in inlinedValueIds)
			temporaryRequests.remove(valueId);
		return {
			prepared: prepared,
			ir: ir,
			localRequests: localRequests,
			spanLengthRequests: spanLengthRequests,
			temporaryRequests: temporaryRequests,
			tailArgumentRequests: tailArgumentRequests,
			labelRequests: labelRequests,
			runtimeRequirements: runtimeRequirements
		};
	}

	function lowerStatement(expression:TypedExpr):Void {
		if (currentBlock.terminator != null) {
			unsupported(expression, 'unreachable ${nodeName(expression)}');
		}
		switch expression.expr {
			case TBlock(expressions):
				lowerStatementBlock(expressions);
			case TVar(variable, initializer):
				lowerVariable(variable, initializer, expression.pos);
			case TReturn(value):
				lowerReturn(value, expression.pos);
			case TParenthesis(inner):
				lowerStatement(inner);
			case TMeta(_, inner):
				lowerStatement(inner);
			case TConst(_) | TLocal(_) | TArray(_, _) | TField(_, _) | TCast(_, _) | TBinop(_, _, _) | TUnop(_, _, _):
				lowerValue(expression);
			case TCall(_, _) if (isEnumConstructorExpression(expression)):
				lowerValue(expression);
			case TCall(callee, arguments) if (isSuperCall(callee)):
				lowerSuperCall(expression, arguments);
			case TCall(_, _):
				lowerCall(expression, false);
			case TThrow(value):
				lowerThrow(expression, value);
			case TIf(condition, whenTrue, whenFalse):
				lowerStatementConditional(expression, condition, whenTrue, whenFalse);
			case TWhile(condition, body, normalWhile):
				lowerLoop(expression, condition, body, normalWhile);
			case TSwitch(subject, cases, defaultExpression):
				lowerStatementSwitch(expression, subject, cases, defaultExpression);
			case TBreak:
				lowerLoopJump(expression, true);
			case TContinue:
				lowerLoopJump(expression, false);
			case TNew(_, _, _):
				unsupported(expression, "TNew(stack-construction-requires-direct-local)");
			case _:
				unsupported(expression, nodeName(expression));
		}
	}

	function lowerStatementBlock(expressions:Array<TypedExpr>):Void {
		var index = 0;
		while (index < expressions.length) {
			if (index + 1 < expressions.length && tryLowerSpanLoop(expressions[index], expressions[index + 1])) {
				index += 2;
				continue;
			}
			final nested = expressions[index];
			switch nested.expr {
				case TVar(variable, null) if (index + 1 < expressions.length
					&& followingSwitchInitializesLocal(expressions[index + 1], variable.id)):
					if (currentBlock.terminator != null) {
						unsupported(nested, 'unreachable ${nodeName(nested)}');
					}
					lowerVariable(variable, null, nested.pos, true);
				case _:
					lowerStatement(nested);
			}
			index++;
		}
	}

	function tryLowerSpanLoop(iteratorDeclaration:TypedExpr, loopExpression:TypedExpr):Bool {
		final pattern = spanLoopPattern(iteratorDeclaration, loopExpression);
		if (pattern == null) {
			return false;
		}
		lowerSpanLoop(pattern);
		return true;
	}

	function spanLoopPattern(iteratorDeclaration:TypedExpr, loopExpression:TypedExpr):Null<SpanLoopPattern> {
		final iterator = switch iteratorDeclaration.expr {
			case TVar(variable, initializer) if (initializer != null): {variable: variable, initializer: initializer};
			case _: return null;
		};
		final spanVariable = switch iterator.initializer.expr {
			case TCall(callee, [argument]) if (isAbstractMethod(callee, "c.Span", "iterator")
				|| isAbstractMethod(callee, "c.ConstSpan", "iterator")):
				switch unwrapExpression(argument).expr {
					case TLocal(variable): variable;
					case _: return null;
				}
			case _: return null;
		};
		final span = collectionBindingsByCompilerId.get(spanVariable.id);
		if (span == null) {
			return null;
		}
		switch span.kind {
			case BCKSpan(_):
			case BCKFixedArray(_):
				return null;
		}
		final spanLength = span.length;
		if (spanLength == null) {
			return unsupported(loopExpression, "TFor(span-parameter-dynamic-length-loop-not-admitted)");
		}
		final loop = switch loopExpression.expr {
			case TWhile(condition, body, true): {condition: condition, body: body};
			case _: return null;
		};
		if (!isIteratorCall(loop.condition, iterator.variable.id, "hasNext")) {
			return null;
		}
		final expressions = switch unwrapExpression(loop.body).expr {
			case TBlock(values): values;
			case _: return null;
		};
		if (expressions.length == 0) {
			return null;
		}
		final loopVariable = switch expressions[0].expr {
			case TVar(variable, initializer) if (initializer != null
				&& isIteratorCall(initializer, iterator.variable.id, "next")): variable;
			case _: return null;
		};
		if (typeKey(span.element.irType) != typeKey(collectionElement(loopVariable.t, expressions[0].pos, 'TFor(${loopVariable.name}:type)').irType)) {
			return null;
		}
		return {
			iteratorCompilerId: iterator.variable.id,
			loopVariable: loopVariable,
			span: span,
			length: spanLength,
			body: expressions.slice(1),
			sourceExpression: loopExpression
		};
	}

	function lowerSpanLoop(pattern:SpanLoopPattern):Void {
		final source = HaxeSourceSpan.fromPosition(pattern.sourceExpression.pos, input.sourcePath);
		final indexMapping = compilerSpanIndexMapping(pattern.sourceExpression.pos);
		final indexType = CBodyValueType.primitive(indexMapping);
		final zero:HxcIRResult = {id: nextValueId(), type: indexMapping.irType};
		appendInstruction(zero, IRIOConstant(IRCInt("0")), source, "span-loop-zero");
		final indexLocalId = createFlowLocal(indexType, zero.id, source, "span-loop-index");

		final conditionBlock = createGeneratedBlock("span-loop-condition", source);
		final bodyBlock = createGeneratedBlock("span-loop-body", source);
		final incrementBlock = reserveGeneratedBlock("span-loop-increment", source);
		final exitBlock = createGeneratedBlock("span-loop-exit", source);
		currentBlock.terminator = {kind: IRTJump(edge(conditionBlock.id)), source: source};

		currentBlock = conditionBlock;
		final conditionIndex = loadPlace({place: IRPLocal(indexLocalId), mapping: indexType, mutable: true}, pattern.sourceExpression.pos,
			"span-loop-condition-index");
		final length:HxcIRResult = {id: nextValueId(), type: indexMapping.irType};
		appendInstruction(length, IRIOConstant(IRCInt(Std.string(pattern.length))), source, "span-loop-length");
		final condition:HxcIRResult = {id: nextValueId(), type: IRTBool};
		final guardInstruction = appendInstruction(condition, IRIOBinary("hxc.size.less.span-index", conditionIndex.id, length.id, IRIStatic), source,
			"span-loop-condition");
		currentBlock.terminator = {kind: IRTBranch(condition.id, edge(bodyBlock.id), edge(exitBlock.id)), source: source};

		final control = loopControl(exitBlock.id, incrementBlock.id);
		loopControlStack.push(control);
		currentBlock = bodyBlock;
		final bodyIndex = loadPlace({place: IRPLocal(indexLocalId), mapping: indexType, mutable: true}, pattern.sourceExpression.pos, "span-loop-body-index");
		appendInstruction(null,
			IRIOBoundsCheck(IRPLocal(pattern.span.localId), bodyIndex.id, IRBPLoopGuarded(guardInstruction.id, indexLocalId, pattern.length)), source,
			"span-loop-bounds");
		final element = loadPlace({
			place: IRPIndex(IRPLocal(pattern.span.localId), bodyIndex.id),
			mapping: CBodyValueType.primitive(pattern.span.element),
			mutable: switch pattern.span.kind {
				case BCKSpan(mutable): mutable;
				case BCKFixedArray(_): false;
			}
		}, pattern.sourceExpression.pos, "span-loop-element");
		parameterValuesByCompilerId.set(pattern.loopVariable.id, element);
		for (expression in pattern.body) {
			lowerStatement(expression);
		}
		parameterValuesByCompilerId.remove(pattern.loopVariable.id);
		loopControlStack.pop();
		final bodyEnd = currentBlock;
		final needsIncrement = bodyEnd.terminator == null || control.usedContinue;
		if (needsIncrement) {
			activateGeneratedBlock(incrementBlock);
			if (bodyEnd.terminator == null) {
				bodyEnd.terminator = {kind: IRTJump(edge(incrementBlock.id)), source: source};
			}
			currentBlock = incrementBlock;
			final currentIndex = loadPlace({place: IRPLocal(indexLocalId), mapping: indexType, mutable: true}, pattern.sourceExpression.pos,
				"span-loop-increment-index");
			final one:HxcIRResult = {id: nextValueId(), type: indexMapping.irType};
			appendInstruction(one, IRIOConstant(IRCInt("1")), source, "span-loop-one");
			final next:HxcIRResult = {id: nextValueId(), type: indexMapping.irType};
			appendInstruction(next, IRIOBinary("hxc.size.add-one.span-index-proven", currentIndex.id, one.id, IRIStatic), source, "span-loop-increment");
			appendInstruction(null, IRIOStore(IRPLocal(indexLocalId), next.id), source, "span-loop-increment-store");
			currentBlock.terminator = {kind: IRTJump(edge(conditionBlock.id)), source: source};
		}
		currentBlock = exitBlock;
	}

	function isIteratorCall(expression:TypedExpr, iteratorCompilerId:Int, methodName:String):Bool {
		return switch unwrapExpression(expression).expr {
			case TCall(callee, []):
				switch unwrapExpression(callee).expr {
					case TField(receiver, access): fieldAccessName(access) == methodName && switch unwrapExpression(receiver).expr {
							case TLocal(variable): variable.id == iteratorCompilerId;
							case _: false;
						};
					case _: false;
				}
			case _: false;
		};
	}

	function lowerVariable(variable:TVar, initializer:Null<TypedExpr>, position:Position, compilerSwitchCarrier:Bool = false):Void {
		final ordinal = localOrdinal++;
		final localId = 'local.$ordinal';
		if (initializer != null) {
			final construction = newExpression(initializer);
			if (construction != null) {
				lowerConstructedVariable(variable, initializer, construction, position, ordinal, localId);
				return;
			}
			if (referencesStackConstructedValue(initializer)) {
				unsupported(initializer, 'TNew(stack-reference-escape:local-alias:${variable.name})');
			}
		}
		final collectionType = bodyCollectionType(variable.t, position, 'TVar(${variable.name}:type)');
		if (collectionType != null) {
			lowerCollectionVariable(variable, initializer, position, ordinal, localId, collectionType);
			return;
		}
		final localMapping = bodyValueType(variable.t, position, 'TVar(${variable.name}:type)');
		if (localMapping.irType == IRTVoid) {
			unsupportedAt(position, 'TVar(${variable.name}:Void)');
		}
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		final value:LoweredValue = switch initializer {
			case null if (compilerSwitchCarrier):
				// Reflaxe exposes a value switch as a temporary followed by a switch
				// that assigns it. The structural recognition below proves every arm
				// assigns; this defensive value prevents C uninitialized storage without
				// becoming observable on an admitted path.
				final result:HxcIRResult = {id: nextValueId(), type: localMapping.irType};
				appendInstruction(result, IRIOConstant(defaultConstantAt(localMapping.irType, position, 'TVar(${variable.name}:switch-carrier)')), source,
					"switch-carrier-default");
				{id: result.id, type: result.type, mapping: localMapping};
			case null:
				unsupportedAt(position, 'TVar(${variable.name}:uninitialized)');
			case expression:
				coerce(lowerValue(expression, localMapping), localMapping, expression.pos, 'TVar(${variable.name}:initializer)');
		};
		locals.push({
			id: localId,
			type: localMapping.irType,
			storage: IRLSAutomatic,
			initialState: IRISUninitialized,
			source: source
		});
		final request = new CSymbolRequest(CSKLocal, input.declarationPath.split(".").concat([input.fieldName, variable.name]),
			CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
		context.symbols.register(request);
		localRequests.set(localId, request);
		appendInstruction(null, IRIOInitialize(IRPLocal(localId), value.id, IRISUninitialized, IRISInitialized), source, "initialize");
		localIdsByCompilerId.set(variable.id, localId);
		localTypesByCompilerId.set(variable.id, localMapping);
	}

	function lowerConstructedVariable(variable:TVar, expression:TypedExpr, construction:BodyNewExpression, position:Position, ordinal:Int,
			localId:String):Void {
		if (currentBlock.id != "entry" || blocks.length != 1) {
			unsupported(expression, "TNew(stack-construction-requires-unconditional-entry-block)");
		}
		final classDefinition = construction.classReference.get();
		final classPath = CBodyConstructor.classPath(construction.classReference);
		if (construction.parameters.length != 0 || classDefinition.params.length != 0) {
			// Preserve the established local-type boundary for unsupported generic
			// class references. Constructor discovery must not make an unrelated
			// program fail earlier with a less fundamental expression diagnostic.
			bodyValueType(variable.t, position, 'TVar(${variable.name}:type)');
			unsupported(expression, 'TNew(generic-class-constructor-requires-specialization:$classPath)');
		}
		if (classDefinition.isExtern || classDefinition.meta.has(":c.layout")) {
			unsupported(expression, 'TNew(unsupported-native-layout:$classPath)');
		}
		if (classDefinition.isInterface) {
			unsupported(expression, 'TNew(interface-layout:$classPath)');
		}
		final targetId = CBodyConstructor.id(classPath);
		final signature = constructorSignaturesById.get(targetId);
		if (signature == null)
			unsupported(expression, 'TNew(unavailable-constructor:$targetId)');
		if (construction.arguments.length != signature.arguments.length) {
			unsupported(expression, 'TNew(argument-count=${construction.arguments.length},expected=${signature.arguments.length},target=$targetId)');
		}
		final localMapping = bodyValueType(variable.t, position, 'TVar(${variable.name}:constructed-type)');
		if (localMapping.classValue() == null)
			unsupportedAt(position, 'TNew(non-class-local:${variable.name})');

		// Haxe evaluates constructor arguments before entering the constructor body.
		final arguments:Array<String> = [];
		for (index in 0...construction.arguments.length) {
			final argumentExpression = construction.arguments[index];
			if (referencesStackConstructedValue(argumentExpression)) {
				unsupported(argumentExpression, 'TNew(stack-reference-escape:constructor-argument:$index)');
			}
			arguments.push(coerce(lowerValue(argumentExpression, signature.arguments[index].mapping), signature.arguments[index].mapping,
				argumentExpression.pos, 'TNew(argument:$index,target=$targetId)').id);
		}

		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		final backingOrdinal = localOrdinal++;
		final backingLocalId = 'local.$backingOrdinal';
		locals.push({
			id: backingLocalId,
			type: IRTInstance(signature.classValue.instanceId),
			storage: IRLSAutomatic,
			initialState: IRISUninitialized,
			source: source
		});
		final backingRequest = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, variable.name, "object-storage"]),
			CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], backingOrdinal);
		context.symbols.register(backingRequest);
		localRequests.set(backingLocalId, backingRequest);
		appendInstruction(null,
			IRIODefaultInitialize(IRPLocal(backingLocalId), IRISUninitialized, signature.input.elided ? IRISInitialized : IRISInitializing), source,
			"class-default-initialize");
		final virtualTable = dispatch.tableForInstance(signature.classValue.instanceId);
		if (virtualTable != null) {
			appendInstruction(null, IRIOBindVirtualTable(IRPLocal(backingLocalId), virtualTable.input.id), source, "class-bind-virtual-table");
		}

		final address:HxcIRResult = {id: nextValueId(), type: signature.selfMapping.irType};
		appendInstruction(address, IRIOAddress(IRPLocal(backingLocalId)), source, "class-object-address");
		registerValueTemporary(address.id, "class-object-address");
		final self:LoweredValue = {id: address.id, type: address.type, mapping: signature.selfMapping};

		final constructionOrdinal = constructedObjects.length;
		final partialActionId = 'construction.$constructionOrdinal.partial';
		final initializedActionId = 'construction.$constructionOrdinal.initialized';
		constructionCleanupActions.push({
			id: partialActionId,
			idempotence: IRCExactlyOnce,
			kind: IRCADestroy(IRPLocal(backingLocalId), IRISInitializing, IRISDestroyed),
			source: source
		});
		constructionCleanupActions.push({
			id: initializedActionId,
			idempotence: IRCExactlyOnce,
			kind: IRCADestroy(IRPLocal(backingLocalId), IRISInitialized, IRISDestroyed),
			source: source
		});

		if (!signature.input.elided) {
			final target = functionsById.get(targetId);
			if (target == null)
				throw new CBodyEmissionError('non-elided constructor `$targetId` has no prepared function');
			final failure:Null<HxcIRFailureEdge> = signature.input.canFail ? {
				kind: IRFException,
				target: constructionFailureTarget(),
				arguments: [],
				cleanup: partialConstructionCleanup(partialActionId)
			} : null;
			appendInstruction(null, IRIOCall({
				dispatch: IRCDDirect(targetId),
				arguments: [self.id].concat(arguments),
				returnType: IRTVoid,
				failure: failure
			}), source, "constructor-call");
			appendInstruction(null, IRIOLifetime(IRPLocal(backingLocalId), IRISInitializing, IRISInitialized, "constructor completed"), source,
				"constructor-complete");
		}

		final reference = coerce(self, localMapping, expression.pos, 'TNew(result:$targetId)');
		locals.push({
			id: localId,
			type: localMapping.irType,
			storage: IRLSAutomatic,
			initialState: IRISUninitialized,
			source: source
		});
		final request = new CSymbolRequest(CSKLocal, input.declarationPath.split(".").concat([input.fieldName, variable.name]),
			CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
		context.symbols.register(request);
		localRequests.set(localId, request);
		appendInstruction(null, IRIOInitialize(IRPLocal(localId), reference.id, IRISUninitialized, IRISInitialized), source, "initialize-class-reference");
		localIdsByCompilerId.set(variable.id, localId);
		localTypesByCompilerId.set(variable.id, localMapping);
		stackConstructedCompilerIds.set(variable.id, true);
		constructedObjects.push({
			backingLocalId: backingLocalId,
			partialActionId: partialActionId,
			initializedActionId: initializedActionId,
			source: source
		});
	}

	function lowerSuperCall(expression:TypedExpr, callArguments:Array<TypedExpr>):Void {
		final signature = switch prepared.role {
			case PBRConstructor(value): value;
			case _: return unsupported(expression, "TCall(super:outside-constructor)");
		};
		final baseId = signature.input.baseConstructorId;
		if (baseId == null)
			unsupported(expression, 'TCall(super:constructor-without-base:${signature.input.declarationPath})');
		final target = constructorSignaturesById.get(baseId);
		if (target == null)
			unsupported(expression, 'TCall(super:unavailable-constructor:$baseId)');
		if (callArguments.length != target.arguments.length) {
			unsupported(expression, 'TCall(super:argument-count=${callArguments.length},expected=${target.arguments.length},target=$baseId)');
		}
		final self = selfValue;
		if (self == null)
			throw new CBodyEmissionError('constructor `${prepared.irId}` lost its self parameter');
		final baseSelf = coerce(self, target.selfMapping, expression.pos, 'TCall(super:self,target=$baseId)');
		final arguments:Array<String> = [baseSelf.id];
		for (index in 0...callArguments.length) {
			final argument = callArguments[index];
			if (referencesStackConstructedValue(argument))
				unsupported(argument, 'TNew(stack-reference-escape:super-argument:$index)');
			arguments.push(coerce(lowerValue(argument, target.arguments[index].mapping), target.arguments[index].mapping, argument.pos,
				'TCall(super:argument:$index,target=$baseId)').id);
		}
		if (target.input.elided)
			return;
		if (!functionsById.exists(baseId))
			throw new CBodyEmissionError('non-elided base constructor `$baseId` has no prepared function');
		final failure:Null<HxcIRFailureEdge> = target.input.canFail ? {
			kind: IRFException,
			target: IRFTPropagate,
			arguments: [],
			cleanup: normalCleanupSteps()
		} : null;
		appendInstruction(null, IRIOCall({
			dispatch: IRCDDirect(baseId),
			arguments: arguments,
			returnType: IRTVoid,
			failure: failure
		}), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
			"super-constructor-call");
	}

	function lowerThrow(expression:TypedExpr, valueExpression:TypedExpr):Void {
		final canFail = switch prepared.role {
			case PBRConstructor(signature): signature.input.canFail;
			case _: false;
		};
		if (!canFail)
			unsupported(expression, "TThrow");
		if (referencesStackConstructedValue(valueExpression))
			unsupported(valueExpression, "TNew(stack-reference-escape:throw-payload)");
		final value = lowerValue(valueExpression);
		currentBlock.terminator = {
			kind: IRTThrow(value.id, {
				kind: IRFException,
				target: IRFTPropagate,
				arguments: [],
				cleanup: normalCleanupSteps()
			}),
			source: HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath)
		};
	}

	function normalCleanupSteps():Array<HxcIRCleanupStep> {
		final result:Array<HxcIRCleanupStep> = [];
		var index = constructedObjects.length;
		while (index > 0) {
			final value = constructedObjects[--index];
			result.push({regionId: "cleanup.construction", actionId: value.initializedActionId});
		}
		return result;
	}

	function partialConstructionCleanup(partialActionId:String):Array<HxcIRCleanupStep> {
		final result:Array<HxcIRCleanupStep> = [{regionId: "cleanup.construction", actionId: partialActionId}];
		return result.concat(normalCleanupSteps());
	}

	function constructionFailureTarget():HxcIRFailureTarget {
		return switch prepared.role {
			case PBRConstructor(signature) if (signature.input.canFail): IRFTPropagate;
			case _: IRFTAbort;
		};
	}

	static function newExpression(expression:TypedExpr):Null<BodyNewExpression> {
		return switch expression.expr {
			case TNew(classReference, parameters, arguments): {classReference: classReference, parameters: parameters, arguments: arguments};
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): newExpression(inner);
			case _: null;
		};
	}

	function referencesStackConstructedValue(expression:TypedExpr):Bool {
		return switch expression.expr {
			case TLocal(variable): stackConstructedCompilerIds.exists(variable.id);
			case TConst(TThis):
				switch prepared.role {
					case PBRConstructor(_): true;
					case _: false;
				}
			case TField(receiver, _) if (isClassReferenceType(expression.t)):
				referencesStackConstructedValue(receiver);
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): referencesStackConstructedValue(inner);
			case TIf(_, whenTrue, whenFalse): referencesStackConstructedValue(whenTrue) || whenFalse != null && referencesStackConstructedValue(whenFalse);
			case TBlock(expressions): expressions.length > 0 && referencesStackConstructedValue(expressions[expressions.length - 1]);
			case TSwitch(_, cases, defaultExpression):
				var found = defaultExpression != null && referencesStackConstructedValue(defaultExpression);
				for (item in cases) {
					if (referencesStackConstructedValue(item.expr))
						found = true;
				}
				found;
			case _: false;
		};
	}

	static function isClassReferenceType(type:Type):Bool {
		return switch TypeTools.follow(type) {
			case TInst(reference, _): final value = reference.get(); !value.isExtern && switch value.kind {
					case KTypeParameter(_): false;
					case _: true;
				};
			case _: false;
		};
	}

	static function isSuperCall(callee:TypedExpr):Bool {
		return switch callee.expr {
			case TConst(TSuper): true;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isSuperCall(inner);
			case _: false;
		};
	}

	function lowerCollectionVariable(variable:TVar, initializer:Null<TypedExpr>, position:Position, ordinal:Int, localId:String,
			collectionType:BodyCollectionType):Void {
		if (initializer == null) {
			unsupportedAt(position, 'TVar(${variable.name}:collection-uninitialized)');
		}
		final expression:TypedExpr = initializer;
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		switch collectionType.kind {
			case BCKFixedArray(witnessId):
				final zeroLengthExpression = fixedArrayZeroLengthExpression(expression);
				var length:Int;
				var initializer:BodyFixedArrayInitializer;
				if (zeroLengthExpression == null) {
					final elements = requireArrayLiteral(expression, variable.name);
					if (elements.length == 0) {
						unsupported(expression, 'TArrayDecl(empty-fixed-array-not-strict-c11)');
					}
					final values:Array<String> = [];
					for (element in elements) {
						final elementType = CBodyValueType.primitive(collectionType.element);
						values.push(coerce(lowerValue(element, elementType), elementType, element.pos, 'TArrayDecl(element:${values.length})').id);
					}
					length = elements.length;
					initializer = BFAIValues(values);
				} else {
					length = requireFixedArrayZeroLength(zeroLengthExpression);
					requireFixedArrayZeroStorage(collectionType.element.irType, length, zeroLengthExpression);
					initializer = BFAIZero;
				}
				locals.push({
					id: localId,
					type: IRTFixedArray(collectionType.element.irType, length, witnessId),
					storage: IRLSAutomatic,
					initialState: IRISUninitialized,
					source: source
				});
				registerCollectionLocal(variable, ordinal, localId, false);
				switch initializer {
					case BFAIValues(values):
						appendInstruction(null, IRIOInitializeFixedArray(IRPLocal(localId), values, IRISUninitialized, IRISInitialized), source,
							"fixed-array-initialize");
					case BFAIZero:
						appendInstruction(null, IRIOZeroInitializeFixedArray(IRPLocal(localId), IRISUninitialized, IRISInitialized), source,
							"fixed-array-zero-initialize");
				}
				collectionBindingsByCompilerId.set(variable.id, {
					localId: localId,
					kind: collectionType.kind,
					element: collectionType.element,
					length: length
				});
			case BCKSpan(mutable):
				final sourceVariable = requireSpanSource(expression, mutable);
				final sourceBinding = collectionBindingsByCompilerId.get(sourceVariable.id);
				if (sourceBinding == null) {
					unsupported(expression, 'TCall(${mutable ? "span" : "constSpan"}:source-outside-admitted-fixed-array-local)');
				}
				switch sourceBinding.kind {
					case BCKFixedArray(_):
					case BCKSpan(_): unsupported(expression, 'TCall(${mutable ? "span" : "constSpan"}:span-source-not-fixed-array)');
				}
				if (typeKey(sourceBinding.element.irType) != typeKey(collectionType.element.irType)) {
					unsupported(expression, 'TCall(${mutable ? "span" : "constSpan"}:element-type-mismatch)');
				}
				locals.push({
					id: localId,
					type: IRTSpan(collectionType.element.irType, mutable),
					storage: IRLSAutomatic,
					initialState: IRISUninitialized,
					source: source
				});
				registerCollectionLocal(variable, ordinal, localId, true);
				appendInstruction(null, IRIOInitializeSpan(IRPLocal(localId), IRPLocal(sourceBinding.localId), IRISUninitialized, IRISInitialized), source,
					"span-initialize");
				collectionBindingsByCompilerId.set(variable.id, {
					localId: localId,
					kind: collectionType.kind,
					element: collectionType.element,
					length: sourceBinding.length
				});
		}
	}

	function registerCollectionLocal(variable:TVar, ordinal:Int, localId:String, span:Bool):Void {
		final request = new CSymbolRequest(CSKLocal, input.declarationPath.split(".").concat([input.fieldName, variable.name]),
			CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
		context.symbols.register(request);
		localRequests.set(localId, request);
		if (span) {
			final lengthRequest = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, variable.name, "length"]),
				CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
			context.symbols.register(lengthRequest);
			spanLengthRequests.set(localId, lengthRequest);
		}
	}

	function requireArrayLiteral(expression:TypedExpr, variableName:String):Array<TypedExpr> {
		return switch expression.expr {
			case TArrayDecl(values): values;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): requireArrayLiteral(inner, variableName);
			case _: unsupported(expression, 'TVar($variableName:fixed-array-requires-direct-literal)');
		};
	}

	function fixedArrayZeroLengthExpression(expression:TypedExpr):Null<TypedExpr> {
		return switch expression.expr {
			case TCall(callee, arguments) if (isAbstractMethod(callee, "c.CArray", "zero")):
				if (arguments.length != 1) {
					unsupported(expression, 'TCall(c.CArray.zero:argument-count=${arguments.length})');
				}
				arguments[0];
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): fixedArrayZeroLengthExpression(inner);
			case _: null;
		};
	}

	function requireFixedArrayZeroLength(expression:TypedExpr):Int {
		final length = foldFixedArrayLength(expression, expression, []);
		if (length <= 0) {
			unsupported(expression, 'TCall(c.CArray.zero:length-must-be-positive:$length)');
		}
		return length;
	}

	function foldFixedArrayLength(expression:TypedExpr, anchor:TypedExpr, activeInlineFields:Array<String>):Int {
		return switch expression.expr {
			case TConst(TInt(value)): value;
			case TBinop(OpMult, left, right):
				final leftValue = foldFixedArrayLength(left, anchor, activeInlineFields);
				final rightValue = foldFixedArrayLength(right, anchor, activeInlineFields);
				if (leftValue <= 0 || rightValue <= 0) {
					unsupported(anchor, 'TCall(c.CArray.zero:length-must-be-positive-product:$leftValue*$rightValue)');
				}
				if (leftValue > Std.int(2147483647 / rightValue)) {
					unsupported(anchor, 'TCall(c.CArray.zero:length-product-overflow:$leftValue*$rightValue)');
				}
				leftValue * rightValue;
			case TField(_, FStatic(classReference, fieldReference)):
				final field = fieldReference.get();
				final owner = classReference.get();
				final fieldId = owner.pack.concat([owner.name, field.name]).join(".");
				switch field.kind {
					case FVar(AccInline, _):
						if (activeInlineFields.indexOf(fieldId) != -1) {
							unsupported(anchor, 'TCall(c.CArray.zero:recursive-inline-length:$fieldId)');
						}
						final value = field.expr();
						if (value == null) {
							unsupported(anchor, 'TCall(c.CArray.zero:inline-length-without-value:$fieldId)');
						}
						foldFixedArrayLength(value, anchor, activeInlineFields.concat([fieldId]));
					case _:
						unsupported(anchor, 'TCall(c.CArray.zero:length-must-be-compile-time-product:${nodeName(expression)})');
				}
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): foldFixedArrayLength(inner, anchor, activeInlineFields);
			case TUnop(OpNeg, _, _): unsupported(anchor, 'TCall(c.CArray.zero:length-must-be-positive)');
			case _: unsupported(anchor, 'TCall(c.CArray.zero:length-must-be-compile-time-product:${nodeName(expression)})');
		};
	}

	function requireFixedArrayZeroStorage(element:HxcIRTypeRef, length:Int, expression:TypedExpr):Void {
		switch HxcIRFixedArrayPolicy.zeroStorage(element, length) {
			case IRFASAutomatic(_, _):
			case IRFASInvalidLength(invalidLength):
				unsupported(expression, 'TCall(c.CArray.zero:length-must-be-positive:$invalidLength)');
			case IRFASUnsupportedElement:
				unsupported(expression, 'TCall(c.CArray.zero:element-requires-exact-storage-size:${typeKey(element)})');
			case IRFASSizeOverflow(elementBytes, invalidLength):
				unsupported(expression, 'TCall(c.CArray.zero:storage-size-overflow:$invalidLength*$elementBytes)');
			case IRFASOverBudget(elementBytes, totalBytes, maximumBytes):
				unsupported(expression,
					'TCall(c.CArray.zero:automatic-storage-over-budget:length=$length,element-bytes=$elementBytes,total-bytes=$totalBytes,limit-bytes=$maximumBytes)');
		}
	}

	function requireSpanSource(expression:TypedExpr, mutable:Bool):TVar {
		return switch expression.expr {
			case TCall(callee, [argument]) if (isAbstractMethod(callee, "c.CArray", mutable ? "span" : "constSpan")):
				switch argument.expr {
					case TLocal(variable): variable;
					case TParenthesis(inner) | TMeta(_,
						inner) | TCast(inner, _): requireLocalVariable(inner, 'TCall(${mutable ? "span" : "constSpan"}:source)');
					case _: unsupported(argument, 'TCall(${mutable ? "span" : "constSpan"}:source=${nodeName(argument)})');
				}
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): requireSpanSource(inner, mutable);
			case _: unsupported(expression, 'TVar(${mutable ? "Span" : "ConstSpan"}:requires-fixed-array-borrow)');
		};
	}

	function requireLocalVariable(expression:TypedExpr, owner:String):TVar {
		return switch expression.expr {
			case TLocal(variable): variable;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): requireLocalVariable(inner, owner);
			case _: unsupported(expression, '$owner:source=${nodeName(expression)}');
		};
	}

	function followingSwitchInitializesLocal(expression:TypedExpr, compilerId:Int):Bool {
		return switch expression.expr {
			case TSwitch(_, cases, defaultExpression): switchArmsAssignLocal(cases, defaultExpression, compilerId);
			case TParenthesis(inner) | TMeta(_, inner): followingSwitchInitializesLocal(inner, compilerId);
			case _: false;
		};
	}

	function definitelyAssignsLocal(expression:TypedExpr, compilerId:Int):Bool {
		return switch expression.expr {
			case TBinop(OpAssign, left, _):
				isLocalTarget(left, compilerId);
			case TBlock(expressions): expressions.length > 0 && definitelyAssignsLocal(expressions[expressions.length - 1], compilerId);
			case TIf(_, whenTrue, whenFalse): whenFalse != null && definitelyAssignsLocal(whenTrue,
					compilerId) && definitelyAssignsLocal(whenFalse, compilerId);
			case TSwitch(_, cases, defaultExpression): switchArmsAssignLocal(cases, defaultExpression, compilerId);
			case TParenthesis(inner) | TMeta(_, inner): definitelyAssignsLocal(inner, compilerId);
			case _: false;
		};
	}

	function switchArmsAssignLocal(cases:Array<TypedSwitchArm>, defaultExpression:Null<TypedExpr>, compilerId:Int):Bool {
		if (defaultExpression == null || !definitelyAssignsLocal(defaultExpression, compilerId)) {
			return false;
		}
		for (item in cases) {
			if (!definitelyAssignsLocal(item.expr, compilerId)) {
				return false;
			}
		}
		return true;
	}

	function isLocalTarget(expression:TypedExpr, compilerId:Int):Bool {
		return switch expression.expr {
			case TLocal(variable): variable.id == compilerId;
			case TParenthesis(inner) | TMeta(_, inner): isLocalTarget(inner, compilerId);
			case _: false;
		};
	}

	function lowerReturn(value:Null<TypedExpr>, position:Position):Void {
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		if (value == null) {
			currentBlock.terminator = {kind: IRTReturn(null, normalCleanupSteps()), source: source};
			return;
		}
		if (referencesStackConstructedValue(value))
			unsupported(value, "TNew(stack-reference-escape:return)");
		if (prepared.returnMapping.irType == IRTVoid) {
			switch value.expr {
				case TCall(_, _):
					lowerCall(value, false);
				case _:
					unsupported(value, "TReturn(value-for-Void)");
			}
			currentBlock.terminator = {kind: IRTReturn(null, normalCleanupSteps()), source: source};
			return;
		}
		final lowered = coerce(lowerValue(value, prepared.returnMapping), prepared.returnMapping, value.pos, "TReturn(value)");
		currentBlock.terminator = {kind: IRTReturn(lowered.id, normalCleanupSteps()), source: source};
	}

	function lowerValue(expression:TypedExpr, ?expectedMapping:CBodyValueType):LoweredValue {
		return switch expression.expr {
			case TConst(constant): lowerConstant(expression, constant, expectedMapping);
			case TLocal(variable): lowerLocal(expression, variable);
			case TArray(_, _): loadPlace(lowerPlace(expression), expression.pos, "collection-index-load");
			case TObjectDecl(fields): lowerAggregateLiteral(expression, fields, expectedMapping);
			case TField(_, FEnum(enumReference, enumField)):
				final imported = aggregateRegistry.importEnumConstant(enumReference, enumField, expression.pos, input.sourcePath);
				imported == null ? lowerEnumConstructor(expression, enumReference, enumField, [],
					expectedMapping) : lowerImportConstant(expression, imported, expectedMapping);
			case TField(receiver, FAnon(fieldReference)): lowerAggregateField(expression, receiver, fieldReference.get().name);
			case TField(receiver, FInstance(_, _, fieldReference)):
				final receiverType = bodyValueType(receiver.t, receiver.pos, 'TField(${fieldReference.get().name}:receiver-type)');
				receiverType.importedStructValue() == null ? lowerClassField(expression, receiver,
					fieldReference.get().name) : lowerImportedField(expression, receiver, fieldReference.get().name, receiverType);
			case TField(_, FStatic(classReference, fieldReference)):
				final imported = aggregateRegistry.importStaticConstant(classReference, fieldReference, expression.pos, input.sourcePath);
				imported == null ? lowerStaticField(expression, classReference, fieldReference) : lowerImportConstant(expression, imported, expectedMapping);
			case TParenthesis(inner): lowerValue(inner, expectedMapping);
			case TMeta(_, inner): lowerValue(inner, expectedMapping);
			case TBlock(expressions): lowerValueBlock(expression, expressions, expectedMapping);
			case TCast(inner, _):
				final target = bodyValueType(expression.t, expression.pos, "TCast(target-type)");
				switch target.kind {
					case CBVKPrimitive(primitive):
						switch tryLowerUIntIntrinsic(expression, inner, primitive) {
							case UIIntrinsicLowered(value): value;
							case UIIntrinsicNotMatched: coerce(lowerValue(inner), target, expression.pos, "TCast");
						}
					case CBVKSpan(_, _) | CBVKCString | CBVKImport(_) | CBVKAggregate(_) | CBVKEnum(_) | CBVKClass(_, _):
						coerce(lowerValue(inner, target), target, expression.pos, "TCast(record-alias)");
				}
			case TCall(callee, arguments) if (enumConstructor(callee) != null):
				final constructor = enumConstructor(callee);
				if (constructor == null)
					return unsupported(expression, "TCall(enum-constructor-lost)");
				lowerEnumConstructor(expression, constructor.reference, constructor.field, arguments, expectedMapping);
			case TCall(_, _):
				final result = lowerCall(expression, true);
				if (result == null) {
					unsupported(expression, "TCall(Void-used-as-value)");
				}
				result;
			case TBinop(OpAssign, left, right): lowerAssignment(expression, left, right);
			case TBinop(OpAssignOp(operation), left, right): lowerCompoundAssignment(expression, operation, left, right);
			case TBinop(OpBoolAnd, left, right): lowerLazyBoolean(expression, left, right, false);
			case TBinop(OpBoolOr, left, right): lowerLazyBoolean(expression, left, right, true);
			case TBinop(operation, left, right): lowerBinary(expression, operation, left, right);
			case TUnop(OpIncrement, postFix, target): lowerUpdate(expression, target, postFix, true);
			case TUnop(OpDecrement, postFix, target): lowerUpdate(expression, target, postFix, false);
			case TUnop(operation, _, operand): lowerUnary(expression, operation, operand);
			case TIf(condition, whenTrue, whenFalse): lowerConditional(expression, condition, whenTrue, whenFalse, expectedMapping);
			case TSwitch(subject, cases, defaultExpression):
				lowerValueSwitch(expression, subject, cases, defaultExpression, expectedMapping);
			case TEnumParameter(receiver, enumField, payloadIndex):
				lowerEnumParameter(expression, receiver, enumField, payloadIndex);
			case TNew(_, _, _): unsupported(expression, "TNew(stack-construction-requires-direct-local)");
			case _: unsupported(expression, nodeName(expression));
		};
	}

	function lowerImportConstant(expression:TypedExpr, constant:reflaxe.c.interop.CImportRegistry.CPreparedImportConstant,
			expectedMapping:Null<CBodyValueType>):LoweredValue {
		final result:HxcIRResult = {id: nextValueId(), type: constant.type.irType};
		appendInstruction(result, IRIOConstant(IRCNativeConstant(constant.id)), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
			"native-constant");
		final value:LoweredValue = {id: result.id, type: result.type, mapping: constant.type};
		return expectedMapping == null ? value : coerce(value, expectedMapping, expression.pos, "native-constant-context");
	}

	function lowerImportedField(expression:TypedExpr, receiver:TypedExpr, fieldName:String, receiverType:CBodyValueType):LoweredValue {
		final imported = receiverType.importedStructValue();
		if (imported == null)
			return unsupported(expression, 'TField($fieldName:receiver-not-imported-struct)');
		final field = imported.field(fieldName);
		if (field == null)
			return unsupported(expression, 'TField($fieldName:unknown-imported-field)');
		final expressionType = bodyValueType(expression.t, expression.pos, 'TField($fieldName:imported-result-type)');
		if (typeKey(expressionType.irType) != typeKey(field.type.irType))
			return unsupported(expression, 'TField($fieldName:imported-result-mismatch)');
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final addressableBase = importedReadPlace(receiver);
		if (addressableBase != null) {
			final pointer:HxcIRResult = {id: nextValueId(), type: IRTPointer(field.type.irType, false)};
			appendInstruction(pointer, IRIOAddress(IRPField(addressableBase, fieldName)), source, "imported-field-address");
			registerValueTemporary(pointer.id, "imported-field-address");
			return loadPlace({place: IRPDereference(pointer.id), mapping: field.type, mutable: field.mutable}, expression.pos, "imported-field-load");
		}
		final receiverValue = coerce(lowerValue(receiver, receiverType), receiverType, receiver.pos, 'TField($fieldName:imported-receiver)');
		final result:HxcIRResult = {id: nextValueId(), type: field.type.irType};
		appendInstruction(result, IRIOProject(receiverValue.id, fieldName), source, "imported-field-project");
		registerValueTemporary(result.id, "imported-field-project");
		return {id: result.id, type: result.type, mapping: field.type};
	}

	function importedReadPlace(expression:TypedExpr):Null<HxcIRPlace> {
		return switch expression.expr {
			case TLocal(variable): final localId = localIdsByCompilerId.get(variable.id); final localType = localTypesByCompilerId.get(variable.id); localId != null && localType != null && localType.importedStructValue() != null ? IRPLocal(localId) : null;
			case TField(base, FInstance(_, _, fieldReference)):
				final basePlace = importedReadPlace(base);
				basePlace == null ? null : IRPField(basePlace, fieldReference.get().name);
			case TParenthesis(inner) | TMeta(_, inner): importedReadPlace(inner);
			case _: null;
		};
	}

	function lowerEnumConstructor(expression:TypedExpr, enumReference:Ref<EnumType>, enumField:EnumField, arguments:Array<TypedExpr>,
			expectedMapping:Null<CBodyValueType>):LoweredValue {
		final mapping = bodyValueType(expression.t, expression.pos, 'enum-constructor:${enumField.name}:type');
		final value = mapping.enumValue();
		if (value == null) {
			return unsupported(expression, 'enum-constructor:${enumField.name}:non-enum-result');
		}
		final owner = enumReference.get();
		if (owner.pack.concat([owner.name]).join(".") != value.haxePath) {
			return unsupported(expression, 'enum-constructor:${enumField.name}:owner-type-mismatch');
		}
		final tagCase = value.tagCase(enumField.name);
		if (tagCase == null) {
			return unsupported(expression, 'enum-constructor:${enumField.name}:unknown-case');
		}
		if (arguments.length != tagCase.payload.length) {
			return unsupported(expression, 'enum-constructor:${enumField.name}:argument-count=${arguments.length},expected=${tagCase.payload.length}');
		}
		final payloadIds:Array<String> = [];
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		for (index in 0...arguments.length) {
			final payload = tagCase.payload[index];
			final argument = arguments[index];
			final lowered = coerce(lowerValue(argument, payload.valueType), payload.valueType, argument.pos,
				'enum-constructor:${enumField.name}:payload:$index');
			if (payload.indirect) {
				final stableLocalId = createFlowLocal(payload.valueType, lowered.id, HaxeSourceSpan.fromPosition(argument.pos, input.sourcePath),
					'enum-recursive-payload-$index');
				final pointer:HxcIRResult = {id: nextValueId(), type: payload.storageType()};
				appendInstruction(pointer, IRIOAddress(IRPLocal(stableLocalId)), HaxeSourceSpan.fromPosition(argument.pos, input.sourcePath),
					"enum-recursive-payload-address");
				registerValueTemporary(pointer.id, "enum-recursive-payload-address");
				payloadIds.push(pointer.id);
			} else {
				payloadIds.push(lowered.id);
			}
		}
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOConstructTag(value.instanceId, tagCase.name, payloadIds), source, "construct-enum");
		registerValueTemporary(result.id, "enum-result");
		final lowered:LoweredValue = {id: result.id, type: result.type, mapping: mapping};
		return expectedMapping == null ? lowered : coerce(lowered, expectedMapping, expression.pos, "enum-constructor:contextual-type");
	}

	function lowerEnumParameter(expression:TypedExpr, receiver:TypedExpr, enumField:EnumField, payloadIndex:Int):LoweredValue {
		final receiverMapping = bodyValueType(receiver.t, receiver.pos, 'TEnumParameter(${enumField.name}:receiver-type)');
		final value = receiverMapping.enumValue();
		if (value == null) {
			return unsupported(expression, 'TEnumParameter(${enumField.name}:receiver-not-enum)');
		}
		final tagCase = value.tagCase(enumField.name);
		if (tagCase == null || payloadIndex < 0 || payloadIndex >= tagCase.payload.length) {
			return unsupported(expression, 'TEnumParameter(${enumField.name}:payload-index=$payloadIndex)');
		}
		final payload = tagCase.payload[payloadIndex];
		final expressionMapping = bodyValueType(expression.t, expression.pos, 'TEnumParameter(${enumField.name}:result-type)');
		if (typeKey(expressionMapping.irType) != typeKey(payload.valueType.irType)) {
			return unsupported(expression, 'TEnumParameter(${enumField.name}:typed-result-mismatch)');
		}
		final receiverValue = coerce(lowerValue(receiver, receiverMapping), receiverMapping, receiver.pos, 'TEnumParameter(${enumField.name}:receiver)');
		final result:HxcIRResult = {id: nextValueId(), type: payload.storageType()};
		appendInstruction(result,
			IRIOProjectTag(receiverValue.id, tagCase.name, payloadIndex, IRTCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "enum-payload-project");
		registerValueTemporary(result.id, "enum-payload-project");
		if (payload.indirect) {
			return loadPlace({place: IRPDereference(result.id), mapping: payload.valueType, mutable: false}, expression.pos, "enum-recursive-payload-load");
		}
		return {id: result.id, type: result.type, mapping: payload.valueType};
	}

	function lowerAggregateLiteral(expression:TypedExpr, fields:Array<{name:String, expr:TypedExpr}>, expectedMapping:Null<CBodyValueType>):LoweredValue {
		final mapping = bodyValueType(expression.t, expression.pos, "TObjectDecl(type)");
		final aggregate = mapping.aggregateValue();
		if (aggregate == null) {
			return unsupported(expression, "TObjectDecl(non-aggregate-type)");
		}
		final valuesByName:Map<String, String> = [];
		for (field in fields) {
			if (valuesByName.exists(field.name)) {
				return unsupported(field.expr, 'TObjectDecl(duplicate-field:${field.name})');
			}
			final expectedField = preparedAggregateField(aggregate, field.name);
			if (expectedField == null) {
				return unsupported(field.expr, 'TObjectDecl(unknown-field:${field.name})');
			}
			final value = coerce(lowerValue(field.expr, expectedField.type), expectedField.type, field.expr.pos, 'TObjectDecl(field:${field.name})');
			valuesByName.set(field.name, value.id);
		}
		final namedValues:Array<HxcIRNamedValue> = [];
		for (field in aggregate.fields) {
			final valueId = valuesByName.get(field.name);
			if (valueId == null) {
				return unsupported(expression, 'TObjectDecl(missing-field:${field.name})');
			}
			namedValues.push({name: field.name, valueId: valueId});
		}
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOConstructAggregate(aggregate.instanceId, namedValues), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
			"construct-record");
		registerValueTemporary(result.id, "record-result");
		final lowered:LoweredValue = {id: result.id, type: result.type, mapping: mapping};
		return expectedMapping == null ? lowered : coerce(lowered, expectedMapping, expression.pos, "TObjectDecl(contextual-type)");
	}

	function lowerAggregateField(expression:TypedExpr, receiver:TypedExpr, fieldName:String):LoweredValue {
		final receiverType = bodyValueType(receiver.t, receiver.pos, 'TField($fieldName:receiver-type)');
		final aggregate = receiverType.aggregateValue();
		if (aggregate == null) {
			return unsupported(expression, 'TField($fieldName:receiver-not-closed-record)');
		}
		final field = preparedAggregateField(aggregate, fieldName);
		if (field == null) {
			return unsupported(expression, 'TField($fieldName:unknown-record-field)');
		}
		final expressionType = bodyValueType(expression.t, expression.pos, 'TField($fieldName:result-type)');
		if (typeKey(expressionType.irType) != typeKey(field.type.irType)) {
			return unsupported(expression, 'TField($fieldName:typed-result-mismatch)');
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final addressableBase = aggregateReadPlace(receiver);
		if (addressableBase != null) {
			// A compiler-owned record local is already a stable C place. Reading its
			// field does not require manufacturing `&record.field` and then loading
			// through that pointer. Keeping the field place structural also lets the
			// value-coalescing proof emit `record.field` at one safe use.
			return loadPlace({place: IRPField(addressableBase, fieldName), mapping: field.type, mutable: false}, expression.pos, "record-field-load");
		}
		final receiverValue = coerce(lowerValue(receiver, receiverType), receiverType, receiver.pos, 'TField($fieldName:receiver)');
		final result:HxcIRResult = {id: nextValueId(), type: field.type.irType};
		appendInstruction(result, IRIOProject(receiverValue.id, fieldName), source, "record-field-project");
		registerValueTemporary(result.id, "record-field-project");
		return {id: result.id, type: result.type, mapping: field.type};
	}

	function aggregateReadPlace(expression:TypedExpr):Null<HxcIRPlace> {
		return switch expression.expr {
			case TLocal(variable): final localId = localIdsByCompilerId.get(variable.id); final localType = localTypesByCompilerId.get(variable.id); localId != null && localType != null && localType.aggregateValue() != null ? IRPLocal(localId) : null;
			case TField(base, FAnon(fieldReference)):
				final basePlace = aggregateReadPlace(base);
				basePlace == null ? null : IRPField(basePlace, fieldReference.get().name);
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): aggregateReadPlace(inner);
			case _: null;
		};
	}

	static function preparedAggregateField(aggregate:CPreparedBodyAggregate, name:String):Null<CPreparedBodyAggregateField> {
		for (field in aggregate.fields) {
			if (field.name == name) {
				return field;
			}
		}
		return null;
	}

	function lowerClassField(expression:TypedExpr, receiver:TypedExpr, fieldName:String):LoweredValue {
		final receiverType = bodyValueType(receiver.t, receiver.pos, 'TField($fieldName:receiver-class-type)');
		final classValue = receiverType.classValue();
		if (classValue == null)
			return unsupported(expression, 'TField($fieldName:receiver-not-concrete-class-reference)');
		final field = classValue.field(fieldName);
		if (field == null)
			return unsupported(expression, 'TField($fieldName:unknown-class-storage-field)');
		final expressionType = bodyValueType(expression.t, expression.pos, 'TField($fieldName:result-type)');
		if (typeKey(expressionType.irType) != typeKey(field.type.irType))
			return unsupported(expression, 'TField($fieldName:typed-result-mismatch)');
		final receiverValue = lowerValue(receiver);
		if (receiverValue.mapping.classValue() == null)
			return unsupported(expression, 'TField($fieldName:receiver-value-not-class-reference)');
		if (isNullableClassReference(receiverValue.type)) {
			appendInstruction(null, IRIONullCheck(receiverValue.id, IRNCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))),
				HaxeSourceSpan.fromPosition(receiver.pos, input.sourcePath), "class-field-null-check");
		}
		return loadPlace({
			place: IRPField(IRPDereference(receiverValue.id), fieldName),
			mapping: field.type,
			mutable: field.mutable
		}, expression.pos, "class-field-load");
	}

	function lowerStatementConditional(expression:TypedExpr, condition:TypedExpr, whenTrue:TypedExpr, whenFalse:Null<TypedExpr>):Void {
		final conditionValue = lowerBooleanCondition(condition, "TIf");
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final trueBlock = createGeneratedBlock("if-true", source);
		if (whenFalse == null) {
			final joinBlock = createGeneratedBlock("if-join", source);
			currentBlock.terminator = {kind: IRTBranch(conditionValue.id, edge(trueBlock.id), edge(joinBlock.id)), source: source};
			currentBlock = trueBlock;
			lowerStatement(whenTrue);
			if (currentBlock.terminator == null) {
				currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
			}
			currentBlock = joinBlock;
			return;
		}

		final falseExpression:TypedExpr = whenFalse;
		final falseBlock = createGeneratedBlock("if-false", source);
		final dispatchBlock = currentBlock;
		dispatchBlock.terminator = {kind: IRTBranch(conditionValue.id, edge(trueBlock.id), edge(falseBlock.id)), source: source};

		currentBlock = trueBlock;
		lowerStatement(whenTrue);
		final trueEnd = currentBlock;

		currentBlock = falseBlock;
		lowerStatement(falseExpression);
		final falseEnd = currentBlock;

		if (trueEnd.terminator == null || falseEnd.terminator == null) {
			final joinBlock = createGeneratedBlock("if-join", source);
			if (trueEnd.terminator == null) {
				trueEnd.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
			}
			if (falseEnd.terminator == null) {
				falseEnd.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
			}
			currentBlock = joinBlock;
		} else {
			// Retain a terminated arm as the current point so a following source
			// expression receives the stable unreachable-source diagnostic.
			currentBlock = falseEnd;
		}
	}

	function lowerLoop(expression:TypedExpr, condition:TypedExpr, body:TypedExpr, normalWhile:Bool):Void {
		if (normalWhile) {
			lowerPreTestLoop(expression, condition, body);
		} else {
			lowerPostTestLoop(expression, condition, body);
		}
	}

	function lowerPreTestLoop(expression:TypedExpr, condition:TypedExpr, body:TypedExpr):Void {
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final conditionBlock = createGeneratedBlock("while-condition", source);
		final bodyBlock = createGeneratedBlock("while-body", source);
		final exitBlock = createGeneratedBlock("while-exit", source);
		currentBlock.terminator = {kind: IRTJump(edge(conditionBlock.id)), source: source};

		currentBlock = conditionBlock;
		final conditionValue = lowerBooleanCondition(condition, "TWhile");
		currentBlock.terminator = {kind: IRTBranch(conditionValue.id, edge(bodyBlock.id), edge(exitBlock.id)), source: source};

		final control = loopControl(exitBlock.id, conditionBlock.id);
		loopControlStack.push(control);
		currentBlock = bodyBlock;
		lowerStatement(body);
		loopControlStack.pop();
		if (currentBlock.terminator == null) {
			currentBlock.terminator = {kind: IRTJump(edge(conditionBlock.id)), source: source};
		}
		currentBlock = exitBlock;
	}

	function lowerPostTestLoop(expression:TypedExpr, condition:TypedExpr, body:TypedExpr):Void {
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final bodyBlock = createGeneratedBlock("do-body", source);
		final conditionBlock = reserveGeneratedBlock("do-condition", source);
		final exitBlock = reserveGeneratedBlock("do-exit", source);
		currentBlock.terminator = {kind: IRTJump(edge(bodyBlock.id)), source: source};

		final control = loopControl(exitBlock.id, conditionBlock.id);
		loopControlStack.push(control);
		currentBlock = bodyBlock;
		lowerStatement(body);
		loopControlStack.pop();
		final bodyEnd = currentBlock;
		final reachesCondition = bodyEnd.terminator == null || control.usedContinue;
		if (bodyEnd.terminator == null) {
			bodyEnd.terminator = {kind: IRTJump(edge(conditionBlock.id)), source: source};
		}

		if (reachesCondition) {
			activateGeneratedBlock(conditionBlock);
			activateGeneratedBlock(exitBlock);
			currentBlock = conditionBlock;
			final conditionValue = lowerBooleanCondition(condition, "TWhile");
			currentBlock.terminator = {kind: IRTBranch(conditionValue.id, edge(bodyBlock.id), edge(exitBlock.id)), source: source};
			currentBlock = exitBlock;
		} else if (control.usedBreak) {
			activateGeneratedBlock(exitBlock);
			currentBlock = exitBlock;
		} else {
			// An unconditional return/throw from the body makes both the condition
			// and loop exit unreachable; do not emit unused C labels for them.
			currentBlock = bodyEnd;
		}
	}

	function lowerLoopJump(expression:TypedExpr, isBreak:Bool):Void {
		if (loopControlStack.length == 0) {
			unsupported(expression, isBreak ? "TBreak(outside-loop)" : "TContinue(outside-loop)");
		}
		final control = loopControlStack[loopControlStack.length - 1];
		if (isBreak) {
			control.usedBreak = true;
		} else {
			control.usedContinue = true;
		}
		final target = isBreak ? control.breakTargetBlockId : control.continueTargetBlockId;
		currentBlock.terminator = {
			kind: IRTJump(edge(target)),
			source: HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath)
		};
	}

	function lowerStatementSwitch(expression:TypedExpr, subject:TypedExpr, cases:Array<TypedSwitchArm>, defaultExpression:Null<TypedExpr>):Void {
		final enumSubject = enumIndexSubject(subject);
		if (enumSubject != null) {
			lowerStatementEnumSwitch(expression, enumSubject, cases, defaultExpression);
			return;
		}
		final subjectValue = lowerSwitchSubject(subject);
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final dispatchBlock = currentBlock;
		final caseBlocks:Array<MutableBodyBlock> = [];
		for (index in 0...cases.length) {
			caseBlocks.push(createGeneratedBlock('switch-case-$index', source));
		}
		final defaultBlock = defaultExpression == null ? null : createGeneratedBlock("switch-default", source);
		final openEnds:Array<MutableBodyBlock> = [];

		for (index in 0...cases.length) {
			currentBlock = caseBlocks[index];
			lowerStatement(cases[index].expr);
			if (currentBlock.terminator == null) {
				openEnds.push(currentBlock);
			}
		}
		if (defaultExpression != null && defaultBlock != null) {
			currentBlock = defaultBlock;
			lowerStatement(defaultExpression);
			if (currentBlock.terminator == null) {
				openEnds.push(currentBlock);
			}
		}

		final needsExit = defaultExpression == null || openEnds.length > 0;
		final exitBlock = needsExit ? createGeneratedBlock("switch-exit", source) : null;
		if (exitBlock != null) {
			for (end in openEnds) {
				end.terminator = {kind: IRTJump(edge(exitBlock.id)), source: source};
			}
		}
		final irCases = switchCases(cases, caseBlocks, requirePrimitive(subjectValue.mapping, expression.pos, "TSwitch(subject)"));
		final defaultTarget = defaultBlock != null ? defaultBlock.id : requireBlock(exitBlock, "switch without default").id;
		dispatchBlock.terminator = {kind: IRTSwitch(subjectValue.id, irCases, edge(defaultTarget)), source: source};
		if (exitBlock != null) {
			currentBlock = exitBlock;
		} else if (defaultBlock != null) {
			currentBlock = defaultBlock;
		} else if (caseBlocks.length > 0) {
			currentBlock = caseBlocks[caseBlocks.length - 1];
		} else {
			throw new CBodyEmissionError('switch in `${prepared.irId}` has no continuation block');
		}
	}

	function lowerStatementEnumSwitch(expression:TypedExpr, subject:TypedExpr, cases:Array<TypedSwitchArm>, defaultExpression:Null<TypedExpr>):Void {
		final subjectValue = lowerEnumSwitchSubject(subject);
		final enumValue = subjectValue.mapping.enumValue();
		if (enumValue == null)
			return unsupported(subject, "TSwitch(enum-subject-lost)");
		if (defaultExpression == null && enumSwitchCoveredCaseCount(cases, enumValue) != enumValue.cases.length) {
			return unsupported(expression, "TSwitch(non-exhaustive-enum-statement-without-default)");
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final dispatchBlock = currentBlock;
		final caseBlocks:Array<MutableBodyBlock> = [];
		for (index in 0...cases.length)
			caseBlocks.push(createGeneratedBlock('enum-switch-case-$index', source));
		final defaultBlock = defaultExpression == null ? null : createGeneratedBlock("enum-switch-default", source);
		final openEnds:Array<MutableBodyBlock> = [];
		for (index in 0...cases.length) {
			currentBlock = caseBlocks[index];
			lowerStatement(cases[index].expr);
			if (currentBlock.terminator == null)
				openEnds.push(currentBlock);
		}
		if (defaultExpression != null && defaultBlock != null) {
			currentBlock = defaultBlock;
			lowerStatement(defaultExpression);
			if (currentBlock.terminator == null)
				openEnds.push(currentBlock);
		}
		final needsExit = openEnds.length > 0;
		final exitBlock = needsExit ? createGeneratedBlock("enum-switch-exit", source) : null;
		if (exitBlock != null) {
			for (end in openEnds)
				end.terminator = {kind: IRTJump(edge(exitBlock.id)), source: source};
		}
		dispatchBlock.terminator = {
			kind: IRTTagSwitch(subjectValue.id, enumSwitchCases(cases, caseBlocks, enumValue), defaultBlock == null ? null : edge(defaultBlock.id)),
			source: source
		};
		if (exitBlock != null) {
			currentBlock = exitBlock;
		} else if (defaultBlock != null) {
			currentBlock = defaultBlock;
		} else if (caseBlocks.length > 0) {
			currentBlock = caseBlocks[caseBlocks.length - 1];
		} else {
			throw new CBodyEmissionError('enum switch in `${prepared.irId}` has no continuation block');
		}
	}

	function lowerValueSwitch(expression:TypedExpr, subject:TypedExpr, cases:Array<TypedSwitchArm>, defaultExpression:Null<TypedExpr>,
			expectedMapping:Null<CBodyValueType>):LoweredValue {
		final enumSubject = enumIndexSubject(subject);
		if (enumSubject != null) {
			return lowerValueEnumSwitch(expression, enumSubject, cases, defaultExpression, expectedMapping);
		}
		if (defaultExpression == null) {
			return unsupported(expression, "TSwitch(value-without-default)");
		}
		final subjectValue = lowerSwitchSubject(subject);
		final resultMapping = expectedMapping == null ? bodyValueType(expression.t, expression.pos, "TSwitch(result-type)") : expectedMapping;
		requirePrimitive(resultMapping, expression.pos, "TSwitch(result-type)");
		if (resultMapping.irType == IRTVoid) {
			return unsupported(expression, "TSwitch(Void-as-value)");
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final initialResult:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
		appendInstruction(initialResult, IRIOConstant(defaultConstant(resultMapping.irType, expression, "TSwitch")), source, "switch-default-result");
		final resultLocalId = createFlowLocal(resultMapping, initialResult.id, source, "switch-result");
		final dispatchBlock = currentBlock;
		final caseBlocks:Array<MutableBodyBlock> = [];
		for (index in 0...cases.length) {
			caseBlocks.push(createGeneratedBlock('switch-value-case-$index', source));
		}
		final defaultBlock = createGeneratedBlock("switch-value-default", source);
		final joinBlock = createGeneratedBlock("switch-value-join", source);

		for (index in 0...cases.length) {
			currentBlock = caseBlocks[index];
			final value = coerce(lowerValue(cases[index].expr, resultMapping), resultMapping, cases[index].expr.pos, "TSwitch(case-value)");
			appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), value.id), source, "switch-case-store");
			currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
		}

		final resolvedDefault:TypedExpr = defaultExpression;
		currentBlock = defaultBlock;
		final defaultValue = coerce(lowerValue(resolvedDefault, resultMapping), resultMapping, resolvedDefault.pos, "TSwitch(default-value)");
		appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), defaultValue.id), source, "switch-default-store");
		currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};

		dispatchBlock.terminator = {
			kind: IRTSwitch(subjectValue.id, switchCases(cases, caseBlocks, requirePrimitive(subjectValue.mapping, expression.pos, "TSwitch(subject)")),
				edge(defaultBlock.id)),
			source: source
		};
		currentBlock = joinBlock;
		return loadPlace({place: IRPLocal(resultLocalId), mapping: resultMapping, mutable: true}, expression.pos, "switch-result-load");
	}

	function lowerValueEnumSwitch(expression:TypedExpr, subject:TypedExpr, cases:Array<TypedSwitchArm>, defaultExpression:Null<TypedExpr>,
			expectedMapping:Null<CBodyValueType>):LoweredValue {
		final subjectValue = lowerEnumSwitchSubject(subject);
		final enumValue = subjectValue.mapping.enumValue();
		if (enumValue == null)
			return unsupported(subject, "TSwitch(enum-subject-lost)");
		if (defaultExpression == null && enumSwitchCoveredCaseCount(cases, enumValue) != enumValue.cases.length) {
			return unsupported(expression, "TSwitch(non-exhaustive-enum-value-without-default)");
		}
		final resultMapping = expectedMapping == null ? bodyValueType(expression.t, expression.pos, "TSwitch(enum-result-type)") : expectedMapping;
		requirePrimitive(resultMapping, expression.pos, "TSwitch(enum-result-type)");
		if (resultMapping.irType == IRTVoid)
			return unsupported(expression, "TSwitch(enum-Void-as-value)");
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final initialResult:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
		appendInstruction(initialResult, IRIOConstant(defaultConstant(resultMapping.irType, expression, "TSwitch(enum)")), source,
			"enum-switch-default-result");
		final resultLocalId = createFlowLocal(resultMapping, initialResult.id, source, "enum-switch-result");
		final dispatchBlock = currentBlock;
		final caseBlocks:Array<MutableBodyBlock> = [];
		for (index in 0...cases.length)
			caseBlocks.push(createGeneratedBlock('enum-switch-value-case-$index', source));
		final defaultBlock = defaultExpression == null ? null : createGeneratedBlock("enum-switch-value-default", source);
		final joinBlock = createGeneratedBlock("enum-switch-value-join", source);
		for (index in 0...cases.length) {
			currentBlock = caseBlocks[index];
			final value = coerce(lowerValue(cases[index].expr, resultMapping), resultMapping, cases[index].expr.pos, "TSwitch(enum-case-value)");
			appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), value.id), source, "enum-switch-case-store");
			currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
		}
		if (defaultExpression != null && defaultBlock != null) {
			currentBlock = defaultBlock;
			final defaultValue = coerce(lowerValue(defaultExpression, resultMapping), resultMapping, defaultExpression.pos, "TSwitch(enum-default-value)");
			appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), defaultValue.id), source, "enum-switch-default-store");
			currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
		}
		dispatchBlock.terminator = {
			kind: IRTTagSwitch(subjectValue.id, enumSwitchCases(cases, caseBlocks, enumValue), defaultBlock == null ? null : edge(defaultBlock.id)),
			source: source
		};
		currentBlock = joinBlock;
		return loadPlace({place: IRPLocal(resultLocalId), mapping: resultMapping, mutable: true}, expression.pos, "enum-switch-result-load");
	}

	function lowerEnumSwitchSubject(expression:TypedExpr):LoweredValue {
		final mapping = bodyValueType(expression.t, expression.pos, "TSwitch(enum-subject-type)");
		if (mapping.enumValue() == null)
			unsupported(expression, "TSwitch(enum-subject-not-enum)");
		return coerce(lowerValue(expression, mapping), mapping, expression.pos, "TSwitch(enum-subject)");
	}

	function enumSwitchCases(cases:Array<TypedSwitchArm>, blocks:Array<MutableBodyBlock>, value:CPreparedBodyEnumInstance):Array<HxcIRTagSwitchCase> {
		final result:Array<HxcIRTagSwitchCase> = [];
		for (index in 0...cases.length) {
			for (caseValue in cases[index].values) {
				result.push({tagName: enumSwitchCase(caseValue, value).name, edge: edge(blocks[index].id)});
			}
		}
		return result;
	}

	function enumSwitchCoveredCaseCount(cases:Array<TypedSwitchArm>, value:CPreparedBodyEnumInstance):Int {
		final names:Map<String, Bool> = [];
		for (item in cases)
			for (caseValue in item.values)
				names.set(enumSwitchCase(caseValue, value).name, true);
		return [for (_ in names.keys()) 1].length;
	}

	function enumSwitchCase(expression:TypedExpr, value:CPreparedBodyEnumInstance):CPreparedBodyEnumCase {
		final index = switch expression.expr {
			case TConst(TInt(tagValue)): tagValue;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): return enumSwitchCase(inner, value);
			case _: return unsupported(expression, 'TSwitch(enum-case=${nodeName(expression)}:requires-compiler-tag-index)');
		};
		for (tagCase in value.cases)
			if (tagCase.tagValue == index)
				return tagCase;
		return unsupported(expression, 'TSwitch(enum-case-index=$index:outside-${value.haxePath})');
	}

	static function enumIndexSubject(expression:TypedExpr):Null<TypedExpr> {
		return switch expression.expr {
			case TEnumIndex(value): value;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): enumIndexSubject(inner);
			case _: null;
		};
	}

	function lowerSwitchSubject(expression:TypedExpr):LoweredValue {
		final mapping = primitiveMapping(expression.t, expression.pos, "TSwitch(subject-type)");
		switch mapping.irType {
			case IRTBool | IRTInt(_, _):
			case _:
				unsupported(expression, 'TSwitch(non-integral-subject:${mapping.cSpelling})');
		}
		final bodyType = CBodyValueType.primitive(mapping);
		return coerce(lowerValue(expression, bodyType), bodyType, expression.pos, "TSwitch(subject)");
	}

	function switchCases(cases:Array<TypedSwitchArm>, blocks:Array<MutableBodyBlock>, mapping:CPrimitiveTypeMapping):Array<HxcIRSwitchCase> {
		final result:Array<HxcIRSwitchCase> = [];
		for (index in 0...cases.length) {
			for (value in cases[index].values) {
				result.push({value: switchConstant(value, mapping), edge: edge(blocks[index].id)});
			}
		}
		return result;
	}

	function switchConstant(expression:TypedExpr, mapping:CPrimitiveTypeMapping):HxcIRConstant {
		return switch expression.expr {
			case TConst(TInt(value)):
				switch mapping.irType {
					case IRTInt(_, _): IRCInt(Std.string(value));
					case _: unsupported(expression, "TSwitch(integer-case-for-non-integer-subject)");
				}
			case TConst(TBool(value)):
				mapping.irType == IRTBool ? IRCBool(value) : unsupported(expression, "TSwitch(boolean-case-for-non-Bool-subject)");
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _):
				switchConstant(inner, mapping);
			case _:
				unsupported(expression, 'TSwitch(case=${nodeName(expression)}:requires-typed-primitive-constant)');
		};
	}

	function lowerBooleanCondition(expression:TypedExpr, owner:String):LoweredValue {
		final boolMapping = primitiveMapping(expression.t, expression.pos, '$owner(condition-type)');
		if (boolMapping.irType != IRTBool) {
			unsupported(expression, '$owner(non-Bool-condition)');
		}
		final boolType = CBodyValueType.primitive(boolMapping);
		return coerce(lowerValue(expression, boolType), boolType, expression.pos, '$owner(condition)');
	}

	static function loopControl(breakTargetBlockId:String, continueTargetBlockId:String):LoopControlTargets
		return {
			breakTargetBlockId: breakTargetBlockId,
			continueTargetBlockId: continueTargetBlockId,
			usedBreak: false,
			usedContinue: false
		};

	function lowerValueBlock(expression:TypedExpr, expressions:Array<TypedExpr>, expectedMapping:Null<CBodyValueType>):LoweredValue {
		if (expressions.length == 0) {
			return unsupported(expression, "TBlock(empty-as-value)");
		}
		final lastIndex = expressions.length - 1;
		for (index in 0...lastIndex) {
			lowerStatement(expressions[index]);
		}
		return lowerValue(expressions[lastIndex], expectedMapping);
	}

	function lowerConstant(expression:TypedExpr, constant:TConstant, expectedMapping:Null<CBodyValueType>):LoweredValue {
		if (constant == TThis) {
			final self = selfValue;
			if (self == null)
				return unsupported(expression, "TThis(outside-constructor)");
			return expectedMapping == null ? self : coerce(self, expectedMapping, expression.pos, "TThis(contextual-type)");
		}
		if (constant == TNull) {
			final mapping = expectedMapping == null ? bodyValueType(expression.t, expression.pos, "TConst(TNull:type)") : expectedMapping;
			if (mapping.classValue() == null)
				return unsupported(expression, "TConst(TNull:requires-concrete-class-reference-context)");
			final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
			appendInstruction(result, IRIOConstant(IRCNull), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "null-class-reference");
			return {id: result.id, type: result.type, mapping: mapping};
		}
		final inferredMapping = primitiveMapping(expression.t, expression.pos, nodeName(expression));
		final expectedPrimitive = expectedMapping == null ? null : expectedMapping.primitiveMapping();
		final mapping = contextualConstantMapping(constant, inferredMapping, expectedPrimitive);
		final type = mapping.irType;
		final value:HxcIRConstant = switch constant {
			case TInt(value):
				requireConstantType(type, expression, "integer", valueType -> switch valueType {
					case IRTInt(_, _): true;
					case _: false;
				});
				if (!integerConstantFits(value, type))
					unsupported(expression, 'TConst(integer-out-of-range:$value:${mapping.cSpelling})');
				IRCInt(Std.string(value));
			case TFloat(value):
				requireConstantType(type, expression, "floating", valueType -> switch valueType {
					case IRTFloat(64): true;
					case _: false;
				});
				IRCFloat(canonicalFloat(value, expression));
			case TBool(value):
				requireConstantType(type, expression, "boolean", valueType -> valueType == IRTBool);
				IRCBool(value);
			case TString(_) | TNull | TThis | TSuper:
				unsupported(expression, nodeName(expression));
		};
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOConstant(value), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "constant");
		return {id: result.id, type: result.type, mapping: CBodyValueType.primitive(mapping)};
	}

	static function integerConstantFits(value:Int, type:HxcIRTypeRef):Bool {
		return switch type {
			case IRTInt(width, signed):
				if (width >= 32) {
					signed
					|| value >= 0;
				} else if (signed) {
					final limit = 1 << (width - 1);
					value >= -limit && value < limit;
				} else {
					value >= 0 && value < (1 << width)
					;
				}
			case _: false;
		};
	}

	static function contextualConstantMapping(constant:TConstant, inferred:CPrimitiveTypeMapping, expected:Null<CPrimitiveTypeMapping>):CPrimitiveTypeMapping {
		if (expected == null) {
			return inferred;
		}
		return switch constant {
			case TInt(_):
				switch inferred.irType {
					case IRTInt(_, _):
						switch expected.irType {
							case IRTInt(_, _): expected;
							case _: inferred;
						}
					case _: inferred;
				}
			case _: inferred;
		};
	}

	function lowerLocal(expression:TypedExpr, variable:TVar):LoweredValue {
		final parameter = parameterValuesByCompilerId.get(variable.id);
		if (parameter != null) {
			return parameter;
		}
		final collection = collectionBindingsByCompilerId.get(variable.id);
		if (collection != null) {
			switch collection.kind {
				case BCKSpan(mutable):
					final mapping = CBodyValueType.span(collection.element, mutable);
					final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
					appendInstruction(result, IRIOLoad(IRPLocal(collection.localId)), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
						"load-span-borrow");
					return {id: result.id, type: result.type, mapping: mapping};
				case BCKFixedArray(_):
					return unsupported(expression, 'TLocal(${variable.name}:fixed-array-value-escape)');
			}
		}
		final localId = localIdsByCompilerId.get(variable.id);
		if (localId == null) {
			return unsupported(expression, 'TLocal(${variable.name}:outside-admitted-body)');
		}
		final mapping = localTypesByCompilerId.get(variable.id);
		if (mapping == null) {
			return unsupported(expression, 'TLocal(${variable.name}:missing-admitted-type)');
		}
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOLoad(IRPLocal(localId)), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "load");
		registerValueTemporary(result.id, "load-result");
		return {id: result.id, type: result.type, mapping: mapping};
	}

	function lowerStaticField(expression:TypedExpr, classReference:Ref<ClassType>, fieldReference:Ref<ClassField>):LoweredValue {
		final global = globalRegistry.require(classReference, fieldReference, expression, rejectGlobal);
		return loadPlace({place: IRPGlobal(global.ir.id), mapping: CBodyValueType.primitive(global.mapping), mutable: global.ir.mutable}, expression.pos,
			"global-load");
	}

	function lowerAssignment(expression:TypedExpr, left:TypedExpr, right:TypedExpr):LoweredValue {
		if (newExpression(right) != null)
			unsupported(right, "TNew(stack-construction-requires-direct-local)");
		if (referencesStackConstructedValue(right))
			unsupported(right, "TNew(stack-reference-escape:assignment)");
		final target = lowerPlace(left);
		if (!target.mutable) {
			unsupported(left, "TBinop(OpAssign:immutable-place)");
		}
		final source = lowerValue(right, target.mapping);
		final value = coerce(source, target.mapping, right.pos, "TBinop(OpAssign:right)");
		appendInstruction(null, IRIOStore(target.place, value.id), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "store");
		return value;
	}

	function lowerCompoundAssignment(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr):LoweredValue {
		final target = lowerPlace(left);
		if (!target.mutable) {
			unsupported(left, "TBinop(OpAssignOp:immutable-place)");
		}
		final oldValue = loadPlace(target, left.pos, "compound-load");
		final oldValueLocal = expressionCreatesFlow(right) ? createFlowLocal(oldValue.mapping, oldValue.id,
			HaxeSourceSpan.fromPosition(left.pos, input.sourcePath), "compound-left") : null;
		final rightValue = lowerValue(right);
		final stableOldValue = oldValueLocal == null ? oldValue : loadPlace({place: IRPLocal(oldValueLocal), mapping: oldValue.mapping, mutable: true},
			left.pos, "compound-left-load");
		final nextValue = lowerBinaryValues(expression, operation, stableOldValue, rightValue, "compound", target.mapping);
		final stored = coerce(nextValue, target.mapping, expression.pos, "TBinop(OpAssignOp:result)");
		appendInstruction(null, IRIOStore(target.place, stored.id), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "compound-store");
		return stored;
	}

	function lowerBinary(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr):LoweredValue {
		if (operation == OpEq || operation == OpNotEq) {
			final leftMapping = isNullConstantExpression(left) ? null : bodyValueType(left.t, left.pos, "TBinop(class-equality:left-type)");
			final rightMapping = isNullConstantExpression(right) ? null : bodyValueType(right.t, right.pos, "TBinop(class-equality:right-type)");
			if (leftMapping != null && leftMapping.classValue() != null || rightMapping != null && rightMapping.classValue() != null) {
				return lowerClassEquality(expression, operation, left, right, leftMapping, rightMapping);
			}
		}
		final leftValue = lowerValue(left);
		final leftValueLocal = expressionCreatesFlow(right) ? createFlowLocal(leftValue.mapping, leftValue.id,
			HaxeSourceSpan.fromPosition(left.pos, input.sourcePath), "binary-left") : null;
		final rightValue = lowerValue(right);
		final stableLeftValue = leftValueLocal == null ? leftValue : loadPlace({place: IRPLocal(leftValueLocal), mapping: leftValue.mapping, mutable: true},
			left.pos, "binary-left-load");
		return lowerBinaryValues(expression, operation, stableLeftValue, rightValue, "binary");
	}

	function lowerClassEquality(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr, leftMapping:Null<CBodyValueType>,
			rightMapping:Null<CBodyValueType>):LoweredValue {
		if (leftMapping == null && rightMapping == null)
			return unsupported(expression, "TBinop(class-reference-equality-without-nominal-type)");
		final target:CBodyValueType = if (leftMapping == null) {
			rightMapping;
		} else if (rightMapping == null) {
			leftMapping;
		} else {
			final leftClass = leftMapping.classValue();
			final rightClass = rightMapping.classValue();
			if (leftClass == null || rightClass == null)
				return unsupported(expression, "TBinop(class-reference-equality-mixed-value-category)");
			if (leftClass.isDescendantOf(rightClass)) {
				rightMapping;
			} else if (rightClass.isDescendantOf(leftClass)) {
				leftMapping;
			} else {
				return unsupported(expression, 'TBinop(unrelated-class-reference-equality:${leftClass.haxePath}->${rightClass.haxePath})');
			}
		};
		if (target.classValue() == null)
			return unsupported(expression, "TBinop(class-reference-equality-target-not-class)");
		final leftValue = coerce(lowerValue(left, target), target, left.pos, "TBinop(class-reference-equality:left)");
		final leftValueLocal = expressionCreatesFlow(right) ? createFlowLocal(target, leftValue.id, HaxeSourceSpan.fromPosition(left.pos, input.sourcePath),
			"class-equality-left") : null;
		final rightValue = coerce(lowerValue(right, target), target, right.pos, "TBinop(class-reference-equality:right)");
		final stableLeft = leftValueLocal == null ? leftValue : loadPlace({place: IRPLocal(leftValueLocal), mapping: target, mutable: true}, left.pos,
			"class-equality-left-load");
		final boolMapping = bodyValueType(expression.t, expression.pos, "TBinop(class-reference-equality:result-type)");
		if (boolMapping.irType != IRTBool)
			return unsupported(expression, "TBinop(class-reference-equality:result-not-Bool)");
		final result:HxcIRResult = {id: nextValueId(), type: IRTBool};
		appendInstruction(result,
			IRIOBinary(operation == OpEq ? "haxe.class-reference.equal" : "haxe.class-reference.not-equal", stableLeft.id, rightValue.id, IRIStatic),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "class-reference-equality");
		return {id: result.id, type: result.type, mapping: boolMapping};
	}

	static function isNullConstantExpression(expression:TypedExpr):Bool {
		return switch expression.expr {
			case TConst(TNull): true;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isNullConstantExpression(inner);
			case _: false;
		};
	}

	function tryLowerUIntIntrinsic(expression:TypedExpr, inner:TypedExpr, target:CPrimitiveTypeMapping):UIntIntrinsicResult {
		if (target.sourceType != CPHaxeUInt || target.nullability != CPNonNullable) {
			return UIIntrinsicNotMatched;
		}
		final candidate = unwrapPatternExpression(inner);
		switch candidate.expr {
			case TBinop(operation, left, right):
				final leftUInt = extractUIntBitValue(left);
				if (leftUInt == null) {
					return UIIntrinsicNotMatched;
				}
				final rightSource = switch operation {
					case OpShl | OpShr | OpUShr: right;
					case OpAdd | OpSub | OpMult | OpAnd | OpOr | OpXor:
						extractUIntBitValue(right);
					case _:
						null;
				};
				if (rightSource == null) {
					return UIIntrinsicNotMatched;
				}
				return UIIntrinsicLowered(lowerUIntBinary(expression, operation, leftUInt, rightSource, target));
			case TCall(callee, arguments) if (isStdInt(callee) && arguments.length == 1):
				final modulo = unwrapPatternExpression(arguments[0]);
				switch modulo.expr {
					case TBinop(OpMod, left, right):
						final leftUInt = extractUIntFloatValue(left);
						final rightUInt = extractUIntFloatValue(right);
						if (leftUInt != null && rightUInt != null) {
							return UIIntrinsicLowered(lowerUIntBinary(expression, OpMod, leftUInt, rightUInt, target));
						}
					case _:
				}
			case _:
		}
		return UIIntrinsicNotMatched;
	}

	function lowerUIntBinary(expression:TypedExpr, operation:Binop, leftExpression:TypedExpr, rightExpression:TypedExpr,
			target:CPrimitiveTypeMapping):LoweredValue {
		final leftValue = lowerValue(leftExpression);
		final rightValue = lowerValue(rightExpression);
		return lowerBinaryValues(expression, operation, leftValue, rightValue, "uint-intrinsic", CBodyValueType.primitive(target));
	}

	function extractUIntBitValue(expression:TypedExpr):Null<TypedExpr> {
		final candidate = unwrapPatternExpression(expression);
		final mapping = switch CPrimitiveTypeMapper.map(applyCurrentSpecialization(candidate.t), context.profile) {
			case CTPrimitive(value): value;
			case _: return null;
		};
		return mapping.sourceType == CPHaxeUInt && mapping.nullability == CPNonNullable ? candidate : null;
	}

	/** Recognize the pinned `UInt.toFloat()` expansion without treating arbitrary blocks as intrinsics. */
	function extractUIntFloatValue(expression:TypedExpr):Null<TypedExpr> {
		final candidate = unwrapPatternExpression(expression);
		final expressions = switch candidate.expr {
			case TBlock(values) if (values.length == 2): values;
			case _: return null;
		};
		final declaration = switch expressions[0].expr {
			case TVar(variable, initializer) if (initializer != null): {variable: variable, initializer: initializer};
			case _: return null;
		};
		final source = extractUIntBitValue(declaration.initializer);
		if (source == null) {
			return null;
		}
		final branches = switch expressions[1].expr {
			case TIf(condition, whenTrue, whenFalse) if (whenFalse != null): {condition: condition, whenTrue: whenTrue, whenFalse: whenFalse};
			case _: return null;
		};
		if (!matchesNegativeLocalTest(branches.condition, declaration.variable.id)
			|| !matchesUIntFloatNegativeBranch(branches.whenTrue, declaration.variable.id)
			|| !matchesUIntFloatNonNegativeBranch(branches.whenFalse, declaration.variable.id)) {
			return null;
		}
		return source;
	}

	function matchesNegativeLocalTest(expression:TypedExpr, variableId:Int):Bool {
		final candidate = unwrapPatternExpression(expression);
		return switch candidate.expr {
			case TBinop(OpLt, left, right): isLocal(left, variableId) && isIntegerConstant(right, "0");
			case _: false;
		};
	}

	function matchesUIntFloatNegativeBranch(expression:TypedExpr, variableId:Int):Bool {
		final candidate = unwrapValueBlock(expression);
		return switch candidate.expr {
			case TBinop(OpAdd, left, right): isFloatConstant(left, "4294967296") && isLocal(right, variableId);
			case _: false;
		};
	}

	function matchesUIntFloatNonNegativeBranch(expression:TypedExpr, variableId:Int):Bool {
		final candidate = unwrapValueBlock(expression);
		return switch candidate.expr {
			case TBinop(OpAdd, left, right): isLocal(left, variableId) && isFloatConstant(right, "0");
			case _: false;
		};
	}

	function unwrapValueBlock(expression:TypedExpr):TypedExpr {
		final candidate = unwrapPatternExpression(expression);
		return switch candidate.expr {
			case TBlock(expressions) if (expressions.length == 1): unwrapValueBlock(expressions[0]);
			case _: candidate;
		};
	}

	function unwrapPatternExpression(expression:TypedExpr):TypedExpr {
		return switch expression.expr {
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): unwrapPatternExpression(inner);
			case _: expression;
		};
	}

	static function isLocal(expression:TypedExpr, variableId:Int):Bool {
		return switch expression.expr {
			case TLocal(variable): variable.id == variableId;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isLocal(inner, variableId);
			case _: false;
		};
	}

	static function isIntegerConstant(expression:TypedExpr, expected:String):Bool {
		return switch expression.expr {
			case TConst(TInt(value)): Std.string(value) == expected;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isIntegerConstant(inner, expected);
			case _: false;
		};
	}

	static function isFloatConstant(expression:TypedExpr, expectedWhole:String):Bool {
		return switch expression.expr {
			case TConst(TFloat(value)): final lower = value.toLowerCase(); lower == expectedWhole || lower == expectedWhole + ".0";
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isFloatConstant(inner, expectedWhole);
			case _: false;
		};
	}

	function lowerBinaryValues(expression:TypedExpr, operation:Binop, leftValue:LoweredValue, rightValue:LoweredValue, role:String,
			?expectedResult:CBodyValueType):LoweredValue {
		final semanticOperation = primitiveBinaryOperator(operation, expression);
		final resultType = expectedResult == null ? bodyValueType(expression.t, expression.pos, 'TBinop($operation:result-type)') : expectedResult;
		final leftPrimitive = requirePrimitive(leftValue.mapping, expression.pos, 'TBinop($operation:left-type)');
		final rightPrimitive = requirePrimitive(rightValue.mapping, expression.pos, 'TBinop($operation:right-type)');
		final resultMapping = requirePrimitive(resultType, expression.pos, 'TBinop($operation:result-type)');
		final decision = switch CPrimitiveSemantics.binaryOperation(semanticOperation, leftPrimitive, rightPrimitive, resultMapping) {
			case CPBOperationAllowed(value): value;
			case CPBOperationRejected(reason): return unsupported(expression, 'TBinop($operation:$reason)');
		};
		switch decision.implementation {
			case IRIStatic | IRIProgramLocal(_):
			case IRIRuntime(featureId):
				return unsupported(expression, 'TBinop($operation:primitive-operation-must-not-use-runtime:$featureId)');
		}
		final left = coerce(leftValue, CBodyValueType.primitive(decision.leftOperand), expression.pos, 'TBinop($operation:left)');
		final right = coerce(rightValue, CBodyValueType.primitive(decision.rightOperand), expression.pos, 'TBinop($operation:right)');
		final result:HxcIRResult = {id: nextValueId(), type: decision.result.irType};
		appendInstruction(result, IRIOBinary(decision.operationId, left.id, right.id, decision.implementation),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), role);
		return {id: result.id, type: result.type, mapping: CBodyValueType.primitive(decision.result)};
	}

	function lowerUnary(expression:TypedExpr, operation:Unop, operandExpression:TypedExpr):LoweredValue {
		final semanticOperation = switch operation {
			case OpNeg: CPUONegate;
			case OpNegBits: CPUOBitwiseNot;
			case OpNot: CPUOLogicalNot;
			case _: return unsupported(expression, 'TUnop($operation)');
		};
		final operandValue = lowerValue(operandExpression);
		final resultMapping = primitiveMapping(expression.t, expression.pos, 'TUnop($operation:result-type)');
		final operandMapping = requirePrimitive(operandValue.mapping, expression.pos, 'TUnop($operation:operand-type)');
		final decision = switch CPrimitiveSemantics.unaryOperation(semanticOperation, operandMapping, resultMapping) {
			case CPUOperationAllowed(value): value;
			case CPUOperationRejected(reason): return unsupported(expression, 'TUnop($operation:$reason)');
		};
		switch decision.implementation {
			case IRIStatic | IRIProgramLocal(_):
			case IRIRuntime(featureId):
				return unsupported(expression, 'TUnop($operation:primitive-operation-must-not-use-runtime:$featureId)');
		}
		final operand = coerce(operandValue, CBodyValueType.primitive(decision.operand), expression.pos, 'TUnop($operation:operand)');
		final result:HxcIRResult = {id: nextValueId(), type: decision.result.irType};
		appendInstruction(result, IRIOUnary(decision.operationId, operand.id, decision.implementation),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "unary");
		return {id: result.id, type: result.type, mapping: CBodyValueType.primitive(decision.result)};
	}

	function lowerUpdate(expression:TypedExpr, targetExpression:TypedExpr, postFix:Bool, increment:Bool):LoweredValue {
		final target = lowerPlace(targetExpression);
		if (!target.mutable) {
			unsupported(targetExpression, 'TUnop(${increment ? "OpIncrement" : "OpDecrement"}:immutable-place)');
		}
		final role = increment ? "increment" : "decrement";
		final oldValue = loadPlace(target, targetExpression.pos, role + "-load");
		final oneResult:HxcIRResult = {id: nextValueId(), type: target.mapping.irType};
		final one = switch target.mapping.irType {
			case IRTInt(_, _): IRCInt("1");
			case IRTFloat(64): IRCFloat("1.0");
			case _: return unsupported(expression, 'TUnop(${increment ? "OpIncrement" : "OpDecrement"}:non-numeric)');
		};
		appendInstruction(oneResult, IRIOConstant(one), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), role + "-one");
		final oneValue:LoweredValue = {id: oneResult.id, type: oneResult.type, mapping: target.mapping};
		final nextValue = lowerBinaryValues(expression, increment ? OpAdd : OpSub, oldValue, oneValue, role, target.mapping);
		appendInstruction(null, IRIOStore(target.place, nextValue.id), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), role + "-store");
		return postFix ? oldValue : nextValue;
	}

	function lowerLazyBoolean(expression:TypedExpr, left:TypedExpr, right:TypedExpr, shortCircuitWhenTrue:Bool):LoweredValue {
		final boolMapping = primitiveMapping(expression.t, expression.pos, "TBinop(short-circuit:result-type)");
		if (boolMapping.irType != IRTBool) {
			unsupported(expression, "TBinop(short-circuit:non-Bool-result)");
		}
		final boolType = CBodyValueType.primitive(boolMapping);
		final leftValue = coerce(lowerValue(left), boolType, left.pos, "TBinop(short-circuit:left)");
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final resultLocalId = createFlowLocal(boolType, leftValue.id, source, "short-circuit-result");
		final rhsBlock = createGeneratedBlock("short-circuit-rhs", source);
		final joinBlock = createGeneratedBlock("short-circuit-join", source);
		currentBlock.terminator = {
			kind: IRTBranch(leftValue.id, edge(shortCircuitWhenTrue ? joinBlock.id : rhsBlock.id), edge(shortCircuitWhenTrue ? rhsBlock.id : joinBlock.id)),
			source: source
		};
		currentBlock = rhsBlock;
		final rightValue = coerce(lowerValue(right), boolType, right.pos, "TBinop(short-circuit:right)");
		appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), rightValue.id), source, "short-circuit-store");
		currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
		currentBlock = joinBlock;
		return loadPlace({place: IRPLocal(resultLocalId), mapping: boolType, mutable: true}, expression.pos, "short-circuit-load");
	}

	function lowerConditional(expression:TypedExpr, condition:TypedExpr, whenTrue:TypedExpr, whenFalse:Null<TypedExpr>,
			expectedMapping:Null<CBodyValueType>):LoweredValue {
		final falseExpression = whenFalse;
		if (falseExpression == null) {
			return unsupported(expression, "TIf(without-else-as-value)");
		}
		final conditionValue = lowerBooleanCondition(condition, "TIf");
		final resultMapping = expectedMapping == null ? bodyValueType(expression.t, expression.pos, "TIf(result-type)") : expectedMapping;
		requirePrimitive(resultMapping, expression.pos, "TIf(result-type)");
		if (resultMapping.irType == IRTVoid) {
			return unsupported(expression,
				'TIf(Void-as-value:${expectedMapping == null ? "typed-expression" : "contextual"}:function-return=${prepared.returnMapping.cSpelling})');
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final defaultResult:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
		appendInstruction(defaultResult, IRIOConstant(defaultConstant(resultMapping.irType, expression, "TIf")), source, "conditional-default");
		final resultLocalId = createFlowLocal(resultMapping, defaultResult.id, source, "conditional-result");
		final trueBlock = createGeneratedBlock("conditional-true", source);
		final falseBlock = createGeneratedBlock("conditional-false", source);
		final joinBlock = createGeneratedBlock("conditional-join", source);
		currentBlock.terminator = {kind: IRTBranch(conditionValue.id, edge(trueBlock.id), edge(falseBlock.id)), source: source};

		currentBlock = trueBlock;
		final trueValue = coerce(lowerValue(whenTrue, resultMapping), resultMapping, whenTrue.pos, "TIf(true-value)");
		appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), trueValue.id), source, "conditional-true-store");
		currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};

		currentBlock = falseBlock;
		final falseValue = coerce(lowerValue(falseExpression, resultMapping), resultMapping, falseExpression.pos, "TIf(false-value)");
		appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), falseValue.id), source, "conditional-false-store");
		currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};

		currentBlock = joinBlock;
		return loadPlace({place: IRPLocal(resultLocalId), mapping: resultMapping, mutable: true}, expression.pos, "conditional-load");
	}

	function lowerPlace(expression:TypedExpr):LoweredPlace {
		return switch expression.expr {
			case TArray(collection, index): lowerCollectionIndexPlace(expression, collection, index);
			case TLocal(variable):
				if (parameterValuesByCompilerId.exists(variable.id)) {
					unsupported(expression, 'TLocal(${variable.name}:parameter-assignment-not-yet-lowered)');
				}
				final localId = localIdsByCompilerId.get(variable.id);
				if (localId == null) {
					unsupported(expression, 'TLocal(${variable.name}:outside-admitted-body)');
				}
				final localType = localTypesByCompilerId.get(variable.id);
				if (localType == null) {
					return unsupported(expression, 'TLocal(${variable.name}:missing-place-type)');
				}
				{place: IRPLocal(localId), mapping: localType, mutable: true};
			case TField(_, FAnon(fieldReference)):
				unsupported(expression, 'TField(${fieldReference.get().name}:anonymous-field-mutation-requires-identity-preserving-alias-analysis)');
			case TField(receiver, FInstance(_, _, fieldReference)):
				final fieldName = fieldReference.get().name;
				final receiverType = bodyValueType(receiver.t, receiver.pos, 'TField($fieldName:receiver-class-place-type)');
				final imported = receiverType.importedStructValue();
				if (imported != null) {
					final field = imported.field(fieldName);
					if (field == null)
						return unsupported(expression, 'TField($fieldName:unknown-imported-field)');
					final base = importedReadPlace(receiver);
					if (base == null)
						return unsupported(expression, 'TField($fieldName:imported-mutation-requires-addressable-local)');
					return {place: IRPField(base, fieldName), mapping: field.type, mutable: field.mutable};
				}
				final classValue = receiverType.classValue();
				if (classValue == null)
					return unsupported(expression, 'TField($fieldName:receiver-not-concrete-class-reference)');
				final field = classValue.field(fieldName);
				if (field == null)
					return unsupported(expression, 'TField($fieldName:unknown-class-storage-field)');
				final receiverValue = lowerValue(receiver);
				if (receiverValue.mapping.classValue() == null)
					return unsupported(expression, 'TField($fieldName:receiver-place-value-not-class-reference)');
				if (isNullableClassReference(receiverValue.type)) {
					appendInstruction(null, IRIONullCheck(receiverValue.id, IRNCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))),
						HaxeSourceSpan.fromPosition(receiver.pos, input.sourcePath), "class-field-null-check");
				}
				{place: IRPField(IRPDereference(receiverValue.id), fieldName), mapping: field.type, mutable: field.mutable};
			case TField(_, FStatic(classReference, fieldReference)):
				final global = globalRegistry.require(classReference, fieldReference, expression, rejectGlobal);
				{place: IRPGlobal(global.ir.id), mapping: CBodyValueType.primitive(global.mapping), mutable: global.ir.mutable};
			case TParenthesis(inner) | TMeta(_, inner): lowerPlace(inner);
			case _: unsupported(expression, 'place(${nodeName(expression)})');
		};
	}

	function lowerCollectionIndexPlace(expression:TypedExpr, collection:TypedExpr, index:TypedExpr):LoweredPlace {
		final binding = requireCollectionBinding(collection);
		final indexMapping = primitiveMapping(index.t, index.pos, "TArray(index-type)");
		switch indexMapping.irType {
			case IRTInt(32, true):
			case _:
				unsupported(index, "TArray(index-must-be-Int)");
		}
		final indexType = CBodyValueType.primitive(indexMapping);
		final indexValue = coerce(lowerValue(index, indexType), indexType, index.pos, "TArray(index)");
		appendInstruction(null, IRIOBoundsCheck(IRPLocal(binding.localId), indexValue.id, boundsPolicy(binding, index)),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "collection-bounds");
		final mutable = switch binding.kind {
			case BCKFixedArray(_): true;
			case BCKSpan(value): value;
		};
		return {place: IRPIndex(IRPLocal(binding.localId), indexValue.id), mapping: CBodyValueType.primitive(binding.element), mutable: mutable};
	}

	function requireCollectionBinding(expression:TypedExpr):BodyCollectionBinding {
		return switch unwrapExpression(expression).expr {
			case TLocal(variable):
				final binding = collectionBindingsByCompilerId.get(variable.id);
				binding == null ? unsupported(expression, 'TArray(collection-local-outside-admitted-slice:${variable.name})') : binding;
			case _: unsupported(expression, 'TArray(collection=${nodeName(expression)})');
		};
	}

	function boundsPolicy(binding:BodyCollectionBinding, index:TypedExpr):HxcIRBoundsPolicy {
		final value = constantInt(index);
		final knownLength = binding.length;
		if (value != null && knownLength != null) {
			if (value < 0 || value >= knownLength) {
				unsupported(index, 'TArray(index-statically-out-of-bounds:length=$knownLength,index=$value)');
			}
			return IRBPStaticProof(knownLength, value);
		}
		return IRBPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode));
	}

	static function constantInt(expression:TypedExpr):Null<Int> {
		return switch expression.expr {
			case TConst(TInt(value)): value;
			case TUnop(OpNeg, _, inner): final value = constantInt(inner); value == null || value == -2147483648 ? null : -value;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): constantInt(inner);
			case _: null;
		};
	}

	function loadPlace(place:LoweredPlace, position:Position, role:String):LoweredValue {
		final result:HxcIRResult = {id: nextValueId(), type: place.mapping.irType};
		appendInstruction(result, IRIOLoad(place.place), HaxeSourceSpan.fromPosition(position, input.sourcePath), role);
		registerValueTemporary(result.id, role + "-result");
		return {id: result.id, type: result.type, mapping: place.mapping};
	}

	function lowerCall(expression:TypedExpr, materializeResult:Bool):Null<LoweredValue> {
		final call = switch expression.expr {
			case TCall(callee, arguments): {callee: callee, arguments: arguments};
			case _: return unsupported(expression, nodeName(expression));
		};
		if (isSysPrintln(call.callee)) {
			return lowerLiteralOutput(expression, call.arguments, "sys-println-literal", "Sys.println(String literal)", false);
		}
		if (isHaxeLogTrace(call.callee)) {
			return lowerLiteralOutput(expression, call.arguments, "trace-literal", "trace(String literal)", true);
		}
		final integerConversion = integerConversionMode(call.callee);
		if (integerConversion != null) {
			return lowerIntegerConversion(expression, call.arguments, integerConversion);
		}
		final float32Conversion = float32ConversionMode(call.callee);
		if (float32Conversion != null) {
			return lowerFloat32Conversion(expression, call.arguments, float32Conversion);
		}
		if (isAbstractMethod(call.callee, "c.StructInit", "make")) {
			return lowerImportedStructInit(expression, call.arguments);
		}
		if (isStdInt(call.callee)) {
			if (call.arguments.length != 1) {
				return unsupported(expression, 'TCall(Std.int:argument-count=${call.arguments.length})');
			}
			final source = lowerValue(call.arguments[0]);
			final target = primitiveMapping(expression.t, expression.pos, "TCall(Std.int:result-type)");
			final sourceMapping = requirePrimitive(source.mapping, expression.pos, "TCall(Std.int:argument-type)");
			final decision = switch CPrimitiveSemantics.conversion(sourceMapping, target, CPUStdInt) {
				case CPConversionAllowed(value): value;
				case CPConversionElided: return unsupported(expression, "TCall(Std.int:unexpected-elided-conversion)");
				case CPConversionRejected(reason): return unsupported(expression, 'TCall(Std.int:$reason)');
			};
			if (decision.failureRequired) {
				return unsupported(expression, "TCall(Std.int:unexpected-failure-edge)");
			}
			switch decision.implementation {
				case IRIStatic | IRIProgramLocal(_):
				case IRIRuntime(featureId):
					return unsupported(expression, 'TCall(Std.int:primitive-conversion-must-not-use-runtime:$featureId)');
			}
			final result:HxcIRResult = {id: nextValueId(), type: target.irType};
			appendInstruction(result, IRIOConvert(source.id, decision.irKind, target.irType, decision.implementation, null),
				HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "std-int");
			return {id: result.id, type: result.type, mapping: CBodyValueType.primitive(target)};
		}
		final imported = aggregateRegistry.importFunction(call.callee, expression.pos, input.sourcePath);
		if (imported != null)
			return lowerImportCall(expression, call.arguments, imported, materializeResult);
		final instanceAccess = CBodyDispatchCatalog.instanceAccess(call.callee);
		if (instanceAccess != null)
			return lowerInstanceCall(expression, instanceAccess, call.arguments, materializeResult);
		final targetId = directStaticFunctionId(call.callee, call.arguments);
		final target = functionsById.get(targetId);
		if (target == null) {
			return unsupported(expression, 'TCall(unavailable-static-target:$targetId)');
		}
		if (targetId == prepared.irId) {
			for (parameter in target.parameters) {
				if (parameter.mapping.spanElement() != null) {
					return unsupported(expression, 'TCall(recursive-borrowed-span-target-not-admitted:$targetId)');
				}
			}
		}
		if (call.arguments.length != target.parameters.length) {
			return unsupported(expression, 'TCall(argument-count=${call.arguments.length},expected=${target.parameters.length},target=$targetId)');
		}
		final arguments:Array<String> = [];
		for (index in 0...call.arguments.length) {
			final argumentExpression = call.arguments[index];
			if (referencesStackConstructedValue(argumentExpression)) {
				return unsupported(argumentExpression, 'TNew(stack-reference-escape:static-call-argument:$index,target=$targetId)');
			}
			final value = lowerValue(argumentExpression);
			final converted = coerce(value, target.parameters[index].mapping, argumentExpression.pos, 'TCall(argument:$index,target=$targetId)');
			arguments.push(converted.id);
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final returnType = target.returnMapping.irType;
		if (returnType == IRTVoid) {
			final callInstruction = instruction(null, IRIOCall({
				dispatch: IRCDDirect(targetId),
				arguments: arguments,
				returnType: returnType,
				failure: null
			}), source, "call");
			currentBlock.instructions.push(callInstruction);
			registerTailArguments(targetId, callInstruction.id, arguments.length);
			return null;
		}
		final result:HxcIRResult = {id: nextValueId(), type: returnType};
		final callInstruction = instruction(result, IRIOCall({
			dispatch: IRCDDirect(targetId),
			arguments: arguments,
			returnType: returnType,
			failure: null
		}), source, "call");
		currentBlock.instructions.push(callInstruction);
		registerTailArguments(targetId, callInstruction.id, arguments.length);
		if (materializeResult) {
			final ordinal = temporaryOrdinal++;
			final request = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, "call-result"]),
				CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
			temporaryRequests.set(result.id, request);
		}
		return {id: result.id, type: result.type, mapping: target.returnMapping};
	}

	function lowerImportedStructInit(expression:TypedExpr, arguments:Array<TypedExpr>):LoweredValue {
		if (arguments.length != 1) {
			return unsupported(expression, 'TCall(c.StructInit.make:argument-count=${arguments.length})');
		}
		final mapping = bodyValueType(expression.t, expression.pos, "TCall(c.StructInit.make:result-type)");
		final imported = mapping.importedStructValue();
		if (imported == null) {
			return unsupported(expression, "TCall(c.StructInit.make:result-must-be-imported-struct)");
		}
		final fields = switch unwrapExpression(arguments[0]).expr {
			case TObjectDecl(value): value;
			case _: return unsupported(arguments[0], "TCall(c.StructInit.make:requires-direct-object-literal)");
		};
		final valuesByName:Map<String, String> = [];
		for (field in fields) {
			if (valuesByName.exists(field.name)) {
				return unsupported(field.expr, 'TCall(c.StructInit.make:duplicate-field:${field.name})');
			}
			final expectedField = imported.field(field.name);
			if (expectedField == null) {
				return unsupported(field.expr, 'TCall(c.StructInit.make:unknown-field:${field.name})');
			}
			final value = coerce(lowerValue(field.expr, expectedField.type), expectedField.type, field.expr.pos,
				'TCall(c.StructInit.make:field:${field.name})');
			valuesByName.set(field.name, value.id);
		}
		final namedValues:Array<HxcIRNamedValue> = [];
		for (field in imported.fields) {
			final valueId = valuesByName.get(field.name);
			if (valueId == null) {
				return unsupported(arguments[0], 'TCall(c.StructInit.make:missing-field:${field.name})');
			}
			namedValues.push({name: field.name, valueId: valueId});
		}
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOConstructAggregate(imported.instanceId, namedValues), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
			"construct-imported-struct");
		registerValueTemporary(result.id, "imported-struct-result");
		return {id: result.id, type: result.type, mapping: mapping};
	}

	function lowerIntegerConversion(expression:TypedExpr, arguments:Array<TypedExpr>, mode:IntegerConversionMode):LoweredValue {
		final surface = switch mode {
			case ICExact: "c.IntConvert.exact";
			case ICModulo: "c.IntConvert.modulo";
		};
		if (arguments.length != 1) {
			return unsupported(expression, 'TCall($surface:argument-count=${arguments.length})');
		}
		final source = lowerValue(arguments[0]);
		final target = bodyValueType(expression.t, expression.pos, 'TCall($surface:result-type)');
		final sourcePrimitive = source.mapping.primitiveMapping();
		final targetPrimitive = target.primitiveMapping();
		if (sourcePrimitive == null || targetPrimitive == null) {
			return unsupported(expression, 'TCall($surface:requires-primitive-integer-carriers)');
		}
		switch [sourcePrimitive.irType, targetPrimitive.irType] {
			case [IRTInt(_, _), IRTInt(_, _)]:
			case _:
				return unsupported(expression, 'TCall($surface:requires-exact-width-integer-carriers)');
		}
		if (mode == ICModulo && targetPrimitive.signedness != CPSignUnsigned) {
			return unsupported(expression, 'TCall($surface:requires-unsigned-target)');
		}
		return switch CPrimitiveSemantics.conversion(sourcePrimitive, targetPrimitive, CPUWrapping) {
			case CPConversionElided:
				{id: source.id, type: target.irType, mapping: target};
			case CPConversionAllowed(decision):
				if (decision.failureRequired) {
					unsupported(expression, 'TCall($surface:requires-failure-edge)');
				}
				switch decision.implementation {
					case IRIStatic:
					case IRIProgramLocal(helperId):
						unsupported(expression, 'TCall($surface:requires-program-local-helper:$helperId)');
					case IRIRuntime(featureId):
						unsupported(expression, 'TCall($surface:must-not-use-runtime:$featureId)');
				}
				if (mode == ICExact && decision.irKind != IRCNumericExact) {
					unsupported(expression, 'TCall($surface:source-range-not-contained-by-target)');
				}
				final result:HxcIRResult = {id: nextValueId(), type: target.irType};
				appendInstruction(result, IRIOConvert(source.id, decision.irKind, target.irType, IRIStatic, null),
					HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), mode == ICExact ? "integer-conversion-exact" : "integer-conversion-modulo");
				{id: result.id, type: result.type, mapping: target};
			case CPConversionRejected(reason):
				unsupported(expression, 'TCall($surface:unsupported:$reason)');
		};
	}

	function lowerFloat32Conversion(expression:TypedExpr, arguments:Array<TypedExpr>, mode:Float32ConversionMode):LoweredValue {
		final surface = mode == FCNarrow ? "c.Float32.fromFloat" : "c.Float32.toFloat";
		if (arguments.length != 1) {
			return unsupported(expression, 'TCall($surface:argument-count=${arguments.length})');
		}
		final source = lowerValue(arguments[0]);
		final target = bodyValueType(expression.t, expression.pos, 'TCall($surface:result-type)');
		final sourcePrimitive = source.mapping.primitiveMapping();
		final targetPrimitive = target.primitiveMapping();
		if (sourcePrimitive == null || targetPrimitive == null) {
			return unsupported(expression, 'TCall($surface:requires-direct-floating-carriers)');
		}
		final use = mode == FCNarrow ? CPUFloat32Narrow : CPUFloat32Widen;
		return switch CPrimitiveSemantics.conversion(sourcePrimitive, targetPrimitive, use) {
			case CPConversionAllowed(decision):
				if (decision.failureRequired || decision.implementation != IRIStatic) {
					unsupported(expression, 'TCall($surface:must-be-direct-and-infallible)');
				}
				final result:HxcIRResult = {id: nextValueId(), type: target.irType};
				appendInstruction(result, IRIOConvert(source.id, decision.irKind, target.irType, IRIStatic, null),
					HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), mode == FCNarrow ? "float32-narrow" : "float32-widen");
				{id: result.id, type: result.type, mapping: target};
			case CPConversionElided:
				unsupported(expression, 'TCall($surface:unexpected-elided-conversion)');
			case CPConversionRejected(reason):
				unsupported(expression, 'TCall($surface:unsupported:$reason)');
		};
	}

	function lowerImportCall(expression:TypedExpr, argumentExpressions:Array<TypedExpr>, target:CPreparedImportFunction,
			materializeResult:Bool):Null<LoweredValue> {
		if (argumentExpressions.length != target.parameters.length)
			return invalidAbi(expression,
				'Imported C function `${target.haxePath}` expects ${target.parameters.length} argument(s), received ${argumentExpressions.length}.');
		final arguments:Array<String> = [];
		for (index in 0...argumentExpressions.length) {
			final argument = argumentExpressions[index];
			final expected = target.parameters[index];
			final value = expected.isCString() ? lowerCStringLiteral(argument, target,
				index) : coerce(lowerValue(argument, expected), expected, argument.pos, 'native-call:${target.id}:argument:$index');
			arguments.push(value.id);
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		if (target.returnType.irType == IRTVoid) {
			appendInstruction(null, IRIOCall({
				dispatch: IRCDNative(target.id),
				arguments: arguments,
				returnType: IRTVoid,
				failure: null
			}), source, "native-call");
			return null;
		}
		final result:HxcIRResult = {id: nextValueId(), type: target.returnType.irType};
		appendInstruction(result, IRIOCall({
			dispatch: IRCDNative(target.id),
			arguments: arguments,
			returnType: result.type,
			failure: null
		}), source, "native-call");
		if (materializeResult)
			registerValueTemporary(result.id, "native-call-result");
		return {id: result.id, type: result.type, mapping: target.returnType};
	}

	function lowerCStringLiteral(expression:TypedExpr, target:CPreparedImportFunction, argumentIndex:Int):LoweredValue {
		final text = directStringLiteral(expression);
		if (text == null)
			return invalidAbi(expression,
				'Imported C function `${target.haxePath}` argument $argumentIndex requires a direct String literal so its borrowed lifetime is static.');
		if (text.indexOf("\x00") != -1)
			return invalidAbi(expression, 'Imported C function `${target.haxePath}` argument $argumentIndex contains an embedded NUL byte.');
		final byteLength = HxcUtf8.byteLength(text);
		if (byteLength == null)
			return invalidAbi(expression, 'Imported C function `${target.haxePath}` argument $argumentIndex is not valid Unicode-scalar text.');
		final mapping = CBodyValueType.cString();
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOConstant(IRCCStringLiteral(text, byteLength)), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
			"cstring-literal");
		return {id: result.id, type: result.type, mapping: mapping};
	}

	static function directStringLiteral(expression:TypedExpr):Null<String> {
		return switch expression.expr {
			case TConst(TString(value)): value;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): directStringLiteral(inner);
			case _: null;
		};
	}

	function lowerInstanceCall(expression:TypedExpr, access:reflaxe.c.lowering.CBodyDispatch.CBodyInstanceCallAccess, argumentExpressions:Array<TypedExpr>,
			materializeResult:Bool):Null<LoweredValue> {
		final declaration = CBodyDispatchCatalog.declaringClass(access.owner, access.field);
		final field = access.field.get();
		final targetId = CBodyDispatchCatalog.methodIdForAccess(access.owner, access.field);
		final ownerMapping = bodyValueType(TInst(declaration, []), access.receiver.pos, 'TCall(instance:$targetId:receiver-type)');
		if (ownerMapping.classValue() == null)
			return unsupported(expression, 'TCall(instance:$targetId:receiver-not-concrete-class)');
		var receiver = if (CBodyDispatchCatalog.isSuperReceiver(access.receiver)) {
			final self = selfValue;
			self == null ? unsupported(access.receiver, 'TCall(super-method:outside-instance-method:$targetId)') : self;
		} else {
			lowerValue(access.receiver);
		};
		receiver = coerce(receiver, ownerMapping, access.receiver.pos, 'TCall(instance:$targetId:receiver)');

		final directReason = CBodyDispatchCatalog.directReason(access.receiver, declaration, field);
		final explicitMappings:Array<CBodyValueType> = [];
		var returnMapping:CBodyValueType;
		var dispatchKind:HxcIRCallDispatch;
		var directTarget:Null<PreparedBodyFunction> = null;
		if (directReason != null) {
			directTarget = functionsById.get(targetId);
			if (directTarget == null)
				return unsupported(expression, 'TCall(unavailable-instance-target:$targetId)');
			if (directTarget.parameters.length == 0)
				throw new CBodyEmissionError('instance target `$targetId` lost its self parameter');
			for (index in 1...directTarget.parameters.length)
				explicitMappings.push(directTarget.parameters[index].mapping);
			returnMapping = directTarget.returnMapping;
			dispatchKind = IRCDDirect(targetId);
		} else {
			final slot = dispatch.slotForMethodId(targetId);
			if (slot == null)
				return unsupported(expression, 'TCall(unavailable-virtual-slot:$targetId)');
			final slotReceiver = CBodyValueType.classReference(slot.owner, true);
			receiver = coerce(receiver, slotReceiver, access.receiver.pos, 'TCall(instance:$targetId:virtual-receiver)');
			for (mapping in slot.parameters)
				explicitMappings.push(mapping);
			returnMapping = slot.returnType;
			dispatchKind = IRCDVirtual(slot.input.id, receiver.id);
		}
		if (isNullableClassReference(receiver.type)) {
			appendInstruction(null, IRIONullCheck(receiver.id, IRNCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))),
				HaxeSourceSpan.fromPosition(access.receiver.pos, input.sourcePath), "instance-call-null-check");
		}
		if (argumentExpressions.length != explicitMappings.length) {
			return unsupported(expression, 'TCall(instance-argument-count=${argumentExpressions.length},expected=${explicitMappings.length},target=$targetId)');
		}
		final explicitArguments:Array<String> = [];
		for (index in 0...argumentExpressions.length) {
			final argument = argumentExpressions[index];
			if (referencesStackConstructedValue(argument))
				return unsupported(argument, 'TNew(stack-reference-escape:instance-call-argument:$index,target=$targetId)');
			final value = coerce(lowerValue(argument, explicitMappings[index]), explicitMappings[index], argument.pos,
				'TCall(instance-argument:$index,target=$targetId)');
			explicitArguments.push(value.id);
		}
		final callArguments = directReason == null ? explicitArguments : [receiver.id].concat(explicitArguments);
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		if (returnMapping.irType == IRTVoid) {
			final callInstruction = instruction(null, IRIOCall({
				dispatch: dispatchKind,
				arguments: callArguments,
				returnType: IRTVoid,
				failure: null
			}), source, "instance-call");
			currentBlock.instructions.push(callInstruction);
			if (directReason != null)
				registerTailArguments(targetId, callInstruction.id, callArguments.length);
			return null;
		}
		final result:HxcIRResult = {id: nextValueId(), type: returnMapping.irType};
		final callInstruction = instruction(result, IRIOCall({
			dispatch: dispatchKind,
			arguments: callArguments,
			returnType: returnMapping.irType,
			failure: null
		}), source, "instance-call");
		currentBlock.instructions.push(callInstruction);
		if (directReason != null)
			registerTailArguments(targetId, callInstruction.id, callArguments.length);
		if (materializeResult) {
			final ordinal = temporaryOrdinal++;
			final request = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, "instance-call-result"]),
				CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
			temporaryRequests.set(result.id, request);
		}
		return {id: result.id, type: result.type, mapping: returnMapping};
	}

	function lowerLiteralOutput(expression:TypedExpr, arguments:Array<TypedExpr>, operationId:String, surface:String, traceFormatting:Bool):Null<LoweredValue> {
		if (prepared.role != PBRFunction) {
			return unsupported(expression, 'TCall($surface:initializer-output-not-admitted)');
		}
		final expectedArguments = traceFormatting ? 2 : 1;
		if (arguments.length != expectedArguments) {
			return unsupported(expression, 'TCall($surface:argument-count=${arguments.length},expected=$expectedArguments)');
		}
		final literal = stringLiteral(arguments[0]);
		if (literal == null) {
			return unsupported(arguments[0], 'TCall($surface:requires-String-literal)');
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final output = traceFormatting ? traceOutput(literal, arguments[1], source) : literal;
		final byteLength = HxcUtf8.byteLength(output);
		if (byteLength == null) {
			return unsupported(arguments[0], 'TCall($surface:malformed-Unicode-literal)');
		}
		final literalResult:HxcIRResult = {id: nextValueId(), type: IRTString};
		appendInstruction(literalResult, IRIOConstant(IRCString(output, byteLength)), source, "string-literal");
		appendInstruction(null, IRIOCall({
			dispatch: IRCDRuntime("io", operationId),
			arguments: [literalResult.id],
			returnType: IRTVoid,
			failure: {
				kind: IRFNativeStatus,
				target: IRFTAbort,
				arguments: [],
				cleanup: []
			}
		}), source, "hosted-output");
		runtimeRequirements.push(new CBodyRuntimeRequirement("io", operationId, surface, source, expression.pos));
		return null;
	}

	function traceOutput(literal:String, infoExpression:TypedExpr, source:HxcSourceSpan):String {
		final info = unwrapExpression(infoExpression);
		return switch info.expr {
			case TConst(TNull): literal;
			case TObjectDecl(fields):
				if (!isDefaultTraceInfo(fields, source)) {
					unsupported(infoExpression, "TCall(trace(String literal):custom-position-info-not-admitted)");
				}
				'${source.file}:${source.startLine}: $literal';
			case _:
				unsupported(infoExpression, 'TCall(trace(String literal):position-info=${nodeName(infoExpression)})');
		};
	}

	function isDefaultTraceInfo(fields:Array<{name:String, expr:TypedExpr}>, source:HxcSourceSpan):Bool {
		if (fields.length != 4) {
			return false;
		}
		var fileMatches = false;
		var lineMatches = false;
		var classMatches = false;
		var methodMatches = false;
		var unknownField = false;
		for (field in fields) {
			switch field.name {
				case "fileName":
					final injectedFile = stringLiteral(field.expr);
					if (injectedFile != null) {
						final normalized = StringTools.replace(injectedFile, "\\", "/");
						fileMatches = normalized == source.file || StringTools.endsWith(normalized, "/" + source.file);
					}
				case "lineNumber":
					lineMatches = switch unwrapExpression(field.expr).expr {
						case TConst(TInt(value)): value == source.startLine;
						case _: false;
					};
				case "className":
					classMatches = stringLiteral(field.expr) == input.declarationPath;
				case "methodName":
					methodMatches = stringLiteral(field.expr) == input.fieldName;
				case _:
					unknownField = true;
			}
		}
		return fileMatches && lineMatches && classMatches && methodMatches && !unknownField;
	}

	static function stringLiteral(expression:TypedExpr):Null<String> {
		return switch expression.expr {
			case TConst(TString(value)): value;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): stringLiteral(inner);
			case _: null;
		};
	}

	static function isSysPrintln(callee:TypedExpr):Bool
		return isStaticMethod(callee, "", "Sys", "println");

	static function isEnumConstructorExpression(expression:TypedExpr):Bool {
		return switch expression.expr {
			case TField(_, FEnum(_, _)): true;
			case TCall(callee, _): enumConstructor(callee) != null;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isEnumConstructorExpression(inner);
			case _: false;
		};
	}

	static function enumConstructor(callee:TypedExpr):Null<EnumConstructorAccess> {
		return switch callee.expr {
			case TField(_, FEnum(reference, field)): {reference: reference, field: field};
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): enumConstructor(inner);
			case _: null;
		};
	}

	static function isHaxeLogTrace(callee:TypedExpr):Bool
		return isStaticMethod(callee, "haxe", "Log", "trace");

	static function integerConversionMode(callee:TypedExpr):Null<IntegerConversionMode> {
		if (isStaticMethod(callee, "c", "IntConvert", "exact")) {
			return ICExact;
		}
		return isStaticMethod(callee, "c", "IntConvert", "modulo") ? ICModulo : null;
	}

	static function float32ConversionMode(callee:TypedExpr):Null<Float32ConversionMode> {
		if (isAbstractMethod(callee, "c.Float32", "fromFloat")) {
			return FCNarrow;
		}
		return isAbstractMethod(callee, "c.Float32", "toFloat") ? FCWiden : null;
	}

	static function isStaticMethod(callee:TypedExpr, ownerPackage:String, ownerName:String, fieldName:String):Bool {
		return switch callee.expr {
			case TField(_, FStatic(classReference, fieldReference)): final owner = classReference.get(); owner.pack.join(".") == ownerPackage && owner.name == ownerName && fieldReference.get()
					.name == fieldName;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isStaticMethod(inner, ownerPackage, ownerName, fieldName);
			case _: false;
		};
	}

	function isStdInt(callee:TypedExpr):Bool {
		return switch callee.expr {
			case TField(_, FStatic(classReference, fieldReference)): final owner = classReference.get(); owner.pack.length == 0 && owner.name == "Std" && fieldReference.get()
					.name == "int";
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isStdInt(inner);
			case _: false;
		};
	}

	function registerTailArguments(targetId:String, instructionId:String, argumentCount:Int):Void {
		if (targetId != prepared.irId) {
			return;
		}
		final requests:Array<CSymbolRequest> = [];
		for (index in 0...argumentCount) {
			final ordinal = temporaryOrdinal++;
			final request = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, "tail-argument"]),
				CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
			context.symbols.register(request);
			requests.push(request);
		}
		tailArgumentRequests.set(instructionId, requests);
	}

	function registerValueTemporary(valueId:String, role:String):Void {
		if (temporaryRequests.exists(valueId)) {
			return;
		}
		final ordinal = temporaryOrdinal++;
		final request = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, role]),
			CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
		temporaryRequests.set(valueId, request);
	}

	function createFlowLocal(mapping:CBodyValueType, initialValueId:String, source:HxcSourceSpan, role:String):String {
		final ordinal = localOrdinal++;
		final localId = 'local.$ordinal';
		locals.push({
			id: localId,
			type: mapping.irType,
			storage: IRLSAutomatic,
			initialState: IRISUninitialized,
			source: source
		});
		final request = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, role]),
			CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
		context.symbols.register(request);
		localRequests.set(localId, request);
		appendInstruction(null, IRIOInitialize(IRPLocal(localId), initialValueId, IRISUninitialized, IRISInitialized), source, role + "-initialize");
		return localId;
	}

	function createEntryBlock(source:HxcSourceSpan):MutableBodyBlock {
		final block:MutableBodyBlock = {
			id: "entry",
			source: source,
			instructions: [],
			generatedOrdinal: null,
			generatedRole: null,
			active: true,
			terminator: null
		};
		blocks.push(block);
		return block;
	}

	function reserveGeneratedBlock(role:String, source:HxcSourceSpan):MutableBodyBlock {
		final ordinal = blockOrdinal++;
		return {
			id: 'block.$ordinal.$role',
			source: source,
			instructions: [],
			generatedOrdinal: ordinal,
			generatedRole: role,
			active: false,
			terminator: null
		};
	}

	function activateGeneratedBlock(block:MutableBodyBlock):MutableBodyBlock {
		if (block.active || block.generatedOrdinal == null || block.generatedRole == null) {
			throw new CBodyEmissionError('invalid generated block activation `${block.id}` in `${prepared.irId}`');
		}
		block.active = true;
		blocks.push(block);
		final request = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, "block-label", block.generatedRole]),
			CNSLabel(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], block.generatedOrdinal);
		context.symbols.register(request);
		labelRequests.set(block.id, request);
		return block;
	}

	function createGeneratedBlock(role:String, source:HxcSourceSpan):MutableBodyBlock
		return activateGeneratedBlock(reserveGeneratedBlock(role, source));

	static function requireBlock(block:Null<MutableBodyBlock>, context:String):MutableBodyBlock {
		if (block == null) {
			throw new CBodyEmissionError('$context requires a generated continuation block');
		}
		return block;
	}

	static function edge(targetBlockId:String):HxcIRBlockEdge
		return {targetBlockId: targetBlockId, arguments: [], cleanup: []};

	function appendInstruction(result:Null<HxcIRResult>, kind:HxcIRInstructionKind, source:HxcSourceSpan, role:String):HxcIRInstruction {
		final value = instruction(result, kind, source, role);
		currentBlock.instructions.push(value);
		return value;
	}

	function rejectGlobal(position:Position, node:String):Void {
		unsupportedAt(position, node);
	}

	function defaultConstant(type:HxcIRTypeRef, expression:TypedExpr, owner:String):HxcIRConstant {
		return switch type {
			case IRTBool: IRCBool(false);
			case IRTInt(_, _): IRCInt("0");
			case IRTFloat(32) | IRTFloat(64): IRCFloat("0.0");
			case IRTPointer(IRTInstance(_), true): IRCNull;
			case _: unsupported(expression, '$owner(result-type-without-direct-default)');
		};
	}

	function defaultConstantAt(type:HxcIRTypeRef, position:Position, owner:String):HxcIRConstant {
		return switch type {
			case IRTBool: IRCBool(false);
			case IRTInt(_, _): IRCInt("0");
			case IRTFloat(32) | IRTFloat(64): IRCFloat("0.0");
			case IRTPointer(IRTInstance(_), true): IRCNull;
			case _: unsupportedAt(position, '$owner(result-type-without-direct-default)');
		};
	}

	function coerce(value:LoweredValue, target:CBodyValueType, position:Position, node:String):LoweredValue {
		if (typeKey(value.mapping.irType) == typeKey(target.irType)) {
			return value;
		}
		final sourceClass = value.mapping.classValue();
		final targetClass = target.classValue();
		if (sourceClass != null || targetClass != null) {
			if (sourceClass == null || targetClass == null)
				return unsupportedAt(position, '$node:class-reference-category-mismatch:${value.mapping.cSpelling}->${target.cSpelling}');
			final sourceNullable = value.mapping.classNullable();
			final targetNullable = target.classNullable();
			if (sourceNullable == null || targetNullable == null)
				return unsupportedAt(position, '$node:class-reference-nullability-missing');
			if (!sourceClass.isDescendantOf(targetClass)) {
				if (targetClass.isDescendantOf(sourceClass))
					return unsupportedAt(position, '$node:unsafe-class-downcast-needs-runtime-type-proof:${sourceClass.haxePath}->${targetClass.haxePath}');
				return unsupportedAt(position, '$node:unrelated-class-reference-conversion:${sourceClass.haxePath}->${targetClass.haxePath}');
			}
			if (sourceNullable && !targetNullable)
				return unsupportedAt(position, '$node:nullable-class-reference-requires-proof');
			var converted = value;
			if (sourceClass.instanceId != targetClass.instanceId) {
				final upcastTarget = CBodyValueType.classReference(targetClass, sourceNullable);
				final upcast:HxcIRResult = {id: nextValueId(), type: upcastTarget.irType};
				appendInstruction(upcast, IRIOConvert(converted.id, IRCRepresentation, upcastTarget.irType, IRIStatic, null),
					HaxeSourceSpan.fromPosition(position, input.sourcePath), "class-upcast");
				converted = {id: upcast.id, type: upcast.type, mapping: upcastTarget};
			}
			if (!sourceNullable && targetNullable) {
				final injected:HxcIRResult = {id: nextValueId(), type: target.irType};
				appendInstruction(injected, IRIOConvert(converted.id, IRCNullableInject, target.irType, IRIStatic, null),
					HaxeSourceSpan.fromPosition(position, input.sourcePath), "class-nullable-inject");
				converted = {id: injected.id, type: injected.type, mapping: target};
			}
			return converted;
		}
		final sourcePrimitive = value.mapping.primitiveMapping();
		final targetPrimitive = target.primitiveMapping();
		if (sourcePrimitive == null || targetPrimitive == null) {
			return unsupportedAt(position, '$node:incompatible-closed-record-shapes:${value.mapping.cSpelling}->${target.cSpelling}');
		}
		return switch CPrimitiveSemantics.conversion(sourcePrimitive, targetPrimitive, CPUImplicit) {
			case CPConversionElided:
				value;
			case CPConversionAllowed(decision):
				if (decision.failureRequired) {
					unsupportedAt(position, '$node:implicit-conversion-requires-failure-edge');
				}
				switch decision.implementation {
					case IRIStatic:
					case IRIProgramLocal(_):
					case IRIRuntime(featureId):
						unsupportedAt(position, '$node:primitive-conversion-must-not-use-runtime:$featureId');
				}
				final result:HxcIRResult = {id: nextValueId(), type: target.irType};
				appendInstruction(result, IRIOConvert(value.id, decision.irKind, target.irType, decision.implementation, null),
					HaxeSourceSpan.fromPosition(position, input.sourcePath), "convert");
				{id: result.id, type: result.type, mapping: target};
			case CPConversionRejected(reason):
				unsupportedAt(position, '$node:unsupported-implicit-conversion:$reason');
		};
	}

	function requirePrimitive(type:CBodyValueType, position:Position, node:String):CPrimitiveTypeMapping {
		final mapping = type.primitiveMapping();
		return mapping == null ? unsupportedAt(position, '$node:closed-record-not-admitted-in-primitive-operation') : mapping;
	}

	function primitiveBinaryOperator(operation:Binop, expression:TypedExpr):CPrimitiveBinaryOperator {
		return switch operation {
			case OpAdd: CPBOAdd;
			case OpSub: CPBOSubtract;
			case OpMult: CPBOMultiply;
			case OpDiv: CPBODivide;
			case OpMod: CPBOModulo;
			case OpShl: CPBOShiftLeft;
			case OpShr: CPBOShiftRight;
			case OpUShr: CPBOUnsignedShiftRight;
			case OpAnd: CPBOBitAnd;
			case OpOr: CPBOBitOr;
			case OpXor: CPBOBitXor;
			case OpEq: CPBOEqual;
			case OpNotEq: CPBONotEqual;
			case OpLt: CPBOLess;
			case OpLte: CPBOLessEqual;
			case OpGt: CPBOGreater;
			case OpGte: CPBOGreaterEqual;
			case _:
				unsupported(expression, 'TBinop($operation)');
		};
	}

	function expressionCreatesFlow(expression:TypedExpr):Bool {
		return switch expression.expr {
			case TIf(_, _, _) | TSwitch(_, _, _): true;
			case TBinop(OpBoolAnd, _, _) | TBinop(OpBoolOr, _, _): true;
			case TBinop(_, left, right): expressionCreatesFlow(left) || expressionCreatesFlow(right);
			case TUnop(_, _, operand) | TParenthesis(operand) | TMeta(_, operand) | TCast(operand, _): expressionCreatesFlow(operand);
			case TBlock(expressions): anyExpressionCreatesFlow(expressions);
			case TCall(callee, arguments): expressionCreatesFlow(callee) || anyExpressionCreatesFlow(arguments);
			case TVar(_, initializer): initializer != null && expressionCreatesFlow(initializer);
			case TReturn(value): value != null && expressionCreatesFlow(value);
			case _: false;
		};
	}

	function anyExpressionCreatesFlow(expressions:Array<TypedExpr>):Bool {
		for (expression in expressions) {
			if (expressionCreatesFlow(expression)) {
				return true;
			}
		}
		return false;
	}

	function directStaticFunctionId(callee:TypedExpr, arguments:Array<TypedExpr>):String {
		return switch callee.expr {
			case TField(_, FStatic(classReference, fieldReference)):
				final owner = classReference.get();
				final field = fieldReference.get();
				final baseFunctionId = CBodyLowering.functionId(owner.pack.concat([owner.name]).join("."), field.name);
				CGenericCallResolver.resolve(baseFunctionId, field.type, field.params, callee.t, arguments.map(argument -> argument.t), input.specialization,
					context.profile, callee.pos, unsupportedAt)
					.instanceId();
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): directStaticFunctionId(inner, arguments);
			case _: unsupported(callee, 'TCall(callee=${nodeName(callee)}:not-direct-static)');
		};
	}

	function primitiveMapping(type:Type, position:Position, node:String):CPrimitiveTypeMapping {
		return switch CPrimitiveTypeMapper.map(applyCurrentSpecialization(type), context.profile) {
			case CTPrimitive(mapping):
				final admitted = mapping.nullability == CPNonNullable && switch mapping.sourceType {
					case CPHaxeVoid | CPHaxeBool | CPHaxeInt | CPHaxeUInt | CPHaxeFloat: true;
					case _: false;
				};
				if (!admitted) {
					unsupportedAt(position, '$node:${mapping.cSpelling}');
				}
				mapping;
			case CTReference(identity, nullable):
				unsupportedAt(position, '$node:reference-$identity-${nullable ? "nullable" : "non-null"}');
			case CTNativePointer(identity, nullable):
				unsupportedAt(position, '$node:native-pointer-$identity-${nullable ? "nullable" : "non-null"}');
			case CTUnsupported(reason):
				unsupportedAt(position, '$node:$reason');
		};
	}

	function bodyValueType(type:Type, position:Position, node:String):CBodyValueType
		return aggregateRegistry.valueType(applyCurrentSpecialization(type), position, input.modulePath, input.sourcePath, rejectAggregateType, node);

	function applyCurrentSpecialization(type:Type):Type
		return input.specialization == null ? type : input.specialization.apply(type);

	function rejectAggregateType(position:Position, node:String):Void
		unsupportedAt(position, node);

	function compilerSpanIndexMapping(position:Position):CPrimitiveTypeMapping {
		return switch CPrimitiveTypeMapper.map(Context.getType("c.Size"), context.profile) {
			case CTPrimitive(mapping) if (mapping.nullability == CPNonNullable):
				switch mapping.irType {
					case IRTAbiInteger(IRAKSize): mapping;
					case _: unsupportedAt(position, "span-loop-index:compiler-size-type-unavailable");
				}
			case _:
				unsupportedAt(position, "span-loop-index:compiler-size-type-unavailable");
		};
	}

	function bodyCollectionType(type:Type, position:Position, node:String):Null<BodyCollectionType> {
		return switch applyCurrentSpecialization(type) {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : bodyCollectionType(resolved, position, node);
			case TLazy(resolve): bodyCollectionType(resolve(), position, node);
			case TType(reference, parameters):
				final definition = reference.get();
				bodyCollectionType(TypeTools.applyTypeParameters(definition.type, definition.params, parameters), position, node);
			case TAbstract(reference, parameters):
				final abstractType = reference.get();
				final path = abstractType.pack.concat([abstractType.name]).join(".");
				switch path {
					case "c.CArray" if (parameters.length == 2):
						{
							kind: BCKFixedArray(stableTypeIdentity(parameters[1], position, '$node:length-witness')),
							element: collectionElement(parameters[0], position, node)
						};
					case "c.Span" if (parameters.length == 1):
						{kind: BCKSpan(true), element: collectionElement(parameters[0], position, node)};
					case "c.ConstSpan" if (parameters.length == 1):
						{kind: BCKSpan(false), element: collectionElement(parameters[0], position, node)};
					case _: null;
				}
			case _: null;
		};
	}

	function collectionElement(type:Type, position:Position, node:String):CPrimitiveTypeMapping {
		return switch CPrimitiveTypeMapper.map(applyCurrentSpecialization(type), context.profile) {
			case CTPrimitive(mapping):
				final admitted = mapping.nullability == CPNonNullable && switch mapping.irType {
					case IRTBool | IRTInt(_, _) | IRTFloat(32) | IRTFloat(64): true;
					case _: false;
				};
				if (!admitted) {
					unsupportedAt(position, '$node:collection-element:${mapping.cSpelling}');
				}
				mapping;
			case CTReference(identity, nullable):
				unsupportedAt(position, '$node:collection-element:reference-$identity-${nullable ? "nullable" : "non-null"}');
			case CTNativePointer(identity, nullable):
				unsupportedAt(position, '$node:collection-element:native-pointer-$identity-${nullable ? "nullable" : "non-null"}');
			case CTUnsupported(reason):
				unsupportedAt(position, '$node:collection-element:$reason');
		};
	}

	function stableTypeIdentity(type:Type, position:Position, node:String):String {
		return switch applyCurrentSpecialization(type) {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? unsupportedAt(position, '$node:unresolved') : stableTypeIdentity(resolved, position, node);
			case TLazy(resolve): stableTypeIdentity(resolve(), position, node);
			case TType(reference, _):
				final value = reference.get();
				value.pack.concat([value.name]).join(".");
			case TAbstract(reference, _):
				final value = reference.get();
				value.pack.concat([value.name]).join(".");
			case TInst(reference, _):
				final value = reference.get();
				value.pack.concat([value.name]).join(".");
			case TEnum(reference, _):
				final value = reference.get();
				value.pack.concat([value.name]).join(".");
			case _: unsupportedAt(position, '$node:requires-named-type');
		};
	}

	static function unwrapExpression(expression:TypedExpr):TypedExpr {
		return switch expression.expr {
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): unwrapExpression(inner);
			case _: expression;
		};
	}

	static function isAbstractMethod(callee:TypedExpr, ownerPath:String, methodName:String):Bool {
		return switch unwrapExpression(callee).expr {
			case TField(_, FStatic(classReference, fieldReference)) if (fieldReference.get().name == methodName):
				switch classReference.get().kind {
					case KAbstractImpl(abstractReference):
						final owner = abstractReference.get();
						owner.pack.concat([owner.name]).join(".") == ownerPath;
					case _: false;
				}
			case _: false;
		};
	}

	static function fieldAccessName(access:FieldAccess):String {
		return switch access {
			case FInstance(_, _, field) | FStatic(_, field) | FAnon(field) | FClosure(_, field): field.get().name;
			case FDynamic(name): name;
			case FEnum(_, field): field.name;
		};
	}

	static function typeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTAbiInteger(kind): 'abi:$kind';
			case IRTFloat(width): 'f$width';
			case IRTString: "string-utf8";
			case IRTCString: "cstring-borrowed-literal";
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(pointee, nullable): 'pointer:${nullable ? "nullable" : "nonnull"}<${typeKey(pointee)}>';
			case IRTNullable(inner, representation): 'nullable:$representation<${typeKey(inner)}>';
			case IRTFunction(parameters, result): 'function(${parameters.map(typeKey).join(",")})->${typeKey(result)}';
			case IRTFixedArray(element, length, witnessId): 'fixed-array:$length:$witnessId<${typeKey(element)}>';
			case IRTSpan(element, mutable): 'span:${mutable ? "mutable" : "const"}<${typeKey(element)}>';
			case IRTDynamic: "dynamic";
		};
	}

	static function isNullableClassReference(type:HxcIRTypeRef):Bool {
		return switch type {
			case IRTPointer(IRTInstance(_), true): true;
			case _: false;
		};
	}

	function requireConstantType(type:HxcIRTypeRef, expression:TypedExpr, family:String, predicate:HxcIRTypeRef->Bool):Void {
		if (!predicate(type)) {
			unsupported(expression, 'TConst($family:type-mismatch)');
		}
	}

	function instruction(result:Null<HxcIRResult>, kind:HxcIRInstructionKind, source:HxcSourceSpan, role:String):HxcIRInstruction {
		return {
			id: 'instruction.${instructionOrdinal++}.$role',
			result: result,
			kind: kind,
			source: source
		};
	}

	function nextValueId():String
		return 'value.${valueOrdinal++}';

	function unsupported<T>(expression:TypedExpr, node:String):T
		return unsupportedAt(expression.pos, node);

	function unsupportedAt<T>(position:Position, node:String):T {
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile), node, functionContext, source), position);
	}

	function invalidAbi<T>(expression:TypedExpr, detail:String):T {
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		throw new CBodyLoweringError(HxcIRDiagnostic.invalidAbiBoundary(Std.string(context.profile), functionContext, detail, source), expression.pos);
	}

	function canonicalFloat(value:String, expression:TypedExpr):String {
		var text = value;
		var sign = "";
		if (StringTools.startsWith(text, "+") || StringTools.startsWith(text, "-")) {
			sign = StringTools.startsWith(text, "-") ? "-" : "";
			text = text.substr(1);
		}
		var exponentText:Null<String> = null;
		var exponentIndex = text.indexOf("e");
		if (exponentIndex == -1) {
			exponentIndex = text.indexOf("E");
		}
		if (exponentIndex != -1) {
			if (text.indexOf("e", exponentIndex + 1) != -1 || text.indexOf("E", exponentIndex + 1) != -1) {
				unsupported(expression, 'TConst(TFloat:$value)');
			}
			exponentText = text.substr(exponentIndex + 1);
			text = text.substr(0, exponentIndex);
		}
		final dot = text.indexOf(".");
		if (dot != -1 && text.indexOf(".", dot + 1) != -1) {
			unsupported(expression, 'TConst(TFloat:$value)');
		}
		var whole = dot == -1 ? text : text.substr(0, dot);
		var fraction:Null<String> = dot == -1 ? null : text.substr(dot + 1);
		if ((whole == "" && (fraction == null || fraction == ""))
			|| !decimalDigits(whole)
			|| fraction != null
			&& !decimalDigits(fraction)) {
			unsupported(expression, 'TConst(TFloat:$value)');
		}
		whole = canonicalDigits(whole == "" ? "0" : whole);
		if (fraction != null && fraction == "") {
			fraction = "0";
		}
		var exponent = "";
		if (exponentText != null) {
			final parsed = Std.parseInt(exponentText);
			if (parsed == null) {
				unsupported(expression, 'TConst(TFloat:$value)');
			}
			exponent = 'e$parsed';
		}
		if (fraction == null && exponent == "") {
			fraction = "0";
		}
		return sign + whole + (fraction == null ? "" : "." + fraction) + exponent;
	}

	static function decimalDigits(value:String):Bool {
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null || code < 0x30 || code > 0x39) {
				return false;
			}
		}
		return true;
	}

	static function canonicalDigits(value:String):String {
		var index = 0;
		while (index + 1 < value.length && value.charCodeAt(index) == 0x30) {
			index++;
		}
		return value.substr(index);
	}

	public static function nodeName(expression:TypedExpr):String {
		return switch expression.expr {
			case TConst(value): 'TConst(${constantName(value)})';
			case TLocal(variable): 'TLocal(${variable.name})';
			case TArray(_, _): "TArray";
			case TBinop(operation, _, _): 'TBinop($operation)';
			case TField(_, _): "TField";
			case TTypeExpr(_): "TTypeExpr";
			case TParenthesis(_): "TParenthesis";
			case TObjectDecl(_): "TObjectDecl";
			case TArrayDecl(_): "TArrayDecl";
			case TCall(_, _): "TCall";
			case TNew(_, _, _): "TNew";
			case TUnop(operation, flag, _): 'TUnop($operation,$flag)';
			case TFunction(_): "TFunction";
			case TVar(variable, _): 'TVar(${variable.name})';
			case TBlock(_): "TBlock";
			case TFor(_, _, _): "TFor";
			case TIf(_, _, _): "TIf";
			case TWhile(_, _, _): "TWhile";
			case TSwitch(_, _, _): "TSwitch";
			case TTry(_, _): "TTry";
			case TReturn(_): "TReturn";
			case TBreak: "TBreak";
			case TContinue: "TContinue";
			case TThrow(_): "TThrow";
			case TCast(_, _): "TCast";
			case TMeta(metadata, _): 'TMeta(${metadata.name})';
			case TEnumParameter(_, _, _): "TEnumParameter";
			case TEnumIndex(_): "TEnumIndex";
			case TIdent(value): 'TIdent($value)';
		};
	}

	static function constantName(value:TConstant):String {
		return switch value {
			case TInt(_): "TInt";
			case TFloat(_): "TFloat";
			case TString(_): "TString";
			case TBool(_): "TBool";
			case TNull: "TNull";
			case TThis: "TThis";
			case TSuper: "TSuper";
		};
	}
}
#else
class CBodyLowering {
	public function new() {}
}
#end

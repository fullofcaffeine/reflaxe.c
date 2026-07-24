package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Expr.Binop;
import haxe.macro.Expr.Unop;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import haxe.macro.TypedExprTools;
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
import reflaxe.c.ir.HxcIRManagedRootPlanner;
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
import reflaxe.c.lowering.CBodyArray.CPreparedBodyArray;
import reflaxe.c.lowering.CBodyArray.CLoweredBodyArray;
import reflaxe.c.lowering.CBodyArray.CBodyArrayRecognition;
import reflaxe.c.lowering.CBodyBytes.CPreparedBodyBytes;
import reflaxe.c.lowering.CBodyBytes.CBodyBytesRecognition;
import reflaxe.c.lowering.CBodyClass.CLoweredBodyClass;
import reflaxe.c.lowering.CBodyClass.CBodyInterfaceImplementation;
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
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRepresentation;
import reflaxe.c.lowering.CBodyValueCoalescing.CBodyValueCoalescingPlanner;
import reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumCase;
import reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance;
import reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumPayload;
import reflaxe.c.lowering.CBodyInterface.CPreparedBodyInterface;
import reflaxe.c.lowering.CBodyIntrinsicReceiver.CBodyIntrinsicReceiverFamily;
import reflaxe.c.lowering.CBodyIntMap.CBodyIntMapRecognition;
import reflaxe.c.lowering.CBodyIntMap.CPreparedBodyIntMap;
import reflaxe.c.lowering.CBodyOptional.CLoweredBodyOptional;
import reflaxe.c.lowering.CBodyStringMap.CBodyStringMapRecognition;
import reflaxe.c.lowering.CBodyStringMap.CLoweredBodyStringMap;
import reflaxe.c.lowering.CBodyStringMap.CPreparedBodyStringMap;
import reflaxe.c.lowering.CGenericSpecialization.CGenericCallResolver;
import reflaxe.c.lowering.CGenericSpecialization.CGenericFunctionSpecialization;
import reflaxe.c.lowering.CBodyNullCheckCoalescing;
import reflaxe.c.lowering.CPrimitiveHelperEmitter.CPrimitiveHelperPlan;
import reflaxe.c.lowering.CPrimitiveHelperEmitter.CPrimitiveHelperSelection;
import reflaxe.c.semantics.CPrimitiveTypeMapper;
import reflaxe.c.semantics.CPrimitiveSemantics;
import reflaxe.c.semantics.CPrimitiveTypes;

/** Stable semantic coordinates plus one real pinned-compiler function expression. */
typedef CBodyFunctionInput = {
	final modulePath:String;
	final declarationPath:String;

	/** Source-facing owner used only to make generated C names readable. */
	final ?readableDeclarationPath:String;

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

	/** Source-facing owner used only to make generated C names readable. */
	final ?readableDeclarationPath:String;

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

	/** Source-facing owner used only to make generated C names readable. */
	final ?readableDeclarationPath:String;

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

/** One source-rooted runtime requirement retained for planning and diagnostics. */
class CBodyRuntimeRequirement {
	public final featureId:String;
	public final operationId:String;
	public final surface:String;
	public final source:HxcSourceSpan;
	public final position:Null<Position>;
	public final kind:String;

	public function new(featureId:String, operationId:String, surface:String, source:HxcSourceSpan, position:Null<Position>,
			kind:String = "runtime-operation") {
		this.featureId = featureId;
		this.operationId = operationId;
		this.surface = surface;
		this.source = source;
		this.position = position;
		this.kind = kind;
	}
}

/** Final collision-checked names for the generated executable collector state. */
class CManagedProgramNames {
	public final collector:CIdentifier;
	public final thread:CIdentifier;
	public final rootArrays:Map<String, CIdentifier>;
	public final rootFrames:Map<String, CIdentifier>;

	public function new(collector:CIdentifier, thread:CIdentifier, rootArrays:Map<String, CIdentifier>, rootFrames:Map<String, CIdentifier>) {
		this.collector = collector;
		this.thread = thread;
		this.rootArrays = rootArrays;
		this.rootFrames = rootFrames;
	}
}

private typedef CManagedProgramRequests = {
	final collector:CSymbolRequest;
	final thread:CSymbolRequest;
	final rootArrays:Map<String, CSymbolRequest>;
	final rootFrames:Map<String, CSymbolRequest>;
}

/** Complete deterministic result for the admitted body subset. */
class CBodyLoweringResult {
	public final program:HxcIRProgram;
	public final functions:Array<CLoweredBodyFunction>;
	public final globals:Array<CLoweredBodyGlobal>;
	public final aggregates:Array<CLoweredBodyAggregate>;
	public final enums:Array<CLoweredBodyEnum>;
	public final classes:Array<CLoweredBodyClass>;
	public final arrays:Array<CLoweredBodyArray>;
	public final intMaps:Array<CPreparedBodyIntMap>;
	public final stringMaps:Array<CLoweredBodyStringMap>;
	public final bytes:Array<CPreparedBodyBytes>;
	public final optionals:Array<CLoweredBodyOptional>;
	public final constructors:Array<CLoweredBodyConstructor>;
	public final dispatch:CLoweredBodyDispatch;
	public final imports:CLoweredImports;
	public final helpers:Array<CPrimitiveHelperPlan>;
	public final buildFacts:Array<TypedCBuildFact>;
	public final symbolTable:CSymbolTableSnapshot;
	public final boundsAbortName:Null<CIdentifier>;
	public final runtimeRequirements:Array<CBodyRuntimeRequirement>;
	public final managedProgram:Null<CManagedProgramNames>;

	public function new(program:HxcIRProgram, functions:Array<CLoweredBodyFunction>, globals:Array<CLoweredBodyGlobal>,
			aggregates:Array<CLoweredBodyAggregate>, enums:Array<CLoweredBodyEnum>, classes:Array<CLoweredBodyClass>, arrays:Array<CLoweredBodyArray>,
			intMaps:Array<CPreparedBodyIntMap>, stringMaps:Array<CLoweredBodyStringMap>, bytes:Array<CPreparedBodyBytes>,
			optionals:Array<CLoweredBodyOptional>, constructors:Array<CLoweredBodyConstructor>, dispatch:CLoweredBodyDispatch, imports:CLoweredImports,
			helpers:Array<CPrimitiveHelperPlan>, buildFacts:Array<TypedCBuildFact>, symbolTable:CSymbolTableSnapshot, boundsAbortName:Null<CIdentifier>,
			runtimeRequirements:Array<CBodyRuntimeRequirement>, managedProgram:Null<CManagedProgramNames>) {
		this.program = program;
		this.functions = functions.copy();
		this.globals = globals.copy();
		this.aggregates = aggregates.copy();
		this.enums = enums.copy();
		this.classes = classes.copy();
		this.arrays = arrays.copy();
		this.intMaps = intMaps.copy();
		this.stringMaps = stringMaps.copy();
		this.bytes = bytes.copy();
		this.optionals = optionals.copy();
		this.constructors = constructors.copy();
		this.dispatch = dispatch;
		this.imports = imports;
		this.helpers = helpers.copy();
		this.buildFacts = buildFacts.copy();
		this.symbolTable = symbolTable;
		this.boundsAbortName = boundsAbortName;
		this.runtimeRequirements = runtimeRequirements.copy();
		this.managedProgram = managedProgram;
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
		final enumConstructorAdapters = new EnumConstructorAdapterRegistry(context, aggregateRegistry);
		for (fn in prepared)
			enumConstructorAdapters.discover(fn);
		for (adapter in enumConstructorAdapters.preparedFunctions()) {
			if (preparedById.exists(adapter.irId))
				throw new CBodyEmissionError('enum-constructor adapter collides with semantic function `${adapter.irId}`');
			preparedById.set(adapter.irId, adapter);
		}
		final builders:Array<FunctionBuilder> = [];
		for (fn in prepared)
			builders.push(new FunctionBuilder(context, fn, preparedById, constructorSignaturesById, globalRegistry, aggregateRegistry,
				enumConstructorAdapters, preparedDispatch));
		// Representation is a whole-program decision. Discover the narrow
		// `Array<Class>` graph first so an earlier function cannot choose stack
		// storage merely because a later function is the first place that mentions
		// the same class as an Array element.
		for (builder in builders)
			builder.discoverManagedRepresentations();
		final interfaceImplementations:Array<CBodyInterfaceImplementation> = [];
		for (table in preparedDispatch.tables) {
			final interfaceValue = table.layout.rootInterface;
			if (interfaceValue != null)
				interfaceImplementations.push(new CBodyInterfaceImplementation(interfaceValue, table.classValue));
		}
		aggregateRegistry.completeManagedRepresentations(interfaceImplementations);
		for (builder in builders)
			builder.completeManagedRepresentations();
		final built:Array<BuiltBodyFunction> = [];
		for (builder in builders)
			built.push(builder.build());
		for (adapter in enumConstructorAdapters.builtFunctions())
			built.push(adapter);
		aggregateRegistry.completeClassLayouts();
		final preparedGlobals = globalRegistry.canonicalGlobals();
		final preparedAggregates = aggregateRegistry.canonicalAggregates();
		final preparedEnums = aggregateRegistry.canonicalEnums();
		final preparedClasses = aggregateRegistry.canonicalClasses();
		final preparedInterfaces = aggregateRegistry.canonicalInterfaces();
		final preparedArrays = aggregateRegistry.canonicalArrays();
		final preparedIntMaps = aggregateRegistry.canonicalIntMaps();
		final preparedStringMaps = aggregateRegistry.canonicalStringMaps();
		final preparedBytes = aggregateRegistry.canonicalBytes();
		final preparedImports = aggregateRegistry.canonicalImports();
		final program = buildProgram(built, preparedGlobals, preparedAggregates, preparedEnums, preparedClasses, preparedInterfaces, preparedArrays,
			preparedIntMaps, preparedStringMaps, preparedBytes, preparedImports, preparedDispatch);
		new HxcIRManagedRootPlanner().run(program);
		new CBodyNullCheckCoalescing().run(program);
		CPhaseTiming.stop(hxcIRConstructionTimer);
		final hxcIRValidationTimer = CPhaseTiming.start(CPHxcIRValidation);
		new HxcIRValidator().requireValid(program, Std.string(context.profile));
		CPhaseTiming.stop(hxcIRValidationTimer);
		final analysisTimer = CPhaseTiming.start(CPSemanticAnalysesAndNaming);
		final helperSelection = new CPrimitiveHelperSelection();
		helperSelection.collect(program);
		helperSelection.register(context.symbols);
		final boundsAbortRequest = registerBoundsAbort(program);
		final managedProgramRequests = registerManagedProgramNames(program, preparedById);
		final symbolTable = context.symbols.finalizeSymbols();
		final loweredAggregates = aggregateRegistry.finalize(context.symbols);
		final loweredEnums = aggregateRegistry.finalizeEnums(context.symbols);
		final loweredClasses = aggregateRegistry.finalizeClasses(context.symbols);
		final loweredArrays = aggregateRegistry.finalizeArrays(context.symbols);
		final loweredStringMaps = aggregateRegistry.finalizeStringMaps(context.symbols);
		final loweredOptionals = aggregateRegistry.finalizeOptionals(context.symbols);
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
		final managedProgram = finalizeManagedProgramNames(managedProgramRequests);
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
		final emitter = new CBodyEmitter(loweredAggregates, loweredEnums, loweredClasses, loweredArrays, preparedIntMaps, loweredStringMaps, preparedBytes,
			loweredOptionals, loweredDispatch, loweredImports, managedProgram);
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
		for (array in preparedArrays) {
			final feature = array.managedByCollector ? "gc" : "array";
			runtimeRequirements.push(new CBodyRuntimeRequirement(feature, "managed-type-representation",
				"ordinary Haxe Array<T> shared container representation", array.source, array.position));
		}
		for (map in preparedStringMaps)
			runtimeRequirements.push(new CBodyRuntimeRequirement("string-map", "managed-type-representation",
				"ordinary Haxe Map<String, V> shared hash-table representation", map.source, map.position));
		for (map in preparedIntMaps)
			runtimeRequirements.push(new CBodyRuntimeRequirement("int-map", "managed-type-representation",
				"ordinary Haxe Map<Int, Bool> shared membership-table representation", map.source, map.position));
		for (value in preparedClasses) {
			if (!value.managedByCollector)
				continue;
			runtimeRequirements.push(new CBodyRuntimeRequirement("gc", "managed-type-representation",
				'escaping Haxe class `${value.haxePath}` stable identity', value.source, null));
			runtimeRequirements.push(new CBodyRuntimeRequirement("gc", "class-object-header", 'escaping Haxe class `${value.haxePath}` exact object layout',
				value.source, null));
		}
		for (bytes in preparedBytes)
			runtimeRequirements.push(new CBodyRuntimeRequirement("bytes", "managed-type-representation",
				"ordinary haxe.io.Bytes shared fixed-length binary storage", bytes.source, bytes.position));
		for (module in program.modules)
			for (fn in module.functions) {
				final roots = fn.managedRoots == null ? [] : fn.managedRoots;
				for (root in roots)
					runtimeRequirements.push(new CBodyRuntimeRequirement("gc", "root-frame", "compiler-emitted exact managed root frame", root.source, null));
			}
		runtimeRequirements.sort(compareRuntimeRequirements);
		return new CBodyLoweringResult(program, lowered, loweredGlobals, loweredAggregates, loweredEnums, loweredClasses, loweredArrays, preparedIntMaps,
			loweredStringMaps, preparedBytes, loweredOptionals, loweredConstructors, loweredDispatch, loweredImports, helpers,
			helperSelection.buildFacts().concat(loweredImports.buildFacts), symbolTable, boundsAbortName, runtimeRequirements, managedProgram);
	}

	function registerManagedProgramNames(program:HxcIRProgram, preparedById:Map<String, PreparedBodyFunction>):Null<CManagedProgramRequests> {
		var required = false;
		for (module in program.modules)
			for (fn in module.functions)
				if (fn.managedRoots != null && fn.managedRoots.length > 0)
					required = true;
		if (!required)
			return null;
		final collector = new CSymbolRequest(CSKRuntimePrivate, ["compiler", "gc", "collector"], CNSOrdinary("translation-unit"), CSVInternal, null, [], [],
			0, ["program", "gc"]);
		final thread = new CSymbolRequest(CSKRuntimePrivate, ["compiler", "gc", "thread"], CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 1,
			["program", "gc", "thread"]);
		context.symbols.register(collector);
		context.symbols.register(thread);
		final rootArrays:Map<String, CSymbolRequest> = [];
		final rootFrames:Map<String, CSymbolRequest> = [];
		for (module in program.modules)
			for (fn in module.functions) {
				if (fn.managedRoots == null || fn.managedRoots.length == 0)
					continue;
				final prepared = preparedById.get(fn.id);
				if (prepared == null)
					throw new CBodyEmissionError('managed-root function `${fn.id}` lost its prepared symbol owner');
				final roots = new CSymbolRequest(CSKRuntimePrivate, ["compiler", "gc", fn.id, "roots"], CNSOrdinary(prepared.functionRequest.stableKey()),
					CSVInternal, null, [], [], 0, ["gc", "roots"]);
				final frame = new CSymbolRequest(CSKRuntimePrivate, ["compiler", "gc", fn.id, "frame"], CNSOrdinary(prepared.functionRequest.stableKey()),
					CSVInternal, null, [], [], 1, ["gc", "frame"]);
				context.symbols.register(roots);
				context.symbols.register(frame);
				rootArrays.set(fn.id, roots);
				rootFrames.set(fn.id, frame);
			}
		return {
			collector: collector,
			thread: thread,
			rootArrays: rootArrays,
			rootFrames: rootFrames
		};
	}

	function finalizeManagedProgramNames(requests:Null<CManagedProgramRequests>):Null<CManagedProgramNames> {
		if (requests == null)
			return null;
		final roots:Map<String, CIdentifier> = [];
		final frames:Map<String, CIdentifier> = [];
		for (id => request in requests.rootArrays)
			roots.set(id, context.symbols.identifierFor(request));
		for (id => request in requests.rootFrames)
			frames.set(id, context.symbols.identifierFor(request));
		return new CManagedProgramNames(context.symbols.identifierFor(requests.collector), context.symbols.identifierFor(requests.thread), roots, frames);
	}

	function registerBoundsAbort(program:HxcIRProgram):Null<CSymbolRequest> {
		for (module in program.modules) {
			for (fn in module.functions) {
				if (fn.managedRoots != null && fn.managedRoots.length > 0) {
					final request = new CSymbolRequest(CSKMethod, ["c-standard-library", "abort"], CNSOrdinary("translation-unit"), CSVExternal, "abort");
					context.symbols.register(request);
					return request;
				}
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
							case IRTThrow(_, {target: IRFTAbort}) | IRTUnreachable:
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
			enums:Array<CPreparedBodyEnumInstance>, classes:Array<CPreparedBodyClass>, interfaces:Array<CPreparedBodyInterface>,
			arrays:Array<CPreparedBodyArray>, intMaps:Array<CPreparedBodyIntMap>, stringMaps:Array<CPreparedBodyStringMap>, bytes:Array<CPreparedBodyBytes>,
			imports:Array<CPreparedImportType>, dispatch:CPreparedBodyDispatch):HxcIRProgram {
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
		final interfacesByModule:Map<String, Array<CPreparedBodyInterface>> = [];
		for (value in interfaces) {
			var moduleInterfaces = interfacesByModule.get(value.ownerModule);
			if (moduleInterfaces == null) {
				moduleInterfaces = [];
				interfacesByModule.set(value.ownerModule, moduleInterfaces);
			}
			moduleInterfaces.push(value);
		}
		final arraysByModule:Map<String, Array<CPreparedBodyArray>> = [];
		for (value in arrays) {
			var moduleArrays = arraysByModule.get(value.ownerModule);
			if (moduleArrays == null) {
				moduleArrays = [];
				arraysByModule.set(value.ownerModule, moduleArrays);
			}
			moduleArrays.push(value);
		}
		final stringMapsByModule:Map<String, Array<CPreparedBodyStringMap>> = [];
		for (value in stringMaps) {
			var moduleMaps = stringMapsByModule.get(value.ownerModule);
			if (moduleMaps == null) {
				moduleMaps = [];
				stringMapsByModule.set(value.ownerModule, moduleMaps);
			}
			moduleMaps.push(value);
		}
		final intMapsByModule:Map<String, Array<CPreparedBodyIntMap>> = [];
		for (value in intMaps) {
			var moduleMaps = intMapsByModule.get(value.ownerModule);
			if (moduleMaps == null) {
				moduleMaps = [];
				intMapsByModule.set(value.ownerModule, moduleMaps);
			}
			moduleMaps.push(value);
		}
		final bytesByModule:Map<String, Array<CPreparedBodyBytes>> = [];
		for (value in bytes) {
			var moduleBytes = bytesByModule.get(value.ownerModule);
			if (moduleBytes == null) {
				moduleBytes = [];
				bytesByModule.set(value.ownerModule, moduleBytes);
			}
			moduleBytes.push(value);
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
		for (moduleId in interfacesByModule.keys()) {
			moduleIdSet.set(moduleId, true);
		}
		for (moduleId in arraysByModule.keys()) {
			moduleIdSet.set(moduleId, true);
		}
		for (moduleId in stringMapsByModule.keys())
			moduleIdSet.set(moduleId, true);
		for (moduleId in intMapsByModule.keys())
			moduleIdSet.set(moduleId, true);
		for (moduleId in bytesByModule.keys())
			moduleIdSet.set(moduleId, true);
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
			final interfaceEntries = interfacesByModule.get(moduleId);
			final moduleInterfaces = interfaceEntries == null ? [] : interfaceEntries;
			moduleInterfaces.sort((left, right) -> compareUtf8(left.declarationId, right.declarationId));
			final arrayEntries = arraysByModule.get(moduleId);
			final moduleArrays = arrayEntries == null ? [] : arrayEntries;
			moduleArrays.sort((left, right) -> compareUtf8(left.declarationId, right.declarationId));
			final stringMapEntries = stringMapsByModule.get(moduleId);
			final moduleStringMaps = stringMapEntries == null ? [] : stringMapEntries;
			moduleStringMaps.sort((left, right) -> compareUtf8(left.declarationId, right.declarationId));
			final intMapEntries = intMapsByModule.get(moduleId);
			final moduleIntMaps = intMapEntries == null ? [] : intMapEntries;
			moduleIntMaps.sort((left, right) -> compareUtf8(left.declarationId, right.declarationId));
			final bytesEntries = bytesByModule.get(moduleId);
			final moduleBytes = bytesEntries == null ? [] : bytesEntries;
			final importEntries = importsByModule.get(moduleId);
			final moduleImports = importEntries == null ? [] : importEntries;
			moduleImports.sort((left, right) -> compareUtf8(left.declarationId, right.declarationId));
			final spans = moduleFunctions.map(entry -> entry.ir.source)
				.concat(moduleGlobals.map(global -> global.ir.source))
				.concat(moduleAggregates.map(aggregate -> aggregate.source))
				.concat(moduleEnums.map(value -> value.source))
				.concat(moduleClasses.map(value -> value.source))
				.concat(moduleInterfaces.map(value -> value.source))
				.concat(moduleArrays.map(value -> value.source))
				.concat(moduleIntMaps.map(value -> value.source))
				.concat(moduleStringMaps.map(value -> value.source))
				.concat(moduleBytes.map(value -> value.source))
				.concat(moduleImports.map(value -> value.source));
			if (spans.length == 0) {
				throw new CBodyEmissionError('body lowering lost module `$moduleId` while building HxcIR');
			}
			modules.push({
				id: moduleId,
				types: moduleAggregates.map(aggregate -> aggregate.declaration())
					.concat(moduleEnums.map(value -> value.declaration()))
					.concat(moduleClasses.map(value -> value.declaration()))
					.concat(moduleInterfaces.map(value -> value.declaration()))
					.concat(moduleArrays.map(value -> value.declaration()))
					.concat(moduleIntMaps.map(value -> value.declaration()))
					.concat(moduleStringMaps.map(value -> value.declaration()))
					.concat(moduleBytes.map(value -> value.declaration()))
					.concat(moduleImports.map(value -> value.declaration())),
				typeInstances: moduleAggregates.map(aggregate -> aggregate.instance())
					.concat(moduleEnums.map(value -> value.instance()))
					.concat(moduleClasses.map(value -> value.instance()))
					.concat(moduleInterfaces.map(value -> value.instance()))
					.concat(moduleArrays.map(value -> value.instance()))
					.concat(moduleIntMaps.map(value -> value.instance()))
					.concat(moduleStringMaps.map(value -> value.instance()))
					.concat(moduleBytes.map(value -> value.instance()))
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
		return input.specialization != null ? input.specialization.instanceId : input.instanceOwner != null ? methodId(input.declarationPath,
			input.fieldName) : functionId(input.declarationPath, input.fieldName);

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

/** One checked index base, whether it is a local collection or an owned field. */
private typedef BodyIndexedCollection = {
	final place:HxcIRPlace;
	final kind:BodyCollectionKind;
	final element:CPrimitiveTypeMapping;
	final length:Null<Int>;
}

/** One addressable fixed array borrowed only while the current function runs. */
private typedef BodyFixedArrayBorrowSource = {
	final place:HxcIRPlace;
	final element:CPrimitiveTypeMapping;
	final length:Int;
	final witnessId:String;
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

/** One left-to-right value, optionally saved across later expression flow. */
private typedef StagedFlowValue = {
	final value:LoweredValue;
	final localId:Null<String>;
	final position:Position;
}

private typedef LoweredPlace = {
	final place:HxcIRPlace;
	final mapping:CBodyValueType;
	final mutable:Bool;
}

/** An assignment destination, optionally saved as one checked element address. */
private typedef StagedFlowPlace = {
	final target:LoweredPlace;
	final addressLocalId:Null<String>;
	final position:Position;
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

	/** Cleanup stack depth owned by the scope outside this loop. */
	final cleanupDepth:Int;

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

	/**
		Whether the parameter may name caller-owned object storage for this call.

		Concrete class pointers and interface `{ object, table }` values use
		different HxcIR ownership lists, but source-level escape checks need the
		same answer: the callee may use this reference and must not keep it.
	**/
	final borrowedReference:Bool;

	/**
	 * The Haxe-typed value used when a direct call omits this parameter.
	 *
	 * Haxe validates default expressions before the custom target runs. Keeping
	 * that typed expression preserves nominal enum, abstract, and nullability
	 * information that would be lost if haxe.c copied the value into an
	 * untyped flag or C token. The call lowerer consumes it exactly once when it
	 * turns a shorter direct call into the function's full C arity.
	 */
	final defaultValue:Null<TypedExpr>;
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
		final readableName = input.readableDeclarationPath == null ? null : input.readableDeclarationPath.split(".").concat([fieldName]);
		final request = new CSymbolRequest(CSKField, declarationPath.split(".").concat([fieldName]), CNSOrdinary("translation-unit"), CSVInternal, null, [],
			[], null, readableName);
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

/**
	Turns a Haxe enum constructor used as a value into one real typed function.

	Haxe permits `parse(token, ShowDialogue)`: the constructor itself has a
	function type even though no named Haxe method exists. C function pointers can
	point only at functions, so the compiler creates one deterministic adapter
	whose parameters match the constructor payload and whose body constructs that
	enum case. The adapter stays in validated HxcIR and ordinary project emission;
	it is not printer-invented C or runtime reflection.
**/
private class EnumConstructorAdapterRegistry {
	final context:CompilationContext;
	final aggregateRegistry:CBodyAggregateRegistry;
	final byId:Map<String, PreparedBodyFunction> = [];
	final casesById:Map<String, CPreparedBodyEnumCase> = [];

	public function new(context:CompilationContext, aggregateRegistry:CBodyAggregateRegistry) {
		this.context = context;
		this.aggregateRegistry = aggregateRegistry;
	}

	/** Discover constructor values while excluding constructors called directly. */
	public function discover(owner:PreparedBodyFunction):Void {
		function visit(expression:TypedExpr):Void {
			switch expression.expr {
				case TCall(callee, arguments):
					if (enumConstructor(callee) == null)
						visit(callee);
					for (argument in arguments)
						visit(argument);
				case TField(_, FEnum(reference, field)) if (isFunctionType(expression.t)):
					require(expression, reference, field, owner);
				case _:
					TypedExprTools.iter(expression, visit);
			}
		}
		visit(owner.bodyExpression);
	}

	public function require(expression:TypedExpr, reference:Ref<EnumType>, field:EnumField, owner:PreparedBodyFunction):PreparedBodyFunction {
		final callable = aggregateRegistry.valueType(expression.t, expression.pos, owner.modulePath, owner.sourcePath,
			(position, node) -> reject(owner, position, node), 'enum-constructor-function:${field.name}');
		final signature = callable.functionValue();
		if (signature == null)
			return reject(owner, expression.pos, 'enum-constructor-function:${field.name}:signature-lost');
		final enumValue = signature.result.enumValue();
		if (enumValue == null)
			return reject(owner, expression.pos, 'enum-constructor-function:${field.name}:result-not-enum');
		final definition = reference.get();
		if (definition.pack.concat([definition.name]).join(".") != enumValue.haxePath)
			return reject(owner, expression.pos, 'enum-constructor-function:${field.name}:owner-type-mismatch');
		final tagCase = enumValue.tagCase(field.name);
		if (tagCase == null)
			return reject(owner, expression.pos, 'enum-constructor-function:${field.name}:unknown-case');
		if (signature.parameters.length != tagCase.payload.length)
			return reject(owner, expression.pos,
				'enum-constructor-function:${field.name}:parameter-count=${signature.parameters.length},expected=${tagCase.payload.length}');
		for (index in 0...signature.parameters.length) {
			final parameter = signature.parameters[index];
			final payload = tagCase.payload[index];
			if (FunctionBuilder.typeKey(parameter.irType) != FunctionBuilder.typeKey(payload.valueType.irType))
				return reject(owner, expression.pos, 'enum-constructor-function:${field.name}:parameter-$index-type-mismatch');
			if (payload.indirect || hasManagedLifetime(payload.valueType))
				return reject(owner, expression.pos, 'enum-constructor-function:${field.name}:parameter-$index-managed-or-recursive-adapter-not-yet-admitted');
		}
		final id = adapterId(enumValue, field.name);
		final existing = byId.get(id);
		if (existing != null)
			return existing;
		// The adapter belongs to the enum's logical module, so anchor it at the
		// constructor declaration rather than whichever caller happened to discover
		// the value first. HxcIR modules must stay within one source file, and using
		// the caller here would also make output depend on discovery order.
		final source = tagCase.source;
		final request = new CSymbolRequest(CSKMethod, ["compiler", "enum-constructor-adapter", enumValue.digest, field.name], CNSOrdinary("translation-unit"),
			CSVInternal, null, [], enumValue.typeArguments.map(argument -> argument.key), null, [enumValue.displayName, field.name, "adapter"]);
		context.symbols.register(request);
		final parameters:Array<PreparedParameter> = [];
		final parameterRequests:Map<String, CSymbolRequest> = [];
		for (index in 0...tagCase.payload.length) {
			final payload = tagCase.payload[index];
			final parameterId = 'parameter.$index';
			parameters.push({
				compilerId: -1 - index,
				ir: {id: parameterId, type: payload.valueType.irType, source: source},
				mapping: payload.valueType,
				borrowedReference: false,
				defaultValue: null
			});
			final parameterRequest = new CSymbolRequest(CSKLocal, [
				"compiler",
				"enum-constructor-adapter",
				enumValue.digest,
				field.name,
				payload.name
			], CNSOrdinary(request.stableKey()), CSVInternal, null, [], [], index,
				[payload.name]);
			context.symbols.register(parameterRequest);
			parameterRequests.set(parameterId, parameterRequest);
		}
		final prepared:PreparedBodyFunction = {
			modulePath: enumValue.ownerModule,
			declarationPath: enumValue.haxePath,
			sourcePath: source.file,
			displayName: '${field.name} constructor adapter',
			fieldName: field.name,
			specialization: null,
			sourceExpression: expression,
			bodyExpression: expression,
			role: PBRFunction,
			irId: id,
			parameters: parameters,
			returnMapping: signature.result,
			functionRequest: request,
			parameterRequests: parameterRequests
		};
		byId.set(id, prepared);
		casesById.set(id, tagCase);
		return prepared;
	}

	public function preparedFunctions():Array<PreparedBodyFunction> {
		final result = [for (value in byId) value];
		result.sort((left, right) -> CBodyLowering.compareUtf8(left.irId, right.irId));
		return result;
	}

	public function builtFunctions():Array<BuiltBodyFunction>
		return preparedFunctions().map(build);

	function build(prepared:PreparedBodyFunction):BuiltBodyFunction {
		final tagCase = casesById.get(prepared.irId);
		if (tagCase == null)
			throw new CBodyEmissionError('enum-constructor adapter `${prepared.irId}` lost its case plan');
		final source = tagCase.source;
		final enumValue = prepared.returnMapping.enumValue();
		if (enumValue == null)
			throw new CBodyEmissionError('enum-constructor adapter `${prepared.irId}` lost its enum result representation');
		final result:HxcIRResult = {id: "value.0", type: prepared.returnMapping.irType};
		final ir:HxcIRFunction = {
			id: prepared.irId,
			displayName: '${prepared.declarationPath}.${prepared.displayName}',
			parameters: prepared.parameters.map(parameter -> parameter.ir),
			borrowedClassParameterIds: [],
			borrowedInterfaceParameterIds: [],
			borrowedClassLocalIds: [],
			managedRoots: [],
			locals: [],
			returnType: prepared.returnMapping.irType,
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [
						{
							id: "instruction.0.construct-enum-adapter",
							result: result,
							kind: IRIOConstructTag(enumValue.instanceId, tagCase.name, prepared.parameters.map(parameter -> parameter.ir.id)),
							source: source
						}
					],
					terminator: {kind: IRTReturn(result.id, []), source: source},
					source: source
				}
			],
			cleanupRegions: [],
			source: source
		};
		return {
			prepared: prepared,
			ir: ir,
			localRequests: [],
			spanLengthRequests: [],
			temporaryRequests: [],
			tailArgumentRequests: [],
			labelRequests: [],
			runtimeRequirements: []
		};
	}

	static function adapterId(value:CPreparedBodyEnumInstance, caseName:String):String
		return 'function.enum-constructor-adapter.${value.instanceId}.$caseName';

	static function hasManagedLifetime(value:CBodyValueType):Bool
		return switch value.kind {
			case CBVKArray(_) | CBVKBytes(_): true;
			case CBVKEnum(nested): nested.managedLifetime;
			case CBVKAggregate(aggregate): aggregate.managedLifetime;
			case CBVKOptional(optional): optional.managedLifetime;
			case _: false;
		};

	static function isFunctionType(type:Type):Bool
		return switch TypeTools.follow(type) {
			case TFun(_, _): true;
			case _: false;
		};

	static function enumConstructor(expression:TypedExpr):Null<EnumConstructorAccess>
		return switch expression.expr {
			case TField(_, FEnum(reference, field)): {reference: reference, field: field};
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): enumConstructor(inner);
			case _: null;
		};

	function reject<T>(owner:PreparedBodyFunction, position:Position, node:String):T {
		final source = HaxeSourceSpan.fromPosition(position, owner.sourcePath);
		throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile), node,
			'function ${owner.declarationPath}.${owner.displayName} body', source),
			position);
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
			if (declared.opt && argument.value == null)
				unsupported(input.expression.pos, 'TFunction(optional-argument-without-typed-default:${argument.v.name})');
			final mapping = admittedValueType(declared.t, input.expression.pos, 'TFunction(argument:${argument.v.name})');
			if (mapping.irType == IRTVoid) {
				unsupported(input.expression.pos, 'TFunction(argument:${argument.v.name}:Void)');
			}
			if (mapping.spanElement() != null) {
				// A final class has no subclasses, so this method can only use the
				// compiler-known body prepared here. That makes a span parameter a
				// short-lived borrow for one direct call, just like a static helper.
				// Keep overridable instance methods closed: an unknown override could
				// retain the pointer after its caller-owned storage has gone away.
				if (input.instanceOwner != null && !input.instanceOwner.get().isFinal) {
					unsupported(input.expression.pos, 'TFunction(argument:${argument.v.name}:borrowed-span-requires-static-function)');
				}
				if (input.specialization != null) {
					unsupported(input.expression.pos, 'TFunction(argument:${argument.v.name}:borrowed-span-generic-specialization-not-admitted)');
				}
			}
			final enumArgument = mapping.enumValue();
			final parameterId = 'parameter.$index';
			final source = HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath);
			parameters.push({
				compilerId: argument.v.id,
				ir: {id: parameterId, type: mapping.irType, source: source},
				mapping: mapping,
				borrowedReference: mapping.classValue() != null && parameterCanBorrow(functionValue.expr, argument.v.id),
				defaultValue: argument.value
			});
		}
		if (isBorrowedSpanType(declaredSignature.result)) {
			unsupported(input.expression.pos, "TFunction(return-type:borrowed-span-escape)");
		}
		final returnMapping = admittedValueType(declaredSignature.result, input.expression.pos, "TFunction(return-type)");
		final returnEnum = returnMapping.enumValue();
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
				mapping: selfMapping,
				borrowedReference: true,
				defaultValue: null
			};
		}
		final signatureParameters = parameters.copy();
		if (selfParameter != null)
			signatureParameters.unshift(selfParameter);
		final overloadSignature = signatureParameters.length == 0 ? [] : signatureParameters.map(parameter -> valueTypeKey(parameter.ir.type));
		final specializationArguments = input.specialization == null ? [] : input.specialization.arguments.map(argument -> argument.key);
		final readableName = input.readableDeclarationPath == null ? null : input.readableDeclarationPath.split(".").concat([input.fieldName]);
		final functionRequest = new CSymbolRequest(CSKMethod, input.declarationPath.split(".").concat([input.fieldName]), CNSOrdinary("translation-unit"),
			CSVInternal, null, overloadSignature, specializationArguments, null, readableName);
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

	/**
		Decide whether one class parameter can safely accept caller-owned storage.

		Returning, storing, aliasing, throwing, or constructor-capturing the exact
		reference would let it outlive the call, so those uses keep the ordinary
		owned-or-value function contract. Field reads/mutation and receiver calls do
		not copy the reference. Explicit call arguments are checked later against the
		known target parameter, which permits safe direct borrow forwarding while
		virtual, native, and otherwise unproven forwarding remains closed.
	**/
	public static function parameterCanBorrow(body:TypedExpr, compilerId:Int):Bool {
		var safe = true;
		function visit(expression:TypedExpr):Void {
			if (!safe)
				return;
			switch expression.expr {
				case TVar(_, initializer) if (initializer != null && isDirectParameterValue(initializer, compilerId)):
					safe = false;
				case TBinop(OpAssign, _, right) if (isDirectParameterValue(right, compilerId)):
					safe = false;
				case TReturn(value) if (value != null && isDirectParameterValue(value, compilerId)):
					safe = false;
				case TThrow(value) if (isDirectParameterValue(value, compilerId)):
					safe = false;
				case TNew(_, _, arguments):
					for (argument in arguments)
						if (isDirectParameterValue(argument, compilerId)) {
							safe = false;
							break;
						}
				case TFunction(_) if (referencesParameter(expression, compilerId)):
					// A nested function can run after its enclosing call returns.
					safe = false;
				case _:
			}
			if (safe)
				TypedExprTools.iter(expression, visit);
		}
		visit(body);
		return safe;
	}

	/**
		Prove that every escaping use of an interface parameter is `this.field = value`.

		A self-field store gives the representation planner one concrete ownership
		boundary: the constructed object traces the interface's object pointer.
		Aliases, returns, throws, closures, and storage through another object do not
		have that proof and remain rejected. Ordinary reads, receiver calls, and
		forwarding to a separately checked direct call stay permitted.
	**/
	public static function parameterRetainedOnlyBySelfField(body:TypedExpr, compilerId:Int):Bool {
		var safe = true;
		var retained = false;
		function visit(expression:TypedExpr):Void {
			if (!safe)
				return;
			switch expression.expr {
				case TVar(_, initializer) if (initializer != null && isDirectParameterValue(initializer, compilerId)):
					safe = false;
				case TBinop(OpAssign, left, right) if (isDirectParameterValue(right, compilerId)):
					if (isThisInstanceField(left)) {
						retained = true;
						visit(left);
					} else {
						safe = false;
					}
					return;
				case TReturn(value) if (value != null && isDirectParameterValue(value, compilerId)):
					safe = false;
				case TThrow(value) if (isDirectParameterValue(value, compilerId)):
					safe = false;
				case TNew(_, _, arguments):
					for (argument in arguments)
						if (isDirectParameterValue(argument, compilerId)) {
							safe = false;
							break;
						}
				case TFunction(_) if (referencesParameter(expression, compilerId)):
					safe = false;
				case _:
			}
			if (safe)
				TypedExprTools.iter(expression, visit);
		}
		visit(body);
		return safe && retained;
	}

	/** Recognize the typed left side of an assignment to the object being built. */
	static function isThisInstanceField(expression:TypedExpr):Bool {
		return switch expression.expr {
			case TField(owner, FInstance(_, _, _)): isThisValue(owner);
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isThisInstanceField(inner);
			case _: false;
		};
	}

	static function isThisValue(expression:TypedExpr):Bool {
		return switch expression.expr {
			case TConst(TThis): true;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isThisValue(inner);
			case _: false;
		};
	}

	/** True when this expression's value is the parameter itself, not a field/call result. */
	static function isDirectParameterValue(expression:TypedExpr, compilerId:Int):Bool {
		return switch expression.expr {
			case TLocal(variable): variable.id == compilerId;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isDirectParameterValue(inner, compilerId);
			case TIf(_, whenTrue, whenFalse): isDirectParameterValue(whenTrue,
					compilerId) || whenFalse != null && isDirectParameterValue(whenFalse, compilerId);
			case TBlock(expressions): expressions.length > 0 && isDirectParameterValue(expressions[expressions.length - 1], compilerId);
			case TSwitch(_, cases, defaultExpression):
				var direct = defaultExpression != null && isDirectParameterValue(defaultExpression, compilerId);
				for (item in cases)
					if (isDirectParameterValue(item.expr, compilerId)) {
						direct = true;
						break;
					}
				direct;
			case _: false;
		};
	}

	static function referencesParameter(expression:TypedExpr, compilerId:Int):Bool {
		var found = false;
		function visit(value:TypedExpr):Void {
			if (found)
				return;
			switch value.expr {
				case TLocal(variable) if (variable.id == compilerId):
					found = true;
				case _:
					TypedExprTools.iter(value, visit);
			}
		}
		visit(expression);
		return found;
	}

	static function valueTypeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTFloat(width): 'f$width';
			case IRTString: "string-utf8-static-view";
			case IRTSpan(element, mutable): 'span:${mutable ? "mutable" : "const"}<${valueTypeKey(element)}>';
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(IRTInstance(instanceId), nullable): 'class-reference:${nullable ? "nullable" : "nonnull"}:$instanceId';
			case IRTNullable(inner, representation): 'nullable:$representation<${valueTypeKey(inner)}>';
			case IRTFunction(parameters, result): 'function(${parameters.map(valueTypeKey).join(",")})->${valueTypeKey(result)}';
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
			if (declared.opt && argument.value == null)
				unsupported(input.expression.pos, 'TFunction(constructor-optional-argument-without-typed-default:${argument.v.name})');
			final mapping = admittedValueType(declared.t, input.expression.pos, 'TFunction(constructor-argument:${argument.v.name})');
			if (mapping.irType == IRTVoid)
				unsupported(input.expression.pos, 'TFunction(constructor-argument:${argument.v.name}:Void)');
			if (mapping.spanElement() != null)
				unsupported(input.expression.pos, 'TFunction(constructor-argument:${argument.v.name}:borrowed-span-constructor-not-admitted)');
			final borrowedInterface = mapping.interfaceValue() != null;
			final interfaceRemainsCallBounded = borrowedInterface
				&& FunctionPreparer.parameterCanBorrow(functionValue.expr, argument.v.id);
			final interfaceRetainedBySelf = borrowedInterface
				&& !interfaceRemainsCallBounded
				&& FunctionPreparer.parameterRetainedOnlyBySelfField(functionValue.expr, argument.v.id);
			if (borrowedInterface && !interfaceRemainsCallBounded && !interfaceRetainedBySelf)
				unsupported(input.expression.pos, 'TFunction(constructor-argument:${argument.v.name}:interface-retention-must-target-this-field)');
			arguments.push({
				compilerId: argument.v.id,
				ir: {
					id: 'parameter.$index',
					type: mapping.irType,
					source: HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath)
				},
				mapping: mapping,
				// A call-bounded interface may still point at caller-owned stack
				// storage. A `this.field` capture instead enters the collector graph
				// settled before this body is lowered.
				borrowedReference: interfaceRemainsCallBounded,
				defaultValue: argument.value
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
		final overloadSignature = [constructorTypeKey(signature.selfMapping, "self")];
		for (index in 0...signature.arguments.length)
			overloadSignature.push(constructorTypeKey(signature.arguments[index].mapping, 'argument:$index'));
		final functionRequest = new CSymbolRequest(CSKMethod, ["compiler", "constructor"].concat(input.declarationPath.split(".")),
			CNSOrdinary("translation-unit"), CSVInternal, null, overloadSignature, [], input.sourceOrder);
		context.symbols.register(functionRequest);
		final source = HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath);
		final self:PreparedParameter = {
			compilerId: -1,
			ir: {id: "parameter.self", type: signature.selfMapping.irType, source: source},
			mapping: signature.selfMapping,
			borrowedReference: false,
			defaultValue: null
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

	/**
	 * Build one constructor overload component without erasing its Haxe family.
	 *
	 * Several unrelated values use `IRTInstance` after semantic lowering. A raw
	 * IR match therefore cannot tell a direct closed record from an enum, Array,
	 * map, interface value, or managed object. Constructor admission uses the
	 * prepared `CBodyValueType` instead, so each family earns this boundary
	 * independently and an unsupported family receives a source diagnostic.
	 */
	function constructorTypeKey(value:CBodyValueType, role:String):String {
		// Enum recursion and payload ownership are whole-graph facts. Type
		// discovery has already reached this enum and its payload dependencies,
		// but the registry computes those lifecycle flags lazily. Recompute before
		// admitting a constructor key so an early recursive or managed enum cannot
		// look like an unmanaged by-value carrier.
		if (value.enumValue() != null)
			aggregateRegistry.canonicalEnums();
		return switch value.kind {
			case CBVKPrimitive(_):
				switch value.irType {
					case IRTBool: "bool";
					case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
					case IRTFloat(width): 'f$width';
					case _: throw new CBodyEmissionError('constructor primitive `$role` has unexpected IR type `${Std.string(value.irType)}`');
				}
			case CBVKClass(classValue, nullable):
				'class-reference:${nullable ? "nullable" : "nonnull"}:${classValue.instanceId}';
			case CBVKAggregate(aggregate):
				'closed-record:${aggregate.instanceId}';
			case CBVKArray(array):
				'array-reference:${array.instanceId}';
			case CBVKStaticString(sourceIdentity):
				'string-utf8-static-view:$sourceIdentity';
			case CBVKEnum(value) if (value.representation == CBERNativeEnum && !value.managedLifetime):
				'direct-enum:${value.instanceId}';
			case CBVKEnum(value) if (value.representation == CBERTaggedUnion && !value.managedLifetime):
				'unmanaged-payload-enum:${value.instanceId}';
			case CBVKOptional(optional) if (!optional.managedLifetime):
				'direct-optional:${optional.planId}';
			case CBVKInterface(interfaceValue):
				'interface-reference:${interfaceValue.instanceId}';
			case _:
				unsupported(input.expression.pos, 'TFunction(constructor-$role-type-not-admitted:${value.cSpelling})');
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
		final readableName = input.readableDeclarationPath == null ? null : input.readableDeclarationPath.split(".").concat([input.displayName]);
		final functionRequest = new CSymbolRequest(CSKStaticInitializer, symbolPath, CNSOrdinary("translation-unit"), CSVInternal, null, [], [],
			input.sourceOrder, readableName);
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
	final enumConstructorAdapters:EnumConstructorAdapterRegistry;
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
	final normalCleanupActionIds:Array<String> = [];
	final freshManagedArrayValueIds:Map<String, Bool> = [];
	final freshManagedIntMapValueIds:Map<String, Bool> = [];
	final freshManagedStringMapValueIds:Map<String, Bool> = [];
	final freshManagedBytesValueIds:Map<String, Bool> = [];
	final freshManagedEnumValueIds:Map<String, Bool> = [];
	final freshManagedAggregateValueIds:Map<String, Bool> = [];
	final freshManagedOptionalValueIds:Map<String, Bool> = [];
	final borrowedManagedArrayElementValueIds:Map<String, Bool> = [];
	final arrayCleanupActionIdsByCompilerId:Map<Int, String> = [];
	final intMapCleanupActionIdsByCompilerId:Map<Int, String> = [];
	final stringMapCleanupActionIdsByCompilerId:Map<Int, String> = [];
	final bytesCleanupActionIdsByCompilerId:Map<Int, String> = [];
	final enumCleanupActionIdsByCompilerId:Map<Int, String> = [];
	final aggregateCleanupActionIdsByCompilerId:Map<Int, String> = [];
	final optionalCleanupActionIdsByCompilerId:Map<Int, String> = [];
	final stackConstructedCompilerIds:Map<Int, Bool> = [];

	/** Automatic pointer locals that name parent-owned child storage without owning it. */
	final borrowedClassLocalIds:Map<String, Bool> = [];

	final borrowedClassValueIds:Map<String, Bool> = [];
	final initializedOwnedFixedArrayFields:Map<String, Bool> = [];
	final initializedOwnedClassFields:Map<String, Bool> = [];
	final initializedRetainedInterfaceFields:Map<String, Bool> = [];
	final initializedStaticStringFields:Map<String, Bool> = [];
	final initializedUnmanagedEnumFields:Map<String, Bool> = [];
	final initializedManagedArrayFields:Map<String, Bool> = [];
	final initializedManagedStringMapFields:Map<String, Bool> = [];
	var selfValue:Null<LoweredValue> = null;
	var localOrdinal = 0;
	var temporaryOrdinal = 0;
	var instructionOrdinal = 0;
	var valueOrdinal = 0;
	var blockOrdinal = 0;
	var currentBlock:MutableBodyBlock;

	public function new(context:CompilationContext, prepared:PreparedBodyFunction, functionsById:Map<String, PreparedBodyFunction>,
			constructorSignaturesById:Map<String, PreparedConstructorSignature>, globalRegistry:BodyGlobalRegistry, aggregateRegistry:CBodyAggregateRegistry,
			enumConstructorAdapters:EnumConstructorAdapterRegistry, dispatch:CPreparedBodyDispatch) {
		this.context = context;
		this.prepared = prepared;
		this.input = prepared;
		this.functionsById = functionsById;
		this.constructorSignaturesById = constructorSignaturesById;
		this.globalRegistry = globalRegistry;
		this.aggregateRegistry = aggregateRegistry;
		this.enumConstructorAdapters = enumConstructorAdapters;
		this.dispatch = dispatch;
		this.functionContext = 'function ${input.declarationPath}.${input.displayName} body';
		this.localOrdinal = prepared.parameters.length;
		this.currentBlock = createEntryBlock(HaxeSourceSpan.fromPosition(prepared.bodyExpression.pos, input.sourcePath));
		for (parameter in prepared.parameters) {
			final value:LoweredValue = {id: parameter.ir.id, type: parameter.ir.type, mapping: parameter.mapping};
			if (parameter.borrowedReference)
				borrowedClassValueIds.set(parameter.ir.id, true);
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

	/**
		Discover representation-changing types without emitting instructions.

		Function bodies are built only after every builder completes this walk. That
		makes `Array<Class>` selection independent of module or function discovery
		order: all uses of one nominal class see the same direct or collector-backed
		representation.
	**/
	public function discoverManagedRepresentations():Void
		discoverManagedExpression(prepared.bodyExpression);

	/**
		Remove stack-borrow restrictions after a class gains collector ownership.

		Function signatures are prepared before the whole-program representation
		fixed point. A parameter initially looks like a caller-owned stack pointer,
		but `Array<Class>` can later prove that every instance has stable traced
		storage. Such a pointer may safely be stored in another traced object; exact
		root planning, rather than the old stack-borrow rule, owns its lifetime.
	**/
	public function completeManagedRepresentations():Void {
		for (parameter in prepared.parameters) {
			final classValue = parameter.mapping.classValue();
			if (classValue != null && classValue.managedByCollector)
				borrowedClassValueIds.remove(parameter.ir.id);
		}
	}

	function discoverManagedExpression(expression:TypedExpr):Void {
		// `trace(value)` arrives from Haxe as `haxe.Log.trace(value, info)`. The
		// second argument is compiler-built source metadata; custom trace values are
		// nested inside it as an Array. The trace lowerer validates that metadata and
		// currently rejects custom parameters. Treating the synthesized object as
		// ordinary gameplay/application data here would discover a managed Array and
		// replace the useful trace diagnostic with an unrelated representation error.
		switch expression.expr {
			case TCall(callee, arguments) if (isHaxeLogTrace(callee)):
				if (arguments.length > 0)
					discoverManagedExpression(arguments[0]);
				return;
			case _:
		}
		final expressionType = applyCurrentSpecialization(expression.t);
		// A fixed CArray literal is typed as an ordinary Array expression by the
		// Haxe frontend before its destination abstract supplies the storage
		// contract. The fixed-array lowering owns that contextual conversion.
		// Skipping the literal subtree here prevents discovery from inventing an
		// unused managed Array representation (and an hxrt dependency) for
		// runtime-free fixed storage. CArray elements are deliberately restricted
		// to direct primitive values, so no nested managed representation is lost.
		switch expression.expr {
			case TVar(variable, _) if (CBodyFixedArray.isCArrayType(applyCurrentSpecialization(variable.t))):
				return;
			case _:
		}
		if (CBodyArrayRecognition.isCoreArrayType(expressionType))
			bodyValueType(expressionType, expression.pos, "managed-representation-discovery:Array");
		switch expression.expr {
			case TVar(variable, _):
				final variableType = applyCurrentSpecialization(variable.t);
				if (CBodyArrayRecognition.isCoreArrayType(variableType))
					bodyValueType(variableType, expression.pos, 'managed-representation-discovery:local:${variable.name}');
			case _:
		}
		TypedExprTools.iter(expression, discoverManagedExpression);
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
		validateConstructorManagedFields(bodyExpression.pos);
		if (freshManagedArrayValueIds.keys().hasNext())
			unsupportedAt(bodyExpression.pos, "function-exit:unowned-fresh-managed-Array-value");
		if (freshManagedStringMapValueIds.keys().hasNext())
			unsupportedAt(bodyExpression.pos, "function-exit:unowned-fresh-managed-StringMap-value");
		if (freshManagedIntMapValueIds.keys().hasNext())
			unsupportedAt(bodyExpression.pos, "function-exit:unowned-fresh-managed-IntMap-value");
		if (freshManagedBytesValueIds.keys().hasNext())
			unsupportedAt(bodyExpression.pos, "function-exit:unowned-fresh-managed-Bytes-value");
		if (freshManagedEnumValueIds.keys().hasNext())
			unsupportedAt(bodyExpression.pos, "function-exit:unowned-fresh-managed-enum-value");
		if (freshManagedAggregateValueIds.keys().hasNext())
			unsupportedAt(bodyExpression.pos, "function-exit:unowned-fresh-managed-record-value");
		if (freshManagedOptionalValueIds.keys().hasNext())
			unsupportedAt(bodyExpression.pos, "function-exit:unowned-fresh-managed-optional-value");
		if (currentBlock.terminator == null) {
			currentBlock.terminator = {
				kind: IRTReturn(null, normalCleanupSteps()),
				source: HaxeSourceSpan.fromPosition(bodyExpression.pos, input.sourcePath)
			};
		}
		final functionSpan = HaxeSourceSpan.fromPosition(input.sourceExpression.pos, input.sourcePath);
		final borrowedLocalIds = [for (localId in borrowedClassLocalIds.keys()) localId];
		borrowedLocalIds.sort((left, right) -> left < right ? -1 : left > right ? 1 : 0);
		final ir:HxcIRFunction = {
			id: prepared.irId,
			displayName: '${input.declarationPath}.${input.displayName}',
			parameters: prepared.parameters.map(parameter -> parameter.ir),
			borrowedClassParameterIds: prepared.parameters.filter(parameter -> {
				final classValue = parameter.mapping.classValue();
				return parameter.borrowedReference && classValue != null && !classValue.managedByCollector;
			}).map(parameter -> parameter.ir.id),
			borrowedInterfaceParameterIds: prepared.parameters.filter(parameter -> parameter.borrowedReference
				&& parameter.mapping.interfaceValue() != null)
				.map(parameter -> parameter.ir.id),
			borrowedClassLocalIds: borrowedLocalIds,
			managedRoots: [],
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
			case TBinop(OpAssign, left, right) if (lowerOwnedFixedArrayInitializer(left, right)):
				// The enclosing object was structurally zero-initialized before its
				// constructor call. The exact source field initializer is therefore
				// already satisfied without an illegal C whole-array assignment.
			case TBinop(OpAssign, left, right) if (lowerRetainedInterfaceFieldInitializer(left, right)):
				// The interface value copies a concrete object pointer and its exact
				// table into the newly allocated owner. The owner's trace callback
				// follows that object pointer for the rest of the field's lifetime.
			case TBinop(OpAssign, left, right) if (lowerStaticStringFieldInitializer(left, right)):
				// The immutable view is copied by value. Its bytes come from
				// compiler-owned literal storage and therefore outlive every object;
				// no retain, release, or owned-String runtime operation is needed.
			case TBinop(OpAssign, left, right) if (lowerUnmanagedEnumFieldInitializer(left, right)):
				// The prepared enum needs no retain, tracing, or destruction. The
				// first assignment initializes the final field by value.
			case TBinop(OpAssign, left, right) if (lowerManagedArrayFieldInitializer(left, right)):
				// A final Haxe Array field receives the one newly allocated shared
				// container. Later local aliases retain that identity; the enclosing
				// class cleanup releases this owning field exactly once.
			case TBinop(OpAssign, left, right) if (lowerManagedStringMapFieldInitializer(left, right)):
				// A final Haxe Map field receives the fresh table created by its
				// source initializer. The field owns that reference until its class
				// instance is destroyed.
			case TBinop(OpAssign, left, right) if (lowerOwnedClassInitializer(left, right)):
				// The child occupies an inline subobject of the parent. Its own
				// constructor runs against that stable address exactly once.
			case TConst(_) | TLocal(_) | TArray(_, _) | TField(_, _) | TCast(_, _) | TBinop(_, _, _) | TUnop(_, _, _):
				lowerValue(expression);
			case TCall(_, _) if (isEnumConstructorExpression(expression)):
				lowerValue(expression);
			case TCall(callee, arguments) if (isSuperCall(callee)):
				lowerSuperCall(expression, arguments);
			case TCall(_, _):
				final result = lowerCall(expression, false);
				if (result != null && freshManagedStringMapValueIds.exists(result.id))
					unsupported(expression, "TCall(discarded-fresh-managed-StringMap-needs-owner)");
				if (result != null && freshManagedBytesValueIds.exists(result.id))
					unsupported(expression, "TCall(discarded-fresh-managed-Bytes-needs-owner)");
				if (result != null && freshManagedEnumValueIds.exists(result.id))
					unsupported(expression, "TCall(discarded-fresh-managed-enum-needs-owner)");
				if (result != null && freshManagedAggregateValueIds.exists(result.id))
					unsupported(expression, "TCall(discarded-fresh-managed-record-needs-owner)");
				if (result != null && freshManagedOptionalValueIds.exists(result.id))
					unsupported(expression, "TCall(discarded-fresh-managed-optional-needs-owner)");
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
				lowerValue(expression);
			case _:
				unsupported(expression, nodeName(expression));
		}
	}

	function lowerStatementBlock(expressions:Array<TypedExpr>):Void {
		lowerStatementSequence(expressions, expressions.length);
	}

	/**
		Recognize only Haxe's constructor assignment for a validated zeroed array field.

		Ordinary whole-array assignment remains unsupported. Class preparation first
		proves that the field declares this exact initializer. Haxe then places that
		assignment before the authored constructor body, so only the first assignment
		for each prepared array field is construction; a later reset or copy is still
		rejected by ordinary assignment lowering.
	**/
	function lowerOwnedFixedArrayInitializer(left:TypedExpr, right:TypedExpr):Bool {
		switch prepared.role {
			case PBRConstructor(_):
			case _:
				return false;
		}
		final access = switch unwrapExpression(left).expr {
			case TField(receiver, FInstance(_, _, fieldReference)):
				switch unwrapExpression(receiver).expr {
					case TConst(TThis): fieldReference.get();
					case _: return false;
				}
			case _: return false;
		};
		if (!CBodyFixedArray.isZeroInitializer(right) || initializedOwnedFixedArrayFields.exists(access.name))
			return false;
		final self = selfValue;
		if (self == null)
			throw new CBodyEmissionError('constructor `${prepared.irId}` lost its self parameter while validating `${access.name}`');
		final owner = self.mapping.classValue();
		final field = owner == null ? null : owner.field(access.name);
		final fixed = field == null ? null : field.type.fixedArrayShape();
		if (fixed == null)
			return false;
		final length = CBodyFixedArray.zeroLength(right, fixed.element.irType, rejectAggregateType, 'TField(${access.name}:CArray.zero)');
		if (length != fixed.length)
			unsupported(right, 'TField(${access.name}:fixed-array-length-mismatch:planned=${fixed.length},constructor=$length)');
		initializedOwnedFixedArrayFields.set(access.name, true);
		return true;
	}

	/**
		Initialize one interface field retained by the object under construction.

		Haxe emits final-field constructor assignments as ordinary typed `=` nodes.
		Normal assignment rejects a final destination, so this owner recognizes only
		the first `this.field = value` for an interface field. Whole-program planning
		has already selected collector storage for the owner and every reachable
		concrete implementation; a stack-backed interface therefore cannot enter
		this store.
	**/
	function lowerRetainedInterfaceFieldInitializer(left:TypedExpr, right:TypedExpr):Bool {
		switch prepared.role {
			case PBRConstructor(_):
			case _:
				return false;
		}
		final fieldName = switch unwrapExpression(left).expr {
			case TField(receiver, FInstance(_, _, fieldReference)):
				switch unwrapExpression(receiver).expr {
					case TConst(TThis): fieldReference.get().name;
					case _: return false;
				}
			case _:
				return false;
		};
		if (initializedRetainedInterfaceFields.exists(fieldName))
			return false;
		final self = selfValue;
		if (self == null)
			throw new CBodyEmissionError('constructor `${prepared.irId}` lost its self parameter while initializing interface field `$fieldName`');
		final owner = self.mapping.classValue();
		final field = owner == null ? null : owner.field(fieldName);
		if (field == null || field.type.interfaceValue() == null)
			return false;
		final value = coerce(lowerValue(right, field.type), field.type, right.pos, 'TField($fieldName:retained-interface-initializer)');
		rejectOwnedClassBorrow(value, right.pos, 'TField($fieldName:retained-interface-stack-escape)');
		appendInstruction(null, IRIOStore(IRPField(IRPDereference(self.id), fieldName), value.id), HaxeSourceSpan.fromPosition(left.pos, input.sourcePath),
			"initialize-retained-interface-field");
		initializedRetainedInterfaceFields.set(fieldName, true);
		return true;
	}

	/**
	 * Initialize one final literal-backed String field during construction.
	 *
	 * The currently admitted Haxe String representation is a small immutable
	 * `(bytes, length)` view into compiler-owned literal storage. Copying that
	 * view into a field preserves the source value, and the bytes remain valid
	 * for the complete program. This rule therefore emits one by-value store
	 * without selecting the owned-String runtime or inventing reference counting.
	 *
	 * Haxe types the first `this.field = value` for a final field as an ordinary
	 * assignment. Normal assignment must reject immutable destinations, so this
	 * constructor-only owner recognizes exactly the first assignment to the
	 * prepared final String field. Any later assignment falls through and fails
	 * closed. Future runtime-created Strings need a separate owned-storage plan;
	 * this literal-backed rule must not be widened to cover them implicitly.
	 */
	function lowerStaticStringFieldInitializer(left:TypedExpr, right:TypedExpr):Bool {
		switch prepared.role {
			case PBRConstructor(_):
			case _:
				return false;
		}
		final fieldName = switch unwrapExpression(left).expr {
			case TField(receiver, FInstance(_, _, fieldReference)):
				switch unwrapExpression(receiver).expr {
					case TConst(TThis): fieldReference.get().name;
					case _: return false;
				}
			case _:
				return false;
		};
		if (initializedStaticStringFields.exists(fieldName))
			return false;
		final self = selfValue;
		if (self == null)
			throw new CBodyEmissionError('constructor `${prepared.irId}` lost its self parameter while initializing String field `$fieldName`');
		final owner = self.mapping.classValue();
		final field = owner == null ? null : owner.field(fieldName);
		if (field == null || field.mutable || field.type.staticStringIdentity() == null)
			return false;
		final value = coerce(lowerValue(right, field.type), field.type, right.pos, 'TField($fieldName:static-String-initializer)');
		appendInstruction(null, IRIOStore(IRPField(IRPDereference(self.id), fieldName), value.id), HaxeSourceSpan.fromPosition(left.pos, input.sourcePath),
			"initialize-static-string-field");
		initializedStaticStringFields.set(fieldName, true);
		return true;
	}

	/**
	 * Initialize one final unmanaged-enum field during construction.
	 *
	 * A fieldless Haxe enum is one nominal C tag. An unmanaged payload enum is a
	 * tagged C value whose complete active payload is safe to copy by value.
	 * Preparation proves the shared requirement through `managedLifetime ==
	 * false`, so construction can store either representation directly.
	 *
	 * This owner accepts only the first `this.field = value` in the constructor.
	 * Normal assignment still rejects later writes to the final field. Managed
	 * or recursive enums deliberately do not enter this path: their active
	 * payload may own nested values and needs a separately proven
	 * retain/transfer rule.
	 */
	function lowerUnmanagedEnumFieldInitializer(left:TypedExpr, right:TypedExpr):Bool {
		switch prepared.role {
			case PBRConstructor(_):
			case _:
				return false;
		}
		final fieldName = switch unwrapExpression(left).expr {
			case TField(receiver, FInstance(_, _, fieldReference)):
				switch unwrapExpression(receiver).expr {
					case TConst(TThis): fieldReference.get().name;
					case _: return false;
				}
			case _:
				return false;
		};
		if (initializedUnmanagedEnumFields.exists(fieldName))
			return false;
		final self = selfValue;
		if (self == null)
			throw new CBodyEmissionError('constructor `${prepared.irId}` lost its self parameter while initializing enum field `$fieldName`');
		final owner = self.mapping.classValue();
		final field = owner == null ? null : owner.field(fieldName);
		final enumValue = field == null ? null : field.type.enumValue();
		if (field == null || field.mutable || enumValue == null || enumValue.managedLifetime)
			return false;
		final value = coerce(lowerValue(right, field.type), field.type, right.pos, 'TField($fieldName:direct-enum-initializer)');
		appendInstruction(null, IRIOStore(IRPField(IRPDereference(self.id), fieldName), value.id), HaxeSourceSpan.fromPosition(left.pos, input.sourcePath),
			"initialize-unmanaged-enum-field");
		initializedUnmanagedEnumFields.set(fieldName, true);
		return true;
	}

	/**
		Initialize one final Array field emitted by Haxe before the constructor body.

		Haxe gives Array values shared identity: assigning an existing Array to a
		field must keep that same container alive, not clone its elements. A fresh
		reference-counted literal transfers its first owner directly into the field;
		an existing reference gains a field owner through one retain. Arrays whose
		elements can reach collector-managed objects need neither operation because
		the constructed object's trace callback follows the field instead.

		This remains a construction-only rule. The first typed assignment to the
		final field establishes its lifetime; later whole-Array replacement still
		falls through to ordinary assignment lowering and fails closed.
	**/
	function lowerManagedArrayFieldInitializer(left:TypedExpr, right:TypedExpr):Bool {
		switch prepared.role {
			case PBRConstructor(_):
			case _:
				return false;
		}
		final fieldName = switch unwrapExpression(left).expr {
			case TField(receiver, FInstance(_, _, fieldReference)):
				switch unwrapExpression(receiver).expr {
					case TConst(TThis): fieldReference.get().name;
					case _: return false;
				}
			case _: return false;
		};
		if (initializedManagedArrayFields.exists(fieldName))
			return false;
		final self = selfValue;
		if (self == null)
			throw new CBodyEmissionError('constructor `${prepared.irId}` lost its self parameter while initializing Array field `$fieldName`');
		final owner = self.mapping.classValue();
		final field = owner == null ? null : owner.field(fieldName);
		if (field == null || field.type.arrayValue() == null)
			return false;
		final value = coerce(lowerValue(right, field.type), field.type, right.pos, 'TField($fieldName:Array-initializer)');
		final array = field.type.arrayValue();
		if (array == null)
			throw new CBodyEmissionError('Array field `$fieldName` lost its prepared specialization');
		final source = HaxeSourceSpan.fromPosition(left.pos, input.sourcePath);
		final fieldPlace = IRPField(IRPDereference(self.id), fieldName);
		final transferredFreshOwner = !array.managedByCollector && freshManagedArrayValueIds.remove(value.id);
		appendInstruction(null, IRIOStore(fieldPlace, value.id), source, "initialize-array-field");
		if (!array.managedByCollector && !transferredFreshOwner) {
			appendInstruction(null, IRIORetain(fieldPlace, IRIRuntime("array")), source, "retain-array-field-owner");
			runtimeRequirements.push(new CBodyRuntimeRequirement("array", "retain",
				'ordinary Haxe Array field `${owner.haxePath}.$fieldName` retained from constructor input', source, right.pos));
		}
		initializedManagedArrayFields.set(fieldName, true);
		return true;
	}

	/**
		Transfer Haxe's lowered `=[]` initializer into one final StringMap field.

		Haxe places field initializers at the start of the constructor body. This
		recognizer accepts only the first assignment of a freshly constructed
		StringMap to the matching prepared field; later whole-map reassignment keeps
		failing through the ordinary assignment rule.
	**/
	function lowerManagedStringMapFieldInitializer(left:TypedExpr, right:TypedExpr):Bool {
		switch prepared.role {
			case PBRConstructor(_):
			case _:
				return false;
		}
		final fieldName = switch unwrapExpression(left).expr {
			case TField(receiver, FInstance(_, _, fieldReference)):
				switch unwrapExpression(receiver).expr {
					case TConst(TThis): fieldReference.get().name;
					case _: return false;
				}
			case _: return false;
		};
		if (initializedManagedStringMapFields.exists(fieldName))
			return false;
		final self = selfValue;
		if (self == null)
			throw new CBodyEmissionError('constructor `${prepared.irId}` lost its self parameter while initializing StringMap field `$fieldName`');
		final owner = self.mapping.classValue();
		final field = owner == null ? null : owner.field(fieldName);
		if (field == null || field.type.stringMapValue() == null)
			return false;
		final construction = newExpression(right);
		if (construction == null || !CBodyStringMapRecognition.isStringMap(construction.classReference))
			return false;
		final value = lowerStringMapConstruction(right, construction.arguments, field.type);
		if (!freshManagedStringMapValueIds.remove(value.id))
			throw new CBodyEmissionError('StringMap field `$fieldName` did not receive a fresh table owner');
		appendInstruction(null, IRIOStore(IRPField(IRPDereference(self.id), fieldName), value.id), HaxeSourceSpan.fromPosition(left.pos, input.sourcePath),
			"initialize-string-map-field");
		initializedManagedStringMapFields.set(fieldName, true);
		return true;
	}

	/** Construct one final child object directly inside its nonescaping parent. */
	function lowerOwnedClassInitializer(left:TypedExpr, right:TypedExpr):Bool {
		switch prepared.role {
			case PBRConstructor(_):
			case _:
				return false;
		}
		final fieldName = switch unwrapExpression(left).expr {
			case TField(receiver, FInstance(_, _, fieldReference)):
				switch unwrapExpression(receiver).expr {
					case TConst(TThis): fieldReference.get().name;
					case _: return false;
				}
			case _: return false;
		};
		if (initializedOwnedClassFields.exists(fieldName))
			return false;
		final construction = newExpression(right);
		if (construction == null)
			return false;
		final self = selfValue;
		if (self == null)
			throw new CBodyEmissionError('constructor `${prepared.irId}` lost its self parameter while validating `$fieldName`');
		final owner = self.mapping.classValue();
		final preparedField = owner == null ? null : owner.field(fieldName);
		final child = preparedField == null ? null : preparedField.type.ownedClassValue();
		if (child == null)
			return false;
		// Do not lower the left-hand side until the prepared layout proves that
		// this is an owned child. Lowering can emit receiver checks and address
		// temporaries, so probing an ordinary class assignment here would otherwise
		// duplicate observable compiler work before normal assignment lowering.
		final target = lowerPlace(left);
		if (target.mapping.ownedClassValue() != child)
			throw new CBodyEmissionError('owned child field `$fieldName` changed identity between class preparation and constructor lowering');
		final constructedPath = CBodyConstructor.classPath(construction.classReference);
		if (constructedPath != child.haxePath)
			return unsupported(right, 'TNew(owned-field-type-mismatch:$constructedPath->${child.haxePath})');
		final constructorId = CBodyConstructor.id(constructedPath);
		final signature = constructorSignaturesById.get(constructorId);
		if (signature == null)
			return unsupported(right, 'TNew(owned-field-constructor-unavailable:$constructorId)');
		if (signature.input.canFail)
			return unsupported(right, 'TNew(owned-field-fallible-construction-not-admitted:$constructorId)');
		if (construction.arguments.length != signature.arguments.length)
			return unsupported(right,
				'TNew(owned-field-argument-count=${construction.arguments.length},expected=${signature.arguments.length},target=$constructorId)');
		final arguments:Array<String> = [];
		for (index in 0...construction.arguments.length) {
			final argumentExpression = construction.arguments[index];
			final expected = signature.arguments[index].mapping;
			if (expected.classValue() != null || referencesStackConstructedValue(argumentExpression))
				return unsupported(argumentExpression, 'TNew(owned-field-argument:$index:class-alias-not-admitted)');
			arguments.push(coerce(lowerValue(argumentExpression, expected), expected, argumentExpression.pos,
				'TNew(owned-field-argument:$index,target=$constructorId)').id);
		}
		final virtualTable = dispatch.tableForInstance(child.instanceId);
		if (virtualTable != null) {
			appendInstruction(null, IRIOBindVirtualTable(target.place, virtualTable.input.id), HaxeSourceSpan.fromPosition(right.pos, input.sourcePath),
				"owned-class-field-bind-virtual-table");
		}
		if (!signature.input.elided) {
			final address:HxcIRResult = {
				id: nextValueId(),
				type: IRTPointer(IRTInstance(child.instanceId), false)
			};
			appendInstruction(address, IRIOAddress(target.place), HaxeSourceSpan.fromPosition(right.pos, input.sourcePath), "owned-class-field-address");
			registerValueTemporary(address.id, "owned-class-field-address");
			appendInstruction(null, IRIOCall({
				dispatch: IRCDDirect(constructorId),
				arguments: [address.id].concat(arguments),
				returnType: IRTVoid,
				failure: null
			}), HaxeSourceSpan.fromPosition(right.pos, input.sourcePath),
				"owned-class-field-constructor-call");
		}
		initializedOwnedClassFields.set(fieldName, true);
		return true;
	}

	/**
	 * Lower the statement prefix of a block through one shared sequence rule.
	 *
	 * Reflaxe can represent one value-producing expression as a block whose
	 * early entries declare and fill a compiler temporary. Value blocks need the
	 * same checked temporary recognition as ordinary statement blocks; otherwise
	 * a helper call inside `&&` or `||` can look like an unsafe uninitialized
	 * source local even though every generated branch assigns it.
	 */
	function lowerStatementSequence(expressions:Array<TypedExpr>, endExclusive:Int):Void {
		var index = 0;
		while (index < endExclusive) {
			if (index + 1 < endExclusive && tryLowerSpanLoop(expressions[index], expressions[index + 1])) {
				index += 2;
				continue;
			}
			final nested = expressions[index];
			switch nested.expr {
				case TVar(variable, null) if (sequenceFlowInitializesLocal(expressions, index + 1, endExclusive, variable.id)):
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

	/**
		Prove a flow carrier is assigned after compiler-hoisted prerequisite locals.

		Haxe may declare its Boolean result temporary first, then evaluate one or more
		call results into initialized locals before emitting the exhaustive `if` that
		assigns the temporary. Those declarations preserve source evaluation order and
		cannot observe the carrier. Stop at every other statement shape or any early
		reference, so a genuinely uninitialized source local remains rejected.
	**/
	function sequenceFlowInitializesLocal(expressions:Array<TypedExpr>, startIndex:Int, endExclusive:Int, compilerId:Int):Bool {
		var index = startIndex;
		final nestedCarriers:Array<Int> = [];
		while (index < endExclusive) {
			final candidate = expressions[index];
			if (followingFlowInitializesLocal(candidate, compilerId))
				return true;
			if (referencesCompilerLocal(candidate, compilerId))
				return false;
			var completesNestedCarrier = false;
			var nestedIndex = 0;
			while (nestedIndex < nestedCarriers.length && !completesNestedCarrier) {
				if (followingFlowInitializesLocal(candidate, nestedCarriers[nestedIndex])) {
					nestedCarriers.splice(nestedIndex, 1);
					completesNestedCarrier = true;
				} else {
					nestedIndex++;
				}
			}
			if (completesNestedCarrier) {
				index++;
				continue;
			}
			switch candidate.expr {
				case TVar(_, initializer) if (initializer != null && !expressionCreatesFlow(initializer)):
					// A normal initialized declaration is the only admitted prelude. Its
					// effects still run after the defensive carrier initialization.
				case TVar(nested, null) if (sequenceFlowInitializesLocal(expressions, index + 1, endExclusive, nested.id)):
					// One source lazy chain can make Haxe hoist several result carriers
					// together before their nested assigning branches. Admit the next
					// declaration only after proving that exact carrier independently;
					// recursion always advances, and neither carrier may be referenced here.
					nestedCarriers.push(nested.id);
				case _:
					return false;
			}
			index++;
		}
		return false;
	}

	/** True when one typed expression mentions the exact compiler local. */
	static function referencesCompilerLocal(expression:TypedExpr, compilerId:Int):Bool {
		var found = false;
		function visit(candidate:TypedExpr):Void {
			if (found)
				return;
			switch candidate.expr {
				case TLocal(variable) if (variable.id == compilerId):
					found = true;
				case _:
					TypedExprTools.iter(candidate, visit);
			}
		}
		visit(expression);
		return found;
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

		final bodyCleanupDepth = normalCleanupActionIds.length;
		final control = loopControl(exitBlock.id, incrementBlock.id, bodyCleanupDepth);
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
				appendScopedCleanupInstructions(bodyCleanupDepth, source);
				bodyEnd.terminator = {kind: IRTJump(edge(incrementBlock.id)), source: source};
			}
			restoreCleanupDepth(bodyCleanupDepth);
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
		restoreCleanupDepth(bodyCleanupDepth);
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

	function lowerVariable(variable:TVar, initializer:Null<TypedExpr>, position:Position, compilerFlowCarrier:Bool = false):Void {
		final ordinal = localOrdinal++;
		final localId = 'local.$ordinal';
		var stackReferenceAlias = false;
		if (initializer != null) {
			final construction = newExpression(initializer);
			if (construction != null
				&& !CBodyIntMapRecognition.isIntMap(construction.classReference)
				&& !CBodyStringMapRecognition.isStringMap(construction.classReference)) {
				// Constructor preparation already owns the admitted nominal class. Use
				// that plan to choose stack or collector storage instead of typing the
				// local a second time here. Re-typing would make an unsupported extern,
				// native-layout, or generic class fail at `TVar` before the established
				// `TNew` diagnostic can explain the actual constructor boundary.
				final constructionPath = CBodyConstructor.classPath(construction.classReference);
				final signature = constructorSignaturesById.get(CBodyConstructor.id(constructionPath));
				final constructedClass = signature == null ? null : signature.classValue;
				if (constructedClass == null || !constructedClass.managedByCollector) {
					lowerConstructedVariable(variable, initializer, construction, position, ordinal, localId);
					return;
				}
			}
			stackReferenceAlias = isDirectStackConstructedAlias(initializer);
			if (referencesStackConstructedValue(initializer) && !stackReferenceAlias) {
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
		// A switch-pattern binding views the active payload while its enum owner
		// remains live for the branch. Ref-counted Array and Bytes values therefore
		// borrow that owner instead of retaining and releasing a redundant local
		// copy. Returning the binding still fails closed because a borrowed local
		// has no cleanup owner that can be transferred across the function boundary.
		final borrowedEnumManagedPayload = initializer != null
			&& isEnumPayloadProjection(initializer)
			&& (localMapping.arrayValue() != null || localMapping.bytesValue() != null);
		final value:Null<LoweredValue> = switch initializer {
			case null if (compilerFlowCarrier):
				// Reflaxe can expose a value-producing if/switch as a temporary followed
				// by exhaustive control flow that assigns it. The structural recognition
				// below proves every arm assigns; this defensive value prevents C
				// uninitialized storage without becoming observable on an admitted path.
				switch localMapping.kind {
					case CBVKAggregate(_):
						// Strict C can safely declare a record carrier as `{0}`. Every
						// admitted source path overwrites it before the later load, while
						// a forged enum-abstract value takes the fail-stop switch edge.
						null;
					case _:
						final result:HxcIRResult = {id: nextValueId(), type: localMapping.irType};
						appendInstruction(result, IRIOConstant(defaultConstantAt(localMapping.irType, position, 'TVar(${variable.name}:flow-carrier)')),
							source, "flow-carrier-default");
						{id: result.id, type: result.type, mapping: localMapping};
				}
			case null:
				unsupportedAt(position, 'TVar(${variable.name}:uninitialized)');
			case expression:
				coerce(lowerValue(expression, localMapping), localMapping, expression.pos, 'TVar(${variable.name}:initializer)');
		};
		if (value != null && !stackReferenceAlias)
			rejectOwnedClassBorrow(value, position, 'TVar(${variable.name}:owned-class-borrow-escape)');
		final borrowedStackAlias = value != null && stackReferenceAlias && borrowedClassValueIds.exists(value.id);
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
		if (value == null) {
			appendInstruction(null, IRIODefaultInitialize(IRPLocal(localId), IRISUninitialized, IRISInitialized), source, "flow-carrier-default-initialize");
		} else {
			appendInstruction(null, IRIOInitialize(IRPLocal(localId), value.id, IRISUninitialized, IRISInitialized), source, "initialize");
		}
		final localArray = localMapping.arrayValue();
		if (localArray != null && !localArray.managedByCollector && !borrowedEnumManagedPayload) {
			final transferredFreshOwner = value != null && freshManagedArrayValueIds.remove(value.id);
			if (!transferredFreshOwner) {
				appendInstruction(null, IRIORetain(IRPLocal(localId), IRIRuntime("array")), source, "retain-array-alias");
				runtimeRequirements.push(new CBodyRuntimeRequirement("array", "retain", "ordinary Haxe Array local alias", source, position));
			}
			final cleanupId = 'array-local.$ordinal.release';
			constructionCleanupActions.push({
				id: cleanupId,
				idempotence: IRCExactlyOnce,
				kind: IRCARelease(IRPLocal(localId), IRIRuntime("array")),
				source: source
			});
			normalCleanupActionIds.push(cleanupId);
			arrayCleanupActionIdsByCompilerId.set(variable.id, cleanupId);
			runtimeRequirements.push(new CBodyRuntimeRequirement("array", "cleanup-release", "ordinary Haxe Array local lifetime", source, position));
		}
		if (localMapping.stringMapValue() != null) {
			final transferredFreshOwner = value != null && freshManagedStringMapValueIds.remove(value.id);
			if (!transferredFreshOwner) {
				appendInstruction(null, IRIORetain(IRPLocal(localId), IRIRuntime("string-map")), source, "retain-string-map-alias");
				runtimeRequirements.push(new CBodyRuntimeRequirement("string-map", "retain", "ordinary Haxe StringMap local alias", source, position));
			}
			final cleanupId = 'string-map-local.$ordinal.release';
			constructionCleanupActions.push({
				id: cleanupId,
				idempotence: IRCExactlyOnce,
				kind: IRCARelease(IRPLocal(localId), IRIRuntime("string-map")),
				source: source
			});
			normalCleanupActionIds.push(cleanupId);
			stringMapCleanupActionIdsByCompilerId.set(variable.id, cleanupId);
			runtimeRequirements.push(new CBodyRuntimeRequirement("string-map", "cleanup-release", "ordinary Haxe StringMap local lifetime", source, position));
		}
		if (localMapping.intMapValue() != null) {
			final transferredFreshOwner = value != null && freshManagedIntMapValueIds.remove(value.id);
			if (!transferredFreshOwner) {
				appendInstruction(null, IRIORetain(IRPLocal(localId), IRIRuntime("int-map")), source, "retain-int-map-alias");
				runtimeRequirements.push(new CBodyRuntimeRequirement("int-map", "retain", "ordinary Haxe IntMap local alias", source, position));
			}
			final cleanupId = 'int-map-local.$ordinal.release';
			constructionCleanupActions.push({
				id: cleanupId,
				idempotence: IRCExactlyOnce,
				kind: IRCARelease(IRPLocal(localId), IRIRuntime("int-map")),
				source: source
			});
			normalCleanupActionIds.push(cleanupId);
			intMapCleanupActionIdsByCompilerId.set(variable.id, cleanupId);
			runtimeRequirements.push(new CBodyRuntimeRequirement("int-map", "cleanup-release", "ordinary Haxe IntMap local lifetime", source, position));
		}
		if (localMapping.bytesValue() != null && !borrowedEnumManagedPayload) {
			final transferredFreshOwner = value != null && freshManagedBytesValueIds.remove(value.id);
			if (!transferredFreshOwner) {
				appendInstruction(null, IRIORetain(IRPLocal(localId), IRIRuntime("bytes")), source, "retain-bytes-alias");
				runtimeRequirements.push(new CBodyRuntimeRequirement("bytes", "retain", "ordinary haxe.io.Bytes local alias", source, position));
			}
			final cleanupId = 'bytes-local.$ordinal.release';
			constructionCleanupActions.push({
				id: cleanupId,
				idempotence: IRCExactlyOnce,
				kind: IRCARelease(IRPLocal(localId), IRIRuntime("bytes")),
				source: source
			});
			normalCleanupActionIds.push(cleanupId);
			bytesCleanupActionIdsByCompilerId.set(variable.id, cleanupId);
			runtimeRequirements.push(new CBodyRuntimeRequirement("bytes", "cleanup-release", "ordinary haxe.io.Bytes local lifetime", source, position));
		}
		final managedEnum = localMapping.enumValue();
		if (managedEnum != null && managedEnum.managedLifetime) {
			final retainId = managedEnum.retainImplementationId();
			final destroyId = managedEnum.destroyImplementationId();
			if (retainId == null || destroyId == null)
				throw new CBodyEmissionError('managed enum `${managedEnum.instanceId}` lost its ownership plan');
			final transferredFreshOwner = value != null && freshManagedEnumValueIds.remove(value.id);
			if (!transferredFreshOwner)
				appendInstruction(null, IRIORetain(IRPLocal(localId), IRIProgramLocal(retainId)), source, "retain-enum-alias");
			final cleanupId = 'enum-local.$ordinal.release';
			constructionCleanupActions.push({
				id: cleanupId,
				idempotence: IRCExactlyOnce,
				kind: IRCARelease(IRPLocal(localId), IRIProgramLocal(destroyId)),
				source: source
			});
			normalCleanupActionIds.push(cleanupId);
			enumCleanupActionIdsByCompilerId.set(variable.id, cleanupId);
		}
		final managedAggregate = localMapping.aggregateValue();
		if (managedAggregate != null && managedAggregate.managedLifetime) {
			final retainId = managedAggregate.retainImplementationId();
			final destroyId = managedAggregate.destroyImplementationId();
			if (retainId == null || destroyId == null)
				throw new CBodyEmissionError('managed aggregate `${managedAggregate.instanceId}` lost its ownership plan');
			final transferredFreshOwner = value != null && freshManagedAggregateValueIds.remove(value.id);
			if (!transferredFreshOwner)
				appendInstruction(null, IRIORetain(IRPLocal(localId), IRIProgramLocal(retainId)), source, "retain-record-alias");
			final cleanupId = 'record-local.$ordinal.release';
			constructionCleanupActions.push({
				id: cleanupId,
				idempotence: IRCExactlyOnce,
				kind: IRCARelease(IRPLocal(localId), IRIProgramLocal(destroyId)),
				source: source
			});
			normalCleanupActionIds.push(cleanupId);
			aggregateCleanupActionIdsByCompilerId.set(variable.id, cleanupId);
		}
		final managedOptional = localMapping.optionalValue();
		if (managedOptional != null && managedOptional.managedLifetime) {
			final retainId = managedOptional.retainImplementationId();
			final destroyId = managedOptional.destroyImplementationId();
			if (retainId == null || destroyId == null)
				throw new CBodyEmissionError('managed optional `${managedOptional.planId}` lost its ownership plan');
			final transferredFreshOwner = value != null && freshManagedOptionalValueIds.remove(value.id);
			if (!transferredFreshOwner)
				appendInstruction(null, IRIORetain(IRPLocal(localId), IRIProgramLocal(retainId)), source, "retain-optional-alias");
			final cleanupId = 'optional-local.$ordinal.release';
			constructionCleanupActions.push({
				id: cleanupId,
				idempotence: IRCExactlyOnce,
				kind: IRCARelease(IRPLocal(localId), IRIProgramLocal(destroyId)),
				source: source
			});
			normalCleanupActionIds.push(cleanupId);
			optionalCleanupActionIdsByCompilerId.set(variable.id, cleanupId);
		}
		localIdsByCompilerId.set(variable.id, localId);
		localTypesByCompilerId.set(variable.id, localMapping);
		if (borrowedStackAlias) {
			// Haxe introduces locals such as `_this = parent.child` while inlining.
			// The pointer local is a stable non-owning name, not a second object: it
			// can therefore be reloaded inside a later short-circuit block without
			// losing the parent's lifetime. Keep the borrow fact beside the local so
			// every reload retains escape checks, and reject reassignment in
			// `lowerPlace` below.
			borrowedClassLocalIds.set(localId, true);
		}
		if (stackReferenceAlias)
			stackConstructedCompilerIds.set(variable.id, true);
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
		final argumentExpressions = completeDirectCallArguments(expression, construction.arguments, signature.arguments, 0, targetId, "constructor-argument");
		final localMapping = bodyValueType(variable.t, position, 'TVar(${variable.name}:constructed-type)');
		if (localMapping.classValue() == null)
			unsupportedAt(position, 'TNew(non-class-local:${variable.name})');

		// Haxe evaluates constructor arguments before entering the constructor body.
		final arguments:Array<String> = [];
		for (index in 0...argumentExpressions.length) {
			final argumentExpression = argumentExpressions[index];
			if (referencesStackConstructedValue(argumentExpression) && !signature.arguments[index].borrowedReference) {
				unsupported(argumentExpression, 'TNew(stack-reference-escape:constructor-argument:$index)');
			}
			var argument = coerce(lowerValue(argumentExpression, signature.arguments[index].mapping), signature.arguments[index].mapping,
				argumentExpression.pos, 'TNew(argument:$index,target=$targetId)');
			argument = stabilizeFreshManagedArray(argument, argumentExpression.pos, 'constructor-argument-$index');
			rejectOwnedClassBorrow(argument, argumentExpression.pos, 'TNew(owned-class-borrow-escape:constructor-argument:$index)');
			arguments.push(argument.id);
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
		normalCleanupActionIds.push(initializedActionId);
		final constructedClass = localMapping.classValue();
		if (constructedClass == null)
			throw new CBodyEmissionError('constructed local `${variable.name}` lost its class layout');
		for (field in managedArrayFields(constructedClass)) {
			final cleanupId = 'construction.$constructionOrdinal.array-field.${field.name}.release';
			constructionCleanupActions.push({
				id: cleanupId,
				idempotence: IRCExactlyOnce,
				kind: IRCARelease(IRPField(IRPLocal(backingLocalId), field.name), IRIRuntime("array")),
				source: source
			});
			normalCleanupActionIds.push(cleanupId);
			runtimeRequirements.push(new CBodyRuntimeRequirement("array", "cleanup-release",
				'ordinary Haxe Array field `${constructedClass.haxePath}.${field.name}` lifetime', source, expression.pos));
		}
		for (field in managedStringMapFields(constructedClass)) {
			final cleanupId = 'construction.$constructionOrdinal.string-map-field.${field.name}.release';
			constructionCleanupActions.push({
				id: cleanupId,
				idempotence: IRCExactlyOnce,
				kind: IRCARelease(IRPField(IRPLocal(backingLocalId), field.name), IRIRuntime("string-map")),
				source: source
			});
			normalCleanupActionIds.push(cleanupId);
			runtimeRequirements.push(new CBodyRuntimeRequirement("string-map", "cleanup-release",
				'ordinary Haxe StringMap field `${constructedClass.haxePath}.${field.name}` lifetime', source, expression.pos));
		}
	}

	/**
		Allocate one escaping class through the precise collector.

		`hxc_gc_allocate` returns zeroed, stable storage. The allocation instruction's
		result is an exact managed value, so the root planner publishes it before the
		following constructor call. A constructor can therefore allocate another
		managed object without losing the object that is still being initialized.
	**/
	function lowerManagedConstructedValue(expression:TypedExpr, construction:BodyNewExpression, expectedMapping:Null<CBodyValueType>):LoweredValue {
		// Select allocation from the `new ConcreteClass(...)` expression itself.
		// Its surrounding context may expect an interface value; using that fat
		// interface type as the allocation type would lose the concrete descriptor
		// and dispatch table needed below.
		final sourceMapping = bodyValueType(expression.t, expression.pos, "TNew(managed-result-type)");
		final classValue = sourceMapping.classValue();
		if (classValue == null || !classValue.managedByCollector)
			return unsupported(expression, "TNew(stack-construction-requires-direct-local)");
		final classPath = CBodyConstructor.classPath(construction.classReference);
		if (classPath != classValue.haxePath)
			return unsupported(expression, 'TNew(managed-class-type-mismatch:$classPath->${classValue.haxePath})');
		final targetId = CBodyConstructor.id(classPath);
		final signature = constructorSignaturesById.get(targetId);
		if (signature == null)
			return unsupported(expression, 'TNew(unavailable-constructor:$targetId)');
		if (signature.input.canFail)
			return unsupported(expression, 'TNew(managed-fallible-constructor-not-yet-admitted:$targetId)');
		final argumentExpressions = completeDirectCallArguments(expression, construction.arguments, signature.arguments, 0, targetId,
			"managed-constructor-argument");

		final arguments:Array<String> = [];
		for (index in 0...argumentExpressions.length) {
			final sourceArgument = argumentExpressions[index];
			var argument = coerce(lowerValue(sourceArgument, signature.arguments[index].mapping), signature.arguments[index].mapping, sourceArgument.pos,
				'TNew(managed-argument:$index,target=$targetId)');
			argument = stabilizeFreshManagedArray(argument, sourceArgument.pos, 'managed-constructor-argument-$index');
			rejectOwnedClassBorrow(argument, sourceArgument.pos, 'TNew(owned-class-borrow-escape:managed-constructor-argument:$index)');
			arguments.push(argument.id);
		}

		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final allocated:HxcIRResult = {id: nextValueId(), type: signature.selfMapping.irType};
		appendInstruction(allocated, IRIOAllocate(IRTInstance(classValue.instanceId), IRAShared, IRIRuntime("gc"), {
			kind: IRFAllocationFailure,
			target: IRFTAbort,
			arguments: [],
			cleanup: []
		}), source, "managed-class-allocate");
		runtimeRequirements.push(new CBodyRuntimeRequirement("gc", "allocation", 'escaping Haxe class `${classValue.haxePath}` allocation', source,
			expression.pos));
		registerValueTemporary(allocated.id, "managed-class-result");
		final self:LoweredValue = {id: allocated.id, type: allocated.type, mapping: signature.selfMapping};
		if (!signature.input.elided) {
			final target = functionsById.get(targetId);
			if (target == null)
				throw new CBodyEmissionError('non-elided managed constructor `$targetId` has no prepared function');
			appendInstruction(null, IRIOCall({
				dispatch: IRCDDirect(targetId),
				arguments: [self.id].concat(arguments),
				returnType: IRTVoid,
				failure: null
			}), source, "managed-constructor-call");
		}
		final target = expectedMapping == null ? sourceMapping : expectedMapping;
		return coerce(self, target, expression.pos, 'TNew(managed-result:$targetId)');
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
		final argumentExpressions = completeDirectCallArguments(expression, callArguments, target.arguments, 0, baseId, "super-argument");
		final self = selfValue;
		if (self == null)
			throw new CBodyEmissionError('constructor `${prepared.irId}` lost its self parameter');
		final baseSelf = coerce(self, target.selfMapping, expression.pos, 'TCall(super:self,target=$baseId)');
		final arguments:Array<String> = [baseSelf.id];
		for (index in 0...argumentExpressions.length) {
			final argument = argumentExpressions[index];
			if (referencesStackConstructedValue(argument))
				unsupported(argument, 'TNew(stack-reference-escape:super-argument:$index)');
			var converted = coerce(lowerValue(argument, target.arguments[index].mapping), target.arguments[index].mapping, argument.pos,
				'TCall(super:argument:$index,target=$baseId)');
			converted = stabilizeFreshManagedArray(converted, argument.pos, 'super-constructor-argument-$index');
			rejectOwnedClassBorrow(converted, argument.pos, 'TCall(owned-class-borrow-escape:super-argument:$index)');
			arguments.push(converted.id);
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
		rejectOwnedClassBorrow(value, valueExpression.pos, "TThrow(owned-class-borrow-escape)");
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

	function normalCleanupSteps(?excludedActionId:String):Array<HxcIRCleanupStep> {
		final result:Array<HxcIRCleanupStep> = [];
		var index = normalCleanupActionIds.length;
		while (index > 0) {
			final actionId = normalCleanupActionIds[--index];
			if (actionId != excludedActionId)
				result.push({regionId: "cleanup.construction", actionId: actionId});
		}
		return result;
	}

	/** Emit branch-local releases before leaving the C lexical scope that owns them. */
	function appendScopedCleanupInstructions(depth:Int, source:HxcSourceSpan):Void {
		var index = normalCleanupActionIds.length;
		while (index > depth) {
			final actionId = normalCleanupActionIds[--index];
			var found:Null<HxcIRCleanupAction> = null;
			for (action in constructionCleanupActions)
				if (action.id == actionId)
					found = action;
			if (found == null)
				throw new CBodyEmissionError('branch-local cleanup `$actionId` in `${prepared.irId}` lost its typed action');
			switch found.kind {
				case IRCARelease(place, implementation):
					appendInstruction(null, IRIORelease(place, implementation), source, "release-branch-local-owner");
				case _:
					throw new CBodyEmissionError('branch-local cleanup `$actionId` in `${prepared.irId}` is not a managed release');
			}
		}
	}

	/** Stop branch-local owners from leaking into later sibling or join cleanup. */
	function restoreCleanupDepth(depth:Int):Void
		while (normalCleanupActionIds.length > depth)
			normalCleanupActionIds.pop();

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

	/**
		Recognize one same-function pointer alias without losing stack provenance.

		Haxe may introduce a local `_this` while inlining an instance method. An
		ordinary authored `var alias = value` has the same lifetime: storing the
		pointer in another automatic local does not itself make the object escape.
		The caller records the alias as stack-backed, so later return, assignment,
		unknown-call, and constructor-argument checks still reject real escapes.
	**/
	function isDirectStackConstructedAlias(expression:TypedExpr):Bool {
		return switch expression.expr {
			case TLocal(variable): stackConstructedCompilerIds.exists(variable.id);
			case TField(receiver, _) if (isClassReferenceType(expression.t)): // A final owned child is embedded inside the parent object. Its address
				// may be named by another automatic local for the same bounded lifetime.
				// In an ordinary instance method, current `this` is caller-owned for
				// exactly this call. Admit that one alias shape here without broadly
				// classifying `this` as stack construction: return/storage checks then
				// retain their more precise owned-child-borrow diagnostics.
				referencesStackConstructedValue(receiver)
				|| selfValue != null
				&& isThisExpression(receiver);
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isDirectStackConstructedAlias(inner);
			case _: false;
		};
	}

	static function isThisExpression(expression:TypedExpr):Bool {
		return switch expression.expr {
			case TConst(TThis): true;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isThisExpression(inner);
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
				final borrowSource = requireSpanSource(expression, mutable);
				if (typeKey(borrowSource.element.irType) != typeKey(collectionType.element.irType)) {
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
				appendInstruction(null, IRIOInitializeSpan(IRPLocal(localId), borrowSource.place, IRISUninitialized, IRISInitialized), source,
					"span-initialize");
				collectionBindingsByCompilerId.set(variable.id, {
					localId: localId,
					kind: collectionType.kind,
					element: collectionType.element,
					length: borrowSource.length
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

	function requireSpanSource(expression:TypedExpr, mutable:Bool):BodyFixedArrayBorrowSource {
		return switch expression.expr {
			case TCall(callee, [argument]) if (isAbstractMethod(callee, "c.CArray", mutable ? "span" : "constSpan")):
				requireFixedArrayBorrowSource(argument, mutable);
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): requireSpanSource(inner, mutable);
			case _: unsupported(expression, 'TVar(${mutable ? "Span" : "ConstSpan"}:requires-fixed-array-borrow)');
		};
	}

	function requireFixedArrayBorrowSource(expression:TypedExpr, mutable:Bool):BodyFixedArrayBorrowSource {
		return switch unwrapExpression(expression).expr {
			case TLocal(variable):
				final binding = collectionBindingsByCompilerId.get(variable.id);
				if (binding == null)
					unsupported(expression, 'TCall(${mutable ? "span" : "constSpan"}:source-outside-admitted-fixed-array-place)');
				switch binding.kind {
					case BCKFixedArray(witnessId):
						final length = binding.length;
						if (length == null)
							unsupported(expression, 'TCall(${mutable ? "span" : "constSpan"}:fixed-array-length-missing)');
						{
							place: IRPLocal(binding.localId),
							element: binding.element,
							length: length,
							witnessId: witnessId
						};
					case BCKSpan(_): unsupported(expression, 'TCall(${mutable ? "span" : "constSpan"}:span-source-not-fixed-array)');
				}
			case TField(_, FInstance(_, _, _)):
				final source = lowerPlace(expression);
				final fixed = source.mapping.fixedArrayShape();
				if (fixed == null)
					unsupported(expression, 'TCall(${mutable ? "span" : "constSpan"}:field-source-not-fixed-array)');
				{
					place: source.place,
					element: fixed.element,
					length: fixed.length,
					witnessId: fixed.witnessId
				};
			case _: unsupported(expression, 'TCall(${mutable ? "span" : "constSpan"}:source=${nodeName(expression)})');
		};
	}

	function requireLocalVariable(expression:TypedExpr, owner:String):TVar {
		return switch expression.expr {
			case TLocal(variable): variable;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): requireLocalVariable(inner, owner);
			case _: unsupported(expression, '$owner:source=${nodeName(expression)}');
		};
	}

	function followingFlowInitializesLocal(expression:TypedExpr, compilerId:Int):Bool {
		return switch expression.expr {
			case TIf(_, whenTrue, whenFalse): whenFalse != null && definitelyAssignsLocal(whenTrue,
					compilerId) && definitelyAssignsLocal(whenFalse, compilerId);
			case TSwitch(subject, cases, defaultExpression): switchArmsAssignLocal(subject, cases, defaultExpression, compilerId);
			case TParenthesis(inner) | TMeta(_, inner): followingFlowInitializesLocal(inner, compilerId);
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
			case TSwitch(subject, cases, defaultExpression): switchArmsAssignLocal(subject, cases, defaultExpression, compilerId);
			case TParenthesis(inner) | TMeta(_, inner): definitelyAssignsLocal(inner, compilerId);
			case _: false;
		};
	}

	function switchArmsAssignLocal(subject:TypedExpr, cases:Array<TypedSwitchArm>, defaultExpression:Null<TypedExpr>, compilerId:Int):Bool {
		if (defaultExpression == null) {
			if (!isExhaustiveEnumAbstractSwitch(subject, cases)) {
				return false;
			}
		} else if (!definitelyAssignsLocal(defaultExpression, compilerId)) {
			return false;
		}
		for (item in cases) {
			if (!definitelyAssignsLocal(item.expr, compilerId)) {
				return false;
			}
		}
		return true;
	}

	/**
	 * Prove that a default-free value switch covers a closed enum abstract.
	 *
	 * An enum abstract is stored as its underlying integer or Boolean, but Haxe
	 * still treats its declared `var` values as a finite set during exhaustiveness
	 * checking. The typed switch no longer carries that proof explicitly, so this
	 * compiler boundary rebuilds it from the abstract's `@:enum` fields. This is
	 * deliberately narrower than asking whether all visible integer literals
	 * happen to be present: ordinary integers are open-ended and must retain a
	 * real default or fall-through path.
	 */
	function isExhaustiveEnumAbstractSwitch(subject:TypedExpr, cases:Array<TypedSwitchArm>):Bool {
		final expected = enumAbstractConstantKeys(subject.t);
		if (expected == null || expected.length == 0) {
			return false;
		}
		final covered:Map<String, Bool> = [];
		for (item in cases) {
			for (value in item.values) {
				final key = typedSwitchConstantKey(value);
				if (key == null) {
					return false;
				}
				covered.set(key, true);
			}
		}
		for (key in expected) {
			if (!covered.exists(key)) {
				return false;
			}
		}
		return true;
	}

	/** Return the distinct runtime constants declared by one enum abstract. */
	function enumAbstractConstantKeys(type:Type):Null<Array<String>> {
		return switch applyCurrentSpecialization(type) {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : enumAbstractConstantKeys(resolved);
			case TLazy(resolve): enumAbstractConstantKeys(resolve());
			case TType(reference, parameters):
				final definition = reference.get();
				enumAbstractConstantKeys(TypeTools.applyTypeParameters(definition.type, definition.params, parameters));
			case TAbstract(reference, _) if (reference.get().meta.has(":enum")):
				final implementation = reference.get().impl;
				if (implementation == null) {
					null;
				} else {
					final result:Array<String> = [];
					final seen:Map<String, Bool> = [];
					for (field in implementation.get().statics.get()) {
						if (!field.meta.has(":enum")) {
							continue;
						}
						final expression = field.expr();
						if (expression == null) {
							return null;
						}
						final key = typedSwitchConstantKey(expression);
						if (key == null) {
							return null;
						}
						if (!seen.exists(key)) {
							seen.set(key, true);
							result.push(key);
						}
					}
					result;
				}
			case _: null;
		};
	}

	/** Read only the integral constant forms that strict C switches can own. */
	static function typedSwitchConstantKey(expression:TypedExpr, depth:Int = 0):Null<String> {
		if (depth > 8) {
			return null;
		}
		return switch expression.expr {
			case TConst(TInt(value)): 'int:$value';
			case TConst(TBool(value)): 'bool:${value ? "true" : "false"}';
			case TUnop(OpNeg, _, inner): final value = constantInt(inner); value == null || value == -2147483648 ? null : 'int:${- value}';
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): typedSwitchConstantKey(inner, depth + 1);
			case TField(_, FStatic(_, field)):
				final value = field.get().expr();
				value == null ? null : typedSwitchConstantKey(value, depth + 1);
			case _: null;
		};
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
		final returnedArray = prepared.returnMapping.arrayValue();
		final lowered = coerce(lowerValue(value, prepared.returnMapping), prepared.returnMapping, value.pos, "TReturn(value)");
		if (borrowedManagedArrayElementValueIds.exists(lowered.id))
			unsupported(value, "TReturn(borrowed-managed-Array-element-needs-owner-transfer)");
		if (returnedArray != null && !returnedArray.managedByCollector) {
			if (isNullConstantExpression(value)) {
				// NULL owns no container, so it crosses the return boundary without a
				// retain/release pair. This keeps the generated C as direct as the
				// source while the runtime remains null-safe for dynamic paths.
				currentBlock.terminator = {kind: IRTReturn(lowered.id, normalCleanupSteps()), source: source};
				return;
			}
			var transferredCleanupId:Null<String> = null;
			var returnedValueId = lowered.id;
			if (!freshManagedArrayValueIds.remove(lowered.id)) {
				final returnedLocal = directLocalCompilerId(value);
				if (returnedLocal != null)
					transferredCleanupId = arrayCleanupActionIdsByCompilerId.get(returnedLocal);
				if (transferredCleanupId == null) {
					final ownerLocalId = createFlowLocal(prepared.returnMapping, lowered.id, source, "returned-array-owner");
					appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIRuntime("array")), source, "retain-array-return");
					runtimeRequirements.push(new CBodyRuntimeRequirement("array", "retain", "ordinary Haxe Array borrowed return", source, value.pos));
					returnedValueId = loadPlace({place: IRPLocal(ownerLocalId), mapping: prepared.returnMapping, mutable: false}, value.pos,
						"returned-array-owned-load").id;
				}
			}
			currentBlock.terminator = {kind: IRTReturn(returnedValueId, normalCleanupSteps(transferredCleanupId)), source: source};
			return;
		}
		if (prepared.returnMapping.stringMapValue() != null) {
			var transferredCleanupId:Null<String> = null;
			var returnedValueId = lowered.id;
			if (!freshManagedStringMapValueIds.remove(lowered.id)) {
				final returnedLocal = directLocalCompilerId(value);
				if (returnedLocal != null)
					transferredCleanupId = stringMapCleanupActionIdsByCompilerId.get(returnedLocal);
				if (transferredCleanupId == null) {
					final ownerLocalId = createFlowLocal(prepared.returnMapping, lowered.id, source, "returned-string-map-owner");
					appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIRuntime("string-map")), source, "retain-string-map-return");
					runtimeRequirements.push(new CBodyRuntimeRequirement("string-map", "retain", "ordinary Haxe StringMap borrowed return", source, value.pos));
					returnedValueId = loadPlace({place: IRPLocal(ownerLocalId), mapping: prepared.returnMapping, mutable: false}, value.pos,
						"returned-string-map-owned-load").id;
				}
			}
			currentBlock.terminator = {kind: IRTReturn(returnedValueId, normalCleanupSteps(transferredCleanupId)), source: source};
			return;
		}
		if (prepared.returnMapping.bytesValue() != null) {
			var transferredCleanupId:Null<String> = null;
			if (!freshManagedBytesValueIds.remove(lowered.id)) {
				final returnedLocal = directLocalCompilerId(value);
				if (returnedLocal == null)
					unsupported(value, "TReturn(managed-Bytes-borrowed-return-needs-retain)");
				transferredCleanupId = bytesCleanupActionIdsByCompilerId.get(returnedLocal);
				if (transferredCleanupId == null)
					unsupported(value, "TReturn(managed-Bytes-borrowed-return-needs-retain)");
			}
			currentBlock.terminator = {kind: IRTReturn(lowered.id, normalCleanupSteps(transferredCleanupId)), source: source};
			return;
		}
		final returnedEnum = prepared.returnMapping.enumValue();
		if (returnedEnum != null && returnedEnum.managedLifetime) {
			var transferredCleanupId:Null<String> = null;
			var returnedValueId = lowered.id;
			if (!freshManagedEnumValueIds.remove(lowered.id)) {
				final returnedLocal = directLocalCompilerId(value);
				if (returnedLocal != null)
					transferredCleanupId = enumCleanupActionIdsByCompilerId.get(returnedLocal);
				if (transferredCleanupId == null) {
					final retainId = returnedEnum.retainImplementationId();
					if (retainId == null)
						throw new CBodyEmissionError('managed enum `${returnedEnum.instanceId}` lost its retain plan');
					final ownerLocalId = createFlowLocal(prepared.returnMapping, lowered.id, source, "returned-enum-owner");
					appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIProgramLocal(retainId)), source, "retain-enum-return");
					returnedValueId = loadPlace({place: IRPLocal(ownerLocalId), mapping: prepared.returnMapping, mutable: false}, value.pos,
						"returned-enum-owned-load").id;
				}
			}
			currentBlock.terminator = {kind: IRTReturn(returnedValueId, normalCleanupSteps(transferredCleanupId)), source: source};
			return;
		}
		final returnedAggregate = prepared.returnMapping.aggregateValue();
		if (returnedAggregate != null && returnedAggregate.managedLifetime) {
			var transferredCleanupId:Null<String> = null;
			var returnedValueId = lowered.id;
			if (!freshManagedAggregateValueIds.remove(lowered.id)) {
				final returnedLocal = directLocalCompilerId(value);
				if (returnedLocal != null)
					transferredCleanupId = aggregateCleanupActionIdsByCompilerId.get(returnedLocal);
				if (transferredCleanupId == null) {
					final retainId = returnedAggregate.retainImplementationId();
					if (retainId == null)
						throw new CBodyEmissionError('managed aggregate `${returnedAggregate.instanceId}` lost its retain plan');
					final ownerLocalId = createFlowLocal(prepared.returnMapping, lowered.id, source, "returned-record-owner");
					appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIProgramLocal(retainId)), source, "retain-record-return");
					returnedValueId = loadPlace({place: IRPLocal(ownerLocalId), mapping: prepared.returnMapping, mutable: false}, value.pos,
						"returned-record-owned-load").id;
				}
			}
			currentBlock.terminator = {kind: IRTReturn(returnedValueId, normalCleanupSteps(transferredCleanupId)), source: source};
			return;
		}
		final returnedOptional = prepared.returnMapping.optionalValue();
		if (returnedOptional != null && returnedOptional.managedLifetime) {
			var transferredCleanupId:Null<String> = null;
			var returnedValueId = lowered.id;
			if (!freshManagedOptionalValueIds.remove(lowered.id)) {
				final returnedLocal = directLocalCompilerId(value);
				if (returnedLocal != null)
					transferredCleanupId = optionalCleanupActionIdsByCompilerId.get(returnedLocal);
				if (transferredCleanupId == null) {
					final retainId = returnedOptional.retainImplementationId();
					if (retainId == null)
						throw new CBodyEmissionError('managed optional `${returnedOptional.planId}` lost its retain plan');
					final ownerLocalId = createFlowLocal(prepared.returnMapping, lowered.id, source, "returned-optional-owner");
					appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIProgramLocal(retainId)), source, "retain-optional-return");
					returnedValueId = loadPlace({place: IRPLocal(ownerLocalId), mapping: prepared.returnMapping, mutable: false}, value.pos,
						"returned-optional-owned-load").id;
				}
			}
			currentBlock.terminator = {kind: IRTReturn(returnedValueId, normalCleanupSteps(transferredCleanupId)), source: source};
			return;
		}
		rejectOwnedClassBorrow(lowered, value.pos, "TReturn(owned-class-borrow-escape)");
		currentBlock.terminator = {kind: IRTReturn(lowered.id, normalCleanupSteps()), source: source};
	}

	/** Find a named local whose existing owner can move across a return boundary. */
	static function directLocalCompilerId(expression:TypedExpr):Null<Int> {
		return switch expression.expr {
			case TLocal(variable): variable.id;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): directLocalCompilerId(inner);
			case _: null;
		};
	}

	function lowerValue(expression:TypedExpr, ?expectedMapping:CBodyValueType):LoweredValue {
		return switch expression.expr {
			case TConst(constant): lowerConstant(expression, constant, expectedMapping);
			case TLocal(variable): lowerLocal(expression, variable);
			case TArray(collection, index):
				if (CBodyArrayRecognition.isCoreArrayType(collection.t)) {
					final collectionType = bodyValueType(collection.t, collection.pos, "TArray(collection-type)");
					lowerManagedArrayGet(expression, collection, index, collectionType);
				} else {
					loadPlace(lowerPlace(expression), expression.pos, "collection-index-load");
				}
			case TArrayDecl(elements): lowerManagedArrayLiteral(expression, elements, expectedMapping);
			case TObjectDecl(fields): lowerAggregateLiteral(expression, fields, expectedMapping);
			case TField(_, FEnum(enumReference, enumField)) if (isFunctionType(expression.t)):
				lowerEnumConstructorFunctionReference(expression, enumReference, enumField, expectedMapping);
			case TField(_, FEnum(enumReference, enumField)):
				final imported = aggregateRegistry.importEnumConstant(enumReference, enumField, expression.pos, input.sourcePath);
				imported == null ? lowerEnumConstructor(expression, enumReference, enumField, [],
					expectedMapping) : lowerImportConstant(expression, imported, expectedMapping);
			case TField(receiver, FAnon(fieldReference)): lowerAggregateField(expression, receiver, fieldReference.get().name);
			case TField(receiver, FInstance(owner, _, fieldReference)) if (CBodyArrayRecognition.isCoreArray(owner)
				&& fieldReference.get().name == "length"):
				lowerManagedArrayLength(expression, receiver);
			case TField(receiver, FInstance(owner, _, fieldReference)) if (CBodyBytesRecognition.isCoreBytes(owner)
				&& fieldReference.get().name == "length"):
				lowerManagedBytesLength(expression, receiver);
			case TField(receiver, FInstance(_, _, fieldReference)):
				final receiverType = bodyValueType(receiver.t, receiver.pos, 'TField(${fieldReference.get().name}:receiver-type)');
				receiverType.importedStructValue() == null ? lowerClassField(expression, receiver,
					fieldReference.get().name) : lowerImportedField(expression, receiver, fieldReference.get().name, receiverType);
			case TField(_, FStatic(classReference, fieldReference)) if (isFunctionType(expression.t)):
				lowerStaticFunctionReference(expression, classReference, fieldReference, expectedMapping);
			case TField(_, FStatic(classReference, fieldReference)):
				final imported = aggregateRegistry.importStaticConstant(classReference, fieldReference, expression.pos, input.sourcePath);
				imported == null ? lowerStaticField(expression, classReference, fieldReference) : lowerImportConstant(expression, imported, expectedMapping);
			case TParenthesis(inner): lowerValue(inner, expectedMapping);
			case TMeta(_, inner): lowerValue(inner, expectedMapping);
			case TBlock(expressions): lowerValueBlock(expression, expressions, expectedMapping);
			case TCast(inner, _) if (CBodyIntMapRecognition.isIMapType(expression.t)
				|| CBodyStringMapRecognition.isIMapType(expression.t)):
				final innerMapping = bodyValueType(inner.t, inner.pos, "TCast(Map-interface-view:inner-type)");
				if (innerMapping.intMapValue() == null && innerMapping.stringMapValue() == null)
					unsupported(expression, "TCast(Map-interface-view:inner-not-admitted-map)");
				lowerValue(inner, expectedMapping == null ? innerMapping : expectedMapping);
			case TCast(inner, _):
				final typedTarget = bodyValueType(expression.t, expression.pos, "TCast(target-type)");
				// Haxe can insert an intermediate `Null<Int>` cast while typing a
				// literal for a declared `Null<UInt>` field. Keep the enclosing
				// optional expectation through that wrapper so the constant selects
				// UInt before it is injected. A non-constant incompatible optional
				// still reaches `coerce` and fails closed.
				final target = expectedMapping != null
					&& typedTarget.optionalValue() != null
					&& expectedMapping.optionalValue() != null ? expectedMapping : typedTarget;
				switch target.kind {
					case CBVKPrimitive(primitive):
						switch tryLowerUIntIntrinsic(expression, inner, primitive) {
							case UIIntrinsicLowered(value): value;
							case UIIntrinsicNotMatched: coerce(lowerValue(inner), target, expression.pos, "TCast");
						}
					case CBVKFixedArray(_, _, _):
						unsupported(expression, "TCast(fixed-array)");
					case CBVKOwnedClass(_):
						unsupported(expression, "TCast(owned-class-field)");
					case CBVKInterface(_):
						// A checked Haxe cast whose source class already proves the target
						// interface cannot fail at runtime. Reuse the same typed interface
						// construction as an implicit assignment; genuinely dynamic casts
						// still reach `coerce`'s fail-closed runtime-proof diagnostic.
						coerce(lowerValue(inner), target, expression.pos, "TCast(interface)");
					case CBVKStaticString(_) | CBVKSpan(_, _) | CBVKCString | CBVKImport(_) | CBVKAggregate(_) | CBVKEnum(_) | CBVKClass(_, _) |
						CBVKArray(_) | CBVKIntMap(_) | CBVKStringMap(_) | CBVKBytes(_) | CBVKOptional(_) | CBVKFunction(_, _):
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
			case TNew(classReference, _, arguments) if (CBodyIntMapRecognition.isIntMap(classReference)):
				lowerIntMapConstruction(expression, arguments, expectedMapping);
			case TNew(classReference, _, arguments) if (CBodyStringMapRecognition.isStringMap(classReference)):
				lowerStringMapConstruction(expression, arguments, expectedMapping);
			case TNew(_, _, _):
				final construction = newExpression(expression);
				if (construction == null)
					unsupported(expression, "TNew(managed-construction-shape-lost)");
				lowerManagedConstructedValue(expression, construction, expectedMapping);
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
			final ownedPayload = captureManagedValue(lowered, payload.valueType, argument.pos, 'enum-payload-$index');
			final payloadSource = HaxeSourceSpan.fromPosition(argument.pos, input.sourcePath);
			if (payload.indirect) {
				final pointer:HxcIRResult = {id: nextValueId(), type: payload.storageType()};
				appendInstruction(pointer, IRIOAllocate(payload.valueType.irType, IRAOwned, IRIRuntime("alloc"), {
					kind: IRFAllocationFailure,
					target: IRFTAbort,
					arguments: [],
					cleanup: []
				}), payloadSource, "enum-recursive-payload-allocate");
				registerValueTemporary(pointer.id, "enum-recursive-payload-owner");
				appendInstruction(null, IRIOStore(IRPDereference(pointer.id), ownedPayload.id), payloadSource, "enum-recursive-payload-initialize");
				runtimeRequirements.push(new CBodyRuntimeRequirement("alloc", "allocation", "recursive Haxe enum payload", payloadSource, argument.pos));
				payloadIds.push(pointer.id);
			} else {
				payloadIds.push(ownedPayload.id);
			}
		}
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOConstructTag(value.instanceId, tagCase.name, payloadIds), source, "construct-enum");
		registerValueTemporary(result.id, "enum-result");
		if (value.managedLifetime)
			freshManagedEnumValueIds.set(result.id, true);
		final lowered:LoweredValue = {id: result.id, type: result.type, mapping: mapping};
		return expectedMapping == null ? lowered : coerce(lowered, expectedMapping, expression.pos, "enum-constructor:contextual-type");
	}

	function lowerEnumParameter(expression:TypedExpr, receiver:TypedExpr, enumField:EnumField, payloadIndex:Int):LoweredValue {
		final receiverValue = lowerRequiredEnumValue(receiver, 'TEnumParameter(${enumField.name}:receiver-type)',
			'TEnumParameter(${enumField.name}:receiver-not-enum)', 'enum-parameter-${enumField.name}');
		final receiverMapping = receiverValue.mapping;
		final value = receiverMapping.enumValue();
		if (value == null)
			return unsupported(expression, 'TEnumParameter(${enumField.name}:receiver-enum-lost)');
		final tagCase = value.tagCase(enumField.name);
		if (tagCase == null || payloadIndex < 0 || payloadIndex >= tagCase.payload.length) {
			return unsupported(expression, 'TEnumParameter(${enumField.name}:payload-index=$payloadIndex)');
		}
		final payload = tagCase.payload[payloadIndex];
		final expressionMapping = bodyValueType(expression.t, expression.pos, 'TEnumParameter(${enumField.name}:result-type)');
		if (typeKey(expressionMapping.irType) != typeKey(payload.valueType.irType)) {
			return unsupported(expression, 'TEnumParameter(${enumField.name}:typed-result-mismatch)');
		}
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
		final valuesByName:Map<String, StagedFlowValue> = [];
		for (index => field in fields) {
			if (valuesByName.exists(field.name)) {
				return unsupported(field.expr, 'TObjectDecl(duplicate-field:${field.name})');
			}
			final expectedField = preparedAggregateField(aggregate, field.name);
			if (expectedField == null) {
				return unsupported(field.expr, 'TObjectDecl(unknown-field:${field.name})');
			}
			final value = coerce(lowerValue(field.expr, expectedField.type), expectedField.type, field.expr.pos, 'TObjectDecl(field:${field.name})');
			final ownedValue = captureManagedValue(value, expectedField.type, field.expr.pos, 'record-field-${field.name}');
			valuesByName.set(field.name, stageFlowValue(ownedValue, field.expr, laterAggregateFieldCreatesFlow(fields, index), 'record-field-${field.name}'));
		}
		final namedValues:Array<HxcIRNamedValue> = [];
		for (field in aggregate.fields) {
			final value = valuesByName.get(field.name);
			if (value == null) {
				return unsupported(expression, 'TObjectDecl(missing-field:${field.name})');
			}
			namedValues.push({name: field.name, valueId: restoreStagedValue(value, 'record-field-${field.name}-load')});
		}
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOConstructAggregate(aggregate.instanceId, namedValues), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
			"construct-record");
		registerValueTemporary(result.id, "record-result");
		if (aggregate.managedLifetime)
			freshManagedAggregateValueIds.set(result.id, true);
		final lowered:LoweredValue = {id: result.id, type: result.type, mapping: mapping};
		return expectedMapping == null ? lowered : coerce(lowered, expectedMapping, expression.pos, "TObjectDecl(contextual-type)");
	}

	/**
		Give one managed field its own owner before a record copies its C value.

		The temporary local is a transfer slot, not a second owner: after retain it
		is copied into the record, and the record-level destroy helper becomes the
		only cleanup owner. Fresh constructor/call results already carry one owner,
		so those move directly into the record without an extra retain.
	**/
	function captureManagedValue(value:LoweredValue, mapping:CBodyValueType, position:Position, role:String):LoweredValue {
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		if (mapping.arrayValue() != null) {
			final array = mapping.arrayValue();
			if (array != null && array.managedByCollector)
				return value;
			if (freshManagedArrayValueIds.remove(value.id))
				return value;
			final ownerLocalId = createFlowLocal(mapping, value.id, source, role + "-owner");
			appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIRuntime("array")), source, "retain-record-array-field");
			runtimeRequirements.push(new CBodyRuntimeRequirement("array", "retain", "managed Array captured by a closed record", source, position));
			return loadPlace({place: IRPLocal(ownerLocalId), mapping: mapping, mutable: false}, position, role + "-owned-load");
		}
		if (mapping.bytesValue() != null) {
			if (freshManagedBytesValueIds.remove(value.id))
				return value;
			final ownerLocalId = createFlowLocal(mapping, value.id, source, role + "-owner");
			appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIRuntime("bytes")), source, "retain-record-bytes-field");
			runtimeRequirements.push(new CBodyRuntimeRequirement("bytes", "retain", "managed Bytes captured by a closed record", source, position));
			return loadPlace({place: IRPLocal(ownerLocalId), mapping: mapping, mutable: false}, position, role + "-owned-load");
		}
		final managedEnum = mapping.enumValue();
		if (managedEnum != null && managedEnum.managedLifetime) {
			if (freshManagedEnumValueIds.remove(value.id))
				return value;
			final retainId = managedEnum.retainImplementationId();
			if (retainId == null)
				throw new CBodyEmissionError('managed enum `${managedEnum.instanceId}` lost its retain plan');
			final ownerLocalId = createFlowLocal(mapping, value.id, source, role + "-owner");
			appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIProgramLocal(retainId)), source, "retain-record-enum-field");
			return loadPlace({place: IRPLocal(ownerLocalId), mapping: mapping, mutable: false}, position, role + "-owned-load");
		}
		final managedAggregate = mapping.aggregateValue();
		if (managedAggregate != null && managedAggregate.managedLifetime) {
			if (freshManagedAggregateValueIds.remove(value.id))
				return value;
			final retainId = managedAggregate.retainImplementationId();
			if (retainId == null)
				throw new CBodyEmissionError('managed aggregate `${managedAggregate.instanceId}` lost its retain plan');
			final ownerLocalId = createFlowLocal(mapping, value.id, source, role + "-owner");
			appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIProgramLocal(retainId)), source, "retain-record-field");
			return loadPlace({place: IRPLocal(ownerLocalId), mapping: mapping, mutable: false}, position, role + "-owned-load");
		}
		final managedOptional = mapping.optionalValue();
		if (managedOptional != null && managedOptional.managedLifetime) {
			if (freshManagedOptionalValueIds.remove(value.id))
				return value;
			final retainId = managedOptional.retainImplementationId();
			if (retainId == null)
				throw new CBodyEmissionError('managed optional `${managedOptional.planId}` lost its retain plan');
			final ownerLocalId = createFlowLocal(mapping, value.id, source, role + "-owner");
			appendInstruction(null, IRIORetain(IRPLocal(ownerLocalId), IRIProgramLocal(retainId)), source, "retain-record-optional-field");
			return loadPlace({place: IRPLocal(ownerLocalId), mapping: mapping, mutable: false}, position, role + "-owned-load");
		}
		return value;
	}

	function lowerAggregateField(expression:TypedExpr, receiver:TypedExpr, fieldName:String):LoweredValue {
		final receiverType = bodyValueType(receiver.t, receiver.pos, 'TField($fieldName:receiver-type)');
		final optional = receiverType.optionalValue();
		final payloadType = optional == null ? receiverType : optional.payload;
		final aggregate = payloadType.aggregateValue();
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
		if (optional != null) {
			// Haxe permits a field read through `Null<Record>` and raises at runtime
			// when the value is absent. Keep that observable check in HxcIR before
			// projecting the payload; the C emitter may not silently read `.value`.
			final nullable = coerce(lowerValue(receiver, receiverType), receiverType, receiver.pos, 'TField($fieldName:optional-receiver)');
			appendInstruction(null, IRIONullCheck(nullable.id, IRNCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))),
				HaxeSourceSpan.fromPosition(receiver.pos, input.sourcePath), "optional-record-field-null-check");
			final unwrappedResult:HxcIRResult = {id: nextValueId(), type: payloadType.irType};
			appendInstruction(unwrappedResult, IRIOConvert(nullable.id, IRCNullableUnwrap, payloadType.irType, IRIStatic, null), source,
				"optional-record-field-unwrap");
			registerValueTemporary(unwrappedResult.id, "optional-record-payload");
			final result:HxcIRResult = {id: nextValueId(), type: field.type.irType};
			appendInstruction(result, IRIOProject(unwrappedResult.id, fieldName), source, "optional-record-field-project");
			registerValueTemporary(result.id, "optional-record-field-project");
			return {id: result.id, type: result.type, mapping: field.type};
		}
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
		final ownedChild = field.type.ownedClassValue();
		if (ownedChild == null && typeKey(expressionType.irType) != typeKey(field.type.irType))
			return unsupported(expression, 'TField($fieldName:typed-result-mismatch)');
		if (ownedChild != null && (expressionType.classValue() == null || expressionType.classValue().haxePath != ownedChild.haxePath))
			return unsupported(expression, 'TField($fieldName:owned-class-result-mismatch)');
		final receiverValue = lowerValue(receiver);
		if (receiverValue.mapping.classValue() == null)
			return unsupported(expression, 'TField($fieldName:receiver-value-not-class-reference)');
		if (isNullableClassReference(receiverValue.type)) {
			appendInstruction(null, IRIONullCheck(receiverValue.id, IRNCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))),
				HaxeSourceSpan.fromPosition(receiver.pos, input.sourcePath), "class-field-null-check");
		}
		final place = IRPField(IRPDereference(receiverValue.id), fieldName);
		if (ownedChild != null) {
			final result:HxcIRResult = {id: nextValueId(), type: IRTPointer(IRTInstance(ownedChild.instanceId), false)};
			appendInstruction(result, IRIOBorrowClassField(place), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "owned-class-field-borrow");
			registerValueTemporary(result.id, "owned-class-field-address");
			borrowedClassValueIds.set(result.id, true);
			return {id: result.id, type: result.type, mapping: CBodyValueType.classReference(ownedChild, false)};
		}
		return loadPlace({place: place, mapping: field.type, mutable: field.mutable}, expression.pos, "class-field-load");
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
		final conditionCleanupDepth = normalCleanupActionIds.length;
		final conditionValue = lowerBooleanCondition(condition, "TWhile");
		// A call in the condition can return a fresh managed value. Release that
		// iteration-local owner before either edge leaves the condition block; a
		// function-exit cleanup would name C storage outside its lexical scope.
		appendScopedCleanupInstructions(conditionCleanupDepth, source);
		restoreCleanupDepth(conditionCleanupDepth);
		currentBlock.terminator = {kind: IRTBranch(conditionValue.id, edge(bodyBlock.id), edge(exitBlock.id)), source: source};

		final bodyCleanupDepth = normalCleanupActionIds.length;
		final control = loopControl(exitBlock.id, conditionBlock.id, bodyCleanupDepth);
		loopControlStack.push(control);
		currentBlock = bodyBlock;
		lowerStatement(body);
		loopControlStack.pop();
		if (currentBlock.terminator == null) {
			appendScopedCleanupInstructions(bodyCleanupDepth, source);
			currentBlock.terminator = {kind: IRTJump(edge(conditionBlock.id)), source: source};
		}
		restoreCleanupDepth(bodyCleanupDepth);
		currentBlock = exitBlock;
	}

	function lowerPostTestLoop(expression:TypedExpr, condition:TypedExpr, body:TypedExpr):Void {
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final bodyBlock = createGeneratedBlock("do-body", source);
		final conditionBlock = reserveGeneratedBlock("do-condition", source);
		final exitBlock = reserveGeneratedBlock("do-exit", source);
		currentBlock.terminator = {kind: IRTJump(edge(bodyBlock.id)), source: source};

		final bodyCleanupDepth = normalCleanupActionIds.length;
		final control = loopControl(exitBlock.id, conditionBlock.id, bodyCleanupDepth);
		loopControlStack.push(control);
		currentBlock = bodyBlock;
		lowerStatement(body);
		loopControlStack.pop();
		final bodyEnd = currentBlock;
		final reachesCondition = bodyEnd.terminator == null || control.usedContinue;
		if (bodyEnd.terminator == null) {
			appendScopedCleanupInstructions(bodyCleanupDepth, source);
			bodyEnd.terminator = {kind: IRTJump(edge(conditionBlock.id)), source: source};
		}
		restoreCleanupDepth(bodyCleanupDepth);

		if (reachesCondition) {
			activateGeneratedBlock(conditionBlock);
			activateGeneratedBlock(exitBlock);
			currentBlock = conditionBlock;
			final conditionCleanupDepth = normalCleanupActionIds.length;
			final conditionValue = lowerBooleanCondition(condition, "TWhile");
			appendScopedCleanupInstructions(conditionCleanupDepth, source);
			restoreCleanupDepth(conditionCleanupDepth);
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
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		appendScopedCleanupInstructions(control.cleanupDepth, source);
		currentBlock.terminator = {
			kind: IRTJump(edge(target)),
			source: source
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
		final exhaustiveEnumAbstract = defaultExpression == null && isExhaustiveEnumAbstractSwitch(subject, cases);
		final defaultBlock = defaultExpression == null && !exhaustiveEnumAbstract ? null : createGeneratedBlock("switch-default", source);
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
		} else if (exhaustiveEnumAbstract && defaultBlock != null) {
			// The type checker proved every declared abstract value covered. Keep an
			// explicit fail-stop edge for a forged underlying value instead of
			// inventing a Haxe result or falling out of a non-Void C function.
			defaultBlock.terminator = {kind: IRTUnreachable, source: source};
		}

		final needsExit = defaultExpression == null && !exhaustiveEnumAbstract || openEnds.length > 0;
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
			final cleanupDepth = normalCleanupActionIds.length;
			lowerStatement(cases[index].expr);
			if (currentBlock.terminator == null) {
				openEnds.push(currentBlock);
				appendScopedCleanupInstructions(cleanupDepth, source);
			}
			restoreCleanupDepth(cleanupDepth);
		}
		if (defaultExpression != null && defaultBlock != null) {
			currentBlock = defaultBlock;
			final cleanupDepth = normalCleanupActionIds.length;
			lowerStatement(defaultExpression);
			if (currentBlock.terminator == null) {
				openEnds.push(currentBlock);
				appendScopedCleanupInstructions(cleanupDepth, source);
			}
			restoreCleanupDepth(cleanupDepth);
		}
		final needsExit = openEnds.length > 0;
		final exitBlock = needsExit ? createGeneratedBlock("enum-switch-exit", source) : null;
		if (exitBlock != null)
			for (end in openEnds)
				end.terminator = {kind: IRTJump(edge(exitBlock.id)), source: source};
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
		final exhaustiveEnumAbstract = defaultExpression == null && isExhaustiveEnumAbstractSwitch(subject, cases);
		if (defaultExpression == null && !exhaustiveEnumAbstract) {
			return unsupported(expression, "TSwitch(value-without-default)");
		}
		final subjectValue = lowerSwitchSubject(subject);
		final resultMapping = expectedMapping == null ? bodyValueType(expression.t, expression.pos, "TSwitch(result-type)") : expectedMapping;
		switch resultMapping.kind {
			case CBVKPrimitive(_) | CBVKStaticString(_) | CBVKCString | CBVKAggregate(_):
			case _:
				return unsupported(expression, 'TSwitch(result-type:${resultMapping.cSpelling})');
		}
		if (resultMapping.irType == IRTVoid) {
			return unsupported(expression, "TSwitch(Void-as-value)");
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final initialResultId:Null<String> = switch resultMapping.kind {
			case CBVKAggregate(_): null;
			case _:
				final initialResult:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
				appendInstruction(initialResult, IRIOConstant(defaultConstant(resultMapping.irType, expression, "TSwitch")), source, "switch-default-result");
				initialResult.id;
		}
		final resultLocalId = createFlowLocal(resultMapping, initialResultId, source, "switch-result");
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

		currentBlock = defaultBlock;
		if (defaultExpression == null) {
			currentBlock.terminator = {kind: IRTUnreachable, source: source};
		} else {
			final defaultValue = coerce(lowerValue(defaultExpression, resultMapping), resultMapping, defaultExpression.pos, "TSwitch(default-value)");
			appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), defaultValue.id), source, "switch-default-store");
			currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
		}

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
			final cleanupDepth = normalCleanupActionIds.length;
			final value = coerce(lowerValue(cases[index].expr, resultMapping), resultMapping, cases[index].expr.pos, "TSwitch(enum-case-value)");
			appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), value.id), source, "enum-switch-case-store");
			appendScopedCleanupInstructions(cleanupDepth, source);
			currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
			restoreCleanupDepth(cleanupDepth);
		}
		if (defaultExpression != null && defaultBlock != null) {
			currentBlock = defaultBlock;
			final cleanupDepth = normalCleanupActionIds.length;
			final defaultValue = coerce(lowerValue(defaultExpression, resultMapping), resultMapping, defaultExpression.pos, "TSwitch(enum-default-value)");
			appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), defaultValue.id), source, "enum-switch-default-store");
			appendScopedCleanupInstructions(cleanupDepth, source);
			currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
			restoreCleanupDepth(cleanupDepth);
		}
		dispatchBlock.terminator = {
			kind: IRTTagSwitch(subjectValue.id, enumSwitchCases(cases, caseBlocks, enumValue), defaultBlock == null ? null : edge(defaultBlock.id)),
			source: source
		};
		currentBlock = joinBlock;
		return loadPlace({place: IRPLocal(resultLocalId), mapping: resultMapping, mutable: true}, expression.pos, "enum-switch-result-load");
	}

	function lowerEnumSwitchSubject(expression:TypedExpr):LoweredValue {
		return lowerRequiredEnumValue(expression, "TSwitch(enum-subject-type)", "TSwitch(enum-subject-not-enum)", "optional-enum-switch");
	}

	/**
		Lower an enum use that requires a present value, including `Null<Enum>`.

		Haxe's typed tree keeps the nullable wrapper on pattern-match subjects and
		payload receivers. This helper gives both operations the same checked unwrap
		instead of letting either operation read a missing C payload.
	**/
	function lowerRequiredEnumValue(expression:TypedExpr, typeNode:String, notEnumDiagnostic:String, role:String):LoweredValue {
		final mapping = bodyValueType(expression.t, expression.pos, typeNode);
		final optional = mapping.optionalValue();
		if (optional != null && optional.payload.enumValue() != null) {
			// Haxe keeps the declared `Null<Enum>` type on the switch subject even
			// when source control flow has already excluded `null`, for example in
			// `value == null ? fallback : switch value { ... }`. Preserve safety in
			// HxcIR instead of assuming that source-level narrowing survived: check
			// the presence flag, then expose the enum payload to tag dispatch. The
			// null-check coalescer can remove a repeated check only when dominance
			// proves an earlier check covers this exact value.
			final nullable = coerce(lowerValue(expression, mapping), mapping, expression.pos, '$role:nullable-value');
			final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
			appendInstruction(null, IRIONullCheck(nullable.id, IRNCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))), source,
				'$role-null-check');
			final unwrapped:HxcIRResult = {id: nextValueId(), type: optional.payload.irType};
			appendInstruction(unwrapped, IRIOConvert(nullable.id, IRCNullableUnwrap, optional.payload.irType, IRIStatic, null), source, '$role-unwrap');
			registerValueTemporary(unwrapped.id, "optional-enum-payload");
			return {id: unwrapped.id, type: unwrapped.type, mapping: optional.payload};
		}
		if (mapping.enumValue() == null)
			unsupported(expression, notEnumDiagnostic);
		return coerce(lowerValue(expression, mapping), mapping, expression.pos, '$role:value');
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
		// `enum abstract Choice(Int)` is a closed Haxe type whose runtime value is
		// still an ordinary Int. Ask the shared body-type planner for that proven
		// representation instead of requiring the source type itself to be a core
		// primitive. This is the same abstraction-preserving rule used for locals,
		// parameters, record fields, and switch results.
		final bodyType = bodyValueType(expression.t, expression.pos, "TSwitch(subject-type)");
		final mapping = requirePrimitive(bodyType, expression.pos, "TSwitch(subject-type)");
		switch mapping.irType {
			case IRTBool | IRTInt(_, _):
			case _:
				unsupported(expression, 'TSwitch(non-integral-subject:${mapping.cSpelling})');
		}
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
		final sourceMapping = bodyValueType(expression.t, expression.pos, '$owner(condition-type)');
		final sourceOptional = sourceMapping.optionalValue();
		final boolType = if (sourceMapping.irType == IRTBool) {
			sourceMapping;
		} else if (sourceOptional != null && sourceOptional.payload.irType == IRTBool) {
			sourceOptional.payload;
		} else {
			unsupported(expression, '$owner(non-Bool-condition)');
		};
		return coerce(lowerValue(expression, sourceMapping), boolType, expression.pos, '$owner(condition)');
	}

	static function loopControl(breakTargetBlockId:String, continueTargetBlockId:String, cleanupDepth:Int):LoopControlTargets
		return {
			breakTargetBlockId: breakTargetBlockId,
			continueTargetBlockId: continueTargetBlockId,
			cleanupDepth: cleanupDepth,
			usedBreak: false,
			usedContinue: false
		};

	function lowerValueBlock(expression:TypedExpr, expressions:Array<TypedExpr>, expectedMapping:Null<CBodyValueType>):LoweredValue {
		if (expressions.length == 0) {
			return unsupported(expression, "TBlock(empty-as-value)");
		}
		final lastIndex = expressions.length - 1;
		lowerStatementSequence(expressions, lastIndex);
		return lowerValue(expressions[lastIndex], expectedMapping);
	}

	function lowerConstant(expression:TypedExpr, constant:TConstant, expectedMapping:Null<CBodyValueType>):LoweredValue {
		if (expectedMapping != null && expectedMapping.isCString()) {
			return switch constant {
				case TString(value): lowerCStringConstant(expression, value);
				case _: unsupported(expression, '${nodeName(expression)}:requires-static-C-string-literal');
			};
		}
		if (constant == TThis) {
			final self = selfValue;
			if (self == null)
				return unsupported(expression, "TThis(outside-constructor)");
			return expectedMapping == null ? self : coerce(self, expectedMapping, expression.pos, "TThis(contextual-type)");
		}
		if (constant == TNull) {
			final mapping = expectedMapping == null ? bodyValueType(expression.t, expression.pos, "TConst(TNull:type)") : expectedMapping;
			if (!mapping.hasExactNullCarrier() && mapping.optionalValue() == null)
				return unsupported(expression, "TConst(TNull:requires-nullable-reference-or-direct-optional-context)");
			final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
			appendInstruction(result, IRIOConstant(IRCNull), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
				mapping.optionalValue() == null ? "null-reference" : "null-direct-optional");
			final optional = mapping.optionalValue();
			if (optional != null && optional.managedLifetime)
				freshManagedOptionalValueIds.set(result.id, true);
			return {id: result.id, type: result.type, mapping: mapping};
		}
		if (constant.match(TString(_))) {
			final mapping = expectedMapping == null ? bodyValueType(expression.t, expression.pos, "TConst(TString:type)") : expectedMapping;
			if (mapping.staticStringIdentity() == null)
				return unsupported(expression, 'TConst(TString:context-is-not-static-String-view:${mapping.cSpelling})');
			final text = switch constant {
				case TString(value): value;
				case _: throw new CBodyEmissionError("matched String constant changed before lowering");
			};
			final byteLength = HxcUtf8.byteLength(text);
			if (byteLength == null)
				return unsupported(expression, "TConst(TString:malformed-Unicode-literal)");
			final result:HxcIRResult = {id: nextValueId(), type: IRTString};
			final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
			appendInstruction(result, IRIOConstant(IRCString(text, byteLength)), source, "static-string-literal");
			runtimeRequirements.push(new CBodyRuntimeRequirement("string-literal", "static-value", mapping.cSpelling, source, expression.pos,
				"direct-string-value"));
			return {id: result.id, type: result.type, mapping: mapping};
		}
		final inferredMapping = primitiveMapping(expression.t, expression.pos, nodeName(expression));
		final expectedPrimitive = if (expectedMapping == null) {
			null;
		} else {
			final direct = expectedMapping.primitiveMapping();
			final optional = expectedMapping.optionalValue();
			direct != null ? direct : optional == null ? null : optional.payload.primitiveMapping();
		};
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
		if (borrowedClassLocalIds.exists(localId))
			borrowedClassValueIds.set(result.id, true);
		return {id: result.id, type: result.type, mapping: mapping};
	}

	function lowerStaticField(expression:TypedExpr, classReference:Ref<ClassType>, fieldReference:Ref<ClassField>):LoweredValue {
		final global = globalRegistry.require(classReference, fieldReference, expression, rejectGlobal);
		return loadPlace({place: IRPGlobal(global.ir.id), mapping: CBodyValueType.primitive(global.mapping), mutable: global.ir.mutable}, expression.pos,
			"global-load");
	}

	/** Materialize one reachable static method as an exact C function pointer. */
	function lowerStaticFunctionReference(expression:TypedExpr, classReference:Ref<ClassType>, fieldReference:Ref<ClassField>,
			expectedMapping:Null<CBodyValueType>):LoweredValue {
		final field = fieldReference.get();
		if (field.params.length != 0)
			return unsupported(expression, 'TField(function-value:generic:${field.name})');
		final owner = classReference.get();
		final targetId = CBodyLowering.functionId(owner.pack.concat([owner.name]).join("."), field.name);
		final target = functionsById.get(targetId);
		if (target == null)
			return unsupported(expression, 'TField(function-value:unreachable-target:$targetId)');
		final mapping = bodyValueType(expression.t, expression.pos, 'TField(function-value:$targetId)');
		final signature = mapping.functionValue();
		if (signature == null)
			return unsupported(expression, 'TField(function-value:signature-lost:$targetId)');
		if (signature.parameters.length != target.parameters.length)
			return unsupported(expression, 'TField(function-value:parameter-count:$targetId)');
		for (index in 0...signature.parameters.length)
			if (typeKey(signature.parameters[index].irType) != typeKey(target.parameters[index].mapping.irType))
				return unsupported(expression, 'TField(function-value:parameter-$index-type:$targetId)');
		if (typeKey(signature.result.irType) != typeKey(target.returnMapping.irType))
			return unsupported(expression, 'TField(function-value:return-type:$targetId)');
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOFunctionReference(targetId), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "function-reference");
		final lowered:LoweredValue = {id: result.id, type: result.type, mapping: mapping};
		return expectedMapping == null ? lowered : coerce(lowered, expectedMapping, expression.pos, "function-reference:contextual-type");
	}

	/** Point an enum constructor value at its validated generated adapter. */
	function lowerEnumConstructorFunctionReference(expression:TypedExpr, enumReference:Ref<EnumType>, enumField:EnumField,
			expectedMapping:Null<CBodyValueType>):LoweredValue {
		final adapter = enumConstructorAdapters.require(expression, enumReference, enumField, prepared);
		final mapping = bodyValueType(expression.t, expression.pos, 'enum-constructor-function:${enumField.name}:type');
		final signature = mapping.functionValue();
		if (signature == null)
			return unsupported(expression, 'enum-constructor-function:${enumField.name}:signature-lost-after-preparation');
		if (typeKey(signature.result.irType) != typeKey(adapter.returnMapping.irType)
			|| signature.parameters.length != adapter.parameters.length)
			return unsupported(expression, 'enum-constructor-function:${enumField.name}:adapter-signature-drift');
		for (index in 0...signature.parameters.length)
			if (typeKey(signature.parameters[index].irType) != typeKey(adapter.parameters[index].mapping.irType))
				return unsupported(expression, 'enum-constructor-function:${enumField.name}:adapter-parameter-$index-drift');
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOFunctionReference(adapter.irId), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
			"enum-constructor-function-reference");
		final lowered:LoweredValue = {id: result.id, type: result.type, mapping: mapping};
		return expectedMapping == null ? lowered : coerce(lowered, expectedMapping, expression.pos, "enum-constructor-function-reference:contextual-type");
	}

	function lowerAssignment(expression:TypedExpr, left:TypedExpr, right:TypedExpr):LoweredValue {
		final managedArrayAssignment = lowerManagedArrayAssignment(expression, left, right);
		if (managedArrayAssignment != null)
			return managedArrayAssignment;
		if (newExpression(right) != null)
			unsupported(right, "TNew(stack-construction-requires-direct-local)");
		if (referencesStackConstructedValue(right))
			unsupported(right, "TNew(stack-reference-escape:assignment)");
		final target = lowerPlace(left);
		if (!target.mutable) {
			unsupported(left, "TBinop(OpAssign:immutable-place)");
		}
		if (target.mapping.fixedArrayShape() != null)
			unsupported(expression, "TBinop(OpAssign:fixed-array-whole-value-not-admitted)");
		if (target.mapping.ownedClassValue() != null)
			unsupported(expression, "TBinop(OpAssign:owned-class-field-reassignment-not-admitted)");
		if (target.mapping.arrayValue() != null)
			unsupported(expression, "TBinop(OpAssign:managed-Array-reassignment-not-admitted)");
		if (target.mapping.stringMapValue() != null)
			unsupported(expression, "TBinop(OpAssign:managed-StringMap-reassignment-not-admitted)");
		if (target.mapping.bytesValue() != null)
			unsupported(expression, "TBinop(OpAssign:managed-Bytes-reassignment-not-admitted)");
		final stagedTarget = stageFlowPlace(target, left.pos, expressionCreatesFlow(right), "assignment-target");
		final source = lowerValue(right, target.mapping);
		final value = coerce(source, target.mapping, right.pos, "TBinop(OpAssign:right)");
		rejectOwnedClassBorrow(value, right.pos, "TBinop(OpAssign:owned-class-borrow-escape)");
		final stableTarget = restoreStagedPlace(stagedTarget, "assignment-target");
		final optional = target.mapping.optionalValue();
		if (optional != null && optional.managedLifetime) {
			final destroyId = optional.destroyImplementationId();
			if (destroyId == null)
				throw new CBodyEmissionError('managed optional `${optional.planId}` lost its destroy plan');
			final replacement = captureManagedValue(value, target.mapping, right.pos, "optional-assignment-replacement");
			final sourceSpan = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
			// Capture the replacement before destroying the prior owner. Besides
			// preserving failure behavior, this makes `value = value` safe.
			appendInstruction(null, IRIORelease(stableTarget.place, IRIProgramLocal(destroyId)), sourceSpan, "release-optional-assignment-target");
			appendInstruction(null, IRIOStore(stableTarget.place, replacement.id), sourceSpan, "store-optional-assignment-replacement");
			return replacement;
		}
		appendInstruction(null, IRIOStore(stableTarget.place, value.id), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "store");
		return value;
	}

	/** Lower `array[index] = value` without pretending a resizable Array is a C place. */
	function lowerManagedArrayAssignment(expression:TypedExpr, left:TypedExpr, right:TypedExpr):Null<LoweredValue> {
		final indexed = switch unwrapExpression(left).expr {
			case TArray(collection, index): {collection: collection, index: index};
			case _: return null;
		};
		// `TArray` is Haxe's shared typed shape for every indexed access,
		// including fixed `c.CArray` storage and spans. Check the nominal
		// receiver first so this specialized Array path can decline without
		// asking the general value mapper to classify an intentionally
		// non-first-class CArray value.
		if (!CBodyArrayRecognition.isCoreArrayType(indexed.collection.t))
			return null;
		final receiverMapping = bodyValueType(indexed.collection.t, indexed.collection.pos, "TArray(set:receiver-type)");
		final array = receiverMapping.arrayValue();
		if (array == null)
			return null;
		final receiver = coerce(lowerValue(indexed.collection, receiverMapping), receiverMapping, indexed.collection.pos, "TArray(set:receiver)");
		final indexMapping = CBodyValueType.primitive(primitiveMapping(indexed.index.t, indexed.index.pos, "TArray(set:index-type)"));
		if (typeKey(indexMapping.irType) != typeKey(IRTInt(32, true)))
			return unsupported(indexed.index, "TArray(set:index-must-be-Int)");
		final index = coerce(lowerValue(indexed.index, indexMapping), indexMapping, indexed.index.pos, "TArray(set:index)");
		final element = coerce(lowerValue(right, array.element), array.element, right.pos, "TArray(set:value)");
		final resultMapping = bodyValueType(expression.t, expression.pos, "TArray(set:result-type)");
		if (typeKey(resultMapping.irType) != typeKey(array.element.irType))
			return unsupported(expression, "TArray(set:assignment-result-mismatch)");
		final result:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("array", "set"),
			arguments: [receiver.id, index.id, element.id],
			returnType: result.type,
			failure: managedArrayFailure()
		}), source, "array-set");
		registerValueTemporary(result.id, "array-set-result");
		runtimeRequirements.push(new CBodyRuntimeRequirement("array", "set", "ordinary Haxe Array indexed assignment", source, expression.pos));
		return {id: result.id, type: result.type, mapping: resultMapping};
	}

	function lowerCompoundAssignment(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr):LoweredValue {
		final target = lowerPlace(left);
		if (!target.mutable) {
			unsupported(left, "TBinop(OpAssignOp:immutable-place)");
		}
		final oldValue = loadPlace(target, left.pos, "compound-load");
		final rightCreatesFlow = expressionCreatesFlow(right);
		final stagedTarget = stageFlowPlace(target, left.pos, rightCreatesFlow, "compound-target");
		final oldValueLocal = rightCreatesFlow ? createFlowLocal(oldValue.mapping, oldValue.id, HaxeSourceSpan.fromPosition(left.pos, input.sourcePath),
			"compound-left") : null;
		final rightValue = lowerValue(right);
		final stableOldValue = oldValueLocal == null ? oldValue : loadPlace({place: IRPLocal(oldValueLocal), mapping: oldValue.mapping, mutable: true},
			left.pos, "compound-left-load");
		final nextValue = lowerBinaryValues(expression, operation, stableOldValue, rightValue, "compound", target.mapping);
		final stored = coerce(nextValue, target.mapping, expression.pos, "TBinop(OpAssignOp:result)");
		final stableTarget = restoreStagedPlace(stagedTarget, "compound-target");
		appendInstruction(null, IRIOStore(stableTarget.place, stored.id), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "compound-store");
		return stored;
	}

	function lowerBinary(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr):LoweredValue {
		if (operation == OpEq || operation == OpNotEq) {
			final leftMapping = isNullConstantExpression(left) ? null : nullEqualityValueMapping(left, "TBinop(class-equality:left-type)");
			final rightMapping = isNullConstantExpression(right) ? null : nullEqualityValueMapping(right, "TBinop(class-equality:right-type)");
			if (leftMapping != null && leftMapping.optionalValue() != null || rightMapping != null && rightMapping.optionalValue() != null) {
				return lowerOptionalNullEquality(expression, operation, left, right, leftMapping, rightMapping);
			}
			if (leftMapping != null && leftMapping.classValue() != null || rightMapping != null && rightMapping.classValue() != null) {
				return lowerClassEquality(expression, operation, left, right, leftMapping, rightMapping);
			}
			if (leftMapping != null && leftMapping.arrayValue() != null || rightMapping != null && rightMapping.arrayValue() != null) {
				return lowerArrayReferenceEquality(expression, operation, left, right, leftMapping, rightMapping);
			}
			if (leftMapping != null
				&& leftMapping.stringMapValue() != null
				|| rightMapping != null
				&& rightMapping.stringMapValue() != null) {
				return lowerStringMapReferenceEquality(expression, operation, left, right, leftMapping, rightMapping);
			}
			if (leftMapping != null && leftMapping.enumValue() != null || rightMapping != null && rightMapping.enumValue() != null) {
				return lowerFieldlessEnumEquality(expression, operation, left, right, leftMapping, rightMapping);
			}
			if (leftMapping != null
				&& leftMapping.staticStringIdentity() != null
				|| rightMapping != null
				&& rightMapping.staticStringIdentity() != null) {
				return lowerStaticStringEquality(expression, operation, left, right, leftMapping, rightMapping);
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

	/**
		Preserve a nullable carrier hidden by Haxe's null-comparison casts.

		The Haxe typer wraps `Null<UInt>` in a cast when it builds `value == null`.
		That cast changes the expression's apparent type to UInt even though the
		comparison still asks whether the original optional value is absent. Prefer
		an inner tagged optional only at this equality-classification boundary; all
		ordinary casts continue through the normal checked conversion path.
	**/
	function nullEqualityValueMapping(expression:TypedExpr, node:String):CBodyValueType {
		final direct = bodyValueType(expression.t, expression.pos, node);
		if (direct.optionalValue() != null)
			return direct;
		return switch expression.expr {
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _):
				final innerMapping = nullEqualityValueMapping(inner, '$node.inner');
				innerMapping.optionalValue() == null ? direct : innerMapping;
			case _:
				direct;
		};
	}

	/**
		Find the tagged value beneath a wrapper inserted for null comparison.

		Type classification alone is not enough: lowering the outer UInt cast
		would unwrap the optional before asking whether it is null. Return the
		inner expression whose own mapping still carries the presence bit.
	**/
	function nullEqualityCarrierExpression(expression:TypedExpr, node:String):TypedExpr {
		final direct = bodyValueType(expression.t, expression.pos, node);
		if (direct.optionalValue() != null)
			return expression;
		return switch expression.expr {
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _):
				nullEqualityCarrierExpression(inner, '$node.inner');
			case _:
				expression;
		};
	}

	/** Compare immutable UTF-8 String views by contents, never by data pointer. */
	function lowerStaticStringEquality(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr, leftMapping:Null<CBodyValueType>,
			rightMapping:Null<CBodyValueType>):LoweredValue {
		if (leftMapping == null
			|| rightMapping == null
			|| leftMapping.staticStringIdentity() == null
			|| rightMapping.staticStringIdentity() == null)
			return unsupported(expression, "TBinop(String-equality:both-operands-must-be-non-null-admitted-String-values)");
		final leftValue = coerce(lowerValue(left, leftMapping), leftMapping, left.pos, "TBinop(String-equality:left)");
		final stagedLeft = stageFlowValue(leftValue, left, expressionCreatesFlow(right), "string-equality-left");
		final rightValue = coerce(lowerValue(right, rightMapping), rightMapping, right.pos, "TBinop(String-equality:right)");
		final stableLeftId = restoreStagedValue(stagedLeft, "string-equality-left");
		final result:HxcIRResult = {id: nextValueId(), type: IRTBool};
		appendInstruction(result, IRIOBinary(operation == OpEq ? "haxe.string.equal" : "haxe.string.not-equal", stableLeftId, rightValue.id, IRIStatic),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "string-equality");
		registerValueTemporary(result.id, "string-equality-result");
		final boolMapping = bodyValueType(expression.t, expression.pos, "TBinop(String-equality:result-type)");
		if (boolMapping.irType != IRTBool)
			return unsupported(expression, "TBinop(String-equality:result-not-Bool)");
		return {id: result.id, type: result.type, mapping: boolMapping};
	}

	/**
	 * Compare a direct optional record with `null` through its explicit presence bit.
	 *
	 * C structs have no built-in null value, so comparing the whole generated struct
	 * would be both invalid C and the wrong semantic question. This operation names
	 * the real Haxe intent and lets the validated C layer emit `has_value` directly.
	 */
	function lowerOptionalNullEquality(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr, leftMapping:Null<CBodyValueType>,
			rightMapping:Null<CBodyValueType>):LoweredValue {
		final leftIsNull = isNullConstantExpression(left);
		final rightIsNull = isNullConstantExpression(right);
		if (leftIsNull == rightIsNull)
			return unsupported(expression, "TBinop(direct-optional-equality-requires-exactly-one-null-operand)");
		final optionalMapping = leftIsNull ? rightMapping : leftMapping;
		if (optionalMapping == null || optionalMapping.optionalValue() == null)
			return unsupported(expression, "TBinop(direct-optional-equality-mixed-value-category)");
		final valueExpression = nullEqualityCarrierExpression(leftIsNull ? right : left, "TBinop(direct-optional-null-equality:carrier)");
		var value = coerce(lowerValue(valueExpression, optionalMapping), optionalMapping, valueExpression.pos, "TBinop(direct-optional-null-equality:value)");
		value = stabilizeFreshManagedOptional(value, valueExpression.pos, "optional-null-equality");
		final boolMapping = bodyValueType(expression.t, expression.pos, "TBinop(direct-optional-null-equality:result-type)");
		if (boolMapping.irType != IRTBool)
			return unsupported(expression, "TBinop(direct-optional-null-equality:result-not-Bool)");
		final result:HxcIRResult = {id: nextValueId(), type: IRTBool};
		appendInstruction(result, IRIOUnary(operation == OpEq ? "haxe.direct-optional.is-null" : "haxe.direct-optional.is-not-null", value.id, IRIStatic),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "direct-optional-null-equality");
		return {id: result.id, type: result.type, mapping: boolMapping};
	}

	/**
	 * Compare two values of the same fieldless enum through their native C tags.
	 *
	 * Payload enums need a separate, explicit Haxe equality contract: comparing
	 * their outer C structs would be invalid C and would not define how nested
	 * values participate. The fieldless representation contains only the tag, so
	 * direct comparison is complete and preserves ordinary Haxe `==`/`!=`.
	 */
	function lowerFieldlessEnumEquality(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr, leftMapping:Null<CBodyValueType>,
			rightMapping:Null<CBodyValueType>):LoweredValue {
		if (leftMapping == null || rightMapping == null)
			return unsupported(expression, "TBinop(fieldless-enum-equality-does-not-admit-null)");
		final leftEnum = leftMapping.enumValue();
		final rightEnum = rightMapping.enumValue();
		if (leftEnum == null || rightEnum == null)
			return unsupported(expression, "TBinop(fieldless-enum-equality-mixed-value-category)");
		if (leftEnum.instanceId != rightEnum.instanceId)
			return unsupported(expression, 'TBinop(unrelated-enum-equality:${leftEnum.haxePath}->${rightEnum.haxePath})');
		if (leftEnum.representation != CBERNativeEnum || rightEnum.representation != CBERNativeEnum)
			return unsupported(expression, 'TBinop(payload-enum-equality-requires-structural-semantics:${leftEnum.haxePath})');

		final leftValue = coerce(lowerValue(left, leftMapping), leftMapping, left.pos, "TBinop(fieldless-enum-equality:left)");
		final leftValueLocal = expressionCreatesFlow(right) ? createFlowLocal(leftMapping, leftValue.id,
			HaxeSourceSpan.fromPosition(left.pos, input.sourcePath), "fieldless-enum-equality-left") : null;
		final rightValue = coerce(lowerValue(right, rightMapping), rightMapping, right.pos, "TBinop(fieldless-enum-equality:right)");
		final stableLeft = leftValueLocal == null ? leftValue : loadPlace({place: IRPLocal(leftValueLocal), mapping: leftMapping, mutable: true}, left.pos,
			"fieldless-enum-equality-left-load");
		final boolMapping = bodyValueType(expression.t, expression.pos, "TBinop(fieldless-enum-equality:result-type)");
		if (boolMapping.irType != IRTBool)
			return unsupported(expression, "TBinop(fieldless-enum-equality:result-not-Bool)");
		final result:HxcIRResult = {id: nextValueId(), type: IRTBool};
		appendInstruction(result, IRIOBinary(operation == OpEq ? "haxe.enum-tag.equal" : "haxe.enum-tag.not-equal", stableLeft.id, rightValue.id, IRIStatic),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "fieldless-enum-equality");
		return {id: result.id, type: result.type, mapping: boolMapping};
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

	/**
		Compare Haxe Array references by identity, including explicit `null`.

		An Array is already one nullable C pointer carrier. This operation compares
		those pointers directly; it does not inspect elements or add a tagged
		optional wrapper around the reference.
	**/
	function lowerArrayReferenceEquality(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr, leftMapping:Null<CBodyValueType>,
			rightMapping:Null<CBodyValueType>):LoweredValue {
		if (leftMapping == null && rightMapping == null)
			return unsupported(expression, "TBinop(array-reference-equality-without-Array-type)");
		final target = leftMapping == null ? rightMapping : leftMapping;
		if (target == null || target.arrayValue() == null)
			return unsupported(expression, "TBinop(array-reference-equality-target-not-Array)");
		if (leftMapping != null
			&& rightMapping != null
			&& (leftMapping.arrayValue() == null
				|| rightMapping.arrayValue() == null
				|| typeKey(leftMapping.irType) != typeKey(rightMapping.irType)))
			return unsupported(expression, "TBinop(array-reference-equality-requires-matching-specializations)");
		final leftValue = coerce(lowerValue(left, target), target, left.pos, "TBinop(array-reference-equality:left)");
		final stagedLeft = stageFlowValue(leftValue, left, expressionCreatesFlow(right), "array-reference-equality-left");
		final rightValue = coerce(lowerValue(right, target), target, right.pos, "TBinop(array-reference-equality:right)");
		final stableLeftId = restoreStagedValue(stagedLeft, "array-reference-equality-left");
		final boolMapping = bodyValueType(expression.t, expression.pos, "TBinop(array-reference-equality:result-type)");
		if (boolMapping.irType != IRTBool)
			return unsupported(expression, "TBinop(array-reference-equality:result-not-Bool)");
		final result:HxcIRResult = {id: nextValueId(), type: IRTBool};
		appendInstruction(result,
			IRIOBinary(operation == OpEq ? "haxe.array-reference.equal" : "haxe.array-reference.not-equal", stableLeftId, rightValue.id, IRIStatic),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "array-reference-equality");
		return {id: result.id, type: result.type, mapping: boolMapping};
	}

	/**
		Compare Haxe StringMap references by identity, including explicit `null`.

		A StringMap is one shared mutable object represented by a nullable C
		pointer. Equality therefore compares the pointer carriers; it does not walk
		keys or compare entries.
	**/
	function lowerStringMapReferenceEquality(expression:TypedExpr, operation:Binop, left:TypedExpr, right:TypedExpr, leftMapping:Null<CBodyValueType>,
			rightMapping:Null<CBodyValueType>):LoweredValue {
		if (leftMapping == null && rightMapping == null)
			return unsupported(expression, "TBinop(string-map-reference-equality-without-StringMap-type)");
		final target = leftMapping == null ? rightMapping : leftMapping;
		if (target == null || target.stringMapValue() == null)
			return unsupported(expression, "TBinop(string-map-reference-equality-target-not-StringMap)");
		if (leftMapping != null
			&& rightMapping != null
			&& (leftMapping.stringMapValue() == null
				|| rightMapping.stringMapValue() == null
				|| typeKey(leftMapping.irType) != typeKey(rightMapping.irType)))
			return unsupported(expression, "TBinop(string-map-reference-equality-requires-matching-specializations)");
		final leftValue = coerce(lowerValue(left, target), target, left.pos, "TBinop(string-map-reference-equality:left)");
		final stagedLeft = stageFlowValue(leftValue, left, expressionCreatesFlow(right), "string-map-reference-equality-left");
		final rightValue = coerce(lowerValue(right, target), target, right.pos, "TBinop(string-map-reference-equality:right)");
		final stableLeftId = restoreStagedValue(stagedLeft, "string-map-reference-equality-left");
		final boolMapping = bodyValueType(expression.t, expression.pos, "TBinop(string-map-reference-equality:result-type)");
		if (boolMapping.irType != IRTBool)
			return unsupported(expression, "TBinop(string-map-reference-equality:result-not-Bool)");
		final result:HxcIRResult = {id: nextValueId(), type: IRTBool};
		appendInstruction(result,
			IRIOBinary(operation == OpEq ? "haxe.string-map-reference.equal" : "haxe.string-map-reference.not-equal", stableLeftId, rightValue.id, IRIStatic),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "string-map-reference-equality");
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
		final leftPrimitive = requirePrimitiveOperand(leftValue.mapping, expression.pos, 'TBinop($operation:left-type)');
		final rightPrimitive = requirePrimitiveOperand(rightValue.mapping, expression.pos, 'TBinop($operation:right-type)');
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
		final rhsCleanupDepth = normalCleanupActionIds.length;
		final rightValue = coerce(lowerValue(right), boolType, right.pos, "TBinop(short-circuit:right)");
		appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), rightValue.id), source, "short-circuit-store");
		// The right side runs in its own C block. A call there may return a
		// managed temporary, such as `map.get(key) != null`. Copy the resulting
		// Bool into the outer flow local, then destroy every owner created in this
		// block before jumping to the join. Registering those owners for ordinary
		// function-exit cleanup would emit a C reference outside their lexical
		// scope and would skip cleanup whenever short-circuiting bypasses the block.
		appendScopedCleanupInstructions(rhsCleanupDepth, source);
		currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
		restoreCleanupDepth(rhsCleanupDepth);
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
		final optionalResult = resultMapping.optionalValue();
		final managedEnumResult = switch resultMapping.enumValue() {
			case null: null;
			// Recursive enums deep-copy owned child links during retain. That work
			// can fail and therefore needs an explicit failure edge on carrier
			// acquisition before it can share this infallible join protocol.
			case value if (value.managedLifetime && !value.recursive): value;
			case _: null;
		};
		final branchInitializesResult = conditionalDirectValue(resultMapping);
		if (resultMapping.primitiveMapping() == null
			&& !resultMapping.isCString()
			&& optionalResult == null
			&& managedEnumResult == null
			&& !branchInitializesResult)
			return unsupported(expression, 'TIf(result-type:${resultMapping.cSpelling})');
		if (resultMapping.irType == IRTVoid) {
			return unsupported(expression,
				'TIf(Void-as-value:${expectedMapping == null ? "typed-expression" : "contextual"}:function-return=${prepared.returnMapping.cSpelling})');
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final resultLocalId = if (managedEnumResult != null) {
			final destroyId = managedEnumResult.destroyImplementationId();
			if (destroyId == null)
				throw new CBodyEmissionError('managed enum `${managedEnumResult.instanceId}` lost its destroy plan');
			final localId = declareFlowLocal(resultMapping, source, "conditional-managed-result");
			appendInstruction(null, IRIODeclareManagedCarrier(IRPLocal(localId), IRIProgramLocal(destroyId)), source, "conditional-managed-result-declare");
			localId;
		} else if (branchInitializesResult) {
			final localId = declareFlowLocal(resultMapping, source, "conditional-result");
			appendInstruction(null, IRIODeclareUninitialized(IRPLocal(localId)), source, "conditional-result-declare");
			localId;
		} else {
			final defaultResult:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
			appendInstruction(defaultResult, IRIOConstant(defaultConstant(resultMapping.irType, expression, "TIf")), source, "conditional-default");
			createFlowLocal(resultMapping, defaultResult.id, source, "conditional-result");
		}
		final trueBlock = createGeneratedBlock("conditional-true", source);
		final falseBlock = createGeneratedBlock("conditional-false", source);
		final joinBlock = createGeneratedBlock("conditional-join", source);
		currentBlock.terminator = {kind: IRTBranch(conditionValue.id, edge(trueBlock.id), edge(falseBlock.id)), source: source};

		currentBlock = trueBlock;
		final trueCleanupDepth = normalCleanupActionIds.length;
		var trueValue = coerce(lowerValue(whenTrue, resultMapping), resultMapping, whenTrue.pos, "TIf(true-value)");
		if (managedEnumResult != null) {
			appendManagedConditionalAcquire(resultLocalId, trueValue, managedEnumResult, source, "conditional-true-acquire");
		} else {
			if (optionalResult != null && optionalResult.managedLifetime)
				trueValue = captureManagedValue(trueValue, resultMapping, whenTrue.pos, "conditional-true");
			appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), trueValue.id), source, "conditional-true-store");
		}
		appendScopedCleanupInstructions(trueCleanupDepth, source);
		currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
		restoreCleanupDepth(trueCleanupDepth);

		currentBlock = falseBlock;
		final falseCleanupDepth = normalCleanupActionIds.length;
		var falseValue = coerce(lowerValue(falseExpression, resultMapping), resultMapping, falseExpression.pos, "TIf(false-value)");
		if (managedEnumResult != null) {
			appendManagedConditionalAcquire(resultLocalId, falseValue, managedEnumResult, source, "conditional-false-acquire");
		} else {
			if (optionalResult != null && optionalResult.managedLifetime)
				falseValue = captureManagedValue(falseValue, resultMapping, falseExpression.pos, "conditional-false");
			appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), falseValue.id), source, "conditional-false-store");
		}
		appendScopedCleanupInstructions(falseCleanupDepth, source);
		currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
		restoreCleanupDepth(falseCleanupDepth);

		currentBlock = joinBlock;
		final loaded:LoweredValue = if (managedEnumResult == null) {
			loadPlace({place: IRPLocal(resultLocalId), mapping: resultMapping, mutable: true}, expression.pos, "conditional-load");
		} else {
			final result:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
			appendInstruction(result, IRIOMoveManagedCarrier(IRPLocal(resultLocalId)), source, "conditional-managed-move");
			// The carrier itself already owns the `conditional-managed-result`
			// symbol. The moved value needs a distinct semantic name so C cannot
			// accidentally redeclare the carrier as `T carrier = carrier`.
			registerValueTemporary(result.id, "conditional-managed-move-result");
			({id: result.id, type: result.type, mapping: resultMapping} : LoweredValue);
		}
		if (managedEnumResult != null)
			freshManagedEnumValueIds.set(loaded.id, true);
		if (optionalResult != null && optionalResult.managedLifetime)
			freshManagedOptionalValueIds.set(loaded.id, true);
		return loaded;
	}

	/**
	 * Give one managed conditional arm to its outer join carrier.
	 *
	 * Fresh constructors and owned call results move directly. Parameters,
	 * locals, and other borrowed values are copied and retained through the
	 * enum's active-tag helper before branch-local cleanup runs.
	 */
	function appendManagedConditionalAcquire(localId:String, value:LoweredValue, managed:CPreparedBodyEnumInstance, source:HxcSourceSpan, role:String):Void {
		final acquisition = if (freshManagedEnumValueIds.remove(value.id)) {
			IRMCAMoveFresh;
		} else {
			final retainId = managed.retainImplementationId();
			if (retainId == null)
				throw new CBodyEmissionError('managed enum `${managed.instanceId}` lost its retain plan');
			IRMCARetainBorrowed(IRIProgramLocal(retainId));
		}
		appendInstruction(null, IRIOAcquireManagedCarrier(IRPLocal(localId), value.id, acquisition), source, role);
	}

	/**
	 * Whether both conditional branches can initialize one direct value carrier.
	 *
	 * These families copy their complete representation without ownership work.
	 * Managed records/enums are excluded because selecting one branch would also
	 * need active-branch transfer and join cleanup. A direct imported struct is
	 * admitted only when its authoritative C header proves by-value storage.
	 */
	static function conditionalDirectValue(mapping:CBodyValueType):Bool {
		return switch mapping.kind {
			case CBVKStaticString(_): true;
			case CBVKAggregate(aggregate): !aggregate.managedLifetime;
			case CBVKEnum(value): !value.managedLifetime;
			case CBVKImport(value): value.directStructTarget() != null;
			case _: false;
		};
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
				if (borrowedClassLocalIds.exists(localId)) {
					return unsupported(expression, 'TLocal(${variable.name}:borrowed-class-alias-assignment)');
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
				{
					place: IRPField(IRPDereference(receiverValue.id), fieldName),
					mapping: field.type,
					mutable: field.mutable
				};
			case TField(_, FStatic(classReference, fieldReference)):
				final global = globalRegistry.require(classReference, fieldReference, expression, rejectGlobal);
				{place: IRPGlobal(global.ir.id), mapping: CBodyValueType.primitive(global.mapping), mutable: global.ir.mutable};
			case TParenthesis(inner) | TMeta(_, inner): lowerPlace(inner);
			case _: unsupported(expression, 'place(${nodeName(expression)})');
		};
	}

	function lowerCollectionIndexPlace(expression:TypedExpr, collection:TypedExpr, index:TypedExpr):LoweredPlace {
		final binding = requireIndexedCollection(collection);
		final indexMapping = primitiveMapping(index.t, index.pos, "TArray(index-type)");
		switch indexMapping.irType {
			case IRTInt(32, true):
			case _:
				unsupported(index, "TArray(index-must-be-Int)");
		}
		final indexType = CBodyValueType.primitive(indexMapping);
		final indexValue = coerce(lowerValue(index, indexType), indexType, index.pos, "TArray(index)");
		final policy = boundsPolicy(binding.length, index);
		appendInstruction(null, IRIOBoundsCheck(binding.place, indexValue.id, policy), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
			"collection-bounds");
		final mutable = switch binding.kind {
			case BCKFixedArray(_): true;
			case BCKSpan(value): value;
		};
		return {
			place: IRPIndex(binding.place, indexValue.id),
			mapping: CBodyValueType.primitive(binding.element),
			mutable: mutable
		};
	}

	/** Resolve a local span/fixed array or a fixed array embedded in a class. */
	function requireIndexedCollection(expression:TypedExpr):BodyIndexedCollection {
		return switch unwrapExpression(expression).expr {
			case TLocal(variable):
				final binding = collectionBindingsByCompilerId.get(variable.id);
				if (binding == null)
					unsupported(expression, 'TArray(collection-local-outside-admitted-slice:${variable.name})');
				{
					place: IRPLocal(binding.localId),
					kind: binding.kind,
					element: binding.element,
					length: binding.length
				};
			case TField(_, FInstance(_, _, _)):
				final field = lowerPlace(expression);
				final fixed = field.mapping.fixedArrayShape();
				if (fixed == null)
					unsupported(expression, "TArray(class-field-not-fixed-array)");
				{
					place: field.place,
					kind: BCKFixedArray(fixed.witnessId),
					element: fixed.element,
					length: fixed.length
				};
			case _: unsupported(expression, 'TArray(collection=${nodeName(expression)})');
		};
	}

	function boundsPolicy(knownLength:Null<Int>, index:TypedExpr):HxcIRBoundsPolicy {
		final value = constantInt(index);
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
		final bytesStaticMethod = coreBytesStaticMethod(call.callee);
		if (bytesStaticMethod != null)
			return lowerManagedBytesStaticCall(expression, bytesStaticMethod, call.arguments);
		if (isStdInt(call.callee)) {
			if (call.arguments.length != 1) {
				return unsupported(expression, 'TCall(Std.int:argument-count=${call.arguments.length})');
			}
			final rawSource = lowerValue(call.arguments[0]);
			final sourceOptional = rawSource.mapping.optionalValue();
			final source = sourceOptional == null ? rawSource : coerce(rawSource, sourceOptional.payload, call.arguments[0].pos,
				"TCall(Std.int:checked-nullable-argument)");
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
			return switch CBodyIntrinsicReceiver.classify(instanceAccess) {
				case CBIRArray: lowerManagedArrayCall(expression, instanceAccess, call.arguments, materializeResult);
				case CBIRIntMap: lowerIntMapCall(expression, instanceAccess, call.arguments, materializeResult);
				case CBIRStringMap: lowerStringMapCall(expression, instanceAccess, call.arguments, materializeResult);
				case CBIRBytes: lowerManagedBytesCall(expression, instanceAccess, call.arguments, materializeResult);
				case CBIRString: lowerStringCall(expression, instanceAccess, call.arguments);
				case CBIROrdinaryClass: lowerInstanceCall(expression, instanceAccess, call.arguments, materializeResult);
			};
		if (!isDirectStaticFunctionExpression(call.callee)) {
			final callableMapping = bodyValueType(call.callee.t, call.callee.pos, "TCall(indirect-callee-type)");
			if (callableMapping.functionValue() != null)
				return lowerIndirectFunctionCall(expression, call.callee, call.arguments, callableMapping, materializeResult);
		}
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
		final argumentExpressions = completeDirectCallArguments(expression, call.arguments, target.parameters, 0, targetId, "argument");
		final stagedArguments:Array<StagedFlowValue> = [];
		for (index in 0...argumentExpressions.length) {
			final argumentExpression = argumentExpressions[index];
			final parameter = target.parameters[index];
			if (referencesStackConstructedValue(argumentExpression) && !parameter.borrowedReference) {
				return unsupported(argumentExpression, 'TNew(stack-reference-escape:static-call-argument:$index,target=$targetId)');
			}
			final value = lowerValue(argumentExpression, parameter.mapping);
			var converted = coerce(value, parameter.mapping, argumentExpression.pos, 'TCall(argument:$index,target=$targetId)');
			converted = stabilizeFreshManagedEnum(converted, argumentExpression.pos, 'static-call-argument-$index');
			converted = stabilizeFreshManagedAggregate(converted, argumentExpression.pos, 'static-call-argument-$index');
			converted = stabilizeFreshManagedOptional(converted, argumentExpression.pos, 'static-call-argument-$index');
			if (freshManagedArrayValueIds.exists(converted.id))
				return unsupported(argumentExpression, 'TCall(fresh-managed-Array-argument-needs-owner:$index,target=$targetId)');
			if (freshManagedStringMapValueIds.exists(converted.id))
				return unsupported(argumentExpression, 'TCall(fresh-managed-StringMap-argument-needs-owner:$index,target=$targetId)');
			if (freshManagedBytesValueIds.exists(converted.id))
				return unsupported(argumentExpression, 'TCall(fresh-managed-Bytes-argument-needs-owner:$index,target=$targetId)');
			if (borrowedManagedArrayElementValueIds.exists(converted.id))
				return unsupported(argumentExpression, 'TCall(borrowed-managed-Array-element-argument:$index,target=$targetId)');
			if (!parameter.borrowedReference)
				rejectOwnedClassBorrow(converted, argumentExpression.pos, 'TCall(owned-class-borrow-escape:static-call-argument:$index,target=$targetId)');
			stagedArguments.push(stageFlowValue(converted, argumentExpression, laterExpressionCreatesFlow(argumentExpressions, index),
				'static-call-argument-$index'));
		}
		final arguments = restoreCallArguments(stagedArguments, "static-call-argument");
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
		if (target.returnMapping.bytesValue() != null)
			freshManagedBytesValueIds.set(result.id, true);
		final returnedArray = target.returnMapping.arrayValue();
		if (returnedArray != null && !returnedArray.managedByCollector)
			freshManagedArrayValueIds.set(result.id, true);
		if (target.returnMapping.stringMapValue() != null)
			freshManagedStringMapValueIds.set(result.id, true);
		final returnedEnum = target.returnMapping.enumValue();
		if (returnedEnum != null && returnedEnum.managedLifetime)
			freshManagedEnumValueIds.set(result.id, true);
		final returnedAggregate = target.returnMapping.aggregateValue();
		if (returnedAggregate != null && returnedAggregate.managedLifetime)
			freshManagedAggregateValueIds.set(result.id, true);
		final returnedOptional = target.returnMapping.optionalValue();
		if (returnedOptional != null && returnedOptional.managedLifetime)
			freshManagedOptionalValueIds.set(result.id, true);
		return {id: result.id, type: result.type, mapping: target.returnMapping};
	}

	/** Call an already evaluated, exact-signature non-capturing function value. */
	function lowerIndirectFunctionCall(expression:TypedExpr, calleeExpression:TypedExpr, argumentExpressions:Array<TypedExpr>, callableMapping:CBodyValueType,
			materializeResult:Bool):Null<LoweredValue> {
		final signature = callableMapping.functionValue();
		if (signature == null)
			return unsupported(calleeExpression, "TCall(indirect-signature-lost)");
		if (argumentExpressions.length != signature.parameters.length)
			return unsupported(expression, 'TCall(indirect-argument-count:expected=${signature.parameters.length},actual=${argumentExpressions.length})');
		var callable = coerce(lowerValue(calleeExpression, callableMapping), callableMapping, calleeExpression.pos, "TCall(indirect-callee)");
		if (laterExpressionCreatesFlow(argumentExpressions, -1))
			callable = stageCallableAcrossArguments(callable, calleeExpression.pos);
		final stagedArguments:Array<StagedFlowValue> = [];
		for (index in 0...argumentExpressions.length) {
			final argumentExpression = argumentExpressions[index];
			final parameter = signature.parameters[index];
			if (referencesStackConstructedValue(argumentExpression))
				return unsupported(argumentExpression, 'TNew(stack-reference-escape:indirect-call-argument:$index)');
			var argument = coerce(lowerValue(argumentExpression, parameter), parameter, argumentExpression.pos, 'TCall(indirect-argument:$index)');
			argument = stabilizeFreshManagedEnum(argument, argumentExpression.pos, 'indirect-call-argument-$index');
			argument = stabilizeFreshManagedAggregate(argument, argumentExpression.pos, 'indirect-call-argument-$index');
			argument = stabilizeFreshManagedOptional(argument, argumentExpression.pos, 'indirect-call-argument-$index');
			if (freshManagedArrayValueIds.exists(argument.id)
				|| freshManagedStringMapValueIds.exists(argument.id)
				|| freshManagedBytesValueIds.exists(argument.id)
				|| borrowedManagedArrayElementValueIds.exists(argument.id))
				return unsupported(argumentExpression, 'TCall(indirect-managed-argument-needs-explicit-ownership:$index)');
			rejectOwnedClassBorrow(argument, argumentExpression.pos, 'TCall(indirect-owned-class-borrow-escape:$index)');
			stagedArguments.push(stageFlowValue(argument, argumentExpression, laterExpressionCreatesFlow(argumentExpressions, index),
				'indirect-call-argument-$index'));
		}
		final arguments = restoreCallArguments(stagedArguments, "indirect-call-argument");
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final returnType = signature.result.irType;
		if (returnType == IRTVoid) {
			appendInstruction(null, IRIOCall({
				dispatch: IRCDClosure(callable.id),
				arguments: arguments,
				returnType: returnType,
				failure: null
			}), source, "indirect-call");
			return null;
		}
		final result:HxcIRResult = {id: nextValueId(), type: returnType};
		appendInstruction(result, IRIOCall({
			dispatch: IRCDClosure(callable.id),
			arguments: arguments,
			returnType: returnType,
			failure: null
		}), source, "indirect-call");
		if (materializeResult)
			registerValueTemporary(result.id, "indirect-call-result");
		if (signature.result.bytesValue() != null)
			freshManagedBytesValueIds.set(result.id, true);
		if (signature.result.stringMapValue() != null)
			freshManagedStringMapValueIds.set(result.id, true);
		final returnedEnum = signature.result.enumValue();
		if (returnedEnum != null && returnedEnum.managedLifetime)
			freshManagedEnumValueIds.set(result.id, true);
		final returnedAggregate = signature.result.aggregateValue();
		if (returnedAggregate != null && returnedAggregate.managedLifetime)
			freshManagedAggregateValueIds.set(result.id, true);
		final returnedOptional = signature.result.optionalValue();
		if (returnedOptional != null && returnedOptional.managedLifetime)
			freshManagedOptionalValueIds.set(result.id, true);
		final returned:LoweredValue = {id: result.id, type: result.type, mapping: signature.result};
		return returned;
	}

	/** Keep a callable available when later argument lowering creates branches. */
	function stageCallableAcrossArguments(callable:LoweredValue, position:Position):LoweredValue {
		final localId = createFlowLocal(callable.mapping, callable.id, HaxeSourceSpan.fromPosition(position, input.sourcePath), "indirect-callee");
		return loadPlace({place: IRPLocal(localId), mapping: callable.mapping, mutable: false}, position, "indirect-callee-reload");
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
		final valuesByName:Map<String, StagedFlowValue> = [];
		for (index => field in fields) {
			if (valuesByName.exists(field.name)) {
				return unsupported(field.expr, 'TCall(c.StructInit.make:duplicate-field:${field.name})');
			}
			final expectedField = imported.field(field.name);
			if (expectedField == null) {
				return unsupported(field.expr, 'TCall(c.StructInit.make:unknown-field:${field.name})');
			}
			final value = coerce(lowerValue(field.expr, expectedField.type), expectedField.type, field.expr.pos,
				'TCall(c.StructInit.make:field:${field.name})');
			valuesByName.set(field.name,
				stageFlowValue(value, field.expr, laterAggregateFieldCreatesFlow(fields, index), 'imported-struct-field-${field.name}'));
		}
		final namedValues:Array<HxcIRNamedValue> = [];
		for (field in imported.fields) {
			final value = valuesByName.get(field.name);
			if (value == null) {
				return unsupported(arguments[0], 'TCall(c.StructInit.make:missing-field:${field.name})');
			}
			namedValues.push({name: field.name, valueId: restoreStagedValue(value, 'imported-struct-field-${field.name}-load')});
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
		// Haxe permits an Int or UInt where a declared Float parameter is
		// expected. Normally the typed call keeps that parameter boundary for us.
		// After Haxe inlines a helper, however, it substitutes the original Int
		// expression directly into this intrinsic call. Reapply the intrinsic's
		// declared input type here so `inline narrow(value:Float)` behaves exactly
		// like the same non-inline function: first perform Haxe's ordinary numeric
		// conversion, then perform the explicit binary32 conversion. `lowerValue`
		// still runs once, so a side-effecting argument is never duplicated.
		final declaredSourceType = mode == FCNarrow ? Context.getType("Float") : Context.getType("c.Float32");
		final declaredSource = bodyValueType(declaredSourceType, arguments[0].pos, 'TCall($surface:declared-input-type)');
		final source = coerce(lowerValue(arguments[0], declaredSource), declaredSource, arguments[0].pos, 'TCall($surface:declared-input)');
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
		final stagedArguments:Array<StagedFlowValue> = [];
		for (index in 0...argumentExpressions.length) {
			final argument = argumentExpressions[index];
			final expected = target.parameters[index];
			final value = expected.isCString() ? lowerBorrowedCString(argument, target,
				index) : coerce(lowerValue(argument, expected), expected, argument.pos, 'native-call:${target.id}:argument:$index');
			stagedArguments.push(stageFlowValue(value, argument, laterExpressionCreatesFlow(argumentExpressions, index), 'native-call-argument-$index'));
		}
		final arguments = restoreCallArguments(stagedArguments, "native-call-argument");
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

	function lowerBorrowedCString(expression:TypedExpr, target:CPreparedImportFunction, argumentIndex:Int):LoweredValue {
		final text = directStringLiteral(expression);
		if (text != null)
			return lowerCStringConstant(expression, text, target, argumentIndex);
		if (!isStaticCStringSelection(expression))
			return invalidAbi(expression,
				'Imported C function `${target.haxePath}` argument $argumentIndex requires a proven static String-literal selection so its borrowed lifetime is static.');
		final mapping = CBodyValueType.cString();
		return coerce(lowerValue(expression, mapping), mapping, expression.pos, 'native-call:${target.id}:static-cstring-argument:$argumentIndex');
	}

	function lowerCStringConstant(expression:TypedExpr, text:String, ?target:CPreparedImportFunction, ?argumentIndex:Int):LoweredValue {
		if (text.indexOf("\x00") != -1)
			return invalidAbi(expression,
				target == null ? "A static c.CString literal contains an embedded NUL byte." : 'Imported C function `${target.haxePath}` argument $argumentIndex contains an embedded NUL byte.');
		final byteLength = HxcUtf8.byteLength(text);
		if (byteLength == null)
			return invalidAbi(expression,
				target == null ? "A static c.CString literal is not valid Unicode-scalar text." : 'Imported C function `${target.haxePath}` argument $argumentIndex is not valid Unicode-scalar text.');
		final mapping = CBodyValueType.cString();
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOConstant(IRCCStringLiteral(text, byteLength)), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath),
			"cstring-literal");
		return {id: result.id, type: result.type, mapping: mapping};
	}

	/**
	 * Prove that an imported C call can only observe NUL-terminated literals
	 * with static storage. A `c.CString` value is admitted because every way to
	 * construct that program-local carrier is checked by this same lowering;
	 * ordinary `String` values are admitted only when every reachable branch is
	 * visibly a literal. This is a lifetime proof, not a general String-to-C
	 * conversion.
	 */
	function isStaticCStringSelection(expression:TypedExpr):Bool {
		switch expression.expr {
			case TParenthesis(inner) | TMeta(_, inner):
				return isStaticCStringSelection(inner);
			case TCast(inner, _) if (isCStringType(expression.t) && !isCStringType(inner.t)):
				// Haxe inserts this abstract conversion for `CString from String`.
				// The destination type alone is not a lifetime proof; inspect the
				// original String expression instead.
				return isStaticCStringSelection(inner);
			case _:
		}
		if (directStringLiteral(expression) != null)
			return true;
		return switch expression.expr {
			case TIf(_, whenTrue, whenFalse): whenFalse != null && isStaticCStringSelection(whenTrue) && isStaticCStringSelection(whenFalse);
			case TSwitch(subject, cases, defaultExpression): (defaultExpression != null
					|| isExhaustiveEnumAbstractSwitch(subject,
						cases)) && allStaticCStringCases(cases) && (defaultExpression == null || isStaticCStringSelection(defaultExpression));
			case TBlock(expressions): expressions.length > 0 && isStaticCStringSelection(expressions[expressions.length - 1]);
			case _: isCStringType(expression.t);
		};
	}

	function allStaticCStringCases(cases:Array<TypedSwitchArm>):Bool {
		for (value in cases)
			if (!isStaticCStringSelection(value.expr))
				return false;
		return true;
	}

	static function isCStringType(type:Type, depth:Int = 0):Bool {
		if (depth > 32)
			return false;
		return switch type {
			case TMono(reference): final resolved = reference.get(); resolved != null && isCStringType(resolved, depth + 1);
			case TLazy(resolve): isCStringType(resolve(), depth + 1);
			case TType(reference, parameters):
				final definition = reference.get();
				isCStringType(TypeTools.applyTypeParameters(definition.type, definition.params, parameters), depth + 1);
			case TAbstract(reference, parameters): final definition = reference.get(); final path = definition.pack.concat([definition.name])
					.join("."); path == "c.CString" || !definition.meta.has(":coreType") && isCStringType(TypeTools.applyTypeParameters(definition.type,
					definition.params, parameters), depth
					+ 1);
			case _: false;
		};
	}

	static function directStringLiteral(expression:TypedExpr):Null<String> {
		return switch expression.expr {
			case TConst(TString(value)): value;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): directStringLiteral(inner);
			case _: null;
		};
	}

	/** Lower one ordinary Haxe Array literal into a typed managed-array operation. */
	function lowerManagedArrayLiteral(expression:TypedExpr, elements:Array<TypedExpr>, expected:Null<CBodyValueType>):LoweredValue {
		final mapping = expected == null ? bodyValueType(expression.t, expression.pos, "TArrayDecl(result-type)") : expected;
		final array = mapping.arrayValue();
		if (array == null)
			return unsupported(expression, 'TArrayDecl(non-Array-result:${mapping.cSpelling})');
		final arguments:Array<String> = [];
		for (index in 0...elements.length) {
			final element = elements[index];
			var loweredElement = coerce(lowerValue(element, array.element), array.element, element.pos, 'TArrayDecl(element:$index)');
			// The runtime copy callback gives the new Array slot its own owner. A
			// fresh record still needs a caller-owned lifetime until that copy
			// succeeds, just like a fresh record passed to `Array.push`.
			loweredElement = stabilizeFreshManagedAggregate(loweredElement, element.pos, 'array-literal-element-$index');
			arguments.push(loweredElement.id);
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("array", "create-literal"),
			arguments: arguments,
			returnType: mapping.irType,
			failure: managedArrayFailure()
		}), source, "array-create-literal");
		registerValueTemporary(result.id, "array-create-result");
		if (!array.managedByCollector)
			freshManagedArrayValueIds.set(result.id, true);
		runtimeRequirements.push(new CBodyRuntimeRequirement("array", "create-literal", "ordinary Haxe Array literal", source, expression.pos));
		return {id: result.id, type: result.type, mapping: mapping};
	}

	/** Every admitted non-null managed field must own a real container on return. */
	function validateConstructorManagedFields(position:Position):Void {
		final owner = switch prepared.role {
			case PBRConstructor(signature): signature.selfMapping.classValue();
			case _: null;
		};
		if (owner == null)
			return;
		for (field in owner.fields)
			if (field.type.arrayValue() != null && !initializedManagedArrayFields.exists(field.name))
				unsupportedAt(position, 'TConstructor(uninitialized-managed-Array-field:${owner.haxePath}.${field.name})');
			else if (field.type.stringMapValue() != null && !initializedManagedStringMapFields.exists(field.name))
				unsupportedAt(position, 'TConstructor(uninitialized-managed-StringMap-field:${owner.haxePath}.${field.name})');
	}

	/** Base-first list of managed fields destroyed with one constructed object. */
	static function managedArrayFields(owner:CPreparedBodyClass):Array<CPreparedBodyClassField> {
		final result:Array<CPreparedBodyClassField> = owner.base == null ? [] : managedArrayFields(owner.base);
		for (field in owner.fields)
			if (field.type.arrayValue() != null && field.type.arrayValue().managedByCollector == false)
				result.push(field);
		return result;
	}

	/** Base-first list of reference-counted StringMap fields owned by one class. */
	static function managedStringMapFields(owner:CPreparedBodyClass):Array<CPreparedBodyClassField> {
		final result:Array<CPreparedBodyClassField> = owner.base == null ? [] : managedStringMapFields(owner.base);
		for (field in owner.fields)
			if (field.type.stringMapValue() != null)
				result.push(field);
		return result;
	}

	/** Read Array.length through the shared container rather than a class field. */
	function lowerManagedArrayLength(expression:TypedExpr, receiverExpression:TypedExpr):LoweredValue {
		final receiver = lowerManagedArrayReceiver(receiverExpression, "length");
		final mapping = bodyValueType(expression.t, expression.pos, "TField(Array.length:result-type)");
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("array", "length"),
			arguments: [receiver.id],
			returnType: mapping.irType,
			failure: managedArrayFailure()
		}), source, "array-length");
		registerValueTemporary(result.id, "array-length-result");
		runtimeRequirements.push(new CBodyRuntimeRequirement("array", "length", "ordinary Haxe Array.length", source, expression.pos));
		return {id: result.id, type: result.type, mapping: mapping};
	}

	/** Checked Array indexing preserves Haxe's signed index and fail-stop policy. */
	function lowerManagedArrayGet(expression:TypedExpr, collection:TypedExpr, index:TypedExpr, mapping:CBodyValueType):LoweredValue {
		final array = mapping.arrayValue();
		if (array == null)
			return unsupported(expression, "TArray(managed-identity-lost)");
		final receiver = coerce(lowerValue(collection, mapping), mapping, collection.pos, "TArray(receiver)");
		final indexType = CBodyValueType.primitive(primitiveMapping(index.t, index.pos, "TArray(index-type)"));
		switch indexType.irType {
			case IRTInt(32, true):
			case _:
				return unsupported(index, 'TArray(index-must-be-Int:actual=${indexType.cSpelling})');
		}
		final indexValue = coerce(lowerValue(index, indexType), indexType, index.pos, "TArray(index)");
		final result:HxcIRResult = {id: nextValueId(), type: array.element.irType};
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("array", "get-checked"),
			arguments: [receiver.id, indexValue.id],
			returnType: array.element.irType,
			failure: managedArrayFailure()
		}), source, "array-get-checked");
		registerValueTemporary(result.id, "array-get-result");
		runtimeRequirements.push(new CBodyRuntimeRequirement("array", "get-checked", "ordinary Haxe Array indexing", source, expression.pos));
		final destroyImplementationId = array.destroyImplementationId();
		if (destroyImplementationId == null)
			return {id: result.id, type: result.type, mapping: array.element};
		if (currentBlock.generatedRole != null && !StringTools.endsWith(currentBlock.generatedRole, "-exit"))
			return unsupported(expression, "TArray(managed-element-owner-in-nested-control-flow-not-yet-admitted)");

		// `get_copy` has constructed a new owned element. Give that value a stable
		// place so every normal exit can run the same typed destroy callback that the
		// Array uses for its slots. Later expressions borrow from this owner; moving
		// the record across a return/call boundary is rejected until that boundary
		// has an explicit ownership-transfer contract.
		final ownerLocalId = createFlowLocal(array.element, result.id, source, "array-element-owner");
		final cleanupId = 'array-element.$ownerLocalId.release';
		constructionCleanupActions.push({
			id: cleanupId,
			idempotence: IRCExactlyOnce,
			kind: IRCARelease(IRPLocal(ownerLocalId), IRIProgramLocal(destroyImplementationId)),
			source: source
		});
		normalCleanupActionIds.push(cleanupId);
		final borrowed = loadPlace({place: IRPLocal(ownerLocalId), mapping: array.element, mutable: false}, expression.pos, "array-element-borrow");
		borrowedManagedArrayElementValueIds.set(borrowed.id, true);
		return borrowed;
	}

	/** Lower the first mutating Array method without entering virtual dispatch. */
	function lowerManagedArrayCall(expression:TypedExpr, access:reflaxe.c.lowering.CBodyDispatch.CBodyInstanceCallAccess, arguments:Array<TypedExpr>,
			materializeResult:Bool):Null<LoweredValue> {
		final method = access.field.get().name;
		final receiver = lowerManagedArrayReceiver(access.receiver, method);
		final array = receiver.mapping.arrayValue();
		if (array == null)
			return unsupported(expression, 'TCall(Array.$method:receiver-identity-lost)');
		return switch method {
			case "push":
				if (arguments.length != 1)
					return unsupported(expression, 'TCall(Array.push:argument-count=${arguments.length})');
				var element = coerce(lowerValue(arguments[0], array.element), array.element, arguments[0].pos, "TCall(Array.push:element)");
				element = stabilizeFreshManagedEnum(element, arguments[0].pos, "array-push-element");
				element = stabilizeFreshManagedAggregate(element, arguments[0].pos, "array-push-element");
				element = stabilizeFreshManagedOptional(element, arguments[0].pos, "array-push-element");
				final resultMapping = bodyValueType(expression.t, expression.pos, "TCall(Array.push:result-type)");
				final result:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
				final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
				appendInstruction(result, IRIOCall({
					dispatch: IRCDRuntime("array", "push"),
					arguments: [receiver.id, element.id],
					returnType: result.type,
					failure: managedArrayFailure()
				}), source, "array-push");
				registerValueTemporary(result.id, "array-push-result");
				runtimeRequirements.push(new CBodyRuntimeRequirement("array", "push", "ordinary Haxe Array.push", source, expression.pos));
				{id: result.id, type: result.type, mapping: resultMapping};
			case _:
				unsupported(expression, 'TCall(Array.$method:not-yet-admitted)');
		};
	}

	function lowerManagedArrayReceiver(expression:TypedExpr, operation:String):LoweredValue {
		final mapping = bodyValueType(expression.t, expression.pos, 'Array.$operation:receiver-type');
		if (mapping.arrayValue() == null)
			return unsupported(expression, 'Array.$operation:receiver-not-Array');
		return coerce(lowerValue(expression, mapping), mapping, expression.pos, 'Array.$operation:receiver');
	}

	static function managedArrayFailure():HxcIRFailureEdge
		return {
			kind: IRFNativeStatus,
			target: IRFTAbort,
			arguments: [],
			cleanup: []
		};

	/** Construct one empty integer-keyed Haxe Map with shared reference identity. */
	function lowerIntMapConstruction(expression:TypedExpr, arguments:Array<TypedExpr>, expected:Null<CBodyValueType>):LoweredValue {
		if (arguments.length != 0)
			return unsupported(expression, 'TNew(IntMap:argument-count=${arguments.length})');
		final mapping = expected == null ? bodyValueType(expression.t, expression.pos, "TNew(IntMap:result-type)") : expected;
		if (mapping.intMapValue() == null)
			return unsupported(expression, 'TNew(IntMap:expected-type=${mapping.cSpelling})');
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("int-map", "create"),
			arguments: [],
			returnType: mapping.irType,
			failure: managedArrayFailure()
		}), source, "int-map-create");
		registerValueTemporary(result.id, "int-map-create-result");
		freshManagedIntMapValueIds.set(result.id, true);
		runtimeRequirements.push(new CBodyRuntimeRequirement("int-map", "create", "ordinary Haxe Map<Int, Bool> construction", source, expression.pos));
		return {id: result.id, type: result.type, mapping: mapping};
	}

	/**
		Lower the first bounded IntMap family without virtual dispatch.

		Only `set(Int, Bool)` and `exists(Int)` are admitted. The table preserves
		key presence separately from the stored Bool, so setting `false` still
		makes `exists` return true exactly as Haxe requires. Reading that stored
		value is intentionally unsupported until `get` owns its nullable contract.
	**/
	function lowerIntMapCall(expression:TypedExpr, access:reflaxe.c.lowering.CBodyDispatch.CBodyInstanceCallAccess, arguments:Array<TypedExpr>,
			materializeResult:Bool):Null<LoweredValue> {
		final receiver = lowerValue(access.receiver);
		final map = receiver.mapping.intMapValue();
		if (map == null)
			return unsupported(access.receiver, "TCall(IntMap:receiver-identity-lost)");
		final method = access.field.get().name;
		final expectedArguments = method == "set" ? 2 : 1;
		if (method != "set" && method != "exists")
			return unsupported(expression, 'TCall(IntMap.$method:not-yet-admitted)');
		if (arguments.length != expectedArguments)
			return unsupported(expression, 'TCall(IntMap.$method:argument-count=${arguments.length},expected=$expectedArguments)');
		final intMapping = bodyValueType(arguments[0].t, arguments[0].pos, 'TCall(IntMap.$method:key-type)');
		switch intMapping.irType {
			case IRTInt(32, true):
			case _:
				return unsupported(arguments[0], 'TCall(IntMap.$method:key-not-Haxe-Int)');
		}
		final key = coerce(lowerValue(arguments[0], intMapping), intMapping, arguments[0].pos, 'TCall(IntMap.$method:key)');
		final loweredArguments:Array<String> = [receiver.id, key.id];
		if (method == "set")
			loweredArguments.push(coerce(lowerValue(arguments[1], map.value), map.value, arguments[1].pos, "TCall(IntMap.set:value)").id);
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		if (method == "set") {
			appendInstruction(null, IRIOCall({
				dispatch: IRCDRuntime("int-map", "set"),
				arguments: loweredArguments,
				returnType: IRTVoid,
				failure: managedArrayFailure()
			}), source, "int-map-set");
			runtimeRequirements.push(new CBodyRuntimeRequirement("int-map", "set", "ordinary Haxe IntMap.set", source, expression.pos));
			return null;
		}
		final resultMapping = bodyValueType(expression.t, expression.pos, "TCall(IntMap.exists:result-type)");
		final result:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("int-map", "exists"),
			arguments: loweredArguments,
			returnType: result.type,
			failure: managedArrayFailure()
		}), source, "int-map-exists");
		registerValueTemporary(result.id, "int-map-exists-result");
		runtimeRequirements.push(new CBodyRuntimeRequirement("int-map", "exists", "ordinary Haxe IntMap.exists", source, expression.pos));
		return {id: result.id, type: result.type, mapping: resultMapping};
	}

	/** Construct one empty String-keyed Haxe Map with shared reference identity. */
	function lowerStringMapConstruction(expression:TypedExpr, arguments:Array<TypedExpr>, expected:Null<CBodyValueType>):LoweredValue {
		if (arguments.length != 0)
			return unsupported(expression, 'TNew(StringMap:argument-count=${arguments.length})');
		final mapping = expected == null ? bodyValueType(expression.t, expression.pos, "TNew(StringMap:result-type)") : expected;
		if (mapping.stringMapValue() == null)
			return unsupported(expression, 'TNew(StringMap:expected-type=${mapping.cSpelling})');
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("string-map", "create"),
			arguments: [],
			returnType: mapping.irType,
			failure: managedArrayFailure()
		}), source, "string-map-create");
		registerValueTemporary(result.id, "string-map-create-result");
		freshManagedStringMapValueIds.set(result.id, true);
		runtimeRequirements.push(new CBodyRuntimeRequirement("string-map", "create", "ordinary Haxe Map<String, V> construction", source, expression.pos));
		return {id: result.id, type: result.type, mapping: mapping};
	}

	/**
		Lower the complete first StringMap method family without virtual dispatch.

		The prepared receiver is authoritative because Haxe 5 can omit the value
		parameter from a method-access receiver after multi-type specialization.
	**/
	function lowerStringMapCall(expression:TypedExpr, access:reflaxe.c.lowering.CBodyDispatch.CBodyInstanceCallAccess, arguments:Array<TypedExpr>,
			materializeResult:Bool):Null<LoweredValue> {
		final receiver = lowerValue(access.receiver);
		final map = receiver.mapping.stringMapValue();
		if (map == null)
			return unsupported(access.receiver, "TCall(StringMap:receiver-identity-lost)");
		final method = access.field.get().name;
		final expectedArguments = method == "clear" ? 0 : method == "set" ? 2 : 1;
		if (arguments.length != expectedArguments)
			return unsupported(expression, 'TCall(StringMap.$method:argument-count=${arguments.length},expected=$expectedArguments)');
		final loweredArguments:Array<String> = [receiver.id];
		if (arguments.length > 0) {
			final keyMapping = bodyValueType(arguments[0].t, arguments[0].pos, 'TCall(StringMap.$method:key-type)');
			if (keyMapping.staticStringIdentity() == null)
				return unsupported(arguments[0], 'TCall(StringMap.$method:key-not-admitted-String)');
			loweredArguments.push(coerce(lowerValue(arguments[0], keyMapping), keyMapping, arguments[0].pos, 'TCall(StringMap.$method:key)').id);
		}
		if (method == "set")
			loweredArguments.push(coerce(lowerValue(arguments[1], map.value), map.value, arguments[1].pos, "TCall(StringMap.set:value)").id);
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		if (method == "set" || method == "clear") {
			appendInstruction(null, IRIOCall({
				dispatch: IRCDRuntime("string-map", method),
				arguments: loweredArguments,
				returnType: IRTVoid,
				failure: managedArrayFailure()
			}), source, 'string-map-$method');
			runtimeRequirements.push(new CBodyRuntimeRequirement("string-map", method, 'ordinary Haxe StringMap.$method', source, expression.pos));
			return null;
		}
		if (method != "exists" && method != "get" && method != "remove")
			return unsupported(expression, 'TCall(StringMap.$method:not-yet-admitted)');
		final resultMapping = bodyValueType(expression.t, expression.pos, 'TCall(StringMap.$method:result-type)');
		final result:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("string-map", method),
			arguments: loweredArguments,
			returnType: result.type,
			failure: managedArrayFailure()
		}), source, 'string-map-$method');
		registerValueTemporary(result.id, 'string-map-$method-result');
		if (method == "get") {
			final optional = resultMapping.optionalValue();
			if (optional != null && optional.managedLifetime)
				freshManagedOptionalValueIds.set(result.id, true);
		}
		runtimeRequirements.push(new CBodyRuntimeRequirement("string-map", method, 'ordinary Haxe StringMap.$method', source, expression.pos));
		return {id: result.id, type: result.type, mapping: resultMapping};
	}

	/**
		Lower the first allocation-free ordinary Haxe String operation.

		`charAt` returns a view into the receiver's immutable UTF-8 bytes. It does
		not allocate or copy: the selected scalar runtime finds one Unicode scalar
		and returns its byte range, while negative or out-of-range indices produce
		the empty String required by Haxe. The receiver is lowered before the index,
		preserving Haxe's left-to-right evaluation order.
	**/
	function lowerStringCall(expression:TypedExpr, access:reflaxe.c.lowering.CBodyDispatch.CBodyInstanceCallAccess, arguments:Array<TypedExpr>):LoweredValue {
		final method = access.field.get().name;
		if (method != "charAt")
			return unsupported(expression, 'TCall(String.$method:not-yet-admitted)');
		if (arguments.length != 1)
			return unsupported(expression, 'TCall(String.charAt:argument-count=${arguments.length},expected=1)');
		final receiverMapping = bodyValueType(access.receiver.t, access.receiver.pos, "TCall(String.charAt:receiver-type)");
		if (receiverMapping.staticStringIdentity() == null || typeKey(receiverMapping.irType) != typeKey(IRTString))
			return unsupported(access.receiver, "TCall(String.charAt:receiver-not-immutable-String-view)");
		final receiver = coerce(lowerValue(access.receiver, receiverMapping), receiverMapping, access.receiver.pos, "TCall(String.charAt:receiver)");
		final indexMapping = bodyValueType(arguments[0].t, arguments[0].pos, "TCall(String.charAt:index-type)");
		if (typeKey(indexMapping.irType) != typeKey(IRTInt(32, true)))
			return unsupported(arguments[0], "TCall(String.charAt:index-requires-Haxe-Int)");
		final index = coerce(lowerValue(arguments[0], indexMapping), indexMapping, arguments[0].pos, "TCall(String.charAt:index)");
		final resultMapping = bodyValueType(expression.t, expression.pos, "TCall(String.charAt:result-type)");
		if (resultMapping.staticStringIdentity() == null || typeKey(resultMapping.irType) != typeKey(IRTString))
			return unsupported(expression, "TCall(String.charAt:result-not-immutable-String-view)");
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final result:HxcIRResult = {id: nextValueId(), type: IRTString};
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("string-scalar", "char-at"),
			arguments: [receiver.id, index.id],
			returnType: IRTString,
			failure: null
		}), source, "string-char-at");
		registerValueTemporary(result.id, "string-char-at-result");
		runtimeRequirements.push(new CBodyRuntimeRequirement("string-scalar", "char-at", "ordinary Haxe String.charAt with Unicode-scalar indexing", source,
			expression.pos));
		return {id: result.id, type: result.type, mapping: resultMapping};
	}

	/** Read Bytes.length through the fixed-length shared binary owner. */
	function lowerManagedBytesLength(expression:TypedExpr, receiverExpression:TypedExpr):LoweredValue {
		final receiver = lowerManagedBytesReceiver(receiverExpression, "length");
		final mapping = bodyValueType(expression.t, expression.pos, "TField(Bytes.length:result-type)");
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("bytes", "length"),
			arguments: [receiver.id],
			returnType: mapping.irType,
			failure: managedBytesFailure()
		}), source, "bytes-length");
		registerValueTemporary(result.id, "bytes-length-result");
		runtimeRequirements.push(new CBodyRuntimeRequirement("bytes", "length", "ordinary haxe.io.Bytes.length", source, expression.pos));
		return {id: result.id, type: result.type, mapping: mapping};
	}

	/** Lower the admitted static constructors without entering class dispatch. */
	function lowerManagedBytesStaticCall(expression:TypedExpr, method:String, arguments:Array<TypedExpr>):LoweredValue {
		final mapping = bodyValueType(expression.t, expression.pos, 'TCall(Bytes.$method:result-type)');
		if (mapping.bytesValue() == null)
			return unsupported(expression, 'TCall(Bytes.$method:result-not-Bytes)');
		final loweredArguments:Array<String> = [];
		switch method {
			case "alloc":
				if (arguments.length != 1)
					return unsupported(expression, 'TCall(Bytes.alloc:argument-count=${arguments.length})');
				loweredArguments.push(lowerBytesIntArgument(arguments[0], "Bytes.alloc:length").id);
			case "ofString":
				if (arguments.length < 1 || arguments.length > 2)
					return unsupported(expression, 'TCall(Bytes.ofString:argument-count=${arguments.length})');
				if (arguments.length == 2 && !isNullExpression(arguments[1]))
					return unsupported(arguments[1], "TCall(Bytes.ofString:explicit-encoding-not-yet-admitted)");
				final text = stringLiteral(arguments[0]);
				if (text == null)
					return unsupported(arguments[0], "TCall(Bytes.ofString:non-literal-String-not-yet-admitted)");
				final byteLength = HxcUtf8.byteLength(text);
				if (byteLength == null)
					return unsupported(arguments[0], "TCall(Bytes.ofString:malformed-Unicode-literal)");
				final literalResult:HxcIRResult = {id: nextValueId(), type: IRTString};
				final literalSource = HaxeSourceSpan.fromPosition(arguments[0].pos, input.sourcePath);
				appendInstruction(literalResult, IRIOConstant(IRCString(text, byteLength)), literalSource, "bytes-string-literal");
				runtimeRequirements.push(new CBodyRuntimeRequirement("string-literal", "static-value", "Bytes.ofString literal", literalSource,
					arguments[0].pos, "direct-string-value"));
				loweredArguments.push(literalResult.id);
			case _:
				return unsupported(expression, 'TCall(Bytes.$method:not-yet-admitted)');
		}
		final operation = method == "alloc" ? "alloc" : "of-string-utf8";
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("bytes", operation),
			arguments: loweredArguments,
			returnType: mapping.irType,
			failure: managedBytesFailure()
		}), source, 'bytes-$operation');
		registerValueTemporary(result.id, 'bytes-$operation-result');
		freshManagedBytesValueIds.set(result.id, true);
		runtimeRequirements.push(new CBodyRuntimeRequirement("bytes", operation, 'ordinary haxe.io.Bytes.$method', source, expression.pos));
		return {id: result.id, type: result.type, mapping: mapping};
	}

	/** Lower fixed-length byte operations through one typed runtime family. */
	function lowerManagedBytesCall(expression:TypedExpr, access:reflaxe.c.lowering.CBodyDispatch.CBodyInstanceCallAccess, arguments:Array<TypedExpr>,
			materializeResult:Bool):Null<LoweredValue> {
		final method = access.field.get().name;
		final receiver = lowerManagedBytesReceiver(access.receiver, method);
		final loweredArguments:Array<String> = [receiver.id];
		var operation = method;
		switch method {
			case "get":
				if (arguments.length != 1)
					return unsupported(expression, 'TCall(Bytes.get:argument-count=${arguments.length})');
				loweredArguments.push(lowerBytesIntArgument(arguments[0], "Bytes.get:position").id);
			case "set":
				if (arguments.length != 2)
					return unsupported(expression, 'TCall(Bytes.set:argument-count=${arguments.length})');
				loweredArguments.push(lowerBytesIntArgument(arguments[0], "Bytes.set:position").id);
				loweredArguments.push(lowerBytesIntArgument(arguments[1], "Bytes.set:value").id);
			case "sub":
				if (arguments.length != 2)
					return unsupported(expression, 'TCall(Bytes.sub:argument-count=${arguments.length})');
				loweredArguments.push(lowerBytesIntArgument(arguments[0], "Bytes.sub:position").id);
				loweredArguments.push(lowerBytesIntArgument(arguments[1], "Bytes.sub:length").id);
			case "compare":
				if (arguments.length != 1)
					return unsupported(expression, 'TCall(Bytes.compare:argument-count=${arguments.length})');
				final otherMapping = bodyValueType(arguments[0].t, arguments[0].pos, "Bytes.compare:other-type");
				if (otherMapping.bytesValue() == null)
					return unsupported(arguments[0], "TCall(Bytes.compare:other-not-Bytes)");
				final other = coerce(lowerValue(arguments[0], otherMapping), otherMapping, arguments[0].pos, "Bytes.compare:other");
				if (freshManagedBytesValueIds.exists(other.id))
					return unsupported(arguments[0], "Bytes.compare:fresh-argument-needs-owner");
				loweredArguments.push(other.id);
			case "blit":
				if (arguments.length != 4)
					return unsupported(expression, 'TCall(Bytes.blit:argument-count=${arguments.length})');
				loweredArguments.push(lowerBytesIntArgument(arguments[0], "Bytes.blit:destination-position").id);
				final sourceMapping = bodyValueType(arguments[1].t, arguments[1].pos, "Bytes.blit:source-type");
				if (sourceMapping.bytesValue() == null)
					return unsupported(arguments[1], "TCall(Bytes.blit:source-not-Bytes)");
				final sourceValue = coerce(lowerValue(arguments[1], sourceMapping), sourceMapping, arguments[1].pos, "Bytes.blit:source");
				if (freshManagedBytesValueIds.exists(sourceValue.id))
					return unsupported(arguments[1], "Bytes.blit:fresh-source-needs-owner");
				loweredArguments.push(sourceValue.id);
				loweredArguments.push(lowerBytesIntArgument(arguments[2], "Bytes.blit:source-position").id);
				loweredArguments.push(lowerBytesIntArgument(arguments[3], "Bytes.blit:length").id);
			case "fill":
				if (arguments.length != 3)
					return unsupported(expression, 'TCall(Bytes.fill:argument-count=${arguments.length})');
				loweredArguments.push(lowerBytesIntArgument(arguments[0], "Bytes.fill:position").id);
				loweredArguments.push(lowerBytesIntArgument(arguments[1], "Bytes.fill:length").id);
				loweredArguments.push(lowerBytesIntArgument(arguments[2], "Bytes.fill:value").id);
			case _:
				return unsupported(expression, 'TCall(Bytes.$method:not-yet-admitted)');
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final resultMapping = bodyValueType(expression.t, expression.pos, 'TCall(Bytes.$method:result-type)');
		if (resultMapping.irType == IRTVoid) {
			appendInstruction(null, IRIOCall({
				dispatch: IRCDRuntime("bytes", operation),
				arguments: loweredArguments,
				returnType: IRTVoid,
				failure: managedBytesFailure()
			}), source, 'bytes-$operation');
			runtimeRequirements.push(new CBodyRuntimeRequirement("bytes", operation, 'ordinary haxe.io.Bytes.$method', source, expression.pos));
			return null;
		}
		final result:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
		appendInstruction(result, IRIOCall({
			dispatch: IRCDRuntime("bytes", operation),
			arguments: loweredArguments,
			returnType: result.type,
			failure: managedBytesFailure()
		}), source, 'bytes-$operation');
		registerValueTemporary(result.id, 'bytes-$operation-result');
		if (resultMapping.bytesValue() != null)
			freshManagedBytesValueIds.set(result.id, true);
		runtimeRequirements.push(new CBodyRuntimeRequirement("bytes", operation, 'ordinary haxe.io.Bytes.$method', source, expression.pos));
		return {id: result.id, type: result.type, mapping: resultMapping};
	}

	function lowerManagedBytesReceiver(expression:TypedExpr, operation:String):LoweredValue {
		final mapping = bodyValueType(expression.t, expression.pos, 'Bytes.$operation:receiver-type');
		if (mapping.bytesValue() == null)
			return unsupported(expression, 'Bytes.$operation:receiver-not-Bytes');
		final receiver = coerce(lowerValue(expression, mapping), mapping, expression.pos, 'Bytes.$operation:receiver');
		if (freshManagedBytesValueIds.exists(receiver.id))
			return unsupported(expression, 'Bytes.$operation:fresh-receiver-needs-owner');
		return receiver;
	}

	function lowerBytesIntArgument(expression:TypedExpr, role:String):LoweredValue {
		final mapping = bodyValueType(expression.t, expression.pos, role);
		switch mapping.irType {
			case IRTInt(32, true):
			case _:
				return unsupported(expression, '$role:requires-Haxe-Int');
		}
		return coerce(lowerValue(expression, mapping), mapping, expression.pos, role);
	}

	static function managedBytesFailure():HxcIRFailureEdge
		return {
			kind: IRFNativeStatus,
			target: IRFTAbort,
			arguments: [],
			cleanup: []
		};

	function lowerInstanceCall(expression:TypedExpr, access:reflaxe.c.lowering.CBodyDispatch.CBodyInstanceCallAccess, argumentExpressions:Array<TypedExpr>,
			materializeResult:Bool):Null<LoweredValue> {
		final declaration = CBodyDispatchCatalog.declaringClass(access.owner, access.field);
		final field = access.field.get();
		final baseTargetId = CBodyDispatchCatalog.methodIdForAccess(access.owner, access.field);
		final interfaceCall = declaration.get().isInterface;
		final ownerMapping = bodyValueType(interfaceCall ? access.receiver.t : TInst(declaration, []), access.receiver.pos,
			'TCall(instance:$baseTargetId:receiver-type)');
		if (!interfaceCall && ownerMapping.classValue() == null)
			return unsupported(expression, 'TCall(instance:$baseTargetId:receiver-not-concrete-class)');
		if (interfaceCall && ownerMapping.interfaceValue() == null)
			return unsupported(expression, 'TCall(instance:$baseTargetId:receiver-not-interface)');
		var receiver = if (CBodyDispatchCatalog.isSuperReceiver(access.receiver)) {
			final self = selfValue;
			self == null ? unsupported(access.receiver, 'TCall(super-method:outside-instance-method:$baseTargetId)') : self;
		} else {
			lowerValue(access.receiver);
		};
		receiver = coerce(receiver, ownerMapping, access.receiver.pos, 'TCall(instance:$baseTargetId:receiver)');

		final directReason = interfaceCall ? null : CBodyDispatchCatalog.directReason(access.receiver, declaration, field);
		final targetId = directReason != null
			&& field.params.length != 0 ? CGenericCallResolver.resolve(baseTargetId, field.type, field.params, access.calleeType,
				argumentExpressions.map(argument -> argument.t), input.specialization, context.profile, expression.pos, unsupportedAt)
				.instanceId() : baseTargetId;
		final explicitMappings:Array<CBodyValueType> = [];
		final explicitBorrowedClasses:Array<Bool> = [];
		var returnMapping:CBodyValueType;
		var dispatchKind:HxcIRCallDispatch;
		var directTarget:Null<PreparedBodyFunction> = null;
		if (interfaceCall) {
			final interfaceValue = ownerMapping.interfaceValue();
			if (interfaceValue == null)
				return unsupported(expression, 'TCall(instance:$targetId:interface-identity-lost)');
			final slot = dispatch.slotForInterface(interfaceValue.instanceId, field.name);
			if (slot == null)
				return unsupported(expression, 'TCall(unavailable-interface-slot:$targetId)');
			for (mapping in slot.parameters) {
				explicitMappings.push(mapping);
				explicitBorrowedClasses.push(false);
			}
			returnMapping = slot.returnType;
			dispatchKind = IRCDInterface(interfaceValue.instanceId, slot.input.id, receiver.id);
		} else if (directReason != null) {
			directTarget = functionsById.get(targetId);
			if (directTarget == null)
				return unsupported(expression, 'TCall(unavailable-instance-target:$targetId)');
			if (directTarget.parameters.length == 0)
				throw new CBodyEmissionError('instance target `$targetId` lost its self parameter');
			for (index in 1...directTarget.parameters.length) {
				explicitMappings.push(directTarget.parameters[index].mapping);
				explicitBorrowedClasses.push(directTarget.parameters[index].borrowedReference);
			}
			returnMapping = directTarget.returnMapping;
			dispatchKind = IRCDDirect(targetId);
		} else {
			final slot = dispatch.slotForMethodId(targetId);
			if (slot == null)
				return unsupported(expression, 'TCall(unavailable-virtual-slot:$targetId)');
			final slotOwner = slot.ownerClass;
			if (slotOwner == null)
				return unsupported(expression, 'TCall(interface-slot-requires-interface-call-lowering:$targetId)');
			final slotReceiver = CBodyValueType.classReference(slotOwner, true);
			receiver = coerce(receiver, slotReceiver, access.receiver.pos, 'TCall(instance:$targetId:virtual-receiver)');
			for (mapping in slot.parameters) {
				explicitMappings.push(mapping);
				explicitBorrowedClasses.push(false);
			}
			returnMapping = slot.returnType;
			dispatchKind = IRCDVirtual(slot.input.id, receiver.id);
		}
		if (isNullableClassReference(receiver.type)) {
			appendInstruction(null, IRIONullCheck(receiver.id, IRNCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))),
				HaxeSourceSpan.fromPosition(access.receiver.pos, input.sourcePath), "instance-call-null-check");
		}
		final effectiveArgumentExpressions = directTarget == null ? argumentExpressions : completeDirectCallArguments(expression, argumentExpressions,
			directTarget.parameters, 1, targetId, "instance-argument");
		if (effectiveArgumentExpressions.length != explicitMappings.length)
			return unsupported(expression,
				'TCall(instance-argument-count=${effectiveArgumentExpressions.length},expected=${explicitMappings.length},target=$targetId)');
		final explicitArguments:Array<String> = [];
		for (index in 0...effectiveArgumentExpressions.length) {
			final argument = effectiveArgumentExpressions[index];
			if (referencesStackConstructedValue(argument) && !explicitBorrowedClasses[index])
				return unsupported(argument, 'TNew(stack-reference-escape:instance-call-argument:$index,target=$targetId)');
			var value = coerce(lowerValue(argument, explicitMappings[index]), explicitMappings[index], argument.pos,
				'TCall(instance-argument:$index,target=$targetId)');
			value = stabilizeFreshManagedEnum(value, argument.pos, 'instance-call-argument-$index');
			value = stabilizeFreshManagedAggregate(value, argument.pos, 'instance-call-argument-$index');
			value = stabilizeFreshManagedOptional(value, argument.pos, 'instance-call-argument-$index');
			if (freshManagedArrayValueIds.exists(value.id))
				return unsupported(argument, 'TCall(fresh-managed-Array-argument-needs-owner:$index,target=$targetId)');
			if (freshManagedStringMapValueIds.exists(value.id))
				return unsupported(argument, 'TCall(fresh-managed-StringMap-argument-needs-owner:$index,target=$targetId)');
			if (freshManagedBytesValueIds.exists(value.id))
				return unsupported(argument, 'TCall(fresh-managed-Bytes-argument-needs-owner:$index,target=$targetId)');
			if (borrowedManagedArrayElementValueIds.exists(value.id))
				return unsupported(argument, 'TCall(borrowed-managed-Array-element-argument:$index,target=$targetId)');
			if (!explicitBorrowedClasses[index])
				rejectOwnedClassBorrow(value, argument.pos, 'TCall(owned-class-borrow-escape:instance-call-argument:$index,target=$targetId)');
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
		if (borrowedClassValueIds.exists(receiver.id) && returnMapping.classValue() != null)
			borrowedClassValueIds.set(result.id, true);
		if (returnMapping.bytesValue() != null)
			freshManagedBytesValueIds.set(result.id, true);
		if (returnMapping.stringMapValue() != null)
			freshManagedStringMapValueIds.set(result.id, true);
		final returnedEnum = returnMapping.enumValue();
		if (returnedEnum != null && returnedEnum.managedLifetime)
			freshManagedEnumValueIds.set(result.id, true);
		final returnedAggregate = returnMapping.aggregateValue();
		if (returnedAggregate != null && returnedAggregate.managedLifetime)
			freshManagedAggregateValueIds.set(result.id, true);
		final returnedOptional = returnMapping.optionalValue();
		if (returnedOptional != null && returnedOptional.managedLifetime)
			freshManagedOptionalValueIds.set(result.id, true);
		return {id: result.id, type: result.type, mapping: returnMapping};
	}

	/**
	 * Complete a compiler-known direct call with its declaration defaults.
	 *
	 * Haxe leaves omitted trailing arguments out of `TCall` for this target.
	 * C has fixed arity, so haxe.c appends the already typed declaration values
	 * before building HxcIR. This keeps the IR and generated C simple: every
	 * call has the full parameter list and needs no runtime "was this supplied?"
	 * test. A written `null` is already in `written`, so it is never confused
	 * with omission.
	 *
	 * Only direct calls use this helper. Virtual and interface dispatch do not
	 * yet carry the one proven declaration whose default owns the semantics.
	 */
	function completeDirectCallArguments(call:TypedExpr, written:Array<TypedExpr>, parameters:Array<PreparedParameter>, parameterOffset:Int, targetId:String,
			diagnosticRole:String):Array<TypedExpr> {
		final expected = parameters.length - parameterOffset;
		if (written.length > expected)
			return unsupported(call, 'TCall($diagnosticRole-count=${written.length},expected=$expected,target=$targetId)');
		final completed = written.copy();
		for (index in written.length...expected) {
			final defaultValue = parameters[index + parameterOffset].defaultValue;
			if (defaultValue == null)
				return unsupported(call, 'TCall($diagnosticRole-count=${written.length},expected=$expected,target=$targetId)');
			completed.push(defaultAtCallSite(defaultValue, call.pos));
		}
		return completed;
	}

	/** Preserve the typed default while attributing failures to the call that uses it. */
	static function defaultAtCallSite(value:TypedExpr, position:Position):TypedExpr
		return {expr: value.expr, t: value.t, pos: position};

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
		runtimeRequirements.push(new CBodyRuntimeRequirement("string-literal", "static-value", surface, source, expression.pos, "direct-string-value"));
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
		runtimeRequirements.push(new CBodyRuntimeRequirement("io", operationId, surface, source, expression.pos, "hosted-output"));
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

	static function isFunctionType(type:Type):Bool {
		return switch TypeTools.follow(type) {
			case TFun(_, _): true;
			case _: false;
		};
	}

	static function isDirectStaticFunctionExpression(expression:TypedExpr):Bool {
		return switch expression.expr {
			case TField(_, FStatic(_, _)): true;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isDirectStaticFunctionExpression(inner);
			case _: false;
		};
	}

	static function isHaxeLogTrace(callee:TypedExpr):Bool
		return isStaticMethod(callee, "haxe", "Log", "trace");

	static function coreBytesStaticMethod(callee:TypedExpr):Null<String> {
		return switch unwrapExpression(callee).expr {
			case TField(_, FStatic(classReference, fieldReference)) if (CBodyBytesRecognition.isCoreBytes(classReference)):
				fieldReference.get().name;
			case _: null;
		};
	}

	static function isNullExpression(expression:TypedExpr):Bool {
		return switch unwrapExpression(expression).expr {
			case TConst(TNull): true;
			case _: false;
		};
	}

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

	/** Reject a class pointer whose storage remains owned by its caller or parent. */
	function rejectOwnedClassBorrow(value:LoweredValue, position:Position, node:String):Void {
		if (borrowedClassValueIds.exists(value.id))
			unsupportedAt(position, node);
	}

	/** Preserve the borrow fact across representation-only pointer conversions. */
	function carryOwnedClassBorrow(sourceId:String, resultId:String):Void {
		if (borrowedClassValueIds.exists(sourceId))
			borrowedClassValueIds.set(resultId, true);
	}

	/**
		Give a fresh reference-counted Array a caller-owned lifetime around a call.

		Constructor parameters borrow their Array value. If the callee stores that
		value, its field takes a separate retain; it cannot consume an otherwise
		ownerless literal from the caller. This temporary local owns the literal
		until normal or failure cleanup, making both call-only and retained
		constructor uses obey the same rule. Collector-managed Arrays already have
		an exact root and therefore need no reference-count operation.
	**/
	function stabilizeFreshManagedArray(value:LoweredValue, position:Position, role:String):LoweredValue {
		final managed = value.mapping.arrayValue();
		if (managed == null || managed.managedByCollector || !freshManagedArrayValueIds.remove(value.id))
			return value;
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		final ownerLocalId = createFlowLocal(value.mapping, value.id, source, role + "-owner");
		final cleanupId = 'array-temporary.$ownerLocalId.release';
		constructionCleanupActions.push({
			id: cleanupId,
			idempotence: IRCExactlyOnce,
			kind: IRCARelease(IRPLocal(ownerLocalId), IRIRuntime("array")),
			source: source
		});
		normalCleanupActionIds.push(cleanupId);
		runtimeRequirements.push(new CBodyRuntimeRequirement("array", "cleanup-release", "fresh ordinary Haxe Array constructor argument lifetime", source,
			position));
		return loadPlace({place: IRPLocal(ownerLocalId), mapping: value.mapping, mutable: false}, position, role + "-borrow");
	}

	/**
		Give a temporary managed enum a stable owner before another operation borrows it.

		Calls and `Array.push` copy their arguments; they do not consume them. A
		fresh enum constructor therefore still needs one caller-owned lifetime.
		The local created here owns that lifetime until the function's normal or
		failure cleanup, while the callee or destination retains its own copy.
	**/
	function stabilizeFreshManagedEnum(value:LoweredValue, position:Position, role:String):LoweredValue {
		final managed = value.mapping.enumValue();
		if (managed == null || !managed.managedLifetime || !freshManagedEnumValueIds.remove(value.id))
			return value;
		final destroyId = managed.destroyImplementationId();
		if (destroyId == null)
			throw new CBodyEmissionError('managed enum `${managed.instanceId}` lost its destroy plan');
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		final ownerLocalId = createFlowLocal(value.mapping, value.id, source, role + "-owner");
		final cleanupId = 'enum-temporary.$ownerLocalId.release';
		constructionCleanupActions.push({
			id: cleanupId,
			idempotence: IRCExactlyOnce,
			kind: IRCARelease(IRPLocal(ownerLocalId), IRIProgramLocal(destroyId)),
			source: source
		});
		normalCleanupActionIds.push(cleanupId);
		return loadPlace({place: IRPLocal(ownerLocalId), mapping: value.mapping, mutable: false}, position, role + "-borrow");
	}

	/**
		Keep a fresh managed record alive while another operation copies it.

		The automatic local owns the fresh value until normal cleanup. Calls and
		Array insertion borrow that stable C value and acquire their own owner
		through the record's typed retain plan.
	**/
	function stabilizeFreshManagedAggregate(value:LoweredValue, position:Position, role:String):LoweredValue {
		final managed = value.mapping.aggregateValue();
		if (managed == null || !managed.managedLifetime || !freshManagedAggregateValueIds.remove(value.id))
			return value;
		final destroyId = managed.destroyImplementationId();
		if (destroyId == null)
			throw new CBodyEmissionError('managed aggregate `${managed.instanceId}` lost its destroy plan');
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		final ownerLocalId = createFlowLocal(value.mapping, value.id, source, role + "-owner");
		final cleanupId = 'record-temporary.$ownerLocalId.release';
		constructionCleanupActions.push({
			id: cleanupId,
			idempotence: IRCExactlyOnce,
			kind: IRCARelease(IRPLocal(ownerLocalId), IRIProgramLocal(destroyId)),
			source: source
		});
		normalCleanupActionIds.push(cleanupId);
		return loadPlace({place: IRPLocal(ownerLocalId), mapping: value.mapping, mutable: false}, position, role + "-borrow");
	}

	/** Keep a fresh managed tagged optional alive while another operation borrows it. */
	function stabilizeFreshManagedOptional(value:LoweredValue, position:Position, role:String):LoweredValue {
		final managed = value.mapping.optionalValue();
		if (managed == null || !managed.managedLifetime || !freshManagedOptionalValueIds.remove(value.id))
			return value;
		final destroyId = managed.destroyImplementationId();
		if (destroyId == null)
			throw new CBodyEmissionError('managed optional `${managed.planId}` lost its destroy plan');
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		final ownerLocalId = createFlowLocal(value.mapping, value.id, source, role + "-owner");
		final cleanupId = 'optional-temporary.$ownerLocalId.release';
		constructionCleanupActions.push({
			id: cleanupId,
			idempotence: IRCExactlyOnce,
			kind: IRCARelease(IRPLocal(ownerLocalId), IRIProgramLocal(destroyId)),
			source: source
		});
		normalCleanupActionIds.push(cleanupId);
		return loadPlace({place: IRPLocal(ownerLocalId), mapping: value.mapping, mutable: false}, position, role + "-borrow");
	}

	/**
	 * Preserve one lowered value across a branch or join in automatic storage.
	 *
	 * An HxcIR temporary belongs to the basic block that computed it, so a later
	 * branch cannot safely name it from its join block. This helper creates a
	 * typed local, records its deterministic C name, and initializes it before
	 * control leaves the current block. Span values also reserve the paired
	 * length name because their C representation is a pointer plus a length.
	 */
	function createFlowLocal(mapping:CBodyValueType, initialValueId:Null<String>, source:HxcSourceSpan, role:String):String {
		final localId = declareFlowLocal(mapping, source, role);
		if (initialValueId == null) {
			switch mapping.kind {
				case CBVKAggregate(_):
					appendInstruction(null, IRIODefaultInitialize(IRPLocal(localId), IRISUninitialized, IRISInitialized), source, role
						+ "-default-initialize");
				case _:
					throw new CBodyEmissionError('flow local `$localId` in `${prepared.irId}` omitted a required initial value');
			}
		} else {
			appendInstruction(null, IRIOInitialize(IRPLocal(localId), initialValueId, IRISUninitialized, IRISInitialized), source, role + "-initialize");
		}
		return localId;
	}

	/**
	 * Declare an automatic flow carrier without choosing its initial value.
	 *
	 * Most callers immediately initialize the local in the current block through
	 * `createFlowLocal`. A conditional direct value instead emits the narrow
	 * uninitialized-declaration operation, then assigns it in both branches.
	 */
	function declareFlowLocal(mapping:CBodyValueType, source:HxcSourceSpan, role:String):String {
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
		switch mapping.kind {
			case CBVKSpan(_, _):
				// A C span is emitted as two values: its pointer and its length.
				// Register both names when control flow saves the span, just as we
				// do for source locals and parameters. The matching HxcIR
				// initializer carries both values across the join.
				final lengthRequest = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, role, "length"]),
					CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
				context.symbols.register(lengthRequest);
				spanLengthRequests.set(localId, lengthRequest);
			case _:
		}
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
			case IRTString: IRCString("", 0);
			case IRTCString: IRCCStringLiteral("", 0);
			case IRTPointer(IRTInstance(_), true): IRCNull;
			case IRTNullable(_, IRNTagged): IRCNull;
			case _: unsupported(expression, '$owner(result-type-without-direct-default)');
		};
	}

	function defaultConstantAt(type:HxcIRTypeRef, position:Position, owner:String):HxcIRConstant {
		return switch type {
			case IRTBool: IRCBool(false);
			case IRTInt(_, _): IRCInt("0");
			case IRTFloat(32) | IRTFloat(64): IRCFloat("0.0");
			case IRTString: IRCString("", 0);
			case IRTCString: IRCCStringLiteral("", 0);
			case IRTPointer(IRTInstance(_), true): IRCNull;
			case IRTNullable(_, IRNTagged): IRCNull;
			case _: unsupportedAt(position, '$owner(result-type-without-direct-default)');
		};
	}

	function coerce(value:LoweredValue, target:CBodyValueType, position:Position, node:String):LoweredValue {
		if (typeKey(value.mapping.irType) == typeKey(target.irType)) {
			// The carrier is already correct, but retain the contextual Haxe identity
			// (for example LogicalPath rather than plain String) for later diagnostics.
			return {id: value.id, type: value.type, mapping: target};
		}
		final targetOptional = target.optionalValue();
		final sourceOptional = value.mapping.optionalValue();
		if (targetOptional != null && sourceOptional == null) {
			// Haxe may type a literal through the payload's ordinary conversion
			// before it enters `Null<T>`. For example, `optionalUInt(0)` arrives
			// here as Int -> Null<UInt>. Reuse the normal payload coercion first;
			// injecting the Int bits directly would give the wrapper the wrong C
			// representation and bypass UInt's range policy.
			final convertedPayload = coerce(value, targetOptional.payload, position, '$node:optional-payload');
			final payload = targetOptional.managedLifetime ? captureManagedValue(convertedPayload, targetOptional.payload, position,
				"optional-payload") : convertedPayload;
			final injected:HxcIRResult = {id: nextValueId(), type: target.irType};
			appendInstruction(injected, IRIOConvert(payload.id, IRCNullableInject, target.irType, IRIStatic, null),
				HaxeSourceSpan.fromPosition(position, input.sourcePath), "direct-optional-inject");
			registerValueTemporary(injected.id, "direct-optional-value");
			if (targetOptional.managedLifetime)
				freshManagedOptionalValueIds.set(injected.id, true);
			return {id: injected.id, type: injected.type, mapping: target};
		}
		if (sourceOptional != null && targetOptional == null) {
			// A tagged optional stores a payload even while it is absent, but that
			// storage is not a usable Haxe value. Prove presence before exposing
			// the payload, then let the ordinary coercion rules handle any
			// following conversion. The null-check coalescer may remove a repeated
			// check only when control-flow dominance proves an earlier check covers
			// this exact value.
			final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
			appendInstruction(null, IRIONullCheck(value.id, IRNCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))), source,
				"direct-optional-null-check");
			final unwrapped:HxcIRResult = {id: nextValueId(), type: sourceOptional.payload.irType};
			appendInstruction(unwrapped, IRIOConvert(value.id, IRCNullableUnwrap, sourceOptional.payload.irType, IRIStatic, null), source,
				"direct-optional-unwrap");
			registerValueTemporary(unwrapped.id, "direct-optional-payload");
			return coerce({id: unwrapped.id, type: unwrapped.type, mapping: sourceOptional.payload}, target, position, '$node:unwrapped-optional');
		}
		final sourceClass = value.mapping.classValue();
		final targetClass = target.classValue();
		final sourceInterface = value.mapping.interfaceValue();
		final targetInterface = target.interfaceValue();
		if (sourceInterface != null || targetInterface != null) {
			if (sourceInterface != null)
				return unsupportedAt(position, '$node:interface-cast-requires-runtime-type-proof:${value.mapping.cSpelling}->${target.cSpelling}');
			if (sourceClass == null || targetInterface == null)
				return unsupportedAt(position, '$node:interface-reference-category-mismatch:${value.mapping.cSpelling}->${target.cSpelling}');
			final table = dispatch.tableForInterface(sourceClass.instanceId, targetInterface.instanceId);
			if (table == null)
				return unsupportedAt(position, '$node:class-does-not-have-reachable-interface-table:${sourceClass.haxePath}->${targetInterface.haxePath}');
			final sourceNullable = value.mapping.classNullable();
			if (sourceNullable == null)
				return unsupportedAt(position, '$node:interface-source-class-nullability-missing');
			if (sourceNullable)
				appendInstruction(null, IRIONullCheck(value.id, IRNCPCheckedAbort(Std.string(context.profile), Std.string(context.buildMode))),
					HaxeSourceSpan.fromPosition(position, input.sourcePath), "interface-construction-null-check");
			final result:HxcIRResult = {id: nextValueId(), type: target.irType};
			appendInstruction(result, IRIOConstructInterface(targetInterface.instanceId, value.id, table.input.id),
				HaxeSourceSpan.fromPosition(position, input.sourcePath), "interface-construction");
			registerValueTemporary(result.id, "interface-value");
			carryOwnedClassBorrow(value.id, result.id);
			return {id: result.id, type: result.type, mapping: target};
		}
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
				carryOwnedClassBorrow(converted.id, upcast.id);
				converted = {id: upcast.id, type: upcast.type, mapping: upcastTarget};
			}
			if (!sourceNullable && targetNullable) {
				final injected:HxcIRResult = {id: nextValueId(), type: target.irType};
				appendInstruction(injected, IRIOConvert(converted.id, IRCNullableInject, target.irType, IRIStatic, null),
					HaxeSourceSpan.fromPosition(position, input.sourcePath), "class-nullable-inject");
				carryOwnedClassBorrow(converted.id, injected.id);
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

	/**
		Read the primitive semantics carried by a direct value or `Null<T>`.

		This helper only selects the operation's type. The later `coerce` call
		still emits a checked unwrap before the operation can read a nullable
		payload, so type planning cannot accidentally turn missing into zero.
	**/
	function requirePrimitiveOperand(type:CBodyValueType, position:Position, node:String):CPrimitiveTypeMapping {
		final optional = type.optionalValue();
		return requirePrimitive(optional == null ? type : optional.payload, position, node);
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

	/** True when an expression to the right can move lowering into another block. */
	function laterExpressionCreatesFlow(expressions:Array<TypedExpr>, index:Int):Bool {
		var candidate = index + 1;
		while (candidate < expressions.length) {
			if (expressionCreatesFlow(expressions[candidate]))
				return true;
			candidate++;
		}
		return false;
	}

	/** True when a source-ordered record field to the right can create a join. */
	function laterAggregateFieldCreatesFlow(fields:Array<{name:String, expr:TypedExpr}>, index:Int):Bool {
		var candidate = index + 1;
		while (candidate < fields.length) {
			if (expressionCreatesFlow(fields[candidate].expr))
				return true;
			candidate++;
		}
		return false;
	}

	/**
	 * Save a completed value before a later expression introduces control flow.
	 *
	 * Haxe evaluates calls and aggregate fields from left to right. HxcIR values belong to
	 * one basic block, so a value computed before a conditional cannot be named
	 * directly in the conditional's join block. An automatic local preserves the
	 * value and its order. The caller reloads it only after every later expression
	 * has completed.
	 */
	function stageFlowValue(value:LoweredValue, expression:TypedExpr, crossesFlow:Bool, role:String):StagedFlowValue {
		final localId = crossesFlow ? createFlowLocal(value.mapping, value.id, HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), role) : null;
		return {value: value, localId: localId, position: expression.pos};
	}

	/** Reload one saved value in the final block, or reuse its still-local result. */
	function restoreStagedValue(value:StagedFlowValue, role:String):String {
		if (value.localId == null)
			return value.value.id;
		return loadPlace({place: IRPLocal(value.localId), mapping: value.value.mapping, mutable: true}, value.position, role).id;
	}

	/** Reload staged arguments in the final block without changing source effects. */
	function restoreCallArguments(arguments:Array<StagedFlowValue>, role:String):Array<String> {
		final restored:Array<String> = [];
		for (index => argument in arguments)
			restored.push(restoreStagedValue(argument, '$role-$index-load'));
		return restored;
	}

	/**
	 * Save an assignment destination before its right side creates control flow.
	 *
	 * Haxe locates `receiver.field` or `span[index]` before evaluating the
	 * assigned value. A conditional right side moves lowering into new HxcIR
	 * blocks, where value IDs used by that place are no longer available. After
	 * its null and bounds checks have succeeded, the element address is the
	 * smallest complete representation of that already-evaluated destination.
	 * Saving that address in an automatic local avoids both re-evaluating user
	 * code and repeating a runtime check after the branches join.
	 */
	function stageFlowPlace(target:LoweredPlace, position:Position, crossesFlow:Bool, role:String):StagedFlowPlace {
		if (!crossesFlow || !placeUsesBlockValue(target.place))
			return {target: target, addressLocalId: null, position: position};
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		final address:HxcIRResult = {
			id: nextValueId(),
			type: IRTPointer(target.mapping.irType, false)
		};
		appendInstruction(address, IRIOAddress(target.place), source, role + "-address");
		registerValueTemporary(address.id, role + "-address");
		final ordinal = localOrdinal++;
		final localId = 'local.$ordinal';
		locals.push({
			id: localId,
			type: address.type,
			storage: IRLSAutomatic,
			initialState: IRISUninitialized,
			source: source
		});
		final request = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, role, "address"]),
			CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
		context.symbols.register(request);
		localRequests.set(localId, request);
		appendInstruction(null, IRIOInitialize(IRPLocal(localId), address.id, IRISUninitialized, IRISInitialized), source, role + "-initialize");
		return {target: target, addressLocalId: localId, position: position};
	}

	/**
	 * Reload a staged destination address in the current block.
	 *
	 * The dereference is valid because `stageFlowPlace` took the address only
	 * after HxcIR had validated the original place's null and bounds evidence.
	 * The pointer remains local to this function and cannot escape.
	 */
	function restoreStagedPlace(staged:StagedFlowPlace, role:String):LoweredPlace {
		if (staged.addressLocalId == null)
			return staged.target;
		final pointer:HxcIRResult = {
			id: nextValueId(),
			type: IRTPointer(staged.target.mapping.irType, false)
		};
		appendInstruction(pointer, IRIOLoad(IRPLocal(staged.addressLocalId)), HaxeSourceSpan.fromPosition(staged.position, input.sourcePath),
			role + "-address-load");
		registerValueTemporary(pointer.id, role + "-address-load");
		return {
			place: IRPDereference(pointer.id),
			mapping: staged.target.mapping,
			mutable: staged.target.mutable
		};
	}

	/** True when a place contains an HxcIR value that belongs to one basic block. */
	function placeUsesBlockValue(place:HxcIRPlace):Bool {
		return switch place {
			case IRPLocal(_) | IRPGlobal(_): false;
			case IRPDereference(_): true;
			case IRPField(base, _): placeUsesBlockValue(base);
			case IRPIndex(_, _): true;
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

	/** A switch-pattern binding borrows storage from its still-live enum value. */
	static function isEnumPayloadProjection(expression:TypedExpr):Bool
		return switch unwrapExpression(expression).expr {
			case TEnumParameter(_, _, _): true;
			case _: false;
		};

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

	public static function typeKey(type:HxcIRTypeRef):String {
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

package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CProfile;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRDiagnostic;
import reflaxe.c.ir.HxcIRValidator;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
import reflaxe.c.naming.CSymbolRequest;
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

/** One validated body with finalized C names and both source-mapping modes. */
class CLoweredBodyFunction {
	public final modulePath:String;
	public final declarationPath:String;
	public final fieldName:String;
	public final ir:HxcIRFunction;
	public final cName:CIdentifier;
	public final parameterNames:Map<String, CIdentifier>;
	public final localNames:Map<String, CIdentifier>;
	public final temporaryNames:Map<String, CIdentifier>;
	public final tailArgumentNames:Map<String, Array<CIdentifier>>;
	public final labelNames:Map<String, CIdentifier>;
	public final requiredHeaders:Array<String>;
	public final body:CStmt;
	public final lineMappedBody:CStmt;

	public function new(modulePath:String, declarationPath:String, fieldName:String, ir:HxcIRFunction, cName:CIdentifier,
			parameterNames:Map<String, CIdentifier>, localNames:Map<String, CIdentifier>, temporaryNames:Map<String, CIdentifier>,
			tailArgumentNames:Map<String, Array<CIdentifier>>, labelNames:Map<String, CIdentifier>, requiredHeaders:Array<String>, body:CStmt,
			lineMappedBody:CStmt) {
		this.modulePath = modulePath;
		this.declarationPath = declarationPath;
		this.fieldName = fieldName;
		this.ir = ir;
		this.cName = cName;
		this.parameterNames = parameterNames;
		this.localNames = localNames;
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

/** Complete deterministic result for the admitted body subset. */
class CBodyLoweringResult {
	public final program:HxcIRProgram;
	public final functions:Array<CLoweredBodyFunction>;
	public final globals:Array<CLoweredBodyGlobal>;
	public final symbolTable:CSymbolTableSnapshot;

	public function new(program:HxcIRProgram, functions:Array<CLoweredBodyFunction>, globals:Array<CLoweredBodyGlobal>, symbolTable:CSymbolTableSnapshot) {
		this.program = program;
		this.functions = functions.copy();
		this.globals = globals.copy();
		this.symbolTable = symbolTable;
	}
}

/** Typed Haxe body -> HxcIR -> validated structural C body pipeline. */
class CBodyLowering {
	final context:CompilationContext;

	public function new(context:CompilationContext) {
		this.context = context;
	}

	public function lower(inputFunctions:Array<CBodyFunctionInput>, ?inputGlobals:Array<CBodyGlobalInput>):CBodyLoweringResult {
		if (inputFunctions.length == 0) {
			throw new CBodyEmissionError("body lowering requires at least one typed function input");
		}
		final inputs = inputFunctions.copy();
		inputs.sort(compareInputs);
		final prepared:Array<PreparedBodyFunction> = [];
		final preparedById:Map<String, PreparedBodyFunction> = [];
		for (input in inputs) {
			final fn = new FunctionPreparer(context, input).prepare();
			if (preparedById.exists(fn.irId)) {
				throw new CBodyEmissionError('body lowering received duplicate semantic function `${fn.irId}`');
			}
			prepared.push(fn);
			preparedById.set(fn.irId, fn);
		}
		final globalRegistry = new BodyGlobalRegistry(context, inputGlobals == null ? [] : inputGlobals);
		final built:Array<BuiltBodyFunction> = [];
		for (fn in prepared) {
			built.push(new FunctionBuilder(context, fn, preparedById, globalRegistry).build());
		}
		final preparedGlobals = globalRegistry.canonicalGlobals();
		final program = buildProgram(built, preparedGlobals);
		new HxcIRValidator().requireValid(program, Std.string(context.profile));
		final symbolTable = context.symbols.finalizeSymbols();
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
		final emitter = new CBodyEmitter();
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
			final input = item.prepared.input;
			lowered.push(new CLoweredBodyFunction(input.modulePath, input.declarationPath, input.fieldName, item.ir,
				context.symbols.identifierFor(item.prepared.functionRequest), parameterNames, localNames, temporaryNames, tailArgumentNames, labelNames,
				emitter.requiredHeaders(item.ir),
				emitter.emitBody(item.ir, parameterNames, localNames, temporaryNames, functionNames, globalNames, false, tailArgumentNames, labelNames),
				emitter.emitBody(item.ir, parameterNames, localNames, temporaryNames, functionNames, globalNames, true, tailArgumentNames, labelNames)));
		}
		lowered.sort((left, right) -> compareUtf8(left.ir.id, right.ir.id));
		return new CBodyLoweringResult(program, lowered, loweredGlobals, symbolTable);
	}

	static function buildProgram(functions:Array<BuiltBodyFunction>, globals:Array<PreparedBodyGlobal>):HxcIRProgram {
		final byModule:Map<String, Array<BuiltBodyFunction>> = [];
		for (fn in functions) {
			var moduleFunctions = byModule.get(fn.prepared.input.modulePath);
			if (moduleFunctions == null) {
				moduleFunctions = [];
				byModule.set(fn.prepared.input.modulePath, moduleFunctions);
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
		final moduleIdSet:Map<String, Bool> = [];
		for (moduleId in byModule.keys()) {
			moduleIdSet.set(moduleId, true);
		}
		for (moduleId in globalsByModule.keys()) {
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
			final spans = moduleFunctions.map(entry -> entry.ir.source).concat(moduleGlobals.map(global -> global.ir.source));
			if (spans.length == 0) {
				throw new CBodyEmissionError('body lowering lost module `$moduleId` while building HxcIR');
			}
			modules.push({
				id: moduleId,
				types: [],
				typeInstances: [],
				globals: moduleGlobals.map(global -> global.ir),
				functions: moduleFunctions.map(entry -> entry.ir),
				source: enclosingSpan(spans)
			});
		}
		return {schemaVersion: 2, modules: modules};
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
		final identity = compareUtf8('${left.modulePath}\x00${left.declarationPath}\x00${left.fieldName}',
			'${right.modulePath}\x00${right.declarationPath}\x00${right.fieldName}');
		return identity != 0 ? identity : left.sourceOrder - right.sourceOrder;
	}

	public static function functionId(declarationPath:String, fieldName:String):String
		return 'function.$declarationPath.$fieldName';

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
	final temporaryRequests:Map<String, CSymbolRequest>;
	final tailArgumentRequests:Map<String, Array<CSymbolRequest>>;
	final labelRequests:Map<String, CSymbolRequest>;
}

private typedef LoweredValue = {
	final id:String;
	final type:HxcIRTypeRef;
	final mapping:CPrimitiveTypeMapping;
}

private typedef LoweredPlace = {
	final place:HxcIRPlace;
	final mapping:CPrimitiveTypeMapping;
	final mutable:Bool;
}

private typedef MutableBodyBlock = {
	final id:String;
	final source:HxcSourceSpan;
	final instructions:Array<HxcIRInstruction>;
	var terminator:Null<HxcIRTerminator>;
}

private typedef PreparedParameter = {
	final compilerId:Int;
	final ir:HxcIRParameter;
	final mapping:CPrimitiveTypeMapping;
}

private typedef PreparedBodyFunction = {
	final input:CBodyFunctionInput;
	final functionValue:TFunc;
	final irId:String;
	final parameters:Array<PreparedParameter>;
	final returnMapping:CPrimitiveTypeMapping;
	final functionRequest:CSymbolRequest;
	final parameterRequests:Map<String, CSymbolRequest>;
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

	public function new(context:CompilationContext, inputs:Array<CBodyGlobalInput>) {
		this.context = context;
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
		final existing = byId.get(id);
		if (existing != null) {
			return existing;
		}
		final input = inputsById.get(id);
		if (input == null) {
			return rejected(fail, expression.pos, 'TField(static:${field.name}:outside-captured-program)');
		}
		final mapping = switch CPrimitiveTypeMapper.map(input.fieldType, context.profile) {
			case CTPrimitive(value):
				final admitted = value.nullability == CPNonNullable && switch value.sourceType {
					case CPHaxeBool | CPHaxeInt | CPHaxeUInt | CPHaxeFloat: true;
					case _: false;
				};
				if (!admitted) {
					return rejected(fail, expression.pos, 'TField(static:${field.name}:type:${value.cSpelling})');
				}
				value;
			case CTReference(identity, nullable):
				return rejected(fail, expression.pos, 'TField(static:${field.name}:reference-$identity-${nullable ? "nullable" : "non-null"})');
			case CTNativePointer(identity, nullable):
				return rejected(fail, expression.pos, 'TField(static:${field.name}:native-pointer-$identity-${nullable ? "nullable" : "non-null"})');
			case CTUnsupported(reason):
				return rejected(fail, expression.pos, 'TField(static:${field.name}:$reason)');
		};
		final initializerExpression = input.expression;
		final initializer = if (initializerExpression == null) {
			return rejected(fail, input.position, 'TField(static:${field.name}:uninitialized)');
		} else globalInitializer(initializerExpression, mapping.irType, fail, field.name);
		final request = new CSymbolRequest(CSKField, declarationPath.split(".").concat([field.name]), CNSOrdinary("translation-unit"), CSVInternal);
		context.symbols.register(request);
		final prepared:PreparedBodyGlobal = {
			modulePath: input.modulePath,
			declarationPath: declarationPath,
			fieldName: field.name,
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
		return 'global.$declarationPath.$fieldName';

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
	final functionContext:String;

	public function new(context:CompilationContext, input:CBodyFunctionInput) {
		this.context = context;
		this.input = input;
		this.functionContext = 'function ${input.declarationPath}.${input.fieldName} signature';
	}

	public function prepare():PreparedBodyFunction {
		final functionValue = switch input.expression.expr {
			case TFunction(value): value;
			case _: unsupported(input.expression.pos, FunctionBuilder.nodeName(input.expression));
		};
		final declaredSignature = switch TypeTools.follow(input.fieldType) {
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
			final mapping = admittedPrimitive(argument.v.t, input.expression.pos, 'TFunction(argument:${argument.v.name})');
			if (mapping.irType == IRTVoid) {
				unsupported(input.expression.pos, 'TFunction(argument:${argument.v.name}:Void)');
			}
			final parameterId = 'parameter.$index';
			final source = HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath);
			parameters.push({
				compilerId: argument.v.id,
				ir: {id: parameterId, type: mapping.irType, source: source},
				mapping: mapping
			});
		}
		final returnMapping = admittedPrimitive(declaredSignature.result, input.expression.pos, "TFunction(return-type)");
		final overloadSignature = parameters.length == 0 ? [] : parameters.map(parameter -> primitiveTypeKey(parameter.ir.type));
		final functionRequest = new CSymbolRequest(CSKMethod, input.declarationPath.split(".").concat([input.fieldName]), CNSOrdinary("translation-unit"),
			CSVInternal, null, overloadSignature);
		context.symbols.register(functionRequest);
		for (index in 0...parameters.length) {
			final parameter = parameters[index];
			final argument = functionValue.args[index];
			final request = new CSymbolRequest(CSKLocal, input.declarationPath.split(".").concat([input.fieldName, argument.v.name]),
				CNSOrdinary(functionRequest.stableKey()), CSVInternal, null, [], [], index);
			context.symbols.register(request);
			parameterRequests.set(parameter.ir.id, request);
		}
		return {
			input: input,
			functionValue: functionValue,
			irId: CBodyLowering.functionId(input.declarationPath, input.fieldName),
			parameters: parameters,
			returnMapping: returnMapping,
			functionRequest: functionRequest,
			parameterRequests: parameterRequests
		};
	}

	function admittedPrimitive(type:Type, position:Position, node:String):CPrimitiveTypeMapping {
		return switch CPrimitiveTypeMapper.map(type, context.profile) {
			case CTPrimitive(mapping):
				final admitted = mapping.nullability == CPNonNullable && switch mapping.sourceType {
					case CPHaxeVoid | CPHaxeBool | CPHaxeInt | CPHaxeUInt | CPHaxeFloat: true;
					case _: false;
				};
				if (!admitted) {
					unsupported(position, '$node:${mapping.cSpelling}');
				}
				mapping;
			case CTReference(identity, nullable):
				unsupported(position, '$node:reference-$identity-${nullable ? "nullable" : "non-null"}');
			case CTNativePointer(identity, nullable):
				unsupported(position, '$node:native-pointer-$identity-${nullable ? "nullable" : "non-null"}');
			case CTUnsupported(reason):
				unsupported(position, '$node:$reason');
		};
	}

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

	static function primitiveTypeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTFloat(width): 'f$width';
			case IRTVoid: "void";
			case _: throw new CBodyEmissionError("function signature contains a non-primitive admitted type");
		};
	}
}

private class FunctionBuilder {
	final context:CompilationContext;
	final prepared:PreparedBodyFunction;
	final input:CBodyFunctionInput;
	final functionsById:Map<String, PreparedBodyFunction>;
	final globalRegistry:BodyGlobalRegistry;
	final functionContext:String;
	final parameterValuesByCompilerId:Map<Int, LoweredValue> = [];
	final localIdsByCompilerId:Map<Int, String> = [];
	final localRequests:Map<String, CSymbolRequest> = [];
	final temporaryRequests:Map<String, CSymbolRequest> = [];
	final tailArgumentRequests:Map<String, Array<CSymbolRequest>> = [];
	final labelRequests:Map<String, CSymbolRequest> = [];
	final locals:Array<HxcIRLocal> = [];
	final blocks:Array<MutableBodyBlock> = [];
	var localOrdinal = 0;
	var temporaryOrdinal = 0;
	var instructionOrdinal = 0;
	var valueOrdinal = 0;
	var blockOrdinal = 0;
	var currentBlock:MutableBodyBlock;

	public function new(context:CompilationContext, prepared:PreparedBodyFunction, functionsById:Map<String, PreparedBodyFunction>,
			globalRegistry:BodyGlobalRegistry) {
		this.context = context;
		this.prepared = prepared;
		this.input = prepared.input;
		this.functionsById = functionsById;
		this.globalRegistry = globalRegistry;
		this.functionContext = 'function ${input.declarationPath}.${input.fieldName} body';
		this.localOrdinal = prepared.parameters.length;
		this.currentBlock = createBlock("entry", HaxeSourceSpan.fromPosition(prepared.functionValue.expr.pos, input.sourcePath), false);
		for (parameter in prepared.parameters) {
			parameterValuesByCompilerId.set(parameter.compilerId, {id: parameter.ir.id, type: parameter.ir.type, mapping: parameter.mapping});
		}
	}

	public function build():BuiltBodyFunction {
		final functionValue = prepared.functionValue;
		lowerStatement(functionValue.expr);
		if (currentBlock.terminator == null) {
			currentBlock.terminator = {kind: IRTReturn(null, []), source: HaxeSourceSpan.fromPosition(functionValue.expr.pos, input.sourcePath)};
		}
		final functionSpan = HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath);
		final ir:HxcIRFunction = {
			id: prepared.irId,
			displayName: '${input.declarationPath}.${input.fieldName}',
			parameters: prepared.parameters.map(parameter -> parameter.ir),
			locals: locals,
			returnType: prepared.returnMapping.irType,
			entryBlockId: "entry",
			blocks: blocks.map(block -> {
				id: block.id,
				parameters: [],
				instructions: block.instructions,
				terminator: block.terminator,
				source: block.source
			}),
			cleanupRegions: [],
			source: functionSpan
		};
		return {
			prepared: prepared,
			ir: ir,
			localRequests: localRequests,
			temporaryRequests: temporaryRequests,
			tailArgumentRequests: tailArgumentRequests,
			labelRequests: labelRequests
		};
	}

	function lowerStatement(expression:TypedExpr):Void {
		if (currentBlock.terminator != null) {
			unsupported(expression, 'unreachable ${nodeName(expression)}');
		}
		switch expression.expr {
			case TBlock(expressions):
				for (nested in expressions) {
					lowerStatement(nested);
				}
			case TVar(variable, initializer):
				lowerVariable(variable, initializer, expression.pos);
			case TReturn(value):
				lowerReturn(value, expression.pos);
			case TParenthesis(inner):
				lowerStatement(inner);
			case TConst(_) | TLocal(_) | TField(_, _) | TCast(_, _) | TBinop(_, _, _) | TUnop(_, _, _):
				lowerValue(expression);
			case TCall(_, _):
				lowerCall(expression, false);
			case TIf(_, _, _):
				// Statement control flow belongs to E2.T06. A value-form ternary is
				// admitted only when it reaches `lowerValue` in a value context.
				unsupported(expression, "TIf");
			case _:
				unsupported(expression, nodeName(expression));
		}
	}

	function lowerVariable(variable:TVar, initializer:Null<TypedExpr>, position:Position):Void {
		final ordinal = localOrdinal++;
		final localId = 'local.$ordinal';
		final localMapping = primitiveMapping(variable.t, position, 'TVar(${variable.name}:type)');
		if (localMapping.irType == IRTVoid) {
			unsupportedAt(position, 'TVar(${variable.name}:Void)');
		}
		final initialExpression:TypedExpr = switch initializer {
			case null: unsupportedAt(position, 'TVar(${variable.name}:uninitialized)');
			case value: value;
		};
		final value = lowerValue(initialExpression, localMapping);
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
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
	}

	function lowerReturn(value:Null<TypedExpr>, position:Position):Void {
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		if (value == null) {
			currentBlock.terminator = {kind: IRTReturn(null, []), source: source};
			return;
		}
		if (prepared.returnMapping.irType == IRTVoid) {
			switch value.expr {
				case TCall(_, _):
					lowerCall(value, false);
				case _:
					unsupported(value, "TReturn(value-for-Void)");
			}
			currentBlock.terminator = {kind: IRTReturn(null, []), source: source};
			return;
		}
		final lowered = lowerValue(value, prepared.returnMapping);
		currentBlock.terminator = {kind: IRTReturn(lowered.id, []), source: source};
	}

	function lowerValue(expression:TypedExpr, ?expectedMapping:CPrimitiveTypeMapping):LoweredValue {
		return switch expression.expr {
			case TConst(constant): lowerConstant(expression, constant, expectedMapping);
			case TLocal(variable): lowerLocal(expression, variable);
			case TField(_, FStatic(classReference, fieldReference)):
				lowerStaticField(expression, classReference, fieldReference);
			case TParenthesis(inner): lowerValue(inner, expectedMapping);
			case TMeta(_, inner): lowerValue(inner, expectedMapping);
			case TBlock(expressions): lowerValueBlock(expression, expressions, expectedMapping);
			case TCast(inner, _):
				final source = lowerValue(inner);
				coerce(source, primitiveMapping(expression.t, expression.pos, "TCast(target-type)"), expression.pos, "TCast");
			case TCall(_, _):
				final result = lowerCall(expression, true);
				if (result == null) {
					unsupported(expression, "TCall(Void-used-as-value)");
				}
				result;
			case TBinop(OpAssign, left, right): lowerAssignment(expression, left, right);
			case TBinop(OpBoolAnd, left, right): lowerLazyBoolean(expression, left, right, false);
			case TBinop(OpBoolOr, left, right): lowerLazyBoolean(expression, left, right, true);
			case TUnop(OpIncrement, postFix, target): lowerIncrement(expression, target, postFix);
			case TIf(condition, whenTrue, whenFalse): lowerConditional(expression, condition, whenTrue, whenFalse, expectedMapping);
			case _: unsupported(expression, nodeName(expression));
		};
	}

	function lowerValueBlock(expression:TypedExpr, expressions:Array<TypedExpr>, expectedMapping:Null<CPrimitiveTypeMapping>):LoweredValue {
		if (expressions.length == 0) {
			return unsupported(expression, "TBlock(empty-as-value)");
		}
		final lastIndex = expressions.length - 1;
		for (index in 0...lastIndex) {
			lowerStatement(expressions[index]);
		}
		return lowerValue(expressions[lastIndex], expectedMapping);
	}

	function lowerConstant(expression:TypedExpr, constant:TConstant, expectedMapping:Null<CPrimitiveTypeMapping>):LoweredValue {
		final inferredMapping = primitiveMapping(expression.t, expression.pos, nodeName(expression));
		final mapping = contextualConstantMapping(constant, inferredMapping, expectedMapping);
		final type = mapping.irType;
		final value:HxcIRConstant = switch constant {
			case TInt(value):
				requireConstantType(type, expression, "integer", valueType -> switch valueType {
					case IRTInt(_, _): true;
					case _: false;
				});
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
		return {id: result.id, type: result.type, mapping: mapping};
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
		final localId = localIdsByCompilerId.get(variable.id);
		if (localId == null) {
			return unsupported(expression, 'TLocal(${variable.name}:outside-admitted-body)');
		}
		final mapping = primitiveMapping(expression.t, expression.pos, 'TLocal(${variable.name}:type)');
		final result:HxcIRResult = {id: nextValueId(), type: mapping.irType};
		appendInstruction(result, IRIOLoad(IRPLocal(localId)), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "load");
		registerValueTemporary(result.id, "load-result");
		return {id: result.id, type: result.type, mapping: mapping};
	}

	function lowerStaticField(expression:TypedExpr, classReference:Ref<ClassType>, fieldReference:Ref<ClassField>):LoweredValue {
		final global = globalRegistry.require(classReference, fieldReference, expression, rejectGlobal);
		return loadPlace({place: IRPGlobal(global.ir.id), mapping: global.mapping, mutable: global.ir.mutable}, expression.pos, "global-load");
	}

	function lowerAssignment(expression:TypedExpr, left:TypedExpr, right:TypedExpr):LoweredValue {
		final target = lowerPlace(left);
		if (!target.mutable) {
			unsupported(left, "TBinop(OpAssign:immutable-place)");
		}
		final source = lowerValue(right, target.mapping);
		final value = coerce(source, target.mapping, right.pos, "TBinop(OpAssign:right)");
		appendInstruction(null, IRIOStore(target.place, value.id), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "store");
		return value;
	}

	function lowerIncrement(expression:TypedExpr, targetExpression:TypedExpr, postFix:Bool):LoweredValue {
		final target = lowerPlace(targetExpression);
		if (!target.mutable) {
			unsupported(targetExpression, "TUnop(OpIncrement:immutable-place)");
		}
		switch target.mapping.irType {
			case IRTInt(32, false):
			case _:
				unsupported(expression, "TUnop(OpIncrement:requires-E2.T05-for-non-UInt)");
		}
		final oldValue = loadPlace(target, targetExpression.pos, "increment-load");
		final oneResult:HxcIRResult = {id: nextValueId(), type: target.mapping.irType};
		appendInstruction(oneResult, IRIOConstant(IRCInt("1")), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "increment-one");
		final nextResult:HxcIRResult = {id: nextValueId(), type: target.mapping.irType};
		appendInstruction(nextResult, IRIOBinary("haxe.u32.add", oldValue.id, oneResult.id, IRIStatic),
			HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "increment");
		appendInstruction(null, IRIOStore(target.place, nextResult.id), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "increment-store");
		final nextValue:LoweredValue = {id: nextResult.id, type: nextResult.type, mapping: target.mapping};
		return postFix ? oldValue : nextValue;
	}

	function lowerLazyBoolean(expression:TypedExpr, left:TypedExpr, right:TypedExpr, shortCircuitWhenTrue:Bool):LoweredValue {
		final boolMapping = primitiveMapping(expression.t, expression.pos, "TBinop(short-circuit:result-type)");
		if (boolMapping.irType != IRTBool) {
			unsupported(expression, "TBinop(short-circuit:non-Bool-result)");
		}
		final leftValue = coerce(lowerValue(left), boolMapping, left.pos, "TBinop(short-circuit:left)");
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final resultLocalId = createFlowLocal(boolMapping, leftValue.id, source, "short-circuit-result");
		final rhsBlock = createGeneratedBlock("short-circuit-rhs", source);
		final joinBlock = createGeneratedBlock("short-circuit-join", source);
		currentBlock.terminator = {
			kind: IRTBranch(leftValue.id, edge(shortCircuitWhenTrue ? joinBlock.id : rhsBlock.id), edge(shortCircuitWhenTrue ? rhsBlock.id : joinBlock.id)),
			source: source
		};
		currentBlock = rhsBlock;
		final rightValue = coerce(lowerValue(right), boolMapping, right.pos, "TBinop(short-circuit:right)");
		appendInstruction(null, IRIOStore(IRPLocal(resultLocalId), rightValue.id), source, "short-circuit-store");
		currentBlock.terminator = {kind: IRTJump(edge(joinBlock.id)), source: source};
		currentBlock = joinBlock;
		return loadPlace({place: IRPLocal(resultLocalId), mapping: boolMapping, mutable: true}, expression.pos, "short-circuit-load");
	}

	function lowerConditional(expression:TypedExpr, condition:TypedExpr, whenTrue:TypedExpr, whenFalse:Null<TypedExpr>,
			expectedMapping:Null<CPrimitiveTypeMapping>):LoweredValue {
		final falseExpression = whenFalse;
		if (falseExpression == null) {
			return unsupported(expression, "TIf(without-else-as-value)");
		}
		final boolMapping = primitiveMapping(condition.t, condition.pos, "TIf(condition-type)");
		if (boolMapping.irType != IRTBool) {
			unsupported(condition, "TIf(non-Bool-condition)");
		}
		final conditionValue = coerce(lowerValue(condition), boolMapping, condition.pos, "TIf(condition)");
		final resultMapping = expectedMapping == null ? primitiveMapping(expression.t, expression.pos, "TIf(result-type)") : expectedMapping;
		if (resultMapping.irType == IRTVoid) {
			return unsupported(expression,
				'TIf(Void-as-value:${expectedMapping == null ? "typed-expression" : "contextual"}:function-return=${prepared.returnMapping.cSpelling})');
		}
		final source = HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath);
		final defaultResult:HxcIRResult = {id: nextValueId(), type: resultMapping.irType};
		appendInstruction(defaultResult, IRIOConstant(defaultConstant(resultMapping.irType, expression)), source, "conditional-default");
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
			case TLocal(variable):
				if (parameterValuesByCompilerId.exists(variable.id)) {
					unsupported(expression, 'TLocal(${variable.name}:parameter-assignment-not-yet-lowered)');
				}
				final localId = localIdsByCompilerId.get(variable.id);
				if (localId == null) {
					unsupported(expression, 'TLocal(${variable.name}:outside-admitted-body)');
				}
				{place: IRPLocal(localId), mapping: primitiveMapping(variable.t, expression.pos, 'TLocal(${variable.name}:place-type)'), mutable: true};
			case TField(_, FStatic(classReference, fieldReference)):
				final global = globalRegistry.require(classReference, fieldReference, expression, rejectGlobal);
				{place: IRPGlobal(global.ir.id), mapping: global.mapping, mutable: global.ir.mutable};
			case TParenthesis(inner) | TMeta(_, inner): lowerPlace(inner);
			case _: unsupported(expression, 'place(${nodeName(expression)})');
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
		final targetId = directStaticFunctionId(call.callee);
		final target = functionsById.get(targetId);
		if (target == null) {
			return unsupported(expression, 'TCall(unavailable-static-target:$targetId)');
		}
		if (call.arguments.length != target.parameters.length) {
			return unsupported(expression, 'TCall(argument-count=${call.arguments.length},expected=${target.parameters.length},target=$targetId)');
		}
		final arguments:Array<String> = [];
		for (index in 0...call.arguments.length) {
			final argumentExpression = call.arguments[index];
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
			context.symbols.register(request);
			temporaryRequests.set(result.id, request);
		}
		return {id: result.id, type: result.type, mapping: target.returnMapping};
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
		context.symbols.register(request);
		temporaryRequests.set(valueId, request);
	}

	function createFlowLocal(mapping:CPrimitiveTypeMapping, initialValueId:String, source:HxcSourceSpan, role:String):String {
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

	function createGeneratedBlock(role:String, source:HxcSourceSpan):MutableBodyBlock {
		final ordinal = blockOrdinal++;
		final block = createBlock('block.$ordinal.$role', source, true);
		final request = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, "block-label", role]),
			CNSLabel(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
		context.symbols.register(request);
		labelRequests.set(block.id, request);
		return block;
	}

	function createBlock(id:String, source:HxcSourceSpan, generated:Bool):MutableBodyBlock {
		if ((generated && id == "entry") || (!generated && id != "entry")) {
			throw new CBodyEmissionError('invalid body block identity `$id` in `${prepared.irId}`');
		}
		final block:MutableBodyBlock = {
			id: id,
			source: source,
			instructions: [],
			terminator: null
		};
		blocks.push(block);
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

	function defaultConstant(type:HxcIRTypeRef, expression:TypedExpr):HxcIRConstant {
		return switch type {
			case IRTBool: IRCBool(false);
			case IRTInt(_, _): IRCInt("0");
			case IRTFloat(64): IRCFloat("0.0");
			case _: unsupported(expression, "TIf(result-type-without-direct-default)");
		};
	}

	function coerce(value:LoweredValue, target:CPrimitiveTypeMapping, position:Position, node:String):LoweredValue {
		return switch CPrimitiveSemantics.conversion(value.mapping, target, CPUImplicit) {
			case CPConversionElided:
				value;
			case CPConversionAllowed(decision):
				if (decision.failureRequired) {
					unsupportedAt(position, '$node:implicit-conversion-requires-failure-edge');
				}
				switch decision.implementation {
					case IRIStatic:
					case IRIProgramLocal(helperId):
						unsupportedAt(position, '$node:program-local-conversion-helper-not-emitted:$helperId');
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

	function directStaticFunctionId(callee:TypedExpr):String {
		return switch callee.expr {
			case TField(_, FStatic(classReference, fieldReference)):
				final owner = classReference.get();
				CBodyLowering.functionId(owner.pack.concat([owner.name]).join("."), fieldReference.get().name);
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): directStaticFunctionId(inner);
			case _: unsupported(callee, 'TCall(callee=${nodeName(callee)}:not-direct-static)');
		};
	}

	function primitiveMapping(type:Type, position:Position, node:String):CPrimitiveTypeMapping {
		return switch CPrimitiveTypeMapper.map(type, context.profile) {
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

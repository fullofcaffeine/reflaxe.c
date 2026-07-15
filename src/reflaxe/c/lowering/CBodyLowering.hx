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
	public final requiredHeaders:Array<String>;
	public final body:CStmt;
	public final lineMappedBody:CStmt;

	public function new(modulePath:String, declarationPath:String, fieldName:String, ir:HxcIRFunction, cName:CIdentifier,
			parameterNames:Map<String, CIdentifier>, localNames:Map<String, CIdentifier>, temporaryNames:Map<String, CIdentifier>,
			requiredHeaders:Array<String>, body:CStmt, lineMappedBody:CStmt) {
		this.modulePath = modulePath;
		this.declarationPath = declarationPath;
		this.fieldName = fieldName;
		this.ir = ir;
		this.cName = cName;
		this.parameterNames = parameterNames;
		this.localNames = localNames;
		this.temporaryNames = temporaryNames;
		this.requiredHeaders = requiredHeaders.copy();
		this.body = body;
		this.lineMappedBody = lineMappedBody;
	}
}

/** Complete deterministic result for the admitted body subset. */
class CBodyLoweringResult {
	public final program:HxcIRProgram;
	public final functions:Array<CLoweredBodyFunction>;
	public final symbolTable:CSymbolTableSnapshot;

	public function new(program:HxcIRProgram, functions:Array<CLoweredBodyFunction>, symbolTable:CSymbolTableSnapshot) {
		this.program = program;
		this.functions = functions.copy();
		this.symbolTable = symbolTable;
	}
}

/** Typed Haxe body -> HxcIR -> validated structural C body pipeline. */
class CBodyLowering {
	final context:CompilationContext;

	public function new(context:CompilationContext) {
		this.context = context;
	}

	public function lower(inputFunctions:Array<CBodyFunctionInput>):CBodyLoweringResult {
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
		final built:Array<BuiltBodyFunction> = [];
		for (fn in prepared) {
			built.push(new FunctionBuilder(context, fn, preparedById).build());
		}
		final program = buildProgram(built);
		new HxcIRValidator().requireValid(program, Std.string(context.profile));
		final symbolTable = context.symbols.finalizeSymbols();
		final functionNames:Map<String, CIdentifier> = [];
		for (item in built) {
			functionNames.set(item.ir.id, context.symbols.identifierFor(item.prepared.functionRequest));
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
			final input = item.prepared.input;
			lowered.push(new CLoweredBodyFunction(input.modulePath, input.declarationPath, input.fieldName, item.ir,
				context.symbols.identifierFor(item.prepared.functionRequest), parameterNames, localNames, temporaryNames, emitter.requiredHeaders(item.ir),
				emitter.emitBody(item.ir, parameterNames, localNames, temporaryNames, functionNames, false),
				emitter.emitBody(item.ir, parameterNames, localNames, temporaryNames, functionNames, true)));
		}
		lowered.sort((left, right) -> compareUtf8(left.ir.id, right.ir.id));
		return new CBodyLoweringResult(program, lowered, symbolTable);
	}

	static function buildProgram(functions:Array<BuiltBodyFunction>):HxcIRProgram {
		final byModule:Map<String, Array<BuiltBodyFunction>> = [];
		for (fn in functions) {
			var moduleFunctions = byModule.get(fn.prepared.input.modulePath);
			if (moduleFunctions == null) {
				moduleFunctions = [];
				byModule.set(fn.prepared.input.modulePath, moduleFunctions);
			}
			moduleFunctions.push(fn);
		}
		final moduleIds = [for (moduleId in byModule.keys()) moduleId];
		moduleIds.sort(compareUtf8);
		final modules:Array<HxcIRModule> = [];
		for (moduleId in moduleIds) {
			final entries = byModule.get(moduleId);
			if (entries == null || entries.length == 0) {
				throw new CBodyEmissionError('body lowering lost module `$moduleId` while building HxcIR');
			}
			entries.sort((left, right) -> compareUtf8(left.ir.id, right.ir.id));
			modules.push({
				id: moduleId,
				types: [],
				typeInstances: [],
				globals: [],
				functions: entries.map(entry -> entry.ir),
				source: enclosingSpan(entries.map(entry -> entry.ir.source))
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

	static function compareUtf8(left:String, right:String):Int {
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
}

private typedef LoweredValue = {
	final id:String;
	final type:HxcIRTypeRef;
	final mapping:CPrimitiveTypeMapping;
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
		final declaredArguments = switch TypeTools.follow(input.fieldType) {
			case TFun(arguments, _): arguments;
			case _: unsupported(input.expression.pos, "TFunction(field-type-not-function)");
		};
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
		final returnMapping = admittedPrimitive(functionValue.t, input.expression.pos, "TFunction(return-type)");
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
	final functionContext:String;
	final parameterValuesByCompilerId:Map<Int, LoweredValue> = [];
	final localIdsByCompilerId:Map<Int, String> = [];
	final localRequests:Map<String, CSymbolRequest> = [];
	final temporaryRequests:Map<String, CSymbolRequest> = [];
	final locals:Array<HxcIRLocal> = [];
	final instructions:Array<HxcIRInstruction> = [];
	var localOrdinal = 0;
	var temporaryOrdinal = 0;
	var instructionOrdinal = 0;
	var valueOrdinal = 0;
	var terminator:Null<HxcIRTerminator> = null;

	public function new(context:CompilationContext, prepared:PreparedBodyFunction, functionsById:Map<String, PreparedBodyFunction>) {
		this.context = context;
		this.prepared = prepared;
		this.input = prepared.input;
		this.functionsById = functionsById;
		this.functionContext = 'function ${input.declarationPath}.${input.fieldName} body';
		this.localOrdinal = prepared.parameters.length;
		for (parameter in prepared.parameters) {
			parameterValuesByCompilerId.set(parameter.compilerId, {id: parameter.ir.id, type: parameter.ir.type, mapping: parameter.mapping});
		}
	}

	public function build():BuiltBodyFunction {
		final functionValue = prepared.functionValue;
		lowerStatement(functionValue.expr);
		if (terminator == null) {
			terminator = {kind: IRTReturn(null, []), source: HaxeSourceSpan.fromPosition(functionValue.expr.pos, input.sourcePath)};
		}
		final functionSpan = HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath);
		final bodySpan = HaxeSourceSpan.fromPosition(functionValue.expr.pos, input.sourcePath);
		final ir:HxcIRFunction = {
			id: prepared.irId,
			displayName: '${input.declarationPath}.${input.fieldName}',
			parameters: prepared.parameters.map(parameter -> parameter.ir),
			locals: locals,
			returnType: prepared.returnMapping.irType,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: instructions,
					terminator: terminator,
					source: bodySpan
				}
			],
			cleanupRegions: [],
			source: functionSpan
		};
		return {
			prepared: prepared,
			ir: ir,
			localRequests: localRequests,
			temporaryRequests: temporaryRequests
		};
	}

	function lowerStatement(expression:TypedExpr):Void {
		if (terminator != null) {
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
			case TConst(_) | TLocal(_) | TCast(_, _):
				lowerValue(expression);
			case TCall(_, _):
				lowerCall(expression, false);
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
		instructions.push(instruction(null, IRIOInitialize(IRPLocal(localId), value.id, IRISUninitialized, IRISInitialized), source, "initialize"));
		localIdsByCompilerId.set(variable.id, localId);
	}

	function lowerReturn(value:Null<TypedExpr>, position:Position):Void {
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		if (value == null) {
			terminator = {kind: IRTReturn(null, []), source: source};
			return;
		}
		if (prepared.returnMapping.irType == IRTVoid) {
			switch value.expr {
				case TCall(_, _):
					lowerCall(value, false);
				case _:
					unsupported(value, "TReturn(value-for-Void)");
			}
			terminator = {kind: IRTReturn(null, []), source: source};
			return;
		}
		final lowered = lowerValue(value, prepared.returnMapping);
		terminator = {kind: IRTReturn(lowered.id, []), source: source};
	}

	function lowerValue(expression:TypedExpr, ?expectedMapping:CPrimitiveTypeMapping):LoweredValue {
		return switch expression.expr {
			case TConst(constant): lowerConstant(expression, constant, expectedMapping);
			case TLocal(variable): lowerLocal(expression, variable);
			case TParenthesis(inner): lowerValue(inner, expectedMapping);
			case TCast(inner, _):
				final source = lowerValue(inner);
				coerce(source, primitiveMapping(expression.t, expression.pos, "TCast(target-type)"), expression.pos, "TCast");
			case TCall(_, _):
				final result = lowerCall(expression, true);
				if (result == null) {
					unsupported(expression, "TCall(Void-used-as-value)");
				}
				result;
			case _: unsupported(expression, nodeName(expression));
		};
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
		instructions.push(instruction(result, IRIOConstant(value), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "constant"));
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
		instructions.push(instruction(result, IRIOLoad(IRPLocal(localId)), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "load"));
		return {id: result.id, type: result.type, mapping: mapping};
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
			instructions.push(instruction(null, IRIOCall({
				dispatch: IRCDDirect(targetId),
				arguments: arguments,
				returnType: returnType,
				failure: null
			}), source, "call"));
			return null;
		}
		final result:HxcIRResult = {id: nextValueId(), type: returnType};
		instructions.push(instruction(result, IRIOCall({
			dispatch: IRCDDirect(targetId),
			arguments: arguments,
			returnType: returnType,
			failure: null
		}), source, "call"));
		if (materializeResult) {
			final ordinal = temporaryOrdinal++;
			final request = new CSymbolRequest(CSKTemporary, input.declarationPath.split(".").concat([input.fieldName, "call-result"]),
				CNSOrdinary(prepared.functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
			context.symbols.register(request);
			temporaryRequests.set(result.id, request);
		}
		return {id: result.id, type: result.type, mapping: target.returnMapping};
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
				instructions.push(instruction(result, IRIOConvert(value.id, decision.irKind, target.irType, decision.implementation, null),
					HaxeSourceSpan.fromPosition(position, input.sourcePath), "convert"));
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

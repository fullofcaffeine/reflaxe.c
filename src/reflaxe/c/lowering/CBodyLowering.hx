package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
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
import reflaxe.c.semantics.CPrimitiveTypes;

/** Stable semantic coordinates plus one real pinned-compiler function expression. */
typedef CBodyFunctionInput = {
	final modulePath:String;
	final declarationPath:String;
	final sourcePath:String;
	final fieldName:String;
	final sourceOrder:Int;
	final expression:TypedExpr;
}

/** One validated body with finalized C names and both source-mapping modes. */
class CLoweredBodyFunction {
	public final modulePath:String;
	public final declarationPath:String;
	public final fieldName:String;
	public final ir:HxcIRFunction;
	public final cName:CIdentifier;
	public final localNames:Map<String, CIdentifier>;
	public final requiredHeaders:Array<String>;
	public final body:CStmt;
	public final lineMappedBody:CStmt;

	public function new(modulePath:String, declarationPath:String, fieldName:String, ir:HxcIRFunction, cName:CIdentifier, localNames:Map<String, CIdentifier>,
			requiredHeaders:Array<String>, body:CStmt, lineMappedBody:CStmt) {
		this.modulePath = modulePath;
		this.declarationPath = declarationPath;
		this.fieldName = fieldName;
		this.ir = ir;
		this.cName = cName;
		this.localNames = localNames;
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
		final built:Array<BuiltBodyFunction> = [];
		for (input in inputs) {
			built.push(new FunctionBuilder(context, input).build());
		}
		final program = buildProgram(built);
		new HxcIRValidator().requireValid(program, Std.string(context.profile));
		final symbolTable = context.symbols.finalizeSymbols();
		final emitter = new CBodyEmitter();
		final lowered:Array<CLoweredBodyFunction> = [];
		for (item in built) {
			final localNames:Map<String, CIdentifier> = [];
			for (localId => request in item.localRequests) {
				localNames.set(localId, context.symbols.identifierFor(request));
			}
			lowered.push(new CLoweredBodyFunction(item.input.modulePath, item.input.declarationPath, item.input.fieldName, item.ir,
				context.symbols.identifierFor(item.functionRequest), localNames, emitter.requiredHeaders(item.ir),
				emitter.emitBody(item.ir, localNames, false), emitter.emitBody(item.ir, localNames, true)));
		}
		lowered.sort((left, right) -> compareUtf8(left.ir.id, right.ir.id));
		return new CBodyLoweringResult(program, lowered, symbolTable);
	}

	static function buildProgram(functions:Array<BuiltBodyFunction>):HxcIRProgram {
		final byModule:Map<String, Array<BuiltBodyFunction>> = [];
		for (fn in functions) {
			var moduleFunctions = byModule.get(fn.input.modulePath);
			if (moduleFunctions == null) {
				moduleFunctions = [];
				byModule.set(fn.input.modulePath, moduleFunctions);
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
	final input:CBodyFunctionInput;
	final ir:HxcIRFunction;
	final functionRequest:CSymbolRequest;
	final localRequests:Map<String, CSymbolRequest>;
}

private typedef LoweredValue = {
	final id:String;
	final type:HxcIRTypeRef;
}

private class FunctionBuilder {
	final context:CompilationContext;
	final input:CBodyFunctionInput;
	final functionContext:String;
	final functionRequest:CSymbolRequest;
	final localIdsByCompilerId:Map<Int, String> = [];
	final localRequests:Map<String, CSymbolRequest> = [];
	final locals:Array<HxcIRLocal> = [];
	final instructions:Array<HxcIRInstruction> = [];
	var localOrdinal = 0;
	var instructionOrdinal = 0;
	var valueOrdinal = 0;
	var returnType:HxcIRTypeRef = IRTVoid;
	var terminator:Null<HxcIRTerminator> = null;

	public function new(context:CompilationContext, input:CBodyFunctionInput) {
		this.context = context;
		this.input = input;
		this.functionContext = 'function ${input.declarationPath}.${input.fieldName} body';
		final qualifiedName = input.declarationPath.split(".").concat([input.fieldName]);
		this.functionRequest = new CSymbolRequest(CSKMethod, qualifiedName, CNSOrdinary("translation-unit"), CSVInternal);
		context.symbols.register(functionRequest);
	}

	public function build():BuiltBodyFunction {
		final functionValue = switch input.expression.expr {
			case TFunction(value): value;
			case _: unsupported(input.expression, nodeName(input.expression));
		};
		if (functionValue.args.length != 0) {
			unsupported(input.expression, 'TFunction(arguments=${functionValue.args.length})');
		}
		returnType = primitiveType(functionValue.t, input.expression.pos, "TFunction(return-type)");
		lowerStatement(functionValue.expr);
		if (terminator == null) {
			terminator = {kind: IRTReturn(null, []), source: HaxeSourceSpan.fromPosition(functionValue.expr.pos, input.sourcePath)};
		}
		final functionSpan = HaxeSourceSpan.fromPosition(input.expression.pos, input.sourcePath);
		final bodySpan = HaxeSourceSpan.fromPosition(functionValue.expr.pos, input.sourcePath);
		final ir:HxcIRFunction = {
			id: 'function.${input.declarationPath}.${input.fieldName}',
			displayName: '${input.declarationPath}.${input.fieldName}',
			parameters: [],
			locals: locals,
			returnType: returnType,
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
			input: input,
			ir: ir,
			functionRequest: functionRequest,
			localRequests: localRequests
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
			case TConst(_) | TLocal(_):
				lowerValue(expression);
			case _:
				unsupported(expression, nodeName(expression));
		}
	}

	function lowerVariable(variable:TVar, initializer:Null<TypedExpr>, position:Position):Void {
		final ordinal = localOrdinal++;
		final localId = 'local.$ordinal';
		final localType = primitiveType(variable.t, position, 'TVar(${variable.name}:type)');
		if (localType == IRTVoid) {
			unsupportedAt(position, 'TVar(${variable.name}:Void)');
		}
		final initialExpression:TypedExpr = switch initializer {
			case null: unsupportedAt(position, 'TVar(${variable.name}:uninitialized)');
			case value: value;
		};
		final value = lowerValue(initialExpression, localType);
		final source = HaxeSourceSpan.fromPosition(position, input.sourcePath);
		locals.push({
			id: localId,
			type: localType,
			storage: IRLSAutomatic,
			initialState: IRISUninitialized,
			source: source
		});
		final request = new CSymbolRequest(CSKLocal, input.declarationPath.split(".").concat([input.fieldName, variable.name]),
			CNSOrdinary(functionRequest.stableKey()), CSVInternal, null, [], [], ordinal);
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
		final lowered = lowerValue(value, returnType);
		terminator = {kind: IRTReturn(lowered.id, []), source: source};
	}

	function lowerValue(expression:TypedExpr, ?expectedType:HxcIRTypeRef):LoweredValue {
		return switch expression.expr {
			case TConst(constant): lowerConstant(expression, constant, expectedType);
			case TLocal(variable): lowerLocal(expression, variable);
			case TParenthesis(inner): lowerValue(inner, expectedType);
			case _: unsupported(expression, nodeName(expression));
		};
	}

	function lowerConstant(expression:TypedExpr, constant:TConstant, expectedType:Null<HxcIRTypeRef>):LoweredValue {
		final inferredType = primitiveType(expression.t, expression.pos, nodeName(expression));
		final type = contextualConstantType(constant, inferredType, expectedType);
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
		final result:HxcIRResult = {id: nextValueId(), type: type};
		instructions.push(instruction(result, IRIOConstant(value), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "constant"));
		return {id: result.id, type: result.type};
	}

	static function contextualConstantType(constant:TConstant, inferred:HxcIRTypeRef, expected:Null<HxcIRTypeRef>):HxcIRTypeRef {
		if (expected == null) {
			return inferred;
		}
		return switch constant {
			case TInt(_):
				switch inferred {
					case IRTInt(_, _):
						switch expected {
							case IRTInt(_, _): expected;
							case _: inferred;
						}
					case _: inferred;
				}
			case _: inferred;
		};
	}

	function lowerLocal(expression:TypedExpr, variable:TVar):LoweredValue {
		final localId:String = switch localIdsByCompilerId.get(variable.id) {
			case null: unsupported(expression, 'TLocal(${variable.name}:outside-admitted-body)');
			case value: value;
		};
		final type = primitiveType(expression.t, expression.pos, 'TLocal(${variable.name}:type)');
		final result:HxcIRResult = {id: nextValueId(), type: type};
		instructions.push(instruction(result, IRIOLoad(IRPLocal(localId)), HaxeSourceSpan.fromPosition(expression.pos, input.sourcePath), "load"));
		return {id: result.id, type: result.type};
	}

	function primitiveType(type:Type, position:Position, node:String):HxcIRTypeRef {
		return switch CPrimitiveTypeMapper.map(type, context.profile) {
			case CTPrimitive(mapping):
				final admitted = mapping.nullability == CPNonNullable && switch mapping.sourceType {
					case CPHaxeVoid | CPHaxeBool | CPHaxeInt | CPHaxeUInt | CPHaxeFloat: true;
					case _: false;
				};
				if (!admitted) {
					unsupportedAt(position, '$node:${mapping.cSpelling}');
				}
				mapping.irType;
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

	static function nodeName(expression:TypedExpr):String {
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

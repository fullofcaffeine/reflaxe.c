package reflaxe.c.lowering;

import haxe.io.Bytes;
import reflaxe.c.ast.CAST;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;

/** Lowers the admitted primitive HxcIR body subset into structural strict C11. */
class CBodyEmitter {
	public function new() {}

	public function emitBody(fn:HxcIRFunction, parameterNames:Map<String, CIdentifier>, localNames:Map<String, CIdentifier>,
			temporaryNames:Map<String, CIdentifier>, functionNames:Map<String, CIdentifier>, lineDirectives:Bool):CStmt {
		if (fn.blocks.length != 1 || fn.entryBlockId != fn.blocks[0].id || fn.cleanupRegions.length != 0) {
			fail('body lowering requires one cleanup-free entry block in `${fn.id}`');
		}
		final block = fn.blocks[0];
		final values:Map<String, CExpr> = [];
		final declared:Map<String, Bool> = [];
		final referencedValues = referencedValueIds(block);
		final referencedLocals = referencedLocalIds(block);
		final statements:Array<CStmt> = [];
		for (parameter in fn.parameters) {
			final name = requireParameterName(parameterNames, parameter.id, fn.id);
			values.set(parameter.id, EIdentifier(name));
			if (!referencedValues.exists(parameter.id)) {
				addLineDirective(statements, parameter.source, lineDirectives);
				statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(name))));
			}
		}
		for (instruction in block.instructions) {
			switch instruction.kind {
				case IRIOConstant(value):
					final result = requireResult(instruction, fn.id);
					values.set(result.id, constantExpression(value));
				case IRIOLoad(IRPLocal(localId)):
					final result = requireResult(instruction, fn.id);
					final expression:CExpr = EIdentifier(requireLocalName(localNames, localId, fn.id));
					values.set(result.id, expression);
					if (!referencedValues.exists(result.id)) {
						addLineDirective(statements, instruction.source, lineDirectives);
						statements.push(SExpr(ECast(new CType(TVoid), DName(null), expression)));
					}
				case IRIOInitialize(IRPLocal(localId), valueId, IRISUninitialized, IRISInitialized):
					if (instruction.result != null) {
						fail('initializer `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
					}
					if (declared.exists(localId)) {
						fail('local `$localId` in `${fn.id}` is initialized more than once');
					}
					final local = requireLocal(fn, localId);
					addLineDirective(statements, instruction.source, lineDirectives);
					statements.push(SDecl({
						storage: [],
						alignments: [],
						type: cType(local.type),
						declarator: DName(requireLocalName(localNames, localId, fn.id)),
						initializer: IExpr(requireValue(values, valueId, fn.id)),
						attributes: []
					}));
					declared.set(localId, true);
					if (!referencedLocals.exists(localId)) {
						statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(requireLocalName(localNames, localId, fn.id)))));
					}
				case IRIOConvert(valueId, kind, targetType, IRIStatic, null):
					final result = requireResult(instruction, fn.id);
					final expression = switch kind {
						case IRCNumericExact | IRCNumericWrapping:
							ECast(cType(targetType), DName(null), requireValue(values, valueId, fn.id));
						case _:
							fail('conversion `${instruction.id}` in `${fn.id}` is outside the admitted direct primitive conversion subset');
					};
					values.set(result.id, expression);
					if (!referencedValues.exists(result.id)) {
						addLineDirective(statements, instruction.source, lineDirectives);
						statements.push(SExpr(ECast(new CType(TVoid), DName(null), expression)));
					}
				case IRIOCall(call):
					emitCall(statements, values, referencedValues, instruction, call, temporaryNames, functionNames, lineDirectives, fn.id);
				case _:
					fail('HxcIR instruction `${instruction.id}` in `${fn.id}` is outside the static primitive function subset');
			}
		}

		final terminator = requireTerminator(block.terminator, fn.id);
		addLineDirective(statements, terminator.source, lineDirectives);
		switch terminator.kind {
			case IRTReturn(valueId, cleanup):
				if (cleanup.length != 0) {
					fail('return in `${fn.id}` unexpectedly requires cleanup');
				}
				statements.push(SReturn(valueId == null ? null : requireValue(values, valueId, fn.id)));
			case _:
				fail('function `${fn.id}` has a non-return terminator outside the admitted body subset');
		}
		return SBlock(statements);
	}

	static function referencedValueIds(block:HxcIRBlock):Map<String, Bool> {
		final referenced:Map<String, Bool> = [];
		for (instruction in block.instructions) {
			switch instruction.kind {
				case IRIOInitialize(_, valueId, _, _):
					referenced.set(valueId, true);
				case IRIOConvert(valueId, _, _, _, _):
					referenced.set(valueId, true);
				case IRIOCall(call):
					for (argument in call.arguments) {
						referenced.set(argument, true);
					}
				case _:
			}
		}
		if (block.terminator != null) {
			switch block.terminator.kind {
				case IRTReturn(valueId, _) if (valueId != null):
					referenced.set(valueId, true);
				case _:
			}
		}
		return referenced;
	}

	static function referencedLocalIds(block:HxcIRBlock):Map<String, Bool> {
		final referenced:Map<String, Bool> = [];
		for (instruction in block.instructions) {
			switch instruction.kind {
				case IRIOLoad(IRPLocal(localId)) | IRIOStore(IRPLocal(localId), _) | IRIOAddress(IRPLocal(localId)):
					referenced.set(localId, true);
				case _:
			}
		}
		return referenced;
	}

	public function cType(type:HxcIRTypeRef):CType {
		return switch type {
			case IRTVoid: new CType(TVoid);
			case IRTBool: new CType(TBool);
			case IRTInt(width, signed): new CType(TInt(width, signed));
			case IRTFloat(64): new CType(TDouble);
			case _:
				throw new CBodyEmissionError('HxcIR type `${typeKey(type)}` is outside the admitted primitive C body subset');
		};
	}

	public function requiredHeaders(fn:HxcIRFunction):Array<String> {
		final headers:Array<String> = [];
		addTypeHeaders(headers, fn.returnType);
		for (parameter in fn.parameters) {
			addTypeHeaders(headers, parameter.type);
		}
		for (local in fn.locals) {
			addTypeHeaders(headers, local.type);
		}
		for (block in fn.blocks) {
			for (instruction in block.instructions) {
				if (instruction.result != null) {
					addTypeHeaders(headers, instruction.result.type);
				}
			}
		}
		headers.sort(compareUtf8);
		return headers;
	}

	public function parameters(fn:HxcIRFunction, names:Map<String, CIdentifier>):Array<CParam> {
		final result:Array<CParam> = [];
		for (parameter in fn.parameters) {
			result.push({
				type: cType(parameter.type),
				declarator: DName(requireParameterName(names, parameter.id, fn.id)),
				attributes: []
			});
		}
		return result;
	}

	static function addTypeHeaders(headers:Array<String>, type:HxcIRTypeRef):Void {
		switch type {
			case IRTBool:
				addUnique(headers, "stdbool.h");
			case IRTInt(_, _):
				addUnique(headers, "stdint.h");
			case IRTVoid | IRTFloat(64):
			case _:
				throw new CBodyEmissionError('HxcIR type `${typeKey(type)}` has no admitted strict-C body header mapping');
		}
	}

	static function constantExpression(value:HxcIRConstant):CExpr {
		return switch value {
			case IRCInt(text): integerExpression(text);
			case IRCFloat(text): floatExpression(text);
			case IRCBool(value): EBool(value);
			case IRCString(_) | IRCNull:
				throw new CBodyEmissionError("string and null constants are outside the admitted primitive C body subset");
		};
	}

	static function integerExpression(text:String):CExpr {
		final negative = StringTools.startsWith(text, "-");
		final magnitude = negative ? text.substr(1) : text;
		final literal:CExpr = EInt(CIntegerLiteral.decimal(magnitude));
		return negative ? EUnary(Minus, literal) : literal;
	}

	static function floatExpression(text:String):CExpr {
		final negative = StringTools.startsWith(text, "-");
		var unsigned = negative ? text.substr(1) : text;
		var exponent:Null<Int> = null;
		var exponentIndex = unsigned.indexOf("e");
		if (exponentIndex == -1) {
			exponentIndex = unsigned.indexOf("E");
		}
		if (exponentIndex != -1) {
			final exponentText = unsigned.substr(exponentIndex + 1);
			exponent = Std.parseInt(exponentText);
			if (exponent == null) {
				fail('floating constant `$text` has an exponent outside the C AST integer range');
			}
			unsigned = unsigned.substr(0, exponentIndex);
		}
		final dot = unsigned.indexOf(".");
		final whole = dot == -1 ? unsigned : unsigned.substr(0, dot);
		var fraction = dot == -1 ? "" : unsigned.substr(dot + 1);
		if (fraction == "" && exponent == null) {
			fraction = "0";
		}
		final literal:CExpr = EFloat(new CFloatLiteral(FRDecimal(whole, fraction, exponent)));
		return negative ? EUnary(Minus, literal) : literal;
	}

	function emitCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction, call:HxcIRCall,
			temporaryNames:Map<String, CIdentifier>, functionNames:Map<String, CIdentifier>, lineDirectives:Bool, functionId:String):Void {
		final targetId = switch call.dispatch {
			case IRCDDirect(value): value;
			case _: return fail('call `${instruction.id}` in `$functionId` is not direct static dispatch');
		};
		final targetName = requireFunctionName(functionNames, targetId, functionId);
		final arguments = call.arguments.map(argument -> requireValue(values, argument, functionId));
		final callExpression:CExpr = ECall(EIdentifier(targetName), arguments);
		addLineDirective(statements, instruction.source, lineDirectives);
		if (call.returnType == IRTVoid) {
			if (instruction.result != null) {
				fail('Void call `${instruction.id}` in `$functionId` unexpectedly defines a value');
			}
			statements.push(SExpr(callExpression));
			return;
		}

		final result = requireResult(instruction, functionId);
		final temporaryName = temporaryNames.get(result.id);
		if (temporaryName == null) {
			if (referencedValues.exists(result.id)) {
				fail('referenced call result `${result.id}` in `$functionId` has no finalized C temporary');
			}
			statements.push(SExpr(callExpression));
			return;
		}
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: cType(result.type),
			declarator: DName(temporaryName),
			initializer: IExpr(callExpression),
			attributes: []
		}));
		values.set(result.id, EIdentifier(temporaryName));
	}

	static function requireResult(instruction:HxcIRInstruction, functionId:String):HxcIRResult {
		final result = instruction.result;
		if (result == null) {
			throw new CBodyEmissionError('value instruction `${instruction.id}` in `$functionId` has no result after validation');
		}
		return result;
	}

	static function requireTerminator(terminator:Null<HxcIRTerminator>, functionId:String):HxcIRTerminator {
		if (terminator == null) {
			throw new CBodyEmissionError('entry block in `$functionId` has no terminator after validation');
		}
		return terminator;
	}

	static function requireValue(values:Map<String, CExpr>, valueId:String, functionId:String):CExpr {
		final value = values.get(valueId);
		if (value == null) {
			throw new CBodyEmissionError('C lowering in `$functionId` cannot resolve HxcIR value `$valueId`');
		}
		return value;
	}

	static function requireLocal(fn:HxcIRFunction, localId:String):HxcIRLocal {
		for (local in fn.locals) {
			if (local.id == localId) {
				return local;
			}
		}
		throw new CBodyEmissionError('function `${fn.id}` cannot resolve HxcIR local `$localId`');
	}

	static function requireLocalName(localNames:Map<String, CIdentifier>, localId:String, functionId:String):CIdentifier {
		final name = localNames.get(localId);
		if (name == null) {
			throw new CBodyEmissionError('function `$functionId` has no finalized C name for local `$localId`');
		}
		return name;
	}

	static function requireParameterName(parameterNames:Map<String, CIdentifier>, parameterId:String, functionId:String):CIdentifier {
		final name = parameterNames.get(parameterId);
		if (name == null) {
			throw new CBodyEmissionError('function `$functionId` has no finalized C name for parameter `$parameterId`');
		}
		return name;
	}

	static function requireFunctionName(functionNames:Map<String, CIdentifier>, targetId:String, ownerId:String):CIdentifier {
		final name = functionNames.get(targetId);
		if (name == null) {
			throw new CBodyEmissionError('function `$ownerId` has no finalized C name for direct target `$targetId`');
		}
		return name;
	}

	static function addLineDirective(statements:Array<CStmt>, source:HxcSourceSpan, enabled:Bool):Void {
		if (enabled) {
			statements.push(SLineDirective({line: source.startLine, file: source.file}));
		}
	}

	static function addUnique(values:Array<String>, value:String):Void {
		if (values.indexOf(value) == -1) {
			values.push(value);
		}
	}

	static function typeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): 'int:$width:${signed ? "signed" : "unsigned"}';
			case IRTAbiInteger(kind): 'abi-int:$kind';
			case IRTFloat(width): 'float:$width';
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(_, nullable): 'pointer:${nullable ? "nullable" : "non-null"}';
			case IRTNullable(_, representation): 'nullable:$representation';
			case IRTFunction(_, _): "function";
			case IRTDynamic: "dynamic";
		};
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

	static function fail<T>(detail:String):T
		throw new CBodyEmissionError(detail);
}

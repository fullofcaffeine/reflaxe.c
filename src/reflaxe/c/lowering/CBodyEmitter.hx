package reflaxe.c.lowering;

import haxe.io.Bytes;
import reflaxe.c.ast.CAST;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;

/** Lowers the admitted primitive HxcIR body subset into structural strict C11. */
class CBodyEmitter {
	public function new() {}

	public function emitBody(fn:HxcIRFunction, localNames:Map<String, CIdentifier>, lineDirectives:Bool):CStmt {
		if (fn.blocks.length != 1
			|| fn.entryBlockId != fn.blocks[0].id
			|| fn.parameters.length != 0
			|| fn.cleanupRegions.length != 0) {
			fail('body lowering requires one parameter-free, cleanup-free entry block in `${fn.id}`');
		}
		final block = fn.blocks[0];
		final values:Map<String, CExpr> = [];
		final declared:Map<String, Bool> = [];
		final referencedValues = referencedValueIds(block);
		final statements:Array<CStmt> = [];
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
				case _:
					fail('HxcIR instruction `${instruction.id}` in `${fn.id}` is outside the constants/locals/body subset');
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
		for (local in fn.locals) {
			addTypeHeaders(headers, local.type);
		}
		headers.sort(compareUtf8);
		return headers;
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

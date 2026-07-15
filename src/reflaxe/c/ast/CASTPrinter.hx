package reflaxe.c.ast;

import reflaxe.c.ast.CAST;

/** Deterministic C11 printer seed. Formatting is intentionally boring. */
class CASTPrinter {
	final indentUnit:String;
	var indent:Int = 0;

	public function new(indentUnit:String = "  ") {
		if (indentUnit == "")
			throw "C AST indent unit must not be empty";
		this.indentUnit = indentUnit;
	}

	public function printTranslationUnit(unit:CTranslationUnit):String {
		indent = 0;
		final out:Array<String> = [];
		final includes = unit.includes.copy();
		includes.sort((a, b) -> {
			final kindOrder = includeKindOrder(a.kind) - includeKindOrder(b.kind);
			return kindOrder != 0 ? kindOrder : compareUtf8(a.path, b.path);
		});

		var lastKind:Null<CIncludeKind> = null;
		for (include in includes) {
			if (lastKind != null && lastKind != include.kind)
				out.push("");
			out.push(switch include.kind {
				case System:
					validateIncludePath(include.path, true);
					'#include <${include.path}>';
				case Local:
					validateIncludePath(include.path, false);
					'#include "${include.path}"';
			});
			lastKind = include.kind;
		}
		if (includes.length > 0 && unit.declarations.length > 0)
			out.push("");

		for (index in 0...unit.declarations.length) {
			out.push(printDecl(unit.declarations[index]));
			if (index + 1 < unit.declarations.length)
				out.push("");
		}
		return out.join("\n") + "\n";
	}

	public function printDecl(decl:CDecl):String {
		return switch decl {
			case DComment(text): printComment(text);
			case DRawCompilerOwned(text): text;
			case DStaticAssert(condition, message): '_Static_assert(${printExpr(condition, 3)}, ${quote(message)});';
			case DForwardStruct(name): 'struct $name;';
			case DForwardUnion(name): 'union $name;';
			case DStruct(name, fields, attributes): printAggregate("struct", name, fields, attributes);
			case DUnion(name, fields, attributes): printAggregate("union", name, fields, attributes);
			case DEnum(name, values, attributes): printEnum(name, values, attributes);
			case DTypedef(type, declarator): 'typedef ${printTypedDeclarator(type, declarator)};';
			case DVariable(variable): printVariable(variable) + ";";
			case DPrototype(storage, returnType, declarator, attributes):
				joinTokens([
					printStorage(storage),
					printType(returnType),
					printDeclarator(declarator),
					printAttributes(attributes)
				]) + ";";
			case DFunction(definition): printFunction(definition);
		}
	}

	public function printStmt(stmt:CStmt):String {
		return switch stmt {
			case SEmpty: line(";");
			case SComment(text): line(printComment(text));
			case SExpr(expr): line(printExpr(expr) + ";");
			case SDecl(decl): line(printVariable(decl) + ";");
			case SBlock(statements): printBlock(statements);
			case SIf(condition, thenBranch, elseBranch): printIf(condition, thenBranch, elseBranch);
			case SWhile(condition, body): line('while (${printExpr(condition)})') + "\n" + printControlledBody(body);
			case SDoWhile(body, condition): line("do")
				+ "\n"
				+ printControlledBody(body)
				+ "\n"
				+ line('while (${printExpr(condition)});');
			case SFor(init, condition, step, body):
				final initText = init == null ? "" : printForInit(init);
				final conditionText = condition == null ? "" : printExpr(condition);
				final stepText = step == null ? "" : printExpr(step);
				line('for ($initText; $conditionText; $stepText)') + "\n" + printControlledBody(body);
			case SSwitch(value, cases): printSwitch(value, cases);
			case SReturn(value): line(value == null ? "return;" : 'return ${printExpr(value)};');
			case SBreak: line("break;");
			case SContinue: line("continue;");
			case SGoto(labelName): line('goto $labelName;');
			case SLabel(labelName, statement): printLabel(labelName, statement);
		}
	}

	public function printExpr(expr:CExpr, parentPrecedence:Int = 0):String {
		final current = precedence(expr);
		final rendered = switch expr {
			case EIdentifier(name): name;
			case EInt(value): value;
			case EFloat(value): value;
			case EString(value): quote(value);
			case EChar(value): printChar(value);
			case EBool(value): value ? "true" : "false";
			case ENull: "NULL";
			case EUnary(op, value): printUnary(op, value);
			case EBinary(op, left, right): printBinary(op, left, right);
			case EConditional(condition, thenValue, elseValue):
				'${printExpr(condition, 4)} ? ${printExpr(thenValue)} : ${printExpr(elseValue, 3)}';
			case ECall(callee, args):
				'${printExpr(callee, 15)}(${args.map(arg -> printExpr(arg, 2)).join(", ")})';
			case EIndex(target, index): '${printExpr(target, 15)}[${printExpr(index)}]';
			case EMember(target, field, throughPointer): '${printExpr(target, 15)}${throughPointer ? "->" : "."}$field';
			case ECast(type, declarator, value): '(${printTypeName(type, declarator)})${printExpr(value, 14)}';
			case ESizeOfType(type, declarator): 'sizeof(${printTypeName(type, declarator)})';
			case EAlignOfType(type, declarator): '_Alignof(${printTypeName(type, declarator)})';
			case ECompoundLiteral(type, declarator, initializer): '(${printTypeName(type, declarator)})${printInitializer(initializer)}';
			case EParen(value): '(${printExpr(value)})';
			case ERawCompilerOwned(text): text;
		}
		return current < parentPrecedence ? '($rendered)' : rendered;
	}

	public function printTypedDeclarator(type:CType, declarator:CDeclarator):String {
		return joinTokens([printType(type), printDeclarator(declarator)]);
	}

	public function printType(type:CType):String {
		final qualifiers = type.qualifiers.map(q -> cast(q, String));
		final spec = switch type.spec {
			case TVoid: "void";
			case TBool: "bool";
			case TChar(null): "char";
			case TChar(true): "signed char";
			case TChar(false): "unsigned char";
			case TInt(8, true): "int8_t";
			case TInt(8, false): "uint8_t";
			case TInt(16, true): "int16_t";
			case TInt(16, false): "uint16_t";
			case TInt(32, true): "int32_t";
			case TInt(32, false): "uint32_t";
			case TInt(64, true): "int64_t";
			case TInt(64, false): "uint64_t";
			case TInt(bits, _): throw 'Unsupported exact-width C integer: $bits bits';
			case TFloat: "float";
			case TDouble: "double";
			case TNamed(name): name;
			case TStruct(name): 'struct $name';
			case TUnion(name): 'union $name';
			case TEnum(name): 'enum $name';
		}
		qualifiers.push(spec);
		return qualifiers.join(" ");
	}

	/**
		Prints grammar-level declarators. Callers express semantic grouping with
		`DGroup`; the printer never guesses pointer/function/array association.
	 */
	public function printDeclarator(declarator:CDeclarator):String {
		return switch declarator {
			case DName(name): name;
			case DPointer(inner, qualifiers):
				final qualifierText = qualifiers.map(q -> cast(q, String)).join(" ");
				"*" + (qualifierText == "" ? "" : qualifierText + " ") + printDeclarator(inner);
			case DArray(inner, size, isStatic, qualifiers):
				final parts:Array<String> = [];
				if (isStatic)
					parts.push("static");
				for (qualifier in qualifiers)
					parts.push(cast(qualifier, String));
				if (size != null)
					parts.push(printExpr(size, 2));
				'${printDeclarator(inner)}[${parts.join(" ")}]';
			case DFunction(inner, params, variadic):
				final values = params.map(param -> printTypedDeclarator(param.type, param.declarator));
				if (variadic)
					values.push("...");
				if (values.length == 0)
					values.push("void");
				'${printDeclarator(inner)}(${values.join(", ")})';
			case DGroup(inner): '(${printDeclarator(inner)})';
		}
	}

	function printTypeName(type:CType, declarator:CDeclarator):String {
		return printTypedDeclarator(type, declarator);
	}

	function printFunction(definition:CFunctionDef):String {
		final header = joinTokens([
			printStorage(definition.storage),
			printType(definition.returnType),
			printDeclarator(definition.declarator),
			printAttributes(definition.attributes)
		]);
		return header + "\n" + printControlledBody(definition.body);
	}

	function printVariable(variable:CVariableDecl):String {
		final left = joinTokens([
			printStorage(variable.storage),
			printType(variable.type),
			printDeclarator(variable.declarator),
			printAttributes(variable.attributes)
		]);
		return variable.initializer == null ? left : '$left = ${printInitializer(variable.initializer)}';
	}

	function printInitializer(initializer:CInitializer):String {
		return switch initializer {
			case IExpr(expr): printExpr(expr, 2);
			case IList(items):
				final rendered = items.map(item -> {
					final prefix = item.designators.map(designator -> switch designator {
						case DField(name): '.$name';
						case DIndex(index): '[${printExpr(index)}]';
					}).join("");
					(prefix == "" ? "" : prefix + " = ") + printInitializer(item.value);
				});
				'{ ${rendered.join(", ")} }';
		}
	}

	function printAggregate(keyword:String, name:String, fields:Array<CField>, attributes:Array<String>):String {
		final out:Array<String> = [];
		out.push(joinTokens(['$keyword $name', printAttributes(attributes)]) + " {");
		indent++;
		for (field in fields) {
			final bit = field.bitWidth == null ? "" : ' : ${printExpr(field.bitWidth, 3)}';
			out.push(line(printTypedDeclarator(field.type, field.declarator) + bit + ";"));
		}
		indent--;
		out.push(line("};"));
		return out.join("\n");
	}

	function printEnum(name:String, values:Array<CEnumerator>, attributes:Array<String>):String {
		final out:Array<String> = [];
		out.push(joinTokens(['enum $name', printAttributes(attributes)]) + " {");
		indent++;
		for (index in 0...values.length) {
			final value = values[index];
			final assignment = value.value == null ? "" : ' = ${printExpr(value.value, 3)}';
			final comma = index + 1 < values.length ? "," : "";
			out.push(line(value.name + assignment + comma));
		}
		indent--;
		out.push(line("};"));
		return out.join("\n");
	}

	function printBlock(statements:Array<CStmt>):String {
		final out:Array<String> = [line("{")];
		indent++;
		for (statement in statements)
			out.push(printStmt(statement));
		indent--;
		out.push(line("}"));
		return out.join("\n");
	}

	function printControlledBody(body:CStmt):String {
		return switch body {
			case SBlock(_): printStmt(body);
			case _: printBlock([body]);
		}
	}

	function printIf(condition:CExpr, thenBranch:CStmt, elseBranch:Null<CStmt>):String {
		var result = line('if (${printExpr(condition)})') + "\n" + printControlledBody(thenBranch);
		if (elseBranch != null)
			result += "\n" + line("else") + "\n" + printControlledBody(elseBranch);
		return result;
	}

	function printLabel(labelName:String, statement:CStmt):String {
		final previous = indent;
		indent = 0;
		final header = line('$labelName:');
		indent = previous + 1;
		final body = switch statement {
			// In C11 a label cannot directly prefix a declaration. A block is a statement.
			case SDecl(_): printBlock([statement]);
			case _: printStmt(statement);
		}
		indent = previous;
		return header + "\n" + body;
	}

	function printSwitch(value:CExpr, cases:Array<CCase>):String {
		final out:Array<String> = [line('switch (${printExpr(value)}) {')];
		indent++;
		for (caseData in cases) {
			if (caseData.isDefault)
				out.push(line("default:"));
			for (caseValue in caseData.values)
				out.push(line('case ${printExpr(caseValue, 3)}:'));
			indent++;
			for (statement in caseData.body)
				out.push(printStmt(statement));
			indent--;
		}
		indent--;
		out.push(line("}"));
		return out.join("\n");
	}

	function printForInit(stmt:CStmt):String {
		return switch stmt {
			case SDecl(decl): printVariable(decl);
			case SExpr(expr): printExpr(expr);
			case SEmpty: "";
			case _: throw "Only declaration/expression/empty statements are valid for C for-loop initialization";
		}
	}

	function printUnary(op:CUnaryOp, value:CExpr):String {
		final operandPrecedence = switch op {
			case PostIncrement | PostDecrement: 15;
			case _: 14;
		}
		final operand = printExpr(value, operandPrecedence);
		return switch op {
			case AddressOf: '&$operand';
			case Dereference: '*$operand';
			case Plus: '+$operand';
			case Minus: '-$operand';
			case LogicalNot: '!$operand';
			case BitwiseNot: '~$operand';
			case PreIncrement: '++$operand';
			case PreDecrement: '--$operand';
			case PostIncrement: '$operand++';
			case PostDecrement: '$operand--';
			case SizeOfExpr: 'sizeof($operand)';
		}
	}

	function printBinary(op:CBinaryOp, left:CExpr, right:CExpr):String {
		final p = binaryPrecedence(op);
		final rightAssociative = isRightAssociative(op);
		final leftParent = rightAssociative ? p + 1 : p;
		final rightParent = rightAssociative ? p : p + 1;
		return '${printExpr(left, leftParent)} ${binaryToken(op)} ${printExpr(right, rightParent)}';
	}

	function precedence(expr:CExpr):Int {
		return switch expr {
			case EIdentifier(_) | EInt(_) | EFloat(_) | EString(_) | EChar(_) | EBool(_) | ENull | ERawCompilerOwned(_): 16;
			case ECall(_, _) | EIndex(_, _) | EMember(_, _, _): 15;
			case EUnary(PostIncrement, _) | EUnary(PostDecrement, _): 15;
			case EUnary(_, _) | ECast(_, _, _) | ESizeOfType(_, _) | EAlignOfType(_, _): 14;
			case EBinary(op, _, _): binaryPrecedence(op);
			case EConditional(_, _, _): 3;
			case ECompoundLiteral(_, _, _) | EParen(_): 16;
		}
	}

	function binaryPrecedence(op:CBinaryOp):Int {
		return switch op {
			case Multiply | Divide | Modulo: 13;
			case Add | Subtract: 12;
			case ShiftLeft | ShiftRight: 11;
			case Less | LessEqual | Greater | GreaterEqual: 10;
			case Equal | NotEqual: 9;
			case BitAnd: 8;
			case BitXor: 7;
			case BitOr: 6;
			case LogicalAnd: 5;
			case LogicalOr: 4;
			case Assign | AddAssign | SubtractAssign | MultiplyAssign | DivideAssign | ModuloAssign | ShiftLeftAssign | ShiftRightAssign | BitAndAssign |
				BitXorAssign | BitOrAssign: 2;
			case Comma: 1;
		}
	}

	function binaryToken(op:CBinaryOp):String {
		return switch op {
			case Multiply: "*";
			case Divide: "/";
			case Modulo: "%";
			case Add: "+";
			case Subtract: "-";
			case ShiftLeft: "<<";
			case ShiftRight: ">>";
			case Less: "<";
			case LessEqual: "<=";
			case Greater: ">";
			case GreaterEqual: ">=";
			case Equal: "==";
			case NotEqual: "!=";
			case BitAnd: "&";
			case BitXor: "^";
			case BitOr: "|";
			case LogicalAnd: "&&";
			case LogicalOr: "||";
			case Assign: "=";
			case AddAssign: "+=";
			case SubtractAssign: "-=";
			case MultiplyAssign: "*=";
			case DivideAssign: "/=";
			case ModuloAssign: "%=";
			case ShiftLeftAssign: "<<=";
			case ShiftRightAssign: ">>=";
			case BitAndAssign: "&=";
			case BitXorAssign: "^=";
			case BitOrAssign: "|=";
			case Comma: ",";
		}
	}

	function isRightAssociative(op:CBinaryOp):Bool {
		return switch op {
			case Assign | AddAssign | SubtractAssign | MultiplyAssign | DivideAssign | ModuloAssign | ShiftLeftAssign | ShiftRightAssign | BitAndAssign |
				BitXorAssign | BitOrAssign: true;
			case _: false;
		}
	}

	function printStorage(storage:Array<CStorage>):String
		return storage.map(value -> cast(value, String)).join(" ");

	function printAttributes(attributes:Array<String>):String
		return attributes.join(" ");

	function includeKindOrder(kind:CIncludeKind):Int
		return switch kind {
			case System: 0;
			case Local: 1;
		}

	function joinTokens(tokens:Array<String>):String
		return tokens.filter(token -> StringTools.trim(token) != "").join(" ");

	function line(value:String):String
		return indentPrefix() + value;

	function indentPrefix():String {
		final out = new StringBuf();
		for (_ in 0...indent)
			out.add(indentUnit);
		return out.toString();
	}

	function compareUtf8(a:String, b:String):Int {
		final left = utf8Bytes(a);
		final right = utf8Bytes(b);
		final shared = left.length < right.length ? left.length : right.length;
		for (index in 0...shared) {
			if (left[index] != right[index])
				return left[index] < right[index] ? -1 : 1;
		}
		return left.length < right.length ? -1 : (left.length > right.length ? 1 : 0);
	}

	function printComment(text:String):String {
		final safe = StringTools.replace(text, "*/", "* /");
		return '/* $safe */';
	}

	function quote(value:String):String {
		final escaped = new StringBuf();
		escaped.add('"');
		for (byte in utf8Bytes(value)) {
			switch byte {
				case 0x22:
					escaped.add('\\"');
				case 0x3F:
					escaped.add('\\?'); // Prevent C11 trigraph formation.
				case 0x5C:
					escaped.add('\\\\');
				case 0x07:
					escaped.add('\\a');
				case 0x08:
					escaped.add('\\b');
				case 0x09:
					escaped.add('\\t');
				case 0x0A:
					escaped.add('\\n');
				case 0x0B:
					escaped.add('\\v');
				case 0x0C:
					escaped.add('\\f');
				case 0x0D:
					escaped.add('\\r');
				case _ if (byte >= 0x20 && byte < 0x7F):
					escaped.addChar(byte);
				case _:
					escaped.add('\\' + octalByte(byte));
			}
		}
		escaped.add('"');
		return escaped.toString();
	}

	function printChar(value:Int):String {
		if (value < 0 || value > 0xFF)
			throw 'C byte character literal out of range: $value';
		return switch value {
			case 0x27: "'\\\''";
			case 0x3F: "'\\?'";
			case 0x5C: "'\\\\'";
			case 0x07: "'\\a'";
			case 0x08: "'\\b'";
			case 0x09: "'\\t'";
			case 0x0A: "'\\n'";
			case 0x0B: "'\\v'";
			case 0x0C: "'\\f'";
			case 0x0D: "'\\r'";
			case _ if (value >= 0x20 && value < 0x7F): "'" + String.fromCharCode(value) + "'";
			case _: "'\\" + octalByte(value) + "'";
		}
	}

	function octalByte(value:Int):String {
		final a = (value >> 6) & 7;
		final b = (value >> 3) & 7;
		final c = value & 7;
		return String.fromCharCode(48 + a) + String.fromCharCode(48 + b) + String.fromCharCode(48 + c);
	}

	function utf8Bytes(value:String):Array<Int> {
		final result:Array<Int> = [];
		var index = 0;
		while (index < value.length) {
			final first = codeUnitAt(value, index++);
			var codePoint = first;
			if (first >= 0xD800 && first <= 0xDBFF) {
				if (index >= value.length)
					throw "Unpaired high surrogate in C string literal";
				final second = codeUnitAt(value, index++);
				if (second < 0xDC00 || second > 0xDFFF)
					throw "Unpaired high surrogate in C string literal";
				codePoint = 0x10000 + ((first - 0xD800) << 10) + (second - 0xDC00);
			} else if (first >= 0xDC00 && first <= 0xDFFF) {
				throw "Unpaired low surrogate in C string literal";
			}

			if (codePoint <= 0x7F) {
				result.push(codePoint);
			} else if (codePoint <= 0x7FF) {
				result.push(0xC0 | (codePoint >> 6));
				result.push(0x80 | (codePoint & 0x3F));
			} else if (codePoint <= 0xFFFF) {
				result.push(0xE0 | (codePoint >> 12));
				result.push(0x80 | ((codePoint >> 6) & 0x3F));
				result.push(0x80 | (codePoint & 0x3F));
			} else {
				result.push(0xF0 | (codePoint >> 18));
				result.push(0x80 | ((codePoint >> 12) & 0x3F));
				result.push(0x80 | ((codePoint >> 6) & 0x3F));
				result.push(0x80 | (codePoint & 0x3F));
			}
		}
		return result;
	}

	function codeUnitAt(value:String, index:Int):Int {
		final code = value.charCodeAt(index);
		if (code == null)
			throw 'String index out of range while printing C literal: $index';
		return code;
	}

	function validateIncludePath(path:String, system:Bool):Void {
		if (path == "" || path.indexOf("\n") != -1 || path.indexOf("\r") != -1 || path.indexOf('"') != -1) {
			throw 'Invalid C include path: "$path"';
		}
		if (system && (path.indexOf("<") != -1 || path.indexOf(">") != -1)) {
			throw 'Invalid system include path: "$path"';
		}
	}
}

package reflaxe.c.ast;

import reflaxe.c.ast.CAST;

/** Deterministic C11 printer seed. Formatting is intentionally boring. */
class CASTPrinter {
	final indentUnit:String;
	final dialect:CDialect;
	var indent:Int = 0;

	public function new(indentUnit:String = "  ", dialect:CDialect = StrictC11) {
		if (indentUnit == "")
			throw "C AST indent unit must not be empty";
		this.indentUnit = indentUnit;
		this.dialect = dialect;
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
			final declaration = unit.declarations[index];
			out.push(printDecl(declaration));
			if (index + 1 < unit.declarations.length && !isLineDirectiveDecl(declaration))
				out.push("");
		}
		return out.join("\n") + "\n";
	}

	public function printHeader(unit:CHeaderUnit):String {
		final guard = identifier(unit.guard);
		final body = printTranslationUnit(unit.translationUnit);
		return '#ifndef $guard\n#define $guard\n\n$body\n#endif /* $guard */\n';
	}

	public function printDecl(decl:CDecl):String {
		return switch decl {
			case DComment(text): printComment(text);
			case DLineDirective(directive): printLineDirective(directive);
			case DStaticAssert(condition, message): '_Static_assert(${printExpr(condition, 3)}, ${quote(message)});';
			case DForwardStruct(name, attributes): joinTokens(['struct ${identifier(name)}', printAttributes(attributes)]) + ";";
			case DForwardUnion(name, attributes): joinTokens(['union ${identifier(name)}', printAttributes(attributes)]) + ";";
			case DStruct(name, fields, attributes): printAggregate("struct", name, fields, attributes);
			case DUnion(name, fields, attributes): printAggregate("union", name, fields, attributes);
			case DEnum(name, values, attributes): printEnum(name, values, attributes);
			case DTypedef(type, declarator, attributes): joinTokens(["typedef", printTypedDeclarator(type, declarator), printAttributes(attributes)]) + ";";
			case DVariable(variable): printVariable(variable) + ";";
			case DPrototype(storage, functionSpecifiers, returnType, declarator, attributes):
				joinTokens([
					printStorage(storage),
					printFunctionSpecifiers(functionSpecifiers),
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
			case SLineDirective(directive): printLineDirective(directive);
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
			case SGoto(labelName): line('goto ${identifier(labelName)};');
			case SLabel(labelName, statement): printLabel(labelName, statement);
		}
	}

	public function printExpr(expr:CExpr, parentPrecedence:Int = 0):String {
		final current = precedence(expr);
		final rendered = switch expr {
			case EIdentifier(name): identifier(name);
			case EInt(value): printIntegerLiteral(value);
			case EFloat(value): printFloatLiteral(value);
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
			case EMember(target, field, throughPointer): '${printExpr(target, 15)}${throughPointer ? "->" : "."}${identifier(field)}';
			case ECast(type, declarator, value): '(${printTypeName(type, declarator)})${printExpr(value, 14)}';
			case ESizeOfType(type, declarator): 'sizeof(${printTypeName(type, declarator)})';
			case EAlignOfType(type, declarator): '_Alignof(${printTypeName(type, declarator)})';
			case EOffsetOf(type, declarator, member): 'offsetof(${printTypeName(type, declarator)}, ${identifier(member)})';
			case ECompoundLiteral(type, declarator, initializer): '(${printTypeName(type, declarator)})${printInitializer(initializer)}';
			case EGenericSelection(control, associations): printGenericSelection(control, associations);
			case EParen(value): '(${printExpr(value)})';
		}
		return current < parentPrecedence ? '($rendered)' : rendered;
	}

	public function printTypedDeclarator(type:CType, declarator:CDeclarator):String {
		return joinTokens([printType(type), printDeclarator(declarator)]);
	}

	public function printType(type:CType):String {
		if (isAtomicTypeSpecifier(type.spec) && type.qualifiers.contains(QAtomic))
			throw "A C atomic type specifier cannot also carry the _Atomic qualifier";

		final spec = switch type.spec {
			case TVoid: "void";
			case TBool: "bool";
			case TChar(null): "char";
			case TChar(true): "signed char";
			case TChar(false): "unsigned char";
			case TNativeInt(rank, signed): printNativeInteger(rank, signed);
			case TInt(8, true): "int8_t";
			case TInt(8, false): "uint8_t";
			case TInt(16, true): "int16_t";
			case TInt(16, false): "uint16_t";
			case TInt(32, true): "int32_t";
			case TInt(32, false): "uint32_t";
			case TInt(64, true): "int64_t";
			case TInt(64, false): "uint64_t";
			case TInt(bits, _): throw 'Unsupported exact-width C integer: $bits bits';
			case TSizeT: "size_t";
			case TFloat: "float";
			case TDouble: "double";
			case TLongDouble: "long double";
			case TComplex(realType): printRealType(realType) + " _Complex";
			case TNamed(name): identifier(name);
			case TStruct(name): 'struct ${identifier(name)}';
			case TUnion(name): 'union ${identifier(name)}';
			case TEnum(name): 'enum ${identifier(name)}';
			case TStructDefinition(name, fields, attributes): printAggregateSpecifier("struct", name, fields, attributes);
			case TUnionDefinition(name, fields, attributes): printAggregateSpecifier("union", name, fields, attributes);
			case TEnumDefinition(name, values, attributes): printEnumSpecifier(name, values, attributes);
			case TAtomic(atomicType, declarator):
				if (declaratorIdentifier(declarator) != null)
					throw "_Atomic(type-name) requires an abstract declarator";
				'_Atomic(${printTypeName(atomicType, declarator)})';
		}
		return joinTokens([printQualifiers(type.qualifiers), spec]);
	}

	/**
		Prints the C declarator grammar tree. `DGroup` is an explicit source-level
		parenthesized declarator, so precedence never has to be reverse-engineered.
	 */
	public function printDeclarator(declarator:CDeclarator):String {
		return switch declarator {
			case DName(null): "";
			case DName(name): identifier(name);
			case DPointer(inner, qualifiers):
				final qualifierText = printQualifiers(qualifiers);
				"*" + (qualifierText == "" ? "" : qualifierText + " ") + printDeclarator(inner);
			case DArray(inner, bound, qualifiers):
				final parts:Array<String> = [];
				final qualifierText = printQualifiers(qualifiers);
				if (qualifierText != "")
					parts.push(qualifierText);
				switch bound {
					case ABIncomplete:
					case ABVariable: parts.push("*");
					case ABFixed(size): parts.push(printExpr(size, 2));
					case ABStaticMinimum(size):
						parts.push("static");
						parts.push(printExpr(size, 2));
				}
				'${printDeclarator(inner)}[${parts.join(" ")}]';
			case DFunction(inner, parameters): '${printDeclarator(inner)}(${printFunctionParameters(parameters)})';
			case DGroup(inner): '(${printDeclarator(inner)})';
			case DAttributed(inner, attributes): joinTokens([printAttributes(attributes), printDeclarator(inner)]);
		}
	}

	function printTypeName(type:CType, declarator:CDeclarator):String {
		return printTypedDeclarator(type, declarator);
	}

	function printFunction(definition:CFunctionDef):String {
		final header = joinTokens([
			printStorage(definition.storage),
			printFunctionSpecifiers(definition.functionSpecifiers),
			printType(definition.returnType),
			printDeclarator(definition.declarator),
			printAttributes(definition.attributes)
		]);
		return header + "\n" + printControlledBody(definition.body);
	}

	function printVariable(variable:CVariableDecl):String {
		final left = joinTokens([
			printStorage(variable.storage),
			printAlignments(variable.alignments),
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
						case DField(name): '.${identifier(name)}';
						case DIndex(index): '[${printExpr(index)}]';
					}).join("");
					(prefix == "" ? "" : prefix + " = ") + printInitializer(item.value);
				});
				'{ ${rendered.join(", ")} }';
		}
	}

	function printAggregate(keyword:String, name:Null<CIdentifier>, fields:Array<CField>, attributes:Array<CAttribute>):String {
		return printAggregateSpecifier(keyword, name, fields, attributes) + ";";
	}

	function printAggregateSpecifier(keyword:String, name:Null<CIdentifier>, fields:Array<CField>, attributes:Array<CAttribute>):String {
		if (fields.length == 0)
			throw 'A complete C11 $keyword definition must contain at least one field';
		final tag = name == null ? keyword : '$keyword ${identifier(name)}';
		final out:Array<String> = [joinTokens([tag, printAttributes(attributes)]) + " {"];
		indent++;
		for (field in fields) {
			final bit = field.bitWidth == null ? "" : ' : ${printExpr(field.bitWidth, 3)}';
			out.push(line(joinTokens([
				printAlignments(field.alignments),
				printTypedDeclarator(field.type, field.declarator),
				printAttributes(field.attributes)
			]) + bit + ";"));
		}
		indent--;
		out.push(line("}"));
		return out.join("\n");
	}

	function printEnum(name:Null<CIdentifier>, values:Array<CEnumerator>, attributes:Array<CAttribute>):String {
		return printEnumSpecifier(name, values, attributes) + ";";
	}

	function printEnumSpecifier(name:Null<CIdentifier>, values:Array<CEnumerator>, attributes:Array<CAttribute>):String {
		if (values.length == 0)
			throw "A complete C11 enum definition must contain at least one enumerator";
		final tag = name == null ? "enum" : 'enum ${identifier(name)}';
		final out:Array<String> = [];
		out.push(joinTokens([tag, printAttributes(attributes)]) + " {");
		indent++;
		for (index in 0...values.length) {
			final value = values[index];
			final assignment = value.value == null ? "" : ' = ${printExpr(value.value, 3)}';
			final comma = index + 1 < values.length ? "," : "";
			out.push(line(joinTokens([identifier(value.name), printAttributes(value.attributes)]) + assignment + comma));
		}
		indent--;
		out.push(line("}"));
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

	function printLabel(labelName:CIdentifier, statement:CStmt):String {
		final previous = indent;
		indent = 0;
		final header = line('${identifier(labelName)}:');
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
		var defaultCount = 0;
		for (caseData in cases) {
			if (!caseData.isDefault && caseData.values.length == 0)
				throw "A C switch case group requires at least one case or default label";
			if (caseData.isDefault)
				defaultCount++;
		}
		if (defaultCount > 1)
			throw "A C switch permits at most one default label";

		final out:Array<String> = [line('switch (${printExpr(value)}) {')];
		indent++;
		for (caseData in cases) {
			if (caseData.isDefault)
				out.push(line("default:"));
			for (caseValue in caseData.values)
				out.push(line('case ${printExpr(caseValue, 3)}:'));
			indent++;
			out.push(printBlock(caseData.body));
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
		if (op == SizeOfExpr)
			return 'sizeof(${printExpr(value)})';

		final operandPrecedence = switch op {
			case PostIncrement | PostDecrement: 15;
			case _: 14;
		}
		final operand = switch op {
			case PreIncrement | PreDecrement: printUnaryExpressionOperand(value);
			case _: printExpr(value, operandPrecedence);
		}
		return switch op {
			case AddressOf: joinUnaryToken("&", operand);
			case Dereference: joinUnaryToken("*", operand);
			case Plus: joinUnaryToken("+", operand);
			case Minus: joinUnaryToken("-", operand);
			case LogicalNot: joinUnaryToken("!", operand);
			case BitwiseNot: joinUnaryToken("~", operand);
			case PreIncrement: joinUnaryToken("++", operand);
			case PreDecrement: joinUnaryToken("--", operand);
			case PostIncrement: '$operand++';
			case PostDecrement: '$operand--';
			case SizeOfExpr: throw "unreachable sizeof expression branch";
		}
	}

	function printBinary(op:CBinaryOp, left:CExpr, right:CExpr):String {
		final p = binaryPrecedence(op);
		if (isAssignment(op))
			return '${printAssignmentLeft(left)} ${binaryToken(op)} ${printExpr(right, p)}';
		if (op == Comma)
			return '${printExpr(left, p)}, ${printExpr(right, p + 1)}';
		return '${printExpr(left, p)} ${binaryToken(op)} ${printExpr(right, p + 1)}';
	}

	function precedence(expr:CExpr):Int {
		return switch expr {
			case EIdentifier(_) | EInt(_) | EFloat(_) | EString(_) | EChar(_) | EBool(_) | ENull | EGenericSelection(_, _) | EOffsetOf(_, _, _): 16;
			case ECall(_, _) | EIndex(_, _) | EMember(_, _, _): 15;
			case EUnary(PostIncrement, _) | EUnary(PostDecrement, _): 15;
			case EUnary(_, _) | ECast(_, _, _) | ESizeOfType(_, _) | EAlignOfType(_, _): 14;
			case EBinary(op, _, _): binaryPrecedence(op);
			case EConditional(_, _, _): 3;
			case ECompoundLiteral(_, _, _): 15;
			case EParen(_): 16;
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

	function isAssignment(op:CBinaryOp):Bool {
		return switch op {
			case Assign | AddAssign | SubtractAssign | MultiplyAssign | DivideAssign | ModuloAssign | ShiftLeftAssign | ShiftRightAssign | BitAndAssign |
				BitXorAssign | BitOrAssign: true;
			case _: false;
		}
	}

	function printAssignmentLeft(expression:CExpr):String
		return isUnaryExpression(expression) ? printExpr(expression) : '(${printExpr(expression)})';

	function printUnaryExpressionOperand(expression:CExpr):String
		return isUnaryExpression(expression) ? printExpr(expression) : '(${printExpr(expression)})';

	/** Mirrors the C11 `unary-expression` grammar category, not lvalue validity. */
	function isUnaryExpression(expression:CExpr):Bool {
		return switch expression {
			case ECast(_, _, _) | EBinary(_, _, _) | EConditional(_, _, _): false;
			case _: true;
		}
	}

	function joinUnaryToken(token:String, operand:String):String {
		final left = token.charAt(token.length - 1);
		final right = operand.charAt(0);
		final separator = left == "+" && right == "+" || left == "-" && right == "-" || left == "&" && right == "&" ? " " : "";
		return token + separator + operand;
	}

	function printGenericSelection(control:CExpr, associations:Array<CGenericAssociation>):String {
		if (associations.length == 0)
			throw "A C11 generic selection requires at least one association";
		var defaultCount = 0;
		final rendered = associations.map(association -> {
			final selector = if (association.type == null) {
				defaultCount++;
				"default";
			} else {
				if (declaratorIdentifier(association.type.declarator) != null)
					throw "A generic association requires an abstract type-name";
				printTypeName(association.type.type, association.type.declarator);
			}
			'$selector: ${printExpr(association.expression, 2)}';
		});
		if (defaultCount > 1)
			throw "A C11 generic selection permits at most one default association";
		return '_Generic(${printExpr(control, 2)}, ${rendered.join(", ")})';
	}

	function printIntegerLiteral(literal:CIntegerLiteral):String {
		final value = switch literal.base {
			case IBDecimal: literal.digits;
			case IBOctal: literal.digits == "0" ? "0" : "0" + literal.digits;
			case IBHexadecimal: "0x" + literal.digits;
		}
		return value + switch literal.suffix {
			case ISNone: "";
			case ISUnsigned: "U";
			case ISLong: "L";
			case ISUnsignedLong: "UL";
			case ISLongLong: "LL";
			case ISUnsignedLongLong: "ULL";
		}
	}

	function printFloatLiteral(literal:CFloatLiteral):String {
		final value = switch literal.representation {
			case FRDecimal(whole, fraction, exponent):
				final significand = (whole == "" ? "0" : whole) + "." + (fraction == "" ? "0" : fraction);
				exponent == null ? significand : significand + "e" + Std.string(exponent);
			case FRHexadecimal(whole, fraction, exponent):
				"0x"
				+ (whole == "" ? "0" : whole.toUpperCase())
				+ "."
				+ (fraction == "" ? "0" : fraction.toUpperCase())
				+ "p"
				+ Std.string(exponent);
		}
		return value + switch literal.suffix {
			case FSNone: "";
			case FSFloat: "F";
			case FSLongDouble: "L";
		}
	}

	function printStorage(storage:Array<CStorage>):String {
		final tokens = storage.map(storageToken);
		ensureUnique(tokens, "storage-class specifier");
		if (storage.length > 1) {
			final validThreadCombination = storage.length == 2
				&& storage.contains(SThreadLocal)
				&& (storage.contains(SExtern) || storage.contains(SStatic));
			if (!validThreadCombination)
				throw "C11 permits multiple storage-class specifiers only for _Thread_local with static or extern";
		}
		final canonical = [STypedef, SExtern, SStatic, SThreadLocal, SAuto, SRegister];
		return canonical.filter(value -> storage.contains(value)).map(storageToken).join(" ");
	}

	function printFunctionSpecifiers(specifiers:Array<CFunctionSpecifier>):String {
		final tokens = specifiers.map(functionSpecifierToken);
		ensureUnique(tokens, "function specifier");
		return [FInline, FNoReturn].filter(value -> specifiers.contains(value)).map(functionSpecifierToken).join(" ");
	}

	function printQualifiers(qualifiers:Array<CQualifier>):String {
		final tokens = qualifiers.map(qualifierToken);
		ensureUnique(tokens, "type qualifier");
		return [QConst, QRestrict, QVolatile, QAtomic].filter(value -> qualifiers.contains(value)).map(qualifierToken).join(" ");
	}

	function printAlignments(alignments:Array<CAlignment>):String {
		return alignments.map(alignment -> switch alignment {
			case AlignExpr(value): '_Alignas(${printExpr(value, 2)})';
			case AlignType(type, declarator):
				if (declaratorIdentifier(declarator) != null)
					throw "_Alignas(type-name) requires an abstract declarator";
				'_Alignas(${printTypeName(type, declarator)})';
		}).join(" ");
	}

	function printAttributes(attributes:Array<CAttribute>):String {
		if (attributes.length == 0)
			return "";
		if (dialect == StrictC11)
			throw "Compiler-extension attributes require an explicit GNU or Clang C11 dialect";
		return '__attribute__((${attributes.map(printAttribute).join(", ")}))';
	}

	function printAttribute(attribute:CAttribute):String {
		return switch attribute {
			case APacked: "packed";
			case AAligned(null): "aligned";
			case AAligned(value): 'aligned(${printExpr(value, 2)})';
			case ASection(name):
				if (name == "")
					throw "C section attribute name must not be empty";
				'section(${quote(name)})';
			case AVisibility(visibility): 'visibility(${quote(visibilityToken(visibility))})';
			case ACallingConvention(CCVectorcall) if (dialect != ClangC11):
				throw "vectorcall requires the Clang C11 extension dialect";
			case ACallingConvention(convention): callingConventionToken(convention);
			case AUsed: "used";
			case AUnused: "unused";
			case AWeak: "weak";
		}
	}

	function printFunctionParameters(parameters:CFunctionParameters):String {
		return switch parameters {
			case FPPrototype(params, variadic):
				if (variadic && params.length == 0)
					throw "A C11 variadic prototype requires at least one named parameter";
				final values = params.map(param -> joinTokens([
					printTypedDeclarator(param.type, param.declarator),
					printAttributes(param.attributes)
				]));
				if (variadic)
					values.push("...");
				if (values.length == 0)
					values.push("void");
				values.join(", ");
			case FPUnspecified: "";
			case FPIdentifierList(names):
				if (names.length == 0)
					throw "A C identifier-list declarator must contain at least one identifier";
				names.map(identifier).join(", ");
		}
	}

	function printNativeInteger(rank:CNativeIntegerRank, signed:Bool):String {
		final rankToken = switch rank {
			case IRShort: "short";
			case IRInt: "int";
			case IRLong: "long";
			case IRLongLong: "long long";
		}
		return signed ? rankToken : "unsigned " + rankToken;
	}

	function isAtomicTypeSpecifier(specifier:CTypeSpec):Bool {
		return switch specifier {
			case TAtomic(_, _): true;
			case _: false;
		}
	}

	function printRealType(realType:CRealType):String {
		return switch realType {
			case RFloat: "float";
			case RDouble: "double";
			case RLongDouble: "long double";
		}
	}

	function storageToken(storage:CStorage):String {
		return switch storage {
			case SAuto: "auto";
			case SExtern: "extern";
			case SStatic: "static";
			case SRegister: "register";
			case SThreadLocal: "_Thread_local";
			case STypedef: "typedef";
		}
	}

	function functionSpecifierToken(specifier:CFunctionSpecifier):String {
		return switch specifier {
			case FInline: "inline";
			case FNoReturn: "_Noreturn";
		}
	}

	function qualifierToken(qualifier:CQualifier):String {
		return switch qualifier {
			case QConst: "const";
			case QRestrict: "restrict";
			case QVolatile: "volatile";
			case QAtomic: "_Atomic";
		}
	}

	function callingConventionToken(convention:CCallingConvention):String {
		return switch convention {
			case CCCdecl: "cdecl";
			case CCStdcall: "stdcall";
			case CCFastcall: "fastcall";
			case CCVectorcall: "vectorcall";
		}
	}

	function visibilityToken(visibility:CVisibility):String {
		return switch visibility {
			case VDefault: "default";
			case VHidden: "hidden";
			case VProtected: "protected";
			case VInternal: "internal";
		}
	}

	function declaratorIdentifier(declarator:CDeclarator):Null<CIdentifier> {
		return switch declarator {
			case DName(name): name;
			case DPointer(inner, _) | DArray(inner, _, _) | DFunction(inner, _) | DGroup(inner) | DAttributed(inner, _): declaratorIdentifier(inner);
		}
	}

	function identifier(value:CIdentifier):String
		return value.value;

	function ensureUnique(values:Array<String>, kind:String):Void {
		final seen:Map<String, Bool> = [];
		for (value in values) {
			if (seen.exists(value))
				throw 'Duplicate C $kind: $value';
			seen.set(value, true);
		}
	}

	function isLineDirectiveDecl(declaration:CDecl):Bool {
		return switch declaration {
			case DLineDirective(_): true;
			case _: false;
		}
	}

	function printLineDirective(directive:CLineDirective):String {
		if (directive.line < 1)
			throw 'C #line number must be positive, got ${directive.line}';
		return directive.file == null ? '#line ${directive.line}' : '#line ${directive.line} ${quote(directive.file)}';
	}

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
		final escaped = new StringBuf();
		for (byte in utf8Bytes(text)) {
			if (byte == 0x3F) {
				escaped.add("\\077"); // Prevent trigraph replacement before comments are removed.
			} else if (byte >= 0x20 && byte < 0x7F) {
				escaped.addChar(byte);
			} else {
				escaped.add("\\" + octalByte(byte));
			}
		}
		var safe = escaped.toString();
		safe = StringTools.replace(safe, "/*", "/ *");
		safe = StringTools.replace(safe, "*/", "* /");
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

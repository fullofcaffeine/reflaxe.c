import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;

/** Exhaustive expression precedence/escaping assertions plus a strict-C11 golden. */
class ExpressionGolden {
	static final printer = new CASTPrinter();

	static function main():Void {
		verifyModelGuards();
		Sys.print(printer.printTranslationUnit(buildUnit()));
	}

	public static function verifyModelGuards():Void {
		verifyFamilyPrecedenceMatrix();
		verifyOperatorsAndGrammarEdges();
		verifyLiteralsAndSourceSafety();
		verifyStatementGuards();
	}

	/**
		Every ordered outer/inner pair among the six precedence-sensitive families.
		Expected text is deliberately literal rather than derived from printer metadata.
	 */
	static function verifyFamilyPrecedenceMatrix():Void {
		final unary = EUnary(Minus, identifier("a"));
		final binary = EBinary(Add, identifier("a"), identifier("b"));
		final conditional = EConditional(identifier("a"), identifier("b"), identifier("c"));
		final call = ECall(identifier("f"), [identifier("a")]);
		final member = EMember(identifier("object"), id("field"), false);
		final castExpression = ECast(i32(), abstractName(), identifier("a"));

		assertExpr("unary/unary", "- -a", EUnary(Minus, unary));
		assertExpr("unary/binary", "-(a + b)", EUnary(Minus, binary));
		assertExpr("unary/conditional", "-(a ? b : c)", EUnary(Minus, conditional));
		assertExpr("unary/call", "-f(a)", EUnary(Minus, call));
		assertExpr("unary/member", "-object.field", EUnary(Minus, member));
		assertExpr("unary/cast", "-(int32_t)a", EUnary(Minus, castExpression));

		assertExpr("binary/unary", "-a + z", EBinary(Add, unary, identifier("z")));
		assertExpr("binary/binary", "a + b + z", EBinary(Add, binary, identifier("z")));
		assertExpr("binary/conditional", "(a ? b : c) + z", EBinary(Add, conditional, identifier("z")));
		assertExpr("binary/call", "f(a) + z", EBinary(Add, call, identifier("z")));
		assertExpr("binary/member", "object.field + z", EBinary(Add, member, identifier("z")));
		assertExpr("binary/cast", "(int32_t)a + z", EBinary(Add, castExpression, identifier("z")));

		assertExpr("conditional/unary", "-a ? b : c", EConditional(unary, identifier("b"), identifier("c")));
		assertExpr("conditional/binary", "a + b ? b : c", EConditional(binary, identifier("b"), identifier("c")));
		assertExpr("conditional/conditional", "(a ? b : c) ? b : c", EConditional(conditional, identifier("b"), identifier("c")));
		assertExpr("conditional/call", "f(a) ? b : c", EConditional(call, identifier("b"), identifier("c")));
		assertExpr("conditional/member", "object.field ? b : c", EConditional(member, identifier("b"), identifier("c")));
		assertExpr("conditional/cast", "(int32_t)a ? b : c", EConditional(castExpression, identifier("b"), identifier("c")));

		assertExpr("call/unary", "(-a)(z)", ECall(unary, [identifier("z")]));
		assertExpr("call/binary", "(a + b)(z)", ECall(binary, [identifier("z")]));
		assertExpr("call/conditional", "(a ? b : c)(z)", ECall(conditional, [identifier("z")]));
		assertExpr("call/call", "f(a)(z)", ECall(call, [identifier("z")]));
		assertExpr("call/member", "object.field(z)", ECall(member, [identifier("z")]));
		assertExpr("call/cast", "((int32_t)a)(z)", ECall(castExpression, [identifier("z")]));

		assertExpr("member/unary", "(-a).field", EMember(unary, id("field"), false));
		assertExpr("member/binary", "(a + b).field", EMember(binary, id("field"), false));
		assertExpr("member/conditional", "(a ? b : c).field", EMember(conditional, id("field"), false));
		assertExpr("member/call", "f(a).field", EMember(call, id("field"), false));
		assertExpr("member/member", "object.field.field", EMember(member, id("field"), false));
		assertExpr("member/cast", "((int32_t)a).field", EMember(castExpression, id("field"), false));

		assertExpr("cast/unary", "(int32_t)-a", ECast(i32(), abstractName(), unary));
		assertExpr("cast/binary", "(int32_t)(a + b)", ECast(i32(), abstractName(), binary));
		assertExpr("cast/conditional", "(int32_t)(a ? b : c)", ECast(i32(), abstractName(), conditional));
		assertExpr("cast/call", "(int32_t)f(a)", ECast(i32(), abstractName(), call));
		assertExpr("cast/member", "(int32_t)object.field", ECast(i32(), abstractName(), member));
		assertExpr("cast/cast", "(int32_t)(int32_t)a", ECast(i32(), abstractName(), castExpression));
	}

	static function verifyOperatorsAndGrammarEdges():Void {
		final a = identifier("a");
		final b = identifier("b");
		final c = identifier("c");

		final unaryCases:Array<{label:String, expected:String, op:CUnaryOp}> = [
			{label: "address", expected: "&a", op: AddressOf},
			{label: "dereference", expected: "*a", op: Dereference},
			{label: "plus", expected: "+a", op: Plus},
			{label: "minus", expected: "-a", op: Minus},
			{label: "logical-not", expected: "!a", op: LogicalNot},
			{label: "bitwise-not", expected: "~a", op: BitwiseNot},
			{label: "pre-increment", expected: "++a", op: PreIncrement},
			{label: "pre-decrement", expected: "--a", op: PreDecrement},
			{label: "post-increment", expected: "a++", op: PostIncrement},
			{label: "post-decrement", expected: "a--", op: PostDecrement},
			{label: "sizeof-expression", expected: "sizeof(a)", op: SizeOfExpr}
		];
		for (test in unaryCases)
			assertExpr('unary operator ${test.label}', test.expected, EUnary(test.op, a));

		final binaryCases:Array<{label:String, expected:String, op:CBinaryOp}> = [
			{label: "multiply", expected: "a * b", op: Multiply},
			{label: "divide", expected: "a / b", op: Divide},
			{label: "modulo", expected: "a % b", op: Modulo},
			{label: "add", expected: "a + b", op: Add},
			{label: "subtract", expected: "a - b", op: Subtract},
			{label: "shift-left", expected: "a << b", op: ShiftLeft},
			{label: "shift-right", expected: "a >> b", op: ShiftRight},
			{label: "less", expected: "a < b", op: Less},
			{label: "less-equal", expected: "a <= b", op: LessEqual},
			{label: "greater", expected: "a > b", op: Greater},
			{label: "greater-equal", expected: "a >= b", op: GreaterEqual},
			{label: "equal", expected: "a == b", op: Equal},
			{label: "not-equal", expected: "a != b", op: NotEqual},
			{label: "bit-and", expected: "a & b", op: BitAnd},
			{label: "bit-xor", expected: "a ^ b", op: BitXor},
			{label: "bit-or", expected: "a | b", op: BitOr},
			{label: "logical-and", expected: "a && b", op: LogicalAnd},
			{label: "logical-or", expected: "a || b", op: LogicalOr},
			{label: "assign", expected: "a = b", op: Assign},
			{label: "add-assign", expected: "a += b", op: AddAssign},
			{label: "subtract-assign", expected: "a -= b", op: SubtractAssign},
			{label: "multiply-assign", expected: "a *= b", op: MultiplyAssign},
			{label: "divide-assign", expected: "a /= b", op: DivideAssign},
			{label: "modulo-assign", expected: "a %= b", op: ModuloAssign},
			{label: "shift-left-assign", expected: "a <<= b", op: ShiftLeftAssign},
			{label: "shift-right-assign", expected: "a >>= b", op: ShiftRightAssign},
			{label: "bit-and-assign", expected: "a &= b", op: BitAndAssign},
			{label: "bit-xor-assign", expected: "a ^= b", op: BitXorAssign},
			{label: "bit-or-assign", expected: "a |= b", op: BitOrAssign},
			{label: "comma", expected: "a, b", op: Comma}
		];
		for (test in binaryCases)
			assertExpr('binary operator ${test.label}', test.expected, EBinary(test.op, a, b));

		// Both directions at every adjacent C11 precedence boundary.
		assertExpr("multiply above add", "a * b + c", EBinary(Add, EBinary(Multiply, a, b), c));
		assertExpr("add below multiply", "(a + b) * c", EBinary(Multiply, EBinary(Add, a, b), c));
		assertExpr("add above shift", "a + b << c", EBinary(ShiftLeft, EBinary(Add, a, b), c));
		assertExpr("shift below add", "(a << b) + c", EBinary(Add, EBinary(ShiftLeft, a, b), c));
		assertExpr("shift above relational", "a << b < c", EBinary(Less, EBinary(ShiftLeft, a, b), c));
		assertExpr("relational below shift", "(a < b) << c", EBinary(ShiftLeft, EBinary(Less, a, b), c));
		assertExpr("relational above equality", "a < b == c", EBinary(Equal, EBinary(Less, a, b), c));
		assertExpr("equality below relational", "(a == b) < c", EBinary(Less, EBinary(Equal, a, b), c));
		assertExpr("equality above bit-and", "a == b & c", EBinary(BitAnd, EBinary(Equal, a, b), c));
		assertExpr("bit-and below equality", "(a & b) == c", EBinary(Equal, EBinary(BitAnd, a, b), c));
		assertExpr("bit-and above bit-xor", "a & b ^ c", EBinary(BitXor, EBinary(BitAnd, a, b), c));
		assertExpr("bit-xor below bit-and", "(a ^ b) & c", EBinary(BitAnd, EBinary(BitXor, a, b), c));
		assertExpr("bit-xor above bit-or", "a ^ b | c", EBinary(BitOr, EBinary(BitXor, a, b), c));
		assertExpr("bit-or below bit-xor", "(a | b) ^ c", EBinary(BitXor, EBinary(BitOr, a, b), c));
		assertExpr("bit-or above logical-and", "a | b && c", EBinary(LogicalAnd, EBinary(BitOr, a, b), c));
		assertExpr("logical-and below bit-or", "(a && b) | c", EBinary(BitOr, EBinary(LogicalAnd, a, b), c));
		assertExpr("logical-and above logical-or", "a && b || c", EBinary(LogicalOr, EBinary(LogicalAnd, a, b), c));
		assertExpr("logical-or below logical-and", "(a || b) && c", EBinary(LogicalAnd, EBinary(LogicalOr, a, b), c));
		assertExpr("logical-or above conditional", "a || b ? c : a", EConditional(EBinary(LogicalOr, a, b), c, a));
		assertExpr("conditional below logical-or", "(a ? b : c) || a", EBinary(LogicalOr, EConditional(a, b, c), a));
		assertExpr("conditional above assignment", "a = b ? c : a", EBinary(Assign, a, EConditional(b, c, a)));
		assertExpr("assignment below conditional", "a ? b : (c = a)", EConditional(a, b, EBinary(Assign, c, a)));
		assertExpr("assignment above comma", "a = b, c", EBinary(Comma, EBinary(Assign, a, b), c));
		assertExpr("comma below assignment", "a = (b, c)", EBinary(Assign, a, EBinary(Comma, b, c)));

		assertExpr("left associativity", "a - b - c", EBinary(Subtract, EBinary(Subtract, a, b), c));
		assertExpr("right nested subtraction", "a - (b - c)", EBinary(Subtract, a, EBinary(Subtract, b, c)));
		assertExpr("right nested multiply", "a * (b / c)", EBinary(Multiply, a, EBinary(Divide, b, c)));
		assertExpr("right nested relational", "a < (b <= c)", EBinary(Less, a, EBinary(LessEqual, b, c)));
		assertExpr("right nested bit-and", "a & (b & c)", EBinary(BitAnd, a, EBinary(BitAnd, b, c)));
		assertExpr("right nested logical-and", "a && (b && c)", EBinary(LogicalAnd, a, EBinary(LogicalAnd, b, c)));
		assertExpr("right nested comma", "a, (b, c)", EBinary(Comma, a, EBinary(Comma, b, c)));
		assertExpr("right associative assignment", "a = b = c", EBinary(Assign, a, EBinary(Assign, b, c)));
		assertExpr("left nested assignment grammar", "(a = b) = c", EBinary(Assign, EBinary(Assign, a, b), c));
		assertExpr("assignment conditional lhs grammar", "(a ? b : c) = a", EBinary(Assign, EConditional(a, b, c), a));
		assertExpr("assignment cast lhs grammar", "((int32_t)a) = b", EBinary(Assign, ECast(i32(), abstractName(), a), b));
		assertExpr("preincrement cast grammar", "++((int32_t)a)", EUnary(PreIncrement, ECast(i32(), abstractName(), a)));
		assertExpr("predecrement binary grammar", "--(a + b)", EUnary(PreDecrement, EBinary(Add, a, b)));
		assertExpr("postincrement cast grammar", "((int32_t)a)++", EUnary(PostIncrement, ECast(i32(), abstractName(), a)));
		assertExpr("postdecrement conditional grammar", "(a ? b : c)--", EUnary(PostDecrement, EConditional(a, b, c)));
		assertExpr("nested unary minus token boundary", "- -a", EUnary(Minus, EUnary(Minus, a)));
		assertExpr("nested unary plus token boundary", "+ +a", EUnary(Plus, EUnary(Plus, a)));
		assertExpr("nested address token boundary", "& &a", EUnary(AddressOf, EUnary(AddressOf, a)));
		assertExpr("preincrement plus token boundary", "++ +a", EUnary(PreIncrement, EUnary(Plus, a)));
		assertExpr("predecrement minus token boundary", "-- -a", EUnary(PreDecrement, EUnary(Minus, a)));
		assertExpr("sizeof expression grouping", "sizeof(a + b)", EUnary(SizeOfExpr, EBinary(Add, a, b)));
		assertExpr("call comma argument", "f((a, b))", ECall(identifier("f"), [EBinary(Comma, a, b)]));
		assertExpr("conditional comma middle", "a ? b, c : a", EConditional(a, EBinary(Comma, b, c), a));
		assertExpr("conditional assignment else", "a ? b : (c = a)", EConditional(a, b, EBinary(Assign, c, a)));
		assertExpr("conditional in conditional else", "a ? b : b ? c : a", EConditional(a, b, EConditional(b, c, a)));
		assertExpr("explicit parentheses", "(a + b)", EParen(EBinary(Add, a, b)));
	}

	static function verifyLiteralsAndSourceSafety():Void {
		assertExpr("decimal integer", "42", EInt(new CIntegerLiteral(IBDecimal, "42")));
		assertExpr("octal integer", "017", EInt(new CIntegerLiteral(IBOctal, "17")));
		assertExpr("hex integer", "0xDEADULL", EInt(new CIntegerLiteral(IBHexadecimal, "dead", ISUnsignedLongLong)));
		assertExpr("unsigned integer", "9U", EInt(CIntegerLiteral.decimal("9", ISUnsigned)));
		assertExpr("long integer", "9L", EInt(CIntegerLiteral.decimal("9", ISLong)));
		assertExpr("unsigned long integer", "9UL", EInt(CIntegerLiteral.decimal("9", ISUnsignedLong)));
		assertExpr("long long integer", "9LL", EInt(CIntegerLiteral.decimal("9", ISLongLong)));
		assertExpr("decimal float", "12.5", EFloat(new CFloatLiteral(FRDecimal("12", "5", null))));
		assertExpr("fractional float", "0.25e-3F", EFloat(new CFloatLiteral(FRDecimal("", "25", -3), FSFloat)));
		assertExpr("whole long double", "7.0e2L", EFloat(new CFloatLiteral(FRDecimal("7", "", 2), FSLongDouble)));
		assertExpr("hex float", "0xA.Fp-2", EFloat(new CFloatLiteral(FRHexadecimal("a", "f", -2))));

		expectFailure("empty integer", () -> new CIntegerLiteral(IBDecimal, ""));
		expectFailure("decimal leading zero", () -> new CIntegerLiteral(IBDecimal, "01"));
		expectFailure("invalid octal digit", () -> new CIntegerLiteral(IBOctal, "8"));
		expectFailure("invalid hex digit", () -> new CIntegerLiteral(IBHexadecimal, "G"));
		expectFailure("empty float", () -> new CFloatLiteral(FRDecimal("", "", null)));
		expectFailure("invalid decimal float", () -> new CFloatLiteral(FRDecimal("1", "x", null)));
		expectFailure("invalid hex float", () -> new CFloatLiteral(FRHexadecimal("g", "0", 0)));

		final adversarial = 'quote" slash\\ question? bell' + String.fromCharCode(7) + ' nul' + String.fromCharCode(0) + ' é🙂';
		assertExpr("string escaping", '"quote\\" slash\\\\ question\\? bell\\a nul\\000 \\303\\251\\360\\237\\231\\202"', EString(adversarial));
		assertExpr("printable character", "'A'", EChar(0x41));
		assertExpr("quote character", "'\\''", EChar(0x27));
		assertExpr("backslash character", "'\\\\'", EChar(0x5C));
		assertExpr("question character", "'\\?'", EChar(0x3F));
		assertExpr("newline character", "'\\n'", EChar(0x0A));
		assertExpr("nul character", "'\\000'", EChar(0));
		assertExpr("byte character", "'\\377'", EChar(0xFF));
		expectFailure("negative character", () -> printer.printExpr(EChar(-1)));
		expectFailure("wide character", () -> printer.printExpr(EChar(0x100)));
		expectFailure("unpaired high surrogate", () -> printer.printExpr(EString(String.fromCharCode(0xD800))));
		expectFailure("unpaired low surrogate", () -> printer.printExpr(EString(String.fromCharCode(0xDC00))));

		assertEqual("comment escaping", "/* open / * close * / questions \\077\\077/\\012\\303\\251 */",
			printer.printDecl(DComment("open /* close */ questions ??/\né")));
		assertEqual("line filename escaping", '#line 23 "src/\\"quoted\\"\\\\questions\\?\\?.hx"',
			printer.printDecl(DLineDirective({line: 23, file: 'src/"quoted"\\questions??.hx'})));
		assertEqual("line without filename", "#line 24", printer.printDecl(DLineDirective({line: 24, file: null})));

		expectFailure("keyword identifier", () -> new CIdentifier("switch"));
		expectFailure("empty identifier", () -> new CIdentifier(""));
		expectFailure("non-ASCII identifier", () -> new CIdentifier("café"));
		expectFailure("identifier token injection", () -> new CIdentifier("value; return 1"));
		expectFailure("zero line", () -> printer.printDecl(DLineDirective({line: 0, file: null})));

		final generic = EGenericSelection(identifier("value"), [
			{type: typedName(i32()), expression: integer("1")},
			{type: null, expression: integer("0")}
		]);
		assertExpr("generic selection", "_Generic(value, int32_t: 1, default: 0)", generic);
		expectFailure("empty generic selection", () -> printer.printExpr(EGenericSelection(identifier("value"), [])));
		expectFailure("duplicate generic default",
			() -> printer.printExpr(EGenericSelection(identifier("value"), [{type: null, expression: integer("0")}, {type: null, expression: integer("1")}])));
		expectFailure("named generic type", () -> printer.printExpr(EGenericSelection(identifier("value"), [
			{type: {type: i32(), declarator: name("not_abstract")}, expression: integer("1")}
		])));
	}

	static function verifyStatementGuards():Void {
		final mapped = new CTranslationUnit();
		mapped.declarations.push(DLineDirective({line: 17, file: null}));
		mapped.declarations.push(DStaticAssert(EBinary(Equal, identifier("__LINE__"), integer("17")), "mapped"));
		assertEqual("line applies to immediate declaration", '#line 17\n_Static_assert(__LINE__ == 17, "mapped");\n', printer.printTranslationUnit(mapped));

		assertEqual("statement line starts in preprocessing column", '{\n#line 25 "mapped.hx"\n  int32_t observed = __LINE__;\n}', printer.printStmt(SBlock([
			SLineDirective({
				line: 25,
				file: "mapped.hx"
			}),
			SDecl(variable(i32(), name("observed"), IExpr(identifier("__LINE__"))))
		])));

		expectFailure("case without label", () -> printer.printStmt(SSwitch(identifier("value"), [{values: [], isDefault: false, body: [SBreak]}])));
		expectFailure("duplicate switch default", () -> printer.printStmt(SSwitch(identifier("value"), [
			{values: [], isDefault: true, body: [SBreak]},
			{values: [], isDefault: true, body: [SBreak]}
		])));
	}

	public static function buildUnit():CTranslationUnit {
		final unit = new CTranslationUnit();
		unit.includes.push({path: "stdio.h", kind: System});
		unit.includes.push({path: "stdint.h", kind: System});
		unit.includes.push({path: "stddef.h", kind: System});
		unit.includes.push({path: "stdbool.h", kind: System});

		unit.declarations.push(DComment("Source-safe /* comments */ neutralize ??/ trigraphs\nand UTF-8 é."));
		unit.declarations.push(DStruct(id("hxc_pair"), [field(i32(), name("left")), field(i32(), name("right"))], []));
		unit.declarations.push(DFunction({
			storage: [SStatic],
			functionSpecifiers: [],
			returnType: i32(),
			declarator: DFunction(name("identity"), FPPrototype([param(i32(), name("value"))], false)),
			body: SBlock([SReturn(identifier("value"))]),
			attributes: []
		}));
		unit.declarations.push(DLineDirective({line: 200, file: 'haxe/source"\\questions??.hx'}));
		unit.declarations.push(DStaticAssert(EBinary(Equal, identifier("__LINE__"), integer("200")), "declaration line mapping is immediate"));
		unit.declarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: nativeInt(),
			declarator: DFunction(name("main"), FPPrototype([], false)),
			body: buildMainBody(),
			attributes: []
		}));
		return unit;
	}

	static function buildMainBody():CStmt {
		final a = identifier("a");
		final b = identifier("b");
		final c = identifier("c");
		final scratch = identifier("scratch");
		final pair = identifier("pair");
		final values = identifier("values");
		final statements:Array<CStmt> = [
			SDecl(variable(i32(), name("a"), IExpr(integer("8")))),
			SDecl(variable(i32(), name("b"), IExpr(integer("3")))),
			SDecl(variable(i32(), name("c"), IExpr(integer("1")))),
			SDecl(variable(i32(), name("scratch"), IExpr(integer("0")))),
			SDecl(variable(type(TStruct(id("hxc_pair"))), name("pair"),
				IList([
					{
						designators: [DField(id("left"))],
						value: IExpr(integer("2"))
					},
					{designators: [DField(id("right"))], value: IExpr(integer("4"))}
				]))),
			SDecl(variable(type(TStruct(id("hxc_pair"))), DPointer(name("pair_ptr"), []), IExpr(EUnary(AddressOf, pair)))),
			SDecl(variable(i32(), DArray(name("values"), ABFixed(integer("2")), []),
				IList([
					{
						designators: [],
						value: IExpr(integer("1"))
					},
					{designators: [], value: IExpr(integer("2"))}
				]))),
			SComment("statement comment closes */ and opens /* without leaking ??/"),
			SLineDirective({line: 400, file: 'haxe/main"\\questions??.hx'}),
			SDecl(variable(i32(), name("mapped_line"), IExpr(identifier("__LINE__")))),
			SIf(EBinary(NotEqual, identifier("mapped_line"), integer("400")), SReturn(integer("10")), null),
			SDecl(variable(type(TChar(null), [QConst]), DPointer(name("escaped"), []),
				IExpr(EString('quote" slash\\ question? tab\t newline\n nul' + String.fromCharCode(0) + ' é🙂')))),
			SIf(EBinary(NotEqual, EIndex(identifier("escaped"), integer("0")), EChar(0x71)), SReturn(integer("11")), null),
			SIf(EBinary(NotEqual, ECall(identifier("identity"), [a]), integer("8")), SReturn(integer("12")), null),
			SIf(EBinary(NotEqual, EMember(identifier("pair_ptr"), id("right"), true), integer("4")), SReturn(integer("13")), null),
			SIf(EBinary(NotEqual, EBinary(Add, EIndex(values, integer("0")), EIndex(values, integer("1"))), integer("3")), SReturn(integer("14")), null),
			SIf(EBinary(NotEqual, EConditional(EBinary(Greater, a, b), a, b), integer("8")), SReturn(integer("15")), null),
			SIf(EBinary(NotEqual, EGenericSelection(a, [
				{
					type: typedName(i32()),
					expression: integer("1")
				},
				{type: null, expression: integer("0")}
			]),
				integer("1")), SReturn(integer("16")),
				null),
			SIf(EBinary(NotEqual, EMember(ECompoundLiteral(type(TStruct(id("hxc_pair"))), abstractName(),
				IList([
					{
						designators: [DField(id("left"))],
						value: IExpr(integer("6"))
					},
					{designators: [DField(id("right"))], value: IExpr(integer("7"))}
				])),
				id("right"), false),
				integer("7")),
				SReturn(integer("17")), null),
			SIf(EBinary(Equal, EUnary(SizeOfExpr, EBinary(Add, a, b)), integer("0")), SReturn(integer("18")), null)
		];

		for (expression in sourceLeafExpressions())
			statements.push(discard(expression));
		for (expression in unaryExecutionExpressions(scratch, b))
			statements.push(discard(expression));
		for (expression in binaryExecutionExpressions(a, b, c, scratch))
			statements.push(discard(expression));

		statements.push(SExpr(EBinary(Assign, scratch, integer("0"))));
		statements.push(SIf(EBinary(Greater, a, b), SExpr(EBinary(Assign, scratch, a)), SReturn(integer("20"))));
		statements.push(SWhile(EBinary(Less, c, integer("2")), SBlock([SExpr(EUnary(PreIncrement, c))])));
		statements.push(SDoWhile(SBlock([SExpr(EUnary(PreDecrement, c))]), EBinary(Greater, c, integer("1"))));
		statements.push(SFor(SDecl(variable(i32(), name("index"), IExpr(integer("0")))), EBinary(Less, identifier("index"), integer("2")),
			EUnary(PreIncrement, identifier("index")), SBlock([
				SIf(EBinary(Equal, identifier("index"), integer("0")), SContinue, null),
				SExpr(EBinary(AddAssign, scratch, EIndex(values, identifier("index"))))
			])));
		statements.push(SSwitch(c, [
			{
				values: [integer("1")],
				isDefault: false,
				body: [
					SDecl(variable(i32(), name("local"), IExpr(EMember(pair, id("left"), false)))),
					SExpr(EBinary(AddAssign, scratch, identifier("local"))),
					SBreak
				]
			},
			{values: [], isDefault: true, body: [SReturn(integer("30"))]}
		]));
		statements.push(SIf(EBinary(NotEqual, scratch, integer("12")), SReturn(integer("40")), null));
		statements.push(SGoto(id("hxc_done")));
		statements.push(SLabel(id("hxc_done"), SEmpty));
		statements.push(SExpr(ECall(identifier("puts"), [EString("c-expression-golden: OK")])));
		statements.push(SReturn(integer("0")));
		return SBlock(statements);
	}

	static function sourceLeafExpressions():Array<CExpr> {
		return [
			EFloat(new CFloatLiteral(FRDecimal("1", "25", null), FSFloat)),
			EChar(0x27),
			EChar(0x5C),
			EChar(0x3F),
			EChar(0),
			EBool(true),
			ENull,
			EAlignOfType(i32(), abstractName()),
			ESizeOfType(i32(), abstractName())
		];
	}

	static function unaryExecutionExpressions(scratch:CExpr, b:CExpr):Array<CExpr> {
		return [
			EUnary(AddressOf, scratch),
			EUnary(Dereference, EUnary(AddressOf, scratch)),
			EUnary(Plus, scratch),
			EUnary(Minus, scratch),
			EUnary(LogicalNot, scratch),
			EUnary(BitwiseNot, scratch),
			EUnary(PreIncrement, scratch),
			EUnary(PreDecrement, scratch),
			EUnary(PostIncrement, scratch),
			EUnary(PostDecrement, scratch),
			EUnary(SizeOfExpr, EBinary(Add, scratch, b))
		];
	}

	static function binaryExecutionExpressions(a:CExpr, b:CExpr, c:CExpr, scratch:CExpr):Array<CExpr> {
		return [
			EBinary(Multiply, a, b),
			EBinary(Divide, a, b),
			EBinary(Modulo, a, b),
			EBinary(Add, a, b),
			EBinary(Subtract, a, b),
			EBinary(ShiftLeft, a, c),
			EBinary(ShiftRight, a, c),
			EBinary(Less, a, b),
			EBinary(LessEqual, a, b),
			EBinary(Greater, a, b),
			EBinary(GreaterEqual, a, b),
			EBinary(Equal, a, b),
			EBinary(NotEqual, a, b),
			EBinary(BitAnd, a, b),
			EBinary(BitXor, a, b),
			EBinary(BitOr, a, b),
			EBinary(LogicalAnd, a, b),
			EBinary(LogicalOr, a, b),
			EBinary(Assign, scratch, a),
			EBinary(AddAssign, scratch, b),
			EBinary(SubtractAssign, scratch, c),
			EBinary(MultiplyAssign, scratch, b),
			EBinary(DivideAssign, scratch, b),
			EBinary(ModuloAssign, scratch, b),
			EBinary(ShiftLeftAssign, scratch, c),
			EBinary(ShiftRightAssign, scratch, c),
			EBinary(BitAndAssign, scratch, a),
			EBinary(BitXorAssign, scratch, b),
			EBinary(BitOrAssign, scratch, c),
			EBinary(Comma, EBinary(Assign, scratch, a), b),
			EConditional(EBinary(Greater, a, b), a, b),
			ECall(identifier("identity"), [a]),
			EIndex(identifier("values"), integer("0")),
			EMember(identifier("pair"), id("left"), false),
			ECast(i32(), abstractName(), a),
			EParen(EBinary(Add, a, b))
		];
	}

	static function discard(expression:CExpr):CStmt
		return SExpr(ECast(type(TVoid), abstractName(), expression));

	static function assertExpr(label:String, expected:String, expression:CExpr):Void
		assertEqual(label, expected, printer.printExpr(expression));

	static function assertEqual(label:String, expected:String, actual:String):Void {
		if (actual != expected)
			throw '$label failed\nexpected: $expected\nactual:   $actual';
	}

	static function expectFailure<T>(label:String, action:() -> T):Void {
		var failed = false;
		try {
			action();
		} catch (_:haxe.Exception) {
			failed = true;
		}
		if (!failed)
			throw 'Expected C AST failure: $label';
	}

	static function id(value:String):CIdentifier
		return new CIdentifier(value);

	static function identifier(value:String):CExpr
		return EIdentifier(id(value));

	static function type(spec:CTypeSpec, ?qualifiers:Array<CQualifier>):CType
		return new CType(spec, qualifiers);

	static function i32():CType
		return type(TInt(32, true));

	static function nativeInt():CType
		return type(TNativeInt(IRInt, true));

	static function integer(value:String):CExpr
		return EInt(CIntegerLiteral.decimal(value));

	static function name(value:String):CDeclarator
		return DName(id(value));

	static function abstractName():CDeclarator
		return DName(null);

	static function typedName(type:CType):CTypedDeclarator
		return {type: type, declarator: abstractName()};

	static function variable(type:CType, declarator:CDeclarator, initializer:CInitializer):CVariableDecl
		return {
			storage: [],
			alignments: [],
			type: type,
			declarator: declarator,
			initializer: initializer,
			attributes: []
		};

	static function param(type:CType, declarator:CDeclarator):CParam
		return {type: type, declarator: declarator, attributes: []};

	static function field(type:CType, declarator:CDeclarator):CField
		return {
			type: type,
			declarator: declarator,
			bitWidth: null,
			alignments: [],
			attributes: []
		};
}

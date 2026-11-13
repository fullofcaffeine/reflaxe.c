import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;

/**
	Protects statement bytes and public override dispatch while buffers are shared.
	Expectations are authored C text and visit traces, not another printer implementation.
	The existing ExpressionGolden native corpus checks nested control flow and scalar results.
 */
class StatementGolden {
	/** Exercises every recursive statement entry without changing the emitted corpus. */
	public static function verify():Void {
		if (!new ConstructorIdentity().exact || new DerivedConstructorIdentity().exact)
			throw "base constructor must observe the actual instance class";
		check(SBlock([SEmpty]), "{\n  ;\n}", "{\n  ;~\n}~", "+SBlock,+SEmpty,-SEmpty,-SBlock");
		check(SIf(expr("a"), SBlock([SEmpty]), SEmpty), "if (a)\n{\n  ;\n}\nelse\n{\n  ;\n}", "if (a)\n{\n  ;~\n}~\nelse\n{\n  ;~\n}~",
			"+SIf,expr:a,+SBlock,+SEmpty,-SEmpty,-SBlock,+SEmpty,-SEmpty,-SIf");
		check(SWhile(expr("a"), SEmpty), "while (a)\n{\n  ;\n}", "while (a)\n{\n  ;~\n}~", "+SWhile,expr:a,+SEmpty,-SEmpty,-SWhile");
		check(SDoWhile(SExpr(expr("body")), expr("condition")), "do\n{\n  body;\n}\nwhile (condition);", "do\n{\n  body;~\n}\nwhile (condition);~",
			"+SDoWhile,+SExpr,expr:body,-SExpr,expr:condition,-SDoWhile");
		check(SFor(SExpr(expr("init")), expr("condition"), expr("step"), SEmpty), "for (init; condition; step)\n{\n  ;\n}",
			"for (init; condition; step)\n{\n  ;~\n}~", "+SFor,expr:init,expr:condition,expr:step,+SEmpty,-SEmpty,-SFor");
		check(SSwitch(expr("selector"), [{values: [expr("key")], isDefault: true, body: [SBreak]}]),
			"switch (selector) {\n  default:\n  case key:\n    {\n      break;\n    }\n}",
			"switch (selector) {\n  default:\n  case key:\n    {\n      break;~\n    }\n}~", "+SSwitch,expr:selector,expr:key,+SBreak,-SBreak,-SSwitch");
		check(SLabel(id("done"), SEmpty), "done:\n  ;", "done:\n  ;~~", "+SLabel,+SEmpty,-SEmpty,-SLabel");
		check(SLabel(id("decl"), SDecl(variable())), "decl:\n  {\n    int value;\n  }", "decl:\n  {\n    int value;~\n  }~", "+SLabel,+SDecl,-SDecl,-SLabel");
		check(SBlock([SLineDirective({line: 17, file: "source.hx"}), SEmpty]), "{\n#line 17 \"source.hx\"\n  ;\n}", "{\n#line 17 \"source.hx\"~\n  ;~\n}~",
			"+SBlock,+SLineDirective,-SLineDirective,+SEmpty,-SEmpty,-SBlock");

		final nested = SBlock([SIf(expr("a"), SWhile(expr("b"), SBlock([SReturn(null)])), SEmpty)]);
		equal("{\n  if (a)\n  {\n    while (b)\n    {\n      return;\n    }\n  }\n  else\n  {\n    ;\n  }\n}", new CASTPrinter().printStmt(nested));
		final skipping = new StatementObserver(true);
		equal("SKIP", skipping.printStmt(nested));
		equal("+SBlock,skip", skipping.events.join(","));

		// Invalid groups must fail before the selector's public expression override.
		invalidSwitch([{values: [], isDefault: false, body: []}], "A C switch case group requires at least one case or default label");
		invalidSwitch([{values: [], isDefault: true, body: []}, {values: [], isDefault: true, body: []}], "A C switch permits at most one default label");
		final failing = new StatementObserver(false, "body");
		var caught = "";
		try {
			failing.printStmt(SDoWhile(SExpr(expr("body")), expr("condition")));
		} catch (error:haxe.Exception) {
			caught = error.message;
		}
		equal("expression rejected: body", caught);
		equal("+SDoWhile,+SExpr,expr:body", failing.events.join(","));

		final observer = new StatementObserver();
		final unit = functionUnit(SBlock([SEmpty]));
		equal("int sample(void)\n{\n  ;~\n}~\n/*decl*/\n", observer.printTranslationUnit(unit));
		equal("decl,+SBlock,+SEmpty,-SEmpty,-SBlock", observer.events.join(","));
		final ordinary = new CASTPrinter();
		equal("int sample(void)\n{\n  ;\n}\n", ordinary.printTranslationUnit(unit));
		equal("int sample(void)\n{\n  ;\n}\n", ordinary.printTranslationUnit(unit));
	}

	#if (reflaxe_c_phase_timing && (macro || reflaxe_runtime))
	/** Synthetic braces and intercepted statements must not inflate visit counts. */
	public static function verifyWork():Void {
		final unit = functionUnit(SBlock([
			SIf(expr("a"), SEmpty, SBlock([SReturn(null)])),
			SSwitch(expr("b"), [{values: [], isDefault: true, body: [SEmpty]}]),
			SLabel(id("decl"), SDecl(variable()))
		]));
		final printers:Array<CASTPrinter> = [new CASTPrinter(), new StatementObserver()];
		for (printer in printers) {
			final printed = printer.printTranslationUnitWithWork(unit);
			if (printed.work.statementCount != 9 || printed.work.expressionCount != 2 || printed.work.declarationCount != 1)
				throw "compound statement visits changed";
		}
		final skipped = new StatementObserver(true).printTranslationUnitWithWork(unit);
		if (skipped.work.statementCount != 0 || skipped.work.expressionCount != 0 || skipped.work.declarationCount != 1)
			throw "override without super must not count or visit skipped statements";
	}
	#end

	static function check(stmt:CStmt, ordinary:String, overridden:String, events:String):Void {
		equal(ordinary, new CASTPrinter().printStmt(stmt));
		final observer = new StatementObserver();
		equal(overridden, observer.printStmt(stmt));
		equal(events, observer.events.join(","));
	}

	static function invalidSwitch(cases:Array<CCase>, expected:String):Void {
		final observer = new StatementObserver();
		var caught = "";
		try {
			observer.printStmt(SSwitch(expr("selector"), cases));
		} catch (error:haxe.Exception) {
			caught = error.message;
		}
		equal(expected, caught);
		equal("+SSwitch", observer.events.join(","));
	}

	static function functionUnit(body:CStmt):CTranslationUnit {
		final unit = new CTranslationUnit();
		unit.declarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNativeInt(IRInt, true)),
			declarator: DFunction(DName(id("sample")), FPPrototype([], false)),
			body: body,
			attributes: []
		}));
		return unit;
	}

	static function variable():CVariableDecl
		return {
			storage: [],
			alignments: [],
			type: new CType(TNativeInt(IRInt, true)),
			declarator: DName(id("value")),
			initializer: null,
			attributes: []
		};

	static function expr(value:String):CExpr
		return EIdentifier(id(value));

	static function id(value:String):CIdentifier
		return new CIdentifier(value);

	static function equal(expected:String, actual:String):Void {
		if (expected != actual)
			throw 'statement contract mismatch\nexpected: $expected\nactual: $actual';
	}
}

/** Observes virtual dispatch and deliberately changes returned text at public boundaries. */
private class StatementObserver extends CASTPrinter {
	public final events:Array<String> = [];

	final skipBlocks:Bool;
	final rejectExpression:Null<String>;

	public function new(skipBlocks:Bool = false, ?rejectExpression:String) {
		super();
		this.skipBlocks = skipBlocks;
		this.rejectExpression = rejectExpression;
	}

	override public function printStmt(stmt:CStmt):String {
		final name = Type.enumConstructor(stmt);
		events.push("+" + name);
		switch stmt {
			case SBlock(_) if (skipBlocks):
				events.push("skip");
				return "SKIP";
			case _:
		}
		final text = super.printStmt(stmt);
		events.push("-" + name);
		return text + "~";
	}

	override public function printExpr(expr:CExpr, parentPrecedence:Int = 0):String {
		switch expr {
			case EIdentifier(name):
				events.push("expr:" + name.value);
				if (rejectExpression == name.value)
					throw "expression rejected: " + name.value;
			case _:
		}
		return super.printExpr(expr, parentPrecedence);
	}

	override public function printDecl(decl:CDecl):String {
		events.push("decl");
		return super.printDecl(decl) + "\n/*decl*/";
	}
}

/** Proves the public class-identity API used by the constructor fast-path decision. */
private class ConstructorIdentity {
	public final exact:Bool;

	public function new() {
		exact = Type.getClass(this) == ConstructorIdentity;
	}
}

/** The base constructor must see this derived identity before its own body returns. */
private class DerivedConstructorIdentity extends ConstructorIdentity {
	public function new() {
		super();
	}
}

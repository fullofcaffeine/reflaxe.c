#if macro
import haxe.Json;
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.CProfile;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.CStaticFunctionProjectEmitter;
import reflaxe.c.ir.HxcIRDumper;
import reflaxe.c.lowering.CBodyLowering;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.plan.CDeclarationPlanner;

typedef ArithmeticSourceRecord = {
	final path:String;
	final content:String;
}

typedef ArithmeticSemanticsReport = {
	final schemaVersion:Int;
	final status:String;
	final profile:String;
	final hxcir:String;
	final header:String;
	final sources:Array<ArithmeticSourceRecord>;
	final helpers:Array<String>;
	final buildFacts:Array<reflaxe.c.contract.TypedCContract.TypedCBuildFact>;
	final symbols:reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
	final runtimeFeatures:Array<String>;
}
#end

class ArithmeticSemanticsProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_ARITHMETIC_SEMANTICS=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("ArithmeticFixture") {
			case TInst(reference, _): reference.get();
			case _: fatal("ArithmeticFixture must remain a class", Context.currentPos());
		};
		final inputs:Array<CBodyFunctionInput> = [];
		for (sourceOrder => field in fixture.statics.get()) {
			switch field.kind {
				case FMethod(_):
					final expression = switch field.expr() {
						case null: fatal('arithmetic function `${field.name}` has no typed expression', field.pos);
						case value: value;
					};
					inputs.push({
						modulePath: fixture.module,
						declarationPath: fixture.pack.concat([fixture.name]).join("."),
						sourcePath: "test/arithmetic_semantics/fixtures/ArithmeticFixture.hx",
						fieldName: field.name,
						sourceOrder: sourceOrder,
						fieldType: field.type,
						expression: expression
					});
				case FVar(_, _):
			}
		}
		if (Context.defined("arithmetic_semantics_reverse_input")) {
			inputs.reverse();
		}
		final profile = Context.definedValue("arithmetic_semantics_profile") == "metal" ? CProfile.Metal : CProfile.Portable;
		final context = new CompilationContext(profile);
		final entryId = CBodyLowering.functionId("ArithmeticFixture", "main");
		final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", entryId], CNSOrdinary("translation-unit"),
			CSVInternal, "main");
		final guardRequest = new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal,
			CDeclarationPlanner.headerGuardFor(CStaticFunctionProjectEmitter.HEADER_PATH));
		context.symbols.register(entryRequest);
		context.symbols.register(guardRequest);
		final lowered = new CBodyLowering(context).lower(inputs);
		final project = new CStaticFunctionProjectEmitter().plan(lowered, entryId, context.symbols.identifierFor(entryRequest),
			context.symbols.identifierFor(guardRequest));
		final printer = new CASTPrinter();
		final helperIds = lowered.helpers.map(helper -> helper.helperId);
		helperIds.sort(compareStrings);
		final report:ArithmeticSemanticsReport = {
			schemaVersion: 1,
			status: "typed-ub-safe-arithmetic-runtime-free",
			profile: Std.string(profile),
			hxcir: new HxcIRDumper().dump(lowered.program),
			header: printer.printHeader(project.header),
			sources: project.sources.map(source -> {
				path: source.path,
				content: printer.printTranslationUnit(source.unit)
			}),
			helpers: helperIds,
			buildFacts: lowered.buildFacts,
			symbols: lowered.symbolTable,
			runtimeFeatures: []
		};
		Sys.println(REPORT_PREFIX + Json.stringify(report));
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);

	static function fatal<T>(message:String, position:haxe.macro.Expr.Position):T {
		Context.fatalError(message, position);
		throw new haxe.Exception("Context.fatalError returned after an arithmetic fixture diagnostic");
	}
	#else
	public static function install():Void {}
	#end
}

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

typedef FunctionLoweringRecord = {
	final field:String;
	final irId:String;
	final cName:String;
	final parameters:Array<String>;
	final temporaries:Array<String>;
}

typedef FunctionLoweringReport = {
	final schemaVersion:Int;
	final status:String;
	final profile:String;
	final hxcir:String;
	final header:String;
	final source:String;
	final functions:Array<FunctionLoweringRecord>;
	final symbols:reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
	final runtimeFeatures:Array<String>;
	final runtimeArtifacts:Array<String>;
}
#end

class FunctionLoweringProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_FUNCTION_LOWERING=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("FunctionFixture") {
			case TInst(reference, _): reference.get();
			case _: fatal("FunctionFixture must remain a class", Context.currentPos());
		};
		final inputs:Array<CBodyFunctionInput> = [];
		for (sourceOrder => field in fixture.statics.get()) {
			final expression = switch field.expr() {
				case null: fatal('function fixture `${field.name}` has no typed expression', field.pos);
				case value: value;
			};
			inputs.push({
				modulePath: fixture.module,
				declarationPath: fixture.pack.concat([fixture.name]).join("."),
				sourcePath: "test/function_lowering/fixtures/positive/FunctionFixture.hx",
				fieldName: field.name,
				sourceOrder: sourceOrder,
				fieldType: field.type,
				expression: expression
			});
		}
		if (Context.defined("function_lowering_reverse_input")) {
			inputs.reverse();
		}
		final profile = Context.definedValue("function_lowering_profile") == "metal" ? CProfile.Metal : CProfile.Portable;
		final context = new CompilationContext(profile);
		final entryId = CBodyLowering.functionId("FunctionFixture", "main");
		final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", entryId], CNSOrdinary("translation-unit"),
			CSVInternal, "main");
		final guardRequest = new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal,
			CDeclarationPlanner.headerGuardFor(CStaticFunctionProjectEmitter.HEADER_PATH));
		context.symbols.register(entryRequest);
		context.symbols.register(guardRequest);
		final lowered = new CBodyLowering(context).lower(inputs);
		final project = new CStaticFunctionProjectEmitter().plan(lowered, entryId, context.symbols.identifierFor(entryRequest),
			context.symbols.identifierFor(guardRequest));
		final records:Array<FunctionLoweringRecord> = [];
		for (fn in lowered.functions) {
			final parameters = [for (name in fn.parameterNames) name.value];
			parameters.sort(compareStrings);
			final temporaries = [for (name in fn.temporaryNames) name.value];
			temporaries.sort(compareStrings);
			records.push({
				field: fn.fieldName,
				irId: fn.ir.id,
				cName: fn.cName.value,
				parameters: parameters,
				temporaries: temporaries
			});
		}
		records.sort((left, right) -> compareStrings(left.field, right.field));
		final printer = new CASTPrinter();
		final report:FunctionLoweringReport = {
			schemaVersion: 1,
			status: "typed-static-functions-direct-calls-runtime-free",
			profile: Std.string(profile),
			hxcir: new HxcIRDumper().dump(lowered.program),
			header: printer.printHeader(project.header),
			source: printer.printTranslationUnit(project.source),
			functions: records,
			symbols: lowered.symbolTable,
			runtimeFeatures: [],
			runtimeArtifacts: []
		};
		Sys.println(REPORT_PREFIX + Json.stringify(report));
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);

	static function fatal<T>(message:String, position:haxe.macro.Expr.Position):T {
		Context.fatalError(message, position);
		throw new haxe.Exception("Context.fatalError returned after a function-lowering fixture diagnostic");
	}
	#else
	public static function install():Void {}
	#end
}

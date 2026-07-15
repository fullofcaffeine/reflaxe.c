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
import reflaxe.c.lowering.CBodyLowering.CBodyGlobalInput;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.plan.CDeclarationPlanner;

typedef EvaluationFunctionRecord = {
	final field:String;
	final cName:String;
	final blocks:Array<String>;
	final temporaryValues:Array<String>;
	final temporaries:Array<String>;
	final labels:Array<String>;
}

typedef EvaluationGlobalRecord = {
	final id:String;
	final cName:String;
	final mutable:Bool;
}

typedef EvaluationSourceRecord = {
	final path:String;
	final content:String;
}

typedef EvaluationOrderReport = {
	final schemaVersion:Int;
	final status:String;
	final profile:String;
	final hxcir:String;
	final header:String;
	final sources:Array<EvaluationSourceRecord>;
	final functions:Array<EvaluationFunctionRecord>;
	final globals:Array<EvaluationGlobalRecord>;
	final symbols:reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
	final runtimeFeatures:Array<String>;
	final temporaryElisionProof:String;
}
#end

class EvaluationOrderProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_EVALUATION_ORDER=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("EvaluationFixture") {
			case TInst(reference, _): reference.get();
			case _: fatal("EvaluationFixture must remain a class", Context.currentPos());
		};
		final inputs:Array<CBodyFunctionInput> = [];
		final globals:Array<CBodyGlobalInput> = [];
		for (sourceOrder => field in fixture.statics.get()) {
			switch field.kind {
				case FMethod(_):
					final expression = switch field.expr() {
						case null: fatal('evaluation-order function `${field.name}` has no typed expression', field.pos);
						case value: value;
					};
					inputs.push({
						modulePath: fixture.module,
						declarationPath: fixture.pack.concat([fixture.name]).join("."),
						sourcePath: "test/evaluation_order/fixtures/EvaluationFixture.hx",
						fieldName: field.name,
						sourceOrder: sourceOrder,
						fieldType: field.type,
						expression: expression
					});
				case FVar(_, write):
					globals.push({
						modulePath: fixture.module,
						declarationPath: fixture.pack.concat([fixture.name]).join("."),
						sourcePath: "test/evaluation_order/fixtures/EvaluationFixture.hx",
						fieldName: field.name,
						sourceOrder: sourceOrder,
						fieldType: field.type,
						mutable: write != AccNever,
						position: field.pos,
						expression: field.expr()
					});
			}
		}
		if (Context.defined("evaluation_order_reverse_input")) {
			inputs.reverse();
			globals.reverse();
		}
		final profile = Context.definedValue("evaluation_order_profile") == "metal" ? CProfile.Metal : CProfile.Portable;
		final context = new CompilationContext(profile);
		final entryId = CBodyLowering.functionId("EvaluationFixture", "main");
		final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", entryId], CNSOrdinary("translation-unit"),
			CSVInternal, "main");
		final guardRequest = new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal,
			CDeclarationPlanner.headerGuardFor(CStaticFunctionProjectEmitter.HEADER_PATH));
		context.symbols.register(entryRequest);
		context.symbols.register(guardRequest);
		final lowered = new CBodyLowering(context).lower(inputs, globals);
		final project = new CStaticFunctionProjectEmitter().plan(lowered, entryId, context.symbols.identifierFor(entryRequest),
			context.symbols.identifierFor(guardRequest));
		final functions:Array<EvaluationFunctionRecord> = [];
		for (fn in lowered.functions) {
			final temporaryValues = [for (valueId in fn.temporaryNames.keys()) valueId];
			temporaryValues.sort(compareStrings);
			final temporaries = [for (name in fn.temporaryNames) name.value];
			temporaries.sort(compareStrings);
			final labels = [for (name in fn.labelNames) name.value];
			labels.sort(compareStrings);
			functions.push({
				field: fn.fieldName,
				cName: fn.cName.value,
				blocks: fn.ir.blocks.map(block -> block.id),
				temporaryValues: temporaryValues,
				temporaries: temporaries,
				labels: labels
			});
		}
		functions.sort((left, right) -> compareStrings(left.field, right.field));
		final globals:Array<EvaluationGlobalRecord> = lowered.globals.map(global -> {
			id: global.ir.id,
			cName: global.cName.value,
			mutable: global.ir.mutable
		});
		globals.sort((left, right) -> compareStrings(left.id, right.id));
		final printer = new CASTPrinter();
		final sources:Array<EvaluationSourceRecord> = project.sources.map(source -> {
			path: source.path,
			content: printer.printTranslationUnit(source.unit)
		});
		final report:EvaluationOrderReport = {
			schemaVersion: 1,
			status: "typed-evaluation-order-runtime-free",
			profile: Std.string(profile),
			hxcir: new HxcIRDumper().dump(lowered.program),
			header: printer.printHeader(project.header),
			sources: sources,
			functions: functions,
			globals: globals,
			symbols: lowered.symbolTable,
			runtimeFeatures: [],
			temporaryElisionProof: "only constants, parameters, and proven-total pure conversions/operators over stable values remain inline; every load and consumed call is materialized"
		};
		Sys.println(REPORT_PREFIX + Json.stringify(report));
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);

	static function fatal<T>(message:String, position:haxe.macro.Expr.Position):T {
		Context.fatalError(message, position);
		throw new haxe.Exception("Context.fatalError returned after an evaluation-order fixture diagnostic");
	}
	#else
	public static function install():Void {}
	#end
}

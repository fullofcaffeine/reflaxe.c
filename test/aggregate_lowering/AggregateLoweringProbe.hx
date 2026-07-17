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

typedef AggregateFieldRecord = {
	final semanticName:String;
	final cName:String;
	final type:String;
}

typedef AggregateRecord = {
	final declarationId:String;
	final instanceId:String;
	final digest:String;
	final cTag:String;
	final fields:Array<AggregateFieldRecord>;
}

typedef AggregateSourceRecord = {
	final path:String;
	final content:String;
}

typedef AggregateLoweringReport = {
	final schemaVersion:Int;
	final status:String;
	final profile:String;
	final hxcir:String;
	final header:String;
	final sources:Array<AggregateSourceRecord>;
	final aggregates:Array<AggregateRecord>;
	final symbols:reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
	final runtimeFeatures:Array<String>;
	final runtimeArtifacts:Array<String>;
}
#end

class AggregateLoweringProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_AGGREGATE_LOWERING=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("AggregateFixture") {
			case TInst(reference, _): reference.get();
			case _: fatal("AggregateFixture must remain a class", Context.currentPos());
		};
		final inputs:Array<CBodyFunctionInput> = [];
		for (sourceOrder => field in fixture.statics.get()) {
			final expression = switch field.expr() {
				case null: fatal('aggregate fixture `${field.name}` has no typed expression', field.pos);
				case value: value;
			};
			inputs.push({
				modulePath: fixture.module,
				declarationPath: fixture.pack.concat([fixture.name]).join("."),
				sourcePath: "test/aggregate_lowering/fixtures/positive/AggregateFixture.hx",
				fieldName: field.name,
				sourceOrder: sourceOrder,
				fieldType: field.type,
				expression: expression
			});
		}
		if (Context.defined("aggregate_lowering_reverse_input")) {
			inputs.reverse();
		}
		final profile = Context.definedValue("aggregate_lowering_profile") == "metal" ? CProfile.Metal : CProfile.Portable;
		final context = new CompilationContext(profile);
		final entryId = CBodyLowering.functionId("AggregateFixture", "main");
		final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", entryId], CNSOrdinary("translation-unit"),
			CSVInternal, "main");
		final guardRequest = new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal,
			CDeclarationPlanner.headerGuardFor(CStaticFunctionProjectEmitter.HEADER_PATH));
		context.symbols.register(entryRequest);
		context.symbols.register(guardRequest);
		final lowered = new CBodyLowering(context).lower(inputs);
		final project = new CStaticFunctionProjectEmitter().plan(lowered, entryId, context.symbols.identifierFor(entryRequest),
			context.symbols.identifierFor(guardRequest));
		final aggregateRecords:Array<AggregateRecord> = [];
		for (aggregate in lowered.aggregates) {
			aggregateRecords.push({
				declarationId: aggregate.prepared.declarationId,
				instanceId: aggregate.prepared.instanceId,
				digest: aggregate.prepared.digest,
				cTag: aggregate.cTag.value,
				fields: aggregate.fields.map(field -> {
					semanticName: field.semanticName,
					cName: field.cName.value,
					type: typeName(field.type.irType)
				})
			});
		}
		final printer = new CASTPrinter();
		final sources:Array<AggregateSourceRecord> = [];
		for (source in project.sources) {
			sources.push({path: source.path, content: printer.printTranslationUnit(source.unit)});
		}
		final report:AggregateLoweringReport = {
			schemaVersion: 1,
			status: "closed-anonymous-records-direct-runtime-free",
			profile: Std.string(profile),
			hxcir: new HxcIRDumper().dump(lowered.program),
			header: printer.printHeader(project.header),
			sources: sources,
			aggregates: aggregateRecords,
			symbols: lowered.symbolTable,
			runtimeFeatures: [],
			runtimeArtifacts: []
		};
		Sys.println(REPORT_PREFIX + Json.stringify(report));
	}

	static function typeName(type:reflaxe.c.ir.HxcIR.HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTFloat(width): 'f$width';
			case IRTInstance(instanceId): 'instance:$instanceId';
			case _: fatal("aggregate fixture field escaped its direct-value subset", Context.currentPos());
		};
	}

	static function fatal<T>(message:String, position:haxe.macro.Expr.Position):T {
		Context.fatalError(message, position);
		throw new haxe.Exception("Context.fatalError returned after an aggregate-lowering fixture diagnostic");
	}
	#else
	public static function install():Void {}
	#end
}

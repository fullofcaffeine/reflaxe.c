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

typedef ClassFieldRecord = {
	final semanticName:String;
	final cName:String;
	final type:String;
	final mutable:Bool;
}

typedef ClassRecord = {
	final declarationId:String;
	final instanceId:String;
	final digest:String;
	final haxePath:String;
	final cTag:String;
	final baseInstanceId:Null<String>;
	final baseMember:Null<String>;
	final emptyAnchor:Null<String>;
	final fields:Array<ClassFieldRecord>;
}

typedef ClassSourceRecord = {
	final path:String;
	final content:String;
}

typedef ClassFunctionRecord = {
	final semanticName:String;
	final cName:String;
}

typedef ClassLayoutReport = {
	final schemaVersion:Int;
	final status:String;
	final profile:String;
	final hxcir:String;
	final header:String;
	final sources:Array<ClassSourceRecord>;
	final classes:Array<ClassRecord>;
	final functions:Array<ClassFunctionRecord>;
	final symbols:reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
	final runtimeFeatures:Array<String>;
	final runtimeArtifacts:Array<String>;
}
#end

class ClassLayoutProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_CLASS_LAYOUT=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("ClassLayoutFixture") {
			case TInst(reference, _): reference.get();
			case _: fatal("ClassLayoutFixture must remain a class", Context.currentPos());
		};
		final inputs:Array<CBodyFunctionInput> = [];
		for (sourceOrder => field in fixture.statics.get()) {
			final expression = switch field.expr() {
				case null: fatal('class-layout fixture `${field.name}` has no typed expression', field.pos);
				case value: value;
			};
			inputs.push({
				modulePath: fixture.module,
				declarationPath: fixture.pack.concat([fixture.name]).join("."),
				sourcePath: "test/class_layout/fixtures/positive/ClassLayoutFixture.hx",
				fieldName: field.name,
				sourceOrder: sourceOrder,
				fieldType: field.type,
				expression: expression
			});
		}
		if (Context.defined("class_layout_reverse_input"))
			inputs.reverse();
		final profile = Context.definedValue("class_layout_profile") == "metal" ? CProfile.Metal : CProfile.Portable;
		final context = new CompilationContext(profile);
		final entryId = CBodyLowering.functionId("ClassLayoutFixture", "main");
		final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", entryId], CNSOrdinary("translation-unit"),
			CSVInternal, "main");
		final guardRequest = new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal,
			CDeclarationPlanner.headerGuardFor(CStaticFunctionProjectEmitter.HEADER_PATH));
		context.symbols.register(entryRequest);
		context.symbols.register(guardRequest);
		final lowered = new CBodyLowering(context).lower(inputs);
		final project = new CStaticFunctionProjectEmitter().plan(lowered, entryId, context.symbols.identifierFor(entryRequest),
			context.symbols.identifierFor(guardRequest));
		final records:Array<ClassRecord> = [];
		for (value in lowered.classes) {
			records.push({
				declarationId: value.prepared.declarationId,
				instanceId: value.prepared.instanceId,
				digest: value.prepared.digest,
				haxePath: value.prepared.haxePath,
				cTag: value.cTag.value,
				baseInstanceId: value.prepared.base == null ? null : value.prepared.base.instanceId,
				baseMember: value.baseMember == null ? null : value.baseMember.value,
				emptyAnchor: value.emptyAnchor == null ? null : value.emptyAnchor.value,
				fields: value.fields.map(field -> {
					semanticName: field.prepared.name,
					cName: field.cName.value,
					type: typeName(field.prepared.type.irType),
					mutable: field.prepared.mutable
				})
			});
		}
		final printer = new CASTPrinter();
		final sources:Array<ClassSourceRecord> = [];
		for (source in project.sources)
			sources.push({path: source.path, content: printer.printTranslationUnit(source.unit)});
		final report:ClassLayoutReport = {
			schemaVersion: 1,
			status: "concrete-private-class-layouts-direct-runtime-free",
			profile: Std.string(profile),
			hxcir: new HxcIRDumper().dump(lowered.program),
			header: printer.printHeader(project.header),
			sources: sources,
			classes: records,
			functions: lowered.functions.map(fn -> {
				semanticName: '${fn.declarationPath}.${fn.fieldName}',
				cName: fn.cName.value
			}),
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
			case IRTPointer(IRTInstance(instanceId), true): 'nullable-instance:$instanceId';
			case _: fatal("class-layout field escaped its direct subset", Context.currentPos());
		};
	}

	static function fatal<T>(message:String, position:haxe.macro.Expr.Position):T {
		Context.fatalError(message, position);
		throw new haxe.Exception("Context.fatalError returned after a class-layout fixture diagnostic");
	}
	#else
	public static function install():Void {}
	#end
}

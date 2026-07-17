#if macro
import haxe.Json;
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.CProfile;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.CStaticFunctionProjectEmitter;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRDumper;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRepresentation;
import reflaxe.c.lowering.CBodyLowering;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.plan.CDeclarationPlanner;

typedef EnumPayloadRecord = {
	final semanticName:String;
	final cName:String;
	final type:String;
	final indirect:Bool;
}

typedef EnumCaseRecord = {
	final semanticName:String;
	final tagValue:Int;
	final discriminant:String;
	final payloadStructTag:Null<String>;
	final unionMember:Null<String>;
	final payload:Array<EnumPayloadRecord>;
}

typedef EnumRecord = {
	final declarationId:String;
	final instanceId:String;
	final digest:String;
	final displayName:String;
	final representation:String;
	final recursive:Bool;
	final scopedLifetime:Bool;
	final valueTag:String;
	final discriminantTag:String;
	final payloadUnionTag:Null<String>;
	final tagMember:Null<String>;
	final payloadMember:Null<String>;
	final cases:Array<EnumCaseRecord>;
}

typedef EnumSourceRecord = {
	final path:String;
	final content:String;
}

typedef EnumLoweringReport = {
	final schemaVersion:Int;
	final status:String;
	final profile:String;
	final hxcir:String;
	final header:String;
	final sources:Array<EnumSourceRecord>;
	final enums:Array<EnumRecord>;
	final symbols:reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
	final runtimeFeatures:Array<String>;
	final runtimeArtifacts:Array<String>;
}
#end

class EnumLoweringProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_ENUM_LOWERING=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("EnumFixture") {
			case TInst(reference, _): reference.get();
			case _: fatal("EnumFixture must remain a class", Context.currentPos());
		};
		final inputs:Array<CBodyFunctionInput> = [];
		for (sourceOrder => field in fixture.statics.get()) {
			final expression = switch field.expr() {
				case null: fatal('enum fixture `${field.name}` has no typed expression', field.pos);
				case value: value;
			};
			inputs.push({
				modulePath: fixture.module,
				declarationPath: fixture.pack.concat([fixture.name]).join("."),
				sourcePath: "test/enum_lowering/fixtures/positive/EnumFixture.hx",
				fieldName: field.name,
				sourceOrder: sourceOrder,
				fieldType: field.type,
				expression: expression
			});
		}
		if (Context.defined("enum_lowering_reverse_input"))
			inputs.reverse();
		final profile = Context.definedValue("enum_lowering_profile") == "metal" ? CProfile.Metal : CProfile.Portable;
		final context = new CompilationContext(profile);
		final entryId = CBodyLowering.functionId("EnumFixture", "main");
		final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", entryId], CNSOrdinary("translation-unit"),
			CSVInternal, "main");
		final guardRequest = new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal,
			CDeclarationPlanner.headerGuardFor(CStaticFunctionProjectEmitter.HEADER_PATH));
		context.symbols.register(entryRequest);
		context.symbols.register(guardRequest);
		final lowered = new CBodyLowering(context).lower(inputs);
		final project = new CStaticFunctionProjectEmitter().plan(lowered, entryId, context.symbols.identifierFor(entryRequest),
			context.symbols.identifierFor(guardRequest));
		final enumRecords:Array<EnumRecord> = [];
		for (value in lowered.enums) {
			enumRecords.push({
				declarationId: value.prepared.declarationId,
				instanceId: value.prepared.instanceId,
				digest: value.prepared.digest,
				displayName: value.prepared.displayName,
				representation: switch value.prepared.representation {
					case CBERNativeEnum: "native-enum";
					case CBERTaggedUnion: "tagged-union";
				},
				recursive: value.prepared.recursive,
				scopedLifetime: value.prepared.scopedLifetime,
				valueTag: value.valueTag.value,
				discriminantTag: value.discriminantTag.value,
				payloadUnionTag: identifier(value.payloadUnionTag),
				tagMember: identifier(value.tagMember),
				payloadMember: identifier(value.payloadMember),
				cases: value.cases.map(tagCase -> {
					semanticName: tagCase.prepared.name,
					tagValue: tagCase.prepared.tagValue,
					discriminant: tagCase.discriminant.value,
					payloadStructTag: identifier(tagCase.payloadStructTag),
					unionMember: identifier(tagCase.unionMember),
					payload: tagCase.payload.map(payload -> {
						semanticName: payload.prepared.name,
						cName: payload.cName.value,
						type: typeName(payload.prepared.storageType()),
						indirect: payload.prepared.indirect
					})
				})
			});
		}
		final printer = new CASTPrinter();
		final sources:Array<EnumSourceRecord> = [];
		for (source in project.sources)
			sources.push({path: source.path, content: printer.printTranslationUnit(source.unit)});
		final report:EnumLoweringReport = {
			schemaVersion: 1,
			status: "haxe-enums-direct-runtime-free",
			profile: Std.string(profile),
			hxcir: new HxcIRDumper().dump(lowered.program),
			header: printer.printHeader(project.header),
			sources: sources,
			enums: enumRecords,
			symbols: lowered.symbolTable,
			runtimeFeatures: [],
			runtimeArtifacts: []
		};
		Sys.println(REPORT_PREFIX + Json.stringify(report));
	}

	static function identifier(value:Null<reflaxe.c.ast.CAST.CIdentifier>):Null<String>
		return value == null ? null : value.value;

	static function typeName(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTFloat(width): 'f$width';
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(pointee, nullable): 'pointer:${nullable ? "nullable" : "nonnull"}<${typeName(pointee)}>';
			case _: fatal("enum fixture payload escaped its direct-value subset", Context.currentPos());
		};
	}

	static function fatal<T>(message:String, position:haxe.macro.Expr.Position):T {
		Context.fatalError(message, position);
		throw new haxe.Exception("Context.fatalError returned after an enum-lowering fixture diagnostic");
	}
	#else
	public static function install():Void {}
	#end
}

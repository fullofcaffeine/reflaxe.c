#if macro
import haxe.Json;
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.CBuildMode;
import reflaxe.c.CProfile;
import reflaxe.c.CompilationContext;
import reflaxe.c.ir.HxcIRDumper;
import reflaxe.c.lowering.CBodyLowering;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;

typedef SpanLoweringFunctionRecord = {
	final field:String;
	final cName:String;
	final localNames:Array<String>;
	final spanLengthNames:Array<String>;
}

typedef SpanLoweringProbeRecord = {
	final schemaVersion:Int;
	final status:String;
	final profile:String;
	final buildMode:String;
	final hxcir:String;
	final functions:Array<SpanLoweringFunctionRecord>;
	final symbols:reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
	final runtimeFeatures:Array<String>;
	final runtimeArtifacts:Array<String>;
}
#end

class SpanLoweringProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_SPAN_LOWERING=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("SpanFixture") {
			case TInst(reference, _): reference.get();
			case _: fatal("SpanFixture must remain a class");
		};
		final selected = [
			"checkedAt",
			"constSum",
			"linearIndex",
			"main",
			"mutableSum",
			"mutatedGridCell",
			"zeroedGridCell"
		];
		final inputs:Array<CBodyFunctionInput> = [];
		for (sourceOrder => field in fixture.statics.get()) {
			if (selected.indexOf(field.name) == -1) {
				continue;
			}
			final expression = field.expr();
			if (expression == null) {
				fatal('selected span fixture `${field.name}` has no typed expression');
			}
			inputs.push({
				modulePath: fixture.module,
				declarationPath: fixture.pack.concat([fixture.name]).join("."),
				sourcePath: "test/span_lowering/fixtures/SpanFixture.hx",
				fieldName: field.name,
				sourceOrder: sourceOrder,
				fieldType: field.type,
				expression: expression
			});
		}
		if (inputs.length != selected.length) {
			fatal('span fixture selection found ${inputs.length} functions, expected ${selected.length}');
		}
		if (Context.defined("span_lowering_reverse_input")) {
			inputs.reverse();
		}
		final profile = Context.definedValue("span_lowering_profile") == "metal" ? CProfile.Metal : CProfile.Portable;
		final buildMode = switch Context.definedValue("span_lowering_build") {
			case null | "" | "debug": CBuildMode.Debug;
			case "release": CBuildMode.Release;
			case "minsizerel": CBuildMode.MinSizeRel;
			case value: fatal('invalid probe build mode `$value`');
		};
		final result = new CBodyLowering(new CompilationContext(profile, buildMode)).lower(inputs);
		final functions:Array<SpanLoweringFunctionRecord> = [];
		for (fn in result.functions) {
			final localNames = [for (name in fn.localNames) name.value];
			localNames.sort(compareStrings);
			final spanLengthNames = [for (name in fn.spanLengthNames) name.value];
			spanLengthNames.sort(compareStrings);
			functions.push({
				field: fn.fieldName,
				cName: fn.cName.value,
				localNames: localNames,
				spanLengthNames: spanLengthNames
			});
		}
		functions.sort((left, right) -> compareStrings(left.field, right.field));
		final record:SpanLoweringProbeRecord = {
			schemaVersion: 2,
			status: "typed-zero-fixed-arrays-and-spans-runtime-free",
			profile: Std.string(profile),
			buildMode: Std.string(buildMode),
			hxcir: new HxcIRDumper().dump(result.program),
			functions: functions,
			symbols: result.symbolTable,
			runtimeFeatures: [],
			runtimeArtifacts: []
		};
		Sys.println(REPORT_PREFIX + Json.stringify(record));
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);

	static function fatal<T>(message:String):T {
		Context.fatalError(message, Context.currentPos());
		throw new haxe.Exception("Context.fatalError returned from span-lowering probe");
	}
	#else
	public static function install():Void {}
	#end
}

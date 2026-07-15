#if macro
import haxe.io.Bytes;
import haxe.Json;
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.CProfile;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.ir.HxcIRDumper;
import reflaxe.c.lowering.CBodyEmitter;
import reflaxe.c.lowering.CBodyLowering;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
import reflaxe.c.lowering.CBodyLowering.CBodyLoweringResult;

typedef BodyLoweringFunctionRecord = {
	final field:String;
	final irId:String;
	final cName:String;
	final locals:Array<BodyLoweringLocalRecord>;
}

typedef BodyLoweringLocalRecord = {
	final irId:String;
	final cName:String;
}

typedef BodyLoweringProbeRecord = {
	final schemaVersion:Int;
	final status:String;
	final profile:String;
	final hxcir:String;
	final cSource:String;
	final lineMappedCSource:String;
	final functions:Array<BodyLoweringFunctionRecord>;
	final symbols:reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
	final runtimeFeatures:Array<String>;
	final runtimeArtifacts:Array<String>;
}
#end

class BodyLoweringProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_BODY_LOWERING=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("BodyFixture") {
			case TInst(reference, _): reference.get();
			case _: fatal("BodyFixture must remain a class", Context.currentPos());
		};
		final selected = [
			"booleanValue",
			"directInteger",
			"explicitVoid",
			"floatingValue",
			"implicitVoid",
			"integerValue",
			"unsignedValue"
		];
		final inputs:Array<CBodyFunctionInput> = [];
		for (sourceOrder => field in fixture.statics.get()) {
			if (selected.indexOf(field.name) == -1) {
				continue;
			}
			final typedExpression:TypedExpr = switch field.expr() {
				case null: fatal('selected body fixture `${field.name}` has no typed expression', field.pos);
				case value: value;
			};
			inputs.push({
				modulePath: fixture.module,
				declarationPath: fixture.pack.concat([fixture.name]).join("."),
				sourcePath: "test/body_lowering/fixtures/positive/BodyFixture.hx",
				fieldName: field.name,
				sourceOrder: sourceOrder,
				expression: typedExpression
			});
		}
		if (inputs.length != selected.length) {
			fatal('body fixture selection found ${inputs.length} functions, expected ${selected.length}', fixture.pos);
		}
		if (Context.defined("body_lowering_reverse_input")) {
			inputs.reverse();
		}
		final profile = Context.definedValue("body_lowering_profile") == "metal" ? CProfile.Metal : CProfile.Portable;
		final result = new CBodyLowering(new CompilationContext(profile)).lower(inputs);
		final printer = new CASTPrinter();
		final functionRecords:Array<BodyLoweringFunctionRecord> = [];
		for (fn in result.functions) {
			final localIds = [for (localId in fn.localNames.keys()) localId];
			localIds.sort(compareStrings);
			final locals:Array<BodyLoweringLocalRecord> = [];
			for (localId in localIds) {
				final cName = fn.localNames.get(localId);
				if (cName == null) {
					fatal('lowered function `${fn.ir.id}` lost local `$localId`', fixture.pos);
				}
				locals.push({irId: localId, cName: cName.value});
			}
			functionRecords.push({
				field: fn.fieldName,
				irId: fn.ir.id,
				cName: fn.cName.value,
				locals: locals
			});
		}
		functionRecords.sort((left, right) -> compareStrings(left.field, right.field));
		final record:BodyLoweringProbeRecord = {
			schemaVersion: 1,
			status: "typed-haxe-body-lowering-no-calls-or-entry-point-emission",
			profile: Std.string(profile),
			hxcir: new HxcIRDumper().dump(result.program),
			cSource: printer.printTranslationUnit(translationUnit(result, false)),
			lineMappedCSource: printer.printTranslationUnit(translationUnit(result, true)),
			functions: functionRecords,
			symbols: result.symbolTable,
			runtimeFeatures: [],
			runtimeArtifacts: []
		};
		Sys.println(REPORT_PREFIX + Json.stringify(record));
	}

	/** Test-only static-function envelope; production function emission is E2.T03. */
	static function translationUnit(result:CBodyLoweringResult, lineDirectives:Bool):CTranslationUnit {
		final unit = new CTranslationUnit();
		final headers:Array<String> = [];
		for (fn in result.functions) {
			for (header in fn.requiredHeaders) {
				if (headers.indexOf(header) == -1) {
					headers.push(header);
				}
			}
		}
		headers.sort(compareUtf8);
		for (header in headers) {
			unit.includes.push({path: header, kind: System});
		}
		final emitter = new CBodyEmitter();
		for (fn in result.functions) {
			if (lineDirectives) {
				unit.declarations.push(DLineDirective({line: fn.ir.source.startLine, file: fn.ir.source.file}));
			}
			unit.declarations.push(DFunction({
				storage: [SStatic],
				functionSpecifiers: [],
				returnType: emitter.cType(fn.ir.returnType),
				declarator: DFunction(DName(fn.cName), FPPrototype([], false)),
				body: lineDirectives ? fn.lineMappedBody : fn.body,
				attributes: []
			}));
		}
		return unit;
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}

	static function fatal<T>(message:String, position:haxe.macro.Expr.Position):T {
		Context.fatalError(message, position);
		throw new haxe.Exception("Context.fatalError returned after a fatal body-lowering fixture diagnostic");
	}
	#else
	public static function install():Void {}
	#end
}

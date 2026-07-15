import haxe.macro.Type.AbstractType;
import haxe.macro.Type.ClassType;
import haxe.macro.Type.DefType;
import haxe.macro.Type.EnumType;
import reflaxe.BaseCompiler;
import reflaxe.BaseCompiler.BaseCompilerFileOutputType;
import reflaxe.c.CProfile;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.emit.CProjectEmitter;
import reflaxe.c.emit.CProjectEmitter.CProjectCompilationStatus;
import reflaxe.c.emit.CProjectEmitter.CProjectEmissionPlan;
import reflaxe.c.emit.CProjectEmitter.CProjectEnvironment;
import reflaxe.c.emit.CProjectEmitter.CProjectRuntimeDiagnostics;
import reflaxe.c.emit.CProjectEmitter.CProjectRuntimePolicy;
import reflaxe.c.emit.CProjectEmitter.CProjectStandard;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.emit.ReflaxeOutputWriter;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.naming.CSymbolRequest.CSymbolKind;
import reflaxe.c.naming.CSymbolRequest.CSymbolNamespace;
import reflaxe.c.naming.CSymbolRequest.CSymbolVisibility;
import reflaxe.data.ClassFuncData;
import reflaxe.data.ClassVarData;
import reflaxe.data.EnumOptionData;
import reflaxe.output.DataAndFileInfo;
import reflaxe.output.StringOrBytes;

/** Drives the real Reflaxe OutputManager with a direct structural project plan. */
class ProjectEmitterGolden {
	public static function run(mode:String, outputDirectory:String):Void {
		final files = switch mode {
			case "full": new CProjectEmitter().emit(plan(false, false, CProjectCompilationStatus.StructuralFixture));
			case "reverse": new CProjectEmitter().emit(plan(true, false, CProjectCompilationStatus.StructuralFixture));
			case "trimmed": new CProjectEmitter().emit(plan(false, true, CProjectCompilationStatus.StructuralFixture));
			case "duplicate":
				final duplicatePlan = plan(false, false, CProjectCompilationStatus.StructuralFixture);
				duplicatePlan.units.push(duplicatePlan.units[0]);
				new CProjectEmitter().emit(duplicatePlan);
			case "invalid-path":
				[new GeneratedFile("../outside.c", "int outside;\n", GeneratedFileKind.Source)];
			case "invalid-layout":
				new CProjectEmitter().emit({
					schemaVersion: CProjectEmitter.SCHEMA_VERSION,
					projectName: "emitter-fixture",
					compilationStatus: CProjectCompilationStatus.StructuralFixture,
					profile: CProfile.Portable,
					environment: CProjectEnvironment.Hosted,
					cStandard: CProjectStandard.C11,
					runtimePolicy: CProjectRuntimePolicy.None,
					runtimeDiagnostics: CProjectRuntimeDiagnostics.Off,
					units: [
						new GeneratedFile("include/not_a_source.c", "int misplaced;\n", GeneratedFileKind.Source)
					],
					buildFacts: [],
					symbolTable: emptySymbols()
				});
			case "lowered": new CProjectEmitter().emit(plan(false, false, CProjectCompilationStatus.LoweredProgram));
			case _: throw 'unknown project emitter fixture mode `$mode`';
		};

		final compiler = new ProjectOutputCompiler(files);
		compiler.setOptions({
			fileOutputType: BaseCompilerFileOutputType.Manual,
			fileOutputExtension: ".c",
			outputDirDefineName: "project_emitter_output",
			deleteOldOutput: true
		});
		compiler.setOutputDir(outputDirectory);
		compiler.generateFiles();
		Sys.println("project-emitter-macro: OK");
	}

	static function plan(reverse:Bool, trimmed:Bool, compilationStatus:CProjectCompilationStatus):CProjectEmissionPlan {
		final registry = new CSymbolRegistry();
		final exportRequest = new CSymbolRequest(CSymbolKind.CSKExport, ["fixture", "Api", "value"], CSymbolNamespace.CNSOrdinary("translation-unit"),
			CSymbolVisibility.CSVPublic);
		final bootRequest = new CSymbolRequest(CSymbolKind.CSKStaticInitializer, ["fixture", "Main"], CSymbolNamespace.CNSOrdinary("translation-unit"));
		final requests = [exportRequest, bootRequest];
		if (reverse) {
			requests.reverse();
		}
		registry.registerAll(requests);
		final symbolTable = registry.finalizeSymbols();
		final exportedName = registry.identifierFor(exportRequest).value;

		final units = [
			new GeneratedFile("include/hxc/emitter_fixture.h", publicHeader(exportedName), GeneratedFileKind.PublicHeader),
			new GeneratedFile("include/hxc/detail/emitter_fixture_internal.h", privateHeader(), GeneratedFileKind.PrivateHeader),
			new GeneratedFile("src/emitter_fixture.c", implementation(exportedName), GeneratedFileKind.Source),
			new GeneratedFile("src/hxc_boot.c", bootSource(exportedName), GeneratedFileKind.Source)
		];
		if (!trimmed) {
			units.push(new GeneratedFile("include/hxc/removed_module.h", removedHeader(), GeneratedFileKind.PublicHeader));
		}
		final buildFacts:Array<TypedCBuildFact> = [
			{
				kind: "include",
				name: "stdio.h",
				value: "system",
				valueKind: "enum",
				ownerModulePaths: ["fixture.Main"]
			},
			{
				kind: "include",
				name: "stdio.h",
				value: "system",
				valueKind: "enum",
				ownerModulePaths: ["fixture.Api", "fixture.Main"]
			},
			{
				kind: "define",
				name: "HXC_EMITTER_FIXTURE",
				value: "1",
				valueKind: "integer",
				ownerModulePaths: ["fixture.Main"]
			}
		];
		if (reverse) {
			units.reverse();
			buildFacts.reverse();
			for (fact in buildFacts) {
				fact.ownerModulePaths.reverse();
			}
		}
		return {
			schemaVersion: CProjectEmitter.SCHEMA_VERSION,
			projectName: "emitter-fixture",
			compilationStatus: compilationStatus,
			profile: CProfile.Portable,
			environment: CProjectEnvironment.Hosted,
			cStandard: CProjectStandard.C11,
			runtimePolicy: CProjectRuntimePolicy.None,
			runtimeDiagnostics: CProjectRuntimeDiagnostics.Off,
			units: units,
			buildFacts: buildFacts,
			symbolTable: symbolTable
		};
	}

	static function emptySymbols():reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot {
		final registry = new CSymbolRegistry();
		return registry.finalizeSymbols();
	}

	static function publicHeader(exportedName:String):String {
		return "#ifndef HXC_EMITTER_FIXTURE_H\n"
			+ "#define HXC_EMITTER_FIXTURE_H\n"
			+ "\n"
			+ 'int $exportedName(void);\n'
			+ "\n"
			+ "#endif\n";
	}

	static function privateHeader():String {
		return "#ifndef HXC_DETAIL_EMITTER_FIXTURE_INTERNAL_H\n" + "#define HXC_DETAIL_EMITTER_FIXTURE_INTERNAL_H\n" + "\n"
			+ "enum { HXC_EMITTER_FIXTURE_ANSWER = 42 };\n" + "\n" + "#endif\n";
	}

	static function implementation(exportedName:String):String {
		return '#include "hxc/emitter_fixture.h"\n' + '#include "hxc/detail/emitter_fixture_internal.h"\n' + "\n" + 'int $exportedName(void) {\n'
			+ "    return HXC_EMITTER_FIXTURE_ANSWER;\n" + "}\n";
	}

	static function bootSource(exportedName:String):String {
		return '#include "hxc/emitter_fixture.h"\n' + "#include <stdio.h>\n" + "\n" + "int main(void) {\n" + '    if ($exportedName() != 42) {\n'
			+ "        return 1;\n" + "    }\n" + '    (void)puts("project-emitter: OK");\n' + "    return 0;\n" + "}\n";
	}

	static function removedHeader():String {
		return "#ifndef HXC_REMOVED_MODULE_H\n"
			+ "#define HXC_REMOVED_MODULE_H\n"
			+ "\n"
			+ "enum { HXC_REMOVED_MODULE_MARKER = 1 };\n"
			+ "\n"
			+ "#endif\n";
	}
}

private class ProjectOutputCompiler extends BaseCompiler {
	final files:Array<GeneratedFile>;

	public function new(files:Array<GeneratedFile>) {
		super();
		this.files = files;
	}

	override public function generateFilesManually():Void {
		final manager = output;
		if (manager == null) {
			throw "missing Reflaxe output manager";
		}
		new ReflaxeOutputWriter().write(manager, files);
	}

	public function generateOutputIterator():Iterator<DataAndFileInfo<StringOrBytes>> {
		final empty:Array<DataAndFileInfo<StringOrBytes>> = [];
		return empty.iterator();
	}

	public function compileClass(classType:ClassType, varFields:Array<ClassVarData>, funcFields:Array<ClassFuncData>):Void {}

	public function compileEnum(enumType:EnumType, options:Array<EnumOptionData>):Void {}

	public function compileTypedef(classType:DefType):Void {}

	public function compileAbstract(classType:AbstractType):Void {}
}

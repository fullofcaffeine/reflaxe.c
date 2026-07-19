import haxe.macro.Type.AbstractType;
import haxe.macro.Type.ClassType;
import haxe.macro.Type.DefType;
import haxe.macro.Type.EnumType;
import haxe.macro.Context;
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
import reflaxe.c.emit.CProjectLayout;
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
import sys.io.File;

private enum ProjectFixtureVariant {
	PFVOriginal;
	PFVRenamed;
}

private typedef ProjectFixtureIdentity = {
	final apiType:String;
	final apiField:String;
	final mainType:String;
	final publicHeaderPath:String;
	final privateHeaderPath:String;
	final sourcePath:String;
	final bootSourcePath:String;
	final guardStem:String;
	final defineName:String;
}

/** Drives the real Reflaxe OutputManager with a direct structural project plan. */
class ProjectEmitterGolden {
	public static function run(mode:String, outputDirectory:String):Void {
		final files = switch mode {
			case "full": new CProjectEmitter().emit(plan(false, false, CProjectCompilationStatus.StructuralFixture, ProjectFixtureVariant.PFVOriginal));
			case "reverse": new CProjectEmitter().emit(plan(true, false, CProjectCompilationStatus.StructuralFixture, ProjectFixtureVariant.PFVOriginal));
			case "trimmed": new CProjectEmitter().emit(plan(false, true, CProjectCompilationStatus.StructuralFixture, ProjectFixtureVariant.PFVOriginal));
			case "renamed": new CProjectEmitter().emit(plan(false, true, CProjectCompilationStatus.StructuralFixture, ProjectFixtureVariant.PFVRenamed));
			case "ast-declarators": new CProjectEmitter().emit(astPlan("declarators"));
			case "ast-expressions": new CProjectEmitter().emit(astPlan("expressions"));
			case "duplicate":
				final duplicatePlan = plan(false, false, CProjectCompilationStatus.StructuralFixture, ProjectFixtureVariant.PFVOriginal);
				duplicatePlan.units.push(duplicatePlan.units[0]);
				new CProjectEmitter().emit(duplicatePlan);
			case "invalid-path":
				[new GeneratedFile("../outside.c", "int outside;\n", GeneratedFileKind.Source)];
			case "invalid-line-endings":
				[
					new GeneratedFile("src/invalid_line_endings.c", "int invalid_line_endings;\r\n", GeneratedFileKind.Source)
				];
			case "invalid-layout":
				new CProjectEmitter().emit({
					schemaVersion: CProjectEmitter.SCHEMA_VERSION,
					projectName: "emitter-fixture",
					compilationStatus: CProjectCompilationStatus.StructuralFixture,
					profile: CProfile.Portable,
					environment: CProjectEnvironment.Hosted,
					cStandard: CProjectStandard.C11,
					projectLayout: CProjectLayout.Unity,
					runtimePolicy: CProjectRuntimePolicy.None,
					runtimeDiagnostics: CProjectRuntimeDiagnostics.Off,
					units: [
						new GeneratedFile("include/not_a_source.c", "int misplaced;\n", GeneratedFileKind.Source)
					],
					buildFacts: [],
					symbolTable: emptySymbols()
				});
			case "invalid-adapter-path":
				final invalidPathPlan = plan(false, false, CProjectCompilationStatus.StructuralFixture, ProjectFixtureVariant.PFVOriginal);
				invalidPathPlan.units.push(new GeneratedFile("src/$<unsafe>.c", "int unsafe_adapter_path(void) { return 0; }\n", GeneratedFileKind.Source));
				new CProjectEmitter().emit(invalidPathPlan);
			case "invalid-define":
				final invalidDefinePlan = plan(false, false, CProjectCompilationStatus.StructuralFixture, ProjectFixtureVariant.PFVOriginal);
				invalidDefinePlan.buildFacts.push({
					kind: "define",
					name: "HXC_INVALID_DEFINE",
					value: "1;injected",
					valueKind: "integer",
					ownerModulePaths: ["fixture.Main"]
				});
				new CProjectEmitter().emit(invalidDefinePlan);
			case "conflicting-define":
				final conflictingDefinePlan = plan(false, false, CProjectCompilationStatus.StructuralFixture, ProjectFixtureVariant.PFVOriginal);
				conflictingDefinePlan.buildFacts.push({
					kind: "define",
					name: "HXC_EMITTER_FIXTURE",
					value: "2",
					valueKind: "integer",
					ownerModulePaths: ["fixture.ConflictingMain"]
				});
				new CProjectEmitter().emit(conflictingDefinePlan);
			case "conflicting-include":
				final conflictingIncludePlan = plan(false, false, CProjectCompilationStatus.StructuralFixture, ProjectFixtureVariant.PFVOriginal);
				conflictingIncludePlan.buildFacts.push({
					kind: "include",
					name: "stdio.h",
					value: "local",
					valueKind: "enum",
					ownerModulePaths: ["fixture.ConflictingMain"]
				});
				new CProjectEmitter().emit(conflictingIncludePlan);
			case "lowered":
				new CProjectEmitter().emit(plan(false, false, CProjectCompilationStatus.LoweredProgram, ProjectFixtureVariant.PFVOriginal));
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

	static function plan(reverse:Bool, trimmed:Bool, compilationStatus:CProjectCompilationStatus, variant:ProjectFixtureVariant):CProjectEmissionPlan {
		final identity = fixtureIdentity(variant);
		final registry = new CSymbolRegistry();
		final exportRequest = new CSymbolRequest(CSymbolKind.CSKExport, ["fixture", identity.apiType, identity.apiField],
			CSymbolNamespace.CNSOrdinary("translation-unit"), CSymbolVisibility.CSVPublic);
		final bootRequest = new CSymbolRequest(CSymbolKind.CSKStaticInitializer, ["fixture", identity.mainType],
			CSymbolNamespace.CNSOrdinary("translation-unit"));
		final requests = [exportRequest, bootRequest];
		if (reverse) {
			requests.reverse();
		}
		registry.registerAll(requests);
		final symbolTable = registry.finalizeSymbols();
		final exportedName = registry.identifierFor(exportRequest).value;

		final units = [
			new GeneratedFile(identity.publicHeaderPath, publicHeader(identity, exportedName), GeneratedFileKind.PublicHeader),
			new GeneratedFile(identity.privateHeaderPath, privateHeader(identity), GeneratedFileKind.PrivateHeader),
			new GeneratedFile(identity.sourcePath, implementation(identity, exportedName), GeneratedFileKind.Source),
			new GeneratedFile(identity.bootSourcePath, bootSource(identity, exportedName), GeneratedFileKind.Source)
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
				ownerModulePaths: ['fixture.${identity.mainType}']
			},
			{
				kind: "include",
				name: "stdio.h",
				value: "system",
				valueKind: "enum",
				ownerModulePaths: ['fixture.${identity.apiType}', 'fixture.${identity.mainType}']
			},
			{
				kind: "define",
				name: identity.defineName,
				value: "1",
				valueKind: "integer",
				ownerModulePaths: ['fixture.${identity.mainType}']
			},
			{
				kind: "define",
				name: "HXC_ADAPTER_TEXT",
				value: "adapter;$<ignored>;quote'\"\\end",
				valueKind: "string",
				ownerModulePaths: ['fixture.${identity.apiType}']
			},
			{
				kind: "link",
				name: "m",
				value: null,
				valueKind: null,
				ownerModulePaths: ['fixture.${identity.mainType}']
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
			projectLayout: CProjectLayout.Unity,
			runtimePolicy: CProjectRuntimePolicy.None,
			runtimeDiagnostics: CProjectRuntimeDiagnostics.Off,
			units: units,
			buildFacts: buildFacts,
			symbolTable: symbolTable
		};
	}

	static function astPlan(name:String):CProjectEmissionPlan {
		final expectedPath = switch name {
			case "declarators": "test/c_ast/expected/declarators.c";
			case "expressions": "test/c_ast/expected/expressions.c";
			case _: throw 'unknown C AST build-adapter fixture `$name`';
		};
		final source = "#if !defined(HXC_BUILD_ADAPTER_FIXTURE) || HXC_BUILD_ADAPTER_FIXTURE != 1\n"
			+ '#error "build adapter lost the neutral compile definition"\n'
			+ "#endif\n"
			+ File.getContent(Context.resolvePath(expectedPath));
		return {
			schemaVersion: CProjectEmitter.SCHEMA_VERSION,
			projectName: 'c-ast-$name',
			compilationStatus: CProjectCompilationStatus.StructuralFixture,
			profile: CProfile.Portable,
			environment: CProjectEnvironment.Hosted,
			cStandard: CProjectStandard.C11,
			projectLayout: CProjectLayout.Unity,
			runtimePolicy: CProjectRuntimePolicy.None,
			runtimeDiagnostics: CProjectRuntimeDiagnostics.Off,
			units: [
				new GeneratedFile('src/AST fixture\'s $name.c', source, GeneratedFileKind.Source)
			],
			buildFacts: [
				{
					kind: "define",
					name: "HXC_BUILD_ADAPTER_FIXTURE",
					value: "1",
					valueKind: "integer",
					ownerModulePaths: ['fixture.CAST.$name']
				}
			],
			symbolTable: emptySymbols()
		};
	}

	static function fixtureIdentity(variant:ProjectFixtureVariant):ProjectFixtureIdentity {
		return switch variant {
			case PFVOriginal:
				{
					apiType: "Api",
					apiField: "value",
					mainType: "Main",
					publicHeaderPath: "include/hxc/emitter_fixture.h",
					privateHeaderPath: "include/hxc/detail/emitter_fixture_internal.h",
					sourcePath: "src/emitter_fixture.c",
					bootSourcePath: "src/hxc_boot.c",
					guardStem: "EMITTER_FIXTURE",
					defineName: "HXC_EMITTER_FIXTURE"
				};
			case PFVRenamed:
				{
					apiType: "RenamedApi",
					apiField: "renamedValue",
					mainType: "RenamedMain",
					publicHeaderPath: "include/hxc/renamed_fixture.h",
					privateHeaderPath: "include/hxc/detail/renamed_fixture_internal.h",
					sourcePath: "src/renamed_fixture.c",
					bootSourcePath: "src/hxc_renamed_boot.c",
					guardStem: "RENAMED_FIXTURE",
					defineName: "HXC_RENAMED_FIXTURE"
				};
		};
	}

	static function emptySymbols():reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot {
		final registry = new CSymbolRegistry();
		return registry.finalizeSymbols();
	}

	static function publicHeader(identity:ProjectFixtureIdentity, exportedName:String):String {
		final guard = 'HXC_${identity.guardStem}_H';
		return '#ifndef $guard\n' + '#define $guard\n' + "\n" + 'int $exportedName(void);\n' + "\n" + "#endif\n";
	}

	static function privateHeader(identity:ProjectFixtureIdentity):String {
		final guard = 'HXC_DETAIL_${identity.guardStem}_INTERNAL_H';
		final answer = 'HXC_${identity.guardStem}_ANSWER';
		return '#ifndef $guard\n' + '#define $guard\n' + "\n" + 'enum { $answer = 42 };\n' + "\n" + "#endif\n";
	}

	static function implementation(identity:ProjectFixtureIdentity, exportedName:String):String {
		final publicHeader = identity.publicHeaderPath.substr("include/".length);
		final privateHeader = identity.privateHeaderPath.substr("include/".length);
		final answer = 'HXC_${identity.guardStem}_ANSWER';
		return '#include "$publicHeader"\n'
			+ '#include "$privateHeader"\n'
			+ "\n"
			+ 'int $exportedName(void) {\n'
			+ '    return $answer;\n'
			+ "}\n";
	}

	static function bootSource(identity:ProjectFixtureIdentity, exportedName:String):String {
		final publicHeader = identity.publicHeaderPath.substr("include/".length);
		return '#include "$publicHeader"\n' + "#include <stdio.h>\n" + "\n" + "int main(void) {\n" + '    if ($exportedName() != 42) {\n'
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

#if macro
import haxe.Json;
import haxe.macro.Context;
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.CProjectLayout.CProjectLayout;
import reflaxe.c.emit.CProjectLayout.CProjectLayoutPlanner;
import reflaxe.c.emit.CStaticFunctionProjectEmitter;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.interop.CImportRegistry.CLoweredImports;
import reflaxe.c.lowering.CBodyDispatch.CBodyDispatchGraph;
import reflaxe.c.lowering.CBodyDispatch.CLoweredBodyDispatch;
import reflaxe.c.lowering.CBodyDispatch.CPreparedBodyDispatch;
import reflaxe.c.lowering.CBodyEmitter;
import reflaxe.c.lowering.CBodyLowering.CBodyLoweringResult;
import reflaxe.c.lowering.CBodyLowering.CLoweredBodyFunction;
import reflaxe.c.lowering.CBodyLowering.CManagedProgramNames;
import reflaxe.c.runtime.RuntimeAbiContract;

private typedef GeneratedRootFile = {
	final path:String;
	final content:String;
}

private typedef GeneratedRootProject = {
	final layout:String;
	final headers:Array<GeneratedRootFile>;
	final sources:Array<GeneratedRootFile>;
}
#end

/**
	Emits a tiny structural-C program fragment for the exact-root native test.

	This fixture deliberately starts from hand-built, typed HxcIR. The semantic
	validator owns root/type correctness elsewhere; this probe isolates the next
	boundary and proves that `CBodyEmitter` turns the plan into balanced stack-root
	operations on both a normal return and an early status-propagation return.
**/
class GeneratedRootFrameProbe {
	#if macro
	static inline final REPORT_PREFIX = "HXC_GENERATED_ROOT_FRAME=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final normal = rootFunction("fn.generated-root.normal", false);
		final failure = rootFunction("fn.generated-root.failure", true);
		final rootArrays:Map<String, CIdentifier> = [];
		final rootFrames:Map<String, CIdentifier> = [];
		rootArrays.set(normal.id, new CIdentifier("hxc_normal_roots"));
		rootFrames.set(normal.id, new CIdentifier("hxc_normal_frame"));
		rootArrays.set(failure.id, new CIdentifier("hxc_failure_roots"));
		rootFrames.set(failure.id, new CIdentifier("hxc_failure_frame"));
		final names = new CManagedProgramNames(new CIdentifier("hxc_program_gc"), new CIdentifier("hxc_program_thread"), rootArrays, rootFrames);
		final emitter = new CBodyEmitter(null, null, null, null, null, null, null, null, null, null, names);
		final unit = new CTranslationUnit();
		for (header in ["stdbool.h", "stddef.h", "stdlib.h", "hxrt/gc.h"])
			unit.includes.push({path: header, kind: System});
		unit.declarations.push(DVariable({
			storage: [SExtern],
			alignments: [],
			type: new CType(TStruct(new CIdentifier("hxc_gc"))),
			declarator: DName(names.collector),
			initializer: null,
			attributes: []
		}));
		unit.declarations.push(DVariable({
			storage: [SExtern],
			alignments: [],
			type: new CType(TStruct(new CIdentifier("hxc_gc_thread"))),
			declarator: DName(names.thread),
			initializer: null,
			attributes: []
		}));
		unit.declarations.push(prototype("hxc_probe_collect_success", false));
		unit.declarations.push(prototype("hxc_probe_collect_failure", true));
		unit.declarations.push(generatedPrototype(emitter, normal, "hxc_generated_root_success"));
		unit.declarations.push(generatedPrototype(emitter, failure, "hxc_generated_root_failure"));
		unit.declarations.push(definition(emitter, normal, "hxc_generated_root_success", "hxc_probe_collect_success"));
		unit.declarations.push(definition(emitter, failure, "hxc_generated_root_failure", "hxc_probe_collect_failure"));
		Sys.println(REPORT_PREFIX + Json.stringify({
			source: new CASTPrinter().printTranslationUnit(unit),
			projects: managedProjects()
		}));
	}

	/**
		Exercise executable collector ownership in every supported file layout.

		The separate native harness above proves object lifetime. These projects keep
		the semantic workload intentionally small so failures point specifically at
		program-global declarations, support definitions, entry startup/shutdown, or
		header ownership in unity, module-split, and package-coalesced output.
	**/
	static function managedProjects():Array<GeneratedRootProject> {
		final root = rootFunction("fn.project.root", false);
		root.blocks[0].instructions.resize(0);
		final entry = emptyFunction("fn.project.entry");
		final rootArrays:Map<String, CIdentifier> = [root.id => new CIdentifier("hxc_project_roots")];
		final rootFrames:Map<String, CIdentifier> = [root.id => new CIdentifier("hxc_project_frame")];
		final names = new CManagedProgramNames(new CIdentifier("hxc_project_gc"), new CIdentifier("hxc_project_thread"), rootArrays, rootFrames);
		final emitter = new CBodyEmitter(null, null, null, null, null, null, null, null, null, null, names);
		final rootNames:Map<String, CIdentifier> = ["value.object" => new CIdentifier("value_object")];
		final loweredRoot = loweredFunction(emitter, root, "fixture.ManagedRoots", new CIdentifier("hxc_project_root"), rootNames);
		final loweredEntry = loweredFunction(emitter, entry, "fixture.ManagedRoots", new CIdentifier("hxc_project_entry"), []);
		final preparedDispatch = new CPreparedBodyDispatch(CBodyDispatchGraph.empty(), [], [], [], [], [], []);
		final dispatch = new CLoweredBodyDispatch(preparedDispatch, [], [], [], []);
		final program:HxcIRProgram = {
			schemaVersion: 17,
			dispatch: {layouts: [], slots: [], tables: []},
			modules: [
				{
					id: "fixture.ManagedRoots",
					types: [],
					typeInstances: [],
					globals: [],
					functions: [root, entry],
					source: span()
				}
			]
		};
		final lowered = new CBodyLoweringResult(program, [loweredRoot, loweredEntry], [], [], [], [], [], [], [], [], [], [], dispatch,
			CLoweredImports.empty(), [], [], {
				schemaVersion: 1,
				algorithm: "synthetic-root-frame",
				symbols: [],
				collisions: []
			}, new CIdentifier("abort"), [], names);
		final projects:Array<GeneratedRootProject> = [];
		final printer = new CASTPrinter();
		for (layout in [CProjectLayout.Unity, CProjectLayout.Split, CProjectLayout.Package]) {
			final layoutPlan = new CProjectLayoutPlanner().plan(layout, ["fixture.ManagedRoots"]);
			final guards:Map<String, CIdentifier> = [];
			for (index => path in layoutPlan.headerPaths)
				guards.set(path, new CIdentifier('HXC_GENERATED_ROOT_HEADER_$index'));
			final plan = new CStaticFunctionProjectEmitter().planWithLayout(lowered, entry.id, new CIdentifier("main"), layoutPlan, guards, null, null,
				RuntimeAbiContract.MAJOR);
			projects.push({
				layout: Std.string(layout),
				headers: plan.headers.map(header -> {path: header.path, content: printer.printHeader(header.unit)}),
				sources: plan.sources.map(source -> {path: source.path, content: printer.printTranslationUnit(source.unit)})
			});
		}
		return projects;
	}

	/** Emit one already-planned function record for the project-layout fixture. */
	static function loweredFunction(emitter:CBodyEmitter, fn:HxcIRFunction, modulePath:String, cName:CIdentifier,
			parameterNames:Map<String, CIdentifier>):CLoweredBodyFunction {
		final body = emitter.emitBody(fn, parameterNames, [], [], [], [], [], false, [], [], null, [], new CIdentifier("abort"));
		return new CLoweredBodyFunction(modulePath, modulePath, fn.displayName, fn, cName, parameterNames, [], [], [], [], [], emitter.requiredHeaders(fn),
			body, null);
	}

	/** Build the root-free Haxe entry that brackets collector startup/shutdown. */
	static function emptyFunction(id:String):HxcIRFunction {
		final source = span();
		return {
			id: id,
			displayName: id,
			parameters: [],
			borrowedClassParameterIds: [],
			borrowedClassLocalIds: [],
			managedRoots: [],
			locals: [],
			returnType: IRTVoid,
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [],
					terminator: {kind: IRTReturn(null, []), source: source},
					source: source
				}
			],
			cleanupRegions: [],
			source: source
		};
	}

	/** Build one function whose only managed value is its incoming object. */
	static function rootFunction(id:String, fails:Bool):HxcIRFunction {
		final source = span();
		final call:HxcIRCall = {
			dispatch: IRCDDirect(fails ? "fn.probe.failure" : "fn.probe.success"),
			arguments: ["value.object"],
			returnType: IRTVoid,
			failure: fails ? {
				kind: IRFException,
				target: IRFTPropagate,
				arguments: [],
				cleanup: []
			} : null
		};
		return {
			id: id,
			displayName: id,
			parameters: [{id: "value.object", type: IRTPointer(IRTVoid, true), source: source}],
			borrowedClassParameterIds: [],
			borrowedClassLocalIds: [],
			managedRoots: [
				{
					id: "root.object",
					valueId: "value.object",
					projections: [],
					source: source
				}
			],
			locals: [],
			returnType: IRTVoid,
			failureConvention: fails ? IRFCStatus(IRFException) : IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [
						{
							id: "instruction.collect",
							result: null,
							kind: IRIOCall(call),
							source: source
						}
					],
					terminator: {kind: IRTReturn(null, []), source: source},
					source: source
				}
			],
			cleanupRegions: [],
			source: source
		};
	}

	/** Emit one complete function while preserving the emitter's typed declarator. */
	static function definition(emitter:CBodyEmitter, fn:HxcIRFunction, cName:String, targetName:String):CDecl {
		final parameterNames:Map<String, CIdentifier> = ["value.object" => new CIdentifier("value_object")];
		final functionNames:Map<String, CIdentifier> = [];
		functionNames.set(fn.failureConvention == IRFCInfallible ? "fn.probe.success" : "fn.probe.failure", new CIdentifier(targetName));
		final declarator = emitter.functionDeclarator(fn, DFunction(DName(new CIdentifier(cName)), FPPrototype(emitter.parameters(fn, parameterNames), false)));
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: declarator.type,
			declarator: declarator.declarator,
			body: emitter.emitBody(fn, parameterNames, [], [], functionNames, [], [], false, [], [], null, [], new CIdentifier("abort")),
			attributes: []
		});
	}

	/** Give strict native builds a declaration before each generated definition. */
	static function generatedPrototype(emitter:CBodyEmitter, fn:HxcIRFunction, cName:String):CDecl {
		final parameterNames:Map<String, CIdentifier> = ["value.object" => new CIdentifier("value_object")];
		final declarator = emitter.functionDeclarator(fn, DFunction(DName(new CIdentifier(cName)), FPPrototype(emitter.parameters(fn, parameterNames), false)));
		return DPrototype([SExtern], [], declarator.type, declarator.declarator, []);
	}

	/** Declare the native observation hook called inside the registered frame. */
	static function prototype(name:String, returnsStatus:Bool):CDecl
		return DPrototype([SExtern], [], new CType(returnsStatus ? TBool : TVoid), DFunction(DName(new CIdentifier(name)), FPPrototype([
			{type: new CType(TVoid), declarator: DPointer(DName(new CIdentifier("object")), []), attributes: []}
		], false)), []);

	static function span():HxcSourceSpan
		return new HxcSourceSpan("test/runtime/gc/GeneratedRootFrameProbe.hx", 1, 1, 1, 1);
	#else
	public static function install():Void {}
	#end
}

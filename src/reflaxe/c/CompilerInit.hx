package reflaxe.c;

#if macro
import haxe.macro.Compiler as MacroCompiler;
import haxe.macro.Context;
import reflaxe.c.macros.TypedCContractMacro;
#if !reflaxe_c_lifecycle_probe
import reflaxe.BaseCompiler.BaseCompilerFileOutputType;
import reflaxe.ReflectCompiler;
#end
#end

/** Target-gated, exactly-once Reflaxe registration. */
class CompilerInit {
	public static inline final READY_DEFINE = "reflaxe_c_init_ready";

	#if macro
	static var initialized = false;

	public static function Start():Void {
		if (!BuildDetection.isCBuild()) {
			return;
		}

		if (!Context.defined(CompilerBootstrap.READY_DEFINE)) {
			Context.fatalError("reflaxe.c.CompilerBootstrap.Start() must run before CompilerInit.Start().", Context.currentPos());
		}

		if (initialized) {
			return;
		}
		initialized = true;

		if (Context.defined("target.utf16")) {
			Context.fatalError("The C target is Unicode-capable but not UTF-16; `target.utf16` conflicts with the target contract.", Context.currentPos());
		}

		MacroCompiler.define("c");
		MacroCompiler.define("reflaxe_c");
		MacroCompiler.define("target.unicode");
		MacroCompiler.define(READY_DEFINE);
		TypedCContractMacro.install();

		#if !reflaxe_c_lifecycle_probe
		ReflectCompiler.Start();
		ReflectCompiler.AddCompiler(new CReflaxeCompiler(), {
			outputDirDefineName: "c_output",
			fileOutputType: Manual,
			fileOutputExtension: ".c",
			targetCodeInjectionName: "__c__",
			expressionPreprocessors: [],
			ignoreBodilessFunctions: false,
			ignoreExterns: true,
			trackUsedTypes: true,
			trackClassHierarchy: true,
			deleteOldOutput: true,
			reservedVarNames: []
		});
		#end
	}
	#else
	public static function Start():Void {}
	#end
}

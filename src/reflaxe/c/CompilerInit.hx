package reflaxe.c;

#if macro
import haxe.macro.Compiler as MacroCompiler;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import reflaxe.c.macros.TypedCContractMacro;
#if !reflaxe_c_lifecycle_probe
import reflaxe.BaseCompiler.BaseCompilerFileOutputType;
import reflaxe.ReflectCompiler;
#end
#end

/** Target-gated, exactly-once Reflaxe registration. */
class CompilerInit {
	public static inline final READY_DEFINE = "reflaxe_c_init_ready";
	public static inline final COUNT_DEFINE = "reflaxe_c_init_count";
	public static inline final CARRIER_DEFINE = "reflaxe_c_haxe_carrier";

	#if macro
	static var initialized = false;

	public static function Start():Void {
		if (!BuildDetection.isCBuild()) {
			return;
		}

		if (!Context.defined(CompilerBootstrap.READY_DEFINE)) {
			Context.fatalError("HXC9000: reflaxe.c.CompilerBootstrap.Start() must run before CompilerInit.Start().", configurationPosition());
		}

		if (initialized) {
			return;
		}

		final carrier = carrierName();
		validateTargetIdentity(carrier);

		if (Context.defined("target.utf16")) {
			configurationError('Haxe carrier `$carrier` exposes `target.utf16`, but reflaxe.c requires Unicode-scalar String indexing. '
				+
				"Haxe 4.3.7 Cross facts cannot be removed through its public initialization-macro API; production typing is blocked until the carrier decision is resolved, and the lifecycle probe is test-only.");
		}

		if (Context.defined("target.atomics")) {
			configurationError("`target.atomics` is present before a reflaxe.c platform adapter has proven atomic support; target capabilities must come from the resolved adapter.");
		}

		if (Context.definedValue("hxc_environment") == "freestanding"
			&& (Context.defined("target.sys") || Context.defined("target.threaded"))) {
			configurationError("the freestanding environment conflicts with inherited `target.sys` or `target.threaded` carrier facts; freestanding builds require a capability-clean carrier.");
		}

		initialized = true;
		MacroCompiler.define("target.name", "c");
		MacroCompiler.define("c");
		MacroCompiler.define("reflaxe_c");
		MacroCompiler.define("target.unicode");
		MacroCompiler.define(CARRIER_DEFINE, carrier);
		MacroCompiler.define(COUNT_DEFINE, "1");
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

	static function validateTargetIdentity(carrier:String):Void {
		final targetName = Context.definedValue("target.name");
		final lifecycleProbe = Context.defined("reflaxe_c_lifecycle_probe");

		#if (haxe >= version("5.0.0"))
		final configuration = MacroCompiler.getConfiguration();
		if (configuration != null) {
			switch (configuration.platform) {
				case CustomTarget("c"):
					if (targetName == "c") {
						return;
					}
				case _:
			}
		}
		#end

		if (carrier == "cross" && (targetName == "cross" || targetName == "c")) {
			return;
		}

		if (lifecycleProbe && carrier == "eval" && (targetName == "eval" || targetName == "c")) {
			return;
		}

		configurationError('activation requested C, but Haxe carrier `$carrier` advertises `target.name=${targetName == null ? "<unset>" : targetName}`. '
			+ "A caller-supplied `-D c` is not activation, and unrelated target identities cannot be relabeled as C.");
	}

	static function carrierName():String {
		final configuration = MacroCompiler.getConfiguration();
		return configuration == null ? "unknown" : Std.string(configuration.platform);
	}

	static function configurationError(message:String):Void {
		Context.fatalError('HXC0003: $message', configurationPosition());
	}

	static function configurationPosition():Position {
		final configuration = MacroCompiler.getConfiguration();
		if (configuration != null && configuration.mainClass != null) {
			final mainClass = configuration.mainClass;
			final relativePath = mainClass.pack.concat([mainClass.name]).join("/") + ".hx";
			try {
				final file = Context.resolvePath(relativePath);
				return Context.makePosition({file: file, min: 0, max: 0});
			} catch (_:Dynamic) {}
		}
		return Context.currentPos();
	}
	#else
	public static function Start():Void {}
	#end
}

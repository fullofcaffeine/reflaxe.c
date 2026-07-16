package reflaxe.c;

#if macro
import haxe.macro.Compiler;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import reflaxe.c.CDiagnostic.CDiagnosticId;
#if (haxe >= version("5.0.0-preview.1"))
import haxe.macro.PlatformConfig;
#end
#end

/** Installs the compiler-owned Haxe platform facts for the C custom target. */
class TargetPlatform {
	public static inline final READY_DEFINE = "reflaxe_c_platform_ready";
	public static inline final ENVIRONMENT_DEFINE = "reflaxe_c_platform_environment";

	#if macro
	public static function configure():Void {
		#if (haxe >= version("5.0.0-preview.1"))
		final environment = resolveEnvironment();
		final dynamicType = {pack: ["StdTypes"], name: "Dynamic"};
		final config:PlatformConfig = {
			staticTypeSystem: true,
			sys: environmentSupportsSys(environment),
			capturePolicy: None,
			padNulls: false,
			addFinalReturn: false,
			overloadFunctions: false,
			reservedTypePaths: [],
			supportsFunctionEquality: true,
			usesUtf16: false,
			thisBeforeSuper: true,
			supportsThreads: false,
			supportsUnicode: true,
			supportsRestArgs: false,
			exceptions: {
				nativeThrows: [],
				nativeCatches: [],
				avoidWrapping: true,
				wildcardCatch: dynamicType,
				baseThrow: dynamicType
			},
			scoping: {
				scope: BlockScope,
				flags: []
			},
			supportsAtomics: false
		};

		Compiler.setPlatformConfiguration(config);
		Compiler.define(ENVIRONMENT_DEFINE, environment);
		Compiler.define(READY_DEFINE);
		#else
		CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration,
			"reflaxe.c production typing requires Haxe custom-target platform configuration; Haxe 4 lifecycle probes are test-only.", configurationPosition());
		#end
	}

	public static function environmentSupportsSys(environment:String):Bool {
		return environment == "hosted";
	}

	static function resolveEnvironment():String {
		final requested = Context.definedValue("hxc_environment");
		final environment = requested == null || requested == "" ? "hosted" : requested;
		return switch (environment) {
			case "hosted" | "freestanding" | "wasi" | "emscripten":
				environment;
			case _:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration,
					'unsupported C environment `$environment`; expected hosted, freestanding, wasi, or emscripten.', configurationPosition());
		};
	}

	static function configurationPosition():Position {
		final configuration = Compiler.getConfiguration();
		if (configuration != null && configuration.mainClass != null) {
			final mainClass = configuration.mainClass;
			final relativePath = mainClass.pack.concat([mainClass.name]).join("/") + ".hx";
			try {
				final file = Context.resolvePath(relativePath);
				return Context.makePosition({file: file, min: 0, max: 0});
			} catch (_:haxe.Exception) {}
		}
		return Context.currentPos();
	}
	#else
	public static function configure():Void {}

	public static function environmentSupportsSys(environment:String):Bool {
		return environment == "hosted";
	}
	#end
}

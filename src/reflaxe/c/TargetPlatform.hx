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
	public static inline final ATOMIC_BACKEND_DEFINE = "reflaxe_c_platform_atomic_backend";
	public static inline final THREAD_BACKEND_DEFINE = "reflaxe_c_platform_thread_backend";

	#if macro
	public static function configure():Void {
		#if (haxe >= version("5.0.0-preview.1"))
		final environment = resolveEnvironment();
		final atomicBackend = resolveAtomicBackend();
		final threadBackend = resolveThreadBackend(environment, atomicBackend);
		final dynamicType = {pack: ["StdTypes"], name: "Dynamic"};
		final config:PlatformConfig = {
			staticTypeSystem: true,
			sys: environmentSupportsSys(environment),
			capturePolicy: None,
			// Keep omission visible in TypedExpr calls. Asking Haxe to pad calls
			// with null would erase the difference between `f()` and `f(null)`.
			// The direct-call lowerer instead copies the declaration's typed
			// default, then gives fixed-arity HxcIR and C the complete argument list.
			padNulls: false,
			addFinalReturn: false,
			overloadFunctions: false,
			reservedTypePaths: [],
			supportsFunctionEquality: true,
			usesUtf16: false,
			thisBeforeSuper: true,
			supportsThreads: threadBackend != "none",
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
			supportsAtomics: atomicBackend != "none"
		};

		Compiler.setPlatformConfiguration(config);
		Compiler.define(ENVIRONMENT_DEFINE, environment);
		Compiler.define(ATOMIC_BACKEND_DEFINE, atomicBackend);
		Compiler.define(THREAD_BACKEND_DEFINE, threadBackend);
		Compiler.define(READY_DEFINE);
		#else
		CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration,
			"reflaxe.c production typing requires Haxe custom-target platform configuration; Haxe 4 lifecycle probes are test-only.", configurationPosition());
		#end
	}

	public static function environmentSupportsSys(environment:String):Bool {
		return environment == "hosted";
	}

	/** Report whether the resolved backend proves C11 atomic operations. */
	public static function atomicBackendSupportsAtomics(backend:String):Bool {
		return backend == "c11";
	}

	/** Report whether the resolved adapter proves operating-system threads. */
	public static function threadBackendSupportsThreads(backend:String):Bool {
		return backend == "posix";
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

	/**
		Resolve atomics from an explicit target fact instead of the hosted label.

		A hosted C environment does not guarantee `<stdatomic.h>`. The explicit
		backend keeps unsupported toolchains capability-clean and also permits a
		conforming freestanding toolchain to select atomics without pthreads.
	**/
	static function resolveAtomicBackend():String {
		final requested = Context.definedValue("hxc_atomic_backend");
		final backend = requested == null || requested == "" ? "none" : requested;
		return switch backend {
			case "none" | "c11":
				backend;
			case _:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, 'unsupported C atomic backend `$backend`; expected none or c11.',
					configurationPosition());
		};
	}

	/** Resolve pthreads only for hosted C with C11 runtime coordination. */
	static function resolveThreadBackend(environment:String, atomicBackend:String):String {
		final requested = Context.definedValue("hxc_thread_backend");
		final backend = requested == null || requested == "" ? "none" : requested;
		return switch backend {
			case "none": backend;
			case "posix":
				if (environment != "hosted")
					CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, "the POSIX thread backend requires the hosted C environment.",
						configurationPosition());
				if (atomicBackend != "c11")
					CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration,
						"the POSIX thread backend requires `hxc_atomic_backend=c11` for runtime coordination.", configurationPosition());
				backend;
			case _:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, 'unsupported C thread backend `$backend`; expected none or posix.',
					configurationPosition());
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

	public static function atomicBackendSupportsAtomics(backend:String):Bool {
		return backend == "c11";
	}

	public static function threadBackendSupportsThreads(backend:String):Bool {
		return backend == "posix";
	}
	#end
}

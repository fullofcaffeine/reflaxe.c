package reflaxe.c;

#if macro
import haxe.io.Path;
import haxe.macro.Compiler as MacroCompiler;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import reflaxe.c.CDiagnostic.CDiagnosticId;
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
			CDiagnostic.fatal(CDiagnosticId.InternalCompilerError, "reflaxe.c.CompilerBootstrap.Start() must run before CompilerInit.Start().",
				configurationPosition());
		}

		if (initialized) {
			return;
		}

		final carrier = carrierName();
		validateTargetIdentity(carrier);
		validatePlatformConfiguration(carrier);
		prepareOutputDefine(carrier);

		if (Context.defined("target.utf16")) {
			configurationError('Haxe carrier `$carrier` exposes `target.utf16`, but reflaxe.c requires Unicode-scalar String indexing. '
				+ "Legacy Cross facts cannot be removed safely; use the pinned Haxe 5 `--custom-target c=<output>` carrier.");
		}

		if (Context.defined("target.atomics")) {
			configurationError("`target.atomics` is present before a reflaxe.c platform adapter has proven atomic support; target capabilities must come from the resolved adapter.");
		}

		if (Context.definedValue("hxc_environment") == "freestanding"
			&& (Context.defined("target.sys") || Context.defined("target.threaded"))) {
			configurationError("the freestanding environment conflicts with inherited `target.sys` or `target.threaded` carrier facts; freestanding builds require a capability-clean carrier.");
		}

		initialized = true;
		if (Context.definedValue("target.name") != "c") {
			MacroCompiler.define("target.name", "c");
		}
		MacroCompiler.define("c");
		MacroCompiler.define("reflaxe_c");
		if (!Context.defined("target.unicode")) {
			MacroCompiler.define("target.unicode");
		}
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

		if (carrier == "c" && targetName == "c") {
			return;
		}

		if (carrier == "cross" && (targetName == "cross" || targetName == "c")) {
			return;
		}

		if (lifecycleProbe && carrier == "eval" && (targetName == "eval" || targetName == "c")) {
			return;
		}

		configurationError('activation requested C, but Haxe carrier `$carrier` advertises `target.name=${targetName == null ? "<unset>" : targetName}`. '
			+ "A caller-supplied `-D c` is not activation, and unrelated target identities cannot be relabeled as C.");
	}

	static function validatePlatformConfiguration(carrier:String):Void {
		if (carrier != "c") {
			return;
		}
		if (!Context.defined(TargetPlatform.READY_DEFINE)) {
			configurationError("the C custom target did not run its compiler-owned `c.Init.init()` platform configuration.");
		}

		final environment = Context.definedValue(TargetPlatform.ENVIRONMENT_DEFINE);
		if (environment == null) {
			configurationError("the C custom target did not record its resolved environment.");
		}
		final expectsSys = TargetPlatform.environmentSupportsSys(cast environment);
		final configuration = MacroCompiler.getConfiguration();
		final platform = configuration == null ? null : configuration.platformConfig;
		if (platform == null
			|| Reflect.field(platform, "staticTypeSystem") != true
			|| Reflect.field(platform, "supportsUnicode") != true
			|| Reflect.field(platform, "usesUtf16") != false
			|| Reflect.field(platform, "sys") != expectsSys
			|| Reflect.field(platform, "supportsThreads") != false
			|| Reflect.field(platform, "supportsAtomics") != false) {
			configurationError("the compiler platform configuration drifted from static, scalar-Unicode, adapter-derived C semantics.");
		}
		if (!Context.defined("target.static") || !Context.defined("target.unicode")) {
			configurationError("the C custom target did not publish its required static and Unicode platform facts.");
		}
		if (Context.defined("target.sys") != expectsSys) {
			configurationError('the `$environment` adapter and `target.sys` disagree; platform facts must come from the resolved C adapter.');
		}
		if (Context.defined("target.threaded")) {
			configurationError("`target.threaded` is present before a reflaxe.c platform adapter has proven thread support.");
		}
	}

	static function prepareOutputDefine(carrier:String):Void {
		if (carrier != "c") {
			return;
		}
		final output = MacroCompiler.getOutput();
		if (output == null || StringTools.trim(output) == "") {
			configurationError("the C custom target requires an output directory: use `--custom-target c=<output>`.");
		}

		final existing = Context.definedValue("c_output");
		if (existing == null || existing == "") {
			MacroCompiler.define("c_output", output);
			return;
		}
		if (Path.normalize(existing) != Path.normalize(output)) {
			configurationError('`c_output=$existing` conflicts with the custom-target output `$output`; declare one output directory.');
		}
	}

	static function carrierName():String {
		final configuration = MacroCompiler.getConfiguration();
		if (configuration == null) {
			return "unknown";
		}
		final raw = Std.string(configuration.platform);
		if (StringTools.startsWith(raw, "CustomTarget(") && StringTools.endsWith(raw, ")")) {
			return raw.substr("CustomTarget(".length, raw.length - "CustomTarget(".length - 1);
		}
		return raw.toLowerCase();
	}

	static function configurationError(message:String):Void {
		CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, message, configurationPosition());
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

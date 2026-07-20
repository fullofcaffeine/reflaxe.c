import haxe.Json;
import haxe.io.Path;
import hxc.config.HxcArtifactKind;
import hxc.config.HxcCExtensionPolicy;
import hxc.config.HxcCStandard;
import hxc.config.HxcConfigError;
import hxc.config.HxcConfigLoader;
import hxc.config.HxcConfigParser;
import hxc.config.HxcConfigPatch;
import hxc.config.HxcConfigResolutionRequest;
import hxc.config.HxcConfigResolver;
import hxc.config.HxcDirectDefine;
import hxc.config.HxcEffectiveConfig;
import hxc.config.HxcLoadedProjectConfig;
import hxc.config.HxcProjectLayout;
import reflaxe.c.CBuildMode;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;

/** Eval bootstrap probe for the target-neutral configuration core. */
class ConfigProbe {
	static inline final REPORT_PREFIX = "HXC_CONFIG_REPORT=";
	static inline final CONTRACT_PREFIX = "HXC_CONFIG_CONTRACT=";
	static inline final HOST_PREFIX = "HXC_CONFIG_HOST_PATHS=";

	static function main():Void {
		final arguments = Sys.args();
		if (arguments.length == 0) {
			throw "ConfigProbe requires a mode";
		}
		final mode = arguments[0];
		if (arguments.length >= 3) {
			Sys.setCwd(arguments[2]);
		}
		switch mode {
			case "contract":
				Sys.println(CONTRACT_PREFIX + Json.stringify(HxcConfigParser.contractSnapshot()));
			case "resolve" | "resolve-reversed":
				final loaded = requireProject(arguments);
				final config = resolveFull(loaded, mode == "resolve-reversed");
				selfTest();
				Sys.println(REPORT_PREFIX + config.toJson());
			case "defaults":
				final config = HxcConfigResolver.resolve(new HxcConfigResolutionRequest());
				Sys.println(REPORT_PREFIX + config.toJson());
			case "host-paths":
				final loaded = requireProject(arguments);
				final config = resolveFull(loaded, false);
				Sys.println(HOST_PREFIX + loaded.resolveHostPath(config.hxml.value) + "\n" + loaded.resolveHostPath(config.output.value));
			case "invalid-overlay":
				final loaded = requireProject(arguments);
				HxcConfigResolver.resolve(new HxcConfigResolutionRequest(loaded, null, null, "missing"));
			case "invalid-direct-duplicate":
				HxcConfigResolver.resolve(new HxcConfigResolutionRequest(null, null, null, null, null, [
					new HxcDirectDefine("hxc_runtime", "auto"),
					new HxcDirectDefine("hxc_runtime", "none")
				]));
			case "invalid-direct-unknown":
				HxcConfigResolver.resolve(new HxcConfigResolutionRequest(null, null, null, null, null, [new HxcDirectDefine("hxc_runtme", "none")]));
			case "invalid-freestanding-shared":
				HxcConfigResolver.resolve(new HxcConfigResolutionRequest(null, null, null, null,
					new HxcConfigPatch({environment: CEnvironment.Freestanding, artifact: HxcArtifactKind.SharedLibrary})));
			case "invalid-output-owns-hxml":
				HxcConfigResolver.resolve(new HxcConfigResolutionRequest(null, null, null, null,
					new HxcConfigPatch({hxml: new hxc.config.HxcProjectPath("build/c/project.hxml"), output: new hxc.config.HxcProjectPath("build/c")})));
			case _:
				throw 'unknown ConfigProbe mode `$mode`';
		}
		Sys.println("hxc-config-haxe: OK");
	}

	static function requireProject(arguments:Array<String>):HxcLoadedProjectConfig {
		if (arguments.length < 2) {
			throw "ConfigProbe mode requires an absolute config path";
		}
		return HxcConfigLoader.load(arguments[1]);
	}

	static function resolveFull(project:HxcLoadedProjectConfig, reverseDefines:Bool):HxcEffectiveConfig {
		final environmentPreset = new HxcConfigPatch({
			cStandard: HxcCStandard.C17,
			cExtensions: HxcCExtensionPolicy.Gnu,
			output: new hxc.config.HxcProjectPath("preset/c")
		});
		final cli = new HxcConfigPatch({
			profile: CProfile.Portable,
			cStandard: HxcCStandard.C17,
			artifact: HxcArtifactKind.StaticLibrary
		});
		final defines = [
			new HxcDirectDefine("hxc_build", "minsizerel"),
			new HxcDirectDefine("hxc_c_standard", "c23"),
			new HxcDirectDefine("hxc_project_layout", "unity"),
			new HxcDirectDefine("hxc_runtime", "none"),
			new HxcDirectDefine("ordinary_haxe_define", "kept-outside-hxc")
		];
		if (reverseDefines) {
			defines.reverse();
		}
		return HxcConfigResolver.resolve(new HxcConfigResolutionRequest(project, environmentPreset, "host-toolchain", "dev", cli, defines));
	}

	static function selfTest():Void {
		final defaults = HxcConfigResolver.resolve(new HxcConfigResolutionRequest());
		require(Std.string(defaults.profile.value) == "portable", "default profile");
		require(Std.string(defaults.runtime.value) == "auto", "portable runtime preset");
		require(Std.string(defaults.runtimeDiagnostics.value) == "summary", "portable diagnostic preset");
		require(Std.string(defaults.artifact.value) == "executable", "default artifact");
		require(defaults.projectLayout.value == HxcProjectLayout.Split, "default split project layout");
		require(HxcConfigParser.projectLayout("package") == HxcProjectLayout.Package, "explicit package project layout");

		final metal = HxcConfigResolver.resolve(new HxcConfigResolutionRequest(null, null, null, null, new HxcConfigPatch({profile: CProfile.Metal})));
		require(Std.string(metal.runtime.value) == "minimal", "metal runtime preset");
		require(Std.string(metal.runtimeDiagnostics.value) == "warn", "metal diagnostic preset");

		final portableNone = HxcConfigResolver.resolve(new HxcConfigResolutionRequest(null, null, null, null,
			new HxcConfigPatch({profile: CProfile.Portable, runtime: CRuntimePolicy.None})));
		require(portableNone.runtime.value == CRuntimePolicy.None, "portable explicit none remains valid");
		final metalAuto = HxcConfigResolver.resolve(new HxcConfigResolutionRequest(null, null, null, null,
			new HxcConfigPatch({profile: CProfile.Metal, runtime: CRuntimePolicy.Auto, runtimeDiagnostics: CRuntimeDiagnostics.Off})));
		require(metalAuto.runtime.value == CRuntimePolicy.Auto, "metal explicit auto remains valid");

		final lowerExplicit = HxcConfigResolver.resolve(new HxcConfigResolutionRequest(null, new HxcConfigPatch({runtime: CRuntimePolicy.Auto}),
			"embedded-adapter", null, new HxcConfigPatch({profile: CProfile.Metal})));
		require(lowerExplicit.runtime.value == CRuntimePolicy.Auto, "independent lower-precedence runtime survives a later profile");
		require(lowerExplicit.runtime.origin.detail == "embedded-adapter", "lower-precedence runtime provenance");

		final unicode = HxcConfigParser.parse('{"schemaVersion":1,"hxml":"source/\\uD83D\\uDE00.hxml"}');
		require(unicode.patch.hxml != null && unicode.patch.hxml.value == "source/😀.hxml", "Unicode surrogate-pair decoding");
	}

	static function require(condition:Bool, label:String):Void {
		if (!condition) {
			throw new HxcConfigError('configuration self-test failed: $label');
		}
	}
}

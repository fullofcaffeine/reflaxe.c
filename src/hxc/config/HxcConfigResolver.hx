package hxc.config;

import hxc.config.HxcConfigOrigin.HxcConfigOriginKind;
import reflaxe.c.CBuildMode;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;

private class HxcResolutionState {
	public var hxml:HxcResolvedSetting<HxcProjectPath>;
	public var output:HxcResolvedSetting<HxcProjectPath>;
	public var profile:HxcResolvedSetting<CProfile>;
	public var runtime:HxcResolvedSetting<CRuntimePolicy>;
	public var runtimeDiagnostics:HxcResolvedSetting<CRuntimeDiagnostics>;
	public var environment:HxcResolvedSetting<CEnvironment>;
	public var cStandard:HxcResolvedSetting<HxcCStandard>;
	public var cExtensions:HxcResolvedSetting<HxcCExtensionPolicy>;
	public var build:HxcResolvedSetting<CBuildMode>;
	public var artifact:HxcResolvedSetting<HxcArtifactKind>;
	public var runtimeExplicit:Bool = false;
	public var runtimeDiagnosticsExplicit:Bool = false;

	public function new() {
		final defaultOrigin = new HxcConfigOrigin(HxcConfigOriginKind.CompilerDefault, "built-in");
		hxml = new HxcResolvedSetting(new HxcProjectPath("build.hxml", "default hxml"), defaultOrigin);
		output = new HxcResolvedSetting(new HxcProjectPath("build/c", "default output"), defaultOrigin);
		profile = new HxcResolvedSetting(CProfile.Portable, defaultOrigin);
		runtime = new HxcResolvedSetting(CRuntimePolicy.Auto, new HxcConfigOrigin(HxcConfigOriginKind.ProfilePreset, "portable"));
		runtimeDiagnostics = new HxcResolvedSetting(CRuntimeDiagnostics.Summary, new HxcConfigOrigin(HxcConfigOriginKind.ProfilePreset, "portable"));
		environment = new HxcResolvedSetting(CEnvironment.Hosted, defaultOrigin);
		cStandard = new HxcResolvedSetting(HxcCStandard.C11, defaultOrigin);
		cExtensions = new HxcResolvedSetting(HxcCExtensionPolicy.None, defaultOrigin);
		build = new HxcResolvedSetting(CBuildMode.Debug, defaultOrigin);
		artifact = new HxcResolvedSetting(HxcArtifactKind.Executable, defaultOrigin);
	}
}

/** Applies every configuration source in the documented precedence order. */
class HxcConfigResolver {
	public static function resolve(request:HxcConfigResolutionRequest):HxcEffectiveConfig {
		final state = new HxcResolutionState();
		if (request.environmentPreset != null && request.environmentPresetName != null) {
			applyPatch(state, request.environmentPreset, HxcConfigOriginKind.EnvironmentPreset, request.environmentPresetName);
		}
		if (request.project != null) {
			applyPatch(state, request.project.config.patch, HxcConfigOriginKind.ProjectFile, request.project.configSource);
		}
		if (request.selectedOverlay != null && request.project != null) {
			applyPatch(state, request.project.config.overlay(request.selectedOverlay), HxcConfigOriginKind.NamedOverlay, request.selectedOverlay);
		}
		if (request.cli != null) {
			applyPatch(state, request.cli, HxcConfigOriginKind.CliFlag, "explicit");
		}
		applyDirectDefines(state, request.directDefines);
		resolveProfilePresets(state);
		final effective = new HxcEffectiveConfig(state.hxml, state.output, state.profile, state.runtime, state.runtimeDiagnostics, state.environment,
			state.cStandard, state.cExtensions, state.build, state.artifact, request.project == null ? null : request.project.configSource,
			request.selectedOverlay);
		HxcConfigRules.validateEffective(effective);
		return effective;
	}

	static function applyPatch(state:HxcResolutionState, patch:HxcConfigPatch, kind:HxcConfigOriginKind, detail:String):Void {
		if (patch.hxml != null) {
			state.hxml = new HxcResolvedSetting(patch.hxml, origin(kind, detailFor(kind, detail, "hxml")));
		}
		if (patch.output != null) {
			state.output = new HxcResolvedSetting(patch.output, origin(kind, detailFor(kind, detail, "output")));
		}
		if (patch.profile != null) {
			state.profile = new HxcResolvedSetting(patch.profile, origin(kind, detailFor(kind, detail, "profile")));
		}
		if (patch.runtime != null) {
			state.runtime = new HxcResolvedSetting(patch.runtime, origin(kind, detailFor(kind, detail, "runtime")));
			state.runtimeExplicit = true;
		}
		if (patch.runtimeDiagnostics != null) {
			state.runtimeDiagnostics = new HxcResolvedSetting(patch.runtimeDiagnostics, origin(kind, detailFor(kind, detail, "runtimeDiagnostics")));
			state.runtimeDiagnosticsExplicit = true;
		}
		if (patch.environment != null) {
			state.environment = new HxcResolvedSetting(patch.environment, origin(kind, detailFor(kind, detail, "environment")));
		}
		if (patch.cStandard != null) {
			state.cStandard = new HxcResolvedSetting(patch.cStandard, origin(kind, detailFor(kind, detail, "cStandard")));
		}
		if (patch.cExtensions != null) {
			state.cExtensions = new HxcResolvedSetting(patch.cExtensions, origin(kind, detailFor(kind, detail, "cExtensions")));
		}
		if (patch.build != null) {
			state.build = new HxcResolvedSetting(patch.build, origin(kind, detailFor(kind, detail, "build")));
		}
		if (patch.artifact != null) {
			state.artifact = new HxcResolvedSetting(patch.artifact, origin(kind, detailFor(kind, detail, "artifact")));
		}
	}

	static function applyDirectDefines(state:HxcResolutionState, inputs:Array<HxcDirectDefine>):Void {
		final defines = inputs.copy();
		defines.sort((left, right) -> HxcProjectConfig.compareUtf8(left.name, right.name));
		var prior:Null<String> = null;
		for (define in defines) {
			if (!ownsDefine(define.name)) {
				continue;
			}
			if (define.name == prior) {
				throw new HxcConfigError('duplicate direct Haxe define `${define.name}`', "command-line");
			}
			prior = define.name;
			final source = "command-line";
			final origin = new HxcConfigOrigin(HxcConfigOriginKind.DirectDefine, define.name);
			switch define.name {
				case "reflaxe_c_profile":
					state.profile = new HxcResolvedSetting(HxcConfigParser.profile(define.value, source), origin);
				case "hxc_runtime":
					state.runtime = new HxcResolvedSetting(HxcConfigParser.runtime(define.value, source), origin);
					state.runtimeExplicit = true;
				case "hxc_runtime_diagnostics":
					state.runtimeDiagnostics = new HxcResolvedSetting(HxcConfigParser.runtimeDiagnostics(define.value, source), origin);
					state.runtimeDiagnosticsExplicit = true;
				case "hxc_environment":
					state.environment = new HxcResolvedSetting(HxcConfigParser.environment(define.value, source), origin);
				case "hxc_c_standard":
					state.cStandard = new HxcResolvedSetting(HxcConfigParser.cStandard(define.value, source), origin);
				case "hxc_c_extensions":
					state.cExtensions = new HxcResolvedSetting(HxcConfigParser.cExtensions(define.value, source), origin);
				case "hxc_build":
					state.build = new HxcResolvedSetting(HxcConfigParser.build(define.value, source), origin);
				case name if (StringTools.startsWith(name, "hxc_") || StringTools.startsWith(name, "reflaxe_c_profile")):
					throw new HxcConfigError('unknown configuration define `$name`', source);
				case _:
					throw new HxcConfigError('unclassified configuration define `${define.name}`', source);
			}
		}
	}

	static function resolveProfilePresets(state:HxcResolutionState):Void {
		final profile = state.profile.value;
		final profileName = Std.string(profile);
		final presetOrigin = new HxcConfigOrigin(HxcConfigOriginKind.ProfilePreset, profileName);
		if (!state.runtimeExplicit) {
			state.runtime = new HxcResolvedSetting(profile == CProfile.Portable ? CRuntimePolicy.Auto : CRuntimePolicy.Minimal, presetOrigin);
		}
		if (!state.runtimeDiagnosticsExplicit) {
			state.runtimeDiagnostics = new HxcResolvedSetting(profile == CProfile.Portable ? CRuntimeDiagnostics.Summary : CRuntimeDiagnostics.Warn,
				presetOrigin);
		}
	}

	static function origin(kind:HxcConfigOriginKind, detail:String):HxcConfigOrigin
		return new HxcConfigOrigin(kind, detail);

	static function detailFor(kind:HxcConfigOriginKind, detail:String, field:String):String {
		return kind == HxcConfigOriginKind.CliFlag ? "--" + switch field {
			case "runtimeDiagnostics": "runtime-diagnostics";
			case "cStandard": "c-standard";
			case "cExtensions": "c-extensions";
			case _: field;
		} : detail;
	}

	static function ownsDefine(name:String):Bool {
		return name == "reflaxe_c_profile" || StringTools.startsWith(name, "hxc_");
	}
}

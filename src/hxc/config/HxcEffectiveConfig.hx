package hxc.config;

import haxe.Json;
import hxc.config.HxcConfigOrigin.HxcConfigOriginJson;
import reflaxe.c.CBuildMode;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;

typedef HxcInspectedSetting = {
	final value:String;
	final source:HxcConfigOriginJson;
}

typedef HxcEffectiveConfigJson = {
	final hxml:HxcInspectedSetting;
	final output:HxcInspectedSetting;
	final profile:HxcInspectedSetting;
	final runtime:HxcInspectedSetting;
	final runtimeDiagnostics:HxcInspectedSetting;
	final environment:HxcInspectedSetting;
	final cStandard:HxcInspectedSetting;
	final projectLayout:HxcInspectedSetting;
	final cExtensions:HxcInspectedSetting;
	final build:HxcInspectedSetting;
	final artifact:HxcInspectedSetting;
}

typedef HxcConfigInspectionJson = {
	final schemaVersion:Int;
	final configSchemaVersion:Int;
	final configurationFile:Null<String>;
	final selectedOverlay:Null<String>;
	final precedence:Array<String>;
	final effective:HxcEffectiveConfigJson;
}

/** Fully resolved configuration; no field can escape without provenance. */
class HxcEffectiveConfig {
	public static inline final INSPECTION_SCHEMA_VERSION = 1;

	public final hxml:HxcResolvedSetting<HxcProjectPath>;
	public final output:HxcResolvedSetting<HxcProjectPath>;
	public final profile:HxcResolvedSetting<CProfile>;
	public final runtime:HxcResolvedSetting<CRuntimePolicy>;
	public final runtimeDiagnostics:HxcResolvedSetting<CRuntimeDiagnostics>;
	public final environment:HxcResolvedSetting<CEnvironment>;
	public final cStandard:HxcResolvedSetting<HxcCStandard>;
	public final projectLayout:HxcResolvedSetting<HxcProjectLayout>;
	public final cExtensions:HxcResolvedSetting<HxcCExtensionPolicy>;
	public final build:HxcResolvedSetting<CBuildMode>;
	public final artifact:HxcResolvedSetting<HxcArtifactKind>;
	public final configurationFile:Null<String>;
	public final selectedOverlay:Null<String>;

	public function new(hxml:HxcResolvedSetting<HxcProjectPath>, output:HxcResolvedSetting<HxcProjectPath>, profile:HxcResolvedSetting<CProfile>,
			runtime:HxcResolvedSetting<CRuntimePolicy>, runtimeDiagnostics:HxcResolvedSetting<CRuntimeDiagnostics>,
			environment:HxcResolvedSetting<CEnvironment>, cStandard:HxcResolvedSetting<HxcCStandard>, projectLayout:HxcResolvedSetting<HxcProjectLayout>,
			cExtensions:HxcResolvedSetting<HxcCExtensionPolicy>, build:HxcResolvedSetting<CBuildMode>, artifact:HxcResolvedSetting<HxcArtifactKind>,
			configurationFile:Null<String>, selectedOverlay:Null<String>) {
		this.hxml = hxml;
		this.output = output;
		this.profile = profile;
		this.runtime = runtime;
		this.runtimeDiagnostics = runtimeDiagnostics;
		this.environment = environment;
		this.cStandard = cStandard;
		this.projectLayout = projectLayout;
		this.cExtensions = cExtensions;
		this.build = build;
		this.artifact = artifact;
		this.configurationFile = configurationFile;
		this.selectedOverlay = selectedOverlay;
	}

	public function toJsonValue():HxcConfigInspectionJson {
		return {
			schemaVersion: INSPECTION_SCHEMA_VERSION,
			configSchemaVersion: HxcProjectConfig.SCHEMA_VERSION,
			configurationFile: configurationFile,
			selectedOverlay: selectedOverlay,
			precedence: [
				"compiler-default",
				"environment-preset",
				"project-file",
				"named-overlay",
				"cli-flag",
				"direct-define"
			],
			effective: {
				hxml: inspected(hxml.value.value, hxml.origin),
				output: inspected(output.value.value, output.origin),
				profile: inspected(Std.string(profile.value), profile.origin),
				runtime: inspected(Std.string(runtime.value), runtime.origin),
				runtimeDiagnostics: inspected(Std.string(runtimeDiagnostics.value), runtimeDiagnostics.origin),
				environment: inspected(Std.string(environment.value), environment.origin),
				cStandard: inspected(Std.string(cStandard.value), cStandard.origin),
				projectLayout: inspected(Std.string(projectLayout.value), projectLayout.origin),
				cExtensions: inspected(Std.string(cExtensions.value), cExtensions.origin),
				build: inspected(Std.string(build.value), build.origin),
				artifact: inspected(Std.string(artifact.value), artifact.origin)
			}
		};
	}

	public function toJson():String
		return Json.stringify(toJsonValue(), null, "  ") + "\n";

	static function inspected(value:String, origin:HxcConfigOrigin):HxcInspectedSetting
		return {value: value, source: origin.toJsonValue()};
}

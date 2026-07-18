package hxc.config;

import reflaxe.c.CBuildMode;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;

/** Closed optional fields admitted at any non-default precedence layer. */
typedef HxcConfigPatchData = {
	final ?hxml:HxcProjectPath;
	final ?output:HxcProjectPath;
	final ?profile:CProfile;
	final ?runtime:CRuntimePolicy;
	final ?runtimeDiagnostics:CRuntimeDiagnostics;
	final ?environment:CEnvironment;
	final ?cStandard:HxcCStandard;
	final ?cExtensions:HxcCExtensionPolicy;
	final ?build:CBuildMode;
	final ?artifact:HxcArtifactKind;
}

/** Immutable, typed partial configuration layer. */
class HxcConfigPatch {
	public final hxml:Null<HxcProjectPath>;
	public final output:Null<HxcProjectPath>;
	public final profile:Null<CProfile>;
	public final runtime:Null<CRuntimePolicy>;
	public final runtimeDiagnostics:Null<CRuntimeDiagnostics>;
	public final environment:Null<CEnvironment>;
	public final cStandard:Null<HxcCStandard>;
	public final cExtensions:Null<HxcCExtensionPolicy>;
	public final build:Null<CBuildMode>;
	public final artifact:Null<HxcArtifactKind>;

	public function new(data:HxcConfigPatchData) {
		this.hxml = data.hxml;
		this.output = data.output;
		this.profile = data.profile;
		this.runtime = data.runtime;
		this.runtimeDiagnostics = data.runtimeDiagnostics;
		this.environment = data.environment;
		this.cStandard = data.cStandard;
		this.cExtensions = data.cExtensions;
		this.build = data.build;
		this.artifact = data.artifact;
	}

	public static function empty():HxcConfigPatch
		return new HxcConfigPatch({});
}

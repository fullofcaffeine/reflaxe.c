package hxc.config;

import reflaxe.c.CEnvironment;

/** Cross-field checks shared by parsed layers and the final effective model. */
class HxcConfigRules {
	public static function validatePatch(patch:HxcConfigPatch):Void {
		if (patch.hxml != null && !StringTools.endsWith(patch.hxml.value, ".hxml")) {
			throw new HxcConfigError('hxml path must end in `.hxml`: `${patch.hxml.value}`');
		}
		if (patch.hxml != null && patch.output != null) {
			validateInputAndOutput(patch.hxml, patch.output);
		}
		if (patch.environment != null && patch.artifact != null) {
			validateEnvironmentArtifact(patch.environment, patch.artifact);
		}
		if (patch.environment != null && patch.cExtensions != null) {
			validateEnvironmentExtensions(patch.environment, patch.cExtensions);
		}
	}

	public static function validateEffective(config:HxcEffectiveConfig):Void {
		if (!StringTools.endsWith(config.hxml.value.value, ".hxml")) {
			throw new HxcConfigError('effective hxml path must end in `.hxml`: `${config.hxml.value.value}`');
		}
		validateInputAndOutput(config.hxml.value, config.output.value);
		validateEnvironmentArtifact(config.environment.value, config.artifact.value);
		validateEnvironmentExtensions(config.environment.value, config.cExtensions.value);
	}

	static function validateInputAndOutput(hxml:HxcProjectPath, output:HxcProjectPath):Void {
		if (hxml.value == output.value || StringTools.startsWith(hxml.value, output.value + "/")) {
			throw new HxcConfigError("output directory cannot own or equal the authoritative HXML input");
		}
	}

	static function validateEnvironmentArtifact(environment:CEnvironment, artifact:HxcArtifactKind):Void {
		if (environment == CEnvironment.Freestanding && artifact == HxcArtifactKind.SharedLibrary) {
			throw new HxcConfigError("freestanding configuration cannot request a hosted shared-library artifact");
		}
	}

	static function validateEnvironmentExtensions(environment:CEnvironment, extensions:HxcCExtensionPolicy):Void {
		if (environment != CEnvironment.Hosted && extensions == HxcCExtensionPolicy.Msvc) {
			throw new HxcConfigError("the MSVC extension policy requires a hosted environment adapter");
		}
	}
}

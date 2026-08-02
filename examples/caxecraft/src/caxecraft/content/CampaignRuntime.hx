package caxecraft.content;

import caxecraft.content.CampaignManifest.CampaignLevel;
import caxecraft.content.CampaignManifest.CampaignManifestReadResult;
import caxecraft.content.CampaignManifest.decodeCampaignManifest;
import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.ResolvedLevelPlan.LevelPlayerOptions;
import caxecraft.content.RuntimeContentDigest.runtimeSha256;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelCandidate;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelLoadError;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelLoadResult;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelSource;
import caxecraft.content.RuntimeLevelLoader.loadRuntimeLevel;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.scenario.ScenarioContentRegistry;

/**
 * Connects an admitted campaign index to the existing runtime map loader.
 *
 * This module owns no filesystem path and never receives active gameplay.
 * Callers supply a confined package store, then choose one already-validated
 * level record. The selected bytes must match the manifest's independent
 * length and SHA-256 expectations before the shared CAXEMAP pipeline may build
 * a private candidate. `ActiveContent` remains the sole publication owner.
 */
/** Why a package campaign document could not become a typed manifest. */
enum CampaignPackageLoadError {
	/** The confined package store rejected the manifest read. */
	CampaignManifestSourceRejected(error:ContentPackageError);

	/** The bounded JSON/schema/graph decoder rejected the manifest. */
	CampaignManifestSchemaRejected(diagnostic:RuntimeSchemaDiagnostic);
}

/** One complete typed package manifest, or a closed source/schema rejection. */
enum CampaignPackageLoadResult {
	/** The package bytes produced one complete immutable campaign. */
	CampaignPackageReady(manifest:CampaignManifest);

	/** No partial campaign escaped. */
	CampaignPackageRejected(error:CampaignPackageLoadError);
}

/** Why one selected campaign map did not become a playable candidate. */
enum CampaignLevelLoadError {
	/** The confined package store rejected the selected map read. */
	CampaignLevelSourceRejected(logicalPath:String, error:ContentPackageError);

	/** The file changed relative to the manifest's reviewed byte count. */
	CampaignLevelLengthMismatch(logicalPath:String, expected:Int, actual:Int);

	/** The file bytes changed relative to the manifest's reviewed SHA-256. */
	CampaignLevelHashMismatch(logicalPath:String, expected:String);

	/** The shared parser, validator, resolver, or generation builder rejected. */
	CampaignLevelRuntimeRejected(logicalPath:String, error:RuntimeLevelLoadError);
}

/** One complete unpublished level candidate, or one fail-closed rejection. */
enum CampaignLevelLoadResult {
	/** Exact admitted bytes traversed the ordinary runtime-level pipeline. */
	CampaignLevelReady(candidate:RuntimeLevelCandidate);

	/** No candidate escaped and no active owner was available to mutate. */
	CampaignLevelRejected(error:CampaignLevelLoadError);
}

/** Read and decode one campaign manifest below an opened package root. */
function loadCampaignManifest(store:ContentPackageStore, logicalPath:String):CampaignPackageLoadResult {
	return switch store.read(logicalPath) {
		case PackageBytesRejected(error):
			CampaignPackageRejected(CampaignManifestSourceRejected(error));
		case PackageBytesRead(content):
			switch decodeCampaignManifest(content.bytes) {
				case CampaignManifestRejected(diagnostic):
					CampaignPackageRejected(CampaignManifestSchemaRejected(diagnostic));
				case CampaignManifestReady(manifest):
					CampaignPackageReady(manifest);
			};
	};
}

/**
 * Verify and build one selected level without touching active gameplay.
 *
 * The runtime registry implements both narrow interfaces: semantic validation
 * checks authored content names, and level resolution chooses concrete engine
 * facts. Keeping both parameters explicit prevents a campaign from laundering
 * a map through a different pack than the one active at the transition.
 */
function loadCampaignLevel(store:ContentPackageStore, level:CampaignLevel, generationId:ContentGenerationId, validationRegistry:ScenarioContentRegistry,
		resolutionRegistry:LevelContentResolver, playerOptions:LevelPlayerOptions):CampaignLevelLoadResult {
	return switch store.read(level.logicalPath) {
		case PackageBytesRejected(error):
			CampaignLevelRejected(CampaignLevelSourceRejected(level.logicalPath, error));
		case PackageBytesRead(content):
			if (content.bytes.length != level.byteLength) {
				CampaignLevelRejected(CampaignLevelLengthMismatch(level.logicalPath, level.byteLength, content.bytes.length));
			} else if (!sha256Matches(content.bytes, level.sha256)) {
				CampaignLevelRejected(CampaignLevelHashMismatch(level.logicalPath, level.sha256));
			} else {
				switch loadRuntimeLevel(AdmittedPackageBytes(content), generationId, validationRegistry, resolutionRegistry, playerOptions) {
					case RuntimeLevelReady(candidate): CampaignLevelReady(candidate);
					case RuntimeLevelRejected(error): CampaignLevelRejected(CampaignLevelRuntimeRejected(level.logicalPath, error));
				};
			}
	};
}

/** Compare exact digest bytes with a lowercase manifest expectation. */
private function sha256Matches(input:haxe.io.Bytes, expected:String):Bool {
	if (expected.length != 64)
		return false;
	final digest = runtimeSha256(input);
	if (digest.length != 32)
		return false;
	for (index in 0...digest.length) {
		final high = hexDigit(expected.charCodeAt(index * 2));
		final low = hexDigit(expected.charCodeAt(index * 2 + 1));
		if (high < 0 || low < 0 || digest.get(index) != high * 16 + low)
			return false;
	}
	return true;
}

/** Decode one lowercase hexadecimal digit after schema validation. */
private inline function hexDigit(code:Int):Int
	return code >= 0x30 && code <= 0x39 ? code - 0x30 : code >= 0x61 && code <= 0x66 ? code - 0x61 + 10 : -1;

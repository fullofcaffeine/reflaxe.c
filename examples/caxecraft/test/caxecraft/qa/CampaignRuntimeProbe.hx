package caxecraft.qa;

import caxecraft.app.ActivePlayableLevel;
import caxecraft.app.ActivePlayableLevel.PlayableLevelCreationResult;
import caxecraft.app.ActivePlayableLevel.PlayableLevelPublicationResult;
import caxecraft.content.CampaignManifest.CampaignManifestReadResult;
import caxecraft.content.CampaignManifest.decodeCampaignManifest;
import caxecraft.content.CampaignRuntime.CampaignLevelLoadResult;
import caxecraft.content.CampaignRuntime.CampaignPackageLoadResult;
import caxecraft.content.CampaignRuntime.loadCampaignLevel;
import caxecraft.content.CampaignRuntime.loadCampaignManifest;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.ResolvedLevelPlan.LevelPlayerOptions;
import caxecraft.content.RuntimeContentGeneration.RuntimeContentLoadResult;
import caxecraft.content.RuntimeContentGeneration.RuntimeContentPlayerOptions;
import caxecraft.content.RuntimeContentGeneration.loadRuntimeContent;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelLoadError;
import caxecraft.content.RuntimeSchema.RuntimeSchemaErrorKind;
import caxecraft.domain.EntityId;
import caxecraft.domain.Vitals.MAX_HEALTH;
import haxe.io.Bytes;

/**
 * Proves the first real two-level campaign from package bytes to publication.
 *
 * Scenario table and independent oracles:
 *
 * - Given the checked-in manifest and maps, read the package, select
 *   `evergrove`, then load `western-falls`; reviewed file lengths/SHA-256 and
 *   literal IDs/paths are the oracle.
 * - Given unknown fields, duplicate IDs/paths, an unresolved entry/source/
 *   destination, an unsafe path, or a required cycle, decode; the closed schema
 *   contract and one-based source coordinate are the oracle.
 * - Given a valid manifest naming an absent destination, load while generation
 *   one is active; package-store `EntryMissing` and unchanged publication
 *   counters are the oracle.
 * - Given the valid destination, build generation two through the existing
 *   CAXEMAP pipeline and publish it; reviewed authored counts/digests and the
 *   generated-C ABI observer are the oracle.
 *
 * The probe intentionally makes no graphical transition, save, editor, ZIP,
 * or arbitrary-campaign claim. Those remain broader owners after this tracer.
 */
/** First broken invariant, or zero after every focused and vertical scenario. */
var observed:Int = 0;

/** Compact literal identity/version/count proof from the manifest. */
var traceManifest:Int = 0;

/** Generation selected by the real `ActiveContent` publication owner. */
var traceGeneration:Int = 0;

/** World-state trace of the independently parsed second CAXEMAP. */
var traceWorldState:Int = 0;

/** Scenario-identity trace of the independently parsed second CAXEMAP. */
var traceScenario:Int = 0;

/** Authored dialogue/objective/flow trace retained by `RuntimeLevelLoader`. */
var traceAuthored:Int = 0;

/** Run the Haxe-owned proof or expose its scalar envelope to native C. */
function main():Void {
	observed = selfCheck();
	#if !c
	Sys.println(observed);
	Sys.println(traceManifest);
	Sys.println(traceGeneration);
	Sys.println(traceWorldState);
	Sys.println(traceScenario);
	Sys.println(traceAuthored);
	#end
}

/** Return zero only when schema, package, level, and publication owners agree. */
function selfCheck():Int {
	final store = switch ContentPackageStore.open(".", "caxecraft-campaign", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 1;
	};
	final initialPlayer:RuntimeContentPlayerOptions = {
		entityId: EntityId.fromValidatedStorageCode(1),
		initialHealth: MAX_HEALTH
	};
	final initial = switch loadRuntimeContent(store, ContentGenerationId.fromSequence(1), initialPlayer) {
		case RuntimeContentReady(value): value;
		case RuntimeContentRejected(_): return 2;
	};
	final manifest = switch loadCampaignManifest(store, "campaigns/first-adventure/campaign.json") {
		case CampaignPackageReady(value): value;
		case CampaignPackageRejected(_): return 3;
	};
	final manifestText = switch store.read("campaigns/first-adventure/campaign.json") {
		case PackageBytesRead(value): value.bytes.toString();
		case PackageBytesRejected(_): return 4;
	};
	if (!verifySchemaRejections(manifestText))
		return 5;

	final entry = manifest.entryLevel();
	final forward = manifest.transitionAt(0);
	if (manifest.id.text() != "caxecraft:first-adventure"
		|| manifest.version != 1
		|| manifest.levelCount() != 2
		|| manifest.transitionCount() != 2
		|| manifest.entryLevelId().text() != "evergrove"
		|| entry.logicalPath != "scenarios/first-playable/map.caxemap"
		|| entry.byteLength != 14223
		|| forward.exit.text() != "evergrove-west"
		|| forward.sourceLevel.text() != "evergrove"
		|| forward.destinationLevel.text() != "western-falls"
		|| forward.destinationEntrance.text() != "default"
		|| !forward.required
		|| manifest.unambiguousTransitionFrom(entry.id) != forward)
		return 6;
	if (initial.receipt().map.logicalPath != entry.logicalPath || initial.level().generation().generationId().value() != 1)
		return 7;
	final player:LevelPlayerOptions = {
		entityId: initialPlayer.entityId,
		initialHealth: initialPlayer.initialHealth,
		aquaticProfile: initial.registry().defaultAquaticProfile()
	};

	final active = switch ActivePlayableLevel.create(initial.level()) {
		case PlayableLevelCreated(value): value;
		case PlayableLevelCreationRejected(_): return 114;
	};
	final initialView = active.level();
	if (initialView.logicalPath() != "scenarios/first-playable/map.caxemap"
		|| initialView.spawnTransform().xMilli != 16500
		|| initialView.loadedItemCount() != 1
		|| initialView.loadedItemAt(0).xMilli != 4500
		|| !initialView.dialogueActorId().isValid()
		|| !initialView.enemyActorId().isValid())
		return 115;
	final staleLengthText = replaceOnce(manifestText, '"byteLength": 5065', '"byteLength": 5064');
	final staleLengthManifest = switch decodeCampaignManifest(Bytes.ofString(staleLengthText)) {
		case CampaignManifestReady(value): value;
		case CampaignManifestRejected(_): return 8;
	};
	switch loadCampaignLevel(store, staleLengthManifest.levelAt(1), ContentGenerationId.fromSequence(2), initial.registry(), initial.registry(), player) {
		case CampaignLevelRejected(CampaignLevelLengthMismatch("scenarios/first-adventure/western-falls.caxemap", 5064, 5065)):
		case _:
			return 9;
	}
	final staleHashText = replaceOnce(manifestText, "ac54f3c91e79157839785cc647f58f9cd30f62ccfc300b836fd277a8e475da3f",
		"bc54f3c91e79157839785cc647f58f9cd30f62ccfc300b836fd277a8e475da3f");
	final staleHashManifest = switch decodeCampaignManifest(Bytes.ofString(staleHashText)) {
		case CampaignManifestReady(value): value;
		case CampaignManifestRejected(_): return 10;
	};
	switch loadCampaignLevel(store, staleHashManifest.levelAt(1), ContentGenerationId.fromSequence(2), initial.registry(), initial.registry(), player) {
		case CampaignLevelRejected(CampaignLevelHashMismatch("scenarios/first-adventure/western-falls.caxemap",
			"bc54f3c91e79157839785cc647f58f9cd30f62ccfc300b836fd277a8e475da3f")):
		case _:
			return 11;
	}
	final missingText = replaceOnce(manifestText, "scenarios/first-adventure/western-falls.caxemap", "scenarios/first-adventure/missing.caxemap");
	final missingManifest = switch decodeCampaignManifest(Bytes.ofString(missingText)) {
		case CampaignManifestReady(value): value;
		case CampaignManifestRejected(_): return 12;
	};
	switch loadCampaignLevel(store, missingManifest.levelAt(1), ContentGenerationId.fromSequence(2), initial.registry(), initial.registry(), player) {
		case CampaignLevelRejected(CampaignLevelSourceRejected("scenarios/first-adventure/missing.caxemap", EntryMissing)):
		case _:
			return 13;
	}
	if (active.generationId().value() != 1 || active.publicationCount() != 0)
		return 14;

	final destination = manifest.levelAt(1);
	final second = switch loadCampaignLevel(store, destination, ContentGenerationId.fromSequence(2), initial.registry(), initial.registry(), player) {
		case CampaignLevelReady(value): value;
		case CampaignLevelRejected(CampaignLevelRuntimeRejected(_, RuntimeLevelScenarioRejected(_))): return 111;
		case CampaignLevelRejected(CampaignLevelRuntimeRejected(_, RuntimeLevelPlanRejected(_))): return 112;
		case CampaignLevelRejected(CampaignLevelRuntimeRejected(_, RuntimeLevelGenerationRejected(_))): return 113;
		case CampaignLevelRejected(_): return 15;
	};
	final receipt = second.receipt();
	final authored = second.authoredTrace();
	if (destination.id.text() != "western-falls"
		|| destination.logicalPath != "scenarios/first-adventure/western-falls.caxemap"
		|| destination.byteLength != 5065
		|| destination.sha256 != "ac54f3c91e79157839785cc647f58f9cd30f62ccfc300b836fd277a8e475da3f"
		|| receipt.logicalPath != destination.logicalPath
		|| receipt.byteLength != destination.byteLength
		|| authored.dialogues != 1
		|| authored.objectives != 1
		|| authored.flowVariables != 0
		|| authored.flowSequences != 0
		|| authored.flowRules != 0)
		return 16;
	switch active.publish(second) {
		case PlayableLevelPublished(retired, selected) if (retired.value() == 1 && selected.value() == 2):
		case _:
			return 17;
	}
	final westernFalls = active.level();
	if (active.generationId().value() != 2
		|| active.publicationCount() != 1
		|| active.session() != second.generation().session()
		|| westernFalls.logicalPath() != destination.logicalPath
		|| westernFalls.spawnTransform().xMilli != 12500
		|| westernFalls.loadedItemCount() != 1
		|| westernFalls.loadedItemAt(0).xMilli != 6500
		|| !westernFalls.dialogueActorId().isValid()
		|| !westernFalls.enemyActorId().isValid())
		return 18;
	switch active.publish(initial.level()) {
		case PlayableLevelPublicationRejected(_):
		case _:
			return 116;
	}
	if (active.generationId().value() != 2 || active.publicationCount() != 1 || active.level().logicalPath() != destination.logicalPath)
		return 117;

	final semantic = active.semanticTrace();
	traceManifest = manifest.id.text().length * 10000 + manifest.version * 1000 + manifest.levelCount() * 100 + manifest.transitionCount() * 10
		+ (forward.required ? 1 : 0);
	traceGeneration = semantic.generationId;
	traceWorldState = semantic.worldState;
	traceScenario = semantic.scenarioDigest;
	traceAuthored = authored.dialogueDigest;
	traceAuthored = traceAuthored * 31 + authored.objectiveDigest;
	traceAuthored = traceAuthored * 31 + authored.flowDigest;
	return traceManifest == 251221 && traceGeneration == 2 && traceWorldState == -1715484850 && traceScenario == -1399453551
		&& traceAuthored == -1139403351 ? 0 : 19;
}

/** Challenge every campaign-only schema relationship with a reviewed mutation. */
function verifySchemaRejections(source:String):Bool {
	final unknown = replaceOnce(source, '"campaignVersion": 1,', '"campaignVersion": 1,\n  "unexpected": true,');
	if (!rejects(unknown, 1))
		return false;
	final duplicateId = replaceOnce(source, '"id": "western-falls"', '"id": "evergrove"');
	if (!rejects(duplicateId, 2))
		return false;
	final duplicatePath = replaceOnce(source, '"path": "scenarios/first-adventure/western-falls.caxemap"', '"path": "scenarios/first-playable/map.caxemap"');
	if (!rejects(duplicatePath, 3))
		return false;
	final missingEntry = replaceOnce(source, '"entryLevel": "evergrove"', '"entryLevel": "missing"');
	if (!rejects(missingEntry, 4))
		return false;
	final missingSource = replaceOnce(source, '"sourceLevel": "evergrove"', '"sourceLevel": "missing"');
	if (!rejects(missingSource, 5))
		return false;
	final missingDestination = replaceOnce(source, '"destinationLevel": "western-falls"', '"destinationLevel": "missing"');
	if (!rejects(missingDestination, 6))
		return false;
	final unsafePath = replaceOnce(source, '"path": "scenarios/first-adventure/western-falls.caxemap"', '"path": "../western-falls.caxemap"');
	if (!rejects(unsafePath, 7))
		return false;
	final requiredCycle = replaceOnce(source, '"required": false', '"required": true');
	return rejects(requiredCycle, 8);
}

/** Match one located closed diagnostic without accepting a neighboring error. */
function rejects(source:String, expected:Int):Bool {
	return switch decodeCampaignManifest(Bytes.ofString(source)) {
		case CampaignManifestReady(_): false;
		case CampaignManifestRejected(diagnostic):
			if (diagnostic.line < 1 || diagnostic.column < 1) {
				false;
			} else {
				switch [expected, diagnostic.kind] {
					case [1, SchemaUnknownField("campaign", "unexpected")]: true;
					case [2, SchemaDuplicateId("levels", "evergrove")]: true;
					case [3, SchemaDuplicateLogicalPath("scenarios/first-playable/map.caxemap")]: true;
					case [4, SchemaUnresolvedReference("entryLevel", "missing", "campaign level")]: true;
					case [
						5,
						SchemaUnresolvedReference("transitions[0].sourceLevel", "missing", "campaign level")
					]: true;
					case [
						6,
						SchemaUnresolvedReference("transitions[0].destinationLevel", "missing", "campaign level")
					]: true;
					case [7, SchemaInvalidLogicalPath("levels[1].path")]: true;
					case [8, SchemaRequiredTransitionCycle(_)]: true;
					case _: false;
				}
			}
	};
}

/** Replace exactly one reviewed fixture fragment; empty means fixture drift. */
function replaceOnce(source:String, needle:String, replacement:String):String {
	final at = source.indexOf(needle);
	if (at < 0 || source.indexOf(needle, at + needle.length) >= 0)
		return "";
	return source.substring(0, at) + replacement + source.substring(at + needle.length);
}

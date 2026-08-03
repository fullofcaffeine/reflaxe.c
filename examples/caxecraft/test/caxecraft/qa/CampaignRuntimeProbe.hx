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
import caxecraft.scenario.LocaleId;
import haxe.io.Bytes;

/**
 * Proves one real campaign transition from package bytes to publication.
 *
 * Scenario table and independent oracles:
 *
 * - Given the checked-in manifest and maps, read the package, select its entry,
 *   then follow its sole required transition; the manifest receipt and real
 *   package bytes are the oracle. Decorative coordinates and authored counts
 *   deliberately remain free to change.
 * - Given a manually authored minimal campaign document, mutate unknown fields,
 *   duplicate IDs/paths, references, paths, and cycles; the closed schema
 *   contract and one-based source coordinate are the oracle.
 * - Given a valid manifest naming an absent destination, load while generation
 *   one is active; package-store `EntryMissing` and unchanged publication
 *   counters are the oracle.
 * - Given the valid destination, build generation two through the existing
 *   CAXEMAP pipeline and publish it; publication invariants and the generated-C
 *   ABI observer are the oracle. Dynamic digests prove Eval/native agreement,
 *   not independent truth about one campaign's decoration.
 *
 * The probe intentionally makes no graphical transition, save, editor, ZIP,
 * or arbitrary-campaign claim. Those remain broader owners after this tracer.
 */
/** First broken invariant, or zero after every focused and vertical scenario. */
var observed:Int = 0;

/** Dynamic manifest envelope used only for Eval/native agreement. */
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
	if (!verifySchemaRejections(schemaFixture()))
		return 5;

	final entry = manifest.entryLevel();
	final forward = manifest.unambiguousTransitionFrom(entry.id);
	if (manifest.id.text() != "caxecraft:first-adventure" || forward == null || !forward.required)
		return 6;
	final destination = manifest.level(forward.destinationLevel);
	if (destination == null)
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
	if (initialView.logicalPath() != entry.logicalPath)
		return 115;
	final english = new LocaleId("en");
	final spanish = new LocaleId("es-mx");
	final initialPresentation = initial.level().presentation();
	if (initialView.presentation() != initialPresentation
		|| initialView.scenarioTitle(english) != initialPresentation.scenarioTitle(english)
		|| initialView.scenarioTitle(spanish) != initialPresentation.scenarioTitle(spanish)
		|| initialView.initialObjectiveTitle(english) != initialPresentation.initialObjectiveTitle(english)
		|| initialView.initialObjectiveTitle(spanish) != initialPresentation.initialObjectiveTitle(spanish)
		|| initialView.initialObjectiveBody(english) != initialPresentation.initialObjectiveBody(english)
		|| initialView.initialObjectiveBody(spanish) != initialPresentation.initialObjectiveBody(spanish))
		return 118;
	final staleLength = destination.byteLength == 1 ? 2 : destination.byteLength - 1;
	final destinationIdentity = '"id": "${destination.id.text()}"';
	final staleLengthText = replaceObjectField(manifestText, destinationIdentity, '"byteLength": ${destination.byteLength}', '"byteLength": $staleLength');
	final staleLengthManifest = switch decodeCampaignManifest(Bytes.ofString(staleLengthText)) {
		case CampaignManifestReady(value): value;
		case CampaignManifestRejected(_): return 8;
	};
	final staleLengthDestination = staleLengthManifest.level(forward.destinationLevel);
	if (staleLengthDestination == null)
		return 8;
	switch loadCampaignLevel(store, staleLengthDestination, ContentGenerationId.fromSequence(2), initial.registry(), initial.registry(), player) {
		case CampaignLevelRejected(CampaignLevelLengthMismatch(path, expected, actual))
			if (path == destination.logicalPath && expected == staleLength && actual == destination.byteLength):
		case _:
			return 9;
	}
	final staleHash = (destination.sha256.charAt(0) == "0" ? "1" : "0") + destination.sha256.substring(1);
	final staleHashText = replaceObjectField(manifestText, destinationIdentity, '"sha256": "${destination.sha256}"', '"sha256": "$staleHash"');
	final staleHashManifest = switch decodeCampaignManifest(Bytes.ofString(staleHashText)) {
		case CampaignManifestReady(value): value;
		case CampaignManifestRejected(_): return 10;
	};
	final staleHashDestination = staleHashManifest.level(forward.destinationLevel);
	if (staleHashDestination == null)
		return 10;
	switch loadCampaignLevel(store, staleHashDestination, ContentGenerationId.fromSequence(2), initial.registry(), initial.registry(), player) {
		case CampaignLevelRejected(CampaignLevelHashMismatch(path, expected)) if (path == destination.logicalPath && expected == staleHash):
		case _:
			return 11;
	}
	final missingPath = "scenarios/caxecraft-probe/missing.caxemap";
	final missingText = replaceObjectField(manifestText, destinationIdentity, '"path": "${destination.logicalPath}"', '"path": "$missingPath"');
	final missingManifest = switch decodeCampaignManifest(Bytes.ofString(missingText)) {
		case CampaignManifestReady(value): value;
		case CampaignManifestRejected(_): return 12;
	};
	final missingDestination = missingManifest.level(forward.destinationLevel);
	if (missingDestination == null)
		return 12;
	switch loadCampaignLevel(store, missingDestination, ContentGenerationId.fromSequence(2), initial.registry(), initial.registry(), player) {
		case CampaignLevelRejected(CampaignLevelSourceRejected(path, EntryMissing)) if (path == missingPath):
		case _:
			return 13;
	}
	if (active.generationId().value() != 1 || active.publicationCount() != 0)
		return 14;

	final second = switch loadCampaignLevel(store, destination, ContentGenerationId.fromSequence(2), initial.registry(), initial.registry(), player) {
		case CampaignLevelReady(value): value;
		case CampaignLevelRejected(CampaignLevelRuntimeRejected(_, RuntimeLevelScenarioRejected(_))): return 111;
		case CampaignLevelRejected(CampaignLevelRuntimeRejected(_, RuntimeLevelPlanRejected(_))): return 112;
		case CampaignLevelRejected(CampaignLevelRuntimeRejected(_, RuntimeLevelGenerationRejected(_))): return 113;
		case CampaignLevelRejected(_): return 15;
	};
	final receipt = second.receipt();
	final authored = second.authoredTrace();
	if (receipt.logicalPath != destination.logicalPath || receipt.byteLength != destination.byteLength)
		return 16;
	switch active.publish(second) {
		case PlayableLevelPublished(retired, selected) if (retired.value() == 1 && selected.value() == 2):
		case _:
			return 17;
	}
	final destinationView = active.level();
	if (active.generationId().value() != 2
		|| active.publicationCount() != 1
		|| active.session() != second.generation().session()
		|| destinationView.logicalPath() != destination.logicalPath)
		return 18;
	final destinationPresentation = second.presentation();
	if (destinationView.presentation() != destinationPresentation
		|| destinationView.scenarioTitle(english) != destinationPresentation.scenarioTitle(english)
		|| destinationView.scenarioTitle(spanish) != destinationPresentation.scenarioTitle(spanish)
		|| destinationView.initialObjectiveTitle(english) != destinationPresentation.initialObjectiveTitle(english)
		|| destinationView.initialObjectiveTitle(spanish) != destinationPresentation.initialObjectiveTitle(spanish)
		|| destinationView.initialObjectiveBody(english) != destinationPresentation.initialObjectiveBody(english)
		|| destinationView.initialObjectiveBody(spanish) != destinationPresentation.initialObjectiveBody(spanish))
		return 119;
	switch active.publish(initial.level()) {
		case PlayableLevelPublicationRejected(_):
		case _:
			return 116;
	}
	if (active.generationId().value() != 2
		|| active.publicationCount() != 1
		|| active.level().logicalPath() != destination.logicalPath
		|| active.level().presentation() != destinationPresentation
		|| active.level().scenarioTitle(english) != destinationPresentation.scenarioTitle(english)
		|| active.level().initialObjectiveTitle(english) != destinationPresentation.initialObjectiveTitle(english))
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
	return traceGeneration == 2 ? 0 : 19;
}

/** Provide a tiny independent schema oracle unrelated to shipped campaign art. */
function schemaFixture():String
	return '{"schemaVersion":1,"campaignId":"caxecraft:schema-probe","campaignVersion":1,"entryLevel":"a",'
		+ '"levels":[{"id":"a","path":"scenarios/a.caxemap","byteLength":1,"sha256":"0000000000000000000000000000000000000000000000000000000000000000"},'
		+ '{"id":"b","path":"scenarios/b.caxemap","byteLength":1,"sha256":"1111111111111111111111111111111111111111111111111111111111111111"}],'
		+ '"transitions":[{"exit":"a-b","sourceLevel":"a","destinationLevel":"b","destinationEntrance":"default","required":true},'
		+ '{"exit":"b-a","sourceLevel":"b","destinationLevel":"a","destinationEntrance":"default","required":false}]}';

/** Challenge every campaign-only schema relationship with a reviewed mutation. */
function verifySchemaRejections(source:String):Bool {
	final unknown = replaceOnce(source, '"campaignVersion":1,', '"campaignVersion":1,"unexpected":true,');
	if (!rejects(unknown, 1))
		return false;
	final duplicateId = replaceOnce(source, '"id":"b"', '"id":"a"');
	if (!rejects(duplicateId, 2))
		return false;
	final duplicatePath = replaceOnce(source, '"path":"scenarios/b.caxemap"', '"path":"scenarios/a.caxemap"');
	if (!rejects(duplicatePath, 3))
		return false;
	final missingEntry = replaceOnce(source, '"entryLevel":"a"', '"entryLevel":"missing"');
	if (!rejects(missingEntry, 4))
		return false;
	final missingSource = replaceOnce(source, '"sourceLevel":"a"', '"sourceLevel":"missing"');
	if (!rejects(missingSource, 5))
		return false;
	final missingDestination = replaceOnce(source, '"destinationLevel":"b"', '"destinationLevel":"missing"');
	if (!rejects(missingDestination, 6))
		return false;
	final unsafePath = replaceOnce(source, '"path":"scenarios/b.caxemap"', '"path":"../b.caxemap"');
	if (!rejects(unsafePath, 7))
		return false;
	final requiredCycle = replaceOnce(source, '"required":false', '"required":true');
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
					case [2, SchemaDuplicateId("levels", "a")]: true;
					case [3, SchemaDuplicateLogicalPath("scenarios/a.caxemap")]: true;
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

/** Replace one field inside the uniquely identified JSON object under test. */
function replaceObjectField(source:String, identity:String, field:String, replacement:String):String {
	final identityAt = source.indexOf(identity);
	if (identityAt < 0 || source.indexOf(identity, identityAt + identity.length) >= 0)
		return "";
	final objectEnd = source.indexOf("}", identityAt + identity.length);
	final fieldAt = source.indexOf(field, identityAt + identity.length);
	final repeatedFieldAt = source.indexOf(field, fieldAt + field.length);
	if (objectEnd < 0 || fieldAt < 0 || fieldAt >= objectEnd || (repeatedFieldAt >= 0 && repeatedFieldAt < objectEnd))
		return "";
	return source.substring(0, fieldAt) + replacement + source.substring(fieldAt + field.length);
}

/** Replace exactly one reviewed fixture fragment; empty means fixture drift. */
function replaceOnce(source:String, needle:String, replacement:String):String {
	final at = source.indexOf(needle);
	if (at < 0 || source.indexOf(needle, at + needle.length) >= 0)
		return "";
	return source.substring(0, at) + replacement + source.substring(at + needle.length);
}

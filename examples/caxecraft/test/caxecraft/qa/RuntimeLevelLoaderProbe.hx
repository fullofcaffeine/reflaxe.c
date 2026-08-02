package caxecraft.qa;

import caxecraft.content.ActiveContent;
import caxecraft.content.ActiveContent.ContentPublicationResult;
import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildError;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildFault;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.LoadedContentGeneration.LoadedContentGenerationTrace;
import caxecraft.content.ResolvedLevelPlan.LevelPlayerOptions;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelAuthority;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelAuthoredTrace;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelLoadResult;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelSource;
import caxecraft.content.RuntimeLevelLoader.loadRuntimeLevel;
import caxecraft.content.RuntimeLevelLoader.loadRuntimeLevelWithFault;
import caxecraft.domain.ActorControllerProfile;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.EntityId;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.domain.World;
import caxecraft.qa.ScenarioNativeCodecProbe.firstPlayableBytes;
import caxecraft.qa.FocusedContentFixture.FocusedContentRegistry;
import caxecraft.qa.FocusedContentFixture.standardAquaticProfile;
import haxe.io.Bytes;

/**
 * Proves that staged CAXEMAP bytes can become the native playable generation.
 *
 * The probe first loads the checked-in bytes from an embedded QA fixture, then
 * reads the real file through `ContentPackageStore`. Both requests must produce
 * the same story, construction, actor-mechanics, and session evidence before
 * the package candidate is published. Malformed input, missing input, and a
 * late injected construction failure must leave the active generation
 * unchanged.
 */
/** Complete Haxe-authored assertion result observed by the native harness. */
var observed:Int = 0;

/** Diagnostic fingerprint of the exact authored bytes read from the package. */
var traceInputHash:Int = 0;

/** Exact byte length reported by the native package receipt. */
var traceByteLength:Int = 0;

/** Final native generation identity after one successful publication. */
var traceGenerationId:Int = 0;

/** Deterministic state of the session built from the staged map bytes. */
var traceWorldState:Int = 0;

/** Folded dialogue, objective, and CaxeFlow source evidence. */
var traceAuthored:Int = 0;

/** Folded Nia and Mossling mechanics loaded through the compiled registry. */
var traceActorMechanics:Int = 0;

/** Explicit byte source selected for the published generation. */
var traceAuthority:Int = 0;

/** Run the complete source-to-publication specification on Eval and native C. */
function main():Void {
	final status = selfCheck();
	#if c
	observed = status;
	#else
	Sys.println(status);
	Sys.println(traceInputHash);
	Sys.println(traceByteLength);
	Sys.println(traceGenerationId);
	Sys.println(traceWorldState);
	Sys.println(traceAuthored);
	Sys.println(traceActorMechanics);
	Sys.println(traceAuthority);
	#end
}

/** Return zero, or the stable number of the first broken loading invariant. */
function selfCheck():Int {
	final registry = new FocusedContentRegistry();
	final options:LevelPlayerOptions = {
		entityId: EntityId.fromValidatedStorageCode(1),
		initialHealth: MAX_HEALTH,
		aquaticProfile: standardAquaticProfile()
	};
	final logicalPath = "scenarios/first-playable/map.caxemap";
	final embedded = switch loadRuntimeLevel(InMemoryBytes(firstPlayableBytes(), "embedded-first-playable", logicalPath), ContentGenerationId.fromSequence(1),
		registry, registry, options) {
		case RuntimeLevelReady(candidate): candidate;
		case RuntimeLevelRejected(_): return 1;
	};
	final embeddedReceipt = embedded.receipt();
	if (embeddedReceipt.authority != InMemorySource
		|| embeddedReceipt.logicalPath != logicalPath
		|| embeddedReceipt.rootLabel != "embedded-first-playable"
		|| embeddedReceipt.byteLength != 5098
		|| embeddedReceipt.readAttempts != 1)
		return 2;
	final embeddedFacts = embedded.authoredTrace();
	if (!expectedAuthoredFacts(embeddedFacts))
		return 3;
	if (!expectedResolvedLevel(embedded))
		return 4;

	final active = new ActiveContent(embedded.generation());
	final embeddedTrace = active.semanticTrace();
	final store = switch ContentPackageStore.open(".", "caxecraft-source", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 5;
	};
	final nativeCandidate = switch loadRuntimeLevel(NativePackageFile(store, logicalPath), ContentGenerationId.fromSequence(2), registry, registry, options) {
		case RuntimeLevelReady(candidate): candidate;
		case RuntimeLevelRejected(_): return 6;
	};
	final nativeReceipt = nativeCandidate.receipt();
	if (nativeReceipt.authority != NativePackage
		|| nativeReceipt.logicalPath != logicalPath
		|| nativeReceipt.rootLabel != "caxecraft-source"
		|| nativeReceipt.inputHash != embeddedReceipt.inputHash
		|| nativeReceipt.byteLength != embeddedReceipt.byteLength
		|| nativeReceipt.readAttempts != 1)
		return 7;
	if (!sameAuthoredTrace(embeddedFacts, nativeCandidate.authoredTrace())
		|| !sameGenerationSemantics(embeddedTrace, nativeCandidate.generation().semanticTrace())
		|| !expectedResolvedLevel(nativeCandidate))
		return 8;

	final beforeFailures = active.semanticTrace();
	switch loadRuntimeLevel(NativePackageFile(store, "scenarios/first-playable/missing.caxemap"), ContentGenerationId.fromSequence(2), registry, registry,
		options) {
		case RuntimeLevelRejected(RuntimeLevelSourceRejected(EntryMissing)):
		case _:
			return 9;
	}
	if (!sameGenerationTrace(beforeFailures, active.semanticTrace()))
		return 10;

	switch loadRuntimeLevel(InMemoryBytes(Bytes.ofString("CAXEMAP 1\nend-map\n"), "malformed-fixture", logicalPath), ContentGenerationId.fromSequence(2),
		registry, registry, options) {
		case RuntimeLevelRejected(RuntimeLevelScenarioRejected(diagnostics)) if (diagnostics.length > 0):
		case _:
			return 11;
	}
	if (!sameGenerationTrace(beforeFailures, active.semanticTrace()))
		return 12;

	switch loadRuntimeLevelWithFault(NativePackageFile(store, logicalPath), ContentGenerationId.fromSequence(2), registry, registry, options,
		FailBeforeActors) {
		case RuntimeLevelRejected(RuntimeLevelGenerationRejected(InjectedFailure(ActorConstruction))):
		case _:
			return 13;
	}
	if (!sameGenerationTrace(beforeFailures, active.semanticTrace()))
		return 14;

	switch active.publish(nativeCandidate.generation()) {
		case ContentPublished(retired, selected) if (retired.value() == 1 && selected.value() == 2):
		case _:
			return 15;
	}
	final finalTrace = active.semanticTrace();
	if (active.generationId().value() != 2 || active.publicationCount() != 1 || !sameGenerationSemantics(embeddedTrace, finalTrace))
		return 16;

	traceInputHash = nativeReceipt.inputHash;
	traceByteLength = nativeReceipt.byteLength;
	traceGenerationId = active.generationId().value();
	traceWorldState = finalTrace.worldState;
	traceAuthored = digestAuthored(nativeCandidate.authoredTrace());
	traceActorMechanics = digestActorMechanics(nativeCandidate);
	traceAuthority = authorityCode(nativeReceipt.authority);
	return traceByteLength == 5098 && traceAuthority == 2 ? 0 : 17;
}

/**
 * Verify construction and authored identities that must originate in CAXEMAP.
 *
 * Mechanics values are checked beside those IDs because the selected compiled
 * registry—not the map—owns health, controller radii, and attack timings.
 */
function expectedResolvedLevel(candidate:caxecraft.content.RuntimeLevelLoader.RuntimeLevelCandidate):Bool {
	final plan = candidate.generation().plan();
	final terrain = plan.terrainRuns();
	final fluids = plan.fluids();
	final items = plan.items();
	final actors = plan.actors();
	final player = plan.player();
	if (terrain.length == 0
		|| terrain[0].storage.value() != 4
		|| terrain[0].count != 32
		|| fluids.length != 2
		|| fluids[0].authoredId.text() != "water.pool"
		|| fluids[1].authoredId.text() != "water.spring"
		|| items.length != 1
		|| items[0].authoredId.text() != "item.tideweave"
		|| items[0].contentId.text() != "caxecraft:tideweave-suit"
		|| items[0].storage.value() != 8
		|| actors.length != 2
		|| actors[0].authoredId.text() != "enemy.mossling"
		|| actors[0].contentId.text() != "caxecraft:mossling"
		|| actors[0].maximumHealth != 3
		|| actors[1].authoredId.text() != "guide.nia"
		|| actors[1].contentId.text() != "caxecraft:nia"
		|| actors[1].maximumHealth != 6
		|| player.authoredId.text() != "player.start"
		|| player.transform.xMilli != 16500
		|| player.transform.yMilli != 5000
		|| player.transform.zMilli != 16500
		|| plan.flowBindings().length != 0)
		return false;
	final mossling = switch actors[0].controller {
		case WanderChaseMelee(profile):
			profile.noticeRadiusMilli == 6000
			&& profile.strikeRadiusMilli == 3000
			&& profile.attackRadiusMilli == 1400
			&& profile.windupTicks == 8
			&& profile.recoveryTicks == 12
			&& profile.stepMilli == 80
			&& profile.drop.text() == "caxecraft:mossling-berries";
		case _:
			false;
	};
	final nia = switch actors[1].controller {
		case StationaryDialogue(radius): radius == 3500;
		case _:
			false;
	};
	return mossling && nia;
}

/** Verify the first map's current dialogue, objective, and empty flow surface. */
function expectedAuthoredFacts(trace:RuntimeLevelAuthoredTrace):Bool
	return trace.dialogues == 1
		&& trace.objectives == 1
		&& trace.flowVariables == 0
		&& trace.flowSequences == 0
		&& trace.flowRules == 0;

/** Compare source facts produced by the generated and native byte authorities. */
function sameAuthoredTrace(left:RuntimeLevelAuthoredTrace, right:RuntimeLevelAuthoredTrace):Bool
	return left.dialogues == right.dialogues
		&& left.dialogueDigest == right.dialogueDigest
		&& left.objectives == right.objectives
		&& left.objectiveDigest == right.objectiveDigest
		&& left.flowVariables == right.flowVariables
		&& left.flowSequences == right.flowSequences
		&& left.flowRules == right.flowRules
		&& left.flowDigest == right.flowDigest;

/** Compare complete session semantics while allowing different generation IDs. */
function sameGenerationSemantics(left:LoadedContentGenerationTrace, right:LoadedContentGenerationTrace):Bool
	return left.scenarioDigest == right.scenarioDigest
		&& left.resolvedDigest == right.resolvedDigest
		&& left.presentationDigest == right.presentationDigest
		&& left.worldState == right.worldState
		&& left.pendingWater == right.pendingWater
		&& left.activeItems == right.activeItems
		&& left.activeItemDigest == right.activeItemDigest
		&& left.characters == right.characters
		&& left.controllers == right.controllers
		&& left.localPlayerId == right.localPlayerId;

/** Compare every active-generation field when a rejected request must be inert. */
function sameGenerationTrace(left:LoadedContentGenerationTrace, right:LoadedContentGenerationTrace):Bool
	return left.generationId == right.generationId && sameGenerationSemantics(left, right);

/** Fold the authored trace into one compact cross-target output value. */
function digestAuthored(trace:RuntimeLevelAuthoredTrace):Int {
	var digest = CaxecraftTrace.mix(trace.dialogues, trace.dialogueDigest);
	digest = CaxecraftTrace.mix(digest, trace.objectives);
	digest = CaxecraftTrace.mix(digest, trace.objectiveDigest);
	digest = CaxecraftTrace.mix(digest, trace.flowVariables);
	digest = CaxecraftTrace.mix(digest, trace.flowSequences);
	digest = CaxecraftTrace.mix(digest, trace.flowRules);
	return CaxecraftTrace.mix(digest, trace.flowDigest);
}

/** Fold the exact compiled-registry actor mechanics paired with loaded IDs. */
function digestActorMechanics(candidate:caxecraft.content.RuntimeLevelLoader.RuntimeLevelCandidate):Int {
	var digest = 0;
	for (actor in candidate.generation().plan().actors()) {
		digest = CaxecraftTrace.mix(digest, actor.maximumHealth);
		digest = switch actor.controller {
			case StationaryDialogue(radius):
				CaxecraftTrace.mix(digest, radius);
			case WanderChaseMelee(profile):
				var controller = CaxecraftTrace.mix(digest, profile.noticeRadiusMilli);
				controller = CaxecraftTrace.mix(controller, profile.strikeRadiusMilli);
				controller = CaxecraftTrace.mix(controller, profile.attackRadiusMilli);
				controller = CaxecraftTrace.mix(controller, profile.windupTicks);
				controller = CaxecraftTrace.mix(controller, profile.recoveryTicks);
				CaxecraftTrace.mix(controller, profile.stepMilli);
		};
	}
	return digest;
}

/** Convert the explicit byte source into a stable output scalar. */
function authorityCode(authority:RuntimeLevelAuthority):Int
	return switch authority {
		case InMemorySource: 1;
		case NativePackage: 2;
	};

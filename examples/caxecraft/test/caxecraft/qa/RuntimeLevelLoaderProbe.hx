package caxecraft.qa;

import caxecraft.content.ActiveContent;
import caxecraft.content.ActiveContent.ContentPublicationResult;
import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
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
import caxecraft.domain.Aquatics.input as aquaticInput;
import caxecraft.domain.BlockKind;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.CharacterDamagePolicy;
import caxecraft.domain.EntityId;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.domain.World;
import caxecraft.domain.WorldRead.query as queryWorld;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.ItemKind;
import caxecraft.qa.FocusedContentFixture.FocusedContentRegistry;
import caxecraft.qa.FocusedContentFixture.standardAquaticProfile;
import caxecraft.qa.FocusedContentFixture.tideweaveAquaticProfile;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.CaxeFlowRuntime.FlowPresentationEvent;
import haxe.io.Bytes;

/**
 * Proves that staged CAXEMAP bytes can become the native playable generation.
 *
 * The probe reads the checked-in map once through `ContentPackageStore`, then
 * sends those exact bytes through both the in-memory and native-file loading
 * authorities. Both requests must produce the same story, construction,
 * actor-mechanics, landmark, and session evidence before publication. This
 * keeps the product map runtime-authored: changing map data does not require a
 * matching Haxe byte-array fixture or a product rebuild. The smaller codec
 * probe separately retains a manually reviewed parser fixture.
 *
 * Malformed input, missing input, and a late injected construction failure
 * must leave the active generation unchanged.
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
	final store = switch ContentPackageStore.open(".", "caxecraft-source", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 1;
	};
	final presentationPath = "test/fixtures/caxemap/runtime-presentation.caxemap";
	final presentationCandidate = switch loadRuntimeLevel(NativePackageFile(store, presentationPath), ContentGenerationId.fromSequence(99), registry,
		registry, options) {
		case RuntimeLevelReady(candidate): candidate;
		case RuntimeLevelRejected(RuntimeLevelSourceRejected(_)): return 101;
		case RuntimeLevelRejected(RuntimeLevelScenarioRejected(_)): return 103;
		case RuntimeLevelRejected(RuntimeLevelPlanRejected(_)): return 104;
		case RuntimeLevelRejected(RuntimeLevelGenerationRejected(_)): return 105;
	};
	final presentation = presentationCandidate.presentation();
	final english = new LocaleId("en");
	final spanish = new LocaleId("es-mx");
	final fallback = new LocaleId("fr");
	if (presentation.scenarioTitle(english) != "Runtime presentation probe"
		|| presentation.scenarioTitle(spanish) != "Prueba de presentacion en ejecucion"
		|| presentation.scenarioTitle(fallback) != "Runtime presentation probe"
		|| presentation.message(new MessageId("objective.title"), english) != "Reach the test marker"
		|| presentation.message(new MessageId("objective.title"), spanish) != "Llega al marcador de prueba"
		|| presentation.message(new MessageId("objective.title"), fallback) != "Reach the test marker"
		|| presentation.message(new MessageId("missing.message"), english) != ""
		|| presentation.adventureTagline(english) != "A runtime-authored adventure"
		|| presentation.adventureTagline(spanish) != "Una aventura creada en tiempo de ejecucion"
		|| presentation.adventureTagline(fallback) != "A runtime-authored adventure"
		|| presentation.initialObjectiveTitle(english) != "Reach the test marker"
		|| presentation.initialObjectiveTitle(spanish) != "Llega al marcador de prueba"
		|| presentation.initialObjectiveBody(english) != "Cross the test bridge."
		|| presentation.initialObjectiveBody(spanish) != "Cruza el puente de prueba."
		|| presentation.dialogueLine(new ScenarioId("dialogue.guide"), 0, english) != "Synthetic engine fixture"
		|| presentation.dialogueLine(new ScenarioId("dialogue.missing"), 0, english) != ""
		|| presentation.dialogueLine(new ScenarioId("dialogue.guide"), 1, english) != "")
		return 102;
	final flowSession = presentationCandidate.generation().session();
	final flowActors = presentationCandidate.generation().actorBindings();
	final initialFlowTick = flowSession.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (!initialFlowTick.committed
		|| initialFlowTick.flow == null
		|| initialFlowTick.flow.diagnostics.length != 0
		|| initialFlowTick.flow.activeObjective == null
		|| initialFlowTick.flow.activeObjective.text() != "objective.marker")
		return 111;
	if (flowActors.length != 1 || !flowSession.interactWithActor(flowActors[0].entityId))
		return 106;
	final flowTick = flowSession.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (!flowTick.committed || flowTick.flow == null)
		return 107;
	final flow = flowTick.flow;
	if (flow.firedRules.length != 1
		|| flow.firedRules[0].text() != "rule.fixture-advance"
		|| flow.presentation.length != 2
		|| flow.diagnostics.length != 0
		|| flow.activeObjective == null
		|| flow.activeObjective.text() != "objective.next")
		return 108;
	switch flow.presentation[0] {
		case FlowPresentationEvent.ObjectiveChanged(id, Complete) if (id.text() == "objective.marker"):
		case _:
			return 109;
	}
	switch flow.presentation[1] {
		case FlowPresentationEvent.ObjectiveChanged(id, Active) if (id.text() == "objective.next"):
		case _:
			return 110;
	}
	final collected = flowSession.collectAuthoredInventoryItem(0, Inventory.make(0, 0, 0, 0, 0, 0, 0, 0, 0), ItemKind.Lantern, 1);
	if (!collected.resolved || collected.collected != 1 || collected.inventory.lantern != 1)
		return 112;
	final collectionTick = flowSession.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (!collectionTick.committed
		|| collectionTick.flow == null
		|| collectionTick.flow.firedRules.length != 1
		|| collectionTick.flow.firedRules[0].text() != "rule.fixture-collect"
		|| collectionTick.flow.activeObjective == null
		|| collectionTick.flow.activeObjective.text() != "objective.collected")
		return 113;
	if (!flowSession.activateAuthoredItemDuringLoad(0))
		return 114;
	final fullInventory = Inventory.make(0, 0, 0, 0, 0, 0, 0, 0, Inventory.MAX_STACK);
	final rejectedCollection = flowSession.collectAuthoredInventoryItem(0, fullInventory, ItemKind.Lantern, 1);
	if (!rejectedCollection.resolved || rejectedCollection.collected != 0 || !flowSession.authoredItemIsActive(0))
		return 115;
	final rejectedCollectionTick = flowSession.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (!rejectedCollectionTick.committed || rejectedCollectionTick.flow == null || rejectedCollectionTick.flow.firedRules.length != 0)
		return 116;
	final equipped = flowSession.collectAuthoredAquaticEquipment(1, tideweaveAquaticProfile());
	if (!equipped.resolved || !equipped.collected || equipped.character.aquaticProfile.maximumBreathTicks != 1200)
		return 117;
	final equipmentTick = flowSession.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (!equipmentTick.committed
		|| equipmentTick.flow == null
		|| equipmentTick.flow.firedRules.length != 1
		|| equipmentTick.flow.firedRules[0].text() != "rule.fixture-equip"
		|| equipmentTick.flow.activeObjective == null
		|| equipmentTick.flow.activeObjective.text() != "objective.equipped")
		return 118;
	final logicalPath = "scenarios/first-playable/map.caxemap";
	final checkedIn = switch store.read(logicalPath) {
		case PackageBytesRead(value): value;
		case PackageBytesRejected(_): return 2;
	};
	final embedded = switch loadRuntimeLevel(InMemoryBytes(checkedIn.bytes, "checked-in-first-playable", logicalPath), ContentGenerationId.fromSequence(1),
		registry, registry, options) {
		case RuntimeLevelReady(candidate): candidate;
		case RuntimeLevelRejected(_): return 3;
	};
	final embeddedReceipt = embedded.receipt();
	if (embeddedReceipt.authority != InMemorySource
		|| embeddedReceipt.logicalPath != logicalPath
		|| embeddedReceipt.rootLabel != "checked-in-first-playable"
		|| embeddedReceipt.byteLength <= 0
		|| embeddedReceipt.byteLength != checkedIn.bytes.length
		|| embeddedReceipt.readAttempts != 1)
		return 4;
	final embeddedFacts = embedded.authoredTrace();
	if (!expectedResolvedLevel(embedded))
		return 6;

	final active = new ActiveContent(embedded.generation());
	final embeddedTrace = active.semanticTrace();
	final nativeCandidate = switch loadRuntimeLevel(NativePackageFile(store, logicalPath), ContentGenerationId.fromSequence(2), registry, registry, options) {
		case RuntimeLevelReady(candidate): candidate;
		case RuntimeLevelRejected(_): return 7;
	};
	final nativeReceipt = nativeCandidate.receipt();
	if (nativeReceipt.authority != NativePackage
		|| nativeReceipt.logicalPath != logicalPath
		|| nativeReceipt.rootLabel != "caxecraft-source"
		|| nativeReceipt.inputHash != embeddedReceipt.inputHash
		|| nativeReceipt.byteLength != embeddedReceipt.byteLength
		|| nativeReceipt.readAttempts != 1)
		return 8;
	if (!sameAuthoredTrace(embeddedFacts, nativeCandidate.authoredTrace())
		|| !sameGenerationSemantics(embeddedTrace, nativeCandidate.generation().semanticTrace())
		|| !expectedResolvedLevel(nativeCandidate))
		return 9;

	final beforeFailures = active.semanticTrace();
	switch loadRuntimeLevel(NativePackageFile(store, "scenarios/first-playable/missing.caxemap"), ContentGenerationId.fromSequence(2), registry, registry,
		options) {
		case RuntimeLevelRejected(RuntimeLevelSourceRejected(EntryMissing)):
		case _:
			return 10;
	}
	if (!sameGenerationTrace(beforeFailures, active.semanticTrace()))
		return 11;

	switch loadRuntimeLevel(InMemoryBytes(Bytes.ofString("CAXEMAP 1\nend-map\n"), "malformed-fixture", logicalPath), ContentGenerationId.fromSequence(2),
		registry, registry, options) {
		case RuntimeLevelRejected(RuntimeLevelScenarioRejected(diagnostics)) if (diagnostics.length > 0):
		case _:
			return 12;
	}
	if (!sameGenerationTrace(beforeFailures, active.semanticTrace()))
		return 13;

	switch loadRuntimeLevelWithFault(NativePackageFile(store, logicalPath), ContentGenerationId.fromSequence(2), registry, registry, options,
		FailBeforeActors) {
		case RuntimeLevelRejected(RuntimeLevelGenerationRejected(InjectedFailure(ActorConstruction))):
		case _:
			return 14;
	}
	if (!sameGenerationTrace(beforeFailures, active.semanticTrace()))
		return 15;

	switch active.publish(nativeCandidate.generation()) {
		case ContentPublished(retired, selected) if (retired.value() == 1 && selected.value() == 2):
		case _:
			return 16;
	}
	final finalTrace = active.semanticTrace();
	if (active.generationId().value() != 2 || active.publicationCount() != 1 || !sameGenerationSemantics(embeddedTrace, finalTrace))
		return 17;

	traceInputHash = nativeReceipt.inputHash;
	traceByteLength = nativeReceipt.byteLength;
	traceGenerationId = active.generationId().value();
	traceWorldState = finalTrace.worldState;
	traceAuthored = digestAuthored(nativeCandidate.authoredTrace());
	traceActorMechanics = digestActorMechanics(nativeCandidate);
	traceAuthority = authorityCode(nativeReceipt.authority);
	return traceByteLength == checkedIn.bytes.length && traceAuthority == 2 ? 0 : 18;
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
		|| fluids.length != 5
		|| fluids[0].authoredId.text() != "water.evergrove-stream-north"
		|| fluids[1].authoredId.text() != "water.evergrove-stream-south"
		|| fluids[2].authoredId.text() != "water.evergrove-stream-under-bridge"
		|| fluids[3].authoredId.text() != "water.village-well"
		|| fluids[4].authoredId.text() != "water.evergrove-stream-source"
		|| items.length != 2
		|| items[0].authoredId.text() != "item.nia-berries"
		|| items[0].contentId.text() != "caxecraft:berries"
		|| items[0].quantity != 2
		|| items[1].authoredId.text() != "item.tideweave"
		|| items[1].contentId.text() != "caxecraft:tideweave-suit"
		|| items[1].storage.value() != 8
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
		|| player.transform.zMilli != 16500)
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
	return mossling && nia && expectedEvergroveLandmarks(candidate);
}

/**
 * Check a small independent coordinate oracle for the village's readable shape.
 *
 * These expectations were selected by hand from the content brief, not derived
 * from the map's run encoder. Together they protect the arrival road, north
 * gate, house, bridge, stream, well, and forest edge without freezing every
 * decorative voxel into a snapshot.
 */
function expectedEvergroveLandmarks(candidate:caxecraft.content.RuntimeLevelLoader.RuntimeLevelCandidate):Bool {
	final session = candidate.generation().session();
	return queryWorld(session.worldView(), World.coord(15, 4, 12)) == Sand
		&& queryWorld(session.worldView(), World.coord(13, 7, 6)) == Wood
		&& queryWorld(session.worldView(), World.coord(22, 6, 23)) == Wood
		&& queryWorld(session.worldView(), World.coord(22, 5, 24)) == Air
		&& queryWorld(session.worldView(), World.coord(22, 4, 23)) == Stone
		&& queryWorld(session.worldView(), World.coord(5, 4, 21)) == Wood
		&& queryWorld(session.worldView(), World.coord(5, 4, 20)) == Air
		&& queryWorld(session.worldView(), World.coord(5, 3, 20)) == Air
		&& queryWorld(session.worldView(), World.coord(5, 2, 20)) == Sand
		&& queryWorld(session.worldView(), World.coord(5, 3, 21)) == Air
		&& queryWorld(session.worldView(), World.coord(12, 5, 22)) == Air
		&& queryWorld(session.worldView(), World.coord(9, 7, 7)) == Wood;
}

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

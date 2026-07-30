package caxecraft.qa;

import caxecraft.content.ActorIdentityPlanner.ActorIdentityPlanError;
import caxecraft.content.ActorIdentityPlanner.ActorIdentityPlanResult;
import caxecraft.content.ActorIdentityPlanner.actorEntityId;
import caxecraft.content.ActorIdentityPlanner.planActorIdentities;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnRole;
import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.BaseAquaticProfile;
import caxecraft.content.FirstPlayableSessionLoader.FirstPlayableSessionLoad;
import caxecraft.content.FirstPlayableSessionLoader.LoadedActorBinding;
import caxecraft.content.FirstPlayableSessionLoader.loadCandidate;
import caxecraft.domain.Aquatics.input as aquaticInput;
import caxecraft.domain.ActorControllerEvent.*;
import caxecraft.domain.ActorControllerTick.ActorControllerTickStatus;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.Character.start as startCharacter;
import caxecraft.domain.CharacterDamagePolicy;
import caxecraft.domain.CharacterPhysics.body as createBody;
import caxecraft.domain.EntityId;
import caxecraft.domain.GameSession;
import caxecraft.domain.World;
import caxecraft.domain.WorldRead.query as queryWorld;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.MiningOutcome;
import caxecraft.gameplay.RecoveryDecision;
import caxecraft.scenario.ScenarioId;

/**
	Executable specification for unpublished level assembly and session ownership.

	Eval and generated native C construct the same candidate session through the
	temporary CAXEMAP adapter. The assertions observe only typed session methods;
	the test never borrows the world's or item table's backing storage.
**/
var observed:Int = 0;

function main():Void {
	#if c
	observed = selfCheck() + trace();
	#else
	Sys.println(selfCheck());
	Sys.println(trace());
	#end
}

/** Return zero, or the stable number of the first broken assembly invariant. */
function selfCheck():Int {
	final identityFailure = checkActorIdentityPlanning();
	if (identityFailure != 0)
		return identityFailure;

	final loadedSession = new GameSession();
	final loaded = loadBuiltInCandidate(loadedSession, 6);
	if (!loaded.valid)
		return 1;
	if (loaded.waterPresentationCell != 5)
		return 2;
	final dialogueActorId = actorByRole(loaded.actors, true);
	final enemyActorId = actorByRole(loaded.actors, false);
	if (!dialogueActorId.isValid() || !enemyActorId.isValid())
		return 45;
	if (loadedSession.characterCount() != 3
		|| !loadedSession.hasLocalPlayer()
		|| loadedSession.readLocalPlayer().id.storageCode() != 1
		|| loadedSession.actorControllerSnapshots().length != 2)
		return 44;
	if (!loadedSession.actorInteractionAvailable(dialogueActorId) || loadedSession.actorInteractionAvailable(enemyActorId))
		return 46;
	final firstEnemyDamage = loadedSession.damageCharacter(enemyActorId, 1);
	if (!firstEnemyDamage.resolved
		|| firstEnemyDamage.damageApplied != 1
		|| firstEnemyDamage.defeated
		|| firstEnemyDamage.character.vitals.health != 2
		|| loadedSession.readCharacter(enemyActorId).vitals.health != 2)
		return 47;
	final defeatedEnemy = loadedSession.damageCharacter(enemyActorId, 2);
	if (!defeatedEnemy.resolved
		|| defeatedEnemy.damageApplied != 2
		|| !defeatedEnemy.defeated
		|| defeatedEnemy.character.vitals.health != 0)
		return 48;
	final actorTick = loadedSession.stepAuthoredActorControllers(0, CharacterDamagePolicy.Invulnerable);
	switch actorTick.status {
		case ControllersAdvanced if (actorTick.processed == 2 && actorTick.emittedEvents == 2):
		case _:
			return 49;
	}
	final actorEvents = loadedSession.actorControllerEventSnapshots();
	if (actorEvents.length != 2)
		return 50;
	switch actorEvents[0] {
		case DropRequested(id, drop) if (id == enemyActorId && drop.text() == "caxecraft:mossling-berries"):
		case _:
			return 51;
	}
	switch actorEvents[1] {
		case InteractionAvailable(id) if (id == dialogueActorId):
		case _:
			return 52;
	}
	if (!loadedSession.authoredItemIsActive(0) || loadedSession.authoredItemIsActive(1))
		return 3;
	if (loadedSession.pendingWaterWork() <= 0 || loadedSession.worldStateHash() == 0)
		return 4;
	final editable = World.coord(16, 4, 16);
	if (queryWorld(loadedSession.worldView(), editable) != Grass)
		return 15;
	final beforeFullInventory = loadedSession.worldStateHash();
	final fullInventory = Inventory.make(0, Inventory.MAX_STACK, 0, 0, 0, 0, 0, 0, 0);
	final rejectedMining = loadedSession.mineTerrain(editable, fullInventory);
	if (rejectedMining.outcome != MiningOutcome.InventoryFull
		|| loadedSession.worldStateHash() != beforeFullInventory
		|| queryWorld(loadedSession.worldView(), editable) != Grass)
		return 16;
	final mined = loadedSession.mineTerrain(editable, Inventory.make(0, 0, 0, 0, 0, 0, 0, 0, 0));
	if (mined.outcome != MiningOutcome.Collected || mined.inventory.grass != 1 || queryWorld(loadedSession.worldView(), editable) != Air)
		return 17;
	if (!loadedSession.placeTerrain(editable, Grass) || queryWorld(loadedSession.worldView(), editable) != Grass)
		return 18;
	if (loadedSession.removeTerrain(World.coord(16, 0, 16)))
		return 19;

	final beforeRejectedRun = loadedSession.worldStateHash();
	if (loadedSession.writeTerrainRunDuringLoad(World.VOLUME, 1, 1) != -1
		|| loadedSession.writeTerrainRunDuringLoad(0, 10, 1) != -1
		|| loadedSession.worldStateHash() != beforeRejectedRun)
		return 5;
	if (loadedSession.activateAuthoredItemDuringLoad(-1))
		return 6;

	final session = new GameSession();
	final unboundView = session.view();
	if (unboundView.valid || unboundView.localPlayer.id.isValid() || unboundView.completedTicks != 0)
		return 7;
	final recoveryInventory = Inventory.make(5, 0, 0, 0, 0, 0, 2, 0, 0);
	final rejectedRecovery = session.useSelectedRecovery(recoveryInventory);
	if (rejectedRecovery.resolved
		|| rejectedRecovery.character.id.isValid()
		|| rejectedRecovery.inventory.berries != recoveryInventory.berries
		|| session.authoredItemIsActive(0))
		return 20;
	final rejectedAttack = session.receiveLocalPlayerAttack();
	if (rejectedAttack.resolved || rejectedAttack.character.id.isValid())
		return 21;
	final pendingBeforeRejectedTick = session.pendingWaterWork();
	final missingPlayerTick = session.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (session.hasLocalPlayer()
		|| missingPlayerTick.committed
		|| missingPlayerTick.tickIndex != -1
		|| session.completedTickCount() != 0
		|| missingPlayerTick.water.processed != 0
		|| missingPlayerTick.water.remaining != pendingBeforeRejectedTick
		|| session.pendingWaterWork() != pendingBeforeRejectedTick)
		return 8;
	final localId = EntityId.fromValidatedStorageCode(11);
	final localProfile = BaseContentPack.aquaticProfile(BaseContentPack.defaultAquaticProfile());
	if (!session.bindLocalPlayer(startCharacter(localId, createBody(8.5, 2.0, 8.5), localProfile, 4))
		|| !session.hasLocalPlayer()
		|| session.readLocalPlayer().id != localId)
		return 9;
	final boundView = session.view();
	if (!boundView.valid || boundView.localPlayer.id != localId || boundView.completedTicks != 0)
		return 10;
	final npcId = EntityId.fromValidatedStorageCode(12);
	final npcStart = startCharacter(npcId, createBody(10.5, 2.0, 8.5), localProfile, 4);
	if (!session.addCharacter(npcStart)
		|| session.addCharacter(npcStart)
		|| session.characterCount() != 2
		|| session.readCharacter(npcId).id != npcId)
		return 30;
	final localBeforeNpcStep = session.readLocalPlayer();
	final npcStep = session.stepCharacter(npcId, aquaticInput(-0.5, 0.0, false, false), CharacterDamagePolicy.Survival);
	if (!npcStep.resolved
		|| npcStep.character.id != npcId
		|| npcStep.character.body.x == npcStart.body.x
		|| session.readCharacter(npcId).body.x != npcStep.character.body.x
		|| session.readLocalPlayer().body.x != localBeforeNpcStep.body.x)
		return 31;
	final characterSnapshots = session.characterSnapshots();
	if (characterSnapshots.length != 2 || characterSnapshots[0].id != localId || characterSnapshots[1].id != npcId)
		return 32;
	characterSnapshots.push(npcStart);
	if (session.characterCount() != 2 || session.removeCharacter(localId) || !session.removeCharacter(npcId) || session.characterCount() != 1)
		return 33;
	if (!session.activateAuthoredItemDuringLoad(0))
		return 45;
	final aquaticEquipment = session.collectAuthoredAquaticEquipment(0, BaseContentPack.aquaticProfile(BaseAquaticProfile.TideweaveAquatics));
	if (!aquaticEquipment.resolved
		|| !aquaticEquipment.collected
		|| aquaticEquipment.character.id != localId
		|| aquaticEquipment.character.aquaticProfile.maximumBreathTicks != 1200
		|| session.authoredItemIsActive(0))
		return 22;
	final repeatedEquipment = session.collectAuthoredAquaticEquipment(0, localProfile);
	if (!repeatedEquipment.resolved
		|| repeatedEquipment.collected
		|| repeatedEquipment.character.aquaticProfile.maximumBreathTicks != 1200
		|| session.authoredItemIsActive(0))
		return 23;
	final damaged = session.receiveLocalPlayerAttack();
	if (!damaged.resolved || damaged.character.vitals.health != 3 || damaged.character.vitals.safeTicks <= 0)
		return 24;
	final persistedDamage = session.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Survival,
		waterUpdateBudget: 0
	});
	if (!persistedDamage.committed
		|| persistedDamage.tickIndex != 0
		|| persistedDamage.character.vitals.health != 3
		|| persistedDamage.character.vitals.safeTicks != damaged.character.vitals.safeTicks - 1
		|| persistedDamage.character.aquaticProfile.maximumBreathTicks != 1200)
		return 29;
	final recovered = session.useSelectedRecovery(recoveryInventory);
	if (!recovered.resolved
		|| recovered.decision != RecoveryDecision.UseBerries
		|| recovered.inventory.berries != 1
		|| recovered.character.vitals.health != 5
		|| session.readLocalPlayer().vitals.health != 5)
		return 25;
	final recoveredAgain = session.useSelectedRecovery(recovered.inventory);
	if (!recoveredAgain.resolved
		|| recoveredAgain.decision != RecoveryDecision.UseBerries
		|| recoveredAgain.inventory.berries != 0
		|| recoveredAgain.character.vitals.health != 6)
		return 26;
	final fullHealthRecovery = session.useSelectedRecovery(Inventory.make(5, 0, 0, 0, 0, 0, 1, 0, 0));
	if (!fullHealthRecovery.resolved
		|| fullHealthRecovery.decision != RecoveryDecision.HealthAlreadyFull
		|| fullHealthRecovery.inventory.berries != 1
		|| fullHealthRecovery.character.vitals.health != 6)
		return 27;
	final revived = session.reviveLocalPlayerAt(createBody(4.5, 2.0, 4.5));
	if (!revived.resolved
		|| revived.character.id != localId
		|| revived.character.body.x != 4.5
		|| revived.character.vitals.health != 6
		|| session.readLocalPlayer().body.x != 4.5)
		return 28;
	final firstTick = session.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (!firstTick.committed || firstTick.tickIndex != 1 || session.completedTickCount() != 2)
		return 12;
	final secondTick = session.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (!secondTick.committed || secondTick.tickIndex != 2 || session.completedTickCount() != 3)
		return 13;
	final committedView = session.view();
	if (!committedView.valid || committedView.localPlayer.id != localId || committedView.completedTicks != 3)
		return 14;
	return 0;
}

/**
	Prove authored actor IDs stay stable and fail closed before session mutation.

	The two `actor.a…` values below are valid CAXEMAP identifiers and an actual
	collision for the production FNV-1a mapping. Keeping that pair here exercises
	the real algorithm; the test does not gain a privileged hash override.
**/
function checkActorIdentityPlanning():Int {
	final nia = new ScenarioId("guide.nia");
	final mossling = new ScenarioId("enemy.mossling");
	final ivvy = new ScenarioId("companion.ivvy");
	final niaId = actorEntityId(nia);
	final mosslingId = actorEntityId(mossling);
	if (!niaId.isValid() || !mosslingId.isValid() || niaId == mosslingId)
		return 34;

	final first = switch planActorIdentities([nia, mossling]) {
		case Planned(bindings): bindings;
		case Rejected(_): return 35;
	};
	final reordered = switch planActorIdentities([mossling, nia]) {
		case Planned(bindings): bindings;
		case Rejected(_): return 36;
	};
	final extended = switch planActorIdentities([ivvy, nia, mossling]) {
		case Planned(bindings): bindings;
		case Rejected(_): return 37;
	};
	if (first.length != 2
		|| first[0].authoredId.text() != "guide.nia"
		|| first[0].entityId != niaId
		|| first[1].authoredId.text() != "enemy.mossling"
		|| first[1].entityId != mosslingId)
		return 38;
	if (reordered[0].entityId != mosslingId || reordered[1].entityId != niaId || extended[1].entityId != niaId || extended[2].entityId != mosslingId)
		return 39;

	switch planActorIdentities([]) {
		case Planned(bindings) if (bindings.length == 0):
		case _:
			return 40;
	}
	switch planActorIdentities([nia, nia]) {
		case Rejected(DuplicateAuthoredId(id)) if (id.text() == "guide.nia"):
		case _:
			return 41;
	}

	final collisionLeft = new ScenarioId("actor.abn8u2c");
	final collisionRight = new ScenarioId("actor.aowprp4");
	switch planActorIdentities([collisionLeft, collisionRight]) {
		case Rejected(StorageCodeCollision(firstId, secondId, storageCode))
			if (firstId.text() == "actor.abn8u2c" && secondId.text() == "actor.aowprp4" && storageCode == 2001336683):
		case _:
			return 42;
	}

	final tooMany:Array<ScenarioId> = [];
	for (index in 0...caxecraft.domain.EntityStore.MAX_CHARACTERS + 1)
		tooMany.push(new ScenarioId('actor.a$index'));
	switch planActorIdentities(tooMany) {
		case Rejected(CharacterCapacityExceeded(count, maximum))
			if (count == caxecraft.domain.EntityStore.MAX_CHARACTERS + 1 && maximum == caxecraft.domain.EntityStore.MAX_CHARACTERS):
		case _:
			return 43;
	}
	return 0;
}

/** Stable cross-target summary of the assembled candidate's owned state. */
function trace():Int {
	final session = new GameSession();
	final loaded = loadBuiltInCandidate(session, 6);
	if (!loaded.valid)
		return -1;
	var hash = CaxecraftTrace.mix(session.worldStateHash(), loaded.waterPresentationCell);
	hash = CaxecraftTrace.mix(hash, session.pendingWaterWork());
	hash = CaxecraftTrace.mix(hash, session.authoredItemIsActive(0) ? 1 : 0);
	hash = CaxecraftTrace.mix(hash, session.actorControllerSnapshots().length);
	return CaxecraftTrace.mix(hash, session.characterCount());
}

/** Select the temporary generated first-playable bridge at the test edge. */
private function loadBuiltInCandidate(session:GameSession, health:Int):FirstPlayableSessionLoad
	return loadCandidate(session, health);

/**
	Return the one actor matching the requested generic role, or an invalid ID.

	This test validates the current first-playable shape without teaching the
	engine loader a Nia or Mossling name. Multiple matches are rejected because
	the current presentation has one slot for each role.
**/
private function actorByRole(actors:Array<LoadedActorBinding>, dialogue:Bool):EntityId {
	var result = EntityId.invalid();
	for (actor in actors) {
		final matches = switch actor.role {
			case DialogueNpc(_): dialogue;
			case EnemyActor: !dialogue;
		};
		if (matches) {
			if (result.isValid())
				return EntityId.invalid();
			result = actor.entityId;
		}
	}
	return result;
}

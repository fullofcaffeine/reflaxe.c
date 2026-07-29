package caxecraft.qa;

import caxecraft.content.FirstPlayableSessionLoader.loadCandidate;
import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.BaseAquaticProfile;
import caxecraft.domain.Aquatics.input as aquaticInput;
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
	final session = new GameSession();
	final loaded = loadCandidate(session);
	if (!loaded.valid)
		return 1;
	if (loaded.waterPresentationCell != 5)
		return 2;
	if (!session.authoredItemIsActive(0) || session.authoredItemIsActive(1))
		return 3;
	if (session.pendingWaterWork() <= 0 || session.worldStateHash() == 0)
		return 4;
	final editable = World.coord(16, 4, 16);
	if (queryWorld(session.worldView(), editable) != Grass)
		return 15;
	final beforeFullInventory = session.worldStateHash();
	final fullInventory = Inventory.make(0, Inventory.MAX_STACK, 0, 0, 0, 0, 0, 0, 0);
	final rejectedMining = session.mineTerrain(editable, fullInventory);
	if (rejectedMining.outcome != MiningOutcome.InventoryFull
		|| session.worldStateHash() != beforeFullInventory
		|| queryWorld(session.worldView(), editable) != Grass)
		return 16;
	final mined = session.mineTerrain(editable, Inventory.make(0, 0, 0, 0, 0, 0, 0, 0, 0));
	if (mined.outcome != MiningOutcome.Collected || mined.inventory.grass != 1 || queryWorld(session.worldView(), editable) != Air)
		return 17;
	if (!session.placeTerrain(editable, Grass) || queryWorld(session.worldView(), editable) != Grass)
		return 18;
	if (session.removeTerrain(World.coord(16, 0, 16)))
		return 19;

	final beforeRejectedRun = session.worldStateHash();
	if (session.writeTerrainRunDuringLoad(World.VOLUME, 1, 1) != -1
		|| session.writeTerrainRunDuringLoad(0, 10, 1) != -1
		|| session.worldStateHash() != beforeRejectedRun)
		return 5;
	if (session.activateAuthoredItemDuringLoad(-1))
		return 6;
	final unboundView = session.view();
	if (unboundView.valid || unboundView.localPlayer.id.isValid() || unboundView.completedTicks != 0)
		return 7;
	final recoveryInventory = Inventory.make(5, 0, 0, 0, 0, 0, 2, 0, 0);
	final rejectedRecovery = session.useSelectedRecovery(recoveryInventory);
	if (rejectedRecovery.resolved
		|| rejectedRecovery.character.id.isValid()
		|| rejectedRecovery.inventory.berries != recoveryInventory.berries
		|| !session.authoredItemIsActive(0))
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

/** Stable cross-target summary of the assembled candidate's owned state. */
function trace():Int {
	final session = new GameSession();
	final loaded = loadCandidate(session);
	if (!loaded.valid)
		return -1;
	var hash = CaxecraftTrace.mix(session.worldStateHash(), loaded.waterPresentationCell);
	hash = CaxecraftTrace.mix(hash, session.pendingWaterWork());
	return CaxecraftTrace.mix(hash, session.authoredItemIsActive(0) ? 1 : 0);
}

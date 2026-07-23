package caxecraft.qa;

import caxecraft.content.FirstPlayableSessionLoader.loadCandidate;
import caxecraft.content.BaseContentPack;
import caxecraft.domain.Aquatics.input as aquaticInput;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.Character.start as startCharacter;
import caxecraft.domain.CharacterDamagePolicy;
import caxecraft.domain.CharacterPhysics.body as createBody;
import caxecraft.domain.EntityId;
import caxecraft.domain.GameSession;
import caxecraft.domain.World;

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
	if (session.water.pending() <= 0 || session.worldStateHash() == 0)
		return 4;

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
	final pendingBeforeRejectedTick = session.water.pending();
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
		|| session.water.pending() != pendingBeforeRejectedTick)
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
	final otherId = EntityId.fromValidatedStorageCode(12);
	if (session.replaceLocalPlayer(startCharacter(otherId, createBody(8.5, 2.0, 8.5), localProfile, 4)))
		return 11;
	final firstTick = session.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (!firstTick.committed || firstTick.tickIndex != 0 || session.completedTickCount() != 1)
		return 12;
	final secondTick = session.tick({
		intent: aquaticInput(0.0, 0.0, false, false),
		damagePolicy: CharacterDamagePolicy.Invulnerable,
		waterUpdateBudget: 0
	});
	if (!secondTick.committed || secondTick.tickIndex != 1 || session.completedTickCount() != 2)
		return 13;
	final committedView = session.view();
	if (!committedView.valid || committedView.localPlayer.id != localId || committedView.completedTicks != 2)
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
	hash = CaxecraftTrace.mix(hash, session.water.pending());
	return CaxecraftTrace.mix(hash, session.authoredItemIsActive(0) ? 1 : 0);
}

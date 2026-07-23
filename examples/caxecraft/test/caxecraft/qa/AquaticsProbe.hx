package caxecraft.qa;

import caxecraft.domain.AquaticProfile;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.Character.applyAttack as applyCharacterAttack;
import caxecraft.domain.Character.isValid as isValidCharacter;
import caxecraft.domain.Character.start as startCharacter;
import caxecraft.domain.Character.step as stepCharacter;
import caxecraft.domain.CharacterDamagePolicy;
import caxecraft.domain.EntityId;
import caxecraft.domain.EntityStore;
import caxecraft.domain.GameSession;
import caxecraft.domain.Aquatics.canMine;
import caxecraft.domain.Aquatics.classifyMedium;
import caxecraft.domain.Aquatics.adoptProfile;
import caxecraft.domain.Aquatics.hasColdProtection;
import caxecraft.domain.Aquatics.input as aquaticInput;
import caxecraft.domain.Aquatics.observe;
import caxecraft.domain.Aquatics.profile;
import caxecraft.domain.Aquatics.start as startAquatics;
import caxecraft.domain.Aquatics.step as stepAquatics;
import caxecraft.domain.AquaticMedium;
import caxecraft.domain.PlayerAgent.bind as bindPlayer;
import caxecraft.domain.CharacterPhysics.body as createPlayer;
import caxecraft.domain.CharacterBody;
import caxecraft.domain.WaterPendingCells;
import caxecraft.domain.WaterSimulation;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.domain.WorldStorage;
import caxecraft.domain.WorldVolume;
import caxecraft.gameplay.WorldItemPickup.isInRange as itemIsInRange;
#if c
import c.CArray;
import c.UInt8;
#end

/**
	Executable specification for fixed-tick swimming and breath.

	The setup creates shallow and deep source columns directly through the water
	API. Eval and native C then run the same observations, state transitions,
	forces, air loss, recovery, and generic capability checks. The native harness
	only prints the two scalar results; it contains no aquatic rule.
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

/** Return zero, or the stable number of the first broken aquatic invariant. */
function selfCheck():Int {
	#if c
	var worldStorage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
	var pendingStorage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
	var cells:WorldCells = worldStorage.span();
	var pending:WaterPendingCells = pendingStorage.span();
	#else
	var cells:WorldCells = zeroes();
	var pending:WaterPendingCells = zeroes();
	#end
	prepare(cells, pending);
	final basic = basicProfile();
	final enhanced = enhancedProfile();

	final dryPlayer = createPlayer(12.5, 1.0, 12.5);
	final dry = observe(cells, dryPlayer);
	if (dry.submersion != 0.0 || dry.feetWet || dry.bodyWet || dry.headWet || dry.cameraBlend != 0.0)
		return 1;
	if (classifyMedium(AquaticMedium.Dry, dry) != AquaticMedium.Dry)
		return 2;

	final wadingPlayer = createPlayer(4.5, 1.0, 4.5);
	final wading = observe(cells, wadingPlayer);
	if (!near(wading.submersion, 1.0 / 1.8) || !wading.feetWet || !wading.bodyWet || wading.headWet)
		return 3;
	if (classifyMedium(AquaticMedium.Dry, wading) != AquaticMedium.Wading)
		return 4;

	final floatingPlayer = createPlayer(8.5, 1.4, 8.5);
	final floating = observe(cells, floatingPlayer);
	if (floating.submersion < 0.80 || floating.headWet || floating.cameraBlend <= 0.0 || floating.cameraBlend >= 1.0)
		return 5;
	if (classifyMedium(AquaticMedium.Wading, floating) != AquaticMedium.Floating)
		return 6;

	final submergedPlayer = createPlayer(8.5, 1.2, 8.5);
	final submerged = observe(cells, submergedPlayer);
	if (!near(submerged.submersion, 1.0) || !submerged.headWet || submerged.cameraBlend != 1.0)
		return 7;
	if (classifyMedium(AquaticMedium.Floating, submerged) != AquaticMedium.Submerged)
		return 8;

	final enterFloat = observe(cells, createPlayer(8.5, 1.75, 8.5));
	final stayFloat = observe(cells, createPlayer(8.5, 1.80, 8.5));
	if (classifyMedium(AquaticMedium.Wading, enterFloat) != AquaticMedium.Floating
		|| classifyMedium(AquaticMedium.Floating, stayFloat) != AquaticMedium.Floating
		|| classifyMedium(AquaticMedium.Wading, stayFloat) != AquaticMedium.Wading)
		return 9;

	final initial = startAquatics(basic);
	final rising = stepAquatics(cells, submergedPlayer, initial, aquaticInput(0.0, 0.0, true, false), basic);
	final descending = stepAquatics(cells, submergedPlayer, initial, aquaticInput(0.0, 0.0, false, true), basic);
	if (rising.body.velocityY <= 0.0 || descending.body.velocityY >= 0.0 || rising.body.y <= descending.body.y)
		return 10;

	final basicMove = stepAquatics(cells, submergedPlayer, initial, aquaticInput(1.0, 0.0, false, false), basic);
	final enhancedMove = stepAquatics(cells, submergedPlayer, startAquatics(enhanced), aquaticInput(1.0, 0.0, false, false), enhanced);
	if (enhancedMove.body.velocityX <= basicMove.body.velocityX || enhanced.maximumBreathTicks <= basic.maximumBreathTicks)
		return 11;
	if (canMine(basicMove.aquatic, basic)
		|| !canMine(enhancedMove.aquatic, enhanced)
		|| hasColdProtection(basic)
		|| !hasColdProtection(enhanced))
		return 12;

	setDeepPoolRoof(cells, true);
	var breathStep = stepAquatics(cells, submergedPlayer, initial, aquaticInput(0.0, 0.0, false, false), basic);
	var ticks = 1;
	while (ticks < basic.maximumBreathTicks) {
		breathStep = stepAquatics(cells, breathStep.body, breathStep.aquatic, aquaticInput(0.0, 0.0, false, false), basic);
		if (breathStep.drowningDamage != 0)
			return 13;
		ticks++;
	}
	if (breathStep.aquatic.breathTicks != 0)
		return 14;
	var drowningTicks = 0;
	var damage = 0;
	while (drowningTicks < basic.drowningIntervalTicks) {
		breathStep = stepAquatics(cells, breathStep.body, breathStep.aquatic, aquaticInput(0.0, 0.0, false, false), basic);
		damage += breathStep.drowningDamage;
		drowningTicks++;
	}
	if (damage != 1)
		return 15;

	var recovered = breathStep.aquatic;
	var dryTicks = 0;
	while (dryTicks < 3) {
		final result = stepAquatics(cells, dryPlayer, recovered, aquaticInput(0.0, 0.0, false, false), basic);
		recovered = result.aquatic;
		dryTicks++;
	}
	if (recovered.breathTicks != basic.maximumBreathTicks || recovered.medium != AquaticMedium.Dry)
		return 16;
	if (startAquatics(basic).breathTicks != basic.maximumBreathTicks || startAquatics(basic).drowningTicks != basic.drowningIntervalTicks)
		return 17;

	setDeepPoolRoof(cells, false);
	var breach = stepAquatics(cells, submergedPlayer, initial, aquaticInput(0.0, 0.0, true, false), basic);
	var breachTicks = 0;
	while (breach.immersion.headWet && breachTicks < 40) {
		breach = stepAquatics(cells, breach.body, breach.aquatic, aquaticInput(0.0, 0.0, true, false), basic);
		breachTicks++;
	}
	if (breach.immersion.headWet || breach.body.y <= submergedPlayer.y || breachTicks >= 40)
		return 18;

	var spent = stepAquatics(cells, submergedPlayer, initial, aquaticInput(0.0, 0.0, false, false), basic).aquatic;
	spent = stepAquatics(cells, submergedPlayer, spent, aquaticInput(0.0, 0.0, false, false), basic).aquatic;
	final upgraded = adoptProfile(spent, basic, enhanced);
	if (upgraded.breathTicks != enhanced.maximumBreathTicks - 2
		|| upgraded.medium != spent.medium
		|| upgraded.headSubmerged != spent.headSubmerged)
		return 19;
	final downgraded = adoptProfile(upgraded, enhanced, basic);
	if (downgraded.breathTicks != basic.maximumBreathTicks - 2 || downgraded.drowningTicks > basic.drowningIntervalTicks)
		return 20;
	if (!itemIsInRange(4.5, 4.1, 4.5, 4500, 5000, 4500) || itemIsInRange(8.0, 4.1, 8.0, 4500, 5000, 4500))
		return 21;
	final sharedCharacterResult = sharedCharacterCheck(cells, basic);
	if (sharedCharacterResult != 0)
		return 21 + sharedCharacterResult;
	return 0;
}

/**
	Prove player and non-player controllers share one character implementation.

	The two stores deliberately begin with equal component state but different
	stable IDs. Equal intents must produce equal movement, water, and health; the
	player binding contains only the local ID and cannot replace the NPC slot.
**/
private function sharedCharacterCheck(cells:WorldCells, capability:AquaticProfile):Int {
	final localStore = new EntityStore();
	final npcStore = new EntityStore();
	final localId = EntityId.fromValidatedStorageCode(1);
	final npcId = EntityId.fromValidatedStorageCode(2);
	final localAgent = bindPlayer(localId);
	final startBody = createPlayer(8.5, 1.2, 8.5);
	if (!localStore.put(startCharacter(localId, startBody, capability, 6))
		|| !npcStore.put(startCharacter(npcId, startBody, capability, 6)))
		return 1;
	if (!localStore.contains(localAgent.characterId) || npcStore.contains(localAgent.characterId))
		return 2;

	final intent = aquaticInput(0.5, -0.25, true, false);
	final localStep = stepCharacter(cells, localStore.read(localAgent.characterId), intent, CharacterDamagePolicy.Survival);
	final npcStep = stepCharacter(cells, npcStore.read(npcId), intent, CharacterDamagePolicy.Survival);
	if (localStep.character.body.x != npcStep.character.body.x
		|| localStep.character.body.y != npcStep.character.body.y
		|| localStep.character.body.z != npcStep.character.body.z
		|| localStep.character.aquatic.medium != npcStep.character.aquatic.medium
		|| localStep.character.aquatic.breathTicks != npcStep.character.aquatic.breathTicks
		|| localStep.character.vitals.health != npcStep.character.vitals.health)
		return 3;
	if (!localStore.replace(localAgent.characterId, localStep.character) || !npcStore.replace(npcId, npcStep.character))
		return 4;
	if (localStore.replace(localAgent.characterId, npcStep.character) || npcStore.put(localStep.character))
		return 5;
	final attackedLocal = applyCharacterAttack(localStore.read(localAgent.characterId), true);
	final attackedNpc = applyCharacterAttack(npcStore.read(npcId), true);
	if (attackedLocal.vitals.health != attackedNpc.vitals.health || attackedLocal.vitals.safeTicks != attackedNpc.vitals.safeTicks)
		return 6;
	if (isValidCharacter(localStore.read(npcId)) || isValidCharacter(npcStore.read(localId)))
		return 7;
	return 0;
}

/** Stable cross-target summary for movement, breath, and capability changes. */
function trace():Int {
	final session = new GameSession();
	prepareSession(session);
	final capability = enhancedProfile();
	final characterId = EntityId.fromValidatedStorageCode(7);
	if (!session.bindLocalPlayer(startCharacter(characterId, createPlayer(8.5, 1.0, 8.5), capability, 6)))
		return 0;
	var hash = 0x41515541;
	var tick = 0;
	while (tick < 12) {
		final ascend = tick >= 4 && tick < 9;
		final descend = tick >= 9;
		final result = session.tick({
			intent: aquaticInput(1.0, 0.25, ascend, descend),
			damagePolicy: CharacterDamagePolicy.Survival,
			waterUpdateBudget: 0
		});
		if (!result.committed)
			return 0;
		hash = CaxecraftTrace.mix(hash, Std.int(result.character.body.x * 1000.0));
		hash = CaxecraftTrace.mix(hash, Std.int(result.character.body.y * 1000.0));
		hash = CaxecraftTrace.mix(hash, result.character.aquatic.breathTicks);
		hash = CaxecraftTrace.mix(hash, result.character.aquatic.medium);
		tick++;
	}
	return hash;
}

/** Populate the session through target-neutral level-loading operations. */
private function prepareSession(session:GameSession):Void {
	session.resetEmptyWorld();
	var z = 0;
	while (z < World.DEPTH) {
		var x = 0;
		while (x < World.WIDTH) {
			session.replaceTerrainDuringLoad(World.coord(x, 0, z), caxecraft.domain.BlockKind.Bedrock);
			x++;
		}
		z++;
	}
	placeSessionLayer(session, 4, 4, 1, 0);
	placeSessionLayer(session, 8, 8, 1, 1);
}

/** Add the same small source layers used by the independent aquatic checks. */
private function placeSessionLayer(session:GameSession, centerX:Int, centerZ:Int, bottomY:Int, extraLayers:Int):Void {
	var layer = 0;
	while (layer <= extraLayers) {
		var z = centerZ - 1;
		while (z <= centerZ + 1) {
			var x = centerX - 1;
			while (x <= centerX + 1) {
				session.placeWaterSource(World.coord(x, bottomY + layer, z));
				x++;
			}
			z++;
		}
		layer++;
	}
}

private function basicProfile():AquaticProfile
	return profile(4, 2, 0.35, 14.0, 20.0, 12.0, 0.18, 3, false, false);

private function enhancedProfile():AquaticProfile
	return profile(12, 4, 0.90, 18.0, 22.0, 14.0, 0.10, 3, true, true);

private function prepare(cells:WorldCells, pending:WaterPendingCells):Void {
	clear(cells);
	makeFloor(cells);
	placeLayer(cells, pending, 4, 4, 1, 0);
	placeLayer(cells, pending, 8, 8, 1, 1);
}

private function placeLayer(cells:WorldCells, pending:WaterPendingCells, centerX:Int, centerZ:Int, bottomY:Int, extraLayers:Int):Void {
	final water = new WaterSimulation();
	water.resetPending(pending);
	var layer = 0;
	while (layer <= extraLayers) {
		var z = centerZ - 1;
		while (z <= centerZ + 1) {
			var x = centerX - 1;
			while (x <= centerX + 1) {
				water.placeSource(cells, pending, World.coord(x, bottomY + layer, z));
				x++;
			}
			z++;
		}
		layer++;
	}
}

/** Add or remove a low test roof that keeps the breath case submerged. */
private function setDeepPoolRoof(cells:WorldCells, closed:Bool):Void {
	var z = 7;
	while (z <= 9) {
		var x = 7;
		while (x <= 9) {
			if (closed)
				World.replace(cells, World.coord(x, 3, z), caxecraft.domain.BlockKind.Stone);
			else
				World.replace(cells, World.coord(x, 3, z), caxecraft.domain.BlockKind.Air);
			x++;
		}
		z++;
	}
}

private function clear(cells:WorldCells):Void {
	var index = 0;
	while (index < World.VOLUME) {
		WorldStorage.writeCode(cells, index, 0);
		index++;
	}
}

private function makeFloor(cells:WorldCells):Void {
	var z = 0;
	while (z < World.DEPTH) {
		var x = 0;
		while (x < World.WIDTH) {
			World.replace(cells, World.coord(x, 0, z), caxecraft.domain.BlockKind.Bedrock);
			x++;
		}
		z++;
	}
}

private function near(left:Float, right:Float):Bool {
	final difference = left < right ? right - left : left - right;
	return difference < 0.00001;
}

#if !c
private function zeroes():Array<Int> {
	final values:Array<Int> = [];
	var index = 0;
	while (index < World.VOLUME) {
		values.push(0);
		index++;
	}
	return values;
}
#end

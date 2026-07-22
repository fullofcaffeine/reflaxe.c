package caxecraft.qa;

import caxecraft.domain.AquaticProfile;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.PlayerAquatics.canMine;
import caxecraft.domain.PlayerAquatics.classifyMedium;
import caxecraft.domain.PlayerAquatics.hasColdProtection;
import caxecraft.domain.PlayerAquatics.input as aquaticInput;
import caxecraft.domain.PlayerAquatics.observe;
import caxecraft.domain.PlayerAquatics.profile;
import caxecraft.domain.PlayerAquatics.start as startAquatics;
import caxecraft.domain.PlayerAquatics.step as stepAquatics;
import caxecraft.domain.PlayerMedium;
import caxecraft.domain.PlayerPhysics.player as createPlayer;
import caxecraft.domain.PlayerState;
import caxecraft.domain.WaterPendingCells;
import caxecraft.domain.WaterSimulation;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.domain.WorldStorage;
import caxecraft.domain.WorldVolume;
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
	if (classifyMedium(PlayerMedium.Dry, dry) != PlayerMedium.Dry)
		return 2;

	final wadingPlayer = createPlayer(4.5, 1.0, 4.5);
	final wading = observe(cells, wadingPlayer);
	if (!near(wading.submersion, 1.0 / 1.8) || !wading.feetWet || !wading.bodyWet || wading.headWet)
		return 3;
	if (classifyMedium(PlayerMedium.Dry, wading) != PlayerMedium.Wading)
		return 4;

	final floatingPlayer = createPlayer(8.5, 1.4, 8.5);
	final floating = observe(cells, floatingPlayer);
	if (floating.submersion < 0.80 || floating.headWet || floating.cameraBlend <= 0.0 || floating.cameraBlend >= 1.0)
		return 5;
	if (classifyMedium(PlayerMedium.Wading, floating) != PlayerMedium.Floating)
		return 6;

	final submergedPlayer = createPlayer(8.5, 1.2, 8.5);
	final submerged = observe(cells, submergedPlayer);
	if (!near(submerged.submersion, 1.0) || !submerged.headWet || submerged.cameraBlend != 1.0)
		return 7;
	if (classifyMedium(PlayerMedium.Floating, submerged) != PlayerMedium.Submerged)
		return 8;

	final enterFloat = observe(cells, createPlayer(8.5, 1.75, 8.5));
	final stayFloat = observe(cells, createPlayer(8.5, 1.80, 8.5));
	if (classifyMedium(PlayerMedium.Wading, enterFloat) != PlayerMedium.Floating
		|| classifyMedium(PlayerMedium.Floating, stayFloat) != PlayerMedium.Floating
		|| classifyMedium(PlayerMedium.Wading, stayFloat) != PlayerMedium.Wading)
		return 9;

	final initial = startAquatics(basic);
	final rising = stepAquatics(cells, submergedPlayer, initial, aquaticInput(0.0, 0.0, true, false), basic);
	final descending = stepAquatics(cells, submergedPlayer, initial, aquaticInput(0.0, 0.0, false, true), basic);
	if (rising.player.velocityY <= 0.0 || descending.player.velocityY >= 0.0 || rising.player.y <= descending.player.y)
		return 10;

	final basicMove = stepAquatics(cells, submergedPlayer, initial, aquaticInput(1.0, 0.0, false, false), basic);
	final enhancedMove = stepAquatics(cells, submergedPlayer, startAquatics(enhanced), aquaticInput(1.0, 0.0, false, false), enhanced);
	if (enhancedMove.player.velocityX <= basicMove.player.velocityX || enhanced.maximumBreathTicks <= basic.maximumBreathTicks)
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
		breathStep = stepAquatics(cells, breathStep.player, breathStep.aquatic, aquaticInput(0.0, 0.0, false, false), basic);
		if (breathStep.drowningDamage != 0)
			return 13;
		ticks++;
	}
	if (breathStep.aquatic.breathTicks != 0)
		return 14;
	var drowningTicks = 0;
	var damage = 0;
	while (drowningTicks < basic.drowningIntervalTicks) {
		breathStep = stepAquatics(cells, breathStep.player, breathStep.aquatic, aquaticInput(0.0, 0.0, false, false), basic);
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
	if (recovered.breathTicks != basic.maximumBreathTicks || recovered.medium != PlayerMedium.Dry)
		return 16;
	if (startAquatics(basic).breathTicks != basic.maximumBreathTicks || startAquatics(basic).drowningTicks != basic.drowningIntervalTicks)
		return 17;

	setDeepPoolRoof(cells, false);
	var breach = stepAquatics(cells, submergedPlayer, initial, aquaticInput(0.0, 0.0, true, false), basic);
	var breachTicks = 0;
	while (breach.immersion.headWet && breachTicks < 40) {
		breach = stepAquatics(cells, breach.player, breach.aquatic, aquaticInput(0.0, 0.0, true, false), basic);
		breachTicks++;
	}
	if (breach.immersion.headWet || breach.player.y <= submergedPlayer.y || breachTicks >= 40)
		return 18;
	return 0;
}

/** Stable cross-target summary for movement, breath, and capability changes. */
function trace():Int {
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
	final capability = enhancedProfile();
	var player = createPlayer(8.5, 1.0, 8.5);
	var aquatic = startAquatics(capability);
	var hash = 0x41515541;
	var tick = 0;
	while (tick < 12) {
		final ascend = tick >= 4 && tick < 9;
		final descend = tick >= 9;
		final result = stepAquatics(cells, player, aquatic, aquaticInput(1.0, 0.25, ascend, descend), capability);
		player = result.player;
		aquatic = result.aquatic;
		hash = CaxecraftTrace.mix(hash, Std.int(player.x * 1000.0));
		hash = CaxecraftTrace.mix(hash, Std.int(player.y * 1000.0));
		hash = CaxecraftTrace.mix(hash, aquatic.breathTicks);
		hash = CaxecraftTrace.mix(hash, aquatic.medium);
		tick++;
	}
	return hash;
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

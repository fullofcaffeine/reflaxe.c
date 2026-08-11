package caxecraft.content;

import caxecraft.content.ActorContentResolver.ActorMechanicsProfile;

/**
	Owns the reusable safety limits for content-defined actor mechanics.

	Both spawn planning and live publication call this one predicate. A custom
	content adapter or direct test value therefore cannot pass one boundary with
	health, aquatic, or controller values that the other boundary would reject.
**/
/**
	Return true only when every actor-mechanics value is safe for this engine.

	The JSON loader already checks the built-in pack. Repeating the typed check at
	the planning and publication boundaries protects future runtime pack adapters
	and callers that construct an `ActorMechanicsProfile` directly.
**/
function isValidActorMechanics(profile:ActorMechanicsProfile):Bool {
	if (profile.maximumHealth < 1 || profile.maximumHealth > 10000)
		return false;
	final aquatic = profile.aquaticProfile;
	if (aquatic.maximumBreathTicks < 1 || aquatic.maximumBreathTicks > 12000 || aquatic.breathRecoveryPerTick < 1 || aquatic.breathRecoveryPerTick > 120
		|| aquatic.horizontalControl < 0.0 || aquatic.horizontalControl > 1.0 || aquatic.ascentAcceleration < 0.0 || aquatic.ascentAcceleration > 40.0
		|| aquatic.descentAcceleration < 0.0 || aquatic.descentAcceleration > 40.0 || aquatic.buoyancyAcceleration < 0.0
		|| aquatic.buoyancyAcceleration > 30.0 || aquatic.dragPerTick < 0.0 || aquatic.dragPerTick > 0.9 || aquatic.drowningIntervalTicks < 1
		|| aquatic.drowningIntervalTicks > 1200)
		return false;
	return switch profile.controller {
		case StationaryDialogue(radius): radius >= 250 && radius <= 32000;
		case WanderChaseMelee(controller) | TelegraphedCharge(controller):
			controller.noticeRadiusMilli >= 250
			&& controller.noticeRadiusMilli <= 64000
			&& controller.strikeRadiusMilli >= controller.attackRadiusMilli
			&& controller.noticeRadiusMilli >= controller.strikeRadiusMilli
			&& controller.attackRadiusMilli >= 250
			&& controller.windupTicks >= 1
			&& controller.windupTicks <= 1200
			&& controller.recoveryTicks >= 1
			&& controller.recoveryTicks <= 1200
			&& controller.stepMilli >= 1
			&& controller.stepMilli <= 10000;
	};
}

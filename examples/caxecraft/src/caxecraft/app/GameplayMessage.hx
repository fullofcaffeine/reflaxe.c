package caxecraft.app;

import caxecraft.scenario.MessageId;

/**
 * Names the generic gameplay notices that a playable map can present.
 *
 * The enum gives application code stable semantic slots without embedding any
 * player-visible prose or campaign names. Each active CAXEMAP supplies the
 * translated text for these IDs, so creators can change the words without
 * rebuilding Caxecraft.
 */
enum abstract GameplayMessage(Int) {
	var PickupOne = 0;
	var PickupMany = 1;
	var RecoveryUsed = 2;
	var BerryStackFull = 3;
	var BlockStackFull = 4;
	var AttackHit = 5;
	var PlayerFallen = 6;
	var EnemyAlert = 7;
	var EnemyDroppedItems = 8;
	var EnemyWindup = 9;
	var GuideTalk = 11;
	var RecoveryEmpty = 13;
	var ReturnPrompt = 14;
	var EnemyHitWarning = 15;
}

/** Return the locale-independent CAXEMAP key for one gameplay notice. */
function gameplayMessageId(message:GameplayMessage):MessageId {
	return new MessageId(switch message {
		case PickupOne: "pickup_one";
		case PickupMany: "pickup_many";
		case RecoveryUsed: "recovery_used";
		case BerryStackFull: "berry_stack_full";
		case BlockStackFull: "block_stack_full";
		case AttackHit: "attack_hit";
		case PlayerFallen: "player_fallen";
		case EnemyAlert: "enemy_alert";
		case EnemyDroppedItems: "enemy_dropped_items";
		case EnemyWindup: "enemy_windup";
		case GuideTalk: "guide_talk";
		case RecoveryEmpty: "recovery_empty";
		case ReturnPrompt: "return_prompt";
		case EnemyHitWarning: "enemy_hit_warning";
	});
}

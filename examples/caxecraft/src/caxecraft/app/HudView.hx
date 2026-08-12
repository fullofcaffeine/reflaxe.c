package caxecraft.app;

import caxecraft.domain.ActorControllerPhase;
import caxecraft.domain.Character;
import caxecraft.domain.RaycastHit;
import caxecraft.domain.VitalsState;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelPresentation;
import caxecraft.gameplay.InventoryFullReason;
import caxecraft.gameplay.InventoryState;
import caxecraft.gameplay.RecoveryDecision;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.scenario.ScenarioId;
import caxecraft.app.InteractionPrompt.InteractionPrompt;
import caxecraft.app.ConversationFlow.ConversationState;

/**
	Read-only facts needed to draw one Caxecraft heads-up display (HUD) frame.

	The outer application creates this value only after the current simulation
	work is committed. The renderer can therefore inspect health, inventory,
	prompts, and short-lived feedback without receiving `GameSession`, a mutable
	storage view, or permission to change gameplay. Raylib textures live in the
	separate `HudResources` presentation value, so this view remains ordinary,
	target-neutral Haxe.

	The nested records group facts by purpose and give every call-site value a
	name. The compiler lowers these immutable records to direct C structs with no
	heap allocation. Dialogue is identified by the stable ID requested by
	CaxeFlow; the active level resolves its words. Actor position, health, and
	controller phase come from the authoritative generic session.
**/
typedef HudView = {
	/** Performance and deterministic-clock facts shown by the debug panel. */
	final metrics:HudMetricsView;

	/** Player facts that the HUD may observe but never mutate. */
	final character:HudCharacterView;

	/** Feedback produced by committed interaction and gameplay decisions. */
	final feedback:HudFeedbackView;

	/** True when the current application screen pauses fixed simulation. */
	final paused:Bool;

	/** True when the current application screen owns relative mouse input. */
	final pointerCaptured:Bool;

	/** Last committed block-selection result. */
	final hit:RaycastHit;

	/** Selected game mode used by the current progress panel. */
	final mode:GameMode;

	/** Validated locale cursor shared by user-interface and campaign catalogs. */
	final locale:LocaleCursor;

	/** Immutable bounded inventory snapshot. */
	final inventory:InventoryState;

	/** Generic line and reveal cursor for the blocking conversation panel. */
	final conversation:Null<ConversationState>;

	/** Stable reloadable dialogue identity paired with the generic cursor. */
	final activeDialogue:Null<ScenarioId>;

	/** Authored speaker identity for the current line, or null for narration. */
	final conversationSpeaker:Null<ScenarioId>;

	/** Reloadable atlas selected by the current speaker's NPC profile. */
	final conversationPortraitAsset:String;

	/** Reloadable atlas cell selected by the current speaker's NPC profile. */
	final conversationPortraitCell:Int;

	/** Player-facing meaning of the nearest available authored interaction. */
	final interactionPrompt:InteractionPrompt;

	/** Committed first-playable enemy character from the generic entity store. */
	final enemy:Character;

	/** Read-only generic controller phase used for enemy prompt presentation. */
	final enemyPhase:ActorControllerPhase;

	/** Runtime-selected campaign level ID, or the standalone map path. */
	final levelLabel:String;

	/** Localized initial objective selected from the same published level. */
	final objectiveTitle:String;

	/** Localized title of the latest unlocked journal entry, or empty text. */
	final journalTitle:String;

	/** Localized body of the latest unlocked journal entry, or empty text. */
	final journalBody:String;

	/** Validated message catalog selected with the same published level. */
	final presentation:RuntimeLevelPresentation;
}

/** Small debug-panel snapshot for one rendered frame. */
typedef HudMetricsView = {
	/** Terrain and water blocks admitted by the current visibility pass. */
	final visibleBlocks:Int;

	/** Terrain and water draw calls submitted for the current frame. */
	final drawCalls:Int;

	/** Number of outer display frames completed before this snapshot. */
	final renderedFrames:Int;

	/** Number of deterministic fixed simulation steps already committed. */
	final completedTicks:Int;
}

/** Player-owned values that affect HUD presentation. */
typedef HudCharacterView = {
	/** Committed horizontal world position used for nearby prompts. */
	final x:Float;

	/** Committed depth world position used for nearby prompts. */
	final z:Float;

	/** Health and temporary damage protection. */
	final vitals:VitalsState;

	/** Stable authored item code, or a negative value when no gear is equipped. */
	final aquaticEquipmentCode:Int;

	/** Whether the recent-equipment message is still visible. */
	final aquaticEquipmentVisible:Bool;

	/** Whether the committed eye position is underwater. */
	final headSubmerged:Bool;

	/** Remaining deterministic breath time. */
	final breathTicks:Int;

	/** Profile-defined full breath time. */
	final maximumBreathTicks:Int;
}

/** Short-lived messages and highlights produced by committed gameplay. */
typedef HudFeedbackView = {
	/** Whether the most recent placement attempt was rejected. */
	final placementBlocked:Bool;

	/** Whether the player's current strike connected. */
	final strikeHit:Bool;

	/** Whether an enemy was defeated during the feedback window. */
	final enemyDefeated:Bool;

	/** Whether an enemy attack connected during the feedback window. */
	final enemyAttacked:Bool;

	/** Whether a pickup message is visible. */
	final pickedUp:Bool;

	/** Quantity shown by the current pickup message. */
	final pickupAmount:Int;

	/** Typed reason the bounded inventory rejected its latest transfer. */
	final inventoryFullReason:InventoryFullReason;

	/** Typed result of the latest recovery-item decision. */
	final recoveryDecision:RecoveryDecision;

	/** Whether the recovery decision should currently be shown. */
	final recoveryVisible:Bool;
}

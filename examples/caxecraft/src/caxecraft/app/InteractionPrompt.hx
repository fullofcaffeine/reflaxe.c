package caxecraft.app;

/**
	Describes the player-facing meaning of a nearby world interaction.

	The application keeps the actual actor or object identity private. This
	module carries only the small presentation decision needed by the heads-up
	display, so a mechanism cannot inherit an NPC's talk prompt.
**/
enum InteractionTargetKind {
	/** No authored interaction is currently in range. */
	NoInteractionTarget;

	/** A character owns the nearest valid interaction. */
	DialogueInteractionTarget;

	/** A stateful world mechanism owns the nearest valid interaction. */
	MechanismInteractionTarget;
}

/** The closed prompt meaning that the heads-up display may present. */
enum InteractionPrompt {
	/** Draw no interaction prompt. */
	NoInteractionPrompt;

	/** Invite the player to talk to a character. */
	TalkInteractionPrompt;

	/** Invite the player to use a world mechanism. */
	UseInteractionPrompt;
}

/** Convert one target kind to its player-facing interaction meaning. */
function interactionPrompt(kind:InteractionTargetKind):InteractionPrompt
	return switch kind {
		case NoInteractionTarget: NoInteractionPrompt;
		case DialogueInteractionTarget: TalkInteractionPrompt;
		case MechanismInteractionTarget: UseInteractionPrompt;
	};

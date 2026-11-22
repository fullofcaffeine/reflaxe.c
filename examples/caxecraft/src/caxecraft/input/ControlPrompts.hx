package caxecraft.input;

import caxecraft.localization.UiTypes.UiMessage;

/**
 * Selects stable localization keys for the most recently available input kind.
 *
 * Renderers receive this small value after platform input has been sampled.
 * They can therefore show controller-aware help without knowing Raylib button
 * constants or duplicating translated prose in Haxe. The runtime UI JSON
 * remains the only owner of every displayed sentence.
 */
enum abstract ControlPromptDevice(Int) {
	/** Keyboard and mouse are the available play controls. */
	var KeyboardMouse = 0;

	/** A gamepad is connected and can complete the current path. */
	var Gamepad = 1;
}

/** Select the complete gameplay-control summary shown on menu screens. */
function controlsMessage(device:ControlPromptDevice):UiMessage
	return device == Gamepad ? UiMessage.ControlsGamepad : UiMessage.Controls;

/** Select title and campaign navigation help. */
function menuInstructionsMessage(device:ControlPromptDevice):UiMessage
	return device == Gamepad ? UiMessage.MenuInstructionsGamepad : UiMessage.MenuInstructions;

/** Select the explicit resume action shown while the pointer is released. */
function capturePromptMessage(device:ControlPromptDevice):UiMessage
	return device == Gamepad ? UiMessage.CapturePromptGamepad : UiMessage.CapturePrompt;

/** Select pause-screen resume and quit help. */
function pauseHelpMessage(device:ControlPromptDevice):UiMessage
	return device == Gamepad ? UiMessage.PauseHelpGamepad : UiMessage.PauseHelp;

/** Select the recovery action shown after the player is defeated. */
function returnPromptMessage(device:ControlPromptDevice):UiMessage
	return device == Gamepad ? UiMessage.ReturnPromptGamepad : UiMessage.ReturnPrompt;

/** Select normal-action and deliberate-skip help for a conversation. */
function conversationHelpMessage(device:ControlPromptDevice):UiMessage
	return device == Gamepad ? UiMessage.ConversationHelpGamepad : UiMessage.ConversationHelp;

/**
 * Replace only an authored prompt's leading control token for gamepad display.
 *
 * CAXEMAP still owns the localized action phrase, such as “TALK TO NIA”. Its
 * existing first token is a locale-independent keyboard key followed by two
 * spaces. Controller presentation replaces that token with catalog-owned help;
 * malformed older content is preserved after a clear gamepad prefix.
 */
function gamepadInteractionPrompt(authored:String, gamepadControl:String):String {
	final separator = authored.indexOf("  ");
	if (separator < 0)
		return gamepadControl + "  " + authored;
	return gamepadControl + authored.substring(separator);
}

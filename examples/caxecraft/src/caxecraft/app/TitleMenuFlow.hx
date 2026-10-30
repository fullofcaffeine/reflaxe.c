package caxecraft.app;

import caxecraft.app.AppScreen.openEditor;
import caxecraft.app.AppScreen.openCampaignSelection;
import caxecraft.app.AppScreen.startPlaying;

/**
 * Applies device-independent title-menu choices to the selected game mode.
 *
 * Keyboard, mouse, controller, and deterministic pilots first describe what
 * the player meant with `TitleMenuCommand`. This module then changes the title
 * state once, so every input source launches Creative, Adventure, or the editor
 * through the same rule. Raylib remains responsible only for turning physical
 * keys and pointer positions into these commands.
 */
/** One complete title-menu action for the current rendered frame. */
enum abstract TitleMenuCommand(Int) {
	var None = 0;
	var CycleMode = 1;
	var SelectCreative = 2;
	var SelectAdventure = 3;
	var ConfirmMode = 4;
	var OpenEditor = 5;
}

/** The title screen and highlighted mode after one semantic menu action. */
typedef TitleMenuState = {
	final screen:AppScreen;
	final mode:GameMode;
}

/** Device-neutral inputs collected during one title-menu frame. */
typedef TitleMenuFrameInput = {
	final state:TitleMenuState;
	final campaignAvailable:Bool;
	final cycleMode:Bool;
	final confirmMode:Bool;

	/** Hovered pointer choice, or `-1` when no pointer choice is active. */
	final pointerChoice:Int;

	/** Whether the active pointer choice was pressed during this frame. */
	final pointerPressed:Bool;
}

/**
 * Apply one menu command without reading a keyboard, mouse, or window.
 *
 * Commands outside the title screen preserve the current state. This keeps a
 * late or repeated input from changing gameplay after the menu has closed.
 */
function applyTitleMenuCommand(state:TitleMenuState, command:TitleMenuCommand):TitleMenuState {
	if (state.screen != AppScreen.Title)
		return state;
	return switch command {
		case CycleMode:
			newState(state.screen, state.mode == GameMode.Creative ? GameMode.Adventure : GameMode.Creative);
		case SelectCreative:
			newState(state.screen, GameMode.Creative);
		case SelectAdventure:
			newState(state.screen, GameMode.Adventure);
		case ConfirmMode:
			newState(state.mode == GameMode.Adventure ? openCampaignSelection(state.screen) : startPlaying(state.screen), state.mode);
		case OpenEditor:
			newState(openEditor(state.screen), state.mode);
		case None:
			state;
	};
}

/** Build an immutable menu state without exposing a device-specific object. */
function titleMenuState(screen:AppScreen, mode:GameMode):TitleMenuState
	return newState(screen, mode);

/**
 * Advance all semantic title-menu choices collected during one frame.
 *
 * Device adapters supply only a hovered choice and button states. Keeping the
 * precedence here prevents keyboard, mouse, controller, and pilot paths from
 * rebuilding the same transition branches inside the application loop.
 */
function advanceTitleMenuFrame(input:TitleMenuFrameInput):TitleMenuState {
	var state = input.state;
	if (input.cycleMode)
		state = applyTitleMenuCommand(state, TitleMenuCommand.CycleMode);
	if (input.pointerChoice == 0)
		state = applyTitleMenuCommand(state, TitleMenuCommand.SelectCreative);
	if (input.pointerChoice == 1)
		state = applyTitleMenuCommand(state, TitleMenuCommand.SelectAdventure);
	if (input.pointerPressed && input.pointerChoice >= 0)
		state = applyTitleMenuCommand(state, input.pointerChoice == 2 ? TitleMenuCommand.OpenEditor : TitleMenuCommand.ConfirmMode);
	else if (input.confirmMode)
		state = applyTitleMenuCommand(state, TitleMenuCommand.ConfirmMode);
	if (!input.campaignAvailable && state.screen == AppScreen.CampaignSelect)
		state = newState(input.state.screen, state.mode);
	return state;
}

/**
 * Admit campaign travel only after the player has launched Adventure.
 *
 * A highlighted but unconfirmed Adventure choice remains on the title screen,
 * while Creative play keeps its world-editing contract and ignores campaign
 * exits. The campaign manifest separately decides whether a valid exit exists.
 */
function allowsCampaignTravel(screen:AppScreen, mode:GameMode):Bool
	return screen == AppScreen.Playing && mode == GameMode.Adventure;

private inline function newState(screen:AppScreen, mode:GameMode):TitleMenuState
	return {screen: screen, mode: mode};

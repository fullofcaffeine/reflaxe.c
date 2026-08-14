package caxecraft.app;

/**
 * The current top-level screen of the small playable application.
 *
 * This closed enum replaces loosely related Boolean values for title, pause,
 * editor, and pointer capture. Each variant describes one complete valid state:
 * the title, campaign-selection, pause, and editor screens stop simulation and
 * release the pointer, while playing advances simulation and captures it.
 * Keeping those rules here makes impossible combinations unrepresentable and
 * keeps Raylib device calls at the application edge.
 *
 * Loading is a real one-frame boundary between an authored campaign exit and
 * the checked level publication on the next outer frame. It pauses simulation
 * and releases the pointer without pretending that synchronous loading has a
 * measurable progress value. Settings and exit variants still belong here only
 * when their screens exist.
 */
enum AppScreen {
	Title;
	CampaignSelect;
	Playing;
	Paused;
	Loading;
	Editor;
	EditorTestPlay;
}

/** Choose the first valid screen without depending on a windowing library. */
function initialScreen(showTitle:Bool):AppScreen {
	if (showTitle)
		return Title;
	return Playing;
}

/** Report whether presentation should draw the title instead of the world. */
function showsTitle(screen:AppScreen):Bool {
	return switch screen {
		case Title: true;
		case CampaignSelect: false;
		case Playing: false;
		case Paused: false;
		case Loading: false;
		case Editor: false;
		case EditorTestPlay: false;
	};
}

/** Report whether Adventure is presenting its runtime-loaded campaign. */
function showsCampaignSelection(screen:AppScreen):Bool {
	return switch screen {
		case CampaignSelect: true;
		case Title | Playing | Paused | Loading | Editor | EditorTestPlay: false;
	};
}

/** Report whether a real campaign handoff owns this presentation frame. */
function showsLoading(screen:AppScreen):Bool {
	return switch screen {
		case Loading: true;
		case Title | CampaignSelect | Playing | Paused | Editor | EditorTestPlay: false;
	};
}

/** Report whether the visual editor, rather than the game world, owns the frame. */
function showsEditor(screen:AppScreen):Bool {
	return switch screen {
		case Title: false;
		case CampaignSelect: false;
		case Playing: false;
		case Paused: false;
		case Loading: false;
		case Editor: true;
		case EditorTestPlay: false;
	};
}

/** Report whether player input may control the simulated world. */
function isPlaying(screen:AppScreen):Bool {
	return switch screen {
		case Title: false;
		case CampaignSelect: false;
		case Playing: true;
		case Paused: false;
		case Loading: false;
		case Editor: false;
		case EditorTestPlay: true;
	};
}

/** Report whether the operating-system pointer belongs to camera control. */
function capturesPointer(screen:AppScreen):Bool {
	return switch screen {
		case Title: false;
		case CampaignSelect: false;
		case Playing: true;
		case Paused: false;
		case Loading: false;
		case Editor: false;
		case EditorTestPlay: true;
	};
}

/** Report whether fixed gameplay ticks must stop while frames may still draw. */
function pausesSimulation(screen:AppScreen):Bool {
	return switch screen {
		case Title: true;
		case CampaignSelect: true;
		case Playing: false;
		case Paused: true;
		case Loading: true;
		case Editor: true;
		case EditorTestPlay: false;
	};
}

/** Admit authored campaign exits only from the normal playing owner. */
function acceptsCampaignExit(screen:AppScreen):Bool
	return screen == Playing;

/** Stop native editor Test Play before effects after Escape or focus loss. */
function stopsEditorTestPlay(screen:AppScreen, focused:Bool, pausePressed:Bool):Bool
	return screen == EditorTestPlay && (!focused || pausePressed);

/** Start play from the title; repeated or misplaced requests change nothing. */
function startPlaying(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Playing;
		case CampaignSelect: CampaignSelect;
		case Playing: Playing;
		case Paused: Paused;
		case Loading: Loading;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Open the campaign picker only from the title's Adventure choice. */
function openCampaignSelection(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: CampaignSelect;
		case CampaignSelect: CampaignSelect;
		case Playing: Playing;
		case Paused: Paused;
		case Loading: Loading;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Launch the selected runtime campaign; other screens ignore the request. */
function startSelectedCampaign(screen:AppScreen):AppScreen {
	return switch screen {
		case CampaignSelect: Playing;
		case Title: Title;
		case Playing: Playing;
		case Paused: Paused;
		case Loading: Loading;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Cancel campaign selection without starting or mutating gameplay. */
function closeCampaignSelection(screen:AppScreen):AppScreen {
	return switch screen {
		case CampaignSelect: Title;
		case Title: Title;
		case Playing: Playing;
		case Paused: Paused;
		case Loading: Loading;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Pause active play after focus leaves the game window. */
function loseFocus(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Title;
		case CampaignSelect: CampaignSelect;
		case Playing: Paused;
		case Paused: Paused;
		case Loading: Loading;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Toggle between active play and pause without bypassing the title. */
function togglePause(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Title;
		case CampaignSelect: CampaignSelect;
		case Playing: Paused;
		case Paused: Playing;
		case Loading: Loading;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Resume a paused game when the player explicitly recaptures the pointer. */
function recapture(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Title;
		case CampaignSelect: CampaignSelect;
		case Playing: Playing;
		case Paused: Playing;
		case Loading: Loading;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Start a campaign handoff only from active play. */
function beginLoading(screen:AppScreen):AppScreen {
	return switch screen {
		case Playing: Loading;
		case Title: Title;
		case CampaignSelect: CampaignSelect;
		case Paused: Paused;
		case Loading: Loading;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Resume active play after either a successful or rejected handoff. */
function finishLoading(screen:AppScreen):AppScreen {
	return switch screen {
		case Loading: Playing;
		case Title: Title;
		case CampaignSelect: CampaignSelect;
		case Playing: Playing;
		case Paused: Paused;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Enter the editor only from the title screen, where the pointer is free. */
function openEditor(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Editor;
		case CampaignSelect: CampaignSelect;
		case Playing: Playing;
		case Paused: Paused;
		case Loading: Loading;
		case Editor: Editor;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Return from the editor to the title without starting or resuming gameplay. */
function closeEditor(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Title;
		case CampaignSelect: CampaignSelect;
		case Playing: Playing;
		case Paused: Paused;
		case Loading: Loading;
		case Editor: Title;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Start real-engine Test Play only from the editor workspace. */
function beginEditorTestPlay(screen:AppScreen):AppScreen {
	return switch screen {
		case Editor: EditorTestPlay;
		case Title: Title;
		case CampaignSelect: CampaignSelect;
		case Playing: Playing;
		case Paused: Paused;
		case Loading: Loading;
		case EditorTestPlay: EditorTestPlay;
	};
}

/** Stop Test Play and restore the editor workspace that started it. */
function finishEditorTestPlay(screen:AppScreen):AppScreen {
	return switch screen {
		case EditorTestPlay: Editor;
		case Title: Title;
		case CampaignSelect: CampaignSelect;
		case Playing: Playing;
		case Paused: Paused;
		case Loading: Loading;
		case Editor: Editor;
	};
}

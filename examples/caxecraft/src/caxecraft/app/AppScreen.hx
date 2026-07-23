package caxecraft.app;

/**
 * The current top-level screen of the small playable application.
 *
 * This closed enum replaces loosely related Boolean values for title, pause,
 * editor, and pointer capture. Each variant describes one complete valid state:
 * the title, pause, and editor screens stop simulation and release the pointer,
 * while playing advances simulation and captures it. Keeping those rules here makes
 * impossible combinations unrepresentable and keeps Raylib device calls at the
 * application edge.
 *
 * The editor variant became real with the first RayguiHx shell. Loading,
 * settings, and exit variants still belong here only when their screens exist.
 */
enum AppScreen {
	Title;
	Playing;
	Paused;
	Editor;
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
		case Playing: false;
		case Paused: false;
		case Editor: false;
	};
}

/** Report whether the visual editor, rather than the game world, owns the frame. */
function showsEditor(screen:AppScreen):Bool {
	return switch screen {
		case Title: false;
		case Playing: false;
		case Paused: false;
		case Editor: true;
	};
}

/** Report whether player input may control the simulated world. */
function isPlaying(screen:AppScreen):Bool {
	return switch screen {
		case Title: false;
		case Playing: true;
		case Paused: false;
		case Editor: false;
	};
}

/** Report whether the operating-system pointer belongs to camera control. */
function capturesPointer(screen:AppScreen):Bool {
	return switch screen {
		case Title: false;
		case Playing: true;
		case Paused: false;
		case Editor: false;
	};
}

/** Report whether fixed gameplay ticks must stop while frames may still draw. */
function pausesSimulation(screen:AppScreen):Bool {
	return switch screen {
		case Title: true;
		case Playing: false;
		case Paused: true;
		case Editor: true;
	};
}

/** Start play from the title; repeated or misplaced requests change nothing. */
function startPlaying(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Playing;
		case Playing: Playing;
		case Paused: Paused;
		case Editor: Editor;
	};
}

/** Pause active play after focus leaves the game window. */
function loseFocus(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Title;
		case Playing: Paused;
		case Paused: Paused;
		case Editor: Editor;
	};
}

/** Toggle between active play and pause without bypassing the title. */
function togglePause(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Title;
		case Playing: Paused;
		case Paused: Playing;
		case Editor: Editor;
	};
}

/** Resume a paused game when the player explicitly recaptures the pointer. */
function recapture(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Title;
		case Playing: Playing;
		case Paused: Playing;
		case Editor: Editor;
	};
}

/** Enter the editor only from the title screen, where the pointer is free. */
function openEditor(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Editor;
		case Playing: Playing;
		case Paused: Paused;
		case Editor: Editor;
	};
}

/** Return from the editor to the title without starting or resuming gameplay. */
function closeEditor(screen:AppScreen):AppScreen {
	return switch screen {
		case Title: Title;
		case Playing: Playing;
		case Paused: Paused;
		case Editor: Title;
	};
}

package caxecraft.qa;

import caxecraft.app.AppScreen;
import caxecraft.app.AppScreen.capturesPointer;
import caxecraft.app.AppScreen.initialScreen;
import caxecraft.app.AppScreen.isPlaying;
import caxecraft.app.AppScreen.loseFocus;
import caxecraft.app.AppScreen.openEditor;
import caxecraft.app.AppScreen.pausesSimulation;
import caxecraft.app.AppScreen.recapture;
import caxecraft.app.AppScreen.closeEditor;
import caxecraft.app.AppScreen.showsEditor;
import caxecraft.app.AppScreen.showsTitle;
import caxecraft.app.AppScreen.startPlaying;
import caxecraft.app.AppScreen.togglePause;

/**
 * Cross-target executable specification for the shipped application screens.
 *
 * The same assertions run quickly on Eval and through generated native C. The
 * graphical pause pilot then checks that the Raylib edge performs the cursor
 * and drawing effects requested by these target-neutral transitions.
 */
var observed:Int = 0;

function main():Void {
	#if c
	observed = selfCheck();
	#else
	Sys.println(selfCheck());
	#end
}

/** Return zero, or the stable number of the first broken screen invariant. */
function selfCheck():Int {
	final title = initialScreen(true);
	if (!showsTitle(title) || !pausesSimulation(title) || capturesPointer(title))
		return 1;
	if (togglePause(title) != AppScreen.Title || recapture(title) != AppScreen.Title)
		return 2;

	final playing = startPlaying(title);
	if (!isPlaying(playing) || !capturesPointer(playing) || pausesSimulation(playing))
		return 3;
	if (startPlaying(playing) != AppScreen.Playing || recapture(playing) != AppScreen.Playing)
		return 4;

	final pausedByFocus = loseFocus(playing);
	if (pausedByFocus != AppScreen.Paused || !pausesSimulation(pausedByFocus) || capturesPointer(pausedByFocus))
		return 5;
	if (togglePause(pausedByFocus) != AppScreen.Playing || recapture(pausedByFocus) != AppScreen.Playing)
		return 6;
	if (togglePause(playing) != AppScreen.Paused)
		return 7;
	if (initialScreen(false) != AppScreen.Playing)
		return 8;
	final editor = openEditor(title);
	if (!showsEditor(editor) || !pausesSimulation(editor) || capturesPointer(editor) || isPlaying(editor))
		return 9;
	if (togglePause(editor) != AppScreen.Editor || recapture(editor) != AppScreen.Editor || loseFocus(editor) != AppScreen.Editor)
		return 10;
	if (startPlaying(editor) != AppScreen.Editor || closeEditor(editor) != AppScreen.Title)
		return 11;
	if (openEditor(playing) != AppScreen.Playing || closeEditor(playing) != AppScreen.Playing)
		return 12;
	return 0;
}

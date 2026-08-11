package caxecraft.qa;

import caxecraft.app.AppScreen;
import caxecraft.app.AppScreen.beginLoading;
import caxecraft.app.AppScreen.capturesPointer;
import caxecraft.app.AppScreen.finishLoading;
import caxecraft.app.AppScreen.initialScreen;
import caxecraft.app.AppScreen.isPlaying;
import caxecraft.app.AppScreen.loseFocus;
import caxecraft.app.AppScreen.closeCampaignSelection;
import caxecraft.app.AppScreen.openCampaignSelection;
import caxecraft.app.AppScreen.openEditor;
import caxecraft.app.AppScreen.showsCampaignSelection;
import caxecraft.app.AppScreen.startSelectedCampaign;
import caxecraft.app.AppScreen.pausesSimulation;
import caxecraft.app.AppScreen.recapture;
import caxecraft.app.AppScreen.closeEditor;
import caxecraft.app.AppScreen.showsEditor;
import caxecraft.app.AppScreen.showsLoading;
import caxecraft.app.AppScreen.showsTitle;
import caxecraft.app.AppScreen.startPlaying;
import caxecraft.app.AppScreen.togglePause;
import caxecraft.app.GameMode;
import caxecraft.app.SpawnCameraHeading.headingForSpawn;
import caxecraft.app.TitleMenuFlow.TitleMenuCommand;
import caxecraft.app.TitleMenuFlow.allowsCampaignTravel;
import caxecraft.app.TitleMenuFlow.applyTitleMenuCommand;
import caxecraft.app.TitleMenuFlow.titleMenuState;

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
	final initialMenu = titleMenuState(title, GameMode.Creative);
	final adventureSelected = applyTitleMenuCommand(initialMenu, TitleMenuCommand.CycleMode);
	if (adventureSelected.screen != AppScreen.Title || adventureSelected.mode != GameMode.Adventure)
		return 13;
	if (allowsCampaignTravel(AppScreen.Playing, GameMode.Creative)
		|| allowsCampaignTravel(adventureSelected.screen, adventureSelected.mode))
		return 14;
	final adventureStarted = applyTitleMenuCommand(adventureSelected, TitleMenuCommand.ConfirmMode);
	if (!showsCampaignSelection(adventureStarted.screen) || adventureStarted.mode != GameMode.Adventure)
		return 15;
	if (allowsCampaignTravel(adventureStarted.screen, adventureStarted.mode))
		return 16;
	final campaignStarted = startSelectedCampaign(adventureStarted.screen);
	if (campaignStarted != AppScreen.Playing || !allowsCampaignTravel(campaignStarted, adventureStarted.mode))
		return 17;
	if (closeCampaignSelection(adventureStarted.screen) != AppScreen.Title
		|| startSelectedCampaign(AppScreen.Title) != AppScreen.Title
		|| openCampaignSelection(AppScreen.Playing) != AppScreen.Playing)
		return 18;
	final lateMenuInput = applyTitleMenuCommand(adventureStarted, TitleMenuCommand.CycleMode);
	if (lateMenuInput.screen != AppScreen.CampaignSelect || lateMenuInput.mode != GameMode.Adventure)
		return 19;
	final editorFromTitle = applyTitleMenuCommand(initialMenu, TitleMenuCommand.OpenEditor);
	if (editorFromTitle.screen != AppScreen.Editor || editorFromTitle.mode != GameMode.Creative)
		return 20;
	final loading = beginLoading(playing);
	if (loading != AppScreen.Loading || !showsLoading(loading) || !pausesSimulation(loading) || capturesPointer(loading) || isPlaying(loading))
		return 21;
	if (beginLoading(title) != AppScreen.Title || togglePause(loading) != AppScreen.Loading || recapture(loading) != AppScreen.Loading)
		return 22;
	if (finishLoading(loading) != AppScreen.Playing || finishLoading(title) != AppScreen.Title)
		return 23;
	final east = headingForSpawn({
		xMilli: 5500,
		yMilli: 5000,
		zMilli: 16500,
		yawDegrees: 90
	});
	if (east.x < 0.999 || east.x > 1.001 || east.y != -0.18 || east.z < -0.001 || east.z > 0.001)
		return 24;
	final north = headingForSpawn({
		xMilli: 0,
		yMilli: 0,
		zMilli: 0,
		yawDegrees: 0
	});
	if (north.x < -0.001 || north.x > 0.001 || north.z < -1.001 || north.z > -0.999)
		return 25;
	final west = headingForSpawn({
		xMilli: 0,
		yMilli: 0,
		zMilli: 0,
		yawDegrees: 270
	});
	if (west.x < -1.001 || west.x > -0.999 || west.z < -0.001 || west.z > 0.001)
		return 26;
	final south = headingForSpawn({
		xMilli: 0,
		yMilli: 0,
		zMilli: 0,
		yawDegrees: 180
	});
	if (south.x < -0.001 || south.x > 0.001 || south.z < 0.999 || south.z > 1.001)
		return 27;
	return 0;
}

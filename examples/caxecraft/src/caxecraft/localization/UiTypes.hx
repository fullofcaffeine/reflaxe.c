package caxecraft.localization;

/**
	Closed identities shared by runtime UI data and its typed Haxe consumers.

	The JSON decoder verifies that its locale and message order matches these
	identities before publishing a catalog. This module owns no player-visible
	text, filesystem path, rendering behavior, or package selection.
**/
/** Locale position inside one admitted runtime catalog. */
enum abstract LocaleCursor(Int) {
	var Locale0 = 0;
	var Locale1 = 1;
}

/** Stable message identity understood by the current application surface. */
enum abstract UiMessage(Int) {
	var AquaticGearEquipped = 0;
	var Brand = 1;
	var CapturePrompt = 2;
	var Controls = 3;
	var DebugCells = 4;
	var DebugDraws = 5;
	var DebugFrame = 6;
	var DebugTick = 7;
	var DebugVisible = 8;
	var EditorAdvanced = 9;
	var EditorBack = 10;
	var EditorCanvasHelp = 11;
	var EditorInvalid = 12;
	var EditorName = 13;
	var EditorNewWorld = 14;
	var EditorReady = 15;
	var EditorRedo = 16;
	var EditorStopTest = 17;
	var EditorTest = 18;
	var EditorTesting = 19;
	var EditorTitle = 20;
	var EditorToolList = 21;
	var EditorUndo = 22;
	var EditorValid = 23;
	var EditorValidate = 24;
	var HealthFull = 25;
	var MenuAdventure = 26;
	var MenuCreative = 27;
	var MenuEditor = 28;
	var MenuInstructions = 29;
	var NoBlockInReach = 30;
	var PauseHelp = 31;
	var PauseTitle = 32;
	var PlaceBlocked = 33;
	var TitleFallback = 34;

	/** Appended so existing message codes remain stable. Catalog storage stays ID-sorted. */
	var EditorScene = 35;

	var EditorBuild = 36;
	var EditorCoordinates = 37;
	var EditorErase = 38;
	var EditorGround = 39;
	var EditorKeepEditing = 40;
	var EditorLeaveWithoutSaving = 41;
	var EditorMaterial = 42;
	var EditorMoreDetails = 43;
	var EditorPlan = 44;
	var EditorSelect = 45;
	var EditorUnsavedChanges = 46;
	var EditorWorldList = 47;
}

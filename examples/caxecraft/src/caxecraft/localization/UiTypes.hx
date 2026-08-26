package caxecraft.localization;

/**
	Closed identities shared by runtime UI data and its typed Haxe consumers.

	Each message value is only a stable lookup key used by a real call site. The
	JSON catalog independently owns membership, canonical order, and translated
	prose, so adding or reordering data never requires a parallel code table. This
	module owns no player-visible text, filesystem path, rendering behavior, or
	package selection.
**/
/** Locale position inside one admitted runtime catalog. */
enum abstract LocaleCursor(Int) {
	var Locale0 = 0;
	var Locale1 = 1;
}

/** Stable catalog key understood by one typed application call site. */
enum abstract UiMessage(String) {
	var AquaticGearEquipped = "aquatic_gear_equipped";
	var CapturePrompt = "capture_prompt";
	var DebugDraws = "debug_draws";
	var DebugFrame = "debug_frame";
	var DebugTick = "debug_tick";
	var DebugVisible = "debug_visible";
	var EditorBack = "editor_back";
	var EditorAssetBrowser = "editor_asset_browser";
	var EditorAssetCategoryEnemy = "editor_asset_category_enemy";
	var EditorAssetCategoryItem = "editor_asset_category_item";
	var EditorAssetCategoryMechanism = "editor_asset_category_mechanism";
	var EditorAssetCategoryNpc = "editor_asset_category_npc";
	var EditorAssetCategoryTerrain = "editor_asset_category_terrain";
	var EditorAssetClose = "editor_asset_close";
	var EditorAssetEmpty = "editor_asset_empty";
	var EditorAssetSearch = "editor_asset_search";
	var EditorAssetShortcut = "editor_asset_shortcut";
	var EditorCanvasHelp = "editor_canvas_help";
	var EditorInvalid = "editor_invalid";
	var EditorReady = "editor_ready";
	var EditorRedo = "editor_redo";
	var EditorTest = "editor_test";
	var EditorTesting = "editor_testing";
	var EditorTitle = "editor_title";
	var EditorUndo = "editor_undo";
	var EditorValid = "editor_valid";
	var HealthFull = "health_full";
	var MenuAdventure = "menu_adventure";
	var MenuCreative = "menu_creative";
	var MenuEditor = "menu_editor";
	var MenuInstructions = "menu_instructions";
	var NoBlockInReach = "no_block_in_reach";
	var PauseHelp = "pause_help";
	var PauseTitle = "pause_title";
	var PlaceBlocked = "place_blocked";
	var TitleFallback = "title_fallback";
	var EditorBuild = "editor_build";
	var EditorCoordinates = "editor_coordinates";
	var EditorErase = "editor_erase";
	var EditorGround = "editor_ground";
	var EditorKeepEditing = "editor_keep_editing";
	var EditorLeaveWithoutSaving = "editor_leave_without_saving";
	var EditorMaterial = "editor_material";
	var EditorMoreDetails = "editor_more_details";
	var EditorPlan = "editor_plan";
	var EditorSelect = "editor_select";
	var EditorUnsavedChanges = "editor_unsaved_changes";
	var EditorWorldList = "editor_world_list";
	var EditorCheckpoint = "editor_checkpoint";
	var EditorDelete = "editor_delete";
	var EditorDuplicate = "editor_duplicate";
	var EditorEnvironment = "editor_environment";
	var EditorEnvironmentClouds = "editor_environment_clouds";
	var EditorEnvironmentDone = "editor_environment_done";
	var EditorEnvironmentEast = "editor_environment_east";
	var EditorEnvironmentEnabled = "editor_environment_enabled";
	var EditorEnvironmentNorth = "editor_environment_north";
	var EditorEnvironmentOff = "editor_environment_off";
	var EditorEnvironmentOn = "editor_environment_on";
	var EditorEnvironmentRadius = "editor_environment_radius";
	var EditorEnvironmentSeed = "editor_environment_seed";
	var EditorEnvironmentSky = "editor_environment_sky";
	var EditorEnvironmentSouth = "editor_environment_south";
	var EditorEnvironmentSun = "editor_environment_sun";
	var EditorEnvironmentWater = "editor_environment_water";
	var EditorEnvironmentWest = "editor_environment_west";
	var EditorSave = "editor_save";
	var EditorSaveFailed = "editor_save_failed";
	var EditorSaved = "editor_saved";
	var EditorLayer = "editor_layer";
	var EditorTrigger = "editor_trigger";
	var ConversationHelp = "conversation_help";
	var ConversationNarrator = "conversation_narrator";
	var ReturnPrompt = "return_prompt";
	var PlayerFallen = "player_fallen";
	var EditorCamera = "editor_camera";
	var EditorCameraWalk = "editor_camera_walk";
	var EditorCameraFly = "editor_camera_fly";
	var EditorCameraOrbit = "editor_camera_orbit";

	/** Expose the stable data key without a cast or parallel ordinal table. */
	public inline function text():String
		return this;
}

package caxecraft.app;

#if c
import caxecraft.content.RuntimeContentPack.RuntimeItemUseProfile;
import caxecraft.content.ActiveRuntimeContent;
import caxecraft.app.ActivePlayableLevel.PlayableLevelCreationResult;
import caxecraft.app.ActivePlayableLevel.PlayableLevelPublicationResult;
import caxecraft.app.ActivePlayableLevel.PlayableLevelView;
import caxecraft.content.CampaignManifest.CampaignLevel;
import caxecraft.content.CampaignManifest.CampaignManifest;
import caxecraft.content.CampaignRuntime.CampaignLevelLoadError;
import caxecraft.content.CampaignRuntime.CampaignLevelLoadResult;
import caxecraft.content.CampaignRuntime.CampaignPackageLoadError;
import caxecraft.content.CampaignRuntime.CampaignPackageLoadResult;
import caxecraft.content.CampaignRuntime.loadCampaignLevel;
import caxecraft.content.CampaignRuntime.loadCampaignManifest;
import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.ResolvedLevelPlan.LevelPlayerOptions;
import caxecraft.content.RuntimeContentGeneration.RuntimeContentLoadResult;
import caxecraft.content.RuntimeContentGeneration.loadRuntimeContent;
import caxecraft.app.AppScreen;
import caxecraft.app.AppScreen.beginLoading;
import caxecraft.app.AppScreen.capturesPointer as screenCapturesPointer;
import caxecraft.app.AppScreen.closeCampaignSelection;
import caxecraft.app.AppScreen.closeEditor;
import caxecraft.app.AppScreen.initialScreen;
import caxecraft.app.AppScreen.finishLoading;
import caxecraft.app.AppScreen.isPlaying as screenIsPlaying;
import caxecraft.app.AppScreen.loseFocus as pauseAfterFocusLoss;
import caxecraft.app.AppScreen.openEditor;
import caxecraft.app.AppScreen.pausesSimulation as screenPausesSimulation;
import caxecraft.app.AppScreen.recapture as recapturePointer;
import caxecraft.app.AppScreen.showsEditor as screenShowsEditor;
import caxecraft.app.AppScreen.showsLoading as screenShowsLoading;
import caxecraft.app.AppScreen.showsCampaignSelection as screenShowsCampaignSelection;
import caxecraft.app.AppScreen.showsTitle as screenShowsTitle;
import caxecraft.app.AppScreen.startSelectedCampaign;
import caxecraft.app.AppScreen.togglePause;
import caxecraft.app.TitleMenuFlow.TitleMenuCommand;
import caxecraft.app.TitleMenuFlow.allowsCampaignTravel;
import caxecraft.app.TitleMenuFlow.applyTitleMenuCommand;
import caxecraft.app.TitleMenuFlow.titleMenuState;
import caxecraft.app.CampaignMenu.CampaignMenuHit;
import caxecraft.app.CaxecraftEditorScreen.EditorScreenAction;
import caxecraft.app.MotionInterpolation.advance as advanceMotion;
import caxecraft.app.MotionInterpolation.reset as resetMotion;
import caxecraft.app.MotionInterpolation.sample as sampleMotion;
import caxecraft.app.MotionInterpolation.start as startMotion;
import caxecraft.app.RuntimeInventoryBinding.inventoryKindForRuntimeItem;
import caxecraft.app.SpawnCameraHeading.headingForSpawn;
import caxecraft.app.StatefulObjectRenderer.drawStatefulObjects;
import caxecraft.app.InteractionPrompt.InteractionPrompt;
import caxecraft.app.InteractionPrompt.InteractionTargetKind;
import caxecraft.app.InteractionPrompt.interactionPrompt;
import caxecraft.domain.CharacterDamagePolicy;
import caxecraft.domain.Character;
import caxecraft.domain.ActorControllerEvent;
import caxecraft.domain.ActorControllerPhase;
import caxecraft.domain.ActorControllerState;
import caxecraft.domain.ActorControllerTick.ActorControllerTickStatus;
import caxecraft.domain.AquaticMedium;
import caxecraft.domain.EntityId;
import caxecraft.domain.GameSession;
import caxecraft.domain.Aquatics.canMine as playerCanMine;
import caxecraft.domain.Aquatics.input as aquaticInput;
import caxecraft.domain.CharacterPhysics.canPlaceAt as playerCanPlaceAt;
import caxecraft.domain.CharacterPhysics.body as createPlayer;
import caxecraft.domain.CharacterPhysics.recoverSpawn as recoverPlayerSpawn;
import caxecraft.domain.CharacterBody;
import caxecraft.domain.RaycastHit;
import caxecraft.domain.VoxelRaycast;
import caxecraft.domain.World;
import caxecraft.domain.WorldView;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.CaxeFlowRuntime.FlowPresentationEvent;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.InventoryFullReason;
import caxecraft.gameplay.InventoryState;
import caxecraft.app.CaxecraftAtlas.HotbarFrame;
import caxecraft.app.CaxecraftAtlas.HudGlyph;
import caxecraft.gameplay.BerryDrop.collectAmount as collectBerryDropAmount;
import caxecraft.gameplay.BerryDrop.fromDefeatedCharacter as berryDropFromDefeatedCharacter;
import caxecraft.gameplay.BerryDrop.isInRange as berryDropIsInRange;
import caxecraft.gameplay.BerryDrop.none as emptyBerryDrop;
import caxecraft.gameplay.BerryDropState;
import caxecraft.gameplay.ItemKind;
import caxecraft.gameplay.MiningOutcome;
import caxecraft.gameplay.SwordCombat.after as afterSwordCombat;
import caxecraft.gameplay.SwordCombat.decide as decideSwordCombat;
import caxecraft.gameplay.SwordCombat.start as startSwordCombat;
import caxecraft.gameplay.SwordCombat.step as stepSwordCombat;
import caxecraft.domain.VitalsState;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.domain.Vitals.isDefeated as characterIsDefeated;
import caxecraft.gameplay.RecoveryDecision;
import caxecraft.gameplay.SwordCombat;
import caxecraft.gameplay.SwordCombatDecision;
import caxecraft.gameplay.SwordCombatState;
import caxecraft.gameplay.WorldItemPickup.isInRange as authoredItemIsInRange;
import caxecraft.app.GameplayMessage.gameplayMessageId;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import caxecraft.input.NavigationInput.NavigationCommand;
import caxecraft.input.NavigationInput.NavigationRepeater;
import caxecraft.input.NavigationInput.NavigationSample;
import caxecraft.app.RaylibNavigationInput.samplePrimaryGamepad;
#if caxecraft_pilot
import caxecraft.app.PilotTelemetry.drawPilotTelemetry;
import caxecraft.editor.EditorViewport.EditorTool;
import raylib.Rlgl;
#end
import caxecraft.pilot.GameInputFrame;
import caxecraft.pilot.PilotScript;
import caxecraft.pilot.PilotScript.PilotAction;
import caxecraft.pilot.PilotScript.PilotScriptName;
#if caxecraft_pilot_runtime
import caxecraft.app.AgentWorldProjection.agentInventory;
import caxecraft.app.AgentWorldProjection.agentNearby;
import caxecraft.app.AgentWorldProjection.agentTarget;
import caxecraft.app.AgentWorldProjection.agentTerrain;
import caxecraft.pilot.AgentWorldObservation.renderAgentWorldObservation;
import caxecraft.pilot.RuntimePilotScript;
import caxecraft.pilot.RuntimePilotScript.RuntimePilotReadResult;
import caxecraft.pilot.RuntimePilotScript.RuntimePilotRunResult;
import haxe.io.Bytes;
#end
import raylib.Camera3D;
import raylib.CameraProjection;
import raylib.Color;
import raylib.ConfigFlags;
import raylib.KeyboardKey;
import raylib.MouseButton;
import raylib.Raylib;
import raylib.Texture2D;
import raylib.Vector3;

/** One read-only controller phase lookup used to validate presentation input. */
private typedef ActorPhaseObservation = {
	final valid:Bool;
	final phase:ActorControllerPhase;
}

/** One nearest semantic interaction selected without campaign-specific IDs. */
private enum AvailableInteractionTarget {
	/** No authored interaction is currently valid. */
	NoAvailableInteraction;

	/** A dialogue actor owns the nearest valid interaction. */
	DialogueInteraction(id:EntityId);

	/** A generic stateful object owns the nearest valid interaction. */
	StatefulObjectInteraction(id:ScenarioId);
}

/**
 * Runs Caxecraft's current Raylib application and game loop.
 *
 * One outer loop follows the display: it polls window/input state and presents
 * one frame whenever Raylib is ready. An inner accumulator loop advances
 * gameplay in exact 50 ms steps. Keeping simulation time fixed makes collision,
 * water, combat, saves, Eval comparisons, and native pilots deterministic even
 * when rendering speeds up or stalls. These are two timing layers inside one
 * game loop, not two independent games.
 *
 * `GameSession` owns one fixed update and `AppScreen` owns the valid
 * title/play/pause transitions. This class is the stateful shell around those
 * target-neutral parts: it owns Raylib startup, resource lifetime, input,
 * screen transitions, fixed-step scheduling, drawing, and shutdown.
 *
 * The run method still contains presentation and first-playable migration
 * seams. Those are visible follow-up work, not a reason to leave platform
 * lifetime in the executable entry class or to duplicate this loop.
 */
final class CaxecraftApp {
	static inline final FIXED_SECONDS:Float = 0.05;
	static inline final MAX_FRAME_SECONDS:Float = 0.25;
	static inline final PICK_DISTANCE:Float = 7.0;

	/** Persistent terrain faces rebuilt only after successful world edits. */
	final terrainRenderer:TerrainRenderer = new TerrainRenderer();

	/** Persistent transparent-face storage reused by each water frame. */
	final waterRenderer:WaterRenderer = new WaterRenderer();

	/** Held-direction clock shared by real controller input and native pilots. */
	final editorNavigation:NavigationRepeater;

	/**
	 * Create the application shell before selecting one complete content generation.
	 *
	 * Native presentation resources are acquired later by `run`, after package
	 * bytes have produced a complete candidate session.
	 */
	public function new() {
		editorNavigation = new NavigationRepeater();
	}

	/**
	 * Run one complete native application lifetime.
	 *
	 * A successful call opens the window, loads presentation resources, services
	 * frames until an exit is requested, releases every valid resource, and then
	 * closes the window. If level validation or window creation fails, it returns
	 * before publishing a partial live application.
	 */
	public function run():Void {
		#if caxecraft_pilot_secondary_locale
		final pilotName:PilotScriptName = PilotScriptName.LaunchSmoke;
		#elseif caxecraft_pilot_launch_smoke
		final pilotName:PilotScriptName = PilotScriptName.LaunchSmoke;
		#elseif caxecraft_pilot_move_jump_edit
		final pilotName:PilotScriptName = PilotScriptName.MoveJumpEdit;
		#elseif caxecraft_pilot_pause_recapture
		final pilotName:PilotScriptName = PilotScriptName.PauseRecapture;
		#elseif caxecraft_pilot_combat_drop
		final pilotName:PilotScriptName = PilotScriptName.CombatDrop;
		#elseif caxecraft_pilot_recovery_use
		final pilotName:PilotScriptName = PilotScriptName.RecoveryUse;
		#elseif caxecraft_pilot_full_inventory_mining
		final pilotName:PilotScriptName = PilotScriptName.FullInventoryMining;
		#elseif caxecraft_pilot_resize_layout
		final pilotName:PilotScriptName = PilotScriptName.ResizeLayout;
		#elseif caxecraft_pilot_aquatic_gear
		final pilotName:PilotScriptName = PilotScriptName.AquaticGear;
		#elseif caxecraft_pilot_smooth_motion
		final pilotName:PilotScriptName = PilotScriptName.SmoothMotion;
		#elseif caxecraft_pilot_editor_shell
		final pilotName:PilotScriptName = PilotScriptName.EditorShell;
		#elseif caxecraft_pilot_campaign_travel
		final pilotName:PilotScriptName = PilotScriptName.CampaignTravel;
		#elseif caxecraft_pilot_runtime
		final pilotName:PilotScriptName = PilotScriptName.RuntimeContentJourney;
		#end
		var initialHealth = MAX_HEALTH;
		#if caxecraft_pilot
		// Only the deterministic provider may select fixture state. The release
		// build contains neither this branch nor a way to alter starting health.
		initialHealth = PilotScript.initialHealth(pilotName);
		#end

		// The executable runs from its distribution directory. `content` is the
		// application-selected capability root staged beside it by play.py.
		final contentStore = switch ContentPackageStore.open("content", "staged-content", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
			case PackageStoreOpened(store): store;
			case PackageStoreRejected(_):
				Sys.println("caxecraft: runtime content root rejected");
				return;
		};
		#if caxecraft_pilot_runtime
		// The launcher stages the selected content journey at one generic package
		// path. Haxe owns parsing and semantics; the host never interprets records.
		var runtimePilot = switch contentStore.read("pilots/active.piloscript") {
			case PackageBytesRejected(_):
				Sys.println("caxecraft: runtime Piloscript read failed: pilots/active.piloscript");
				return;
			case PackageBytesRead(content):
				switch RuntimePilotScript.read(content.bytes, content.provenance.logicalPath.text()) {
					case RuntimePilotRejected(diagnostic):
						Sys.println('caxecraft: runtime Piloscript rejected at ${diagnostic.source}:${diagnostic.line}: ${diagnostic.message}');
						return;
					case RuntimePilotReady(script): script;
				}
		};
		var pilotInputHash = runtimePilot.inputHash();
		final agentSession = StringTools.startsWith(runtimePilot.stableName(), "agent-session-");
		var runtimePilotFrame = 0;
		var agentWaiting = false;
		var lastAgentRequestBytesHash = 0;
		var agentObservationSequence = 0;
		#elseif caxecraft_pilot
		final pilotInputHash = PilotScript.inputHash(pilotName);
		#end
		final completeCandidate = switch loadRuntimeContent(contentStore, ContentGenerationId.fromSequence(1), {
			entityId: EntityId.fromValidatedStorageCode(1),
			initialHealth: initialHealth
		}) {
			case RuntimeContentReady(candidate): candidate;
			case RuntimeContentRejected(_):
				Sys.println("caxecraft: runtime content rejected");
				return;
		};
		final activeRuntimeContent = new ActiveRuntimeContent(completeCandidate);
		// Borrow one complete snapshot so the registry, catalog, level, and receipt
		// cannot come from different publications during this application run.
		final runtimeContent = activeRuntimeContent.generation();
		final contentRegistry = runtimeContent.registry();
		final uiCatalog = runtimeContent.catalog();
		final editorScreen = new CaxecraftEditorScreen(contentRegistry, uiCatalog);
		final loadedCandidate = runtimeContent.level();
		final activeLevel = switch ActivePlayableLevel.create(loadedCandidate) {
			case PlayableLevelCreated(value): value;
			case PlayableLevelCreationRejected(_):
				Sys.println("caxecraft: initial level lacks required playable bindings");
				return;
		};
		var campaign:Null<CampaignManifest> = null;
		var campaignLevel:Null<CampaignLevel> = null;
		var pendingCampaignLevel:Null<CampaignLevel> = null;
		var pendingCampaignLabel = "";
		var loadingFramePresented = false;
		switch loadCampaignManifest(contentStore, "campaigns/first-adventure/campaign.json") {
			case CampaignPackageReady(manifest):
				final entry = manifest.entryLevel();
				if (entry.logicalPath != loadedCandidate.receipt().logicalPath) {
					Sys.println("caxecraft: campaign entry does not match the active map");
					return;
				}
				campaign = manifest;
				campaignLevel = entry;
			case CampaignPackageRejected(CampaignManifestSourceRejected(EntryMissing)):
				// A standalone Creative map needs no campaign wrapper. It keeps the
				// exact same runtime generation and simply has no travel action.
				Sys.println("caxecraft: campaign-source=standalone-map");
			case CampaignPackageRejected(_):
				Sys.println("caxecraft: campaign manifest rejected: campaigns/first-adventure/campaign.json");
				return;
		}
		var levelLabel = campaignLevel == null ? loadedCandidate.receipt().logicalPath : campaignLevel.id.text();
		var selectedCampaignLevelIndex = 0;
		if (campaign != null && campaignLevel != null)
			for (index in 0...campaign.levelCount())
				if (campaign.levelAt(index).id.text() == campaignLevel.id.text())
					selectedCampaignLevelIndex = index;
		final initialLevel = activeLevel.level();
		final initialSession = activeLevel.session();
		final receipt = loadedCandidate.receipt();
		Sys.println("caxecraft: content-source=runtime-package");
		Sys.println("caxecraft: content-path=" + receipt.logicalPath);
		Sys.println("caxecraft: content-input-hash=" + Std.string(receipt.inputHash));
		Sys.println("caxecraft: content-generation=" + Std.string(runtimeContent.generationId().value()));
		var windowFlags = ConfigFlags.VsyncHint | ConfigFlags.WindowResizable;
		#if !raylib_platform_macos
		windowFlags |= ConfigFlags.WindowHighDpi;
		#else
		// The pinned Raylib 6.0 macOS path applies Retina scaling twice for this
		// terminal-launched window: a 1280x720 request becomes a 5120x2880
		// framebuffer whose useful image occupies one quadrant. Keep the logical
		// pixel size until the dependency can prove a correct high-DPI surface.
		#end
		Raylib.SetConfigFlags(windowFlags);
		Raylib.InitWindow(1280, 720, "Caxecraft — Haxe shaped into careful C");
		// A background agent or remote shell may not own a desktop session. Raylib
		// reports that platform failure through window readiness; stop before any
		// texture or draw call can touch the unavailable graphics context.
		if (!Raylib.IsWindowReady())
			return;
		Raylib.SetWindowMinSize(800, 450);
		Raylib.SetExitKey(KeyboardKey.Null);
		Raylib.SetTargetFPS(120);
		Raylib.EnableCursor();

		// Texture ownership stays explicit until haxe.c can prove automatic
		// cleanup on every exit. Invalid loads use the code-drawn fallback and
		// are never passed to UnloadTexture.
		final titleTexture:Texture2D = CaxecraftTextures.loadTitlePanorama();
		final titleTextureReady = CaxecraftTextures.isValid(titleTexture);
		final wordmarkTexture:Texture2D = CaxecraftTextures.loadWordmark();
		final wordmarkTextureReady = CaxecraftTextures.isValid(wordmarkTexture);
		final hudTexture:Texture2D = CaxecraftTextures.loadHudAtlas();
		final hudTextureReady = CaxecraftTextures.isValid(hudTexture);
		final itemTexture:Texture2D = CaxecraftTextures.loadItemAtlas();
		final itemTextureReady = CaxecraftTextures.isValid(itemTexture);
		final adventureItemTexture:Texture2D = CaxecraftTextures.loadAdventureItemAtlas();
		final adventureItemTextureReady = CaxecraftTextures.isValid(adventureItemTexture);
		final hudResources:HudResources = {
			hudTexture: hudTexture,
			hudTextureReady: hudTextureReady,
			itemTexture: itemTexture,
			itemTextureReady: itemTextureReady,
			adventureItemTexture: adventureItemTexture,
			adventureItemTextureReady: adventureItemTextureReady
		};
		final entityTexture:Texture2D = CaxecraftTextures.loadEntityAtlas();
		final entityTextureReady = CaxecraftTextures.isValid(entityTexture);
		final terrainTexture:Texture2D = CaxecraftTextures.loadTerrainAtlas();
		final terrainTextureReady = CaxecraftTextures.isValid(terrainTexture);
		final adventureTerrainTexture:Texture2D = CaxecraftTextures.loadAdventureTerrainAtlas();
		final adventureTerrainTextureReady = CaxecraftTextures.isValid(adventureTerrainTexture);
		final loadedRuntimeTextures = RuntimeTextureAtlasCatalog.load("assets/caxecraft-runtime-assets.json");
		if (loadedRuntimeTextures == null) {
			if (adventureTerrainTextureReady)
				CaxecraftTextures.unload(adventureTerrainTexture);
			if (terrainTextureReady)
				CaxecraftTextures.unload(terrainTexture);
			if (entityTextureReady)
				CaxecraftTextures.unload(entityTexture);
			if (adventureItemTextureReady)
				CaxecraftTextures.unload(adventureItemTexture);
			if (itemTextureReady)
				CaxecraftTextures.unload(itemTexture);
			if (hudTextureReady)
				CaxecraftTextures.unload(hudTexture);
			if (wordmarkTextureReady)
				CaxecraftTextures.unload(wordmarkTexture);
			if (titleTextureReady)
				CaxecraftTextures.unload(titleTexture);
			Raylib.CloseWindow();
			return;
		}
		final runtimeTextures:RuntimeTextureAtlasCatalog = loadedRuntimeTextures;
		var cameraWaterBlend = 0.0;
		var inventory:InventoryState = Inventory.starter();
		#if caxecraft_pilot
		// Only the deterministic provider may replace the ordinary starter kit.
		// This keeps test setup out of release gameplay and lets native pilots
		// exercise the same typed inventory transitions as a real player.
		inventory = PilotScript.initialInventory(pilotName);
		#end
		var activeDialogue:Null<ScenarioId> = null;
		var latestJournalId:Null<ScenarioId> = null;
		var currentObjectiveId = initialLevel.initialObjectiveId();
		var availableInteractionPrompt = promptForAvailableInteraction(nearestAvailableInteraction(initialSession, initialLevel));
		var enemyActor = initialSession.readCharacter(initialLevel.enemyActorId());
		var initialActorPhases = initialSession.actorControllerStateSnapshots();
		var enemyPhase = observeActorPhase(initialActorPhases, initialLevel.enemyActorId(), ActorControllerPhase.Resting);
		if (!dialogueActorsAreValid(initialSession, initialLevel, initialActorPhases) || !enemyActor.id.isValid() || !enemyPhase.valid)
			return;
		var swordCombat:SwordCombatState = startSwordCombat();
		var berryDrop:BerryDropState = emptyBerryDrop();
		final initialHeading = headingForSpawn(initialLevel.spawnTransform());
		var lookX = initialHeading.x;
		var lookY = initialHeading.y;
		var lookZ = initialHeading.z;
		// Real frame time collects here until there is enough for one or more
		// authoritative fixed simulation steps. The remainder selects a visual
		// position between the last two committed bodies; it never changes gameplay.
		var accumulator = 0.0;
		final initialPresentation = initialSession.view();
		if (!initialPresentation.valid)
			return;
		var motionHistory = startMotion(initialPresentation.localPlayer.body);
		var jumpQueued = false;
		var swordQueued = false;
		var selectedMode:GameMode = GameMode.Creative;
		#if (caxecraft_pilot_move_jump_edit
			|| caxecraft_pilot_combat_drop
			|| caxecraft_pilot_recovery_use
			|| caxecraft_pilot_full_inventory_mining
			|| caxecraft_pilot_aquatic_gear
			|| caxecraft_pilot_campaign_travel)
		// A deterministic provider choice, not gameplay branching: this pilot
		// exercises finite Adventure inventory and actor behavior from frame one.
		selectedMode = GameMode.Adventure;
		#end
		var locale:LocaleCursor = uiCatalog.defaultLocale();
		#if caxecraft_pilot_secondary_locale
		// The graphical locale pilot selects the next validated catalog without
		// teaching the application which human language that catalog contains.
		locale = uiCatalog.nextLocale(locale);
		#end
		#if caxecraft_pilot
		final showInitialTitle = pilotName == PilotScriptName.LaunchSmoke
			|| pilotName == PilotScriptName.ResizeLayout
			|| pilotName == PilotScriptName.EditorShell
			|| pilotName == PilotScriptName.RuntimeContentJourney;
		#else
		final showInitialTitle = true;
		#end
		var screen = initialScreen(showInitialTitle);
		#if caxecraft_pilot_editor_shell
		// Start at the same typed screen transition used by the title menu. The
		// pilot bypasses only operating-system pointer input, not editor drawing.
		screen = openEditor(screen);
		#end
		if (screenCapturesPointer(screen))
			Raylib.DisableCursor();
		var quit = false;
		var frameCount = 0;
		var placementBlockedFrames = 0;
		var strikeHitFrames = 0;
		var enemyDefeatedFrames = 0;
		var enemyAttackFrames = 0;
		var pickupFrames = 0;
		var pickupAmount = 0;
		var inventoryFullFrames = 0;
		var inventoryFullReason = InventoryFullReason.None;
		var recoveryFeedback = RecoveryDecision.NotRecoveryItem;
		var recoveryFeedbackFrames = 0;
		var aquaticEquipmentCode = -1;
		var aquaticEquipmentFrames = 0;
		#if caxecraft_pilot
		var removedBlocks = 0;
		var placedBlocks = 0;
		var rejectedEdits = 0;
		var interpolationObserved = false;
		var submersionObserved = false;
		var waterExitObserved = false;
		var sandMinedObserved = false;
		var flowRuleObserved = false;
		var objectiveChangeObserved = false;
		// The review capture happens before the final state frame. Keep Raylib's
		// immediate filesystem observation so final telemetry can attribute a
		// missing image to the native producer instead of leaving the host to guess.
		var reviewScreenshotObserved = false;
		#if caxecraft_render_benchmark
		var measuredTerrainMicroseconds = 0;
		var measuredTerrainFrames = 0;
		var measuredUpdateMicroseconds = 0;
		var measuredPreparationMicroseconds = 0;
		#end
		#end

		// Outer frame loop: service the operating system and input devices once,
		// advance as many fixed ticks as elapsed time permits, then draw one frame.
		// Its frequency may follow VSync, window load, or GPU speed without changing
		// the duration of a gameplay tick.
		while (!quit && !Raylib.WindowShouldClose()) {
			final loadingAtFrameStart = screenShowsLoading(screen);
			// Work on one immutable view for this frame. Every owned player change
			// below enters through a semantic GameSession operation; presentation
			// never receives the mutable session or its entity store.
			var levelView = activeLevel.level();
			var session = activeLevel.session();
			var initialView = session.view();
			if (!initialView.valid)
				quit = true;
			var character = initialView.localPlayer;
			var recapturedThisFrame = false;
			// Discontinuous application transitions discard old visual motion at the
			// frame's final committed body. This prevents interpolation through walls,
			// across a pause, or from a defeated position back to the authored spawn.
			var resetMotionThisFrame = false;
			#if caxecraft_pilot
			// Pilots own an exact display cadence; interactive builds read the same
			// Raylib frame clock once and share it between UI repeat and simulation.
			#if caxecraft_pilot_runtime
			var frameSeconds = 0.05;
			#else
			var frameSeconds = PilotScript.frameDurationMilliseconds(pilotName, frameCount) / 1000.0;
			#end
			#else
			var frameSeconds = Raylib.GetFrameTime().toFloat();
			#end
			if (frameSeconds > MAX_FRAME_SECONDS)
				frameSeconds = MAX_FRAME_SECONDS;
			#if caxecraft_pilot
			#if caxecraft_pilot_runtime
			if (agentSession && agentWaiting && frameCount % 6 == 0)
				switch contentStore.read("pilots/active.piloscript") {
					case PackageBytesRejected(_):
					case PackageBytesRead(content):
						final requestBytesHash = agentRequestBytesHash(content.bytes);
						if (requestBytesHash != lastAgentRequestBytesHash) {
							lastAgentRequestBytesHash = requestBytesHash;
							switch RuntimePilotScript.read(content.bytes, content.provenance.logicalPath.text()) {
								case RuntimePilotRejected(diagnostic):
									Sys.println('CAXECRAFT_AGENT_ERROR=request rejected at line ${diagnostic.line}: ${diagnostic.message}');
								case RuntimePilotReady(request):
									if (!StringTools.startsWith(request.stableName(),
										"agent-session-")) Sys.println("CAXECRAFT_AGENT_ERROR=request name must start with agent-session-"); else
										if (request.inputHash() != pilotInputHash) {
										runtimePilot = request;
										pilotInputHash = request.inputHash();
										runtimePilotFrame = 0;
										agentWaiting = false;
									}
							}
						}
				};
			final requestedWindowWidth = 0;
			final requestedWindowHeight = 0;
			final activeRuntimePilotFrame = agentSession ? runtimePilotFrame : frameCount;
			final pilotAction = agentWaiting ? PilotAction.Idle : runtimePilot.actionAt(activeRuntimePilotFrame);
			#else
			final requestedWindowWidth = PilotScript.requestedWindowWidth(pilotName, frameCount);
			// Keep both conditional results stable before the native call. haxe_c-af1
			// owns the compiler fix that will make this explicit local unnecessary.
			final requestedWindowHeight = PilotScript.requestedWindowHeight(pilotName, frameCount);
			final pilotAction = PilotScript.actionAt(pilotName, frameCount);
			#end
			if (requestedWindowWidth > 0)
				Raylib.SetWindowSize(requestedWindowWidth, requestedWindowHeight);
			final focused = true;
			final moveForward = PilotScript.moveForward(pilotAction);
			final moveRight = PilotScript.moveRight(pilotAction);
			final lookYaw = PilotScript.lookYaw(pilotAction);
			final lookPitch = PilotScript.lookPitch(pilotAction);
			final jumpPressed = PilotScript.jumpPressed(pilotAction);
			final riseHeld = PilotScript.riseHeld(pilotAction);
			final primaryPressed = PilotScript.primaryPressed(pilotAction);
			final secondaryPressed = PilotScript.secondaryPressed(pilotAction);
			final interactPressed = PilotScript.interactPressed(pilotAction);
			final travelPressed = PilotScript.travelPressed(pilotAction);
			final pausePressed = PilotScript.pausePressed(pilotAction);
			final capturePressed = PilotScript.capturePressed(pilotAction);
			final quitPressed = PilotScript.quitPressed(pilotAction);
			final hotbarSelection = PilotScript.hotbarSelection(pilotAction);
			final hotbarCycle = PilotScript.hotbarCycle(pilotAction);
			final menuNextPressed = PilotScript.menuNextPressed(pilotAction);
			final menuConfirmPressed = PilotScript.menuConfirmPressed(pilotAction);
			final descendHeld = PilotScript.descendHeld(pilotAction);
			#else
			final focused = Raylib.IsWindowFocused();
			final frameInput:GameInputFrame = RaylibGameInput.sample(screenCapturesPointer(screen), screenPausesSimulation(screen));
			// Project the direct record immediately. Only scalar values stay live
			// through the loop, so generated C retains one obvious input sample.
			final moveForward = frameInput.moveForward;
			final moveRight = frameInput.moveRight;
			final lookYaw = frameInput.lookYaw;
			final lookPitch = frameInput.lookPitch;
			final jumpPressed = frameInput.jumpPressed;
			final riseHeld = frameInput.riseHeld;
			final primaryPressed = frameInput.primaryPressed;
			final secondaryPressed = frameInput.secondaryPressed;
			final interactPressed = frameInput.interactPressed;
			final travelPressed = frameInput.travelPressed;
			final pausePressed = frameInput.pausePressed;
			final capturePressed = frameInput.capturePressed;
			final quitPressed = frameInput.quitPressed;
			final hotbarSelection = frameInput.hotbarSelection;
			final hotbarCycle = frameInput.hotbarCycle;
			final menuNextPressed = frameInput.menuNextPressed;
			final menuConfirmPressed = frameInput.menuConfirmPressed;
			final descendHeld = frameInput.descendHeld;
			#end
			var requestedCampaignLevel:Null<CampaignLevel> = null;
			if (loadingAtFrameStart && loadingFramePresented)
				requestedCampaignLevel = pendingCampaignLevel;
			var requestedFromCampaignMenu = false;
			final requestedFromAuthoredExit = requestedCampaignLevel != null && loadingAtFrameStart;
			#if (caxecraft_devmode || caxecraft_pilot_campaign_travel)
			if (!quit && allowsCampaignTravel(screen, selectedMode) && travelPressed) {
				final selectedCampaign = campaign;
				final sourceLevel = campaignLevel;
				if (selectedCampaign != null && sourceLevel != null) {
					final transition = selectedCampaign.unambiguousTransitionFrom(sourceLevel.id);
					if (transition == null) {
						Sys.println("caxecraft: campaign travel requires exactly one outgoing transition from " + sourceLevel.id.text());
					} else {
						final destination = selectedCampaign.level(transition.destinationLevel);
						if (destination == null)
							Sys.println("caxecraft: campaign destination disappeared after manifest validation");
						else
							requestedCampaignLevel = destination;
					}
				}
			}
			#end

			if (screenShowsCampaignSelection(screen) && focused) {
				var campaignLaunchRequested = menuConfirmPressed;
				var campaignBackRequested = pausePressed;
				#if caxecraft_devmode
				if (menuNextPressed && campaign != null && campaign.levelCount() > 0)
					selectedCampaignLevelIndex = (selectedCampaignLevelIndex + 1) % campaign.levelCount();
				#end
				#if !caxecraft_pilot
				if (Raylib.IsKeyPressed(KeyboardKey.L))
					locale = uiCatalog.nextLocale(locale);
				final campaignMouse = Raylib.GetMousePosition();
				final campaignChoice = CampaignMenu.selectionAt(campaignMouse.x.toFloat(), campaignMouse.y.toFloat(), Raylib.GetScreenWidth(),
					Raylib.GetScreenHeight(), campaign == null ? 0 : campaign.levelCount(), selectedCampaignLevelIndex);
				if (Raylib.IsMouseButtonPressed(MouseButton.Left))
					switch campaignChoice {
						case NoCampaignMenuHit:
						case LevelHit(index):
							#if caxecraft_devmode
							selectedCampaignLevelIndex = index;
							#end
						case LaunchHit:
							campaignLaunchRequested = true;
						case BackHit:
							campaignBackRequested = true;
					}
				#end
				if (campaignBackRequested) {
					screen = closeCampaignSelection(screen);
					accumulator = 0.0;
					resetMotionThisFrame = true;
				} else if (campaign != null && campaignLaunchRequested) {
					requestedCampaignLevel = campaign.levelAt(selectedCampaignLevelIndex);
					requestedFromCampaignMenu = true;
				}
			}

			if (screenShowsTitle(screen) && focused) {
				final screenBeforeMenu = screen;
				final modeBeforeMenu = selectedMode;
				var menuState = titleMenuState(screen, selectedMode);
				#if !caxecraft_pilot
				if (Raylib.IsKeyPressed(KeyboardKey.L))
					locale = uiCatalog.nextLocale(locale);
				if (menuNextPressed)
					menuState = applyTitleMenuCommand(menuState, TitleMenuCommand.CycleMode);

				final menuMouse = Raylib.GetMousePosition();
				final hovered = TitleMenu.selectionAt(menuMouse.x.toFloat(), menuMouse.y.toFloat(), Raylib.GetScreenWidth(), Raylib.GetScreenHeight());
				if (hovered == 0)
					menuState = applyTitleMenuCommand(menuState, TitleMenuCommand.SelectCreative);
				if (hovered == 1)
					menuState = applyTitleMenuCommand(menuState, TitleMenuCommand.SelectAdventure);
				final clickedChoice = hovered >= 0 && Raylib.IsMouseButtonPressed(MouseButton.Left);
				if (clickedChoice)
					menuState = applyTitleMenuCommand(menuState, hovered == 2 ? TitleMenuCommand.OpenEditor : TitleMenuCommand.ConfirmMode);
				else if (menuConfirmPressed)
					menuState = applyTitleMenuCommand(menuState, TitleMenuCommand.ConfirmMode);
				#else
				if (menuNextPressed)
					menuState = applyTitleMenuCommand(menuState, TitleMenuCommand.CycleMode);
				if (menuConfirmPressed)
					menuState = applyTitleMenuCommand(menuState, TitleMenuCommand.ConfirmMode);
				#end
				screen = menuState.screen;
				selectedMode = menuState.mode;
				if (screenShowsCampaignSelection(screen) && campaign == null)
					screen = screenBeforeMenu;
				if (screen != screenBeforeMenu || selectedMode != modeBeforeMenu) {
					accumulator = 0.0;
					resetMotionThisFrame = true;
				}
				if (screenBeforeMenu == AppScreen.Title && screenIsPlaying(screen)) {
					recapturedThisFrame = true;
					Raylib.DisableCursor();
				} else if (screenShowsEditor(screen)) {
					Raylib.EnableCursor();
				}
			}

			// Authored exits and the developer level picker enter through the same
			// checked loader. The new map replaces live state only after its package
			// receipt, schema, content references, and playable bindings all succeed.
			final destination = requestedCampaignLevel;
			if (destination != null) {
				final playerOptions:LevelPlayerOptions = {
					entityId: character.id,
					initialHealth: character.vitals.health,
					aquaticProfile: character.aquaticProfile
				};
				final nextGeneration = ContentGenerationId.fromSequence(activeLevel.generationId().value() + 1);
				switch loadCampaignLevel(contentStore, destination, nextGeneration, contentRegistry, contentRegistry, playerOptions) {
					case CampaignLevelRejected(error):
						Sys.println("caxecraft: campaign level rejected: " + campaignLevelLoadFailure(error));
					case CampaignLevelReady(candidate):
						switch activeLevel.publish(candidate) {
							case PlayableLevelPublicationRejected(_):
								Sys.println("caxecraft: campaign level could not replace the active level");
							case PlayableLevelPublished(_, selected):
								campaignLevel = destination;
								levelLabel = destination.id.text();
								final selectedCampaign = campaign;
								if (selectedCampaign != null)
									for (index in 0...selectedCampaign.levelCount())
										if (selectedCampaign.levelAt(index).id.text() == destination.id.text())
											selectedCampaignLevelIndex = index;
								levelView = activeLevel.level();
								session = activeLevel.session();
								initialView = session.view();
								if (!initialView.valid) {
									quit = true;
								} else {
									character = initialView.localPlayer;
									activeDialogue = null;
									latestJournalId = null;
									currentObjectiveId = levelView.initialObjectiveId();
									availableInteractionPrompt = promptForAvailableInteraction(nearestAvailableInteraction(session, levelView));
									enemyActor = session.readCharacter(levelView.enemyActorId());
									final phases = session.actorControllerStateSnapshots();
									enemyPhase = observeActorPhase(phases, levelView.enemyActorId(), ActorControllerPhase.Resting);
									if (!dialogueActorsAreValid(session, levelView, phases)
										|| !enemyActor.id.isValid()
										|| !enemyPhase.valid)
										quit = true;
									swordCombat = startSwordCombat();
									berryDrop = emptyBerryDrop();
									cameraWaterBlend = 0.0;
									final publishedHeading = headingForSpawn(levelView.spawnTransform());
									lookX = publishedHeading.x;
									lookY = publishedHeading.y;
									lookZ = publishedHeading.z;
									accumulator = 0.0;
									motionHistory = resetMotion(character.body);
									jumpQueued = false;
									swordQueued = false;
									placementBlockedFrames = 0;
									strikeHitFrames = 0;
									enemyDefeatedFrames = 0;
									enemyAttackFrames = 0;
									pickupFrames = 0;
									inventoryFullFrames = 0;
									inventoryFullReason = InventoryFullReason.None;
									recoveryFeedbackFrames = 0;
									aquaticEquipmentFrames = 0;
									terrainRenderer.invalidateAll();
									resetMotionThisFrame = true;
									if (requestedFromCampaignMenu) {
										screen = startSelectedCampaign(screen);
										recapturedThisFrame = true;
										Raylib.DisableCursor();
									}
									Sys.println("caxecraft: campaign-level=" + levelLabel + " generation=" + Std.string(selected.value()));
								}
						}
				}
				if (requestedFromAuthoredExit) {
					pendingCampaignLevel = null;
					pendingCampaignLabel = "";
					loadingFramePresented = false;
					screen = finishLoading(screen);
					recapturedThisFrame = true;
					Raylib.DisableCursor();
				}
			}

			final enemyActorId = levelView.enemyActorId();
			final spawnTransform = levelView.spawnTransform();
			if (quitPressed)
				quit = true;
			if (hotbarSelection >= 0)
				inventory = Inventory.select(inventory, hotbarSelection);
			if (hotbarCycle != 0)
				inventory = Inventory.cycle(inventory, hotbarCycle);
			if (screenIsPlaying(screen) && interactPressed) {
				if (characterIsDefeated(character.vitals)) {
					final revival = session.reviveLocalPlayerAt(spawnPlayer(session.worldView(), spawnTransform));
					character = revival.character;
					if (!revival.resolved)
						quit = true;
					else {
						final revivedHeading = headingForSpawn(spawnTransform);
						lookX = revivedHeading.x;
						lookY = revivedHeading.y;
						lookZ = revivedHeading.z;
						cameraWaterBlend = 0.0;
						accumulator = 0.0;
						resetMotionThisFrame = true;
					}
				} else if (activeDialogue != null) {
					activeDialogue = null;
				} else {
					switch nearestAvailableInteraction(session, levelView) {
						case NoAvailableInteraction:
						case DialogueInteraction(id):
							if (!session.interactWithActor(id))
								quit = true;
						case StatefulObjectInteraction(id):
							if (!session.interactWithStatefulObject(id))
								quit = true;
					}
				}
			}

			if (!focused && screenIsPlaying(screen)) {
				screen = pauseAfterFocusLoss(screen);
				jumpQueued = false;
				accumulator = 0.0;
				resetMotionThisFrame = true;
				Raylib.EnableCursor();
			}
			if (!screenShowsTitle(screen)
				&& !screenShowsCampaignSelection(screen)
				&& !screenShowsLoading(screen)
				&& !screenShowsEditor(screen)
				&& focused
				&& pausePressed) {
				screen = togglePause(screen);
				accumulator = 0.0;
				resetMotionThisFrame = true;
				if (screenPausesSimulation(screen))
					jumpQueued = false;
				if (screenCapturesPointer(screen))
					Raylib.DisableCursor();
				else
					Raylib.EnableCursor();
			}
			if (!screenShowsTitle(screen)
				&& !screenShowsCampaignSelection(screen)
				&& !screenShowsLoading(screen)
				&& !screenShowsEditor(screen)
				&& focused
				&& screenPausesSimulation(screen)
				&& capturePressed) {
				screen = recapturePointer(screen);
				recapturedThisFrame = true;
				accumulator = 0.0;
				resetMotionThisFrame = true;
				Raylib.DisableCursor();
			}

			// These are read-only projections of one closed state, not independent
			// flags. A fixed tick can begin a campaign handoff later in this frame;
			// gameplay mutations after that tick therefore re-read the live screen.
			final onTitle = screenShowsTitle(screen);
			final onCampaignSelect = screenShowsCampaignSelection(screen);
			final onLoading = screenShowsLoading(screen);
			final onEditor = screenShowsEditor(screen);
			final paused = screenPausesSimulation(screen);
			final captured = screenCapturesPointer(screen);
			#if caxecraft_pilot_runtime
			var runtimePilotFrameAccepted = true;
			var observedLevel = levelLabel;
			if (onCampaignSelect) {
				final selectedCampaign = campaign;
				if (selectedCampaign != null)
					observedLevel = selectedCampaign.levelAt(selectedCampaignLevelIndex).id.text();
			}
			final observedObjective = currentObjectiveId == null ? "none" : currentObjectiveId.text();
			final observedDialogue = activeDialogue == null ? "none" : activeDialogue.text();
			final observedJournal = latestJournalId == null ? "none" : latestJournalId.text();
			final observedScreen = onTitle ? "title" : onCampaignSelect ? "campaign" : onLoading ? "loading" : onEditor ? "editor" : paused ? "paused" : "playing";
			final observedMode = selectedMode == GameMode.Creative ? "creative" : "adventure";
			final observedMedium = if (character.aquatic.medium == AquaticMedium.Dry) "dry" else if (character.aquatic.medium == AquaticMedium.Wading)
				"wading" else if (character.aquatic.medium == AquaticMedium.Floating) "floating" else "submerged";
			final observedEquipment = aquaticEquipmentCode < 0 ? "none" : contentRegistry.itemIdForStorageCode(aquaticEquipmentCode);
			final observedStatefulObjectIds:Array<String> = [];
			final observedStatefulObjectStates:Array<String> = [];
			final pilotLevelView = levelView;
			for (index in 0...pilotLevelView.statefulObjectCount()) {
				final authoredId = pilotLevelView.statefulObjectIdAt(index);
				final state = session.statefulObjectState(authoredId);
				observedStatefulObjectIds.push(authoredId.text());
				observedStatefulObjectStates.push(state == null ? "missing" : state.text());
			}
			switch runtimePilot.observe(activeRuntimePilotFrame, {
				screen: observedScreen,
				mode: observedMode,
				level: observedLevel,
				objective: observedObjective,
				dialogue: observedDialogue,
				journal: observedJournal,
				generation: activeLevel.generationId().value(),
				publications: activeLevel.publicationCount(),
				cellX: Std.int(character.body.x),
				cellY: Std.int(character.body.y),
				cellZ: Std.int(character.body.z),
				aquaticMedium: observedMedium,
				aquaticEquipment: observedEquipment,
				lanterns: inventory.lantern,
				sand: inventory.sand,
				statefulObjectIds: observedStatefulObjectIds,
				statefulObjectStates: observedStatefulObjectStates
			}) {
				case RuntimePilotFrameAccepted:
				case RuntimePilotFrameRejected(diagnostic):
					runtimePilotFrameAccepted = false;
					quit = true;
					Sys.println('caxecraft: runtime Piloscript mismatch at ${diagnostic.source}:${diagnostic.line}: ${diagnostic.message}');
			}
			#end
			var editorNavigationCommand = NavigationCommand.None;
			#if !caxecraft_pilot
			if (onEditor)
				editorNavigationCommand = editorNavigation.advance(samplePrimaryGamepad(), frameSeconds);
			else
				editorNavigation.release();
			#end
			#if caxecraft_pilot
			// The editor pilot submits a title and voxel gestures through the
			// same screen methods used by native input. It bypasses only the
			// operating system's keyboard and mouse delivery so repeated
			// headless runs remain deterministic. Selecting the newly painted
			// cell also gives the framebuffer oracle a specific 3D outline.
			// One held controller direction moves immediately, repeats after the
			// production delay, then repeats at the production interval. The
			// resulting three moves land on Validate before the south face button
			// confirms it through the same device-neutral screen handler.
			if (pilotName == PilotScriptName.EditorShell && onEditor && frameCount == 1) {
				final heldDown:NavigationSample = {
					connected: true,
					up: false,
					right: false,
					down: true,
					left: false,
					confirmPressed: false,
					cancelPressed: false,
					horizontal: 0.0,
					vertical: 0.0
				};
				editorScreen.applyNavigation(editorNavigation.advance(heldDown, 0.0));
				editorScreen.applyNavigation(editorNavigation.advance(heldDown, NavigationRepeater.INITIAL_REPEAT_DELAY_SECONDS));
				editorScreen.applyNavigation(editorNavigation.advance(heldDown, NavigationRepeater.REPEAT_INTERVAL_SECONDS));
				final confirm:NavigationSample = {
					connected: true,
					up: false,
					right: false,
					down: false,
					left: false,
					confirmPressed: true,
					cancelPressed: false,
					horizontal: 0.0,
					vertical: 0.0
				};
				if (editorScreen.applyNavigation(editorNavigation.advance(confirm, 0.0)) != EditorScreenAction.StayInEditor)
					rejectedEdits++;
				// Observe cancel through the same handler without changing the
				// application's screen here; the pilot must keep the editor visible
				// long enough to capture its focus ring.
				if (editorScreen.applyNavigation(NavigationCommand.Cancel) != EditorScreenAction.ReturnToTitle)
					rejectedEdits++;
				if (!editorScreen.applyPilotWorldName("Ivvy's Workshop"))
					rejectedEdits++;
				if (!editorScreen.applyPilotCamera({
					forward: 0.5,
					right: -0.25,
					vertical: 0.1,
					yaw: 0.08,
					pitch: 0.02,
					wheel: 0.0
				}, 0.05))
					rejectedEdits++;
				if (editorScreen.applyPilotTool(EditorTool.PaintTool, {x: 2, y: 0, z: 2})) {
					placedBlocks++;
					if (!editorScreen.applyPilotTool(EditorTool.SelectTool, {x: 2, y: 0, z: 2}))
						rejectedEdits++;
				} else
					rejectedEdits++;
			}
			#end
			if (captured) {
				var yawDelta = lookYaw;
				if (yawDelta > 0.25)
					yawDelta = 0.25;
				if (yawDelta < -0.25)
					yawDelta = -0.25;
				final candidateX = lookX + yawDelta * lookZ;
				final candidateZ = lookZ - yawDelta * lookX;
				final lengthSquared = candidateX * candidateX + candidateZ * candidateZ;
				final normalize = 1.5 - 0.5 * lengthSquared;
				lookX = candidateX * normalize;
				lookZ = candidateZ * normalize;
				lookY += lookPitch;
				if (lookY > 0.90)
					lookY = 0.90;
				if (lookY < -0.90)
					lookY = -0.90;
				if (jumpPressed)
					jumpQueued = true;
			}

			if (captured
				&& !recapturedThisFrame
				&& primaryPressed
				&& selectedMode == GameMode.Adventure
				&& Inventory.selectedIs(inventory, ItemKind.CopperSword))
				swordQueued = true;
			if (!paused)
				accumulator += frameSeconds;
			#if caxecraft_render_benchmark
			final updateStarted = Raylib.GetTime();
			#end
			// Inner fixed-step loop: a slow rendered frame may need several gameplay
			// ticks, while a fast frame may need none. Every tick receives the same
			// documented duration, so game rules never depend on frame rate.
			while (!screenPausesSimulation(screen) && accumulator >= FIXED_SECONDS) {
				// Water owns a bounded amount of work per game tick. A large leak
				// therefore continues over later ticks without freezing a frame.
				var moveX = moveForward * lookX - moveRight * lookZ;
				var moveZ = moveForward * lookZ + moveRight * lookX;
				if (moveForward != 0.0 && moveRight != 0.0) {
					moveX *= 0.7071067811865476;
					moveZ *= 0.7071067811865476;
				}
				final damagePolicy = selectedMode == GameMode.Adventure ? CharacterDamagePolicy.Survival : CharacterDamagePolicy.Invulnerable;
				final rising = jumpQueued || (character.aquatic.medium != AquaticMedium.Dry && riseHeld);
				final gameTick = session.tick({
					intent: aquaticInput(moveX, moveZ, rising, descendHeld),
					damagePolicy: damagePolicy,
					waterUpdateBudget: 64
				});
				character = gameTick.character;
				final flow = gameTick.flow;
				if (flow != null) {
					#if caxecraft_pilot
					if (flow.firedRules.length > 0)
						flowRuleObserved = true;
					#end
					if (flow.diagnostics.length == 0) {
						for (event in flow.presentation)
							switch event {
								case FlowPresentationEvent.DialogueRequested(id):
									activeDialogue = id;
								case FlowPresentationEvent.JournalAdded(id):
									latestJournalId = id;
								case FlowPresentationEvent.CampaignExitRequested(exit):
									final selectedCampaign = campaign;
									final sourceLevel = campaignLevel;
									if (selectedMode != GameMode.Adventure || !screenIsPlaying(screen)) {
										Sys.println("caxecraft: ignored campaign exit outside active Adventure play: " + exit.text());
									} else if (selectedCampaign == null || sourceLevel == null) {
										Sys.println("caxecraft: ignored campaign exit without an active campaign: " + exit.text());
									} else {
										final transition = selectedCampaign.transitionForRequest(sourceLevel.id, exit);
										if (transition == null) {
											Sys.println('caxecraft: ignored unknown campaign exit ${exit.text()} from ${sourceLevel.id.text()}');
										} else {
											final nextLevel = selectedCampaign.level(transition.destinationLevel);
											if (nextLevel == null) {
												Sys.println("caxecraft: campaign destination disappeared after manifest validation");
											} else {
												pendingCampaignLevel = nextLevel;
												pendingCampaignLabel = nextLevel.id.text();
												loadingFramePresented = false;
												screen = beginLoading(screen);
												accumulator = 0.0;
												jumpQueued = false;
												Raylib.EnableCursor();
											}
										}
									}
								case _:
							}
						#if caxecraft_pilot
						final beforeObjective = currentObjectiveId;
						final afterObjective = flow.activeObjective;
						if ((beforeObjective == null && afterObjective != null)
							|| (beforeObjective != null && afterObjective == null)
							|| (beforeObjective != null && afterObjective != null && beforeObjective.text() != afterObjective.text()))
							objectiveChangeObserved = true;
						#end
						currentObjectiveId = flow.activeObjective;
					}
				}
				#if caxecraft_pilot
				if (pilotName == PilotScriptName.AquaticGear) {
					if (character.aquatic.medium == AquaticMedium.Submerged)
						submersionObserved = true;
					if (submersionObserved && character.aquatic.medium == AquaticMedium.Dry)
						waterExitObserved = true;
				}
				#end
				if (gameTick.committed)
					motionHistory = advanceMotion(motionHistory, character.body);
				cameraWaterBlend = gameTick.immersion.cameraBlend;
				if (!gameTick.committed)
					quit = true;
				if (!characterIsDefeated(character.vitals)) {
					var pickupIndex = 0;
					while (pickupIndex < levelView.loadedItemCount()) {
						final loadedItem = levelView.loadedItemAt(pickupIndex);
						if (session.authoredItemIsActive(pickupIndex)
							&& authoredItemIsInRange(character.body.x, character.body.y, character.body.z, loadedItem.xMilli, loadedItem.yMilli,
								loadedItem.zMilli)) {
							final itemCode = loadedItem.storageCode;
							if (contentRegistry.itemUseProfile(itemCode) == RuntimeItemUseProfile.EquipAquatic
								&& contentRegistry.itemProvidesAquaticProfile(itemCode)) {
								final replacement = contentRegistry.itemAquaticProfile(itemCode);
								final equipment = session.collectAuthoredAquaticEquipment(pickupIndex, replacement);
								character = equipment.character;
								if (!equipment.resolved)
									quit = true;
								else if (equipment.collected) {
									aquaticEquipmentCode = itemCode;
									aquaticEquipmentFrames = 120;
								}
							} else {
								final inventoryKind = inventoryKindForRuntimeItem(contentRegistry, itemCode);
								if (inventoryKind != null) {
									final pickup = session.collectAuthoredInventoryItem(pickupIndex, inventory, inventoryKind, loadedItem.quantity);
									inventory = pickup.inventory;
									if (!pickup.resolved)
										quit = true;
								}
							}
						}
						pickupIndex++;
					}
				}
				if (selectedMode == GameMode.Adventure) {
					if (!characterIsDefeated(character.vitals)) {
						final actorTick = session.stepAuthoredActorControllers(gameTick.tickIndex, damagePolicy);
						switch actorTick.status {
							case ControllersAdvanced:
								availableInteractionPrompt = InteractionPrompt.NoInteractionPrompt;
								for (event in session.actorControllerEventSnapshots())
									switch event {
										case NoControllerEvent:
										case InteractionAvailable(source):
											if (levelView.hasDialogueActor(source)) availableInteractionPrompt = InteractionPrompt.TalkInteractionPrompt;
										case LocalPlayerAttack(source):
											if (source == enemyActorId) enemyAttackFrames = 120;
										case DropRequested(source, drop):
											final quantity = contentRegistry.dropQuantityById(drop);
											final defeatedActor = session.readCharacter(source);
											if (quantity <= 0 || !defeatedActor.id.isValid()) quit = true; else if (!berryDrop.active) {
												berryDrop = berryDropFromDefeatedCharacter(defeatedActor, quantity);
												enemyDefeatedFrames = 120;
											}
									}
							case ControllerModelRejected(_, _) | ControlledCharacterMissing(_) | CharacterCommandRejected(_) | LocalAttackCommandRejected(_):
								quit = true;
						}
					}
					final actorPhases = session.actorControllerStateSnapshots();
					enemyPhase = observeActorPhase(actorPhases, enemyActorId, ActorControllerPhase.Defeated);
					enemyActor = session.readCharacter(enemyActorId);
					if (!dialogueActorsAreValid(session, levelView, actorPhases) || !enemyPhase.valid || !enemyActor.id.isValid())
						quit = true;
					swordCombat = stepSwordCombat(swordCombat);
					if (swordQueued) {
						final swordDecision = decideSwordCombat(swordCombat, inventory, character.vitals, enemyActor, character.body.x, character.body.z,
							lookX, lookZ);
						if (swordDecision == SwordCombatDecision.Hit) {
							final damage = session.damageCharacter(enemyActorId, 1);
							if (!damage.resolved)
								quit = true;
							else if (damage.damageApplied > 0) {
								enemyActor = damage.character;
								strikeHitFrames = 16;
							}
						}
						swordCombat = afterSwordCombat(swordDecision, swordCombat);
						swordQueued = false;
					}
				}
				jumpQueued = false;
				accumulator -= FIXED_SECONDS;
			}
			#if caxecraft_render_benchmark
			if (frameCount >= 2)
				measuredUpdateMicroseconds += Std.int((Raylib.GetTime() - updateStarted) * 1000000.0);
			#end

			// Selection is authoritative gameplay: it originates at the latest committed
			// body, never at the presentation-only camera position below.
			final selectionEyeX = character.body.x;
			final selectionEyeY = character.body.y + 1.62;
			final selectionEyeZ = character.body.z;
			final hit = VoxelRaycast.trace(session.worldView(), selectionEyeX, selectionEyeY, selectionEyeZ, lookX, lookY, lookZ, PICK_DISTANCE);
			if (screenCapturesPointer(screen) && !recapturedThisFrame && primaryPressed) {
				if (!characterIsDefeated(character.vitals)) {
					if (selectedMode == GameMode.Adventure) {
						if (!Inventory.selectedIs(inventory, ItemKind.CopperSword)
							&& hit.hit
							&& playerCanMine(character.aquatic, character.aquaticProfile)) {
							final minedCoordinate = World.coord(hit.cellX, hit.cellY, hit.cellZ);
							final sandBefore = inventory.sand;
							final mining = session.mineTerrain(minedCoordinate, inventory);
							inventory = mining.inventory;
							if (mining.outcome == MiningOutcome.Collected) {
								terrainRenderer.invalidate(minedCoordinate);
							}
							#if caxecraft_pilot
							if (mining.outcome == MiningOutcome.Collected) {
								removedBlocks++;
								if (inventory.sand == sandBefore + 1)
									sandMinedObserved = true;
							}
							#end
							if (mining.outcome == MiningOutcome.InventoryFull) {
								#if caxecraft_pilot
								rejectedEdits++;
								#end
								inventoryFullReason = InventoryFullReason.BlockStack;
								inventoryFullFrames = 90;
							}
						}
					} else if (hit.hit) {
						final removedCoordinate = World.coord(hit.cellX, hit.cellY, hit.cellZ);
						final removed = session.removeTerrain(removedCoordinate);
						if (removed)
							terrainRenderer.invalidate(removedCoordinate);
						#if caxecraft_pilot
						if (removed)
							removedBlocks++;
						#end
					}
				}
			}
			if (screenCapturesPointer(screen) && secondaryPressed) {
				if (!characterIsDefeated(character.vitals)) {
					final recovery = session.useSelectedRecovery(inventory);
					character = recovery.character;
					if (!recovery.resolved)
						quit = true;
					else if (recovery.decision != RecoveryDecision.NotRecoveryItem) {
						recoveryFeedback = recovery.decision;
						recoveryFeedbackFrames = 90;
						inventory = recovery.inventory;
					} else if (hit.hit) {
						final placement = World.coord(hit.previousX, hit.previousY, hit.previousZ);
						final selectedBlock = Inventory.selectedBlock(inventory);
						final hasItem = Inventory.countAt(inventory, inventory.selected) > 0;
						if (!hasItem
							|| !World.isPlaceable(selectedBlock)
							|| !playerCanPlaceAt(character.body, placement)
							|| !session.placeTerrain(placement, selectedBlock)) {
							placementBlockedFrames = 60;
							#if caxecraft_pilot
							rejectedEdits++;
							#end
						} else {
							terrainRenderer.invalidate(placement);
							#if caxecraft_pilot
							placedBlocks++;
							#end
							if (selectedMode == GameMode.Adventure)
								inventory = Inventory.consumeSelected(inventory);
						}
					}
				}
			}
			if (placementBlockedFrames > 0)
				placementBlockedFrames--;
			if (!screenPausesSimulation(screen)) {
				if (berryDropIsInRange(berryDrop, character.body.x, character.body.y, character.body.z)) {
					final acceptedDrop = Inventory.acceptedAmount(inventory, ItemKind.Berries, berryDrop.amount);
					if (acceptedDrop > 0) {
						inventory = Inventory.collectItem(inventory, ItemKind.Berries, acceptedDrop);
						berryDrop = collectBerryDropAmount(berryDrop, acceptedDrop);
						pickupAmount = acceptedDrop;
						pickupFrames = 90;
					} else {
						inventoryFullReason = InventoryFullReason.BerryStack;
						inventoryFullFrames = 90;
					}
				}
			}
			if (strikeHitFrames > 0)
				strikeHitFrames--;
			if (enemyDefeatedFrames > 0)
				enemyDefeatedFrames--;
			if (enemyAttackFrames > 0)
				enemyAttackFrames--;
			if (pickupFrames > 0)
				pickupFrames--;
			if (inventoryFullFrames > 0) {
				inventoryFullFrames--;
				if (inventoryFullFrames == 0)
					inventoryFullReason = InventoryFullReason.None;
			}
			if (recoveryFeedbackFrames > 0)
				recoveryFeedbackFrames--;
			if (aquaticEquipmentFrames > 0)
				aquaticEquipmentFrames--;
			// Presentation samples the session after every semantic command. The app
			// never publishes a whole replacement character, so a later fixed tick
			// cannot overwrite damage, recovery, revival, or equipped capabilities
			// that existed only in a temporary frame variable.
			final committedView = session.view();
			if (!committedView.valid)
				quit = true;
			else
				character = committedView.localPlayer;
			availableInteractionPrompt = promptForAvailableInteraction(nearestAvailableInteraction(session, levelView));
			final presentationActorPhases = session.actorControllerStateSnapshots();
			enemyPhase = observeActorPhase(presentationActorPhases, enemyActorId, ActorControllerPhase.Defeated);
			enemyActor = session.readCharacter(enemyActorId);
			if (!dialogueActorsAreValid(session, levelView, presentationActorPhases) || !enemyPhase.valid || !enemyActor.id.isValid())
				quit = true;
			final dialogueActors = dialogueActorSnapshots(session, levelView);
			final completedTicks = committedView.completedTicks;
			if (resetMotionThisFrame)
				motionHistory = resetMotion(character.body);

			// Rendering intentionally trails the simulation by at most one fixed tick.
			// That small delay turns 20 Hz committed motion into smooth display-rate
			// camera motion without guessing a future position or changing collision.
			final renderPosition = sampleMotion(motionHistory, accumulator, FIXED_SECONDS);
			#if caxecraft_pilot
			if (pilotName == PilotScriptName.SmoothMotion) {
				final xBetween = motionHistory.previous.x != motionHistory.current.x
					&& renderPosition.x != motionHistory.previous.x
					&& renderPosition.x != motionHistory.current.x;
				final yBetween = motionHistory.previous.y != motionHistory.current.y
					&& renderPosition.y != motionHistory.previous.y
					&& renderPosition.y != motionHistory.current.y;
				final zBetween = motionHistory.previous.z != motionHistory.current.z
					&& renderPosition.z != motionHistory.previous.z
					&& renderPosition.z != motionHistory.current.z;
				if (xBetween || yBetween || zBetween)
					interpolationObserved = true;
			}
			#end
			final eyeX = renderPosition.x;
			final eyeY = renderPosition.y + 1.62;
			final eyeZ = renderPosition.z;
			final camera = Camera3D.make(Vector3.fromFloat(eyeX, eyeY, eyeZ), Vector3.fromFloat(eyeX + lookX, eyeY + lookY, eyeZ + lookZ),
				Vector3.fromFloat(0.0, 1.0, 0.0), c.Float32.fromFloat(70.0), CameraProjection.Perspective);
			#if caxecraft_pilot
			var visibleBlocks = 0;
			var terrainDrawCalls = 0;
			var visibleTerrainFaces = 0;
			var rebuiltTerrainChunks = 0;
			var totalRebuiltTerrainChunks = 0;
			var terrainCacheValid = true;
			#end
			// Drawing occurs once per outer frame after all available fixed steps. It
			// must observe committed state and must not advance simulation itself.
			Raylib.BeginDrawing();
			if (onTitle) {
				TitleMenu.draw(titleTexture, titleTextureReady, wordmarkTexture, wordmarkTextureReady, selectedMode, locale, uiCatalog,
					levelView.adventureTagline(scenarioLocale(locale)));
			} else if (onCampaignSelect) {
				final selectedCampaign = campaign;
				if (selectedCampaign == null)
					screen = closeCampaignSelection(screen);
				else
					CampaignMenu.draw(titleTexture, titleTextureReady, wordmarkTexture, wordmarkTextureReady, selectedCampaign, locale, uiCatalog,
						selectedCampaignLevelIndex, levelView.scenarioTitle(scenarioLocale(locale)), levelView.adventureTagline(scenarioLocale(locale)));
			} else if (onLoading) {
				drawCampaignLoading(pendingCampaignLabel, locale, uiCatalog);
			} else if (onEditor) {
				if (editorScreen.draw(locale, editorNavigationCommand) == EditorScreenAction.ReturnToTitle)
					screen = closeEditor(screen);
			} else {
				Raylib.ClearBackground(CaxecraftPalette.sky());
				Raylib.BeginMode3D(camera);
				drawWorldSun();
				#if caxecraft_render_benchmark
				final terrainStarted = Raylib.GetTime();
				#end
				final renderCounters = terrainRenderer.draw(session.worldView(), terrainTexture, terrainTextureReady, adventureTerrainTexture,
					adventureTerrainTextureReady, renderPosition.x, renderPosition.z);
				#if caxecraft_render_benchmark
				if (frameCount >= 2) {
					measuredTerrainMicroseconds += Std.int((Raylib.GetTime() - terrainStarted) * 1000000.0);
					measuredPreparationMicroseconds += renderCounters.preparationMicroseconds;
					measuredTerrainFrames++;
				}
				#end
				final waterCounters = waterRenderer.draw(session.worldView(), terrainTexture, terrainTextureReady, levelView.waterPresentationCell(), eyeX,
					eyeY, eyeZ);
				final totalVisible = renderCounters.visible + waterCounters.visible;
				final totalDrawCalls = renderCounters.drawCalls + waterCounters.drawCalls;
				#if caxecraft_pilot
				visibleBlocks = totalVisible;
				terrainDrawCalls = totalDrawCalls;
				visibleTerrainFaces = renderCounters.faces;
				rebuiltTerrainChunks = renderCounters.rebuiltChunks;
				totalRebuiltTerrainChunks = renderCounters.totalRebuiltChunks;
				terrainCacheValid = renderCounters.cacheValid;
				#end
				drawActors(camera, entityTexture, entityTextureReady, runtimeTextures, dialogueActors, levelView, enemyActor,
					levelView.enemyActorPresentationAsset(), levelView.enemyActorPresentationCell(), enemyPhase.phase, berryDrop);
				drawStatefulObjects(contentRegistry, session, levelView, camera, entityTexture, entityTextureReady, itemTexture, itemTextureReady,
					adventureItemTexture, adventureItemTextureReady, terrainTexture, terrainTextureReady, runtimeTextures);
				AuthoredItemRenderer.drawWorldItems(contentRegistry, camera, session.authoredItemsView(), levelView, itemTexture, itemTextureReady,
					adventureItemTexture, adventureItemTextureReady);
				if (hit.hit)
					Raylib.DrawCubeWires(Vector3.fromFloat(hit.cellX + 0.5, hit.cellY + 0.5, hit.cellZ + 0.5), c.Float32.fromFloat(1.04),
						c.Float32.fromFloat(1.04), c.Float32.fromFloat(1.04), CaxecraftPalette.selection());
				Raylib.EndMode3D();
				if (cameraWaterBlend > 0.0) {
					final overlayAlpha = Std.int(105.0 * cameraWaterBlend);
					Raylib.DrawRectangle(0, 0, Raylib.GetScreenWidth(), Raylib.GetScreenHeight(), CaxecraftPalette.underwaterOverlay(overlayAlpha));
				}
				final hudView:HudView = {
					metrics: {
						visibleBlocks: totalVisible,
						drawCalls: totalDrawCalls,
						renderedFrames: frameCount,
						completedTicks: completedTicks
					},
					character: {
						x: character.body.x,
						z: character.body.z,
						vitals: character.vitals,
						aquaticEquipmentCode: aquaticEquipmentCode,
						aquaticEquipmentVisible: aquaticEquipmentFrames > 0,
						headSubmerged: character.aquatic.headSubmerged,
						breathTicks: character.aquatic.breathTicks,
						maximumBreathTicks: character.aquaticProfile.maximumBreathTicks
					},
					feedback: {
						placementBlocked: placementBlockedFrames > 0,
						strikeHit: strikeHitFrames > 0,
						enemyDefeated: enemyDefeatedFrames > 0,
						enemyAttacked: enemyAttackFrames > 0,
						pickedUp: pickupFrames > 0,
						pickupAmount: pickupAmount,
						inventoryFullReason: inventoryFullReason,
						recoveryDecision: recoveryFeedback,
						recoveryVisible: recoveryFeedbackFrames > 0
					},
					paused: paused,
					pointerCaptured: captured,
					hit: hit,
					mode: selectedMode,
					locale: locale,
					inventory: inventory,
					activeDialogue: activeDialogue,
					interactionPrompt: availableInteractionPrompt,
					enemy: enemyActor,
					enemyPhase: enemyPhase.phase,
					levelLabel: levelLabel,
					objectiveTitle: levelView.objectiveTitle(currentObjectiveId, scenarioLocale(locale)),
					journalTitle: latestJournalId == null ? "" : levelView.presentation().journalTitle(latestJournalId, scenarioLocale(locale)),
					journalBody: latestJournalId == null ? "" : levelView.presentation().journalBody(latestJournalId, scenarioLocale(locale)),
					presentation: levelView.presentation()
				};
				drawHud(hudView, hudResources, contentRegistry, uiCatalog);
			}
			#if caxecraft_pilot
			#if caxecraft_pilot_runtime
			final pilotComplete = agentSession ? false : runtimePilot.complete(frameCount);
			#else
			final pilotComplete = PilotScript.complete(pilotName, frameCount);
			#end
			#if caxecraft_render_benchmark
			if (pilotComplete)
				drawPilotTelemetry(pilotName, pilotInputHash, frameCount + 1, completedTicks, character.body, session.worldView(), hit, removedBlocks,
					placedBlocks, rejectedEdits, visibleBlocks, terrainDrawCalls, character.vitals.health, inventory.selected, activeDialogue != null,
					!characterIsDefeated(enemyActor.vitals), onTitle, onEditor, paused, captured, aquaticEquipmentCode >= 0, interpolationObserved,
					reviewScreenshotObserved, submersionObserved, waterExitObserved, sandMinedObserved, flowRuleObserved, objectiveChangeObserved,
					visibleTerrainFaces, rebuiltTerrainChunks, totalRebuiltTerrainChunks, terrainCacheValid, measuredTerrainMicroseconds,
					measuredTerrainFrames, measuredUpdateMicroseconds, measuredPreparationMicroseconds, activeLevel.generationId().value(),
					activeLevel.publicationCount());
			#else
			if (pilotComplete)
				drawPilotTelemetry(pilotName, pilotInputHash, frameCount + 1, completedTicks, character.body, session.worldView(), hit, removedBlocks,
					placedBlocks, rejectedEdits, visibleBlocks, terrainDrawCalls, character.vitals.health, inventory.selected, activeDialogue != null,
					!characterIsDefeated(enemyActor.vitals), onTitle, onEditor, paused, captured, aquaticEquipmentCode >= 0, interpolationObserved,
					reviewScreenshotObserved, submersionObserved, waterExitObserved, sandMinedObserved, flowRuleObserved, objectiveChangeObserved,
					visibleTerrainFaces, rebuiltTerrainChunks, totalRebuiltTerrainChunks, terrainCacheValid, 0, 0, 0, 0, activeLevel.generationId().value(),
					activeLevel.publicationCount());
			#end
			var capturePilotFrame = pilotComplete;
			if ((pilotName == PilotScriptName.LaunchSmoke && frameCount == 1)
				|| (pilotName == PilotScriptName.MoveJumpEdit && frameCount == 12)
				|| (pilotName == PilotScriptName.PauseRecapture && frameCount == 4)
				|| (pilotName == PilotScriptName.CombatDrop && frameCount == 38)
				|| (pilotName == PilotScriptName.RecoveryUse && frameCount == 2)
				|| (pilotName == PilotScriptName.FullInventoryMining && frameCount == 5)
				|| (pilotName == PilotScriptName.ResizeLayout && frameCount == 3)
				|| (pilotName == PilotScriptName.AquaticGear && frameCount == 146)
				|| (pilotName == PilotScriptName.SmoothMotion && frameCount == 10)
				|| (pilotName == PilotScriptName.EditorShell && frameCount == 2)
				|| (pilotName == PilotScriptName.CampaignTravel && frameCount == 3))
				capturePilotFrame = true;
			#if caxecraft_pilot_runtime
			final runtimeCheckpoint = runtimePilot.checkpointAt(activeRuntimePilotFrame);
			final agentResponseReady = agentSession && !agentWaiting && runtimePilot.complete(activeRuntimePilotFrame + 1);
			if (runtimeCheckpoint != null && runtimePilotFrameAccepted)
				capturePilotFrame = true;
			if (agentResponseReady && runtimePilotFrameAccepted)
				capturePilotFrame = true;
			#end
			// Submit this frame before reading it. `EndDrawing()` would otherwise
			// swap the buffers first, causing Raylib's screenshot function to read
			// the previous frame on a double-buffered desktop window.
			if (capturePilotFrame)
				Rlgl.FlushBatch();
			if (pilotName == PilotScriptName.LaunchSmoke && frameCount == 1) {
				#if caxecraft_pilot_secondary_locale
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-secondary-locale.png");
				#else
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-smoke.png");
				#end
			}
			if (pilotName == PilotScriptName.MoveJumpEdit && frameCount == 12)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-move.png");
			if (pilotName == PilotScriptName.PauseRecapture && frameCount == 4)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-pause.png");
			if (pilotName == PilotScriptName.CombatDrop && frameCount == 38)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-combat.png");
			if (pilotName == PilotScriptName.RecoveryUse && frameCount == 2)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-recovery.png");
			if (pilotName == PilotScriptName.FullInventoryMining && frameCount == 5)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-full-mining.png");
			if (pilotName == PilotScriptName.ResizeLayout && frameCount == 3)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-resize.png");
			if (pilotName == PilotScriptName.AquaticGear && frameCount == 146)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-aquatic-gear.png");
			if (pilotName == PilotScriptName.SmoothMotion && frameCount == 10)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-smooth-motion.png");
			if (pilotName == PilotScriptName.EditorShell && frameCount == 2)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-editor.png");
			if (pilotName == PilotScriptName.CampaignTravel && frameCount == 3)
				reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-campaign-travel.png");
			#if caxecraft_pilot_runtime
			if ((runtimeCheckpoint != null || agentResponseReady) && runtimePilotFrameAccepted) {
				final observationScreenshot = agentResponseReady ? "caxecraft-agent-session.png" : runtimeCheckpointScreenshot(runtimeCheckpoint.label);
				if (agentResponseReady)
					reviewScreenshotObserved = capturePilotScreenshot("caxecraft-agent-session.png");
				else if (runtimeCheckpoint.label == "title-selection")
					reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-runtime-title-selection.png");
				else if (runtimeCheckpoint.label == "campaign-selection")
					reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-runtime-campaign-selection.png");
				else if (runtimeCheckpoint.label == "level-selection")
					reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-runtime-level-selection.png");
				else
					reviewScreenshotObserved = capturePilotScreenshot("caxecraft-pilot-runtime-final.png");
				final recentEvents:Array<String> = [];
				if (placementBlockedFrames > 0)
					recentEvents.push("placement-blocked");
				if (strikeHitFrames > 0)
					recentEvents.push("strike-hit");
				if (enemyDefeatedFrames > 0)
					recentEvents.push("enemy-defeated");
				if (enemyAttackFrames > 0)
					recentEvents.push("player-attacked");
				if (pickupFrames > 0)
					recentEvents.push("item-collected");
				if (recoveryFeedbackFrames > 0)
					recentEvents.push("recovery-used");
				final interaction = switch nearestAvailableInteraction(session, levelView) {
					case NoAvailableInteraction: "none";
					case DialogueInteraction(id): "talk:" + authoredDialogueId(levelView, id);
					case StatefulObjectInteraction(id): "use:" + id.text();
				};
				final playerCellX = Std.int(character.body.x);
				final playerCellY = Std.int(character.body.y);
				final playerCellZ = Std.int(character.body.z);
				Sys.println("CAXECRAFT_AGENT_OBSERVATION=" + renderAgentWorldObservation({
					sequence: agentResponseReady ? agentObservationSequence + 1 : frameCount + 1,
					frame: frameCount,
					tick: completedTicks,
					screen: observedScreen,
					mode: observedMode,
					level: observedLevel,
					objective: observedObjective,
					dialogue: observedDialogue,
					journal: observedJournal,
					interaction: interaction,
					aquaticMedium: observedMedium,
					aquaticEquipment: observedEquipment,
					position: {
						xMilli: Std.int(character.body.x * 1000.0),
						yMilli: Std.int(character.body.y * 1000.0),
						zMilli: Std.int(character.body.z * 1000.0),
						cellX: playerCellX,
						cellY: playerCellY,
						cellZ: playerCellZ
					},
					heading: {xMilli: Std.int(lookX * 1000.0), yMilli: Std.int(lookY * 1000.0), zMilli: Std.int(lookZ * 1000.0)},
					vitals: {
						health: character.vitals.health,
						safeTicks: character.vitals.safeTicks,
						breathTicks: character.aquatic.breathTicks,
						maximumBreathTicks: character.aquaticProfile.maximumBreathTicks
					},
					inventory: agentInventory(inventory),
					target: agentTarget(hit, session.worldView()),
					nearby: agentNearby(session, levelView, character),
					terrainRadius: runtimePilot.inspectionRadius(),
					terrain: agentTerrain(session.worldView(), playerCellX, playerCellZ, runtimePilot.inspectionRadius()),
					events: recentEvents,
					screenshot: reviewScreenshotObserved ? observationScreenshot : "none"
				}));
			}
			if (agentSession && !agentWaiting) {
				if (agentResponseReady) {
					agentObservationSequence++;
					agentWaiting = true;
				}
				runtimePilotFrame++;
			}
			#end
			if (pilotComplete)
				Raylib.TakeScreenshot("caxecraft-pilot-state.png");
			#end
			Raylib.EndDrawing();
			if (onLoading)
				loadingFramePresented = true;
			frameCount++;
		}

		Raylib.EnableCursor();
		runtimeTextures.unload();
		if (adventureTerrainTextureReady)
			CaxecraftTextures.unload(adventureTerrainTexture);
		if (terrainTextureReady)
			CaxecraftTextures.unload(terrainTexture);
		if (entityTextureReady)
			CaxecraftTextures.unload(entityTexture);
		if (adventureItemTextureReady)
			CaxecraftTextures.unload(adventureItemTexture);
		if (itemTextureReady)
			CaxecraftTextures.unload(itemTexture);
		if (hudTextureReady)
			CaxecraftTextures.unload(hudTexture);
		if (wordmarkTextureReady)
			CaxecraftTextures.unload(wordmarkTexture);
		if (titleTextureReady)
			CaxecraftTextures.unload(titleTexture);
		Raylib.CloseWindow();
	}

	/**
		Draw the daylight source at one fixed world position.

		The old sun used screen coordinates, so mouse look could not move it. This
		world-space sphere now obeys the same camera transform as terrain. Its
		north-east position also matches the brighter terrain faces.
	**/
	static function drawWorldSun():Void {
		final center = Vector3.fromFloat(52.0, 18.0, -38.0);
		Raylib.DrawSphere(center, c.Float32.fromFloat(3.4), CaxecraftPalette.sunCore());
		Raylib.DrawSphere(center, c.Float32.fromFloat(4.8), CaxecraftPalette.sunGlow());
	}

	/** Turn one closed campaign-load failure into a path-and-stage console hint. */
	static function campaignLevelLoadFailure(error:CampaignLevelLoadError):String {
		return switch error {
			case CampaignLevelSourceRejected(path, _): "source read failed for " + path;
			case CampaignLevelLengthMismatch(path, expected, actual):
				"length changed for "
				+ path
				+ " (expected "
				+ Std.string(expected)
				+ ", found "
				+ Std.string(actual)
				+ ")";
			case CampaignLevelHashMismatch(path, _): "SHA-256 receipt changed for " + path;
			case CampaignLevelRuntimeRejected(path, _): "runtime validation failed for " + path;
		};
	}

	#if caxecraft_pilot

	#if caxecraft_pilot_runtime
	/** Fingerprint one request so one malformed file reports only once. */
	static function agentRequestBytesHash(bytes:Bytes):Int {
		var hash = -2128831035;
		for (index in 0...bytes.length)
			hash = (hash ^ bytes.get(index)) * 16777619;
		return hash;
	}

	/** Select the fixed review image that belongs to one admitted checkpoint. */
	static function runtimeCheckpointScreenshot(label:String):String {
		if (label == "title-selection")
			return "caxecraft-pilot-runtime-title-selection.png";
		if (label == "campaign-selection")
			return "caxecraft-pilot-runtime-campaign-selection.png";
		if (label == "level-selection")
			return "caxecraft-pilot-runtime-level-selection.png";
		return "caxecraft-pilot-runtime-final.png";
	}

	/** Recover the authored dialogue ID that matches one validated runtime ID. */
	static function authoredDialogueId(level:PlayableLevelView, entityId:EntityId):String {
		for (index in 0...level.dialogueActorCount())
			if (level.dialogueActorIdAt(index) == entityId)
				return level.dialogueActorAuthoredIdAt(index).text();
		return "missing";
	}
	#end

	/**
	 * Capture one flushed review frame and immediately observe its published file.
	 *
	 * Raylib's screenshot function has no return value. Its `FileExists` query is
	 * therefore the smallest native success observation available without adding
	 * a second filesystem runtime to the game. The runner deletes stale evidence
	 * before launch, so `true` can only describe this bounded process run.
	 */
	static inline function capturePilotScreenshot(fileName:c.CString):Bool {
		Raylib.TakeScreenshot(fileName);
		return Raylib.FileExists(fileName);
	}
	#end

	/** Restore the validated authored spawn, then recover if later edits blocked it. */
	static function spawnPlayer(cells:WorldView, transform:ScenarioTransform):CharacterBody {
		final spawnX = transform.xMilli / 1000.0;
		final spawnY = transform.yMilli / 1000.0;
		final spawnZ = transform.zMilli / 1000.0;
		return recoverPlayerSpawn(cells, createPlayer(spawnX, spawnY, spawnZ));
	}

	/**
		Find one controller phase in a copy-owned session observation.

		`valid` stays separate from the fallback phase so a missing actor cannot be
		drawn as a plausible state. The caller treats that mismatch as an ownership
		failure and exits the frame cleanly.
	**/
	static function observeActorPhase(states:Array<ActorControllerState>, id:EntityId, fallback:ActorControllerPhase):ActorPhaseObservation {
		for (state in states)
			if (state.characterId == id)
				return {valid: true, phase: state.phase};
		return {valid: false, phase: fallback};
	}

	/** Select the nearest available semantic target, with published order as the tie-break. */
	static function nearestAvailableInteraction(session:GameSession, level:PlayableLevelView):AvailableInteractionTarget {
		final view = session.view();
		if (!view.valid)
			return NoAvailableInteraction;
		final player = view.localPlayer;
		var selected:AvailableInteractionTarget = NoAvailableInteraction;
		var hasSelection = false;
		var selectedDistance = 0.0;
		for (index in 0...level.dialogueActorCount()) {
			final id = level.dialogueActorIdAt(index);
			if (session.actorInteractionAvailable(id)) {
				final actor = session.readCharacter(id);
				final dx = actor.body.x - player.body.x;
				final dz = actor.body.z - player.body.z;
				final distance = dx * dx + dz * dz;
				if (!hasSelection || distance < selectedDistance) {
					selected = DialogueInteraction(id);
					hasSelection = true;
					selectedDistance = distance;
				}
			}
		}
		for (index in 0...level.statefulObjectCount()) {
			final id = level.statefulObjectIdAt(index);
			if (session.statefulObjectInteractionAvailable(id)) {
				final transform = level.statefulObjectTransformAt(index);
				final dx = transform.xMilli / 1000.0 - player.body.x;
				final dz = transform.zMilli / 1000.0 - player.body.z;
				final distance = dx * dx + dz * dz;
				if (!hasSelection || distance < selectedDistance) {
					selected = StatefulObjectInteraction(id);
					hasSelection = true;
					selectedDistance = distance;
				}
			}
		}
		return selected;
	}

	/** Reduce one identity-bearing target to the prompt meaning needed by the HUD. */
	static function promptForAvailableInteraction(target:AvailableInteractionTarget):InteractionPrompt
		return interactionPrompt(switch target {
			case NoAvailableInteraction: InteractionTargetKind.NoInteractionTarget;
			case DialogueInteraction(_): InteractionTargetKind.DialogueInteractionTarget;
			case StatefulObjectInteraction(_): InteractionTargetKind.MechanismInteractionTarget;
		});

	/**
		Make sure that each published dialogue actor still has matching session state.

		A level transition replaces the view and session together. This check stops
		presentation if those owners disagree instead of hiding a missing actor.
	**/
	static function dialogueActorsAreValid(session:GameSession, level:PlayableLevelView, states:Array<ActorControllerState>):Bool {
		for (index in 0...level.dialogueActorCount()) {
			final id = level.dialogueActorIdAt(index);
			if (!session.readCharacter(id).id.isValid() || !observeActorPhase(states, id, ActorControllerPhase.Stationary).valid)
				return false;
		}
		return true;
	}

	/** Copy the committed dialogue characters before presentation starts. */
	static function dialogueActorSnapshots(session:GameSession, level:PlayableLevelView):Array<Character> {
		final actors:Array<Character> = [];
		for (index in 0...level.dialogueActorCount())
			actors.push(session.readCharacter(level.dialogueActorIdAt(index)));
		return actors;
	}

	/**
	 * Draw one honest synchronous campaign handoff between two playable maps.
	 *
	 * The campaign level ID comes from the validated reloadable manifest. The card
	 * deliberately has no progress bar because the next frame performs one bounded
	 * checked load rather than an asynchronous operation with measurable progress.
	 */
	static function drawCampaignLoading(destinationLabel:String, locale:LocaleCursor, catalog:RuntimeUiCatalog):Void {
		final width = Raylib.GetScreenWidth();
		final height = Raylib.GetScreenHeight();
		final panelWidth = 460;
		final panelHeight = 150;
		final panelX = Std.int((width - panelWidth) / 2);
		final panelY = Std.int((height - panelHeight) / 2);
		Raylib.ClearBackground(CaxecraftPalette.sky());
		Raylib.DrawRectangle(panelX, panelY, panelWidth, panelHeight, CaxecraftPalette.hudPanel());
		Raylib.DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, CaxecraftPalette.selection());
		drawUiText(catalog, locale, UiMessage.MenuAdventure, panelX + 28, panelY + 24, 22, CaxecraftPalette.selection());
		Raylib.DrawTextString(destinationLabel, panelX + 28, panelY + 76, 30, CaxecraftPalette.hudText());
	}

	/**
		Draw committed actor observations without advancing or reconstructing them.

		`GameSession` owns all characters and the enemy controller phase. This
		presentation helper reads the published dialogue list for one frame; it cannot
		change movement, health, or controller timing.
	**/
	static function drawActors(camera:Camera3D, entityTexture:Texture2D, entityTextureReady:Bool, runtimeTextures:RuntimeTextureAtlasCatalog,
			dialogueActors:Array<Character>, level:PlayableLevelView, enemy:Character, enemyAsset:String, enemyCell:Int, enemyPhase:ActorControllerPhase,
			berryDrop:BerryDropState):Void {
		for (index in 0...level.dialogueActorCount()) {
			final actor = dialogueActors[index];
			final position = Vector3.fromFloat(actor.body.x, actor.body.y + 0.76, actor.body.z);
			if (!drawActorPresentation(camera, level.dialogueActorPresentationAssetAt(index), level.dialogueActorPresentationCellAt(index), position, 0.95,
				1.52, entityTexture, entityTextureReady, runtimeTextures)) {
				Raylib.DrawCube(Vector3.fromFloat(actor.body.x, actor.body.y + 0.54, actor.body.z), c.Float32.fromFloat(0.50), c.Float32.fromFloat(0.86),
					c.Float32.fromFloat(0.42), CaxecraftPalette.selection());
				Raylib.DrawCube(Vector3.fromFloat(actor.body.x, actor.body.y + 1.18, actor.body.z), c.Float32.fromFloat(0.44), c.Float32.fromFloat(0.44),
					c.Float32.fromFloat(0.44), CaxecraftPalette.hudText());
				Raylib.DrawCube(Vector3.fromFloat(actor.body.x, actor.body.y + 1.41, actor.body.z), c.Float32.fromFloat(0.48), c.Float32.fromFloat(0.16),
					c.Float32.fromFloat(0.48), CaxecraftPalette.hudPanel());
			}
		}
		if (!characterIsDefeated(enemy.vitals)) {
			if (!drawActorPresentation(camera, enemyAsset, enemyCell, Vector3.fromFloat(enemy.body.x, enemy.body.y + 0.48, enemy.body.z), 1.05, 0.96,
				entityTexture, entityTextureReady, runtimeTextures)) {
				Raylib.DrawCube(Vector3.fromFloat(enemy.body.x, enemy.body.y + 0.30, enemy.body.z), c.Float32.fromFloat(0.70), c.Float32.fromFloat(0.54),
					c.Float32.fromFloat(0.70), CaxecraftPalette.damage());
				Raylib.DrawCube(Vector3.fromFloat(enemy.body.x, enemy.body.y + 0.66, enemy.body.z), c.Float32.fromFloat(0.50), c.Float32.fromFloat(0.34),
					c.Float32.fromFloat(0.50), CaxecraftPalette.selection());
			}
			if (enemyPhase == ActorControllerPhase.Windup)
				Raylib.DrawCube(Vector3.fromFloat(enemy.body.x, enemy.body.y + 1.02, enemy.body.z), c.Float32.fromFloat(0.20), c.Float32.fromFloat(0.20),
					c.Float32.fromFloat(0.20), CaxecraftPalette.damage());
		}
		if (berryDrop.active) {
			Raylib.DrawCube(Vector3.fromFloat(berryDrop.x - 0.12, berryDrop.y, berryDrop.z), c.Float32.fromFloat(0.18), c.Float32.fromFloat(0.18),
				c.Float32.fromFloat(0.18), CaxecraftPalette.berry());
			Raylib.DrawCube(Vector3.fromFloat(berryDrop.x + 0.12, berryDrop.y, berryDrop.z), c.Float32.fromFloat(0.18), c.Float32.fromFloat(0.18),
				c.Float32.fromFloat(0.18), CaxecraftPalette.berry());
		}
	}

	/** Route one validated actor visual to its owning legacy or reloadable atlas. */
	static function drawActorPresentation(camera:Camera3D, asset:String, cellIndex:Int, position:Vector3, width:Float, height:Float, entityTexture:Texture2D,
			entityTextureReady:Bool, runtimeTextures:RuntimeTextureAtlasCatalog):Bool {
		if (asset == "entities" && entityTextureReady) {
			CaxecraftAtlas.drawEntitySprite(camera, entityTexture, cellIndex, position, width, height);
			return true;
		}
		return runtimeTextures.drawSprite(camera, asset, cellIndex, position, width, height);
	}

	/** Draw one immutable post-simulation HUD snapshot using borrowed textures. */
	static function drawHud(view:HudView, resources:HudResources, contentRegistry:caxecraft.content.RuntimeContentPack.RuntimeContentRegistry,
			uiCatalog:RuntimeUiCatalog):Void {
		final visible = view.metrics.visibleBlocks;
		final drawCalls = view.metrics.drawCalls;
		final frames = view.metrics.renderedFrames;
		final updates = view.metrics.completedTicks;
		final paused = view.paused;
		final captured = view.pointerCaptured;
		final placementBlocked = view.feedback.placementBlocked;
		final hit = view.hit;
		final mode = view.mode;
		final locale = view.locale;
		final inventory = view.inventory;
		final activeDialogue = view.activeDialogue;
		final availableInteractionPrompt = view.interactionPrompt;
		final enemy = view.enemy;
		final enemyPhase = view.enemyPhase;
		final vitals = view.character.vitals;
		final strikeHit = view.feedback.strikeHit;
		final enemyDefeated = view.feedback.enemyDefeated;
		final enemyAttacked = view.feedback.enemyAttacked;
		final pickedUp = view.feedback.pickedUp;
		final pickupAmount = view.feedback.pickupAmount;
		final inventoryFullReason = view.feedback.inventoryFullReason;
		final recoveryFeedback = view.feedback.recoveryDecision;
		final recoveryVisible = view.feedback.recoveryVisible;
		final hudTexture = resources.hudTexture;
		final hudTextureReady = resources.hudTextureReady;
		final itemTexture = resources.itemTexture;
		final itemTextureReady = resources.itemTextureReady;
		final adventureItemTexture = resources.adventureItemTexture;
		final adventureItemTextureReady = resources.adventureItemTextureReady;
		final aquaticEquipmentCode = view.character.aquaticEquipmentCode;
		final aquaticEquipmentVisible = view.character.aquaticEquipmentVisible;
		final headSubmerged = view.character.headSubmerged;
		final breathTicks = view.character.breathTicks;
		final maximumBreathTicks = view.character.maximumBreathTicks;
		final presentation = view.presentation;
		final width = Raylib.GetScreenWidth();
		final height = Raylib.GetScreenHeight();
		final centerX = Std.int(width / 2);
		final centerY = Std.int(height / 2);
		final text = CaxecraftPalette.hudText();
		Raylib.DrawLine(centerX - 8, centerY, centerX - 3, centerY, text);
		Raylib.DrawLine(centerX + 3, centerY, centerX + 8, centerY, text);
		Raylib.DrawLine(centerX, centerY - 8, centerX, centerY - 3, text);
		Raylib.DrawLine(centerX, centerY + 3, centerX, centerY + 8, text);
		Raylib.DrawRectangle(18, 18, 460, 108, CaxecraftPalette.hudPanel());
		Raylib.DrawRectangleLines(18, 18, 460, 108, CaxecraftPalette.selection());
		drawUiText(uiCatalog, locale, UiMessage.Brand, 32, 28, 20, text);
		// This runtime identity makes a campaign handoff visible without baking a
		// level name into the executable or the static localization catalog.
		Raylib.DrawTextString(view.levelLabel, 250, 30, 16, CaxecraftPalette.selection());
		drawUiText(uiCatalog, locale, UiMessage.DebugCells, 32, 58, 14, text);
		HudDigits.drawNumber(World.VOLUME, 82, 59, 5, CaxecraftPalette.selection());
		drawUiText(uiCatalog, locale, UiMessage.DebugVisible, 160, 58, 14, text);
		HudDigits.drawNumber(visible, 230, 59, 5, CaxecraftPalette.selection());
		drawUiText(uiCatalog, locale, UiMessage.DebugDraws, 326, 58, 14, text);
		HudDigits.drawNumber(drawCalls, 382, 59, 5, CaxecraftPalette.selection());
		drawUiText(uiCatalog, locale, UiMessage.DebugFrame, 32, 86, 12, text);
		HudDigits.drawNumber(frames, 82, 85, 6, text);
		drawUiText(uiCatalog, locale, UiMessage.DebugTick, 174, 86, 12, text);
		HudDigits.drawNumber(updates, 216, 85, 6, text);
		drawHotbar(inventory, hudTexture, hudTextureReady, itemTexture, itemTextureReady, width, height);
		drawHealth(vitals, hudTexture, hudTextureReady, width);
		if (aquaticEquipmentCode >= 0)
			AuthoredItemRenderer.drawEquippedIcon(contentRegistry, aquaticEquipmentCode, itemTexture, itemTextureReady, adventureItemTexture,
				adventureItemTextureReady, width - 226, 18, 42);
		if (headSubmerged)
			drawBreath(breathTicks, maximumBreathTicks, width, height);
		drawUiText(uiCatalog, locale, UiMessage.Controls, 20, height - 22, 14, text);
		if (mode == GameMode.Adventure && view.objectiveTitle.length > 0)
			Raylib.DrawTextString(view.objectiveTitle, 32, 110, 14, CaxecraftPalette.selection());
		if (!paused && activeDialogue != null) {
			Raylib.DrawRectangle(centerX - 260, centerY + 54, 520, 60, CaxecraftPalette.hudPanel());
			Raylib.DrawTextString(presentation.dialogueLine(activeDialogue, 0, scenarioLocale(locale)), centerX - 225, centerY + 74, 16, text);
		} else if (!paused && availableInteractionPrompt != InteractionPrompt.NoInteractionPrompt) {
			Raylib.DrawRectangle(centerX - 260, centerY + 54, 520, 60, CaxecraftPalette.hudPanel());
			final prompt = switch availableInteractionPrompt {
				case TalkInteractionPrompt: GameplayMessage.GuideTalk;
				case UseInteractionPrompt: GameplayMessage.ObjectUse;
				case NoInteractionPrompt: GameplayMessage.GuideTalk;
			};
			drawScenarioText(presentation, locale, prompt, centerX - 110, centerY + 74, 18, text);
		}
		if (!characterIsDefeated(enemy.vitals)) {
			if (enemyPhase == ActorControllerPhase.Windup)
				drawScenarioText(presentation, locale, GameplayMessage.EnemyWindup, width - 300, 28, 16, CaxecraftPalette.damage());
			else if (enemyPhase == ActorControllerPhase.Chasing)
				drawScenarioText(presentation, locale, GameplayMessage.EnemyAlert, width - 180, 28, 16, CaxecraftPalette.selection());
		}
		if (strikeHit)
			drawScenarioText(presentation, locale, GameplayMessage.AttackHit, centerX - 70, centerY - 54, 18, CaxecraftPalette.selection());
		if (enemyDefeated)
			drawScenarioText(presentation, locale, GameplayMessage.EnemyDroppedItems, width - 285, 54, 16, CaxecraftPalette.selection());
		if (enemyAttacked)
			drawScenarioText(presentation, locale, GameplayMessage.EnemyHitWarning, width - 330, 82, 16, CaxecraftPalette.damage());
		if (pickedUp) {
			final pickupMessage = pickupAmount == 1 ? GameplayMessage.PickupOne : GameplayMessage.PickupMany;
			drawScenarioText(presentation, locale, pickupMessage, centerX - 48, centerY + 24, 18, CaxecraftPalette.berry());
		}
		if (inventoryFullReason == InventoryFullReason.BerryStack)
			drawScenarioText(presentation, locale, GameplayMessage.BerryStackFull, centerX - 150, centerY + 48, 16, CaxecraftPalette.inventoryFull());
		else if (inventoryFullReason == InventoryFullReason.BlockStack)
			drawScenarioText(presentation, locale, GameplayMessage.BlockStackFull, centerX - 155, centerY + 48, 16, CaxecraftPalette.inventoryFull());
		if (recoveryVisible) {
			if (recoveryFeedback == RecoveryDecision.UseBerries)
				drawScenarioText(presentation, locale, GameplayMessage.RecoveryUsed, centerX - 88, centerY + 24, 18, CaxecraftPalette.recovery());
			else if (recoveryFeedback == RecoveryDecision.HealthAlreadyFull)
				drawUiText(uiCatalog, locale, UiMessage.HealthFull, centerX - 96, centerY + 24, 18, CaxecraftPalette.selection());
			else if (recoveryFeedback == RecoveryDecision.RecoveryStackEmpty)
				drawScenarioText(presentation, locale, GameplayMessage.RecoveryEmpty, centerX - 76, centerY + 24, 18, CaxecraftPalette.selection());
		}
		if (aquaticEquipmentVisible)
			drawUiText(uiCatalog, locale, UiMessage.AquaticGearEquipped, centerX - 128, centerY + 24, 18, CaxecraftPalette.selection());
		if (vitals.safeTicks > 15)
			Raylib.DrawRectangleLines(4, 4, width - 8, height - 8, CaxecraftPalette.damage());
		if (characterIsDefeated(vitals)) {
			Raylib.DrawRectangle(centerX - 250, centerY - 74, 500, 148, CaxecraftPalette.hudPanel());
			Raylib.DrawRectangleLines(centerX - 250, centerY - 74, 500, 148, CaxecraftPalette.damage());
			drawScenarioText(presentation, locale, GameplayMessage.PlayerFallen, centerX - 122, centerY - 42, 24, text);
			drawScenarioText(presentation, locale, GameplayMessage.ReturnPrompt, centerX - 125, centerY + 10, 18, CaxecraftPalette.selection());
		}
		if (paused) {
			final hasJournal = view.journalTitle.length > 0 || view.journalBody.length > 0;
			final panelX = hasJournal ? centerX - 330 : centerX - 170;
			final panelY = hasJournal ? centerY - 110 : centerY - 48;
			final panelWidth = hasJournal ? 660 : 340;
			final panelHeight = hasJournal ? 220 : 96;
			Raylib.DrawRectangle(panelX, panelY, panelWidth, panelHeight, CaxecraftPalette.hudPanel());
			Raylib.DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, CaxecraftPalette.selection());
			drawUiText(uiCatalog, locale, UiMessage.PauseTitle, centerX - 48, panelY + 18, 24, text);
			if (hasJournal) {
				Raylib.DrawTextString(view.journalTitle, panelX + 30, panelY + 68, 18, CaxecraftPalette.selection());
				drawWrappedText(view.journalBody, panelX + 30, panelY + 104, 16, 58, 22, 3, text);
			}
			drawUiText(uiCatalog, locale, UiMessage.PauseHelp, centerX - 160, panelY + panelHeight - 34, 16, text);
		} else if (placementBlocked) {
			drawUiText(uiCatalog, locale, UiMessage.PlaceBlocked, centerX - 170, centerY + 26, 14, CaxecraftPalette.selection());
		} else if (!captured) {
			drawUiText(uiCatalog, locale, UiMessage.CapturePrompt, centerX - 90, centerY + 26, 14, text);
		} else if (!hit.hit) {
			drawUiText(uiCatalog, locale, UiMessage.NoBlockInReach, centerX - 105, centerY + 26, 14, text);
		}
	}

	/**
	 * Draw a short authored paragraph inside a fixed panel without clipping words.
	 *
	 * The caller supplies a conservative character width and line limit selected
	 * for its panel. This is presentation-only wrapping: it does not change or
	 * truncate the validated journal text stored by the active level.
	 */
	static function drawWrappedText(value:String, x:Int, y:Int, fontSize:Int, maximumCharacters:Int, lineHeight:Int, maximumLines:Int, color:Color):Void {
		final words = value.split(" ");
		var line = "";
		var lineIndex = 0;
		for (word in words) {
			final candidate = line.length == 0 ? word : line + " " + word;
			if (line.length > 0 && candidate.length > maximumCharacters) {
				if (lineIndex >= maximumLines)
					return;
				Raylib.DrawTextString(line, x, y + lineIndex * lineHeight, fontSize, color);
				lineIndex++;
				line = word;
			} else {
				line = candidate;
			}
		}
		if (line.length > 0 && lineIndex < maximumLines)
			Raylib.DrawTextString(line, x, y + lineIndex * lineHeight, fontSize, color);
	}

	/** Keep native drawing in the UI layer; the catalog only chooses text. */
	static inline function drawUiText(catalog:RuntimeUiCatalog, locale:LocaleCursor, message:UiMessage, x:Int, y:Int, fontSize:Int, color:Color):Void
		Raylib.DrawTextString(catalog.text(locale, message), x, y, fontSize, color);

	/** Draw one gameplay notice from the active map's validated runtime catalog. */
	static inline function drawScenarioText(presentation:caxecraft.content.RuntimeLevelLoader.RuntimeLevelPresentation, locale:LocaleCursor,
			message:GameplayMessage, x:Int, y:Int, fontSize:Int, color:Color):Void
		Raylib.DrawTextString(presentation.message(gameplayMessageId(message), scenarioLocale(locale)), x, y, fontSize, color);

	/** Map the closed UI locale cursor to the spelling used by CAXEMAP catalogs. */
	static inline function scenarioLocale(locale:LocaleCursor):LocaleId
		return switch locale {
			case Locale0: new LocaleId("en");
			case Locale1: new LocaleId("es-mx");
			case _: new LocaleId("en");
		};

	/** Draw ten bubbles from deterministic fixed-tick breath, with no text. */
	static function drawBreath(breathTicks:Int, maximumBreathTicks:Int, width:Int, height:Int):Void {
		final bubbleCount = 10;
		var filled = 0;
		if (maximumBreathTicks > 0)
			filled = Std.int((breathTicks * bubbleCount + maximumBreathTicks - 1) / maximumBreathTicks);
		if (filled < 0)
			filled = 0;
		if (filled > bubbleCount)
			filled = bubbleCount;
		final startX = Std.int((width - (bubbleCount * 18 - 4)) / 2);
		final y = height - 128;
		var bubble = 0;
		while (bubble < bubbleCount) {
			if (bubble < filled)
				Raylib.DrawCircle(startX + bubble * 18, y, c.Float32.fromFloat(6.0), CaxecraftPalette.breathFull());
			else
				Raylib.DrawCircle(startX + bubble * 18, y, c.Float32.fromFloat(6.0), CaxecraftPalette.breathEmpty());
			bubble++;
		}
	}

	/** Draw three whole/half/empty hearts from the reviewed HUD atlas. */
	static function drawHealth(vitals:VitalsState, hudTexture:Texture2D, hudTextureReady:Bool, width:Int):Void {
		var heart = 0;
		while (heart < 3) {
			final x = width - 170 + heart * 50;
			final points = vitals.health - heart * 2;
			if (hudTextureReady) {
				if (points >= 2)
					CaxecraftAtlas.drawHudGlyph(hudTexture, HudGlyph.HealthFull, x, 18, 42);
				else if (points == 1)
					CaxecraftAtlas.drawHudGlyph(hudTexture, HudGlyph.HealthHalf, x, 18, 42);
				else
					CaxecraftAtlas.drawHudGlyph(hudTexture, HudGlyph.HealthEmpty, x, 18, 42);
			} else if (points > 0)
				Raylib.DrawRectangle(x, 22, 34, 26, CaxecraftPalette.damage());
			else
				Raylib.DrawRectangleLines(x, 22, 34, 26, CaxecraftPalette.hudText());
			heart++;
		}
	}

	/** Draw the bounded inventory directly from two reviewed 4x4 source atlases. */
	static function drawHotbar(inventory:InventoryState, hudTexture:Texture2D, hudTextureReady:Bool, itemTexture:Texture2D, itemTextureReady:Bool, width:Int,
			height:Int):Void {
		final slotSize = 64;
		final gap = 4;
		final totalWidth = Inventory.SLOT_COUNT * slotSize + (Inventory.SLOT_COUNT - 1) * gap;
		final startX = Std.int((width - totalWidth) / 2);
		final y = height - 96;
		var slot = 0;
		while (slot < Inventory.SLOT_COUNT) {
			final x = startX + slot * (slotSize + gap);
			if (hudTextureReady) {
				if (slot == inventory.selected)
					CaxecraftAtlas.drawHotbarFrame(hudTexture, HotbarFrame.Selected, x, y, slotSize);
				else
					CaxecraftAtlas.drawHotbarFrame(hudTexture, HotbarFrame.Normal, x, y, slotSize);
			} else {
				Raylib.DrawRectangle(x, y, slotSize, slotSize, CaxecraftPalette.hudPanel());
				Raylib.DrawRectangleLines(x, y, slotSize, slotSize, slot == inventory.selected ? CaxecraftPalette.selection() : CaxecraftPalette.hudText());
			}
			if (itemTextureReady)
				CaxecraftAtlas.drawItem(itemTexture, Inventory.itemAt(slot), x + 6, y + 4, slotSize - 12);
			HudDigits.drawNumber(Inventory.countAt(inventory, slot), x + 39, y + 44, 2, CaxecraftPalette.hudText());
			slot++;
		}
	}
}
#end

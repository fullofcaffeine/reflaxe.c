package caxecraft.app;

#if c
import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.ItemUseProfile;
import caxecraft.content.FirstPlayableLevel.itemCount;
import caxecraft.content.FirstPlayableLevel.itemStorageCode;
import caxecraft.content.FirstPlayableLevel.itemXMilli;
import caxecraft.content.FirstPlayableLevel.itemYMilli;
import caxecraft.content.FirstPlayableLevel.itemZMilli;
import caxecraft.content.FirstPlayableLevel.spawnXMilli;
import caxecraft.content.FirstPlayableLevel.spawnYMilli;
import caxecraft.content.FirstPlayableLevel.spawnZMilli;
import caxecraft.content.FirstPlayableSessionLoader.loadCandidate as loadFirstPlayableSession;
import caxecraft.app.AppScreen;
import caxecraft.app.AppScreen.capturesPointer as screenCapturesPointer;
import caxecraft.app.AppScreen.closeEditor;
import caxecraft.app.AppScreen.initialScreen;
import caxecraft.app.AppScreen.isPlaying as screenIsPlaying;
import caxecraft.app.AppScreen.loseFocus as pauseAfterFocusLoss;
import caxecraft.app.AppScreen.openEditor;
import caxecraft.app.AppScreen.pausesSimulation as screenPausesSimulation;
import caxecraft.app.AppScreen.recapture as recapturePointer;
import caxecraft.app.AppScreen.showsEditor as screenShowsEditor;
import caxecraft.app.AppScreen.showsTitle as screenShowsTitle;
import caxecraft.app.AppScreen.startPlaying;
import caxecraft.app.AppScreen.togglePause;
import caxecraft.app.CaxecraftEditorScreen.EditorScreenAction;
import caxecraft.app.MotionInterpolation.advance as advanceMotion;
import caxecraft.app.MotionInterpolation.reset as resetMotion;
import caxecraft.app.MotionInterpolation.sample as sampleMotion;
import caxecraft.app.MotionInterpolation.start as startMotion;
import caxecraft.domain.Character.adoptProfile as adoptCharacterProfile;
import caxecraft.domain.Character.applyAttack as applyCharacterAttack;
import caxecraft.domain.Character.reviveAt as reviveCharacterAt;
import caxecraft.domain.Character.start as startCharacter;
import caxecraft.domain.Character.withVitals as withCharacterVitals;
import caxecraft.domain.CharacterDamagePolicy;
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
import caxecraft.domain.WorldCells;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.InventoryFullReason;
import caxecraft.gameplay.InventoryState;
import caxecraft.app.CaxecraftAtlas.HotbarFrame;
import caxecraft.app.CaxecraftAtlas.HudGlyph;
import caxecraft.app.CaxecraftAtlas.WorldSprite;
import caxecraft.gameplay.BerryDrop.collectAmount as collectBerryDropAmount;
import caxecraft.gameplay.BerryDrop.fromDefeatedMossling as berryDropFromDefeatedMossling;
import caxecraft.gameplay.BerryDrop.isInRange as berryDropIsInRange;
import caxecraft.gameplay.BerryDrop.none as emptyBerryDrop;
import caxecraft.gameplay.BerryDropState;
import caxecraft.gameplay.GuideNpc;
import caxecraft.gameplay.GuidePhase;
import caxecraft.gameplay.GuideState;
import caxecraft.gameplay.ItemKind;
import caxecraft.gameplay.Mossling;
import caxecraft.gameplay.MosslingMode;
import caxecraft.gameplay.MosslingState;
import caxecraft.gameplay.Mining.attempt as attemptMining;
import caxecraft.gameplay.MiningOutcome;
import caxecraft.domain.VitalsState;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.domain.Vitals.isDefeated as characterIsDefeated;
import caxecraft.gameplay.Recovery.applyInventory as applyRecoveryInventory;
import caxecraft.gameplay.Recovery.applyVitals as applyRecoveryVitals;
import caxecraft.gameplay.Recovery.decide as decideRecovery;
import caxecraft.gameplay.RecoveryDecision;
import caxecraft.gameplay.SwordCombat;
import caxecraft.gameplay.SwordCombatDecision;
import caxecraft.gameplay.SwordCombatState;
import caxecraft.gameplay.WorldItemPickup.isInRange as authoredItemIsInRange;
import caxecraft.localization.FirstPlayableCatalog;
import caxecraft.localization.FirstPlayableCatalog.ScenarioMessage;
import caxecraft.localization.UiCatalog;
import caxecraft.localization.UiCatalog.LocaleCursor;
import caxecraft.localization.UiCatalog.UiMessage;
#if caxecraft_pilot
import caxecraft.app.PilotTelemetry.drawPilotTelemetry;
import raylib.Rlgl;
#end
import caxecraft.pilot.GameInputFrame;
import caxecraft.pilot.PilotScript;
import caxecraft.pilot.PilotScript.PilotScriptName;
import raylib.Camera3D;
import raylib.CameraProjection;
import raylib.Color;
import raylib.ConfigFlags;
import raylib.KeyboardKey;
import raylib.MouseButton;
import raylib.Raylib;
import raylib.Texture2D;
import raylib.Vector3;

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

	/** The one loaded simulation owned for this application's lifetime. */
	final session:GameSession = new GameSession();

	/** Persistent terrain faces rebuilt only after successful world edits. */
	final terrainRenderer:TerrainRenderer = new TerrainRenderer();

	/** Native visual editor shell over the shared renderer-independent session. */
	final editorScreen:CaxecraftEditorScreen;

	/**
	 * Create the application and its single simulation owner.
	 *
	 * Haxe.c embeds this final child directly in the generated application struct;
	 * neither object needs a heap allocation. Native presentation resources are
	 * acquired later by `run`, after the candidate level validates.
	 */
	public function new() {
		editorScreen = new CaxecraftEditorScreen();
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
		// Construct and validate a candidate before treating it as the live session.
		// The temporary generated level bridge receives the session itself, not its
		// buffers, so content cannot keep or mutate a raw storage view.
		final loadedLevel = loadFirstPlayableSession(session);
		if (!loadedLevel.valid)
			return;
		final waterPresentationCell = loadedLevel.waterPresentationCell;

		// These friend views are now presentation/interaction migration seams only.
		// Level assembly above creates its own short-lived views inside GameSession.
		// `GameView` will remove the remaining reads in the presentation task.
		final cells = session.worldStorage.span();
		final pendingCells = session.pendingWaterStorage.span();
		final itemActive = session.authoredItemStorage.span();
		final itemActiveRead = session.authoredItemStorage.constSpan();
		final inactiveItem = 0;
		final initialAquaticProfile = BaseContentPack.aquaticProfile(BaseContentPack.defaultAquaticProfile());
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
		#elseif caxecraft_pilot_full_inventory_gift
		final pilotName:PilotScriptName = PilotScriptName.FullInventoryGift;
		#elseif caxecraft_pilot_full_inventory_mining
		final pilotName:PilotScriptName = PilotScriptName.FullInventoryMining;
		#elseif caxecraft_pilot_resize_layout
		final pilotName:PilotScriptName = PilotScriptName.ResizeLayout;
		#elseif caxecraft_pilot_aquatic_gear
		final pilotName:PilotScriptName = PilotScriptName.AquaticGear;
		#elseif caxecraft_pilot_smooth_motion
		final pilotName:PilotScriptName = PilotScriptName.SmoothMotion;
		#end
		var initialHealth = MAX_HEALTH;
		#if caxecraft_pilot
		// Only the deterministic provider may select fixture state. The release
		// build contains neither this branch nor a way to alter starting health.
		initialHealth = PilotScript.initialHealth(pilotName);
		#end
		if (!session.bindLocalPlayer(startCharacter(EntityId.fromValidatedStorageCode(1), spawnPlayer(cells), initialAquaticProfile, initialHealth)))
			return;

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
		var cameraWaterBlend = 0.0;
		var inventory:InventoryState = Inventory.starter();
		#if caxecraft_pilot
		// Only the deterministic provider may replace the ordinary starter kit.
		// This keeps test setup out of release gameplay and lets native pilots
		// exercise the same typed inventory transitions as a real player.
		inventory = PilotScript.initialInventory(pilotName);
		#end
		var guide:GuideState = GuideNpc.start(cells, 17.5, 13.5);
		var mossling:MosslingState = Mossling.start(cells, 15.5, 13.8);
		var swordCombat:SwordCombatState = SwordCombat.start();
		var berryDrop:BerryDropState = emptyBerryDrop();
		var lookX = 0.0;
		var lookY = -0.18;
		var lookZ = -1.0;
		// Real frame time collects here until there is enough for one or more
		// authoritative fixed simulation steps. The remainder selects a visual
		// position between the last two committed bodies; it never changes gameplay.
		var accumulator = 0.0;
		final initialPresentation = session.view();
		if (!initialPresentation.valid)
			return;
		var motionHistory = startMotion(initialPresentation.localPlayer.body);
		var jumpQueued = false;
		var swordQueued = false;
		var selectedMode:GameMode = GameMode.Creative;
		#if (caxecraft_pilot_move_jump_edit
			|| caxecraft_pilot_combat_drop
			|| caxecraft_pilot_recovery_use
			|| caxecraft_pilot_full_inventory_gift
			|| caxecraft_pilot_full_inventory_mining
			|| caxecraft_pilot_aquatic_gear)
		// A deterministic provider choice, not gameplay branching: this pilot
		// exercises finite Adventure inventory and actor behavior from frame one.
		selectedMode = GameMode.Adventure;
		#end
		var locale:LocaleCursor = UiCatalog.defaultLocale();
		#if caxecraft_pilot_secondary_locale
		// The graphical locale pilot selects the next validated catalog without
		// teaching the application which human language that catalog contains.
		locale = UiCatalog.nextLocale(locale);
		#end
		#if caxecraft_pilot
		final showInitialTitle = pilotName == PilotScriptName.LaunchSmoke || pilotName == PilotScriptName.ResizeLayout;
		#else
		final showInitialTitle = true;
		#end
		var screen = initialScreen(showInitialTitle);
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
			// Work on one immutable view for this frame, then commit gameplay changes
			// through the temporary migration seam below. Presentation never receives
			// the mutable session or its entity store.
			final initialView = session.view();
			if (!initialView.valid)
				quit = true;
			var character = initialView.localPlayer;
			var recapturedThisFrame = false;
			// Discontinuous application transitions discard old visual motion at the
			// frame's final committed body. This prevents interpolation through walls,
			// across a pause, or from a defeated position back to the authored spawn.
			var resetMotionThisFrame = false;
			#if caxecraft_pilot
			final requestedWindowWidth = PilotScript.requestedWindowWidth(pilotName, frameCount);
			// Keep both conditional results stable before the native call. haxe_c-af1
			// owns the compiler fix that will make this explicit local unnecessary.
			final requestedWindowHeight = PilotScript.requestedWindowHeight(pilotName, frameCount);
			if (requestedWindowWidth > 0)
				Raylib.SetWindowSize(requestedWindowWidth, requestedWindowHeight);
			final focused = true;
			final pilotAction = PilotScript.actionAt(pilotName, frameCount);
			final moveForward = PilotScript.moveForward(pilotAction);
			final moveRight = PilotScript.moveRight(pilotAction);
			final lookYaw = PilotScript.lookYaw(pilotAction);
			final lookPitch = PilotScript.lookPitch(pilotAction);
			final jumpPressed = PilotScript.jumpPressed(pilotAction);
			final primaryPressed = PilotScript.primaryPressed(pilotAction);
			final secondaryPressed = PilotScript.secondaryPressed(pilotAction);
			final interactPressed = PilotScript.interactPressed(pilotAction);
			final pausePressed = PilotScript.pausePressed(pilotAction);
			final capturePressed = PilotScript.capturePressed(pilotAction);
			final quitPressed = PilotScript.quitPressed(pilotAction);
			final hotbarSelection = PilotScript.hotbarSelection(pilotAction);
			final hotbarCycle = PilotScript.hotbarCycle(pilotAction);
			// Existing deterministic scripts do not request downward swimming yet.
			// A dedicated water pilot will own that authored action when added.
			final descendHeld = false;
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
			final primaryPressed = frameInput.primaryPressed;
			final secondaryPressed = frameInput.secondaryPressed;
			final interactPressed = frameInput.interactPressed;
			final pausePressed = frameInput.pausePressed;
			final capturePressed = frameInput.capturePressed;
			final quitPressed = frameInput.quitPressed;
			final hotbarSelection = frameInput.hotbarSelection;
			final hotbarCycle = frameInput.hotbarCycle;
			final descendHeld = frameInput.descendHeld;
			#end
			if (quitPressed)
				quit = true;
			if (hotbarSelection >= 0)
				inventory = Inventory.select(inventory, hotbarSelection);
			if (hotbarCycle != 0)
				inventory = Inventory.cycle(inventory, hotbarCycle);
			if (screenIsPlaying(screen) && interactPressed) {
				if (characterIsDefeated(character.vitals)) {
					character = reviveCharacterAt(character, spawnPlayer(cells));
					cameraWaterBlend = 0.0;
					accumulator = 0.0;
					resetMotionThisFrame = true;
				} else if (GuideNpc.isInRange(guide, character.body.x, character.body.z)) {
					final sharesBerries = GuideNpc.sharesBerriesOnNextInteraction(guide);
					if (sharesBerries) {
						final acceptedGift = Inventory.acceptedAmount(inventory, ItemKind.Berries, 2);
						if (acceptedGift == 2) {
							inventory = Inventory.collectItem(inventory, ItemKind.Berries, acceptedGift);
							guide = GuideNpc.interact(guide);
						} else {
							inventoryFullReason = InventoryFullReason.BerryStack;
							inventoryFullFrames = 90;
						}
					} else {
						guide = GuideNpc.interact(guide);
					}
				}
			}

			if (screenShowsTitle(screen) && focused) {
				#if !caxecraft_pilot
				final modeBeforeInput = selectedMode;
				if (Raylib.IsKeyPressed(KeyboardKey.L))
					locale = UiCatalog.nextLocale(locale);
				if (Raylib.IsKeyPressed(KeyboardKey.Up) || Raylib.IsKeyPressed(KeyboardKey.Down))
					selectedMode = selectedMode == GameMode.Creative ? GameMode.Adventure : GameMode.Creative;

				final menuMouse = Raylib.GetMousePosition();
				final hovered = TitleMenu.selectionAt(menuMouse.x.toFloat(), menuMouse.y.toFloat(), Raylib.GetScreenWidth(), Raylib.GetScreenHeight());
				if (hovered == 0)
					selectedMode = GameMode.Creative;
				if (hovered == 1)
					selectedMode = GameMode.Adventure;
				if (selectedMode != modeBeforeInput) {
					accumulator = 0.0;
					resetMotionThisFrame = true;
				}
				final clickedChoice = hovered >= 0 && Raylib.IsMouseButtonPressed(MouseButton.Left);
				if (clickedChoice && hovered == 2) {
					screen = openEditor(screen);
					accumulator = 0.0;
					resetMotionThisFrame = true;
					Raylib.EnableCursor();
				} else if (clickedChoice || Raylib.IsKeyPressed(KeyboardKey.Enter)) {
					screen = startPlaying(screen);
					recapturedThisFrame = true;
					accumulator = 0.0;
					resetMotionThisFrame = true;
					Raylib.DisableCursor();
				}
				#end
			}

			if (!focused && screenIsPlaying(screen)) {
				screen = pauseAfterFocusLoss(screen);
				jumpQueued = false;
				accumulator = 0.0;
				resetMotionThisFrame = true;
				Raylib.EnableCursor();
			}
			if (screenShowsEditor(screen) && focused && pausePressed) {
				screen = closeEditor(screen);
				accumulator = 0.0;
				resetMotionThisFrame = true;
				Raylib.EnableCursor();
			} else if (!screenShowsTitle(screen) && focused && pausePressed) {
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
			if (!screenShowsTitle(screen) && !screenShowsEditor(screen) && focused && screenPausesSimulation(screen) && capturePressed) {
				screen = recapturePointer(screen);
				recapturedThisFrame = true;
				accumulator = 0.0;
				resetMotionThisFrame = true;
				Raylib.DisableCursor();
			}

			// These are read-only projections of one closed state, not independent
			// flags. No screen transition occurs after this point in the frame.
			final onTitle = screenShowsTitle(screen);
			final onEditor = screenShowsEditor(screen);
			final paused = screenPausesSimulation(screen);
			final captured = screenCapturesPointer(screen);
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

			#if caxecraft_pilot
			// Ordinary pilots supply one exact 50 ms step per frame. SmoothMotion
			// supplies a deterministic display cadence that crosses those fixed-tick
			// boundaries with a remainder. Interactive builds use Raylib's clock.
			var frameSeconds = PilotScript.frameDurationMilliseconds(pilotName, frameCount) / 1000.0;
			#else
			var frameSeconds = Raylib.GetFrameTime().toFloat();
			#end
			if (frameSeconds > MAX_FRAME_SECONDS)
				frameSeconds = MAX_FRAME_SECONDS;
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
			while (!paused && accumulator >= FIXED_SECONDS) {
				// Water owns a bounded amount of work per game tick. A large leak
				// therefore continues over later ticks without freezing a frame.
				var moveX = moveForward * lookX - moveRight * lookZ;
				var moveZ = moveForward * lookZ + moveRight * lookX;
				if (moveForward != 0.0 && moveRight != 0.0) {
					moveX *= 0.7071067811865476;
					moveZ *= 0.7071067811865476;
				}
				final damagePolicy = selectedMode == GameMode.Adventure ? CharacterDamagePolicy.Survival : CharacterDamagePolicy.Invulnerable;
				final gameTick = session.tick({
					intent: aquaticInput(moveX, moveZ, jumpQueued, descendHeld),
					damagePolicy: damagePolicy,
					waterUpdateBudget: 64
				});
				character = gameTick.character;
				if (gameTick.committed)
					motionHistory = advanceMotion(motionHistory, character.body);
				cameraWaterBlend = gameTick.immersion.cameraBlend;
				if (!gameTick.committed)
					quit = true;
				if (!characterIsDefeated(character.vitals)) {
					var pickupIndex = 0;
					while (pickupIndex < itemCount()) {
						if (itemActive[pickupIndex] != inactiveItem
							&& authoredItemIsInRange(character.body.x, character.body.y, character.body.z, itemXMilli(pickupIndex), itemYMilli(pickupIndex),
								itemZMilli(pickupIndex))) {
							final itemCode = itemStorageCode(pickupIndex);
							final item = BaseContentPack.itemFromValidatedStorageCode(itemCode);
							if (BaseContentPack.itemUseProfile(item) == ItemUseProfile.EquipAquatic
								&& BaseContentPack.itemProvidesAquaticProfile(item)) {
								final replacement = BaseContentPack.aquaticProfile(BaseContentPack.itemAquaticProfile(item));
								character = adoptCharacterProfile(character, replacement);
								aquaticEquipmentCode = itemCode;
								aquaticEquipmentFrames = 120;
								itemActive[pickupIndex] = inactiveItem;
							}
						}
						pickupIndex++;
					}
				}
				if (selectedMode == GameMode.Adventure) {
					if (!characterIsDefeated(character.vitals)) {
						final mosslingAttacked = Mossling.attacksThisTick(mossling, character.body.x, character.body.z);
						character = applyCharacterAttack(character, mosslingAttacked);
						if (mosslingAttacked)
							enemyAttackFrames = 120;
						mossling = Mossling.step(cells, mossling, character.body.x, character.body.z, gameTick.tickIndex);
					}
					swordCombat = SwordCombat.step(swordCombat);
					if (swordQueued) {
						final swordDecision = SwordCombat.decide(swordCombat, inventory, character.vitals, mossling, character.body.x, character.body.z,
							lookX, lookZ);
						if (swordDecision == SwordCombatDecision.Hit) {
							mossling = Mossling.strike(mossling);
							strikeHitFrames = 16;
							if (!Mossling.isAlive(mossling)) {
								berryDrop = berryDropFromDefeatedMossling(mossling);
								enemyDefeatedFrames = 120;
							}
						}
						swordCombat = SwordCombat.after(swordDecision, swordCombat);
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
			final committedView = session.view();
			if (!committedView.valid)
				quit = true;
			final completedTicks = committedView.completedTicks;

			// Selection is authoritative gameplay: it originates at the latest committed
			// body, never at the presentation-only camera position below.
			final selectionEyeX = character.body.x;
			final selectionEyeY = character.body.y + 1.62;
			final selectionEyeZ = character.body.z;
			final hit = VoxelRaycast.trace(cells, selectionEyeX, selectionEyeY, selectionEyeZ, lookX, lookY, lookZ, PICK_DISTANCE);
			if (captured && !recapturedThisFrame && primaryPressed) {
				if (!characterIsDefeated(character.vitals)) {
					if (selectedMode == GameMode.Adventure) {
						if (!Inventory.selectedIs(inventory, ItemKind.CopperSword)
							&& hit.hit
							&& playerCanMine(character.aquatic, character.aquaticProfile)) {
							final minedCoordinate = World.coord(hit.cellX, hit.cellY, hit.cellZ);
							final mining = attemptMining(cells, minedCoordinate, inventory);
							inventory = mining.inventory;
							if (mining.outcome == MiningOutcome.Collected) {
								session.water.terrainChanged(pendingCells, minedCoordinate);
								terrainRenderer.invalidate(minedCoordinate);
							}
							#if caxecraft_pilot
							if (mining.outcome == MiningOutcome.Collected)
								removedBlocks++;
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
						final removed = session.water.removeTerrain(cells, pendingCells, removedCoordinate);
						if (removed)
							terrainRenderer.invalidate(removedCoordinate);
						#if caxecraft_pilot
						if (removed)
							removedBlocks++;
						#end
					}
				}
			}
			if (captured && secondaryPressed) {
				if (!characterIsDefeated(character.vitals)) {
					final recoveryDecision = decideRecovery(inventory, character.vitals);
					if (recoveryDecision != RecoveryDecision.NotRecoveryItem) {
						recoveryFeedback = recoveryDecision;
						recoveryFeedbackFrames = 90;
						inventory = applyRecoveryInventory(recoveryDecision, inventory);
						character = withCharacterVitals(character, applyRecoveryVitals(recoveryDecision, character.vitals));
					} else if (hit.hit) {
						final placement = World.coord(hit.previousX, hit.previousY, hit.previousZ);
						final selectedBlock = Inventory.selectedBlock(inventory);
						final hasItem = Inventory.countAt(inventory, inventory.selected) > 0;
						if (!hasItem
							|| !World.isPlaceable(selectedBlock)
							|| !playerCanPlaceAt(character.body, placement)
							|| !session.water.placeTerrain(cells, pendingCells, placement, selectedBlock)) {
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
			if (!paused) {
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
			// Publish the complete frame result together. A mismatched ID indicates an
			// engine ownership defect, so stop after this frame instead of overwriting
			// another character.
			if (!session.replaceLocalPlayer(character))
				quit = true;
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
				TitleMenu.draw(titleTexture, titleTextureReady, wordmarkTexture, wordmarkTextureReady, selectedMode, locale);
			} else if (onEditor) {
				if (editorScreen.draw(locale) == EditorScreenAction.ReturnToTitle)
					screen = closeEditor(screen);
			} else {
				Raylib.ClearBackground(CaxecraftPalette.sky());
				// The sun is a screen-space backdrop, so it remains calm and legible
				// while the camera moves. Terrain and actors are drawn over it below.
				final sunX = Raylib.GetScreenWidth() - 260;
				Raylib.DrawCircle(sunX, 86, c.Float32.fromFloat(42.0), CaxecraftPalette.sunGlow());
				Raylib.DrawCircle(sunX, 86, c.Float32.fromFloat(30.0), CaxecraftPalette.sunCore());
				Raylib.BeginMode3D(camera);
				#if caxecraft_render_benchmark
				final terrainStarted = Raylib.GetTime();
				#end
				final renderCounters = terrainRenderer.draw(cells, terrainTexture, terrainTextureReady, adventureTerrainTexture, adventureTerrainTextureReady,
					renderPosition.x, renderPosition.z);
				#if caxecraft_render_benchmark
				if (frameCount >= 2) {
					measuredTerrainMicroseconds += Std.int((Raylib.GetTime() - terrainStarted) * 1000000.0);
					measuredPreparationMicroseconds += renderCounters.preparationMicroseconds;
					measuredTerrainFrames++;
				}
				#end
				final waterCounters = WaterRenderer.draw(cells, terrainTexture, terrainTextureReady, waterPresentationCell);
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
				drawActors(camera, entityTexture, entityTextureReady, guide, mossling, berryDrop);
				AuthoredItemRenderer.drawWorldItems(camera, itemActiveRead, itemTexture, itemTextureReady, adventureItemTexture, adventureItemTextureReady);
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
					guide: guide,
					mossling: mossling
				};
				drawHud(hudView, hudResources);
			}
			#if caxecraft_pilot
			final pilotComplete = PilotScript.complete(pilotName, frameCount);
			#if caxecraft_render_benchmark
			if (pilotComplete)
				drawPilotTelemetry(pilotName, frameCount + 1, completedTicks, character.body, cells, hit, removedBlocks, placedBlocks, rejectedEdits,
					visibleBlocks, terrainDrawCalls, character.vitals.health, inventory.selected, GuideNpc.phase(guide), Mossling.isAlive(mossling), onTitle,
					paused, captured, aquaticEquipmentCode >= 0, interpolationObserved, visibleTerrainFaces, rebuiltTerrainChunks, totalRebuiltTerrainChunks,
					terrainCacheValid, measuredTerrainMicroseconds, measuredTerrainFrames, measuredUpdateMicroseconds, measuredPreparationMicroseconds);
			#else
			if (pilotComplete)
				drawPilotTelemetry(pilotName, frameCount + 1, completedTicks, character.body, cells, hit, removedBlocks, placedBlocks, rejectedEdits,
					visibleBlocks, terrainDrawCalls, character.vitals.health, inventory.selected, GuideNpc.phase(guide), Mossling.isAlive(mossling), onTitle,
					paused, captured, aquaticEquipmentCode >= 0, interpolationObserved, visibleTerrainFaces, rebuiltTerrainChunks, totalRebuiltTerrainChunks,
					terrainCacheValid, 0, 0, 0, 0);
			#end
			var capturePilotFrame = pilotComplete;
			if ((pilotName == PilotScriptName.LaunchSmoke && frameCount == 1)
				|| (pilotName == PilotScriptName.MoveJumpEdit && frameCount == 12)
				|| (pilotName == PilotScriptName.PauseRecapture && frameCount == 4)
				|| (pilotName == PilotScriptName.CombatDrop && frameCount == 38)
				|| (pilotName == PilotScriptName.RecoveryUse && frameCount == 2)
				|| (pilotName == PilotScriptName.FullInventoryGift && frameCount == 2)
				|| (pilotName == PilotScriptName.FullInventoryMining && frameCount == 5)
				|| (pilotName == PilotScriptName.ResizeLayout && frameCount == 3)
				|| (pilotName == PilotScriptName.AquaticGear && frameCount == 92)
				|| (pilotName == PilotScriptName.SmoothMotion && frameCount == 10))
				capturePilotFrame = true;
			// Submit this frame before reading it. `EndDrawing()` would otherwise
			// swap the buffers first, causing Raylib's screenshot function to read
			// the previous frame on a double-buffered desktop window.
			if (capturePilotFrame)
				Rlgl.FlushBatch();
			if (pilotName == PilotScriptName.LaunchSmoke && frameCount == 1)
				#if caxecraft_pilot_secondary_locale
				Raylib.TakeScreenshot("caxecraft-secondary-locale.png");
				#else
				Raylib.TakeScreenshot("caxecraft-smoke.png");
				#end
				if (pilotName == PilotScriptName.MoveJumpEdit && frameCount == 12)
					Raylib.TakeScreenshot("caxecraft-pilot-move.png");
			if (pilotName == PilotScriptName.PauseRecapture && frameCount == 4)
				Raylib.TakeScreenshot("caxecraft-pilot-pause.png");
			if (pilotName == PilotScriptName.CombatDrop && frameCount == 38)
				Raylib.TakeScreenshot("caxecraft-pilot-combat.png");
			if (pilotName == PilotScriptName.RecoveryUse && frameCount == 2)
				Raylib.TakeScreenshot("caxecraft-pilot-recovery.png");
			if (pilotName == PilotScriptName.FullInventoryGift && frameCount == 2)
				Raylib.TakeScreenshot("caxecraft-pilot-full-inventory.png");
			if (pilotName == PilotScriptName.FullInventoryMining && frameCount == 5)
				Raylib.TakeScreenshot("caxecraft-pilot-full-mining.png");
			if (pilotName == PilotScriptName.ResizeLayout && frameCount == 3)
				Raylib.TakeScreenshot("caxecraft-pilot-resize.png");
			if (pilotName == PilotScriptName.AquaticGear && frameCount == 92)
				Raylib.TakeScreenshot("caxecraft-pilot-aquatic-gear.png");
			if (pilotName == PilotScriptName.SmoothMotion && frameCount == 10)
				Raylib.TakeScreenshot("caxecraft-pilot-smooth-motion.png");
			if (pilotComplete)
				Raylib.TakeScreenshot("caxecraft-pilot-state.png");
			#end
			Raylib.EndDrawing();
			frameCount++;
		}

		Raylib.EnableCursor();
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

	/** Restore the validated authored spawn, then recover if later edits blocked it. */
	static function spawnPlayer(cells:WorldCells):CharacterBody {
		final spawnX = spawnXMilli() / 1000.0;
		final spawnY = spawnYMilli() / 1000.0;
		final spawnZ = spawnZMilli() / 1000.0;
		return recoverPlayerSpawn(cells, createPlayer(spawnX, spawnY, spawnZ));
	}

	/** Original atlas sprites with code-drawn fallbacks; actor rules remain in gameplay/. */
	static function drawActors(camera:Camera3D, entityTexture:Texture2D, entityTextureReady:Bool, guide:GuideState, mossling:MosslingState,
			berryDrop:BerryDropState):Void {
		if (entityTextureReady)
			CaxecraftAtlas.drawWorldSprite(camera, entityTexture, WorldSprite.NiaFront, Vector3.fromFloat(guide.x, guide.y + 0.76, guide.z), 0.95, 1.52);
		else {
			Raylib.DrawCube(Vector3.fromFloat(guide.x, guide.y + 0.54, guide.z), c.Float32.fromFloat(0.50), c.Float32.fromFloat(0.86),
				c.Float32.fromFloat(0.42), CaxecraftPalette.niaCoat());
			Raylib.DrawCube(Vector3.fromFloat(guide.x, guide.y + 1.18, guide.z), c.Float32.fromFloat(0.44), c.Float32.fromFloat(0.44),
				c.Float32.fromFloat(0.44), CaxecraftPalette.niaSkin());
			Raylib.DrawCube(Vector3.fromFloat(guide.x, guide.y + 1.41, guide.z), c.Float32.fromFloat(0.48), c.Float32.fromFloat(0.16),
				c.Float32.fromFloat(0.48), CaxecraftPalette.niaHair());
		}
		if (Mossling.isAlive(mossling)) {
			if (entityTextureReady)
				CaxecraftAtlas.drawWorldSprite(camera, entityTexture, WorldSprite.MosslingFront, Vector3.fromFloat(mossling.x, mossling.y + 0.48, mossling.z),
					1.05, 0.96);
			else {
				Raylib.DrawCube(Vector3.fromFloat(mossling.x, mossling.y + 0.30, mossling.z), c.Float32.fromFloat(0.70), c.Float32.fromFloat(0.54),
					c.Float32.fromFloat(0.70), CaxecraftPalette.mosslingBody());
				Raylib.DrawCube(Vector3.fromFloat(mossling.x, mossling.y + 0.66, mossling.z), c.Float32.fromFloat(0.50), c.Float32.fromFloat(0.34),
					c.Float32.fromFloat(0.50), CaxecraftPalette.mosslingCrown());
			}
			if (Mossling.mode(mossling) == MosslingMode.Windup)
				Raylib.DrawCube(Vector3.fromFloat(mossling.x, mossling.y + 1.02, mossling.z), c.Float32.fromFloat(0.20), c.Float32.fromFloat(0.20),
					c.Float32.fromFloat(0.20), CaxecraftPalette.damage());
		}
		if (berryDrop.active) {
			Raylib.DrawCube(Vector3.fromFloat(berryDrop.x - 0.12, berryDrop.y, berryDrop.z), c.Float32.fromFloat(0.18), c.Float32.fromFloat(0.18),
				c.Float32.fromFloat(0.18), CaxecraftPalette.berry());
			Raylib.DrawCube(Vector3.fromFloat(berryDrop.x + 0.12, berryDrop.y, berryDrop.z), c.Float32.fromFloat(0.18), c.Float32.fromFloat(0.18),
				c.Float32.fromFloat(0.18), CaxecraftPalette.berry());
		}
	}

	/** Draw one immutable post-simulation HUD snapshot using borrowed textures. */
	static function drawHud(view:HudView, resources:HudResources):Void {
		final visible = view.metrics.visibleBlocks;
		final drawCalls = view.metrics.drawCalls;
		final frames = view.metrics.renderedFrames;
		final updates = view.metrics.completedTicks;
		final paused = view.paused;
		final captured = view.pointerCaptured;
		final placementBlocked = view.feedback.placementBlocked;
		final hit = view.hit;
		final playerX = view.character.x;
		final playerZ = view.character.z;
		final mode = view.mode;
		final locale = view.locale;
		final inventory = view.inventory;
		final guide = view.guide;
		final mossling = view.mossling;
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
		drawUiText(locale, UiMessage.Brand, 32, 28, 20, text);
		drawUiText(locale, UiMessage.DebugCells, 32, 58, 14, text);
		HudDigits.drawNumber(World.VOLUME, 82, 59, 5, CaxecraftPalette.selection());
		drawUiText(locale, UiMessage.DebugVisible, 160, 58, 14, text);
		HudDigits.drawNumber(visible, 230, 59, 5, CaxecraftPalette.selection());
		drawUiText(locale, UiMessage.DebugDraws, 326, 58, 14, text);
		HudDigits.drawNumber(drawCalls, 382, 59, 5, CaxecraftPalette.selection());
		drawUiText(locale, UiMessage.DebugFrame, 32, 86, 12, text);
		HudDigits.drawNumber(frames, 82, 85, 6, text);
		drawUiText(locale, UiMessage.DebugTick, 174, 86, 12, text);
		HudDigits.drawNumber(updates, 216, 85, 6, text);
		drawHotbar(inventory, hudTexture, hudTextureReady, itemTexture, itemTextureReady, width, height);
		drawHealth(vitals, hudTexture, hudTextureReady, width);
		if (aquaticEquipmentCode >= 0)
			AuthoredItemRenderer.drawEquippedIcon(aquaticEquipmentCode, itemTexture, itemTextureReady, adventureItemTexture, adventureItemTextureReady,
				width - 226, 18, 42);
		if (headSubmerged)
			drawBreath(breathTicks, maximumBreathTicks, width, height);
		drawUiText(locale, UiMessage.Controls, 20, height - 22, 14, text);
		if (mode == GameMode.Adventure)
			drawScenarioText(locale, ScenarioMessage.AdventureProgress, 32, 110, 14, CaxecraftPalette.selection());
		if (GuideNpc.isInRange(guide, playerX, playerZ)) {
			Raylib.DrawRectangle(centerX - 260, centerY + 54, 520, 60, CaxecraftPalette.hudPanel());
			if (GuideNpc.phase(guide) == GuidePhase.Waiting)
				drawScenarioText(locale, ScenarioMessage.NiaTalk, centerX - 110, centerY + 74, 18, text);
			else if (GuideNpc.phase(guide) == GuidePhase.Welcomed)
				drawScenarioText(locale, ScenarioMessage.NiaWelcome, centerX - 225, centerY + 74, 16, text);
			else
				drawScenarioText(locale, ScenarioMessage.NiaGift, centerX - 205, centerY + 74, 16, text);
		}
		if (Mossling.isAlive(mossling)) {
			if (Mossling.mode(mossling) == MosslingMode.Windup)
				drawScenarioText(locale, ScenarioMessage.MosslingWindup, width - 300, 28, 16, CaxecraftPalette.damage());
			else if (Mossling.mode(mossling) == MosslingMode.Chasing)
				drawScenarioText(locale, ScenarioMessage.MosslingAlert, width - 180, 28, 16, CaxecraftPalette.selection());
		}
		if (strikeHit)
			drawScenarioText(locale, ScenarioMessage.CopperStrike, centerX - 70, centerY - 54, 18, CaxecraftPalette.selection());
		if (enemyDefeated)
			drawScenarioText(locale, ScenarioMessage.MosslingDroppedBerries, width - 285, 54, 16, CaxecraftPalette.selection());
		if (enemyAttacked)
			drawScenarioText(locale, ScenarioMessage.TelegraphedHit, width - 330, 82, 16, CaxecraftPalette.damage());
		if (pickedUp) {
			final pickupMessage = pickupAmount == 1 ? ScenarioMessage.BerryPickupOne : ScenarioMessage.BerryPickupTwo;
			drawScenarioText(locale, pickupMessage, centerX - 48, centerY + 24, 18, CaxecraftPalette.berry());
		}
		if (inventoryFullReason == InventoryFullReason.BerryStack)
			drawScenarioText(locale, ScenarioMessage.BerryStackFull, centerX - 150, centerY + 48, 16, CaxecraftPalette.inventoryFull());
		else if (inventoryFullReason == InventoryFullReason.BlockStack)
			drawScenarioText(locale, ScenarioMessage.BlockStackFull, centerX - 155, centerY + 48, 16, CaxecraftPalette.inventoryFull());
		if (recoveryVisible) {
			if (recoveryFeedback == RecoveryDecision.UseBerries)
				drawScenarioText(locale, ScenarioMessage.BerryRecovery, centerX - 88, centerY + 24, 18, CaxecraftPalette.recovery());
			else if (recoveryFeedback == RecoveryDecision.HealthAlreadyFull)
				drawUiText(locale, UiMessage.HealthFull, centerX - 96, centerY + 24, 18, CaxecraftPalette.selection());
			else if (recoveryFeedback == RecoveryDecision.RecoveryStackEmpty)
				drawScenarioText(locale, ScenarioMessage.NoBerries, centerX - 76, centerY + 24, 18, CaxecraftPalette.selection());
		}
		if (aquaticEquipmentVisible)
			drawUiText(locale, UiMessage.AquaticGearEquipped, centerX - 128, centerY + 24, 18, CaxecraftPalette.selection());
		if (vitals.safeTicks > 15)
			Raylib.DrawRectangleLines(4, 4, width - 8, height - 8, CaxecraftPalette.damage());
		if (characterIsDefeated(vitals)) {
			Raylib.DrawRectangle(centerX - 250, centerY - 74, 500, 148, CaxecraftPalette.hudPanel());
			Raylib.DrawRectangleLines(centerX - 250, centerY - 74, 500, 148, CaxecraftPalette.damage());
			drawScenarioText(locale, ScenarioMessage.HaxirioFallen, centerX - 122, centerY - 42, 24, text);
			drawScenarioText(locale, ScenarioMessage.ReturnToMeadow, centerX - 125, centerY + 10, 18, CaxecraftPalette.selection());
		}
		if (paused) {
			Raylib.DrawRectangle(centerX - 170, centerY - 48, 340, 96, CaxecraftPalette.hudPanel());
			Raylib.DrawRectangleLines(centerX - 170, centerY - 48, 340, 96, CaxecraftPalette.selection());
			drawUiText(locale, UiMessage.PauseTitle, centerX - 48, centerY - 30, 24, text);
			drawUiText(locale, UiMessage.PauseHelp, centerX - 160, centerY + 8, 16, text);
		} else if (placementBlocked) {
			drawUiText(locale, UiMessage.PlaceBlocked, centerX - 170, centerY + 26, 14, CaxecraftPalette.selection());
		} else if (!captured) {
			drawUiText(locale, UiMessage.CapturePrompt, centerX - 90, centerY + 26, 14, text);
		} else if (!hit.hit) {
			drawUiText(locale, UiMessage.NoBlockInReach, centerX - 105, centerY + 26, 14, text);
		}
	}

	/** Keep native drawing in the UI layer; the catalog only chooses text. */
	static inline function drawUiText(locale:LocaleCursor, message:UiMessage, x:Int, y:Int, fontSize:Int, color:Color):Void
		Raylib.DrawText(UiCatalog.text(locale, message), x, y, fontSize, color);

	/** Draw one campaign-owned message after its catalog resolves the locale. */
	static inline function drawScenarioText(locale:LocaleCursor, message:ScenarioMessage, x:Int, y:Int, fontSize:Int, color:Color):Void
		Raylib.DrawText(FirstPlayableCatalog.text(locale, message), x, y, fontSize, color);

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
				// Keep the aggregate-valued Color choice in the call branches. The
				// general compiler gap for aggregate conditional expressions is
				// tracked by haxe_c-djl.1 rather than hidden in this renderer.
				if (slot == inventory.selected)
					Raylib.DrawRectangleLines(x, y, slotSize, slotSize, CaxecraftPalette.selection());
				else
					Raylib.DrawRectangleLines(x, y, slotSize, slotSize, CaxecraftPalette.hudText());
			}
			if (itemTextureReady)
				CaxecraftAtlas.drawItem(itemTexture, Inventory.itemAt(slot), x + 6, y + 4, slotSize - 12);
			HudDigits.drawNumber(Inventory.countAt(inventory, slot), x + 39, y + 44, 2, CaxecraftPalette.hudText());
			slot++;
		}
	}
}
#end

package caxecraft.app;

#if c
import c.CArray;
import c.UInt8;
import caxecraft.domain.PlayerPhysics;
import caxecraft.domain.PlayerState;
import caxecraft.domain.RaycastHit;
import caxecraft.domain.VoxelRaycast;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.domain.WorldVolume;
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
import caxecraft.gameplay.PlayerVitals;
import caxecraft.gameplay.PlayerVitalsState;
import caxecraft.gameplay.Recovery.applyInventory as applyRecoveryInventory;
import caxecraft.gameplay.Recovery.applyVitals as applyRecoveryVitals;
import caxecraft.gameplay.Recovery.decide as decideRecovery;
import caxecraft.gameplay.RecoveryDecision;
import caxecraft.gameplay.SwordCombat;
import caxecraft.gameplay.SwordCombatDecision;
import caxecraft.gameplay.SwordCombatState;
import caxecraft.localization.FirstPlayableCatalog;
import caxecraft.localization.FirstPlayableCatalog.ScenarioMessage;
import caxecraft.localization.UiCatalog;
import caxecraft.localization.UiCatalog.LocaleCursor;
import caxecraft.localization.UiCatalog.UiMessage;
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

/** C-specific window/input/render adapter around the shared Caxecraft domain. */
final class Main {
	static inline final FIXED_SECONDS:Float = 0.05;
	static inline final MAX_FRAME_SECONDS:Float = 0.25;
	static inline final PICK_DISTANCE:Float = 7.0;
	static inline final SPAWN_X:Float = 16.5;
	static inline final SPAWN_Z:Float = 16.5;

	static function main():Void {
		var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		final cells:WorldCells = storage.span();
		World.generate(cells, 0x0cafe);
		World.prepareSpawnMeadow(cells);

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
		final entityTexture:Texture2D = CaxecraftTextures.loadEntityAtlas();
		final entityTextureReady = CaxecraftTextures.isValid(entityTexture);
		final terrainTexture:Texture2D = CaxecraftTextures.loadTerrainAtlas();
		final terrainTextureReady = CaxecraftTextures.isValid(terrainTexture);
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
		#end

		var player:PlayerState = spawnPlayer(cells);
		var inventory:InventoryState = Inventory.starter();
		#if caxecraft_pilot
		// Only the deterministic provider may replace the ordinary starter kit.
		// This keeps test setup out of release gameplay and lets native pilots
		// exercise the same typed inventory transitions as a real player.
		inventory = PilotScript.initialInventory(pilotName);
		#end
		var guide:GuideState = GuideNpc.start(cells, 17.5, 13.5);
		var mossling:MosslingState = Mossling.start(cells, 15.5, 13.8);
		var initialHealth = PlayerVitals.MAX_HEALTH;
		#if caxecraft_pilot
		// Only the deterministic provider may select fixture state. The release
		// build contains neither this branch nor a way to alter starting health.
		initialHealth = PilotScript.initialHealth(pilotName);
		#end
		var vitals:PlayerVitalsState = PlayerVitals.startAt(initialHealth);
		var swordCombat:SwordCombatState = SwordCombat.start();
		var berryDrop:BerryDropState = emptyBerryDrop();
		var lookX = 0.0;
		var lookY = -0.18;
		var lookZ = -1.0;
		var accumulator = 0.0;
		var jumpQueued = false;
		var swordQueued = false;
		var selectedMode:GameMode = GameMode.Creative;
		#if (caxecraft_pilot_move_jump_edit || caxecraft_pilot_combat_drop || caxecraft_pilot_recovery_use || caxecraft_pilot_full_inventory_gift
			|| caxecraft_pilot_full_inventory_mining)
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
		var onTitle = pilotName == PilotScriptName.LaunchSmoke || pilotName == PilotScriptName.ResizeLayout;
		var paused = onTitle;
		var captured = !onTitle;
		if (captured)
			Raylib.DisableCursor();
		#else
		var onTitle = true;
		var paused = true;
		var captured = false;
		#end
		var quit = false;
		var frameCount = 0;
		var updateCount = 0;
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

		while (!quit && !Raylib.WindowShouldClose()) {
			var recapturedThisFrame = false;
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
			#else
			final focused = Raylib.IsWindowFocused();
			final frameInput:GameInputFrame = RaylibGameInput.sample(captured, paused);
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
			#end
			if (quitPressed)
				quit = true;
			if (hotbarSelection >= 0)
				inventory = Inventory.select(inventory, hotbarSelection);
			if (hotbarCycle != 0)
				inventory = Inventory.cycle(inventory, hotbarCycle);
			if (!onTitle && !paused && interactPressed) {
				if (PlayerVitals.isDefeated(vitals)) {
					vitals = PlayerVitals.revive(vitals);
					player = spawnPlayer(cells);
				} else if (GuideNpc.isInRange(guide, player.x, player.z)) {
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

			if (onTitle && focused) {
				#if !caxecraft_pilot
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
				final clickedChoice = hovered >= 0 && Raylib.IsMouseButtonPressed(MouseButton.Left);
				if (clickedChoice || Raylib.IsKeyPressed(KeyboardKey.Enter)) {
					onTitle = false;
					paused = false;
					captured = true;
					recapturedThisFrame = true;
					Raylib.DisableCursor();
				}
				#end
			}

			if (!onTitle && !focused && captured) {
				paused = true;
				captured = false;
				jumpQueued = false;
				Raylib.EnableCursor();
			}
			if (!onTitle && focused && pausePressed) {
				paused = !paused;
				captured = !paused;
				if (paused)
					jumpQueued = false;
				if (captured)
					Raylib.DisableCursor();
				else
					Raylib.EnableCursor();
			}
			if (!onTitle && focused && paused && capturePressed) {
				paused = false;
				captured = true;
				recapturedThisFrame = true;
				Raylib.DisableCursor();
			}

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
			// A pilot supplies one exact 50 ms simulation step per rendered frame.
			// Interactive builds still use Raylib's measured elapsed time.
			var frameSeconds = FIXED_SECONDS;
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
			while (!paused && accumulator >= FIXED_SECONDS) {
				var moveX = moveForward * lookX - moveRight * lookZ;
				var moveZ = moveForward * lookZ + moveRight * lookX;
				if (moveForward != 0.0 && moveRight != 0.0) {
					moveX *= 0.7071067811865476;
					moveZ *= 0.7071067811865476;
				}
				if (!PlayerVitals.isDefeated(vitals))
					player = PlayerPhysics.step(cells, player, PlayerPhysics.input(moveX, moveZ, jumpQueued));
				if (selectedMode == GameMode.Adventure) {
					if (!PlayerVitals.isDefeated(vitals)) {
						vitals = PlayerVitals.step(vitals);
						final mosslingAttacked = Mossling.attacksThisTick(mossling, player.x, player.z);
						vitals = PlayerVitals.applyAttack(vitals, mosslingAttacked);
						if (mosslingAttacked)
							enemyAttackFrames = 120;
						mossling = Mossling.step(cells, mossling, player.x, player.z, updateCount);
					}
					swordCombat = SwordCombat.step(swordCombat);
					if (swordQueued) {
						final swordDecision = SwordCombat.decide(swordCombat, inventory, vitals, mossling, player.x, player.z, lookX, lookZ);
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
				updateCount++;
			}

			final eyeX = player.x;
			final eyeY = player.y + 1.62;
			final eyeZ = player.z;
			final hit = VoxelRaycast.trace(cells, eyeX, eyeY, eyeZ, lookX, lookY, lookZ, PICK_DISTANCE);
			if (captured && !recapturedThisFrame && primaryPressed) {
				if (!PlayerVitals.isDefeated(vitals)) {
					if (selectedMode == GameMode.Adventure) {
						if (!Inventory.selectedIs(inventory, ItemKind.CopperSword) && hit.hit) {
							final mining = attemptMining(cells, World.coord(hit.cellX, hit.cellY, hit.cellZ), inventory);
							inventory = mining.inventory;
							if (mining.outcome == MiningOutcome.InventoryFull) {
								inventoryFullReason = InventoryFullReason.BlockStack;
								inventoryFullFrames = 90;
							}
						}
					} else if (hit.hit) {
						World.remove(cells, World.coord(hit.cellX, hit.cellY, hit.cellZ));
					}
				}
			}
			if (captured && secondaryPressed) {
				if (!PlayerVitals.isDefeated(vitals)) {
					final recoveryDecision = decideRecovery(inventory, vitals);
					if (recoveryDecision != RecoveryDecision.NotRecoveryItem) {
						recoveryFeedback = recoveryDecision;
						recoveryFeedbackFrames = 90;
						inventory = applyRecoveryInventory(recoveryDecision, inventory);
						vitals = applyRecoveryVitals(recoveryDecision, vitals);
					} else if (hit.hit) {
						final placement = World.coord(hit.previousX, hit.previousY, hit.previousZ);
						final selectedBlock = Inventory.selectedBlock(inventory);
						final hasItem = Inventory.countAt(inventory, inventory.selected) > 0;
						if (!hasItem
							|| !World.isPlaceable(selectedBlock)
							|| !PlayerPhysics.canPlaceAt(player, placement)
							|| !World.place(cells, placement, selectedBlock))
							placementBlockedFrames = 60;
						else if (selectedMode == GameMode.Adventure)
							inventory = Inventory.consumeSelected(inventory);
					}
				}
			}
			if (placementBlockedFrames > 0)
				placementBlockedFrames--;
			if (!paused) {
				if (berryDropIsInRange(berryDrop, player.x, player.y, player.z)) {
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

			final camera = Camera3D.make(Vector3.fromFloat(eyeX, eyeY, eyeZ), Vector3.fromFloat(eyeX + lookX, eyeY + lookY, eyeZ + lookZ),
				Vector3.fromFloat(0.0, 1.0, 0.0), c.Float32.fromFloat(70.0), CameraProjection.Perspective);
			Raylib.BeginDrawing();
			if (onTitle) {
				TitleMenu.draw(titleTexture, titleTextureReady, wordmarkTexture, wordmarkTextureReady, selectedMode, locale);
			} else {
				Raylib.ClearBackground(CaxecraftPalette.sky());
				// The sun is a screen-space backdrop, so it remains calm and legible
				// while the camera moves. Terrain and actors are drawn over it below.
				final sunX = Raylib.GetScreenWidth() - 260;
				Raylib.DrawCircle(sunX, 86, c.Float32.fromFloat(42.0), CaxecraftPalette.sunGlow());
				Raylib.DrawCircle(sunX, 86, c.Float32.fromFloat(30.0), CaxecraftPalette.sunCore());
				Raylib.BeginMode3D(camera);
				final renderCounters = TerrainRenderer.draw(cells, terrainTexture, terrainTextureReady, player.x, player.z);
				drawActors(camera, entityTexture, entityTextureReady, guide, mossling, berryDrop);
				if (hit.hit)
					Raylib.DrawCubeWires(Vector3.fromFloat(hit.cellX + 0.5, hit.cellY + 0.5, hit.cellZ + 0.5), c.Float32.fromFloat(1.04),
						c.Float32.fromFloat(1.04), c.Float32.fromFloat(1.04), CaxecraftPalette.selection());
				Raylib.EndMode3D();
				drawHud(renderCounters.visible, renderCounters.drawCalls, frameCount, updateCount, paused, captured, placementBlockedFrames > 0, hit,
					player.x, player.z, selectedMode, locale, inventory, guide, mossling, vitals, strikeHitFrames > 0, enemyDefeatedFrames > 0,
					enemyAttackFrames > 0, pickupFrames > 0, pickupAmount, inventoryFullReason, recoveryFeedback, recoveryFeedbackFrames > 0, hudTexture,
					hudTextureReady, itemTexture, itemTextureReady);
			}
			Raylib.EndDrawing();
			#if caxecraft_pilot
			// Checkpoints are handled after presentation, just as browser test
			// tools capture a page only after the requested state is visible.
			if (pilotName == PilotScriptName.LaunchSmoke && frameCount == 1)
				#if caxecraft_pilot_secondary_locale
				Raylib.TakeScreenshot("caxecraft-secondary-locale.png");
				#else
				Raylib.TakeScreenshot("caxecraft-smoke.png");
				#end
				if (pilotName == PilotScriptName.MoveJumpEdit && frameCount == 8)
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
			#end
			frameCount++;
		}

		Raylib.EnableCursor();
		if (terrainTextureReady)
			CaxecraftTextures.unload(terrainTexture);
		if (entityTextureReady)
			CaxecraftTextures.unload(entityTexture);
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

	/** Derive the meadow spawn afresh so no stale height survives a world edit. */
	static function spawnPlayer(cells:WorldCells):PlayerState {
		final spawnY = World.surfaceY(cells, 16, 16) + 1.0;
		return PlayerPhysics.recoverSpawn(cells, PlayerPhysics.player(SPAWN_X, spawnY, SPAWN_Z));
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

	static function drawHud(visible:Int, drawCalls:Int, frames:Int, updates:Int, paused:Bool, captured:Bool, placementBlocked:Bool, hit:RaycastHit,
			playerX:Float, playerZ:Float, mode:GameMode, locale:LocaleCursor, inventory:InventoryState, guide:GuideState, mossling:MosslingState,
			vitals:PlayerVitalsState, strikeHit:Bool, enemyDefeated:Bool, enemyAttacked:Bool, pickedUp:Bool, pickupAmount:Int,
			inventoryFullReason:InventoryFullReason, recoveryFeedback:RecoveryDecision, recoveryVisible:Bool, hudTexture:Texture2D, hudTextureReady:Bool,
			itemTexture:Texture2D, itemTextureReady:Bool):Void {
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
		UiCatalog.draw(locale, UiMessage.Brand, 32, 28, 20, text);
		UiCatalog.draw(locale, UiMessage.DebugCells, 32, 58, 14, text);
		HudDigits.drawNumber(World.VOLUME, 82, 59, 5, CaxecraftPalette.selection());
		UiCatalog.draw(locale, UiMessage.DebugVisible, 160, 58, 14, text);
		HudDigits.drawNumber(visible, 230, 59, 5, CaxecraftPalette.selection());
		UiCatalog.draw(locale, UiMessage.DebugDraws, 326, 58, 14, text);
		HudDigits.drawNumber(drawCalls, 382, 59, 5, CaxecraftPalette.selection());
		UiCatalog.draw(locale, UiMessage.DebugFrame, 32, 86, 12, text);
		HudDigits.drawNumber(frames, 82, 85, 6, text);
		UiCatalog.draw(locale, UiMessage.DebugTick, 174, 86, 12, text);
		HudDigits.drawNumber(updates, 216, 85, 6, text);
		drawHotbar(inventory, hudTexture, hudTextureReady, itemTexture, itemTextureReady, width, height);
		drawHealth(vitals, hudTexture, hudTextureReady, width);
		UiCatalog.draw(locale, UiMessage.Controls, 20, height - 22, 14, text);
		if (mode == GameMode.Adventure)
			FirstPlayableCatalog.draw(locale, ScenarioMessage.AdventureProgress, 32, 110, 14, CaxecraftPalette.selection());
		if (GuideNpc.isInRange(guide, playerX, playerZ)) {
			Raylib.DrawRectangle(centerX - 260, centerY + 54, 520, 60, CaxecraftPalette.hudPanel());
			if (GuideNpc.phase(guide) == GuidePhase.Waiting)
				FirstPlayableCatalog.draw(locale, ScenarioMessage.NiaTalk, centerX - 110, centerY + 74, 18, text);
			else if (GuideNpc.phase(guide) == GuidePhase.Welcomed)
				FirstPlayableCatalog.draw(locale, ScenarioMessage.NiaWelcome, centerX - 225, centerY + 74, 16, text);
			else
				FirstPlayableCatalog.draw(locale, ScenarioMessage.NiaGift, centerX - 205, centerY + 74, 16, text);
		}
		if (Mossling.isAlive(mossling)) {
			if (Mossling.mode(mossling) == MosslingMode.Windup)
				FirstPlayableCatalog.draw(locale, ScenarioMessage.MosslingWindup, width - 300, 28, 16, CaxecraftPalette.damage());
			else if (Mossling.mode(mossling) == MosslingMode.Chasing)
				FirstPlayableCatalog.draw(locale, ScenarioMessage.MosslingAlert, width - 180, 28, 16, CaxecraftPalette.selection());
		}
		if (strikeHit)
			FirstPlayableCatalog.draw(locale, ScenarioMessage.CopperStrike, centerX - 70, centerY - 54, 18, CaxecraftPalette.selection());
		if (enemyDefeated)
			FirstPlayableCatalog.draw(locale, ScenarioMessage.MosslingDroppedBerries, width - 285, 54, 16, CaxecraftPalette.selection());
		if (enemyAttacked)
			FirstPlayableCatalog.draw(locale, ScenarioMessage.TelegraphedHit, width - 330, 82, 16, CaxecraftPalette.damage());
		if (pickedUp) {
			final pickupMessage = pickupAmount == 1 ? ScenarioMessage.BerryPickupOne : ScenarioMessage.BerryPickupTwo;
			FirstPlayableCatalog.draw(locale, pickupMessage, centerX - 48, centerY + 24, 18, CaxecraftPalette.berry());
		}
		if (inventoryFullReason == InventoryFullReason.BerryStack)
			FirstPlayableCatalog.draw(locale, ScenarioMessage.BerryStackFull, centerX - 150, centerY + 48, 16, CaxecraftPalette.inventoryFull());
		else if (inventoryFullReason == InventoryFullReason.BlockStack)
			FirstPlayableCatalog.draw(locale, ScenarioMessage.BlockStackFull, centerX - 155, centerY + 48, 16, CaxecraftPalette.inventoryFull());
		if (recoveryVisible) {
			if (recoveryFeedback == RecoveryDecision.UseBerries)
				FirstPlayableCatalog.draw(locale, ScenarioMessage.BerryRecovery, centerX - 88, centerY + 24, 18, CaxecraftPalette.recovery());
			else if (recoveryFeedback == RecoveryDecision.HealthAlreadyFull)
				UiCatalog.draw(locale, UiMessage.HealthFull, centerX - 96, centerY + 24, 18, CaxecraftPalette.selection());
			else if (recoveryFeedback == RecoveryDecision.RecoveryStackEmpty)
				FirstPlayableCatalog.draw(locale, ScenarioMessage.NoBerries, centerX - 76, centerY + 24, 18, CaxecraftPalette.selection());
		}
		if (vitals.safeTicks > 15)
			Raylib.DrawRectangleLines(4, 4, width - 8, height - 8, CaxecraftPalette.damage());
		if (PlayerVitals.isDefeated(vitals)) {
			Raylib.DrawRectangle(centerX - 250, centerY - 74, 500, 148, CaxecraftPalette.hudPanel());
			Raylib.DrawRectangleLines(centerX - 250, centerY - 74, 500, 148, CaxecraftPalette.damage());
			FirstPlayableCatalog.draw(locale, ScenarioMessage.HaxirioFallen, centerX - 122, centerY - 42, 24, text);
			FirstPlayableCatalog.draw(locale, ScenarioMessage.ReturnToMeadow, centerX - 125, centerY + 10, 18, CaxecraftPalette.selection());
		}
		if (paused) {
			Raylib.DrawRectangle(centerX - 170, centerY - 48, 340, 96, CaxecraftPalette.hudPanel());
			Raylib.DrawRectangleLines(centerX - 170, centerY - 48, 340, 96, CaxecraftPalette.selection());
			UiCatalog.draw(locale, UiMessage.PauseTitle, centerX - 48, centerY - 30, 24, text);
			UiCatalog.draw(locale, UiMessage.PauseHelp, centerX - 160, centerY + 8, 16, text);
		} else if (placementBlocked) {
			UiCatalog.draw(locale, UiMessage.PlaceBlocked, centerX - 170, centerY + 26, 14, CaxecraftPalette.selection());
		} else if (!captured) {
			UiCatalog.draw(locale, UiMessage.CapturePrompt, centerX - 90, centerY + 26, 14, text);
		} else if (!hit.hit) {
			UiCatalog.draw(locale, UiMessage.NoBlockInReach, centerX - 105, centerY + 26, 14, text);
		}
	}

	/** Draw three whole/half/empty hearts from the reviewed HUD atlas. */
	static function drawHealth(vitals:PlayerVitalsState, hudTexture:Texture2D, hudTextureReady:Bool, width:Int):Void {
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

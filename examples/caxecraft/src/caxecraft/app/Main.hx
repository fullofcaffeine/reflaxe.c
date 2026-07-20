package caxecraft.app;

#if c
import c.CArray;
import c.UInt8;
import caxecraft.domain.BlockKind;
import caxecraft.domain.PlayerPhysics;
import caxecraft.domain.PlayerState;
import caxecraft.domain.RaycastHit;
import caxecraft.domain.VoxelRaycast;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.domain.WorldVolume;
import raylib.Camera3D;
import raylib.CameraProjection;
import raylib.Color;
import raylib.ConfigFlags;
import raylib.KeyboardKey;
import raylib.MouseButton;
import raylib.Raylib;
import raylib.Vector3;

/** C-specific window/input/render adapter around the shared Caxecraft domain. */
final class Main {
	static inline final FIXED_SECONDS:Float = 0.05;
	static inline final LOOK_SENSITIVITY:Float = 0.0025;
	static inline final MAX_FRAME_SECONDS:Float = 0.25;
	static inline final PICK_DISTANCE:Float = 7.0;

	static function main():Void {
		var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		final cells:WorldCells = storage.span();
		World.generate(cells, 0x0cafe);

		Raylib.SetConfigFlags(ConfigFlags.VsyncHint | ConfigFlags.WindowResizable | ConfigFlags.WindowHighDpi);
		Raylib.InitWindow(1280, 720, "Caxecraft — Haxe shaped into careful C");
		Raylib.SetWindowMinSize(800, 450);
		Raylib.SetExitKey(KeyboardKey.Null);
		Raylib.SetTargetFPS(120);
		Raylib.DisableCursor();

		var player:PlayerState = PlayerPhysics.recoverSpawn(cells, PlayerPhysics.player(16.5, 10.0, 16.5));
		var lookX = 0.0;
		var lookY = -0.18;
		var lookZ = -1.0;
		var accumulator = 0.0;
		var jumpQueued = false;
		var paused = false;
		var captured = true;
		var quit = false;
		var frameCount = 0;
		var updateCount = 0;
		var placementBlockedFrames = 0;

		while (!quit && !Raylib.WindowShouldClose()) {
			var recapturedThisFrame = false;
			final focused = Raylib.IsWindowFocused();
			if (!focused && captured) {
				paused = true;
				captured = false;
				jumpQueued = false;
				Raylib.EnableCursor();
			}
			if (Raylib.IsKeyPressed(KeyboardKey.Q))
				quit = true;
			if (focused && Raylib.IsKeyPressed(KeyboardKey.Escape)) {
				paused = !paused;
				captured = !paused;
				if (paused)
					jumpQueued = false;
				if (captured)
					Raylib.DisableCursor();
				else
					Raylib.EnableCursor();
			}
			if (focused && paused && Raylib.IsMouseButtonPressed(MouseButton.Left)) {
				paused = false;
				captured = true;
				recapturedThisFrame = true;
				Raylib.DisableCursor();
			}

			if (captured) {
				final mouse = Raylib.GetMouseDelta();
				var yawDelta = mouse.x.toFloat() * LOOK_SENSITIVITY;
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
				lookY -= mouse.y.toFloat() * LOOK_SENSITIVITY;
				if (lookY > 0.90)
					lookY = 0.90;
				if (lookY < -0.90)
					lookY = -0.90;
				if (Raylib.IsKeyPressed(KeyboardKey.Space))
					jumpQueued = true;
			}

			var frameSeconds = Raylib.GetFrameTime().toFloat();
			if (frameSeconds > MAX_FRAME_SECONDS)
				frameSeconds = MAX_FRAME_SECONDS;
			if (!paused)
				accumulator += frameSeconds;
			while (!paused && accumulator >= FIXED_SECONDS) {
				var moveX = 0.0;
				var moveZ = 0.0;
				if (Raylib.IsKeyDown(KeyboardKey.W)) {
					moveX += lookX;
					moveZ += lookZ;
				}
				if (Raylib.IsKeyDown(KeyboardKey.S)) {
					moveX -= lookX;
					moveZ -= lookZ;
				}
				if (Raylib.IsKeyDown(KeyboardKey.A)) {
					moveX += lookZ;
					moveZ -= lookX;
				}
				if (Raylib.IsKeyDown(KeyboardKey.D)) {
					moveX -= lookZ;
					moveZ += lookX;
				}
				final forwardHeld = Raylib.IsKeyDown(KeyboardKey.W) || Raylib.IsKeyDown(KeyboardKey.S);
				final sideHeld = Raylib.IsKeyDown(KeyboardKey.A) || Raylib.IsKeyDown(KeyboardKey.D);
				if (forwardHeld && sideHeld) {
					moveX *= 0.7071067811865476;
					moveZ *= 0.7071067811865476;
				}
				player = PlayerPhysics.step(cells, player, PlayerPhysics.input(moveX, moveZ, jumpQueued));
				jumpQueued = false;
				accumulator -= FIXED_SECONDS;
				updateCount++;
			}

			final eyeX = player.x;
			final eyeY = player.y + 1.62;
			final eyeZ = player.z;
			final hit = VoxelRaycast.trace(cells, eyeX, eyeY, eyeZ, lookX, lookY, lookZ, PICK_DISTANCE);
			if (captured && !recapturedThisFrame && hit.hit && Raylib.IsMouseButtonPressed(MouseButton.Left))
				World.remove(cells, World.coord(hit.cellX, hit.cellY, hit.cellZ));
			if (captured && hit.hit && Raylib.IsMouseButtonPressed(MouseButton.Right)) {
				final placement = World.coord(hit.previousX, hit.previousY, hit.previousZ);
				if (!PlayerPhysics.canPlaceAt(player, placement) || !World.place(cells, placement, BlockKind.Dirt))
					placementBlockedFrames = 60;
			}
			if (placementBlockedFrames > 0)
				placementBlockedFrames--;

			final camera = Camera3D.make(Vector3.fromFloat(eyeX, eyeY, eyeZ), Vector3.fromFloat(eyeX + lookX, eyeY + lookY, eyeZ + lookZ),
				Vector3.fromFloat(0.0, 1.0, 0.0), c.Float32.fromFloat(70.0), CameraProjection.Perspective);
			Raylib.BeginDrawing();
			Raylib.ClearBackground(CaxecraftPalette.sky());
			Raylib.BeginMode3D(camera);
			final renderCounters = drawWorld(cells);
			if (hit.hit)
				Raylib.DrawCubeWires(Vector3.fromFloat(hit.cellX + 0.5, hit.cellY + 0.5, hit.cellZ + 0.5), c.Float32.fromFloat(1.04),
					c.Float32.fromFloat(1.04), c.Float32.fromFloat(1.04), CaxecraftPalette.selection());
			Raylib.EndMode3D();
			drawHud(renderCounters.visible, renderCounters.drawCalls, frameCount, updateCount, paused, captured, placementBlockedFrames > 0, hit);
			Raylib.EndDrawing();
			frameCount++;
		}

		Raylib.EnableCursor();
		Raylib.CloseWindow();
	}

	static function drawWorld(cells:WorldCells):RenderCounters {
		var visible = 0;
		var drawCalls = 0;
		var z = 0;
		while (z < World.DEPTH) {
			var y = 0;
			while (y < World.HEIGHT) {
				var x = 0;
				while (x < World.WIDTH) {
					final kind = World.query(cells, World.coord(x, y, z));
					if (World.isSolid(kind) && isExposed(cells, x, y, z)) {
						visible++;
						drawCalls += 2;
						Raylib.DrawCube(Vector3.fromFloat(x + 0.5, y + 0.5, z + 0.5), c.Float32.fromFloat(1.0), c.Float32.fromFloat(1.0),
							c.Float32.fromFloat(1.0), CaxecraftPalette.block(kind));
						Raylib.DrawCubeWires(Vector3.fromFloat(x + 0.5, y + 0.5, z + 0.5), c.Float32.fromFloat(1.002), c.Float32.fromFloat(1.002),
							c.Float32.fromFloat(1.002), CaxecraftPalette.blockOutline(kind));
					}
					x++;
				}
				y++;
			}
			z++;
		}
		return {visible: visible, drawCalls: drawCalls};
	}

	static function isExposed(cells:WorldCells, x:Int, y:Int, z:Int):Bool {
		return !World.isSolid(World.query(cells, World.coord(x - 1, y, z)))
			|| !World.isSolid(World.query(cells, World.coord(x + 1, y, z)))
			|| !World.isSolid(World.query(cells, World.coord(x, y - 1, z)))
			|| !World.isSolid(World.query(cells, World.coord(x, y + 1, z)))
			|| !World.isSolid(World.query(cells, World.coord(x, y, z - 1)))
			|| !World.isSolid(World.query(cells, World.coord(x, y, z + 1)));
	}

	static function drawHud(visible:Int, drawCalls:Int, frames:Int, updates:Int, paused:Bool, captured:Bool, placementBlocked:Bool, hit:RaycastHit):Void {
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
		Raylib.DrawText("CAXECRAFT  //  C + HAXE", 32, 28, 20, text);
		Raylib.DrawText("CELLS", 32, 58, 14, text);
		HudDigits.drawNumber(World.VOLUME, 82, 59, 5, CaxecraftPalette.selection());
		Raylib.DrawText("VISIBLE", 160, 58, 14, text);
		HudDigits.drawNumber(visible, 230, 59, 5, CaxecraftPalette.selection());
		Raylib.DrawText("DRAWS", 326, 58, 14, text);
		HudDigits.drawNumber(drawCalls, 382, 59, 5, CaxecraftPalette.selection());
		Raylib.DrawText("FRAME", 32, 86, 12, text);
		HudDigits.drawNumber(frames, 82, 85, 6, text);
		Raylib.DrawText("TICK", 174, 86, 12, text);
		HudDigits.drawNumber(updates, 216, 85, 6, text);
		Raylib.DrawText("WASD MOVE  SPACE JUMP  LMB MINE  RMB PLACE  ESC PAUSE  Q QUIT", 20, height - 30, 16, text);
		if (paused) {
			Raylib.DrawRectangle(centerX - 170, centerY - 48, 340, 96, CaxecraftPalette.hudPanel());
			Raylib.DrawRectangleLines(centerX - 170, centerY - 48, 340, 96, CaxecraftPalette.selection());
			Raylib.DrawText("PAUSED", centerX - 48, centerY - 30, 24, text);
			Raylib.DrawText("CLICK TO CAPTURE  //  Q TO QUIT", centerX - 145, centerY + 8, 16, text);
		} else if (placementBlocked) {
			Raylib.DrawText("PLACE BLOCKED: PLAYER OR WORLD EDGE", centerX - 150, centerY + 26, 14, CaxecraftPalette.selection());
		} else if (!captured) {
			Raylib.DrawText("CLICK TO CAPTURE", centerX - 74, centerY + 26, 14, text);
		} else if (!hit.hit) {
			Raylib.DrawText("NO BLOCK IN REACH", centerX - 78, centerY + 26, 14, text);
		}
	}
}

private typedef RenderCounters = {
	final visible:Int;
	final drawCalls:Int;
}
#end

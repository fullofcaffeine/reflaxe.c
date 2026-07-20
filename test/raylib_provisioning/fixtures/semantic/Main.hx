import raylib.Camera3D;
import raylib.CameraProjection;
import raylib.Color;
import raylib.Colors;
import raylib.ConfigFlags;
import raylib.KeyboardKey;
import raylib.MouseButton;
import raylib.Raylib;
import raylib.TraceLogLevel;
import raylib.Vector2;
import raylib.Vector3;

class Main {
	static function main():Void {
		final flags = ConfigFlags.VsyncHint | ConfigFlags.WindowResizable;
		Raylib.SetConfigFlags(flags);
		Raylib.SetTraceLogLevel(TraceLogLevel.None);
		Raylib.InitWindow(64, 64, "hxc-raylib-semantic");
		Raylib.SetTargetFPS(60);
		while (!Raylib.IsWindowReady()) {}

		final origin = Vector3.fromFloat(0.0, 0.0, 0.0);
		final camera = Camera3D.make(Vector3.fromFloat(4.0, 4.0, 4.0), origin, Vector3.fromFloat(0.0, 1.0, 0.0), c.Float32.fromFloat(45.0),
			CameraProjection.Perspective);
		final cursor = Vector2.fromFloat(32.0, 32.0);
		final ray = Raylib.GetScreenToWorldRay(cursor, camera);
		final keyDown = Raylib.IsKeyDown(KeyboardKey.W);
		final mouseDown = Raylib.IsMouseButtonDown(MouseButton.Left);
		final accent = Color.rgba(245, 166, 35);

		Raylib.BeginDrawing();
		Raylib.ClearBackground(Colors.RAYWHITE);
		Raylib.BeginMode3D(camera);
		Raylib.DrawCube(origin, c.Float32.fromFloat(1.0), c.Float32.fromFloat(1.0), c.Float32.fromFloat(1.0), accent);
		Raylib.EndMode3D();
		Raylib.DrawText("Caxe: direct C, nicer Haxe", 4, 4, 10, Colors.DARKGRAY);
		Raylib.EndDrawing();
		Raylib.CloseWindow();
	}
}

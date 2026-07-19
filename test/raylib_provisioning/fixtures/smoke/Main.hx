import raylib.raw.Raylib;

class Main {
	static function main():Void {
		Raylib.SetTraceLogLevel(SmokeConstants.logNone);
		Raylib.InitWindow(SmokeConstants.size, SmokeConstants.size, "hxc-raylib-smoke");
		while (!Raylib.IsWindowReady()) {}

		// These results are intentionally retained as typed locals. Together they
		// prove direct binary32, binary64, bool, and by-value struct returns through
		// the public raw package before any semantic convenience layer is involved.
		var frameTime = Raylib.GetFrameTime();
		var elapsed = Raylib.GetTime();
		var mouse = Raylib.GetMousePosition();
		var keyDown = Raylib.IsKeyDown(SmokeConstants.keyNull);
		var mouseDown = Raylib.IsMouseButtonDown(SmokeConstants.mouseLeft);
		var camera = SmokeValues.camera();
		var screenPosition = SmokeValues.screenPosition();
		var worldRay = Raylib.GetScreenToWorldRay(screenPosition, camera);

		var color = Raylib.GetColor(SmokeConstants.color);
		Raylib.BeginDrawing();
		Raylib.ClearBackground(color);
		Raylib.DrawPixel(SmokeConstants.center, SmokeConstants.center, color);
		Raylib.EndDrawing();
		Raylib.CloseWindow();
	}
}

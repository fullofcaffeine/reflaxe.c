class Main {
	static function main():Void {
		RaylibNative.SetTraceLogLevel(TraceLogLevel.None);
		RaylibNative.InitWindow(RaylibNative.smokeSize, RaylibNative.smokeSize, "hxc-raylib-smoke");
		while (!RaylibNative.IsWindowReady()) {}
		var color = RaylibNative.GetColor(RaylibNative.smokeColor);
		RaylibNative.BeginDrawing();
		RaylibNative.ClearBackground(color);
		RaylibNative.DrawPixel(RaylibNative.smokeCenter, RaylibNative.smokeCenter, color);
		RaylibNative.EndDrawing();
		RaylibNative.CloseWindow();
	}
}

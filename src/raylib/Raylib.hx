package raylib;

#if c
/**
 * Zero-cost semantic facade for the reviewed raylib core slice.
 *
 * Names stay aligned with raylib's C cheatsheet. Every method below is inline:
 * it forwards to the same `raylib.raw.Raylib` function, allocates nothing,
 * selects no `hxrt` feature, creates no ownership, and adds no failure path.
 * Window, input, and drawing calls remain main/render-thread operations.
 * `c.CString` parameters currently accept only embedded-NUL-free literals with
 * static borrowed lifetime. Resource, pointer, callback, and variadic APIs are
 * intentionally absent; use the public raw layer only where its explicit C
 * lifetime or unsafe contract is acceptable.
 */
class Raylib {
	public static inline function SetConfigFlags(flags:ConfigFlags):Void
		raylib.raw.Raylib.SetConfigFlags(c.IntConvert.modulo(flags));

	public static inline function InitWindow(width:Int, height:Int, title:c.CString):Void
		raylib.raw.Raylib.InitWindow(c.IntConvert.exact(width), c.IntConvert.exact(height), title);

	public static inline function IsWindowReady():Bool
		return raylib.raw.Raylib.IsWindowReady();

	public static inline function WindowShouldClose():Bool
		return raylib.raw.Raylib.WindowShouldClose();

	public static inline function CloseWindow():Void
		raylib.raw.Raylib.CloseWindow();

	public static inline function IsWindowFocused():Bool
		return raylib.raw.Raylib.IsWindowFocused();

	public static inline function IsWindowResized():Bool
		return raylib.raw.Raylib.IsWindowResized();

	public static inline function SetWindowSize(width:Int, height:Int):Void
		raylib.raw.Raylib.SetWindowSize(c.IntConvert.exact(width), c.IntConvert.exact(height));

	public static inline function SetWindowMinSize(width:Int, height:Int):Void
		raylib.raw.Raylib.SetWindowMinSize(c.IntConvert.exact(width), c.IntConvert.exact(height));

	public static inline function GetScreenWidth():Int
		return c.IntConvert.exact(raylib.raw.Raylib.GetScreenWidth());

	public static inline function GetScreenHeight():Int
		return c.IntConvert.exact(raylib.raw.Raylib.GetScreenHeight());

	public static inline function SetTargetFPS(fps:Int):Void
		raylib.raw.Raylib.SetTargetFPS(c.IntConvert.exact(fps));

	public static inline function GetFPS():Int
		return c.IntConvert.exact(raylib.raw.Raylib.GetFPS());

	public static inline function GetFrameTime():c.Float32
		return raylib.raw.Raylib.GetFrameTime();

	public static inline function GetTime():Float
		return raylib.raw.Raylib.GetTime();

	public static inline function SetExitKey(key:KeyboardKey):Void
		raylib.raw.Raylib.SetExitKey(c.IntConvert.exact(key));

	public static inline function SetTraceLogLevel(level:TraceLogLevel):Void
		raylib.raw.Raylib.SetTraceLogLevel(c.IntConvert.exact(level));

	public static inline function IsKeyPressed(key:KeyboardKey):Bool
		return raylib.raw.Raylib.IsKeyPressed(c.IntConvert.exact(key));

	public static inline function IsKeyDown(key:KeyboardKey):Bool
		return raylib.raw.Raylib.IsKeyDown(c.IntConvert.exact(key));

	public static inline function IsKeyReleased(key:KeyboardKey):Bool
		return raylib.raw.Raylib.IsKeyReleased(c.IntConvert.exact(key));

	public static inline function IsMouseButtonPressed(button:MouseButton):Bool
		return raylib.raw.Raylib.IsMouseButtonPressed(c.IntConvert.exact(button));

	public static inline function IsMouseButtonDown(button:MouseButton):Bool
		return raylib.raw.Raylib.IsMouseButtonDown(c.IntConvert.exact(button));

	public static inline function IsMouseButtonReleased(button:MouseButton):Bool
		return raylib.raw.Raylib.IsMouseButtonReleased(c.IntConvert.exact(button));

	public static inline function GetMousePosition():Vector2
		return raylib.raw.Raylib.GetMousePosition();

	public static inline function GetMouseDelta():Vector2
		return raylib.raw.Raylib.GetMouseDelta();

	public static inline function GetMouseWheelMove():c.Float32
		return raylib.raw.Raylib.GetMouseWheelMove();

	public static inline function DisableCursor():Void
		raylib.raw.Raylib.DisableCursor();

	public static inline function EnableCursor():Void
		raylib.raw.Raylib.EnableCursor();

	public static inline function IsCursorHidden():Bool
		return raylib.raw.Raylib.IsCursorHidden();

	public static inline function GetScreenToWorldRay(position:Vector2, camera:Camera3D):Ray
		return raylib.raw.Raylib.GetScreenToWorldRay(position, camera);

	public static inline function GetRayCollisionBox(ray:Ray, box:BoundingBox):RayCollision
		return raylib.raw.Raylib.GetRayCollisionBox(ray, box);

	public static inline function GetRayCollisionSphere(ray:Ray, center:Vector3, radius:c.Float32):RayCollision
		return raylib.raw.Raylib.GetRayCollisionSphere(ray, center, radius);

	public static inline function CheckCollisionBoxes(left:BoundingBox, right:BoundingBox):Bool
		return raylib.raw.Raylib.CheckCollisionBoxes(left, right);

	public static inline function BeginDrawing():Void
		raylib.raw.Raylib.BeginDrawing();

	public static inline function EndDrawing():Void
		raylib.raw.Raylib.EndDrawing();

	public static inline function ClearBackground(color:Color):Void
		raylib.raw.Raylib.ClearBackground(color);

	public static inline function BeginMode3D(camera:Camera3D):Void
		raylib.raw.Raylib.BeginMode3D(camera);

	public static inline function EndMode3D():Void
		raylib.raw.Raylib.EndMode3D();

	public static inline function DrawPixel(x:Int, y:Int, color:Color):Void
		raylib.raw.Raylib.DrawPixel(c.IntConvert.exact(x), c.IntConvert.exact(y), color);

	public static inline function DrawLine(startX:Int, startY:Int, endX:Int, endY:Int, color:Color):Void
		raylib.raw.Raylib.DrawLine(c.IntConvert.exact(startX), c.IntConvert.exact(startY), c.IntConvert.exact(endX), c.IntConvert.exact(endY), color);

	public static inline function DrawRectangle(x:Int, y:Int, width:Int, height:Int, color:Color):Void
		raylib.raw.Raylib.DrawRectangle(c.IntConvert.exact(x), c.IntConvert.exact(y), c.IntConvert.exact(width), c.IntConvert.exact(height), color);

	public static inline function DrawRectangleLines(x:Int, y:Int, width:Int, height:Int, color:Color):Void
		raylib.raw.Raylib.DrawRectangleLines(c.IntConvert.exact(x), c.IntConvert.exact(y), c.IntConvert.exact(width), c.IntConvert.exact(height), color);

	public static inline function DrawCircle(centerX:Int, centerY:Int, radius:c.Float32, color:Color):Void
		raylib.raw.Raylib.DrawCircle(c.IntConvert.exact(centerX), c.IntConvert.exact(centerY), radius, color);

	public static inline function DrawText(text:c.CString, x:Int, y:Int, fontSize:Int, color:Color):Void
		raylib.raw.Raylib.DrawText(text, c.IntConvert.exact(x), c.IntConvert.exact(y), c.IntConvert.exact(fontSize), color);

	public static inline function DrawFPS(x:Int, y:Int):Void
		raylib.raw.Raylib.DrawFPS(c.IntConvert.exact(x), c.IntConvert.exact(y));

	public static inline function DrawLine3D(start:Vector3, end:Vector3, color:Color):Void
		raylib.raw.Raylib.DrawLine3D(start, end, color);

	public static inline function DrawCube(position:Vector3, width:c.Float32, height:c.Float32, length:c.Float32, color:Color):Void
		raylib.raw.Raylib.DrawCube(position, width, height, length, color);

	public static inline function DrawCubeV(position:Vector3, size:Vector3, color:Color):Void
		raylib.raw.Raylib.DrawCubeV(position, size, color);

	public static inline function DrawCubeWires(position:Vector3, width:c.Float32, height:c.Float32, length:c.Float32, color:Color):Void
		raylib.raw.Raylib.DrawCubeWires(position, width, height, length, color);

	public static inline function DrawCubeWiresV(position:Vector3, size:Vector3, color:Color):Void
		raylib.raw.Raylib.DrawCubeWiresV(position, size, color);

	public static inline function DrawSphere(center:Vector3, radius:c.Float32, color:Color):Void
		raylib.raw.Raylib.DrawSphere(center, radius, color);

	public static inline function DrawSphereWires(center:Vector3, radius:c.Float32, rings:Int, slices:Int, color:Color):Void
		raylib.raw.Raylib.DrawSphereWires(center, radius, c.IntConvert.exact(rings), c.IntConvert.exact(slices), color);

	public static inline function DrawGrid(slices:Int, spacing:c.Float32):Void
		raylib.raw.Raylib.DrawGrid(c.IntConvert.exact(slices), spacing);

	public static inline function GetColor(rgba:UInt):Color
		return raylib.raw.Raylib.GetColor(c.IntConvert.modulo(rgba));
}
#end

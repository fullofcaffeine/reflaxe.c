package raylib;

// The complete facade is intentionally absent on non-C targets. `#if c` is a
// compile-time package boundary: it prevents another target from accidentally
// treating C ABI types as portable Haxe values and emits no run-time test.
// See docs/raylib-semantic-core.md for the cross-target adapter design.

#if c
/** Direct raylib aliases whose text borrow is checked at the Haxe/C boundary. */
@:c.include("raylib.h", c.IncludeKind.System)
private extern class RaylibRuntimeText {
	/** Draw one runtime-owned String without retaining its UTF-8 pointer. */
	@:c.name("DrawText")
	public static function drawText(text:c.CStringRef, x:c.Int32, y:c.Int32, fontSize:c.Int32, color:Color):Void;
}

/**
 * Zero-cost semantic facade for the reviewed raylib core slice.
 *
 * Names stay aligned with raylib's C cheatsheet. Most methods below are inline
 * zero-cost forwards to `raylib.raw.Raylib`. `DrawTextString` is the deliberate
 * exception: it copies arbitrary Haxe text into terminated storage before C
 * observes it. Static text can use `DrawText` without that allocation.
 * Window, input, and drawing calls remain main/render-thread operations.
 * `c.CString` parameters accept embedded-NUL-free static literals and closed
 * selections composed only from those literals. Resource, pointer, callback,
 * and variadic APIs are
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

	/**
	 * Report whether Raylib currently exposes one zero-based controller slot.
	 *
	 * A controller may disappear between frames, so callers sample this before
	 * reading buttons or axes and reset their own held/repeat state when it
	 * becomes false. Raylib retains no Haxe value and this check allocates
	 * nothing in generated C.
	 */
	public static inline function IsGamepadAvailable(gamepad:Int):Bool
		return raylib.raw.Raylib.IsGamepadAvailable(c.IntConvert.exact(gamepad));

	/** Report the one-frame press edge for a platform-neutral button position. */
	public static inline function IsGamepadButtonPressed(gamepad:Int, button:GamepadButton):Bool
		return raylib.raw.Raylib.IsGamepadButtonPressed(c.IntConvert.exact(gamepad), c.IntConvert.exact(button));

	/** Report whether a platform-neutral button position remains held now. */
	public static inline function IsGamepadButtonDown(gamepad:Int, button:GamepadButton):Bool
		return raylib.raw.Raylib.IsGamepadButtonDown(c.IntConvert.exact(gamepad), c.IntConvert.exact(button));

	/**
	 * Read one normalized analog axis as C binary32.
	 *
	 * Raylib returns values in the nominal `-1.0` to `1.0` range. This facade
	 * deliberately does not choose a dead zone or repeat rate: those are
	 * application interaction policies, not properties of the C ABI.
	 */
	public static inline function GetGamepadAxisMovement(gamepad:Int, axis:GamepadAxis):c.Float32
		return raylib.raw.Raylib.GetGamepadAxisMovement(c.IntConvert.exact(gamepad), c.IntConvert.exact(axis));

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

	/**
	 * Report whether one static C-string path names an existing filesystem entry.
	 *
	 * Raylib checks during this call and retains neither the path nor any file
	 * handle. Caxecraft's bounded pilot uses the result immediately after
	 * `TakeScreenshot`: native evidence can then distinguish “the capture
	 * checkpoint ran but Raylib did not publish a file” from “the checkpoint was
	 * never reached.” This is only an observation; it grants no file ownership.
	 */
	public static inline function FileExists(fileName:c.CString):Bool
		return raylib.raw.Raylib.FileExists(fileName);

	/**
	 * Save the current framebuffer to a file named by a static C string.
	 *
	 * Raylib reads `fileName` only during this call; it does not keep the
	 * pointer. The current facade therefore admits the same embedded-NUL-free,
	 * statically borrowed literal selections as `InitWindow` and `DrawText`.
	 */
	public static inline function TakeScreenshot(fileName:c.CString):Void
		raylib.raw.Raylib.TakeScreenshot(fileName);

	public static inline function ClearBackground(color:Color):Void
		raylib.raw.Raylib.ClearBackground(color);

	public static inline function BeginMode3D(camera:Camera3D):Void
		raylib.raw.Raylib.BeginMode3D(camera);

	public static inline function EndMode3D():Void
		raylib.raw.Raylib.EndMode3D();

	/**
	 * Clip following 2D or 3D drawing to one screen-space rectangle.
	 *
	 * Raylib stores this render state until `EndScissorMode`; it retains no Haxe
	 * value or pointer. The editor uses the pair to keep its perspective world
	 * inside the canvas instead of drawing over immediate-mode controls.
	 */
	public static inline function BeginScissorMode(x:Int, y:Int, width:Int, height:Int):Void
		raylib.raw.Raylib.BeginScissorMode(c.IntConvert.exact(x), c.IntConvert.exact(y), c.IntConvert.exact(width), c.IntConvert.exact(height));

	/** End the matching screen-space clip region. */
	public static inline function EndScissorMode():Void
		raylib.raw.Raylib.EndScissorMode();

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

	/**
	 * Draw arbitrary Haxe text through one checked, non-retaining C call.
	 *
	 * Use `DrawText` for static literals. A Haxe substring can end before its
	 * owner's NUL terminator, so it is not always safe to lend directly as a C
	 * string. `StringBuf` makes one terminated managed copy. The call-scoped
	 * `CStringRef` then validates it and keeps it alive until raylib returns.
	 * Embedded NUL still fails before C observes truncated text.
	 */
	public static inline function DrawTextString(text:String, x:Int, y:Int, fontSize:Int, color:Color):Void {
		final buffer = new StringBuf();
		buffer.add(text);
		final terminated = buffer.toString();
		RaylibRuntimeText.drawText(c.CStringRef.to(terminated), c.IntConvert.exact(x), c.IntConvert.exact(y), c.IntConvert.exact(fontSize), color);
	}

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

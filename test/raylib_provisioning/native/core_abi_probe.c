/*
 * Generated ABI probe for the precise raylib 6.0 Caxecraft core.
 * Authority: scripts/raylib/core_binding.py and dbc56a87da87d973a9c5baa4e7438a9d20121d28.
 */
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include "raylib.h"

_Static_assert(sizeof(bool) == 1U, "raylib bool must occupy one byte");
_Static_assert(sizeof(float) == 4U, "raylib float must be binary32-sized");
_Static_assert(FLT_RADIX == 2, "raylib float radix must be binary");
_Static_assert(FLT_MANT_DIG == 24, "raylib float mantissa must be binary32");
_Static_assert(FLT_MAX_EXP == 128, "raylib float exponent must be binary32");
_Static_assert(sizeof(int) == 4U, "raylib int must be 32-bit");
_Static_assert(sizeof(unsigned int) == 4U, "raylib unsigned int must be 32-bit");

_Static_assert(sizeof(BoundingBox) == 24U, "BoundingBox size drifted");
_Static_assert(_Alignof(BoundingBox) == 4U, "BoundingBox alignment drifted");
_Static_assert(offsetof(BoundingBox, min) == 0U, "BoundingBox.min offset drifted");
_Static_assert(offsetof(BoundingBox, max) == 12U, "BoundingBox.max offset drifted");

_Static_assert(sizeof(Camera3D) == 44U, "Camera3D size drifted");
_Static_assert(_Alignof(Camera3D) == 4U, "Camera3D alignment drifted");
_Static_assert(offsetof(Camera3D, position) == 0U, "Camera3D.position offset drifted");
_Static_assert(offsetof(Camera3D, target) == 12U, "Camera3D.target offset drifted");
_Static_assert(offsetof(Camera3D, up) == 24U, "Camera3D.up offset drifted");
_Static_assert(offsetof(Camera3D, fovy) == 36U, "Camera3D.fovy offset drifted");
_Static_assert(offsetof(Camera3D, projection) == 40U, "Camera3D.projection offset drifted");

_Static_assert(sizeof(Color) == 4U, "Color size drifted");
_Static_assert(_Alignof(Color) == 1U, "Color alignment drifted");
_Static_assert(offsetof(Color, r) == 0U, "Color.r offset drifted");
_Static_assert(offsetof(Color, g) == 1U, "Color.g offset drifted");
_Static_assert(offsetof(Color, b) == 2U, "Color.b offset drifted");
_Static_assert(offsetof(Color, a) == 3U, "Color.a offset drifted");

_Static_assert(sizeof(Ray) == 24U, "Ray size drifted");
_Static_assert(_Alignof(Ray) == 4U, "Ray alignment drifted");
_Static_assert(offsetof(Ray, position) == 0U, "Ray.position offset drifted");
_Static_assert(offsetof(Ray, direction) == 12U, "Ray.direction offset drifted");

_Static_assert(sizeof(RayCollision) == 32U, "RayCollision size drifted");
_Static_assert(_Alignof(RayCollision) == 4U, "RayCollision alignment drifted");
_Static_assert(offsetof(RayCollision, hit) == 0U, "RayCollision.hit offset drifted");
_Static_assert(offsetof(RayCollision, distance) == 4U, "RayCollision.distance offset drifted");
_Static_assert(offsetof(RayCollision, point) == 8U, "RayCollision.point offset drifted");
_Static_assert(offsetof(RayCollision, normal) == 20U, "RayCollision.normal offset drifted");

_Static_assert(sizeof(Rectangle) == 16U, "Rectangle size drifted");
_Static_assert(_Alignof(Rectangle) == 4U, "Rectangle alignment drifted");
_Static_assert(offsetof(Rectangle, x) == 0U, "Rectangle.x offset drifted");
_Static_assert(offsetof(Rectangle, y) == 4U, "Rectangle.y offset drifted");
_Static_assert(offsetof(Rectangle, width) == 8U, "Rectangle.width offset drifted");
_Static_assert(offsetof(Rectangle, height) == 12U, "Rectangle.height offset drifted");

_Static_assert(sizeof(Texture) == 20U, "Texture size drifted");
_Static_assert(_Alignof(Texture) == 4U, "Texture alignment drifted");
_Static_assert(offsetof(Texture, id) == 0U, "Texture.id offset drifted");
_Static_assert(offsetof(Texture, width) == 4U, "Texture.width offset drifted");
_Static_assert(offsetof(Texture, height) == 8U, "Texture.height offset drifted");
_Static_assert(offsetof(Texture, mipmaps) == 12U, "Texture.mipmaps offset drifted");
_Static_assert(offsetof(Texture, format) == 16U, "Texture.format offset drifted");

_Static_assert(sizeof(Vector2) == 8U, "Vector2 size drifted");
_Static_assert(_Alignof(Vector2) == 4U, "Vector2 alignment drifted");
_Static_assert(offsetof(Vector2, x) == 0U, "Vector2.x offset drifted");
_Static_assert(offsetof(Vector2, y) == 4U, "Vector2.y offset drifted");

_Static_assert(sizeof(Vector3) == 12U, "Vector3 size drifted");
_Static_assert(_Alignof(Vector3) == 4U, "Vector3 alignment drifted");
_Static_assert(offsetof(Vector3, x) == 0U, "Vector3.x offset drifted");
_Static_assert(offsetof(Vector3, y) == 4U, "Vector3.y offset drifted");
_Static_assert(offsetof(Vector3, z) == 8U, "Vector3.z offset drifted");

_Static_assert(sizeof(Camera) == sizeof(Camera3D), "Camera alias size drifted");
_Static_assert(_Alignof(Camera) == _Alignof(Camera3D), "Camera alias alignment drifted");
_Static_assert(_Generic((Camera){0}, Camera3D: 1, default: 0) == 1, "Camera must alias Camera3D");

_Static_assert(sizeof(Texture2D) == sizeof(Texture), "Texture2D alias size drifted");
_Static_assert(_Alignof(Texture2D) == _Alignof(Texture), "Texture2D alias alignment drifted");
_Static_assert(_Generic((Texture2D){0}, Texture: 1, default: 0) == 1, "Texture2D must alias Texture");

_Static_assert(sizeof(CameraProjection) == sizeof(int), "CameraProjection underlying ABI drifted");
_Static_assert(CAMERA_PERSPECTIVE == 0, "CAMERA_PERSPECTIVE value drifted");
_Static_assert(CAMERA_ORTHOGRAPHIC == 1, "CAMERA_ORTHOGRAPHIC value drifted");

_Static_assert(sizeof(ConfigFlags) == sizeof(int), "ConfigFlags underlying ABI drifted");
_Static_assert(FLAG_VSYNC_HINT == 64, "FLAG_VSYNC_HINT value drifted");
_Static_assert(FLAG_FULLSCREEN_MODE == 2, "FLAG_FULLSCREEN_MODE value drifted");
_Static_assert(FLAG_WINDOW_RESIZABLE == 4, "FLAG_WINDOW_RESIZABLE value drifted");
_Static_assert(FLAG_WINDOW_UNDECORATED == 8, "FLAG_WINDOW_UNDECORATED value drifted");
_Static_assert(FLAG_WINDOW_HIDDEN == 128, "FLAG_WINDOW_HIDDEN value drifted");
_Static_assert(FLAG_WINDOW_MINIMIZED == 512, "FLAG_WINDOW_MINIMIZED value drifted");
_Static_assert(FLAG_WINDOW_MAXIMIZED == 1024, "FLAG_WINDOW_MAXIMIZED value drifted");
_Static_assert(FLAG_WINDOW_UNFOCUSED == 2048, "FLAG_WINDOW_UNFOCUSED value drifted");
_Static_assert(FLAG_WINDOW_TOPMOST == 4096, "FLAG_WINDOW_TOPMOST value drifted");
_Static_assert(FLAG_WINDOW_ALWAYS_RUN == 256, "FLAG_WINDOW_ALWAYS_RUN value drifted");
_Static_assert(FLAG_WINDOW_TRANSPARENT == 16, "FLAG_WINDOW_TRANSPARENT value drifted");
_Static_assert(FLAG_WINDOW_HIGHDPI == 8192, "FLAG_WINDOW_HIGHDPI value drifted");
_Static_assert(FLAG_WINDOW_MOUSE_PASSTHROUGH == 16384, "FLAG_WINDOW_MOUSE_PASSTHROUGH value drifted");
_Static_assert(FLAG_BORDERLESS_WINDOWED_MODE == 32768, "FLAG_BORDERLESS_WINDOWED_MODE value drifted");
_Static_assert(FLAG_MSAA_4X_HINT == 32, "FLAG_MSAA_4X_HINT value drifted");
_Static_assert(FLAG_INTERLACED_HINT == 65536, "FLAG_INTERLACED_HINT value drifted");

_Static_assert(sizeof(KeyboardKey) == sizeof(int), "KeyboardKey underlying ABI drifted");
_Static_assert(KEY_NULL == 0, "KEY_NULL value drifted");
_Static_assert(KEY_APOSTROPHE == 39, "KEY_APOSTROPHE value drifted");
_Static_assert(KEY_COMMA == 44, "KEY_COMMA value drifted");
_Static_assert(KEY_MINUS == 45, "KEY_MINUS value drifted");
_Static_assert(KEY_PERIOD == 46, "KEY_PERIOD value drifted");
_Static_assert(KEY_SLASH == 47, "KEY_SLASH value drifted");
_Static_assert(KEY_ZERO == 48, "KEY_ZERO value drifted");
_Static_assert(KEY_ONE == 49, "KEY_ONE value drifted");
_Static_assert(KEY_TWO == 50, "KEY_TWO value drifted");
_Static_assert(KEY_THREE == 51, "KEY_THREE value drifted");
_Static_assert(KEY_FOUR == 52, "KEY_FOUR value drifted");
_Static_assert(KEY_FIVE == 53, "KEY_FIVE value drifted");
_Static_assert(KEY_SIX == 54, "KEY_SIX value drifted");
_Static_assert(KEY_SEVEN == 55, "KEY_SEVEN value drifted");
_Static_assert(KEY_EIGHT == 56, "KEY_EIGHT value drifted");
_Static_assert(KEY_NINE == 57, "KEY_NINE value drifted");
_Static_assert(KEY_SEMICOLON == 59, "KEY_SEMICOLON value drifted");
_Static_assert(KEY_EQUAL == 61, "KEY_EQUAL value drifted");
_Static_assert(KEY_A == 65, "KEY_A value drifted");
_Static_assert(KEY_B == 66, "KEY_B value drifted");
_Static_assert(KEY_C == 67, "KEY_C value drifted");
_Static_assert(KEY_D == 68, "KEY_D value drifted");
_Static_assert(KEY_E == 69, "KEY_E value drifted");
_Static_assert(KEY_F == 70, "KEY_F value drifted");
_Static_assert(KEY_G == 71, "KEY_G value drifted");
_Static_assert(KEY_H == 72, "KEY_H value drifted");
_Static_assert(KEY_I == 73, "KEY_I value drifted");
_Static_assert(KEY_J == 74, "KEY_J value drifted");
_Static_assert(KEY_K == 75, "KEY_K value drifted");
_Static_assert(KEY_L == 76, "KEY_L value drifted");
_Static_assert(KEY_M == 77, "KEY_M value drifted");
_Static_assert(KEY_N == 78, "KEY_N value drifted");
_Static_assert(KEY_O == 79, "KEY_O value drifted");
_Static_assert(KEY_P == 80, "KEY_P value drifted");
_Static_assert(KEY_Q == 81, "KEY_Q value drifted");
_Static_assert(KEY_R == 82, "KEY_R value drifted");
_Static_assert(KEY_S == 83, "KEY_S value drifted");
_Static_assert(KEY_T == 84, "KEY_T value drifted");
_Static_assert(KEY_U == 85, "KEY_U value drifted");
_Static_assert(KEY_V == 86, "KEY_V value drifted");
_Static_assert(KEY_W == 87, "KEY_W value drifted");
_Static_assert(KEY_X == 88, "KEY_X value drifted");
_Static_assert(KEY_Y == 89, "KEY_Y value drifted");
_Static_assert(KEY_Z == 90, "KEY_Z value drifted");
_Static_assert(KEY_LEFT_BRACKET == 91, "KEY_LEFT_BRACKET value drifted");
_Static_assert(KEY_BACKSLASH == 92, "KEY_BACKSLASH value drifted");
_Static_assert(KEY_RIGHT_BRACKET == 93, "KEY_RIGHT_BRACKET value drifted");
_Static_assert(KEY_GRAVE == 96, "KEY_GRAVE value drifted");
_Static_assert(KEY_SPACE == 32, "KEY_SPACE value drifted");
_Static_assert(KEY_ESCAPE == 256, "KEY_ESCAPE value drifted");
_Static_assert(KEY_ENTER == 257, "KEY_ENTER value drifted");
_Static_assert(KEY_TAB == 258, "KEY_TAB value drifted");
_Static_assert(KEY_BACKSPACE == 259, "KEY_BACKSPACE value drifted");
_Static_assert(KEY_INSERT == 260, "KEY_INSERT value drifted");
_Static_assert(KEY_DELETE == 261, "KEY_DELETE value drifted");
_Static_assert(KEY_RIGHT == 262, "KEY_RIGHT value drifted");
_Static_assert(KEY_LEFT == 263, "KEY_LEFT value drifted");
_Static_assert(KEY_DOWN == 264, "KEY_DOWN value drifted");
_Static_assert(KEY_UP == 265, "KEY_UP value drifted");
_Static_assert(KEY_PAGE_UP == 266, "KEY_PAGE_UP value drifted");
_Static_assert(KEY_PAGE_DOWN == 267, "KEY_PAGE_DOWN value drifted");
_Static_assert(KEY_HOME == 268, "KEY_HOME value drifted");
_Static_assert(KEY_END == 269, "KEY_END value drifted");
_Static_assert(KEY_CAPS_LOCK == 280, "KEY_CAPS_LOCK value drifted");
_Static_assert(KEY_SCROLL_LOCK == 281, "KEY_SCROLL_LOCK value drifted");
_Static_assert(KEY_NUM_LOCK == 282, "KEY_NUM_LOCK value drifted");
_Static_assert(KEY_PRINT_SCREEN == 283, "KEY_PRINT_SCREEN value drifted");
_Static_assert(KEY_PAUSE == 284, "KEY_PAUSE value drifted");
_Static_assert(KEY_F1 == 290, "KEY_F1 value drifted");
_Static_assert(KEY_F2 == 291, "KEY_F2 value drifted");
_Static_assert(KEY_F3 == 292, "KEY_F3 value drifted");
_Static_assert(KEY_F4 == 293, "KEY_F4 value drifted");
_Static_assert(KEY_F5 == 294, "KEY_F5 value drifted");
_Static_assert(KEY_F6 == 295, "KEY_F6 value drifted");
_Static_assert(KEY_F7 == 296, "KEY_F7 value drifted");
_Static_assert(KEY_F8 == 297, "KEY_F8 value drifted");
_Static_assert(KEY_F9 == 298, "KEY_F9 value drifted");
_Static_assert(KEY_F10 == 299, "KEY_F10 value drifted");
_Static_assert(KEY_F11 == 300, "KEY_F11 value drifted");
_Static_assert(KEY_F12 == 301, "KEY_F12 value drifted");
_Static_assert(KEY_LEFT_SHIFT == 340, "KEY_LEFT_SHIFT value drifted");
_Static_assert(KEY_LEFT_CONTROL == 341, "KEY_LEFT_CONTROL value drifted");
_Static_assert(KEY_LEFT_ALT == 342, "KEY_LEFT_ALT value drifted");
_Static_assert(KEY_LEFT_SUPER == 343, "KEY_LEFT_SUPER value drifted");
_Static_assert(KEY_RIGHT_SHIFT == 344, "KEY_RIGHT_SHIFT value drifted");
_Static_assert(KEY_RIGHT_CONTROL == 345, "KEY_RIGHT_CONTROL value drifted");
_Static_assert(KEY_RIGHT_ALT == 346, "KEY_RIGHT_ALT value drifted");
_Static_assert(KEY_RIGHT_SUPER == 347, "KEY_RIGHT_SUPER value drifted");
_Static_assert(KEY_KB_MENU == 348, "KEY_KB_MENU value drifted");
_Static_assert(KEY_KP_0 == 320, "KEY_KP_0 value drifted");
_Static_assert(KEY_KP_1 == 321, "KEY_KP_1 value drifted");
_Static_assert(KEY_KP_2 == 322, "KEY_KP_2 value drifted");
_Static_assert(KEY_KP_3 == 323, "KEY_KP_3 value drifted");
_Static_assert(KEY_KP_4 == 324, "KEY_KP_4 value drifted");
_Static_assert(KEY_KP_5 == 325, "KEY_KP_5 value drifted");
_Static_assert(KEY_KP_6 == 326, "KEY_KP_6 value drifted");
_Static_assert(KEY_KP_7 == 327, "KEY_KP_7 value drifted");
_Static_assert(KEY_KP_8 == 328, "KEY_KP_8 value drifted");
_Static_assert(KEY_KP_9 == 329, "KEY_KP_9 value drifted");
_Static_assert(KEY_KP_DECIMAL == 330, "KEY_KP_DECIMAL value drifted");
_Static_assert(KEY_KP_DIVIDE == 331, "KEY_KP_DIVIDE value drifted");
_Static_assert(KEY_KP_MULTIPLY == 332, "KEY_KP_MULTIPLY value drifted");
_Static_assert(KEY_KP_SUBTRACT == 333, "KEY_KP_SUBTRACT value drifted");
_Static_assert(KEY_KP_ADD == 334, "KEY_KP_ADD value drifted");
_Static_assert(KEY_KP_ENTER == 335, "KEY_KP_ENTER value drifted");
_Static_assert(KEY_KP_EQUAL == 336, "KEY_KP_EQUAL value drifted");
_Static_assert(KEY_BACK == 4, "KEY_BACK value drifted");
_Static_assert(KEY_MENU == 5, "KEY_MENU value drifted");
_Static_assert(KEY_VOLUME_UP == 24, "KEY_VOLUME_UP value drifted");
_Static_assert(KEY_VOLUME_DOWN == 25, "KEY_VOLUME_DOWN value drifted");

_Static_assert(sizeof(MouseButton) == sizeof(int), "MouseButton underlying ABI drifted");
_Static_assert(MOUSE_BUTTON_LEFT == 0, "MOUSE_BUTTON_LEFT value drifted");
_Static_assert(MOUSE_BUTTON_RIGHT == 1, "MOUSE_BUTTON_RIGHT value drifted");
_Static_assert(MOUSE_BUTTON_MIDDLE == 2, "MOUSE_BUTTON_MIDDLE value drifted");
_Static_assert(MOUSE_BUTTON_SIDE == 3, "MOUSE_BUTTON_SIDE value drifted");
_Static_assert(MOUSE_BUTTON_EXTRA == 4, "MOUSE_BUTTON_EXTRA value drifted");
_Static_assert(MOUSE_BUTTON_FORWARD == 5, "MOUSE_BUTTON_FORWARD value drifted");
_Static_assert(MOUSE_BUTTON_BACK == 6, "MOUSE_BUTTON_BACK value drifted");

_Static_assert(sizeof(TraceLogLevel) == sizeof(int), "TraceLogLevel underlying ABI drifted");
_Static_assert(LOG_ALL == 0, "LOG_ALL value drifted");
_Static_assert(LOG_TRACE == 1, "LOG_TRACE value drifted");
_Static_assert(LOG_DEBUG == 2, "LOG_DEBUG value drifted");
_Static_assert(LOG_INFO == 3, "LOG_INFO value drifted");
_Static_assert(LOG_WARNING == 4, "LOG_WARNING value drifted");
_Static_assert(LOG_ERROR == 5, "LOG_ERROR value drifted");
_Static_assert(LOG_FATAL == 6, "LOG_FATAL value drifted");
_Static_assert(LOG_NONE == 7, "LOG_NONE value drifted");

static void hxc_check_raylib_core_signatures(void);

static void
hxc_check_raylib_core_signatures(void)
{
    void (*const hxc_signature_BeginDrawing)(void) = &BeginDrawing;
    void (*const hxc_signature_BeginMode3D)(Camera3D) = &BeginMode3D;
    bool (*const hxc_signature_CheckCollisionBoxes)(BoundingBox, BoundingBox) = &CheckCollisionBoxes;
    void (*const hxc_signature_ClearBackground)(Color) = &ClearBackground;
    void (*const hxc_signature_CloseWindow)(void) = &CloseWindow;
    void (*const hxc_signature_DisableCursor)(void) = &DisableCursor;
    void (*const hxc_signature_DrawBillboardRec)(Camera, Texture2D, Rectangle, Vector3, Vector2, Color) = &DrawBillboardRec;
    void (*const hxc_signature_DrawCircle)(int, int, float, Color) = &DrawCircle;
    void (*const hxc_signature_DrawCube)(Vector3, float, float, float, Color) = &DrawCube;
    void (*const hxc_signature_DrawCubeV)(Vector3, Vector3, Color) = &DrawCubeV;
    void (*const hxc_signature_DrawCubeWires)(Vector3, float, float, float, Color) = &DrawCubeWires;
    void (*const hxc_signature_DrawCubeWiresV)(Vector3, Vector3, Color) = &DrawCubeWiresV;
    void (*const hxc_signature_DrawFPS)(int, int) = &DrawFPS;
    void (*const hxc_signature_DrawGrid)(int, float) = &DrawGrid;
    void (*const hxc_signature_DrawLine)(int, int, int, int, Color) = &DrawLine;
    void (*const hxc_signature_DrawLine3D)(Vector3, Vector3, Color) = &DrawLine3D;
    void (*const hxc_signature_DrawPixel)(int, int, Color) = &DrawPixel;
    void (*const hxc_signature_DrawRectangle)(int, int, int, int, Color) = &DrawRectangle;
    void (*const hxc_signature_DrawRectangleLines)(int, int, int, int, Color) = &DrawRectangleLines;
    void (*const hxc_signature_DrawSphere)(Vector3, float, Color) = &DrawSphere;
    void (*const hxc_signature_DrawSphereWires)(Vector3, float, int, int, Color) = &DrawSphereWires;
    void (*const hxc_signature_DrawText)(const char *, int, int, int, Color) = &DrawText;
    void (*const hxc_signature_DrawTexturePro)(Texture2D, Rectangle, Rectangle, Vector2, float, Color) = &DrawTexturePro;
    void (*const hxc_signature_EnableCursor)(void) = &EnableCursor;
    void (*const hxc_signature_EndDrawing)(void) = &EndDrawing;
    void (*const hxc_signature_EndMode3D)(void) = &EndMode3D;
    Color (*const hxc_signature_GetColor)(unsigned int) = &GetColor;
    int (*const hxc_signature_GetFPS)(void) = &GetFPS;
    float (*const hxc_signature_GetFrameTime)(void) = &GetFrameTime;
    Vector2 (*const hxc_signature_GetMouseDelta)(void) = &GetMouseDelta;
    Vector2 (*const hxc_signature_GetMousePosition)(void) = &GetMousePosition;
    float (*const hxc_signature_GetMouseWheelMove)(void) = &GetMouseWheelMove;
    RayCollision (*const hxc_signature_GetRayCollisionBox)(Ray, BoundingBox) = &GetRayCollisionBox;
    RayCollision (*const hxc_signature_GetRayCollisionSphere)(Ray, Vector3, float) = &GetRayCollisionSphere;
    int (*const hxc_signature_GetScreenHeight)(void) = &GetScreenHeight;
    Ray (*const hxc_signature_GetScreenToWorldRay)(Vector2, Camera) = &GetScreenToWorldRay;
    int (*const hxc_signature_GetScreenWidth)(void) = &GetScreenWidth;
    double (*const hxc_signature_GetTime)(void) = &GetTime;
    void (*const hxc_signature_InitWindow)(int, int, const char *) = &InitWindow;
    bool (*const hxc_signature_IsCursorHidden)(void) = &IsCursorHidden;
    bool (*const hxc_signature_IsKeyDown)(int) = &IsKeyDown;
    bool (*const hxc_signature_IsKeyPressed)(int) = &IsKeyPressed;
    bool (*const hxc_signature_IsKeyReleased)(int) = &IsKeyReleased;
    bool (*const hxc_signature_IsMouseButtonDown)(int) = &IsMouseButtonDown;
    bool (*const hxc_signature_IsMouseButtonPressed)(int) = &IsMouseButtonPressed;
    bool (*const hxc_signature_IsMouseButtonReleased)(int) = &IsMouseButtonReleased;
    bool (*const hxc_signature_IsTextureValid)(Texture2D) = &IsTextureValid;
    bool (*const hxc_signature_IsWindowFocused)(void) = &IsWindowFocused;
    bool (*const hxc_signature_IsWindowReady)(void) = &IsWindowReady;
    bool (*const hxc_signature_IsWindowResized)(void) = &IsWindowResized;
    Texture2D (*const hxc_signature_LoadTexture)(const char *) = &LoadTexture;
    void (*const hxc_signature_SetConfigFlags)(unsigned int) = &SetConfigFlags;
    void (*const hxc_signature_SetExitKey)(int) = &SetExitKey;
    void (*const hxc_signature_SetTargetFPS)(int) = &SetTargetFPS;
    void (*const hxc_signature_SetTraceLogLevel)(int) = &SetTraceLogLevel;
    void (*const hxc_signature_SetWindowMinSize)(int, int) = &SetWindowMinSize;
    void (*const hxc_signature_SetWindowSize)(int, int) = &SetWindowSize;
    void (*const hxc_signature_TakeScreenshot)(const char *) = &TakeScreenshot;
    void (*const hxc_signature_UnloadTexture)(Texture2D) = &UnloadTexture;
    bool (*const hxc_signature_WindowShouldClose)(void) = &WindowShouldClose;

    (void)hxc_signature_BeginDrawing;
    (void)hxc_signature_BeginMode3D;
    (void)hxc_signature_CheckCollisionBoxes;
    (void)hxc_signature_ClearBackground;
    (void)hxc_signature_CloseWindow;
    (void)hxc_signature_DisableCursor;
    (void)hxc_signature_DrawBillboardRec;
    (void)hxc_signature_DrawCircle;
    (void)hxc_signature_DrawCube;
    (void)hxc_signature_DrawCubeV;
    (void)hxc_signature_DrawCubeWires;
    (void)hxc_signature_DrawCubeWiresV;
    (void)hxc_signature_DrawFPS;
    (void)hxc_signature_DrawGrid;
    (void)hxc_signature_DrawLine;
    (void)hxc_signature_DrawLine3D;
    (void)hxc_signature_DrawPixel;
    (void)hxc_signature_DrawRectangle;
    (void)hxc_signature_DrawRectangleLines;
    (void)hxc_signature_DrawSphere;
    (void)hxc_signature_DrawSphereWires;
    (void)hxc_signature_DrawText;
    (void)hxc_signature_DrawTexturePro;
    (void)hxc_signature_EnableCursor;
    (void)hxc_signature_EndDrawing;
    (void)hxc_signature_EndMode3D;
    (void)hxc_signature_GetColor;
    (void)hxc_signature_GetFPS;
    (void)hxc_signature_GetFrameTime;
    (void)hxc_signature_GetMouseDelta;
    (void)hxc_signature_GetMousePosition;
    (void)hxc_signature_GetMouseWheelMove;
    (void)hxc_signature_GetRayCollisionBox;
    (void)hxc_signature_GetRayCollisionSphere;
    (void)hxc_signature_GetScreenHeight;
    (void)hxc_signature_GetScreenToWorldRay;
    (void)hxc_signature_GetScreenWidth;
    (void)hxc_signature_GetTime;
    (void)hxc_signature_InitWindow;
    (void)hxc_signature_IsCursorHidden;
    (void)hxc_signature_IsKeyDown;
    (void)hxc_signature_IsKeyPressed;
    (void)hxc_signature_IsKeyReleased;
    (void)hxc_signature_IsMouseButtonDown;
    (void)hxc_signature_IsMouseButtonPressed;
    (void)hxc_signature_IsMouseButtonReleased;
    (void)hxc_signature_IsTextureValid;
    (void)hxc_signature_IsWindowFocused;
    (void)hxc_signature_IsWindowReady;
    (void)hxc_signature_IsWindowResized;
    (void)hxc_signature_LoadTexture;
    (void)hxc_signature_SetConfigFlags;
    (void)hxc_signature_SetExitKey;
    (void)hxc_signature_SetTargetFPS;
    (void)hxc_signature_SetTraceLogLevel;
    (void)hxc_signature_SetWindowMinSize;
    (void)hxc_signature_SetWindowSize;
    (void)hxc_signature_TakeScreenshot;
    (void)hxc_signature_UnloadTexture;
    (void)hxc_signature_WindowShouldClose;
}

int
main(void)
{
    hxc_check_raylib_core_signatures();
    (void)puts("raylib-core-abi: OK");
    return 0;
}

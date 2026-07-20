#include "hxc/program.h"

void hxc_Main_main(void)
{
  SetTraceLogLevel(HXC_RAYLIB_SMOKE_LOG_NONE);
  InitWindow(HXC_RAYLIB_SMOKE_SIZE, HXC_RAYLIB_SMOKE_SIZE, "hxc-raylib-smoke");
  while (1)
  {
    bool hxc_tmp_native_call_result_n0 = IsWindowReady();
    if (!!hxc_tmp_native_call_result_n0)
    {
      break;
    }
  }
  float hxc_tmp_native_call_result_n1 = GetFrameTime();
  float hxc_frameTime = hxc_tmp_native_call_result_n1;
  (void)hxc_frameTime;
  double hxc_tmp_native_call_result_n2 = GetTime();
  double hxc_elapsed = hxc_tmp_native_call_result_n2;
  (void)hxc_elapsed;
  struct Vector2 hxc_tmp_native_call_result_n3 = GetMousePosition();
  struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n3;
  (void)hxc_mouse;
  bool hxc_tmp_native_call_result_n4 = IsKeyDown(HXC_RAYLIB_SMOKE_KEY_NULL);
  bool hxc_keyDown = hxc_tmp_native_call_result_n4;
  (void)hxc_keyDown;
  bool hxc_tmp_native_call_result_n5 = IsMouseButtonDown(HXC_RAYLIB_SMOKE_MOUSE_LEFT);
  bool hxc_mouseDown = hxc_tmp_native_call_result_n5;
  (void)hxc_mouseDown;
  struct Camera3D hxc_tmp_native_call_result_n6 = raylib_smoke_fixture_camera();
  struct Camera3D hxc_camera = hxc_tmp_native_call_result_n6;
  struct Vector2 hxc_tmp_native_call_result_n7 = raylib_smoke_fixture_screen_position();
  struct Vector2 hxc_screenPosition = hxc_tmp_native_call_result_n7;
  struct Vector2 hxc_tmp_load_result_n8 = hxc_screenPosition;
  struct Ray hxc_tmp_native_call_result_n10 = GetScreenToWorldRay(hxc_tmp_load_result_n8, hxc_camera);
  struct Ray hxc_worldRay = hxc_tmp_native_call_result_n10;
  (void)hxc_worldRay;
  struct Color hxc_tmp_native_call_result_n11 = GetColor(HXC_RAYLIB_SMOKE_COLOR);
  struct Color hxc_color = hxc_tmp_native_call_result_n11;
  BeginDrawing();
  ClearBackground(hxc_color);
  DrawPixel(HXC_RAYLIB_SMOKE_CENTER, HXC_RAYLIB_SMOKE_CENTER, hxc_color);
  EndDrawing();
  CloseWindow();
  return;
}

int main(void)
{
  hxc_Main_main();
  return 0;
}

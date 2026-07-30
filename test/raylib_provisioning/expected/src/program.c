#include "hxc/program.h"

void hxc_Main_main(void)
{
  SetTraceLogLevel(HXC_RAYLIB_SMOKE_LOG_NONE);
  InitWindow(HXC_RAYLIB_SMOKE_SIZE, HXC_RAYLIB_SMOKE_SIZE, "hxc-raylib-smoke");
  while (1)
  {
    bool hxc_l_tmp_native_call_result_n0 = IsWindowReady();
    if (!!hxc_l_tmp_native_call_result_n0)
    {
      break;
    }
  }
  float hxc_l_tmp_native_call_result_n1 = GetFrameTime();
  float hxc_l_frameTime = hxc_l_tmp_native_call_result_n1;
  (void)hxc_l_frameTime;
  double hxc_l_tmp_native_call_result_n2 = GetTime();
  double hxc_l_elapsed = hxc_l_tmp_native_call_result_n2;
  (void)hxc_l_elapsed;
  struct Vector2 hxc_l_tmp_native_call_result_n3 = GetMousePosition();
  struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n3;
  (void)hxc_l_mouse;
  bool hxc_l_tmp_native_call_result_n4 = IsKeyDown(HXC_RAYLIB_SMOKE_KEY_NULL);
  bool hxc_l_keyDown = hxc_l_tmp_native_call_result_n4;
  (void)hxc_l_keyDown;
  bool hxc_l_tmp_native_call_result_n5 = IsMouseButtonDown(HXC_RAYLIB_SMOKE_MOUSE_LEFT);
  bool hxc_l_mouseDown = hxc_l_tmp_native_call_result_n5;
  (void)hxc_l_mouseDown;
  struct Camera3D hxc_l_tmp_native_call_result_n6 = raylib_smoke_fixture_camera();
  struct Camera3D hxc_l_camera = hxc_l_tmp_native_call_result_n6;
  struct Vector2 hxc_l_tmp_native_call_result_n7 = raylib_smoke_fixture_screen_position();
  struct Vector2 hxc_l_screenPosition = hxc_l_tmp_native_call_result_n7;
  struct Vector2 hxc_l_tmp_load_result_n8 = hxc_l_screenPosition;
  struct Ray hxc_l_tmp_native_call_result_n10 = GetScreenToWorldRay(hxc_l_tmp_load_result_n8, hxc_l_camera);
  struct Ray hxc_l_worldRay = hxc_l_tmp_native_call_result_n10;
  (void)hxc_l_worldRay;
  struct Color hxc_l_tmp_native_call_result_n11 = GetColor(HXC_RAYLIB_SMOKE_COLOR);
  struct Color hxc_l_color = hxc_l_tmp_native_call_result_n11;
  BeginDrawing();
  ClearBackground(hxc_l_color);
  DrawPixel(HXC_RAYLIB_SMOKE_CENTER, HXC_RAYLIB_SMOKE_CENTER, hxc_l_color);
  EndDrawing();
  CloseWindow();
  return;
}

int main(void)
{
  hxc_Main_main();
  return 0;
}

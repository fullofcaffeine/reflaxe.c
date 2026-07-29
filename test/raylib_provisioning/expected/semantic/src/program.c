#include "hxc/program.h"

void hxc_Main_main(void)
{
  int32_t hxc_flags = hxc_i32_bit_or(64, 4);
  SetConfigFlags((uint32_t)hxc_flags);
  SetTraceLogLevel((int32_t)7);
  InitWindow((int32_t)64, (int32_t)64, "hxc-raylib-semantic");
  SetTargetFPS((int32_t)60);
  while (1)
  {
    bool hxc_tmp_native_call_result_n1 = IsWindowReady();
    if (!!hxc_tmp_native_call_result_n1)
    {
      break;
    }
  }
  struct Vector3 hxc_origin = (struct Vector3){ .x = (float)0.0, .y = (float)0.0, .z = (float)0.0 };
  struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)4.0, .y = (float)4.0, .z = (float)4.0 }, .target = hxc_origin, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)45.0, .projection = 0 };
  struct Vector2 hxc_cursor = (struct Vector2){ .x = (float)32.0, .y = (float)32.0 };
  struct Vector2 hxc_tmp_load_result_n8 = hxc_cursor;
  struct Ray hxc_tmp_native_call_result_n10 = GetScreenToWorldRay(hxc_tmp_load_result_n8, hxc_camera);
  struct Ray hxc_ray = hxc_tmp_native_call_result_n10;
  struct Vector3 hxc_rayOrigin = hxc_ray.position;
  struct Vector3 hxc_rayDirection = hxc_ray.direction;
  bool hxc_tmp_native_call_result_n15 = IsKeyDown((int32_t)87);
  bool hxc_keyDown = hxc_tmp_native_call_result_n15;
  (void)hxc_keyDown;
  bool hxc_tmp_native_call_result_n16 = IsMouseButtonDown((int32_t)0);
  bool hxc_mouseDown = hxc_tmp_native_call_result_n16;
  (void)hxc_mouseDown;
  struct Color hxc_accent = (struct Color){ .r = 245, .g = 166, .b = 35, .a = 255 };
  int32_t hxc_tmp_native_call_result_n18 = GetScreenWidth();
  int32_t hxc_a = (int32_t)hxc_tmp_native_call_result_n18;
  uint8_t hxc_red = (uint8_t)31;
  uint8_t hxc_green = (uint8_t)115;
  uint8_t hxc_blue = (uint8_t)154;
  int32_t hxc_tmp_load_result_n19 = hxc_a;
  int32_t hxc_tmp_conditional_result_n15 = 0;
  if (hxc_tmp_load_result_n19 < 0)
  {
    hxc_tmp_conditional_result_n15 = 0;
  }
  else
  {
    int32_t hxc_tmp_load_result_n20 = hxc_a;
    int32_t hxc_tmp_conditional_result_n16 = 0;
    if (hxc_tmp_load_result_n20 > 255)
    {
      hxc_tmp_conditional_result_n16 = 255;
    }
    else
    {
      hxc_tmp_conditional_result_n16 = hxc_a;
    }
    hxc_tmp_conditional_result_n15 = hxc_tmp_conditional_result_n16;
  }
  uint8_t hxc_alpha = (uint8_t)hxc_tmp_conditional_result_n15;
  uint8_t hxc_tmp_load_result_n24 = hxc_red;
  uint8_t hxc_tmp_load_result_n25 = hxc_green;
  uint8_t hxc_tmp_load_result_n26 = hxc_blue;
  struct Color hxc_fade = (struct Color){ .r = hxc_tmp_load_result_n24, .g = hxc_tmp_load_result_n25, .b = hxc_tmp_load_result_n26, .a = hxc_alpha };
  BeginDrawing();
  ClearBackground(RAYWHITE);
  BeginScissorMode((int32_t)0, (int32_t)0, (int32_t)64, (int32_t)64);
  DrawRectangle((int32_t)0, (int32_t)0, (int32_t)64, (int32_t)64, hxc_fade);
  BeginMode3D(hxc_camera);
  struct Vector3 hxc_tmp_load_result_n31 = hxc_origin;
  DrawCube(hxc_tmp_load_result_n31, (float)1.0, (float)1.0, (float)1.0, hxc_accent);
  EndMode3D();
  EndScissorMode();
  DrawText("Caxe: direct C, nicer Haxe", (int32_t)4, (int32_t)4, (int32_t)10, DARKGRAY);
  struct Vector3 hxc_tmp_load_result_n33 = hxc_rayOrigin;
  if ((double)hxc_tmp_load_result_n33.x + (double)hxc_rayDirection.x > 1000000.0)
  {
    DrawPixel((int32_t)1, (int32_t)0, hxc_accent);
  }
  bool hxc_tmp_native_call_result_n38 = FileExists("hxc-raylib-semantic.capture");
  if (hxc_tmp_native_call_result_n38)
  {
    DrawPixel((int32_t)0, (int32_t)0, hxc_accent);
  }
  EndDrawing();
  CloseWindow();
  return;
}

int main(void)
{
  hxc_Main_main();
  return 0;
}

#include "hxc/program.h"

void hxc_method_Main_main(void)
{
  SetTraceLogLevel(HXC_RAYLIB_SMOKE_LOG_NONE);
  InitWindow(HXC_RAYLIB_SMOKE_SIZE, HXC_RAYLIB_SMOKE_SIZE, "hxc-raylib-smoke");
  goto hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dcondition_n0;
hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dcondition_n0:
    ;
  bool hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n0 = IsWindowReady();
  if (!hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n0)
  {
    goto hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dbody_n1;
  }
  else
  {
    goto hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dexit_n2;
  }
hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dbody_n1:
    ;
  goto hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dcondition_n0;
hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dexit_n2:
    ;
  float hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n1 = GetFrameTime();
  float hxc_local_Main_main_frameTime_n0 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n1;
  (void)hxc_local_Main_main_frameTime_n0;
  double hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n2 = GetTime();
  double hxc_local_Main_main_elapsed_n1 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n2;
  (void)hxc_local_Main_main_elapsed_n1;
  struct Vector2 hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n3 = GetMousePosition();
  struct Vector2 hxc_local_Main_main_mouse_n2 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n3;
  (void)hxc_local_Main_main_mouse_n2;
  bool hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n4 = IsKeyDown(HXC_RAYLIB_SMOKE_KEY_NULL);
  bool hxc_local_Main_main_keyDown_n3 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n4;
  (void)hxc_local_Main_main_keyDown_n3;
  bool hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n5 = IsMouseButtonDown(HXC_RAYLIB_SMOKE_MOUSE_LEFT);
  bool hxc_local_Main_main_mouseDown_n4 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n5;
  (void)hxc_local_Main_main_mouseDown_n4;
  struct Camera3D hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n6 = raylib_smoke_fixture_camera();
  struct Camera3D hxc_local_Main_main_camera_n5 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n6;
  struct Vector2 hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n7 = raylib_smoke_fixture_screen_position();
  struct Vector2 hxc_local_Main_main_screenPosition_n6 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n7;
  struct Vector2 hxc_temp_Main_main_loadzx2Dresult_n8 = hxc_local_Main_main_screenPosition_n6;
  struct Camera3D hxc_temp_Main_main_loadzx2Dresult_n9 = hxc_local_Main_main_camera_n5;
  struct Ray hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n10 = GetScreenToWorldRay(hxc_temp_Main_main_loadzx2Dresult_n8, hxc_temp_Main_main_loadzx2Dresult_n9);
  struct Ray hxc_local_Main_main_worldRay_n7 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n10;
  (void)hxc_local_Main_main_worldRay_n7;
  struct Color hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n11 = GetColor(HXC_RAYLIB_SMOKE_COLOR);
  struct Color hxc_local_Main_main_color_n8 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n11;
  BeginDrawing();
  struct Color hxc_temp_Main_main_loadzx2Dresult_n12 = hxc_local_Main_main_color_n8;
  ClearBackground(hxc_temp_Main_main_loadzx2Dresult_n12);
  struct Color hxc_temp_Main_main_loadzx2Dresult_n13 = hxc_local_Main_main_color_n8;
  DrawPixel(HXC_RAYLIB_SMOKE_CENTER, HXC_RAYLIB_SMOKE_CENTER, hxc_temp_Main_main_loadzx2Dresult_n13);
  EndDrawing();
  CloseWindow();
  return;
}

int main(void)
{
  hxc_method_Main_main();
  return 0;
}

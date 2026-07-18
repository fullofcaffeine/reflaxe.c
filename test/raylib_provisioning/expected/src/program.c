#include "hxc/program.h"

void hxc_method_Main_main(void)
{
  SetTraceLogLevel(LOG_NONE);
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
  struct Color hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n1 = GetColor(HXC_RAYLIB_SMOKE_COLOR);
  struct Color hxc_local_Main_main_color_n0 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n1;
  BeginDrawing();
  struct Color hxc_temp_Main_main_loadzx2Dresult_n2 = hxc_local_Main_main_color_n0;
  ClearBackground(hxc_temp_Main_main_loadzx2Dresult_n2);
  struct Color hxc_temp_Main_main_loadzx2Dresult_n3 = hxc_local_Main_main_color_n0;
  DrawPixel(HXC_RAYLIB_SMOKE_CENTER, HXC_RAYLIB_SMOKE_CENTER, hxc_temp_Main_main_loadzx2Dresult_n3);
  EndDrawing();
  CloseWindow();
  return;
}

int main(void)
{
  hxc_method_Main_main();
  return 0;
}

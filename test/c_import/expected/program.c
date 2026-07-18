#include "hxc/program.h"

void hxc_method_Main_main(void)
{
  struct pointlib_point hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n0 = pointlib_point_make(POINTLIB_COORD_ONE, POINTLIB_COORD_NEGATIVE_THREE);
  struct pointlib_point hxc_local_Main_main_left_n0 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n0;
  struct pointlib_point hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n1 = pointlib_point_make(POINTLIB_COORD_FIVE, POINTLIB_COORD_SEVEN);
  struct pointlib_point hxc_local_Main_main_right_n1 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n1;
  hxc_local_Main_main_left_n0.x = POINTLIB_COORD_ONE;
  pointlib_coord *hxc_temp_Main_main_importedzx2Dfieldzx2Daddress_n2 = &hxc_local_Main_main_left_n0.x;
  pointlib_coord hxc_temp_Main_main_importedzx2Dfieldzx2Dloadzx2Dresult_n3 = *hxc_temp_Main_main_importedzx2Dfieldzx2Daddress_n2;
  pointlib_coord hxc_local_Main_main_delta_n2 = hxc_temp_Main_main_importedzx2Dfieldzx2Dloadzx2Dresult_n3;
  struct pointlib_point hxc_temp_Main_main_loadzx2Dresult_n4 = hxc_local_Main_main_left_n0;
  pointlib_coord hxc_temp_Main_main_loadzx2Dresult_n5 = hxc_local_Main_main_delta_n2;
  struct pointlib_point hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n6 = pointlib_point_translate(hxc_temp_Main_main_loadzx2Dresult_n4, hxc_temp_Main_main_loadzx2Dresult_n5, POINTLIB_COORD_FIVE);
  hxc_local_Main_main_left_n0 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n6;
  struct pointlib_point hxc_temp_Main_main_loadzx2Dresult_n7 = hxc_local_Main_main_left_n0;
  struct pointlib_point hxc_temp_Main_main_loadzx2Dresult_n8 = hxc_local_Main_main_right_n1;
  int64_t hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n9 = pointlib_point_dot(hxc_temp_Main_main_loadzx2Dresult_n7, hxc_temp_Main_main_loadzx2Dresult_n8);
  int64_t hxc_local_Main_main_dot_n3 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n9;
  pointlib_axis hxc_local_Main_main_axis_n4 = POINTLIB_AXIS_Y;
  struct pointlib_point hxc_temp_Main_main_loadzx2Dresult_n10 = hxc_local_Main_main_left_n0;
  pointlib_axis hxc_temp_Main_main_loadzx2Dresult_n11 = hxc_local_Main_main_axis_n4;
  pointlib_coord hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n12 = pointlib_point_component(hxc_temp_Main_main_loadzx2Dresult_n10, hxc_temp_Main_main_loadzx2Dresult_n11);
  pointlib_coord hxc_local_Main_main_component_n5 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n12;
  goto hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dcondition_n0;
hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dcondition_n0:
    ;
  struct pointlib_point hxc_temp_Main_main_loadzx2Dresult_n13 = hxc_local_Main_main_left_n0;
  struct pointlib_point hxc_temp_Main_main_loadzx2Dresult_n14 = hxc_local_Main_main_right_n1;
  int64_t hxc_temp_Main_main_loadzx2Dresult_n15 = hxc_local_Main_main_dot_n3;
  pointlib_coord hxc_temp_Main_main_loadzx2Dresult_n16 = hxc_local_Main_main_component_n5;
  pointlib_axis hxc_temp_Main_main_loadzx2Dresult_n17 = hxc_local_Main_main_axis_n4;
  bool hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n18 = pointlib_point_verify(hxc_temp_Main_main_loadzx2Dresult_n13, hxc_temp_Main_main_loadzx2Dresult_n14, hxc_temp_Main_main_loadzx2Dresult_n15, hxc_temp_Main_main_loadzx2Dresult_n16, hxc_temp_Main_main_loadzx2Dresult_n17, "c-import-\303\251");
  if (!hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n18)
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
  return;
}

int main(void)
{
  hxc_method_Main_main();
  return 0;
}

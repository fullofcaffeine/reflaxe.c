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
  struct pointlib_float_point hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n13 = pointlib_float_point_make(POINTLIB_FLOAT_ONE_POINT_FIVE, POINTLIB_FLOAT_NEGATIVE_TWO);
  struct pointlib_float_point hxc_local_Main_main_floatPoint_n6 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n13;
  hxc_local_Main_main_floatPoint_n6.x = (float)0.5;
  struct pointlib_float_point hxc_temp_Main_main_loadzx2Dresult_n14 = hxc_local_Main_main_floatPoint_n6;
  struct pointlib_float_point hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n15 = pointlib_float_point_scale(hxc_temp_Main_main_loadzx2Dresult_n14, (float)2.0);
  hxc_local_Main_main_floatPoint_n6 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n15;
  struct pointlib_float_point hxc_temp_Main_main_loadzx2Dresult_n16 = hxc_local_Main_main_floatPoint_n6;
  struct pointlib_float_point hxc_temp_Main_main_loadzx2Dresult_n17 = hxc_local_Main_main_floatPoint_n6;
  float hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n18 = pointlib_float_point_dot(hxc_temp_Main_main_loadzx2Dresult_n16, hxc_temp_Main_main_loadzx2Dresult_n17);
  float hxc_local_Main_main_floatDot_n7 = hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n18;
  float hxc_temp_Main_main_loadzx2Dresult_n19 = hxc_local_Main_main_floatDot_n7;
  double hxc_local_Main_main_widened_n8 = (double)hxc_temp_Main_main_loadzx2Dresult_n19;
  float hxc_local_Main_main_tie_n9 = (float)1.000000059604644775390625;
  float hxc_local_Main_main_subnormal_n10 = (float)1.401298464324817e-45;
  float hxc_local_Main_main_positiveInfinity_n11 = (float)hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe(1.0, 0.0);
  float hxc_local_Main_main_nan_n12 = (float)hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe(0.0, 0.0);
  float hxc_local_Main_main_negativezZero_n13 = (float)-0.0;
  float hxc_local_Main_main_finiteOverflow_n14 = (float)1.0e300;
  goto hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dcondition_n0;
hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dcondition_n0:
    ;
  struct pointlib_point hxc_temp_Main_main_loadzx2Dresult_n20 = hxc_local_Main_main_left_n0;
  struct pointlib_point hxc_temp_Main_main_loadzx2Dresult_n21 = hxc_local_Main_main_right_n1;
  int64_t hxc_temp_Main_main_loadzx2Dresult_n22 = hxc_local_Main_main_dot_n3;
  pointlib_coord hxc_temp_Main_main_loadzx2Dresult_n23 = hxc_local_Main_main_component_n5;
  pointlib_axis hxc_temp_Main_main_loadzx2Dresult_n24 = hxc_local_Main_main_axis_n4;
  bool hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n25 = pointlib_point_verify(hxc_temp_Main_main_loadzx2Dresult_n20, hxc_temp_Main_main_loadzx2Dresult_n21, hxc_temp_Main_main_loadzx2Dresult_n22, hxc_temp_Main_main_loadzx2Dresult_n23, hxc_temp_Main_main_loadzx2Dresult_n24, "c-import-\303\251");
  bool hxc_temp_Main_main_shortzx2Dcircuitzx2Dresult_n15 = !hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n25;
  if (!hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n25)
  {
    goto hxc_temp_Main_main_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n4;
  }
  else
  {
    goto hxc_temp_Main_main_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n3;
  }
hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dbody_n1:
    ;
  goto hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dcondition_n0;
hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dexit_n2:
    ;
  return;
hxc_temp_Main_main_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n3:
    ;
  struct pointlib_float_point hxc_temp_Main_main_loadzx2Dresult_n26 = hxc_local_Main_main_floatPoint_n6;
  float hxc_temp_Main_main_loadzx2Dresult_n27 = hxc_local_Main_main_floatDot_n7;
  double hxc_temp_Main_main_loadzx2Dresult_n28 = hxc_local_Main_main_widened_n8;
  float hxc_temp_Main_main_loadzx2Dresult_n29 = hxc_local_Main_main_tie_n9;
  float hxc_temp_Main_main_loadzx2Dresult_n30 = hxc_local_Main_main_subnormal_n10;
  float hxc_temp_Main_main_loadzx2Dresult_n31 = hxc_local_Main_main_positiveInfinity_n11;
  float hxc_temp_Main_main_loadzx2Dresult_n32 = hxc_local_Main_main_nan_n12;
  float hxc_temp_Main_main_loadzx2Dresult_n33 = hxc_local_Main_main_negativezZero_n13;
  float hxc_temp_Main_main_loadzx2Dresult_n34 = hxc_local_Main_main_finiteOverflow_n14;
  bool hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n35 = pointlib_float32_verify(hxc_temp_Main_main_loadzx2Dresult_n26, hxc_temp_Main_main_loadzx2Dresult_n27, hxc_temp_Main_main_loadzx2Dresult_n28, hxc_temp_Main_main_loadzx2Dresult_n29, hxc_temp_Main_main_loadzx2Dresult_n30, hxc_temp_Main_main_loadzx2Dresult_n31, hxc_temp_Main_main_loadzx2Dresult_n32, hxc_temp_Main_main_loadzx2Dresult_n33, hxc_temp_Main_main_loadzx2Dresult_n34);
  hxc_temp_Main_main_shortzx2Dcircuitzx2Dresult_n15 = !hxc_temp_Main_main_nativezx2Dcallzx2Dresult_n35;
  goto hxc_temp_Main_main_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n4;
hxc_temp_Main_main_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n4:
    ;
  bool hxc_temp_Main_main_shortzx2Dcircuitzx2Dloadzx2Dresult_n36 = hxc_temp_Main_main_shortzx2Dcircuitzx2Dresult_n15;
  if (hxc_temp_Main_main_shortzx2Dcircuitzx2Dloadzx2Dresult_n36)
  {
    goto hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dbody_n1;
  }
  else
  {
    goto hxc_temp_Main_main_blockzx2Dlabel_whilezx2Dexit_n2;
  }
}

int main(void)
{
  hxc_method_Main_main();
  return 0;
}

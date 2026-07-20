#include "hxc/program.h"

struct pointlib_point hxc_Main_localPoint(pointlib_coord hxc_x, pointlib_coord hxc_y)
{
  return (struct pointlib_point){ .x = hxc_x, .y = hxc_y };
}

void hxc_Main_main(void)
{
  pointlib_build_fact_probe();
  struct pointlib_point hxc_tmp_native_call_result_n0 = pointlib_point_make(POINTLIB_COORD_ONE, POINTLIB_COORD_NEGATIVE_THREE);
  struct pointlib_point hxc_left = hxc_tmp_native_call_result_n0;
  struct pointlib_point hxc_tmp_call_result_n1 = hxc_Main_localPoint(POINTLIB_COORD_FIVE, POINTLIB_COORD_SEVEN);
  struct pointlib_point hxc_right = hxc_tmp_call_result_n1;
  hxc_left.x = POINTLIB_COORD_ONE;
  pointlib_coord *hxc_tmp_imported_field_address_n2 = &hxc_left.x;
  pointlib_coord hxc_tmp_imported_field_load_result_n3 = *hxc_tmp_imported_field_address_n2;
  pointlib_coord hxc_delta = hxc_tmp_imported_field_load_result_n3;
  struct pointlib_point hxc_tmp_load_result_n4 = hxc_left;
  struct pointlib_point hxc_tmp_native_call_result_n6 = pointlib_point_translate(hxc_tmp_load_result_n4, hxc_delta, POINTLIB_COORD_FIVE);
  hxc_left = hxc_tmp_native_call_result_n6;
  struct pointlib_point hxc_tmp_native_call_result_n8 = pointlib_point_alias_identity(hxc_left);
  struct pointlib_point hxc_pointAlias = hxc_tmp_native_call_result_n8;
  (void)hxc_pointAlias;
  struct pointlib_point hxc_tmp_load_result_n9 = hxc_left;
  int64_t hxc_tmp_native_call_result_n11 = pointlib_point_dot(hxc_tmp_load_result_n9, hxc_right);
  int64_t hxc_dot = hxc_tmp_native_call_result_n11;
  pointlib_axis hxc_axis = POINTLIB_AXIS_Y;
  struct pointlib_point hxc_tmp_load_result_n12 = hxc_left;
  pointlib_coord hxc_tmp_native_call_result_n14 = pointlib_point_component(hxc_tmp_load_result_n12, hxc_axis);
  pointlib_coord hxc_component = hxc_tmp_native_call_result_n14;
  struct pointlib_float_point hxc_tmp_native_call_result_n15 = pointlib_float_point_make(POINTLIB_FLOAT_ONE_POINT_FIVE, POINTLIB_FLOAT_NEGATIVE_TWO);
  struct pointlib_float_point hxc_floatPoint = hxc_tmp_native_call_result_n15;
  hxc_floatPoint.x = (float)0.5;
  struct pointlib_float_point hxc_tmp_native_call_result_n17 = pointlib_float_point_scale(hxc_floatPoint, (float)2.0);
  hxc_floatPoint = hxc_tmp_native_call_result_n17;
  struct pointlib_float_point hxc_tmp_load_result_n18 = hxc_floatPoint;
  float hxc_tmp_native_call_result_n20 = pointlib_float_point_dot(hxc_tmp_load_result_n18, hxc_floatPoint);
  float hxc_floatDot = hxc_tmp_native_call_result_n20;
  double hxc_widened = (double)hxc_floatDot;
  float hxc_tie = (float)1.000000059604644775390625;
  float hxc_subnormal = (float)1.401298464324817e-45;
  float hxc_positiveInfinity = (float)hxc_f64_divide_zero_safe(1.0, 0.0);
  float hxc_nan = (float)hxc_f64_divide_zero_safe(0.0, 0.0);
  float hxc_negativeZero = (float)-0.0;
  float hxc_finiteOverflow = (float)1.0e300;
  while (1)
  {
    struct pointlib_point hxc_tmp_load_result_n22 = hxc_left;
    struct pointlib_point hxc_tmp_load_result_n23 = hxc_right;
    int64_t hxc_tmp_load_result_n24 = hxc_dot;
    pointlib_coord hxc_tmp_load_result_n25 = hxc_component;
    bool hxc_tmp_native_call_result_n27 = pointlib_point_verify(hxc_tmp_load_result_n22, hxc_tmp_load_result_n23, hxc_tmp_load_result_n24, hxc_tmp_load_result_n25, hxc_axis, "c-import-\303\251");
    bool hxc_tmp_short_circuit_result_n16 = !hxc_tmp_native_call_result_n27;
    if (!!hxc_tmp_native_call_result_n27)
    {
      struct pointlib_float_point hxc_tmp_load_result_n28 = hxc_floatPoint;
      float hxc_tmp_load_result_n29 = hxc_floatDot;
      double hxc_tmp_load_result_n30 = hxc_widened;
      float hxc_tmp_load_result_n31 = hxc_tie;
      float hxc_tmp_load_result_n32 = hxc_subnormal;
      float hxc_tmp_load_result_n33 = hxc_positiveInfinity;
      float hxc_tmp_load_result_n34 = hxc_nan;
      float hxc_tmp_load_result_n35 = hxc_negativeZero;
      bool hxc_tmp_native_call_result_n37 = pointlib_float32_verify(hxc_tmp_load_result_n28, hxc_tmp_load_result_n29, hxc_tmp_load_result_n30, hxc_tmp_load_result_n31, hxc_tmp_load_result_n32, hxc_tmp_load_result_n33, hxc_tmp_load_result_n34, hxc_tmp_load_result_n35, hxc_finiteOverflow);
      hxc_tmp_short_circuit_result_n16 = !hxc_tmp_native_call_result_n37;
    }
    if (!hxc_tmp_short_circuit_result_n16)
    {
      break;
    }
  }
  return;
}

int main(void)
{
  hxc_Main_main();
  return 0;
}

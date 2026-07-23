#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_PointResources, hxc_point) == 0, "closed record hxc_PointResources first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_PointResources) >= _Alignof(struct pointlib_point), "closed record hxc_PointResources alignment admits field 0");

_Static_assert(offsetof(struct hxc_PointResources, hxc_ready) >= offsetof(struct hxc_PointResources, hxc_point) + sizeof(struct pointlib_point), "closed record hxc_PointResources field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_PointResources) >= _Alignof(bool), "closed record hxc_PointResources alignment admits field 1");

_Static_assert(sizeof(struct hxc_PointResources) >= offsetof(struct hxc_PointResources, hxc_ready) + sizeof(bool), "closed record hxc_PointResources size contains its final field");

int32_t hxc_InlineFloat32Probe_sideEffectCount;

float hxc_InlineFloat32Probe_narrowWithoutInlining(double hxc_value)
{
  return (float)hxc_value;
}

int32_t hxc_InlineFloat32Probe_nextInteger(void)
{
  int32_t hxc_tmp_increment_load_result_n0 = hxc_InlineFloat32Probe_sideEffectCount;
  hxc_InlineFloat32Probe_sideEffectCount = hxc_i32_add_wrapping(hxc_tmp_increment_load_result_n0, 1);
  return 7;
}

bool hxc_InlineFloat32Probe_run(void)
{
  int32_t hxc_integerLocal = 3;
  double hxc_floatLocal = 6.5;
  float hxc_fromIntegerLocal = (float)(double)hxc_integerLocal;
  float hxc_fromIntegerArithmetic = (float)(double)hxc_i32_add_wrapping(hxc_integerLocal, 2);
  float hxc_fromFloatLocal = (float)hxc_floatLocal;
  int32_t hxc_tmp_call_result_n3 = hxc_InlineFloat32Probe_nextInteger();
  float hxc_fromSideEffect = (float)(double)hxc_tmp_call_result_n3;
  float hxc_tmp_call_result_n5 = hxc_InlineFloat32Probe_narrowWithoutInlining((double)hxc_i32_add_wrapping(hxc_integerLocal, 5));
  float hxc_fromNonInline = hxc_tmp_call_result_n5;
  int32_t hxc_tmp_global_load_result_n6 = hxc_InlineFloat32Probe_sideEffectCount;
  bool hxc_tmp_short_circuit_result_n7 = hxc_tmp_global_load_result_n6 == 1;
  if (hxc_tmp_global_load_result_n6 == 1)
  {
    float hxc_tmp_load_result_n7 = hxc_fromIntegerLocal;
    float hxc_tmp_load_result_n8 = hxc_fromIntegerArithmetic;
    float hxc_tmp_load_result_n9 = hxc_fromFloatLocal;
    float hxc_tmp_load_result_n10 = hxc_fromSideEffect;
    bool hxc_tmp_native_call_result_n12 = pointlib_inline_float32_verify(hxc_tmp_load_result_n7, hxc_tmp_load_result_n8, hxc_tmp_load_result_n9, hxc_tmp_load_result_n10, hxc_fromNonInline);
    hxc_tmp_short_circuit_result_n7 = hxc_tmp_native_call_result_n12;
  }
  return hxc_tmp_short_circuit_result_n7;
}

struct pointlib_point hxc_Main_localPoint(pointlib_coord hxc_y, bool hxc_useY)
{
  struct pointlib_point hxc_tmp_native_call_result_n0 = pointlib_point_make(POINTLIB_COORD_FIVE, POINTLIB_COORD_SEVEN);
  pointlib_coord hxc_tmp_native_call_result_n1 = pointlib_point_component(hxc_tmp_native_call_result_n0, POINTLIB_AXIS_X);
  pointlib_coord hxc_tmp = hxc_tmp_native_call_result_n1;
  pointlib_coord hxc_selected = hxc_y;
  if (!hxc_useY)
  {
    hxc_selected = POINTLIB_COORD_ONE;
  }
  pointlib_coord hxc_tmp_load_result_n2 = hxc_tmp;
  return (struct pointlib_point){ .x = hxc_tmp_load_result_n2, .y = hxc_selected };
}

const char *hxc_Main_localizedLabel(int32_t hxc_locale, int32_t hxc_message)
{
  if (!(hxc_locale == 0))
  {
    return "other-locale";
  }
  const char *hxc_tmp_call_result_n0 = hxc_Main_localizedMessage(hxc_message);
  return hxc_tmp_call_result_n0;
}

const char *hxc_Main_localizedMessage(int32_t hxc_message)
{
  if (!(hxc_message == 0))
  {
    return "other-message";
  }
  return "c-import-\303\251";
}

void hxc_Main_main(void)
{
  pointlib_build_fact_probe();
  struct pointlib_point hxc_tmp_native_call_result_n0 = pointlib_point_make(POINTLIB_COORD_ONE, POINTLIB_COORD_NEGATIVE_THREE);
  struct pointlib_point hxc_left = hxc_tmp_native_call_result_n0;
  struct pointlib_point hxc_tmp_call_result_n1 = hxc_Main_localPoint(POINTLIB_COORD_SEVEN, true);
  struct hxc_PointResources hxc_tmp_call_result_n2 = hxc_Main_pointResources(hxc_tmp_call_result_n1);
  struct hxc_PointResources hxc_resources = hxc_tmp_call_result_n2;
  struct pointlib_point hxc_right = hxc_resources.hxc_point;
  hxc_left.x = POINTLIB_COORD_ONE;
  pointlib_coord *hxc_tmp_imported_field_address_n4 = &hxc_left.x;
  pointlib_coord hxc_tmp_imported_field_load_result_n5 = *hxc_tmp_imported_field_address_n4;
  pointlib_coord hxc_delta = hxc_tmp_imported_field_load_result_n5;
  struct pointlib_point hxc_tmp_load_result_n6 = hxc_left;
  struct pointlib_point hxc_tmp_native_call_result_n8 = pointlib_point_translate(hxc_tmp_load_result_n6, hxc_delta, POINTLIB_COORD_FIVE);
  hxc_left = hxc_tmp_native_call_result_n8;
  struct pointlib_point hxc_tmp_native_call_result_n10 = pointlib_point_alias_identity(hxc_left);
  struct pointlib_point hxc_pointAlias = hxc_tmp_native_call_result_n10;
  (void)hxc_pointAlias;
  struct pointlib_point hxc_tmp_load_result_n11 = hxc_left;
  int64_t hxc_tmp_native_call_result_n13 = pointlib_point_dot(hxc_tmp_load_result_n11, hxc_right);
  int64_t hxc_dot = hxc_tmp_native_call_result_n13;
  pointlib_axis hxc_axis = POINTLIB_AXIS_Y;
  struct pointlib_point hxc_tmp_load_result_n14 = hxc_left;
  pointlib_coord hxc_tmp_native_call_result_n16 = pointlib_point_component(hxc_tmp_load_result_n14, hxc_axis);
  pointlib_coord hxc_component = hxc_tmp_native_call_result_n16;
  struct pointlib_float_point hxc_tmp_native_call_result_n17 = pointlib_float_point_make(POINTLIB_FLOAT_ONE_POINT_FIVE, POINTLIB_FLOAT_NEGATIVE_TWO);
  struct pointlib_float_point hxc_floatPoint = hxc_tmp_native_call_result_n17;
  hxc_floatPoint.x = (float)0.5;
  struct pointlib_float_point hxc_tmp_native_call_result_n19 = pointlib_float_point_scale(hxc_floatPoint, (float)2.0);
  hxc_floatPoint = hxc_tmp_native_call_result_n19;
  struct pointlib_float_point hxc_tmp_load_result_n20 = hxc_floatPoint;
  float hxc_tmp_native_call_result_n22 = pointlib_float_point_dot(hxc_tmp_load_result_n20, hxc_floatPoint);
  float hxc_floatDot = hxc_tmp_native_call_result_n22;
  double hxc_widened = (double)hxc_floatDot;
  float hxc_tie = (float)1.000000059604644775390625;
  float hxc_subnormal = (float)1.401298464324817e-45;
  float hxc_positiveInfinity = (float)hxc_f64_divide_zero_safe(1.0, 0.0);
  float hxc_nan = (float)hxc_f64_divide_zero_safe(0.0, 0.0);
  float hxc_negativeZero = (float)-0.0;
  float hxc_finiteOverflow = (float)1.0e300;
  int32_t hxc_locale = 0;
  while (1)
  {
    bool hxc_tmp_record_field_load_result_n24 = hxc_resources.hxc_ready;
    bool hxc_tmp_short_circuit_result_n18 = !hxc_tmp_record_field_load_result_n24;
    if (!!hxc_tmp_record_field_load_result_n24)
    {
      struct pointlib_point hxc_tmp_load_result_n25 = hxc_left;
      struct pointlib_point hxc_tmp_load_result_n26 = hxc_right;
      int64_t hxc_tmp_load_result_n27 = hxc_dot;
      pointlib_coord hxc_tmp_load_result_n28 = hxc_component;
      pointlib_axis hxc_tmp_load_result_n29 = hxc_axis;
      const char *hxc_tmp_call_result_n31 = hxc_Main_localizedLabel(hxc_locale, 0);
      bool hxc_tmp_native_call_result_n32 = pointlib_point_verify(hxc_tmp_load_result_n25, hxc_tmp_load_result_n26, hxc_tmp_load_result_n27, hxc_tmp_load_result_n28, hxc_tmp_load_result_n29, hxc_tmp_call_result_n31);
      hxc_tmp_short_circuit_result_n18 = !hxc_tmp_native_call_result_n32;
    }
    bool hxc_tmp_short_circuit_load_result_n33 = hxc_tmp_short_circuit_result_n18;
    bool hxc_tmp_short_circuit_result_n19 = hxc_tmp_short_circuit_load_result_n33;
    if (!hxc_tmp_short_circuit_load_result_n33)
    {
      struct pointlib_float_point hxc_tmp_load_result_n34 = hxc_floatPoint;
      float hxc_tmp_load_result_n35 = hxc_floatDot;
      double hxc_tmp_load_result_n36 = hxc_widened;
      float hxc_tmp_load_result_n37 = hxc_tie;
      float hxc_tmp_load_result_n38 = hxc_subnormal;
      float hxc_tmp_load_result_n39 = hxc_positiveInfinity;
      float hxc_tmp_load_result_n40 = hxc_nan;
      float hxc_tmp_load_result_n41 = hxc_negativeZero;
      bool hxc_tmp_native_call_result_n43 = pointlib_float32_verify(hxc_tmp_load_result_n34, hxc_tmp_load_result_n35, hxc_tmp_load_result_n36, hxc_tmp_load_result_n37, hxc_tmp_load_result_n38, hxc_tmp_load_result_n39, hxc_tmp_load_result_n40, hxc_tmp_load_result_n41, hxc_finiteOverflow);
      hxc_tmp_short_circuit_result_n19 = !hxc_tmp_native_call_result_n43;
    }
    bool hxc_tmp_short_circuit_load_result_n44 = hxc_tmp_short_circuit_result_n19;
    bool hxc_tmp_short_circuit_result_n20 = hxc_tmp_short_circuit_load_result_n44;
    if (!hxc_tmp_short_circuit_load_result_n44)
    {
      bool hxc_tmp_call_result_n45 = hxc_InlineFloat32Probe_run();
      hxc_tmp_short_circuit_result_n20 = !hxc_tmp_call_result_n45;
    }
    if (!hxc_tmp_short_circuit_result_n20)
    {
      break;
    }
  }
  return;
}

struct hxc_PointResources hxc_Main_pointResources(struct pointlib_point hxc_point)
{
  return (struct hxc_PointResources){ .hxc_point = hxc_point, .hxc_ready = true };
}

void hxc_init_compiler_static_initialization_InlineFloat32Probe_static_field_sideEffectCount(void)
{
  hxc_InlineFloat32Probe_sideEffectCount = 0;
  return;
}

static void hxc_init_compiler_static_initialization_hosted_executable_function_Main_main(void)
{
  hxc_init_compiler_static_initialization_InlineFloat32Probe_static_field_sideEffectCount();
}

int main(void)
{
  hxc_init_compiler_static_initialization_hosted_executable_function_Main_main();
  hxc_Main_main();
  return 0;
}

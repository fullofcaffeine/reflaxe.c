#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_HiddenPointResources, hxc_point) == 0, "closed record hxc_HiddenPointResources first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_HiddenPointResources) >= _Alignof(struct pointlib_hidden_point), "closed record hxc_HiddenPointResources alignment admits field 0");

_Static_assert(offsetof(struct hxc_HiddenPointResources, hxc_ready) >= offsetof(struct hxc_HiddenPointResources, hxc_point) + sizeof(struct pointlib_hidden_point), "closed record hxc_HiddenPointResources field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_HiddenPointResources) >= _Alignof(bool), "closed record hxc_HiddenPointResources alignment admits field 1");

_Static_assert(sizeof(struct hxc_HiddenPointResources) >= offsetof(struct hxc_HiddenPointResources, hxc_ready) + sizeof(bool), "closed record hxc_HiddenPointResources size contains its final field");

_Static_assert(offsetof(struct hxc_PointResources, hxc_point) == 0, "closed record hxc_PointResources first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_PointResources) >= _Alignof(struct pointlib_point), "closed record hxc_PointResources alignment admits field 0");

_Static_assert(offsetof(struct hxc_PointResources, hxc_ready) >= offsetof(struct hxc_PointResources, hxc_point) + sizeof(struct pointlib_point), "closed record hxc_PointResources field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_PointResources) >= _Alignof(bool), "closed record hxc_PointResources alignment admits field 1");

_Static_assert(sizeof(struct hxc_PointResources) >= offsetof(struct hxc_PointResources, hxc_ready) + sizeof(bool), "closed record hxc_PointResources size contains its final field");

_Static_assert(offsetof(struct hxc_PointOwner, hxc_point) == 0, "class hxc_PointOwner first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_PointOwner) >= _Alignof(struct pointlib_point), "class hxc_PointOwner alignment admits field 0");

_Static_assert(sizeof(struct hxc_PointOwner) >= offsetof(struct hxc_PointOwner, hxc_point) + sizeof(struct pointlib_point), "class hxc_PointOwner size contains its final storage member");

_Static_assert(offsetof(struct hxc_HiddenPointOwner, hxc_point) == 0, "class hxc_HiddenPointOwner first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_HiddenPointOwner) >= _Alignof(struct pointlib_hidden_point), "class hxc_HiddenPointOwner alignment admits field 0");

_Static_assert(sizeof(struct hxc_HiddenPointOwner) >= offsetof(struct hxc_HiddenPointOwner, hxc_point) + sizeof(struct pointlib_hidden_point), "class hxc_HiddenPointOwner size contains its final storage member");

int32_t hxc_InlineFloat32Probe_sideEffectCount;

void hxc_compiler_constructor_HiddenPointOwner(struct hxc_HiddenPointOwner *hxc_l_self, struct pointlib_hidden_point hxc_l_point)
{
  (*hxc_l_self).hxc_point = hxc_l_point;
  return;
}

void hxc_compiler_constructor_PointOwner(struct hxc_PointOwner *hxc_l_self, struct pointlib_point hxc_l_point)
{
  (*hxc_l_self).hxc_point = hxc_l_point;
  return;
}

float hxc_InlineFloat32Probe_narrowWithoutInlining(double hxc_l_value)
{
  return (float)hxc_l_value;
}

int32_t hxc_InlineFloat32Probe_nextInteger(void)
{
  int32_t hxc_l_tmp_increment_load_result_n0 = hxc_InlineFloat32Probe_sideEffectCount;
  hxc_InlineFloat32Probe_sideEffectCount = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n0, 1);
  return 7;
}

bool hxc_InlineFloat32Probe_run(void)
{
  int32_t hxc_l_integerLocal = 3;
  double hxc_l_floatLocal = 6.5;
  float hxc_l_fromIntegerLocal = (float)(double)hxc_l_integerLocal;
  float hxc_l_fromIntegerArithmetic = (float)(double)hxc_i32_add_wrapping(hxc_l_integerLocal, 2);
  float hxc_l_fromFloatLocal = (float)hxc_l_floatLocal;
  int32_t hxc_l_tmp_call_result_n3 = hxc_InlineFloat32Probe_nextInteger();
  float hxc_l_fromSideEffect = (float)(double)hxc_l_tmp_call_result_n3;
  float hxc_l_tmp_call_result_n5 = hxc_InlineFloat32Probe_narrowWithoutInlining((double)hxc_i32_add_wrapping(hxc_l_integerLocal, 5));
  float hxc_l_fromNonInline = hxc_l_tmp_call_result_n5;
  int32_t hxc_l_tmp_global_load_result_n6 = hxc_InlineFloat32Probe_sideEffectCount;
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_global_load_result_n6 == 1;
  if (hxc_l_tmp_global_load_result_n6 == 1)
  {
    float hxc_l_tmp_load_result_n7 = hxc_l_fromIntegerLocal;
    float hxc_l_tmp_load_result_n8 = hxc_l_fromIntegerArithmetic;
    float hxc_l_tmp_load_result_n9 = hxc_l_fromFloatLocal;
    float hxc_l_tmp_load_result_n10 = hxc_l_fromSideEffect;
    bool hxc_l_tmp_native_call_result_n12 = pointlib_inline_float32_verify(hxc_l_tmp_load_result_n7, hxc_l_tmp_load_result_n8, hxc_l_tmp_load_result_n9, hxc_l_tmp_load_result_n10, hxc_l_fromNonInline);
    hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_native_call_result_n12;
  }
  return hxc_l_tmp_short_circuit_result_n7;
}

struct hxc_HiddenPointResources hxc_Main_hiddenPointResources(struct pointlib_hidden_point hxc_l_point)
{
  return (struct hxc_HiddenPointResources){ .hxc_point = hxc_l_point, .hxc_ready = true };
}

struct pointlib_point hxc_Main_localPoint(pointlib_coord hxc_l_y, bool hxc_l_useY)
{
  struct pointlib_point hxc_l_tmp_native_call_result_n0 = pointlib_point_make(POINTLIB_COORD_FIVE, POINTLIB_COORD_SEVEN);
  pointlib_coord hxc_l_tmp_native_call_result_n1 = pointlib_point_component(hxc_l_tmp_native_call_result_n0, POINTLIB_AXIS_X);
  pointlib_coord hxc_l_tmp = hxc_l_tmp_native_call_result_n1;
  pointlib_coord hxc_l_selected = hxc_l_y;
  if (!hxc_l_useY)
  {
    hxc_l_selected = POINTLIB_COORD_ONE;
  }
  pointlib_coord hxc_l_tmp_load_result_n2 = hxc_l_tmp;
  return (struct pointlib_point){ .x = hxc_l_tmp_load_result_n2, .y = hxc_l_selected };
}

const char *hxc_Main_localizedLabel(int32_t hxc_l_locale, int32_t hxc_l_message)
{
  if (!(hxc_l_locale == 0))
  {
    return "other-locale";
  }
  const char *hxc_l_tmp_call_result_n0 = hxc_Main_localizedMessage(hxc_l_message);
  return hxc_l_tmp_call_result_n0;
}

const char *hxc_Main_localizedMessage(int32_t hxc_l_message)
{
  if (!(hxc_l_message == 0))
  {
    return "other-message";
  }
  return "c-import-\303\251";
}

void hxc_Main_main(void)
{
  pointlib_build_fact_probe();
  bool hxc_l_flipped = false;
  bool *hxc_l_tmp_native_call_ref_n0 = &hxc_l_flipped;
  pointlib_flip(hxc_l_tmp_native_call_ref_n0);
  struct pointlib_point hxc_l_tmp_native_call_result_n1 = pointlib_point_make(POINTLIB_COORD_ONE, POINTLIB_COORD_NEGATIVE_THREE);
  struct pointlib_point hxc_l_left = hxc_l_tmp_native_call_result_n1;
  struct pointlib_point hxc_l_tmp_native_call_result_n2 = pointlib_point_make(POINTLIB_COORD_ONE, POINTLIB_COORD_NEGATIVE_THREE);
  struct pointlib_point hxc_l_inPlace = hxc_l_tmp_native_call_result_n2;
  struct pointlib_point *hxc_l_tmp_native_call_ref_n3 = &hxc_l_inPlace;
  pointlib_point_translate_in_place(hxc_l_tmp_native_call_ref_n3, POINTLIB_COORD_ONE, POINTLIB_COORD_FIVE);
  struct pointlib_point hxc_l_tmp_call_result_n4 = hxc_Main_localPoint(POINTLIB_COORD_SEVEN, true);
  struct hxc_PointResources hxc_l_tmp_call_result_n5 = hxc_Main_pointResources(hxc_l_tmp_call_result_n4);
  struct hxc_PointResources hxc_l_resources = hxc_l_tmp_call_result_n5;
  struct pointlib_point hxc_l_tmp_record_field_load_result_n6 = hxc_l_resources.hxc_point;
  struct hxc_PointOwner hxc_l_tmp_object_storage_n5 = { 0 };
  struct hxc_PointOwner *hxc_l_tmp_class_object_address_n7 = &hxc_l_tmp_object_storage_n5;
  hxc_compiler_constructor_PointOwner(hxc_l_tmp_class_object_address_n7, hxc_l_tmp_record_field_load_result_n6);
  struct hxc_PointOwner *hxc_l_owner = hxc_l_tmp_class_object_address_n7;
  struct pointlib_hidden_point hxc_l_tmp_native_call_result_n8 = pointlib_hidden_point_make(POINTLIB_COORD_FIVE, POINTLIB_COORD_SEVEN);
  struct hxc_HiddenPointResources hxc_l_tmp_call_result_n9 = hxc_Main_hiddenPointResources(hxc_l_tmp_native_call_result_n8);
  struct hxc_HiddenPointResources hxc_l_hiddenResources = hxc_l_tmp_call_result_n9;
  struct pointlib_hidden_point hxc_l_tmp_record_field_load_result_n10 = hxc_l_hiddenResources.hxc_point;
  struct hxc_HiddenPointOwner hxc_l_tmp_object_storage_n8 = { 0 };
  struct hxc_HiddenPointOwner *hxc_l_tmp_class_object_address_n11 = &hxc_l_tmp_object_storage_n8;
  hxc_compiler_constructor_HiddenPointOwner(hxc_l_tmp_class_object_address_n11, hxc_l_tmp_record_field_load_result_n10);
  struct hxc_HiddenPointOwner *hxc_l_hiddenOwner = hxc_l_tmp_class_object_address_n11;
  struct hxc_HiddenPointOwner *hxc_l_tmp_load_result_n12 = hxc_l_hiddenOwner;
  if (hxc_l_tmp_load_result_n12 == NULL)
  {
    abort();
  }
  struct pointlib_hidden_point hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_tmp_load_result_n12).hxc_point;
  struct pointlib_hidden_point hxc_l_tmp_native_call_result_n14 = pointlib_hidden_point_identity(hxc_l_tmp_class_field_load_result_n13);
  struct pointlib_hidden_point hxc_l_hiddenPoint = hxc_l_tmp_native_call_result_n14;
  struct pointlib_point hxc_l_tmp_load_result_n15 = hxc_l_left;
  struct hxc_PointOwner *hxc_l_tmp_load_result_n16 = hxc_l_owner;
  if (hxc_l_tmp_load_result_n16 == NULL)
  {
    abort();
  }
  struct pointlib_point hxc_l_tmp_class_field_load_result_n17 = (*hxc_l_tmp_load_result_n16).hxc_point;
  struct pointlib_point hxc_l_tmp_call_result_n18 = hxc_Main_selectPoint(false, hxc_l_tmp_load_result_n15, hxc_l_tmp_class_field_load_result_n17);
  struct pointlib_point hxc_l_right = hxc_l_tmp_call_result_n18;
  struct pointlib_point hxc_l_zeroed = (struct pointlib_point){ 0 };
  hxc_l_left.x = POINTLIB_COORD_ONE;
  pointlib_coord *hxc_l_tmp_imported_field_address_n20 = &hxc_l_left.x;
  pointlib_coord hxc_l_tmp_imported_field_load_result_n21 = *hxc_l_tmp_imported_field_address_n20;
  pointlib_coord hxc_l_delta = hxc_l_tmp_imported_field_load_result_n21;
  struct pointlib_point hxc_l_tmp_load_result_n22 = hxc_l_left;
  struct pointlib_point hxc_l_tmp_native_call_result_n24 = pointlib_point_translate(hxc_l_tmp_load_result_n22, hxc_l_delta, POINTLIB_COORD_FIVE);
  hxc_l_left = hxc_l_tmp_native_call_result_n24;
  struct pointlib_point hxc_l_tmp_native_call_result_n26 = pointlib_point_alias_identity(hxc_l_left);
  struct pointlib_point hxc_l_pointAlias = hxc_l_tmp_native_call_result_n26;
  (void)hxc_l_pointAlias;
  struct pointlib_point hxc_l_tmp_load_result_n27 = hxc_l_left;
  int64_t hxc_l_tmp_native_call_result_n29 = pointlib_point_dot(hxc_l_tmp_load_result_n27, hxc_l_right);
  int64_t hxc_l_dot = hxc_l_tmp_native_call_result_n29;
  pointlib_axis hxc_l_tmp_call_result_n30 = hxc_Main_selectAxis(true);
  pointlib_axis hxc_l_axis = hxc_l_tmp_call_result_n30;
  bool hxc_l_axisIsY = hxc_l_axis == POINTLIB_AXIS_Y;
  bool hxc_l_axisIsNotX = hxc_l_axis != POINTLIB_AXIS_X;
  struct pointlib_point hxc_l_tmp_load_result_n33 = hxc_l_left;
  pointlib_coord hxc_l_tmp_native_call_result_n35 = pointlib_point_component(hxc_l_tmp_load_result_n33, hxc_l_axis);
  pointlib_coord hxc_l_component = hxc_l_tmp_native_call_result_n35;
  struct pointlib_float_point hxc_l_tmp_native_call_result_n36 = pointlib_float_point_make(POINTLIB_FLOAT_ONE_POINT_FIVE, POINTLIB_FLOAT_NEGATIVE_TWO);
  struct pointlib_float_point hxc_l_floatPoint = hxc_l_tmp_native_call_result_n36;
  hxc_l_floatPoint.x = (float)0.5;
  struct pointlib_float_point hxc_l_tmp_native_call_result_n38 = pointlib_float_point_scale(hxc_l_floatPoint, (float)2.0);
  hxc_l_floatPoint = hxc_l_tmp_native_call_result_n38;
  struct pointlib_float_point hxc_l_tmp_load_result_n39 = hxc_l_floatPoint;
  float hxc_l_tmp_native_call_result_n41 = pointlib_float_point_dot(hxc_l_tmp_load_result_n39, hxc_l_floatPoint);
  float hxc_l_floatDot = hxc_l_tmp_native_call_result_n41;
  double hxc_l_widened = (double)hxc_l_floatDot;
  float hxc_l_tie = (float)1.000000059604644775390625;
  float hxc_l_subnormal = (float)1.401298464324817e-45;
  float hxc_l_positiveInfinity = (float)hxc_f64_divide_zero_safe(1.0, 0.0);
  float hxc_l_nan = (float)hxc_f64_divide_zero_safe(0.0, 0.0);
  float hxc_l_negativeZero = (float)-0.0;
  float hxc_l_finiteOverflow = (float)1.0e300;
  int32_t hxc_l_locale = 0;
  while (1)
  {
    bool hxc_l_tmp_load_result_n43 = hxc_l_flipped;
    bool hxc_l_tmp_short_circuit_result_n29 = !hxc_l_tmp_load_result_n43;
    if (!!hxc_l_tmp_load_result_n43)
    {
      hxc_l_tmp_short_circuit_result_n29 = !hxc_l_resources.hxc_ready;
    }
    bool hxc_l_tmp_short_circuit_load_result_n45 = hxc_l_tmp_short_circuit_result_n29;
    bool hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_short_circuit_load_result_n45;
    if (!hxc_l_tmp_short_circuit_load_result_n45)
    {
      hxc_l_tmp_short_circuit_result_n30 = !hxc_l_hiddenResources.hxc_ready;
    }
    bool hxc_l_tmp_short_circuit_load_result_n47 = hxc_l_tmp_short_circuit_result_n30;
    bool hxc_l_tmp_short_circuit_result_n31 = hxc_l_tmp_short_circuit_load_result_n47;
    if (!hxc_l_tmp_short_circuit_load_result_n47)
    {
      bool hxc_l_tmp_native_call_result_n49 = pointlib_hidden_point_verify(hxc_l_hiddenPoint);
      hxc_l_tmp_short_circuit_result_n31 = !hxc_l_tmp_native_call_result_n49;
    }
    bool hxc_l_tmp_short_circuit_load_result_n50 = hxc_l_tmp_short_circuit_result_n31;
    bool hxc_l_tmp_short_circuit_result_n32 = hxc_l_tmp_short_circuit_load_result_n50;
    if (!hxc_l_tmp_short_circuit_load_result_n50)
    {
      bool hxc_l_tmp_native_call_result_n51 = pointlib_variadic_fixed_prefix_verify(POINTLIB_COORD_ONE);
      hxc_l_tmp_short_circuit_result_n32 = !hxc_l_tmp_native_call_result_n51;
    }
    bool hxc_l_tmp_short_circuit_load_result_n52 = hxc_l_tmp_short_circuit_result_n32;
    bool hxc_l_tmp_short_circuit_result_n33 = hxc_l_tmp_short_circuit_load_result_n52;
    if (!hxc_l_tmp_short_circuit_load_result_n52)
    {
      hxc_l_tmp_short_circuit_result_n33 = !hxc_l_axisIsY;
    }
    bool hxc_l_tmp_short_circuit_load_result_n54 = hxc_l_tmp_short_circuit_result_n33;
    bool hxc_l_tmp_short_circuit_result_n34 = hxc_l_tmp_short_circuit_load_result_n54;
    if (!hxc_l_tmp_short_circuit_load_result_n54)
    {
      hxc_l_tmp_short_circuit_result_n34 = !hxc_l_axisIsNotX;
    }
    bool hxc_l_tmp_short_circuit_load_result_n56 = hxc_l_tmp_short_circuit_result_n34;
    bool hxc_l_tmp_short_circuit_result_n35 = hxc_l_tmp_short_circuit_load_result_n56;
    if (!hxc_l_tmp_short_circuit_load_result_n56)
    {
      bool hxc_l_tmp_native_call_result_n58 = pointlib_point_is_zero(hxc_l_zeroed);
      hxc_l_tmp_short_circuit_result_n35 = !hxc_l_tmp_native_call_result_n58;
    }
    bool hxc_l_tmp_short_circuit_load_result_n59 = hxc_l_tmp_short_circuit_result_n35;
    bool hxc_l_tmp_short_circuit_result_n36 = hxc_l_tmp_short_circuit_load_result_n59;
    if (!hxc_l_tmp_short_circuit_load_result_n59)
    {
      struct pointlib_point hxc_l_tmp_load_result_n60 = hxc_l_inPlace;
      struct pointlib_point hxc_l_tmp_load_result_n61 = hxc_l_right;
      int64_t hxc_l_tmp_load_result_n62 = hxc_l_dot;
      pointlib_coord hxc_l_tmp_load_result_n63 = hxc_l_component;
      pointlib_axis hxc_l_tmp_load_result_n64 = hxc_l_axis;
      const char *hxc_l_tmp_call_result_n66 = hxc_Main_localizedLabel(hxc_l_locale, 0);
      bool hxc_l_tmp_call_result_n67 = hxc_Main_verifyPoint(hxc_l_tmp_load_result_n60, hxc_l_tmp_load_result_n61, hxc_l_tmp_load_result_n62, hxc_l_tmp_load_result_n63, hxc_l_tmp_load_result_n64, hxc_l_tmp_call_result_n66);
      hxc_l_tmp_short_circuit_result_n36 = !hxc_l_tmp_call_result_n67;
    }
    bool hxc_l_tmp_short_circuit_load_result_n68 = hxc_l_tmp_short_circuit_result_n36;
    bool hxc_l_tmp_short_circuit_result_n37 = hxc_l_tmp_short_circuit_load_result_n68;
    if (!hxc_l_tmp_short_circuit_load_result_n68)
    {
      struct pointlib_float_point hxc_l_tmp_load_result_n69 = hxc_l_floatPoint;
      float hxc_l_tmp_load_result_n70 = hxc_l_floatDot;
      double hxc_l_tmp_load_result_n71 = hxc_l_widened;
      float hxc_l_tmp_load_result_n72 = hxc_l_tie;
      float hxc_l_tmp_load_result_n73 = hxc_l_subnormal;
      float hxc_l_tmp_load_result_n74 = hxc_l_positiveInfinity;
      float hxc_l_tmp_load_result_n75 = hxc_l_nan;
      float hxc_l_tmp_load_result_n76 = hxc_l_negativeZero;
      bool hxc_l_tmp_native_call_result_n78 = pointlib_float32_verify(hxc_l_tmp_load_result_n69, hxc_l_tmp_load_result_n70, hxc_l_tmp_load_result_n71, hxc_l_tmp_load_result_n72, hxc_l_tmp_load_result_n73, hxc_l_tmp_load_result_n74, hxc_l_tmp_load_result_n75, hxc_l_tmp_load_result_n76, hxc_l_finiteOverflow);
      hxc_l_tmp_short_circuit_result_n37 = !hxc_l_tmp_native_call_result_n78;
    }
    bool hxc_l_tmp_short_circuit_load_result_n79 = hxc_l_tmp_short_circuit_result_n37;
    bool hxc_l_tmp_short_circuit_result_n38 = hxc_l_tmp_short_circuit_load_result_n79;
    if (!hxc_l_tmp_short_circuit_load_result_n79)
    {
      bool hxc_l_tmp_call_result_n80 = hxc_InlineFloat32Probe_run();
      hxc_l_tmp_short_circuit_result_n38 = !hxc_l_tmp_call_result_n80;
    }
    if (!hxc_l_tmp_short_circuit_result_n38)
    {
      break;
    }
  }
  return;
}

struct hxc_PointResources hxc_Main_pointResources(struct pointlib_point hxc_l_point)
{
  return (struct hxc_PointResources){ .hxc_point = hxc_l_point, .hxc_ready = true };
}

pointlib_axis hxc_Main_selectAxis(bool hxc_l_useY)
{
  pointlib_axis hxc_l_tmp_conditional_result_n2 = { 0 };
  if (hxc_l_useY)
  {
    hxc_l_tmp_conditional_result_n2 = POINTLIB_AXIS_Y;
  }
  else
  {
    hxc_l_tmp_conditional_result_n2 = POINTLIB_AXIS_X;
  }
  pointlib_axis hxc_l_selected = hxc_l_tmp_conditional_result_n2;
  pointlib_axis hxc_l_tmp_native_call_result_n2 = pointlib_axis_identity(hxc_l_selected);
  return hxc_l_tmp_native_call_result_n2;
}

struct pointlib_point hxc_Main_selectPoint(bool hxc_l_useLeft, struct pointlib_point hxc_l_left, struct pointlib_point hxc_l_right)
{
  struct pointlib_point hxc_l_tmp_conditional_result_n4 = { 0 };
  if (hxc_l_useLeft)
  {
    hxc_l_tmp_conditional_result_n4 = hxc_l_left;
  }
  else
  {
    hxc_l_tmp_conditional_result_n4 = hxc_l_right;
  }
  struct pointlib_point hxc_l_selected = hxc_l_tmp_conditional_result_n4;
  return hxc_l_selected;
}

bool hxc_Main_verifyPoint(struct pointlib_point hxc_l_left, struct pointlib_point hxc_l_right, int64_t hxc_l_dot, pointlib_coord hxc_l_component, pointlib_axis hxc_l_axis, const char *hxc_l_label)
{
  bool hxc_l_tmp_native_call_result_n0 = pointlib_point_verify(hxc_l_left, hxc_l_right, hxc_l_dot, hxc_l_component, hxc_l_axis, hxc_l_label);
  return hxc_l_tmp_native_call_result_n0;
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

#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "pointlib.h"

_Static_assert(CHAR_BIT == 8, "hxc c.Float32 requires 8-bit C bytes");

_Static_assert(sizeof(float) * CHAR_BIT == 32, "hxc c.Float32 requires 32-bit C float storage");

_Static_assert(FLT_RADIX == 2, "hxc c.Float32 requires radix-2 C floating point");

_Static_assert(FLT_MANT_DIG == 24, "hxc c.Float32 requires a 24-bit binary32 significand");

_Static_assert(FLT_MAX_EXP == 128, "hxc c.Float32 requires the binary32 maximum exponent");

_Static_assert(FLT_MIN_EXP == -125, "hxc c.Float32 requires the binary32 minimum exponent");

_Static_assert(FLT_HAS_SUBNORM == 1, "hxc c.Float32 requires binary32 subnormal support");

static inline int32_t hxc_u32_to_i32_bits(uint32_t hxc_l_value)
{
  if (hxc_l_value <= UINT32_C(2147483647))
  {
    return (int32_t)hxc_l_value;
  }
  return INT32_MIN + (int32_t)(hxc_l_value - UINT32_C(2147483648));
}

static inline double hxc_f64_divide_zero_safe(double hxc_l_left, double hxc_l_right)
{
  if (hxc_l_right != 0.0)
  {
    return hxc_l_left / hxc_l_right;
  }
  if (hxc_l_left == 0.0 || hxc_l_left != hxc_l_left)
  {
    return NAN;
  }
  return signbit(hxc_l_left) != signbit(hxc_l_right) ? -INFINITY : INFINITY;
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left + (uint64_t)(uint32_t)hxc_l_right));
}

struct hxc_PointOwner;

struct hxc_HiddenPointOwner;

struct hxc_HiddenPointResources {
  struct pointlib_hidden_point hxc_point;
  bool hxc_ready;
};

struct hxc_PointResources {
  struct pointlib_point hxc_point;
  bool hxc_ready;
};

struct hxc_HiddenPointOwner {
  struct pointlib_hidden_point hxc_point;
};

struct hxc_PointOwner {
  struct pointlib_point hxc_point;
};

extern int32_t hxc_InlineFloat32Probe_sideEffectCount;

void hxc_compiler_constructor_HiddenPointOwner(struct hxc_HiddenPointOwner *hxc_l_self, struct pointlib_hidden_point hxc_l_point);

void hxc_compiler_constructor_PointOwner(struct hxc_PointOwner *hxc_l_self, struct pointlib_point hxc_l_point);

float hxc_InlineFloat32Probe_narrowWithoutInlining(double hxc_l_value);

int32_t hxc_InlineFloat32Probe_nextInteger(void);

bool hxc_InlineFloat32Probe_run(void);

struct hxc_HiddenPointResources hxc_Main_hiddenPointResources(struct pointlib_hidden_point hxc_l_point);

struct pointlib_point hxc_Main_localPoint(pointlib_coord hxc_l_y, bool hxc_l_useY);

const char *hxc_Main_localizedLabel(int32_t hxc_l_locale, int32_t hxc_l_message);

const char *hxc_Main_localizedMessage(int32_t hxc_l_message);

void hxc_Main_main(void);

struct hxc_PointResources hxc_Main_pointResources(struct pointlib_point hxc_l_point);

pointlib_axis hxc_Main_selectAxis(bool hxc_l_useY);

struct pointlib_point hxc_Main_selectPoint(bool hxc_l_useLeft, struct pointlib_point hxc_l_left, struct pointlib_point hxc_l_right);

bool hxc_Main_verifyPoint(struct pointlib_point hxc_l_left, struct pointlib_point hxc_l_right, int64_t hxc_l_dot, pointlib_coord hxc_l_component, pointlib_axis hxc_l_axis, const char *hxc_l_label);

void hxc_init_compiler_static_initialization_InlineFloat32Probe_static_field_sideEffectCount(void);

#endif /* HXC_PROGRAM_H_INCLUDED */

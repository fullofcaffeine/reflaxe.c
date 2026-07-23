#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "pointlib.h"

_Static_assert(CHAR_BIT == 8, "hxc c.Float32 requires 8-bit C bytes");

_Static_assert(sizeof(float) * CHAR_BIT == 32, "hxc c.Float32 requires 32-bit C float storage");

_Static_assert(FLT_RADIX == 2, "hxc c.Float32 requires radix-2 C floating point");

_Static_assert(FLT_MANT_DIG == 24, "hxc c.Float32 requires a 24-bit binary32 significand");

_Static_assert(FLT_MAX_EXP == 128, "hxc c.Float32 requires the binary32 maximum exponent");

_Static_assert(FLT_MIN_EXP == -125, "hxc c.Float32 requires the binary32 minimum exponent");

_Static_assert(FLT_HAS_SUBNORM == 1, "hxc c.Float32 requires binary32 subnormal support");

static inline int32_t hxc_u32_to_i32_bits(uint32_t hxc_value)
{
  if (hxc_value <= UINT32_C(2147483647))
  {
    return (int32_t)hxc_value;
  }
  return INT32_MIN + (int32_t)(hxc_value - UINT32_C(2147483648));
}

static inline double hxc_f64_divide_zero_safe(double hxc_left, double hxc_right)
{
  if (hxc_right != 0.0)
  {
    return hxc_left / hxc_right;
  }
  if (hxc_left == 0.0 || hxc_left != hxc_left)
  {
    return NAN;
  }
  return signbit(hxc_left) != signbit(hxc_right) ? -INFINITY : INFINITY;
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left + (uint64_t)(uint32_t)hxc_right));
}

struct hxc_PointResources {
  struct pointlib_point hxc_point;
  bool hxc_ready;
};

extern int32_t hxc_InlineFloat32Probe_sideEffectCount;

float hxc_InlineFloat32Probe_narrowWithoutInlining(double hxc_value);

int32_t hxc_InlineFloat32Probe_nextInteger(void);

bool hxc_InlineFloat32Probe_run(void);

struct pointlib_point hxc_Main_localPoint(pointlib_coord hxc_y, bool hxc_useY);

const char *hxc_Main_localizedLabel(int32_t hxc_locale, int32_t hxc_message);

const char *hxc_Main_localizedMessage(int32_t hxc_message);

void hxc_Main_main(void);

struct hxc_PointResources hxc_Main_pointResources(struct pointlib_point hxc_point);

void hxc_init_compiler_static_initialization_InlineFloat32Probe_static_field_sideEffectCount(void);

#endif /* HXC_PROGRAM_H_INCLUDED */

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

struct pointlib_point hxc_Main_localPoint(pointlib_coord hxc_x, pointlib_coord hxc_y);

void hxc_Main_main(void);

#endif /* HXC_PROGRAM_H_INCLUDED */

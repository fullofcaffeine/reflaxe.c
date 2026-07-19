#ifndef HXC_GENERATED_PATH_696E636C7564652F6878632F70726F6772616D2E68_INCLUDED
#define HXC_GENERATED_PATH_696E636C7564652F6878632F70726F6772616D2E68_INCLUDED

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

static inline double hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe(double hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_left_n0, double hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_right_n1)
{
  if (hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_right_n1 != 0.0)
  {
    return hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_left_n0 / hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_right_n1;
  }
  if (hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_left_n0 == 0.0 || hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_left_n0 != hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_left_n0)
  {
    return NAN;
  }
  return signbit(hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_left_n0) != signbit(hxc_local_compiler_primitivezx2Dhelper_hxczx2Ef64zx2Edividezx2Ezzerozx2Dsafe_right_n1) ? -INFINITY : INFINITY;
}

void hxc_method_Main_main(void);

#endif /* HXC_GENERATED_PATH_696E636C7564652F6878632F70726F6772616D2E68_INCLUDED */

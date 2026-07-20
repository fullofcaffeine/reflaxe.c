#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <float.h>
#include <limits.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

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

static inline int32_t hxc_i32_bit_or(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left | (uint64_t)(uint32_t)hxc_right));
}

void hxc_Main_main(void);

#endif /* HXC_PROGRAM_H_INCLUDED */

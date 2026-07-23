#ifndef HXC_DETAIL_PROGRAM_TYPES_H_INCLUDED
#define HXC_DETAIL_PROGRAM_TYPES_H_INCLUDED

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

static inline int32_t hxc_f64_to_i32_saturating(double hxc_value)
{
  if (hxc_value != hxc_value)
  {
    return 0;
  }
  if (hxc_value >= 2147483647.0)
  {
    return INT32_MAX;
  }
  if (hxc_value <= -2147483648.0)
  {
    return INT32_MIN;
  }
  return (int32_t)hxc_value;
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left + (uint64_t)(uint32_t)hxc_right));
}

static inline int32_t hxc_i32_bit_and(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left & (uint64_t)(uint32_t)hxc_right));
}

static inline int32_t hxc_i32_bit_xor(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left ^ (uint64_t)(uint32_t)hxc_right));
}

static inline int32_t hxc_i32_modulo_zero_safe(int32_t hxc_left, int32_t hxc_right)
{
  if (hxc_right == 0 || (hxc_left == INT32_MIN && hxc_right == -1))
  {
    return 0;
  }
  return hxc_left % hxc_right;
}

static inline int32_t hxc_i32_multiply_wrapping(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left * (uint64_t)(uint32_t)hxc_right));
}

static inline int32_t hxc_i32_negate_wrapping(int32_t hxc_value)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)0 - (uint64_t)(uint32_t)hxc_value));
}

static inline int32_t hxc_i32_subtract_wrapping(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left - (uint64_t)(uint32_t)hxc_right));
}

static inline int32_t hxc_i32_unsigned_shift_right_masked(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left >> ((uint32_t)hxc_right & (uint32_t)31)));
}

struct hxc_caxecraft_domain_BlockCoord;

struct hxc_caxecraft_domain_RaycastHit;

struct hxc_caxecraft_domain_StepInput;

struct hxc_caxecraft_domain_AxisMove;

struct hxc_caxecraft_domain_CharacterBody;

#endif /* HXC_DETAIL_PROGRAM_TYPES_H_INCLUDED */

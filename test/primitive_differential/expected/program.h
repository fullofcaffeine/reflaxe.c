#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

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

static inline double hxc_f64_modulo(double hxc_l_left, double hxc_l_right)
{
  if (hxc_l_right == 0.0)
  {
    return NAN;
  }
  return fmod(hxc_l_left, hxc_l_right);
}

static inline int32_t hxc_f64_to_i32_saturating(double hxc_l_value)
{
  if (hxc_l_value != hxc_l_value)
  {
    return 0;
  }
  if (hxc_l_value >= 2147483647.0)
  {
    return INT32_MAX;
  }
  if (hxc_l_value <= -2147483648.0)
  {
    return INT32_MIN;
  }
  return (int32_t)hxc_l_value;
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left + (uint64_t)(uint32_t)hxc_l_right));
}

static inline int32_t hxc_i32_bit_and(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left & (uint64_t)(uint32_t)hxc_l_right));
}

static inline int32_t hxc_i32_bit_not(int32_t hxc_l_value)
{
  return hxc_u32_to_i32_bits((uint32_t)~(uint64_t)(uint32_t)hxc_l_value);
}

static inline int32_t hxc_i32_bit_or(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left | (uint64_t)(uint32_t)hxc_l_right));
}

static inline int32_t hxc_i32_bit_xor(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left ^ (uint64_t)(uint32_t)hxc_l_right));
}

static inline int32_t hxc_i32_modulo_zero_safe(int32_t hxc_l_left, int32_t hxc_l_right)
{
  if (hxc_l_right == 0 || (hxc_l_left == INT32_MIN && hxc_l_right == -1))
  {
    return 0;
  }
  return hxc_l_left % hxc_l_right;
}

static inline int32_t hxc_i32_multiply_wrapping(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left * (uint64_t)(uint32_t)hxc_l_right));
}

static inline int32_t hxc_i32_shift_left_masked(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left << ((uint32_t)hxc_l_right & (uint32_t)31)));
}

static inline int32_t hxc_i32_shift_right_masked(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits(((uint32_t)hxc_l_left & UINT32_C(2147483648)) == 0 ? (uint32_t)((uint64_t)(uint32_t)hxc_l_left >> ((uint32_t)hxc_l_right & (uint32_t)31)) : (uint32_t)~(uint64_t)(uint32_t)((uint64_t)(uint32_t)~(uint64_t)(uint32_t)hxc_l_left >> ((uint32_t)hxc_l_right & (uint32_t)31)));
}

static inline int32_t hxc_i32_subtract_wrapping(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left - (uint64_t)(uint32_t)hxc_l_right));
}

static inline int32_t hxc_i32_unsigned_shift_right_masked(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left >> ((uint32_t)hxc_l_right & (uint32_t)31)));
}

int32_t hxc_PrimitiveDifferentialFixture_faddInt(double hxc_l_left, double hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_fdivInt(double hxc_l_left, double hxc_l_right);

bool hxc_PrimitiveDifferentialFixture_feq(double hxc_l_left, double hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_fint(double hxc_l_value);

bool hxc_PrimitiveDifferentialFixture_flt(double hxc_l_left, double hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_fmodInt(double hxc_l_left, double hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_fmulInt(double hxc_l_left, double hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_fsubInt(double hxc_l_left, double hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_iadd(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_iand(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_idivInt(int32_t hxc_l_left, int32_t hxc_l_right);

bool hxc_PrimitiveDifferentialFixture_ieq(int32_t hxc_l_left, int32_t hxc_l_right);

bool hxc_PrimitiveDifferentialFixture_ilt(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_imod(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_imul(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_inot(int32_t hxc_l_value);

int32_t hxc_PrimitiveDifferentialFixture_ior(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_ishl(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_ishr(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_isub(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_iushr(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_PrimitiveDifferentialFixture_ixor(int32_t hxc_l_left, int32_t hxc_l_right);

void hxc_PrimitiveDifferentialFixture_main(void);

uint32_t hxc_PrimitiveDifferentialFixture_uadd(uint32_t hxc_l_left, uint32_t hxc_l_right);

bool hxc_PrimitiveDifferentialFixture_ult(uint32_t hxc_l_left, uint32_t hxc_l_right);

uint32_t hxc_PrimitiveDifferentialFixture_umod(uint32_t hxc_l_left, uint32_t hxc_l_right);

uint32_t hxc_PrimitiveDifferentialFixture_umul(uint32_t hxc_l_left, uint32_t hxc_l_right);

uint32_t hxc_PrimitiveDifferentialFixture_ushl(uint32_t hxc_l_left, int32_t hxc_l_right);

uint32_t hxc_PrimitiveDifferentialFixture_ushr(uint32_t hxc_l_left, int32_t hxc_l_right);

uint32_t hxc_PrimitiveDifferentialFixture_usub(uint32_t hxc_l_left, uint32_t hxc_l_right);

#endif /* HXC_PROGRAM_H_INCLUDED */

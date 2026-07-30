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

static inline int32_t hxc_i32_negate_wrapping(int32_t hxc_l_value)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)0 - (uint64_t)(uint32_t)hxc_l_value));
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

static inline uint32_t hxc_u32_modulo_zero_safe(uint32_t hxc_l_left, uint32_t hxc_l_right)
{
  if (hxc_l_right == 0)
  {
    return 0;
  }
  return hxc_l_left % hxc_l_right;
}

double hxc_ArithmeticFixture_fadd(double hxc_l_left, double hxc_l_right);

double hxc_ArithmeticFixture_fdiv(double hxc_l_left, double hxc_l_right);

bool hxc_ArithmeticFixture_fequal(double hxc_l_left, double hxc_l_right);

int32_t hxc_ArithmeticFixture_fint(double hxc_l_value);

double hxc_ArithmeticFixture_fmod(double hxc_l_left, double hxc_l_right);

double hxc_ArithmeticFixture_fmul(double hxc_l_left, double hxc_l_right);

double hxc_ArithmeticFixture_fneg(double hxc_l_value);

double hxc_ArithmeticFixture_fsub(double hxc_l_left, double hxc_l_right);

uint8_t hxc_ArithmeticFixture_i32ToU8(int32_t hxc_l_value);

uint16_t hxc_ArithmeticFixture_i64ToU16(int64_t hxc_l_value);

int32_t hxc_ArithmeticFixture_i8ToI32(int8_t hxc_l_value);

int32_t hxc_ArithmeticFixture_iadd(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_iand(int32_t hxc_l_left, int32_t hxc_l_right);

double hxc_ArithmeticFixture_idiv(int32_t hxc_l_left, int32_t hxc_l_right);

bool hxc_ArithmeticFixture_iless(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_imod(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_imul(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_ineg(int32_t hxc_l_value);

int32_t hxc_ArithmeticFixture_inot(int32_t hxc_l_value);

int32_t hxc_ArithmeticFixture_ior(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_ishl(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_ishr(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_isub(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_iushr(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_ixor(int32_t hxc_l_left, int32_t hxc_l_right);

uint8_t hxc_ArithmeticFixture_literalToU8(void);

void hxc_ArithmeticFixture_main(void);

uint64_t hxc_ArithmeticFixture_u32ToU64(uint32_t hxc_l_value);

uint8_t hxc_ArithmeticFixture_u32ToU8(uint32_t hxc_l_value);

int16_t hxc_ArithmeticFixture_u8ToI16(uint8_t hxc_l_value);

int32_t hxc_ArithmeticFixture_u8ToI32(uint8_t hxc_l_value);

uint32_t hxc_ArithmeticFixture_uadd(uint32_t hxc_l_left, uint32_t hxc_l_right);

uint32_t hxc_ArithmeticFixture_umod(uint32_t hxc_l_left, uint32_t hxc_l_right);

int32_t hxc_ArithmeticFixture_update(int32_t hxc_l_value);

int32_t hxc_ArithmeticFixture_updateParameter(int32_t hxc_l_value);

uint32_t hxc_ArithmeticFixture_ushl(uint32_t hxc_l_left, int32_t hxc_l_right);

uint32_t hxc_ArithmeticFixture_ushr(uint32_t hxc_l_left, int32_t hxc_l_right);

#endif /* HXC_PROGRAM_H_INCLUDED */

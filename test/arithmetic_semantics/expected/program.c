#include "hxc/program.h"

double hxc_ArithmeticFixture_fadd(double hxc_l_left, double hxc_l_right)
{
  return hxc_l_left + hxc_l_right;
}

double hxc_ArithmeticFixture_fdiv(double hxc_l_left, double hxc_l_right)
{
  return hxc_f64_divide_zero_safe(hxc_l_left, hxc_l_right);
}

bool hxc_ArithmeticFixture_fequal(double hxc_l_left, double hxc_l_right)
{
  return hxc_l_left == hxc_l_right;
}

int32_t hxc_ArithmeticFixture_fint(double hxc_l_value)
{
  return hxc_f64_to_i32_saturating(hxc_l_value);
}

double hxc_ArithmeticFixture_fmod(double hxc_l_left, double hxc_l_right)
{
  return hxc_f64_modulo(hxc_l_left, hxc_l_right);
}

double hxc_ArithmeticFixture_fmul(double hxc_l_left, double hxc_l_right)
{
  return hxc_l_left * hxc_l_right;
}

double hxc_ArithmeticFixture_fneg(double hxc_l_value)
{
  return -hxc_l_value;
}

double hxc_ArithmeticFixture_fsub(double hxc_l_left, double hxc_l_right)
{
  return hxc_l_left - hxc_l_right;
}

uint8_t hxc_ArithmeticFixture_i32ToU8(int32_t hxc_l_value)
{
  return (uint8_t)hxc_l_value;
}

uint16_t hxc_ArithmeticFixture_i64ToU16(int64_t hxc_l_value)
{
  return (uint16_t)hxc_l_value;
}

int32_t hxc_ArithmeticFixture_i8ToI32(int8_t hxc_l_value)
{
  return (int32_t)hxc_l_value;
}

int32_t hxc_ArithmeticFixture_iadd(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_add_wrapping(hxc_l_left, hxc_l_right);
}

int32_t hxc_ArithmeticFixture_iand(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_bit_and(hxc_l_left, hxc_l_right);
}

double hxc_ArithmeticFixture_idiv(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_f64_divide_zero_safe((double)hxc_l_left, (double)hxc_l_right);
}

bool hxc_ArithmeticFixture_iless(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_l_left < hxc_l_right;
}

int32_t hxc_ArithmeticFixture_imod(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_modulo_zero_safe(hxc_l_left, hxc_l_right);
}

int32_t hxc_ArithmeticFixture_imul(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_multiply_wrapping(hxc_l_left, hxc_l_right);
}

int32_t hxc_ArithmeticFixture_ineg(int32_t hxc_l_value)
{
  return hxc_i32_negate_wrapping(hxc_l_value);
}

int32_t hxc_ArithmeticFixture_inot(int32_t hxc_l_value)
{
  return hxc_i32_bit_not(hxc_l_value);
}

int32_t hxc_ArithmeticFixture_ior(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_bit_or(hxc_l_left, hxc_l_right);
}

int32_t hxc_ArithmeticFixture_ishl(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_shift_left_masked(hxc_l_left, hxc_l_right);
}

int32_t hxc_ArithmeticFixture_ishr(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_shift_right_masked(hxc_l_left, hxc_l_right);
}

int32_t hxc_ArithmeticFixture_isub(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_subtract_wrapping(hxc_l_left, hxc_l_right);
}

int32_t hxc_ArithmeticFixture_iushr(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_unsigned_shift_right_masked(hxc_l_left, hxc_l_right);
}

int32_t hxc_ArithmeticFixture_ixor(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_i32_bit_xor(hxc_l_left, hxc_l_right);
}

uint8_t hxc_ArithmeticFixture_literalToU8(void)
{
  return (uint8_t)300;
}

void hxc_ArithmeticFixture_main(void)
{
  hxc_ArithmeticFixture_iadd(1, 2);
  hxc_ArithmeticFixture_isub(1, 2);
  hxc_ArithmeticFixture_imul(1, 2);
  hxc_ArithmeticFixture_ineg(1);
  hxc_ArithmeticFixture_idiv(1, 2);
  hxc_ArithmeticFixture_imod(1, 2);
  hxc_ArithmeticFixture_ishl(1, -1);
  hxc_ArithmeticFixture_ishr(-1, -1);
  hxc_ArithmeticFixture_iushr(-1, -1);
  hxc_ArithmeticFixture_iand(1, 2);
  hxc_ArithmeticFixture_ior(1, 2);
  hxc_ArithmeticFixture_ixor(1, 2);
  hxc_ArithmeticFixture_inot(1);
  hxc_ArithmeticFixture_iless(1, 2);
  hxc_ArithmeticFixture_fadd(1.0, 2.0);
  hxc_ArithmeticFixture_fsub(1.0, 2.0);
  hxc_ArithmeticFixture_fmul(1.0, 2.0);
  hxc_ArithmeticFixture_fneg(1.0);
  hxc_ArithmeticFixture_fdiv(1.0, 0.0);
  hxc_ArithmeticFixture_fmod(1.0, 0.0);
  hxc_ArithmeticFixture_fint(3.75);
  hxc_ArithmeticFixture_fequal(1.0, 2.0);
  hxc_ArithmeticFixture_uadd(1, 2);
  hxc_ArithmeticFixture_umod(1, 2);
  hxc_ArithmeticFixture_ushl(1, -1);
  hxc_ArithmeticFixture_ushr(1, -1);
  hxc_ArithmeticFixture_literalToU8();
  hxc_ArithmeticFixture_i32ToU8(-1);
  hxc_ArithmeticFixture_u8ToI32((uint8_t)255);
  hxc_ArithmeticFixture_i64ToU16((int64_t)-1);
  hxc_ArithmeticFixture_u32ToU64((uint32_t)-1);
  hxc_ArithmeticFixture_u32ToU8((uint32_t)-1);
  hxc_ArithmeticFixture_u8ToI16((uint8_t)255);
  hxc_ArithmeticFixture_update(3);
  hxc_ArithmeticFixture_updateParameter(3);
  return;
}

uint64_t hxc_ArithmeticFixture_u32ToU64(uint32_t hxc_l_value)
{
  return (uint64_t)hxc_l_value;
}

uint8_t hxc_ArithmeticFixture_u32ToU8(uint32_t hxc_l_value)
{
  return (uint8_t)hxc_l_value;
}

int16_t hxc_ArithmeticFixture_u8ToI16(uint8_t hxc_l_value)
{
  return (int16_t)hxc_l_value;
}

int32_t hxc_ArithmeticFixture_u8ToI32(uint8_t hxc_l_value)
{
  return (int32_t)hxc_l_value;
}

uint32_t hxc_ArithmeticFixture_uadd(uint32_t hxc_l_left, uint32_t hxc_l_right)
{
  return (uint32_t)((uint64_t)(uint32_t)hxc_l_left + (uint64_t)(uint32_t)hxc_l_right);
}

uint32_t hxc_ArithmeticFixture_umod(uint32_t hxc_l_left, uint32_t hxc_l_right)
{
  return hxc_u32_modulo_zero_safe(hxc_l_left, hxc_l_right);
}

int32_t hxc_ArithmeticFixture_update(int32_t hxc_l_value)
{
  int32_t hxc_l_current = hxc_l_value;
  int32_t hxc_l_tmp_increment_load_result_n0 = hxc_l_current;
  hxc_l_current = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n0, 1);
  int32_t hxc_l_old = hxc_l_tmp_increment_load_result_n0;
  int32_t hxc_l_tmp_decrement_load_result_n1 = hxc_l_current;
  hxc_l_current = hxc_i32_subtract_wrapping(hxc_l_tmp_decrement_load_result_n1, 1);
  int32_t hxc_l_fresh = hxc_i32_subtract_wrapping(hxc_l_tmp_decrement_load_result_n1, 1);
  int32_t hxc_l_tmp_compound_load_result_n2 = hxc_l_current;
  hxc_l_current = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n2, hxc_l_old);
  int32_t hxc_l_tmp_compound_load_result_n4 = hxc_l_current;
  hxc_l_current = hxc_i32_multiply_wrapping(hxc_l_tmp_compound_load_result_n4, hxc_l_fresh);
  return hxc_l_current;
}

int32_t hxc_ArithmeticFixture_updateParameter(int32_t hxc_l_value)
{
  int32_t hxc_l_value_mutable = hxc_l_value;
  hxc_l_value_mutable = hxc_i32_add_wrapping(hxc_l_value_mutable, 3);
  return hxc_l_value_mutable;
}

uint32_t hxc_ArithmeticFixture_ushl(uint32_t hxc_l_left, int32_t hxc_l_right)
{
  return (uint32_t)((uint64_t)(uint32_t)hxc_l_left << ((uint32_t)hxc_l_right & (uint32_t)31));
}

uint32_t hxc_ArithmeticFixture_ushr(uint32_t hxc_l_left, int32_t hxc_l_right)
{
  return (uint32_t)((uint64_t)(uint32_t)hxc_l_left >> ((uint32_t)hxc_l_right & (uint32_t)31));
}

int main(void)
{
  hxc_ArithmeticFixture_main();
  return 0;
}

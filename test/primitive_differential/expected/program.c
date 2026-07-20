#include "hxc/program.h"

int32_t hxc_PrimitiveDifferentialFixture_faddInt(double hxc_left, double hxc_right)
{
  return hxc_f64_to_i32_saturating(hxc_left + hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_fdivInt(double hxc_left, double hxc_right)
{
  return hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe(hxc_left, hxc_right));
}

bool hxc_PrimitiveDifferentialFixture_feq(double hxc_left, double hxc_right)
{
  return hxc_left == hxc_right;
}

int32_t hxc_PrimitiveDifferentialFixture_fint(double hxc_value)
{
  return hxc_f64_to_i32_saturating(hxc_value);
}

bool hxc_PrimitiveDifferentialFixture_flt(double hxc_left, double hxc_right)
{
  return hxc_left < hxc_right;
}

int32_t hxc_PrimitiveDifferentialFixture_fmodInt(double hxc_left, double hxc_right)
{
  return hxc_f64_to_i32_saturating(hxc_f64_modulo(hxc_left, hxc_right));
}

int32_t hxc_PrimitiveDifferentialFixture_fmulInt(double hxc_left, double hxc_right)
{
  return hxc_f64_to_i32_saturating(hxc_left * hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_fsubInt(double hxc_left, double hxc_right)
{
  return hxc_f64_to_i32_saturating(hxc_left - hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_iadd(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_add_wrapping(hxc_left, hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_iand(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_bit_and(hxc_left, hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_idivInt(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_left, (double)hxc_right));
}

bool hxc_PrimitiveDifferentialFixture_ieq(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_left == hxc_right;
}

bool hxc_PrimitiveDifferentialFixture_ilt(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_left < hxc_right;
}

int32_t hxc_PrimitiveDifferentialFixture_imod(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_modulo_zero_safe(hxc_left, hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_imul(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_multiply_wrapping(hxc_left, hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_inot(int32_t hxc_value)
{
  return hxc_i32_bit_not(hxc_value);
}

int32_t hxc_PrimitiveDifferentialFixture_ior(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_bit_or(hxc_left, hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_ishl(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_shift_left_masked(hxc_left, hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_ishr(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_shift_right_masked(hxc_left, hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_isub(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_subtract_wrapping(hxc_left, hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_iushr(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_unsigned_shift_right_masked(hxc_left, hxc_right);
}

int32_t hxc_PrimitiveDifferentialFixture_ixor(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_i32_bit_xor(hxc_left, hxc_right);
}

void hxc_PrimitiveDifferentialFixture_main(void)
{
  hxc_PrimitiveDifferentialFixture_usub((uint32_t)2, (uint32_t)65536);
  hxc_PrimitiveDifferentialFixture_usub((uint32_t)1242540958, (uint32_t)-1487789886);
  hxc_PrimitiveDifferentialFixture_iushr(-1529960360, -595944127);
  hxc_PrimitiveDifferentialFixture_ushr((uint32_t)-1, -1);
  hxc_PrimitiveDifferentialFixture_ushl((uint32_t)2, -1942861741);
  hxc_PrimitiveDifferentialFixture_umul((uint32_t)31, (uint32_t)65536);
  hxc_PrimitiveDifferentialFixture_ishl(663349440, -65536);
  hxc_PrimitiveDifferentialFixture_flt(-1.0, 2411.0);
  hxc_PrimitiveDifferentialFixture_ishl(1, 2147483646);
  hxc_PrimitiveDifferentialFixture_inot(-65536);
  hxc_PrimitiveDifferentialFixture_umul((uint32_t)-2147483648, (uint32_t)-2);
  hxc_PrimitiveDifferentialFixture_ilt(2140014822, 65535);
  hxc_PrimitiveDifferentialFixture_ishr(2147483646, -17340960);
  hxc_PrimitiveDifferentialFixture_feq(-8192.0, 1.0);
  hxc_PrimitiveDifferentialFixture_fsubInt(-1563.25, -0.0);
  hxc_PrimitiveDifferentialFixture_flt(-8192.0, -5133.75);
  hxc_PrimitiveDifferentialFixture_imul(-65536, 1659658283);
  hxc_PrimitiveDifferentialFixture_isub(-192194838, 0);
  hxc_PrimitiveDifferentialFixture_iushr(2147483647, 864338147);
  hxc_PrimitiveDifferentialFixture_isub(65535, 1889045594);
  hxc_PrimitiveDifferentialFixture_idivInt(-65536, 65535);
  hxc_PrimitiveDifferentialFixture_umod((uint32_t)2, (uint32_t)31);
  hxc_PrimitiveDifferentialFixture_umod((uint32_t)2147483647, (uint32_t)-1965165540);
  hxc_PrimitiveDifferentialFixture_isub(-2146252034, 555746818);
  hxc_PrimitiveDifferentialFixture_fmodInt(-5015.25, 1.0);
  hxc_PrimitiveDifferentialFixture_idivInt(2, -2147483648);
  hxc_PrimitiveDifferentialFixture_ishr(31, 2);
  hxc_PrimitiveDifferentialFixture_flt(-0.0, 6775.0);
  hxc_PrimitiveDifferentialFixture_flt(0.0, 4015.0);
  hxc_PrimitiveDifferentialFixture_fmulInt(1.0, -130.5);
  hxc_PrimitiveDifferentialFixture_iand(-1, -65536);
  hxc_PrimitiveDifferentialFixture_fmulInt(-1.0, 1.0);
  hxc_PrimitiveDifferentialFixture_ushr((uint32_t)-1885620801, -31);
  hxc_PrimitiveDifferentialFixture_ishl(-394090625, -478136379);
  hxc_PrimitiveDifferentialFixture_ilt(32, -1590363146);
  hxc_PrimitiveDifferentialFixture_ushl((uint32_t)-2, -33295407);
  hxc_PrimitiveDifferentialFixture_imul(-1371727805, 42081295);
  hxc_PrimitiveDifferentialFixture_inot(33);
  hxc_PrimitiveDifferentialFixture_ult((uint32_t)-1296410921, (uint32_t)2147483647);
  hxc_PrimitiveDifferentialFixture_iadd(32, 65535);
  hxc_PrimitiveDifferentialFixture_ilt(31, 0);
  hxc_PrimitiveDifferentialFixture_ushr((uint32_t)65536, -2);
  hxc_PrimitiveDifferentialFixture_ior(-1546044362, 1723067671);
  hxc_PrimitiveDifferentialFixture_imod(-1772570994, -32);
  hxc_PrimitiveDifferentialFixture_fmodInt(-6818.0, -8052.75);
  hxc_PrimitiveDifferentialFixture_inot(-456293137);
  hxc_PrimitiveDifferentialFixture_fdivInt(1024.5, -2246.25);
  hxc_PrimitiveDifferentialFixture_ixor(-1687817877, -865181082);
  hxc_PrimitiveDifferentialFixture_ult((uint32_t)625950247, (uint32_t)1730156747);
  hxc_PrimitiveDifferentialFixture_flt(1024.5, 1024.5);
  hxc_PrimitiveDifferentialFixture_faddInt(1.0, -4873.0);
  hxc_PrimitiveDifferentialFixture_idivInt(65535, 33);
  hxc_PrimitiveDifferentialFixture_ixor(660778134, 138556725);
  hxc_PrimitiveDifferentialFixture_flt(8191.75, 3211.5);
  hxc_PrimitiveDifferentialFixture_feq(5275.5, 1024.5);
  hxc_PrimitiveDifferentialFixture_ushl((uint32_t)-23526078, 0);
  hxc_PrimitiveDifferentialFixture_ior(574871839, -2);
  hxc_PrimitiveDifferentialFixture_ixor(65535, 2147483646);
  hxc_PrimitiveDifferentialFixture_uadd((uint32_t)65535, (uint32_t)32);
  hxc_PrimitiveDifferentialFixture_faddInt(0.25, 8191.75);
  hxc_PrimitiveDifferentialFixture_isub(1625540872, -62965307);
  hxc_PrimitiveDifferentialFixture_ilt(-2, 1547661062);
  hxc_PrimitiveDifferentialFixture_imul(-2147483648, -1816108123);
  hxc_PrimitiveDifferentialFixture_iadd(2147483646, 1);
  hxc_PrimitiveDifferentialFixture_idivInt(-33, -2147483648);
  hxc_PrimitiveDifferentialFixture_ieq(-2147483647, -2147483648);
  hxc_PrimitiveDifferentialFixture_fdivInt(-0.0, -8192.0);
  hxc_PrimitiveDifferentialFixture_ult((uint32_t)65536, (uint32_t)31);
  hxc_PrimitiveDifferentialFixture_inot(-1243861418);
  hxc_PrimitiveDifferentialFixture_ieq(65535, -2147483648);
  hxc_PrimitiveDifferentialFixture_ior(0, -65536);
  hxc_PrimitiveDifferentialFixture_umul((uint32_t)1, (uint32_t)33);
  hxc_PrimitiveDifferentialFixture_ushr((uint32_t)32, 0);
  hxc_PrimitiveDifferentialFixture_iushr(2, -65536);
  hxc_PrimitiveDifferentialFixture_ixor(487681730, 2147483647);
  hxc_PrimitiveDifferentialFixture_faddInt(1024.5, -1024.25);
  hxc_PrimitiveDifferentialFixture_iand(65535, 374364252);
  hxc_PrimitiveDifferentialFixture_usub((uint32_t)0, (uint32_t)-2);
  hxc_PrimitiveDifferentialFixture_umul((uint32_t)-1, (uint32_t)2147483647);
  hxc_PrimitiveDifferentialFixture_idivInt(-685406939, -2147483647);
  hxc_PrimitiveDifferentialFixture_ishr(-31, -2);
  hxc_PrimitiveDifferentialFixture_ixor(-1574636976, -31);
  hxc_PrimitiveDifferentialFixture_fdivInt(1024.5, 1.0);
  hxc_PrimitiveDifferentialFixture_flt(8191.75, -1.0);
  hxc_PrimitiveDifferentialFixture_ieq(1, 932078650);
  hxc_PrimitiveDifferentialFixture_isub(1772983708, 65535);
  hxc_PrimitiveDifferentialFixture_fdivInt(0.0, 8191.75);
  hxc_PrimitiveDifferentialFixture_feq(1024.5, 0.0);
  hxc_PrimitiveDifferentialFixture_feq(-8192.0, -2758.0);
  hxc_PrimitiveDifferentialFixture_umod((uint32_t)65535, (uint32_t)237502992);
  hxc_PrimitiveDifferentialFixture_fdivInt(0.0, -8192.0);
  hxc_PrimitiveDifferentialFixture_iand(0, -2);
  hxc_PrimitiveDifferentialFixture_imul(-31, 1421103656);
  hxc_PrimitiveDifferentialFixture_idivInt(-2, 1035513225);
  hxc_PrimitiveDifferentialFixture_usub((uint32_t)-80221250, (uint32_t)-1);
  hxc_PrimitiveDifferentialFixture_ilt(-65536, 2);
  hxc_PrimitiveDifferentialFixture_iadd(-65536, 1636456628);
  hxc_PrimitiveDifferentialFixture_fmodInt(-1.0, 1024.5);
  hxc_PrimitiveDifferentialFixture_ior(-33, -65536);
  hxc_PrimitiveDifferentialFixture_fsubInt(-0.0, 1024.5);
  hxc_PrimitiveDifferentialFixture_usub((uint32_t)31, (uint32_t)32);
  hxc_PrimitiveDifferentialFixture_iushr(65535, 31);
  hxc_PrimitiveDifferentialFixture_fmulInt(0.25, -6485.5);
  hxc_PrimitiveDifferentialFixture_fmodInt(-1887.0, -1024.25);
  hxc_PrimitiveDifferentialFixture_imod(-65536, -32);
  hxc_PrimitiveDifferentialFixture_fsubInt(1024.5, -8192.0);
  hxc_PrimitiveDifferentialFixture_iand(-353266926, -31);
  hxc_PrimitiveDifferentialFixture_fdivInt(-1.0, 757.25);
  hxc_PrimitiveDifferentialFixture_uadd((uint32_t)65536, (uint32_t)-345080564);
  hxc_PrimitiveDifferentialFixture_imod(1864848646, -2147483648);
  hxc_PrimitiveDifferentialFixture_isub(32, 2147483646);
  hxc_PrimitiveDifferentialFixture_imod(-33, -290659865);
  hxc_PrimitiveDifferentialFixture_faddInt(1024.5, 1.0);
  hxc_PrimitiveDifferentialFixture_ior(389429777, -2147483648);
  hxc_PrimitiveDifferentialFixture_fsubInt(-0.0, -6050.0);
  hxc_PrimitiveDifferentialFixture_umul((uint32_t)0, (uint32_t)2147483647);
  hxc_PrimitiveDifferentialFixture_imul(32, 1375878956);
  hxc_PrimitiveDifferentialFixture_iand(32, -2147483647);
  hxc_PrimitiveDifferentialFixture_uadd((uint32_t)-377354062, (uint32_t)0);
  hxc_PrimitiveDifferentialFixture_ishl(33, -607981955);
  hxc_PrimitiveDifferentialFixture_ishr(-126297699, -31);
  hxc_PrimitiveDifferentialFixture_iand(-33, 0);
  hxc_PrimitiveDifferentialFixture_ixor(-31, -2147483647);
  hxc_PrimitiveDifferentialFixture_ult((uint32_t)-2, (uint32_t)-28441671);
  hxc_PrimitiveDifferentialFixture_ushr((uint32_t)0, 0);
  hxc_PrimitiveDifferentialFixture_ishl(-1221893229, -65536);
  hxc_PrimitiveDifferentialFixture_idivInt(-1159360382, 2147483647);
  hxc_PrimitiveDifferentialFixture_ushr((uint32_t)-1, -32);
  hxc_PrimitiveDifferentialFixture_imod(7, 0);
  hxc_PrimitiveDifferentialFixture_fint(0.0);
  hxc_PrimitiveDifferentialFixture_fint(2147483648.0);
  return;
}

uint32_t hxc_PrimitiveDifferentialFixture_uadd(uint32_t hxc_left, uint32_t hxc_right)
{
  return (uint32_t)((uint64_t)(uint32_t)hxc_left + (uint64_t)(uint32_t)hxc_right);
}

bool hxc_PrimitiveDifferentialFixture_ult(uint32_t hxc_left, uint32_t hxc_right)
{
  bool hxc_aNeg = hxc_u32_to_i32_bits(hxc_right) < 0;
  bool hxc_bNeg = hxc_u32_to_i32_bits(hxc_left) < 0;
  bool hxc_tmp_load_result_n0 = hxc_aNeg;
  if (!(hxc_tmp_load_result_n0 != hxc_bNeg))
  {
    return hxc_u32_to_i32_bits(hxc_right) > hxc_u32_to_i32_bits(hxc_left);
  }
  return hxc_aNeg;
}

uint32_t hxc_PrimitiveDifferentialFixture_umod(uint32_t hxc_left, uint32_t hxc_right)
{
  int32_t hxc_int = hxc_u32_to_i32_bits(hxc_left);
  int32_t hxc_int1 = hxc_u32_to_i32_bits(hxc_right);
  int32_t hxc_tmp_load_result_n0 = hxc_int;
  double hxc_tmp_conditional_result_n4 = 0.0;
  if (hxc_tmp_load_result_n0 < 0)
  {
    hxc_tmp_conditional_result_n4 = 4294967296.0 + (double)hxc_int;
  }
  else
  {
    hxc_tmp_conditional_result_n4 = (double)hxc_int + 0.0;
  }
  double hxc_tmp_binary_left_n5 = hxc_tmp_conditional_result_n4;
  int32_t hxc_tmp_load_result_n4 = hxc_int1;
  double hxc_tmp_conditional_result_n6 = 0.0;
  if (hxc_tmp_load_result_n4 < 0)
  {
    hxc_tmp_conditional_result_n6 = 4294967296.0 + (double)hxc_int1;
  }
  else
  {
    hxc_tmp_conditional_result_n6 = (double)hxc_int1 + 0.0;
  }
  double hxc_tmp_conditional_load_result_n7 = hxc_tmp_conditional_result_n6;
  return (uint32_t)hxc_f64_to_i32_saturating(hxc_f64_modulo(hxc_tmp_binary_left_n5, hxc_tmp_conditional_load_result_n7));
}

uint32_t hxc_PrimitiveDifferentialFixture_umul(uint32_t hxc_left, uint32_t hxc_right)
{
  return (uint32_t)((uint64_t)(uint32_t)hxc_left * (uint64_t)(uint32_t)hxc_right);
}

uint32_t hxc_PrimitiveDifferentialFixture_ushl(uint32_t hxc_left, int32_t hxc_right)
{
  return (uint32_t)((uint64_t)(uint32_t)hxc_left << ((uint32_t)hxc_right & (uint32_t)31));
}

uint32_t hxc_PrimitiveDifferentialFixture_ushr(uint32_t hxc_left, int32_t hxc_right)
{
  return (uint32_t)((uint64_t)(uint32_t)hxc_left >> ((uint32_t)hxc_right & (uint32_t)31));
}

uint32_t hxc_PrimitiveDifferentialFixture_usub(uint32_t hxc_left, uint32_t hxc_right)
{
  return (uint32_t)((uint64_t)(uint32_t)hxc_left - (uint64_t)(uint32_t)hxc_right);
}

int main(void)
{
  hxc_PrimitiveDifferentialFixture_main();
  return 0;
}

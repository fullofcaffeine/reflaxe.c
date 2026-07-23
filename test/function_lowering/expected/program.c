#include "hxc/program.h"

int32_t hxc_FunctionFixture_apply(int32_t hxc_value, int32_t (*hxc_operation)(int32_t))
{
  int32_t hxc_tmp_indirect_call_result_n0 = hxc_operation(hxc_value);
  return hxc_tmp_indirect_call_result_n0;
}

double hxc_FunctionFixture_asFloat(double hxc_value)
{
  return hxc_value;
}

int32_t hxc_FunctionFixture_chain(int32_t hxc_value)
{
  int32_t hxc_tmp_call_result_n0 = hxc_FunctionFixture_passthrough(hxc_value);
  return hxc_tmp_call_result_n0;
}

int32_t (*hxc_FunctionFixture_choose(void))(int32_t)
{
  return hxc_FunctionFixture_chain;
}

double hxc_FunctionFixture_convert(int32_t hxc_value)
{
  double hxc_tmp_call_result_n0 = hxc_FunctionFixture_asFloat((double)hxc_value);
  return hxc_tmp_call_result_n0;
}

void hxc_FunctionFixture_discarded(int32_t hxc_value)
{
  hxc_FunctionFixture_passthrough(hxc_value);
  return;
}

int32_t hxc_FunctionFixture_first(int32_t hxc_left, int32_t hxc_right)
{
  (void)hxc_right;
  return hxc_left;
}

int32_t hxc_FunctionFixture_indirect(int32_t hxc_value)
{
  int32_t (*hxc_tmp_call_result_n0)(int32_t) = hxc_FunctionFixture_choose();
  int32_t (*hxc_operation)(int32_t) = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_indirect_call_result_n2 = hxc_operation(hxc_value);
  int32_t hxc_tmp_call_result_n3 = hxc_FunctionFixture_apply(hxc_tmp_indirect_call_result_n2, hxc_FunctionFixture_passthrough);
  return hxc_tmp_call_result_n3;
}

void hxc_FunctionFixture_main(void)
{
  int32_t hxc_tmp_call_result_n0 = hxc_FunctionFixture_chain(7);
  int32_t hxc_integer = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_load_result_n1 = hxc_integer;
  (void)hxc_tmp_load_result_n1;
  double hxc_tmp_call_result_n2 = hxc_FunctionFixture_convert(9);
  double hxc_floating = hxc_tmp_call_result_n2;
  double hxc_tmp_load_result_n3 = hxc_floating;
  (void)hxc_tmp_load_result_n3;
  hxc_FunctionFixture_discarded(11);
  int32_t hxc_tmp_call_result_n4 = hxc_FunctionFixture_ordered(13);
  int32_t hxc_orderedValue = hxc_tmp_call_result_n4;
  int32_t hxc_tmp_load_result_n5 = hxc_orderedValue;
  (void)hxc_tmp_load_result_n5;
  int32_t hxc_tmp_call_result_n6 = hxc_FunctionFixture_indirect(17);
  int32_t hxc_indirectValue = hxc_tmp_call_result_n6;
  while (1)
  {
    if (!(hxc_indirectValue != 17))
    {
      break;
    }
  }
  return;
}

int32_t hxc_FunctionFixture_ordered(int32_t hxc_value)
{
  int32_t hxc_tmp_call_result_n0 = hxc_FunctionFixture_passthrough(hxc_value);
  int32_t hxc_tmp_call_result_n1 = hxc_FunctionFixture_chain(hxc_value);
  int32_t hxc_tmp_call_result_n2 = hxc_FunctionFixture_first(hxc_tmp_call_result_n0, hxc_tmp_call_result_n1);
  return hxc_tmp_call_result_n2;
}

int32_t hxc_FunctionFixture_passthrough(int32_t hxc_value)
{
  return hxc_value;
}

int main(void)
{
  hxc_FunctionFixture_main();
  return 0;
}

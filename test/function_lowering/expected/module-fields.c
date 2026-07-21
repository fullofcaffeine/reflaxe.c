#include "hxc/program.h"

int32_t hxc_ModuleFunctions_base;

int32_t hxc_ModuleFunctions_answer(void)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_ModuleFunctions_base;
  int32_t hxc_tmp_call_result_n1 = hxc_ModuleFunctions_doubled(hxc_tmp_global_load_result_n0);
  return hxc_i32_add_wrapping(hxc_tmp_call_result_n1, 1);
}

int32_t hxc_ModuleFunctions_doubled(int32_t hxc_value)
{
  return hxc_i32_multiply_wrapping(hxc_value, 2);
}

void hxc_ModuleFunctions_main(void)
{
  while (1)
  {
    int32_t hxc_tmp_call_result_n0 = hxc_ModuleFunctions_answer();
    if (!(hxc_tmp_call_result_n0 != 15))
    {
      break;
    }
  }
  return;
}

void hxc_ModuleFunctions_static_field_base(void)
{
  hxc_ModuleFunctions_base = 7;
  return;
}

#include "hxc/program.h"

_Noreturn void hxc_FunctionFixture_recursive(int32_t hxc_l_left, int32_t hxc_l_right)
{
  while (1)
  {
    int32_t hxc_l_tmp_tail_argument_n0 = hxc_l_right;
    int32_t hxc_l_tmp_tail_argument_n1 = hxc_l_left;
    hxc_l_left = hxc_l_tmp_tail_argument_n0;
    hxc_l_right = hxc_l_tmp_tail_argument_n1;
    continue;
  }
}

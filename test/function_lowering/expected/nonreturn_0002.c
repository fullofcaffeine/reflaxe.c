#include "hxc/program.h"

_Noreturn void hxc_FunctionFixture_recursive(int32_t hxc_left, int32_t hxc_right)
{
  while (1)
  {
    int32_t hxc_tmp_tail_argument_n0 = hxc_right;
    int32_t hxc_tmp_tail_argument_n1 = hxc_left;
    hxc_left = hxc_tmp_tail_argument_n0;
    hxc_right = hxc_tmp_tail_argument_n1;
    continue;
  }
}

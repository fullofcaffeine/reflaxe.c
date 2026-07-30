#include "hxc/program.h"

bool hxc_caxecraft_gameplay_WorldItemPickup_isInRange(double hxc_l_collectorX, double hxc_l_collectorY, double hxc_l_collectorZ, int32_t hxc_l_itemXMilli, int32_t hxc_l_itemYMilli, int32_t hxc_l_itemZMilli)
{
  double hxc_l_dx = hxc_l_collectorX - hxc_f64_divide_zero_safe((double)hxc_l_itemXMilli, 1000.0);
  double hxc_l_dy = hxc_l_collectorY + 0.9 - hxc_f64_divide_zero_safe((double)hxc_l_itemYMilli, 1000.0);
  double hxc_l_dz = hxc_l_collectorZ - hxc_f64_divide_zero_safe((double)hxc_l_itemZMilli, 1000.0);
  double hxc_l_tmp_load_result_n0 = hxc_l_dx;
  double hxc_l_tmp_load_result_n1 = hxc_l_dx;
  double hxc_l_tmp_load_result_n2 = hxc_l_dy;
  double hxc_l_tmp_load_result_n3 = hxc_l_dy;
  double hxc_l_tmp_load_result_n4 = hxc_l_dz;
  return hxc_l_tmp_load_result_n0 * hxc_l_tmp_load_result_n1 + hxc_l_tmp_load_result_n2 * hxc_l_tmp_load_result_n3 + hxc_l_tmp_load_result_n4 * hxc_l_dz <= 2.25;
}

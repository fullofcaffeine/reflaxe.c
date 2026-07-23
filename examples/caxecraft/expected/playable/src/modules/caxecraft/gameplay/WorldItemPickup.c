#include "hxc/program.h"

bool hxc_caxecraft_gameplay_WorldItemPickup_isInRange(double hxc_collectorX, double hxc_collectorY, double hxc_collectorZ, int32_t hxc_itemXMilli, int32_t hxc_itemYMilli, int32_t hxc_itemZMilli)
{
  double hxc_dx = hxc_collectorX - hxc_f64_divide_zero_safe((double)hxc_itemXMilli, 1000.0);
  double hxc_dy = hxc_collectorY + 0.9 - hxc_f64_divide_zero_safe((double)hxc_itemYMilli, 1000.0);
  double hxc_dz = hxc_collectorZ - hxc_f64_divide_zero_safe((double)hxc_itemZMilli, 1000.0);
  double hxc_tmp_load_result_n0 = hxc_dx;
  double hxc_tmp_load_result_n1 = hxc_dx;
  double hxc_tmp_load_result_n2 = hxc_dy;
  double hxc_tmp_load_result_n3 = hxc_dy;
  double hxc_tmp_load_result_n4 = hxc_dz;
  return hxc_tmp_load_result_n0 * hxc_tmp_load_result_n1 + hxc_tmp_load_result_n2 * hxc_tmp_load_result_n3 + hxc_tmp_load_result_n4 * hxc_dz <= 2.25;
}

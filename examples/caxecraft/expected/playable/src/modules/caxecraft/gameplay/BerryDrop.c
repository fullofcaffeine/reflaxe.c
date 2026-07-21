#include "hxc/program.h"

struct hxc_caxecraft_gameplay_BerryDropState hxc_caxecraft_gameplay_BerryDrop_collectAmount(struct hxc_caxecraft_gameplay_BerryDropState hxc_state, int32_t hxc_amount)
{
  bool hxc_tmp_short_circuit_result_n2 = !hxc_state.hxc_active;
  if (!!hxc_state.hxc_active)
  {
    hxc_tmp_short_circuit_result_n2 = hxc_amount <= 0;
  }
  if (!hxc_tmp_short_circuit_result_n2)
  {
    int32_t hxc_remaining = hxc_i32_subtract_wrapping(hxc_state.hxc_amount, hxc_amount);
    if (!(hxc_remaining <= 0))
    {
      return (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = true, .hxc_amount = hxc_remaining, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
    }
    return (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
  }
  return hxc_state;
}

struct hxc_caxecraft_gameplay_BerryDropState hxc_caxecraft_gameplay_BerryDrop_fromDefeatedMossling(struct hxc_caxecraft_gameplay_MosslingState hxc_state)
{
  return (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = true, .hxc_amount = 2, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y + 0.24, .hxc_z = hxc_state.hxc_z };
}

bool hxc_caxecraft_gameplay_BerryDrop_isInRange(struct hxc_caxecraft_gameplay_BerryDropState hxc_state, double hxc_playerX, double hxc_playerY, double hxc_playerZ)
{
  if (!!hxc_state.hxc_active)
  {
    double hxc_dx = hxc_state.hxc_x - hxc_playerX;
    double hxc_dy = hxc_state.hxc_y - hxc_playerY;
    double hxc_dz = hxc_state.hxc_z - hxc_playerZ;
    double hxc_tmp_load_result_n4 = hxc_dx;
    double hxc_tmp_load_result_n5 = hxc_dx;
    double hxc_tmp_load_result_n6 = hxc_dy;
    double hxc_tmp_load_result_n7 = hxc_dy;
    double hxc_tmp_load_result_n8 = hxc_dz;
    return hxc_tmp_load_result_n4 * hxc_tmp_load_result_n5 + hxc_tmp_load_result_n6 * hxc_tmp_load_result_n7 + hxc_tmp_load_result_n8 * hxc_dz <= 2.25;
  }
  return false;
}

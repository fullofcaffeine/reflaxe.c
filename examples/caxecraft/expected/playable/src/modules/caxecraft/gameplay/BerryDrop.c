#include "hxc/program.h"

struct hxc_caxecraft_gameplay_BerryDropState hxc_caxecraft_gameplay_BerryDrop_collectAmount(struct hxc_caxecraft_gameplay_BerryDropState hxc_l_state, int32_t hxc_l_amount)
{
  bool hxc_l_tmp_short_circuit_result_n2 = !hxc_l_state.hxc_active;
  if (!!hxc_l_state.hxc_active)
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_amount <= 0;
  }
  if (hxc_l_tmp_short_circuit_result_n2)
  {
    return hxc_l_state;
  }
  int32_t hxc_l_remaining = hxc_i32_subtract_wrapping(hxc_l_state.hxc_amount, hxc_l_amount);
  if (!(hxc_l_remaining <= 0))
  {
    return (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = true, .hxc_amount = hxc_l_remaining, .hxc_x = hxc_l_state.hxc_x, .hxc_y = hxc_l_state.hxc_y, .hxc_z = hxc_l_state.hxc_z };
  }
  return (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = hxc_l_state.hxc_x, .hxc_y = hxc_l_state.hxc_y, .hxc_z = hxc_l_state.hxc_z };
}

struct hxc_caxecraft_gameplay_BerryDropState hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(struct hxc_caxecraft_domain_Character hxc_l_character, int32_t hxc_l_amount)
{
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_character.hxc_id <= 0;
  if (!(hxc_l_character.hxc_id <= 0))
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_character.hxc_vitals.hxc_health > 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n3 = hxc_l_tmp_short_circuit_result_n2;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_short_circuit_load_result_n3;
  if (!hxc_l_tmp_short_circuit_load_result_n3)
  {
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_amount <= 0;
  }
  if (!hxc_l_tmp_short_circuit_result_n3)
  {
    return (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = true, .hxc_amount = hxc_l_amount, .hxc_x = hxc_l_character.hxc_body.hxc_x, .hxc_y = hxc_l_character.hxc_body.hxc_y + 0.24, .hxc_z = hxc_l_character.hxc_body.hxc_z };
  }
  return (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
}

bool hxc_caxecraft_gameplay_BerryDrop_isInRange(struct hxc_caxecraft_gameplay_BerryDropState hxc_l_state, double hxc_l_playerX, double hxc_l_playerY, double hxc_l_playerZ)
{
  double hxc_l_dx = { 0 };
  double hxc_l_dy = { 0 };
  double hxc_l_dz = { 0 };
  if (!!hxc_l_state.hxc_active)
  {
    hxc_l_dx = hxc_l_state.hxc_x - hxc_l_playerX;
    hxc_l_dy = hxc_l_state.hxc_y - hxc_l_playerY;
    hxc_l_dz = hxc_l_state.hxc_z - hxc_l_playerZ;
    double hxc_l_tmp_load_result_n4 = hxc_l_dx;
    double hxc_l_tmp_load_result_n5 = hxc_l_dx;
    double hxc_l_tmp_load_result_n6 = hxc_l_dy;
    double hxc_l_tmp_load_result_n7 = hxc_l_dy;
    double hxc_l_tmp_load_result_n8 = hxc_l_dz;
    return hxc_l_tmp_load_result_n4 * hxc_l_tmp_load_result_n5 + hxc_l_tmp_load_result_n6 * hxc_l_tmp_load_result_n7 + hxc_l_tmp_load_result_n8 * hxc_l_dz <= 2.25;
  }
  return false;
}

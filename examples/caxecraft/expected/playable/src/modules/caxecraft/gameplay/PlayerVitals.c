#include "hxc/program.h"

struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_caxecraft_gameplay_PlayerVitals_make(int32_t hxc_health, int32_t hxc_safeTicks)
{
  int32_t hxc_boundedHealth = hxc_health;
  if (hxc_boundedHealth < 0)
  {
    hxc_boundedHealth = 0;
  }
  int32_t hxc_boundedSafeTicks = hxc_safeTicks;
  if (hxc_boundedSafeTicks < 0)
  {
    hxc_boundedSafeTicks = 0;
  }
  int32_t hxc_tmp_load_result_n2 = hxc_boundedHealth;
  return (struct hxc_caxecraft_gameplay_PlayerVitalsState){ .hxc_health = hxc_tmp_load_result_n2, .hxc_safeTicks = hxc_boundedSafeTicks };
}

struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_caxecraft_gameplay_PlayerVitals_recover(struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_state, int32_t hxc_amount)
{
  bool hxc_tmp_short_circuit_result_n2 = hxc_amount <= 0;
  if (!(hxc_amount <= 0))
  {
    hxc_tmp_short_circuit_result_n2 = hxc_state.hxc_health <= 0;
  }
  bool hxc_tmp_short_circuit_load_result_n1 = hxc_tmp_short_circuit_result_n2;
  bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_short_circuit_load_result_n1;
  if (!hxc_tmp_short_circuit_load_result_n1)
  {
    hxc_tmp_short_circuit_result_n3 = hxc_state.hxc_health >= 6;
  }
  if (!hxc_tmp_short_circuit_result_n3)
  {
    struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n6 = hxc_caxecraft_gameplay_PlayerVitals_make(hxc_i32_add_wrapping(hxc_state.hxc_health, hxc_amount), hxc_state.hxc_safeTicks);
    return hxc_tmp_call_result_n6;
  }
  return hxc_state;
}

struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_caxecraft_gameplay_PlayerVitals_step(struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_state, double hxc_playerX, double hxc_playerZ, double hxc_threatX, double hxc_threatZ, bool hxc_threatActive)
{
  if (!(hxc_state.hxc_health <= 0))
  {
    if (!(hxc_state.hxc_safeTicks > 0))
    {
      if (!!hxc_threatActive)
      {
        double hxc_dx = hxc_threatX - hxc_playerX;
        double hxc_dz = hxc_threatZ - hxc_playerZ;
        double hxc_tmp_load_result_n5 = hxc_dx;
        double hxc_tmp_load_result_n6 = hxc_dx;
        double hxc_tmp_load_result_n7 = hxc_dz;
        if (!(hxc_tmp_load_result_n5 * hxc_tmp_load_result_n6 + hxc_tmp_load_result_n7 * hxc_dz > 1.44))
        {
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n10 = hxc_caxecraft_gameplay_PlayerVitals_make(hxc_i32_subtract_wrapping(hxc_state.hxc_health, 1), 20);
          return hxc_tmp_call_result_n10;
        }
        return hxc_state;
      }
      return hxc_state;
    }
    struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n4 = hxc_caxecraft_gameplay_PlayerVitals_make(hxc_state.hxc_health, hxc_i32_subtract_wrapping(hxc_state.hxc_safeTicks, 1));
    return hxc_tmp_call_result_n4;
  }
  return hxc_state;
}

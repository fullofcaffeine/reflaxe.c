#include "hxc/program.h"

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_domain_Vitals_applyAttack(struct hxc_caxecraft_domain_VitalsState hxc_state, bool hxc_attacked)
{
  bool hxc_tmp_short_circuit_result_n2 = !hxc_attacked;
  if (!!hxc_attacked)
  {
    hxc_tmp_short_circuit_result_n2 = hxc_state.hxc_health <= 0;
  }
  bool hxc_tmp_short_circuit_load_result_n1 = hxc_tmp_short_circuit_result_n2;
  bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_short_circuit_load_result_n1;
  if (!hxc_tmp_short_circuit_load_result_n1)
  {
    hxc_tmp_short_circuit_result_n3 = hxc_state.hxc_safeTicks > 0;
  }
  if (!hxc_tmp_short_circuit_result_n3)
  {
    struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n5 = hxc_caxecraft_domain_Vitals_make(hxc_i32_subtract_wrapping(hxc_state.hxc_health, 1), 20);
    return hxc_tmp_call_result_n5;
  }
  return hxc_state;
}

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_domain_Vitals_make(int32_t hxc_health, int32_t hxc_safeTicks)
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
  return (struct hxc_caxecraft_domain_VitalsState){ .hxc_health = hxc_tmp_load_result_n2, .hxc_safeTicks = hxc_boundedSafeTicks };
}

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_domain_Vitals_recover(struct hxc_caxecraft_domain_VitalsState hxc_state, int32_t hxc_amount)
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
    struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n6 = hxc_caxecraft_domain_Vitals_make(hxc_i32_add_wrapping(hxc_state.hxc_health, hxc_amount), hxc_state.hxc_safeTicks);
    return hxc_tmp_call_result_n6;
  }
  return hxc_state;
}

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_domain_Vitals_step(struct hxc_caxecraft_domain_VitalsState hxc_state)
{
  if (!(hxc_state.hxc_health <= 0))
  {
    if (!(hxc_state.hxc_safeTicks > 0))
    {
      return hxc_state;
    }
    struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n4 = hxc_caxecraft_domain_Vitals_make(hxc_state.hxc_health, hxc_i32_subtract_wrapping(hxc_state.hxc_safeTicks, 1));
    return hxc_tmp_call_result_n4;
  }
  return hxc_state;
}

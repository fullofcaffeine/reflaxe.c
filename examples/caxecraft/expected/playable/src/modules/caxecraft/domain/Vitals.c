#include "hxc/program.h"

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_domain_Vitals_applyAttack(struct hxc_caxecraft_domain_VitalsState hxc_l_state, bool hxc_l_attacked)
{
  bool hxc_l_tmp_short_circuit_result_n2 = !hxc_l_attacked;
  if (!!hxc_l_attacked)
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_state.hxc_health <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n1 = hxc_l_tmp_short_circuit_result_n2;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_short_circuit_load_result_n1;
  if (!hxc_l_tmp_short_circuit_load_result_n1)
  {
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_state.hxc_safeTicks > 0;
  }
  if (!hxc_l_tmp_short_circuit_result_n3)
  {
    struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_Vitals_make(hxc_i32_subtract_wrapping(hxc_l_state.hxc_health, 1), 20);
    return hxc_l_tmp_call_result_n5;
  }
  return hxc_l_state;
}

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_domain_Vitals_applyDamage(struct hxc_caxecraft_domain_VitalsState hxc_l_state, int32_t hxc_l_amount)
{
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_amount <= 0;
  if (!(hxc_l_amount <= 0))
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_state.hxc_health <= 0;
  }
  if (!hxc_l_tmp_short_circuit_result_n2)
  {
    struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_Vitals_make(hxc_i32_subtract_wrapping(hxc_l_state.hxc_health, hxc_l_amount), hxc_l_state.hxc_safeTicks);
    return hxc_l_tmp_call_result_n4;
  }
  return hxc_l_state;
}

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_domain_Vitals_make(int32_t hxc_l_health, int32_t hxc_l_safeTicks)
{
  int32_t hxc_l_boundedHealth = hxc_l_health;
  if (hxc_l_boundedHealth < 0)
  {
    hxc_l_boundedHealth = 0;
  }
  int32_t hxc_l_boundedSafeTicks = hxc_l_safeTicks;
  if (hxc_l_boundedSafeTicks < 0)
  {
    hxc_l_boundedSafeTicks = 0;
  }
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_boundedHealth;
  return (struct hxc_caxecraft_domain_VitalsState){ .hxc_health = hxc_l_tmp_load_result_n2, .hxc_safeTicks = hxc_l_boundedSafeTicks };
}

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_domain_Vitals_recover(struct hxc_caxecraft_domain_VitalsState hxc_l_state, int32_t hxc_l_amount)
{
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_amount <= 0;
  if (!(hxc_l_amount <= 0))
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_state.hxc_health <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n1 = hxc_l_tmp_short_circuit_result_n2;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_short_circuit_load_result_n1;
  if (!hxc_l_tmp_short_circuit_load_result_n1)
  {
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_state.hxc_health >= 6;
  }
  if (hxc_l_tmp_short_circuit_result_n3)
  {
    return hxc_l_state;
  }
  int32_t hxc_l_recoveredHealth = hxc_i32_add_wrapping(hxc_l_state.hxc_health, hxc_l_amount);
  if (hxc_l_recoveredHealth > 6)
  {
    hxc_l_recoveredHealth = 6;
  }
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n8 = hxc_caxecraft_domain_Vitals_make(hxc_l_recoveredHealth, hxc_l_state.hxc_safeTicks);
  return hxc_l_tmp_call_result_n8;
}

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_domain_Vitals_step(struct hxc_caxecraft_domain_VitalsState hxc_l_state)
{
  if (hxc_l_state.hxc_health <= 0)
  {
    return hxc_l_state;
  }
  if (!(hxc_l_state.hxc_safeTicks > 0))
  {
    return hxc_l_state;
  }
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_Vitals_make(hxc_l_state.hxc_health, hxc_i32_subtract_wrapping(hxc_l_state.hxc_safeTicks, 1));
  return hxc_l_tmp_call_result_n4;
}

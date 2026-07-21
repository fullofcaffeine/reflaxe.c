#include "hxc/program.h"

struct hxc_caxecraft_gameplay_GuideState hxc_caxecraft_gameplay_GuideNpc_interact(struct hxc_caxecraft_gameplay_GuideState hxc_state)
{
  int32_t hxc_code = hxc_state.hxc_phaseCode;
  int32_t hxc_tmp_load_result_n1 = hxc_code;
  int32_t hxc_tmp_conditional_result_n3 = 0;
  if (hxc_tmp_load_result_n1 == 1)
  {
    hxc_tmp_conditional_result_n3 = 1;
  }
  else
  {
    int32_t hxc_tmp_load_result_n2 = hxc_code;
    int32_t hxc_tmp_conditional_result_n4 = 0;
    if (hxc_tmp_load_result_n2 == 2)
    {
      hxc_tmp_conditional_result_n4 = 2;
    }
    else
    {
      hxc_tmp_conditional_result_n4 = 0;
    }
    hxc_tmp_conditional_result_n3 = hxc_tmp_conditional_result_n4;
  }
  int32_t hxc_nextPhase = hxc_tmp_conditional_result_n3;
  if (hxc_nextPhase == 0)
  {
    hxc_nextPhase = 1;
  }
  else
  {
    if (hxc_nextPhase == 1)
    {
      hxc_nextPhase = 2;
    }
  }
  return (struct hxc_caxecraft_gameplay_GuideState){ .hxc_phaseCode = hxc_nextPhase, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
}

bool hxc_caxecraft_gameplay_GuideNpc_isInRange(struct hxc_caxecraft_gameplay_GuideState hxc_state, double hxc_playerX, double hxc_playerZ)
{
  double hxc_dx = hxc_state.hxc_x - hxc_playerX;
  double hxc_dz = hxc_state.hxc_z - hxc_playerZ;
  double hxc_tmp_load_result_n2 = hxc_dx;
  double hxc_tmp_load_result_n3 = hxc_dx;
  double hxc_tmp_load_result_n4 = hxc_dz;
  return hxc_tmp_load_result_n2 * hxc_tmp_load_result_n3 + hxc_tmp_load_result_n4 * hxc_dz <= 12.25;
}

bool hxc_caxecraft_gameplay_GuideNpc_sharesBerriesOnNextInteraction(struct hxc_caxecraft_gameplay_GuideState hxc_state)
{
  int32_t hxc_code = hxc_state.hxc_phaseCode;
  int32_t hxc_tmp_load_result_n1 = hxc_code;
  int32_t hxc_tmp_conditional_result_n2 = 0;
  if (hxc_tmp_load_result_n1 == 1)
  {
    hxc_tmp_conditional_result_n2 = 1;
  }
  else
  {
    int32_t hxc_tmp_load_result_n2 = hxc_code;
    int32_t hxc_tmp_conditional_result_n3 = 0;
    if (hxc_tmp_load_result_n2 == 2)
    {
      hxc_tmp_conditional_result_n3 = 2;
    }
    else
    {
      hxc_tmp_conditional_result_n3 = 0;
    }
    hxc_tmp_conditional_result_n2 = hxc_tmp_conditional_result_n3;
  }
  return hxc_tmp_conditional_result_n2 == 1;
}

struct hxc_caxecraft_gameplay_GuideState hxc_caxecraft_gameplay_GuideNpc_start(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_length, hxc_f64_to_i32_saturating(hxc_x), hxc_f64_to_i32_saturating(hxc_z));
  return (struct hxc_caxecraft_gameplay_GuideState){ .hxc_phaseCode = 0, .hxc_x = hxc_x, .hxc_y = (double)hxc_tmp_call_result_n0 + 1.0, .hxc_z = hxc_z };
}

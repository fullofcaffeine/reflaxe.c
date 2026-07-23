#include "hxc/program.h"

struct hxc_caxecraft_gameplay_GuideState hxc_caxecraft_gameplay_GuideNpc_interact(struct hxc_caxecraft_gameplay_GuideState hxc_state)
{
  enum hxc_caxecraft_gameplay_GuidePhase hxc_nextPhase = hxc_state.hxc_phase;
  if (hxc_nextPhase == hxc_caxecraft_gameplay_GuidePhase_Waiting)
  {
    hxc_nextPhase = hxc_caxecraft_gameplay_GuidePhase_Welcomed;
  }
  else
  {
    if (hxc_nextPhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed)
    {
      hxc_nextPhase = hxc_caxecraft_gameplay_GuidePhase_SharedBerries;
    }
  }
  return (struct hxc_caxecraft_gameplay_GuideState){ .hxc_phase = hxc_nextPhase, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
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
  return hxc_state.hxc_phase == hxc_caxecraft_gameplay_GuidePhase_Welcomed;
}

struct hxc_caxecraft_gameplay_GuideState hxc_caxecraft_gameplay_GuideNpc_start(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_length, hxc_f64_to_i32_saturating(hxc_x), hxc_f64_to_i32_saturating(hxc_z));
  return (struct hxc_caxecraft_gameplay_GuideState){ .hxc_phase = hxc_caxecraft_gameplay_GuidePhase_Waiting, .hxc_x = hxc_x, .hxc_y = (double)hxc_tmp_call_result_n0 + 1.0, .hxc_z = hxc_z };
}

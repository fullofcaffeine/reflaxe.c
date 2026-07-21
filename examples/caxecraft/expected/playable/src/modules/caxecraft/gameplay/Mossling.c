#include "hxc/program.h"

struct hxc_caxecraft_gameplay_MosslingState hxc_caxecraft_gameplay_Mossling_start(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_length, hxc_f64_to_i32_saturating(hxc_x), hxc_f64_to_i32_saturating(hxc_z));
  return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_homeX = hxc_x, .hxc_homeZ = hxc_z, .hxc_modeCode = 0, .hxc_x = hxc_x, .hxc_y = (double)hxc_tmp_call_result_n0 + 1.0, .hxc_z = hxc_z };
}

struct hxc_caxecraft_gameplay_MosslingState hxc_caxecraft_gameplay_Mossling_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_gameplay_MosslingState hxc_state, double hxc_playerX, double hxc_playerZ)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  double hxc_playerDx = hxc_playerX - hxc_state.hxc_x;
  double hxc_playerDz = hxc_playerZ - hxc_state.hxc_z;
  double hxc_tmp_load_result_n2 = hxc_playerDx;
  double hxc_tmp_load_result_n3 = hxc_playerDx;
  double hxc_tmp_load_result_n4 = hxc_playerDz;
  double hxc_playerDistanceSquared = hxc_tmp_load_result_n2 * hxc_tmp_load_result_n3 + hxc_tmp_load_result_n4 * hxc_playerDz;
  double hxc_homeDx = hxc_state.hxc_homeX - hxc_state.hxc_x;
  double hxc_homeDz = hxc_state.hxc_homeZ - hxc_state.hxc_z;
  double hxc_tmp_load_result_n10 = hxc_homeDx;
  double hxc_tmp_load_result_n11 = hxc_homeDx;
  double hxc_tmp_load_result_n12 = hxc_homeDz;
  double hxc_homeDistanceSquared = hxc_tmp_load_result_n10 * hxc_tmp_load_result_n11 + hxc_tmp_load_result_n12 * hxc_homeDz;
  int32_t hxc_mode = 0;
  double hxc_targetX = hxc_state.hxc_x;
  double hxc_targetZ = hxc_state.hxc_z;
  if (hxc_playerDistanceSquared <= 36.0)
  {
    hxc_mode = 1;
    hxc_targetX = hxc_playerX;
    hxc_targetZ = hxc_playerZ;
  }
  else
  {
    if (hxc_homeDistanceSquared > 0.0001)
    {
      hxc_mode = 2;
      hxc_targetX = hxc_state.hxc_homeX;
      hxc_targetZ = hxc_state.hxc_homeZ;
    }
  }
  if (!(hxc_mode == 0))
  {
    double hxc_candidateX = hxc_state.hxc_x;
    double hxc_candidateZ = hxc_state.hxc_z;
    double hxc_dx = hxc_targetX - hxc_state.hxc_x;
    double hxc_dz = hxc_targetZ - hxc_state.hxc_z;
    double hxc_tmp_load_result_n34 = hxc_dx;
    double hxc_tmp_conditional_result_n18 = 0.0;
    if (hxc_tmp_load_result_n34 < 0.0)
    {
      hxc_tmp_conditional_result_n18 = -hxc_dx;
    }
    else
    {
      hxc_tmp_conditional_result_n18 = hxc_dx;
    }
    double hxc_tmp_binary_left_n19 = hxc_tmp_conditional_result_n18;
    double hxc_tmp_load_result_n38 = hxc_dz;
    double hxc_tmp_conditional_result_n20 = 0.0;
    if (hxc_tmp_load_result_n38 < 0.0)
    {
      hxc_tmp_conditional_result_n20 = -hxc_dz;
    }
    else
    {
      hxc_tmp_conditional_result_n20 = hxc_dz;
    }
    double hxc_tmp_conditional_load_result_n41 = hxc_tmp_conditional_result_n20;
    if (hxc_tmp_binary_left_n19 >= hxc_tmp_conditional_load_result_n41)
    {
      double hxc_tmp_compound_left_n21 = hxc_candidateX;
      double hxc_tmp_load_result_n44 = hxc_dx;
      double hxc_tmp_conditional_result_n22 = 0.0;
      if (hxc_tmp_load_result_n44 > 0.08)
      {
        hxc_tmp_conditional_result_n22 = 0.08;
      }
      else
      {
        double hxc_tmp_load_result_n45 = hxc_dx;
        double hxc_tmp_conditional_result_n23 = 0.0;
        if (hxc_tmp_load_result_n45 < -0.08)
        {
          hxc_tmp_conditional_result_n23 = -0.08;
        }
        else
        {
          hxc_tmp_conditional_result_n23 = hxc_dx;
        }
        hxc_tmp_conditional_result_n22 = hxc_tmp_conditional_result_n23;
      }
      double hxc_tmp_conditional_load_result_n48 = hxc_tmp_conditional_result_n22;
      hxc_candidateX = hxc_tmp_compound_left_n21 + hxc_tmp_conditional_load_result_n48;
    }
    else
    {
      double hxc_tmp_compound_left_n24 = hxc_candidateZ;
      double hxc_tmp_load_result_n51 = hxc_dz;
      double hxc_tmp_conditional_result_n25 = 0.0;
      if (hxc_tmp_load_result_n51 > 0.08)
      {
        hxc_tmp_conditional_result_n25 = 0.08;
      }
      else
      {
        double hxc_tmp_load_result_n52 = hxc_dz;
        double hxc_tmp_conditional_result_n26 = 0.0;
        if (hxc_tmp_load_result_n52 < -0.08)
        {
          hxc_tmp_conditional_result_n26 = -0.08;
        }
        else
        {
          hxc_tmp_conditional_result_n26 = hxc_dz;
        }
        hxc_tmp_conditional_result_n25 = hxc_tmp_conditional_result_n26;
      }
      double hxc_tmp_conditional_load_result_n55 = hxc_tmp_conditional_result_n25;
      hxc_candidateZ = hxc_tmp_compound_left_n24 + hxc_tmp_conditional_load_result_n55;
    }
    double hxc_tmp_load_result_n57 = hxc_candidateX;
    int32_t hxc_tmp_call_result_n59 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_length, hxc_f64_to_i32_saturating(hxc_tmp_load_result_n57), hxc_f64_to_i32_saturating(hxc_candidateZ));
    int32_t hxc_surface = hxc_tmp_call_result_n59;
    double hxc_candidateY = (double)hxc_surface + 1.0;
    if (!(hxc_surface < 0))
    {
      double hxc_value = hxc_candidateY - hxc_state.hxc_y;
      double hxc_tmp_load_result_n71 = hxc_value;
      double hxc_tmp_conditional_result_n30 = 0.0;
      if (hxc_tmp_load_result_n71 < 0.0)
      {
        hxc_tmp_conditional_result_n30 = -hxc_value;
      }
      else
      {
        hxc_tmp_conditional_result_n30 = hxc_value;
      }
      if (!(hxc_tmp_conditional_result_n30 > 1.0))
      {
        double hxc_tmp_load_result_n82 = hxc_candidateX;
        double hxc_tmp_load_result_n83 = hxc_candidateY;
        double hxc_tmp_load_result_n84 = hxc_candidateZ;
        return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_mode, .hxc_x = hxc_tmp_load_result_n82, .hxc_y = hxc_tmp_load_result_n83, .hxc_z = hxc_tmp_load_result_n84 };
      }
      return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_mode, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
    }
    return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_mode, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
  }
  return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_mode, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
}

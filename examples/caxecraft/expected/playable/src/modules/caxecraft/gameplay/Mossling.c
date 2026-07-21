#include "hxc/program.h"

bool hxc_caxecraft_gameplay_Mossling_canStrike(struct hxc_caxecraft_gameplay_MosslingState hxc_state, double hxc_playerX, double hxc_playerZ, double hxc_lookX, double hxc_lookZ)
{
  if (!(hxc_state.hxc_health <= 0))
  {
    double hxc_dx = hxc_state.hxc_x - hxc_playerX;
    double hxc_dz = hxc_state.hxc_z - hxc_playerZ;
    double hxc_tmp_load_result_n3 = hxc_dx;
    double hxc_tmp_load_result_n4 = hxc_dx;
    double hxc_tmp_load_result_n5 = hxc_dz;
    if (!(hxc_tmp_load_result_n3 * hxc_tmp_load_result_n4 + hxc_tmp_load_result_n5 * hxc_dz > 9.0))
    {
      double hxc_tmp_load_result_n7 = hxc_dx;
      return hxc_tmp_load_result_n7 * hxc_lookX + hxc_dz * hxc_lookZ >= 0.35;
    }
    return false;
  }
  return false;
}

struct hxc_caxecraft_gameplay_MosslingState hxc_caxecraft_gameplay_Mossling_start(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_length, hxc_f64_to_i32_saturating(hxc_x), hxc_f64_to_i32_saturating(hxc_z));
  return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = 3, .hxc_homeX = hxc_x, .hxc_homeZ = hxc_z, .hxc_modeCode = 0, .hxc_x = hxc_x, .hxc_y = (double)hxc_tmp_call_result_n0 + 1.0, .hxc_z = hxc_z };
}

struct hxc_caxecraft_gameplay_MosslingState hxc_caxecraft_gameplay_Mossling_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_gameplay_MosslingState hxc_state, double hxc_playerX, double hxc_playerZ)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  if (!(hxc_state.hxc_health <= 0))
  {
    double hxc_playerDx = hxc_playerX - hxc_state.hxc_x;
    double hxc_playerDz = hxc_playerZ - hxc_state.hxc_z;
    double hxc_tmp_load_result_n3 = hxc_playerDx;
    double hxc_tmp_load_result_n4 = hxc_playerDx;
    double hxc_tmp_load_result_n5 = hxc_playerDz;
    double hxc_playerDistanceSquared = hxc_tmp_load_result_n3 * hxc_tmp_load_result_n4 + hxc_tmp_load_result_n5 * hxc_playerDz;
    double hxc_homeDx = hxc_state.hxc_homeX - hxc_state.hxc_x;
    double hxc_homeDz = hxc_state.hxc_homeZ - hxc_state.hxc_z;
    double hxc_tmp_load_result_n11 = hxc_homeDx;
    double hxc_tmp_load_result_n12 = hxc_homeDx;
    double hxc_tmp_load_result_n13 = hxc_homeDz;
    double hxc_homeDistanceSquared = hxc_tmp_load_result_n11 * hxc_tmp_load_result_n12 + hxc_tmp_load_result_n13 * hxc_homeDz;
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
      double hxc_tmp_load_result_n36 = hxc_dx;
      double hxc_tmp_conditional_result_n18 = 0.0;
      if (hxc_tmp_load_result_n36 < 0.0)
      {
        hxc_tmp_conditional_result_n18 = -hxc_dx;
      }
      else
      {
        hxc_tmp_conditional_result_n18 = hxc_dx;
      }
      double hxc_tmp_binary_left_n19 = hxc_tmp_conditional_result_n18;
      double hxc_tmp_load_result_n40 = hxc_dz;
      double hxc_tmp_conditional_result_n20 = 0.0;
      if (hxc_tmp_load_result_n40 < 0.0)
      {
        hxc_tmp_conditional_result_n20 = -hxc_dz;
      }
      else
      {
        hxc_tmp_conditional_result_n20 = hxc_dz;
      }
      double hxc_tmp_conditional_load_result_n43 = hxc_tmp_conditional_result_n20;
      if (hxc_tmp_binary_left_n19 >= hxc_tmp_conditional_load_result_n43)
      {
        double hxc_tmp_compound_left_n21 = hxc_candidateX;
        double hxc_tmp_load_result_n46 = hxc_dx;
        double hxc_tmp_conditional_result_n22 = 0.0;
        if (hxc_tmp_load_result_n46 > 0.08)
        {
          hxc_tmp_conditional_result_n22 = 0.08;
        }
        else
        {
          double hxc_tmp_load_result_n47 = hxc_dx;
          double hxc_tmp_conditional_result_n23 = 0.0;
          if (hxc_tmp_load_result_n47 < -0.08)
          {
            hxc_tmp_conditional_result_n23 = -0.08;
          }
          else
          {
            hxc_tmp_conditional_result_n23 = hxc_dx;
          }
          hxc_tmp_conditional_result_n22 = hxc_tmp_conditional_result_n23;
        }
        double hxc_tmp_conditional_load_result_n50 = hxc_tmp_conditional_result_n22;
        hxc_candidateX = hxc_tmp_compound_left_n21 + hxc_tmp_conditional_load_result_n50;
      }
      else
      {
        double hxc_tmp_compound_left_n24 = hxc_candidateZ;
        double hxc_tmp_load_result_n53 = hxc_dz;
        double hxc_tmp_conditional_result_n25 = 0.0;
        if (hxc_tmp_load_result_n53 > 0.08)
        {
          hxc_tmp_conditional_result_n25 = 0.08;
        }
        else
        {
          double hxc_tmp_load_result_n54 = hxc_dz;
          double hxc_tmp_conditional_result_n26 = 0.0;
          if (hxc_tmp_load_result_n54 < -0.08)
          {
            hxc_tmp_conditional_result_n26 = -0.08;
          }
          else
          {
            hxc_tmp_conditional_result_n26 = hxc_dz;
          }
          hxc_tmp_conditional_result_n25 = hxc_tmp_conditional_result_n26;
        }
        double hxc_tmp_conditional_load_result_n57 = hxc_tmp_conditional_result_n25;
        hxc_candidateZ = hxc_tmp_compound_left_n24 + hxc_tmp_conditional_load_result_n57;
      }
      double hxc_tmp_load_result_n59 = hxc_candidateX;
      int32_t hxc_tmp_call_result_n61 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_length, hxc_f64_to_i32_saturating(hxc_tmp_load_result_n59), hxc_f64_to_i32_saturating(hxc_candidateZ));
      int32_t hxc_surface = hxc_tmp_call_result_n61;
      double hxc_candidateY = (double)hxc_surface + 1.0;
      if (!(hxc_surface < 0))
      {
        double hxc_value = hxc_candidateY - hxc_state.hxc_y;
        double hxc_tmp_load_result_n74 = hxc_value;
        double hxc_tmp_conditional_result_n30 = 0.0;
        if (hxc_tmp_load_result_n74 < 0.0)
        {
          hxc_tmp_conditional_result_n30 = -hxc_value;
        }
        else
        {
          hxc_tmp_conditional_result_n30 = hxc_value;
        }
        if (!(hxc_tmp_conditional_result_n30 > 1.0))
        {
          double hxc_tmp_load_result_n86 = hxc_candidateX;
          double hxc_tmp_load_result_n87 = hxc_candidateY;
          double hxc_tmp_load_result_n88 = hxc_candidateZ;
          return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_mode, .hxc_x = hxc_tmp_load_result_n86, .hxc_y = hxc_tmp_load_result_n87, .hxc_z = hxc_tmp_load_result_n88 };
        }
        return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_mode, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
      }
      return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_mode, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
    }
    return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_mode, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
  }
  return hxc_state;
}

struct hxc_caxecraft_gameplay_MosslingState hxc_caxecraft_gameplay_Mossling_strike(struct hxc_caxecraft_gameplay_MosslingState hxc_state)
{
  int32_t hxc_nextHealth = hxc_state.hxc_health;
  if (hxc_nextHealth > 0)
  {
    hxc_nextHealth = hxc_i32_subtract_wrapping(hxc_nextHealth, 1);
  }
  int32_t hxc_code = hxc_state.hxc_modeCode;
  int32_t hxc_tmp_load_result_n4 = hxc_code;
  int32_t hxc_tmp_conditional_result_n4 = 0;
  if (hxc_tmp_load_result_n4 == 1)
  {
    hxc_tmp_conditional_result_n4 = 1;
  }
  else
  {
    int32_t hxc_tmp_load_result_n5 = hxc_code;
    int32_t hxc_tmp_conditional_result_n5 = 0;
    if (hxc_tmp_load_result_n5 == 2)
    {
      hxc_tmp_conditional_result_n5 = 2;
    }
    else
    {
      hxc_tmp_conditional_result_n5 = 0;
    }
    hxc_tmp_conditional_result_n4 = hxc_tmp_conditional_result_n5;
  }
  int32_t hxc_currentMode = hxc_tmp_conditional_result_n4;
  int32_t hxc_tmp_load_result_n13 = hxc_currentMode;
  return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_nextHealth, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_tmp_load_result_n13, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
}

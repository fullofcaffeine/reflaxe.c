#include "hxc/program.h"

bool hxc_caxecraft_gameplay_Mossling_attacksThisTick(struct hxc_caxecraft_gameplay_MosslingState hxc_state, double hxc_playerX, double hxc_playerZ)
{
  bool hxc_tmp_short_circuit_result_n3 = hxc_state.hxc_health <= 0;
  if (!(hxc_state.hxc_health <= 0))
  {
    hxc_tmp_short_circuit_result_n3 = hxc_state.hxc_mode != hxc_caxecraft_gameplay_MosslingMode_Windup;
  }
  bool hxc_tmp_short_circuit_load_result_n3 = hxc_tmp_short_circuit_result_n3;
  bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_short_circuit_load_result_n3;
  if (!hxc_tmp_short_circuit_load_result_n3)
  {
    hxc_tmp_short_circuit_result_n4 = hxc_state.hxc_phaseTicks != 1;
  }
  if (!hxc_tmp_short_circuit_result_n4)
  {
    double hxc_dx = hxc_playerX - hxc_state.hxc_x;
    double hxc_dz = hxc_playerZ - hxc_state.hxc_z;
    double hxc_tmp_load_result_n8 = hxc_dx;
    double hxc_tmp_load_result_n9 = hxc_dx;
    double hxc_tmp_load_result_n10 = hxc_dz;
    return hxc_tmp_load_result_n8 * hxc_tmp_load_result_n9 + hxc_tmp_load_result_n10 * hxc_dz <= 1.96;
  }
  return false;
}

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
  return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = 3, .hxc_homeX = hxc_x, .hxc_homeZ = hxc_z, .hxc_mode = hxc_caxecraft_gameplay_MosslingMode_Resting, .hxc_phaseTicks = 0, .hxc_x = hxc_x, .hxc_y = (double)hxc_tmp_call_result_n0 + 1.0, .hxc_z = hxc_z };
}

struct hxc_caxecraft_gameplay_MosslingState hxc_caxecraft_gameplay_Mossling_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_gameplay_MosslingState hxc_state, double hxc_playerX, double hxc_playerZ, int32_t hxc_tickNumber)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n5 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n5;
  if (!(hxc_state.hxc_health <= 0))
  {
    enum hxc_caxecraft_gameplay_MosslingMode hxc_currentMode = hxc_state.hxc_mode;
    double hxc_dx_haec5b9f9eceb = hxc_playerX - hxc_state.hxc_x;
    double hxc_dz_he89d6aff5187 = hxc_playerZ - hxc_state.hxc_z;
    double hxc_tmp_load_result_n4 = hxc_dx_haec5b9f9eceb;
    double hxc_tmp_load_result_n5 = hxc_dx_haec5b9f9eceb;
    double hxc_tmp_load_result_n6 = hxc_dz_he89d6aff5187;
    double hxc_playerDistanceSquared = hxc_tmp_load_result_n4 * hxc_tmp_load_result_n5 + hxc_tmp_load_result_n6 * hxc_dz_he89d6aff5187;
    if (!(hxc_currentMode == hxc_caxecraft_gameplay_MosslingMode_Windup))
    {
      enum hxc_caxecraft_gameplay_MosslingMode hxc_tmp_load_result_n37 = hxc_currentMode;
      bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_load_result_n37 == hxc_caxecraft_gameplay_MosslingMode_Recovering;
      if (hxc_tmp_load_result_n37 == hxc_caxecraft_gameplay_MosslingMode_Recovering)
      {
        hxc_tmp_short_circuit_result_n10 = hxc_state.hxc_phaseTicks > 1;
      }
      if (!hxc_tmp_short_circuit_result_n10)
      {
        if (!(hxc_playerDistanceSquared <= 1.96))
        {
          double hxc_homeDx = hxc_state.hxc_homeX - hxc_state.hxc_x;
          double hxc_homeDz = hxc_state.hxc_homeZ - hxc_state.hxc_z;
          double hxc_tmp_load_result_n63 = hxc_homeDx;
          double hxc_tmp_load_result_n64 = hxc_homeDx;
          double hxc_tmp_load_result_n65 = hxc_homeDz;
          double hxc_homeDistanceSquared = hxc_tmp_load_result_n63 * hxc_tmp_load_result_n64 + hxc_tmp_load_result_n65 * hxc_homeDz;
          enum hxc_caxecraft_gameplay_MosslingMode hxc_nextMode = hxc_caxecraft_gameplay_MosslingMode_Resting;
          double hxc_targetX = hxc_state.hxc_x;
          double hxc_targetZ = hxc_state.hxc_z;
          if (hxc_playerDistanceSquared <= 36.0)
          {
            hxc_nextMode = hxc_caxecraft_gameplay_MosslingMode_Chasing;
            hxc_targetX = hxc_playerX;
            hxc_targetZ = hxc_playerZ;
          }
          else
          {
            enum hxc_caxecraft_gameplay_MosslingMode hxc_tmp_load_result_n72 = hxc_currentMode;
            bool hxc_tmp_short_circuit_result_n17 = hxc_tmp_load_result_n72 == hxc_caxecraft_gameplay_MosslingMode_Chasing;
            if (!(hxc_tmp_load_result_n72 == hxc_caxecraft_gameplay_MosslingMode_Chasing))
            {
              hxc_tmp_short_circuit_result_n17 = hxc_currentMode == hxc_caxecraft_gameplay_MosslingMode_Returning;
            }
            bool hxc_tmp_short_circuit_load_result_n76 = hxc_tmp_short_circuit_result_n17;
            bool hxc_tmp_short_circuit_result_n18 = hxc_tmp_short_circuit_load_result_n76;
            if (hxc_tmp_short_circuit_load_result_n76)
            {
              hxc_tmp_short_circuit_result_n18 = hxc_homeDistanceSquared > 0.0001;
            }
            if (hxc_tmp_short_circuit_result_n18)
            {
              hxc_nextMode = hxc_caxecraft_gameplay_MosslingMode_Returning;
              hxc_targetX = hxc_state.hxc_homeX;
              hxc_targetZ = hxc_state.hxc_homeZ;
            }
            else
            {
              int32_t hxc_cycleTick = hxc_i32_modulo_zero_safe(hxc_tickNumber, 240);
              if (hxc_cycleTick < 0)
              {
                hxc_cycleTick = hxc_i32_add_wrapping(hxc_cycleTick, 240);
              }
              hxc_targetX = hxc_state.hxc_homeX;
              hxc_targetZ = hxc_state.hxc_homeZ;
              int32_t hxc_tmp_load_result_n86 = hxc_cycleTick;
              bool hxc_tmp_short_circuit_result_n20 = hxc_tmp_load_result_n86 >= 20;
              if (hxc_tmp_load_result_n86 >= 20)
              {
                hxc_tmp_short_circuit_result_n20 = hxc_cycleTick < 60;
              }
              if (hxc_tmp_short_circuit_result_n20)
              {
                hxc_targetX = hxc_targetX + 1.0;
              }
              else
              {
                int32_t hxc_tmp_load_result_n90 = hxc_cycleTick;
                bool hxc_tmp_short_circuit_result_n21 = hxc_tmp_load_result_n90 >= 60;
                if (hxc_tmp_load_result_n90 >= 60)
                {
                  hxc_tmp_short_circuit_result_n21 = hxc_cycleTick < 100;
                }
                if (hxc_tmp_short_circuit_result_n21)
                {
                  hxc_targetX = hxc_targetX + 1.0;
                  hxc_targetZ = hxc_targetZ + 1.0;
                }
                else
                {
                  int32_t hxc_tmp_load_result_n95 = hxc_cycleTick;
                  bool hxc_tmp_short_circuit_result_n22 = hxc_tmp_load_result_n95 >= 100;
                  if (hxc_tmp_load_result_n95 >= 100)
                  {
                    hxc_tmp_short_circuit_result_n22 = hxc_cycleTick < 140;
                  }
                  if (hxc_tmp_short_circuit_result_n22)
                  {
                    hxc_targetX = hxc_targetX - 1.0;
                    hxc_targetZ = hxc_targetZ + 1.0;
                  }
                  else
                  {
                    int32_t hxc_tmp_load_result_n100 = hxc_cycleTick;
                    bool hxc_tmp_short_circuit_result_n23 = hxc_tmp_load_result_n100 >= 140;
                    if (hxc_tmp_load_result_n100 >= 140)
                    {
                      hxc_tmp_short_circuit_result_n23 = hxc_cycleTick < 180;
                    }
                    if (hxc_tmp_short_circuit_result_n23)
                    {
                      hxc_targetX = hxc_targetX - 1.0;
                      hxc_targetZ = hxc_targetZ - 1.0;
                    }
                    else
                    {
                      int32_t hxc_tmp_load_result_n105 = hxc_cycleTick;
                      bool hxc_tmp_short_circuit_result_n24 = hxc_tmp_load_result_n105 >= 180;
                      if (hxc_tmp_load_result_n105 >= 180)
                      {
                        hxc_tmp_short_circuit_result_n24 = hxc_cycleTick < 220;
                      }
                      if (hxc_tmp_short_circuit_result_n24)
                      {
                        hxc_targetZ = hxc_targetZ - 1.0;
                      }
                    }
                  }
                }
              }
              double hxc_wanderDx = hxc_targetX - hxc_state.hxc_x;
              double hxc_wanderDz = hxc_targetZ - hxc_state.hxc_z;
              double hxc_tmp_load_result_n113 = hxc_wanderDx;
              double hxc_tmp_load_result_n114 = hxc_wanderDx;
              double hxc_tmp_load_result_n115 = hxc_wanderDz;
              if (hxc_tmp_load_result_n113 * hxc_tmp_load_result_n114 + hxc_tmp_load_result_n115 * hxc_wanderDz > 0.0001)
              {
                hxc_nextMode = hxc_caxecraft_gameplay_MosslingMode_Wandering;
              }
            }
          }
          if (!(hxc_nextMode == hxc_caxecraft_gameplay_MosslingMode_Resting))
          {
            double hxc_candidateX = hxc_state.hxc_x;
            double hxc_candidateZ = hxc_state.hxc_z;
            double hxc_dx_h1c67db7572bc = hxc_targetX - hxc_state.hxc_x;
            double hxc_dz_h5a414b986b35 = hxc_targetZ - hxc_state.hxc_z;
            double hxc_tmp_load_result_n134 = hxc_dx_h1c67db7572bc;
            double hxc_tmp_conditional_result_n31 = 0.0;
            if (hxc_tmp_load_result_n134 < 0.0)
            {
              hxc_tmp_conditional_result_n31 = -hxc_dx_h1c67db7572bc;
            }
            else
            {
              hxc_tmp_conditional_result_n31 = hxc_dx_h1c67db7572bc;
            }
            double hxc_tmp_binary_left_n32 = hxc_tmp_conditional_result_n31;
            double hxc_tmp_load_result_n138 = hxc_dz_h5a414b986b35;
            double hxc_tmp_conditional_result_n33 = 0.0;
            if (hxc_tmp_load_result_n138 < 0.0)
            {
              hxc_tmp_conditional_result_n33 = -hxc_dz_h5a414b986b35;
            }
            else
            {
              hxc_tmp_conditional_result_n33 = hxc_dz_h5a414b986b35;
            }
            double hxc_tmp_conditional_load_result_n141 = hxc_tmp_conditional_result_n33;
            if (hxc_tmp_binary_left_n32 >= hxc_tmp_conditional_load_result_n141)
            {
              double hxc_tmp_compound_left_n34 = hxc_candidateX;
              double hxc_tmp_load_result_n144 = hxc_dx_h1c67db7572bc;
              double hxc_tmp_conditional_result_n35 = 0.0;
              if (hxc_tmp_load_result_n144 > 0.08)
              {
                hxc_tmp_conditional_result_n35 = 0.08;
              }
              else
              {
                double hxc_tmp_load_result_n145 = hxc_dx_h1c67db7572bc;
                double hxc_tmp_conditional_result_n36 = 0.0;
                if (hxc_tmp_load_result_n145 < -0.08)
                {
                  hxc_tmp_conditional_result_n36 = -0.08;
                }
                else
                {
                  hxc_tmp_conditional_result_n36 = hxc_dx_h1c67db7572bc;
                }
                hxc_tmp_conditional_result_n35 = hxc_tmp_conditional_result_n36;
              }
              double hxc_tmp_conditional_load_result_n148 = hxc_tmp_conditional_result_n35;
              hxc_candidateX = hxc_tmp_compound_left_n34 + hxc_tmp_conditional_load_result_n148;
            }
            else
            {
              double hxc_tmp_compound_left_n37 = hxc_candidateZ;
              double hxc_tmp_load_result_n151 = hxc_dz_h5a414b986b35;
              double hxc_tmp_conditional_result_n38 = 0.0;
              if (hxc_tmp_load_result_n151 > 0.08)
              {
                hxc_tmp_conditional_result_n38 = 0.08;
              }
              else
              {
                double hxc_tmp_load_result_n152 = hxc_dz_h5a414b986b35;
                double hxc_tmp_conditional_result_n39 = 0.0;
                if (hxc_tmp_load_result_n152 < -0.08)
                {
                  hxc_tmp_conditional_result_n39 = -0.08;
                }
                else
                {
                  hxc_tmp_conditional_result_n39 = hxc_dz_h5a414b986b35;
                }
                hxc_tmp_conditional_result_n38 = hxc_tmp_conditional_result_n39;
              }
              double hxc_tmp_conditional_load_result_n155 = hxc_tmp_conditional_result_n38;
              hxc_candidateZ = hxc_tmp_compound_left_n37 + hxc_tmp_conditional_load_result_n155;
            }
            double hxc_tmp_load_result_n157 = hxc_candidateX;
            int32_t hxc_tmp_call_result_n159 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_length, hxc_f64_to_i32_saturating(hxc_tmp_load_result_n157), hxc_f64_to_i32_saturating(hxc_candidateZ));
            int32_t hxc_surface = hxc_tmp_call_result_n159;
            double hxc_candidateY = (double)hxc_surface + 1.0;
            if (!(hxc_surface < 0))
            {
              double hxc_value = hxc_candidateY - hxc_state.hxc_y;
              double hxc_tmp_load_result_n172 = hxc_value;
              double hxc_tmp_conditional_result_n43 = 0.0;
              if (hxc_tmp_load_result_n172 < 0.0)
              {
                hxc_tmp_conditional_result_n43 = -hxc_value;
              }
              else
              {
                hxc_tmp_conditional_result_n43 = hxc_value;
              }
              if (!(hxc_tmp_conditional_result_n43 > 1.0))
              {
                double hxc_tmp_load_result_n184 = hxc_candidateX;
                double hxc_tmp_load_result_n185 = hxc_candidateY;
                double hxc_tmp_load_result_n186 = hxc_candidateZ;
                return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_nextMode, .hxc_phaseTicks = 0, .hxc_x = hxc_tmp_load_result_n184, .hxc_y = hxc_tmp_load_result_n185, .hxc_z = hxc_tmp_load_result_n186 };
              }
              return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_nextMode, .hxc_phaseTicks = 0, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
            }
            return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_nextMode, .hxc_phaseTicks = 0, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
          }
          return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_nextMode, .hxc_phaseTicks = 0, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
        }
        return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_caxecraft_gameplay_MosslingMode_Windup, .hxc_phaseTicks = 8, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
      }
      return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_currentMode, .hxc_phaseTicks = hxc_i32_subtract_wrapping(hxc_state.hxc_phaseTicks, 1), .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
    }
    if (!(hxc_playerDistanceSquared > 1.96))
    {
      if (!(hxc_state.hxc_phaseTicks > 1))
      {
        return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_caxecraft_gameplay_MosslingMode_Recovering, .hxc_phaseTicks = 12, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
      }
      return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_currentMode, .hxc_phaseTicks = hxc_i32_subtract_wrapping(hxc_state.hxc_phaseTicks, 1), .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
    }
    return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_caxecraft_gameplay_MosslingMode_Chasing, .hxc_phaseTicks = 0, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
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
  return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_nextHealth, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_mode = hxc_state.hxc_mode, .hxc_phaseTicks = hxc_state.hxc_phaseTicks, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
}

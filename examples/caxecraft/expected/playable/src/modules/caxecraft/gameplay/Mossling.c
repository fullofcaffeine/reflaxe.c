#include "hxc/program.h"

bool hxc_caxecraft_gameplay_Mossling_attacksThisTick(struct hxc_caxecraft_gameplay_MosslingState hxc_state, double hxc_playerX, double hxc_playerZ)
{
  bool hxc_tmp = false;
  if (hxc_state.hxc_health > 0)
  {
    int32_t hxc_code = hxc_state.hxc_modeCode;
    int32_t hxc_tmp_load_result_n2 = hxc_code;
    int32_t hxc_tmp_conditional_result_n5 = 0;
    if (hxc_tmp_load_result_n2 == 1)
    {
      hxc_tmp_conditional_result_n5 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n3 = hxc_code;
      int32_t hxc_tmp_conditional_result_n6 = 0;
      if (hxc_tmp_load_result_n3 == 2)
      {
        hxc_tmp_conditional_result_n6 = 2;
      }
      else
      {
        int32_t hxc_tmp_load_result_n4 = hxc_code;
        int32_t hxc_tmp_conditional_result_n7 = 0;
        if (hxc_tmp_load_result_n4 == 3)
        {
          hxc_tmp_conditional_result_n7 = 3;
        }
        else
        {
          int32_t hxc_tmp_load_result_n5 = hxc_code;
          int32_t hxc_tmp_conditional_result_n8 = 0;
          if (hxc_tmp_load_result_n5 == 4)
          {
            hxc_tmp_conditional_result_n8 = 4;
          }
          else
          {
            int32_t hxc_tmp_load_result_n6 = hxc_code;
            int32_t hxc_tmp_conditional_result_n9 = 0;
            if (hxc_tmp_load_result_n6 == 5)
            {
              hxc_tmp_conditional_result_n9 = 5;
            }
            else
            {
              hxc_tmp_conditional_result_n9 = 0;
            }
            hxc_tmp_conditional_result_n8 = hxc_tmp_conditional_result_n9;
          }
          hxc_tmp_conditional_result_n7 = hxc_tmp_conditional_result_n8;
        }
        hxc_tmp_conditional_result_n6 = hxc_tmp_conditional_result_n7;
      }
      hxc_tmp_conditional_result_n5 = hxc_tmp_conditional_result_n6;
    }
    hxc_tmp = hxc_tmp_conditional_result_n5 != 4;
  }
  else
  {
    hxc_tmp = true;
  }
  bool hxc_tmp_load_result_n12 = hxc_tmp;
  bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_load_result_n12;
  if (!hxc_tmp_load_result_n12)
  {
    hxc_tmp_short_circuit_result_n10 = hxc_state.hxc_phaseTicks != 1;
  }
  if (!hxc_tmp_short_circuit_result_n10)
  {
    double hxc_dx = hxc_playerX - hxc_state.hxc_x;
    double hxc_dz = hxc_playerZ - hxc_state.hxc_z;
    double hxc_tmp_load_result_n17 = hxc_dx;
    double hxc_tmp_load_result_n18 = hxc_dx;
    double hxc_tmp_load_result_n19 = hxc_dz;
    return hxc_tmp_load_result_n17 * hxc_tmp_load_result_n18 + hxc_tmp_load_result_n19 * hxc_dz <= 1.96;
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
  return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = 3, .hxc_homeX = hxc_x, .hxc_homeZ = hxc_z, .hxc_modeCode = 0, .hxc_phaseTicks = 0, .hxc_x = hxc_x, .hxc_y = (double)hxc_tmp_call_result_n0 + 1.0, .hxc_z = hxc_z };
}

struct hxc_caxecraft_gameplay_MosslingState hxc_caxecraft_gameplay_Mossling_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_gameplay_MosslingState hxc_state, double hxc_playerX, double hxc_playerZ, int32_t hxc_tickNumber)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n5 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n5;
  if (!(hxc_state.hxc_health <= 0))
  {
    int32_t hxc_code = hxc_state.hxc_modeCode;
    int32_t hxc_tmp_load_result_n2 = hxc_code;
    int32_t hxc_tmp_conditional_result_n8 = 0;
    if (hxc_tmp_load_result_n2 == 1)
    {
      hxc_tmp_conditional_result_n8 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n3 = hxc_code;
      int32_t hxc_tmp_conditional_result_n9 = 0;
      if (hxc_tmp_load_result_n3 == 2)
      {
        hxc_tmp_conditional_result_n9 = 2;
      }
      else
      {
        int32_t hxc_tmp_load_result_n4 = hxc_code;
        int32_t hxc_tmp_conditional_result_n10 = 0;
        if (hxc_tmp_load_result_n4 == 3)
        {
          hxc_tmp_conditional_result_n10 = 3;
        }
        else
        {
          int32_t hxc_tmp_load_result_n5 = hxc_code;
          int32_t hxc_tmp_conditional_result_n11 = 0;
          if (hxc_tmp_load_result_n5 == 4)
          {
            hxc_tmp_conditional_result_n11 = 4;
          }
          else
          {
            int32_t hxc_tmp_load_result_n6 = hxc_code;
            int32_t hxc_tmp_conditional_result_n12 = 0;
            if (hxc_tmp_load_result_n6 == 5)
            {
              hxc_tmp_conditional_result_n12 = 5;
            }
            else
            {
              hxc_tmp_conditional_result_n12 = 0;
            }
            hxc_tmp_conditional_result_n11 = hxc_tmp_conditional_result_n12;
          }
          hxc_tmp_conditional_result_n10 = hxc_tmp_conditional_result_n11;
        }
        hxc_tmp_conditional_result_n9 = hxc_tmp_conditional_result_n10;
      }
      hxc_tmp_conditional_result_n8 = hxc_tmp_conditional_result_n9;
    }
    int32_t hxc_currentMode = hxc_tmp_conditional_result_n8;
    double hxc_dx_hfb925925d3f9 = hxc_playerX - hxc_state.hxc_x;
    double hxc_dz_he911879b12cd = hxc_playerZ - hxc_state.hxc_z;
    double hxc_tmp_load_result_n14 = hxc_dx_hfb925925d3f9;
    double hxc_tmp_load_result_n15 = hxc_dx_hfb925925d3f9;
    double hxc_tmp_load_result_n16 = hxc_dz_he911879b12cd;
    double hxc_playerDistanceSquared = hxc_tmp_load_result_n14 * hxc_tmp_load_result_n15 + hxc_tmp_load_result_n16 * hxc_dz_he911879b12cd;
    if (!(hxc_currentMode == 4))
    {
      int32_t hxc_tmp_load_result_n44 = hxc_currentMode;
      bool hxc_tmp_short_circuit_result_n16 = hxc_tmp_load_result_n44 == 5;
      if (hxc_tmp_load_result_n44 == 5)
      {
        hxc_tmp_short_circuit_result_n16 = hxc_state.hxc_phaseTicks > 1;
      }
      if (!hxc_tmp_short_circuit_result_n16)
      {
        if (!(hxc_playerDistanceSquared <= 1.96))
        {
          double hxc_homeDx = hxc_state.hxc_homeX - hxc_state.hxc_x;
          double hxc_homeDz = hxc_state.hxc_homeZ - hxc_state.hxc_z;
          double hxc_tmp_load_result_n68 = hxc_homeDx;
          double hxc_tmp_load_result_n69 = hxc_homeDx;
          double hxc_tmp_load_result_n70 = hxc_homeDz;
          double hxc_homeDistanceSquared = hxc_tmp_load_result_n68 * hxc_tmp_load_result_n69 + hxc_tmp_load_result_n70 * hxc_homeDz;
          int32_t hxc_nextMode = 0;
          double hxc_targetX = hxc_state.hxc_x;
          double hxc_targetZ = hxc_state.hxc_z;
          if (hxc_playerDistanceSquared <= 36.0)
          {
            hxc_nextMode = 1;
            hxc_targetX = hxc_playerX;
            hxc_targetZ = hxc_playerZ;
          }
          else
          {
            int32_t hxc_tmp_load_result_n75 = hxc_currentMode;
            bool hxc_tmp_short_circuit_result_n23 = hxc_tmp_load_result_n75 == 1;
            if (!(hxc_tmp_load_result_n75 == 1))
            {
              hxc_tmp_short_circuit_result_n23 = hxc_currentMode == 2;
            }
            bool hxc_tmp_short_circuit_load_result_n77 = hxc_tmp_short_circuit_result_n23;
            bool hxc_tmp_short_circuit_result_n24 = hxc_tmp_short_circuit_load_result_n77;
            if (hxc_tmp_short_circuit_load_result_n77)
            {
              hxc_tmp_short_circuit_result_n24 = hxc_homeDistanceSquared > 0.0001;
            }
            if (hxc_tmp_short_circuit_result_n24)
            {
              hxc_nextMode = 2;
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
              bool hxc_tmp_short_circuit_result_n26 = hxc_tmp_load_result_n86 >= 20;
              if (hxc_tmp_load_result_n86 >= 20)
              {
                hxc_tmp_short_circuit_result_n26 = hxc_cycleTick < 60;
              }
              if (hxc_tmp_short_circuit_result_n26)
              {
                hxc_targetX = hxc_targetX + 1.0;
              }
              else
              {
                int32_t hxc_tmp_load_result_n90 = hxc_cycleTick;
                bool hxc_tmp_short_circuit_result_n27 = hxc_tmp_load_result_n90 >= 60;
                if (hxc_tmp_load_result_n90 >= 60)
                {
                  hxc_tmp_short_circuit_result_n27 = hxc_cycleTick < 100;
                }
                if (hxc_tmp_short_circuit_result_n27)
                {
                  hxc_targetX = hxc_targetX + 1.0;
                  hxc_targetZ = hxc_targetZ + 1.0;
                }
                else
                {
                  int32_t hxc_tmp_load_result_n95 = hxc_cycleTick;
                  bool hxc_tmp_short_circuit_result_n28 = hxc_tmp_load_result_n95 >= 100;
                  if (hxc_tmp_load_result_n95 >= 100)
                  {
                    hxc_tmp_short_circuit_result_n28 = hxc_cycleTick < 140;
                  }
                  if (hxc_tmp_short_circuit_result_n28)
                  {
                    hxc_targetX = hxc_targetX - 1.0;
                    hxc_targetZ = hxc_targetZ + 1.0;
                  }
                  else
                  {
                    int32_t hxc_tmp_load_result_n100 = hxc_cycleTick;
                    bool hxc_tmp_short_circuit_result_n29 = hxc_tmp_load_result_n100 >= 140;
                    if (hxc_tmp_load_result_n100 >= 140)
                    {
                      hxc_tmp_short_circuit_result_n29 = hxc_cycleTick < 180;
                    }
                    if (hxc_tmp_short_circuit_result_n29)
                    {
                      hxc_targetX = hxc_targetX - 1.0;
                      hxc_targetZ = hxc_targetZ - 1.0;
                    }
                    else
                    {
                      int32_t hxc_tmp_load_result_n105 = hxc_cycleTick;
                      bool hxc_tmp_short_circuit_result_n30 = hxc_tmp_load_result_n105 >= 180;
                      if (hxc_tmp_load_result_n105 >= 180)
                      {
                        hxc_tmp_short_circuit_result_n30 = hxc_cycleTick < 220;
                      }
                      if (hxc_tmp_short_circuit_result_n30)
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
                hxc_nextMode = 3;
              }
            }
          }
          if (!(hxc_nextMode == 0))
          {
            double hxc_candidateX = hxc_state.hxc_x;
            double hxc_candidateZ = hxc_state.hxc_z;
            double hxc_dx_hc84b106395d5 = hxc_targetX - hxc_state.hxc_x;
            double hxc_dz_h0f76a758436d = hxc_targetZ - hxc_state.hxc_z;
            double hxc_tmp_load_result_n132 = hxc_dx_hc84b106395d5;
            double hxc_tmp_conditional_result_n37 = 0.0;
            if (hxc_tmp_load_result_n132 < 0.0)
            {
              hxc_tmp_conditional_result_n37 = -hxc_dx_hc84b106395d5;
            }
            else
            {
              hxc_tmp_conditional_result_n37 = hxc_dx_hc84b106395d5;
            }
            double hxc_tmp_binary_left_n38 = hxc_tmp_conditional_result_n37;
            double hxc_tmp_load_result_n136 = hxc_dz_h0f76a758436d;
            double hxc_tmp_conditional_result_n39 = 0.0;
            if (hxc_tmp_load_result_n136 < 0.0)
            {
              hxc_tmp_conditional_result_n39 = -hxc_dz_h0f76a758436d;
            }
            else
            {
              hxc_tmp_conditional_result_n39 = hxc_dz_h0f76a758436d;
            }
            double hxc_tmp_conditional_load_result_n139 = hxc_tmp_conditional_result_n39;
            if (hxc_tmp_binary_left_n38 >= hxc_tmp_conditional_load_result_n139)
            {
              double hxc_tmp_compound_left_n40 = hxc_candidateX;
              double hxc_tmp_load_result_n142 = hxc_dx_hc84b106395d5;
              double hxc_tmp_conditional_result_n41 = 0.0;
              if (hxc_tmp_load_result_n142 > 0.08)
              {
                hxc_tmp_conditional_result_n41 = 0.08;
              }
              else
              {
                double hxc_tmp_load_result_n143 = hxc_dx_hc84b106395d5;
                double hxc_tmp_conditional_result_n42 = 0.0;
                if (hxc_tmp_load_result_n143 < -0.08)
                {
                  hxc_tmp_conditional_result_n42 = -0.08;
                }
                else
                {
                  hxc_tmp_conditional_result_n42 = hxc_dx_hc84b106395d5;
                }
                hxc_tmp_conditional_result_n41 = hxc_tmp_conditional_result_n42;
              }
              double hxc_tmp_conditional_load_result_n146 = hxc_tmp_conditional_result_n41;
              hxc_candidateX = hxc_tmp_compound_left_n40 + hxc_tmp_conditional_load_result_n146;
            }
            else
            {
              double hxc_tmp_compound_left_n43 = hxc_candidateZ;
              double hxc_tmp_load_result_n149 = hxc_dz_h0f76a758436d;
              double hxc_tmp_conditional_result_n44 = 0.0;
              if (hxc_tmp_load_result_n149 > 0.08)
              {
                hxc_tmp_conditional_result_n44 = 0.08;
              }
              else
              {
                double hxc_tmp_load_result_n150 = hxc_dz_h0f76a758436d;
                double hxc_tmp_conditional_result_n45 = 0.0;
                if (hxc_tmp_load_result_n150 < -0.08)
                {
                  hxc_tmp_conditional_result_n45 = -0.08;
                }
                else
                {
                  hxc_tmp_conditional_result_n45 = hxc_dz_h0f76a758436d;
                }
                hxc_tmp_conditional_result_n44 = hxc_tmp_conditional_result_n45;
              }
              double hxc_tmp_conditional_load_result_n153 = hxc_tmp_conditional_result_n44;
              hxc_candidateZ = hxc_tmp_compound_left_n43 + hxc_tmp_conditional_load_result_n153;
            }
            double hxc_tmp_load_result_n155 = hxc_candidateX;
            int32_t hxc_tmp_call_result_n157 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_length, hxc_f64_to_i32_saturating(hxc_tmp_load_result_n155), hxc_f64_to_i32_saturating(hxc_candidateZ));
            int32_t hxc_surface = hxc_tmp_call_result_n157;
            double hxc_candidateY = (double)hxc_surface + 1.0;
            if (!(hxc_surface < 0))
            {
              double hxc_value = hxc_candidateY - hxc_state.hxc_y;
              double hxc_tmp_load_result_n170 = hxc_value;
              double hxc_tmp_conditional_result_n49 = 0.0;
              if (hxc_tmp_load_result_n170 < 0.0)
              {
                hxc_tmp_conditional_result_n49 = -hxc_value;
              }
              else
              {
                hxc_tmp_conditional_result_n49 = hxc_value;
              }
              if (!(hxc_tmp_conditional_result_n49 > 1.0))
              {
                double hxc_tmp_load_result_n182 = hxc_candidateX;
                double hxc_tmp_load_result_n183 = hxc_candidateY;
                double hxc_tmp_load_result_n184 = hxc_candidateZ;
                return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_nextMode, .hxc_phaseTicks = 0, .hxc_x = hxc_tmp_load_result_n182, .hxc_y = hxc_tmp_load_result_n183, .hxc_z = hxc_tmp_load_result_n184 };
              }
              return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_nextMode, .hxc_phaseTicks = 0, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
            }
            return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_nextMode, .hxc_phaseTicks = 0, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
          }
          return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_nextMode, .hxc_phaseTicks = 0, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
        }
        return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = 4, .hxc_phaseTicks = 8, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
      }
      return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_currentMode, .hxc_phaseTicks = hxc_i32_subtract_wrapping(hxc_state.hxc_phaseTicks, 1), .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
    }
    if (!(hxc_playerDistanceSquared > 1.96))
    {
      if (!(hxc_state.hxc_phaseTicks > 1))
      {
        return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = 5, .hxc_phaseTicks = 12, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
      }
      return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_currentMode, .hxc_phaseTicks = hxc_i32_subtract_wrapping(hxc_state.hxc_phaseTicks, 1), .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
    }
    return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_state.hxc_health, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = 1, .hxc_phaseTicks = 0, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
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
      int32_t hxc_tmp_load_result_n6 = hxc_code;
      int32_t hxc_tmp_conditional_result_n6 = 0;
      if (hxc_tmp_load_result_n6 == 3)
      {
        hxc_tmp_conditional_result_n6 = 3;
      }
      else
      {
        int32_t hxc_tmp_load_result_n7 = hxc_code;
        int32_t hxc_tmp_conditional_result_n7 = 0;
        if (hxc_tmp_load_result_n7 == 4)
        {
          hxc_tmp_conditional_result_n7 = 4;
        }
        else
        {
          int32_t hxc_tmp_load_result_n8 = hxc_code;
          int32_t hxc_tmp_conditional_result_n8 = 0;
          if (hxc_tmp_load_result_n8 == 5)
          {
            hxc_tmp_conditional_result_n8 = 5;
          }
          else
          {
            hxc_tmp_conditional_result_n8 = 0;
          }
          hxc_tmp_conditional_result_n7 = hxc_tmp_conditional_result_n8;
        }
        hxc_tmp_conditional_result_n6 = hxc_tmp_conditional_result_n7;
      }
      hxc_tmp_conditional_result_n5 = hxc_tmp_conditional_result_n6;
    }
    hxc_tmp_conditional_result_n4 = hxc_tmp_conditional_result_n5;
  }
  int32_t hxc_currentMode = hxc_tmp_conditional_result_n4;
  int32_t hxc_tmp_load_result_n19 = hxc_currentMode;
  return (struct hxc_caxecraft_gameplay_MosslingState){ .hxc_health = hxc_nextHealth, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_modeCode = hxc_tmp_load_result_n19, .hxc_phaseTicks = hxc_state.hxc_phaseTicks, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_state.hxc_y, .hxc_z = hxc_state.hxc_z };
}

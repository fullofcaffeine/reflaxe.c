#include "hxc/program.h"

struct hxc_caxecraft_domain_AquaticState hxc_caxecraft_domain_Aquatics_adoptProfile(struct hxc_caxecraft_domain_AquaticState hxc_state, struct hxc_caxecraft_domain_AquaticProfile hxc_previous, struct hxc_caxecraft_domain_AquaticProfile hxc_replacement)
{
  int32_t hxc_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_state.hxc_breathTicks, 0, hxc_previous.hxc_maximumBreathTicks);
  int32_t hxc_previousBreath = hxc_tmp_call_result_n2;
  int32_t hxc_spentBreath = hxc_i32_subtract_wrapping(hxc_previous.hxc_maximumBreathTicks, hxc_previousBreath);
  int32_t hxc_tmp_call_result_n11 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_i32_subtract_wrapping(hxc_replacement.hxc_maximumBreathTicks, hxc_spentBreath), 0, hxc_replacement.hxc_maximumBreathTicks);
  int32_t hxc_tmp_call_result_n14 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_state.hxc_drowningTicks, 1, hxc_replacement.hxc_drowningIntervalTicks);
  return (struct hxc_caxecraft_domain_AquaticState){ .hxc_breathTicks = hxc_tmp_call_result_n11, .hxc_drowningTicks = hxc_tmp_call_result_n14, .hxc_headSubmerged = hxc_state.hxc_headSubmerged, .hxc_medium = hxc_state.hxc_medium, .hxc_submersion = hxc_state.hxc_submersion };
}

struct hxc_caxecraft_domain_Aquatics_BreathStep hxc_caxecraft_domain_Aquatics_advanceBreath(struct hxc_caxecraft_domain_AquaticState hxc_previous, struct hxc_caxecraft_domain_Immersion hxc_immersion, struct hxc_caxecraft_domain_AquaticProfile hxc_capability)
{
  int32_t hxc_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_previous.hxc_breathTicks, 0, hxc_capability.hxc_maximumBreathTicks);
  int32_t hxc_remaining = hxc_tmp_call_result_n2;
  int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_previous.hxc_drowningTicks, 1, hxc_capability.hxc_drowningIntervalTicks);
  int32_t hxc_drowningTicks = hxc_tmp_call_result_n5;
  int32_t hxc_damage = 0;
  if (hxc_immersion.hxc_headWet)
  {
    if (hxc_remaining > 0)
    {
      hxc_remaining = hxc_i32_subtract_wrapping(hxc_remaining, 1);
    }
    else
    {
      hxc_drowningTicks = hxc_i32_subtract_wrapping(hxc_drowningTicks, 1);
      if (hxc_drowningTicks <= 0)
      {
        hxc_damage = 1;
        hxc_drowningTicks = hxc_capability.hxc_drowningIntervalTicks;
      }
    }
  }
  else
  {
    int32_t hxc_tmp_call_result_n15 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_i32_add_wrapping(hxc_remaining, hxc_capability.hxc_breathRecoveryPerTick), 0, hxc_capability.hxc_maximumBreathTicks);
    hxc_remaining = hxc_tmp_call_result_n15;
    hxc_drowningTicks = hxc_capability.hxc_drowningIntervalTicks;
  }
  int32_t hxc_tmp_load_result_n17 = hxc_remaining;
  int32_t hxc_tmp_load_result_n18 = hxc_drowningTicks;
  return (struct hxc_caxecraft_domain_Aquatics_BreathStep){ .hxc_damage = hxc_damage, .hxc_drowningTicks = hxc_tmp_load_result_n18, .hxc_remaining = hxc_tmp_load_result_n17 };
}

double hxc_caxecraft_domain_Aquatics_approach(double hxc_current, double hxc_target, double hxc_maximumChange)
{
  if (!(hxc_current < hxc_target))
  {
    if (!(hxc_current > hxc_target))
    {
      return hxc_current;
    }
    double hxc_left_h538705580c87 = hxc_current - hxc_maximumChange;
    if (!(hxc_left_h538705580c87 > hxc_target))
    {
      return hxc_target;
    }
    return hxc_left_h538705580c87;
  }
  double hxc_left_hb76f33a33e23 = hxc_current + hxc_maximumChange;
  if (!(hxc_left_hb76f33a33e23 < hxc_target))
  {
    return hxc_target;
  }
  return hxc_left_hb76f33a33e23;
}

bool hxc_caxecraft_domain_Aquatics_canMine(struct hxc_caxecraft_domain_AquaticState hxc_state, struct hxc_caxecraft_domain_AquaticProfile hxc_capability)
{
  bool hxc_tmp_short_circuit_result_n2 = hxc_state.hxc_medium != 3;
  if (!(hxc_state.hxc_medium != 3))
  {
    hxc_tmp_short_circuit_result_n2 = hxc_capability.hxc_underwaterMining;
  }
  return hxc_tmp_short_circuit_result_n2;
}

double hxc_caxecraft_domain_Aquatics_clamp(double hxc_value, double hxc_minimum, double hxc_maximum)
{
  if (!(hxc_value < hxc_minimum))
  {
    if (!(hxc_value > hxc_maximum))
    {
      return hxc_value;
    }
    return hxc_maximum;
  }
  return hxc_minimum;
}

int32_t hxc_caxecraft_domain_Aquatics_clampInt(int32_t hxc_value, int32_t hxc_minimum, int32_t hxc_maximum)
{
  if (!(hxc_value < hxc_minimum))
  {
    if (!(hxc_value > hxc_maximum))
    {
      return hxc_value;
    }
    return hxc_maximum;
  }
  return hxc_minimum;
}

int32_t hxc_caxecraft_domain_Aquatics_classifyMedium(int32_t hxc_previous, struct hxc_caxecraft_domain_Immersion hxc_immersion)
{
  switch (hxc_previous) {
    case 0:
      {
        break;
      }
    case 1:
      {
        if (!(hxc_immersion.hxc_submersion <= 0.04))
        {
          if (!hxc_immersion.hxc_headWet)
          {
            if (!(hxc_immersion.hxc_submersion >= 0.68))
            {
              return 1;
            }
            return 2;
          }
          return 3;
        }
        return 0;
      }
    case 2:
      {
        if (!hxc_immersion.hxc_headWet)
        {
          if (!(hxc_immersion.hxc_submersion <= 0.52))
          {
            return 2;
          }
          return 1;
        }
        return 3;
      }
    case 3:
      {
        bool hxc_tmp_short_circuit_result_n2 = !hxc_immersion.hxc_headWet;
        if (!hxc_immersion.hxc_headWet)
        {
          hxc_tmp_short_circuit_result_n2 = hxc_immersion.hxc_submersion <= 0.74;
        }
        if (!hxc_tmp_short_circuit_result_n2)
        {
          return 3;
        }
        return 2;
      }
    default:
      {
        abort();
      }
  }
  if (!(hxc_immersion.hxc_submersion >= 0.08))
  {
    return 0;
  }
  return 1;
}

int32_t hxc_caxecraft_domain_Aquatics_floorToInt(double hxc_value)
{
  int32_t hxc_truncated = hxc_f64_to_i32_saturating(hxc_value);
  if (!(hxc_value < (double)hxc_truncated))
  {
    return hxc_truncated;
  }
  return hxc_i32_subtract_wrapping(hxc_truncated, 1);
}

struct hxc_caxecraft_domain_CharacterIntent hxc_caxecraft_domain_Aquatics_input(double hxc_moveX, double hxc_moveZ, bool hxc_ascend, bool hxc_descend)
{
  return (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = hxc_descend, .hxc_jump = hxc_ascend, .hxc_moveX = hxc_moveX, .hxc_moveZ = hxc_moveZ };
}

struct hxc_caxecraft_domain_Immersion hxc_caxecraft_domain_Aquatics_observe(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_CharacterBody hxc_body)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n2 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n2;
  double hxc_feetY = hxc_body.hxc_y;
  double hxc_headY = hxc_body.hxc_y + 1.80;
  double hxc_submergedHeight = 0.0;
  int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_feetY);
  int32_t hxc_cellY = hxc_tmp_call_result_n3;
  int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_headY - 0.0001);
  int32_t hxc_lastY = hxc_tmp_call_result_n5;
  while (1)
  {
    int32_t hxc_tmp_load_result_n6 = hxc_cellY;
    if (!(hxc_tmp_load_result_n6 <= hxc_lastY))
    {
      break;
    }
    double hxc_tmp_call_result_n11 = hxc_caxecraft_domain_Aquatics_waterSurface(hxc_cells, hxc_length, hxc_body.hxc_x, hxc_cellY, hxc_body.hxc_z);
    double hxc_top = hxc_tmp_call_result_n11;
    double hxc_tmp_load_result_n12 = hxc_top;
    if (hxc_tmp_load_result_n12 > (double)hxc_cellY)
    {
      double hxc_tmp_load_result_n14 = hxc_feetY;
      int32_t hxc_tmp_load_result_n15 = hxc_cellY;
      double hxc_tmp_conditional_result_n10 = 0.0;
      if (hxc_tmp_load_result_n14 > (double)hxc_tmp_load_result_n15)
      {
        hxc_tmp_conditional_result_n10 = hxc_feetY;
      }
      else
      {
        hxc_tmp_conditional_result_n10 = (double)hxc_cellY;
      }
      double hxc_overlapBottom = hxc_tmp_conditional_result_n10;
      double hxc_tmp_load_result_n19 = hxc_headY;
      double hxc_tmp_load_result_n20 = hxc_top;
      double hxc_tmp_conditional_result_n12 = 0.0;
      if (hxc_tmp_load_result_n19 < hxc_tmp_load_result_n20)
      {
        hxc_tmp_conditional_result_n12 = hxc_headY;
      }
      else
      {
        hxc_tmp_conditional_result_n12 = hxc_top;
      }
      double hxc_overlapTop = hxc_tmp_conditional_result_n12;
      double hxc_tmp_load_result_n24 = hxc_overlapTop;
      if (hxc_tmp_load_result_n24 > hxc_overlapBottom)
      {
        double hxc_tmp_compound_load_result_n26 = hxc_submergedHeight;
        double hxc_tmp_load_result_n27 = hxc_overlapTop;
        hxc_submergedHeight = hxc_tmp_compound_load_result_n26 + (hxc_tmp_load_result_n27 - hxc_overlapBottom);
      }
    }
    hxc_cellY = hxc_i32_add_wrapping(hxc_cellY, 1);
  }
  double hxc_eyeY = hxc_body.hxc_y + 1.62;
  int32_t hxc_tmp_call_result_n33 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_eyeY);
  double hxc_tmp_call_result_n35 = hxc_caxecraft_domain_Aquatics_waterSurface(hxc_cells, hxc_length, hxc_body.hxc_x, hxc_tmp_call_result_n33, hxc_body.hxc_z);
  double hxc_eyeSurface = hxc_tmp_call_result_n35;
  double hxc_tmp_load_result_n36 = hxc_eyeSurface;
  double hxc_cameraDepth = hxc_tmp_load_result_n36 - hxc_eyeY;
  double hxc_tmp_call_result_n39 = hxc_caxecraft_domain_Aquatics_clamp(hxc_f64_divide_zero_safe(hxc_submergedHeight, 1.80), 0.0, 1.0);
  bool hxc_tmp_call_result_n43 = hxc_caxecraft_domain_Aquatics_pointIsWet(hxc_cells, hxc_length, hxc_body.hxc_x, hxc_body.hxc_y + 0.08, hxc_body.hxc_z);
  bool hxc_tmp_call_result_n47 = hxc_caxecraft_domain_Aquatics_pointIsWet(hxc_cells, hxc_length, hxc_body.hxc_x, hxc_body.hxc_y + 0.90, hxc_body.hxc_z);
  bool hxc_tmp_call_result_n51 = hxc_caxecraft_domain_Aquatics_pointIsWet(hxc_cells, hxc_length, hxc_body.hxc_x, hxc_eyeY, hxc_body.hxc_z);
  double hxc_tmp_call_result_n53 = hxc_caxecraft_domain_Aquatics_clamp(hxc_f64_divide_zero_safe(hxc_cameraDepth + 0.08, 0.16), 0.0, 1.0);
  return (struct hxc_caxecraft_domain_Immersion){ .hxc_bodyWet = hxc_tmp_call_result_n47, .hxc_cameraBlend = hxc_tmp_call_result_n53, .hxc_feetWet = hxc_tmp_call_result_n43, .hxc_headWet = hxc_tmp_call_result_n51, .hxc_submersion = hxc_tmp_call_result_n39 };
}

bool hxc_caxecraft_domain_Aquatics_pointIsWet(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_y, double hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_y);
  int32_t hxc_cellY = hxc_tmp_call_result_n0;
  double hxc_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_waterSurface(hxc_cells, hxc_length, hxc_x, hxc_cellY, hxc_z);
  return hxc_tmp_call_result_n2 > hxc_y + 0.0001;
}

struct hxc_caxecraft_domain_AquaticProfile hxc_caxecraft_domain_Aquatics_profile(int32_t hxc_maximumBreathTicks, int32_t hxc_breathRecoveryPerTick, double hxc_horizontalControl, double hxc_ascentAcceleration, double hxc_descentAcceleration, double hxc_buoyancyAcceleration, double hxc_dragPerTick, int32_t hxc_drowningIntervalTicks, bool hxc_underwaterMining, bool hxc_coldProtection)
{
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_maximumBreathTicks, 1, 12000);
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_breathRecoveryPerTick, 1, 120);
  double hxc_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_clamp(hxc_horizontalControl, 0.0, 1.0);
  double hxc_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_clamp(hxc_ascentAcceleration, 0.0, 40.0);
  double hxc_tmp_call_result_n4 = hxc_caxecraft_domain_Aquatics_clamp(hxc_descentAcceleration, 0.0, 40.0);
  double hxc_tmp_call_result_n5 = hxc_caxecraft_domain_Aquatics_clamp(hxc_buoyancyAcceleration, 0.0, 30.0);
  double hxc_tmp_call_result_n6 = hxc_caxecraft_domain_Aquatics_clamp(hxc_dragPerTick, 0.0, 0.90);
  int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_drowningIntervalTicks, 1, 1200);
  return (struct hxc_caxecraft_domain_AquaticProfile){ .hxc_ascentAcceleration = hxc_tmp_call_result_n3, .hxc_breathRecoveryPerTick = hxc_tmp_call_result_n1, .hxc_buoyancyAcceleration = hxc_tmp_call_result_n5, .hxc_coldProtection = hxc_coldProtection, .hxc_descentAcceleration = hxc_tmp_call_result_n4, .hxc_dragPerTick = hxc_tmp_call_result_n6, .hxc_drowningIntervalTicks = hxc_tmp_call_result_n7, .hxc_horizontalControl = hxc_tmp_call_result_n2, .hxc_maximumBreathTicks = hxc_tmp_call_result_n0, .hxc_underwaterMining = hxc_underwaterMining };
}

struct hxc_caxecraft_domain_AquaticState hxc_caxecraft_domain_Aquatics_start(struct hxc_caxecraft_domain_AquaticProfile hxc_capability)
{
  return (struct hxc_caxecraft_domain_AquaticState){ .hxc_breathTicks = hxc_capability.hxc_maximumBreathTicks, .hxc_drowningTicks = hxc_capability.hxc_drowningIntervalTicks, .hxc_headSubmerged = false, .hxc_medium = 0, .hxc_submersion = 0.0 };
}

struct hxc_caxecraft_domain_AquaticStep hxc_caxecraft_domain_Aquatics_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_CharacterBody hxc_original, struct hxc_caxecraft_domain_AquaticState hxc_aquatic, struct hxc_caxecraft_domain_CharacterIntent hxc_command, struct hxc_caxecraft_domain_AquaticProfile hxc_capability)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n5 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n5;
  struct hxc_caxecraft_domain_Immersion hxc_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_observe(hxc_cells, hxc_length, hxc_original);
  struct hxc_caxecraft_domain_Immersion hxc_before = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_classifyMedium(hxc_aquatic.hxc_medium, hxc_before);
  int32_t hxc_beforeMedium = hxc_tmp_call_result_n3;
  struct hxc_caxecraft_domain_CharacterBody hxc_moved = hxc_original;
  if (hxc_beforeMedium == 0)
  {
    struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n8 = hxc_caxecraft_domain_CharacterPhysics_input(hxc_command.hxc_moveX, hxc_command.hxc_moveZ, hxc_command.hxc_jump);
    struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n9 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_cells, hxc_length, hxc_original, hxc_tmp_call_result_n8);
    hxc_moved = hxc_tmp_call_result_n9;
  }
  else
  {
    double hxc_wet = hxc_before.hxc_submersion;
    double hxc_speed = 2.0 + 2.0 * hxc_capability.hxc_horizontalControl;
    double hxc_acceleration = 4.0 + 8.0 * hxc_capability.hxc_horizontalControl;
    double hxc_tmp_load_result_n15 = hxc_speed;
    double hxc_tmp_call_result_n17 = hxc_caxecraft_domain_Aquatics_approach(hxc_original.hxc_velocityX, hxc_command.hxc_moveX * hxc_tmp_load_result_n15, hxc_acceleration * 0.05);
    double hxc_velocityX = hxc_tmp_call_result_n17;
    double hxc_tmp_load_result_n20 = hxc_speed;
    double hxc_tmp_call_result_n22 = hxc_caxecraft_domain_Aquatics_approach(hxc_original.hxc_velocityZ, hxc_command.hxc_moveZ * hxc_tmp_load_result_n20, hxc_acceleration * 0.05);
    double hxc_velocityZ = hxc_tmp_call_result_n22;
    double hxc_drag = 1.0 - hxc_capability.hxc_dragPerTick * hxc_wet;
    double hxc_tmp_compound_load_result_n25 = hxc_velocityX;
    hxc_velocityX = hxc_tmp_compound_load_result_n25 * hxc_drag;
    double hxc_tmp_compound_load_result_n27 = hxc_velocityZ;
    hxc_velocityZ = hxc_tmp_compound_load_result_n27 * hxc_drag;
    double hxc_tmp_load_result_n29 = hxc_wet;
    double hxc_verticalAcceleration = -18.0 * (1.0 - 0.85 * hxc_tmp_load_result_n29) + hxc_capability.hxc_buoyancyAcceleration * hxc_wet;
    if (hxc_command.hxc_jump)
    {
      double hxc_tmp_compound_load_result_n33 = hxc_verticalAcceleration;
      hxc_verticalAcceleration = hxc_tmp_compound_load_result_n33 + hxc_capability.hxc_ascentAcceleration * hxc_wet;
    }
    if (hxc_command.hxc_descend)
    {
      double hxc_tmp_compound_load_result_n37 = hxc_verticalAcceleration;
      hxc_verticalAcceleration = hxc_tmp_compound_load_result_n37 - hxc_capability.hxc_descentAcceleration * hxc_wet;
    }
    double hxc_tmp_load_result_n41 = hxc_verticalAcceleration;
    double hxc_velocityY = (hxc_original.hxc_velocityY + hxc_tmp_load_result_n41 * 0.05) * hxc_drag;
    double hxc_tmp_call_result_n44 = hxc_caxecraft_domain_Aquatics_clamp(hxc_velocityY, -4.5, 4.5);
    hxc_velocityY = hxc_tmp_call_result_n44;
    double hxc_tmp_load_result_n45 = hxc_velocityX;
    double hxc_tmp_load_result_n46 = hxc_velocityY;
    struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n48 = hxc_caxecraft_domain_CharacterPhysics_resolveVelocity(hxc_cells, hxc_length, hxc_original, hxc_tmp_load_result_n45, hxc_tmp_load_result_n46, hxc_velocityZ);
    hxc_moved = hxc_tmp_call_result_n48;
  }
  struct hxc_caxecraft_domain_Immersion hxc_tmp_call_result_n50 = hxc_caxecraft_domain_Aquatics_observe(hxc_cells, hxc_length, hxc_moved);
  struct hxc_caxecraft_domain_Immersion hxc_after = hxc_tmp_call_result_n50;
  int32_t hxc_tmp_load_result_n51 = hxc_beforeMedium;
  int32_t hxc_tmp_call_result_n53 = hxc_caxecraft_domain_Aquatics_classifyMedium(hxc_tmp_load_result_n51, hxc_after);
  int32_t hxc_medium = hxc_tmp_call_result_n53;
  struct hxc_caxecraft_domain_Aquatics_BreathStep hxc_tmp_call_result_n55 = hxc_caxecraft_domain_Aquatics_advanceBreath(hxc_aquatic, hxc_after, hxc_capability);
  struct hxc_caxecraft_domain_Aquatics_BreathStep hxc_breath = hxc_tmp_call_result_n55;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_load_result_n56 = hxc_moved;
  struct hxc_caxecraft_domain_Immersion hxc_tmp_load_result_n57 = hxc_after;
  int32_t hxc_tmp_load_result_n58 = hxc_medium;
  double hxc_tmp_record_field_load_result_n59 = hxc_after.hxc_submersion;
  bool hxc_tmp_record_field_load_result_n60 = hxc_after.hxc_headWet;
  int32_t hxc_tmp_record_field_load_result_n61 = hxc_breath.hxc_remaining;
  int32_t hxc_tmp_record_field_load_result_n62 = hxc_breath.hxc_drowningTicks;
  return (struct hxc_caxecraft_domain_AquaticStep){ .hxc_aquatic = (struct hxc_caxecraft_domain_AquaticState){ .hxc_breathTicks = hxc_tmp_record_field_load_result_n61, .hxc_drowningTicks = hxc_tmp_record_field_load_result_n62, .hxc_headSubmerged = hxc_tmp_record_field_load_result_n60, .hxc_medium = hxc_tmp_load_result_n58, .hxc_submersion = hxc_tmp_record_field_load_result_n59 }, .hxc_body = hxc_tmp_load_result_n56, .hxc_drowningDamage = hxc_breath.hxc_damage, .hxc_immersion = hxc_tmp_load_result_n57 };
}

double hxc_caxecraft_domain_Aquatics_waterSurface(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, int32_t hxc_y, double hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_x);
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_z);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n2 = hxc_caxecraft_domain_World_coord(hxc_tmp_call_result_n0, hxc_y, hxc_tmp_call_result_n1);
  struct hxc_caxecraft_domain_BlockCoord hxc_coord = hxc_tmp_call_result_n2;
  struct hxc_caxecraft_domain_WaterCellState hxc_tmp_call_result_n4 = hxc_caxecraft_domain_WaterCellCodec_stateAt(hxc_cells, hxc_length, hxc_coord);
  struct hxc_caxecraft_domain_WaterCellState hxc_g = hxc_tmp_call_result_n4;
  switch (hxc_g.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
    case hxc_caxecraft_domain_WaterCellState_Blocked:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        return (double)hxc_y + 1.0;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        if (hxc_g.hxc_tag != hxc_caxecraft_domain_WaterCellState_Flowing)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n7 = hxc_g.hxc_payload.hxc_Flowing.hxc_level;
        int32_t hxc_level = hxc_tmp_enum_payload_project_n7;
        if (hxc_g.hxc_tag != hxc_caxecraft_domain_WaterCellState_Flowing)
        {
          abort();
        }
        bool hxc_tmp_enum_payload_project_n9 = hxc_g.hxc_payload.hxc_Flowing.hxc_falling;
        bool hxc_falling = hxc_tmp_enum_payload_project_n9;
        if (!hxc_falling)
        {
          int32_t hxc_distance = hxc_level;
          return (double)hxc_y + hxc_f64_divide_zero_safe(8.0 - (double)hxc_distance, 8.0);
        }
        return (double)hxc_y + 1.0;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
      {
        return (double)hxc_y;
      }
  }
  return (double)hxc_y;
}

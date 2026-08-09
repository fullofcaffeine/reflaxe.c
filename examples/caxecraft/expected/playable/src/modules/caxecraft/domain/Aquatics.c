#include "hxc/program.h"

struct hxc_caxecraft_domain_AquaticState hxc_caxecraft_domain_Aquatics_adoptProfile(struct hxc_caxecraft_domain_AquaticState hxc_l_state, struct hxc_caxecraft_domain_AquaticProfile hxc_l_previous, struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement)
{
  int32_t hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_l_state.hxc_breathTicks, 0, hxc_l_previous.hxc_maximumBreathTicks);
  int32_t hxc_l_previousBreath = hxc_l_tmp_call_result_n2;
  int32_t hxc_l_spentBreath = hxc_i32_subtract_wrapping(hxc_l_previous.hxc_maximumBreathTicks, hxc_l_previousBreath);
  int32_t hxc_l_tmp_call_result_n11 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_i32_subtract_wrapping(hxc_l_replacement.hxc_maximumBreathTicks, hxc_l_spentBreath), 0, hxc_l_replacement.hxc_maximumBreathTicks);
  int32_t hxc_l_tmp_call_result_n14 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_l_state.hxc_drowningTicks, 1, hxc_l_replacement.hxc_drowningIntervalTicks);
  return (struct hxc_caxecraft_domain_AquaticState){ .hxc_breathTicks = hxc_l_tmp_call_result_n11, .hxc_drowningTicks = hxc_l_tmp_call_result_n14, .hxc_headSubmerged = hxc_l_state.hxc_headSubmerged, .hxc_medium = hxc_l_state.hxc_medium, .hxc_submersion = hxc_l_state.hxc_submersion };
}

struct hxc_caxecraft_domain_Aquatics_BreathStep hxc_caxecraft_domain_Aquatics_advanceBreath(struct hxc_caxecraft_domain_AquaticState hxc_l_previous, struct hxc_caxecraft_domain_Immersion hxc_l_immersion, struct hxc_caxecraft_domain_AquaticProfile hxc_l_capability)
{
  int32_t hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_l_previous.hxc_breathTicks, 0, hxc_l_capability.hxc_maximumBreathTicks);
  int32_t hxc_l_remaining = hxc_l_tmp_call_result_n2;
  int32_t hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_l_previous.hxc_drowningTicks, 1, hxc_l_capability.hxc_drowningIntervalTicks);
  int32_t hxc_l_drowningTicks = hxc_l_tmp_call_result_n5;
  int32_t hxc_l_damage = 0;
  if (hxc_l_immersion.hxc_headWet)
  {
    if (hxc_l_remaining > 0)
    {
      hxc_l_remaining = hxc_i32_subtract_wrapping(hxc_l_remaining, 1);
    }
    else
    {
      hxc_l_drowningTicks = hxc_i32_subtract_wrapping(hxc_l_drowningTicks, 1);
      if (hxc_l_drowningTicks <= 0)
      {
        hxc_l_damage = 1;
        hxc_l_drowningTicks = hxc_l_capability.hxc_drowningIntervalTicks;
      }
    }
  }
  else
  {
    int32_t hxc_l_tmp_call_result_n15 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_i32_add_wrapping(hxc_l_remaining, hxc_l_capability.hxc_breathRecoveryPerTick), 0, hxc_l_capability.hxc_maximumBreathTicks);
    hxc_l_remaining = hxc_l_tmp_call_result_n15;
    hxc_l_drowningTicks = hxc_l_capability.hxc_drowningIntervalTicks;
  }
  int32_t hxc_l_tmp_load_result_n17 = hxc_l_remaining;
  int32_t hxc_l_tmp_load_result_n18 = hxc_l_drowningTicks;
  return (struct hxc_caxecraft_domain_Aquatics_BreathStep){ .hxc_damage = hxc_l_damage, .hxc_drowningTicks = hxc_l_tmp_load_result_n18, .hxc_remaining = hxc_l_tmp_load_result_n17 };
}

double hxc_caxecraft_domain_Aquatics_approach(double hxc_l_current, double hxc_l_target, double hxc_l_maximumChange)
{
  if (!(hxc_l_current < hxc_l_target))
  {
    if (!(hxc_l_current > hxc_l_target))
    {
      return hxc_l_current;
    }
    double hxc_l_left_h538705580c87 = hxc_l_current - hxc_l_maximumChange;
    if (!(hxc_l_left_h538705580c87 > hxc_l_target))
    {
      return hxc_l_target;
    }
    return hxc_l_left_h538705580c87;
  }
  double hxc_l_left_hb76f33a33e23 = hxc_l_current + hxc_l_maximumChange;
  if (!(hxc_l_left_hb76f33a33e23 < hxc_l_target))
  {
    return hxc_l_target;
  }
  return hxc_l_left_hb76f33a33e23;
}

bool hxc_caxecraft_domain_Aquatics_canMine(struct hxc_caxecraft_domain_AquaticState hxc_l_state, struct hxc_caxecraft_domain_AquaticProfile hxc_l_capability)
{
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_state.hxc_medium != 3;
  if (!(hxc_l_state.hxc_medium != 3))
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_capability.hxc_underwaterMining;
  }
  return hxc_l_tmp_short_circuit_result_n2;
}

double hxc_caxecraft_domain_Aquatics_clamp(double hxc_l_value, double hxc_l_minimum, double hxc_l_maximum)
{
  if (hxc_l_value < hxc_l_minimum)
  {
    return hxc_l_minimum;
  }
  if (!(hxc_l_value > hxc_l_maximum))
  {
    return hxc_l_value;
  }
  return hxc_l_maximum;
}

int32_t hxc_caxecraft_domain_Aquatics_clampInt(int32_t hxc_l_value, int32_t hxc_l_minimum, int32_t hxc_l_maximum)
{
  if (hxc_l_value < hxc_l_minimum)
  {
    return hxc_l_minimum;
  }
  if (!(hxc_l_value > hxc_l_maximum))
  {
    return hxc_l_value;
  }
  return hxc_l_maximum;
}

int32_t hxc_caxecraft_domain_Aquatics_classifyMedium(int32_t hxc_l_previous, struct hxc_caxecraft_domain_Immersion hxc_l_immersion)
{
  switch (hxc_l_previous) {
    case 0:
      {
        break;
      }
    case 1:
      {
        if (hxc_l_immersion.hxc_submersion <= 0.04)
        {
          return 0;
        }
        if (hxc_l_immersion.hxc_headWet)
        {
          return 3;
        }
        if (!(hxc_l_immersion.hxc_submersion >= 0.68))
        {
          return 1;
        }
        return 2;
      }
    case 2:
      {
        if (hxc_l_immersion.hxc_headWet)
        {
          return 3;
        }
        if (!(hxc_l_immersion.hxc_submersion <= 0.52))
        {
          return 2;
        }
        return 1;
      }
    case 3:
      {
        bool hxc_l_tmp_short_circuit_result_n2 = !hxc_l_immersion.hxc_headWet;
        if (!hxc_l_immersion.hxc_headWet)
        {
          hxc_l_tmp_short_circuit_result_n2 = hxc_l_immersion.hxc_submersion <= 0.74;
        }
        if (!hxc_l_tmp_short_circuit_result_n2)
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
  if (!(hxc_l_immersion.hxc_submersion >= 0.08))
  {
    return 0;
  }
  return 1;
}

int32_t hxc_caxecraft_domain_Aquatics_floorToInt(double hxc_l_value)
{
  int32_t hxc_l_truncated = hxc_f64_to_i32_saturating(hxc_l_value);
  if (!(hxc_l_value < (double)hxc_l_truncated))
  {
    return hxc_l_truncated;
  }
  return hxc_i32_subtract_wrapping(hxc_l_truncated, 1);
}

struct hxc_caxecraft_domain_CharacterIntent hxc_caxecraft_domain_Aquatics_input(double hxc_l_moveX, double hxc_l_moveZ, bool hxc_l_ascend, bool hxc_l_descend)
{
  return (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = hxc_l_descend, .hxc_jump = hxc_l_ascend, .hxc_moveX = hxc_l_moveX, .hxc_moveZ = hxc_l_moveZ };
}

struct hxc_caxecraft_domain_Immersion hxc_caxecraft_domain_Aquatics_observe(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_CharacterBody hxc_l_body)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  double hxc_l_feetY = hxc_l_body.hxc_y;
  double hxc_l_headY = hxc_l_body.hxc_y + 1.80;
  double hxc_l_submergedHeight = 0.0;
  int32_t hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_l_feetY);
  int32_t hxc_l_cellY = hxc_l_tmp_call_result_n3;
  int32_t hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_l_headY - 0.0001);
  int32_t hxc_l_lastY = hxc_l_tmp_call_result_n5;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_cellY;
    if (!(hxc_l_tmp_load_result_n6 <= hxc_l_lastY))
    {
      break;
    }
    double hxc_l_tmp_call_result_n11 = hxc_caxecraft_domain_Aquatics_waterSurface(hxc_l_cells, hxc_l_length, hxc_l_body.hxc_x, hxc_l_cellY, hxc_l_body.hxc_z);
    double hxc_l_top = hxc_l_tmp_call_result_n11;
    double hxc_l_tmp_load_result_n12 = hxc_l_top;
    if (hxc_l_tmp_load_result_n12 > (double)hxc_l_cellY)
    {
      double hxc_l_tmp_load_result_n14 = hxc_l_feetY;
      int32_t hxc_l_tmp_load_result_n15 = hxc_l_cellY;
      double hxc_l_tmp_conditional_result_n10 = 0.0;
      if (hxc_l_tmp_load_result_n14 > (double)hxc_l_tmp_load_result_n15)
      {
        hxc_l_tmp_conditional_result_n10 = hxc_l_feetY;
      }
      else
      {
        hxc_l_tmp_conditional_result_n10 = (double)hxc_l_cellY;
      }
      double hxc_l_overlapBottom = hxc_l_tmp_conditional_result_n10;
      double hxc_l_tmp_load_result_n19 = hxc_l_headY;
      double hxc_l_tmp_load_result_n20 = hxc_l_top;
      double hxc_l_tmp_conditional_result_n12 = 0.0;
      if (hxc_l_tmp_load_result_n19 < hxc_l_tmp_load_result_n20)
      {
        hxc_l_tmp_conditional_result_n12 = hxc_l_headY;
      }
      else
      {
        hxc_l_tmp_conditional_result_n12 = hxc_l_top;
      }
      double hxc_l_overlapTop = hxc_l_tmp_conditional_result_n12;
      double hxc_l_tmp_load_result_n24 = hxc_l_overlapTop;
      if (hxc_l_tmp_load_result_n24 > hxc_l_overlapBottom)
      {
        double hxc_l_tmp_compound_load_result_n26 = hxc_l_submergedHeight;
        double hxc_l_tmp_load_result_n27 = hxc_l_overlapTop;
        hxc_l_submergedHeight = hxc_l_tmp_compound_load_result_n26 + (hxc_l_tmp_load_result_n27 - hxc_l_overlapBottom);
      }
    }
    hxc_l_cellY = hxc_i32_add_wrapping(hxc_l_cellY, 1);
  }
  double hxc_l_eyeY = hxc_l_body.hxc_y + 1.62;
  int32_t hxc_l_tmp_call_result_n33 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_l_eyeY);
  double hxc_l_tmp_call_result_n35 = hxc_caxecraft_domain_Aquatics_waterSurface(hxc_l_cells, hxc_l_length, hxc_l_body.hxc_x, hxc_l_tmp_call_result_n33, hxc_l_body.hxc_z);
  double hxc_l_eyeSurface = hxc_l_tmp_call_result_n35;
  double hxc_l_tmp_load_result_n36 = hxc_l_eyeSurface;
  double hxc_l_cameraDepth = hxc_l_tmp_load_result_n36 - hxc_l_eyeY;
  double hxc_l_tmp_call_result_n39 = hxc_caxecraft_domain_Aquatics_clamp(hxc_f64_divide_zero_safe(hxc_l_submergedHeight, 1.80), 0.0, 1.0);
  bool hxc_l_tmp_call_result_n43 = hxc_caxecraft_domain_Aquatics_pointIsWet(hxc_l_cells, hxc_l_length, hxc_l_body.hxc_x, hxc_l_body.hxc_y + 0.08, hxc_l_body.hxc_z);
  bool hxc_l_tmp_call_result_n47 = hxc_caxecraft_domain_Aquatics_pointIsWet(hxc_l_cells, hxc_l_length, hxc_l_body.hxc_x, hxc_l_body.hxc_y + 0.90, hxc_l_body.hxc_z);
  bool hxc_l_tmp_call_result_n51 = hxc_caxecraft_domain_Aquatics_pointIsWet(hxc_l_cells, hxc_l_length, hxc_l_body.hxc_x, hxc_l_eyeY, hxc_l_body.hxc_z);
  double hxc_l_tmp_call_result_n53 = hxc_caxecraft_domain_Aquatics_clamp(hxc_f64_divide_zero_safe(hxc_l_cameraDepth + 0.08, 0.16), 0.0, 1.0);
  return (struct hxc_caxecraft_domain_Immersion){ .hxc_bodyWet = hxc_l_tmp_call_result_n47, .hxc_cameraBlend = hxc_l_tmp_call_result_n53, .hxc_feetWet = hxc_l_tmp_call_result_n43, .hxc_headWet = hxc_l_tmp_call_result_n51, .hxc_submersion = hxc_l_tmp_call_result_n39 };
}

bool hxc_caxecraft_domain_Aquatics_pointIsWet(const uint8_t *hxc_l_cells, size_t hxc_l_length, double hxc_l_x, double hxc_l_y, double hxc_l_z)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n4 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n4;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_l_y);
  int32_t hxc_l_cellY = hxc_l_tmp_call_result_n0;
  double hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_waterSurface(hxc_l_cells, hxc_l_length, hxc_l_x, hxc_l_cellY, hxc_l_z);
  return hxc_l_tmp_call_result_n2 > hxc_l_y + 0.0001;
}

struct hxc_caxecraft_domain_AquaticProfile hxc_caxecraft_domain_Aquatics_profile(int32_t hxc_l_maximumBreathTicks, int32_t hxc_l_breathRecoveryPerTick, double hxc_l_horizontalControl, double hxc_l_ascentAcceleration, double hxc_l_descentAcceleration, double hxc_l_buoyancyAcceleration, double hxc_l_dragPerTick, int32_t hxc_l_drowningIntervalTicks, bool hxc_l_underwaterMining, bool hxc_l_coldProtection)
{
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_l_maximumBreathTicks, 1, 12000);
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_l_breathRecoveryPerTick, 1, 120);
  double hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_clamp(hxc_l_horizontalControl, 0.0, 1.0);
  double hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_clamp(hxc_l_ascentAcceleration, 0.0, 40.0);
  double hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_Aquatics_clamp(hxc_l_descentAcceleration, 0.0, 40.0);
  double hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_Aquatics_clamp(hxc_l_buoyancyAcceleration, 0.0, 30.0);
  double hxc_l_tmp_call_result_n6 = hxc_caxecraft_domain_Aquatics_clamp(hxc_l_dragPerTick, 0.0, 0.90);
  int32_t hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_Aquatics_clampInt(hxc_l_drowningIntervalTicks, 1, 1200);
  return (struct hxc_caxecraft_domain_AquaticProfile){ .hxc_ascentAcceleration = hxc_l_tmp_call_result_n3, .hxc_breathRecoveryPerTick = hxc_l_tmp_call_result_n1, .hxc_buoyancyAcceleration = hxc_l_tmp_call_result_n5, .hxc_coldProtection = hxc_l_coldProtection, .hxc_descentAcceleration = hxc_l_tmp_call_result_n4, .hxc_dragPerTick = hxc_l_tmp_call_result_n6, .hxc_drowningIntervalTicks = hxc_l_tmp_call_result_n7, .hxc_horizontalControl = hxc_l_tmp_call_result_n2, .hxc_maximumBreathTicks = hxc_l_tmp_call_result_n0, .hxc_underwaterMining = hxc_l_underwaterMining };
}

struct hxc_caxecraft_domain_AquaticState hxc_caxecraft_domain_Aquatics_start(struct hxc_caxecraft_domain_AquaticProfile hxc_l_capability)
{
  return (struct hxc_caxecraft_domain_AquaticState){ .hxc_breathTicks = hxc_l_capability.hxc_maximumBreathTicks, .hxc_drowningTicks = hxc_l_capability.hxc_drowningIntervalTicks, .hxc_headSubmerged = false, .hxc_medium = 0, .hxc_submersion = 0.0 };
}

struct hxc_caxecraft_domain_AquaticStep hxc_caxecraft_domain_Aquatics_stepWithCollisions(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_array_ref *hxc_l_collisions, struct hxc_caxecraft_domain_CharacterBody hxc_l_original, struct hxc_caxecraft_domain_AquaticState hxc_l_aquatic, struct hxc_caxecraft_domain_CharacterIntent hxc_l_command, struct hxc_caxecraft_domain_AquaticProfile hxc_l_capability)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n6 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n6;
  struct hxc_caxecraft_domain_Immersion hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_observe(hxc_l_cells, hxc_l_length, hxc_l_original);
  struct hxc_caxecraft_domain_Immersion hxc_l_before = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_classifyMedium(hxc_l_aquatic.hxc_medium, hxc_l_before);
  int32_t hxc_l_beforeMedium = hxc_l_tmp_call_result_n3;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_moved = hxc_l_original;
  if (hxc_l_beforeMedium == 0)
  {
    struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n8 = hxc_caxecraft_domain_CharacterPhysics_input(hxc_l_command.hxc_moveX, hxc_l_command.hxc_moveZ, hxc_l_command.hxc_jump);
    struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n9 = hxc_caxecraft_domain_CharacterPhysics_stepWithCollisions(hxc_l_cells, hxc_l_length, hxc_l_collisions, hxc_l_original, hxc_l_tmp_call_result_n8);
    hxc_l_moved = hxc_l_tmp_call_result_n9;
  }
  else
  {
    double hxc_l_wet = hxc_l_before.hxc_submersion;
    double hxc_l_speed = 2.0 + 2.0 * hxc_l_capability.hxc_horizontalControl;
    double hxc_l_acceleration = 4.0 + 8.0 * hxc_l_capability.hxc_horizontalControl;
    double hxc_l_tmp_load_result_n15 = hxc_l_speed;
    double hxc_l_tmp_call_result_n17 = hxc_caxecraft_domain_Aquatics_approach(hxc_l_original.hxc_velocityX, hxc_l_command.hxc_moveX * hxc_l_tmp_load_result_n15, hxc_l_acceleration * 0.05);
    double hxc_l_velocityX = hxc_l_tmp_call_result_n17;
    double hxc_l_tmp_load_result_n20 = hxc_l_speed;
    double hxc_l_tmp_call_result_n22 = hxc_caxecraft_domain_Aquatics_approach(hxc_l_original.hxc_velocityZ, hxc_l_command.hxc_moveZ * hxc_l_tmp_load_result_n20, hxc_l_acceleration * 0.05);
    double hxc_l_velocityZ = hxc_l_tmp_call_result_n22;
    double hxc_l_drag = 1.0 - hxc_l_capability.hxc_dragPerTick * hxc_l_wet;
    double hxc_l_tmp_compound_load_result_n25 = hxc_l_velocityX;
    hxc_l_velocityX = hxc_l_tmp_compound_load_result_n25 * hxc_l_drag;
    double hxc_l_tmp_compound_load_result_n27 = hxc_l_velocityZ;
    hxc_l_velocityZ = hxc_l_tmp_compound_load_result_n27 * hxc_l_drag;
    double hxc_l_tmp_load_result_n29 = hxc_l_wet;
    double hxc_l_verticalAcceleration = -18.0 * (1.0 - 0.85 * hxc_l_tmp_load_result_n29) + hxc_l_capability.hxc_buoyancyAcceleration * hxc_l_wet;
    if (hxc_l_command.hxc_jump)
    {
      double hxc_l_tmp_compound_load_result_n33 = hxc_l_verticalAcceleration;
      hxc_l_verticalAcceleration = hxc_l_tmp_compound_load_result_n33 + hxc_l_capability.hxc_ascentAcceleration * hxc_l_wet;
    }
    if (hxc_l_command.hxc_descend)
    {
      double hxc_l_tmp_compound_load_result_n37 = hxc_l_verticalAcceleration;
      hxc_l_verticalAcceleration = hxc_l_tmp_compound_load_result_n37 - hxc_l_capability.hxc_descentAcceleration * hxc_l_wet;
    }
    double hxc_l_tmp_load_result_n41 = hxc_l_verticalAcceleration;
    double hxc_l_velocityY = (hxc_l_original.hxc_velocityY + hxc_l_tmp_load_result_n41 * 0.05) * hxc_l_drag;
    double hxc_l_tmp_call_result_n44 = hxc_caxecraft_domain_Aquatics_clamp(hxc_l_velocityY, -4.5, 4.5);
    hxc_l_velocityY = hxc_l_tmp_call_result_n44;
    double hxc_l_tmp_load_result_n45 = hxc_l_velocityX;
    double hxc_l_tmp_load_result_n46 = hxc_l_velocityY;
    struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n48 = hxc_caxecraft_domain_CharacterPhysics_resolveVelocityWithCollisions(hxc_l_cells, hxc_l_length, hxc_l_collisions, hxc_l_original, hxc_l_tmp_load_result_n45, hxc_l_tmp_load_result_n46, hxc_l_velocityZ);
    hxc_l_moved = hxc_l_tmp_call_result_n48;
    bool hxc_l_tmp_short_circuit_result_n19 = hxc_l_command.hxc_moveX != 0.0;
    if (hxc_l_command.hxc_moveX != 0.0)
    {
      hxc_l_tmp_short_circuit_result_n19 = hxc_l_moved.hxc_velocityX == 0.0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n51 = hxc_l_tmp_short_circuit_result_n19;
    bool hxc_l_tmp_short_circuit_result_n20 = hxc_l_tmp_short_circuit_load_result_n51;
    if (!hxc_l_tmp_short_circuit_load_result_n51)
    {
      bool hxc_l_tmp_short_circuit_result_n21 = hxc_l_command.hxc_moveZ != 0.0;
      if (hxc_l_command.hxc_moveZ != 0.0)
      {
        hxc_l_tmp_short_circuit_result_n21 = hxc_l_moved.hxc_velocityZ == 0.0;
      }
      hxc_l_tmp_short_circuit_result_n20 = hxc_l_tmp_short_circuit_result_n21;
    }
    bool hxc_l_horizontalBlocked = hxc_l_tmp_short_circuit_result_n20;
    bool hxc_l_tmp_record_field_project_n56 = hxc_l_command.hxc_jump;
    bool hxc_l_tmp_short_circuit_result_n22 = hxc_l_tmp_record_field_project_n56;
    if (hxc_l_tmp_record_field_project_n56)
    {
      hxc_l_tmp_short_circuit_result_n22 = hxc_l_horizontalBlocked;
    }
    if (hxc_l_tmp_short_circuit_result_n22)
    {
      double hxc_l_tmp_load_result_n59 = hxc_l_velocityX;
      struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n61 = hxc_caxecraft_domain_CharacterPhysics_tryStepUpWithCollisions(hxc_l_cells, hxc_l_length, hxc_l_collisions, hxc_l_original, hxc_l_tmp_load_result_n59, hxc_l_velocityZ);
      struct hxc_caxecraft_domain_CharacterBody hxc_l_stepped = hxc_l_tmp_call_result_n61;
      double hxc_l_tmp_record_field_load_result_n62 = hxc_l_stepped.hxc_x;
      bool hxc_l_tmp_short_circuit_result_n24 = hxc_l_tmp_record_field_load_result_n62 != hxc_l_original.hxc_x;
      if (!(hxc_l_tmp_record_field_load_result_n62 != hxc_l_original.hxc_x))
      {
        hxc_l_tmp_short_circuit_result_n24 = hxc_l_stepped.hxc_y != hxc_l_original.hxc_y;
      }
      bool hxc_l_tmp_short_circuit_load_result_n66 = hxc_l_tmp_short_circuit_result_n24;
      bool hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_short_circuit_load_result_n66;
      if (!hxc_l_tmp_short_circuit_load_result_n66)
      {
        hxc_l_tmp_short_circuit_result_n25 = hxc_l_stepped.hxc_z != hxc_l_original.hxc_z;
      }
      if (hxc_l_tmp_short_circuit_result_n25)
      {
        hxc_l_moved = hxc_l_stepped;
      }
    }
  }
  struct hxc_caxecraft_domain_Immersion hxc_l_tmp_call_result_n72 = hxc_caxecraft_domain_Aquatics_observe(hxc_l_cells, hxc_l_length, hxc_l_moved);
  struct hxc_caxecraft_domain_Immersion hxc_l_after = hxc_l_tmp_call_result_n72;
  int32_t hxc_l_tmp_load_result_n73 = hxc_l_beforeMedium;
  int32_t hxc_l_tmp_call_result_n75 = hxc_caxecraft_domain_Aquatics_classifyMedium(hxc_l_tmp_load_result_n73, hxc_l_after);
  int32_t hxc_l_medium = hxc_l_tmp_call_result_n75;
  struct hxc_caxecraft_domain_Aquatics_BreathStep hxc_l_tmp_call_result_n77 = hxc_caxecraft_domain_Aquatics_advanceBreath(hxc_l_aquatic, hxc_l_after, hxc_l_capability);
  struct hxc_caxecraft_domain_Aquatics_BreathStep hxc_l_breath = hxc_l_tmp_call_result_n77;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n78 = hxc_l_moved;
  struct hxc_caxecraft_domain_Immersion hxc_l_tmp_load_result_n79 = hxc_l_after;
  int32_t hxc_l_tmp_load_result_n80 = hxc_l_medium;
  double hxc_l_tmp_record_field_load_result_n81 = hxc_l_after.hxc_submersion;
  bool hxc_l_tmp_record_field_load_result_n82 = hxc_l_after.hxc_headWet;
  int32_t hxc_l_tmp_record_field_load_result_n83 = hxc_l_breath.hxc_remaining;
  int32_t hxc_l_tmp_record_field_load_result_n84 = hxc_l_breath.hxc_drowningTicks;
  return (struct hxc_caxecraft_domain_AquaticStep){ .hxc_aquatic = (struct hxc_caxecraft_domain_AquaticState){ .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n83, .hxc_drowningTicks = hxc_l_tmp_record_field_load_result_n84, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n82, .hxc_medium = hxc_l_tmp_load_result_n80, .hxc_submersion = hxc_l_tmp_record_field_load_result_n81 }, .hxc_body = hxc_l_tmp_load_result_n78, .hxc_drowningDamage = hxc_l_breath.hxc_damage, .hxc_immersion = hxc_l_tmp_load_result_n79 };
}

double hxc_caxecraft_domain_Aquatics_waterSurface(const uint8_t *hxc_l_cells, size_t hxc_l_length, double hxc_l_x, int32_t hxc_l_y, double hxc_l_z)
{
  int32_t hxc_l_distance = { 0 };
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n4 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n4;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_l_x);
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_Aquatics_floorToInt(hxc_l_z);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_call_result_n0, hxc_l_y, hxc_l_tmp_call_result_n1);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord = hxc_l_tmp_call_result_n2;
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_coord);
  struct hxc_caxecraft_domain_WaterCellState hxc_l_g = hxc_l_tmp_call_result_n4;
  switch (hxc_l_g.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
    case hxc_caxecraft_domain_WaterCellState_Blocked:
      {
        return (double)hxc_l_y;
      }
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        return (double)hxc_l_y + 1.0;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
      {
        return (double)hxc_l_y;
      }
    default:
      {
        abort();
      }
  }
  if (hxc_l_g.hxc_tag != hxc_caxecraft_domain_WaterCellState_Flowing)
  {
    abort();
  }
  int32_t hxc_l_tmp_enum_payload_project_n7 = hxc_l_g.hxc_payload.hxc_Flowing.hxc_level;
  int32_t hxc_l_level = hxc_l_tmp_enum_payload_project_n7;
  if (hxc_l_g.hxc_tag != hxc_caxecraft_domain_WaterCellState_Flowing)
  {
    abort();
  }
  bool hxc_l_tmp_enum_payload_project_n9 = hxc_l_g.hxc_payload.hxc_Flowing.hxc_falling;
  bool hxc_l_falling = hxc_l_tmp_enum_payload_project_n9;
  if (!hxc_l_falling)
  {
    hxc_l_distance = hxc_l_level;
    return (double)hxc_l_y + hxc_f64_divide_zero_safe(8.0 - (double)hxc_l_distance, 8.0);
  }
  return (double)hxc_l_y + 1.0;
}

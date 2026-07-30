#include "hxc/program.h"

bool hxc_caxecraft_domain_ActorControllerScheduler_interactionAvailable(struct hxc_caxecraft_domain_ActorControllerState hxc_state, struct hxc_caxecraft_domain_Character hxc_character, struct hxc_caxecraft_domain_Character hxc_localPlayer)
{
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_g = { 0 };
  bool hxc_tmp_short_circuit_result_n3 = hxc_character.hxc_id <= 0;
  if (!(hxc_character.hxc_id <= 0))
  {
    hxc_tmp_short_circuit_result_n3 = hxc_localPlayer.hxc_id <= 0;
  }
  bool hxc_tmp_short_circuit_load_result_n2 = hxc_tmp_short_circuit_result_n3;
  bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_short_circuit_load_result_n2;
  if (!hxc_tmp_short_circuit_load_result_n2)
  {
    hxc_tmp_short_circuit_result_n4 = hxc_state.hxc_characterId != hxc_character.hxc_id;
  }
  if (hxc_tmp_short_circuit_result_n4)
  {
    return false;
  }
  hxc_g = hxc_state.hxc_profile;
  if (hxc_enum_25b991ce_retain(&hxc_g) != HXC_STATUS_OK)
  {
    abort();
  }
  switch (hxc_g.hxc_tag) {
    case hxc_caxecraft_domain_ActorControllerProfile_StationaryDialogue:
      {
        break;
      }
    case hxc_caxecraft_domain_ActorControllerProfile_WanderChaseMelee:
      {
        hxc_enum_25b991ce_destroy(&hxc_g);
        return false;
      }
  }
  if (hxc_g.hxc_tag != hxc_caxecraft_domain_ActorControllerProfile_StationaryDialogue)
  {
    abort();
  }
  int32_t hxc_tmp_enum_payload_project_n9 = hxc_g.hxc_payload.hxc_StationaryDialogue.hxc_interactionRadiusMilli;
  int32_t hxc_interactionRadiusMilli = hxc_tmp_enum_payload_project_n9;
  bool hxc_tmp_short_circuit_result_n7 = hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Stationary;
  if (hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Stationary)
  {
    hxc_tmp_short_circuit_result_n7 = hxc_state.hxc_phaseTicks == 0;
  }
  bool hxc_tmp_short_circuit_load_result_n13 = hxc_tmp_short_circuit_result_n7;
  bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_short_circuit_load_result_n13;
  if (hxc_tmp_short_circuit_load_result_n13)
  {
    hxc_tmp_short_circuit_result_n8 = !hxc_state.hxc_dropPublished;
  }
  if (!hxc_tmp_short_circuit_result_n8)
  {
    hxc_enum_25b991ce_destroy(&hxc_g);
    return false;
  }
  double hxc_dx = (hxc_localPlayer.hxc_body.hxc_x - hxc_character.hxc_body.hxc_x) * 1000.0;
  double hxc_dz = (hxc_localPlayer.hxc_body.hxc_z - hxc_character.hxc_body.hxc_z) * 1000.0;
  double hxc_tmp_load_result_n24 = hxc_dx;
  double hxc_tmp_load_result_n25 = hxc_dx;
  double hxc_tmp_load_result_n26 = hxc_dz;
  double hxc_tmp_load_result_n27 = hxc_dz;
  int32_t hxc_tmp_load_result_n28 = hxc_interactionRadiusMilli;
  int32_t hxc_tmp_load_result_n29 = hxc_interactionRadiusMilli;
  hxc_enum_25b991ce_destroy(&hxc_g);
  return hxc_tmp_load_result_n24 * hxc_tmp_load_result_n25 + hxc_tmp_load_result_n26 * hxc_tmp_load_result_n27 <= (double)hxc_tmp_load_result_n28 * 1.0 * (double)hxc_tmp_load_result_n29;
}

struct hxc_caxecraft_domain_ActorControllerDecision hxc_caxecraft_domain_ActorControllerScheduler_planActorController(struct hxc_caxecraft_domain_ActorControllerState hxc_state, struct hxc_caxecraft_domain_Character hxc_character, struct hxc_caxecraft_domain_Character hxc_localPlayer, int32_t hxc_tickNumber)
{
  struct hxc_caxecraft_domain_WanderChaseMeleeProfile hxc_profile = { 0 };
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_g = { 0 };
  bool hxc_tmp_short_circuit_result_n4 = hxc_character.hxc_id <= 0;
  if (!(hxc_character.hxc_id <= 0))
  {
    hxc_tmp_short_circuit_result_n4 = hxc_localPlayer.hxc_id <= 0;
  }
  bool hxc_tmp_short_circuit_load_result_n2 = hxc_tmp_short_circuit_result_n4;
  bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_short_circuit_load_result_n2;
  if (!hxc_tmp_short_circuit_load_result_n2)
  {
    hxc_tmp_short_circuit_result_n5 = hxc_state.hxc_characterId != hxc_character.hxc_id;
  }
  if (hxc_tmp_short_circuit_result_n5)
  {
    return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanRejected, .hxc_payload.hxc_ControllerPlanRejected.hxc_error = hxc_caxecraft_domain_ActorControllerPlanError_CharacterIdentityMismatch };
  }
  hxc_g = hxc_state.hxc_profile;
  if (hxc_enum_25b991ce_retain(&hxc_g) != HXC_STATUS_OK)
  {
    abort();
  }
  switch (hxc_g.hxc_tag) {
    case hxc_caxecraft_domain_ActorControllerProfile_StationaryDialogue:
      {
        break;
      }
    case hxc_caxecraft_domain_ActorControllerProfile_WanderChaseMelee:
      {
        if (hxc_g.hxc_tag != hxc_caxecraft_domain_ActorControllerProfile_WanderChaseMelee)
        {
          abort();
        }
        struct hxc_caxecraft_domain_WanderChaseMeleeProfile hxc_tmp_enum_payload_project_n30 = hxc_g.hxc_payload.hxc_WanderChaseMelee.hxc_profile;
        hxc_profile = hxc_tmp_enum_payload_project_n30;
        if (hxc_record_daefea2e_retain(&hxc_profile) != HXC_STATUS_OK)
        {
          abort();
        }
        if (!(hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Stationary))
        {
          struct hxc_caxecraft_domain_ActorControllerDecision hxc_tmp_call_result_n36 = hxc_caxecraft_domain_ActorControllerScheduler_planWanderChaseMelee(hxc_state, hxc_character, hxc_localPlayer, hxc_tickNumber, hxc_profile);
          hxc_record_daefea2e_destroy(&hxc_profile);
          hxc_enum_25b991ce_destroy(&hxc_g);
          return hxc_tmp_call_result_n36;
        }
        hxc_record_daefea2e_destroy(&hxc_profile);
        hxc_enum_25b991ce_destroy(&hxc_g);
        return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanRejected, .hxc_payload.hxc_ControllerPlanRejected.hxc_error = hxc_caxecraft_domain_ActorControllerPlanError_ProfileStateMismatch };
      }
  }
  bool hxc_tmp_short_circuit_result_n7 = hxc_state.hxc_phase != hxc_caxecraft_domain_ActorControllerPhase_Stationary;
  if (!(hxc_state.hxc_phase != hxc_caxecraft_domain_ActorControllerPhase_Stationary))
  {
    hxc_tmp_short_circuit_result_n7 = hxc_state.hxc_phaseTicks != 0;
  }
  bool hxc_tmp_short_circuit_load_result_n13 = hxc_tmp_short_circuit_result_n7;
  bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_short_circuit_load_result_n13;
  if (!hxc_tmp_short_circuit_load_result_n13)
  {
    hxc_tmp_short_circuit_result_n8 = hxc_state.hxc_dropPublished;
  }
  if (hxc_tmp_short_circuit_result_n8)
  {
    hxc_enum_25b991ce_destroy(&hxc_g);
    return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanRejected, .hxc_payload.hxc_ControllerPlanRejected.hxc_error = hxc_caxecraft_domain_ActorControllerPlanError_ProfileStateMismatch };
  }
  bool hxc_tmp_call_result_n18 = hxc_caxecraft_domain_ActorControllerScheduler_interactionAvailable(hxc_state, hxc_character, hxc_localPlayer);
  bool hxc_inRange = hxc_tmp_call_result_n18;
  struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_enum_payload_0_owner_n10 = hxc_state;
  if (hxc_record_50dff1c7_retain(&hxc_tmp_enum_payload_0_owner_n10) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_enum_payload_0_n11 = hxc_tmp_enum_payload_0_owner_n10;
  struct hxc_caxecraft_domain_CharacterIntent hxc_tmp_enum_payload_1_n12 = (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = false, .hxc_jump = false, .hxc_moveX = 0.0, .hxc_moveZ = 0.0 };
  bool hxc_tmp_load_result_n21 = hxc_inRange;
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_tmp_conditional_managed_result_n13;
  if (hxc_tmp_load_result_n21)
  {
    (void)(struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable, .hxc_payload.hxc_InteractionAvailable.hxc_characterId = hxc_character.hxc_id };
    hxc_tmp_conditional_managed_result_n13 = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable, .hxc_payload.hxc_InteractionAvailable.hxc_characterId = hxc_character.hxc_id };
  }
  else
  {
    (void)(struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent };
    hxc_tmp_conditional_managed_result_n13 = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent };
  }
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_tmp_conditional_managed_move_result_n25 = hxc_tmp_conditional_managed_result_n13;
  struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_enum_payload_0_load_result_n26 = hxc_tmp_enum_payload_0_n11;
  struct hxc_caxecraft_domain_CharacterIntent hxc_tmp_enum_payload_1_load_result_n27 = hxc_tmp_enum_payload_1_n12;
  hxc_enum_25b991ce_destroy(&hxc_g);
  return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned, .hxc_payload.hxc_ControllerPlanned.hxc_next = hxc_tmp_enum_payload_0_load_result_n26, .hxc_payload.hxc_ControllerPlanned.hxc_intent = hxc_tmp_enum_payload_1_load_result_n27, .hxc_payload.hxc_ControllerPlanned.hxc_event = hxc_tmp_conditional_managed_move_result_n25 };
}

struct hxc_caxecraft_domain_ActorControllerDecision hxc_caxecraft_domain_ActorControllerScheduler_planWanderChaseMelee(struct hxc_caxecraft_domain_ActorControllerState hxc_state, struct hxc_caxecraft_domain_Character hxc_character, struct hxc_caxecraft_domain_Character hxc_localPlayer, int32_t hxc_tickNumber, struct hxc_caxecraft_domain_WanderChaseMeleeProfile hxc_profile)
{
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owner_n16 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owner_n18 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owner_n20 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owner_n21 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owner_n39 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_event = { 0 };
  if (!(hxc_character.hxc_vitals.hxc_health <= 0))
  {
    bool hxc_tmp_short_circuit_result_n10 = hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Defeated;
    if (!(hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Defeated))
    {
      hxc_tmp_short_circuit_result_n10 = hxc_state.hxc_dropPublished;
    }
    if (hxc_tmp_short_circuit_result_n10)
    {
      return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanRejected, .hxc_payload.hxc_ControllerPlanRejected.hxc_error = hxc_caxecraft_domain_ActorControllerPlanError_ProfileStateMismatch };
    }
    double hxc_dx_h64d085768560 = (hxc_localPlayer.hxc_body.hxc_x - hxc_character.hxc_body.hxc_x) * 1000.0;
    double hxc_dz_h837dee7c178a = (hxc_localPlayer.hxc_body.hxc_z - hxc_character.hxc_body.hxc_z) * 1000.0;
    double hxc_tmp_load_result_n34 = hxc_dx_h64d085768560;
    double hxc_tmp_load_result_n35 = hxc_dx_h64d085768560;
    double hxc_tmp_load_result_n36 = hxc_dz_h837dee7c178a;
    double hxc_playerDistanceSquared = hxc_tmp_load_result_n34 * hxc_tmp_load_result_n35 + hxc_tmp_load_result_n36 * hxc_dz_h837dee7c178a;
    int32_t hxc_value_h928a5e0edee8 = hxc_profile.hxc_attackRadiusMilli;
    int32_t hxc_tmp_load_result_n39 = hxc_value_h928a5e0edee8;
    double hxc_attackRadiusSquared = (double)hxc_tmp_load_result_n39 * 1.0 * (double)hxc_value_h928a5e0edee8;
    if (!(hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Windup))
    {
      bool hxc_tmp_short_circuit_result_n19 = hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Recovering;
      if (hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Recovering)
      {
        hxc_tmp_short_circuit_result_n19 = hxc_state.hxc_phaseTicks > 1;
      }
      if (hxc_tmp_short_circuit_result_n19)
      {
        hxc_tmp_record_field_profile_owner_n20 = hxc_state.hxc_profile;
        if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_profile_owner_n20) != HXC_STATUS_OK)
        {
          abort();
        }
        return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned, .hxc_payload.hxc_ControllerPlanned.hxc_next = (struct hxc_caxecraft_domain_ActorControllerState){ .hxc_characterId = hxc_state.hxc_characterId, .hxc_dropPublished = false, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_phase = hxc_caxecraft_domain_ActorControllerPhase_Recovering, .hxc_phaseTicks = hxc_i32_subtract_wrapping(hxc_state.hxc_phaseTicks, 1), .hxc_profile = hxc_tmp_record_field_profile_owner_n20 }, .hxc_payload.hxc_ControllerPlanned.hxc_intent = (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = false, .hxc_jump = false, .hxc_moveX = 0.0, .hxc_moveZ = 0.0 }, .hxc_payload.hxc_ControllerPlanned.hxc_event = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent } };
      }
      double hxc_tmp_load_result_n94 = hxc_playerDistanceSquared;
      if (hxc_tmp_load_result_n94 <= hxc_attackRadiusSquared)
      {
        hxc_tmp_record_field_profile_owner_n21 = hxc_state.hxc_profile;
        if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_profile_owner_n21) != HXC_STATUS_OK)
        {
          abort();
        }
        return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned, .hxc_payload.hxc_ControllerPlanned.hxc_next = (struct hxc_caxecraft_domain_ActorControllerState){ .hxc_characterId = hxc_state.hxc_characterId, .hxc_dropPublished = false, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_phase = hxc_caxecraft_domain_ActorControllerPhase_Windup, .hxc_phaseTicks = hxc_profile.hxc_windupTicks, .hxc_profile = hxc_tmp_record_field_profile_owner_n21 }, .hxc_payload.hxc_ControllerPlanned.hxc_intent = (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = false, .hxc_jump = false, .hxc_moveX = 0.0, .hxc_moveZ = 0.0 }, .hxc_payload.hxc_ControllerPlanned.hxc_event = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent } };
      }
      double hxc_homeDx = hxc_state.hxc_homeX - hxc_character.hxc_body.hxc_x;
      double hxc_homeDz = hxc_state.hxc_homeZ - hxc_character.hxc_body.hxc_z;
      double hxc_tmp_load_result_n113 = hxc_homeDx;
      double hxc_tmp_load_result_n114 = hxc_homeDx;
      double hxc_tmp_load_result_n115 = hxc_homeDz;
      double hxc_homeDistanceSquared = hxc_tmp_load_result_n113 * hxc_tmp_load_result_n114 + hxc_tmp_load_result_n115 * hxc_homeDz;
      enum hxc_caxecraft_domain_ActorControllerPhase hxc_nextPhase = hxc_caxecraft_domain_ActorControllerPhase_Resting;
      double hxc_targetX = hxc_character.hxc_body.hxc_x;
      double hxc_targetZ = hxc_character.hxc_body.hxc_z;
      int32_t hxc_value_hdd66a17d019c = hxc_profile.hxc_noticeRadiusMilli;
      double hxc_tmp_load_result_n123 = hxc_playerDistanceSquared;
      int32_t hxc_tmp_load_result_n124 = hxc_value_hdd66a17d019c;
      if (hxc_tmp_load_result_n123 <= (double)hxc_tmp_load_result_n124 * 1.0 * (double)hxc_value_hdd66a17d019c)
      {
        hxc_nextPhase = hxc_caxecraft_domain_ActorControllerPhase_Chasing;
        hxc_targetX = hxc_localPlayer.hxc_body.hxc_x;
        hxc_targetZ = hxc_localPlayer.hxc_body.hxc_z;
      }
      else
      {
        bool hxc_tmp_short_circuit_result_n29 = hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Chasing;
        if (!(hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Chasing))
        {
          hxc_tmp_short_circuit_result_n29 = hxc_state.hxc_phase == hxc_caxecraft_domain_ActorControllerPhase_Returning;
        }
        bool hxc_tmp_short_circuit_load_result_n135 = hxc_tmp_short_circuit_result_n29;
        bool hxc_tmp_short_circuit_result_n30 = hxc_tmp_short_circuit_load_result_n135;
        if (hxc_tmp_short_circuit_load_result_n135)
        {
          hxc_tmp_short_circuit_result_n30 = hxc_homeDistanceSquared > 0.0001;
        }
        if (hxc_tmp_short_circuit_result_n30)
        {
          hxc_nextPhase = hxc_caxecraft_domain_ActorControllerPhase_Returning;
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
          int32_t hxc_tmp_load_result_n145 = hxc_cycleTick;
          bool hxc_tmp_short_circuit_result_n32 = hxc_tmp_load_result_n145 >= 20;
          if (hxc_tmp_load_result_n145 >= 20)
          {
            hxc_tmp_short_circuit_result_n32 = hxc_cycleTick < 60;
          }
          if (hxc_tmp_short_circuit_result_n32)
          {
            hxc_targetX = hxc_targetX + 1.0;
          }
          else
          {
            int32_t hxc_tmp_load_result_n149 = hxc_cycleTick;
            bool hxc_tmp_short_circuit_result_n33 = hxc_tmp_load_result_n149 >= 60;
            if (hxc_tmp_load_result_n149 >= 60)
            {
              hxc_tmp_short_circuit_result_n33 = hxc_cycleTick < 100;
            }
            if (hxc_tmp_short_circuit_result_n33)
            {
              hxc_targetX = hxc_targetX + 1.0;
              hxc_targetZ = hxc_targetZ + 1.0;
            }
            else
            {
              int32_t hxc_tmp_load_result_n154 = hxc_cycleTick;
              bool hxc_tmp_short_circuit_result_n34 = hxc_tmp_load_result_n154 >= 100;
              if (hxc_tmp_load_result_n154 >= 100)
              {
                hxc_tmp_short_circuit_result_n34 = hxc_cycleTick < 140;
              }
              if (hxc_tmp_short_circuit_result_n34)
              {
                hxc_targetX = hxc_targetX - 1.0;
                hxc_targetZ = hxc_targetZ + 1.0;
              }
              else
              {
                int32_t hxc_tmp_load_result_n159 = hxc_cycleTick;
                bool hxc_tmp_short_circuit_result_n35 = hxc_tmp_load_result_n159 >= 140;
                if (hxc_tmp_load_result_n159 >= 140)
                {
                  hxc_tmp_short_circuit_result_n35 = hxc_cycleTick < 180;
                }
                if (hxc_tmp_short_circuit_result_n35)
                {
                  hxc_targetX = hxc_targetX - 1.0;
                  hxc_targetZ = hxc_targetZ - 1.0;
                }
                else
                {
                  int32_t hxc_tmp_load_result_n164 = hxc_cycleTick;
                  bool hxc_tmp_short_circuit_result_n36 = hxc_tmp_load_result_n164 >= 180;
                  if (hxc_tmp_load_result_n164 >= 180)
                  {
                    hxc_tmp_short_circuit_result_n36 = hxc_cycleTick < 220;
                  }
                  if (hxc_tmp_short_circuit_result_n36)
                  {
                    hxc_targetZ = hxc_targetZ - 1.0;
                  }
                }
              }
            }
          }
          double hxc_dx_h6708ac27226b = hxc_targetX - hxc_character.hxc_body.hxc_x;
          double hxc_dz_h55822cf369e2 = hxc_targetZ - hxc_character.hxc_body.hxc_z;
          double hxc_tmp_load_result_n174 = hxc_dx_h6708ac27226b;
          double hxc_tmp_load_result_n175 = hxc_dx_h6708ac27226b;
          double hxc_tmp_load_result_n176 = hxc_dz_h55822cf369e2;
          if (hxc_tmp_load_result_n174 * hxc_tmp_load_result_n175 + hxc_tmp_load_result_n176 * hxc_dz_h55822cf369e2 > 0.0001)
          {
            hxc_nextPhase = hxc_caxecraft_domain_ActorControllerPhase_Wandering;
          }
        }
      }
      if (hxc_nextPhase == hxc_caxecraft_domain_ActorControllerPhase_Resting)
      {
        hxc_tmp_record_field_profile_owner_n39 = hxc_state.hxc_profile;
        if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_profile_owner_n39) != HXC_STATUS_OK)
        {
          abort();
        }
        return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned, .hxc_payload.hxc_ControllerPlanned.hxc_next = (struct hxc_caxecraft_domain_ActorControllerState){ .hxc_characterId = hxc_state.hxc_characterId, .hxc_dropPublished = false, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_phase = hxc_caxecraft_domain_ActorControllerPhase_Resting, .hxc_phaseTicks = 0, .hxc_profile = hxc_tmp_record_field_profile_owner_n39 }, .hxc_payload.hxc_ControllerPlanned.hxc_intent = (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = false, .hxc_jump = false, .hxc_moveX = 0.0, .hxc_moveZ = 0.0 }, .hxc_payload.hxc_ControllerPlanned.hxc_event = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent } };
      }
      double hxc_dx_hb4f481376b83 = hxc_targetX - hxc_character.hxc_body.hxc_x;
      double hxc_dz_h4e27711f7a61 = hxc_targetZ - hxc_character.hxc_body.hxc_z;
      double hxc_amount = hxc_f64_divide_zero_safe((double)hxc_profile.hxc_stepMilli, (double)200);
      double hxc_tmp_load_result_n198 = hxc_dx_hb4f481376b83;
      double hxc_tmp_conditional_result_n44 = 0.0;
      if (hxc_tmp_load_result_n198 < 0.0)
      {
        hxc_tmp_conditional_result_n44 = -hxc_dx_hb4f481376b83;
      }
      else
      {
        hxc_tmp_conditional_result_n44 = hxc_dx_hb4f481376b83;
      }
      double hxc_tmp_binary_left_n45 = hxc_tmp_conditional_result_n44;
      double hxc_tmp_load_result_n202 = hxc_dz_h4e27711f7a61;
      double hxc_tmp_conditional_result_n46 = 0.0;
      if (hxc_tmp_load_result_n202 < 0.0)
      {
        hxc_tmp_conditional_result_n46 = -hxc_dz_h4e27711f7a61;
      }
      else
      {
        hxc_tmp_conditional_result_n46 = hxc_dz_h4e27711f7a61;
      }
      double hxc_tmp_conditional_load_result_n205 = hxc_tmp_conditional_result_n46;
      double hxc_tmp_binary_left_load_result_n206 = hxc_tmp_binary_left_n45;
      double hxc_tmp_conditional_result_n47 = 0.0;
      if (hxc_tmp_binary_left_load_result_n206 >= hxc_tmp_conditional_load_result_n205)
      {
        double hxc_tmp_load_result_n207 = hxc_dx_hb4f481376b83;
        double hxc_tmp_conditional_result_n48 = 0.0;
        if (hxc_tmp_load_result_n207 < 0.0)
        {
          hxc_tmp_conditional_result_n48 = -1.0;
        }
        else
        {
          double hxc_tmp_load_result_n208 = hxc_dx_hb4f481376b83;
          double hxc_tmp_conditional_result_n49 = 0.0;
          if (hxc_tmp_load_result_n208 > 0.0)
          {
            hxc_tmp_conditional_result_n49 = 1.0;
          }
          else
          {
            hxc_tmp_conditional_result_n49 = 0.0;
          }
          hxc_tmp_conditional_result_n48 = hxc_tmp_conditional_result_n49;
        }
        double hxc_tmp_conditional_load_result_n210 = hxc_tmp_conditional_result_n48;
        hxc_tmp_conditional_result_n47 = hxc_tmp_conditional_load_result_n210 * hxc_amount;
      }
      else
      {
        hxc_tmp_conditional_result_n47 = 0.0;
      }
      double hxc_moveX = hxc_tmp_conditional_result_n47;
      double hxc_tmp_load_result_n213 = hxc_dx_hb4f481376b83;
      double hxc_tmp_conditional_result_n51 = 0.0;
      if (hxc_tmp_load_result_n213 < 0.0)
      {
        hxc_tmp_conditional_result_n51 = -hxc_dx_hb4f481376b83;
      }
      else
      {
        hxc_tmp_conditional_result_n51 = hxc_dx_hb4f481376b83;
      }
      double hxc_tmp_binary_left_n52 = hxc_tmp_conditional_result_n51;
      double hxc_tmp_load_result_n217 = hxc_dz_h4e27711f7a61;
      double hxc_tmp_conditional_result_n53 = 0.0;
      if (hxc_tmp_load_result_n217 < 0.0)
      {
        hxc_tmp_conditional_result_n53 = -hxc_dz_h4e27711f7a61;
      }
      else
      {
        hxc_tmp_conditional_result_n53 = hxc_dz_h4e27711f7a61;
      }
      double hxc_tmp_conditional_load_result_n220 = hxc_tmp_conditional_result_n53;
      double hxc_tmp_binary_left_load_result_n221 = hxc_tmp_binary_left_n52;
      double hxc_tmp_conditional_result_n54 = 0.0;
      if (hxc_tmp_binary_left_load_result_n221 >= hxc_tmp_conditional_load_result_n220)
      {
        hxc_tmp_conditional_result_n54 = 0.0;
      }
      else
      {
        double hxc_tmp_load_result_n222 = hxc_dz_h4e27711f7a61;
        double hxc_tmp_conditional_result_n55 = 0.0;
        if (hxc_tmp_load_result_n222 < 0.0)
        {
          hxc_tmp_conditional_result_n55 = -1.0;
        }
        else
        {
          double hxc_tmp_load_result_n223 = hxc_dz_h4e27711f7a61;
          double hxc_tmp_conditional_result_n56 = 0.0;
          if (hxc_tmp_load_result_n223 > 0.0)
          {
            hxc_tmp_conditional_result_n56 = 1.0;
          }
          else
          {
            hxc_tmp_conditional_result_n56 = 0.0;
          }
          hxc_tmp_conditional_result_n55 = hxc_tmp_conditional_result_n56;
        }
        double hxc_tmp_conditional_load_result_n225 = hxc_tmp_conditional_result_n55;
        hxc_tmp_conditional_result_n54 = hxc_tmp_conditional_load_result_n225 * hxc_amount;
      }
      double hxc_moveZ = hxc_tmp_conditional_result_n54;
      struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owner_n57 = hxc_state.hxc_profile;
      if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_profile_owner_n57) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owned_load_result_n230 = hxc_tmp_record_field_profile_owner_n57;
      enum hxc_caxecraft_domain_ActorControllerPhase hxc_tmp_load_result_n233 = hxc_nextPhase;
      double hxc_tmp_load_result_n235 = hxc_moveX;
      return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned, .hxc_payload.hxc_ControllerPlanned.hxc_next = (struct hxc_caxecraft_domain_ActorControllerState){ .hxc_characterId = hxc_state.hxc_characterId, .hxc_dropPublished = false, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_phase = hxc_tmp_load_result_n233, .hxc_phaseTicks = 0, .hxc_profile = hxc_tmp_record_field_profile_owned_load_result_n230 }, .hxc_payload.hxc_ControllerPlanned.hxc_intent = (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = false, .hxc_jump = false, .hxc_moveX = hxc_tmp_load_result_n235, .hxc_moveZ = hxc_moveZ }, .hxc_payload.hxc_ControllerPlanned.hxc_event = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent } };
    }
    double hxc_tmp_load_result_n43 = hxc_playerDistanceSquared;
    if (hxc_tmp_load_result_n43 > hxc_attackRadiusSquared)
    {
      hxc_tmp_record_field_profile_owner_n16 = hxc_state.hxc_profile;
      if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_profile_owner_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned, .hxc_payload.hxc_ControllerPlanned.hxc_next = (struct hxc_caxecraft_domain_ActorControllerState){ .hxc_characterId = hxc_state.hxc_characterId, .hxc_dropPublished = false, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_phase = hxc_caxecraft_domain_ActorControllerPhase_Chasing, .hxc_phaseTicks = 0, .hxc_profile = hxc_tmp_record_field_profile_owner_n16 }, .hxc_payload.hxc_ControllerPlanned.hxc_intent = (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = false, .hxc_jump = false, .hxc_moveX = 0.0, .hxc_moveZ = 0.0 }, .hxc_payload.hxc_ControllerPlanned.hxc_event = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent } };
    }
    if (!(hxc_state.hxc_phaseTicks > 1))
    {
      hxc_tmp_record_field_profile_owner_n18 = hxc_state.hxc_profile;
      if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_profile_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned, .hxc_payload.hxc_ControllerPlanned.hxc_next = (struct hxc_caxecraft_domain_ActorControllerState){ .hxc_characterId = hxc_state.hxc_characterId, .hxc_dropPublished = false, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_phase = hxc_caxecraft_domain_ActorControllerPhase_Recovering, .hxc_phaseTicks = hxc_profile.hxc_recoveryTicks, .hxc_profile = hxc_tmp_record_field_profile_owner_n18 }, .hxc_payload.hxc_ControllerPlanned.hxc_intent = (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = false, .hxc_jump = false, .hxc_moveX = 0.0, .hxc_moveZ = 0.0 }, .hxc_payload.hxc_ControllerPlanned.hxc_event = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack, .hxc_payload.hxc_LocalPlayerAttack.hxc_characterId = hxc_character.hxc_id } };
    }
    struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owner_n17 = hxc_state.hxc_profile;
    if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_profile_owner_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned, .hxc_payload.hxc_ControllerPlanned.hxc_next = (struct hxc_caxecraft_domain_ActorControllerState){ .hxc_characterId = hxc_state.hxc_characterId, .hxc_dropPublished = false, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_phase = hxc_caxecraft_domain_ActorControllerPhase_Windup, .hxc_phaseTicks = hxc_i32_subtract_wrapping(hxc_state.hxc_phaseTicks, 1), .hxc_profile = hxc_tmp_record_field_profile_owner_n17 }, .hxc_payload.hxc_ControllerPlanned.hxc_intent = (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = false, .hxc_jump = false, .hxc_moveX = 0.0, .hxc_moveZ = 0.0 }, .hxc_payload.hxc_ControllerPlanned.hxc_event = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent } };
  }
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_tmp_conditional_managed_result_n6;
  if (hxc_state.hxc_dropPublished)
  {
    (void)(struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent };
    hxc_tmp_conditional_managed_result_n6 = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent };
  }
  else
  {
    hxc_string hxc_tmp_enum_payload_1_owner_n7 = hxc_profile.hxc_drop;
    if (hxc_string_retain(hxc_tmp_enum_payload_1_owner_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)(struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_DropRequested, .hxc_payload.hxc_DropRequested.hxc_characterId = hxc_character.hxc_id, .hxc_payload.hxc_DropRequested.hxc_drop = hxc_tmp_enum_payload_1_owner_n7 };
    hxc_tmp_conditional_managed_result_n6 = (struct hxc_caxecraft_domain_ActorControllerEvent){ .hxc_tag = hxc_caxecraft_domain_ActorControllerEvent_DropRequested, .hxc_payload.hxc_DropRequested.hxc_characterId = hxc_character.hxc_id, .hxc_payload.hxc_DropRequested.hxc_drop = hxc_tmp_enum_payload_1_owner_n7 };
  }
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_tmp_conditional_managed_move_result_n8 = hxc_tmp_conditional_managed_result_n6;
  hxc_event = hxc_tmp_conditional_managed_move_result_n8;
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owner_n8 = hxc_state.hxc_profile;
  if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_profile_owner_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owned_load_result_n11 = hxc_tmp_record_field_profile_owner_n8;
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_tmp_enum_payload_2_owner_n9 = hxc_event;
  if (hxc_enum_8b2a26b8_retain(&hxc_tmp_enum_payload_2_owner_n9) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_tmp_enum_payload_2_owned_load_result_n18 = hxc_tmp_enum_payload_2_owner_n9;
  hxc_enum_8b2a26b8_destroy(&hxc_event);
  return (struct hxc_caxecraft_domain_ActorControllerDecision){ .hxc_tag = hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned, .hxc_payload.hxc_ControllerPlanned.hxc_next = (struct hxc_caxecraft_domain_ActorControllerState){ .hxc_characterId = hxc_state.hxc_characterId, .hxc_dropPublished = true, .hxc_homeX = hxc_state.hxc_homeX, .hxc_homeZ = hxc_state.hxc_homeZ, .hxc_phase = hxc_caxecraft_domain_ActorControllerPhase_Defeated, .hxc_phaseTicks = 0, .hxc_profile = hxc_tmp_record_field_profile_owned_load_result_n11 }, .hxc_payload.hxc_ControllerPlanned.hxc_intent = (struct hxc_caxecraft_domain_CharacterIntent){ .hxc_descend = false, .hxc_jump = false, .hxc_moveX = 0.0, .hxc_moveZ = 0.0 }, .hxc_payload.hxc_ControllerPlanned.hxc_event = hxc_tmp_enum_payload_2_owned_load_result_n18 };
}

struct hxc_caxecraft_domain_ActorControllerState hxc_caxecraft_domain_ActorControllerScheduler_startActorController(struct hxc_caxecraft_domain_ActorControllerBinding hxc_binding, struct hxc_caxecraft_domain_Character hxc_character)
{
  enum hxc_caxecraft_domain_ActorControllerPhase hxc_phase;
  switch (hxc_binding.hxc_profile.hxc_tag) {
    case hxc_caxecraft_domain_ActorControllerProfile_StationaryDialogue:
      {
        hxc_phase = hxc_caxecraft_domain_ActorControllerPhase_Stationary;
        break;
      }
    case hxc_caxecraft_domain_ActorControllerProfile_WanderChaseMelee:
      {
        hxc_phase = hxc_caxecraft_domain_ActorControllerPhase_Resting;
        break;
      }
  }
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owner_n3 = hxc_binding.hxc_profile;
  if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_profile_owner_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_profile_owned_load_result_n5 = hxc_tmp_record_field_profile_owner_n3;
  return (struct hxc_caxecraft_domain_ActorControllerState){ .hxc_characterId = hxc_binding.hxc_characterId, .hxc_dropPublished = false, .hxc_homeX = hxc_character.hxc_body.hxc_x, .hxc_homeZ = hxc_character.hxc_body.hxc_z, .hxc_phase = hxc_phase, .hxc_phaseTicks = 0, .hxc_profile = hxc_tmp_record_field_profile_owned_load_result_n5 };
}

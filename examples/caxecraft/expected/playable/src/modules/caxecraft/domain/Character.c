#include "hxc/program.h"

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_adoptProfile(struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_AquaticProfile hxc_replacement)
{
  struct hxc_caxecraft_domain_AquaticState hxc_tmp_call_result_n4 = hxc_caxecraft_domain_Aquatics_adoptProfile(hxc_original.hxc_aquatic, hxc_original.hxc_aquaticProfile, hxc_replacement);
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_tmp_call_result_n4, .hxc_aquaticProfile = hxc_replacement, .hxc_body = hxc_original.hxc_body, .hxc_id = hxc_original.hxc_id, .hxc_vitals = hxc_original.hxc_vitals };
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_applyAttack(struct hxc_caxecraft_domain_Character hxc_original, bool hxc_attacked)
{
  struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n1 = hxc_caxecraft_domain_Vitals_applyAttack(hxc_original.hxc_vitals, hxc_attacked);
  struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n2 = hxc_caxecraft_domain_Character_withVitals(hxc_original, hxc_tmp_call_result_n1);
  return hxc_tmp_call_result_n2;
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_empty(void)
{
  struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_profile(1, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 1, false, false);
  struct hxc_caxecraft_domain_AquaticProfile hxc_emptyProfile = hxc_tmp_call_result_n0;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n1 = hxc_caxecraft_domain_CharacterPhysics_body(0.0, 0.0, 0.0);
  struct hxc_caxecraft_domain_AquaticState hxc_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_start(hxc_emptyProfile);
  struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_load_result_n4 = hxc_emptyProfile;
  struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n5 = hxc_caxecraft_domain_Vitals_make(0, 0);
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_tmp_call_result_n3, .hxc_aquaticProfile = hxc_tmp_load_result_n4, .hxc_body = hxc_tmp_call_result_n1, .hxc_id = 0, .hxc_vitals = hxc_tmp_call_result_n5 };
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_reviveAt(struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_CharacterBody hxc_body)
{
  int32_t hxc_original1 = hxc_original.hxc_id;
  struct hxc_caxecraft_domain_AquaticState hxc_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_start(hxc_original.hxc_aquaticProfile);
  struct hxc_caxecraft_domain_AquaticState hxc_tmp = hxc_tmp_call_result_n2;
  struct hxc_caxecraft_domain_VitalsState hxc_state = hxc_original.hxc_vitals;
  (void)hxc_state;
  int32_t hxc_tmp_load_result_n4 = hxc_original1;
  struct hxc_caxecraft_domain_AquaticState hxc_tmp_load_result_n5 = hxc_tmp;
  struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n7 = hxc_caxecraft_domain_Vitals_make(6, 0);
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_tmp_load_result_n5, .hxc_aquaticProfile = hxc_original.hxc_aquaticProfile, .hxc_body = hxc_body, .hxc_id = hxc_tmp_load_result_n4, .hxc_vitals = hxc_tmp_call_result_n7 };
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_start(int32_t hxc_id, struct hxc_caxecraft_domain_CharacterBody hxc_body, struct hxc_caxecraft_domain_AquaticProfile hxc_aquaticProfile, int32_t hxc_health)
{
  struct hxc_caxecraft_domain_AquaticState hxc_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_start(hxc_aquaticProfile);
  struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n1 = hxc_caxecraft_domain_Vitals_make(hxc_health, 0);
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_tmp_call_result_n0, .hxc_aquaticProfile = hxc_aquaticProfile, .hxc_body = hxc_body, .hxc_id = hxc_id, .hxc_vitals = hxc_tmp_call_result_n1 };
}

struct hxc_caxecraft_domain_CharacterStep hxc_caxecraft_domain_Character_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_CharacterIntent hxc_intent, int32_t hxc_damagePolicy)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  if (!(hxc_original.hxc_vitals.hxc_health <= 0))
  {
    struct hxc_caxecraft_domain_AquaticStep hxc_tmp_call_result_n8 = hxc_caxecraft_domain_Aquatics_step(hxc_cells, hxc_length, hxc_original.hxc_body, hxc_original.hxc_aquatic, hxc_intent, hxc_original.hxc_aquaticProfile);
    struct hxc_caxecraft_domain_AquaticStep hxc_aquaticStep = hxc_tmp_call_result_n8;
    struct hxc_caxecraft_domain_VitalsState hxc_vitals = hxc_original.hxc_vitals;
    if (hxc_damagePolicy == 1)
    {
      struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n11 = hxc_caxecraft_domain_Vitals_step(hxc_vitals);
      hxc_vitals = hxc_tmp_call_result_n11;
      struct hxc_caxecraft_domain_VitalsState hxc_tmp_load_result_n12 = hxc_vitals;
      struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n14 = hxc_caxecraft_domain_Vitals_applyAttack(hxc_tmp_load_result_n12, hxc_aquaticStep.hxc_drowningDamage > 0);
      hxc_vitals = hxc_tmp_call_result_n14;
    }
    struct hxc_caxecraft_domain_CharacterBody hxc_tmp_record_field_load_result_n16 = hxc_aquaticStep.hxc_body;
    struct hxc_caxecraft_domain_AquaticState hxc_tmp_record_field_load_result_n17 = hxc_aquaticStep.hxc_aquatic;
    struct hxc_caxecraft_domain_VitalsState hxc_tmp_load_result_n19 = hxc_vitals;
    struct hxc_caxecraft_domain_Immersion hxc_tmp_record_field_load_result_n21 = hxc_aquaticStep.hxc_immersion;
    return (struct hxc_caxecraft_domain_CharacterStep){ .hxc_character = (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_tmp_record_field_load_result_n17, .hxc_aquaticProfile = hxc_original.hxc_aquaticProfile, .hxc_body = hxc_tmp_record_field_load_result_n16, .hxc_id = hxc_original.hxc_id, .hxc_vitals = hxc_tmp_load_result_n19 }, .hxc_drowningDamage = hxc_aquaticStep.hxc_drowningDamage, .hxc_immersion = hxc_tmp_record_field_load_result_n21 };
  }
  struct hxc_caxecraft_domain_Immersion hxc_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_observe(hxc_cells, hxc_length, hxc_original.hxc_body);
  return (struct hxc_caxecraft_domain_CharacterStep){ .hxc_character = hxc_original, .hxc_drowningDamage = 0, .hxc_immersion = hxc_tmp_call_result_n3 };
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_withVitals(struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_VitalsState hxc_vitals)
{
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_original.hxc_aquatic, .hxc_aquaticProfile = hxc_original.hxc_aquaticProfile, .hxc_body = hxc_original.hxc_body, .hxc_id = hxc_original.hxc_id, .hxc_vitals = hxc_vitals };
}

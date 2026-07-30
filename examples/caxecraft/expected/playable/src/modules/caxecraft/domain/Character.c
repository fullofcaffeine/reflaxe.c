#include "hxc/program.h"

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_adoptProfile(struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement)
{
  struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_Aquatics_adoptProfile(hxc_l_original.hxc_aquatic, hxc_l_original.hxc_aquaticProfile, hxc_l_replacement);
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_l_tmp_call_result_n4, .hxc_aquaticProfile = hxc_l_replacement, .hxc_body = hxc_l_original.hxc_body, .hxc_id = hxc_l_original.hxc_id, .hxc_vitals = hxc_l_original.hxc_vitals };
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_applyAttack(struct hxc_caxecraft_domain_Character hxc_l_original, bool hxc_l_attacked)
{
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_Vitals_applyAttack(hxc_l_original.hxc_vitals, hxc_l_attacked);
  struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_Character_withVitals(hxc_l_original, hxc_l_tmp_call_result_n1);
  return hxc_l_tmp_call_result_n2;
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_applyDamage(struct hxc_caxecraft_domain_Character hxc_l_original, int32_t hxc_l_amount)
{
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_Vitals_applyDamage(hxc_l_original.hxc_vitals, hxc_l_amount);
  struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_Character_withVitals(hxc_l_original, hxc_l_tmp_call_result_n1);
  return hxc_l_tmp_call_result_n2;
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_empty(void)
{
  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_profile(1, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 1, false, false);
  struct hxc_caxecraft_domain_AquaticProfile hxc_l_emptyProfile = hxc_l_tmp_call_result_n0;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_CharacterPhysics_body(0.0, 0.0, 0.0);
  struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_start(hxc_l_emptyProfile);
  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_load_result_n4 = hxc_l_emptyProfile;
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_Vitals_make(0, 0);
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_l_tmp_call_result_n3, .hxc_aquaticProfile = hxc_l_tmp_load_result_n4, .hxc_body = hxc_l_tmp_call_result_n1, .hxc_id = 0, .hxc_vitals = hxc_l_tmp_call_result_n5 };
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_reviveAt(struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_CharacterBody hxc_l_body)
{
  int32_t hxc_l_original1 = hxc_l_original.hxc_id;
  struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_start(hxc_l_original.hxc_aquaticProfile);
  struct hxc_caxecraft_domain_AquaticState hxc_l_tmp = hxc_l_tmp_call_result_n2;
  struct hxc_caxecraft_domain_VitalsState hxc_l_state = hxc_l_original.hxc_vitals;
  (void)hxc_l_state;
  int32_t hxc_l_tmp_load_result_n4 = hxc_l_original1;
  struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_load_result_n5 = hxc_l_tmp;
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_Vitals_make(6, 0);
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_l_tmp_load_result_n5, .hxc_aquaticProfile = hxc_l_original.hxc_aquaticProfile, .hxc_body = hxc_l_body, .hxc_id = hxc_l_tmp_load_result_n4, .hxc_vitals = hxc_l_tmp_call_result_n7 };
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_start(int32_t hxc_l_id, struct hxc_caxecraft_domain_CharacterBody hxc_l_body, struct hxc_caxecraft_domain_AquaticProfile hxc_l_aquaticProfile, int32_t hxc_l_health)
{
  struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_Aquatics_start(hxc_l_aquaticProfile);
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_Vitals_make(hxc_l_health, 0);
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_l_tmp_call_result_n0, .hxc_aquaticProfile = hxc_l_aquaticProfile, .hxc_body = hxc_l_body, .hxc_id = hxc_l_id, .hxc_vitals = hxc_l_tmp_call_result_n1 };
}

struct hxc_caxecraft_domain_CharacterStep hxc_caxecraft_domain_Character_step(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_CharacterIntent hxc_l_intent, int32_t hxc_l_damagePolicy)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n4 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n4;
  if (hxc_l_original.hxc_vitals.hxc_health <= 0)
  {
    struct hxc_caxecraft_domain_Immersion hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_observe(hxc_l_cells, hxc_l_length, hxc_l_original.hxc_body);
    return (struct hxc_caxecraft_domain_CharacterStep){ .hxc_character = hxc_l_original, .hxc_drowningDamage = 0, .hxc_immersion = hxc_l_tmp_call_result_n3 };
  }
  struct hxc_caxecraft_domain_AquaticStep hxc_l_tmp_call_result_n8 = hxc_caxecraft_domain_Aquatics_step(hxc_l_cells, hxc_l_length, hxc_l_original.hxc_body, hxc_l_original.hxc_aquatic, hxc_l_intent, hxc_l_original.hxc_aquaticProfile);
  struct hxc_caxecraft_domain_AquaticStep hxc_l_aquaticStep = hxc_l_tmp_call_result_n8;
  struct hxc_caxecraft_domain_VitalsState hxc_l_vitals = hxc_l_original.hxc_vitals;
  if (hxc_l_damagePolicy == 1)
  {
    struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n11 = hxc_caxecraft_domain_Vitals_step(hxc_l_vitals);
    hxc_l_vitals = hxc_l_tmp_call_result_n11;
    struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_load_result_n12 = hxc_l_vitals;
    struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n14 = hxc_caxecraft_domain_Vitals_applyAttack(hxc_l_tmp_load_result_n12, hxc_l_aquaticStep.hxc_drowningDamage > 0);
    hxc_l_vitals = hxc_l_tmp_call_result_n14;
  }
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n16 = hxc_l_aquaticStep.hxc_body;
  struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n17 = hxc_l_aquaticStep.hxc_aquatic;
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_load_result_n19 = hxc_l_vitals;
  struct hxc_caxecraft_domain_Immersion hxc_l_tmp_record_field_load_result_n21 = hxc_l_aquaticStep.hxc_immersion;
  return (struct hxc_caxecraft_domain_CharacterStep){ .hxc_character = (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_l_tmp_record_field_load_result_n17, .hxc_aquaticProfile = hxc_l_original.hxc_aquaticProfile, .hxc_body = hxc_l_tmp_record_field_load_result_n16, .hxc_id = hxc_l_original.hxc_id, .hxc_vitals = hxc_l_tmp_load_result_n19 }, .hxc_drowningDamage = hxc_l_aquaticStep.hxc_drowningDamage, .hxc_immersion = hxc_l_tmp_record_field_load_result_n21 };
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_withVitals(struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_VitalsState hxc_l_vitals)
{
  return (struct hxc_caxecraft_domain_Character){ .hxc_aquatic = hxc_l_original.hxc_aquatic, .hxc_aquaticProfile = hxc_l_original.hxc_aquaticProfile, .hxc_body = hxc_l_original.hxc_body, .hxc_id = hxc_l_original.hxc_id, .hxc_vitals = hxc_l_vitals };
}

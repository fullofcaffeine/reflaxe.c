#ifndef HXC_CAXECRAFT_DOMAIN_CHARACTER_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_CHARACTER_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/domain/AquaticProfile.h"
#include "hxc/modules/caxecraft/domain/AquaticState.h"
#include "hxc/modules/caxecraft/domain/CharacterBody.h"
#include "hxc/modules/caxecraft/domain/VitalsState.h"

struct hxc_caxecraft_domain_Character {
  struct hxc_caxecraft_domain_AquaticState hxc_aquatic;
  struct hxc_caxecraft_domain_AquaticProfile hxc_aquaticProfile;
  struct hxc_caxecraft_domain_CharacterBody hxc_body;
  int32_t hxc_id;
  struct hxc_caxecraft_domain_VitalsState hxc_vitals;
};

struct hxc_optional_caxecraft_domain_Character {
  bool hxc_has_value;
  struct hxc_caxecraft_domain_Character hxc_value;
};

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_adoptProfile(struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_applyAttack(struct hxc_caxecraft_domain_Character hxc_l_original, bool hxc_l_attacked);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_applyDamage(struct hxc_caxecraft_domain_Character hxc_l_original, int32_t hxc_l_amount);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_empty(void);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_reviveAt(struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_CharacterBody hxc_l_body);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_start(int32_t hxc_l_id, struct hxc_caxecraft_domain_CharacterBody hxc_l_body, struct hxc_caxecraft_domain_AquaticProfile hxc_l_aquaticProfile, int32_t hxc_l_health);

struct hxc_caxecraft_domain_CharacterStep hxc_caxecraft_domain_Character_step(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_CharacterIntent hxc_l_intent, int32_t hxc_l_damagePolicy);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_withVitals(struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_VitalsState hxc_l_vitals);

#endif /* HXC_CAXECRAFT_DOMAIN_CHARACTER_H_INCLUDED */

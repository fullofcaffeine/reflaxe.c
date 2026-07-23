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

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_adoptProfile(struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_AquaticProfile hxc_replacement);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_applyAttack(struct hxc_caxecraft_domain_Character hxc_original, bool hxc_attacked);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_empty(void);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_reviveAt(struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_CharacterBody hxc_body);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_start(int32_t hxc_id, struct hxc_caxecraft_domain_CharacterBody hxc_body, struct hxc_caxecraft_domain_AquaticProfile hxc_aquaticProfile, int32_t hxc_health);

struct hxc_caxecraft_domain_CharacterStep hxc_caxecraft_domain_Character_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_CharacterIntent hxc_intent, int32_t hxc_damagePolicy);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_Character_withVitals(struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_VitalsState hxc_vitals);

#endif /* HXC_CAXECRAFT_DOMAIN_CHARACTER_H_INCLUDED */

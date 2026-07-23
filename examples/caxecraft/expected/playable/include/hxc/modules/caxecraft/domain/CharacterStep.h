#ifndef HXC_CAXECRAFT_DOMAIN_CHARACTER_STEP_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_CHARACTER_STEP_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/domain/Character.h"
#include "hxc/modules/caxecraft/domain/Immersion.h"

struct hxc_caxecraft_domain_CharacterStep {
  struct hxc_caxecraft_domain_Character hxc_character;
  int32_t hxc_drowningDamage;
  struct hxc_caxecraft_domain_Immersion hxc_immersion;
};

#endif /* HXC_CAXECRAFT_DOMAIN_CHARACTER_STEP_H_INCLUDED */

#ifndef HXC_CAXECRAFT_DOMAIN_ACTOR_CONTROLLER_STATE_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_ACTOR_CONTROLLER_STATE_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/domain/ActorControllerProfile.h"

struct hxc_caxecraft_domain_ActorControllerState {
  int32_t hxc_characterId;
  bool hxc_dropPublished;
  double hxc_homeX;
  double hxc_homeZ;
  enum hxc_caxecraft_domain_ActorControllerPhase hxc_phase;
  int32_t hxc_phaseTicks;
  struct hxc_caxecraft_domain_ActorControllerProfile hxc_profile;
};

struct hxc_optional_caxecraft_domain_ActorControllerState {
  bool hxc_has_value;
  struct hxc_caxecraft_domain_ActorControllerState hxc_value;
};

#endif /* HXC_CAXECRAFT_DOMAIN_ACTOR_CONTROLLER_STATE_H_INCLUDED */

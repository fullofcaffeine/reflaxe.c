#ifndef HXC_CAXECRAFT_DOMAIN_ACTOR_CONTROLLER_EVENT_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_ACTOR_CONTROLLER_EVENT_H_INCLUDED

#include "hxc/detail/program_types.h"

enum hxc_caxecraft_domain_ActorControllerEvent_tag {
  hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent = 0,
  hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable = 1,
  hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack = 2,
  hxc_caxecraft_domain_ActorControllerEvent_DropRequested = 3
};

struct hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable_payload {
  int32_t hxc_characterId;
};

struct hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack_payload {
  int32_t hxc_characterId;
};

struct hxc_caxecraft_domain_ActorControllerEvent_DropRequested_payload {
  int32_t hxc_characterId;
  hxc_string hxc_drop;
};

union hxc_caxecraft_domain_ActorControllerEvent_payload {
  struct hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable_payload hxc_InteractionAvailable;
  struct hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack_payload hxc_LocalPlayerAttack;
  struct hxc_caxecraft_domain_ActorControllerEvent_DropRequested_payload hxc_DropRequested;
};

struct hxc_caxecraft_domain_ActorControllerEvent {
  enum hxc_caxecraft_domain_ActorControllerEvent_tag hxc_tag;
  union hxc_caxecraft_domain_ActorControllerEvent_payload hxc_payload;
};

struct hxc_optional_caxecraft_domain_ActorControllerEvent {
  bool hxc_has_value;
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_value;
};

#endif /* HXC_CAXECRAFT_DOMAIN_ACTOR_CONTROLLER_EVENT_H_INCLUDED */

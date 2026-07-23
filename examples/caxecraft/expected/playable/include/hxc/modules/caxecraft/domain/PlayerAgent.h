#ifndef HXC_CAXECRAFT_DOMAIN_PLAYER_AGENT_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_PLAYER_AGENT_H_INCLUDED

#include "hxc/detail/program_types.h"

struct hxc_caxecraft_domain_PlayerAgent {
  int32_t hxc_characterId;
};

struct hxc_caxecraft_domain_PlayerAgent hxc_caxecraft_domain_PlayerAgent_bind(int32_t hxc_characterId);

#endif /* HXC_CAXECRAFT_DOMAIN_PLAYER_AGENT_H_INCLUDED */

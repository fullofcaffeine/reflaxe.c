#include "hxc/program.h"

struct hxc_caxecraft_domain_PlayerAgent hxc_caxecraft_domain_PlayerAgent_bind(int32_t hxc_characterId)
{
  return (struct hxc_caxecraft_domain_PlayerAgent){ .hxc_characterId = hxc_characterId };
}

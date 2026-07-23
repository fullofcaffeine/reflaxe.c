#ifndef HXC_CAXECRAFT_DOMAIN_ENTITY_STORE_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_ENTITY_STORE_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/domain/Character.h"

struct hxc_caxecraft_domain_EntityStore {
  bool hxc_occupied;
  struct hxc_caxecraft_domain_Character hxc_current;
};

void hxc_compiler_constructor_caxecraft_domain_EntityStore(struct hxc_caxecraft_domain_EntityStore *hxc_self);

bool hxc_caxecraft_domain_EntityStore_put(struct hxc_caxecraft_domain_EntityStore *hxc_self, struct hxc_caxecraft_domain_Character hxc_character);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_EntityStore_read(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id);

bool hxc_caxecraft_domain_EntityStore_replace(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id, struct hxc_caxecraft_domain_Character hxc_character);

#endif /* HXC_CAXECRAFT_DOMAIN_ENTITY_STORE_H_INCLUDED */

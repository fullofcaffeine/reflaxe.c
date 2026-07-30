#ifndef HXC_CAXECRAFT_DOMAIN_ENTITY_STORE_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_ENTITY_STORE_H_INCLUDED

#include "hxc/detail/program_types.h"

struct hxc_caxecraft_domain_EntityStore {
  struct hxc_array_ref *hxc_characters;
};

void hxc_compiler_constructor_caxecraft_domain_EntityStore(struct hxc_caxecraft_domain_EntityStore *hxc_self);

bool hxc_caxecraft_domain_EntityStore_contains(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id);

int32_t hxc_caxecraft_domain_EntityStore_indexOf(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id);

bool hxc_caxecraft_domain_EntityStore_put(struct hxc_caxecraft_domain_EntityStore *hxc_self, struct hxc_caxecraft_domain_Character hxc_character);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_EntityStore_read(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id);

bool hxc_caxecraft_domain_EntityStore_replace(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id, struct hxc_caxecraft_domain_Character hxc_character);

bool hxc_caxecraft_domain_EntityStore_replaceOthers(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_retained, struct hxc_array_ref *hxc_replacements);

#endif /* HXC_CAXECRAFT_DOMAIN_ENTITY_STORE_H_INCLUDED */

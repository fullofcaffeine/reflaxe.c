#ifndef HXC_CAXECRAFT_DOMAIN_WORLD_READ_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_WORLD_READ_H_INCLUDED

#include "hxc/detail/program_types.h"

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_WorldRead_query(const uint8_t *hxc_view, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord);

int32_t hxc_caxecraft_domain_WorldRead_storageCodeAt(const uint8_t *hxc_view, size_t hxc_length, int32_t hxc_index);

#endif /* HXC_CAXECRAFT_DOMAIN_WORLD_READ_H_INCLUDED */

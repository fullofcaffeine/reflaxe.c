#ifndef HXC_CAXECRAFT_DOMAIN_WORLD_READ_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_WORLD_READ_H_INCLUDED

#include "hxc/detail/program_types.h"

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_WorldRead_query(const uint8_t *hxc_l_view, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord);

int32_t hxc_caxecraft_domain_WorldRead_storageCodeAt(const uint8_t *hxc_l_view, size_t hxc_l_length, int32_t hxc_l_index);

#endif /* HXC_CAXECRAFT_DOMAIN_WORLD_READ_H_INCLUDED */

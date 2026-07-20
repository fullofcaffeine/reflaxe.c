#ifndef HXC_CAXECRAFT_DOMAIN_WORLD_STORAGE_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_WORLD_STORAGE_H_INCLUDED

#include "hxc/detail/program_types.h"

int32_t hxc_caxecraft_domain_WorldStorage_readCode(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_index);

void hxc_caxecraft_domain_WorldStorage_writeCode(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_index, int32_t hxc_code);

#endif /* HXC_CAXECRAFT_DOMAIN_WORLD_STORAGE_H_INCLUDED */

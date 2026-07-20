#ifndef HXC_CAXECRAFT_DOMAIN_CAXECRAFT_TRACE_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_CAXECRAFT_TRACE_H_INCLUDED

#include "hxc/detail/program_types.h"

int32_t hxc_caxecraft_domain_CaxecraftTrace_collisionTrace(void);

int32_t hxc_caxecraft_domain_CaxecraftTrace_editTrace(void);

void hxc_caxecraft_domain_CaxecraftTrace_makeFloor(uint8_t *hxc_cells, size_t hxc_length);

int32_t hxc_caxecraft_domain_CaxecraftTrace_mix(int32_t hxc_hash, int32_t hxc_value);

int32_t hxc_caxecraft_domain_CaxecraftTrace_propertyTrace(int32_t hxc_seed);

int32_t hxc_caxecraft_domain_CaxecraftTrace_rayTrace(void);

int32_t hxc_caxecraft_domain_CaxecraftTrace_runTrace(void);

int32_t hxc_caxecraft_domain_CaxecraftTrace_terrainTrace(void);

#endif /* HXC_CAXECRAFT_DOMAIN_CAXECRAFT_TRACE_H_INCLUDED */

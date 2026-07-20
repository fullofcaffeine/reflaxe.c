#ifndef HXC_CAXECRAFT_DOMAIN_WORLD_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_WORLD_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/domain/BlockKind.h"

bool hxc_caxecraft_domain_World_contains(struct hxc_caxecraft_domain_BlockCoord hxc_coord);

struct hxc_caxecraft_domain_BlockCoord hxc_caxecraft_domain_World_coord(int32_t hxc_x, int32_t hxc_y, int32_t hxc_z);

void hxc_caxecraft_domain_World_generate(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_seed);

int32_t hxc_caxecraft_domain_World_indexOf(struct hxc_caxecraft_domain_BlockCoord hxc_coord);

bool hxc_caxecraft_domain_World_isPlaceable(enum hxc_caxecraft_domain_BlockKind hxc_kind);

bool hxc_caxecraft_domain_World_isSolid(enum hxc_caxecraft_domain_BlockKind hxc_kind);

int32_t hxc_caxecraft_domain_World_kindCode(enum hxc_caxecraft_domain_BlockKind hxc_kind);

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_World_kindFromCode(int32_t hxc_code);

bool hxc_caxecraft_domain_World_place(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord, enum hxc_caxecraft_domain_BlockKind hxc_kind);

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_World_query(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord);

bool hxc_caxecraft_domain_World_remove(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord);

bool hxc_caxecraft_domain_World_replace(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord, enum hxc_caxecraft_domain_BlockKind hxc_kind);

int32_t hxc_caxecraft_domain_World_stateHash(uint8_t *hxc_cells, size_t hxc_length);

int32_t hxc_caxecraft_domain_World_terrainNoise(int32_t hxc_seed, int32_t hxc_x, int32_t hxc_z);

#endif /* HXC_CAXECRAFT_DOMAIN_WORLD_H_INCLUDED */

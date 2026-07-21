#ifndef HXC_CAXECRAFT_GAMEPLAY_MINING_H_INCLUDED
#define HXC_CAXECRAFT_GAMEPLAY_MINING_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/domain/BlockKind.h"
#include "hxc/modules/caxecraft/gameplay/ItemKind.h"

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_gameplay_Mining_attempt(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coordinate, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory);

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_gameplay_Mining_collect(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coordinate, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, enum hxc_caxecraft_domain_BlockKind hxc_kind, enum hxc_caxecraft_gameplay_ItemKind hxc_item);

#endif /* HXC_CAXECRAFT_GAMEPLAY_MINING_H_INCLUDED */

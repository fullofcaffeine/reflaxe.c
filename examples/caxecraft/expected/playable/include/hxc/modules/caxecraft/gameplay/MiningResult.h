#ifndef HXC_CAXECRAFT_GAMEPLAY_MINING_RESULT_H_INCLUDED
#define HXC_CAXECRAFT_GAMEPLAY_MINING_RESULT_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/gameplay/InventoryState.h"

struct hxc_caxecraft_gameplay_MiningResult {
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory;
  int32_t hxc_outcome;
};

#endif /* HXC_CAXECRAFT_GAMEPLAY_MINING_RESULT_H_INCLUDED */

#include "hxc/program.h"

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_gameplay_Mining_attempt(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coordinate, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_coordinate);
  enum hxc_caxecraft_domain_BlockKind hxc_kind = hxc_tmp_call_result_n0;
  switch (hxc_kind) {
    case hxc_caxecraft_domain_BlockKind_Grass:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Dirt:
      {
        struct hxc_caxecraft_gameplay_MiningResult hxc_tmp_call_result_n7 = hxc_caxecraft_gameplay_Mining_collect(hxc_cells, hxc_length, hxc_coordinate, hxc_inventory, hxc_kind, hxc_caxecraft_gameplay_ItemKind_DirtBlock);
        return hxc_tmp_call_result_n7;
      }
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        struct hxc_caxecraft_gameplay_MiningResult hxc_tmp_call_result_n10 = hxc_caxecraft_gameplay_Mining_collect(hxc_cells, hxc_length, hxc_coordinate, hxc_inventory, hxc_kind, hxc_caxecraft_gameplay_ItemKind_StoneBlock);
        return hxc_tmp_call_result_n10;
      }
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
      {
        return (struct hxc_caxecraft_gameplay_MiningResult){ .hxc_inventory = hxc_inventory, .hxc_outcome = 0 };
      }
  }
  struct hxc_caxecraft_gameplay_MiningResult hxc_tmp_call_result_n4 = hxc_caxecraft_gameplay_Mining_collect(hxc_cells, hxc_length, hxc_coordinate, hxc_inventory, hxc_kind, hxc_caxecraft_gameplay_ItemKind_GrassBlock);
  return hxc_tmp_call_result_n4;
}

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_gameplay_Mining_collect(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coordinate, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, enum hxc_caxecraft_domain_BlockKind hxc_kind, enum hxc_caxecraft_gameplay_ItemKind hxc_item)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n5 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n5;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_inventory, hxc_item, 1);
  if (!(hxc_tmp_call_result_n0 != 1))
  {
    bool hxc_tmp_call_result_n2 = hxc_caxecraft_domain_World_remove(hxc_cells, hxc_length, hxc_coordinate);
    if (!!hxc_tmp_call_result_n2)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n4 = hxc_caxecraft_gameplay_Inventory_collectBlock(hxc_inventory, hxc_kind);
      return (struct hxc_caxecraft_gameplay_MiningResult){ .hxc_inventory = hxc_tmp_call_result_n4, .hxc_outcome = 2 };
    }
    return (struct hxc_caxecraft_gameplay_MiningResult){ .hxc_inventory = hxc_inventory, .hxc_outcome = 0 };
  }
  return (struct hxc_caxecraft_gameplay_MiningResult){ .hxc_inventory = hxc_inventory, .hxc_outcome = 1 };
}
